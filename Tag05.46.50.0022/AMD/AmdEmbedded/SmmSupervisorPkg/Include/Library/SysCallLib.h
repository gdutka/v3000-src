/** @file
  Provides function interface to perform syscall.

Copyright (C) Microsoft Corporation.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __SYS_CALL_LIB__
#define __SYS_CALL_LIB__

#define CPL_BITMASK             (BIT1 | BIT0)
#define SYSCALL_REQUIRED_CPL    3

//======================================================================================
//
// Define syscall method
//
//======================================================================================
///
/// To keep the enum value consistant, please explicitly specify the value for each enum item;
/// if you add/remove/update any enum item, please also add/remove/update related information in SyscallIdNamePairs array
///
typedef enum {
  SMM_SC_RDMSR            = 0,
  SMM_SC_WRMSR            = 1,
  SMM_SC_CLI              = 2,
  SMM_SC_IO_READ          = 3,
  SMM_SC_IO_WRITE         = 4,
  SMM_SC_WBINVD           = 5,
  SMM_SC_HLT              = 6,
  SMM_SC_SVST_READ        = 7,
  SMM_SC_IHV_SUPV_READ    = 8,
  SMM_SC_IHV_SUPV_WRITE   = 9,
} SMM_SYS_CALL;

typedef struct {
  SMM_SYS_CALL        Id;
  CHAR8               *Name;
} SYS_CALL_ID_NAME_PAIR;

STATIC SYS_CALL_ID_NAME_PAIR SyscallIdNamePairs [] = {
  //  Id,                     Name
  {  SMM_SC_RDMSR,           "SMM_SC_RDMSR"     },
  {  SMM_SC_WRMSR,           "SMM_SC_WRMSR"     },
  {  SMM_SC_CLI,             "SMM_SC_CLI"       },
  {  SMM_SC_IO_READ,         "SMM_SC_IO_READ"   },
  {  SMM_SC_IO_WRITE,        "SMM_SC_IO_WRITE"  },
  {  SMM_SC_WBINVD,          "SMM_SC_WBINVD"    },
  {  SMM_SC_HLT,             "SMM_SC_HLT"       },
  {  SMM_SC_SVST_READ,       "SMM_SC_SVST_READ" },
  {  SMM_SC_IHV_SUPV_READ,   "SMM_SC_IHV_SUPV_READ"},
  {  SMM_SC_IHV_SUPV_WRITE,  "SMM_SC_IHV_SUPV_WRITE"},
};

UINT64
EFIAPI
SysCall (
  UINTN CallIndex,
  UINTN Arg1,
  UINTN Arg2,
  UINTN Arg3
  );

/**
 Check if high privilege instruction need go through Syscall


 @param  NONE

 @return TRUE  Syscall required
 @return FALSE Syscall not required

**/
BOOLEAN
NeedSysCall (
    VOID
  );

#endif // !defined (__SYS_CALL_LIB__)
