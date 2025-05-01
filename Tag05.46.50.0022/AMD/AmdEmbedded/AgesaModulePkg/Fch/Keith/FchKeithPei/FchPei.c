/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchPei.h"
#include "FchReset.h"
#include "FchReset2.h"
#include "FchStall.h"
#include <SMU_RMB_MsgDef.h>
#include <Library/FchPeiLib.h>

#include <Ppi/CpuIo.h>
#include <Ppi/Reset.h>
#include <Ppi/Reset2.h>
#include <Ppi/Stall.h>
#include <Ppi/MemoryDiscovered.h>
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieComplexPpi.h>
//#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/NbioPcieTrainingPpi.h>

#include <Library/BaseMemoryLib.h>

#define FILECODE FCH_KEITH_FCHKEITHPEI_FCHPEI_FILECODE

extern EFI_GUID gFchResetDataHobGuid;
extern FCH_RESET_DATA_BLOCK   InitResetCfgDefault;

//
// Module globals
//
STATIC EFI_PEI_RESET_PPI mResetPpi = {
  FchPeiReset
};

STATIC EFI_PEI_RESET2_PPI mResetPpi2 = {
  FchPeiReset2
};

STATIC EFI_PEI_STALL_PPI mStallPpi = {
  FCH_STALL_RESOLUTION_USEC,
  FchPeiStall
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiListReset = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiResetPpiGuid,
  &mResetPpi
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiListReset2 = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReset2PpiGuid,
  &mResetPpi2
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiListStall = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiStallPpiGuid,
  &mStallPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR   mMemoryDicoverPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  FchMemoryDiscoveredPpiCallback
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNbioPcieComplexPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieComplexPpiGuid,
  SataNbioPcieComplexPpiCallback
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mPeiNbioPcieTrainingDonePpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieTrainingDonePpiGuid,
  UsbPeiNbioPcieTrainingDonePpiCallback
};


//Data init routine to setup default value
EFI_STATUS
EFIAPI
FchInitResetDataDefault (
  IN       FCH_RESET_DATA_BLOCK  *FchParams
  );

EFI_STATUS
EFIAPI
FchInitPcdResetData (
  IN       FCH_RESET_DATA_BLOCK  *FchParams
  );

//main routine for Fch PEI init work
EFI_STATUS
EFIAPI
FchInitPei (
  IN       FCH_RESET_DATA_BLOCK  *FchParams
  );

/*********************************************************************************
 * Name: FchPeiInit
 *
 * Description:
 *               > Update boot mode
 *               > Install Reset PPI
 *               > Install SMBUS PPI
 *               > Install Stall PPI
 *
 * Input
 *   FfsHeader   : pointer to the firmware file system header
 *   PeiServices : pointer to the PEI service table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchPeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                Status;
  EFI_PEI_RESET_PPI         *PeiReset;
  EFI_PEI_RESET2_PPI        *PeiReset2;
  EFI_PEI_STALL_PPI         *PeiStall;
  EFI_HOB_GUID_TYPE         *FchHob;

  FCH_PEI_PRIVATE           *FchPrivate;
  EFI_PEI_PPI_DESCRIPTOR    *PpiListFchInit;
  FCH_RESET_DATA_BLOCK      *FchParams;

  UINT32                    Usb4PhyTuningSetSize;
  UINT32                    Usb4PhyTuningSetContent;
  UINT32                    Usb4PhyTuningSetPtr;

  AGESA_TESTPOINT (TpFchPeiEntry, NULL);
  //
  // Check SOC ID
  //


  //
  // Check Fch HW ID
  //


  // Create Fch GUID HOB to save FCH_RESET_DATA_BLOCK
  Status = (*PeiServices)->CreateHob (
                             PeiServices,
                             EFI_HOB_TYPE_GUID_EXTENSION,
                             sizeof (EFI_HOB_GUID_TYPE) + sizeof (FCH_RESET_DATA_BLOCK) + 1616,
                             &FchHob
                             );

  ASSERT_EFI_ERROR (Status);

  CopyMem (&FchHob->Name, &gFchResetDataHobGuid, sizeof (EFI_GUID));
  FchHob++;
  FchParams = (FCH_RESET_DATA_BLOCK *)FchHob;
  //load default to Fch data structure
  Status = FchInitResetDataDefault (FchParams);

  FchParams->pUsbInitData = (BIOSSMC_MSG_UsbInit_Data_t*) ((((UINTN)(FchParams + 1)) + 8) & 0xFFFFFFF8);
  FchParams->pUsbConfigRequest = (UsbConfigRequest_t*) (FchParams->pUsbInitData + 1);
  FchParams->pUsbInitData->Usb3EntriesOffset = sizeof (BIOSSMC_MSG_UsbInit_Data_t);

  //Init FCH_PEI_PRIVATE
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (FCH_PEI_PRIVATE),
                             &FchPrivate
                             );
  ASSERT_EFI_ERROR ( Status);

  FchPrivate->Signature                    = FCH_PEI_PRIVATE_DATA_SIGNATURE;
  FchPrivate->StdHdr.AltImageBasePtr       = (UINT32) PeiServices;
  FchPrivate->FchInitPpi.Revision          = AMD_FCH_INIT_PPI_REV;
  FchPrivate->FchInitPpi.FchResetData      = (VOID*) FchParams;
  FchPrivate->FchInitPpi.FpFchXhciRecovery = FchInitXhciOnRecovery;
  FchPrivate->FchInitPpi.FpFchEhciRecovery = FchInitEhciOnRecovery;
  FchPrivate->FchInitPpi.FpFchSataRecovery = FchInitSataOnRecovery;
  FchPrivate->FchInitPpi.FpFchGppRecovery  = FchInitGppOnRecovery;

  if (FchCheckRmbB0()){
    Usb4PhyTuningSetSize = PcdGet32(PcdUsb4PhyTuningSettingTableSize);
    Usb4PhyTuningSetContent = PcdGet32(PcdUsb4PhyTuningSettingTableContent);

    DEBUG ((DEBUG_INFO, "USB4 Phy Tuning Set Size %x\n", Usb4PhyTuningSetSize));
    DEBUG ((DEBUG_INFO, "USB4 Phy Tuning Set Content %x\n", Usb4PhyTuningSetContent));

    if ((Usb4PhyTuningSetContent != 0) && (Usb4PhyTuningSetSize != 0)) {
      Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               (sizeof (C20_N6_PHY_TUNING_SET) * Usb4PhyTuningSetSize) + sizeof(UINT64),
//[-start-240520-IB20840057-modify]//
                               (VOID **)&Usb4PhyTuningSetPtr
//[-end-240520-IB20840057-modify]//
                               );
      DEBUG ((DEBUG_INFO, "USB4 Phy Tuning Set Ptr %x\n", Usb4PhyTuningSetPtr));
      Usb4PhyTuningSetPtr += sizeof(UINT64);
      Usb4PhyTuningSetPtr &= ~ (UINT32)(BIT2 | BIT1 | BIT0);
      DEBUG ((DEBUG_INFO, "USB4 Phy Tuning Set aligned Ptr %x\n", Usb4PhyTuningSetPtr));
      if (!EFI_ERROR(Status)) {
        CopyMem ((VOID *)Usb4PhyTuningSetPtr,
                 (VOID *)Usb4PhyTuningSetContent,
                 sizeof (C20_N6_PHY_TUNING_SET) * Usb4PhyTuningSetSize);
        FchParams->Usb4PhyTuningSetPtr = (UINT32)Usb4PhyTuningSetPtr;
        FchParams->Usb4PhyTuningSetSize = (UINT32)Usb4PhyTuningSetSize;
      }
    }
  }

  //Do the real init tasks
  DEBUG ((DEBUG_INFO, "[FchInitPei] Fch Pei Init ...Start.\n"));
  Status = FchInitPei (FchParams);
  DEBUG ((DEBUG_INFO, "[FchInitPei] Fch Pei Init ...Complete.\n"));
  //
  // Update the boot mode
  //
  Status = FchUpdateBootMode ((EFI_PEI_SERVICES **)PeiServices);
  ASSERT_EFI_ERROR (Status);


  //
  // publish other PPIs
  //
  // Reset PPI
  // check to see if an instance is already installed
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gEfiPeiResetPpiGuid,
                             0,
                             NULL,
                             &PeiReset
                             );

  if (EFI_NOT_FOUND == Status) {
    // No instance currently installed, install our own
    Status = (*PeiServices)->InstallPpi (
                               PeiServices,
                               &mPpiListReset
                               );

    ASSERT_EFI_ERROR ( Status);
  }

  // Reset2 PPI
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gEfiPeiReset2PpiGuid,
                             0,
                             NULL,
                             &PeiReset2
                             );

  if (EFI_NOT_FOUND == Status) {
    // No instance currently installed, install our own
    Status = (*PeiServices)->InstallPpi (
                               PeiServices,
                               &mPpiListReset2
                               );

    ASSERT_EFI_ERROR ( Status);
  }

  // Stall PPI
  // check to see if an instance is already installed
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gEfiPeiStallPpiGuid,
                             0,
                             NULL,
                             &PeiStall
                             );

  if (EFI_NOT_FOUND == Status) {
    // There is no instance currently installed, install our own
    Status = (*PeiServices)->InstallPpi (
                               PeiServices,
                               &mPpiListStall
                               );

    ASSERT_EFI_ERROR ( Status);
  }

  // Allocate memory for the PPI descriptor
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (EFI_PEI_PPI_DESCRIPTOR),
                             &PpiListFchInit
                             );
  ASSERT_EFI_ERROR ( Status);

  PpiListFchInit->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListFchInit->Guid  = &gAmdFchInitPpiGuid;
  PpiListFchInit->Ppi   = &FchPrivate->FchInitPpi;

  Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             PpiListFchInit
                             );
  ASSERT_EFI_ERROR ( Status);

  Status = (*PeiServices)->NotifyPpi (PeiServices, &mMemoryDicoverPpiCallback);
  ASSERT_EFI_ERROR ( Status);

  if (FchParams->SataEnable[0]) {
    Status = (*PeiServices)->NotifyPpi (PeiServices, &mNbioPcieComplexPpiCallback);
    ASSERT_EFI_ERROR ( Status);
  }

  Status = (*PeiServices)->NotifyPpi (PeiServices, &mPeiNbioPcieTrainingDonePpiCallback);
  ASSERT_EFI_ERROR ( Status);

  AGESA_TESTPOINT (TpFchPeiExit, NULL);
  return Status;
}
/*********************************************************************************
 * Name: FchInitResetDataDefault
 *
 * Description:
 *               load Default value of FCH_RESET_DATA_BLOCK
 *
 * Input
 *   FchParams   : pointer to FCH_RESET_DATA_BLOCK
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchInitResetDataDefault (
  IN       FCH_RESET_DATA_BLOCK  *FchParams
  )
{
  EFI_STATUS                Status;
  *FchParams = InitResetCfgDefault;

  //Platform call out
  Status = FchPlatformOemPeiInit ((VOID *)FchParams);

  ASSERT_EFI_ERROR (Status);

  IDS_HOOK (IDS_HOOK_FCH_INIT_RESET, NULL, (VOID *)FchParams);

  FchInitPcdResetData (FchParams);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitPcdResetData (
  IN       FCH_RESET_DATA_BLOCK  *FchParams
  )
{
  UINT8 Value8;
  FchParams->FchBldCfg.CfgSmbus0BaseAddress = PcdGet16 (PcdAmdFchCfgSmbus0BaseAddress);
  FchParams->FchBldCfg.CfgSioPmeBaseAddress = PcdGet16 (PcdAmdFchCfgSioPmeBaseAddress);
  FchParams->FchBldCfg.CfgAcpiPm1EvtBlkAddr = PcdGet16 (PcdAmdFchCfgAcpiPm1EvtBlkAddr);
  FchParams->FchBldCfg.CfgAcpiPm1CntBlkAddr = PcdGet16 (PcdAmdFchCfgAcpiPm1CntBlkAddr);
  FchParams->FchBldCfg.CfgAcpiPmTmrBlkAddr  = PcdGet16 (PcdAmdFchCfgAcpiPmTmrBlkAddr);
  FchParams->FchBldCfg.CfgCpuControlBlkAddr = PcdGet16 (PcdAmdFchCfgCpuControlBlkAddr);
  FchParams->FchBldCfg.CfgAcpiGpe0BlkAddr   = PcdGet16 (PcdAmdFchCfgAcpiGpe0BlkAddr);
  FchParams->FchBldCfg.CfgSmiCmdPortAddr    = PcdGet16 (PcdAmdFchCfgSmiCmdPortAddr);

  //Dynamic PCDs
  FchParams->LegacyFree                    = PcdGetBool (PcdLegacyFree);
  FchParams->FchOscout1ClkContinous        = PcdGetBool (PcdFchOscout1ClkContinous);
  FchParams->FchReset.SataEnable           = PcdGetBool (PcdSataEnable);
  if (FchCheckAm5PkgType()){
    FchParams->FchReset.SataEnable = FALSE;
  }
  if (FchParams->FchReset.SataEnable == FALSE) {
    FCH_PCDSET8 (PcdSataEnable2, 0x00);        //Disable all Sata
    FCH_PCDSET32 (FchRTD3ColdEnableMap, (PcdGet32 (FchRTD3ColdEnableMap)) & ~(UINT32)BIT28);
  }
  FchParams->SataSetMaxGen2                = PcdGetBool (PcdSataSetMaxGen2);
  FchParams->SataClkMode                   = PcdGet8 (PcdSataClkMode);
  FchParams->FchReset.Xhci0Enable          = PcdGetBool (PcdXhci0Enable);
  FchParams->FchReset.Xhci1Enable          = PcdGetBool (PcdXhci1Enable);
  FchParams->Xhci2Enable                   = PcdGetBool (PcdXhci2Enable);
  FchParams->LpcClockDriveStrength         = PcdGet8 (PcdLpcClockDriveStrength);
  FchParams->XhciECCDedErrRptEn            = PcdGetBool (PcdXhciECCDedErrRptEn);
  FchParams->Mode                          = PcdGet8 (PcdResetMode);
  FchParams->SpiSpeed                      = PcdGet8 (PcdResetSpiSpeed);
  FchParams->FastSpeed                     = PcdGet8 (PcdResetFastSpeed);
  FchParams->WriteSpeed                    = PcdGet8 (PcdResetWriteSpeed);
  FchParams->SpiTpmSpeed                   = PcdGet8 (PcdResetSpiTpmSpeed);
  FchParams->SpiClkEarlier                 = PcdGetBool (PcdResetSpiClkEarlier);
  FchParams->Spi.LpcClk0                   = PcdGetBool (PcdLpcClk0);
  FchParams->Spi.LpcClk1                   = PcdGetBool (PcdLpcClk1);
  FchParams->EspiEnable                    = PcdGetBool (PcdEspiEnable);
  FchParams->EspiDevice0Enable             = PcdGetBool (PcdEspiDevice0Enable);
  FchParams->EspiIo80Enable                = PcdGetBool (PcdEspiIo80Enable);
  FchParams->EspiKbc6064Enable             = PcdGetBool (PcdEspiKbc6064Enable);
  FchParams->Gpp.SerialDebugBusEnable      = PcdGetBool (PcdSerialDebugBusEnable);
  FchParams->WdtEnable                     = PcdGetBool (PcdFchWdtEnable);
  FchParams->Xhci0DevRemovable             = PcdGet32 (PcdXhci0DevRemovable);
  FchParams->DisableXhciPortLate           = PcdGetBool (PcdDisableXhciPortLate);
  FchParams->XhciUsb3PortDisable           = PcdGet32 (PcdXhciUsb3PortDisable);
  FchParams->XhciUsb2PortDisable           = PcdGet32 (PcdXhciUsb2PortDisable);
  FchParams->XhciOCpinSelect               = PcdGet32 (PcdXhciOcPinSelect);
  FchParams->Xhci1OCpinSelect              = PcdGet32 (PcdXhci1OcPinSelect);
  FchParams->Xhci2OCpinSelect              = PcdGet32 (PcdXhci2OcPinSelect);
  FchParams->Xhci3OCpinSelect              = PcdGet32 (PcdXhci3OcPinSelect);
  FchParams->Xhci4OCpinSelect              = PcdGet32 (PcdXhci4OcPinSelect);
  FchParams->XhciOcPolarityCfgLow          = PcdGetBool (PcdXhciOcPolarityCfgLow);
  FchParams->XhciUsbDdiModeEnable          = PcdGet32 (PcdAmdBitMapDisaplyOnlyController);
  FchParams->Usb3PortForceGen1             = PcdGet8 (PcdXhciForceGen1);
  FchParams->ToggleAllPwrGoodOnCf9         = PcdGetBool (PcdToggleAllPwrGoodOnCf9);
  FchParams->OemUsbConfigurationTablePtr   = PcdGetPtr (PcdUsbKTOemConfigurationTable);

  FchParams->OemUsbConfigurationTable      =
                                *(FCH_KT_USB_OEM_PLATFORM_TABLE*)(UINT8*)(UINTN)FchParams->OemUsbConfigurationTablePtr;

  FchParams->SerialIrqEnable               = PcdGetBool (PcdSerialIrqEnable);
  FchParams->Espi.OperatingFreq            = PcdGet8 (PcdEspiOperatingFreq);
  FchParams->Espi.Channel                  = PcdGet8 (PcdEspiChannel);
  FchParams->Espi.IoMode                   = PcdGet8 (PcdEspiIoMode);
  FchParams->Espi.AlertMode                = PcdGet8 (PcdEspiAlertMode);
  FchParams->Espi.Decode.Io[0].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[0].Enable;
  FchParams->Espi.Decode.Io[0].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[0].Base;
  FchParams->Espi.Decode.Io[0].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[0].Size;
  FchParams->Espi.Decode.Io[1].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[1].Enable;
  FchParams->Espi.Decode.Io[1].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[1].Base;
  FchParams->Espi.Decode.Io[1].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[1].Size;
  FchParams->Espi.Decode.Io[2].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[2].Enable;
  FchParams->Espi.Decode.Io[2].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[2].Base;
  FchParams->Espi.Decode.Io[2].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[2].Size;
  FchParams->Espi.Decode.Io[3].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[3].Enable;
  FchParams->Espi.Decode.Io[3].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[3].Base;
  FchParams->Espi.Decode.Io[3].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[3].Size;
  FchParams->Espi.Decode.Io[4].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[4].Enable;
  FchParams->Espi.Decode.Io[4].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[4].Base;
  FchParams->Espi.Decode.Io[4].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[4].Size;
  FchParams->Espi.Decode.Io[5].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[5].Enable;
  FchParams->Espi.Decode.Io[5].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[5].Base;
  FchParams->Espi.Decode.Io[5].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[5].Size;
  FchParams->Espi.Decode.Io[6].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[6].Enable;
  FchParams->Espi.Decode.Io[6].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[6].Base;
  FchParams->Espi.Decode.Io[6].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[6].Size;
  FchParams->Espi.Decode.Io[7].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[7].Enable;
  FchParams->Espi.Decode.Io[7].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[7].Base;
  FchParams->Espi.Decode.Io[7].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[7].Size;
  FchParams->Espi.Decode.Io[8].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[8].Enable;
  FchParams->Espi.Decode.Io[8].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[8].Base;
  FchParams->Espi.Decode.Io[8].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[8].Size;
  FchParams->Espi.Decode.Io[9].Enable      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[9].Enable;
  FchParams->Espi.Decode.Io[9].Base        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[9].Base;
  FchParams->Espi.Decode.Io[9].Size        = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[9].Size;
  FchParams->Espi.Decode.Io[10].Enable     = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[10].Enable;
  FchParams->Espi.Decode.Io[10].Base       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[10].Base;
  FchParams->Espi.Decode.Io[10].Size       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[10].Size;
  FchParams->Espi.Decode.Io[11].Enable     = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[11].Enable;
  FchParams->Espi.Decode.Io[11].Base       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[11].Base;
  FchParams->Espi.Decode.Io[11].Size       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[11].Size;
  FchParams->Espi.Decode.Io[12].Enable     = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[12].Enable;
  FchParams->Espi.Decode.Io[12].Base       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[12].Base;
  FchParams->Espi.Decode.Io[12].Size       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[12].Size;
  FchParams->Espi.Decode.Io[13].Enable     = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[13].Enable;
  FchParams->Espi.Decode.Io[13].Base       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[13].Base;
  FchParams->Espi.Decode.Io[13].Size       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[13].Size;
  FchParams->Espi.Decode.Io[14].Enable     = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[14].Enable;
  FchParams->Espi.Decode.Io[14].Base       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[14].Base;
  FchParams->Espi.Decode.Io[14].Size       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[14].Size;
  FchParams->Espi.Decode.Io[15].Enable     = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[15].Enable;
  FchParams->Espi.Decode.Io[15].Base       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[15].Base;
  FchParams->Espi.Decode.Io[15].Size       = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Io[15].Size;
  FchParams->Espi.Decode.Mmio[0].Enable    = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[0].Enable;
  FchParams->Espi.Decode.Mmio[0].Base      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[0].Base;
  FchParams->Espi.Decode.Mmio[0].Size      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[0].Size;
  FchParams->Espi.Decode.Mmio[1].Enable    = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[1].Enable;
  FchParams->Espi.Decode.Mmio[1].Base      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[1].Base;
  FchParams->Espi.Decode.Mmio[1].Size      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[1].Size;
  FchParams->Espi.Decode.Mmio[2].Enable    = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[2].Enable;
  FchParams->Espi.Decode.Mmio[2].Base      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[2].Base;
  FchParams->Espi.Decode.Mmio[2].Size      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[2].Size;
  FchParams->Espi.Decode.Mmio[3].Enable    = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[3].Enable;
  FchParams->Espi.Decode.Mmio[3].Base      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[3].Base;
  FchParams->Espi.Decode.Mmio[3].Size      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[3].Size;
  FchParams->Espi.Decode.Mmio[4].Enable    = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[4].Enable;
  FchParams->Espi.Decode.Mmio[4].Base      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[4].Base;
  FchParams->Espi.Decode.Mmio[4].Size      = ((FCH_ESPI_DECODE *)PcdGetPtr (PcdEspiIoMmioDecodeV2))->Mmio[4].Size;
  FchParams->Espi.IrqPolarity              = PcdGet32 (PcdEspiIrqPolarity);
  FchParams->Espi.IrqMask                  = PcdGet32 (PcdEspiIrqMask);
  FchParams->BpX48M0ClockEnable            = PcdGetBool (PcdBpX48M0ClockEnable);
  FchParams->TurnOffXtalS3S5               = PcdGetBool (PcdTurnOffXtalS3S5);
  FchParams->AmdEnvironmentFlag            = PcdGet32 (PcdAmdEnvironmentFlag);
  FchParams->Xtal48MPadPowerSaving         = PcdGetBool (PcdXtal48MPadPowerSaving);
  FchParams->PdInterruptModeEn             = PcdGet8 (PcdPdInterruptModeEn);
  FchParams->DeviceEnableMap               = PcdGet32 (FchRTDeviceEnableMap);
  FchParams->I2cI3cEarlyInit               = PcdGetBool (PcdFchI2cI3cEarlyInit);
  FchParams->UsbSparseModeEnable           = PcdGetBool (PcdUsbSparseModeEnable);
  FchParams->Usb4PhyTuningEnable           = PcdGetBool (PcdUsb4PhyTuningEnable);
  //Sata controller
  {
    UINT8 SataController;
    UINT8 SataEnable2;

    SataEnable2 = PcdGet8 (PcdSataEnable2);
    for (SataController = 0; SataController < KEITH_SATA_CONTROLLER_NUM; SataController++) {
      if (SataEnable2 & (1 << SataController)) {
        FchParams->SataEnable[SataController] = TRUE;
      } else {
        FchParams->SataEnable[SataController] = FALSE;
      }
    }
  }

  if (FchParams->AmdEnvironmentFlag != 0) {
    FchParams->XhciUsb3PortDisable           |= BIT1 | BIT3; //PLAT-54867
  }

  // USB4
  FchParams->Usb4Host[0].Usb4InitEnable = PcdGetBool (PcdUsb40Enable);
  FchParams->Usb4Host[1].Usb4InitEnable = PcdGetBool (PcdUsb41Enable);
  FchParams->Usb4Host[0].Usb4HostEnable = PcdGetBool (PcdUsb4Rt0En);
  FchParams->Usb4Host[1].Usb4HostEnable = PcdGetBool (PcdUsb4Rt1En);
  FchParams->Usb4Host[0].Usb3HCDisable = ~ (UINT8) (PcdGet8 (PcdUsb4Rt0XhciEn));
  FchParams->Usb4Host[1].Usb3HCDisable = ~ (UINT8) (PcdGet8 (PcdUsb4Rt1XhciEn));
  FchParams->Usb4Host[0].Usb4Tbt3NotSupport = (PcdGetBool (PcdUsb4Rt0Tbt3En) == TRUE) ? FALSE : TRUE;
  FchParams->Usb4Host[1].Usb4Tbt3NotSupport = (PcdGetBool (PcdUsb4Rt1Tbt3En) == TRUE) ? FALSE : TRUE;

  // USB4 - USB3 tunneling
  //
  // Router0
  if (PcdGetBool (PcdUsb4Rt0Usb3TnlEn)) {
    // USB3 Tunneling enable
    FchParams->Usb4Host[0].Usb4Usb3TunnelingDisable = 0;
    FchParams->Usb4Host[0].Usb4Usb3AdpHidden = 0;
  } else {
    if (FchParams->Usb4Host[0].Usb4Usb3TunnelingDisable == 0) {
      FchParams->Usb4Host[0].Usb4Usb3AdpHidden = BIT0;       // Hide USB3 Adapter if not set to disable tunnel
    } else {
      FchParams->Usb4Host[0].Usb4Usb3AdpHidden = 0;
    }
  }

  // Router1
  if (PcdGetBool (PcdUsb4Rt1Usb3TnlEn)) {
    // USB3 Tunneling enable
    FchParams->Usb4Host[1].Usb4Usb3TunnelingDisable = 0;
    FchParams->Usb4Host[1].Usb4Usb3AdpHidden = 0;
  } else {
    if (FchParams->Usb4Host[1].Usb4Usb3TunnelingDisable == 0) {
      FchParams->Usb4Host[1].Usb4Usb3AdpHidden = BIT0;       // Hide USB3 Adapter if not set to disable tunnel
    } else {
      FchParams->Usb4Host[1].Usb4Usb3AdpHidden = 0;
    }
  }

  // USB4 - PCIe tunneling
  //
  // Router0
  if (PcdGetBool (PcdUsb4Rt0PcieTnlEn)) {
    // PCIe Tunneling enable
    FchParams->Usb4Host[0].Usb4PcieTunnelingDisable = 0;
    FchParams->Usb4Host[0].Usb4PcieAdpHidden = 0;
  } else {
    if (FchParams->Usb4Host[0].Usb4PcieAdpHidden == 0) {
      FchParams->Usb4Host[0].Usb4PcieTunnelingDisable = BIT0;
    } else {
      FchParams->Usb4Host[0].Usb4PcieTunnelingDisable = 0;
    }
  }

  // Router1
  if (PcdGetBool (PcdUsb4Rt1PcieTnlEn)) {
    // USB3 Tunneling enable
    FchParams->Usb4Host[1].Usb4PcieTunnelingDisable = 0;
    FchParams->Usb4Host[1].Usb4PcieAdpHidden = 0;
  } else {
    if (FchParams->Usb4Host[1].Usb4PcieAdpHidden == 0) {
      FchParams->Usb4Host[1].Usb4PcieTunnelingDisable = BIT0;       // Hide PCIe Adapter if not set to disable tunnel
    } else {
      FchParams->Usb4Host[1].Usb4PcieTunnelingDisable = 0;
    }
  }

  // USB4 - DP tunneling
  //
  // Router0
  if (PcdGetBool (PcdUsb4Rt0DpTnlEn)) {
    // DP Tunneling enable
    FchParams->Usb4Host[0].Usb4DPTunnelingDisable = FALSE;
    FchParams->Usb4Host[0].Usb4DPAdpHidden = 0;
  } else {
    if (FchParams->Usb4Host[0].Usb4DPTunnelingDisable == FALSE) {
      FchParams->Usb4Host[0].Usb4DPAdpHidden = BIT0 + BIT1;  // Hide DP Adapter if not set to disable tunnel
    } else {
      FchParams->Usb4Host[0].Usb4DPAdpHidden = 0;
    }
  }

  // Router1
  if (PcdGetBool (PcdUsb4Rt1DpTnlEn)) {
    // DP Tunneling enable
    FchParams->Usb4Host[1].Usb4DPTunnelingDisable = FALSE;
    FchParams->Usb4Host[1].Usb4DPAdpHidden = 0;
  } else {
    if (FchParams->Usb4Host[1].Usb4DPTunnelingDisable == FALSE) {
      FchParams->Usb4Host[1].Usb4DPAdpHidden = BIT0 + BIT1;  // Hide DP Adapter if not set to disable tunnel
    } else {
      FchParams->Usb4Host[1].Usb4DPAdpHidden = 0;
    }
  }
  // i2c4_padctrl:i2crxsel
  Value8 = PcdGet8 (PcdFch_i2c4_padctrl_i2crxsel);
  if (0xFF != Value8) {
    FchParams->i2c4_padctrl_i2crxsel = Value8 & 3;
  }
  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchInitPei
 *
 * Description:
 *               Initialization for FCH controller
 *
 * Input
 *   FchParams   : pointer to FCH_RESET_DATA_BLOCK
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchInitPei (
  IN       FCH_RESET_DATA_BLOCK  *FchParams
  )
{
  FchInitReset (FchParams);

  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchGetBootMode
 *
 * Description:
 *   This function determines whether the platform is resuming from an S state
 *   using the FCH ACPI registers
 *
 * Input:
 *   PeiServices : a pointer to the PEI service table pointer
 *   pFchPrivate  : pointer to the FCH PEI private data structure
 *
 * Output:
 *   EFI_BOOT_MODE : Boot mode from SB
 *
 *********************************************************************************/
EFI_BOOT_MODE
FchGetBootMode (
  IN       EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_PEI_CPU_IO_PPI  *CpuIo;
  UINT16              FchBootMode;
  UINTN               AcpiPm1Ctl;

  //
  // find the CpuIo protocol
  //
  CpuIo = (*PeiServices)->CpuIo;

  //
  // Check the FCH WAK_STS bit in the ACPI_PM1_CTL register
  //
  // get the address PM1_CTL register address
  AcpiPm1Ctl  = LibFchPmIoRead16V2 (PeiServices, FCH_PMIOA_REG62);
  AcpiPm1Ctl  &= ~BIT0;

  // get the boot mode as seen by the south bridge
  FchBootMode = (CpuIo->IoRead16 (PeiServices, CpuIo, AcpiPm1Ctl) & ACPI_BM_MASK);

  // convert the boot mode to the EFI version
  if (ACPI_S3 == FchBootMode) {
    return (BOOT_ON_S3_RESUME);
  }
  if (ACPI_S4 == FchBootMode) {
    return (BOOT_ON_S4_RESUME);
  }
  if (ACPI_S5 == FchBootMode) {
    return (BOOT_ON_S5_RESUME);
  }
  // S0 or unsupported Sx mode
  return (BOOT_WITH_FULL_CONFIGURATION);
}


/*********************************************************************************
 * Name: FchUpdateBootMode
 *
 * Description:
 *   This function update the platform boot mode based on the information
 *   gathered from the south bridge.
 *   Note that we do not publish the BOOT_MODE PPI since the platform
 *   is responsible for deciding what the actual boot mode is.
 *
 * Input:
 *   PeiServices : a pointer to the PEI service table pointer
 *   pFchPrivate  : pointer to the FCH PEI private data structure
 *
 * Output:
 *
 *********************************************************************************/
EFI_STATUS
FchUpdateBootMode (
  IN       EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_BOOT_MODE  BootMode;
  EFI_BOOT_MODE  FchBootMode;

  // Get FCH Boot mode
  FchBootMode = FchGetBootMode (PeiServices);

  // Get the platform boot mode
  (*PeiServices)->GetBootMode (
                    PeiServices,
                    &BootMode
                    );

  // Update boot mode if we are more important than the platform
  if ((BOOT_IN_RECOVERY_MODE != BootMode)
    && (BOOT_ON_FLASH_UPDATE != BootMode)
    && (BOOT_WITH_FULL_CONFIGURATION != FchBootMode))
  {
    // Set Sx boot mode
    (*PeiServices)->SetBootMode (
                      PeiServices,
                      FchBootMode
                      );
  }
  return (EFI_SUCCESS);
}

EFI_STATUS
EFIAPI
FchMemoryDiscoveredPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_HOB_GUID_TYPE         *FchHob;
  FCH_RESET_DATA_BLOCK      *FchResetParams;
  AMD_FCH_INIT_PPI          *FchInitPpi;
  EFI_STATUS                Status;

  Status = EFI_SUCCESS;

  FchHob = GetFirstGuidHob (&gFchResetDataHobGuid);
  if (FchHob == NULL) {
    DEBUG ((DEBUG_INFO, "FCH HOB Not located, Exiting.\n"));
    return EFI_NOT_FOUND;
  }
  FchHob++;
  FchResetParams = (FCH_RESET_DATA_BLOCK *)FchHob;

  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdFchInitPpiGuid,
                             0,
                             NULL,
                             &FchInitPpi
                             );
  ASSERT_EFI_ERROR ( Status);

  FchInitPpi->FchResetData      = (VOID*) FchResetParams;
  return Status;
}

EFI_STATUS
EFIAPI
SataNbioPcieComplexPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN  VOID                            *InvokePpi
  )
{
  BOOLEAN                               SataPresent = FALSE;
  PEI_AMD_NBIO_PCIE_COMPLEX_PPI         *NbioPcieComplexPpi;
  DXIO_COMPLEX_DESCRIPTOR               *PcieComplex;
  DXIO_PORT_DESCRIPTOR                  *PortNode;
  EFI_STATUS                            Status = EFI_SUCCESS;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioPcieComplexPpiGuid, 0, NULL, &NbioPcieComplexPpi);

  if (!EFI_ERROR (Status)) {
    NbioPcieComplexPpi->PcieGetComplex (NbioPcieComplexPpi, &PcieComplex);
    PortNode = PcieComplex->PciePortList;
    do {
      if ((PortNode->EngineData.EngineType == DxioSATAEngine) && (PortNode->Port.PortPresent == DxioPortEnabled)) {
        SataPresent = TRUE;
        FchKTSataInitBootUpSata (0, TRUE, NULL);
        FchKTSataInitEnableSata (0, 0, NULL);
      }
      PortNode++;
    } while ((PortNode->Flags & DESCRIPTOR_TERMINATE_LIST) == 0);

    if (SataPresent == FALSE){
      PcdSetBoolS (PcdSataEnable, 0);
      PcdSet8S (PcdSataEnable2, 0);
      PcdSet32S (FchRTD3ColdEnableMap, (PcdGet32 (FchRTD3ColdEnableMap)) & ~(UINT32)BIT28);
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
UsbPeiNbioPcieTrainingDonePpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN  VOID                            *InvokePpi
  )
{
  FCH_RESET_DATA_BLOCK      *FchResetParams;
  AMD_FCH_INIT_PPI          *FchInitPpi;
  EFI_STATUS                Status;

  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdFchInitPpiGuid,
                             0,
                             NULL,
                             &FchInitPpi
                             );
  ASSERT_EFI_ERROR ( Status);

  FchResetParams = FchInitPpi->FchResetData;

  //Do the real init tasks
  DEBUG ((DEBUG_INFO, "UsbPeiNbioPcieTrainingDonePpiCallback ...Start.\n"));
  if ((BOOT_ON_S4_RESUME == FchGetBootMode (PeiServices)) || (BOOT_ON_S5_RESUME == FchGetBootMode (PeiServices))) {
    FchKTUsbSmuServiceNew(0, BIOSSMC_MSG_UsbInit,FchResetParams);
  } else {
    FchKTUsbSmuServiceReqOnly(0, BIOSSMC_MSG_UsbInit,FchResetParams);
  }
  DEBUG ((DEBUG_INFO, "UsbPeiNbioPcieTrainingDonePpiCallback ...Complete.\n"));
  return Status;
}

EFI_STATUS
EFIAPI
FchInitXhciOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       UINT32             XhciRomAddress
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  FchKTSmuServiceOnly(0,0,0);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitEhciOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       UINT32             EhciTemporaryBarAddress
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  //FchEhciOnRecovery (*FchPrivate, EhciTemporaryBarAddress);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitSataOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       UINT32             SataBar0,
  IN       UINT32             SataBar5
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  FchKTSataRecovery (0);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitGppOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       FCH_GPP_R          *FchGpp
  )
{
  return EFI_SUCCESS;
}
