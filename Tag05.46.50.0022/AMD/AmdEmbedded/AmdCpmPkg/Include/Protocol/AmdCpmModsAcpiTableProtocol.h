/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_CPM_MS_SSDT_TABLE_PROTOCOL_H_
#define _AMD_CPM_MS_SSDT_TABLE_PROTOCOL_H_

#include <Uefi.h>
#include <AmdCpmBase.h>
typedef struct  _AMD_CPM_MS_SSDT_TABLE_PROTOCOL AMD_CPM_MS_SSDT_TABLE_PROTOCOL;
#pragma pack(push, 1)
typedef struct {
  EFI_GUID                        *SsdtFfsGuid;
  UINT64                          AcpiOemTableId;
  BOOLEAN                         (EFIAPI *AmdCpmIsSsdtNeeded) (UINT64 AcpiOemTableId);
  AMD_CPM_ADDSSDTCALLBACK_FN      AmdCpmMsSsdtCallBackFunction;
  VOID* (EFIAPI *AmdCpmMsSsdtCallBackContext) (AMD_CPM_TABLE_PROTOCOL  *This);
} AMD_CPM_MS_SSDT_INSTALL_TABLE;
#pragma pack(pop)

typedef EFI_STATUS (EFIAPI *AMD_CPM_MS_SSDT_REGISTER) (
  IN  AMD_CPM_MS_SSDT_TABLE_PROTOCOL       *This,
  IN  AMD_CPM_MS_SSDT_INSTALL_TABLE        *pTable,
  OUT EFI_HANDLE                           *DispatchHandle
);

typedef EFI_STATUS (EFIAPI *AMD_CPM_MS_SSDT_UNREGISTER) (
  IN  AMD_CPM_MS_SSDT_TABLE_PROTOCOL      *This,
  IN       EFI_HANDLE                          DispatchHandle
);

struct  _AMD_CPM_MS_SSDT_TABLE_PROTOCOL {
  AMD_CPM_MS_SSDT_REGISTER    Register;
  AMD_CPM_MS_SSDT_UNREGISTER  UnRegister;
};

#endif
