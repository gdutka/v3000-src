/*
*****************************************************************************
*
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <GnbDxio.h>
#include <Filecode.h>
#include <Library/IdsLib.h>
#include <Library/GnbPcieConfigLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/PcieMiscCommLib.h>
#include <Library/SmnAccessLib.h>
#include <GnbRegistersRMB.h>
#include "PcieStraps.h"

#define FILECODE        NBIO_PCIE_RMB_DXE_AMDNBIOPCIEASPM_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to Disable ASPM
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/
VOID
PcieAspmBlackListInterface (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;
  UINT16                         AspmData;
  UINT16                         Index;
  UINT16                         AspmCap;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAspmBlackListInterface Enter\n");
  AspmData = 0xFFFF;
  AspmCap = 1;
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        if (PcieConfigCheckPortStatus (PcieEngine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
          AspmData = PcieDisableASPMCallback (PcieEngine, &AspmCap);
          if (AspmData != 0xFFFF) {
            PcieEngine->Type.Port.PortData.LinkAspm &= (UINT8)AspmData;
            if (AspmCap == 0) {
              // Configure PM Support
              Index = PORT_STRAP_INDEX(STRAP_BIF_PM_SUPPORT_A_INDEX, (PcieEngine->Type.Port.PortId % 8));
              IDS_HDT_CONSOLE (GNB_TRACE, " - writing strap 0x%x to ASPM %d\n", Index, PcieEngine->Type.Port.PortData.LinkAspm);
              WritePcieStrap (GnbHandle, Index, PcieEngine->Type.Port.PortData.LinkAspm, PcieWrapper->WrapId);
            }
          }

          if ((PcieEngine->Type.Port.LinkAspmL1_1 == 1) || (PcieEngine->Type.Port.LinkAspmL1_2 == 1)) {
            if (PcieDisableL1SSCallback (PcieEngine)) {
              // PCIE_CNTL::HWINIT_WR_LOCK = 0
              SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    (WRAP_SPACE (GnbHandle, PcieWrapper, SMN_PCIE0_PCIE_CNTL_ADDRESS)),
                    (UINT32) ~(PCIE_CNTL_HWINIT_WR_LOCK_MASK),
                    0 << PCIE_CNTL_HWINIT_WR_LOCK_OFFSET,
                    GNB_REG_ACC_FLAG_S3SAVE
                    );
              if (PcieEngine->Type.Port.LinkAspmL1_1 == 0) {
                // PCIE_L1_PM_SUB_CAP_funcX::PCI_PM_L1_1_SUPPORTED = 0
                // PCIE_L1_PM_SUB_CAP_funcX::ASPM_L1_1_SUPPORTED = 0
               SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                         PORT_SPACE (GnbHandle, PcieWrapper, (PcieEngine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS),
                         (UINT32) ~(PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_1_EN_MASK | PCIE_L1_PM_SUB_CNTL_ASPM_L1_1_EN_MASK),
                         (0x0 << PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_1_EN_OFFSET) | (0x0 << PCIE_L1_PM_SUB_CNTL_ASPM_L1_1_EN_OFFSET),
                         GNB_REG_ACC_FLAG_S3SAVE
                         );
              }
              if (PcieEngine->Type.Port.LinkAspmL1_2 == 0) {
                // PCIE_L1_PM_SUB_CAP_funcX::PCI_PM_L1_2_SUPPORTED = 0
                // PCIE_L1_PM_SUB_CAP_funcX::ASPM_L1_2_SUPPORTED = 0
               SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                         PORT_SPACE (GnbHandle, PcieWrapper, (PcieEngine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS),
                         (UINT32) ~(PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_2_EN_MASK | PCIE_L1_PM_SUB_CNTL_ASPM_L1_2_EN_MASK),
                         (0x0 << PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_2_EN_OFFSET) | (0x0 << PCIE_L1_PM_SUB_CNTL_ASPM_L1_2_EN_OFFSET),
                         GNB_REG_ACC_FLAG_S3SAVE
                         );
              }
              // PCIE_CNTL::HWINIT_WR_LOCK = 1
              SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    (WRAP_SPACE (GnbHandle, PcieWrapper, SMN_PCIE0_PCIE_CNTL_ADDRESS)),
                    (UINT32) ~(PCIE_CNTL_HWINIT_WR_LOCK_MASK),
                    1 << PCIE_CNTL_HWINIT_WR_LOCK_OFFSET,
                    GNB_REG_ACC_FLAG_S3SAVE
                    );
            }
          }
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAspmBlackListInterface Exit\n");
}
