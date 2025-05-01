/** @file
  Implementation of PciDeviceInfoDxe.

;******************************************************************************
;* Copyright (c) 2023 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "PciDeviceInfoDxe.h"

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DeviceInfoLookupDataLib.h>
#include <Library/DevicePathLib.h>
#include <Library/H2OCpLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>

#include <Protocol/DevicePath.h>
#include <Protocol/H2ODeviceInfoLookup.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>

#include <Protocol/H2OPciDeviceInfo.h>

#include <Guid/H2OCp.h>

#ifndef BASE_LIST_FOR_EACH
#define BASE_LIST_FOR_EACH(Entry, ListHead)    \
  for(Entry = (ListHead)->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)
#endif

STATIC UINT32                           mSerialNumber = 0;
STATIC H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *mDeviceInfoLookup = NULL;

///
/// Function Declarations
///
EFI_STATUS
GetPciDeviceInfoPool (
  IN  H2O_PCI_DEVICE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **PciDeviceInfoPool
  );

STATIC H2O_PCI_DEVICE_INFO_PRIVATE_DATA   mPrivateData = {
  H2O_PCI_DEVICE_INFO_PRIVATE_SIGNATURE,
  0,
  {
    sizeof (H2O_PCI_DEVICE_INFO_PROTOCOL),
    GetPciDeviceInfoPool
  },
  INITIALIZE_LIST_HEAD_VARIABLE (mPrivateData.PciDeviceInfoPool)
};

EFI_STATUS
GetPciDeviceInfoPool (
  IN  H2O_PCI_DEVICE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **PciDeviceInfoPool
  )
{
  H2O_PCI_DEVICE_INFO_PRIVATE_DATA  *Private;

  if (This == NULL || PciDeviceInfoPool == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = H2O_PCI_DEVICE_INFO_PRIVATE_DATA_FROM_THIS (This);

  *PciDeviceInfoPool = &Private->PciDeviceInfoPool;

  return EFI_SUCCESS;
}

/**
  Finds the Offset to a given Capabilities ID
  Each capability has an ID and a pointer to next Capability, so they form a linked list.
  This function walks the list of Capabilities present in device's pci cfg. If requested capability
  can be found, its offset is returned.
  If the capability can't be found or if device doesn't exist, function returns 0
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] DeviceBase           device's base address
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found (this includes situation where device doesn't exit)
  @retval Other                   CAPID found, Offset of desired CAPID

**/
UINT8
PcieBaseFindCapId (
  IN UINT64  DeviceBase,
  IN UINT8   CapId
  )
{
  UINT8  CapHeaderOffset;
  UINT8  CapHeaderId;
  UINT16 Data16;
  //
  // We do not explicitly check if device exists to save time and avoid unnecessary PCI access
  // If the device doesn't exist, check for CapHeaderId != 0xFF will fail and function will return offset 0
  //
  if ((PciSegmentRead8 (DeviceBase + PCI_PRIMARY_STATUS_OFFSET) & EFI_PCI_STATUS_CAPABILITY) == 0x00) {
    ///
    /// Function has no capability pointer
    ///
    return 0;
  } else {
    ///
    /// Check the header layout to determine the Offset of Capabilities Pointer Register
    ///
    if ((PciSegmentRead8 (DeviceBase + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE) == (HEADER_TYPE_CARDBUS_BRIDGE)) {
      ///
      /// If CardBus bridge, start at Offset 0x14
      ///
      CapHeaderOffset = EFI_PCI_CARDBUS_BRIDGE_CAPABILITY_PTR;
    } else {
      ///
      /// Otherwise, start at Offset 0x34
      ///
      CapHeaderOffset = PCI_CAPBILITY_POINTER_OFFSET;
    }
    ///
    /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
    ///
    CapHeaderId     = 0;
    CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset) & ((UINT8) ~(BIT0 | BIT1));
    while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
      Data16 = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
      CapHeaderId = (UINT8)(Data16 & 0xFF);
      if (CapHeaderId == CapId) {
        if (CapHeaderOffset > PCI_MAXLAT_OFFSET) {
          ///
          /// Return valid capability offset
          ///
          return CapHeaderOffset;
        } else {
          ASSERT ((FALSE));
          return 0;
        }
      }
      ///
      /// Each capability must be DWORD aligned.
      /// The bottom two bits of all pointers (including the initial pointer at 34h) are reserved
      /// and must be implemented as 00b although software must mask them to allow for future uses of these bits.
      ///
      CapHeaderOffset = (UINT8)(Data16 >> 8);
    }
    return 0;
  }
}

/**
  Helpder function to get 64-bit PCI Segment Library Address through PciIo Protocol.

  @param[in]  PciIo         The PciIo protocol instance of the device.
  @param[in]  BaseAddress   The 64-bit PCI Segment Library Address of the device.

  @retval EFI_SUCCESS             Successfully get the address.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval Others                  Fail to get the address.

**/
EFI_STATUS
GetBaseAddress (
  IN    EFI_PCI_IO_PROTOCOL   *PciIo,
  OUT   UINT64                *BaseAddress
  )
{
  EFI_STATUS  Status;
  UINTN       Segment;
  UINTN       Bus;
  UINTN       Device;
  UINTN       Function;

  if (PciIo == NULL || BaseAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
  if (!EFI_ERROR (Status)) {
    *BaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
  }

  return Status;
}

EFI_STATUS
GetSlotName (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT CHAR16                    **SlotNameStr
  )
{
  EFI_STATUS                        Status;
  CHAR16                            *DisplayName;
  CHAR16                            StrBuffer[0x100];

  if ((DevicePath == NULL) || (SlotNameStr == NULL) || (mDeviceInfoLookup == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  (*SlotNameStr) = NULL;

  Status = mDeviceInfoLookup->GetAncestorDeviceInfoOfType (
                                mDeviceInfoLookup,
                                DevicePath,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                &DisplayName
                                );
  if (EFI_ERROR (Status) || DisplayName == NULL) {
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"N/A");
  } else {
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"%s", DisplayName);
    FreePool (DisplayName);
  }

  (*SlotNameStr) = AllocateCopyPool (StrnSizeS (StrBuffer, sizeof (StrBuffer)), StrBuffer);
  if ((*SlotNameStr) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Helpder function to initialize a new PCI Device Info of a given device.

  @param[in]  DeviceHandle    The device handle of the PCI device.
  @param[in]  PciDeviceInfo   The pointer to the instance of the PCI Device Info.
                              It is the caller's responsibility to free the buffer through
                              function "DestroyPciDeviceInfo".

  @retval EFI_SUCCESS             Successfully initialize the PCI Devcie Info.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resources.
  @retval Others                  Fail to initialize the PCI Device Info.

**/
EFI_STATUS
InitializePciDeviceInfo (
  IN  EFI_HANDLE        DeviceHandle,
  OUT PCI_DEVICE_INFO   **PciDeviceInfo
  )
{
  EFI_STATUS                      Status;
  PCI_DEVICE_INFO                 *NewPciDeviceInfo;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINT64                          BaseAddress;
  PCI_TYPE00                      PciConfigSpace;
  UINT8                           PcieCapOffset;
  PCI_CAPABILITY_PCIEXP           PcieCap;
  BOOLEAN                         IsRootBridge;
  BOOLEAN                         IsBridge;

  NewPciDeviceInfo  = NULL;
  DevicePath        = NULL;
  PciIo             = NULL;
  PciRootBridgeIo   = NULL;
  BaseAddress       = 0;
  ZeroMem ((VOID *) &PciConfigSpace, sizeof (PciConfigSpace));
  PcieCapOffset     = 0;
  ZeroMem ((VOID *) &PcieCap, sizeof (PcieCap));
  IsRootBridge      = FALSE;
  IsBridge          = FALSE;

  if (DeviceHandle == NULL || PciDeviceInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (
                  DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HandleProtocol on Handle 0x%08x, Status: %r\n", __FUNCTION__, DeviceHandle, Status));
    goto ErrorExit;
  }

  Status = gBS->HandleProtocol (
                  DeviceHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo
                  );
  if (EFI_ERROR (Status)) {
    Status = gBS->HandleProtocol (
                    DeviceHandle,
                    &gEfiPciRootBridgeIoProtocolGuid,
                    (VOID **) &PciRootBridgeIo
                    );
    if (!EFI_ERROR (Status)) {
      IsRootBridge = TRUE;
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HandleProtocol on Handle 0x%08x, Status: %r\n", __FUNCTION__, DeviceHandle, Status));
    goto ErrorExit;
  }

  if (!IsRootBridge) {
    Status = GetBaseAddress (PciIo, &BaseAddress);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to GetBaseAddress, Status: %r\n", __FUNCTION__, Status));
      goto ErrorExit;
    }

    PciSegmentReadBuffer (BaseAddress, sizeof (PciConfigSpace), (VOID *) &PciConfigSpace);
    if (IsZeroBuffer (&PciConfigSpace, sizeof (PciConfigSpace)) ||
        PciConfigSpace.Hdr.VendorId == 0xFFFF ||
        PciConfigSpace.Hdr.DeviceId == 0xFFFF) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to read PCI configuration space.\n", __FUNCTION__, Status));
      Status = EFI_DEVICE_ERROR;
      goto ErrorExit;
    }

    if (IS_PCI_BRIDGE (&PciConfigSpace)) {
      IsBridge = TRUE;
    }
  }

  PcieCapOffset = PcieBaseFindCapId (BaseAddress, EFI_PCI_CAPABILITY_ID_PCIEXP);

  if (PcieCapOffset != 0) {
    PciSegmentReadBuffer ((BaseAddress + PcieCapOffset), sizeof (PcieCap), (VOID *) &PcieCap);
    if (IsZeroBuffer (&PcieCap, sizeof (PcieCap)) ||
        PcieCap.Hdr.CapabilityID != EFI_PCI_CAPABILITY_ID_PCIEXP) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to read PCI Capbility PCI Express at offset %02xh.\n", __FUNCTION__, PcieCapOffset));
      Status = EFI_DEVICE_ERROR;
      goto ErrorExit;
    }
  }

  NewPciDeviceInfo = AllocateZeroPool (sizeof (PCI_DEVICE_INFO));
  if (NewPciDeviceInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewPciDeviceInfo->Signature       = PCI_DEVICE_INFO_SIGNATURE;
  NewPciDeviceInfo->DeviceHandle    = DeviceHandle;
  NewPciDeviceInfo->DevicePath      = DuplicateDevicePath (DevicePath);
  NewPciDeviceInfo->PciIo           = PciIo;
  NewPciDeviceInfo->PciRootBridgeIo = PciRootBridgeIo;
  NewPciDeviceInfo->BaseAddress     = BaseAddress;
  CopyMem (
    (VOID *) &NewPciDeviceInfo->PciConfigSpace,
    (VOID *) &PciConfigSpace,
    sizeof (PciConfigSpace)
    );
  NewPciDeviceInfo->PcieCapOffset   = PcieCapOffset;
  CopyMem (
    (VOID *) &NewPciDeviceInfo->PcieCap,
    (VOID *) &PcieCap,
    sizeof (PcieCap)
    );
  NewPciDeviceInfo->IsRootBridge    = IsRootBridge;
  NewPciDeviceInfo->IsBridge        = IsBridge;

  //
  // Initialize anyway
  //
  InitializeListHead (&NewPciDeviceInfo->ChildList);

  NewPciDeviceInfo->SerialNumber    = mSerialNumber++;

  GetSlotName (DevicePath, &NewPciDeviceInfo->SlotNameStr);

  *PciDeviceInfo = NewPciDeviceInfo;

  return EFI_SUCCESS;

ErrorExit:

  return Status;
}

/**
  Insert the PCI Device Info into the PCI Device Info Pool after sort.

  @param[in]  ListHead          The pointer to the PCI Device Info Pool.
  @param[in]  NewPciDeviceInfo  The pointer to the PCI Device Info.

**/
VOID
InsertAfterSort (
  IN  LIST_ENTRY        *ListHead,
  IN  PCI_DEVICE_INFO   *NewPciDeviceInfo
  )
{
  LIST_ENTRY        *Entry;
  PCI_DEVICE_INFO   *PciDeviceInfo;

  if (IsListEmpty (ListHead)) {
    InsertTailList (ListHead, &NewPciDeviceInfo->Link);
    return;
  }

  BASE_LIST_FOR_EACH (Entry, ListHead) {
    PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
    if (NewPciDeviceInfo->IsRootBridge == PciDeviceInfo->IsRootBridge) {
      if ((NewPciDeviceInfo->IsRootBridge &&
            ((ACPI_HID_DEVICE_PATH *) PciDeviceInfo->DevicePath)->UID >
            ((ACPI_HID_DEVICE_PATH *) NewPciDeviceInfo->DevicePath)->UID) ||
          (!NewPciDeviceInfo->IsRootBridge &&
            PciDeviceInfo->BaseAddress > NewPciDeviceInfo->BaseAddress)) {
        break;
      }
    } else {
      if (!NewPciDeviceInfo->IsRootBridge && PciDeviceInfo->IsRootBridge) {
        continue;
      } else {
        break;
      }
    }
  }

  if (Entry == ListHead) {
    InsertTailList (ListHead, &NewPciDeviceInfo->Link);
  } else {
    Entry->BackLink->ForwardLink = &NewPciDeviceInfo->Link;
    NewPciDeviceInfo->Link.BackLink = Entry->BackLink;
    Entry->BackLink = &NewPciDeviceInfo->Link;
    NewPciDeviceInfo->Link.ForwardLink = Entry;
  }
}

/**
  Check if Child is an immediate child of Parent.

  @param[in]  Parent  The device path of the parent device.
  @param[in]  Child   The device path of the child device.

  @retval TRUE    Child is an immediate child of Parent.
  @retval FALSE   Otherwise.

**/
BOOLEAN
IsChildDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *Parent,
  IN  EFI_DEVICE_PATH_PROTOCOL  *Child
  )
{
  UINTN                     ParentSize;
  UINTN                     ChildSize;
  EFI_DEVICE_PATH_PROTOCOL  *Node;

  ParentSize = GetDevicePathSize (Parent);
  ChildSize = GetDevicePathSize (Child);

  if (ChildSize <= ParentSize) {
    return FALSE;
  }

  if (CompareMem (
        (VOID *) Parent,
        (VOID *) Child,
        ParentSize - sizeof (EFI_DEVICE_PATH_PROTOCOL)) != 0) {
    return FALSE;
  }

  Node = (EFI_DEVICE_PATH_PROTOCOL *) ((UINT8 *) Child + ParentSize - sizeof (EFI_DEVICE_PATH_PROTOCOL));
  Node = NextDevicePathNode (Node);
  if (IsDevicePathEnd (Node)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Helper function to find the last device path node before the End.

  @param[in,out]  DevicePath  On input, it points to the device path to process.
                              On output, it points to the last device path node.

  @retval EFI_SUCCESS             Successfully get the last device path node.
  @retval EFI_INVALID_PARAMETER   The DevicePath is NULL.
  @retval EFI_NOT_FOUND           Fail get the last device path node.

**/
EFI_STATUS
FindLastDevicePathNode (
  IN OUT  EFI_DEVICE_PATH_PROTOCOL **DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL *Node;
  EFI_DEVICE_PATH_PROTOCOL *PrevNode;

  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Node        = *DevicePath;
  PrevNode    = NULL;

  while (!IsDevicePathEnd (Node)) {
    PrevNode = Node;
    Node = NextDevicePathNode (Node);
  }

  if (PrevNode != NULL) {
    *DevicePath = PrevNode;
  }

  return PrevNode != NULL ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
  Insert the child node into the child list of the parent after sort.

  @param[in,out]  ChildLink   The pointer to field ChildLink of the PCI Device Info
                              of the child device.
  @param[in,out]  ChildList   The pointer to field ChildList of the PCI Device Info
                              of the parent device.

**/
VOID
InsertChildAfterSort (
  IN OUT  LIST_ENTRY  *ChildLink,
  IN OUT  LIST_ENTRY  *ChildList
  )
{
  EFI_STATUS                Status;
  LIST_ENTRY                *Entry;
  PCI_DEVICE_INFO           *PciDeviceInfo1;
  PCI_DEVICE_INFO           *PciDeviceInfo2;
  EFI_DEVICE_PATH_PROTOCOL  *PciDevicePath1;
  EFI_DEVICE_PATH_PROTOCOL  *PciDevicePath2;

  if (IsListEmpty (ChildList)) {
    InsertTailList (ChildList, ChildLink);
    return;
  }

  PciDeviceInfo1 = PCI_DEVICE_INFO_FROM_CHILD_LINK (ChildLink);
  PciDevicePath1 = PciDeviceInfo1->DevicePath;
  Status = FindLastDevicePathNode (&PciDevicePath1);
  if (EFI_ERROR (Status) || (PciDevicePath1 == NULL)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to FindLastDevicePathNode(PciDevicePath1), Status: %r\n", __FUNCTION__, Status));
    return;
  }

  BASE_LIST_FOR_EACH (Entry, ChildList) {
    PciDeviceInfo2 = PCI_DEVICE_INFO_FROM_CHILD_LINK (ChildLink);
    PciDevicePath2 = PciDeviceInfo2->DevicePath;
    FindLastDevicePathNode (&PciDevicePath2);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to FindLastDevicePathNode(PciDevicePath2), Status: %r\n", __FUNCTION__, Status));
      continue;
    }

    if (((((PCI_DEVICE_PATH *) PciDevicePath2)->Device << 8) | ((PCI_DEVICE_PATH *) PciDevicePath2)->Function) >
        ((((PCI_DEVICE_PATH *) PciDevicePath1)->Device << 8) | ((PCI_DEVICE_PATH *) PciDevicePath1)->Function)) {
      break;
    }
  }

  if (Entry == ChildList) {
    InsertTailList (ChildList, ChildLink);
  } else {
    Entry->BackLink->ForwardLink = &PciDeviceInfo1->ChildLink;
    PciDeviceInfo1->ChildLink.BackLink = Entry->BackLink;
    Entry->BackLink = &PciDeviceInfo1->ChildLink;
    PciDeviceInfo1->ChildLink.ForwardLink = Entry;
  }
}

/**
  Helper function to build relationships of all PCI Device Info in the pool.

**/
VOID
BuildRelationShips (
  LIST_ENTRY  *ListHead
  )
{
  LIST_ENTRY        *Entry1;
  LIST_ENTRY        *Entry2;
  PCI_DEVICE_INFO   *PciDeviceInfo1;
  PCI_DEVICE_INFO   *PciDeviceInfo2;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  BASE_LIST_FOR_EACH (Entry1, ListHead) {
    PciDeviceInfo1 = PCI_DEVICE_INFO_FROM_LINK (Entry1);
    if (!PciDeviceInfo1->IsRootBridge && !PciDeviceInfo1->IsBridge) {
      continue;
    }
    BASE_LIST_FOR_EACH (Entry2, ListHead) {
      if (Entry2 == Entry1) {
        continue;
      }
      PciDeviceInfo2 = PCI_DEVICE_INFO_FROM_LINK (Entry2);
      if (!PciDeviceInfo2->IsRootBridge &&
          PciDeviceInfo2->Parent == NULL &&
          IsChildDevicePath (
            PciDeviceInfo1->DevicePath,
            PciDeviceInfo2->DevicePath
            )) {
        InsertChildAfterSort (&PciDeviceInfo2->ChildLink, &PciDeviceInfo1->ChildList);
        PciDeviceInfo2->Parent = PciDeviceInfo1;
      }
    }
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Helper function to print indent by level.

  @param[in]  Level   The integer that indicates the recursion level.

**/
VOID
PrintIndentByLevel (
  IN  UINTN   Level
  )
{
  UINTN Index;

  for (Index = 0; Index < Level; Index++) {
    DEBUG ((EFI_D_INFO, "  "));
  }
}

/**
  Helper function to traverse all the PCI Device Info below
  the PCI Root Bridge or the PCI Bridge.

  @param[in]  PciDeviceInfo   The pointer to the PCI Device Info for traverse.
  @param[in]  Level           The integer that indicates the recursion level.

**/
VOID
Traverse (
  IN  PCI_DEVICE_INFO *PciDeviceInfo,
  IN  UINTN           Level
  )
{
  CHAR16          *DevicePathStr;
  LIST_ENTRY      *ChildLink;
  PCI_DEVICE_INFO *ChildPciDeviceInfo;

  if (PciDeviceInfo == NULL) {
    return;
  }

  DevicePathStr = ConvertDevicePathToText (PciDeviceInfo->DevicePath, TRUE, FALSE);
  PrintIndentByLevel (Level);
  DEBUG ((EFI_D_INFO, "%s\n", DevicePathStr));
  FreePool (DevicePathStr);

  if (!IsListEmpty (&PciDeviceInfo->ChildList)) {
    BASE_LIST_FOR_EACH (ChildLink, &PciDeviceInfo->ChildList) {
      ChildPciDeviceInfo = PCI_DEVICE_INFO_FROM_CHILD_LINK (ChildLink);
      Traverse (ChildPciDeviceInfo, Level + 1);
    }
  }
}

/**
  Helper function to list all the PCI Device Info excepts the PCI Root Bridges.

**/
VOID
ListPciDevices (
  IN  LIST_ENTRY  *ListHead
  )
{
  LIST_ENTRY        *Entry;
  PCI_DEVICE_INFO   *PciDeviceInfo;
  CHAR16            *DevicePathStr;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  DEBUG ((EFI_D_INFO, "Base Address       VendorId   DeviceId   PcieCapOffset   DevicePath\n"));
  DEBUG ((EFI_D_INFO, "-----------------+----------+----------+---------------+-----------------------------------------\n"));
  BASE_LIST_FOR_EACH (Entry, ListHead) {
    PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
    if (PciDeviceInfo->IsRootBridge) {
      continue;
    }
    DevicePathStr = ConvertDevicePathToText (PciDeviceInfo->DevicePath, TRUE, FALSE);
    DEBUG ((EFI_D_INFO, "%016llx   %04xh      %04xh      %02xh             %s\n",
      PciDeviceInfo->BaseAddress,
      PciDeviceInfo->PciConfigSpace.Hdr.VendorId,
      PciDeviceInfo->PciConfigSpace.Hdr.DeviceId,
      PciDeviceInfo->PcieCapOffset,
      DevicePathStr
      ));
    FreePool (DevicePathStr);
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Helper function to initialize the PCI Device Pool.

**/
VOID
InitializePciDeviceInfoPool (
  VOID
  )
{
  EFI_STATUS        Status;
  UINTN             NoHandles;
  EFI_HANDLE        *HandleBuffer;
  UINTN             Index;
  PCI_DEVICE_INFO   *PciDeviceInfo;
  LIST_ENTRY        *Entry;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  //
  // Find Root Bridges
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to LocateHandleBuffer, Status: %r\n", __FUNCTION__, Status));
    return;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    Status = InitializePciDeviceInfo (HandleBuffer[Index], &PciDeviceInfo);
    if (!EFI_ERROR (Status)) {
      InsertAfterSort (&mPrivateData.PciDeviceInfoPool, PciDeviceInfo);
    }
  }

  FreePool (HandleBuffer);

  //
  // Find PCI I/O Devices
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to LocateHandleBuffer, Status: %r\n", __FUNCTION__, Status));
    return;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    Status = InitializePciDeviceInfo (HandleBuffer[Index], &PciDeviceInfo);
    if (!EFI_ERROR (Status)) {
      InsertAfterSort (&mPrivateData.PciDeviceInfoPool, PciDeviceInfo);
    }
  }

  FreePool (HandleBuffer);

  BuildRelationShips (&mPrivateData.PciDeviceInfoPool);

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_INFO, "Traverse() - Start\n"));
  BASE_LIST_FOR_EACH (Entry, &mPrivateData.PciDeviceInfoPool) {
    PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
    if (PciDeviceInfo->IsRootBridge) {
      Traverse (PciDeviceInfo, 0);
    }
  }
  DEBUG ((EFI_D_INFO, "Traverse() - End\n"));

  ListPciDevices (&mPrivateData.PciDeviceInfoPool);
  DEBUG_CODE_END ();

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Callback function for check point gH2OBdsCpConnectAllAfterGuid.

  This function will build private data for H2O SATA Drive Info Protocol and install it.

  @param[in]  Event   A pointer to the Event that triggered the callback.
  @param[in]  Handle  Checkpoint handle.

**/
VOID
H2OBdsCpConnectAllAfterCallback (
  IN  EFI_EVENT       Event,
  IN  H2O_CP_HANDLE   Handle
  )
{
  EFI_STATUS  Status;

  DEBUG ((EFI_D_INFO, "PciDeviceInfo: %a() - Start\n", __FUNCTION__));

  H2OCpUnregisterHandler (Handle);

  InitDeviceInfoLookupProtocol (&mDeviceInfoLookup);

  InitializePciDeviceInfoPool ();

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivateData.Handle,
                  &gH2OPciDeviceInfoProtocolGuid,
                  (VOID *) &mPrivateData.H2oPciDeviceInfo,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to InstallMultipleProtocolInterfaces, Status: %r\n", __FUNCTION__, Status));
  }

  DEBUG ((EFI_D_INFO, "PciDeviceInfo: %a() - End\n", __FUNCTION__));
}

/**
  The Entry Point of PciDeviceInfoDxe.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurred when executing this entry point.

**/
EFI_STATUS
EFIAPI
PciDeviceInfoDxeEntry (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS      Status;
  H2O_CP_HANDLE   H2OBdsCpConnectAllAfterHandle;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  if (FeaturePcdGet (PcdH2OBdsCpConnectAllAfterSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpConnectAllAfterGuid,
               H2OBdsCpConnectAllAfterCallback,
               H2O_CP_MEDIUM,
               &H2OBdsCpConnectAllAfterHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to register checkpoint: %g, Status: %r\n", __FUNCTION__, &gH2OBdsCpConnectAllAfterGuid, Status));
      return Status;
    }
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}