/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchSmm.h"
#include <SMU_RMB_MsgDef.h>
#define FILECODE FCH_KEITH_FCHKEITHSMM_SXSMI_FILECODE

extern UINT8                               *mFchPciIrqRoutingTable;
extern FCH_DATA_BLOCK                      gFchDataInSmm;
extern UINT8                               *mPspMboxSmmBuffer;
extern BOOLEAN                             *mPspMboxSmmFlagAddr;

EFI_STATUS
FchSmmRegisterSxSmi (
  VOID
  )
{
  EFI_STATUS                               Status;
  FCH_SMM_SX_DISPATCH2_PROTOCOL            *AmdSxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT              SxRegisterContext;
  EFI_HANDLE                               SxHandle;

  //
  // Register AMD SX SMM
  //
  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSxDispatch2ProtocolGuid,
                  NULL,
                  &AmdSxDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  SxRegisterContext.Type  = SxS3;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdSmiS3SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS4;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdSmiS4SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS5;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdSmiS5SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  return Status;
}

/**
 * FchKTUsb3SxEntry
 *
 *
 * @retval  VOID
 *
 */
VOID
FchKTUsb3SxEntry (
  OUT VOID
  )
{
  UINT32                UsbD3TimeOut;
  UINT32                CurrentPowerState;
  USB_CONFIG_DATA       UsbConfigData;
  EFI_STATUS            Status;
  BOOLEAN               Usb0InD3;
  BOOLEAN               Usb1InD3;
  BOOLEAN               Usb2InD3;
  BOOLEAN               Usb3InD3;
  BOOLEAN               Usb7InD3;
  BOOLEAN               IsRmbB0 = FALSE;

  IsRmbB0 = FchCheckRmbB0();

  UsbConfigData.UsbConfigureOp    = MBOX_USB_CONFIG_OP_RD;
  UsbConfigData.UsbRegisterID     = ID_CURRENT_POWER_STATE_CNTR0;
  UsbConfigData.UsbConfigureData0 = 0;
  CurrentPowerState               = 0;
  UsbD3TimeOut                    = 0;
  Status = EFI_SUCCESS;
  Usb0InD3 = FALSE;
  Usb1InD3 = FALSE;
  Usb2InD3 = FALSE;
  Usb3InD3 = FALSE;
  Usb7InD3 = FALSE;

  if (gFchDataInSmm.FchResetDataBlock.FchReset.Xhci0Enable == FALSE) {
    //skip USB0 if disabled
    Usb0InD3 = TRUE;
  }

  if (gFchDataInSmm.FchResetDataBlock.FchReset.Xhci1Enable == FALSE) {
    //skip USB1 if disabled
    Usb1InD3 = TRUE;
  }

  if (gFchDataInSmm.FchResetDataBlock.Xhci2Enable == FALSE) {
    //skip USB2 if disabled
    Usb2InD3 = TRUE;
  }

  if (IsRmbB0 == TRUE) {
    if (gFchDataInSmm.FchResetDataBlock.Usb4Host[0].Usb3HCDisable) {
      //skip USB3 if disabled
      Usb3InD3 = TRUE;
    }

    if (gFchDataInSmm.FchResetDataBlock.Usb4Host[1].Usb3HCDisable) {
      //skip USB7 if disabled
      Usb7InD3 = TRUE;
    }
  } else {
    //skip USB3/7
    Usb3InD3 = TRUE;
    Usb7InD3 = TRUE;
  }

  if (mPspMboxSmmBuffer != NULL) {
    do {
      if (UsbD3TimeOut != 0) {
        FchStall(100, NULL);
      }
      UsbD3TimeOut = UsbD3TimeOut + 100;
      // USB0
      if (!Usb0InD3) {
        UsbConfigData.UsbConfigureData0 = 0;
        UsbConfigData.UsbRegisterID &= 0x00FFFFFF;
        UsbConfigData.UsbRegisterID |= 0x00000000;
        Status = PspMboxBiosCmdUsbConfig (
                   &UsbConfigData,
                   mPspMboxSmmBuffer,
                   mPspMboxSmmFlagAddr
                   );
        if (Status == EFI_SUCCESS) {
          CurrentPowerState = UsbConfigData.UsbConfigureData0;
          if ((CurrentPowerState & (BIT8 + BIT9)) == (BIT8 + BIT9)) {
            // USB0 in D3
            Usb0InD3 = TRUE;
            DEBUG((DEBUG_INFO, "USB0 in D3\n"));
          }
        }
      }

      // USB1
      if (!Usb1InD3) {
        UsbConfigData.UsbConfigureData0 = 0;
        UsbConfigData.UsbRegisterID &= 0x00FFFFFF;
        UsbConfigData.UsbRegisterID |= 0x01000000;
        Status = PspMboxBiosCmdUsbConfig (
                   &UsbConfigData,
                   mPspMboxSmmBuffer,
                   mPspMboxSmmFlagAddr
                   );
        if (Status == EFI_SUCCESS) {
          CurrentPowerState = UsbConfigData.UsbConfigureData0;
          if ((CurrentPowerState & (BIT8 + BIT9)) == (BIT8 + BIT9)) {
            // USB1 in D3
            Usb1InD3 = TRUE;
            DEBUG((DEBUG_INFO, "USB1 in D3\n"));
          }
        }
      }

      // USB2
      if (!Usb2InD3) {
        UsbConfigData.UsbConfigureData0 = 0;
        UsbConfigData.UsbRegisterID &= 0x00FFFFFF;
        UsbConfigData.UsbRegisterID |= 0x02000000;
        Status = PspMboxBiosCmdUsbConfig (
                   &UsbConfigData,
                   mPspMboxSmmBuffer,
                   mPspMboxSmmFlagAddr
                   );
        if (Status == EFI_SUCCESS) {
          CurrentPowerState = UsbConfigData.UsbConfigureData0;
          if ((CurrentPowerState & (BIT8 + BIT9)) == (BIT8 + BIT9)) {
            // USB2 in D3
            Usb2InD3 = TRUE;
            DEBUG((DEBUG_INFO, "USB2 in D3\n"));
          }
        }
      }

      if (IsRmbB0 == TRUE) {
        // USB3
        if (!Usb3InD3) {
          UsbConfigData.UsbConfigureData0 = 0;
          UsbConfigData.UsbRegisterID &= 0x00FFFFFF;
          UsbConfigData.UsbRegisterID |= 0x03000000;
          Status = PspMboxBiosCmdUsbConfig (
                     &UsbConfigData,
                     mPspMboxSmmBuffer,
                     mPspMboxSmmFlagAddr
                     );
          if (Status == EFI_SUCCESS) {
            CurrentPowerState = UsbConfigData.UsbConfigureData0;
            if ((CurrentPowerState & (BIT8 + BIT9)) == (BIT8 + BIT9)) {
              // USB3 in D3
              Usb3InD3 = TRUE;
              DEBUG((DEBUG_INFO, "USB3 in D3\n"));
            }
          }
        }

        // USB7
        if (!Usb7InD3) {
          UsbConfigData.UsbConfigureData0 = 0;
          UsbConfigData.UsbRegisterID &= 0x00FFFFFF;
          UsbConfigData.UsbRegisterID |= 0x04000000;
          Status = PspMboxBiosCmdUsbConfig (
                     &UsbConfigData,
                     mPspMboxSmmBuffer,
                     mPspMboxSmmFlagAddr
                     );
          if (Status == EFI_SUCCESS) {
            CurrentPowerState = UsbConfigData.UsbConfigureData0;
            if ((CurrentPowerState & (BIT8 + BIT9)) == (BIT8 + BIT9)) {
              // USB7 in D3
              Usb7InD3 = TRUE;
              DEBUG((DEBUG_INFO, "USB7 in D3\n"));
            }
          }
        }
      }
    } while (((Usb0InD3 == FALSE) || \
              (Usb1InD3 == FALSE) || \
              (Usb2InD3 == FALSE) || \
              ((Usb3InD3 == FALSE) && (IsRmbB0 == TRUE)) || \
              ((Usb7InD3 == FALSE) && (IsRmbB0 == TRUE))) && \
             (UsbD3TimeOut < 5000));
  }

  FchKTXhciSmuService (0, BIOSSMC_MSG_UsbSxEntry);
}

/*++

Routine Description:


Arguments:

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:

  None.

--*/
EFI_STATUS
EFIAPI
AmdSmiS3SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  UINT8           Index;
  UINT8           *pData;
  FCH_DATA_BLOCK  *pFchPolicy;
  BOOLEAN         EspiEnable;

  pFchPolicy = &gFchDataInSmm;
  // Save entire FCH PCI IRQ routing space (C00/C01)
  pData = mFchPciIrqRoutingTable;
  Index = 0xFF;
  do {
    Index++;
    LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC00, &Index);
    LibFchSmmIoRead (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC01, pData++);
  } while (Index != 0xFF);

  //Put Usb3 to S0 power rail
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEE, AccessWidth8, (UINT32)~(BIT0 + BIT1), (BIT1 + BIT0));
  BackUpCG2 ();
  FchKTUsb3SxEntry ();
  ProgramPMEDis (0, FALSE);

  EspiEnable = pFchPolicy->FchResetDataBlock.EspiEnable;
  if(EspiEnable == FALSE){
      FchAoacPowerOnDev (FCH_AOAC_ESPI, 1);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdSmiS4SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  FCH_DATA_BLOCK  *pFchPolicy;
  BOOLEAN         EspiEnable;

  pFchPolicy = &gFchDataInSmm;
  //Put Usb3 to S0 power rail
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEE, AccessWidth8, (UINT32)~(BIT0 + BIT1), (BIT1 + BIT0));
  BackUpCG2 ();
  FchKTUsb3SxEntry ();
  ProgramPMEDis (0, FALSE);

  EspiEnable = pFchPolicy->FchResetDataBlock.EspiEnable;
  if(EspiEnable == FALSE){
      FchAoacPowerOnDev (FCH_AOAC_ESPI, 1);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdSmiS5SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  FCH_DATA_BLOCK  *pFchPolicy;
  BOOLEAN         EspiEnable;

  pFchPolicy = &gFchDataInSmm;
  BackUpCG2 ();
  //RwMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04 + 2, AccessWidth8, ~BIT3, BIT3);
  FchKTUsb3SxEntry ();

  EspiEnable = pFchPolicy->FchResetDataBlock.EspiEnable;
  if(EspiEnable == FALSE){
      FchAoacPowerOnDev (FCH_AOAC_ESPI, 1);
  }

  return EFI_SUCCESS;
}

