/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/IdsLib.h>
#include <Library/AmdFtpmLib.h>
#include <Library/TimerLib.h>
#include <Protocol/Tcg2Protocol.h>
#include <Library/AmdTpm2CommandLib.h>
#include <FchRegistersCommon.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmdPspIntrusionLogProtocol.h>
#include "AmdPspIntrusionDetectionCommon.h"
#include <Filecode.h>

#define FILECODE PSP_AMDPSPINTRUSIONDETECTION_AMDPSPINTRUSIONDETECTIONDXE_FILECODE

AMD_PSP_INTRUSION_LOG_PROTOCOL mAmdPspIntrusionLogProtocol;

BOOLEAN
IsIntrusionDetected (
  )
{
  volatile UINT8 IntruderControl;
  IntruderControl = ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5A);
  if ((IntruderControl & BIT2) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 * @brief clear the IntruderAlert status and IntruderAlert bit will be on hold
 *
 * @return VOID no return value
 */
VOID
ClearIntruderAlert (
  )
{
  // enable  internal pullup and set the clear bit to clear the IntruderAlert status
  ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5A) |= (BIT0 + BIT1);
  MicroSecondDelay (1);
}

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

VOID
PowerOffSystem (
  )
{
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

VOID
ColdResetSystem (
  )
{
  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
}

UINT32
GetIntrusionVariable (
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;
  UINT32                              IntrusionDetectionFlag = INTRUSION_VARIABLE_INITIALIZED;
  BufferSize = sizeof (UINT32);
  Status = gRT->GetVariable (
                  AMD_INTRUSION_DETECTION_VARIABLE_NAME,
                  &mAmdPspIntrusionDetectionFlagGuid,
                  NULL,
                  &BufferSize,
                  &IntrusionDetectionFlag
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("GetIntrusionVariable Status = %r\n", Status);
  if (Status == EFI_NOT_FOUND) {
    return INTRUSION_VARIABLE_NOT_EXIST;
  } else {
    return IntrusionDetectionFlag;
  }
}

EFI_STATUS
EFIAPI
SetIntrusionVariable (
  IN UINT32 IntrusionVariable
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;

  BufferSize = sizeof (UINT32);
  Status = gRT->SetVariable (
                AMD_INTRUSION_DETECTION_VARIABLE_NAME,
                &mAmdPspIntrusionDetectionFlagGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                BufferSize,
                &IntrusionVariable
                );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Fail to set intrusion variable. Status = %r\n", Status);
  }
  return Status;
}

EFI_STATUS
EFIAPI
ReportIntrusionConfigToPsp (
  )
{
  EFI_STATUS                          Status;
  UINT32                              IntrusionCountermeasure;
  UINT32                              IntrusionVariable;
  BOOLEAN                             FirstBoot = FALSE;

  IntrusionVariable = GetIntrusionVariable ();
  if (IntrusionVariable == INTRUSION_VARIABLE_NOT_EXIST) {
    FirstBoot = TRUE;
    //
    // For first boot, clear Intrusder status and initialize intrusion variable
    //
    IDS_HDT_CONSOLE_PSP_TRACE ("First boot, initialize Intrusder status and intrusion variable\n");
    ClearIntruderAlert ();
    ReArmIntruderAlert ();

    // Init variable
    Status = SetIntrusionVariable (INTRUSION_VARIABLE_INITIALIZED);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Failed to initialize intrusion variable. Status = %r\n", Status);
      return Status;
    }
    IntrusionVariable = GetIntrusionVariable ();
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("ReportIntrusionConfigToPsp IntrusionVariable:0x%x\n", IntrusionVariable);

  IntrusionCountermeasure = DETECT_INTRUSION;
  // if it's first boot, PSP will initilize intrusion log
  if (FirstBoot) {
    IntrusionCountermeasure |= INIT_INTRUSION_DETECTION;
  }

  if (PcdGetBool (PcdAmdPspLogEventIfIntrusionDetected) == TRUE) {
    IntrusionCountermeasure |= LOG_EVENT;
  }

  if (PcdGetBool (PcdAmdPspClearTpmIfIntrusionDetected) == TRUE) {
    IntrusionCountermeasure |= CLEAR_TPM;
  }

  if (PcdGetBool (PcdAmdPspPowerOffSystemIfIntrusionDetected) == TRUE) {
    IntrusionCountermeasure |= POWER_OFF_SYSTEM;
  }

  Status = PspMboxBiosCmdCfgIntrusionDetection (IntrusionCountermeasure);
  return Status;
}

/**
 * @brief Intrusion detection callback to clear TPM2
 *    TPM2 can only be cleared after gEfiTcg2ProtocolGuid is installed and
 *    before TPM_CC_HierarchyChangeAuth is called
 * @param Event        The event this callback is based on
 * @param Context      Event context
 */
VOID
EFIAPI
IntrusionDetectionTpm2ClearCallback (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  EFI_STATUS                  Status;
  UINT32                      IntrusionVariable;
  EFI_TCG2_PROTOCOL           *Tcg2Protocol;

  AGESA_TESTPOINT (TpPspIntrusionDetectionClearTpm, NULL);
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  IntrusionVariable = GetIntrusionVariable ();
  IDS_HDT_CONSOLE_PSP_TRACE ("IntrusionDetectionTpm2ClearCallback IntrusionVariable:0x%x\n", IntrusionVariable);
  ASSERT (IntrusionVariable != INTRUSION_VARIABLE_NOT_EXIST);
  if ((IntrusionVariable & INTRUSION_VARIABLE_CLEAR_TPM_IN_PROGRESS) != 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Intrusiondetection Clear TPM\n");
    // clear TPM
    Status = gBS->LocateProtocol(&gEfiTcg2ProtocolGuid, NULL, (VOID **)&Tcg2Protocol);
    if (EFI_ERROR(Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE("Sorry, failed to locate Tcg2 Protocol. Status: %r\n", Status);
      return;
    } else {
      Status = Tpm2Clear (Tcg2Protocol);
      if (EFI_ERROR(Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE("Sorry, failed to clear TPM. Status: %r\n", Status);
        return;
      }
    }

    IntrusionVariable = IntrusionVariable & ~INTRUSION_VARIABLE_CLEAR_TPM_IN_PROGRESS;
    Status = SetIntrusionVariable (IntrusionVariable);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Failed to set intrusion variable. Status: %r\n", Status);
      return;
    }

    if (PcdGetBool (PcdAmdPspPowerOffSystemIfIntrusionDetected) == TRUE) {
      PowerOffSystem ();
    } else {
      ReArmIntruderAlert ();
      // according to TPM specification, additional boot cycle is required after TPM clear.
      // if we don't have to shutdown, we should reset the system to continue the boot sequence.
      ColdResetSystem ();
    }
  }
}

EFI_STATUS
EFIAPI
IntrusionDetectionTpm2Clear (
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  EFI_STATUS          CalledStatus = EFI_SUCCESS;
  EFI_TCG2_PROTOCOL   *Tcg2Protocol;
  EFI_EVENT           Tpm2ClearEvent;
  VOID                *Registration;

  IDS_HDT_CONSOLE_PSP_TRACE ("IntrusionDetectionTpm2Clear Enter\n");
  CalledStatus = gBS->LocateProtocol(&gEfiTcg2ProtocolGuid, NULL, (VOID **)&Tcg2Protocol);
  if (EFI_ERROR(CalledStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE(
      "Tcg2 Protocol has not been installed (Status: %r), install protocol notification\n", Status);

    CalledStatus = gBS->CreateEventEx (
        EVT_NOTIFY_SIGNAL,
        TPL_NOTIFY,
        IntrusionDetectionTpm2ClearCallback,
        NULL,
        NULL,
        &Tpm2ClearEvent
        );
    ASSERT (CalledStatus == EFI_SUCCESS);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    CalledStatus = gBS->RegisterProtocolNotify (
        &gEfiTcg2ProtocolGuid,
        Tpm2ClearEvent,
        &Registration
        );
    ASSERT (CalledStatus == EFI_SUCCESS);
    Status = (CalledStatus > Status) ? CalledStatus : Status;
  } else {
    IntrusionDetectionTpm2ClearCallback (NULL, NULL);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("IntrusionDetectionTpm2Clear Exit. Status: %r\n", Status);
  return Status;
}

EFI_STATUS
EFIAPI
CheckIntrusionLogs (
  )
{
  EFI_STATUS                          Status;
  INTRUSION_EVENT_LOGS                IntrusionEventLogs;
  EFI_HANDLE                          Handle;

  Status = PspMboxBiosCmdGetIntrusionLog (&IntrusionEventLogs);
  // if intrusion log is non-existent or invalid,
  // AGESA will notify SBIOS to show warning message to the user.
  // BIOS should stop the post after user acknowlege the message
  if (Status == EFI_NOT_FOUND) {
    mAmdPspIntrusionLogProtocol.IntrusionLogStatus = INTRUSION_LOG_STATUS_BAD;
  } else if (Status == EFI_SUCCESS) {
    mAmdPspIntrusionLogProtocol.IntrusionLogStatus = INTRUSION_LOG_STATUS_GOOD;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to get intrusion event logs. Status = %r\n", Status);
    return Status;
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdPspIntrusionLogProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAmdPspIntrusionLogProtocol
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to install gAmdPspIntrusionLogProtocolGuid. Status = %r\n", Status);
    ASSERT (FALSE);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Install Protocol gAmdPspIntrusionLogProtocolGuid!\n");
  }
  return Status;
}

EFI_STATUS
DoIntrusionDetectionAction (
  )
{
  EFI_STATUS      Status;
  UINT32          IntrusionVariable;
  BOOLEAN         ClearTpmRequired = FALSE;

  AGESA_TESTPOINT (TpPspDoIntrusionDetectionActionDxe, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("DoIntrusionDetectionAction Dxe\n");
  ClearIntruderAlert ();
  if (PcdGetBool (PcdAmdPspLogEventIfIntrusionDetected) == TRUE) {
    Status = PspMboxBiosCmdNotifyIntrusionEvent (CASE_PANEL_OPENED_0, NULL, NULL);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Failed to report intrusion event to PSP. Status: %r\n", Status);
      AGESA_TESTPOINT (TpPspIntrusionEventLoggingError, NULL);
    }
    if (!PcdGetBool (PcdAmdPspClearTpmIfIntrusionDetected) &&
         !PcdGetBool (PcdAmdPspPowerOffSystemIfIntrusionDetected)) {
      ReArmIntruderAlert ();
    }
  }

  if (PcdGetBool (PcdAmdPspClearTpmIfIntrusionDetected) == TRUE) {
    // check if there is any TPM selected
    if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_NONE) {
      ClearTpmRequired = TRUE;

      IntrusionVariable = GetIntrusionVariable ();
      IDS_HDT_CONSOLE_PSP_TRACE ("DoIntrusionDetectionAction DXE IntrusionVariable:0x%x\n", IntrusionVariable);
      ASSERT (IntrusionVariable != INTRUSION_VARIABLE_NOT_EXIST);
      IntrusionVariable |= INTRUSION_VARIABLE_CLEAR_TPM_IN_PROGRESS;
      Status = SetIntrusionVariable (IntrusionVariable);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Failed to set intrusion variable. Status: %r\n", Status);
        return Status;
      }
      Status = IntrusionDetectionTpm2Clear ();
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Failed to clear TPM\n");
        return Status;
      }
    }
  }
  if (ClearTpmRequired == FALSE) {
    if (PcdGetBool (PcdAmdPspPowerOffSystemIfIntrusionDetected) == TRUE) {
      PowerOffSystem ();
    }
  }
  return EFI_SUCCESS;
}

/**
 * Entry point of the AMD PSP Intrusion Detection DXE driver
 *
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdPspIntrusionDetectionDxeEntry (
  IN      EFI_HANDLE                  ImageHandle,
  IN      EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                          Status;

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspIntrusionDetectionDxeEntry Enter\n");
  if (!PcdGetBool (PcdAmdPspIntrusionDetectionEnabled)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Intrusion Detection feature not enabled, exit Intrusion Dxe driver\n");
    return EFI_UNSUPPORTED;
  }
  AGESA_TESTPOINT (TpPspIntrusionDetectionDxeEntry, NULL);
  Status = ReportIntrusionConfigToPsp ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to report Intrusion Detection configuration to PSP\n");
    return EFI_DEVICE_ERROR;
  }

  // Clear TPM if required, this is in case TPM has not been clear in last boot
  Status = IntrusionDetectionTpm2Clear ();
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to clear TPM\n");
    return Status;
  }

  Status = CheckIntrusionLogs ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to check Intrusion Detection logs\n");
    AGESA_TESTPOINT (TpPspIntrusionGetEventLogError, NULL);
  }

  //
  // check Intruder Alert status
  //
  if (IsIntrusionDetected ()) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Intrusion event is detected in Intrusion DXE driver entry!\n");
    DoIntrusionDetectionAction ();
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspIntrusionDetectionDxeEntry Exit\n");
  AGESA_TESTPOINT (TpPspIntrusionDetectionDxeExit, NULL);
  return Status;
}
