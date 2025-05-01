/** @file
  Search and parse verbtable binary in FD.

;******************************************************************************
;* Copyright (c) 2016 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "VerbTableCfgGetVerbTable.h"


STATIC CHAR8 mFdmSignature[] = {'H', 'F', 'D', 'M'};

/**
  Check if this FDM entry supports the target board.
  This method works with board-id value over 0xFF.
  The board-id value less than 0xFF may compare with region id in each fdm entry.

  @param[in] BoardId         The qword size board id.
  @param[in] RegionIndex     The index of fdm entry.

  @retval TRUE               The entry supports this board.
  @retval FALSE              The entry does not support this board
                             or Fdm not found or Unsupported fdm version
                             or invalid fdm header discovered.
**/
BOOLEAN
DoesRegionSupportBoardId (
  IN CONST UINT64        BoardId,
  IN CONST UINT32        RegionIndex
  )
{
  H2O_FLASH_DEVICE_MAP_HEADER   *FdmHeader;
  H2O_FLASH_DEVICE_MAP_EXTS     *FdmExt;
  H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP *Map;
  UINT32                        Index;
  UINT32                        Index2;
  UINT64                        *BoardIds;
  UINT32                        BoardIdCount;

  FdmHeader = (H2O_FLASH_DEVICE_MAP_HEADER *)(UINTN) PcdGet64(PcdH2OFlashDeviceMapStart);
  if (FdmHeader == NULL) {
    return FALSE;
  }

  if (CompareMem(FdmHeader, mFdmSignature, sizeof(mFdmSignature))) {
    return FALSE;
  }

  //
  //  In Revision 3, reserved byte in Fdm header will be ExtensionCount.See BTR 3.17.1.
  //  The second ext block indicates the location of the first region-to-board-id-map.
  //
  if (FdmHeader->Revision != 3 || FdmHeader->ExtensionCount < 2) {
    return FALSE;
  }

  FdmExt = (H2O_FLASH_DEVICE_MAP_EXTS *)(FdmHeader + 1);
  //
  //  Jump to second ext block.
  //
  FdmExt = FdmExt + 1;

  Map = (H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP *)((UINT8 *)FdmHeader + FdmExt->ExtOffset);
  for (Index = 0; Index < FdmExt->ExtCount; Index++) {
    BoardIdCount = Map->BoardIdCount;
    if (Map->RegionIndex == RegionIndex) {
      BoardIds = (UINT64 *)((UINT8 *)Map + sizeof(H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP));
      for (Index2 = 0; Index2 < BoardIdCount; Index2++) {
        if (BoardIds[Index2] == BoardId) {
          return TRUE;
        }
      }
    }
    Map = Map + 1;
    Map = (H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP *)((UINT64 *)Map + BoardIdCount);
  }

  return FALSE;
}
/**
  Search and return the "entry" and the "index of the entry" in FDM.
  User may specify the instance number by 1 or greater if there are multiple instances available.

  @param[in] RegionType    A pointer to the guid of target region.
  @param[in] Instance      The Nth instace of specified region type.
  @param[out] RegionIndex  The index of target FDM entries.
  @param[out] Entry        A pointer to address of target Entry.

  @retval EFI_NOT_FOUND      Entry not found.
  @retval EFI_UNSUPPORTED    Unsupported fdm version or invalid fdm header discovered.
**/
EFI_STATUS
FdmGetNIndex (
  IN CONST EFI_GUID  *RegionType,
  IN CONST UINT32     Instance,
  OUT UINT32          *RegionIndex,
  OUT H2O_FLASH_DEVICE_MAP_ENTRY **Entry
  )
{
  H2O_FLASH_DEVICE_MAP_HEADER  *FdmHeader;
  H2O_FLASH_DEVICE_MAP_ENTRY   *FdmEntry;
  UINT32                       EntryArraySize;
  UINT32                       EntryCount;
  UINT32                       Index;
  UINT32                       MatchCount;

  FdmHeader = NULL;
  FdmHeader = (H2O_FLASH_DEVICE_MAP_HEADER *)(UINTN) PcdGet64(PcdH2OFlashDeviceMapStart);
  if (FdmHeader == NULL) {
    return EFI_NOT_FOUND;
  }
  FdmEntry = (H2O_FLASH_DEVICE_MAP_ENTRY *)((UINT8 *)FdmHeader + FdmHeader->Offset);
  EntryArraySize = FdmHeader->Size - FdmHeader->Offset; 
  EntryCount = EntryArraySize / FdmHeader->EntrySize;

  MatchCount = 0;

  //
  //  Search each fdm entry. If search hit, return entry address and its index.
  //
  for (Index = 0; Index < EntryCount; Index++) {
    if (CompareGuid(RegionType, &FdmEntry->RegionType)) {
      MatchCount++;
      if (MatchCount == Instance) {
        *RegionIndex = Index;
        *Entry = FdmEntry;
        return EFI_SUCCESS;
      }
    }
    FdmEntry = (H2O_FLASH_DEVICE_MAP_ENTRY *)((UINT8 *)FdmEntry + FdmHeader->EntrySize);
  }

  return EFI_NOT_FOUND;
}

/*
  Search fdm using dword board-id, if search hit,
  allocate space and fill it with varbtable data.

  @param[in]   BoardId    Qword board-id.
  @param[out]  VerbTable  A pointer to address of verbtable.

  @retval EFI_OUT_OF_RESOURCES Memory allocation failed.
  @retval EFI_SUCCESS          Get verbtable successfully.
  @retval EFI_NOT_FOUND        Target not found.
*/
EFI_STATUS
GetVerbTableQwordBoard (
  UINT64    BoardId,
  COMMON_CHIPSET_AZALIA_VERB_TABLE  **VerbTable
  )
{
  EFI_STATUS Status;
  UINT32     RegionIndex;
  UINT32     Instance;
  H2O_FLASH_DEVICE_MAP_ENTRY  *FdmEntry;
  UINT32     RegionOffset;
  UINT8      *Data;
  UINT8      CodecCount;
  UINT8      Index;
  UINT32     DwordDataCount;
  COMMON_CHIPSET_AZALIA_VERB_TABLE *AzaliaVerbTable;
  COMMON_VERB_TABLE_BIN_HEADER     *CommonVerbTableBinHeader;

  Status = EFI_SUCCESS;
  Instance = 1;

  while (TRUE) {
    Status = FdmGetNIndex(&gH2OFlashMapRegionVerbTableGuid, Instance, &RegionIndex, &FdmEntry);
    if (!EFI_ERROR(Status)) {
      if (DoesRegionSupportBoardId((UINT64)BoardId, RegionIndex)) {
        //
        //  Use this Fdm Entry.
        //
        RegionOffset = (UINT32)FdmEntry->RegionOffset;
        Data = (UINT8 *)(UINT32)(FdmGetBaseAddr() + RegionOffset);

        CodecCount = *Data;
        AzaliaVerbTable = AllocateZeroPool (sizeof (COMMON_CHIPSET_AZALIA_VERB_TABLE) * (CodecCount + 1));

        if (AzaliaVerbTable == NULL) {
          *VerbTable = NULL;
          return EFI_OUT_OF_RESOURCES;
        }
        //
        //  Binary format :
        //              ---------------------------------------------
        //       1Byte  |      Number of codecs                     | 
        //              ---------------------------------------------
        //       14Byte |   COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER |  --
        //              ---------------------------------------------   |
        //        2Byte |      Data size in Dword                   |   | codec1
        //              ---------------------------------------------   |
        //    non-fixed |      Verb table data                      |  --
        //              ---------------------------------------------
        //       14Byte |   COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER |  --
        //              ---------------------------------------------   |
        //        2Byte |      Data size in Dword                   |   | codec2
        //              ---------------------------------------------   |
        //    non-fixed |      Verb table data                      |  --
        //              ---------------------------------------------
        //              |                  ...                      |  -- codec n
        //
        CommonVerbTableBinHeader = (COMMON_VERB_TABLE_BIN_HEADER *)(Data + 1);

        for (Index = 0; Index < CodecCount; Index++) {
          AzaliaVerbTable[Index].VerbTableHeader = (COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER *)&CommonVerbTableBinHeader->Header;
          AzaliaVerbTable[Index].VerbTableData   = (UINT32 *)(CommonVerbTableBinHeader + 1);
          DwordDataCount = CommonVerbTableBinHeader->DataSizeDword;

          CommonVerbTableBinHeader = CommonVerbTableBinHeader + 1;
          CommonVerbTableBinHeader = (COMMON_VERB_TABLE_BIN_HEADER *)((UINT32 *)CommonVerbTableBinHeader + DwordDataCount);
        }
        
        AzaliaVerbTable[CodecCount].VerbTableHeader = NULL;
        AzaliaVerbTable[CodecCount].VerbTableData   = NULL;
        *VerbTable = AzaliaVerbTable;
        return EFI_SUCCESS;
      }
    } else {
      break;
    }
    Instance++;
  }
  return Status;
}

/**
  Find matched region by board id, and contruct a COMMON_CHIPSET_AZALIA_VERB_TABLE.
  If match not found, upate VerbTableHeaderDataAddress to NULL.

  @param[in]   BoardId                     8-bit length board-id.
  @param[out]  VerbTableHeaderDataAddress  A pointer to address of verbtable.

  @retval EFI_SUCCESS          Get verbtable entry successfully.
  @retval EFI_OUT_OF_RESOURCES Memory alloccation failed.
  @retval EFI_NOT_FOUND        Target not found.
**/

EFI_STATUS
GetVerbTableByteBoardId (
  IN  UINT8  BoardId,
  OUT COMMON_CHIPSET_AZALIA_VERB_TABLE  **VerbTableHeaderDataAddress
  )
{
  COMMON_CHIPSET_AZALIA_VERB_TABLE *AzaliaVerbTable;
  EFI_STATUS        Status;
  UINT8             Instance;
  VOID              *VerbTableRaw;
  UINT16            DwordOffset;
  UINT8             RegionId[FDM_ENTRY_REGION_ID_SIZE];
  UINT64            RegionOffset;
  UINT64            RegionSize;
  UINT32            Attribs;
  UINT8             Index;
  UINT8             NumberOfCodecs;
  COMMON_VERB_TABLE_BIN_HEADER  *CommonVerbTableBinHeader;
                        
  AzaliaVerbTable = NULL;
  Status = EFI_NOT_FOUND;
  Instance = 1;
  VerbTableRaw = NULL;
  DwordOffset  = 0;
  Index        = 0;
  NumberOfCodecs = 0;
  CommonVerbTableBinHeader = NULL;

  //
  //  Enumerate each verbtable RegionType in FDM
  //
  while (TRUE) {

    Status = FdmGetNAt (
      &gH2OFlashMapRegionVerbTableGuid,
      Instance,
      RegionId,
      &RegionOffset,
      &RegionSize,
      &Attribs
      );

    if (EFI_ERROR (Status)) {
      *VerbTableHeaderDataAddress = NULL;
      return Status;
    }

    //
    //  Search within Boards [ SUPPORT_BOARD_COUNT ]
    //  Expected SUPPORT_BOARD_COUNT is 6.
    //  See BTR 3.17.3.6 
    //  typedef struct _H2O_FLASH_MAP_VERB_TABLE_ID {
    //    UINT8  Reserved[10];
    //    UINT8  Boards[6];
    //  } H2O_FLASH_MAP_VERB_TABLE_ID;
    //
    for (Index = 0; Index < SUPPORT_BOARD_COUNT; Index++) {

      if (((H2O_FLASH_MAP_VERB_TABLE_ID *)(RegionId))->Boards[Index] == 0xff) {
        break;
      }
      //
      //  Compare BoardId with Boards array with platform board-id.
      //
      if (((H2O_FLASH_MAP_VERB_TABLE_ID *)(RegionId))->Boards[Index] == (UINT8)BoardId) {
        //
        //  VerbTableRaw : a pointer to matched verbtable binary entry.
        //
        VerbTableRaw = (VOID *)(UINTN)(FdmGetBaseAddr () + RegionOffset);

        //
        //  First byte of verbtable binary indicate total codec numbers.
        //  Ex. If there are two codecs exist, then two pairs of common table required to store pointers to its header and data,
        //      plus extra one to store NULL pointer.
        //
        //  NOTE: This pool is allocated to store pointers to meet the requirement of chipset init code.
        //        (COMMON_CHIPSET_AZALIA_VERB_TABLE is a common table to store pointers point to verbtable header and data)
        //        If pointer lengh in pei is 32bit long, in most of cases, the pool cost about 32bit * 4 ~ 32bit * 6 (16byte ~ 24byte).
        //        Memorymanagement in pei phase could be different from other phases, user need to careffully control the usage of memory.
        //        Any Violation of this function can resualt in unexpected memory issues.
        //        Caller has responsibility to free this pool when COMMON_CHIPSET_AZALIA_VERB_TABLE is nolonger used
        //        and the verb setting process in chipset init is finished.
        //
        NumberOfCodecs = *((UINT8 *)VerbTableRaw);
        AzaliaVerbTable = AllocateZeroPool (sizeof (COMMON_CHIPSET_AZALIA_VERB_TABLE) * (NumberOfCodecs + 1));

        if (AzaliaVerbTable == NULL) {
          *VerbTableHeaderDataAddress = NULL;
          Status = EFI_OUT_OF_RESOURCES;
          return Status;
        }

        //
        //  Binary format :
        //              ---------------------------------------------
        //       1Byte  |      Number of codecs                     | 
        //              ---------------------------------------------
        //       14Byte |   COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER |  --
        //              ---------------------------------------------   |
        //        2Byte |      Data size in Dword                   |   | codec1
        //              ---------------------------------------------   |
        //    non-fixed |      Verb table data                      |  --
        //              ---------------------------------------------
        //       14Byte |   COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER |  --
        //              ---------------------------------------------   |
        //        2Byte |      Data size in Dword                   |   | codec2
        //              ---------------------------------------------   |
        //    non-fixed |      Verb table data                      |  --
        //              ---------------------------------------------
        //              |                  ...                      |  -- codec n
        //

        //
        //  Move pointer to header of first codec.
        //
        CommonVerbTableBinHeader = (VOID *)(((UINT8 *)VerbTableRaw) + 1);
        //
        //  Install address of each header and data into COMMON_CHIPSET_AZALIA_VERB_TABLE.
        //
        for (Index = 0; Index < NumberOfCodecs; Index++) {
          AzaliaVerbTable[Index].VerbTableHeader = (VOID *)CommonVerbTableBinHeader;
          AzaliaVerbTable[Index].VerbTableData   = (VOID *)(CommonVerbTableBinHeader + 1);
          DwordOffset = CommonVerbTableBinHeader->DataSizeDword;
          //
          //  Move pointer to header of next codec.
          //
          CommonVerbTableBinHeader = CommonVerbTableBinHeader + 1;
          CommonVerbTableBinHeader = (VOID *)(((UINT32 *)CommonVerbTableBinHeader) + DwordOffset);
        }
        
        //
        //  The last pair must be NULL
        //
        AzaliaVerbTable[NumberOfCodecs].VerbTableHeader = NULL;
        AzaliaVerbTable[NumberOfCodecs].VerbTableData   = NULL;
        *VerbTableHeaderDataAddress = AzaliaVerbTable;

        return EFI_SUCCESS;
      }
    }
    Instance++;
  }

}

/**
  Find matched region by board-id, and contruct a COMMON_CHIPSET_AZALIA_VERB_TABLE.
  If match not found, upate VerbTableHeaderDataAddress to NULL

  @param[out]  VerbTableHeaderDataAddress  A pointer to address of verbtable.

  @retval EFI_SUCCESS          Get verbtable entry successfully.
  @retval EFI_NOT_FOUND        Target not found.
**/

EFI_STATUS
EFIAPI
GetVerbTable (
  OUT COMMON_CHIPSET_AZALIA_VERB_TABLE  **VerbTableHeaderDataAddress
  )
{
  EFI_STATUS Status;
  UINT64     BoardId;

  Status = EFI_NOT_FOUND;
  BoardId = LibPcdGetSku();
  //
  //  Search original FDM entry array, compare each UINT8 boards[] in regionid,
  //  if not found, use "0" to search default item.
  //
  Status = GetVerbTableByteBoardId((UINT8)BoardId, VerbTableHeaderDataAddress);
  if (EFI_ERROR(Status)) {
    Status = GetVerbTableByteBoardId(0, VerbTableHeaderDataAddress);
  }
  return Status;
}

/**
  This function searches fdm 3 extension area if board-id > 0xff,
  or it searches fdm original area, if target not found, it searches
  default item with board-id "0".

  @param[out]     VerbTable    A pointer to the address of common verbtable entry.

  @retval EFI_SUCCESS          Get verbtable successfully.
  @retval EFI_NOT_FOUND        Target not found.
**/
EFI_STATUS
EFIAPI
GetVerbTableEx (
  OUT COMMON_CHIPSET_AZALIA_VERB_TABLE      **VerbTable
  )
{
  EFI_STATUS    Status;
  UINT64        BoardId;
  H2O_FLASH_DEVICE_MAP_ENTRY  *Entry;

  Entry = NULL;
  Status = EFI_NOT_FOUND;

  BoardId = LibPcdGetSku();

  if (BoardId > 0xFF) {
    Status = GetVerbTableQwordBoard(BoardId, VerbTable);
    if (EFI_ERROR(Status)) {
      //
      //  Use board id "0" search again.
      //
      Status = GetVerbTableByteBoardId(0, VerbTable);
    }
  } else {
    //
    //  Search using 8bit board-id.
    //
    Status = GetVerbTableByteBoardId((UINT8)BoardId, VerbTable);
    if (EFI_ERROR(Status)) {
      //
      //  Use board id "0" search again.
      //
      Status = GetVerbTableByteBoardId(0, VerbTable);
    }
  }
  return Status;
}
