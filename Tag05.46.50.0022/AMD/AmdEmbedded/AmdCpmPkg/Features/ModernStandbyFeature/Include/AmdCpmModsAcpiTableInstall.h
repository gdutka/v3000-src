/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#ifndef _AMD_CPM_MODS_ACPI_TABLE_INSTALL_DXE_H_
#define _AMD_CPM_MODS_ACPI_TABLE_INSTALL_DXE_H_

#include <AmdCpmDxe.h>
#include <Library/BaseLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/AmdCpmModsAcpiTableProtocol.h>
#include <Library/AmdBaseLib.h>

#define PLATFORM_LPI_ACPI_OEM_TABLE_ID             SIGNATURE_64('C', 'P', 'M', 'M', 'S', 'L', 'P', 'I')
#define PLATFORM_LPI_DIS_ACPI_OEM_TABLE_ID         SIGNATURE_64('M', 'S', 'L', 'P', 'I', 'D', 'I', 'S')

//
//_LPI
//
/*----------------------------------------------------------------------------------------*/
/**
 * boolean function to check LPI ssdt is enabled or not
 *
 *
 * @retval   TRUE is enabled, others do not.
 */
BOOLEAN
EFIAPI
AmdCpmIsLpiModsEnabled (
  IN UINT64 AcpiTableId
  );

/*----------------------------------------------------------------------------------------*/
/**
 * boolean function to check LPI DIS ssdt is enabled or not
 *
 *
 * @retval   TRUE is enabled, others do not.
 */
BOOLEAN
EFIAPI
AmdCpmIsLpiNonModsEnabled (
  IN UINT64 AcpiTableId
  );
/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override LPI SSDT dynamically  Update context
 *
 * This function is used to update ASL name.
 *
 * @retval        a pointer will return which contain the context
 */

VOID *
EFIAPI
AmdCpmLpiContext (
  IN      AMD_CPM_TABLE_PROTOCOL  *This
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override LPI SSDT Table
 *
 * This function is used to update ASL name.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmLpiCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );



#endif