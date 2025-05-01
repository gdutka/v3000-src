/** @file -- SmmSupervisorProtocol.h
Defines a protocol which provides functions to perform smm lock hook and entry installation.

Copyright (C) Microsoft Corporation.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#pragma once

#include <Protocol/Cpu.h>

extern EFI_GUID gSmmSupervisorProtocolGuid;

//=================================================================================================
//
//  Protocol Definition
//
//=================================================================================================

typedef struct  _SMM_SUPERVISOR_PROTOCOL  SMM_SUPERVISOR_PROTOCOL;

#define SMM_SUPERVISOR_PROTOCOL_VERSION         0x00000004

#define SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT     0x00000004
#define SMM_SUPV_PROTOCOL_VER_EXCEPTION_HDLR    0x00000003
#define SMM_SUPV_PROTOCOL_VER_ISOLATION_EVENT   0x00000002
#define SMM_SUPV_PROTOCOL_VER_MIN               0x00000001

// SMM Supervisor version

// Version string definition
// Version = Major.Minor
// Major: major version is one or two digits from 0 to 99
// Minor: minor version is four digits, the first 3 digits are the minor number, the last digit is the flag
//        flag=9 represents RELEASE version, flag=8 represents DEBUG version, flag=0~7 represents test version
// MAX_VERSION_CHAR_COUNT: the maximum character count in the version string of SmmSupervisor driver,
//                         including the null terminator.
#define MAX_VERSION_CHAR_COUNT (2 + 1 + 4 + 1)

#define SMM_SUPERVISOR_VERSION_VARIABLE_NAME  L"SmmSupervisorVersion"
extern EFI_GUID mSmmSupervisorVersionGuid;
extern CHAR8                       mSmmSupVersion[MAX_VERSION_CHAR_COUNT];

/**
  SMM supervisor smi entry installation handler.

  @param[in] CpuIndex             The index of the CPU to install the custom SMI handler.
                                  The value must be between 0 and the NumberOfCpus field
                                  in the System Management System Table (SMST).
  @param[in] SmBase               The SMBASE address for the CPU specified by CpuIndex.
  @param[in] SmiStack             The stack to use when an SMI is processed by the
                                  the CPU specified by CpuIndex.
  @param[in] StackSize            The size, in bytes, if the stack used when an SMI is
                                  processed by the CPU specified by CpuIndex.
  @param[in] Cr3                  The base address of the page tables to use when an SMI
                                  is processed by the CPU specified by CpuIndex.
  @param[in] SmiHandlerIdtrPtr    The base address of the IDT to use when an SMI is
                                  processed by the CPU specified by CpuIndex.
  @param[in] SmiRendezvousPtr     Function pointer to SmiRendezvousEntry from PiSmmCpuDxeSmm.
                                  ASM function for SMI entry, each processor comes here upon
                                  SMI trigger.
  @param[out] SmmEntryAddr        Pointer to returned, allocated and patched SmiEntry, caller
                                  holds responsibility to free buffer.
  @param[out] SmmEntrySize        Pointer to hold SmiEntry size upon return.

**/
typedef
VOID
(EFIAPI *SMM_SV_INSTALL_SMI_HANDLER) (
  IN UINTN    CpuIndex,
  IN UINT32   SmBase,
  IN VOID     *SmiStack,
  IN UINTN    StackSize,
  IN UINT32   Cr3,
  IN UINT64   SmiHandlerGdtBase,
  IN UINT64   SmiHandlerGdtSize,
  IN UINT64   SmiHandlerIdtrPtr,
  IN UINT64   SmiRendezvousPtr,
  OUT UINT8   **SmmEntryAddr,
  OUT UINT32  *SmmEntrySize
  );

/**
  This function is hook point published by SmmSupervisor to allow SMM driver
  to register error handling jump point.

  @param[in] ErrorLogger          Function pointer to error handling return point. At the entry of this
                                  function, CallIndex from syscall instruction will be at RAX, other arguments
                                  will be at corresponding registers native to their compiler's ABI.

  @retval EFI_SUCCESS             Registration is successful
  @retval EFI_ALREADY_STARTED     Registration is rejected due to another driver has registered before this call
  @retval EFI_INVALID_PARAMETER   Registration is rejected due to input pointer is NULL
**/
typedef
EFI_STATUS
(EFIAPI *SMM_SV_INSTALL_ERROR_REPORTER) (
  IN VOID  *ErrorLogger
  );


// Enable SMM Isolation at DRTM TA.
#define SMM_ISO_ENABLE_EVENT_DRTM_TA          0

// Enable SMM Isolation at Ready To Boot.
#define SMM_ISO_ENABLE_EVENT_READY_TO_BOOT    1

/**
  Install default exception handler.

  @param[in] Handler  Default Handler

  @retval EFI_SUCCESS             Handler installed
  @retval EFI_ALREADY_STARTED     Handler already installed.
**/
typedef
EFI_STATUS
(EFIAPI *SMM_SV_INSTALL_EXCEPTION_HANDLER) (
  IN EFI_CPU_INTERRUPT_HANDLER  Handler
  );

/**
  This function pass the parameter to SmmSupervisor if need to turn on SMM isolation at an event such as SMM ready to boot.
  By default, SMM isolation only be enabled when "DrtmBootFlag" set to TRUE during DRTM boot path.
  This function provide a mechanism to enable the SmmIsolation at BIOS phase regardless DrtmBootFlag set or not.

  @param[in] EnableEvent          Switch to control turn on SMM isolation at SMM ready to boot, TRUE turn on, FALSE turn off

  @retval EFI_SUCCESS             Registration is successful
**/
typedef
EFI_STATUS
(EFIAPI *SMM_SV_TURN_ON_SMM_ISO_AT_EVENT) (
  IN UINT8 EnableEvent
  );


/**
  Sets return address of Interrupt Exception Handler when completed.

    @param[in] ReturnAddress       NULL = No addresses is registered. System will hang/reset depending on debug/release.

    @retval EFI_SUCCESS            Registration is successful

**/
typedef
EFI_STATUS
(EFIAPI *SMM_SV_SET_EXCEPTION_HANDLER_RETURN_ADDRESS) (
  IN VOID *ReturnAddress
  );

#define SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT         0   // Release mode - Reset, Debug mode - ASSERT
#define SMM_SV_END_OF_POLICY_VIOLATION_ACTION_CONTINUE        1   // For validating policy, continue

/**
  Sets the action of debug/hang taken after policy violation.

    @param[in] Action           SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT  Release mode - Reset, Debug mode - ASSERT
                                SMM_SV_END_OF_POLICY_VIOLATION_ACTION_CONTINUE For validating policy, continue

    @retval EFI_SUCCESS

**/
typedef
EFI_STATUS
(EFIAPI *SMM_SV_SET_END_OF_POLICY_VIOLATION_ACTION) (
  IN UINT8 Action
  );

/**
  Smm supervisor protocol structure
**/
struct _SMM_SUPERVISOR_PROTOCOL {
  UINT32                                      Version;
  UINT32                                      SmiHandlerSize;                       // Supported since SMM_SUPV_PROTOCOL_VER_MIN
  UINTN                                       ErrorReportJumpBuffer;                // Supported since SMM_SUPV_PROTOCOL_VER_MIN, deprecated since SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT
  SMM_SV_INSTALL_SMI_HANDLER                  InstallSmiHandler;                    // Supported since SMM_SUPV_PROTOCOL_VER_MIN
  SMM_SV_INSTALL_ERROR_REPORTER               InstallErrorReporter;                 // Supported since SMM_SUPV_PROTOCOL_VER_MIN
  SMM_SV_TURN_ON_SMM_ISO_AT_EVENT             TurnOnSmmIsoAtEvent;                  // Supported since SMM_SUPV_PROTOCOL_VER_ISOLATION_EVENT
  SMM_SV_INSTALL_EXCEPTION_HANDLER            InstallExceptionHandler;              // Supported since SMM_SUPV_PROTOCOL_VER_EXCEPTION_HDLR
  SMM_SV_SET_EXCEPTION_HANDLER_RETURN_ADDRESS SetExceptionHandlerReturnAddress;     // Supported since SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT
  SMM_SV_SET_END_OF_POLICY_VIOLATION_ACTION   SetEndOfPolicyViolationAction;        // Supported since SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT
};

