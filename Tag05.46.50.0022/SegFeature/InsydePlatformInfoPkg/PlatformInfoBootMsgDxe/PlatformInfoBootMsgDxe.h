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

#ifndef PLATFORM_INFO_BOOT_MSG_DXE_H_
#define PLATFORM_INFO_BOOT_MSG_DXE_H_

#include <Uefi.h>

#include <Protocol/H2OConsoleBootMessage.h>
#include <Protocol/H2ODeviceInfoLookup.h>

#include <Library/PcdLib.h>

#define MAX_STRING_SIZE   1000

typedef struct {
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg;
  EFI_HII_HANDLE                     HiiHandle;
  BOOLEAN                            IsUefiOsFastBootActive;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup;
  EFI_EVENT                          SataDriveInfoEvent;
  EFI_EVENT                          NvmeInfoEvent;
  EFI_EVENT                          SdEmmcInfoEvent;
  VOID                               *SataDriveInfoRegistration;
  VOID                               *NvmeInfoRegistration;
  VOID                               *SdEmmcInfoRegistration;
  BOOLEAN                            RecordCursorPosition;
  UINTN                              RecordTailCursorCol;
  UINTN                              RecordTailCursorRow;
  BOOLEAN                            IsSataDeviceExist;
  BOOLEAN                            IsNvmeDeviceExist;
  BOOLEAN                            IsSdEmmcExist;
} H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT;

VOID
FreePoolIfNonNull (
  IN VOID  *Buffer
  );

//
// BasicSystemInfo.c
//

/**
  Displays common system information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
BasicSystemInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  );

/**
  Handler function for displaying processor and cache information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
CpuInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  );

/**
  Handler function for displaying memory information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
MemoryInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  );

#ifdef INSYDEIPMIPKG_IMPORTED
/**
  Handler function for displaying BMC information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
BmcInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  );
#endif

/**
  Handler function for displaying USB (peripheral) information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
InputDeviceInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  );

/**
  Handler function for displaying SATA drive information.

  @param[in]  Console    A pointer to the instance of Simple Text Output protocol.
  @param[in]  MaxColumn  Maximum column count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  MaxRow     Maximum row count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  Context    A pointer to the instance of platform info boot message context.

  @retval EFI_SUCCESS             Successfully display system information.
  @retval EFI_UNSUPPORTED         This display system information is disabled.
  @retval EFI_INVALID_PARAMETER   There is a problem with the Context pointer.
  @retval EFI_NOT_FOUND           The informaion protocol not reday.

**/
EFI_STATUS
EFIAPI
SataDriveInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context
  );

/**
  Handler function for displaying NVME information.

  @param[in]  Console    A pointer to the instance of Simple Text Output protocol.
  @param[in]  MaxColumn  Maximum column count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  MaxRow     Maximum row count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  Context    A pointer to the instance of platform info boot message context.

  @retval EFI_SUCCESS             Successfully display system information.
  @retval EFI_UNSUPPORTED         This display system information is disabled.
  @retval EFI_INVALID_PARAMETER   There is a problem with the Context pointer.
  @retval EFI_NOT_FOUND           The informaion protocol not reday.

**/
EFI_STATUS
EFIAPI
NvmeInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context
  );

/**
  Handler function for displaying Sd/eMMc information.

  @param[in]  Console    A pointer to the instance of Simple Text Output protocol.
  @param[in]  MaxColumn  Maximum column count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  MaxRow     Maximum row count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  Context    A pointer to the instance of platform info boot message context.

  @retval EFI_SUCCESS             Successfully display system information.
  @retval EFI_UNSUPPORTED         This display system information is disabled.
  @retval EFI_INVALID_PARAMETER   There is a problem with the Context pointer.
  @retval EFI_NOT_FOUND           The informaion protocol not reday.

**/
EFI_STATUS
EFIAPI
SdEmmcInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context
  );

#endif // PLATFORM_INFO_BOOT_MSG_DXE_H_
