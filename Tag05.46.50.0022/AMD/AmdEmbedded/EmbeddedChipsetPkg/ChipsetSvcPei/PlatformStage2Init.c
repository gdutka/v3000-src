/** @file
 PEI Chipset Services Library.

 This file contains only one function that is PeiCsSvcPlatformStage2Init().
 The function PeiCsSvcPlatformStage2Init() use chipset services to initialization
 chipset in stage 2 of PEi phase.

***************************************************************************
* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <PiPei.h>
//
// Libraries
//
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/KernelConfigLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciExpressLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/VerbTable.h>
#include <Ppi/RomCorrupt.h>
#include <Ppi/EndOfPeiPhase.h>

#include <YangtzeReg.h>
#include <BiosRamUsage.h>
#include <MmioAccess.h>

#include <Ppi/AmdFchInitPpi.h>
#include <Ppi/AmdPspRecoveryDetectPpi.h>

#include <PortConfig.h>
//
// For Fast recovery.
//
//#define EHCI_RECOVERY_TEMP_MEM_BASE_ADDRESS     0xFC100000
#define SATA_RECOVERY_TEMP_BAR5_ADDRESS         0xFEC01000
#define SATA_RECOVERY_TEMP_BAR0_IOBASE          0x9000

EFI_STATUS
EFIAPI
PspCorruptDetectNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  );

//
// Fch Recovery Init
//
EFI_STATUS
EFIAPI
AmdFchRecoveryInit (
  IN EFI_PEI_SERVICES                 **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR        *NotifyDescriptor,
  IN VOID                             *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mNotifyOnPspCorruptDetect[] = {
  {(EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  PspCorruptDetectNotifyCallback}
};

CONST EFI_PEI_PPI_DESCRIPTOR mPpiRomCorrupt = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiRomCorruptPpiGuid,
  NULL
};
STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFchInitPpiGuid,
  AmdFchRecoveryInit
};

EFI_STATUS
EFIAPI
PspCorruptDetectNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  )
{
  EFI_STATUS                        Status;
  AMD_PSP_RECOVERY_DETECT_PPI       *AmdPspRecoveryDetectPpi = NULL;

  DEBUG ((EFI_D_ERROR, "\tPspCorruptDetectNotifyCallback Entry\n"));

  Status = (*PeiServices)->LocatePpi  (
                             (CONST EFI_PEI_SERVICES**)PeiServices,
                             &gAmdPspRecoveryDetectPpiGuid,
                             0,
                             NULL,
                             (VOID**)&AmdPspRecoveryDetectPpi
                             );

  if (AmdPspRecoveryDetectPpi != NULL) {
    DEBUG ((EFI_D_ERROR, "\tPSP FW Corruption Detected!!!\n"));
    Status = PeiServicesInstallPpi (&mPpiRomCorrupt);
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((EFI_D_ERROR, "\tPspCorruptDetectNotifyCallback Exit\n"));

  return EFI_SUCCESS;
}


/**
 Clear all SMI enable bit in PMIO register

 @param [in]        PeiServices         Describes the list of possible PEI Services
 @param [in]        CpuIo               A pointer to CPU IO Services PPI

 @retval            None
*/
VOID
ClearAllSmiEnableInPmio (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN       EFI_PEI_CPU_IO_PPI       *CpuIo
  )
{
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA0, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA4, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA4, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB8, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGBC, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC0, 0x00);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC4, 0x00);
}

/**
 Clear SB Smi and Wake state

 @param [in]        PeiServices         Describes the list of possible PEI Services
 @param [in]        CpuIo               A pointer to CPU IO Services PPI
 @param [in]        PmBase              Acpi Pm1 baseaddress

 @retval            None
*/
VOID
ClearSBSmiAndWake (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN       EFI_PEI_CPU_IO_PPI       *CpuIo,
  IN       UINT16                   PmBase
  )
{
  UINT16  Pm1Sts;
  UINT32  Pm1Cnt;
  UINT32  Gpe0Sts;

  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (PmBase + R_FCH_ACPI_PM1_STATUS);
  Pm1Cnt  = IoRead32 (PmBase + R_FCH_ACPI_PM_CONTROL);
  Gpe0Sts = IoRead32 (PmBase + R_FCH_ACPI_EVENT_STATUS);

  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts &= B_PWR_BTN_STATUS + B_WAKEUP_STATUS;
  Pm1Cnt &= ~(B_SCI_EN);

  //
  // Write them back
  //
  IoWrite16 (PmBase + R_FCH_ACPI_PM1_STATUS, Pm1Sts);
  IoWrite32 (PmBase + R_FCH_ACPI_PM_CONTROL, Pm1Cnt);
  IoWrite32 (PmBase + R_FCH_ACPI_EVENT_STATUS, Gpe0Sts);
}

/**
 Chipset initialization code in stage2 of PEI phase.

 @param[in]         None

 @retval            EFI_SUCCESS         Procedure complete.
*/
EFI_STATUS
PlatformStage2Init (
  VOID
  )
{
  EFI_STATUS                           Status;
  EFI_BOOT_MODE                        BootMode;
  CONST EFI_PEI_SERVICES               **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  (**PeiServices).GetBootMode (PeiServices, &BootMode);

  if ((BootMode == BOOT_IN_RECOVERY_MODE)) {
    (**PeiServices).NotifyPpi (PeiServices, &mNotifyList);
  }

  //
  //Disable PM1 and GPE0
  //
  if ((BootMode != BOOT_ON_S3_RESUME)) {
    //
    // Clear all pending SMI. On S3 clear power button enable so it wll not generate an SMI
    //
    ClearSBSmiAndWake (PeiServices, (**PeiServices).CpuIo, MmioRead16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60));
    ClearAllSmiEnableInPmio (PeiServices, (**PeiServices).CpuIo);

    //
    // Clear the SPI protect flag
    //
    (**PeiServices).CpuIo->IoWrite8 (PeiServices, ((**PeiServices).CpuIo), FCH_IOMAP_REGCD4, SPI_LOCK_PROTECT_STATUS);
    (**PeiServices).CpuIo->IoWrite8 (PeiServices, ((**PeiServices).CpuIo), FCH_IOMAP_REGCD5, 0);

    // //
    // // Set SATA port enable/disable to Agesa PCD
    // //
    // SataPortConfigDxe ();
  }
  if ((BootMode != BOOT_IN_RECOVERY_MODE) && (BootMode != BOOT_ON_S3_RESUME)) {
    Status = PeiServicesNotifyPpi (mNotifyOnPspCorruptDetect);
    ASSERT_EFI_ERROR (Status);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdFchRecoveryInit(
  IN EFI_PEI_SERVICES                 **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR        *NotifyDescriptor,
  IN VOID                             *Ppi
  )
{
  EFI_STATUS            Status;
  AMD_FCH_INIT_PPI      *AmdFchInitPpi;

  Status = (*PeiServices)->LocatePpi  (
                             (CONST EFI_PEI_SERVICES**)PeiServices,
                             &gAmdFchInitPpiGuid,
                             0,
                             NULL,
                             (VOID**)&AmdFchInitPpi
                             );

  if (!EFI_ERROR (Status)) {
    AmdFchInitPpi->FpFchSataRecovery (AmdFchInitPpi, SATA_RECOVERY_TEMP_BAR0_IOBASE, SATA_RECOVERY_TEMP_BAR5_ADDRESS);
    // Clear AHCI Enable
    MmioAndThenOr32 (SATA_RECOVERY_TEMP_BAR5_ADDRESS + FCH_SATA_BAR5_REG04, ~(UINT32) (BIT31), 0);
  }

  return EFI_SUCCESS;
}

