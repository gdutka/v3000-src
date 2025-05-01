/** @file
    Provide support functions for variable lock service.

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "VariableLock.h"
#include "VariablePolicy.h"
///
/// The list to store the variables which cannot be set after the EFI_END_OF_DXE_EVENT_GROUP_GUID
/// or EVT_GROUP_READY_TO_BOOT event.
///
LIST_ENTRY             mLockedVariableList    = INITIALIZE_LIST_HEAD_VARIABLE (mLockedVariableList);
BOOLEAN                mEnableLocking         = TRUE;

/**
  Acquires lock only at boot time. Simply returns at runtime.

  This is a temperary function that will be removed when
  EfiAcquireLock() in UefiLib can handle the call in UEFI
  Runtimer driver in RT phase.
  It calls EfiAcquireLock() at boot time, and simply returns
  at runtime.

  @param[in]  Lock         A pointer to the lock to acquire.

**/
VOID
AcquireLockOnlyAtBootTime (
  IN EFI_LOCK                             *Lock
  )
{
  if (mSmst == NULL && !VariableAtRuntime ()) {
    EfiAcquireLock (Lock);
  }
}


/**
  Releases lock only at boot time. Simply returns at runtime.

  This is a temperary function which will be removed when
  EfiReleaseLock() in UefiLib can handle the call in UEFI
  Runtimer driver in RT phase.
  It calls EfiReleaseLock() at boot time and simply returns
  at runtime.

  @param[in]  Lock         A pointer to the lock to release.

**/
VOID
ReleaseLockOnlyAtBootTime (
  IN EFI_LOCK                             *Lock
  )
{
  if (mSmst == NULL && !VariableAtRuntime ()) {
    EfiReleaseLock (Lock);
  }
}

/**
  Internal function to check this variable is whether in variable lock list

  @param[in] VariableName  Name of Variable to be found.
  @param[in] VendorGuid    Variable vendor GUID.

  @retval TRUE             This variable is in variable lock list.
  @retval FALSE            This variable isn't in variable lock list.
**/
STATIC
BOOLEAN
IsVariableInLockList (
  IN CHAR16                  *VariableName,
  IN EFI_GUID                *VendorGuid
  )
{
  LIST_ENTRY              *Link;
  VARIABLE_ENTRY          *Entry;
  CHAR16                  *Name;

  for ( Link = GetFirstNode (&mLockedVariableList)
      ; !IsNull (&mLockedVariableList, Link)
      ; Link = GetNextNode (&mLockedVariableList, Link)
      ) {
    Entry = BASE_CR (Link, VARIABLE_ENTRY, Link);
    Name = (CHAR16 *) ((UINTN) Entry + sizeof (*Entry));
    if (CompareGuid (&Entry->Guid, VendorGuid) && (StrCmp (Name, VariableName) == 0)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Mark a variable that will become read-only after leaving the DXE phase of execution.

  @param[in] This          The VARIABLE_LOCK_PROTOCOL instance.
  @param[in] VariableName  A pointer to the variable name that will be made read-only subsequently.
  @param[in] VendorGuid    A pointer to the vendor GUID that will be made read-only subsequently.

  @retval EFI_SUCCESS           The variable specified by the VariableName and the VendorGuid was marked
                                as pending to be read-only.
  @retval EFI_INVALID_PARAMETER VariableName or VendorGuid is NULL.
                                Or VariableName is an empty string.
  @retval EFI_ACCESS_DENIED     EFI_END_OF_DXE_EVENT_GROUP_GUID or EFI_EVENT_GROUP_READY_TO_BOOT has
                                already been signaled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource to hold the lock request.
**/
EFI_STATUS
EFIAPI
VariableLockRequestToLock (
  IN CONST EDKII_VARIABLE_LOCK_PROTOCOL *This,
  IN       CHAR16                       *VariableName,
  IN       EFI_GUID                     *VendorGuid
  )
{
  EFI_STATUS              Status;
  VARIABLE_POLICY_ENTRY   *NewPolicy;

  DEBUG ((DEBUG_WARN, "!!! DEPRECATED INTERFACE !!! %a() will go away soon!\n", __FUNCTION__));
  DEBUG ((DEBUG_WARN, "!!! DEPRECATED INTERFACE !!! Please move to use Variable Policy!\n"));
  DEBUG ((DEBUG_WARN, "!!! DEPRECATED INTERFACE !!! Variable: %g %s\n", VendorGuid, VariableName));

  NewPolicy = NULL;
  Status = CreateBasicVariablePolicy( VendorGuid,
                                      VariableName,
                                      VARIABLE_POLICY_NO_MIN_SIZE,
                                      VARIABLE_POLICY_NO_MAX_SIZE,
                                      VARIABLE_POLICY_NO_MUST_ATTR,
                                      VARIABLE_POLICY_NO_CANT_ATTR,
                                      VARIABLE_POLICY_TYPE_LOCK_NOW,
                                      &NewPolicy );
  if (!EFI_ERROR( Status )) {
    Status = ProtocolRegisterVariablePolicy ( NewPolicy );
    //
    // If the error returned is EFI_ALREADY_STARTED, we need to check the
    // current database for the variable and see whether it's locked. If it's
    // locked, we're still fine, but also generate a DEBUG_WARN message so the
    // duplicate lock can be removed.
    //
    if (Status == EFI_ALREADY_STARTED) {
      Status = ValidateSetVariable (VariableName, VendorGuid, 0, 0, NULL);
      if (Status == EFI_WRITE_PROTECTED) {
        DEBUG ((DEBUG_WARN, "  Variable: %g %s is already locked!\n", VendorGuid, VariableName));
        Status = EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_WARN, "  Variable: %g %s is already locked!\n", VendorGuid, VariableName));
        Status = EFI_ACCESS_DENIED;
      }
    }
  }
  if (EFI_ERROR( Status )) {
    ASSERT_EFI_ERROR( Status );
  }
  if (NewPolicy != NULL) {
    FreePool( NewPolicy );
  }

  return Status;

}


/**
  Convert all of variables in variable list to virtual address.
**/
VOID
ConvertVariableLockList (
  VOID
  )
{
  ConvertList (&mLockedVariableList);
}

/**
  Internal function to check this variable is whether locked.

  @param[in] VariableName  Name of Variable to be found.
  @param[in] VendorGuid    Variable vendor GUID.

  @retval TRUE             This variable is locked.
  @retval FALSE            This variable isn't locked.
**/
BOOLEAN
IsVariableLocked (
  IN CHAR16                  *VariableName,
  IN EFI_GUID                *VendorGuid
  )
{
  if (mSmst == NULL) {
    if (mVariableModuleGlobal->EndOfDxe && mEnableLocking && IsVariableInLockList (VariableName, VendorGuid)) {
      return TRUE;
    }
  } else {
    if (mSmmVariableGlobal->ProtectedModeVariableModuleGlobal->EndOfDxe && mEnableLocking && IsVariableInLockList (VariableName, VendorGuid)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Create variable lock list in SMM RAM.

  @param[in] ListHead      Head of linked list from protected mode.

  @retval EFI_SUCCESS           Create variable lock list in SMM RAM successfully.
  @retval EFI_UNSUPPORTED        System isn't in SMM mode.
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource to hold the variable entry.
**/
STATIC
EFI_STATUS
CreateVariableLockListInSmm (
  LIST_ENTRY         *ListHead
  )
{
  LIST_ENTRY           *Link;
  VARIABLE_ENTRY       *Entry;
  UINTN                EntrySize;
  VARIABLE_ENTRY       *WorkingEntry;
  CHAR16               *Name;

  if (mSmst == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // in the list of locked variables, convert the name pointers first
  //
  for ( Link = GetFirstNode (ListHead)
      ; !IsNull (ListHead, Link)
      ; Link = GetNextNode (ListHead, Link)
      ) {
    Entry     = BASE_CR (Link, VARIABLE_ENTRY, Link);
    Name      = (CHAR16 *) ((UINTN) Entry + sizeof (*Entry));
    if (StrnLenS (Name, MAX_VARIABLE_NAME_SIZE / sizeof (CHAR16)) == MAX_VARIABLE_NAME_SIZE / sizeof (CHAR16)) {
      return EFI_INVALID_PARAMETER;;
    }
    EntrySize = sizeof (VARIABLE_ENTRY) + StrSize (Name);
    WorkingEntry = VariableAllocateZeroBuffer (EntrySize, TRUE);
    ASSERT (WorkingEntry != NULL);
    if (WorkingEntry == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (WorkingEntry, Entry, EntrySize);
    InsertTailList (&mLockedVariableList, &WorkingEntry->Link);
  }

  return EFI_SUCCESS;
}


/**
  This function uses to send SMI to create variable lock list in SMM mode.
**/
VOID
CreateVariableLockListThroughSmi (
  VOID
  )
{
  LIST_ENTRY        *ListHead;

  if (mVariableModuleGlobal->SmmCodeReady && mSmst == NULL) {
    InitCommunicationBufferHeader ();
    mVariableModuleGlobal->SmmVarBuf->Signature = SMM_VARIABLE_LOCK_SIGNATURE;
    mVariableModuleGlobal->SmmVarBuf->AccessType = SMM_VARIABLE_LOCK_FUN_NUM;
    mVariableModuleGlobal->SmmVarBuf->DataSize = sizeof (LIST_ENTRY *);
    ListHead = &mLockedVariableList;
    CopyMem (mVariableModuleGlobal->SmmVarBuf + 1, &ListHead, sizeof (LIST_ENTRY *));
    SendCommunicateBuffer ();
  }
}

/**
  This function uses to create variable lock list.

  @return EFI_SUCCESS    Create variable lock list in SMM RAM successfully.
  @return Other          Any error occurred while creating variable lock list in SMM RAM.
**/
EFI_STATUS
SmmCreateVariableLockList (
  VOID
  )
{
  SMM_VAR_BUFFER *VariableBuffer;
  LIST_ENTRY     *ListHead;
  EFI_STATUS     Status;
  STATIC BOOLEAN Initialized = FALSE;

  if (Initialized) {
    return EFI_ALREADY_STARTED;
  }
  VariableBuffer = mVariableModuleGlobal->SmmVarBuf;
  if (VariableBuffer->Signature != SMM_VARIABLE_LOCK_SIGNATURE || VariableBuffer->DataSize != sizeof (LIST_ENTRY *)) {
    return EFI_UNSUPPORTED;
  }
  Initialized = TRUE;
  ListHead = *((LIST_ENTRY **) (VariableBuffer + 1));
  Status = CreateVariableLockListInSmm (ListHead);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return CreatePolicyTable ();
}
