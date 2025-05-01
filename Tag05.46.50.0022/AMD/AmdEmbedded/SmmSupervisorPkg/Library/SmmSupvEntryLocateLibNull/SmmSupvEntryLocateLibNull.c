/** @file
  Null instance of Supv Entry locate library.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

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
  return EFI_UNSUPPORTED;
}
