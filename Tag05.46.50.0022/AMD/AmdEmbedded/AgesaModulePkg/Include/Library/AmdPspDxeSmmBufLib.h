/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#ifndef _AMD_PSP_DXE_SMM_BUF_LIB_H_
#define _AMD_PSP_DXE_SMM_BUF_LIB_H_

#include "Porting.h"

#define AMD_PSP_SMM_TMP_BUFFER_SIZE    (5 * 1024)

/**
 * @brief Get Amd Psp Smm/Run Time Buffer Address
 *
 * @param   VOID
 *
 * @return  UINT8* Pointer
 */
UINT8 *
GetAmdPspSmmRunTimeBufferAddress (
  VOID
  );

#endif //_AMD_PSP_DXE_SMM_BUF_LIB_H_
