/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTHWACPI_KTSSSERVICE_FILECODE

/**
 * FchInitResetAcpiMmioTable - Fch ACPI MMIO initial
 * during the power on stage.
 *
 *
 *
 *
 */
ACPI_REG_WRITE FchInitResetAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  {PMIO_BASE >> 8,  FCH_PMIOA_REG00 + 2, 0xFB, BIT2},            /// Set ASF SMBUS master function enabled here
  {PMIO_BASE >> 8,  FCH_PMIOA_REGD2, 0xCF, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG44 + 3, 0x67, 0x88},            /// Stop Boot timer
  {PMIO_BASE >> 8,  FCH_PMIOA_REG00, 0xF7, 0x77},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08, 0xFE, BIT2 + BIT4},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG10, 0xFE, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG10, 0xFD, 0x02},                /// toggleallpwrgoodoncf9 PLAT-48147
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54, 0x00, BIT4 + BIT6},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG74, 0xF6, BIT0},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGC4, 0xEE, 0x04},                /// Release NB_PCIE_RST
  {PMIO_BASE >> 8,  FCH_PMIOA_REGC0 + 2, 0xBF, 0x40},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGC4, 0xFB, 0},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGDC, 0x00, 0},                   /// Clear XDC SATA configuration

  {MISC_BASE >> 8,  FCH_MISC_REG6C + 2, 0x7F, BIT7},             // MISC 0x6C BIT23
  {MISC_BASE >> 8,  FCH_MISC_REG6C + 3, 0xF7, BIT3},             // MISC 0x6C BIT27
  {MISC_BASE >> 8,  FCH_MISC_REG4C + 1, 0xFB, BIT2},             // MISC 0x4C BIT10

  {0xFF, 0xFF, 0xFF, 0xFF},
};

ACPI_REG_WRITE FchKeithInitResetRtcextTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x01},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x02},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x03},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x04},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x10},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x11},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x12},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x13},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5E, 0x00, 0x14},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5F, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * ProgramFchHwAcpiResetP
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchHwAcpiResetP (
  IN VOID  *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

}

VOID
FchInitEnableWdt (
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;

  if (LocalCfgPtr->WdtEnable) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth8, 0x7F, BIT7);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth8, 0x7F, 0);
  }
}

VOID
FchInitEnableI2cI3c (
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  UINT32                    FchDeviceEnMap;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  FchDeviceEnMap = LocalCfgPtr->DeviceEnableMap;

  //
  // I2C0
  //
  if ( FchDeviceEnMap & BIT5 ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C0, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x91, AccessWidth8, 0, 0x0);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x92, AccessWidth8, 0, 0x0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I2C0, 0);
  }
  //
  // I2C1
  //
  if ( FchDeviceEnMap & BIT6 ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C1, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x93, AccessWidth8, 0, 0x0);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x94, AccessWidth8, 0, 0x0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I2C1, 0);
  }
  //
  // I2C2
  //
  if ( FchDeviceEnMap & BIT7 ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C2, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x71, AccessWidth8, 0, 0x1);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x72, AccessWidth8, 0, 0x1);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I2C2, 0);
  }
  //
  // I2C3
  //
  if ( FchDeviceEnMap & BIT8 ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C3, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x13, AccessWidth8, 0, 0x1);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x14, AccessWidth8, 0, 0x1);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I2C3, 0);
  }

  //
  // I3C0
  //
  if ( FchDeviceEnMap & BIT21 ) {
    FchAoacPowerOnDev (FCH_AOAC_I3C0, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x91, AccessWidth8, 0, 0x1);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x92, AccessWidth8, 0, 0x1);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I3C0, 0);
  }
  //
  // I3C1
  //
  if ( FchDeviceEnMap & BIT13 ) {
    FchAoacPowerOnDev (FCH_AOAC_I3C1, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x93, AccessWidth8, 0, 0x1);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x94, AccessWidth8, 0, 0x1);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I3C1, 0);
  }
  //
  // I3C2
  //
  if ( FchDeviceEnMap & BIT14 ) {
    FchAoacPowerOnDev (FCH_AOAC_I3C2, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x71, AccessWidth8, 0, 0x2);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x72, AccessWidth8, 0, 0x2);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I3C2, 0);
  }
  //
  // I3C3
  //
  if ( FchDeviceEnMap & BIT15 ) {
    FchAoacPowerOnDev (FCH_AOAC_I3C3, 1);
    // IOMUX
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x13, AccessWidth8, 0, 0x2);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x14, AccessWidth8, 0, 0x2);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_I3C3, 0);
  }
}

/**
 * ProgramResetRtcExt - Config RTC External registers
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramResetRtcExt (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                  RtcExtData;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //check if RTCext data lost
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E, AccessWidth8, 0, 0x01);
  ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F, AccessWidth8, &RtcExtData);
  if ( RtcExtData == 0xFF ) {
    ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKeithInitResetRtcextTable[0]), StdHeader);
  }
}


/**
 * ProgramCpuRstBTmr - Config RESET_L time
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramCpuRstBTmr (
  IN  VOID     *FchDataPtr
  )
{

}

