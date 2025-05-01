/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains code associated with microcode patch loading for Zen3 processors
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
 */
/*++
Module Name:

  CcxZen3MicrocodePatch.c
  Load microcode

Abstract:
--*/

#include <Library/UefiLib.h>
#include <AGESA.h>
#include <Library/CcxMicrocodePatchLib.h>
#include "CcxZen3MicrocodePatch.h"
#include <IdsLib.h>
#include <Filecode.h>

#define FILECODE CCX_ZEN3_DXE_CCXZEN3MICROCODEPATCH_FILECODE

/*++

Routine Description:

  Zen3 Microcode Patch loading

Arguments:
 *  @param[in,out] UcodePatchAddr        - The selected UcodePatch address, return 0 if not found
 *  @param[in] StdHeader                 - Config handle for library and services.
Returns:

--*/
VOID
CcxZen3MicrocodeInit (
  IN OUT   UINT64             *UcodePatchAddr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  LoadMicrocodePatch (UcodePatchAddr, StdHeader);
}


