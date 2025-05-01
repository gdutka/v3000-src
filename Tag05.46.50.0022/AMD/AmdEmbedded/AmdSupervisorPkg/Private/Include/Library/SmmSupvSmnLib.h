/** @file
 * SmmSupv SMN related function

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SMMSUPV_SMN_H_
#define _SMMSUPV_SMN_H_

/**
 * @brief Light version version SMN Read with less depx
 *
 * @param Address   SMN address
 * @return UINT32   Smn Register value
 */
UINT32
SmmSupvSmnRegRead (
  UINT32 Address
  );

/**
 * @brief Light version version SMN write with less depx
 *
 * @param Address
 * @param Value
 * @return VOID
 */
VOID
SmmSupvSmnRegWrite (
  UINT32 Address,
  UINT32 Value
  );
#endif // _SMMSUPV_SMN_H_


