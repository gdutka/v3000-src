/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#include "FchSmmGpiDispatcher.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHSMMDISPATCHER_FCHSMMGPIDISPATCHER_FCHSMMGPIDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmGpiDispatch2Register (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_GPI_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT    *GpiRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmGpiDispatch2UnRegister (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  );

FCH_SMM_GPI_DISPATCH2_PROTOCOL gFchSmmGpiDispatch2Protocol = {
  FchSmmGpiDispatch2Register,
  FchSmmGpiDispatch2UnRegister,
  MAX_GPI_VALUE
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM GPI dispatcher handler
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
FchSmmGpiDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS        Status;
  FCH_SMM_GPI_NODE  *CurrentFchSmmGpiNodePtr;
  UINT16            GpiSmiDispatched;
  UINT32            GpiStatus;
  UINT32            GpiEnable;
  UINT32            SmiSciStat;
  UINT64            GpiStatusNum;
  UINTN             SizeOfContext;

  GpiSmiDispatched = 0;
  Status = EFI_NOT_FOUND;
  SizeOfContext = sizeof (EFI_SMM_GPI_REGISTER_CONTEXT);

  if (HeadFchSmmGpiNodePtr->FchGpiNodePtr == NULL) {
    GpiStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10) = GpiStatus;
    Status = EFI_NOT_FOUND;
  } else {
    GpiStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
    GpiEnable = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14);
    GpiStatus &= GpiEnable;
    if (GpiStatus == 0) {
      Status = EFI_NOT_FOUND;
    } else {
      GpiStatusNum = 0;
      SmiSciStat = 1;
      while ((GpiStatus & BIT0) != 1) {
        GpiStatus >>= 1;
        SmiSciStat <<= 1;
        GpiStatusNum++;
      }
      CurrentFchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
      while (CurrentFchSmmGpiNodePtr->FchGpiNodePtr!= NULL) {
        if (CurrentFchSmmGpiNodePtr->Context.GpiNum == GpiStatusNum) {
          Status = CurrentFchSmmGpiNodePtr->CallBack2Function (
                                             CurrentFchSmmGpiNodePtr->DispatchHandle,
                                             &CurrentFchSmmGpiNodePtr->Context,
                                             &CurrentFchSmmGpiNodePtr->Context,
                                             &SizeOfContext
                                             );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] GPI SMM handler dispatched GpiNum = 0x%x, \
                  return - %r\n", CurrentFchSmmGpiNodePtr->Context.GpiNum, Status));
          GpiSmiDispatched++;
          break;
        }
        CurrentFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
      }
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10) = SmiSciStat;

      if (GpiSmiDispatched <= 0) {
        Status = EFI_NOT_FOUND;
      } else {
        Status = EFI_SUCCESS;
      }
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register GPI child handler worker function
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   GpiRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmGpiDispatch2RegisterWorker (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_GPI_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT    *GpiRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  UINT8       SmiSciNum;
  EFI_STATUS  Status;
  FCH_SMM_GPI_NODE *NewFchSmmGpiNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || GpiRegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (GpiRegisterContext->GpiNum >= 0x20) {
    return EFI_INVALID_PARAMETER;
  }
  SmiSciNum = (UINT8) (GpiRegisterContext->GpiNum);

  Status = gSmst->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_GPI_NODE),
                       &NewFchSmmGpiNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering GPI SMM handler: GpiNum = 0x%x\n", GpiRegisterContext->GpiNum));
  NewFchSmmGpiNodePtr->CallBack2Function = CallBackFunction;
  *DispatchHandle = &NewFchSmmGpiNodePtr->DispatchHandle;
  NewFchSmmGpiNodePtr->DispatchHandle = *DispatchHandle;
  NewFchSmmGpiNodePtr->Context = *GpiRegisterContext;
  NewFchSmmGpiNodePtr->FchGpiNodePtr = HeadFchSmmGpiNodePtr;
  HeadFchSmmGpiNodePtr = NewFchSmmGpiNodePtr;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14) |= (1 << SmiSciNum);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Unregister GPI child handler worker function
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
FchSmmGpiDispatch2UnRegisterWorker (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  UINT8       SmiSciNum;
  EFI_STATUS  Status;
  FCH_SMM_GPI_NODE *CurrentFchSmmGpiNodePtr;
  FCH_SMM_GPI_NODE *PreviousFchSmmGpiNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SmiSciNum = 0;

  if (HeadFchSmmGpiNodePtr->FchGpiNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
    CurrentFchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
    if (CurrentFchSmmGpiNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
    } else {
      while (CurrentFchSmmGpiNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr;
        CurrentFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
        if (CurrentFchSmmGpiNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmGpiNodePtr->FchGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
    }
    SmiSciNum = (UINT8) (CurrentFchSmmGpiNodePtr->Context.GpiNum);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14) &= ~(UINT32) (1 << SmiSciNum);
    Status = gSmst->SmmFreePool (
                         CurrentFchSmmGpiNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register GPI child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   GpiRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmGpiDispatch2Register (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_GPI_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT    *GpiRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  RegisterStatus;

  Status = FchSmmGpiDispatch2RegisterWorker (
              This,
              CallBackFunction,
              GpiRegisterContext,
              DispatchHandle
              );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmGpiDispatch2ProtocolGuid,
                                                    CallBackFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)GpiRegisterContext,
                                                    sizeof (EFI_SMM_GPI_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Unregister GPI child handler
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
FchSmmGpiDispatch2UnRegister (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  EFI_STATUS          Status;
  EFI_STATUS          UnregisterStatus;
  FCH_SMM_GPI_NODE    *FchSmmGpiNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmGpiNodePtr->FchGpiNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
    while (FchSmmGpiNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmGpiNodePtr = FchSmmGpiNodePtr->FchGpiNodePtr;
      if (FchSmmGpiNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                      mSmiHandlerProfileProtocol,
                                                      &gEfiSmmGpiDispatch2ProtocolGuid,
                                                      FchSmmGpiNodePtr->CallBack2Function,
                                                      (VOID*)&(FchSmmGpiNodePtr->Context),
                                                      sizeof (EFI_SMM_GPI_REGISTER_CONTEXT)
                                                      );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmGpiDispatch2UnRegisterWorker (
              This,
              DispatchHandle
              );

  return Status;
}


EFI_STATUS
EFIAPI
EfiSmmGpiDispatch2Register (
  IN       CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2         DispatchFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  )
{
  EFI_STATUS          Status;
  EFI_STATUS          RegisterStatus;

  Status = FchSmmGpiDispatch2RegisterWorker (
           &gFchSmmGpiDispatch2Protocol,
           DispatchFunction,
           RegisterContext,
           DispatchHandle
           );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmGpiDispatch2ProtocolGuid,
                                                    DispatchFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)RegisterContext,
                                                    sizeof (EFI_SMM_GPI_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return Status;
}

EFI_STATUS
EFIAPI
EfiSmmGpiDispatch2UnRegister (
  IN       CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                           DispatchHandle
  )
{
  EFI_STATUS          Status;
  EFI_STATUS          UnregisterStatus;
  FCH_SMM_GPI_NODE    *FchSmmGpiNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmGpiNodePtr->FchGpiNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
    while (FchSmmGpiNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmGpiNodePtr = FchSmmGpiNodePtr->FchGpiNodePtr;
      if (FchSmmGpiNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                      mSmiHandlerProfileProtocol,
                                                      &gEfiSmmGpiDispatch2ProtocolGuid,
                                                      FchSmmGpiNodePtr->CallBack2Function,
                                                      (VOID*)&(FchSmmGpiNodePtr->Context),
                                                      sizeof (EFI_SMM_GPI_REGISTER_CONTEXT)
                                                      );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmGpiDispatch2UnRegisterWorker (
           &gFchSmmGpiDispatch2Protocol,
           DispatchHandle
           );

  return Status;
}

EFI_SMM_GPI_DISPATCH2_PROTOCOL gEfiSmmGpiDispatch2Protocol = {
  EfiSmmGpiDispatch2Register,
  EfiSmmGpiDispatch2UnRegister,
  MAX_GPI_VALUE
};


