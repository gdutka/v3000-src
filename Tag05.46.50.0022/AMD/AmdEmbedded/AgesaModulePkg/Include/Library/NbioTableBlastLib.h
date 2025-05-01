/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Defines needed to define NBIO Tables
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: NBIO
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _NBIOTABLEBLAST_H_
#define _NBIOTABLEBLAST_H_

#include "AMD.h"
#include <GnbDxio.h>

#pragma pack (push, 1)

// These are based on SSP - please verify that any new MCM program uses similar SMN apertures before using this library

#ifndef NBIO_SPACE
  #define  NBIO_SPACE(HANDLE, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20))
#endif

#ifndef WRAP_SPACE
  #define  WRAP_SPACE(HANDLE, WRAPPER, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId << 22))
#endif

#ifndef PORT_SPACE
  #define  PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId << 22) + (PORTINDEX << 12))
#endif

#ifndef IOHC_PORT_SPACE
  #define  IOHC_PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId * 0x100) + (PORTINDEX * 0x20))
#endif

#ifndef IOHC_BRIDGE_SPACE
  #define  IOHC_BRIDGE_SPACE(HANDLE, ENGINE, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (ENGINE->Type.Port.LogicalBridgeId << 10))
#endif

/// CAUTION: This is very specific to SDPMUX registers that use a different address calculation
#ifndef SDPMUX_SPACE
  #define  SDPMUX_SPACE(HANDLE, ADDRESS)   ((HANDLE->RBIndex == 0) ? ADDRESS : (ADDRESS + (1 << 23) + ((HANDLE->RBIndex + 1) << 20)))
#endif


#define ONE_ADDR_TYPE   0  ///< Apply table entry just once to the supplied address
#define ALL_NBIO_TYPE   1  ///< Apply table entry once per NBIO. Address supplied should be NBIO0.
#define ALL_WRAP_TYPE   2  ///< Apply table entry to all wrappers
#define ALL_PORT_TYPE   3  ///< Apply table entry to all ports
#define ALL_SDPMUX_TYPE 4  ///< Used for SDPMUX (different address calculation versus ALL_NBIO_TYPE
#define OTHER_TYPE      5  ///< Used for polling and debugging purposes
#define SIMPLE_ADDR_TYPE 6  ///< Used to avoid macro application to address
#define MAX_ADDR_TYPE    7  ///< Maximum type supported

#define NBIO_TABLE_FLAGS_FORCE_S3_SAVE  0x00000001ul

#define __DATA(x) x

/// Break up UINT64 or UINT32 value into individual bytes so
/// they can be generically initialized into a byte array
#define _DATA32(Data) ((UINT32) (__DATA(Data))) & 0xFF, (((UINT32) (__DATA(Data))) >> 8) & 0xFF, (((UINT32) (__DATA(Data))) >> 16) & 0xFF, (((UINT32) (__DATA(Data))) >> 24) & 0xFF
#define _DATA64(Data) _DATA32(Data & 0xfffffffful) , _DATA32((UINT64) Data >> 32)

typedef UINT8 GNB_TABLE;

/// Entry type
typedef enum {
  GnbEntryWr,                     ///< Write register
  GnbEntryRmw,                    ///< Read Modify Write register
  GnbEntryPropertyRmw,            ///< Read Modify Write register check property
  GnbEntryStall,                  ///< Copy field from one register to another
  GnbEntryPcdWr,                  ///< Write register contingent on a PCD value
  GnbEntryPcdRmw,                 ///< Read Modify Write register contingent on a PCD value
  GnbEntryCpuDeadLoop,            ///< Suspend execution at this table instruction (debug use only)
  GnbEntryTerminate = 0xFF        ///< Terminate table
} GNB_TABLE_ENTRY_TYPE;

/// Pass these in to determine PCD Operation Type
typedef enum {
  TableBlastWriteIfValEqual,
  TableBlastWriteIfValNotEqual,
  TableBlastWriteValue,
} GNB_TABLE_PCD_OPERATION_TYPE;

///
/// Header stucture all table entries have in common
///
typedef struct {
  UINT8           EntryType;               ///< Entry type as listed in the above enum GNB_TABLE_ENTRY_TYPE
  UINT8           RegisterSpaceType;       ///< Register space type
} ENTRY_HEAD;

///
/// Info structure that all Read/Modify/Write entries have in
/// common
///
typedef struct {
  UINT32  Address;                ///< Register address
  UINT32  AndMask;                ///< And Mask
  UINT32  OrMask;                 ///< Or Mask
} GNB_RMW_BLOCK;

///
/// Write register entry : start
///
#define GNB_ENTRY_WR(RegisterSpaceType, Address, Value) \
  GnbEntryWr, RegisterSpaceType, _DATA32 (Address), _DATA32 (Value)

typedef struct {
  ENTRY_HEAD  Head;               ///< Structure descriptor
  UINT32  Address;                ///< Register address
  UINT32  Value;                  ///< Value
} GNB_TABLE_ENTRY_WR;
///
/// Write register entry : end
///

///
/// Read/Modify/Write register entry : start
///
#define GNB_ENTRY_RMW(RegisterSpaceType, Address, AndMask, OrMask) \
  GnbEntryRmw, RegisterSpaceType, _DATA32 (Address), _DATA32 (AndMask), _DATA32 (OrMask)

typedef struct {
  ENTRY_HEAD      Head;           ///< Structure descriptor
  GNB_RMW_BLOCK   Data;           ///< Data
} GNB_TABLE_ENTRY_RMW;
///
/// Read/Modify/Write register entry : end
///

///
/// Write register entry based off of PCD : start
///
#define GNB_ENTRY_PCD_WR(RegisterSpaceType, PcdToken, PcdOperationType, CompareDataValue, Address, Value) \
  GnbEntryPcdWr, RegisterSpaceType, _DATA32 (PcdToken), PcdOperationType, _DATA32(CompareDataValue), _DATA32 (Address), _DATA32 (Value)

#define GNB_ENTRY_PCD_VAL_WR(RegisterSpaceType, PcdToken, Address) \
  GNB_ENTRY_PCD_WR(RegisterSpaceType, PcdToken, TableBlastWriteValue, 0x00000000, Address, 0x00000000)

#define GNB_ENTRY_WR_IF_PCD_EQUAL(RegisterSpaceType, PcdToken, CompareDataValue, Address, Value) \
  GNB_ENTRY_PCD_WR(RegisterSpaceType, PcdToken, TableBlastWriteIfValEqual, CompareDataValue, Address, Value)

#define GNB_ENTRY_WR_IF_PCD_NOT_EQUAL(RegisterSpaceType, PcdToken, CompareDataValue, Address, Value) \
  GNB_ENTRY_PCD_WR(RegisterSpaceType, PcdToken, TableBlastWriteIfValNotEqual, CompareDataValue, Address, Value)

typedef struct {
  ENTRY_HEAD  Head;           ///< Structure descriptor
  UINT32      Token;          ///< Local PCD Token value
  UINT8       PcdType;        ///< Pcd Type as listed in above enum GNB_TABLE_PCD_OPERATION_TYPE
  UINT32      CompareValue;   ///< Compare data associated with the operation, if applicable
  UINT32      Address;        ///< Register address
  UINT32      Value;          ///< Value
} GNB_TABLE_ENTRY_PCD_WR;
///
/// Write register entry based off of PCD : end
///

///
/// Read/Modify/Write register entry based off of PCD : start
///
#define GNB_ENTRY_PCD_RMW(RegisterSpaceType, PcdToken, PcdOperationType, CompareDataValue, Address, AndMask, OrMask ) \
  GnbEntryPcdRmw, RegisterSpaceType, _DATA32 (PcdToken), PcdOperationType, _DATA32(CompareDataValue), _DATA32 (Address), _DATA32 (AndMask), _DATA32 (OrMask)

#define GNB_ENTRY_RMW_IF_PCD_EQUAL(RegisterSpaceType, PcdToken, CompareDataValue, Address, AndMask, OrMask ) \
  GNB_ENTRY_PCD_RMW(RegisterSpaceType, PcdToken, TableBlastWriteIfValEqual, CompareDataValue, Address, AndMask, OrMask)

#define GNB_ENTRY_RMW_IF_PCD_NOT_EQUAL(RegisterSpaceType, PcdToken, CompareDataValue, Address, AndMask, OrMask ) \
  GNB_ENTRY_PCD_RMW(RegisterSpaceType, PcdToken, TableBlastWriteIfValNotEqual, CompareDataValue, Address, AndMask, OrMask)

typedef struct {
  ENTRY_HEAD      Head;           ///< Structure descriptor
  UINT32          Token;          ///< Local PCD Token value
  UINT8           PcdType;        ///< Pcd Type as listed in above enum GNB_TABLE_PCD_OPERATION_TYPE
  UINT32          CompareValue;   ///< Compare data associated with the operation, if applicable
  GNB_RMW_BLOCK   Data;           ///< Data for RMW operation
} GNB_TABLE_ENTRY_PCD_RMW;
///
/// Read/Modify/Write register entry based off of PCD : end
///


#define GNB_ENTRY_PROPERTY_RMW(Property,  RegisterSpaceType, Address, AndMask, OrMask) \
  GnbEntryPropertyRmw, RegisterSpaceType, _DATA32 (Property), _DATA32 (Address), _DATA32 (AndMask), _DATA32 (OrMask)

/// Read Modify Write register entry
typedef struct {
  ENTRY_HEAD      Head;           ///< Structure descriptor
  UINT32          Property;       ///< Property
  GNB_RMW_BLOCK   Data;           ///< Data
} GNB_TABLE_ENTRY_PROPERTY_RMW;


/// Used to stall the code execution during table blast. Use for debug purposes.
#define GNB_ENTRY_CPU_DEAD_LOOP(DebugId) \
  GnbEntryCpuDeadLoop, OTHER_TYPE, DebugId

/// CPU Dead Loop entry
typedef struct {
  ENTRY_HEAD      Head;           ///< Structure descriptor
  UINT8           DebugId;        ///< Number to use for debug purposes
} GNB_TABLE_ENTRY_CPU_DEAD_LOOP;

/// Stall amount entry
#define GNB_ENTRY_STALL(Microsecond) \
  GnbEntryStall, OTHER_TYPE, _DATA32 (Microsecond)

typedef struct {
  ENTRY_HEAD      Head;            ///< Structure descriptor
  UINT32          Microsecond;     ///< Value of delay
} GNB_TABLE_ENTRY_STALL;

#define GNB_ENTRY_TERMINATE GnbEntryTerminate

AGESA_STATUS
GnbBlastTable (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GNB_TABLE             *Table,
  IN      UINT32                Property,
  IN      UINT32                Flags,
  IN      AMD_CONFIG_PARAMS     *StdHeader
  );

#pragma pack (pop)

#endif /* _NBIOTABLEBLAST_H_ */


