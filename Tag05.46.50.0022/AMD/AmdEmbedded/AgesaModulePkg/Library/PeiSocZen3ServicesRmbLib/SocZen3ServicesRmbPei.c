/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Ppi/SocZen3ServicesPpi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Filecode.h>
#include "SocZen3ServicesRmbPei.h"
#include <AGESA.h>

#define FILECODE LIBRARY_PEISOCZEN3SERVICESRMBLIB_SOCZEN3SERVICESRMBPEI_FILECODE

STATIC AMD_PEI_SOC_ZEN3_SERVICES_PPI  mSocZen3ServicesRmbPpi = {
  SOC_ZEN3_SERVICES_PPI_REVISION,
  SocZen3RmbGetOpnCorePresence,
  SocZen3RmbGetCcd0SoftDowncoreAddress,
  SocZen3RmbIsOneCcdModeSupported,
  SocZen3RmbIsS3Supported,
  SocZen3RmbIsGameModeSupported,
  TRUE
};

STATIC EFI_PEI_PPI_DESCRIPTOR mSocZen3ServicesRmbPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdSocZen3ServicesPpiGuid,
  &mSocZen3ServicesRmbPpi
};


EFI_STATUS
EFIAPI
SocZen3ServicesPpiInstall (
  IN       CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  // Install SoC Zen3 Services
  Status = (**PeiServices).InstallPpi (PeiServices, &mSocZen3ServicesRmbPpiList);

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves information on the OPN's core / thread presence.
 *
 * @param[in]  PeiServices                    Pointer to UEFI PEI services.
 * @param[in]  SystemDieNumber                Logical system die number to check.
 * @param[out] CcdEnableMap                   Pointer to the OPN enabled CCD map.
 * @param[out] CcdDisableMap                  Pointer to the OPN disabled CCD map.
 * @param[out] CoreDisMap                     Pointer to the OPN disabled core map.
 * @param[out] SmtEnable                      Pointer to whether or not the OPN supports SMT.
 *
 * @retval EFI_SUCCESS                        Presence info was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              SystemDieNumber is invalid.
 * @retval EFI_INVALID_PARAMETER              All output parameter pointers are NULL.
 * @retval EFI_DEVICE_ERROR                   Presence info not found.
 *
 **/
EFI_STATUS
SocZen3RmbGetOpnCorePresence (
  IN       EFI_PEI_SERVICES                               **PeiServices,
  IN       UINT32                                           SystemDieNumber,
     OUT   UINT32                                          *CcdEnableMap,
     OUT   UINT32                                          *CcdDisableMap,
     OUT   UINT32                                          *CoreDisMap,
     OUT   BOOLEAN                                         *SmtEnable
  )
{
  EFI_STATUS                      Status;
  PEI_AMD_NBIO_SMU_SERVICES_PPI  *NbioSmuServices;
  UINT32                          CoreFuse;

  if ((SystemDieNumber != 0) || ((CcdEnableMap == NULL) && (CcdDisableMap == NULL) && (CoreDisMap == NULL) && (SmtEnable == NULL))) {
    Status = EFI_INVALID_PARAMETER;
  } else if ((*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &NbioSmuServices) == EFI_SUCCESS) {
    if (CcdEnableMap != NULL) {
      // Enable virtual CCD 0
      *CcdEnableMap = 0x00000001;
    }
    if (CcdDisableMap != NULL) {
      *CcdDisableMap = 0x00000000;
    }
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, 0, 0x5D4DC, &CoreFuse);
    if (SmtEnable != NULL) {
      *SmtEnable = (BOOLEAN) ((CoreFuse & BIT6) != 0);
    }
    if (CoreDisMap != NULL) {
      *CoreDisMap = (CoreFuse >> 7) & 0xFF;
    }
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
  }


  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves the SMN address for CCD0's (physical or virtual) soft downcore
 * register.
 *
 * @param[in]  PeiServices                    Pointer to UEFI PEI services.
 *
 * @return                     The SMN address for CCD0's soft downcore register.
 *
 **/
UINT32
SocZen3RmbGetCcd0SoftDowncoreAddress (
  IN       EFI_PEI_SERVICES                               **PeiServices
  )
{
  return 0x5A860;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service returns whether or not the SoC supports disabling all but one CCD.
 *
 * @param[in]  PeiServices                    Pointer to UEFI PEI services.
 *
 * @retval TRUE                               Downing to 1 CCD is supported
 * @retval FALSE                              Downing to 1 CCD is not supported
 *
 **/
BOOLEAN
SocZen3RmbIsOneCcdModeSupported (
  IN       EFI_PEI_SERVICES                               **PeiServices
  )
{
  return TRUE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service returns whether or not the SoC supports the S3 sleep state.
 *
 * @param[in]  PeiServices                    Pointer to UEFI PEI services.
 *
 * @retval TRUE                               S3 is supported by the SoC
 * @retval FALSE                              S3 is not supported by the SoC
 *
 **/
BOOLEAN
SocZen3RmbIsS3Supported (
  IN       EFI_PEI_SERVICES                               **PeiServices
  )
{
  return TRUE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service returns whether or not the SoC supports game mode.
 *
 * @param[in]  PeiServices                    Pointer to UEFI PEI services.
 *
 * @retval TRUE                               Game mode is supported by the SoC
 * @retval FALSE                              Game mode is not supported by the SoC
 *
 **/
BOOLEAN
SocZen3RmbIsGameModeSupported (
  IN       EFI_PEI_SERVICES   **PeiServices
  )
{
  return FALSE;
}
