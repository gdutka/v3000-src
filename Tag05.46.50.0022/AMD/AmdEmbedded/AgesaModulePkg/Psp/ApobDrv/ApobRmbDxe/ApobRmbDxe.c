/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * APOB driver for RMB system
 *
 * Contains code that public APOB service
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision$   @e \$Date$
 *
 */
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/ApobRmbLib.h>
#include <Protocol/ApobCommonServiceProtocol.h>
#include <Filecode.h>

#define FILECODE PSP_APOBDRV_APOBRMBDXE_APOBRMBDXE_FILECODE

STATIC APOB_COMMON_SERVICE_PROTOCOL mApobCommonServiceProtocol = {
  ApobGetApcbInstanceRmb,
  ApobGetApcbRecoveryFlagRmb,
  ApobGetDimmCfgUpdatedFlagRmb,
  ApobGetPhysCcdNumberRmb,
  ApobGetPhysComplexNumberRmb,
  ApobGetPhysCoreNumberRmb,
  ApobGetIsThreadEnabledRmb,
  ApobGetEccEnableRmb,
  ApobGetNvdimmPresentInSystemRmb,
  ApobGetNvdimmInfoRmb,
  ApobGetMaxDimmsPerChannelRmb,
  ApobGetMaxChannelsPerDieRmb,
  ApobGetMaxChannelsPerSocketRmb,
  ApobGetDimmSmbusInfoRmb,
  ApobGetMemClkFreqRmb,
  ApobGetDdrMaxRateRmb,
  ApobGetMemGenInfoElementRmb,
  ApobGetNumberOfValidThresholdsRmb,
  ApobGetThresholdsRmb,
  ApobGetBoardMaskRmb,
  ApobGetSubProgramRmb,
  ApobGetDimmSpdDataRmb
};


EFI_STATUS
EFIAPI
ApobRmbDxeDriverEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;


  IDS_HDT_CONSOLE_PSP_TRACE ("ApobRmbDxeDriverEntry >>\n");
  Handle = NULL;
  //Install APOB Common service Protocol
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gApobCommonServiceProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mApobCommonServiceProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("ApobRmbDxeDriverEntry <<\n");

  return EFI_SUCCESS;
}


