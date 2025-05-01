/** @file
;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _H2O_DEFAULT_PASSWORD_DXE_H_
#define _H2O_DEFAULT_PASSWORD_DXE_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/VariableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SysPasswordService.h>
#include <Protocol/VariableLock.h>
#include <Guid/GlobalVariable.h>
#include <Library/HiiLib.h>
#include <Library/H2OLib.h>
#include <Protocol/H2ODialog.h>

#define H2O_DEFAULT_PASSWORD_VARIABLE_NAME   L"DefSetupPswd"

#define FixedPcdGetPtrSize(TokenName)      (UINTN)_PCD_PATCHABLE_##TokenName##_SIZE

#endif

