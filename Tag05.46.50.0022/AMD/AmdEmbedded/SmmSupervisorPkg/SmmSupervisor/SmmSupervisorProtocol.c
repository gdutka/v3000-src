/** @file
Agent Module to load other modules to deploy SMM Entry Vector for X86 CPU.

Copyright (c) 2009 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <SmmSecurePolicy.h>

#include <Protocol/DebugSupport.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>

#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/LocalApicLib.h>
#include <Library/SysCallLib.h>
#include <Library/SmmSupervisorExceptionHandlerLib.h>
#include <Library/SmmPolicyGateLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SmmSupvEntryLocateLib.h>
#include <Library/SmmSupervisorPageMgmtLib.h>
#include <Protocol/SmmSupervisorProtocol.h>

#include "SmmSupervisor.h"

SMI_CPL3_ENTRIES mSmiCpl3Entries;

//
// Variables used by SMI Handler
//
IA32_DESCRIPTOR  mSmiIdtr;
IA32_DESCRIPTOR  *mSmiIdtrPtr;

///
/// Handle for the SMM Supervisor Protocol
///
EFI_HANDLE  mSmmSupervisorHandle = NULL;

//
// External error logger registered during boot time
//
VOID *mExternalErrorLogger = NULL;
UINT8 mEndOfPolicyViolationAction = SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT;

VOID *mSmmEntryAddr = NULL;
STATIC UINT32   mSmmEntrySize  = 0;

extern UINTN   Cpl3HandlerEndStub;
extern UINTN   Cpl3HandlerStub;
extern UINT16  mOemExceptionHandlerFixUpOffset;
extern BOOLEAN mPcdSmmIsolationEnable;

UINT8        *mCpl3HandlerStubBuffer;

EFI_CPU_INTERRUPT_HANDLER   mOemExceptionHandler = NULL;
EFI_GUID                    mSmmSupervisorVersionGuid = {
                                      0xd4adfc6f, 0x2f58, 0x4bcf, {0xa8, 0x87, 0x5, 0xef, 0xb4, 0x7d, 0x42, 0x99}};
                                      // [Guid("D4ADFC6F-2F58-4BCF-A887-05EFB47D4299")]
CHAR8                       mSmmSupVersion[MAX_VERSION_CHAR_COUNT] = {'\0'};

EFI_STATUS
EFIAPI
SmmSupervisorInstallErrorReporter (
  VOID  *ErrorLogger
  );

VOID
EFIAPI
SmmSupervisorInstallSmiHandler (
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

EFI_STATUS
EFIAPI
TurnOnSmmIsoAtEvent (
  IN UINT8 EnableEvent
  );

EFI_STATUS
EFIAPI
InstallExceptionHandler (
  IN EFI_CPU_INTERRUPT_HANDLER  Handler
  );

EFI_STATUS
EFIAPI
SetExceptionHandlerReturnAddress (
  IN VOID *ReturnAddress
  );

EFI_STATUS
EFIAPI
SetEndOfPolicyViolationAction (
  IN UINT8 Action
  );

///
/// SMM MP Protocol instance
///
SMM_SUPERVISOR_PROTOCOL  mSmmSupervisor  = {
  SMM_SUPERVISOR_PROTOCOL_VERSION,
  0,  //SmiHandlerSize
  0,  //Reserved
  SmmSupervisorInstallSmiHandler,
  SmmSupervisorInstallErrorReporter,
  TurnOnSmmIsoAtEvent,
  InstallExceptionHandler,
  SetExceptionHandlerReturnAddress,
  SetEndOfPolicyViolationAction
};

/**
  Locate Smm Entry Binary.

  @retval  EFI_SUCCESS            Find SmmEntry successfully.
  @retval  EFI_OUT_OF_RESOURCES   Find SmmEntry successfully.
  @retval  EFI_NOT_FOUND          The SmmEntry could not be found.
  @retval  EFI_DEVICE_ERROR       A hardware error occurs during reading from the
                                  Firmware Volume.
  @retval  EFI_ACCESS_DENIED      The firmware volume containing the searched
                                  Firmware File is configured to disallow reads.

**/
EFI_STATUS
EFIAPI
LocateSmmEntryBinary (
  VOID        **SmmEntryAddr,
  UINT32      *SmmEntrySize
  )
{
  EFI_STATUS Status;

  VOID   *Section = NULL;
  UINT32 SectionSize;

  if (SmmEntryAddr == NULL || SmmEntrySize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //Find the SmmEntry binary in SMMSUPV directory structure
  Status = FindSmmSupvEntry (SMM_SUPV_CORE_ENTRY_BINARY, &Section, &SectionSize);
  DEBUG ((DEBUG_INFO, "CoreSmmEntry Addr:%x Size:%x\n", Section, SectionSize));

  if (Status != EFI_SUCCESS) {
    WriteErrorData (SMMSUPV_LOCATE_SMIENTRY_FAIL);
    DEBUG ((DEBUG_INFO, "FindSmmSupvEntry returned error %d\n", Status));
    ASSERT (FALSE);
    return Status;
  }
  *SmmEntryAddr = Section;
  *SmmEntrySize = SectionSize;

  return EFI_SUCCESS;
}

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
EFI_STATUS
EFIAPI
SmmSupervisorInstallErrorReporter (
  VOID                                  *ErrorLogger
  )
{
  if (!mPcdSmmIsolationEnable) {
    return EFI_UNSUPPORTED;
  }

  if (mExternalErrorLogger != NULL) {
    DEBUG ((DEBUG_WARN, "%a mExternalErrorLogger has already been populated %p\n", __FUNCTION__, mExternalErrorLogger));
    return EFI_ALREADY_STARTED;
  }

  if (ErrorLogger == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  mExternalErrorLogger = ErrorLogger;
  return EFI_SUCCESS;
}

/**
  Get the version and SPL value of SmmSupervisor driver

  @retval  TRUE    Get SmmSupervisor driver version and SPL value successfully.
  @retval  FALSE   Get SmmSupervisor driver version and SPL value failed.
**/
BOOLEAN
EFIAPI
GetSmmSupervisorVersionAndSplValue (
  IN OUT CHAR8    *Version,
  IN OUT UINT32   *SmmSupSplValue
)
{
  EFI_STATUS  Status;
  VOID                          *LocalRomImage;
  UINTN                         Index;
  EFI_IMAGE_DOS_HEADER          *ImageDosHdr;
  EFI_IMAGE_NT_HEADERS64        *ImageNt64Hdr;

  Status = GetSectionFromAnyFv (
              &gEfiCallerIdGuid,
              EFI_SECTION_PE32,
              0,
              &LocalRomImage,
              &Index
              );
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_GET_PE_SECTION_IN_FV_FAIL);
    DEBUG ((DEBUG_WARN, "Failed to get SmmSupervisor version: cannot find PE section in the FV\n"));
    return FALSE;
  }

  ImageDosHdr = (EFI_IMAGE_DOS_HEADER *)LocalRomImage;
  if (ImageDosHdr->e_magic != EFI_IMAGE_DOS_SIGNATURE) {
    WriteErrorData (SMMSUPV_INVALID_DOS_SIGNATURE);
    DEBUG ((DEBUG_WARN, "Failed to get SmmSupervisor version: invalid Dos signature\n"));
    return FALSE;
  }

  ImageNt64Hdr = (EFI_IMAGE_NT_HEADERS64 *)(((CHAR8 *)ImageDosHdr) + ImageDosHdr->e_lfanew);
  if (ImageNt64Hdr->Signature != EFI_IMAGE_NT_SIGNATURE) {
    WriteErrorData (SMMSUPV_INVALID_NT_SIGNATURE);
    DEBUG ((DEBUG_WARN, "Failed to get SmmSupervisor version: invalid NT signature\n"));
    return FALSE;
  }

  if (ImageNt64Hdr->OptionalHeader.Magic != EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    WriteErrorData (SMMSUPV_INVALID_PE64_SIGNATURE);
    DEBUG ((DEBUG_WARN, "Failed to get SmmSupervisor version: invalid PE64 signature\n"));
    return FALSE;
  }

  AsciiSPrint (Version, MAX_VERSION_CHAR_COUNT, "%d.%04d", ImageNt64Hdr->OptionalHeader.MajorSubsystemVersion, ImageNt64Hdr->OptionalHeader.MinorSubsystemVersion);
  *SmmSupSplValue = ((ImageNt64Hdr->OptionalHeader.MajorImageVersion << 16) | (ImageNt64Hdr->OptionalHeader.MinorImageVersion));
  return TRUE;
}

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
  @param[in] CpuSmmDebugEntryPtr  Function pointer to CpuSmmDebugEntry from PiSmmCpuDxeSmm.
                                  This function sets DR6 & DR7 according to SMM save state,
                                  before running SMM C code.
  @param[in] SmiRendezvousPtr     Function pointer to SmiRendezvous from PiSmmCpuDxeSmm.
                                  C function for SMI entry, each processor comes here upon
                                  SMI trigger.
  @param[in] CpuSmmDebugExitPtr   Function pointer to CpuSmmDebugExit from PiSmmCpuDxeSmm.
                                  This function restores DR6 & DR7 to SMM save state.
  @param[out] SmmEntryAddr        Pointer to returned, allocated and patched SmiEntry, caller
                                  holds responsibility to free buffer.
  @param[out] SmmEntrySize        Pointer to hold SmiEntry size upon return.

**/
VOID
EFIAPI
SmmSupervisorInstallSmiHandler (
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
  )
{
  EFI_STATUS                Status;
  UINT32                    CpuSmiStackCpl3;
  UINT32                    Cpl0Stack;
  PER_CORE_SMI_ENTRY_STRUCT_HDR *SmiEntryStructHdrPtr = NULL;
  UINT32                    SmiEntryStructHdrAddr;
  UINT32                    WholeStructSize;
  EFI_PHYSICAL_ADDRESS      Cpl0GdtTssTable;
  UINTN                     Cpl0GdtTssTableSize;
  UINT16                    *FixStructPtr;
  UINT32                    *Fixup32Ptr;
  UINT64                    *Fixup64Ptr;

  UINT32                    SmmSupSplValue = 0;
  UINTN                     Index;

  if (!mPcdSmmIsolationEnable) {
    *SmmEntryAddr = NULL;
    *SmmEntrySize = 0;
    return;
  }

  TESTPOINT (SMMSUPV_INSTALL_SMI_HANDLER);
  if (SmmEntryAddr == NULL || SmmEntrySize == NULL) {
    ASSERT (FALSE);
    return;
  }

  CpuSmiStackCpl3   = 0;
  *SmmEntryAddr  = NULL;

  Status = AllocateGdtBuffer (CpuIndex, &Cpl0GdtTssTable, &Cpl0GdtTssTableSize);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_ALLOCATE_GDT_BUFFER_FAIL);
    ASSERT (FALSE);
    return;
  }

  // Initialize GDT table with NULL call gate and CPL0 stack pointer
  Status = InitGdtTable (Cpl0GdtTssTable, Cpl0GdtTssTableSize, 0, 0);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_INIT_GDT_TABLE_FAIL);
    ASSERT (FALSE);
    return;
  }

  mGdtrPtr[CpuIndex].Limit = (UINT16) mSmiGdtr.Limit;
  mGdtrPtr[CpuIndex].Base = (UINTN) Cpl0GdtTssTable;

  //
  // Initialize values in template before copy
  //
  CpuSmiStackCpl3 = (UINT32)((UINTN)SmiStack + StackSize - 2 * sizeof (UINTN));

  ASSERT (mSmmEntryAddr != NULL); // mSmmEntryAddr is initialized before protocol is installed.
  ASSERT (mSmmEntrySize != 0);    // mSmmEntrySize is initialized before protocol is installed.

  if (mSmmEntryAddr == NULL || mSmmEntrySize == 0) {
    return;
  }

  *SmmEntryAddr = AllocateCopyPool (mSmmEntrySize, mSmmEntryAddr);
  *SmmEntrySize = mSmmEntrySize;

  // Get Whole structure size
  WholeStructSize = (UINT32)*(EFI_PHYSICAL_ADDRESS *)(UINTN)(((UINTN)*SmmEntryAddr) + *SmmEntrySize - sizeof(UINT32));

  // Get header address
  SmiEntryStructHdrAddr = (UINT32)(((UINTN)*SmmEntryAddr) + *SmmEntrySize - sizeof(UINT32) - WholeStructSize);
  SmiEntryStructHdrPtr = (PER_CORE_SMI_ENTRY_STRUCT_HDR *)(UINTN)(SmiEntryStructHdrAddr);
  if (CpuIndex == 0) {
    DEBUG ((DEBUG_INFO, "SmiEntry binary version: %X.%X, SPL:0x%08X\n", (SmiEntryStructHdrPtr->BinaryVersion >> 8), (SmiEntryStructHdrPtr->BinaryVersion & 0xFF), SmiEntryStructHdrPtr->SplValue));
    if (GetSmmSupervisorVersionAndSplValue (mSmmSupVersion, &SmmSupSplValue) == TRUE) {
      DEBUG ((DEBUG_INFO, "SmmSupervisor driver version: %a, SPL:0x%08X\n", mSmmSupVersion, SmmSupSplValue));
    }
  }
  FixStructPtr = (UINT16 *)(UINTN)(SmiEntryStructHdrAddr + SmiEntryStructHdrPtr->FixUpStructOffset);
  Fixup32Ptr = (UINT32 *)(UINTN)(SmiEntryStructHdrAddr + SmiEntryStructHdrPtr->FixUp32Offset);
  Fixup64Ptr = (UINT64 *)(UINTN)(SmiEntryStructHdrAddr + SmiEntryStructHdrPtr->FixUp64Offset);

  //Do the fixup
  Fixup32Ptr[FIXUP32_STACK_OFFSET_CPL3] = CpuSmiStackCpl3;
  Fixup32Ptr[FIXUP32_STACK_SIZE_CPL3] = (UINT32)StackSize;

  //Allocate SMM CPL0 code stack
  Cpl0Stack =  (UINT32) (UINTN) AllocatePages (EFI_SIZE_TO_PAGES (SMM_CPL0_OVERALL_STACK_SIZE));
  //Log stack for current processor
  mSmmSupvCpuData.StackBase[CpuIndex] = Cpl0Stack;

  Cpl0Stack =  Cpl0Stack + SMM_CPL0_OVERALL_STACK_SIZE - sizeof (UINTN); //Substract UINTN to make SMI entry asm generic
  Fixup32Ptr[FIXUP32_STACK_OFFSET_CPL0] = (UINT32)Cpl0Stack;
  Fixup32Ptr[FIXUP32_CR3_OFFSET] = (UINT32)Cr3;
  Fixup32Ptr[FIXUP32_GDTR] = (UINT32)(UINTN) &mGdtrPtr[CpuIndex];
  Fixup32Ptr[FIXUP32_CONFIG_OFFSET] = 0;
  if (CalculateMaximumSupportAddress () > 48) {
    DEBUG ((DEBUG_INFO, "5 Level Page table Supported\n"));
    Fixup32Ptr[FIXUP32_CONFIG_OFFSET] |= FIXUP32_CONFIG_5LEVEL_PAGE_TABLE;
  }

  // Note: Switch below lines to determine if we want to use our own IDT in its entirety
  mSmiIdtrPtr = (IA32_DESCRIPTOR *) SmiHandlerIdtrPtr;

  Fixup64Ptr[FIXUP64_SMMSUPV_SMIENTRY] = (UINT64) &SmmSupvSmiEntry;
  Fixup64Ptr[FIXUP64_SMMSUPV_SMIEXIT] = (UINT64) &SmmSupvSmiExit;
  Fixup64Ptr[FIXUP64_SMI_HANDLER_IDTR] = (UINT64) mSmiIdtrPtr;
  Fixup64Ptr[FIXUP64_SMI_RDZ_ENTRY] = (UINT64) SmiRendezvousPtr;

  DEBUG ((DEBUG_VERBOSE, "CpuIndex: %d\n", CpuIndex));
  for (Index = 0; Index < SmiEntryStructHdrPtr->FixUp32Num; Index++) {
    DEBUG ((DEBUG_VERBOSE, "Fixup Var %d-0x%x\n", Index, Fixup32Ptr[Index]));
  }

  for (Index = 0; Index < SmiEntryStructHdrPtr->FixUp64Num; Index++) {
    DEBUG ((DEBUG_VERBOSE, "Fixup Addr %d-0x%x\n", Index, Fixup64Ptr[Index]));
  }

  for (Index = 0; Index < (SmiEntryStructHdrPtr->FixUpStructNum * 2); Index += 2) {
    DEBUG ((DEBUG_VERBOSE, "Fixup Offset %d- Size: 0x%x, Offset: 0x%x\n", Index, \
    FixStructPtr[Index], \
    FixStructPtr[Index+1]));
  }

  DEBUG ((DEBUG_VERBOSE, "Cr3 is 0x%x, SmBase is 0x%x, SmiEntryStack is 0x%x, mSmiIdtrPtr is 0x%x, SmiRendezvousPtr is 0x%x\n\n\n",
    Cr3, SmBase, CpuSmiStackCpl3, mSmiIdtrPtr, SmiRendezvousPtr));

  //
  // Set the value at the top of the CPU stack to the CPU Index
  //
  *(UINTN*)(UINTN)CpuSmiStackCpl3 = CpuIndex;

  TESTPOINT (SMMSUPV_INSTALL_SMI_HANDLER_EXIT);
}

/**
  This function pass the parameter to SmmSupervisor if need to turn on SMM isolation at SMM ready to boot
  By default, SMM isolation only be enabled when "DrtmBootFlag" set to TRUE during DRTM boot path.
  This function provide a mechanism to enable the SmmIsolation at BIOS phase regardless DrtmBootFlag set or not.

  @param[in] EnableEvent          Switch to control turn on SMM isolation at SMM ready to boot, TRUE turn on, FALSE turn off

  @retval EFI_SUCCESS             Registration is successful
**/
EFI_STATUS
EFIAPI
TurnOnSmmIsoAtEvent (
  IN UINT8 EnableEvent
  )
{
  if (!mPcdSmmIsolationEnable) {
    return EFI_UNSUPPORTED;
  }

  mSmmIsoEnableEvent = EnableEvent;
  return EFI_SUCCESS;
}

/**
  Install default exception handler.

  @param[in] Handler  Default Handler

  @retval EFI_SUCCESS             Handler installed
  @retval EFI_ALREADY_STARTED     Handler already installed.
**/
EFI_STATUS
EFIAPI
InstallExceptionHandler (
  IN EFI_CPU_INTERRUPT_HANDLER  Handler
  )
{
  EFI_STATUS  Status;
  UINTN       HandlerSize;

  if (!mPcdSmmIsolationEnable) {
    return EFI_UNSUPPORTED;
  }

  if (mOemExceptionHandler != NULL) {
    return EFI_ALREADY_STARTED;
  }

  HandlerSize = (UINTN)&Cpl3HandlerEndStub - (UINTN)&Cpl3HandlerStub;

  // Allocate runtime service code, so the code will be stored in user page.
  Status = gSmst->SmmAllocatePool (
             EfiRuntimeServicesCode,
             HandlerSize,
             &mCpl3HandlerStubBuffer
          );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy Cpl3HandlerStub to User page. Code in this driver is protected from Ring 3.
  CopyMem (mCpl3HandlerStubBuffer, &Cpl3HandlerStub, HandlerSize);

  // Cpl3HandlerStub fix up to call with Oem handler.
  *(UINTN*)(mCpl3HandlerStubBuffer + mOemExceptionHandlerFixUpOffset) = (UINTN)Handler;

  mOemExceptionHandler = Handler; // Store handler, so it can only be installed once.
//[-start-220301-IB14740220-modify]//
  RegisterCpuDefaultInterruptHandler ((EFI_CPU_INTERRUPT_HANDLER)(UINTN)mCpl3HandlerStubBuffer); //Register stub to call default handler.
//[-end-220301-IB14740220-modify]//

  return EFI_SUCCESS;
}

/**
  Sets return address of Interrupt Exception Handler when completed.

    @param[in] ReturnAddress       NULL = No addresses is registered. System will hang/reset depending on debug/release.

    @retval EFI_SUCCESS            Registration is successful

**/
EFI_STATUS
EFIAPI
SetExceptionHandlerReturnAddress (
  IN VOID *ReturnAddress
  )
{
  SetReturnFromExceptionHandlerAddress (ReturnAddress);
  return EFI_SUCCESS;
}

/**
  Sets the action of debug/hang taken after policy violation.

    @param[in] Action           SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT  Release mode - Reset, Debug mode - ASSERT
                                SMM_SV_END_OF_POLICY_VIOLATION_ACTION_CONTINUE For validating policy, continue

    @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetEndOfPolicyViolationAction (
  IN UINT8 Action
  )
{
  if (Action > SMM_SV_END_OF_POLICY_VIOLATION_ACTION_CONTINUE) {
    return EFI_INVALID_PARAMETER;
  }
  mEndOfPolicyViolationAction = Action;

  return EFI_SUCCESS;
 }

/**
  Install the SmmSupv Protocol into SMM protocol database.

  @retval EFI_SUCCESS           The protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES  Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER Handle is NULL.
  @retval EFI_INVALID_PARAMETER Protocol is NULL.
  @retval EFI_INVALID_PARAMETER InterfaceType is not EFI_NATIVE_INTERFACE.
  @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle.

**/
EFI_STATUS
EFIAPI
InstallSmmSupvPublicInterface (
  VOID
)
{
  EFI_STATUS Status;

  Status = LocateSmmEntryBinary (&mSmmEntryAddr, &mSmmEntrySize);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_LOCATE_SMIENTRY_FAIL);
    ASSERT (FALSE);
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Current SmmSupervisor protocol version is 0x%x\n", mSmmSupervisor.Version));
  // Initialize size for SMI handler
  mSmmSupervisor.SmiHandlerSize = mSmmEntrySize;
  DEBUG ((DEBUG_INFO, "%a smi handler size is 0x%x\n", __FUNCTION__, mSmmSupervisor.SmiHandlerSize));

  Status = gSmst->SmmInstallProtocolInterface (
                    &mSmmSupervisorHandle,
                    &gSmmSupervisorProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mSmmSupervisor
                    );
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_INSTALL_INTERFACE_FAIL);
    ASSERT (FALSE);
  }

Done:
  DEBUG ((DEBUG_INFO, "%a Install interface exit with %r\n", __FUNCTION__, Status));
  return Status;
}
