/*
*******************************************************************************

Supv Entry locate Library that could locate specific sub-entry from given
Supv directory buffer.

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
Copyright (c) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#include <Uefi.h>
#include <AmdPspDirectory.h>
#include <Pi/PiFirmwareFile.h>

#include <Library/SmmSupvEntryLocateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>

/**
  Find the SmmEntry binary in SMMSUPV directory structure

  @param[in]  EntryType  Smm supervisor entry type.
  @param[out] Location   Location of SmmEntry binary Entry.
  @param[out] EntrySize  Size of SmmEntry binary.

  @retval  EFI_SUCCESS            Find smm supervisor successfully.
  @retval  EFI_OUT_OF_RESOURCES   Find smm supervisor successfully.
  @retval  EFI_NOT_FOUND          The smm supervisor could not be found.
  @retval  EFI_DEVICE_ERROR       A hardware error occurs during reading from the
                                  Firmware Volume.
  @retval  EFI_ACCESS_DENIED      The firmware volume containing the searched
                                  Firmware File is configured to disallow reads.
**/
EFI_STATUS
EFIAPI
FindSmmSupvEntry (
  IN  UINT32  EntryType,
  OUT VOID    **Location,
  OUT UINT32  *EntrySize
)
{
  EFI_STATUS    Status;
  INTN          Instance;
  VOID          *Buffer;
  UINTN         Size;
  UINT32        TotalEntries;
  UINT32        i;
  PSP_DIRECTORY *PspDir;
  EFI_GUID      SmmSupvDirFileGuid = SMM_SUPV_DIR_FILE_GUID;

  //
  // Loop all FV to find the FV with SmmSupv directory
  //
  Instance     = 0;
  Buffer       = NULL;
  Size         = 0;
  Status = GetSectionFromAnyFv  (
             &SmmSupvDirFileGuid,
             EFI_SECTION_RAW,
             0,
             &Buffer,
             &Size
             );
  DEBUG ((DEBUG_INFO, "Status: %r Buffer %x Size%x \n", Status, Buffer, Size));
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  PspDir = (PSP_DIRECTORY*) Buffer;
  TotalEntries = PspDir->Header.TotalEntries;

  for (i = 0; i < TotalEntries; i++) {
    if (PspDir->PspEntry[i].Type.Value == EntryType) {
      *Location = AllocatePool (PspDir->PspEntry[i].Size); //temp data, no need to call AllocateSmmSupvPool
      *EntrySize = PspDir->PspEntry[i].Size;
      if (*Location != NULL) {
        CopyMem (*Location, (VOID *) (PspDir->PspEntry[i].Location + (UINT64)Buffer), *EntrySize);
      } else {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      break;
    }
  }

Exit:
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
  return Status;
}
