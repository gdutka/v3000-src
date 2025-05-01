/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#include "FchSmmMiscDispatcher.h"
#include <Library/FchSmmLib.h>
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHSMMDISPATCHER_FCHSMMMISCDISPATCHER_FCHSMMMISCDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmMiscDispatchRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_MISC_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext,
     OUT   EFI_HANDLE                             *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmMiscDispatchUnRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                             DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmMiscSmiEnable (
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext
  );

EFI_STATUS
EFIAPI
FchSmmMiscSmiDisable (
  IN       UINT32                                 ContextSmiStatusReg,
  IN       UINT32                                 ContextSmiStatusBit
  );

FCH_SMM_MISC_DISPATCH_PROTOCOL gFchSmmMiscDispatchProtocol = {
  FchSmmMiscDispatchRegister,
  FchSmmMiscDispatchUnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM Misc dispatcher handler
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
FchSmmMiscDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  FCH_SMM_MISC_NODE                   *CurrentFchSmmMiscNodePtr;
  FCH_SMM_COMMUNICATION_BUFFER        *CommunicationBufferPtr;
  UINT16                              MiscSmiDispatched;
  UINT8                               ResetData = 0;
  BOOLEAN                             IsFullHardReset = FALSE;

  MiscSmiDispatched = 0;
  Status = EFI_NOT_FOUND;
  CommunicationBufferPtr = (FCH_SMM_COMMUNICATION_BUFFER *)CommunicationBuffer;

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + CommunicationBufferPtr->SmiStatusReg) =
      CommunicationBufferPtr->SmiStatusBit;
  } else {
    CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + CommunicationBufferPtr->SmiStatusReg) =
      CommunicationBufferPtr->SmiStatusBit;
    while (CurrentFchSmmMiscNodePtr->FchMiscNodePtr!= NULL) {
      if (CurrentFchSmmMiscNodePtr->Context.SmiStatusReg == CommunicationBufferPtr->SmiStatusReg &&
          CurrentFchSmmMiscNodePtr->Context.SmiStatusBit == CommunicationBufferPtr->SmiStatusBit) {
        DEBUG (
          (DEBUG_INFO,
          "[FchSmmDispatcher] MISC SMM Dispatch Handler: StatusReg = 0x%x, StatusBit = 0x%x, Order = 0x%x\n",
          CurrentFchSmmMiscNodePtr->Context.SmiStatusReg,
          CurrentFchSmmMiscNodePtr->Context.SmiStatusBit,
          CurrentFchSmmMiscNodePtr->Context.Order)
          );
        Status = CurrentFchSmmMiscNodePtr->CallBackFunction (
                                            CurrentFchSmmMiscNodePtr->DispatchHandle,
                                            &CurrentFchSmmMiscNodePtr->Context
                                            );
        MiscSmiDispatched++;
      }
      CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    }

    if (CommunicationBufferPtr->SmiStatusReg == FCH_SMI_REG90 && CommunicationBufferPtr->SmiStatusBit == BIT13) {
      //
      // Clear cf9rstdisable to 0 in PMx00000D2 (FCH::PM::PMIODEBUG) BIT6
      //
      DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] MISC SMM Dispatch Handler: Clear cf9rstdisable to 0\n"));
      LibFchSmmPmioReadWrite8V2 (&gSmst->SmmIo, FCH_PMIOA_REGD2, ~ (UINT32) (BIT6), 0);
      //
      // Do CF9 reset
      //
      DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] MISC SMM Dispatch Handler: Do CF9 reset\n"));
      ResetData = 0x06;
      IsFullHardReset = PcdGetBool (PcdFchFullHardReset);
      if (IsFullHardReset) {
        ResetData = 0x0E;
      }
      LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCF9, &ResetData);
      //
      // Given we should have reset getting here would be bad
      //
      FCH_DEADLOOP();
    }

    if (MiscSmiDispatched <= 0) {
      Status = EFI_NOT_FOUND;
    } else {
      Status = EFI_SUCCESS;
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register MISC child handler worker function
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   MiscRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmMiscDispatchRegisterWorker (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_MISC_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext,
     OUT   EFI_HANDLE                             *DispatchHandle
  )
{
  EFI_STATUS            Status;
  FCH_SMM_MISC_NODE     *NewFchSmmMiscNodePtr;
  FCH_SMM_MISC_NODE     *CurrentFchSmmMiscNodePtr;
  FCH_SMM_MISC_NODE     *PreviousFchSmmMiscNodePtr;

  Status = EFI_OUT_OF_RESOURCES;
  if (CallBackFunction == NULL || MiscRegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_MISC_NODE),
                       &NewFchSmmMiscNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  NewFchSmmMiscNodePtr->CallBackFunction = CallBackFunction;
  *DispatchHandle = &NewFchSmmMiscNodePtr->DispatchHandle;
  NewFchSmmMiscNodePtr->DispatchHandle   = *DispatchHandle;
  NewFchSmmMiscNodePtr->Context          = *MiscRegisterContext;
  NewFchSmmMiscNodePtr->FchMiscNodePtr = HeadFchSmmMiscNodePtr;

  Status = FchSmmMiscSmiEnable (MiscRegisterContext);

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    NewFchSmmMiscNodePtr->FchMiscNodePtr = HeadFchSmmMiscNodePtr;
    HeadFchSmmMiscNodePtr = NewFchSmmMiscNodePtr;
  } else {
    PreviousFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    while (CurrentFchSmmMiscNodePtr->FchMiscNodePtr != NULL) {
      if (NewFchSmmMiscNodePtr->Context.Order <= CurrentFchSmmMiscNodePtr->Context.Order &&
          CurrentFchSmmMiscNodePtr->Context.SmiStatusReg == NewFchSmmMiscNodePtr->Context.SmiStatusReg &&
          CurrentFchSmmMiscNodePtr->Context.SmiStatusBit == NewFchSmmMiscNodePtr->Context.SmiStatusBit) {
        if (PreviousFchSmmMiscNodePtr == CurrentFchSmmMiscNodePtr) {
          NewFchSmmMiscNodePtr->FchMiscNodePtr = HeadFchSmmMiscNodePtr;
          HeadFchSmmMiscNodePtr = NewFchSmmMiscNodePtr;
          Status = EFI_SUCCESS;
          return  Status;
        }
        NewFchSmmMiscNodePtr->FchMiscNodePtr = PreviousFchSmmMiscNodePtr->FchMiscNodePtr;
        PreviousFchSmmMiscNodePtr->FchMiscNodePtr = NewFchSmmMiscNodePtr;

        Status = EFI_SUCCESS;
        return  Status;
      }
      PreviousFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr;
      CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    }
    PreviousFchSmmMiscNodePtr->FchMiscNodePtr = NewFchSmmMiscNodePtr;
    NewFchSmmMiscNodePtr->FchMiscNodePtr = CurrentFchSmmMiscNodePtr;
  }
  Status = EFI_SUCCESS;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister MISC child handler worker function
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
FchSmmMiscDispatchUnRegisterWorker (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                             DispatchHandle
  )
{
  EFI_STATUS        Status;
  FCH_SMM_MISC_NODE *CurrentFchSmmMiscNodePtr;
  FCH_SMM_MISC_NODE *PreviousFchSmmMiscNodePtr;
  BOOLEAN           Cf9IoTrapSmiRegClr = TRUE;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    if (CurrentFchSmmMiscNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    } else {
      while (CurrentFchSmmMiscNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr;
        CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
        if (CurrentFchSmmMiscNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmMiscNodePtr->FchMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    }
    Status = gSmst->SmmFreePool (
                       CurrentFchSmmMiscNodePtr
                       );
    ASSERT_EFI_ERROR (Status);
  }

  CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
  while (CurrentFchSmmMiscNodePtr->DispatchHandle != NULL) {
    if (CurrentFchSmmMiscNodePtr->Context.SmiStatusReg == FCH_SMI_REG90 &&
        CurrentFchSmmMiscNodePtr->Context.SmiStatusBit == BIT13) {
      Cf9IoTrapSmiRegClr = FALSE;
      break;
    }
    CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
  }

  if (Cf9IoTrapSmiRegClr == TRUE) {
    Status = FchSmmMiscSmiDisable (FCH_SMI_REG90, BIT13);
  }

  Status = EFI_SUCCESS;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register MISC child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   MiscRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmMiscDispatchRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_MISC_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext,
     OUT   EFI_HANDLE                             *DispatchHandle
  )
{
  EFI_STATUS            Status;
  EFI_STATUS            RegisterStatus;

  Status = FchSmmMiscDispatchRegisterWorker (
              This,
              CallBackFunction,
              MiscRegisterContext,
              DispatchHandle
              );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gFchSmmMiscDispatchProtocolGuid,
                                                    (EFI_SMM_HANDLER_ENTRY_POINT2)CallBackFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)MiscRegisterContext,
                                                    sizeof (FCH_SMM_MISC_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Unregister MISC child handler
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
FchSmmMiscDispatchUnRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                             DispatchHandle
  )
{
  EFI_STATUS           Status;
  EFI_STATUS           UnregisterStatus;
  FCH_SMM_MISC_NODE    *FchSmmMiscNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    while (FchSmmMiscNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmMiscNodePtr = FchSmmMiscNodePtr->FchMiscNodePtr;
      if (FchSmmMiscNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gFchSmmMiscDispatchProtocolGuid,
                                                    (EFI_SMM_HANDLER_ENTRY_POINT2)FchSmmMiscNodePtr->CallBackFunction,
                                                    (VOID*)&(FchSmmMiscNodePtr->Context),
                                                    sizeof (FCH_SMM_MISC_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmMiscDispatchUnRegisterWorker (
            This,
            DispatchHandle
            );

  return  Status;
}

EFI_STATUS
EFIAPI
FchSmmMiscSmiEnable (
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext
  )
{
  UINT32    SmiEvent;
  UINT32    SmiStatusBit;
  UINT32    SmiControlReg;
  UINT32    SmiControlBit;
  UINT32    OrMask;
  UINT32    AndMask;

  SmiEvent = 0;
  SmiStatusBit = MiscRegisterContext->SmiStatusBit;
  while ((SmiStatusBit & BIT0) == 0) {
    SmiEvent++;
    SmiStatusBit = SmiStatusBit >> 1;
  }
  SmiEvent += (MiscRegisterContext->SmiStatusReg - FCH_SMI_REG80) * 8;

  SmiControlReg = FCH_SMI_REGA0 + (SmiEvent * 2 / 32) * 4;
  SmiControlBit = 1 << ((SmiEvent *2 ) % 32);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + MiscRegisterContext->SmiStatusReg) =  MiscRegisterContext->SmiStatusBit;

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + SmiControlReg) &= ~(UINT32) (SmiControlBit + SmiControlBit * 2);
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + SmiControlReg) |= SmiControlBit;
  // Save in BootScriptTable for S3
  OrMask = SmiControlBit;
  AndMask = 0xFFFFFFFFul;
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    (ACPI_MMIO_BASE + SMI_BASE + SmiControlReg),
    &OrMask,
    &AndMask
    );

  if (MiscRegisterContext->SmiStatusReg == FCH_SMI_REG90 && MiscRegisterContext->SmiStatusBit == BIT13) {
    //
    // Set cf9rstdisable to 1 in PMx00000D2 (FCH::PM::PMIODEBUG) BIT6
    //
    LibFchSmmPmioReadWrite8V2 (&gSmst->SmmIo, FCH_PMIOA_REGD2, ~ (UINT32) (BIT6), BIT6);
    OrMask = BIT22;
    AndMask = 0xFFFFFFFFul;
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32,
                                  (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD0),
                                  &OrMask,
                                  &AndMask);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchSmmMiscSmiDisable (
  IN       UINT32                                 ContextSmiStatusReg,
  IN       UINT32                                 ContextSmiStatusBit
  )
{
  UINT32    SmiEvent;
  UINT32    SmiStatusBit;
  UINT32    SmiControlReg;
  UINT32    SmiControlBit;
  UINT32    OrMask;
  UINT32    AndMask;

  SmiEvent = 0;
  SmiStatusBit = ContextSmiStatusBit;
  while ((SmiStatusBit & BIT0) == 0) {
    SmiEvent++;
    SmiStatusBit = SmiStatusBit >> 1;
  }
  SmiEvent += (ContextSmiStatusReg - FCH_SMI_REG80) * 8;

  SmiControlReg = FCH_SMI_REGA0 + (SmiEvent * 2 / 32) * 4;
  SmiControlBit = 1 << ((SmiEvent *2 ) % 32);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + ContextSmiStatusReg) =  ContextSmiStatusBit;

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + SmiControlReg) &= ~(UINT32) (SmiControlBit + SmiControlBit * 2);
  // Save in BootScriptTable for S3
  OrMask = 0;
  AndMask = ~(UINT32) (SmiControlBit + SmiControlBit * 2);
  S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32,
                                (ACPI_MMIO_BASE + SMI_BASE + SmiControlReg),
                                &OrMask,
                                &AndMask);
  if (ContextSmiStatusReg == FCH_SMI_REG90 && ContextSmiStatusBit == BIT13) {
    //
    // Clear cf9rstdisable to 0 in PMx00000D2 (FCH::PM::PMIODEBUG) BIT6
    //
    LibFchSmmPmioReadWrite8 (&gSmst->SmmIo, FCH_PMIOA_REGD2, ~ (UINT32) (BIT6), 0);
    OrMask = 0;
    AndMask = 0xFFBFFFFFul;
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32,
                                  (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD0),
                                  &OrMask,
                                  &AndMask);
  }
  return EFI_SUCCESS;
}

