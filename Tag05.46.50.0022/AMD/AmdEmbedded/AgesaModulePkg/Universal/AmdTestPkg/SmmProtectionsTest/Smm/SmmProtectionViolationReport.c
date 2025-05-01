/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmTestViolationAndExceptionInfo.h>

// Flag to set if SMM Violation Error.
BOOLEAN mAmdTestSmmErrorReportViolationFlag = FALSE;

// Flag to set if SMM Exception Error.
BOOLEAN mAmdTestSmmErrorReportIntFlag = FALSE;

// On exception, Interrupt handler will return to this function.
VOID
AsmInterruptRet (
  VOID
  );

// Reset flags that will be set if SMM Supervisor violation or SMM exception.
VOID
ResetErrorReportFlags (
  VOID
  )
{
  mAmdTestSmmErrorReportViolationFlag = FALSE;
  mAmdTestSmmErrorReportIntFlag = FALSE;
}

// Return TRUE if either SMM Error Report Flag is true.
BOOLEAN CheckErrorReportFlags (VOID)
{
  return mAmdTestSmmErrorReportViolationFlag || mAmdTestSmmErrorReportIntFlag;
}

// Function will be called by SMM Error Report library function SmmErrorReportOem on SMM Supervisor violation through protocol interface.
EFI_STATUS
EFIAPI
AmdTestSmmErrorReportOem (
  IN UINTN          CallIndex,
  IN UINTN          Arg1,
  IN UINTN          Arg2,
  IN UINTN          Arg3,
  IN UINTN          CallerAddr
  )
{
  mAmdTestSmmErrorReportViolationFlag = TRUE;
  return EFI_SUCCESS;
}

// Function will be called by SMM Error Report library function SmmErrorReportInterruptHandler on SMM exception through protocol interface.
VOID
EFIAPI
AmdTestSmmErrorReportInterruptHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  )
{
  mAmdTestSmmErrorReportIntFlag = TRUE;
}

SMM_TEST_VIOLATION_AND_EXCEPTION_INFO_PROTOCOL SmmTestViolationAndExceptionInfoProtocol = {
  AmdTestSmmErrorReportOem,                   // Callback on SMM Supervisor Violation
  AmdTestSmmErrorReportInterruptHandler,      // Callback on SMM Exception
  (VOID*)(UINTN)AsmInterruptRet,                     // Return to this address on SMM Exception IRET.
  SMM_SV_END_OF_POLICY_VIOLATION_ACTION_CONTINUE  // On SMM Supervisor Violation, return to this address.
};


// Install SMM Violation Info Protocol
EFI_STATUS
InstallSmmTestViolationInfoProtocol (
  VOID
  )
{
  EFI_HANDLE  ProtocolHandle = NULL;

  return  gSmst->SmmInstallProtocolInterface (
                    &ProtocolHandle,
                    &gSmmTestViolationAndExceptionInfoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &SmmTestViolationAndExceptionInfoProtocol
                    );
}
