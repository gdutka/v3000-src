/** @file
  HSTI DXE Driver

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <FchRegistersCommon.h>

#include "HstiDxe.h"



//#define AMD_HSTI_CS_SYSTEM_SUPPORT

/**
  Internal function to init IBV HSTI Platform Security State Interface
**/
STATIC
EFI_STATUS
InitHstiIbvData (
  VOID
  )
{
  ADAPTER_INFO_PLATFORM_SECURITY   *Hsti;
  UINTN                            HstiSize;
  UINT8                            *HstiData;

  //
  // Total HstiSize is structure size + requirement/implement/Verify feature size + a null string for error string,
  // Allocate Pool for hold ADAPTER_INFO_PLATFORM_SECURITY
  //
  HstiSize = sizeof (ADAPTER_INFO_PLATFORM_SECURITY) + 3 * HSTI_AMD_FEATUERS_SIZE_IN_BYTES + HSTI_AMD_ERROR_STRING_SIZE;
  Hsti = AllocateZeroPool (HstiSize);
  if (Hsti == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize HSTI table
  //
  Hsti->Version              = PLATFORM_SECURITY_VERSION_VNEXTCS;
  Hsti->Role                 = PLATFORM_SECURITY_ROLE_PLATFORM_IBV;
  StrCpyS (Hsti->ImplementationID, sizeof(Hsti->ImplementationID) / sizeof(CHAR16), HSTI_IBV_IMPLEMENT_ID);
  Hsti->SecurityFeaturesSize = HSTI_AMD_FEATUERS_SIZE_IN_BYTES;

  HstiData = (UINT8 *) (Hsti + 1);

  //
  // The SecurityFeaturesRequired field is only used in processing when a field in an IHV HSTI objec
  // So IBV doesn't need to file requirement field.
  //

  //
  // Implementation
  //
  HstiData += HSTI_AMD_FEATUERS_SIZE_IN_BYTES;

  HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_0] = HSTI_AMD_CRYPTO_STRENGTH;

  if (FeaturePcdGet (PcdSecureFlashSupported)) {
    HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_1] |= HSTI_AMD_FWCODE_PROTECT_PROTECT_SPI;
  }

  if (FeaturePcdGet (PcdSecureFlashSupported) ||
      FeaturePcdGet (PcdH2OCapsuleUpdateSupported) ||
      FeaturePcdGet (PcdUefiCapsuleUpdateSupported)) {
    HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_1] |= HSTI_AMD_FWCODE_PROTECT_SIGNED_FW_CHECK;
    HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_2] = HSTI_AMD_SECURE_FW_UPDATE_DFT_TESTKEY +
                                              HSTI_AMD_SECURE_FW_UPDATE_CHECK_TESTKEY_IN_PRODUCTION +
											  HSTI_AMD_SECURE_FW_UPDATE_ROLLBACK_CHECK;
  }

  HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_3] = HSTI_AMD_SECUREBOOT_BACKDOOR_INLINE_PROMPT_CHECK +

                                            HSTI_AMD_SECUREBOOT_BACKDOOR_MANUFACTURE_CHECK;
  //
  //CS System
  //
  #ifdef AMD_HSTI_CS_SYSTEM_SUPPORT
    HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_5] = HSTI_AMD_CSM_DISABLE_IF_SECUREBOOT_EN +
                                              HSTI_AMD_CSM_DISABLE_ON_CS_SYSTEM;
  #else
    HstiData[HSTI_AMD_FEATURE_BYTE_INDEX_5] = HSTI_AMD_CSM_DISABLE_IF_SECUREBOOT_EN;
  #endif

  //
  // Verification
  //
  HstiData += HSTI_AMD_FEATUERS_SIZE_IN_BYTES;
    
  //
  // Install HSTI table.
  //
  return HstiLibSetTable ((VOID *) Hsti, HstiSize);
}

/**
  Internal function to check whether support SHA256 and RSA2048 secure feature.
**/
STATIC
VOID
CheckSecurityStrength (
  VOID
  )
{
  EFI_STATUS                  Status;
  CRYPTO_SERVICES_PROTOCOL    *Crypto;
  EFI_HASH_PROTOCOL           *Hash;


  Status = gBS->LocateProtocol (&gCryptoServicesProtocolGuid, NULL, (VOID **)&Crypto);
  if (Status != EFI_SUCCESS) {
    return;
  }
  Status = gBS->LocateProtocol (&gEfiHashProtocolGuid, NULL, (VOID **)&Hash);
  if (Status != EFI_SUCCESS) {
    return;
  }

  HstiLibSetFeaturesVerified (
    PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
    HSTI_IBV_IMPLEMENT_ID,
    HSTI_AMD_FEATURE_BYTE_INDEX_0,
    HSTI_AMD_CRYPTO_STRENGTH
    );
}

/**
  Internal function to update check CSM policy secure feature result.
**/
STATIC
VOID
CheckCsmPolicy (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_LEGACY_BIOS_PROTOCOL    *LegacyBios;
  UINTN                       DataSize;
  UINT8                       SecureBoot;
  UINT8                       FeatureBitField;

  FeatureBitField = 0; 

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
  if (Status == EFI_SUCCESS) {
    DataSize = sizeof(UINT8);
    Status = gRT->GetVariable (
                    EFI_SECURE_BOOT_MODE_NAME,
                    &gEfiGlobalVariableGuid,
                    NULL,
                    &DataSize,
                    &SecureBoot
                    );
    if (Status == EFI_SUCCESS && SecureBoot == 1) {
      HstiLibSetErrorString (
        PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
        HSTI_IBV_IMPLEMENT_ID,
        L"Error!! Secure boot Must be disabled if CSM is enabled. Now, secure boot is enabled in CSM is enabled environment."
        );
      return;
    }
  }

  #ifdef AMD_HSTI_CS_SYSTEM_SUPPORT
    FeatureBitField |= HSTI_AMD_CSM_DISABLE_IF_SECUREBOOT_EN +
                       HSTI_AMD_CSM_DISABLE_ON_CS_SYSTEM;
  #else
    FeatureBitField |= HSTI_AMD_CSM_DISABLE_IF_SECUREBOOT_EN;
  #endif
  
  HstiLibSetFeaturesVerified (
    PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
    HSTI_IBV_IMPLEMENT_ID,
    HSTI_AMD_FEATURE_BYTE_INDEX_5,
    FeatureBitField
    );
}

/**
  Internal function to update firmware code protect secure feature result.
**/
STATIC
VOID
CheckFirmwareCodeProtect (
  VOID
  )
{
  UINT8                            FeatureBitField;
  UINTN                            SpiRomBaseAddress;
  UINT32                           SpiControl0Reg;

  FeatureBitField = 0;

  if (FeaturePcdGet (PcdSecureFlashSupported)) {

    SpiRomBaseAddress = (PciRead32 (PCI_LIB_ADDRESS (FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPC_REGA0))) & 0xFFFFFF00;
    SpiControl0Reg = MmioRead32 (SpiRomBaseAddress);

    if ((SpiControl0Reg & (BIT22 + BIT23)) == 0) {
      FeatureBitField |= HSTI_AMD_FWCODE_PROTECT_PROTECT_SPI;
    }
  }

  if (FeaturePcdGet (PcdSecureFlashSupported) ||
      FeaturePcdGet (PcdH2OCapsuleUpdateSupported) ||
      FeaturePcdGet (PcdUefiCapsuleUpdateSupported)) {
    FeatureBitField  |= HSTI_AMD_FWCODE_PROTECT_SIGNED_FW_CHECK;
  } 

  HstiLibSetFeaturesVerified (
    PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
    HSTI_IBV_IMPLEMENT_ID,
    HSTI_AMD_FEATURE_BYTE_INDEX_1,
    FeatureBitField
    );
}


/**
  Internal function to update secure firmware update secure feature result.
**/
STATIC
VOID
CheckSecureFirmwareUpdate (
  VOID
  )
{
  UINT8                            FeatureBitField;

  FeatureBitField = 0;

  if (!FeaturePcdGet (PcdSecureFlashSupported) &&
      !FeaturePcdGet (PcdH2OCapsuleUpdateSupported) &&
      !FeaturePcdGet (PcdUefiCapsuleUpdateSupported)) {
     return;
  }

  FeatureBitField |= HSTI_AMD_SECURE_FW_UPDATE_DFT_TESTKEY +
                     HSTI_AMD_SECURE_FW_UPDATE_CHECK_TESTKEY_IN_PRODUCTION +
                     HSTI_AMD_SECURE_FW_UPDATE_ROLLBACK_CHECK;

  HstiLibSetFeaturesVerified (
    PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
    HSTI_IBV_IMPLEMENT_ID,
    HSTI_AMD_FEATURE_BYTE_INDEX_2,
    FeatureBitField 
    );
}

/**
  Internal function to update secure boot policy secure feature result.
**/
STATIC
VOID
CheckSecureBootPolicy (
  VOID
  )
{
  UINTN               DataSize;
  UINT8               SecureBoot;
  EFI_STATUS          Status;
  UINT8               FeatureBitField;

  if (!FeaturePcdGet (PcdH2OSecureBootSupported)) {
    return;
  }

  DataSize = sizeof(UINT8);
  Status = gRT->GetVariable (
                  EFI_SECURE_BOOT_MODE_NAME,
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &DataSize,
                  &SecureBoot
                  );
  if (Status != EFI_SUCCESS) {
    return;
  }
  Status = gRT->SetVariable (
                  EFI_SECURE_BOOT_MODE_NAME,
                  &gEfiGlobalVariableGuid,
                  0,
                  0,
                  NULL
                  );
  if (Status == EFI_SUCCESS) {
    HstiLibSetErrorString (
      PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
      HSTI_IBV_IMPLEMENT_ID,
      L"Error!! SecureBoot variable must be read-only. But this variable can be deleted."
      );
    return;
  }
  FeatureBitField= HSTI_AMD_SECUREBOOT_BACKDOOR_INLINE_PROMPT_CHECK +
                   HSTI_AMD_SECUREBOOT_BACKDOOR_MANUFACTURE_CHECK;

  HstiLibSetFeaturesVerified (
    PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
    HSTI_IBV_IMPLEMENT_ID,
    HSTI_AMD_FEATURE_BYTE_INDEX_3,
    FeatureBitField
    );
}

/**
  Internal function to update HSTI related data.
**/
STATIC
VOID
UpdateHstiData (
  VOID
  )
{
  CheckSecurityStrength ();
  CheckFirmwareCodeProtect ();
  CheckSecureFirmwareUpdate ();
  CheckSecureBootPolicy ();
  CheckCsmPolicy ();
}

/**
  Callback function for ready to boot event to update HSTI related data.

  @param Event    - Event whose notification function is being invoked.
  @param Context  - Pointer to the notification function's context.
**/
STATIC
VOID
EFIAPI
OnReadyToBoot (
  EFI_EVENT                               Event,
  VOID                                    *Context
  )
{

  UpdateHstiData ();
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}


/**
  The HSTI driver's entry point.

  Create IBV gAdapterInfoPlatformSecurityGuid protocol

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
**/
EFI_STATUS
EFIAPI
HstiEntryPoint (
  IN  EFI_HANDLE           ImageHandle,
  IN  EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS     Status;
  EFI_EVENT      Event;

  Status = InitHstiIbvData ();
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  Status = EfiCreateEventReadyToBootEx (
            TPL_CALLBACK - 1,
            OnReadyToBoot,
            NULL,
            &Event
            );

  return Status;
}
