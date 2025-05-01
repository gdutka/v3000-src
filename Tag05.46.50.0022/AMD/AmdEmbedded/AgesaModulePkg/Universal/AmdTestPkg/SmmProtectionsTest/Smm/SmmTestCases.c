/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "SmmProtectionsTestSmm.h"


// Check if can access NULL pointer.
VOID
SmmCheckAccessNullPointer (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  DEBUG ((DEBUG_INFO, "Check NULL pointer access in SMM.\n"));

  ResetErrorReportFlags ();  // Required to clear flags, before test.
  AsmSmmProtectionsTestCommon (DATA_READ_CHECK, NULL);

  if (CheckErrorReportFlags ()) {
      DEBUG ((DEBUG_INFO, "Check error report flag pass.\n"));
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "NULL pointer access blocked SMM.");
  } else {
      DEBUG ((DEBUG_INFO, "Check error report flag Fail.\n"));
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "NULL pointer access succeeded in SMM.");
  }
}

// Check if can access unallowed data outside SMM.
VOID
SmmCheckDataAccessOutsideSmm (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  DEBUG ((DEBUG_INFO, "Check if unallowed data access outside of SMM.\n"));

  ResetErrorReportFlags ();
  AsmSmmProtectionsTestCommon (DATA_READ_CHECK, mBootServicesTestData);

  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Data access outside of SMM failed.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Data access outside of SMM succeeded.");
  }

}

// Check if can execute in data region.
VOID
SmmCheckExecuteInDataRegion (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  UINT8 *AllocateDataRegion;

  DEBUG ((DEBUG_INFO, "Check code execution in data region in SMM.\n"));

  AllocateDataRegion = AllocatePool (1);
  *AllocateDataRegion = RET_OPCODE;

  ResetErrorReportFlags ();
  AsmSmmProtectionsTestCommon (EXECUTE_FUNCTION_CHECK, AllocateDataRegion);

  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Code execution blocked in SMM data region.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Code executed in SMM data region.");
  }

  FreePool (AllocateDataRegion);
}

// Check if can execute in stack region.
VOID
SmmCheckExecuteInStackRegion (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  UINT8 StackRegion = RET_OPCODE;

  DEBUG ((DEBUG_INFO, "Check code execution of stack region in SMM.\n"));

  ResetErrorReportFlags ();
  AsmSmmProtectionsTestCommon (EXECUTE_FUNCTION_CHECK, &StackRegion);

  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Code execution blocked in SMM stack region.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Code executed in SMM stack region.");
  }
}

// Check if can execute outside of SMM.
VOID
SmmCheckExecuteOutsideOfSmm (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  DEBUG ((DEBUG_INFO, "Check if can execute code outside of SMM.\n"));

  ResetErrorReportFlags ();
  AsmSmmProtectionsTestCommon (EXECUTE_FUNCTION_CHECK, (VOID*)(UINTN)mOutsideOfSmmFuncPtr);

  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Code execution blocked outside of SMM.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Code executed outside of SMM.");
  }
}
