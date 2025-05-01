/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#include "AodSmm.h"
AOD_MOUDLE_NAME ("AodSmm")

 BOOLEAN                          mReadyToLock           = FALSE;;
 VOID                             *gAodConfig            = NULL;
 AodNvAreaStruct                  *gAodNvs               = NULL;
 BOOLEAN                          gAodConfigBuffered    = FALSE;
 PRIVATE_DATA                     mPrivateData = {
  {
    NULL,
    NULL
  },                                    ///< CallbackDataBase linked list head
  NULL,                                 ///< EFI handle returned when calling InstallMultipleProtocolInterfaces
  NULL,                                 ///< Physical address that will stored the cmdid information
  {
    AodCmdSmmRegister,
    AodCmdSmmUnRegister
  }                                    ///< AodCmdSmmProtocol
};

/**
  SMM ready to lock notification event handler.

  @param  Protocol   Points to the protocol's unique identifier
  @param  Interface  Points to the interface instance
  @param  Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   SmmReadyToLockCallback runs successfully

**/
EFI_STATUS
EFIAPI
AodCmdSmmReadyToLockCallback (
  IN CONST EFI_GUID                       *Protocol,
  IN VOID                                 *Interface,
  IN EFI_HANDLE                           Handle
  )
{

  mReadyToLock = TRUE;

  return EFI_SUCCESS;
}

/**
  Check the CmdId to see if there is a duplicated one in the database

  @param[in] CmdId      Aod command ID

  @retval EFI_SUCCESS             There is no duplicated CmdId
  @retval EFI_INVALID_PARAMETER   There is a duplicated CmdId
**/
EFI_STATUS
EFIAPI
AodCmdIdDuplicateCheck (
  IN    UINT32          CmdId
  )
{

  CMD_RECORD      *RecordInDb;
  LIST_ENTRY      *LinkInDb;

  LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);
  while (!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
    RecordInDb = CMD_RECORD_FROM_LINK (LinkInDb);

    if (RecordInDb->CmdId == CmdId) {
      return EFI_ALREADY_STARTED;
    }

    LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AodSmmCoreDispatcherCallback (
  IN  UINT8           CallbackPhase,
  IN  UINT32          CmdId,
  IN  UINT32          Data,
  IN  VOID            *Context
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  CMD_RECORD                 *RecordInDb;
  LIST_ENTRY                 *LinkInDb;
  //
  // make sure database is not empty
  //
  if (!IsListEmpty (&mPrivateData.CallbackDataBase)) {
    //
    // retrieve the database
    //
    LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);
    while (!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
      RecordInDb = CMD_RECORD_FROM_LINK (LinkInDb);
      if (RecordInDb == NULL) return EFI_NOT_FOUND;
      if (RecordInDb->CallbackPhase == CallbackPhase) {
        if ((RecordInDb->CmdId == CmdId) || (RecordInDb->CmdId == 0)) {
          Status = RecordInDb->Callback (CmdId, Data, Context);
          if (EFI_ERROR (Status)) break;
        }
      }
      LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);
    }
  }
  return Status;
}
/**
  Register a child SMI source callback function for the WMI smi

  This service registers a function which will be called when the input WMI command ID
  and Phase is mathced. On return,  DispatchHandle contains a unique handle which may be used later
  to unregister the function using UnRegister().

  @param[in]  This                 Pointer to the AOD_CMD_SMM_DISPATCH_PROTOCOL instance.
  @param[in]  Context              Pointer to AOD_CMD_SMM_REGISTER_CONTEXT context.
                                   The caller fills this context in before calling
                                   the register function to indicate to the register
                                   function which callback function should be invoked for.
  @param[in]  Callback             Pointer to the AOD_CMD_SMM_CALLBACK callback function.
  @param[out] DispatchHandle       Handle generated by the dispatcher to track the
                                   function instance.

  @retval EFI_SUCCESS            The callback function has been successfully registered
  @retval EFI_INVALID_PARAMETER  Context is invalid.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory (system or SMM) to manage this
                                 child.
  @retval EFI_ACCESS_DENIED      SmmReadyToLock is signaled
**/
EFI_STATUS
EFIAPI
AodCmdSmmRegister (
  IN  CONST  AOD_CMD_SMM_DISPATCH_PROTOCOL  *This,
  IN    AOD_CMD_SMM_REGISTER_CONTEXT        *Context,
  IN    AOD_CMD_SMM_CALLBACK                Callback,
  OUT   EFI_HANDLE                          *Handle
  )
{
  EFI_STATUS                   Status;
  CMD_RECORD                   *Record;
  UINT32                       CmdId;

  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CmdId = Context->CmdId;
  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    AOD_DEBUG ((DEBUG_ERROR, "Register is not allowed if the EndOfDxe event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  //
  // Create cmd record and add to database
  //
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (CMD_RECORD), (VOID **) &Record);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (Record, sizeof (CMD_RECORD));

  //
  // Gather information about the registration request
  //
  Record->Signature         = CMD_RECORD_SIGNATURE;
  Record->Callback          = Callback;
  Record->CmdId             = Context->CmdId;
  Record->CallbackPhase     = Context->CallbackPhase;
  Record->Processed         = FALSE;
  //
  // Insert the record into the database
  //
  InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
  *Handle = &Record->Link;
  return EFI_SUCCESS;
}
#define EFI_BAD_POINTER          0xAFAFAFAFAFAFAFAFULL

/**
  Unregister a child SMI source callback function for the WMI smi

  @param[in] This                Pointer to the EFI_SMM_SW_DISPATCH2_PROTOCOL instance.
  @param[in] Handle              Handle of dispatch function to deregister.

  @retval EFI_SUCCESS            The dispatch function has been successfully unregistered.
  @retval EFI_INVALID_PARAMETER  The DispatchHandle was not valid.
  @retval EFI_ACCESS_DENIED      SmmReadyToLock is signaled
**/
EFI_STATUS
AodCmdSmmUnRegister (
  IN  CONST  AOD_CMD_SMM_DISPATCH_PROTOCOL            *This,
  IN         EFI_HANDLE                               *Handle
  )
{
  EFI_STATUS      Status;
  CMD_RECORD      *RecordInDb;

  if ((This == NULL) || (Handle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    AOD_DEBUG ((DEBUG_ERROR, "UnRegister is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }
  RecordInDb = CMD_RECORD_FROM_LINK (Handle);

  //
  // Take the entry out of the linked list
  //
  if (RecordInDb->Link.ForwardLink == (LIST_ENTRY *) EFI_BAD_POINTER) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Remove the link and free the resources
  //
  RemoveEntryList (&RecordInDb->Link);
  Status = gSmst->SmmFreePool (RecordInDb);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  hook to sync L"AodSetup" into APCB/CMOS

  @param[in] Context             L"AodSetup" POINTER

  @retval EFI_SUCCESS            The function has been successfully called.
**/
EFI_STATUS
EFIAPI
AodVariableHook (
  IN  VOID            *Context
  )
{
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  return AodSmmCoreDispatcherCallback (AOD_SMM_CMD_VARIABLE_HOOK, 0, 0, Context);
}


VOID
FinalizeTransferBuffer (
  AodNvAreaStruct *AodNvAreaPtr,
  EFI_STATUS  Status
  )
{
  AodNvAreaPtr->AodSmiCmd = 0;
  AodNvAreaPtr->AodSmiStatus = (UINT32) Status;
}

/**
  Communication service SMI Handler entry.

  This SMI handler provides services for dispatch phase.

  @param[in]     DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]     RegisterContext Points to an optional handler context which was specified when the
                                 handler was registered.
  @param[in, out] CommBuffer     A pointer to a collection of Data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize The Size of the CommBuffer.

  @retval EFI_SUCCESS                         The interrupt was handled and quiesced. No other handlers
                                              should still be called.
  @retval others                              failure
**/
EFI_STATUS
EFIAPI
AodSmmCommunicateHandler (
  IN     EFI_HANDLE                 DispatchHandle,
  IN     CONST VOID                 *RegisterContext,
  IN OUT VOID                       *CommBuffer,
  IN OUT UINTN                      *CommBufferSize
  )
{
  EFI_STATUS                        Status;
  AOD_SMM_COMMUNICATE_HEADER        *AodSmmCommHeader;
  UINTN                             TempCommBufferSize;

  if (CommBuffer == NULL || CommBufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (*CommBufferSize < OFFSET_OF (AOD_SMM_COMMUNICATE_HEADER, Data)) return EFI_INVALID_PARAMETER;
  if (!SmmIsBufferOutsideSmmValid ((UINTN)CommBuffer, *CommBufferSize)) {
    AOD_DEBUG ((EFI_D_ERROR,"SMM communication data buffer in SMRAM or overflow!\n"));
    return EFI_INVALID_PARAMETER;
  }
  AodSmmCommHeader = (AOD_SMM_COMMUNICATE_HEADER *)CommBuffer;
  TempCommBufferSize = *CommBufferSize - OFFSET_OF (AOD_SMM_COMMUNICATE_HEADER, Data);
  switch (AodSmmCommHeader->Id) {
    case AOD_SMM_COMM_VARIABLE_HOOK:
      if (((AOD_VARIABLE_HEADER *)(AodSmmCommHeader->Data))->AodVariableSize != TempCommBufferSize) {
        AOD_DEBUG ((EFI_D_ERROR,"SMM communication data size(%x) not equal to Variable Size%x\n", \
          TempCommBufferSize, ((AOD_VARIABLE_HEADER *)(AodSmmCommHeader->Data))->AodVariableSize ));
        Status = EFI_UNSUPPORTED;
      } else {
        Status = AodVariableHook ((VOID *)AodSmmCommHeader->Data);
        if (EFI_ERROR (Status)) {
          AOD_DEBUG ((EFI_D_ERROR,"SMM communication AOD_SMM_COMM_VARIABLE_HOOK failed status = %r\n", Status));
        }
      }
      break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
  }
  AodSmmCommHeader->RetStatus = Status;

  return EFI_SUCCESS;
}
/**
  The callback function to handle subsequent Aod Smm Handle.
  This callback will be called by Rymaster send the WMI command.

  @param[in] SmmImageHandle             Not used
  @param[in] PchSmmCore                 Not used
  @param[in, out] CommunicationBuffer   Not used
  @param[in, out] SourceSize            Not used

  @retval EFI_SUCCESS                   Function successfully completed
**/
EFI_STATUS
EFIAPI
AodSmmCoreDispatcher (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  EFI_STATUS                 Status;


  UINT32                     CmdId;
  UINT32                     CmdData;

  CmdId   = gAodNvs->CmdId;
  CmdData = gAodNvs->CmdData;

  AOD_DEBUG ((EFI_D_INFO, "[AOD][SMM] Cmd = %08x, Data = %08x\n", CmdId, CmdData));
  if (gAodConfigBuffered == FALSE) {
    Status = AodGetConfiguration (&gAodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_INFO, "[AOD][SMM] GET_AOD_VARIABLE Status = %r\n", Status));
      goto Exit;
    }
  }
  //
  // Step1: Sync the AOD WMI information into NVS
  //
  Status = AodSmmCoreDispatcherCallback (AOD_SMM_CMD_TO_NVS, CmdId, CmdData, gAodNvs);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_INFO, "[AOD][SMM] AOD_SMM_CMD_TO_NVS Status = %r\n", Status));
    goto Exit;
  }
  if (CmdId == COMMAND_BUFFER_START) {
    gAodConfigBuffered = TRUE;
    Status = EFI_SUCCESS;
  } else if (CmdId == COMMAND_BUFFER_END) {
    gAodConfigBuffered = FALSE;
  } else {

    //
    // Step2:  Sync the AOD WMI information into AOD variable
    //
    Status = AodSmmCoreDispatcherCallback (AOD_SMM_CMD_SPECAIL, CmdId, CmdData, gAodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_INFO, "[AOD][SMM] AOD_SMM_CMD_SPECAIL Status = %r\n", Status));
      goto Exit;
    }

    //
    // Step3:  Sync the AOD WMI information into AOD variable
    //
    Status = AodSmmCoreDispatcherCallback (AOD_SMM_CMD_TO_VARIABLE, CmdId, CmdData, gAodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_INFO, "[AOD][SMM] AOD_SMM_CMD_TO_VARIABLE Status = %r\n", Status));
      goto Exit;
    }
  }
  if (gAodConfigBuffered == FALSE) {
    Status = AodSetConfiguration (gAodConfig, ((AOD_VARIABLE_HEADER *)gAodConfig)->AodVariableSize);
    if (EFI_ERROR(Status)) {
      AOD_DEBUG ((EFI_D_INFO, "SET_AOD_VARIABLE Status = %r\n", Status));
      goto Exit;
    }
    Status = AodSmmCoreDispatcherCallback (AOD_SMM_VARIABLE_TO_APCB, CmdId, CmdData, gAodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_INFO, "[AOD][SMM] AOD_SMM_CMD_TO_NVS Status = %r\n", Status));
      goto Exit;
    }
  }
Exit:
  FinalizeTransferBuffer (gAodNvs, Status);
  return Status;
}
/*********************************************************************************
 * Name: AodSmmEntryPoint
 *
 * Description
 *   Entry point of the AMD AOD SMM driver, register AOD Smm callbacks
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
AodSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                       Status;
  FCH_SMM_SW_DISPATCH2_PROTOCOL    *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT      SwRegisterContext;
  EFI_HANDLE                       Handle = NULL;
  AOD_NV_AREA_UPDATED_PROTOCOL     *AodNvsProtocol;
  VOID                             *SmmReadyToLockRegistration;
  AMD_CPM_VARIABLE_HOOK_PROTOCOL   *AmdCpmVariableHook;
  EFI_PHYSICAL_ADDRESS             Address;

  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSwDispatch2ProtocolGuid,
                  NULL,
                  &AmdSwDispatch
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gAodNvAreaUpdatedProtocolGuid, NULL, (VOID **) &AodNvsProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Update Aod NVS address;
  //
  mPrivateData.Nvs = (VOID *)AodNvsProtocol->AodNvAreaPtr;
  gAodNvs = AodNvsProtocol->AodNvAreaPtr;

  //
  // Initialize callback Database
  //
  InitializeListHead (&mPrivateData.CallbackDataBase);

  //
  // Register Aod Cmd dispatched hander
  //
  SwRegisterContext.AmdSwValue  = PcdGet8(SwSmiCmdAodUpdateMemClkTimings);
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AodSmmCoreDispatcher,
                            &SwRegisterContext,
                            &mPrivateData.SmiHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Initialize Aod Smi Register Protocol
  //
  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gAmdAodSmmCmdProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPrivateData.CmdSmmProtocol
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Register EFI_SMM_READY_TO_LOCK_PROTOCOL_GUID notify function.
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmReadyToLockProtocolGuid,
                    AodCmdSmmReadyToLockCallback,
                    &SmmReadyToLockRegistration
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Initialize Aod Variable Hook Protocol
  //
  Handle = NULL;
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (AMD_CPM_VARIABLE_HOOK_PROTOCOL),
                    (VOID **)&AmdCpmVariableHook
                    );
  if (EFI_ERROR (Status) || (AmdCpmVariableHook == NULL)) {
    return Status;
  }
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    StrnSizeS (AOD_SETUP_VARIABLE_NAME, 0x100),
                    (VOID **)&AmdCpmVariableHook->VariableName
                    );
  if (EFI_ERROR (Status) || (AmdCpmVariableHook->VariableName == NULL)) {
    return Status;
  }
  AmdCpmVariableHook->Version = AMD_CPM_VARIABLE_HOOK_PROTOCOL_VERSION;
  CopyMem (AmdCpmVariableHook->VariableName, AOD_SETUP_VARIABLE_NAME, StrnSizeS (AOD_SETUP_VARIABLE_NAME, 0x100));
  CopyMem (&AmdCpmVariableHook->VariableGuid, &gAodSystemConfigurationGuid, sizeof(EFI_GUID));
  AmdCpmVariableHook->VariableHook = AodVariableHook;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gAmdCpmVariableHookProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    AmdCpmVariableHook
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Init AOD smm communicate Handler
  //
  Handle = NULL;
  gAodNvs->AodSmmCommBufSize = SIZE_4KB;
  Address = 0xffffffff;
  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiRuntimeServicesData,
                   EFI_SIZE_TO_PAGES (gAodNvs->AodSmmCommBufSize),
                   &Address
                   );
  gAodNvs->AodSmmCommBufAddr = Address;
  Status = gSmst->SmiHandlerRegister (AodSmmCommunicateHandler, &gAodSystemConfigurationGuid, &Handle);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "SmiHandlerRegister CommunicateHandler Status =%r\n", Status));
    return Status;
  }

  //
  // Dispacth SOC specific ACPI Table
  //
  Status = AodDispatchInitTable (AOD_UPDATE_NVS, &mPrivateData.CmdSmmProtocol);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "AOD_UPDATE_NVS Status =%r\n", Status));
    return Status;
  }
  Status = AodDispatchInitTable (AOD_UPDATE_VARIABLE, &mPrivateData.CmdSmmProtocol);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "AOD_UPDATE_VARIABLE Status =%r\n", Status));
    return Status;
  }
  Status = AodDispatchInitTable (AOD_CONFIG_SYNC_APCB, &mPrivateData.CmdSmmProtocol);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "AOD_CONFIG_SYNC_APCB Status =%r\n", Status));
    return Status;
  }
  return Status;
}
