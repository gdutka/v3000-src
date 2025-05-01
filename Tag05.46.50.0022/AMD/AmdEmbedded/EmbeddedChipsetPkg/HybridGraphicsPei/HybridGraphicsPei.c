/** @file
  This module is for Hybrid Graphics Feature PEI pre-initialize.

;******************************************************************************
;* Copyright (c) 2015 - 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <HybridGraphicsPei.h>

//
// Function Prototypes
//

STATIC
EFI_STATUS
CreateHgInfoDataHob (
  IN OUT HG_INFORMATION_DATA_HOB              **HgData,
  IN CHIPSET_CONFIGURATION                    SystemConfiguration,
  IN AMD_PBS_SETUP_OPTION                     AmdPbsConfiguration
  );

STATIC
PRIMARY_DISPLAY_SETTING
SetHgModeValue (
  IN AMD_PBS_SETUP_OPTION                     AmdPbsConfiguration
  );

EFI_STATUS
EFIAPI
HybridGraphicsInitNotify (
  IN  EFI_PEI_SERVICES                        **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR               *NotifyDescriptor,
  IN  VOID                                    *Ppi
  );

VOID
EFIAPI
Stall (
  IN UINTN                                    Microseconds
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR              mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiMemoryDiscoveredPpiGuid,
    HybridGraphicsInitNotify
  }
};

/**

  Hybrid Graphics feature PEIM entry point.
  This module will modify HG related SA MCH policy, set HG mode and power enable the discrete GPU.

  @param[in] FileHandle     Handle of the file being invoked.
  @param[in] PeiServices    It's a general purpose services available to every PEIM.

  @retval EFI_SUCCESS       Hybrid Graphics feature PEI initialized successfully.
  @retval EFI_UNSUPPORTED   Hybrid Graphics feature doesn't be supported.

**/
EFI_STATUS
HybridGraphicsPeiInitEntry (
  IN EFI_PEI_FILE_HANDLE                      FileHandle,
  IN CONST EFI_PEI_SERVICES                   **PeiServices
  )
{
  EFI_STATUS                                  Status;

  Status = PeiServicesNotifyPpi (&mNotifyList[0]);

  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
HybridGraphicsInitNotify (
  IN  EFI_PEI_SERVICES                        **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR               *NotifyDescriptor,
  IN  VOID                                    *Ppi
  )
{
  EFI_STATUS                                  Status;
  HG_INFORMATION_DATA_HOB                     *HgData;
  CHIPSET_CONFIGURATION                       ChipsetConfiguration;
  AMD_PBS_SETUP_OPTION                        AmdPbsConfiguration;

  if (FeaturePcdGet (PcdHybridGraphicsSupported)) {
    //
    // Get Setup Variable and Amd PBS configuration.
    //
    Status = GetChipsetConfiguration (&ChipsetConfiguration);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // Create Hybrid Graphics information data HOB
    //
    Status = CreateHgInfoDataHob (&HgData, ChipsetConfiguration, AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}


/**

  Base on PCD variables to set HG related information data, then through HOB pass to DXE driver.

  @param[in, out] HgData           A double pointer of HG information data HOB.
  @param[in] SystemConfiguration   It's the setup variables (system configuration).

  @retval EFI_SUCCESS    Created HG information data HOB and initialized successfully.
  @retval !EFI_SUCCESS   Created HG information data HOB failed.

**/
STATIC
EFI_STATUS
CreateHgInfoDataHob (
  IN OUT HG_INFORMATION_DATA_HOB              **HgData,
  IN CHIPSET_CONFIGURATION                    SystemConfiguration,
  IN AMD_PBS_SETUP_OPTION                     AmdPbsConfiguration
  )
{
  EFI_STATUS                                  Status;

  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (HG_INFORMATION_DATA_HOB),
             HgData
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  (**HgData).EfiHobGuidType.Name = gH2OHgInformationDataHobGuid;

  (**HgData).DgpuPresentGpioNo       = PcdGet16 (PcdHgDgpuPrsntGpioNo);
  (**HgData).DgpuResetGpioNo         = PcdGet16 (PcdHgDgpuResetGpioNo);
  (**HgData).DgpuPwrEnableGpioNo     = PcdGet16 (PcdHgDgpuPwrEnableGpioNo);
  (**HgData).DgpuPwrOnGpioNo         = PcdGet16 (PcdHgDgpuPwrOnGpioNo);
  (**HgData).DgpuPwrGoodGpioNo       = PcdGet16 (PcdHgDgpuPwrGoodGpioNo);
  (**HgData).DgpuResetActive         = PcdGetBool (PcdHgDgpuResetActive);
  (**HgData).DgpuPwrEnableActive     = PcdGetBool (PcdHgDgpuPwrEnableActive);
  (**HgData).DgpuPwrOnActive         = PcdGetBool (PcdHgDgpuPwrOnActive);
  (**HgData).DgpuPwrGoodActive       = PcdGetBool (PcdHgDgpuPwrGoodActive);
  (**HgData).DgpuGpioSupport         = PcdGetBool (PcdHgDgpuGpioSupport);

  (**HgData).DelayAfterPwrEn         = PcdGet32 (PcdDelayAfterPwrEnable);
  (**HgData).DelayAfterReset         = PcdGet32 (PcdDelayAfterReset);
  //
  // AmdPbsConfiguration set HG mode default value
  // 0 = HgModeDisabled, 2 = HgModeDgpu, 4 = HgModeEnabled
  //
  (**HgData).HgMode = SetHgModeValue (AmdPbsConfiguration);
  (**HgData).BootType = SystemConfiguration.BootType;


  return Status;
}
/**

  Base on system configuration to set Hybrid Graphics own HG mode variable.

  @param[in] SystemConfiguration   It's the setup variables (system configuration).

  @retval DisplayModeHgDisabled   HG mode should be set to disabled on internal graphics only platform,
                                  and should not power enable the discrete GPU.
  @retval DisplayModeHg           HG mode should be set to discrete GPU on discrete graphics only platform,
                                  if the discrete GPU used MXM interface, discrete GPU should be powered on.

**/
STATIC
PRIMARY_DISPLAY_SETTING
SetHgModeValue (
  IN AMD_PBS_SETUP_OPTION                     AmdPbsConfiguration
  )
{
  PRIMARY_DISPLAY_SETTING                     HgMode;

  //
  // Set HG mode default
  //
  HgMode = DisplayModeHgDisabled;

  //
  // Base on System Configuration variable data to change HG mode
  //
  if ((AmdPbsConfiguration.SpecialVgaFeature == 4) && (AmdPbsConfiguration.PrimaryVideoAdaptor == 1)) {
    HgMode = DisplayModeHg;
  }

  if (AmdPbsConfiguration.SpecialVgaFeature == 0) {
    HgMode = DisplayModeHgDisabled;
  }

  if (AmdPbsConfiguration.PrimaryVideoAdaptor == 2) {
    HgMode = DisplayModeDgpu;
  }

  return HgMode;
}

/**

  Waits for at least the given number of microseconds.

  @param[in] Microseconds - Desired length of time to wait

**/

VOID
EFIAPI
Stall (
  IN UINTN                                    Microseconds
  )
{
  MicroSecondDelay (Microseconds);
}
