/** @file
SmmSupv secure policy

Copyright (c) 2009 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <SmmSecurePolicy.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/SmmSupervisorPolicyMgmtLib.h>
#include <Library/SmmSupervisorPageMgmtLib.h>
#include <Library/SmmSupervisorMemMgmtLib.h>
#include <Library/SmmSupvTestPointLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Library/BaseMemoryLib.h>


SMM_SUPV_SECURE_POLICY_DATA_V1_0 *mSmmTempPolicyData = NULL;  //Used to hold temporary policydata during DRTM boot time
SMM_SUPV_SECURE_POLICY_DATA_V1_0 *mSmmPolicyData = NULL;

/**
  Initialize the Desc passed in with Version, Type, and Size.

  @param[in, out] Desc Policy Memory Descriptor.
**/
VOID
EFIAPI
InitPolicyMemDescHdr (
  IN OUT SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *Desc
  )
{
  Desc->Header.Version = 1;
  Desc->Header.Type = SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM;
  Desc->Header.DescriptorSize = sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR);
}

/**
 * @brief Add new memory policy record (Legacy policy struct)
 * @details Add new memory policy record to pre-allocated policy buffer
 *          This routine assuming processing the record with the order of Address.
 *          Update the last record if the new record address is continual
 *          Or else create a new record in the buffer
 * @param pMemoryPolicy pre-allocated policy buffer
 * @param pMemoryPolicyCount Record count of pre-allocated policy buffer, add 1 if new record added
 * @param MemoryAttr The attribute of new record
 * @param PageTableBaseAddress Base address of new record
 * @param Size Size of new record
 * @return VOID
 */
VOID
EFIAPI
UpdateLegacyMemoryDesc (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR **pMemoryPolicy,
  UINT32 *pMemoryPolicyCount,
  UINT32 MemoryAttr,
  UINT64 PageTableBaseAddress,
  UINT64 Size
  )
{
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *MemoryPolicy;
// { volatile UINTN __i; __i = 1; while (__i); }

  MemoryPolicy = *pMemoryPolicy;
  // DEBUG ((DEBUG_INFO, "UpdateMemoryDesc %x %x \n", MemoryPolicy, pMemoryPolicyCount));
  //This is the 1st record
  if (MemoryPolicy->BaseAddress == MEM_DESC_UNINIT_BASEADDR) {
    MemoryPolicy->BaseAddress = PageTableBaseAddress;
    MemoryPolicy->Size = Size;
    MemoryPolicy->MemAttributes = MemoryAttr;
  } else {
    //Check if the continual address of Current memory policy
    if (PageTableBaseAddress == MemoryPolicy->BaseAddress + MemoryPolicy->Size) {
      //Check if attribute is the same
      if (MemoryAttr == MemoryPolicy->MemAttributes) {
        MemoryPolicy->Size += Size;
      } else {
        MemoryPolicy++;
        InitPolicyMemDescHdr (MemoryPolicy);
        MemoryPolicy->BaseAddress = PageTableBaseAddress;
        MemoryPolicy->Size = Size;
        MemoryPolicy->MemAttributes = MemoryAttr;
        *pMemoryPolicyCount += 1;
        *pMemoryPolicy = MemoryPolicy;
      }
    } else {
      MemoryPolicy++;
      InitPolicyMemDescHdr (MemoryPolicy);
      MemoryPolicy->BaseAddress = PageTableBaseAddress;
      MemoryPolicy->Size = Size;
      MemoryPolicy->MemAttributes = MemoryAttr;
      *pMemoryPolicyCount += 1;
      *pMemoryPolicy = MemoryPolicy;
    }
  }
}

/**
 * @brief Add new memory policy record
 * @details Add new memory policy record to pre-allocated policy buffer
 *          This routine assuming processing the record with the order of Address.
 *          Update the last record if the new record address is continual
 *          Or else create a new record in the buffer
 * @param pMemoryPolicy pre-allocated policy buffer
 * @param pMemoryPolicyCount Record count of pre-allocated policy buffer, add 1 if new record added
 * @param MemoryAttr The attribute of new record
 * @param PageTableBaseAddress Base address of new record
 * @param Size Size of new record
 * @return VOID
 */
VOID
EFIAPI
UpdateMemoryDesc (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 **pMemoryPolicy,
  UINT32 *pMemoryPolicyCount,
  UINT32 MemoryAttr,
  UINT64 PageTableBaseAddress,
  UINT64 Size
  )
{
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 *MemoryPolicy;

  MemoryPolicy = *pMemoryPolicy;
  // DEBUG ((DEBUG_INFO, "UpdateMemoryDesc %x %x \n", MemoryPolicy, pMemoryPolicyCount));
  //This is the 1st record
  if (MemoryPolicy->BaseAddress == MEM_DESC_UNINIT_BASEADDR) {
    MemoryPolicy->BaseAddress = PageTableBaseAddress;
    MemoryPolicy->Size = Size;
    MemoryPolicy->MemAttributes = MemoryAttr;
    MemoryPolicy->Reserved = 0;
  } else {
    //Check if the continual address of Current memory policy
    if ((PageTableBaseAddress == MemoryPolicy->BaseAddress + MemoryPolicy->Size) &&
        (MemoryAttr == MemoryPolicy->MemAttributes)) {
      MemoryPolicy->Size += Size;
    } else {
      MemoryPolicy++;
      MemoryPolicy->BaseAddress = PageTableBaseAddress;
      MemoryPolicy->Size = Size;
      MemoryPolicy->MemAttributes = MemoryAttr;
      MemoryPolicy->Reserved = 0;
      *pMemoryPolicyCount += 1;
      *pMemoryPolicy = MemoryPolicy;
    }
  }
}

/**
  Dump the memory policy data.
**/
VOID
DumpLegacyMemPolicyData (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *MemoryPolicy,
  UINT32 Count
  )
{
  UINT32 i;
  for (i = 0; i < Count; i++) {
    DEBUG ((DEBUG_INFO, "MEM: [%lx-%lx] %s %s %s\n",\
           MemoryPolicy[i].BaseAddress, \
           MemoryPolicy[i].BaseAddress + MemoryPolicy[i].Size -1, \
           (MemoryPolicy[i].MemAttributes & SECURE_POLICY_RESOURCE_ATTR_READ_DIS) ? "." : "R", \
           (MemoryPolicy[i].MemAttributes & SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS) ? "." : "W", \
           (MemoryPolicy[i].MemAttributes & SECURE_POLICY_RESOURCE_ATTR_EXECUTE_DIS) ? "." : "X"
    ));
  }
}

/**
  Dump a single memory policy data.
**/
VOID
DumpMemPolicyEntry (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 *MemoryPolicy
  )
{
  if (MemoryPolicy == NULL) {
    DEBUG ((DEBUG_INFO, "%a Received null input pointer!\n", __FUNCTION__));
    ASSERT (FALSE);
    return;
  }

  DEBUG ((DEBUG_INFO, "V1.0 MEM: [%lx-%lx] %s %s %s\n",\
          MemoryPolicy->BaseAddress, \
          MemoryPolicy->BaseAddress + MemoryPolicy->Size -1, \
          (MemoryPolicy->MemAttributes & SECURE_POLICY_RESOURCE_ATTR_READ) ? "R" : ".", \
          (MemoryPolicy->MemAttributes & SECURE_POLICY_RESOURCE_ATTR_WRITE) ? "W" : ".", \
          (MemoryPolicy->MemAttributes & SECURE_POLICY_RESOURCE_ATTR_EXECUTE) ? "X" : "."
  ));
}

/**
  Update the smm policy data.
**/
VOID
DumpSmmPolicyData (
  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *Data
  )
{
  UINT32 i;
  UINT32 j;

  SMM_SUPV_POLICY_ROOT_V1                             *PolicyRoot;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR               *LegacyMemoryPolicy;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0          *MemoryPolicy;
  SMM_SUPV_SECURE_POLICY_IO_DESCRIPTOR_V1_0           *IoPolicy;
  SMM_SUPV_SECURE_POLICY_MSR_DESCRIPTOR_V1_0          *MsrPolicy;
  SMM_SUPV_SECURE_POLICY_INSTRUCTION_DESCRIPTOR_V1_0  *InstructionPolicy;
  SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0   *SaveStatePolicy;

  DEBUG ((DEBUG_INFO, "SMM_SUPV_SECURE_POLICY_DATA_V1_0:\n"));
  DEBUG ((DEBUG_INFO, "Version Major:%x\n", Data->VersionMajor));
  DEBUG ((DEBUG_INFO, "Version Minor:%x\n", Data->VersionMinor));
  DEBUG ((DEBUG_INFO, "Size:0x%x\n", Data->Size));
  DEBUG ((DEBUG_INFO, "MemoryPolicyOffset:0x%x\n", Data->MemoryPolicyOffset));
  DEBUG ((DEBUG_INFO, "MemoryPolicyCount:0x%x\n", Data->MemoryPolicyCount));
  DEBUG ((DEBUG_INFO, "Flags:%x\n", Data->Flags));
  DEBUG ((DEBUG_INFO, "Capabilities:%x\n", Data->Capabilities));
  DEBUG ((DEBUG_INFO, "PolicyRootOffset:0x%x\n", Data->PolicyRootOffset));
  DEBUG ((DEBUG_INFO, "PolicyRootCount:0x%x\n", Data->PolicyRootCount));

  PolicyRoot = (SMM_SUPV_POLICY_ROOT_V1 *) ((UINTN) Data + Data->PolicyRootOffset);
  // Iterate through each policy root
  for (i = 0; i < Data->PolicyRootCount; i++) {
    DEBUG ((DEBUG_INFO, "Policy Root:\n"));
    DEBUG ((DEBUG_INFO, "  Version: %x\n", PolicyRoot[i].Version));
    DEBUG ((DEBUG_INFO, "  PolicyRootSize: %x\n",  PolicyRoot[i].PolicyRootSize));
    DEBUG ((DEBUG_INFO, "  Type: %x\n",  PolicyRoot[i].Type));
    DEBUG ((DEBUG_INFO, "  Offset: %x\n",  PolicyRoot[i].Offset));
    DEBUG ((DEBUG_INFO, "  Count: %x\n",  PolicyRoot[i].Count));
    DEBUG ((DEBUG_INFO, "  AccessAttr: %a\n",  (PolicyRoot[i].AccessAttr == SMM_SUPV_ACCESS_ATTR_ALLOW) ? "ALLOW" : "DENY"));
    // Iterate through each policy descriptor described by this policy root
    for (j = 0; j < PolicyRoot[i].Count; j++) {
      if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM) {
        // Dump Memory Policy
        MemoryPolicy = (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 *) ((UINTN) Data + PolicyRoot[i].Offset);
        DumpMemPolicyEntry (&MemoryPolicy[j]);
      }
      else if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_IO) {
        // Dump IoPolicy
        IoPolicy = (SMM_SUPV_SECURE_POLICY_IO_DESCRIPTOR_V1_0 *) ((UINTN) Data + PolicyRoot[i].Offset);
        DEBUG ((DEBUG_INFO, "IO: [%lx-%lx] %s %s\n",\
              IoPolicy[j].IoAddress, \
              IoPolicy[j].IoAddress + IoPolicy[j].LengthOrWidth - 1, \
              (IoPolicy[j].Attributes & SECURE_POLICY_RESOURCE_ATTR_READ) ? "R" : ".", \
              (IoPolicy[j].Attributes & SECURE_POLICY_RESOURCE_ATTR_WRITE) ? "W" : "." \
        ));
      }
      else if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MSR) {
        // Dump MsrPolicy
        MsrPolicy = (SMM_SUPV_SECURE_POLICY_MSR_DESCRIPTOR_V1_0 *) ((UINTN) Data + PolicyRoot[i].Offset);
        DEBUG ((DEBUG_INFO, "MSR: [%lx-%lx] %s %s\n",\
              MsrPolicy[j].MsrAddress, \
              MsrPolicy[j].MsrAddress + MsrPolicy[j].Length - 1, \
              (MsrPolicy[j].Attributes & SECURE_POLICY_RESOURCE_ATTR_READ) ? "R" : ".", \
              (MsrPolicy[j].Attributes & SECURE_POLICY_RESOURCE_ATTR_WRITE) ? "W" : "." \
        ));
      }
      else if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_INSTRUCTION) {
        // Dump InstructionPolicy
        InstructionPolicy = (SMM_SUPV_SECURE_POLICY_INSTRUCTION_DESCRIPTOR_V1_0 *)  ((UINTN) Data + PolicyRoot[i].Offset);
        DEBUG ((DEBUG_INFO, "INSTRUCTION: [%lx] %s\n",\
              InstructionPolicy[j].InstructionIndex, \
              (InstructionPolicy[j].Attributes & SECURE_POLICY_RESOURCE_ATTR_EXECUTE) ? "X" : "."
        ));
      }
      else if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_SAVE_STATE) {
        // Dump SaveStatePolicy
        SaveStatePolicy = (SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0 *)  ((UINTN) Data + PolicyRoot[i].Offset);
        DEBUG ((DEBUG_INFO, "SAVESTATE: [%lx] %x %a\n",\
              SaveStatePolicy[j].MapField, \
              (SaveStatePolicy[j].Attributes), \
              ((SaveStatePolicy[j].AccessCondition == SECURE_POLICY_SVST_UNCONDITIONAL) ? "Unconditional" :
               ((SaveStatePolicy[j].AccessCondition == SECURE_POLICY_SVST_CONDITION_IO_RD) ? "IoRead" : "IoWrite"))
        ));
      }
      else {
        DEBUG ((DEBUG_ERROR, "Unrecognized policy root type found %x, bailing!!!\n", PolicyRoot[i].Type));
        return;
      }
    }
  }

  if (FeaturePcdGet (SupportLegacyMemoryPolicy)) {
    DEBUG ((DEBUG_INFO, "Legacy memory policy entries:\n"));
    DEBUG ((DEBUG_INFO, "  Offset: 0x%p\n", Data->MemoryPolicyOffset));
    DEBUG ((DEBUG_INFO, "  Count: 0x%x\n", Data->MemoryPolicyCount));
    LegacyMemoryPolicy = (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *) ((UINT8 *)Data + Data->MemoryPolicyOffset);
    DumpLegacyMemPolicyData (LegacyMemoryPolicy, Data->MemoryPolicyCount);
  }
}

/**
  Policy validity check for a given security policy. Check covers policy range
  overlap, policy entry header type mismatch, etc.

  @param[in]  SmmSecurityPolicy - The address of applied SMM secure policy.

  @retval EFI_SECURITY_VIOLATION  The supplied policy failed checking due to
                                  overlapping, type mismatch, etc.
          EFI_INVALID_PARAMETER   The supplied policy pointer is a null pointer.
          EFI_SUCCESS             The supplied policy has passed supervisor
                                  checking.
**/
EFI_STATUS
EFIAPI
SecurityPolicyCheck (
  IN SMM_SUPV_SECURE_POLICY_DATA_V1_0     *SmmSecurityPolicy
)
{
  EFI_STATUS                                          Status;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR               *LegacyMemDescriptors = NULL;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0          *MemDescriptors       = NULL;
  SMM_SUPV_SECURE_POLICY_IO_DESCRIPTOR_V1_0           *IoDescriptors        = NULL;
  SMM_SUPV_SECURE_POLICY_MSR_DESCRIPTOR_V1_0          *MsrDescriptors       = NULL;
  SMM_SUPV_SECURE_POLICY_INSTRUCTION_DESCRIPTOR_V1_0  *InstrDescriptors     = NULL;
  SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0   *SvstDescriptors      = NULL;
  SMM_SUPV_POLICY_ROOT_V1                             *PolicyRoot           = NULL;
  UINT64                                              TypeDuplicationFlag   = 0;
  UINT8                                               *CurrentOffset        = NULL;
  UINT8                                               *PreviousOffset       = NULL;
  UINTN                                               Index0;
  UINTN                                               Index1;
  UINTN                                               Index2;
  UINTN                                               TempAddress;
  UINTN                                               TempSize;
  UINTN                                               TotalScannedSize;
  BOOLEAN                                             IsOverlapping;

  DEBUG ((DEBUG_INFO, "%a - Policy overlap check entry ...\n", __FUNCTION__));

  if (SmmSecurityPolicy == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (SmmSecurityPolicy->Reserved != 0 &&
      SmmSecurityPolicy->Flags != 0 &&
      SmmSecurityPolicy->Capabilities != 0) {
    DEBUG ((DEBUG_ERROR, "%a - Secure policy header has unrecognized bits set.\n", __FUNCTION__));
    Status = EFI_SECURITY_VIOLATION;
    goto Exit;
  }

  // Keep a total size to indicate all the scanned portions, the final size should match the size from header
  TotalScannedSize = sizeof (SMM_SUPV_SECURE_POLICY_DATA_V1_0);

  PolicyRoot = (SMM_SUPV_POLICY_ROOT_V1*) ((UINTN) SmmSecurityPolicy + SmmSecurityPolicy->PolicyRootOffset);
  for (Index0 = 0; Index0 < SmmSecurityPolicy->PolicyRootCount; Index0++) {
    if (PolicyRoot[Index0].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_IO) {
      // IO Policy Overlap Check
      if (TypeDuplicationFlag & (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_IO)) {
        DEBUG ((DEBUG_INFO, "%a - Duplicated IO policy root found ...\n", __FUNCTION__));
        Status = EFI_SECURITY_VIOLATION;
        goto Exit;
      }
      TypeDuplicationFlag |= (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_IO);
      IoDescriptors = (SMM_SUPV_SECURE_POLICY_IO_DESCRIPTOR_V1_0*) ((UINTN) SmmSecurityPolicy + PolicyRoot[Index0].Offset);
      for (Index1 = 0; Index1 < PolicyRoot[Index0].Count; Index1++) {
        for (Index2 = 0; Index2 < Index1; Index2++) {
          TempAddress = IoDescriptors[Index1].IoAddress;
          TempSize = IoDescriptors[Index1].LengthOrWidth;

          // Naively iterate through all entries to check overlap
          if (IoDescriptors[Index2].Attributes & SECURE_POLICY_RESOURCE_ATTR_STRICT_WIDTH) {
            // Strict width entry will only be shadowed by its superset
            if ((TempAddress <= IoDescriptors[Index2].IoAddress) &&
                (TempAddress + TempSize >= (UINT32) IoDescriptors[Index2].IoAddress + IoDescriptors[Index2].LengthOrWidth)) {
              DEBUG ((DEBUG_ERROR, "%a - IO policy strict width overlap check failed\n", __FUNCTION__));
              Status = EFI_SECURITY_VIOLATION;
              goto Exit;
            }
          } else {
            // Otherwise, any overlap will count as policy check failure
            Status = OverlapStatus (
                      (UINTN) IoDescriptors[Index2].IoAddress,
                      (UINTN) IoDescriptors[Index2].LengthOrWidth,
                      TempAddress,
                      TempSize,
                      &IsOverlapping);
            if (EFI_ERROR (Status) || IsOverlapping) {
              DEBUG ((DEBUG_ERROR, "%a - IO policy overlap check failed - %r\n", __FUNCTION__, Status));
              Status = EFI_SECURITY_VIOLATION;
              goto Exit;
            }
          }
        }
        if (IoDescriptors[Index1].Reserved != 0) {
          DEBUG ((DEBUG_ERROR, "%a - IO policy has non zero reserved field.\n", __FUNCTION__));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }
        TotalScannedSize += sizeof (SMM_SUPV_SECURE_POLICY_IO_DESCRIPTOR_V1_0);
      }
      TotalScannedSize += sizeof (SMM_SUPV_POLICY_ROOT_V1);
    }
    else if (PolicyRoot[Index0].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM) {
      // Memory Policy Overlap Check
      if (TypeDuplicationFlag & (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM)) {
        DEBUG ((DEBUG_INFO, "%a - Duplicated Memory policy root found ...\n", __FUNCTION__));
        Status = EFI_SECURITY_VIOLATION;
        goto Exit;
      }
      TypeDuplicationFlag |= (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM);
      MemDescriptors = (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0*) ((UINTN) SmmSecurityPolicy + PolicyRoot[Index0].Offset);
      for (Index1 = 0; Index1 < PolicyRoot[Index0].Count; Index1++) {
        for (Index2 = 0; Index2 < Index1; Index2++) {
          TempAddress = MemDescriptors[Index1].BaseAddress;
          TempSize = MemDescriptors[Index1].Size;

          // Naively iterate through all entries to check overlap
          Status = OverlapStatus (
                    (UINTN) MemDescriptors[Index2].BaseAddress,
                    (UINTN) MemDescriptors[Index2].Size,
                    TempAddress,
                    TempSize,
                    &IsOverlapping);
          if (EFI_ERROR (Status) || IsOverlapping) {
            DEBUG ((DEBUG_ERROR, "%a - Memory policy overlap check failed - %r\n", __FUNCTION__, Status));
            Status = EFI_SECURITY_VIOLATION;
            goto Exit;
          }
        }
        if (MemDescriptors[Index1].Reserved != 0) {
          DEBUG ((DEBUG_ERROR, "%a - Mem policy has non zero reserved field.\n", __FUNCTION__));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }
        TotalScannedSize += sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0);
      }
      TotalScannedSize += sizeof (SMM_SUPV_POLICY_ROOT_V1);
    }
    else if (PolicyRoot[Index0].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MSR) {
      // MSR Policy Overlap Check
      if (TypeDuplicationFlag & (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MSR)) {
        DEBUG ((DEBUG_INFO, "%a - Duplicated MSR policy root found ...\n", __FUNCTION__));
        Status = EFI_SECURITY_VIOLATION;
        goto Exit;
      }
      TypeDuplicationFlag |= (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MSR);
      MsrDescriptors = (SMM_SUPV_SECURE_POLICY_MSR_DESCRIPTOR_V1_0*) ((UINTN) SmmSecurityPolicy + PolicyRoot[Index0].Offset);
      for (Index1 = 0; Index1 < PolicyRoot[Index0].Count; Index1++) {
        for (Index2 = 0; Index2 < Index1; Index2++) {
          TempAddress = MsrDescriptors[Index1].MsrAddress;
          TempSize = MsrDescriptors[Index1].Length;

          // Naively iterate through all entries to check overlap
          Status = OverlapStatus (
                    (UINTN) MsrDescriptors[Index2].MsrAddress,
                    (UINTN) MsrDescriptors[Index2].Length,
                    TempAddress,
                    TempSize,
                    &IsOverlapping);
          if (EFI_ERROR (Status) || IsOverlapping) {
            DEBUG ((DEBUG_ERROR, "%a - MSR policy overlap check failed - %r\n", __FUNCTION__, Status));
            Status = EFI_SECURITY_VIOLATION;
            goto Exit;
          }
        }
        TotalScannedSize += sizeof (SMM_SUPV_SECURE_POLICY_MSR_DESCRIPTOR_V1_0);
      }
      TotalScannedSize += sizeof (SMM_SUPV_POLICY_ROOT_V1);
    }
    else if (PolicyRoot[Index0].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_INSTRUCTION) {
      // Instruction Policy Duplication Check
      if (TypeDuplicationFlag & (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_INSTRUCTION)) {
        DEBUG ((DEBUG_INFO, "%a - Duplicated Instruction policy root found ...\n", __FUNCTION__));
        Status = EFI_SECURITY_VIOLATION;
        goto Exit;
      }
      TypeDuplicationFlag |= (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_INSTRUCTION);
      InstrDescriptors = (SMM_SUPV_SECURE_POLICY_INSTRUCTION_DESCRIPTOR_V1_0*) ((UINTN) SmmSecurityPolicy + PolicyRoot[Index0].Offset);
      for (Index1 = 0; Index1 < PolicyRoot[Index0].Count; Index1++) {
        for (Index2 = 0; Index2 < Index1; Index2++) {
          // Naively iterate through all entries to check duplication
          if (InstrDescriptors[Index1].InstructionIndex == InstrDescriptors[Index2].InstructionIndex) {
            DEBUG ((DEBUG_ERROR, "%a - Instruction policy duplication check failed - %r\n", __FUNCTION__, Status));
            Status = EFI_SECURITY_VIOLATION;
            goto Exit;
          }
        }
        if (InstrDescriptors[Index1].Reserved != 0) {
          DEBUG ((DEBUG_ERROR, "%a - Instruction policy has non zero reserved field.\n", __FUNCTION__));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }
        TotalScannedSize += sizeof (SMM_SUPV_SECURE_POLICY_INSTRUCTION_DESCRIPTOR_V1_0);
      }
      TotalScannedSize += sizeof (SMM_SUPV_POLICY_ROOT_V1);
    }
    else if (PolicyRoot[Index0].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_SAVE_STATE) {
      // Save State Policy Duplication Check
      if (TypeDuplicationFlag & (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_SAVE_STATE)) {
        DEBUG ((DEBUG_INFO, "%a - Duplicated Save state policy root found ...\n", __FUNCTION__));
        Status = EFI_SECURITY_VIOLATION;
        goto Exit;
      }
      TypeDuplicationFlag |= (BIT0 << SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_SAVE_STATE);
      SvstDescriptors = (SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0*) ((UINTN) SmmSecurityPolicy + PolicyRoot[Index0].Offset);
      for (Index1 = 0; Index1 < PolicyRoot[Index0].Count; Index1++) {
        for (Index2 = 0; Index2 < Index1; Index2++) {
          // Naively iterate through all entries to check duplication
          // This means we allow different attributes on the same map field
          if (SvstDescriptors[Index1].MapField == SvstDescriptors[Index2].MapField &&
              SvstDescriptors[Index1].Attributes == SvstDescriptors[Index2].Attributes) {
            DEBUG ((DEBUG_ERROR, "%a - Save state policy duplication check failed - %r\n", __FUNCTION__, Status));
            Status = EFI_SECURITY_VIOLATION;
            goto Exit;
          }
        }
        // Do not allow write attributes for this version of supervisor, also conditional attribute and unconditional ones are mutually exclusive
        if (((SvstDescriptors[Index1].Attributes & (SECURE_POLICY_RESOURCE_ATTR_WRITE | SECURE_POLICY_RESOURCE_ATTR_COND_WRITE)) != 0) ||
            ((SvstDescriptors[Index1].Attributes & (SECURE_POLICY_RESOURCE_ATTR_READ | SECURE_POLICY_RESOURCE_ATTR_COND_READ)) ==
             (SECURE_POLICY_RESOURCE_ATTR_READ | SECURE_POLICY_RESOURCE_ATTR_COND_READ))) {
          DEBUG ((DEBUG_ERROR, "%a - Save state policy has conflicting attributes %x on mapfield %x.\n",
                  __FUNCTION__,
                  SvstDescriptors[Index1].Attributes,
                  SvstDescriptors[Index1].MapField));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }
        // Unconditional entries shall not have conditions specified
        // Only conditional read is checked here since we do not allow write attributes for this version of supervisor
        if ((SvstDescriptors[Index1].Attributes & SECURE_POLICY_RESOURCE_ATTR_COND_READ) == 0 &&
            SvstDescriptors[Index1].AccessCondition != SECURE_POLICY_SVST_UNCONDITIONAL) {
          DEBUG ((DEBUG_ERROR, "%a - Save state policy has conflicting condition on attributes.\n", __FUNCTION__));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }
        if (SvstDescriptors[Index1].Reserved != 0) {
          DEBUG ((DEBUG_ERROR, "%a - Save state policy has non zero reserved field.\n", __FUNCTION__));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }
        TotalScannedSize += sizeof (SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0);
      }
      TotalScannedSize += sizeof (SMM_SUPV_POLICY_ROOT_V1);
    }
    else {
      DEBUG ((DEBUG_ERROR, "%a - Unrecognized policy type check %x\n", __FUNCTION__, PolicyRoot[Index0].Type));
      Status = EFI_SECURITY_VIOLATION;
      goto Exit;
    }
    if (!IsZeroBuffer (PolicyRoot[Index0].Reserved, sizeof (PolicyRoot[Index0].Reserved))) {
      DEBUG ((DEBUG_ERROR, "%a - Policy root has non zero reserved field.\n", __FUNCTION__));
      Status = EFI_SECURITY_VIOLATION;
      goto Exit;
    }
  }

  // Legacy Memory Policy Overlap Check
  if (FeaturePcdGet (SupportLegacyMemoryPolicy)) {
    CurrentOffset = (UINT8*) SmmSecurityPolicy + SmmSecurityPolicy->MemoryPolicyOffset;
    for (Index1 = 0; Index1 < SmmSecurityPolicy->MemoryPolicyCount; Index1++) {
      LegacyMemDescriptors = (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR*) CurrentOffset;

      // Check descriptor type first
      if (LegacyMemDescriptors->Header.Type != SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM) {
        DEBUG ((DEBUG_ERROR, "%a - Memory policy header type check failed\n", __FUNCTION__));
        Status = EFI_SECURITY_VIOLATION;
        goto Exit;
      }

      PreviousOffset = (UINT8*)SmmSecurityPolicy + SmmSecurityPolicy->MemoryPolicyOffset;
      for (Index2 = 0; Index2 < Index1; Index2++) {
        TempAddress = ((SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR*) PreviousOffset)->BaseAddress;
        TempSize = ((SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR*) PreviousOffset)->Size;

        // Naively iterate through all entries to check overlap
        Status = OverlapStatus (
                  (UINTN) LegacyMemDescriptors->BaseAddress,
                  (UINTN) LegacyMemDescriptors->Size,
                  TempAddress,
                  TempSize,
                  &IsOverlapping);
        if (EFI_ERROR (Status) || IsOverlapping) {
          DEBUG ((DEBUG_ERROR, "%a - Memory policy overlap check failed - %r\n", __FUNCTION__, Status));
          Status = EFI_SECURITY_VIOLATION;
          goto Exit;
        }

        PreviousOffset += ((SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR*) PreviousOffset)->Header.DescriptorSize;
      }

      CurrentOffset += LegacyMemDescriptors->Header.DescriptorSize;
      TotalScannedSize += sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR);
    }
  }

  if (TotalScannedSize != SmmSecurityPolicy->Size) {
    DEBUG ((DEBUG_ERROR, "%a - Unrecognized bytes detected in the policy (expecting 0x%x, has 0x%x), not allowed!\n",
            __FUNCTION__,
            TotalScannedSize,
            SmmSecurityPolicy->Size));
    Status = EFI_SECURITY_VIOLATION;
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Policy overlap check failed - %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "%a - Policy overlap check exit ...\n", __FUNCTION__));
  return Status;
}


/**
  Initializes internal policy data based on SMM Isolation Policy.

  @param[in] SmmIsolationPolicy   SMM Isolation Policy stored in FW.
  @param[in] Size                 Size of SMM Isolation Policy.
**/
EFI_STATUS
EFIAPI
InitializePolicyData (
  IN VOID   *SmmIsolationPolicy,
  IN UINT32 Size
  )
{
  if (Size > SMM_POLICY_DATA_SIZE) {
    DEBUG ((DEBUG_ERROR, "Policy information is too large.\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  mSmmPolicyData = (SMM_SUPV_SECURE_POLICY_DATA_V1_0 *) AllocateSmmSupvPages (EFI_SIZE_TO_PAGES (SMM_POLICY_DATA_SIZE));
  if (mSmmPolicyData == NULL) {
    WriteErrorData (SMMSUPV_ALLOCATE_PAGE_FAIL);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "mSmmPolicyData %x\n", mSmmPolicyData));
  //Put this policy at allocated place here, wait for ReadyToLock will make FindSmmSupvEntry fail with #PF
  ZeroMem (mSmmPolicyData, SMM_POLICY_DATA_SIZE);
  CopyMem (mSmmPolicyData, SmmIsolationPolicy, Size);

  if (((SMM_SUPV_SECURE_POLICY_DATA_V1_0 *)SmmIsolationPolicy)->VersionMajor < 1) {
    DEBUG ((DEBUG_ERROR, "SmmIsolationPolicy has legacy version that not supported by this firmware: V%x.%x\n",
              ((SMM_SUPV_SECURE_POLICY_DATA_V1_0 *)SmmIsolationPolicy)->VersionMajor,
              ((SMM_SUPV_SECURE_POLICY_DATA_V1_0 *)SmmIsolationPolicy)->VersionMinor));
    ASSERT (EFI_INCOMPATIBLE_VERSION);
    return EFI_INCOMPATIBLE_VERSION;
  }

  mSmmTempPolicyData = (SMM_SUPV_SECURE_POLICY_DATA_V1_0 *) AllocateSmmSupvPages (EFI_SIZE_TO_PAGES (SMM_POLICY_DATA_SIZE));
  if (mSmmTempPolicyData == NULL) {
    WriteErrorData (SMMSUPV_ALLOCATE_PAGE_FAIL);
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG ((DEBUG_INFO, "mSmmTempPolicyData %x\n", mSmmTempPolicyData));
  return EFI_SUCCESS;
}

/**
  Helper function that populates memory policy on demands.

  @param[in] SmmPolicyBuffer   Input buffer points to the entire v1.0 policy.
  @param[in] Cr3               CR3 value to be converted, if input is zero, check the real HW register.
  @param[in] IsShadow          This function will shadow current pagetable if set.

  @param[in] CpuIndex Logical number assigned to CPU.
**/
EFI_STATUS
EFIAPI
PopulateMemoryPolicyEntries (
  IN  SMM_SUPV_SECURE_POLICY_DATA_V1_0    *SmmPolicyBuffer,
  IN  UINT64                              Cr3,
  IN  BOOLEAN                             IsShadow
)
{
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR       *LegacyMemoryPolicy;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0  *MemoryPolicy;
  SMM_SUPV_POLICY_ROOT_V1                     *PolicyRoot;
  UINTN                                       MemoryPolicySize;
  UINTN                                       i;
  EFI_STATUS                                  Status;

  if (SmmPolicyBuffer == NULL) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "%a Incoming policy buffer is null pointer.\n", __FUNCTION__));
    goto Exit;
  }

  //IO and MSR policies are populated during report DRTM info time,
  //Here just append MemPolicy to the end of static table.
  PolicyRoot = (SMM_SUPV_POLICY_ROOT_V1 *) ((UINTN) SmmPolicyBuffer + SmmPolicyBuffer->PolicyRootOffset);
  for (i = 0; i < SmmPolicyBuffer->PolicyRootCount; i++) {
    if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM) {
      PolicyRoot = PolicyRoot + i;
      break;
    }
  }

  if (i >= SmmPolicyBuffer->PolicyRootCount) {
    // TODO: Do we want to add handling here?
    // Something is wrong, there is not placeholder left for memory type, do not want to handle it...
    DEBUG ((DEBUG_ERROR, "%a Incoming policy buffer does not contain memory type policy root.\n", __FUNCTION__));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //Init PolicyRoot->Offset field
  PolicyRoot->AccessAttr = SMM_SUPV_ACCESS_ATTR_ALLOW;
  if (PolicyRoot->Offset == 0) {
    // Only populate the offset if not already set
    PolicyRoot->Offset = SmmPolicyBuffer->Size;
  }
  PolicyRoot->PolicyRootSize = sizeof (SMM_SUPV_POLICY_ROOT_V1);
  // This is not needed with our check above
  // PolicyRoot->Type = SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_MEM;
  PolicyRoot->Version = 1;
  MemoryPolicy = (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 *) ((UINTN) SmmPolicyBuffer + PolicyRoot->Offset);
  MemoryPolicySize = SMM_POLICY_DATA_SIZE - PolicyRoot->Offset - 1;
  //Generate Policy of current Pagetable
  Status = GenMemPolicyAndShadowPageTable (Cr3, MemoryPolicy, MemoryPolicySize, &PolicyRoot->Count, IsShadow, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Fail to GenMemPolicyAndShadowPageTable for non-legacy structures %r\n", __FUNCTION__, Status));
    goto Exit;
  }
  SmmPolicyBuffer->Size = PolicyRoot->Offset + (sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0) * PolicyRoot->Count);

  // Then populate the legacy structures if supported
  if (FeaturePcdGet (SupportLegacyMemoryPolicy)) {
    LegacyMemoryPolicy = (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *) ((UINTN) SmmPolicyBuffer + SmmPolicyBuffer->Size);
    SmmPolicyBuffer->MemoryPolicyOffset = SmmPolicyBuffer->Size;
    MemoryPolicySize = SMM_POLICY_DATA_SIZE - SmmPolicyBuffer->MemoryPolicyOffset - 1;
    //Generate Policy and shadow current Pagetable
    //Shadow the pagetable on BSP only, as all processors are sharing one pagetable copy
    Status = GenMemPolicyAndShadowPageTable (Cr3, LegacyMemoryPolicy, MemoryPolicySize, &SmmPolicyBuffer->MemoryPolicyCount, IsShadow, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Fail to GenMemPolicyAndShadowPageTable for legacy structures %r\n", __FUNCTION__, Status));
      goto Exit;
    }
    SmmPolicyBuffer->Size = (UINT32) SmmPolicyBuffer->Size + (sizeof (SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR) * SmmPolicyBuffer->MemoryPolicyCount);
  }
  else {
    SmmPolicyBuffer->MemoryPolicyCount = 0;
  }

Exit:
  return Status;
}

/**
  Called after SMM Read to lock,
  Update memory policy,
  Perform policy sanity check
  @param[in] CpuIndex Logical number assigned to CPU.
**/
VOID
EFIAPI
UpdateMemoryPolicy (
  IN        UINT64                     CpuIndex
  )
{
  EFI_STATUS Status;

  if (CpuIndex == 0) {
    DEBUG ((DEBUG_INFO, "UpdateMemoryPolicy\n"));

    // We only populate the memory descriptor entries into mSmmPolicyData here
    Status = PopulateMemoryPolicyEntries (mSmmPolicyData, 0, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Fail to PopulateMemoryPolicyEntries %r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
    }

    TESTPOINT (SMMSUPV_SECURITY_POLICY_CHECK_START);
    Status = SecurityPolicyCheck (mSmmPolicyData);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Security Policy Overlap Check failed - %r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
    }
    TESTPOINT (SMMSUPV_SECURITY_POLICY_CHECK_END);
  }
}

/**
  Return pointer to internal SMM Policy Data.

  @retval Pointer to internal SMM Policy Data.
**/
SMM_SUPV_SECURE_POLICY_DATA_V1_0 *
EFIAPI
GetSmmPolicyData (
  VOID
)
{
  return mSmmPolicyData;
}

/**
  Return pointer to internal SMM Temp Policy Data.

  @retval Pointer to internal SMM Temp Policy Data.
**/
SMM_SUPV_SECURE_POLICY_DATA_V1_0 *
EFIAPI
GetSmmTempPolicyData (
  VOID
)
{
  return mSmmTempPolicyData;
}