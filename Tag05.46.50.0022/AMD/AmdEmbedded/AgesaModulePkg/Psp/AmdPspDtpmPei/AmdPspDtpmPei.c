/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Ppi/AmdPspDtpmPpi.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Filecode.h>
#include <IndustryStandard/UefiTcgPlatform.h>

#define FILECODE PSP_AMDPSPDTPMPEI_AMDPSPDTPMPEI_FILECODE

STATIC PSP_DTPM_PPI mAmdPspDtpmPpi = {
  PSP_DTPM_PPI_REVISION,
  PspMboxGetDTPMData,
};

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdPspDtpmPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPspDtpmPpiGuid,
  &mAmdPspDtpmPpi
};

/*++

Routine Description:

  PSP dTPM Driver Entry. Install PSP dTPM status service PPI

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
AmdPspDtpmPeiDriverEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  AGESA_TESTPOINT (TpPspdTpmPeiEntry, NULL);

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.DtpmPei Enter\n");

  IDS_HDT_CONSOLE_PSP_TRACE ("\tInstall Psp Dtpm PPI\n");
  Status = (**PeiServices).InstallPpi (PeiServices, &mAmdPspDtpmPpiList);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("InstallPpi fail\n");
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("DtpmPei Exit\n");
  AGESA_TESTPOINT (TpPspdTpmPeiExit, NULL);

  return Status;
}



