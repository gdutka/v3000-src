/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _SOC_ZEN3_SERVICES_RMB_PEI_H_
#define _SOC_ZEN3_SERVICES_RMB_PEI_H_

EFI_STATUS
SocZen3RmbGetOpnCorePresence (
  IN       EFI_PEI_SERVICES   **PeiServices,
  IN       UINT32               SystemDieNumber,
     OUT   UINT32              *CcdEnableMap,
     OUT   UINT32              *CcdDisableMap,
     OUT   UINT32              *CoreDisMap,
     OUT   BOOLEAN             *SmtEnable
  );

UINT32
SocZen3RmbGetCcd0SoftDowncoreAddress (
  IN       EFI_PEI_SERVICES   **PeiServices
  );

BOOLEAN
SocZen3RmbIsOneCcdModeSupported (
  IN       EFI_PEI_SERVICES   **PeiServices
  );

BOOLEAN
SocZen3RmbIsS3Supported (
  IN       EFI_PEI_SERVICES   **PeiServices
  );

BOOLEAN
SocZen3RmbIsGameModeSupported (
  IN       EFI_PEI_SERVICES   **PeiServices
  );

#endif
