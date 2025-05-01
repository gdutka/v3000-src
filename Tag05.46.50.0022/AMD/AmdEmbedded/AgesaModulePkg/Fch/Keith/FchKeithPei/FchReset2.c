/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchReset2.h"
#include "Library/AmdBaseLib.h"
#include "FchRegistersCommon.h"

#define FILECODE FCH_KEITH_FCHKEITHPEI_FCHRESET2_FILECODE

extern EFI_GUID gEfiAmdAgesaSpecificWarmResetGuid;

UINT16
ReadPmio16 (
  IN       UINT8        Index
  )
{
  UINT16    Temp16;

  Temp16 = 0;

  LibAmdMemRead (
    AccessWidth16,
    (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + (UINT64)Index),
    &Temp16,
    NULL
    );

  return Temp16;
}

VOID SpecificWarmResetSystem (
    IN EFI_RESET_TYPE ResetType
){
  UINT8 InitialData;
  UINT8 OutputData;
  UINT8 PwrRsrCfg;

  DEBUG((DEBUG_INFO, "[SpecificWarmResetSystem] ResetSystem invoked:  ResetType = %d\n", ResetType));

  InitialData = HARDSTARTSTATE;
  OutputData = HARDRESET;

  LibAmdMemRead (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + 0x10), &PwrRsrCfg, NULL);
  PwrRsrCfg = PwrRsrCfg & 0xFD; //clear ToggleAllPwrGoodOnCf9
  LibAmdMemWrite (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + 0x10), &PwrRsrCfg, NULL);

  IoWrite8 (FCH_IOMAP_REGCF9, InitialData);
  IoWrite8 (FCH_IOMAP_REGCF9, OutputData);
  //
  // Given we should have reset getting here would be bad
  //
  FCH_DEADLOOP();
}

/*********************************************************************************
 * Name: FchPeiReset2
 *
 * Description
 *   Resets the entire platform.
 *
 * Input
 *   ResetType   - warm or cold
 *   ResetStatus - possible cause of reset
 *   DataSize    - Size of ResetData in bytes
 *   ResetData   - Optional Unicode string
 *  For details, see PiPeiCis.h
 *
 * Output
 *   none if the reset is successful
 *
 *********************************************************************************/
VOID
EFIAPI
FchPeiReset2 (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  )
{
  UINT8      InitialData;
  UINT8      OutputData;
  UINT8      PwrRsrCfg;
  UINT16     AcpiGpeBase;
  UINT16     AcpiPm1StsBase;
  UINT16     AcpiPm1CntBase;
  UINT32     Gpe0Enable;
  UINT16     PmCntl;
  UINT16     PwrSts;
  UINTN      ResetDataStringSize;
  EFI_GUID   *ResetTypeGuid;

  DEBUG ((DEBUG_INFO, "[FchPeiReset2] FchPeiReset2 invoked:  ResetType = %d\n", ResetType));
  switch (ResetType) {
  case EfiResetWarm:
    InitialData = HARDSTARTSTATE;
    OutputData  = HARDRESET;
    break;

  case EfiResetCold:
    InitialData = HARDSTARTSTATE;
    OutputData  = HARDRESET;
    LibAmdMemRead (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + 0x10), &PwrRsrCfg, NULL);
    PwrRsrCfg = PwrRsrCfg | BIT1; //set ToggleAllPwrGoodOnCf9
    LibAmdMemWrite (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + 0x10), &PwrRsrCfg, NULL);
    break;

  case EfiResetPlatformSpecific:
    InitialData = HARDSTARTSTATE;
    OutputData  = HARDRESET;
    if ((DataSize >= sizeof(EFI_GUID)) && (ResetData != NULL)){
      ResetDataStringSize = StrnSizeS (ResetData, (DataSize / sizeof(CHAR16)));
      if ((ResetDataStringSize < DataSize) && ((DataSize - ResetDataStringSize) >= sizeof(EFI_GUID))){
        ResetTypeGuid = (EFI_GUID *)((UINT8 *)ResetData + ResetDataStringSize);
        if (CompareGuid (&gEfiAmdAgesaSpecificWarmResetGuid, ResetTypeGuid)){
          SpecificWarmResetSystem (EfiResetWarm);
        }
      }
    }
    break;

  case EfiResetShutdown:
    // Disable all GPE0 Event
    AcpiGpeBase = ReadPmio16 (FCH_PMIOA_REG68);
    AcpiGpeBase += 4; //Get enable base
    Gpe0Enable  = 0;
    IoWrite32 (AcpiGpeBase, Gpe0Enable);

    // Clear Power Button status.
    AcpiPm1StsBase = ReadPmio16 (FCH_PMIOA_REG60);
    PwrSts  = BIT8 | BIT15; //Clear WakeStatus with PwrBtnStatus
    IoWrite16 (AcpiPm1StsBase, PwrSts);

    // Transform system into S5 sleep state
    AcpiPm1CntBase = ReadPmio16 (FCH_PMIOA_REG62);
    PmCntl  = IoRead16 (AcpiPm1CntBase);
    PmCntl  = (PmCntl & ~SLP_TYPE) | SUS_S5 | SLP_EN;
    IoWrite16 (AcpiPm1CntBase, PmCntl);
    FCH_DEADLOOP();
    return ;

  default:
    return ;
  }

  IoWrite8 (FCH_IOMAP_REGCF9, InitialData);
  IoWrite8 (FCH_IOMAP_REGCF9, OutputData);

  //
  // Given we should have reset getting here would be bad
  //
  FCH_DEADLOOP();
}

