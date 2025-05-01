/** @file
  Implementation of BmcInfoLib.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "Library/BmcInfoLib.h"

#include <Uefi.h>

#include <IpmiConfigForm.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IpmiGeneralCommandLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <IndustryStandard/IpmiNetFnChassis.h>

#define MAX_STRING_SIZE     1000

typedef struct {
  UINTN   Selector;
  CHAR16  *String;
} BMC_BOOT_DEVICE_SELECTOR_STRING;

STATIC CONST BMC_BOOT_DEVICE_SELECTOR_STRING  mBootDeviceSelectorString[] = {
  { IPMI_BOOT_DEVICE_SELECTOR_NO_OVERRIDE,          L"No Override"                        },
  { IPMI_BOOT_DEVICE_SELECTOR_PXE,                  L"Force Pxe"                          },
  { IPMI_BOOT_DEVICE_SELECTOR_HARDDRIVE,            L"Force Default Hard Disk"            },
  { IPMI_BOOT_DEVICE_SELECTOR_HARDDRIVE_SAFE_MODE,  L"Force Default Hard Disk Safe Mode"  },
  { IPMI_BOOT_DEVICE_SELECTOR_DIAGNOSTIC_PARTITION, L"Force Default Diagnostic Partition" },
  { IPMI_BOOT_DEVICE_SELECTOR_CD_DVD,               L"Force Default CD/DVD"               },
  { IPMI_BOOT_DEVICE_SELECTOR_BIOS_SETUP,           L"Force Setup Utility"                },
  { IPMI_BOOT_DEVICE_SELECTOR_REMOTE_FLOPPY,        L"Force Remote Removable Media"       },
  { IPMI_BOOT_DEVICE_SELECTOR_REMOTE_CD_DVD,        L"Force Remote CD/DVD"                },
  { IPMI_BOOT_DEVICE_SELECTOR_PRIMARY_REMOTE_MEDIA, L"Force Primary Remote Media"         },
  { IPMI_BOOT_DEVICE_SELECTOR_REMOTE_HARDDRIVE,     L"Force Remote Hard Disk"             },
  { IPMI_BOOT_DEVICE_SELECTOR_FLOPPY,               L"Force Removable Media"              },
};

/**
  Get Boot Device Selector string by selector.

  @param[in]    Selector  Boot Device Selector.
  @param[out]   String    The pointer to Boot Device Selector string.
                          Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetBootDeviceSelectorStringBySelector (
  IN  UINTN   Selector,
  OUT CHAR16  **String
  )
{
  UINTN   Index;

  for (Index = 0; Index < ARRAY_SIZE (mBootDeviceSelectorString); Index++) {
    if (mBootDeviceSelectorString[Index].Selector == Selector) {
      *String = (CHAR16 *) AllocateCopyPool (
                             StrSize (mBootDeviceSelectorString[Index].String),
                             mBootDeviceSelectorString[Index].String
                             );
      if (*String == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Get Boot Option string.

  @param[out]   BootOptionStr   The pointer to Boot Option string.
                                Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetBootOptionStr (
  OUT CHAR16  **BootOptionStr
  )
{
  EFI_STATUS           Status;
  IPMI_BOOT_OPTIONS_RESPONSE_PARAMETER_5  BootFlags;
  UINT8                                   CompletionCode;

  Status = IpmiLibGetSystemBootFlags (&BootFlags, FALSE, &CompletionCode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to get System Boot Flags, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  return GetBootDeviceSelectorStringBySelector (BootFlags.Data2.Bits.BootDeviceSelector, BootOptionStr);
}

/**
  Get Current Boot Mode string.

  @param[out]   CurrentBootModeStr  The pointer to Current Boot Mode string.
                                    Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetCurrentBootModeStr (
  OUT CHAR16  **CurrentBootModeStr
  )
{
  EFI_STATUS                              Status;
  IPMI_BOOT_OPTIONS_RESPONSE_PARAMETER_5  BootFlags;
  UINT8                                   CompletionCode;
  CHAR16                                  *StringPtr;

  if (CurrentBootModeStr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = IpmiLibGetSystemBootFlags (&BootFlags, FALSE, &CompletionCode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to get System Boot Flags, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (BootFlags.Data1.Bits.BiosBootType) {
    StringPtr = L"EFI";
  } else {
    StringPtr = L"Legacy";
  }

  *CurrentBootModeStr = (CHAR16 *) AllocateCopyPool (StrSize (StringPtr), StringPtr);
  if (*CurrentBootModeStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Get IPv4 Address strings.

  @param[in]    ChannelNumber   The channel number.
  @param[out]   IPv4AddressStr  The pointer to IPv4 Address string.
                                Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetIPv4AddressStr (
  IN  UINT8   ChannelNumber,
  OUT CHAR16  **IPv4AddressStr
  )
{
  EFI_STATUS            Status;
  IPMI_LAN_IP_ADDRESS   IPv4Address;
  UINT8                 CompletionCode;
  CHAR16                IPv4AddressStrBuffer[MAX_STRING_SIZE];
  CHAR16                *TempIPv4AddressStr;

  if (IPv4AddressStr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = IpmiLibGetIpv4Address (ChannelNumber, &IPv4Address, FALSE, &CompletionCode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to get IPv4 Address, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  UnicodeSPrint (
    IPv4AddressStrBuffer,
    sizeof (IPv4AddressStrBuffer),
    L"%d.%d.%d.%d",
    IPv4Address.IpAddress[0],
    IPv4Address.IpAddress[1],
    IPv4Address.IpAddress[2],
    IPv4Address.IpAddress[3]
    );

  TempIPv4AddressStr = (CHAR16 *) AllocateCopyPool (
                                    StrnSizeS (IPv4AddressStrBuffer, sizeof (IPv4AddressStrBuffer)),
                                    IPv4AddressStrBuffer
                                    );
  if (TempIPv4AddressStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *IPv4AddressStr = TempIPv4AddressStr;

  return EFI_SUCCESS;
}

/**
  Convert EFI IPv6 Address to a NULL-terminated unicode string.
  The text representation of IPv6 address is defined in RFC 4291.

  @param[in]  Ipv6Address     The pointer to the EFI IPv6 Address.
  @param[out] IPv6AddressStr  A pointer to the buffer to return the text representation
                              of IPv6 Address. Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
STATIC
IPv6AddressToStr (
  IN  EFI_IPv6_ADDRESS  *IPv6Address,
  OUT CHAR16            **IPv6AddressStr
  )
{
  union {
    EFI_IPv6_ADDRESS  EfiIPv6Address;
    UINT16            Word[8];
  } Address;

  UINTN   Index;
  UINTN   Index2;
  UINTN   ZeroSpanStart;
  UINTN   ZeroSpanEnd;
  UINTN   LongestZeroSpan;
  UINTN   LongestZeroSpanStart;
  UINTN   LongestZeroSpanEnd;

  CHAR16  Result[MAX_STRING_SIZE];
  CHAR16  *Ptr;
  CHAR16  WordStr[5];
  UINTN   NumberOfCharacters;

  if (IPv6Address == NULL || IPv6AddressStr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  LongestZeroSpanStart = 8;
  LongestZeroSpanEnd   = 8;

  CopyMem (&Address.EfiIPv6Address, IPv6Address, sizeof (EFI_IPv6_ADDRESS));
  for (Index = 0; Index < 8; Index++) {
    Address.Word[Index] = SwapBytes16 (Address.Word[Index]);
  }

  //
  // Find longest zero span
  //
  for (Index = 0, LongestZeroSpan = 0; Index < 8; Index++) {
    if (Address.Word[Index] != 0) {
      continue;
    }
    ZeroSpanStart = Index;
    ZeroSpanEnd   = ZeroSpanStart;
    for (Index2 = Index + 1; Index2 < 8 && Address.Word[Index2] == 0; Index2++);
    ZeroSpanEnd   = --Index2;
    if (ZeroSpanEnd - ZeroSpanStart + 1 > LongestZeroSpan) {
      LongestZeroSpan       = ZeroSpanEnd - ZeroSpanStart + 1;
      LongestZeroSpanStart  = ZeroSpanStart;
      LongestZeroSpanEnd    = ZeroSpanEnd;
    }
  }

  //
  // Build IPv6 Address String
  //
  ZeroMem (Result, sizeof (Result));

  for (Index = 0, Ptr = Result; Index < 8; Index++) {
    if (Index == LongestZeroSpanStart) {
      *Ptr++ = L':';
      if (Index == 0) {
        *Ptr++ = L':';
      }
      Index = LongestZeroSpanEnd;
      continue;
    }
    ZeroMem (WordStr, sizeof (WordStr));
    NumberOfCharacters = UnicodeSPrint (WordStr, sizeof (WordStr), L"%x", Address.Word[Index]);
    CopyMem (Ptr, WordStr, NumberOfCharacters * sizeof (CHAR16));
    Ptr += NumberOfCharacters;
    if (Index + 1 != 8) {
      *Ptr++ = L':';
    }
  }

  *IPv6AddressStr = (CHAR16 *) AllocateCopyPool (StrnSizeS (Result, sizeof (Result)), Result);
  if (*IPv6AddressStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Get IPv6 Address strings.

  @param[in]    ChannelNumber           The channel number.
  @param[out]   NumberOfIPv6AddressStr  Number of IPv6 Address strings.
  @param[out]   IPv6AddressStrBuffer    The pointer to buffer of IPv6 Address strings.
                                        Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetIPv6AddressStrBuffer (
  IN  UINT8   ChannelNumber,
  OUT UINTN   *NumberOfIPv6AddressStr,
  OUT CHAR16  ***IPv6AddressStrBuffer
  )
{
  EFI_STATUS                    Status;
  UINT8                         NumberOfStaticIPv6Address;
  UINT8                         NumberOfDynamicIPv6Address;
  UINT8                         SLAACSupported;
  UINT8                         DHCPv6Supported;
  UINT8                         CompletionCode;
  UINTN                         TempNumberOfIPv6AddressStr;
  CHAR16                        **TempIPv6AddressStrBuffer;
  UINTN                         Index;
  IPMI_LAN_IPV6_STATIC_ADDRESS  IPv6Address;
  EFI_IPv6_ADDRESS              EfiIPv6Address;
  CHAR16                        *IPv6AddressStr;
  CHAR16                        StringBuffer[MAX_STRING_SIZE];

  if (NumberOfIPv6AddressStr == NULL || IPv6AddressStrBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberOfIPv6AddressStr     = 0;
  *IPv6AddressStrBuffer       = NULL;

  NumberOfStaticIPv6Address   = 0;
  NumberOfDynamicIPv6Address  = 0;

  Status = IpmiLibGetIpv6SourceNumber (
             ChannelNumber,
             FALSE,
             &NumberOfStaticIPv6Address,
             &NumberOfDynamicIPv6Address,
             &SLAACSupported,
             &DHCPv6Supported,
             &CompletionCode
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to get number of IPv6 Source, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  TempNumberOfIPv6AddressStr  = NumberOfStaticIPv6Address + NumberOfDynamicIPv6Address;
  TempIPv6AddressStrBuffer    = (CHAR16 **) AllocateZeroPool (TempNumberOfIPv6AddressStr * sizeof (CHAR16 *));
  if (TempIPv6AddressStrBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < TempNumberOfIPv6AddressStr; Index++) {
    Status = IpmiLibGetIpv6Address (
               ChannelNumber,
               (UINT8) ((Index < NumberOfStaticIPv6Address)? Index : Index - NumberOfStaticIPv6Address),
               (Index < NumberOfStaticIPv6Address),
               FALSE,
               &IPv6Address,
               &CompletionCode
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_INFO,
        "%a() - Fail to get %a IPv6 Address (Selector = %d), Status: %r\n",
        __FUNCTION__,
        (Index < NumberOfStaticIPv6Address)? "Static" : "Dynamic",
        (UINT8) ((Index < NumberOfStaticIPv6Address)? Index : Index - NumberOfStaticIPv6Address),
        Status
        ));
      continue;
    }

    CopyMem (&EfiIPv6Address, &IPv6Address.Ipv6Address, sizeof (IPv6Address.Ipv6Address));
    Status = IPv6AddressToStr (&EfiIPv6Address, &IPv6AddressStr);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "%a() - Fail to convert IPv6 Address to string, Status: %r\n", __FUNCTION__, Index, Status));
      continue;
    }

    ZeroMem (StringBuffer, sizeof (StringBuffer));
    UnicodeSPrint (
      StringBuffer,
      sizeof (StringBuffer),
      (Index < NumberOfStaticIPv6Address)? L"%s(%d)  %s" : L"%s(%d) %s",
      (Index < NumberOfStaticIPv6Address)? L"Static" : L"Dynamic",
      (Index < NumberOfStaticIPv6Address)? Index : Index - NumberOfStaticIPv6Address,
      IPv6AddressStr
      );

    TempIPv6AddressStrBuffer[Index] = (CHAR16 *) AllocateCopyPool (
                                                   StrnSizeS (StringBuffer, sizeof (StringBuffer)),
                                                   StringBuffer
                                                   );
    if (TempIPv6AddressStrBuffer[Index] == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    FreePool (IPv6AddressStr);
  }

  *NumberOfIPv6AddressStr = TempNumberOfIPv6AddressStr;
  *IPv6AddressStrBuffer   = TempIPv6AddressStrBuffer;

  return EFI_SUCCESS;
}