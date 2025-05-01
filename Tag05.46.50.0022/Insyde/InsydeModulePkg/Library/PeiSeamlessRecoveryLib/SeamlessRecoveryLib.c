/** @file
  Lite version of SeamlessRecoveryLib for PEIMs before memory initialization

;******************************************************************************
;* Copyright (c) 2013 - 2021, Insyde Software Corp. All Rights Reserved.
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
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/VariableLib.h>
#include <Library/SeamlessRecoveryLib.h>

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

  RecoverySig = (EFI_GUID *)(UINTN) FdmGetNAtAddr (&gH2OFlashMapRegionFtwBackupGuid ,1);
  if (RecoverySig != NULL && CompareGuid( PcdGetPtr(PcdSeamlessRecoverySignature), RecoverySig) ) {
    return TRUE;
  }

  return FALSE;
}

/**
  Set firmware updating in process signature, not supported if PEI memory is not initialized

  @param  None

  @returns None

**/
VOID
EFIAPI
SetFirmwareUpdatingFlag (
  BOOLEAN FirmwareUpdatingFlag
  )
{
  //
  // not supported
  //
  return;
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
  //
  // not supported
  //
  return EFI_UNSUPPORTED;
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
  EFI_HOB_GUID_TYPE                 *GuidHob;

  GuidHob = GetFirstGuidHob (&gH2OSeamlessRecoveryGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  *Progress = *(UINT32 *)GET_GUID_HOB_DATA (GuidHob);
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
  UINT32                            Data32;

  Status = GetFirmwareUpdateProgress (&Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Progress hob -%r\n", Status));
    return FALSE;
  }

  return (Data32 > FlashStart) && (Data32 < FlashResiliency);
}

/**
  Clear firmware updating progress.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
ClearFirmwareUpdateProgress (
  VOID
  )
{
  //
  // not supported
  //
  return EFI_UNSUPPORTED;
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
  Create hobs for H2O progress.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
STATIC
EFI_STATUS
CreateH2OSeamlessRecoveryHob (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINT32                            Data32;
  UINTN                             DataSize;
  EFI_HOB_GUID_TYPE                 *GuidHob;

  GuidHob = GetFirstGuidHob (&gH2OSeamlessRecoveryGuid);
  if (GuidHob == NULL) {
    DataSize = sizeof (UINT32);
    Status = CommonGetVariable (
               UPDATE_PROGRESS_NAME,
               &gH2OSeamlessRecoveryGuid,
               &DataSize,
               &Data32
               );
    if (!EFI_ERROR (Status)) {
      BuildGuidDataHob (&gH2OSeamlessRecoveryGuid, &Data32, sizeof (UINT32));
    }
  }

  return EFI_SUCCESS;
}

/**
  Create hobs for digest variable.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
STATIC
EFI_STATUS
CreateH2OSeamlessRecoveryDigestHob (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             DataSize;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  UINT8                             Sha256Digest[SHA256_DIGEST_SIZE];

  GuidHob = GetFirstGuidHob (&gH2OSeamlessRecoveryDigestGuid);
  if (GuidHob == NULL) {
    ZeroMem (Sha256Digest, SHA256_DIGEST_SIZE);
    DataSize = SHA256_DIGEST_SIZE;
    Status = CommonGetVariable (
               BACKUP_SBB_FILE_DIGEST_SHA256_NAME,
               &gH2OSeamlessRecoveryDigestGuid,
               &DataSize,
               Sha256Digest
               );
    if (!EFI_ERROR (Status)) {
      BuildGuidDataHob (&gH2OSeamlessRecoveryDigestGuid, Sha256Digest, SHA256_DIGEST_SIZE);
    }
  }

  return EFI_SUCCESS;
}

/**
  The constructor function of PeiSeamlessRecoveryLib.

  @param  FileHandle   The handle of FFS header the loaded driver.
  @param  PeiServices  The pointer to the PEI services.

  @retval EFI_SUCCESS           The constructor executes successfully.
  @retval EFI_OUT_OF_RESOURCES  There is no enough resource for the constructor.

**/
EFI_STATUS
EFIAPI
SeamlessRecoveryLibConstructor (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  CreateH2OSeamlessRecoveryHob ();
  CreateH2OSeamlessRecoveryDigestHob ();

  return EFI_SUCCESS;
}

