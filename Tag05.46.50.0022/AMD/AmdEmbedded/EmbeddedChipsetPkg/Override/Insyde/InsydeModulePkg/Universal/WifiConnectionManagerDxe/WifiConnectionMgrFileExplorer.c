/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "WifiConnectionMgrDxe.h"

WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA  mWifiMgrFileExplorerData = {0};
STRING_DEPOSITORY                    mSecureBootFileOptionStrDepository = {0};
ENROLL_MENU_OPTION                   mSecureBootFsOptionMenu  = {WIFI_FE_MENU_ENTRY_SIGNATURE, NULL, 0};
ENROLL_MENU_OPTION                   mSecureBootDirectoryMenu = {WIFI_FE_MENU_ENTRY_SIGNATURE, NULL, 0};
STATIC EFI_TEXT_STRING               mOriginalOutputString;

STATIC
BOOLEAN
WifiFeGrowBuffer (
  IN OUT EFI_STATUS   *Status,
  IN OUT VOID         **Buffer,
  IN UINTN            BufferSize
  )
{
  BOOLEAN TryAgain;

  //
  // If this is an initial request, buffer will be null with a new buffer size
  //
  if ((*Buffer == NULL) && (BufferSize != 0)) {
    *Status = EFI_BUFFER_TOO_SMALL;
  }
  //
  // If the status code is "buffer too small", resize the buffer
  //
  TryAgain = FALSE;
  if (*Status == EFI_BUFFER_TOO_SMALL) {

    if (*Buffer != NULL) {
      FreePool (*Buffer);
    }

    *Buffer = AllocateZeroPool (BufferSize);

    if (*Buffer != NULL) {
      TryAgain = TRUE;
    } else {
      *Status = EFI_OUT_OF_RESOURCES;
    }
  }
  //
  // If there's an error, free the buffer
  //
  if (!TryAgain && EFI_ERROR (*Status) && (*Buffer != NULL)) {
    FreePool (*Buffer);
    *Buffer = NULL;
  }

  return TryAgain;
}

STATIC
EFI_FILE_HANDLE
WifiFeOpenRoot (
  IN EFI_HANDLE                   DeviceHandle
  )
{
  EFI_STATUS                      Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Volume;
  EFI_FILE_HANDLE                 File;

  File = NULL;

  //
  // File the file system interface to the device
  //
  Status = gBS->HandleProtocol (
                  DeviceHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID *) &Volume
                  );

  //
  // Open the root directory of the volume
  //
  if (!EFI_ERROR (Status)) {
    Status = Volume->OpenVolume (
                      Volume,
                      &File
                      );
  }
  //
  // Done
  //
  return EFI_ERROR (Status) ? NULL : File;
}

STATIC
EFI_FILE_SYSTEM_VOLUME_LABEL *
WifiFeFileSystemVolumeLabelInfo (
  IN EFI_FILE_HANDLE      FHand
  )
{
  EFI_STATUS                        Status;
  EFI_FILE_SYSTEM_VOLUME_LABEL      *Buffer;
  UINTN                             BufferSize;
  //
  // Initialize for GrowBuffer loop
  //
  Buffer      = NULL;
  BufferSize  = SIZE_OF_EFI_FILE_SYSTEM_VOLUME_LABEL + 200;

  //
  // Call the real function
  //
  Status = EFI_SUCCESS;
  while (WifiFeGrowBuffer (&Status, (VOID **) &Buffer, BufferSize)) {
    Status = FHand->GetInfo (
                      FHand,
                      &gEfiFileSystemVolumeLabelInfoIdGuid,
                      &BufferSize,
                      Buffer
                      );
  }

  return Buffer;
}

STATIC
EFI_FILE_INFO *
WifiFeFileInfo (
  IN EFI_FILE_HANDLE      FHand
  )
{
  EFI_STATUS    Status;
  EFI_FILE_INFO *Buffer;
  UINTN         BufferSize;

  //
  // Initialize for GrowBuffer loop
  //
  Buffer      = NULL;
  BufferSize  = SIZE_OF_EFI_FILE_INFO + 200;

  //
  // Call the real function
  //
  Status = EFI_SUCCESS;
  while (WifiFeGrowBuffer (&Status, (VOID **) &Buffer, BufferSize)) {
    Status = FHand->GetInfo (
                      FHand,
                      &gEfiFileInfoGuid,
                      &BufferSize,
                      Buffer
                      );
  }

  return Buffer;
}

STATIC
VOID
WifiFeSafeFreePool (
  IN VOID                              **Buffer
  )
{
  if (Buffer != NULL && *Buffer != NULL) {
    gBS->FreePool (*Buffer);
    *Buffer = NULL;
  }
}

STATIC
VOID
WifiFeReclaimStrDepository (
  IN STRING_DEPOSITORY                 *StrDepository
  )
{
  if (StrDepository != NULL) {
    StrDepository->CurrentNode = StrDepository->ListHead;
  }
}

STATIC
EFI_STRING_ID
WifFeGetStrTokenFromDepository (
  IN WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA   *CallbackData,
  IN STRING_DEPOSITORY                     *StringDepository
  )
{
  STRING_LIST_NODE                     *CurrentListNode;
  STRING_LIST_NODE                     *NextListNode;

  if (CallbackData == NULL || StringDepository == NULL) {
    return 0;
  }

  CurrentListNode = StringDepository->CurrentNode;

  if ((NULL != CurrentListNode) && (NULL != CurrentListNode->Next)) {
    //
    // Fetch one reclaimed node from the list.
    //
    NextListNode = StringDepository->CurrentNode->Next;
  } else {
    //
    // If there is no usable node in the list, update the list.
    //
    NextListNode = AllocateZeroPool (sizeof (STRING_LIST_NODE));
    if (NextListNode == NULL) {
      return 0;
    }
    NextListNode->StringToken = HiiSetString (
                                  CallbackData->HiiHandle,
                                  NextListNode->StringToken,
                                  L" ",
                                  NULL
                                  );
    ASSERT (NextListNode->StringToken != 0);

    StringDepository->TotalNodeNumber++;

    if (NULL == CurrentListNode) {
      StringDepository->ListHead = NextListNode;
    } else {
      CurrentListNode->Next = NextListNode;
    }
  }

  StringDepository->CurrentNode = NextListNode;

  return StringDepository->CurrentNode->StringToken;
}

STATIC
VOID
EmDestroyMenuEntry (
  ENROLL_MENU_ENTRY         *MenuEntry
  )
{
  EM_FILE_CONTEXT           *FileContext;

  FileContext = (EM_FILE_CONTEXT *) MenuEntry->VariableContext;

  if (!FileContext->IsRoot) {
    WifiFeSafeFreePool ((VOID **) &FileContext->DevicePath);
  } else {
    if (FileContext->FHandle != NULL) {
      FileContext->FHandle->Close (FileContext->FHandle);
    }
  }

  WifiFeSafeFreePool ((VOID **) &FileContext->FileName);
  WifiFeSafeFreePool ((VOID **) &FileContext->Info);
  WifiFeSafeFreePool ((VOID **) &FileContext);
  WifiFeSafeFreePool ((VOID **) &MenuEntry->DisplayString);
  WifiFeSafeFreePool ((VOID **) &MenuEntry->HelpString);
  WifiFeSafeFreePool ((VOID **) &MenuEntry);
}

EFI_STATUS
EFIAPI
DummyOutputString (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL        *This,
  IN CHAR16                                 *String
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
HookOutputString (
  VOID
  )
{
  mOriginalOutputString = gST->ConOut->OutputString;
  gST->ConOut->OutputString = DummyOutputString;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
RestoreOutputString (
  VOID
  )
{
  gST->ConOut->OutputString = mOriginalOutputString;

  return EFI_SUCCESS;
}

/**
  Read file based on the input file context.

  @param[in]  FileContext       Pointer to the file context
  @param[out] FileSize          Returns the size of the file that was read
  @param[out] File              Dynamically allocated memory that contains a copy of the file

  @retval EFI_SUCCESS           Read file successfully.
  @retval EFI_UNSUPPORTED       File context is invalid or load file function return failed.
  @retval EFI_OUT_OF_RESOURCES  Fail to allocate memory.
  @retval EFI_NOT_FOUND         Fail to get file info.
  @retval Other                 Fail to get load file protocol instance or open/read file.
**/
STATIC
EFI_STATUS
EmReadFile (
  IN  EM_FILE_CONTEXT                  *FileContext,
  OUT UINTN                            *FileSize,
  OUT UINT8                            **File
  )
{
  EFI_FILE_HANDLE                      Dir;
  EFI_FILE_HANDLE                      NewDir;
  EFI_FILE_INFO                        *DirInfo;
  EFI_STATUS                           Status;
  UINTN                                BufferSize;
  UINT8                                *Buffer;
  EFI_LOAD_FILE_PROTOCOL               *LoadFile;

  if (FileContext->IsLoadFile && !FileContext->IsBootLegacy) {
    if (FileContext->Handle == NULL) {
      return EFI_UNSUPPORTED;
    }
    Status = gBS->HandleProtocol (FileContext->Handle, &gEfiLoadFileProtocolGuid, (VOID**)&LoadFile);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    HookOutputString ();
    //
    // Call LoadFile with the correct buffer size
    //
    BufferSize = 0;
    Buffer     = NULL;
    Status = LoadFile->LoadFile (
                         LoadFile,
                         FileContext->DevicePath,
                         TRUE,
                         &BufferSize,
                         Buffer
                         );
    if (Status == EFI_BUFFER_TOO_SMALL) {
      Buffer = AllocatePool (BufferSize);
      if (Buffer == NULL) {
        RestoreOutputString ();
        return EFI_OUT_OF_RESOURCES;
      }
      Status = LoadFile->LoadFile (
                           LoadFile,
                           FileContext->DevicePath,
                           TRUE,
                           &BufferSize,
                           Buffer
                           );
    }
    RestoreOutputString ();
    if (EFI_ERROR (Status)) {
      WifiFeSafeFreePool ((VOID **) &Buffer);
      return EFI_UNSUPPORTED;
    }
  } else {
    Dir = FileContext->FHandle;
    Status = Dir->Open (
                    Dir,
                    &NewDir,
                    FileContext->FileName,
                    EFI_FILE_READ_ONLY,
                    0
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    DirInfo = WifiFeFileInfo (NewDir);
    if (!DirInfo) {
      NewDir->Close (NewDir);
      return EFI_NOT_FOUND;
    }

    BufferSize = (UINTN) DirInfo->FileSize;
    Buffer     = AllocateZeroPool (BufferSize);
    ASSERT (Buffer != NULL);
    if (Buffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    WifiFeSafeFreePool ((VOID **) &DirInfo);

    Status = NewDir->Read (NewDir, &BufferSize, Buffer);
    NewDir->Close (NewDir);
    if (EFI_ERROR (Status)) {
      FreePool (Buffer);
      return Status;
    }
  }

  *File     = Buffer;
  *FileSize = BufferSize;
  return EFI_SUCCESS;
}

/**
  Append file name to existing file name.

  @param  Str1         existing file name
  @param  Str2         file name to be appended

  @return Allocate a new string to hold the appended result.
          Caller is responsible to free the returned string.

**/
CHAR16 *
EmAppendFileName (
  IN  CHAR16  *Str1,
  IN  CHAR16  *Str2
  )
{
  UINTN   Size1;
  UINTN   Size2;
  CHAR16  *Str;
  CHAR16  *TmpStr;
  CHAR16  *Ptr;
  CHAR16  *LastSlash;

  Size1 = StrSize (Str1);
  Size2 = StrSize (Str2);
  //
  // Integer Overflow check
  //
  if (Size1 > Size1 + Size2 + sizeof (CHAR16) || Size2 > Size1 + Size2 + sizeof (CHAR16)) {
    ASSERT (FALSE);
    return NULL;
  }
  Str   = AllocateZeroPool (Size1 + Size2 + sizeof (CHAR16));
  ASSERT (Str != NULL);
  if (Str == NULL) {
    return NULL;
  }
  TmpStr = AllocateZeroPool (Size1 + Size2 + sizeof (CHAR16));
  ASSERT (TmpStr != NULL);
  if (TmpStr == NULL) {
    return NULL;
  }

  StrCatS (Str, (Size1 + Size2 + sizeof (CHAR16)) / sizeof(CHAR16), Str1);
  if (!((*Str == '\\') && (*(Str + 1) == 0))) {
    StrCatS (Str, (Size1 + Size2 + sizeof (CHAR16)) / sizeof(CHAR16), L"\\");
  }

  StrCatS (Str, (Size1 + Size2 + sizeof (CHAR16)) / sizeof(CHAR16), Str2);

  Ptr       = Str;
  LastSlash = Str;
  while (*Ptr != 0) {
    if (*Ptr == '\\' && *(Ptr + 1) == '.' && *(Ptr + 2) == '.' && *(Ptr + 3) == L'\\') {
      //
      // Convert "\Name\..\" to "\"
      // DO NOT convert the .. if it is at the end of the string. This will
      // break the .. behavior in changing directories.
      //

      //
      // Use TmpStr as a backup, as StrCpy in BaseLib does not handle copy of two strings
      // that overlap.
      //
      StrCpyS (TmpStr, (Size1 + Size2 + sizeof (CHAR16)) / sizeof(CHAR16), Ptr + 3);
      StrCpyS (LastSlash,  (Size1 + Size2 + sizeof (CHAR16) - ((UINT8 *) LastSlash - (UINT8 *) Str)) / sizeof(CHAR16), TmpStr);
      Ptr = LastSlash;
    } else if (*Ptr == '\\' && *(Ptr + 1) == '.' && *(Ptr + 2) == '\\') {
      //
      // Convert a "\.\" to a "\"
      //

      //
      // Use TmpStr as a backup, as StrCpy in BaseLib does not handle copy of two strings
      // that overlap.
      //
      StrCpyS (TmpStr,  (Size1 + Size2 + sizeof (CHAR16))/sizeof(CHAR16), Ptr + 2);
      StrCpyS (Ptr,  (Size1 + Size2 + sizeof (CHAR16)) / sizeof(CHAR16), TmpStr);
      Ptr = LastSlash;
    } else if (*Ptr == '\\') {
      LastSlash = Ptr;
    }

    Ptr++;
  }

  FreePool (TmpStr);

  return Str;
}

/**
  Create Menu Entry for future use, make all types together
  in order to reduce code size

  @param  MenuType      Use this parameter to identify current
                        Menu type

  @return A valid pointer pointing to the allocated memory pool for current menu entry
          or NULL if it cannot allocate memory for current menu entry

**/
STATIC
ENROLL_MENU_ENTRY *
EmCreateFileMenuEntry (
  VOID
  )
{
  ENROLL_MENU_ENTRY *MenuEntry;
  UINTN             ContextSize;

  ContextSize = sizeof (BM_FILE_CONTEXT);

  MenuEntry = AllocateZeroPool (sizeof (ENROLL_MENU_ENTRY));
  if (NULL == MenuEntry) {
    return MenuEntry;
  }

  MenuEntry->VariableContext = AllocateZeroPool (ContextSize);
  if (NULL == MenuEntry->VariableContext) {
    WifiFeSafeFreePool ((VOID**) &MenuEntry);
    MenuEntry = NULL;
    return MenuEntry;
  }

  MenuEntry->Signature        = WIFI_FE_MENU_ENTRY_SIGNATURE;
  MenuEntry->ContextSelection = WIFI_FE_FILE_CONTEXT_SELECT;
  return MenuEntry;
}


/**
  Find files under current directory
  All files and sub-directories in current directory
  will be stored in DirectoryMenu for future use.

  @param  FileOption    Pointer for Dir to explore

  @retval TRUE          Get files from current dir successfully
  @retval FALSE         Can't get files from current dir

**/
EFI_STATUS
EmFindFiles (
  IN WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA    *CallbackData,
  IN ENROLL_MENU_ENTRY                      *MenuEntry
  )
{
  EFI_FILE_HANDLE     NewDir;
  EFI_FILE_HANDLE     Dir;
  EFI_FILE_INFO       *DirInfo;
  UINTN               BufferSize;
  UINTN               DirBufferSize;
  ENROLL_MENU_ENTRY   *NewMenuEntry;
  EM_FILE_CONTEXT     *FileContext;
  EM_FILE_CONTEXT     *NewFileContext;
  UINTN               Pass;
  EFI_STATUS          Status;
  UINTN               OptionNumber;

  FileContext   = (EM_FILE_CONTEXT *) MenuEntry->VariableContext;
  Dir           = FileContext->FHandle;
  OptionNumber  = 0;
  //
  // Open current directory to get files from it
  //
  Status = Dir->Open (
                  Dir,
                  &NewDir,
                  FileContext->FileName,
                  EFI_FILE_READ_ONLY,
                  0
                  );
  if (!FileContext->IsRoot) {
    Dir->Close (Dir);
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DirInfo = WifiFeFileInfo (NewDir);
  if (!DirInfo) {
    return EFI_NOT_FOUND;
  }
  if (!(DirInfo->Attribute & EFI_FILE_DIRECTORY)) {
    return EFI_INVALID_PARAMETER;
  }

  FileContext->DevicePath = FileDevicePath (FileContext->Handle, FileContext->FileName);

  DirBufferSize = sizeof (EFI_FILE_INFO) + 1024;
  DirInfo       = AllocateZeroPool (DirBufferSize);
  if (!DirInfo) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Get all files in current directory
  // Pass 1 to get Directories
  // Pass 2 to get files that are EFI images
  //
  for (Pass = 1; Pass <= 2; Pass++) {
    NewDir->SetPosition (NewDir, 0);
    for (;;) {
      BufferSize  = DirBufferSize;
      Status      = NewDir->Read (NewDir, &BufferSize, DirInfo);
      if (EFI_ERROR (Status) || BufferSize == 0) {
        break;
      }

      if ((DirInfo->Attribute & EFI_FILE_DIRECTORY && Pass == 2) ||
          (!(DirInfo->Attribute & EFI_FILE_DIRECTORY) && Pass == 1)
          ) {
        //
        // Pass 1 is for Directories
        // Pass 2 is for file names
        //
        continue;
      }

      NewMenuEntry = EmCreateFileMenuEntry ();
      if (NULL == NewMenuEntry) {
        return EFI_OUT_OF_RESOURCES;
      }

      NewFileContext           = (EM_FILE_CONTEXT *) NewMenuEntry->VariableContext;
      NewFileContext->Handle   = FileContext->Handle;
      NewFileContext->FileName = EmAppendFileName (FileContext->FileName, DirInfo->FileName);
      if (NewFileContext->FileName == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      NewFileContext->FHandle    = NewDir;
      NewFileContext->DevicePath = FileDevicePath (NewFileContext->Handle, NewFileContext->FileName);
      NewMenuEntry->HelpString   = NULL;

      MenuEntry->DisplayStringToken = WifFeGetStrTokenFromDepository (CallbackData, &mSecureBootFileOptionStrDepository);

      NewFileContext->IsDir = (BOOLEAN) ((DirInfo->Attribute & EFI_FILE_DIRECTORY) == EFI_FILE_DIRECTORY);
      if (NewFileContext->IsDir) {
        BufferSize                  = StrLen (DirInfo->FileName) * 2 + 6;
        NewMenuEntry->DisplayString = AllocateZeroPool (BufferSize);
        UnicodeSPrint (NewMenuEntry->DisplayString, BufferSize, L"<%s>", DirInfo->FileName);
      } else {
        NewMenuEntry->DisplayString = AllocateCopyPool (StrSize (DirInfo->FileName), DirInfo->FileName);
      }

      NewFileContext->IsRoot            = FALSE;
      NewFileContext->IsLoadFile        = FALSE;
      NewFileContext->IsRemovableMedia  = FALSE;
      NewMenuEntry->OptionNumber        = OptionNumber;
      OptionNumber++;
      InsertTailList (&mSecureBootDirectoryMenu.Head, &NewMenuEntry->Link);
    }
  }

  mSecureBootDirectoryMenu.MenuNumber = OptionNumber;
  WifiFeSafeFreePool ((VOID**) &DirInfo);
  return EFI_SUCCESS;
}


/**
  Use this routine to get one particular menu entry in specified menu

  @param  MenuOption         The menu that we will search
  @param  MenuNumber         The menunubmer that we want

  @return The desired menu entry

**/
ENROLL_MENU_ENTRY *
EmGetMenuEntry (
  ENROLL_MENU_OPTION     *MenuOption,
  UINTN                  MenuNumber
  )
{
  ENROLL_MENU_ENTRY   *NewMenuEntry;
  UINTN               Index;
  LIST_ENTRY          *List;

  if (MenuNumber >= MenuOption->MenuNumber) {
    return NULL;
  }

  List = MenuOption->Head.ForwardLink;
  for (Index = 0; Index < MenuNumber; Index++) {
    List = List->ForwardLink;
  }

  NewMenuEntry = CR (List, ENROLL_MENU_ENTRY, Link, WIFI_FE_MENU_ENTRY_SIGNATURE);

  return NewMenuEntry;
}

/**
  Update the Enroll Menu page.

  @param  CallbackData         Pointer to SECURE_BOOT_MANAGER_CALLBACK_DATA instance
  @param  MenuOption           Pointer to menu options to display.

**/
VOID
UpdateEnrollMenuPage (
  IN WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA   *CallbackData,
  IN ENROLL_MENU_OPTION                    *MenuOption
  )
{
  UINTN                       Index;
  ENROLL_MENU_ENTRY           *NewMenuEntry;
  EM_FILE_CONTEXT             *NewFileContext;
  VOID                        *StartOpCodeHandle;
  VOID                        *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL          *StartLabel;
  EFI_IFR_GUID_LABEL          *EndLabel;
  EFI_GUID                    WifiConfigFormSetGuid = WIFI_CONFIG_FORM_SET_GUID;

  NewMenuEntry    = NULL;
  NewFileContext  = NULL;

  //
  // Allocate space for creation of UpdateData Buffer
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle   = HiiAllocateOpCodeHandle ();
  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = LABEL_FORM_FILE_EXPLORER_START_ID;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = LABEL_FORM_FILE_EXPLORER_END_ID;

  for (Index = 0; Index < MenuOption->MenuNumber; Index++) {
    NewMenuEntry    = EmGetMenuEntry (MenuOption, Index);
    NewFileContext  = (EM_FILE_CONTEXT *) NewMenuEntry->VariableContext;

    if (NewFileContext->IsBootLegacy) {
      continue;
    }

    if (NewFileContext->IsDir || NewFileContext->IsLoadFile) {
      HiiCreateActionOpCode (
        StartOpCodeHandle,
        (UINT16) (KEY_FILE_EXPLORER_FILE_OPTION_BASE + Index),
        NewMenuEntry->DisplayStringToken,
        STRING_TOKEN (STR_NULL_STRING),
        EFI_IFR_FLAG_CALLBACK,
        0
        );
    } else {
      HiiCreateGotoExOpCode (
        StartOpCodeHandle,
        CallbackData->EnrollFormId,
        NewMenuEntry->DisplayStringToken,
        STRING_TOKEN (STR_NULL_STRING),
        EFI_IFR_FLAG_CALLBACK,
        (UINT16) (KEY_FILE_EXPLORER_FILE_OPTION_BASE + Index),
        1,    // specify question ID 1 to make old browser to jump to first selectable question in this form.
        NULL,
        0
        );
    }
  }
  HiiUpdateForm (
    CallbackData->HiiHandle,
    &WifiConfigFormSetGuid,
    FORMID_FILE_EXPLORER,
    StartOpCodeHandle,
    EndOpCodeHandle
    );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  Create string tokens for a menu from its help strings and display strings

  @param  HiiHandle         Hii Handle of the package to be updated.
  @param  MenuOption        The Menu whose string tokens need to be created

  @retval EFI_SUCCESS   string tokens created successfully
  @retval Other         contain some errors

**/
EFI_STATUS
EmCreateMenuStringToken (
  IN EFI_HII_HANDLE                   HiiHandle,
  IN ENROLL_MENU_OPTION               *MenuOption
  )
{
  ENROLL_MENU_ENTRY *NewMenuEntry;
  UINTN             Index;

  for (Index = 0; Index < MenuOption->MenuNumber; Index++) {
    NewMenuEntry = EmGetMenuEntry (MenuOption, Index);
    NewMenuEntry->DisplayStringToken = HiiSetString (HiiHandle, NewMenuEntry->DisplayStringToken, NewMenuEntry->DisplayString, NULL);
    if (NULL == NewMenuEntry->HelpString) {
      NewMenuEntry->HelpStringToken = NewMenuEntry->DisplayStringToken;
    } else {
      NewMenuEntry->HelpStringToken = HiiSetString (HiiHandle, NewMenuEntry->HelpStringToken, NewMenuEntry->HelpString, NULL);
    }
  }

  return EFI_SUCCESS;
}

/**
  Free resources allocated in Allocate Rountine

  @param  FreeMenu      Menu to be freed

**/
VOID
EmFreeMenu (
  ENROLL_MENU_OPTION       *FreeMenu
  )
{
  ENROLL_MENU_ENTRY    *MenuEntry;

  while (!IsListEmpty (&FreeMenu->Head)) {
    MenuEntry = CR (
                  FreeMenu->Head.ForwardLink,
                  ENROLL_MENU_ENTRY,
                  Link,
                  WIFI_FE_MENU_ENTRY_SIGNATURE
                  );
    RemoveEntryList (&MenuEntry->Link);
    EmDestroyMenuEntry (MenuEntry);
  }
}

/**
  Find file systems for current Extensible Firmware
  Including Handles that support Simple File System
  protocol, Load File protocol.

  Building up the FileSystem Menu for user selection
  All file system will be stored in FsOptionMenu
  for future use.

  @param  CallbackData  BMM context data

  @retval EFI_SUCCESS               Success find the file system
  @retval EFI_OUT_OF_RESOURCES      Can not create menu entry

**/
EFI_STATUS
EmFindFileSystem (
  VOID
  )
{
  UINTN                     NoBlkIoHandles;
  UINTN                     NoSimpleFsHandles;
  EFI_HANDLE                *BlkIoHandle;
  EFI_HANDLE                *SimpleFsHandle;
  UINT16                    *VolumeLabel;
  EFI_BLOCK_IO_PROTOCOL     *BlkIo;
  UINTN                     Index;
  EFI_STATUS                Status;
  ENROLL_MENU_ENTRY          *MenuEntry;
  EM_FILE_CONTEXT           *FileContext;
  UINTN                     OptionNumber;
  VOID                      *Buffer;
  BOOLEAN                   RemovableMedia;



  NoSimpleFsHandles = 0;
  OptionNumber      = 0;
  InitializeListHead (&mSecureBootFsOptionMenu.Head);

  //
  // Locate Handles that support BlockIo protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &NoBlkIoHandles,
                  &BlkIoHandle
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < NoBlkIoHandles; Index++) {
      Status = gBS->HandleProtocol (BlkIoHandle[Index], &gEfiBlockIoProtocolGuid, (VOID **) &BlkIo);
      if (EFI_ERROR (Status)) {
        continue;
      }

      //
      // Issue a dummy read to trigger reinstall of BlockIo protocol for removable media
      //
      if (BlkIo->Media->RemovableMedia) {
        Buffer = AllocateZeroPool (BlkIo->Media->BlockSize);
        if (NULL == Buffer) {
          WifiFeSafeFreePool ((VOID**) &BlkIoHandle);
          return EFI_OUT_OF_RESOURCES;
        }

        BlkIo->ReadBlocks (BlkIo, BlkIo->Media->MediaId, 0, BlkIo->Media->BlockSize, Buffer);
        WifiFeSafeFreePool ((VOID**) &Buffer);
      }
    }
    WifiFeSafeFreePool ((VOID**) &BlkIoHandle);
  }

  //
  // Locate Handles that support Simple File System protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NoSimpleFsHandles,
                  &SimpleFsHandle
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Find all the instances of the File System prototocol
    //
    for (Index = 0; Index < NoSimpleFsHandles; Index++) {
      Status = gBS->HandleProtocol (SimpleFsHandle[Index], &gEfiBlockIoProtocolGuid, (VOID **) &BlkIo);
      if (EFI_ERROR (Status)) {
        //
        // If no block IO exists assume it's NOT a removable media
        //
        RemovableMedia = FALSE;
      } else {
        //
        // If block IO exists check to see if it's remobable media
        //
        RemovableMedia = BlkIo->Media->RemovableMedia;
      }

      //
      // Allocate pool for this load option
      //
      MenuEntry = EmCreateFileMenuEntry ();
      if (NULL == MenuEntry) {
        WifiFeSafeFreePool ((VOID**) &SimpleFsHandle);
        return EFI_OUT_OF_RESOURCES;
      }

      FileContext = (EM_FILE_CONTEXT *) MenuEntry->VariableContext;

      FileContext->Handle     = SimpleFsHandle[Index];
      MenuEntry->OptionNumber = Index;
      FileContext->FHandle    = WifiFeOpenRoot (FileContext->Handle);
      if (!FileContext->FHandle) {
        EmDestroyMenuEntry (MenuEntry);
        continue;
      }

      MenuEntry->HelpString = ConvertDevicePathToText (
                                DevicePathFromHandle (FileContext->Handle),
                                FALSE,
                                TRUE
                                );
      FileContext->Info     = WifiFeFileSystemVolumeLabelInfo (FileContext->FHandle);
      FileContext->FileName = AllocateCopyPool (sizeof (L"\\"), L"\\");
      if (FileContext->FileName == NULL) {
        EmDestroyMenuEntry (MenuEntry);
        WifiFeSafeFreePool ((VOID**) &SimpleFsHandle);
        return EFI_OUT_OF_RESOURCES;
      }
      FileContext->DevicePath       = FileDevicePath (FileContext->Handle, FileContext->FileName);
      FileContext->IsDir            = TRUE;
      FileContext->IsRoot           = TRUE;
      FileContext->IsRemovableMedia = FALSE;
      FileContext->IsLoadFile       = FALSE;

      //
      // Get current file system's Volume Label
      //
      if (FileContext->Info == NULL) {
        VolumeLabel = L"NO FILE SYSTEM INFO";
      } else {
        VolumeLabel = FileContext->Info->VolumeLabel;
        if (*VolumeLabel == 0x0000) {
          VolumeLabel = L"NO VOLUME LABEL";
        }
      }

      MenuEntry->DisplayString  = AllocateZeroPool (MAX_CHAR);
      ASSERT (MenuEntry->DisplayString != NULL);
      UnicodeSPrint (
        MenuEntry->DisplayString,
        MAX_CHAR,
        L"%s, [%s]",
        VolumeLabel,
        MenuEntry->HelpString
        );
      OptionNumber++;
      InsertTailList (&mSecureBootFsOptionMenu.Head, &MenuEntry->Link);
    }
  }
  if (NoSimpleFsHandles != 0) {
    WifiFeSafeFreePool ((VOID**) &SimpleFsHandle);
  }

  //
  // Remember how many file system options are here
  //
  mSecureBootFsOptionMenu.MenuNumber = OptionNumber;
  return EFI_SUCCESS;
}

/**
  Update the file explower page with the refershed file system.

  @param  CallbackData      BSECURE_BOOT_MANAGER_CALLBACK_DATA instance
  @param  KeyValue          Key value to identify the type of data to expect.

  @retval TRUE              Inform the caller to create a callback packet to exit file explorer.
  @retval FALSE             Indicate that there is no need to exit file explorer.

**/
BOOLEAN
UpdateEmFileExplorer (
  IN  UINT16                                KeyValue,
  OUT EFI_DEVICE_PATH_PROTOCOL              **FilePath
  )
{
  WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA       *CallbackData;
  UINT16                                    FileOptionMask;
  ENROLL_MENU_ENTRY                         *NewMenuEntry;
  EM_FILE_CONTEXT                           *NewFileContext;
  EFI_STATUS                                Status;

  CallbackData      = &mWifiMgrFileExplorerData;
  FileOptionMask    = (UINT16) (KeyValue - KEY_FILE_EXPLORER_FILE_OPTION_BASE);
  NewMenuEntry      = NULL;

  if (EM_UNKNOWN_CONTEXT == CallbackData->EmDisplayContext) {
    EmFindFileSystem ();
    EmCreateMenuStringToken (CallbackData->HiiHandle, &mSecureBootFsOptionMenu);
    UpdateEnrollMenuPage (CallbackData, &mSecureBootFsOptionMenu);
    CallbackData->EmDisplayContext = EM_FILE_SYSTEM;
  } else {
    if (EM_FILE_SYSTEM == CallbackData->EmDisplayContext) {
      NewMenuEntry = EmGetMenuEntry (&mSecureBootFsOptionMenu, FileOptionMask);
    } else if (EM_DIRECTORY == CallbackData->EmDisplayContext) {
      NewMenuEntry = EmGetMenuEntry (&mSecureBootDirectoryMenu, FileOptionMask);
    }
    if (NewMenuEntry == NULL) {
      return FALSE;
    }
    NewFileContext = (EM_FILE_CONTEXT *) NewMenuEntry->VariableContext;

    if (NewFileContext->IsDir) {
      CallbackData->EmDisplayContext = EM_DIRECTORY;

      RemoveEntryList (&NewMenuEntry->Link);
      EmFreeMenu (&mSecureBootDirectoryMenu);
      WifiFeReclaimStrDepository (&mSecureBootFileOptionStrDepository);

      Status = EmFindFiles (CallbackData, NewMenuEntry);
      EmDestroyMenuEntry (NewMenuEntry);
      if (EFI_ERROR (Status)) {
        return TRUE;
      }

      EmCreateMenuStringToken (CallbackData->HiiHandle, &mSecureBootDirectoryMenu);
      UpdateEnrollMenuPage (CallbackData, &mSecureBootDirectoryMenu);
    } else {
      *FilePath = NewFileContext->DevicePath;
    }
  }

  return FALSE;
}

/**
  Initialize file system for Enroll menu

  @param[in] EnrollFormId    Enroll form ID

  @retval EFI_SUCCESS        Initialize enroll menu successful
  @retval Other              Fail to get the file format of enroll menu.
**/
EFI_STATUS
InitializeEnrollMenu (
  IN EFI_FORM_ID                           EnrollFormId
  )
{
  mWifiMgrFileExplorerData.EnrollFormId     = EnrollFormId;
  mWifiMgrFileExplorerData.EmDisplayContext = EM_UNKNOWN_CONTEXT;

  UpdateEmFileExplorer (0, NULL);
  return EFI_SUCCESS;
}

EFI_STATUS
WifiMgrFileExplorerInit (
  IN WIFI_MGR_PRIVATE_DATA                 *Private
  )
{
  mWifiMgrFileExplorerData.HiiHandle = Private->RegisteredHandle;

  InitializeListHead (&mSecureBootFsOptionMenu.Head);
  InitializeListHead (&mSecureBootDirectoryMenu.Head);
  return EFI_SUCCESS;
}
