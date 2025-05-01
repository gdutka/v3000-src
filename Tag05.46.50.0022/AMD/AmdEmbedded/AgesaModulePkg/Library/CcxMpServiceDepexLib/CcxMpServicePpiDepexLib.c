/*****************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * This is for a NULL library to create a dependency for CpuMpPei driver in UDK.
 * CCX must have launched APs before CpuMpPei driver can load.
 *
*/

#include <Base.h>
#include <Filecode.h>

#define FILECODE LIBRARY_CCXMPSERVICEDEPEXLIB_CCXMPSERVICEPPIDEPEXLIB_FILECODE

/**
  Empty function to allow forcing a Depex on CpuMpPei Driver
**/
VOID CpuMpServicePpiDepexPlaceHolderFunction (VOID) {}
