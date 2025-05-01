/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH reset system library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASERESETSYSTEMLIB_FCHBASERESETSYSTEMLIB_FILECODE

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

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
extern EFI_GUID   gEfiAmdAgesaSpecificWarmResetGuid;
EFI_EVENT         mVirtualNotifyEvent;
PHYSICAL_ADDRESS  mFchAcpiMmio;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                            L O C A L   F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


 /*----------------------------------------------------------------------------------------
 *                                 F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
STATIC
SpecificWarmResetSystem(
    IN EFI_RESET_TYPE   ResetType
    ) {

  DEBUG((DEBUG_INFO, "[SpecificWarmResetSystem] ResetSystem invoked:  ResetType = %d\n", ResetType));

  //clear ToggleAllPwrGoodOnCf9
  ACPIMMIO8(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG10) &= 0xFD;

  IoWrite8(FCH_IOMAP_REGCF9, HARDSTARTSTATE);
  IoWrite8(FCH_IOMAP_REGCF9, HARDRESET);
  //
  // Given we should have reset getting here would be bad
  //
  FCH_DEADLOOP();
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed.
  @param[in]    Context The Event Context.
**/
VOID
EFIAPI
VirtualMemoryNotifyEvent(
    IN EFI_EVENT        Event,
    IN VOID             *Context
    ) {
  //
  // Update global for Runtime Services Table and IO
  //
  gRT->ConvertPointer(0, (VOID **)&mFchAcpiMmio);
}

/******************************************************************************
 * Name: FchDxeRuntimeResetSystemLibConstructor
 *
 * Description
 *   Library Constructor
 *
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 ******************************************************************************/
EFI_STATUS
EFIAPI
FchDxeRuntimeResetSystemLibConstructor(
    VOID
    ) {
  EFI_STATUS  Status;
  mFchAcpiMmio = ACPI_MMIO_BASE;
  //
  // Register SetVirtualAddressMap () notify function
  //
  Status = gBS->CreateEvent(
      EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
      TPL_NOTIFY,
      VirtualMemoryNotifyEvent,
      NULL,
      &mVirtualNotifyEvent
      );
  ASSERT_EFI_ERROR(Status);
  return Status;
}

/**
  This function causes a system-wide reset (cold reset), in which
  all circuitry within the system returns to its initial state. This type of
  reset is asynchronous to system operation and operates without regard to
  cycle boundaries.

  If this function returns, it means that the system does not support
  cold reset.
**/
VOID
EFIAPI
ResetCold(
    VOID
    ) {
  //set ToggleAllPwrGoodOnCf9
  ACPIMMIO8(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG10) |= BIT1;
  if (PcdGetBool(PcdFchFullHardReset)) {
    IoWrite8(FCH_IOMAP_REGCF9, FULLSTARTSTATE);
    IoWrite8(FCH_IOMAP_REGCF9, FULLRESET);
  } else {
    IoWrite8(FCH_IOMAP_REGCF9, HARDSTARTSTATE);
    IoWrite8(FCH_IOMAP_REGCF9, HARDRESET);
  }
}

/**
  This function causes a system-wide initialization (warm reset), in which all
  processors are set to their initial state. Pending cycles are not corrupted.

  If this function returns, it means that the system does not support warm reset.
**/
VOID
EFIAPI
ResetWarm(
    VOID
    ) {
  //clear ToggleAllPwrGoodOnCf9
  ACPIMMIO8(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG10) &= 0xFD;
  IoWrite8(FCH_IOMAP_REGCF9, HARDSTARTSTATE);
  IoWrite8(FCH_IOMAP_REGCF9, HARDRESET);
}

/**
  This function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  If this function returns, it means that the system does not support
  shutdown reset.
**/
VOID
EFIAPI
ResetShutdown(
    VOID
    ) {
  UINT16     AcpiGpeBase;
  UINT16     AcpiPm1StsBase;
  UINT16     AcpiPm1CntBase;
  UINT32     Gpe0Enable;
  UINT16     PmCntl;
  UINT16     PwrSts;

  // Disable all GPE0 Event
  AcpiGpeBase = ACPIMMIO16(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG68);
  AcpiGpeBase += 4; //Get enable base
  Gpe0Enable  = 0;
  IoWrite32(AcpiGpeBase, Gpe0Enable);

  // Clear Power Button status.
  AcpiPm1StsBase = ACPIMMIO16(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG60);
  PwrSts  = BIT8 | BIT15; //Clear WakeStatus with PwrBtnStatus
  IoWrite16(AcpiPm1StsBase, PwrSts);

  // Transform system into S5 sleep state
  AcpiPm1CntBase = ACPIMMIO16(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG62);
  PmCntl  = IoRead16(AcpiPm1CntBase);
  PmCntl  = (PmCntl & ~SLP_TYPE) | SUS_S5 | SLP_EN;
  IoWrite16(AcpiPm1CntBase, PmCntl);
}

/**
  This function causes a systemwide reset. The exact type of the reset is
  defined by the EFI_GUID that follows the Null-terminated Unicode string
  passed into ResetData. If the platform does not recognize the EFI_GUID in
  ResetData the platform must pick a supported reset type to perform. The
  platform may optionally log the parameters from any non-normal reset that
  occurs.

  @param[in]  DataSize   The size, in bytes, of ResetData.
  @param[in]  ResetData  The data buffer starts with a Null-terminated string,
                         followed by the EFI_GUID.
**/
VOID
EFIAPI
ResetPlatformSpecific(
    IN UINTN   DataSize,
    IN VOID    *ResetData
    ) {
  UINTN      ResetDataStringSize;
  EFI_GUID   *ResetTypeGuid;

  if ((DataSize >= sizeof(EFI_GUID)) && (ResetData != NULL)) {
    ResetDataStringSize = StrnSizeS(ResetData, (DataSize / sizeof(CHAR16)));
    if ((ResetDataStringSize < DataSize) && ((DataSize - ResetDataStringSize) >= sizeof(EFI_GUID))) {
      ResetTypeGuid = (EFI_GUID *)((UINT8 *)ResetData + ResetDataStringSize);
      if (CompareGuid(&gEfiAmdAgesaSpecificWarmResetGuid, ResetTypeGuid)) {
        SpecificWarmResetSystem(EfiResetPlatformSpecific);
      }
    }
  }
  IoWrite8(FCH_IOMAP_REGCF9, HARDSTARTSTATE);
  IoWrite8(FCH_IOMAP_REGCF9, HARDRESET);
}

/**
  The ResetSystem function resets the entire platform.

  @param[in] ResetType      The type of reset to perform.
  @param[in] ResetStatus    The status code for the reset.
  @param[in] DataSize       The size, in bytes, of ResetData.
  @param[in] ResetData      For a ResetType of EfiResetCold, EfiResetWarm,
                            or EfiResetShutdown the data buffer starts with a
                            Null-terminated string, optionally followed by
                            additional binary data. The string is a description
                            that the caller may use to further indicate the
                            reason for the system reset.
**/
VOID
EFIAPI
ResetSystem(
    IN EFI_RESET_TYPE               ResetType,
    IN EFI_STATUS                   ResetStatus,
    IN UINTN                        DataSize,
    IN VOID                         *ResetData OPTIONAL
    ) {
  DEBUG((DEBUG_INFO, "[FchCf9Reset] ResetSystem invoked:  ResetType = %d\n", ResetType));

  switch (ResetType) {
    //
    // For update resets, the reset data is a null-terminated string followed
    // by a VOID * to the capsule descriptors. Get the pointer and set the
    // capsule variable before we do a warm reset. Per the EFI 1.10 spec, the
    // reset data is only valid if ResetStatus != EFI_SUCCESS.
    //
    case EfiResetWarm:
      ResetWarm();
      break;

    case EfiResetCold:
      ResetCold();
      break;

    case EfiResetPlatformSpecific:
      ResetPlatformSpecific(DataSize, ResetData);
      break;

    case EfiResetShutdown:
      ResetShutdown();
      return;

    default:
      return;
  }

  //
  // Given we should have reset getting here would be bad
  //
  FCH_DEADLOOP();
}
