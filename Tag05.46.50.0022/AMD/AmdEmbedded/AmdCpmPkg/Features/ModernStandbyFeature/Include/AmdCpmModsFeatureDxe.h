/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#ifndef _AMD_CPM_MODERN_STANDBY_DXE_H_
#define _AMD_CPM_MODERN_STANDBY_DXE_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <AmdCpmDxe.h>
#include <Protocol/AmdCpmModsAcpiTableProtocol.h>

typedef struct {
  UINT32                               Signature;
  LIST_ENTRY                           Link;
  EFI_HANDLE                           Handle;
  AMD_CPM_MS_SSDT_INSTALL_TABLE        *pTable;
} AMD_MS_SSDT_INSTALL_TABLE_INSTANCE;
#define AMD_MS_SSDT_INSTALL_TABLE_INSTANCE_SIGNATURE   SIGNATURE_32('C', 'M', 'S', 'T')
#define AMD_MS_SSDT_INSTALL_TABLE_INSTANCE_FROM_LINK(a)  CR (a, AMD_MS_SSDT_INSTALL_TABLE_INSTANCE, Link, AMD_MS_SSDT_INSTALL_TABLE_INSTANCE_SIGNATURE)
/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM ModernStandby SSDT Init DXE driver
 *
 * @param[in]     pThis              Pointer to the gAmdCpmOemModsSsdtTableProtocol protocol
 * @param[in]     pTable             Pointer to the table to been installed
 * @param[out]    pDispatchHandle    Pointer to the out ssdt install table handle
 *
 * @retval        EFI_SUCCESS    the ssdt table is register successfully
 */
EFI_STATUS
EFIAPI 
AmdCpmMsSsdtRegister (
  IN       AMD_CPM_MS_SSDT_TABLE_PROTOCOL       *pThis,
  IN       AMD_CPM_MS_SSDT_INSTALL_TABLE        *pTable,
  OUT      EFI_HANDLE                           *pDispatchHandle
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM ModernStandby SSDT Init DXE driver
 *
 * @param[in]     pThis              Pointer to the gAmdCpmOemModsSsdtTableProtocol protocol
 * @param[in]     pDispatchHandle    Pointer to the ssdt install table handle
 *
 * @retval        EFI_SUCCESS    the ssdt table is unregister successfully
 */
EFI_STATUS
EFIAPI 
AmdCpmMsSsdtUnRegister (
  IN       AMD_CPM_MS_SSDT_TABLE_PROTOCOL       *pThis,
  IN       EFI_HANDLE                           DispatchHandle
  );
#endif