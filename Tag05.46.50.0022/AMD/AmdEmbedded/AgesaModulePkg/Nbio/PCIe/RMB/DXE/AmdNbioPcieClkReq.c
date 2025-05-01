/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <GnbDxio.h>
#include  <Filecode.h>
#include  <Library/IdsLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/GnbPcieConfigLib.h>
#include  <Library/GnbCommonLib.h>
#include  <GnbRegistersRMB.h>
#include  <Library/SmnAccessLib.h>
#include  <Library/OemClkReqControlLib.h>

#define FILECODE NBIO_PCIE_RMB_DXE_AMDNBIOPCIECLKREQ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
// This value is the same for both Matisse and Premium Chipset

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

typedef struct {
  GNB_PCI_SCAN_DATA       ScanData;
  PCI_ADDR                DownstreamPort;
} PCIE_CLKREQ_PMM_DATA;

typedef struct {
  UINT8         ClkReqMask;
  UINT8         ClkReq;
  UINT8         Assigned;
} CLKREQ_MAP_CNTL;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                     L O C A L   D A T A   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


CLKREQ_MAP_CNTL ROMDATA DefaultClkReqMapCntl [] = {
  {1, 0x0, 0},    // CLKREQB_0  Core 0, Port 0
  {1, 0x1, 0},    // CLKREQB_1  Core 0, Port 1
  {1, 0x2, 0},    // CLKREQB_2  Core 0, Port 2
  {1, 0x3, 0},    // CLKREQB_3  Core 0, Port 3
  {1, 0x4, 0},    // CLKREQB_4  Core 0, Dummy port
  {1, 0x5, 0},    // CLKREQB_5  Core 1, Port 0
  {1, 0x6, 0},    // CLKREQB_6  Core 1, Port 1
  {1, 0x7, 0},    // CLKREQB_7  Core 1, Port 2
  {1, 0x8, 0},    // CLKREQB_8  Core 1, Port 3
  {1, 0x9, 0},    // CLKREQB_9  Core 1, Port 4
  {1, 0xA, 0},  // CLKREQB_A  Core 1, Port 5
  {1, 0xB, 0}   // NBIO_CLKREQb_MAP_CNTL & NBIO_CLKREQb_MAP_CNTL2 has 12 pairs of mask + map, the last one should be disabled by setting mask = 1
};

VOID
UpdateClkReqMapCntl ()
{
  UINT8                 Index;
  UINT8                 Index1;

  for (Index = 0; Index < 12; Index++) {
    if (DefaultClkReqMapCntl[Index].ClkReqMask == 0x1 && DefaultClkReqMapCntl[Index].Assigned == 0x1) {
      for (Index1 = 0; Index1 < 12; Index1++) {
        if (DefaultClkReqMapCntl[Index1].ClkReqMask == 0x0 && DefaultClkReqMapCntl[Index1].Assigned == 0) {
          DefaultClkReqMapCntl[Index].ClkReq = Index1;
          DefaultClkReqMapCntl[Index1].Assigned = 0x1;
          break;
        }
      }
    }
  }
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to enable PM L1 SS
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/
VOID
PcieClkReqInterface (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG               *PcieEngine;
  PCIe_WRAPPER_CONFIG              *PcieWrapper;
  UINT32                           ClkReqCounter;
  NBIO_CLKREQb_MAP_CNTL_STRUCT     ClkReqMap;
  NBIO_CLKREQb_MAP_CNTL2_STRUCT    ClkReqMap2;
  UINT32                           Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  ClkReqCounter = 0;
  Index = 0;

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if ((PcieLibIsEngineAllocated (PcieEngine)) &&
          (PcieConfigCheckPortStatus (PcieEngine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {
        if (PcieEngine->Type.Port.ClkReq != 0) {
          if (((PcieEngine->Type.Port.LinkAspmL1_1 == 1) ||
               (PcieEngine->Type.Port.LinkAspmL1_2 == 1)) ||
               PcieEngine->Type.Port.PortData.MiscControls.ClkPmSupport == 1) {
            if (OemGpioClkReqControl (GnbHandle->Address.Address.Bus, PcieEngine->Type.Port.ClkReq, 0) == EFI_UNSUPPORTED) {
              PcieEngine->Type.Port.LinkAspmL1_1 = 0;
              PcieEngine->Type.Port.LinkAspmL1_2 = 0;
              IDS_HDT_CONSOLE (GNB_TRACE, "CLKREQ%d unsupport on %d:%d:%d (L1.1=%d, L1.2=%d, CLKPM=%d))\n",
                               PcieEngine->Type.Port.ClkReq,
                               PcieEngine->Type.Port.Address.Address.Bus,
                               PcieEngine->Type.Port.Address.Address.Device,
                               PcieEngine->Type.Port.Address.Address.Function,
                               PcieEngine->Type.Port.LinkAspmL1_1,
                               PcieEngine->Type.Port.LinkAspmL1_2,
                               PcieEngine->Type.Port.PortData.MiscControls.ClkPmSupport
                               );
            }
          }
          //
          // Only need to assing 7 port, becasue RMB max CLK REQ is 7
          //
          if (ClkReqCounter < 7) {
            Index = PcieEngine->Type.Port.ClkReq - 1;
            DefaultClkReqMapCntl[Index].ClkReqMask = 0;
            DefaultClkReqMapCntl[Index].ClkReq = PcieEngine->Type.Port.CoreId * 5 + PcieEngine->Type.Port.PortId; // Core 0 has 5 ports including the dummy port
            Index = DefaultClkReqMapCntl[Index].ClkReq;
            DefaultClkReqMapCntl[Index].Assigned = 1;
          }
          ClkReqCounter++;
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  UpdateClkReqMapCntl ();

  ClkReqMap.Value = 0;
  ClkReqMap2.Value = 0;

  // MAP 0 ~ 7 should be set to ClkReqMap
  // MASK 0 ~ 7 should be set to ClkReqMap2
  for (Index = 0; Index <= 7; Index++) {
    ClkReqMap.Value |= (DefaultClkReqMapCntl[Index].ClkReq & 0xF) << (Index * 4);
    ClkReqMap2.Value |= (DefaultClkReqMapCntl[Index].ClkReqMask & 1) << (16 + Index);
  }
  // MAP 8 ~ 11 and Mask 8 ~ 11 should be set to ClkReqMap2
  for (Index = 8; Index <= 11; Index++) {
    ClkReqMap2.Value |= (DefaultClkReqMapCntl[Index].ClkReq & 0xF) << ((Index - 8) * 4);
    ClkReqMap2.Value |= (DefaultClkReqMapCntl[Index].ClkReqMask & 1) << (16 + Index);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "NBIO_CLKREQb_MAP_CNTL = 0x%x\n", ClkReqMap.Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "NBIO_CLKREQb_MAP_CNTL2 = 0x%x\n", ClkReqMap2.Value);

  // The SELECT control the RMB_clkreq_mux are connected to the CLKREQb MUX MAP registers of PCIE1 (6x8) which is in VDDP power domain.
  SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_PCIE1_NBIO_CLKREQb_MAP_CNTL_ADDRESS), &ClkReqMap.Value, GNB_REG_ACC_FLAG_S3SAVE);
  SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_PCIE1_NBIO_CLKREQb_MAP_CNTL2_ADDRESS), &ClkReqMap2.Value, GNB_REG_ACC_FLAG_S3SAVE);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

