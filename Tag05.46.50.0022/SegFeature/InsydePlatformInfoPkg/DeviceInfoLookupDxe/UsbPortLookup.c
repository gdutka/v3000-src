/** @file


;******************************************************************************
;* Copyright (c) 2023 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/H2ODeviceInfoLookup.h> // InsydePlatformInfoPkg

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DeviceInfoLookupDataLib.h>  // InsydePlatformInfoPkg
#include <Library/UefiLib.h>

#include "DeviceInfoLookupDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT

/**

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
GetUsbHcDevicePathByNumber (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN UINT16                           HostControllerNumber,
  OUT EFI_DEVICE_PATH_PROTOCOL        **HostControllerPath
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath;

  if (This == NULL || HostControllerPath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  *HostControllerPath = NULL;

  if (HostControllerNumber == 0 ||
      HostControllerNumber > PrivateData->UsbHostControllerCount)
  {
    return EFI_NOT_FOUND;
  }
  DevicePath = PrivateData->UsbHcDevicePaths[HostControllerNumber - 1];
  *HostControllerPath = DuplicateDevicePath (DevicePath);
  if (*HostControllerPath == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}

/**

  @param[in]  PrivateData
  @param[in]  PortNumberChain

**/
STATIC
CONST H2O_USB_PORT_INFO_TRIE_NODE *
LookupUsbPortInfoTrieNode (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST USB_PORT_NUMBER_CHAIN          *PortNumberChain,
  OUT UINT32                              AcpiNames[USB_PORT_MAX_DEPTH]
  )
{
  H2O_USB_PORT_INFO_TRIE_NODE  *NodeArray;
  H2O_USB_PORT_INFO_TRIE_NODE  *Node;
  UINT8                        Depth;
  UINT8                        PortNumber;

  if ((PrivateData == NULL) || (PortNumberChain == NULL) || (AcpiNames == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/PortNumberChain/AcpiNames is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  if (PortNumberChain->HostControllerNumber == 0 ||
      PortNumberChain->HostControllerNumber >
      PrivateData->UsbHostControllerCount)
  {
    return NULL;
  }
  NodeArray = PrivateData->UsbPortInfoTrieNodes;
  Node = &NodeArray[PortNumberChain->HostControllerNumber - 1];
  ASSERT (Node->AcpiName == 0);

  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
    AcpiNames[Depth] = 0;

    PortNumber = PortNumberChain->UsbPortNumbers[Depth];
    if (PortNumber <= 0) {
      break;
    }

    if (PortNumber > Node->MaxDownstreamPortNumber) {
      Node = NULL;
      break;
    }
    Node = &NodeArray[Node->DownstreamNodeStartIndex + PortNumber - 1];
    AcpiNames[Depth] = Node->AcpiName;
  }
  return Node;
}

/**

  @implements H2O_LOOKUP_USB_PORT_INFO_BY_NUMBER_CHAIN

  @param[in]  This
  @param[in]  UsbPortNumberChain
  @param[out] PcdTokenNumber
  @param[out] AcpiPath
  @param[out] DisplayName
  @param[out] DownstreamPortCount

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
GetUsbPortInfoByNumberChain (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL,
  OUT UINT8                           *DownstreamPortCount OPTIONAL
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  CONST H2O_USB_PORT_INFO_TRIE_NODE    *Node;
  UINT32                               AcpiNames[USB_PORT_MAX_DEPTH];
  CONST CHAR16                         *NameString;

  if (This == NULL || UsbPortNumberChain == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  if (PcdTokenNumber != NULL) {
    *PcdTokenNumber = 0;
  }
  if (AcpiPath != NULL) {
    *AcpiPath = NULL;
  }
  if (DisplayName != NULL) {
    *DisplayName = NULL;
  }
  if (DownstreamPortCount != NULL) {
    *DownstreamPortCount = 0;
  }

  Node = LookupUsbPortInfoTrieNode (
           PrivateData,
           UsbPortNumberChain,
           AcpiNames
           );
  if (Node == NULL) {
    return EFI_NOT_FOUND;
  }

  if (PcdTokenNumber != NULL) {
    *PcdTokenNumber = Node->PcdTokenNumber;
  }
  if (DownstreamPortCount != NULL) {
    *DownstreamPortCount = Node->MaxDownstreamPortNumber;
  }
  if (AcpiPath != NULL) {
    // Not implemented yet
  }
  if (DisplayName != NULL &&
      Node->DisplayNameIndex < PrivateData->DisplayNameStringCount)
  {
    NameString = PrivateData->DisplayNameStrings[Node->DisplayNameIndex];
    *DisplayName = CatSPrint (NULL, L"%s", NameString);
    if (*DisplayName == NULL) {
      goto DisplayNameFail;
    }
  }
  return EFI_SUCCESS;

DisplayNameFail:
  if (AcpiPath != NULL) {
    FreePoolIfNonNull (*AcpiPath);
    *AcpiPath = NULL;
  }
//AcpiPathFail:
  return EFI_OUT_OF_RESOURCES;
}

/**
  Converts an EFI Device Path of a USB device into a USB_PORT_NUMBER_CHAIN
  structure, and additional information such as USB Interface Number and byte
  offset from DevicePath to the first USB Device Path node.

  @param[in]  PrivateData
  @param[in]  DevicePath
  @param[out] PortNumberChain
  @param[out] InterfaceNumber
  @param[out] UsbDevicePathOffset

  @return  Void.

**/
STATIC
VOID
InternalDevicePathToUsbPortNumberChain (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  OUT USB_PORT_NUMBER_CHAIN               *PortNumberChain,
  OUT UINT8                               *InterfaceNumber,
  OUT UINTN                               *UsbDevicePathOffset,
  OUT UINT8                               *UsbPortDepth
  )
{
  USB_PORT_NUMBER_CHAIN           ZeroPortNumberChainEntry = {0};
  UINT16                          Index;
  CONST EFI_DEVICE_PATH_PROTOCOL  *HcDevicePath;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  CONST USB_DEVICE_PATH           *UsbDevicePathNode;
  UINT8                           Depth;

  if ((PrivateData == NULL) || (DevicePath == NULL) || (PortNumberChain == NULL) ||
      (InterfaceNumber == NULL) || (UsbDevicePathOffset == NULL) || (UsbPortDepth == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/DevicePath/PortNumberChain/InterfaceNumber/UsbDevicePathOffset/UsbPortDepth is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  *PortNumberChain = ZeroPortNumberChainEntry;
  *InterfaceNumber = 0;
  *UsbPortDepth = 0;

  //
  // Decompose the Device Path into a host controller part and a chain of USB
  // Device Path nodes. If no USB Device Path node is found, the entire path
  // should be treated as a host controller.
  //
  // ((CONST UINT8 *) DevicePath + *UsbDevicePathOffset) will point to either
  // the start of the USB Device Path node or, for a host controller, the End
  // Device Path node.
  //
  (VOID) FindDevicePathNodeByType (
           DevicePath,
           MESSAGING_DEVICE_PATH,
           MSG_USB_DP,
           (UINT16) sizeof (USB_DEVICE_PATH),
           UsbDevicePathOffset
           );

  //
  // Host controller number
  //
  for (Index = 0; Index < PrivateData->UsbHostControllerCount; Index++) {
    if (PrivateData->UsbHcDevicePaths == NULL) {
      continue;
    }
    HcDevicePath = PrivateData->UsbHcDevicePaths[Index];
    if (CompareMem (DevicePath, HcDevicePath, *UsbDevicePathOffset) != 0) {
      continue;
    }
    if (IsDevicePathEnd ((UINT8 *) HcDevicePath + (*UsbDevicePathOffset))) {
      break;
    }
  }
  ASSERT (PortNumberChain->HostControllerNumber == 0);
  if (Index < PrivateData->UsbHostControllerCount) {
    PortNumberChain->HostControllerNumber = Index + 1;
  }

  //
  // USB port numbers and Interface number
  // Note: We can't tolerate bad length of a USB Device Path node.
  //
  DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *) (
                     (CONST UINT8 *) DevicePath + (*UsbDevicePathOffset));
  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
    //
    // USB specification requires hubs to support only one Interface. If the
    // Interface number of previous node is not 0, there shouldn't be any more
    // USB Device Path node at this point, or the Device Path is invalid.
    //
    if (*InterfaceNumber != 0) {
      ASSERT (PortNumberChain->UsbPortNumbers[Depth] == 0);
      break;
    }

    if (DevicePathType (DevicePathNode) != MESSAGING_DEVICE_PATH ||
        DevicePathSubType (DevicePathNode) != MSG_USB_DP ||
        DevicePathNodeLength (DevicePathNode) != sizeof (USB_DEVICE_PATH))
    {
      ASSERT (PortNumberChain->UsbPortNumbers[Depth] == 0);
      break;
    }
    UsbDevicePathNode = (CONST USB_DEVICE_PATH *) DevicePathNode;
    //
    // USB specification numbers USB downstream ports from 1 to 255, but USB
    // Device Path in UEFI starts the port number from zero. (A serious
    // discrepancy.)
    //
    if (UsbDevicePathNode->ParentPortNumber >= 255) {
      ASSERT (PortNumberChain->UsbPortNumbers[Depth] == 0);
      break;
    }
    PortNumberChain->UsbPortNumbers[Depth] =
                       (UsbDevicePathNode->ParentPortNumber + 1);

    *InterfaceNumber = UsbDevicePathNode->InterfaceNumber;
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
  *UsbPortDepth = Depth;

  DEBUG ((
    EFI_D_VERBOSE,
    "%a: %a(): USB port %u-%u-%u-%u-%u-%u-%u, Interface number %u.\n",
    gEfiCallerBaseName,
    "DevicePathToUsbPortNumberChain",
    (unsigned int) PortNumberChain->HostControllerNumber,
    (unsigned int) PortNumberChain->UsbPortNumbers[0],
    (unsigned int) PortNumberChain->UsbPortNumbers[1],
    (unsigned int) PortNumberChain->UsbPortNumbers[2],
    (unsigned int) PortNumberChain->UsbPortNumbers[3],
    (unsigned int) PortNumberChain->UsbPortNumbers[4],
    (unsigned int) PortNumberChain->UsbPortNumbers[5],
    (unsigned int) *InterfaceNumber
    ));
}

VOID
EFIAPI
DevicePathToUsbPortNumberChain (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  OUT USB_PORT_NUMBER_CHAIN           *UsbPortNumberChain OPTIONAL,
  OUT UINT8                           *UsbInterfaceNumber OPTIONAL
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  USB_PORT_NUMBER_CHAIN                PortNumberChainBuffer;
  UINT8                                InterfaceNumberBuffer;
  UINTN                                UsbDevicePathOffset;
  UINT8                                UsbPortDepth;

  if ((This == NULL) || (DevicePath == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/DevicePath is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  DebugPrintDevicePath (EFI_D_VERBOSE, __FUNCTION__, DevicePath);

  if (UsbPortNumberChain == NULL) {
    UsbPortNumberChain = &PortNumberChainBuffer;
  }
  if (UsbInterfaceNumber == NULL) {
    UsbInterfaceNumber = &InterfaceNumberBuffer;
  }

  InternalDevicePathToUsbPortNumberChain (
    PrivateData,
    DevicePath,
    UsbPortNumberChain,
    UsbInterfaceNumber,
    &UsbDevicePathOffset,
    &UsbPortDepth
    );

  (VOID) PortNumberChainBuffer;
  (VOID) InterfaceNumberBuffer;
  (VOID) UsbDevicePathOffset;
  (VOID) UsbPortDepth;
}

/**
  Compares two UsbPortNumberChain entries according to port names in the
  database.

  @implements H2O_USB_PORT_NUMBER_CHAIN_COMPARE

  @param[in]  UsbPortNumberChain1  The pointer to first entry.
  @param[in]  UsbPortNumberChain2  The pointer to second entry.
  @param[in]  This

  @retval 0   UsbPortNumberChain1 is equal to UsbPortNumberChain2.
  @retval <0  UsbPortNumberChain1 precedes UsbPortNumberChain2.
  @retval >0  UsbPortNumberChain1 succeeds UsbPortNumberChain2.

**/
INTN
EFIAPI
UsbPortNumberChainCompare (
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain1,
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  USB_PORT_NUMBER_CHAIN                QueryChain1 = {0};
  USB_PORT_NUMBER_CHAIN                QueryChain2 = {0};
  INTN                                 CompareResult;
  UINT8                                Depth;
  CONST H2O_USB_PORT_INFO_TRIE_NODE    *Node1;
  UINT32                               AcpiNames[USB_PORT_MAX_DEPTH];
  CONST H2O_USB_PORT_INFO_TRIE_NODE    *Node2;

  INTN                                 DisplayNameIndex1;
  INTN                                 DisplayNameIndex2;
  INTN                                 IndexDifference;

  if ((This == NULL) || (UsbPortNumberChain1 == NULL) || (UsbPortNumberChain2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/UsbPortNumberChain1/UsbPortNumberChain2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  QueryChain1.HostControllerNumber = UsbPortNumberChain1->HostControllerNumber;
  QueryChain2.HostControllerNumber = UsbPortNumberChain2->HostControllerNumber;
  CompareResult = (INTN) QueryChain1.HostControllerNumber -
                    (INTN) QueryChain2.HostControllerNumber;
  if (QueryChain1.HostControllerNumber == 0 ||
      QueryChain2.HostControllerNumber == 0)
  {
    //
    // Host controllers not in our lookup data will have host controller
    // number 0 and are ordered after those in our lookup data.
    //
    return (-CompareResult);
  }
  if (CompareResult == 0) {
    for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
      QueryChain1.UsbPortNumbers[Depth] = UsbPortNumberChain1->
                                            UsbPortNumbers[Depth];
      QueryChain2.UsbPortNumbers[Depth] = UsbPortNumberChain2->
                                            UsbPortNumbers[Depth];
      CompareResult = (INTN) QueryChain1.UsbPortNumbers[Depth] -
                        (INTN) QueryChain2.UsbPortNumbers[Depth];
      if (QueryChain1.UsbPortNumbers[Depth] == 0 ||
          QueryChain2.UsbPortNumbers[Depth] == 0)
      {
        //
        // USB hub before USB ports of the hub.
        //
        return CompareResult;
      }
      if (CompareResult != 0) {
        break;
      }
    }
    if (CompareResult == 0) {
      return CompareResult;
    }
  }

  Node1 = LookupUsbPortInfoTrieNode (PrivateData, &QueryChain1, AcpiNames);
  (VOID) AcpiNames;
  Node2 = LookupUsbPortInfoTrieNode (PrivateData, &QueryChain2, AcpiNames);
  (VOID) AcpiNames;
  DisplayNameIndex1 = ((Node1 != NULL) ? (INTN) Node1->DisplayNameIndex :
                        MAX_INTN);
  DisplayNameIndex2 = ((Node2 != NULL) ? (INTN) Node2->DisplayNameIndex :
                        MAX_INTN);
  IndexDifference = DisplayNameIndex1 - DisplayNameIndex2;
  if (IndexDifference != 0) {
    return IndexDifference;
  } else {
    return CompareResult;
  }
}

STATIC
CONST H2O_USB_PORT_INFO_TRIE_NODE *
LookupUsbPortInfoTrieNodeByDevicePath (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  OUT UINT32                              AcpiNames[USB_PORT_MAX_DEPTH]
  )
{
  USB_PORT_NUMBER_CHAIN              PortNumberChain;
  UINT8                              InterfaceNumber;
  UINTN                              UsbDevicePathOffset;
  UINT8                              UsbPortDepth;
  CONST EFI_DEVICE_PATH_PROTOCOL     *DevicePathNode;

  InternalDevicePathToUsbPortNumberChain (
    PrivateData,
    DevicePath,
    &PortNumberChain,
    &InterfaceNumber,
    &UsbDevicePathOffset,
    &UsbPortDepth
    );

  if (InterfaceNumber != 0) {
    return NULL;
  }

  DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *)(
                     (CONST UINT8 *) DevicePath + UsbDevicePathOffset +
                     UsbPortDepth * sizeof (USB_DEVICE_PATH));
  if (!IsDevicePathEnd (DevicePathNode)) {
    //
    // This Device Path is not for a USB device but a sub-component of it.
    //
    return NULL;
  }

  return LookupUsbPortInfoTrieNode (
           PrivateData,
           &PortNumberChain,
           AcpiNames
           );
}

EFI_STATUS
GetUsbPortInfoByDevicePath (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  OUT UINT32                              *PcdTokenNumber OPTIONAL,
  OUT UINTN                               *Attributes OPTIONAL,
  OUT CHAR8                               **AcpiPath OPTIONAL,
  OUT CHAR16                              **DisplayName OPTIONAL
  )
{
  CONST H2O_USB_PORT_INFO_TRIE_NODE  *Node;
  UINT32                             AcpiNames[USB_PORT_MAX_DEPTH];
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES  AttributeFlags;

  Node = LookupUsbPortInfoTrieNodeByDevicePath (
           PrivateData,
           DevicePath,
           AcpiNames
           );
  if (Node == NULL) {
    return EFI_NOT_FOUND;
  }

  if (PcdTokenNumber != NULL) {
    *PcdTokenNumber = Node->PcdTokenNumber;
  }
  if (Attributes != NULL) {
    AttributeFlags.Uint8 = 0;
    AttributeFlags.Bits.IsUsb = 1;
    *Attributes = AttributeFlags.Uint8;
  }
  if (AcpiPath != NULL) {
    // Not implemented yet
  }
  if (DisplayName != NULL &&
      Node->DisplayNameIndex < PrivateData->DisplayNameStringCount)
  {
    *DisplayName = CatSPrint (
                     NULL,
                     L"%s",
                     PrivateData->DisplayNameStrings[Node->DisplayNameIndex]
                     );
    if (*DisplayName == NULL) {
      goto DisplayNameFail;
    }
  }
  return EFI_SUCCESS;

DisplayNameFail:
  if (AcpiPath != NULL) {
    FreePoolIfNonNull (*AcpiPath);
    *AcpiPath = NULL;
  }
//AcpiPathFail:
  return EFI_OUT_OF_RESOURCES;
}

UINT16
LookupUsbPortInfoDisplayNameIndex (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath
  )
{
  CONST H2O_USB_PORT_INFO_TRIE_NODE  *Node;
  UINT32                             AcpiNames[USB_PORT_MAX_DEPTH];

  Node = LookupUsbPortInfoTrieNodeByDevicePath (
           PrivateData,
           DevicePath,
           AcpiNames
           );

  (VOID) AcpiNames;

  if (Node == NULL) {
    return MAX_UINT16;
  }

  return Node->DisplayNameIndex;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
AddUsbPortLookupEntry (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL           *DevicePath,
  IN UINT32                                   PcdTokenNumber,
  IN UINTN                                    Attributes,
  IN CONST CHAR8                              *AcpiPath OPTIONAL,
  IN CONST CHAR16                             *DisplayName OPTIONAL,
  IN CONST CHAR16                             *DisplayNameSortKey OPTIONAL
  )
{
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES  AttributeFlags;
  USB_PORT_NUMBER_CHAIN              PortNumberChain;
  UINT8                              InterfaceNumber;
  UINTN                              UsbDevicePathOffset;
  UINT8                              UsbPortDepth;
  CONST EFI_DEVICE_PATH_PROTOCOL     *DevicePathNode;

  UINT32                             TrieNodeIndices[USB_PORT_MAX_DEPTH + 2];
  UINT16                             NumOfNodesNeeded[USB_PORT_MAX_DEPTH + 1];
  H2O_USB_PORT_INFO_TRIE_NODE        *Node;
  UINT16                             NodeMoveOffsets[USB_PORT_MAX_DEPTH + 1];
  UINT8                              Tier;
  UINT8                              PortNumber;
  BOOLEAN                            NodeContainsData;
  UINT16                             TotalNumOfNodesNeeded;

  EFI_STATUS                         Status;

  VOID                               *DevicePathBuffer;
  UINTN                              BufferSize;

  UINTN                              DisplayNameIndex;
  UINTN                              Index;
  H2O_USB_PORT_INFO_TRIE_NODE        TrieNodeBuffer;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePathPtr;
  UINT32                             NodeStartIndex;
  UINT8                              DownstreamPortNumber;

  if ((PrivateData == NULL) || (DevicePath == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/DevicePath is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if the Device Path can be stored into the USB trie structure of the
  // PrivateData. If not, return EFI_UNSUPPORTED to the caller.
  //
  AttributeFlags.Uint8 = (UINT8) Attributes;
  AttributeFlags.Bits.IsUsb = 0;
  if (AttributeFlags.Uint8 != 0) {
    //
    // This device information entry is not just for USB.
    //
    return EFI_UNSUPPORTED;
  }

  InternalDevicePathToUsbPortNumberChain (
    PrivateData,
    DevicePath,
    &PortNumberChain,
    &InterfaceNumber,
    &UsbDevicePathOffset,
    &UsbPortDepth
    );

  if (InterfaceNumber != 0) {
    return EFI_UNSUPPORTED;
  }

  if (PortNumberChain.UsbPortNumbers[0] <= 0) {
    //
    // A USB host controller entry should have "IsUsb" attribute bit set.
    // Otherwise the device information entry might be for something else.
    //
    AttributeFlags.Uint8 = (UINT8) Attributes;
    if (!AttributeFlags.Bits.IsUsb) {
      return EFI_UNSUPPORTED;
    }
  }

  DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *)(
                     (CONST UINT8 *) DevicePath + UsbDevicePathOffset +
                     UsbPortDepth * sizeof (USB_DEVICE_PATH));
  if (!IsDevicePathEnd (DevicePathNode)) {
    //
    // This Device Path is not for a USB device but a sub-component of it.
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Get node index of each tier in the PortNumberChain, and calculate how many
  // nodes need to be created and how many be expanded.
  //
  // The "hub tiers" defined in USB 2.0 specification are numbered from 1 to 7.
  //
  ASSERT (PortNumberChain.HostControllerNumber <= PrivateData->UsbHostControllerCount);
  //
  // When adding a USB host controller entry, the host controller numbers (for
  // this database) will be adjusted according to display name sort keys, and
  // that routine expects TrieNodeIndices[0] to have initial value of 0.
  //
  TrieNodeIndices[0] = 0;
  if (PortNumberChain.HostControllerNumber > 0) {
    //
    // Host controller exists.
    //
    if (PortNumberChain.UsbPortNumbers[0] > 0) {
      TrieNodeIndices[0] = PortNumberChain.HostControllerNumber - 1;
    }
    NumOfNodesNeeded[0] = 0;
    Node = &PrivateData->UsbPortInfoTrieNodes[
                           PortNumberChain.HostControllerNumber - 1];
  } else {
    //
    // Needs to create a new host controller node.
    //
    if (PortNumberChain.UsbPortNumbers[0] > 0) {
      TrieNodeIndices[0] = PrivateData->UsbHostControllerCount;
    }
    NumOfNodesNeeded[0] = 1;
    Node = NULL;
  }

  NodeMoveOffsets[0] = NumOfNodesNeeded[0];
  for (Tier = 2; Tier <= USB_PORT_MAX_DEPTH + 2; Tier++) {
    TrieNodeIndices[Tier - 1] = PrivateData->UsbPortInfoTrieNodeCount;
    if (Tier >= USB_PORT_MAX_DEPTH + 2) {
      break;
    }

    PortNumber = PortNumberChain.UsbPortNumbers[Tier - 2];

    NumOfNodesNeeded[Tier - 1] = PortNumber;
    if (PortNumber <= 0) {
      NodeContainsData = (Node != NULL) &&
                         (Node->PcdTokenNumber != 0 ||
                         Node->DisplayNameIndex < MAX_UINT16);
      if (NodeContainsData) {
        DEBUG ((
          EFI_D_ERROR,
          "%a: %a(): info entry already exists.\n",
          gEfiCallerBaseName,
          __FUNCTION__
          ));
        return EFI_ALREADY_STARTED;
      }
      NumOfNodesNeeded[Tier - 1] = 0;
      Node = NULL;
    }
    if (Node != NULL) {
      if (PortNumber <= Node->MaxDownstreamPortNumber) {
        //
        // Existing node; no expansion needed.
        //
        TrieNodeIndices[Tier - 1] = (Node->DownstreamNodeStartIndex +
                                             PortNumber - 1);
        ASSERT (TrieNodeIndices[Tier - 1] < PrivateData->UsbPortInfoTrieNodeCount);
        NumOfNodesNeeded[Tier - 1] = 0;
        Node = &PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[Tier - 1]];
      } else {
        //
        // Need to expand node to cover new port number.
        //
        if (Node->DownstreamNodeStartIndex <
            PrivateData->UsbPortInfoTrieNodeCount)
        {
          TrieNodeIndices[Tier - 1] = (Node->DownstreamNodeStartIndex +
                                        Node->MaxDownstreamPortNumber);
          ASSERT (TrieNodeIndices[Tier - 1] <= PrivateData->UsbPortInfoTrieNodeCount);
        }
        NumOfNodesNeeded[Tier - 1] = (PortNumber -
                                       Node->MaxDownstreamPortNumber);
        Node = NULL;
      }
    }
    NodeMoveOffsets[Tier - 1] = (NodeMoveOffsets[Tier - 2] +
                                  NumOfNodesNeeded[Tier - 1]);
  }
  TotalNumOfNodesNeeded = NodeMoveOffsets[USB_PORT_MAX_DEPTH];

  AcquireDeviceInfoLookupDataWriteLock (PrivateData);

  //
  // Grow UsbPortInfoTrieNodes buffer when needed.
  //
  if (MAX_UINT32 - PrivateData->UsbPortInfoTrieNodeCount <
      TotalNumOfNodesNeeded)
  {
    DEBUG ((
      EFI_D_ERROR,
      "%a: %a(): trie node limit exceeded.\n",
      gEfiCallerBaseName,
      __FUNCTION__
      ));
    goto GrowBufferFail;
  }

  Status = GrowBufferIfFull (
             (VOID **) &PrivateData->UsbPortInfoTrieNodes,
             PrivateData->UsbPortInfoTrieNodeCount,
             TotalNumOfNodesNeeded,
             sizeof (*PrivateData->UsbPortInfoTrieNodes)
             );
  if (EFI_ERROR (Status)) {
    ASSERT (Status == EFI_OUT_OF_RESOURCES);
    goto GrowBufferFail;
  }

  //
  // Grow UsbHcDevicePaths and UsbHcAcpiPaths buffers when needed.
  //
  if (PortNumberChain.HostControllerNumber <= 0) {
    ASSERT (PrivateData->UsbHostControllerCount < MAX_UINT16);
    Status = GrowBufferIfFull (
               (VOID **) &PrivateData->UsbHcDevicePaths,
               PrivateData->UsbHostControllerCount,
               1,
               sizeof (*PrivateData->UsbHcDevicePaths)
               );
    if (EFI_ERROR (Status)) {
      ASSERT (Status == EFI_OUT_OF_RESOURCES);
      goto GrowBufferFail;
    }
    if (PrivateData->UsbHcAcpiPaths != NULL) {
      Status = GrowBufferIfFull (
                 (VOID **) &PrivateData->UsbHcAcpiPaths,
                 PrivateData->UsbHostControllerCount,
                 1,
                 sizeof (*PrivateData->UsbHcAcpiPaths)
                 );
    } else if (AcpiPath != NULL) {
      Status = GrowBufferIfFull (
                 (VOID **) &PrivateData->UsbHcAcpiPaths,
                 0, // NumOfElements
                 (PrivateData->UsbHostControllerCount + 1),
                 sizeof (*PrivateData->UsbHcAcpiPaths)
                 );
    }
    if (EFI_ERROR (Status)) {
      ASSERT (Status == EFI_OUT_OF_RESOURCES);
      goto GrowBufferFail;
    }
  }

  //
  // If host controller Device Path doesn't exists in the database, allocate a
  // buffer for the Device Path.
  //
  DevicePathBuffer = NULL;
  if (PortNumberChain.HostControllerNumber <= 0) {
    if (PrivateData->UsbHostControllerCount >= MAX_UINT16) {
      DEBUG ((
        EFI_D_ERROR,
        "%a: %a(): too many host controller device paths.\n",
        gEfiCallerBaseName,
        __FUNCTION__
        ));
      goto DevicePathBufferFail;
    }

    ASSERT (UsbDevicePathOffset <= MAX_UINTN - END_DEVICE_PATH_LENGTH);
    BufferSize = UsbDevicePathOffset + END_DEVICE_PATH_LENGTH;
    DevicePathBuffer = AllocatePool (BufferSize);
    if (DevicePathBuffer == NULL) {
      goto DevicePathBufferFail;
    }
    CopyMem (DevicePathBuffer, DevicePath, UsbDevicePathOffset);
    SetDevicePathEndNode ((UINT8 *) DevicePathBuffer + UsbDevicePathOffset);
  }

  //
  // Start changing database content:
  // (1/4) Insert display name string. The subroutine should also update the
  // DisplayNameIndex pointers within UsbPortInfoTrieNodes.
  //
  DisplayNameIndex = MAX_UINT16;
  if (DisplayName != NULL) {
    Status = InsertDisplayNameString (
               PrivateData,
               DisplayName,
               DisplayNameSortKey,
               &DisplayNameIndex
               );
    if (EFI_ERROR (Status) && Status != EFI_ALREADY_STARTED) {
      goto DisplayNameFail;
    }
    ASSERT (DisplayNameIndex < MAX_UINT16);
  }

  //
  // (2/4) Update downstream node index pointers within UsbPortInfoTrieNodes.
  //
  for (Index = 0; Index < PrivateData->UsbPortInfoTrieNodeCount; Index++) {
    Node = &PrivateData->UsbPortInfoTrieNodes[Index];

    ASSERT (Node->DownstreamNodeStartIndex >= PrivateData->UsbHostControllerCount);

    if (Node->DownstreamNodeStartIndex <
        PrivateData->UsbPortInfoTrieNodeCount)
    {
      for (Tier = USB_PORT_MAX_DEPTH + 1; Tier > 0; Tier--) {
        if (Node->DownstreamNodeStartIndex >= TrieNodeIndices[Tier - 1]) {
          Node->DownstreamNodeStartIndex += NodeMoveOffsets[Tier - 1];
          ASSERT (Node->DownstreamNodeStartIndex < PrivateData->UsbPortInfoTrieNodeCount + TotalNumOfNodesNeeded);
          break;
        }
      }
    }

    if (PortNumberChain.UsbPortNumbers[0] <= 0 &&
        Index < PrivateData->UsbHostControllerCount)
    {
      //
      // Find insert position of the new host controller node.
      // TrieNodeIndices[0] is expected to have the initial value of 0.
      //
      if (DisplayNameIndex >= Node->DisplayNameIndex) {
        TrieNodeIndices[0] = (UINT16)(Index + 1);
      }
    }
  }

  //
  // (3/4) Move nodes to new positions and insert new nodes.
  //
  if (PortNumberChain.UsbPortNumbers[0] <= 0 &&
      PortNumberChain.HostControllerNumber > 0)
  {
    //
    // New display name will be added to an existing host controller node.
    // Re-order host controller nodes by DisplayNameIndex values.
    //
    ASSERT (NumOfNodesNeeded[0] == 0);
    ASSERT (TrieNodeIndices[0] + 1 <= PortNumberChain.HostControllerNumber);
    TrieNodeBuffer = PrivateData->UsbPortInfoTrieNodes[
                                    PortNumberChain.HostControllerNumber - 1];
    BufferSize = (PortNumberChain.HostControllerNumber - 1 -
                   TrieNodeIndices[0]) * sizeof (H2O_USB_PORT_INFO_TRIE_NODE);
    CopyMem (
      &PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[0] + 1],
      &PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[0]],
      BufferSize
      );
    PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[0]] = TrieNodeBuffer;

    //
    // Re-order host controller device paths.
    //
    DevicePathPtr = PrivateData->UsbHcDevicePaths[
                                   PortNumberChain.HostControllerNumber - 1];
    BufferSize = (PortNumberChain.HostControllerNumber - 1 -
                   TrieNodeIndices[0]) * sizeof (EFI_DEVICE_PATH_PROTOCOL *);
    CopyMem (
      &PrivateData->UsbHcDevicePaths[TrieNodeIndices[0] + 1],
      &PrivateData->UsbHcDevicePaths[TrieNodeIndices[0]],
      BufferSize
      );
    PrivateData->UsbHcDevicePaths[TrieNodeIndices[0]] = DevicePathPtr;

    DEBUG ((
      EFI_D_VERBOSE,
      "%a: %a(): tier %u, index %u, %u node(s), new index = %u.\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (unsigned int) 1,
      (unsigned int) TrieNodeIndices[0],
      (unsigned int) (PortNumberChain.HostControllerNumber - 1 -
                       TrieNodeIndices[0]),
      (unsigned int) (TrieNodeIndices[0] + 1)
      ));
  }
  PortNumberChain.HostControllerNumber = (UINT16)(TrieNodeIndices[0] + 1);

  NodeStartIndex = MAX_UINT32;
  DownstreamPortNumber = 0;

  for (Tier = USB_PORT_MAX_DEPTH + 1; Tier > 0; Tier--) {
    if (Tier >= 2 && PortNumberChain.UsbPortNumbers[Tier - 2] <= 0) {
      continue;
    }

    ASSERT (TrieNodeIndices[Tier] <= PrivateData->UsbPortInfoTrieNodeCount);
    Node = &PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[Tier - 1] +
                                                NodeMoveOffsets[Tier - 1]];
    BufferSize = (TrieNodeIndices[Tier] - TrieNodeIndices[Tier - 1]) *
                   sizeof (H2O_USB_PORT_INFO_TRIE_NODE);
    CopyMem (
      Node,
      &PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[Tier - 1]],
      BufferSize
      );
    DEBUG ((
      EFI_D_VERBOSE,
      "%a: %a(): tier %u, index %u, %u node(s), new index = %u.\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (unsigned int) Tier,
      (unsigned int) TrieNodeIndices[Tier - 1],
      (unsigned int) (TrieNodeIndices[Tier] - TrieNodeIndices[Tier - 1]),
      (unsigned int) (TrieNodeIndices[Tier - 1] + NodeMoveOffsets[Tier - 1])
      ));

    //
    // Initial (blank) data for new nodes.
    //
    Node = Node - NumOfNodesNeeded[Tier - 1];
    if (Tier >= 2) {
      ASSERT (Node == &PrivateData->UsbPortInfoTrieNodes[TrieNodeIndices[Tier - 1] + NodeMoveOffsets[Tier - 2]]);
    }
    for (Index = 0; Index < NumOfNodesNeeded[Tier - 1]; Index++) {
      Node[Index].PcdTokenNumber           = 0;
      Node[Index].AcpiName                 = 0;
      Node[Index].DownstreamNodeStartIndex = MAX_UINT32;
      Node[Index].MaxDownstreamPortNumber  = 0;
      Node[Index].DisplayNameIndex         = MAX_UINT16;
    }
    DEBUG ((
      EFI_D_VERBOSE,
      "%a: %a(): tier %u, %u new node(s).\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (unsigned int) Tier,
      (unsigned int) NumOfNodesNeeded[Tier - 1]
      ));

    //
    // Write new data.
    //
    if (NumOfNodesNeeded[Tier - 1] > 0) {
      Node = Node + NumOfNodesNeeded[Tier - 1] - 1;
    }
    if (NodeStartIndex == MAX_UINT32) {
      Node->PcdTokenNumber   = PcdTokenNumber;
      Node->DisplayNameIndex = (UINT16) DisplayNameIndex;
    }
    if (Node->MaxDownstreamPortNumber < DownstreamPortNumber) {
      Node->DownstreamNodeStartIndex = NodeStartIndex;
      Node->MaxDownstreamPortNumber  = DownstreamPortNumber;
    }
    if (Tier >= 2) {
      PortNumber = PortNumberChain.UsbPortNumbers[Tier - 2];
      NodeStartIndex = TrieNodeIndices[Tier - 1] + NodeMoveOffsets[Tier - 1] -
                         PortNumber;
      DownstreamPortNumber = PortNumber;
    }
  }
  PrivateData->UsbPortInfoTrieNodeCount += TotalNumOfNodesNeeded;

  //
  // (4/4) Insert host controller device path.
  //
  ASSERT (PortNumberChain.HostControllerNumber > 0);
  Index = PortNumberChain.HostControllerNumber - 1;
  if (DevicePathBuffer != NULL) {
    CopyMem (
      &PrivateData->UsbHcDevicePaths[Index + 1],
      &PrivateData->UsbHcDevicePaths[Index],
      ((PrivateData->UsbHostControllerCount - Index) *
        sizeof (*PrivateData->UsbHcDevicePaths))
      );
    PrivateData->UsbHcDevicePaths[Index] = DevicePathBuffer;
    PrivateData->UsbHostControllerCount++;
    DEBUG ((
      EFI_D_VERBOSE,
      "%a: %a(): inserted host controller path to index %u.\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (unsigned int) Index
      ));
  }

  //
  // Done.
  //
  goto End;

DisplayNameFail:
//  FreePoolIfNonNull (AcpiPathBuffer);
//AcpiPathFail:
  FreePoolIfNonNull (DevicePathBuffer);
DevicePathBufferFail:
GrowBufferFail:
  Status = EFI_OUT_OF_RESOURCES;

End:
  ReleaseDeviceInfoLookupDataWriteLock (PrivateData);

  DEBUG ((
    EFI_D_VERBOSE,
    "%a: %a() status: %r\n",
    gEfiCallerBaseName,
    __FUNCTION__,
    Status
    ));
  return Status;
}

EFI_STATUS
ResetDeviceInfoUsbPortLookupData (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  )
{
  UINTN  Index;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (PrivateData->IsBeingWritten);

  FreePoolIfNonNull (PrivateData->UsbPortInfoTrieNodes);
  if (PrivateData->UsbHcDevicePaths != NULL) {
    for (Index = 0; Index < PrivateData->UsbHostControllerCount; Index++) {
      FreePool (PrivateData->UsbHcDevicePaths[Index]);
    }
    FreePool (PrivateData->UsbHcDevicePaths);
  }
  if (PrivateData->UsbHcAcpiPaths != NULL) {
    for (Index = 0; Index < PrivateData->UsbHostControllerCount; Index++) {
      FreePool (PrivateData->UsbHcAcpiPaths[Index]);
    }
    FreePool (PrivateData->UsbHcAcpiPaths);
  }
  PrivateData->UsbPortInfoTrieNodes     = NULL;
  PrivateData->UsbHcDevicePaths         = NULL;
  PrivateData->UsbHcAcpiPaths           = NULL;
  PrivateData->UsbPortInfoTrieNodeCount = 0;
  PrivateData->UsbHostControllerCount   = 0;

  return EFI_SUCCESS;
}

STATIC
VOID
DebugPrintUsbPortInfoDownstreamNodes (
  IN UINTN                                ErrorLevel,
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT8                                Depth,
  IN OUT USB_PORT_NUMBER_CHAIN            *PortNumberChain,
  IN UINTN                                StartIndex,
  IN UINTN                                MaxPortNumber
  )
{
  UINTN                              Index;
  CONST H2O_USB_PORT_INFO_TRIE_NODE  *Node;
  CHAR16                             *DisplayNameText;

  ASSERT (Depth < USB_PORT_MAX_DEPTH);
  ASSERT (PortNumberChain->UsbPortNumbers[Depth] == 0);
  DEBUG ((
    ErrorLevel,
    "\nPort %u-%u-%u-%u-%u-%u-%u\n",
    (unsigned int) PortNumberChain->HostControllerNumber,
    (unsigned int) PortNumberChain->UsbPortNumbers[0],
    (unsigned int) PortNumberChain->UsbPortNumbers[1],
    (unsigned int) PortNumberChain->UsbPortNumbers[2],
    (unsigned int) PortNumberChain->UsbPortNumbers[3],
    (unsigned int) PortNumberChain->UsbPortNumbers[4],
    (unsigned int) PortNumberChain->UsbPortNumbers[5]
    ));

  for (Index = StartIndex; Index < (StartIndex + MaxPortNumber); Index++) {
    ASSERT (Index < PrivateData->UsbPortInfoTrieNodeCount);
    Node = &PrivateData->UsbPortInfoTrieNodes[Index];

    DisplayNameText = CreateDisplayNameTextForDebug (
                        PrivateData,
                        Node->DisplayNameIndex
                        );

    DEBUG ((
      ErrorLevel,
      "%7u [%5u]: (%u, %u), 0x%08x, %a, %s\n",
      (unsigned int) (Index - StartIndex + 1),
      (unsigned int) Index,
      (unsigned int) Node->DownstreamNodeStartIndex,
      (unsigned int) Node->MaxDownstreamPortNumber,
      (unsigned int) Node->PcdTokenNumber,
      "(not implemented)",
      ((DisplayNameText != NULL) ? DisplayNameText : L"(out of resource)")
      ));
    FreePoolIfNonNull (DisplayNameText);
  }

  for (Index = StartIndex; Index < (StartIndex + MaxPortNumber); Index++) {
    Node = &PrivateData->UsbPortInfoTrieNodes[Index];
    if (Node->MaxDownstreamPortNumber <= 0) {
      continue;
    }
    PortNumberChain->UsbPortNumbers[Depth] = (UINT8)(Index - StartIndex + 1);
    DebugPrintUsbPortInfoDownstreamNodes (
      ErrorLevel,
      PrivateData,
      (Depth + 1),
      PortNumberChain,
      Node->DownstreamNodeStartIndex,
      Node->MaxDownstreamPortNumber
      );
  }

  if (Depth < USB_PORT_MAX_DEPTH) {
    PortNumberChain->UsbPortNumbers[Depth] = 0;
  }
}

VOID
DebugPrintUsbPortInfoTrie (
  IN UINTN                                ErrorLevel,
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  )
{
  UINT16                             Index;
  CONST H2O_USB_PORT_INFO_TRIE_NODE  *Node;
  CHAR16                             *DisplayNameText;
  CONST CHAR8                        *AcpiPath;
  CHAR16                             *DevicePathText;
  USB_PORT_NUMBER_CHAIN              PortNumberChain = {0};

  if ((!DebugPrintEnabled ()) || (!DebugPrintLevelEnabled (ErrorLevel))) {
    return;
  }

  DEBUG ((
    ErrorLevel,
    "USB host controller count: %u\n"
    "PortNum [Index]: Downstream (Index, MaxPortNum), PcdTokenNum, AcpiName, DisplayName\n",
    (unsigned int) PrivateData->UsbHostControllerCount
    ));

  for (Index = 0; Index < PrivateData->UsbHostControllerCount; Index++) {
    ASSERT (Index < PrivateData->UsbPortInfoTrieNodeCount);
    Node = &PrivateData->UsbPortInfoTrieNodes[Index];

    DisplayNameText = CreateDisplayNameTextForDebug (
                        PrivateData,
                        Node->DisplayNameIndex
                        );

    AcpiPath = "(null)";
    if (PrivateData->UsbHcAcpiPaths != NULL &&
        PrivateData->UsbHcAcpiPaths[Index] != NULL)
    {
      AcpiPath = PrivateData->UsbHcAcpiPaths[Index];
    }

    DEBUG ((
      ErrorLevel,
      "%7u [%5u]: (%u, %u), 0x%08x, %a, %s\n",
      (unsigned int) (Index + 1),
      (unsigned int) Index,
      (unsigned int) Node->DownstreamNodeStartIndex,
      (unsigned int) Node->MaxDownstreamPortNumber,
      (unsigned int) Node->PcdTokenNumber,
      AcpiPath,
      ((DisplayNameText != NULL) ? DisplayNameText : L"(out of resource)")
      ));
    FreePoolIfNonNull (DisplayNameText);

    DevicePathText = ConvertDevicePathToText (
                       PrivateData->UsbHcDevicePaths[Index],
                       TRUE, // DisplayOnly
                       TRUE  // AllowShortcuts
                       );
    DEBUG ((
      ErrorLevel,
      "%7u [%5u] DevicePath = %s\n",
      (unsigned int) (Index + 1),
      (unsigned int) Index,
      ((DevicePathText != NULL) ? DevicePathText : L"(out of resource)")
      ));
    FreePoolIfNonNull (DevicePathText);
  }

  for (Index = 0; Index < PrivateData->UsbHostControllerCount; Index++) {
    Node = &PrivateData->UsbPortInfoTrieNodes[Index];
    if (Node->MaxDownstreamPortNumber <= 0) {
      continue;
    }
    PortNumberChain.HostControllerNumber = (Index + 1);

    DebugPrintUsbPortInfoDownstreamNodes (
      ErrorLevel,
      PrivateData,
      0,
      &PortNumberChain,
      Node->DownstreamNodeStartIndex,
      Node->MaxDownstreamPortNumber
      );
  }
}

VOID
UpdateUsbPortInfoDisplayNameIndices (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT16                                   NewNameIndex
  )
{
  UINTN                        Index;
  H2O_USB_PORT_INFO_TRIE_NODE  *Node;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  ASSERT (PrivateData->IsBeingWritten);

  for (Index = 0; Index < PrivateData->UsbPortInfoTrieNodeCount; Index++) {
    if (PrivateData->UsbPortInfoTrieNodes == NULL) {
      break;
    }
    Node = &PrivateData->UsbPortInfoTrieNodes[Index];
    if (Node->DisplayNameIndex >= NewNameIndex &&
        Node->DisplayNameIndex < MAX_UINT16)
    {
      Node->DisplayNameIndex++;
      ASSERT (Node->DisplayNameIndex < MAX_UINT16);
    }
  }
}

#endif // H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
