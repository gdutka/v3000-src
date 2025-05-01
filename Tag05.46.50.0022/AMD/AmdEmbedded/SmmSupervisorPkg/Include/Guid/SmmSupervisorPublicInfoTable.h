/*
*******************************************************************************

Copyright(C) 2021 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#ifndef _SMM_SUPERVISOR_PUBLIC_INFO_TABLE_H_
#define _SMM_SUPERVISOR_PUBLIC_INFO_TABLE_H_

#define SMM_SUPERVISOR_PUBLIC_INFO_TABLE_GUID {\
  0xfcb34add, 0x53eb, 0x4d8b, { 0xae, 0x9c, 0x30, 0x8c, 0x38, 0xf6, 0xfe, 0xfd } \
}

//
// The SmmSupervisor public info table contains the public info to share with external SMM driver
//
// This table is installed to SMST as SMM configuration table.
//
// This table is stored in the memory with the type EfiRuntimeServicesData
//
typedef struct {
  UINT32                Version;
  UINT32                SmmIsolationEnabled;
  UINT32                Reserved1;
  UINT32                Reserved2;
} SMM_SUPERVISOR_PUBLIC_INFO_TABLE;

#define SMM_SUPERVISOR_PUBLIC_INFO_TABLE_VERSION  0x00000001

extern EFI_GUID gSmmSupervisorPublicInfoTableGuid;

#endif
