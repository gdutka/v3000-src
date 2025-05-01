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
#include <Filecode.h>
#include <PiPei.h>
#include <AmdPcieComplex.h>
#include <GnbDxio.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Library/BaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/GnbLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/MpioInitLib.h>
#include <Library/GnbPciAccLib.h>
#include "MpioLibLocal.h"

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOEARLYTRAIN_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to process early train
 *
 *
 *
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 * @param[in]       ThisNode        Pointer to the platform descriptor for this node
 * @param[in]       ThisNodeSize    size of the platform descriptor for this node
 * @param[in]       EarlyTrainData  Pointer to early train data
 * @param[in]       SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 *
 */
VOID
DxioProcessEarlyTrain (
  IN       GNB_HANDLE                      *GnbHandle,
  IN       DXIO_PLATFORM_DESCRIPTOR        *ThisNode,
  IN       UINT32                          ThisNodeSize,
  IN       EARLY_TRAIN_DATA                *EarlyTrainData,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi
  )
{
  UINT32                    Value;
  UINT16                    Value16;
  BOOLEAN                   ChipsetLocated;
  PCI_ADDR                  NbioAddress;

  NbioAddress = NbioGetHostPciAddress (GnbHandle);

  GnbLibPciRead (EarlyTrainData->EarlyTrainAddress.AddressValue | 0x6A, AccessWidth16, &Value16, (AMD_CONFIG_PARAMS *) NULL);
  if ((Value16 & (1 << 13)) != 0) {
    EarlyTrainData->EarlyTrainAddress.AddressValue |= NbioAddress.AddressValue;
    GnbLibPciRMW (EarlyTrainData->EarlyTrainAddress.AddressValue | 0x18, AccessWidth32, 0xFF0000FF, 0x020200, (AMD_CONFIG_PARAMS *) NULL);
    DxioLibServiceRequest (SmuServicesPpi, GnbHandle->SocketId, DXIO_MSG_INIT_DMAENGINECONIFGTODRAM, 0, (UINT32) ThisNode, ThisNodeSize, 0, 0);

    GnbLibPciRead (MAKE_SBDFO (0, 2, 0, 0, 0), AccessWidth32, &Value, NULL);
    if ((Value & 0xFFFF) == 0x10B5) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Downstream switch is present\n");
      GnbLibPciRmw (MAKE_SBDFO (0, 2, 0, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x070300, (AMD_CONFIG_PARAMS *) NULL);

      // Program downstream bus numbers for bridges on D1-D3
      GnbLibPciRead (MAKE_SBDFO (0, 3, 1, 0, 0), AccessWidth32, &Value, NULL);
      if ((Value & 0xFFFF) == 0x10B5) {
        GnbLibPciRead (MAKE_SBDFO (0, 3, 1, 0, 0x08), AccessWidth32, &Value, NULL);
        if (((Value >> 24) & 0xFF) == 0x06) {
          GnbLibPciRmw (MAKE_SBDFO (0, 3, 1, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x050500, (AMD_CONFIG_PARAMS *) NULL);
        }
      }
      GnbLibPciRead (MAKE_SBDFO (0, 3, 2, 0, 0), AccessWidth32, &Value, NULL);
      if ((Value & 0xFFFF) == 0x10B5) {
        GnbLibPciRead (MAKE_SBDFO (0, 3, 2, 0, 0x08), AccessWidth32, &Value, NULL);
        if (((Value >> 24) & 0xFF) == 0x06) {
          GnbLibPciRmw (MAKE_SBDFO (0, 3, 2, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x060600, (AMD_CONFIG_PARAMS *) NULL);
        }
      }
      GnbLibPciRead (MAKE_SBDFO (0, 3, 3, 0, 0), AccessWidth32, &Value, NULL);
      if ((Value & 0xFFFF) == 0x10B5) {
        GnbLibPciRead (MAKE_SBDFO (0, 3, 3, 0, 0x08), AccessWidth32, &Value, NULL);
        if (((Value >> 24) & 0xFF) == 0x06) {
          GnbLibPciRmw (MAKE_SBDFO (0, 3, 3, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x070700, (AMD_CONFIG_PARAMS *) NULL);
        }
      }

      ChipsetLocated = FALSE;
      while (ChipsetLocated == FALSE) {
        GnbLibPciRead(MAKE_SBDFO(0, 3, 1, 0, 0), AccessWidth32, &Value, NULL);
        IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D1F00x00 = 0x%x\n", Value);
        if ((Value & 0xFFFF) == 0x10B5) {
          GnbLibPciRead (MAKE_SBDFO (0, 3, 1, 0, 0x80), AccessWidth32, &Value, NULL);
          IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D1F0x80 = 0x%x\n", Value);
          if (((Value >> 22) & 0x01) == 0x01) {
            GnbLibPciRead (MAKE_SBDFO (0, 3, 1, 0, 0x78), AccessWidth32, &Value, NULL);
            IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D1F0x78 = 0x%x\n", Value);
            if (((Value >> 27) & 0x05) == 0x04) {
              do {
                GnbLibPciRead(MAKE_SBDFO(0, 5, 0, 0, 0x08), AccessWidth32, &Value, NULL);
                IDS_HDT_CONSOLE (GNB_TRACE, "Bus5 D0F0x08 = 0x%x\n", Value);
              } while (Value == 0xFFFFFFFF);
              GnbLibPciRead(MAKE_SBDFO(0, 5, 0, 2, 0x08), AccessWidth32, &Value, NULL);
              IDS_HDT_CONSOLE (GNB_TRACE, "Bus5 D0F2x08 = 0x%x\n", Value);
              if (((Value >> 24) & 0xFF) == 0x06) {
                GnbLibPciRmw (MAKE_SBDFO (0, 3, 1, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x040400, (AMD_CONFIG_PARAMS *) NULL);
                ChipsetLocated = TRUE;
              }
            }
          }
        }
        GnbLibPciRead (MAKE_SBDFO (0, 3, 2, 0, 0), AccessWidth32, &Value, NULL);
        IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D2F00x00 = 0x%x\n", Value);
        if ((Value & 0xFFFF) == 0x10B5) {
          GnbLibPciRead (MAKE_SBDFO (0, 3, 2, 0, 0x80), AccessWidth32, &Value, NULL);
          IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D2F0x80 = 0x%x\n", Value);
          if (((Value >> 22) & 0x01) == 0x01) {
            GnbLibPciRead (MAKE_SBDFO (0, 3, 2, 0, 0x78), AccessWidth32, &Value, NULL);
            IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D2F0x78 = 0x%x\n", Value);
            if (((Value >> 27) & 0x05) == 0x04) {
              do {
                GnbLibPciRead(MAKE_SBDFO(0, 6, 0, 0, 0x08), AccessWidth32, &Value, NULL);
                IDS_HDT_CONSOLE (GNB_TRACE, "Bus6 D0F0x08 = 0x%x\n", Value);
              } while (Value == 0xFFFFFFFF);
              GnbLibPciRead(MAKE_SBDFO(0, 6, 0, 2, 0x08), AccessWidth32, &Value, NULL);
              IDS_HDT_CONSOLE (GNB_TRACE, "Bus6 D0F2x08 = 0x%x\n", Value);
              if (((Value >> 24) & 0xFF) == 0x06) {
                GnbLibPciRmw (MAKE_SBDFO (0, 3, 2, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x040400, (AMD_CONFIG_PARAMS *) NULL);
                ChipsetLocated = TRUE;
              }
            }
          }
        }
        GnbLibPciRead (MAKE_SBDFO (0, 3, 3, 0, 0), AccessWidth32, &Value, NULL);
        IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D3F00x00 = 0x%x\n", Value);
        if ((Value & 0xFFFF) == 0x10B5) {
          GnbLibPciRead (MAKE_SBDFO (0, 3, 3, 0, 0x80), AccessWidth32, &Value, NULL);
          IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D3F0x80 = 0x%x\n", Value);
          if (((Value >> 22) & 0x01) == 0x01) {
            GnbLibPciRead (MAKE_SBDFO (0, 3, 3, 0, 0x78), AccessWidth32, &Value, NULL);
            IDS_HDT_CONSOLE (GNB_TRACE, "Bus3 D3F0x78 = 0x%x\n", Value);
            if (((Value >> 27) & 0x05) == 0x04) {
              do {
                GnbLibPciRead(MAKE_SBDFO(0, 7, 0, 0, 0x08), AccessWidth32, &Value, NULL);
                IDS_HDT_CONSOLE (GNB_TRACE, "Bus7 D0F0x08 = 0x%x\n", Value);
              } while (Value == 0xFFFFFFFF);
              GnbLibPciRead(MAKE_SBDFO(0, 7, 0, 2, 0x08), AccessWidth32, &Value, NULL);
              IDS_HDT_CONSOLE (GNB_TRACE, "Bus7 D0F2x08 = 0x%x\n", Value);
              if (((Value >> 24) & 0xFF) == 0x06) {
                GnbLibPciRmw (MAKE_SBDFO (0, 3, 3, 0, 0x18), AccessWidth32, 0xFF0000FF, 0x040400, (AMD_CONFIG_PARAMS *) NULL);
                ChipsetLocated = TRUE;
              }
            }
          }
        }
      }
      EarlyTrainData->EarlyTrainDesc->LinkDesc.Pcie.DevFunNumber = 0x400;
    } else {
      IDS_HDT_CONSOLE (GNB_TRACE, "Downstream switch not found\n");
      EarlyTrainData->EarlyTrainDesc->LinkDesc.Pcie.DevFunNumber = 0x200;
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "Sending this Platform Descriptor back to SMU...\n");
    GnbLibDebugDumpBuffer ((VOID*) ThisNode, ThisNodeSize << 2, 1, 16);
    IDS_HDT_CONSOLE (GNB_TRACE, "\n");
    DxioLibServiceRequest (SmuServicesPpi, GnbHandle->SocketId, DXIO_MSG_INIT_LOADHEAP, 0, (UINT32) ThisNode, ThisNodeSize, 0, DXIO_HEAPTYPE_CFGPTR);
  }
}

