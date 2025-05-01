/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#ifndef _FCH_SMM_USB_DISPATCHER_H_
#define _FCH_SMM_USB_DISPATCHER_H_

#include "FchSmmDispatcher.h"

#include <Protocol/SmmUsbDispatch2.h>
#include <Protocol/FchSmmUsbDispatch2.h>

extern  FCH_SMM_USB_DISPATCH2_PROTOCOL gFchSmmUsbDispatch2Protocol;
extern  EFI_SMM_USB_DISPATCH2_PROTOCOL gEfiSmmUsbDispatch2Protocol;

EFI_STATUS
EFIAPI
FchSmmUsbDispatchHandler (
  IN      EFI_HANDLE     SmmImageHandle,
  IN OUT  VOID           *CommunicationBuffer OPTIONAL,
  IN OUT  UINTN          *SourceSize OPTIONAL
  );

EFI_STATUS
EFIAPI
FchSmmUsbDispatchHandler2 (
  IN      EFI_HANDLE     SmmImageHandle,
  IN OUT  VOID           *CommunicationBuffer OPTIONAL,
  IN OUT  UINTN          *SourceSize OPTIONAL
  );
///
/// USB SMI Node
///
typedef struct _FCH_SMM_USB_NODE {
  EFI_HANDLE                           DispatchHandle;         ///< Dispatch Hangle
  FCH_SMM_USB_REGISTER_CONTEXT         Context;                ///< Register context
  FCH_SMM_USB_HANDLER_ENTRY_POINT2     CallBack2Function;      ///< SMM handler entry point
  struct _FCH_SMM_USB_NODE             *FchUsbNodePtr;         ///< pointer to next node
} FCH_SMM_USB_NODE;

extern  FCH_SMM_USB_NODE               *HeadFchSmmUsbNodePtr;
#endif

