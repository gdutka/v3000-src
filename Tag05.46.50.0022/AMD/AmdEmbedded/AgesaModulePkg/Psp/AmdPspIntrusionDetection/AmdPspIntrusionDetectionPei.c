/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "Uefi.h"
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <FchRegistersCommon.h>
#include <Library/IdsLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPINTRUSIONDETECTION_AMDPSPINTRUSIONDETECTIONPEI_FILECODE

/**
 * @brief re-accept IntruderAlert and its status will be refreshed
 *
 */
VOID
ReArmIntruderAlert (
  )
{
  // unset the clear bit to wait for next IntruderAlert
  ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5A) &= ~BIT1;
}

/*++

Routine Description:

  PSP Driver Entry. Initialize Intrusion Detection Pei driver

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
AmdPspIntrusionDetectionPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspIntrusionDetectionPeiEntry Enter\n");
  if (!PcdGetBool (PcdAmdPspIntrusionDetectionEnabled)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Intrusion Detection feature not enabled, exit Intrusion Pei driver\n");
    return EFI_UNSUPPORTED;
  }
  AGESA_TESTPOINT (TpPspIntrusionDetectionPeiEntry, NULL);
  ReArmIntruderAlert ();

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspIntrusionDetectionPeiEntry Exit\n");
  AGESA_TESTPOINT (TpPspIntrusionDetectionPeiExit, NULL);
  return EFI_SUCCESS;
}



