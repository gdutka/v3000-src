/** @file
  Functions to access discrete TPM module.

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

#include <PiPei.h>

#include <Guid/TpmInstance.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PerformanceLib.h>
#include <Library/BaseLib.h>
#include <Library/Tpm2DeviceLib.h>

#include <Ppi/Tpm2InterfacePei.h>

EFI_STATUS
Tpm2PeiCommand (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This,
  IN     UINT8                            *BufferIn,
  IN     UINT32                           SizeIn,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  return Tpm2SubmitCommand (SizeIn, BufferIn, SizeOut, BufferOut);
}

EFI_STATUS
Tpm2PeiRequestUseTpm (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This
  )
{
  return Tpm2RequestUseTpm ();
}

EFI_STATUS
Tpm2PeiSend (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This,
  IN     UINT8                            *BufferIn,
  IN     UINT32                           SizeIn
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
Tpm2PeiReceive (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  return EFI_UNSUPPORTED;
}

PEI_TPM2_INTERFACE_PPI         mPeiTpm2InterfacePpi = {
  Tpm2PeiCommand,
  Tpm2PeiRequestUseTpm,
  Tpm2PeiSend,
  Tpm2PeiReceive
};

EFI_PEI_PPI_DESCRIPTOR   mInstallPeiTpm2InterfacePpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTpm2InterfacePpiGuid,
  &mPeiTpm2InterfacePpi
};

/**
  Entry point of this module.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @return Status.

**/
EFI_STATUS
EFIAPI
PeimEntry (
  IN       EFI_PEI_FILE_HANDLE            FileHandle,
  IN CONST EFI_PEI_SERVICES               **PeiServices
  )
{
  EFI_STATUS                              Status;
  EFI_PEI_PPI_DESCRIPTOR                  *PpiDescriptor;
  VOID                                    *Ppi;

  if (!CompareGuid (PcdGetPtr (PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm20DtpmGuid)) {
    DEBUG ((EFI_D_ERROR, "No TPM2 DTPM instance required!\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGetBool (PcdTpmHide)) {
    DEBUG((EFI_D_INFO, "TPM is hidden\n"));
    return EFI_UNSUPPORTED;
  }

  Ppi           = NULL;
  PpiDescriptor = NULL;
  Status = PeiServicesLocatePpi (
             &gPeiTpm2InterfacePpiGuid,
             0,
             &PpiDescriptor,
             (VOID **) &Ppi
             );
  if (!EFI_ERROR (Status)) {
    //
    // TPM2 Interface PPI already exists.
    // Return EFI_SUCCESS here to handle the case that fTPM driver executed prior to this driver.
    //
    return EFI_SUCCESS;
  }

  Status = PeiServicesInstallPpi (&mInstallPeiTpm2InterfacePpi);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
