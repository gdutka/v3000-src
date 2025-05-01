/** @file

;*******************************************************************************
;* Copyright (c) 2017-2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
;
;   The AMD GOP UEFI display driver will query PlatformToDriverConfiguration()
;   with *Instance=0, and ChildHandle set to NULL when DriverBinding.Start() gets
;   called the first time to configure the graphics subsystem. Moreover, if the LCD child
;   handle is found, then our GOP driver will make another
;   PlatformToDriverConfiguration.Query() call once with *Instance=1, and
;   ChildHandle set to LCD.
;   If the protocol is supported, upon receiving ParameterBlock (refer to the following
;   definitions), the AMD GOP UEFI display driver will issue
;   EfiPlatformConfigurationActionNone in response.
;   If a platform chooses not to support this protocol for AMD GPU controllers, it should
;   return an EFI status code other than EFI_SUCCESS so that it will not jeopardize the
;   GOP UEFI display driver operation.
;   Note: When the AMD GOP UEFI display is connected to an AMD APU product,
;   Instance=1 is also retrieved even though LCD-related parameters are already passed
;   into the GOP driver through a private method. This is to provide some flexibility when
;   the platforms want to adjust these LCD-related parameters after knowing what LCD
;   is present in the system.
*/

#include <AmdGopConfigDrvDxe.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>
#include <GNbReg.h>

//#pragma optimize ("", off)

STATIC EFI_PLATFORM_TO_DRIVER_CONFIGURATION_PROTOCOL mAmdGopConfigProtocol = {
  AmdGopConfigQuery,
  AmdGopConfigResponse
};

UINT32 mDefaultDisplayDevicePath[] = {
  ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, 0), //eDP
  ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, 1), //eDP1
  ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 0), //DFP1
  ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 1), //DFP2
  ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 2), //DFP3
  ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 3), //DFP4
};

CHAR16 *
GetAdrStr (
  IN UINT32             Adr
  )
{
  switch (Adr) {
  case ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, 0):
    return L"eDP";
  case ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, 1):
    return L"eDP1";
  case ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 0):
    return L"DFP1";
  case ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 1):
    return L"DFP2";
  case ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 2):
    return L"DFP3";
  case ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, 3):
    return L"DFP4";
  }

  return L"Unknown";
}


EFI_STATUS
GetAcpiAdrFromConOutVar (
  OUT UINT32  *DisplayDeviceAdr
  );

/**
 AmdGopConfigDrvInit

 @param [in]   ImageHandle      Image handle of this driver.
 @param [in]   SystemTable      Pointer to EFI system table.

 @retval EFI_SUCCESS            The Results is filled with the requested values.
 @retval EFI_INVALID_PARAMETER  Request is NULL, illegal syntax, or unknown name.

**/
EFI_STATUS
AmdGopConfigDrvInit (
  IN EFI_HANDLE                                      ImageHandle,
  IN EFI_SYSTEM_TABLE                                *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiPlatformToDriverConfigurationProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAmdGopConfigProtocol
                  );

  return Status;
}

BOOLEAN
IsAmdGop (
  IN EFI_HANDLE                   ControllerHandle
  )
{
  EFI_STATUS                       Status;
  EFI_PCI_IO_PROTOCOL              *PciIo;
  PCI_TYPE00                       Pci;

  DEBUG ((EFI_D_INFO, "IsAmdGop start\n"));

  Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
  if (!EFI_ERROR(Status)) {
    Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0,
                            sizeof (Pci) / sizeof (UINT32),
                            &Pci
                            );
    if (!EFI_ERROR(Status)) {
      DEBUG ((EFI_D_INFO, "VID: 0x%x DID: 0x%x!\n", Pci.Hdr.VendorId, Pci.Hdr.DeviceId));
      if ((Pci.Hdr.VendorId == INTERNAL_VGA_VID) && ((Pci.Hdr.DeviceId == DEFAULT_IGP_DID) || (Pci.Hdr.DeviceId == INTERNAL_VGA_DID18) || (Pci.Hdr.DeviceId == INTERNAL_VGA_DID17))) {
        DEBUG ((EFI_D_INFO, "Not CZN/RN/LCN iGPU, Should not run here!\n"));
        return TRUE;
      }
    }
  }

  DEBUG ((EFI_D_INFO, "IsAmdGop end\n"));

  return FALSE;
}

EFI_STATUS
InitDisplayDeviceAdrByCpm (
  OUT  UINT32  **CpmDisplayDevicePathArray,
  OUT  UINTN   *CpmArrayCount
  )
{
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_DXIO_TOPOLOGY_TABLE     *DxioTopologyTablePtr;
  UINTN                           Index;
  UINTN                           DeviceAdrIndex;
  UINTN                           DdiCount;
  UINTN                           NumOfDdiInstance;
  UINTN                           DisplayTypeCount[5] = {0};

  *CpmDisplayDevicePathArray = NULL;
  *CpmArrayCount = 0;
  NumOfDdiInstance = 0;

  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID**)&CpmTableProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DxioTopologyTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DXIO_TOPOLOGY);

  if (DxioTopologyTablePtr == NULL) {
    return EFI_UNSUPPORTED;
  }

  DdiCount = 0;
  for (Index = 0; ; Index++) {
    NumOfDdiInstance++;
    if (DxioTopologyTablePtr->Ddi[Index].Flags != DESCRIPTOR_TERMINATE_LIST) {
      if (DxioTopologyTablePtr->Ddi[Index].Ddi.ConnectorType != UnusedType) {
        DdiCount++;
      }
    } else {
      if (DxioTopologyTablePtr->Ddi[Index].Ddi.ConnectorType != UnusedType) {
        DdiCount++;
      }
      break;
    }
  }

  if (DdiCount > AMDGOP_PRIORITY_COUNT) {
    DdiCount = AMDGOP_PRIORITY_COUNT;
  }

  *CpmDisplayDevicePathArray = AllocateZeroPool (AMDGOP_PRIORITY_COUNT * sizeof (UINT32));
  if (*CpmDisplayDevicePathArray == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DeviceAdrIndex = 0;
  Index = 0;

  for (Index = 0; Index < NumOfDdiInstance; Index++) {

    switch (DxioTopologyTablePtr->Ddi[Index].Ddi.ConnectorType) {
    case ConnectorTypeDpToVga:
    case ConnectorTypeNutmegDpToVga:
      (*CpmDisplayDevicePathArray)[DeviceAdrIndex] = ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_VGA, 0, DisplayTypeCount[ACPI_ADR_DISPLAY_TYPE_VGA]);
      DisplayTypeCount[ACPI_ADR_DISPLAY_TYPE_VGA]++;
      DeviceAdrIndex++;
      break;

    case ConnectorTypeEDP:
    case ConnectorTypeEDPToLvds:

      (*CpmDisplayDevicePathArray)[DeviceAdrIndex] = ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, DisplayTypeCount[ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL]);
      DisplayTypeCount[ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL]++;
      DeviceAdrIndex++;
      break;

    case ConnectorTypeDP:
    case ConnectorTypeHDMI:
    case ConnectorTypeDpWithTypeC:
    case ConnectorTypeDpWithoutTypeC:
      (*CpmDisplayDevicePathArray)[DeviceAdrIndex] = ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL, 0, DisplayTypeCount[ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL]);
      DisplayTypeCount[ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL]++;
      DeviceAdrIndex++;
      break;
    }

  }

  *CpmArrayCount = DdiCount;

  return EFI_SUCCESS;
}

EFI_STATUS
AmdGopConfigQuery (
  IN CONST  EFI_PLATFORM_TO_DRIVER_CONFIGURATION_PROTOCOL *This,
  IN CONST  EFI_HANDLE                                    ControllerHandle,
  IN CONST  EFI_HANDLE                                    ChildHandle OPTIONAL,
  IN CONST  UINTN                                         *Instance,
  OUT       EFI_GUID                                      **ParameterTypeGuid,
  OUT       VOID                                          **ParameterBlock,
  OUT       UINTN                                         *ParameterBlockSize
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  GetingStatus;
  UINT32      DisplayDeviceAdr;
  UINT32      TmpDisplayDeviceAdr;
  UINTN       MaxDisplayDevicePath;
  UINTN       Index;
  UINTN       Index2;
  UINT32      DisplayDevicePathArray[6] = {0, 0, 0, 0, 0, 0};
  UINT32      *CpmDisplayDevicePathArray;
  UINTN       CpmArrayCount;

  PLATFORM_TO_AMDGOP_CONFIGURATION_COMMON_1  *AmdGopConfigCommonn;
  EFI_PCI_IO_PROTOCOL                        *PciIo;
  PCI_TYPE00                                 Pci;

  DEBUG ((EFI_D_INFO, "AmdGopConfigQuery Enter..\n"));

  Status = EFI_NOT_FOUND;
  *ParameterTypeGuid = NULL;
  *ParameterBlockSize = 0;

  if ((ControllerHandle == NULL) || (Instance == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsAmdGop (ControllerHandle)) {
    return Status;
  }

  Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
  if (!EFI_ERROR(Status)) {
    Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0,
                            sizeof (Pci) / sizeof (UINT32),
                            &Pci
                            );
    if (!EFI_ERROR(Status)) {
      DEBUG ((EFI_D_INFO, "VID: 0x%x DID: 0x%x!\n", Pci.Hdr.VendorId, Pci.Hdr.DeviceId));
      if (!((Pci.Hdr.VendorId == INTERNAL_VGA_VID) && (Pci.Hdr.DeviceId == INTERNAL_VGA_DID17 || 
	                                                Pci.Hdr.DeviceId == INTERNAL_VGA_DID18))) {
        DEBUG ((EFI_D_INFO, "Not iGPU, AmdGopConfigQuery return EFI_UNSUPPORTED!\n"));
        return EFI_UNSUPPORTED;
      }
    }
  }

  switch (*Instance) {
  case 0:
    Status = gBS->AllocatePool (
                    EfiACPIReclaimMemory,
                    sizeof (PLATFORM_TO_AMDGOP_CONFIGURATION_COMMON_1),
                    ParameterBlock
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    AmdGopConfigCommonn = *ParameterBlock;
    ZeroMem (AmdGopConfigCommonn, sizeof (PLATFORM_TO_AMDGOP_CONFIGURATION_COMMON_1));

    //
    // Get Display device path from "ConOut", this device path has highest priority
    //
    GetingStatus = GetAcpiAdrFromConOutVar (&DisplayDeviceAdr);

    //
    // Create Display Device Path by CPM
    //
    Status = InitDisplayDeviceAdrByCpm (&CpmDisplayDevicePathArray, &CpmArrayCount);

    //
    // If Device Path init by CPM failed, use default table.
    //
    if (EFI_ERROR (Status)) {
      MaxDisplayDevicePath = sizeof (mDefaultDisplayDevicePath) / sizeof (UINT32);
      CopyMem(&DisplayDevicePathArray[0], &mDefaultDisplayDevicePath[0], MaxDisplayDevicePath);
    } else {

      Index2 = 0;
      //
      //  prioritize edp, edp1 to first
      //
      for (Index = 0; Index < CpmArrayCount; Index++) {
        if (((CpmDisplayDevicePathArray[Index] & 0xF00) != 0x400)) {
          continue;
        }
        DisplayDevicePathArray[Index2++] = CpmDisplayDevicePathArray[Index];
      }

      for (Index = 0; Index < CpmArrayCount; Index++) {
        if (((CpmDisplayDevicePathArray[Index] & 0xF00) == 0x400)) {
          continue;
        }
        DisplayDevicePathArray[Index2++] = CpmDisplayDevicePathArray[Index];
      }

      MaxDisplayDevicePath = CpmArrayCount;

      if (!EFI_ERROR (GetingStatus)) {
        for (Index = 0; Index < CpmArrayCount; Index++) {
          if (DisplayDeviceAdr == CpmDisplayDevicePathArray[Index]) {
            break;
          }
        }

        //
        // If Display DevicePath doesn't exist in CPM table, use default table.
        //
        if (Index == CpmArrayCount) {
          MaxDisplayDevicePath = sizeof (mDefaultDisplayDevicePath) / sizeof (UINT32);
          CopyMem(&DisplayDevicePathArray[0], &mDefaultDisplayDevicePath[0], MaxDisplayDevicePath);
        }
      }
    }

    //
    // If GetAcpiAdrFromConOutVar failed, don't need to sort DisplayDevicePathArray
    //
    if (!EFI_ERROR (GetingStatus)) {
      //
      // Put the "ConOut" device path on the top of DisplayDevicePathArray.
      //
      for (Index = 0; Index < MaxDisplayDevicePath; Index++) {
        if (DisplayDeviceAdr == DisplayDevicePathArray[Index]) {
          TmpDisplayDeviceAdr = DisplayDevicePathArray[0];
          DisplayDevicePathArray[0] = DisplayDeviceAdr;
          DisplayDevicePathArray[Index] = TmpDisplayDeviceAdr;
          break;
        }
      }
    }

    for (Index = 0; Index < 5; Index++) {
      DEBUG ((
        EFI_D_INFO,
        "  DisplayDevicePathArray[%d] = 0x%x (%s)\n",
        Index,
        DisplayDevicePathArray[Index],
        GetAdrStr(DisplayDevicePathArray[Index])
        ));
    }

    AmdGopConfigCommonn->Priority1ActiveDisplay = DisplayDevicePathArray[0];
    AmdGopConfigCommonn->Priority2ActiveDisplay = DisplayDevicePathArray[1];
    AmdGopConfigCommonn->Priority3ActiveDisplay = DisplayDevicePathArray[2];
    AmdGopConfigCommonn->Priority4ActiveDisplay = DisplayDevicePathArray[3];
    AmdGopConfigCommonn->Priority5ActiveDisplay = DisplayDevicePathArray[4];
    AmdGopConfigCommonn->Priority6ActiveDisplay = DisplayDevicePathArray[5];

    if (CpmDisplayDevicePathArray != NULL) {
      gBS->FreePool (CpmDisplayDevicePathArray);
    }

    //
    // Sample
    //
    //((PLATFORM_TO_AMDGOP_CONFIGURATION_0 *)(*ParameterBlock))->Priority1ActiveDisplay = 0x80010100; //CRT1
    //
    //if (!EFI_ERROR (Status)) {
    //  ((PLATFORM_TO_AMDGOP_CONFIGURATION_0 *)(*ParameterBlock))->Priority1ActiveDisplay = DisplayDeviceAdr;
    //  if (DisplayDeviceAdr != ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_VGA, 0, 0)) {
    //    ((PLATFORM_TO_AMDGOP_CONFIGURATION_0 *)(*ParameterBlock))->Priority2ActiveDisplay = ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_VGA, 0, 0);
    //  }
    //
    //  if (DisplayDeviceAdr != ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, 0)) {
    //    ((PLATFORM_TO_AMDGOP_CONFIGURATION_0 *)(*ParameterBlock))->Priority2ActiveDisplay = ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 0, 0);
    //  }
    //}

    *ParameterBlockSize = sizeof (PLATFORM_TO_AMDGOP_CONFIGURATION_COMMON_1);
    Status = EFI_SUCCESS;
    break;
  case 1:
    //Status = gBS->AllocatePool (
    //              EfiACPIReclaimMemory,
    //              sizeof (PLATFORM_TO_AMDGOP_CONFIGURATION_1),
    //              ParameterBlock);
    //if (EFI_ERROR (Status)) {
    //  return Status;
    //}
    //EfiZeroMem (*ParameterBlock, sizeof (PLATFORM_TO_AMDGOP_CONFIGURATION_1));
    //
    // Sample
    //
    //((PLATFORM_TO_AMDGOP_CONFIGURATION_1 *)(*ParameterBlock))->LCD_BootUp_BL_Level = 128;

    //*ParameterBlockSize = sizeof (PLATFORM_TO_AMDGOP_CONFIGURATION_1);
    *ParameterBlock = NULL;
    *ParameterBlockSize = 0;
    Status = EFI_NOT_FOUND;
    break;
  default:
    DEBUG ((EFI_D_INFO, "AmdGopConfigQuery exit!\n"));
    return EFI_NOT_FOUND;
  }

  *ParameterTypeGuid = &gEfiPlatformtoAmdgopConfigurationGuid;

  DEBUG ((EFI_D_INFO, "AmdGopConfigQuery end\n"));

  return Status;
}

EFI_STATUS
AmdGopConfigResponse (
  IN CONST  EFI_PLATFORM_TO_DRIVER_CONFIGURATION_PROTOCOL *This,
  IN CONST  EFI_HANDLE                                    ControllerHandle,
  IN CONST  EFI_HANDLE                                    ChildHandle OPTIONAL,
  IN CONST  UINTN                                         *Instance,
  IN CONST  EFI_GUID                                      *ParameterTypeGuid,
  IN CONST  VOID                                          *ParameterBlock,
  IN CONST  UINTN                                         ParameterBlockSize ,
  IN CONST  EFI_PLATFORM_CONFIGURATION_ACTION             ConfigurationAction
  )
{
  EFI_STATUS  Status;

  Status = EFI_NOT_FOUND;

  DEBUG ((EFI_D_INFO, "AmdGopConfigResponse start\n"));

  if ((ControllerHandle == NULL) || (Instance == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsAmdGop (ControllerHandle)) {
    DEBUG ((EFI_D_INFO, "It is not iGPU use gEfiPlatformToDriverConfigurationProtocolGuid then return\n"));
    return EFI_NOT_FOUND;
  }

  if (ConfigurationAction == EfiPlatformConfigurationActionNone) {
    if (ParameterBlock) {
      gBS->FreePool ((VOID *)ParameterBlock);
    }
    return EFI_SUCCESS;
  }

  DEBUG ((EFI_D_INFO, "AmdGopConfigResponse end\n"));

  return Status;
}

EFI_STATUS
GetAcpiAdrFromConOutVar (
  OUT UINT32  *DisplayDeviceAdr
  )
{
  EFI_STATUS                        Status;
  EFI_DEVICE_PATH_PROTOCOL          *ConOutVar;
  EFI_DEVICE_PATH_PROTOCOL          *TmpConOutVar;
  ACPI_ADR_DEVICE_PATH              *LocalDisplayDeviceAdr;
  UINTN                             ConOutSize;

  Status = EFI_NOT_FOUND;
  ConOutVar = NULL;
  ConOutSize = 0;
  //
  // Get last run Acpi_ADR Value from ConOut.
  //
  ConOutVar = BdsLibGetVariableAndSize (
                L"UserVgaSelection", 
                &gEfiGenericVariableGuid,
                &ConOutSize
                );

  if (ConOutVar) {
    TmpConOutVar = ConOutVar;
    while (!IsDevicePathEnd (TmpConOutVar)) {
      if ((DevicePathType (TmpConOutVar) == ACPI_DEVICE_PATH) && (DevicePathSubType (TmpConOutVar) == ACPI_ADR_DP)) {
        if (DevicePathNodeLength (TmpConOutVar) == sizeof (ACPI_ADR_DEVICE_PATH)) {
          LocalDisplayDeviceAdr = (ACPI_ADR_DEVICE_PATH *)(TmpConOutVar);
          *DisplayDeviceAdr = LocalDisplayDeviceAdr->ADR;
          Status = EFI_SUCCESS;
        }
      }
      TmpConOutVar = NextDevicePathNode (TmpConOutVar);
    }

    gBS->FreePool (ConOutVar);
  }

  return Status;
}
