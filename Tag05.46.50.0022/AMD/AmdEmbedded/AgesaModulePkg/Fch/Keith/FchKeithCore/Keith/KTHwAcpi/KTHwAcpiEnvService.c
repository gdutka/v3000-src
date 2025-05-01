/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

#include "FchPlatform.h"
#include "Filecode.h"

#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTHWACPI_KTHWACPIENVSERVICE_FILECODE

#define AMD_CPUID_APICID_LPC_BID    0x00000001ul  // Local APIC ID, Logical Processor Count, Brand ID

ACPI_REG_WRITE FchKeithInitEnvSpecificHwAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},
#ifdef ACPI_SLEEP_TRAP
  {SMI_BASE  >> 8,  FCH_SMI_REGB0, 0xF3, BIT2},          // Set SLP_TYPE as SMI event
  {PMIO_BASE >> 8,  FCH_PMIOA_REGBE, 0xDF, 0x00},        // Disabled SLP function for S1/S3/S4/S5
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08 + 3, 0xFC, BIT1},    // Set S state transition disabled (BIT0)
                                                         //   force ACPI to send SMI message when writing to
                                                         //   SLP_TYP Acpi register. (BIT1)
  {SMI_BASE  >> 8,  FCH_SMI_REG98 + 3, 0x7F, 0x00},      // Enabled Global Smi ( BIT7 clear as 0 to enable )
#endif
  {0xFF, 0xFF, 0xFF, 0xFF},
};


/**
 * FchInitEnvHwAcpiMmioTable - Fch ACPI MMIO initial
 * during POST.
 *
 */
ACPI_REG_WRITE FchKeithInitEnvHwAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  {PMIO_BASE >> 8, FCH_PMIOA_REG74, 0x00, BIT0 + BIT1 + BIT2 + BIT4},
  {PMIO_BASE >> 8, FCH_PMIOA_REG74 + 2, 0x00, BIT1 + BIT2},             //en_shutdown_msg, en_sync_flood
  {PMIO_BASE >> 8, FCH_PMIOA_REG74 + 3, 0xDF, 0},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBA, 0xF7, BIT3},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBC, 0xFD, BIT1},

  {SMI_BASE >> 8, FCH_SMI_Gevent1, 0, 1},
  {SMI_BASE >> 8, FCH_SMI_Gevent3, 0, 3},
  {SMI_BASE >> 8, FCH_SMI_Gevent4, 0, 4},
  {SMI_BASE >> 8, FCH_SMI_Gevent5, 0, 5},
  {SMI_BASE >> 8, FCH_SMI_Gevent6, 0, 6},
  {SMI_BASE >> 8, FCH_SMI_Gevent23, 0, 23},
  {SMI_BASE >> 8, FCH_SMI_Gevent8, 0, 24},
  {SMI_BASE >> 8, FCH_SMI_REG08,  0xE7, 0},
  {SMI_BASE >> 8, FCH_SMI_REG0C + 2, 0xF7, BIT3},
  {SMI_BASE >> 8, FCH_SMI_KT_TWRN, 0, 9},
  {SMI_BASE >> 8, FCH_SMI_KT_Xhc0Wake, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_KT_Xhc1Wake, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_KT_Xhc2Wake, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_KT_FanThGevent, 0, 13},
  {SMI_BASE >> 8, FCH_SMI_KT_NbGppPme, 0, 16},
  {SMI_BASE >> 8, FCH_SMI_KT_NbGppHp, 0, 17},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

ACPI_REG_WRITE FchKeithInitEnvHwAcpiMmioTable2[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  {SMI_BASE >> 8, FCH_SMI_KT_CioFchPme0, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_KT_CioFchPme1, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_KT_Xhc3Wake, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_KT_Xhc4Wake, 0, 11},

  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchKeithInitEnvHwAcpiPciTable - PCI device registers initial
 * during early POST.
 *
 */
REG8_MASK FchKeithInitEnvHwAcpiPciTable[] =
{
  //
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  //
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {FCH_CFG_REG10, 0X00, (FCH_VERSION_KEITH & 0xFF)},                ///Program the version information
  {FCH_CFG_REG11, 0X00, (FCH_VERSION_KEITH >> 8)},
  {0xFF, 0xFF, 0xFF},
};

/**
 * FchKeithInitEnvRasPciTable
 *
 */
REG8_MASK FchKeithInitEnvRasPciTable[] =
{
  {0x00, SMBUS_BUS_DEV_FUN, 0},           // SMBUS Device (Bus 0, Dev 20, Func 0)
  {FCH_CFG_REG04, 0xBF, BIT6},            // Command Register bit6
  {FCH_CFG_REG04 + 1, 0xFE, BIT0},        // Command Register bit8
  {0xFF, 0xFF, 0xFF},
  {0x00, LPC_BUS_DEV_FUN, 0},             // LPC Device (Bus 0, Dev 20, Func 3)
  {FCH_CFG_REG04, 0xBF, BIT6},            // Command Register bit6
  {FCH_CFG_REG04 + 1, 0xFE, BIT0},        // Command Register bit8
  {0xFF, 0xFF, 0xFF},
};

ACPI_REG_WRITE FchKeithInitEnvRasMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {SMI_BASE  >> 8,  FCH_SMI_REGB4 + 1, 0xFC, BIT0},         // Enable SERR SMI
  {0xFF, 0xFF, 0xFF, 0xFF},
};

///
/// PCI_IRQ_REG_BLOCK- FCH PCI IRQ registers block
///
typedef struct _PCI_IRQ_REG_BLOCK {
  UINT8                PciIrqIndex;       // PciIrqIndex - selects which PCI interrupt to map
  UINT8                PciIrqData;        // PciIrqData  - Interrupt #
} PCI_IRQ_REG_BLOCK;

STATIC PCI_IRQ_REG_BLOCK FchInternalDeviceIrqForApicMode[] = {
    { (FCH_KT_IRQ_INTA | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_KT_IRQ_INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_KT_IRQ_INTC | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_KT_IRQ_INTD | FCH_IRQ_IOAPIC), 0x13},
    { (FCH_KT_IRQ_SD | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_KT_IRQ_SDIO | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_KT_IRQ_EMMC | FCH_IRQ_IOAPIC), 0x5},
    { (FCH_KT_IRQ_GPIO | FCH_IRQ_IOAPIC), 0x7},
    { (FCH_KT_IRQ_UART0 | FCH_IRQ_IOAPIC), 0x3},
    { (FCH_KT_IRQ_UART1 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_KT_IRQ_UART2 | FCH_IRQ_IOAPIC), 0x3},
    { (FCH_KT_IRQ_UART3 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_KT_IRQ_UART4 | FCH_IRQ_IOAPIC), 0xf},
    { (FCH_KT_IRQ_I2C0 | FCH_IRQ_IOAPIC), 0xa},
    { (FCH_KT_IRQ_I2C1 | FCH_IRQ_IOAPIC), 0xb},
    { (FCH_KT_IRQ_I2C2 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_KT_IRQ_I2C3 | FCH_IRQ_IOAPIC), 0x6},
    { (FCH_KT_IRQ_I2C4 | FCH_IRQ_IOAPIC), 0xe},
  };

STATIC PCI_IRQ_REG_BLOCK FchInternalDeviceIrqForVWMode[] = {
    { (FCH_KT_IRQ_INTA | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_KT_IRQ_INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_KT_IRQ_INTC | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_KT_IRQ_INTD | FCH_IRQ_IOAPIC), 0x13},
    { (FCH_KT_IRQ_SD | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_KT_IRQ_SDIO | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_KT_IRQ_EMMC | FCH_IRQ_IOAPIC), 0x5},
    { (FCH_KT_IRQ_GPIO | FCH_IRQ_IOAPIC), 0x7},
    { (FCH_KT_IRQ_UART0 | FCH_IRQ_IOAPIC), 0x3},
    { (FCH_KT_IRQ_UART1 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_KT_IRQ_UART2 | FCH_IRQ_IOAPIC), 0x3},
    { (FCH_KT_IRQ_UART3 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_KT_IRQ_UART4 | FCH_IRQ_IOAPIC), 0xf},
    { (FCH_KT_IRQ_I2C0 | FCH_IRQ_IOAPIC), 0xa},
    { (FCH_KT_IRQ_I2C1 | FCH_IRQ_IOAPIC), 0xb},
    { (FCH_KT_IRQ_I2C2 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_KT_IRQ_I2C3 | FCH_IRQ_IOAPIC), 0x6},
    { (FCH_KT_IRQ_I2C4 | FCH_IRQ_IOAPIC), 0xe},
  };
#define NUM_OF_DEVICE_FOR_APICIRQ  sizeof (FchInternalDeviceIrqForApicMode) / sizeof (PCI_IRQ_REG_BLOCK)
#define VW_SRCACTIVELOW  0xFFCEF8ul

VOID
ClearThermalTripSts (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  if (LocalCfgPtr->HwAcpi.NoClearThermalTripSts == FALSE) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC0, AccessWidth8, 0, BIT0);
  }
}

/**
 * ProgramEnvPFchAcpiMmio - Config HwAcpi MMIO registers
 *   Acpi S3 resume won't execute this procedure (POST only)
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramEnvPFchAcpiMmio (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKeithInitEnvHwAcpiMmioTable[0]), StdHeader);

  if (FchCheckRmbB0()) {
    ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKeithInitEnvHwAcpiMmioTable2[0]), StdHeader);
  }

  ClearThermalTripSts (FchDataPtr);
}

/**
 * ProgramFchEnvHwAcpiPciReg - Config HwAcpi PCI controller
 * before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvHwAcpiPciReg (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  //Early post initialization of pci config space
  //
  ProgramPciByteTable ((REG8_MASK*) (&FchKeithInitEnvHwAcpiPciTable[0]),
                        sizeof (FchKeithInitEnvHwAcpiPciTable) / sizeof (REG8_MASK),
                        StdHeader
                        );

  if ( LocalCfgPtr->Smbus.SmbusSsid != NULL ) {
    RwPci ((SMBUS_BUS_DEV_FUN << 16) + FCH_CFG_REG2C, AccessWidth32, 0x00, LocalCfgPtr->Smbus.SmbusSsid, StdHeader);
  }
  if ( LocalCfgPtr->Misc.NoneSioKbcSupport ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED, AccessWidth8, ~(UINT32) (BIT1), BIT1);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED, AccessWidth8, ~(UINT32) (BIT1), 0);
  }
  ProgramPcieNativeMode (FchDataPtr);
}

/**
 * FchVgaInit - Config VGA CODEC
 *
 * @param[in] VOID empty
 *
 */
VOID
FchVgaInit (
  OUT VOID
  )
{
}

/**
 * ProgramSpecificFchInitEnvAcpiMmio - Config HwAcpi MMIO before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramSpecificFchInitEnvAcpiMmio (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKeithInitEnvSpecificHwAcpiMmioTable[0]), StdHeader);

  //
  // LpcClk0DrivenZero & RTC Wake Mode Control
  //
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG6C + 2, AccessWidth8, 0x5F, 0xA0);
  //
  // Ac Loss Control
  //
  AcLossControl ((UINT8) LocalCfgPtr->HwAcpi.PwrFailShadow);
  //
  // FCH VGA Init
  //
  FchVgaInit ();

  //
  // Set ACPIMMIO by OEM Input table
  //
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE *) (LocalCfgPtr->HwAcpi.OemProgrammingTablePtr), StdHeader);
}

/**
 * ValidateFchVariant - Validate FCH Variant
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
ValidateFchVariant (
  IN  VOID     *FchDataPtr
  )
{
}

/**
 * IsExternalClockMode - Is External Clock Mode?
 *
 *
 * @retval  TRUE or FALSE
 *
 */
BOOLEAN
IsExternalClockMode (
  IN  VOID     *FchDataPtr
  )
{
  UINT8    MISC80;
  ReadMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80 + 2, AccessWidth8, &MISC80);
  return ( (BOOLEAN) ((MISC80 & BIT1) == 0) );
}


/**
 * ProgramFchEnvSpreadSpectrum - Config SpreadSpectrum before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvSpreadSpectrum (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        FchSpreadSpectrum;

  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8        WarmReset;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  FchSpreadSpectrum = LocalCfgPtr->HwAcpi.SpreadSpectrum;
  WarmReset = 0x06;

//Software enable SSC by programming
//1. FCH::MISC::cgpllconfig1[cg1_spread_spectrum_enable] = 1
//2. FCH::MISC::cgpllconfig3[cg1pll_fracn_en_override] = 1
//3. FCH::MISC::miscclkcntrl0[cg1_cfg_update_req] = 1
//4. FCH::IO::IO_PORT_CF9 [7:0] = 0x6

//CGPLL SSC is default disabled.
//If SSC got enabled first and then decided to disable it afterward, below is SSC disable sequence. Programming
//1. FCH::MISC::cgpllconfig1[cg1_spread_spectrum_enable] = 0
//2. FCH::MISC::miscclkcntrl0[cg1_cfg_update_req] = 1
//3. FCH::IO::IO_PORT_CF9 [7:0] = 0x6

  if (( FchSpreadSpectrum ) && ((ACPIMMIO32 (ACPI_MMIO_BASE + MISC_BASE + 0x08) & BIT0) == 0)) {
    //
    //For SSC enable:
    //  Misc_ Reg x08[0]=1 (CORE_CGPLL_ssc_en)
    //  Misc_ Reg x10[29]=1 (CORE_CGPLL_fracn_en)
    //  Misc_ Reg x40[30]=1 (cfg_update_req)
    //  warm reset (IO xCF9 = x06)
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth32, ~(UINT32) BIT0, BIT0);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x10, AccessWidth32, ~(UINT32) BIT29, BIT29);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x40, AccessWidth32, ~(UINT32) BIT30, BIT30);

    IDS_HDT_CONSOLE (FCH_TRACE, "  ProgramFchEnvSpreadSpectrum doing Warm Reset...\n");
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCF9, &WarmReset, StdHeader);
    //
    // Given we should have reset getting here would be bad
    //
    FCH_DEADLOOP ();
  } else if (( !FchSpreadSpectrum ) && (ACPIMMIO32 (ACPI_MMIO_BASE + MISC_BASE + 0x08) & BIT0)){
    //
    //For SSC disable:
    //  Misc_ Reg x08[0]=0 (CORE_CGPLL_ssc_en)
    //  Misc_ Reg x40[30]=1 (cfg_update_req)
    //  warm reset (IO xCF9 = x06)
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth32, ~(UINT32) BIT0, 0x00);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x40, AccessWidth32, ~(UINT32) BIT30, BIT30);

    IDS_HDT_CONSOLE (FCH_TRACE, "  ProgramFchEnvSpreadSpectrum Disable SSC doing Warm Reset...\n");
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCF9, &WarmReset, StdHeader);
    //
    // Given we should have reset getting here would be bad
    //
    FCH_DEADLOOP ();
  }
}

/**
 * TurnOffCG2
 *
 *
 * @retval  VOID
 *
 */
VOID
TurnOffCG2 (
  OUT VOID
  )
{
}

/**
 * BackUpCG2
 *
 *
 * @retval  VOID
 *
 */
VOID
BackUpCG2 (
  OUT VOID
  )
{
}

/**
 * HpetInit - Program Fch HPET function
 *
 *
 *
 * @param[in] FchDataPtr         Fch configuration structure pointer.
 *
 */
VOID
HpetInit (
  IN  VOID     *FchDataPtr
  )
{
  DESCRIPTION_HEADER   *HpetTable;
  BOOLEAN              FchHpetTimer;
  BOOLEAN              FchHpetMsiDis;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  FchHpetTimer = LocalCfgPtr->Hpet.HpetEnable;
  FchHpetMsiDis = LocalCfgPtr->Hpet.HpetMsiDis;

  HpetTable = NULL;
  if ( FchHpetTimer == TRUE ) {
    //
    //Enabling decoding of HPET MMIOProgram the HPET BAR address
    //
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth8, 0xFF, BIT6);

    //
    //Enable HPET MSI support
    //
    if ( FchHpetMsiDis == FALSE ) {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) BIT29, BIT29);
    } else {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) BIT29, 0);
    }
  }
}

/**
 * ProgramPcieNativeMode - Config Pcie Native Mode
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramPcieNativeMode (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        FchNativepciesupport;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  FchNativepciesupport = (UINT8) LocalCfgPtr->Misc.NativePcieSupport;

  //
  // PCIE Native setting
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBA + 1, AccessWidth8, ~(UINT32) BIT6, 0);
  if ( FchNativepciesupport == 1) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74 + 3, AccessWidth8, ~(UINT32) (BIT3 + BIT1 + BIT0), BIT3 + BIT0);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74 + 3, AccessWidth8, ~(UINT32) (BIT3 + BIT1 + BIT0), BIT3);
  }
}

/**
 * PciIntVwInit - Config Pci Interrupt routing for VW mode
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
PciIntVwInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8                  i;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  for (i = 0; i < NUM_OF_DEVICE_FOR_APICIRQ; i++) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &FchInternalDeviceIrqForVWMode[i].PciIrqIndex, StdHeader);
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &FchInternalDeviceIrqForVWMode[i].PciIrqData, StdHeader);
  }

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA8, AccessWidth32, 0, BIT31 + VW_SRCACTIVELOW );

}

VOID
FchModifyDeviceIrq (
    IN  VOID     *FchDataPtr
    )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  UINT8                  i;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  for (i = 0; i < NUM_OF_DEVICE_FOR_APICIRQ; i++) {
    switch (FchInternalDeviceIrqForApicMode[i].PciIrqIndex){
      case (FCH_KT_IRQ_UART0 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart0Irq;
        break;
      case (FCH_KT_IRQ_UART1 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart1Irq;
        break;
      case (FCH_KT_IRQ_UART2 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart2Irq;
        break;
      case (FCH_KT_IRQ_UART3 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart3Irq;
        break;
      case (FCH_KT_IRQ_UART4 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart4Irq;
        break;
      case (FCH_KT_IRQ_I2C0 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c0Irq;
        break;
      case (FCH_KT_IRQ_I2C1 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c1Irq;
        break;
      case (FCH_KT_IRQ_I2C2 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c2Irq;
        break;
      case (FCH_KT_IRQ_I2C3 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c3Irq;
        break;
      case (FCH_KT_IRQ_I2C4 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c4Irq;
        break;
    }
  }
}



/**
 * FchInternalDeviceIrqInit - Config Fch internal Device Interrupt routing
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInternalDeviceIrqInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8                  i;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchModifyDeviceIrq (FchDataPtr);

  for (i = 0; i < NUM_OF_DEVICE_FOR_APICIRQ; i++) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &FchInternalDeviceIrqForApicMode[i].PciIrqIndex, StdHeader);
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &FchInternalDeviceIrqForApicMode[i].PciIrqData, StdHeader);
  }
}

/**
 * FchAl2ahbInit - Config Fch AL2AHB init
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchAl2ahbInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // Enable Clock Gating
  //
  RwMem (FCH_AL2AHB_BASE + 0x10, AccessWidth8, 0xFF, BIT1);
  RwMem (FCH_AL2AHB_BASE + 0x30, AccessWidth8, 0xFF, BIT1);
}
/**
 * FchI2cUartInit - Config Fch AMBA I2C Uart init
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchI2cUartInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT32                 FchDeviceEnMap;
  UINT8                  Value8;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  FchDeviceEnMap = LocalCfgPtr->FchRunTime.FchDeviceEnableMap;

  if (LocalCfgPtr->FchResetDataBlock.I2cI3cEarlyInit == FALSE) {
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

  //
  // UART0
  //
  if ( FchDeviceEnMap & BIT11 ) {
    FchAoacPowerOnDev (FCH_AOAC_UART0, 1);
    // IOMUX
    //EGPIO140/UART0_CTS_L/UART1_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8C, AccessWidth8, 0, 0x1);
    //EGPIO141/UART0_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8D, AccessWidth8, 0, 0x1);
    //EGPIO142/UART0_RTS_L/UART1_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8E, AccessWidth8, 0, 0x1);
    //EGPIO143/UART0_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8F, AccessWidth8, 0, 0x1);
    //AGPIO144/SHUTDOWN_L/UART0_INTR
//    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x90, AccessWidth8, 0, 0x2);

    // When UART Input is selected (UART_CTS_L, UART_RXD, UART_RTS_N, UART_INTR),
    // corresponding GPIOOutEn bit need be set to '0'
    // EGPIO140/UART0_CTS_L/UART1_TXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x30, AccessWidth32, ~(UINT32)(BIT23), 0);
    // EGPIO141/UART0_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x34, AccessWidth32, ~(UINT32)(BIT23), 0);
    // EGPIO142/UART0_RTS_L/UART1_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x38, AccessWidth32, ~(UINT32)(BIT23), 0);
    // AGPIO144/SHUTDOWN_L/UART0_INTR
//    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x40, AccessWidth32, ~(UINT32)(BIT23), 0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_UART0, 0);
  }

  //
  // UART1
  //
  if ( FchDeviceEnMap & BIT12 ) {
    FchAoacPowerOnDev (FCH_AOAC_UART1, 1);
    // IOMUX
    //EGPIO140/UART0_CTS_L/UART1_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8C, AccessWidth8, 0, 0x2);
    //EGPIO142/UART0_RTS_L/UART1_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8E, AccessWidth8, 0, 0x2);

    // When UART Input is selected (UART_CTS_L, UART_RXD, UART_RTS_N, UART_INTR),
    // corresponding GPIOOutEn bit need be set to '0'
    //EGPIO142/UART0_RTS_L/UART1_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x38, AccessWidth32, ~(UINT32)(BIT23), 0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_UART1, 0);
  }

  //
  // UART2
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "UART2 enable bit value is 0x%x.\n", FchDeviceEnMap & BIT16);
  if ( FchDeviceEnMap & BIT16 ) {
    FchAoacPowerOnDev (FCH_AOAC_UART2, 1);
    // IOMUX
    //EGPIO135/UART2_CTS_L/UART3_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x87, AccessWidth8, 0, 0x1);
    //EGPIO136/UART2_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x88, AccessWidth8, 0, 0x1);
    //EGPIO137/UART2_RTS_L/UART3_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x89, AccessWidth8, 0, 0x1);
    //EGPIO138/UART2_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8A, AccessWidth8, 0, 0x1);
    //AGPIO139/UART2_INTR
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x8B, AccessWidth8, 0, 0x1);

    // When UART Input is selected (UART_CTS_L, UART_RXD, UART_RTS_N, UART_INTR),
    // corresponding GPIOOutEn bit need be set to '0'
    //EGPIO135/UART2_CTS_L/UART3_TXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x1C, AccessWidth32, ~(UINT32)(BIT23), 0);
    //EGPIO136/UART2_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x20, AccessWidth32, ~(UINT32)(BIT23), 0);
    //EGPIO137/UART2_RTS_L/UART3_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x24, AccessWidth32, ~(UINT32)(BIT23), 0);
    //AGPIO139/UART2_INTR
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x2C, AccessWidth32, ~(UINT32)(BIT23), 0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_UART2, 0);
  }

  //
  // UART3
  //
  if ( FchDeviceEnMap & BIT26 ) {
    FchAoacPowerOnDev (FCH_AOAC_UART3, 1);
    // IOMUX
    //EGPIO135/UART2_CTS_L/UART3_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x87, AccessWidth8, 0, 0x2);
    //EGPIO137/UART2_RTS_L/UART3_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x89, AccessWidth8, 0, 0x2);

    // When UART Input is selected (UART_CTS_L, UART_RXD, UART_RTS_N, UART_INTR),
    // corresponding GPIOOutEn bit need be set to '0'
    // EGPIO137/UART2_RTS_L/UART3_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x24, AccessWidth32, ~(UINT32)(BIT23), 0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_UART3, 0);
  }

  //
  // UART4
  //
  if ( FchDeviceEnMap & BIT20 ) {
    FchAoacPowerOnDev (FCH_AOAC_UART4, 1);
    // IOMUX
    // EGPIO153/UART4_CTS_L
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x99, AccessWidth8, 0, 0x1);
    // EGPIO154/UART4_RTS_L
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x9A, AccessWidth8, 0, 0x1);
    // EGPIO155/UART4_RXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x9B, AccessWidth8, 0, 0x1);
    // EGPIO156/UART4_TXD
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x9C, AccessWidth8, 0, 0x1);
    // AGPIO157/UART4_INTR
//    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x9D, AccessWidth8, 0, 0x1);

    // When UART Input is selected (UART_CTS_L, UART_RXD, UART_RTS_N, UART_INTR),
    // corresponding GPIOOutEn bit need be set to '0'
    // EGPIO153/UART4_CTS_L
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x64, AccessWidth32, ~(UINT32)(BIT23), 0);
    // EGPIO154/UART4_RTS_L
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x68, AccessWidth32, ~(UINT32)(BIT23), 0);
    // EGPIO155/UART4_RXD
    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x6C, AccessWidth32, ~(UINT32)(BIT23), 0);
    // AGPIO157/UART4_INTR
//    RwMem (ACPI_MMIO_BASE + GPIO_BANK2_BASE + 0x74, AccessWidth32, ~(UINT32)(BIT23), 0);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_UART4, 0);
  }

  // PSP_INTR0
  //
  // IOMUX
  //PLAT-16166: Enable ExpressModule hotplug RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x59, AccessWidth8, 0, 0x1);

  // UART Legacy IO Enable Support
  if ( LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable ) {
    // Check to disalbe eSPI 3F8 decode to avoid hard hang when UART 3F8 is eanbled.
    if ( LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable & BIT3 ) {
      if ( ACPIMMIO16 (0xFEC20044) == 0x3f8) {
        ACPIMMIO32 (0xFEC20040) &= ~ (UINT32) BIT8; //Disable eSPI 3F8 decoding at Range0
      }
    }
    //EMBDEV-21056_start
    if ( LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable & BIT1 ) {
      if ( ACPIMMIO16 (0xFEC20048) == 0x2f8) {
        ACPIMMIO32 (0xFEC20040) &= ~ (UINT32) BIT10; //Disable eSPI 2f8 decoding at Range2
      }
    }
    if ( LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable & BIT2 ) {
      if ( ACPIMMIO16 (0xFEC2004A) == 0x3e8) {
        ACPIMMIO32 (0xFEC20040) &= ~ (UINT32) BIT11; //Disable eSPI 3e8 decoding at Range3
      }
    }
    if ( LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable & BIT0 ) {
      if ( ACPIMMIO16 (0xFEC20080) == 0x2e8) {
        ACPIMMIO32 (0xFEC20040) &= ~ (UINT32) BIT16; //Disable eSPI 2e8 decoding at Range4
      }
    }
    //EMBDEV-21056_End
    ACPIMMIO16 (FCH_AL2AHBx20_LEGACY_UART_IO_ENABLE) = LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable;
    ACPIMMIO32 (FCH_SMBUSPCI_HCLK_UARTCONTROLREG) |= FCH_SMBUSPCI_HCLK_UARTCONTROLREG_UART_1P843MCLK_EN;  //PLAT-50828
    // Need setup UART 3F8 baudrate 115200 here to make debug message working normally
    if ( LocalCfgPtr->FchRunTime.Al2AhbLegacyUartIoEnable & BIT3 ) {
      //Set DLAB
      Value8 = 0x80;
      LibAmdIoWrite (AccessWidth8, 0x3FB, &Value8, StdHeader);
      //DLL = 1;
      Value8 = 1;
      LibAmdIoWrite (AccessWidth8, 0x3F8, &Value8, StdHeader);
      //clear DLAB
      Value8 = 0x00;
      LibAmdIoWrite (AccessWidth8, 0x3FB, &Value8, StdHeader);
    }
  }
}

/**
 * FchAlinkRasEnable - Enable FCH A-Link parity error
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchAlinkRasEnable (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  if ( LocalCfgPtr->HwAcpi.FchAlinkRasSupport ) {
    RwAlink (FCH_ABCFG_REG10050 | (UINT32) (ABCFG << 29), ~(UINT32) BIT3, BIT3, StdHeader);
    ProgramPciByteTable ((REG8_MASK*) (&FchKeithInitEnvRasPciTable[0]),
                          sizeof (FchKeithInitEnvRasPciTable) / sizeof (REG8_MASK),
                          StdHeader
                          );
    ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKeithInitEnvRasMmioTable[0]), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG10050 | (UINT32) (ABCFG << 29), ~(UINT32) BIT3, 0, StdHeader);
  }
}

/**
 * ProgramFchEnvAoacInit - AOAC configuration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvAoacInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  if (LocalCfgPtr->HwAcpi.FchAoacProgramEnable) {
    //
    //Program AOAC shadow SRAM to allocate space for each of the controllers
    //
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x94, AccessWidth16, 0xFF0FFFFF, 0x00B00000);
    //AB
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x88, AccessWidth16, 0, 0x0001);
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x8C, AccessWidth32, 0, 0x39260080);

    //ACPISMBUS
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x88, AccessWidth16, 0, 0x0002);
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x8C, AccessWidth32, 0, 0x88440100);

    //LPC
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x88, AccessWidth16, 0, 0x0004);
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x8C, AccessWidth32, 0, 0x80540200);

    //ESPI
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x88, AccessWidth16, 0, 0x001B);
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x8C, AccessWidth32, 0, 0x201D0300);

    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x90, AccessWidth32, 0, 0x00000016);

    if ( ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (FCH_AOAC_ESPI << 1)) & BIT3  ) { //check if ESPI is on
      RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x90, AccessWidth32, ~ (UINT32) (1 << 27) , (UINT32) (1 << 27));
    }

    //Set these two bits to enable store/restore of HPET and ACPI PM timer automatically.
    RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x9C, AccessWidth32, 0, BIT0 + BIT1);

    //Start Shadow Timer
    //ShdwSysCtrl.ShdwSysCntRun = 1
    //ShdwSysCtrl.Cnt48M100Run = 1
    RwMem (ACPI_MMIO_BASE + 0x1100 + 0x10, AccessWidth32, 0, BIT0 + BIT3);
  }
}

/**
 * ProgramFchEnvHfpInit - Host Finger Print configuration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvHfpInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;


  if (LocalCfgPtr->HwAcpi.FchHfpEnable) {
    //
    //Enable ACPI HFP device
    //
    LocalCfgPtr->FchRunTime.FchDeviceEnableMap |= BIT29;
  }

  //
  //FWDEV-61984: Fix PSIRT ticket CVE-2023-20579
  //  Program Host Finger Print CS to SPI_CS3_L
  //  Clear bit spiaccessmacromen[22] and spihostaccessromen [23] to 0 to lock CS
  RwMem (FCH_HFP_BASE_ADDRESS + FCH_SPI_MMIO_REG1D,     AccessWidth8, 0xFC, 0x2);
  RwMem (FCH_HFP_BASE_ADDRESS + FCH_SPI_MMIO_REG00 + 2, AccessWidth8, 0x3F, 0x0);
}

/**
 * ProgramFchEnvHidInit - Host Hid configuration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvHidInit (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;


  if (LocalCfgPtr->FchRunTime.FchDeviceEnableMap & BIT30) {
    //
    //Program HID SpiReadMode
    //
    RwMem (FCH_HID_BASE_ADDRESS + FCH_SPI_MMIO_REG00, AccessWidth32, ~(UINT32) (BIT18 + BIT29 + BIT30),\
           ((LocalCfgPtr->FchRunTime.HidControl[0].SpiReadMode & 1) << 18) \
            + ((LocalCfgPtr->FchRunTime.HidControl[0].SpiReadMode & 6) << 28));
    //
    //Promgram HID Speed
    //
    RwMem (FCH_HID_BASE_ADDRESS + FCH_SPI_MMIO_REG20, AccessWidth32, 0x000FFFFF,\
           ((LocalCfgPtr->FchRunTime.HidControl[0].SpiSpeed & 0xf ) <<20) \
            + ((LocalCfgPtr->FchRunTime.HidControl[0].SpiSpeed & 0xf ) <<24)
             + ((LocalCfgPtr->FchRunTime.HidControl[0].SpiSpeed & 0xf ) <<28));
    //
    //Promgram HID spi_spd6 and spi_spd7
    //
    RwMem (FCH_HID_BASE_ADDRESS + FCH_SPI_MMIO_REG6C, AccessWidth16, 0xc0c0,\
           (LocalCfgPtr->FchRunTime.HidControl[0].Spi_spd6 & 0x3f) \
            + ((LocalCfgPtr->FchRunTime.HidControl[0].Spi_spd7 & 0x3f) << 8));

    //
    //Program FCH_HIDx00000150_HID_CONTROL[15] hid_int_route_to_host=1, HID Interrupts are routed to Host as LPC_SPI_INT.
    //
    RwMem (FCH_HID_BASE_ADDRESS + FCH_HIDx00000150_HID_CONTROL, AccessWidth32, ~(UINT32) (BIT15),BIT15);
  }
  if (LocalCfgPtr->FchRunTime.FchDeviceEnableMap & BIT31) {
    //
    //Program HID2 SpiReadMode
    //
    RwMem (FCH_HID2_BASE_ADDRESS + FCH_SPI_MMIO_REG00, AccessWidth32, ~(UINT32) (BIT18 + BIT29 + BIT30),\
           ((LocalCfgPtr->FchRunTime.HidControl[1].SpiReadMode & 1) << 18) \
            + ((LocalCfgPtr->FchRunTime.HidControl[1].SpiReadMode & 6) << 28));
    //
    //Promgram HID Speed
    //
    RwMem (FCH_HID2_BASE_ADDRESS + FCH_SPI_MMIO_REG20, AccessWidth32, 0x000FFFFF,\
           ((LocalCfgPtr->FchRunTime.HidControl[1].SpiSpeed & 0xf ) <<20) \
            + ((LocalCfgPtr->FchRunTime.HidControl[1].SpiSpeed & 0xf ) <<24)
             + ((LocalCfgPtr->FchRunTime.HidControl[1].SpiSpeed & 0xf ) <<28));
    //
    //Promgram HID2 spi_spd6 and spi_spd7
    //
    RwMem (FCH_HID2_BASE_ADDRESS + FCH_SPI_MMIO_REG6C, AccessWidth16, 0xc0c0,\
           (LocalCfgPtr->FchRunTime.HidControl[1].Spi_spd6 & 0x3f) \
            + ((LocalCfgPtr->FchRunTime.HidControl[1].Spi_spd7 & 0x3f) << 8));
    //
    //Program FCH_HIDx00000150_HID_CONTROL[15] hid_int_route_to_host=1, HID Interrupts are routed to Host as LPC_SPI_INT.
    //
    RwMem (FCH_HID2_BASE_ADDRESS + FCH_HIDx00000150_HID_CONTROL, AccessWidth32, ~(UINT32) (BIT15),BIT15);
  }
}

/**
 * ProgramFchEnvZstateTiming - ZState Timing configuration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvZstateTiming (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  if (!FchCheckRmbB0 ()) {
    //
    // REGISTER                      DEFAULT     ZSTATE RECOMMENDED VALUE (BIOS PROGRAMMED)  NOTES
    // NBPWRGOODTMR(PMx24)           0xA0        0x1
    // FCHPWRGOODTMR(PMx12)          0x150       0x14
    // PGPWRGOODTOALLENTMR(PMx14)    0x16        0x9
    // PGPWRGOODASSERTIONTMR(PMx16)  0x20        0x9
    // PWRGATETMR(PMx18)             0xA8648642  0xA8648642                                  No change from default
    // PLLRSTBTMR(PMx1C)             0x40        0x1
    // PLLLOCKTMR(PMx1E)             0x3C        0xF
    // PCIRSTBTMR(PMx20)             0x120       0x1F
    // CPURSTBTMR(PMx22)             0x19C       0x4FF
    // CPUPWRGOODTMR(PMx26)          0xE0        0xFF
    // PMx10[5]                      ??          0x0
    // PMx3C[31:24]                  ??          0x6
    // PMx48[31:24]                  ??          0x9
    // PMxA8[31:0]                   ??          0x00010001
    // PMxB8[21]                     ??          0x1
    // PMxBC[25]                     ??          0x1
    //
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG24, AccessWidth16, 0x00, 0x01);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG12, AccessWidth16, 0x00, 0x14);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG14, AccessWidth16, 0x00, 0x09);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG16, AccessWidth16, 0x00, 0x09);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG1C, AccessWidth16, 0x00, 0x01);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG1E, AccessWidth16, 0x00, 0x0F);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG20, AccessWidth16, 0x00, 0x1F);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG22, AccessWidth16, 0x00, 0x4FF);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG26, AccessWidth16, 0x00, 0xFF);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10, AccessWidth8, 0xDF, 0x00);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG3C + 3, AccessWidth8, 0x00, 0x06);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG48 + 3, AccessWidth8, 0x00, 0x09);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA8, AccessWidth32, 0x00, 0x00010001);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGB8, AccessWidth32, ~(UINT32) (BIT21), BIT21);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBC, AccessWidth32, ~(UINT32) (BIT25), BIT25);
  } else {
    //
    // PMx14[15:0] = 0x4810 (PGPwrGoodToAllEnTmr)
    // PMx14 [31:16] = 0x4808 (PGPwrGoodAssertionTmr)
    // PMx1C[15:0] = 0x0801 (PllRstBTmr)
    // PMx1C[31:16] = 0x1804 (PllLockTmr)
    // PMx20[15:0] = 0x3820 (PciRstTmr)
    // PMx24[15:0] = 0x08A0 (NBPwrGoodTmr)
    // PMx48[31:24] = 0x59 (PG1PwrDownDlyTmr)
    // PMx4C[7:0] = 0x09 (PG1PwrDownDlyTmr)
    // PMxA8[15:0] = 0x0885 (PG1PwrRstTmr)
    // PMxA8[31:16] = 0x0823 (OtherPwrRstTmr)
    // PMx10[5] = 0x0 (disable RTC clock check)
    // PMxB8[21]=0x1 (override short timer mode)
    // PMxBC[25]=0x0 (DO NOT set this bit)
    // PMx3C[31:24]=0x06 (timer tick=0.5us)
    // PMx3C[18] = 0x1 (secondary set enable)
    //
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG14, AccessWidth32, 0x00, 0x48084810);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG1C, AccessWidth32, 0x00, 0x18040801);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG20, AccessWidth16, 0x00, 0x3820);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG24, AccessWidth16, 0x00, 0x08A0);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG48 + 3, AccessWidth8, 0x00, 0x59);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG4C, AccessWidth8, 0x00, 0x09);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA8, AccessWidth32, 0x00, 0x08230885);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10, AccessWidth8, 0xDF, 0x00);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGB8, AccessWidth32, ~(UINT32) (BIT21), BIT21);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBC, AccessWidth32, ~(UINT32) (BIT25), 0x00);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG3C + 3, AccessWidth8, 0x00, 0x06);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG3C, AccessWidth32, ~(UINT32) (BIT18), BIT18);
  }

  //
  // AOAC x94 s0i3ctrl register setting for Zstate enablement
  //  Set bit[24] = 1 to enable FCH ZState.
  //  Set bit[23:20] = 0xB to optimize Z10 latency.
  //
  RwMem (ACPI_MMIO_BASE + AOAC_BASE + 0x94, AccessWidth32, 0xFE0FFFFF, 0x01B00000);
}

