/***************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/AmdPspFlashAccLib.h>
#include <Features/ABRecovery/Include/ABRecoveryBaseLib.h>

#define BACKUP_ISHA_OFFSET          0x100

SOC_ID_STRUCT mSocId[] = {
  {F19_CZN_RAW_ID, ZEN3_FP6},
  {F17_LCN_RAW_ID, RN_FP6},
};

EFI_STATUS
EFIAPI
AmdCpmABRecoveryPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
AmdCpmABRecoveryPeimCZN (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmABRecoveryNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPspRecoveryDetectPpiGuid,
  AmdCpmABRecoveryPeim
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmABRecoveryNotifyCZN = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),  &gAmdPspRecoveryDetectPpiGuid,  AmdCpmABRecoveryPeimCZN};

BOOLEAN
CheckISHHeaderCorrupted(
  IN       UINT32                      ISHActiveAddress,
  IN       UINT32                      ISHAlternativeAddress
  )
{
  EFI_PHYSICAL_ADDRESS            ISHActiveBuffer;
  EFI_PHYSICAL_ADDRESS            ISHAlternativeBuffer;
  IMAGE_SLOT_HEADER_V2            *ISHActiveHeader;
  IMAGE_SLOT_HEADER_V2            *ISHAlternativeHeader;
  EFI_STATUS                      Status;

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)sizeof(IMAGE_SLOT_HEADER_V2)),
             &ISHActiveBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate ISHActiveBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISHActiveAddress=%x\n", __FUNCTION__, ISHActiveAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISHAlternativeAddress=%x\n", __FUNCTION__, ISHAlternativeAddress));

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)sizeof(IMAGE_SLOT_HEADER_V2)),
             &ISHAlternativeBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate ISHAlternativeBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  if (MapSpiDataToBuffer ((UINT32)ISHActiveAddress, (UINT8*)(UINTN)ISHActiveBuffer, sizeof(IMAGE_SLOT_HEADER_V2)) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-% a ISHActiveBuffer fail\n", __FUNCTION__));
    return FALSE;
  }
  ISHActiveHeader = (IMAGE_SLOT_HEADER_V2*)(UINTN)ISHActiveBuffer;

  if (MapSpiDataToBuffer ((UINT32)ISHAlternativeAddress, (UINT8*)(UINTN)ISHAlternativeBuffer, sizeof(IMAGE_SLOT_HEADER_V2)) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a ISHAlternativeBuffer fail\n", __FUNCTION__));
    return FALSE;
  }
  ISHAlternativeHeader = (IMAGE_SLOT_HEADER_V2*)(UINTN)ISHAlternativeBuffer;

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISHActiveHeader->Priority=%x\n", __FUNCTION__, ISHActiveHeader->Priority));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISHActiveHeader->PspId=%x\n", __FUNCTION__, ISHActiveHeader->PspId));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISHAlternativeHeader->Priority=%x\n", __FUNCTION__, ISHAlternativeHeader->Priority));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISHAlternativeHeader->PspId=%x\n", __FUNCTION__, ISHAlternativeHeader->PspId));

  if ((ISHActiveHeader->CheckSum != ISHAlternativeHeader->CheckSum) ||
      (ISHActiveHeader->Priority != ISHAlternativeHeader->Priority) ||
      (ISHActiveHeader->UpdateRetries != ISHAlternativeHeader->UpdateRetries) ||
      (ISHActiveHeader->GlitchRetries != ISHAlternativeHeader->GlitchRetries) ||
      (ISHActiveHeader->ImageSlotAddr != ISHAlternativeHeader->ImageSlotAddr) ||
      (ISHActiveHeader->PspId != ISHAlternativeHeader->PspId)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ImageSlotHeader got corrupted \n", __FUNCTION__));
    return TRUE;
   }

  return FALSE;
}

BOOLEAN
RestoreISHHeader (
  IN UINT32                         ActiveSlot,
  IN UINT64                         SourceEntryAddress,
  IN UINT32                         SourceEntrySize,
  IN UINT64                         DestinationEntryAddress,
  IN UINT32                         DestinationEntrySize
  )
{
  EFI_PHYSICAL_ADDRESS              TempBufferAddr;
  EFI_STATUS                        Status;

  SourceEntryAddress = SourceEntryAddress & (~(SIZE_4KB -1));
  DestinationEntryAddress = DestinationEntryAddress & (~(SIZE_4KB -1));

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)DestinationEntrySize),
             &TempBufferAddr
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer1 fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-TempBufferAddr 0x%x\n", __FUNCTION__, TempBufferAddr));

  if (MapSpiDataToBuffer ((UINT32)SourceEntryAddress, (UINT8*)(UINTN)TempBufferAddr, SourceEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  Status = PspEraseFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  if (ActiveSlot != 0) {
    //
    // Flash ISH-A back up data which existed in ISH-B area.
    //
    TempBufferAddr += 0x100;
  }
  DestinationEntrySize = sizeof (IMAGE_SLOT_HEADER_V2);
  Status = PspWriteFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize, (UINT8 *)(UINTN)TempBufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
RestoreDirectoryEntry (
  IN UINT64                          SourceEntryAddress,
  IN UINT32                          SourceEntrySize,
  IN UINT64                          DestinationEntryAddress,
  IN UINT32                          DestinationEntrySize
 )
{
  EFI_PHYSICAL_ADDRESS            BufferAddr;
  EFI_STATUS                      Status;

  // cover the part of recovery data.
  SourceEntrySize += SIZE_4KB;
  DestinationEntrySize = SourceEntrySize;

  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)SourceEntrySize),
             &BufferAddr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));

  if (MapSpiDataToBuffer ((UINT32)SourceEntryAddress, (UINT8*)(UINTN)BufferAddr, SourceEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  if (DestinationEntrySize > UINT_MAX ) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6- size incorrect \n", __FUNCTION__));
    return FALSE;
  }

  Status = PspEraseFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = PspWriteFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
UpdateL2APspHeaderBuffer (
  IN EFI_PHYSICAL_ADDRESS         BufferAddr
  )
{
  UINT8                           TotalEntries;
  EFI_PHYSICAL_ADDRESS            BufferAddrTemp;
  UINT32                          Checksum;
  UINT32                          Index;

  // Update L2A BIOS_REGION_DIR or PSP_NV_DATA if it is not same as in L2B
  if ((PcdGet64 (PcdCpmBIOSPointAddressL2A) != 0) || (PcdGet64 (PcdCpmPSPNVDataL2A) != 0)) {
    TotalEntries = *(UINT8*)(UINTN)(BufferAddr + 0x8);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP TotalEntries=%x\n", __FUNCTION__, TotalEntries));
    BufferAddrTemp = BufferAddr + sizeof (PSP_DIRECTORY_HEADER);
    for (Index = 0; Index < TotalEntries; Index++) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP BufferAddrTemp=%x\n", __FUNCTION__, BufferAddrTemp));
      if (*(UINT8*)(UINTN)BufferAddrTemp == BIOS_REGION_DIR) {
        if (PcdGet64 (PcdCpmBIOSPointAddressL2A) != 0) {
          *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmBIOSPointAddressL2A);
          DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS_REGION_DIR Point Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
        }
      }
      if (*(UINT8*)(UINTN)BufferAddrTemp == PSP_NV_DATA) {
        if (PcdGet64 (PcdCpmPSPNVDataL2A) != 0) {
          *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmPSPNVDataL2A);
          DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP_NV_DATA Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
        }
      }
      BufferAddrTemp += sizeof (PSP_DIRECTORY_ENTRY);
    }
    BufferAddrTemp = BufferAddr + 0x8;
    Checksum = Fletcher32 ((UINT16*)(UINTN)BufferAddrTemp, ((TotalEntries * 0x10 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
    BufferAddrTemp = BufferAddr + 0x4;
    *(UINT32*)(UINTN)(BufferAddrTemp ) = Checksum;
  }

  return TRUE;
}

BOOLEAN
RestoreL2APSPHeader (
  IN UINT64                          SourceEntryAddress,
  IN UINT32                          SourceEntrySize,
  IN UINT64                          DestinationEntryAddress,
  IN UINT32                          DestinationEntrySize
  )
{
  UINT8                           TotalEntries;
  EFI_PHYSICAL_ADDRESS            BufferAddrTemp;
  UINT32                          Checksum;
  EFI_PHYSICAL_ADDRESS            BufferAddr;
  EFI_STATUS                      Status;
  UINT32                          Index;

  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)SourceEntrySize),
             &BufferAddr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));

  if (MapSpiDataToBuffer ((UINT32)SourceEntryAddress, (UINT8*)(UINTN)BufferAddr, SourceEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  // Update L2A BIOS_REGION_DIR or PSP_NV_DATA if it is not same as in L2B
  if ((PcdGet64 (PcdCpmBIOSPointAddressL2A) != 0) || (PcdGet64 (PcdCpmPSPNVDataL2A) != 0)) {
    TotalEntries = *(UINT8*)(UINTN)(BufferAddr + 0x8);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP TotalEntries=%x\n", __FUNCTION__, TotalEntries));
    BufferAddrTemp = BufferAddr + sizeof (PSP_DIRECTORY_HEADER);
    for (Index = 0; Index < TotalEntries; Index++) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP BufferAddrTemp=%x\n", __FUNCTION__, BufferAddrTemp));
      if (*(UINT8*)(UINTN)BufferAddrTemp == BIOS_REGION_DIR) {
        if (PcdGet64 (PcdCpmBIOSPointAddressL2A) != 0) {
          *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmBIOSPointAddressL2A);
          DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS_REGION_DIR Point Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
        }
      }
      if (*(UINT8*)(UINTN)BufferAddrTemp == PSP_NV_DATA) {
        if (PcdGet64 (PcdCpmPSPNVDataL2A) != 0) {
          *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmPSPNVDataL2A);
          DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP_NV_DATA Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
        }
      }
      BufferAddrTemp += sizeof (PSP_DIRECTORY_ENTRY);
    }
    BufferAddrTemp = BufferAddr + 0x8;
    Checksum = Fletcher32 ((UINT16*)(UINTN)BufferAddrTemp, ((TotalEntries * 0x10 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
    BufferAddrTemp = BufferAddr + 0x4;
    *(UINT32*)(UINTN)(BufferAddrTemp ) = Checksum;

    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));

    if (DestinationEntrySize > UINT_MAX ) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End size incorrect \n", __FUNCTION__));
      return FALSE;
    }
  }

  Status = PspEraseFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = PspWriteFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
UpdateL2ABIOSHeaderBuffer (
  IN EFI_PHYSICAL_ADDRESS         BufferAddr
  )
{
  UINT8                           TotalEntries;
  EFI_PHYSICAL_ADDRESS            BufferAddrTemp;
  UINT32                          Checksum;
  UINT32                          Index;
//[-start-240202-IB20840046-add]//
  BIOS_DIRECTORY_ENTRY            *BiosDirectoryEntry;
//[-end-240202-IB20840046-add]//
  
  // Update L2A PEI_ENTRY if it is not same as in L2B
  if (PcdGet64 (PcdCpmPEIPointAddressL2A) != 0) {
    TotalEntries = *(UINT8*)(UINTN)(BufferAddr + 0x8);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A Bios TotalEntries=%x\n", __FUNCTION__, TotalEntries));
    BufferAddrTemp = BufferAddr + sizeof (PSP_DIRECTORY_HEADER);
    for (Index = 0; Index < TotalEntries; Index++) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BufferAddrTemp=%x\n", __FUNCTION__, BufferAddrTemp));
//[-start-240202-IB20840046-modify]//
      if (*(UINT8*)(UINTN)BufferAddrTemp == PEI_ENTRY) {
        BiosDirectoryEntry = (BIOS_DIRECTORY_ENTRY*)BufferAddrTemp;
        if (BiosDirectoryEntry->TypeAttrib.Instance == 0) {
          *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmPEIPointAddressL2A);
          DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS PEI Point Addr 0=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 0x8))));
        } else if (BiosDirectoryEntry->TypeAttrib.Instance == 1) {
          *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmPEIPointAddressL2B);
          DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS PEI Point Addr 1=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 0x8))));
        }
      }
//[-end-240202-IB20840046-modify]//
      BufferAddrTemp += sizeof (BIOS_DIRECTORY_ENTRY);
    }
    BufferAddrTemp = BufferAddr + 0x8;
    Checksum = Fletcher32 ((UINT16*)(UINTN)BufferAddrTemp, ((TotalEntries * 0x18 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
    BufferAddrTemp = BufferAddr + 0x4;
    *(UINT32*)(UINTN)(BufferAddrTemp) = Checksum;
  }

  return TRUE;
}

BOOLEAN
RestoreL2ABIOSHeader (
  IN UINT64                          SourceEntryAddress,
  IN UINT32                          SourceEntrySize,
  IN UINT64                          DestinationEntryAddress,
  IN UINT32                          DestinationEntrySize
  )
{
  UINT8                           TotalEntries;
  EFI_PHYSICAL_ADDRESS            BufferAddrTemp;
  UINT32                          Checksum;
  EFI_PHYSICAL_ADDRESS            BufferAddr;
  EFI_STATUS                      Status;
  UINT32                          Index;
  UINT32                          EraseSize;

  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)SourceEntrySize),
             &BufferAddr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));

  if (MapSpiDataToBuffer ((UINT32)SourceEntryAddress, (UINT8*)(UINTN)BufferAddr, SourceEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  // Update L2A PEI_ENTRY if it is not same as in L2B
  if (PcdGet64 (PcdCpmPEIPointAddressL2A) != 0) {
    TotalEntries = *(UINT8*)(UINTN)(BufferAddr + 0x8);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A Bios TotalEntries=%x\n", __FUNCTION__, TotalEntries));
    BufferAddrTemp = BufferAddr + sizeof (PSP_DIRECTORY_HEADER);
    for (Index = 0; Index < TotalEntries; Index++) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BufferAddrTemp=%x\n", __FUNCTION__, BufferAddrTemp));
      if (*(UINT8*)(UINTN)BufferAddrTemp == PEI_ENTRY) {
        *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = (UINT32)PcdGet64 (PcdCpmPEIPointAddressL2A);
        DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS PEI Point Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 0x8))));
      }
      BufferAddrTemp += sizeof (BIOS_DIRECTORY_ENTRY);
    }
    BufferAddrTemp = BufferAddr + 0x8;
    Checksum = Fletcher32 ((UINT16*)(UINTN)BufferAddrTemp, ((TotalEntries * 0x18 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
    BufferAddrTemp = BufferAddr + 0x4;
    *(UINT32*)(UINTN)(BufferAddrTemp) = Checksum;

    if (DestinationEntrySize > UINT_MAX) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End size incorrect \n", __FUNCTION__));
      return FALSE;
    }
  }

  if (!ALIGN_4K_CHECK (DestinationEntrySize)) {
    EraseSize = (DestinationEntrySize & (~((UINT32)0xFFF))) + ALIGNMENT_4K;
  } else {
    EraseSize = DestinationEntrySize;
  }
  Status = PspEraseFlash ((UINTN)DestinationEntryAddress, (UINTN*)&EraseSize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = PspWriteFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
GetPSPDirectorySize (
  IN       UINT32                      Address,
  IN  OUT  UINT32                      *Size
  )
{
  EFI_PHYSICAL_ADDRESS            PspDirBuffer;
  PSP_DIRECTORY                   *PspDir;
  EFI_STATUS                      Status;

  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize),
             &PspDirBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp Buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Address=%x Size=%x \n", __FUNCTION__, Address,Size));

  if (MapSpiDataToBuffer ((UINT32)Address, (UINT8*)(UINTN)PspDirBuffer, MaxPspDirSize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  PspDir = (PSP_DIRECTORY*)(UINTN)PspDirBuffer;
  *Size = PspDir->Header.AdditionalInfo.Field.MaxSize * SIZE_4KB;

  return TRUE;
}

BOOLEAN
GetBIOSDirectorySize (
  IN       UINT32                      Address,
  IN  OUT  UINT32                      *Size
  )
{
  EFI_PHYSICAL_ADDRESS            BiosDirBuffer;
  BIOS_DIRECTORY                   *BiosDir;
  EFI_STATUS                      Status;

  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize),
             &BiosDirBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp Buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Address=%x Size=%x \n", __FUNCTION__, Address,Size));

  if (MapSpiDataToBuffer ((UINT32)Address, (UINT8*)(UINTN)BiosDirBuffer, MaxPspDirSize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  BiosDir = (BIOS_DIRECTORY*)(UINTN)BiosDirBuffer;
  *Size = BiosDir->Header.AdditionalInfo.Field.MaxSize * SIZE_4KB;

  return TRUE;
}

BOOLEAN
UpdateL2BFromL2A (
  IN CPM_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         APIStatus;
  PSP_DIRECTORY                   *PspDir;
  IMAGE_SLOT_INFO                 *Slot;
  UINT32                          CurrentBootPartition;
  EFI_PHYSICAL_ADDRESS            GlobalBufferAddr;
  // L2 Active
  UINT8                           ActiveSlotIndex;
  UINT32                          ActivePSPLevel2BaseAddress;
  UINT32                          ActivePSPLevel2Size;
  UINT32                          ActiveBiosLevel2BaseAddress;
  UINT64                          ActiveBiosLevel2BaseAddress64;
  UINT32                          ActiveBiosLevel2BSize;
  // L2 Backup
  UINT8                           AlternativeSlotIndex;
  UINT32                          AlternativePSPLevel2BaseAddress;
  UINT32                          AlternativePSPLevel2Size;
  UINT32                          AlternativeBIOSLevel2BaseAddress;
  UINT32                          AlternativeBIOSLevel2Size;
  UINT64                          ISHActiveBase;
  UINT64                          ISHAlernativeBase;

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Start\n", __FUNCTION__));

  //Call PspMboxBiosGetActiveBootPartitionId to know which partition is being executed
  Status = PspMboxBiosGetActiveBootPartitionId (&CurrentBootPartition);
  if (CurrentBootPartition != 0) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-No A to B copy if it's boot from B\n", __FUNCTION__));
    return FALSE;
  }

  // Get PSP L1
  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Failed to retrieve PSP L1 data\n", __FUNCTION__));
    return FALSE;
  }

  // Check if it's PSP AB scheme
  if (!IsTwoSlotAbRecovery (PspDir)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-It's not A+B scheme\n", __FUNCTION__));
    return TRUE;
  }

  GetSharedB(PspDir, &Slot);

  // Get PSP/BIOS L2 A informations
  ActivePSPLevel2BaseAddress = Slot->SlotAHeader.ImageSlotAddr;
  ActiveSlotIndex = Slot->SlotAIndex;
  AlternativeSlotIndex = Slot->SlotBIndex;

  ISHActiveBase = MASK_SPIADDR_BIT24 (((PspDir->PspEntry)+ActiveSlotIndex)->Location);
  ISHAlernativeBase = MASK_SPIADDR_BIT24 (((PspDir->PspEntry)+AlternativeSlotIndex)->Location);

  if (CheckISHHeaderCorrupted ((UINT32)ISHActiveBase, (UINT32)ISHAlernativeBase) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-% a ISH Headers are same \n", __FUNCTION__));
  }

  if (GetPSPDirectorySize (ActivePSPLevel2BaseAddress, &ActivePSPLevel2Size) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-% a Get Active PSP Directory Size failed \n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-ActiveSlotIndex=%x\n",            __FUNCTION__, ActiveSlotIndex));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-ActivePSPLevel2BaseAddress=%x\n", __FUNCTION__, ActivePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-ActivePSPLevel2Size=%x\n",        __FUNCTION__, ActivePSPLevel2Size));

  APIStatus = GetLevel2PSPEntryInfo (BIOS_REGION_DIR, ActivePSPLevel2BaseAddress, &ActiveBiosLevel2BaseAddress64, &ActiveBiosLevel2BSize);
  if (!APIStatus) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-% a Get Active BIOS Directory Size failed \n", __FUNCTION__));
    return FALSE;
  }

  ActiveBiosLevel2BaseAddress = (UINT32)(ActiveBiosLevel2BaseAddress64 & 0xFFFFFFFF);

  if (GetBIOSDirectorySize (ActiveBiosLevel2BaseAddress, &ActiveBiosLevel2BSize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-% a Get Active BIOS Directory Size failed \n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2-ActiveBiosLevel2BaseAddress=%x\n", __FUNCTION__, ActiveBiosLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2-ActiveBiosLevel2BSize=%x\n",       __FUNCTION__, ActiveBiosLevel2BSize));

  // Get PSP/BIOS L2 B informations
  AlternativePSPLevel2BaseAddress = Slot->SlotBHeader.ImageSlotAddr;
  AlternativePSPLevel2Size = ActivePSPLevel2Size;

  AlternativeBIOSLevel2BaseAddress = AlternativePSPLevel2BaseAddress + AlternativePSPLevel2Size;
  AlternativeBIOSLevel2Size = ActiveBiosLevel2BSize;

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-AlternativeSlotIndex=%x\n",            __FUNCTION__, AlternativeSlotIndex));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-AlternativePSPLevel2BaseAddress=%x\n", __FUNCTION__, AlternativePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-AlternativePSPLevel2Size=%x\n",        __FUNCTION__, AlternativePSPLevel2Size));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2-EstimateBiosLevel2Base=%x\n",        __FUNCTION__, AlternativeBIOSLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2-EstimateBiosLevel2Size=%x\n",        __FUNCTION__, AlternativeBIOSLevel2Size));


  //Allocate Global memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINTN)(ActivePSPLevel2Size + ActiveBiosLevel2BSize)),
             &GlobalBufferAddr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate global buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  // Read PSP L2 from A
  Status = PspReadFlash ((UINTN)ActivePSPLevel2BaseAddress, (UINTN*)&ActivePSPLevel2Size, (UINT8 *)(UINTN)GlobalBufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspReadFlash(PSP L2) Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status))
    return FALSE;

  // Read BIOS L2 from A
  Status = PspReadFlash ((UINTN)ActiveBiosLevel2BaseAddress, (UINTN*)&ActiveBiosLevel2BSize, (UINT8 *)(UINTN)GlobalBufferAddr + ActivePSPLevel2Size);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspReadFlash(BIOS L2) Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status))
    return FALSE;

  // Clean up slot B PSP L2
  Status = PspEraseFlash ((UINTN)AlternativePSPLevel2BaseAddress, (UINTN*)&AlternativePSPLevel2Size);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status))
    return FALSE;

  // Clean up slot B BIOS L2
  Status = PspEraseFlash ((UINTN)AlternativeBIOSLevel2BaseAddress, (UINTN*)&AlternativeBIOSLevel2Size);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status))
    return FALSE;

  // Update PSP L2B
  Status = PspWriteFlash ((UINTN)AlternativePSPLevel2BaseAddress, (UINTN*)&AlternativePSPLevel2Size, (UINT8 *)(UINTN)GlobalBufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash(PSP L2B) Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status))
    return FALSE;

  // Update BIOS L2B
  Status = PspWriteFlash ((UINTN)AlternativeBIOSLevel2BaseAddress, (UINTN*)&AlternativeBIOSLevel2Size, (UINT8 *)(UINTN)GlobalBufferAddr + AlternativePSPLevel2Size);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash(BIOS L2B) Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status))
    return FALSE;

  // Update ISH B
  if (RestoreISHHeader (ActiveSlotIndex, ISHActiveBase, SIZE_4KB, ISHAlernativeBase, SIZE_4KB) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-% a RestoreISHHeader failed \n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Restore Completed, Restart the system\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-EFI_SUCCESS\n", __FUNCTION__));
  (*PeiServices)->ResetSystem ((CONST EFI_PEI_SERVICES **)PeiServices);

  return TRUE;
}


EFI_STATUS
EFIAPI
AmdCpmABRecoveryPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         APIStatus;
  PSP_DIRECTORY                   *PspDir;
  IMAGE_SLOT_INFO                 *Slot;
  UINT32                          CorruptedEntryType;
  UINT32                          CurrentBootPartition;
  UINT8                           CorruptedDirectoryTable;
  UINT8                           CorruptedSubProgram;
  UINT8                           CorruptedInstance;
  UINT8                           CorruptedPartitionNumber;
  UINT64                          ActivePSPLevel2BaseAddress;
  UINT64                          AlternativePSPLevel2BaseAddress;
  UINT64                          SourceEntryAddress;
  UINT32                          SourceEntrySize;
  UINT64                          DestinationEntryAddress;
  UINT32                          DestinationEntrySize;
  UINT64                          TargetDestinationEntryAddress;
  TYPE_ATTRIB                     TypeAttrib;
  RECOVERY_REASON_V2              Reason;
  UINT32                          ReasonSize;
  UINT8                           ActiveSlotIndex;
  UINT8                           AlternativeSlotIndex;
  FIRMWARE_ENTRY_TABLEV2          Efs;
  UINT8                           ResetImageBuffer[0x20];
  EFI_PHYSICAL_ADDRESS            EntrySourceDataAddress;
  UINT64                          Align4kSourceEntryAddress;
  UINT32                          Align4kSourceEntrySize;
  UINT64                          Align4kDestinationEntryAddress;
  UINT32                          Align4kDestinationEntrySize;
  EFI_PHYSICAL_ADDRESS            SourceDataBufferAddr;
  EFI_PHYSICAL_ADDRESS            DestinationDataBufferAddr;

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Start\n", __FUNCTION__));
  ActiveSlotIndex                 = 0;
  AlternativeSlotIndex            = 1;
  ActivePSPLevel2BaseAddress      = 0;
  AlternativePSPLevel2BaseAddress = 0;
  SourceEntryAddress              = 0;
  SourceEntrySize                 = 0;
  DestinationEntryAddress         = 0;
  DestinationEntrySize            = 0;
  Align4kSourceEntryAddress       = 0;
  Align4kSourceEntrySize          = 0;
  Align4kDestinationEntryAddress  = 0;
  Align4kDestinationEntrySize     = 0;
  //Skip Self healing if PcdSkipSelfHealing is set by AGESA PSP driver.
  if (PcdGetBool (PcdSkipSelfHealing)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Self-Healing Skipped\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  //
  // 1.Call PspMboxBiosGetActiveBootPartitionId to know which partition is being executed
  //
  Status = PspMboxBiosGetActiveBootPartitionId (&CurrentBootPartition);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-1-Get Active Boot Partition Fail\n", __FUNCTION__));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-CurrentBootPartition=%x\n", __FUNCTION__, CurrentBootPartition));

  //
  // 2. Call AGESA PSP logging function to know which FW got corrupted
  //
  ReasonSize = sizeof (RECOVERY_REASON_V2);
  Status = GetRecoveryReason ((VOID*)&Reason, &ReasonSize, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-1-Get GetRecoveryReason Fail\n", __FUNCTION__));
    return Status;
  }

  // PSP RECOVERY_REASON_V2 structure modified as per PSP v0.28.A0.69
  CorruptedDirectoryTable  = (UINT8)Reason.DirectoryLevel;
  CorruptedEntryType       = Reason.EntryType;
  CorruptedSubProgram      = (UINT8)((Reason.SubProgramH << 4)|Reason.SubProgramL);
  CorruptedInstance        = (UINT8)Reason.Instance;
  CorruptedPartitionNumber = (UINT8)Reason.PartitionNumber;

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason PSP directory level = %x\n", __FUNCTION__, CorruptedDirectoryTable));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason entry_type = %x\n", __FUNCTION__, CorruptedEntryType));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason SubProgram = %x\n", __FUNCTION__, CorruptedSubProgram));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason Instance info = %x\n", __FUNCTION__, CorruptedInstance));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason Partition number = %x\n", __FUNCTION__, CorruptedPartitionNumber));


  //
  // 3. Get source/ destination entry address and entry size
  //
  if (!GetPspDirBaseV2 (&PspDir)) {
    return EFI_UNSUPPORTED;
  } else {
    if (IsTwoSlotAbRecovery (PspDir)) {
      if (GetImageSlotInfo (PspDir, &Slot)) {
        //TODO: SWAP SRC and DEST Address if needed based on Partiton ID.
        //Assumption now SLOT A  will have the Active Slot Address.
        if ((CurrentBootPartition != CorruptedPartitionNumber) || (CorruptedPartitionNumber == 0)){
          //
          //make B as active partition to boot from
          //
          ActivePSPLevel2BaseAddress = Slot->SlotBHeader.ImageSlotAddr;
          AlternativePSPLevel2BaseAddress = Slot->SlotAHeader.ImageSlotAddr;
          ActiveSlotIndex = Slot->SlotBIndex;
          AlternativeSlotIndex = Slot->SlotAIndex;
        } else {
          ActivePSPLevel2BaseAddress = Slot->SlotAHeader.ImageSlotAddr;
          AlternativePSPLevel2BaseAddress = Slot->SlotBHeader.ImageSlotAddr;
          ActiveSlotIndex = Slot->SlotAIndex;
          AlternativeSlotIndex = Slot->SlotBIndex;
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-ActiveSlotIndex=%x\n", __FUNCTION__, ActiveSlotIndex));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ActivePSPLevel2BaseAddress=%x\n", __FUNCTION__, ActivePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-AlternativeSlotIndex=%x\n", __FUNCTION__, AlternativeSlotIndex));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-AlternativePSPLevel2BaseAddress=%x\n", __FUNCTION__, AlternativePSPLevel2BaseAddress));
  //
  // 3.a Update reason to PSP L2 if needed
  //
  // If get the wrong recovery reason.
  if (CorruptedDirectoryTable > ImageSlotHeader) {
    // Check PSP L2A corrupted or not
    if (!GetPspLv2DirBaseV2 (Slot->SlotAHeader.ImageSlotAddr, &PspDir)) {
      CorruptedDirectoryTable  = PSPL1DirectoryTableEntry;
      CorruptedEntryType       = PSP_REGION_A_DIR;
      CorruptedPartitionNumber = 0;
    }
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason updated PSP directory level = %x\n", __FUNCTION__, CorruptedDirectoryTable));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason updated entry_type = %x\n", __FUNCTION__, CorruptedEntryType));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason updated SubProgram = %x\n", __FUNCTION__, CorruptedSubProgram));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason updated Instance info = %x\n", __FUNCTION__, CorruptedInstance));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason updated Partition number = %x\n", __FUNCTION__, CorruptedPartitionNumber));
  }

  //
  // 4. Prepare flash range
  //

  //
  // 4.a Check PSP L1 Header
  //
  if (CorruptedDirectoryTable == PSPL1DirectoryHeader) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL1 Directory got corrupted=%x\n", __FUNCTION__, CorruptedEntryType));

    if (!GetEfs (&Efs)) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Failed to get PSPL1 Directory \n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }

    DestinationEntryAddress = Efs.PspDirBase;
    SourceEntryAddress      = Efs.PspDirBackupBase;
    SourceEntrySize         = sizeof (PSP_DIRECTORY_HEADER) + ((PspDir->Header.TotalEntries) * sizeof (PSP_DIRECTORY_ENTRY));
    DestinationEntrySize    = SourceEntrySize;

    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL1-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL1-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL1-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL1-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }

  //
  // 4.b Check ISH
  //
  if (CorruptedDirectoryTable == ImageSlotHeader) {

    //For one SOC program only.
    AlternativeSlotIndex    = (ActiveSlotIndex == 1) ? 0 : 1;
    SourceEntryAddress      = MASK_SPIADDR_BIT24 (((PspDir->PspEntry) + ActiveSlotIndex)->Location);
    DestinationEntryAddress = MASK_SPIADDR_BIT24 (((PspDir->PspEntry) + AlternativeSlotIndex)->Location);

    if (CheckISHHeaderCorrupted ((UINT32)SourceEntryAddress,(UINT32)DestinationEntryAddress) != TRUE) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-% a ISH are identical \n", __FUNCTION__));
      return FALSE;
    }

    //ISH A had back up starts from ISH B + 0x100
    SourceEntryAddress  += BACKUP_ISHA_OFFSET;
    SourceEntrySize      = sizeof (IMAGE_SLOT_HEADER_V2) * MAX_IMAGE_SLOT_COUNT;
    DestinationEntrySize = SourceEntrySize;

    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-ISH-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }

  //
  // 4.c Check PSP L1 Entry
  //
  if (CorruptedDirectoryTable == PSPL1DirectoryTableEntry) {

    // TODO: Why we have a PSP L2 pointed by PSP L1 entry and what if there is a other entry under L1?
    if (CorruptedEntryType == PSP_REGION_A_DIR) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2 Directory HEADER got corrupted=%x\n", __FUNCTION__, CorruptedEntryType));
      SourceEntryAddress = ActivePSPLevel2BaseAddress;
      DestinationEntryAddress = AlternativePSPLevel2BaseAddress;
      if (GetPspLv2DirBaseV2 (SourceEntryAddress, &PspDir)) {
        SourceEntrySize = sizeof (PSP_DIRECTORY_HEADER) + ((PspDir->Header.TotalEntries) * sizeof (PSP_DIRECTORY_ENTRY));
        DestinationEntrySize = SourceEntrySize;
      } else {
        return EFI_UNSUPPORTED;
      }
    }
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableHeader-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableHeader-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableHeader-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableHeader-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }



  //
  // 4.d Check PSP DIR L2 Entry.
  //
  if (CorruptedDirectoryTable == PSPL2DirectoryTableEntry) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2 Directory ENTRY got corrupted=%x\n", __FUNCTION__, CorruptedEntryType));

    APIStatus = GetLevel2PSPEntryInfo ((((UINT32)CorruptedSubProgram << 8) + CorruptedEntryType), ActivePSPLevel2BaseAddress, &SourceEntryAddress, &SourceEntrySize);
    if (APIStatus == FALSE) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-3-Source Entry Address/ Size Fail\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }

    APIStatus = GetLevel2PSPEntryInfo ((((UINT32)CorruptedSubProgram << 8) + CorruptedEntryType), AlternativePSPLevel2BaseAddress, &DestinationEntryAddress, &DestinationEntrySize);
    if (APIStatus == FALSE) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-4-Destination Entry Address/ Size Fail\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableEntry-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableEntry-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableEntry-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSPL2DirectoryTableEntry-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }

  //
  // 4.e Check BIOS DIR L2 Entry.
  //
  if (CorruptedDirectoryTable == BIOSL2DirectoryTableEntry) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BIOSL2 Directory Entry got corrupted=%x\n", __FUNCTION__, CorruptedEntryType));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BIOSL2 Directory Entry got corrupted=%x\n", __FUNCTION__, CorruptedInstance));
    if (CorruptedEntryType == PEI_ENTRY) {
      APIStatus = GetLevel2BIOSEntryInfo (
                    CorruptedEntryType,
                    (CorruptedInstance == 0) ? 1 : 0,
                    CorruptedSubProgram,
                    DIR_TYPE_PSP_LV2,
                    ActivePSPLevel2BaseAddress,
                    &TypeAttrib,
                    &SourceEntryAddress,
                    &SourceEntrySize,
                    &TargetDestinationEntryAddress
                    );
    } else {
      APIStatus = GetLevel2BIOSEntryInfo (
                    CorruptedEntryType,
                    CorruptedInstance,
                    CorruptedSubProgram,
                    DIR_TYPE_PSP_LV2,
                    ActivePSPLevel2BaseAddress,
                    &TypeAttrib,
                    &SourceEntryAddress,
                    &SourceEntrySize,
                    &TargetDestinationEntryAddress
                    );
    }
    if (APIStatus == FALSE) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-3-Source BIOS Entry Address/ Size Fail\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }

    APIStatus = GetLevel2BIOSEntryInfo (
                  CorruptedEntryType,
                  CorruptedInstance,
                  CorruptedSubProgram,
                  DIR_TYPE_PSP_LV2,
                  AlternativePSPLevel2BaseAddress,
                  &TypeAttrib,
                  &DestinationEntryAddress,
                  &DestinationEntrySize,
                  &TargetDestinationEntryAddress
                  );
    if (APIStatus == FALSE) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-4-Destination BIOS Entry Address/ Size Fail\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }

    if (CorruptedEntryType == PEI_ENTRY) { //PEI FD Corrupted
        //
        // Check Attribute Compressed bit first
        //
        if (TypeAttrib.Compressed) {
            SourceEntrySize = (UINT32)PcdGet64 (PcdCpmPEICompressSize);
            // PcdCpmPEICompressSize is not set,  standard mode
            if (SourceEntrySize == 0) {
              SourceEntrySize = 0x20;
              if (SourceEntrySize > (sizeof (ResetImageBuffer) / sizeof (UINT8))) {
                return EFI_BUFFER_TOO_SMALL;
              }
              Status = PspReadFlash ((UINTN)SourceEntryAddress, (UINTN*)&SourceEntrySize, (UINT8 *)(UINTN)ResetImageBuffer);
              SourceEntrySize = *((UINT32*)(ResetImageBuffer + 0x14));   // data size, offset 0x14
              SourceEntrySize += 0x100;  // + header size, 0x100
            }
            DestinationEntrySize = SourceEntrySize;
        }
    }
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BIOSL2DirectoryTableEntry-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BIOSL2DirectoryTableEntry-SourceEntrySize=%x \n", __FUNCTION__, SourceEntrySize));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BIOSL2DirectoryTableEntry-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BIOSL2DirectoryTableEntry-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }

  //
  // 5 Start to do self-healing
  //
  //
  // 5.1 Check and make range to SPI 4K aligned.
  //
  Align4kSourceEntryAddress      = SourceEntryAddress;
  Align4kSourceEntrySize         = SourceEntrySize;
  Align4kDestinationEntryAddress = DestinationEntryAddress;
  Align4kDestinationEntrySize    = DestinationEntrySize;

  if (ValidateAddressAndSize (&Align4kSourceEntryAddress, &Align4kSourceEntrySize, &Align4kDestinationEntryAddress, &Align4kDestinationEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-5-Source/ Destination Entry Size are different\n", __FUNCTION__));
    return FALSE;
  }

  //
  // 5.2a Prepare data to write and do some modification.
  //
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINTN)(Align4kSourceEntrySize)),
             &SourceDataBufferAddr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  // TODO: This requied a double resource, may need to check this part.
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINTN)(Align4kDestinationEntrySize)),
             &DestinationDataBufferAddr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  if (MapSpiDataToBuffer ((UINT32)Align4kSourceEntryAddress, (UINT8*)(UINTN)SourceDataBufferAddr, Align4kSourceEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  if (MapSpiDataToBuffer ((UINT32)Align4kDestinationEntryAddress, (UINT8*)(UINTN)DestinationDataBufferAddr, Align4kDestinationEntrySize) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-MapSpiDataToBuffer fail\n", __FUNCTION__));
    return FALSE;
  }

  //
  // Restore data which not belong to source
  //
  (*PeiServices)->CopyMem (
                    (UINT8*)(DestinationDataBufferAddr + (DestinationEntryAddress - Align4kDestinationEntryAddress)),
                    (UINT8*)(SourceDataBufferAddr + (SourceEntryAddress - Align4kSourceEntryAddress)),
                    (UINTN)SourceEntrySize
                    );

  //
  // 5.2b Header Data Original begin
  //
  EntrySourceDataAddress = DestinationDataBufferAddr + (DestinationEntryAddress - Align4kDestinationEntryAddress);

  //Restore L2A PSP Header
  if ((CorruptedDirectoryTable == PSPL1DirectoryTableEntry) && (CorruptedEntryType == PSP_REGION_A_DIR)) {
    UpdateL2APspHeaderBuffer (EntrySourceDataAddress);
  }

  if ((CorruptedDirectoryTable == PSPL2DirectoryTableEntry) && (CorruptedEntryType == BIOS_REGION_DIR)) {
    UpdateL2ABIOSHeaderBuffer (EntrySourceDataAddress);
  }

  //
  // 5.3 Flash update.
  //
  Status = PspEraseFlash ((UINTN)Align4kDestinationEntryAddress, (UINTN*)&Align4kDestinationEntrySize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    (*PeiServices)->FreePages (PeiServices, SourceDataBufferAddr, EFI_SIZE_TO_PAGES ((UINTN)(Align4kSourceEntrySize)));
    (*PeiServices)->FreePages (PeiServices, DestinationDataBufferAddr, EFI_SIZE_TO_PAGES ((UINTN)(Align4kDestinationEntrySize)));
    return Status;
  }

  Status = PspWriteFlash ((UINTN)Align4kDestinationEntryAddress, (UINTN*)&Align4kDestinationEntrySize, (UINT8*)(UINTN)DestinationDataBufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    (*PeiServices)->FreePages (PeiServices, SourceDataBufferAddr, EFI_SIZE_TO_PAGES ((UINTN)(Align4kSourceEntrySize)));
    (*PeiServices)->FreePages (PeiServices, DestinationDataBufferAddr, EFI_SIZE_TO_PAGES ((UINTN)(Align4kDestinationEntrySize)));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Restore Entry Instance %x Completed, Restart the system\n", __FUNCTION__, CorruptedEntryType));
  (*PeiServices)->ResetSystem ((CONST EFI_PEI_SERVICES **)PeiServices);

  return Status;
}

EFI_STATUS
EFIAPI
AmdCpmABRecoveryPeimCZN (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  EFI_STATUS                      Status;
  UINT32                          CorruptedEntryId;
  UINT32                          CurrentBootPartition;
  PSP_DIRECTORY                   *PspDir;
  UINT64                          SourceEntryAddress;
  UINT32                          SourceEntrySize;
  UINT64                          SourceEntryDest;
  UINT64                          DestinationEntryAddress;
  UINT32                          DestinationEntrySize;
  UINT64                          DestinationEntryDest;
  UINT64                          PspRegionAEntryAddress;
  UINT64                          PspRegionBEntryAddress;
  UINT64                          BiosRegionAEntryAddress;
  UINT64                          BiosRegionBEntryAddress;
  RECOVERY_REASON_V1              Reason;
  EFI_PHYSICAL_ADDRESS            BufferAddr;
  UINT32                          PSPdirectorylevel;
  TYPE_ATTRIB                     IgnoredTypeAttrib;
  UINT32                          IgnoredEntrySize;
  UINT8                           InstanceInfo;
  UINT8                           TotalEntries;
  UINT32                          Index;
  EFI_PHYSICAL_ADDRESS            BufferAddrTemp;
  UINT32                          Checksum;
  UINT32                          BIOSPointAddr;
  UINT32                          PEIPointAddr;
  SOC_ID_STRUCT                   SocId;
  AMD_CPM_TABLE_PPI               *CpmTablePpiPtr;
  UINT32                          ReasonSize;

  PspDir = NULL;
  DestinationEntryAddress = 0;
  DestinationEntrySize    = 0;
  BIOSPointAddr           = 0;
  PEIPointAddr            = 0;

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Start\n", __FUNCTION__));
  Status = (*PeiServices)->LocatePpi (
                             (CPM_PEI_SERVICES**)PeiServices,
                             &gAmdCpmTablePpiGuid,
                             0,
                             NULL,
                             (VOID**)&CpmTablePpiPtr
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //1.Call AGESA PSP logging function to know which FW got corrupted
  CurrentBootPartition = 0xFF;
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetReasonBegin);

  ReasonSize = sizeof (RECOVERY_REASON_V1);
  Status = GetRecoveryReason ((VOID*)&Reason, &ReasonSize, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-1-Get GetRecoveryReason Fail\n", __FUNCTION__));
    return Status;
  }
  CorruptedEntryId = Reason.EntryType;
  PSPdirectorylevel = Reason.DirectoryLevel;
  InstanceInfo = (UINT8)Reason.Instance;
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason PSP directory level = %x\n", __FUNCTION__, (Reason.DirectoryLevel)));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason entry_type = %x\n", __FUNCTION__, (Reason.EntryType)));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason Instance info = %x\n", __FUNCTION__, (Reason.Instance)));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason Partition number = %x\n", __FUNCTION__, (Reason.PartitionNumber)));
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetReasonEnd);

  //2.Call PspMboxBiosGetActiveBootPartitionId to know which partition is being executed
  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    return EFI_UNSUPPORTED;
  }
  if ((NULL == PspDir) || (PspDir->Header.TotalEntries == 0)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PspDir not retreived \n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return FALSE;
  }

  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetPartitionBegin);
  if (IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress)) {
    DEBUG ((DEBUG_INFO, "PspRegionAEntryAddress=%x\n", PspRegionAEntryAddress));
    DEBUG ((DEBUG_INFO, "PspRegionBEntryAddress=%x\n", PspRegionBEntryAddress));
    if (CheckPspRecoveryFlagV2 () == FALSE) {
      CurrentBootPartition = 1;
    } else {
      CurrentBootPartition = 0;
    }
  } else {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-Not AB Recovery EFI_UNSUPPORTED\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }
  GetLevel2PSPEntryInfo (BIOS_REGION_DIR, PspRegionBEntryAddress, &BiosRegionBEntryAddress, &IgnoredEntrySize);
  GetLevel2PSPEntryInfo (BIOS_REGION_DIR, PspRegionAEntryAddress, &BiosRegionAEntryAddress, &IgnoredEntrySize);
  DEBUG ((DEBUG_INFO, "BiosRegionAEntryAddress=%x\n", BiosRegionAEntryAddress));
  DEBUG ((DEBUG_INFO, "BiosRegionBEntryAddress=%x\n", BiosRegionBEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-CurrentBootPartition=%x\n", __FUNCTION__, CurrentBootPartition));
  if ((CurrentBootPartition) > 1) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-2-Incorret Boot Partition\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  //3.Get source/ destination entry address and entry size
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-PSP Directory got corrupted=%x\n", __FUNCTION__, CorruptedEntryId));
  if (CorruptedEntryId == PSP_REGION_A_DIR) {
    if (PSPEntryInfoV2 (PSP_REGION_B_DIR, &SourceEntryAddress, &SourceEntrySize)) {
      DestinationEntrySize = SourceEntrySize;
      DestinationEntryAddress = PspRegionAEntryAddress;
    } else {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-Get source destination EFI_UNSUPPORTED\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
  } else if (CorruptedEntryId == BIOS_FIRMWARE) {
    // Get the BIOS Firmware Binary Address from L2B, if not suessfully, change to get the address from L2A
    //  1). Try to get the source Binary Address via the another Instace ID
    //  2). Get the destination Binary Address via the broken Intance ID
    if (GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2,\
     BiosRegionBEntryAddress, &IgnoredTypeAttrib, &SourceEntryAddress, &SourceEntrySize, &SourceEntryDest)) {
      GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2,\
      BiosRegionBEntryAddress, &IgnoredTypeAttrib, &DestinationEntryAddress, &DestinationEntrySize, \
       &DestinationEntryDest);
    } else if (GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2,
     BiosRegionAEntryAddress, &IgnoredTypeAttrib, &SourceEntryAddress, &SourceEntrySize, &SourceEntryDest)) {
      GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2,
      BiosRegionAEntryAddress, &IgnoredTypeAttrib, &DestinationEntryAddress, &DestinationEntrySize, \
      &DestinationEntryDest);
    } else {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-Get source destination EFI_UNSUPPORTED\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
  } else {
    if (GetLevel2PSPEntryInfo (CorruptedEntryId, PspRegionBEntryAddress, &SourceEntryAddress, &SourceEntrySize)) {
      GetLevel2PSPEntryInfo (CorruptedEntryId, PspRegionAEntryAddress, &DestinationEntryAddress,\
       &DestinationEntrySize);
      DEBUG ((DEBUG_INFO, "PspRegionAEntryAddress=%x\n", PspRegionAEntryAddress));
      DEBUG ((DEBUG_INFO, "PspRegionBEntryAddress=%x\n", PspRegionBEntryAddress));
    } else if (GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2,
     BiosRegionBEntryAddress, &IgnoredTypeAttrib, &SourceEntryAddress, &SourceEntrySize, &SourceEntryDest)) {
      GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2,
      BiosRegionAEntryAddress, &IgnoredTypeAttrib, &DestinationEntryAddress, &DestinationEntrySize, \
      &DestinationEntryDest);
    } else {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-Get source destination EFI_UNSUPPORTED\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
  }
  //Compressed Reset Image
  if (CorruptedEntryId == BIOS_FIRMWARE) {
    SourceEntrySize = (UINT32)PcdGet64(PcdCpmPEICompressSize);
    DestinationEntrySize = SourceEntrySize;
  }
  if (SourceEntrySize != DestinationEntrySize) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-5-Source/ Destination Entry Size are different\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));

  if (!ALIGN_4K_CHECK (SourceEntryAddress)) {
    SourceEntrySize += (UINT64)(SourceEntryAddress & 0xFFF);
    SourceEntryAddress &= (UINT64)(~((UINT64)0xFFF));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  }
  if (!ALIGN_4K_CHECK (DestinationEntryAddress)) {
    DestinationEntrySize += (UINT64)(DestinationEntryAddress & 0xFFF);
    DestinationEntryAddress &= (UINT64)(~((UINT64)0xFFF));
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  }
  if (!ALIGN_4K_CHECK (SourceEntrySize)) {
    SourceEntrySize &= (UINT64)(~((UINT64)0xFFF));
    SourceEntrySize += (UINT64)(0x1000);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  }
  if (!ALIGN_4K_CHECK (DestinationEntrySize)) {
    DestinationEntrySize &= (UINT64)(~((UINT64)0xFFF));
    DestinationEntrySize += (UINT64)(0x1000);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetPartitionEnd);

  //4.Do self-healing
  //source entry size should be same as destination entry size
  if (SourceEntrySize != DestinationEntrySize) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-5-Source/ Destination Entry Size are different\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimSelfHealingBegin);
  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)SourceEntrySize),
             &BufferAddr
             );
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer Status %r BufferAddr 0x%x\n", __FUNCTION__, Status, BufferAddr));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer fail\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if (SourceEntrySize > UINT_MAX ) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-6-read size incorrect \n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Status = PspReadFlash ((UINTN)SourceEntryAddress, (UINTN*)&SourceEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspReadFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  //Restore entry instance SPI region
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));

  SocId.SocFamilyID = F19_CZN_RAW_ID;
  SocId.PackageType = ZEN3_FP6;
  if (SocHardwareIdentificationCheckV2 (&SocId)) {
    BIOSPointAddr = (UINT32)PcdGet64(PcdCpmBIOSPointAddressCZN);
    PEIPointAddr = (UINT32)PcdGet64(PcdCpmPEIPointAddressCZN);
  } else {
    SocId.SocFamilyID = F17_LCN_RAW_ID;
    SocId.PackageType = RN_FP6;
    if (SocHardwareIdentificationCheckV2 (&SocId)) {
      BIOSPointAddr = (UINT32)PcdGet64(PcdCpmBIOSPointAddressLCN);
      PEIPointAddr = (UINT32)PcdGet64(PcdCpmPEIPointAddressLCN);
    }
  }
  //Restore L2A PSP Header
  if ((DestinationEntryAddress == PspRegionAEntryAddress) || (CorruptedEntryId == PSP_REGION_A_DIR)) {
    TotalEntries = *(UINT8*)(UINTN)(BufferAddr + 0x8);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP TotalEntries=%x\n", __FUNCTION__, TotalEntries));
    BufferAddrTemp = BufferAddr + sizeof (PSP_DIRECTORY_HEADER);
    for (Index = 0; Index < TotalEntries; Index++) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP BufferAddrTemp=%x\n", __FUNCTION__, BufferAddrTemp));
      if (*(UINT8*)(UINTN)BufferAddrTemp == BIOS_REGION_DIR) {
        *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = BIOSPointAddr;
        DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS_REGION_DIR Point Addr=%x\n", __FUNCTION__, (*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
      }
      if ((*(UINT8*)(UINTN)BufferAddrTemp == PSP_NV_DATA) && (IS_ADDRESS_MODE_2 (*(UINT64*)(UINTN)(BufferAddrTemp + 0x8)))) {
      *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) + (UINT32)PspRegionBEntryAddress - (UINT32)PspRegionAEntryAddress;
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP_NV_DATA Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
      }
      if ((*(UINT8*)(UINTN)BufferAddrTemp == PSP_NVRAM) && (IS_ADDRESS_MODE_2 (*(UINT64*)(UINTN)(BufferAddrTemp + 0x8)))) {
      *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) + (UINT32)PspRegionBEntryAddress - (UINT32)PspRegionAEntryAddress;
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A PSP_NVRAM Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)(BufferAddrTemp + 8))));
      }
      BufferAddrTemp += sizeof (PSP_DIRECTORY_ENTRY);
    }
    BufferAddrTemp = BufferAddr + 0x8;
    Checksum = Fletcher32 ((UINT16*)(UINTN)BufferAddrTemp, ((TotalEntries * 0x10 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
    BufferAddrTemp = BufferAddr + 0x4;
    *(UINT32*)(UINTN)(BufferAddrTemp ) = Checksum;
    Checksum = Fletcher32 ((UINT16*)(UINTN)(PspRegionBEntryAddress+8), ((TotalEntries * 0x10 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
  }
  //Restore L2A BIOS Header
  if ((DestinationEntryAddress == BiosRegionAEntryAddress) || (CorruptedEntryId == BIOS_REGION_DIR)) {
    TotalEntries = *(UINT8*)(UINTN)(BufferAddr + 0x8);
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A Bios TotalEntries=%x\n", __FUNCTION__, TotalEntries));
    BufferAddrTemp = BufferAddr + sizeof (PSP_DIRECTORY_HEADER);
    for (Index = 0; Index < TotalEntries; Index++) {
      DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BufferAddrTemp=%x\n", __FUNCTION__, BufferAddrTemp));
      if (*(UINT8*)(UINTN)BufferAddrTemp == 0x62) {
        *(UINT32*)(UINTN)(BufferAddrTemp + 0x8) = PEIPointAddr;
        DEBUG ((DEBUG_INFO, "FEA-PEI-%a-L2A BIOS PEI Point Addr=%x\n", __FUNCTION__, (UINT32)(*(UINT32*)(UINTN)BufferAddrTemp)));
      }
      BufferAddrTemp += sizeof (BIOS_DIRECTORY_ENTRY);
    }
    BufferAddrTemp = BufferAddr + 0x8;
    Checksum = Fletcher32 ((UINT16*)(UINTN)BufferAddrTemp, ((TotalEntries * 0x18 + 8) / 2));
    DEBUG ((EFI_D_ERROR, "FEA-PEI-%a Checksum %x\n", __FUNCTION__, Checksum));
    BufferAddrTemp = BufferAddr + 0x4;
    *(UINT32*)(UINTN)(BufferAddrTemp) = Checksum;
  }
  Status = PspEraseFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));
  Status = PspWriteFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimSelfHealingEnd);

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Restore Entry Instance %x Completed, Restart the system\n", __FUNCTION__, CorruptedEntryId));

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-EFI_SUCCESS\n", __FUNCTION__));
  return (*PeiServices)->ResetSystem ((CONST EFI_PEI_SERVICES **)PeiServices);
}

EFI_STATUS
EFIAPI
AmdCpmABRecoveryEntryPoint (
  IN        CPM_PEI_FILE_HANDLE         FileHandle,
  IN        CPM_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PPI               *CpmTablePpiPtr;
  UINT32                          CurrentBootPartition;
  UINT8                           Index;
  RECOVERY_REASON_V2              Reason;
  UINT32                          ReasonSize;

  //Call PspMboxBiosGetActiveBootPartitionId to know which partition is being executed
  Status = PspMboxBiosGetActiveBootPartitionId (&CurrentBootPartition);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-1-Get Active Boot Partition Fail\n", __FUNCTION__));
    return FALSE;
  }
  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-CurrentBootPartition=%x\n", __FUNCTION__, CurrentBootPartition));

  //Skip Self healing if PcdSkipSelfHealing is set by AGESA PSP driver.
  if (PcdGetBool (PcdSkipSelfHealing)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-Self-Healing Skipped\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }


  //Call AGESA PSP logging function to know which FW got corrupted first
  ReasonSize = sizeof (RECOVERY_REASON_V2);
  Status = GetRecoveryReason ((VOID*)&Reason, &ReasonSize, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-1-Get GetRecoveryReason Fail\n", __FUNCTION__));
    return Status;
  }
  //if ISH-A corrupted, do not share A to B
  if ((Reason.DirectoryLevel < PSPL1DirectoryTableEntry) && (Reason.DirectoryLevel > ImageSlotHeader)) {
    //Check for SharedB update.
    UpdateL2BFromL2A( PeiServices );
  }

  Status = (*PeiServices)->LocatePpi (
                             (CPM_PEI_SERVICES**)PeiServices,
                             &gAmdCpmTablePpiGuid,
                             0,
                             NULL,
                             (VOID**)&CpmTablePpiPtr
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimEntryBegin);

  for (Index = 0; Index < ARRAY_SIZE (mSocId); Index++) {
    if (SocHardwareIdentificationCheck (&mSocId[Index])) {
      break;
    }
  }

  if (Index != ARRAY_SIZE (mSocId)) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmABRecoveryNotifyCZN);
  } else {
    DEBUG ((DEBUG_INFO, "FEA-PEI-%a-register-notify\n", __FUNCTION__));
    Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmABRecoveryNotify);
  }

  DEBUG ((DEBUG_INFO, "FEA-PEI-%a-End-EFI_SUCCESS\n", __FUNCTION__));
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimEntryEnd);

  return Status;
}
