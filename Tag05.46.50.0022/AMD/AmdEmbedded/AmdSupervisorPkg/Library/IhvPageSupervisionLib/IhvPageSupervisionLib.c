/** @file
  Instance of AMD DRTM page supervision layer. This library provides
  supervision over pages covering PSP MMIO, etc.

  Copyright (c) Microsoft Corporation.
  Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmSupvPspMmioLib.h>

typedef enum {
  AmdPageSuperviseTypePspMmio,
  // Do not hardcode any member.
  // Do not add new members below this line.
  AmdPageSuperviseTypeCount
} AMD_SUPERVISED_PAGE_TYPE;

/*
  Helper function to parse supervision type based on supplied base address
*/
EFI_STATUS
ParseSupervisionType (
  IN  UINT64                      BaseAddress,
  OUT AMD_SUPERVISED_PAGE_TYPE    *Type
  )
{
  EFI_STATUS  Status;
  UINT64      PspMmioBase;
  UINT32      DrtmOffset;
  UINT32      DrtmSize;

  Status = GetDrtmMmio (&PspMmioBase, &DrtmOffset, &DrtmSize);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  else if (BaseAddress == PspMmioBase) {
    *Type = AmdPageSuperviseTypePspMmio;
    Status = EFI_SUCCESS;
    goto Exit;
  }

  Status = EFI_NOT_FOUND;

Exit:
  return Status;
}

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
  )
{
  EFI_STATUS Status;
  AMD_SUPERVISED_PAGE_TYPE Type;

  Status = ParseSupervisionType (BaseAddress, &Type);
  DEBUG ((DEBUG_VERBOSE, "%a  Type %x\n",__FUNCTION__, Type));
  switch (Type) {
    case AmdPageSuperviseTypePspMmio:
      if (Offset > 0xFFFFFFFFul) {
        Status = EFI_INVALID_PARAMETER;
        DEBUG ((DEBUG_ERROR, "%a PspMmioRead invalid offset \n", __FUNCTION__));
        break;
      }
      Status = IsPspMmioReadAllowed ((UINT32) Offset);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a PspMmioRead blocked by Level30 mandatory policy - %r\n", __FUNCTION__, Status));
      }
      break;
    default:
      ASSERT (FALSE);
      Status = EFI_INVALID_PARAMETER;
      break;
  }

  return Status;
}

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
  )
{
  EFI_STATUS Status;
  AMD_SUPERVISED_PAGE_TYPE Type;
  UINT32 Ret = 0;

  Status = ParseSupervisionType (BaseAddress, &Type);

  switch (Type) {
    case AmdPageSuperviseTypePspMmio:
      Ret = SmmSupvPspMmioRead ((UINT32) Offset);
      break;
    default:
      ASSERT (FALSE);
      Status = EFI_INVALID_PARAMETER;
      break;
  }

  ASSERT_EFI_ERROR (Status);
  return Ret;
}

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
  )
{
  EFI_STATUS Status;
  AMD_SUPERVISED_PAGE_TYPE Type;

  Status = ParseSupervisionType (BaseAddress, &Type);

  switch (Type) {
    case AmdPageSuperviseTypePspMmio:
      if (Offset > 0xFFFFFFFFul) {
        Status = EFI_INVALID_PARAMETER;
        DEBUG ((DEBUG_ERROR, "%a PspMmioWrite invalid offset \n", __FUNCTION__));
        break;
      }
      Status = IsPspMmioWriteAllowed ((UINT32) Offset);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a PspMmioWrite blocked by Level30 mandatory policy - %r\n", __FUNCTION__, Status));
        break;
      }
      break;
    default:
      ASSERT (FALSE);
      Status = EFI_INVALID_PARAMETER;
      break;
  }

  return Status;
}

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
  )
{
  EFI_STATUS Status;
  AMD_SUPERVISED_PAGE_TYPE Type;

  Status = ParseSupervisionType (BaseAddress, &Type);

  switch (Type) {
    case AmdPageSuperviseTypePspMmio:
      SmmSupvPspMmioWrite ((UINT32) Offset, Content);
      break;
    default:
      ASSERT (FALSE);
      Status = EFI_INVALID_PARAMETER;
      break;
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status;
  UINTN       InternalCount;
  UINT64      PspMmioBase;
  UINT32      DrtmOffset;
  UINT32      DrtmSize;
  UINT64      AlignedBase;
  UINT64      AlignedSize;

  if (MemoryDescriptors == NULL || Count == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  *MemoryDescriptors = AllocatePool (AmdPageSuperviseTypeCount * sizeof (EFI_MEMORY_DESCRIPTOR));
  if (*MemoryDescriptors == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Cleanup;
  }

  // Protect IOMMU MMIO region on shadown copy, this shadow copy will applied once DRTM flag been set.
  // Below code only works for the program which only have one IOMMU
  InternalCount = 0;
  Status = GetDrtmMmio (&PspMmioBase, &DrtmOffset, &DrtmSize);
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "PspMmioBase %x DrtmOffset %x DrtmMmioSize %x\n", PspMmioBase, DrtmOffset, DrtmSize));
    // Align base and size to EFI_PAGE_SIZE boundary
    AlignedBase = ((PspMmioBase + DrtmOffset) & ~(EFI_PAGE_MASK));
    AlignedSize = (UINT32)(PspMmioBase + DrtmOffset - AlignedBase) + DrtmSize;
    (*MemoryDescriptors)[InternalCount].PhysicalStart = AlignedBase;
    (*MemoryDescriptors)[InternalCount].NumberOfPages = EFI_SIZE_TO_PAGES (AlignedSize);
    (*MemoryDescriptors)[InternalCount].Attribute = (EFI_MEMORY_XP | EFI_MEMORY_SP);
  }

  *Count = AmdPageSuperviseTypeCount;

Cleanup:
  return Status;
}
