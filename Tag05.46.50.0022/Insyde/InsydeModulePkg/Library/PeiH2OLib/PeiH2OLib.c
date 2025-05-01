/** @file
  PEI H2O library. This library provides PEI utility functions that have
  access to core H2O resources.

;******************************************************************************
;* Copyright (c) 2019 - 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <Uefi.h>
#include <KernelSetupConfig.h>

#include <Library/H2OLib.h>
#include <Library/PcdLib.h>
#include <Library/KernelConfigLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>



/**
  This function returns the currently selected boot type.

  NOTE: If PcdH2OCsmSupported is set to FALSE, then this will always return EFI_BOOT_TYPE.

  @return         Enumerated value that indicates the current boot type. Valid values are:
                    EFI_BOOT_TYPE    - UEFI Boot
                    LEGACY_BOOT_TYPE - Legacy Boot
                    DUAL_BOOT_TYPE   - Either UEFI or Legacy.
**/
UINT8
H2OGetBootType (
  VOID
  )
{
  UINT8                  BootType;
  EFI_STATUS             Status;
  KERNEL_CONFIGURATION   KernelConfig;

  if (!PcdGetBool (PcdH2OCsmSupported)) {
    return EFI_BOOT_TYPE;
  }

  BootType = EFI_BOOT_TYPE;
  Status = GetKernelConfiguration (&KernelConfig);
  if (!EFI_ERROR (Status)) {
    BootType = KernelConfig.BootType;
  }
  return BootType;
}

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
  )
{
  if (Buffer != NULL && *Buffer != NULL) {
    FreePool (*Buffer);
    *Buffer = NULL;
  }
}


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
  )
{
  CONST EFI_PEI_SERVICES      **PeiServices;
  
  if (Guid == NULL || Instance == NULL) {
    return FALSE;
  }

  PeiServices = GetPeiServicesTablePointer ();
  if (PeiServices == NULL) {
    return FALSE;
  }
  
  *Instance = NULL;
  return (BOOLEAN)(!EFI_ERROR ((*PeiServices)->LocatePpi (PeiServices, Guid, 0, NULL, Instance)));
}


/**
  Allocates one or more 4KB pages of a certain memory type.

  Allocates the number of 4KB pages of a certain memory type and returns a pointer to the allocated
  buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL is returned.
  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  MemoryType            The type of memory to allocate.
  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
H2OInternalAllocatePages (
  IN EFI_MEMORY_TYPE  MemoryType,
  IN UINTN            Pages
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Memory;

  if (Pages == 0) {
    return NULL;
  }

  Status = PeiServicesAllocatePages (MemoryType, Pages, &Memory);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return (VOID *) (UINTN) Memory;
}


/**
  Allocates a buffer of a certain pool type.

  Allocates the number bytes specified by AllocationSize of a certain pool type and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  MemoryType            The type of memory to allocate.
  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
H2OInternalAllocatePool (
  IN EFI_MEMORY_TYPE  MemoryType,
  IN UINTN            AllocationSize
  )
{
  //
  // If we need lots of small runtime/reserved memory type from PEI in the future,
  // we can consider providing a more complex algorithm that allocates runtime pages and
  // provide pool allocations from those pages.
  //
  return H2OInternalAllocatePages (MemoryType, EFI_SIZE_TO_PAGES (AllocationSize));
}


/**
  Allocates and zeros a buffer of a certain pool type.

  Allocates the number bytes specified by AllocationSize of a certain pool type, clears the buffer
  with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a valid
  buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the request,
  then NULL is returned.

  @param  PoolType              The type of memory to allocate.
  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
H2OInternalAllocateZeroPool (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            AllocationSize
  )
{
  VOID  *Memory;

  Memory = H2OInternalAllocatePool (PoolType, AllocationSize);
  if (Memory != NULL) {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}


/**
  Copies a buffer to an allocated buffer of a certain pool type.

  Allocates the number bytes specified by AllocationSize of a certain pool type, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param  PoolType              The type of pool to allocate.
  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
H2OInternalAllocateCopyPool (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            AllocationSize,
  IN CONST VOID       *Buffer
  )
{
  VOID  *Memory;

  ASSERT (Buffer != NULL);
  ASSERT (AllocationSize <= (MAX_ADDRESS - (UINTN) Buffer + 1));

  Memory = H2OInternalAllocatePool (PoolType, AllocationSize);
  if (Memory != NULL) {
     Memory = CopyMem (Memory, Buffer, AllocationSize);
  }
  return Memory;
}


/**
  Allocates a buffer by using PEI service.

  Allocates the number bytes specified by AllocationSize and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
H2OAllocate (
  IN UINTN  AllocationSize
  )
{
  EFI_STATUS        Status;
  VOID              *Buffer;

  Status = PeiServicesAllocatePool (AllocationSize, &Buffer);
  if (EFI_ERROR (Status)) {
    Buffer = NULL;
  }
  return Buffer;
}

/**
  Allocates and zeros a buffer by using PEI service.

  Allocates the number bytes specified by AllocationSize, clears the
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
  )
{
  VOID  *Memory;

  Memory = H2OAllocate (AllocationSize);
  if (Memory != NULL) {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}

/**
  Copies a buffer to an allocated buffer by using PEI service.

  Allocates the number bytes specified by AllocationSize, copies
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
  )
{
  VOID  *Memory;

  ASSERT (Buffer != NULL);
  ASSERT (AllocationSize <= (MAX_ADDRESS - (UINTN) Buffer + 1));

  Memory = H2OAllocate (AllocationSize);
  if (Memory != NULL) {
     Memory = CopyMem (Memory, Buffer, AllocationSize);
  }
  return Memory;
}



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
  )
{
  return H2OInternalAllocatePool (EfiRuntimeServicesData, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateZeroPool (EfiRuntimeServicesData, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateCopyPool (EfiRuntimeServicesData, AllocationSize, Buffer);
}


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
  )
{
  return H2OInternalAllocatePool (EfiReservedMemoryType, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateZeroPool (EfiReservedMemoryType, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateCopyPool (EfiReservedMemoryType, AllocationSize, Buffer);
}


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
  )
{
  return H2OInternalAllocatePool (EfiACPIMemoryNVS, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateZeroPool (EfiACPIMemoryNVS, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateCopyPool (EfiACPIMemoryNVS, AllocationSize, Buffer);
}


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
  )
{
  return H2OInternalAllocatePool (EfiACPIReclaimMemory, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateZeroPool (EfiACPIReclaimMemory, AllocationSize);
}


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
  )
{
  return H2OInternalAllocateCopyPool (EfiACPIReclaimMemory, AllocationSize, Buffer);
}