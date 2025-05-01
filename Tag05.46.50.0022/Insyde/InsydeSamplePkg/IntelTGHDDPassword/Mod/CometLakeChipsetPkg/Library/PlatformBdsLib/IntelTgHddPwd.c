/** @file
  IntelTgHddPwd

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <IntelTgHddPwd.h>
#pragma optimize("", off)
EFI_DEVICE_PATH_PROTOCOL  mEndDevicePathNode[] = {
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};


/**
  check if the NVME is Intel Tg devices

 **/
BOOLEAN
IsIntelTg (
  EFI_PCI_IO_PROTOCOL                   *PciIo
  )
{
  PCI_TYPE00               PciConfigHeader00;
  EFI_STATUS               Status;
  
  Status = PciIo->Pci.Read (
                    PciIo,
                    EfiPciIoWidthUint32,
                    0,
                    sizeof(PciConfigHeader00) / sizeof(UINT32),
                    &PciConfigHeader00
                    );
  
  if (Status != EFI_SUCCESS) {
    return FALSE;
  }
  
  if (IsTgDevice(PciConfigHeader00.Hdr.VendorId, PciConfigHeader00.Hdr.DeviceId) == TRUE) {
    return TRUE;
  }    

  return FALSE;
}



/**
  Returns the next entry of the HDD password table.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.

  @retval           The pointer to the next table.

**/
STATIC HDD_PASSWORD_TABLE *
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
STATIC UINTN
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


VOID
DisconnectAllChildHandles (
  EFI_HANDLE  ControllerHandle
  )
{
  EFI_GUID                             **ProtocolBuffer;
  UINTN                                ProtocolBufferCount;
  UINTN                                ProtocolIndex;
  EFI_OPEN_PROTOCOL_INFORMATION_ENTRY  *OpenInfoBuffer;    
  EFI_HANDLE                          *VisitedDeviceHandles;
  UINTN                               VisitedDeviceHandlesCount;
  UINTN                               HandlesIndex;
  UINT8                                Index;
  BOOLEAN                             FoundVisitedHandle;  
  EFI_STATUS                          Status;
  UINTN                                EntryCount;  

  VisitedDeviceHandlesCount = 0;
  VisitedDeviceHandles = NULL;
  
//  for (ControllerHandleIndex = 0; ControllerHandleIndex < HandleNum; ControllerHandleIndex++) {
  Status = gBS->ProtocolsPerHandle (
                  ControllerHandle,
                  &ProtocolBuffer,
                  &ProtocolBufferCount
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
  
  
  for (ProtocolIndex = 0; ProtocolIndex < ProtocolBufferCount; ProtocolIndex++) {
    Status = gBS->OpenProtocolInformation (
                    ControllerHandle,
                    ProtocolBuffer[ProtocolIndex],
                    &OpenInfoBuffer,
                    &EntryCount
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
  
    for (Index = 0; Index < EntryCount; Index++) {
      if (!(OpenInfoBuffer[Index].Attributes & EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER)) {
        continue;
      }
  
  
      //
      // Check if the handle has already being checked SMART status
      //
      for (HandlesIndex = 0; HandlesIndex < VisitedDeviceHandlesCount; HandlesIndex++) {
        if (OpenInfoBuffer[Index].ControllerHandle == VisitedDeviceHandles[HandlesIndex]) {
          FoundVisitedHandle = TRUE;
          break;
        }
      }
  
      //
      // if found, continue with next handle
      //
      if (FoundVisitedHandle) {
        FoundVisitedHandle = FALSE;
        continue;  
      }
      
  
      VisitedDeviceHandles = ReallocatePool (
                               VisitedDeviceHandlesCount * sizeof(EFI_HANDLE),
                               (VisitedDeviceHandlesCount + 1) * sizeof(EFI_HANDLE),
                               (VOID *)VisitedDeviceHandles);
      if (VisitedDeviceHandles == NULL) {
        continue;
      }
      VisitedDeviceHandles[VisitedDeviceHandlesCount] = OpenInfoBuffer[Index].ControllerHandle;
      VisitedDeviceHandlesCount++;
      //
      // Query all the children that opened with EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 
      //
      Status =  gBS->DisconnectController (
                      OpenInfoBuffer[Index].ControllerHandle,
                      NULL, 
                      NULL
                     );
      
    }
  
    if (VisitedDeviceHandles != NULL) {
      FreePool (VisitedDeviceHandles);
    }
    
    if (OpenInfoBuffer != NULL) {
      FreePool (OpenInfoBuffer);
    }
  }

  if (ProtocolBuffer != NULL) {
    FreePool (ProtocolBuffer);
  }

}


/**
  Reconnect Rst Controller after its being unlocked

 **/
VOID
DisconnectTgController (
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_TYPE00                            Pci;
  UINTN                                 Device;
  UINTN                                 Seg;
  UINTN                                 Bus;
  UINTN                                 Function;

  UINTN                                 PciIoHandleCount;
  EFI_HANDLE                            *PciIoHandleBuffer;
  UINTN                                 PciIoHandleIndex;

  PCH_SETUP                             *PchSetup;
  UINTN                                 SetupSize;

  EFI_HANDLE                            ControllerHandle;
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
    
    if (!IsIntelTg(PciIo)) {
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

    ControllerHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &ControllerHandle,
                    &gUefiRaidOpromReadyGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    gDS->Dispatch();
    
  }

}



/**
  Build Tg NVMe device info and save them to variable

 **/
VOID
BuildTgDeviceInfoForIntelRst (
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_TYPE00                            Pci;
  EFI_HDD_PASSWORD_SERVICE_PROTOCOL     *HddPasswordService;
  HDD_PASSWORD_HDD_INFO                 *HddInfoArray;
  UINTN                                 NumOfHdd;
  UINTN                                 Index;
  EFI_HANDLE                            P2pBridgeHandle;
  EFI_HANDLE                            ControllerHandle;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *P2pBridgeDevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *TmpDevicePath;
  UINTN                                 Count;
  OPAL_DEVICE_INTEL_REMAP               *DevInfoRemap;
  NVME_NAMESPACE_DEVICE_PATH            *NvmeDevPath;
  SATA_DEVICE_PATH                      *SataDevpath;
    
  HDD_PASSWORD_TABLE                    *HddPasswordTable;
  HDD_PASSWORD_TABLE                    *HddPasswordTablePtr;
  UINTN                                 HddPasswordTableIndex;
  UINTN                                 HddPasswordTableSize; 
  UINTN                                 NumOfTable;
  UINTN                                 TempPasswordLength;
  UINTN                                 Seg;
  UINTN                                 Bus;
  UINTN                                 Device;
  UINTN                                 Function;
  EFI_DEVICE_PATH_PROTOCOL              *EndNode;
  UINTN                                 Length;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePathNode;

  HddPasswordService = NULL;
  HddInfoArray       = NULL;
  NumOfHdd           = 0;
  Count              = 0;
  DevInfoRemap       = NULL;
  NvmeDevPath        = NULL;

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

    if (!IS_PCI_NVM(&Pci) && 
        !IS_PCI_SATADPA(&Pci)) {    
//    if (!IsIntelTg(PciIo) && !IS_PCI_NVM(&Pci)) {
      continue;
    }    


    SataDevpath = NULL;
    TmpDevicePath = DevicePath;
    while (!IsDevicePathEnd (TmpDevicePath)) {
      if (TmpDevicePath->Type == MESSAGING_DEVICE_PATH && TmpDevicePath->SubType == MSG_SATA_DP) {
        SataDevpath = (SATA_DEVICE_PATH *) TmpDevicePath;
        break;
      }
      TmpDevicePath = NextDevicePathNode (TmpDevicePath);
    }    
     
    //
    // if cannot find a SATA device, it might be Intel Tg
    //
    TmpDevicePath = DevicePath;
    while (!IsDevicePathEnd (TmpDevicePath)) {
      if (TmpDevicePath->Type == MESSAGING_DEVICE_PATH && TmpDevicePath->SubType == MSG_NVME_NAMESPACE_DP) {
        NvmeDevPath = (NVME_NAMESPACE_DEVICE_PATH *) TmpDevicePath;
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
    DevInfoRemap[Count].DeviceType = OPAL_DEVICE_TYPE_TG;
    
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
         
    if (HddPasswordTableIndex == NumOfTable) {
      continue;
    }
    //
    // for Intel PeiOpalDeviceInfoPpi, need original port number from device path
    //
    if (SataDevpath == NULL) {
      DevInfoRemap[Count].Port               = 0xff;
      DevInfoRemap[Count].PortMultiplierPort = 0xff;
      DevInfoRemap[Count].NvmeNamespaceId    = NvmeDevPath->NamespaceId;
      DevInfoRemap[Count].BarAddr = (Pci.Device.Bar[0] & 0xffffc000);

      //
      // Assemble a new devicepath for P2pBridge controller 
      //
      TmpDevicePath       = DevicePath;
      DevicePathNode      = DevicePath;
      for (;!IsDevicePathEnd (DevicePathNode);
        DevicePathNode = NextDevicePathNode (DevicePathNode)) {
        if (((DevicePathType (DevicePathNode) == HARDWARE_DEVICE_PATH) &&
            (DevicePathSubType (DevicePathNode) == HW_PCI_DP))) {
          //
          // assemble to a new Pci device path
          //
          EndNode = (EFI_DEVICE_PATH*)((UINT8*)DevicePathNode + sizeof(PCI_DEVICE_PATH));
          Length = ((UINTN) EndNode - (UINTN) TmpDevicePath);
          P2pBridgeDevicePath = AllocateZeroPool(Length + sizeof(mEndDevicePathNode));
          CopyMem(P2pBridgeDevicePath, TmpDevicePath, Length);
          CopyMem(((UINT8*)P2pBridgeDevicePath + Length), mEndDevicePathNode, sizeof(mEndDevicePathNode));
          break;
        }  
      }

      //
      // find the P2pBridge handle
      //
      Status = gBS->LocateDevicePath(&gEfiPciIoProtocolGuid, &P2pBridgeDevicePath, &P2pBridgeHandle);    

      Status = gBS->HandleProtocol (
                      P2pBridgeHandle,
                      &gEfiPciIoProtocolGuid,
                      (VOID **)&PciIo
                      );

      Status = PciIo->GetLocation (
                        PciIo,
                        &Seg,
                        &Bus,
                        &Device,
                        &Function
                        );
      if (!EFI_ERROR(Status)) {
        DevInfoRemap[Count].P2pBridgeSeg      = (UINT8)Seg;
        DevInfoRemap[Count].P2pBridgeBus      = (UINT8)Bus;
        DevInfoRemap[Count].P2pBridgeDevice   = (UINT8)Device;
        DevInfoRemap[Count].P2pBridgeFunction = (UINT8)Function;
      }

    } else {
       DevInfoRemap[Count].DeviceType = OPAL_DEVICE_TYPE_ATA;

      //
      // for Intel PeiOpalDeviceInfoPpi, need original port number from device path
      //
      DevInfoRemap[Count].Port               = SataDevpath->HBAPortNumber;
      DevInfoRemap[Count].PortMultiplierPort = HddInfoArray[Index].PortMulNumber;

    }
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
