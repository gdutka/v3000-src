/** @file
  This driver provides a protocol to backup ESP flag when executing secure flash.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "SecureFlashEspFlagSmm.h"

/**
  Ihisi driver entry point to initialize secure flash ESP flag protocol.

  @param[in] ImageHandle        The firmware allocated handle for the UEFI image.
  @param[in] SystemTable        A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
*/
EFI_STATUS
EFIAPI
SecureFlashEspFlagEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{

  EFI_STATUS                           Status;
  SECURE_FLASH_ESP_FLAG_PROTOCOL       *SecureFlashEspFlagProtocol;
  EFI_HANDLE                           Handle;

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (SECURE_FLASH_ESP_FLAG_PROTOCOL),
                    (VOID**)&SecureFlashEspFlagProtocol
                    );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
  }
  //
  // install SMM protocol
  //
  SecureFlashEspFlagProtocol->ViaEspFlag     = FALSE;

  Handle =  NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gEfiSecureFlashEspFlagProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    SecureFlashEspFlagProtocol
                    );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}