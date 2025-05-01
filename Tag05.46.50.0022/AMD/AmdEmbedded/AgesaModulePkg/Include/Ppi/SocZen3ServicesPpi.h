/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _SOC_ZEN3_SERVICES_PPI_H_
#define _SOC_ZEN3_SERVICES_PPI_H_
#include <AMD.h>

//----------------------------------------------------
//
//  SoC Service Functions for Zen3 cores
//
//-------------------------------------------------------

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
typedef
EFI_STATUS
(EFIAPI *AMD_PEI_SOC_ZEN3_SERVICES_GET_OPN_CORE_PRESENCE) (
  IN       EFI_PEI_SERVICES                               **PeiServices,
  IN       UINT32                                           SystemDieNumber,
     OUT   UINT32                                          *CcdEnableMap,
     OUT   UINT32                                          *CcdDisableMap,
     OUT   UINT32                                          *CoreDisMap,
     OUT   BOOLEAN                                         *SmtEnable
  );

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
typedef
UINT32
(EFIAPI *AMD_PEI_SOC_ZEN3_SERVICES_GET_CCD0_SW_DOWNCORE_ADDR) (
  IN       EFI_PEI_SERVICES                               **PeiServices
  );

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
typedef
BOOLEAN
(EFIAPI *AMD_PEI_SOC_ZEN3_SERVICES_IS_1CCD_SUPPORTED) (
  IN       EFI_PEI_SERVICES                               **PeiServices
  );

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
typedef
BOOLEAN
(EFIAPI *AMD_PEI_SOC_ZEN3_SERVICES_IS_S3_SUPPORTED) (
  IN       EFI_PEI_SERVICES                               **PeiServices
  );

/*---------------------------------------------------------------------------------------*/
/**
 * This service returns whether or not the SoC supports game mode.
 *
 * @param[in]  PeiServices                    Pointer to UEFI PEI services.
 *
 * @retval TRUE                               game mode is supported by the SoC
 * @retval FALSE                              game mode is not supported by the SoC
 *
 **/
typedef
BOOLEAN
(EFIAPI *AMD_PEI_SOC_ZEN3_SERVICES_IS_GAME_MODE_SUPPORTED) (
  IN       EFI_PEI_SERVICES                               **PeiServices
  );

///
/// When installed, the SoC Zen3 Services PPI produces a collection of
/// services that provide the Zen3 CCX PEIM with SoC specific info.
///
typedef struct _AMD_PEI_SOC_ZEN3_SERVICES_PPI {
  UINTN                                               Revision;                   ///< Revision Number
  AMD_PEI_SOC_ZEN3_SERVICES_GET_OPN_CORE_PRESENCE     GetOpnCorePresence;         ///< Get information on the OPN's cores
  AMD_PEI_SOC_ZEN3_SERVICES_GET_CCD0_SW_DOWNCORE_ADDR GetCcd0SoftDowncoreAddress; ///< Get the SMN address for CCD0's soft downcore register
  AMD_PEI_SOC_ZEN3_SERVICES_IS_1CCD_SUPPORTED         IsOneCcdModeSupported;      ///< Is down coring to 1 enabled CCD supported
  AMD_PEI_SOC_ZEN3_SERVICES_IS_S3_SUPPORTED           IsS3Supported;              ///< Is the S3 sleep state supported by the SoC
  AMD_PEI_SOC_ZEN3_SERVICES_IS_GAME_MODE_SUPPORTED    IsGameModeSupported;        ///< Is game mode supported by the SoC
  BOOLEAN                                             SocDoesNotHavePhysicalCcds; ///< FALSE indicates IOD+CCD model, TRUE indicates APU
} AMD_PEI_SOC_ZEN3_SERVICES_PPI;

extern EFI_GUID gAmdSocZen3ServicesPpiGuid;

// Current PPI revision
#define SOC_ZEN3_SERVICES_PPI_REVISION   0x00

#endif


