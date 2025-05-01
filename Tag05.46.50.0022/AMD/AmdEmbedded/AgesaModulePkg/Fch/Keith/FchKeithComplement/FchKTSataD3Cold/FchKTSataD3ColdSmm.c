/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchKTSataD3ColdSmm.h"
#define FILECODE  FCH_KEITH_FCHKEITHCOMPLEMENT_FCHKTSATAD3COLD_FCHKTSATAD3COLDSMM_FILECODE

EFI_STATUS
EFIAPI
AmdSmiSataD3ColdEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  UINT16              Debug;
  UINT32              SmuArg[6];
  UINT32              SmuRequestId;
  UINT32              SmuResponse;
  PCI_ADDR            NbioPciAddress;
  UINT8               Controller = 0;

  DEBUG ((DEBUG_INFO, "\nSata.D3ColdEntry, DataPort %x\n", SwContext->DataPort));

  Controller = SwContext->DataPort;

  //check if we're NOT in D3Cold
  if (!FchSataD3ColdClockOn (Controller)) {
    DEBUG ((DEBUG_INFO, "Sata.D3ColdEntry, already in D3Cold\n"));
    return EFI_SUCCESS;
  }

  Debug = SATA_POWERDOWN_MESSAGE_ENTRY | Controller;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, 0x80, &Debug);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  SmuArg[0] = Controller;
  SmuRequestId = BIOSSMC_MSG_PowerDownSata;

  SmuResponse = NbioSmuServiceRequestV13 (NbioPciAddress, SmuRequestId, SmuArg, 0);

  Debug = SATA_POWERDOWN_MESSAGE_EXIT | Controller;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, 0x80, &Debug);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdSmiSataD3ColdExitCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  UINT16              Debug;
  UINT32              SmuArg[6];
  UINT32              SmuRequestId;
  UINT32              SmuResponse;
  PCI_ADDR            NbioPciAddress;
  UINT8               Controller = 0;

  DEBUG ((DEBUG_INFO, "\nSata.D3ColdExit, DataPort %x\n", SwContext->DataPort));

  Controller = SwContext->DataPort;

  //check if we're in D3Cold
  if (FchSataD3ColdClockOn (Controller)) {
    DEBUG ((DEBUG_INFO, "Sata.D3ColdExit, already out of D3Cold\n"));
    return EFI_SUCCESS;
  }

  Debug = SATA_POWERUP_MESSAGE_ENTRY | Controller;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, 0x80, &Debug);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  SmuArg[0] = Controller;
  SmuRequestId = BIOSSMC_MSG_PowerUpSata;

  SmuResponse = NbioSmuServiceRequestV13 (NbioPciAddress, SmuRequestId, SmuArg, 0);

  Debug = SATA_POWERUP_MESSAGE_EXIT | Controller;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, 0x80, &Debug);

  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchSataD3ColdSmmEntryPoint
 *
 * Description
 *   Entry point of the SataD3Cold SMM driver
 *   copy FCH_INIT_PROTOCOL to SMM, and register Fch Smm callbacks
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
FchSataD3ColdSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  EFI_STATUS                               Status;
  FCH_SMM_SW_DISPATCH2_PROTOCOL            *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT              SwRegisterContext;
  EFI_HANDLE                               SwHandle;
  UINT32                                   RTD3ColdEnableMap;

  Status = EFI_SUCCESS;
  RTD3ColdEnableMap = PcdGet32 (FchRTD3ColdEnableMap);

  if ((RTD3ColdEnableMap & BIT28) == 0) {
    return Status;
  }

  //
  //  Locate SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
             &gFchSmmSwDispatch2ProtocolGuid,
             NULL,
             &AmdSwDispatch
             );
  ASSERT_EFI_ERROR (Status);

  SwRegisterContext.AmdSwValue  = PcdGet8 (PcdFchSataD3EntrySwSmi);
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiSataD3ColdEntryCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SwRegisterContext.AmdSwValue  = PcdGet8 (PcdFchSataD3ExitSwSmi);
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiSataD3ColdExitCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}



