/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#include "FchSmmUsbDispatcher.h"
#include "Filecode.h"
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#define FILECODE FCH_KEITH_FCHKEITHSMMDISPATCHER_FCHSMMUSBDISPATCHER_FCHSMMUSBDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmUsbDispatch2Register (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_USB_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_USB_REGISTER_CONTEXT    *RegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmUsbDispatch2UnRegister (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  );

FCH_SMM_USB_DISPATCH2_PROTOCOL gFchSmmUsbDispatch2Protocol = {
  FchSmmUsbDispatch2Register,
  FchSmmUsbDispatch2UnRegister
};

//[-start-220902-IB14740260-add]//
//
// The device path used for SMI registration.
//
struct {
  ACPI_HID_DEVICE_PATH          Acpi;
  PCI_DEVICE_PATH               Pci;
  EFI_DEVICE_PATH_PROTOCOL      End;
} mKbcEmulatorDevicePath = {
  { ACPI_DEVICE_PATH, ACPI_DP, (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)), (UINT8)((sizeof (ACPI_HID_DEVICE_PATH)) >> 8), EISA_PNP_ID (0x0A03), 0 },
  { HARDWARE_DEVICE_PATH, HW_PCI_DP, (UINT8)(sizeof (PCI_DEVICE_PATH)), (UINT8)((sizeof (PCI_DEVICE_PATH)) >> 8), 0x03, 0x00 },
  { END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, END_DEVICE_PATH_LENGTH, 0 }
};

/*----------------------------------------------------------------------------------------*/
/**
 * IsKbcEmulator device path
 *
 *
 * @param[in]       DevicePath          DevicePath

 * @retval          TRUE
 * @retval          FALSE
 */
/*----------------------------------------------------------------------------------------*/
BOOLEAN
IsKbcEmulator (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  if (CompareMem (DevicePath, &mKbcEmulatorDevicePath, GetDevicePathSize ((EFI_DEVICE_PATH_PROTOCOL*)&mKbcEmulatorDevicePath)) == 0) return TRUE;
  return FALSE;
}
//[-end-220902-IB14740260-add]//

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM USB dispatcher handler
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
FchSmmUsbDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;

  Status = EFI_NOT_FOUND;

  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (CurrentFchSmmUsbNodePtr->FchUsbNodePtr!= NULL) {
//[-start-220902-IB14740260-modify]//
      if (CurrentFchSmmUsbNodePtr->CallBack2Function != NULL && !IsKbcEmulator (CurrentFchSmmUsbNodePtr->Context.Device)) {
//[-end-220902-IB14740260-modify]//
        Status = CurrentFchSmmUsbNodePtr->CallBack2Function (
                                            CurrentFchSmmUsbNodePtr->DispatchHandle,
                                            &CurrentFchSmmUsbNodePtr->Context,
                                            NULL,
                                            NULL
                                            );
        DEBUG ((DEBUG_VERBOSE, "[FchSmmDispatcher] USB SMM handler dispatched: Order = 0x%x, \
               return - %r\n", CurrentFchSmmUsbNodePtr->Context.Order, Status));
      }
      CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }
  }
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = UsbSmi;
  FchSmnRW (0, 0x13B10120, 0xfffffffe, BIT0, NULL);
//[-start-220902-IB14740260-modify]//
  FchSmnRW (0, 0x13B20020, 0xfffffeff, BIT8, NULL);
//[-end-220902-IB14740260-modify]//
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = UsbSmi; //PLAT-11747
  return  Status;
}

EFI_STATUS
EFIAPI
FchSmmUsbDispatchHandler2 (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;
  Status = EFI_NOT_FOUND;
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (CurrentFchSmmUsbNodePtr->FchUsbNodePtr!= NULL) {
//[-start-220902-IB14740260-modify]//
      if (CurrentFchSmmUsbNodePtr->CallBack2Function != NULL && IsKbcEmulator (CurrentFchSmmUsbNodePtr->Context.Device)) {
//[-end-220902-IB14740260-modify]//
        Status = CurrentFchSmmUsbNodePtr->CallBack2Function (
                                            CurrentFchSmmUsbNodePtr->DispatchHandle,
                                            &CurrentFchSmmUsbNodePtr->Context,
                                            NULL,
                                            NULL
                                            );
        DEBUG ((DEBUG_VERBOSE, "[FchSmmDispatcher] USB SMM handler dispatched: Order = 0x%x, \
               return - %r\n", CurrentFchSmmUsbNodePtr->Context.Order, Status));
      }
      CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }
  }
//[-start-220902-IB14740260-modify]//
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = BIT26;
//[-end-220902-IB14740260-modify]//
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register USB child handler worker function
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   RegisterContext            Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmUsbDispatch2RegisterWorker (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_USB_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_USB_REGISTER_CONTEXT    *RegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *NewFchSmmUsbNodePtr;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;
  FCH_SMM_USB_NODE *PreviousFchSmmUsbNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
//[-start-220902-IB14740260-remove]//
//  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT24;
//[-end-220902-IB14740260-remove]//
  ACPIMMIO32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED) |= (BIT4);

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_USB_NODE),
                    &NewFchSmmUsbNodePtr
                    );
  ASSERT_EFI_ERROR (Status);

  NewFchSmmUsbNodePtr->CallBack2Function = CallBackFunction;
  NewFchSmmUsbNodePtr->Context = *RegisterContext;
  *DispatchHandle = &NewFchSmmUsbNodePtr->DispatchHandle;
  NewFchSmmUsbNodePtr->DispatchHandle = *DispatchHandle;

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering USB SMM handler: Type = 0x%x, Order = 0x%x\n", \
         RegisterContext->Type, RegisterContext->Order));
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    NewFchSmmUsbNodePtr->FchUsbNodePtr = HeadFchSmmUsbNodePtr;
    HeadFchSmmUsbNodePtr = NewFchSmmUsbNodePtr;
  } else {
    PreviousFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (CurrentFchSmmUsbNodePtr->FchUsbNodePtr != NULL) {
      if (NewFchSmmUsbNodePtr->Context.Order <= CurrentFchSmmUsbNodePtr->Context.Order
          && CurrentFchSmmUsbNodePtr->Context.Type == NewFchSmmUsbNodePtr->Context.Type
          && (CurrentFchSmmUsbNodePtr->Context.Device != NULL
               && NewFchSmmUsbNodePtr->Context.Device != NULL
               && CompareMem (CurrentFchSmmUsbNodePtr->Context.Device,
                                NewFchSmmUsbNodePtr->Context.Device,
                                GetDevicePathSize (NewFchSmmUsbNodePtr->Context.Device)) == 0))
      {
        if (PreviousFchSmmUsbNodePtr == CurrentFchSmmUsbNodePtr) {
          NewFchSmmUsbNodePtr->FchUsbNodePtr = HeadFchSmmUsbNodePtr;
          HeadFchSmmUsbNodePtr = NewFchSmmUsbNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmUsbNodePtr->FchUsbNodePtr = PreviousFchSmmUsbNodePtr->FchUsbNodePtr;
        PreviousFchSmmUsbNodePtr->FchUsbNodePtr = NewFchSmmUsbNodePtr;

        Status = EFI_SUCCESS;
        return  Status;
      }
      PreviousFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr;
      CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }
    PreviousFchSmmUsbNodePtr->FchUsbNodePtr = NewFchSmmUsbNodePtr;
    NewFchSmmUsbNodePtr->FchUsbNodePtr = CurrentFchSmmUsbNodePtr;
  }

//[-start-220902-IB14740260-modify]//
  FchSmnRW (0, 0x13B20020, 0xfffffeff, BIT8, NULL);
//[-end-220902-IB14740260-modify]//
//[-start-220901-IB14740259-modify]//
  FchSmnRW (0, 0x13B200F4, 0xffffbfff, BIT14, NULL);
//[-end-220901-IB14740259-modify]//
//  FchSmnRW (0, 0x16D80118, 0xfffff000, 0x100, NULL);
//  FchSmnRW (0, 0x16F80118, 0xfffff000, 0x100, NULL);
  FchSmnRW (0, 0x13B10120, 0xfffffffe, BIT0, NULL);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = UsbSmi;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC) |= BIT10;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = UsbSmi;

//[-start-220902-IB14740260-add]//
  // USB Legacy KBC Emulation
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = BIT26;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) |= BIT20;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = BIT26;
//[-end-220902-IB14740260-add]//

  Status = EFI_SUCCESS;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister USB child handler worker fucntion
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
FchSmmUsbDispatch2UnRegisterWorker (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;
  FCH_SMM_USB_NODE *PreviousFchSmmUsbNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    if (CurrentFchSmmUsbNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    } else {
      while (CurrentFchSmmUsbNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr;
        CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
        if (CurrentFchSmmUsbNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmUsbNodePtr->FchUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }

    Status = gSmst->SmmFreePool (
                      CurrentFchSmmUsbNodePtr
                      );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register USB child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   RegisterContext            Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmUsbDispatch2Register (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_USB_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_USB_REGISTER_CONTEXT    *RegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  EFI_STATUS       Status;
  EFI_STATUS       RegisterStatus;

//[-start-221003-IB14740265-add]//
  if (PcdGet8 (PcdUsbEfiTimerPolling) == 1) {
    return EFI_UNSUPPORTED;
  }
//[-end-221003-IB14740265-add]//
  Status = FchSmmUsbDispatch2RegisterWorker (
              This,
              CallBackFunction,
              RegisterContext,
              DispatchHandle
              );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmUsbDispatch2ProtocolGuid,
                                                    CallBackFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)RegisterContext,
                                                    sizeof (EFI_SMM_USB_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Unregister USB child handler
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
FchSmmUsbDispatch2UnRegister (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  EFI_STATUS       Status;
  EFI_STATUS       UnregisterStatus;
  FCH_SMM_USB_NODE *FchSmmUsbNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
//[-start-221003-IB14740265-add]//
  if (PcdGet8 (PcdUsbEfiTimerPolling) == 1) {
    return EFI_UNSUPPORTED;
  }
//[-end-221003-IB14740265-add]//
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (FchSmmUsbNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmUsbNodePtr = FchSmmUsbNodePtr->FchUsbNodePtr;
      if (FchSmmUsbNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                      mSmiHandlerProfileProtocol,
                                                      &gEfiSmmUsbDispatch2ProtocolGuid,
                                                      FchSmmUsbNodePtr->CallBack2Function,
                                                      (VOID*)&(FchSmmUsbNodePtr->Context),
                                                      sizeof (EFI_SMM_USB_REGISTER_CONTEXT)
                                                      );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmUsbDispatch2UnRegisterWorker (
              This,
              DispatchHandle
              );

  return  Status;
}

EFI_STATUS
EFIAPI
EfiSmmUsbDispatch2Register (
  IN       CONST EFI_SMM_USB_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2         DispatchFunction,
  IN       CONST EFI_SMM_USB_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  )
{
  EFI_STATUS                       Status;
  EFI_STATUS                       RegisterStatus;
  FCH_SMM_USB_REGISTER_CONTEXT     FchRegisterContext;

//[-start-221003-IB14740265-add]//
  if (PcdGet8 (PcdUsbEfiTimerPolling) == 1) {
    return EFI_UNSUPPORTED;
  }
//[-end-221003-IB14740265-add]//
  FchRegisterContext.Type   = RegisterContext->Type;
  FchRegisterContext.Device = RegisterContext->Device;
  FchRegisterContext.Order  = 0x80;      // SMM_DEFAULT_ORDER
  Status = FchSmmUsbDispatch2RegisterWorker (
           &gFchSmmUsbDispatch2Protocol,
           DispatchFunction,
           &FchRegisterContext,
           DispatchHandle
           );

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    RegisterStatus = mSmiHandlerProfileProtocol->RegisterHandler (
                                                    mSmiHandlerProfileProtocol,
                                                    &gEfiSmmUsbDispatch2ProtocolGuid,
                                                    DispatchFunction,
                                                    (PHYSICAL_ADDRESS)(UINTN)RETURN_ADDRESS (0),
                                                    (VOID*)RegisterContext,
                                                    sizeof (EFI_SMM_USB_REGISTER_CONTEXT)
                                                    );
    ASSERT_EFI_ERROR (RegisterStatus);
  }

  return Status;
}

EFI_STATUS
EFIAPI
EfiSmmUsbDispatch2UnRegister (
  IN       CONST EFI_SMM_USB_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                           DispatchHandle
  )
{
  EFI_STATUS          Status;
  EFI_STATUS          UnregisterStatus;
  FCH_SMM_USB_NODE    *FchSmmUsbNodePtr;
  Status = EFI_OUT_OF_RESOURCES;
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
//[-start-221003-IB14740265-add]//
  if (PcdGet8 (PcdUsbEfiTimerPolling) == 1) {
    return EFI_UNSUPPORTED;
  }
//[-end-221003-IB14740265-add]//
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    FchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (FchSmmUsbNodePtr->DispatchHandle != DispatchHandle) {
      FchSmmUsbNodePtr = FchSmmUsbNodePtr->FchUsbNodePtr;
      if (FchSmmUsbNodePtr->DispatchHandle == NULL) {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  if ((mSmiHandlerProfileProtocol != NULL) && (!EFI_ERROR(Status))) {
    UnregisterStatus = mSmiHandlerProfileProtocol->UnregisterHandler (
                                                      mSmiHandlerProfileProtocol,
                                                      &gEfiSmmUsbDispatch2ProtocolGuid,
                                                      FchSmmUsbNodePtr->CallBack2Function,
                                                      (VOID*)&(FchSmmUsbNodePtr->Context),
                                                      sizeof (EFI_SMM_USB_REGISTER_CONTEXT)
                                                      );
    ASSERT_EFI_ERROR (UnregisterStatus);
  }

  Status = FchSmmUsbDispatch2UnRegisterWorker (
           &gFchSmmUsbDispatch2Protocol,
           DispatchHandle
           );

  return Status;
}

EFI_SMM_USB_DISPATCH2_PROTOCOL gEfiSmmUsbDispatch2Protocol = {
  EfiSmmUsbDispatch2Register,
  EfiSmmUsbDispatch2UnRegister
};


