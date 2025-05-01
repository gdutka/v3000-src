/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/**
  Library to support forcibly reset library, inheritted from Agesa
  Cf9Reset module.

**/

#include <Uefi.h>
#include "AMD.h"
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/FchBaseLib.h>
#include <Protocol/Reset.h>
#include <Library/ResetSystemLib.h>
#include <FchRegistersCommon.h>

#define FILECODE LIBRARY_FCHBASERESETSYSTEMLIB_FCHFORCERESETSYSTEMLIB_FILECODE

//
// Reset control register values
//
#define FULLRESET       0x0E
#define HARDRESET       0x06
#define SOFTRESET       0x04
#define FULLSTARTSTATE  0x0A
#define HARDSTARTSTATE  0x02
#define SOFTSTARTSTATE  0x00

#define SUS_S3          0x0C00U     // S3
#define SUS_S5          0x1400U     // S4
#define SLP_TYPE        0x1C00U     // MASK
#define SLP_EN          0x2000U     // BIT13

/**
  This function causes a system-wide reset (cold reset), in which
  all circuitry within the system returns to its initial state. This type of reset
  is asynchronous to system operation and operates without regard to
  cycle boundaries.

  If this function returns, it means that the system does not support cold reset.
**/
VOID
EFIAPI
ResetCold (
  VOID
)
{
  UINT8      InitialData;
  UINT8      OutputData;
  BOOLEAN    IsFullHardReset = FALSE;

  DEBUG(( DEBUG_INFO,"%a - enter..\n", __FUNCTION__ ));

  IsFullHardReset = PcdGetBool (PcdFchFullHardReset);
  if (IsFullHardReset){
    InitialData = FULLSTARTSTATE;
    OutputData  = FULLRESET;
  }else{
    InitialData = HARDSTARTSTATE;
    OutputData  = HARDRESET;
  }
  ACPIMMIO8(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10) |= BIT1;          //set ToggleAllPwrGoodOnCf9
  ACPIMMIO8(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD2) &= ~(UINT8)BIT6;  //clear cf9rstdisable

  IoWrite8 (FCH_IOMAP_REGCF9, InitialData);
  IoWrite8 (FCH_IOMAP_REGCF9, OutputData);

  //
  // Given we should have reset getting here would be bad
  //
  DEBUG(( DEBUG_ERROR, "%a - should never be here!!!\n", __FUNCTION__ ));
  FCH_DEADLOOP();
}

/**
  This function causes a system-wide initialization (warm reset), in which all processors
  are set to their initial state. Pending cycles are not corrupted.

  If this function returns, it means that the system does not support warm reset.
**/
VOID
EFIAPI
ResetWarm (
  VOID
)
{
  UINT8      InitialData;
  UINT8      OutputData;

  DEBUG(( DEBUG_INFO, "%a - enter..\n",__FUNCTION__ ));

  InitialData = HARDSTARTSTATE;
  OutputData  = HARDRESET;

  ACPIMMIO8(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD2) &= ~(UINT8)BIT6;  //clear cf9rstdisable

  IoWrite8 (FCH_IOMAP_REGCF9, InitialData);
  IoWrite8 (FCH_IOMAP_REGCF9, OutputData);

  //
  // Given we should have reset getting here would be bad
  //
  DEBUG(( DEBUG_ERROR, "%a - should never be here!!!\n",__FUNCTION__ ));
  FCH_DEADLOOP();
}

/**
  This function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  If this function returns, it means that the system does not support shutdown reset.
**/
VOID
EFIAPI
ResetShutdown (
  VOID
)
{
  UINT16     AcpiGpeBase;
  UINT16     AcpiPm1StsBase;
  UINT16     AcpiPm1CntBase;
  UINT32     Gpe0Enable;
  UINT16     PmCntl;
  UINT16     PwrSts;

  DEBUG(( DEBUG_INFO, "%a - enter..\n",__FUNCTION__ ));

  // Set SLP_TYPEn to enable SLP_TYP to do S1/S3/S5
  ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) |= BIT5;
  // Enable S state transition
  ACPIMMIO32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG08) &= ~ (UINT32)(BIT24 + BIT25);
  ACPIMMIO32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG08) |= BIT24;

  // Disable all GPE0 Event
  AcpiGpeBase = ACPIMMIO16(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG68);
  AcpiGpeBase += 4; //Get enable base
  Gpe0Enable  = 0;
  IoWrite32 (AcpiGpeBase, Gpe0Enable);

  // Clear Power Button status.
  AcpiPm1StsBase = ACPIMMIO16(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60);
  PwrSts  = BIT8 | BIT15; //Clear WakeStatus with PwrBtnStatus
  IoWrite16 (AcpiPm1StsBase, PwrSts);

  // Transform system into S5 sleep state
  AcpiPm1CntBase = ACPIMMIO16(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG62);
  PmCntl  = IoRead16 (AcpiPm1CntBase);
  PmCntl  = (PmCntl & ~SLP_TYPE) | SUS_S5 | SLP_EN;
  IoWrite16 (AcpiPm1CntBase, PmCntl);

  //
  // Given we should have shutdown getting here would be bad
  //
  DEBUG(( DEBUG_ERROR, "%a - should never be here!!!\n",__FUNCTION__ ));
  FCH_DEADLOOP();
}

/**
  This function causes the system to enter S3 and then wake up immediately.

  If this function returns, it means that the system does not support S3 feature.
**/
VOID
EFIAPI
EnterS3WithImmediateWake (
  VOID
)
{
  DEBUG(( DEBUG_ERROR, "%a - No S3WithImmediateWake handler found!\n",__FUNCTION__ ));
  ASSERT (FALSE);
}

/**
  This function causes a systemwide reset. The exact type of the reset is
  defined by the EFI_GUID that follows the Null-terminated Unicode string passed
  into ResetData. If the platform does not recognize the EFI_GUID in ResetData
  the platform must pick a supported reset type to perform.The platform may
  optionally log the parameters from any non-normal reset that occurs.

  @param[in]  DataSize   The size, in bytes, of ResetData.
  @param[in]  ResetData  The data buffer starts with a Null-terminated string,
                         followed by the EFI_GUID.
**/
VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN   DataSize,
  IN VOID    *ResetData
)
{
  DEBUG(( DEBUG_ERROR, "%a - No PlatformSpecific reset handler found!\n",__FUNCTION__ ));
  ASSERT( FALSE );
  ResetCold();
}
