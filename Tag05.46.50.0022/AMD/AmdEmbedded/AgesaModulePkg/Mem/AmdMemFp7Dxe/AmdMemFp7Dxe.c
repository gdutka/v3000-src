/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Memory API, and related functions.
 *
 * Contains code that initializes memory
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  MEM
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include "Filecode.h"

#define FILECODE MEM_AMDMEMFP7DXE_AMDMEMFP7DXE_FILECODE

/*++
Module Name:

  AmdMemFp7Dxe.c
  Init Memory interface

Abstract:
--*/

/*++

Routine Description:

  PSP Driver Entry. Initialize PSP device and report DRAM info to PSP once found

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdMemFp7Dxe (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  return (Status);
}



