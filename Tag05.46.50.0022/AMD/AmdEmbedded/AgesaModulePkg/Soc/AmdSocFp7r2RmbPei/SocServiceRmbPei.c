/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <AGESA.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/HobLib.h>
#include <Guid/SocCoreInfo.h>
#include <Filecode.h>

#define FILECODE SOC_AMDSOCFP7R2RMBPEI_SOCSERVICERMBPEI_FILECODE

EFI_STATUS
EFIAPI
BuildSocCoreInfoHob (
  VOID
  )
{
  SOC_CORE_INFO SocCoreInfoHob;
  UINT8 CoreIndex;
  UINT32 SmnData32;

  // Init the Hob
  ZeroMem (&SocCoreInfoHob, sizeof(SOC_CORE_INFO));

  // Ccd Max Support
  SocCoreInfoHob.CcdMaxCount = 1;

  // Ccd Enable Map
  SocCoreInfoHob.CcdEnableMap.Value = 1;

  // Core Status
  SocCoreInfoHob.CoreMaxCountPerCcd = 8;

  // Read SMU::FUSE::FUSE_DATA_xxx
  SmnRegisterRead (0, 0x5D4DC, &SmnData32);
  // Core Fused Disable Map
  SocCoreInfoHob.CoreDisMap[0] = (SmnData32 >> 7) & 0xFF; //[MP0_CORE_DISABLE]

  // Core Fused SMT Status
  SocCoreInfoHob.SmtEnable = (SmnData32 & BIT6) ? Core_Multi_Thread : Core_Single_Thread; //[MP0_SMT_ENABLE]

  // Core Thread Status via SMU::SMUIO::THREAD_CONFIGURATION[SMTDisable]
  SmnRegisterRead (0, 0x5A81C, &SmnData32);
  SocCoreInfoHob.CoreThreadStatus = (SmnData32 & BIT9) ? Core_Single_Thread : Core_Multi_Thread;

  // Core Active Map via SMU::SMUIO::CORE_ENABLE[CoreEn]
  SmnRegisterRead (0, 0x5A880, &SmnData32);
  SocCoreInfoHob.CoreActiveMap[0] = SmnData32 & 0xFF;

  // Core Status
  for (CoreIndex = 0; CoreIndex < SocCoreInfoHob.CoreMaxCountPerCcd; CoreIndex++) {
    if (SocCoreInfoHob.CoreDisMap[0] & (1 << CoreIndex)) {
      SocCoreInfoHob.CoreStatus[0][CoreIndex] = Core_Fused_Off;
    } else if (SocCoreInfoHob.CoreActiveMap[0] & (1 << CoreIndex)) {
      SocCoreInfoHob.CoreStatus[0][CoreIndex] = Core_Active;
    } else {
      SocCoreInfoHob.CoreStatus[0][CoreIndex] = Core_Soft_Off;
    }
  }

  if (BuildGuidDataHob (&gSocCoreInfoGuid, &SocCoreInfoHob, sizeof(SOC_CORE_INFO)) == NULL) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
