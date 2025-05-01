/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#pragma once

#include <Protocol/Cpu.h>
#include <Protocol/SmmSupervisorProtocol.h>

extern EFI_GUID gSmmTestViolationAndExceptionInfoProtocolGuid;

typedef
EFI_STATUS
(EFIAPI *SMM_ERROR_REPORT_OEM) (
  IN UINTN          CallIndex,
  IN UINTN          Arg1,
  IN UINTN          Arg2,
  IN UINTN          Arg3,
  IN UINTN          CallerAddr
  );

typedef
VOID
(EFIAPI *SMM_ERROR_REPORT_INTERRUPT_HANDLER) (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  );


typedef struct {
  SMM_ERROR_REPORT_OEM                SmmErrorReportOem;
  SMM_ERROR_REPORT_INTERRUPT_HANDLER  SmmErrorReportInterruptHandler;
  VOID                                *SmmErrorReportInterruptHandlerReturnAddress;
  UINT8                               EndOfPolicyViolationAction;
} SMM_TEST_VIOLATION_AND_EXCEPTION_INFO_PROTOCOL;

