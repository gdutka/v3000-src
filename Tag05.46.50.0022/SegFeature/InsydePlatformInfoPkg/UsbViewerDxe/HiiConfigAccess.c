/** @file
  Configuration Access Protocol and form callback routines of H2O USB Viewer

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

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <IndustryStandard/Pci.h>

#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>
#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/PciIo.h>
#include <Protocol/UsbIo.h>
#include <Protocol/Usb3HostController.h>         // InsydeModulePkg
#include <Protocol/H2ODeviceInfoLookup.h>        // InsydePlatformInfoPkg
#include <Protocol/H2OUsbTextLookup.h>           // InsydePlatformInfoPkg

#include <Guid/MdeModuleHii.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiUsbLib.h>
#include <Library/HiiLib.h>                      // MdeModulePkg
#include <Library/SortLib.h>                     // MdeModulePkg
#include <Library/HiiExLib.h>                    // InsydeModulePkg
#include <Library/BlockIoCapacityLib.h>          // InsydePlatformInfoPkg
#include <Library/DeviceInfoLookupDataLib.h>     // InsydePlatformInfoPkg
#include <Library/UsbHandleRoutineLib.h>         // InsydePlatformInfoPkg
#include <Library/UefiHiiServicesLib.h>

#include "UsbViewerDxe.h"
#include "UsbViewerSetup.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

STATIC H2O_USB_VIEWER_SORT_CONTEXT  mH2OUsbViewerSortContext = {
  NULL, // DeviceInfoLookup
  NULL  // NameDbHcNumberMap
};

/**
  Initializes a USB_HANDLE_INFO_ENTRY from a device path.

  @param[out] UsbHandleInfoEntry
  @param[in]  DeviceHandle
  @param[in]  HostControllerNumber
  @param[in]  NameDatabaseHcNumber
  @param[in]  UsbDevicePath

  @return  Void.

**/
STATIC
VOID
InitializeUsbHandleInfoEntry (
  OUT USB_HANDLE_INFO_ENTRY  *UsbHandleInfoEntry,
  IN EFI_HANDLE              DeviceHandle,
  IN UINT16                  HostControllerNumber,
  IN CONST USB_DEVICE_PATH   *UsbDevicePath OPTIONAL
  )
{
  USB_HANDLE_INFO_ENTRY           *Entry;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  UINTN                           Depth;
  CONST USB_DEVICE_PATH           *UsbDevicePathNode;

  if (UsbHandleInfoEntry == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UsbHandleInfoEntry is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  Entry = UsbHandleInfoEntry;
  Entry->Handle = DeviceHandle;
  Entry->PortNumberChain.HostControllerNumber = HostControllerNumber;
  Entry->InterfaceNumber = 0;

  if (UsbDevicePath == NULL) {
    Entry->PortNumberChain.UsbPortNumbers[0] = 0;
    return;
  }

  DevicePathNode = &UsbDevicePath->Header;
  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
    if (DevicePathType (DevicePathNode) != MESSAGING_DEVICE_PATH ||
        DevicePathSubType (DevicePathNode) != MSG_USB_DP ||
        DevicePathNodeLength (DevicePathNode) != sizeof (USB_DEVICE_PATH))
    {
      Entry->PortNumberChain.UsbPortNumbers[Depth] = 0;
      return;
    }
    UsbDevicePathNode = (CONST USB_DEVICE_PATH *) DevicePathNode;
    //
    // USB specification numbers USB downstream ports from 1 to 255, but USB
    // Device Path in UEFI starts the port number from zero. (A serious
    // discrepancy.)
    //
    if (UsbDevicePathNode->ParentPortNumber >= 255) {
      Entry->PortNumberChain.UsbPortNumbers[Depth] = 0;
      return;
    }
    Entry->PortNumberChain.UsbPortNumbers[Depth] =
                             (UsbDevicePathNode->ParentPortNumber + 1);
    Entry->InterfaceNumber = UsbDevicePathNode->InterfaceNumber;
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
}

/**
  Retreives the start of the USB portion of the Device Path if the full path
  matches a given path prefix.

  @param[in]  DevicePath        Device Path to check
  @param[in]  DevicePathPrefix  Device Path prefix to match. This prefix must
                                be a device path of a USB host controller or a
                                USB hub.

  @retval Other  Pointer to the start of the USB portion of DevicePath if the
                 path matches the prefix.
  @retval NULL   DevicePath is not a descendant, USB device under the path
                 specified in DevicePathPrefix.

**/
STATIC
USB_DEVICE_PATH *
GetUsbDevicePathIfDescendant (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathPrefix
  )
{
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  UINTN                           UsbDevicePathOffset;
  UINTN                           UsbDevicePathLength;
  UINTN                           CompareSize;

  if ((DevicePath == NULL) || (DevicePathPrefix == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DevicePath/DevicePathPrefix is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  //
  // Find the USB Device Path nodes from DevicePathPrefix, if none is found,
  // this is a host controller and use the entire path (subtracting the end
  // node) for prefix matching.
  //
  FindUsbDevicePathNodeAndLength (
    DevicePathPrefix,
    &UsbDevicePathOffset,
    &UsbDevicePathLength
    );
  if (UsbDevicePathLength >= USB_PORT_MAX_DEPTH * sizeof (USB_DEVICE_PATH)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: USB device path depth limit exceeded (protocol address %p)\n",
      gEfiCallerBaseName,
      DevicePathPrefix
      ));
    return NULL;
  }

  //
  // DevicePath might be shorter than DevicePathPrefix.
  // CompareMem from BaseMemoryLib reads no further than the first mismatched
  // byte - a behaviour not required by C standard memcmp().
  //
  CompareSize = UsbDevicePathOffset + UsbDevicePathLength;
  if (CompareMem (DevicePath, DevicePathPrefix, CompareSize) != 0) {
    return NULL;
  }

  DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *)
                     (((CONST UINT8 *) DevicePath) + CompareSize);
  if (DevicePathType (DevicePathNode) != MESSAGING_DEVICE_PATH ||
      DevicePathSubType (DevicePathNode) != MSG_USB_DP ||
      DevicePathNodeLength (DevicePathNode) != sizeof (USB_DEVICE_PATH))
  {
    return NULL;
  }

  return (USB_DEVICE_PATH *) (
           ((CONST UINT8 *) DevicePath) + UsbDevicePathOffset);
}

STATIC
INTN
CompareUsbHostControllerNames (
  IN UINT16                           NameDbHcNumber1,
  IN UINT16                           NameDbHcNumber2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *HostControllerPath1;
  EFI_DEVICE_PATH_PROTOCOL  *HostControllerPath2;
  INTN                      CompareResult;

  if (DeviceInfoLookup == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DeviceInfoLookup is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  if (DeviceInfoLookup->Usb == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DeviceInfoLookup->Usb is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  Status = DeviceInfoLookup->Usb->GetUsbHcDevicePathByNumber (
                                    DeviceInfoLookup,
                                    NameDbHcNumber1,
                                    &HostControllerPath1
                                    );
  if (EFI_ERROR (Status)) {
    HostControllerPath1 = NULL;
  }

  Status = DeviceInfoLookup->Usb->GetUsbHcDevicePathByNumber (
                                    DeviceInfoLookup,
                                    NameDbHcNumber2,
                                    &HostControllerPath2
                                    );
  if (EFI_ERROR (Status)) {
    HostControllerPath2 = NULL;
  }

  CompareResult = ((HostControllerPath1 == NULL) ? 1 : 0) -
                    ((HostControllerPath2 == NULL) ? 1 : 0);

  if (HostControllerPath1 != NULL && HostControllerPath2 != NULL) {
    CompareResult = DeviceInfoLookup->DeviceInfoDisplayNameCompare (
                                        HostControllerPath1,
                                        HostControllerPath2,
                                        DeviceInfoLookup
                                        );
  }

  if (HostControllerPath1 != NULL) {
    FreePool (HostControllerPath1);
  }
  if (HostControllerPath2 != NULL) {
    FreePool (HostControllerPath2);
  }

  return CompareResult;
}

/**

**/
STATIC
INTN
CompareUsbPortsWithName (
  IN CONST USB_PORT_NUMBER_CHAIN  *PortNumberChain1,
  IN CONST USB_PORT_NUMBER_CHAIN  *PortNumberChain2,
  IN H2O_USB_VIEWER_SORT_CONTEXT  *SortContext
  )
{
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup;
  INTN                             CompareResult;
  UINT16                           NameDbHcNumber1;
  UINT16                           NameDbHcNumber2;
  INTN                             HcNameCompareResult;
  USB_PORT_NUMBER_CHAIN            PortNumberChainBuffer1;
  USB_PORT_NUMBER_CHAIN            PortNumberChainBuffer2;

  if ((SortContext == NULL) || (PortNumberChain1 == NULL) || (PortNumberChain2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The SortContext/PortNumberChain1/PortNumberChain2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  DeviceInfoLookup = SortContext->DeviceInfoLookup;

  CompareResult = ((INTN) PortNumberChain1->HostControllerNumber -
                    (INTN) PortNumberChain2->HostControllerNumber);

  NameDbHcNumber1 = *(SortContext->NameDbHcNumberMap +
                      PortNumberChain1->HostControllerNumber);

  if (CompareResult != 0) {
    //
    // If two host controllers have the same name in the Name Database, their
    // numbers are decided by the order they are added into the database, which
    // can be unpredictable (depending on how LibPcdGetNextToken() fetches PCD
    // entries). We ignore the numbering in the Name Database and instead use
    // the order of the handles created in LocateHandleBuffer().
    //
    if (DeviceInfoLookup != NULL) {
      NameDbHcNumber2 = *(SortContext->NameDbHcNumberMap +
                          PortNumberChain2->HostControllerNumber);

      HcNameCompareResult = CompareUsbHostControllerNames (
                              NameDbHcNumber1,
                              NameDbHcNumber2,
                              DeviceInfoLookup
                              );
      if (HcNameCompareResult != 0) {
        return HcNameCompareResult;
      }
    }

    return CompareResult;
  }
  if (NameDbHcNumber1 != 0) {
    PortNumberChainBuffer1 = *PortNumberChain1;
    PortNumberChainBuffer1.HostControllerNumber = NameDbHcNumber1;
    PortNumberChainBuffer2 = *PortNumberChain2;
    PortNumberChainBuffer2.HostControllerNumber = NameDbHcNumber1;

    if (DeviceInfoLookup != NULL) {
      CompareResult = DeviceInfoLookup->Usb->UsbPortNumberChainCompare (
                                               &PortNumberChainBuffer1,
                                               &PortNumberChainBuffer2,
                                               DeviceInfoLookup
                                               );
    }
  } else {
    //
    // Host controller not registered in the Port Name Database.
    //
    CompareResult = CompareMem (
                      PortNumberChain1->UsbPortNumbers,
                      PortNumberChain2->UsbPortNumbers,
                      sizeof (PortNumberChain1->UsbPortNumbers)
                      );
  }

  return CompareResult;
}

/**
  Compares two USB_HANDLE_INFO_ENTRY buffers.

  @implements SORT_COMPARE

  @param[in]  Buffer1  The pointer to first buffer.
  @param[in]  Buffer2  The pointer to second buffer.

  @retval 0            Buffer1 is equal to Buffer2.
  @retval <0           Buffer1 precedes or is less than Buffer2.
  @retval >0           Buffer1 succeeds or is greater than Buffer2.

**/
STATIC
INTN
EFIAPI
CompareUsbHandleInfo (
  IN CONST VOID  *Buffer1,
  IN CONST VOID  *Buffer2
  )
{
  CONST USB_HANDLE_INFO_ENTRY  *InfoEntry1;
  CONST USB_HANDLE_INFO_ENTRY  *InfoEntry2;
  INTN                         CompareResult;

  if ((Buffer1 == NULL) || (Buffer2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Buffer1/Buffer2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  InfoEntry1 = (CONST USB_HANDLE_INFO_ENTRY *) Buffer1;
  InfoEntry2 = (CONST USB_HANDLE_INFO_ENTRY *) Buffer2;

  CompareResult = CompareUsbPortsWithName (
                    &InfoEntry1->PortNumberChain,
                    &InfoEntry2->PortNumberChain,
                    &mH2OUsbViewerSortContext
                    );
  if (CompareResult != 0) {
    return CompareResult;
  }
  CompareResult = (INTN) InfoEntry1->InterfaceNumber -
                    InfoEntry2->InterfaceNumber;
  if (CompareResult != 0) {
    return CompareResult;
  }
  //
  // Make sure the sort order is deterministic.
  //
  if ((UINTN) InfoEntry1->Handle < (UINTN) InfoEntry2->Handle) {
    return -1;
  }
  if ((UINTN) InfoEntry1->Handle > (UINTN) InfoEntry2->Handle) {
    return 1;
  }
  return 0;
}

/**

  @param[in]  Entry1  The pointer to first entry.
  @param[in]  Entry2  The pointer to second entry.

  @retval 0   Entry1 is equal to Entry2.
  @retval 1   Entry1 differs Entry2 in host controller number.
  @retval >1  Entry1 differs Entry2 in a USB port tier.

**/
STATIC
UINT8
FindUsbPortNumberDifferingTier (
  IN CONST USB_PORT_NUMBER_CHAIN  *Entry1,
  IN CONST USB_PORT_NUMBER_CHAIN  *Entry2
  )
{
  UINT8  Depth;

  if ((Entry1 == NULL) || (Entry2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Entry1/Entry2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 1;
  }

  if (Entry1->HostControllerNumber != Entry2->HostControllerNumber) {
    return 1;
  }
  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
    if (Entry1->UsbPortNumbers[Depth] != Entry2->UsbPortNumbers[Depth]) {
      return (Depth + 2);
    }
    if (Entry1->UsbPortNumbers[Depth] == 0 &&
        Entry2->UsbPortNumbers[Depth] == 0)
    {
      return 0;
    }
  }
  return 0;
}

/**

**/
STATIC
VOID
CountUsbDownstreamDevices (
  IN UINTN                               UsbDeviceCount,
  IN CONST USB_HANDLE_INFO_ENTRY         *UsbHandleInfoArray,
  IN UINTN                               NonEmptyHubCount,
  OUT USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamDeviceCountArray
  )
{
  UINTN                              HandleIndex;
  CONST USB_HANDLE_INFO_ENTRY        *HubEntry;
  UINT8                              Depth;
  UINT8                              CountingTier;
  USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *Stack[USB_PORT_MAX_DEPTH] = {NULL};
  USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *CountEntry;
  CONST USB_HANDLE_INFO_ENTRY        *DeviceEntry;
  UINT8                              Tier;
  USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *NextCountEntry;

  if ((UsbHandleInfoArray == NULL) || (DownstreamDeviceCountArray == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UsbHandleInfoArray/DownstreamDeviceCountArray is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // DownstreamDeviceCountArray[0] represents the total count of USB hubs and
  // peripherals. The entry is considered "tier 0" in this algorithm.
  //
  HubEntry     = NULL;
  CountingTier = 0;
  CountEntry   = &DownstreamDeviceCountArray[0];
  NextCountEntry = CountEntry + 1;

  for (HandleIndex = 0; TRUE; HandleIndex++) {
    if (HubEntry == NULL || HandleIndex >= UsbDeviceCount) {
      Tier = 1;
    } else {
      DeviceEntry = &UsbHandleInfoArray[HandleIndex];
      Tier = FindUsbPortNumberDifferingTier (
               &HubEntry->PortNumberChain,
               &DeviceEntry->PortNumberChain
               );
    }
    ASSERT (Tier > 0);
    if (Tier < 2 || HubEntry->PortNumberChain.UsbPortNumbers[Tier - 2] != 0) {
      //
      // "DeviceEntry" is not downstream of "HubEntry".
      // Wrap up the count stack until the real upsteam hub of "DeviceEntry".
      //
      while ((CountingTier >= Tier) && (CountingTier < USB_PORT_MAX_DEPTH)) {
        do {
          ASSERT (CountingTier > 0);
          if (CountingTier <= 0) {
            UNREACHABLE ();
            return;
          }
          CountingTier--;
        } while (Stack[CountingTier] == NULL);
        Stack[CountingTier]->PeripheralCount += CountEntry->PeripheralCount;
        Stack[CountingTier]->HubCount        += (CountEntry->HubCount + 1);
        CountEntry = Stack[CountingTier];
        Stack[CountingTier] = NULL;
      }
    }

    if (HandleIndex >= UsbDeviceCount) {
      break;
    }
    DeviceEntry = &UsbHandleInfoArray[HandleIndex];

    if (DeviceEntry->HasDownstreamDevice) {
      for (Depth = Tier - 1; Depth < USB_PORT_MAX_DEPTH; Depth++) {
        if (DeviceEntry->PortNumberChain.UsbPortNumbers[Depth] == 0) {
          break;
        }
      }
      Stack[CountingTier] = CountEntry;
      HubEntry = DeviceEntry;
      CountingTier = Depth + 1;
      CountEntry = NextCountEntry++;
      ASSERT (CountEntry <= &DownstreamDeviceCountArray[NonEmptyHubCount]);
      ASSERT (CountEntry->HubCount == 0);
      ASSERT (CountEntry->PeripheralCount == 0);
    } else {
      if (DeviceEntry->IsHub) {
        CountEntry->HubCount++;
      } else {
        CountEntry->PeripheralCount++;
      }
    }
  }
  return;
}

STATIC
VOID
RefreshUsbDeviceGotoOpIndex (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN H2O_USB_VIEWER_REFRESH_MODE      RefreshMode,
  IN H2O_USB_VIEWER_SORT_CONTEXT      *SortContext,
  IN UINTN                            UsbDeviceCount,
  IN OUT USB_HANDLE_INFO_ENTRY        *UsbHandleInfoArray OPTIONAL,
  OUT USB_PORT_NUMBER_CHAIN           *FirstRemovedPortNumber,
  OUT UINT16                          *FirstRemovedGotoOpIndex
  )
{
  UINT16                        GotoOpIndex;
  H2O_USB_VIEWER_GOTO_OP_ENTRY  *GotoOpEntry;
  UINTN                         HandleIndex;
  INTN                          CompareResult;
  USB_PORT_NUMBER_CHAIN         ZeroPortNumberChainEntry = {0};
  BOOLEAN                       DeviceChangeRefresh;

  if ((PrivateData == NULL) || (SortContext == NULL) || (FirstRemovedPortNumber == NULL) || (FirstRemovedGotoOpIndex == NULL) ||
      ((UsbDeviceCount > 0) && (UsbHandleInfoArray == NULL))) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/SortContext/FirstRemovedPortNumber/FirstRemovedGotoOpIndex is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  ASSERT (UsbDeviceCount <= 0 || UsbHandleInfoArray != NULL);

  DeviceChangeRefresh = (RefreshMode != H2OUsbViewerDeviceChangeRefresh);
  GotoOpIndex = PrivateData->LastDeviceGotoOpIndex;
  GotoOpEntry = NULL;
  HandleIndex = UsbDeviceCount - 1;
  while (GotoOpIndex <= 0xFFFF) {
    if (GotoOpIndex >= PrivateData->NumGotoOpsAllocated) {
      if (HandleIndex == (UINTN)(-1)) {
        break;
      }
      CompareResult = 1;
    } else {
      if (GotoOpIndex == 0xFFFF) {
        DeviceChangeRefresh = FALSE;
      }
      GotoOpEntry = &PrivateData->GotoOpEntries[GotoOpIndex];
      if (HandleIndex == (UINTN)(-1) || DeviceChangeRefresh) {
        CompareResult = -1;
      } else {
        CompareResult = CompareUsbPortsWithName (
                          &UsbHandleInfoArray[HandleIndex].PortNumberChain,
                          &GotoOpEntry->PortNumberChain,
                          SortContext
                          );
      }
    }
    if (CompareResult > 0) {
      //
      // Either a new device or the entry has never been displayed to the
      // form due to filter.
      //
      UsbHandleInfoArray[HandleIndex].GotoOpIndex = 0xFFFF;
      HandleIndex--;
      continue;
    }
    if (CompareResult < 0) {
      //
      // "First removed" refers to the earliest entry in the list. It is the
      // last entry popped out in this iteration loop.
      //
      *FirstRemovedPortNumber  = GotoOpEntry->PortNumberChain;
      *FirstRemovedGotoOpIndex = GotoOpIndex;
      GotoOpEntry->PortNumberChain = ZeroPortNumberChainEntry;
      GotoOpIndex = GotoOpEntry->PreviousDeviceGotoOpIndex;
      continue;
    }
    UsbHandleInfoArray[HandleIndex].GotoOpIndex = GotoOpIndex;
    GotoOpIndex = GotoOpEntry->PreviousDeviceGotoOpIndex;
    HandleIndex--;
  }
  PrivateData->LastDeviceGotoOpIndex = 0xFFFF;
}

/**

  @param[in,out] PrivateData
  @param[in]     DevicePathPrefixCount
  @param[in]     DevicePathPrefixes
  @param[in]     HostControllerHandles
  @param[in]     OldTpl
  @param[in]     RefreshMode
  @param[out]    NameDatabaseHcNumbers
  @param[out]    UsbDeviceCount
  @param[out]    UsbHandleInfoArray
  @param[out]    DownstreamDeviceCountArray
  @param[out]    FirstRemovedPortNumber
  @param[out]    FirstRemovedGotoOpIndex

  @retval  EFI_SUCCESS
  @retval  EFI_NOT_FOUND         No USB device or interface handles are found.
  @retval  EFI_OUT_OF_RESOURCES  Not enough memory to allocate buffers.

**/
STATIC
EFI_STATUS
CollectUsbDeviceInfoAndRestoreTpl (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA     *PrivateData,
  IN UINT16                              DevicePathPrefixCount,
  IN CONST EFI_DEVICE_PATH_PROTOCOL      **DevicePathPrefixes,
  IN EFI_HANDLE                          *HostControllerHandles OPTIONAL,
  IN EFI_TPL                             OldTpl,
  IN H2O_USB_VIEWER_REFRESH_MODE         RefreshMode,
  OUT UINT16                             *NameDatabaseHcNumbers,
  OUT UINTN                              *UsbDeviceCount,
  OUT USB_HANDLE_INFO_ENTRY              **UsbHandleInfoArray,
  OUT USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  **DownstreamDeviceCountArray,
  OUT USB_PORT_NUMBER_CHAIN              *FirstRemovedPortNumber,
  OUT UINT16                             *FirstRemovedGotoOpIndex
  )
{
  USB_PORT_NUMBER_CHAIN            PortNumberChainBuffer = {0};
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup;
  UINT16                           PathIndex;
  UINT8                            InterfaceNumber;

  UINTN                            ActiveHandleCount;
  EFI_STATUS                       Status;
  UINTN                            UsbIoHandleCount;
  EFI_HANDLE                       *UsbIoHandles;
  UINTN                            AllocateSize;

  UINT16                           HostControllerNumber;
  UINTN                            HandleIndex;
  EFI_STATUS                       OpenStatus;
  CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath;
  CONST USB_DEVICE_PATH            *UsbDevicePath;
  UINTN                            NonEmptyHubCount;
  BOOLEAN                          IsSameDevice;
  UINT8                            Tier;

  if ((PrivateData == NULL) || (DevicePathPrefixes == NULL) || (NameDatabaseHcNumbers == NULL) || (UsbDeviceCount == NULL) || (DevicePathPrefixCount == 0) ||
      (UsbHandleInfoArray == NULL) || (DownstreamDeviceCountArray == NULL) || (FirstRemovedPortNumber == NULL) || (FirstRemovedGotoOpIndex == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/ControllerEntry/PromptString/HelpString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (EfiGetCurrentTpl () == TPL_NOTIFY);

  *UsbDeviceCount = 0;
  *UsbHandleInfoArray = NULL;
  *DownstreamDeviceCountArray = NULL;
  *FirstRemovedPortNumber = PortNumberChainBuffer; // Buffer is {0}
  *FirstRemovedGotoOpIndex = 0xFFFF;

  DeviceInfoLookup = PrivateData->DeviceInfoLookup;

  for (PathIndex = 0; PathIndex < DevicePathPrefixCount; PathIndex++) {
    NameDatabaseHcNumbers[PathIndex] = 0;

    if (DevicePathPrefixes[PathIndex] == NULL) {
      continue;
    }

    if (DeviceInfoLookup == NULL) {
      continue;
    }
    DeviceInfoLookup->Usb->DevicePathToUsbPortNumberChain (
                             DeviceInfoLookup,
                             DevicePathPrefixes[PathIndex],
                             &PortNumberChainBuffer,
                             &InterfaceNumber
                             );
    NameDatabaseHcNumbers[PathIndex] = PortNumberChainBuffer.
                                         HostControllerNumber;
  }

  ActiveHandleCount = 0;
  //
  // Get USB handle list.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiUsbIoProtocolGuid,
                  NULL, // SearchKey
                  &UsbIoHandleCount,
                  &UsbIoHandles
                  );
  if (Status == EFI_NOT_FOUND) {
    Status = EFI_SUCCESS;
    UsbIoHandleCount = 0;
    UsbIoHandles = NULL;
  } else if (EFI_ERROR (Status)) {
    goto UsbIoHandlesFail;
  }
  AllocateSize = UsbIoHandleCount * sizeof (USB_HANDLE_INFO_ENTRY);
  if (HostControllerHandles != NULL) {
    AllocateSize += DevicePathPrefixCount * sizeof (USB_HANDLE_INFO_ENTRY);
  }
  if (AllocateSize <= 0) {
    Status = EFI_NOT_FOUND;
    goto UsbIoHandlesFail;
  }
  *UsbHandleInfoArray = AllocateZeroPool (AllocateSize);
  if ((*UsbHandleInfoArray) == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto UsbHandleInfoFail;
  }

  //
  // Create info entries for host controllers.
  //
  if (HostControllerHandles != NULL) {
    for (PathIndex = 0; PathIndex < DevicePathPrefixCount; PathIndex++) {
      if (DevicePathPrefixes[PathIndex] == NULL ||
          HostControllerHandles[PathIndex] == NULL)
      {
        continue;
      }
      HostControllerNumber = (PathIndex + 1);
      if (DevicePathPrefixCount <= 1 &&
          PrivateData->SelectedPortNumberChain.HostControllerNumber > 0)
      {
        HostControllerNumber =
          PrivateData->SelectedPortNumberChain.HostControllerNumber;
      }
      InitializeUsbHandleInfoEntry (
        &(*UsbHandleInfoArray)[ActiveHandleCount],
        HostControllerHandles[PathIndex],
        HostControllerNumber,
        NULL
        );
      (*UsbHandleInfoArray)[ActiveHandleCount].HasDownstreamDevice = 0;
      (*UsbHandleInfoArray)[ActiveHandleCount].IsHub = 1;
      (*UsbHandleInfoArray)[ActiveHandleCount].Reserved = 0;
      ActiveHandleCount++;
    }
  }

  //
  // Create info entries for USB I/O handles we found.
  //
  for (HandleIndex = 0; HandleIndex < UsbIoHandleCount; HandleIndex++) {
    OpenStatus = GetProtocolWithCorrectStatus (
                   UsbIoHandles[HandleIndex],
                   &gEfiDevicePathProtocolGuid,
                   (VOID **) &DevicePath
                   );
    if (EFI_ERROR (OpenStatus)) {
      continue;
    }
    UsbDevicePath = NULL; // Silence warning in old MSVC
    for (PathIndex = 0; PathIndex < DevicePathPrefixCount; PathIndex++) {
      if (DevicePathPrefixes[PathIndex] == NULL) {
        continue;
      }
      UsbDevicePath = GetUsbDevicePathIfDescendant (
                        DevicePath,
                        DevicePathPrefixes[PathIndex]
                        );
      if (UsbDevicePath != NULL) {
        break;
      }
    }
    if (PathIndex >= DevicePathPrefixCount) {
      //
      // The USB IO handle is outside the host controllers / hub that's
      // selected as the filter. Skip it.
      //
      continue;
    }

    HostControllerNumber = (PathIndex + 1);
    if (DevicePathPrefixCount <= 1 &&
        PrivateData->SelectedPortNumberChain.HostControllerNumber > 0)
    {
      HostControllerNumber =
        PrivateData->SelectedPortNumberChain.HostControllerNumber;
    }
    InitializeUsbHandleInfoEntry (
      &(*UsbHandleInfoArray)[ActiveHandleCount],
      UsbIoHandles[HandleIndex],
      HostControllerNumber,
      UsbDevicePath
      );
    ActiveHandleCount++;
  }

  if (ActiveHandleCount <= 0) {
    Status = EFI_NOT_FOUND;
    FreePool (*UsbHandleInfoArray);
    *UsbHandleInfoArray = NULL;
  }

UsbHandleInfoFail:
  if (UsbIoHandles != NULL) {
    gBS->FreePool (UsbIoHandles);
  }
UsbIoHandlesFail:
  gBS->RestoreTPL (OldTpl);

  if (EFI_ERROR (Status)) {
    goto Final;
  }

  //
  // Sort the list by port numbers.
  //
  mH2OUsbViewerSortContext.DeviceInfoLookup = DeviceInfoLookup;
  mH2OUsbViewerSortContext.NameDbHcNumberMap = (NameDatabaseHcNumbers - 1);
  if (DevicePathPrefixCount <= 1 &&
      PrivateData->SelectedPortNumberChain.HostControllerNumber > 0)
  {
    mH2OUsbViewerSortContext.NameDbHcNumberMap =
      (NameDatabaseHcNumbers - PrivateData->SelectedPortNumberChain.
                                              HostControllerNumber);
  }
  PerformQuickSort (
    (*UsbHandleInfoArray),
    ActiveHandleCount,
    sizeof (USB_HANDLE_INFO_ENTRY),
    CompareUsbHandleInfo
    );

  //
  // Remove handles of same physical device but different USB "interfaces".
  // For robustness, we don't assume the handle of interface number 0 always
  // exists for a device.
  //
  *UsbDeviceCount = 1;
  NonEmptyHubCount = 0;
  for (HandleIndex = 1; HandleIndex < ActiveHandleCount; HandleIndex++) {
    IsSameDevice = CompareMem (
                     &(*UsbHandleInfoArray)[HandleIndex - 1].PortNumberChain,
                     &(*UsbHandleInfoArray)[HandleIndex].PortNumberChain,
                     sizeof (USB_PORT_NUMBER_CHAIN)
                     ) == 0;
    if (IsSameDevice) {
      continue;
    }

    ASSERT ((*UsbDeviceCount) <= HandleIndex);
    if ((*UsbDeviceCount) < HandleIndex) {
      (*UsbHandleInfoArray)[*UsbDeviceCount] =
        (*UsbHandleInfoArray)[HandleIndex];
    }

    //
    // A USB hub is required by specification to have only one interface.
    //
    if ((*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].InterfaceNumber == 0) {
      Tier = FindUsbPortNumberDifferingTier (
               &(*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].PortNumberChain,
               &(*UsbHandleInfoArray)[*UsbDeviceCount].PortNumberChain
               );
      ASSERT (Tier > 0);
      if (Tier >= 2 &&
          (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].
            PortNumberChain.UsbPortNumbers[Tier - 2] == 0)
      {
        (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].HasDownstreamDevice = 1;
        (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].IsHub = 1;
        (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].Reserved = 0;
        NonEmptyHubCount++;
      }
      else if (IsUsbHubHandle (
                 (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].Handle
                 ))
      {
        (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].HasDownstreamDevice = 0;
        (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].IsHub = 1;
        (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].Reserved = 0;
      }
    }

    ++(*UsbDeviceCount);
  }

  if ((*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].InterfaceNumber == 0 &&
      IsUsbHubHandle ((*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].Handle))
  {
    (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].HasDownstreamDevice = 0;
    (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].IsHub = 1;
    (*UsbHandleInfoArray)[(*UsbDeviceCount) - 1].Reserved = 0;
  }

  AllocateSize = (NonEmptyHubCount + 1) *
                   sizeof (USB_DOWNSTREAM_DEVICE_COUNT_ENTRY);
  *DownstreamDeviceCountArray = AllocateZeroPool (AllocateSize);
  if ((*DownstreamDeviceCountArray) != NULL) {
    CountUsbDownstreamDevices (
      *UsbDeviceCount,
      *UsbHandleInfoArray,
      NonEmptyHubCount,
      *DownstreamDeviceCountArray
      );
  }

Final:
  if (Status != EFI_OUT_OF_RESOURCES) {
    RefreshUsbDeviceGotoOpIndex (
      PrivateData,
      RefreshMode,
      &mH2OUsbViewerSortContext,
      *UsbDeviceCount,
      *UsbHandleInfoArray,
      FirstRemovedPortNumber,
      FirstRemovedGotoOpIndex
      );
  }
  return Status;
}

/**

  @param[in,out] PrivateData
  @param[in]     IfrStatementCount

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
SetIfrStatementCount (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN UINT16                           IfrStatementCount
  )
{
  VOID   *NewBuffer;
  UINTN  OldSize;
  UINTN  NewSize;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (IfrStatementCount > H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Error!! IfrStatementCount (0x%x) Less than H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT (0x%x)\n",
      __FUNCTION__,
      IfrStatementCount,
      H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT
      ));
    return EFI_OUT_OF_RESOURCES;
  }

  if (PrivateData->NumIfrStatementsAllocated < IfrStatementCount) {
    OldSize = ((UINTN) PrivateData->NumIfrStatementsAllocated *
                sizeof (*PrivateData->StatementStringIds));
    NewSize = ((UINTN) IfrStatementCount *
                sizeof (*PrivateData->StatementStringIds));
    NewBuffer = ReallocatePool (
                  OldSize,
                  NewSize,
                  PrivateData->StatementStringIds
                  );
    if (NewBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    PrivateData->StatementStringIds = (EFI_IFR_STATEMENT_HEADER *) NewBuffer;
    PrivateData->NumIfrStatementsAllocated = IfrStatementCount;
  }
  return EFI_SUCCESS;
}

/**

  @param[in,out] PrivateData
  @param[in]     GotoOpCount

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
SetGotoOpCodeCount (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN UINT16                           GotoOpCount
  )
{
  EFI_STATUS  Status;
  UINTN       OldSize;
  VOID        *NewBuffer;

  ASSERT (PrivateData != NULL);
  ASSERT (GotoOpCount <= H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT);

  Status = SetIfrStatementCount (PrivateData, GotoOpCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PrivateData->NumGotoOpsAllocated < GotoOpCount) {
    OldSize = ((UINTN) PrivateData->NumGotoOpsAllocated *
                sizeof (*PrivateData->GotoOpEntries));
    NewBuffer = ReallocatePool (
                  OldSize,
                  ((UINTN) GotoOpCount * sizeof (*PrivateData->GotoOpEntries)),
                  PrivateData->GotoOpEntries
                  );
    if (NewBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    PrivateData->GotoOpEntries = (H2O_USB_VIEWER_GOTO_OP_ENTRY *) NewBuffer;
    PrivateData->NumGotoOpsAllocated = GotoOpCount;
  }
  return EFI_SUCCESS;
}

/**
**/
STATIC
UINT8 *
CreateGrayedOutTextOpCode (
  IN OUT VOID       *OpCodeHandle,
  IN EFI_STRING_ID  Prompt,
  IN EFI_STRING_ID  TextTwo OPTIONAL
  )
{
  struct {
    EFI_IFR_GRAY_OUT_IF  GrayOutIf;
    EFI_IFR_TRUE         True;
    EFI_IFR_TEXT         Text;
    EFI_IFR_END          End;
  } OpCodeList;

  ASSERT (OpCodeHandle != NULL);

  OpCodeList.GrayOutIf.Header.OpCode = EFI_IFR_GRAY_OUT_IF_OP;
  OpCodeList.GrayOutIf.Header.Length = sizeof (EFI_IFR_GRAY_OUT_IF);
  OpCodeList.GrayOutIf.Header.Scope  = 1;

  OpCodeList.True.Header.OpCode      = EFI_IFR_TRUE_OP;
  OpCodeList.True.Header.Length      = sizeof (EFI_IFR_TRUE);
  OpCodeList.True.Header.Scope       = 0;

  //
  // H2O Form Browser Metro Display Engine allows a grayed-out text op-code to
  // be highlighted (by text cursor). The form item would look awkward when
  // help text is empty.
  //
  OpCodeList.Text.Header.OpCode      = EFI_IFR_TEXT_OP;
  OpCodeList.Text.Header.Length      = sizeof (EFI_IFR_TEXT);
  OpCodeList.Text.Header.Scope       = 0;
  OpCodeList.Text.Statement.Prompt   = Prompt;
  OpCodeList.Text.Statement.Help     = Prompt;
  OpCodeList.Text.TextTwo            = TextTwo;

  OpCodeList.End.Header.OpCode       = EFI_IFR_END_OP;
  OpCodeList.End.Header.Length       = sizeof (EFI_IFR_END);
  OpCodeList.End.Header.Scope        = 0;

  return HiiCreateRawOpCodes (
           OpCodeHandle,
           (UINT8 *) &OpCodeList,
           sizeof (OpCodeList)
           );
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
SetDisconnectedDevicePromptString (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN CONST CHAR16                     *DeviceName
  )
{
  EFI_STRING     Format;
  CHAR16         *PromptString;
  EFI_STRING_ID  NewStringId;

  if ((PrivateData == NULL) || (DeviceName == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/DeviceName is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Format = HiiGetString (
             PrivateData->HiiHandle,
             STRING_TOKEN (STR_DISCONNECTED_DEVICE_PROMPT_FORMAT),
             NULL // Language
             );
  if (Format == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  PromptString = CatSPrint (
                   NULL,
                   (CHAR16 *) Format,
                   DeviceName
                   );
  FreePool (Format);
  if (PromptString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  NewStringId = HiiSetString (
                  PrivateData->HiiHandle,
                  PrivateData->DisconnectedDeviceStringId,
                  (EFI_STRING) PromptString,
                  NULL // SupportedLanguages
                  );
  FreePool (PromptString);
  if (NewStringId == 0) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreateUsbPortListHeadingOpCodes (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA          *PrivateData,
  IN OUT VOID                                 *OpCodeHandle,
  IN H2O_USB_VIEWER_REFRESH_MODE              RefreshMode,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntry OPTIONAL,
  IN UINT16                                   HostControllerCount,
  IN CONST USB_PORT_NUMBER_CHAIN              *FirstRemovedPortNumber,
  IN UINT16                                   FirstRemovedGotoOpIndex
  )
{
  CHAR16                 *HcCountSubstring;
  EFI_STRING_ID          PromptStringId;
  EFI_STRING             Format;

  CHAR16                 *PromptString;
  UINT8                  *OpCodePtr;

  EFI_STATUS             Status;
  USB_PORT_NUMBER_CHAIN  ZeroPortNumberChainEntry = {0};
  UINTN                  IndentLength;
  UINT8                  Depth;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL) || (FirstRemovedPortNumber == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle/FirstRemovedPortNumber is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  HcCountSubstring = NULL;
  if (HostControllerCount > 0) {
    Format = HiiGetString (
               PrivateData->HiiHandle,
               STRING_TOKEN (STR_HOST_CONTROLLER_COUNT_SUBSTRING_FORMAT),
               NULL // Language
               );
    if (Format != NULL) {
      HcCountSubstring = CatSPrint (
                           NULL,
                           (CHAR16 *) Format,
                           (unsigned int) HostControllerCount
                           );
      FreePool (Format);
    }
  }

  PromptStringId = STRING_TOKEN (STR_USB_PORTS_LIST);
  if (DownstreamCountEntry == NULL) {
    goto CreateSubtitle;
  }
  Format = HiiGetString (
             PrivateData->HiiHandle,
             STRING_TOKEN (STR_USB_DEVICE_COUNT_SUBTITLE_FORMAT),
             NULL // Language
             );
  if (Format == NULL) {
    goto CreateSubtitle;
  }
  PromptString = CatSPrint (
                   NULL,
                   (CHAR16 *) Format,
                   (UINT64) DownstreamCountEntry->PeripheralCount,
                   (UINT64) (DownstreamCountEntry->HubCount -
                              HostControllerCount),
                   ((HcCountSubstring == NULL) ? L"" : HcCountSubstring)
                   );
  FreePool (Format);
  if (HcCountSubstring != NULL) {
    FreePool (HcCountSubstring);
  }
  if (PromptString == NULL) {
    goto CreateSubtitle;
  }
  PromptStringId = PrivateData->DeviceCountSubtitleStringId;
  (VOID) HiiSetString (
           PrivateData->HiiHandle,
           PromptStringId,
           (EFI_STRING) PromptString,
           NULL // SupportedLanguages
           );
  FreePool (PromptString);

CreateSubtitle:
  OpCodePtr = HiiCreateSubTitleOpCode (
                OpCodeHandle,
                STRING_TOKEN (0x0000), // Prompt
                STRING_TOKEN (0x0000), // Help
                0,                     // Flags
                0                      // Scope
                );
  if (OpCodePtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  OpCodePtr = HiiCreateSubTitleOpCode (
                OpCodeHandle,
                PromptStringId,
                STRING_TOKEN (0x0000), // Help
                0,                     // Flags
                0                      // Scope
                );
  if (OpCodePtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = EFI_SUCCESS;
  if (PrivateData->DisconnectedPortNumberChain.HostControllerNumber != 0) {
    //
    // The form refresh event might be signaled between a "goto" op-code being
    // processed and the new form being displayed on the screen.
    // Don't let user miss the important removal notice.
    //
    if (RefreshMode != H2OUsbViewerDeviceChangeRefresh) {
      PrivateData->DisconnectedPortNumberChain = ZeroPortNumberChainEntry;
    }
    OpCodePtr = CreateGrayedOutTextOpCode (
                  OpCodeHandle,
                  PrivateData->DisconnectedDeviceStringId,
                  STRING_TOKEN (0x0000) // TextTwo
                  );
    if (OpCodePtr == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
    }
    return Status;
  }
  if (RefreshMode == H2OUsbViewerDeviceChangeRefresh &&
      FirstRemovedPortNumber->HostControllerNumber != 0)
  {
    PromptString = (CHAR16 *) HiiGetString (
                                PrivateData->HiiHandle,
                                PrivateData->StatementStringIds[
                                               FirstRemovedGotoOpIndex].Prompt,
                                NULL // Language
                               );
    if (PromptString != NULL) {
      IndentLength = 0;
      if (PrivateData->ViewOption.FlatView != 0) {
        for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
          if (FirstRemovedPortNumber->UsbPortNumbers[Depth] == 0) {
            break;
          }
        }
        IndentLength = Depth * 2;
      }
      Status = SetDisconnectedDevicePromptString (
                 PrivateData,
                 (PromptString + IndentLength)
                 );
      FreePool (PromptString);
      if (!EFI_ERROR (Status)) {
        (VOID) CreateGrayedOutTextOpCode (
                 OpCodeHandle,
                 PrivateData->DisconnectedDeviceStringId,
                 STRING_TOKEN (0x0000) // TextTwo
                 );
      }
    }
  }
  return Status;
}

STATIC
EFI_STATUS
InitializeUsbViewerHcCommonInfo (
  OUT H2O_USB_VIEWER_HC_COMMON_INFO  *CommonInfo,
  IN EFI_USB3_HC_PROTOCOL            *Usb3Hc,
  IN EFI_PCI_IO_PROTOCOL             *PciIo OPTIONAL
  )
{
  EFI_STATUS        Status;
  UINT8             MaxSpeed;
  UINT16            PciConfigReadLength;
  UINT8             *PciConfigBuffer;
  EFI_STATUS        WarnStatus;
  CONST PCI_TYPE00  *PciConfig;
  UINT8             SerialBusReleaseBcd;
  UINT8             HcCapabilityReadLength;
  UINT8             Buffer[0x14];
  UINT32            ExtendedCapabilityOffset;
  UINT32            NextCapability;
  UINT32            Usb2PortCount;
  UINT8             SupportedProtocolCapabilityReadLength;

  if ((CommonInfo == NULL) || (Usb3Hc == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The CommonInfo/Usb3Hc is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (IsZeroBuffer (CommonInfo, sizeof (*CommonInfo)));

  Status = Usb3Hc->GetCapability (
                     Usb3Hc,
                     &MaxSpeed,
                     &CommonInfo->MaxPortNumber,
                     &CommonInfo->Is64BitCapable
                     );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CommonInfo->HcPciInterfaceCode = PCI_IF_USB_OTHER;
  CommonInfo->NumOfUsb2Ports     = CommonInfo->MaxPortNumber;

  switch (MaxSpeed) {

  case EFI_USB_SPEED_FULL:
    CommonInfo->SerialBusReleaseBcd = 0x10;
    break;

  case EFI_USB_SPEED_HIGH:
    CommonInfo->HcPciInterfaceCode  = PCI_IF_EHCI;
    CommonInfo->SerialBusReleaseBcd = 0x20;
    break;

  case EFI_USB_SPEED_SUPER:
    CommonInfo->HcPciInterfaceCode  = PCI_IF_XHCI;
    CommonInfo->SerialBusReleaseBcd = 0x30;
    break;
  }

  if (PciIo == NULL) {
    goto EndInfo;
  }

  PciConfigReadLength = 0x064 * sizeof (UINT8);

  PciConfigBuffer = AllocateZeroPool (PciConfigReadLength);
  if (PciConfigBuffer == NULL) {
    goto EndInfo;
  }
  WarnStatus = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0x000, // Offset
                            (PciConfigReadLength / sizeof (UINT32)),
                            PciConfigBuffer
                            );
  if (EFI_ERROR (WarnStatus)) {
    goto EndInfo;
  }
  PciConfig = (PCI_TYPE00 *) PciConfigBuffer;
  if (!IS_CLASS2 (PciConfig, PCI_CLASS_SERIAL, PCI_CLASS_SERIAL_USB)) {
    goto EndInfo;
  }
  CommonInfo->PciConfigBuffer = PciConfigBuffer;
  CommonInfo->PciConfigBufferSize = PciConfigReadLength;

  switch (PciConfig->Hdr.ClassCode[0]) {

  case PCI_IF_XHCI:
    SerialBusReleaseBcd = ((UINT8 *) PciConfigBuffer)[0x60];
    //
    // Serial Bus Release Number (SBRN). Other values are reserved.
    //
    if (SerialBusReleaseBcd >= 0x30 && SerialBusReleaseBcd <= 0x32) {
      CommonInfo->SerialBusReleaseBcd = SerialBusReleaseBcd;
    }

    //
    // USB 2 and USB 3 are electrically separate buses and have separate port
    // numbers in HCI. It's common for two different port numbers to wire to
    // same connector, thus the number of ports that can simultaneously connect
    // to devices could be much less than what "MaxPorts" (xHCI specification)
    // provides. To minimize user confusion, don't present the sum of USB 2 and
    // USB 3 ports. (This problem is specific to USB 3 / xHCI.)
    //
    HcCapabilityReadLength = 0x14;
    ASSERT (sizeof (Buffer) >= HcCapabilityReadLength);
    WarnStatus = PciIo->Mem.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,    // BarIndex
                              0x00, // Offset
                              (HcCapabilityReadLength / sizeof (UINT32)),
                              Buffer
                              );
    if (EFI_ERROR (WarnStatus)) {
      goto EndInfo;
    }
    //
    // xHCI 1.2 (0120h) is the latest version we referenced.
    //
    CommonInfo->HciRevisionBcd = *(UINT16 *)&((UINT8 *) Buffer)[0x02];
    ExtendedCapabilityOffset = ((*(UINT16 *)&((UINT8 *) Buffer)[0x12]) *
                                 sizeof (UINT32));
    NextCapability = 0;
    Usb2PortCount = 0;
    do {
      ExtendedCapabilityOffset += NextCapability;
      WarnStatus = PciIo->Mem.Read (
                                PciIo,
                                EfiPciIoWidthUint16,
                                0, // BarIndex
                                ExtendedCapabilityOffset,
                                1, // Count
                                Buffer
                                );
      if (EFI_ERROR (WarnStatus)) {
        break;
      }
      NextCapability = ((XHCI_EXTENDED_CAPABILITY_HEADER *) Buffer)->
                         NextCapabilityPointer * sizeof (UINT32);
      if (((XHCI_EXTENDED_CAPABILITY_HEADER *) Buffer)->
          CapabilityId != XhciSupportedProtocolCapability)
      {
        continue;
      }
      SupportedProtocolCapabilityReadLength = 0x0C;
      WarnStatus = PciIo->Mem.Read (
                                PciIo,
                                EfiPciIoWidthUint32,
                                0, // BarIndex
                                ExtendedCapabilityOffset,
                                (SupportedProtocolCapabilityReadLength /
                                  sizeof (UINT32)),
                                Buffer
                                );
      if (EFI_ERROR (WarnStatus)) {
        break;
      }
      if (((XHCI_SUPPORTED_PROTOCOL_CAPABILITY *) Buffer)->
          NameString != SIGNATURE_32 ('U', 'S', 'B', ' '))
      {
        continue;
      }
      if (((XHCI_SUPPORTED_PROTOCOL_CAPABILITY *) Buffer)->
          RevisionMajor < 0x03)
      {
        //
        // The port ranges defined in USB 3 xHCI Supported Protocol Capability
        // and USB 2 xHCI Supported Protocol Capability "shall not overlap".
        //
        Usb2PortCount += ((XHCI_SUPPORTED_PROTOCOL_CAPABILITY *) Buffer)->
                           CompatiblePortCount;
        if (Usb2PortCount > USB_HUB_MAX_PORTS) {
          Usb2PortCount = 0;
          break;
        }
      }
    } while (NextCapability != 0);
    if (Usb2PortCount > 0) {
      CommonInfo->NumOfUsb2Ports = (UINT8) Usb2PortCount;
    }
    break;

  case PCI_IF_EHCI:
    HcCapabilityReadLength = 0x04;
    ASSERT (sizeof (Buffer) >= HcCapabilityReadLength);
    WarnStatus = PciIo->Mem.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,    // BarIndex
                              0x00, // Offset
                              (HcCapabilityReadLength / sizeof (UINT32)),
                              Buffer
                              );
    if (EFI_ERROR (WarnStatus)) {
      goto EndInfo;
    }
    //
    // The value of 0110h indicates EHCI 1.1 Addendum has been implemented.
    //
    CommonInfo->HciRevisionBcd = *(UINT16 *)&((UINT8 *) Buffer)[0x02];
    break;

  case PCI_IF_OHCI:
    CommonInfo->HcPciInterfaceCode = PCI_IF_OHCI;

    HcCapabilityReadLength = 0x04;
    ASSERT (sizeof (Buffer) >= HcCapabilityReadLength);
    WarnStatus = PciIo->Mem.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,    // BarIndex
                              0x00, // Offset
                              (HcCapabilityReadLength / sizeof (UINT32)),
                              Buffer
                              );
    if (EFI_ERROR (WarnStatus)) {
      goto EndInfo;
    }
    //
    // Convert to version format compatible to EHCI and xHCI.
    //
    CommonInfo->HciRevisionBcd = (((UINT8 *) Buffer)[0x00] << 4);
    break;

  case PCI_IF_UHCI:
    CommonInfo->HcPciInterfaceCode = PCI_IF_UHCI;

    SerialBusReleaseBcd = ((UINT8 *) PciConfigBuffer)[0x60];
    //
    // The value of 00h indicates "Pre-release 1.0".
    // Didn't see a value that distinguishes USB 1.1 compliance from 1.0, so we
    // just display 10h as generic "1.x".
    //
    if (SerialBusReleaseBcd < 0x20) {
      CommonInfo->SerialBusReleaseBcd = SerialBusReleaseBcd;
    }
    break;
  }

EndInfo:
  return Status;
}

/**
**/
STATIC
CHAR16 *
CreatePortCountString (
  IN H2O_USB_VIEWER_PRIVATE_DATA              *PrivateData,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntry OPTIONAL,
  IN UINT8                                    PortCount
  )
{
  UINT64      DownstreamPeripheralCount;
  UINT64      DownstreamHubCount;
  EFI_STRING  Format;
  CHAR16      *String;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  DownstreamPeripheralCount = 0;
  DownstreamHubCount        = 0;
  if (DownstreamCountEntry != NULL) {
    DownstreamPeripheralCount = DownstreamCountEntry->PeripheralCount;
    DownstreamHubCount        = DownstreamCountEntry->HubCount;
  }

  if (DownstreamPeripheralCount > 0) {
    Format = HiiGetString (
               PrivateData->HiiHandle,
               STRING_TOKEN (STR_USB_DEVICE_COUNT_SUBSTRING_FORMAT),
               NULL // Language
               );
    if (Format == NULL) {
      return NULL;
    }
    String = CatSPrint (
               NULL,
               (CHAR16 *) Format,
               (UINT64) DownstreamPeripheralCount
               );
  } else if (DownstreamHubCount > 0) {
    Format = HiiGetString (
               PrivateData->HiiHandle,
               STRING_TOKEN (STR_USB_HUB_COUNT_SUBSTRING_FORMAT),
               NULL // Language
               );
    if (Format == NULL) {
      return NULL;
    }
    String = CatSPrint (NULL, (CHAR16 *) Format, (UINT64) DownstreamHubCount);
  } else if (PortCount > 1) {
    Format = HiiGetString (
               PrivateData->HiiHandle,
               STRING_TOKEN (STR_USB_PORT_COUNT_SUBSTRING_FORMAT),
               NULL // Language
               );
    if (Format == NULL) {
      return NULL;
    }
    String = CatSPrint (NULL, (CHAR16 *) Format, (unsigned int) PortCount);
  } else {
    return NULL;
  }
  FreePool (Format);
  return String;
}

/**
**/
STATIC
CHAR16 *
CreateHostControllerGotoPromptString (
  IN H2O_USB_VIEWER_PRIVATE_DATA              *PrivateData,
  IN UINT16                                   HostControllerNumber,
  IN CONST CHAR16                             *HostControllerName OPTIONAL,
  IN CONST H2O_USB_VIEWER_HC_COMMON_INFO      *CommonInfo,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntry OPTIONAL
  )
{
  EFI_STRING             Format;
  CHAR16                 *HostControllerDisplayName;
  UINT8                  SerialBusReleaseBcd;
  EFI_STRING_ID          StringId;
  CHAR16                 *UsbSpecVersionString;
  EFI_STRING             HcInterfaceString;
  CHAR16                 *PortCountString;
  CHAR16                 *PromptString;

  if ((PrivateData == NULL) || (CommonInfo == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/CommonInfo is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  PromptString = NULL;
  Format = HiiGetString (
             PrivateData->HiiHandle,
             STRING_TOKEN (STR_USB_HOST_GOTO_OP_ENTRY_FORMAT),
             NULL // Language
             );
  if (Format == NULL) {
    goto FormatStringFail;
  }

  if (HostControllerName != NULL) {
    HostControllerDisplayName = CatSPrint (
                                  NULL,
                                  L"%s",
                                  HostControllerName
                                  );
  } else {
    HostControllerDisplayName = CatSPrint (
                                  NULL,
                                  L"%u",
                                  (unsigned int) HostControllerNumber
                                  );
  }
  if (HostControllerDisplayName == NULL) {
    goto HcDisplayNameFail;
  }

  SerialBusReleaseBcd = CommonInfo->SerialBusReleaseBcd;
  if (SerialBusReleaseBcd < 0x20) {
    if (SerialBusReleaseBcd < 0x10) {
      StringId = STRING_TOKEN (STR_USB_PRE_1_0);
    } else {
      StringId = STRING_TOKEN (STR_USB_1_X);
    }
    UsbSpecVersionString = (CHAR16 *) HiiGetString (
                                        PrivateData->HiiHandle,
                                        StringId,
                                        NULL // Language
                                        );
  } else {
    UsbSpecVersionString = CatSPrint (
                             NULL,
                             L"%u.%u",
                             (unsigned int) (SerialBusReleaseBcd >> 4),
                             (unsigned int) (SerialBusReleaseBcd & 0x0F)
                             );
  }
  if (UsbSpecVersionString == NULL) {
    goto UsbVersionStringFail;
  }

  switch (CommonInfo->HcPciInterfaceCode) {

  case PCI_IF_UHCI:
    StringId = STRING_TOKEN (STR_UHCI);
    break;

  case PCI_IF_OHCI:
    StringId = STRING_TOKEN (STR_OHCI);
    break;

  case PCI_IF_EHCI:
    StringId = STRING_TOKEN (STR_EHCI);
    break;

  case PCI_IF_XHCI:
    StringId = STRING_TOKEN (STR_XHCI);
    break;

  default:
    StringId = STRING_TOKEN (STR_OTHER_HOST_INTERFACE);
    break;
  }
  HcInterfaceString = HiiGetString (
                        PrivateData->HiiHandle,
                        StringId,
                        NULL // Language
                        );
  if (HcInterfaceString == NULL) {
    goto HcInterfaceStringFail;
  }

  PortCountString = NULL;
  if (PrivateData->ViewOption.FlatView == 0) {
    PortCountString = CreatePortCountString (
                        PrivateData,
                        DownstreamCountEntry,
                        CommonInfo->NumOfUsb2Ports
                        );
  }
  PromptString = CatSPrint (
                   NULL,
                   (CHAR16 *) Format,
                   HostControllerDisplayName,
                   UsbSpecVersionString,
                   (CHAR16 *) HcInterfaceString,
                   ((PortCountString == NULL) ? L"" : PortCountString)
                   );
  if (PortCountString != NULL) {
    FreePool (PortCountString);
  }
  FreePool (HcInterfaceString);
HcInterfaceStringFail:
  FreePool (UsbSpecVersionString);
UsbVersionStringFail:
  FreePool (HostControllerDisplayName);
HcDisplayNameFail:
  FreePool (Format);
FormatStringFail:

  return PromptString;
}

/**
**/
STATIC
CHAR16 *
CreateHostControllerGotoHelpString (
  IN H2O_USB_VIEWER_PRIVATE_DATA              *PrivateData,
  IN CONST H2O_USB_VIEWER_HC_COMMON_INFO      *CommonInfo,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntry OPTIONAL
  )
{
  UINT64         DownstreamPeripheralCount;
  UINT64         DownstreamHubCount;

  CHAR16         *HelpString;
  EFI_STRING_ID  FormatStringId;
  EFI_STRING     Format;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  DownstreamPeripheralCount = 0;
  DownstreamHubCount        = 0;
  if (DownstreamCountEntry != NULL) {
    DownstreamPeripheralCount = DownstreamCountEntry->PeripheralCount;
    DownstreamHubCount        = DownstreamCountEntry->HubCount;
  }

  FormatStringId = STRING_TOKEN (STR_USB_HOST_CONTROLLER_GOTO_HELP_FORMAT);
  Format = HiiGetString (
             PrivateData->HiiHandle,
             FormatStringId,
             NULL // Language
             );
  if (Format == NULL) {
    return NULL;
  }
  HelpString = CatSPrint (
                 NULL,
                 (CHAR16 *) Format,
                 (UINT64) DownstreamPeripheralCount,
                 (UINT64) DownstreamHubCount
                 );
  FreePool (Format);
  return HelpString;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
SetIfrStatementStrings (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN UINT16                           ItemIndex,
  IN CONST CHAR16                     *PromptString,
  IN CONST CHAR16                     *HelpString OPTIONAL
  )
{
  EFI_STRING_ID  NewStringId;

  if ((PrivateData == NULL) || (PromptString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/PromptString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (ItemIndex < PrivateData->NumIfrStatementsAllocated);

  NewStringId = HiiSetString (
                  PrivateData->HiiHandle,
                  PrivateData->StatementStringIds[ItemIndex].Prompt,
                  (CONST EFI_STRING) PromptString,
                  NULL // SupportedLanguages
                  );
  if (NewStringId == 0) {
    return EFI_OUT_OF_RESOURCES;
  }
  PrivateData->StatementStringIds[ItemIndex].Prompt = NewStringId;

  if (HelpString == NULL) {
    return EFI_SUCCESS;
  }

  NewStringId = HiiSetString (
                  PrivateData->HiiHandle,
                  PrivateData->StatementStringIds[ItemIndex].Help,
                  (CONST EFI_STRING) HelpString,
                  NULL // SupportedLanguages
                  );
  if (NewStringId == 0) {
    return EFI_OUT_OF_RESOURCES;
  }
  PrivateData->StatementStringIds[ItemIndex].Help = NewStringId;

  return EFI_SUCCESS;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES
  @retval  EFI_BUFFER_TOO_SMALL

**/
STATIC
EFI_STATUS
CreateGotoOpCodeEntry (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA          *PrivateData,
  IN OUT VOID                                 *OpCodeHandle,
  IN CONST USB_HANDLE_INFO_ENTRY              *UsbHandleInfoEntry,
  IN EFI_FORM_ID                              DestinationFormId,
  IN CONST CHAR16                             *PromptString OPTIONAL,
  IN CONST CHAR16                             *HelpString OPTIONAL
  )
{
  UINT16                        GotoOpIndex;
  EFI_STATUS                    Status;
  EFI_QUESTION_ID               QuestionId;
  UINT8                         *OpCodePtr;
  H2O_USB_VIEWER_GOTO_OP_ENTRY  *GotoOpEntry;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL) || (UsbHandleInfoEntry == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle/UsbHandleInfoEntry is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  GotoOpIndex = UsbHandleInfoEntry->GotoOpIndex;
  if (GotoOpIndex >= PrivateData->NumGotoOpsAllocated) {
    return EFI_BUFFER_TOO_SMALL;
  }

  if (PromptString != NULL) {
    Status = SetIfrStatementStrings (
               PrivateData,
               GotoOpIndex,
               PromptString,
               ((HelpString == NULL) ? L"" : HelpString)
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  QuestionId = (H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE + GotoOpIndex);

  OpCodePtr = HiiCreateGotoOpCode (
                OpCodeHandle,
                DestinationFormId,
                PrivateData->StatementStringIds[GotoOpIndex].Prompt,
                PrivateData->StatementStringIds[GotoOpIndex].Help,
                EFI_IFR_FLAG_CALLBACK,
                QuestionId
                );
  if (OpCodePtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // GotoOpEntries is a linked list and array combined.
  // The array is indexed by (Question ID - base).
  // An index link is included in each entry so the list can be traversed in
  // the reverse order of USB port numbers.
  //
  if (PrivateData->GotoOpEntries == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GotoOpEntry = &PrivateData->GotoOpEntries[GotoOpIndex];
  CopyMem (
    GotoOpEntry,
    UsbHandleInfoEntry,
    OFFSET_OF (H2O_USB_VIEWER_GOTO_OP_ENTRY, PreviousDeviceGotoOpIndex)
    );
  GotoOpEntry->PreviousDeviceGotoOpIndex = PrivateData->LastDeviceGotoOpIndex;
  GotoOpEntry->HasDownstreamDevice = UsbHandleInfoEntry->HasDownstreamDevice;
  GotoOpEntry->Reserved = 0;

  PrivateData->LastDeviceGotoOpIndex = GotoOpIndex;

  return EFI_SUCCESS;
}

STATIC
VOID
CleanUpUsbViewerHcCommonInfo (
  IN H2O_USB_VIEWER_HC_COMMON_INFO  *CommonInfo
  )
{
  if (CommonInfo->PciConfigBuffer != NULL) {
    FreePool (CommonInfo->PciConfigBuffer);
  }
}

STATIC
EFI_STATUS
CreateHostControllerGotoOpCode (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA          *PrivateData,
  IN OUT VOID                                 *OpCodeHandle,
  IN USB_HANDLE_INFO_ENTRY                    *UsbHandleInfoEntry,
  IN CONST CHAR16                             *HostControllerName OPTIONAL,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntry OPTIONAL
  )
{
  EFI_TPL                        OldTpl;
  EFI_STATUS                     Status;
  EFI_USB3_HC_PROTOCOL           *Usb3Hc;
  EFI_STATUS                     WarnStatus;
  EFI_PCI_IO_PROTOCOL            *PciIo;
  H2O_USB_VIEWER_HC_COMMON_INFO  HcCommonInfo = {0};
  CHAR16                         *PromptString;
  CHAR16                         *HelpString;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL) || (UsbHandleInfoEntry == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle/UsbHandleInfoEntry is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = GetProtocolWithCorrectStatus (
             UsbHandleInfoEntry->Handle,
             &gEfiUsb3HcProtocolGuid,
             (VOID **) &Usb3Hc
             );
  if (!EFI_ERROR (Status)) {
    WarnStatus = GetProtocolWithCorrectStatus (
                   UsbHandleInfoEntry->Handle,
                   &gEfiPciIoProtocolGuid,
                   (VOID **) &PciIo
                   );
    if (EFI_ERROR (WarnStatus)) {
      PciIo = NULL;
    }
    Status = InitializeUsbViewerHcCommonInfo (
               &HcCommonInfo,
               Usb3Hc,
               PciIo
               );
  }
  gBS->RestoreTPL (OldTpl);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PromptString = CreateHostControllerGotoPromptString (
                   PrivateData,
                   UsbHandleInfoEntry->PortNumberChain.HostControllerNumber,
                   HostControllerName,
                   &HcCommonInfo,
                   DownstreamCountEntry
                   );
  if (PromptString == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto PromptStringFail;
  }
  HelpString = CreateHostControllerGotoHelpString (
                 PrivateData,
                 &HcCommonInfo,
                 DownstreamCountEntry
                 );
  Status = CreateGotoOpCodeEntry (
             PrivateData,
             OpCodeHandle,
             UsbHandleInfoEntry,
             H2O_USB_VIEWER_HOST_CONTROLLER_FORM_ID,
             PromptString,
             HelpString
             );
  FreePool (PromptString);
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

PromptStringFail:
  CleanUpUsbViewerHcCommonInfo (&HcCommonInfo);

  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
CreateUsbPreferredLangIdArray (
  IN EFI_USB_IO_PROTOCOL  *UsbIo,
  IN UINT16               PreferredLangId OPTIONAL,
  OUT UINT16              **LangIdArray,
  OUT UINT8               *LangIdCount
  )
{
  EFI_STATUS    Status;
  CONST UINT16  *SupportedLangIds; // Buffer is maintained by USB Bus Driver.
  UINT16        LangIdSize;
  BOOLEAN       PreferredLangIsSupported;
  UINTN         LangIndex;

  if ((UsbIo == NULL) || (LangIdArray == NULL) || (LangIdCount == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UsbIo/LangIdArray/LangIdCount is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PreferredLangIsSupported = FALSE;
  Status = UsbIo->UsbGetSupportedLanguages (
                    UsbIo,
                    (UINT16 **) &SupportedLangIds,
                    &LangIdSize
                    );
  //
  // UsbBusDxe in InsydeModulePkg might violate specification by returning
  // EFI_NOT_FOUND. (UEFI specification only permits EFI_SUCCESS.)
  //
  if (EFI_ERROR (Status)) {
    LangIdSize = 0;
  }

  *LangIdArray = NULL;
  *LangIdCount = (UINT8) (LangIdSize / sizeof (UINT16));
  if ((*LangIdCount) <= 0) {
    return EFI_SUCCESS;
  }
  for (LangIndex = 0; LangIndex < (*LangIdCount); LangIndex++) {
    if (PreferredLangId != 0x0000 &&
        SupportedLangIds[LangIndex] == PreferredLangId)
    {
      PreferredLangIsSupported = TRUE;
    }
    //
    // TODO: Find a way to utilize "PlatformLang" EFI global variable
    //
  }
  *LangIdCount += ((PreferredLangIsSupported) ? sizeof (UINT16) : 0);

  *LangIdArray = AllocatePool ((*LangIdCount) * sizeof (UINT16));
  if ((*LangIdArray) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  LangIndex = 0;
  if (PreferredLangIsSupported) {
    (*LangIdArray)[LangIndex++] = PreferredLangId;
  }
  CopyMem (&(*LangIdArray)[LangIndex], SupportedLangIds, LangIdSize);

  return EFI_SUCCESS;
}

STATIC
BOOLEAN
IsUnicodeWhiteSpace (
  IN INT32  Char
  )
{
  //
  // All Unicode characters with White_Space property.
  // See https://www.unicode.org/reports/tr44/#White_Space
  // https://www.unicode.org/Public/UCD/latest/ucd/PropList.txt
  //
  return (
    Char == L' ' ||
    (Char <= L'\r' && Char >= L'\t') ||
    Char == 0x0085 || // Next Line (NEL)
    Char == 0x00A0 || // No-Break Space (NBSP)
    Char == 0x1680 || // Ogham Space Mark
    (Char >= 0x2000 && Char <= 0x200A) ||
    Char == 0x2028 || // Line Separator
    Char == 0x2029 || // Paragraph Separator
    Char == 0x202F || // Narrow No-Break Space
    Char == 0x205F || // Medium Mathematical Space
    Char == 0x3000    // Ideographic Space
    );
}

STATIC
BOOLEAN
IsUnicodeBidiEmbeddingInitiator (
  IN INT32  Char
  )
{
  //
  // https://www.unicode.org/reports/tr9/#BD10
  //
  return (
    Char == 0x202A || // Left-to-Right Embedding (LRE)
    Char == 0x202B || // Right-to-Left Embedding (RLE)
    Char == 0x202D || // Right-to-Left Override (LRO)
    Char == 0x202E    // Right-to-Left Override (RLO)
    );
}

STATIC
BOOLEAN
IsUnicodeBidiIsolateInitiator (
  IN INT32  Char
  )
{
  //
  // Unicode 6.3 or later
  // https://www.unicode.org/reports/tr9/#BD8
  //
  return (
    Char == 0x2066 || // Left-to-Right Isolate (LRI)
    Char == 0x2067 || // Right-to-Left Isolate (RLI)
    Char == 0x2068    // First Strong Isolate (FSI)
    );
}

STATIC
UINTN
CountUnterminatedBidiEmbeddings (
  IN CONST CHAR16  *UserString,
  IN UINTN         CharIndex,
  IN UINTN         UnterminatedIsolateCount
  )
{
  UINT16  PopDirectionalFormatting; // (PDF)
  UINT16  PopDirectionalIsolate;    // (PDI)

  UINTN   RelativeIsolateDepth;
  UINTN   UnmatchedPdfCount;
  UINTN   UnterminatedEmbeddingCount;

  PopDirectionalFormatting = 0x202C;
  PopDirectionalIsolate    = 0x2069;

  RelativeIsolateDepth = 0;
  UnmatchedPdfCount = 0;
  UnterminatedEmbeddingCount = 0;

  CharIndex--;
  for (; CharIndex != (UINTN) -1; CharIndex--) {
    if (IsUnicodeBidiEmbeddingInitiator (UserString[CharIndex])) {
      if (RelativeIsolateDepth > 0) {
        continue;
      }
      if (UnmatchedPdfCount > 0) {
        UnmatchedPdfCount--;
        continue;
      }
      ASSERT (UnterminatedEmbeddingCount < MAX_UINTN);
      UnterminatedEmbeddingCount++;
      continue;
    }

    if (UserString[CharIndex] == PopDirectionalFormatting) {
      if (RelativeIsolateDepth > 0) {
        continue;
      }
      ASSERT (UnmatchedPdfCount < MAX_UINTN);
      UnmatchedPdfCount++;
      continue;
    }

    if (IsUnicodeBidiIsolateInitiator (UserString[CharIndex])) {
      if (RelativeIsolateDepth > 0) {
        RelativeIsolateDepth--;
        continue;
      }
      if (UnterminatedIsolateCount > 0) {
        UnterminatedIsolateCount--;
        continue;
      }
      break;
    }

    if (UserString[CharIndex] == PopDirectionalIsolate) {
      ASSERT (RelativeIsolateDepth < MAX_UINTN);
      RelativeIsolateDepth++;
      continue;
    }
  }

  ASSERT (RelativeIsolateDepth == 0);
  return UnterminatedEmbeddingCount;
}

STATIC
CHAR16 *
SanitizeAndCopyUnicodeString (
  IN CONST CHAR16  *UserString
  )
{
  UINT16  PopDirectionalFormatting; // (PDF)
  UINT16  PopDirectionalIsolate;    // (PDI)
  UINT16  SymbolForDelete;
  UINT16  ReplacementCharacter;

  UINTN   Length;
  UINTN   BidiIsolateDepth;
  UINTN   BidiEmbeddingCount;
  UINTN   Index;
  UINTN   Count;
  UINTN   AllocateSize;
  CHAR16  *Buffer;
  UINTN   BufferIndex;

  PopDirectionalFormatting = 0x202C;
  PopDirectionalIsolate    = 0x2069;
  SymbolForDelete          = 0x2421;
  ReplacementCharacter     = 0xFFFD;

  if (UserString == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UserString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  ASSERT (UserString != NULL);
  while (IsUnicodeWhiteSpace (*UserString)) {
    ++UserString;
  }

  Length = 0;
  BidiIsolateDepth = 0;
  BidiEmbeddingCount = 0;
  for (Index = 0; UserString[Index] != '\0'; Index++) {
    if (Index > MAX_UINTN - 2) {
      //
      // Treat arithmetic overflow as out of resources.
      //
      return NULL;
    }
    if (!IsUnicodeWhiteSpace (UserString[Index])) {
      Length = Index + 1;
    }

    if (IsUnicodeBidiIsolateInitiator (UserString[Index])) {
      ASSERT (BidiIsolateDepth < MAX_UINTN);
      BidiIsolateDepth++;
      continue;
    }

    if (UserString[Index] == PopDirectionalIsolate) {
      if (BidiIsolateDepth <= 0) {
        continue;
      }
      BidiIsolateDepth--;
      Count = CountUnterminatedBidiEmbeddings (UserString, Index, 0);
      if (BidiEmbeddingCount > MAX_UINTN - Count) {
        return NULL;
      }
      BidiEmbeddingCount += Count;
      continue;
    }
  }

  Count = CountUnterminatedBidiEmbeddings (
            UserString,
            Length,
            BidiIsolateDepth
            );
  if (BidiEmbeddingCount > MAX_UINTN - Count) {
    return NULL;
  }
  BidiEmbeddingCount += Count;

  if (Length + 1 > MAX_UINTN - BidiIsolateDepth ||
      Length + 1 + BidiIsolateDepth >
        MAX_UINTN / sizeof (CHAR16) - BidiEmbeddingCount)
  {
    return NULL;
  }

  AllocateSize =
    (Length + 1 + BidiIsolateDepth + BidiEmbeddingCount) * sizeof (CHAR16);

  Buffer = AllocatePool (AllocateSize);
  if (Buffer == NULL) {
    return NULL;
  }

  //
  // Begin copying and sanitizing the string.
  //
  BidiEmbeddingCount = 0;
  BidiIsolateDepth = 0;
  Index = 0;
  BufferIndex = 0;
  for (; Index < Length; Index++, BufferIndex++) {
    if (UserString[Index] <= 0x1F) {
      //
      // Unicode "Control Pictures"
      //
      Buffer[BufferIndex] = 0x2400 + UserString[Index];
      continue;
    }
    if (UserString[Index] == 0x7F) {
      Buffer[BufferIndex] = SymbolForDelete;
      continue;
    }
    if ((UserString[Index] >= 0x80 && UserString[Index] <= 0x9F) ||
        UserString[Index] == 0x2028 || UserString[Index] == 0x2029)
    {
      Buffer[BufferIndex] = ReplacementCharacter;
      continue;
    }

    Buffer[BufferIndex] = UserString[Index];

    //
    // Sanitize Unicode bidi control characters.
    //
    if (IsUnicodeBidiEmbeddingInitiator (UserString[Index])) {
      ASSERT (BidiEmbeddingCount < MAX_UINTN);
      BidiEmbeddingCount++;
      continue;
    }

    if (UserString[Index] == PopDirectionalFormatting) {
      if (BidiEmbeddingCount <= 0) {
        Buffer[BufferIndex] = ReplacementCharacter;
        continue;
      }
      BidiEmbeddingCount--;
      continue;
    }

    if (IsUnicodeBidiIsolateInitiator (UserString[Index])) {
      ASSERT (BidiIsolateDepth < MAX_UINTN);
      BidiIsolateDepth++;
      BidiEmbeddingCount = 0;
      continue;
    }

    if (UserString[Index] == PopDirectionalIsolate) {
      if (BidiIsolateDepth <= 0) {
        Buffer[BufferIndex] = ReplacementCharacter;
        continue;
      }

      for (; BidiEmbeddingCount > 0; BidiEmbeddingCount--) {
        Buffer[BufferIndex] = PopDirectionalFormatting;
        BufferIndex++;
      }
      BidiIsolateDepth--;
      Buffer[BufferIndex] = PopDirectionalIsolate;
      BidiEmbeddingCount = CountUnterminatedBidiEmbeddings (
                             Buffer,
                             (BufferIndex + 1),
                             0
                             );
      continue;
    }
  }

  //
  // Terminate all unterminated bidi isolates and embeddings. We do this to
  // avoid UI spoofing that makes (untrusted) user strings displayed outside
  // the contained area or quotation marks.
  // This is a defense in depth protection.
  //
  while (TRUE) {
    for (; BidiEmbeddingCount > 0; BidiEmbeddingCount--) {
      Buffer[BufferIndex] = PopDirectionalFormatting;
      BufferIndex++;
    }
    if (BidiIsolateDepth <= 0) {
      break;
    }
    BidiIsolateDepth--;
    Buffer[BufferIndex] = PopDirectionalIsolate;
    BidiEmbeddingCount = CountUnterminatedBidiEmbeddings (
                           Buffer,
                           (BufferIndex + 1),
                           0
                           );
    BufferIndex++;
  }

  Buffer[BufferIndex] = '\0';
  return Buffer;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
GetUsbStringInPreferredLanguage (
  IN EFI_USB_IO_PROTOCOL  *UsbIo,
  IN UINT16               *LangIdArray,
  IN UINT8                LangIdCount,
  IN UINT8                StringDescriptorIndex,
  OUT CHAR16              **String
  )
{
  EFI_STATUS  Status;
  UINTN       LangIndex;
  CHAR16      *TempString;

  if ((UsbIo == NULL) || (LangIdArray == NULL) || (String == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UsbIo/LangIdArray/String is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_NOT_FOUND;
  *String = NULL;
  for (LangIndex = 0; LangIndex < LangIdCount; LangIndex++) {
    Status = UsbIo->UsbGetStringDescriptor (
                      UsbIo,
                      LangIdArray[LangIndex],
                      StringDescriptorIndex,
                      &TempString
                      );
    if (Status == EFI_NOT_FOUND) {
      continue;
    }
    if (!EFI_ERROR (Status)) {
      *String = SanitizeAndCopyUnicodeString (TempString);
      gBS->FreePool (TempString);
      if ((*String) == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
      }
    }
    break;
  }
  return Status;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
InitializeViewerUsbCommonDescriptors (
  OUT H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS  *CommonDescriptors,
  IN EFI_USB_IO_PROTOCOL                     *UsbIo,
  IN UINT16                                  PreferredLangId OPTIONAL
  )
{
  EFI_STATUS  Status;

  if ((CommonDescriptors == NULL) || (UsbIo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsZeroBuffer (CommonDescriptors, sizeof (*CommonDescriptors))) {
    return EFI_NOT_READY;
  }

  Status = UsbIo->UsbGetDeviceDescriptor (
                    UsbIo,
                    &CommonDescriptors->DeviceDescriptor
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = UsbIo->UsbGetConfigDescriptor (
                    UsbIo,
                    &CommonDescriptors->ConfigurationDescriptor
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = CreateUsbPreferredLangIdArray (
             UsbIo,
             PreferredLangId,
             &CommonDescriptors->PreferredLangIdArray,
             &CommonDescriptors->LangIdCount
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CommonDescriptors->DeviceDescriptor.StrManufacturer != 0) {
    (VOID) GetUsbStringInPreferredLanguage (
             UsbIo,
             CommonDescriptors->PreferredLangIdArray,
             CommonDescriptors->LangIdCount,
             CommonDescriptors->DeviceDescriptor.StrManufacturer,
             &CommonDescriptors->ManufacturerString
             );
  }

  if (CommonDescriptors->DeviceDescriptor.StrProduct != 0) {
    (VOID) GetUsbStringInPreferredLanguage (
             UsbIo,
             CommonDescriptors->PreferredLangIdArray,
             CommonDescriptors->LangIdCount,
             CommonDescriptors->DeviceDescriptor.StrProduct,
             &CommonDescriptors->ProductString
             );
  }

  if (CommonDescriptors->DeviceDescriptor.StrSerialNumber != 0) {
    (VOID) GetUsbStringInPreferredLanguage (
             UsbIo,
             CommonDescriptors->PreferredLangIdArray,
             CommonDescriptors->LangIdCount,
             CommonDescriptors->DeviceDescriptor.StrSerialNumber,
             &CommonDescriptors->SerialNumberString
             );
  }

  if (CommonDescriptors->ConfigurationDescriptor.Configuration != 0) {
    (VOID) GetUsbStringInPreferredLanguage (
             UsbIo,
             CommonDescriptors->PreferredLangIdArray,
             CommonDescriptors->LangIdCount,
             CommonDescriptors->ConfigurationDescriptor.Configuration,
             &CommonDescriptors->ConfigurationString
             );
  }

  return Status;
}

STATIC
UINTN
PrintUsbPortNumberChainString (
  OUT CHAR16                      *StringBuffer,
  IN UINTN                        BufferSize,
  IN CONST USB_PORT_NUMBER_CHAIN  *UsbPortNumberChain
  )
{
  UINTN   TotalCharCount;
  UINTN   CharCount;
  UINTN   Depth;
  CHAR16  SeparatorChar;

  if ((StringBuffer == NULL) || (UsbPortNumberChain == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The StringBuffer/UsbPortNumberChain is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  TotalCharCount = 0;
  CharCount = UnicodeSPrint (
                (StringBuffer + TotalCharCount),
                BufferSize,
                L"%u",
                (unsigned int) (UsbPortNumberChain->HostControllerNumber)
                );
  TotalCharCount += CharCount;

  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
    if (UsbPortNumberChain->UsbPortNumbers[Depth] == 0) {
      break;
    }
    if (BufferSize <= TotalCharCount * sizeof (CHAR16)) {
      break;
    }
    SeparatorChar = L'-';
    CharCount = UnicodeSPrintAsciiFormat (
                  (StringBuffer + TotalCharCount),
                  (BufferSize - TotalCharCount * sizeof (CHAR16)),
                  "%c%u",
                  (UINTN) SeparatorChar,
                  (unsigned int) UsbPortNumberChain->UsbPortNumbers[Depth]
                  );
    TotalCharCount += CharCount;
  }
  return TotalCharCount;
}

/**

**/
STATIC
CHAR16 *
CreateUsbDeviceDisplayName (
  IN CONST H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS  *CommonDescriptors
  )
{
  CONST CHAR16  *ManufacturerString;
  CONST CHAR16  *ProductString;
  CONST CHAR16  *ProductIdPrintFormat;

  if (CommonDescriptors == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The CommonDescriptors is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  ManufacturerString = CommonDescriptors->ManufacturerString;
  ProductString      = CommonDescriptors->ProductString;

  //
  // In Windows, "VID_%04X&PID_%04X"
  // In 'lsusb' (Linux utility), "ID %04x:%04x"
  //
  ProductIdPrintFormat = L"VID_%04X&PID_%04X";

  if (ManufacturerString == NULL && ProductString == NULL) {
    return CatSPrint (
             NULL,
             ProductIdPrintFormat,
             (unsigned int) CommonDescriptors->DeviceDescriptor.IdVendor,
             (unsigned int) CommonDescriptors->DeviceDescriptor.IdProduct
             );
  } else {
    return CatSPrint (
             NULL,
             L"%s%a%s",
             ((ManufacturerString == NULL) ? L"" : ManufacturerString),
             ((ManufacturerString == NULL || ProductString == NULL) ?
               "" : " "),
             ((ProductString == NULL) ? L"" : ProductString)
             );
  }
}

STATIC
UINTN
SPrintLengthFixedArgs (
  IN CONST CHAR16  *FormatString,
  ...
  )
{
  VA_LIST  Marker;
  UINTN    Length;

  VA_START (Marker, FormatString);
  Length = SPrintLength (FormatString, Marker);
  VA_END (Marker);
  return Length;
}

STATIC
CHAR16 *
CreateNumberWithDescriptionString (
  IN CONST CHAR16  *NumberFormat,
  IN CONST CHAR16  *DescriptionFormat,
  IN CONST VOID    *DescriptionString OPTIONAL,
  ...
  )
{
  VA_LIST  Marker;
  UINTN    CharCount;
  UINTN    BufferSize;
  CHAR16   *ResultString;

  ASSERT (NumberFormat != NULL);
  ASSERT (DescriptionFormat != NULL);

  VA_START (Marker, DescriptionString);
  CharCount = SPrintLength (NumberFormat, Marker);
  VA_END (Marker);
  if (DescriptionString != NULL) {
    CharCount += SPrintLengthFixedArgs (DescriptionFormat, DescriptionString);
  }

  BufferSize = (CharCount + 1) * sizeof (CHAR16);
  ResultString = AllocatePool (BufferSize);
  if (ResultString == NULL) {
    return NULL;
  }

  VA_START (Marker, DescriptionString);
  CharCount = UnicodeVSPrint (
                ResultString,
                BufferSize,
                NumberFormat,
                Marker
                );
  VA_END (Marker);
  if (DescriptionString != NULL) {
    ASSERT (BufferSize >= CharCount * sizeof (CHAR16));
    (VOID) UnicodeSPrint (
             (ResultString + CharCount),
             (BufferSize - CharCount * sizeof (CHAR16)),
             DescriptionFormat,
             DescriptionString
             );
  }
  return ResultString;
}

/**
**/
STATIC
CHAR16 *
CreateUsbDeviceGotoHelpString (
  IN H2O_USB_VIEWER_PRIVATE_DATA                  *PrivateData,
  IN CONST CHAR16                                 *PortName OPTIONAL,
  IN CONST CHAR16                                 *PortNumberChainString,
  IN CONST H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS  *CommonDescriptors,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY      *DownstreamCountEntry OPTIONAL
  )
{
  CONST CHAR16                 *ManufacturerString;
  CONST CHAR16                 *ProductString;
  CONST CHAR16                 *SerialNumberString;
  CONST USB_DEVICE_DESCRIPTOR  *DeviceDescriptor;
  CONST USB_CONFIG_DESCRIPTOR  *ConfigurationDescriptor;
  UINT64                       DownstreamPeripheralCount;
  UINT64                       DownstreamHubCount;

  CHAR16                       *HelpString;
  EFI_STRING_ID                FormatStringId;
  EFI_STRING                   Format;
  EFI_STRING                   NotAvailableString;
  CHAR16                       *PortDisplayName;
  CHAR16                       VendorIdStringBuffer[8];

  if ((PrivateData == NULL) || (PortNumberChainString == NULL) || (CommonDescriptors == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/PortNumberChainString/CommonDescriptors is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  ManufacturerString      = CommonDescriptors->ManufacturerString;
  ProductString           = CommonDescriptors->ProductString;
  SerialNumberString      = CommonDescriptors->SerialNumberString;
  DeviceDescriptor        = &CommonDescriptors->DeviceDescriptor;
  ConfigurationDescriptor = &CommonDescriptors->ConfigurationDescriptor;

  DownstreamPeripheralCount = 0;
  DownstreamHubCount        = 0;
  if (DownstreamCountEntry != NULL) {
    DownstreamPeripheralCount = DownstreamCountEntry->PeripheralCount;
    DownstreamHubCount        = DownstreamCountEntry->HubCount;
  }

  if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE) {
    FormatStringId = STRING_TOKEN (STR_USB_HUB_GOTO_HELP_FORMAT);
  } else {
    FormatStringId = STRING_TOKEN (STR_USB_DEVICE_GOTO_HELP_FORMAT);
  }
  HelpString = NULL;
  Format = HiiGetString (
             PrivateData->HiiHandle,
             FormatStringId,
             NULL // Language
             );
  if (Format == NULL) {
    goto FormatStringFail;
  }
  NotAvailableString = HiiGetString (
                         PrivateData->HiiHandle,
                         STRING_TOKEN (STR_NOT_AVAILABLE),
                         NULL // Language
                         );
  if (NotAvailableString == NULL) {
    goto NaStringFail;
  }
  PortDisplayName = NULL;
  if (PortName != NULL && PortName[0] != L'\0') {
    PortDisplayName = CatSPrint (
                        NULL,
                        L"%s\n",
                        PortName
                        );
  }

  if (ManufacturerString == NULL) {
    UnicodeSPrintAsciiFormat (
      VendorIdStringBuffer,
      sizeof (VendorIdStringBuffer),
      "(%04Xh)",
      DeviceDescriptor->IdVendor
      );
    ManufacturerString = VendorIdStringBuffer;
  }
  if (ProductString == NULL) {
    ProductString = (CONST CHAR16 *) NotAvailableString;
  }
  if (SerialNumberString == NULL) {
    SerialNumberString = (CONST CHAR16 *) NotAvailableString;
  }
  if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE) {
    HelpString = CatSPrint (
                   NULL,
                   Format,
                   ((PortDisplayName == NULL) ? L"" : PortDisplayName),
                   PortNumberChainString,
                   (UINT64) DownstreamPeripheralCount,
                   (UINT64) DownstreamHubCount,
                   ManufacturerString,
                   ProductString,
                   SerialNumberString,
                   (unsigned int) DeviceDescriptor->BcdDevice
                   );
  } else {
    HelpString = CatSPrint (
                   NULL,
                   Format,
                   ((PortDisplayName == NULL) ? L"" : PortDisplayName),
                   PortNumberChainString,
                   ManufacturerString,
                   ProductString,
                   SerialNumberString,
                   (unsigned int) DeviceDescriptor->BcdDevice,
                   (unsigned int) ConfigurationDescriptor->NumInterfaces
                   );
  }
  if (PortDisplayName != NULL) {
    FreePool (PortDisplayName);
  }
  FreePool (NotAvailableString);
NaStringFail:
  FreePool (Format);
FormatStringFail:
  return HelpString;
}

/**
**/
STATIC
VOID
CleanUpViewerUsbCommonDescriptors (
  IN H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS  *CommonDescriptors
  )
{
  CHAR16  **StringPtr;
  CHAR16  **LastString;

  if (CommonDescriptors->PreferredLangIdArray != NULL) {
    FreePool (CommonDescriptors->PreferredLangIdArray);
  }

  StringPtr  = &CommonDescriptors->ManufacturerString;
  LastString = &CommonDescriptors->ConfigurationString;
  while (StringPtr <= LastString) {
    if ((*StringPtr) != NULL) {
      FreePool (*StringPtr);
    }
    StringPtr++;
  }
}

/**
  Creates an EFI_IFR_REF ("Goto") op-code to a USB device subform.

  @param[in,out] PrivateData
  @param[in,out] OpCodeHandle        Handle which the new op-codes will
                                     append to
  @param[in]     RefreshMode
  @param[in]     UsbHandleInfoEntry
  @param[in]     DownstreamCountEntry

  @retval EFI_SUCCESS           Op-code has been created successfully.
  @retval EFI_UNSUPPORTED
  @retval EFI_DEVICE_ERROR
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_TIMEOUT
  @retval EFI_BUFFER_TOO_SMALL

**/
STATIC
EFI_STATUS
CreateUsbDeviceGotoOpCode (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA          *PrivateData,
  IN OUT VOID                                 *OpCodeHandle,
  IN H2O_USB_VIEWER_REFRESH_MODE              RefreshMode,
  IN USB_HANDLE_INFO_ENTRY                    *UsbHandleInfoEntry,
  IN CONST CHAR16                             *PortName,
  IN CONST USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntry OPTIONAL
  )
{
  UINT8                                  Depth;
  BOOLEAN                                IsExistingDevice;
  EFI_TPL                                OldTpl;
  EFI_STATUS                             Status;
  EFI_USB_IO_PROTOCOL                    *UsbIo;
  H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS  CommonDescriptors = {0};
  CONST EFI_USB_DEVICE_DESCRIPTOR        *DeviceDescriptor;
  CONST EFI_USB_CONFIG_DESCRIPTOR        *ConfigurationDescriptor;

  VOID                                   *HubDescriptor;
  UINT32                                 TransferResult;
  UINT8                                  PortCount;
  EFI_STATUS                             WarnStatus;

  BOOLEAN                                ShouldRefreshPrompt;
  CHAR16                                 *PromptString;
  CHAR16                                 *HelpString;

  CONST CHAR8                            IndentString[] = "------------- ";
  UINTN                                  IndentStringOffset;
  UINTN                                  StringLength;
  CHAR16                                 PortNumberChainString[
                                           USB_PORT_NUMBER_CHAIN_STRING_SIZE /
                                           sizeof (CHAR16)];
  CHAR16                                 *DeviceDisplayName;
  CHAR16                                 *PortCountString;
  EFI_STRING                             NewTagString;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL) || (UsbHandleInfoEntry == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle/UsbHandleInfoEntry is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (UsbHandleInfoEntry->PortNumberChain.UsbPortNumbers[0] != 0);
  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH - 1; Depth++) {
    if (UsbHandleInfoEntry->PortNumberChain.UsbPortNumbers[Depth + 1] == 0) {
      break;
    }
  }

  IsExistingDevice =
    (CompareMem (
       &PrivateData->GotoOpEntries[UsbHandleInfoEntry->GotoOpIndex],
       UsbHandleInfoEntry,
       OFFSET_OF (H2O_USB_VIEWER_GOTO_OP_ENTRY, PreviousDeviceGotoOpIndex)
       ) == 0);

  //
  // Raise TPL to block hotplug events and prevent uninstallation of any
  // protocol. A potential use-after-free could happen if we don't do this.
  //
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  Status = GetProtocolWithCorrectStatus (
             UsbHandleInfoEntry->Handle,
             &gEfiUsbIoProtocolGuid,
             (VOID **) &UsbIo
             );
  if (!EFI_ERROR (Status)) {
    Status = InitializeViewerUsbCommonDescriptors (
               &CommonDescriptors,
               UsbIo,
               PrivateData->ViewOption.SelectedLangId
               );
    if (Status == EFI_NOT_FOUND) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  DeviceDescriptor        = &CommonDescriptors.DeviceDescriptor;
  ConfigurationDescriptor = &CommonDescriptors.ConfigurationDescriptor;

  HubDescriptor  = NULL;
  TransferResult = EFI_USB_NOERROR;
  PortCount      = 0;
  if (!EFI_ERROR (Status)) {
    if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE) {
      WarnStatus = GetUsbHubDescriptor (
                     UsbIo,
                     DeviceDescriptor->BcdUSB,
                     (VOID **) &HubDescriptor,
                     &TransferResult
                     );
      if (!EFI_ERROR (WarnStatus) && (HubDescriptor != NULL)) {
        PortCount = ((UINT8 *) HubDescriptor)[2];
      } else if (IsExistingDevice) {
        Status = WarnStatus;
      }
    }
  }
  gBS->RestoreTPL (OldTpl);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ShouldRefreshPrompt = ((!IsExistingDevice) ||
                          RefreshMode != H2OUsbViewerDeviceChangeRefresh ||
                          DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE);
  if (!ShouldRefreshPrompt) {
    //
    // It is possible that UsbGetDeviceDescriptor() from EFI USB I/O Protocol
    // return cached data while UsbGetStringDescriptor() doesn't use cache.
    // If a device disconnects when such happens, our prompt string can get
    // "updated" with malformed device names. Avoid that.
    //
    ShouldRefreshPrompt = (DeviceDescriptor->StrManufacturer == 0 ||
                            CommonDescriptors.ManufacturerString != NULL) &&
                          (DeviceDescriptor->StrProduct == 0 ||
                            CommonDescriptors.ProductString != NULL) &&
                          (DeviceDescriptor->StrManufacturer == 0 ||
                            CommonDescriptors.ManufacturerString != NULL) &&
                          (ConfigurationDescriptor->Configuration == 0 ||
                            CommonDescriptors.ConfigurationString != NULL);
  }

  if (!ShouldRefreshPrompt) {
    PromptString = NULL;
    HelpString = NULL;
  } else {
    if (PrivateData->ViewOption.FlatView != 0) {
      ASSERT (ARRAY_SIZE (IndentString) >= (Depth + 1) * 2 + 1);
      IndentStringOffset = ARRAY_SIZE (IndentString) - ((Depth + 1) * 2 + 1);
    } else {
      IndentStringOffset = ARRAY_SIZE (IndentString) - 1;
    }

    StringLength = PrintUsbPortNumberChainString (
                     PortNumberChainString,
                     sizeof (PortNumberChainString),
                     &UsbHandleInfoEntry->PortNumberChain
                     );

    DeviceDisplayName = CreateUsbDeviceDisplayName (&CommonDescriptors);
    if (DeviceDisplayName == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto PromptStringFail;
    }

    PortCountString = NULL;
    if (PrivateData->ViewOption.FlatView == 0) {
      PortCountString = CreatePortCountString (
                          PrivateData,
                          DownstreamCountEntry,
                          PortCount
                          );
    }

    NewTagString = NULL;
    if ((!IsExistingDevice) &&
        RefreshMode == H2OUsbViewerDeviceChangeRefresh)
    {
      NewTagString = HiiGetString (
                       PrivateData->HiiHandle,
                       STRING_TOKEN (STR_USB_NEW_DEVICE_SUBSTRING),
                       NULL // Language
                       );
    }

    PromptString = CatSPrint (
                     NULL,
                     L"%a%s%s: %s%s%s",
                     (IndentString + IndentStringOffset),
                     ((PortName == NULL) ? L"" : PortName),
                     ((PortName == NULL) ? PortNumberChainString : L""),
                     DeviceDisplayName,
                     ((PortCountString == NULL) ? L"" : PortCountString),
                     ((NewTagString == NULL) ? L"" : (CHAR16 *) NewTagString)
                     );
    if (NewTagString != NULL) {
      FreePool (NewTagString);
    }
    if (PortCountString != NULL) {
      FreePool (PortCountString);
    }
    FreePool (DeviceDisplayName);
    if (PromptString == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto PromptStringFail;
    }
    HelpString = CreateUsbDeviceGotoHelpString (
                   PrivateData,
                   PortName,
                   PortNumberChainString,
                   &CommonDescriptors,
                   DownstreamCountEntry
                   );
  }
  Status = CreateGotoOpCodeEntry (
             PrivateData,
             OpCodeHandle,
             UsbHandleInfoEntry,
             (H2O_USB_VIEWER_DEPTH_0_DEVICE_FORM_ID + Depth),
             PromptString,
             HelpString
             );
  if (HelpString != NULL) {
    FreePool (HelpString);
  }
  if (PromptString != NULL) {
    FreePool (PromptString);
  }
PromptStringFail:
  CleanUpViewerUsbCommonDescriptors (&CommonDescriptors);

  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_MEDIA_CHANGED
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
BuildEntryForm (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN H2O_USB_VIEWER_REFRESH_MODE      RefreshMode
  )
{
  EFI_STATUS                         Status;
  UINTN                              HostControllerCount;
  EFI_HANDLE                         *HostControllerHandles;

  UINTN                              AllocateSize;
  EFI_DEVICE_PATH_PROTOCOL           **HostControllerDevicePaths;
  UINT16                             *NameDatabaseHcNumbers;
  EFI_TPL                            OldTpl;
  UINTN                              Index;
  EFI_STATUS                         WarnStatus;

  UINTN                              UsbDeviceCount;
  USB_HANDLE_INFO_ENTRY              *UsbHandleInfoArray;
  USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *NewDsDeviceCountArray;
  USB_PORT_NUMBER_CHAIN              FirstRemovedPortNumber;
  UINT16                             FirstRemovedGotoOpIndex;

  BOOLEAN                            HasDisconnectedDevice;
  UINTN                              GotoOpCodeCount;
  UINT16                             GotoOpIndex;
  UINTN                              DsCountEntryCount;
  UINTN                              NewDsCountEntryIndex;
  BOOLEAN                            HasDownstream;
  H2O_USB_VIEWER_GOTO_OP_ENTRY       *GotoOpEntry;
  CHAR16                             *PortName;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup;
  UINT16                             NameDatabaseHcNumber;
  USB_PORT_NUMBER_CHAIN              PortNumberChainBuffer;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get host controller handles.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiUsb3HcProtocolGuid,
                  NULL, // SearchKey
                  &HostControllerCount,
                  &HostControllerHandles
                  );
  if (Status == EFI_NOT_FOUND) {
    HostControllerCount = 0;
  } else if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HostControllerCount == 0) {
    (VOID) HiiCreateSubTitleOpCode (
             OpCodeHandle,
             STRING_TOKEN (STR_NO_USB_HOST_CONTROLLER),
             STRING_TOKEN (0x0000), // Help
             0,                     // Flags
             0                      // Scope
             );
    if (PrivateData->NumGotoOpsAllocated == 0 ||
        PrivateData->LastDeviceGotoOpIndex < PrivateData->NumGotoOpsAllocated)
    {
      PrivateData->LastDeviceGotoOpIndex = 0xFFFF;
      return EFI_MEDIA_CHANGED;
    }
    return EFI_SUCCESS;
  }
  if (HostControllerCount > H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT) {
    DEBUG ((
      EFI_D_WARN,
      "%a: Very large number of USB host controller handles (%lu).\n",
      gEfiCallerBaseName,
      (UINT64) HostControllerCount
      ));
    HostControllerCount = H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT;
  }

  //
  // Get host controller device paths.
  // Raise TPL to block hotplug events and prevent uninstallation of any
  // protocol. A potential use-after-free could happen if we don't do this.
  //
  AllocateSize = HostControllerCount *
                   (sizeof (EFI_DEVICE_PATH_PROTOCOL *) + sizeof (UINT16));
  HostControllerDevicePaths = AllocatePool (AllocateSize);
  if (HostControllerDevicePaths == NULL) {
    gBS->FreePool (HostControllerHandles);
    return EFI_OUT_OF_RESOURCES;
  }
  NameDatabaseHcNumbers = (UINT16 *)
                            (HostControllerDevicePaths + HostControllerCount);
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  for (Index = 0; Index < HostControllerCount; Index++) {
    WarnStatus = GetProtocolWithCorrectStatus (
                   HostControllerHandles[Index],
                   &gEfiDevicePathProtocolGuid,
                   (VOID **) &HostControllerDevicePaths[Index]
                   );
    if (EFI_ERROR (WarnStatus)) {
      DEBUG ((
        EFI_D_WARN,
        "%a: Can't open Device Path for USB host controller handle %p: %r\n",
        gEfiCallerBaseName,
        HostControllerHandles[Index],
        WarnStatus
        ));
      HostControllerDevicePaths[Index] = NULL;
    }
  }
  Status = CollectUsbDeviceInfoAndRestoreTpl (
             PrivateData,
             (UINT16) HostControllerCount,
             (CONST EFI_DEVICE_PATH_PROTOCOL **) HostControllerDevicePaths,
             HostControllerHandles,
             OldTpl,
             RefreshMode,
             NameDatabaseHcNumbers,
             &UsbDeviceCount,
             &UsbHandleInfoArray,
             &NewDsDeviceCountArray,
             &FirstRemovedPortNumber,
             &FirstRemovedGotoOpIndex
             );
  gBS->FreePool (HostControllerHandles);
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    goto CollectDeviceFail;
  }

  Status = EFI_SUCCESS;
  if (RefreshMode != H2OUsbViewerDeviceChangeRefresh ||
      FirstRemovedPortNumber.HostControllerNumber != 0)
  {
    Status = EFI_MEDIA_CHANGED;
  }

  HasDisconnectedDevice =
    PrivateData->DisconnectedPortNumberChain.HostControllerNumber != 0 ||
    (RefreshMode == H2OUsbViewerDeviceChangeRefresh &&
      FirstRemovedPortNumber.HostControllerNumber != 0);

  //
  // Count the number of "goto" op-codes we need to create.
  //
  if (PrivateData->ViewOption.FlatView != 0) {
    GotoOpCodeCount = UsbDeviceCount;
    if (GotoOpCodeCount > H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT) {
      GotoOpCodeCount = H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT;
    }
  } else {
    GotoOpCodeCount = HostControllerCount;
  }
  (VOID) SetGotoOpCodeCount (PrivateData, (UINT16) GotoOpCodeCount);

  (VOID) CreateUsbPortListHeadingOpCodes (
           PrivateData,
           OpCodeHandle,
           RefreshMode,
           NewDsDeviceCountArray,
           (UINT16) HostControllerCount,
           &FirstRemovedPortNumber,
           FirstRemovedGotoOpIndex
           );
  if ((PrivateData->DownstreamCountEntries == NULL) !=
      (NewDsDeviceCountArray == NULL))
  {
    Status = EFI_MEDIA_CHANGED;
  }
  if (Status == EFI_SUCCESS) {
    if ((PrivateData->DownstreamCountEntries != NULL) &&
        (NewDsDeviceCountArray != NULL) &&
        CompareMem (
          &PrivateData->DownstreamCountEntries[0],
          &NewDsDeviceCountArray[0],
          sizeof (USB_DOWNSTREAM_DEVICE_COUNT_ENTRY)
          ) != 0)
    {
      Status = EFI_MEDIA_CHANGED;
    }
  }

  GotoOpIndex = 0;
  DsCountEntryCount = 1;
  NewDsCountEntryIndex = 1;
  for (Index = 0; Index < UsbDeviceCount; Index++) {
    HasDownstream = (NewDsDeviceCountArray != NULL &&
                      UsbHandleInfoArray[Index].HasDownstreamDevice != 0);
    if (UsbHandleInfoArray[Index].PortNumberChain.UsbPortNumbers[0] != 0 &&
        PrivateData->ViewOption.FlatView == 0)
    {
      if (HasDownstream) {
        NewDsCountEntryIndex++;
      }
      continue;
    }

    if (UsbHandleInfoArray[Index].GotoOpIndex >= PrivateData->
                                                   NumGotoOpsAllocated)
    {
      while (GotoOpIndex < PrivateData->NumGotoOpsAllocated &&
          PrivateData->GotoOpEntries[GotoOpIndex].PortNumberChain.
                                                    HostControllerNumber != 0)
      {
        GotoOpIndex++;
      }
      if (GotoOpIndex >= PrivateData->NumGotoOpsAllocated) {
        break;
      }

      UsbHandleInfoArray[Index].GotoOpIndex = GotoOpIndex;
      Status = EFI_MEDIA_CHANGED;
    }

    GotoOpEntry = &PrivateData->GotoOpEntries[
                                  UsbHandleInfoArray[Index].GotoOpIndex];
    if (HasDownstream != (GotoOpEntry->HasDownstreamDevice != 0)) {
      Status = EFI_MEDIA_CHANGED;
    }
    //
    // "DsCountEntryCount" index might exceed the allocated size of
    // "PrivateData->DownstreamCountEntries".
    //
    if (Status == EFI_SUCCESS && HasDownstream &&
        CompareMem (
          &PrivateData->DownstreamCountEntries[DsCountEntryCount],
          &NewDsDeviceCountArray[NewDsCountEntryIndex],
          sizeof (USB_DOWNSTREAM_DEVICE_COUNT_ENTRY)
          ) != 0) {
      Status = EFI_MEDIA_CHANGED;
    }

    PortName = NULL;
    DeviceInfoLookup = PrivateData->DeviceInfoLookup;
    if ((DeviceInfoLookup != NULL) && (DeviceInfoLookup->Usb != NULL)) {
      NameDatabaseHcNumber = NameDatabaseHcNumbers[
                               UsbHandleInfoArray[Index].
                                 PortNumberChain.HostControllerNumber - 1];

      PortNumberChainBuffer = UsbHandleInfoArray[Index].PortNumberChain;
      PortNumberChainBuffer.HostControllerNumber = NameDatabaseHcNumber;
      WarnStatus = DeviceInfoLookup->Usb->GetUsbPortInfoByNumberChain (
                                            DeviceInfoLookup,
                                            &PortNumberChainBuffer,
                                            NULL, // PcdTokenNumber
                                            NULL, // AcpiPath
                                            &PortName,
                                            NULL  // DownstreamPortCount
                                            );
      if (EFI_ERROR (WarnStatus)) {
        PortName = NULL;
      }
    }

    if (UsbHandleInfoArray[Index].PortNumberChain.UsbPortNumbers[0] == 0) {
      WarnStatus = CreateHostControllerGotoOpCode (
                     PrivateData,
                     OpCodeHandle,
                     &UsbHandleInfoArray[Index],
                     PortName,
                     (HasDownstream ?
                       &NewDsDeviceCountArray[NewDsCountEntryIndex] : NULL)
                     );
    } else {
      WarnStatus = CreateUsbDeviceGotoOpCode (
                     PrivateData,
                     OpCodeHandle,
                     RefreshMode,
                     &UsbHandleInfoArray[Index],
                     PortName,
                     (HasDownstream ?
                       &NewDsDeviceCountArray[NewDsCountEntryIndex] : NULL)
                     );
    }
    if (PortName != NULL) {
      FreePool (PortName);
    }
    if (HasDownstream) {
      if (DsCountEntryCount < NewDsCountEntryIndex) {
        NewDsDeviceCountArray[DsCountEntryCount] = NewDsDeviceCountArray[
                                                     NewDsCountEntryIndex];
      }
      DsCountEntryCount++;
      NewDsCountEntryIndex++;
      ASSERT (DsCountEntryCount <= NewDsCountEntryIndex);
    }
    if (EFI_ERROR (WarnStatus)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a: Can't make op-code for USB device (handle %p): %r\n",
        gEfiCallerBaseName,
        UsbHandleInfoArray[Index].Handle,
        WarnStatus
        ));
    }
    switch (WarnStatus) {

    case EFI_UNSUPPORTED:
    case EFI_TIMEOUT:
    case EFI_DEVICE_ERROR:
      if (!HasDisconnectedDevice) {
        //
        // Queue a refresh event for a new "device disconnected" message.
        // The current loop still needs to be completed for logging other
        // existing devices.
        //
        gBS->SignalEvent (PrivateData->FormRefreshEvent);
        HasDisconnectedDevice = TRUE;
      }
      break;
    }
  }

  if (PrivateData->DownstreamCountEntries != NULL) {
    FreePool (PrivateData->DownstreamCountEntries);
  }
  PrivateData->DownstreamCountEntries = NewDsDeviceCountArray;

  FreePool (UsbHandleInfoArray);
CollectDeviceFail:
  FreePool (HostControllerDevicePaths);

  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_BUFFER_TOO_SMALL
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
CreateTextOpCodeWithValueString (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN UINT16                           ItemIndex,
  IN EFI_STRING_ID                    PromptStringId OPTIONAL,
  IN EFI_STRING_ID                    HelpStringId OPTIONAL,
  IN CONST CHAR16                     *HelpString OPTIONAL,
  IN CONST CHAR16                     *ValueString
  )
{
  EFI_STATUS       Status;
  EFI_QUESTION_ID  QuestionId;
  UINT8            *OpCodePtr;

  ASSERT (HelpString != NULL || HelpStringId != STRING_TOKEN (0x0000));

  if ((PrivateData == NULL) || (OpCodeHandle == NULL) || (ValueString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/ControllerEntry/PromptString/HelpString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (ItemIndex >= PrivateData->NumIfrStatementsAllocated) {
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = SetIfrStatementStrings (
             PrivateData,
             ItemIndex,
             ValueString,
             HelpString
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HelpString != NULL) {
    HelpStringId = PrivateData->StatementStringIds[ItemIndex].Help;
  }

  //
  // H2O Form Browser Text Mode renders a text op-code without the ability
  // to highlight it by text cursor. Creating action op-code instead is a usual
  // workaround, but note that Metro Display Engine (graphical mode) renders
  // text and action op-codes differently.
  //
  if (FeaturePcdGet (PcdH2OFormBrowserLocalTextDESupported)) {
    QuestionId = (H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE + ItemIndex);
    if (PromptStringId != STRING_TOKEN (0x0000)) {
      OpCodePtr = HiiCreateActionOpCodeEx (
                    OpCodeHandle,
                    QuestionId,
                    PromptStringId,
                    HelpStringId,
                    PrivateData->StatementStringIds[ItemIndex].Prompt,
                    0x00,                 // QuestionFlags,
                    STRING_TOKEN (0x0000) // QuestionConfig
                    );
    } else {
      OpCodePtr = HiiCreateActionOpCode (
                    OpCodeHandle,
                    QuestionId,
                    PrivateData->StatementStringIds[ItemIndex].Prompt,
                    HelpStringId,
                    0x00,                 // QuestionFlags,
                    STRING_TOKEN (0x0000) // QuestionConfig
                    );
    }
  } else {
    if (PromptStringId != STRING_TOKEN (0x0000)) {
      OpCodePtr = HiiCreateTextOpCode (
                    OpCodeHandle,
                    PromptStringId,
                    HelpStringId,
                    PrivateData->StatementStringIds[ItemIndex].Prompt
                    );
    } else {
      OpCodePtr = HiiCreateTextOpCode (
                    OpCodeHandle,
                    PrivateData->StatementStringIds[ItemIndex].Prompt,
                    HelpStringId,
                    STRING_TOKEN (0x0000) // TextTwo
                    );
    }
  }
  if (OpCodePtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**

  @param[in,out] PrivateData
  @param[in,out] OpCodeHandle
  @param[in]     ItemIndex
  @param[in]     PromptStringId
  @param[in]     HelpStringId
  @param[in]     ValueStringFormat

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES
  @retval  EFI_BUFFER_TOO_SMALL

**/
STATIC
EFI_STATUS
CreateTextOpCodeWithFormattedValue (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN UINT16                           ItemIndex,
  IN EFI_STRING_ID                    PromptStringId OPTIONAL,
  IN EFI_STRING_ID                    HelpStringId OPTIONAL,
  IN CONST CHAR16                     *ValueStringFormat,
  ...
  )
{
  VA_LIST     Marker;
  CHAR16      *StringBuffer;
  EFI_STATUS  Status;

  VA_START (Marker, ValueStringFormat);
  StringBuffer = CatVSPrint (
                   NULL,
                   ValueStringFormat,
                   Marker
                   );
  VA_END (Marker);

  if (StringBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = CreateTextOpCodeWithValueString (
             PrivateData,
             OpCodeHandle,
             ItemIndex,
             PromptStringId,
             HelpStringId,
             NULL, // HelpString
             StringBuffer
             );
  FreePool (StringBuffer);
  return Status;
}

STATIC
EFI_STATUS
CreatePciLocationTextOpCode (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN UINT16                           ItemIndex,
  IN EFI_PCI_IO_PROTOCOL              *PciIo
  )
{
  EFI_STATUS     Status;
  UINTN          SegmentNum;
  UINTN          BusNum;
  UINTN          DeviceNum;
  UINTN          FunctionNum;

  EFI_STRING_ID  FormatStringIds[2];
  INT8           Index;
  EFI_STRING     Format;
  CHAR16         *StringBuffers[2] = {NULL};

  if ((PrivateData == NULL) || (PciIo == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/PciIo is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIo->GetLocation (
                    PciIo,
                    &SegmentNum,
                    &BusNum,
                    &DeviceNum,
                    &FunctionNum
                    );
  ASSERT_EFI_ERROR (Status);

  if (SegmentNum != 0) {
    FormatStringIds[0] = STRING_TOKEN (STR_PCI_LOCATION_VALUE_FORMAT_FULL);
    FormatStringIds[1] = STRING_TOKEN (STR_PCI_LOCATION_HELP_FORMAT_FULL);
  } else {
    FormatStringIds[0] = STRING_TOKEN (STR_PCI_LOCATION_VALUE_FORMAT_NOSEG);
    FormatStringIds[1] = STRING_TOKEN (STR_PCI_LOCATION_HELP_FORMAT_NOSEG);
  }

  for (Index = 0; Index < 2; Index++) {
    Format = HiiGetString (
               PrivateData->HiiHandle,
               FormatStringIds[Index],
               NULL // Language
               );
    if (Format == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeStrings;
    }

    if (SegmentNum != 0) {
      StringBuffers[Index] = CatSPrint (
                               NULL,
                               (CHAR16 *) Format,
                               (unsigned int) SegmentNum,
                               (unsigned int) BusNum,
                               (unsigned int) DeviceNum,
                               (unsigned int) FunctionNum
                               );
    } else {
      StringBuffers[Index] = CatSPrint (
                               NULL,
                               (CHAR16 *) Format,
                               (unsigned int) BusNum,
                               (unsigned int) DeviceNum,
                               (unsigned int) FunctionNum
                               );
    }
    FreePool (Format);
    if (StringBuffers[Index] == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeStrings;
    }
  }
  Status = CreateTextOpCodeWithValueString (
             PrivateData,
             OpCodeHandle,
             ItemIndex,
             STRING_TOKEN (STR_PCI_LOCATION_PROMPT),
             STRING_TOKEN (0x0000), // HelpStringId
             StringBuffers[1],
             StringBuffers[0]
             );

FreeStrings:
  while ((--Index) >= 0) {
    FreePool (StringBuffers[Index]);
  }

  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_MEDIA_CHANGED
  @retval  EFI_UNSUPPORTED
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
CreateUsbPortOpCodeList (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN H2O_USB_VIEWER_REFRESH_MODE      RefreshMode,
  IN UINT8                            NumberOfPorts
  )
{
  UINTN                              Tier;
  EFI_HANDLE                         UsbDeviceHandle;
  EFI_TPL                            OldTpl;
  EFI_STATUS                         Status;
  CONST EFI_DEVICE_PATH_PROTOCOL     *DevicePath;

  UINT16                             NameDatabaseHcNumber;
  UINTN                              UsbDeviceCount;
  USB_HANDLE_INFO_ENTRY              *UsbHandleInfoArray;
  USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *NewDsDeviceCountArray;
  USB_PORT_NUMBER_CHAIN              FirstRemovedPortNumber;
  UINT16                             FirstRemovedGotoOpIndex;

  BOOLEAN                            HasDisconnectedDevice;
  UINT16                             GotoOpIndex;
  UINTN                              DsCountEntryCount;
  UINTN                              NewDsCountEntryIndex;
  UINTN                              Index;
  EFI_STATUS                         WarnStatus;
  BOOLEAN                            HasDownstream;
  H2O_USB_VIEWER_GOTO_OP_ENTRY       *GotoOpEntry;
  CHAR16                             *PortName;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup;
  USB_PORT_NUMBER_CHAIN              PortNumberChainBuffer;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->ViewOption.FlatView != 0) {
    return EFI_SUCCESS;
  }

  Tier = (UINTN) PrivateData->ActiveForm - H2O_USB_VIEWER_HOST_CONTROLLER_FORM_ID + 1;
  if ((Tier == 0) || (Tier >= USB_PORT_MAX_DEPTH + 2)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Tier (0x%x) is invalid\n",
      __FUNCTION__,
      Tier
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (Tier == 1) {
    UsbDeviceHandle = PrivateData->SelectedHostControllerHandle;
  } else {
    UsbDeviceHandle = PrivateData->SelectedUsbDeviceHandleStack[Tier - 2];
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = GetProtocolWithCorrectStatus (
             UsbDeviceHandle,
             &gEfiDevicePathProtocolGuid,
             (VOID **) &DevicePath
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = CollectUsbDeviceInfoAndRestoreTpl (
             PrivateData,
             1,    // DevicePathPrefixCount
             &DevicePath,
             NULL, // HostControllerHandles
             OldTpl,
             RefreshMode,
             &NameDatabaseHcNumber,
             &UsbDeviceCount,
             &UsbHandleInfoArray,
             &NewDsDeviceCountArray,
             &FirstRemovedPortNumber,
             &FirstRemovedGotoOpIndex
             );
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    return Status;
  }

  Status = EFI_SUCCESS;
  if (RefreshMode != H2OUsbViewerDeviceChangeRefresh ||
      FirstRemovedPortNumber.HostControllerNumber != 0)
  {
    Status = EFI_MEDIA_CHANGED;
  }

  HasDisconnectedDevice =
    PrivateData->DisconnectedPortNumberChain.HostControllerNumber != 0 ||
    (RefreshMode == H2OUsbViewerDeviceChangeRefresh &&
      FirstRemovedPortNumber.HostControllerNumber != 0);

  (VOID) CreateUsbPortListHeadingOpCodes (
           PrivateData,
           OpCodeHandle,
           RefreshMode,
           NewDsDeviceCountArray,
           0, // HostControllerCount
           &FirstRemovedPortNumber,
           FirstRemovedGotoOpIndex
           );
  if ((PrivateData->DownstreamCountEntries == NULL) !=
      (NewDsDeviceCountArray == NULL))
  {
    Status = EFI_MEDIA_CHANGED;
  }
  if (Status == EFI_SUCCESS) {
    if ((PrivateData->DownstreamCountEntries != NULL) &&
        (NewDsDeviceCountArray != NULL) &&
        CompareMem (
          &PrivateData->DownstreamCountEntries[0],
          &NewDsDeviceCountArray[0],
          sizeof (USB_DOWNSTREAM_DEVICE_COUNT_ENTRY)
          ) != 0)
    {
      Status = EFI_MEDIA_CHANGED;
    }
  }

  GotoOpIndex = 0;
  DsCountEntryCount = 1;
  NewDsCountEntryIndex = 1;
  for (Index = 0; Index < UsbDeviceCount; Index++) {
    WarnStatus = EFI_SUCCESS;
    HasDownstream = (NewDsDeviceCountArray != NULL &&
                      UsbHandleInfoArray[Index].HasDownstreamDevice != 0);
    if (Tier < USB_PORT_MAX_DEPTH &&
        UsbHandleInfoArray[Index].PortNumberChain.UsbPortNumbers[Tier] != 0)
    {
      if (HasDownstream) {
        NewDsCountEntryIndex++;
      }
      continue;
    }

    if (UsbHandleInfoArray[Index].GotoOpIndex >= PrivateData->
                                                   NumGotoOpsAllocated)
    {
      while (GotoOpIndex < PrivateData->NumGotoOpsAllocated &&
          PrivateData->GotoOpEntries[GotoOpIndex].PortNumberChain.
                                                    HostControllerNumber != 0)
      {
        GotoOpIndex++;
      }
      if (GotoOpIndex >= PrivateData->NumGotoOpsAllocated) {
        break;
      }

      UsbHandleInfoArray[Index].GotoOpIndex = GotoOpIndex;
      Status = EFI_MEDIA_CHANGED;
    }

    GotoOpEntry = &PrivateData->GotoOpEntries[
                                  UsbHandleInfoArray[Index].GotoOpIndex];
    if (HasDownstream != (GotoOpEntry->HasDownstreamDevice != 0)) {
      Status = EFI_MEDIA_CHANGED;
    }
    //
    // "DsCountEntryCount" index might exceed the allocated size of
    // "PrivateData->DownstreamCountEntries".
    //
    if (Status == EFI_SUCCESS && HasDownstream &&
        CompareMem (
          &PrivateData->DownstreamCountEntries[DsCountEntryCount],
          &NewDsDeviceCountArray[NewDsCountEntryIndex],
          sizeof (USB_DOWNSTREAM_DEVICE_COUNT_ENTRY)
          ) != 0) {
      Status = EFI_MEDIA_CHANGED;
    }

    PortName = NULL;
    DeviceInfoLookup = PrivateData->DeviceInfoLookup;
    if ((DeviceInfoLookup != NULL) && (DeviceInfoLookup->Usb != NULL)) {
      PortNumberChainBuffer = UsbHandleInfoArray[Index].PortNumberChain;
      PortNumberChainBuffer.HostControllerNumber = NameDatabaseHcNumber;
      WarnStatus = DeviceInfoLookup->Usb->GetUsbPortInfoByNumberChain (
                                            DeviceInfoLookup,
                                            &PortNumberChainBuffer,
                                            NULL, // PcdTokenNumber
                                            NULL, // AcpiPath
                                            &PortName,
                                            NULL  // DownstreamPortCount
                                            );
      if (EFI_ERROR (WarnStatus)) {
        PortName = NULL;
      }
    }

    WarnStatus = CreateUsbDeviceGotoOpCode (
                   PrivateData,
                   OpCodeHandle,
                   RefreshMode,
                   &UsbHandleInfoArray[Index],
                   PortName,
                   (HasDownstream ?
                     &NewDsDeviceCountArray[NewDsCountEntryIndex] : NULL)
                   );
    if (PortName != NULL) {
      FreePool (PortName);
    }
    if (HasDownstream) {
      if (DsCountEntryCount < NewDsCountEntryIndex) {
        NewDsDeviceCountArray[DsCountEntryCount] = NewDsDeviceCountArray[
                                                     NewDsCountEntryIndex];
      }
      DsCountEntryCount++;
      NewDsCountEntryIndex++;
      ASSERT (DsCountEntryCount <= NewDsCountEntryIndex);
    }
    if (EFI_ERROR (WarnStatus)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a: Can't make op-code for USB device (handle %p): %r\n",
        gEfiCallerBaseName,
        UsbHandleInfoArray[Index].Handle,
        WarnStatus
        ));
    }
    switch (WarnStatus) {

    case EFI_UNSUPPORTED:
    case EFI_TIMEOUT:
    case EFI_DEVICE_ERROR:
      if (!HasDisconnectedDevice) {
        //
        // Queue a refresh event for a new "device disconnected" message.
        // The current loop still needs to be completed for logging other
        // existing devices.
        //
        gBS->SignalEvent (PrivateData->FormRefreshEvent);
        HasDisconnectedDevice = TRUE;
      }
      break;
    }
  }

  if (PrivateData->DownstreamCountEntries != NULL) {
    FreePool (PrivateData->DownstreamCountEntries);
  }
  PrivateData->DownstreamCountEntries = NewDsDeviceCountArray;

  if (UsbHandleInfoArray != NULL) {
    FreePool (UsbHandleInfoArray);
  }
  return Status;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_MEDIA_CHANGED
  @retval EFI_UNSUPPORTED
  @retval EFI_DEVICE_ERROR
  @retval EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
BuildHostControllerInfoForm (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN H2O_USB_VIEWER_REFRESH_MODE      RefreshMode
  )
{
  EFI_HANDLE                       HostControllerHandle;

  EFI_TPL                          OldTpl;
  EFI_STATUS                       Status;
  EFI_USB3_HC_PROTOCOL             *Usb3Hc;
  CHAR16                           *HostControllerName;
  CHAR16                           *DevicePathText;
  EFI_PCI_IO_PROTOCOL              *PciIo;
  EFI_STATUS                       WarnStatus;
  CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup;
  H2O_USB_VIEWER_HC_COMMON_INFO    HcCommonInfo = {0};

  CHAR16                           *TitleString;
  UINT8                            MaxPortNumber;
  UINTN                            NumOpCodesForDeviceInfo;
  UINTN                            NumStatementsNeeded;
  UINT8                            NumOfUsb2Ports;
  EFI_STRING_ID                    ShowHcInfoStringId;
  UINT16                           ItemIndexBase;
  CONST PCI_TYPE00                 *PciConfig;
  UINT8                            *OpCodePtr;

  HostControllerHandle = PrivateData->SelectedHostControllerHandle;

  if (RefreshMode == H2OUsbViewerFullRefresh) {
    (VOID) HiiSetString (
             PrivateData->HiiHandle,
             PrivateData->UsbDeviceFormSubtitleStringId,
             (CONST EFI_STRING) L"",
             NULL // SupportedLanguages
             );
  }

  if (HostControllerHandle == NULL) {
    return EFI_UNSUPPORTED;
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  Status = GetProtocolWithCorrectStatus (
             HostControllerHandle,
             &gEfiUsb3HcProtocolGuid,
             (VOID **) &Usb3Hc
             );
  HostControllerName = NULL;
  DevicePathText = NULL;
  PciIo = NULL; // Silence warning in old MSVC
  if (!EFI_ERROR (Status)) {
    WarnStatus = GetProtocolWithCorrectStatus (
                   HostControllerHandle,
                   &gEfiDevicePathProtocolGuid,
                   (VOID **) &DevicePath
                   );
    if (EFI_ERROR (WarnStatus)) {
      DevicePath = NULL;
    } else {
      DeviceInfoLookup = PrivateData->DeviceInfoLookup;
      if (DeviceInfoLookup != NULL) {
        WarnStatus = DeviceInfoLookup->GetDeviceInfoByDevicePath (
                                         DeviceInfoLookup,
                                         DevicePath,
                                         NULL, // PcdTokenNumber
                                         NULL, // Attributes
                                         NULL, // AcpiPath
                                         &HostControllerName
                                         );
        if (EFI_ERROR (WarnStatus)) {
          HostControllerName = NULL;
        }
      }

      DevicePathText = ConvertDevicePathToText (
                         DevicePath,
                         TRUE, // DisplayOnly
                         TRUE  // AllowShortcuts
                         );
    }

    WarnStatus = GetProtocolWithCorrectStatus (
                   HostControllerHandle,
                   &gEfiPciIoProtocolGuid,
                   (VOID **) &PciIo
                   );
    if (EFI_ERROR (WarnStatus)) {
      PciIo = NULL;
    }
    Status = InitializeUsbViewerHcCommonInfo (
               &HcCommonInfo,
               Usb3Hc,
               PciIo
               );
  }

  gBS->RestoreTPL (OldTpl);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (RefreshMode == H2OUsbViewerFullRefresh) {
    TitleString = CreateHostControllerGotoPromptString (
                    PrivateData,
                    PrivateData->SelectedPortNumberChain.HostControllerNumber,
                    HostControllerName,
                    &HcCommonInfo,
                    NULL
                    );
    if (TitleString != NULL) {
      (VOID) HiiSetString (
               PrivateData->HiiHandle,
               PrivateData->UsbDeviceFormSubtitleStringId,
               (EFI_STRING) TitleString,
               NULL // SupportedLanguages
               );
      (VOID) HiiSetString (
               PrivateData->HiiHandle,
               PrivateData->HostControllerFormTitleStringId,
               (EFI_STRING) TitleString,
               NULL // SupportedLanguages
               );
      FreePool (TitleString);
    }
  }
  if (HostControllerName != NULL) {
    FreePool (HostControllerName);
  }

  MaxPortNumber = HcCommonInfo.MaxPortNumber;
  NumOpCodesForDeviceInfo = 9;

  NumStatementsNeeded = MaxPortNumber + NumOpCodesForDeviceInfo;
  if (NumStatementsNeeded > H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT) {
    NumStatementsNeeded = H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT;
  }
  (VOID) SetIfrStatementCount (PrivateData, (UINT16) NumStatementsNeeded);
  if (MaxPortNumber > 0) {
    (VOID) SetGotoOpCodeCount (PrivateData, MaxPortNumber);
  }

  NumOfUsb2Ports = HcCommonInfo.NumOfUsb2Ports;
  HcCommonInfo.NumOfUsb2Ports = 0;

  ItemIndexBase = MaxPortNumber;
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 0),
           STRING_TOKEN (STR_USB_NUM_OF_PORTS_PROMPT),
           STRING_TOKEN (STR_USB_HC_NUM_OF_PORTS_HELP),
           L"%u",
           (unsigned int) NumOfUsb2Ports
           );

  if (PrivateData->ViewOption.FlatView == 0) {
    if (PrivateData->ViewOption.ShowHubInformation == 0) {
      ShowHcInfoStringId =
        STRING_TOKEN (STR_USB_SHOW_HOST_CONTROLLER_INFORMATION);
    } else {
      ShowHcInfoStringId =
        STRING_TOKEN (STR_USB_HIDE_HOST_CONTROLLER_INFORMATION);
    }
    (VOID) HiiCreateActionOpCode (
             OpCodeHandle,
             H2O_USB_VIEWER_SHOW_HUB_INFO_ACTION_QUESTION_ID,
             ShowHcInfoStringId,    // Prompt
             ShowHcInfoStringId,    // Help
             EFI_IFR_FLAG_CALLBACK,
             STRING_TOKEN (0x0000)  // QuestionConfig
             );
  }

  if (PrivateData->ViewOption.FlatView == 0 &&
      PrivateData->ViewOption.ShowHubInformation == 0)
  {
    goto HostControllerInfoEnd;
  }
  //
  // Host controller information
  //
  if (PciIo == NULL) {
    goto HostControllerPciInfoEnd;
  }
  (VOID) CreatePciLocationTextOpCode (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 1),
           PciIo
           );

  if (HcCommonInfo.PciConfigBuffer == NULL) {
    goto HostControllerPciInfoEnd;
  }
  PciConfig = (PCI_TYPE00 *) HcCommonInfo.PciConfigBuffer;
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 2),
           STRING_TOKEN (STR_PCI_VENDOR_ID),
           STRING_TOKEN (STR_USB_HC_GENERIC_INFORMATION_HELP),
           L"%04Xh",
           (unsigned int) PciConfig->Hdr.VendorId
           );
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 3),
           STRING_TOKEN (STR_PCI_DEVICE_ID),
           STRING_TOKEN (STR_USB_HC_GENERIC_INFORMATION_HELP),
           L"%04Xh",
           (unsigned int) PciConfig->Hdr.DeviceId
           );
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 4),
           STRING_TOKEN (STR_PCI_REVISION_ID),
           STRING_TOKEN (STR_USB_HC_GENERIC_INFORMATION_HELP),
           L"%02Xh",
           (unsigned int) PciConfig->Hdr.RevisionID
           );
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 5),
           STRING_TOKEN (STR_PCI_SUBSYSTEM_VENDOR_ID),
           STRING_TOKEN (STR_USB_HC_GENERIC_INFORMATION_HELP),
           L"%04Xh",
           (unsigned int) PciConfig->Device.SubsystemVendorID
           );
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 6),
           STRING_TOKEN (STR_PCI_SUBSYSTEM_ID),
           STRING_TOKEN (STR_USB_HC_GENERIC_INFORMATION_HELP),
           L"%04Xh",
           (unsigned int) PciConfig->Device.SubsystemID
           );

  //
  // xHCI specific info
  //
  if (HcCommonInfo.HcPciInterfaceCode == PCI_IF_XHCI) {
  }
  //
  // EHCI specific info
  //
  if (HcCommonInfo.HcPciInterfaceCode == PCI_IF_EHCI) {
  }

HostControllerPciInfoEnd:
  if (HcCommonInfo.HciRevisionBcd > 0x0000) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 7),
             STRING_TOKEN (STR_HCI_REVISION),
             STRING_TOKEN (STR_USB_HC_GENERIC_INFORMATION_HELP),
             L"%x.%d.%d",
             (unsigned int) (HcCommonInfo.HciRevisionBcd >> 8),
             (unsigned int) ((HcCommonInfo.HciRevisionBcd >> 4) & 0xF),
             (unsigned int) (HcCommonInfo.HciRevisionBcd & 0xF)
             );
  }
  (VOID) CreateGrayedOutTextOpCode (
           OpCodeHandle,
           STRING_TOKEN (STR_64_BIT_ADDRESS_CAPABLE),
           ((HcCommonInfo.Is64BitCapable != 0) ?
             STRING_TOKEN (STR_YES) : STRING_TOKEN (STR_NO))
           );

  if (DevicePathText != NULL) {
    OpCodePtr = HiiCreateSubTitleOpCode (
                  OpCodeHandle,
                  STRING_TOKEN (STR_EFI_DEVICE_PATH),
                  STRING_TOKEN (0x0000), // Help
                  0,                     // Flags
                  1                      // Scope
                  );
    if (OpCodePtr != NULL) {
      WarnStatus = SetIfrStatementStrings (
                     PrivateData,
                     (ItemIndexBase + 8),
                     DevicePathText,
                     NULL // HelpString
                     );
      if (!EFI_ERROR (WarnStatus)) {
        (VOID) HiiCreateActionOpCode (
                 OpCodeHandle,
                 (H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE + ItemIndexBase + 8),
                 PrivateData->StatementStringIds[ItemIndexBase + 8].Prompt,
                 STRING_TOKEN (STR_EFI_DEVICE_PATH),
                 0x00,                 // QuestionFlags,
                 STRING_TOKEN (0x0000) // QuestionConfig
                 );
      }
      (VOID) HiiCreateEndOpCode (OpCodeHandle);
    }
    FreePool (DevicePathText);
  }

HostControllerInfoEnd:
  //
  // List of ports
  //
  WarnStatus = CreateUsbPortOpCodeList (
                 PrivateData,
                 OpCodeHandle,
                 RefreshMode,
                 MaxPortNumber
                 );
  if (WarnStatus == EFI_MEDIA_CHANGED) {
    Status = EFI_MEDIA_CHANGED;
  }

  if (RefreshMode != H2OUsbViewerDeviceChangeRefresh) {
    Status = EFI_MEDIA_CHANGED;
  }

  CleanUpUsbViewerHcCommonInfo (&HcCommonInfo);

  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_OUT_OF_RESOURCES
  @retval  EFI_BUFFER_TOO_SMALL

**/
STATIC
EFI_STATUS
CreateUsbClassTextOpCodes (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN UINT16                           ItemIndexBase,
  IN H2O_USB_CLASS_CODE_SEARCH_KEY    SearchKey
  )
{
  EFI_STATUS     Status;
  EFI_STRING     HelpStringFormat;
  EFI_STATUS     WarnStatus;
  EFI_STRING     ClassNameStrings[3] = {NULL};
  UINT8          Codes[3];
  UINT8          ArrayIndex;
  EFI_STRING     PromptString;
  EFI_STRING_ID  PromptStringIds[3] = {
    STRING_TOKEN (STR_DEVICE_CLASS_CODE),
    STRING_TOKEN (STR_DEVICE_SUBCLASS_CODE),
    STRING_TOKEN (STR_DEVICE_PROTOCOL_CODE),
  };
  CHAR16         *HelpString;
  CHAR16         *ValueString;
  UINTN          CharIndex;

  Status = EFI_SUCCESS;
  HelpStringFormat = HiiGetString (
                       PrivateData->HiiHandle,
                       STRING_TOKEN (STR_DEVICE_CLASS_CODE_HELP_FORMAT),
                       NULL // Language
                       );
  if (HelpStringFormat == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (PrivateData->UsbTextLookup != NULL &&
      PrivateData->UsbTextLookup->Flags.ClassTextLookupSupported)
  {
    WarnStatus = PrivateData->UsbTextLookup->GetUsbClassCodeString (
                                               PrivateData->UsbTextLookup,
                                               SearchKey,
                                               NULL, // Language
                                               &ClassNameStrings[0],
                                               &ClassNameStrings[1],
                                               &ClassNameStrings[2]
                                               );
  }
  Codes[0] = SearchKey.Codes.Class;
  Codes[1] = SearchKey.Codes.SubClass;
  Codes[2] = SearchKey.Codes.Protocol;
  for (ArrayIndex = 0; ArrayIndex < 3; ArrayIndex++) {
    PromptString = HiiGetString (
                     PrivateData->HiiHandle,
                     PromptStringIds[ArrayIndex],
                     NULL // Language
                     );
    if (PromptString == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto HelpStringFail;
    }
    HelpString = CatSPrint (
                   NULL,
                   (CHAR16 *) HelpStringFormat,
                   PromptString,
                   (unsigned int) Codes[ArrayIndex],
                   ((ClassNameStrings[ArrayIndex] == NULL) ? L"" :
                     ClassNameStrings[ArrayIndex])
                   );
    FreePool (PromptString);
    if (HelpString == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto HelpStringFail;
    }
    if (ClassNameStrings[ArrayIndex] != NULL) {
      for (CharIndex = 0; TRUE; CharIndex++) {
        if (ClassNameStrings[ArrayIndex][CharIndex] == L'\0') {
          break;
        }
        if (ClassNameStrings[ArrayIndex][CharIndex] == L'\r' ||
            ClassNameStrings[ArrayIndex][CharIndex] == L'\n')
        {
          ClassNameStrings[ArrayIndex][CharIndex] = L'\0';
          break;
        }
      }
    }
    ValueString = CreateNumberWithDescriptionString (
                    L"%02Xh",
                    L" (%s)",
                    ClassNameStrings[ArrayIndex],
                    (unsigned int) Codes[ArrayIndex]
                    );
    if (ValueString == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto ValueStringFail;
    }

    Status = CreateTextOpCodeWithValueString (
               PrivateData,
               OpCodeHandle,
               (ItemIndexBase + ArrayIndex),
               PromptStringIds[ArrayIndex],
               STRING_TOKEN (0x0000), // HelpStringId
               HelpString,
               ValueString
               );
    FreePool (ValueString);
ValueStringFail:
    FreePool (HelpString);
HelpStringFail:
    if (ClassNameStrings[ArrayIndex] != NULL) {
      FreePool (ClassNameStrings[ArrayIndex]);
    }
    if (EFI_ERROR (Status)) {
      break;
    }
  }
  FreePool (HelpStringFormat);
  return Status;
}

/**
**/
STATIC
EFI_STATUS
CreateMediaCapacityTextOpCode (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN UINT16                           ItemIndex,
  IN UINT32                           BlockSize,
  IN UINT64                           LastBlock
  )
{
  EFI_STRING                   Format;
  H2O_CAPACITY_REPRESENTATION  CapacityRepresentation;
  H2O_CAPACITY_UINT128         CapacityTotalBytes;
  CHAR8                        AsciiStringBuffer[UINT128_STR_MAX_SIZE];
  EFI_STATUS                   Status;
  CHAR16                       ValueString[CAPACITY_REP_STR_MAX_SIZE];
  CHAR16                       TotalBytesString[UINT128_STR_MAX_SIZE];
  UINT64                       NumOfBlocks;
  CHAR16                       *HelpString;

  ASSERT (UINT128_STR_MAX_SIZE >= CAPACITY_REP_STR_MAX_SIZE);

  if (BlockSize <= 0) {
    return EFI_INVALID_PARAMETER;
  }

  Format = HiiGetString (
             PrivateData->HiiHandle,
             STRING_TOKEN (STR_MEDIA_CAPACITY_HELP_FORMAT),
             NULL // Language
             );
  if (Format == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ComputeBlockIoCapacity (
    BlockSize,
    LastBlock,
    FORMULA_NORMAL,
    &CapacityRepresentation,
    &CapacityTotalBytes
    );

  BlockIoCapacityRepToString (
    &CapacityRepresentation,
    CAPACITY_REP_TO_STR_BASE_1024,
    AsciiStringBuffer
    );

  Status = AsciiStrToUnicodeStrS (
             AsciiStringBuffer,
             ValueString,
             ARRAY_SIZE (ValueString)
             );
  ASSERT_EFI_ERROR (Status);

  if (CapacityTotalBytes.UpperBits > 0) {
    Uint128ToDecimalString (
      CapacityTotalBytes.LowerBits,
      CapacityTotalBytes.UpperBits,
      AsciiStringBuffer
      );
    Status = AsciiStrToUnicodeStrS (
               AsciiStringBuffer,
               TotalBytesString,
               ARRAY_SIZE (TotalBytesString)
               );
    ASSERT_EFI_ERROR (Status);
  } else {
    UnicodeSPrintAsciiFormat (
      TotalBytesString,
      sizeof (TotalBytesString),
      "%,lu",
      CapacityTotalBytes.LowerBits
      );
  }
  NumOfBlocks = ((LastBlock > MAX_UINT64 - 1) ? MAX_UINT64 : (LastBlock + 1));

  HelpString = CatSPrint (
                 NULL,
                 (CHAR16 *) Format,
                 ValueString,
                 TotalBytesString,
                 NumOfBlocks,
                 (unsigned int) BlockSize
                 );
  FreePool (Format);
  if (HelpString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BlockIoCapacityRepToString (
    &CapacityRepresentation,
    CAPACITY_REP_TO_STR_BASE_1000,
    AsciiStringBuffer
    );

  Status = AsciiStrToUnicodeStrS (
             AsciiStringBuffer,
             ValueString,
             ARRAY_SIZE (ValueString)
             );
  ASSERT_EFI_ERROR (Status);

  Status = CreateTextOpCodeWithValueString (
             PrivateData,
             OpCodeHandle,
             ItemIndex,
             STRING_TOKEN (STR_MEDIA_CAPACITY_PROMPT),
             STRING_TOKEN (0x0000), // HelpStringId
             HelpString,
             ValueString
             );
  FreePool (HelpString);
  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_MEDIA_CHANGED
  @retval  EFI_UNSUPPORTED
  @retval  EFI_TIMEOUT
  @retval  EFI_DEVICE_ERROR
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
BuildUsbDeviceInfoForm (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                         *OpCodeHandle,
  IN H2O_USB_VIEWER_REFRESH_MODE      RefreshMode
  )
{
  UINTN                                  Depth;
  EFI_HANDLE                             UsbDeviceHandle;
  EFI_TPL                                OldTpl;
  EFI_STATUS                             Status;
  EFI_USB_IO_PROTOCOL                    *UsbIo;
  H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS  CommonDescriptors = {0};
  CONST EFI_USB_DEVICE_DESCRIPTOR        *DeviceDescriptor;
  CONST EFI_USB_CONFIG_DESCRIPTOR        *ConfigurationDescriptor;

  UINT8                                  *HubDescriptor;
  UINT32                                 TransferResult;
  UINT8                                  PortCount;
  EFI_STATUS                             WarnStatus;

  CHAR16                                 PortNumberChainString[
                                           USB_PORT_NUMBER_CHAIN_STRING_SIZE /
                                           sizeof (CHAR16)];
  CHAR16                                 *DeviceDisplayName;
  CHAR16                                 *TitleString;
  UINTN                                  SelectedDepth;

  UINT8                                  UsbInterfaceCount;
  EFI_HANDLE                             *UsbInterfaceHandles;

  UINT16                                 NumOpCodesForDeviceInfo;
  UINT16                                 NumOpCodesForInterfaceInfo;
  UINTN                                  NumStatementsNeeded;

  UINT16                                 ItemIndexBase;
  EFI_STRING_ID                          ShowHubInfoStringId;
  CONST CHAR16                           *String;
  UINT8                                  ArrayIndex;
  EFI_STRING_ID                          PromptStringIds[3] = {
    STRING_TOKEN (STR_MANUFACTURER_PROMPT),
    STRING_TOKEN (STR_PRODUCT_PROMPT),
    STRING_TOKEN (STR_SERIAL_NUMBER_PROMPT),
  };
  H2O_USB_CLASS_CODE_SEARCH_KEY          ClassSearchKey;
  UINT8                                  InterfaceNum;
  EFI_USB_INTERFACE_DESCRIPTOR           InterfaceDescriptor = {0};
  CHAR16                                 *InterfaceString;
  EFI_STRING                             Format;
  UINTN                                  FormatPartLength;
  CHAR16                                 *ValueString;
  EFI_BLOCK_IO2_PROTOCOL                 *BlockIo2;
  EFI_BLOCK_IO_PROTOCOL                  *BlockIo;
  UINT32                                 BlockSize;
  EFI_LBA                                LastBlock;

  Depth = (UINTN) PrivateData->ActiveForm -
            H2O_USB_VIEWER_DEPTH_0_DEVICE_FORM_ID;
  UsbDeviceHandle = PrivateData->SelectedUsbDeviceHandleStack[Depth];
  if (Depth < USB_PORT_MAX_DEPTH - 1) {
    PrivateData->SelectedPortNumberChain.UsbPortNumbers[Depth + 1] = 0;
  }

  if (RefreshMode == H2OUsbViewerFullRefresh) {
    (VOID) HiiSetString (
             PrivateData->HiiHandle,
             PrivateData->UsbDeviceFormSubtitleStringId,
             (CONST EFI_STRING) L"",
             NULL // SupportedLanguages
             );
  }

  if (UsbDeviceHandle == NULL) {
    return EFI_UNSUPPORTED;
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  Status = GetProtocolWithCorrectStatus (
             UsbDeviceHandle,
             &gEfiUsbIoProtocolGuid,
             (VOID **) &UsbIo
             );
  if (!EFI_ERROR (Status)) {
    Status = InitializeViewerUsbCommonDescriptors (
               &CommonDescriptors,
               UsbIo,
               PrivateData->ViewOption.SelectedLangId
               );
    if (Status == EFI_NOT_FOUND) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  DeviceDescriptor        = &CommonDescriptors.DeviceDescriptor;
  ConfigurationDescriptor = &CommonDescriptors.ConfigurationDescriptor;

  HubDescriptor  = NULL;
  TransferResult = EFI_USB_NOERROR;
  PortCount      = 0;
  if (!EFI_ERROR (Status)) {
    if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE) {
      WarnStatus = GetUsbHubDescriptor (
                     UsbIo,
                     DeviceDescriptor->BcdUSB,
                     (VOID **) &HubDescriptor,
                     &TransferResult
                     );
      if (!EFI_ERROR (WarnStatus) && (HubDescriptor != NULL)) {
        PortCount = ((UINT8 *) HubDescriptor)[2];
      }
    }
  }
  gBS->RestoreTPL (OldTpl);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Form title string & the heading "subtitle" op-code
  //
  if (RefreshMode != H2OUsbViewerDeviceChangeRefresh) {
    (VOID) PrintUsbPortNumberChainString (
             PortNumberChainString,
             sizeof (PortNumberChainString),
             &PrivateData->SelectedPortNumberChain
             );
    DeviceDisplayName = CreateUsbDeviceDisplayName (&CommonDescriptors);
    if (DeviceDisplayName != NULL) {
      TitleString = CatSPrint (
                      NULL,
                      L"%s: %s",
                      PortNumberChainString,
                      DeviceDisplayName
                      );
      if (TitleString != NULL) {
        if (RefreshMode == H2OUsbViewerFullRefresh) {
          (VOID) HiiSetString (
                   PrivateData->HiiHandle,
                   PrivateData->UsbDeviceFormSubtitleStringId,
                   (EFI_STRING) TitleString,
                   NULL // SupportedLanguages
                   );
        }
        if (PrivateData->ViewOption.FlatView == 0) {
          FreePool (TitleString);
          TitleString = CatSPrint (
                          NULL,
                          L"...%u: %s",
                          ((unsigned int) PrivateData->SelectedPortNumberChain.
                                                         UsbPortNumbers[Depth]),
                          DeviceDisplayName
                          );
        }
      }
      FreePool (DeviceDisplayName);
      (VOID) HiiSetString (
               PrivateData->HiiHandle,
               PrivateData->UsbFormTitleStringIds[Depth],
               (EFI_STRING) ((TitleString == NULL) ? L"" : TitleString),
               NULL // SupportedLanguages
               );
      if (TitleString != NULL) {
        FreePool (TitleString);
      }
    }
    SelectedDepth = Depth;
    if (PrivateData->ViewOption.FlatView == 0) {
      (VOID) UnicodeSPrint (
               PortNumberChainString,
               sizeof (PortNumberChainString),
               L"%u",
               ((unsigned int) PrivateData->SelectedPortNumberChain.
                                              HostControllerNumber)
               );
      (VOID) HiiSetString (
               PrivateData->HiiHandle,
               PrivateData->HostControllerFormTitleStringId,
               (EFI_STRING) PortNumberChainString,
               NULL // SupportedLanguages
               );
      for (Depth = 0; Depth < SelectedDepth; Depth++) {
        (VOID) UnicodeSPrint (
                 PortNumberChainString,
                 sizeof (PortNumberChainString),
                 L"%u",
                 ((unsigned int) PrivateData->SelectedPortNumberChain.
                                                UsbPortNumbers[Depth])
                 );
        (VOID) HiiSetString (
                 PrivateData->HiiHandle,
                 PrivateData->UsbFormTitleStringIds[Depth],
                 (EFI_STRING) PortNumberChainString,
                 NULL // SupportedLanguages
                 );
      }
    }
    Depth = SelectedDepth;
  }

  UsbInterfaceCount = ConfigurationDescriptor->NumInterfaces;
  if (UsbInterfaceCount <= 0) {
    Status = EFI_DEVICE_ERROR;
    goto UsbInterfaceHandleFail;
  } else if (UsbInterfaceCount == 1) {
    UsbInterfaceHandles = AllocatePool (sizeof (EFI_HANDLE));
    if (UsbInterfaceHandles == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto UsbInterfaceHandleFail;
    }
    UsbInterfaceHandles[0] = UsbDeviceHandle;
  } else {
    Status = LocateUsbInterfaceHandlesOfDevice (
               UsbDeviceHandle,
               NULL, // NumUsbInterfaces
               &UsbInterfaceHandles
               );
    if (Status == EFI_NOT_FOUND) {
      Status = EFI_DEVICE_ERROR;
    }
    if (EFI_ERROR (Status) || (UsbInterfaceHandles == NULL)) {
      goto UsbInterfaceHandleFail;
    }
  }

  NumOpCodesForDeviceInfo    = 13;
  NumOpCodesForInterfaceInfo = 7;

  NumStatementsNeeded = PortCount + NumOpCodesForDeviceInfo +
                        (UsbInterfaceCount * NumOpCodesForInterfaceInfo);
  if (NumStatementsNeeded > H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT) {
    NumStatementsNeeded = H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT;
  }
  (VOID) SetIfrStatementCount (PrivateData, (UINT16) NumStatementsNeeded);
  if (PortCount > 0) {
    (VOID) SetGotoOpCodeCount (PrivateData, PortCount);
  }

  ItemIndexBase = PortCount;
  if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 0),
             STRING_TOKEN (STR_USB_NUM_OF_PORTS_PROMPT),
             STRING_TOKEN (STR_USB_DEVICE_DESCRIPTOR_HELP),
             L"%u",
             (unsigned int) PortCount
             );
    if (PrivateData->ViewOption.FlatView == 0) {
      if (PrivateData->ViewOption.ShowHubInformation == 0) {
        ShowHubInfoStringId = STRING_TOKEN (STR_USB_SHOW_HUB_INFORMATION);
      } else {
        ShowHubInfoStringId = STRING_TOKEN (STR_USB_HIDE_HUB_INFORMATION);
      }
      (VOID) HiiCreateActionOpCode (
               OpCodeHandle,
               H2O_USB_VIEWER_SHOW_HUB_INFO_ACTION_QUESTION_ID,
               ShowHubInfoStringId,   // Prompt
               ShowHubInfoStringId,   // Help
               EFI_IFR_FLAG_CALLBACK,
               STRING_TOKEN (0x0000)  // QuestionConfig
               );
    }
  }

  if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE &&
      PrivateData->ViewOption.FlatView == 0 &&
      PrivateData->ViewOption.ShowHubInformation == 0)
  {
    goto UsbDeviceInfoEnd;
  }
  //
  // Device information
  //
  ArrayIndex = 0;
  String = (CONST CHAR16 *) CommonDescriptors.ManufacturerString;
  if (String != NULL) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 1 + ArrayIndex),
             PromptStringIds[ArrayIndex],
             STRING_TOKEN (STR_USB_DEVICE_DESCRIPTOR_HELP),
             L"\"%s\"",
             String
             );
    ArrayIndex++;
  }
  String = (CONST CHAR16 *) CommonDescriptors.ProductString;
  if (String != NULL) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 1 + ArrayIndex),
             PromptStringIds[ArrayIndex],
             STRING_TOKEN (STR_USB_DEVICE_DESCRIPTOR_HELP),
             L"\"%s\"",
             String
             );
    ArrayIndex++;
  }
  String = (CONST CHAR16 *) CommonDescriptors.SerialNumberString;
  if (String != NULL) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 1 + ArrayIndex),
             PromptStringIds[ArrayIndex],
             STRING_TOKEN (STR_USB_DEVICE_DESCRIPTOR_HELP),
             L"\"%s\"",
             String
             );
    ArrayIndex++;
  }

  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 4),
           STRING_TOKEN (STR_DEVICE_VENDOR_AND_PRODUCT_ID),
           STRING_TOKEN (STR_USB_DEVICE_DESCRIPTOR_HELP),
           L"%04X:%04X",
           (unsigned int) DeviceDescriptor->IdVendor,
           (unsigned int) DeviceDescriptor->IdProduct
           );
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 5),
           STRING_TOKEN (STR_DEVICE_RELEASE_NUMBER),
           STRING_TOKEN (STR_USB_DEVICE_DESCRIPTOR_HELP),
           L"%04X",
           (unsigned int) DeviceDescriptor->BcdDevice
           );

  if (DeviceDescriptor->DeviceClass != 0 ||
      DeviceDescriptor->DeviceSubClass != 0 ||
      DeviceDescriptor->DeviceProtocol != 0)
  {
    //
    // Hide device class of zero to save form space. It is common for USB
    // devices to specify class code at Interface level and set device class
    // code triplet to all 00h.
    //
    ClassSearchKey.Codes.DescriptorType = USB_DESC_TYPE_DEVICE;
    ClassSearchKey.Codes.Class          = DeviceDescriptor->DeviceClass;
    ClassSearchKey.Codes.SubClass       = DeviceDescriptor->DeviceSubClass;
    ClassSearchKey.Codes.Protocol       = DeviceDescriptor->DeviceProtocol;
    (VOID) CreateUsbClassTextOpCodes (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 6),
             ClassSearchKey
             );
  }

  if (DeviceDescriptor->NumConfigurations != 1 ||
      ConfigurationDescriptor->ConfigurationValue != 1)
  {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 9),
             STRING_TOKEN (STR_NUM_OF_CONFIGURATIONS_PROMPT),
             STRING_TOKEN (STR_NUM_OF_CONFIGURATIONS_HELP),
             L"%u",
             (unsigned int) DeviceDescriptor->NumConfigurations
             );
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 10),
             STRING_TOKEN (STR_CURRENT_CONFIGURATION_PROMPT),
             STRING_TOKEN (STR_CURRENT_CONFIGURATION_HELP),
             L"%u",
             (unsigned int) ConfigurationDescriptor->ConfigurationValue
             );
  }
  if (CommonDescriptors.ConfigurationString != NULL) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 11),
             STRING_TOKEN (STR_CONFIGURATION_DESCRIPTION_PROMPT),
             STRING_TOKEN (STR_CONFIGURATION_DESCRIPTION_HELP),
             L"\"%s\"",
             CommonDescriptors.ConfigurationString
             );
  }
  (VOID) CreateTextOpCodeWithFormattedValue (
           PrivateData,
           OpCodeHandle,
           (ItemIndexBase + 12),
           STRING_TOKEN (STR_NUM_OF_INTERFACES_PROMPT),
           STRING_TOKEN (STR_NUM_OF_INTERFACES_HELP),
           L"%u",
           (unsigned int) ConfigurationDescriptor->NumInterfaces
           );

  for (InterfaceNum = 0; InterfaceNum < UsbInterfaceCount; InterfaceNum++) {
    ItemIndexBase = PortCount + NumOpCodesForDeviceInfo +
                    (InterfaceNum * NumOpCodesForInterfaceInfo);
    if (UsbInterfaceHandles[InterfaceNum] == NULL) {
      continue;
    }
    OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
    WarnStatus = GetProtocolWithCorrectStatus (
                   UsbInterfaceHandles[InterfaceNum],
                   &gEfiUsbIoProtocolGuid,
                   (VOID **) &UsbIo
                   );
    if (!EFI_ERROR (WarnStatus)) {
      WarnStatus = UsbIo->UsbGetInterfaceDescriptor (
                            UsbIo,
                            &InterfaceDescriptor
                            );
    }
    InterfaceString = NULL; // Silence warning in old MSVC
    if (!EFI_ERROR (WarnStatus)) {
      (VOID) GetUsbStringInPreferredLanguage (
               UsbIo,
               CommonDescriptors.PreferredLangIdArray,
               CommonDescriptors.LangIdCount,
               InterfaceDescriptor.Interface,
               &InterfaceString
               );
    }
    gBS->RestoreTPL (OldTpl);
    if (EFI_ERROR (WarnStatus)) {
      continue;
    }
    Format = HiiGetString (
               PrivateData->HiiHandle,
               STRING_TOKEN (STR_USB_INTERFACE_SUBTITLE_FORMAT),
               NULL // Language
               );
    if (Format != NULL) {
      FormatPartLength = 0;
      while (((CHAR16 *) Format)[FormatPartLength] != L'|') {
        ASSERT (((CHAR16 *) Format)[FormatPartLength] != L'\0');
        FormatPartLength++;
      }
      ((CHAR16 *) Format)[FormatPartLength] = L'\0';

      ValueString = CreateNumberWithDescriptionString (
                      (CHAR16 *) Format,
                      ((CHAR16 *) Format + FormatPartLength + 1),
                      InterfaceString,
                      (unsigned int) InterfaceNum
                      );
      FreePool (Format);
      if (ValueString != NULL) {
        WarnStatus = SetIfrStatementStrings (
                       PrivateData,
                       (ItemIndexBase + 0),
                       ValueString,
                       NULL
                       );
        FreePool (ValueString);
        if (!EFI_ERROR (WarnStatus)) {
          (VOID) HiiCreateSubTitleOpCode (
                   OpCodeHandle,
                   PrivateData->StatementStringIds[ItemIndexBase + 0].Prompt,
                   STRING_TOKEN (0x0000), // Help
                   0,                     // Flags
                   0                      // Scope
                   );
        }
      }
    }
    if (InterfaceString != NULL) {
      FreePool (InterfaceString);
    }
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 1),
             STRING_TOKEN (STR_SELECTED_ALTERNATE_SETTING_PROMPT),
             STRING_TOKEN (STR_SELECTED_ALTERNATE_SETTING_HELP),
             L"%u",
             (unsigned int) InterfaceDescriptor.AlternateSetting
             );
    (VOID) CreateTextOpCodeWithFormattedValue (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 2),
             STRING_TOKEN (STR_NUM_OF_ENDPOINTS_PROMPT),
             STRING_TOKEN (STR_NUM_OF_ENDPOINTS_HELP),
             L"%u",
             (unsigned int) InterfaceDescriptor.NumEndpoints
             );
    ClassSearchKey.Codes.DescriptorType = USB_DESC_TYPE_INTERFACE;
    ClassSearchKey.Codes.Class    = InterfaceDescriptor.InterfaceClass;
    ClassSearchKey.Codes.SubClass = InterfaceDescriptor.InterfaceSubClass;
    ClassSearchKey.Codes.Protocol = InterfaceDescriptor.InterfaceProtocol;
    (VOID) CreateUsbClassTextOpCodes (
             PrivateData,
             OpCodeHandle,
             (ItemIndexBase + 3),
             ClassSearchKey
             );

    BlockSize = 0;
    LastBlock = 0;
    //
    // Raise TPL to prevent uninstallation of the protocol before we've done
    // reading. EFI Block I/O Protocol functions only operate at TPL_CALLBACK
    // or lower, but we only read static data (EFI_BLOCK_IO_MEDIA).
    //
    OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
    WarnStatus = GetProtocolWithCorrectStatus (
                   UsbInterfaceHandles[InterfaceNum],
                   &gEfiBlockIo2ProtocolGuid,
                   (VOID **) &BlockIo2
                   );
    if (!EFI_ERROR (WarnStatus)) {
      BlockSize = BlockIo2->Media->BlockSize;
      LastBlock = BlockIo2->Media->LastBlock;
    } else {
      WarnStatus = GetProtocolWithCorrectStatus (
                     UsbInterfaceHandles[InterfaceNum],
                     &gEfiBlockIoProtocolGuid,
                     (VOID **) &BlockIo
                     );
      if (!EFI_ERROR (WarnStatus)) {
        BlockSize = BlockIo->Media->BlockSize;
        LastBlock = BlockIo->Media->LastBlock;
      }
    }
    gBS->RestoreTPL (OldTpl);
    if (BlockSize > 0) {
      (VOID) CreateMediaCapacityTextOpCode (
               PrivateData,
               OpCodeHandle,
               (ItemIndexBase + 6),
               BlockSize,
               (UINT64) LastBlock
               );
    }
  }

UsbDeviceInfoEnd:
  if (DeviceDescriptor->DeviceClass == USB_HUB_CLASS_CODE) {
    //
    // List of ports (if this device is a hub)
    //
    WarnStatus = CreateUsbPortOpCodeList (
                   PrivateData,
                   OpCodeHandle,
                   RefreshMode,
                   PortCount
                   );
    if (WarnStatus == EFI_MEDIA_CHANGED) {
      Status = EFI_MEDIA_CHANGED;
    }
  }
  if (RefreshMode != H2OUsbViewerDeviceChangeRefresh) {
    Status = EFI_MEDIA_CHANGED;
  }

  if (HubDescriptor != NULL) {
    FreePool (HubDescriptor);
  }
  FreePool (UsbInterfaceHandles);
UsbInterfaceHandleFail:
  CleanUpViewerUsbCommonDescriptors (&CommonDescriptors);

  return Status;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_DEVICE_ERROR
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
RefreshForm (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN H2O_USB_VIEWER_REFRESH_MODE      RefreshMode,
  OUT EFI_FORM_ID                     *DestinationForm
  )
{
  VOID                   *StartOpCodeHandle;
  VOID                   *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL     *StartLabel;
  EFI_IFR_GUID_LABEL     *EndLabel;
  EFI_FORM_ID            ActiveForm;
  EFI_STATUS             Status;
  BOOLEAN                UpstreamHubDisconnected;
  UINT16                 Tier;
  EFI_HANDLE             Handle;
  EFI_STATUS             OpenStatus;

  EFI_STRING             DeviceFormTitleString;
  CHAR16                 PortNumberChainString[
                           USB_PORT_NUMBER_CHAIN_STRING_SIZE /
                           sizeof (CHAR16)];
  CONST CHAR16           *DisconnectedDeviceName;
  EFI_STATUS             WarnStatus;
  USB_PORT_NUMBER_CHAIN  ZeroPortNumberChainEntry = {0};

  if ((PrivateData == NULL) || (DestinationForm == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/DestinationForm is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *DestinationForm = PrivateData->ActiveForm;
  if (PrivateData->ActiveForm < H2O_USB_VIEWER_ENTRY_FORM_ID) {
    return EFI_SUCCESS;
  }

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // HiiCreateGuidOpCode() throws an assertion error when there's not
  // enough memory.
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                        StartOpCodeHandle,
                                        &gEfiIfrTianoGuid,
                                        NULL,
                                        sizeof (EFI_IFR_GUID_LABEL)
                                        );
  if (StartLabel == NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    return EFI_OUT_OF_RESOURCES;
  }
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_USB_VIEWER_LABEL_START;

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                      EndOpCodeHandle,
                                      &gEfiIfrTianoGuid,
                                      NULL,
                                      sizeof (EFI_IFR_GUID_LABEL)
                                      );
  if (EndLabel == NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    HiiFreeOpCodeHandle (StartLabel);
    return EFI_OUT_OF_RESOURCES;
  }
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = H2O_USB_VIEWER_LABEL_END;

  ActiveForm = PrivateData->ActiveForm;

  if (ActiveForm == H2O_USB_VIEWER_ENTRY_FORM_ID) {
    Status = BuildEntryForm (
               PrivateData,
               StartOpCodeHandle,
               RefreshMode
               );
  } else if (ActiveForm == H2O_USB_VIEWER_HOST_CONTROLLER_FORM_ID) {
    Status = BuildHostControllerInfoForm (
               PrivateData,
               StartOpCodeHandle,
               RefreshMode
               );
  } else {
    ASSERT (ActiveForm >= H2O_USB_VIEWER_DEPTH_0_DEVICE_FORM_ID);
    ASSERT (ActiveForm <= H2O_USB_VIEWER_DEPTH_5_DEVICE_FORM_ID);
    Status = BuildUsbDeviceInfoForm (
               PrivateData,
               StartOpCodeHandle,
               RefreshMode
               );
  }
  switch (Status) {

  case EFI_UNSUPPORTED:
  case EFI_TIMEOUT:
  case EFI_DEVICE_ERROR:
    PrivateData->DisconnectedPortNumberChain = PrivateData->
                                                 SelectedPortNumberChain;
    UpstreamHubDisconnected = FALSE;
    ASSERT (ActiveForm >= H2O_USB_VIEWER_HOST_CONTROLLER_FORM_ID);
    Tier = (UINT16) ActiveForm - H2O_USB_VIEWER_ENTRY_FORM_ID;
    for (; Tier >= 2; Tier--) {
      PrivateData->SelectedPortNumberChain.UsbPortNumbers[Tier - 2] = 0;
      if (Tier < 3) {
        break;
      }
      Handle = PrivateData->SelectedUsbDeviceHandleStack[Tier - 3];
      if (Handle == NULL) {
        continue;
      }
      OpenStatus = GetProtocolWithCorrectStatus (
                     Handle,
                     &gEfiUsbIoProtocolGuid,
                     NULL
                     );
      if (!EFI_ERROR (OpenStatus)) {
        break;
      }
      PrivateData->DisconnectedPortNumberChain = PrivateData->
                                                   SelectedPortNumberChain;
      UpstreamHubDisconnected = TRUE;
    }
    if (Tier == 2) {
      Handle = PrivateData->SelectedHostControllerHandle;
      if (Handle == NULL) {
        Tier--;
      } else {
        OpenStatus = GetProtocolWithCorrectStatus (
                       Handle,
                       &gEfiUsb3HcProtocolGuid,
                       NULL
                       );
        if (EFI_ERROR (OpenStatus)) {
          PrivateData->DisconnectedPortNumberChain = PrivateData->
                                                       SelectedPortNumberChain;
          UpstreamHubDisconnected = TRUE;
          Tier--;
        }
      }
    }
    if (Tier <= 1) {
      PrivateData->SelectedPortNumberChain.HostControllerNumber = 0;
      Tier = 1;
    }

    if (PrivateData->ViewOption.FlatView != 0) {
      *DestinationForm = H2O_USB_VIEWER_ENTRY_FORM_ID;
    } else {
      *DestinationForm = (EFI_FORM_ID) (H2O_USB_VIEWER_ENTRY_FORM_ID +
                                         Tier - 1);
      //
      // Ensure that the "Device disconnected" message line can be seen without
      // scrolling.
      //
      PrivateData->ViewOption.ShowHubInformation = 0;
    }
    PrivateData->ActiveForm = 0x0000;
    Status = EFI_DEVICE_ERROR;

    DeviceFormTitleString = NULL;
    if (UpstreamHubDisconnected || RefreshMode == H2OUsbViewerFullRefresh) {
      (VOID) PrintUsbPortNumberChainString (
               PortNumberChainString,
               sizeof (PortNumberChainString),
               &PrivateData->DisconnectedPortNumberChain
               );
      DisconnectedDeviceName = PortNumberChainString;
    } else {
      DeviceFormTitleString = HiiGetString (
                                PrivateData->HiiHandle,
                                PrivateData->UsbDeviceFormSubtitleStringId,
                                NULL // Language
                                );
      DisconnectedDeviceName = (CHAR16 *) DeviceFormTitleString;
    }

    if (DisconnectedDeviceName != NULL) {
      WarnStatus = SetDisconnectedDevicePromptString (
                     PrivateData,
                     DisconnectedDeviceName
                     );
      if (EFI_ERROR (WarnStatus)) {
        PrivateData->DisconnectedPortNumberChain = ZeroPortNumberChainEntry;
      } else {
        (VOID) CreateGrayedOutTextOpCode (
                 StartOpCodeHandle,
                 PrivateData->DisconnectedDeviceStringId,
                 STRING_TOKEN (0x0000)
                 );
      }
    }
    if (DeviceFormTitleString != NULL) {
      FreePool (DeviceFormTitleString);
    }
    (VOID) HiiCreateGotoOpCode (
             StartOpCodeHandle,
             (*DestinationForm),
             STRING_TOKEN (STR_GO_BACK_PROMPT),
             STRING_TOKEN (STR_GO_BACK_HELP),
             EFI_IFR_FLAG_CALLBACK,
             H2O_USB_VIEWER_GO_BACK_QUESTION_ID
             );
    (VOID) HiiUpdateForm (
             PrivateData->HiiHandle,
             &gH2OUsbViewerFormsetGuid,
             ActiveForm,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
    break;

  case EFI_MEDIA_CHANGED:
    Status = HiiUpdateForm (
               PrivateData->HiiHandle,
               &gH2OUsbViewerFormsetGuid,
               ActiveForm,
               StartOpCodeHandle,
               EndOpCodeHandle
               );
    break;
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Refresh form 0x%04X (mode %u): %r\n",
      gEfiCallerBaseName,
      (unsigned int) ActiveForm,
      (unsigned int) RefreshMode,
      Status
      ));
  }

  HiiFreeOpCodeHandle (EndOpCodeHandle);
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  return Status;
}

STATIC
VOID
GotoOpCodeHandler (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData,
  IN EFI_QUESTION_ID                  QuestionId
  )
{
  UINT16                        GotoOpIndex;
  H2O_USB_VIEWER_GOTO_OP_ENTRY  *GotoOpEntry;
  UINT8                         Depth;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  GotoOpIndex = (UINT16) QuestionId - H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE;

  ASSERT (GotoOpIndex < PrivateData->NumGotoOpsAllocated);
  GotoOpEntry = &PrivateData->GotoOpEntries[GotoOpIndex];

  PrivateData->SelectedPortNumberChain = GotoOpEntry->PortNumberChain;
  if (GotoOpEntry->PortNumberChain.UsbPortNumbers[0] == 0) {
    PrivateData->SelectedHostControllerHandle = GotoOpEntry->Handle;
  } else {
    for (Depth = 1; Depth < USB_PORT_MAX_DEPTH; Depth++) {
      if (GotoOpEntry->PortNumberChain.UsbPortNumbers[Depth] == 0) {
        break;
      }
    }
    --Depth;
    PrivateData->SelectedUsbDeviceHandleStack[Depth] = GotoOpEntry->Handle;
  }
}

/**

  @implements EFI_EVENT_NOTIFY

  @param[in]  Event
  @param[in]  Context

  @return  Void.

**/
VOID
EFIAPI
FormRefreshEventHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_FORM_ID  DestinationForm;

  (VOID) RefreshForm (
           (H2O_USB_VIEWER_PRIVATE_DATA *) Context,
           H2OUsbViewerDeviceChangeRefresh,
           &DestinationForm
           );
}

STATIC
EFI_STATUS
InitializeFormRefreshEvent (
  IN OUT H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData
  )
{
  EFI_STATUS  Status;
  UINT32      FormRefreshPeriod;
  VOID        *Registration;

  if (PrivateData->FormRefreshEvent != NULL) {
    return EFI_ALREADY_STARTED;
  }
  Status = gBS->CreateEvent (
                  (EVT_TIMER | EVT_NOTIFY_SIGNAL),
                  H2O_USB_VIEWER_FORM_REFRESH_TPL,
                  FormRefreshEventHandler, // NotifyFunction
                  (VOID *) PrivateData,    // NotifyContext
                  &PrivateData->FormRefreshEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Can't create form refresh event: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    return Status;
  }

  FormRefreshPeriod = PcdGet32 (PcdH2OUsbViewerFormRefreshPeriod);
  if (FormRefreshPeriod > 0) {
    Status = gBS->SetTimer (
                    PrivateData->FormRefreshEvent,
                    TimerPeriodic,
                    EFI_TIMER_PERIOD_MILLISECONDS (FormRefreshPeriod)
                    );
    ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiUsbIoProtocolGuid,
                  PrivateData->FormRefreshEvent,
                  &Registration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_WARN,
      "%a: Can't listen to gEfiUsbIoProtocolGuid: %r\n",
      gEfiCallerBaseName,
      Status
      ));
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
H2OUsbViewerSyncViewOption (
  IN CONST H2O_USB_VIEWER_VIEW_OPTION  *ViewOptionBuffer
  )
{
  BOOLEAN  IsSuccess;

  IsSuccess = HiiSetBrowserData (
                &gH2OUsbViewerFormsetGuid,
                L"H2OUsbViewerViewOption",
                sizeof (ViewOptionBuffer),
                (CONST UINT8 *) ViewOptionBuffer,
                NULL // RequestElement
                );
  return ((IsSuccess) ? EFI_SUCCESS : EFI_UNSUPPORTED);
}

EFI_STATUS
EFIAPI
H2OUsbViewerExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Request,
  OUT EFI_STRING                           *Progress,
  OUT EFI_STRING                           *Results
  )
{
  H2O_USB_VIEWER_PRIVATE_DATA              *Private;
  EFI_STATUS                               Status;
  UINTN                                    BufferSize;
  H2O_USB_VIEWER_VIEW_OPTION               ViewOption;
  EFI_STRING                               ConfigRequestHdr;
  EFI_STRING                               ConfigRequest;
  UINTN                                    ConfigRequestSize;
  BOOLEAN                                  AllocatedRequest;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Request;

  if (Request != NULL &&
      !HiiIsConfigHdrMatch (Request, &gH2OUsbViewerFormsetGuid, L"H2OUsbViewerViewOption")) {
    return EFI_NOT_FOUND;
  }

  Private          = H2O_USB_VIEWER_PRIVATE_FROM_CONFIG_ACCESS (This);
  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  AllocatedRequest = FALSE;

  BufferSize = sizeof (H2O_USB_VIEWER_VIEW_OPTION);
  Status = gRT->GetVariable (
                  L"H2OUsbViewerViewOption",
                  &gH2OUsbViewerFormsetGuid,
                  NULL,
                  &BufferSize,
                  &ViewOption
                  );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Construct the request string.
  //
  ConfigRequest = Request;
  if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
    ConfigRequestHdr = HiiConstructConfigHdr (
                         &gH2OUsbViewerFormsetGuid,
                         L"H2OUsbViewerViewOption",
                         Private->ServiceHandle
                         );
    if (ConfigRequestHdr == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    ConfigRequestSize = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16); 
    ConfigRequest     = AllocateZeroPool (ConfigRequestSize);
    AllocatedRequest  = TRUE;
    if (ConfigRequest == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    UnicodeSPrint (
      ConfigRequest,
      ConfigRequestSize,
      L"%s&OFFSET=0&WIDTH=%016LX",
      ConfigRequestHdr,
      (UINT64) BufferSize
      );
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = gHiiConfigRouting->BlockToConfig (
                                gHiiConfigRouting,
                                ConfigRequest,
                                (UINT8 *) &ViewOption,
                                BufferSize,
                                Results,
                                Progress
                                );

  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

Exit:
  if (ConfigRequestHdr != NULL) {
    FreePool (ConfigRequestHdr);
  }

  if (AllocatedRequest && ConfigRequest != NULL) {
    FreePool (ConfigRequest);
  }

  return Status;
}

EFI_STATUS
EFIAPI
H2OUsbViewerRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  )
{
  EFI_STATUS                               Status;
  UINTN                                    BufferSize;
  H2O_USB_VIEWER_VIEW_OPTION               ViewOption;

  if (This == NULL || Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  if (!HiiIsConfigHdrMatch (Configuration, &gH2OUsbViewerFormsetGuid, L"H2OUsbViewerViewOption")) {
    return EFI_NOT_FOUND;
  }

  BufferSize = sizeof (H2O_USB_VIEWER_VIEW_OPTION);
  Status = gRT->GetVariable (
                  L"H2OUsbViewerViewOption",
                  &gH2OUsbViewerFormsetGuid,
                  NULL,
                  &BufferSize,
                  &ViewOption
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = gHiiConfigRouting->ConfigToBlock (
                                gHiiConfigRouting,
                                Configuration,
                                (UINT8 *) &ViewOption,
                                &BufferSize,
                                Progress
                                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gRT->SetVariable (
         L"H2OUsbViewerViewOption",
         &gH2OUsbViewerFormsetGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
         sizeof (H2O_USB_VIEWER_VIEW_OPTION),
         &ViewOption 
         );

  return EFI_SUCCESS;
}

/**
  This function processes the results of changes in configuration.

  @implements EFI_HII_ACCESS_FORM_CALLBACK

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval  EFI_SUCCESS           The callback successfully handled the action.
  @retval  EFI_OUT_OF_RESOURCES  Not enough storage is available to hold the variable and its data.
  @retval  EFI_DEVICE_ERROR      The variable could not be saved.
  @retval  EFI_UNSUPPORTED       The specified Action is not supported by the callback.

**/
EFI_STATUS
EFIAPI
H2OUsbViewerFormCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN OUT EFI_IFR_TYPE_VALUE                *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData;
  EFI_STATUS                   Status;
  EFI_TPL                      OldTpl;
  BOOLEAN                      IsFormRefreshQuestion;
  EFI_FORM_ID                  DestinationForm;
  USB_PORT_NUMBER_CHAIN        ZeroPortNumberChainEntry = {0};
  EFI_STATUS                   WarnStatus;
  H2O_USB_VIEWER_VIEW_OPTION   DefaultViewOption;
  BOOLEAN                      IsGotoQuestion;
  USB_PORT_NUMBER_CHAIN        SavedPortNumberChain;
  EFI_STATUS                   RefreshStatus;

  if (ActionRequest != NULL) {
    *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
  }

  PrivateData = H2O_USB_VIEWER_PRIVATE_FROM_CONFIG_ACCESS (This);

  Status = EFI_UNSUPPORTED;
  //
  // Any modification to PrivateData or RefreshForm() call should raise TPL to
  // prevent contention with the FormRefreshEvent.
  //
  OldTpl = gBS->RaiseTPL (H2O_USB_VIEWER_FORM_REFRESH_TPL);

  IsFormRefreshQuestion =
    QuestionId >= H2O_USB_VIEWER_ENTRY_REFRESH_QUESTION_ID &&
    QuestionId <= H2O_USB_VIEWER_DEPTH_5_REFRESH_QUESTION_ID;

  if (IsFormRefreshQuestion) {
    switch (Action) {

    case EFI_BROWSER_ACTION_CHANGING:
      if (PrivateData->ActiveForm == (EFI_FORM_ID) QuestionId) {
        goto ViewChangeRefresh;
      }
      break;

    case EFI_BROWSER_ACTION_FORM_OPEN:
      //
      // We expect only one form being active at a time, but UEFI specification
      // permits a FORM_OPEN callback of second form to be called before the
      // FORM_CLOSE callback of the first form.
      //
      ASSERT (PrivateData->FormOpenCount < MAX_UINT8);
      PrivateData->FormOpenCount++;
      (VOID) InitializeFormRefreshEvent (PrivateData);

      if (PrivateData->ActiveForm == (EFI_FORM_ID) QuestionId) {
        //
        // Form is arrived via a "goto" op-code and is redrawn already.
        //
        PrivateData->DisconnectedPortNumberChain = ZeroPortNumberChainEntry;
      } else {
        PrivateData->ActiveForm = (EFI_FORM_ID) QuestionId;
        (VOID) RefreshForm (
                 PrivateData,
                 H2OUsbViewerFullRefresh,
                 &DestinationForm
                 );
      }
      break;

    case EFI_BROWSER_ACTION_FORM_CLOSE:
      if (PrivateData->FormOpenCount == 1 &&
          PrivateData->FormRefreshEvent != NULL)
      {
        gBS->CloseEvent (PrivateData->FormRefreshEvent);
        PrivateData->FormRefreshEvent = NULL;
      }
      if (PrivateData->FormOpenCount > 0) {
        PrivateData->FormOpenCount--;
      }
      break;
    }
  }

  //
  // Workaround H2O Kernel behavior not compliant with UEFI:
  // With EFI_BROWSER_ACTION_DEFAULT_*, the "Type" and "Value" arguments should
  // contain "proposed default value ... based on the lower-priority defaulting
  // methods".
  //
  if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD &&
      Action < EFI_BROWSER_ACTION_DEFAULT_FIRMWARE + 0x1000)
  {
    WarnStatus = H2OUsbViewerGetDefaultViewOption (&DefaultViewOption);
    if (!EFI_ERROR (WarnStatus)) {
      if (QuestionId == H2O_USB_VIEWER_FLAT_VIEW_QUESTION_ID) {
        Value->b = DefaultViewOption.FlatView != 0;
        PrivateData->ViewOption.FlatView = (UINT8) (Value->b != 0);
        goto ViewChangeRefresh;
      }
    }
  }

  if (Action == EFI_BROWSER_ACTION_CHANGING) {
    if (QuestionId == H2O_USB_VIEWER_FLAT_VIEW_QUESTION_ID) {
      PrivateData->ViewOption.FlatView = (UINT8) (Value->b != 0);
      goto ViewChangeRefresh;
    }

    if (QuestionId == H2O_USB_VIEWER_SHOW_HUB_INFO_ACTION_QUESTION_ID) {
      ASSERT (Type == EFI_IFR_TYPE_ACTION);
      PrivateData->ViewOption.ShowHubInformation =
        ((PrivateData->ViewOption.ShowHubInformation != 0) ? 0 : 1);
      WarnStatus = H2OUsbViewerSyncViewOption (&PrivateData->ViewOption);
      goto ViewChangeRefresh;
    }

    IsGotoQuestion =
      (Type == EFI_IFR_TYPE_REF &&
      QuestionId >= H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE &&
      QuestionId < (H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE +
      H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT));
    if (IsGotoQuestion) {
      GotoOpCodeHandler (PrivateData, QuestionId);
    }
    if (IsGotoQuestion || QuestionId == H2O_USB_VIEWER_GO_BACK_QUESTION_ID) {
      ASSERT (Type == EFI_IFR_TYPE_REF);
      SavedPortNumberChain = PrivateData->DisconnectedPortNumberChain;
      while (TRUE) {
        PrivateData->ActiveForm = Value->ref.FormId;
        RefreshStatus = RefreshForm (
                          PrivateData,
                          H2OUsbViewerFullRefresh,
                          &DestinationForm
                          );
        if (RefreshStatus != EFI_DEVICE_ERROR) {
          if (EFI_ERROR (RefreshStatus)) {
            //
            // Out of memory or other error. Cancel the "goto" action.
            //
            Status = EFI_DEVICE_ERROR;
          }
          break;
        }
        //
        // Device error when drawing the form showing the device information.
        // Override the "goto" destination to an "upstream" device's form.
        //
        SavedPortNumberChain  = PrivateData->DisconnectedPortNumberChain;
        Value->ref.FormId     = DestinationForm;
        Value->ref.QuestionId = 0x0000;
        Status                = EFI_SUCCESS;
      }
      //
      // DisconnectedPortNumberChain would be zeroed by the FORM_OPEN callback.
      //
      PrivateData->DisconnectedPortNumberChain = SavedPortNumberChain;
    }
  }
  goto End;

ViewChangeRefresh:
  (VOID) RefreshForm (
           PrivateData,
           H2OUsbViewerViewChangeRefresh,
           &DestinationForm
           );

End:
  gBS->RestoreTPL (OldTpl);

  return Status;
}
