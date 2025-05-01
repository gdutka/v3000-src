/** @file
  Functions to access discrete TPM2 module.

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

#include <Guid/TpmInstance.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/Tpm2DeviceLib.h>

#include <Protocol/Tpm2Interface.h>
#include <Protocol/SmmBase2.h>

TPM2_INTERFACE_PROTOCOL                   mTpm2Interface;

EFI_STATUS
EFIAPI
Tpm2DxeCommand (
  IN     TPM2_INTERFACE_PROTOCOL          *This,
  IN     UINT8                            *BufferIn,
  IN     UINT32                           SizeIn,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  //
  // NOTE: That may take many seconds to minutes for certain commands, such as key generation.
  //
  return Tpm2SubmitCommand (SizeIn, BufferIn, SizeOut, BufferOut);
}

EFI_STATUS
EFIAPI
Tpm2DxeRequestUseTpm (
  IN  TPM2_INTERFACE_PROTOCOL             *This
  )
{
  return Tpm2RequestUseTpm ();
}

EFI_STATUS
EFIAPI
Tpm2DxeSend (
  IN  TPM2_INTERFACE_PROTOCOL             *This,
  IN  UINT8                               *BufferIn,
  IN  UINT32                              SizeIn
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
Tpm2DxeReceive (
  IN     TPM2_INTERFACE_PROTOCOL          *This,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  return EFI_UNSUPPORTED;
}

/**
  The driver's entry point. It publishes TPM2 Interface Protocol.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
DriverEntry (
  IN    EFI_HANDLE                        ImageHandle,
  IN    EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_SMM_BASE2_PROTOCOL                  *SmmBase;
  BOOLEAN                                 InSmm;
  EFI_STATUS                              Status;
  EFI_SMM_SYSTEM_TABLE2                   *Smst;
  EFI_HANDLE                              Handle;
  VOID                                    *Interface;

  if (!CompareGuid (PcdGetPtr (PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm20DtpmGuid)) {
    DEBUG ((EFI_D_ERROR, "No TPM2 DTPM instance required!\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGetBool (PcdTpmHide)) {
    DEBUG((EFI_D_INFO, "TPM is hidden\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **)&SmmBase
                  );
  if (!EFI_ERROR (Status)) {
    SmmBase->InSmm (SmmBase, &InSmm);
  } else {
    InSmm = FALSE;
  }

  if (!InSmm) {
    //
    // In boot service
    //
    mTpm2Interface.SummitCommand  = Tpm2DxeCommand;
    mTpm2Interface.RequestUse     = Tpm2DxeRequestUseTpm;
    mTpm2Interface.Send           = Tpm2DxeSend;
    mTpm2Interface.Receive        = Tpm2DxeReceive;

    Status = gBS->LocateProtocol (
                    &gTpm2InterfaceGuid,
                    NULL,
                    (VOID **)&Interface
                    );
    if (!EFI_ERROR (Status)) {
      //
      // TPM2 Interface Protocol already exists.
      // Return EFI_SUCCESS here to handle the case that fTPM driver executed prior to this driver.
      //
      return EFI_SUCCESS;
    }

    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                     &Handle,
                     &gTpm2InterfaceGuid,
                     EFI_NATIVE_INTERFACE,
                     &mTpm2Interface
                     );
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // In Smm
    //
    mTpm2Interface.SummitCommand  = Tpm2DxeCommand;
    mTpm2Interface.RequestUse     = Tpm2DxeRequestUseTpm;
    mTpm2Interface.Send           = Tpm2DxeSend;
    mTpm2Interface.Receive        = Tpm2DxeReceive;

    SmmBase->GetSmstLocation (SmmBase, &Smst);

    Status = Smst->SmmLocateProtocol (
                     &gTpm2InterfaceGuid,
                     NULL,
                     (VOID **)&Interface
                     );
    if (!EFI_ERROR (Status)) {
      //
      // TPM2 Interface Protocol already exists.
      // Return EFI_SUCCESS here to handle the case that fTPM driver executed prior to this driver.
      //
      return EFI_SUCCESS;
    }

    Handle = NULL;
    Status = Smst->SmmInstallProtocolInterface (
                     &Handle,
                     &gTpm2InterfaceGuid,
                     EFI_NATIVE_INTERFACE,
                     &mTpm2Interface
                     );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
