/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#include "FchSmmPwrBtnDispatcher.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHSMMDISPATCHER_FCHSMMPWRBTNDISPATCHER_FCHSMMPWRBTNDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2Register (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PWRBTN_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       FCH_SMM_PWRBTN_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                               *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2UnRegister (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                               DispatchHandle
  );

FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL gFchSmmPwrBtnDispatch2Protocol = {
  FchSmmPwrBtnDispatch2Register,
  FchSmmPwrBtnDispatch2UnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM POWER BUTTON dispatcher handler
 *
 *
 * @param[in]       SmmImageHandle        Image Handle
 * @param[in, out]   OPTIONAL CommunicationBuffer   Communication Buffer (see PI 1.1 for more details)
 * @param[in, out]   OPTIONAL SourceSize            Buffer size (see PI 1.1 for more details)

 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PWRBTN_NODE *CurrentFchSmmPwrBtnNodePtr;
  Status = EFI_NOT_FOUND;

  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = PwrBtn;
    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr!= NULL) {
      if (CurrentFchSmmPwrBtnNodePtr->Context.Phase == EfiPowerButtonEntry) {
        if (CurrentFchSmmPwrBtnNodePtr->CallBack2Function != NULL) {
          Status = CurrentFchSmmPwrBtnNodePtr->CallBack2Function (
                                                 CurrentFchSmmPwrBtnNodePtr->DispatchHandle,
                                                 &CurrentFchSmmPwrBtnNodePtr->Context,
                                                 NULL,
                                                 NULL
                                                 );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] PwrBtn SMM handler dispatched (Phase = Entry) Order = 0x%x, \
                  return - %r\n", CurrentFchSmmPwrBtnNodePtr->Context.Order, Status));
        }
        Status = EFI_SUCCESS;
      }
      CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }

    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr!= NULL) {
      if (CurrentFchSmmPwrBtnNodePtr->Context.Phase == EfiPowerButtonExit) {
        if (CurrentFchSmmPwrBtnNodePtr->CallBack2Function != NULL) {
          Status = CurrentFchSmmPwrBtnNodePtr->CallBack2Function (
                                                 CurrentFchSmmPwrBtnNodePtr->DispatchHandle,
                                                 &CurrentFchSmmPwrBtnNodePtr->Context,
                                                 NULL,
                                                 NULL
                                                 );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] PwrBtn SMM handler dispatched (Phase = Exit) Order = 0x%x, \
                  return - %r\n", CurrentFchSmmPwrBtnNodePtr->Context.Order, Status));
        }
        Status = EFI_SUCCESS;
      }
      CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register POWER BOTTON child handler worker function
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   RegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2RegisterWorker (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PWRBTN_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       FCH_SMM_PWRBTN_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                               *DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PWRBTN_NODE *NewFchSmmPwrBtnNodePtr;
  FCH_SMM_PWRBTN_NODE *CurrentFchSmmPwrBtnNodePtr;
  FCH_SMM_PWRBTN_NODE *PreviousFchSmmPwrBtnNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC) |= BIT6;
  Status = gSmst->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_PWRBTN_NODE),
                       &NewFchSmmPwrBtnNodePtr
                       );
  ASSERT_EFI_ERROR (Status);
  NewFchSmmPwrBtnNodePtr->CallBack2Function = CallBackFunction;
  *DispatchHandle = &NewFchSmmPwrBtnNodePtr->DispatchHandle;
  NewFchSmmPwrBtnNodePtr->DispatchHandle = *DispatchHandle;
  NewFchSmmPwrBtnNodePtr->Context = *RegisterContext;

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering PwrBtn SMM handler: Phase = 0x%x, Order = 0x%x\n", \
         RegisterContext->Phase, RegisterContext->Order));
  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    HeadFchSmmPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;
  } else {
    PreviousFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr != NULL) {
      if (NewFchSmmPwrBtnNodePtr->Context.Order <= CurrentFchSmmPwrBtnNodePtr->Context.Order) {
        if (PreviousFchSmmPwrBtnNodePtr == CurrentFchSmmPwrBtnNodePtr) {
          NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
          HeadFchSmmPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
        PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;

        Status = EFI_SUCCESS;
        return Status;
      }
      PreviousFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr;
      CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }
    PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;
    NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr;
  }
  Status = EFI_SUCCESS;



  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister POWER BUTTON child handler worker fucntion
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchHandle
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2UnRegisterWorker (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PWRBTN_NODE *CurrentFchSmmPwrBtnNodePtr;
  FCH_SMM_PWRBTN_NODE *PreviousFchSmmPwrBtnNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    if (CurrentFchSmmPwrBtnNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    } else {
      while (CurrentFchSmmPwrBtnNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr;
        CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
        if (CurrentFchSmmPwrBtnNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }

    Status = gSmst->SmmFreePool (
                         CurrentFchSmmPwrBtnNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register POWER BOTTON child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   RegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2Register (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PWRBTN_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       FCH_SMM_PWRBTN_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                               *DispatchHandle
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  RegisterStatus;

  Status = FchSmmPwrBtnDispatch2RegisterWorker (
              This,
              CallBackFunction,
              RegisterContext,
              DispatchHandle
              );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                                                    CallBackFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)RegisterContext,
                                                    sizeof (EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister POWER BUTTON child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchHandle
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2UnRegister (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS            Status;
  EFI_STATUS            UnregisterStatus;
  FCH_SMM_PWRBTN_NODE   *FchSmmPwrBtnNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (FchSmmPwrBtnNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmPwrBtnNodePtr = FchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
      if (FchSmmPwrBtnNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                      mSmiHandlerProfileProtocol,
                                                      &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                                                      FchSmmPwrBtnNodePtr->CallBack2Function,
                                                      (VOID*)&(FchSmmPwrBtnNodePtr->Context),
                                                      sizeof (EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT)
                                                      );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmPwrBtnDispatch2UnRegisterWorker (
              This,
              DispatchHandle
              );

  return  Status;
}

EFI_STATUS
EFIAPI
EfiSmmPwrBtnDispatch2Register (
  IN       CONST EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2                  DispatchFunction,
  IN       EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT         *RegisterContext,
     OUT   EFI_HANDLE                                    *DispatchHandle
  )
{
  EFI_STATUS                        Status;
  EFI_STATUS                        RegisterStatus;
  FCH_SMM_PWRBTN_REGISTER_CONTEXT   FchRegisterContext;

  FchRegisterContext.Phase = RegisterContext->Phase;
  FchRegisterContext.Order = 0x80;      // SMM_DEFAULT_ORDER

  Status = FchSmmPwrBtnDispatch2RegisterWorker (
           &gFchSmmPwrBtnDispatch2Protocol,
           DispatchFunction,
           &FchRegisterContext,
           DispatchHandle
           );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                                                    DispatchFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)RegisterContext,
                                                    sizeof (EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return Status;
}

EFI_STATUS
EFIAPI
EfiSmmPwrBtnDispatch2UnRegister (
  IN       CONST EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                                    DispatchHandle
  )
{
  EFI_STATUS            Status;
  EFI_STATUS            UnregisterStatus;
  FCH_SMM_PWRBTN_NODE   *FchSmmPwrBtnNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (FchSmmPwrBtnNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmPwrBtnNodePtr = FchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
      if (FchSmmPwrBtnNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                      mSmiHandlerProfileProtocol,
                                                      &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                                                      FchSmmPwrBtnNodePtr->CallBack2Function,
                                                      (VOID*)&(FchSmmPwrBtnNodePtr->Context),
                                                      sizeof (EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT)
                                                      );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmPwrBtnDispatch2UnRegisterWorker (
           &gFchSmmPwrBtnDispatch2Protocol,
           DispatchHandle
           );

  return Status;
}

EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL gEfiSmmPwrBtnDispatch2Protocol = {
  EfiSmmPwrBtnDispatch2Register,
  EfiSmmPwrBtnDispatch2UnRegister
};


