/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTHWACPI_KTHWACPIRESET_FILECODE

extern ACPI_REG_WRITE FchInitResetAcpiMmioTable[];
extern VOID  ProgramFchHwAcpiResetP (IN VOID  *FchDataPtr);

/**
 * FchInitResetHwAcpiP - Config HwAcpi controller ( Preliminary
 * ) during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetHwAcpiP (
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  AGESA_TESTPOINT (TpFchInitResetHwAcpiP, NULL);

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;

  StdHeader = LocalCfgPtr->StdHeader;
  //
  // Enabled (Mmio_mem_enable)
  //
  RwPmio (FCH_PMIOA_REG04, AccessWidth8, 0xFF, BIT1, StdHeader);

  ACPIMMIO32 (ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_REG94) &= ~ (S0I3CTRL_ARBITERDIS + S0I3CTRL_INTERRUPTDIS);

  RwPmio (FCH_PMIOA_REG08 + 2, AccessWidth8, 0xFF, BIT4, StdHeader);

  ProgramFchHwAcpiResetP (FchDataPtr);


  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG70, AccessWidth32, ~(UINT32) BIT15, 0x00); //PLAT-50619
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG94, AccessWidth8, ~(UINT32) 0x0F, 0x0D); //PLAT-51692

  //
  // enable CF9
  //
  RwPmio (FCH_PMIOA_REGD2, AccessWidth8, ~(UINT32) BIT6, 0, StdHeader);

  RwPmio (FCH_PMIOA_REGC0, AccessWidth8, 0, BIT1, StdHeader);          //Clear 4s shutdown event status
}

/**
 * FchInitResetHwAcpi - Config HwAcpi controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetHwAcpi (
  IN  VOID     *FchDataPtr
  )
{
  UINT16       SmbusBase;
  UINT8        Value;
  UINT32       Value32;
  UINT16       AsfPort;
  UINT32       GeventEnableBits;
  UINT32       GeventValue;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  AGESA_TESTPOINT (TpFchInitResetHwAcpi, NULL);

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // Enabled Base Address
  //
  SmbusBase = LocalCfgPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth16, 0x00FF, SmbusBase + BIT4);
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgAcpiPm1EvtBlkAddr)
    );
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG62,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgAcpiPm1CntBlkAddr)
    );
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgAcpiPmTmrBlkAddr)
    );
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG66,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgCpuControlBlkAddr)
    );
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG68,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgAcpiGpe0BlkAddr)
    );
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgSmiCmdPortAddr)
    );
  RwMem (
    ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6E,
    AccessWidth16,
    00,
    (LocalCfgPtr->FchBldCfg.CfgSmiCmdPortAddr) + 8
    );
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6C, AccessWidth16, 00, 0xFFFF);

  //
  // SmBus init
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) (BIT19 + BIT20), 0);
  Value = 0x00;
  LibAmdIoWrite (AccessWidth8, SmbusBase + 0x14, &Value, StdHeader);

  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchInitResetAcpiMmioTable[0]), StdHeader);

  FchInitEnableWdt (LocalCfgPtr);

  ProgramCpuRstBTmr (LocalCfgPtr);

  ProgramResetRtcExt (LocalCfgPtr);

  if (LocalCfgPtr->FchBldCfg.CfgFchSciMapControl != NULL) {
    ProgramFchSciMapTbl ((LocalCfgPtr->FchBldCfg.CfgFchSciMapControl), LocalCfgPtr);
  }

  //
  // RTC Workaround for Daylight saving time enable bit
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E, AccessWidth8, 0, 0);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F, AccessWidth8, 0xFE, BIT0 );   // Enable DltSavEnable
  Value = 0x0B;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  Value &= 0xFE;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E, AccessWidth8, 0, 0);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F, AccessWidth8, 0xFE, 0 );      // Disable DltSavEnable
  //
  // Prevent RTC error
  //
  Value = 0x0A;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  Value &= 0xEF;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);

  if ( LocalCfgPtr->FchBldCfg.CfgFchRtcWorkAround ) {
    Value = RTC_WORKAROUND_SECOND;
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
    LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    if ( Value > RTC_VALID_SECOND_VALUE ) {
      Value = RTC_SECOND_RESET_VALUE;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    }
    LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    Value &= RTC_SECOND_LOWER_NIBBLE;
    if ( Value > RTC_VALID_SECOND_VALUE_LN ) {
      LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
      Value = RTC_SECOND_RESET_VALUE;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    }
  }

  Value = 0x08;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);

  if ( !LocalCfgPtr->EcKbd ) {
    //
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    //
    Value = Value | 0x0A;
  }
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);

  Value = 0x09;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);
  if ( !LocalCfgPtr->EcKbd ) {
    //
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    //
    Value = Value & 0xF9;
  }

  Value = Value & 0x9F;
  Value = Value | BIT4;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);

  AsfPort = SmbusBase + 0x20;
  if ( AsfPort != 0 ) {
    UINT8  dbValue;
    dbValue = 0x2F;
    LibAmdIoWrite (AccessWidth8, AsfPort + 0x0A, &dbValue, StdHeader);
  }
  //
  // PciExpWakeStatus workaround
  //
  ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60, AccessWidth16, &AsfPort);
  AsfPort++;
  ReadMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04, AccessWidth32, &GeventEnableBits);
  ReadMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG00, AccessWidth32, &GeventValue);
  if ( (GeventValue & GeventEnableBits) != 0 ) {
    Value = 0x40;
    LibAmdIoWrite (AccessWidth8, AsfPort, &Value, StdHeader);
  }
  LibAmdIoRead (AccessWidth8, AsfPort, &Value, StdHeader);
  if ((Value & (BIT2 + BIT0)) != 0) {
    Value = 0x40;
    LibAmdIoWrite (AccessWidth8, AsfPort, &Value, StdHeader);
  }

  if ( LocalCfgPtr->FchOscout1ClkContinous ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG54, AccessWidth8, 0xBF, 0);
  }
  if (LocalCfgPtr->SerialIrqEnable){
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG54, AccessWidth8, 0x7F, BIT7);
  }
  //
  // Clear RTCD Date Alarm
  //
  Value = 0x0D;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  if ((Value & 0x3F) == 0x3F) {
    Value &= 0x80;
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  }

  if ( LocalCfgPtr->ToggleAllPwrGoodOnCf9 ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10, AccessWidth8, 0xFD, 2);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10, AccessWidth8, 0xFD, 0);
  }

  if ( LocalCfgPtr->BpX48M0ClockEnable ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_PMIOA_REG40, AccessWidth8, 0xFB, BIT2);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_PMIOA_REG40, AccessWidth8, 0xFB, 0);
  }

  if ( LocalCfgPtr->TurnOffXtalS3S5 ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C, AccessWidth32, ~(UINT32)(BIT20 + BIT21), (BIT20 + BIT21));
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C, AccessWidth32, ~(UINT32)(BIT20 + BIT21), 0);
  }

  if ( LocalCfgPtr->Xtal48MPadPowerSaving ) {
    ReadMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG5C, AccessWidth32, &Value32);
    if (!(Value32 & BIT22)) {
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG5C, AccessWidth32, ~(UINT32)(BIT22), BIT22);
      FchStall(1, NULL);
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00, AccessWidth32, ~(UINT32)(BIT18 + BIT17 + BIT16), BIT17);
      FchStall(1, NULL);
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00, AccessWidth32, \
                                                               ~(UINT32)(BIT18 + BIT17 + BIT16), (BIT17 + BIT16));
      FchStall(1, NULL);
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG5C, AccessWidth32, ~(UINT32)(BIT21 + BIT20 + BIT19), BIT21);
    }
  }

  //PLAT-80791
  // MISCx00000030 (FCH::MISC::cgpll_configuration0)
  //[1] usb_phy_cmlclk_zstate_dis = 1
  //[8] usb_phy_cmlclk_s3_dis = 1
  //[9] usb_phy_cmlclk_s0i3_dis = 1
  //[10] usb_phy_cmlclk_s5_dis = 1
  RwMem (
    ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG30_CGPLL_CFG0,
    AccessWidth32,
    ~(UINT32)(BIT10 + BIT9 + BIT8 + BIT1),
    (BIT10 + BIT9 + BIT8 + BIT1)
  );

  ///@todo, no such message in SMU V13
  // FchKTXhciSmuService (0, BIOSSMC_MSG_PowerGateXgbe); //PLAT-48973

  if(LocalCfgPtr->I2cI3cEarlyInit == TRUE) {
    FchInitEnableI2cI3c (LocalCfgPtr);
  }
}



