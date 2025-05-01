/*
*******************************************************************************

Copyright(C) 2023 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#ifndef _SMMSUPV_POST_CODE_H_
#define _SMMSUPV_POST_CODE_H_

/**
 *  SMMSUPV Post Code
 *
 *  These are the values displayed to the user to indicate progress through boot.
 *
 */
typedef enum {
  SMMSUPV_MAIN_ENTRY                                          = 0x0001, ///< Entry point of main
  SMMSUPV_MAIN_ENTRY_EXIT                                     = 0x0002, ///< Exit SmmSupv Entry
  SMMSUPV_INSTALL_SMI_HANDLER                                 = 0x0003, ///< SmmSupv install smi handler
  SMMSUPV_INSTALL_SMI_HANDLER_EXIT                            = 0x0004, ///< Exit SmmSupv install smi handler
  SMMSUPV_SMI_ENTRY                                           = 0x0005, ///< SmmSupv SmiEntry
  SMMSUPV_POST_DRTM_ENTRY                                     = 0x0006, ///< SmmSupv post drtm entry
  SMMSUPV_POST_DRTM_ENTRY_END                                 = 0x0007, ///< SmmSupv post drtm entry end
  SMMSUPV_SMI_EXIT                                            = 0x0008, ///< SmmSupv smi exit
  SMMSUPV_SECURITY_POLICY_CHECK_START                         = 0x0009, ///< SmmSupv smi exit
  SMMSUPV_SECURITY_POLICY_CHECK_END                           = 0x000A, ///< SmmSupv smi exit

  // Error Status
  SMMSUPV_CORESYNC_TIMEOUT_BASE                               = 0x0C00, ///< 0x0C00 - 0xCFF was reserved for CORESYNC_TIMEOUT
                                                                        ///< Core sync fail, format (0x0C00 | CpuIndex)
  SMMSUPV_EXCEPTION_BASE                                      = 0x0D00, ///< 0x0D00 - 0xDFF was reserved for exception postcode
                                                                        ///< format (SMMSUPV_EXCEPTION_BASE | Exception ID)
                                                                        ///< e.g. #define EXCEPT_IA32_INVALID_OPCODE   6
                                                                        ///< The postcode of invalid OP will be 0x0D06 (0x0D00 | 0x06)
  SMMSUPV_ALLOCATE_PAGE_FAIL                                  = 0x0E01, ///< SmmSupv allocate page fail
  SMMSUPV_ALLOCATE_FOR_GLOBAL_FAIL                            = 0x0E02, ///< SmmSupv allocate for GlobalData
  SMMSUPV_INIT_SMM_IDT_FAIL                                   = 0x0E03, ///< SmmSupv Initialize SmmIdt fail
  SMMSUPV_INSTALL_INTERFACE_FAIL                              = 0x0E04, ///< SmmSupv install protocol interface fail
  SMMSUPV_LOCATE_SMIENTRY_FAIL                                = 0x0E05, ///< SmmSupv locate smi entry bin fail
  SMMSUPV_IHV_DRTM_INIT_FAIL                                  = 0x0E06, ///< SmmSupv IhvDrtmInit function fail
  SMMSUPV_REGISTER_READYTOLOCK_CALLBACK_FAIL                  = 0x0E07, ///< SmmSupv register ready to lock callback function fail
  SMMSUPV_ALLOCATE_GDT_BUFFER_FAIL                            = 0x0E08, ///< SmmSupv allocate GDT buffer fail
  SMMSUPV_UNSUPPORTED_SMIENTRY_VERSION                        = 0x0E09, ///< Unsupported Smi Entry version detected
  SMMSUPV_POST_DRTM_INFO_NOT_IDENTICAL                        = 0x0E0A, ///< DrtmInfo is not identical as we report during boottime.
  SMMSUPV_POST_DRTM_SECURITY_CHECK_FAIL                       = 0x0E0B, ///< DrtmCmdLaunch execute fail
  SMMSUPV_POST_STACK_NOT_IDENTICAL                            = 0x0E0C, ///< Stack used in SmmEntry is not identical as we reserved
  SMMSUPV_POST_STACK_OVERLAP                                  = 0x0E0D, ///< CPL0 stack and CPL3 stack overlap
  SMMSUPV_FIND_SUPVDIR_FAIL                                   = 0x0E0E, ///< SmmSupv find SmmSupv directory fail
  SMMSUPV_POST_STACK_CHECK_FAIL                               = 0x0E0F, ///< SmmSupv stack check fail
  SMMSUPV_REINIT_GDT_FAIL                                     = 0x0E10, ///< SmmSupv re-init GDT fail
  SMMSUPV_VERIFY_PAGE_TABLE_FAIL                              = 0x0E11, ///< SmmSupv verify page table fail
  SMMSUPV_ALLOCATE_MEM_POOL_FAIL                              = 0x0E12, ///< SmmSupv allocate protected memory pool fail
  SMMSUPV_DRTM_SYNC_TIMEOUT_FAIL_RESET                        = 0x0E13, ///< Fail to synchronize all threads at DRTM protection SMI
  SMMSUPV_REPORT_DRTM_INFO_FAIL                               = 0x0E14, ///< ReportDrtmInfo return fail
  SMMSUPV_INSTALL_PUBLIC_INFO_TABLE_FAIL                      = 0x0E15, ///< InstallSmmSupvPublicInfoTable fail
  SMMSUPV_LOCATE_PROTOCOL_FAIL                                = 0x0E16, ///< Failed to locate Protocol
  SMMSUPV_CPL0_STACK_TO_GS_FAIL                               = 0x0E17, ///< UpdateCpl0StackPtrForGs fail
  SMMSUPV_MSR_REG_ERROR                                       = 0x0E18, ///< MSR reg error
  SMMSUPV_INIT_SPINLOCK_ERROR                                 = 0x0E19, ///< Init Spin lock error
  SMMSUPV_SYSCALL_FAIL_RESET                                  = 0x0E1A, ///< A failure when executing syscall
  SMMSUPV_TRIGGER_EXCEPTION_FAIL_RESET                        = 0x0E1B, ///< SmmSupv trigger exception
  SMMSUPV_SHADOW_CR3_FAIL                                     = 0x0E1C, ///< Shadow CR3 error

  SMMSUPV_UNINSTALL_PROTOCOL_FAIL                             = 0x0E1E, ///< Failed to Uninstall Protocol
  SMMSUPV_SYSCALL_FAIL                                        = 0x0E1F, ///< A failure when executing syscall
  SMMSUPV_INIT_GDT_TABLE_FAIL                                 = 0x0E20, ///< SmmSupv initialize GDT table fail
  SMMSUPV_GET_PE_SECTION_IN_FV_FAIL                           = 0x0E21, ///< Failed to get SmmSupervisor version: cannot find PE section in the FV
  SMMSUPV_INVALID_DOS_SIGNATURE                               = 0x0E22, ///< Failed to get SmmSupervisor version: invalid Dos signature
  SMMSUPV_INVALID_NT_SIGNATURE                                = 0x0E23, ///< Failed to get SmmSupervisor version: invalid NT signature
  SMMSUPV_INVALID_PE64_SIGNATURE                              = 0x0E24, ///< Failed to get SmmSupervisor version: invalid PE64 signature
  SMMSUPV_INIT_SMM_POLICY_DATA_FAIL                           = 0x0E25, ///< Failed to Init SmmPolicyData
  SMMSUPV_PRE_DRTM_INFO_SECURE_STORE_FAIL                     = 0x0E26, ///< Error occurs during DRTM store process
  SMMSUPV_INIT_EXCEPTION_HANDLELER_SPINLOCK_FAIL              = 0x0E27, ///< Failed to init exception handleler Spinlock
  SMMSUPV_INIT_CPU_EXCEPTION_HANDLELER_FAIL                   = 0x0E28, ///< Failed to init cpu exception handleler
  SMMSUPV_REGISTER_CPU_INTERRUPT_HANDLELER_FAIL               = 0x0E29, ///< Failed to register cpu interrupt handler

  SMMSUPV_MAIN_ENTRY_HAPPEN_FAIL_EVENT                        = 0x0EEE, ///< The sub-function return error status in SmmSupervisorEntry
} SMMSUPV_POST_CODE;

#endif  // _SMMSUPV_POST_CODE_H_
