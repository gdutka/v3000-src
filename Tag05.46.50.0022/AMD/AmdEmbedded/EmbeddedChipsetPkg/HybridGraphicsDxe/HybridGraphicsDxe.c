/** @file
  This driver is for Hybrid Graphics Feature DXE initialize.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <HybridGraphicsDxe.h>
#include <SetupConfig.h>

//
// Function Prototypes
//
VOID
EFIAPI
HybridGraphicsBdsCallback (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  );

STATIC
EFI_STATUS
HybridGraphicsDxeInitialize (
  IN OUT HG_DXE_INFORMATION_DATA              **HgDxeInfoData,
  IN OUT HG_INFORMATION_DATA_HOB              **HgInfoDataHob
  );

STATIC
EFI_STATUS
NvidiaOptimusDxeInitialize (
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

STATIC
EFI_STATUS
InitMXMSupport (
  IN EFI_GUID                                 *MxmBinaryGuid,
  IN OUT UINTN                                *Address,
  IN OUT UINTN                                *Size,
  IN UINT8                                    BootType
  );

EFI_STATUS
EFIAPI
ControlNvidiaDgpuHdAudio (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  );

STATIC
EFI_STATUS
SearchDiscreteGraphicsVbios (
  IN OUT HG_DXE_INFORMATION_DATA              *HgDxeInfoData
  );

STATIC
EFI_STATUS
ExecuteDiscreteGraphicsVbios (
  IN OUT HG_DXE_INFORMATION_DATA              *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

STATIC
EFI_STATUS
SetHybridGraphicsSsdt (
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

STATIC
EFI_STATUS
InitializeOpRegion (
  IN EFI_ACPI_DESCRIPTION_HEADER              *NvStoreTable,
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

STATIC
VOID
UpdateHgOpRegion (
  IN OUT EFI_HG_NVS_AREA_PROTOCOL             *HgOpRegion,
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

STATIC
VOID
UpdateNvidiaOpRegion (
  IN OUT OPERATION_REGION_NVIDIA              *NvidiaOpRegion,
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

STATIC
EFI_STATUS
HgInformationUpdate (
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  );

EFI_STATUS
EFIAPI
SetSecondaryGrcphicsCommandRegister (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  );


VOID
EFIAPI
FreeHgDxeInfoData (
  IN OUT HG_DXE_INFORMATION_DATA              **HgDxeInfoData
  );

/**

  Hybrid Graphics feature driver entry point.
  This driver will handle secondary VBIOS and create feature own SSDT.

  @param[in] ImageHandle   Image handle of this driver.
  @param[in] SystemTable   Pointer to standard EFI system table.

  @retval EFI_SUCCESS    Hybrid Graphics feature DXE initialized successfully.
  @retval !EFI_SUCCESS   Hybrid Graphics feature doesn't be supported.

**/
EFI_STATUS
EFIAPI
HybridGraphicsDxeInitEntry (
  IN EFI_HANDLE                               ImageHandle,
  IN EFI_SYSTEM_TABLE                         *SystemTable
  )
{
  EFI_STATUS                                  Status;
  H2O_HG_INFO_PROTOCOL                        *HgInfoProtocol;
  VOID                                        *Registration;
  EFI_HG_NVS_AREA_PROTOCOL                    *HGNvsArea;

  HGNvsArea = AllocateReservedZeroPool (sizeof (EFI_HG_NVS_AREA_PROTOCOL));

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiHgNvsAreaProtocolGuid,
                  HGNvsArea,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (FeaturePcdGet (PcdHybridGraphicsSupported)) {
    EfiCreateProtocolNotifyEvent (
      &gH2OHybridGraphicsEventProtocolGuid,
      TPL_CALLBACK,
      HybridGraphicsBdsCallback,
      NULL,
      &Registration
      );

    //
    // Allocate and install Hybrid Graphics information Protocol.
    //
    HgInfoProtocol = AllocateReservedZeroPool (sizeof (H2O_HG_INFO_PROTOCOL));
    if (HgInfoProtocol == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &ImageHandle,
                    &gH2OHybridGraphicsInfoProtocolGuid,
                    HgInfoProtocol,
                    NULL
                    );

    return Status;
  }

  return EFI_SUCCESS;
}

/**

  After BDS platform driver connect Root Bridge will install HG protocol
  to trigger event to run this Hybrid Graphics feature callback,
  this specific timing discrete graphics already exist then we can
  handle and initialize it.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.

  @retval None.

**/
VOID
EFIAPI
HybridGraphicsBdsCallback (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  )
{
  EFI_STATUS                                  Status;
  HG_DXE_INFORMATION_DATA                     *HgDxeInfoData;
  HG_INFORMATION_DATA_HOB                     *HgInfoDataHob;

  //
  // Get HG information data HOB, and create HG DXE infromation Data.
  //
  Status = HybridGraphicsDxeInitialize (&HgDxeInfoData, &HgInfoDataHob);
  if (EFI_ERROR (Status) || HgDxeInfoData == NULL || HgInfoDataHob == NULL) {
    return;
  }

  //
  // Close Hybrid Graphics BDS event.
  //
  gBS->CloseEvent (Event);

  if (FeaturePcdGet (PcdNvidiaOptimusSupported)) {
    //
    // Get MXM SIS, Update global NVS area variable and set HG variable for nVIDIA Optimus.
    //
    Status = NvidiaOptimusDxeInitialize (HgDxeInfoData, HgInfoDataHob);
    if (EFI_ERROR (Status)) {
      FreeHgDxeInfoData (&HgDxeInfoData);
      return;
    }
  }

  //
  // Search discrete graphics VBIOS location, and initialize secondary ATI graphics VBIOS.
  //
  Status = SearchDiscreteGraphicsVbios (HgDxeInfoData);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Set HG SSDT and initialize HG own operation region.
  //
  Status = SetHybridGraphicsSsdt (HgDxeInfoData, HgInfoDataHob);
  if (EFI_ERROR (Status)) {
    FreeHgDxeInfoData (&HgDxeInfoData);
    return;
  }
  
  //
  // Update Hybrid Graphics infromation protocol
  //
  Status = HgInformationUpdate (HgDxeInfoData, HgInfoDataHob);
  if (EFI_ERROR (Status)) {
    FreeHgDxeInfoData (&HgDxeInfoData);
    return;
  }

}

/**

  Hybrid Graphics feature DXE driver initialize function.
  This function will get HG information data HOB, and create HG DXE infromation Data.

  @param[in, out] HgDxeInfoData   A double pointer of HG DXE information data structure,
                                  this driver or HG Operation Region will use these data.
  @param[in, out] HgInfoDataHob   A double pointer of HG information data HOB,
                                  HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval EFI_SUCCESS    This path initialized data successfully, and need handle HG SSDT and VBIOS.
  @retval !EFI_SUCCESS   This path initialized data failed and doesn't handle HG SSDT and VBIOS.

**/
STATIC
EFI_STATUS
HybridGraphicsDxeInitialize (
  IN OUT HG_DXE_INFORMATION_DATA              **HgDxeInfoData,
  IN OUT HG_INFORMATION_DATA_HOB              **HgInfoDataHob
  )
{
  EFI_STATUS                                  Status;
  UINT8                                       DgpuBridgeBus;
  UINT8                                       DgpuBus;
  UINT16                                      DgpuVendorId;
  UINTN                                       DgpuBridgeDevice;
  UINTN                                       DgpuBridgeFunction;
  UINTN                                       PciAddress;
  UINT16                                      DgpuDeviceId;

  (*HgDxeInfoData) = NULL;

  //
  // Get HG related information data HOB and SA config data HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, HgInfoDataHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  (*HgInfoDataHob) = GetNextGuidHob (&gH2OHgInformationDataHobGuid, (*HgInfoDataHob));
  if ((*HgInfoDataHob) == NULL) {
    return EFI_NOT_FOUND;
  }

  DgpuBridgeBus      = PcdGet8 (PcdHgPegBridgeBus);
  DgpuBridgeDevice   = PcdGet8 (PcdHgPegBridgeDevice);
  DgpuBridgeFunction = PcdGet8 (PcdHgPegBridgeFunction);


  //
  // Read discrete GPU Bus and Venodr ID.
  //
  PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                 DgpuBridgeBus,
                 DgpuBridgeDevice,
                 DgpuBridgeFunction,
                 PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                 );
  DgpuBus = PciExpressRead8 (PciAddress);
  PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                 DgpuBus,
                 DGPU_DEVICE_NUM,
                 DGPU_FUNCTION_NUM,
                 PCI_VENDOR_ID_OFFSET
                 );
  DgpuVendorId = PciExpressRead16 (PciAddress);
  PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                 DgpuBus,
                 DGPU_DEVICE_NUM,
                 DGPU_FUNCTION_NUM,
                 PCI_DEVICE_ID_OFFSET
                 );
  DgpuDeviceId = PciExpressRead16 (PciAddress);

  if ((DgpuBus == 0x00) || (DgpuBus == 0xFF) || (DgpuVendorId == 0xFFFF) ||
      ((**HgInfoDataHob).HgMode == DisplayModeHgDisabled)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate and initialize Hybrid Graphics DXE driver information data.
  //
  (*HgDxeInfoData) = AllocateZeroPool (sizeof (HG_DXE_INFORMATION_DATA));
  if ((*HgDxeInfoData) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  (**HgDxeInfoData).DgpuBridgeBus      = DgpuBridgeBus;
  (**HgDxeInfoData).DgpuBridgeDevice   = DgpuBridgeDevice;
  (**HgDxeInfoData).DgpuBridgeFunction = DgpuBridgeFunction;

  // (**HgDxeInfoData).NvidiaSecondaryGrcphicsCommandRegister = PcdGet8 (PcdNvidiaSecondaryGrcphicsCommandRegister);
  (**HgDxeInfoData).DgpuBus = DgpuBus;
  (**HgDxeInfoData).DgpuVendorId = DgpuVendorId;
  (**HgDxeInfoData).DgpuDeviceId = DgpuDeviceId;

  return EFI_SUCCESS;
}

/**

  nVIDIA Optimus DXE driver initialize function.
  This function will update global NVS area variable, set HG variable and load MXM SIS binary.

  @param[in, out] HgDxeInfoData   A pointer of HG DXE information data structure,
                                  this driver or HG Operation Region will use these data.
  @param[in]      HgInfoDataHob   A pointer of HG information data HOB,
                                  HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval EFI_SUCCESS    Update global NVS area variable and set HG variable both successfully.
  @retval !EFI_SUCCESS   Update global NVS area variable or set HG variable failed.

**/
STATIC
EFI_STATUS
NvidiaOptimusDxeInitialize (
  IN OUT HG_DXE_INFORMATION_DATA              *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  EFI_EVENT                                   ReadyToBootEvent;
  EFI_STATUS                                  Status;
  HG_VARIABLE_CONFIGURATION                   *HgVariableData;
  EFI_HG_NVS_AREA_PROTOCOL                    *HGNvsArea;
  UINT16                                      *HgNvidiaDgpuCheckTable;
  UINTN                                       Index;
  UINTN                                       Size;

  //
  // Base on different mode to load different MXM binary.
  //
  if (HgDxeInfoData->DgpuVendorId == NVIDIA_VID) {
    InitMXMSupport (
      PcdGetPtr (PcdHgModeMxmBinaryGuid),
      &(HgDxeInfoData->DgpuMxmBinFile.Address),
      &(HgDxeInfoData->DgpuMxmBinFile.Size),
      HgInfoDataHob->BootType
      );
  }
  //
  //  Locate HG NVS Protocol and update PEG Vendor ID.
  //
  Status = gBS->LocateProtocol (
                  &gEfiHgNvsAreaProtocolGuid,
                  NULL,
                  (VOID **)&HGNvsArea
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HgInfoDataHob->HgMode == DisplayModeHg || HgInfoDataHob->HgMode == DisplayModeDgpu) {
    HGNvsArea->DgpuVendorID = HgDxeInfoData->DgpuVendorId;
    if (HgDxeInfoData->DgpuVendorId == NVIDIA_VID) {
      //
      // Decide Nvidia dGPU type dynamically.
      // CRB will adopt N18 as default type.
      // Project should fill PcdHgNvidiaDgpuCheckTable to decide Nvidia dGPU type.
      //
      if (PcdGet8 (PcdDefaultBoardId) != 0xFF) {
        if (PcdGet8 (PcdDefaultBoardId) == 0x04) {
          HgDxeInfoData->NvDgpuGen = NV_GEN_18;
          HGNvsArea->NvDgpuGen = NV_GEN_18;
        } else {
          HgDxeInfoData->NvDgpuGen = NV_GEN_20;
          HGNvsArea->NvDgpuGen = NV_GEN_20;
        }
      } else {
        HgDxeInfoData->NvDgpuGen = NV_GEN_17;
        HGNvsArea->NvDgpuGen = NV_GEN_17;
        HgNvidiaDgpuCheckTable = (UINT16 *)PcdGetPtr (PcdHgNvidiaDgpuCheckTable);
        Size = PcdGetSize (PcdHgNvidiaDgpuCheckTable) / sizeof (UINT16);
        for (Index = 0; Index < Size; Index++) {
          if (HgNvidiaDgpuCheckTable[Index] == HgDxeInfoData->DgpuDeviceId) {
            HgDxeInfoData->NvDgpuGen = NV_GEN_18;
            HGNvsArea->NvDgpuGen = NV_GEN_18;
            break;
          }
        }
      }

      if (FeaturePcdGet (PcdHgNvidiaDdsFeatureSupport)) {
        CHIPSET_CONFIGURATION     ChipsetConfiguration;
        //
        // Update DisplayMode setting into Global NVS.
        //
        Size = sizeof (CHIPSET_CONFIGURATION);
        Status = gRT->GetVariable (
                        SETUP_VARIABLE_NAME,
                        &gSystemConfigurationGuid,
                        NULL,
                        &Size,
                        &ChipsetConfiguration
                        );
        HGNvsArea->DisplayMode = ChipsetConfiguration.DisplayMode;
      }

      //
      // If NVIDIA HG variable doesn't exist, set the default value.
      //
      Status = GetVariable2 (
                 L"HybridGraphicsVariable",
                 &gH2OHybridGraphicsVariableGuid,
                 &HgVariableData, 
                 &Size
                 );
      if (HgVariableData == NULL || EFI_ERROR (Status)) {
        HgVariableData = AllocateZeroPool (sizeof (HG_VARIABLE_CONFIGURATION));
        if (HgVariableData == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        HgVariableData->OptimusVariable.OptimusFlag = Inactive;
        HgVariableData->OptimusVariable.DgpuBus  = HgDxeInfoData->DgpuBus;
        HgVariableData->OptimusVariable.NvDgpuGen   = HgDxeInfoData->NvDgpuGen;
        Status = gRT->SetVariable (
                        L"HybridGraphicsVariable",
                        &gH2OHybridGraphicsVariableGuid,
                        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                        sizeof (HG_VARIABLE_CONFIGURATION),
                        HgVariableData
                        );
        if (EFI_ERROR (Status)) {
          FreePool (HgVariableData);
          return Status;
        }
      }
      HgDxeInfoData->OptimusFlag = HgVariableData->OptimusVariable.OptimusFlag;
      Status = EfiCreateEventReadyToBootEx (
                 TPL_CALLBACK,
                 ControlNvidiaDgpuHdAudio,
                 HgDxeInfoData,
                 &ReadyToBootEvent
                 );
      FreePool (HgVariableData);
    }
  }

  return Status;
}

/**

  Get MXM (Mobile PCI Express Module) SIS (System Information Structure) binary from firmware volume,
  legacy boot allocate 0xE0000 legacy memory region and copy MXM SIS binary into this region
  for MXM INT 15 callback function to get this binary,
  EFI boot just get the MXM SIS binary firmware volume address.

  @param[in]      MxmBinaryGuid   MXM SIS binary GUID for search MXM SIS binary from irmware volume.
  @param[in, out] Address         EFI boot firmware volume MXM SIS binary address
                                  or legacy boot allocated 0xE0000 legacy region MXM SIS binary address.
  @param[in, out] Size            MXM SIS binary size.
  @param[in]      BootType        SCU Boot Type setting.

  @retval EFI_SUCCESS    Get MXM SIS binary successfully.
  @retval !EFI_SUCCESS   Get MXM SIS binary failed.

**/
STATIC
EFI_STATUS
InitMXMSupport (
  IN EFI_GUID                                 *MxmBinaryGuid,
  IN OUT UINTN                                *Address,
  IN OUT UINTN                                *Size,
  IN UINT8                                    BootType
  )
{
  EFI_COMPATIBILITY16_TABLE                   *EfiTable;
  EFI_IA32_REGISTER_SET                       Regs;
  EFI_LEGACY_BIOS_PROTOCOL                    *LegacyBios;
  EFI_LEGACY_REGION2_PROTOCOL                 *LegacyRegion;
  EFI_STATUS                                  Status;
  UINT8                                       *Ptr;
  UINTN                                       *BuffPtr;
  UINTN                                       BufferSize;
  UINTN                                       TablePtr;

  EfiTable = NULL;
  BuffPtr  = NULL;
  (*Address) = 0;
  (*Size)    = 0;

  //
  // Locate MXM Binary
  //
  Status = GetSectionFromAnyFv (MxmBinaryGuid, EFI_SECTION_RAW, 0, &BuffPtr, &BufferSize);
  if (EFI_ERROR (Status) || BufferSize == 0) {
    return Status;
  }
  if (BootType == EfiBootType) {
    (*Address) = (UINTN)(VOID *)BuffPtr;
    (*Size)    = BufferSize;
  }

  //
  // Locate Legacy Bios and Legacy Region Protocol.
  //
  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->LocateProtocol (&gEfiLegacyRegion2ProtocolGuid, NULL, (VOID **)&LegacyRegion);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get EFI table from F segment.
  //
  for (Ptr = (UINT8 *)((UINTN)0xFE000); Ptr < (UINT8 *) ((UINTN) 0x100000); Ptr += 0x10) {
    if (*(UINT32 *) Ptr == SIGNATURE_32 ('I', 'F', 'E', '$')) {
      EfiTable = (EFI_COMPATIBILITY16_TABLE *)Ptr;
      break;
    }
  }
  if (EfiTable == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = LegacyRegion->UnLock (LegacyRegion, 0xE0000, 0x20000, NULL);
  //
  // To find the required size of availabe free memory
  //
  ZeroMem (&Regs, sizeof (EFI_IA32_REGISTER_SET));
  Regs.X.AX = Legacy16GetTableAddress;
  Regs.X.BX = 0x02; // E0000Region
  Regs.X.CX = (UINT16)BufferSize;
  Regs.X.DX = 1;
  Status = LegacyBios->FarCall86 (
                         LegacyBios,
                         EfiTable->Compatibility16CallSegment,
                         EfiTable->Compatibility16CallOffset,
                         &Regs,
                         NULL,
                         0
                         );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // To copy the image to legacy memory
  //
  TablePtr = (UINT32) ((Regs.X.DS << 4) + Regs.X.BX);
  CopyMem ((VOID *)TablePtr, (VOID *)BuffPtr, BufferSize);
  (*Address) = TablePtr;
  (*Size)    = BufferSize;
  Status = LegacyRegion->Lock (LegacyRegion, 0xE0000, 0x20000, NULL);

  return Status;
}

/**

  Enable or disable dGPU HD Audio device.

  @param[in]  Event     The Event this notify function registered to.
  @param[in]  Context   Pointer to the context data registerd to the Event.

  @retval EFI_SUCCESS   Set secondary Graphics command register successfully.

**/
EFI_STATUS
EFIAPI
ControlNvidiaDgpuHdAudio (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  )
{
  EFI_BOOT_MODE                               BootMode;
  EFI_PEI_HOB_POINTERS                        Hob;
  EFI_STATUS                                  Status;
  HG_DXE_INFORMATION_DATA                     *HgDxeInfoData;
  UINTN                                       PciAddress;
  VOID                                        *HobList;

  HgDxeInfoData = (HG_DXE_INFORMATION_DATA *)Context;
  if (HgDxeInfoData == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Hob.Raw = HobList;
  if (GET_HOB_TYPE (Hob) != EFI_HOB_TYPE_HANDOFF) {
    return Status;
  }
  BootMode = Hob.HandoffInformationTable->BootMode;

  PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                 HgDxeInfoData->DgpuBus,
                 DGPU_DEVICE_NUM,
                 DGPU_FUNCTION_NUM,
                 NVIDIA_DGPU_HDA_REGISTER
                 );

  if ((BootMode != BOOT_ON_S3_RESUME) && (BootMode != BOOT_ON_S4_RESUME)) {
    if (HgDxeInfoData->NvDgpuGen == NV_GEN_17) {
      //
      // N17: Disable dGPU HDA during normal boot.
      //
      PciExpressAnd32 (PciAddress, (UINT32)~(BIT25));
    } else {
      //
      // N18/N20: Enable dGPU HDA during normal boot.
      //
      PciExpressOr32 (PciAddress, (UINT32)BIT25);
    }
  }

  if (BootMode == BOOT_ON_S4_RESUME) {
    if (HgDxeInfoData->NvDgpuGen == NV_GEN_17) {
      //
      // Sync the value of OptimusFlag to global NVS during S4 resume.
      //
      ((EFI_HG_NVS_AREA_PROTOCOL *)(HgDxeInfoData->HgOpRegionAddress))->OptimusFlag = HgDxeInfoData->OptimusFlag;

      //
      // N17: Based on the value of OptimusFlag to enable or disable dGPU HDA during S4 resume.
      //
      if (((HgDxeInfoData->OptimusFlag) & BIT0) == Inactive) {
        PciExpressAnd32 (PciAddress, (UINT32)~(BIT25));
      } else {
        PciExpressOr32 (PciAddress, (UINT32)BIT25);
      }
    } else {
      //
      // N18/N20: Enable dGPU HDA during S4 resume.
      //
      PciExpressOr32 (PciAddress, (UINT32)BIT25);
    }
  }

  gBS->CloseEvent (Event);

  return EFI_SUCCESS;
}

/**

  Search discrete graphics VBIOS location and save into HG information data.

  @param[in, out] HgDxeInfoData   A pointer of HG DXE information data structure,
                                  this driver or HG Operation Region will use these data.

  @retval EFI_SUCCESS    Search discrete graphics VBIOS successfully.
  @retval !EFI_SUCCESS   Search discrete graphics VBIOS failed.

**/
STATIC
EFI_STATUS
SearchDiscreteGraphicsVbios (
  IN OUT HG_DXE_INFORMATION_DATA              *HgDxeInfoData
  )
{
  EFI_HANDLE                                  *HandleBuffer;
  EFI_PCI_IO_PROTOCOL                         *PciIo;
  EFI_STATUS                                  Status;
  PCI_3_0_DATA_STRUCTURE                      *PcirBlockPtr;
  PCI_EXPANSION_ROM_HEADER                    *VBiosRomImage;
  UINTN                                       HandleCount;
  UINTN                                       Index;

  //
  // Get all PCI IO protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Find the video BIOS by checking each PCI IO handle for DGPU video
  // BIOS OPROM.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID**)&PciIo
                    );
    if (EFI_ERROR (Status) || (PciIo->RomImage == NULL)) {
      //
      // If this PCI device doesn't have a ROM image, skip to the next device.
      //
      continue;
    }
    VBiosRomImage = PciIo->RomImage;

    //
    // Get pointer to PCIR structure
    //
    PcirBlockPtr = (PCI_3_0_DATA_STRUCTURE *)((UINTN)VBiosRomImage + VBiosRomImage->PcirOffset);

    //
    // Check if we have an video BIOS OPROM for DGPU.
    //
    if ((VBiosRomImage->Signature == PCI_EXPANSION_ROM_HEADER_SIGNATURE) &&
        (HgDxeInfoData->DgpuVendorId == PcirBlockPtr->VendorId) &&
        (PcirBlockPtr->ClassCode[2] == PCI_CLASS_DISPLAY)) {
      HgDxeInfoData->Vbios.Address = (UINTN)PciIo->RomImage;
      HgDxeInfoData->Vbios.Size = (UINT32)PciIo->RomSize;
      if (HgDxeInfoData->Vbios.Size > sizeof (OPERATION_REGION_VBIOS)) {
        DEBUG((DEBUG_ERROR, "\nASSERT! Size too small:"));
        ASSERT(FALSE);
      }
      FreePool (HandleBuffer);
      return EFI_SUCCESS;
    }
  }
  FreePool (HandleBuffer);

  return EFI_NOT_FOUND;
}

/**

  Execute discrete graphics VBIOS for ATI graphics initialize.

  @param[in, out] HgDxeInfoData   A pointer of HG DXE information data structure,
                                  this driver or HG Operation Region will use these data.
  @param[in]      HgInfoDataHob   A pointer of HG information data HOB,
                                  HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval EFI_SUCCESS    Execute discrete graphics VBIOS successfully.
  @retval !EFI_SUCCESS   Execute discrete graphics VBIOS failed.

**/
STATIC
EFI_STATUS
ExecuteDiscreteGraphicsVbios (
  IN OUT HG_DXE_INFORMATION_DATA              *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  EFI_IA32_REGISTER_SET                       RegSet;
  EFI_LEGACY_BIOS_PROTOCOL                    *LegacyBios;
  EFI_PHYSICAL_ADDRESS                        ImageLocation;
  EFI_STATUS                                  Status;
  UINTN                                       Offset;
  UINTN                                       PegBridgeCmdRegAddr;
  UINTN                                       PegDeviceCmdRegAddr;
  UINTN                                       VbiosPages;
  VOID                                        *VbiosAddress;

  //
  // Memory IO Bus Master needs to be enabled when we execute the VBIOS.
  //
  PegBridgeCmdRegAddr = PCI_EXPRESS_LIB_ADDRESS (
                          HgDxeInfoData->DgpuBridgeBus,
                          HgDxeInfoData->DgpuBridgeDevice,
                          HgDxeInfoData->DgpuBridgeFunction,
                          PCI_COMMAND_OFFSET
                          );
  PegDeviceCmdRegAddr = PCI_EXPRESS_LIB_ADDRESS (
                          HgDxeInfoData->DgpuBus,
                          DGPU_DEVICE_NUM,
                          DGPU_FUNCTION_NUM,
                          PCI_COMMAND_OFFSET
                          );
  //
  // Enable Memory Access, IO Access Bus Master enable on PEG root port and PEG device.
  //
  PciExpressOr16 (PegBridgeCmdRegAddr, BIT0 + BIT1 + BIT2);
  PciExpressOr16 (PegDeviceCmdRegAddr, BIT0 + BIT1 + BIT2);
  //
  // Allocate under 1MB memory region (less than 640 KB).
  //
  VbiosPages = ((HgDxeInfoData->Vbios.Size) / 0x1000) + 1;
  ImageLocation = (EFI_PHYSICAL_ADDRESS)(UINTN)0xA0000;
  Status  = gBS->AllocatePages (AllocateMaxAddress, EfiBootServicesCode, VbiosPages, &ImageLocation);
  if (!EFI_ERROR (Status)) {
    ZeroMem ((VOID *)(UINTN)ImageLocation, (VbiosPages * 4096));
    //
    // After allocation copy VBIOS to buffer
    //
    CopyMem ((VOID *)(UINTN)ImageLocation, (VOID *)HgDxeInfoData->Vbios.Address, HgDxeInfoData->Vbios.Size);
    Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
    if (!EFI_ERROR (Status)) {
      ZeroMem (&RegSet, sizeof (EFI_IA32_REGISTER_SET));
      RegSet.H.AH = HgDxeInfoData->DgpuBus;
      Offset = MemoryRead16 ((UINTN)ImageLocation + 0x40);
      LegacyBios->FarCall86 (
                    LegacyBios,
                    ((UINT16)(RShiftU64 ((ImageLocation & 0x000FFFF0), 4))),
                    ((UINT16)Offset),
                    &RegSet,
                    NULL,
                    0
                    );
      Offset = (MemoryRead16 ((UINTN)ImageLocation + 0x42)) + (UINTN)ImageLocation;
      if (MemoryRead16 ((UINTN)ImageLocation + 0x44) == 0x0) {
        HgDxeInfoData->Vbios.Size = (MemoryRead8 ((UINTN)ImageLocation + 0x2)) * 512;
      } else {
        HgDxeInfoData->Vbios.Size = (MemoryRead16 ((UINTN)ImageLocation + 0x44)) * 512;
      }
      //
      // Copy Oprom to allocated space
      //
      VbiosAddress = AllocateZeroPool (HgDxeInfoData->Vbios.Size);
      CopyMem (VbiosAddress, (VOID *)Offset, HgDxeInfoData->Vbios.Size);
      HgDxeInfoData->Vbios.Address = (UINTN)VbiosAddress;
      ZeroMem ((VOID *)(UINTN)ImageLocation, HgDxeInfoData->Vbios.Size);
    }
    FreePages ((VOID *)(UINTN)ImageLocation, VbiosPages);
  }
  //
  // Disable Memory Access, IO Access Bus Master enable on PEG device and PEG root port.
  //
  PciExpressAnd16 (PegDeviceCmdRegAddr, BIT0 + BIT1 + BIT2);
  PciExpressAnd16 (PegBridgeCmdRegAddr, BIT0 + BIT1 + BIT2);

  return Status;
}

/**

  Load Hybrid Graphics own SSDT (Secondary System Description Table) and initialize
  Hybrid Graphics own operation region for ASL (ACPI Source Language) code usage.

  @param[in] HgDxeInfoData   A pointer of HG DXE information data structure,
                             this driver or HG Operation Region will use these data.
  @param[in] HgInfoDataHob   A pointer of HG information data HOB,
                             HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval EFI_SUCCESS    Load and set Hybrid Graphics SSDT successfully.
  @retval !EFI_SUCCESS   Load or set Hybrid Graphics SSDT failed.

**/
STATIC
EFI_STATUS
SetHybridGraphicsSsdt (
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  EFI_ACPI_COMMON_HEADER                      *CurrentTable;
  EFI_ACPI_DESCRIPTION_HEADER                 *TempTable;
  EFI_ACPI_TABLE_PROTOCOL                     *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL               *FirmwareVolume;
  EFI_FV_FILE_ATTRIBUTES                      Attributes;
  EFI_FV_FILETYPE                             FileType;
  EFI_GUID                                    *SsdtFileGuid;
  EFI_HANDLE                                  *HandleBuffer;
  EFI_STATUS                                  Status;
  UINT32                                      FvStatus;
  UINTN                                       TableKey;
  UINTN                                       Index;
  UINTN                                       NumberOfHandles;
  UINTN                                       Size;

  CurrentTable   = NULL;
  FirmwareVolume = NULL;
  SsdtFileGuid   = NULL;
  TempTable      = NULL;

  //
  // Choose the SSDT table base on Vendor ID and HG mode.
  //
  if ((FeaturePcdGet (PcdNvidiaOptimusSupported)) && (HgDxeInfoData->DgpuVendorId == NVIDIA_VID)) {
    if (HgInfoDataHob->HgMode == DisplayModeDgpu) {
      switch (HgDxeInfoData->NvDgpuGen) {
        case NV_GEN_17:
          SsdtFileGuid = PcdGetPtr (PcdNvidiaDiscreteSsdtN17Guid);
          break;
        case NV_GEN_18:
          SsdtFileGuid = PcdGetPtr (PcdNvidiaDiscreteSsdtN18Guid);
          break;
        case NV_GEN_20:
          SsdtFileGuid = PcdGetPtr (PcdNvidiaDiscreteSsdtN20Guid);
          break;  
      }
    } else {
      switch (HgDxeInfoData->NvDgpuGen) {
        case NV_GEN_17:
          SsdtFileGuid = PcdGetPtr (PcdNvidiaOptimusSsdtN17Guid);
          break;
        case NV_GEN_18:
          SsdtFileGuid = PcdGetPtr (PcdNvidiaOptimusSsdtN18Guid);
          break;
        case NV_GEN_20:
          SsdtFileGuid = PcdGetPtr (PcdNvidiaOptimusSsdtN20Guid);
          break;  
      }
    }
  }

  if (SsdtFileGuid == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (NumberOfHandles == 0)) {
    return Status;
  }
  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiFirmwareVolume2ProtocolGuid, (VOID**)&FirmwareVolume);
    if (EFI_ERROR (Status)) {
      continue;
    }
    FvStatus = 0;
    Size     = 0;
    Status = FirmwareVolume->ReadFile (
                               FirmwareVolume,
                               SsdtFileGuid,
                               NULL,
                               &Size,
                               &FileType,
                               &Attributes,
                               &FvStatus
                               );
    if (Status == EFI_SUCCESS) {
      break;
    }
  }
  FreePool (HandleBuffer);
  ASSERT (FirmwareVolume != NULL);
  if (EFI_ERROR (Status) || FirmwareVolume == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Read tables from the storage file.
  //
  TableKey = 0;
  Status = FirmwareVolume->ReadSection (
                             FirmwareVolume,
                             SsdtFileGuid,
                             EFI_SECTION_RAW,
                             0,
                             (VOID**)&CurrentTable,
                             &Size,
                             &FvStatus
                             );
  if (!EFI_ERROR (Status)) {
    TempTable = AllocateZeroPool (CurrentTable->Length);
    ASSERT (TempTable != NULL);
    if (TempTable != NULL) {
      CopyMem (TempTable, CurrentTable, CurrentTable->Length);
      Status = InitializeOpRegion ((EFI_ACPI_DESCRIPTION_HEADER*)TempTable, HgDxeInfoData, HgInfoDataHob);
      if (!EFI_ERROR (Status)) {
        Status = AcpiTable->InstallAcpiTable (AcpiTable, TempTable, CurrentTable->Length, &TableKey);
      }
      FreePool (CurrentTable);
      FreePool (TempTable);
    }
  }

  return Status;
}

/**

  Initialize Hybrid Graphics own operation region for ASL (ACPI Source Language) code usage.

  @param[in] NvStoreTable    A pointer of temporary HG SSDT that prepares modified and installed.
  @param[in] HgDxeInfoData   A pointer of HG DXE information data structure,
                             this driver or HG Operation Region will use these data.
  @param[in] HgInfoDataHob   A pointer of HG information data HOB,
                             HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval EFI_SUCCESS    Load Hybrid Graphics SSDT successfully.
  @retval !EFI_SUCCESS   Load Hybrid Graphics SSDT failed.

**/
STATIC
EFI_STATUS
InitializeOpRegion (
  IN EFI_ACPI_DESCRIPTION_HEADER              *NvStoreTable,
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  EFI_ACPI_DESCRIPTION_HEADER                 *Table;
  EFI_STATUS                                  Status;
  OPERATION_REGION_NVIDIA                     *NvidiaOpRegion;
  EFI_HG_NVS_AREA_PROTOCOL                    *HGNvsArea;
  OPERATION_REGION_VBIOS                      *VbiosOpRegion;
  UINT8                                       *SsdtPointer;
  UINT32                                      *Signature;

  Table = NvStoreTable;
  for (SsdtPointer =  (UINT8 *)((UINTN)Table + sizeof (EFI_ACPI_DESCRIPTION_HEADER)); \
       SsdtPointer <= (UINT8 *)((UINTN)Table + (UINTN)(Table->Length)); SsdtPointer++) {
    Signature = (UINT32*)SsdtPointer;
    switch (*Signature) {
      case (SIGNATURE_32 ('H', 'G', 'O', 'P')): // HG operation region initialize
        if (IsAmlOpRegionObject (SsdtPointer)) {
          Status = gBS->LocateProtocol(&gEfiHgNvsAreaProtocolGuid, NULL, (VOID **)&HGNvsArea);
          if (!EFI_ERROR (Status)) {
            UpdateHgOpRegion (HGNvsArea, HgDxeInfoData, HgInfoDataHob);
            SetOpRegion (SsdtPointer, HGNvsArea, sizeof (EFI_HG_NVS_AREA_PROTOCOL));
          }
        }
        break;
      case (SIGNATURE_32 ('V', 'B', 'O', 'R')): // VBIOS operation region initialize
        if (IsAmlOpRegionObject (SsdtPointer)) {
          Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (OPERATION_REGION_VBIOS), (VOID **)&VbiosOpRegion);
          if (!EFI_ERROR (Status)) {
            ZeroMem (VbiosOpRegion, sizeof (OPERATION_REGION_VBIOS));
            VbiosOpRegion->RVBS = (UINT32)HgDxeInfoData->Vbios.Size;
            CopyMem (
              (VOID *)(VbiosOpRegion->VBOIS),
              (VOID *)(HgDxeInfoData->Vbios.Address),
              HgDxeInfoData->Vbios.Size
              );
            HgDxeInfoData->Vbios.Address = (UINTN)(VbiosOpRegion->VBOIS);
            SetOpRegion (SsdtPointer, VbiosOpRegion, sizeof (OPERATION_REGION_VBIOS));
          }
        }
        break;

      case (SIGNATURE_32 ('N', 'O', 'P', 'R')): // nVIDIA operation region initialize
        if ((FeaturePcdGet (PcdNvidiaOptimusSupported)) && IsAmlOpRegionObject (SsdtPointer)) {
          Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (OPERATION_REGION_NVIDIA), (VOID **)&NvidiaOpRegion);
          if (!EFI_ERROR (Status)) {
            ZeroMem (NvidiaOpRegion, sizeof (OPERATION_REGION_NVIDIA));
            UpdateNvidiaOpRegion (NvidiaOpRegion, HgDxeInfoData, HgInfoDataHob);
            SetOpRegion (SsdtPointer, NvidiaOpRegion, sizeof (OPERATION_REGION_NVIDIA));
          }
        }
        SsdtPointer = (UINT8 *)((UINTN)Table + (UINTN)(Table->Length));
        break;
    }
  }

  return EFI_SUCCESS;
}

/**

  Update Hybrid Graphics own operation region for ASL (ACPI Source Language) code usage.

  @param[in] HgOpRegion      A pointer of HG own operation region.
  @param[in] HgDxeInfoData   A pointer of HG DXE information data structure,
                             this driver or HG Operation Region will use these data.
  @param[in] HgInfoDataHob   A pointer of HG information data HOB,
                             HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval None.

**/
STATIC
VOID
UpdateHgOpRegion (
  IN OUT EFI_HG_NVS_AREA_PROTOCOL             *HgOpRegion,
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  UINTN                                       PciAddress;

  PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                 HgDxeInfoData->DgpuBridgeBus,
                 HgDxeInfoData->DgpuBridgeDevice,
                 HgDxeInfoData->DgpuBridgeFunction,
                 0x0
                 );
  HgOpRegion->DgpuPcieCfgBaseAddress = (UINT32)(PcdGet64 (PcdPciExpressBaseAddress) + PciAddress);
  HgDxeInfoData->HgOpRegionAddress = HgOpRegion;

  if (HgInfoDataHob->HgMode == DisplayModeHg || HgInfoDataHob->HgMode == DisplayModeDgpu) {
    //
    // PEG Endpoint Base Addresses and Capability Structure Offsets for ASL usage
    //
    PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                   HgDxeInfoData->DgpuBus,
                   DGPU_DEVICE_NUM,
                   DGPU_FUNCTION_NUM,
                   0x0
                   );
    HgOpRegion->EndpointBaseAddress = (UINT32)(PcdGet64 (PcdPciExpressBaseAddress) + PciAddress);
  }
}

/**
  Update nVIDIA own operation region for ASL (ACPI Source Language) code usage.

  @param[in] NvidiaOpRegion   A pointer of nVIDIA own operation region.
  @param[in] HgDxeInfoData    A pointer of HG DXE information data structure,
                              this driver or HG Operation Region will use these data.
  @param[in] HgInfoDataHob    A pointer of HG information data HOB,
                              HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval None.

**/
STATIC
VOID
UpdateNvidiaOpRegion (
  IN OUT OPERATION_REGION_NVIDIA              *NvidiaOpRegion,
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  if (HgInfoDataHob->HgMode == DisplayModeHg) {
    NvidiaOpRegion->DgpuHotPlugSupport      = PcdGetBool (PcdHgNvidiaOptimusDgpuHotPlugSupport);
    NvidiaOpRegion->DgpuPowerControlSupport = PcdGetBool (PcdHgNvidiaOptimusDgpuPowerControlSupport);
    NvidiaOpRegion->GpsFeatureSupport       = PcdGetBool (PcdHgNvidiaGpsFeatureSupport);
    NvidiaOpRegion->VenturaFeatureSupport   = PcdGetBool (PcdHgNvidiaVenturaFeatureSupport);
    NvidiaOpRegion->NbciFeatureSupport      = PcdGetBool (PcdHgNvidiaNbciFeatureSupport);
    NvidiaOpRegion->OptimusGc6Support       = PcdGetBool (PcdHgNvidiaOptimusGc6FeatureSupport);
    NvidiaOpRegion->OptimusGc6NvsrSupport   = PcdGetBool (PcdHgNvidiaOptimusGc6NvsrFeatureSupport);
    NvidiaOpRegion->NpcfFeatureSupport      = PcdGetBool (PcdHgNvidiaNpcfFeatureSupport);
    NvidiaOpRegion->N20CTGPFeatureSupport   = PcdGetBool (PcdHgNvidiaN20CTGPFeatureSupport);
    NvidiaOpRegion->SpbConfig = Active; // SpbConfig bit0: SPB 1: enable, 0: disable
    NvidiaOpRegion->MxmBinarySize = (UINT32)HgDxeInfoData->DgpuMxmBinFile.Size; // MXM bin file size (bits)
    NvidiaOpRegion->Gc6FbEn = FixedPcdGet32 (PcdNvidiaGC6FBEN);
    NvidiaOpRegion->Gc6Event = FixedPcdGet32 (PcdNvidiaGPUEvent);
    NvidiaOpRegion->PowerGood = HgInfoDataHob->DgpuPwrGoodGpioNo;
    NvidiaOpRegion->DdsMuxModePin = FixedPcdGet32 (PcdNvidiaDDSMuxModePin);
    NvidiaOpRegion->DdsLcdForceResetPin = FixedPcdGet32 (PcdNvidiaDDSLcdForceResetPin);
    //
    // Copy MXM bin file to OpRegion
    //
    CopyMem (
      (VOID *)(NvidiaOpRegion->MxmBinaryBuffer),
      (VOID *)(HgDxeInfoData->DgpuMxmBinFile.Address),
      HgDxeInfoData->DgpuMxmBinFile.Size
      );
  }
  if (HgInfoDataHob->HgMode == DisplayModeDgpu) {
    NvidiaOpRegion->DgpuHotPlugSupport      = PcdGetBool (PcdHgNvidiaOptimusDgpuHotPlugSupport);
    NvidiaOpRegion->DgpuPowerControlSupport = PcdGetBool (PcdHgNvidiaOptimusDgpuPowerControlSupport);NvidiaOpRegion->DgpuHotPlugSupport      = PcdGetBool (PcdHgNvidiaOptimusDgpuHotPlugSupport);
    NvidiaOpRegion->DgpuPowerControlSupport = PcdGetBool (PcdHgNvidiaOptimusDgpuPowerControlSupport);
    NvidiaOpRegion->GpsFeatureSupport       = PcdGetBool (PcdHgNvidiaGpsFeatureSupport);
    NvidiaOpRegion->VenturaFeatureSupport   = PcdGetBool (PcdHgNvidiaVenturaFeatureSupport);
    NvidiaOpRegion->NbciFeatureSupport      = PcdGetBool (PcdHgNvidiaNbciFeatureSupport);
    NvidiaOpRegion->OptimusGc6Support       = PcdGetBool (PcdHgNvidiaOptimusGc6FeatureSupport);
    NvidiaOpRegion->OptimusGc6NvsrSupport   = PcdGetBool (PcdHgNvidiaOptimusGc6NvsrFeatureSupport);
    NvidiaOpRegion->NpcfFeatureSupport      = PcdGetBool (PcdHgNvidiaNpcfFeatureSupport);
    NvidiaOpRegion->N20CTGPFeatureSupport   = PcdGetBool (PcdHgNvidiaN20CTGPFeatureSupport);
    NvidiaOpRegion->SpbConfig = Active; // SpbConfig bit0: SPB 1: enable, 0: disable
    NvidiaOpRegion->SlaveDgpuSupport = Inactive;
    NvidiaOpRegion->MxmBinarySize = (UINT32)HgDxeInfoData->DgpuMxmBinFile.Size; // MXM bin file size (bits)
    NvidiaOpRegion->Gc6FbEn = FixedPcdGet32 (PcdNvidiaGC6FBEN);
    NvidiaOpRegion->Gc6Event = FixedPcdGet32 (PcdNvidiaGPUEvent);
    NvidiaOpRegion->PowerGood = HgInfoDataHob->DgpuPwrGoodGpioNo;
    NvidiaOpRegion->DdsMuxModePin = FixedPcdGet32 (PcdNvidiaDDSMuxModePin);
    NvidiaOpRegion->DdsLcdForceResetPin = FixedPcdGet32 (PcdNvidiaDDSLcdForceResetPin);
    //
    // Copy MXM bin file to OpRegion
    //
    CopyMem (
      (VOID *)(NvidiaOpRegion->MxmBinaryBuffer),
      (VOID *)(HgDxeInfoData->DgpuMxmBinFile.Address),
      HgDxeInfoData->DgpuMxmBinFile.Size
      );
    NvidiaOpRegion->SlaveMxmBinarySize = (UINT32)HgDxeInfoData->Dgpu2MxmBinFile.Size;
    //
    // Copy Slave MXM bin file to OpRegion
    //
    CopyMem (
      (VOID *)(NvidiaOpRegion->SlaveMxmBinaryBuffer),
      (VOID *)(HgDxeInfoData->Dgpu2MxmBinFile.Address),
      HgDxeInfoData->Dgpu2MxmBinFile.Size
      );
  }
}

/**

  Update Hybrid Graphics information protocol related data for nVIDIA INT 15 callback usage.
  Create Exit Boot Service event and Legacy Boot event to set the Secondary Grcphics Command Register.

  @param[in] HgDxeInfoData   A pointer of HG DXE information data structure,
                             this driver or HG Operation Region will use these data.
  @param[in] HgInfoDataHob   A pointer of HG information data HOB,
                             HG PEI Module created this HOB, and passes the data from PEI phase.

  @retval EFI_SUCCESS    Update Hybrid Graphics information protocol successfully.
  @retval !EFI_SUCCESS   Locate Hybrid Graphics information protocol failed.

**/
STATIC
EFI_STATUS
HgInformationUpdate (
  IN HG_DXE_INFORMATION_DATA                  *HgDxeInfoData,
  IN HG_INFORMATION_DATA_HOB                  *HgInfoDataHob
  )
{
  // EFI_EVENT                                   ExitBootServicesEvent;
  // EFI_EVENT                                   LegacyBootEvent;
  // EFI_EVENT                                   ReadyToBootEvent;
  EFI_STATUS                                  Status;
  H2O_HG_INFO_PROTOCOL                        *HgInfoProtocol;
  UINTN                                       PciAddress;

  Status = gBS->LocateProtocol (&gH2OHybridGraphicsInfoProtocolGuid, NULL, (VOID **)&HgInfoProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HgInfoProtocol->HgMode = HgInfoDataHob->HgMode;
  if (FeaturePcdGet (PcdNvidiaOptimusSupported)) {
    HgInfoProtocol->Vbios.Address           = HgDxeInfoData->Vbios.Address;
    HgInfoProtocol->Vbios.Size              = HgDxeInfoData->Vbios.Size;
    HgInfoProtocol->DgpuMxmBinFile.Address  = HgDxeInfoData->DgpuMxmBinFile.Address;
    HgInfoProtocol->DgpuMxmBinFile.Size     = HgDxeInfoData->DgpuMxmBinFile.Size;
  }
  //
  // Assign the value of dGPU SSID/SVID to global NVS.
  //
  PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                 HgDxeInfoData->DgpuBus,
                 DGPU_DEVICE_NUM,
                 DGPU_FUNCTION_NUM,
                 PCI_SVID_OFFSET
                 );
  ((EFI_HG_NVS_AREA_PROTOCOL *)(HgDxeInfoData->HgOpRegionAddress))->DgpuSsidSvid = PciExpressRead32 (PciAddress);
  // if (HgInfoDataHob->BootType == EfiBootType) {
  //   Status = gBS->CreateEvent (
  //                   EVT_SIGNAL_EXIT_BOOT_SERVICES,
  //                   TPL_NOTIFY,
  //                   SetSecondaryGrcphicsCommandRegister,
  //                   HgDxeInfoData,
  //                   &ExitBootServicesEvent
  //                   );
  // } else {
  //   Status = EfiCreateEventLegacyBootEx (
  //              TPL_NOTIFY,
  //              SetSecondaryGrcphicsCommandRegister,
  //              HgDxeInfoData,
  //              &LegacyBootEvent
  //              );
  // }

  return Status;
}

/**

  Set Secondary Graphics Command Register for avoid I/O resource crash.

  @param[in]  Event     The Event this notify function registered to.
  @param[in]  Context   Pointer to the context data registerd to the Event.

  @retval EFI_SUCCESS   Set secondary Graphics command register successfully.

**/
EFI_STATUS
EFIAPI
SetSecondaryGrcphicsCommandRegister (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  )
{
  HG_DXE_INFORMATION_DATA                     *HgDxeInfoData;
  UINT8                                       Data;
  UINTN                                       PciAddress;

  HgDxeInfoData = (HG_DXE_INFORMATION_DATA *)Context;
  if (HgDxeInfoData == NULL) {
    return EFI_UNSUPPORTED;
  }
  Data = 0x07;
    //
    // If dGPU and iGPU exist at the same time, close dGPU IO port.
    // Different vendor requests to write command register with different value.
    //
    if ((HgDxeInfoData->DgpuVendorId) != 0xFFFF) {
      PciAddress = PCI_EXPRESS_LIB_ADDRESS (
                     HgDxeInfoData->DgpuBus,
                     DGPU_DEVICE_NUM,
                     DGPU_FUNCTION_NUM,
                     PCI_COMMAND_OFFSET
                     );
      if (HgDxeInfoData->DgpuVendorId == NVIDIA_VID) {
        Data = HgDxeInfoData->NvidiaSecondaryGrcphicsCommandRegister;
      }
      PciExpressAndThenOr8 (PciAddress, Data, Data);
    }
  // if ((HgDxeInfoData->Dgpu2GpioSupport) && ((HgDxeInfoData->Dgpu2VendorId) != 0xFFFF)) {

  FreeHgDxeInfoData (&HgDxeInfoData);
  gBS->CloseEvent (Event);

  return EFI_SUCCESS;
}

/**

  The function is for free HgDxeInfoData pointer.

  @param[in, out] HgDxeInfoData   A double pointer of HG DXE information data structure.

  @retval None.

**/
VOID
EFIAPI
FreeHgDxeInfoData (
  IN OUT HG_DXE_INFORMATION_DATA              **HgDxeInfoData
  )
{
  if ((*HgDxeInfoData) != NULL) {
    FreePool (*HgDxeInfoData);
    (*HgDxeInfoData) = NULL;
  }
}

