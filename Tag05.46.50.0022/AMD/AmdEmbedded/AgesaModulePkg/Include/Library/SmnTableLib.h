/**
 *  @file SmnTableLib.h
 *  @brief Header file for simple table write functions
 */
/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
#ifndef _SMNTABLELIB_H_
#define _SMNTABLELIB_H_

#include "AMD.h"
#include <GnbDxio.h>

#pragma pack (push, 1)

typedef UINT32 SMN_TABLE;

/// Entry type
typedef enum {
  SmnEntryWr,                     ///< Write register
  SmnEntryRmw,                    ///< Read Modify Write register
  SmnEntryPropertyWr,             ///< Write register check property
  SmnEntryPropertyRmw,            ///< Read Modify Write register check property
  SmnTableEntry,                  ///< Table entry
  SmnTableEntryProperty,          ///< Table entry check property
  SmnEntryTerminate = 0xFF        ///< Terminate table
} SMN_TABLE_ENTRY_TYPE;


///
/// Table entry : start
///
#define SMN_ENTRY_TABLE(Table) \
  (UINT32) SmnTableEntry, (SMN_TABLE *) &Table

typedef struct {
  UINT32     EntryType;             ///< Structure descriptor
  SMN_TABLE  *Address;              ///< Table address
} SMN_TABLE_ENTRY;

///
/// Table entry if property entry : start
///
#define SMN_ENTRY_TABLE_PROPERTY (Property, Table) \
  (UINT32) SmnTableEntryProperty, (UINT32) Property, (SMN_TABLE *) &Table

typedef struct {
  UINT32           EntryType;      ///< Structure descriptor
  UINT32           Property;       ///< Property
  SMN_TABLE        *Address;       ///< Table Address
} SMN_TABLE_ENTRY_PROPERTY;

///
/// Write register entry : start
///
#define SMN_ENTRY_WR(Address, Value) \
  (UINT32) SmnEntryWr, (UINT32) Address, (UINT32) Value

typedef struct {
  UINT32  EntryType;              ///< Structure descriptor
  UINT32  Address;                ///< Register address
  UINT32  Value;                  ///< Value
} SMN_TABLE_ENTRY_WR;

///
/// Write register if property entry : start
///
#define SMN_ENTRY_PROPERTY_WR(Property, Address, Value) \
  (UINT32) SmnEntryPropertyWr, (UINT32) Property, (UINT32) Address, (UINT32) Value
typedef struct {
  UINT32      EntryType;          ///< Structure descriptor
  UINT32      Property;           ///< Property
  UINT32      Address;            ///< Register address
  UINT32      Value;              ///< Value
} SMN_TABLE_ENTRY_PROPERTY_WR;

///
/// Read/Modify/Write register entry : start
///
#define SMN_ENTRY_RMW(Address, AndMask, OrMask) \
  (UINT32) SmnEntryRmw, (UINT32) Address, (UINT32) AndMask, (UINT32) OrMask

typedef struct {
  UINT32          EntryType;      ///< Structure descriptor
  UINT32          Address;        ///< Register address
  UINT32          AndMask;        ///< And Mask
  UINT32          OrMask;         ///< Or Mask
} SMN_TABLE_ENTRY_RMW;

///
/// Read/Modify/Write register if property entry : start
///
#define SMN_ENTRY_PROPERTY_RMW(Property, Address, AndMask, OrMask) \
  (UINT32) SmnEntryPropertyRmw, (UINT32) Property, (UINT32) Address, (UINT32) AndMask, (UINT32) OrMask

typedef struct {
  UINT32          EntryType;      ///< Structure descriptor
  UINT32          Property;       ///< Property
  UINT32          Address;        ///< Register address
  UINT32          AndMask;        ///< And Mask
  UINT32          OrMask;         ///< Or Mask
} SMN_TABLE_ENTRY_PROPERTY_RMW;


#define SMN_ENTRY_TERMINATE (UINT32) SmnEntryTerminate

AGESA_STATUS
NbioSmnTable (
  IN      GNB_HANDLE            *GnbHandle,
  IN      SMN_TABLE             *Table,
  IN      UINT32                Modifier,
  IN      UINT32                Property, 
  IN      UINT32                Flags
  );

#pragma pack (pop)

#endif /* _SMNTABLELIB_H_ */


