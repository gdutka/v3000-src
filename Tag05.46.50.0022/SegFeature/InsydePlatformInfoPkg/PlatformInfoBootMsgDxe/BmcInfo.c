/** @file


;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <H2OIpmi.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg

#include <Library/BaseMemoryLib.h>
#include <Library/BmcInfoLib.h>               // InsydePlatformInfoPkg
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>                   // MdeModulePkg
#include <Library/IpmiGeneralCommandLib.h>    // InsydeIpmiPkg
#include <Library/IpmiOemFeatureLib.h>        // InsydeIpmiPkg
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "PlatformInfoBootMsgDxe.h"

#define MAX_VALID_CHANNEL_NUMBER  15

#define INSYDE_SUPERVYSE_COMPLETION_CODE_INFO_INDEX_NOT_VALID   0x80

#pragma pack(1)
typedef struct {
  UINT8   ChannelNumber;
  CHAR16  *DisplayName;
} DISPLAY_NAME_FOR_CHANNEL_MAPPING_TABLE_ENTRY;

typedef struct {
  UINT8   CompletionCode;
  UINT8   NicDescription;
  UINT8   BmcIpmiChannelNumber;
  UINT16  NcsiPackage;
  UINT8   LinkStatus;
  UINT8   HostMacAddress[6];
  UINT8   BmcMacAddress[6];
} INSYDE_SUPERVYSE_3PX_GET_NIC_INFO_RESPONSE;
#pragma pack()

typedef enum {
  DedicatedToBmc = 0,
  SharedWithHost,
  DedicatedToHost,
  NicDescriptionMax
} INSYDE_SUPERVYSE_3PX_NIC_DESCRIPTION;

STATIC DISPLAY_NAME_FOR_CHANNEL_MAPPING_TABLE_ENTRY *mDisplayNameForChannel      = NULL;
STATIC UINTN                                        mDisplayNameForChannelCount = 0;

/**
  Helper function to parse PcdH2OBmcInfoDisplayNameForChannel

**/
VOID
BuildDisplayNameForChannelMappingTable (
  VOID
  )
{
  UINT8                                         *PcdData;
  UINTN                                         PcdDataSize;
  UINT8                                         *DataPtr;
  DISPLAY_NAME_FOR_CHANNEL_MAPPING_TABLE_ENTRY  *Entry;
  UINTN                                         Count;

  PcdData     = PcdGetPtr (PcdH2OBmcInfoDisplayNameForChannel);
  PcdDataSize = PcdGetSize (PcdH2OBmcInfoDisplayNameForChannel);

  if (PcdDataSize < 4) {
    DEBUG ((EFI_D_ERROR, "PcdH2OBmcInfoDisplayNameForChannel does not match a correct data format.\n"));
    return;
  }

  DataPtr = PcdData;
  Count   = 0;
  while (DataPtr < PcdData + PcdDataSize) {
    DataPtr++;
    DataPtr += StrSize ((CHAR16 *) DataPtr);
    Count++;
  }

  mDisplayNameForChannel = AllocateZeroPool (sizeof (DISPLAY_NAME_FOR_CHANNEL_MAPPING_TABLE_ENTRY) * Count);
  if (mDisplayNameForChannel == NULL) {
    return;
  }

  DataPtr = PcdData;
  Count   = 0;
  while (DataPtr < PcdData + PcdDataSize) {
    Entry = (DISPLAY_NAME_FOR_CHANNEL_MAPPING_TABLE_ENTRY *) DataPtr;
    mDisplayNameForChannel[Count].ChannelNumber = Entry->ChannelNumber;
    DataPtr++;
    mDisplayNameForChannel[Count].DisplayName = AllocateCopyPool (
                                                  StrSize ((CHAR16 *) &Entry->DisplayName),
                                                  (VOID *) &Entry->DisplayName
                                                  );
    DataPtr += StrSize ((CHAR16 *) &Entry->DisplayName);
    Count++;
  }

  mDisplayNameForChannelCount = Count;
}

/**
  Helper function to get the display name for the channel.

  @param[in]  ChannelNumber       The Channel Number.
  @param[out] DisplayName         The pointer to the DisplayName. It is the caller's
                                  responsibility to free this buffer.

  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval EFI_NOT_FOUND           The display name is not found.
  @retval EFI_OUT_OF_RESOURCES    Out of resources.
  @retval EFI_SUCCESS             Successfully get the display name.

**/
EFI_STATUS
GetDisplayNameForChannel (
  IN  UINT8   ChannelNumber,
  OUT CHAR16  **DisplayName
  )
{
  UINTN       Index;
  EFI_STATUS  Status;

  if (DisplayName == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_NOT_FOUND;
  for (Index = 0; Index < mDisplayNameForChannelCount; Index++) {
    if (mDisplayNameForChannel[Index].ChannelNumber == ChannelNumber &&
        mDisplayNameForChannel[Index].DisplayName != NULL) {
      *DisplayName = AllocateCopyPool (
                       StrSize (mDisplayNameForChannel[Index].DisplayName),
                       mDisplayNameForChannel[Index].DisplayName
                       );
      if (*DisplayName == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
      }
      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}

/**
  Helper function to get the NIC Description of an NIC whose MAC Address is same as
  the LAN Channel specified by channel number.

  @param[in]  ChannelNumber       The Channel Number.

  @retval NicDescriptionMax   Cannot get the NIC Description.
  @retval Others              Successfully get the NIC Description.

**/
UINT8
InternalGetNicDescription (
  IN  UINT8   ChannelNumber
  )
{
  UINT8                                       Index;
  INSYDE_SUPERVYSE_3PX_GET_NIC_INFO_RESPONSE  Resp;
  EFI_STATUS                                  Status;

  Index = 0;
  do {
    ZeroMem ((VOID *) &Resp, sizeof (INSYDE_SUPERVYSE_3PX_GET_NIC_INFO_RESPONSE));
    Status = IpmiBmcFeatureLibGetNicInfo (Index, &Resp);
    if (!EFI_ERROR (Status)) {
      if (Resp.BmcIpmiChannelNumber == ChannelNumber) {
        return Resp.NicDescription;
      }
    }
    Index++;
  } while ((Resp.CompletionCode != INSYDE_SUPERVYSE_COMPLETION_CODE_INFO_INDEX_NOT_VALID));

  return NicDescriptionMax;
}

/**
  Handler function for displaying BMC information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
BmcInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *BootOptionValue;
  CHAR16                          *CurrentBootModeValue;
  CHAR16                          *IPv4AddressValue;
  UINTN                           NumberOfIPv6AddressStr;
  CHAR16                          **IPv6AddressStrBuffer;
  UINT8                           ChannelNumber;
  IPMI_GET_CHANNEL_INFO_RESPONSE  ChannelInfo;
  CHAR16                          *DisplayName;
  UINT8                           NicDescription;
  UINTN                           Index;
  INT32                           LastLineAttribute;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  NicDescription = NicDescriptionMax;

  if (PcdGet8 (PcdH2OIpmiEnable) == H2O_IPMI_FUNCTION_DISABLE) {
    DEBUG ((EFI_D_INFO, "%a() - The IPMI is disable in setup.\n", __FUNCTION__));
    return;
  }

  Status = GetBootOptionStr (&BootOptionValue);
  if (!EFI_ERROR (Status)) {
    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_BMC_BOOT_OPTION_FORMAT),
                      NULL, // Language
                      BootOptionValue
                      );
    FreePool (BootOptionValue);
  }

  Status = GetCurrentBootModeStr (&CurrentBootModeValue);
  if (!EFI_ERROR (Status)) {
    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_BMC_CURRENT_BOOT_MODE_FORMAT),
                      NULL, // Language
                      CurrentBootModeValue
                      );
    FreePool (CurrentBootModeValue);
  }

  BuildDisplayNameForChannelMappingTable ();

  for (ChannelNumber = 0; ChannelNumber <= MAX_VALID_CHANNEL_NUMBER; ChannelNumber++) {
    Status = IpmiLibGetChannelInfo (ChannelNumber, &ChannelInfo, FALSE);
    if (EFI_ERROR (Status) || ChannelInfo.MediumType.Bits.ChannelMediumType != IPMI_CHANNEL_MEDIA_TYPE_802_3_LAN) {
      continue;
    }

    DisplayName = NULL;
    Status = GetDisplayNameForChannel (ChannelNumber, &DisplayName);
    if (!EFI_ERROR (Status) && DisplayName != NULL) {
      ConsoleBootMsg->ConsolePrintHiiString (
                        Console,
                        MaxColumn,
                        MaxRow,
                        0,    // AttributeCount
                        NULL, // Attributes
                        HiiHandle,
                        STRING_TOKEN (STR_BMC_LAN_CHANNEL_OEM_FORMAT),
                        NULL, // Language
                        DisplayName
                        );
      FreePool (DisplayName);
    } else {
      if (PcdGetBool (PcdH2OIpmiSupervyseExist)) {
        NicDescription = InternalGetNicDescription (ChannelNumber);
        if (NicDescription == DedicatedToHost) {
          NicDescription = NicDescriptionMax;
        }
        if (NicDescription != NicDescriptionMax) {
          ConsoleBootMsg->ConsolePrintHiiString (
                            Console,
                            MaxColumn,
                            MaxRow,
                            0,    // AttributeCount
                            NULL, // Attributes
                            HiiHandle,
                            STRING_TOKEN (STR_BMC_LAN_CHANNEL_FORMAT2),
                            NULL, // Language
                            (NicDescription == DedicatedToBmc)? L"Dedicated" : L"Shared"
                            );
        }
      }
      if ((PcdGetBool (PcdH2OIpmiSupervyseExist) && NicDescription == NicDescriptionMax) ||
          !PcdGetBool (PcdH2OIpmiSupervyseExist)) {
        ConsoleBootMsg->ConsolePrintHiiString (
                          Console,
                          MaxColumn,
                          MaxRow,
                          0,    // AttributeCount
                          NULL, // Attributes
                          HiiHandle,
                          STRING_TOKEN (STR_BMC_LAN_CHANNEL_FORMAT),
                          NULL, // Language
                          ChannelNumber
                          );
      }
    }

    Status = GetIPv4AddressStr (ChannelNumber, &IPv4AddressValue);
    if (!EFI_ERROR (Status)) {
      ConsoleBootMsg->ConsolePrintHiiString (
                        Console,
                        MaxColumn,
                        MaxRow,
                        0,    // AttributeCount
                        NULL, // Attributes
                        HiiHandle,
                        STRING_TOKEN (STR_BMC_IPV4_ADDRESS_FORMAT),
                        NULL, // Language
                        IPv4AddressValue
                        );
      FreePool (IPv4AddressValue);
    }

    Status = GetIPv6AddressStrBuffer (ChannelNumber, &NumberOfIPv6AddressStr, &IPv6AddressStrBuffer);
    if (!EFI_ERROR (Status) && NumberOfIPv6AddressStr > 0) {
      ConsoleBootMsg->ConsolePrintHiiString (
                        Console,
                        MaxColumn,
                        MaxRow,
                        0,    // AttributeCount
                        NULL, // Attributes
                        HiiHandle,
                        STRING_TOKEN (STR_BMC_IPV6_ADDRESSES_FORMAT),
                        NULL, // Language
                        IPv6AddressStrBuffer[0]
                        );
      for (Index = 1; Index < NumberOfIPv6AddressStr; Index++) {
          ConsoleBootMsg->ConsolePrintHiiString (
                            Console,
                            MaxColumn,
                            MaxRow,
                            0,    // AttributeCount
                            NULL, // Attributes
                            HiiHandle,
                            STRING_TOKEN (STR_BMC_IPV6_ADDRESSES_CONT_FORMAT),
                            NULL, // Language
                            IPv6AddressStrBuffer[Index]
                            );
      }

      for (Index = 0; Index < NumberOfIPv6AddressStr; Index++) {
        if (IPv6AddressStrBuffer[Index] != NULL) {
          FreePool (IPv6AddressStrBuffer[Index]);
        }
      }
      FreePool (IPv6AddressStrBuffer);
    }
  }

  LastLineAttribute = (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
  ConsoleBootMsg->ConsolePrint (
                    Console,
                    MaxColumn,
                    MaxRow,
                    1, // AttributeCount
                    &LastLineAttribute,
                    L"\n"
                    );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}