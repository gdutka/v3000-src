/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTHWACPI_KTHWACPILATESERVICE_FILECODE

/**
 * GcpuRelatedSetting - Program Gcpu C related function
 *
 *
 *
 * @param[in] FchDataPtr   Fch configuration structure pointer.
 *
 */
VOID
GcpuRelatedSetting (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        FchAcDcMsg;
  UINT8        FchTimerTickTrack;
  UINT8        FchClockInterruptTag;
  UINT8        FchOhciTrafficHanding;
  UINT8        FchEhciTrafficHanding;
  UINT8        FchGcpuMsgCMultiCore;
  UINT8        FchGcpuMsgCStage;
  UINT32       Value;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  FchAcDcMsg = (UINT8) LocalCfgPtr->Gcpu.AcDcMsg;
  FchTimerTickTrack = (UINT8) LocalCfgPtr->Gcpu.TimerTickTrack;
  FchClockInterruptTag = (UINT8) LocalCfgPtr->Gcpu.ClockInterruptTag;
  FchOhciTrafficHanding = (UINT8) LocalCfgPtr->Gcpu.OhciTrafficHanding;
  FchEhciTrafficHanding = (UINT8) LocalCfgPtr->Gcpu.EhciTrafficHanding;
  FchGcpuMsgCMultiCore = (UINT8) LocalCfgPtr->Gcpu.GcpuMsgCMultiCore;
  FchGcpuMsgCStage = (UINT8) LocalCfgPtr->Gcpu.GcpuMsgCStage;

  ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA0, AccessWidth32, &Value);
  Value = Value & 0xC07F00A0;

  if ( FchAcDcMsg ) {
    Value = Value | BIT0;
  }

  if ( FchTimerTickTrack ) {
    Value = Value | BIT1;
  }

  if ( FchClockInterruptTag ) {
    Value = Value | BIT10;
  }

  if ( FchOhciTrafficHanding ) {
    Value = Value | BIT13;
  }

  if ( FchEhciTrafficHanding ) {
    Value = Value | BIT15;
  }

  if ( FchGcpuMsgCMultiCore ) {
    Value = Value | BIT23;
  }

  if ( FchGcpuMsgCStage ) {
    Value = (Value | (BIT6 + BIT4 + BIT3 + BIT2));
  }

  WriteMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA0, AccessWidth32, &Value);
}

/**
 * MtC1eEnable - Program Mt C1E Enable Function
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
MtC1eEnable (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  if ( LocalCfgPtr->HwAcpi.MtC1eEnable ) {
  }
}

/**
 * StressResetModeLate - Stress Reset Mode
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
StressResetModeLate (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                  ResetValue;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  switch ( LocalCfgPtr->HwAcpi.StressResetMode ) {
  case 0:
    return;
  case 1:
    ResetValue = FCH_KBC_RESET_COMMAND;
    LibAmdIoWrite (AccessWidth8, FCH_KBDRST_BASE_IO, &ResetValue, StdHeader);
    break;
  case 2:
    ResetValue = FCH_PCI_RESET_COMMAND06;
    LibAmdIoWrite (AccessWidth8, FCH_PCIRST_BASE_IO, &ResetValue, StdHeader);
    break;
  case 3:
    ResetValue = FCH_PCI_RESET_COMMAND0E;
    LibAmdIoWrite (AccessWidth8, FCH_PCIRST_BASE_IO, &ResetValue, StdHeader);
    break;
  case 4:
    LocalCfgPtr->HwAcpi.StressResetMode = 3;
    return;
  default:
    //ASSERT (FALSE);
    return;
  }
  while (LocalCfgPtr->HwAcpi.StressResetMode) {
  }
}

/**
 * FchEventInitUsbGpe - Init Gevent that mapped to Usb Pme
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
FchEventInitUsbGpe (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                  Usb0GpeNum, Usb1GpeNum, Usb2GpeNum, Usb3GpeNum, Usb4GpeNum;
  UINT32                 SciLevl32;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  Usb0GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_Xhc0Wake);
  Usb1GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_Xhc1Wake);
  Usb2GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_Xhc2Wake);

  if (FchCheckRmbB0()) {
    Usb3GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_Xhc3Wake);
    Usb4GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_Xhc4Wake);
  }

  SciLevl32 = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C);

  if (Usb0GpeNum != 0) {
    Usb0GpeNum &= 0x1F;
    SciLevl32 |= (UINT32) (1 << Usb0GpeNum);
  }

  if (Usb1GpeNum != 0) {
    Usb1GpeNum &= 0x1F;
    SciLevl32 |= (UINT32) (1 << Usb1GpeNum);
  }

  if (Usb2GpeNum != 0) {
    Usb2GpeNum &= 0x1F;
    SciLevl32 |= (UINT32) (1 << Usb2GpeNum);
  }

  if (FchCheckRmbB0()) {
    if (Usb3GpeNum != 0) {
      Usb3GpeNum &= 0x1F;
      SciLevl32 |= (UINT32) (1 << Usb3GpeNum);
    }

    if (Usb4GpeNum != 0) {
      Usb4GpeNum &= 0x1F;
      SciLevl32 |= (UINT32) (1 << Usb4GpeNum);
    }
  }

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C) = SciLevl32;
}


/**
 * FchEventInitCioGpe - Init Gevent that mapped to Cio Pme
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
FchEventInitCioGpe (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                  Cio0GpeNum, Cio1GpeNum;
  UINT32                 SciLevl32;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  Cio0GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_CioFchPme0);
  Cio1GpeNum = ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_KT_CioFchPme0);

  SciLevl32 = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C);

  if (Cio0GpeNum != 0) {
    Cio0GpeNum &= 0x1F;
    SciLevl32 |= (UINT32) (1 << Cio0GpeNum);
  }

  if (Cio1GpeNum != 0) {
    Cio1GpeNum &= 0x1F;
    SciLevl32 |= (UINT32) (1 << Cio1GpeNum);
  }

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C) = SciLevl32;
}

/**
 * FchI2cUartInitLate - Config Fch AMBA I2C Uart init Late
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchI2cUartInitLate (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
}


