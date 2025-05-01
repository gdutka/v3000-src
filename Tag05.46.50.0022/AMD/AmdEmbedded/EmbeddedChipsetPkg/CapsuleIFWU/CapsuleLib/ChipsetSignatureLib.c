/** @file
   Seamless Recovery support library for Windows UEFI Firmware Update Platform

;******************************************************************************
;* Copyright (c) 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/VariableLib.h>
#include <Guid/ChipsetCapsuleSignature.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#define  EFI_CHIPSET_CAPSULE_SIGNATURE  L"DevFwCapsuleUpdate"

EFI_RESET_SYSTEM                        mOriginalResetSystemPtr;

VOID
EFIAPI
DummyResetSystem (
  IN EFI_RESET_TYPE             ResetType,
  IN EFI_STATUS                 ResetStatus,
  IN UINTN                      DataSize,
  IN VOID                       *ResetData OPTIONAL
  )
{
  return;
}


/**
  Hook gRT->ResetSystem to avoid system reset cause firmware update fail

  @param  TRUE      Hook gRT->ResetSystem
          FALSE     Restore gRT->ResetSystem

  @retval None

**/
VOID
HookSystemResetRoutine (
  IN BOOLEAN                    ToHook
  )
{

  if (ToHook) {
    mOriginalResetSystemPtr = gRT->ResetSystem;
    gRT->ResetSystem = DummyResetSystem;
  } else {
    gRT->ResetSystem = mOriginalResetSystemPtr;
  }
}

/**
  Check whether it is in device firmware update process

  @param  None

  @retval TRUE      The system is in firmware failure recovery mode
          FALSE     The system is not in firmware failure recovery mode

**/
BOOLEAN
IsDeviceFirmwareUpdateProcess (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  UINT8                                 Flag;

  Size = sizeof (Flag);
  Status = CommonGetVariable (
             EFI_CHIPSET_CAPSULE_SIGNATURE,
             &gChipsetCapsuleSignatureGuid,
             &Size,
             &Flag
             );
  if (!EFI_ERROR (Status)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Set device firmware updating in process signature

  @param  None

  @returns None

**/
VOID
SetDeviceFirmwareUpdatingFlag (
  BOOLEAN   DeviceFirmwareUpdating
  )
{
  EFI_STATUS                            Status;
  UINT8                                 Flag;

  if ( (!DeviceFirmwareUpdating) && (!IsDeviceFirmwareUpdateProcess()) ) {
    return;
  }

  Status = CommonSetVariable (
             EFI_CHIPSET_CAPSULE_SIGNATURE,
             &gChipsetCapsuleSignatureGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             0,
             NULL
             );

  if (DeviceFirmwareUpdating) {
    Flag = 1;
    Status = CommonSetVariable (
               EFI_CHIPSET_CAPSULE_SIGNATURE,
               &gChipsetCapsuleSignatureGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               sizeof(Flag),
               &Flag
               );
    if (EFI_ERROR (Status)) {
      return;
    }
  }

  HookSystemResetRoutine (DeviceFirmwareUpdating);
}
