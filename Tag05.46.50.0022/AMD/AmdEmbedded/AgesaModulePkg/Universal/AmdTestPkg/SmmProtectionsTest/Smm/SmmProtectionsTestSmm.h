/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#pragma once

#include <PiSmm.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <SmmProtectionsTestCommon.h>

#define RET_OPCODE 0xc3

typedef VOID (*TEST_CASE_FUNC) (SMM_PROT_TEST_EXECUTE_MSG *);
typedef VOID (*TEST_CASE_INTERNAL_FUNC) (VOID);

extern UINT8 *mBootServicesTestData;
extern TEST_CASE_INTERNAL_FUNC mOutsideOfSmmFuncPtr;

VOID
ResetErrorReportFlags (
  VOID
  );

BOOLEAN
CheckErrorReportFlags (
  VOID
  );

typedef enum {
  PROTECTED_INSTRURCTION_CHECK = 0,
  DATA_READ_CHECK = 1,
  EXECUTE_FUNCTION_CHECK = 2
} SMM_PROTECTION_TEST_TYPE;

VOID
AsmSmmProtectionsTestCommon (
  IN SMM_PROTECTION_TEST_TYPE TestType,
  IN VOID *Address
  );

VOID
CopyResponseStrToExecuteBuffer (
  OUT SMM_PROT_TEST_EXECUTE_MSG *Execute,
  IN  CHAR8* ResponseStr);

VOID
SmmCheckAccessNullPointer (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmCheckDataAccessOutsideSmm (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmCheckExecuteInDataRegion (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmCheckExecuteInStackRegion (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmCheckExecuteOutsideOfSmm (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmIsoCheckProtectedInstruction (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmIsoCheckProtectedMsr (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );

VOID
SmmIsoCheckProtectedIo (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *
  );
