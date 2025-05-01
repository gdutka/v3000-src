/*
*******************************************************************************

Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#ifndef _MEM_LOG_LIB_H_
#define _MEM_LOG_LIB_H_

#include <Uefi.h>

#define MAX_LOCAL_BUFFER_SIZE 1024
#define MEM_LOG_BUFFER_SIZE   (sizeof(CHAR8) * MAX_LOCAL_BUFFER_SIZE * 1024)   //1024KB

extern UINT32                 mMemLogCount;
#define MEMLOG(str, ...)      MemLogInternal("%c%08x%c" str, 02, mMemLogCount++, 03, ##__VA_ARGS__);

/**
 *  Process debug string and print to memory
 *
 *  @param[in] *Format - format string
 *  @param[in] Marker  - Variable parameter
 *
**/
VOID
EFIAPI
MemLogInternal (
  IN      CONST CHAR8 *Format,
  ...
  );

/**
 *  Get debug string and print to memory
 *
 *  @param[in] *Format - format string
 *
**/
VOID
EFIAPI
MemVLog (
  IN   UINTN OutPutStringLen,
  IN   CHAR8 *LocalBuffer
  );
#endif
