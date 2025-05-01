/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file
 *
 * Local header file for IOMMU implementation
 *
 */
#ifndef _AMD_NBIO_IOMMU_DXE_H_
#define _AMD_NBIO_IOMMU_DXE_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

// Have to declare this before anything else due to conflicts in IdsLib.h
#include <Library/DebugLib.h>

#include <Filecode.h>
#include <AMD.h>
#include <GnbIommu.h>
#include <GnbDxio.h>

#include <Protocol/IoMmu.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>
#include <Protocol/AmdNbioPcieServicesProtocol.h>

#include <Library/IdsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/BaseLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/GnbPciLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/NbioHandleLib.h>

#include <Guid/GnbPcieInfoHob.h>

/**
 * This structure is not in the IOMMU spec but is used in the protocol/ppi for caching key information
 *
 */
#define AMD_IOMMU_MAP_INFO_SIGNATURE  SIGNATURE_32 ('A', 'M', 'A', 'P')
typedef struct {
  UINT32                                    Signature;
  LIST_ENTRY                                Link;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION Operation;
  UINTN                                     NumberOfBytes;
  UINTN                                     NumberOfPages;
  EFI_PHYSICAL_ADDRESS                      HostAddress;
  EFI_PHYSICAL_ADDRESS                      MappedDeviceAddress;
  PCI_ADDR                                  PciDeviceAddress;
} AMD_IOMM_MAP_INFO;
#define AMD_IOMM_MAP_INFO_FROM_LINK(a) CR(a, AMD_IOMM_MAP_INFO, Link, AMD_IOMMU_MAP_INFO_SIGNATURE)

// Define a sub-trace level here so logging can quickly be switched on or off
#define IOMMU_TRACE_VERBOSE GNB_TRACE_RSV2
#define IOMMU_TRACE_NORMAL  GNB_TRACE_RSV2
#define IOMMU_TRACE_URGENT  GNB_TRACE

/**
 * Used for fields where the lower significant bits are shifted out. May require additional cast to final data type
**/
#define SHIFT_RIGHT_POINTER_ADDR(Address, Bits)     (RShiftU64((UINT64)Address, Bits))
#define SHIFT_LEFT_POINTER_ADDR(Address, Bits)      (LShiftU64((UINT64)Address, Bits))
/**
  Each NBIO has one IOMMU upstream.

  @param GnbHandle

  @return BOOLEAN
**/
BOOLEAN
SetupIommuOnEachNbio (
  IN GNB_HANDLE          *GnbHandle
  );


/**
  Sets the specified attributes of a device address or frees the entries associated with that address for disposal

  @param Device               PCI address of the device associated with this address.
  @param DeviceAddress        Virtual device address that has previously been assigned to this device.
  @param BufferSizeInPages    Size of the buffer in pages.
  @param SetAttributesOnly    If true only read/write bits are set after table walk. If false the translation entries are freed.
  @param ReadPermissions      Only active if SetAttributesOnly is TRUE. This is passed on to the page translation entries.
  @param WritePermissions     Only active if SetAttributesOnly is TRUE. This is passed on to the page translation entries.

  @return BOOLEAN             Returns TRUE if successful. False if a problem was encountered and request could not be completed
**/
BOOLEAN
SetAttributesOrFreeDeviceAddress (
  IN PCI_ADDR         Device,
  IN UINT64           DeviceAddress,
  IN UINTN            BufferSizeInPages,
  IN BOOLEAN          SetAttributesOnly,
  IN BOOLEAN          ReadPermissions,
  IN BOOLEAN          WritePermissions
  );

/**
  Assign a virtual address for a device's DMA memory buffer for usage with IOMMU.

  @param PhysicalBuffer               Pointer to beginning of physical address. Should be 4k aligned
  @param PhysicalBufferSizeInPages    Size of buffer in terms of 4k pages
  @param Above4GDeviceAddress         Does device support an over 4G device address

  @return UINT64
**/
UINT64
AssignDeviceAddress (
  IN VOID             *HostBuffer,
  IN UINTN            PhysicalBufferSizeInPages,
  IN BOOLEAN          Above4GDeviceAddress
  );

/**
  Install the Protocol needed for AMD IOMMU services in DXE and initialize anything else needed.

  @return EFI_STATUS  Returns the status of InstallProtocolInterface()
**/
EFI_STATUS
IommuSetupProtocol (
  VOID
  );

/**
  Dump the event log to the console. Nothing will be printed if there are no entries.

**/
VOID
DumpEventLog (
  VOID
  );

/**
  Send IOMMU command buffer a command to invalidate cache for a range of pages.

**/
VOID
SendInvdPagesCommand (
  IN UINT64      Address,
  IN BOOLEAN     Invd4kOnly
  );

/**
  Send IOMMU command buffer a command. For now just INVD but once we enhance caching strategy it will be other commands

**/
VOID
SendInvdAllCommand (
  VOID
  );

/**
  Dump the contents of a device table entry, if you are curious

  @param DeviceTableEntry  Pointer to entry to dump
**/
VOID
DumpDTE (
  IOMMU_DEVICE_TABLE                 *DeviceTableEntry
  );

/**
  Get alignment of input address and given length. This is important because the IOMMU only works with
   aligned memory addresses. Optional outputs are only populated if their respective pointers are NULL

  @param Address                      Address to evaluate
  @param InNumberOfBytes              Length of memory region in bytes
  @param NewAlignedAddress            Optional output represents the new aligned address that should be used
  @param Alignment                    Optional output represents the alignment that should be used
  @param Numberof4kPagesNeeded        Optional output represents the number of 4k pages ultimately mapped
  @param NumberOfNativePagesNeeded    Optional output represents the number of native pages mapped, size of page is Alignment

  @return BOOLEAN
**/
BOOLEAN
GetAppropriateAlignmentAndSpan (
  IN  UINT64       InAddress,
  IN  UINTN        InNumberOfBytes,
  OUT UINT64       *NewAlignedAddress OPTIONAL,
  OUT UINTN        *Alignment OPTIONAL,
  OUT UINTN        *Numberof4kPagesNeeded OPTIONAL,
  OUT UINTN        *NumberOfNativePagesNeeded OPTIONAL
  );

#endif /* _AMD_NBIO_IOMMU_DXE_H_ */
