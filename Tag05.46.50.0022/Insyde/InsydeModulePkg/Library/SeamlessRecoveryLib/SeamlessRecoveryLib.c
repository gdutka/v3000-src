/** @file
   Seamless Recovery support library for Windows UEFI Firmware Update Platform

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
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
#include <Library/FdSupportLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/VariableLib.h>
#include <Library/SeamlessRecoveryLib.h>
#include <Library/BaseLib.h>
#include <Protocol/SmmFwBlockService.h>
#include <Protocol/SwapAddressRange.h>

STATIC  UINT64       mFtwBackupAddress;

/**
  Check whether it is in firmware failure recovery mode

  @param  None

  @retval TRUE      The system is in firmware failure recovery mode
          FALSE     The system is not in firmware failure recovery mode

**/
BOOLEAN
EFIAPI
IsFirmwareFailureRecovery (
  VOID
  )
{
  EFI_GUID *RecoverySig;

  RecoverySig = (EFI_GUID *)(UINTN)mFtwBackupAddress;
  if (RecoverySig != NULL && CompareGuid (PcdGetPtr (PcdSeamlessRecoverySignature), RecoverySig)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Set firmware updating in process signature and write capsule file
  name

  @param[in] FirmwareUpdatingFlag     A boolean indicator.
                                      TRUE   Write seamless signature into NV_FTW_SPARE region after erase
                                             also write capsule file name if success to get CapsuleFileName
                                             variable.
                                      FALSE  Only erase NV_FTW_SPARE region.

  @returns None

**/
VOID
EFIAPI
SetFirmwareUpdatingFlag (
  BOOLEAN FirmwareUpdatingFlag
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  EFI_SMM_FW_BLOCK_SERVICE_PROTOCOL     *SmmFwbService;
  UINTN                                 BlocksSize;
  CHAR16                                *CapsuleFileName;
  UINTN                                 CapsuleFileNameSize;
  UINT32                                *Data;

  if ((!FirmwareUpdatingFlag) && (!IsFirmwareFailureRecovery())) {
    return;
  }
  Data                = NULL;
  CapsuleFileNameSize = 0;
  Status = CommonGetVariableDataAndSize (
             L"CapsuleFileName",
             &gH2OSeamlessRecoveryGuid,
             &CapsuleFileNameSize,
             (VOID **) &CapsuleFileName
             );
  if (!EFI_ERROR (Status)) {
    Data = AllocatePool (sizeof(UINT8) + CapsuleFileNameSize);
    if (Data == NULL) {
      return;
    }
    *Data = GetString;
    CopyMem ((VOID*)(((UINT8*)Data) + sizeof(UINT8)), CapsuleFileName, CapsuleFileNameSize);
    if (CapsuleFileName != NULL) {
      FreePool (CapsuleFileName);
    }
  }

  BlocksSize = GetFlashBlockSize ();

  Status = gBS->LocateProtocol (
                  &gEfiSmmFwBlockServiceProtocolGuid,
                  NULL,
                  (VOID **)&SmmFwbService
                  );
  if (EFI_ERROR (Status)) {
    Status = FlashErase ((UINTN)mFtwBackupAddress, BlocksSize);
    if (EFI_ERROR(Status)) {
      ASSERT_EFI_ERROR(Status);
      return;
    }
    if (FirmwareUpdatingFlag) {
      Size = sizeof(EFI_GUID);
      FlashProgram (
        (UINT8 *)(UINTN)mFtwBackupAddress,
        PcdGetPtr(PcdSeamlessRecoverySignature),
        &Size,
        (UINTN)mFtwBackupAddress
        );

      if (Data != NULL) {
        CapsuleFileNameSize += sizeof(UINT8);
        FlashProgram (
          (UINT8 *)(UINTN)((UINT8)mFtwBackupAddress + sizeof(EFI_GUID)),
          (UINT8 *)Data,
          &CapsuleFileNameSize,
          (UINTN)(mFtwBackupAddress + sizeof(EFI_GUID))
          );
      }
    }
  } else {

    Status = SmmFwbService->EraseBlocks(
                              SmmFwbService,
                              (UINTN)mFtwBackupAddress,
                              &BlocksSize
                              );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return;
    }
    if (FirmwareUpdatingFlag) {
      Size = sizeof (EFI_GUID);
      Status = SmmFwbService->Write (
                                SmmFwbService,
                                (UINTN)mFtwBackupAddress,
                                (UINTN *)&Size,
                                (UINT8 *)PcdGetPtr (PcdSeamlessRecoverySignature)
                                );
      if (Data != NULL) {
        CapsuleFileNameSize += sizeof(UINT8);
        Status = SmmFwbService->Write (
                                  SmmFwbService,
                                  (UINTN)(mFtwBackupAddress + sizeof(EFI_GUID)),
                                  (UINTN *)&CapsuleFileNameSize,
                                  (UINT8 *)Data
                                  );
      }
    }
  }
  if (Data != NULL) {
    FreePool (Data);
  }
}

/**
  Set firmware updating progress.

  @param[in]  Progress         Update progress

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
SetFirmwareUpdateProgress (
  IN  UINT32    Progress
  )
{
  return SetVariableToSensitiveVariable (
           UPDATE_PROGRESS_NAME,
           &gH2OSeamlessRecoveryGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           sizeof (UINT32),
           &Progress
           );
}

/**
  Get firmware updating progress.

  @param[out]  Progress         Update progress

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
GetFirmwareUpdateProgress (
  OUT  UINT32    *Progress
  )
{
  EFI_STATUS                        Status;
  UINTN                             DataSize;
  UINT32                            Data32;

  DataSize = sizeof (UINT32);
  Status = CommonGetVariable (
             UPDATE_PROGRESS_NAME,
             &gH2OSeamlessRecoveryGuid,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Progress variable -%r\n", Status));
    return Status;
  }

  *Progress = Data32;
  return EFI_SUCCESS;
}

/**
  Check whether there was a firmware update interrupted.

  @retval TRUE      Fimware update was interrupted.
          FALSE     No fimware update was interrupted.

**/
BOOLEAN
EFIAPI
IsFirmwareUpdateInterrupted (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             DataSize;
  UINT32                            Data32;

  DataSize = sizeof (UINT32);
  Status = CommonGetVariable (
             UPDATE_PROGRESS_NAME,
             &gH2OSeamlessRecoveryGuid,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Progress variable -%r\n", Status));
    return FALSE;
  }

  return ((Data32 > FlashStart) && (Data32 < FlashResiliency));
}

/**
  Clear firmware updating progress.

  @retval EFI_SUCCESS         Operation completed successfully.

**/
EFI_STATUS
EFIAPI
ClearFirmwareUpdateProgress (
  VOID
  )
{
  SetVariableToSensitiveVariable (
    UPDATE_PROGRESS_NAME,
    &gH2OSeamlessRecoveryGuid,
    0,
    0,
    NULL
    );
  return EFI_SUCCESS;
}

/**
  Check whether firmware update resiliency is supported.

  @retval TRUE      Firmware update resiliecny is supported.
          FALSE     Firmware update resiliecny is not supported.

**/
BOOLEAN
EFIAPI
IsFirmwareUpdateResiliencySupported (
  VOID
  )
{
  return FeaturePcdGet (PcdH2OBiosUpdateFaultToleranceResiliencyEnabled);
}

/**
  Check whether firmware update resiliency is enabled.

  @retval TRUE      Firmware update resiliency is enabled.
          FALSE     Firmware update resiliency is not enabled.

**/
BOOLEAN
EFIAPI
IsFirmwareUpdateResiliencyEnabled (
  VOID
  )
{
  EFI_STATUS            Status;
  UINT32                Data32;

  if (!IsFirmwareUpdateResiliencySupported ()) {
    return FALSE;
  }

  Status = GetFirmwareUpdateProgress (&Data32);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return (Data32 == FlashResiliency);
}

/**
  The constructor function stores the FDM information to prevent from the
  FDM location is changed after BIOS is updated.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SeamlessRecoveryLibConstructor (
  IN      EFI_HANDLE                ImageHandle,
  IN      EFI_SYSTEM_TABLE          *SystemTable
  )
{

  mFtwBackupAddress = FdmGetNAtAddr (&gH2OFlashMapRegionFtwBackupGuid ,1);
  return EFI_SUCCESS;
}
