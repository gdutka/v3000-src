/** @file


;******************************************************************************
;* Copyright 2023 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>

EFI_STATUS
FdmHashVerify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    FdmHashVerify
  }
};

EFI_STATUS
GetFfsSectionData (
  IN CONST  EFI_GUID              *FileName,
  OUT UINTN                       *BufferSize,
  OUT VOID                        **Buffer
  )
{
  EFI_STATUS            Status;
  UINTN                 Instance;
  EFI_PEI_FV_HANDLE     VolumeHandle;
  EFI_PEI_FILE_HANDLE   FileHandle;
  EFI_FV_FILE_INFO      FileInfo;
  EFI_FFS_FILE_HEADER   *FileHeader;

  *BufferSize = 0;
  *Buffer = NULL;
  Instance = 0;
  while (TRUE) {
    Status = PeiServicesFfsFindNextVolume (Instance++, &VolumeHandle);
    if (EFI_ERROR (Status)) {
      break;
    }
    Status = PeiServicesFfsFindFileByName (FileName, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (Status)) {
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FileHandle, Buffer);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO, "Unable to find Certificate file section ffs %r!\n", Status));
        return Status;
      }
      Status = PeiServicesFfsGetFileInfo (FileHandle, &FileInfo);
      FileHeader = (EFI_FFS_FILE_HEADER*)FileHandle;
      if (IS_FFS_FILE2 (FileHeader)) {
        *BufferSize = FileInfo.BufferSize - sizeof(EFI_COMMON_SECTION_HEADER2);
      } else {
        *BufferSize = FileInfo.BufferSize - sizeof(EFI_COMMON_SECTION_HEADER);
      }
      
      break;
    } else {
      continue;
    }
  }

  return Status;
}

EFI_STATUS
FdmHashVerify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                      Status;
  H2O_FLASH_DEVICE_MAP_HEADER     *FdmHeader;
  UINTN                           FdmBufferSize;
  UINT8                           *FdmBuffer;
  UINT8                           FdmSha256Value[0x20];
  BOOLEAN                         Ok;

  if (GetBootModeHob () == BOOT_IN_RECOVERY_MODE) {
    return EFI_SUCCESS;
  }

  FdmHeader = (H2O_FLASH_DEVICE_MAP_HEADER*)(UINTN) PcdGet64(PcdH2OFlashDeviceMapStart);
  Status = GetFfsSectionData ((EFI_GUID*)PcdGetPtr (PcdFdmHash), &FdmBufferSize, &FdmBuffer);

//[-start-241030-IB20840101-modify]//
  Ok = Sha256HashAll ((VOID*)FdmHeader, (UINTN) PcdGet64(PcdH2OFlashDeviceMapSize), FdmSha256Value);
//[-end-241030-IB20840101-modify]//
  if (Ok == FALSE) {
    DEBUG ((EFI_D_INFO, "Computation failed\n"));
    CpuDeadLoop ();
  }
  if (CompareMem ((VOID*)FdmBuffer, (VOID*)FdmSha256Value, FdmBufferSize) != 0) {
    DEBUG ((EFI_D_INFO, "Fdm Hash Verify failed\n"));
    CpuDeadLoop ();
  }

  return Status;
}

EFI_STATUS
EFIAPI
FdmHashVerifyPeimEntry (
  IN       EFI_PEI_FILE_HANDLE    PeiFileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS                      Status;

  Status = (**PeiServices).NotifyPpi (PeiServices, mNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Error Installing callback %r!\n", Status));
    CpuDeadLoop ();
  }

  return Status;
}
