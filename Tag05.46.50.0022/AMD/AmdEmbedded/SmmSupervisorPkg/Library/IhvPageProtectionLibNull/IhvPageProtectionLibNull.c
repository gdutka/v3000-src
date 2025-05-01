/** @file
  Null instance of IHV DRTM abstraction layer.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

/**
  This routine is to be called by Smm supervisor to obtains an array of
  memory ranges that needs to be marked particularly. This will be called
  by supervisor at ready to lock event.

  @param MemoryDescriptors  Pointer to the callee allocated buffer that contains
                            all ranges to be protected in this method. Caller is
                            responsible for freeing the allocated buffer.
  @param Count              The count of descriptors returned by callee.

  @retval EFI_SUCCESS       The DRTM secure store operation succeeds.
  @retval Error             Some error occurs during DRTM store process.

**/
EFI_STATUS
EFIAPI
IhvGetPagesForProtection (
  OUT  EFI_MEMORY_DESCRIPTOR    **MemoryDescriptors,
  OUT  UINTN                    *Count
  )
{
  return EFI_UNSUPPORTED;
}
