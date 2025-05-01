/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#ifndef _AGESA_CONFIG_LIB_H_
#define _AGESA_CONFIG_LIB_H_
#include "AMD.h"

#pragma pack (push, 1)
#define AGESA_CONFIG_DB_HEADER_VERSION_1   0x00000001
#define AGESA_CONFIG_DB_POINTER_VARIABLE_NAME               L"ActDbPointer"

typedef struct _AGESA_CONFIG_DB_HEADER_V1 {
  UINT32  Version;                    //Version, current version AGESA_CONFIG_DB_HEADER_VERSION_1
  UINT32  ByteTypeDataOffset;         //Offset in bytes from the beginning of this table,point to Bool type AGESA_CONFIG_TYPE_DESCRIPTOR_V1
  UINT32  WordTypeDataOffset;         //Offset in bytes from the beginning of this table,point to Word type AGESA_CONFIG_TYPE_DESCRIPTOR_V1
  UINT32  DwordTypeDataOffset;        //Offset in bytes from the beginning of this table,point to Dword type AGESA_CONFIG_TYPE_DESCRIPTOR_V1
  //Note: For backwards compatibility concern, one should not change the above fields even for future versions.
  UINT32  Reserved [3];
} AGESA_CONFIG_DB_HEADER_V1;

#define AGESA_CONFIG_TYPE_DESCRIPTOR_VERSION_1 0x00000001

#define AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_BYTE  1
#define AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_WORD  2
#define AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_DWORD 3

#define AGESA_CONFIG_BYTE_TYPE_MAX_COUNT   0x1000
#define AGESA_CONFIG_BYTE_TYPE_MAX_SIZE   (AGESA_CONFIG_BYTE_TYPE_MAX_COUNT * sizeof (AGESA_CONFIG_RECORD_BYTE))

#define AGESA_CONFIG_WORD_TYPE_MAX_COUNT   0x400
#define AGESA_CONFIG_WORD_TYPE_MAX_SIZE   (AGESA_CONFIG_WORD_TYPE_MAX_COUNT * sizeof (AGESA_CONFIG_RECORD_WORD))

#define AGESA_CONFIG_DWORD_TYPE_MAX_COUNT   0x400
#define AGESA_CONFIG_DWORD_TYPE_MAX_SIZE   (AGESA_CONFIG_DWORD_TYPE_MAX_COUNT * sizeof (AGESA_CONFIG_RECORD_DWORD))
typedef struct _AGESA_CONFIG_TYPE_DESCRIPTOR_V1 {
  UINT16 Version;               // The version of this descriptor. Current Version is AGESA_CONFIG_TYPE_DESCRIPTOR_VERSION_1.
  UINT16 Type;                  // Type of current blob, defined with AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_*
  UINT32 Size;                  // Size allocated for current type exclude AGESA_CONFIG_TYPE_DESCRIPTOR_V1
  UINT32  Count;                // Count of records follow by AGESA_CONFIG_TYPE_DESCRIPTOR_V1
  //Note: For backwards compatibility concern, one should not change the above fields even for future versions.
  UINT32  Reserved;
  //AGESA_CONFIG_RECORD Record [];
} AGESA_CONFIG_TYPE_DESCRIPTOR_V1;

typedef struct _AGESA_CONFIG_RECORD_BYTE {
  UINT32 Uid;                   //Unique ID of given config
  UINT8 Value;                 //Value, use UINT32 to satify ARM alignment
  UINT8 Rsv[3];
} AGESA_CONFIG_RECORD_BYTE;

typedef struct _AGESA_CONFIG_RECORD_WORD {
  UINT32 Uid;                   //Unique ID of given config
  UINT16 Value;                 //Value, use UINT32 to satify ARM alignment
  UINT8 Rsv[2];
} AGESA_CONFIG_RECORD_WORD;

typedef struct _AGESA_CONFIG_RECORD_DWORD {
  UINT32 Uid;                   //Unique ID of given config
  UINT32 Value;                 //Value, use UINT32 to satify ARM alignment
} AGESA_CONFIG_RECORD_DWORD;
#pragma pack (pop)
/**
 * @brief Initial AGESA configuration Database
 *
 * @details Allocate space from heap, initial the database with default value, record database pointer to gContextPtr
 *
 * @return AGESA_STATUS
 */
AGESA_STATUS
AgesaInitCfgDb (
  VOID
  );

/**
  Sets the 8-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to set a current value for.
  @param[in] Value          The 8 bits value to set.

  @return The status of the set operation. Non-Zero, fail
**/
AGESA_STATUS
SetAgesaCfg8 (
  UINT32 Uid,
  UINT8  Value
);

/**
  Sets the 16-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to set a current value for.
  @param[in] Value          The 8 bits value to set.

  @return The status of the set operation. Non-Zero, fail
**/
AGESA_STATUS
SetAgesaCfg16 (
  UINT32 Uid,
  UINT16 Value
);

/**
  Sets the 32-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to set a current value for.
  @param[in] Value          The 8 bits value to set.

  @return The status of the set operation. Non-Zero, fail
**/
AGESA_STATUS
SetAgesaCfg32 (
  UINT32 Uid,
  UINT32 Value
);

/**
  Gets the 8-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.
  @param[in] Index          Index of record, if found


  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg8Worker (
  UINT32 Uid,
  UINT8 *Value,
  UINT32 *Index
  );

/**
  Gets the 8-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.

  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg8 (
  UINT32 Uid,
  UINT8 *Value
);

/**
  Gets the 16-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.
  @param[in] Index          Index of record, if found


  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg16Worker (
  UINT32 Uid,
  UINT16 *Value,
  UINT32 *Index
  );

/**
  Gets the 16-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.

  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg16 (
  UINT32 Uid,
  UINT16 *Value
);

/**
  Gets the 32-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.
  @param[in] Index          Index of record, if found

  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg32Worker (
  UINT32 Uid,
  UINT32 *Value,
  UINT32 *Index
  );

/**
  Gets the 32-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.

  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg32 (
  UINT32 Uid,
  UINT32 *Value
);

/**
 * @brief Dump the Act Database buffer
 *
 * @return VOID
 */
VOID
DumpActDb (
  AGESA_CONFIG_DB_HEADER_V1 *ActDbHdr
  );

/**
 * @brief Locate Act DB from HEAP
 *
 * @param ActDbHdr Address of ActDb in the heap
 * @return AGESA_STATUS
           AGESA_SUCCESS: ActDb locate successfully in the HEAP
           Other value: ActDb not found in the HEAP
 */
AGESA_STATUS
LocateActDb (
  AGESA_CONFIG_DB_HEADER_V1 **ActDbHdr
  );
/**
 * @brief Dump the Act Database buffer in HEAP
 *
 * @return VOID
 */
VOID
DumpActDbInHeap (
  );
#endif

