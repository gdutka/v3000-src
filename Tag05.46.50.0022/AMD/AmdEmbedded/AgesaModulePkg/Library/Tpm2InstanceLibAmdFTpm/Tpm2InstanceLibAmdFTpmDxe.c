/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <IndustryStandard/Tpm20.h>
#include <Library/PcdLib.h>
#include <Protocol/AmdFtpmProtocol.h>
#include <Library/AmdFtpmLib.h>

// gEfiTpmDeviceInstanceTpm20AmdFtpmGuid
#define TPM_DEVICE_INTERFACE_TPM20_AMD_FTPM  \
  {0x765be6b2, 0x02db, 0x40ea, {0x94, 0xa8, 0xfa, 0xc0, 0x9f, 0x83, 0x3f, 0x08}}

 AMD_FTPM_PROTOCOL     *mfTpmProtocol = NULL;

/**
  This service enables the sending of commands to the FTPM.

  @param[in]      InputParameterBlockSize  Size of the FTPM input parameter block.
  @param[in]      InputParameterBlock      Pointer to the FTPM input parameter block.
  @param[in,out]  OutputParameterBlockSize Size of the FTPM output parameter block.
  @param[in]      OutputParameterBlock     Pointer to the FTPM output parameter block.

  @retval EFI_SUCCESS            The command byte stream was successfully sent to the device and a response was successfully received.
  @retval EFI_DEVICE_ERROR       The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_BUFFER_TOO_SMALL   The output parameter block is too small.
**/
EFI_STATUS
EFIAPI
FTpmSubmitCommand (
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN OUT UINT32        *OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  )
{
  return mfTpmProtocol->Execute(mfTpmProtocol, InputParameterBlock, InputParameterBlockSize, OutputParameterBlock, OutputParameterBlockSize);
}

/**
  This service requests use FTPM.

  @retval EFI_SUCCESS      Get the control of FTPM chip.
  @retval EFI_NOT_FOUND    FTPM not found.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
FTpmRequestUseTpm (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       fTpmStatus;

  if (PcdGet8 (PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_PSP_FTPM ||
     (PcdGet8 (PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_HSP_FTPM)) {
    Status = mfTpmProtocol->CheckStatus (mfTpmProtocol, &fTpmStatus);
    DEBUG ((DEBUG_INFO, "fTPM Status = %r\n", Status));
    return Status;
  }
  return EFI_NOT_FOUND;
}

TPM2_DEVICE_INTERFACE  mFTpmInternalTpm2Device = {
  TPM_DEVICE_INTERFACE_TPM20_AMD_FTPM,
  FTpmSubmitCommand,
  FTpmRequestUseTpm,
};

/**
  The function register FTPM instance.

  @retval EFI_SUCCESS   FTPM instance is registered, or system dose not surpport registr FTPM instance
**/
EFI_STATUS
EFIAPI
Tpm2InstanceLibAmdFTpmConstructor (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
                  &gAmdFtpmProtocolGuid,
                  NULL,
                  (VOID **)&mfTpmProtocol
                  );
  if (Status == EFI_SUCCESS) {
    Status = Tpm2RegisterTpm2DeviceLib (&mFTpmInternalTpm2Device);
    if(Status == EFI_UNSUPPORTED) {
      //
      // Unsupported means platform policy does not need this instance enabled.
      //
      return EFI_SUCCESS;
    }
  }
  return EFI_SUCCESS;
}
