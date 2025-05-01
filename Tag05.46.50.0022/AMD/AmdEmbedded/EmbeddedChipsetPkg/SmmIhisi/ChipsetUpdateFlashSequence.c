/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "ChipsetUpdateFlashSequence.h"

#define                                     UPDATE_EXT_ITEM_FUN_TABLE_MAX 8

STATIC BOOLEAN                              mUpdateSequenceEnabled = FALSE;

/**
  AH=11h, Get platform information.Update Extend Platform

  @retval EFI_SUCCESS        Get platform information successful.
  @return Other              Get platform information failed.
**/
EFI_STATUS
EFIAPI
ChipsetFbtsUpdateExtendPlatform (
  VOID
  )
{
  EFI_STATUS                                Status;
  UINT8                                     Index;
  UINT32                                    BufferSize;
  UINT32                                    DataItemUseSize;
  UINT32                                    DataItemTotalSize;
  EXTEND_PLATFORM_DATA_ITEM                 *DataItemPtr;
  EXTEND_PLATFORM_DATA_ITEM                 *TempDataItemPtr;
  FBTS_EXTEND_PLATFORM_INFO_TABLE_OUTPUT    *ExtendPlatformInfoOutput;
  FBTS_EXTEND_PLATFORM_INFO_TABLE_INPUT     *ExternPlatformInfoInput;
  UPDATE_EXT_ITEM_FUN_TABLE                 UpdateFunTable[UPDATE_EXT_ITEM_FUN_TABLE_MAX];
  UINTN                                     UpdateFunCount;

  ExternPlatformInfoInput = (FBTS_EXTEND_PLATFORM_INFO_TABLE_INPUT *)(UINTN)mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RSI);

  if (!mH2OIhisi->BufferInCmdBuffer ((VOID *) ExternPlatformInfoInput, sizeof (FBTS_EXTEND_PLATFORM_INFO_TABLE_INPUT))){
    return IHISI_BUFFER_RANGE_ERROR;
  }
  if (ExternPlatformInfoInput->Signature != EXTEND_PLATFORM_INPUT_BUFFER_SIGNATURE) {
    //
    // Tool do not support Extend Platform Info, no change , return success.
    //
    return IHISI_SUCCESS;
  }

  if (ExternPlatformInfoInput->StructureSize < sizeof (FBTS_EXTEND_PLATFORM_INFO_TABLE_INPUT)) {
    //
    // The structure size should bigger than Signature size
    //
    return IHISI_UNSUPPORTED_FUNCTION;
  }

  BufferSize = ExternPlatformInfoInput->StructureSize;
  if (!mH2OIhisi->BufferInCmdBuffer ((VOID *) ExternPlatformInfoInput, BufferSize)){
    return IHISI_BUFFER_RANGE_ERROR;
  }

  ExtendPlatformInfoOutput = (FBTS_EXTEND_PLATFORM_INFO_TABLE_OUTPUT *)(UINTN)mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RSI);
  if (!mH2OIhisi->BufferInCmdBuffer ((VOID *) ExtendPlatformInfoOutput, sizeof (FBTS_EXTEND_PLATFORM_INFO_TABLE_OUTPUT))){
    return IHISI_BUFFER_RANGE_ERROR;
  }
  DataItemPtr = ExtendPlatformInfoOutput->DataItem;
  DataItemTotalSize = BufferSize - EXTEND_OFFSET(FBTS_EXTEND_PLATFORM_INFO_TABLE_OUTPUT, DataItem);
  TempDataItemPtr = AllocatePool (DataItemTotalSize);
  if (TempDataItemPtr == NULL) {
    return IHISI_OUT_OF_RESOURCES;
  }
  ExtendPlatformInfoOutput->DataItemCount = 0;

  UpdateFunCount = 0;

  //
  // BuildDateTimeID
  //
  ASSERT (UpdateFunCount < UPDATE_EXT_ITEM_FUN_TABLE_MAX);
  if (UpdateFunCount >= UPDATE_EXT_ITEM_FUN_TABLE_MAX) {
    return IHISI_OUT_OF_RESOURCES;
  }
  UpdateFunTable[UpdateFunCount].DataID           = BuildDateTimeID;
  UpdateFunTable[UpdateFunCount].UpdateExtItemFun = UpdateExtendPlatformBuildDateTimeInfo;
  UpdateFunCount++;

  //
  // BiosUpdateSequence
  //
  if (FeaturePcdGet (PcdH2OBiosUpdateFlashSequenceEnabled)) {
    ASSERT (UpdateFunCount < UPDATE_EXT_ITEM_FUN_TABLE_MAX);
    if (UpdateFunCount >= UPDATE_EXT_ITEM_FUN_TABLE_MAX) {
      return IHISI_OUT_OF_RESOURCES;
    }
    UpdateFunTable[UpdateFunCount].DataID           = BiosUpdateSequence;
    UpdateFunTable[UpdateFunCount].UpdateExtItemFun = ChipsetUpdateExtendPlatformBiosUpdateSequence;
    UpdateFunCount++;
  }

  for (Index = 0, DataItemUseSize = 0; Index < UpdateFunCount; Index++) {
    ZeroMem (TempDataItemPtr, DataItemTotalSize);
    TempDataItemPtr->DataID = UpdateFunTable[Index].DataID;
    //
    //DataSize :Indicates how many available data size in Data field (offset 05h). Not include Data_ID (offset 00h) and Data_Size (offset 01h) fields.
    //
    TempDataItemPtr->DataSize = DataItemTotalSize - EXTEND_OFFSET(EXTEND_PLATFORM_DATA_ITEM, Data);
    Status = UpdateFunTable[Index].UpdateExtItemFun(TempDataItemPtr);
    if (Status == EFI_OUT_OF_RESOURCES) {
      FreePool (TempDataItemPtr);
      return IHISI_OUT_OF_RESOURCES;
    }

    if (Status == EFI_SUCCESS || Status == EFI_BUFFER_TOO_SMALL) {
      //
      //Add each function dataitem use size to DataItemUseSize
      //
      ExtendPlatformInfoOutput->Signature = EXTEND_PLATFORM_OUTPUT_BUFFER_SIGNATURE;
      DataItemUseSize += TempDataItemPtr->DataSize + EXTEND_OFFSET(EXTEND_PLATFORM_DATA_ITEM, Data);
      if (DataItemUseSize < DataItemTotalSize) {
        //
        //Copy and point to next DataItem start address.
        //
        ExtendPlatformInfoOutput->DataItemCount ++;
        CopyMem (DataItemPtr, TempDataItemPtr, TempDataItemPtr->DataSize + EXTEND_OFFSET(EXTEND_PLATFORM_DATA_ITEM, Data));
        DataItemPtr = (EXTEND_PLATFORM_DATA_ITEM *)((UINT8 *)DataItemPtr + TempDataItemPtr->DataSize + EXTEND_OFFSET(EXTEND_PLATFORM_DATA_ITEM, Data));
      }
    }
  }
  FreePool (TempDataItemPtr);
  //
  //if DataItemUseSize over AP provide buffer size(DataItemTotalSize),restore signature and return buffer to small.
  //
  if (DataItemUseSize > DataItemTotalSize) {
    ExtendPlatformInfoOutput->DataItemCount = DataItemUseSize;
    return IHISI_OB_LEN_TOO_SMALL;
  }
  return IHISI_SUCCESS;
}

EFI_STATUS
UpdateExtendPlatformBuildDateTimeInfo (
  IN OUT EXTEND_PLATFORM_DATA_ITEM                *ExtInfoDataItemPtr
  )
{
  UINTN                                     StringSize;
  EFI_STATUS                                Status;
  CHAR16                                    StringDate[MODEL_DATE_SIZE];
  CHAR16                                    StringTime[MODEL_TIME_SIZE];
  UINT32                                    BufferSize;

  BufferSize = ExtInfoDataItemPtr->DataSize;
  ExtInfoDataItemPtr->DataSize = 0;

  //
  // Get build date string
  //
  StringSize = MODEL_DATE_SIZE;
  Status  = GetBvdtInfo ((BVDT_TYPE) BvdtBuildDate, &StringSize, StringDate);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get build time string
  //
  Status  = GetBvdtInfo ((BVDT_TYPE) BvdtBuildTime, &StringSize, StringTime);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ExtInfoDataItemPtr->DataSize = (UINT32)(StrSize (StringDate) + StrSize (StringTime));
  if (ExtInfoDataItemPtr->DataSize > BufferSize) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //Merge Build date String . Build time string
  StringSize = StrSize (StringDate);
  StringDate[(StrSize (StringDate)/sizeof (StringDate[0])) - 1] = L' ';
  CopyMem ( ExtInfoDataItemPtr->Data, StringDate, StringSize);
  //
  // Update build time string
  //
  ExtInfoDataItemPtr = (EXTEND_PLATFORM_DATA_ITEM *)(ExtInfoDataItemPtr->Data + StringSize);
  StringSize = StrSize (StringTime);
  CopyMem ( ExtInfoDataItemPtr, StringTime, StringSize);

  return EFI_SUCCESS;
}

/**
  Report the bios Update sequence for AH=11h.

  @param[in,out] ExtInfoDataItemPtr    Pointer to ExtInfoData.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
ChipsetUpdateExtendPlatformBiosUpdateSequence (
  IN OUT EXTEND_PLATFORM_DATA_ITEM          *ExtInfoDataItemPtr
  )
{
  EFI_STATUS             Status;
  UINT32                 BufferSize;
  UINT32                 DataSize;
  BIOS_UPDATE_SEQUENCE   *BiosUpdateSequence;
  BIOS_UPDATE_AREA       *BiosUpdateArea;
  UINT32                 BiosRegionOffset;
  UINT32                 RomSize;
  UINT8                  Flag;
  UINTN                  Size;

  RomSize = 0;
  Status = GetRomSizeFromBlockMap (&RomSize);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  if (RomSize < PcdGet32 (PcdFlashAreaSize)) {
    return EFI_UNSUPPORTED;
  }

  BiosRegionOffset = RomSize - PcdGet32 (PcdFlashAreaSize);

  DataSize = sizeof (BIOS_UPDATE_SEQUENCE) + sizeof (BIOS_UPDATE_AREA) * 0x0A;
  BiosUpdateSequence = AllocatePool (DataSize);
  if (BiosUpdateSequence == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Number of Areas
  BiosUpdateSequence->AreaCount = 0x0A;
  BiosUpdateArea = (BIOS_UPDATE_AREA *)(BiosUpdateSequence + 1);
  // Area 1: Flash B part of PSP
  BiosUpdateArea[0].Type        = FbtsRomMapPrimaryBootBlock;
  BiosUpdateArea[0].Address     = PcdGet32 (PcdFlashPspDirBBase);
  BiosUpdateArea[0].DataSize    = PcdGet32 (PcdFlashPspNvRamBase) - PcdGet32 (PcdFlashPspDirBBase);
  BiosUpdateArea[0].DataOffset  = PcdGet32 (PcdFlashPspDirBBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 2: Flash B part of Pei
  BiosUpdateArea[1].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[1].Address     = PcdGet32 (PcdFlashPeiFlashBBase);
  BiosUpdateArea[1].DataSize    = PcdGet32 (PcdFlashFvMainBase) - PcdGet32 (PcdFlashPeiFlashBBase);
  BiosUpdateArea[1].DataOffset  = PcdGet32 (PcdFlashPeiFlashBBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 3: Flash A part of PSP
  BiosUpdateArea[2].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[2].Address     = PcdGet32 (PcdFlashPspDirABase);
  BiosUpdateArea[2].DataSize    = PcdGet32 (PcdFlashPspDirBBase) - PcdGet32 (PcdFlashPspDirABase);
  BiosUpdateArea[2].DataOffset  = PcdGet32 (PcdFlashPspDirABase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 4: Flash A part of Pei
  BiosUpdateArea[3].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[3].Address     = PcdGet32 (PcdFlashFvRecoveryBase);
  BiosUpdateArea[3].DataSize    = PcdGet32 (PcdFlashPeiFlashBBase) - PcdGet32 (PcdFlashFvRecoveryBase);
  BiosUpdateArea[3].DataOffset  = PcdGet32 (PcdFlashFvRecoveryBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 5: Flash PSP RPMC NvRam
  BiosUpdateArea[4].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[4].Address     = PcdGet32 (PcdFlashPspNvRamBase);
  BiosUpdateArea[4].DataSize    = PcdGet32 (PcdFlashFvRecoveryBase) - PcdGet32 (PcdFlashPspNvRamBase);
  BiosUpdateArea[4].DataOffset  = PcdGet32 (PcdFlashPspNvRamBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 6: Flash Fv Main
  BiosUpdateArea[5].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[5].Address     = PcdGet32 (PcdFlashFvMainBase);
  BiosUpdateArea[5].DataSize    = PcdGet32 (PcdFlashAreaBaseAddress) + PcdGet32 (PcdFlashAreaSize) - PcdGet32 (PcdFlashFvMainBase);
  BiosUpdateArea[5].DataOffset  = PcdGet32 (PcdFlashFvMainBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 7: Flash Fv Ec
  BiosUpdateArea[6].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[6].Address     = PcdGet32 (PcdH2OFlashFvEcBase);
  BiosUpdateArea[6].DataSize    = PcdGet32 (PcdFlashPSPL1HdrBase) - PcdGet32 (PcdH2OFlashFvEcBase);
  BiosUpdateArea[6].DataOffset  = PcdGet32 (PcdH2OFlashFvEcBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 8: Flash B of PSP L1 Hdr
  BiosUpdateArea[7].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[7].Address     = PcdGet32 (PcdFlashBackupPSPL1HdrBase);
  BiosUpdateArea[7].DataSize    = PcdGet32 (PcdFlashImageSlot1HdrBase) - PcdGet32 (PcdFlashBackupPSPL1HdrBase);
  BiosUpdateArea[7].DataOffset  = PcdGet32 (PcdFlashBackupPSPL1HdrBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 9: Flash A of PSP L1 Hdr
  BiosUpdateArea[8].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[8].Address     = PcdGet32 (PcdFlashPSPL1HdrBase);
  BiosUpdateArea[8].DataSize    = PcdGet32 (PcdFlashBackupPSPL1HdrBase) - PcdGet32 (PcdFlashPSPL1HdrBase);
  BiosUpdateArea[8].DataOffset  = PcdGet32 (PcdFlashPSPL1HdrBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;
  // Area 10: Flash Image Slot Hdr
  BiosUpdateArea[9].Type        = FbtsRomMapSecondaryBootBlock;
  BiosUpdateArea[9].Address     = PcdGet32 (PcdFlashImageSlot1HdrBase);
  BiosUpdateArea[9].DataSize    = PcdGet32 (PcdFlashPspDirABase) - PcdGet32 (PcdFlashImageSlot1HdrBase);
  BiosUpdateArea[9].DataOffset  = PcdGet32 (PcdFlashImageSlot1HdrBase) - PcdGet32 (PcdFlashAreaBaseAddress) + BiosRegionOffset;

  Size = sizeof (Flag);
  Status = CommonGetVariable (
             PREVENT_FLASH_POWER_LOSS,
             &gPreventFlashPowerLossGuid,
             &Size,
             &Flag
             );
  if (EFI_ERROR (Status)) {
    Flag = 0x01;
    Status = CommonSetVariable (
               PREVENT_FLASH_POWER_LOSS,
               &gPreventFlashPowerLossGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               Size,
               &Flag
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  BufferSize = ExtInfoDataItemPtr->DataSize;

  ExtInfoDataItemPtr->DataSize = DataSize;
  if (ExtInfoDataItemPtr->DataSize > BufferSize) {
    FreePool (BiosUpdateSequence);
    return EFI_BUFFER_TOO_SMALL;
  }

  CopyMem (ExtInfoDataItemPtr->Data, BiosUpdateSequence, DataSize);

  //
  // Clear update flag if using sequence update.
  //
  SetFirmwareUpdatingFlag (FALSE);
  mUpdateSequenceEnabled = TRUE;

  FreePool (BiosUpdateSequence);
  return EFI_SUCCESS;
}

/**
  Get rom size from block map.

  @param[out] RomSize         ROM size.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
GetRomSizeFromBlockMap (
  OUT  UINT32                               *RomSize
  )
{
  FLASH_DEVICE                              *Buffer;
  EFI_STATUS                                Status;
  UINT16                                    BlockMap[3];
  UINT8                                     SpiFlashNumber;

  Buffer = AllocatePool (sizeof (FLASH_DEVICE));
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = mSmmFwBlockService->DetectDevice (
                                 mSmmFwBlockService,
                                 (UINT8 *)Buffer
                                 );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyMem ((VOID *) BlockMap, &(Buffer->DeviceInfo.BlockMap), sizeof (FD_BLOCK_MAP));
  if (Buffer->DeviceInfo.Size != 0xFF) {
    Status = mSmmFwBlockService->GetSpiFlashNumber (
                                   mSmmFwBlockService,
                                   &SpiFlashNumber
                                   );

    if (!EFI_ERROR (Status)) {
      BlockMap[1] *= 1 << (SpiFlashNumber - 1);
    }
  }

  FreePool (Buffer);

  *RomSize = (UINT32)(BlockMap[0] * BlockMap[1] * 0x100);

  return EFI_SUCCESS;
}

STATIC
IHISI_REGISTER_TABLE
CHIPSET_SECURE_FLASH_REGISTER_TABLE[] = {
  //
  // AH=11h
  //
  { FBTSGetPlatformInfo, "S11Cs_UpExtPlatform", ChipsetFbtsUpdateExtendPlatform}
};

EFI_STATUS
ChipsetFbtsUpdateExtendPlatformInit (
  VOID
  )
{
  EFI_STATUS              Status;
  IHISI_REGISTER_TABLE    *SubFuncTable;
  UINT16                  TableCount;

  SubFuncTable = CHIPSET_SECURE_FLASH_REGISTER_TABLE;
  TableCount = sizeof(CHIPSET_SECURE_FLASH_REGISTER_TABLE)/sizeof(CHIPSET_SECURE_FLASH_REGISTER_TABLE[0]);
  Status = RegisterIhisiSubFunction (SubFuncTable, TableCount);
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}