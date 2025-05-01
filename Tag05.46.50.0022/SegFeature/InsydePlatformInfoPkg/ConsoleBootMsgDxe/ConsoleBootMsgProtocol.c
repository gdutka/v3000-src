/** @file

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "ConsoleBootMsgDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

/**
  Registers a driver that prints messages to consoles at boot time.

  @implements H2O_CONSOLE_BOOT_MESSAGE_REGISTER_DRIVER

  @param[in,out] This            A pointer to the
                                 H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL instance.
  @param[in]     Priority        A value to determine the order of dispatching.
  @param[in]     InitFunction    A pointer to the function that initializes the
                                 context. Optional.
  @param[in]     OutputFunction  A pointer to the output function.
  @param[in]     Context         A pointer to the context data. This will be
                                 passed as the Context parameter to both
                                 InitFunction and OutputFunction. Optional.

  @retval  EFI_SUCCESS            Driver is registered successfully.
  @retval  EFI_INVALID_PARAMETER  The This parameter is NULL.
  @retval  EFI_INVALID_PARAMETER  OutputFunction is NULL.
  @retval  EFI_INVALID_PARAMETER  The Priority value is invalid.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory.

**/
EFI_STATUS
EFIAPI
ConsoleBootMsgRegisterDriver (
  IN OUT H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *This,
  IN H2O_CONSOLE_BOOT_MESSAGE_PRIORITY      Priority,
  IN H2O_CONSOLE_BOOT_MESSAGE_INIT_FUNC     InitFunction OPTIONAL,
  IN H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC   OutputFunction,
  IN VOID                                   *Context OPTIONAL
  )
{
  H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA  *Private;
  H2O_CONSOLE_BOOT_MSG_REGISTRATION  *Registration;

  if (This == NULL || (UINTN) Priority > MAX_UINT8 || OutputFunction == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA_FROM_THIS (This);

  Registration = AllocateZeroPool (sizeof (H2O_CONSOLE_BOOT_MSG_REGISTRATION));
  if (Registration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Registration->Signature      = H2O_CONSOLE_BOOT_MSG_REGISTRATION_SIGNATURE;
  Registration->Priority       = (UINT8) Priority;
  Registration->InitFunction   = InitFunction;
  Registration->OutputFunction = OutputFunction;
  Registration->Context        = Context;
  InsertTailList (&Private->RegistrationList, &Registration->Link);

  if ((UINT8) Priority > Private->HighestPriority) {
    Private->HighestPriority = (UINT8) Priority;
  }

  DEBUG ((
    EFI_D_INFO,
    "%a: Registered. (Priority=%u, InitFn=%p, OutputFn=%p, Context=%p)\n",
    gEfiCallerBaseName,
    (unsigned int) Priority,
    InitFunction,
    OutputFunction,
    Context
    ));

  return EFI_SUCCESS;
}

/**
  @param[in]  Console         An output console (a Simple Text Output Protocol
                              instance).
  @param[in]  OutputFunction  A pointer to the output function.
  @param[in]  Context         A pointer to the context data which
                              OutputFunction may refer to. Optional.

  @retval  EFI_SUCCESS
  @retval  EFI_UNSUPPORTED
  @retval  Others

**/
STATIC
EFI_STATUS
InternalAppendOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *Console,
  IN H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC  OutputFunction,
  IN VOID                                  *Context OPTIONAL
  )
{
  INT32       CurrentMode;
  UINTN       MaxColumn;
  UINTN       MaxRow;
  EFI_STATUS  Status;

  CurrentMode = Console->Mode->Mode;
  if (CurrentMode == -1) {
    DEBUG ((
      EFI_D_WARN,
      "%a: Invalid text mode for console %p.\n",
      gEfiCallerBaseName,
      Console
      ));
    return EFI_UNSUPPORTED;
  }
  Console->QueryMode (Console, (UINTN) CurrentMode, &MaxColumn, &MaxRow);
  MaxColumn -= PcdGet16 (PcdH2OConsoleBootMessageMarginRight);
  MaxRow    -= PcdGet16 (PcdH2OConsoleBootMessageMarginBottom);

  Status = OutputFunction (Console, MaxColumn, MaxRow, Context);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Output Function %p returns status: %r\n",
      gEfiCallerBaseName,
      OutputFunction,
      Status
      ));
  }
  return Status;
}

/**
  Appends a boot message to all consoles. This function may be called in an
  event by a Console Boot Message driver for a notification message.

  @implements H2O_CONSOLE_BOOT_MESSAGE_APPEND_OUTPUT

  @param[in]  This            A pointer to the
                              H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL instance.
  @param[in]  OutputFunction  A pointer to the output function.
  @param[in]  Context         A pointer to the context data which
                              OutputFunction may refer to. Optional.

  @retval  EFI_SUCCESS
  @retval  EFI_INVALID_PARAMETER  The This parameter is NULL.
  @retval  EFI_INVALID_PARAMETER  OutputFunction is NULL.
  @retval  EFI_NOT_FOUND          No output device is found.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory to complete the operation.

**/
EFI_STATUS
EFIAPI
ConsoleBootMsgAppendOutput (
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL     *This,
  IN H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC  OutputFunction,
  IN VOID                                  *Context OPTIONAL
  )
{
  EFI_STATUS                       Status;
  UINTN                            NoHandles;
  EFI_HANDLE                       *HandleBuffer;

  EFI_STATUS                       WarnStatus;
  EFI_TPL                          OldTpl;
  UINTN                            Index;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console;

  if (This == NULL || OutputFunction == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleTextOutProtocolGuid,
                  NULL, // SearchKey (No registration)
                  &NoHandles,
                  &HandleBuffer
                  );
  if (!EFI_ERROR (Status) && NoHandles <= 0) {
    Status = EFI_NOT_FOUND;
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: LocateHandleBuffer (%a) failed: %r\n",
      gEfiCallerBaseName,
      "SimpleTextOut",
      Status
      ));
    return Status;
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  for (Index = 0; Index < NoHandles; Index++) {
    WarnStatus = gBS->OpenProtocol (
                        HandleBuffer[Index],
                        &gEfiSimpleTextOutProtocolGuid,
                        (VOID **) &Console,
                        gImageHandle, // AgentHandle
                        NULL,         // ControllerHandle
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL
                        );
    if (EFI_ERROR (WarnStatus)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a: Failed to get %a on handle %p: %r\n",
        gEfiCallerBaseName,
        "SimpleTextOut",
        HandleBuffer[Index],
        WarnStatus
        ));
      continue;
    }
    if (Console == gST->ConOut || Console == gST->StdErr) {
      continue;
    }

    (VOID) InternalAppendOutput (Console, OutputFunction, Context);
  }

  gBS->RestoreTPL (OldTpl);

  gBS->FreePool (HandleBuffer);

  return EFI_SUCCESS;
}

/**
  Dispatches all Console Boot Message drivers and prints messages to all
  consoles.

  @implements H2O_CONSOLE_BOOT_MESSAGE_DISPATCH

  @param[in] This  A pointer to the H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL instance.

  @retval  EFI_SUCCESS
  @retval  EFI_INVALID_PARAMETER

**/
EFI_STATUS
EFIAPI
ConsoleBootMsgDispatch (
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *This
  )
{
  H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA  *Private;
  UINTN                              Priority;
  LIST_ENTRY                         *Node;
  H2O_CONSOLE_BOOT_MSG_REGISTRATION  *Registration;
  EFI_STATUS                         Status;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Private = H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA_FROM_THIS (This);

  for (Priority = (UINTN) Private->HighestPriority;
      Priority != 0;
      Priority--)
  {
    for (Node = GetFirstNode (&Private->RegistrationList);
        !IsNull (&Private->RegistrationList, Node);
        Node = GetNextNode (&Private->RegistrationList, Node))
    {
      Registration = H2O_CONSOLE_BOOT_MSG_REGISTRATION_FROM_LINK (Node);
      if (Registration->Priority != Priority) {
        continue;
      }

      DEBUG ((
        EFI_D_VERBOSE,
        "%a: Priority=%u, InitFn=%p, OutputFn=%p, Context=%p\n",
        gEfiCallerBaseName,
        (unsigned int) Priority,
        Registration->InitFunction,
        Registration->OutputFunction,
        Registration->Context
        ));

      if (Registration->InitFunction != NULL) {
        Status = Registration->InitFunction (Registration->Context);
        if (EFI_ERROR (Status)) {
          DEBUG ((
            EFI_D_ERROR,
            "%a: Init Function %p returns status: %r\n",
            gEfiCallerBaseName,
            Registration->InitFunction,
            Status
            ));
          if (Status != EFI_ALREADY_STARTED) {
            continue;
          }
        }
      }

      (VOID) ConsoleBootMsgAppendOutput (
               This,
               Registration->OutputFunction,
               Registration->Context
               );
    }
  }

  return EFI_SUCCESS;
}
