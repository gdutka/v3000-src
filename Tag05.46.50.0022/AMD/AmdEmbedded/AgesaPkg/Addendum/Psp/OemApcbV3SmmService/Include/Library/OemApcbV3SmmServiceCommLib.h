/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#ifndef _OEM_APCB_V3_SMM_SERVICE_COMM_LIB_H_
#define _OEM_APCB_V3_SMM_SERVICE_COMM_LIB_H_

#include <Protocol/AmdApcbProtocol.h>

/**
 * @brief Update Apcb Data
 *
 * @param[in][out]    *Value
 *
 * @return            EFI_STATUS
 */
EFI_STATUS
OemApcbV3SmmServiceCommUpdateApcb (
  IN VOID   *Value
);

#endif // _OEM_APCB_V3_SMM_SERVICE_COMM_LIB_H_

