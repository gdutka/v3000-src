/** @file
  Header file for SeamlessRecoveryLib Library Class

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

#ifndef _SEAMLESS_RECOVERY_LIB_H_
#define _SEAMLESS_RECOVERY_LIB_H_

#include <Guid/H2OSeamlessRecovery.h>

typedef enum {
  GetString
  //Reserved for furture
} SEAMLESS_TYPE;

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
  );

/**
  Set firmware updating in process signature

  @param  None

  @returns None

**/
VOID
EFIAPI
SetFirmwareUpdatingFlag (
  BOOLEAN FirmwareUpdatingFlag
  );

/**
  Check whether there was a firmware update interrupted.

  @retval TRUE      Fimware update was interrupted.
          FALSE     No fimware update was interrupted.

**/
BOOLEAN
EFIAPI
IsFirmwareUpdateInterrupted (
  VOID
  );

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
  );

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
  );

/**
  Clear firmware updating progress.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
ClearFirmwareUpdateProgress (
  VOID
  );

/**
  Check whether firmware update resiliency is supported.

  @retval TRUE      Firmware update resiliecny is supported.
          FALSE     Firmware update resiliecny is not supported.

**/
BOOLEAN
EFIAPI
IsFirmwareUpdateResiliencySupported (
  VOID
  );

/**
  Check whether firmware update resiliency is enabled.

  @retval TRUE      Firmware update resiliency is enabled.
          FALSE     Firmware update resiliency is not enabled.

**/
BOOLEAN
EFIAPI
IsFirmwareUpdateResiliencyEnabled (
  VOID
  );

#endif
