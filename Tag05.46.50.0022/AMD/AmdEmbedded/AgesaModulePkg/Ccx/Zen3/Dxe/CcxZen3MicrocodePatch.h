/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCX Zen3 Microcode definitions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _CCX_ZEN3_MICROCODE_PATCH_H_
#define _CCX_ZEN3_MICROCODE_PATCH_H_

VOID
CcxZen3MicrocodeInit (
  IN OUT   UINT64             *UcodePatchAddr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

#endif


