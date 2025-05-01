/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <AMD.h>
#include <Gnb.h>
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
#include <GnbRegistersRMB.h>

//From AgesaPkg
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieComplexPpi.h>

#include <Guid/GnbPcieInfoHob.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/NbioPcieTrainingPpi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/NbioPcieDpcStatusPpi.h>
#include <Library/AmdHeapLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/NbioHandleLib.h>
#include <Addendum/Apcb/Inc/RMB/APOB.h>
#include <Library/AmdPspApobLib.h>
#include <Library/PcieMiscCommLib.h>
//Dependent on AgesaPkg
#include <Library/DxioLibV2.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/PcieConfigLib.h>

#include "AmdNbioPciePei.h"
#include "PcieStraps.h"
#include <Ppi/SocLogicalIdPpi.h>
#include "PcieComplexData.h"

#define FILECODE        NBIO_PCIE_RMB_PEI_DXIOCFGPOINTS_FILECODE

#define STRAP_BIF_PORT_DIFF                 (HOLD_TRAINING_B_INDEX - HOLD_TRAINING_A_INDEX)

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// Comment this line to enable extended debug output if tracing is enabled
#undef GNB_TRACE_ENABLE

typedef enum  {
  SRIS_CMN = BIT0,
  SRIS_DBG = BIT1,
  SRIS_DBG_PBS = BIT2,
  SRIS_AUTODETECT = BIT3
} SRIS_CFG_TYPE;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
#define DEVFUNC(d, f) ((((UINT8) d) << 3) | ((UINT8) f))
#define NumberOfBridges 16

typedef struct {
  UINT8         DevMap;
  UINT8         LogicalBrId;
  UINT32        InterruptRountingData;
} IOAPIC_BR_INTERRUPT;


typedef struct {
  UINT8         LogicalBrId;
  UINT8         LogicalBrIdAfterRemapNx;
} MAPPING_RESULT_STRUCT;

typedef struct {
  DXIO_PORT_DESCRIPTOR    *PortList;
  UINT32                  StrapSize;
} PCIE_STRAPSIZE_CALLBACK_DATA;

typedef struct {
  VOID                    *StrapList;
  DXIO_PORT_DESCRIPTOR    *PortList;
  UINT32                  StrapSize;
} PCIE_STRAP_CALLBACK_DATA;

typedef struct {
  UINT8                   DsTxPreset;
  UINT8                   UsTxPreset;
  UINT8                   DsRxPresetHint;
  UINT8                   UsRxPresetHint;
} PRESET_SETTINGS;

typedef struct {
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_PM_SUPPORT_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_AER_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_TPH_SUPPORTED;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_16GT_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_MARGINING_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_32GT_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_LTR_SUPPORTED;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ECRC_GEN_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ECRC_CHECK_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_SDP_UNIT_ID;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_STRAP_F0_ATOMIC_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_STRAP_F0_ATOMIC_ROUTING_EN;
  DXIO_STRAPENTRY_LONG_CTRLLER      STRAP_BIF_SUBSYS_ID;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_TARGET_LINK_SPEED_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_L0S_EXIT_LATENCY_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_EXTENDED_FMT_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_E2E_PREFIX_EN_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_INITIAL_N_FTS_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_LOCAL_DLF_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_DLF_EXCHANGE_EN_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_L1_EXIT_LATENCY_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_DPC_EN;
} USB4_CTRLLR_STRAPS;

typedef struct {
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_PM_SUPPORT_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_ASPM_L1_2_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_PCI_PM_L1_2_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_ASPM_L1_1_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_PCI_PM_L1_1_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_CM_RESTORE_TIME_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_T_P_ON_SCALE_A;
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_T_P_ON_VALUE_A;
} PERPORT_STRAPS;

typedef struct {
  DXIO_STRAPENTRY_SHORT_PERPORT     STRAP_BIF_SHORT_PERPORT;
} PERPORT_STRAPS_APPEND;

typedef struct {
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_SDP_UNIT_ID;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_CPL_ABORT_ERR_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_TPH_SUPPORTED;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_SOURCE_VALIDATION;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_TRANSLATION_BLOCKING;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_P2P_REQUEST_REDIRECT;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ACS_UPSTREAM_FORWARDING;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_LTR_SUPPORTED;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_OBFF_SUPPORTED;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_DLF_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_DPC_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_LC_UPCONFIGURE_SUPPORT;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_F0_ATOMIC_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_F0_ATOMIC_ROUTING_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_MARGIN_IGNORE_C_SKP;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_AER_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ECRC_GEN_EN;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_ECRC_CHECK_EN;
  DXIO_STRAPENTRY_LONG_CTRLLER      STRAP_BIF_SUBSYS_ID;
  DXIO_STRAPENTRY_LONG_CTRLLER      STRAP_BIF_SUBSYS_VEN_ID;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_TARGET_LINK_SPEED_A;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_L0S_EXIT_LATENCY_A;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_KILL_GEN5;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_LOCAL_DLF_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_DLF_EXCHANGE_EN_A;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_ALLPORT     STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET;
  DXIO_STRAPENTRY_SHORT_CTRLLER     STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET;
} ALLCTRLR_STRAPS;

/*----------------------------------------------------------------------------------------
 *           I N S T A N T I A T I O N   O F   L O C A L   S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

USB4_CTRLLR_STRAPS USB4CtrllrStraps = {
  SHORT_CTRLLR_STRAP (STRAP_BIF_PM_SUPPORT_A_INDEX, 2),                               // Override from topology
  SHORT_CTRLLR_STRAP (STRAP_BIF_AER_EN_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_TPH_SUPPORTED_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_16GT_EN_INDEX, 0),
  SHORT_CTRLLR_STRAP (STRAP_BIF_MARGINING_EN_INDEX, 0),
  SHORT_CTRLLR_STRAP (STRAP_BIF_32GT_EN_INDEX, 0),
  SHORT_CTRLLR_STRAP (STRAP_BIF_LTR_SUPPORTED_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_ECRC_GEN_EN_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_ECRC_CHECK_EN_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN_INDEX, 0),
  SHORT_CTRLLR_STRAP (STRAP_BIF_SDP_UNIT_ID_INDEX, 0x22),
  SHORT_CTRLLR_STRAP (STRAP_BIF_STRAP_F0_ATOMIC_EN_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_STRAP_F0_ATOMIC_ROUTING_EN_INDEX, 1),
  LONG_CTRLLR_STRAP  (STRAP_BIF_SUBSYS_ID_INDEX, 0x1453),
  SHORT_CTRLLR_STRAP (STRAP_BIF_TARGET_LINK_SPEED_A_INDEX, 0),
  SHORT_CTRLLR_STRAP (STRAP_BIF_L0S_EXIT_LATENCY_A_INDEX, 7),
  SHORT_CTRLLR_STRAP (STRAP_BIF_EXTENDED_FMT_SUPPORTED_A_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_E2E_PREFIX_EN_A_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_INITIAL_N_FTS_A_INDEX, 0xff),
  SHORT_CTRLLR_STRAP (STRAP_BIF_LOCAL_DLF_SUPPORTED_A_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_DLF_EXCHANGE_EN_A_INDEX, 1),
  SHORT_CTRLLR_STRAP (STRAP_BIF_L1_EXIT_LATENCY_A_INDEX, 2),
  SHORT_CTRLLR_STRAP (STRAP_BIF_DPC_EN_INDEX, 0),
};

PERPORT_STRAPS      PerPortStraps = {
  SHORT_PERPORT_STRAP (STRAP_BIF_PM_SUPPORT_A_INDEX, 2),                              // Override from topology
  SHORT_PERPORT_STRAP (STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A_INDEX, 0),               // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_ASPM_L1_2_SUPPORTED_A_INDEX, 0),                     // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_PCI_PM_L1_2_SUPPORTED_A_INDEX, 0),                   // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_ASPM_L1_1_SUPPORTED_A_INDEX, 0),                     // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_PCI_PM_L1_1_SUPPORTED_A_INDEX, 0),                   // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_CM_RESTORE_TIME_A_INDEX, 0),                         // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_T_P_ON_SCALE_A_INDEX, 0),                            // Default to 0, update optional
  SHORT_PERPORT_STRAP (STRAP_BIF_T_P_ON_VALUE_A_INDEX, 0x5)                           // Default to 0x5, update optional
};

ALLCTRLR_STRAPS     AllCtrlrStraps = {
  SHORT_CTRLLR_STRAP (STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN_INDEX, 0),                       // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_INDEX, 3), // Set default 3
  SHORT_CTRLLR_STRAP (STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_INDEX, 3),   // Set default 3
  SHORT_CTRLLR_STRAP (STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_INDEX, 7),      // Set default 7
  SHORT_CTRLLR_STRAP (STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_INDEX, 7),        // Set default 7
  SHORT_CTRLLR_STRAP (STRAP_BIF_SDP_UNIT_ID_INDEX, 0),                                  // Default to 2, update required
  SHORT_CTRLLR_STRAP (STRAP_BIF_CPL_ABORT_ERR_EN_INDEX, 0),                             // Always 0
  SHORT_CTRLLR_STRAP (STRAP_BIF_TPH_SUPPORTED_INDEX, 1),                                // Always 1
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_EN_INDEX, 0),                                       // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_SOURCE_VALIDATION_INDEX, 0),                        // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_TRANSLATION_BLOCKING_INDEX, 0),                     // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P_INDEX, 0),                    // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT_INDEX, 0),                  // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_P2P_REQUEST_REDIRECT_INDEX, 0),                     // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ACS_UPSTREAM_FORWARDING_INDEX, 0),                      // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_LTR_SUPPORTED_INDEX, 0),                                // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_SWUS_OBFF_SUPPORTED_INDEX, 0),                          // Default to 0, update optional 2 - OBFF using WAKE# Supported
  SHORT_CTRLLR_STRAP (STRAP_BIF_DLF_EN_INDEX, 0),                                       // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_DPC_EN_INDEX, 0),                                       // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_LC_UPCONFIGURE_SUPPORT_INDEX, 1),                       // Default to 1, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_STRAP_F0_ATOMIC_EN_INDEX, 0),                           // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_STRAP_F0_ATOMIC_ROUTING_EN_INDEX, 0),                   // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_MARGIN_IGNORE_C_SKP_INDEX, 1),                          // Default to 1, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_AER_EN_INDEX, 0),                                       // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ECRC_GEN_EN_INDEX, 0),                                  // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_ECRC_CHECK_EN_INDEX, 0),                                // Default to 0, update optional
  LONG_CTRLLR_STRAP (STRAP_BIF_SUBSYS_ID_INDEX, 0x1234),                                // Override with PCD
  LONG_CTRLLR_STRAP (STRAP_BIF_SUBSYS_VEN_ID_INDEX, 0x1022),                            // Override with PCD
  SHORT_ALLPORT_STRAP (STRAP_BIF_TARGET_LINK_SPEED_A_INDEX, 0x3),                       // Set all ports to Gen4
  SHORT_ALLPORT_STRAP (STRAP_BIF_L0S_EXIT_LATENCY_A_INDEX, 7),                          // PCI-E specification states L0 Exit latency should be 7
  SHORT_ALLPORT_STRAP (STRAP_BIF_KILL_GEN5_INDEX, 1),                                   // Kill Gen5
  SHORT_ALLPORT_STRAP (STRAP_BIF_LOCAL_DLF_SUPPORTED_A_INDEX, 0),                       // Default to 0, update optional
  SHORT_ALLPORT_STRAP (STRAP_BIF_DLF_EXCHANGE_EN_A_INDEX, 0),                           // Default to 0, update optional
  SHORT_ALLPORT_STRAP (STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A_INDEX, 0),           // Default to 0, update optional
  SHORT_ALLPORT_STRAP (STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A_INDEX, 0),           // Default to 0, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET_INDEX, 3),    // Default to 3, update optional
  SHORT_CTRLLR_STRAP (STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET_INDEX, 1)     // Default to 1, update optional
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */

//=========================================================================================
// Timepoint during Ancillary data calculation
//=========================================================================================


/*----------------------------------------------------------------------------------------*/
/**
 * USB4 Per Engine Callback to count straps to be set
 *
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
DxioCfgUSB4StrapSizeWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  PCIE_STRAPSIZE_CALLBACK_DATA  *StrapSize;
  DXIO_PORT_DESCRIPTOR          *PortPointer;
  BOOLEAN                        IsDxioUsbOverPcie = FALSE;

  StrapSize = (PCIE_STRAPSIZE_CALLBACK_DATA *) Buffer;

  PortPointer = StrapSize->PortList;
  while (PortPointer != NULL) {
    // RMB B0
    if (PortPointer->EngineData.EngineType == DxioUSB_OVER_PCIE) {
      if (IsDxioUsbOverPcie == FALSE) { // USB only exist on 1 Wrapper, once over 2 wrappers as public need to update again.
        IDS_HDT_CONSOLE (GNB_TRACE, "DxioUSB_OVER_PCIE PerPort Rom strap \n");
        StrapSize->StrapSize += sizeof (USB4_CTRLLR_STRAPS);
      }
      IsDxioUsbOverPcie = TRUE;
    }

    PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
  }

  // Only if there's any strap setting with DxioUsbOverPcie, then we should add size of strap subheap here
  if (IsDxioUsbOverPcie != 0) {
    StrapSize->StrapSize += sizeof (DXIO_STRAPSUBHEAP);
  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Wrapper Callback to count append straps to be set
 *
 *
 *
 * @param[in,out] Buffer    StrapSize pointer
 */
VOID
STATIC
DxioCfgAppendStrapSizeWrapperCallback (
  IN OUT  PCIE_STRAPSIZE_CALLBACK_DATA      *StrapSize
  )
{
  if (PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen3)) {
    StrapSize->StrapSize += sizeof (PERPORT_STRAPS_APPEND);
  }
  if (PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen4)) {
    StrapSize->StrapSize += sizeof (PERPORT_STRAPS_APPEND);
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Wrapper Callback to count straps to be set
 *
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
DxioCfgStrapSizeWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  PCIE_STRAPSIZE_CALLBACK_DATA  *StrapSize;
  DXIO_PORT_DESCRIPTOR          *PortPointer;
  LOCATE_HEAP_PTR                LocateHeap;
  AGESA_STATUS                   AgesaStatus;
  UINT32                        *CbsRomstrapSize;


  StrapSize = (PCIE_STRAPSIZE_CALLBACK_DATA *) Buffer;
  StrapSize->StrapSize += sizeof (ALLCTRLR_STRAPS);

  LocateHeap.BufferHandle = AMD_NBIO_PCIE_ALL_PORT_STRAP;
  AgesaStatus = HeapLocateBuffer (&LocateHeap, NULL);
  if (AgesaStatus == AGESA_SUCCESS) {
    CbsRomstrapSize = (UINT32 *) LocateHeap.BufferPtr;
    StrapSize->StrapSize += *CbsRomstrapSize;
  }

  if (Wrapper->WrapId == 0) {
    LocateHeap.BufferHandle = AMD_NBIO_PCIE_PER_PORT_0_STRAP;
  } else {
    LocateHeap.BufferHandle = AMD_NBIO_PCIE_PER_PORT_1_STRAP;
  }
  AgesaStatus = HeapLocateBuffer (&LocateHeap, NULL);
  if (AgesaStatus == AGESA_SUCCESS) {
    CbsRomstrapSize = (UINT32 *) LocateHeap.BufferPtr;
    StrapSize->StrapSize += *CbsRomstrapSize;
  }

  PortPointer = StrapSize->PortList;
  while (PortPointer != NULL) {
    if ((PortPointer->EngineData.StartLane >= Wrapper->StartPhyLane) && (PortPointer->EngineData.StartLane <= Wrapper->EndPhyLane)) {
      if (PortPointer->EngineData.EngineType == DxioPcieEngine) {
        StrapSize->StrapSize += sizeof (PERPORT_STRAPS);
        DxioCfgAppendStrapSizeWrapperCallback (StrapSize);
      }
    }
    PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
  }

  // Only if there's any strap setting, then we should add size of strap subheap here
  if (StrapSize->StrapSize != 0) {
    StrapSize->StrapSize += sizeof (DXIO_STRAPSUBHEAP);
  }

  return;
}

/**----------------------------------------------------------------------------------------*/
/**
 * This function calculates the memory required for ancillary data listing the straps to be set
 *
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  PortPointer    Pointer to the first topology entry
 *
 * @retval    UINT32          Size of ancillary data required to store the strap subheap
 */
 /*----------------------------------------------------------------------------------------*/

UINT32
DxioCfgGetStrapListSize (
  IN       GNB_HANDLE            *GnbHandle,
  IN       DXIO_PORT_DESCRIPTOR  *PortPointer
  )
{
  PCIE_STRAPSIZE_CALLBACK_DATA  StrapSizeData;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  StrapSizeData.StrapSize = 0;
  StrapSizeData.PortList = PortPointer;
  PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, DxioCfgStrapSizeWrapperCallback, (VOID *)&StrapSizeData, GnbHandle);
  //Check for RMB B0 USB4
  PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, DxioCfgUSB4StrapSizeWrapperCallback, (VOID *)&StrapSizeData, GnbHandle);
  // Only if there's any strap setting, then we should add size of strap subheap here
  if (StrapSizeData.StrapSize != 0) {
    StrapSizeData.StrapSize += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit returning 0x%x\n", __FUNCTION__, StrapSizeData.StrapSize);
  return StrapSizeData.StrapSize;
}

//=========================================================================================
// Timepoint during Ancillary data initialization
//=========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 *
 * Add per-port straps to the strap list
 *
 *
 * @param[in]     PortPointer  Pointer to topology entry
 * @param[in]     EngineId     Index into EngineConfig
 * @param[in,out] StrapList    Pointer to a pointer to the end of the strap list
 */
VOID
STATIC
DxioCfgAddPortStrapList (
  IN      DXIO_PORT_DESCRIPTOR       *PortPointer,
  IN      UINT32                     EngineId,
  IN OUT  VOID                       **StrapList
  )
{
  PERPORT_STRAPS      *StrapEntry;
  PERPORT_STRAPS_APPEND   *StrapAppendEntry;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  StrapEntry = (PERPORT_STRAPS *) *StrapList;
  StrapAppendEntry = NULL;
  IDS_HDT_CONSOLE (GNB_TRACE, "  StrapEntry = 0x%x\n", StrapEntry);
  LibAmdMemCopy (*StrapList,
                 (VOID *) &PerPortStraps,
                 sizeof (PERPORT_STRAPS),
                 (AMD_CONFIG_PARAMS *)NULL
                 );

  // Update All EngineIds
  StrapEntry->STRAP_BIF_PM_SUPPORT_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_PM_SUPPORT_A.data = PortPointer->Port.LinkAspm & 0x2;

  StrapEntry->STRAP_BIF_PM_SUPPORT_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_ASPM_L1_2_SUPPORTED_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_PCI_PM_L1_2_SUPPORTED_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_ASPM_L1_1_SUPPORTED_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_PCI_PM_L1_1_SUPPORTED_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_CM_RESTORE_TIME_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_T_P_ON_SCALE_A.engineId = EngineId;
  StrapEntry->STRAP_BIF_T_P_ON_VALUE_A.engineId = EngineId;

  if ((PortPointer->Port.LinkAspmL1_1 == 1) || (PortPointer->Port.LinkAspmL1_2 == 1))  {
    if (PortPointer->Port.LinkAspmL1_2 == 1) {
      StrapEntry->STRAP_BIF_ASPM_L1_2_SUPPORTED_A.data = 1;
      StrapEntry->STRAP_BIF_PCI_PM_L1_2_SUPPORTED_A.data = 1;
    }
    if (PortPointer->Port.LinkAspmL1_1 == 1)  {
      StrapEntry->STRAP_BIF_ASPM_L1_1_SUPPORTED_A.data = 1;
      StrapEntry->STRAP_BIF_PCI_PM_L1_1_SUPPORTED_A.data = 1;
    }
    // STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A_INDEX
    StrapEntry->STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A.data = 1;

    // STRAP_BIF_CM_RESTORE_TIME_A_INDEX
    StrapEntry->STRAP_BIF_CM_RESTORE_TIME_A.data = 0xA;

    // STRAP_BIF_T_P_ON_SCALE_A_INDEX
    StrapEntry->STRAP_BIF_T_P_ON_SCALE_A.data = 1;

    // STRAP_BIF_T_P_ON_VALUE_A_INDEX
    StrapEntry->STRAP_BIF_T_P_ON_VALUE_A.data = 0xF;
  }
  StrapEntry++;
  *StrapList = (VOID *) StrapEntry;
  IDS_HDT_CONSOLE (GNB_TRACE, "  StrapAppendEntry = 0x%x\n", StrapAppendEntry);
  if (PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen3)) {
    StrapAppendEntry = (PERPORT_STRAPS_APPEND *) *StrapList;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.strapType = shortPerPortStrap;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.strapIdx = STRAP_BIF_LC_PRESET_MASK_8GT_A_INDEX;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.engineId = EngineId;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.data = PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen3);
    StrapAppendEntry++;
    *StrapList = (VOID *) StrapAppendEntry;
  }
  if (PcdGet32(PcdCfgPCIeCntlLcPresetMaskGen4)) {
    StrapAppendEntry = (PERPORT_STRAPS_APPEND *) *StrapList;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.strapType = shortPerPortStrap;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.strapIdx = STRAP_BIF_LC_PRESET_MASK_16GT_A_INDEX;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.engineId = EngineId;
    StrapAppendEntry->STRAP_BIF_SHORT_PERPORT.data = PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen4);
    StrapAppendEntry++;
    *StrapList = (VOID *) StrapAppendEntry;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "  Exit StrapEntry = 0x%x\n", StrapEntry);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * USB4 Wrapper Callback for wrapper strap settings
 *
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
DxioCfgStrapListUSB4WrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  PCIE_STRAP_CALLBACK_DATA      *StrapData;
  DXIO_STRAPSUBHEAP             *StrapSubHeap;
  USB4_CTRLLR_STRAPS            *StrapEntry;
  UINT8                         *LastStrap;
  UINT32                        LocalStrapCount;
  DXIO_PORT_DESCRIPTOR          *PortPointer;
  BOOLEAN                       IsDxioUsbOverPcie = FALSE;

  StrapData = (PCIE_STRAP_CALLBACK_DATA *) Buffer;

  PortPointer = StrapData->PortList;
  while (PortPointer != NULL) {
    switch (PortPointer->EngineData.EngineType) {
    case DxioUSB_OVER_PCIE:
      if (IsDxioUsbOverPcie == FALSE) { // USB only exist on 1 Wrapper, once over 2 wrappers as public need to update again.
        StrapSubHeap = (DXIO_STRAPSUBHEAP *) StrapData->StrapList;
        StrapEntry = (USB4_CTRLLR_STRAPS *) ((UINT8 *) (StrapData->StrapList) + sizeof (DXIO_STRAPSUBHEAP));

        LibAmdMemCopy ((VOID *) StrapEntry,
                      (VOID *) (&USB4CtrllrStraps),
                      sizeof (USB4_CTRLLR_STRAPS),
                      (AMD_CONFIG_PARAMS *)NULL
                      );

        // Update SDP UNIT ID for PCIE2/3
        StrapEntry->STRAP_BIF_SDP_UNIT_ID.data = (Wrapper->WrapId == 0) ? 0x22 : 0x23;

        LocalStrapCount = sizeof (DXIO_STRAPSUBHEAP);
        LocalStrapCount += sizeof (USB4_CTRLLR_STRAPS);

        IDS_HDT_CONSOLE (GNB_TRACE, "USB4 Wrapper->WrapId = 0x%x\n", Wrapper->WrapId);

        LastStrap = (UINT8 *) (StrapEntry + 1);

        StrapSubHeap->numDwords = LocalStrapCount >> 2;
        StrapSubHeap->ctrllerId = (Wrapper->WrapId) + 2;
        StrapSubHeap->reserved = 0;

        StrapData->StrapList = LastStrap;
        StrapData->StrapSize += LocalStrapCount;
        IDS_HDT_CONSOLE (GNB_TRACE, "At EXIT StrapSize = 0x%x\n", StrapData->StrapSize);
        IsDxioUsbOverPcie = TRUE;
      }
      break;
    default:
      break;
    }
    PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
  }

  return;
}
/*----------------------------------------------------------------------------------------*/
/**
 * Per-Wrapper Callback for wrapper strap settings
 *
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
DxioCfgStrapListWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  PCIE_STRAP_CALLBACK_DATA      *StrapData;
  DXIO_STRAPSUBHEAP             *StrapSubHeap;
  ALLCTRLR_STRAPS               *StrapEntry;
  UINT8                         *LastStrap;
  UINT32                        EngineId;
  UINT32                        LocalStrapCount;
  UINT32                        SubsystemDeviceId;
  UINT32                        SubsystemVendorId;
  DXIO_PORT_DESCRIPTOR          *PortPointer;
  LOCATE_HEAP_PTR                LocateHeap;
  UINT32                        *CbsRomstrapSize;
  AGESA_STATUS                   AgesaStatus;

  StrapData = (PCIE_STRAP_CALLBACK_DATA *) Buffer;

  StrapSubHeap = (DXIO_STRAPSUBHEAP *) StrapData->StrapList;
  StrapEntry = (ALLCTRLR_STRAPS *) ((UINT8 *) (StrapData->StrapList) + sizeof (DXIO_STRAPSUBHEAP));

  LibAmdMemCopy ((VOID *) StrapEntry,
                 (VOID *) &AllCtrlrStraps,
                 sizeof (ALLCTRLR_STRAPS),
                 (AMD_CONFIG_PARAMS *)NULL
                 );

  LocalStrapCount = sizeof (DXIO_STRAPSUBHEAP);
  LocalStrapCount += sizeof (ALLCTRLR_STRAPS);

  // SDP Unit ID
  StrapEntry->STRAP_BIF_SDP_UNIT_ID.data = (Wrapper->WrapId == 0) ? 0x10 : 0x19;

  // ACS Enablement
  if (PcdGetBool (PcdCfgACSEnable)) {
    StrapEntry->STRAP_BIF_ACS_EN.data = 1;
    StrapEntry->STRAP_BIF_ACS_SOURCE_VALIDATION.data = 1;
    StrapEntry->STRAP_BIF_ACS_TRANSLATION_BLOCKING.data = 1;
    StrapEntry->STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P.data = 1;
    StrapEntry->STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT.data = 1;
    StrapEntry->STRAP_BIF_ACS_P2P_REQUEST_REDIRECT.data = 1;
    StrapEntry->STRAP_BIF_ACS_UPSTREAM_FORWARDING.data = 1;
  }

  // STRAP_BIF_LTR_SUPPORTED_INDEX;
  if (PcdGetBool (PcdCfgPCIeLTREnable)) {
    StrapEntry->STRAP_BIF_LTR_SUPPORTED.data = 1;
  }

  if (PcdGet8 (PcdPcieOBFF) == TRUE) {
    StrapEntry->STRAP_BIF_OBFF_SUPPORTED.data = 0x2;
  }

  // Data Link Feature Extended Capability
  if (PcdGetBool (PcdAmdDlfCapEn)) {
    StrapEntry->STRAP_BIF_DLF_EN.data = 1;
    StrapEntry->STRAP_BIF_LOCAL_DLF_SUPPORTED_A.data = 1;
  }

  // Data Link Feature Exchange Enable
  if (PcdGetBool (PcdAmdDlfExEn)) {
    StrapEntry->STRAP_BIF_DLF_EXCHANGE_EN_A.data = 1;
  }

  // Atomic
  if ((PcdGet8 (PcdGppAtomicOps) == 1) || (PcdGet8 (PcdGfxAtomicOps) == 1)) {
    StrapEntry->STRAP_F0_ATOMIC_EN.data = 1;
    StrapEntry->STRAP_F0_ATOMIC_ROUTING_EN.data = 1;
  }

  // MARGIN_IGNORE_C_SKP
  if (PcdGet8 (PcdPcieCoreMarginIgnoreCSkip) == 0) {
    StrapEntry->STRAP_BIF_MARGIN_IGNORE_C_SKP.data = 0;
  }

  // AER
  if (PcdGetBool (PcdCfgAEREnable)) {
    StrapEntry->STRAP_BIF_AER_EN.data = 1;
    StrapEntry->STRAP_BIF_CPL_ABORT_ERR_EN.data = 1;
  }

  // ECRC
  if (PcdGetBool (PcdPcieEcrcEnablement) || PcdGetBool (PcdCfgAEREnable)) {
    StrapEntry->STRAP_BIF_ECRC_GEN_EN.data = 1;
    StrapEntry->STRAP_BIF_ECRC_CHECK_EN.data = 1;
  }

  // 10-Bit TAG
  if (PcdGetBool (PcdCfgPcieTbtSupport)) {
    StrapEntry->STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A.data = 1;
    StrapEntry->STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A.data = 1;
    if (FALSE == PcdGetBool (PcdCfgTbtCompleterEn)) {
      StrapEntry->STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A.data = 0;
    }
    if (FALSE == PcdGetBool (PcdCfgTbtRequesterEn)) {
      StrapEntry->STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A.data = 0;
    }
  }

  // PCIE Subsystem
  SubsystemDeviceId = (UINT32) PcdGet16 (PcdAmdPcieSubsystemDeviceID);
  if (SubsystemDeviceId != 0) {
    StrapEntry->STRAP_BIF_SUBSYS_ID.data = SubsystemDeviceId;
  }
  SubsystemVendorId = (UINT32) PcdGet16 (PcdAmdPcieSubsystemVendorID);
  if (SubsystemVendorId != 0) {
    StrapEntry->STRAP_BIF_SUBSYS_VEN_ID.data = SubsystemVendorId;
  }

  if (PcdGet8 (PcdPcieGen4LaneEqDsTxPreset) != 0xFF) {
    StrapEntry->STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET.data = PcdGet8 (PcdPcieGen4LaneEqDsTxPreset);
  }

  if (PcdGet8 (PcdPcieGen4LaneEqUsTxPreset) != 0xFF) {
    StrapEntry->STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET.data = PcdGet8 (PcdPcieGen4LaneEqUsTxPreset);
  }

  if (PcdGet8 (PcdPcieGen3LaneEqDsTxPreset) != 0xFF) {
    StrapEntry->STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET.data = PcdGet8 (PcdPcieGen3LaneEqDsTxPreset);
  }

  if (PcdGet8 (PcdPcieGen3LaneEqUsTxPreset) != 0xFF) {
    StrapEntry->STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET.data = PcdGet8 (PcdPcieGen3LaneEqUsTxPreset);
  }

  LastStrap = (UINT8 *) (StrapEntry + 1);

  // copy all port setting for CBS to Ancillary Data
  LocateHeap.BufferHandle = AMD_NBIO_PCIE_ALL_PORT_STRAP;
  AgesaStatus = HeapLocateBuffer (&LocateHeap, NULL);
  if (AgesaStatus == AGESA_SUCCESS) {
    CbsRomstrapSize = (UINT32 *) LocateHeap.BufferPtr;
    if (*CbsRomstrapSize != 0) {
      LibAmdMemCopy ((VOID *) LastStrap,
                    (VOID *) (CbsRomstrapSize + 1),
                    *CbsRomstrapSize,
                    NULL
                    );
      LocalStrapCount += *CbsRomstrapSize;
      LastStrap += *CbsRomstrapSize;
    }
  }

  EngineId = 0;
  PortPointer = StrapData->PortList;
  while (PortPointer != NULL) {
    switch (PortPointer->EngineData.EngineType) {
    case DxioPcieEngine:
      if ((PortPointer->EngineData.StartLane >= Wrapper->StartPhyLane) && (PortPointer->EngineData.StartLane <= Wrapper->EndPhyLane)) {
        DxioCfgAddPortStrapList (PortPointer, EngineId, (VOID *) &LastStrap);
        LocalStrapCount += sizeof (PERPORT_STRAPS);
        if (PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen3)) {
          LocalStrapCount += sizeof (PERPORT_STRAPS_APPEND);
        }
        if (PcdGet32 (PcdCfgPCIeCntlLcPresetMaskGen4)) {
          LocalStrapCount += sizeof (PERPORT_STRAPS_APPEND);
        }
      }
      EngineId++;
      break;
    case DxioSATAEngine:
      EngineId++;
      break;
    default:
      break;
    }
    PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
  }

  // copy per port setting for CBS to Ancillary Data
  if (Wrapper->WrapId == 0) {
    LocateHeap.BufferHandle = AMD_NBIO_PCIE_PER_PORT_0_STRAP;
  } else {
    LocateHeap.BufferHandle = AMD_NBIO_PCIE_PER_PORT_1_STRAP;
  }
  AgesaStatus = HeapLocateBuffer (&LocateHeap, NULL);
  if (AgesaStatus == AGESA_SUCCESS) {
    CbsRomstrapSize = (UINT32 *) LocateHeap.BufferPtr;
    if (*CbsRomstrapSize != 0) {
      LibAmdMemCopy ((VOID *) LastStrap,
                    (VOID *) (CbsRomstrapSize + 1),
                    *CbsRomstrapSize,
                    NULL
                    );
      LocalStrapCount += *CbsRomstrapSize;
      LastStrap += *CbsRomstrapSize;
    }
  }

  StrapSubHeap->numDwords = LocalStrapCount >> 2;
  StrapSubHeap->ctrllerId = Wrapper->WrapId;
  StrapSubHeap->reserved = 0;

  StrapData->StrapList = LastStrap;
  StrapData->StrapSize += LocalStrapCount;
  IDS_HDT_CONSOLE (GNB_TRACE, "At EXIT StrapSize = 0x%x\n", StrapData->StrapSize);
  return;
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to build a list of straps to be written by DXIO firmware.
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  AncillaryData  Pointer to the next available entry in Ancillary Data for this node
 * @param[in]  PortList       Pointer to the topology structures for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/

UINT32
DxioCfgAddStrapList (
  IN       GNB_HANDLE             *GnbHandle,
  IN       VOID                   *AncillaryData,
  IN       DXIO_PORT_DESCRIPTOR   *PortList
  )
{
  PCIE_STRAP_CALLBACK_DATA      StrapData;
  DXIO_ANCILLARYDATA_SUBHEAP    *SubHeap;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter with Ancillary Data Pointer 0x%x\n", __FUNCTION__, AncillaryData);

  SubHeap = (DXIO_ANCILLARYDATA_SUBHEAP *) (AncillaryData);

  StrapData.PortList = PortList;
  StrapData.StrapList = (VOID *) ((UINT8 *) AncillaryData + sizeof (DXIO_ANCILLARYDATA_SUBHEAP));
  StrapData.StrapSize = sizeof (DXIO_ANCILLARYDATA_SUBHEAP);

  PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, DxioCfgStrapListWrapperCallback, (VOID *)&StrapData, GnbHandle);
  // USB4
  PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, DxioCfgStrapListUSB4WrapperCallback, (VOID *)&StrapData, GnbHandle);

  SubHeap->descriptorType = DXIO_ANCDATA_SUBHEAPTYPE_PCIESTRAP;
  SubHeap->version = DXIO_ANCILLARYDATA_SUBHEAP_VERSION;
  SubHeap->reserved1 = 0;
  SubHeap->numDWORDs = StrapData.StrapSize >> 2;
  SubHeap->reserved2 = 0;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit returning 0x%x\n", __FUNCTION__, StrapData.StrapSize);
  return StrapData.StrapSize;
}

//=========================================================================================
// Timepoint after INIT_INIT to allow APU-specific DXIO requests
//=========================================================================================
/**
 * RMB specific - Support configuration of PCIe lanes that may be mapped to either controller
 *
 * @param GnbHandle         Descriptor handle
 * @param SmuServicesPpi    Instances of Smu Services Protocol
 * @param InstanceId        Instance ID
 */
VOID
DxioCfgAfterInitInit (
  IN       GNB_HANDLE                     *GnbHandle,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *SmuServicesPpi,
  IN       UINT8                          InstanceId
  )
{
  S0I3_GPIO_RST_EP     *GpioReset = NULL;
  // APU Specific Config for now
  DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_MAP_TO_WHICH_CONTROLLER, PcdGet8 (PcdApuLaneRouteConfig), 0, 0, 0);

  // DXIO CLK REFCLK Shutdown 28
  if (PcdGetBool(PcdCfgDxioRefClkShutDown)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_REFCLK_SHUTDOWN, 1, 0, 0, 0);
    //
    // These 2 feature only can enable when CLK Refclk shutdown as true.
    //
    // DXIO PMA Power Gating 33
    if (PcdGetBool(PcdCfgDxioPmaPowerGating)) {
      DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_PMA_POWER_GATING, 1, 0, 0, 0);
    }
    // DXIO PMA Clock Gating 34
    if (PcdGetBool(PcdCfgDxioPmaClockGating)) {
      DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_PMA_CLOCK_GATING, 1, 0, 0, 0);
    }
  }

  // DXIO CLK Cating 26
  if (PcdGetBool(PcdCfgDxioClockGating)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_CLK_GATING, 1, 0, 0, 0);
  }
  // DXIO Static PWR Gating 27
  if (PcdGetBool(PcdCfgDxioStaticPowerGating)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_STATIC_PWR_GATING, 1, 0, 0, 0);
  }

  if (PcdGetBool(PcdCfgDxioPCIeRSTGenericReset)) {
    if (PcdGet32 (PcdCfgDxioPCIeGPIOResetEP1) != 0) {
      GpioReset = (S0I3_GPIO_RST_EP*) PcdGet32 (PcdCfgDxioPCIeGPIOResetEP1);
      DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH,
        DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_GPIO_GENERIC_RESET,
        GpioReset->AssertionSeq,
        GpioReset->Address, GpioReset->DeassertionValue,
        GpioReset->AssertionValue);
    }
    if (PcdGet32 (PcdCfgDxioPCIeGPIOResetEP2) != 0) {
      GpioReset = (S0I3_GPIO_RST_EP*) PcdGet32 (PcdCfgDxioPCIeGPIOResetEP2);
      DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH,
        DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_GPIO_EP1_RESET,
        GpioReset->AssertionSeq,
        GpioReset->Address, GpioReset->DeassertionValue,
        GpioReset->AssertionValue);
    }
    if (PcdGet32 (PcdCfgDxioPCIeGPIOResetEP3) != 0) {
      GpioReset = (S0I3_GPIO_RST_EP*) PcdGet32 (PcdCfgDxioPCIeGPIOResetEP3);
      DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH,
        DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_GPIO_EP2_RESET,
        GpioReset->AssertionSeq,
        GpioReset->Address, GpioReset->DeassertionValue,
        GpioReset->AssertionValue);
    }
  }

  // PCIE_ALLOW_COMPLETION_PASS
  if (PcdGetBool (PcdCfgDxioAllowCompPass)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PCIE_ALLOW_COMPLETION_PASS, 1, 0, 0, 0);
    }
  else {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PCIE_ALLOW_COMPLETION_PASS, 0, 0, 0, 0);
    }

  // Allow Pointer Slip Interval
  if (PcdGetBool (PcdCfgDxioAllowPointerSlipInterval)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_CBSOPTIONS_ALLOWPOINTERSLIPINTERVAL, 1, PcdGet8 (PcdCfgDxioAllowPointerSlipIntervalRange), 0, 0);
  }

  if (PcdGet32 (PcdCfgDxioSataGen1Settings)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_SATA_Gen1_Settings, PcdGet32 (PcdCfgDxioSataGen1Settings), 0, 0, 0);
  }
  if (PcdGet32 (PcdCfgDxioSataGen2Settings)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_SATA_Gen2_Settings, PcdGet32 (PcdCfgDxioSataGen2Settings), 0, 0, 0);
  }
  if (PcdGet32 (PcdCfgDxioSataGen3Settings)) {
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_SATA_Gen3_Settings, PcdGet32 (PcdCfgDxioSataGen3Settings), 0, 0, 0);
  }

  return;
}


//=========================================================================================
// Timepoint after port mapping and before reconfig
//=========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Engine Callback for port configuration before bifurcation
 *
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
DxioCfgBeforeReconfigCallback (
  IN      PCIe_ENGINE_CONFIG                *Engine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  UINT8            DynLanesPwrState;
  GNB_HANDLE      *GnbHandle;

  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Wrapper);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  Engine->Type.Port.EqPreset = Engine->Type.Port.EqSearchMode;
  if (PcieLibIsEngineAllocated(Engine)) {

    // These are all programmed per engine
    // Gen3 Bypass Phase3 EQ
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS),
      (UINT32) ~(PCIE_LC_EQ_CNTL_8GT_LC_BYPASS_EQ_REQ_PHASE_8GT_MASK),
      Engine->Type.Port.BypassGen3EQ << PCIE_LC_EQ_CNTL_8GT_LC_BYPASS_EQ_REQ_PHASE_8GT_OFFSET,
      0
      );

    // Gen3 Bypass Phase2/3 EQ
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS),
      (UINT32) ~(PCIE_LC_EQ_CNTL_8GT_LC_USC_EQ_NOT_REQD_8GT_MASK),
      Engine->Type.Port.DisGen3EQPhase << PCIE_LC_EQ_CNTL_8GT_LC_USC_EQ_NOT_REQD_8GT_OFFSET,
      0
      );

    // Gen4 Bypass Phase3 EQ
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS),
      (UINT32) ~(PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_REQ_PHASE_16GT_MASK),
      Engine->Type.Port.BypassGen4EQ << PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_REQ_PHASE_16GT_OFFSET,
      0
      );

    // Gen4 Bypass Phase2/3 EQ
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS),
      (UINT32) ~(PCIE_LC_EQ_CNTL_16GT_LC_USC_EQ_NOT_REQD_16GT_MASK),
      Engine->Type.Port.DisGen4EQPhase << PCIE_LC_EQ_CNTL_16GT_LC_USC_EQ_NOT_REQD_16GT_OFFSET,
      0
      );

    Engine->Type.Port.EqPreset = Engine->Type.Port.EqSearchMode;

    IDS_HDT_CONSOLE (GNB_TRACE, "addr: <%08x>, eqpre: <%d>\n",
        GnbHandle->Address.AddressValue,
        Engine->Type.Port.EqPreset);


    // Gen3 EQ Search Mode
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS),
      (UINT32) ~(PCIE_LC_EQ_CNTL_8GT_LC_EQ_SEARCH_MODE_8GT_MASK),
      Engine->Type.Port.EqSearchMode << PCIE_LC_EQ_CNTL_8GT_LC_EQ_SEARCH_MODE_8GT_OFFSET,
      0
      );

    // Gen4 EQ Search Mode
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS),
      (UINT32) ~(PCIE_LC_EQ_CNTL_16GT_LC_EQ_SEARCH_MODE_16GT_MASK),
      Engine->Type.Port.EqSearchModeGen4 << PCIE_LC_EQ_CNTL_16GT_LC_EQ_SEARCH_MODE_16GT_OFFSET,
      0
      );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL9_ADDRESS),
                    (UINT32) ~(PCIE_LC_CNTL9_LC_LOOPBACK_WAIT_FOR_ALL_ACTIVE_LANES_MASK),
                    (((PcdGetBool (PcdAmdLcLoopbackWaitForAllActiveLanes) == TRUE) ? 1 : 0)\
                      << PCIE_LC_CNTL9_LC_LOOPBACK_WAIT_FOR_ALL_ACTIVE_LANES_OFFSET),
                    0
                    );
    // Set PCIE_TX_CNTL::TX_CPL_PASS_P
    if (PcdGetBool (PcdCfgDxioAllowCompPass)) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS),
                  (UINT32) ~(PCIE_TX_PORT_CTRL_1_TX_CPL_PASS_P_MASK),
                  (0x1 << PCIE_TX_PORT_CTRL_1_TX_CPL_PASS_P_OFFSET),
                  0
                  );
    }

    // LC Dynamic Lanes Power State
    DynLanesPwrState = (Engine->Type.Port.PortFeatures.DynLanesPwrState != 0xFF) ? Engine->Type.Port.PortFeatures.DynLanesPwrState : PcdGet8 (PcdCfgDynamicLanesPowerState);
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS),
                  (UINT32) ~(PCIE_LC_LINK_WIDTH_CNTL_LC_DYN_LANES_PWR_STATE_MASK | PCIE_LC_LINK_WIDTH_CNTL_LC_TURN_OFF_UNUSED_LANES_MASK),
                  ((DynLanesPwrState & 0x3) << PCIE_LC_LINK_WIDTH_CNTL_LC_DYN_LANES_PWR_STATE_OFFSET) |
                  ((Engine->Type.Port.PortFeatures.TurnOffUnusedLanes & 0x1) << PCIE_LC_LINK_WIDTH_CNTL_LC_TURN_OFF_UNUSED_LANES_OFFSET),
                  0
                  );

    if (Engine->Type.Port.SrisEnableMode == 1) {
      IDS_HDT_CONSOLE (GNB_TRACE, "SRIS Config:\n");
      IDS_HDT_CONSOLE (GNB_TRACE, "  - PcdSrisCfgType       = 0x%x\n", PcdGet8 (PcdSrisCfgType));
      IDS_HDT_CONSOLE (GNB_TRACE, "  - SrisEnableMode       = 0x%x\n", Engine->Type.Port.SrisEnableMode);
      IDS_HDT_CONSOLE (GNB_TRACE, "  - SrisSkipInterval     = 0x%x\n", Engine->Type.Port.SrisSkipInterval);
      IDS_HDT_CONSOLE (GNB_TRACE, "  - LowerSkpOsGenSup     = 0x%x\n", Engine->Type.Port.LowerSkpOsGenSup);
      IDS_HDT_CONSOLE (GNB_TRACE, "  - LowerSkpOsRcvSup     = 0x%x\n", Engine->Type.Port.LowerSkpOsRcvSup);
      
      WritePcieStrap (
          GnbHandle,
          (STRAP_BIF_LC_SRIS_EN_A_INDEX + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
          ((Engine->Type.Port.SrisAutoDetectMode == 1)? 0: 1),
          Wrapper->WrapId
          );
    }
    if (Engine->Type.Port.SrisAutoDetectMode == 1) {
      IDS_HDT_CONSOLE (GNB_TRACE, "SRIS AutoDetect:\n");
      IDS_HDT_CONSOLE (GNB_TRACE, "  - SrisAutoDetectMode   = 0x%x\n", Engine->Type.Port.SrisAutoDetectMode);
      IDS_HDT_CONSOLE (GNB_TRACE, "  - SrisSkpIntervalSel   = 0x%x\n", Engine->Type.Port.SrisSkpIntervalSel);
      IDS_HDT_CONSOLE (GNB_TRACE, "  - SrisAutodetectFactor = 0x%x\n", Engine->Type.Port.SrisAutodetectFactor);
      WritePcieStrap (
          GnbHandle,
          (STRAP_BIF_LC_SRIS_AUTODETECT_EN_A_INDEX + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
          0x1,
          Wrapper->WrapId
          );
    }

  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Wrapper Callback for wrapper configuration before bifurcation
 *
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
DxioCfgBeforeReconfigWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  UINT16                              Index;

  PcieConfigRunProcForAllEnginesInWrapper (DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE, DxioCfgBeforeReconfigCallback, NULL, Wrapper);
  // ACS Enablement
  if (PcdGetBool (PcdCfgACSEnable)) {
    for (Index = STRAP_BIF_ACS_EN_INDEX; Index <= STRAP_BIF_ACS_UPSTREAM_FORWARDING_INDEX; Index++) {
      WritePcieStrap (
        GnbHandle,
        (UINT16) Index,
        1,
        Wrapper->WrapId
        );
    }
  }

  return;
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure DXIO/PCIe ports after ports are mapped and before reconfig
 *  - This function is called once for each socket
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/

VOID
DxioCfgBeforeReconfig (
  IN       GNB_HANDLE       *GnbHandle
  )
{
  GNB_HANDLE                *LocalHandle;
  UINT32                    SocketId;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a for Socket %d\n", __FUNCTION__, GnbHandle->SocketId);

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      IDS_HOOK(IDS_HOOK_NBIO_PCIE_TUNING, (VOID *)LocalHandle, (VOID *)NULL);
      PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, DxioCfgBeforeReconfigWrapperCallback, NULL, LocalHandle);

    }
    LocalHandle = GnbGetNextHandle (LocalHandle);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return;
}


//=========================================================================================
// Timepoint after reconfig and before port training
//=========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Engine Callback for configuration after bifurcation
 *
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
DxioCfgAfterReconfigCallback (
  IN      PCIe_ENGINE_CONFIG                *Engine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  GNB_HANDLE                          *GnbHandle;
  UINT32                              Value32;
  PCIe_DPC_STATUS_DATA                *DpcStatusData;

  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Wrapper);

  if (Buffer != NULL) {
    DpcStatusData = (PCIe_DPC_STATUS_DATA*) Buffer;
    SmnRegisterRead (GnbHandle->Address.Address.Bus,
                     PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_DPC_STATUS_ADDRESS),
                     &Value32
                     );
    GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE, "SocketId=%d, DieNumber=%d, BRIndex=%d Bus=0x%x CoreID=%d PortID=%d PCIE_DPC_STATUS(0x388) = 0x%x\n",
                     GnbHandle->SocketId,
                     GnbHandle->DieNumber,
                     GnbHandle->RBIndex,
                     GnbHandle->Address.Address.Bus,
                     Wrapper->WrapId,
                     (Engine->Type.Port.PortId % 8),
                     Value32
                     ));
    if (Value32 & BIT0) {
      if (DpcStatusData->size < MAX_NUMBER_DPCSTATUS) {
        DpcStatusData->DpcStatusArray[DpcStatusData->size].SocketId = (UINT8) GnbHandle->SocketId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DieID = (UINT8) GnbHandle->DieNumber;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].RBIndex = (UINT8) GnbHandle->RBIndex;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].BusNumber = (UINT8) GnbHandle->Address.Address.Bus;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIeCoreID = (UINT8) Wrapper->WrapId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIePortID = (UINT8) (Engine->Type.Port.PortId % 8);
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DpcStatus = (UINT16) Value32;

        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_DPC_STATUS_ADDRESS),
                          &Value32,
                          0
                          );
        DpcStatusData->size++;
      }
    }
  }

  if (PcieLibIsEngineAllocated(Engine)) {
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS),
      (UINT32) ~(PCIE_TX_PORT_CTRL_1_TX_FLUSH_TLP_DIS_MASK),
      0 << PCIE_TX_PORT_CTRL_1_TX_FLUSH_TLP_DIS_OFFSET,
      0
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Wrapper Callback for configuration after bifurcation
 *
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
DxioCfgAfterReconfigWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  PcieConfigRunProcForAllEnginesInWrapper (DESCRIPTOR_ALL_ENGINES, DxioCfgAfterReconfigCallback, Buffer, Wrapper);
  if (PcdGetBool(PcdCfgNbioCTOtoSC)) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE (GnbHandle, Wrapper, SMN_PCIE0_PCIE_CI_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_CI_CNTL_RX_RCB_RC_CTO_TO_SC_IN_LINK_DOWN_EN_MASK),
                    1 << PCIE_CI_CNTL_RX_RCB_RC_CTO_TO_SC_IN_LINK_DOWN_EN_OFFSET,
                    0
                    );
  }

  if (PcdGetBool(PcdCfgNbioCTOIgnoreError)) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE (GnbHandle, Wrapper, SMN_PCIE0_PCIE_CI_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_CI_CNTL_RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN_MASK),
                    (UINT32)  (1 << PCIE_CI_CNTL_RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN_OFFSET),
                    0
                    );
  }

  if (PcdGetBool(PcdAmdRxMarginEnabled)) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_RXMARGIN_CONTROL_CAPABILITIES_ADDRESS),
                    (UINT32) ~(PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDLEFTRIGHTTIMING_MASK),
                    (0x1 << PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDLEFTRIGHTTIMING_OFFSET),
                    0
                    );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_RXMARGIN_1_SETTINGS_ADDRESS),
                    (UINT32) ~(PCIE_RXMARGIN_1_SETTINGS_M_NUMTIMINGSTEPS_MASK | PCIE_RXMARGIN_1_SETTINGS_M_MAXTIMINGOFFSET_MASK),
                    (0x11 << PCIE_RXMARGIN_1_SETTINGS_M_NUMTIMINGSTEPS_OFFSET) |
                    (0x32 << PCIE_RXMARGIN_1_SETTINGS_M_MAXTIMINGOFFSET_OFFSET),
                    0
                    );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_RXMARGIN_2_SETTINGS_ADDRESS),
                    (UINT32) ~(PCIE_RXMARGIN_2_SETTINGS_M_MAXLANES_MASK),
                    (0xF << PCIE_RXMARGIN_2_SETTINGS_M_MAXLANES_OFFSET),
                    0
                    );
  }

  return;
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure DXIO/PCIe ports after reconfig and before training
 *  - This function is called once for each socket
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/

VOID
DxioCfgAfterReconfig (
  IN       GNB_HANDLE       *GnbHandle
  )
{
  EFI_STATUS                      Status;
  PCIe_DPC_STATUS_DATA            *DpcStatusData;
  PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI *DpcStatusPpi;
  CONST EFI_PEI_SERVICES          **PeiServices;
  PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi;
  UINT32                          *CfgDxioNoTrainLinkDevFunc;
  UINT32                          Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a for Socket %d\n", __FUNCTION__, GnbHandle->SocketId);
  DpcStatusData = NULL;
  DpcStatusPpi = NULL;
  PeiServices = GetPeiServicesTablePointer();
  Index = 0;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioPcieDpcStatusPpiGuid, 0, NULL, &DpcStatusPpi);
  if (!EFI_ERROR (Status)) {
    DpcStatusPpi->GetDpcStatus (DpcStatusPpi, (PCIe_DPC_STATUS_DATA **)&DpcStatusData);
  }

  PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, DxioCfgAfterReconfigWrapperCallback, DpcStatusData, GnbHandle);

  // Skip Pcie Training S0i3 Exit
  if (PcdGet32 (PcdCfgDxioNoTrainLinkDevFunc) != 0) {
    CfgDxioNoTrainLinkDevFunc = (UINT32*)PcdGet32 (PcdCfgDxioNoTrainLinkDevFunc);
    Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &SmuServicesPpi);
    if (!EFI_ERROR (Status)) {
      while ((CfgDxioNoTrainLinkDevFunc[Index] != 0xFFFFFFFF) && (Index < (NUMBER_OF_GPP0_PORTS + NUMBER_OF_GFX_PORTS))) {
        DxioLibServiceRequest (SmuServicesPpi, GnbHandle->InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, 0x1, 0, 0x3, 0x1, CfgDxioNoTrainLinkDevFunc[Index]);
        Index++;
      }
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return;
}

//=========================================================================================
// Timepoint before DXIO firmware intialization begins
//=========================================================================================

/**----------------------------------------------------------------------------------------*/
/**
 * Configuration Timepoint before DXIO firmware initialization starts
 *
 *
 *
 * @param[in]  Pcie                 Pointer silicon complex descriptor
 * @param[in]  ComplexDescriptor    Pointer to platform complex descriptor
 *
 /*----------------------------------------------------------------------------------------*/


VOID
DxioCfgBeforeDxioInit (
  IN      PCIe_PLATFORM_CONFIG      *Pcie,
  IN      DXIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor
  )
{
  GNB_HANDLE                    *GnbHandle;
  DXIO_COMPLEX_DESCRIPTOR       *LocalDescriptor;
  DXIO_ANCILLARYPORTDATA_PSPP   PsppParams;
  DXIO_ANCILLARYPORTDATA_PSPP_2 PsppParams2;

  GnbHandle = NbioGetHandle (Pcie);
  Pcie->PsppPolicy = PcdGet8 (PcdPsppPolicy);
  PsppParams.Value = 0;
  PsppParams.Field.lowBWThreshold = PsppThreshold100Percent;
  PsppParams.Field.HighBWThreshold = PsppThreshold95Percent;
  PsppParams.Field.lowBWThresholdG2 =  PsppThreshold20Percent;
  PsppParams.Field.HighBWThresholdG2 = PsppThreshold95Percent;
  PsppParams.Field.lowBWThresholdG3 = PsppThreshold20Percent;
  PsppParams.Field.HighBWThresholdG3 = PsppThreshold95Percent;
  PsppParams.Field.BWHintCount = 3;
  Pcie->PsppTuningParams = PsppParams.Value;

  PsppParams2.Value = 0;
  PsppParams2.Field.lowBWThresholdG4 = PsppThreshold50Percent;
  PsppParams2.Field.HighBWThresholdG4 = PsppThreshold0Percent;
  Pcie->PsppTuningParams2 = PsppParams2.Value;

  IDS_HOOK (IDS_HOOK_NBIO_PSPP_TUNING, (VOID *)NULL, (VOID *)Pcie);

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "%a for Socket %d\n", __FUNCTION__, LocalDescriptor->SocketId);
    IDS_HOOK(IDS_HOOK_NBIO_PCIE_USER_CONFIG, (VOID *)GnbHandle, (VOID *)LocalDescriptor);
    DxioTopologyWorkarounds (LocalDescriptor);
    DxioManageTopology (LocalDescriptor);

    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
  }
  return;
}

//=========================================================================================
// Timepoint after DXIO firmware initialization completes
//=========================================================================================


/**----------------------------------------------------------------------------------------*/
/**
 * PCIE interface to configure register setting after Dxio init done
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/

VOID
SubsystemIdSetting (
  IN       GNB_HANDLE       *GnbHandle
  )
{
  UINT32      Value;

  IDS_HDT_CONSOLE (GNB_TRACE,
                   "%a Enter for Socket %d Nbio %d\n",
                   __FUNCTION__,
                   GnbHandle->SocketId,
                   GnbHandle->RBIndex
                   );

  // NB ADAPTER D0F0
  Value = PcdGet32 (PcdCfgNbioSsid);
  if (Value != 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcdCfgNbioSsid = %x\n", Value);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_NB_ADAPTER_ID_W_ADDRESS), &Value, 0);
  }

  // IOMMU
  Value = PcdGet32 (PcdCfgIommuSsid);
  if (Value != 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcdCfgIommuSsid = %x\n", Value);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_IOMMU_ADAPTER_ID_W_ADDRESS), &Value, 0);
  }

  // NBIF Root Bridge Functions
  Value = PcdGet32 (PcdCfgNbifRCSsid);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcdCfgNbifRCSsid = %x\n", Value);
  if (Value != 0) {
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP1_ADDRESS), &Value, 0);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP1_ADDRESS), &Value, 0);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP1_ADDRESS), &Value, 0);
  }

  // GFX
  Value = PcdGet32 (PcdAmdCfgGnbIGPUSSID);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcdAmdCfgGnbIGPUSSID = %x\n", Value);
  if (Value != 0) {
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC0_NBIF0_ADAPTER_ID_W_ADDRESS), &Value, 0);
  }

  // GFX Audio SSID
  Value = PcdGet32 (PcdAmdCfgGnbIGPUAudioSSID);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcdAmdCfgGnbIGPUAudioSSID = %x\n", Value);
  if (Value != 0) {
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC1_NBIF0_ADAPTER_ID_W_ADDRESS), &Value, 0);
  }

  // PSPCCP  - F2
  Value = PcdGet32 (PcdCfgPspccpSsid);
  if (Value != 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcdCfgPspccpSsid = %x\n", Value);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC2_NBIF0_ADAPTER_ID_W_ADDRESS), &Value, 0);
  }

  if (PcdGetBool (PcdAcpController)) {
    Value = PcdGet32 (PcdCfgAcpSsid);
    if (Value != 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcdCfgAcpSsid = %x\n", Value);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC5_NBIF0_ADAPTER_ID_W_ADDRESS ), &Value, 0);
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return;
}

/**----------------------------------------------------------------------------------------*/
/**
 * PCIE interface to configure  setting after Dxio init done
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

STATIC
VOID
GetCcdInfo (
    IN       UINT32  *CcdBitfield
  )
{
  UINT32 ApobInstanceId;
  UINT32 Socket;
  UINT32 Index;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  APOB_TYPE_HEADER                              *ApobEntry;

  Socket = 0;
  ApobInstanceId = Socket;
  AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, &ApobEntry);
  CopyMem (&ApobCcdLogToPhysMap, ApobEntry, sizeof (APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT));

  for (Index = 0;Index < CCD_MAX_CCDS_PER_DIE; Index++) {
    if (ApobCcdLogToPhysMap.CcdMap[Index].PhysCcdNumber != CCX_NOT_PRESENT) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Ccd: %d\n",
          ApobCcdLogToPhysMap.CcdMap[Index].PhysCcdNumber);
      *CcdBitfield |= (1 << ApobCcdLogToPhysMap.CcdMap[Index].PhysCcdNumber);
    } else {
      break;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * After Pcie Training Enumerate all Pcie connectors for register setting.
 *
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
InitBusRanges (
  IN     GNB_HANDLE                           *GnbHandle,
  IN     PCIe_PLATFORM_CONFIG                 *Pcie,
  IN     AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServicesPpi

  )
{
  GNB_HANDLE                      *InnerHandle;
  PCIE_VDM_NODE_CTRL4_STRUCT      VdmNode;
  UINTN                           SystemFabricID;
  UINTN                           BusNumberBase;
  UINTN                           BusNumberLimit;
  UINT32                          VdmNodeAddress;

  GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE,
                   "%a Enter for Socket %d Nbio %d\n",
                   __FUNCTION__,
                   GnbHandle->SocketId,
                   GnbHandle->RBIndex
                   ));

  InnerHandle = NbioGetHandle (Pcie);
  while (InnerHandle != NULL) {
    FabricTopologyServicesPpi->GetRootBridgeInfo (InnerHandle->SocketId,
                                          InnerHandle->DieNumber,
                                          InnerHandle->RBIndex,
                                          &SystemFabricID,
                                          &BusNumberBase,
                                          &BusNumberLimit,
                                          NULL,
                                          NULL,
                                          NULL
                                          );
    VdmNode.Field.BUS_RANGE_BASE = BusNumberBase;
    VdmNode.Field.BUS_RANGE_LIMIT = BusNumberLimit;
    if (InnerHandle->InstanceId == GnbHandle->InstanceId) {
      VdmNode.Field.NODE_PRESENT = 0;
    } else {
      VdmNode.Field.NODE_PRESENT = 1;
    }
    VdmNodeAddress = SMN_N0_PCIE_VDM_NODE_CTRL4_ADDRESS + (InnerHandle->InstanceId * 0x14);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      NBIO_SPACE(GnbHandle, VdmNodeAddress),
                      &VdmNode.Value,
                      0
                      );
    InnerHandle = GnbGetNextHandle (InnerHandle);
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
AGESA_STATUS
STATIC
PcieCommonCoreConfigurationCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE                *GnbHandle;
  UINT32                    Value;


  if (Wrapper->WrapId != 0xFF) {
    GnbHandle = (GNB_HANDLE *)PcieConfigGetParentSilicon(Wrapper);
    GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE,
                     "%a Enter for Socket %d Nbio %d Wrapper %d\n",
                     __FUNCTION__,
                     GnbHandle->SocketId,
                     GnbHandle->RBIndex,
                     Wrapper->WrapId
                     ));

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE (GnbHandle, Wrapper, SMN_PCIE0_PCIE_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_CNTL_RX_RCB_INVALID_SIZE_DIS_MASK |
                               PCIE_CNTL_RX_RCB_WRONG_ATTR_DIS_MASK),
                    (0x0 << PCIE_CNTL_RX_RCB_INVALID_SIZE_DIS_OFFSET |
                     0x1 << PCIE_CNTL_RX_RCB_WRONG_ATTR_DIS_OFFSET),
                    0
                    );


    if ( PcdGetBool (PcdAmdNbioReportEdbErrors) ) {
      Value = 0;
    } else {
      Value = 1;
    }
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE (GnbHandle, Wrapper, SMN_PCIE0_PCIE_P_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_P_CNTL_P_IGNORE_EDB_ERR_MASK |
                               PCIE_P_CNTL_P_ELEC_IDLE_MODE_MASK),
                    Value << PCIE_P_CNTL_P_IGNORE_EDB_ERR_OFFSET |
                    0x1 << PCIE_P_CNTL_P_ELEC_IDLE_MODE_OFFSET,
                    0
                    );
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to dump PCIe LC STATE
 *
 *
 *
 * @param[in]     Engine    PCIe Engine info
 */
VOID DumpEnginePcieLcState (
  IN      PCIe_ENGINE_CONFIG    *Engine
  )
{
  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  PCIE_LC_STATE0_STRUCT       PcieLCState;
  UINT32                      index;

  Wrapper = PcieConfigGetParentWrapper(Engine);
  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
  IDS_HDT_CONSOLE (GNB_TRACE,
                  "Core%d Port%d[B%d:D%d:F%d]:\n",
                  Wrapper->WrapId,
                  Engine->Type.Port.PortId,
                  GnbHandle->Address.Address.Bus,
                  Engine->Type.Port.PortData.DeviceNumber,
                  Engine->Type.Port.PortData.FunctionNumber
                  );
  for (index=0; index <= 5; index++) {
    SmnRegisterRead (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_STATE0_ADDRESS + index * 4),
                    &PcieLCState.Value
                    );
    IDS_HDT_CONSOLE (GNB_TRACE, "  LC_STATE%d = 0x%08x\n",
                     index,
                     PcieLCState.Value
                     );
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
AGESA_STATUS
STATIC
PcieCommonEngineConfigurationCallback (
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  PCIE_TX_REQUESTER_ID_STRUCT TxRequesterId;
  UINT32                      Value32;

  Wrapper = PcieConfigGetParentWrapper(Engine);
  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
  DumpEnginePcieLcState (Engine);
  if ((Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
       ((Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) &&
        (Engine->Type.Port.PortData.LinkHotplug != HotplugInboard))) {

    GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE,
                     "%a Enter for Socket %d Nbio %d Wrapper %d Engine\n",
                     __FUNCTION__,
                     GnbHandle->SocketId,
                     GnbHandle->RBIndex,
                     Wrapper->WrapId,
                     Engine->Type.Port.PortId
                     ));

    SmnRegisterRead (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS),
      &TxRequesterId.Value
      );

    TxRequesterId.Field.TX_REQUESTER_ID_FUNCTION = Engine->Type.Port.PortData.FunctionNumber;
    TxRequesterId.Field.TX_REQUESTER_ID_DEVICE = Engine->Type.Port.PortData.DeviceNumber;
    TxRequesterId.Field.TX_REQUESTER_ID_BUS = GnbHandle->Address.Address.Bus;

    SmnRegisterWrite (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS),
      &TxRequesterId.Value,
      GNB_REG_ACC_FLAG_S3SAVE
      );

    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_SLOT_CAP_ADDRESS),
      (UINT32) ~(SLOT_CAP_SLOT_PWR_LIMIT_VALUE_MASK),
      (Engine->Type.Port.SlotPowerLimit << SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET),
      0
      );

    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_SLOT_CAP_ADDRESS),
      (UINT32) ~(SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK),
      (Engine->Type.Port.SlotPowerLimitScale << SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET),
      0
      );

    // Set slot_implemented
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_CAP_LIST_ADDRESS),
                    (UINT32) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK << 16),
                    (1 << (PCIE_CAP_SLOT_IMPLEMENTED_OFFSET + 16)),
                    0
                    );

    // Set Physical Slot Number
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_SLOT_CAP_ADDRESS),
                    (UINT32) ~(SLOT_CAP_PHYSICAL_SLOT_NUM_MASK),
                    (Engine->Type.Port.PortData.SlotNum << SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET),
                    0
                    );

    // Set Completion Timeout
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_DEVICE_CNTL2_ADDRESS),
                    (UINT32) ~(DEVICE_CNTL2_CPL_TIMEOUT_VALUE_MASK),
                    (0x6 << DEVICE_CNTL2_CPL_TIMEOUT_VALUE_OFFSET),
                    0
                    );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIEP_STRAP_MISC_ADDRESS),
                    (UINT32) ~(PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_MASK |
                               PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_MASK),
                    (1 << PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_OFFSET) |
                    (1 << PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_OFFSET),
                    0
                    );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_MASK |
                               PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_MASK),
                    (1 << PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_OFFSET) |
                    (0 << PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_OFFSET),
                    0
                    );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_LC_LINK_WIDTH_CNTL_LC_DUAL_END_RECONFIG_EN_MASK),
                    (0x1 << PCIE_LC_LINK_WIDTH_CNTL_LC_DUAL_END_RECONFIG_EN_OFFSET),
                    0
                    );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL2_ADDRESS),
                    (UINT32) ~(PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_MASK |
                               PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L1_MASK |
                               PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L23_MASK),
                    (0x1 << PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_OFFSET) |
                    (0x1 << PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L1_OFFSET) |
                    (0x1 << PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L23_OFFSET),
                    0
                    );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL3_ADDRESS),
                    (UINT32) ~(PCIE_LC_CNTL3_LC_LINK_DOWN_SPD_CHG_EN_MASK),
                    (0x1 << PCIE_LC_CNTL3_LC_LINK_DOWN_SPD_CHG_EN_OFFSET),
                    0
                    );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_TRAINING_CNTL_ADDRESS),
                    (UINT32) ~(PCIE_LC_TRAINING_CNTL_LC_L0S_L1_TRAINING_CNTL_EN_MASK),
                    1 << PCIE_LC_TRAINING_CNTL_LC_L0S_L1_TRAINING_CNTL_EN_OFFSET,
                    0
                    );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIEP_HW_DEBUG_ADDRESS),
                    (UINT32) ~(PCIEP_HW_DEBUG_LC_HW_15_DEBUG_LC_MASK),
                    (0x1 << PCIEP_HW_DEBUG_LC_HW_15_DEBUG_LC_OFFSET),
                    0
                    );
    // Set Tx Margin
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_LINK_CNTL2_ADDRESS),
                    (UINT32) ~(LINK_CNTL2_XMIT_MARGIN_MASK),
                    (Engine->Type.Port.TXMargin << 7),
                    0
                    );

    // We should program straps instead of SMN_FUNC0_PCIE0_PCIE_LC_CNTL6_ADDRESS
    // So comments out below code, and if strap register works, we will remove below code

    // SmnRegisterRMW (GnbHandle->Address.Address.Bus,
    //                 PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL6_ADDRESS),
    //                 (UINT32) ~(PCIE_LC_CNTL6_LC_SPC_MODE_2P5GT_MASK | PCIE_LC_CNTL6_LC_SPC_MODE_5GT_MASK | PCIE_LC_CNTL6_LC_SPC_MODE_8GT_MASK | PCIE_LC_CNTL6_LC_SPC_MODE_16GT_MASK),
    //                 (Engine->Type.Port.SpcMode.SpcGen1 << PCIE_LC_CNTL6_LC_SPC_MODE_2P5GT_OFFSET) |
    //                 (Engine->Type.Port.SpcMode.SpcGen2 << PCIE_LC_CNTL6_LC_SPC_MODE_5GT_OFFSET) |
    //                 (Engine->Type.Port.SpcMode.SpcGen3 << PCIE_LC_CNTL6_LC_SPC_MODE_8GT_OFFSET) |
    //                 (Engine->Type.Port.SpcMode.SpcGen4 << PCIE_LC_CNTL6_LC_SPC_MODE_16GT_OFFSET),
    //                 0
    //                 );

    // Set Powerdown state in L1/L1.1/L1.2
    if (Engine->Type.Port.PortFeatures.L1PowerDown != 0xFF) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL4_ADDRESS),
                  (UINT32) ~(PCIE_LC_CNTL4_LC_L1_POWERDOWN_MASK),
                  ((Engine->Type.Port.PortFeatures.L1PowerDown & 1) << PCIE_LC_CNTL4_LC_L1_POWERDOWN_OFFSET),
                  0
                  );
    }
    if (Engine->Type.Port.PortFeatures.L11PowerDown != 0xFF) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS),
                  (UINT32) ~(PCIE_LC_L1_PM_SUBSTATE_LC_L1_1_POWERDOWN_MASK),
                  ((Engine->Type.Port.PortFeatures.L11PowerDown & 7) << PCIE_LC_L1_PM_SUBSTATE_LC_L1_1_POWERDOWN_OFFSET),
                  0
                  );
    }
    if (Engine->Type.Port.PortFeatures.L12PowerDown != 0xFF) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS),
                  (UINT32) ~(PCIE_LC_L1_PM_SUBSTATE_LC_L1_2_POWERDOWN_MASK),
                  ((Engine->Type.Port.PortFeatures.L12PowerDown & 7) << PCIE_LC_L1_PM_SUBSTATE_LC_L1_2_POWERDOWN_OFFSET),
                  0
                  );
    }

    if (Engine->Type.Port.SrisEnableMode == 1 || Engine->Type.Port.SrisAutoDetectMode == 1) {
      SmnRegisterRead (
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS),
        &Value32
        );

      IDS_HDT_CONSOLE (GNB_TRACE, "PCIE_LC_SPEED_CNTL = 0x%x\n", Value32);
      Value32 = ((Value32 & PCIE_LC_SPEED_CNTL_LC_GEN5_EN_STRAP_MASK)? 0x1F :\
                ((Value32 & PCIE_LC_SPEED_CNTL_LC_GEN4_EN_STRAP_MASK)? 0xF :\
                ((Value32 & PCIE_LC_SPEED_CNTL_LC_GEN3_EN_STRAP_MASK)? 0x7 :\
                ((Value32 & PCIE_LC_SPEED_CNTL_LC_GEN2_EN_STRAP_MASK)? 0x3 : 0x1))));
      IDS_HDT_CONSOLE (GNB_TRACE, "set STRAP_LOWER_SKP_OS_GEN_SUPPORT = 0x%x\n", Value32);
      WritePcieStrap (
          GnbHandle,
          (STRAP_LOWER_SKP_OS_GEN_SUPPORT_A_INDEX + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
          ((Engine->Type.Port.SrisAutoDetectMode != 1 && (PcdGet8 (PcdSrisCfgType) & (SRIS_DBG | SRIS_DBG_PBS)))?
           ((Engine->Type.Port.LowerSkpOsGenSup <= Value32)? Engine->Type.Port.LowerSkpOsGenSup : Value32) : 0),
          Wrapper->WrapId
          );
      WritePcieStrap (
          GnbHandle,
          (STRAP_LOWER_SKP_OS_RCV_SUPPORT_A_INDEX + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
          ((Engine->Type.Port.SrisAutoDetectMode != 1 && (PcdGet8 (PcdSrisCfgType) & (SRIS_DBG | SRIS_DBG_PBS)))?
           ((Engine->Type.Port.LowerSkpOsRcvSup <= Value32)? Engine->Type.Port.LowerSkpOsRcvSup : Value32) : 0),
          Wrapper->WrapId
          );
      SmnRegisterRead (
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL6_ADDRESS),
        &Value32
        );

      if (Engine->Type.Port.SrisAutoDetectMode == 1) {
        // Set AutoDetect Mode & Autodetection factor
        Value32 &= ~(PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_MODE_MASK | PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_FACTOR_MASK);
        Value32 |= (Engine->Type.Port.SrisSkpIntervalSel << PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_MODE_OFFSET) |\
                   (Engine->Type.Port.SrisAutodetectFactor << PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_FACTOR_OFFSET) |\
                   (1 << PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_EN_OFFSET);
      } else {
        // Set SRIS Skip Interval
        if (Engine->Type.Port.SrisEnableMode == 1 && (PcdGet8 (PcdSrisCfgType) & (SRIS_DBG | SRIS_DBG_PBS))) {
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIEP_STRAP_LC_ADDRESS),
                  (UINT32) ~(PCIEP_STRAP_LC_STRAP_SKIP_INTERVAL_MASK),
                  (Engine->Type.Port.SrisSkipInterval << PCIEP_STRAP_LC_STRAP_SKIP_INTERVAL_OFFSET),
                  0
                  );
        }
        // SRIS
        if (Engine->Type.Port.SrisEnableMode == 1) {
          Value32 |= (1 << PCIE_LC_CNTL6_LC_SRIS_EN_OFFSET);
        }
      }

    SmnRegisterWrite (
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL6_ADDRESS),
      &Value32,
      0
      );
    }

    // Program before DXIO training for compatibility issue
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL3_ADDRESS),
                    (UINT32) ~(PCIE_LC_CNTL3_LC_MULT_AUTO_SPD_CHG_ON_LAST_RATE_MASK),
                    (0x1 << PCIE_LC_CNTL3_LC_MULT_AUTO_SPD_CHG_ON_LAST_RATE_OFFSET),
                    0
                    );

    if (Engine->Type.Port.PortFeatures.AutoSpdChngEn != 0xFF) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_SPEED_CNTL2_ADDRESS),
                  (UINT32) ~(PCIE_LC_SPEED_CNTL2_LC_MULT_UPSTREAM_AUTO_SPD_CHNG_EN_MASK),
                  ((Engine->Type.Port.PortFeatures.AutoSpdChngEn & 1) << PCIE_LC_SPEED_CNTL2_LC_MULT_UPSTREAM_AUTO_SPD_CHNG_EN_OFFSET),
                  0
                  );
    }

  }

  return AGESA_SUCCESS;
}

/**----------------------------------------------------------------------------------------*/
/**
 * Identify unused wrappers and mark them as unused
 *
 *
 *
 * @param[in]  Pcie                 Pointer silicon complex descriptor
 *
 /*----------------------------------------------------------------------------------------*/
AGESA_STATUS
STATIC
PortPresenceCallback (
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  UINT8                 *WrapperNumbers;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  Wrapper = PcieConfigGetParentWrapper(Engine);
  WrapperNumbers = (UINT8 *) Buffer;
  if (Wrapper->WrapId < 2) {
    WrapperNumbers[Wrapper->WrapId] = Wrapper->WrapId;
  }
  return AGESA_SUCCESS;
}
AGESA_STATUS
STATIC
WrapperPresenceCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                 *WrapperNumbers;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  WrapperNumbers = (UINT8 *) Buffer;
  if (Wrapper->WrapId < 2) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Setting WrapId %d to 0x%x\n", Wrapper->WrapId, WrapperNumbers[Wrapper->WrapId]);
    Wrapper->WrapId = WrapperNumbers[Wrapper->WrapId];
  }
  return AGESA_SUCCESS;
}
VOID
DxioCfgHideUnusedWrappers (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie
  )
{
  UINT8          WrapperNumbers[2];
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  WrapperNumbers[0] = 0xFF;
  WrapperNumbers[1] = 0xFF;
  PcieConfigRunProcForAllEngines (DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE, PortPresenceCallback, WrapperNumbers, Pcie);
  PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, WrapperPresenceCallback, WrapperNumbers, Pcie);
}

VOID
UpdateInputList(
  IN   GNB_HANDLE  *GnbHandle,
  IN   UINT8       *InputList
  )
{
  UINT8   RemapIndex;
  UINT32  Value;

  for (RemapIndex = 0;RemapIndex < NumberOfBridges; RemapIndex++) {
    SmnRegisterRead (
      GnbHandle->Address.Address.Bus,
      SMN_N0_NB_PROG_DEVICE_REMAP_ADDRESS + (RemapIndex * 4),
      &Value
      );
    InputList[RemapIndex] = (UINT8)Value;
//    IDS_HDT_CONSOLE (GNB_TRACE, "input_list[%d] %x \n",RemapIndex, InputList[RemapIndex]);
  }
}

VOID
IoapicRoutingProcess(
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT8  DeviceIndex;
  UINT8  DetectIndex;
  UINT8  ProgramIndex;

  IOAPIC_BR_INTERRUPT  Map [NumberOfBridges] = {
    DEVFUNC (1, 1),  0,       0,
    DEVFUNC (1, 2),  1,       1,
    DEVFUNC (1, 3),  2,       2,
    DEVFUNC (1, 4),  3, 0x10003,
    DEVFUNC (1, 5),  4,       4,
    DEVFUNC (2, 1),  5, 0x10005,
    DEVFUNC (2, 2),  6, 0x10005,
    DEVFUNC (2, 3),  7, 0x10025,
    DEVFUNC (2, 4),  8, 0x10025,
    DEVFUNC (2, 5),  9, 0x10024,
    DEVFUNC (2, 6), 10, 0x10023,
    DEVFUNC (3, 1), 11, 0x40000,
    DEVFUNC (4, 1), 12, 0x40000,
    DEVFUNC (8, 1), 13, 0x20022,
    DEVFUNC (8, 2), 14, 0x20021,
    DEVFUNC (8, 3), 15, 0x20020
  };

  UINT8  PhysicalBridgeIdRemapList [NumberOfBridges] = {
          0, 1, 2, 3, 4,           // Core0
          7, 8, 9, 10, 11, 12,     // Core1
          5, 6,                    // Core2/3
          13, 14, 15};             // nbif0/1/2

  // Need to get input list from remap registers (default table)
  UINT8  InputList[NumberOfBridges] = {0x9, 0xA, 0xC, 0xD, 0x15, 0x14, 0xB, 0x11, 0x13, 0x12, 0x16, 0x19, 0x21, 0x41, 0x42, 0x43};

  UINT8  PhysicalBridgeIdAfterRemap;
  UINT8  Temp;
  UINT8  LogicalBridgeId;
  MAPPING_RESULT_STRUCT MappingResult[NumberOfBridges];
  UINT8  RemapNxCounter = 0;
  UINT8  BrIdCounter = 0;

  UpdateInputList (GnbHandle, InputList);

  for (DeviceIndex = 0; DeviceIndex < NumberOfBridges; DeviceIndex++) {
    PhysicalBridgeIdAfterRemap = PhysicalBridgeIdRemapList[DeviceIndex];
    // read the remap offset to get the real input
    Temp = InputList[PhysicalBridgeIdAfterRemap];
    // found logicId
    LogicalBridgeId = InputList[DeviceIndex];

    //Build logic data
    //compare map and input list to know which SMN Nx need to program
    for (DetectIndex = 0; DetectIndex < NumberOfBridges; DetectIndex++) {
      if (Temp == Map[DetectIndex].DevMap) {
        MappingResult[RemapNxCounter].LogicalBrIdAfterRemapNx = Map[DetectIndex].LogicalBrId;
        RemapNxCounter++;
        //DetectIndex found the final logic Nx registers
//        IDS_HDT_CONSOLE (GNB_TRACE, "DetectIndex:%d  Map[DetectIndex].LogicalBridgeId N%d \n", DetectIndex, Map[DetectIndex].LogicalBrId);
      }

      if (LogicalBridgeId == Map[DetectIndex].DevMap) {
        MappingResult[BrIdCounter].LogicalBrId = Map[DetectIndex].LogicalBrId;
        BrIdCounter++;
//        IDS_HDT_CONSOLE (GNB_TRACE, "DetectIndex:%d MappingResult[DetectIndex].LogicalBridgeId: %x \n",DetectIndex, MappingResult[DetectIndex].LogicalBrId);
      }

    }

  }

  // Program interrput routing
  for (ProgramIndex = 0; ProgramIndex < NumberOfBridges; ProgramIndex++) {
    SmnRegisterWrite (
      GnbHandle->Address.Address.Bus,
      SMN_N0_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS + (MappingResult[ProgramIndex].LogicalBrIdAfterRemapNx * 4),
      &Map[MappingResult[ProgramIndex].LogicalBrId].InterruptRountingData,
      0
      );
  }

}

/**----------------------------------------------------------------------------------------*/
/**
 * Configuration Timepoint after DXIO firmware initialization completes B0
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 *
 /*----------------------------------------------------------------------------------------*/

VOID
AfterDxioInitB0  (
  IN       GNB_HANDLE  *GnbHandle
  )
{

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  // ASPM L1 and PM L1
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_FUNC0_PCIE2_PCIE_LC_CNTL_ADDRESS),
                  (UINT32) ~(PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_MASK |
                             PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_MASK),
                  (0x1 << PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_OFFSET) |
                  (0x0 << PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_OFFSET),
                  0
                  );
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_FUNC0_PCIE3_PCIE_LC_CNTL_ADDRESS),
                  (UINT32) ~(PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_MASK |
                             PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_MASK),
                  (0x1 << PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_OFFSET) |
                  (0x0 << PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_OFFSET),
                  0
                  );

  // USB4 RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_PCIE2_PCIE_CI_CNTL_ADDRESS),
                  (UINT32) ~(PCIE_CI_CNTL_RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN_MASK),
                  BIT31,
                  0
                  );
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_PCIE3_PCIE_CI_CNTL_ADDRESS),
                  (UINT32) ~(PCIE_CI_CNTL_RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN_MASK),
                  BIT31,
                  0
                  );

  //Update IOAPIC routing
  IoapicRoutingProcess (GnbHandle);

  return;
}
/**----------------------------------------------------------------------------------------*/
/**
 * Configuration Timepoint after DXIO firmware initialization completes
 *
 *
 *
 * @param[in]  Pcie                 Pointer silicon complex descriptor
 *
 /*----------------------------------------------------------------------------------------*/


VOID
DxioCfgAfterDxioInit (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie,
  IN     AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServicesPpi

  )
{
  GNB_HANDLE                      *GnbHandle;
  UINT32                          PackageType;
  AMD_PEI_SOC_LOGICAL_ID_PPI      *SocLogicalIdPpi;
  EFI_STATUS                      Status;
  CONST EFI_PEI_SERVICES          **PeiServices;
  SOC_LOGICAL_ID                  LogicalId;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  PackageType = LibAmdGetPackageType ((AMD_CONFIG_PARAMS *) NULL);

  PcieConfigDebugDump(Pcie);
  DxioCfgHideUnusedWrappers (Pcie);
  PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PcieCommonCoreConfigurationCallback, NULL, Pcie);
  PcieConfigRunProcForAllEngines (DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE, PcieCommonEngineConfigurationCallback, NULL, Pcie);

  GnbHandle = NbioGetHandle (Pcie);
  InitBusRanges (GnbHandle, Pcie, FabricTopologyServicesPpi);
  SubsystemIdSetting (GnbHandle);
  // Initialize ARI
  if (TRUE == PcdGetBool (PcdCfgPcieAriSupport)) {
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, SMN_IOHC_FEATURE_CNTL_ADDRESS),
      (UINT32)~(IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK),
      1 << IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET,
      0
      );
    PcieAriInit (GnbHandle);
  }
  IDS_HOOK(IDS_HOOK_NBIO_AFTER_DXIOINIT, (VOID *)NbioGetHandle (Pcie), (VOID *)NULL);

  PeiServices = GetPeiServicesTablePointer();

  // B0 releated setting
  // Get Logical CPU ID info
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdSocLogicalIdPpiGuid,
                             0,
                             NULL,
                             &SocLogicalIdPpi
                             );
  Status = SocLogicalIdPpi->GetLogicalIdOnCurrentCore (&LogicalId);
  if ((LogicalId.Revision & AMD_REV_F19_RMB_Ax) == 0) {
    AfterDxioInitB0 (GnbHandle);
  }

  return;
}

