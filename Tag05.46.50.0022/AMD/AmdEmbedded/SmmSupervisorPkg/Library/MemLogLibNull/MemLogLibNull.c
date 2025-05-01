/*
*******************************************************************************

Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#include <Library/MemLogLib.h>

UINT32  mMemLogCount = 0;

/**
 *  Process debug string and print to memory
 *
 *  @param[in] *Format - format string
 *
**/
VOID
EFIAPI
MemLogInternal (
  IN      CONST CHAR8 *Format,
  ...
  )
{
}
/**
 *  print debug string to memory
 *
 *  @param[in] *Format - format string
 *
**/
VOID
EFIAPI
MemVLog (
  IN   UINTN OutPutStringLen,
  IN   CHAR8 *LocalBuffer
  )
{
}
