/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Uefi.h"
#include <Pi/PiMultiPhase.h>
#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspApobLib.h>
#include <Library/AgesaConfigLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdHeapLib.h>
#include <Filecode.h>
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AGESACONFIGLIB_AGESACONFIGLIB_FILECODE


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief Initial AGESA configuration Database
 *
 * @details Allocate space from heap, merge the ABL data from APOB, initial default value
 *
 * @return AGESA_STATUS
 */
AGESA_STATUS
AgesaInitCfgDb (
  VOID
  )
{
  ALLOCATE_HEAP_PARAMS            AllocHeapParams;
  AGESA_CONFIG_DB_HEADER_V1       *AblActDbHdr;
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  APOB_TYPE_HEADER                *ApobEntry;
  EFI_STATUS                      EfiStatus;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *AblActByteTypeDesc;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *AblActWordTypeDesc;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *AblActDwordTypeDesc;

  AblActDbHdr = NULL;
  AblActByteTypeDesc  = NULL;
  AblActWordTypeDesc  = NULL;
  AblActDwordTypeDesc = NULL;

  IDS_HDT_CONSOLE (MAIN_FLOW, "\nAgesaInitCfgDb\n");
  EfiStatus = AmdPspGetApobEntryInstance (APOB_GEN, APOB_GEN_CONFIG_DATA_TYPE, 0, FALSE, &ApobEntry);
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Get APOB_GEN_CONFIG_DATA_TYPE from APOB fail\n");
    return AGESA_ERROR;
  } else {
    AblActDbHdr = (AGESA_CONFIG_DB_HEADER_V1 *) (ApobEntry + 1);
    //Get the information of ABL ACT data
    //ABL Byte type
    AblActByteTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) AblActDbHdr + AblActDbHdr->ByteTypeDataOffset);
    ASSERT (AblActByteTypeDesc->Type == AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_BYTE);

    //ABL Word type
    AblActWordTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) AblActDbHdr + AblActDbHdr->WordTypeDataOffset);
    ASSERT (AblActWordTypeDesc->Type == AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_WORD);

    //ABL DWord type
    AblActDwordTypeDesc =
      (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) AblActDbHdr + AblActDbHdr->DwordTypeDataOffset);
    ASSERT (AblActDwordTypeDesc->Type == AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_DWORD);
    ASSERT (AblActByteTypeDesc->Count <= AGESA_CONFIG_BYTE_TYPE_MAX_COUNT);
    ASSERT (AblActWordTypeDesc->Count <= AGESA_CONFIG_WORD_TYPE_MAX_COUNT);
    ASSERT (AblActDwordTypeDesc->Count <= AGESA_CONFIG_DWORD_TYPE_MAX_COUNT);
  }

  //Allocate space from heap
  AllocHeapParams.RequestedBufferSize = sizeof (AGESA_CONFIG_DB_HEADER_V1) + \
                                        sizeof (AGESA_CONFIG_TYPE_DESCRIPTOR_V1) + AGESA_CONFIG_BYTE_TYPE_MAX_SIZE + \
                                        sizeof (AGESA_CONFIG_TYPE_DESCRIPTOR_V1) + AGESA_CONFIG_WORD_TYPE_MAX_SIZE + \
                                        sizeof (AGESA_CONFIG_TYPE_DESCRIPTOR_V1) + AGESA_CONFIG_DWORD_TYPE_MAX_SIZE;

  IDS_HDT_CONSOLE (MAIN_FLOW, "Allocate Heap: U8Count:%d, U16Count:%d, U32Count:%d, Size:0x%x \n", \
                   AGESA_CONFIG_BYTE_TYPE_MAX_COUNT,
                   AGESA_CONFIG_WORD_TYPE_MAX_COUNT,
                   AGESA_CONFIG_DWORD_TYPE_MAX_COUNT,
                   AllocHeapParams.RequestedBufferSize);
  AllocHeapParams.BufferHandle = AGESA_CONFIG_DATABASE_HANDLE;
  AllocHeapParams.Persist = HEAP_RUNTIME_SYSTEM_MEM;
  if (HeapAllocateBuffer (&AllocHeapParams, NULL) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Heap allocate fail\n");
    return AGESA_ERROR;
  }
  ZeroMem (AllocHeapParams.BufferPtr,  AllocHeapParams.RequestedBufferSize);

  //Initial CFG header
  ActDbHdr = (AGESA_CONFIG_DB_HEADER_V1 *) (AllocHeapParams.BufferPtr);
  ActDbHdr->Version = AGESA_CONFIG_DB_HEADER_VERSION_1;
  ActDbHdr->ByteTypeDataOffset = sizeof (AGESA_CONFIG_DB_HEADER_V1);
  ActDbHdr->WordTypeDataOffset = ActDbHdr->ByteTypeDataOffset + sizeof (AGESA_CONFIG_TYPE_DESCRIPTOR_V1) +
                                 AGESA_CONFIG_BYTE_TYPE_MAX_SIZE;
  ActDbHdr->DwordTypeDataOffset = ActDbHdr->WordTypeDataOffset + sizeof (AGESA_CONFIG_TYPE_DESCRIPTOR_V1) +
                                  AGESA_CONFIG_WORD_TYPE_MAX_SIZE;

  //Init AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE BYTE
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->ByteTypeDataOffset);
  ActTypeDesc->Version = AGESA_CONFIG_TYPE_DESCRIPTOR_VERSION_1;
  ActTypeDesc->Type = AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_BYTE;
  ActTypeDesc->Size = AGESA_CONFIG_BYTE_TYPE_MAX_SIZE;
  ActTypeDesc->Count = 0;
  // If APOB config data has been found, create a new copy with old data merged
  if (AblActByteTypeDesc->Count != 0) {
    ActTypeDesc->Count = AblActByteTypeDesc->Count;
    CopyMem ((VOID *) (ActTypeDesc + 1), (VOID *) (AblActByteTypeDesc + 1),
             AblActByteTypeDesc->Count * sizeof (AGESA_CONFIG_RECORD_BYTE));
  }

  //Init AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE WORD
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->WordTypeDataOffset);
  ActTypeDesc->Version = AGESA_CONFIG_TYPE_DESCRIPTOR_VERSION_1;
  ActTypeDesc->Type = AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_WORD;
  ActTypeDesc->Size = AGESA_CONFIG_WORD_TYPE_MAX_SIZE;
  ActTypeDesc->Count = 0;
  // If APOB config data has been found, create a new copy with old data merged
  if (AblActWordTypeDesc->Count != 0) {
    ActTypeDesc->Count = AblActWordTypeDesc->Count;
    CopyMem ((VOID *) (ActTypeDesc + 1), (VOID *) (AblActWordTypeDesc + 1),
             AblActWordTypeDesc->Count * sizeof (AGESA_CONFIG_RECORD_WORD));
  }

  //Init AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE DWORD
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->DwordTypeDataOffset);
  ActTypeDesc->Version = AGESA_CONFIG_TYPE_DESCRIPTOR_VERSION_1;
  ActTypeDesc->Type = AGESA_CONFIG_TYPE_DESCRIPTOR_TYPE_DWORD;
  ActTypeDesc->Size = AGESA_CONFIG_DWORD_TYPE_MAX_SIZE;
  ActTypeDesc->Count = 0;
  // If APOB config data has been found, create a new copy with old data merged
  if (AblActDwordTypeDesc->Count != 0) {
    ActTypeDesc->Count = AblActDwordTypeDesc->Count;
    CopyMem ((VOID *) (ActTypeDesc + 1), (VOID *) (AblActDwordTypeDesc + 1),
             AblActDwordTypeDesc->Count * sizeof (AGESA_CONFIG_RECORD_DWORD));
  }
  // IDS_HDT_CONSOLE (MAIN_FLOW, "AgesaInitCfgDb [Done]\n");
  return AGESA_SUCCESS;
}

/**
 * @brief Dump the Act Database buffer
 *
 * @return VOID
 */
VOID
DumpActDb (
  AGESA_CONFIG_DB_HEADER_V1 *ActDbHdr
  )
{
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  UINT32                          i;
  AGESA_CONFIG_RECORD_BYTE        *ByteRecord;
  AGESA_CONFIG_RECORD_WORD        *WordRecord;
  AGESA_CONFIG_RECORD_DWORD       *DwordRecord;


  if (ActDbHdr == NULL) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDbHdr NULL Exit\n");
    return;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "ActDbHdr 0x%x\n", ActDbHdr);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Version:0x%x\n", ActDbHdr->Version);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t ByteTypeDataOffset:0x%x\n", ActDbHdr->ByteTypeDataOffset);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t WordTypeDataOffset:0x%x\n", ActDbHdr->WordTypeDataOffset);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t DwordTypeDataOffset:0x%x\n", ActDbHdr->DwordTypeDataOffset);

  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->ByteTypeDataOffset);
  IDS_HDT_CONSOLE (MAIN_FLOW, "=================== Byte ActTypeDesc:0x%x  ==================\n", ActTypeDesc);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Version:0x%x\n", ActTypeDesc->Version);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Type:0x%x\n", ActTypeDesc->Type);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Size:0x%x\n", ActTypeDesc->Size);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Count:0x%x\n", ActTypeDesc->Count);
  for (i = 0; i < 16; i++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "0x%08x            ", i);
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "\n");
  ByteRecord = (AGESA_CONFIG_RECORD_BYTE *) (ActTypeDesc + 1);
  for (i = 0; i < ActTypeDesc->Count; i++, ByteRecord++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "0x%08x=0x%08x ", ByteRecord->Uid, ByteRecord->Value);
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "\n");
    }
  }

  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->WordTypeDataOffset);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\n =================== Word ActTypeDesc:0x%x  ==================\n", ActTypeDesc);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Version:0x%x\n", ActTypeDesc->Version);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Type:0x%x\n", ActTypeDesc->Type);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Size:0x%x\n", ActTypeDesc->Size);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Count:0x%x\n", ActTypeDesc->Count);
  for (i = 0; i < 16; i++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "0x%08x            ", i);
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "\n");
  WordRecord = (AGESA_CONFIG_RECORD_WORD *) (ActTypeDesc + 1);
  for (i = 0; i < ActTypeDesc->Count; i++, WordRecord++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "0x%08x=0x%08x ", WordRecord->Uid, WordRecord->Value);
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "\n");
    }
  }

  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->DwordTypeDataOffset);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\n =================== Dword ActTypeDesc:0x%x  ==================\n", ActTypeDesc);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Version:0x%x\n", ActTypeDesc->Version);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Type:0x%x\n", ActTypeDesc->Type);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Size:0x%x\n", ActTypeDesc->Size);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t Count:0x%x\n", ActTypeDesc->Count);
  for (i = 0; i < 16; i++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "0x%08x            ", i);
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "\n");
  DwordRecord = (AGESA_CONFIG_RECORD_DWORD *) (ActTypeDesc + 1);
  for (i = 0; i < ActTypeDesc->Count; i++, DwordRecord++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "0x%08x=0x%08x ", DwordRecord->Uid, DwordRecord->Value);
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "\n");
    }
  }
}
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
  )
{
  LOCATE_HEAP_PTR LocateHeap;
  AGESA_STATUS    AgesaStatus;

  LocateHeap.BufferHandle = AGESA_CONFIG_DATABASE_HANDLE;
  AgesaStatus = HeapLocateBuffer (&LocateHeap, NULL);

  if (AgesaStatus == AGESA_SUCCESS) {
    *ActDbHdr = (AGESA_CONFIG_DB_HEADER_V1 *) LocateHeap.BufferPtr;
  }
  return AgesaStatus;
}
/**
 * @brief Dump the Act Database buffer in HEAP
 *
 * @return VOID
 */
VOID
DumpActDbInHeap (
  )
{
  AGESA_CONFIG_DB_HEADER_V1 *ActHdr;
  if (LocateActDb (&ActHdr) == AGESA_SUCCESS) {
    DumpActDb (ActHdr);
  }
}
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
  )
{
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  AGESA_CONFIG_RECORD_BYTE        *ActRecord;
  UINT32                          Index;

  IDS_HDT_CONSOLE (MAIN_FLOW, "SetAgesaCfg8 UID:%x Value:%x >>\n", Uid, Value);

  if (LocateActDb (&ActDbHdr) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
    return AGESA_ERROR;
  }
  if (ActDbHdr->ByteTypeDataOffset == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb not initialized <<\n");
    return AGESA_UNSUPPORTED;
  }
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->ByteTypeDataOffset);
  //Check if any existing record
  Index = 0;
  ActRecord = (AGESA_CONFIG_RECORD_BYTE *) (ActTypeDesc + 1);
  if (GetAgesaCfg8Worker (Uid, NULL, &Index) == AGESA_SUCCESS) {
    //if yes, update the existing one
    IDS_HDT_CONSOLE (MAIN_FLOW, "ACT8[%d] 0x%x=>0x%x <<\n", Index, ActRecord[Index].Value, Value);
    ActRecord[Index].Value = Value;
  } else {
    // If not found, insert new one at the end
    if ((ActTypeDesc->Count + 1) * sizeof (AGESA_CONFIG_RECORD_BYTE) > ActTypeDesc->Size) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "Not enough space <<\n");
      return AGESA_BOUNDS_CHK;
    }
    ActRecord[ActTypeDesc->Count].Uid = Uid;
    ActRecord[ActTypeDesc->Count].Value = Value;
    IDS_HDT_CONSOLE (MAIN_FLOW, "New ACT8[%d]=0x%x, Count: %d => %d <<\n", \
                     ActTypeDesc->Count, ActRecord[ActTypeDesc->Count].Value, ActTypeDesc->Count,
                     ActTypeDesc->Count + 1);
    ActTypeDesc->Count++;
  }
  return AGESA_SUCCESS;
}

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
  )
{
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  AGESA_CONFIG_RECORD_WORD        *ActRecord;
  UINT32                          Index;

  IDS_HDT_CONSOLE (MAIN_FLOW, "SetAgesaCfg16 UID:%x Value:%x >>\n", Uid, Value);
  if (LocateActDb (&ActDbHdr) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
    return AGESA_ERROR;
  }
  if (ActDbHdr->WordTypeDataOffset == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb not initialized <<\n");
    return AGESA_UNSUPPORTED;
  }
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->WordTypeDataOffset);
  //Check if any existing record
  Index = 0;
  ActRecord = (AGESA_CONFIG_RECORD_WORD *) (ActTypeDesc + 1);
  if (GetAgesaCfg16Worker (Uid, NULL, &Index) == AGESA_SUCCESS) {
    //if yes, update the existing one
    IDS_HDT_CONSOLE (MAIN_FLOW, "ACT16[%d] 0x%x=>0x%x <<\n", Index, ActRecord[Index].Value, Value);
    ActRecord[Index].Value = Value;
  } else {
    // If not found, insert new one at the end
    if ((ActTypeDesc->Count + 1) * sizeof (AGESA_CONFIG_RECORD_WORD) > ActTypeDesc->Size) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "Not enough space <<\n");
      return AGESA_BOUNDS_CHK;
    }
    ActRecord[ActTypeDesc->Count].Uid = Uid;
    ActRecord[ActTypeDesc->Count].Value = Value;
    IDS_HDT_CONSOLE (MAIN_FLOW, "New ACT16[%d]=0x%x, Count: %d => %d <<\n", \
                     ActTypeDesc->Count, ActRecord[ActTypeDesc->Count].Value, ActTypeDesc->Count,
                     ActTypeDesc->Count + 1);
    ActTypeDesc->Count++;
  }
  return AGESA_SUCCESS;
}

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
  )
{
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  AGESA_CONFIG_RECORD_DWORD       *ActRecord;
  UINT32                          Index;

  IDS_HDT_CONSOLE (MAIN_FLOW, "SetAgesaCfg32 UID:%x Value:%x >>\n", Uid, Value);
  if (LocateActDb (&ActDbHdr) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
    return AGESA_ERROR;
  }
  if (ActDbHdr->DwordTypeDataOffset == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb not initialized <<\n");
    return AGESA_UNSUPPORTED;
  }
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->DwordTypeDataOffset);
  //Check if any existing record
  Index = 0;
  ActRecord = (AGESA_CONFIG_RECORD_DWORD *) (ActTypeDesc + 1);
  if (GetAgesaCfg32Worker (Uid, NULL, &Index) == AGESA_SUCCESS) {
    //if yes, update the existing one
    IDS_HDT_CONSOLE (MAIN_FLOW, "ACT32[%d] 0x%x=>%x <<\n", Index, ActRecord[Index].Value, Value);
    ActRecord[Index].Value = Value;
  } else {
    // If not found, insert new one at the end
    if ((ActTypeDesc->Count + 1) * sizeof (AGESA_CONFIG_RECORD_DWORD) > ActTypeDesc->Size) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "Not enough space <<\n");
      return AGESA_BOUNDS_CHK;
    }
    ActRecord[ActTypeDesc->Count].Uid = Uid;
    ActRecord[ActTypeDesc->Count].Value = Value;
    IDS_HDT_CONSOLE (MAIN_FLOW, "New ACT32[%d]=%x, Count: %d => %d <<\n", \
                     ActTypeDesc->Count, ActRecord[ActTypeDesc->Count].Value, ActTypeDesc->Count,
                     ActTypeDesc->Count + 1);
    ActTypeDesc->Count++;
  }
  return AGESA_SUCCESS;
}

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
  UINT8  *Value,
  UINT32 *Index
  )
{
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  AGESA_CONFIG_RECORD_BYTE        *ActRecord;
  UINT32                          i;

  IDS_HDT_CONSOLE (MAIN_FLOW, "GetAgesaCfg8 UID:%x ", Uid);
  if (LocateActDb (&ActDbHdr) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
    return AGESA_ERROR;
  }
  if (ActDbHdr->ByteTypeDataOffset == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb not initialized <<\n");
    return AGESA_UNSUPPORTED;
  }
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->ByteTypeDataOffset);

  ActRecord = (AGESA_CONFIG_RECORD_BYTE *) (ActTypeDesc + 1);
  for (i = 0; i < ActTypeDesc->Count; i++) {
    if (ActRecord[i].Uid == Uid) {
      if (Value != NULL) {
        *Value = ActRecord[i].Value;
      }
      if (Index != NULL) {
        *Index = i;
      }
      IDS_HDT_CONSOLE (MAIN_FLOW, "Index %d Value:%x\n", i, ActRecord[i].Value);
      return AGESA_SUCCESS;
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Not found\n");
  return AGESA_UNSUPPORTED;
}


/**
  Gets the 8-bits value for the AGESA configuration specified by Uid to the value specified by Value.

  @param[in] Uid            The unique ID for AGESA configuration to Get a current value for.
  @param[in] Value          The 8 bits value to Get.

  @return The status of the Get operation. Non-Zero, fail
**/
AGESA_STATUS
GetAgesaCfg8 (
  UINT32 Uid,
  UINT8  *Value
  )
{
  return GetAgesaCfg8Worker (Uid, Value, NULL);
}

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
  )
{
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  AGESA_CONFIG_RECORD_WORD        *ActRecord;
  UINT32                          i;

  IDS_HDT_CONSOLE (MAIN_FLOW, "GetAgesaCfg16 UID:%x ", Uid);
  if (LocateActDb (&ActDbHdr) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
    return AGESA_ERROR;
  }
  if (ActDbHdr->WordTypeDataOffset == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb not initialized <<\n");
    return AGESA_UNSUPPORTED;
  }
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->WordTypeDataOffset);

  ActRecord = (AGESA_CONFIG_RECORD_WORD *) (ActTypeDesc + 1);
  for (i = 0; i < ActTypeDesc->Count; i++) {
    if (ActRecord[i].Uid == Uid) {
      if (Value != NULL) {
        *Value = ActRecord[i].Value;
      }
      if (Index != NULL) {
        *Index = i;
      }
      IDS_HDT_CONSOLE (MAIN_FLOW, "Index %d Value:%x\n", i, ActRecord[i].Value);
      return AGESA_SUCCESS;
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Not found\n");
  return AGESA_UNSUPPORTED;
}

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
  )
{
  return GetAgesaCfg16Worker (Uid, Value, NULL);
}

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
  )
{
  AGESA_CONFIG_DB_HEADER_V1       *ActDbHdr;
  AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *ActTypeDesc;
  AGESA_CONFIG_RECORD_DWORD       *ActRecord;
  UINT32                          i;

  IDS_HDT_CONSOLE (MAIN_FLOW, "GetAgesaCfg32 UID:%x ", Uid);
  if (LocateActDb (&ActDbHdr) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
    return AGESA_ERROR;
  }
  if (ActDbHdr->DwordTypeDataOffset == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb not initialized <<\n");
    return AGESA_UNSUPPORTED;
  }
  ActTypeDesc = (AGESA_CONFIG_TYPE_DESCRIPTOR_V1 *) ((UINT8 *) ActDbHdr + ActDbHdr->DwordTypeDataOffset);

  ActRecord = (AGESA_CONFIG_RECORD_DWORD *) (ActTypeDesc + 1);
  for (i = 0; i < ActTypeDesc->Count; i++) {
    if (ActRecord[i].Uid == Uid) {
      if (Value != NULL) {
        *Value = ActRecord[i].Value;
      }
      if (Index != NULL) {
        *Index = i;
      }
      IDS_HDT_CONSOLE (MAIN_FLOW, "Index %d Value:%x\n", i, ActRecord[i].Value);
      return AGESA_SUCCESS;
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Not found\n");
  return AGESA_UNSUPPORTED;
}

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
  )
{
  return GetAgesaCfg32Worker (Uid, Value, NULL);
}


