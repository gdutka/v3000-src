/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <Features/ModernStandbyFeature/Include/AmdCpmModsAcpiTableInstall.h>

AMD_CPM_MS_SSDT_INSTALL_TABLE  mCommonMsSsdtInstallTable[] = {
  {&gEfiCallerIdGuid, PLATFORM_LPI_ACPI_OEM_TABLE_ID,          AmdCpmIsLpiModsEnabled,        AmdCpmLpiCallBack,  AmdCpmLpiContext},
  {&gEfiCallerIdGuid, PLATFORM_LPI_DIS_ACPI_OEM_TABLE_ID,      AmdCpmIsLpiNonModsEnabled,     AmdCpmLpiCallBack,  AmdCpmLpiContext},
};

#define PLATFORM_LPI_CONTEXT_BUFFER_SIZE  1

//
//_LPI
//
/*----------------------------------------------------------------------------------------*/
/**
 * boolean function to check LPI ssdt is needed or not
 *
 *
 * @retval   TRUE is needed, others do not.
 */
BOOLEAN
EFIAPI
AmdCpmIsLpiModsEnabled (
  IN UINT64 AcpiTableId
  )
{
  return (PcdGetBool (PcdModernStandbyEnable) && PcdGetBool (PcdPlatformLpiEn));
}

/*----------------------------------------------------------------------------------------*/
/**
 * boolean function to check LPI DIS ssdt is needed or not
 *
 *
 * @retval   TRUE is needed, others do not.
 */
BOOLEAN
EFIAPI
AmdCpmIsLpiNonModsEnabled (
  IN UINT64 AcpiTableId
  )
{
  return ((!PcdGetBool (PcdModernStandbyEnable)) && PcdGetBool (PcdPlatformLpiEn));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override LPI dynamically  Update context
 *
 * This function is used to update ASL name.
 *
 * @retval        a pointer will return which contain the context
 */

VOID *
EFIAPI
AmdCpmLpiContext (
  IN      AMD_CPM_TABLE_PROTOCOL  *This
  )
{
  UINT32                                *BufferPtr;
  EFI_STATUS                            Status;

  Status = gBS->AllocatePool ( 
                  EfiBootServicesData,
                  sizeof(UINT32) * PLATFORM_LPI_CONTEXT_BUFFER_SIZE,
                  (VOID**)&BufferPtr 
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }


  LibAmdMemCopy ((VOID *) BufferPtr,
                 (VOID *) PcdGetPtr (PcdAmdAcpiCpuSsdtProcessorContainerName),
                 AsciiStrnLenS (PcdGetPtr (PcdAmdAcpiCpuSsdtProcessorContainerName), 4),
                 NULL);

  DEBUG((DEBUG_INFO, "AmdCpmLpiContext *BufferPtr = 0x%x\n", *BufferPtr));

  return (VOID *)BufferPtr;
}
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
  )
{
  UINT32 *BufferPtr;
  
  BufferPtr = (UINT32 *)Context;
  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('P', 'L', 'T', 'F')):
    if (*(BufferPtr)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr);
    }
    break;
  }

  return FALSE;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM ModernStandby ACPI table install DXE driver
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmModernStandbyAcpiTableInstallEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;

  AMD_CPM_MS_SSDT_TABLE_PROTOCOL          *pAmdCpmMsSsdtTableProtocol;
  EFI_HANDLE                              Handle;
  UINTN                                   Index;
  DEBUG ((DEBUG_INFO, "%a(%d)-Start\n", __FUNCTION__, __LINE__));

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->LocateProtocol (&gAmdCpmOemModsSsdtTableProtocol, NULL, (VOID**)&pAmdCpmMsSsdtTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "%a(%d)- locate %g  Status = %r \n", __FUNCTION__, __LINE__, &gAmdCpmOemModsSsdtTableProtocol, Status));
    return Status;
  }
  for (Index = 0; Index < ARRAY_SIZE (mCommonMsSsdtInstallTable); Index ++) {
    Status = pAmdCpmMsSsdtTableProtocol->Register (
                                           pAmdCpmMsSsdtTableProtocol,
                                           &mCommonMsSsdtInstallTable[Index],
                                           &Handle
                                           );
    {//DEBUG
      UINT8        DbgIdTableName[9];
      gBS->SetMem (DbgIdTableName, 9, 0);
      gBS->CopyMem (DbgIdTableName,  (UINT8 *)&mCommonMsSsdtInstallTable[Index].AcpiOemTableId, 8);
      DEBUG ((DEBUG_INFO, "%a(%d)- Register table id %a status = %r\n", __FUNCTION__, __LINE__, DbgIdTableName, Status));
    }
  }
  DEBUG((DEBUG_INFO, "%a(%d)-End\n", __FUNCTION__, __LINE__));
  return EFI_SUCCESS;
}
