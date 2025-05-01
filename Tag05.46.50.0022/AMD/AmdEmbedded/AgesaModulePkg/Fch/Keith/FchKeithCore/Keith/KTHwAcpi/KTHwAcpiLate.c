/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#include "FchAoacLinkListData.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTHWACPI_KTHWACPILATE_FILECODE

#define AMD_CPUID_APICID_LPC_BID    0x00000001ul  // Local APIC ID, Logical Processor Count, Brand ID

extern VOID  HpetInit                          (IN VOID  *FchDataPtr);
extern VOID  GcpuRelatedSetting              (IN VOID  *FchDataPtr);
extern VOID  StressResetModeLate               (IN VOID  *FchDataPtr);
VOID
FchInitLateHwAcpiP (
  IN  VOID     *FchDataPtr
  );

/**
 * FchInitLateHwAcpi - Prepare HwAcpi controller to boot to OS.
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateHwAcpi (
  IN  VOID     *FchDataPtr
  )
{
  //UINT32                 RegEax;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  AGESA_TESTPOINT (TpFchInitLateHwAcpi, NULL);

  GcpuRelatedSetting (LocalCfgPtr);

  if (LocalCfgPtr->Gpp.SerialDebugBusEnable == TRUE ) {
    RwMem (ACPI_MMIO_BASE + SERIAL_DEBUG_BASE +  FCH_SDB_REG00, AccessWidth8, 0xFF, 0x05);
  }

  StressResetModeLate (LocalCfgPtr);
  SbSleepTrapControl (TRUE);

  FchInternalDeviceIrqInit (FchDataPtr);

  FchEventInitUsbGpe (FchDataPtr);

  if (FchCheckRmbB0()) {
    FchEventInitCioGpe (FchDataPtr);
  }

  FchI2cUartInitLate (FchDataPtr);
}
VOID
FchInitLateHwAcpiP (
  IN  VOID     *FchDataPtr
  )
{
  UINT16                  FchSmiData;
  UINT8                   SwSmiCmdAddress;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  FchSmiData = (FCH_SMI_DATA_POST_LATE << 8) | FCH_AOAC_SW_VALUE;
  SwSmiCmdAddress = ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A);
  LibAmdIoWrite (AccessWidth16, SwSmiCmdAddress, &FchSmiData, LocalCfgPtr->StdHeader);
}

VOID
FchInitLateTriggerSmi (
  IN  VOID     *FchDataPtr
  )
{
  UINT16                  FchSmiData;
  UINT16                  SwSmiCmdAddress;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  if (LocalCfgPtr->Misc.S3Resume == FALSE) {
    FchSmiData = LocalCfgPtr->Misc.FchReadyToBootSmi;
    SwSmiCmdAddress = LocalCfgPtr->FchResetDataBlock.FchBldCfg.CfgSmiCmdPortAddr;
    LibAmdIoWrite (AccessWidth16, SwSmiCmdAddress, &FchSmiData, LocalCfgPtr->StdHeader);
  }
}

