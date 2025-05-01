/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _GNB_CXL_CONFIG_INFO_HOB_H_
#define _GNB_CXL_CONFIG_INFO_HOB_H_

// {68D5E47A-AE62-4BE0-BF9D-9040A18618A3}
#define GNB_CXL_CONFIG_INFO_HOB_GUID \
  { 0x68D5E47A, 0xAE62, 0x4BE0, 0xBF, 0x9D, 0x90, 0x40, 0xA1, 0x86, 0x18, 0xA3 }

extern EFI_GUID gGnbCxlConfigInfoHobGuid;

#pragma pack (push, 1)

/// PCIe information HOB data
typedef struct _GNB_CXL_CONFIG_INFO_HOB {
  UINT8                       NumOfCxlInfo;                 ///< Number of CXL config info
  CXL_ADDR_MAP_INFO           CxlInfo[1];                      ///< CXL Config Structure
} GNB_CXL_CONFIG_INFO_HOB;

#pragma pack (pop)

#endif // _GNB_CXL_CONFIG_INFO_HOB_H_



