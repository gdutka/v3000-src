/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _OEM_GPIO_CLKREQ_CONTROL_LIB_H_
#define _OEM_GPIO_CLKREQ_CONTROL_LIB_H_

#include <PiPei.h>

/**
 Gpio reset control.

 @param[in]         NbioRootBus           Root Bus number of target NBIO (or chipset)
 @param[in]         ClkReqNumber          ClkReq number request
 @param[in]         ChipsetFlag           Premium Chipset flag (1 == Use Premium Chipset registers)

 @retval            EFI_SUCCESS       Function returns successfully
 @retval            EFI_UNSUPPORTED   Function is not supported
*/
EFI_STATUS
OemGpioClkReqControl (
  IN      UINT32                    NbioRootBus,
  IN      UINT8                     ClkReqNumber,
  IN      UINT8                     ChipsetFlag
  );

#endif


