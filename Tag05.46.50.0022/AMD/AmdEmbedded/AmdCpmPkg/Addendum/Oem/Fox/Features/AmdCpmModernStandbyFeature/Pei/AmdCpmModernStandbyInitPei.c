/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <AmdCpmPei.h>
#include <AmdCpmBase.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include "FchPlatform.h"
#include <Ppi/AmdFchInitPpi.h>

#define WWAN_D3_COLD_EN             BIT5
#define WLAN_D3_COLD_EN             BIT6

/**
 This is the declaration of Modern Standby Init PEIM entry point.

 @param[in]         FileHandle          Handle of the file being invoked.
 @param[in]         PeiServices         Describes the list of possible PEI Services.

 @retval            EFI_SUCCESS         The operation completed successfully.
 @retval            Others              An unexpected error occurred.
*/
EFI_STATUS
EFIAPI
AmdCpmModernStandbyInitPeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES        **PeiServices
  )
{
  EFI_STATUS                          Status;
  AMD_CPM_TABLE_PPI                   *AmdCpmTablePpi;
  AMD_PBS_SETUP_OPTION                AmdPbsConfiguration;
  UINT8                               Value8;
  UINT8                               Offset;
  UINT32                              D3ColdEnMap;
  AMD_FCH_INIT_PPI                    *AmdFchInitPpi;
  FCH_RESET_DATA_BLOCK                *FchParams;
  AMD_CPM_NVME_RTD3_TABLE             *NvmeRtd3TablePtr;
  UINT8                               Index = 0;

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint start\n"));

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint-LocatePpi-gAmdCpmTablePpiGuid\n"));
  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint-End-1-Status=%r\n", Status));
    return Status;
  }

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint-End-2-Status=%r\n", Status));
    return Status;
  }

  if (AmdPbsConfiguration.ModernStandbyEn == 1) {
    PcdSetBoolS(PcdModernStandbyEnable, 1);

    //disable Wdt
    PcdSetBoolS(PcdFchWdtEnable, 0);
    Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdFchInitPpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdFchInitPpi
                           );
    if (!EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint-Disable WDT.\n"));
      FchParams = (FCH_RESET_DATA_BLOCK *)AmdFchInitPpi->FchResetData;
      FchParams->WdtEnable = 0;
    }
    AmdCpmTablePpi->CommonFunction.MmioAndThenOr8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, 0x7F, 0);  // WatchdogTmrEn:0

    //Enable s0i3 or s0i2
    if (AmdPbsConfiguration.ModernStandbyType == 0) {
      //enable s0i2
      PcdSet8S(PcdS0i2Enable, 1);
      PcdSet8S(PcdS0i3Enable, 0);
    }else if (AmdPbsConfiguration.ModernStandbyType == 1) {
    //enable s0i3
      PcdSet8S(PcdS0i2Enable, 0);
      PcdSet8S(PcdS0i3Enable, 1);
    } else if (AmdPbsConfiguration.ModernStandbyType == 2) {
    //enable s0i3 + s0i2
      PcdSet8S(PcdS0i2Enable, 1);
      PcdSet8S(PcdS0i3Enable, 1);
    }

    if (PcdGetBool(PcdPlatformLpiEn)) {
      PcdSet8S(PcdAmdAcpiCpuSsdtProcessorLpiC3ParentState, 0x01);
      PcdSet8S(PcdAmdAcpiCpuSsdtProcessorLpiC2ParentState, 0x00);
      PcdSet8S(PcdAmdAcpiC3Ctrl, 0x02);
      PcdSetBoolS(PcdAmdAcpiCstC3, TRUE);
    }

    // temporary workaround to fill missing programming in FCH
    AmdCpmTablePpi->CommonFunction.MmioAndThenOr32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG44, (0xFFFFFFFF&(~BIT27)), 0);       // BootTmrFuncEn = 0

    D3ColdEnMap = PcdGet32(FchRTD3ColdEnableMap);
    D3ColdEnMap |= BIT28;   //enable SATA d3-cold BY DEFAULT
    D3ColdEnMap |= BIT30;   //enable USB4 d3-cold BY DEFAULT
    PcdSet32S(FchRTD3ColdEnableMap, D3ColdEnMap);
    //enable dummy sata d3cold
    PcdSetBoolS(FchSataDummyD3ColdSupport, 1);
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint- FCH DEVICES D3COLD enable MAP =0x%x\n", D3ColdEnMap));

    // set PcdFchAoacInitEnable
    PcdSetBoolS(PcdFchAoacInitEnable, 1);

    //set PcdFchMiscModernStanbyEnable
    PcdSetBoolS(PcdFchMiscModernStanbyEnable, 1);

    //Platfrom-wide _OSC to enable _PR3
    PcdSetBoolS(PcdModernStandbyPlatformOscEnable, TRUE);

    //ECRAM program
    Offset = 0xB7;  //ECRAMB7[7] 1-ms supported 0-s3 supported
    AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);

    if ((Value8 & BIT7) != 0x80) {
      Value8 |= BIT7;
      AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);
    }

    if (AmdPbsConfiguration.NvmeD3cold) {
      NvmeRtd3TablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_NVME_STD3);
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint-NvmeRtd3TablePtr : %x\n", NvmeRtd3TablePtr));
      if (NvmeRtd3TablePtr) {
        NvmeRtd3TablePtr->NvmeD3ColdSupported = AmdPbsConfiguration.NvmeD3cold;
        DEBUG((DEBUG_INFO, "OEM-PEI-NvmeD3ColdSupported : %x\n", NvmeRtd3TablePtr->NvmeD3ColdSupported));
        NvmeRtd3TablePtr->NvmeD3cConfigSize   = sizeof(AMD_CPM_NVME_D3C_CONFIG_LIST);
        DEBUG((DEBUG_INFO, "OEM-PEI-NvmeD3cConfigSize : %x\n", NvmeRtd3TablePtr->NvmeD3cConfigSize));
        for (Index = 0; NvmeRtd3TablePtr->NvmeList[Index].Dev != 0xFF; Index++) {
          if (!NvmeRtd3TablePtr->NvmeList[Index].Tpvperl) {
            NvmeRtd3TablePtr->NvmeList[Index].Tpvperl = AmdPbsConfiguration.Tpvperl;
          }
          if (!NvmeRtd3TablePtr->NvmeList[Index].Trstcfg) {
            NvmeRtd3TablePtr->NvmeList[Index].Trstcfg = AmdPbsConfiguration.Trstcfg;
          }
          DEBUG((DEBUG_INFO, "OEM-PEI-NvmeList[%d].Tpvperl : %d\n", Index, NvmeRtd3TablePtr->NvmeList[Index].Tpvperl));
          DEBUG((DEBUG_INFO, "OEM-PEI-NvmeList[%d].Trstcfg : %d\n", Index, NvmeRtd3TablePtr->NvmeList[Index].Trstcfg));
        }
      }
    }
  } else {
    PcdSetBoolS(PcdModernStandbyEnable, 0);

   PcdSet8S(PcdAmdAcpiCpuSsdtProcessorLpiC3ParentState, 0x00);
   PcdSet8S(PcdAmdAcpiCpuSsdtProcessorLpiC2ParentState, 0x00);

   // set PcdFchAoacInitEnable
    PcdSetBoolS(PcdFchAoacInitEnable, 0);

   //set PcdFchMiscModernStanbyEnable
    PcdSetBoolS(PcdFchMiscModernStanbyEnable, 0);

    Offset = 0xB7;  //ECRAMB7[7] 1-ms supported 0-s3 supported
    AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);

    if ((Value8 & BIT7) != 0x00) {
      Value8 &= (~BIT7);
      AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);
    }
  }

  // Enable WWAN D3 Cold
  Offset = 0xCD;
  AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
  Value8 = Value8 | WWAN_D3_COLD_EN;
  AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);

  if (AmdPbsConfiguration.ReduceHardwareEn == 1) {
    PcdSetBoolS(PcdReduceHardwareEnable, 1);

    // for hardware reduced only, PwrBtnEn = 0, AcpiReducedHWEn = 1
    AmdCpmTablePpi->CommonFunction.MmioAndThenOr8 (ACPI_MMIO_BASE + PMIO_BASE + 0x75, 0xFC, 0x02);
    //power button
    AmdCpmTablePpi->CommonFunction.MmioAndThenOr32 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x00, 0xFFFF00FF, 0x3000E100);
    // (EC SCI) => AGPIO07 (GPIOBase+0x24)
    AmdCpmTablePpi->CommonFunction.MmioAndThenOr32 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x24, 0xFFFF00FF, 0x30002000);
  } else {
    PcdSetBoolS(PcdReduceHardwareEnable, 0);
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmModernStandbyInitPeiEntryPoint-End-Status=%r\n", Status));

  return EFI_SUCCESS;
}
