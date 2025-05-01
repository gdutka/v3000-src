/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#pragma once

//Functions for Communciation messages for APP to communicate with SMM driver.

#define SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC 0     // Get the required Communication message length.
#define SMM_PROT_TEST_QUERY_FUNC            1     // Reports the case id strings and description of all the test cases.
#define SMM_PROT_TEST_EXECUTE_FUNC          2     // Executes test function.

#pragma pack (push, 1)

typedef struct {
  UINT8       Function;                     // Input Function: SMM_PROT_TEST_GET_REG_MSG_SIZE_FUNC, SMM_PROT_TEST_QUERY_FUNC, . . .
  EFI_STATUS  Status;                       // Message return status;
} SMM_PROT_TEST_MSG_HEADER;                 // Common header of all messages.

typedef struct {
  SMM_PROT_TEST_MSG_HEADER Header;
  UINT32  RequiredMesageSize;               // SMM driver returns message size for the rest of the functions.
} SMM_PROT_TEST_REQ_MSG_SIZE_MSG;

typedef struct {
  SMM_PROT_TEST_MSG_HEADER Header;
  UINT32 NumCases;                          // SMM driver returns number of test cases.
  CHAR8  CaseInfo[];                        // SMM driver returns test Case Info in a compressed format of Case ID string followed by Case Description followed by the next case.
} SMM_PROT_TEST_EXECUTE_QUERY_MSG;

typedef struct {
  SMM_PROT_TEST_MSG_HEADER Header;
  BOOLEAN Violation;                        // SMM driver returns TRUE if Violation or Exception.
  //First string in Buffer from SMM Application passed to SMM driver.
  //The next string Buffer is updated by SMM Application returning response.
  CHAR8   Buffer[];

} SMM_PROT_TEST_EXECUTE_MSG;

typedef union {
  SMM_PROT_TEST_MSG_HEADER        Header;
  SMM_PROT_TEST_REQ_MSG_SIZE_MSG  ReqMsgSizeMsg;
  SMM_PROT_TEST_EXECUTE_QUERY_MSG QueryMsg;
  SMM_PROT_TEST_EXECUTE_MSG       ExecuteMsg;
} SMM_PROT_TEST_MSG;


#pragma pack (pop)
