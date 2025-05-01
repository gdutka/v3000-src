/** @file SmmErrorReportOemLib.c

  Null implementation for SmmErrorReportOemLib

  Copyright (C) Microsoft Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Protocol/SmmSupervisorProtocol.h>
#include <Protocol/Cpu.h>

/**
  Interface open to OEM to conduct platform specific error logging.

  @param[in]  CallIndex         CallIndex used to invoke SyscallLib.
  @param[in]  Arg1              Arg1 used to invoke SyscallLib.
  @param[in]  Arg2              Arg2 used to invoke SyscallLib.
  @param[in]  Arg3              Arg3 used to invoke SyscallLib.
  @param[in]  CallerAddr        CallerAddr that points to return address after sysret.

  @retval EFI_SUCCESS           Operation is successful.
  @retval EFI_UNSUPPORTED       This function is not implemented by this platform

**/
EFI_STATUS
EFIAPI
SmmErrorReportOem (
  IN UINTN          CallIndex,
  IN UINTN          Arg1,
  IN UINTN          Arg2,
  IN UINTN          Arg3,
  IN UINTN          CallerAddr
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Interface open to OEM to conduct Interrupt specific error logging when interrupt exception occurs.

  @param[in]  InterruptType      Interrupt Exception number.
  @param[in]  SystemContext      System Context when interrupt occurred.

**/
VOID
EFIAPI
SmmErrorReportInterruptHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  )
{
}

/**
  Address to return to after Interrupt Exception Handler is completed.

  @retval  NULL                  Interrupt does not return. System will hang in debug or reset in release.
  @retval  Non 0                 Interrupt returns to this address. This is for validation.

**/
VOID *
EFIAPI
SmmErrorReportGetExceptionHandlerReturnAddress (
  VOID
  )
{
  return NULL;
}

/**
  Get policy after policy violation is reported.

  @retval  SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT    System will hang in debug or reset in release.
  @retval  SMM_SV_END_OF_POLICY_VIOLATION_ACTION_CONTINUE   System will continue after policy violation for validation.

**/
UINT8
EFIAPI
SmmErrorReportGetEndOfPolicyViolationAction (
  VOID
  )
{
  return SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT;
}

