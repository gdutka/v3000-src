/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchSmm.h"
#define FILECODE FCH_KEITH_FCHKEITHSMM_MISCSMI_FILECODE

extern FCH_DATA_BLOCK                      gFchDataInSmm;

EFI_STATUS
EFIAPI
AmdSmiSysResetCallback (
  IN       EFI_HANDLE                            DispatchHandle,
  IN       CONST FCH_SMM_MISC_REGISTER_CONTEXT *DispatchContext
  );

EFI_STATUS
FchSmmRegisterMiscSmi (
  VOID
  )
{
  EFI_STATUS                                 Status;
  FCH_SMM_MISC_DISPATCH_PROTOCOL             *FchSmmMiscDispatch;
  FCH_SMM_MISC_REGISTER_CONTEXT              MiscRegisterContext;
  EFI_HANDLE                                 MiscHandle;
  FCH_DATA_BLOCK                             *FchDb;
  //UINT8                                      GppHpGeventNum;

  FchDb = &gFchDataInSmm;

  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmMiscDispatchProtocolGuid,
                  NULL,
                  &FchSmmMiscDispatch
                  );
  ASSERT_EFI_ERROR (Status);
  MiscRegisterContext.Order        = 0x80;
  MiscHandle                       = NULL;

  ////
  //// Sys Reset trapping
  ////

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG80;
  MiscRegisterContext.SmiStatusBit = BIT19;
  MiscRegisterContext.Order        = 0x80;
  Status = FchSmmMiscDispatch->Register (
                                   FchSmmMiscDispatch,
                                   AmdSmiSysResetCallback,
                                   &MiscRegisterContext,
                                   &MiscHandle
                                   );
  // Disable Sys Reset function
  ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC4) &= ~(BIT5);
  // Enable Sys Reset SMI
  ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA4) &= ~(BIT7);
  ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA4) |= (BIT6);

  ////
  //// Smi SBTSI test Done
  ////
  //Status = gBS->LocateProtocol (
  //                &gFchSmmMiscDispatchProtocolGuid,
  //                NULL,
  //                &FchSmmMiscDispatch
  //                );
  //ASSERT_EFI_ERROR (Status);

  //MiscRegisterContext.SmiStatusReg = FCH_SMI_REG84;
  //MiscRegisterContext.SmiStatusBit = BIT15;
  //MiscRegisterContext.Order        = 0x80;
  //Status = FchSmmMiscDispatch->Register (
  //                                 FchSmmMiscDispatch,
  //                                 AmdMiscFchTsiSmiCallback,
  //                                 &MiscRegisterContext,
  //                                 &MiscHandle
  //                                 );
  //ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAB) &= ~(BIT7);
  //ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAB) |= (BIT6);

  //
  // Initialize GPP hotplug SMI
  //
  //if (FchDb->Gpp.PortCfg[0].PortHotPlug || FchDb->Gpp.PortCfg[1].PortHotPlug ||  \
  //    FchDb->Gpp.PortCfg[2].PortHotPlug || FchDb->Gpp.PortCfg[3].PortHotPlug) {
  //  GppHpGeventNum = FchDb->Gpp.GppHotPlugGeventNum & 31;
  //  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG80;
  //  MiscRegisterContext.SmiStatusBit = 1 << GppHpGeventNum;
  //  MiscRegisterContext.Order        = 0x80;
  //  Status = FchSmmMiscDispatch->Register (
  //                                 FchSmmMiscDispatch,
  //                                 AmdMiscFchGppHpSmiCallback,
  //                                 &MiscRegisterContext,
  //                                 &MiscHandle
  //                                 );
  //  ACPIMMIO8 (
  // ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA0 + ((GppHpGeventNum << 1) / 8)) &=
  // ~(0x03 << ((GppHpGeventNum << 1) % 8)
  // );
  //  ACPIMMIO8 (
  // ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA0 + ((GppHpGeventNum << 1) / 8)) |=
  // (0x01 << ((GppHpGeventNum << 1) % 8)
  // );
  //}

  return Status;
}


/*++

Routine Description:
  SMI handler to restore ACPI mode

Arguments:

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the FCH_SMM_SW_DISPATCH_CONTEXT

Returns:

  None.

--*/
/*
EFI_STATUS
EFIAPI
AmdMiscFchTsiSmiCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT     *DispatchContext
  )
{
  EFI_STATUS     Status;
  UINT8          bData;
//  UINT8          *pData;
  Status = 0;
  //to be implemented

  bData = 0xff;
  LibAmdIoWrite (AccessWidth8, 0xb00, &bData, NULL);
  LibAmdIoRead (AccessWidth8, 0xb04, &bData, NULL);

  LibAmdIoWrite (AccessWidth8, 0xb04, &bData, NULL);
  //LibAmdIoWrite (AccessWidth8, 0x80, &bData, NULL);
  while (ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84 + 1 ) & (BIT7))
  {
    ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84 + 1 ) |= (BIT7);
  }
  bData = 0xff;
  LibAmdIoWrite (AccessWidth8, 0xb00, &bData, NULL);

  return Status;
}
*/

/*++

Routine Description:
  SMI handler to handle GPP hotplug

Arguments:

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the FCH_SMM_SW_DISPATCH_CONTEXT

Returns:

  None.

--*/
/*
EFI_STATUS
EFIAPI
AmdMiscFchGppHpSmiCallback (
  IN  EFI_HANDLE                         DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT      *DispatchContext
  )
{
  FCH_DATA_BLOCK     *FchDb;

  FchDb = &gFchInitInSmm.FchPolicy;
  FchGppHotplugSmiCallbackPtr (FchDb);
  return EFI_SUCCESS;
}
*/
EFI_STATUS
EFIAPI
AmdSmiSysResetCallback (
  IN       EFI_HANDLE                            DispatchHandle,
  IN       CONST FCH_SMM_MISC_REGISTER_CONTEXT *DispatchContext
  )
{
  EFI_STATUS     Status;

  Status = EFI_SUCCESS;
  KTeSPI_NoticeSlaveBeforeUserReset ();

  //Emulate a Reset Button event
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC4, AccessWidth32, (UINT32)~(BIT6), BIT6);

  // System Reset happens here and never come back here.
  FCH_DEADLOOP ();

  return Status;
}


