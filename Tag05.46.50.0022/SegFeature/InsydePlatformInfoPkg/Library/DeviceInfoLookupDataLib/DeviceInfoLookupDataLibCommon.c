/** @file
  Routine for collecting data for Device Info Lookup Protocol.

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
#include <Protocol/H2ODeviceInfoLookup.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/DeviceInfoLookupDataLib.h>  // InsydePlatformInfoPkg
#include <Library/UefiLib.h>

#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2
#include <Library/DeviceInfo2Lib.h> // InsydeModulePkg
#endif
#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO
#include <Library/DeviceInfoLib.h>  // InsydeModulePkg
#endif

#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2
extern EFI_GUID gH2ODeviceInfo2TokenSpaceGuid; // AutoGen.c (PCD token space)
#endif
#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO
extern EFI_GUID gH2ODeviceInfoTokenSpaceGuid;  // AutoGen.c (PCD token space)
#endif

#if defined (DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2) || defined (DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO)

/**

  @retval NULL
  @retval Other

**/
STATIC
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

STATIC
BOOLEAN
ShouldAddLookupEntry (
  IN H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES  AttributeFlags
  )
{
  if (FeaturePcdGet (PcdH2OSetupUsbViewerSupported) &&
      AttributeFlags.Bits.IsUsb)
  {
    return TRUE;
  }
  if (FeaturePcdGet (PcdH2OSataDriveInfoSupported) &&
      AttributeFlags.Bits.IsSata)
  {
    return TRUE;
  }
  if (FeaturePcdGet (PcdH2ONvmeInfoSupported) &&
      AttributeFlags.Bits.IsNvme)
  {
    return TRUE;
  }
  if (FeaturePcdGet (PcdH2OPciDeviceInfoSupported) &&
      AttributeFlags.Bits.IsPci)
  {
    return TRUE;
  }
  if (FeaturePcdGet (PcdH2OSDEmmcInfoSupported) &&
      AttributeFlags.Bits.IsSdEmmc)
  {
    return TRUE;
  }
  return FALSE;
}

STATIC
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
  Create a display name string based on the target Device Info attribute's value.

  If the attribute value is a string token name, the associated HII string is
  returned. Otherwise, the attribute value itself is used as the display name.

  @param  AttribValue           The value of the display name Device Info
                                attribute.
  @param  DeviceInfoLookup      Device Info Lookup protocol instance.
  @param  StringTokenNameToIds  Table from AutoGen.c that maps string token
                                names to their IDs.
  @param  HiiHandle             HII handle that registered string packages.

  @retval NULL                  The display name is not returned because an
                                error occurs.
  @retval Other                 Unicode display name string that should be freed
                                by the caller.

**/
STATIC
CHAR16 *
GetDeviceInfoDisplayName (
  IN CONST CHAR8                        *AttribValue,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup,
  IN PCD_STRING_NAME_TO_ID              *StringTokenNameToIds,
  IN EFI_HII_HANDLE                     HiiHandle
  )
{
  EFI_STATUS                            Status;
  CHAR8                                 *TokenName;
  EFI_STRING_ID                         StringId;
  CHAR8                                 *DisplayNameAscii;
  CHAR16                                *DisplayName;

  if (AttribValue == NULL || DeviceInfoLookup == NULL || StringTokenNameToIds == NULL) {
    return NULL;
  }

  DisplayName = NULL;

  //
  // Try to retrieve the HII string.
  //
  TokenName = AllocateCopyPool (AsciiStrSize (AttribValue), AttribValue);
  if (TokenName != NULL) {
    Status = HiiGetStringIdByName (StringTokenNameToIds, TokenName, &StringId);
    if (!EFI_ERROR (Status)) {
      DisplayName = HiiGetString (HiiHandle, StringId, NULL);
    }
  }

  if (TokenName != NULL) {
    FreePool (TokenName);
  }

  if (DisplayName != NULL) {
    return DisplayName;
  }

  //
  // The attribute value itself is used as the display name.
  //
  DisplayNameAscii = DeviceInfoLookup->CreateDisplayNameString (AttribValue);
  if (DisplayNameAscii != NULL) {
    DisplayName = CatSPrint (NULL, L"%a", DisplayNameAscii);
  }

  if (DisplayNameAscii != NULL) {
    FreePool (DisplayNameAscii);
  }

  return DisplayName;
}

EFI_STATUS
EFIAPI
LoadDeviceInfoLookupDataInternal (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *Protocol,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN PCD_STRING_NAME_TO_ID                *StringTokenNameToIds
  )
{
  CONST GUID                         *TokenSpaceGuid;
  CONST CHAR8                        *TokenSpaceCName;
  UINTN                              TokenNumber;
  EFI_STATUS                         Status;
  CONST EFI_DEVICE_PATH_PROTOCOL     *DevicePath;  // PCD database internal
  UINT32                             DeviceAttribCount;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES  AttributeFlags;
  CHAR8                              *AcpiPath;
  CHAR16                             *DisplayName;
  CONST CHAR8                        *NameAttributeName;
  UINT32                             AttribIndex;
  CONST CHAR8                        *AttribName;  // PCD database internal
  CONST CHAR8                        *AttribValue; // PCD database internal

  if (Protocol == NULL || StringTokenNameToIds == NULL) {
    return EFI_INVALID_PARAMETER;
  }

#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2
  TokenSpaceGuid  = (CONST GUID *) &gH2ODeviceInfo2TokenSpaceGuid;
  TokenSpaceCName = "gH2ODeviceInfo2TokenSpaceGuid";
#else
  TokenSpaceGuid  = (CONST GUID *) &gH2ODeviceInfoTokenSpaceGuid;
  TokenSpaceCName = "gH2ODeviceInfoTokenSpaceGuid";
#endif

  TokenNumber = 0;
  while (TRUE) {
    TokenNumber = LibPcdGetNextToken (
                    TokenSpaceGuid,
                    TokenNumber
                    );
    if (TokenNumber == 0) {
      break;
    }
    ASSERT (TokenNumber <= MAX_UINT32);
#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2
    Status = H2OGetDeviceInfo (
               TokenNumber,
               (EFI_DEVICE_PATH_PROTOCOL **) &DevicePath,
               &DeviceAttribCount
               );
#else
    Status = GetDeviceInfo (
               TokenNumber,
               (EFI_DEVICE_PATH_PROTOCOL **) &DevicePath,
               &DeviceAttribCount
               );
#endif
    if (EFI_ERROR (Status)) {
      //
      // EFI_NOT_FOUND can happen with an empty PCD token, which is harmless.
      //
      DEBUG ((
        ((Status == EFI_NOT_FOUND) ? EFI_D_INFO : EFI_D_ERROR),
        "H2OGetDeviceInfo (%a.0x%08X): %r\n",
        TokenSpaceCName,
        (unsigned int) TokenNumber,
        Status
        ));
      continue;
    }

    //
    // Parse attribute list of the PCD token.
    //
    AttributeFlags.Uint8 = 0;
    AcpiPath             = NULL;
    DisplayName          = NULL;
    NameAttributeName    = PcdGetPtr (PcdH2ODeviceInfoAttributeStr);
    for (AttribIndex = 0; AttribIndex < DeviceAttribCount; AttribIndex++) {
#ifdef DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2
      Status = H2OGetDeviceAttrib (
                 TokenNumber,
                 AttribIndex,
                 (CHAR8 **) &AttribName,
                 (CHAR8 **) &AttribValue
                 );
#else
      Status = GetDeviceAttrib (
                 TokenNumber,
                 AttribIndex,
                 (CHAR8 **) &AttribName,
                 (CHAR8 **) &AttribValue
                 );
#endif
      if (EFI_ERROR (Status)) {
        DEBUG ((
          EFI_D_ERROR,
          "H2OGetDeviceAttrib (%a.0x%08X, %u): %r\n",
          TokenSpaceCName,
          (unsigned int) TokenNumber,
          (unsigned int) AttribIndex,
          Status
          ));
        break;
      }

      if (AsciiStriCmp (AttribName, "type") == 0) {
        if (AsciiStriCmp (AttribValue, "usb") == 0 ||
            AsciiStriCmp (AttribValue, "usb3") == 0)
        {
          AttributeFlags.Bits.IsUsb = 1;
        }
        if (AsciiStriCmp (AttribValue, "sata") == 0 ||
            AsciiStriCmp (AttribValue, "ahci") == 0)
        {
          AttributeFlags.Bits.IsSata = 1;
        }
        if (AsciiStriCmp (AttribValue, "nvme") == 0) {
          AttributeFlags.Bits.IsNvme = 1;
        }
        if (AsciiStriCmp (AttribValue, "pci") == 0 ||
            AsciiStriCmp (AttribValue, "pcie") == 0) {
          AttributeFlags.Bits.IsPci = 1;
        }
        if (AsciiStriCmp (AttribValue, "sd") == 0 ||
            AsciiStriCmp (AttribValue, "emmc") == 0) {
          AttributeFlags.Bits.IsSdEmmc = 1;
        }
        continue;
      }
      //
      // We follow H2OGetDeviceAttribByName() behavior: If there are 2 or more
      // attributes of the same name, return the first one.
      //
      if (AsciiStriCmp (AttribName, "acpipath") == 0) {
        if (AcpiPath == NULL) {
          AcpiPath = AsciiStrDuplicate (AttribValue);
          if (AcpiPath == NULL) {
            Status = EFI_OUT_OF_RESOURCES;
            goto Cleanup;
          }
        } else {
          DEBUG ((
            EFI_D_WARN,
            "%a.0x%08X: \"%a\" attribute of index %u is ignored.\n",
            TokenSpaceCName,
            (unsigned int) TokenNumber,
            "acpipath",
            (unsigned int) AttribIndex
            ));
        }
        continue;
      }
      if (AsciiStriCmp (AttribName, NameAttributeName) == 0) {
        if (DisplayName == NULL) {
          DisplayName = GetDeviceInfoDisplayName (
                          AttribValue,
                          Protocol,
                          StringTokenNameToIds,
                          HiiHandle
                          );
          if (DisplayName == NULL) {
            Status = EFI_OUT_OF_RESOURCES;
            goto Cleanup;
          }
        } else {
          DEBUG ((
            EFI_D_WARN,
            "%a.0x%08X: \"%a\" attribute of index %u is ignored.\n",
            TokenSpaceCName,
            (unsigned int) TokenNumber,
            NameAttributeName,
            (unsigned int) AttribIndex
            ));
        }
        continue;
      }
    }

    //
    // Attribute flags
    //
    if (!AttributeFlags.Bits.IsUsb) {
      Status = FindDevicePathNodeByType (
                 DevicePath,
                 MESSAGING_DEVICE_PATH,
                 MSG_USB_DP,
                 (UINT16) sizeof (USB_DEVICE_PATH),
                 NULL // Offset
                 );
      if (!EFI_ERROR (Status)) {
        AttributeFlags.Bits.IsUsb = 1;
      }
    }
    if (!AttributeFlags.Bits.IsSata) {
      Status = FindDevicePathNodeByType (
                 DevicePath,
                 MESSAGING_DEVICE_PATH,
                 MSG_SATA_DP,
                 (UINT16) sizeof (SATA_DEVICE_PATH),
                 NULL // Offset
                 );
      if (!EFI_ERROR (Status)) {
        AttributeFlags.Bits.IsSata = 1;
      }
    }
    if (!AttributeFlags.Bits.IsNvme) {
      Status = FindDevicePathNodeByType (
                 DevicePath,
                 MESSAGING_DEVICE_PATH,
                 MSG_NVME_NAMESPACE_DP,
                 (UINT16) sizeof (NVME_NAMESPACE_DEVICE_PATH),
                 NULL // Offset
                 );
      if (!EFI_ERROR (Status)) {
        AttributeFlags.Bits.IsNvme = 1;
      }
    }
    //
    // When the type parameter is not filled in as pci/pcie, the flag is automatically set according to the device path.
    //
    if (AttributeFlags.Uint8 == 0) {
      Status = FindDevicePathNodeByType (
                 DevicePath,
                 HARDWARE_DEVICE_PATH,
                 HW_PCI_DP,
                 (UINT16) sizeof (PCI_DEVICE_PATH),
                 NULL // Offset
                 );
      if (!EFI_ERROR (Status)) {
        AttributeFlags.Bits.IsPci = 1;
      }
    }

    Status = EFI_SUCCESS;
    if (ShouldAddLookupEntry (AttributeFlags) && (DisplayName != NULL)) {
      Status = Protocol->AddDeviceInfoLookupEntry (
                           Protocol,
                           DevicePath,
                           (UINT32) TokenNumber,
                           AttributeFlags.Uint8,
                           AcpiPath,
                           DisplayName,
                           NULL // DisplayNameSortKey
                           );
    }

Cleanup:
    FreePoolIfNonNull (AcpiPath);
    FreePoolIfNonNull (DisplayName);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a.0x%08X: %r\n",
        TokenSpaceCName,
        (unsigned int) TokenNumber,
        Status
        ));
      if (Status == EFI_OUT_OF_RESOURCES) {
        return EFI_OUT_OF_RESOURCES;
      }
    }
  }

  return EFI_SUCCESS;
}

#endif // DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO_2 || DEVICE_INFO_LOOKUP_DATA_FROM_DEVICE_INFO
