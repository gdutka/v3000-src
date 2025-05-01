/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/


#include "FchSmmIoTrapDispatcher.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHSMMDISPATCHER_FCHSMMIOTRAPDISPATCHER_FCHSMMIOTRAPDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmIoTrapDispatch2Register (
  IN       CONST FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_IO_TRAP_HANDLER_ENTRY_POINT2      DispatchFunction,
  IN OUT   FCH_SMM_IO_TRAP_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                                *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmIoTrapDispatch2UnRegister (
  IN       CONST FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                               DispatchHandle
  );

FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL gFchSmmIoTrapDispatch2Protocol = {
  FchSmmIoTrapDispatch2Register,
  FchSmmIoTrapDispatch2UnRegister
};

FCH_IO_TRAP_ENTRY  mFchIoTrapList[] = {
  { BIT20, 0,  0xC0, 0xF0, 0, 0, 0},
  { BIT21, 1,  0xC2, 0xF0, 0, 0, 0},
  { BIT22, 2,  0xC4, 0xF0, 0, 0, 0},
  { BIT23, 3,  0xC6, 0xF0, 0, 0, 0},
};

UINTN mFchIoTrapIndex = 0;

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM IO TRAP dispatcher handler
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
FchSmmIoTrapDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  UINT32      SmiIoTrapStatusBitmap;
  Status = EFI_UNSUPPORTED;

  SmiIoTrapStatusBitmap = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90);
  SmiIoTrapStatusBitmap &= BIT20 + BIT21 + BIT22 + BIT23;
  if (SmiIoTrapStatusBitmap != 0) {
    for (Index = 0; Index < (sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)); Index++) {
      if ((mFchIoTrapList[Index].StatusMask & SmiIoTrapStatusBitmap) != 0) {
        ACPIMMIO32 (ACPI_MMIO_BASE + 0x290) = mFchIoTrapList[Index].StatusMask;
        if (mFchIoTrapList[Index].DispatchFunction) {
          Status = mFchIoTrapList[Index].DispatchFunction (
                     mFchIoTrapList[Index].DispatchHandle,
                     &mFchIoTrapList[Index].Context,
                     NULL,
                     NULL
                     );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] IoTrap SMM handler dispatched: Address = 0x%04lx, return - %r\n", \
                 mFchIoTrapList[Index].Context.Address, Status));
        }
        break;
      }
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register IO TRAP child handler worker fucntion
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchFunction      Child dispcther function
 * @param[in, out]   RegisterContext            Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmIoTrapDispatch2RegisterWorker (
  IN       CONST FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_IO_TRAP_HANDLER_ENTRY_POINT2      DispatchFunction,
  IN OUT   FCH_SMM_IO_TRAP_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                                *DispatchHandle
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  UINT32      OrMask;
  UINT32      AndMask;
  BOOLEAN     SecondTrapFlag = FALSE; // Flag for ReadWriteTrap type to register Read/Write Trap seperately

  Status = EFI_OUT_OF_RESOURCES;

  ASSERT ((DispatchFunction != NULL) && (DispatchHandle != NULL));

  if (mFchIoTrapIndex == ((sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)))) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (DispatchFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((mFchIoTrapIndex == ((sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)) - 1)) && \
      (RegisterContext->Type == ReadWriteTrap)) {  // ReadWriteTrap need to take 2 sets of IO trap setting fields
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < (sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)); Index++) {
    if ((mFchIoTrapList[Index].DispatchFunction == NULL) && (Index == mFchIoTrapIndex))  {
      CopyMem (
        &mFchIoTrapList[Index].Context,
        RegisterContext,
        sizeof (FCH_SMM_IO_TRAP_REGISTER_CONTEXT)
        );
      mFchIoTrapList[Index].DispatchFunction = DispatchFunction;
      if(SecondTrapFlag == FALSE) {
        *DispatchHandle = &mFchIoTrapList[Index];
      }
      mFchIoTrapIndex ++;
      //Clear Status
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90) = mFchIoTrapList[Index].StatusMask;
      //Set base address
      ACPIMMIO16 (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REGC0 + Index * 2) = RegisterContext->Address;
      //Set Read Write Attribute
      ACPIMMIO32 (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REGCC) &= ~ (UINT32) (1 << Index);
      if ((RegisterContext->Type == WriteTrap) || (SecondTrapFlag == TRUE)) {
        ACPIMMIO32 (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REGCC) |= (UINT32) (1 << Index);
      }
      // Enable SMI
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC4) |= (UINT32) (1 << ((Index * 2) + 8));
      // Save in BootScriptTable for S3
      OrMask = 1 << ((Index * 2) + 8);
      AndMask = ~ (UINT32) (3 << ((Index * 2) + 8));
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC4),
        &OrMask,
        &AndMask
        );

      DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering IoTrap SMM handler: Address = 0x%04lx, Type = 0x%x\n", \
             RegisterContext->Address, RegisterContext->Type));
      Status = EFI_SUCCESS;
      if ((RegisterContext->Type == ReadWriteTrap) && (SecondTrapFlag == FALSE)) {
        SecondTrapFlag = TRUE; // Set SecondTrapFlag Flag to register Write Trap handler again.
        continue;
      } else {
        break;
      }
    }
  }
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister IO TRAP child handler worker function
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
FchSmmIoTrapDispatch2UnRegisterWorker (
  IN       CONST FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  UINT32      OrMask;
  UINT32      AndMask;
  BOOLEAN     SecondTrapFlag = FALSE; // Flag for ReadWriteTrap type to unregister Read/Write Trap seperately

  Status = EFI_NOT_FOUND;
  for (Index = 0; Index < (sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)); Index++) {
    if ((&mFchIoTrapList[Index] == DispatchHandle) || (SecondTrapFlag == TRUE)) {
      mFchIoTrapList[Index].DispatchFunction = NULL;
      //Clear Status ACTION. Is it safe? wll EOS be taken care?
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90) = mFchIoTrapList[Index].StatusMask;
      // Disable SMI
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC4) &= ~(UINT32) (3 << ((Index * 2) + 8));
      // Save in BootScriptTable for S3
      OrMask = 0;
      AndMask = ~ (UINT32) (3 << ((Index * 2) + 8));
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC4),
        &OrMask,
        &AndMask
        );
      Status = EFI_SUCCESS;

      if ((mFchIoTrapList[Index].Context.Type == ReadWriteTrap) && (SecondTrapFlag == FALSE)){
        SecondTrapFlag = TRUE;  // Set SecondTrapFlag Flag to unregister Write Trap handler again.
        continue;
      }
    }
  }
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Register IO TRAP child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchFunction      Child dispcther function
 * @param[in, out]   RegisterContext            Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmIoTrapDispatch2Register (
  IN       CONST FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_IO_TRAP_HANDLER_ENTRY_POINT2      DispatchFunction,
  IN OUT   FCH_SMM_IO_TRAP_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                                *DispatchHandle
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  RegisterStatus;

  Status = FchSmmIoTrapDispatch2RegisterWorker (
              This,
              DispatchFunction,
              RegisterContext,
              DispatchHandle
              );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmIoTrapDispatch2ProtocolGuid,
                                                    DispatchFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)RegisterContext,
                                                    sizeof (EFI_SMM_IO_TRAP_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Unregister IO TRAP child handler
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
FchSmmIoTrapDispatch2UnRegister (
  IN       CONST FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  UnregisterStatus;
  UINTN       Index;

  for (Index = 0; Index < (sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)); Index++) {
    if (&mFchIoTrapList[Index] == DispatchHandle) {
      if (mSmiHandlerProfileProtocol != NULL) {
        UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                          mSmiHandlerProfileProtocol,
                                                          &gEfiSmmIoTrapDispatch2ProtocolGuid,
                                                          mFchIoTrapList[Index].DispatchFunction,
                                                          (VOID*)&(mFchIoTrapList[Index].Context),
                                                          sizeof (EFI_SMM_IO_TRAP_REGISTER_CONTEXT)
                                                          );
        ASSERT_EFI_ERROR (UnregisterStatus);
      }
    }
  }

  Status = FchSmmIoTrapDispatch2UnRegisterWorker (
              This,
              DispatchHandle
              );

  return  Status;
}


EFI_STATUS
EFIAPI
EfiSmmIoTrapDispatch2Register (
  IN       CONST EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2             DispatchFunction,
  IN OUT   EFI_SMM_IO_TRAP_REGISTER_CONTEXT         *RegisterContext,
     OUT   EFI_HANDLE                               *DispatchHandle
  )
{
  EFI_STATUS                        Status;
  EFI_STATUS                        RegisterStatus;
  FCH_SMM_IO_TRAP_REGISTER_CONTEXT  FchRegisterContext;

  FchRegisterContext.Address = RegisterContext->Address;
  FchRegisterContext.Length  = RegisterContext->Length;
  FchRegisterContext.Type    = RegisterContext->Type;

  Status = FchSmmIoTrapDispatch2RegisterWorker (
              &gFchSmmIoTrapDispatch2Protocol,
              DispatchFunction,
              &FchRegisterContext,
              DispatchHandle
              );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmIoTrapDispatch2ProtocolGuid,
                                                    DispatchFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)&FchRegisterContext,
                                                    sizeof (EFI_SMM_IO_TRAP_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return Status;
}

EFI_STATUS
EFIAPI
EfiSmmIoTrapDispatch2UnRegister (
  IN       CONST EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  UnregisterStatus;
  UINTN       Index;

  for (Index = 0; Index < (sizeof (mFchIoTrapList) / sizeof (FCH_IO_TRAP_ENTRY)); Index++) {
    if (&mFchIoTrapList[Index] == DispatchHandle) {
      if (mSmiHandlerProfileProtocol != NULL) {
        UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                          mSmiHandlerProfileProtocol,
                                                          &gEfiSmmIoTrapDispatch2ProtocolGuid,
                                                          mFchIoTrapList[Index].DispatchFunction,
                                                          (VOID*)&(mFchIoTrapList[Index].Context),
                                                          sizeof (EFI_SMM_IO_TRAP_REGISTER_CONTEXT)
                                                          );
        ASSERT_EFI_ERROR (UnregisterStatus);
      }
    }
  }

  Status = FchSmmIoTrapDispatch2UnRegisterWorker (
              &gFchSmmIoTrapDispatch2Protocol,
              DispatchHandle
              );

  return Status;
}

EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL gEfiSmmIoTrapDispatch2Protocol = {
  EfiSmmIoTrapDispatch2Register,
  EfiSmmIoTrapDispatch2UnRegister
};
