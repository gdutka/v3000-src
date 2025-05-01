/** @file
  TIS (TPM Interface Specification) functions to access discrete TPM module.

;******************************************************************************
;* Copyright (c) 2013 - 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdFtpmProtocol.h>

#include <Protocol/Tpm2Interface.h>
#include <Protocol/SmmBase2.h>
#include <Library/AmdFtpmLib.h>
#include <Library/ChipsetConfigLib.h>
#include <ChipsetSetupConfig.h>
#include <Protocol/SetupUtility.h>
#include <Library/PcdLib.h>
#include <TpmPolicy.h>

#define SKIP_CONTROL_AREA_INIT                   ( 1 << 13 ) //BIT13  //Should define in TpmPolicy.h

TPM2_INTERFACE_PROTOCOL                 mAmdfTpmInterface;
UINTN                                   mCommandAddress, mResponseAddress;
BOOLEAN                                 gInSmm = FALSE;


/**
 *  GET TPM related Info
 *
 * @param[in,out] FtpmStatus              Used to hold more detail info (Unused Currently)
 *
 * @return       EFI_SUCCESS              Ftpm function supported
 * @return       EFI_UNSUPPORTED          Ftpm function unsupported
 *
 */
EFI_STATUS
AmdfTpmCheckSts (
  IN OUT   UINTN                *fTPMStatus
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  AMD_FTPM_PROTOCOL     *fTpmProtocol = NULL;

  do
  {
    if (TRUE == gInSmm)
    {
      Status = EFI_UNSUPPORTED;
      break;
    }

    Status = gBS->LocateProtocol (
                    &gAmdFtpmProtocolGuid,
                    NULL,
                    (VOID **)&fTpmProtocol
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmProtocolGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = fTpmProtocol->CheckStatus (fTpmProtocol, fTPMStatus);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  fTpmProtocol->CheckStatus (...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
EFIAPI
AmdfTpmDxeCommand (
  IN     TPM2_INTERFACE_PROTOCOL          *This,
  IN     UINT8                            *BufferIn,
  IN     UINT32                           SizeIn,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  AMD_FTPM_PROTOCOL     *fTpmProtocol = NULL;

  do
  {
    if (TRUE == gInSmm)
    {
      Status = EFI_UNSUPPORTED;
      break;
    }

    Status = gBS->LocateProtocol (
                    &gAmdFtpmProtocolGuid,
                    NULL,
                    (VOID **)&fTpmProtocol
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmProtocolGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = fTpmProtocol->SendCommand (fTpmProtocol, BufferIn, SizeIn);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  fTpmProtocol->SendCommand (...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = fTpmProtocol->GetResponse (fTpmProtocol, BufferOut, SizeOut);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  fTpmProtocol->GetResponse (...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
EFIAPI
AmdfTpmDxeRequestUseTpm (
  IN  TPM2_INTERFACE_PROTOCOL             *This
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  UINTN             fTpmStatus;

  do
  {
    Status = AmdfTpmCheckSts (&fTpmStatus);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  AmdfTpmCheckSts (...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
EFIAPI
AmdfTpmDxeSend (
  IN  TPM2_INTERFACE_PROTOCOL             *This,
  IN  UINT8                               *BufferIn,
  IN  UINT32                              SizeIn
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  AMD_FTPM_PROTOCOL     *fTpmProtocol = NULL;

  do
  {
    if (TRUE == gInSmm)
    {
      Status = EFI_UNSUPPORTED;
      break;
    }

    Status = gBS->LocateProtocol (
                    &gAmdFtpmProtocolGuid,
                    NULL,
                    (VOID **)&fTpmProtocol
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmProtocolGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = fTpmProtocol->SendCommand (fTpmProtocol, BufferIn, SizeIn);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  fTpmProtocol->SendCommand (...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
EFIAPI
AmdfTpmDxeReceive (
  IN     TPM2_INTERFACE_PROTOCOL          *This,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  AMD_FTPM_PROTOCOL     *fTpmProtocol = NULL;

  do
  {
    if (TRUE == gInSmm)
    {
      Status = EFI_UNSUPPORTED;
      break;
    }

    Status = gBS->LocateProtocol (
                    &gAmdFtpmProtocolGuid,
                    NULL,
                    (VOID **)&fTpmProtocol
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmProtocolGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = fTpmProtocol->GetResponse (fTpmProtocol, BufferOut, SizeOut);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  fTpmProtocol->GetResponse (...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}


/**
  The driver's entry point. It publishes EFI TrEE Protocol.

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
  EFI_STATUS                              Status;
  EFI_HANDLE                              Handle;
  UINTN                                   iTPMStatus;
  UINT32                                  SetTpmPolicy;

  switch (PcdGet8 (PcdH2OTpmType)) {
    case 0:
    case 1:
      DEBUG((EFI_D_INFO, "  Not Select Amd fTPM\n"));
      return EFI_SUCCESS;
      break;
    default:
      break;
  }
  
  if ( EFI_ERROR (AmdfTpmCheckSts (&iTPMStatus)) )
  {
    DEBUG ((EFI_D_ERROR, "Amd fTPM not detected!\n"));
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **)&SmmBase
                  );
  if (!EFI_ERROR (Status)) {
    SmmBase->InSmm (SmmBase, &gInSmm);
  } else {
    gInSmm = FALSE;
  }

  if (!gInSmm)
  {
    //
    // In boot service
    //
    mAmdfTpmInterface.SummitCommand  = AmdfTpmDxeCommand;
    mAmdfTpmInterface.RequestUse     = AmdfTpmDxeRequestUseTpm;
    mAmdfTpmInterface.Send           = AmdfTpmDxeSend;
    mAmdfTpmInterface.Receive        = AmdfTpmDxeReceive;
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                     &Handle,
                     &gTpm2InterfaceGuid,
                     EFI_NATIVE_INTERFACE,
                     &mAmdfTpmInterface
                     );
    ASSERT_EFI_ERROR (Status);

    SetTpmPolicy = PcdGet32(PcdDxeTpmPolicy) | SKIP_INSTALL_TCG_ACPI_TABLE | SKIP_CONTROL_AREA_INIT;
    Status = PcdSet32S(PcdDxeTpmPolicy, SetTpmPolicy);
    ASSERT_EFI_ERROR (Status);
  }
  
  return EFI_SUCCESS;
}
