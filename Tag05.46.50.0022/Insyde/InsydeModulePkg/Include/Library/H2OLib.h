/** @file
  This library provides mode-agnostic utility functions that have
  access to core H2O resources.
;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_LIB_H_
#define _H2O_LIB_H_

#include <Uefi.h>

/**
  This function returns the currently selected boot type.

  NOTE: If PcdH2OCsmSupported is set to FALSE, then this will always return EFI_BOOT_TYPE.

  @return         Enumerated value that indicates the current boot type. Valid values are:
                    EFI_BOOT_TYPE    - UEFI Boot
                    LEGACY_BOOT_TYPE - Legacy Boot
                    DUAL_BOOT_TYPE   - Either UEFI or Legacy.
**/
UINT8
H2OGetBootType  (
  VOID
  );


/**
  This function frees a pool of memory if the address is non-NULL and then resets
  the address to NULL so that future calls will do nothing.

  @param[in out]    Buffer              On entry, optional pointer to the
                                        address of the buffer to free or NULL.
                                        On exit, optional pointer to NULL.

  @return           None
**/
VOID
H2OFreePool (
  IN OUT VOID **Buffer OPTIONAL
  );



/**
  This function locates the first instance of a protocol/PPI. The PEI version
  returns PPIs. The DXE version returns DXE protocols. The DXE/SMM version
  returns DXE protocols outside of SMM and SMM protocols inside of SMM. The
  SMM version returns SMM protocols.

  @param[in]    Guid                    Ptr to GUID that specifies the protocol
                                        or PPI.
  @param[out]   Instance                Ptr to returned protocol interface
                                        structure pointer or NULL if an error.

  @returns      Boolean that indicates whether an instance of the specified
                protocol was found (TRUE) or not (FALSE).
**/
BOOLEAN
H2OLocate (
  IN  CONST EFI_GUID        *Guid,
  OUT       VOID            **Instance
  );


/**
  Allocates a buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAllocate (
  IN UINTN  AllocationSize
  );


/**
  Allocates and zeros a buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAllocateZero (
  IN UINTN  AllocationSize
  );


/**
  Copies a buffer to an allocated buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.

  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAllocateCopy (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  );


/**
  Allocates a buffer of type EfiRuntimeServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData and returns
  a pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2ORuntimeAllocate (
  IN UINTN  AllocationSize
  );


/**
  Allocates and zeros a buffer of type EfiRuntimeServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2ORuntimeAllocateZero (
  IN UINTN  AllocationSize
  );


/**
  Copies a buffer to an allocated buffer of type EfiRuntimeServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.

  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2ORuntimeAllocateCopy (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  );



/**
  Allocates a buffer of type EfiReservedMemoryType.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType and returns
  a pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OReservedAllocate (
  IN UINTN  AllocationSize
  );



/**
  Allocates and zeros a buffer of type EfiReservedMemoryType.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OReservedAllocateZero (
  IN UINTN  AllocationSize
  );


/**
  Copies a buffer to an allocated buffer of type EfiReservedMemoryType.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.

  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OReservedAllocateCopy (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  );



/**
  Allocates a buffer of type EfiACPIMemoryNVS.

  Allocates the number bytes specified by AllocationSize of type EfiACPIMemoryNVS and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAcpiNvsAllocate (
  IN UINTN  AllocationSize
  );


/**
  Allocates and zeros a buffer of type EfiACPIMemoryNVS.

  Allocates the number bytes specified by AllocationSize of type EfiACPIMemoryNVS, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAcpiNvsAllocateZero (
  IN UINTN  AllocationSize
  );


/**
  Copies a buffer to an allocated buffer of type EfiACPIMemoryNVS.

  Allocates the number bytes specified by AllocationSize of type EfiACPIMemoryNVS, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.

  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAcpiNvsAllocateCopy (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  );


/**
  Allocates a buffer of type EfiACPIReclaimMemory.

  Allocates the number bytes specified by AllocationSize of type EfiACPIReclaimMemory and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAcpiReclaimAllocate (
  IN UINTN  AllocationSize
  );


/**
  Allocates and zeros a buffer of type EfiACPIReclaimMemory.

  Allocates the number bytes specified by AllocationSize of type EfiACPIReclaimMemory, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAcpiReclaimAllocateZero (
  IN UINTN  AllocationSize
  );


/**
  Copies a buffer to an allocated buffer of type EfiACPIReclaimMemory.

  Allocates the number bytes specified by AllocationSize of type EfiACPIReclaimMemory, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.

  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAcpiReclaimAllocateCopy (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  );


#endif

