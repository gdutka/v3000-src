/** @file

;******************************************************************************
;* Copyright (c) 2017 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/ChipsetConfigLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiVerbTableCfgLib.h>
#include <Library/PeiOemSvcKernelLib.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <AmdCpmPei.h>
#include <PortConfig.h>
//[-start-220329-IB14740229-add]//
#include "InitPortConfig.h"
//[-end-220329-IB14740229-add]//

#define  SPECIAL_STRUCTURE_VERBTABLE_HEADER_COUNT  0x1
#define  SPECIAL_STRUCTURE_VERBTABLE_END_COUNT     0x3
#define  CODEC_SET_COUNT                           0x4

EFI_STATUS
EFIAPI
PciePortConfigPei (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  );

EFI_STATUS
EFIAPI
HDMIAudioConfigCallback (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  );

EFI_STATUS
EFIAPI
UsbPortConfigPei (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  );

EFI_STATUS
EFIAPI
OemSvcGetVerbTableCallback (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  );

EFI_STATUS
EFIAPI
SataPortConfigPei (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  );

EFI_STATUS
EFIAPI
PortConfigNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );


EFI_PEI_NOTIFY_DESCRIPTOR      mPlatformCustomizeNotifyPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gAmdCpmOverrideTablePpiGuid,
    PciePortConfigPei
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gAmdCpmOverrideTablePpiGuid,
    OemSvcGetVerbTableCallback
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR      mUsb3PcdNotifyPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gAmdCbsHookPpiGuid,
    UsbPortConfigPei
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR      mSataPcdNotifyPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gAmdCbsHookPpiGuid,
    SataPortConfigPei
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR      mHDAudioPcdNotifyPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiReadOnlyVariable2PpiGuid,
    HDMIAudioConfigCallback
  }
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR      mNotifyPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiReadOnlyVariable2PpiGuid,
    PortConfigNotifyCallback
  }
};
/**
 * Set PCI-E Port Config
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */
EFI_STATUS
EFIAPI
PciePortConfigPei (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  )
{
  EFI_STATUS                         Status;
  AMD_CPM_TABLE_PPI                  *AmdCpmTablePpi;
  AMD_CPM_DXIO_TOPOLOGY_TABLE        *DxioTopologyTablePtr;
  DXIO_PORT_DESCRIPTOR               *DxioPort;
  PORT_CONFIG                        PciePortConfig;
//[-start-220329-IB14740229-add]//
  PORT_CONFIG4                       PcieSpeedConfig;
  UINTN                              Index;
//[-end-220329-IB14740229-add]//
//[-start-220408-IB14740232-add]//
  PORT_CONFIG                        PcieAspmConfig;
  PORT_CONFIG                        PcieL1SubConfig;
//[-end-220408-IB14740232-add]//

  Status = (*PeiServices)->LocatePpi (
                           (CONST EFI_PEI_SERVICES**)PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

//[-start-220329-IB14740229-modify]//
  PciePortConfig.Value = PcdGet64 (PcdH2OChipsetPciePortEnable);
  PcieSpeedConfig.Value = PcdGet64 (PcdH2OChipsetPciePortSpeed);
//[-start-220408-IB14740232-add]//
  PcieAspmConfig.Value = PcdGet64 (PcdH2OChipsetPciePortAspm);
  PcieL1SubConfig.Value = PcdGet64 (PcdH2OChipsetPciePortL1Sub);
//[-end-220408-IB14740232-add]//

  //
  // CPU GPP PCIe
  //
  DxioTopologyTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DXIO_TOPOLOGY);
  DxioPort = &(DxioTopologyTablePtr->Port[0]);
  for (Index = 0; DxioPort[Index].Flags != DESCRIPTOR_TERMINATE_LIST; Index++) {
    // X8 Slot J97
    if (DxioPort[Index].EngineData.StartLane == 0 && DxioPort[Index].EngineData.EndLane == 7) {
      if (!PciePortConfig.Config1.Port00En) {
        DxioPort[Index].Port.PortPresent = DxioPortDisabled;
      }
      DxioPort[Index].Port.LinkSpeedCapability = (UINT8)PcieSpeedConfig.Config4.Port00En;
//[-start-220408-IB14740232-add]//
      DxioPort[Index].Port.LinkAspm = (UINT8)PcieAspmConfig.Config2.Port00En;
      if (DxioPort[Index].Port.LinkAspm == DxioAspmL1) {
        if ((PcieL1SubConfig.Config2.Port00En & BIT0) == BIT0) {
          DxioPort[Index].Port.LinkAspmL1_1 = DxioAspmL11Enabled;
        } else {
          DxioPort[Index].Port.LinkAspmL1_1 = DxioAspmL11Disabled;
        }
        if ((PcieL1SubConfig.Config2.Port00En & BIT1) == BIT1) {
          DxioPort[Index].Port.LinkAspmL1_2 = DxioAspmL12Enabled;
        } else {
          DxioPort[Index].Port.LinkAspmL1_2 = DxioAspmL12Disabled;
        }
      } else {
        DxioPort[Index].Port.LinkAspmL1_1 = DxioAspmL11Disabled;
        DxioPort[Index].Port.LinkAspmL1_2 = DxioAspmL12Disabled;
      }
//[-end-220408-IB14740232-add]//
    }
    // X4 Slot CON2
    if (DxioPort[Index].EngineData.StartLane == 12 && DxioPort[Index].EngineData.EndLane == 15) {
      if (!PciePortConfig.Config1.Port01En) {
        DxioPort[Index].Port.PortPresent = DxioPortDisabled;
      }
      DxioPort[Index].Port.LinkSpeedCapability = (UINT8)PcieSpeedConfig.Config4.Port01En;
//[-start-220408-IB14740232-add]//
      DxioPort[Index].Port.LinkAspm = (UINT8)PcieAspmConfig.Config2.Port01En;
      if (DxioPort[Index].Port.LinkAspm == DxioAspmL1) {
        if ((PcieL1SubConfig.Config2.Port01En & BIT0) == BIT0) {
          DxioPort[Index].Port.LinkAspmL1_1 = DxioAspmL11Enabled;
        } else {
          DxioPort[Index].Port.LinkAspmL1_1 = DxioAspmL11Disabled;
        }
        if ((PcieL1SubConfig.Config2.Port01En & BIT1) == BIT1) {
          DxioPort[Index].Port.LinkAspmL1_2 = DxioAspmL12Enabled;
        } else {
          DxioPort[Index].Port.LinkAspmL1_2 = DxioAspmL12Disabled;
        }
      } else {
        DxioPort[Index].Port.LinkAspmL1_1 = DxioAspmL11Disabled;
        DxioPort[Index].Port.LinkAspmL1_2 = DxioAspmL12Disabled;
      }
//[-end-220408-IB14740232-add]//
    }
  }
//[-end-220329-IB14740229-modify]//

  return EFI_SUCCESS;
}

EFI_STATUS
UpdateOemVerbTable (
  IN EFI_PEI_SERVICES                    **PeiServices,
  IN COMMON_CHIPSET_AZALIA_VERB_TABLE    *VerbTableHeaderDataAddress
  )
{
  UINT32                                 *AzaliaVerbTable;
  UINT32                                 Size;
  UINT32                                 StructureCount;
  EFI_STATUS                             Status;
  UINT32                                 Index;

  DEBUG((EFI_D_INFO, "OEM Azalia VerbTable\n"));

  Index        = 0;

  StructureCount = (VerbTableHeaderDataAddress->VerbTableHeader->NumberOfRearJacks +
                    VerbTableHeaderDataAddress->VerbTableHeader->NumberOfFrontJacks) *
                    CODEC_SET_COUNT;

  Size = (StructureCount +
          SPECIAL_STRUCTURE_VERBTABLE_HEADER_COUNT +
          SPECIAL_STRUCTURE_VERBTABLE_END_COUNT) * 
          sizeof (UINT32);

  DEBUG((EFI_D_INFO, "Total Size : 0x%08x\n", Size));
  Status = (*PeiServices)->AllocatePool (
                             (CONST EFI_PEI_SERVICES**)PeiServices, 
                             Size, 
                             (VOID**)&AzaliaVerbTable
                             );
  if (!EFI_ERROR(Status)) {  
    AzaliaVerbTable[0] = VerbTableHeaderDataAddress->VerbTableHeader->VendorDeviceId;
    for (Index = 0; Index < (StructureCount + SPECIAL_STRUCTURE_VERBTABLE_END_COUNT); Index++) {
      if (Index < StructureCount) {
        AzaliaVerbTable[Index + 1] = VerbTableHeaderDataAddress->VerbTableData[Index];
      } else {
        AzaliaVerbTable[Index + 1] = 0xFFFFFFFF;
      }
    };
  } else {
    return Status;
  }

  DEBUG((EFI_D_INFO, "VerbTableData :\n"));
  for (Index = 0; Index < (StructureCount + SPECIAL_STRUCTURE_VERBTABLE_HEADER_COUNT + SPECIAL_STRUCTURE_VERBTABLE_END_COUNT); Index++) {
    DEBUG ((EFI_D_INFO, "0x%08x\n", AzaliaVerbTable[Index]));
  }
  DEBUG((EFI_D_INFO, "Update Status : "));
  Status = PcdSet32S (PcdCfgAzaliaCodecVerbTable, (UINT32) AzaliaVerbTable);
  DEBUG((EFI_D_INFO, "%r\n", Status));
  return Status;
}

/**
 * Set Verb Table to PcdCfgAzaliaCodecVerbTable
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */
EFI_STATUS
EFIAPI
OemSvcGetVerbTableCallback (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  )
{
  EFI_STATUS                         Status;
  COMMON_CHIPSET_AZALIA_VERB_TABLE   *VerbTableHeaderDataAddress;
  //
  // OemServices
  //
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcGetVerbTable \n"));
  Status = OemSvcGetVerbTable (&VerbTableHeaderDataAddress);
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcGetVerbTable Status: %r\n", Status));
  if (Status == EFI_MEDIA_CHANGED) {
    Status = UpdateOemVerbTable (PeiServices, VerbTableHeaderDataAddress);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 * Set SCU HDMI Audio to PcdCfgHdAudioEnable
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */
EFI_STATUS
EFIAPI
HDMIAudioConfigCallback (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  )
{
  EFI_STATUS                         Status;
  CHIPSET_CONFIGURATION              ChipsetConfiguration;

  Status = GetChipsetConfiguration (&ChipsetConfiguration);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (ChipsetConfiguration.EnableHDMIAudio == 0) {
    Status = PcdSetBoolS (PcdCfgHdAudioEnable, FALSE);
    ASSERT_EFI_ERROR (Status);

  } else if (ChipsetConfiguration.EnableHDMIAudio == 1) {
    Status = PcdSetBoolS (PcdCfgHdAudioEnable, TRUE);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

/**
 * Set XHCI Port Config to PCD
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
UsbPortConfigPei (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  )
{
//[-start-220329-IB14740229-modify]//
  PORT_CONFIG                        Usb2PortConfig;
  PORT_CONFIG                        Usb3PortConfig;
  UINT32                             Usb2PortDisable;
  UINT32                             Usb3PortDisable;
  EFI_STATUS                         Status;

  Usb2PortConfig.Value = PcdGet64 (PcdH2OChipsetUsbPortEnable);
  Usb3PortConfig.Value = PcdGet64 (PcdH2OChipsetUsb3PortEnable);

  Usb2PortDisable = 0;
  Usb3PortDisable = 0;

  if (!Usb2PortConfig.Config1.Port00En) {
    Usb2PortDisable |= BIT4;
  }
  if (!Usb3PortConfig.Config1.Port00En) {
    Usb3PortDisable |= BIT2;
  }

  if (!Usb2PortConfig.Config1.Port01En) {
    Usb2PortDisable |= BIT5;
  }
  if (!Usb3PortConfig.Config1.Port01En) {
    Usb3PortDisable |= BIT3;
  }

  if (!Usb2PortConfig.Config1.Port02En) {
    Usb2PortDisable |= BIT6;
  }

  if (!Usb2PortConfig.Config1.Port03En) {
    Usb2PortDisable |= BIT8;
  }

  if (!Usb2PortConfig.Config1.Port04En) {
    Usb2PortDisable |= BIT2;
  }

  if (!Usb2PortConfig.Config1.Port05En) {
    Usb2PortDisable |= BIT3;
  }

  DEBUG ((EFI_D_INFO, "%a Usb2PortConfig: %x\n", __FUNCTION__, Usb2PortConfig.Value));
  DEBUG ((EFI_D_INFO, "%a Usb3PortConfig: %x\n", __FUNCTION__, Usb3PortConfig.Value));

  Status = PcdSet32S (PcdXhciUsb2PortDisable, Usb2PortDisable);
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet32S (PcdXhciUsb3PortDisable, Usb3PortDisable);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
//[-end-220329-IB14740229-modify]//
}

/**
 * Set SATA Port Config to PCD
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
SataPortConfigPei (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN VOID                            *Ppi
  )
{
  PORT_CONFIG                        SataPortConfig;
  UINT64                             SataPortDisable;
  EFI_STATUS                         Status;

  SataPortConfig.Value = PcdGet64 (PcdH2OChipsetSataPortEnable);

  SataPortDisable = ~(SataPortConfig.Value & 0xFFFFFFFFFFFFFFFF);

  Status = PcdSet64S (PcdSataMultiDiePortShutDown, SataPortDisable);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Use the PEI_READ_ONLY_VARIABLE_ACCESS_PPI to notify and set port config value.

  @param  **PeiServices         General purpose services available to every PEIM.
  @param  *NotifyDescriptor     Pointer to EFI_PEI_NOTIFY_DESCRIPTOR.
  @param  *Ppi                  EFI_PEI_FV_FILE_LOADER_PPI

  @retval EFI_SUCCESS           if the interface could be successfully installed.
**/
EFI_STATUS
EFIAPI
PortConfigNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
//[-start-220329-IB14740229-modify]//
  PORT_CONFIG                   Usb2Config;
//[-end-220329-IB14740229-modify]//
  PORT_CONFIG                   Usb3Config;
  PORT_CONFIG                   PcieConfig;
  PORT_CONFIG                   SataConfig;
  EFI_STATUS                    Status;
  CHIPSET_CONFIGURATION         ChipsetConfiguration;
//[-start-220329-IB14740229-add]//
  PORT_CONFIG4                  PcieSpeedConfig;
//[-end-220329-IB14740229-add]//
//[-start-220408-IB14740232-add]//
  PORT_CONFIG                   PcieAspmConfig;
  PORT_CONFIG                   PcieL1SubConfig;
//[-end-220408-IB14740232-add]//

  //
  // Default Enable all ports
  //
//[-start-220329-IB14740229-modify]//
  Usb2Config.Value  = 0xFFFFFFFFFFFFFFFF;
//[-end-220329-IB14740229-modify]//
  Usb3Config.Value = 0xFFFFFFFFFFFFFFFF;
  SataConfig.Value = 0xFFFFFFFFFFFFFFFF;
  PcieConfig.Value = 0xAAAAAAAAAAAAAAAA;
//[-start-220329-IB14740229-add]//
  PcieSpeedConfig.Value = 0x0000000000000000;
//[-end-220329-IB14740229-add]//

  //
  // Initialize port configurations from Setup variable.
  //
  Status = GetChipsetConfiguration (&ChipsetConfiguration);

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

//[-start-220329-IB14740229-modify]//
  //
  // USB Port
  //
  Usb2Config.Config1.Port00En = ChipsetConfiguration.Usb2Port[0];
  Usb2Config.Config1.Port01En = ChipsetConfiguration.Usb2Port[1];
  Usb2Config.Config1.Port02En = ChipsetConfiguration.Usb2Port[2];
  Usb2Config.Config1.Port03En = ChipsetConfiguration.Usb2Port[3];
  Usb2Config.Config1.Port04En = ChipsetConfiguration.Usb2Port[4];
  Usb2Config.Config1.Port05En = ChipsetConfiguration.Usb2Port[5];

  Usb3Config.Config1.Port00En = ChipsetConfiguration.Usb3Port[0];
  Usb3Config.Config1.Port01En = ChipsetConfiguration.Usb3Port[1];

  //
  // PCIE
  //
  PcieConfig.Config1.Port00En = ChipsetConfiguration.PciePortEn[0];
  PcieConfig.Config1.Port01En = ChipsetConfiguration.PciePortEn[1];
//[-end-220329-IB14740229-modify]//

//[-start-220329-IB14740229-add]//
  //
  // PCIE Speed
  //
  PcieSpeedConfig.Config4.Port00En = ChipsetConfiguration.PciePortSpeed[0];
  PcieSpeedConfig.Config4.Port01En = ChipsetConfiguration.PciePortSpeed[1];
//[-end-220329-IB14740229-add]//

//[-start-220408-IB14740232-add]//
  //
  // PCIE ASPM
  //
  PcieAspmConfig.Config2.Port00En = ChipsetConfiguration.PciePortAspm[0];
  PcieAspmConfig.Config2.Port01En = ChipsetConfiguration.PciePortAspm[1];


  //
  // PCIE L1 Sub
  //
  PcieL1SubConfig.Config2.Port00En = ChipsetConfiguration.PciePortL1SubStates[0];
  PcieL1SubConfig.Config2.Port01En = ChipsetConfiguration.PciePortL1SubStates[1];
//[-end-220408-IB14740232-add]//

  //
  // SATA Controller 0
  //
  SataConfig.Config1.Port00En = ChipsetConfiguration.SataControllr0Port0Power;
  SataConfig.Config1.Port01En = ChipsetConfiguration.SataControllr0Port1Power;
  SataConfig.Config1.Port02En = ChipsetConfiguration.SataControllr0Port2Power;
  SataConfig.Config1.Port03En = ChipsetConfiguration.SataControllr0Port3Power;
  SataConfig.Config1.Port04En = ChipsetConfiguration.SataControllr0Port4Power;
  SataConfig.Config1.Port05En = ChipsetConfiguration.SataControllr0Port5Power;
  SataConfig.Config1.Port06En = ChipsetConfiguration.SataControllr0Port6Power;
  SataConfig.Config1.Port07En = ChipsetConfiguration.SataControllr0Port7Power;
  //
  // SATA Contoller 1
  //
  SataConfig.Config1.Port08En = ChipsetConfiguration.SataControllr1Port0Power;
  SataConfig.Config1.Port09En = ChipsetConfiguration.SataControllr1Port1Power;
  SataConfig.Config1.Port10En = ChipsetConfiguration.SataControllr1Port2Power;
  SataConfig.Config1.Port11En = ChipsetConfiguration.SataControllr1Port3Power;
  SataConfig.Config1.Port12En = ChipsetConfiguration.SataControllr1Port4Power;
  SataConfig.Config1.Port13En = ChipsetConfiguration.SataControllr1Port5Power;
  SataConfig.Config1.Port14En = ChipsetConfiguration.SataControllr1Port6Power;
  SataConfig.Config1.Port15En = ChipsetConfiguration.SataControllr1Port7Power;

//[-start-220329-IB14740229-modify]//
  Status = PcdSet64S (PcdH2OChipsetUsbPortEnable , Usb2Config.Value);
//[-end-220329-IB14740229-modify]//
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdH2OChipsetUsb3PortEnable, Usb3Config.Value);
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdH2OChipsetSataPortEnable, SataConfig.Value);
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdH2OChipsetPciePortEnable, PcieConfig.Value);
  ASSERT_EFI_ERROR (Status);

//[-start-220329-IB14740229-add]//
  Status = PcdSet64S (PcdH2OChipsetPciePortSpeed, PcieSpeedConfig.Value);
  ASSERT_EFI_ERROR (Status);
//[-end-220329-IB14740229-add]//

//[-start-220408-IB14740232-add]//
  Status = PcdSet64S (PcdH2OChipsetPciePortAspm, PcieAspmConfig.Value);
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdH2OChipsetPciePortL1Sub, PcieL1SubConfig.Value);
  ASSERT_EFI_ERROR (Status);
//[-end-220408-IB14740232-add]//

  // 
  // To ensure that the FTPM switch is configured
  // 
  DEBUG ((DEBUG_INFO, "PortConfigNotifyCallback TPM setting = %x\n",ChipsetConfiguration.EnableTPM));
  Status = PcdSet8S (PcdH2OTpmType, ChipsetConfiguration.EnableTPM);
  ASSERT_EFI_ERROR (Status);
  if (ChipsetConfiguration.EnableTPM == TPM_CHIP_DEVICE_ENABLE) {
    Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 0);
  } else if (ChipsetConfiguration.EnableTPM == TPM_FIRMWARE_DEVICE_ENABLE) {
    Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 1);
  }else if (ChipsetConfiguration.EnableTPM == HSP_TPM_FIRMWARE_DEVICE_ENABLE) {
    Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 2);
  } else {
    Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 0xFF);
  }
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Notify some PPI for special purpose.

  @param  *FfsHeader            Point to EFI_FFS_FILE_HEADER.
  @param  **PeiServices         Point to EFI_PEI_SERVICES.

  @retval EFI_SUCCESS           Store Notify success.
  @retval Others                Create Notify failed.
**/
EFI_STATUS
EFIAPI
InitPortConfigPcdsEntryPoint (
  IN       EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES           **PeiServices
  )
{
  EFI_STATUS                    Status;

  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyPpiList[0]);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesNotifyPpi (mUsb3PcdNotifyPpiList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesNotifyPpi (mSataPcdNotifyPpiList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesNotifyPpi (mPlatformCustomizeNotifyPpiList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesNotifyPpi (mHDAudioPcdNotifyPpiList);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  
  return EFI_SUCCESS;
}

