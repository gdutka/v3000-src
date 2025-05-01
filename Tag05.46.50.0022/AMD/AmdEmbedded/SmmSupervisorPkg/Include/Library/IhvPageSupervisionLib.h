/** @file
  Provides function interface to protect pages supervised by IHV.

Copyright (C) Microsoft Corporation.
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __IHV_PAGE_SUPERVISION_LIB__
#define __IHV_PAGE_SUPERVISION_LIB__

/**
  Given a base Address and an offset, determine if it is within policy to
  allow read access to the region specified.

  @param[in]  BaseAddress       - The address of the protected region.
  @param[in]  Offset            - The offset to the beginning of protected region.

  @retval EFI_ACCESS_DENIED     The requested operation is not allowed by
                                the policy.
          EFI_INVALID_PARAMETER The combination of BaseAddress and Offset does not
                                comply with IHV supplied supervision list.
          EFI_SUCCESS           The requested operation is allowed by the
                                policy.
**/
EFI_STATUS
EFIAPI
IsIhvSupervisedReadAllowed (
  IN UINT64         BaseAddress,
  IN UINT64         Offset
  );

/**
  Read the target offset from given base, in granularity of 4 bytes.

  @param[in]  BaseAddress       - The address of the protected region.
  @param[in]  Offset            - The offset to the beginning of protected region.

  @retval 4-byte data read from target base address at intended offset.
**/
UINT32
EFIAPI
IhvSupervisedRead (
  IN UINT64         BaseAddress,
  IN UINT64         Offset
  );

/**
  Given a base Address and an offset, determine if it is within policy to
  allow write access to the region specified.

  @param[in]  BaseAddress       - The address of the protected region.
  @param[in]  Offset            - The offset to the beginning of protected region.

  @retval EFI_ACCESS_DENIED     The requested operation is not allowed by
                                the policy.
          EFI_INVALID_PARAMETER The combination of BaseAddress and Offset does not
                                comply with IHV supplied supervision list.
          EFI_SUCCESS           The requested operation is allowed by the
                                policy.
**/
EFI_STATUS
EFIAPI
IsIhvSupervisedWriteAllowed (
  IN UINT64         BaseAddress,
  IN UINT64         Offset
  );

/**
  Write the target offset from given base, in granularity of 4 bytes.

  @param[in]  BaseAddress       - The address of the protected region.
  @param[in]  Offset            - The offset to the beginning of protected region.
  @param[in]  Content           - The value to be written to assigned offset.

  @retval EFI_INVALID_PARAMETER The combination of BaseAddress and Offset does not
                                comply with IHV supplied supervision list.
          EFI_SUCCESS           The requested operation is allowed by the
                                policy.
**/
EFI_STATUS
EFIAPI
IhvSupervisedWrite (
  IN UINT64         BaseAddress,
  IN UINT64         Offset,
  IN UINT32         Content
  );

/**
  This routine is to be called by Smm supervisor to obtains an array of
  memory ranges that needs to be marked particularly for supervision. This
  will be called by supervisor at ready to lock event.

  @param MemoryDescriptors  Pointer to the callee allocated buffer that contains
                            all ranges to be protected in this method. Caller is
                            responsible for freeing the allocated buffer.
  @param Count              Hold the count of descriptors returned by callee.

  @retval EFI_SUCCESS       The DRTM secure store operation succeeds.
  @retval Error             Some error occurs during DRTM store process.

**/
EFI_STATUS
EFIAPI
IhvGetPagesForSupervision (
  OUT  EFI_MEMORY_DESCRIPTOR    **MemoryDescriptors,
  OUT  UINTN                    *Count
  );

#endif // !defined (__IHV_PAGE_SUPERVISION_LIB__)
