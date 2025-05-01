/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef __CBS_VARIABLE_H__
#define __CBS_VARIABLE_H__
#pragma pack(1)

typedef struct _CBS_VARIABLE_HEADER
{
  UINT32 CbsVariableStructUniqueValue;               ///< Will be different if CBS varialbe structure changes
  UINT32 NewRecordOffset;                            ///< Record the offset of reserved region start, which also the offset of the new record
  UINT32 ApcbVariableHash;                           ///< Record the APCB Hash Value
  UINT8  Reserved[20];                               ///< Reserved for future use
} CBS_VARIABLE_HEADER;

typedef struct _CBS_CONFIG {
  CBS_VARIABLE_HEADER  Header;                       ///< Variable header
  UINT8                CbsCmnVariable0;
  UINT8                CbsCmnVariable1;
  UINT8                Reserved;                     ///< reserve
} CBS_CONFIG;

#pragma pack()

#endif //__CBS_VARIABLE_H__
