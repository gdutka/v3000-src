/** @file -- SmmSupvErrorReport.c

This Smm driver will register error reporter to Smm supervisor.

When error occurs, SyscallDispatcher will sysret to this registered.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiSmm.h>
#include <Protocol/SmmSupervisorProtocol.h>
#include <Protocol/SmmCpuService.h>
#include <Library/DebugLib.h>
#include <Library/SysCallLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/SmmErrorReportOemLib.h>
#include <Library/ResetSystemLib.h>

#define SMM_POLICY_SUBCLASS     0x00CE0000
#define SMM_POLICY_VIOLATION    (EFI_SOFTWARE | SMM_POLICY_SUBCLASS | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE)

EFI_SMM_CPU_SERVICE_PROTOCOL    *mMpServices = NULL;
UINT8                           mEndOfPolicyViolationAction = SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT;

VOID
EFIAPI
AsmSmmErrorLogger (
  IN UINTN          Arg0,
  IN UINTN          Arg1,
  IN UINTN          Arg2,
  IN UINTN          Arg3,
  IN UINTN          CallerAddr
  );

/**
  Handler function that report error status code.

  @param[in]  CallIndex         CallIndex used to invoke SyscallLib.
  @param[in]  Arg1              Arg1 used to invoke SyscallLib.
  @param[in]  Arg2              Arg2 used to invoke SyscallLib.
  @param[in]  Arg3              Arg3 used to invoke SyscallLib.
  @param[in]  CallerAddr        CallerAddr that points to return address after sysret.

  @retval EFI_SUCCESS           Device does not have a write in progress and
                                write enable latch is set
  @retval EFI_DEVICE_ERROR      SPI Flash part did not respond properly

**/
VOID
EFIAPI
SmmErrorLogger (
  IN UINTN          CallIndex,
  IN UINTN          Arg1,
  IN UINTN          Arg2,
  IN UINTN          Arg3,
  IN UINTN          CallerAddr
  )
{
  EFI_STATUS  Status;
  UINTN       CpuIndex;

  DEBUG ((DEBUG_ERROR, "%a Enter... CallIndex: %lx, Arg1: %lx, Arg2: %lx, Arg3: %lx, CallerAddr: %p\n",
                      __FUNCTION__,
                      CallIndex,
                      Arg1,
                      Arg2,
                      Arg3,
                      CallerAddr));

  if (NeedSysCall ())  {
    ReportStatusCode ((EFI_ERROR_MAJOR | EFI_ERROR_CODE), SMM_POLICY_VIOLATION);
  }
  else {
    // Why are we even here?
    DEBUG ((DEBUG_ERROR, "%a: This should not happen...\n", __FUNCTION__));
    ASSERT (FALSE);
    goto Done;
  }

  // Allow OEM hook library call here
  Status = SmmErrorReportOem (CallIndex, Arg1, Arg2, Arg3, CallerAddr);
  if (EFI_ERROR (Status)) {
    // Should let the system go ahead and continue resuming
    DEBUG ((DEBUG_ERROR, "%a: OEM error report had error - %r\n", __FUNCTION__, Status));
  }

  // TODO: Set the return status in common buffer if it is valid...

  if (mMpServices == NULL) {
    // Something is wrong about mp services
    DEBUG ((DEBUG_ERROR, "%a: mMpServices is empty, bail here - %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Done;
  }

  Status = mMpServices->WhoAmI (mMpServices, &CpuIndex);
  if (EFI_ERROR (Status)) {
    // Could not even figure out WhoAmI
    DEBUG ((DEBUG_ERROR, "%a: Could not even figure out WhoAmI - %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    goto Done;
  }

Done:
  if (mEndOfPolicyViolationAction == SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT) {
    ResetWarm ();
  }
}

/**
Entry to SmmSupvErrorReport, register SMM error reporter and callback functions

@param[in] ImageHandle                The image handle.
@param[in] SystemTable                The system table.

@retval Status                        From internal routine or boot object, should not fail
**/
EFI_STATUS
EFIAPI
SmmSupvErrorReportEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status             = EFI_SUCCESS;
  SMM_SUPERVISOR_PROTOCOL *mSmmSupvProtocol  = NULL;

  if (!PcdGetBool (PcdSmmIsolationEnable)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a: enter...\n", __FUNCTION__));

  // locate the SMM Supervisor Protocol
  Status = gSmst->SmmLocateProtocol (&gSmmSupervisorProtocolGuid, NULL, (VOID**)&mSmmSupvProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to RSC handler protocol (%r)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Cleanup;
  }

  // locate mMpServices protocol
  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, (VOID**)&mMpServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to locate mMpServices handler protocol (%r)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Cleanup;
  }

  // register OEM Error logger.
  Status = mSmmSupvProtocol->InstallErrorReporter ((VOID *) (UINTN) AsmSmmErrorLogger);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to install OEM Error Reporter (%r)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Cleanup;
  }

  if (mSmmSupvProtocol->Version < SMM_SUPV_PROTOCOL_VER_EXCEPTION_HDLR) {
    // All functionalities are only supported for protocols beyond v3.
    DEBUG ((DEBUG_WARN, "%a Older supervisor detected, the full error report function may not work (v%x)\n", __FUNCTION__, mSmmSupvProtocol->Version));
    Status = EFI_SUCCESS;
    goto Cleanup;
  }

  Status = mSmmSupvProtocol->InstallExceptionHandler (SmmErrorReportInterruptHandler);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to install Smm Error Report Interrupt Handler (%r)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Cleanup;
  }

  if (mSmmSupvProtocol->Version < SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT) {
    // All functionalities are only supported for protocols beyond v4.
    DEBUG ((DEBUG_WARN, "%a Older supervisor detected, the full error report function may not work (v%x)\n", __FUNCTION__, mSmmSupvProtocol->Version));
    Status = EFI_SUCCESS;
    goto Cleanup;
  }

  Status = mSmmSupvProtocol->SetExceptionHandlerReturnAddress (SmmErrorReportGetExceptionHandlerReturnAddress ());
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to set Smm Exception Interrupt Handler Return Address (%r)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Cleanup;
  }

  mEndOfPolicyViolationAction = SmmErrorReportGetEndOfPolicyViolationAction ();
  Status = mSmmSupvProtocol->SetEndOfPolicyViolationAction (mEndOfPolicyViolationAction);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to set Smm Error Report End of Policy Violation Action (%r)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Cleanup;
  }

Cleanup:
  DEBUG ((DEBUG_INFO, "%a: exit (%r)\n", __FUNCTION__, Status));
  return Status;
}
