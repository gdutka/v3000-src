/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/


#include "AmdCbsVariable.h"

VOID
CbsWriteDefalutValue (
  IN UINT8 *IfrData
  )
{
  CBS_CONFIG *pSetup_Config;

  pSetup_Config = (CBS_CONFIG *)IfrData;
  pSetup_Config->Header.NewRecordOffset = OFFSET_OF (CBS_CONFIG, Reserved);
  pSetup_Config->Header.CbsVariableStructUniqueValue = 0x4E554C4C;
  pSetup_Config->Header.ApcbVariableHash = 0x0;
  pSetup_Config->Reserved = 1;
}

