/** @file
Agent Module to load other modules to deploy SMM Entry Vector for X86 CPU.

Copyright (c) 2009 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Pi/PiFirmwareFile.h>

#include <Protocol/DebugSupport.h>
#include <Protocol/LoadedImage.h>

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmSupervisorExceptionHandlerLib.h>
#include <Library/CpuLib.h>
#include <Library/PciLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmSupervisorProtocol.h>
#include <Protocol/SmmVariable.h>
#include <Library/SmmSupervisorPageMgmtLib.h>
#include <Library/IhvPageSupervisionLib.h>
#include <Library/IhvDrtmLib.h>
#include <Library/SmmSupvEntryLocateLib.h>
#include <Library/IhvPageProtectionLib.h>
#include <Guid/SmmSupervisorPublicInfoTable.h>
#include "SmmSupervisor.h"

VOID
EFIAPI
InternalX86Retf (
  VOID
  );

#define MSR_STAR_ADDRESS                0xC0000081
#define MSR_STAR64_ADDRESS              0xC0000082
#define MSR_CSTAR_ADDRESS               0xC0000083
#define MSR_IA32_GS_BASE                0xC0000101
#define MSR_IA32_KERNEL_GS_BASE         0xC0000102
#define LONG_MODE_CS                    0x38
#define COMP_MODE_CS_R3                 0x4B
#define MAX_SMM_SUPV_CPL0_REGIONS       0x400

#define ACPI_MMIO_BASE                  0xFED80000ul
#define SMI_BASE                        0x200        // DWORD
#define PMIO_BASE                       0x300        // DWORD
#define FCH_SMI_REG80                   0x80         // SmiStatus0
#define FCH_SMI_REG84                   0x84         // SmiStatus1
#define FCH_SMI_REG88                   0x88         // SmiStatus2
#define FCH_SMI_REG90                   0x90         // SmiStatus4
#define FCH_SMI_REG94                   0x94         // SmiPointer
#define FCH_SMI_REG10                   0x10         // SmiSciStatus

///
/// Handle for the SMM CPU Protocol
/// TODO: This should rather be in a library, if we open source it
///
extern EFI_HANDLE                      mSmmSupervisorHandle;

SMM_SUPV_GDT_DATA               *mSmmSupvGdtData = NULL;
EFI_MP_SERVICES_PROTOCOL        *mMpService;
volatile UINT64                 mSmmSupervisorPostReadyToLock = 0;
SPIN_LOCK                       *mCpuToken = NULL;
STATIC UINT64                   *mMsrStarValue = NULL;
STATIC UINT64                   *mMsrStar64Value = NULL;
STATIC UINT64                   *mMsrCstarValue = NULL;
STATIC MM_SUPV_SYSCALL_CACHE    *mMmSupvGsStore = NULL;
SMM_SUPV_CPU_DATA               mSmmSupvCpuData;

extern SMM_SUPERVISOR_PROTOCOL  mSmmSupervisor;
SMM_SUPV_CPL0_REGION            mSmmSupvCpl0Regions [MAX_SMM_SUPV_CPL0_REGIONS];
SMM_SUPV_PROTECTED_MEM_POOL     mProtectedMemPool;
volatile BOOLEAN                mPageTblPatchComplete = FALSE;
volatile BOOLEAN                mPageTblPatchComplete1 = FALSE;
volatile UINT32                 mGdtTableUpdateComplete = 0;
volatile UINT32                 mThreadCount = 0;
UINT32                          mThreadsInSmm;
volatile BOOLEAN                mBspInSmm = FALSE;
volatile UINT32                 mSyscallViolationCnt = 0;
IA32_DESCRIPTOR                 *mGdtrPtr = NULL;
UINT8                           mSmmIsoEnableEvent = SMM_ISO_ENABLE_EVENT_DRTM_TA;
BOOLEAN                         mPcdSmmIsolationEnable;
EFI_BOOT_MODE                   mBootMode = BOOT_WITH_FULL_CONFIGURATION;

/**
  Return aligned size where base is required to align to the bottom of a page, but the top is required to align to the top of a page.

  @param[in]  Base    Unaligned Base
  @param[in]  Size    Unalinged Size

  @retval     UINTN   Aligned Size
**/
UINTN
AlignSizeToPage (
  IN UINTN Base,
  IN UINTN Size
  )
{
  UINTN AlignedBase = ALIGN_TO_PAGE (Base);
  UINTN AdjSize = Base + Size - AlignedBase;

  return ALIGN_VALUE (AdjSize, EFI_PAGE_SIZE);
}

/**
  Report Drtm information to secure processor.

  @param  ImageHandle   ImageHandle of the loaded driver.
  @param  SystemTable   Pointer to the EFI System Table.

  @retval  EFI_SUCCESS            Report Drtm information to secure processor successfully.
  @retval  EFI_OUT_OF_RESOURCES   Find smm supervisor successfully.
  @retval  EFI_NOT_FOUND          The smm supervisor could not be found.
  @retval  EFI_DEVICE_ERROR       A hardware error occurs during reading from the
                                  Firmware Volume.
  @retval  EFI_ACCESS_DENIED      The firmware volume containing the searched
                                  Firmware File is configured to disallow reads.
**/
EFI_STATUS
EFIAPI
ReportDrtmInfo (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage;
  INTN                            Instance;
  VOID                            *Buffer;
  UINTN                           Size;
  VOID                            *SmmSupvDirBuffer;
  UINTN                           SmmSupvDirSize;
  EFI_GUID                        SmmSupvDirFileGuid = SMM_SUPV_DIR_FILE_GUID;
  VOID                            *SmmIsolationPolicy;
  UINT32                          SmmIsolationPolicySize;
  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *SmmPolicyData;

  //Below information need be reported, SmmSupvAddr, SmmSupvDirAddr, SmmPolicyDataAddr
  //SmmSupvAddr can be retrived from ImageHandle

  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  &LoadedImage
                  );
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_LOCATE_PROTOCOL_FAIL);
    ASSERT (FALSE);
    goto Exit;
  }

  //
  //Loop all FV to find the FV with SmmSupv directory
  //
  Instance  = 0;
  Buffer    = NULL;
  Size      = 0;
  Status = GetSectionFromAnyFv  (
             &SmmSupvDirFileGuid,
             EFI_SECTION_RAW,
             0,
             &Buffer,
             &Size
             );
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_LOCATE_SMIENTRY_FAIL);
    ASSERT (FALSE);
    goto Exit;
  }
  //Copy to SMMRam, the original buffer will be free at end of post
  SmmSupvDirSize = Size;
  SmmSupvDirBuffer = AllocateZeroSmmSupvPool (SmmSupvDirSize);
  if (SmmSupvDirBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem (SmmSupvDirBuffer, Buffer, SmmSupvDirSize);

  // Try to locate static OEM secure policy (IO and MSR)
  Status = FindSmmSupvEntry (SMM_SUPV_OEM_SECURE_POLICY, &SmmIsolationPolicy, &SmmIsolationPolicySize);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_LOCATE_SMIENTRY_FAIL);
    DEBUG ((DEBUG_ERROR, "Find SMM policy returned error %r\n", Status));
    goto Exit;
  }

  // InitializePolicyData function call modifies data returned by function SmmPolicyData.
  Status = InitializePolicyData (SmmIsolationPolicy, SmmIsolationPolicySize);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_INIT_SMM_POLICY_DATA_FAIL);
    goto Exit;
  }

  SmmPolicyData = GetSmmPolicyData ();

  Status = PreDrtmInfoSecureStore (
                                  (UINTN) LoadedImage->ImageBase,
                                  LoadedImage->ImageSize,
                                  (UINTN) SmmSupvDirBuffer,
                                  SmmSupvDirSize,
                                  (UINTN) SmmPolicyData,
                                  SMM_POLICY_DATA_SIZE,
                                  (UINTN) &SmmSupvSmiEntry,
                                  (UINTN) &SmmSupvSmiExit
                                  );
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_PRE_DRTM_INFO_SECURE_STORE_FAIL);
    DEBUG ((DEBUG_ERROR, "[%a] PreDrtmInfoSecureStore return status: %r\n", __FUNCTION__, Status));
  }

Exit:
  if (SmmIsolationPolicy != NULL) {
    FreePool (SmmIsolationPolicy);
  }

  if (Buffer != NULL) {
    FreePool (Buffer);
  }

  return Status;
}

/**
  Initialize registers in supervisor smi entry.

  @param  CpuIndex    The index of the processor.

**/
VOID
SmmSupvSmiEntryRegInit (
  IN        UINT64                     CpuIndex
 )
{
  UINT32 Eax;
  UINT32 Edx;

  if (mMsrStarValue == NULL || mMsrStar64Value == NULL || mMsrCstarValue == NULL) {
    WriteErrorData (SMMSUPV_MSR_REG_ERROR);
    ASSERT (FALSE);
    return;
  }

  Eax = 0;
  Edx = 0;
  mMsrStarValue[CpuIndex] = (UINT64)AsmReadMsr64 (MSR_STAR_ADDRESS);
  Eax = mMsrStarValue[CpuIndex] & 0xFFFFFFFF;
  Edx = ((COMP_MODE_CS_R3 << 16) | LONG_MODE_CS) & 0xFFFFFFFF;
  AsmWriteMsr64 (MSR_STAR_ADDRESS, (((UINT64)Edx << 32) | Eax));

  mMsrStar64Value[CpuIndex] = (UINT64)AsmReadMsr64 (MSR_STAR64_ADDRESS);
  AsmWriteMsr64 (MSR_STAR64_ADDRESS, (UINT64) SyscallCenter);

  mMsrCstarValue[CpuIndex] = (UINT64)AsmReadMsr64 (MSR_CSTAR_ADDRESS);
  AsmWriteMsr64 (MSR_CSTAR_ADDRESS, 0);

  // Prevent the original OS register from being tampered somehow
  mMmSupvGsStore[CpuIndex].OsGsBasePtr = (UINT64) AsmReadMsr64 (MSR_IA32_GS_BASE);
  AsmWriteMsr64 (MSR_IA32_GS_BASE, 0);

  // Store the original content and replace it with mMmSupvGsStore for this CPU
  mMmSupvGsStore[CpuIndex].OsGsSwapBasePtr = (UINT64) AsmReadMsr64 (MSR_IA32_KERNEL_GS_BASE);
  AsmWriteMsr64 (MSR_IA32_KERNEL_GS_BASE, (UINTN)&mMmSupvGsStore[CpuIndex]);

}

/**
  Update the address inside mMmSupvGsStore for CpuIndex.

  @param[in]      CpuIndex            CpuIndex value of intended core, cannot be
                                      greater than mNumberOfCpus.
  @param[in]      Cpl0StackPtr        Ring0 stack pointer that will be used immediately
                                      into syscall entry routine.

  @retval EFI_SUCCESS               The stack pointer is successfully update.
  @retval EFI_INVALID_PARAMETER     The CpuIndex is out of range or the incoming stack is NULL pointer.
**/
EFI_STATUS
EFIAPI
UpdateCpl0StackPtrForGs (
  IN UINTN                 CpuIndex,
  IN EFI_PHYSICAL_ADDRESS  Cpl0StackPtr
  )
{
  if ((CpuIndex >= mSmmSupvCpuData.NumberOfCpus) ||
      ((VOID *)Cpl0StackPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  //RSP set same as
  // File: ..\SmmSupervisorPkg\SmmSupervisor\SmmSupvGdt.c
  // 137:   // Initial the Ring 0 call gate stack point
  // 138:   //TssDescriptor:
  // 139:       // DD      0                   ; Reserved
  // 140:       // DQ      0                   ; RSP0

  mMmSupvGsStore[CpuIndex].MmSupvRsp = Cpl0StackPtr;

  return EFI_SUCCESS;
}

/**
  Restore registers when exit supervisor smi.

  @param  CpuIndex    The index of the processor.

**/
VOID
SmmSupvSmiExitRegRestore (
  IN        UINT64                     CpuIndex
 )
{
  if (mMsrStarValue == NULL || mMsrStar64Value == NULL || mMsrCstarValue == NULL) {
    WriteErrorData (SMMSUPV_MSR_REG_ERROR);
    ASSERT (FALSE);
    return;
  }

  AsmWriteMsr64 (MSR_STAR64_ADDRESS, mMsrStar64Value[CpuIndex]);
  AsmWriteMsr64 (MSR_STAR_ADDRESS, mMsrStarValue[CpuIndex]);
  AsmWriteMsr64 (MSR_CSTAR_ADDRESS, mMsrCstarValue[CpuIndex]);

  AsmWriteMsr64 (MSR_IA32_GS_BASE, mMmSupvGsStore[CpuIndex].OsGsBasePtr);
  AsmWriteMsr64 (MSR_IA32_KERNEL_GS_BASE, mMmSupvGsStore[CpuIndex].OsGsSwapBasePtr);
}

/**
  Compare memory policy in two SmmPolicy.

  @param  SmmPolicyData1    The first data to compare.
  @param  SmmPolicyData2    The second data to compare.

  @retval FALSE       If two memory policy not identical.

**/
BOOLEAN
CompareMemoryPolicy (
  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *SmmPolicyData1,
  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *SmmPolicyData2
  )
{
  UINTN MemoryPolicySize;
  UINT8 *SmmMemPolicy1 = NULL;
  UINT8 *SmmMemPolicy2 = NULL;
  SMM_SUPV_POLICY_ROOT_V1 *PolicyRoot1 = NULL;
  SMM_SUPV_POLICY_ROOT_V1 *PolicyRoot2 = NULL;
  UINTN i;

  if (SmmPolicyData1 == NULL || SmmPolicyData2 == NULL) {
    return FALSE;
  }

  // Locate memory descriptors first
  for (i = 0; i < SmmPolicyData1->PolicyRootCount; i++) {
    PolicyRoot1 = &((SMM_SUPV_POLICY_ROOT_V1*) ((UINTN) SmmPolicyData1 + SmmPolicyData1->PolicyRootOffset)) [i];
    if (PolicyRoot1->Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM) {
      // Here we found it
      SmmMemPolicy1 = (UINT8 *) SmmPolicyData1 + PolicyRoot1->Offset;
      break;
    }
  }

  for (i = 0; i < SmmPolicyData2->PolicyRootCount; i++) {
    PolicyRoot2 = &((SMM_SUPV_POLICY_ROOT_V1*) ((UINTN) SmmPolicyData2 + SmmPolicyData2->PolicyRootOffset)) [i];
    if (PolicyRoot2->Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM) {
      // Here we found it
      SmmMemPolicy2 = (UINT8 *) SmmPolicyData2 + PolicyRoot2->Offset;
      break;
    }
  }

  if (SmmMemPolicy1 == NULL || SmmMemPolicy2 == NULL ||
      PolicyRoot1 == NULL || PolicyRoot2 == NULL ||
      PolicyRoot1->Version != PolicyRoot2->Version ||
      PolicyRoot1->PolicyRootSize != PolicyRoot2->PolicyRootSize ||
      PolicyRoot1->AccessAttr != PolicyRoot2->AccessAttr ||
      PolicyRoot1->Count != PolicyRoot2->Count) {
    return FALSE;
  }

  MemoryPolicySize = sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0) * PolicyRoot1->Count;
  SmmMemPolicy1 = (UINT8 *) SmmPolicyData1 + PolicyRoot1->Offset;
  SmmMemPolicy2 = (UINT8 *) SmmPolicyData2 + PolicyRoot2->Offset;
  if (CompareMem (SmmMemPolicy1, SmmMemPolicy2, MemoryPolicySize) == 0) {
    return TRUE; //Memory Policy Identical
  }

  // Compare legacy memory descriptors, if needed
  if (FeaturePcdGet (SupportLegacyMemoryPolicy)) {
    if (SmmPolicyData1->MemoryPolicyCount != SmmPolicyData2->MemoryPolicyCount) {
      return FALSE;
    }

    MemoryPolicySize = sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR) * SmmPolicyData1->MemoryPolicyCount;
    SmmMemPolicy1 = (UINT8 *) SmmPolicyData1 + SmmPolicyData1->MemoryPolicyOffset;
    SmmMemPolicy2 = (UINT8 *) SmmPolicyData2 + SmmPolicyData2->MemoryPolicyOffset;
    if (CompareMem (SmmMemPolicy1, SmmMemPolicy2, MemoryPolicySize) == 0) {
      return TRUE; //Memory Policy Identical
    }
  }
  return FALSE;
}

/**
  This function sets SMI entry to R/W
**/
VOID
SetSmmBaseWriteAttribute (
  VOID
  )
{
  UINTN Index;
  DEBUG ((DEBUG_INFO, "SetSmmBaseWriteAttribute:\n"));
  for (Index = 0; Index < mSmmSupvCpuData.NumberOfCpus; Index++) {
    DEBUG ((DEBUG_INFO, "CPU %d SmmBase: 0x%x\n", Index, mSmmSupvCpuData.SmBase[Index]));
    //Set SMI entry to R/W, to allow DRTMBoot flag able to clear
    SmmClearMemoryAttributes (mSmmSupvCpuData.SmBase[Index] + 0x8000, 0x1000, EFI_MEMORY_RO, TRUE);
  }
}

/**
  This function set SMI entry to RO
**/
VOID
ClearSmmBaseWriteAttribute (
  VOID
  )
{
  UINTN Index;
  DEBUG ((DEBUG_INFO, "ClearSmmBaseWriteAttribute:\n"));
  for (Index = 0; Index < mSmmSupvCpuData.NumberOfCpus; Index++) {
    DEBUG ((DEBUG_INFO, "CPU %d SmmBase: 0x%x\n", Index, mSmmSupvCpuData.SmBase[Index]));
    //Set SMI entry which allow DRTMBoot flag able to clear, back to RO
    SmmSetMemoryAttributes (mSmmSupvCpuData.SmBase[Index] + 0x8000, 0x1000, EFI_MEMORY_RO, TRUE);
  }
}

/**
  This function sets mememory attributes for all CPL0 regions
**/
VOID
SetMemoryAttributesForCpl0Regions (
  SMM_SUPV_CPL0_REGION *Cpl0Regions,
  UINTN                NumOfRegion
  )
{
  UINTN i;
  DEBUG ((DEBUG_INFO, "SetMemoryAttributesForCpl0Regions NumOfRegion:0x%x\n", NumOfRegion));
  for (i = 0; i < NumOfRegion; i++) {

    DEBUG ((DEBUG_INFO, "\tBase: 0x%lx, Length: 0x%lx Attributes: %lx\n", \
    ALIGN_VALUE(Cpl0Regions[i].BaseAddress, EFI_PAGE_SIZE), \
    ALIGN_VALUE(Cpl0Regions[i].Length, EFI_PAGE_SIZE), \
    Cpl0Regions[i].Attributes));
    //Todo need to find proper bit to identify CPL0 Only memory
    SmmSetMemoryAttributes (ALIGN_VALUE (Cpl0Regions[i].BaseAddress, EFI_PAGE_SIZE),
                            ALIGN_VALUE (Cpl0Regions[i].Length, EFI_PAGE_SIZE),
                            Cpl0Regions[i].Attributes, TRUE);
  }
}

/**
  Post DRTM security check

  @param  SmiEntryParas            Parameter structure for SmmSupvSmiEntry.

  @retval EFI_SUCCESS              Pass the secure check.
  @retval EFI_SECURITY_VIOLATION   Security check Fail.

**/
EFI_STATUS
EFIAPI
ReInitGdtTable (
  IN      SMM_SUPV_SMI_ENTRY_PARAS *SmiEntryParas
  )
{
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS GdtTablebase;
  UINTN TableSize;
  IA32_DESCRIPTOR           Gdtr;

  //Reinit the pagetable in the reserved memory
  Status = GetGdt (SmiEntryParas->CpuIndex, &GdtTablebase, &TableSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Copy pagetable template to reserved memory, patch callgate and TSS
  Status = InitGdtTable (GdtTablebase, TableSize, SmiEntryParas->Cpl0CallGateOffset, SmiEntryParas->Cpl0StackPoint);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mGdtrPtr[SmiEntryParas->CpuIndex].Limit = mSmiGdtr.Limit;
  mGdtrPtr[SmiEntryParas->CpuIndex].Base = (UINTN) GdtTablebase;

  //Load new GDT
  AsmWriteGdtr (&mGdtrPtr[SmiEntryParas->CpuIndex]);
  AsmReadGdtr (&Gdtr);
  if (SmiEntryParas->CpuIndex == 0) {
    DEBUG ((DEBUG_INFO, "New GDTr Base %x Size %x\n", Gdtr.Base, Gdtr.Limit));
  }
  // Use a far ret to make sure new GDT take effect
  InternalX86Retf ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
StackCheck (
  IN      SMM_SUPV_SMI_ENTRY_PARAS *SmiEntryParas
  )
{
  EFI_STATUS  Status;
  UINT64      Cpl0Stack;
  UINT64      Cpl3Stack;
  UINT64      Cpl3StackSize;
  BOOLEAN     IsOverlapping;

  if (SmiEntryParas->CpuIndex == 0) {
    // 1: Ensure CPL0 and CPL3 stack is not overlap, and no overflow
    DEBUG ((DEBUG_INFO, "Stack Overlapping check \n"));
  }
  // Reverse engineer CPL0 and CPL3 stack calculation
  Cpl3StackSize = SmiEntryParas->Cpl3StackSize;
  Cpl3Stack = SmiEntryParas->Cpl3Stack - Cpl3StackSize + sizeof (UINTN);
  Cpl0Stack = SmiEntryParas->Cpl0Stack - SMM_CPL0_OVERALL_STACK_SIZE + sizeof (UINTN);
  IsOverlapping = FALSE;
  // Check if Cpl0 Stack has been modified
  if (mSmmSupvCpuData.StackBase[SmiEntryParas->CpuIndex] != Cpl0Stack) {
    //Stack fixup has been manipulated
    TESTPOINT (SMMSUPV_POST_STACK_NOT_IDENTICAL);
    return EFI_SECURITY_VIOLATION;
  }

  Status = OverlapStatus (Cpl0Stack, SMM_CPL0_OVERALL_STACK_SIZE, Cpl3Stack, Cpl3StackSize, &IsOverlapping);
  if (EFI_ERROR (Status) || IsOverlapping) {
    TESTPOINT (SMMSUPV_POST_STACK_OVERLAP);
    return EFI_SECURITY_VIOLATION;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
VerifyPolicyAndUpdatePageTable (
  IN OUT SMM_SUPV_SECURE_POLICY_DATA_V1_0 *DrtmSmmPolicyData,
  IN     UINT64 ShadowCr3
)
{
  SMM_SUPV_POLICY_ROOT_V1 *PolicyRoot;
  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *SmmTempPolicyData;
  EFI_STATUS  Status;

  SmmTempPolicyData = GetSmmTempPolicyData ();

  // Use SmmTempPolicyData as temp buffer to hold memory policy data
  ZeroMem (SmmTempPolicyData, SMM_POLICY_DATA_SIZE);

  // First, we populate the bare minimal content for policy root as a kick starter
  PolicyRoot = (SMM_SUPV_POLICY_ROOT_V1 *) (SmmTempPolicyData + 1);
  SmmTempPolicyData->PolicyRootCount = 1;
  SmmTempPolicyData->PolicyRootOffset = (UINT32) ((UINTN) PolicyRoot - (UINTN) SmmTempPolicyData);
  SmmTempPolicyData->Size = SmmTempPolicyData->PolicyRootOffset + sizeof (SMM_SUPV_POLICY_ROOT_V1);
  PolicyRoot->Type = SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM;

  // Then leave the heavy lifting job to the library
  Status = PopulateMemoryPolicyEntries (SmmTempPolicyData, ShadowCr3, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Fail to PopulateMemoryPolicyEntries %r\n", __FUNCTION__, Status));
    return Status;
  }

  //shadow pagetable is match DRTM reported memory policy
  if (ShadowCr3 == 0) {
    DEBUG ((DEBUG_INFO, "Verify  PageTable.Active %lx == PageTable.DrtmReported %lx \n", DrtmSmmPolicyData, SmmTempPolicyData));
  } else {
    DEBUG ((DEBUG_INFO, "Verify  PageTable.Shadow %lx == PageTable.DrtmReported %lx\n", DrtmSmmPolicyData, SmmTempPolicyData));
  }

  if (CompareMemoryPolicy (DrtmSmmPolicyData, SmmTempPolicyData) == FALSE) {
    return EFI_SECURITY_VIOLATION;
  }
  return EFI_SUCCESS;
}

/**
  Initialize IDT IST Field.

  @param[in]  ExceptionType       Exception type.
  @param[in]  Ist                 IST value.

**/
VOID
EFIAPI
InitializeIdtIst (
  IN UINT64 IdtrBase,
  IN EFI_EXCEPTION_TYPE  ExceptionType,
  IN UINT8               Ist
  )
{
  IA32_IDT_GATE_DESCRIPTOR  *IdtGate;

  IdtGate                  = (IA32_IDT_GATE_DESCRIPTOR *)IdtrBase;
  IdtGate                 += ExceptionType;
  IdtGate->Bits.Reserved_0 = Ist;
}

EFI_STATUS
EFIAPI
VerifyAndUpdatePageTbl (
  IN      SMM_SUPV_SMI_ENTRY_PARAS *SmiEntryParas
  )
{
  SMM_SUPV_SECURE_POLICY_DATA_V1_0        *DrtmSmmPolicyData;
  UINT64                SmmSupvAddr;
  UINT64                SmmSupvSize;
  UINTN                 RegionCount;
  UINTN                 i;
  EFI_STATUS            Status;
  PER_CORE_SMI_ENTRY_STRUCT_HDR  *SmiEntryStructHdr;
  UINT32                *Fixup32Ptr;
  UINT32                *Cr3Ptr;
  UINT64                SmmSupvShadowCr3;
  SMM_SUPV_MEM_BLOCK    *MemRegion;
  UINT32                MemRegionCount;

  SmiEntryStructHdr = (PER_CORE_SMI_ENTRY_STRUCT_HDR *) (UINTN) SmiEntryParas->PerCoreSmiEntryStructAddr;
  Fixup32Ptr        = (UINT32 *)(UINTN)(SmiEntryParas->PerCoreSmiEntryStructAddr + SmiEntryStructHdr->FixUp32Offset);
  Cr3Ptr            = &Fixup32Ptr[FIXUP32_CR3_OFFSET];

  if (SmiEntryParas->CpuIndex == 0) {
    DEBUG ((DEBUG_INFO, "VerifyAndUpdatePageTbl\n"));

    //Verify Pagetable, and patch table on BSP only, AP need wait until BSP finish Pagetable patching
    Status = PostDrtmGetSecureStoredInfo (&SmmSupvAddr, &SmmSupvSize, NULL, NULL, (UINT64*) &DrtmSmmPolicyData, NULL, NULL, NULL);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    //Verify  PageTable.Shadow == PageTable.DrtmReported
    //
    //Shadow Cr3 been cleared
    if (!IsCr3Shadowed ()) {
      return EFI_SECURITY_VIOLATION;
    }

    SmmSupvShadowCr3 = GetPageTableBase (TRUE);

    //Check if shadow pagetable has been manipulated, it should idetical to  DRTM reported memory policy
    Status = VerifyPolicyAndUpdatePageTable (DrtmSmmPolicyData, SmmSupvShadowCr3);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //initialize SmmSupvCpl0Regions
    ZeroMem (mSmmSupvCpl0Regions, (MAX_SMM_SUPV_CPL0_REGIONS * sizeof(SMM_SUPV_CPL0_REGION)));
    //Both Current pagetable and shadow pagetable is same as DRTM reported
    RegionCount = 0;
    // Patch SmmSupv Driver code region
    mSmmSupvCpl0Regions[RegionCount].BaseAddress = SmmSupvAddr;
    mSmmSupvCpl0Regions[RegionCount].Length = SmmSupvSize;
    DEBUG ((DEBUG_INFO, "Patch SmmSupv Driver - Base: 0x%lx, Length: 0x%lx\n", \
            mSmmSupvCpl0Regions[RegionCount].BaseAddress, \
            mSmmSupvCpl0Regions[RegionCount].Length));
    mSmmSupvCpl0Regions[RegionCount++].Attributes = EFI_MEMORY_SP;

    //Patch CpL0 Stack for all processors
    DEBUG ((DEBUG_INFO, "Patch CpL0 Stacks -\n"));
    for (i = 0; i < mSmmSupvCpuData.NumberOfCpus && RegionCount < MAX_SMM_SUPV_CPL0_REGIONS; i++) {
      mSmmSupvCpl0Regions[RegionCount].BaseAddress = mSmmSupvCpuData.StackBase[i];
      mSmmSupvCpl0Regions[RegionCount].Length = SMM_CPL0_OVERALL_STACK_SIZE;
      DEBUG ((DEBUG_INFO, "\tCpu %d - Base: 0x%lx, Length: 0x%lx\n", \
            i, mSmmSupvCpl0Regions[RegionCount].BaseAddress, \
            mSmmSupvCpl0Regions[RegionCount].Length));
      mSmmSupvCpl0Regions[RegionCount++].Attributes = EFI_MEMORY_SP;
    }
    if (RegionCount > MAX_SMM_SUPV_CPL0_REGIONS) {
      WriteErrorData (SMMSUPV_VERIFY_PAGE_TABLE_FAIL);
    }
    ASSERT (RegionCount < MAX_SMM_SUPV_CPL0_REGIONS );

    // Patch CpL0 GDT
    DEBUG ((DEBUG_INFO, "Patch CpL0 GDTs -\n"));
    for (i = 0; i < mSmmSupvCpuData.NumberOfCpus && RegionCount < MAX_SMM_SUPV_CPL0_REGIONS; i++) {
      mSmmSupvCpl0Regions[RegionCount].BaseAddress = mSmmSupvGdtData[i].Tablebase;
      mSmmSupvCpl0Regions[RegionCount].Length = mSmmSupvGdtData[i].TableSize;
      DEBUG ((DEBUG_INFO, "\tCpu %d - Base: 0x%lx, Length: 0x%lx\n", \
            i, mSmmSupvCpl0Regions[RegionCount].BaseAddress, \
            mSmmSupvCpl0Regions[RegionCount].Length));
      mSmmSupvCpl0Regions[RegionCount++].Attributes = EFI_MEMORY_SP;
      //Set GDT table to R/W, to allow update
      SmmClearMemoryAttributes (ALIGN_TO_PAGE (mSmmSupvGdtData[i].Tablebase),
                                AlignSizeToPage (mSmmSupvGdtData[i].Tablebase, mSmmSupvGdtData[i].TableSize),
                                EFI_MEMORY_RO, TRUE);
    }
    if (RegionCount > MAX_SMM_SUPV_CPL0_REGIONS) {
      WriteErrorData (SMMSUPV_VERIFY_PAGE_TABLE_FAIL);
    }
    ASSERT (RegionCount < MAX_SMM_SUPV_CPL0_REGIONS );

    // Patch CpL0 IDT
    if (mSmiIdtrPtr->Base != mSmiIdtr.Base || mSmiIdtrPtr->Limit != mSmiIdtr.Limit) {
      InitializeSmmIdt (&mSmiIdtr);
      mSmiIdtrPtr->Base = mSmiIdtr.Base;
      mSmiIdtrPtr->Limit = mSmiIdtr.Limit;
      AsmWriteIdtr (mSmiIdtrPtr);
      InitializeIdtIst (mSmiIdtr.Base, EXCEPT_IA32_DEBUG, SMMSUPV_STACK_IST_INDEX);
    }

    mSmmSupvCpl0Regions[RegionCount].BaseAddress = ALIGN_TO_PAGE (mSmiIdtrPtr->Base);
    mSmmSupvCpl0Regions[RegionCount].Length = AlignSizeToPage (mSmiIdtrPtr->Base, mSmiIdtrPtr->Limit + 1);
    DEBUG ((DEBUG_INFO, "Patch CpL0 IDTs - Base: 0x%lx, Length: 0x%lx\n", \
            mSmmSupvCpl0Regions[RegionCount].BaseAddress, \
            mSmmSupvCpl0Regions[RegionCount].Length));
    mSmmSupvCpl0Regions[RegionCount++].Attributes = EFI_MEMORY_SP;
    if (RegionCount > MAX_SMM_SUPV_CPL0_REGIONS) {
      WriteErrorData (SMMSUPV_VERIFY_PAGE_TABLE_FAIL);
    }
    ASSERT (RegionCount < MAX_SMM_SUPV_CPL0_REGIONS );

    // Patch CpL0 Heap Allocated in SmmSupv
    MemRegionCount = NumberOfMemoryRegions ();
    DEBUG ((DEBUG_VERBOSE, "%a MemRegionCount:%x \n", __FUNCTION__, MemRegionCount));
    DEBUG ((DEBUG_INFO, "Patch SMMSUPV Heaps -\n"));
    for (i = 0; i < MemRegionCount && RegionCount < MAX_SMM_SUPV_CPL0_REGIONS; i++) {
      MemRegion = GetSmmSupvMemBlock ((UINT32)i);
      mSmmSupvCpl0Regions[RegionCount].BaseAddress = MemRegion->BaseAddress;
      mSmmSupvCpl0Regions[RegionCount].Length = MemRegion->Length;
      DEBUG ((DEBUG_INFO, "\tRegion %d - Base: 0x%lx, Length: 0x%lx\n", \
            i, mSmmSupvCpl0Regions[RegionCount].BaseAddress, \
            mSmmSupvCpl0Regions[RegionCount].Length));
      mSmmSupvCpl0Regions[RegionCount++].Attributes = EFI_MEMORY_SP;
    }

    if (RegionCount > MAX_SMM_SUPV_CPL0_REGIONS) {
      WriteErrorData (SMMSUPV_VERIFY_PAGE_TABLE_FAIL);
    }
    ASSERT (RegionCount < MAX_SMM_SUPV_CPL0_REGIONS );
    //Patch the shadow pagetable as SmmSupvCpl0Regions reported
    //SmmEntry, SmmSaveState of  will be patched inside below routine
    PatchSmmSupvPageTbl (&mSmmSupvCpl0Regions[0], RegionCount);
    // Then we mark the rest of the handlers to be run under ring 3
    mSmmSupervisorPostReadyToLock = SMM_SUPV_DRTM_EVT_VALUE;
    mPageTblPatchComplete = TRUE;
  }
  //All processors wait until BSP finish pagetable patching then apply the shadow pagetable
  while (mPageTblPatchComplete != TRUE) {
    ;
  }

  SmmSupvShadowCr3 = GetPageTableBase (TRUE);

  AsmWriteIdtr (mSmiIdtrPtr);
  AsmWriteCr3 (SmmSupvShadowCr3);
  CpuFlushTlb ();

  //Update CR3 to new patched table
  *Cr3Ptr = (UINT32) SmmSupvShadowCr3;

  return EFI_SUCCESS;
}

/**
  Retrieves a pointer to the system configuration table from the SMM System Table
  based on a specified GUID.

  @param[in]   TableGuid       The pointer to table's GUID type.
  @param[out]  Table           The pointer to the table associated with TableGuid in the EFI System Table.

  @retval EFI_SUCCESS     A configuration table matching TableGuid was found.
  @retval EFI_NOT_FOUND   A configuration table matching TableGuid could not be found.

**/
EFI_STATUS
EFIAPI
SmmGetSystemConfigurationTable (
  IN  EFI_GUID  *TableGuid,
  OUT VOID      **Table
  )
{
  UINTN             Index;

  ASSERT (TableGuid != NULL);
  ASSERT (Table != NULL);

  *Table = NULL;
  for (Index = 0; Index < gSmst->NumberOfTableEntries; Index++) {
    if (CompareGuid (TableGuid, &(gSmst->SmmConfigurationTable[Index].VendorGuid))) {
      *Table = gSmst->SmmConfigurationTable[Index].VendorTable;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Install the SmmSupervisor public info into the Smm Configuration Table.

  @retval EFI_SUCCESS           The SmmSupervisor public info was installed successfully
  @retval Others                Failed to install the SmmSupervisor public info

**/
EFI_STATUS
EFIAPI
InstallSmmSupvPublicInfoTable (
  )
{
  EFI_STATUS  Status;
  SMM_SUPERVISOR_PUBLIC_INFO_TABLE    *PublicInfoTable;
  UINTN                               PublicInfoTableSize;
  PublicInfoTableSize = sizeof(SMM_SUPERVISOR_PUBLIC_INFO_TABLE);
  // Allocate memory to save SmmSupervisorPublicInfoTable for external read
  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      PublicInfoTableSize,
                      &PublicInfoTable
                      );

  if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate memory for SmmSupervisorPublicInfoTable - %r\n", Status));
      WriteErrorData (SMMSUPV_ALLOCATE_MEM_POOL_FAIL);
      return Status;
  }

  ASSERT (PublicInfoTable != NULL);
  PublicInfoTable->Version               = SMM_SUPERVISOR_PUBLIC_INFO_TABLE_VERSION;
  PublicInfoTable->SmmIsolationEnabled   = 0;
  PublicInfoTable->Reserved1             = 0;
  PublicInfoTable->Reserved2             = 0;

  Status = gSmst->SmmInstallConfigurationTable (gSmst,
                                                &gSmmSupervisorPublicInfoTableGuid,
                                                PublicInfoTable,
                                                PublicInfoTableSize);
  if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install SmmSupervisorPublicInfoTable - %r\n", Status));
      WriteErrorData (SMMSUPV_INSTALL_INTERFACE_FAIL);
      return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PostDrtmEntry (
  IN      SMM_SUPV_SMI_ENTRY_PARAS *SmiEntryParas,
  IN      BOOLEAN PerformDrtmInfoCheck
  )
{
  EFI_STATUS Status;
  UINTN      i;
  UINT32     *Fixup32Ptr;
  UINT32     *DrtmBootFlag;
  PER_CORE_SMI_ENTRY_STRUCT_HDR     *SmiEntryStructHdr;
  SMM_SUPERVISOR_PUBLIC_INFO_TABLE  *PublicInfoTable;

  if (SmiEntryParas->CpuIndex == 0) {
    TESTPOINT (SMMSUPV_POST_DRTM_ENTRY);
    DEBUG ((DEBUG_INFO, "PostDrtmEntry %x>>\n", PerformDrtmInfoCheck));
    if (PerformDrtmInfoCheck == TRUE) {
      Status = PostDrtmSecureValidation ();
      if (EFI_ERROR (Status)) {
        WriteErrorData (SMMSUPV_POST_DRTM_INFO_NOT_IDENTICAL);
        goto Exit;
      }
    }
  }

  Status = StackCheck (SmiEntryParas);
  if (EFI_ERROR (Status)) {
    TESTPOINT (SMMSUPV_POST_STACK_CHECK_FAIL);
    goto Exit;
  }

  Status = VerifyAndUpdatePageTbl (SmiEntryParas);
  if (EFI_ERROR (Status)) {
    TESTPOINT (SMMSUPV_VERIFY_PAGE_TABLE_FAIL);
    goto Exit;
  }

  Status = ReInitGdtTable (SmiEntryParas);
  if (EFI_ERROR (Status)) {
    TESTPOINT (SMMSUPV_REINIT_GDT_FAIL);
    goto Exit;
  }

  // Store CPL0 stack pointer into supv data structure, this will be used upon syscall entry
  Status = UpdateCpl0StackPtrForGs (SmiEntryParas->CpuIndex, (EFI_PHYSICAL_ADDRESS)SmiEntryParas->Cpl0StackPoint);
  if (EFI_ERROR (Status)) {
    TESTPOINT (SMMSUPV_CPL0_STACK_TO_GS_FAIL);
    goto Exit;
  }

  InterlockedIncrement (&mGdtTableUpdateComplete);
  while (mGdtTableUpdateComplete < mThreadsInSmm) {
    CpuPause ();
  };
  //Clear the DRTM Boot flag
  SmiEntryStructHdr = (PER_CORE_SMI_ENTRY_STRUCT_HDR *) (UINTN) SmiEntryParas->PerCoreSmiEntryStructAddr;
  Fixup32Ptr        = (UINT32 *)(UINTN)(SmiEntryParas->PerCoreSmiEntryStructAddr + SmiEntryStructHdr->FixUp32Offset);
  DrtmBootFlag      = &Fixup32Ptr[FIXUP32_DRTM_BOOT_FLAG];

  *DrtmBootFlag = FALSE;
  if (SmiEntryParas->CpuIndex == 0) {
    SmmGetSystemConfigurationTable (&gSmmSupervisorPublicInfoTableGuid, (VOID **)&PublicInfoTable);
    if (PublicInfoTable == NULL) {
      DEBUG ((DEBUG_ERROR, "PublicInfoTable - NULL\n"));
      ASSERT (FALSE);
    } else {
      PublicInfoTable->SmmIsolationEnabled = 1;
    }
    //Set some region back to RO
    for (i = 0; i < mSmmSupvCpuData.NumberOfCpus; i++) {
        //Set GDT table back to RO
        SmmSetMemoryAttributes (ALIGN_TO_PAGE (mSmmSupvGdtData[i].Tablebase),
                                AlignSizeToPage (mSmmSupvGdtData[i].Tablebase, mSmmSupvGdtData[i].TableSize),
                                EFI_MEMORY_RO, TRUE);
        //Set SMI entry which allow DRTMBoot flag able to clear, back to RO
        SmmSetMemoryAttributes (mSmmSupvCpuData.SmBase[i] + 0x8000, 0x1000, EFI_MEMORY_RO, TRUE);
      }

    mPageTblPatchComplete1 = TRUE;
  }

  //All processors wait until BSP finish pagetable patching then apply the shadow pagetable
  while (mPageTblPatchComplete1 != TRUE) {
    ;
  }
  //Make new pagetable take effect
  CpuFlushTlb ();
  Status = EFI_SUCCESS;

Exit:
  if (SmiEntryParas->CpuIndex == 0) {
    TESTPOINT (SMMSUPV_POST_DRTM_ENTRY_END);
    DEBUG ((DEBUG_INFO, "%aSmmSupv PostDrtmEntry %r <<\n", PREFIX_LOG_ANALYZER, Status));
  }
  return Status;
}


/**
  Allocate pool for Global data.
**/
EFI_STATUS
EFIAPI
AllocatePoolForGlobalData (
)
{
  EFI_STATUS  Status;
  UINTN       NumberOfCPUs = 0;
  UINTN       NumberOfEnabledCPUs = 0;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mMpService);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_LOCATE_PROTOCOL_FAIL);
  }
  ASSERT_EFI_ERROR (Status);

  Status = mMpService->GetNumberOfProcessors (
                        mMpService,
                        &NumberOfCPUs,
                        &NumberOfEnabledCPUs
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "mMpService->GetNumberOfProcessors return status: %r\n", Status));
    return Status;
  }

  mSmmSupvCpuData.NumberOfCpus = NumberOfCPUs;
  mSmmSupvCpuData.SmBase    = (UINTN *)AllocateSmmSupvPool (sizeof (UINTN) * mSmmSupvCpuData.NumberOfCpus);
  mSmmSupvCpuData.StackBase = (UINTN *)AllocateSmmSupvPool (sizeof (UINTN) * mSmmSupvCpuData.NumberOfCpus);

  mMsrStarValue   = AllocateSmmSupvPool (sizeof (UINT64) * mSmmSupvCpuData.NumberOfCpus);
  mMsrStar64Value = AllocateSmmSupvPool (sizeof (UINT64) * mSmmSupvCpuData.NumberOfCpus);
  mMsrCstarValue  = AllocateSmmSupvPool (sizeof (UINT64) * mSmmSupvCpuData.NumberOfCpus);
  mMmSupvGsStore = AllocatePool (sizeof (MM_SUPV_SYSCALL_CACHE) * mSmmSupvCpuData.NumberOfCpus);
  if (mMsrStarValue == NULL || mMsrStar64Value == NULL || mMsrCstarValue == NULL || mMmSupvGsStore == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocation for syscall MSR failed. STAR: %p, STAR64 %p Cstar%p GsStore %p\n", mMsrStarValue, mMsrStar64Value, mMsrCstarValue, mMmSupvGsStore));
    return EFI_OUT_OF_RESOURCES;
  }

  mSmmSupvGdtData = AllocateZeroSmmSupvPool (sizeof (SMM_SUPV_GDT_DATA) * NumberOfCPUs);
  if (mSmmSupvGdtData == NULL) {
    DEBUG ((DEBUG_ERROR, "mSmmSupvGdtData == NULL\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG ((DEBUG_INFO, "mSmmSupvGdtData %x Size %x\n", mSmmSupvGdtData, sizeof (SMM_SUPV_GDT_DATA) * NumberOfCPUs));
  mGdtrPtr = AllocateZeroSmmSupvPool (sizeof (IA32_DESCRIPTOR) * NumberOfCPUs);
  if (mGdtrPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "mGdtrPtr == NULL\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  return Status;
}

BOOLEAN
EFIAPI
AllCpusInSmm (VOID)
{
  ASSERT (mThreadCount <= mSmmSupvCpuData.NumberOfCpus);

  return mThreadCount == mSmmSupvCpuData.NumberOfCpus;
}

VOID
DumpSmiStatusRegs (
  VOID
  )
{
  UINT16      SmiPointer;
  UINT32      SmiReg;
  UINT8       SwSmiId;

  SmiPointer = MmioRead16 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG94); // SMIx0000094 (FCH::SMI::SMIPOINTER)
  if ((SmiPointer & (BIT0 + BIT1 + BIT2 + BIT3 + BIT5)) != 0) {
    IoWrite16 (SMMSUPV_DEBUG_PORT, SmiPointer);
    if ((SmiPointer & BIT1) == BIT1) { // source from smistatus0
      SmiReg = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG80);
      IoWrite32 (SMMSUPV_DEBUG_PORT, SmiReg);
    } else if ((SmiPointer & BIT2) == BIT2) { // source from smistatus1
      SmiReg = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84);
      IoWrite32 (SMMSUPV_DEBUG_PORT, SmiReg);
    } else if ((SmiPointer & BIT3) == BIT3) { // source from smistatus2
      SmiReg = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88);
      IoWrite32 (SMMSUPV_DEBUG_PORT, SmiReg);
      if ((SmiReg & BIT11) == BIT11) {
        SwSmiId = MmioRead8 (ACPI_MMIO_BASE + PMIO_BASE + 0x51C); // PMx0000051C (FCH::PM::SMICMDPORT)
        IoWrite32 (SMMSUPV_DEBUG_PORT, (UINT32)SwSmiId);
      }
    } else if ((SmiPointer & BIT5) == BIT5) { // source from smistatus4
      SmiReg = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90);
      IoWrite32 (SMMSUPV_DEBUG_PORT, SmiReg);
    } else if ((SmiPointer & BIT0) == BIT0) { // source from SmiSciStatus
      SmiReg = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
      IoWrite32 (SMMSUPV_DEBUG_PORT, SmiReg);
      // To further find how SCI status is mapped, check FCH registers scimap0 to scimap15
    }
  }
}

/**
  SMI entry point of SmmSupv, called from SmmSupvEntry.nasm
**/
UINT64
EFIAPI
SmmSupvSmiEntry (
  IN      UINTN SmiEntryParasAddr
  )
{
  PER_CORE_SMI_ENTRY_STRUCT_HDR  *SmiEntryStructHdr;
  SMM_SUPV_SMI_ENTRY_PARAS          *SmiEntryParas;
  EFI_STATUS  Status;
  UINT32      *DrtmBootFlag;
  UINT64      CpuIndex;
  UINT32      *Fixup32Ptr;
  UINT64      Timer;
  UINT32      Value;

  // Increment the count for available core
  do {
    Value = mThreadCount;
  } while (InterlockedCompareExchange32 (
             &mThreadCount,
             Value,
             Value + 1
             ) != Value);

  SmiEntryParas     = (SMM_SUPV_SMI_ENTRY_PARAS *) SmiEntryParasAddr;
  SmiEntryStructHdr = (PER_CORE_SMI_ENTRY_STRUCT_HDR *) (UINTN) SmiEntryParas->PerCoreSmiEntryStructAddr;
  Fixup32Ptr        = (UINT32 *)(UINTN)(SmiEntryParas->PerCoreSmiEntryStructAddr + SmiEntryStructHdr->FixUp32Offset);
  DrtmBootFlag      = &Fixup32Ptr[FIXUP32_DRTM_BOOT_FLAG];
  CpuIndex          = SmiEntryParas->CpuIndex;
  if (SmiEntryParas->CpuIndex == 0) {
    TESTPOINT (SMMSUPV_SMI_ENTRY);

    DEBUG_CODE (
      DumpSmiStatusRegs ();
    );

    DEBUG ((DEBUG_VERBOSE, "SmmSupvSmiEntry >>\n"));
    DEBUG ((DEBUG_VERBOSE,
      "SmiEntryParas:\n \
       CpuIndex %x\n \
       Cpl0Stack %x\n \
       Cpl3Stack %x\n \
       Cpl3StackSize %x\n \
       PerCoreSmiEntryStructAddr %x\n \
       DrtmBootFlag %x\n \
       Cpl0CallGateOffset %x\n \
       Cpl0StackPoint %x\n",\
       SmiEntryParas->CpuIndex, SmiEntryParas->Cpl0Stack, SmiEntryParas->Cpl3Stack, \
       SmiEntryParas->Cpl3StackSize, SmiEntryParas->PerCoreSmiEntryStructAddr, *DrtmBootFlag,\
       SmiEntryParas->Cpl0CallGateOffset, SmiEntryParas->Cpl0StackPoint \
     ));
    //
    // Sync with APs, one for all
    //
    for (Timer = StartSyncTimer ();
         !IsSyncTimerTimeout (Timer, *DrtmBootFlag) && !AllCpusInSmm ();
         ) {
      CpuPause ();
    }
    if (!AllCpusInSmm ()) {
      DEBUG ((DEBUG_ERROR, "{%x\n", CpuIndex));
      TESTPOINT (SMMSUPV_CORESYNC_TIMEOUT_BASE);
      if (NeedsExtendedWait (*DrtmBootFlag)) {
        // We waited for extra time for this instance, but still do not see the AP
        // Critical error, cannot run further than this. ASSERT here should reboot
        // for release build and hang for debug builds.
        WriteErrorDataAndReset (SMMSUPV_DRTM_SYNC_TIMEOUT_FAIL_RESET);
        ASSERT (FALSE);
      }
    }
    mThreadsInSmm = mThreadCount;
    mBspInSmm = TRUE;
  }
  else {
    //
    // Sync with BSP, all for one
    //
    for (Timer = StartSyncTimer ();
        !IsSyncTimerTimeout (Timer, *DrtmBootFlag) && !mBspInSmm;
        ) {
      CpuPause ();
    }
    if (!mBspInSmm) {
      TESTPOINT (SMMSUPV_CORESYNC_TIMEOUT_BASE | (UINT16)CpuIndex);
      DEBUG ((DEBUG_ERROR, "{%x\n", CpuIndex));
    }
  }

  InterlockedDecrement (&mThreadCount);

  if (SmiEntryStructHdr->HeaderVersion < 4) {
    // some thing is wrong here
    WriteErrorData (SMMSUPV_UNSUPPORTED_SMIENTRY_VERSION);
    ASSERT (FALSE);
  }

  //DRTM BOOT FLAG turn on by DRTM TA, perform post DRTM security check
  if (*DrtmBootFlag == TRUE) {
    Status = PostDrtmEntry (SmiEntryParas, TRUE);
    if (EFI_ERROR (Status)) {
      WriteErrorData (SMMSUPV_POST_DRTM_SECURITY_CHECK_FAIL);
      ASSERT (FALSE);
    }
  }

  SmmSupvSmiEntryRegInit (SmiEntryParas->CpuIndex);
  if (mSmmSupervisorPostReadyToLock == SMM_SUPV_PRE_LOCK_VALUE) {
    //Perform SMM isolation at SMM Ready to lock
    if (mSmmIsoEnableEvent == SMM_ISO_ENABLE_EVENT_READY_TO_BOOT) {
      Status = PostDrtmEntry (SmiEntryParas, FALSE);
      if (EFI_ERROR (Status)) {
        WriteErrorData (SMMSUPV_POST_DRTM_SECURITY_CHECK_FAIL);
        ASSERT (FALSE);
      }
      UpdateMemoryPolicy (SmiEntryParas->CpuIndex);
    }
    //Dump out the policy data we report to secure processor
    if (SmiEntryParas->CpuIndex == 0) {
      DEBUG ((DEBUG_INFO, "\nDump mSmmPolicyData reported to secure processor\n"));
      DumpSmmPolicyData (mSmmPolicyData);
    }
    IhvDrtmReadyToLockAtSmiEntry (SmiEntryParas->CpuIndex);
  }

  if ((mSmmSupervisorPostReadyToLock & SMM_SUPV_PRE_LOCK_MASK) == SMM_SUPV_PRE_LOCK_MASK){
    // Invoke IHV specific routine if needed.
    IhvDrtmPostPageTableLock (SmiEntryParas->CpuIndex);
  }

  if (SmiEntryParas->CpuIndex == 0) {
    DEBUG ((DEBUG_VERBOSE, "SmmSupvSmiEntry 0x%x<<\n", mSmmSupervisorPostReadyToLock));
  }
  return mSmmSupervisorPostReadyToLock;
}

/**
  Called on Ready To Lock for each CPU thread after Ready to lock. Generate memory policy Descriptors by traverse pagetables and shadow current pagetable.

  @param[in] CpuIndex Logical number assigned to CPU.
**/
VOID
EFIAPI
PageTblReadyToLockCallback (
  IN        UINT64                     CpuIndex
  )
{
  EFI_STATUS Status;
  EFI_MEMORY_DESCRIPTOR *MemDescriptor;
  UINTN MemDescriptorCount;
  UINTN Index;
  UINT64 SmmSupvShadowCr3;

  if (CpuIndex == 0) {
    DEBUG ((DEBUG_INFO, "PageTblReadyToLockCallback\n"));

    //IO and MSR policies are populated during report DRTM info time
    //Generate Policy and shadow current Pagetable
    //Shadow the pagetable on BSP only, as all processors are sharing one pagetable copy
    Status = PopulateMemoryPolicyEntries (mSmmPolicyData, 0, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Fail to PopulateMemoryPolicyEntries %r\n", __FUNCTION__, Status));
      return;
    }

    // Fetch pages need to be protected by supervisor supplied by IHV
    MemDescriptorCount = 0;
    MemDescriptor = NULL;
    Status = IhvGetPagesForProtection (&MemDescriptor, &MemDescriptorCount);
    if (EFI_ERROR (Status) && (Status != EFI_UNSUPPORTED)) {
      DEBUG ((DEBUG_ERROR, "%a Fail at IhvGetPagesForProtection %r\n", __FUNCTION__, Status));
      return;
    }

    for (Index = 0; Index < MemDescriptorCount; Index ++) {
      SmmSetMemoryAttributes (MemDescriptor[Index].PhysicalStart,
                              EFI_PAGES_TO_SIZE (MemDescriptor[Index].NumberOfPages),
                              MemDescriptor[Index].Attribute, TRUE);
    }

    if (MemDescriptor != NULL) {
      FreePool (MemDescriptor);
    }

    // Fetch pages need to be protected by supervisor supplied by IHV
    MemDescriptorCount = 0;
    MemDescriptor = NULL;
    Status = IhvGetPagesForSupervision (&MemDescriptor, &MemDescriptorCount);
    if (EFI_ERROR (Status) && (Status != EFI_UNSUPPORTED)) {
      DEBUG ((DEBUG_ERROR, "%a Fail at IhvGetPagesForProtection %r\n", __FUNCTION__, Status));
      return;
    }

    for (Index = 0; Index < MemDescriptorCount; Index ++) {
      if ((MemDescriptor[Index].Attribute & EFI_MEMORY_SP) == 0) {
        DEBUG ((DEBUG_ERROR, "%a Requested memory address 0x%p of 0x%d pages does not contain EFI_MEMORY_SP: 0x%x\n",
                __FUNCTION__,
                MemDescriptor[Index].PhysicalStart,
                MemDescriptor[Index].NumberOfPages,
                MemDescriptor[Index].Attribute));
        ASSERT (FALSE);
        break;
      }
      SmmSetMemoryAttributes (MemDescriptor[Index].PhysicalStart,
                              EFI_PAGES_TO_SIZE (MemDescriptor[Index].NumberOfPages),
                              MemDescriptor[Index].Attribute, TRUE);
    }

    if (MemDescriptor != NULL) {
      FreePool (MemDescriptor);
    }

    //Update the policy data from shadow table after patch
    SmmSupvShadowCr3 = GetPageTableBase (TRUE);
    Status = PopulateMemoryPolicyEntries (mSmmPolicyData, SmmSupvShadowCr3, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Fail to PopulateMemoryPolicyEntries after IOMMU patching %r\n", __FUNCTION__, Status));
      return;
    }
  }
}

/**
  SMI exit point of SmmSupv, called from SmmSupvEntry.nasm
**/
UINT64
EFIAPI
SmmSupvSmiExit (
  IN      UINTN                     CpuIndex
  )
{
  if (CpuIndex == 0) {
    mBspInSmm = FALSE;
    mPageTblPatchComplete = FALSE;
    mPageTblPatchComplete1 = FALSE;
    mGdtTableUpdateComplete = 0;

    DEBUG ((DEBUG_VERBOSE, "SmmSupvSmiExit >>\n"));
  }

  if (mSmmSupervisorPostReadyToLock == SMM_READY_TO_LOCK_VALUE) {
    // Call IHV specific hook at ready to lock event
    IhvDrtmReadyToLockAtSmiExit (CpuIndex);

    if (CpuIndex == 0) {
      mSmmSupervisorPostReadyToLock |= SMM_SUPV_PRE_LOCK_MASK; //Clear to avoid re-enter on next smi
    }
    PageTblReadyToLockCallback (CpuIndex);
  } else if (mSmmSupervisorPostReadyToLock == SMM_SUPV_PRE_LOCK_VALUE) {
    if (CpuIndex == 0) {
      mSmmSupervisorPostReadyToLock |= SMM_SUPV_AFT_LOCK_MASK; //Clear to avoid re-enter on next smi
    }
  }

  if (mSyscallViolationCnt != 0) {
    // Violated syscall found during this SMI, reset as our containing policy
    ASSERT (FALSE);
    WriteErrorDataAndReset (SMMSUPV_SYSCALL_FAIL_RESET);
  }

  SmmSupvSmiExitRegRestore (CpuIndex);

  if (CpuIndex == 0) {
    TESTPOINT (SMMSUPV_SMI_EXIT);
    DEBUG ((DEBUG_VERBOSE, "SmmSupvSmiExit <<\n"));
  }

  return TRUE;
}

/**
 * @brief store SMM Supervisor version to UEFI variable so that it can be retrieved by BIOS and OS application
 *
 * @param Version                  SMM Supervisor version
 * @retval EFI_SUCCESS             storing version info is successful
 * @retval Other value             storing version info failed
 */
EFI_STATUS
EFIAPI
StoreSmmSupervisorVersion (
  IN CHAR8    *Version
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;
  EFI_SMM_VARIABLE_PROTOCOL           *SmmVariable;

  Status = EFI_INVALID_PARAMETER;
  if (Version == NULL) {
    DEBUG ((DEBUG_WARN, "Invalid value for SMM Supervisor version\n"));
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmVariableProtocolGuid,
                    NULL,
                    (VOID **)&SmmVariable
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to locate gEfiSmmVariableProtocolGuid. Status = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "SMM Supervisor version to store:%a\n", Version));
  BufferSize = sizeof (CHAR8) * MAX_VERSION_CHAR_COUNT;
  Status = SmmVariable->SmmSetVariable (
                SMM_SUPERVISOR_VERSION_VARIABLE_NAME,
                &mSmmSupervisorVersionGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                BufferSize,
                (VOID *) Version
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to store SMM Supervisor version. Status = %r\n", Status));
  }
  return Status;
}

/**
  Get Smm base.
**/
VOID
EFIAPI
GetSmmBase (
  IN OUT VOID  *SmmBase
  )
{
  *(UINTN *)SmmBase = AsmReadMsr64 (MSR_SMM_BASE);
}

/**
  SMM Ready To Lock event notification handler.

  mSmmReadyToLock is set to perform additional lock actions that must be performed from SMM on the next SMI.

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification handler runs successfully.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.

 **/
EFI_STATUS
EFIAPI
SmmSupvSmmReadyToLockEventNotify (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  UINTN i;
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "SmmSupvSmmReadyToLockEventNotify\n"));

  // store SMM Supervisor version in UEFI variable for later use
  StoreSmmSupervisorVersion (mSmmSupVersion);

  //Free the temporary buffer used to keep SmmEntry
  if (mSmmEntryAddr != NULL) {
    FreePool (mSmmEntryAddr);
  }
  mSmmSupervisorPostReadyToLock = SMM_READY_TO_LOCK_VALUE;

  for (i = 0; i < gSmst->NumberOfCpus; i++) {
//[-start-220225-IB14740218-modify]//
    if (i == gSmst->CurrentlyExecutingCpu) {
//[-end-220225-IB14740218-modify]//
      GetSmmBase (&mSmmSupvCpuData.SmBase[i]);
    } else {
      gSmst->SmmStartupThisAp (
              GetSmmBase,
              i,
              &mSmmSupvCpuData.SmBase[i]
              );
    }
  }
  Status = gSmst->SmmUninstallProtocolInterface (mSmmSupervisorHandle, &gSmmSupervisorProtocolGuid, &mSmmSupervisor);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_UNINSTALL_PROTOCOL_FAIL);
  }
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Allocates a buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateSmmSupvPool (
  IN UINTN  AllocationSize
  )
{
  VOID        *Buffer = NULL;
  if (mProtectedMemPool.CurrentOffset + AllocationSize < mProtectedMemPool.PoolBase + mProtectedMemPool.PoolSize) {
    Buffer = (VOID *)(mProtectedMemPool.PoolBase + mProtectedMemPool.CurrentOffset);
    mProtectedMemPool.CurrentOffset += AllocationSize;
    DEBUG ((DEBUG_INFO, "%a PoolBase:%x, PoolSize:%lx, AllocatedBuffer:%x, AllocationSize:%x \n", \
      __FUNCTION__, mProtectedMemPool.PoolBase, mProtectedMemPool.PoolSize, (UINTN)Buffer, AllocationSize));
    return Buffer;
  } else {
    DEBUG ((DEBUG_ERROR, "%a Out of resource to allocate memory for the memory pool. PoolBase:%x, PoolSize:%lx, CurrentOffset:%x, AllocationSize:%x\n", \
      __FUNCTION__, mProtectedMemPool.PoolBase, mProtectedMemPool.PoolSize, mProtectedMemPool.CurrentOffset, AllocationSize));
    return NULL;
  }
}

/**
  Allocates and zeros a buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateZeroSmmSupvPool (
  IN UINTN  AllocationSize
  )
{
  VOID *Buffer;
  Buffer = AllocateSmmSupvPool (AllocationSize);
  if (Buffer == NULL) {
    return NULL;
  }

  ZeroMem (Buffer, AllocationSize);
  return Buffer;
}

/**
  The module Entry Point of the SMM supervisor driver.

  @param  ImageHandle    The firmware allocated handle for the EFI image.
  @param  SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS    The entry point is executed successfully.
  @retval Other          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
SmmSupervisorEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  VOID        *Registration;

  Status = EFI_SUCCESS;
  mBootMode = GetBootModeHob();
  DEBUG ((DEBUG_INFO, "SmmSupervisorEntry\n"));
  TESTPOINT (SMMSUPV_MAIN_ENTRY);

  //
  // Install the SmmSupv Protocol into SMM protocol database
  //
  Status = InstallSmmSupvPublicInterface ();
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_INSTALL_INTERFACE_FAIL);
    goto Exit;
  }

  mPcdSmmIsolationEnable = PcdGetBool (PcdSmmIsolationEnable);
  if (!mPcdSmmIsolationEnable) {
    DEBUG ((DEBUG_INFO, "SmmSupervisorEntry\n"));
    goto Exit;
  }

  //
  // Clean error data from bios ram for next error log
  //
  CleanErrorData ();

  //Allocate buffer for protected memory pool
  mProtectedMemPool.PoolBase = 0;
  DEBUG ((DEBUG_INFO, "Allocate SmmSupv pages for protected memory pool:%d\n", MAX_PROTECTED_MEM_POOL_PAGES));
  mProtectedMemPool.PoolBase = (UINTN)AllocateSmmSupvPages (MAX_PROTECTED_MEM_POOL_PAGES);
  DEBUG ((DEBUG_INFO, "mProtectedMemPool: %x\n", mProtectedMemPool.PoolBase));
  if (mProtectedMemPool.PoolBase == 0) {
    WriteErrorData (SMMSUPV_ALLOCATE_MEM_POOL_FAIL);
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  mProtectedMemPool.PoolSize = MAX_PROTECTED_MEM_POOL_PAGES * SIZE_4KB;
  mProtectedMemPool.CurrentOffset = 0;

  // Report signature files to secure processor
  Status = ReportDrtmInfo (ImageHandle, SystemTable);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_REPORT_DRTM_INFO_FAIL);
    goto Exit;
  }

  //
  // Allocate memory for Global data
  //
  Status = AllocatePoolForGlobalData ();
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_ALLOCATE_FOR_GLOBAL_FAIL);
    goto Exit;
  }

  //
  // install SmmSupervisor public info table for external read
  //
  Status = InstallSmmSupvPublicInfoTable ();
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_ALLOCATE_FOR_GLOBAL_FAIL);
    goto Exit;
  }

  //
  // Initialize IDT
  //
  ZeroMem (&mSmiIdtr, sizeof (mSmiIdtr));
  InitializeSmmIdt (&mSmiIdtr);

  //
  // SMM supervisor time initialization
  //
  InitializeSmmTimer ();

  Status = IhvDrtmLibInit ();
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_IHV_DRTM_INIT_FAIL);
    goto Exit;
  }
  //
  // register SMM Ready To Lock Protocol notification
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmReadyToLockProtocolGuid,
                    SmmSupvSmmReadyToLockEventNotify,
                    &Registration
                    );
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_REGISTER_READYTOLOCK_CALLBACK_FAIL);
    goto Exit;
  }

Exit:

  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_MAIN_ENTRY_HAPPEN_FAIL_EVENT);
    PcdSetBoolS (PcdSmmIsolationEnable, FALSE);
    mPcdSmmIsolationEnable = FALSE;
  }
  ASSERT_EFI_ERROR (Status);

  TESTPOINT (SMMSUPV_MAIN_ENTRY_EXIT);
  DEBUG ((DEBUG_INFO, "SmmSupervisorEntry exit with - %r\n", Status));

  return EFI_SUCCESS;
}
