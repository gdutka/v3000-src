/*****************************************************************************
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file  AmdStbLib.h
 *
 * @brief Lib for STB (smart trace buffer) related service
 *
 *
 */

#ifndef _AMD_STB_LIB_H_
#define _AMD_STB_LIB_H_
#include <Library/IdsLib.h>

#define   STB_CTRL_DISABLE       0x0    ///< STB disabled
#define   STB_CTRL_PER_IP        0xF    ///< Per IP control
#define   RMB_MP_POSTCODE_IP0_SMN_ADDRESS  0x3E30210    ///< SMN address for RMB
#define   PHX_MP_POSTCODE_IP0_SMN_ADDRESS  0x3B30210    ///< SMN address for PHX
#define   RPL_MP_POSTCODE_IP0_SMN_ADDRESS  0x3B30210    ///< SMN address for RPL
#define   MDN_MP_POSTCODE_IP0_SMN_ADDRESS  0x3E30210    ///< SMN address for MDN

/**
  GetBiosVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetBiosVerbosity (
  UINT8 *Verbosity
  );

/**
  GetPspVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetPspVerbosity (
  UINT8 *Verbosity
  );

/**
  GetSmuVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetSmuVerbosity (
  UINT8 *Verbosity
  );

/**
  Write Value to STB (Smart trace buffer)
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
  @param[in]  Verbosity of the write, Valid value: 0-3
**/
VOID
AmdStbWrite (
  UINT32 Value,
  UINT8  Verbosity
  );

/**
  Write Value to STB (Smart trace buffer) with verbosity LOW_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseLow (
  UINT32 Value
  );

/**
  Write Value to STB (Smart trace buffer) with verbosity PROD_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseProd (
  UINT32 Value
  );

/**
  Write Value to STB (Smart trace buffer) with verbosity HIGH_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseHigh (
  UINT32 Value
  );

/**
  Programing STB filter mask register when Per IP Verbosity Control selected, and any source IP verbosity select to disable
  When register been programmed, STB agent will not save any source IP postcode to STB
**/
VOID
AmdStbFilterMaskProgramming (
  VOID
  );

#endif  // _AMD_STB_LIB_H_


