/** @file

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef H2O_CONSOLE_BOOT_MSG_DXE_H_
#define H2O_CONSOLE_BOOT_MSG_DXE_H_

#include <Uefi.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>

#include <Library/DebugLib.h>

typedef struct {
  UINT32                                Signature;
  UINT8                                 Priority;
  LIST_ENTRY                            Link;
  H2O_CONSOLE_BOOT_MESSAGE_INIT_FUNC    InitFunction;
  H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC  OutputFunction;
  VOID                                  *Context;
} H2O_CONSOLE_BOOT_MSG_REGISTRATION;

#define H2O_CONSOLE_BOOT_MSG_REGISTRATION_SIGNATURE  SIGNATURE_32 ('C', 'B', 'M', 'R')
#define H2O_CONSOLE_BOOT_MSG_REGISTRATION_FROM_LINK(a) \
  CR (a, H2O_CONSOLE_BOOT_MSG_REGISTRATION, Link, H2O_CONSOLE_BOOT_MSG_REGISTRATION_SIGNATURE)

typedef struct {
  UINT32                             Signature;
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  Protocol;
  LIST_ENTRY                         RegistrationList;
  UINT8                              HighestPriority;
} H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA;

#define H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('C', 'B', 'M', 'P')
#define H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA, Protocol, H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA_SIGNATURE)

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
  );

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
  );

/**
  Dispatches all Console Boot Message drivers and prints messages to all
  consoles.

  @implements H2O_CONSOLE_BOOT_MESSAGE_DISPATCH

  @param[in] This  A pointer to the H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL instance.

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
ConsoleBootMsgDispatch (
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *This
  );

/**
  Writes a string to the console until the end of the console row is reached or
  a line break is found.

  @implements H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_LINE

  @param[in,out] Console       An output console (a Simple Text Output Protocol
                               instance).
  @param[in]     MaxColumn     Maximum column count for string output. This
                               value must not be greater than the number of
                               columns of the current mode of Console.
  @param[in,out] StringBuffer  The Null-terminated string to be displayed on
                               Console. The string buffer must be mutable. (The
                               functions modifies the string internally during
                               the process and restores to the original string
                               on return.)

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsoleOutputLine (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN OUT CHAR16                           *StringBuffer
  );

/**
  Prints a formatted Unicode string to a console, with line wrapping and
  optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in,out] FormatBuffer
  @param[in]     Marker

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsoleVaPrint (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN OUT CHAR16                           *FormatBuffer,
  IN VA_LIST                              Marker
  );

/**
  Prints a formatted Unicode string to a console, with line wrapping and
  optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_PRINT

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in]     Format
  @param[in]     ...

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsolePrint (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN CONST CHAR16                         *Format,
  ...
  );

/**
  Prints a formatted string as specified in a HII string package to a console,
  with line wrapping and optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT_HII_STRING

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in]     HiiHandle
  @param[in]     FormatStringId
  @param[in]     Language
  @param[in]     Marker

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsoleVaPrintHiiString (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN EFI_STRING_ID                        FormatStringId,
  IN CONST CHAR8                          *Language OPTIONAL,
  IN VA_LIST                              Marker
  );

/**
  Prints a formatted string as specified in a HII string package to a console,
  with line wrapping and optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_PRINT_HII_STRING

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in]     HiiHandle
  @param[in]     FormatStringId
  @param[in]     Language
  @param[in]     ...

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsolePrintHiiString (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxWidth,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN EFI_STRING_ID                        FormatStringId,
  IN CONST CHAR8                          *Language OPTIONAL,
  ...
  );

#endif // H2O_CONSOLE_BOOT_MSG_DXE_H_
