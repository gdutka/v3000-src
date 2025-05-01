/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/PciLib.h>
#include <Library/AmdStbLib.h>
#include <Filecode.h>
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDSTBLIBNULL_AMDSTBLIBNULL_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
  GetBiosVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetBiosVerbosity (
  UINT8 *Verbosity
  )
{
  return EFI_UNSUPPORTED;
}

/**
  GetPspVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetPspVerbosity (
  UINT8 *Verbosity
  )
{
  return EFI_UNSUPPORTED;
}

/**
  GetSmuVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetSmuVerbosity (
  UINT8 *Verbosity
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Light version version SMN write with less depx

  @param[in]  Address, SMN address
  @param[in]  Value, Value to be writen to the address

**/
VOID
SmnRegWrite (
  UINT32 Address,
  UINT32 Value
  )
{

}

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
  )
{

}


/**
  Write Value to STB (Smart trace buffer) with verbosity LOW_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseLow (
  UINT32 Value
  )
{

}

/**
  Write Value to STB (Smart trace buffer) with verbosity PROD_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseProd (
  UINT32 Value
  )
{

}

/**
  Write Value to STB (Smart trace buffer) with verbosity HIGH_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseHigh (
  UINT32 Value
  )
{

}

/**
  Programing STB filter mask register when Per IP Verbosity Control selected, and any source IP verbosity select to disable
  When register been programmed, STB agent will not save any source IP postcode to STB
**/
VOID
AmdStbFilterMaskProgramming (
  VOID
  )
{

}
