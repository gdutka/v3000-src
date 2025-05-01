/***************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/AmdPspFlashAccLib.h>

#define PSPDirectoryTableEntry    0
#define BIOSDirectoryTableEntry   1

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
  AmdCpmABRecoveryPeimCZN
};

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
  AMD_CPM_TABLE_PPI               *CpmTablePpiPtr;
  UINT32                          ReasonSize;
  UINT8                           ResetImageBuffer[0x20];

  DEBUG((DEBUG_INFO, "FEA-PEI-%a-Start\n", __FUNCTION__));
  Status = (*PeiServices)->LocatePpi (
                             (CPM_PEI_SERVICES**)PeiServices,
                             &gAmdCpmTablePpiGuid,
                             0,
                             NULL,
                             (VOID**)&CpmTablePpiPtr
                             );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //1.Call AGESA PSP logging function to know which FW got corrupted
  CurrentBootPartition = 0xFF;
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetReasonBegin);

  ReasonSize = sizeof (RECOVERY_REASON_V1);
  Status = GetRecoveryReason ((VOID*)&Reason, &ReasonSize, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-1-Get GetRecoveryReason Fail\n", __FUNCTION__));
    return Status;
  }
  CorruptedEntryId = Reason.EntryType;
  PSPdirectorylevel = Reason.DirectoryLevel;
  InstanceInfo = (UINT8)Reason.Instance;
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason PSP directory level = %x\n", __FUNCTION__, (Reason.DirectoryLevel)));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason entry_type = %x\n", __FUNCTION__, (Reason.EntryType)));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason Instance info = %x\n", __FUNCTION__, (Reason.Instance)));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-RecoveryReason Partition number = %x\n", __FUNCTION__, (Reason.PartitionNumber)));
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetReasonEnd);

  //2.Call PspMboxBiosGetActiveBootPartitionId to know which partition is being executed
  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    return EFI_UNSUPPORTED;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetPartitionBegin);
  if (IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress)) {
    DEBUG((DEBUG_INFO, "PspRegionAEntryAddress=%x\n", PspRegionAEntryAddress));
    DEBUG((DEBUG_INFO, "PspRegionBEntryAddress=%x\n", PspRegionBEntryAddress));
    if (CheckPspRecoveryFlagV2 () == FALSE) {
      CurrentBootPartition = 1;
    } else {
      CurrentBootPartition = 0;
    }
  } else {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-Not AB Recovery EFI_UNSUPPORTED\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }
  GetLevel2PSPEntryInfo (BIOS_REGION_DIR, PspRegionBEntryAddress, &BiosRegionBEntryAddress, &IgnoredEntrySize);
  GetLevel2PSPEntryInfo (BIOS_REGION_DIR, PspRegionAEntryAddress, &BiosRegionAEntryAddress, &IgnoredEntrySize);
  DEBUG((DEBUG_INFO, "BiosRegionAEntryAddress=%x\n", BiosRegionAEntryAddress));
  DEBUG((DEBUG_INFO, "BiosRegionBEntryAddress=%x\n", BiosRegionBEntryAddress));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-CurrentBootPartition=%x\n", __FUNCTION__, CurrentBootPartition));
  if ((CurrentBootPartition) > 1) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-2-Incorret Boot Partition\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  //3.Get source/ destination entry address and entry size
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-PSP Directory got corrupted=%x\n", __FUNCTION__, CorruptedEntryId));
  if (CorruptedEntryId == PSP_REGION_A_DIR) {
    if (PSPEntryInfoV2 (PSP_REGION_B_DIR, &SourceEntryAddress, &SourceEntrySize)) {
      DestinationEntrySize = SourceEntrySize;
      DestinationEntryAddress = PspRegionAEntryAddress;
    } else {
      DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-Get source destination EFI_UNSUPPORTED\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
  } else if (CorruptedEntryId == BIOS_FIRMWARE) {
    // Get the BIOS Firmware Binary Address from L2B, if not suessfully, change to get the address from L2A
    //  1). Try to get the source Binary Address via the another Instace ID
    //  2). Get the destination Binary Address via the broken Intance ID
    if (GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo^1, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2, BiosRegionBEntryAddress, &IgnoredTypeAttrib, &SourceEntryAddress, &SourceEntrySize, &SourceEntryDest)) {
      GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2, BiosRegionBEntryAddress, &IgnoredTypeAttrib, &DestinationEntryAddress, &DestinationEntrySize, &DestinationEntryDest);
    } else if (GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo^1, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2, BiosRegionAEntryAddress, &IgnoredTypeAttrib, &SourceEntryAddress, &SourceEntrySize, &SourceEntryDest)) {
      GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2, BiosRegionAEntryAddress, &IgnoredTypeAttrib, &DestinationEntryAddress, &DestinationEntrySize, &DestinationEntryDest);
    } else {
      DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-Get source destination EFI_UNSUPPORTED\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
  } else {
    if (GetLevel2PSPEntryInfo (CorruptedEntryId, PspRegionBEntryAddress, &SourceEntryAddress, &SourceEntrySize)) {
      GetLevel2PSPEntryInfo (CorruptedEntryId, PspRegionAEntryAddress, &DestinationEntryAddress, &DestinationEntrySize);
      DEBUG((DEBUG_INFO, "PspRegionAEntryAddress=%x\n", PspRegionAEntryAddress));
      DEBUG((DEBUG_INFO, "PspRegionBEntryAddress=%x\n", PspRegionBEntryAddress));
    } else if (GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2, BiosRegionBEntryAddress, &IgnoredTypeAttrib, &SourceEntryAddress, &SourceEntrySize, &SourceEntryDest)) {
      GetLevel2BIOSEntryInfo (CorruptedEntryId, InstanceInfo, SUBPROGRAM_IGNORED, DIR_TYPE_BIOS_LV2, BiosRegionAEntryAddress, &IgnoredTypeAttrib, &DestinationEntryAddress, &DestinationEntrySize, &DestinationEntryDest);
    } else {
      DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-Get source destination EFI_UNSUPPORTED\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
  }
  //Compressed Reset Image
  if (CorruptedEntryId == BIOS_FIRMWARE) {
    SourceEntrySize = 0x20;
    Status = PspReadFlash ((UINTN)SourceEntryAddress, (UINTN*)&SourceEntrySize, (UINT8 *)(UINTN)ResetImageBuffer);
    SourceEntrySize = *((UINT32*)(ResetImageBuffer + 0x14));   // data size, offset 0x14
    SourceEntrySize += 0x100;  // + header size, 0x100
    DestinationEntrySize = SourceEntrySize;
  }
  if (SourceEntrySize != DestinationEntrySize) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-5-Source/ Destination Entry Size are different\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  DEBUG((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));

  if (!ALIGN_4K_CHECK (SourceEntryAddress)) {
    SourceEntrySize += (UINT64)(SourceEntryAddress & 0xFFF);
    SourceEntryAddress &= (UINT64)(~((UINT64)0xFFF));
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, SourceEntryAddress));
  }
  if (!ALIGN_4K_CHECK (DestinationEntryAddress)) {
    DestinationEntrySize += (UINT64)(DestinationEntryAddress & 0xFFF);
    DestinationEntryAddress &= (UINT64)(~((UINT64)0xFFF));
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  }
  if (!ALIGN_4K_CHECK (SourceEntrySize)) {
    SourceEntrySize &= (UINT64)(~((UINT64)0xFFF));
    SourceEntrySize += (UINT64)(0x1000);
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  }
  if (!ALIGN_4K_CHECK (DestinationEntrySize)) {
    DestinationEntrySize &= (UINT64)(~((UINT64)0xFFF));
    DestinationEntrySize += (UINT64)(0x1000);
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimGetPartitionEnd);

  //4.Do self-healing
  //source entry size should be same as destination entry size
  if (SourceEntrySize != DestinationEntrySize) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-5-Source/ Destination Entry Size are different\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimSelfHealingBegin);
  //Allocate temp memory to hold backup
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)SourceEntrySize),
             &BufferAddr
             );
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer Status %r BufferAddr 0x%x\n", __FUNCTION__, Status, BufferAddr));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, SourceEntrySize));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));

  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-6-Allocate temp buffer fail\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }
  Status = PspReadFlash ((UINTN)SourceEntryAddress, (UINTN*)&SourceEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspReadFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  //Restore entry instance SPI region
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));

  Status = PspEraseFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspEraseFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, DestinationEntryAddress));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, DestinationEntrySize));
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-BufferAddr 0x%x\n", __FUNCTION__, BufferAddr));
  Status = PspWriteFlash ((UINTN)DestinationEntryAddress, (UINTN*)&DestinationEntrySize, (UINT8 *)(UINTN)BufferAddr);
  DEBUG ((EFI_D_ERROR, "FEA-PEI-%a PspWriteFlash Status %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimSelfHealingEnd);

  DEBUG((DEBUG_INFO, "FEA-PEI-%a-Restore Entry Instance %x Completed, Restart the system\n", __FUNCTION__, CorruptedEntryId));

  DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-EFI_SUCCESS\n", __FUNCTION__));
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

  //Skip Self healing if PcdSkipSelfHealing is set by AGESA PSP driver.
  if (PcdGetBool (PcdSkipSelfHealing)) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-Self-Healing Skipped\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Status = (*PeiServices)->LocatePpi (
                             (CPM_PEI_SERVICES**)PeiServices,
                             &gAmdCpmTablePpiGuid,
                             0,
                             NULL,
                             (VOID**)&CpmTablePpiPtr
                             );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimEntryBegin);
  if (SocFamilyIdentificationCheck (F17_RN_RAW_ID) || 
      SocFamilyIdentificationCheck (F17_LCN_RAW_ID) ||
      SocFamilyIdentificationCheck (F19_CZN_RAW_ID) ) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmABRecoveryNotify);
  }

  DEBUG((DEBUG_INFO, "FEA-PEI-%a-End-EFI_SUCCESS\n", __FUNCTION__));
  CpmTablePpiPtr->CommonFunction.PostCode (CpmTpABRecoveryPeimEntryEnd);

  return Status;
}
