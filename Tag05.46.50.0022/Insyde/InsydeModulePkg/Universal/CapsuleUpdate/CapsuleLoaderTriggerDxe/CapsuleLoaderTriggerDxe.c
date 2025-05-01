/** @file
  Driver to trigger CapsuleUpdate Flash Loader and Firmware Update

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>
#include <Library/VariableLib.h>
#include <Library/CapsuleLib.h>
#include <Library/CapsuleUpdateCriteriaLib.h>
#include <Library/SeamlessRecoveryLib.h>
#include <Library/H2OCpLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DevicePath.h>
#include <Guid/FileInfo.h>
#include <Guid/GlobalVariable.h>
#include <Guid/EfiSystemResourceTable.h>
#include <Guid/H2OBdsCheckPoint.h>
#include <Guid/DebugMask.h>
#include <StdLib/stdlib.h>
#include <SecureFlash.h>

/**
  Sort the capsules to move WinUx capsule into first order.

  @param[in]      SysRootHandle                  EFI_FILE_HANDLE of system drive.
  @param[in]      TotalCapsules                  Total number of capsules.
  @param[in]      CapsuleImagePath               A pointer to the path of capsule.
  @param[in, out] CapsuleFileInfo                A double pointer to capsule file information.

**/
STATIC
VOID
SortCapsules (
  IN     EFI_FILE_HANDLE         SysRootHandle,
  IN     UINTN                   TotalCapsules,
  IN     UINTN                   LongestCapsuleFileNameSize,
  IN OUT EFI_FILE_INFO           **CapsuleFileInfo
  )
{
  EFI_STATUS                     Status;
  UINTN                          Index;
  EFI_FILE_HANDLE                CapsuleHandle;
  UINTN                          FileSize;
  CHAR16                         *CapsuleImagePath;
  EFI_CAPSULE_HEADER             *Capsule;
  EFI_FILE_INFO                  *TempFileInfo;

  if (TotalCapsules <= 1 || CapsuleFileInfo == NULL) {
    return;
  }
  CapsuleImagePath = AllocatePool (StrSize (EFI_CAPSULE_FILE_PATH) + LongestCapsuleFileNameSize);
  if (CapsuleImagePath == NULL) {
    return;
  }
  for (Index = 0, CapsuleHandle = NULL; Index < TotalCapsules; Index ++) {
    UnicodeSPrint (CapsuleImagePath, StrSize (EFI_CAPSULE_FILE_PATH) + StrSize (CapsuleFileInfo[Index]->FileName), \
      L"%s\\%s", EFI_CAPSULE_FILE_PATH,CapsuleFileInfo[Index]->FileName);
    Status = SysRootHandle->Open (
                              SysRootHandle,
                              &CapsuleHandle,
                              CapsuleImagePath,
                              EFI_FILE_MODE_READ,
                              0
                              );
    if (EFI_ERROR (Status)) {
      break;
    }
    FileSize = (UINTN)CapsuleFileInfo[Index]->FileSize;
    Capsule = AllocatePool (FileSize);
    if (Capsule == NULL) {
      CapsuleHandle->Close (CapsuleHandle);
      break;
    }
    Status = CapsuleHandle->Read (
                              CapsuleHandle,
                              &FileSize,
                              (VOID*) Capsule
                              );
    if (!EFI_ERROR (Status)) {
      if (CompareGuid (&(Capsule->CapsuleGuid), &gWindowsUxCapsuleGuid)) {
        if (Index > 0) {
          TempFileInfo = CapsuleFileInfo[Index];
          CapsuleFileInfo[Index] = CapsuleFileInfo[0];
          CapsuleFileInfo[0] = TempFileInfo;
        }
        CapsuleHandle->Close (CapsuleHandle);
        CapsuleHandle = NULL;
        FreePool (Capsule);
        Capsule = NULL;
        break;
      }
    }
    CapsuleHandle->Close (CapsuleHandle);
    CapsuleHandle = NULL;
    FreePool (Capsule);
    Capsule = NULL;
  }
  FreePool (CapsuleImagePath);
}

/**
  Get the system drive information through the boot loader file path under system partition

  @param[out] SysRootDevicePath    The pointer of system root device path pointer
  @param[out] SysRoorDir           The pointer of EFI_FILE_HANDLE of system drive

  @retval EFI_SUCCESS              The system drive information is correctly get
  @return others                   Unable to get system drive information

**/
EFI_STATUS
EFIAPI
GetSystemRootInfo (
  OUT    EFI_DEVICE_PATH        **SysRootDevicePath,
  OUT    EFI_FILE_HANDLE        *SysRootHandle
  );

/**
  File name comparsion for qsort

  @param[in] FileInfo1          Pointer of first file info for comparsion
  @param[in] FileInfo2          Pointer of second file info for comparsion

**/
STATIC
int
FileNameComparsion (
  IN     CONST VOID*    FileInfo1,
  IN     CONST VOID*    FileInfo2
  )
{
  EFI_FILE_INFO *FileInfoPtr1 = *(EFI_FILE_INFO**)FileInfo1;
  EFI_FILE_INFO *FileInfoPtr2 = *(EFI_FILE_INFO**)FileInfo2;

  return ((int)StrCmp (FileInfoPtr1->FileName, FileInfoPtr2->FileName));
}

/**
  ReadyToBoot notification event handler for capsule image update.

  @param[in] Event              Event whose notification function is being invoked.
  @param[in] Handle             Checkpoint handle.

  @retval    None
**/
VOID
EFIAPI
TriggerCapsuleUpdate (
  IN EFI_EVENT      Event,
  IN H2O_CP_HANDLE  Handle
  )
{
  EFI_STATUS                             Status;
  EFI_DEVICE_PATH_PROTOCOL               *SysRootDevicePath;
  EFI_FILE_HANDLE                        SysRootHandle;
  EFI_FILE_HANDLE                        CapsuleHandle;
  EFI_FILE_HANDLE                        UxCapsuleHandle;
  UINTN                                  Size;
  UINT64                                 OsIndications;
  UINTN                                  Index;
  UINTN                                  FileSize;
  UINTN                                  FileInfoSize;
  UINTN                                  TotalCapsules;
  EFI_CAPSULE_HEADER                     *Capsule;
  UINT8                                  *FileInfo;
  UINT8                                  *FileInfoPtr;
  EFI_FILE_INFO                          **CapsuleFileInfo;
  CHAR16                                 *CapsuleImagePath;
  BOOLEAN                                ResetNeeded;
  BOOLEAN                                IsNameCapsuleUpdateFile;
  UINTN                                  LongestCapsuleFileNameSize;

  H2OCpUnregisterHandler (Handle);

  SysRootHandle           = NULL;
  CapsuleHandle           = NULL;
  UxCapsuleHandle         = NULL;
  FileInfo                = NULL;
  CapsuleFileInfo         = NULL;
  CapsuleImagePath        = NULL;
  ResetNeeded             = TRUE;
  IsNameCapsuleUpdateFile = FALSE;

  //
  // Enumerate whole capsule files under
  //
  Status = GetSystemRootInfo (&SysRootDevicePath, &SysRootHandle);
  if (EFI_ERROR (Status) || SysRootHandle == NULL) {
    goto Exit;
  }
  Status = SysRootHandle->Open (
                            SysRootHandle,
                            &CapsuleHandle,
                            EFI_CAPSULE_FILE_PATH,
                            EFI_FILE_MODE_READ,
                            EFI_FILE_DIRECTORY
                            );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  TotalCapsules = 0;
  FileInfoSize  = 0;
  LongestCapsuleFileNameSize = 0;
  Status = CapsuleHandle->GetInfo (
                            CapsuleHandle,
                            &gEfiFileInfoGuid,
                            &FileInfoSize,
                            NULL
                            );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    //
    // error is expected.  getting size to allocate
    //
    FileInfo = AllocatePool (FileInfoSize);
    if (!FileInfo) {
      goto Exit;
    }
    Status = CapsuleHandle->GetInfo (
                              CapsuleHandle,
                              &gEfiFileInfoGuid,
                              &FileInfoSize,
                              FileInfo
                              );
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
    Size = (UINTN)((EFI_FILE_INFO*)FileInfo)->FileSize;
    FreePool (FileInfo);
    if (Size == 0) {
      goto Exit;
    }
    FileInfo = AllocatePool (Size);
    if (!FileInfo) {
      goto Exit;
    }
    TotalCapsules = 0;
    FileInfoPtr   = FileInfo;
    do {
      FileInfoSize = Size;
      Status = CapsuleHandle->Read (
                                CapsuleHandle,
                                &FileInfoSize,
                                FileInfoPtr
                                );
      if (!EFI_ERROR (Status) && FileInfoSize > 0) {
        if (!(((EFI_FILE_INFO*)FileInfoPtr)->Attribute & EFI_FILE_DIRECTORY) && ((EFI_FILE_INFO*)FileInfoPtr)->FileSize > 0) {
          if (CapsuleFileInfo == NULL) {
            CapsuleFileInfo = AllocatePool (sizeof (EFI_FILE_INFO*) * (UINTN)DivU64x32((UINT64)Size, SIZE_OF_EFI_FILE_INFO));
            if (CapsuleFileInfo == NULL) {
              goto Exit;
            }
          }
          LongestCapsuleFileNameSize = MAX (LongestCapsuleFileNameSize, StrSize (((EFI_FILE_INFO*)FileInfoPtr)->FileName));
          CapsuleFileInfo[TotalCapsules ++] = (EFI_FILE_INFO*)FileInfoPtr;
          FileInfoPtr += FileInfoSize;
          if (StrnCmp (((EFI_FILE_INFO*)FileInfoPtr)->FileName, EFI_CAPSULE_FILE_NAME, sizeof (EFI_CAPSULE_FILE_NAME) / sizeof (CHAR16) - 1) == 0) {
            IsNameCapsuleUpdateFile = TRUE;
          }
        }
      }
    } while (FileInfoSize != 0);
    CapsuleHandle->Close (CapsuleHandle);
    CapsuleHandle = NULL;
  }
  if (!FileInfo) {
    goto Exit;
  }
  //
  // Count total capsule images according to file info record
  //
  if (TotalCapsules == 0) {
    //
    // No any capsule file in the EFI_CAPSULE_FILE_PATH directory
    //
    goto Exit;
  }
  //
  // Sort the capsule file list
  //
  if (TotalCapsules > 1) {
    if (IsNameCapsuleUpdateFile) {
      qsort (CapsuleFileInfo, TotalCapsules, sizeof (EFI_FILE_INFO*), FileNameComparsion);
    } else {
      SortCapsules (SysRootHandle, TotalCapsules, LongestCapsuleFileNameSize, CapsuleFileInfo);
    }
  }
  CapsuleImagePath = AllocatePool (StrSize (EFI_CAPSULE_FILE_PATH) + LongestCapsuleFileNameSize);
  if (CapsuleImagePath == NULL) {
    goto Exit;
  }
  //
  // Loop for capsule update processing
  //
  for (Index = 0, CapsuleHandle = NULL; Index < TotalCapsules; Index ++) {
    UnicodeSPrint (CapsuleImagePath, StrSize(EFI_CAPSULE_FILE_PATH) + StrSize(CapsuleFileInfo[Index]->FileName), \
      L"%s\\%s", EFI_CAPSULE_FILE_PATH, CapsuleFileInfo[Index]->FileName);
    Status = SysRootHandle->Open (
                              SysRootHandle,
                              &CapsuleHandle,
                              CapsuleImagePath,
                              EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ,
                              EFI_FILE_ARCHIVE
                              );
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
    FileSize = (UINTN)CapsuleFileInfo[Index]->FileSize;
    Capsule = AllocatePool (FileSize);
    if (Capsule == NULL) {
      goto Exit;
    }
    Status = CapsuleHandle->Read (
                              CapsuleHandle,
                              &FileSize,
                              (VOID*)Capsule
                              );
    if (!EFI_ERROR (Status)) {
      //
      // Clear the Flags in capsule header as no flags allowed in file capsule according to UEFI 2.4B spec 7.5.5
      //
      Capsule->Flags = 0;
      //
      // Process capsule image
      //
      Status = ProcessCapsuleImage (Capsule);
      if (CompareGuid (&Capsule->CapsuleGuid, &gWindowsUxCapsuleGuid)) {
        //
        // Delete UX file after all capsules processed
        //
        UxCapsuleHandle = CapsuleHandle;
      } else {
        //
        // Delete the file after process capsule
        //
        CapsuleHandle->Delete (CapsuleHandle);
        //
        // Perform system reset to make the new firmware take effect before processing the next capsule.
        // No need to reset system after processing UX capsule; for the rest, reset system whether processing capsule was success or not.
        //
        if (Index + 1 < TotalCapsules) {
          gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
        }
      }
    } else {
      //
      // Close the file when read failed
      //
      CapsuleHandle->Close (CapsuleHandle);
    }
    FreePool (Capsule);
    CapsuleHandle = NULL;
  }
Exit:
  //
  // Erase the EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED in
  // OsIndications variable after capsules processed.
  //
  Size          = sizeof(UINT64);
  OsIndications = 0;
  Status = CommonGetVariable (
             L"OsIndications",
             &gEfiGlobalVariableGuid,
             &Size,
             &OsIndications
             );
  if (!EFI_ERROR (Status)) {
    OsIndications &= ~(UINTN)(EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED);
    Status = CommonSetVariable (
               L"OsIndications",
               &gEfiGlobalVariableGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
               sizeof(UINT64),
               &OsIndications
               );
  }
  if (UxCapsuleHandle != NULL) {
    UxCapsuleHandle->Delete (UxCapsuleHandle);
  }
  if (!IsFirmwareUpdateResiliencyEnabled ()) {
    ClearFirmwareUpdateProgress ();
  }
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }
  if (CapsuleFileInfo != NULL) {
    FreePool (CapsuleFileInfo);
  }
  if (CapsuleImagePath != NULL) {
    FreePool (CapsuleImagePath);
  }
  if (SysRootHandle != NULL) {
    SysRootHandle->Close (SysRootHandle);
  }
  if (CapsuleHandle != NULL) {
    CapsuleHandle->Close (CapsuleHandle);
  }
  if (ResetNeeded) {
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }
}

/**
  CapsuleLoaderTrigger DXE driver entry point


  @param ImageHandle     A handle for the image that is initializing this driver
  @param SystemTable     A pointer to the EFI system table

  @retval EFI_SUCCESS:   Module initialized successfully
**/
EFI_STATUS
EFIAPI
CapsuleLoaderTriggerEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;
  UINTN                Size;
  UINT64               OsIndications;
  H2O_CP_HANDLE        CpHandle;
  //
  // Inspect the OsIndications variable to startup file capsule process
  //
  Size          = sizeof(UINT64);
  OsIndications = 0;
  Status = CommonGetVariable (
             L"OsIndications",
             &gEfiGlobalVariableGuid,
             &Size,
             &OsIndications
             );
  if (!EFI_ERROR (Status) && (OsIndications & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED)) {
    if (FeaturePcdGet (PcdH2OBdsCpReadyToBootBeforeSupported)) {
      //
      // Setup the capsule update procedure right before ready to boot event
      // Ues of TPL_CALLBACK-1 to make the USB hot-plug can work properly as it
      // is using TPL_CALLBACK level
      //
      Status = H2OCpRegisterHandler (
                 &gH2OBdsCpReadyToBootBeforeGuid,
                 TriggerCapsuleUpdate,
                 H2O_CP_MEDIUM_LOW,
                 &CpHandle
                 );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpReadyToBootBeforeGuid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpReadyToBootBeforeGuid, Status));
    }
    if (FeaturePcdGet (PcdH2OBiosUpdateFaultToleranceEnabled)) {
      //
      // Capsules are required to be performed before end of dxe.
      //
      if (FeaturePcdGet (PcdH2OBdsCpEndOfDxeBeforeSupported)) {
        Status = H2OCpRegisterHandler (
                   &gH2OBdsCpEndOfDxeBeforeGuid,
                   TriggerCapsuleUpdate,
                   H2O_CP_MEDIUM_LOW,
                   &CpHandle
                   );
        if (EFI_ERROR (Status)) {
          DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpEndOfDxeBeforeGuid, Status));
          return Status;
        }
        DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpEndOfDxeBeforeGuid, Status));
      }
    }
  }
  return EFI_SUCCESS;
}