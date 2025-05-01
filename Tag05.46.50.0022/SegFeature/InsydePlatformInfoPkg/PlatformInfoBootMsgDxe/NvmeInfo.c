/** @file


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

#include <Protocol/PciIo.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg
#include <Protocol/H2ODeviceInfoLookup.h>     // InsydePlatformInfoPkg
#include <Protocol/H2ONvmeInfo.h>             // InsydePlatformInfoPkg

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>                   // MdeModulePkg
#include <Library/BlockIoCapacityLib.h>       // InsydePlatformInfoPkg
#include <Library/DeviceInfoLookupDataLib.h>  // InsydePlatformInfoPkg
#include <Library/NvmeInfoLib.h>              // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

STATIC H2O_NVME_INFO_PROTOCOL  *mNvmeInfo = NULL;

STATIC
EFI_STATUS
LookupNvmePortName (
  IN OUT H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  *Context,
  IN CONST H2O_NVME_INFO_CONTROLLER_ENTRY        *ControllerEntry,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL          *NvmePassThru OPTIONAL,
  OUT CHAR16                                     **DisplayName
  )
{
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup;
  EFI_STATUS                         Status;
  CONST EFI_DEVICE_PATH_PROTOCOL     *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePathBuffer;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES  AttributeFlags;
  UINTN                              AttributesValue;

  if ((Context == NULL) || (ControllerEntry == NULL) || (DisplayName == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Context/ControllerEntry/DisplayName is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *DisplayName = NULL;

  InitDeviceInfoLookupProtocol (&Context->DeviceInfoLookup);

  DeviceInfoLookup = Context->DeviceInfoLookup;
  if (DeviceInfoLookup == NULL) {
    return EFI_UNSUPPORTED;
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
    return Status;
  }

  DevicePathBuffer = NULL;
  if (ControllerEntry->NamespaceId > 0) {
    DevicePathBuffer = CreateNvmeNamespaceDevicePath (
                         mNvmeInfo,
                         NvmePassThru,
                         DevicePath,
                         ControllerEntry->NamespaceId
                         );
    if (DevicePathBuffer != NULL) {
      DevicePath = DevicePathBuffer;
    }
  }

  AttributeFlags.Uint8 = 0;
  AttributeFlags.Bits.IsNvme = 1;
  AttributesValue = AttributeFlags.Uint8;

  Status = DeviceInfoLookup->GetAncestorDeviceInfoOfType (
                               DeviceInfoLookup,
                               DevicePath,
                               &AttributesValue,
                               NULL, // AncestorDevicePath
                               NULL, // PcdTokenNumber
                               NULL, // AcpiPath
                               DisplayName
                               );

  FreePoolIfNonNull (DevicePathBuffer);

  return Status;
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

STATIC
VOID
ComputeNvmeTotalCapacity (
  IN H2O_NVME_INFO_PROTOCOL              *NvmeInfo,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  OUT UINT32                             *NumOfValidNamespaces,
  OUT CHAR16                             **CapacityString
  )
{
  UINT64                       TotalCapacity;
  UINT32                       NamespaceId;
  EFI_STATUS                   Status;
  NVME_ADMIN_NAMESPACE_DATA    *NamespaceData;
  UINT32                       BlockSize;
  UINT64                       NumberOfBlocks;
  UINT64                       NsCapacity;
  H2O_CAPACITY_REPRESENTATION  CapacityRepresentation;
  CHAR8                        Buffer[CAPACITY_REP_STR_MAX_SIZE];
  UINTN                        Length;

  if ((NvmeInfo == NULL) || (NvmePassThru == NULL) || (NumOfValidNamespaces == NULL) || (CapacityString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmeInfo/NvmePassThru/NumOfValidNamespaces/CapacityString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  *NumOfValidNamespaces = 0;
  TotalCapacity = 0;
  BlockSize = 0;
  NumberOfBlocks = 0;
  NamespaceId = 0xFFFFFFFF;
  while (TRUE) {
    Status = NvmePassThru->GetNextNamespace (NvmePassThru, &NamespaceId);
    if (EFI_ERROR (Status)) {
      break;
    }

    ++(*NumOfValidNamespaces);

    Status = NvmeInfo->IdentifyNamespace (
                         NvmePassThru,
                         NamespaceId,
                         &NamespaceData
                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a: Fail to get NVMe Identify Namespace data (protocol %p, NSID 0x%08x): %r\n",
        gEfiCallerBaseName,
        NvmePassThru,
        (unsigned int) NamespaceId,
        Status
        ));
      continue;
    }

    GetNvmeNamespaceCapacity (NamespaceData, &BlockSize, &NumberOfBlocks);
    FreePool (NamespaceData);

    if (NumberOfBlocks > MAX_UINT64 / BlockSize) {
      NsCapacity = MAX_UINT64;
      TotalCapacity = MAX_UINT64;
    } else {
      NsCapacity = NumberOfBlocks * BlockSize;
      if (TotalCapacity > MAX_UINT64 - NsCapacity) {
        TotalCapacity = MAX_UINT64;
      } else {
        TotalCapacity += NsCapacity;
      }
    }
  }

  if (*NumOfValidNamespaces <= 0) {
    *CapacityString = NULL;
    return;
  }
  if (*NumOfValidNamespaces == 1) {
    ComputeBlockIoCapacity (
      BlockSize,
      (NumberOfBlocks - 1),
      FORMULA_NORMAL,
      &CapacityRepresentation,
      NULL // CapacityTotalBytes
      );
  } else {
    ComputeBlockIoCapacity (
      1,
      TotalCapacity,
      FORMULA_NORMAL,
      &CapacityRepresentation,
      NULL // CapacityTotalBytes
      );
  }
  Length = BlockIoCapacityRepToString (
             &CapacityRepresentation,
             CAPACITY_REP_TO_STR_BASE_1000,
             Buffer
             );
  *CapacityString = CatSPrint (NULL, L"%a", Buffer);
  return;
}

/**
  Helper function to get S.M.A.R.T support status string.

  @param[in]  ControllerInfo  Controller Info.

**/
STATIC
VOID
GetSmartStatusStrings (
  IN H2O_NVME_INFO_PROTOCOL              *NvmeInfo,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  IN EFI_HII_HANDLE                      HiiHandle,
  IN CONST CHAR8                         *Language OPTIONAL,
  OUT INT32                              *Attribute,
  OUT EFI_STRING                         *String,
  OUT EFI_STRING                         *String2
  )
{
  EFI_STRING_ID               StringId;
  EFI_STRING_ID               StringId2;

  EFI_STATUS                  Status;
  NVME_SMART_HEALTH_INFO_LOG  *SmartHealthInfo;
  BOOLEAN                     HasCriticalWarning;

  if ((NvmeInfo == NULL) || (NvmePassThru == NULL) || (HiiHandle == NULL) ||
      (Attribute == NULL) || (String == NULL) || (String2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmeInfo/NvmePassThru/HiiHandle/Attribute/String/String2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  StringId = STRING_TOKEN (STR_SMART_NO_INFORMATION);
  StringId2 = STRING_TOKEN (0x0000);
  *Attribute = (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
  *String = NULL;
  *String2 = NULL;

  Status = NvmeInfo->GetSmartHealthInfo (
                       NvmePassThru,
                       NamespaceId,
                       &SmartHealthInfo
                       );
  if (!EFI_ERROR (Status)) {
    *Attribute = (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    StringId = STRING_TOKEN (STR_SMART_STATUS);
    StringId2 = STRING_TOKEN (STR_SMART_STATUS_OK);

    HasCriticalWarning = ((UINT8 *) SmartHealthInfo)[0] != 0x00;
    if (HasCriticalWarning) {
      *Attribute = (EFI_LIGHTRED | EFI_BACKGROUND_BLACK);
      StringId2 = STRING_TOKEN (STR_SMART_STATUS_BAD);
    }

    FreePool (SmartHealthInfo);
  }

  if (StringId2 != STRING_TOKEN (0x0000)) {
    *String2 = HiiGetString (HiiHandle, StringId2, Language);
  } else {
    *String2 = (EFI_STRING) AllocateZeroPool (sizeof (L""));
  }
  if ((*String2) == NULL) {
    return;
  }
  *String = HiiGetString (HiiHandle, StringId, Language);
  if ((*String) == NULL) {
    FreePool (*String2);
  }
  return;
}

/**
**/
STATIC
VOID
EFIAPI
NvmeInfoNotifier (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  ConsoleBootMsg =
    ((H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT *) Context)->ConsoleBootMsg;
  ConsoleBootMsg->AppendOutput (
                    ConsoleBootMsg,
                    NvmeInfoOutput,
                    Context
                    );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Handler function for displaying NVME information.

  @param[in]  Console    A pointer to the instance of Simple Text Output protocol.
  @param[in]  MaxColumn  Maximum column count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  MaxRow     Maximum row count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  Context    A pointer to the instance of platform info boot message context.

  @retval EFI_SUCCESS             Successfully display system information.
  @retval EFI_UNSUPPORTED         This display system information is disabled.
  @retval EFI_INVALID_PARAMETER   There is a problem with the Context pointer.
  @retval EFI_NOT_FOUND           The informaion protocol not reday.

**/
EFI_STATUS
EFIAPI
NvmeInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context
  )
{
  H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  *PlatformInfoBootMsgContext;
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL       *ConsoleBootMsg;
  EFI_HII_HANDLE                          HiiHandle;
  EFI_STATUS                              Status;
  EFI_EVENT                               Event;
  VOID                                    *Registration;

  UINTN                                   NumberOfControllers;
  H2O_NVME_INFO_CONTROLLER_ENTRY          *ControllerEntries;
  EFI_HANDLE                              RaidControllerHandle;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL      *NvmePassThru;
  UINTN                                   Index;
  NVME_ADMIN_CONTROLLER_DATA              *IdentifyControllerData;
  CHAR16                                  *ModelNumberString;
  CHAR16                                  *DisplayName;
  CHAR16                                  *PortName;
  UINT32                                  NumOfValidNamespaces;
  CHAR16                                  *CapacityString;
  CHAR16                                  *NsCountString;
  EFI_STRING                              SmartStatusString;
  EFI_STRING                              SmartStatusString2;

  INT32  NvmeInfoTextAttributes[] = {
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK)
  };

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  if (!FeaturePcdGet (PcdH2ONvmeInfoSupported)) {
    return EFI_UNSUPPORTED;
  }

  if (Context == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Context is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PlatformInfoBootMsgContext = (H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT *) Context;

  ConsoleBootMsg = PlatformInfoBootMsgContext->ConsoleBootMsg;
  if (ConsoleBootMsg == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The ConsoleBootMsg is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  HiiHandle = PlatformInfoBootMsgContext->HiiHandle;
  if (HiiHandle == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiHandle is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gH2ONvmeInfoProtocolGuid,
                  NULL, // No registration
                  (VOID **) &mNvmeInfo
                  );
  if (EFI_ERROR (Status)) {
    mNvmeInfo = NULL;

    PlatformInfoBootMsgContext->RecordCursorPosition = TRUE;
    PlatformInfoBootMsgContext->RecordTailCursorCol  = Console->Mode->CursorColumn;
    PlatformInfoBootMsgContext->RecordTailCursorRow  = Console->Mode->CursorRow;

    if (PlatformInfoBootMsgContext->NvmeInfoEvent == NULL) {
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      NvmeInfoNotifier,
                      Context,
                      &Event
                      );
      if (!EFI_ERROR (Status)) {
        PlatformInfoBootMsgContext->NvmeInfoEvent = Event;
      } else {
        DEBUG ((
          EFI_D_ERROR,
          "%a: Fail to create event\n",
          gEfiCallerBaseName
          ));
      }
    }
    if (PlatformInfoBootMsgContext->NvmeInfoEvent != NULL &&
        PlatformInfoBootMsgContext->NvmeInfoRegistration == NULL)
    {
      Status = gBS->RegisterProtocolNotify (
                      &gH2ONvmeInfoProtocolGuid,
                      PlatformInfoBootMsgContext->NvmeInfoEvent,
                      &Registration
                      );
      if (!EFI_ERROR (Status)) {
        PlatformInfoBootMsgContext->NvmeInfoRegistration = Registration;
      } else {
        DEBUG ((
          EFI_D_ERROR,
          "%a: Fail to register %a for event notify\n",
          gEfiCallerBaseName,
          "H2ONvmeInfoProtocol"
          ));
      }
    }

    return EFI_NOT_FOUND;
  }

  Status = mNvmeInfo->CollectControllers (
                        &NumberOfControllers,
                        &ControllerEntries
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get NVMe info, Status: %r\n", __FUNCTION__, Status));
    if (Status != EFI_NOT_FOUND) {
      return Status;
    }
  }

  PlatformInfoBootMsgContext->IsNvmeDeviceExist = TRUE;
  if (PlatformInfoBootMsgContext->RecordCursorPosition ||
      ((PlatformInfoBootMsgContext->RecordTailCursorRow != 0) &&
       ((!PlatformInfoBootMsgContext->IsSataDeviceExist) && (!PlatformInfoBootMsgContext->IsSdEmmcExist)))) {
    Console->SetCursorPosition (Console, PlatformInfoBootMsgContext->RecordTailCursorCol, PlatformInfoBootMsgContext->RecordTailCursorRow);
    PlatformInfoBootMsgContext->RecordCursorPosition = FALSE;
  }

  RaidControllerHandle = NULL;
  NvmePassThru = NULL;
  for (Index = 0; Index < NumberOfControllers; Index++) {
    if (ControllerEntries[Index].IsRaidController) {
      RaidControllerHandle = ControllerEntries[Index].Handle;
    }
    ASSERT (ControllerEntries[Index].Handle != NULL);
    if (ControllerEntries[Index].IsRaidController ||
        ControllerEntries[Index].Handle != RaidControllerHandle ||
        NvmePassThru == NULL)
    {
      Status = gBS->OpenProtocol (
                      ControllerEntries[Index].Handle,
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
          ControllerEntries[Index].Handle,
          "EfiNvmExpressPassThru",
          Status
          ));
        NvmePassThru = NULL;
        continue;
      }
    }

    Status = mNvmeInfo->IdentifyController (
                          NvmePassThru,
                          ControllerEntries[Index].NamespaceId,
                          &IdentifyControllerData
                          );
    if (!EFI_ERROR (Status)) {
      ModelNumberString = GetNvmeModelNumberFromIdentifyData (
                            IdentifyControllerData
                            );
      FreePool (IdentifyControllerData);
    } else if (ControllerEntries[Index].IsRaidController) {
      //
      // Intel VMD (Volume Management Device), which is a kind of RAID
      // controller, can fail on getting Identify Controller data of
      // Namespace ID 0.
      //
      ModelNumberString = CreateRaidControllerDeviceString (
                            ControllerEntries[Index].Handle
                            );
    } else {
      DEBUG ((
        EFI_D_ERROR,
        "%a: Fail to get NVMe Identify Controller data (protocol %p, NSID 0x%08x): %r\n",
        gEfiCallerBaseName,
        NvmePassThru,
        (unsigned int) ControllerEntries[Index].NamespaceId,
        Status
        ));
      continue;
    }
    if (ModelNumberString == NULL) {
      //
      // Out of resources.
      //
      continue;
    }

    Status = LookupNvmePortName (
               PlatformInfoBootMsgContext,
               &ControllerEntries[Index],
               NvmePassThru,
               &DisplayName
               );
    if (!EFI_ERROR (Status) && DisplayName != NULL) {
      PortName = CatSPrint (
                   NULL,
                   L"%s%s",
                   ((ControllerEntries[Index].NamespaceId > 0) ? L"- " : L""),
                   DisplayName
                   );
      FreePool (DisplayName);
    } else if (ControllerEntries[Index].NamespaceId > 0) {
      PortName = CatSPrint (
                   NULL,
                   L"- NSID %Xh",
                   (unsigned int) ControllerEntries[Index].NamespaceId
                   );
    } else {
      PortName = CatSPrint (
                   NULL,
                   L"NVMe Controller #%d",
                   (unsigned int) Index
                   );
    }

    ComputeNvmeTotalCapacity (
      mNvmeInfo,
      NvmePassThru,
      &NumOfValidNamespaces,
      &CapacityString
      );

    NsCountString = NULL;
    if (NumOfValidNamespaces > 1) {
      NsCountString = CatSPrint (
                        NULL,
                        L" (%u namespaces)",
                        (unsigned int) NumOfValidNamespaces
                        );
    }

    GetSmartStatusStrings (
      mNvmeInfo,
      NvmePassThru,
      ControllerEntries[Index].NamespaceId,
      HiiHandle,
      NULL, // Language
      &NvmeInfoTextAttributes[8],
      &SmartStatusString,
      &SmartStatusString2
      );

    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      ARRAY_SIZE (NvmeInfoTextAttributes),
                      NvmeInfoTextAttributes,
                      HiiHandle,
                      STRING_TOKEN (STR_NVME_INFORMATION_FORMAT),
                      NULL, // Language
                      ((PortName != NULL) ? PortName : L""),
                      ModelNumberString,
                      ((NsCountString != NULL) ? NsCountString : L""),
                      ((CapacityString != NULL) ? CapacityString : L""),
                      ((SmartStatusString != NULL) ? SmartStatusString : L""),
                      ((SmartStatusString2 != NULL) ? SmartStatusString2 : L"")
                      );

    FreePool (ModelNumberString);
    FreePoolIfNonNull (PortName);
    FreePoolIfNonNull (NsCountString);
    FreePoolIfNonNull (CapacityString);
    FreePoolIfNonNull (SmartStatusString);
    FreePoolIfNonNull (SmartStatusString2);
  }

  FreePoolIfNonNull (ControllerEntries);

  Console->SetAttribute (
             Console,
             (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
             );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
