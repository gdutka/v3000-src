/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "SmmProtectionsTestSmm.h"
#include "SmmProtectionsTestCommon.h"
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>


EFI_STATUS InstallSmmTestViolationInfoProtocol (VOID);

UINT8 *mBootServicesTestData;                   // Data allocated as Boot Services. This should not be accessible.
TEST_CASE_INTERNAL_FUNC mOutsideOfSmmFuncPtr;   // SMM Functions outside of SMM should not be executable.

typedef struct {
  CHAR8           *CaseId;      // Test case ID.
  CHAR8           *Desc;        // Desciption of Case.
  TEST_CASE_FUNC  TestCaseFunc; // Test case function to be executed.
} TEST_CASES;


CONST TEST_CASES mTestCases[] = {
  {"SMM_NULL_POINTER",                  "Check NULL pointer access from SMM.",            SmmCheckAccessNullPointer},
  {"SMM_UNALLOWED_DATA_OUTSIDE_SMM",    "Check unallowed data access outside of SMM.",    SmmCheckDataAccessOutsideSmm},
  {"SMM_EXECUTION_DATA_REGION",         "Check code execution of data region in SMM.",    SmmCheckExecuteInDataRegion},
  {"SMM_ECECUTION_STACK_REGION",        "Check code execution of stack region in SMM.",   SmmCheckExecuteInStackRegion},
  {"SMM_EXECUTION_OUTSIDE_SMM",         "Check code execution of memory outside of SMM.", SmmCheckExecuteOutsideOfSmm},

  {"SMM_ISO_PROT_INST",                 "Check executing protected instructions.",        SmmIsoCheckProtectedInstruction},
  {"SMM_ISO_MSR_CHECK",                 "Check executing protected MSRs.",                SmmIsoCheckProtectedMsr},
  {"SMM_ISO_IO_CHECK",                  "Check executing protected IO.",                  SmmIsoCheckProtectedIo}
};

const UINT32 mNumTestCases = sizeof (mTestCases) / sizeof (TEST_CASES);


typedef struct {
  UINT32 CaseIdLen;  // Size of CaseId String including NULL terminator.
  UINT32 DescLen;    // Size of Case Description including NULL terminator.
} TEST_CASE_STR_LEN_CACHE;


// This allocated on entry to cache string lenghts of casses for optimization.
TEST_CASE_STR_LEN_CACHE *mTestCaseStrLenCache = NULL;

// Required Communication Buffer Data Size. This is detemined by the buffer size of the largest message.
UINT32 mRequiredCommBufferDataSize = 0;


// Initialize mTestCaseStrLenCache and mRequiredCommBufferDataSize.
VOID
InitTestCaseStrLenAndCommBuferSize (
  VOID
  )
{
  UINT32 Index;

  mTestCaseStrLenCache = (TEST_CASE_STR_LEN_CACHE*)AllocatePool (mNumTestCases * sizeof (TEST_CASE_STR_LEN_CACHE));

  for (Index = 0; Index < mNumTestCases; Index++) {
    mTestCaseStrLenCache[Index].CaseIdLen = (UINT32) AsciiStrSize (mTestCases[Index].CaseId); //Includes NULL terminator.
    mTestCaseStrLenCache[Index].DescLen   = (UINT32) AsciiStrSize (mTestCases[Index].Desc);

    mRequiredCommBufferDataSize += mTestCaseStrLenCache[Index].CaseIdLen;
    mRequiredCommBufferDataSize += mTestCaseStrLenCache[Index].DescLen;
  }

  // mRequiredCommBufferDataSize is the total Case ID strings and Desciption strings including NULL terminators and Query Message Header.
  mRequiredCommBufferDataSize += sizeof (SMM_PROT_TEST_EXECUTE_QUERY_MSG);
}

// Help function to copy test case response tring to Execute Message Buffer.
VOID
CopyResponseStrToExecuteBuffer (
  OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg,
  IN CHAR8* ResponseStr
  )
{
  CHAR8 *ResponseStrStart;
  UINTN MaxStringSize;

  // Execute Buffer constists of input Case ID string followed by output of case response string.

  // Skip case id string.
  ResponseStrStart = ExecuteMsg->Buffer + AsciiStrSize (ExecuteMsg->Buffer); //Includes NULL terminator.

  // Check if buffer is large enough.
  MaxStringSize = mRequiredCommBufferDataSize - ((UINTN)ResponseStrStart - (UINTN)ExecuteMsg);
  AsciiStrCpyS (ResponseStrStart, MaxStringSize, ResponseStr);
}


// Fill Case Query Message buffer with number of cases, Case ID, and Description.
EFI_STATUS
FillTestCaseInfoBuffer (
  OUT SMM_PROT_TEST_EXECUTE_QUERY_MSG *QueryMsg
  )
{
  UINT32 Index;
  CHAR8* CaseInfoBuffer = QueryMsg->CaseInfo;
  UINTN  RemainingStringSize = mRequiredCommBufferDataSize - ((UINTN)CaseInfoBuffer - (UINTN)QueryMsg);

  QueryMsg->NumCases = mNumTestCases;

  for (Index = 0; Index < mNumTestCases; Index++) {
    if (mTestCaseStrLenCache[Index].CaseIdLen > RemainingStringSize) {
      return EFI_BUFFER_TOO_SMALL;
    }
    // First string is Case ID.
    AsciiStrCpyS (CaseInfoBuffer, mTestCaseStrLenCache[Index].CaseIdLen, mTestCases[Index].CaseId);  //Note: Size is already checked above. Destmax in AsciiStrCatS only needs to be large enough.
    CaseInfoBuffer += mTestCaseStrLenCache[Index].CaseIdLen;
    RemainingStringSize -=mTestCaseStrLenCache[Index].CaseIdLen;

    if (mTestCaseStrLenCache[Index].DescLen > RemainingStringSize) {
      return EFI_BUFFER_TOO_SMALL;
    }
    // Second string is Description.
    AsciiStrCpyS (CaseInfoBuffer, mTestCaseStrLenCache[Index].DescLen, mTestCases[Index].Desc);
    CaseInfoBuffer += mTestCaseStrLenCache[Index].DescLen;
    RemainingStringSize -=mTestCaseStrLenCache[Index].DescLen;
  }

  return EFI_SUCCESS;
}

// Find test case and execute
EFI_STATUS
ExecuteFunction (
  IN OUT SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg
  )
{
  UINT32 Index;

  CHAR8 *CaseId = ExecuteMsg->Buffer;
  UINT32 CaseIdStrLen = (UINT32) AsciiStrSize (CaseId);   // Case ID string length will be use to optmize searching.

  for (Index = 0; Index < mNumTestCases; Index++) {
    // If Case ID string length doesn't match, skip. If string matches, execute function.
    if (mTestCaseStrLenCache[Index].CaseIdLen == CaseIdStrLen && AsciiStrCmp (mTestCases[Index].CaseId, CaseId) == 0) {
      // Test case will update Execute message if test case if had violation as expected and response string.
      mTestCases[Index].TestCaseFunc (ExecuteMsg);
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

// This is called on SMI when SMM Protections Test Application send messages to get required communication buffer size, query tests cases, and execute test cases.
EFI_STATUS
EFIAPI
SmmProtectionsTestHandler (
  IN     EFI_HANDLE                   DispatchHandle,
  IN     CONST VOID                   *RegisterContext,
  IN OUT VOID                         *CommBuffer,
  IN OUT UINTN                        *CommBufferSize
)
{
  SMM_PROT_TEST_MSG   *CommBufferMsg = (SMM_PROT_TEST_MSG*)CommBuffer;
  UINT8               Function = CommBufferMsg->Header.Function;
  EFI_STATUS          Status;

  ASSERT (*CommBufferSize > 0);

  // Function SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC is special. This must be called to get the required Communication buffer size for the rest of the functions.
  // NOTE: Current Combuffer size is based on Query returning the largest amount of information.
  if (Function == SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC) {
    if (*CommBufferSize < sizeof (SMM_PROT_TEST_REQ_MSG_SIZE_MSG)) {
      // Can't update "CommBufferMsg->ReqMsgSizeMsg.Header.Status" as it may not be allocated.
      return EFI_INVALID_PARAMETER;
    }
    CommBufferMsg->ReqMsgSizeMsg.Header.Status = EFI_SUCCESS;
    CommBufferMsg->ReqMsgSizeMsg.RequiredMesageSize = mRequiredCommBufferDataSize;
    return EFI_SUCCESS;
  }

  // For all functions except SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC,
  // Communication buffer size should be at least the size returned from function SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC.
  if (*CommBufferSize < mRequiredCommBufferDataSize) {
    // Can't update "CommBufferMsg->ReqMsgSizeMsg.Header.Status" as it may not be allocated.
    return EFI_INVALID_PARAMETER;
  }

  CommBufferMsg->ReqMsgSizeMsg.Header.Status = EFI_SUCCESS;

  switch (Function) {
  case SMM_PROT_TEST_QUERY_FUNC:
    FillTestCaseInfoBuffer (&CommBufferMsg->QueryMsg);
    return EFI_SUCCESS;
  case SMM_PROT_TEST_EXECUTE_FUNC:
    Status = ExecuteFunction (&CommBufferMsg->ExecuteMsg);
    if (EFI_ERROR (Status)) {
      CommBufferMsg->ReqMsgSizeMsg.Header.Status = EFI_INVALID_PARAMETER;
    }
    return EFI_SUCCESS;
  }

  CommBufferMsg->ReqMsgSizeMsg.Header.Status = EFI_INVALID_PARAMETER;
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
SmmProtectionsTestSmmEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
)
{
  EFI_HANDLE  RegHandle;
  EFI_STATUS  Status;

  if ((PcdGetBool(PcdAmdSmmProtectionsTestSupport)) != TRUE)
    return EFI_UNSUPPORTED;
  InitTestCaseStrLenAndCommBuferSize ();

  Status = gBS->AllocatePool (
                EfiBootServicesData,
                sizeof (*mBootServicesTestData),
                (VOID**) &mBootServicesTestData
                );
  ASSERT_EFI_ERROR (Status);
  *mBootServicesTestData = 1;

  Status = gBS->AllocatePool (
                EfiBootServicesCode,  // This is for a function, code is so correct paging is set.
                sizeof (UINT8),
                (VOID**) &mOutsideOfSmmFuncPtr
                );
  ASSERT_EFI_ERROR (Status);

  // Create a function outside of SMM for test case.
  *(UINT8*)(UINTN)mOutsideOfSmmFuncPtr = RET_OPCODE;

  // Install SMM Test Violation Info Protocol, this is used for the SmmErrorReportOemLib,
  //  so that the SMM Supervisor Error Report driver can report violations to SMM driver.
  Status = InstallSmmTestViolationInfoProtocol ();
  ASSERT_EFI_ERROR (Status);

  return gSmst->SmiHandlerRegister(
             SmmProtectionsTestHandler,
             &gSmmProtectionsTestGuid,
             &RegHandle
             );
}
