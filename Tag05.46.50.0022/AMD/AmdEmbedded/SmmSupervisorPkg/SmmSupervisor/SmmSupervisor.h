/** @file -- SmmSupervisor.h
Defines necessary structures and constants for SMM supervisor usage.

Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SMM_SUPERVISOR_H_
#define _SMM_SUPERVISOR_H_
#include <Library/SynchronizationLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Library/SmmSupervisorMemMgmtLib.h>
#include <Library/SmmSupervisorPageMgmtLib.h>
#include <Library/SmmSupervisorPolicyMgmtLib.h>
#include <Library/SmmSupvTestPointLib.h>

#include <Protocol/SmmSupervisorProtocol.h>
#include <SmmSecurePolicy.h>

#pragma pack(push,1)
#define MSR_SMM_BASE                   (0xC0010111UL)

//FixupOffsets
#define SMM_ADDR_OFFSET           0
#define GDT_DSEC_OFFSET           1
//
#define FIXUP32_DRTM_BOOT_FLAG            0
#define FIXUP32_GDTR                      1
#define FIXUP32_CR3_OFFSET                2
#define FIXUP32_STACK_OFFSET_CPL3         3
#define FIXUP32_CONFIG_OFFSET             4     ///< bit filed can be find by search FIXUP32_CONFIG_
#define FIXUP32_STACK_OFFSET_CPL0         5
#define FIXUP32_STACK_SIZE_CPL3           6

#define FIXUP64_SMMSUPV_SMIENTRY          0
#define FIXUP64_SMMSUPV_SMIEXIT           1
#define FIXUP64_SMI_RDZ_ENTRY             2
#define FIXUP64_OFFSET3_RSVD              3
#define FIXUP64_OFFSET4_RSVD              4
#define FIXUP64_SMI_HANDLER_IDTR          5
#define FIXUP64_OFFSET6_RSVD              6

#define FIXUP32_CONFIG_5LEVEL_PAGE_TABLE BIT0      ///< 0: 4 level page table, 1: 5 level page table

#define SMM_PSD_OFFSET            0xfb00
// |                                                                                                   |
// |-----                         SMM_CPL0_OVERALL_STACK_SIZE                                      ----|
// |                                                                                                   |
// |--------------------|------------------------------------------------------------------------------|
// |                    |                                                                              |
// |                    |                                                                              |
// |                    |                                                                              |
// |    IST             |                                 Generic Stack                                |
// |                    |                                                                              |
// |SMM_IST_STACK_SIZE  |                                SMM_CPL0_STACK_SIZE                           |
// |                    |                                                                              |
// |                    |                                                                              |
// |--------------------|------------------------------------------------------------------------------|
//                      |                                                                              |
//                      |                                                                              |
//                                                                                                  Stack Top
//                    Ist Stack Top
//               Cpl0Stack -SMM_CPL0_STACK_SIZE                                                     Cpl0Stack

#define SMM_CPL0_OVERALL_STACK_SIZE      (32 * 1024)
#define SMM_IST_STACK_SIZE               (4 * 1024)         ///< The Stack dedicated for exception IST stack
#define SMM_CPL0_STACK_SIZE       (SMM_CPL0_OVERALL_STACK_SIZE - SMM_IST_STACK_SIZE)   ///< Generic Stack for CPL0

#define SMM_READY_TO_LOCK_VALUE   0xFF00000000000000
#define SMM_SUPV_PRE_LOCK_MASK    0xFF
#define SMM_SUPV_PRE_LOCK_VALUE   (SMM_READY_TO_LOCK_VALUE | SMM_SUPV_PRE_LOCK_MASK)
#define SMM_SUPV_AFT_LOCK_MASK    0xFF00
#define SMM_SUPV_AFT_LOCK_VALUE   (SMM_SUPV_PRE_LOCK_VALUE | SMM_SUPV_AFT_LOCK_MASK)
#define SMM_SUPV_DRTM_EVT_MASK    0xFF0000
#define SMM_SUPV_DRTM_EVT_VALUE   (SMM_SUPV_AFT_LOCK_VALUE | SMM_SUPV_DRTM_EVT_MASK)

#define SMM_SAVE_STATE_OFFSET     0xFE00
#define SMM_SAVE_STATE_SIZE       0x0200

#define X64_TSS_RSP0_OFFSET       0x4
#define X64_TSS_IST1_OFFSET       0x24
#define SMMSUPV_STACK_IST_INDEX         2       //The IST allocated for some special Interrupt vector 1 #DB, 3 #BP

//Max support 40 pages for the memory pool, currently on typical system with 12 logical processors, it use around 17 pages,
//but if taking CPU number into consideration, it may use up to 20 pages with 256 logical processors in current code.
//we allocate 40 pages for future extendability
#define MAX_PROTECTED_MEM_POOL_PAGES   40

#define STOP_CODE (((UINT32)0xEFEF)*0x10000ul + \
                    ((__LINE__) % 10) + (((__LINE__ / 10) % 10)*0x10) + \
                    (((__LINE__ / 100) % 10)*0x100) + (((__LINE__ / 1000) % 10)*0x1000))
#define STOP  IoWrite32 (0x80, STOP_CODE);

#define ALIGN_TO_PAGE(Value) ((Value) & ~(EFI_PAGE_SIZE - 1))


typedef struct {
  UINT32  HeaderVersion;     //4 For Version 4 Header
  UINT8   FixUpStructOffset; // Offset to FixUpStruct Arrary
  UINT8   FixUpStructNum;    // number of FixUpStruct Arrary member
  UINT8   FixUp64Offset;     // Offset to FixUp64 Arrary
  UINT8   FixUp64Num;        // Number of FixUp64 Arrary member
  UINT8   FixUp32Offset;     // Offset to FixUp32 Arrary
  UINT8   FixUp32Num;        // Number of FixUp32 Arrary member
  UINT16  BinaryVersion;     // SmiEntry Binary Version
  UINT32  SplValue;          // SPL value for SmiEntry Binary
  UINT32  Reserved;          // Reserved for future use
} PER_CORE_SMI_ENTRY_STRUCT_HDR;

typedef struct  {
  UINT64 CpuSmmDebugEntry;
  UINT64 SmiRendezvous;
  UINT64 CpuSmmDebugExit;
} SMI_CPL3_ENTRIES;

//Parameter structure for SmmSupvSmiEntry
typedef struct {
  UINT64  CpuIndex;
  UINT64  Cpl0Stack;
  UINT64  Cpl3Stack;
  UINT64  Cpl3StackSize;
  UINT64  PerCoreSmiEntryStructAddr;
  UINT64  Cpl0CallGateOffset;
  UINT64  Cpl0StackPoint;
} SMM_SUPV_SMI_ENTRY_PARAS;

typedef struct {
  UINT64  Tablebase;
  UINT64  TableSize;
} SMM_SUPV_GDT_DATA;

//
// Size of Task-State Segment defined in IA32 Manual
//
#define TSS_SIZE              104
#pragma pack(pop)

typedef struct {
  UINT64 BaseAddress;
  UINT64 Length;
  UINT64 Attributes;
} SMM_SUPV_CPL0_REGION;

typedef struct {
  UINTN     NumberOfCpus;
  UINTN     *SmBase;           // Pointer to SmBase array
  UINTN     *StackBase;            // Pointer to Stack base arrary
} SMM_SUPV_CPU_DATA;

typedef struct {
  UINTN PoolBase;
  UINTN PoolSize;
  UINTN CurrentOffset;
} SMM_SUPV_PROTECTED_MEM_POOL;

typedef struct {
  EFI_PHYSICAL_ADDRESS  MmSupvRsp;    // Offset should equal to MM_SUPV_RSP in SysCallEntry.nasm
  EFI_PHYSICAL_ADDRESS  SavedUserRsp; // Offset should equal to SAVED_USER_RSP in SysCallEntry.nasm
  EFI_PHYSICAL_ADDRESS  OsGsBasePtr;
  EFI_PHYSICAL_ADDRESS  OsGsSwapBasePtr;
} MM_SUPV_SYSCALL_CACHE;

EFI_STATUS
EFIAPI
InstallSmmSupvPublicInterface (
  VOID
  );

UINT64
EFIAPI
SmmSupvSmiEntry (
  IN      UINTN SmiEntryParasAddr
  );

UINT64
EFIAPI
SmmSupvSmiExit (
  IN      UINTN                     CpuIndex
  );

VOID
EFIAPI
SyscallCenter (
 );

EFI_STATUS
EFIAPI
AllocateGdtBuffer (
  IN  UINT64                 CpuIndex,
  OUT EFI_PHYSICAL_ADDRESS  *Cpl0GdtTssTable,
  OUT UINTN                 *Cpl0GdtTssTableSize
  );

EFI_STATUS
EFIAPI
InitGdtTable (
  IN EFI_PHYSICAL_ADDRESS Cpl0GdtTssTable,
  IN UINTN TableSize,
  IN UINTN Cpl0CallGateOffset,
  IN UINTN Cpl0StackPoint
  );

EFI_STATUS
EFIAPI
GetGdt (
  IN     UINT64 CpuIndex,
  IN OUT EFI_PHYSICAL_ADDRESS *GdtTablebase,
  IN OUT UINTN *TableSize
  );

/**
  Search module name by input IP address and output it.

  @param CallerIpAddress   Caller instruction pointer.

**/
VOID
DumpModuleInfoByIp (
  IN  UINTN              CallerIpAddress
  );

VOID
InitializeSmmIdt (
  IA32_DESCRIPTOR          *NewIdtr
);

VOID
DumpSmmPolicyData (
  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *Data
  );

/**
  This function set SMI entry to R/W
**/
VOID
SetSmmBaseWriteAttribute (
  VOID
  );

/**
  This function set SMI entry to RO
**/
VOID
ClearSmmBaseWriteAttribute (
  VOID
  );

/**
  This function updates mememory attributes for all CPL0 regions
**/
VOID
SetMemoryAttributesForCpl0Regions (
  SMM_SUPV_CPL0_REGION *Cpl0Regions,
  UINTN                NumOfRegion
  );

VOID
PatchSmmSupvPageTbl (
  SMM_SUPV_CPL0_REGION *Cpl0Regions,
  UINTN                NumOfRegion
  );

VOID
DumpMemPolicyData (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *MemoryPolicy,
  UINT32 Count
  );

/**
  Allocates memory buffer.

  Allocates the number bytes specified by AllocationSize (64KB at most) of type EfiReservedMemoryType and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateSmmSupvPool (
  IN UINTN  AllocationSize
  );


/**
  Allocates a small amount of buffer and zero the memory.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateZeroSmmSupvPool (
  IN UINTN  AllocationSize
  );

/**
  Initialize Timer for SMM AP Sync.

**/
VOID
InitializeSmmTimer (
  VOID
  );

/*
  This is a helper function to allow processor to evaluate whether this
  synchronization needs to wait longer instead of normal timeout
*/
BOOLEAN
EFIAPI
NeedsExtendedWait (
  IN UINT32           DrtmBootFlag
);

/**
  Start Timer for SMM AP Sync.

**/
UINT64
EFIAPI
StartSyncTimer (
  VOID
  );

/**
  Check if the SMM AP Sync timer is timeout.

  @param Timer  The start timer from the begin.

**/
BOOLEAN
EFIAPI
IsSyncTimerTimeout (
  IN      UINT64                    Timer,
  IN      UINT32                    DrtmBootFlag
  );

extern IA32_DESCRIPTOR                mSmiIdtr;
extern IA32_DESCRIPTOR                *mSmiIdtrPtr;
extern IA32_DESCRIPTOR                mSmiGdtr;
extern SMM_SUPV_GDT_DATA              *mSmmSupvGdtData;
extern SPIN_LOCK                      *mCpuToken;
extern volatile UINT64                mSmmSupervisorPostReadyToLock;
extern SMM_SUPERVISOR_PROTOCOL        mSmmSupervisor;
extern SMM_SUPV_CPU_DATA              mSmmSupvCpuData;
extern IA32_DESCRIPTOR                *mGdtrPtr;
extern UINT64                         mIDTBaseBuffer[];
extern VOID                           *mExternalErrorLogger;
extern volatile UINT32                mThreadCount;
extern volatile BOOLEAN               mBspInSmm;
extern volatile UINT32                mSyscallViolationCnt;
extern VOID                           *mSmmEntryAddr;
extern UINT8                          mSmmIsoEnableEvent;
extern SMM_SUPV_SECURE_POLICY_DATA_V1_0 *mSmmTempPolicyData;
extern SMM_SUPV_SECURE_POLICY_DATA_V1_0 *mSmmPolicyData;
extern UINT8                           mEndOfPolicyViolationAction;
#endif // _SMM_SUPERVISOR_H_
