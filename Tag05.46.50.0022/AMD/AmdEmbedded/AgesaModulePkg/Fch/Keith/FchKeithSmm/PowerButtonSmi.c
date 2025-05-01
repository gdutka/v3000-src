/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchSmm.h"
#include "GnbDxio.h"
#include <Library/NbioSmuV13Lib.h>

#define FILECODE FCH_KEITH_FCHKEITHSMM_POWERBUTTONSMI_FILECODE

extern FCH_DATA_BLOCK                      gFchDataInSmm;

EFI_STATUS
FchSmmRegisterPowerButtonSmi (
  VOID
  )
{
  EFI_STATUS                              Status;
  FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL       *AmdPwrBtnDispatch;
  FCH_SMM_PWRBTN_REGISTER_CONTEXT         AmdPowerButtonContext;
  EFI_HANDLE                              PowerButtonHandle;

  //
  // Register power button SMM event
  //
  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmPwrBtnDispatch2ProtocolGuid,
                  NULL,
                  &AmdPwrBtnDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  AmdPowerButtonContext.Phase = EfiPowerButtonExit;
  AmdPowerButtonContext.Order = 0xFF;
  Status = AmdPwrBtnDispatch->Register (
                                AmdPwrBtnDispatch,
                                AmdSmiPowerButtonCallback,
                                &AmdPowerButtonContext,
                                &PowerButtonHandle
                                );

  AmdPowerButtonContext.Phase = EfiPowerButtonEntry;
  AmdPowerButtonContext.Order = 0x89;
  Status = AmdPwrBtnDispatch->Register (
                                AmdPwrBtnDispatch,
                                AmdSmiPowerButtonCallbackTest,
                                &AmdPowerButtonContext,
                                &PowerButtonHandle
                                );
  return Status;
}


//
// Enable sleep state transition
//
VOID
FchEnableSleepTransition ( VOID )
{
  // Set SLP_TYPEn to enable SLP_TYP to do S1/S3/S5
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) |= BIT5;
  // Enable S state transition
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT0;
}


/*++

Routine Description:
  SMI handler to restore ACPI mode

Arguments:

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:

  None.

--*/
EFI_STATUS
EFIAPI
AmdSmiPowerButtonCallbackTest (
  IN       EFI_HANDLE                            DispatchHandle,
  IN       CONST FCH_SMM_PWRBTN_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                                  *CommBuffer OPTIONAL,
  IN OUT   UINTN                                 *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS     Status;
  Status = EFI_SUCCESS;
  return Status;
}


EFI_STATUS
EFIAPI
AmdSmiPowerButtonCallback (
  IN       EFI_HANDLE                            DispatchHandle,
  IN       CONST FCH_SMM_PWRBTN_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                                  *CommBuffer OPTIONAL,
  IN OUT   UINTN                                 *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS     Status;
  UINT32         AcpiMmioBase;
  UINT16         AcpiPmbase;
  UINT16         Value16;

  Status = EFI_SUCCESS;

  FchEnableSleepTransition ();
  AcpiMmioBase = 0xFED80000;
  //FchGetAcpiPmEvtBlk (&AcpiPmbase);
  AcpiPmbase = gFchDataInSmm.FchResetDataBlock.FchBldCfg.CfgAcpiPm1EvtBlkAddr;
  BackUpCG2 ();

  // Turn off SLP_TYP SMI
  RwMem (AcpiMmioBase + SMI_BASE + FCH_SMI_REGB0, AccessWidth32, (UINT32)~(BIT3 + BIT2), 0);

  // Clear power button status
  Value16 = BIT8;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, AcpiPmbase + R_FCH_ACPI_PM1_STATUS, &Value16);

  // power the system off now
  LibFchSmmIoRead (&gSmst->SmmIo, SMM_IO_UINT16, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &Value16);
  Value16 &= (UINT32)~(BIT12 + BIT11 + BIT10);
  Value16 |= 5 << 10;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &Value16);
  //Value16 |= BIT13;
  //LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT16, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &Value16);

  // from ZP to send MSG to SMU instead of write PmCnt directly
  AsmWbinvd();
  SmuNotifyS3Entry ();

  return Status;
}


