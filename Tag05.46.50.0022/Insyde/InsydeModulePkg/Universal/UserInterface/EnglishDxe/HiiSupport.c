/** @file
 HII support functions for Unicode Collation

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#include "UnicodeCollationEng.h"
#include "HiiSupport.h"
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiFirmwareVolume.h>
#include <Protocol/FontDatabase.h>
#include <Protocol/FirmwareVolume2.h>
#include <Guid/H2OHiiFontFile.h>

#define ISO_639_2_ENTRY_SIZE    3

STATIC EFI_GUID             mPackageListGuid = {0xf5f219d3, 0x7006, 0x4648, {0xac, 0x8d, 0xd6, 0x1d, 0xfb, 0x7b, 0xc6, 0xad}};

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  UINT32                         Reserved;
  UINT64                         UniqueId;
} HII_VENDOR_DEVICE_PATH_NODE;
#pragma pack()

typedef struct {
  HII_VENDOR_DEVICE_PATH_NODE    Node;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_TEMP_DEVICE_PATH;

#define EFI_IFR_TIANO_GUID \
  { 0xf0b1735, 0x87a0, 0x4193, 0xb2, 0x66, 0x53, 0x8c, 0x38, 0xaf, 0x48, 0xce }

//
// Hii vendor device path template
//
HII_TEMP_DEVICE_PATH  mHiiVendorDevicePathTemplate = {
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        (UINT8) (sizeof (HII_VENDOR_DEVICE_PATH_NODE)),
        (UINT8) ((sizeof (HII_VENDOR_DEVICE_PATH_NODE)) >> 8)
      },
      EFI_IFR_TIANO_GUID,
    },
    0,
    0
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    END_DEVICE_PATH_LENGTH,
    0
  }
};

/**
 The HII driver handle passed in for HiiDatabase.NewPackageList() requires
 that there should be DevicePath Protocol installed on it.
 This routine create a virtual Driver Handle by installing a vendor device
 path on it, so as to use it to invoke HiiDatabase.NewPackageList().

 @param[out] DriverHandle       Handle to be returned

 @retval EFI_SUCCESS            Handle destroy success.
 @retval EFI_OUT_OF_RESOURCES   Not enough memory.
**/
EFI_STATUS
CreateHiiDriverHandle (
  OUT EFI_HANDLE               *DriverHandle
  )
{
  EFI_STATUS                   Status;
  HII_VENDOR_DEVICE_PATH_NODE  *VendorDevicePath;

  VendorDevicePath = AllocateCopyPool (sizeof (HII_TEMP_DEVICE_PATH), &mHiiVendorDevicePathTemplate);
  if (VendorDevicePath == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Use memory address as unique ID to distinguish from different device paths
  //
  VendorDevicePath->UniqueId = (UINT64) ((UINTN) VendorDevicePath);

  *DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  VendorDevicePath,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Collect Font database to install font.
  
  @retval EFI_NOT_FOUND         Doesn't found FontDatabase.
  @retval EFI_LOAD_ERROR        Handle gEfiFontDatabaseProtocolGuid protocol failed.
  @retval EFI_OUT_OF_RESOURCES  Not enough memory.
**/
EFI_STATUS
SetSupportLanguageFonts (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  UINTN                                 NarrowFontSize;
  EFI_HII_SIMPLE_FONT_PACKAGE_HDR       *SimplifiedFont;
  UINTN                                 PackageLength;
  UINT8                                 *Package;
  UINT8                                 *Location;
  EFI_HII_DATABASE_PROTOCOL             *HiiDatabase;
  EFI_HANDLE                            DriverHandle;
  UINTN                                 TotalStringSize;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 HandleCount;
  EFI_FONT_DATABASE_PROTOCOL            *Fonts;
  UINTN                                 Index1;
  CHAR8                                 *LangBuffer;

  LangBuffer = NULL;
  Fonts = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFontDatabaseProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  if (EFI_ERROR (Status) || HandleCount == 0) {
    return EFI_NOT_FOUND;
  }

  Status = gBS->LocateProtocol (
                  &gEfiHiiDatabaseProtocolGuid,
                  NULL,
                  (VOID **)&HiiDatabase
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TotalStringSize = 0;

  for (Index1 = 0; Index1 < HandleCount; Index1++) {
    Status = gBS->HandleProtocol(
                    HandleBuffer[Index1],
                    &gEfiFontDatabaseProtocolGuid,
                    (VOID **)&Fonts
                    );
    if (EFI_ERROR (Status)) {
      return EFI_LOAD_ERROR;
    }
    NarrowFontSize = (Fonts->NumOfNarrowGlyph * sizeof (EFI_NARROW_GLYPH)) + (Fonts->NumOfWideGlyph * sizeof (EFI_WIDE_GLYPH));

    //
    // Add 4 bytes to the header for entire length for PreparePackageList use only.
    // Looks ugly. Might be updated when font tool is ready.
    //
    PackageLength   = sizeof (EFI_HII_SIMPLE_FONT_PACKAGE_HDR) + NarrowFontSize + 4;
    Package = (UINT8 *)AllocateZeroPool (PackageLength);
    if (Package == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (Package, &PackageLength, 4);
    SimplifiedFont = (EFI_HII_SIMPLE_FONT_PACKAGE_HDR*) (Package + 4);
    SimplifiedFont->Header.Length        = (UINT32) (PackageLength - 4);
    SimplifiedFont->Header.Type          = EFI_HII_PACKAGE_SIMPLE_FONTS;
    SimplifiedFont->NumberOfNarrowGlyphs = Fonts->NumOfNarrowGlyph;
    SimplifiedFont->NumberOfWideGlyphs = Fonts->NumOfWideGlyph;

    Location = (UINT8 *) (&SimplifiedFont->NumberOfWideGlyphs + 1);
    CopyMem(
      Location,
      Fonts->FontNarrowGlyph,
      (Fonts->NumOfNarrowGlyph * sizeof (EFI_NARROW_GLYPH))
      );

    //
    //Copy mChineseFontWideGlyphData[]
    //
    CopyMem(
      Location + (Fonts->NumOfNarrowGlyph * sizeof (EFI_NARROW_GLYPH)),
      Fonts->FontsWideGlyph,
      (Fonts->NumOfWideGlyph * sizeof (EFI_WIDE_GLYPH))
      );
    //
    // Add this simplified font package to a package list then install it.
    //

    CreateHiiDriverHandle (&DriverHandle);
    HiiHandle = HiiAddPackages (&mPackageListGuid, DriverHandle, Package, NULL);
    ASSERT(HiiHandle != NULL);
    FreePool (Package);
    TotalStringSize += AsciiStrSize ((CHAR8 *)Fonts->SupportedLanguages);
  }
  LangBuffer = (CHAR8 *)AllocateZeroPool (TotalStringSize);
  ASSERT (LangBuffer != NULL);
  if (LangBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  for (Index1 = 0; Index1 < HandleCount; Index1++) {
    Status = gBS->HandleProtocol(
                    HandleBuffer[Index1],
                    &gEfiFontDatabaseProtocolGuid,
                    (VOID **)&Fonts
                    );
    AsciiStrCatS (LangBuffer, TotalStringSize, (CHAR8 *)Fonts->SupportedLanguages);
    if (Index1 != HandleCount - 1) {
      AsciiStrCatS (LangBuffer, TotalStringSize, ";");
    }
  }

  Status = CommonSetVariable (
             L"PlatformLangCodes",
             &gEfiGlobalVariableGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS |
             EFI_VARIABLE_RUNTIME_ACCESS,
             TotalStringSize,
             LangBuffer
             );

  FreePool (LangBuffer);
  return Status;
}


/**
  Initalize LangCodes from PCD

  @param[in]   LangCodesSettingRequired
**/
VOID
InitializeLanguage (
  IN BOOLEAN LangCodesSettingRequired
  )
{
  EFI_STATUS  Status;
  UINTN       Size;
  CHAR8       *Lang;
  CHAR8       LangCode[ISO_639_2_ENTRY_SIZE + 1];
  CHAR8       *LangCodes;
  CHAR8       *PlatformLang;
  CHAR8       *PlatformLangCodes;
  UINTN       Index;
  BOOLEAN     Invalid;

  LangCodes = (CHAR8 *)PcdGetPtr (PcdUefiVariableDefaultLangCodes);
  if (LangCodesSettingRequired) {
    if (!FeaturePcdGet (PcdUefiVariableDefaultLangDeprecate)) {
      //
      // UEFI 2.1 depricated this variable so we support turning it off
      //
      Status = CommonSetVariable (
                      L"LangCodes",
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                      AsciiStrSize (LangCodes),
                      LangCodes
                      );
    }


    PlatformLangCodes = (CHAR8 *)PcdGetPtr (PcdUefiVariableDefaultPlatformLangCodes);
    Status = CommonSetVariable (
               L"PlatformLangCodes",
               &gEfiGlobalVariableGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               AsciiStrSize (PlatformLangCodes),
               PlatformLangCodes
               );
  }

  if (!FeaturePcdGet (PcdUefiVariableDefaultLangDeprecate)) {
    //
    // UEFI 2.1 depricated this variable so we support turning it off
    //
    //
    // Find current LangCode from Lang NV Variable
    //
    Size = ISO_639_2_ENTRY_SIZE + 1;
    Status =CommonGetVariable (
              L"Lang",
              &gEfiGlobalVariableGuid,
              &Size,
              &LangCode
              );
    if (!EFI_ERROR (Status)) {
      Status = EFI_NOT_FOUND;

      for (Index = 0; LangCodes[Index] != 0; Index += ISO_639_2_ENTRY_SIZE) {
        if (CompareMem (&LangCodes[Index], LangCode, ISO_639_2_ENTRY_SIZE) == 0) {
          Status = EFI_SUCCESS;
          break;
        }
      }
    }

    //
    // If we cannot get language code from Lang variable, or LangCode cannot be found from language table,
    // set the mDefaultLangCode to Lang variable.
    //
    if (EFI_ERROR (Status)) {
      Lang = (CHAR8 *)PcdGetPtr (PcdUefiVariableDefaultLang);
      Status = CommonSetVariable (
                 L"Lang",
                 &gEfiGlobalVariableGuid,
                 EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                 ISO_639_2_ENTRY_SIZE + 1,
                 Lang
                 );
    }
  }

  Invalid = FALSE;
  PlatformLang = CommonGetVariableData (L"PlatformLang", &gEfiGlobalVariableGuid);
  if (PlatformLang != NULL) {
    //
    // Check Current PlatformLang value against PlatformLangCode.
    // Need a library that is TBD Set Invalid based on state of PlatformLang.
    //
    FreePool (PlatformLang);
  } else {
    // No valid variable is set
    Invalid = TRUE;
  }

  if (Invalid) {
    PlatformLang = (CHAR8 *)PcdGetPtr (PcdUefiVariableDefaultPlatformLang);
    Status = CommonSetVariable (
               L"PlatformLang",
               &gEfiGlobalVariableGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               AsciiStrSize (PlatformLang),
               PlatformLang
               );
  }
}

/**
  Set support language font by pcd and install font from H2O_HII_FONT_FILE_GUID,
  if H2O_HII_FONT is not found, then will collect Font database to install font.

  @retval EFI_SUCCESS         Install HII font success.
  @retval EFI_NOT_FOUND       Doesn't found H2O_HII_FONT or FontDatabase.
**/
EFI_STATUS
SetSupportLanguageFontsByPcd (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINTN                         FvProtocolCount;
  EFI_HANDLE                    *FvHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINTN                         Index;
  UINT32                        AuthenticationStatus;
  UINTN                         ImageSize;
  UINT8                         *ImageData;
  UINTN                         PackageLength;
  UINT8                         *Package;
  EFI_HANDLE                    DriverHandle;
  EFI_HII_HANDLE                HiiHandle;



  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &FvProtocolCount,
                  &FvHandles
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ImageData = NULL;
  ImageSize = 0;

  for (Index = 0; Index < FvProtocolCount; Index++) {
    Status = gBS->HandleProtocol (
                    FvHandles[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &Fv
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HandleProtocol(%r)\n", Status));
      continue;
    }

    Status = Fv->ReadSection (
                  Fv,
                  &gH2OHiiFontFileGuid,
                  EFI_SECTION_RAW,
                  0,
                  (VOID **)&ImageData,
                  &ImageSize,
                  &AuthenticationStatus
                  );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    return SetSupportLanguageFonts ();
  }

  InitializeLanguage (TRUE);

  PackageLength = 4 + ImageSize;
  Package = (UINT8 *)AllocateZeroPool (PackageLength);
  if (Package == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (Package, &PackageLength, 4);
  CopyMem (Package + 4, ImageData, ImageSize);

  CreateHiiDriverHandle (&DriverHandle);
  HiiHandle = HiiAddPackages (&mPackageListGuid, DriverHandle, Package, NULL);
  ASSERT(HiiHandle != NULL);

  FreePool (ImageData);
  FreePool (Package);

  return EFI_SUCCESS;

}

/**
  Get all SupportedLanguages and append ";en" in the end of SupportedLanguages.
  Then set back to SupportedLanguages.

  @param  UnicodeCollation2    A pointer to the EFI UNICODE COLLATION PROTOCOL.

  @retval EFI_SUCCESS           Update Unicode Collation 2 successfully.
  @retval EFI_OUT_OF_RESOURCES  AllocateZeroPool failed.
**/
EFI_STATUS
UpdateUnicode2SupportedLanguages (
  OUT EFI_UNICODE_COLLATION_PROTOCOL  *UnicodeCollation2
  )
{
  CHAR8    *PlatformLangCodes;
  CHAR8    *AsciiStrBuffer;
  UINTN    AsciiStrBufferSize;
  UINTN    PaddingSize;
  UINTN    Index;

  PlatformLangCodes = CommonGetVariableData (L"PlatformLangCodes", &gEfiGlobalVariableGuid);
  ASSERT (PlatformLangCodes != NULL);

  if (PlatformLangCodes != NULL) {
    UnicodeCollation2->SupportedLanguages = PlatformLangCodes;
  }

  //
  // Run SCT(edk2-test-stable201910) hang because SCT fails to find matched UnicodeCollation2 by searching "en" with null-terminated char.
  // In order to workaround it, append ";en" in the end of SupportedLanguages and make sure string size is the multiples of 3.
  //
  AsciiStrBufferSize  = AsciiStrLen (UnicodeCollation2->SupportedLanguages) + sizeof(";en");
  PaddingSize         = ((AsciiStrBufferSize % 3) == 0) ? 0 : (3 - (AsciiStrBufferSize % 3));
  AsciiStrBufferSize += PaddingSize;

  AsciiStrBuffer = AllocateZeroPool (AsciiStrBufferSize);
  if (AsciiStrBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  AsciiStrCpyS (AsciiStrBuffer, AsciiStrBufferSize, UnicodeCollation2->SupportedLanguages);
  for (Index = 0; Index < PaddingSize; Index++) {
    AsciiStrCatS (AsciiStrBuffer, AsciiStrBufferSize, " ");
  }
  AsciiStrCatS (AsciiStrBuffer, AsciiStrBufferSize, ";en");

  UnicodeCollation2->SupportedLanguages = AsciiStrBuffer;
  if (PlatformLangCodes != NULL) {
    FreePool (PlatformLangCodes);
  }

  return EFI_SUCCESS;
}