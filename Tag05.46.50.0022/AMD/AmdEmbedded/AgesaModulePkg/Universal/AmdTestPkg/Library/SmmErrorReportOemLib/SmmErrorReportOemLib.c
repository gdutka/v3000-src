/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/** @file SmmErrorReportOemLib.c

  Null implementation for SmmErrorReportOemLib

  Copyright (C) Microsoft Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Protocol/SmmSupervisorProtocol.h>
#include <Protocol/Cpu.h>

//test
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SmmTestViolationAndExceptionInfo.h>
#include <Protocol/SmmBase2.h>

SMM_ERROR_REPORT_OEM                mSmmErrorReportOem = NULL;
SMM_ERROR_REPORT_INTERRUPT_HANDLER  mSmmErrorReportInterruptHandler = NULL;
VOID                                *mSmmErrorReportInterruptHandlerReturnAddress = NULL;
UINT8                               mEndOfPolicyViolationAction = SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT;

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
  if (mSmmErrorReportOem == NULL) {
    return EFI_UNSUPPORTED;
  }
  return mSmmErrorReportOem (
          CallIndex,
          Arg1,
          Arg2,
          Arg3,
          CallerAddr
          );
}

VOID
EFIAPI
SmmErrorReportInterruptHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  )
{
  if (mSmmErrorReportInterruptHandler == NULL) {
    return;
  }
  mSmmErrorReportInterruptHandler (InterruptType, SystemContext);
}

VOID *
EFIAPI
//[-start-221104-IB14740270-modify]//
SmmErrorReportGetExceptionHandlerReturnAddress (VOID)
//[-end-221104-IB14740270-modify]//
{
  return mSmmErrorReportInterruptHandlerReturnAddress;
}


UINT8
EFIAPI
SmmErrorReportGetEndOfPolicyViolationAction (
  VOID
  )
{
  return mEndOfPolicyViolationAction;
}

STATIC
EFI_STATUS
InitSmst (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_SMM_BASE2_PROTOCOL  *InternalSmmBase2;

  InternalSmmBase2 = NULL;
  //
  // Retrieve SMM Base2 Protocol,  Do not use gBS from UefiBootServicesTableLib on purpose
  // to prevent inclusion of gBS, gST, and gImageHandle from SMM Drivers unless the
  // SMM driver explicitly declares that dependency.
  //
  Status = SystemTable->BootServices->LocateProtocol (
                                        &gEfiSmmBase2ProtocolGuid,
                                        NULL,
                                        (VOID **)&InternalSmmBase2
                                        );
  ASSERT_EFI_ERROR (Status);
  ASSERT (InternalSmmBase2 != NULL);

  //
  // We are in SMM, retrieve the pointer to SMM System Table
  //
  InternalSmmBase2->GetSmstLocation (InternalSmmBase2, &gSmst);
  ASSERT (gSmst != NULL);

  return EFI_SUCCESS;
}


/**
  The constructor function

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS      The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmErrorReportOemLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  SMM_TEST_VIOLATION_AND_EXCEPTION_INFO_PROTOCOL *SmmTestviolationInfo;
  EFI_STATUS Status;

  if (gSmst == NULL) {
    InitSmst (ImageHandle, SystemTable);
  }

  Status = gSmst->SmmLocateProtocol (&gSmmTestViolationAndExceptionInfoProtocolGuid, NULL, &SmmTestviolationInfo);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mSmmErrorReportOem = SmmTestviolationInfo->SmmErrorReportOem;
  mSmmErrorReportInterruptHandler = SmmTestviolationInfo->SmmErrorReportInterruptHandler;
  mSmmErrorReportInterruptHandlerReturnAddress = SmmTestviolationInfo->SmmErrorReportInterruptHandlerReturnAddress;
  mEndOfPolicyViolationAction = SmmTestviolationInfo->EndOfPolicyViolationAction;

  return EFI_SUCCESS;
}
