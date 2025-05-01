/*
*******************************************************************************

Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#ifndef _ERROR_DATA_LOG_LIB_H_
#define _ERROR_DATA_LOG_LIB_H_

#include <Uefi.h>
#include <SmmSupvPostCode.h>

#define PREFIX_LOG_ANALYZER               "^$ "        // prefix for the log analyzer

VOID
WriteErrorData (
  IN SMMSUPV_POST_CODE Data
  );

VOID
WriteErrorDataAndReset (
  IN SMMSUPV_POST_CODE Data
  );

VOID
CleanErrorData (
  );
#endif  // _ERROR_DATA_LOG_LIB_H_
