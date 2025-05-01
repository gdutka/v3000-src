/** @file
  NVMe Information

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
#include <Uefi/UefiInternalFormRepresentation.h>

#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/SetupUtilityApplication.h>
#include <Protocol/H2ODeviceInfoLookup.h>     // InsydePlatformInfoPkg
#include <Protocol/H2ONvmeInfo.h>             // InsydePlatformInfoPkg

#include <Guid/MdeModuleHii.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>                   // MdeModulePkg
#include <Library/SortLib.h>                  // MdeModulePkg
#include <Library/HiiExLib.h>                 // InsydeModulePkg
#include <Library/BinarySearchExLib.h>        // InsydePlatformInfoPkg
#include <Library/DeviceInfoLookupDataLib.h>  // InsydePlatformInfoPkg
#include <Library/HiiStringIdManageLib.h>     // InsydePlatformInfoPkg
#include <Library/LanguageDataLib.h>          // InsydePlatformInfoPkg
#include <Library/NvmeInfoLib.h>              // InsydePlatformInfoPkg

#include "NvmeInfoFormset.h"
#include "NvmeInfoSetupUtilityDxe.h"
#include "NvmeDetailDisplay.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

H2O_SETUP_NVME_INFO_PRIVATE_DATA mNvmeInfoData = {
  H2O_SETUP_NVME_INFO_PRIVATE_SIGNATURE,
  {         // ConfigAccess
    NvmeInfoExtractConfig,
    NvmeInfoRouteConfig,
    NvmeInfoFormCallback
  },
  NULL,     // NvmeInfo
  NULL,     // DeviceInfoLookup
  NULL,     // SetupUtilityEvent
  NULL,     // SeviceHandle
  NULL,     // HiiHandle
  NULL,     // HiiStringIdManager
  NULL,     // NvmeControllerEntries
  NULL,     // DeviceInfoLookupNameTable
  0,        // NvmeControllerCount
  0         // DeviceInfoLookupNameCount
};

HII_VENDOR_DEVICE_PATH mHiiNvmeInfoVendorDevPath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    H2O_NVME_INFO_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

/**
  Prints an "Out of resources" debug message.

**/
STATIC
VOID
DebugPrintOutOfResources (
  IN CONST CHAR8  *Tag
  )
{
  DEBUG ((
    EFI_D_ERROR,
    "%a: Out of resources (%a)\n",
    gEfiCallerBaseName,
    Tag
    ));
  return;
}

STATIC
EFI_STATUS
CreateDeviceInfoLookupNameTable (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData
  )
{
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES  AttributeFlags;
  UINTN                              AttributesValue;
  EFI_STATUS                         Status;
  UINTN                              NameCount;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->DeviceInfoLookupNameTable != NULL) {
    ASSERT (PrivateData->DeviceInfoLookupNameCount > 0);
    return EFI_ALREADY_STARTED;
  }

  InitDeviceInfoLookupProtocol (&PrivateData->DeviceInfoLookup);

  DeviceInfoLookup = PrivateData->DeviceInfoLookup;
  if (DeviceInfoLookup == NULL) {
    return EFI_UNSUPPORTED;
  }

  AttributeFlags.Uint8 = 0;
  AttributeFlags.Bits.IsNvme = 1;
  AttributesValue = AttributeFlags.Uint8;

  Status = DeviceInfoLookup->CreateNameTable (
                               DeviceInfoLookup,
                               AttributesValue,
                               &PrivateData->DeviceInfoLookupNameTable,
                               &NameCount
                               );
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: CreateDeviceInfoLookupNameTable() status: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    return Status;
  }

  PrivateData->DeviceInfoLookupNameCount = NameCount;

  return Status;
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

STATIC
INTN
EFIAPI
CompareNameEntryIndicesByDevicePath (
  IN CONST VOID  *Buffer1,
  IN CONST VOID  *Buffer2
  )
{
  UINTN                                          NameEntryIndex1;
  UINTN                                          NameEntryIndex2;
  CONST H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *NameTable;

  NameEntryIndex1 = *(CONST UINTN *) Buffer1;
  NameEntryIndex2 = *(CONST UINTN *) Buffer2;

  NameTable = mNvmeInfoData.DeviceInfoLookupNameTable;
  if (NameTable == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NameTable is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  return CompareDevicePath (
           NameTable[NameEntryIndex1].DevicePath,
           NameTable[NameEntryIndex2].DevicePath,
           MAX_UINTN, // MaxSize
           NULL       // IdenticalPrefixSize
           );
}

STATIC
INTN
EFIAPI
CompareDevicePathWithNameEntryIndex (
  IN CONST VOID  *Key,
  IN CONST VOID  *Element,
  IN VOID        *Context OPTIONAL
  )
{
  INTN                                           CompareResult;
  CONST EFI_DEVICE_PATH_PROTOCOL                 *KeyDevicePath;
  CONST H2O_SETUP_NVME_INFO_NAME_SEARCH_CONTEXT  *SearchContext;
  UINTN                                          NameEntryIndex;
  CONST EFI_DEVICE_PATH_PROTOCOL                 *NameEntryDevicePath;
  UINTN                                          IdenticalPrefixSize;
  UINTN                                          DevicePathSize;

  if (Context == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Context is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return -1;
  }

  KeyDevicePath  = (CONST EFI_DEVICE_PATH_PROTOCOL *) Key;
  SearchContext  = (CONST H2O_SETUP_NVME_INFO_NAME_SEARCH_CONTEXT *) Context;
  NameEntryIndex = *(CONST UINTN *) Element;
  NameEntryDevicePath = SearchContext->DeviceInfoLookupNameTable[
                                         NameEntryIndex].DevicePath;

  CompareResult = CompareDevicePath (
                    KeyDevicePath,
                    NameEntryDevicePath,
                    SearchContext->DevicePathMaxSize,
                    &IdenticalPrefixSize
                    );
  if (CompareResult != 0) {
    return CompareResult;
  }
  //
  // If the prefix part of NameEntryDevicePath matches KeyDevicePath, ensure
  // NameEntryDevicePath has no more Device path nodes after the prefix.
  //
  ASSERT (IdenticalPrefixSize <= SearchContext->DevicePathMaxSize);
  DevicePathSize = GetDevicePathSize (NameEntryDevicePath);
  if (IdenticalPrefixSize == DevicePathSize ||
      IdenticalPrefixSize == DevicePathSize - END_DEVICE_PATH_LENGTH)
  {
    return 0;
  }
  return -1;
}

STATIC
INTN
EFIAPI
CompareDeviceNameRelations (
  IN CONST VOID  *Buffer1,
  IN CONST VOID  *Buffer2
  )
{
  CONST H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION  *Entry1;
  CONST H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION  *Entry2;

  Entry1 = (CONST H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION *) Buffer1;
  Entry2 = (CONST H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION *) Buffer2;

  if (Entry1->ControllerNameIndex < Entry2->ControllerNameIndex) {
    return -1;
  }
  if (Entry1->ControllerNameIndex > Entry2->ControllerNameIndex) {
    return 1;
  }

  if (Entry1->NamespaceNameIndex < Entry2->NamespaceNameIndex) {
    return -1;
  }
  if (Entry1->NamespaceNameIndex > Entry2->NamespaceNameIndex) {
    return 1;
  }

  ASSERT (Entry1->NvmeControllerEntryIndex != Entry2->NvmeControllerEntryIndex);
  if (Entry1->NvmeControllerEntryIndex < Entry2->NvmeControllerEntryIndex) {
    return -1;
  }
  return 1;
}

/**

  @retval  EFI_SUCCESS
  @retval  EFI_NOT_FOUND
  @retval  EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
CreateNvmeDeviceNameRelations (
  IN H2O_SETUP_NVME_INFO_PRIVATE_DATA           *PrivateData,
  OUT H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION  **DeviceNameRelations
  )
{
  BOOLEAN                                  WouldOverflow;
  UINTN                                    AllocateSize;
  UINTN                                    *NameEntryIndices;
  UINTN                                    Index;

  H2O_SETUP_NVME_INFO_NAME_SEARCH_CONTEXT  SearchContext = {0};
  CONST EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  EFI_STATUS                               Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL       *NvmePassThru;
  EFI_DEVICE_PATH_PROTOCOL                 *NamespaceDevicePath;
  RETURN_STATUS                            SearchStatus;
  UINTN                                    ArrayIndex;
  UINTN                                    NameEntryIndex;
  UINTN                                    NameCount;
  INTN                                     CompareResult;
  UINTN                                    IdenticalPrefixSize;

  if ((DeviceNameRelations == NULL) || (PrivateData == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DeviceNameRelations/PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  IdenticalPrefixSize = 0;
  *DeviceNameRelations = NULL;

  ASSERT (PrivateData == &mNvmeInfoData);

  if (PrivateData->NvmeControllerCount <= 0) {
    return EFI_NOT_FOUND;
  }

  //
  // Allocate (*DeviceNameRelations)
  //
  WouldOverflow = PrivateData->NvmeControllerCount >
                    MAX_UINTN / sizeof (**DeviceNameRelations);
  if (WouldOverflow) {
    DEBUG ((EFI_D_ERROR, "%a: Arithmetic overflow\n", gEfiCallerBaseName));
  } else {
    AllocateSize = PrivateData->NvmeControllerCount *
                     sizeof (**DeviceNameRelations);
    *DeviceNameRelations = AllocatePool (AllocateSize);
  }
  if (*DeviceNameRelations == NULL) {
    DebugPrintOutOfResources ("DeviceNameRelations");
    goto DeviceNameRelationsFail;
  }

  NameEntryIndices = NULL;

  if (PrivateData->DeviceInfoLookupNameCount > 0) {
    //
    // Allocate NameEntryIndices
    //
    WouldOverflow = PrivateData->DeviceInfoLookupNameCount >
                      MAX_UINTN / sizeof (*NameEntryIndices);
    if (WouldOverflow) {
      DEBUG ((EFI_D_ERROR, "%a: Arithmetic overflow\n", gEfiCallerBaseName));
    } else {
      AllocateSize = PrivateData->DeviceInfoLookupNameCount *
                       sizeof (*NameEntryIndices);
      NameEntryIndices = AllocatePool (AllocateSize);
    }
    if (NameEntryIndices == NULL) {
      DebugPrintOutOfResources ("NameEntryIndices");
      goto NameEntryIndicesFail;
    }

    //
    // Prepare NameEntryIndices with data sorted by EFI Device Path.
    //
    for (Index = 0; Index < PrivateData->DeviceInfoLookupNameCount; Index++) {
      NameEntryIndices[Index] = Index;
    }
    PerformQuickSort (
      NameEntryIndices,
      PrivateData->DeviceInfoLookupNameCount,
      sizeof (*NameEntryIndices),
      CompareNameEntryIndicesByDevicePath
      );
  }

  //
  // Build NVMe device (controller entry)-to-name relations.
  //
  SearchContext.DeviceInfoLookupNameTable = PrivateData->DeviceInfoLookupNameTable;
  DevicePath = NULL;
  for (Index = 0; Index < PrivateData->NvmeControllerCount; Index++) {
    (*DeviceNameRelations)[Index].NvmeControllerEntryIndex = Index;
    (*DeviceNameRelations)[Index].ControllerNameIndex = MAX_UINTN;
    (*DeviceNameRelations)[Index].NamespaceNameIndex = MAX_UINTN;

    if (PrivateData->DeviceInfoLookupNameCount <= 0) {
      continue;
    }

    if (Index > 0 && PrivateData->NvmeControllerEntries[Index].Handle ==
        PrivateData->NvmeControllerEntries[Index - 1].Handle)
    {
      (*DeviceNameRelations)[Index].ControllerNameIndex =
        (*DeviceNameRelations)[Index - 1].ControllerNameIndex;

      //
      // Look up the NVMe namespace name.
      //
      if (DevicePath == NULL) {
        continue;
      }
      if (PrivateData->NvmeControllerEntries[Index].NamespaceId <= 0) {
        continue;
      }
      Status = gBS->OpenProtocol (
                      PrivateData->NvmeControllerEntries[Index].Handle,
                      &gEfiNvmExpressPassThruProtocolGuid,
                      (VOID **) &NvmePassThru,
                      gImageHandle, // AgentHandle
                      NULL,         // ControllerHandle
                      EFI_OPEN_PROTOCOL_GET_PROTOCOL
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((
          EFI_D_ERROR,
          "Handle %p: Open %a: %r\n",
          PrivateData->NvmeControllerEntries[Index].Handle,
          "EfiNvmExpressPassThru",
          Status
          ));
        continue;
      }

      ASSERT (PrivateData->NvmeInfo != NULL);
      NamespaceDevicePath = CreateNvmeNamespaceDevicePath (
                              PrivateData->NvmeInfo,
                              NvmePassThru,
                              DevicePath,
                              PrivateData->NvmeControllerEntries[
                                             Index].NamespaceId
                              );
      if (NamespaceDevicePath == NULL) {
        continue;
      }

      SearchContext.DevicePathMaxSize = MAX_UINTN;
      SearchStatus = BinarySearchEx (
                       NamespaceDevicePath,
                       NameEntryIndices,
                       PrivateData->DeviceInfoLookupNameCount,
                       sizeof (*NameEntryIndices),
                       CompareDevicePathWithNameEntryIndex,
                       &SearchContext,
                       &ArrayIndex
                       );
      FreePool (NamespaceDevicePath);
      if (!RETURN_ERROR (SearchStatus)) {
        NameEntryIndex = NameEntryIndices[ArrayIndex];
        (*DeviceNameRelations)[Index].NamespaceNameIndex = NameEntryIndex;
      }
      continue;
    }

    Status = gBS->OpenProtocol (
                    PrivateData->NvmeControllerEntries[Index].Handle,
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &DevicePath,
                    gImageHandle, // AgentHandle
                    NULL,         // ControllerHandle
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "Handle %p: Open %a: %r\n",
        PrivateData->NvmeControllerEntries[Index].Handle,
        "EfiDevicePath",
        Status
        ));
      DevicePath = NULL;
      continue;
    }

    SearchContext.DevicePathMaxSize = MAX_UINTN;
    NameCount = PrivateData->DeviceInfoLookupNameCount;
    while (SearchContext.DevicePathMaxSize >=
        sizeof (EFI_DEVICE_PATH_PROTOCOL) &&
        NameCount > 0)
    {
      SearchStatus = BinarySearchEx (
                       DevicePath,
                       NameEntryIndices,
                       NameCount,
                       sizeof (*NameEntryIndices),
                       CompareDevicePathWithNameEntryIndex,
                       &SearchContext,
                       &ArrayIndex
                       );
      if (!RETURN_ERROR (SearchStatus)) {
        NameEntryIndex = NameEntryIndices[ArrayIndex];
        (*DeviceNameRelations)[Index].ControllerNameIndex = NameEntryIndex;
        break;
      }

      if (ArrayIndex <= 0) {
        break;
      }
      NameEntryIndex = NameEntryIndices[ArrayIndex - 1];

      if (PrivateData->DeviceInfoLookupNameTable != NULL) {
        CompareResult = CompareDevicePath (
                          DevicePath,
                          PrivateData->DeviceInfoLookupNameTable[
                                         NameEntryIndex].DevicePath,
                          SearchContext.DevicePathMaxSize,
                          &IdenticalPrefixSize
                          );
        ASSERT (CompareResult > 0);
      }
      SearchContext.DevicePathMaxSize = IdenticalPrefixSize;
      NameCount = ArrayIndex;
    }
  }

  if (NameEntryIndices != NULL) {
    FreePool (NameEntryIndices);
  }

  PerformQuickSort (
    (*DeviceNameRelations),
    PrivateData->NvmeControllerCount,
    sizeof (**DeviceNameRelations),
    CompareDeviceNameRelations
    );

  return EFI_SUCCESS;

NameEntryIndicesFail:
  FreePool (*DeviceNameRelations);
  *DeviceNameRelations = NULL;
DeviceNameRelationsFail:
  return EFI_OUT_OF_RESOURCES;
}

STATIC
CHAR16 *
CreateRaidControllerDeviceString (
  IN EFI_HANDLE  DeviceHandle
  )
{
  EFI_STATUS           Status;
  CHAR8                PciDeviceIdString[
                         sizeof (" (VEN_FFFF&DEV_FFFF)")];
  EFI_PCI_IO_PROTOCOL  *PciIo;
  UINT8                PciReadBuffer[4];

  PciDeviceIdString[0] = '\0';
  Status = gBS->OpenProtocol (
                  DeviceHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0, // Offset
                          (sizeof (PciReadBuffer) / sizeof (UINT32)),
                          PciReadBuffer
                          );
    if (!EFI_ERROR (Status)) {
      (VOID) AsciiSPrint (
               PciDeviceIdString,
               sizeof (PciDeviceIdString),
               " (VEN_%04X&DEV_%04X)",
               (unsigned int) *(UINT16 *) &PciReadBuffer[0],
               (unsigned int) *(UINT16 *) &PciReadBuffer[2]
               );
    }
  }
  return CatSPrint (
           NULL,
           L"%s%a",
           L"RAID Controller",
           PciDeviceIdString
           );
}

VOID
CreateDevicePathTextFromControllerEntry (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData,
  IN CONST H2O_NVME_INFO_CONTROLLER_ENTRY  *ControllerEntry,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmePassThru OPTIONAL,
  OUT CHAR16                               **DevicePathText
  )
{
  EFI_STATUS                      Status;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePathBuffer;

  if ((PrivateData == NULL) || (ControllerEntry == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/ControllerEntry is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  if (DevicePathText != NULL) {
    *DevicePathText = NULL;
  }

  Status = gBS->OpenProtocol (
                  ControllerEntry->Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "Handle %p: Open %a: %r\n",
      ControllerEntry->Handle,
      "EfiDevicePath",
      Status
      ));
    return;
  }

  DevicePathBuffer = NULL;
  if (ControllerEntry->NamespaceId > 0) {
    DevicePathBuffer = CreateNvmeNamespaceDevicePath (
                         PrivateData->NvmeInfo,
                         NvmePassThru,
                         DevicePath,
                         ControllerEntry->NamespaceId
                         );
    if (DevicePathBuffer != NULL) {
      DevicePath = DevicePathBuffer;
    }
  }

  if (DevicePathText != NULL && DevicePath != NULL) {
    *DevicePathText = ConvertDevicePathToText (
                        DevicePath,
                        TRUE, // DisplayOnly
                        TRUE  // AllowShortcuts
                        );
    if (*DevicePathText == NULL) {
      DebugPrintOutOfResources ("DevicePathText");
    }
  }
  if (DevicePathBuffer != NULL) {
    FreePool (DevicePathBuffer);
  }
}

STATIC
EFI_STATUS
CreateNvmeControllerGotoOpStrings (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData,
  IN H2O_NVME_INFO_CONTROLLER_ENTRY        *ControllerEntry,
  IN CONST CHAR16                          *PortName OPTIONAL,
  OUT CHAR16                               **PromptString,
  OUT CHAR16                               **HelpString
  )
{
  EFI_STATUS                          Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru;
  H2O_NVME_INFO_PROTOCOL              *NvmeInfo;
  NVME_ADMIN_CONTROLLER_DATA          *IdentifyControllerData;
  CHAR16                              *ModelNumberString;
  CHAR16                              *DevicePathText;
  CHAR16                              NamespaceIdString[sizeof ("NSID FFFFFFFEh")];

  if ((PrivateData == NULL) || (ControllerEntry == NULL) || (PromptString == NULL) || (HelpString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/ControllerEntry/PromptString/HelpString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *PromptString = NULL;
  *HelpString = NULL;

  Status = gBS->OpenProtocol (
                  ControllerEntry->Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID **) &NvmePassThru,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "Handle %p: Open %a: %r\n",
      ControllerEntry->Handle,
      "EfiNvmExpressPassThru",
      Status
      ));
    return Status;
  }

  NvmeInfo = PrivateData->NvmeInfo;
  Status = NvmeInfo->IdentifyController (
                       NvmePassThru,
                       ControllerEntry->NamespaceId,
                       &IdentifyControllerData
                       );
  if (!EFI_ERROR (Status)) {
    ModelNumberString = GetNvmeModelNumberFromIdentifyData (
                          IdentifyControllerData
                          );
    FreePool (IdentifyControllerData);
  } else if (ControllerEntry->IsRaidController) {
    //
    // Intel VMD (Volume Management Device), which is a kind of RAID
    // controller, can fail on getting Identify Controller data of
    // Namespace ID 0.
    //
    ModelNumberString = CreateRaidControllerDeviceString (
                          ControllerEntry->Handle
                          );
  } else {
    return Status;
  }
  if (ModelNumberString == NULL) {
    DebugPrintOutOfResources ("ModelNumberString");
    return EFI_OUT_OF_RESOURCES;
  }

  CreateDevicePathTextFromControllerEntry (
    PrivateData,
    ControllerEntry,
    NvmePassThru,
    &DevicePathText
    );

  if (PortName == NULL && ControllerEntry->NamespaceId > 0) {
    ASSERT (ControllerEntry->NamespaceId < 0xFFFFFFFF);
    UnicodeSPrint (
      NamespaceIdString,
      sizeof (NamespaceIdString),
      L"NSID %Xh",
      (unsigned int) ControllerEntry->NamespaceId
      );
    PortName = NamespaceIdString;
  }

  *PromptString = CatSPrint (
                    NULL,
                    L"%s%s%s%s",
                    ((ControllerEntry->NamespaceId > 0) ? L"- " : L""),
                    ((PortName != NULL) ? PortName : L""),
                    ((PortName != NULL) ? L": " : L""),
                    ModelNumberString
                    );
  FreePool (ModelNumberString);
  if ((*PromptString) == NULL) {
    if (DevicePathText != NULL) {
      FreePool (DevicePathText);
    }
    DebugPrintOutOfResources ("PromptString");
    return EFI_OUT_OF_RESOURCES;
  }

  *HelpString = CatSPrint (
                  NULL,
                  L"EFI Device Path:\n%s\n",
                  ((DevicePathText != NULL) ? DevicePathText : L"?")
                  );
  if (DevicePathText != NULL) {
    FreePool (DevicePathText);
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

STATIC
EFI_STATUS
CreateEmptyPortTextOpCode (
  IN H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData,
  IN OUT VOID                          *OpCodeHandle,
  IN EFI_STRING_ID                     PromptStringId,
  IN CONST CHAR16                      *PortName
  )
{
  EFI_STRING_ID  NewStringId;
  UINT8          *OpCodePtr;

  if ((PrivateData == NULL) || (OpCodeHandle == NULL) || (PortName == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/OpCodeHandle/PortName is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  NewStringId = HiiSetStringFromFormatString (
                  PrivateData->HiiHandle,
                  PromptStringId,
                  STRING_TOKEN (STR_PORT_NO_DEVICE_FORMAT),
                  PortName
                  );
  if (NewStringId == 0) {
    DebugPrintOutOfResources ("\"No Device\" port HII string");
    return EFI_OUT_OF_RESOURCES;
  }

  OpCodePtr = CreateGrayedOutTextOpCode (
                OpCodeHandle,
                PromptStringId,
                STRING_TOKEN (0x0000) // TextTwo
                );
  if (OpCodePtr == NULL) {
    DebugPrintOutOfResources ("\"No Device\" port IFR op-code");
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}

/**
  Update the form data in the NVME information form

**/
STATIC
EFI_STATUS
UpdateNvmeControllerListForm (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData
  )
{
  EFI_STATUS                                ReturnStatus;
  H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION  *DeviceNameRelations;

  VOID                                      *StartOpCodeHandle;
  VOID                                      *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                        *StartLabel;
  EFI_IFR_GUID_LABEL                        *EndLabel;
  EFI_HII_HANDLE                            HiiHandle;

  UINTN                                     Index;
  UINTN                                     DynamicStringCount;
  CONST EFI_STRING_ID                       *StringIdList;
  UINTN                                     StringIdListIndex;
  UINTN                                     RelationIndex;
  UINTN                                     ControllerIndex;
  CONST CHAR16                              *PortName;
  UINTN                                     NameEntryIndex;
  CONST EFI_DEVICE_PATH_PROTOCOL            *DevicePath;

  EFI_STATUS                                Status;
  CHAR16                                    *PromptString;
  CHAR16                                    *HelpString;
  EFI_STRING_ID                             PromptStringId;
  EFI_STRING_ID                             HelpStringId;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  ReturnStatus = CreateNvmeDeviceNameRelations (
                   PrivateData,
                   &DeviceNameRelations
                   );
  if (EFI_ERROR (ReturnStatus)) {
    if (ReturnStatus != EFI_NOT_FOUND) {
      return ReturnStatus;
    }
    DeviceNameRelations = NULL;
  }

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DebugPrintOutOfResources ("StartOpCodeHandle");
    ReturnStatus = EFI_OUT_OF_RESOURCES;
    goto StartOpCodeHandleFail;
  }
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DebugPrintOutOfResources ("EndOpCodeHandle");
    ReturnStatus = EFI_OUT_OF_RESOURCES;
    goto EndOpCodeHandleFail;
  }

  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                        StartOpCodeHandle,
                                        &gEfiIfrTianoGuid,
                                        NULL,
                                        sizeof (EFI_IFR_GUID_LABEL)
                                        );
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_NVME_INFO_LABEL_START;

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                      EndOpCodeHandle,
                                      &gEfiIfrTianoGuid,
                                      NULL,
                                      sizeof (EFI_IFR_GUID_LABEL)
                                      );
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = H2O_NVME_INFO_LABEL_END;

  HiiHandle = PrivateData->HiiHandle;

  DynamicStringCount = PrivateData->NvmeControllerCount * 2;
  DynamicStringCount += PrivateData->DeviceInfoLookupNameCount * 1;

  if (DynamicStringCount > MAX_UINT16) {
    DynamicStringCount = MAX_UINT16;
  }

  ReturnStatus = SetFormDynamicStringCount (
                   PrivateData->HiiStringIdManager,
                   0, // FormIndex
                   (UINT16) DynamicStringCount,
                   &StringIdList
                   );
  if (EFI_ERROR (ReturnStatus) || (StringIdList == NULL)) {
    goto OutOfStringIds;
  }

  StringIdListIndex = 0;
  RelationIndex = 0;
  for (Index = 0; TRUE; Index++) {
    //
    // Create "goto" op-codes for controllers whose display names are ordered
    // "less than" PrivateData->DeviceInfoLookupNameTable[Index].
    //
    for (; RelationIndex < PrivateData->NvmeControllerCount; RelationIndex++) {
      if (DeviceNameRelations == NULL) {
        break;
      }

      if (DeviceNameRelations[RelationIndex].ControllerNameIndex > Index &&
          Index < PrivateData->DeviceInfoLookupNameCount)
      {
        break;
      }
      if (DeviceNameRelations[RelationIndex].ControllerNameIndex == Index) {
        Index++;
      }

      ControllerIndex =
        DeviceNameRelations[RelationIndex].NvmeControllerEntryIndex;
      if (ControllerIndex > MAX_UINT16 - NVME_DEVICE_KEY) {
        continue;
      }

      PortName = NULL;
      NameEntryIndex = DeviceNameRelations[RelationIndex].NamespaceNameIndex;
      if (NameEntryIndex < PrivateData->DeviceInfoLookupNameCount) {
        PortName =
          PrivateData->DeviceInfoLookupNameTable[NameEntryIndex].DisplayName;
      }
      NameEntryIndex = DeviceNameRelations[RelationIndex].ControllerNameIndex;
      if (NameEntryIndex < PrivateData->DeviceInfoLookupNameCount) {
        PortName =
          PrivateData->DeviceInfoLookupNameTable[NameEntryIndex].DisplayName;
      }

      Status = CreateNvmeControllerGotoOpStrings (
                 PrivateData,
                 &PrivateData->NvmeControllerEntries[ControllerIndex],
                 PortName,
                 &PromptString,
                 &HelpString
                 );
      if (EFI_ERROR (Status)) {
        continue;
      }

      ASSERT (PromptString != NULL);
      PromptStringId = 0;
      if (StringIdListIndex + 1 < DynamicStringCount) {
        ASSERT (StringIdList != NULL);
        PromptStringId = HiiSetString (
                           HiiHandle,
                           StringIdList[StringIdListIndex],
                           (EFI_STRING) PromptString,
                           NULL
                           );
        StringIdListIndex++;
      }
      FreePool (PromptString);

      if (PromptStringId == 0) {
        DebugPrintOutOfResources ("PromptStringId");
        continue;
      }

      HelpStringId = 0;
      if (HelpString != NULL && StringIdListIndex < DynamicStringCount) {
        HelpStringId = HiiSetString (
                         HiiHandle,
                         StringIdList[StringIdListIndex],
                         (EFI_STRING) HelpString,
                         NULL
                         );
        StringIdListIndex++;
        FreePool (HelpString);
      }

      HiiCreateGotoOpCode (
        StartOpCodeHandle,
        H2O_NVME_INFO_CONTROLLER_INFO_FORM_ID,
        PromptStringId,
        HelpStringId,
        EFI_IFR_FLAG_CALLBACK,
        (EFI_QUESTION_ID) (NVME_DEVICE_KEY + ControllerIndex)
        );
    }

    if (Index >= PrivateData->DeviceInfoLookupNameCount) {
      break;
    }

    if (PrivateData->DeviceInfoLookupNameTable[Index].DisplayName == NULL) {
      continue;
    }

    DevicePath = PrivateData->DeviceInfoLookupNameTable[Index].DevicePath;
    Status = FindDevicePathNodeByType (
               DevicePath,
               MESSAGING_DEVICE_PATH,
               MSG_NVME_NAMESPACE_DP,
               (UINT16) sizeof (NVME_NAMESPACE_DEVICE_PATH),
               NULL // Offset
               );
    if (!EFI_ERROR (Status)) {
      continue;
    }

    //
    // Create a "No Device" text op-code for empty ports.
    //
    if (StringIdListIndex < DynamicStringCount) {
      Status = CreateEmptyPortTextOpCode (
                 PrivateData,
                 StartOpCodeHandle,
                 StringIdList[StringIdListIndex],
                 PrivateData->DeviceInfoLookupNameTable[Index].DisplayName
                 );
      StringIdListIndex++;
      if (EFI_ERROR (Status)) {
        continue;
      }
    }
  }

  Status = SetFormDynamicStringCount (
             PrivateData->HiiStringIdManager,
             0, // FormIndex
             (UINT16) StringIdListIndex,
             NULL
             );
  ASSERT_EFI_ERROR (Status);

OutOfStringIds:

  if ((PrivateData->NvmeControllerCount +
      PrivateData->DeviceInfoLookupNameCount) <= 0)
  {
    (VOID) HiiCreateSubTitleOpCode (
             StartOpCodeHandle,
             STRING_TOKEN (STR_NO_DEVICE),
             STRING_TOKEN (0x0000), // Help
             0,                     // Flags
             0                      // Scope
             );
  }

  HiiUpdateForm (
    HiiHandle,
    &gH2ONvmeInfoFormsetGuid,
    H2O_NVME_INFO_CONTROLLER_LIST_FORM_ID,
    StartOpCodeHandle,
    EndOpCodeHandle
    );

  HiiFreeOpCodeHandle (EndOpCodeHandle);
EndOpCodeHandleFail:
  HiiFreeOpCodeHandle (StartOpCodeHandle);
StartOpCodeHandleFail:
  if (DeviceNameRelations != NULL) {
    FreePool (DeviceNameRelations);
  }

  return ReturnStatus;
}

VOID
UpdatePciLocationHiiString (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData,
  IN H2O_NVME_INFO_CONTROLLER_ENTRY        *ControllerEntry
  )
{
  CONST CHAR16         *ValueString;
  CONST CHAR16         *HelpString;
  CHAR16               *HelpStringBuffer;
  EFI_STATUS           Status;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  UINTN                SegmentNum;
  UINTN                BusNum;
  UINTN                DeviceNum;
  UINTN                FunctionNum;
  CHAR16               PciLocationStringBuffer[
                         sizeof (L"FFFF:FF:1F.7") / sizeof (CHAR16)];
  UINTN                PciLocationStringSize = sizeof (L"FFFF:FF:1F.7");
  EFI_STRING_ID        FormatStringId;
  EFI_STRING           Format;
  EFI_STRING_ID        NewStringId;

  ValueString = L"N/A";
  HelpString = L"N/A";
  HelpStringBuffer = NULL;

  if ((PrivateData == NULL) || (ControllerEntry == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/ControllerEntry is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  Status = gBS->OpenProtocol (
                  ControllerEntry->Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = PciIo->GetLocation (
                      PciIo,
                      &SegmentNum,
                      &BusNum,
                      &DeviceNum,
                      &FunctionNum
                      );
    ASSERT_EFI_ERROR (Status);

    //
    // Same notation as in Linux kernel and 'lspci' utility for Unixes.
    //
    if (SegmentNum != 0) {
      UnicodeSPrintAsciiFormat (
        PciLocationStringBuffer,
        PciLocationStringSize,
        "%04x:%02x:%02x.%x",
        (unsigned int) SegmentNum,
        (unsigned int) BusNum,
        (unsigned int) DeviceNum,
        (unsigned int) FunctionNum
        );
      FormatStringId = STRING_TOKEN (STR_NVME_PCI_LOCATION_HELP_FORMAT_FULL);
    } else {
      UnicodeSPrintAsciiFormat (
        PciLocationStringBuffer,
        PciLocationStringSize,
        "%02x:%02x.%x",
        (unsigned int) BusNum,
        (unsigned int) DeviceNum,
        (unsigned int) FunctionNum
        );
      FormatStringId = STRING_TOKEN (STR_NVME_PCI_LOCATION_HELP_FORMAT_NOSEG);
    }

    ValueString = PciLocationStringBuffer;

    Format = HiiGetString (
               PrivateData->HiiHandle,
               FormatStringId,
               NULL // Language
               );
    if (Format == NULL) {
      DebugPrintOutOfResources ("PciLocationHelpFormat");
    } else {
      if (SegmentNum != 0) {
        HelpStringBuffer = CatSPrint (
                             NULL,
                             (CHAR16 *) Format,
                             (unsigned int) SegmentNum,
                             (unsigned int) BusNum,
                             (unsigned int) DeviceNum,
                             (unsigned int) FunctionNum,
                             ValueString
                             );
      } else {
        HelpStringBuffer = CatSPrint (
                             NULL,
                             (CHAR16 *) Format,
                             (unsigned int) BusNum,
                             (unsigned int) DeviceNum,
                             (unsigned int) FunctionNum,
                             ValueString
                             );
      }
      FreePool (Format);
      HelpString = HelpStringBuffer;
    }
  }

  NewStringId = HiiSetString (
                  PrivateData->HiiHandle,
                  STRING_TOKEN (STR_NVME_PCI_LOCATION_VALUE),
                  (CONST EFI_STRING) ValueString,
                  NULL // SupportedLanguages
                  );
  if (NewStringId == 0x0000) {
    DebugPrintOutOfResources ("PciLocationValueStringId");
  }

  NewStringId = HiiSetString (
                  PrivateData->HiiHandle,
                  STRING_TOKEN (STR_NVME_PCI_LOCATION_HELP),
                  (CONST EFI_STRING) HelpString,
                  NULL // SupportedLanguages
                  );
  if (NewStringId == 0x0000) {
    DebugPrintOutOfResources ("PciLocationHelpStringId");
  }
}

/**
  Allocate a new op-code handle with an EDK2 label.

  @param  LabelNumber   Label number.

  @retval NULL          The operation failed.
  @retval Other         The op-code handle.

**/
STATIC
VOID *
AllocateOpCodeHandleWithLabel (
  IN UINT16                                LabelNumber
  )
{
  VOID                                     *OpCodeHandle;
  EFI_IFR_GUID_LABEL                       *Edk2LabelPtr;

  OpCodeHandle = HiiAllocateOpCodeHandle ();
  if (OpCodeHandle == NULL) {
    return NULL;
  }

  Edk2LabelPtr = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                          OpCodeHandle,
                                          &gEfiIfrTianoGuid,
                                          NULL,
                                          sizeof (EFI_IFR_GUID_LABEL)
                                          );
  if (Edk2LabelPtr == NULL) {
    return NULL;
  }

  Edk2LabelPtr->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  Edk2LabelPtr->Number       = LabelNumber;

  return OpCodeHandle;
}

/**
  Update the device information form for the requested controller.

  @param  PrivateData   Driver private data.
  @param  QuestionId    The controller's unique question ID. 

  @retval EFI_SUCCESS   The operation completed successfully.
  @retval EFI_ABORTED   The operation was aborted.

**/
STATIC
EFI_STATUS
UpdateNvmeControllerInfoForm (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA     *PrivateData,
  IN     EFI_QUESTION_ID                      QuestionId
  )
{
  VOID                                        *StartOpCodeHandle;
  VOID                                        *EndOpCodeHandle;
  NVME_DETAIL_DISPLAY_LIST_ELEMENT            *DisplayList;
  UINTN                                       DisplayListSize;
  UINTN                                       DisplayListIndex;
  NVME_DETAIL_DISPLAY_SECTION_ENTRY           SectionEntries[] = {
                                                { FALSE, AddControllerInfoSection },
                                                { FALSE, AddNamespaceInfoSection },
                                                { FALSE, AddSmartInfoSection },
                                                };
  UINT8                                       SectionNumber;

  StartOpCodeHandle = AllocateOpCodeHandleWithLabel (H2O_NVME_INFO_LABEL_START);
  if (StartOpCodeHandle == NULL) {
    return EFI_ABORTED;
  }

  EndOpCodeHandle = AllocateOpCodeHandleWithLabel (H2O_NVME_INFO_LABEL_END);
  if (EndOpCodeHandle == NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return EFI_ABORTED;
  }

  DisplayList     = (NVME_DETAIL_DISPLAY_LIST_ELEMENT *) PcdGetPtr (PcdH2ONvmeDetailDisplayList);
  DisplayListSize = PcdGetSize (PcdH2ONvmeDetailDisplayList) / sizeof (NVME_DETAIL_DISPLAY_LIST_ELEMENT);

  for (DisplayListIndex = 0; DisplayListIndex < DisplayListSize; DisplayListIndex++) {
    SectionNumber = DisplayList[DisplayListIndex].SectionNumber;

    if (SectionNumber >= ARRAY_SIZE (SectionEntries)) {
      continue;
    }

    if (SectionEntries[SectionNumber].IsSectionAdded == TRUE) {
      continue;
    }

    SectionEntries[SectionNumber].AddNvmeFormSection (
                                    StartOpCodeHandle,
                                    &PrivateData->NvmeControllerEntries[QuestionId - NVME_DEVICE_KEY]
                                    );
    
    SectionEntries[SectionNumber].IsSectionAdded = TRUE;
  }

  HiiUpdateForm (
    PrivateData->HiiHandle,
    &gH2ONvmeInfoFormsetGuid,
    H2O_NVME_INFO_CONTROLLER_INFO_FORM_ID,
    StartOpCodeHandle,
    EndOpCodeHandle
    );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  return EFI_SUCCESS;
}


/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.


  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Request         A null-terminated Unicode string in <ConfigRequest> format.
  @param Progress        On return, points to a character in the Request string.
                         Points to the string's null terminator if request was successful.
                         Points to the most recent '&' before the first failing name/value
                         pair (or the beginning of the string if the failure is in the
                         first name/value pair) if the request was not successful.
  @param Results         A null-terminated Unicode string in <ConfigAltResp> format which
                         has all values filled in for the names in the Request string.
                         String to be allocated by the called function.

  @retval  EFI_SUCCESS            The Results is filled with the requested values.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval  EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
NvmeInfoExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Request,
  OUT EFI_STRING                           *Progress,
  OUT EFI_STRING                           *Results
  )
{
  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_NOT_FOUND;
}

/**
  This function processes the results of changes in configuration.


  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Configuration   A null-terminated Unicode string in <ConfigResp> format.
  @param Progress        A pointer to a string filled in with the offset of the most
                         recent '&' before the first failing name/value pair (or the
                         beginning of the string if the failure is in the first
                         name/value pair) or the terminating NULL if all was successful.

  @retval  EFI_SUCCESS            The Results is processed successfully.
  @retval  EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
NvmeInfoRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  )
{
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_NOT_FOUND;
}

/**
  This function processes the results of changes in configuration.


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
NvmeInfoFormCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN OUT EFI_IFR_TYPE_VALUE                *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData;
  EFI_STATUS                        Status;

  PrivateData = H2O_SETUP_NVME_INFO_PRIVATE_FROM_CONFIG_ACCESS (This);

  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    return EFI_UNSUPPORTED;
  }

  if (Type == EFI_IFR_TYPE_REF &&
      QuestionId >= NVME_DEVICE_KEY &&
      QuestionId < NVME_DEVICE_KEY + PrivateData->NvmeControllerCount)
  {
    Status = UpdateNvmeControllerInfoForm (PrivateData, QuestionId);

    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}

STATIC
VOID
DestroyNvmeControllerEntryList (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData
  )
{
  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  if (PrivateData->NvmeControllerEntries == NULL) {
    return;
  }

  FreePool (PrivateData->NvmeControllerEntries);
  PrivateData->NvmeControllerEntries = NULL;
  PrivateData->NvmeControllerCount = 0;
}

/**
  Free the Nvm data and the Hii Handle.

  @retval
  @return

**/
VOID
EFIAPI
CleanUpNvmeInfoData (
  )
{
  if (mNvmeInfoData.DeviceInfoLookupNameTable != NULL) {
    if (mNvmeInfoData.DeviceInfoLookup != NULL) {
      mNvmeInfoData.DeviceInfoLookup->DestroyNameTable (
                                        mNvmeInfoData.DeviceInfoLookupNameTable
                                        );
    }
    mNvmeInfoData.DeviceInfoLookupNameTable = NULL;
    mNvmeInfoData.DeviceInfoLookupNameCount = 0;
  }

  DestroyNvmeControllerEntryList (&mNvmeInfoData);

  if (mNvmeInfoData.SeviceHandle != NULL) {
    if (mNvmeInfoData.HiiHandle != NULL) {
      if (mNvmeInfoData.HiiStringIdManager != NULL) {
        DestroyHiiStringIdManager (mNvmeInfoData.HiiStringIdManager);
        mNvmeInfoData.HiiStringIdManager = NULL;
      }
      HiiRemovePackages (mNvmeInfoData.HiiHandle);
      mNvmeInfoData.HiiHandle = NULL;
    }
    gBS->UninstallMultipleProtocolInterfaces (
           mNvmeInfoData.SeviceHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiNvmeInfoVendorDevPath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mNvmeInfoData.ConfigAccess,
           (EFI_GUID *) NULL
           );
    mNvmeInfoData.SeviceHandle = NULL;
  }
}

/**
  Unloads this driver and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
NvmeInfoUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  if (mNvmeInfoData.SetupUtilityEvent != NULL) {
    gBS->CloseEvent (mNvmeInfoData.SetupUtilityEvent);
  }
  
  CleanUpNvmeInfoData ();

  return EFI_SUCCESS;
}

/**
  To collect data and initialize the nvme info formset after entering setup utility application

  @param[in]  Event                   Event
  @param[in]  Context                 Context

  No return value.
**/
VOID
NvmeSetupCallback (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
  )
{
  EFI_STATUS                               Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL   *SetupUtilityApp;
  EFI_HII_HANDLE                           HiiHandle;
  HII_STRING_ID_MANAGER                    HiiStringIdManager;
  H2O_NVME_INFO_PROTOCOL                   *NvmeInfo;
  UINTN                                    NvmeControllerCount;

  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  NULL,
                  (VOID **) &SetupUtilityApp
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (SetupUtilityApp->VfrDriverState == ShutdownSetupUtility) {
    goto Shutdown;
  }
  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    if (mNvmeInfoData.SeviceHandle == NULL) {
      Status = gBS->LocateProtocol (
                      &gH2ONvmeInfoProtocolGuid,
                      NULL, // Registration
                      (VOID **) &mNvmeInfoData.NvmeInfo
                      );
      if (EFI_ERROR (Status)) {
        goto Shutdown;
      }

      //
      // Initialize driver private data
      //
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &mNvmeInfoData.SeviceHandle,
                      &gEfiDevicePathProtocolGuid,
                      &mHiiNvmeInfoVendorDevPath,
                      &gEfiHiiConfigAccessProtocolGuid,
                      &mNvmeInfoData.ConfigAccess,
                      (EFI_GUID *) NULL
                      );
      if (EFI_ERROR (Status)) {
        goto Shutdown;
      }

      //
      // Publish our HII data
      //
      HiiHandle = HiiAddPackages (
                    &gH2ONvmeInfoFormsetGuid,
                    mNvmeInfoData.SeviceHandle,
                    NvmeInfoSetupUtilityDxeStrings,
                    NvmeInfoVfrBin,
                    (VOID *) NULL
                    );
      if (HiiHandle == NULL) {
        goto Shutdown;
      }
      mNvmeInfoData.HiiHandle = HiiHandle;

      HiiStringIdManager = CreateHiiStringIdManager (
                             HiiHandle,
                             2 // NumberOfForms
                             );
      if (HiiStringIdManager == NULL) {
        goto Shutdown;
      }
      mNvmeInfoData.HiiStringIdManager = HiiStringIdManager;
    }

    NvmeInfo = mNvmeInfoData.NvmeInfo;
    if (NvmeInfo == NULL) {
      goto Shutdown;
    }

    DestroyNvmeControllerEntryList (&mNvmeInfoData);

    Status = NvmeInfo->CollectControllers (
                         &NvmeControllerCount,
                         &mNvmeInfoData.NvmeControllerEntries
                         );
    if (!EFI_ERROR (Status)) {
      mNvmeInfoData.NvmeControllerCount = NvmeControllerCount;
    }

    (VOID) CreateDeviceInfoLookupNameTable (&mNvmeInfoData);

    (VOID) UpdateNvmeControllerListForm (&mNvmeInfoData);
  }
  return;

Shutdown:
  CleanUpNvmeInfoData ();
  return;
}

/**
  The entry point for the driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
NvmeInfoEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status;
  VOID                                  *Registration;

  //
  // When execute Setup Utility application, install HII data
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK - 1,
                  NvmeSetupCallback,
                  NULL,
                  &mNvmeInfoData.SetupUtilityEvent
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiSetupUtilityApplicationProtocolGuid,
                    mNvmeInfoData.SetupUtilityEvent,
                    &Registration
                    );
  }

  return EFI_SUCCESS;
}
