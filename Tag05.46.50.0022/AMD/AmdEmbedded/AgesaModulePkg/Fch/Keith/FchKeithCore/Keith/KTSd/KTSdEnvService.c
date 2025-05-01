/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSD_KTSDENVSERVICE_FILECODE
/**
 * FchInitEnvSdProgram - Config SD controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSdProgram (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // SD Configuration
  //
  if (LocalCfgPtr->Sd.SdConfig == SdDisable) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x00);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE8, AccessWidth8, 0xFE, 0x00);
    //ACPIMMIO8 (FCH_AOACx72_SD1_D3_CONTROL) &= ~ AOAC_PWR_ON_DEV;
    ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_REG72) &= ~ AOAC_PWR_ON_DEV;
  } else if (LocalCfgPtr->Sd.SdConfig != SdDump) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x40);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE8, AccessWidth8, 0xFE, BIT0);

    if (LocalCfgPtr->Sd.SdSsid != NULL ) {
      RwPci ((KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REG2C, AccessWidth32, 0, LocalCfgPtr->Sd.SdSsid, StdHeader);
    }

    RwPci (
      (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4,
      AccessWidth32,
      0x3FFFFFFF,
      ((UINT32) (LocalCfgPtr->Sd.SdSlotType) << 30),
      StdHeader
      );

    if ( LocalCfgPtr->Sd.SdConfig == SdVer2) {
// SD 2.0
//The following programming sequence sets up SD controller to operate in SD 2.0 mode. (UBTS538428).
//A. Program D14F7xA4[BaseFreq]= 32h.
//B. Program D14F7xA8=[Ddr50Sup]=0.
//C. Program D14F7xA8=[Sdr100Sup]=0.
//D. Program D14F7xA8=[Sdr50Sup]=0.
//E. Program D14F7xB0=[HostVersionSel]=1h.
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4,
        AccessWidth32, ~ (UINT32) (0xFF << 8),
        (UINT32) (0x32 << 8),
        StdHeader
        );
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8,
        AccessWidth32,
        ~ (UINT32) (0x01 << 2),
        (UINT32) (0x00 << 2),
        StdHeader
        );
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8,
        AccessWidth32,
        ~ (UINT32) (0x01 << 1),
        (UINT32) (0x00 << 1),
        StdHeader
        );
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8,
        AccessWidth32, ~ (UINT32) (0x01 << 0),
        (UINT32) (0x00 << 0),
        StdHeader
        );
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0,
        AccessWidth32,
        ~ (UINT32) (0xFF << 24),
        (UINT32) (0x01 << 24),
        StdHeader
        );
    } else {
      //  no SD3.0 POR on TS
    }
    if ( LocalCfgPtr->Sd.SdForce18 ) {
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0,
        AccessWidth32,
        ~ ((UINT32) (0x01 << 17)),
        ((UINT32) (0x01) << 17),
        StdHeader
        );
    }

    if (LocalCfgPtr->Sd.SdDbgConfig == 1) {
      //DMA clear BIT19,BIT20
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4,
        AccessWidth32,
        ~ (UINT32) (BIT19 + BIT20),
        0,
        StdHeader
        );
    }

    if (LocalCfgPtr->Sd.SdDbgConfig == 2) {
      //PIO clear BIT19,BIT20,BIT22
      RwPci (
        (KEITH_SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4,
        AccessWidth32,
        ~ (UINT32) (BIT19 + BIT20 + BIT22),
        0,
        StdHeader
        );
    }
  }
}

