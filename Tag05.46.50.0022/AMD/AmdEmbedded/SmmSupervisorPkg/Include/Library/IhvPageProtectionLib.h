/** @file
  IHV abstraction layer to supply pages should be protected by supervisor,
  examples of this usage could cover IOMMU and SecureBio regions.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _IHV_PAGE_PROTECTION_LIB_H_
#define _IHV_PAGE_PROTECTION_LIB_H_

/**
  This routine is to be called by Smm supervisor to obtains an array of
  memory ranges that needs to be marked particularly. This will be called
  by supervisor at ready to lock event.

  @param MemoryDescriptors  Pointer to the callee allocated buffer that contains
                            all ranges to be protected in this method. Caller is
                            responsible for freeing the allocated buffer.
  @param Count              Hold the count of descriptors returned by callee.

  @retval EFI_SUCCESS       The DRTM secure store operation succeeds.
  @retval Error             Some error occurs during DRTM store process.

**/
EFI_STATUS
EFIAPI
IhvGetPagesForProtection (
  OUT  EFI_MEMORY_DESCRIPTOR    **MemoryDescriptors,
  OUT  UINTN                    *Count
  );

#endif
