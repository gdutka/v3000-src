/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _SOC_ZEN_SERVICES_PPI_H_
#define _SOC_ZEN_SERVICES_PPI_H_

//----------------------------------------------------
//
//  SoC Service Functions for Zen cores
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
(EFIAPI *AMD_PEI_SOC_ZEN_SERVICES_GET_OPN_CORE_PRESENCE) (
  IN       CONST EFI_PEI_SERVICES                         **PeiServices,
  IN       UINT32                                           SystemDieNumber,
     OUT   UINT32                                          *CcdEnableMap,
     OUT   UINT32                                          *CcdDisableMap,
     OUT   UINT32                                          *CoreDisMap,
     OUT   BOOLEAN                                         *SmtEnable
  );

///
/// When installed, the SoC Zen Services PPI produces a collection of
/// services that provide the zen CCX PEIM with SoC specific info.
///
typedef struct _AMD_PEI_SOC_ZEN_SERVICES_PPI {
  UINTN                                              Revision;                        ///< Revision Number
  AMD_PEI_SOC_ZEN_SERVICES_GET_OPN_CORE_PRESENCE     GetOpnCorePresence;              ///< Get information on the OPN's cores
} AMD_PEI_SOC_ZEN_SERVICES_PPI;

extern EFI_GUID gAmdSocZenServicesPpiGuid;

// Current PPI revision
#define SOC_ZEN_SERVICES_PPI_REVISION   0x00

#endif


