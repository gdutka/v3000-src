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

#define FILECODE        NBIO_LIBRARY_IOMMUDMARLIB_DXE_IOMMUPROTOCOL_FILECODE

extern
IOMMU_CONTEXT_DATA     mIommuContextData[IOMMU_CONTEXT_DATA_ENTRIES];

#define IOMMU_HDT_CONSOLE(f, s, ...)
// To support IOMMU logging use the following instead
// #define IOMMU_HDT_CONSOLE(f, s, ...) IDT_HDT_CONSOLE(f, s, ...)


/**
  This is the list head of the linked list we are going to use to keep track of our allocations

**/
LIST_ENTRY                        gSavedMaps = INITIALIZE_LIST_HEAD_VARIABLE(gSavedMaps);
EFI_HANDLE                        gNbioIommuHandle = NULL;

/**
  Helper function to extract the PCIe device address out of a UEFI handle passed in

  @param DeviceHandle     Device handle that should contain an instance of PCI IO Protocol
  @param ReturnAddress    Pointer to a PCI return address structure to contain the resultant address

  @return EFI_STATUS      EFI_SUCCESS if the device address is found.
                          EFI_NOT_FOUND if handle does not contain address information
                          EFI_INVALID_PARAMETER if return address structure was null
**/
EFI_STATUS
GetPciAddressFromHandle (
  IN   EFI_HANDLE            DeviceHandle,
  OUT  PCI_ADDR              *ReturnAddress
  )
{
  EFI_STATUS                 Status;
  EFI_PCI_IO_PROTOCOL        *PciIo;
  UINTN                      Segment, Bus, Device, Function;

  // Input parameter check
  ASSERT(ReturnAddress != NULL);
  if (ReturnAddress == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : Null device pointer passed to this function\n", __FUNCTION__);
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (DeviceHandle, &gEfiPciIoProtocolGuid, (VOID **)&PciIo);

  ASSERT(Status == EFI_SUCCESS);
  if (Status != EFI_SUCCESS) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : Could not find PCIe protocol on handle, %r\n", __FUNCTION__, Status);
    return EFI_NOT_FOUND;
  }

  Status = PciIo->GetLocation (
                    PciIo,
                    &Segment,
                    &Bus,
                    &Device,
                    &Function
                    );
  ReturnAddress->Address.Segment  = (UINT32)Segment;
  ReturnAddress->Address.Bus      = (UINT32)Bus;
  ReturnAddress->Address.Device   = (UINT32)Device;
  ReturnAddress->Address.Function = (UINT32)Function;


  if (Status != EFI_SUCCESS) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : Could not get PCIe address from PCI IO protocol, %r\n", __FUNCTION__, Status);
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
/**
  Set IOMMU attribute for a system memory.

  If the IOMMU protocol exists, the system memory cannot be used
  for DMA by default.

  When a device requests a DMA access for a system memory,
  the device driver need use SetAttribute() to update the IOMMU
  attribute to request DMA access (read and/or write).

  The DeviceHandle is used to identify which device submits the request.
  The IOMMU implementation need translate the device path to an IOMMU device ID,
  and set IOMMU hardware register accordingly.
  1) DeviceHandle can be a standard PCI device.
     The memory for BusMasterRead need set EDKII_IOMMU_ACCESS_READ.
     The memory for BusMasterWrite need set EDKII_IOMMU_ACCESS_WRITE.
     The memory for BusMasterCommonBuffer need set EDKII_IOMMU_ACCESS_READ|EDKII_IOMMU_ACCESS_WRITE.
     After the memory is used, the memory need set 0 to keep it being protected.
  2) DeviceHandle can be an ACPI device (ISA, I2C, SPI, etc).
     The memory for DMA access need set EDKII_IOMMU_ACCESS_READ and/or EDKII_IOMMU_ACCESS_WRITE.

  @param[in]  This              The protocol instance pointer.
  @param[in]  DeviceHandle      The device who initiates the DMA access request.
  @param[in]  Mapping           The mapping value returned from Map().
  @param[in]  IoMmuAccess       The IOMMU access.

  @retval EFI_SUCCESS            The IoMmuAccess is set for the memory range specified by DeviceAddress and Length.
  @retval EFI_INVALID_PARAMETER  DeviceHandle is an invalid handle.
  @retval EFI_INVALID_PARAMETER  Mapping is not a value that was returned by Map().
  @retval EFI_INVALID_PARAMETER  IoMmuAccess specified an illegal combination of access.
  @retval EFI_UNSUPPORTED        DeviceHandle is unknown by the IOMMU.
  @retval EFI_UNSUPPORTED        The bit mask of IoMmuAccess is not supported by the IOMMU.
  @retval EFI_UNSUPPORTED        The IOMMU does not support the memory range specified by Mapping.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to modify the IOMMU access.
  @retval EFI_DEVICE_ERROR       The IOMMU device reported an error while attempting the operation.

**/
EFI_STATUS
EFIAPI
IommuSetAttribute (
  IN EDKII_IOMMU_PROTOCOL  *This,
  IN EFI_HANDLE            DeviceHandle,
  IN VOID                  *Mapping,
  IN UINT64                IoMmuAccess
  )
{
  BOOLEAN            ReadPerms;
  BOOLEAN            WritePerms;
  AMD_IOMM_MAP_INFO  *MapInfo;
  EFI_STATUS         Status;

  if (mIommuContextData[0].Present == TRUE) {
    // Input parameter check
    ASSERT(Mapping != NULL);
    if (Mapping == NULL) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Called with NULL mapping info. Cannot setup IOMMU mapping for DMA. \n", __FUNCTION__);
      return EFI_INVALID_PARAMETER;
    }
    MapInfo = (AMD_IOMM_MAP_INFO *)Mapping;
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with 0x%llx address and iommu access bits %d\n",
                    __FUNCTION__, (UINT64)MapInfo->MappedDeviceAddress, IoMmuAccess);

    ReadPerms = FALSE;
    WritePerms = FALSE;

    if ((IoMmuAccess & EDKII_IOMMU_ACCESS_READ) > 0) {
      ReadPerms =  TRUE;
    }
    if ((IoMmuAccess & EDKII_IOMMU_ACCESS_WRITE) > 0) {
      WritePerms =  TRUE;
    }

    // Need to find out device address
    Status = GetPciAddressFromHandle (
               DeviceHandle,
               &(MapInfo->PciDeviceAddress)
               );

    ASSERT(Status == EFI_SUCCESS);
    if (Status != EFI_SUCCESS) {
      IOMMU_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Cannot retrieve PCIe address from handle. Cannot setup IOMMU mapping for DMA. \n", __FUNCTION__);
    }
    IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Requesting PCIe device is B%xD%xF%x \n",
                    __FUNCTION__,
                    MapInfo->PciDeviceAddress.Address.Bus,
                    MapInfo->PciDeviceAddress.Address.Device,
                    MapInfo->PciDeviceAddress.Address.Function
                    );


    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Attribute Device address 0x%llx through 0x%llx  \n",
                     __FUNCTION__,
                     MapInfo->MappedDeviceAddress,
                     MapInfo->MappedDeviceAddress + MapInfo->NumberOfPages * PAGE_SIZE
                     );
    // This is where the real work occurs
    SetAttributesOrFreeDeviceAddress (
                            MapInfo->PciDeviceAddress,
                            MapInfo->MappedDeviceAddress,
                            MapInfo->NumberOfPages,
                            TRUE,
                            ReadPerms,
                            WritePerms
                            );
  }
  return EFI_SUCCESS;
}


/**
  Provides the controller-specific addresses required to access system memory from a
  DMA bus master.

  @param  This                  The protocol instance pointer.
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

**/
EFI_STATUS
EFIAPI
IommuMap (
  IN     EDKII_IOMMU_PROTOCOL                       *This,
  IN     EDKII_IOMMU_OPERATION                      Operation,
  IN     VOID                                       *HostAddress,
  IN OUT UINTN                                      *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS                       *DeviceAddress,
  OUT    VOID                                       **Mapping
  )
{
  AMD_IOMM_MAP_INFO                                 *MapInfo;
  EFI_TPL                                           OriginalTpl;
  UINTN                                             Alignment;


  // Input parameter check
  if (NumberOfBytes == NULL || DeviceAddress == NULL || Mapping == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with %d operation, 0x%x host address, 0x%x number of bytes\n",
                   __FUNCTION__, Operation, HostAddress, *NumberOfBytes);

  if (mIommuContextData[0].Present == TRUE) {
    //
    // Make sure that Operation is valid
    //
    if ((UINT32) Operation >= EdkiiIoMmuOperationMaximum) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
      return EFI_INVALID_PARAMETER;
    }

    //
    // Allocate a AMD_IOMM_MAP_INFO structure to remember the mapping when Unmap() is
    // called later.
    //
    MapInfo = AllocateZeroPool (sizeof (AMD_IOMM_MAP_INFO));
    if (MapInfo == NULL) {
      *NumberOfBytes = 0;
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_OUT_OF_RESOURCES);
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Initialize the AMD_IOMM_MAP_INFO structure
    //
    MapInfo->Signature         = AMD_IOMMU_MAP_INFO_SIGNATURE;
    MapInfo->Operation         = Operation;
    MapInfo->NumberOfBytes     = *NumberOfBytes;
    MapInfo->HostAddress       = (EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress;

    if (!GetAppropriateAlignmentAndSpan (
          MapInfo->HostAddress,
          *NumberOfBytes,
          &(MapInfo->MappedDeviceAddress),
          &Alignment,
          &(MapInfo->NumberOfPages),
          NULL
          )) {
      *NumberOfBytes = 0;
      FreePool (MapInfo);
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : %r\n", __FUNCTION__, EFI_OUT_OF_RESOURCES);
      return EFI_OUT_OF_RESOURCES;
    }

    // Assign virtual device address to access the same memory pointed to by the physical address
    // Discriminate 64 vs 32 bit address assignments
    if (Operation >= EdkiiIoMmuOperationBusMasterRead64) {
      MapInfo->MappedDeviceAddress = AssignDeviceAddress((VOID *)MapInfo->MappedDeviceAddress, MapInfo->NumberOfPages, TRUE);
    }
    else {
      MapInfo->MappedDeviceAddress = AssignDeviceAddress((VOID *)MapInfo->MappedDeviceAddress, MapInfo->NumberOfPages, FALSE);
    }

    ASSERT((VOID *)MapInfo->MappedDeviceAddress != NULL);

    OriginalTpl = gBS->RaiseTPL (AMD_IOMMU_TPL_LEVEL);
    InsertTailList (&gSavedMaps, &MapInfo->Link);
    gBS->RestoreTPL (OriginalTpl);

    // Adjust the aligned virtual address to point to the same offset of the real address
    *DeviceAddress = ((Alignment - 1) & (MapInfo->HostAddress)) | MapInfo->MappedDeviceAddress;

    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : Assigned virtual address ( 0x%lx ) with added offset is 0x%lx \n",
                      __FUNCTION__, MapInfo->MappedDeviceAddress, *DeviceAddress);


    // Use this structure to keep track of the mapping
    *Mapping       = MapInfo;

    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Map Device address 0x%llx through 0x%llx and mapping memory location 0x%x \n",
                     __FUNCTION__,
                     MapInfo->MappedDeviceAddress,
                     MapInfo->MappedDeviceAddress + MapInfo->NumberOfPages * PAGE_SIZE,
                     *Mapping
                     );

    DumpEventLog();
  } else {
    *DeviceAddress = (EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress;
  }

  return EFI_SUCCESS;
}

/**
  Completes the Map() operation and releases any corresponding resources.

  @param  This                  The protocol instance pointer.
  @param  Mapping               The mapping value returned from Map().

  @retval EFI_SUCCESS           The range was unmapped.
  @retval EFI_INVALID_PARAMETER Mapping is not a value that was returned by Map().
  @retval EFI_DEVICE_ERROR      The data was not committed to the target system memory.
**/
EFI_STATUS
EFIAPI
IommuUnmap (
  IN  EDKII_IOMMU_PROTOCOL                     *This,
  IN  VOID                                     *Mapping
  )
{
  AMD_IOMM_MAP_INFO        *MapInfo;
  LIST_ENTRY               *Link;
  EFI_TPL                  OriginalTpl;
  BOOLEAN                  DeviceUnmapStatus;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with 0x%x mapping\n", __FUNCTION__, Mapping);
  if (mIommuContextData[0].Present == TRUE) {
    DumpEventLog();
    // Input parameter check
    if (Mapping == NULL) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a: %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
      return EFI_INVALID_PARAMETER;
    }

    //
    // Verify that this node is actually in the list
    //
    OriginalTpl = gBS->RaiseTPL (AMD_IOMMU_TPL_LEVEL);
    MapInfo = NULL;
    for (Link = GetFirstNode (&gSavedMaps); !IsNull (&gSavedMaps, Link); Link = GetNextNode (&gSavedMaps, Link)) {
      MapInfo = AMD_IOMM_MAP_INFO_FROM_LINK (Link);
      if (MapInfo == Mapping) {
        break;
      }
    }

    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Unmap Device address 0x%llx through 0x%llx \n",
                     __FUNCTION__,
                     MapInfo->MappedDeviceAddress,
                     MapInfo->MappedDeviceAddress + MapInfo->NumberOfPages * PAGE_SIZE
                     );

    // Now call this function to erase the IOMMU mapping
    DeviceUnmapStatus = SetAttributesOrFreeDeviceAddress (
                          MapInfo->PciDeviceAddress,
                          MapInfo->MappedDeviceAddress,
                          MapInfo->NumberOfPages,
                          FALSE,
                          FALSE,
                          FALSE
                          );
    if (!DeviceUnmapStatus) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Unmap unsuccessful \n", __FUNCTION__);
    }
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Unmap success! Now try to free mapping entry in memory \n", __FUNCTION__);
    //
    // Mapping is not a valid value returned by Map()
    //
    if (MapInfo != Mapping) {
      gBS->RestoreTPL (OriginalTpl);
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a: Map info does not equal mapping %r\n", __FUNCTION__, EFI_INVALID_PARAMETER);
      return EFI_INVALID_PARAMETER;
    }
    RemoveEntryList (&MapInfo->Link);
    gBS->RestoreTPL (OriginalTpl);

    FreePool (Mapping);

  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Exit \n", __FUNCTION__);
  return EFI_SUCCESS;
}

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param  This                  The protocol instance pointer.
  @param  Type                  This parameter is not used and must be ignored.
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

**/
EFI_STATUS
EFIAPI
IommuAllocateBuffer (
  IN     EDKII_IOMMU_PROTOCOL                     *This,
  IN     EFI_ALLOCATE_TYPE                        Type,
  IN     EFI_MEMORY_TYPE                          MemoryType,
  IN     UINTN                                    Pages,
  IN OUT VOID                                     **HostAddress,
  IN     UINT64                                   Attributes
  )
{
  EFI_STATUS                Status;
  EFI_PHYSICAL_ADDRESS      PhysicalAddress;

  //
  // Memory type must be EfiRuntimeServicesData or EfiBootServicesData
  //
  if (MemoryType != EfiBootServicesData && MemoryType != EfiRuntimeServicesData) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Memory type invalid\n", __FUNCTION__ );
    return EFI_INVALID_PARAMETER;
  }

  //
  // Make sure address is valid
  //
  if (HostAddress == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Invalid parameter passed in.\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check Attributes
  //
  if ((Attributes & EDKII_IOMMU_ATTRIBUTE_INVALID_FOR_ALLOCATE_BUFFER) != 0) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a: Attribute invalid.\n");
    return EFI_UNSUPPORTED;
  }

  PhysicalAddress = MAX_UINTN;
  if ((Attributes & EDKII_IOMMU_ATTRIBUTE_DUAL_ADDRESS_CYCLE) == 0) {
    //
    // TOOD: Should allocations remain under the 4G boundary?
    //
    PhysicalAddress = MIN (PhysicalAddress, SIZE_4GB - 1);
  }
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  MemoryType,
                  Pages,
                  &PhysicalAddress
                  );
  if (!EFI_ERROR (Status)) {
    *HostAddress = (VOID *) (UINTN) PhysicalAddress;
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with %d Memory type, %d pages, 0x%x hostaddress and 0x%x attributes\n",
                       __FUNCTION__, MemoryType, Pages, (UINT64)*HostAddress, Attributes);
  }
  else {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a: Could not allocate any memory!!!\n", __FUNCTION__);
  }

  return Status;
}

/**
  Frees memory that was allocated with AllocateBuffer().

  @param  This                  The protocol instance pointer.
  @param  Pages                 The number of pages to free.
  @param  HostAddress           The base system memory address of the allocated range.

  @retval EFI_SUCCESS           The requested memory pages were freed.
  @retval EFI_INVALID_PARAMETER The memory range specified by HostAddress and Pages
                                was not allocated with AllocateBuffer().

**/
EFI_STATUS
EFIAPI
IommuFreeBuffer (
  IN  EDKII_IOMMU_PROTOCOL                     *This,
  IN  UINTN                                    Pages,
  IN  VOID                                     *HostAddress
  )
{
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Called with %d pages and 0x%x hostaddress\n",
                     __FUNCTION__, Pages, (UINT64)HostAddress);
  return gBS->FreePages (
                (EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress,
                Pages
                );
}

/**
 * Initialize the protocol to be installed
 *
 */
EDKII_IOMMU_PROTOCOL mAmdIommuServicesProtocol = {
  EDKII_IOMMU_PROTOCOL_REVISION,
  IommuSetAttribute,
  IommuMap,
  IommuUnmap,
  IommuAllocateBuffer,
  IommuFreeBuffer
};


/**
  Install the Protocol needed for AMD IOMMU services in DXE and initialize anything else needed.

  @return EFI_STATUS  Returns the status of InstallProtocolInterface()
**/
EFI_STATUS
IommuSetupProtocol (
  VOID
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a\n", __FUNCTION__);
  Status = gBS->InstallProtocolInterface (
            &gNbioIommuHandle,
            &gEdkiiIoMmuProtocolGuid,
            EFI_NATIVE_INTERFACE,
            &mAmdIommuServicesProtocol
            );

  return Status;
}


