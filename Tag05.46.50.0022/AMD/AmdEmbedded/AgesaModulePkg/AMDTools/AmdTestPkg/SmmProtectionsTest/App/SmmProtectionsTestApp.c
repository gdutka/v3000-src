/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>

#include <Protocol/SmmCommunication.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include "SmmProtectionsTestCommon.h"

EFI_SMM_COMMUNICATION_PROTOCOL  *mSmmCommunication;

UINT32 mRequiredCommunicationBufferSize;

#define MAX_CASE_ID_LEN 100

// Prints command line parameters.
VOID
PrintCommandLineParameters (
  VOID
  )
{
  AsciiPrint ("SmmProtectionsTestApp [-d | -e <TestCase> | -a\n");
  AsciiPrint ("     -d Prints test cases\n");
  AsciiPrint ("     -e <TestCase> Executes test case.\n");
  AsciiPrint ("     -a Executes all test cases.\n");
}

// Validates Command Line Parameters
// Returns TRUE if valid.
BOOLEAN ValidateCommandLineParameters (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  // All options have either 1 or 2 parmeters. Note: Argv[0] is application name, and no parmeters Argc is 1.
  if (Argc < 2 || Argc > 3) {
    return FALSE;
  }
  // 1 parameter
  if (Argc == 2) {
    if (StrCmp (Argv[1], L"-d") == 0) {
      return TRUE;
    }
    if (StrCmp (Argv[1], L"-a") == 0) {
      return TRUE;
    }
  }

  //2 parameters
  if (StrCmp (Argv[1], L"-e") == 0) {
    return TRUE;
  }

  return FALSE;
}

// Prints Tests cases
VOID PrintCases (
  OUT SMM_PROT_TEST_EXECUTE_QUERY_MSG *QueryMsg
  )
{
  UINT32 NumCases = QueryMsg->NumCases;
  UINT32 Index;
  UINT32 StrIndex = 0;

  AsciiPrint ("Number of Cases = %d\n", NumCases);
  for (Index = 0; Index < NumCases; Index++) {
    AsciiPrint ("%-40a ", &QueryMsg->CaseInfo[StrIndex]);                    // First string is Test case.
    StrIndex += (UINT32) AsciiStrSize (&QueryMsg->CaseInfo[StrIndex]);      // Skip string including NULL terminator.
    AsciiPrint ("%a\n", &QueryMsg->CaseInfo[StrIndex]);                     // Next string is description
    StrIndex += (UINT32) AsciiStrSize (&QueryMsg->CaseInfo[StrIndex]);
  }
}

// Return the Commbuffer required size. This is required for all messages except getting the required size.
UINT32
GetCommBufferDataSize (
  VOID
  )
{
  EFI_SMM_COMMUNICATE_HEADER      *CommBuffer;
  SMM_PROT_TEST_REQ_MSG_SIZE_MSG  *CommBufferDataSize;
  UINTN   MessageLength  = sizeof (SMM_PROT_TEST_REQ_MSG_SIZE_MSG);
  UINTN   CommSize;
  UINT32  RequiredDataSize;
  EFI_STATUS Status;

  CommSize = sizeof (EFI_SMM_COMMUNICATE_HEADER) - sizeof (CommBuffer->Data) + MessageLength;
  CommBuffer = AllocateReservedPool (CommSize);
  if (CommBuffer == NULL) {
    AsciiPrint ("Out of Memory.\n");
    return 0;
  }

  CommBufferDataSize = (SMM_PROT_TEST_REQ_MSG_SIZE_MSG*)&CommBuffer->Data;

  CopyGuid (&CommBuffer->HeaderGuid, &gSmmProtectionsTestGuid);
  CommBuffer->MessageLength = MessageLength;
  CommBufferDataSize->Header.Function = SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC;

  Status = mSmmCommunication->Communicate (
            mSmmCommunication,
            CommBuffer,
            &CommSize
           );
  if (EFI_ERROR (Status)) {
    AsciiPrint ("Unable to read required Communication Buffer message size.\n");
    RequiredDataSize = 0;
  } else {
    RequiredDataSize = CommBufferDataSize->RequiredMesageSize;
  }

  FreePool (CommBuffer);

  return RequiredDataSize;
}


// Query all the test casses. CommBuffer must be allocated and EFI_SMM_COMMUNICATER_HEADER header fields must be updated.
EFI_STATUS
QueryCases (
  IN OUT EFI_SMM_COMMUNICATE_HEADER  *CommBuffer,
  IN UINTN *CommSize
  )
{
  SMM_PROT_TEST_EXECUTE_QUERY_MSG *QueryMsg;
  EFI_STATUS Status;

  QueryMsg = (SMM_PROT_TEST_EXECUTE_QUERY_MSG*)&CommBuffer->Data;
  QueryMsg->Header.Function = SMM_PROT_TEST_QUERY_FUNC;

  Status =  mSmmCommunication->Communicate (
              mSmmCommunication,
              CommBuffer,
              CommSize
              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return QueryMsg->Header.Status;
}

// Execute specific test case. CommBuffer must be allocated and EFI_SMM_COMMUNICATER_HEADER header fields must be updated.
EFI_STATUS
ExecuteCase (
  IN OUT EFI_SMM_COMMUNICATE_HEADER  *CommBuffer,
  IN UINTN *CommSize,
  IN CHAR8 *TestCaseId
  )
{
  SMM_PROT_TEST_EXECUTE_MSG *ExecuteMsg;
  EFI_STATUS Status;

  ExecuteMsg = (SMM_PROT_TEST_EXECUTE_MSG *)&CommBuffer->Data;
  ExecuteMsg->Header.Function = SMM_PROT_TEST_EXECUTE_FUNC;
  AsciiStrCpyS (&ExecuteMsg->Buffer[0], MAX_CASE_ID_LEN, TestCaseId); // First string is test case. Next string will be updated by Communicate with response.

  Status = mSmmCommunication->Communicate (
                mSmmCommunication,
                CommBuffer,
                CommSize
              );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  return ExecuteMsg->Header.Status;
}

// Query cases and print them.
EFI_STATUS
QueryAndPrintCases (
  VOID
  )
{
  EFI_SMM_COMMUNICATE_HEADER      *CommBuffer;
  UINT64                          CommSize;
  SMM_PROT_TEST_EXECUTE_QUERY_MSG *QueryMsg;
  EFI_STATUS                      Status;

  // Allocate EFI_SMM_COMMUNICATE_HEADER and Query message Buffer and update EFI_SMM_COMMUNICATE_HEADER header.
  CommSize = sizeof (EFI_SMM_COMMUNICATE_HEADER) - sizeof (CommBuffer->Data) + mRequiredCommunicationBufferSize;
  CommBuffer = AllocateReservedPool (CommSize);
  if (CommBuffer == NULL) {
    AsciiPrint ("Out of Memory.\n");
    return EFI_OUT_OF_RESOURCES;
  }

  QueryMsg = (SMM_PROT_TEST_EXECUTE_QUERY_MSG*)&CommBuffer->Data;

  CopyGuid (&CommBuffer->HeaderGuid, &gSmmProtectionsTestGuid);
  CommBuffer->MessageLength = mRequiredCommunicationBufferSize;

  Status = QueryCases (CommBuffer, &CommSize);  // Send Query communication.
  if (EFI_ERROR (Status)) {
    FreePool (CommBuffer);
    return Status;
  }

  PrintCases (QueryMsg);

  FreePool (CommBuffer);
  return EFI_SUCCESS;
}

// Execute a single test case.
EFI_STATUS
TestSingleCase (
  IN CHAR16 *TestCaseId
  )
{
  EFI_SMM_COMMUNICATE_HEADER      *CommBuffer;
  UINT64                          CommSize;
  SMM_PROT_TEST_EXECUTE_MSG       *ExecuteMsg;
  CHAR8                           InputCaseId[MAX_CASE_ID_LEN];
  EFI_STATUS                      Status;

  // Allocate EFI_SMM_COMMUNICATE_HEADER and Execute message Buffer and update EFI_SMM_COMMUNICATE_HEADER header.
  CommSize = sizeof (EFI_SMM_COMMUNICATE_HEADER) - sizeof (CommBuffer->Data) + mRequiredCommunicationBufferSize;
  CommBuffer = AllocateReservedPool (CommSize);
  if (CommBuffer == NULL) {
    AsciiPrint ("Out of Memory.\n");
    return EFI_OUT_OF_RESOURCES;
  }

  CopyGuid (&CommBuffer->HeaderGuid, &gSmmProtectionsTestGuid);
  CommBuffer->MessageLength = mRequiredCommunicationBufferSize;

  ExecuteMsg = (SMM_PROT_TEST_EXECUTE_MSG *)&CommBuffer->Data;

  UnicodeStrToAsciiStrS (TestCaseId, InputCaseId, MAX_CASE_ID_LEN);   // Application input parameter is Unicode, but test case parameter is ASCII.

  Status = ExecuteCase (CommBuffer, &CommSize, InputCaseId);
  if (EFI_ERROR (Status) || EFI_ERROR (ExecuteMsg->Header.Status)) {
    AsciiPrint ("Invalid case.\n");
    FreePool (CommBuffer);
    return Status;
  }

  AsciiPrint ("%a\n", ExecuteMsg->Buffer + AsciiStrSize (ExecuteMsg->Buffer));
    if (!ExecuteMsg->Violation) {
      AsciiPrint ("  Test failed.\n");
    }

  FreePool (CommBuffer);

  return EFI_SUCCESS;
}

// Query all cases and execute all of them.
EFI_STATUS
TestAllCases (
  VOID
  )
{
  EFI_SMM_COMMUNICATE_HEADER      *CommBufferQuery;
  UINT64                          CommSizeQuery;
  SMM_PROT_TEST_EXECUTE_QUERY_MSG *QueryMsg;

  EFI_SMM_COMMUNICATE_HEADER      *CommBufferExecute;
  UINT64                          CommSizeExecute;
  SMM_PROT_TEST_EXECUTE_MSG       *ExecuteMsg;

  UINT32      NumCases;
  UINT32      Index;
  UINT32      CaseIdIndex = 0;
  EFI_STATUS  Status;

  // Allocate EFI_SMM_COMMUNICATE_HEADER and Query message buffer.
  CommSizeQuery = sizeof (EFI_SMM_COMMUNICATE_HEADER) - sizeof (CommBufferQuery->Data) + mRequiredCommunicationBufferSize;
  CommBufferQuery = AllocateReservedPool (CommSizeQuery);
  if (CommBufferQuery == NULL) {
    AsciiPrint ("Out of Memory.\n");
    return EFI_OUT_OF_RESOURCES;
  }

  CopyGuid (&CommBufferQuery->HeaderGuid, &gSmmProtectionsTestGuid);
  CommBufferQuery->MessageLength = mRequiredCommunicationBufferSize;

  Status = QueryCases (CommBufferQuery, &CommSizeQuery);
  if (EFI_ERROR (Status)) {
    FreePool (CommBufferQuery);
    return Status;
  }

  QueryMsg = (SMM_PROT_TEST_EXECUTE_QUERY_MSG*)&CommBufferQuery->Data;
  NumCases = QueryMsg->NumCases;

  // Allocate EFI_SMM_COMMUNICATE_HEADER and Execution message buffer.
  CommSizeExecute = sizeof (EFI_SMM_COMMUNICATE_HEADER) - sizeof (CommBufferExecute->Data) + mRequiredCommunicationBufferSize;
  CommBufferExecute = AllocateReservedPool (CommSizeExecute);
  if (CommBufferExecute == NULL) {
    AsciiPrint ("Out of Memory.\n");
    FreePool (CommBufferQuery);
    return EFI_OUT_OF_RESOURCES;
  }

  CopyGuid (&CommBufferExecute->HeaderGuid, &gSmmProtectionsTestGuid);
  CommBufferExecute->MessageLength = mRequiredCommunicationBufferSize;

  ExecuteMsg = (SMM_PROT_TEST_EXECUTE_MSG*)&CommBufferExecute->Data;

  // Use the same Execution message buffer for each case.

  for (Index = 0; Index < NumCases; ++Index) {
    AsciiPrint ("Test Case: %a\n", &QueryMsg->CaseInfo[CaseIdIndex]);

    Status = ExecuteCase (CommBufferExecute, &CommSizeExecute, &QueryMsg->CaseInfo[CaseIdIndex]);
    if (EFI_ERROR (Status)) {
      FreePool (CommBufferQuery);
      FreePool (CommBufferExecute);
      AsciiPrint ("Status = %r .\n", Status);
      return Status;
    }

    AsciiPrint ("%a\n", ExecuteMsg->Buffer + AsciiStrSize(ExecuteMsg->Buffer)); // Second string is the Executed case response.
    if (!ExecuteMsg->Violation) {
      AsciiPrint ("  Test failed.\n");
    }

    CaseIdIndex += (UINT32) AsciiStrSize (&QueryMsg->CaseInfo[CaseIdIndex]);  // Move Index to Desc.
    CaseIdIndex += (UINT32) AsciiStrSize (&QueryMsg->CaseInfo[CaseIdIndex]);  // Move Index to next Case ID.
  }

  FreePool (CommBufferQuery);
  FreePool (CommBufferExecute);

  return EFI_SUCCESS;
}

INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  EFI_STATUS  Status;
  INTN        RetStatus = 0;
  BOOLEAN     ValidInputParameters;

  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **)&mSmmCommunication);
  if (EFI_ERROR (Status)) {
    AsciiPrint ("SMM Communication protocol isn't installed.\n");
    return 1;
  }

  ValidInputParameters = ValidateCommandLineParameters (Argc, Argv);
  if (!ValidInputParameters) {
    PrintCommandLineParameters ();
    return 1;
  }

  mRequiredCommunicationBufferSize = (UINT32) GetCommBufferDataSize ();
  if (mRequiredCommunicationBufferSize == 0) {
    return 1;
  }

  if (StrCmp (Argv[1], L"-d") == 0) {
    Status = QueryAndPrintCases ();
  } else if (StrCmp (Argv[1], L"-e") == 0) {
    Status = TestSingleCase (Argv[2]);
  } else if (StrCmp (Argv[1], L"-a") == 0) {
    Status = TestAllCases ();
  }

  RetStatus = (EFI_ERROR (Status)) ? 1 : 0;

  return RetStatus;
}
