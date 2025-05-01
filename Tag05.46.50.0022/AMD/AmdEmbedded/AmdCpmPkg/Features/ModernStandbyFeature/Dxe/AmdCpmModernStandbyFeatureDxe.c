/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <Features/ModernStandbyFeature/Include/AmdCpmModsFeatureDxe.h>

LIST_ENTRY                      gAmdCpmMsSsdtInstanceList = INITIALIZE_LIST_HEAD_VARIABLE (gAmdCpmMsSsdtInstanceList);
AMD_CPM_MS_SSDT_TABLE_PROTOCOL      gAmdCpmMsSsdtTableProtocol = {
  AmdCpmMsSsdtRegister,
  AmdCpmMsSsdtUnRegister
};
/*----------------------------------------------------------------------------------------*/
/**
 * AMD CPM ModernStandby SSDT table regiser
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
  )
{
  AMD_MS_SSDT_INSTALL_TABLE_INSTANCE           *pInstance;
  EFI_STATUS                                   Status;
  if ((pThis == NULL) || (pTable == NULL) ) { //safety check
    return EFI_INVALID_PARAMETER;
  }
  //
  // Allcoate new instance for ssdt install table
  //
  Status = gBS->AllocatePool ( 
                  EfiBootServicesData,
                  sizeof (AMD_MS_SSDT_INSTALL_TABLE_INSTANCE),
                  (VOID**)&pInstance 
                  );
  if (pInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  pInstance->Signature = AMD_MS_SSDT_INSTALL_TABLE_INSTANCE_SIGNATURE;
  //
  // Copy current table into list
  //
  Status = gBS->AllocatePool ( 
                  EfiBootServicesData,
                  sizeof (AMD_MS_SSDT_INSTALL_TABLE_INSTANCE),
                  (VOID**)&pInstance->pTable 
                  );
  if (pInstance->pTable == NULL) {
    Status =  EFI_OUT_OF_RESOURCES;
    goto ErrExit2;
  }
  //
  // Allocate and Copy pTable SSDT FFS FILE GUID
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (EFI_GUID),
                  (VOID**)&pInstance->pTable->SsdtFfsGuid
                  );
  if (pInstance->pTable->SsdtFfsGuid == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrExit1;
  }
  gBS->CopyMem ((UINT8 *)pInstance->pTable->SsdtFfsGuid, (UINT8 *)pTable->SsdtFfsGuid, sizeof (EFI_GUID));
  //
  // Init function in tables
  //
  pInstance->pTable->AcpiOemTableId               = pTable->AcpiOemTableId;
  pInstance->pTable->AmdCpmIsSsdtNeeded           = pTable->AmdCpmIsSsdtNeeded;
  pInstance->pTable->AmdCpmMsSsdtCallBackFunction = pTable->AmdCpmMsSsdtCallBackFunction;
  pInstance->pTable->AmdCpmMsSsdtCallBackContext  = pTable->AmdCpmMsSsdtCallBackContext;

  //
  // Insert into the root.
  //
  InsertTailList (&gAmdCpmMsSsdtInstanceList, &pInstance->Link);

  //
  //initialize the dispatch handle
  //
  *pDispatchHandle = &pInstance->Handle;
  pInstance->Handle = *pDispatchHandle;

  return EFI_SUCCESS;

ErrExit1:
  if (pInstance->pTable->SsdtFfsGuid != NULL) {
    gBS->FreePool (pInstance->pTable->SsdtFfsGuid);
  }
ErrExit2:
  if (pInstance->pTable != NULL) {
    gBS->FreePool (pInstance->pTable);
  }
  if (pInstance != NULL) {
    gBS->FreePool (pInstance);
  }
  return Status;
}
/*----------------------------------------------------------------------------------------*/
/**
 * EAMD CPM ModernStandby SSDT table unregiser
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
  )
{

  AMD_MS_SSDT_INSTALL_TABLE_INSTANCE    *pInstance = NULL;
  LIST_ENTRY                            *Link;
  //
  // Dispatch all instace in the table list
  // And found the matched handle
  //
  Link  = GetFirstNode (&gAmdCpmMsSsdtInstanceList);
  while (!IsNull (&gAmdCpmMsSsdtInstanceList, Link)) {
    pInstance = AMD_MS_SSDT_INSTALL_TABLE_INSTANCE_FROM_LINK (Link);
    if (pInstance != NULL) {
      if (pInstance->Handle == DispatchHandle) {
        RemoveEntryList (Link);
        //
        // Free resource
        //
        if (pInstance->pTable != NULL) {
          if (pInstance->pTable->SsdtFfsGuid != NULL) {
            gBS->FreePool (pInstance->pTable->SsdtFfsGuid);
          }
          gBS->FreePool (pInstance->pTable);
        }
        gBS->FreePool (pInstance);
        return EFI_SUCCESS;
      }
    }
    Link = GetNextNode (&gAmdCpmMsSsdtInstanceList, Link);
  }

  return EFI_NOT_FOUND;
}
/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM Modernstandby SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmMsSsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{

  EFI_STATUS                            Status;
  VOID                                  *Buffer;
  AMD_CPM_TABLE_PROTOCOL                *CpmTableProtocolPtr;
  AMD_MS_SSDT_INSTALL_TABLE_INSTANCE    *pInstance;
  LIST_ENTRY                            *Link;
  UINTN                                 Index = 0;
  DEBUG((DEBUG_INFO, "%a(%d)-start\n", __FUNCTION__, __LINE__));

  Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "%a(%d)-gAmdCpmTableProtocolGuid is not located\n", __FUNCTION__, __LINE__));
    return;
  }
  //
  // Dispatch all instace in the table list
  //
  Link  = GetFirstNode (&gAmdCpmMsSsdtInstanceList);
  while (!IsNull (&gAmdCpmMsSsdtInstanceList, Link)) {
    pInstance = AMD_MS_SSDT_INSTALL_TABLE_INSTANCE_FROM_LINK (Link);
    Link = GetNextNode (&gAmdCpmMsSsdtInstanceList, Link);
    DEBUG ((EFI_D_INFO, "Table Instance %d :\n", Index));
    DEBUG ((EFI_D_INFO, "  Ssdt ffs guid  : %g \n", pInstance->pTable->SsdtFfsGuid));
    {//DEBUG
      UINT8        DbgIdTableName[9];
      gBS->SetMem (DbgIdTableName, 9, 0);
      gBS->CopyMem (DbgIdTableName,  (UINT8 *)&pInstance->pTable->AcpiOemTableId, 8);
      DEBUG ((DEBUG_INFO, "  AcpiOemTableId  : %a \n", DbgIdTableName));
    }
    Index ++;
    if (pInstance->pTable == NULL) return;
    if (pInstance->pTable->AmdCpmIsSsdtNeeded != NULL) {
      if (!pInstance->pTable->AmdCpmIsSsdtNeeded (pInstance->pTable->AcpiOemTableId)) {
         DEBUG ((DEBUG_INFO, "  Install Status : Not Needed, Skip\n"));
        continue;
      }
    }
    Buffer = NULL;
    if (pInstance->pTable->AmdCpmMsSsdtCallBackContext != NULL) {
      Buffer = (VOID *)pInstance->pTable->AmdCpmMsSsdtCallBackContext(CpmTableProtocolPtr);
    }
    Status = CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                   (VOID*)CpmTableProtocolPtr,
                                                   pInstance->pTable->SsdtFfsGuid,
                                                   (UINT64 *)&pInstance->pTable->AcpiOemTableId,
                                                   pInstance->pTable->AmdCpmMsSsdtCallBackFunction,
                                                   Buffer
                                                   );

    DEBUG ((EFI_D_INFO, "  Install Status : %r \n", Status));
  }

  gBS->CloseEvent (Event);

  DEBUG((DEBUG_INFO, "%a(%d)-end\n", __FUNCTION__, __LINE__));

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM ModernStandby SSDT Init DXE driver
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmModernStandbyFeatureDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;
  EFI_HANDLE                              Handle = NULL;

  DEBUG ((DEBUG_INFO, "%a(%d)-Start\n", __FUNCTION__, __LINE__));


  if (!(PcdGetBool(PcdModernStandbyEnable))) {
    DEBUG ((DEBUG_INFO, "%a(%d)- Modern Standby is diabled\n",  __FUNCTION__, __LINE__));
    return EFI_SUCCESS;
  }
  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdCpmOemModsSsdtTableProtocol,
                  EFI_NATIVE_INTERFACE,
                  &gAmdCpmMsSsdtTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a(%d) - install %g status = %r\n",   __FUNCTION__, __LINE__, &gAmdCpmOemModsSsdtTableProtocol, Status));
    return Status;
  }

  //
  //install modernstandby  SSDT when ready to boot
  // 
  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_CALLBACK,
                  AmdCpmMsSsdtInstall,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a(%d) - create ms ssdt install readytoboot status = %r\n",   __FUNCTION__, __LINE__, Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "%a(%d)-end\n", __FUNCTION__, __LINE__));

  return Status;
}
