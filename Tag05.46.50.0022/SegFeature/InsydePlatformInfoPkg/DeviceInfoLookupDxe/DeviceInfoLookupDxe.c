/** @file
  H2O Device Info Lookup Protocol implementation.

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

#include <Protocol/DevicePath.h>
#include <Protocol/H2ODeviceInfoLookup.h> // InsydePlatformInfoPkg

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiExLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/SortLib.h>                  // MdeModulePkg
#include <Library/BinarySearchExLib.h>        // InsydePlatformInfoPkg
#include <Library/DeviceInfoLookupDataLib.h>  // InsydePlatformInfoPkg

#include "DeviceInfoLookupDxe.h"

#ifdef _MSC_VER
  #if _MSC_VER >= 1400 // Visual C++ 2005 (Visual C++ 8.0)
#pragma warning (push)
//
// <intrin.h> might generate a "Inconsistent annotation" warning.
// Warnings from system headers might break the parsing of Insyde's
// SourceCodeAnalyzer.exe (Issue IB19060028). We suppress the warnings as a
// workaround.
//
#pragma warning (disable : 28251)
#include <intrin.h> // For _BitScanReverse
#pragma warning (pop)
#pragma intrinsic (_BitScanReverse)
#pragma intrinsic (_BitScanReverse64)
    #define HAVE_BITSCANREVERSE 1
  #endif
#endif

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
STATIC H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE  mDeviceInfoUsbLookup = {
  0, // MaxHostControllerNumber
  0, // Reserved
  GetUsbHcDevicePathByNumber,
  GetUsbPortInfoByNumberChain,
  DevicePathToUsbPortNumberChain,
  UsbPortNumberChainCompare
};
#endif

STATIC H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  mDeviceInfoLookupPrivateData = {
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_SIGNATURE,
  {     // Protocol
    {0},
    {0},
    GetDeviceInfoByDevicePath,
    GetAncestorDeviceInfoOfType,
    DeviceInfoDisplayNameCompare,
    CreateNameTable,
    DestroyNameTable,
    CreateDisplayNameString,
    AddDeviceInfoLookupEntry,
    LockDeviceInfoLookupData,
    ResetDeviceInfoLookupData,
    LoadDeviceInfoLookupData,

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
    &mDeviceInfoUsbLookup
#else
    NULL
#endif
  },
  NULL,  // InfoEntries
  NULL,  // DisplayNameSortKeys
  NULL,  // DisplayNameStrings
  0,     // InfoEntryCount
  0,     // DisplayNameStringCount
  0,     // IsBeingWritten
  0      // IsDeviceInfoPcdLoaded
#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  , NULL // UsbPortInfoTrieNodes
  , NULL // UsbHcDevicePaths
  , NULL // UsbHcAcpiPaths
  , 0    // UsbPortInfoTrieNodeCount
  , 0    // UsbHostControllerCount
#endif
  , NULL                              // HiiHandle
  , DeviceInfoLookupDxeStringsToIds   // StringTokenNameToIds
};

VOID
DebugPrintDevicePath (
  IN UINTN                           ErrorLevel,
  IN CONST CHAR8                     *FunctionName,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  CHAR16  *DevicePathText;

  if (!DebugPrintEnabled () || !DebugPrintLevelEnabled (ErrorLevel)) {
    return;
  }
  DevicePathText = ConvertDevicePathToText (
                     DevicePath,
                     TRUE, // DisplayOnly
                     TRUE  // AllowShortcuts
                     );
  DEBUG ((
    ErrorLevel,
    "%a: %a(): %s\n",
    gEfiCallerBaseName,
    FunctionName,
    ((DevicePathText != NULL) ?
      DevicePathText : L"ConvertDevicePathToText failed")
    ));
  FreePoolIfNonNull (DevicePathText);
}

/**

  @param[in]  DevicePath1  The pointer to the first Device Path.
  @param[in]  DevicePath2  The pointer to the second Device Path.
  @param[in]  MaxSize      Maximum number of bytes of the Device Path to
                           compare. If there is no limit on the Device Path
                           length, specify MAX_UINTN. If MaxSize equals 0,
                           this function performs no comparison.

  @retval 0   DevicePath1, when compared up to MaxSize bytes, is identical to
              DevicePath2.
  @retval <0  DevicePath1 precedes DevicePath2.
  @retval >0  DevicePath1 succeeds DevicePath2.

**/
STATIC
INTN
CompareDevicePath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath2,
  IN UINTN                           MaxSize,
  OUT UINTN                          *IdenticalPrefixSize OPTIONAL
  )
{
  CONST UINT8                     *Buffer1;
  CONST UINT8                     *Buffer2;
  UINTN                           TotalSize;
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node1;
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node2;
  INTN                            CompareResult;
  UINTN                           NodeLength;
  UINTN                           CompareLength;

  //
  // This function compares Device Paths mostly by raw bytes. It's good enough
  // for use in table lookup. We are not suggesting any proper way of collating
  // Device Paths.
  //
  // Assume the Device Path from either argument may be invalid.
  //

  if ((DevicePath1 == NULL) || (DevicePath2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DevicePath1/DevicePath2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  Buffer1 = (CONST UINT8 *) DevicePath1;
  Buffer2 = (CONST UINT8 *) DevicePath2;
  TotalSize = 0;

  if (IdenticalPrefixSize != NULL) {
    *IdenticalPrefixSize = TotalSize;
  }

  while (TRUE) {
    if (IdenticalPrefixSize != NULL) {
      *IdenticalPrefixSize = TotalSize;
    }

    if (TotalSize >= MaxSize) {
      break;
    }

    Node1 = (CONST EFI_DEVICE_PATH_PROTOCOL *) (Buffer1 + TotalSize);
    Node2 = (CONST EFI_DEVICE_PATH_PROTOCOL *) (Buffer2 + TotalSize);

    //
    // Check for Device Path End node. We order a Device Path End node as less
    // than a non-end node.
    //
    CompareResult = (IsDevicePathEndType (Node2) ? 1 : 0) -
                      (IsDevicePathEndType (Node1) ? 1 : 0);
    if (CompareResult != 0) {
      return CompareResult;
    }
    if (IsDevicePathEndType (Node1)) {
      ASSERT (IsDevicePathEndType (Node2));

      if (TotalSize + 1 >= MaxSize) {
        //
        // MaxSize doesn't cover the Device Path node sub-type field.
        //
        TotalSize++;
        break;
      }

      CompareResult = (IsDevicePathEnd (Node2) ? 1 : 0) -
                        (IsDevicePathEnd (Node1) ? 1 : 0);
      if (CompareResult != 0) {
        return CompareResult;
      }

      if (IsDevicePathEnd (Node1)) {
        ASSERT (IsDevicePathEnd (Node2));
        if (DevicePathNodeLength (Node1) != END_DEVICE_PATH_LENGTH) {
          DEBUG ((
            EFI_D_WARN,
            "%a: %p: End Device Path node length invalid.\n",
            gEfiCallerBaseName,
            DevicePath1
            ));
        }
        if (DevicePathNodeLength (Node2) != END_DEVICE_PATH_LENGTH) {
          DEBUG ((
            EFI_D_WARN,
            "%a: %p: End Device Path node length invalid.\n",
            gEfiCallerBaseName,
            DevicePath2
            ));
        }
        if (DevicePathNodeLength (Node1) == END_DEVICE_PATH_LENGTH &&
            DevicePathNodeLength (Node2) == END_DEVICE_PATH_LENGTH &&
            IdenticalPrefixSize != NULL &&
            TotalSize + END_DEVICE_PATH_LENGTH <= MaxSize)
        {
          *IdenticalPrefixSize = TotalSize + END_DEVICE_PATH_LENGTH;
        }
        return 0;
      }
    }

    //
    // Read the length of the Device Path node, but only if MaxSize covers
    // the length field.
    //
    NodeLength = sizeof (EFI_DEVICE_PATH_PROTOCOL);
    CompareLength = sizeof (EFI_DEVICE_PATH_PROTOCOL);

    if (MaxSize - TotalSize >= sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      NodeLength = DevicePathNodeLength (Node1);
      CompareLength = NodeLength;
      if (NodeLength < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
        DEBUG ((
          EFI_D_WARN,
          "%a: %a(): invalid Device Path in address %p\n",
          gEfiCallerBaseName,
          __FUNCTION__,
          DevicePath1
          ));
        CompareLength = sizeof (EFI_DEVICE_PATH_PROTOCOL);
      }
    }

    if (CompareLength > MaxSize - TotalSize) {
      CompareLength = MaxSize - TotalSize;
    }

    //
    // Perform a binary comparison on the Device Path nodes.
    //
    CompareResult = CompareMem (Node1, Node2, CompareLength);
    if (CompareResult != 0) {
      return CompareResult;
    }
    if (MaxSize - TotalSize < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      //
      // MaxSize doesn't cover the whole Device Path node.
      //
      TotalSize += CompareLength;
      break;
    }

    //
    // Jump to next node.
    //
    ASSERT (NodeLength == DevicePathNodeLength (Node2));
    if (NodeLength < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      DEBUG ((
        EFI_D_WARN,
        "%a: Device Paths in addresses %p and %p are both invalid but identical.\n",
        gEfiCallerBaseName,
        DevicePath1,
        DevicePath2
        ));
      return 0;
    }
    TotalSize += CompareLength;
  }

  //
  // All bytes compared up to MaxSize are identical.
  //
  ASSERT (TotalSize == MaxSize);
  return 0;
}

/**
**/
STATIC
INTN
EFIAPI
CompareDevicePathInInfoEntry (
  IN CONST VOID  *DevicePath,
  IN CONST VOID  *Entry,
  IN VOID        *Context
  )
{
  CONST EFI_DEVICE_PATH_PROTOCOL  *EntryDevicePath;
  UINTN                           MaxSize;
  INTN                            CompareResult;
  UINTN                           TotalSize;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  UINTN                           NodeLength;

  if ((Entry == NULL) || (Context == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Entry/Context is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  EntryDevicePath = ((CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY *)
                      Entry)->DevicePath;
  MaxSize = *(UINTN *) Context;

  CompareResult = CompareDevicePath (
                    (CONST EFI_DEVICE_PATH_PROTOCOL *) DevicePath,
                    EntryDevicePath,
                    MaxSize,
                    NULL // IdenticalPrefixSize
                    );
  if (CompareResult != 0) {
    return CompareResult;
  }

  //
  // If MaxSize != MAX_UINTN, the caller intends to match a Device Path of an
  // ancestor. Make sure the Info Entries of descendant devices don't match.
  //
  TotalSize = 0;
  while (TRUE) {
    if (TotalSize > MaxSize) {
      return -1;
    }
    DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *)
                       ((CONST UINT8 *) EntryDevicePath + TotalSize);
    if (IsDevicePathEnd (DevicePathNode)) {
      break;
    }
    NodeLength = DevicePathNodeLength (DevicePathNode);
    ASSERT (NodeLength >= sizeof (EFI_DEVICE_PATH_PROTOCOL));
    TotalSize += NodeLength;
  }

  return 0;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND

**/
STATIC
EFI_STATUS
LookupInfoEntry (
  IN CONST H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL             *DevicePath,
  OUT UINTN                                     *Index
  )
{
  UINTN          MaxSize;
  RETURN_STATUS  Status;

  if ((PrivateData == NULL) || (Index == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/Index is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (!PrivateData->IsBeingWritten);

  MaxSize = MAX_UINTN;
  Status = BinarySearchEx (
             DevicePath,
             PrivateData->InfoEntries,
             PrivateData->InfoEntryCount,
             sizeof (*PrivateData->InfoEntries),
             CompareDevicePathInInfoEntry,
             &MaxSize, // Context
             Index
             );
  return (EFI_STATUS) Status;
}

/**

  @retval NULL
  @retval Other

**/
CHAR8 *
AsciiStrDuplicate (
  IN CONST CHAR8  *String
  )
{
  if (String == NULL) {
    return NULL;
  }
  return AllocateCopyPool (AsciiStrSize (String), String);
}

VOID
FreePoolIfNonNull (
  IN VOID  *Buffer
  )
{
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
}

/**

  @implements H2O_LOOKUP_DEVICE_INFO_BY_DEVICE_PATH

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
GetDeviceInfoByDevicePath (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT UINTN                           *Attributes OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA      *PrivateData;
  EFI_STATUS                               Status;
  UINTN                                    Index;
  CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *Entry;

  if ((This == NULL) || (DevicePath == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/DevicePath is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  DebugPrintDevicePath (EFI_D_VERBOSE, __FUNCTION__, DevicePath);

  if (PcdTokenNumber != NULL) {
    *PcdTokenNumber = 0;
  }
  if (Attributes != NULL) {
    *Attributes = 0;
  }
  if (AcpiPath != NULL) {
    *AcpiPath = NULL;
  }
  if (DisplayName != NULL) {
    *DisplayName = NULL;
  }

  Status = LookupInfoEntry (PrivateData, DevicePath, &Index);
  if (!EFI_ERROR (Status)) {
    ASSERT (Index < PrivateData->InfoEntryCount);
    Entry = &PrivateData->InfoEntries[Index];

    if (PcdTokenNumber != NULL) {
      *PcdTokenNumber = Entry->PcdTokenNumber;
    }
    if (Attributes != NULL) {
      *Attributes = Entry->Attributes;
    }
    if (AcpiPath != NULL && Entry->AcpiPath != NULL) {
      *AcpiPath = AsciiStrDuplicate (Entry->AcpiPath);
      if (*AcpiPath == NULL) {
        goto AcpiPathFail;
      }
    }
    if (DisplayName != NULL &&
        Entry->DisplayNameIndex < PrivateData->DisplayNameStringCount)
    {
      *DisplayName = CatSPrint (
                       NULL,
                       L"%s",
                       PrivateData->DisplayNameStrings[Entry->DisplayNameIndex]
                       );
      if (*DisplayName == NULL) {
        goto DisplayNameFail;
      }
    }
    return EFI_SUCCESS;
  }

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  Status = GetUsbPortInfoByDevicePath (
             PrivateData,
             DevicePath,
             PcdTokenNumber,
             Attributes,
             AcpiPath,
             DisplayName
             );
  if (Status != EFI_NOT_FOUND) {
    return Status;
  }
#endif

  return EFI_NOT_FOUND;

DisplayNameFail:
  if (AcpiPath != NULL) {
    FreePoolIfNonNull (*AcpiPath);
    *AcpiPath = NULL;
  }
AcpiPathFail:
  return EFI_OUT_OF_RESOURCES;
}

STATIC
BOOLEAN
InfoEntryMatchesAttributeFilter (
  CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *InfoEntry,
  IN UINTN                                 FilterAttributes
  )
{
  if (FilterAttributes == 0) {
    return TRUE;
  }
  if (InfoEntry == NULL) {
    return FALSE;
  }
  return ((InfoEntry->Attributes & FilterAttributes) != 0);
}

/**
**/
STATIC
UINTN
GetOffsetToLastDevicePathNode (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN UINTN                           MaxSize
  )
{
  UINTN                           Offset;
  UINTN                           TotalSize;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  UINTN                           NodeLength;

  if (MaxSize < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
    return 0;
  }

  if (DevicePath == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DevicePath is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  Offset = 0;
  TotalSize = 0;
  while (TRUE) {
    if (TotalSize >= MaxSize - sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      break;
    }

    DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *)
                       ((CONST UINT8 *) DevicePath + TotalSize);
    if (IsDevicePathEnd (DevicePathNode)) {
      break;
    }

    Offset = TotalSize;

    NodeLength = DevicePathNodeLength (DevicePathNode);
    if (NodeLength < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      DEBUG ((
        EFI_D_WARN,
        "%a: %a(): invalid Device Path in address %p\n",
        gEfiCallerBaseName,
        __FUNCTION__,
        DevicePath
        ));
      break;
    }

    if (NodeLength >= MaxSize || TotalSize >= MaxSize - NodeLength) {
      break;
    }

    TotalSize += NodeLength;
  }
  return Offset;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND

**/
STATIC
EFI_STATUS
LookupAncestorInfoEntryOfType (
  IN CONST H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL             *DevicePath,
  IN UINTN                                      FilterAttributes,
  OUT UINTN                                     *Index
  )
{
  UINTN                                    StartIndex;
  UINTN                                    InfoEntryCount;
  UINTN                                    MaxSize;
  RETURN_STATUS                            Status;
  CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *InfoEntry;

  if ((PrivateData == NULL) || (Index == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/Index is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (!PrivateData->IsBeingWritten);

  StartIndex = 0;
  InfoEntryCount = PrivateData->InfoEntryCount;
  MaxSize = MAX_UINTN;

  while (InfoEntryCount > 0 && MaxSize > 0) {
    Status = BinarySearchEx (
               DevicePath,
               &PrivateData->InfoEntries[StartIndex],
               InfoEntryCount,
               sizeof (*PrivateData->InfoEntries),
               CompareDevicePathInInfoEntry,
               &MaxSize, // Context
               Index
               );
    *Index += StartIndex;
    if (!RETURN_ERROR (Status)) {
      InfoEntry = &PrivateData->InfoEntries[*Index];
      if (InfoEntryMatchesAttributeFilter (InfoEntry, FilterAttributes)) {
        return EFI_SUCCESS;
      }
    }

    InfoEntryCount = (*Index) - StartIndex;
    MaxSize = GetOffsetToLastDevicePathNode (DevicePath, MaxSize);
  }

  return EFI_NOT_FOUND;
}

/**
**/
EFI_STATUS
EFIAPI
GetAncestorDeviceInfoOfType (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  IN OUT UINTN                        *Attributes OPTIONAL,
  OUT EFI_DEVICE_PATH_PROTOCOL        **AncestorDevicePath OPTIONAL,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA      *PrivateData;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES        AttributeFlags;
  EFI_STATUS                               Status;
  UINTN                                    Index;
  CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *Entry;

  if ((This == NULL) || (DevicePath == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/DevicePath is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  DebugPrintDevicePath (EFI_D_VERBOSE, __FUNCTION__, DevicePath);

  if (AncestorDevicePath != NULL) {
    *AncestorDevicePath = NULL;
  }
  if (PcdTokenNumber != NULL) {
    *PcdTokenNumber = 0;
  }
  if (AcpiPath != NULL) {
    *AcpiPath = NULL;
  }
  if (DisplayName != NULL) {
    *DisplayName = NULL;
  }

  AttributeFlags.Uint8 = 0;
  if (Attributes != NULL) {
    if (*Attributes > MAX_UINT8) {
      return EFI_UNSUPPORTED;
    }
    AttributeFlags.Uint8 = (UINT8) *Attributes;
  }
  if (AttributeFlags.Bits.IsUsb) {
    return EFI_UNSUPPORTED;
  }

  Status = LookupAncestorInfoEntryOfType (
             PrivateData,
             DevicePath,
             AttributeFlags.Uint8,
             &Index
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ASSERT (Index < PrivateData->InfoEntryCount);
  Entry = &PrivateData->InfoEntries[Index];

  if (PcdTokenNumber != NULL) {
    *PcdTokenNumber = Entry->PcdTokenNumber;
  }
  if (Attributes != NULL) {
    *Attributes = Entry->Attributes;
  }
  if (AncestorDevicePath != NULL) {
    *AncestorDevicePath = DuplicateDevicePath (Entry->DevicePath);
    if (*AncestorDevicePath == NULL) {
      goto DevicePathBufferFail;
    }
  }
  if (AcpiPath != NULL && Entry->AcpiPath != NULL) {
    *AcpiPath = AsciiStrDuplicate (Entry->AcpiPath);
    if (*AcpiPath == NULL) {
      goto AcpiPathFail;
    }
  }
  if (DisplayName != NULL &&
      Entry->DisplayNameIndex < PrivateData->DisplayNameStringCount)
  {
    *DisplayName = CatSPrint (
                     NULL,
                     L"%s",
                     PrivateData->DisplayNameStrings[Entry->DisplayNameIndex]
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
AcpiPathFail:
  if (AncestorDevicePath != NULL) {
    FreePool (*AncestorDevicePath);
    *AncestorDevicePath = NULL;
  }
DevicePathBufferFail:
  return EFI_OUT_OF_RESOURCES;
}

STATIC
UINT16
LookupDisplayNameIndex (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath
  )
{
  UINT16      DisplayNameIndex;
  EFI_STATUS  Status;
  UINTN       EntryIndex;

  DisplayNameIndex = MAX_UINT16;

  Status = LookupInfoEntry (PrivateData, DevicePath, &EntryIndex);
  if (!EFI_ERROR (Status)) {
    ASSERT (EntryIndex < PrivateData->InfoEntryCount);
    DisplayNameIndex = PrivateData->InfoEntries[EntryIndex].DisplayNameIndex;
    ASSERT (DisplayNameIndex < PrivateData->DisplayNameStringCount || DisplayNameIndex == MAX_UINT16);
    return DisplayNameIndex;
  }

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  DisplayNameIndex = LookupUsbPortInfoDisplayNameIndex (
                       PrivateData,
                       DevicePath
                       );
  if (DisplayNameIndex < MAX_UINT16) {
    return DisplayNameIndex;
  }
#endif

  return DisplayNameIndex;
}

INTN
EFIAPI
DeviceInfoDisplayNameCompare (
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA      *PrivateData;
  UINT16                                   NameStringIndex1;
  UINT16                                   NameStringIndex2;

  if ((This == NULL) || (DevicePath1 == NULL) || (DevicePath2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/DevicePath1/DevicePath2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  NameStringIndex1 = LookupDisplayNameIndex (PrivateData, DevicePath1);
  NameStringIndex2 = LookupDisplayNameIndex (PrivateData, DevicePath2);

  return (INTN) NameStringIndex1 - (INTN) NameStringIndex2;
}

VOID
EFIAPI
DestroyNameTable (
  IN H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *NameTable
  )
{
  UINTN  Index;

  if (NameTable == NULL) {
    return;
  }

  for (Index = 0; NameTable[Index].DevicePath != NULL; Index++) {
    FreePoolIfNonNull (NameTable[Index].DisplayName);
    FreePool (NameTable[Index].DevicePath);
    FreePoolIfNonNull (NameTable[Index].AcpiPath);
  }
  FreePool (NameTable);
}

STATIC
INTN
CompareNameTableEntries (
  IN CONST VOID  *Buffer1,
  IN CONST VOID  *Buffer2
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA            *PrivateData;
  CONST H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *Entry1;
  CONST H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *Entry2;
  INTN                                           IndexDifference;

  PrivateData = &mDeviceInfoLookupPrivateData;
  Entry1 = (CONST H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY *) Buffer1;
  Entry2 = (CONST H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY *) Buffer2;

  //
  // We temporarily borrow the PcdTokenNumber field to store the PrivateData
  // "info entry index". The "lookup name table" is supposed to be sorted by
  // display name sort key.
  //
  IndexDifference =
    (INTN) PrivateData->InfoEntries[Entry1->PcdTokenNumber].DisplayNameIndex -
    (INTN) PrivateData->InfoEntries[Entry2->PcdTokenNumber].DisplayNameIndex;

  if (IndexDifference != 0) {
    return IndexDifference;
  }
  return (Entry1->PcdTokenNumber - Entry2->PcdTokenNumber);
}

/**
  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_UNSUPPORTED
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
CreateNameTable (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL           *This,
  IN UINTN                                     FilterAttributes,
  OUT H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  **NameTable OPTIONAL,
  OUT UINTN                                    *NumOfDevices OPTIONAL
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA      *PrivateData;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES        AttributeFlags;
  UINTN                                    DeviceCount;
  UINT32                                   Index;
  CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *InfoEntry;
  UINTN                                    NameTableIndex;
  EFI_DEVICE_PATH_PROTOCOL                 *DevicePathBuffer;
  CHAR8                                    *AcpiPath;
  CHAR16                                   *DisplayName;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (NameTable != NULL) {
    *NameTable = NULL;
  }
  if (NumOfDevices != NULL) {
    *NumOfDevices = 0;
  }

  if (FilterAttributes > MAX_UINT8) {
    return EFI_UNSUPPORTED;
  }
  AttributeFlags.Uint8 = (UINT8) FilterAttributes;
  if (AttributeFlags.Bits.IsUsb) {
    //
    // USB ports are too complex; this function is not designed to retrieve the
    // table of USB ports.
    //
    return EFI_UNSUPPORTED;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);
  ASSERT (!PrivateData->IsBeingWritten);

  //
  // Count the number of device name entries.
  //
  DeviceCount = 0;
  for (Index = 0; Index < PrivateData->InfoEntryCount; Index++) {
    InfoEntry = &PrivateData->InfoEntries[Index];
    if (!InfoEntryMatchesAttributeFilter (InfoEntry, FilterAttributes)) {
      continue;
    }
    DeviceCount++;
    ASSERT (DeviceCount <= MAX_UINTN / sizeof (**NameTable) - 1);
  }

  if (NumOfDevices != NULL) {
    *NumOfDevices = DeviceCount;
  }

  if (NameTable == NULL) {
    return EFI_SUCCESS;
  }
  if (DeviceCount <= 0) {
    return EFI_NOT_FOUND;
  }
  //
  // The name table will leave a null entry in the end, which helps simplifying
  // the DestroyNameTable() function that frees the table.
  //
  *NameTable = AllocateZeroPool ((DeviceCount + 1) * sizeof (**NameTable));
  if (*NameTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Fill the name table entries with indices. These indices will be used for
  // sorting (by display name sort key).
  //
  NameTableIndex = 0;
  for (Index = 0; Index < PrivateData->InfoEntryCount; Index++) {
    InfoEntry = &PrivateData->InfoEntries[Index];
    if (!InfoEntryMatchesAttributeFilter (InfoEntry, FilterAttributes)) {
      continue;
    }

    ASSERT (Index < MAX_UINT32);
    (*NameTable)[NameTableIndex].PcdTokenNumber = Index;
    NameTableIndex++;
    if (NameTableIndex >= DeviceCount) {
      break;
    }
  }

  ASSERT (PrivateData == &mDeviceInfoLookupPrivateData);
  PerformQuickSort (
    *NameTable,
    DeviceCount,
    sizeof (**NameTable),
    CompareNameTableEntries
    );

  //
  // Replace the indices in name table entries with actual data.
  //
  for (NameTableIndex = 0; NameTableIndex < DeviceCount; NameTableIndex++) {
    Index = (*NameTable)[NameTableIndex].PcdTokenNumber;
    InfoEntry = &PrivateData->InfoEntries[Index];

    ASSERT (InfoEntry->DevicePath != NULL);
    DevicePathBuffer = DuplicateDevicePath (InfoEntry->DevicePath);
    if (DevicePathBuffer == NULL) {
      goto DevicePathBufferFail;
    }

    AcpiPath = NULL;
    if (InfoEntry->AcpiPath != NULL) {
      AcpiPath = AsciiStrDuplicate (InfoEntry->AcpiPath);
      if (AcpiPath == NULL) {
        goto AcpiPathFail;
      }
    }

    DisplayName = NULL;
    if (InfoEntry->DisplayNameIndex < PrivateData->DisplayNameStringCount) {
      DisplayName = CatSPrint (
                      NULL,
                      L"%s",
                      PrivateData->DisplayNameStrings[InfoEntry->DisplayNameIndex]
                      );
      if (DisplayName == NULL) {
        goto DisplayNameFail;
      }
    }

    (*NameTable)[NameTableIndex].DisplayName    = DisplayName;
    (*NameTable)[NameTableIndex].DevicePath     = DevicePathBuffer;
    (*NameTable)[NameTableIndex].AcpiPath       = AcpiPath;
    (*NameTable)[NameTableIndex].PcdTokenNumber = InfoEntry->PcdTokenNumber;
    (*NameTable)[NameTableIndex].Attributes     = InfoEntry->Attributes;
    DEBUG ((
      EFI_D_VERBOSE,
      "%a: %a(): %lu (original index %lu) - %c%s%c 0x%08x.\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (UINT64) NameTableIndex,
      (UINT64) Index,
      ((DisplayName != NULL) ? L'"' : L'('),
      ((DisplayName != NULL) ? DisplayName : L"null"),
      ((DisplayName != NULL) ? L'"' : L')'),
      (unsigned int) InfoEntry->PcdTokenNumber
      ));
    DebugPrintDevicePath (EFI_D_VERBOSE, __FUNCTION__, DevicePathBuffer);
  }
  return EFI_SUCCESS;

DisplayNameFail:
  FreePoolIfNonNull (AcpiPath);
AcpiPathFail:
  FreePool (DevicePathBuffer);
DevicePathBufferFail:
  DestroyNameTable (*NameTable);
  *NameTable = NULL;
  return EFI_OUT_OF_RESOURCES;
}

VOID
AcquireDeviceInfoLookupDataWriteLock (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  )
{
  ASSERT (!PrivateData->IsBeingWritten);
  PrivateData->IsBeingWritten = 1;
}

STATIC
UINTN
NextPowerOf2 (
  IN UINTN  Number
  )
{
#ifdef __has_builtin
  #if __has_builtin (__builtin_clz)
    #define HAVE_BUILTIN_CLZ 1
  #endif
#endif
#ifdef __GNUC__
  #if (__GNUC__ > 3)
    #define HAVE_BUILTIN_CLZ 1
  #endif
  #ifdef __GNUC_MINOR__
    #if (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
      #define HAVE_BUILTIN_CLZ 1
    #endif
  #endif
#endif

  if (Number <= 1) {
    return 1;
  }

  ASSERT ((Number - 1) <= (MAX_UINTN >> 1));
  Number--;

#ifdef HAVE_BITSCANREVERSE
  {
    unsigned long Index;
    if (sizeof (UINTN) <= sizeof (unsigned long)) {
      (VOID) _BitScanReverse (&Index, (unsigned long) Number);
    } else {
      (VOID) _BitScanReverse64 (&Index, Number);
    }
    return ((UINTN) 1U << (Index + 1));
  }
#elif defined (HAVE_BUILTIN_CLZ)
  {
    int  LeadingZeroCount;
    if (sizeof (UINTN) <= sizeof (unsigned int)) {
      LeadingZeroCount = __builtin_clz ((unsigned int) Number);
      return (1U << (sizeof (unsigned int) * 8 - LeadingZeroCount));
    } else if (sizeof (UINTN) <= sizeof (unsigned long)) {
      LeadingZeroCount = __builtin_clzl ((unsigned long) Number);
      return (1UL << (sizeof (unsigned long) * 8 - LeadingZeroCount));
    } else {
      LeadingZeroCount = __builtin_clzll (Number);
      return (1ULL << (sizeof (unsigned long long) * 8 - LeadingZeroCount));
    }
  }
#else
  //
  // http://www.graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
  //
  {
    UINT8  Shift;
    for (Shift = 1; Shift < sizeof (Number) * 8; Shift <<= 1) {
      Number |= (Number >> Shift);
    }
    return (Number + 1);
  }
#endif
}

/**

  @param[in,out] Buffer         A pointer to a pointer to the original buffer.
                                If the buffer would be reallocated, the pointer
                                would be updated to point to a new buffer.
                                The Buffer parameter may point to a NULL
                                pointer if NumOfElements is zero.
  @param[in]     NumOfElements  Number of elements in the original buffer.
  @param[in]     ElementSize    Size of each element in bytes in the original
                                buffer.

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
GrowBufferIfFull (
  IN OUT VOID  **Buffer,
  IN UINTN     NumOfElements,
  IN UINTN     Addend,
  IN UINTN     ElementSize
  )
{
  BOOLEAN  CanReuseBuffer;
  UINTN    BufferSize;
  UINTN    NewSize;
  VOID     *NewBuffer;

  if (Buffer == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Buffer is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (ElementSize <= 0) {
    return EFI_INVALID_PARAMETER;
  }
  if (NumOfElements > MAX_UINTN - Addend) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (NumOfElements + Addend > MAX_UINTN / ElementSize) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (NumOfElements > 0) {
    //
    // Whether a power of two is present in the closed interval
    // [NumOfElements, (NumOfElements - 1 + Addend)].
    //
    CanReuseBuffer = ((NumOfElements - 1) ^ (NumOfElements - 1 + Addend)) <=
                       NumOfElements - 1;
    if (CanReuseBuffer) {
      return EFI_SUCCESS;
    }
  }

  BufferSize = NumOfElements * ElementSize;
  NewSize = NumOfElements + Addend;
  if (NewSize * ElementSize <= MAX_UINTN / 2 + 1) {
    NewSize = NextPowerOf2 (NewSize) * ElementSize;
  } else {
    NewSize = (MAX_UINTN / ElementSize) * ElementSize;
  }

  NewBuffer = ReallocatePool (BufferSize, NewSize, *Buffer);
  if (NewBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *Buffer = NewBuffer;
  return EFI_SUCCESS;
}

VOID
ReleaseDeviceInfoLookupDataWriteLock (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  )
{
  ASSERT (PrivateData->IsBeingWritten);
  PrivateData->IsBeingWritten = 0;

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  if (PrivateData->Protocol.Usb != NULL) {
    PrivateData->Protocol.Usb->MaxHostControllerNumber =
                                 PrivateData->UsbHostControllerCount;
  }
#endif
}

/**
**/
EFI_STATUS
EFIAPI
AddDeviceInfoLookupEntry (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN UINT32                               PcdTokenNumber,
  IN UINTN                                Attributes,
  IN CONST CHAR8                          *AcpiPath OPTIONAL,
  IN CONST CHAR16                         *DisplayName OPTIONAL,
  IN CONST CHAR16                         *DisplayNameSortKey OPTIONAL
  )
{
  UINTN                                DevicePathSize;
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  EFI_STATUS                           Status;
  UINTN                                EntryIndex;
  EFI_DEVICE_PATH_PROTOCOL             *DevicePathBuffer;
  CHAR8                                *AcpiPathBuffer;
  UINTN                                DisplayNameIndex;
  H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY    *Entry;

  if (This == NULL || DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (IsDevicePathEnd (DevicePath)) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Assume GetDevicePathSize() checks the validity of the Device Path (calls
  // IsDevicePathValid()) internally. This includes ensuring the length of the
  // End Device Path node is correct.
  //
  DevicePathSize = GetDevicePathSize (DevicePath);
  if (DevicePathSize < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: %a(): invalid Device Path in address %p\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      DevicePath
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (Attributes > MAX_UINT8) {
    return EFI_UNSUPPORTED;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  DebugPrintDevicePath (EFI_D_VERBOSE, __FUNCTION__, DevicePath);

  //
  // If an entry with the device path exists, reject the new entry.
  //
  Status = LookupInfoEntry (PrivateData, DevicePath, &EntryIndex);
  if (!EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_VERBOSE,
      "%a: %a(): info entry already exists (index %u).\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (unsigned int) EntryIndex
      ));
    return EFI_ALREADY_STARTED;
  }

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  //
  // Determine whether the entry should be added into
  // PrivateData->UsbPortInfoTrieNodes instead of PrivateData->InfoEntries.
  //
  Status = AddUsbPortLookupEntry (
             PrivateData,
             DevicePath,
             PcdTokenNumber,
             Attributes,
             AcpiPath,
             DisplayName,
             DisplayNameSortKey
             );
  if (Status != EFI_UNSUPPORTED) {
    return Status;
  }
#endif

  AcquireDeviceInfoLookupDataWriteLock (PrivateData);

  //
  // Grow the InfoEntries list buffer.
  //
  if (PrivateData->InfoEntryCount >= MAX_UINT16) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: %a(): can't store more than %lu info entries.\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (UINT64) MAX_UINT16
      ));
    goto GrowBufferFail;
  }
  Status = GrowBufferIfFull (
             (VOID **) &PrivateData->InfoEntries,
             PrivateData->InfoEntryCount,
             1,
             sizeof (*PrivateData->InfoEntries)
             );
  if (EFI_ERROR (Status)) {
    ASSERT (Status == EFI_OUT_OF_RESOURCES);
    goto GrowBufferFail;
  }

  //
  // Duplicate the Device Path and ACPI path.
  //
  DevicePathBuffer = AllocateCopyPool (DevicePathSize, DevicePath);
  if (DevicePathBuffer == NULL) {
    goto DevicePathBufferFail;
  }

  AcpiPathBuffer = NULL;
  if (AcpiPath != NULL) {
    AcpiPathBuffer = AsciiStrDuplicate (AcpiPath);
    if (AcpiPathBuffer == NULL) {
      goto AcpiPathFail;
    }
  }

  //
  // Insert display name.
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

  Status = EFI_SUCCESS;

  //
  // Insert new entry.
  //
  ASSERT (EntryIndex <= PrivateData->InfoEntryCount);
  Entry = &PrivateData->InfoEntries[EntryIndex];
  CopyMem (
    &PrivateData->InfoEntries[EntryIndex + 1],
    &PrivateData->InfoEntries[EntryIndex],
    ((PrivateData->InfoEntryCount - EntryIndex) * sizeof (*Entry))
    );
  Entry->DevicePath       = DevicePathBuffer;
  Entry->AcpiPath         = AcpiPathBuffer;
  Entry->PcdTokenNumber   = PcdTokenNumber;
  Entry->DisplayNameIndex = (UINT16) DisplayNameIndex;
  Entry->Attributes       = (UINT8) Attributes;
  PrivateData->InfoEntryCount++;
  DEBUG ((
    EFI_D_VERBOSE,
    "%a: %a(): inserted to index %lu (PcdTokenNumber=0x%08x, Attributes=0x%02x).\n",
    gEfiCallerBaseName,
    __FUNCTION__,
    (UINT64) EntryIndex,
    (unsigned int) PcdTokenNumber,
    (unsigned int) Attributes
    ));

  //
  // Done.
  //
  goto End;

DisplayNameFail:
  FreePoolIfNonNull (AcpiPathBuffer);
AcpiPathFail:
  FreePool (DevicePathBuffer);
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
EFIAPI
LockDeviceInfoLookupData (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  if (PrivateData->DisplayNameStringCount <= 0 ||
      PrivateData->DisplayNameSortKeys == NULL)
  {
    return EFI_UNSUPPORTED;
  }

  AcquireDeviceInfoLookupDataWriteLock (PrivateData);
  FreePool (PrivateData->DisplayNameSortKeys);
  PrivateData->DisplayNameSortKeys = NULL;
  ReleaseDeviceInfoLookupDataWriteLock (PrivateData);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ResetDeviceInfoLookupData (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  EFI_STATUS                           Status;
  UINTN                                Index;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  Status = EFI_SUCCESS;

  AcquireDeviceInfoLookupDataWriteLock (PrivateData);

  if (PrivateData->InfoEntries != NULL) {
    for (Index = 0; Index < PrivateData->InfoEntryCount; Index++) {
      ASSERT (PrivateData->InfoEntries[Index].DevicePath != NULL);
      FreePool (PrivateData->InfoEntries[Index].DevicePath);
      FreePoolIfNonNull (PrivateData->InfoEntries[Index].AcpiPath);
    }
    FreePool (PrivateData->InfoEntries);
  }
  if (PrivateData->DisplayNameSortKeys != NULL) {
    for (Index = 0; Index < PrivateData->DisplayNameStringCount; Index++) {
      FreePool (PrivateData->DisplayNameSortKeys[Index]);
    }
    FreePool (PrivateData->DisplayNameSortKeys);
  }
  if (PrivateData->DisplayNameStrings != NULL) {
    for (Index = 0; Index < PrivateData->DisplayNameStringCount; Index++) {
      FreePool (PrivateData->DisplayNameStrings[Index]);
    }
    FreePool (PrivateData->DisplayNameStrings);
  }
  PrivateData->InfoEntries            = NULL;
  PrivateData->DisplayNameSortKeys    = NULL;
  PrivateData->DisplayNameStrings     = NULL;
  PrivateData->InfoEntryCount         = 0;
  PrivateData->DisplayNameStringCount = 0;
  PrivateData->IsDeviceInfoPcdLoaded  = 0;

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  Status = ResetDeviceInfoUsbPortLookupData (PrivateData);
#endif

  ReleaseDeviceInfoLookupDataWriteLock (PrivateData);

  return Status;
}

CHAR16 *
CreateDisplayNameTextForDebug (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT16                               DisplayNameIndex
  )
{
  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return CatSPrint (NULL, L"(null)");
  }

  if (DisplayNameIndex < PrivateData->DisplayNameStringCount) {
    return CatSPrint (
             NULL,
             L"(%u) %s",
             (unsigned int) DisplayNameIndex,
             PrivateData->DisplayNameStrings[DisplayNameIndex]
             );
  }

  return CatSPrint (NULL, L"(null)");
}

STATIC
VOID
DebugPrintDeviceInfoLookupData (
  IN UINTN                                ErrorLevel,
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  )
{
  UINTN                                    Index;
  CONST H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *Entry;
  CHAR16                                   *DevicePathText;
  CHAR16                                   *DisplayNameText;

  if ((!DebugPrintEnabled ()) || (!DebugPrintLevelEnabled (ErrorLevel))) {
    return;
  }

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  ASSERT (!PrivateData->IsBeingWritten);

  DEBUG ((
    ErrorLevel,
    "H2O Device Info Lookup Data\n"
    "Info Entry count: %u\n"
    "Index: PcdTokenNum, Attr, DevicePath, DisplayName, AcpiPath\n",
    (unsigned int) PrivateData->InfoEntryCount
    ));
  for (Index = 0; Index < PrivateData->InfoEntryCount; Index++) {
    Entry = &PrivateData->InfoEntries[Index];

    DevicePathText = ConvertDevicePathToText (
                       Entry->DevicePath,
                       TRUE, // DisplayOnly
                       TRUE  // AllowShortcuts
                       );

    DisplayNameText = CreateDisplayNameTextForDebug (
                        PrivateData,
                        Entry->DisplayNameIndex
                        );

    DEBUG ((
      ErrorLevel,
      "%5u:  0x%08x, 0x%02x, %s, %s, %a\n",
      (unsigned int) Index,
      (unsigned int) Entry->PcdTokenNumber,
      (unsigned int) Entry->Attributes,
      ((DevicePathText != NULL) ? DevicePathText : L"(out of resource)"),
      ((DisplayNameText != NULL) ? DisplayNameText : L"(out of resource)"),
      ((Entry->AcpiPath != NULL) ? Entry->AcpiPath : "(null)")
      ));

    FreePoolIfNonNull (DevicePathText);
    FreePoolIfNonNull (DisplayNameText);
  }

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  DebugPrintUsbPortInfoTrie (ErrorLevel, PrivateData);
#endif
}

/**

  @param[in,out] This

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED

**/
EFI_STATUS
EFIAPI
LoadDeviceInfoLookupData (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  )
{
  H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData;
  BOOLEAN                              IsDeviceInfoPcdLoaded;
  EFI_STATUS                           Status;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS (This);

  IsDeviceInfoPcdLoaded = PrivateData->IsDeviceInfoPcdLoaded;
  PrivateData->IsDeviceInfoPcdLoaded = 1;

  if (IsDeviceInfoPcdLoaded) {
    return EFI_ALREADY_STARTED;
  }

  Status = LoadDeviceInfoLookupDataInternal (
             This,
             PrivateData->HiiHandle,
             PrivateData->StringTokenNameToIds
             );

  DebugPrintDeviceInfoLookupData (EFI_D_VERBOSE, PrivateData);

  return Status;
}

EFI_STATUS
EFIAPI
DeviceInfoLookupDxeUnload (
  IN EFI_HANDLE        ImageHandle
  )
{
  EFI_STATUS           Status;

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gH2ODeviceInfoLookupProtocolGuid,
                  &mDeviceInfoLookupPrivateData.Protocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiRemovePackages (mDeviceInfoLookupPrivateData.HiiHandle);

  return EFI_SUCCESS;
}

/**
  The entry point for the driver.

  @implements EFI_IMAGE_ENTRY_POINT

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
DeviceInfoLookupDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  mDeviceInfoLookupPrivateData.HiiHandle = HiiAddPackages (
                                             &gEfiCallerIdGuid,
                                             ImageHandle,
                                             DeviceInfoLookupDxeStrings,
                                             NULL
                                             );
  if (mDeviceInfoLookupPrivateData.HiiHandle == NULL) {
    return EFI_ABORTED;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gH2ODeviceInfoLookupProtocolGuid,
                  &mDeviceInfoLookupPrivateData.Protocol,
                  (EFI_GUID *) NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Install protocol interface failed: %r\n",
      gEfiCallerBaseName,
      Status
      ));
  }

  return Status;
}
