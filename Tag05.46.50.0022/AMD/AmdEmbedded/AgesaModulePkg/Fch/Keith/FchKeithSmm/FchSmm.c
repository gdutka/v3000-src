/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchSmm.h"
#define FILECODE FCH_KEITH_FCHKEITHSMM_FCHSMM_FILECODE

#include <Library/BaseMemoryLib.h>

//FCH_INIT_PROTOCOL                   gFchInitInSmm;
FCH_DATA_BLOCK                      gFchDataInSmm;
UINT8                               *mFchPciIrqRoutingTable;
UINT8                               *mPspMboxSmmBuffer = NULL;
BOOLEAN                             *mPspMboxSmmFlagAddr = NULL;
FCH_SMM_INIT_PROTOCOL               *mFchSmmInitProtocol;

EFI_STATUS
FchSmmRegistrationCenter (
  VOID
  )
{
  EFI_STATUS     Status;

  Status = FchSmmRegisterSwSmi ();
  Status = FchSmmRegisterPowerButtonSmi ();
  Status = FchSmmRegisterPeriodicTimerSmi ();
  Status = FchSmmRegisterGpiSmi ();
  Status = FchSmmRegisterSxSmi ();
  Status = FchSmmRegisterIoTrapSmi ();
  Status = FchSmmRegisterMiscSmi ();
  return Status;
}

/**
 * FchSmmKTXhciDisablePort  -  Xhci Disable Port Control
 *
 *                          RMB USB ports
 *                             HC0: 2 USB3 + 4 USB2
 *                             HC1: 2 USB3 + 3 USB2
 *                             HC2: 0 USB3 + 1 USB2
 *                             HC3: 1 USB3 + 1 USB2
 *                             HC4: 1 USB3 + 1 USB2
 */
EFI_STATUS
EFIAPI
FchSmmKTXhciDisablePort (
  IN       CONST FCH_SMM_INIT_PROTOCOL   *This              ///< FCH SMM INIT Protocol
  )
{
  UINT32                      Usb3PortDisable;
  UINT32                      Usb2PortDisable;
  UINT32                      UsbPortDisable;
  UINT32                      UsbController;
  UINT32                      UsbControllerNum;
  USB_CONFIG_DATA             UsbConfigData[5];
  EFI_STATUS                  Status;
  BOOLEAN               IsRmbB0 = FALSE;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = &gFchDataInSmm.FchResetDataBlock;

  IsRmbB0 = FchCheckRmbB0();

  if (IsRmbB0) {
    UsbControllerNum = 5;
  } else {
    UsbControllerNum = 3;
  }

  Status = EFI_SUCCESS;

  if (LocalCfgPtr->XhciUsb3PortDisable || LocalCfgPtr->XhciUsb2PortDisable) {
      Usb3PortDisable = LocalCfgPtr->XhciUsb3PortDisable;
      Usb2PortDisable = LocalCfgPtr->XhciUsb2PortDisable;

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchSmmKTXhciDisablePort is started!\n");
      // Port0-5 : Controller0
      if (LocalCfgPtr->FchReset.Xhci0Enable) {
        UsbConfigData[0].UsbConfigureData0 = ((Usb2PortDisable >> 0) & 0x0F) + (((Usb3PortDisable >> 0) & 0x3) << 16);
      }
      // Port0-4 : Controller1
      if (LocalCfgPtr->FchReset.Xhci1Enable) {
        UsbConfigData[1].UsbConfigureData0 = ((Usb2PortDisable >> 4) & 0x07) + (((Usb3PortDisable >> 2) & 0x3) << 16);
      }
      // Port0 : Controller2
      if (LocalCfgPtr->Xhci2Enable) {
        UsbConfigData[2].UsbConfigureData0 = ((Usb2PortDisable >> 8) & 0x01) + (((Usb3PortDisable >> 4) & 0x0) << 16);
      }
      // Port0-1 : Controller3
      if ((LocalCfgPtr->Usb4Host[0].Usb3HCDisable & BIT0) == 0) {
        UsbConfigData[3].UsbConfigureData0 = ((Usb2PortDisable >> 12) & 0x01) + (((Usb3PortDisable >> 6) & 0x1) << 16);
      }
      // Port0-1 : Controller4
      if ((LocalCfgPtr->Usb4Host[1].Usb3HCDisable & BIT0) == 0) {
        UsbConfigData[4].UsbConfigureData0 = ((Usb2PortDisable >> 16) & 0x01) + (((Usb3PortDisable >> 8) & 0x1) << 16);
      }

      for (UsbController = 0; UsbController < UsbControllerNum; UsbController ++) {
        UsbPortDisable = UsbConfigData[UsbController].UsbConfigureData0;
        if (UsbPortDisable) {
          UsbConfigData[UsbController].UsbConfigureData1 = UsbPortDisable;
          UsbConfigData[UsbController].UsbRegisterID = (UsbController << 24) | ID_PORT_DISABLE_WRITE_ONCE_CNTR;
          UsbConfigData[UsbController].UsbConfigureOp    = MBOX_USB_CONFIG_OP_WR;
          DEBUG ((EFI_D_INFO, "Set USB:%x Port Disable, UsbRegisterID:%x ,UsbPortDisable:%x\n",\
            (UINTN)UsbController, (UINTN)UsbConfigData[UsbController].UsbRegisterID, (UINTN)UsbPortDisable));
          Status = PspMboxBiosCmdUsbConfig (
                     &UsbConfigData[UsbController],
                     mPspMboxSmmBuffer,
                     mPspMboxSmmFlagAddr
                     );
          if (Status == EFI_SUCCESS) {
          } else {
            DEBUG((DEBUG_INFO, "Set USB port disable fails\n"));
          }
          UsbConfigData[UsbController].UsbConfigureOp    = MBOX_USB_CONFIG_OP_RD;
          UsbConfigData[UsbController].UsbConfigureData0 = 0;
          Status = PspMboxBiosCmdUsbConfig (
                     &UsbConfigData[UsbController],
                     mPspMboxSmmBuffer,
                     mPspMboxSmmFlagAddr
                     );
          if (Status == EFI_SUCCESS) {
          } else {
            DEBUG((DEBUG_INFO, "Get USB port disable fails\n"));
          }
          if (UsbConfigData[UsbController].UsbConfigureData0 != UsbPortDisable) {
            DEBUG((DEBUG_INFO, "Set Get USB:%x comparision fails. Set:%x Get:%x\n", \
              (UINTN)UsbController, (UINTN)UsbPortDisable, (UINTN)UsbConfigData[UsbController].UsbConfigureData0));
          }
        }
      }

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchSmmKTXhciDisablePort is completed!\n");
  }
  return Status;
}


/*********************************************************************************
 * Name: FchSmmEntryPoint
 *
 * Description
 *   Entry point of the AMD FCH SMM driver
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
EFIAPI
FchSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  FCH_INIT_PROTOCOL                   *pFchCimxInitProtocol;
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;
  EFI_HANDLE                          FchSmmInitHandle;
  EFI_STATUS                          Status;

  AGESA_TESTPOINT (TpFchSmmEntry, NULL);
  //
  // Initialize global variables
  //
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    0x100,
                    &mFchPciIrqRoutingTable
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_INIT_PROTOCOL),
                    &mFchSmmInitProtocol
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &pFchCimxInitProtocol
                  );
  if (!EFI_ERROR (Status)) {
    CopyMem (
      &gFchDataInSmm,
      pFchCimxInitProtocol->FchPolicy,
      sizeof (FCH_DATA_BLOCK)
      );
  } else {
    return EFI_ABORTED;
  }

  //Save PspMboxSmmBuffer to globol variable
  Status = gSmst->SmmLocateProtocol (
                    &gPspMboxSmmBufferAddressProtocolGuid,
                    NULL,
                    &PspMboxSmmBufferAddressProtocol
                    );
  if (Status == EFI_SUCCESS) {
    mPspMboxSmmBuffer   = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
    mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
  }
  //Copy OEM table pointed by Fch parameter
  Status = FchSmmCopyOemTable ();

  Status = FchSmmRegistrationCenter ();

  //
  // install SMM protocol
  //
  mFchSmmInitProtocol->Revision                    = pFchCimxInitProtocol->Revision;
  mFchSmmInitProtocol->FchRev                      = pFchCimxInitProtocol->FchRev;
  mFchSmmInitProtocol->FchSmmPolicy                = &gFchDataInSmm;
  mFchSmmInitProtocol->FchSmmSecureUsbPortDisable  = FchSmmKTXhciDisablePort;

  if (!gFchDataInSmm.FchResetDataBlock.DisableXhciPortLate) {
    FchSmmKTXhciDisablePort (mFchSmmInitProtocol);
  }

  FchSmmInitHandle =  NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &FchSmmInitHandle,
                    &gFchSmmInitProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    mFchSmmInitProtocol
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AGESA_TESTPOINT (TpFchSmmExit, NULL);
  return Status;
}

EFI_STATUS
FchSmmCopyOemTable (
  VOID
  )
{
  VOID                        *OemProgTblPtr;
  UINTN                       OemProgTblSize;
  UINT8                       i;
  ACPI_REG_WRITE              *pAcpiTbl;
  EFI_STATUS                  Status;

  Status = EFI_NOT_FOUND;                            //no table found by default
  //
  // Copy gFchDataInSmm.HwAcpi.OemProgrammingTablePtr to SMM space for SMM handler to access
  //
  OemProgTblSize = 0;
  i=0;
  pAcpiTbl = gFchDataInSmm.HwAcpi.OemProgrammingTablePtr;
  if (pAcpiTbl != NULL) {
    if ((pAcpiTbl->MmioReg == 0)
        && (pAcpiTbl->MmioBase == 0)
        && (pAcpiTbl->DataAndMask == 0xB0)
        && (pAcpiTbl->DataOrMask == 0xAC))
    {
      // Signature Checking
      pAcpiTbl++;
      OemProgTblSize = sizeof (ACPI_REG_WRITE);
      for ( i = 1; pAcpiTbl->MmioBase < 0x1D; i++ ) {
        pAcpiTbl++;
      }
      OemProgTblSize += i*(sizeof (ACPI_REG_WRITE));
    }
    if (OemProgTblSize != 0) {
      Status = gSmst->SmmAllocatePool (
                        EfiRuntimeServicesData,
                        OemProgTblSize,
                        &OemProgTblPtr
                        );
      if (!EFI_ERROR (Status)) {
        CopyMem (
          OemProgTblPtr,
          gFchDataInSmm.HwAcpi.OemProgrammingTablePtr,
          OemProgTblSize
          );

        gFchDataInSmm.HwAcpi.OemProgrammingTablePtr = OemProgTblPtr;  //Update pointer in SMM
      }
    }
  }
  return Status;
}
