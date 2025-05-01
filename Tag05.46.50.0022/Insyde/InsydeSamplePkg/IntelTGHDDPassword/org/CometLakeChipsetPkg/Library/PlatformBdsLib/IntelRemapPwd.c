/** @file
  IntelRemapPwd

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

#include <IntelRemapPwd.h>

/**
  Check if device is a re-mapped NVMe by device path.

  @param[in] DevicePath          Device path info for the device.

  @retval True                   It is a Intel remapped device
  @retval FALSE                  It is not a Intel remapped device
**/
BOOLEAN
IsRemappedNvme (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL              *TmpDevicePath;  
  EFI_HANDLE                            DiskInfoHandle;
  EFI_STATUS                            Status;
  EFI_DISK_INFO_PROTOCOL                *DiskInfo;
  
  DiskInfoHandle = NULL;
  TmpDevicePath = DevicePath;
  Status = gBS->LocateDevicePath(&gEfiDiskInfoProtocolGuid, &TmpDevicePath, &DiskInfoHandle);
  if (EFI_ERROR(Status)) {
    return FALSE;
  }
  Status = gBS->HandleProtocol (
                  DiskInfoHandle,
                  &gEfiDiskInfoProtocolGuid,
                  (VOID **)&DiskInfo
                  );
  
  if (CompareGuid (&DiskInfo->Interface, &gEfiDiskInfoNvmeInterfaceGuid)) {
    return TRUE;
  }

  return FALSE;
}


/**
 Find all agent handles that support HDD Password feature.
 @param[in]        AgentFileGuidList         Pointer to AGENT_FILE_GUID_LIST.
 @param[in]        size                      size of the AGENT_FILE_GUID_LIST.

 @retval *EFI_HANDLE  A list of agent handles that discovered from file Guids
**/

EFI_HANDLE*
FindAgentHandleListByFileGUID (
AGENT_FILE_GUID_LIST*   AgentFileGuidList,
UINTN                   Size
  )
{
  EFI_HANDLE                            *FoundAgentHandleList;
  UINTN                                 HandleCount;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 HandleIndex;
  EFI_DRIVER_BINDING_PROTOCOL           *DriverBinding;
  EFI_GUID                              *GuidPoint;
  EFI_DEVICE_PATH_PROTOCOL              *TempDevicePath;
  EFI_LOADED_IMAGE_PROTOCOL             *LoadedImage;
  UINT8                                 Index;
  UINT8                                 Index1;
  EFI_STATUS                            Status;

  Index1 = 0;

  //
  // Add one more entry to the handle list because according to UEFI spec 2.6
  // "The list is terminated by a NULL handle value."
  //
  FoundAgentHandleList = AllocateZeroPool((Size + 1) * sizeof(EFI_HANDLE));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadedImageProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  //
  // find the right driverbinding handle for HDD Password 
  //
  if (Status == EFI_SUCCESS && HandleBuffer != NULL) {
    for (Index = 0; Index < Size; Index++) {  
      for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
        Status = gBS->HandleProtocol (
                        HandleBuffer[HandleIndex],
                        &gEfiDriverBindingProtocolGuid,
                        (VOID**) &DriverBinding
                        );

        if (EFI_ERROR(Status)) {
          continue;
        }
        
        Status = gBS->HandleProtocol (
                  DriverBinding->ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **) &LoadedImage
                  );

        if (EFI_ERROR(Status)) {
          continue;
        }
        
        TempDevicePath = LoadedImage->FilePath;
        GuidPoint = EfiGetNameGuidFromFwVolDevicePathNode (
                      (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) TempDevicePath
                      );
        if (GuidPoint == NULL) {
          DEBUG ((EFI_D_INFO, " Empty GUID found\n"));
          continue;
        }
        
        if (CompareGuid(&(AgentFileGuidList[Index].FileGuid), GuidPoint)) {
          //
          // store found Agent handles to FoundAgentHandleList in the same order as 
          // AgentFileGuidList defined.  
          //
          FoundAgentHandleList[Index1] = DriverBinding->ImageHandle;          
          Index1++; 
        }
      }
    }
  }

  return FoundAgentHandleList;
}



/**
  Reconnect Rst Controller after its being unlocked

 **/
VOID
ReConnectRstController (
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_TYPE00                            Pci;
  EFI_HDD_PASSWORD_SERVICE_PROTOCOL     *HddPasswordService;
  HDD_PASSWORD_HDD_INFO                 *HddInfoArray;
  UINTN                                 NumOfHdd;
  UINTN                                 Index;
  BOOLEAN                               FoundSecurityEnabledRaid;
  UINTN                                 Device;
  UINTN                                 Seg;
  UINTN                                 Bus;
  UINTN                                 Function;

  UINTN                                 PciIoHandleCount;
  EFI_HANDLE                            *PciIoHandleBuffer;
  UINTN                                 PciIoHandleIndex;

  EFI_HANDLE                            *AgentHandleBuffer; 
  AGENT_FILE_GUID_LIST                  AgentFileGuidList[] = {
    {0x13863F79, 0xD94B, 0x4205, 0xBB, 0x0F, 0xE4, 0xE0, 0x6A, 0xAA, 0x5A, 0x4E}, // DelayUefiRaid    
    {0x3ACC966D, 0x8E33, 0x45c6, 0xb4, 0xfe, 0x62, 0x72, 0x4B, 0xCD, 0x15, 0xA9}, // AhciBusDxe
    {0x5BE3BDF4, 0x53CF, 0x46a3, 0xA6, 0xA9, 0x73, 0xC3, 0x4A, 0x6E, 0x5E, 0xE3}, // NVME
    {0x67BBC344, 0x84BC, 0x4e5c, 0xb4, 0xDF, 0xF5, 0xE4, 0xA0, 0x0E, 0x1F, 0x3A}, // SD
    {0x70D57D67, 0x7F05, 0x494d, 0xA0, 0x14, 0xB7, 0x5D, 0x73, 0x45, 0xB7, 0x00}  // SSCP
  };

  PCH_SETUP                             *PchSetup;
  UINTN                                 SetupSize;


  HddPasswordService = NULL;
  HddInfoArray       = NULL;
  NumOfHdd           = 0;
  FoundSecurityEnabledRaid = FALSE;
  PchSetup           = NULL;
    
  SetupSize = sizeof (PCH_SETUP);
  PchSetup = AllocateZeroPool (SetupSize);
  if (PchSetup == NULL) {
    return;
  }
  
  Status = gRT->GetVariable (
                PCH_SETUP_VARIABLE_NAME,
                &gPchSetupVariableGuid,
                NULL,
                &SetupSize,
                PchSetup
                );
  if (EFI_ERROR(Status)) {
    FreePool(PchSetup);
    return;
  }
  
  if(PchSetup->SataLegacyOrom) {
    return;
  }
    
  Status = gBS->LocateProtocol (
                  &gEfiHddPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **)&HddPasswordService
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  Status = HddPasswordService->GetHddInfo (
                                     HddPasswordService,
                                     &HddInfoArray,
                                     &NumOfHdd
                                     );
  if (NumOfHdd == 0) {
    return;
  }  


  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (Status != EFI_SUCCESS) {
    return;
  }

  if (PciIoHandleCount == 0) {
    return;
  }


  for (PciIoHandleIndex = 0; PciIoHandleIndex < PciIoHandleCount; PciIoHandleIndex++) {
    Status = gBS->HandleProtocol (
                    PciIoHandleBuffer[PciIoHandleIndex],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (Status != EFI_SUCCESS) {
      continue;
    }


    Status = PciIo->GetLocation (
                      PciIo,
                      &Seg,
                      &Bus,
                      &Device,
                      &Function
                      );
    if (Status != EFI_SUCCESS) {
      return;
    }


    Status = PciIo->Pci.Read (
                     PciIo,
                     EfiPciIoWidthUint32,
                     0,
                     sizeof (Pci) / sizeof (UINT32),
                     &Pci
                     );
    
    if (Status != EFI_SUCCESS) {
      continue;
    }
    
    if (!IS_PCI_RAID (&Pci)) {
      continue;
    }    
    
    for (Index = 0; Index < NumOfHdd; Index++) {
      if ((HddInfoArray[Index].PciSeg == Seg) &&
          (HddInfoArray[Index].PciBus == Bus) &&
          (HddInfoArray[Index].PciDevice == Device) &&
          (HddInfoArray[Index].PciFunction == Function)) {
        if ((HddInfoArray[Index].HddSecurityStatus & HDD_SECURITY_ENABLE) &&
           !(HddInfoArray[Index].HddSecurityStatus & HDD_SECURITY_LOCK)) {
          FoundSecurityEnabledRaid = TRUE;
          break;
        }
      }
    }
    
    if (!FoundSecurityEnabledRaid) {
      continue;
    }
    
    //
    // Reconnect the Raid handle
    //
    Status =  gBS->DisconnectController (
                    PciIoHandleBuffer[PciIoHandleIndex],
                    NULL, 
                    NULL
                   );
    ASSERT_EFI_ERROR(Status);
    
    AgentHandleBuffer = FindAgentHandleListByFileGUID(AgentFileGuidList, sizeof(AgentFileGuidList)/sizeof(AGENT_FILE_GUID_LIST));
    Status = gBS->ConnectController (
                    PciIoHandleBuffer[PciIoHandleIndex],
                    AgentHandleBuffer, 
                    NULL,
                    TRUE
                     );
    ASSERT_EFI_ERROR(Status);
    
  }

}


/**
  Returns the next entry of the HDD password table.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.

  @retval           The pointer to the next table.

**/
HDD_PASSWORD_TABLE *
GetNextTableEntry (
  IN  HDD_PASSWORD_TABLE                *HddPasswordTablePtr
  )
{
  return (HDD_PASSWORD_TABLE *)((UINTN)(HddPasswordTablePtr + 1) + HddPasswordTablePtr->ExtDataSize);
}



/**
  Calculate the number of Hdd password tables.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.
  @param[in]        HddPasswordTableSize     HddPasswordTable size.

  @retval The number of tables.

**/
UINTN
NumOfHddPasswordTable (
  IN  HDD_PASSWORD_TABLE                  *HddPasswordTable,
  IN  UINTN                               HddPasswordTableSize
  )
{
  UINTN                               NumOfTable;
  HDD_PASSWORD_TABLE                  *HddPasswordTablePtr;

  if (HddPasswordTable == NULL || HddPasswordTableSize == 0) {
    return 0;
  }

  NumOfTable = 0;
  HddPasswordTablePtr = (HDD_PASSWORD_TABLE *)HddPasswordTable;
  while (((UINTN)HddPasswordTable + HddPasswordTableSize) > (UINTN)HddPasswordTablePtr) {
    HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr);
    NumOfTable++;
  }

  return NumOfTable;
}



/**
  Build Re-mapped NVMe OPAL device info and save them to LockBox.

 **/
VOID
BuildOpalDeviceInfoForIntelRst (
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_TYPE00                            Pci;
  EFI_HDD_PASSWORD_SERVICE_PROTOCOL     *HddPasswordService;
  HDD_PASSWORD_HDD_INFO                 *HddInfoArray;
  UINTN                                 NumOfHdd;
  UINTN                                 Index;
  EFI_HANDLE                            ControllerHandle;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *TmpDevicePath;
  UINTN                                 Count;
  OPAL_DEVICE_INTEL_REMAP               *DevInfoRemap;
  SATA_DEVICE_PATH                      *SataDevPath;

  HDD_PASSWORD_TABLE                    *HddPasswordTable;
  HDD_PASSWORD_TABLE                    *HddPasswordTablePtr;
  UINTN                                 HddPasswordTableIndex;
  UINTN                                 HddPasswordTableSize; 
  UINTN                                 NumOfTable;
  UINTN                                 TempPasswordLength;

  HddPasswordService = NULL;
  HddInfoArray       = NULL;
  NumOfHdd           = 0;
  Count              = 0;
  DevInfoRemap       = NULL;
  SataDevPath        = NULL;

  Status = CommonGetVariableDataAndSize (
             SAVE_HDD_PASSWORD_VARIABLE_NAME,
             &gSaveHddPasswordGuid,
             &HddPasswordTableSize,
             (VOID **) &HddPasswordTable
             );
  if (EFI_ERROR (Status)) {
    return;
  }

    
  Status = gBS->LocateProtocol (
                  &gEfiHddPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **)&HddPasswordService
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  Status = HddPasswordService->GetHddInfo (
                                     HddPasswordService,
                                     &HddInfoArray,
                                     &NumOfHdd
                                     );
  if (NumOfHdd == 0) {
    return;
  }  


  //
  // For those devices still lock at this point, no need to store its information to 
  // SAVE_HDD_PASSWORD_FOR_REMAP_DEVICE_NAME variable because no need to autounlock it in S3
  //
  for (Index = 0; Index < NumOfHdd; Index++) {
    if ((HddInfoArray[Index].HddSecurityStatus & HDD_ENABLE_BIT) != HDD_ENABLE_BIT) {
      continue;
    }

    if ((HddInfoArray[Index].HddSecurityStatus & HDD_LOCKED_BIT) == HDD_LOCKED_BIT) {
      continue;
    }

    Status = gBS->HandleProtocol (
                    HddInfoArray[Index].DeviceHandleInDxe,
                    &gEfiDevicePathProtocolGuid,
                    (VOID *) &DevicePath
                    );
    if (Status != EFI_SUCCESS) {
      continue;
    }
  
    ControllerHandle = NULL;
    TmpDevicePath    = DevicePath;
    Status = gBS->LocateDevicePath(&gEfiPciIoProtocolGuid, &TmpDevicePath, &ControllerHandle);
    if (Status != EFI_SUCCESS || ControllerHandle == NULL) {
      continue;
    }
  
    Status = gBS->HandleProtocol (
                    ControllerHandle,
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
  
    Status = PciIo->Pci.Read (
                     PciIo,
                     EfiPciIoWidthUint32,
                     0,
                     sizeof (Pci) / sizeof (UINT32),
                     &Pci
                     );
    
    if (Status != EFI_SUCCESS) {
      continue;
    }
    
    if (!IS_PCI_RAID (&Pci)) {
      continue;
    }    
     
    //
    // Find the SATA device path
    //
    TmpDevicePath = DevicePath;
    while (!IsDevicePathEnd (TmpDevicePath)) {
      if (TmpDevicePath->Type == MESSAGING_DEVICE_PATH && TmpDevicePath->SubType == MSG_SATA_DP) {
        SataDevPath = (SATA_DEVICE_PATH *) TmpDevicePath;
        break;
      }
      TmpDevicePath = NextDevicePathNode (TmpDevicePath);
    }
    //
    // get new struncture
    //
    DevInfoRemap = (OPAL_DEVICE_INTEL_REMAP*)ReallocatePool (
                                                Count * sizeof(OPAL_DEVICE_INTEL_REMAP),
                                                (Count + 1) * sizeof(OPAL_DEVICE_INTEL_REMAP),
                                                (VOID *)DevInfoRemap);

    DevInfoRemap[Count].Segment  =  (UINT16)HddInfoArray[Index].PciSeg;
    DevInfoRemap[Count].Bus      =  (UINT8)HddInfoArray[Index].PciBus;
    DevInfoRemap[Count].Device   =  (UINT8)HddInfoArray[Index].PciDevice;
    DevInfoRemap[Count].Function =  (UINT8)HddInfoArray[Index].PciFunction;
    DevInfoRemap[Count].ControllerNumberForHddPasswordTable = HddInfoArray[Index].ControllerNumber;

    //
    // get original devicepath
    //
    TmpDevicePath = DevicePath;
    if (IsRemappedNvme(TmpDevicePath)) {
      DevInfoRemap[Count].DeviceType = OPAL_DEVICE_TYPE_REMAP_NVME;
    } else {
       DevInfoRemap[Count].DeviceType = OPAL_DEVICE_TYPE_ATA;
    }

    NumOfTable = NumOfHddPasswordTable (HddPasswordTable ,HddPasswordTableSize);
    HddPasswordTablePtr = (HDD_PASSWORD_TABLE *)HddPasswordTable;
    for (HddPasswordTableIndex = 0; HddPasswordTableIndex < NumOfTable;
         HddPasswordTableIndex++, HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr)) {
      if ((HddPasswordTablePtr->ControllerNumber == HddInfoArray[Index].ControllerNumber) &&   
          (HddPasswordTablePtr->PortNumber == HddInfoArray[Index].PortNumber) &&
          (HddPasswordTablePtr->PortMulNumber == HddInfoArray[Index].PortMulNumber)) {
        HddPasswordService->PasswordStringProcess (
          HddPasswordService,
          USER_PSW,
          HddPasswordTablePtr->PasswordStr,
          StrLen (HddPasswordTablePtr->PasswordStr),
          (VOID **)&DevInfoRemap[Count].Password,
          &TempPasswordLength
          );
        DevInfoRemap[Count].PasswordLength = (UINT8)TempPasswordLength;
        break;
      }
    }
    //
    // for Intel PeiOpalDeviceInfoPpi, need original port number from device path
    //
    DevInfoRemap[Count].Port               = SataDevPath->HBAPortNumber;
    DevInfoRemap[Count].PortMultiplierPort = HddInfoArray[Index].PortMulNumber;
    Count++;           
  }


  Status = CommonSetVariable (
             SAVE_HDD_PASSWORD_FOR_REMAP_DEVICE_NAME,
             &gSaveIntelRemapDevInfoGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             Count * sizeof(OPAL_DEVICE_INTEL_REMAP),
             (VOID *) DevInfoRemap
             );
  ASSERT_EFI_ERROR(Status);  

  
  if (HddInfoArray !=  NULL) { 
    gBS->FreePool (HddInfoArray);
  }

  return;
}



/**
  Collect remap and Raid drive information for s3 auto unlock in Pei phase

  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBdsCpEnumerteAfterCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  H2OCpUnregisterHandler (Handle);
  BuildOpalDeviceInfoForIntelRst();
}


/**
  To check if Intel Raid devices have being security enabled, if Yes, reconnect the controller.
  This is due to Intel Rst Driver can't produce BlockIo, after unlock a device, until re-enumerate device again for security reason.

  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBdsCpEnumerteBeforeCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  H2OCpUnregisterHandler (Handle);
  ReConnectRstController();
}

/**
  To support HddPassword for Intel Remapped device, need to dispatch Intel RST driver
  before reaching H2OBdsCpConOutAfter checkpoint.  This is where HddPassword driver
  collect all supported devices

 **/
VOID
DispatchIntelRstBeforeConOutAfter (
  )
{
  UINT32                             Index;
  EFI_HANDLE                         ControllerHandle;
  PCH_SETUP                          *PchSetup;
  UINTN                              SetupSize;
  EFI_STATUS                         Status;
  BOOLEAN                            FoundRemappedDevice;

  FoundRemappedDevice = FALSE; 
  PchSetup            = NULL;
  SetupSize = sizeof (PCH_SETUP);
  PchSetup = AllocateZeroPool (SetupSize);
  if (PchSetup == NULL) {
    return;
  }
  
  Status = gRT->GetVariable (
                PCH_SETUP_VARIABLE_NAME,
                &gPchSetupVariableGuid,
                NULL,
                &SetupSize,
                PchSetup
                );
  if (EFI_ERROR(Status)) {
    FreePool(PchSetup);
    return;
  }

  if (PchSetup->SataInterfaceMode != SATA_MODE_RAID) {
    FreePool(PchSetup);
    return;
  }

  ControllerHandle = NULL;

  //
  // only need to dispatch Rst driver earlier when remapped device password feature is supported
  //
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    if (PchSetup->RstPcieRemapEnabled[Index]) {
      Status = gBS->InstallProtocolInterface (
                      &ControllerHandle,
                      &gUefiRaidOpromReadyGuid,
                      EFI_NATIVE_INTERFACE,
                      NULL
                      );
      gDS->Dispatch();
      FoundRemappedDevice = TRUE;
      break;
    }
  }

  
  if (!PchSetup->SataLegacyOrom && FoundRemappedDevice) {
    if (FeaturePcdGet (PcdH2OBdsCpBootDeviceEnumBeforeSupported)) {
      //
      // Register for callback on Storage Security Protocol publication
      //
      EFI_HANDLE H2OBdsCpBootDeviceEnumBeforeHandle = NULL;
      Status = H2OCpRegisterHandler (
                 &gH2OBdsCpBootDeviceEnumBeforeGuid,
                 H2OBdsCpEnumerteBeforeCallback,
                 HDD_PASSWORD_CONNECT_CONTROLLER_NOTIFY_TPL,
                 &H2OBdsCpBootDeviceEnumBeforeHandle
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpBootDeviceEnumBeforeGuid, Status));
      }
      DEBUG ((EFI_D_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpBootDeviceEnumBeforeGuid, Status));
    }    
  
    if (FeaturePcdGet (PcdH2OBdsCpBootDeviceEnumAfterSupported)) {
      //
      // Register for callback on Storage Security Protocol publication
      //
      EFI_HANDLE H2OBdsCpBootDeviceEnumAfterHandle = NULL;
      Status = H2OCpRegisterHandler (
                 &gH2OBdsCpBootDeviceEnumAfterGuid,
                 H2OBdsCpEnumerteAfterCallback,
                 HDD_PASSWORD_CONNECT_CONTROLLER_NOTIFY_TPL,
                 &H2OBdsCpBootDeviceEnumAfterHandle
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpBootDeviceEnumAfterGuid, Status));
      }
      DEBUG ((EFI_D_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpBootDeviceEnumAfterGuid, Status));
    }    
  
  }

}

