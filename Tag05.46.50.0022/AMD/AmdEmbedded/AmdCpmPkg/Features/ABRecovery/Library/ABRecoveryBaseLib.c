/*
 ******************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/PciLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/IdsLib.h>
#include <Library/FchBaseLib.h>
#include <Library/FchSpiAccessLib.h>
#include <Features/ABRecovery/Include/ABRecoveryBaseLib.h>
#include <Library/MemoryAllocationLib.h>

#define FILECODE LIBRARY_AMDPSPBASELIBV2_AMDPSPBASELIBV2_FILECODE

/// Table to store all supported program directory infomation
DIR_INFO DirectoryTbl[] = {
  {F19_RMB_RAW_ID, RMB_PSP_CHIP_ID, BIOS_DIR_EFS_OFFSET_IGNORE,                        FALSE, ISH_VERSION_2,      RECOVERY_REASON_VERSION_2},
  {F19_VMR_RAW_ID, VMR_PSP_CHIP_ID, OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, SspBiosDirBase), FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_1},
  {F19_CZN_RAW_ID, CZN_PSP_CHIP_ID, OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase), TRUE,  ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_1},
  {F19_BA_RAW_ID,  BA_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase), FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F19_GN_RAW_ID,  GN_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase), FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F17_VN_RAW_ID,  VN_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                        TRUE,  ISH_VERSION_1,      RECOVERY_REASON_VERSION_2},
  {F17_MR_RAW_ID,  MR_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                        TRUE,  ISH_VERSION_1,      RECOVERY_REASON_VERSION_2},
  {F17_RN_RAW_ID,  RN_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase), TRUE,  ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_1},
  {F17_ZP_RAW_ID,  ZP_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, ZpBiosDirBase),  FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F17_RV_RAW_ID,  RV_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, RvBiosDirBase),  FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F17_PIC_RAW_ID, RV_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, RvBiosDirBase),  FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F17_RV2_RAW_ID, RV2_PSP_CHIP_ID, OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, RvBiosDirBase),  FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F17_MTS_RAW_ID, MTS_PSP_CHIP_ID, OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, SspBiosDirBase), FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F17_SSP_RAW_ID, SSP_PSP_CHIP_ID, OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, SspBiosDirBase), FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
  {F19_RS_RAW_ID,  RS_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase), FALSE, ISH_VERSION_IGNORE, RECOVERY_REASON_VERSION_IGNORE},
};

/**
  Get DIR_INFO information according RAW_FAMILY_ID from DirectoryTbl, return NULL if not found

  @retval return NULL if not found

**/
DIR_INFO *
FetchDirInfoEntry (
  VOID
  )
{
  UINT32 i;
  DIR_INFO *DirInfoTblEntry;

  DirInfoTblEntry = NULL;
  for (i = 0; i < (sizeof (DirectoryTbl)/ sizeof (DIR_INFO)); i++) {
    if (SocFamilyIdentificationCheck (DirectoryTbl[i].FamilyRawId)) {
      DirInfoTblEntry = &DirectoryTbl[i];
      break;
    }
  }
  return DirInfoTblEntry;
}

/**
  This function is to calculate the crc checksum

  @param[in]  pointer to content
  @param[in]  length of content

  @retval CRC value

**/
UINT32
CalculateChecksum (
  IN OUT   UINT16  *data,
  IN       UINTN   words
  )
{
  UINT32 sum1;
  UINT32 sum2;
  UINTN tlen;

  sum1 = 0xffff;
  sum2 = 0xffff;

  while (words) {
    tlen = words >= 359 ? 359 : words;
    words -= tlen;
    do {
      sum2 += sum1 += *data++;
    } while (--tlen);
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  }
  // Second reduction step to reduce sums to 16 bits
  sum1 = (sum1 & 0xffff) + (sum1 >> 16);
  sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  return sum2 << 16 | sum1;
}


BOOLEAN
AllocateHeapBufferByHandle (
  IN      UINT32  BufferHandle,                    ///< An unique ID of buffer.
  IN      UINT32  BufferSize,                      ///< Data buffer size.
  IN OUT  UINT8   **BufferPtr                       ///< Pointer to buffer.
  )
{
    *BufferPtr = AllocateRuntimePool (BufferSize);
    if(*BufferPtr == NULL)
        return FALSE;

    return TRUE;
}

BOOLEAN
CheckRom2Decoded (
  IN       UINT64        Offset
  )
{
  return FALSE;
}

BOOLEAN
CheckPspDirAddrRom2Decoded (
  IN      UINT64                      EntryLocation
  )
{
  if (CheckRom2Decoded (EntryLocation)) {
      return TRUE;
  } else {
      return FALSE;
  }
}

UINT64
TryToConvertOffsetToPhysicalAddress (
  IN      UINT64                      EntryLocation
  )
{
 if (IS_SPI_OFFSET (EntryLocation)) {
      if (CheckPspDirAddrRom2Decoded (EntryLocation)) {
         EntryLocation = FORCE_SPIADDR_BIT24 (EntryLocation);
      }
  }
  return EntryLocation;
}

UINT64
ConvertEntryLocation (
  IN       UINT64                      EntryLocation,
  IN       UINT64                      DirectoryHdrAddr,
  IN       UINT32                      ImageSlotAddr    //if no image slot header, just leave it as 0
  )
{
  //address mode 1, relative to BIOS image base
  if (IS_ADDRESS_MODE_1 (EntryLocation)) {
      EntryLocation = EntryLocation & ~BIT62;
  } else if (IS_ADDRESS_MODE_2 (EntryLocation)) { //address mode 2, relative to current directory header
      EntryLocation = EntryLocation & ~BIT63;
      //get the relative offset compare to directory header
      EntryLocation += DirectoryHdrAddr;
  } else if (IS_ADDRESS_MODE_3 (EntryLocation)) { //address mode 3, relative to current image slot
      EntryLocation = EntryLocation & ~(BIT63 | BIT62);
      //get the relative offset compare to L2
      EntryLocation += ImageSlotAddr;
  }
  return TryToConvertOffsetToPhysicalAddress (EntryLocation);
}

/**
  This function checks if the PSP L1 is multi slot layout

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has multi slots (>=2)
  @retval FALSE            the PSP L1 directory has not multi slots (>=2)

**/
BOOLEAN
VerifyMultiSlotLayout (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  )
{
  UINT32               i;
  UINT32               RecoveryEntryCount = 0;
  BOOLEAN              RecoveryEntryExists = FALSE;

  ASSERT (PspL1Dir != NULL);

  for (i = 0; i < PspL1Dir->Header.TotalEntries; i++) {
    if ((PspL1Dir->PspEntry[i].Type.Field.Type == PSP_REGION_A_DIR) || (PspL1Dir->PspEntry[i].Type.Field.Type == PSP_REGION_B_DIR)) {
       if (!RecoveryEntryExists) {
         RecoveryEntryExists = TRUE;
       }
       RecoveryEntryCount++;
    }
  }

  //recovery exists and all entries are recovery entries
  if (RecoveryEntryExists && (RecoveryEntryCount == PspL1Dir->Header.TotalEntries)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function checks if the PSP L1 is two slot A/B recovery layout (VN)

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has 2 slots with A/B recovery
  @retval FALSE            the PSP L1 directory doesn't have 2 slots with A/B recovery

**/
BOOLEAN
VerifyTwoSlotAbRecovery (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  )
{
  UINT32               i;
  UINT32               RecoveryEntryCount = 0;
  BOOLEAN              RegionBEntryExists = FALSE;

  ASSERT (PspL1Dir != NULL);

  for (i = 0; i < PspL1Dir->Header.TotalEntries; i++) {
    if ((PspL1Dir->PspEntry[i].Type.Field.Type == PSP_REGION_A_DIR) || (PspL1Dir->PspEntry[i].Type.Field.Type == PSP_REGION_B_DIR)) {
      RecoveryEntryCount++;
      if (!RegionBEntryExists && (PspL1Dir->PspEntry[i].Type.Field.Type == PSP_REGION_B_DIR)) {
        RegionBEntryExists = TRUE;
      }
    }
  }

  //recovery B exists and all entries are recovery entries
  if (RegionBEntryExists && (RecoveryEntryCount == PspL1Dir->Header.TotalEntries)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
FetchImageSlotInfo (
  IN PSP_DIRECTORY       *PspL1Dir,
  IN OUT IMAGE_SLOT_INFO **Slot,
  IN EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  )
{
  IMAGE_SLOT_HEADER   ImageSlotEntry;
  IMAGE_SLOT_HEADER_V2 ImageSlotEntryV2;
  UINT8               Index = 0;
  UINT8               *ImageSlotBuffer = NULL;
  IMAGE_SLOT_INFO     *TempSlot = NULL;
  UINT64              TempImageSlotAddr = 0;
  DIR_INFO            *DirInfo;
  UINT32              PspChipId;
  UINT32              IshStructVersion;
  UINT8               Type = 0;

  ASSERT (PspL1Dir != NULL);
  if (AllocateHeapBufferByHandle (AMD_PSP_IMAGE_SLOT_BUFFER_HANDLE, MaxImageSlotInfoSize, &ImageSlotBuffer) != TRUE) {
     return FALSE;
  }

  DirInfo = FetchDirInfoEntry ();
  if (DirInfo == NULL) {
    //Unsupported Family detected
    ASSERT (FALSE);
    return (FALSE);
  }
  PspChipId = DirInfo->PspId;
  IshStructVersion = DirInfo->IshStructVersion;

  TempSlot = (IMAGE_SLOT_INFO *) (UINTN)ImageSlotBuffer;
  TempSlot->SlotCount = 0;
  TempSlot->BootableSlotCount = 0;
  TempSlot->UnbootableSlotCount = 0;

  TempSlot->SlotAIndex = 0xFF;
  TempSlot->SlotAHeader.Priority = IMAGE_SLOT_PRIORITY_UNBOOTABLE;
  TempSlot->SlotAHeader.UpdateRetries = 0;
  TempSlot->SlotAHeader.GlitchRetries = 0;
  TempSlot->SlotAHeader.ImageSlotAddr = 0;

  TempSlot->SlotBIndex = 0xFF;
  TempSlot->SlotBHeader.Priority = IMAGE_SLOT_PRIORITY_UNBOOTABLE;
  TempSlot->SlotBHeader.UpdateRetries = 0;
  TempSlot->SlotBHeader.GlitchRetries = 0;
  TempSlot->SlotBHeader.ImageSlotAddr = 0;

  //
  // All the valid entry in PSP L1 directory MUST be continous.
  // Invalid entry in the middle of valid entries is not supported.
  //
  for (Index = 0; Index < PspL1Dir->Header.TotalEntries; Index++) {
    if ((PspL1Dir->PspEntry[Index].Type.Field.Type == PSP_REGION_A_DIR)
        || (PspL1Dir->PspEntry[Index].Type.Field.Type == PSP_REGION_B_DIR)) {
        TempSlot->SlotCount++;

        Type = (UINT8)PspL1Dir->PspEntry[Index].Type.Field.Type;

        if (IshStructVersion == ISH_VERSION_2) {
          //ISH version 2 detected, start from this version
          // 1. Add CRC checksum
          // 2. Add PSPID to support combo, w/o combo directory
          //Assume entry LOCATION in PSP L1 uses address mode 0 or 1,
          //if address mode 2 is found, we cannot use such method to get image slot address
          TempImageSlotAddr = TryToConvertOffsetToPhysicalAddress (PspL1Dir->PspEntry[Index].Location);

          TempImageSlotAddr = (UINT32)(TempImageSlotAddr & 0xFFFFFFFF);
          TempImageSlotAddr = TempImageSlotAddr + BiosBaseAddr;

          CopyMem((UINT8 *)&ImageSlotEntryV2, (VOID *)(UINTN)(TempImageSlotAddr), sizeof (ImageSlotEntryV2));

          //Todo: Do the checksum check
          //To make the logical generic for all program, transfer ImageSlotEntryV2 to ImageSlotEntry
          ImageSlotEntry.Priority = ImageSlotEntryV2.Priority;
          ImageSlotEntry.UpdateRetries = ImageSlotEntryV2.UpdateRetries;
          ImageSlotEntry.GlitchRetries = ImageSlotEntryV2.GlitchRetries;
          ImageSlotEntry.ImageSlotAddr = ImageSlotEntryV2.ImageSlotAddr;
        } else {
          //assume entry location in PSP L1 uses address mode 0 or 1,
          //if address mode 2 is found, we cannot use such method to get image slot address
          TempImageSlotAddr = TryToConvertOffsetToPhysicalAddress (PspL1Dir->PspEntry[Index].Location);

          TempImageSlotAddr = (UINT32)(TempImageSlotAddr & 0xFFFFFFFF);
          TempImageSlotAddr = TempImageSlotAddr + BiosBaseAddr;

          CopyMem((UINT8 *)&ImageSlotEntry, (VOID *)(UINTN)(TempImageSlotAddr), sizeof (IMAGE_SLOT_HEADER));

        }

      //find image slots with top 2 priorities
      if (ImageSlotEntry.Priority == IMAGE_SLOT_PRIORITY_UNBOOTABLE) {
        TempSlot->UnbootableSlotArray[TempSlot->UnbootableSlotCount++] = Index;
      } else {
        TempSlot->BootableSlotArray[TempSlot->BootableSlotCount++] = Index;
      }

      //if we found image slot with higher priority, update image slot A
      if ( (ImageSlotEntry.Priority > TempSlot->SlotAHeader.Priority)  &&
            (Type == PSP_REGION_A_DIR) &&
            (PspChipId == ImageSlotEntryV2.PspId) ) {
         //update Slot A
         TempSlot->SlotAIndex = Index;
         TempSlot->SlotAHeader.Priority = ImageSlotEntry.Priority;
         TempSlot->SlotAHeader.UpdateRetries = ImageSlotEntry.UpdateRetries;
         TempSlot->SlotAHeader.GlitchRetries = ImageSlotEntry.GlitchRetries;
         TempSlot->SlotAHeader.ImageSlotAddr = (UINT32)TryToConvertOffsetToPhysicalAddress (ImageSlotEntry.ImageSlotAddr);
      }
      if ( (ImageSlotEntry.Priority > TempSlot->SlotBHeader.Priority) &&
           (Type == PSP_REGION_B_DIR) ) {
         TempSlot->SlotBIndex = Index;
         TempSlot->SlotBHeader.Priority = ImageSlotEntry.Priority;
         TempSlot->SlotBHeader.UpdateRetries = ImageSlotEntry.UpdateRetries;
         TempSlot->SlotBHeader.GlitchRetries = ImageSlotEntry.GlitchRetries;
         TempSlot->SlotBHeader.ImageSlotAddr = (UINT32)TryToConvertOffsetToPhysicalAddress (ImageSlotEntry.ImageSlotAddr);
       }
    }
  }

  if (TempSlot->BootableSlotCount == 0) {    //no valid slot entry found
    return FALSE;
  }

  if ((!IS_VALID_ADDR32(TempSlot->SlotAHeader.ImageSlotAddr)) ||
      (!ALIGN_4K_CHECK (TempSlot->SlotAHeader.ImageSlotAddr))) { // no valid Slot A address found
    return FALSE;
  }

  if ((!IS_VALID_ADDR32(TempSlot->SlotBHeader.ImageSlotAddr)) ||
      (!ALIGN_4K_CHECK (TempSlot->SlotBHeader.ImageSlotAddr))) { // no valid Slot B address found
    return FALSE;
  }

  *Slot = TempSlot;
  return TRUE;
}

/**
  This function is to validate PSP directory address on signature and crc checksum

  @param[in]  pointer to PSP directory base
  @param[in]  value of the signaure,
  @param[in]  Is PSP L1 directory or not

  @retval TRUE            the PSP directory base address is valid.
  @retval FALSE           the PSP directory base address is not valid.

**/
BOOLEAN
VerifyPspDir (
  IN       PSP_DIRECTORY     *PspDir,
  IN       UINT32            Signature,
  IN       BOOLEAN           IsPSPL1Dir
  )
{
  UINTN        Index;
  UINT32       SlotCount;
  //Validate Signature
  if (PspDir->Header.Cookie != Signature) {
    return FALSE;
  }

  //Do the checksum
  if (PspDir->Header.Checksum != CalculateChecksum ((UINT16 *) &PspDir->Header.TotalEntries, \
      (sizeof (PSP_DIRECTORY_HEADER) - OFFSET_OF (PSP_DIRECTORY_HEADER, TotalEntries) + PspDir->Header.TotalEntries * sizeof (PSP_DIRECTORY_ENTRY)) / 2)) {
    return FALSE;
  }

  //for multi-slot AB recovery, we will do more validation
  if (IsPSPL1Dir && VerifyMultiSlotLayout (PspDir)) {
    SlotCount = PspDir->Header.TotalEntries;
    if (SlotCount == 0 || SlotCount > MAX_IMAGE_SLOT_COUNT) {
        return FALSE;
    } else  {
      for (Index = 0; Index < SlotCount; Index++) {
        if ((PspDir->PspEntry[Index].Type.Field.Type != PSP_REGION_A_DIR)
              && (PspDir->PspEntry[Index].Type.Field.Type != PSP_REGION_B_DIR)) {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

BOOLEAN
VerifyBiosDir (
  IN       BIOS_DIRECTORY     *BiosDir,
  IN       UINT32     Signature
  )
{
  //Validate Signature
  if (BiosDir->Header.Cookie != Signature) {
    return FALSE;
  }
  //Do the checksum
  if (BiosDir->Header.Checksum != CalculateChecksum ((UINT16 *) &BiosDir->Header.TotalEntries, \
      (sizeof (PSP_DIRECTORY_HEADER) - OFFSET_OF (PSP_DIRECTORY_HEADER, TotalEntries) + BiosDir->Header.TotalEntries * sizeof (BIOS_DIRECTORY_ENTRY)) / 2)) {
    return FALSE;
  }
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function checks if the CPU/APU is Zen 2 or later (starting from Family 17 Model 30h)
 *
 *     @return          TRUE -  Zen 2 or later SOC.
 *     @return          FALSE - Zen 1 SOC or previous CPU/APU.
 */
BOOLEAN
CheckZen2OrLater (
  VOID
  )
{
  CPUID_DATA    CpuId;
  UINT32        Family;
  UINT32        Model;
  AsmCpuid (
      AMD_CPUID_FMF,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  Family = (((CpuId.EAX_Reg & CPUID_EXT_FAMILY_MASK) >> 20) + ((CpuId.EAX_Reg & CPUID_BASE_FAMILY_MASK) >> 8));
  Model = (((CpuId.EAX_Reg & CPUID_EXT_MODEL_MASK) >> 12) | ((CpuId.EAX_Reg & CPUID_BASE_MODEL_MASK) >> 4));

  if ((Family == 0x17 && Model >= 0x30) || (Family > 0x17)) {
      return TRUE;
  } else {
      return FALSE;
  }
}

BOOLEAN
CheckFirstGenEfsBeIgnored (
  IN FIRMWARE_ENTRY_TABLEV2      *FirmwareTableBase
  )
{
  BOOLEAN    FirstGenFfsStructure;
  FirstGenFfsStructure = FALSE;
  //check if it's first generation of EFS structure
  if ((FirmwareTableBase->Config & 0xFF) == 0xFF) {
      FirstGenFfsStructure = TRUE;
  }

  if (FirstGenFfsStructure && CheckZen2OrLater ()) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
FetchEfs (
  IN OUT   FIRMWARE_ENTRY_TABLEV2     *Efs ,
  IN       EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  )
{
//  UINT32                      i;
  UINT32                      j;
  FIRMWARE_ENTRY_TABLEV2      FirmwareTableBase;
  EFI_PHYSICAL_ADDRESS        MemoryAddress;


  ASSERT (Efs != NULL);
  CONST UINT32 RomSigOffsetTable[] =
  {
    0xFA0000, //  --> 512KB base
    0xF20000, //  --> 1MB base
    0xE20000, //  --> 2MB base
    0xC20000, //  --> 4MB base
    0x820000, //  --> 8MB base
    0x020000  //  --> 16MB base
  };

  for (j = 0; j < sizeof (RomSigOffsetTable) / sizeof (UINT32); j++) {
    MemoryAddress = (EFI_PHYSICAL_ADDRESS)(RomSigOffsetTable[j] + BiosBaseAddr);

    IDS_HDT_CONSOLE_PSP_TRACE ("FEA-SMM-%a-MemoryAddress %lx \n", __FUNCTION__, MemoryAddress);

    CopyMem((UINT8 *)&FirmwareTableBase, (VOID *)(UINTN)(MemoryAddress), sizeof (FIRMWARE_ENTRY_TABLEV2));

    IDS_HDT_CONSOLE_PSP_TRACE ("FEA-SMM-%a-FirmwareTableBase %lx \n", __FUNCTION__, &FirmwareTableBase);

    // Search flash for unique signature 0x55AA55AA
    if (FirmwareTableBase.Signature  == FIRMWARE_TABLE_SIGNATURE) {
      if (CheckFirstGenEfsBeIgnored (&FirmwareTableBase) == TRUE) {
        continue;
      }

      CopyMem (Efs, &FirmwareTableBase, sizeof (FIRMWARE_ENTRY_TABLEV2));
      return TRUE;
    }
  }
  return FALSE;
}

BOOLEAN
FetchComboDir (
  IN       UINT32     ComboDirBase,
  IN       UINT32     PspChipId,
  IN       UINT32     ComboSig,
  IN OUT   UINT32     *DirBase
  )
{
  COMBO_DIRECTORY *ComboDir;
  COMBO_DIRECTORY_ENTRY *ComboEntry;
  UINT32 i;


  ComboDir = (COMBO_DIRECTORY *) (UINTN) ComboDirBase;
  ComboEntry = &ComboDir->ComboEntry[0];
  //Check the signature
  if (ComboDir->Header.Cookie != ComboSig) {
    return FALSE;
  }
  //Loop to found matched entry
  for (i = 0; i < ComboDir->Header.TotalEntries; i++) {
    if (ComboEntry[i].Id == PspChipId) {
      *DirBase = (UINT32) ComboEntry[i].DirTableAddr;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  This function is to search EFS structure and get the valid PSP L1 directory base address.

  @param[out]  pointer to PSP directory base

  @retval TRUE            Successfully get the valid PSP directory base address.
  @retval FALSE           Valid PSP directory is not found.

**/
BOOLEAN
FetchPspDirBaseV2 (
  IN OUT   PSP_DIRECTORY     **PspDir ,
  IN       EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  )
{
  FIRMWARE_ENTRY_TABLEV2      FirmwareTableBase;
  UINT32                      FamilyId;
  UINT32                      PspChipId;
  BOOLEAN                     PspDirSigValid;
  UINT8                       *PspDirBuffer;
  UINT32                      PspDirBase;
  DIR_INFO                    *DirInfo;

  FamilyId = 0;
  PspChipId = 0;
  PspDirBuffer = NULL;
  PspDirBase = 0;

  DirInfo = FetchDirInfoEntry ();
  if (DirInfo == NULL) {
    //Unsupported Family detected
    ASSERT (FALSE);
    return (FALSE);
  }
  PspChipId = DirInfo->PspId;

  IDS_HDT_CONSOLE_PSP_TRACE ("FEA-SMM-%a-BiosBaseAddr %lx \n", __FUNCTION__, BiosBaseAddr);

  if (!FetchEfs (&FirmwareTableBase, BiosBaseAddr)) {
    return FALSE;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("FEA-SMM-%a-PspDirBase %lx \n", __FUNCTION__, FirmwareTableBase.PspDirBase);

  if (AllocateHeapBufferByHandle (AMD_PSP_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &PspDirBuffer) != TRUE) {
    return FALSE;
  }

  CopyMem((VOID *)PspDirBuffer, (VOID *)(UINTN)(FirmwareTableBase.PspDirBase + BiosBaseAddr), MaxPspDirSize);

  //Check if PSP combo structure
  if (FetchComboDir ((UINT32)(UINTN)PspDirBuffer, PspChipId, PSP_COMBO_DIRECTORY_COOKIE, &PspDirBase) == TRUE) {
     CopyMem((VOID *)PspDirBuffer, (VOID *)(UINTN)(PspDirBase + BiosBaseAddr), MaxPspDirSize);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("FEA-SMM-%a-PspDirBuffer %lx \n", __FUNCTION__, PspDirBuffer);

  PspDirSigValid = FALSE;
  PspDirSigValid = VerifyPspDir ((PSP_DIRECTORY *) (UINTN)PspDirBuffer, PSP_DIRECTORY_HEADER_SIGNATURE, TRUE);

  if (PspDirSigValid) {
    *PspDir = (PSP_DIRECTORY *) (UINTN)PspDirBuffer;
    return TRUE;
  } else {
    if (DirInfo->IshStructVersion == ISH_VERSION_2) {
      //Start from ISH version 2, it start to support PspDirBackupBase
      CopyMem((VOID *)PspDirBuffer, (VOID *)(UINTN)(FirmwareTableBase.PspDirBackupBase + BiosBaseAddr), MaxPspDirSize);

      PspDirSigValid = VerifyPspDir ((PSP_DIRECTORY *) (UINTN)PspDirBuffer, PSP_DIRECTORY_HEADER_SIGNATURE, TRUE);
      if (PspDirSigValid) {
        *PspDir = (PSP_DIRECTORY *) (UINTN)PspDirBuffer;
        return TRUE;
      } else {
        return FALSE;
      }
    } else {
      return FALSE;
    }
  }
}

/**
 *
 *  Get PSP Directory Entry 's properties from Level 2
 *
 *  @param[in]     EntryType        BIOS Directory Entry type
 *  @param[in]     PspLevel2BaseAddress  The PSP Level 2 Directory base address
 *  @param[in]     ImageSlotAddr  The image slot address, used starting from VN/MR
 *  @param[in,out] EntrySize        Size of entry
 *  @param[in,out] EntryDest        Destination of entry
 *
 *  @retval TRUE   Success to get the Entry 's properties
 *  @retval FALSE  Fail to get the Entry 's properties
 *
 **/
BOOLEAN
FetchLevel2PSPEntryInfo (
  IN       UINT32                      EntryType,
  IN       UINT64                      PspLevel2BaseAddress,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize,
  IN       EFI_PHYSICAL_ADDRESS        BiosBaseAddr
  )
{
  UINTN                    i;
  BOOLEAN                  PspDirValid;
  PSP_DIRECTORY_ENTRY_TYPE EntryTypeValue;
  PSP_DIRECTORY            *PspRegionDir;
  UINT8                    *PspDirBuffer;

  PspDirValid = FALSE;
  PspDirBuffer = NULL;
  EntryTypeValue.Value= EntryType;

  if (AllocateHeapBufferByHandle (AMD_PSP_L2_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &PspDirBuffer) != TRUE) {
     return FALSE;
  }

  //This is the way to replace after testing.
  CopyMem((VOID *)PspDirBuffer, (VOID *)(UINTN)(PspLevel2BaseAddress + BiosBaseAddr), MaxPspDirSize);

  PspRegionDir = (PSP_DIRECTORY*)PspDirBuffer;

  PspDirValid = VerifyPspDir (PspRegionDir, PSP_LV2_DIRECTORY_HEADER_SIGNATURE, FALSE);
  if (PspDirValid) {
  //Loop and compare the EntryType
    for (i = 0; i < PspRegionDir->Header.TotalEntries; i++) {
      if ((PspRegionDir->PspEntry[i].Type.Field.Type == EntryTypeValue.Field.Type) &&
          (PspRegionDir->PspEntry[i].Type.Field.SubProgram == EntryTypeValue.Field.SubProgram)) {
            *EntryAddress = ConvertEntryLocation (PspRegionDir->PspEntry[i].Location, PspLevel2BaseAddress, (UINT32)(PspLevel2BaseAddress & 0xFFFFFFFF));
            *EntrySize = PspRegionDir->PspEntry[i].Size;
            return (TRUE);
      }
    }
  }
  return (FALSE);
}

/**
 *
 *  Get BIOS Directory Entry 's properties from Level 2
 *
 *  @param[in]     EntryType        BIOS Directory Entry type
 *  @param[in]     EntryInstance    If input with INSTANCE_IGNORED, will return 1st Entry type matched
 *                                  If input with Non INSTANCE_IGNORED, will return the entry which both Type & Instance matched
 *  @param[in]     SubProgram       If input with SUBPROGRAM_IGNORED, will return 1st Entry type & Instance matched
 *                                  If input with Non SUBPROGRAM_IGNORED, will return the entry which all Type & Instance & SubProgram matched
 *  @param[in]     DirectoryType    For programs with multiple slot layout(VN/MR/RMB), specify DIR_TYPE_PSP_LV2, it will find BIOS entry in PSP L2,
                                    For programs without multiple slot layout(RN/CZN/MTS), specify DIR_TYPE_BIOS_LV2, it will find BIOS entry in BIOS L2.
                                    Note that in VN/MR, BIOS L2 is part of PSP L2 directory
 *  @param[in]     Level2BaseAddress  if DirectoryType=DIR_TYPE_PSP_LV2, please specify PSP L2 base address(only for VN/MR and beyond),
 *                                    if DirectoryType=DIR_TYPE_BIOS_LV2, please specify BIOS L2 base address(for programs before VN, such as RN/CZN/MTS)
 *  @param[in,out] TypeAttrib       TypeAttrib of entry
 *  @param[in,out] EntryAddress     Address of entry
 *  @param[in,out] EntrySize        Size of entry
 *  @param[in,out] EntryDest        Destination of entry
 *
 *  @retval TRUE   Success to get the Entry 's properties
 *  @retval FALSE  Fail to get the Entry 's properties
 *
 **/
BOOLEAN
FetchLevel2BIOSEntryInfo (
  IN       UINT32                      EntryType,
  IN       UINT8                       EntryInstance,
  IN       UINT8                       SubProgram,
  IN       DIRECTORY_TYPE              DirectoryType,
  IN       UINT64                      Level2BaseAddress,
  IN OUT   TYPE_ATTRIB                 *TypeAttrib,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize,
  IN OUT   UINT64                      *EntryDest,
  IN       EFI_PHYSICAL_ADDRESS        BiosBaseAddr
  )
{
  UINTN                 i;
  BOOLEAN               BiosDirValid;
  BOOLEAN               Found;
  BIOS_DIRECTORY        *BiosLevel2Dir;
  UINT8                 *BiosDirBuffer;
  UINT64                BIOSLevel2BaseAddress;
  UINT32                IgnoredEntrySize;
  UINT32                ImageSlotAddr;

  BiosDirValid = FALSE;
  Found = FALSE;
  BiosDirBuffer = NULL;
  BIOSLevel2BaseAddress = 0;
  IgnoredEntrySize = 0;
  ImageSlotAddr = 0;

  //get buffer
  if (AllocateHeapBufferByHandle (AMD_PSP_L2_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &BiosDirBuffer) != TRUE) {
    return FALSE;
  }

  CopyMem((VOID *)BiosDirBuffer, (VOID *)(UINTN)(BiosBaseAddr), MaxPspDirSize);

  BiosLevel2Dir = (BIOS_DIRECTORY*)BiosDirBuffer;
  BiosDirValid = VerifyBiosDir (BiosLevel2Dir, BIOS_LV2_DIRECTORY_HEADER_SIGNATURE);

  if (BiosDirValid) {
    for (i = 0; i < BiosLevel2Dir->Header.TotalEntries; i++) {
      if (BiosLevel2Dir->BiosEntry[i].TypeAttrib.Type == EntryType) {
        if (EntryInstance == INSTANCE_IGNORED) {
           if ((SubProgram == SUBPROGRAM_IGNORED) || (SubProgram == BiosLevel2Dir->BiosEntry[i].TypeAttrib.SubProgram)) {
               Found = TRUE;
           }
        } else if (BiosLevel2Dir->BiosEntry[i].TypeAttrib.Instance == EntryInstance) {
           if ((SubProgram == SUBPROGRAM_IGNORED) || (SubProgram == BiosLevel2Dir->BiosEntry[i].TypeAttrib.SubProgram)) {
               Found = TRUE;
           }
        }
        if (Found == TRUE) {
          *TypeAttrib = BiosLevel2Dir->BiosEntry[i].TypeAttrib;
          *EntrySize = BiosLevel2Dir->BiosEntry[i].Size;
          *EntryDest = BiosLevel2Dir->BiosEntry[i].Destination;
          *EntryAddress = ConvertEntryLocation (BiosLevel2Dir->BiosEntry[i].Location, BIOSLevel2BaseAddress, ImageSlotAddr);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

BOOLEAN
GetSharedB
(
    IN PSP_DIRECTORY       *PspL1Dir,
    IN OUT IMAGE_SLOT_INFO **Slot
){
    IMAGE_SLOT_HEADER   ImageSlotEntry;
    IMAGE_SLOT_HEADER_V2 ImageSlotEntryV2;
    UINT8               Index = 0;
    UINT8               *ImageSlotBuffer = NULL;
    IMAGE_SLOT_INFO     *TempSlot = NULL;
    UINT64              TempImageSlotAddr = 0;
    DIR_INFO            *DirInfo;
    UINT32              PspChipId;
    UINT32              IshStructVersion;

    ASSERT (PspL1Dir != NULL);
    if (AllocateHeapBufferByHandle (AMD_PSP_IMAGE_SLOT_BUFFER_HANDLE, MaxImageSlotInfoSize, &ImageSlotBuffer) != TRUE) {
       return FALSE;
    }


    DirInfo = FetchDirInfoEntry ();
    if (DirInfo == NULL) {
      //Unsupported Family detected
      ASSERT (FALSE);
      return (FALSE);
    }

    PspChipId = DirInfo->PspId;
    IshStructVersion = DirInfo->IshStructVersion;

    TempSlot = (IMAGE_SLOT_INFO *) (UINTN)ImageSlotBuffer;
    TempSlot->SlotCount = 0;
    TempSlot->BootableSlotCount = 0;
    TempSlot->UnbootableSlotCount = 0;

    TempSlot->SlotAIndex = 0xFF;
    TempSlot->SlotAHeader.Priority = IMAGE_SLOT_PRIORITY_UNBOOTABLE;
    TempSlot->SlotAHeader.UpdateRetries = 0;
    TempSlot->SlotAHeader.GlitchRetries = 0;
    TempSlot->SlotAHeader.ImageSlotAddr = 0;

    TempSlot->SlotBIndex = 0xFF;
    TempSlot->SlotBHeader.Priority = IMAGE_SLOT_PRIORITY_UNBOOTABLE;
    TempSlot->SlotBHeader.UpdateRetries = 0;
    TempSlot->SlotBHeader.GlitchRetries = 0;
    TempSlot->SlotBHeader.ImageSlotAddr = 0;
    //
  // All the valid entry in PSP L1 directory MUST be continous.
  // Invalid entry in the middle of valid entries is not supported.
  //
  for (Index = 0; Index < PspL1Dir->Header.TotalEntries; Index++) {
    if ((PspL1Dir->PspEntry[Index].Type.Field.Type == PSP_REGION_A_DIR)
        || (PspL1Dir->PspEntry[Index].Type.Field.Type == PSP_REGION_B_DIR)) {
        TempSlot->SlotCount++;

      //ISH version 2 detected, start from this version
      // 1. Add CRC checksum
      // 2. Add PSPID to support combo, w/o combo directory
      //Assume entry LOCATION in PSP L1 uses address mode 0 or 1,
      //if address mode 2 is found, we cannot use such method to get image slot address
      TempImageSlotAddr = TryToConvertOffsetToPhysicalAddress (PspL1Dir->PspEntry[Index].Location);
      if (FchSpiRomReadEx ((UINT32)(TempImageSlotAddr & 0xFFFFFFFF), (UINT8 *)&ImageSlotEntryV2,
                          sizeof (ImageSlotEntryV2)) != TRUE) {
        ASSERT (FALSE);
        return FALSE;
      }

      if(PspL1Dir->PspEntry[Index].Type.Field.Type != PSP_REGION_B_DIR)
      {
          if (PspChipId != ImageSlotEntryV2.PspId) {
            TempSlot->UnbootableSlotArray[TempSlot->UnbootableSlotCount++] = Index;
            continue; //Ignore this entry
          }
      }

      //To make the logical generic for all program, transfer ImageSlotEntryV2 to ImageSlotEntry
      ImageSlotEntry.Priority = ImageSlotEntryV2.Priority;
      ImageSlotEntry.UpdateRetries = ImageSlotEntryV2.UpdateRetries;
      ImageSlotEntry.GlitchRetries = ImageSlotEntryV2.GlitchRetries;
      ImageSlotEntry.ImageSlotAddr = ImageSlotEntryV2.ImageSlotAddr;


      //find image slots with top 2 priorities
      if (ImageSlotEntry.Priority == IMAGE_SLOT_PRIORITY_UNBOOTABLE) {
        TempSlot->UnbootableSlotArray[TempSlot->UnbootableSlotCount++] = Index;
      } else {
        TempSlot->BootableSlotArray[TempSlot->BootableSlotCount++] = Index;
      }

      //if we found image slot with higher priority, update image slot A
      if (ImageSlotEntry.Priority > TempSlot->SlotAHeader.Priority) {
        //save TempSlot->SlotA to SlotB
        if (TempSlot->SlotAHeader.Priority != IMAGE_SLOT_PRIORITY_UNBOOTABLE) {
          TempSlot->SlotBIndex = TempSlot->SlotAIndex;
          TempSlot->SlotBHeader.Priority = TempSlot->SlotAHeader.Priority;
          TempSlot->SlotBHeader.UpdateRetries = TempSlot->SlotAHeader.UpdateRetries;
          TempSlot->SlotBHeader.GlitchRetries = TempSlot->SlotAHeader.GlitchRetries;
          TempSlot->SlotBHeader.ImageSlotAddr = TempSlot->SlotAHeader.ImageSlotAddr;
        }
        //update Slot A
        TempSlot->SlotAIndex = Index;
        TempSlot->SlotAHeader.Priority = ImageSlotEntry.Priority;
        TempSlot->SlotAHeader.UpdateRetries = ImageSlotEntry.UpdateRetries;
        TempSlot->SlotAHeader.GlitchRetries = ImageSlotEntry.GlitchRetries;
        TempSlot->SlotAHeader.ImageSlotAddr =
          (UINT32)TryToConvertOffsetToPhysicalAddress (ImageSlotEntry.ImageSlotAddr);
      } else {
        TempSlot->SlotBIndex = Index;
        TempSlot->SlotBHeader.Priority = ImageSlotEntry.Priority;
        TempSlot->SlotBHeader.UpdateRetries = ImageSlotEntry.UpdateRetries;
        TempSlot->SlotBHeader.GlitchRetries = ImageSlotEntry.GlitchRetries;
        TempSlot->SlotBHeader.ImageSlotAddr =
          (UINT32)TryToConvertOffsetToPhysicalAddress (ImageSlotEntry.ImageSlotAddr);
      }
    }
  }

  if (TempSlot->BootableSlotCount == 0) {    //no valid slot entry found
    return FALSE;
  }

  if ((!IS_VALID_ADDR32(TempSlot->SlotAHeader.ImageSlotAddr)) ||
      (!ALIGN_4K_CHECK (TempSlot->SlotAHeader.ImageSlotAddr))) { // no valid Slot A address found
    return FALSE;
  }

  if ((!IS_VALID_ADDR32(TempSlot->SlotBHeader.ImageSlotAddr)) ||
      (!ALIGN_4K_CHECK (TempSlot->SlotBHeader.ImageSlotAddr))) { // no valid Slot B address found
    return FALSE;
  }

  *Slot = TempSlot;
  return TRUE;
}

BOOLEAN
IsRMBA0 (
  VOID
  )
{
  CPUID_DATA    CpuId;
  UINT32        Family;
  UINT32        Model;
  AsmCpuid (
      AMD_CPUID_FMF,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  Family = (((CpuId.EAX_Reg & CPUID_EXT_FAMILY_MASK) >> 20) + ((CpuId.EAX_Reg & CPUID_BASE_FAMILY_MASK) >> 8));
  Model = (((CpuId.EAX_Reg & CPUID_EXT_MODEL_MASK) >> 12) | ((CpuId.EAX_Reg & CPUID_BASE_MODEL_MASK) >> 4));

  if (Family == 0x19 && Model == 0x40) {
      return TRUE;
  } else {
      return FALSE;
  }
}

BOOLEAN
ValidateAddressAndSize(
  IN OUT UINT64*                          SourceEntryAddress,
  IN OUT UINT32*                          SourceEntrySize,
  IN OUT UINT64*                          DestinationEntryAddress,
  IN OUT UINT32*                          DestinationEntrySize
)
{
  if (!ALIGN_4K_CHECK (*SourceEntryAddress)) {
   *SourceEntrySize += *SourceEntryAddress & (UINT64)(0xFFF);
   *SourceEntryAddress &= (UINT64)(~((UINT64)0xFFF));
   IDS_HDT_CONSOLE_PSP_TRACE( "FEA-PEI-%a-SourceEntryAddress=%x\n", __FUNCTION__, *SourceEntryAddress);
  }
  if (!ALIGN_4K_CHECK (*DestinationEntryAddress)) {
   *DestinationEntrySize += *DestinationEntryAddress & (UINT64)(0xFFF);
   *DestinationEntryAddress &= (UINT64)(~((UINT64)0xFFF));
   IDS_HDT_CONSOLE_PSP_TRACE( "FEA-PEI-%a-DestinationEntryAddress=%x\n", __FUNCTION__, *DestinationEntryAddress);
  }
  if (!ALIGN_4K_CHECK (*SourceEntrySize)) {
   *SourceEntrySize &= (UINT64)(~((UINT64)0xFFF));
   *SourceEntrySize += (UINT64)(0x1000);
   IDS_HDT_CONSOLE_PSP_TRACE( "FEA-PEI-%a-SourceEntrySize=%x\n", __FUNCTION__, *SourceEntrySize);
  }
  if (!ALIGN_4K_CHECK (*DestinationEntrySize)) {
   *DestinationEntrySize &= (UINT64)(~((UINT64)0xFFF));
   *DestinationEntrySize += (UINT64)(0x1000);
   IDS_HDT_CONSOLE_PSP_TRACE( "FEA-PEI-%a-DestinationEntrySize=%x\n", __FUNCTION__, *DestinationEntrySize);
  }

  //source entry size should be same as destination entry size
  if (*SourceEntrySize != *DestinationEntrySize) {
   IDS_HDT_CONSOLE_PSP_TRACE( "FEA-PEI-%a-End-5-Source/ Destination Entry Size are different\n", __FUNCTION__);
   return FALSE;
  }
  
  // Source address should not same as destination.
  if (*SourceEntryAddress == *DestinationEntryAddress) {
   IDS_HDT_CONSOLE_PSP_TRACE( "FEA-PEI-%a-End-5-Source/ Destination Entry address are the same\n", __FUNCTION__);
   return FALSE;
  }

  return TRUE;
}

