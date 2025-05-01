/** @file
  Instance of AMD SMM SaveState supervision layer. This library provides
  supervision over SMM SaveState.

  Copyright (c) Microsoft Corporation.
  Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Protocol/SmmCpuIo2.h>
#include <Library/BaseMemoryLib.h>
#include <SmmSecurePolicy.h>
#include <Library/SmmPolicyGateLib.h>
#include <Library/MemLogLib.h>
#include <Library/IhvSmmSaveStateSupervisionLib.h>

#define SMM_SAVE_STATE_OFFSET                 0xFE00
#define SMM_SAVE_STATE_SIZE                   0x0200
#define SMM_SAVE_STATE_IOTRAPOFFSET_OFFSET    0xFEC0
#define SMM_SAVE_STATE_RAX_OFFSET             0xFFF8

#pragma pack (push, 1)
typedef union {
  /// Bitfields of SMM SAVESTATE Core::X86::Smm::TrapOffset Register
  struct {
    UINT32 Type:1;                       ///< RW: port access type. Read-only,Volatile. Reset: 0. 0=IO Write (OUT instruction). 1=IO Read (IN instruction).
    UINT32 Valid:1;                      ///< IO trap word valid.
    UINT32 STR:1;                        ///< String-based port access.
    UINT32 REP:1;                        ///< repeated port access
    UINT32 SZ8:1;                        ///< size 8 bits
    UINT32 SZ16:1;                       ///< size 16 bits
    UINT32 SZ32:1;                       ///< size 32 bits
    UINT32 Reserved:4;                   ///< Reserved
    UINT32 TF:1;                         ///< EFLAGS TF value
    UINT32 BPR:4;                        ///< IO breakpoint match
    UINT32 Port:16;                      ///< trapped IO port address
  } Field;
  UINT32  Value;
} CORE_X86_SMM_TRAPOFFSET;
#pragma pack (pop)

typedef struct _SVST_OFFSET_TO_POLICY_MAP
{
  UINTN SaveStateOffset;
  UINT32 MapId;
} SVST_OFFSET_TO_POLICY_MAP;

//Structure converting SMM Save State offset to policy map id
SVST_OFFSET_TO_POLICY_MAP mSvstOffsetToPolicyMap[] = {
  {SMM_SAVE_STATE_RAX_OFFSET, SECURE_POLICY_SVST_RAX},
  {SMM_SAVE_STATE_IOTRAPOFFSET_OFFSET, SECURE_POLICY_SVST_IO_TRAP},
};

/**
  @brief Loop all cores to found if any core match IO condition specified by AccessCondition
  @param AccessCondition    - Save state content accessible condition
  @param Width              - Access width, need less than 8
  @param CpuSmmData         - Point to GATELIB_CPU_SMM_DATA

  @return BOOLEAN
                            - True: Access condition matched on one of the core
                            - FALSE: Access condition not found on any core
**/
BOOLEAN
CheckIoTrapOnAllCores (
  IN UINT32                             AccessCondition,
  IN UINTN                              Width,
  IN GATELIB_CPU_SMM_DATA               *CpuSmmData
  )
{
  UINT32  i;
  CORE_X86_SMM_TRAPOFFSET IoTrapReg;
  UINT32 IoWidth;
  UINT32 CurIoWidth;

  if (CpuSmmData == NULL) {
    return FALSE;
  }
  //Loop all cores to find the max valid IO access width
  IoWidth = 0;
  for (i = 0; i < CpuSmmData->NumberOfCpus; i++) {
    IoTrapReg.Value = *((UINT32 *) (CpuSmmData->SmBase[i] + SMM_SAVE_STATE_IOTRAPOFFSET_OFFSET));
    DEBUG ((DEBUG_VERBOSE, "Core %x IOTrap Value=%x\n", i, IoTrapReg.Value));
    //Check if it valid, and not repeat IO access
    if ((IoTrapReg.Field.Valid) && (IoTrapReg.Field.REP == 0)) {
      //Check the access width
      CurIoWidth = 0;
      if (IoTrapReg.Field.SZ8) {
        CurIoWidth = 1;
      }
      if (IoTrapReg.Field.SZ16) {
        CurIoWidth = 2;
      }
      if (IoTrapReg.Field.SZ32) {
        CurIoWidth = 4;
      }
      IoWidth = (CurIoWidth > IoWidth ? CurIoWidth: IoWidth);
    }
  }
  //Check if access width match IO access width
  DEBUG ((DEBUG_VERBOSE, "Width %d IoWidth %d\n", Width, IoWidth));
  if (Width > IoWidth) {
    return FALSE;
  }

  //Do the condition check
  for (i = 0; i < CpuSmmData->NumberOfCpus; i++) {
    IoTrapReg.Value = *((UINT32 *) (CpuSmmData->SmBase[i] + SMM_SAVE_STATE_IOTRAPOFFSET_OFFSET));
    //Check if it is valid, and not repeat IO access
    if ((IoTrapReg.Field.Valid) && (IoTrapReg.Field.REP == 0)) {
      //Check access R/W type match the condition
      //IoTrapReg.Field.Type 0=IO Write (OUT instruction). 1=IO Read (IN instruction).
      if ((AccessCondition == SECURE_POLICY_SVST_CONDITION_IO_WR) &&
          (IoTrapReg.Field.Type == 1)) {
        //Access type mismatch, no need to check further
        DEBUG ((DEBUG_VERBOSE, "SECURE_POLICY_SVST_CONDITION_IO_WR\n"));
        continue;
      }
      if ((AccessCondition == SECURE_POLICY_SVST_CONDITION_IO_RD) &&
          (IoTrapReg.Field.Type == 0)) {
        //Access type mismatch, no need to check further
        DEBUG ((DEBUG_VERBOSE, "SECURE_POLICY_SVST_CONDITION_IO_RD\n"));
        continue;
      }
      //Now we hit a valid condition
      DEBUG ((DEBUG_VERBOSE, "Cond-Allowed\n"));
      return TRUE;
    }
  }
  return FALSE;
}

/**
  @brief Given Smm save state address and access width, determine if it is
  allowed to access by parsing the policy

  @param SmmSecurityPolicy  - The address of applied SMM secure policy.
  @param CpuIndex           - Cpu
  @param SmmSaveStateAddr   - The physical address of SMM save state field
  @param Width         - Access width, need less than 8
  @param CpuSmmData         - Point to GATELIB_CPU_SMM_DATA

  @retval EFI_ACCESS_DENIED     The requested operation is not whitelisted by
                                the policy.
          EFI_INVALID_PARAMETER The SaveStateMapField needs to be within the
                                range of [0, SECURE_POLICY_INSTRUCTION_COUNT).
          EFI_SUCCESS           The requested operation is allowed by the
                                policy.
**/
EFI_STATUS
EFIAPI
IsIhvSmmSaveStateReadAllowed (
  IN SMM_SUPV_SECURE_POLICY_DATA_V1_0*  SmmSecurityPolicy,
  IN UINTN                              CpuIndex,
  IN UINTN                              SmmSaveStateAddr,
  IN UINTN                              Width,
  IN GATELIB_CPU_SMM_DATA               *CpuSmmData
  )
{
  EFI_STATUS                                          Status = EFI_SUCCESS;
  SMM_SUPV_POLICY_ROOT_V1                             *PolicyRoot      = NULL;
  UINT32                                              i;
  BOOLEAN                                             FoundMatch       = FALSE;
  SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0   *SvstPolicy      = NULL;
  UINTN SmmSaveStateOffset;

  UINT32 SvstMapId = SECURE_POLICY_SVST_COUNT;
  UINTN SmmSaveStateBase;

  DEBUG ((DEBUG_VERBOSE, "IsSmmSaveStateReadAllowed\n"));
  if ((SmmSecurityPolicy == NULL) || (CpuSmmData == NULL) || (Width > sizeof(UINT64)) ) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  SmmSaveStateBase = CpuSmmData->SmBase[CpuIndex];
  // Check if CpuIndex is out of bound or caller reaches out of save state region.
  if ((CpuIndex > CpuSmmData->NumberOfCpus) ||
      (SmmSaveStateAddr < SmmSaveStateBase + SMM_SAVE_STATE_OFFSET) ||
      ((SmmSaveStateAddr + Width) > (SmmSaveStateBase + SMM_SAVE_STATE_OFFSET + SMM_SAVE_STATE_SIZE)) ) {
    DEBUG ((DEBUG_ERROR, "%a Invalid SmmSaveState requested.\n", __FUNCTION__));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }


  //Check if SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_SAVE_STATE declared in policy structure
  PolicyRoot = (SMM_SUPV_POLICY_ROOT_V1*) ((UINTN) SmmSecurityPolicy + SmmSecurityPolicy->PolicyRootOffset);
  for (i = 0; i < SmmSecurityPolicy->PolicyRootCount; i++) {
    if (PolicyRoot[i].Type == SMM_SUPV_SECURE_POLICY_DESCRIPTOR_TYPE_SAVE_STATE) {
      PolicyRoot = &PolicyRoot[i];
      break;
    }
  }

  //If SAVE_STATE not declared, allow all read access
  if (i >= SmmSecurityPolicy->PolicyRootCount) {
    DEBUG ((DEBUG_VERBOSE, "%a Could not find SVST policy root, allow all\n", __FUNCTION__));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  SmmSaveStateOffset = SmmSaveStateAddr - SmmSaveStateBase;
  //Convert Offset to MapField ID
  for (i = 0; i < (sizeof (mSvstOffsetToPolicyMap) /sizeof (SVST_OFFSET_TO_POLICY_MAP)); i++) {
    if (mSvstOffsetToPolicyMap[i].SaveStateOffset == SmmSaveStateOffset) {
      SvstMapId = mSvstOffsetToPolicyMap[i].MapId;
      break;
    }
  }

  if (SvstMapId == SECURE_POLICY_SVST_COUNT) {
    //Unsupported offset founded
    DEBUG ((DEBUG_ERROR, "%a Unsupported Save state offset requested: %x\n", __FUNCTION__, SmmSaveStateOffset));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //If SAVE_STATE has been declared, loop the SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0 structures, and enforce the policy as declared
  SvstPolicy = (SMM_SUPV_SECURE_POLICY_SAVE_STATE_DESCRIPTOR_V1_0*) ((UINTN) SmmSecurityPolicy + PolicyRoot->Offset);
  for (i = 0; i < PolicyRoot->Count; i++) {
    if (SvstMapId == SvstPolicy[i].MapField) {
      if (SvstPolicy[i].Attributes & SECURE_POLICY_RESOURCE_ATTR_READ) {
        DEBUG ((DEBUG_VERBOSE, "Allowed\n"));
        FoundMatch = TRUE;
      } else if (SvstPolicy[i].Attributes & SECURE_POLICY_RESOURCE_ATTR_COND_READ) {
        //Check if condition matched
        if ((SvstPolicy[i].AccessCondition == SECURE_POLICY_SVST_CONDITION_IO_WR) ||
            (SvstPolicy[i].AccessCondition == SECURE_POLICY_SVST_CONDITION_IO_RD)) {
          //Loop all CPU savestate for condition access
          //In some corner case, SMM handler may access the SMMSaveState field which SMI not trigger by IO access on that core
          //e.g. Core 1 is reading 0x404, Core 3 is writing 0xB0 to trigger SMI, SMI handler request read EAX on Core 1
          //If we only check the core 1, gatelib will block the access, due it thought the SMI is caused by IO read.
          //In this case, we need loop all cores, and check their IOTRAP register, set FoundMatch if any core match the condition
          FoundMatch = CheckIoTrapOnAllCores (SvstPolicy[i].AccessCondition, Width, CpuSmmData);
        }
      }
      //We expected only one instance for each MapField
      break;
    }
  }
  if (FoundMatch) {
    DEBUG ((DEBUG_VERBOSE, "%a MatchedPolicy: [MapField: %d, Attributes:0x%x, AccessCondition:0x%x]\n",
    __FUNCTION__, SvstPolicy[i].MapField, SvstPolicy[i].Attributes, SvstPolicy[i].AccessCondition));
  }
  if ((FoundMatch && PolicyRoot->AccessAttr == SMM_SUPV_ACCESS_ATTR_DENY) ||
      (!FoundMatch && PolicyRoot->AccessAttr == SMM_SUPV_ACCESS_ATTR_ALLOW)) {

    //
    // We reject access based on:
    // 1. found a matching policy, reject access if this is a deny list
    // 2. did not find a matching policy, reject access if this is an allow list
    //
    DEBUG ((DEBUG_ERROR, "%a Rejecting SMM SaveState Read access based on policy walk through: Index: %d, AccessAttr: 0x%x.\n",
      __FUNCTION__, i, PolicyRoot->AccessAttr));
    MEMLOG ("Rejecting SMM SaveState Read access based on policy walk");
    Status = EFI_ACCESS_DENIED;
  }

Exit:
  return Status;
}

/**
  Read data from SmmSaveStateAddr with given Width

  @param Ret                - Point to return value
  @param CpuIndex           - Cpu
  @param SmmSaveStateAddr   - The physical address of SMM save state field
  @param Width              - Access width, need less than 8
**/
VOID
EFIAPI
IhvSmmSaveStateRead (
  IN OUT           VOID *Ret,
  IN UINTN         CpuIndex,
  IN UINTN         SmmSaveStateAddr,
  IN UINTN         Width
  )
{
  if ((Ret == NULL) || (SmmSaveStateAddr == 0)) {
    return;  //Invalid parameter
  }
  CopyMem (Ret, (VOID *)SmmSaveStateAddr, Width);
}