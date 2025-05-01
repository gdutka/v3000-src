/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <Uefi.h>
#pragma pack (push, 1)

#include <FchPT.h>
#include <Library/DebugLib.h>


/*********************************************************************************
 * Name: FchPlatformPTPeiInit
 *
 * Description
 *   Platform hook to update PT parameters
 *
 * Input
 *   FchPTData : a pointer to PT data structure
 *
 * Output
 *   none if the reset is successful
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchPlatformPTPeiInit (
  IN      VOID                          *FchPTData
  )
{
  FCH_PT                *Promontory;

  Promontory  = (FCH_PT *)FchPTData;

  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchPlatformOemPeiInit
 *
 * Description
 *   Platform Oem hook
 *
 * Input
 *   FchData
 *
 * Output
 *
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchPlatformOemPeiInit (
  IN      VOID                          *FchData
  )
{
  DEBUG ((DEBUG_INFO, "OEM-PEI-FchPlatformOemPeiInit-Start\n"));
  DEBUG ((DEBUG_INFO, "OEM-PEI-FchPlatformOemPeiInit-End\n"));
  return EFI_SUCCESS;
}

#pragma pack (pop)

