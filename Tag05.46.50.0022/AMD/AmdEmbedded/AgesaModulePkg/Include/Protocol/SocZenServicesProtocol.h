/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _SOC_ZEN_SERVICES_PROTOCOL_H_
#define _SOC_ZEN_SERVICES_PROTOCOL_H_

//----------------------------------------------------
//
//  SoC Service Functions for Zen cores
//
//-------------------------------------------------------

///
/// Forward declaration for the AMD_SOC_ZEN_SERVICES_PROTOCOL.
///
typedef struct _AMD_SOC_ZEN_SERVICES_PROTOCOL AMD_SOC_ZEN_SERVICES_PROTOCOL;

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves information on the OPN's core / thread presence.
 *
 * @param[in]  This                           A pointer to the AMD_SOC_VALHALLA_SERVICES_PROTOCOL instance.
 * @param[out] CcdMap                         Pointer to the OPN CCD map.
 * @param[out] CoreDisMap                     Pointer to the OPN disabled core map.
 * @param[out] SmtEnable                      Pointer to whether or not the OPN supports SMT.
 *
 * @retval EFI_SUCCESS                        Presence info was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              All output parameter pointers are NULL.
 * @retval EFI_DEVICE_ERROR                   Presence info not found.
 *
 **/
typedef
EFI_STATUS
(EFIAPI *AMD_SOC_ZEN_SERVICES_GET_OPN_CORE_PRESENCE) (
     IN    AMD_SOC_ZEN_SERVICES_PROTOCOL                   *This,
     OUT   UINT32                                          *CcdMap,
     OUT   UINT32                                          *CoreDisMap,
     OUT   BOOLEAN                                         *SmtEnable
  );

///
/// When installed, the SoC Zen Services protocol produces a collection
/// of services that provide the zen CCX driver with SoC specific info.
///
typedef struct _AMD_SOC_ZEN_SERVICES_PROTOCOL {
  UINTN                                               Revision;                   ///< Revision Number
  AMD_SOC_ZEN_SERVICES_GET_OPN_CORE_PRESENCE          GetOpnCorePresence;         ///< Get information on the OPN's cores
} AMD_SOC_ZEN_SERVICES_PROTOCOL;

extern EFI_GUID gAmdSocZenServicesProtocolGuid;

// Current protocol revision
#define SOC_ZEN_SERVICES_PROTOCOL_REVISION   0x00

#endif


