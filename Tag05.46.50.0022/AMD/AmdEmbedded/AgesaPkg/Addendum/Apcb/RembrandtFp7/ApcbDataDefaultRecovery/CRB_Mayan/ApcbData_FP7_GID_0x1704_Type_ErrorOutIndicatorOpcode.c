/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

//
// Helper Marco to encode script entry
//
#define MAKE_SCRIPT_SBDFO(Seg, Bus, Dev, Fun, Off) ((((UINT32) (Seg)) << 28) | (((UINT32) (Bus)) << 20) | (((UINT32)(Dev)) << 15) | (((UINT32)(Fun)) << 12) | ((UINT32)(Off)))
#define MAKE_SCRIPT_UINT8(X) ((X) & 0xff)
#define MAKE_SCRIPT_UINT16(X) ((X) & 0xff), ((((UINT16)(X)) & 0xff00) >> 8)
#define MAKE_SCRIPT_UINT32(X) ((X) & 0xff), ((((UINT32)(X)) & 0xff00) >> 8), ((((UINT32)(X)) & 0x00ff0000) >> 16), ((((UINT32)(X)) & 0xff000000) >> 24)

#define MAKE_SCRIPT_IO_WRITE_8(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_IO_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_IO_WRITE) + 1)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth8), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT8  (D)
#define MAKE_SCRIPT_IO_WRITE_16(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_IO_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_IO_WRITE) + 2)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth16), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT16 (D)
#define MAKE_SCRIPT_IO_WRITE_32(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_IO_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_IO_WRITE) + 4)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth32), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT32 (D)
#define MAKE_SCRIPT_ACPI_MMIO_WRITE_8(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_MMIO_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_MMIO_WRITE) + 1)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth8), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT8  (D)
#define MAKE_SCRIPT_ACPI_MMIO_WRITE_16(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_MMIO_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_MMIO_WRITE) + 2)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth16), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT16 (D)
#define MAKE_SCRIPT_ACPI_MMIO_WRITE_32(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_MMIO_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_MMIO_WRITE) + 4)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth32), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT32 (D)
#define MAKE_SCRIPT_PCICFG_WRITE_8(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_PCICFG_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_PCICFG_WRITE) + 1)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth8), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT8  (D)
#define MAKE_SCRIPT_PCICFG_WRITE_16(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_PCICFG_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_PCICFG_WRITE) + 2)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth16), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT16 (D)
#define MAKE_SCRIPT_PCICFG_WRITE_32(A, D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_PCICFG_WRITE_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_PCICFG_WRITE) + 4)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth32), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT32 (D)
#define MAKE_SCRIPT_IO_POLL_8(A, D, V, M) \
  MAKE_SCRIPT_UINT16 (SCRIPT_IO_POLL_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_IO_POLL) + 2)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth8), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT32 (D), \
  MAKE_SCRIPT_UINT8  (V), \
  MAKE_SCRIPT_UINT8  (M)
#define MAKE_SCRIPT_IO_POLL_16(A, D, V, M)) \
  MAKE_SCRIPT_UINT16 (SCRIPT_IO_POLL_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_IO_POLL) + 4)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth16), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT32 (D), \
  MAKE_SCRIPT_UINT16 (V), \
  MAKE_SCRIPT_UINT16 (M)
#define MAKE_SCRIPT_IO_POLL_32(A, D, V, M)) \
  MAKE_SCRIPT_UINT16 (SCRIPT_IO_POLL_OPCODE), \
  MAKE_SCRIPT_UINT16 ((sizeof (SCRIPT_IO_POLL) + 8)), \
  MAKE_SCRIPT_UINT32 (ScriptAccessWidth32), \
  MAKE_SCRIPT_UINT32 (A), \
  MAKE_SCRIPT_UINT32 (D), \
  MAKE_SCRIPT_UINT32 (V), \
  MAKE_SCRIPT_UINT32 (M)
#define MAKE_SCRIPT_STALL(D) \
  MAKE_SCRIPT_UINT16 (SCRIPT_STALL_OPCODE), \
  MAKE_SCRIPT_UINT16 (sizeof (SCRIPT_STALL)), \
  MAKE_SCRIPT_UINT32 (D), \
  MAKE_SCRIPT_UINT32 (0x00)

#define MAKE_SCRIPT_HEADER_ONE_TIME_EXECUTE(NAME) \
  MAKE_SCRIPT_UINT16 (SCRIPT_TABLE_ONE_TIME_EXECUTE_OPCODE), \
  MAKE_SCRIPT_UINT16 (sizeof (SCRIPT_TABLE_HEADER)), \
  MAKE_SCRIPT_UINT32 (0x00), \
  MAKE_SCRIPT_UINT32 (sizeof (NAME)), \
  MAKE_SCRIPT_UINT16 (0x504F), \
  MAKE_SCRIPT_UINT16 (0x4254)

#define MAKE_SCRIPT_HEADER_REPEATED_EXECUTE(NAME, REPT) \
  MAKE_SCRIPT_UINT16 (SCRIPT_TABLE_REPEATED_EXECUTE_OPCODE), \
  MAKE_SCRIPT_UINT16 (sizeof (SCRIPT_TABLE_HEADER)), \
  MAKE_SCRIPT_UINT32 (0x00), \
  MAKE_SCRIPT_UINT32 (sizeof (NAME)), \
  MAKE_SCRIPT_UINT16 (REPT), \
  MAKE_SCRIPT_UINT16 (0x4254)

#define MAKE_SCRIPT_TERMINATE() \
  MAKE_SCRIPT_UINT16 (SCRIPT_TERMINATE_OPCODE), \
  MAKE_SCRIPT_UINT16 (sizeof (SCRIPT_TERMINATE))


APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_ERROR_OUT_INDICATOR_OPCODE,   // TypeId
  sizeof(ApcbTypeHeader),                     // SizeOfType, will be fixed up by tool
  0x404E,                                     // InstanceId ::= Error code
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,          // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK               // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

UINT8 MemErrorOutIndicatorOpcode [0x40] = {
  // Table Header
  MAKE_SCRIPT_HEADER_ONE_TIME_EXECUTE (MemErrorOutIndicatorOpcode),
  // Entry Begin
  MAKE_SCRIPT_IO_WRITE_32 (0x80, 0x12344321),
  MAKE_SCRIPT_STALL (3000000),
  // Entry Terminate
  MAKE_SCRIPT_TERMINATE ()
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

