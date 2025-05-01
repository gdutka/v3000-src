/** @file
  Terminal Esc Code definition

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _TERMINAL_ESC_CODE_H_
#define _TERMINAL_ESC_CODE_H_

#define TERMINAL_ESC_CODE_PROTOCOL_GUID \
  { \
    0xC40BC698, 0x0F16, 0x443c, 0xB2, 0xF0, 0xBA, 0x6B, 0x5B, 0x16, 0x37, 0x99\
  }

//
// Terminal Type Definition
// CAUTION : The TP_XXXX definition must sync with VFR's TerminalType order
//
#define TP_VT100                1
#define TP_VT100P               (1 << 1)
#define TP_VTUTF8               (1 << 2)
#define TP_PCANSI               (1 << 3)
#define TP_LOG                  (1 << 4)
#define TP_TTY                  (1 << 5)
#define TP_LINUX                (1 << 6)
#define TP_XTERM_R6             (1 << 7)
#define TP_VT400                (1 << 8)
#define TP_SCO                  (1 << 9)

//
// Function Type ID Definition
// NODE : ID number list below are reserved for backup compatible.
//        0xF9-0xFC, 0xF4
//
#define FUNC_RESET                        0xFF
#define FUNC_VIDEO                        0xFE
#define FUNC_SENSE                        0xFD
#define FUNC_AUTO_REFRESH                 0xF8
#define FUNC_MANUAL_REFRESH               0xF7
#define FUNC_CHARSET                      0xF6
#define FUNC_NONVT100_ALTKEY              0xF5

//
// Check State Definition
//
#define ESC_CODE_STATE_DEFAULT         0x00
#define ESC_CODE_STATE_NOT_MATCH       0x01
#define ESC_CODE_STATE_MATCHING        0x02

typedef struct {
  UINT16  EfiScanCode;
  UINT16  KbScanCode;
} EFI_TO_KB_SCANCODE_MAP;

typedef enum {
  ESC_CODE_SCANCODE,
  ESC_CODE_EXTENTION,
  ESC_CODE_CONTROL,
  ESC_CODE_ALTCODE,
  ESC_CODE_FUNC
} ESC_CODE_TYPE;

typedef struct {
  UINT16   EscCodeState;
  UINT16   TerminalType;
  UINT8    DataType;
  UINT16   Data;
  CHAR16   *EscSequenceCode;
} ESC_SEQUENCE_CODE;

typedef enum {
  TYPE_FUNCTION_ID,   
  TYPE_FUNCTION_POINTER
} SPECIAL_COMMAND_TYPE;

typedef struct _CR_SPECIAL_COMMAND {
  CHAR16                    *CommandStr;
  SPECIAL_COMMAND_TYPE      CommandType;
  UINT16                    Command;
} CR_SPECIAL_COMMAND;

typedef struct {
  UINT16                  EscCodeCount;
  ESC_SEQUENCE_CODE       *EscSequenceCode;
  EFI_TO_KB_SCANCODE_MAP  *EfiToKbScanCode;
  UINT16                  CrSpecialCommandCount;
  CR_SPECIAL_COMMAND      *CrSpecialCommand;
} EFI_TERMINAL_ESC_CODE_PROTOCOL;

extern EFI_GUID gTerminalEscCodeProtocolGuid;

#endif
