/** @file
  AMD implementation of IHV DRTM abstraction layer.

  Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>

typedef enum {
  AmdPageProtectTypeIommuMmio,
  // Do not hardcode any member.
  // Do not add new members below this line.
  AmdPageProtectTypeCount
} AMD_PROTECTED_PAGE_TYPE;

#define IOMMUL2_BUS                   0
#define IOMMUL2_DEVICE                0
#define IOMMUL2_FUNCTION              2

#define IOMMU_VENDOR_ID_OFFSET        0x00
#define IOMMU_VENDOR_ID_VALUE         0x1022
#define IOMMU_SUB_CLASS_OFFSET        0x0A
#define IOMMU_SUB_CLASS_VALUE         0x06
#define IOMMU_BASE_CODE_OFFSET        0x0B
#define IOMMU_BASE_CODE_VALUE         0x08
#define IOMMU_CAP_BASE_LO_OFFSET 0x44
#define IOMMU_CAP_BASE_HI_OFFSET 0x48

/**
  Get the Iommu Mmio base and size

  @param[out] IommuMmioBase
  @param[out] IommuMmioSize

  @return EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetIommuMmio (
  OUT UINT64 *IommuMmioBase,
  OUT UINT32 *IommuMmioSize
  )
{
  UINTN  IommuPciAddr;
  UINT8  PciValue8;
  UINT16 PciValue16;
  UINT32 PciValue32;

  //Check if IOMMU exists
  IommuPciAddr = PCI_LIB_ADDRESS (IOMMUL2_BUS, IOMMUL2_DEVICE, IOMMUL2_FUNCTION, IOMMU_VENDOR_ID_OFFSET);
  PciValue16 = PciRead16 (IommuPciAddr);
  if (PciValue16 != IOMMU_VENDOR_ID_VALUE) {
    return EFI_UNSUPPORTED;
  }

  IommuPciAddr = PCI_LIB_ADDRESS (IOMMUL2_BUS, IOMMUL2_DEVICE, IOMMUL2_FUNCTION, IOMMU_SUB_CLASS_OFFSET);
  PciValue8 = PciRead8 (IommuPciAddr);
  if (PciValue8 != IOMMU_SUB_CLASS_VALUE) {
    return EFI_UNSUPPORTED;
  }

  IommuPciAddr = PCI_LIB_ADDRESS (IOMMUL2_BUS, IOMMUL2_DEVICE, IOMMUL2_FUNCTION, IOMMU_BASE_CODE_OFFSET);
  PciValue8 = PciRead8 (IommuPciAddr);
  if (PciValue8 != IOMMU_BASE_CODE_VALUE) {
    return EFI_UNSUPPORTED;
  }

  //IOMMUL2::IOMMU_CAP_BASE_LO
  IommuPciAddr = PCI_LIB_ADDRESS (IOMMUL2_BUS, IOMMUL2_DEVICE, IOMMUL2_FUNCTION, IOMMU_CAP_BASE_LO_OFFSET);
  PciValue32 = PciRead32 (IommuPciAddr);
  // Check if MMIO enabled, IOMMU_ENABLE
  if ((PciValue32 & BIT0) == 0) {
    return EFI_UNSUPPORTED;
  }
  *IommuMmioBase = PciValue32 & 0xFFF80000;
  //IOMMUL2::IOMMU_CAP_BASE_HI
  IommuPciAddr = PCI_LIB_ADDRESS (IOMMUL2_BUS, IOMMUL2_DEVICE, IOMMUL2_FUNCTION, IOMMU_CAP_BASE_HI_OFFSET);
  PciValue32 = PciRead32 (IommuPciAddr);
  *IommuMmioBase |= ((UINT64) PciValue32 << 32);
  *IommuMmioSize = 0x80000;
  return EFI_SUCCESS;
}

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
  EFI_STATUS  Status;
  UINTN       InternalCount;
  UINT64      IommuMmioBase;
  UINT32      IommuMmioSize;

  if (MemoryDescriptors == NULL || Count == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  *MemoryDescriptors = AllocatePool (AmdPageProtectTypeCount * sizeof (EFI_MEMORY_DESCRIPTOR));
  if (*MemoryDescriptors == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Cleanup;
  }

  // Protect IOMMU MMIO region on shadown copy, this shadow copy will applied once DRTM flag been set.
  // Below code only works for the program which only have one IOMMU
  InternalCount = 0;
  Status = GetIommuMmio (&IommuMmioBase, &IommuMmioSize);
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "%a - IommuMmioBase %x IommuMmioSize %x\n", __FUNCTION__, IommuMmioBase, IommuMmioSize));
    // Align base and size to EFI_PAGE_SIZE boundary
    IommuMmioSize = (UINT32)(IommuMmioBase - ((IommuMmioBase) & ~(EFI_PAGE_MASK))) + IommuMmioSize;
    IommuMmioBase = ((IommuMmioBase) & ~(EFI_PAGE_MASK));
    (*MemoryDescriptors)[InternalCount].PhysicalStart = IommuMmioBase;
    (*MemoryDescriptors)[InternalCount].NumberOfPages = EFI_SIZE_TO_PAGES (IommuMmioSize);
    (*MemoryDescriptors)[InternalCount].Attribute = (EFI_MEMORY_RO | EFI_MEMORY_XP);
  }

  *Count = AmdPageProtectTypeCount;

Cleanup:
  return Status;
}
