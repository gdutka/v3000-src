/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "AmdNbioIOMMU.h"

#define FILECODE        NBIO_LIBRARY_IOMMUDMARLIB_PEI_IOMMUPPI_FILECODE

extern EDKII_IOMMU_PPI mAmdIommuServicesPpi;

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdIommuServicesPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiIoMmuPpiGuid,
  &mAmdIommuServicesPpi
};

LIST_ENTRY                        gSavedMaps = INITIALIZE_LIST_HEAD_VARIABLE(gSavedMaps);

/**
  Set IOMMU attribute for a system memory.

  If the IOMMU PPI exists, the system memory cannot be used
  for DMA by default.

  When a device requests a DMA access for a system memory,
  the device driver need use SetAttribute() to update the IOMMU
  attribute to request DMA access (read and/or write).

  @param[in]  This              The PPI instance pointer.
  @param[in]  Mapping           The mapping value returned from Map().
  @param[in]  IoMmuAccess       The IOMMU access.

  @retval EFI_SUCCESS            The IoMmuAccess is set for the memory range specified by DeviceAddress and Length.
  @retval EFI_INVALID_PARAMETER  Mapping is not a value that was returned by Map().
  @retval EFI_INVALID_PARAMETER  IoMmuAccess specified an illegal combination of access.
  @retval EFI_UNSUPPORTED        The bit mask of IoMmuAccess is not supported by the IOMMU.
  @retval EFI_UNSUPPORTED        The IOMMU does not support the memory range specified by Mapping.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to modify the IOMMU access.
  @retval EFI_DEVICE_ERROR       The IOMMU device reported an error while attempting the operation.
  @retval EFI_NOT_AVAILABLE_YET  DMA protection has been enabled, but DMA buffer are
                                 not available to be allocated yet.

**/
EFI_STATUS
EFIAPI
IommuSetAttribute (
  IN EDKII_IOMMU_PPI       *This,
  IN VOID                  *Mapping,
  IN UINT64                IoMmuAccess
  )
{
  BOOLEAN            SetAttributesStatus;
  BOOLEAN            ReadPerms;
  BOOLEAN            WritePerms;
  AMD_IOMM_MAP_INFO  *MapInfo;

  // Input parameter check
  ASSERT(Mapping != NULL);
  if (Mapping == NULL) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Called with NULL mapping info. Cannot setup IOMMU mapping for DMA. \n", __FUNCTION__);
    return EFI_INVALID_PARAMETER;
  }
  MapInfo = (AMD_IOMM_MAP_INFO *)Mapping;
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with 0x%x address and iommu access bits %d\n",
                  __FUNCTION__, (UINT32)MapInfo->MappedDeviceAddress, IoMmuAccess);

  ReadPerms = FALSE;
  WritePerms = FALSE;

  if ((IoMmuAccess & EDKII_IOMMU_ACCESS_READ) > 0) {
    ReadPerms =  TRUE;
  }
  if ((IoMmuAccess & EDKII_IOMMU_ACCESS_WRITE) > 0) {
    WritePerms =  TRUE;
  }

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Attribute Device address (4k) 0x%x through 0x%x  \n",
                   __FUNCTION__,
                   MapInfo->MappedDeviceAddress,
                   MapInfo->MappedDeviceAddress + (MapInfo->NumberOfPages << 12)
                   );
  // This is where the real work occurs
  SetAttributesStatus = SetAttributesOrFreeDeviceAddress (
                          (UINT32)MapInfo->MappedDeviceAddress,
                          MapInfo->NumberOfPages,
                          TRUE,
                          ReadPerms,
                          WritePerms
                          );
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Set attribute operation result %d\n", __FUNCTION__, SetAttributesStatus);

  return EFI_SUCCESS;
}

/**
  Provides the controller-specific addresses required to access system memory from a
  DMA bus master.

  @param  This                  The PPI instance pointer.
  @param  Operation             Indicates if the bus master is going to read or write to system memory.
  @param  HostAddress           The system memory address to map to the PCI controller.
  @param  NumberOfBytes         On input the number of bytes to map. On output the number of bytes
                                that were mapped.
  @param  DeviceAddress         The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param  Mapping               A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           The range was mapped for the returned NumberOfBytes.
  @retval EFI_UNSUPPORTED       The HostAddress cannot be mapped as a common buffer.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_DEVICE_ERROR      The system hardware could not map the requested address.
  @retval EFI_NOT_AVAILABLE_YET DMA protection has been enabled, but DMA buffer are
                                not available to be allocated yet.

**/
EFI_STATUS
EFIAPI
IommuMap (
  IN     EDKII_IOMMU_PPI                            *This,
  IN     EDKII_IOMMU_OPERATION                      Operation,
  IN     VOID                                       *HostAddress,
  IN OUT UINTN                                      *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS                       *DeviceAddress,
  OUT    VOID                                       **Mapping
  )
{
  EFI_PHYSICAL_ADDRESS                              PhysicalAddress;
  AMD_IOMM_MAP_INFO                                 *MapInfo;
  BOOLEAN                                           Realign;
  UINTN                                             ExtraPage;

  Realign = FALSE;
  ExtraPage = 0;

  // Input parameter check
  if (NumberOfBytes == NULL || DeviceAddress == NULL || Mapping == NULL) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with %d operation, 0x%x host address, 0x%x number of bytes\n",
                   __FUNCTION__, Operation, HostAddress, *NumberOfBytes);

  //
  // Make sure that Operation is valid
  //
  if ((UINT32) Operation >= EdkiiIoMmuOperationMaximum) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Test the handle information passed in and extract it if good, otherwise return an error
  //
  PhysicalAddress = (EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress;
  //
  // Alignment check
  //
  if ((*NumberOfBytes != ALIGN_VALUE(*NumberOfBytes, SIZE_4KB)) || (PhysicalAddress != ALIGN_VALUE(PhysicalAddress, SIZE_4KB))) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : WARNING value is not aligned.\n", __FUNCTION__);
    Realign = TRUE;
    // If base value is not aligned and size makes it overflow to span 2 pages
    if (EFI_SIZE_TO_PAGES(*NumberOfBytes) < EFI_SIZE_TO_PAGES((UINTN)((PhysicalAddress & ALIGNMENT_4K_REMAINDER_MASK) + *NumberOfBytes))) {
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : WARNING value requested plus size is not page aligned and result will span 2 pages.\n", __FUNCTION__);
      ExtraPage = 1;
    }
  }

  //
  // Allocate a AMD_IOMM_MAP_INFO structure to remember the mapping when Unmap() is
  // called later.
  //
  MapInfo = AllocateZeroPool (sizeof (AMD_IOMM_MAP_INFO));
  if (MapInfo == NULL) {
    *NumberOfBytes = 0;
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  // TODO : do we want some sort of check here to make sure that the memory region being requested for mapping
  // is not critical. Rooting the box may be trivial if you can map any address without limit.

  //
  // Initialize the AMD_IOMM_MAP_INFO structure
  //
  MapInfo->Signature         = AMD_IOMMU_MAP_INFO_SIGNATURE;
  MapInfo->Operation         = Operation;
  MapInfo->NumberOfBytes     = *NumberOfBytes;
  MapInfo->NumberOfPages     = EFI_SIZE_TO_PAGES (MapInfo->NumberOfBytes) + ExtraPage;
  MapInfo->HostAddress       = PhysicalAddress;

  // Assign virtual device address to access the same memory pointed to by the physical address
  MapInfo->MappedDeviceAddress = (EFI_PHYSICAL_ADDRESS) AssignDeviceAddress ((VOID *)PhysicalAddress, MapInfo->NumberOfPages);

  ASSERT(MapInfo->MappedDeviceAddress != NULL);

  InsertTailList (&gSavedMaps, &MapInfo->Link);

  // If value passed in was misaligned then we need adjust for the offset
  if (!Realign) {
    *DeviceAddress = MapInfo->MappedDeviceAddress;
  }
  else {
    *DeviceAddress = MapInfo->MappedDeviceAddress | (EFI_PHYSICAL_ADDRESS)(ALIGNMENT_4K_REMAINDER_MASK & (UINT32)PhysicalAddress);
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : Adjusted device address for unaligned input host address : 0x%x \n", __FUNCTION__, *DeviceAddress);
  }
  // Use this structure to keep track of the mapping
  *Mapping       = MapInfo;

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Map Device address (4k) 0x%x through 0x%x and mapping 0x%x \n",
                   __FUNCTION__,
                   MapInfo->MappedDeviceAddress,
                   MapInfo->MappedDeviceAddress + (MapInfo->NumberOfPages << 12),
                   *Mapping
                   );

  DumpEventLog();

  return EFI_SUCCESS;
}

/**
  Completes the Map() operation and releases any corresponding resources.

  @param  This                  The PPI instance pointer.
  @param  Mapping               The mapping value returned from Map().

  @retval EFI_SUCCESS           The range was unmapped.
  @retval EFI_INVALID_PARAMETER Mapping is not a value that was returned by Map().
  @retval EFI_DEVICE_ERROR      The data was not committed to the target system memory.
  @retval EFI_NOT_AVAILABLE_YET DMA protection has been enabled, but DMA buffer are
                                not available to be allocated yet.

**/
EFI_STATUS
EFIAPI
IommuUnmap (
  IN  EDKII_IOMMU_PPI                          *This,
  IN  VOID                                     *Mapping
  )
{
  AMD_IOMM_MAP_INFO        *MapInfo;
  LIST_ENTRY               *Link;
  BOOLEAN                  DeviceUnmapStatus;

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with 0x%x mapping\n", __FUNCTION__, Mapping);
  DumpEventLog();
  // Input parameter check
  if (Mapping == NULL) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a: %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Verify that this node is actually in the list
  //
  MapInfo = NULL;
  for (Link = GetFirstNode (&gSavedMaps); !IsNull (&gSavedMaps, Link); Link = GetNextNode (&gSavedMaps, Link)) {
    MapInfo = AMD_IOMM_MAP_INFO_FROM_LINK (Link);
    if (MapInfo == Mapping) {
      break;
    }
  }

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Unmap Device address (4k) 0x%x through 0x%x \n",
                   __FUNCTION__,
                   MapInfo->MappedDeviceAddress,
                   MapInfo->MappedDeviceAddress + (MapInfo->NumberOfPages << 12)
                   );

  // Now call this function to erase the IOMMU mapping
  DeviceUnmapStatus = SetAttributesOrFreeDeviceAddress (
                        (UINT32)MapInfo->MappedDeviceAddress,
                        MapInfo->NumberOfPages,
                        FALSE,
                        FALSE,
                        FALSE
                        );
  if (!DeviceUnmapStatus) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Unmap unsuccessful \n", __FUNCTION__);
  }
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Unmap success! Now try to free mapping entry in memory \n", __FUNCTION__);
  //
  // Mapping is not a valid value returned by Map()
  //
  if (MapInfo != Mapping) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a: Map info does not equal mapping %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }
  RemoveEntryList (&MapInfo->Link);

  FreePool (Mapping);

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Exit \n", __FUNCTION__);
  return EFI_SUCCESS;
}

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param  This                  The PPI instance pointer.
  @param  MemoryType            The type of memory to allocate, EfiBootServicesData or
                                EfiRuntimeServicesData.
  @param  Pages                 The number of pages to allocate.
  @param  HostAddress           A pointer to store the base system memory address of the
                                allocated range.
  @param  Attributes            The requested bit mask of attributes for the allocated range.

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE, MEMORY_CACHED and DUAL_ADDRESS_CYCLE.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.
  @retval EFI_NOT_AVAILABLE_YET DMA protection has been enabled, but DMA buffer are
                                not available to be allocated yet.

**/
EFI_STATUS
EFIAPI
IommuAllocateBuffer (
  IN     EDKII_IOMMU_PPI                          *This,
  IN     EFI_MEMORY_TYPE                          MemoryType,
  IN     UINTN                                    Pages,
  IN OUT VOID                                     **HostAddress,
  IN     UINT64                                   Attributes
  )
{
  EFI_PHYSICAL_ADDRESS      PhysicalAddress;

  //
  // Memory type must be EfiRuntimeServicesData or EfiBootServicesData
  //
  if (MemoryType != EfiBootServicesData && MemoryType != EfiRuntimeServicesData) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a: Memory type invalid\n", __FUNCTION__ );
    return EFI_INVALID_PARAMETER;
  }

  //
  // Make sure address is valid
  //
  if (HostAddress == NULL) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a: Invalid parameter passed in.\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check Attributes
  //
  if ((Attributes & EDKII_IOMMU_ATTRIBUTE_INVALID_FOR_ALLOCATE_BUFFER) != 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a: Attribute invalid.\n");
    return EFI_UNSUPPORTED;
  }

  PhysicalAddress = (EFI_PHYSICAL_ADDRESS)AllocatePages (Pages);

  *HostAddress = (VOID *) (UINTN) PhysicalAddress;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Called with %d Memory type, %d pages, %x hostaddress and %x attributes\n",
                     __FUNCTION__, MemoryType, Pages, (UINT32)*HostAddress, Attributes);

  return EFI_SUCCESS;
}


/**
  Frees memory that was allocated with AllocateBuffer().

  @param  This                  The PPI instance pointer.
  @param  Pages                 The number of pages to free.
  @param  HostAddress           The base system memory address of the allocated range.

  @retval EFI_SUCCESS           The requested memory pages were freed.
  @retval EFI_INVALID_PARAMETER The memory range specified by HostAddress and Pages
                                was not allocated with AllocateBuffer().
  @retval EFI_NOT_AVAILABLE_YET DMA protection has been enabled, but DMA buffer are
                                not available to be allocated yet.

**/
EFI_STATUS
EFIAPI
IommuFreeBuffer (
  IN  EDKII_IOMMU_PPI                          *This,
  IN  UINTN                                    Pages,
  IN  VOID                                     *HostAddress
  )
{
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Called with %d pages and %x hostaddress\n",
                     __FUNCTION__, Pages, (UINT32)HostAddress);
  FreePages(HostAddress, Pages);

  return EFI_SUCCESS;
}

/**
 * Initialize the PPI to be installed
 *
 */
EDKII_IOMMU_PPI mAmdIommuServicesPpi = {
  EDKII_IOMMU_PPI_REVISION,
  IommuSetAttribute,
  IommuMap,
  IommuUnmap,
  IommuAllocateBuffer,
  IommuFreeBuffer
};


/**
  Install the PPI needed for Amd IOMMU services in PEI and initialize anything else needed.

  @param PeiServices  Double pointer to PPI Services

  @return EFI_STATUS  Returns the status of InstallPpi()
**/
EFI_STATUS
IommuSetupPpi (
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS       Status;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a\n", __FUNCTION__);
  Status = (**PeiServices).InstallPpi (PeiServices, &mAmdIommuServicesPpiList);  //This driver is done.

  return Status;
}
