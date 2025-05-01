/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <PiDxe.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/IdsLib.h>
#include <Library/TimerLib.h>
#include <Protocol/FchSmmSwDispatch2.h>
#include <AmdPspSmmCommunication.h>
#include <Protocol/FchSmmMiscDispatch.h>
#include <Library/S3BootScriptLib.h>
#include <Protocol/SmmVariable.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <FchRegistersCommon.h>
#include <Protocol/PspMboxSmmBufferAddressProtocol.h>
#include "AmdPspIntrusionDetectionCommon.h"
#include <Library/SmmMemLib.h>
#include <Library/AmdFtpmLib.h>
#include <Library/ResetSystemLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPINTRUSIONDETECTION_AMDPSPINTRUSIONDETECTIONSMM_FILECODE

//
// Reset control register values
//
#define FULLRESET       0x0E
#define HARDRESET       0x06
#define SOFTRESET       0x04
#define FULLSTARTSTATE  0x0A
#define HARDSTARTSTATE  0x02
#define SOFTSTARTSTATE  0x0

#define SUS_S3          0x0C00U     // S3
#define SUS_S5          0x1400U     // S5
#define SLP_TYPE        0x1C00U     // MASK
#define SLP_EN          0x2000U     // BIT13

BOOLEAN                             mIntrusionDetectionShutdownSystem;
BOOLEAN                             mIntrusiondetectionClearTPM;
BOOLEAN                             mIntrusionDetectionLogEvent;
UINT8                               mSystemTpmConfig;
UINT8                               *mPspMboxSmmBuffer = NULL;
BOOLEAN                             *mPspMboxSmmFlagAddr = NULL;
BOOLEAN                             mSmmEndOfDxeEventEnter = FALSE;
EFI_SMM_VARIABLE_PROTOCOL           *mSmmVariable;

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
  ResetShutdown ();
}

VOID
ColdResetSystem (
  )
{
  ResetCold ();
}

UINT32
GetIntrusionVariable (
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;
  UINT32                              IntrusionDetectionFlag = INTRUSION_VARIABLE_INITIALIZED;
  BufferSize = sizeof (UINT32);
  Status = mSmmVariable->SmmGetVariable (
                  AMD_INTRUSION_DETECTION_VARIABLE_NAME,
                  &mAmdPspIntrusionDetectionFlagGuid,
                  NULL,
                  &BufferSize,
                  &IntrusionDetectionFlag
                  );
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
  Status = mSmmVariable->SmmSetVariable (
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
DoIntrusionDetectionAction (
  )
{
  EFI_STATUS      Status;
  UINT32          IntrusionVariable;

  AGESA_TESTPOINT (TpPspDoIntrusionDetectionActionSmm, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("DoIntrusionDetectionAction Smm\n");
  ClearIntruderAlert ();
  if (mIntrusionDetectionLogEvent) {
    Status = PspMboxBiosCmdNotifyIntrusionEvent (CASE_PANEL_OPENED_0, mPspMboxSmmBuffer, mPspMboxSmmFlagAddr);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Failed to report intrusion event to PSP. Status: %r\n", Status);
      AGESA_TESTPOINT (TpPspIntrusionEventLoggingError, NULL);
    }
    if (!mIntrusiondetectionClearTPM && !mIntrusionDetectionShutdownSystem) {
      ReArmIntruderAlert ();
    }
  }

  if (mIntrusiondetectionClearTPM) {
    // check if there is any TPM selected
    if (mSystemTpmConfig != SYSTEM_TPM_CONFIG_NONE) {
      IntrusionVariable = GetIntrusionVariable ();
      IDS_HDT_CONSOLE_PSP_TRACE ("DoIntrusionDetectionAction SMM IntrusionVariable:0x%x\n", IntrusionVariable);
      ASSERT (IntrusionVariable != INTRUSION_VARIABLE_NOT_EXIST);
      IntrusionVariable |= INTRUSION_VARIABLE_CLEAR_TPM_IN_PROGRESS;
      Status = SetIntrusionVariable (IntrusionVariable);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Failed to set intrusion variable. Status: %r\n", Status);
        return Status;
      }
      ColdResetSystem ();
    }
  }

  if (mIntrusionDetectionShutdownSystem) {
    PowerOffSystem ();
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
IntrusionDetectionSmiCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT   *MiscRegisterContext
  )
{
  AGESA_TESTPOINT (TpPspIntrusionDetectionSmiCallback, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("IntrusionDetectionSmiCallback\n");
  return DoIntrusionDetectionAction ();
}

EFI_STATUS
EnableIntruderAlertSmi (
  VOID
  )
{
  FCH_SMM_MISC_DISPATCH_PROTOCOL    *AmdFchSmmMiscDispatch;
  EFI_HANDLE                        MiscHandle;
  FCH_SMM_MISC_REGISTER_CONTEXT     MiscRegisterContext;
  EFI_STATUS                        Status;
  UINT32                            OrMask;
  UINT32                            AndMask;

  //
  // Enable Intruder Alert SMI [3:2] = 1
  //
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) &= ~(BIT3 + BIT2);
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) |= BIT2;
  OrMask = BIT2;
  AndMask = (UINT32) ~(BIT3 + BIT2);
  S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32,
                                  (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4), &OrMask, &AndMask);

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
  MiscRegisterContext.SmiStatusBit = BIT17;
  MiscRegisterContext.Order        = 0x80;

  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmMiscDispatchProtocolGuid,
                    NULL,
                    &AmdFchSmmMiscDispatch
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = AmdFchSmmMiscDispatch->Register (
                                    AmdFchSmmMiscDispatch,
                                    &IntrusionDetectionSmiCallback,
                                    &MiscRegisterContext,
                                    &MiscHandle
                                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitGlobalVariables (
  )
{
  EFI_STATUS                          Status;
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;
  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmVariableProtocolGuid,
                    NULL,
                    (VOID **)&mSmmVariable
                    );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to locate gEfiSmmVariableProtocolGuid. Status = %r\n", Status);
    return Status;
  }

  // Save PspMboxSmmBuffer to global variable
  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
  if (Status == EFI_SUCCESS) {
    mPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
    mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
    IDS_HDT_CONSOLE_PSP_TRACE ("mPspMboxSmmBuffer 0x%x\n", mPspMboxSmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("mPspMboxSmmFlagAddr 0x%x\n", mPspMboxSmmFlagAddr);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to locate gPspMboxSmmBufferAddressProtocolGuid\n");
    return EFI_UNSUPPORTED;
  }

  mIntrusionDetectionLogEvent       = PcdGetBool (PcdAmdPspLogEventIfIntrusionDetected);
  mIntrusiondetectionClearTPM       = PcdGetBool (PcdAmdPspClearTpmIfIntrusionDetected);
  mIntrusionDetectionShutdownSystem = PcdGetBool (PcdAmdPspPowerOffSystemIfIntrusionDetected);
  mSystemTpmConfig                  = PcdGet8 (PcdAmdPspSystemTpmConfig);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD PSP Intrusion Detection SMM driver
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
AmdPspIntrusionDetectionSmmEntry (
  IN      EFI_HANDLE                  ImageHandle,
  IN      EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                         Status;

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspIntrusionDetectionSmmEntry Enter\n");
  if (!PcdGetBool (PcdAmdPspIntrusionDetectionEnabled)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Intrusion Detection feature not enabled, exit Intrusion Smm driver\n");
    return EFI_UNSUPPORTED;
  }
  AGESA_TESTPOINT (TpPspIntrusionDetectionSmmEntry, NULL);
  InitGlobalVariables ();

  //
  // check Intruder Alert status
  //
  if (IsIntrusionDetected ()) {
     IDS_HDT_CONSOLE_PSP_TRACE ("Intrusion event is detected in Intrusion SMM driver entry!\n");
    DoIntrusionDetectionAction ();
  }

  Status = EnableIntruderAlertSmi ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Failed to enable Intruder Alert Smi\n");
    return Status;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspIntrusionDetectionSmmEntry Exit\n");
  AGESA_TESTPOINT (TpPspIntrusionDetectionSmmExit, NULL);
  return EFI_SUCCESS;
}
