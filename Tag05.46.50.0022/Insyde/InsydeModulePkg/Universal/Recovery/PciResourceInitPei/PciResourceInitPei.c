/** @file
  Implementation of PciResourceInitPei module for Crisis Recovery

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "PciResourceInitPei.h"
#include "XhciInit.h"
#include "AhciInit.h"
#include "NvmeInit.h"
#include "SdInit.h"
#include "UfsInit.h"

CONTROLLER_INIT_ROUTINE      mPciControllerInitRoutineTable[] = {
  InitXhciController,
  InitAhciController,
  InitNvmeController,
  InitSdController,
  InitUfsController,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR    mBridgeNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  BridgeEndOfPeiPpiNotifyCallback
};

LIST_ENTRY                   mBridgeListHead;
CHAR8                        *mPciBarAttribName[] = {
  ATTR_NAME_PCIE_PEI_BAR0,
  ATTR_NAME_PCIE_PEI_BAR1,
  ATTR_NAME_PCIE_PEI_BAR2,
  ATTR_NAME_PCIE_PEI_BAR3,
  ATTR_NAME_PCIE_PEI_BAR4,
  ATTR_NAME_PCIE_PEI_BAR5
};

/**
  Stop the bridge.
 
  @param [in]   Bus                      PCI Bus number.
  @param [in]   Device                   PCI Device number.
  @param [in]   Function                 PCI Function number.

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
VOID
StopP2pBridge (
  IN     UINT32                       Bus,
  IN     UINT32                       Device,
  IN     UINT32                       Function
  )
{
  //
  // Disable bus master
  //
  PciAnd16 (
    PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET),
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  //
  // Clear memory base
  //
  PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_BASE_ADDRESS_REGISTER_OFFSET), 0);
  PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_BASE_ADDRESS_REGISTER_OFFSET + 4), 0);

  //
  // Clear bus number
  //
  PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0);

}

/**
  Enable the bridge.

  @param [in]   Bus                      PCI Bus number.
  @param [in]   Device                   PCI Device number.
  @param [in]   Function                 PCI Function number.
  @param [in]   SecondaryBus             Secondary Bus number.
  @param [in]   SubordinateBus           Subordinate Bus number.
  @param [in]   MemBase                  Memory base address.
  @param [in]   MemLimit                 Memory base limit.

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
InitP2pBridge (
  IN  UINT32                       Bus,
  IN  UINT32                       Device,
  IN  UINT32                       Function,
  IN  UINT32                       SecondaryBus,
  IN  UINT32                       SubordinateBus,
  IN  UINT32                       MemBase,
  IN  UINT32                       MemLimit
  )
{
  UINT32                    Register;

  if (PciRead16 (PCI_LIB_ADDRESS (Bus, Device, 0, PCI_VENDOR_ID_OFFSET)) == 0xFFFF) {
    return EFI_NOT_FOUND;
  }

  //
  // Assign bus numbers
  //
  PciWrite8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), (UINT8)Bus);
  PciWrite8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET), (UINT8)SecondaryBus);
  PciWrite8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET), (UINT8)SubordinateBus);

  //
  // Assign base address
  //
  Register = ((MemBase & 0xFFF00000) >> 16) + ((MemBase + MemLimit - 1) & 0xFFF00000);
  PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_BASE_ADDRESS_REGISTER_OFFSET), Register);

  //
  // Set prefetchable BASE to be larger than LIMIT register since we are not use prefetchable memory in recovery
  //
  Register = 0xFFFF;
  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_PREFETCHABLE_BASE_ADDRESS_REGISTER_OFFSET), (UINT16)Register);
  Register = 0x0;
  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_PREFETCHABLE_BASE_LIMIT_REGISTER_OFFSET), (UINT16)Register);

  Register = 0xFFFFFFFF;
  PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_PREFETCHABLE_UPPER32BIT_BASE_ADDRESS_REGISTER_OFFSET), Register);
  Register = 0x0;
  PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_PREFETCHABLE_UPPER32BIT_BASE_LIMIT_REGISTER_OFFSET), Register);


  //
  // Enable bus master
  //
  PciOr16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));

  //
  // A config write is required in order for the device to re-capture the Bus number,
  // according to PCI Express Base Specification, 2.2.6.2
  // Write to a read-only register VendorID to not cause any side effects.
  //
  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_VENDOR_ID_OFFSET), 0);

  return EFI_SUCCESS;
}

/**
  Find the first child node.
 
  @param [in] ListHead                 List head.
  @param [in] ParentLink               Parent's link.

  @retval Child info.

**/
PCI_INFO_NODE *
GetChildInfo (
  IN     LIST_ENTRY                *ListHead,
  IN     LIST_ENTRY                *ParentLink
  )
{
  LIST_ENTRY                *Link;
  PCI_INFO_NODE             *PciInfo;
  PCI_INFO_NODE             *ParentInfo;

  ParentInfo = PCI_INFO_NODE_FROM_LINK (ParentLink);
  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
    PciInfo    = PCI_INFO_NODE_FROM_LINK (Link);
    if (PciInfo->Parent == ParentInfo) {
      return PciInfo;
    }
  }

  return NULL;
}
/**
  Find the child nodes.
 
  @param [in]  ListHead                 List head.
  @param [in]  ParentLink               Parent's link.
  @param [out] Child                    Child node pointer list.
  @param [out] ChildNum                 Child node number.

  @retval EFI_SUCCESS            Child nodes found.
  @retval EFI_INVALID_PARAMETER  Invalid input.
  @retval EFI_OUT_OF_RESOURCES   Can't allocate enough memory.

**/
EFI_STATUS
GetChildInfoBuffer (
  IN     LIST_ENTRY                *ListHead,
  IN     LIST_ENTRY                *ParentLink,
     OUT PCI_INFO_NODE_PTR         **Child,
     OUT UINT32                    *ChildNum
  )
{
  LIST_ENTRY                *Link;
  PCI_INFO_NODE             *PciInfo;
  UINT32                    Count;
  PCI_INFO_NODE             *ParentInfo;

  if (ChildNum == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Child == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ParentInfo = PCI_INFO_NODE_FROM_LINK (ParentLink);
  Count = 0;
  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
    PciInfo = PCI_INFO_NODE_FROM_LINK (Link);
    if (PciInfo->Parent == ParentInfo) {
      Count++;
    }
  }

  if (Count == 0) {
    *Child = NULL;
    *ChildNum = 0;
    return EFI_NOT_FOUND;
  }

  *Child = AllocateZeroPool (sizeof (PCI_INFO_NODE *) * Count);
  if (*Child == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *ChildNum = 0;
  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
    PciInfo = PCI_INFO_NODE_FROM_LINK (Link);
    if (PciInfo->Parent == ParentInfo) {
      (*Child)[(*ChildNum)++] = PciInfo;
      if (*ChildNum == Count) {
        break;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Check if it is supported device type.
 
  @param [in]   Bus                      PCI Bus number.
  @param [in]   Device                   PCI Device number.
  @param [in]   Function                 PCI Function number.

  @retval Whether the device type is supported

**/
BOOLEAN
IsSupportedDevice (
  IN     UINT8                        Bus,
  IN     UINT8                        Device,
  IN     UINT8                        Function
  )
{
  UINT8                            ProgIf;
  UINT8                            BaseClass;
  UINT8                            SubClass;

  ProgIf    = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET));
  SubClass  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1));
  BaseClass = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2));

  // XHCI
  if ((ProgIf == PCI_IF_XHCI) && (SubClass == PCI_CLASS_SERIAL_USB) && (BaseClass == PCI_CLASS_SERIAL)) {
    if (!CheckUsbPolicy ()) {
      return FALSE;
    }
    return TRUE;
  }
  // AHCI
  if ((SubClass == PCI_CLASS_MASS_STORAGE_SATADPA) && (BaseClass == PCI_CLASS_MASS_STORAGE)) {
    return TRUE;
  }
  // NVME
  if ((SubClass == PCI_CLASS_MASS_STORAGE_SOLID_STATE) && (BaseClass == PCI_CLASS_MASS_STORAGE)) {
    return TRUE;
  }
  // SD
  if ((SubClass == PCI_SUBCLASS_SD_HOST_CONTROLLER) && (BaseClass == PCI_CLASS_SYSTEM_PERIPHERAL)) {
    return TRUE;
  }
  // UFS
  if ((SubClass == 0x09) && (BaseClass == PCI_CLASS_MEMORY_CONTROLLER)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Update related PCI infos.

  @param [in]     ListHead       The List head.
  @param [in,out] NewPciInfo     The new PCI info node.

  @retval The subordinate bus number

**/
UINT8
UpdatePciTopology (
  IN     LIST_ENTRY                *ListHead,
  IN OUT PCI_INFO_NODE             *NewPciInfo
  )
{
  EFI_STATUS                       Status;
  LIST_ENTRY                       *Link;
  LIST_ENTRY                       *NextLink;
  PCI_INFO_NODE                    *PciInfo;
  UINT32                           MemBase;
  UINT32                           MemLimit;
  UINT8                            SecondaryBus;
  UINT8                            SubordinateBus;
  PCI_INFO_NODE                    *ChildInfo;
  UINT32                           ChildNum;
  UINT32                           Index;
  PCI_INFO_NODE                    *NewChildInfo;
  PCI_INFO_NODE                    *ParentInfo;
  BOOLEAN                          UpdateUpstream;
  UINT8                            MaxSubordinateBus;
  PCI_INFO_NODE_PTR                *ChildInfoBuffer;

  UpdateUpstream = FALSE;
  MemBase        = NewPciInfo->MemBase;
  MemLimit       = NewPciInfo->MemLimit;
  SecondaryBus   = NewPciInfo->SecondaryBus;
  SubordinateBus = NewPciInfo->SubordinateBus;
  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = NextLink) {
    NextLink = GetNextNode (ListHead, Link);
    PciInfo = PCI_INFO_NODE_FROM_LINK (Link);
    if ((PciInfo->PciRes.Bus == NewPciInfo->PciRes.Bus) &&
        (PciInfo->PciRes.Device == NewPciInfo->PciRes.Device) &&
        (PciInfo->PciRes.Function == NewPciInfo->PciRes.Function)) {
      //
      // Identical device found.
      //
      Status = GetChildInfoBuffer (ListHead, Link, &ChildInfoBuffer, &ChildNum);
      if (!EFI_ERROR (Status)) {
        MemBase         = MIN (NewPciInfo->MemBase, PciInfo->MemBase);
        MemLimit        = MAX (NewPciInfo->MemBase + NewPciInfo->MemLimit, PciInfo->MemBase + PciInfo->MemLimit) - MemBase;
        SecondaryBus    = MAX (SecondaryBus, PciInfo->SecondaryBus);
        //
        // Update downstream nodes.
        //
        MaxSubordinateBus = NewPciInfo->SubordinateBus;
        for (Index = 0; Index < ChildNum; Index++) {
          ChildInfo = (PCI_INFO_NODE *)ChildInfoBuffer[Index];
          //
          // Create updated node to replace the existing one.
          //
          NewChildInfo = (PCI_INFO_NODE *)AllocateCopyPool (sizeof (PCI_INFO_NODE), ChildInfo);
          ASSERT (NewChildInfo != NULL);
          if (NewChildInfo == NULL) {
            continue;
          }
          NewChildInfo->PciRes.Bus = SecondaryBus;
          NewChildInfo->Parent     = NewPciInfo;
          if (NewChildInfo->IsP2pBridge) {
            //
            // Arrange bus number if needed.
            //
            ASSERT (ChildInfo->SecondaryBus > ChildInfo->PciRes.Bus);
            NewChildInfo->SecondaryBus = SecondaryBus + ChildInfo->SecondaryBus - ChildInfo->PciRes.Bus;
            if (NewChildInfo->SecondaryBus == NewPciInfo->SubordinateBus) {
              //
              // Bus number is duplicated, adding one as new bus number.
              //
              NewChildInfo->SecondaryBus = MaxSubordinateBus + 1;
              UpdateUpstream = TRUE;
            }
          }
          //
          // Assign new bus number.
          //
          ChildInfo->PciRes.Bus = NewChildInfo->PciRes.Bus;
          //
          // Update downstream recursively.
          //
          MaxSubordinateBus = UpdatePciTopology (ListHead, NewChildInfo);
          SubordinateBus    = MAX (SubordinateBus, MaxSubordinateBus);
        }
      }
      RemoveEntryList (Link);
    }
  }

  NewPciInfo->MemBase        = MemBase;
  NewPciInfo->MemLimit       = MemLimit;
  NewPciInfo->SecondaryBus   = SecondaryBus;
  NewPciInfo->SubordinateBus = SubordinateBus;
  InsertTailList (ListHead, &NewPciInfo->Link);

  if (UpdateUpstream) {
    //
    // Update upstream subordinate bus if child's subordinate bus is changed because of duplicated bus number.
    //
    ParentInfo = NewPciInfo->Parent;
    while (ParentInfo != NULL) {
      ParentInfo->SubordinateBus = SubordinateBus;
      ParentInfo = ParentInfo->Parent;
    }
  }

  return (NewPciInfo->IsP2pBridge) ? SubordinateBus : NewPciInfo->PciRes.Bus;
}

/**
  Find the MMIO size of the device requires

  @param [in]  Bus                      PCI Bus number.
  @param [in]  Device                   PCI Device number.
  @param [in]  Function                 PCI Function number.
  @param [out] MmioSize                 MMIO Size in bytes.

**/
VOID
FindDeviceMmioSize (
  IN     UINT8                        Bus,
  IN     UINT8                        Device,
  IN     UINT8                        Function,
     OUT UINT64                       *MmioSize
  )
{
  UINT32                           BarIndex;
  UINT32                           Size;
  UINT64                           CurrentSize;
  UINT32                           BarData;

  *MmioSize = 0;
  for (BarIndex = 0; BarIndex < PCI_MAX_BAR; BarIndex++) {
    //
    // Get MMIO region size.
    //
    BarData = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex)); // Backup Bar Data
    PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex), 0xFFFFFFFF);
    Size = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex));
    PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex), BarData); // Restore Bar Data
    switch (Size & 0x07) {

    case 0x0:
      //
      // Memory space: anywhere in 32 bit address space
      //
      *MmioSize += (~(Size & 0xFFFFFFF0)) + 1;
      break;

    case 0x4:
      //
      // Memory space: anywhere in 64 bit address space
      //
      CurrentSize = Size & 0xFFFFFFF0;
      BarData = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4)); // Backup Bar Data
      PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4), 0xFFFFFFFF);
      Size = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4));
      PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4), BarData); // Restore Bar Data
      //
      // Fix the length to support some spefic 64 bit BAR
      //
      Size |= ((UINT32)(-1) << HighBitSet32 (Size));
      //
      // Calculate the size of 64bit bar
      //
      CurrentSize |= LShiftU64 ((UINT64) Size, 32);
      CurrentSize  = (~(CurrentSize)) + 1;
      *MmioSize += CurrentSize;
      //
      // Skip next BAR
      //
      BarIndex++;
      break;
  
    default:
      //
      // Unknown BAR type
      //
      break;
    };
  }
}

/**
  Reset all bus number from specific bridge.

  @param[in] StartBusNumber   Start bus number.

**/
VOID
ResetAllBridgeBusNumber (
  IN UINT8                        StartBusNumber
  )
{
  UINT8                           Device;
  UINT8                           Function;
  UINTN                           PciAddress;
  UINT8                           BaseClass;
  UINT8                           SubClass;
  UINT8                           SecondaryBus;
  UINT8                           HeaderType;

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
      PciAddress = PCI_LIB_ADDRESS (StartBusNumber, Device, Function, 0);
      if (PciRead16 (PciAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
        if (Function == 0) {
          //
          // Skip sub functions
          //
          Function = PCI_MAX_FUNC;
        }
        continue;
      }

      SubClass   = PciRead8 (PciAddress + PCI_CLASSCODE_OFFSET + 1);
      BaseClass  = PciRead8 (PciAddress + PCI_CLASSCODE_OFFSET + 2);
      if ((BaseClass == PCI_CLASS_BRIDGE) && (SubClass == PCI_CLASS_BRIDGE_P2P)) {
        SecondaryBus  = PciRead8 (PciAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
        if (SecondaryBus != 0) {
          ResetAllBridgeBusNumber (SecondaryBus);
        }
        //
        // Reset register 18h, 19h, 1Ah on PCI Bridge
        //
        PciWrite8 (PciAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
        PciWrite8 (PciAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0);
        PciWrite8 (PciAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0);
      }

      HeaderType = PciRead8 (PciAddress + PCI_HEADER_TYPE_OFFSET);
      if ((Function == 0) && ((HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0)) {
        //
        // Skip sub functions, this is not a multi function device
        //
        Function = PCI_MAX_FUNC;
      }
    }
  }
}

/**
  Calculate total MMIO size of a PCI device needs.
  Enable the bridge if needed.

  @param [in] ListHead         List head.

  @retval Total MMIO size.

**/
UINT64
FindMmioSizeFromPciInfoList (
  IN LIST_ENTRY                    *ListHead
  )
{
  LIST_ENTRY                       *Link;
  PCI_INFO_NODE                    *PciInfo;
  PCI_INFO_NODE                    *ChildPciInfo;
  UINT64                           MmioSize;
  UINTN                            PciAddress;
  UINT8                            Bus;
  UINT8                            Device;
  UINT8                            Function;

  MmioSize = 0;
  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
    PciInfo = PCI_INFO_NODE_FROM_LINK (Link);
    Bus        = PciInfo->PciRes.Bus;
    Device     = PciInfo->PciRes.Device;
    Function   = PciInfo->PciRes.Function;
    PciAddress = PCI_LIB_ADDRESS (Bus, Device, Function , 0);

    ChildPciInfo = GetChildInfo (ListHead, Link);
    if (ChildPciInfo != NULL) {
      PciWrite8 (PciAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, (UINT8)Bus);
      PciWrite8 (PciAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, (UINT8)ChildPciInfo->PciRes.Bus);
      //
      // Assign temporary subordinate bus number so that device behind this bridge can be seen
      //
      PciWrite8 (PciAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0xFF);
      //
      // A config write is required in order for the device to re-capture the Bus number,
      // according to PCI Express Base Specification, 2.2.6.2
      // Write to a read-only register VendorID to not cause any side effects.
      //
      PciWrite16 (PciAddress + PCI_VENDOR_ID_OFFSET, 0);
      continue;
    }

    //
    // We've encountered hanging issue if accessing the specific controller during normal boot.
    // So check the supporting policy before mmio it.
    //
    if (IsSupportedDevice (Bus, Device, Function)) {
      FindDeviceMmioSize (Bus, Device, Function, &MmioSize);
    }
    break;
  }

  ResetAllBridgeBusNumber (0);

  return MmioSize;
}

/**
  Get PCI device info.

  @param [in] ListHead                 List head.
  @param [in] Bus                      PCI Bus number.
  @param [in] Device                   PCI Device number.
  @param [in] Function                 PCI Function number.

  @retval Device info pointer.

**/
PCI_INFO_NODE *
GetPciInfoFromList (
  IN     LIST_ENTRY                   *ListHead,
  IN     UINT8                        Bus,
  IN     UINT8                        Device,
  IN     UINT8                        Function
  )
{
  LIST_ENTRY                       *Link;
  PCI_INFO_NODE                    *PciInfo;

  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
    PciInfo = PCI_INFO_NODE_FROM_LINK (Link);
    if ((Bus == PciInfo->PciRes.Bus) &&
        (Device == PciInfo->PciRes.Device) &&
        (Function == PciInfo->PciRes.Function)) {
      return PciInfo;
    }
  }

  return NULL;
}

/**
  Returns the 16-bit Length field of a device path node.

  Returns the 16-bit Length field of the device path node specified by Node.
  Node is not required to be aligned on a 16-bit boundary, so it is recommended
  that a function such as ReadUnaligned16() be used to extract the contents of
  the Length field.

  If Node is NULL, then ASSERT().

  @param  Node      A pointer to a device path node data structure.

  @return The 16-bit Length field of the device path node specified by Node.

**/
STATIC
UINTN
DevicePathNodeLength (
  IN CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return ReadUnaligned16 ((UINT16 *)&((EFI_DEVICE_PATH_PROTOCOL *)(Node))->Length[0]);
}

/**
  Returns a pointer to the next node in a device path.

  Returns a pointer to the device path node that follows the device path node
  specified by Node.

  If Node is NULL, then ASSERT().

  @param  Node      A pointer to a device path node data structure.

  @return a pointer to the device path node that follows the device path node
  specified by Node.

**/
STATIC
EFI_DEVICE_PATH_PROTOCOL *
NextDevicePathNode (
  IN CONST VOID  *Node
  )
{
  ASSERT (Node != NULL);
  return (EFI_DEVICE_PATH_PROTOCOL *)((UINT8 *)(Node) + DevicePathNodeLength (Node));
}

/**
  Determines if a device path node is an end node of an entire device path.

  Determines if a device path node specified by Node is an end node of an entire
  device path.
  If Node represents the end of an entire device path, then TRUE is returned.
  Otherwise, FALSE is returned.

  If Node is NULL, then ASSERT().

  @param  Node      A pointer to a device path node data structure.

  @retval TRUE      The device path node specified by Node is the end of an entire device path.
  @retval FALSE     The device path node specified by Node is not the end of an entire device path.

**/
STATIC
BOOLEAN
IsDevicePathEnd (
  IN EFI_DEVICE_PATH_PROTOCOL  *Node
  )
{
  ASSERT (Node != NULL);
  return (BOOLEAN)((Node->Type == END_DEVICE_PATH_TYPE) && (Node->SubType == END_ENTIRE_DEVICE_PATH_SUBTYPE));
}

/**
  Get PCI root bus number.

  @param  DevicePath      A pointer to a device path data structure.

  @retval TRUE      The device path node specified by Node is the end of an entire device path.
  @retval FALSE     The device path node specified by Node is not the end of an entire device path.

**/
STATIC
UINT8
GetPciRootBus (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  //
  // Return 0 for now.
  //
  return 0;
}

/**
  Enumerate PCI devices from token space gH2ODeviceInfo2TokenSpaceGuid.

  @param [in,out] ListHead         List head.

  @retval The bottommost bus number.

**/
UINT8
EnumerateFromPcd (
  IN OUT LIST_ENTRY                    *ListHead
  )
{
  EFI_STATUS                      Status;
  UINTN                           TokenNum;
  LIST_ENTRY                      TempPciInfoHead;
  PCI_INFO_NODE                   *PciInfo;
  PCI_INFO_NODE                   *ChildPciInfo;
  PCI_INFO_NODE                   *TempPciInfo;
  PCI_DEVICE_PATH                 *PciDevicePath;
  LIST_ENTRY                      *Link;
  UINT8                           SecondaryBus;
  UINT8                           PrimaryBus;
  UINT64                          MmioSize;
  UINT8                           SubordinateBus;
  PCI_INFO_NODE                   *ParentInfo;
  UINT32                          DeviceAttribCount;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePathNode;
  UINT32                          Index;
  UINT32                          BarIndex;
  CHAR8                           *DeviceAttribValue;
  UINT32                          PciBar[6];
  UINT32                          NodeUid;
  UINT32                          MemBase;
  UINT32                          MemLimit;
  UINT8                           NewSubordinateBus;
  UINT8                           TempSubordinateBus;
  UINT8                           OrgSecondaryBus;

  PrimaryBus     = 0;
  SecondaryBus   = FIRST_DOWNSTREAM_NUM; // Downstream bus starts from 8
  SubordinateBus = 0;
  for (TokenNum = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, 0)
       ; TokenNum != 0
       ; TokenNum = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, TokenNum)
       ) {

    Status = H2OGetDeviceInfo (TokenNum, &DevicePath, &DeviceAttribCount);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (DevicePath == NULL) {
      continue;
    }

    //
    // pcie_pei_enable
    //
    Status = H2OGetDeviceInfoAttribByName (TokenNum, ATTR_NAME_PCIE_PEI_ENABLE, &DeviceAttribValue);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (AsciiStrCmp (DeviceAttribValue, ATTR_VALUE_CRISISRECOVERY) == 0) {
      if (GetBootModeHob () != BOOT_IN_RECOVERY_MODE) {
        continue;
      }
    } else if (AsciiStrCmp (DeviceAttribValue, ATTR_VALUE_YES) != 0) {
      continue;
    }

    //
    // pcie_pei_barx
    //
    ZeroMem (&PciBar, sizeof (PciBar));
    for (BarIndex = 0; BarIndex < ARRAY_SIZE (mPciBarAttribName); BarIndex++) {
      Status = H2OGetDeviceInfoAttribByName (TokenNum, mPciBarAttribName[BarIndex], &DeviceAttribValue);
      if (EFI_ERROR (Status)) {
        continue;
      }
      PciBar[BarIndex] = (UINT32)AsciiStrHexToUint64 (DeviceAttribValue);
    }

    //
    // Device is enabled.
    // Enumerate PCI devices from device path
    //
    InitializeListHead (&TempPciInfoHead);
    for (DevicePathNode = DevicePath, ParentInfo = NULL, OrgSecondaryBus = SecondaryBus
         ; !IsDevicePathEnd (DevicePathNode)
         ; DevicePathNode = NextDevicePathNode (DevicePathNode)
         ) {

      if (IS_ACPI_DP (DevicePathNode)) {
        NodeUid    = ((ACPI_HID_DEVICE_PATH *)DevicePathNode)->UID;
        PrimaryBus = GetPciRootBus (DevicePath);
        ParentInfo = NULL;
        continue;
      }

      if (IS_PCI_DP (DevicePathNode)) {
        PciDevicePath = (PCI_DEVICE_PATH *)DevicePathNode;

        TempPciInfo = (PCI_INFO_NODE *)AllocateZeroPool (sizeof (PCI_INFO_NODE));
        ASSERT (TempPciInfo != NULL);
        if (TempPciInfo == NULL) {
          return 0;
        }

        TempPciInfo->Signature       = PCI_INFO_NODE_SIGNATURE;
        TempPciInfo->Parent          = ParentInfo;
        TempPciInfo->PciRes.Bus      = PrimaryBus;
        TempPciInfo->PciRes.Device   = PciDevicePath->Device;
        TempPciInfo->PciRes.Function = PciDevicePath->Function;
        CopyMem (&TempPciInfo->PciRes.PciBar, &PciBar, sizeof (TempPciInfo->PciRes.PciBar));
        InsertTailList (&TempPciInfoHead, &TempPciInfo->Link);

        ParentInfo     = TempPciInfo;
        SubordinateBus = PrimaryBus;
        if (!IsDevicePathEnd (NextDevicePathNode (DevicePathNode))) {
          PrimaryBus = SecondaryBus;
          SecondaryBus++;
        }
      }
    }

    MemLimit = 0;
    MmioSize = FindMmioSizeFromPciInfoList (&TempPciInfoHead);
    if (MmioSize > 0) {
      //
      // Device is present
      // Find the required memery base & limit
      //
      for (Index = 0, MemBase = 0; Index < PCI_MAX_BAR; Index++) {
        if (PciBar[Index] == 0) {
          continue;
        }
        MemBase  = (MemBase == 0) ? PciBar[Index] : MIN (PciBar[Index], MemBase);
        MemLimit = MAX ((UINT32)MmioSize, PciBar[Index] - MemBase);
      }

      //
      // Record the device info.
      //
      for (Link = GetFirstNode (&TempPciInfoHead), ParentInfo = NULL, TempSubordinateBus = SubordinateBus
           ; !IsNull (&TempPciInfoHead, Link)
           ; Link = GetNextNode (&TempPciInfoHead, Link), ParentInfo = PciInfo
           ) {
        TempPciInfo = PCI_INFO_NODE_FROM_LINK (Link);
        PciInfo = (PCI_INFO_NODE *)AllocateCopyPool (sizeof (PCI_INFO_NODE), TempPciInfo);
        ASSERT (PciInfo != NULL);
        if (PciInfo == NULL) {
          return 0;
        }
        PciInfo->Parent = ParentInfo;
        ChildPciInfo = GetChildInfo (&TempPciInfoHead, Link);
        if (ChildPciInfo != NULL) {
          //
          // Assuming there is only one child in the temp linked list.
          //
          PciInfo->IsP2pBridge    = TRUE;
          PciInfo->SecondaryBus   = ChildPciInfo->PciRes.Bus;
          PciInfo->SubordinateBus = TempSubordinateBus;
          PciInfo->MemBase        = MemBase;
          PciInfo->MemLimit       = MemLimit;
        }
        //
        // The bridge is possibly enumerated before.
        // Update the existing info that is related to the bridge.
        //
        NewSubordinateBus = UpdatePciTopology (ListHead, PciInfo);
        if (NewSubordinateBus > SubordinateBus) {
          SubordinateBus = NewSubordinateBus;
          SecondaryBus   = SubordinateBus + 1;
        }
      }
    } else {
      SecondaryBus = OrgSecondaryBus;
    }
  }

  return (SecondaryBus  == FIRST_DOWNSTREAM_NUM) ? 0 : SecondaryBus;
}

/**
  Program BAR register for PCI device.

  @param [in]   Bus                      PCI Bus number.
  @param [in]   Device                   PCI Device number.
  @param [in]   Function                 PCI Function number.
  @param [in]   BarIndex                 Index of BAR.
  @param [in]   BaseAddress              Bass address.
  @param [out]  MmioSize                 MMIO size.
 
  @retval EFI_SUCCESS     Operation completed successfully.
  @retval Others          Operation failed.

**/
EFI_STATUS
ProgramBar (
  IN      PCI_RESOURCE_DATA         *PciRes
  )
{
  UINT32                       Size;
  UINT64                       MmioSize;
  UINT32                       BarIndex;
  UINT32                       BaseAddress;
  UINT8                        Bus;
  UINT8                        Device;
  UINT8                        Function;

  if (FeaturePcdGet (PcdH2OPeiCpPciEnumUpdateDevResourcesSupported)) {
    H2O_PEI_CP_PCI_ENUM_UPDATE_DEV_RESOURCES        CpPciEnumUpdateDevResourceData;

    CpPciEnumUpdateDevResourceData.Size        = sizeof (H2O_PEI_CP_PCI_ENUM_UPDATE_DEV_RESOURCES);
    CpPciEnumUpdateDevResourceData.Status      = H2O_CP_TASK_NORMAL;
    CpPciEnumUpdateDevResourceData.NodeUid     = PciRes->NodeUid;
    CpPciEnumUpdateDevResourceData.Bus         = PciRes->Bus;
    CpPciEnumUpdateDevResourceData.Device      = PciRes->Device;
    CpPciEnumUpdateDevResourceData.Function    = PciRes->Function;
    CopyMem ((VOID *)&CpPciEnumUpdateDevResourceData.PciBar, (VOID *)&PciRes->PciBar, sizeof (CpPciEnumUpdateDevResourceData.PciBar));

    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OPeiCpPciEnumUpdateDevResourcesGuid));
    H2OCpTrigger (&gH2OPeiCpPciEnumUpdateDevResourcesGuid, &CpPciEnumUpdateDevResourceData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", CpPciEnumUpdateDevResourceData.Status));
    if (CpPciEnumUpdateDevResourceData.Status == H2O_CP_TASK_SKIP) {
      return EFI_SUCCESS;
    } else if (CpPciEnumUpdateDevResourceData.Status == H2O_BDS_TASK_UPDATE) {
      CopyMem ((VOID *)&PciRes->PciBar, (VOID *)&CpPciEnumUpdateDevResourceData.PciBar, sizeof (PciRes->PciBar));
    }
  }

  Bus      = PciRes->Bus;
  Device   = PciRes->Device;
  Function = PciRes->Function;

  for (BarIndex = 0; BarIndex < PCI_MAX_BAR; BarIndex++) {
    BaseAddress = PciRes->PciBar[BarIndex];
    if (BaseAddress == 0) {
      continue;
    }
    //
    // Get MMIO region size.
    //
    PciAnd16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
    PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex), 0xFFFFFFFF);
    Size = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex));

    switch (Size & 0x07) {

    case 0x0:
      DEBUG ((DEBUG_INFO, "PciResourceInitPei - ProgramBar -- BAR[%x] 32-bit memory space\n", BarIndex));
      //
      // Memory space: anywhere in 32 bit address space
      //
      MmioSize = (~(Size & 0xFFFFFFF0)) + 1;
      break;

    case 0x4:
      DEBUG ((DEBUG_INFO, "PciResourceInitPei - ProgramBar -- BAR[%x] 64-bit memory space\n", BarIndex));
      //
      // Memory space: anywhere in 64 bit address space
      //
      MmioSize = Size & 0xFFFFFFF0;
      PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4), 0xFFFFFFFF);
      Size = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4));
      //
      // Fix the length to support some spefic 64 bit BAR
      //
      Size |= ((UINT32)(-1) << HighBitSet32 (Size));
      //
      // Calculate the size of 64bit bar
      //
      MmioSize  |= LShiftU64 ((UINT64) Size, 32);
      MmioSize  = (~(MmioSize)) + 1;
      //
      // Clean the high 32bits of this 64bit BAR to 0 as we only allow a 32bit BAR.
      //
      PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex + 4), 0);
      break;

    default:
      DEBUG ((DEBUG_INFO, "PciResourceInitPei - ProgramBar -- BAR[%x] unknown BAR type\n", BarIndex));
      //
      // Unknown BAR type
      //
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    };
    //
    // Assign resource to the SdMmc Pci host controller's MMIO BAR.
    // Enable the SdMmc Pci host controller by setting BME and MSE bits of PCI_CMD register.
    //
    PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4 * BarIndex), BaseAddress);
    PciOr16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));

    DEBUG ((DEBUG_INFO, "PciResourceInitPei - ProgramBar -- BAR[%x]=0x%x, Size=0x%x\n", BarIndex, BaseAddress, MmioSize));
  }

  return EFI_SUCCESS;
}

/**
  Register notify ppi to reset the bridges.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in] Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EFIAPI
BridgeEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  LIST_ENTRY                       *Link;
  PCI_INFO_NODE                    *PciInfo;

  for (Link = GetFirstNode (&mBridgeListHead)
       ; !IsNull (&mBridgeListHead, Link)
       ; Link = GetNextNode (&mBridgeListHead, Link)
       ) {
    PciInfo = PCI_INFO_NODE_FROM_LINK (Link);
    StopP2pBridge (PciInfo->PciRes.Bus, PciInfo->PciRes.Device, PciInfo->PciRes.Function);
  }

  return EFI_SUCCESS;
}

/**
  PEIM entry.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized
  @retval Others                 Can't initialize the driver

**/
EFI_STATUS
EFIAPI
PeimInitializePciResourceInit (
  IN       EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES           **PeiServices
  )
{
  EFI_STATUS                       Status;
  UINT8                            Bus;
  UINT8                            Device;
  UINT8                            Function;
  UINT8                            BaseClass;
  UINT8                            SubClass;
  CONTROLLER_INIT_ROUTINE          *Routine;
  LIST_ENTRY                       ListHead;
  PCI_INFO_NODE                    *PciInfo;
  PCI_INFO_NODE                    *BridgeInfo;
  UINT8                            MaxBus;

  //
  // Shadow this PEIM to run from memory
  //
  if (!EFI_ERROR (PeiServicesRegisterForShadow (FileHandle))) {
    return EFI_SUCCESS;
  }

  if (LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, 0) == 0) {
    return EFI_UNSUPPORTED;
  }

  InitializeListHead (&mBridgeListHead);
  InitializeListHead (&ListHead);

  MaxBus = EnumerateFromPcd (&ListHead);

  for (Bus = 0; Bus <= MaxBus; Bus++) {
    for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
      for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
        PciInfo = GetPciInfoFromList (&ListHead, Bus, Device, Function);
        if (PciInfo == NULL) {
          continue;
        }

        SubClass  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1));
        BaseClass = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2));
        if ((BaseClass == PCI_CLASS_BRIDGE) && (SubClass == PCI_CLASS_BRIDGE_P2P)) {
          Status = InitP2pBridge (
                     Bus,
                     Device,
                     Function,
                     PciInfo->SecondaryBus,
                     PciInfo->SubordinateBus,
                     PciInfo->MemBase,
                     PciInfo->MemLimit
                     );
          if (EFI_ERROR (Status)) {
            continue;
          }
          //
          // Stop the bridge at End of PEI
          //
          BridgeInfo = (PCI_INFO_NODE *)AllocateCopyPool (sizeof (PCI_INFO_NODE), (VOID *)PciInfo);
          ASSERT (BridgeInfo != NULL);
          InsertTailList (&mBridgeListHead, &BridgeInfo->Link);
        } else if (IsSupportedDevice (Bus, Device, Function)) {
          Routine = &mPciControllerInitRoutineTable[0];
          while (*Routine != NULL) {
            Status = (*Routine) (
                       PeiServices,
                       &PciInfo->PciRes
                       );
            if (!EFI_ERROR (Status) || (EFI_ERROR (Status) && (Status != EFI_UNSUPPORTED))) {
              break;
            }
            Routine++;
          }
        }
      }
    }
  }

  if (!IsListEmpty (&mBridgeListHead)) {
    //
    // Stop PCI bridges at End of PEI
    //
    Status = PeiServicesNotifyPpi (&mBridgeNotifyList);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}
