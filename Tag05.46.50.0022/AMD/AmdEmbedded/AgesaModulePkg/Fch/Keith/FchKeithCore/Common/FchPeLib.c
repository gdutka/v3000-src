/*;********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/

#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_COMMON_FCHPELIB_FILECODE

/*----------------------------------------------------------------------------------------*/
/**
 * ProgramPciByteTable - Program PCI register by table (8 bits data)
 *
 *
 *
 * @param[in] pPciByteTable    - Table data pointer
 * @param[in] dwTableSize      - Table length
 * @param[in] StdHeader
 *
 */
VOID
ProgramPciByteTable (
  IN       REG8_MASK           *pPciByteTable,
  IN       UINT16              dwTableSize,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8     i;
  UINT8     dbBusNo;
  UINT8     dbDevFnNo;
  UINTN     PciAddress;

  dbBusNo = pPciByteTable->RegIndex;
  dbDevFnNo = pPciByteTable->AndMask;
  pPciByteTable++;

  for ( i = 1; i < dwTableSize; i++ ) {
    if ( (pPciByteTable->RegIndex == 0xFF) && (pPciByteTable->AndMask == 0xFF) && (pPciByteTable->OrMask == 0xFF) ) {
      pPciByteTable++;
      dbBusNo = pPciByteTable->RegIndex;
      dbDevFnNo = pPciByteTable->AndMask;
      pPciByteTable++;
      i++;
    } else {
      PciAddress = (dbBusNo << 20) + (dbDevFnNo << 12) + pPciByteTable->RegIndex;
      PciAndThenOr8 (PciAddress, pPciByteTable->AndMask, pPciByteTable->OrMask);
      pPciByteTable++;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * ProgramFchAcpiMmioTbl - Program FCH ACPI MMIO register by table (8 bits data)
 *
 *
 *
 * @param[in] pAcpiTbl   - Table data pointer
 * @param[in] StdHeader
 *
 */
VOID
ProgramFchAcpiMmioTbl (
  IN       ACPI_REG_WRITE      *pAcpiTbl,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8   i;
  UINT8   Or8;
  UINT8   Mask8;
  UINT32  ddtempVar;

  if (pAcpiTbl != NULL) {
    if ((pAcpiTbl->MmioReg == 0)
      && (pAcpiTbl->MmioBase == 0)
      && (pAcpiTbl->DataAndMask == 0xB0)
      && (pAcpiTbl->DataOrMask == 0xAC))
    {
      // Signature Checking
      pAcpiTbl++;
      for ( i = 1; pAcpiTbl->MmioBase < 0x1D; i++ ) {
        ddtempVar = ACPI_MMIO_BASE | (pAcpiTbl->MmioBase) << 8 | pAcpiTbl->MmioReg;
        Or8 = pAcpiTbl->DataOrMask;
        Mask8 = ~pAcpiTbl->DataAndMask;
        LibAmdMemRMW (AccessWidth8, (UINT64) ddtempVar, &Or8, &Mask8, StdHeader);
        pAcpiTbl++;
      }
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * ProgramFchSciMapTbl - Program FCH SCI Map table (8 bits data)
 *
 *
 *
 * @param[in] pSciMapTbl   - Table data pointer
 * @param[in] FchResetDataBlock
 *
 */
VOID
ProgramFchSciMapTbl (
  IN       SCI_MAP_CONTROL  *pSciMapTbl,
  IN       FCH_RESET_DATA_BLOCK *FchResetDataBlock
  )
{
  AMD_CONFIG_PARAMS   *StdHeader;

  UINT32  ddtempVar;
  StdHeader = FchResetDataBlock->StdHeader;

  if (pSciMapTbl != NULL) {
    while (pSciMapTbl->InputPin != 0xFF) {
      if ((pSciMapTbl->InputPin >= 0x40) && (pSciMapTbl->InputPin < 0x80) && (pSciMapTbl->GpeMap < 0x20)) {
        ddtempVar = ACPI_MMIO_BASE | SMI_BASE | pSciMapTbl->InputPin;
        LibAmdMemWrite (AccessWidth8, (UINT64) ddtempVar, &pSciMapTbl->GpeMap, StdHeader);
      } else {
        //Assert Warning "SCI map is invalid"
      }
      pSciMapTbl++;
    }
  }
}

/**
 * GetChipSysMode - Get Chip status
 *
 *
 * @param[in] Value - Return Chip strap status
 *   StrapStatus [15.0] - Hudson-2 chip Strap Status
 *    @li <b>0001</b> - Not USED FWH
 *    @li <b>0002</b> - Not USED LPC ROM
 *    @li <b>0004</b> - EC enabled
 *    @li <b>0008</b> - Reserved
 *    @li <b>0010</b> - Internal Clock mode
 * @param[in] StdHeader
 *
 */
VOID
GetChipSysMode (
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdMemRead (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80), Value, StdHeader);
}

/**
 * IsImcEnabled - Is IMC Enabled
 * @retval  TRUE for IMC Enabled; FALSE for IMC Disabled
 */
BOOLEAN
IsImcEnabled (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  return FALSE;
}


/**
 * GetEfuseStatue - Get Efuse status
 *
 *
 * @param[in] Value - Return Chip strap status
 * @param[in] StdHeader
 *
 */
VOID
GetEfuseStatus (
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    Or8;
  UINT8    Mask8;

  Or8 = BIT5;
  Mask8 = BIT5;
  LibAmdMemRMW (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8), &Or8, &Mask8, StdHeader);
  LibAmdMemWrite (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8), Value, StdHeader);
  LibAmdMemRead (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8 + 1), Value, StdHeader);
  Or8 = 0;
  Mask8 = BIT5;
  LibAmdMemRMW (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8), &Or8, &Mask8, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * SbSleepTrapControl - SB Sleep Trap Control
 *
 *
 *
 * @param[in] SleepTrap    - Whether sleep trap is enabled
 *
 */
VOID
SbSleepTrapControl (
  IN        BOOLEAN          SleepTrap
  )
{
  if (SleepTrap) {
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT2;

    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) &= ~ (BIT5);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT1;
  } else {
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) |= BIT5;
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT0;

    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3);
  }
}
/*----------------------------------------------------------------------------------------*/
/**
 * ProgramPMEDis - Program PMEDis
 *
 *
 *
 * @param[in] dbBusNo    - Die Bus Number
 * @param[in] PMEDis      - True to disable, False to enable
 *
 */
VOID
ProgramPMEDis (
  IN       UINT8     dbBusNo,
  IN       BOOLEAN   PMEDis
  )
{
  UINTN     PciAddress;

  PciAddress = (dbBusNo << 20) + 0x4C;
  if (PMEDis) {
    PciAndThenOr8 (PciAddress, 0xEF, 0x10);
  }  else {
    PciAndThenOr8 (PciAddress, 0xEF, 0x00);
  }
}

BOOLEAN
FchCheckRmbA0 (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;
  UINT8 Stepping;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x4) && (Model == 0x0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
FchCheckRmbB0 (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;
  UINT8 Stepping;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x4) && (Model == 0x4)) {
    return TRUE;
  } else {
    return FALSE;
  }
}
