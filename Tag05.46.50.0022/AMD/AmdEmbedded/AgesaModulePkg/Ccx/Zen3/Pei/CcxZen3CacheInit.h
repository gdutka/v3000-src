/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCX Zen3 cache initializtion
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _CCX_ZEN3_CACHE_INIT_H_
#define _CCX_ZEN3_CACHE_INIT_H_

VOID
InitializeVariableMTRRs (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
CcxZen3CacheInit (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

#endif

