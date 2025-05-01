/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * DxioInit - Initialize DXIO subsystem
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioBaseZPPei
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
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
#include <Ppi/AmdMemoryInfoHobPpi.h>

#include <Guid/GnbPcieInfoHob.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/NbioPcieTrainingPpi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/NbioEarlyLinkPpi.h>
#include <Ppi/SocLogicalIdPpi.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/NbioHandleLib.h>
//Dependent on AgesaPkg
#include <Library/DxioLibV2.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/PcieConfigLib.h>
#include "NbioIommuTbl.h"

#include "AmdNbioPciePei.h"
#include <Library/NbioTableBlastLib.h>
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>

#define FILECODE        NBIO_PCIE_RMB_PEI_DXIOINIT_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
extern
PEI_AMD_NBIO_EARLY_LINK_PPI mAmdNbioEarlyLinkPpi;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// Comment this line to enable extended debug output if tracing is enabled
#undef GNB_TRACE_ENABLE
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

UINT32    StrapList[8] = {0xDB, 0x12C, 0x17D, 0x1Ce, 0x21F, 0x270, 0x2C1, 0x312};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */

STATIC PEI_AMD_NBIO_PCIE_TRAINING_START_PPI mNbioPcieTrainingStartPpi = {
  AMD_TRAINING_START_STATUS          /// Start Status
};

STATIC EFI_PEI_PPI_DESCRIPTOR mNbioPcieTrainingStartPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieTrainingStartPpiGuid,
  &mNbioPcieTrainingStartPpi
};

STATIC PEI_AMD_NBIO_PCIE_TRAINING_DONE_PPI mNbioPcieTrainingDonePpi = {
  AMD_TRAINING_DONE_STATUS          /// Done Status
};

STATIC EFI_PEI_PPI_DESCRIPTOR mNbioPcieTrainingDonePpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieTrainingDonePpiGuid,
  &mNbioPcieTrainingDonePpi
};
STATIC EFI_PEI_PPI_DESCRIPTOR mNbioEarlyLinkPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioEarlyLinkPpiGuid,
  &mAmdNbioEarlyLinkPpi
};


#define ACPI_MMIO_BASE  0xFED80000ul
#define PMIO_BASE       0x300   // DWORD
#define FCH_PMIOA_REG64          0x64         // AcpiPmTmrBlk

VOID
sleep (
  IN     UINT32         Milliseconds
  )
{
  UINT64              uSec;
  UINT16              Address;
  UINT32              startTime;
  UINT64              elapsedTime;

  Address = 0;
  uSec = DivU64x32 ((Milliseconds * 1000), 2);
  LibAmdMemRead (AccessWidth16, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64), &Address, NULL);
  if ( Address == 0 ) {
    IDS_HDT_CONSOLE (GNB_TRACE, "sleep: IO Delay\n");
    while ( uSec != 0) {
      LibAmdIoRead (AccessWidth8, 0x80, (UINT8*)&elapsedTime, NULL);
      uSec--;
    }
  } else {
    LibAmdIoRead (AccessWidth32, Address, &startTime, NULL);
    IDS_HDT_CONSOLE (GNB_TRACE, "sleep: ACPI timer\n");
    elapsedTime = 0;
    for ( ;; ) {
      LibAmdIoRead (AccessWidth32, Address, &elapsedTime, NULL);
      if ( elapsedTime <= startTime ) {
        elapsedTime = elapsedTime + 0xFFFFFFFFul - startTime;
      } else {
        elapsedTime = elapsedTime - startTime;
      }

      if (DivU64x32 ( MultU64x32 (elapsedTime, 28), 100 ) > uSec ) {
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set PCIe training timers
 *
 *
 *
 * @param[in]  LinkResetToTrainingTime       Pointer to LinkResetToTrainingTime
 * @param[in]  LinkReceiverDetectionPolling  Pointer to LinkReceiverDetectionPolling
 * @param[in]  LinkL0Polling                 Pointer to LinkL0Polling
 */

BOOLEAN
SetPCIeTrainingTimers(
  IN OUT   UINT32*  LinkResetToTrainingTime,
  IN OUT   UINT32*  LinkReceiverDetectionPolling,
  IN OUT   UINT32*  LinkL0Polling
  )
{
  BOOLEAN TimingControlEn;

  TimingControlEn = FALSE;
  if (PcdGetBool (PcdPcieDxioTimingControlEnable)) {
    *LinkResetToTrainingTime = PcdGet32 (PcdPCIELinkResetToTrainingTime);
    *LinkReceiverDetectionPolling = PcdGet32 (PcdPCIELinkReceiverDetectionPolling);
    *LinkL0Polling = PcdGet32 (PcdPCIELinkL0Polling);
    TimingControlEn = TRUE;
  }
  return TimingControlEn;
}
/**----------------------------------------------------------------------------------------*/
/**
 * Nbif Device setting Before Dxio init
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/
VOID
NbifDeviceEarlyControl (
  IN       GNB_HANDLE       *GnbHandle
  )
{

  if (!PcdGetBool (PcdCfgSensorFusionHubEnable)) {
    NbioDisableNbifDevice (GnbHandle, NbifFunctionMp2, 0);  //MP2 0x7
  }

  if (!PcdGetBool (PcdAcpController)) {
    NbioDisableNbifDevice (GnbHandle, NbifFunctionAcp, 0);  //ACP 0x5
  }
  if (!PcdGetBool(PcdCfgHdAudioEnable)){
    NbioDisableNbifDevice (GnbHandle, NbifFunctionHdAudio, 0);  //Hd Audio 0x6
  }
  if (!PcdGet8(PcdXgbeDisable)){
    NbioDisableEnableNbifDevice(GnbHandle, NbifFunctionXgbe0,PcdGetBool (PcdXgbePort0ConfigEn) );
    NbioDisableEnableNbifDevice(GnbHandle, NbifFunctionXgbe1, PcdGetBool (PcdXgbePort1ConfigEn));
  } else {
    NbioDisableEnableNbifDevice(GnbHandle, NbifFunctionXgbe0, 0);
    NbioDisableEnableNbifDevice(GnbHandle, NbifFunctionXgbe1, 0);
  }
}

//
// This table initializes IOMMU
//
GNB_TABLE ROMDATA GnbIommuAfterDXIOInitTable [] = {
  NBIO_IOMMU_L1_INIT_TBL
  NBIO_IOMMU_L2_INIT_TBL
  NBIO_IOMMU_CLOCK_GATING_TBL
  NBIO_IOMMU_L2_DYNAMIC_POWER_GATING_TBL
  NBIO_IOMMU_L2_MEMORY_POWER_GATING_TBL
  NBIO_IOMMU_L1_MEMORY_POWER_GATING_TBL

  // Configure IOMMU Power Island
  // Hide IOMMU function if disabled
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_DISABLED,
    ONE_ADDR_TYPE,
    SMN_CFG_IOHC_PCI_ADDRESS,
    CFG_IOHC_PCI_CFG_IOHC_PCI_Dev0Fn2RegEn_MASK,
    (0x0 << CFG_IOHC_PCI_CFG_IOHC_PCI_Dev0Fn2RegEn_OFFSET)
    ),

  GNB_ENTRY_TERMINATE
};

/**----------------------------------------------------------------------------------------*/
/**
 * IOMMU Initialization After Dxio init
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    VOID
 */
 /*----------------------------------------------------------------------------------------*/
VOID
IommuInitAfterDxioInit (
  IN       GNB_HANDLE       *GnbHandle
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  PCIe_ENGINE_CONFIG    *Engine;
  BOOLEAN               CoreUsed;
  UINT32                Value32;

  CoreUsed = FALSE;
  Wrapper = PcieConfigGetChildWrapper (GnbHandle);
  while ((Wrapper != NULL) && (CoreUsed == FALSE)) {
      Engine = PcieConfigGetChildEngine (Wrapper);
      while (Engine != NULL) {
        if (PcieLibIsEngineAllocated (Engine)) {
          if ((Engine->Type.Port.PortData.PortPresent == 1) && (Engine->Type.Port.CoreId == 0)) {
            SmnRegisterRead (GnbHandle->Address.Address.Bus,
                              IOHC_BRIDGE_SPACE (GnbHandle, Engine, SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS),
                              &Value32);
            IDS_HDT_CONSOLE (MAIN_FLOW, "Read IOHC_Bridge_CNTL as 0x%x from 0x%x\n",
                              Value32,
                              IOHC_BRIDGE_SPACE (GnbHandle, Engine, SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS)
                              );
            if ((Value32 & IOHC_Bridge_CNTL_BridgeDis_MASK) == 0)  {
              CoreUsed = TRUE;
              IDS_HDT_CONSOLE (MAIN_FLOW, "EngineId=%d in use(%d)\n", Engine->Type.Port.LogicalBridgeId, CoreUsed);
              break;
            }
          }
        }
        Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (Engine, DESCRIPTOR_TERMINATE_TOPOLOGY);
      }
    Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor (Wrapper, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }

  if (CoreUsed == FALSE) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_L2_CP_CONTROL_1_ADDRESS,
                    (UINT32)~(L2_CP_CONTROL_1_CPL1Off_MASK),
                    1 << L2_CP_CONTROL_1_CPL1Off_OFFSET,
                    0
                    );
    IDS_HDT_CONSOLE (MAIN_FLOW, "set L2_CP_CONTROL_1_CPL1Off\n");
  }
  return;
}

/**----------------------------------------------------------------------------------------*/
/**
 * Hide USB4 PCIe ports
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/
VOID
HideUSB4PCIePorts (
  IN       GNB_HANDLE       *GnbHandle
  )
{
   // Hide PCIE2 DEVINDCFG0
   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE (GnbHandle, SMN_PCIE2DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS),
                   (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK),
                   1 << IOHC_Bridge_CNTL_BridgeDis_OFFSET,
                   0
                   );
   // Hide PCIE3 DEVINDCFG0
   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE (GnbHandle, SMN_PCIE3DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS),
                   (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK),
                   1 << IOHC_Bridge_CNTL_BridgeDis_OFFSET,
                   0
                   );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback for NbioSmuServicesPpi installation
 *
 *
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
DxioInitializationCallbackPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_STATUS                        ReturnStatus;
  AGESA_STATUS                      DxioStatus;
  GNB_HANDLE                        *GnbHandle;
  PCIe_PLATFORM_CONFIG              *Pcie;
  PEI_AMD_NBIO_PCIE_SERVICES_PPI    *PcieServicesPpi;
  PEI_AMD_NBIO_SMU_SERVICES_PPI     *SmuServicesPpi;
  AMD_MEMORY_INFO_HOB_PPI           *AmdMemoryInfoHob;
  GNB_PCIE_INFORMATION_DATA_HOB     *PciePlatformConfigHob;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServicesPpi;
  DXIO_COMPLEX_DESCRIPTOR           *PcieTopologyData;
  PEI_AMD_NBIO_PCIE_COMPLEX_PPI     *NbioPcieComplexPpi;
  AMD_PEI_SOC_LOGICAL_ID_PPI        *SocLogicalIdPpi;
  SOC_LOGICAL_ID                    LogicalId;
  UINT32                            Value;
  UINT32                            Property;
  PRESIL_CTRL0                      PreSilCtrl0;
  PRESIL_CTRL1                      PreSilCtrl1;

  AGESA_TESTPOINT (TpDxioInitializationCallbackPpiEntry, NULL);
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  ReturnStatus = EFI_SUCCESS;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdMemoryInfoHobPpiGuid, 0, NULL, &AmdMemoryInfoHob);
  if (!EFI_ERROR (Status)) {
    // Install PPI to notify other drivers that PCIe training is starting
    Status = (**PeiServices).InstallPpi (PeiServices, &mNbioPcieTrainingStartPpiList);
    GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE, "AmdMemoryInfoHobPpiGuid is installed\n"));
    Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &SmuServicesPpi);
    if (!EFI_ERROR (Status)) {
      GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE, "gAmdNbioSmuServicesPpiGuid is installed\n"));
      Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioPcieComplexPpiGuid, 0, NULL, &NbioPcieComplexPpi);
      if (!EFI_ERROR (Status)) {
        GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE, "gAmdNbioPcieComplexPpiGuid is installed\n"));

        // Use PCIe Complex PPI to get topology from platform BIOS
        NbioPcieComplexPpi->PcieGetComplex (NbioPcieComplexPpi, &PcieTopologyData);

        // Get Logical CPU ID info
        Status = (*PeiServices)->LocatePpi (
                                   PeiServices,
                                   &gAmdSocLogicalIdPpiGuid,
                                   0,
                                   NULL,
                                   &SocLogicalIdPpi
                                   );
        Status = SocLogicalIdPpi->GetLogicalIdOnCurrentCore (&LogicalId);

        // Get Fabric Topology services
        Status = (*PeiServices)->LocatePpi (
                                   PeiServices,
                                   &gAmdFabricTopologyServices2PpiGuid,
                                   0,
                                   NULL,
                                   &FabricTopologyServicesPpi
                                   );

        // Get NBIO topology structure
        Status = (*PeiServices)->LocatePpi (
                                   PeiServices,
                                   &gAmdNbioPcieServicesPpiGuid,
                                   0,
                                   NULL,
                                   (VOID **)&PcieServicesPpi
                                   );
        PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
        Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);

        // Check for user override of PCIe topology configuration.
        IDS_HOOK (IDS_HOOK_NBIO_PCIE_TOPOLOGY, (VOID *) NULL, (VOID *) &PcieTopologyData);

        if ((LogicalId.Revision & AMD_REV_F19_RMB_Ax) == 0) {
          DxioTopologyAddUSB4 (PeiServices, 2, &PcieTopologyData);
        }

        DxioCfgBeforeDxioInit (Pcie, PcieTopologyData);

        GnbHandle = NbioGetHandle (Pcie);
        PcieEarlyTrainFixups (GnbHandle, PcieTopologyData);
        PcieLaneMappingFixups (GnbHandle, PcieTopologyData);
        NbifDeviceEarlyControl (GnbHandle);

        // Skip DXIO for emulation if so requested
        PreSilCtrl0.Value = PcdGet32 (PcdAmdPreSilCtrl0);
        PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
        if (PreSilCtrl0.Field.SkipDxioInitializaion == 0) {
          // At this point we know the NBIO topology and SMU services are available
          // We can now use SMU to initialize DXIO

          if (PreSilCtrl1.Field.DisableMp1DxioPhyFwLoad == 1) {
            PcdSet8S (PcdDxioPhyProgramming, 0); // skip usePhySRAMFlag
          }
          DxioStatus = DxioEarlyInitV2(PeiServices, SmuServicesPpi, Pcie, NbioGetHandle(Pcie), PcieTopologyData);
          if (DxioStatus != AGESA_SUCCESS) {
            IDS_HDT_CONSOLE (GNB_TRACE, "DXIO ERROR!!\n");
            ReturnStatus = EFI_INVALID_PARAMETER;
            ASSERT (FALSE);
          }
        }

        if (PcdGetBool (PcdBixbyLinkFound)) {

          // restore IOMMU

          // Copy SB_LOCATION to the L1 and L2 in IOMMU. This only applies to NBIO3, where the FCH is located
          SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_SB_LOCATION_ADDRESS), &Value);
          IDS_HDT_CONSOLE (MAIN_FLOW, "Read SB_LOCATION as 0x%x from 0x%x\n", \
                                        Value, NBIO_SPACE(GnbHandle, SMN_SB_LOCATION_ADDRESS));
          SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                                        NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_SB_LOCATION_ADDRESS), &Value, 0);
          IDS_HDT_CONSOLE (MAIN_FLOW, "Wrote SB_LOCATION as 0x%x from 0x%x\n", \
                                        Value, NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_SB_LOCATION_ADDRESS));
          SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                                        NBIO_SPACE(GnbHandle, SMN_L2_SB_LOCATION_ADDRESS), &Value, 0);
          IDS_HDT_CONSOLE (MAIN_FLOW, "Wrote SB_LOCATION as 0x%x from 0x%x\n", \
                                        Value, NBIO_SPACE(GnbHandle, SMN_L2_SB_LOCATION_ADDRESS));

          Property = TABLE_PROPERTY_DEFAULT;
          if (FALSE == PcdGetBool (PcdCfgIommuSupport)) {
            Property |= TABLE_PROPERTY_IOMMU_DISABLED;
          }
          // Program up IOMMU NBIO Tables
          GnbBlastTable (
            GnbHandle,
            GnbIommuAfterDXIOInitTable,
            Property,
            0,
            NULL
            );

        }

        GnbHandle = NbioGetHandle (Pcie);
        PciePortsVisibilityControl (HidePorts, GnbHandle);

        DxioCfgAfterDxioInit (Pcie, FabricTopologyServicesPpi);
        IommuInitAfterDxioInit (GnbHandle);

        PcieConfigureHotplugPorts (Pcie);
        if ((LogicalId.Revision & AMD_REV_F19_RMB_Ax) == 0) {
          PcieHotplugConfigureUSB4 (GnbHandle);
          HideUSB4PCIePorts (GnbHandle);
        }

        // Install PPI to notify other drivers that PCIe training is complete
        (**PeiServices).InstallPpi (PeiServices, &mNbioPcieTrainingDonePpiList);
        if (!PcdGetBool (PcdBixbyLinkFound)) {
          (**PeiServices).InstallPpi(PeiServices, &mNbioEarlyLinkPpiList);
        }

      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit Status = 0x%x\n", __FUNCTION__, ReturnStatus);
  AGESA_TESTPOINT (TpDxioInitializationCallbackPpiExit, NULL);
  return ReturnStatus;
}

