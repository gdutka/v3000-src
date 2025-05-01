/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _SOC_ZEN3_SERVICES_PROTOCOL_H_
#define _SOC_ZEN3_SERVICES_PROTOCOL_H_
#include <AMD.h>

//----------------------------------------------------
//
//  SoC Service Functions for Zen3 cores
//
//-------------------------------------------------------

///
/// Forward declaration for the AMD_SOC_ZEN3_SERVICES_PROTOCOL.
///
typedef struct _AMD_SOC_ZEN3_SERVICES_PROTOCOL AMD_SOC_ZEN3_SERVICES_PROTOCOL;

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves the value for the OPN's maximum frequency indicator.
 *
 * @param[in]  This                           A pointer to the AMD_SOC_ZEN3_SERVICES_PROTOCOL instance.
 * @param[out] FMax                           Pointer to the raw FMax value.
 *
 * @retval EFI_SUCCESS                        FMax is valid.
 * @retval EFI_INVALID_PARAMETER              FMax is NULL.
 * @retval EFI_DEVICE_ERROR                   FMax value not found.
 *
 **/
typedef
EFI_STATUS
(EFIAPI *AMD_SOC_ZEN3_SERVICES_GET_OPN_FMAX) (
     IN    AMD_SOC_ZEN3_SERVICES_PROTOCOL                  *This,
     OUT   UINT32                                          *FMax
  );

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves information on the OPN's core / thread presence.
 *
 * @param[in]  This                           A pointer to the AMD_SOC_ZEN3_SERVICES_PROTOCOL instance.
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
(EFIAPI *AMD_SOC_ZEN3_SERVICES_GET_OPN_CORE_PRESENCE) (
     IN    AMD_SOC_ZEN3_SERVICES_PROTOCOL                  *This,
     OUT   UINT32                                          *CcdMap,
     OUT   UINT32                                          *CoreDisMap,
     OUT   BOOLEAN                                         *SmtEnable
  );

/*---------------------------------------------------------------------------------------*/

#define SOC_OC_MSG_ID_MAP_REVISION   0x01

typedef struct {
  UINT32  Revision;                           ///< Revision Number
  UINT32  EnableOverclocking;                 ///< SMU service ID for enable overclocking
  UINT32  SetOverclockVID;                    ///< SMU service ID for set overclock VID
  UINT32  SetOverclockFreqAllCores;           ///< SMU service ID for set overclock frequency for all cores
} SOC_ZEN3_OC_MSG_ID_TABLE;

/**
 * This service retrieves information on the SOC overclocking SMU message ID map
 *
 * @param[in]  This                           A pointer to the AMD_SOC_ZEN3_SERVICES_PROTOCOL instance.
 * @param[out] OcMsgIdTable                   Pointer to the OC message ID table.
 *
 * @retval EFI_SUCCESS                        OcMsgIdTable info was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              Input protocol instance does not match.
 * @retval EFI_UNSUPPORTED                    Unsupported service for this SOC
 *
 **/
typedef
EFI_STATUS
(EFIAPI *AMD_SOC_ZEN3_SERVICES_GET_SOC_OC_SMU_MSG_ID) (
     IN    AMD_SOC_ZEN3_SERVICES_PROTOCOL                   *This,
     OUT   SOC_ZEN3_OC_MSG_ID_TABLE                        **OcMsgIdTable
  );


///
/// When installed, the SoC Zen3 Services protocol produces a collection
/// of services that provide the Zen3 CCX driver with SoC specific info.
///
typedef struct _AMD_SOC_ZEN3_SERVICES_PROTOCOL {
  UINTN                                               Revision;                   ///< Revision Number
  AMD_SOC_ZEN3_SERVICES_GET_OPN_FMAX                  GetOpnFmax;                 ///< Get the FMax value
  AMD_SOC_ZEN3_SERVICES_GET_OPN_CORE_PRESENCE         GetOpnCorePresence;         ///< Get information on the OPN's cores
  AMD_SOC_ZEN3_SERVICES_GET_SOC_OC_SMU_MSG_ID         GetOcSmuMsgId;              ///< Get SMU FW message IDs for overclocking
} AMD_SOC_ZEN3_SERVICES_PROTOCOL;

extern EFI_GUID gAmdSocZen3ServicesProtocolGuid;

// Current protocol revision
#define SOC_ZEN3_SERVICES_PROTOCOL_REVISION   0x01

#endif


