/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _APCB_MEM_ERROR_OUT_INDICATOR_OPCODE_H_
#define _APCB_MEM_ERROR_OUT_INDICATOR_OPCODE_H_
//
// @APCB_START
//


///===============================================================================
typedef struct {
  UINT16                OpCode;
  UINT16                Length;
  UINT32                Width;
  UINT32                Address;
} SCRIPT_IO_WRITE;


typedef struct {
  UINT16                OpCode;
  UINT16                Length;
  UINT32                Width;
  UINT32                Address;
} SCRIPT_MMIO_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT16                Length;
  UINT32                Width;
  UINT32                Address;
} SCRIPT_PCICFG_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT16                Length;
  UINT32                Duration;
  UINT32                Reserved;
} SCRIPT_STALL;

typedef struct {
  UINT16                OpCode;
  UINT16                Length;
  UINT32                Width;
  UINT32                Address;
  UINT32                Delay;
} SCRIPT_IO_POLL;

typedef struct {
  UINT16  OpCode;
  UINT16  Length;
  UINT32  Version;
  UINT32  TableLength;
  UINT16  Reserved[2];
} SCRIPT_TABLE_HEADER;

typedef struct {
  UINT16  OpCode;
  UINT16  Length;
} SCRIPT_TERMINATE;

///===============================================================================
#define SCRIPT_IO_WRITE_OPCODE                 0x00
#define SCRIPT_MMIO_WRITE_OPCODE               0x02
#define SCRIPT_PCICFG_WRITE_OPCODE             0x04
#define SCRIPT_STALL_OPCODE                    0x07
#define SCRIPT_IO_POLL_OPCODE                  0x0D
#define SCRIPT_TERMINATE_OPCODE                0xFF

#define SCRIPT_TABLE_ONE_TIME_EXECUTE_OPCODE   0xAA
#define SCRIPT_TABLE_REPEATED_EXECUTE_OPCODE   0xAB
#define SCRIPT_TABLE_DISABLE_OPCODE            0xAF

typedef enum {
  ScriptAccessWidthNone = 0,                                      ///< dummy access width
  ScriptAccessWidth8    = 1,                                      ///< Access width is 8 bits.
  ScriptAccessWidth16,                                            ///< Access width is 16 bits.
  ScriptAccessWidth32,                                            ///< Access width is 32 bits.
} SCRIPT_ACCESS_WIDTH;


#endif // _APCB_MEM_ERROR_OUT_INDICATOR_OPCODE_H_



