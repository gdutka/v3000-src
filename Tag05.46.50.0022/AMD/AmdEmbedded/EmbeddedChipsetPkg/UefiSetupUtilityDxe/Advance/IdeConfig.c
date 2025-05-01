/** @file

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "SetupUtility.h"
#include <Library/H2OLib.h>
#include <KernelSetupConfig.h>

typedef struct {
  LIST_ENTRY                      Link;
  UINT32                          ControllerAddress;
  SATA_DEVICE_PATH                *SataDevicePath;  
  CHAR16                          *NameString;
} LINK_SATA_NODE;

typedef struct {
  LIST_ENTRY                      Link;
  UINT32                          NvmeAddress;
  CHAR16                          *NameString;
  CHAR16                          *SizeString;
} LINK_NVME_NODE;

VOID
TruncateLastNode (
  IN OUT EFI_DEVICE_PATH_PROTOCOL   *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *LastNode;
  EFI_DEVICE_PATH_PROTOCOL    *DevPath;

  LastNode = NULL;
  DevPath = DevicePath;

  while (!IsDevicePathEnd (DevPath)) {
      LastNode = DevPath;
      DevPath = NextDevicePathNode (DevPath);
  }

  if (LastNode != NULL) {
    CopyMem (LastNode, DevPath, sizeof (EFI_DEVICE_PATH_PROTOCOL));
  }

  return;
}

EFI_STATUS
GetParentHandle (
  IN EFI_HANDLE      ChildHandle,
  OUT EFI_HANDLE      *ParentHandle
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL      *TmpDevPath;

  Status = gBS->HandleProtocol (
                  ChildHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TmpDevPath = DuplicateDevicePath (DevicePath);
  if (TmpDevPath == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  TruncateLastNode (TmpDevPath);

  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &TmpDevPath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  *ParentHandle = Handle;

Exit:

  gBS->FreePool (TmpDevPath);

  return Status;
}

EFI_STATUS
UpdateNvmeForm (
  IN EFI_HII_HANDLE                    HiiHandle,
  IN LIST_ENTRY                        *NvmeList
  )
{
  EFI_STATUS                            Status;
  VOID                                  *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL                    *StartLabel;
  EFI_STRING_ID                         StrToken1;
  EFI_STRING_ID                         StrToken2;
  LINK_NVME_NODE                        *NvmeNode;
  CHAR16                                *AddressString;
  CHAR16                                SizeString1[] = {L"NVMe Size"};

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = NVME_CONFIG_START_LABEL;

  while (!IsListEmpty (NvmeList)) {
    NvmeNode = (LINK_NVME_NODE*)GetFirstNode (NvmeList);
    RemoveEntryList (&NvmeNode->Link);

    AddressString = CatSPrint (NULL, L"Bus:%d Dev:%d Func:%d",
                                 (NvmeNode->NvmeAddress >> 16) & 0xFF,
                                 (NvmeNode->NvmeAddress >> 8) & 0xFF,
                                 (NvmeNode->NvmeAddress) & 0xFF
                                 );
    StrToken1 = HiiSetString (HiiHandle, 0, AddressString, NULL);
    StrToken2 = HiiSetString (HiiHandle, 0, NvmeNode->NameString, NULL);
    HiiCreateTextOpCode (
      StartOpCodeHandle,
      StrToken1,
      0,
      StrToken2
      );

    StrToken1 = HiiSetString (HiiHandle, 0, SizeString1, NULL);
    StrToken2 = HiiSetString (HiiHandle, 0, NvmeNode->SizeString, NULL);
    HiiCreateTextOpCode (
      StartOpCodeHandle,
      StrToken1,
      0,
      StrToken2
      );

    HiiCreateTextOpCode (
      StartOpCodeHandle,
      STRING_TOKEN(STR_BLANK_STRING),
      0,
      0
      );

    gBS->FreePool(AddressString);
    gBS->FreePool(NvmeNode->NameString);
    gBS->FreePool(NvmeNode->SizeString);
    gBS->FreePool(NvmeNode);
  }

  //
  // Add Text op-code
  //
  Status = HiiUpdateForm (
             HiiHandle,
             NULL,
             0x24,  //NVME Form ID
             StartOpCodeHandle,
             NULL
             );

  HiiFreeOpCodeHandle (StartOpCodeHandle);

  return Status;
}

VOID
InsertNvmeNode (
  IN LIST_ENTRY                            *NvmeList,
  IN LINK_NVME_NODE                        *NvmeNode
  )
{
  LINK_NVME_NODE                           *Node;
  LINK_NVME_NODE                           *PrevNode;

  //
  // Insert a new entry on top of the list
  //
  InsertHeadList (NvmeList, &NvmeNode->Link);

  // Refer ShellCommandRegisterCommandName
  //
  // Move a new node to its sorted ordered location in the list
  //
  for (Node = (LINK_NVME_NODE *)GetFirstNode (NvmeList),
        PrevNode = (LINK_NVME_NODE *)GetFirstNode (NvmeList)
        ; !IsNull (NvmeList, &Node->Link)
        ; Node = (LINK_NVME_NODE *)GetNextNode (NvmeList, &Node->Link)) {
          //
          // Swap PrevNode and Node list entry if PrevNode list entry
          // is greater than Node list entry
          //
          if (PrevNode->NvmeAddress > Node->NvmeAddress) {
            Node = (LINK_NVME_NODE *) SwapListEntries (&PrevNode->Link, &Node->Link);
          } else if (PrevNode->NvmeAddress < Node->NvmeAddress) {
            //
            // PrevNode entry is lower than Node entry
            //
            break;
          }
  }
}

CHAR16*
GetContollerName (
  EFI_HANDLE                               ControllerHandle,
  EFI_HANDLE                               ChildHandle,
  BOOLEAN                                  IsComponentName2
  )
{
  EFI_STATUS                               Status;
  UINTN                                    Loop;
  EFI_COMPONENT_NAME_PROTOCOL              *ComponentName;
  UINTN                                    ComponentNameHandleCount;
  EFI_HANDLE                               *ComponentNameHandleBuffer;
  CHAR8                                    *SupportedLanguages;
  EFI_GUID*                                ComponentNameGuid;
  CHAR16                                   *DevNameString;
  CHAR16                                   *ModelNametring;
  UINTN                                    StringSize;

  if (IsComponentName2) {
    ComponentNameGuid = &gEfiComponentName2ProtocolGuid;
  } else {
    ComponentNameGuid = &gEfiComponentNameProtocolGuid;
  }

  Loop                      = 0;
  ComponentNameHandleCount  = 0;
  ComponentNameHandleBuffer = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  ComponentNameGuid,
                  NULL,
                  &ComponentNameHandleCount,
                  &ComponentNameHandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  ModelNametring     = NULL;
  DevNameString      = NULL;

  ComponentName = NULL;
  for (Loop = 0; Loop < ComponentNameHandleCount; Loop++) {
    Status = gBS->HandleProtocol (
                    ComponentNameHandleBuffer[Loop],
                    ComponentNameGuid,
                    (VOID **) &ComponentName
                    );
    if (EFI_ERROR (Status)) {
       continue;
    }

    SupportedLanguages = ComponentName->SupportedLanguages;
    ModelNametring     = NULL;
    Status = ComponentName->GetControllerName (
                              ComponentName,
                              ControllerHandle,
                              ChildHandle,
                              SupportedLanguages,
                              &ModelNametring
                              );
    if (EFI_ERROR (Status)) {
      //
      // For some Single-LUN storages, BlockIo protocol may be installed on the ControllerHandle instead of a new handle created.
      // Tring the BlockIo handle as the ControllerHandle for retrieving the device name.
      //
      ModelNametring     = NULL;
      Status = ComponentName->GetControllerName (
                                ComponentName,
                                ChildHandle,
                                ChildHandle,
                                SupportedLanguages,
                                &ModelNametring
                                );
    }
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (!IsValidComponentNameProtocol (ComponentName, ControllerHandle)) {
      continue;
    }

    if (ModelNametring) {
      StringSize = StrnSizeS (ModelNametring, PcdGet32 (PcdMaximumUnicodeStringLength));
      ASSERT (StringSize != 0);
      DevNameString = AllocateCopyPool (StringSize, ModelNametring);
      CleanSpaceChar (DevNameString);
      break;
    }
  }

  if (ComponentNameHandleCount != 0) {
    gBS->FreePool (ComponentNameHandleBuffer);
  }
  return DevNameString;
}

CHAR16*
GetDiskName (
  EFI_HANDLE                               DiskInfoHandle
)
{
  EFI_STATUS                               Status;
  CHAR16                                   *DevNameString;
  EFI_HANDLE                               ControllerHandle;
  EFI_DEVICE_PATH_PROTOCOL                 *DiskInfoDevicePath;
  EFI_DEVICE_PATH_PROTOCOL                 *DiskInfoTmpDevPath;
  EFI_DEVICE_PATH_PROTOCOL                 *AppendedDevicePath;
  EFI_DEVICE_PATH_PROTOCOL                 *ControllerDevicePath;

  Status = gBS->HandleProtocol (
                  DiskInfoHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID *) &DiskInfoDevicePath
                  );

  if (EFI_ERROR (Status)) {
    return NULL;
  }

  DiskInfoTmpDevPath = DiskInfoDevicePath;
  AppendedDevicePath = NULL;
  ControllerDevicePath = NULL;
  while (!IsDevicePathEnd (NextDevicePathNode (DiskInfoTmpDevPath))) {
    ControllerDevicePath = AppendDevicePathNode (AppendedDevicePath, DiskInfoTmpDevPath);
    if (AppendedDevicePath != NULL) {
      gBS->FreePool (AppendedDevicePath);
    }
    AppendedDevicePath = ControllerDevicePath;
    DiskInfoTmpDevPath = NextDevicePathNode (DiskInfoTmpDevPath);
  }

  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &ControllerDevicePath,
                  &ControllerHandle
                  );
  gBS->FreePool (AppendedDevicePath);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  DevNameString = GetContollerName (ControllerHandle, DiskInfoHandle, TRUE);
  if (DevNameString == NULL) {
    DevNameString = GetContollerName (ControllerHandle, DiskInfoHandle, FALSE);
  }
  return DevNameString;
}

/**
 to check NVMe device and collect the device information.

 @param [in]   HiiHandle        the handle of Advance menu.
 @param [in]   SetupVariable    the SYSTEM CONFIGURATION of SetupBrowser.

 @retval EFI_SUCCESS            it is success to check and get device information.

**/
EFI_STATUS
InitNvmeConfig (
  IN EFI_HII_HANDLE                        HiiHandle,
  IN CHIPSET_CONFIGURATION                 *SetupVariable
  )
{
  EFI_STATUS                               Status;
  EFI_HANDLE                               *DiskInfoBuffer;
  UINTN                                    DiskInfoCount;
  UINTN                                    DiskInfoIndex;
  EFI_DISK_INFO_PROTOCOL                   *DiskInfo;
  EFI_HANDLE                               PciIoHandle;
  EFI_PCI_IO_PROTOCOL                      *PciIo;
  EFI_BLOCK_IO_PROTOCOL                    *BlockIo;
  UINTN                                    Segment;
  UINTN                                    Bus;
  UINTN                                    Dev;
  UINTN                                    Function;
  CHAR16                                   *DevNameString;
  LIST_ENTRY                               NvmeList;
  LINK_NVME_NODE                           *NvmeNode;

  DevNameString      = NULL;

  DiskInfoBuffer     = NULL;
  DiskInfoCount      = 0;
  DiskInfoIndex      = 0;

  Segment            = 0;
  Bus                = 0;
  Dev                = 0;
  Function           = 0;

  InitializeListHead (&NvmeList);

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDiskInfoProtocolGuid,
                  NULL,
                  &DiskInfoCount,
                  &DiskInfoBuffer
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (DiskInfoIndex = 0; DiskInfoIndex < DiskInfoCount; DiskInfoIndex++) {

    Status = gBS->HandleProtocol (
                    DiskInfoBuffer[DiskInfoIndex],
                    &gEfiDiskInfoProtocolGuid,
                    (VOID **) &DiskInfo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (CompareGuid (&DiskInfo->Interface, &gEfiDiskInfoNvmeInterfaceGuid)) {

      Status = GetParentHandle (DiskInfoBuffer[DiskInfoIndex], &PciIoHandle);

      if (!EFI_ERROR (Status)) {
        //
        // Check for all NVMe device
        //
        Status = gBS->HandleProtocol (
                        PciIoHandle,
                        &gEfiPciIoProtocolGuid,
                        (VOID **) &PciIo
                        );

        Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Dev, &Function);
      }

      Status = gBS->HandleProtocol (
                      DiskInfoBuffer[DiskInfoIndex],
                      &gEfiBlockIoProtocolGuid,
                      (VOID **) &BlockIo
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }

      NvmeNode = AllocateZeroPool (sizeof (LINK_NVME_NODE));
      if (NvmeNode == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      NvmeNode->NameString = GetDiskName (DiskInfoBuffer[DiskInfoIndex]);
      if (NvmeNode->NameString == NULL) {
        NvmeNode->NameString = CatSPrint (NULL, L"Unknown Device");
      }
      NvmeNode->SizeString = CatSPrint (
                               NULL,
                               L"%d GB",
                               DivU64x32 (MultU64x32 (BlockIo->Media->LastBlock+1, BlockIo->Media->BlockSize), 1000*1000*1000)
                               );
      NvmeNode->NvmeAddress = (UINT32)((Bus << 16) + (Dev << 8) + Function);

      InsertNvmeNode (&NvmeList, NvmeNode);
    }
  }

  UpdateNvmeForm ( HiiHandle, &NvmeList);

  if (DiskInfoCount != 0) {
    gBS->FreePool (DiskInfoBuffer);
  }

  return EFI_SUCCESS;
}

VOID
InsertSataNode (
  IN LIST_ENTRY                            *SataList,
  IN LINK_SATA_NODE                        *SataNode
  )
{
  LINK_SATA_NODE                           *Node;
  LINK_SATA_NODE                           *PrevNode;

  //
  // Insert a new entry on top of the list
  //
  InsertHeadList (SataList, &SataNode->Link);

  // Refer ShellCommandRegisterCommandName
  //
  // Move a new node to its sorted ordered location in the list
  //
  for (Node = (LINK_SATA_NODE *)GetFirstNode (SataList),
        PrevNode = (LINK_SATA_NODE *)GetFirstNode (SataList)
        ; !IsNull (SataList, &Node->Link)
        ; Node = (LINK_SATA_NODE *)GetNextNode (SataList, &Node->Link)) {
          //
          // Swap PrevNode and Node list entry if PrevNode list entry
          // is greater than Node list entry
          //
          if (PrevNode->ControllerAddress > Node->ControllerAddress) {
            Node = (LINK_SATA_NODE *) SwapListEntries (&PrevNode->Link, &Node->Link);
          } else if (PrevNode->ControllerAddress < Node->ControllerAddress) {
            //
            // PrevNode entry is lower than Node entry
            //
            break;
          }
  }
}

EFI_STATUS
UpdateSataForm (
  IN EFI_HII_HANDLE                    HiiHandle,
  IN LIST_ENTRY                        *SataList
  )
{
  EFI_STATUS                            Status;
  VOID                                  *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL                    *StartLabel;
  EFI_STRING_ID                         StrToken1;
  EFI_STRING_ID                         StrToken2;
  LINK_SATA_NODE                        *SataNode;
  CHAR16                                *AddressString;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = IDE_UPDATE_LABEL;

  while (!IsListEmpty (SataList)) {
    SataNode = (LINK_SATA_NODE*)GetFirstNode (SataList);
    RemoveEntryList (&SataNode->Link);

    AddressString = CatSPrint (NULL, L"Bus:%d Dev:%d Func:%d",
                                 (SataNode->ControllerAddress >> 16) & 0xFF,
                                 (SataNode->ControllerAddress >> 8) & 0xFF,
                                 (SataNode->ControllerAddress) & 0xFF
                                 );
    StrToken1 = HiiSetString (HiiHandle, 0, AddressString, NULL);
    StrToken2 = HiiSetString (HiiHandle, 0, SataNode->NameString, NULL);
    HiiCreateTextOpCode (
      StartOpCodeHandle,
      StrToken1,
      0,
      StrToken2
      );

    HiiCreateTextOpCode (
      StartOpCodeHandle,
      STRING_TOKEN(STR_BLANK_STRING),
      0,
      0
      );

    gBS->FreePool(AddressString);
    gBS->FreePool(SataNode->NameString);
    gBS->FreePool(SataNode);
  }

  //
  // Add Text op-code
  //
  Status = HiiUpdateForm (
             HiiHandle,
             NULL,
             0x23,
             StartOpCodeHandle,
             NULL
             );

  HiiFreeOpCodeHandle (StartOpCodeHandle);

  return Status;
}

EFI_STATUS
InitSataConfig (
  IN EFI_HII_HANDLE                        HiiHandle,
  IN CHIPSET_CONFIGURATION                 *SetupVariable
  )
{
  EFI_STATUS                               Status;
  EFI_HANDLE                               *DiskInfoBuffer;
  UINTN                                    DiskInfoCount;
  UINTN                                    DiskInfoIndex;
  EFI_DISK_INFO_PROTOCOL                   *DiskInfo;
  EFI_HANDLE                               PciIoHandle;
  EFI_PCI_IO_PROTOCOL                      *PciIo;
  UINTN                                    Segment;
  UINTN                                    Bus;
  UINTN                                    Dev;
  UINTN                                    Function;
  CHAR16                                   *DevNameString;
  EFI_DEVICE_PATH_PROTOCOL                 *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL                 *DevicePathNode;  
  SATA_DEVICE_PATH                         *SataDevicePath;    
  LIST_ENTRY                               SataList;
  LINK_SATA_NODE                           *SataNode;

  DevNameString      = NULL;
  DiskInfoBuffer     = NULL;
  DiskInfoCount      = 0;
  DiskInfoIndex      = 0;
  DevicePath         = NULL;
  DevicePathNode     = NULL;
  Segment            = 0;
  Bus                = 0;
  Dev                = 0;
  Function           = 0;

  InitializeListHead (&SataList);

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDiskInfoProtocolGuid,
                  NULL,
                  &DiskInfoCount,
                  &DiskInfoBuffer
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }  
  for (DiskInfoIndex = 0; DiskInfoIndex < DiskInfoCount; DiskInfoIndex++) {

    Status = gBS->HandleProtocol (
                    DiskInfoBuffer[DiskInfoIndex],
                    &gEfiDiskInfoProtocolGuid,
                    (VOID **) &DiskInfo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    SataNode = AllocateZeroPool (sizeof (LINK_SATA_NODE));
    if (SataNode == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }  

    Status = gBS->HandleProtocol (
                    DiskInfoBuffer[DiskInfoIndex],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &DevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    DevicePathNode = DevicePath;
    Status = EFI_NOT_FOUND;
    SataDevicePath = NULL;
    while (!IsDevicePathEnd (DevicePathNode)) {
      if ((DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH) &&
          (DevicePathSubType (DevicePathNode) == MSG_SATA_DP)) {
        SataDevicePath = (SATA_DEVICE_PATH *) DevicePathNode;
        break;;
      }
      DevicePathNode = NextDevicePathNode (DevicePathNode);
    }    
    if (SataDevicePath != NULL) {
      SataNode->SataDevicePath = SataDevicePath;
      SataNode->NameString = GetDiskName (DiskInfoBuffer[DiskInfoIndex]);
      if (SataNode->NameString == NULL) {
        SataNode->NameString = CatSPrint (NULL, L"Unknown Device");
      }
      Status = GetParentHandle (DiskInfoBuffer[DiskInfoIndex], &PciIoHandle);
  
      if (!EFI_ERROR (Status)) {
        Status = gBS->HandleProtocol (
                        PciIoHandle,
                        &gEfiPciIoProtocolGuid,
                        (VOID **) &PciIo
                        );
  
        Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Dev, &Function);
        SataNode->ControllerAddress = (UINT32)((Bus << 16) + (Dev << 8) + Function);
      }       
      InsertSataNode (&SataList, SataNode);
    }
  }

  UpdateSataForm ( HiiHandle, &SataList);

  if (DiskInfoCount != 0) {
    gBS->FreePool (DiskInfoBuffer);
  }

  return EFI_SUCCESS;
}
