/** @file
 Head file for HII Support

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

#ifndef _HII_SUPPORT_H_
#define _HII_SUPPORT_H_

#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiDbLib.h>
#include <Library/HiiStringLib.h>
#include <Library/HiiConfigAccessLib.h>
#include <Library/VariableLib.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/UnicodeCollation.h>
#include <Protocol/FontDatabase.h>
#include <Guid/GlobalVariable.h>

///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

/**
  Set support language font by pcd and install font from H2O_HII_FONT_FILE_GUID,
  if H2O_HII_FONT is not found, then will collect Font database to install font.

  @retval EFI_SUCCESS         Install HII font success.
  @retval EFI_NOT_FOUND       Doesn't found H2O_HII_FONT or FontDatabase.
**/
EFI_STATUS
SetSupportLanguageFontsByPcd (
  VOID
  );

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
  );

#endif