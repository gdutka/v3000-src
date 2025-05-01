/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_SOC_BASE_LIB_H_
#define _AMD_SOC_BASE_LIB_H_

#include <AmdSoc.h>

BOOLEAN
SocHardwareIdentificationCheck (
  IN       SOC_ID_STRUCT *SocId
  );

BOOLEAN
SocHardwareIdentificationCheckV2 (
  IN       SOC_ID_STRUCT *SocId
  );

BOOLEAN
SocFamilyIdentificationCheck (
  IN       UINT32 SocFamilyID
  );

BOOLEAN
SocFamilyIdentificationCheckV2 (
  IN       UINT32 SocFamilyID
  );

BOOLEAN
AmdPcdInit (
  IN       UINT32        NumberOfPcdEntries,
  IN       AMD_PCD_LIST  *AmdPcdList
  );

BOOLEAN
SocSpecialFuseCheck (
  IN       UINT32 Addr,
  IN       UINT32 BitLsb,
  IN       UINT32 BitMsb,
  IN       UINT32 ExpectedValue
  );

#endif  // _AMD_SOC_BASE_LIB_H_


