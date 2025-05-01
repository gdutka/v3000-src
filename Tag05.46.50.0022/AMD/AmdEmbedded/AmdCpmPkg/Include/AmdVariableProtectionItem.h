/*****************************************************************************
 *
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/


#ifndef _AMD_VARIABLE_PROTECTION_ITEM_H_
#define _AMD_VARIABLE_PROTECTION_ITEM_H_

#ifndef MAX_AMD_VARIABLE_PROTECTION_VARIABLE_NAME
#define MAX_AMD_VARIABLE_PROTECTION_VARIABLE_NAME     64
#endif

#ifndef MAX_AMD_VARIABLE_PROTECTION_DEVICE_PATH_NODE
#define MAX_AMD_VARIABLE_PROTECTION_DEVICE_PATH_NODE  32 * 4
#endif

#ifndef MAX_AMD_VARIABLE_PROTECTION_TABLE_SIZE
#define MAX_AMD_VARIABLE_PROTECTION_TABLE_SIZE        16
#endif

#pragma pack(1)

typedef struct _AMD_VARIABLE_PROTECTION_ITEM {
  CHAR16                                VariableName[MAX_AMD_VARIABLE_PROTECTION_VARIABLE_NAME];
  UINT8                                 VendorGuid[16];                                              // EFI_GUID
} AMD_VARIABLE_PROTECTION_ITEM;

typedef struct _AMD_VARIABLE_PROTECTION_TABLE {
  AMD_VARIABLE_PROTECTION_ITEM          ItemList[MAX_AMD_VARIABLE_PROTECTION_TABLE_SIZE];
} AMD_VARIABLE_PROTECTION_TABLE;

#pragma pack()


#endif
