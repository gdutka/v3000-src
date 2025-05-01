/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#ifndef _APCB_VARIABLE_LIB_V3_H_
#define _APCB_VARIABLE_LIB_V3_H_

#include "Porting.h"

#define APCB_VARIABLE_STRUCT_ADDRESS_NAME L"ApcbVariableStructAddressName"

#pragma pack (push, 1)

typedef struct {
  UINT64    Apcb68PostTimeShadowAddress;
  UINT64    Apcb68RunTimeShadowAddress;
  UINT64    Apcb60ConfigPostTimeShadowAddress;
  UINT64    Apcb60ConfigRunTimeShadowAddress;
  UINT64    Apcb60EvtLgPostTimeShadowAddress;
  UINT64    Apcb60EvtLgRunTimeShadowAddress;
  BOOLEAN   ApcbMutexLocked;
} APCB_VARIABLE_STRUCT;

#pragma pack (pop)


/**
 * @brief Get Apcb Variable Struct Address
 *
 * @param   VOID
 *
 * @return  APCB_VARIABLE_STRUCT* Pointer
 */
APCB_VARIABLE_STRUCT*
GetApcbVariableStruct (
  VOID
  );

#endif //_APCB_VARIABLE_LIB_V3_H_
