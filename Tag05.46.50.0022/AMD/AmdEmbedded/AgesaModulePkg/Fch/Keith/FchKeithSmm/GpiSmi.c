/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchSmm.h"
#define FILECODE FCH_KEITH_FCHKEITHSMM_GPISMI_FILECODE

UINT32  mGpiSmiCounter;

EFI_STATUS
EFIAPI
AmdGpiSmiCallback (
  IN       EFI_HANDLE                         DispatchHandle,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT *GpiRegisterContext,
  IN OUT   EFI_SMM_GPI_REGISTER_CONTEXT       *CurrentContext,
  IN OUT   UINTN                              *SizeOfContext
  )
{
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, 0x80, &mGpiSmiCounter);
  mGpiSmiCounter++;
  return EFI_SUCCESS;
}


EFI_STATUS
FchSmmRegisterGpiSmi (
  VOID
  )
{
  EFI_STATUS                           Status;
  FCH_SMM_GPI_DISPATCH2_PROTOCOL       *AmdGpiDispatch;
  EFI_SMM_GPI_REGISTER_CONTEXT         GpiRegisterContext;
  EFI_HANDLE                           GpiHandle;

  //
  // Smi GPI test
  //
  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmGpiDispatch2ProtocolGuid,
                  NULL,
                  &AmdGpiDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  mGpiSmiCounter = 0;
  GpiRegisterContext.GpiNum = 0;
  Status = AmdGpiDispatch->Register (
                             AmdGpiDispatch,
                             AmdGpiSmiCallback,
                             &GpiRegisterContext,
                             &GpiHandle
                             );
  return Status;
}



