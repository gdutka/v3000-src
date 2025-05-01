/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _SETUP_FUNCS_H_
#define _SETUP_FUNCS_H_

#include <SetupUtility.h>
#include <Library/BaseLib.h>
#include <StdLib/StdCLibSupport.h>

EFI_STATUS
LoadCustomOption (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL        *This
  );

EFI_STATUS
SaveCustomOption (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL        *This
  );

EFI_STATUS
DiscardChange (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL        *This
  );

VOID *
GetVariableAndSize (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize
  );

EFI_STATUS
CheckLanguage (
  VOID
  );

EFI_STATUS
SaveSetupConfig (
  IN     CHAR16             *VariableName,
  IN     EFI_GUID           *VendorGuid,
  IN     UINT32             Attributes,
  IN     UINTN              DataSize,
  IN     VOID               *Buffer
  );

EFI_STATUS
DefaultSetup (
  OUT CHIPSET_CONFIGURATION            *SetupNvData
  );

#endif
