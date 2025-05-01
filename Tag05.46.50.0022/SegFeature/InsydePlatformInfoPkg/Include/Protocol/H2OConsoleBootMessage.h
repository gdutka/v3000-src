/** @file
  H2O Console Boot Message Protocol interface definitions

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

#ifndef H2O_CONSOLE_BOOT_MESSAGE_H_
#define H2O_CONSOLE_BOOT_MESSAGE_H_

#include <Uefi.h>

#include <Protocol/SimpleTextOut.h>

#define H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL_GUID \
  { \
    0x7dbba8b9, 0xb670, 0x482e, { 0xbe, 0x71, 0x0a, 0x07, 0x89, 0xa5, 0x9c, 0x42 } \
  }

typedef struct _H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL;

typedef enum {
  H2OConsoleBootMessagePriorityLow      = 4,
  H2OConsoleBootMessagePriorityMedium   = 8,
  H2OConsoleBootMessagePriorityHigh     = 12
} H2O_CONSOLE_BOOT_MESSAGE_PRIORITY;

/**
  Initializes context data for a Console Boot Message output.

  @param[in,out] Context  A pointer to the context data.

  @retval  EFI_SUCCESS          The context data was initialized successfully.
  @retval  EFI_ALREADY_STARTED  The context data was initialized already.
  @retval  Others               An error occurred. If a driver returns this
                                status, the corresponding output function will
                                not be executed for any console.

**/
typedef
EFI_STATUS
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_INIT_FUNC) (
  IN OUT VOID  *Context OPTIONAL
  );

/**
  Outputs boot messages for a console.

  @param[in]  Console    An output console (a Simple Text Output Protocol
                         instance).
  @param[in]  MaxColumn  Maximum column count of the current mode of Console.
  @param[in]  MaxRow     Maximum row count allocated for the current mode of
                         Console for boot messages. This excludes rows at the
                         bottom reserved for badging strings and hot key
                         instruction lines.
  @param[in]  Context    A pointer to the context data. Optional.

  @retval  EFI_SUCCESS   The messages are output successfully.
  @retval  Others        An error occurred.

**/
typedef
EFI_STATUS
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context OPTIONAL
  );

/**
  Registers a driver that prints messages to consoles at boot time.

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
typedef
EFI_STATUS
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_REGISTER_DRIVER) (
  IN OUT H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *This,
  IN H2O_CONSOLE_BOOT_MESSAGE_PRIORITY      Priority,
  IN H2O_CONSOLE_BOOT_MESSAGE_INIT_FUNC     InitFunction OPTIONAL,
  IN H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC   OutputFunction,
  IN VOID                                   *Context OPTIONAL
  );

/**
  Appends a boot message to all consoles. This function may be called in an
  event by a Console Boot Message driver for a notification message.

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
typedef
EFI_STATUS
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_APPEND_OUTPUT) (
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL     *This,
  IN H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_FUNC  OutputFunction,
  IN VOID                                  *Context OPTIONAL
  );

/**
  Dispatches all Console Boot Message drivers and prints messages to all
  consoles.

  @param[in] This  A pointer to the H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL instance.

  @retval  EFI_SUCCESS

**/
typedef
EFI_STATUS
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_DISPATCH) (
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *This
  );

/**
  Writes a string to the console until the end of the console row is reached or
  a line break is found.

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
typedef
UINTN
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_LINE) (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN OUT CHAR16                           *StringBuffer
  );

/**
  Prints a formatted Unicode string to a console, with line wrapping and
  optional color attributes.

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
typedef
UINTN
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT) (
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
typedef
UINTN
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_PRINT) (
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
typedef
UINTN
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT_HII_STRING) (
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
typedef
UINTN
(EFIAPI *H2O_CONSOLE_BOOT_MESSAGE_PRINT_HII_STRING) (
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

struct _H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL {
  H2O_CONSOLE_BOOT_MESSAGE_REGISTER_DRIVER      RegisterDriver;
  H2O_CONSOLE_BOOT_MESSAGE_APPEND_OUTPUT        AppendOutput;
  H2O_CONSOLE_BOOT_MESSAGE_DISPATCH             Dispatch;
  H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_LINE          ConsoleOutputLine;
  H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT             ConsoleVaPrint;
  H2O_CONSOLE_BOOT_MESSAGE_PRINT                ConsolePrint;
  H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT_HII_STRING  ConsoleVaPrintHiiString;
  H2O_CONSOLE_BOOT_MESSAGE_PRINT_HII_STRING     ConsolePrintHiiString;
};

extern EFI_GUID gH2OConsoleBootMessageProtocolGuid;

#endif // H2O_CONSOLE_BOOT_MESSAGE_H_
