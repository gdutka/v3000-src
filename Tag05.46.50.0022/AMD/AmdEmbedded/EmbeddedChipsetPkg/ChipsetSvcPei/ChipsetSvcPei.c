/** @file
 PEI Chipset Services driver.

 It produces an instance of the PEI Chipset Services PPI to provide the chipset related functions
 which will be used by Kernel or Project code. These PPI functions should be used by calling the
 corresponding functions in PeiChipsetSvcLib to have the protocol size checking

***************************************************************************
* Copyright (c) 2012 - 2021, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

//
// Libraries
//
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ChipsetConfigLib.h>
#include <Library/PeiCapsuleLib.h>
#include <Library/PcdLib.h>
#include <Library/CmosLib.h>
#include <Library/VariableLib.h>
#include <PiPei.h>
#include <Guid/H2OCp.h>
#include <Guid/PreventFlashPowerLoss.h>

//
// Produced Ppi
//
#include <Ppi/H2OPeiChipsetServices.h>

#define PCAT_RTC_EX_ADDRESS_REGISTER   0x72
#define PCAT_RTC_EX_DATA_REGISTER      0x73
#define CHIPSET_RECOVERY_FLAG1         0xA4
#define CHIPSET_RECOVERY_FLAG2         0xA5
#define RECOVERY_VALUE1                0x55
#define RECOVERY_VALUE2                0xAA
#define CLEAR_VALUE                    0x00
#define RECOVERY_REQUEST_START         0x80
#define RECOVERY_REQUEST_END           0x88
#define RECOVERY_REQUEST_VALUE         0x44
#define BIOS_RAM_INDEX                 0xCD4
#define BIOS_RAM_DATA                  0xCD5
//
// Structure for H2O Chipsete Services PPI Descriptor
//
typedef struct {
  EFI_PEI_PPI_DESCRIPTOR    PpiDescriptor;
  H2O_CHIPSET_SERVICES_PPI  H2OChipsetSvcPpi;
} H2O_CHIPSET_SERVICES_PPI_DESCRIPTOR;

//
// H2O Chipsete Services function prototypes
//
EFI_STATUS
EFIAPI
EnableFdWrites (
  IN  BOOLEAN                       EnableWrites
  );

EFI_STATUS
EFIAPI
LegacyRegionAccessCtrl (
  IN  UINT32                        Start,
  IN  UINT32                        Length,
  IN  UINT32                        Mode
  );

EFI_STATUS
EFIAPI
GetSleepState (
  IN OUT  UINT8                    *SleepState
  );

EFI_STATUS
EFIAPI
HdaInitHook (
  IN UINT32                         HdaBar
  );

EFI_STATUS
EFIAPI
PlatformStage1Init (
  VOID
  );

EFI_STATUS
EFIAPI
PlatformStage2Init (
  VOID
  );

EFI_STATUS
EFIAPI
Stage1MemoryDiscoverCallback (
  IN  EFI_BOOT_MODE                 *BootMode
  );

EFI_STATUS
EFIAPI
Stage2MemoryDiscoverCallback (
  IN EFI_BOOT_MODE                  *BootMode
  );

EFI_STATUS
EFIAPI
ModifyMemoryRange (
  IN OUT  PEI_MEMORY_RANGE_OPTION_ROM           *OptionRomMask,
  IN OUT  PEI_MEMORY_RANGE_SMRAM                *SmramMask,
  IN OUT  PEI_MEMORY_RANGE_GRAPHICS_MEMORY      *GraphicsMemoryMask,
  IN OUT  PEI_MEMORY_RANGE_PCI_MEMORY           *PciMemoryMask
  );

EFI_STATUS
EFIAPI
SetSetupVariableDefault (
  IN VOID                           *SystemConfiguration
  );

/**
 Change boot mode if Secure flash on warm reboot.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
SetBootModeBeforeCallback (
  IN EFI_EVENT                          Event,
  IN H2O_CP_HANDLE                      Handle
  )
{
  H2O_PEI_CP_SET_BOOT_MODE_BEFORE_DATA  *SetBootModeData;
  EFI_STATUS                            Status;
  UINT8                                 SleepState;

  Status = H2OCpLookup (Handle, (VOID **) &SetBootModeData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG_CP ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG_CP ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }

  if (SetBootModeData->CustomBootModePolicy ||
      SetBootModeData->BootMode == BOOT_IN_RECOVERY_MODE) {
    return;
  }

  Status = GetSleepState (&SleepState);
  if (EFI_ERROR (Status)) {
    SleepState = 0;
  }

  if (SleepState != 3 && SleepState != 4 && DetectPendingUpdateImage()) {
    SetBootModeData->BootMode = BOOT_ON_FLASH_UPDATE;
    SetBootModeData->Status   = H2O_CP_TASK_UPDATE;
  }
}

/**
 Provide a callback function for CRB use.
 After FvMain corrupted, trigger system to entry recovery mode by filling CMOS data.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
CpFvCorruptedCallback (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  EFI_STATUS                         Status;
  H2O_PEI_CP_DXE_FV_CORRUPTED_DATA   *DxeFvCorruptedData;

  Status = H2OCpLookup (Handle, (VOID **) &DxeFvCorruptedData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG_CP ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG_CP ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }
  WriteExtCmos8 (PCAT_RTC_EX_ADDRESS_REGISTER, PCAT_RTC_EX_DATA_REGISTER, CHIPSET_RECOVERY_FLAG1, RECOVERY_VALUE1);
  WriteExtCmos8 (PCAT_RTC_EX_ADDRESS_REGISTER, PCAT_RTC_EX_DATA_REGISTER, CHIPSET_RECOVERY_FLAG2, RECOVERY_VALUE2);
  DxeFvCorruptedData->ResetSystem = TRUE;
  DxeFvCorruptedData->Status      = H2O_CP_TASK_UPDATE;
}

/**
 A callback function for Clearing CMOS flag of recovery.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
CpClearCmosRecoveryFlagCallback (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  EFI_STATUS                              Status;
  H2O_PEI_CP_CRISIS_RECOVERY_FLASH_DATA   *CrisisRecoveryFlashData;

  Status = H2OCpLookup (Handle, (VOID **) &CrisisRecoveryFlashData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG_CP ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG_CP ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }
  WriteExtCmos8 (PCAT_RTC_EX_ADDRESS_REGISTER, PCAT_RTC_EX_DATA_REGISTER, CHIPSET_RECOVERY_FLAG1, CLEAR_VALUE);
  WriteExtCmos8 (PCAT_RTC_EX_ADDRESS_REGISTER, PCAT_RTC_EX_DATA_REGISTER, CHIPSET_RECOVERY_FLAG2, CLEAR_VALUE);

  CrisisRecoveryFlashData->Status = H2O_CP_TASK_UPDATE;
}

/** @file
 A callback function for support A/B recovery to check whether in the flash mode.
  
 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
CheckForFlashModeCallback (
  IN EFI_EVENT                          Event,
  IN H2O_CP_HANDLE                      Handle
  )
{
  EFI_STATUS                       Status;
  UINT8                            Flag;
  UINTN                            Size;

  Size = sizeof (Flag);
  Status = CommonGetVariable (
             PREVENT_FLASH_POWER_LOSS,
             &gPreventFlashPowerLossGuid,
             &Size,
             &Flag
             );
  if (!EFI_ERROR (Status)) {
    Status = PcdSetBoolS (PcdSkipSelfHealing, TRUE);
    if (EFI_ERROR (Status)) {
      return;
    }
  }
}

/**
 This is the declaration of PEI Chipset Services PEIM entry point.

 @param[in]         FileHandle          Handle of the file being invoked.
 @param[in]         PeiServices         Describes the list of possible PEI Services.

 @retval            EFI_SUCCESS         The operation completed successfully.
 @retval            Others              An unexpected error occurred.
*/
EFI_STATUS
EFIAPI
ChipsetSvcPeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE                FileHandle,
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                            Status;
  EFI_STATUS                            ConfigStatus;
  UINT32                                Size;
  H2O_CHIPSET_SERVICES_PPI_DESCRIPTOR   *ChipsetSvcPpiDescriptor;
  EFI_PEI_PPI_DESCRIPTOR                *PpiDescriptor;
  VOID                                  *Ppi;
  UINT8                                 RecoveryFlag1;
  UINT8                                 RecoveryFlag2;
  CHIPSET_CONFIGURATION                 ChipsetConfiguration;
  UINT8                                 Index;

  PpiDescriptor = NULL;
  Ppi = NULL;

  //
  // Register so it will be automatically shadowed to memory
  //
  Status = PeiServicesRegisterForShadow (FileHandle);

  //
  // Create an instance of the H2O Chipset Services ppi,
  //
  ChipsetSvcPpiDescriptor = AllocateZeroPool (sizeof (H2O_CHIPSET_SERVICES_PPI_DESCRIPTOR));
  if (ChipsetSvcPpiDescriptor == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Size = sizeof (H2O_CHIPSET_SERVICES_PPI);
  if (Size < sizeof (UINT32)) {         // must at least contain Size field.
    return EFI_INVALID_PARAMETER;
  }

  //
  // Build ChipsetSvcPpi Private Data
  //
  ChipsetSvcPpiDescriptor->PpiDescriptor.Flags   = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  ChipsetSvcPpiDescriptor->PpiDescriptor.Guid    = &gH2OChipsetServicesPpiGuid;
  ChipsetSvcPpiDescriptor->PpiDescriptor.Ppi     = &ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi;
  ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.Size = Size;

  //
  // Populate all functions based on Size.
  //
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, EnableFdWrites) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.EnableFdWrites = EnableFdWrites;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, LegacyRegionAccessCtrl) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.LegacyRegionAccessCtrl = LegacyRegionAccessCtrl;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, GetSleepState) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.GetSleepState = GetSleepState;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, HdaInitHook) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.HdaInitHook = HdaInitHook;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, PlatformStage1Init) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.PlatformStage1Init = PlatformStage1Init;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, Stage1MemoryDiscoverCallback) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.Stage1MemoryDiscoverCallback = Stage1MemoryDiscoverCallback;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, PlatformStage2Init) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.PlatformStage2Init = PlatformStage2Init;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, Stage2MemoryDiscoverCallback) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.Stage2MemoryDiscoverCallback = Stage2MemoryDiscoverCallback;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, ModifyMemoryRange) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.ModifyMemoryRange = ModifyMemoryRange;
  }
  if (Size >= (OFFSET_OF (H2O_CHIPSET_SERVICES_PPI, SetSetupVariableDefault) + sizeof (VOID*))) {
    ChipsetSvcPpiDescriptor->H2OChipsetSvcPpi.SetSetupVariableDefault = SetSetupVariableDefault;
  }

  if (Status == EFI_ALREADY_STARTED) {
    //
    // Shadow completed and running from memory
    //

    Status = PeiServicesLocatePpi (
               &gH2OChipsetServicesPpiGuid,
               0,
               &PpiDescriptor,
               (VOID **) &Ppi
               );

    if (!EFI_ERROR (Status)) {
      Status = PeiServicesReInstallPpi (
                 PpiDescriptor,
                 &ChipsetSvcPpiDescriptor->PpiDescriptor
                 );
    }
  } else {
    //
    // Install the Ppi.
    //
    Status = PeiServicesInstallPpi (&ChipsetSvcPpiDescriptor->PpiDescriptor);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    ConfigStatus = GetChipsetConfiguration (&ChipsetConfiguration);
    if (EFI_ERROR(ConfigStatus)) {
      return ConfigStatus;
    }
    // 
    // To ensure that the FTPM switch is configured
    // 
    Status = PcdSet8S (PcdH2OTpmType, ChipsetConfiguration.EnableTPM);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, " TPM setting = %x\n", PcdGet8 (PcdH2OTpmType)));
    // Due to the SpiOrLpcTPM have been comment out, it should follow the Platform DSC defined.
    // Status = PcdSet8S (PcdH2OSpiOrLpcTPMSupported, ChipsetConfiguration.SpiOrLpcTPM);
    // ASSERT_EFI_ERROR (Status);
    // DEBUG ((EFI_D_ERROR, " SpiOrLpc TPM = %x\n", PcdGet8 (PcdH2OSpiOrLpcTPMSupported)));
  
    if (ChipsetConfiguration.EnableTPM == TPM_CHIP_DEVICE_ENABLE) {
      Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 0);
    } else if (ChipsetConfiguration.EnableTPM == TPM_FIRMWARE_DEVICE_ENABLE) {
      Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 1);
    } else if (ChipsetConfiguration.EnableTPM == HSP_TPM_FIRMWARE_DEVICE_ENABLE) {
      Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 2);
    } else {
      Status = PcdSet8S (PcdAmdPspSystemTpmConfig, 0xFF);
    }
    ASSERT_EFI_ERROR (Status);

    for (Index = RECOVERY_REQUEST_START; Index <= RECOVERY_REQUEST_END; Index++) {
      IoWrite8 (BIOS_RAM_INDEX, Index);
      if (IoRead8 (BIOS_RAM_DATA) != RECOVERY_REQUEST_VALUE) {
        break ;
      }
    }
    if (Index > RECOVERY_REQUEST_END && PcdGetBool (PcdUseFastCrisisRecovery) == 0) {
      //
      // Check Recovery Flag in CMOS.
      //
      RecoveryFlag1 = ReadExtCmos8 ( PCAT_RTC_EX_ADDRESS_REGISTER, PCAT_RTC_EX_DATA_REGISTER, CHIPSET_RECOVERY_FLAG1);
      RecoveryFlag2 = ReadExtCmos8 ( PCAT_RTC_EX_ADDRESS_REGISTER, PCAT_RTC_EX_DATA_REGISTER, CHIPSET_RECOVERY_FLAG2);

      if (RecoveryFlag1 == RECOVERY_VALUE1 && RecoveryFlag2 == RECOVERY_VALUE2) {
        PeiServicesSetBootMode (BOOT_IN_RECOVERY_MODE);
        DEBUG ((EFI_D_ERROR, "Early set BOOT_IN_RECOVERY_MODE\n"));
      }
    }

    if (FeaturePcdGet (PcdH2OPeiCpSetBootModeBeforeSupported)) {
      H2O_CP_HANDLE         Handle;

      Status = H2OCpRegisterHandler (
                 &gH2OPeiCpSetBootModeBeforeGuid,
                 SetBootModeBeforeCallback,
                 H2O_CP_MEDIUM,
                 &Handle
                 );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OPeiCpSetBootModeBeforeGuid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OPeiCpSetBootModeBeforeGuid, Status));
    }
    if (FeaturePcdGet (PcdH2OPeiCpDxeFvCorruptedSupported)) {
      H2O_CP_HANDLE         CpHandle;

      Status = H2OCpRegisterHandler (
                 &gH2OPeiCpDxeFvCorruptedGuid,
                 CpFvCorruptedCallback,
                 H2O_CP_MEDIUM,
                 &CpHandle
                 );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OPeiCpDxeFvCorruptedGuid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OPeiCpDxeFvCorruptedGuid, Status));
    }
    if (FeaturePcdGet (PcdH2OPeiCpCrisisRecoveryFlashSupported)) {
      H2O_CP_HANDLE         Handle1;

      Status = H2OCpRegisterHandler (
                 &gH2OPeiCpCrisisRecoveryFlashGuid,
                 CpClearCmosRecoveryFlagCallback,
                 H2O_CP_MEDIUM,
                 &Handle1
                 );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OPeiCpCrisisRecoveryFlashGuid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OPeiCpCrisisRecoveryFlashGuid, Status));
    }
    if (FeaturePcdGet (PcdH2OBiosUpdateFlashSequenceEnabled)) {
      H2O_CP_HANDLE         CpHandle1;

      Status = H2OCpRegisterHandler (
                 &gH2OPeiCpInitPlatformStage1Guid,
                 CheckForFlashModeCallback,
                 H2O_CP_MEDIUM,
                 &CpHandle1
                 );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OPeiCpInitPlatformStage1Guid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OPeiCpInitPlatformStage1Guid, Status));
    }

  }

  return Status;
}

