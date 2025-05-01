/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 *
 * AMD Ccx MCA Library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 312770 $   @e \$Date: 2015-02-11 13:24:42 +0800 (Wed, 11 Feb 2015) $
 *
 */

#ifndef _CCX_SET_MCA_LIB_H_
#define _CCX_SET_MCA_LIB_H_
#pragma pack (push, 1)

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxSetMca
 *
 *  Description:
 *    This routine sets all MCA registers
 *
 */
VOID
CcxSetMca (
  );


/**
 *
 *  @brief  CcxSetTransparentErrorLoggingMca
 *
 *  @details
 *    This routine sets TransparentErrorLoggingEnable bit in each MCA
 *    Config MSR
 *
 *  @param[in] TransparentErrorLoggingEnable    TRUE to enable logging
 *                                              or FALSE to disable logging
 *
 *  @retval    None
 */
VOID
CcxSetTransparentErrorLoggingMca (
  IN BOOLEAN        TransparentErrorLoggingEnable
  );

#pragma pack (pop)
#endif // _CCX_SET_MCA_LIB_H_

