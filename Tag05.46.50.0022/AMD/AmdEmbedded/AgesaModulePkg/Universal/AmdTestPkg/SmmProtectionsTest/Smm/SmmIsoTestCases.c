/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "SmmProtectionsTestSmm.h"
#include <Library/IoLib.h>

#define MSR_SMM_BASE  0xC0010111
#define CFG_ADDR_PORT 0xCF8

// Check if protected instruction can execute.
VOID
SmmIsoCheckProtectedInstruction (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  // Check if processor instruction is blocked.

  ResetErrorReportFlags ();

  DEBUG ((DEBUG_ERROR, "Check executing protected instructions.\n"));

  AsmSmmProtectionsTestCommon (PROTECTED_INSTRURCTION_CHECK, NULL);

  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Protected instruction not executed.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Protected instruction executed.");
  }
}

// Check if protected MSR can be accessed.
VOID
SmmIsoCheckProtectedMsr (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
// Write to excluded MSR address blocked.
  UINT64 TsegBase;

  ResetErrorReportFlags ();

  DEBUG ((DEBUG_ERROR, "Check executing protected MSRs.\n"));

  TsegBase = AsmReadMsr64 (MSR_SMM_BASE);
  AsmWriteMsr64 (MSR_SMM_BASE, TsegBase);

  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Protected MSR (SMRR_BASE) not accessed.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Protected MSR (SMRR_BASE) accessed.");
  }
}

// Check if protected IO can be accessed.
VOID
SmmIsoCheckProtectedIo (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  // Write to excluded IO address blocked.
  UINT32 IoData;

  ResetErrorReportFlags ();

  DEBUG ((DEBUG_ERROR, "Check executing protected IO.\n"));
  IoData = IoRead32 (CFG_ADDR_PORT);
  IoWrite32 (CFG_ADDR_PORT, IoData);
  if (CheckErrorReportFlags ()) {
    ExecuteMsg->Violation = TRUE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Protected IO (0xCF8) not accessed.");
  } else {
    ExecuteMsg->Violation = FALSE;
    CopyResponseStrToExecuteBuffer (ExecuteMsg, "Protected IO (0xCF8) accessed.");
  }
}
