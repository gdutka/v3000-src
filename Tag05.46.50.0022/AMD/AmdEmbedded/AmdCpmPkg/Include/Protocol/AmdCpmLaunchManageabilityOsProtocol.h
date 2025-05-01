/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CPM_LAUNCH_MANAGEABILITY_OS_PROTOCOL_H_
#define _AMD_CPM_LAUNCH_MANAGEABILITY_OS_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

//
// Extern the GUID for protocol users
//
extern EFI_GUID gAmdCpmLaunchManageabilityOsProtocolGuid;

typedef struct _AMD_CPM_LAUNCH_ANAGEABILITY_OS_PROTOCOL AMD_CPM_LAUNCH_ANAGEABILITY_OS_PROTOCOL;

/**
  Launch the manageability OS when it is ready to be launched

  @retval                        This function supposedly doesn't return if the manageability OS
                                 is launched successfully
  @retval EFI_UNSUPPORTED        EC indicates that the system should not boot to manageability OS,
                                 or protocol instance is not found
  @retval EFI_NOT_FOUND          gAmdCpmManageabilityProtocolGuid is not installed

**/
typedef
EFI_STATUS
(EFIAPI *LAUNCH_MANAGEABILITY_OS) (
);

//
// Protocol Interface Structure
//
typedef struct _AMD_CPM_LAUNCH_ANAGEABILITY_OS_PROTOCOL
{
  UINTN                     Revision;
  LAUNCH_MANAGEABILITY_OS   LaunchManageabilityOs;
} AMD_CPM_LAUNCH_ANAGEABILITY_OS_PROTOCOL;

#define AMD_CPM_LAUNCH_MANAGEABILITY_OS_PROTOCOL_REVISION 0x00

#endif // _AMD_CPM_LAUNCH_MANAGEABILITY_OS_PROTOCOL_H_


