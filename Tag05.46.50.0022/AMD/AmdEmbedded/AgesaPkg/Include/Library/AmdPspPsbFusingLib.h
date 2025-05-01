/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_PSP_PSB_FUSING_LIB_H_
#define _AMD_PSP_PSB_FUSING_LIB_H_

#include "Porting.h"

/*
 *  Check if PSB Fusing is required
 *
 * @retval    TRUE   PSB Fusing is required
 * @retval    FALSE  PSB Fusing is not required
 */
BOOLEAN
IsPsbFusingRequired (
  );

/*
 *  Do PSB Fusing
 *  Please call IsPsbFusingRequired() to check if PSB Fusing is required before calling this function
 */
VOID
DoPsbFusing (
  );
#endif // _AMD_PSP_PSB_FUSING_LIB_H_


