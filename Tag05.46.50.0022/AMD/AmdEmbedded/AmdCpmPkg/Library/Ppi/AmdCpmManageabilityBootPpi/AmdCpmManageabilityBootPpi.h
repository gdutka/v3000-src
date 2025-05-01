/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_CPM_MANAGEABILITY_BOOT_PPI_H_
#define _AMD_CPM_MANAGEABILITY_BOOT_PPI_H_

#include <AmdCpmBase.h>

//
// GUID definition
//
#define AMD_CPM_MANAGEABILITY_BOOT_PPI_GUID \
  { 0xd1063bff, 0x417b, 0x45c2, 0xb5, 0xbc, 0x60, 0x39, 0xb9, 0xfb, 0x12, 0xf7 }

extern EFI_GUID gAmdCpmManageabilityBootPpiGuid;


#define MANOS_TARGET_MAN_OS         0x37
#define MANOS_TARGET_RST_HOST_OS    0xFE
#define MANOS_TARGET_HOST_OS        0xFF

typedef struct {
  UINT8 DisableSata   : 1;
  UINT8 DisableXhci0  : 1;
  UINT8 DisableXhci1  : 1;
  UINT8 DisablePcie   : 1;
  UINT8 DisableGFX    : 1;
  UINT8 DisableDDIs   : 1;
  UINT8 DisableAudio  : 1;
  UINT8 DisableFchMisc  : 1;
} PcdOverrideBitMapField;

typedef union {
  UINT8 Value;
  PcdOverrideBitMapField BitMap;
} ManageabilityPcdOverrideBitMap;

/**
  Check if the PBS option (Wireless Manageability Support) is enabled or not

  @retval TRUE          Manageability OS booting is supported
  @retval FALSE         Manageability OS booting is NOT supported
**/
typedef
BOOLEAN
(EFIAPI *AMD_CPM_MANAGEABILITY_IS_MAN_OS_BOOT_SUPPORTED) (
);

/**
  Check with EC whether the system should boot to manageability OS or not.

  @retval 1             The system should boot to manageability OS
  @retval other value   The system should not boot to manageablity OS
**/
typedef
UINT8
(EFIAPI *AMD_CPM_MANAGEABILITY_CHECK_MAN_OS_BOOT_FLAG) (
  IN  CPM_PEI_SERVICES    **PeiServices
);

/**
  Set manageablity OS boot flag to EC

  @param[in] PeiServices            Pointer's pointer to CPM_PEI_SERVICES
  @param[in] Enable                 If Enable is TURE, set the manageability OS boot flag to true
                                    If Enable is FALSE, set the manageability OS boot flag to false

  @retval EFI_SUCCESS               Set the manageability OS prepare flag successfully
**/
typedef
VOID
(EFIAPI *AMD_CPM_MANAGEABILITY_SET_MAN_OS_BOOT_FLAG) (
  IN  CPM_PEI_SERVICES    **PeiServices,
  IN  BOOLEAN             Enable
);

/**
  Set manageablity OS prepare flag to EC

  @param[in] PeiServices            Pointer's pointer to CPM_PEI_SERVICES

  @retval EFI_SUCCESS               Set the manageability OS prepare flag successfully
**/
typedef
VOID
(EFIAPI *AMD_CPM_MANAGEABILITY_SET_MAN_OS_PREPARE_FLAG) (
  IN  CPM_PEI_SERVICES    **PeiServices
);

/**
  Get the PCD override flag from EC

  @param[in] PeiServices            Pointer's pointer to CPM_PEI_SERVICES
  @param[out] PcdOverrideFlag       The PCD override flag

  @retval EFI_SUCCESS               Get the PCD override flag successfully
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CPM_MANAGEABILITY_READ_PCD_OVERRIDE_FLAG) (
  IN  CPM_PEI_SERVICES    **PeiServices,
  OUT ManageabilityPcdOverrideBitMap *PcdOverrideFlag
);

/**
  Apply the PCD override flag

  @param[in] WlanDeviceId        The GPIO device ID of Wlan

  @retval EFI_SUCCESS             Get the GPIO device ID of Wlan successfully
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CPM_MANAGEABILITY_APPLY_PCD_OVERRIDE_FLAG) (
  IN  CPM_PEI_SERVICES    **PeiServices,
  IN  ManageabilityPcdOverrideBitMap PcdOverrideFlag
);

//
// Protocol Interface Structure
//
typedef struct _AMD_CPM_MANAGEABILITY_BOOT_PPI {
  UINT32  Revision;
  AMD_CPM_MANAGEABILITY_IS_MAN_OS_BOOT_SUPPORTED    IsManOsBootSupported;
  AMD_CPM_MANAGEABILITY_CHECK_MAN_OS_BOOT_FLAG      CheckManOsBootFlag;
  AMD_CPM_MANAGEABILITY_SET_MAN_OS_BOOT_FLAG        SetManOsBootFlag;
  AMD_CPM_MANAGEABILITY_SET_MAN_OS_PREPARE_FLAG     SetManOsPrepareFlag;
  AMD_CPM_MANAGEABILITY_READ_PCD_OVERRIDE_FLAG      ReadPcdOverrideFlag;
  AMD_CPM_MANAGEABILITY_APPLY_PCD_OVERRIDE_FLAG     ApplyPcdOverrideFlag;
} AMD_CPM_MANAGEABILITY_BOOT_PPI;

#define  AMD_CPM_MANAGEABILITY_BOOT_PPI_REVISION 0x00

#endif //_AMD_CPM_MANAGEABILITY_BOOT_PPI_H_
