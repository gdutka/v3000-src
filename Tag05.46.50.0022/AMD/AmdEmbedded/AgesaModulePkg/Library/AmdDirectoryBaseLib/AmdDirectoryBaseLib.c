/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Base Library
 *
 * Contains interface to the PSP library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
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
#include <Library/AmdPspBaseLibV2.h>
#include <Library/FchBaseLib.h>
#include <Library/FchSpiAccessLib.h>
#include <FchBiosRamUsage.h>
#include <Library/AmdHeapLib.h>
#include <Library/SmnAccessLib.h>

#define FILECODE LIBRARY_AMDDIRECTORYBASELIB_AMDDIRECTORYBASELIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FORCE_SPIADDR_BIT24(SpiAddress) (SpiAddress | 0xFF000000)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

#define BIOS_DIR_EFS_OFFSET_IGNORE  0xFFFFFFFFul   //Ignored
#define ISH_VERSION_1         1   //1: IMAGE_SLOT_HEADER version 1, IMAGE_SLOT_HEADER
#define ISH_VERSION_2         2   //1: IMAGE_SLOT_HEADER version 2, IMAGE_SLOT_HEADER_V2
#define ISH_VERSION_IGNORE    0xFFFFFFFFul   //Ignored

/// Structure to store Directory related information
typedef struct {
  UINT32                  FamilyRawId;            // CpuId.0x80000001.EAX_Reg & RAW_FAMILY_ID_MASK;
  UINT32                  PspId;                  // Id defined by bootrom
  UINT32                  BiosDirEfsOffset;       // Used in the previous programs,
                                                  // when combo BIOS dir and A/B recovery is not supported,
                                                  // check BiosDirBase in FIRMWARE_ENTRY_TABLEV2
  BOOLEAN                 ComboBisoDirSupport;    // TRUE: Combo Dir supported, FALSE: Combo Dir not supported
  UINT32                  IshStructVersion;       // Version for ISH structure, 1: IMAGE_SLOT_HEADER,
                                                  // 2: IMAGE_SLOT_HEADER_V2, 0xFFFFFFF, ignored
  RECOVERY_REASON_VERSION RecoveryReasonVersion;  // Version for Recovery Reason, 1: RECOVERY_REASON_VERSION_1,
                                                  // 2: RECOVERY_REASON_VERSION_2, 0xFFFFFFF, ignored
  PSP_DIRECTORY_ID_SCHEME IdScheme;               // PSP_DIRECTORY_ID_USE_FW_ID: Firmware ID was used in PSP entry, defined in _PSP_DIRECTORY_FW_ID
                                                  // PSP_DIRECTORY_ID_USE_ENTRY_TYPE: Entry Type was used in PSP entry, defined in _PSP_DIRECTORY_ENTRY_TYPE
} DIR_INFO;

/// Entry Structure for mEntryTypeFwIdMap
typedef struct _ENTRY_TYPE_FW_ID_MAP_ENTRY {
  UINT8  EntryType;
  UINT16 FwId;
} ENTRY_TYPE_FW_ID_MAP_ENTRY;
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/// Table to store all supported program directory infomation
DIR_INFO gmDirInfoTbl[] = {
  {F19_PHX_RAW_ID,  PHX_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                         FALSE,  ISH_VERSION_2,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_RPL_RAW_ID,  RPL_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                         FALSE,  ISH_VERSION_2,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_RMB_RAW_ID,  RMB_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                         FALSE,  ISH_VERSION_2,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_VMR_RAW_ID,  VMR_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, SspBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_1, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_CZN_RAW_ID,  CZN_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase),  TRUE,   ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_1, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_BA_RAW_ID,   BA_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_GN_RAW_ID,   GN_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_VN_RAW_ID,   VN_PSP_CHIP_ID,   BIOS_DIR_EFS_OFFSET_IGNORE,                         TRUE,   ISH_VERSION_1,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_MR_RAW_ID,   MR_PSP_CHIP_ID,   BIOS_DIR_EFS_OFFSET_IGNORE,                         TRUE,   ISH_VERSION_1,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_RN_RAW_ID,   RN_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase),  TRUE,   ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_1, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_ZP_RAW_ID,   ZP_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, ZpBiosDirBase),   FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_RV_RAW_ID,   RV_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, RvBiosDirBase),   FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_PIC_RAW_ID,  RV_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, RvBiosDirBase),   FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_RV2_RAW_ID,  RV2_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, RvBiosDirBase),   FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_MTS_RAW_ID,  MTS_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, SspBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_SSP_RAW_ID,  SSP_PSP_CHIP_ID,  OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, SspBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F17_MDN_RAW_ID,  MDN_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                         FALSE,  ISH_VERSION_2,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_RS_RAW_ID,   RS_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_RSDN_RAW_ID, RS_PSP_CHIP_ID,   OFFSET_OF(FIRMWARE_ENTRY_TABLEV2, NewBiosDirBase),  FALSE,  ISH_VERSION_IGNORE,
                    RECOVERY_REASON_VERSION_IGNORE, PSP_DIRECTORY_ID_USE_ENTRY_TYPE},
  {F19_MI3_RAW_ID,  MI3_PSP_CHIP_ID,  BIOS_DIR_EFS_OFFSET_IGNORE,                         FALSE,  ISH_VERSION_2,
                    RECOVERY_REASON_VERSION_2, PSP_DIRECTORY_ID_USE_FW_ID},

};


/**
  Get DIR_INFO information according RAW_FAMILY_ID from gmDirInfoTbl, return NULL if not found

  @retval return TRUE if found
  @retval return FALSE if not found

**/
BOOLEAN
GetDirInfoEntry (
  DIR_INFO **DirInfoTblEntry
  )
{
  UINT32 i;
  BOOLEAN IsFound;

  *DirInfoTblEntry = NULL;
  IsFound          = FALSE;

  for (i = 0; i < (sizeof (gmDirInfoTbl)/ sizeof (DIR_INFO)); i++) {
    if (SocFamilyIdentificationCheck (gmDirInfoTbl[i].FamilyRawId)) {
      IsFound          = TRUE;
      *DirInfoTblEntry = &gmDirInfoTbl[i];
      break;
    }
  }

  return IsFound;
}

/**
  This function is to calculate the crc checksum

  @param[in]  pointer to content
  @param[in]  length of content

  @retval CRC value

**/
UINT32
Fletcher32 (
  IN OUT   VOID  *data,
  IN       UINTN   words
  )
{
  UINT32 sum1;
  UINT32 sum2;
  UINTN tlen;
  UINT16 *data16;

  sum1 = 0xffff;
  sum2 = 0xffff;
  data16 = data;

  while (words) {
    tlen = words >= 359 ? 359 : words;
    words -= tlen;
    do {
      sum2 += sum1 += *data16++;
    } while (--tlen);
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  }
  // Second reduction step to reduce sums to 16 bits
  sum1 = (sum1 & 0xffff) + (sum1 >> 16);
  sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  return sum2 << 16 | sum1;
}

/**
 * @brief Check if current system is using
 *
 * @return BOOLEAN TRUE: Current system using FW ID FALSE: Not used
 */
BOOLEAN
UseFwId (
  VOID
  )
{
  DIR_INFO *DirInfo;

  DirInfo = NULL;

  if (GetDirInfoEntry (&DirInfo) == TRUE) {
    if (DirInfo->IdScheme == PSP_DIRECTORY_ID_USE_FW_ID) {
      return TRUE;
    }
  }

  return FALSE;
}

/// global variable to describe the mapping between Entry Type and Firmware ID
/// Note, only few of record that required by AGESA infrastructure has been listed
ENTRY_TYPE_FW_ID_MAP_ENTRY mEntryTypeFwIdMap [] = {
  {PSP_REGION_A_DIR, PSP_REGION_A_FW_ID},
  {PSP_REGION_B_DIR, PSP_REGION_B_FW_ID},
  {BIOS_REGION_DIR, BIOS_REGION_DIR_FW_ID},
};
/**
 * @details  Check If input can be found the EntryTypeFwIdMap, return FwId, or else return INVALID_FW_ID
 *           The purpose is to minimize the change when introduce Firmware ID
 * @param EntryTypeId
 * @return PSP_DIRECTORY_FW_ID
 */
UINT16
ConvertEntryTypeToFwId (
  UINT8 EntryTypeId
  )
{
  UINT32 i;

  for (i = 0; i < (sizeof (mEntryTypeFwIdMap) / sizeof (ENTRY_TYPE_FW_ID_MAP_ENTRY)); i++) {
    if (mEntryTypeFwIdMap[i].EntryType == EntryTypeId) {
      return mEntryTypeFwIdMap[i].FwId;
    }
  }
  return INVALID_FW_ID;
}
/**
 * @brief   Generic interface to compare if the input Directory entry matches the input EntryTypeId
 *
 * @details This routine use EntryTypeId as input,
 *          On system using FwId, convert it to FwId, compare FwId with PSP_DIRECTORY_FW_ID_FIELD.FwId
 *          On system using EntryType, keep the EntryTypeId, compare EntryTypeId with PSP_DIRECTORY_ENTRY_TYPE_FIELD.Type
 *          The purpose is to minimize the change when introduce Firmware ID

 * @param EntryTypeId
 * @return BOOLEAN True It is the entry using EntryTypeId
 */
BOOLEAN
DirTypeCompare (
  PSP_DIRECTORY_ENTRY_TYPE DirEntry,
  UINT8 EntryTypeId
  )
{
  UINT16 FwId;

  if (UseFwId ()) {
    FwId = ConvertEntryTypeToFwId (EntryTypeId);
    ASSERT (FwId != INVALID_FW_ID);
    if (FwId == INVALID_FW_ID) {
      return FALSE;
    }
    return (DirEntry.FwIdField.FwId == FwId);
  } else {
    return (DirEntry.Field.Type == EntryTypeId);
  }
}
/**
 *
 *  Get A/B Recovery reason
 *
 *  @param[in, out] Reason         buffer to save recovery reason
 *  @param[in, out] Size           buffer size
 *  @param[in, out] ReasonVersion  recovery reason version, optional parameter
 *        with this info, you can know which type(RECOVERY_REASON_V1/RECOVERY_REASON_V2) to convert the buffer into.
 *        specify NULL if you don't need to know the recovery reason version info
 *
 *  @retval EFI_STATUS            0: Success, NonZero Error
 *
 **/
EFI_STATUS
GetRecoveryReason (
  VOID                    *Reason,
  UINT32                  *Size,
  RECOVERY_REASON_VERSION *ReasonVersion
  )
{
  UINT32 RecoveryReason  = 0;
  DIR_INFO      *DirInfo = NULL;
  FchReadBiosram (BIOS_RAM_AB_RECOVERY_REASON_BASE, AccessWidth32, &RecoveryReason, NULL);
  if (*Size >= sizeof (UINT32)) {
    if (ReasonVersion != NULL) {
      if(GetDirInfoEntry (&DirInfo) == FALSE) {
        //Unsupported Family detected
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
      }
      *ReasonVersion = DirInfo->RecoveryReasonVersion;
    }
    CopyMem (Reason, &RecoveryReason, sizeof (UINT32));
    return EFI_SUCCESS;
  } else {
    *Size = sizeof (UINT32);
    return EFI_BUFFER_TOO_SMALL;
  }
}

/**
 * @brief Set A/B Recovery Reason to BIOS RAM
 *
 * @param RecoveryReason  you need to convert RECOVERY_REASON_V1 or RECOVERY_REASON_V2 to UINT32 before this call
 * @return EFI_STATUS     0: Success, NonZero Error
 */
EFI_STATUS
SetRecoveryReason (
  UINT32      RecoveryReason
  )
{
  FchWriteBiosram (BIOS_RAM_AB_RECOVERY_REASON_BASE, AccessWidth32, &RecoveryReason, NULL);
  return EFI_SUCCESS;
}

BOOLEAN
IsPspDirAddrRom2Decoded (
  IN      UINT64                      EntryLocation
  )
{
  if (PcdGetBool (PcdPspDirUsing16MAddress) == TRUE) {
      return TRUE;
  } else {
      return FALSE;
  }
}

UINT64
TryToTranslateOffsetToPhysicalAddress (
  IN      UINT64                      EntryLocation
  )
{
  if (IS_SPI_OFFSET (EntryLocation)) {
      if (IsPspDirAddrRom2Decoded (EntryLocation)) {
         EntryLocation = FORCE_SPIADDR_BIT24 (EntryLocation);
      }
  }
  return EntryLocation;
}

UINT64
TranslateEntryLocation (
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
  return TryToTranslateOffsetToPhysicalAddress (EntryLocation);
}

/**
 *
 *  Map SPI data to the Buffer,
 *  if SPI offset is specified, call FchSpiRomRead to get data, otherwise, access directly
 *
 *  @param[in]     Address    where to get the data, it may be a SPI offset or physical address
 *  @param[in,out] Buffer     where the data save to, the buffer must be allocated by the caller
 *  @param[in]     Size       how big the data is
 *
 *  @retval TRUE   Success to get the content from Address
 *  @retval FALSE  Fail to get the content from Address
 *
 **/
BOOLEAN
MapSpiDataToBuffer (
  IN       UINT32                      Address,
  IN OUT   VOID                        *Buffer,
  IN       UINT32                      Size
  )
{
  ZeroMem(Buffer, Size);
  if (FchSpiRomReadEx (Address, Buffer, Size) != TRUE) {
    return FALSE;
  }
  return TRUE;
}

/**
  This function checks if the PSP L1 is multi slot layout

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has multi slots (>=2)
  @retval FALSE            the PSP L1 directory has not multi slots (>=2)

**/
BOOLEAN
IsMultiSlotLayout (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  )
{
  UINT32               i;
  UINT32               RecoveryEntryCount = 0;
  BOOLEAN              RecoveryEntryExists = FALSE;

  ASSERT (PspL1Dir != NULL);

  if (PspL1Dir->Header.TotalEntries > TOTAL_ENTRIES_MAX) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP Entries exceeds the limit\n");
    return FALSE;
  }

  for (i = 0; i < PspL1Dir->Header.TotalEntries; i++) {
    if (DirTypeCompare (PspL1Dir->PspEntry[i].Type, PSP_REGION_A_DIR) ||
        DirTypeCompare (PspL1Dir->PspEntry[i].Type, PSP_REGION_B_DIR)) {
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
IsTwoSlotAbRecovery (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  )
{
  UINT32               i;
  UINT32               RecoveryEntryCount = 0;
  BOOLEAN              RegionBEntryExists = FALSE;

  ASSERT (PspL1Dir != NULL);

  if (PspL1Dir->Header.TotalEntries > TOTAL_ENTRIES_MAX) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP Entries exceeds the limit\n");
    return FALSE;
  }

  for (i = 0; i < PspL1Dir->Header.TotalEntries; i++) {
    if (DirTypeCompare (PspL1Dir->PspEntry[i].Type, PSP_REGION_A_DIR) ||
        DirTypeCompare (PspL1Dir->PspEntry[i].Type, PSP_REGION_B_DIR)) {
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

/**
 * @brief Copy Image Slot Header
 *
 * @param Source        Source of Image Slot Header
 * @param Destination   Destination of Image Slot Header
 * @return VOID         no return value
 */
VOID
UpdateImageSlotHeader (
  IN      IMAGE_SLOT_HEADER     *Source,
  IN OUT  IMAGE_SLOT_HEADER     *Destination
  )
{
  Destination->Priority      = Source->Priority;
  Destination->UpdateRetries = Source->UpdateRetries;
  Destination->GlitchRetries = Source->GlitchRetries;
  Destination->ImageSlotAddr = Source->ImageSlotAddr;
}

BOOLEAN
GetImageSlotInfo (
  IN PSP_DIRECTORY       *PspL1Dir,
  IN OUT IMAGE_SLOT_INFO **Slot
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

  DirInfo = NULL;

  ASSERT (PspL1Dir != NULL);
  if (GetHeapBufferByHandle (AMD_PSP_IMAGE_SLOT_BUFFER_HANDLE, MaxImageSlotInfoSize, &ImageSlotBuffer) != TRUE) {
     return FALSE;
  }

  if(GetDirInfoEntry (&DirInfo) == FALSE) {
    //Unsupported Family detected
    ASSERT (FALSE);
    return (FALSE);
  }

  PspChipId = DirInfo->PspId;
  IshStructVersion = DirInfo->IshStructVersion;

  // if no image slot, return directly
  if (IshStructVersion == ISH_VERSION_IGNORE) {
    return (FALSE);
  }

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
    if (DirTypeCompare (PspL1Dir->PspEntry[Index].Type, PSP_REGION_A_DIR) ||
        DirTypeCompare (PspL1Dir->PspEntry[Index].Type, PSP_REGION_B_DIR)) {
        TempSlot->SlotCount++;
      if (IshStructVersion == ISH_VERSION_2) {
        //ISH version 2 detected, start from this version
        // 1. Add CRC checksum
        // 2. Add PSPID to support combo, w/o combo directory
        //Assume entry LOCATION in PSP L1 uses address mode 0 or 1,
        //if address mode 2 is found, we cannot use such method to get image slot address
        TempImageSlotAddr = TryToTranslateOffsetToPhysicalAddress (PspL1Dir->PspEntry[Index].Location);
        if (FchSpiRomReadEx ((UINT32)(TempImageSlotAddr & 0xFFFFFFFF), (UINT8 *)&ImageSlotEntryV2,
                            sizeof (ImageSlotEntryV2)) != TRUE) {
          ASSERT (FALSE);
          return FALSE;
        }

        // Do PspId check 1st
        if (PspChipId != ImageSlotEntryV2.PspId) {
          TempSlot->UnbootableSlotArray[TempSlot->UnbootableSlotCount++] = Index;
          continue; //Ignore this entry
        }
        //Todo: Do the checksum check
        //To make the logical generic for all program, transfer ImageSlotEntryV2 to ImageSlotEntry
        ImageSlotEntry.Priority = ImageSlotEntryV2.Priority;
        ImageSlotEntry.UpdateRetries = ImageSlotEntryV2.UpdateRetries;
        ImageSlotEntry.GlitchRetries = ImageSlotEntryV2.GlitchRetries;
        ImageSlotEntry.ImageSlotAddr = ImageSlotEntryV2.ImageSlotAddr;
      } else {
        //assume entry location in PSP L1 uses address mode 0 or 1,
        //if address mode 2 is found, we cannot use such method to get image slot address
        TempImageSlotAddr = TryToTranslateOffsetToPhysicalAddress (PspL1Dir->PspEntry[Index].Location);
        if (FchSpiRomReadEx ((UINT32)(TempImageSlotAddr & 0xFFFFFFFF), (UINT8 *)&ImageSlotEntry,
                              sizeof (IMAGE_SLOT_HEADER)) != TRUE) {
          ASSERT (FALSE);
          return FALSE;
        }
      }

      if (ImageSlotEntry.Priority == IMAGE_SLOT_PRIORITY_UNBOOTABLE) {
        TempSlot->UnbootableSlotArray[TempSlot->UnbootableSlotCount++] = Index;
      } else {
        TempSlot->BootableSlotArray[TempSlot->BootableSlotCount++] = Index;
      }

      ImageSlotEntry.ImageSlotAddr = (UINT32)TryToTranslateOffsetToPhysicalAddress (ImageSlotEntry.ImageSlotAddr);

      // For ISH V2 (RMB and later), 0x48 points to Slot A, 0x4A points to Slot B
      if (IshStructVersion == ISH_VERSION_2) {
        if (DirTypeCompare (PspL1Dir->PspEntry[Index].Type, PSP_REGION_A_DIR)) {
           TempSlot->SlotAIndex = Index;
           UpdateImageSlotHeader (&ImageSlotEntry, &TempSlot->SlotAHeader);
        } else if (DirTypeCompare (PspL1Dir->PspEntry[Index].Type, PSP_REGION_B_DIR)) {
           TempSlot->SlotBIndex = Index;
           UpdateImageSlotHeader (&ImageSlotEntry, &TempSlot->SlotBHeader);
        }
      } else if (IshStructVersion == ISH_VERSION_1) {
        // For ISH V1 (VN/MR), find image slots with top 2 priorities as Slot A and B
        // if we found image slot with higher priority, update image slot A
        if (ImageSlotEntry.Priority > TempSlot->SlotAHeader.Priority) {
          // save TempSlot->SlotA to SlotB
          if (TempSlot->SlotAHeader.Priority != IMAGE_SLOT_PRIORITY_UNBOOTABLE) {
            TempSlot->SlotBIndex = TempSlot->SlotAIndex;
            UpdateImageSlotHeader (&TempSlot->SlotAHeader, &TempSlot->SlotBHeader);
          }
          // update Slot A
          TempSlot->SlotAIndex = Index;
          UpdateImageSlotHeader (&ImageSlotEntry, &TempSlot->SlotAHeader);
        } else {
          TempSlot->SlotBIndex = Index;
          UpdateImageSlotHeader (&ImageSlotEntry, &TempSlot->SlotBHeader);
        }
      }
    }
  }

  if (TempSlot->BootableSlotCount == 0) {    // no valid slot entry found
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
ValidatePspDir (
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
  if (PspDir->Header.Checksum != Fletcher32 ((UINT16 *) &PspDir->Header.TotalEntries, \
      (sizeof (PSP_DIRECTORY_HEADER) - OFFSET_OF (PSP_DIRECTORY_HEADER, TotalEntries)
        + PspDir->Header.TotalEntries * sizeof (PSP_DIRECTORY_ENTRY)) / 2)) {
    return FALSE;
  }

  //for multi-slot AB recovery, we will do more validation
  if (IsPSPL1Dir && IsMultiSlotLayout (PspDir)) {
    SlotCount = PspDir->Header.TotalEntries;
    if (SlotCount == 0 || SlotCount > MAX_IMAGE_SLOT_COUNT) {
        return FALSE;
    } else  {
      for (Index = 0; Index < SlotCount; Index++) {
        if ((DirTypeCompare (PspDir->PspEntry[Index].Type, PSP_REGION_A_DIR) == FALSE) &&
            (DirTypeCompare (PspDir->PspEntry[Index].Type, PSP_REGION_B_DIR) == FALSE)) {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

BOOLEAN
ValidateBiosDir (
  IN       BIOS_DIRECTORY     *BiosDir,
  IN       UINT32     Signature
  )
{
  //Validate Signature
  if (BiosDir->Header.Cookie != Signature) {
    return FALSE;
  }
  //Do the checksum
  if (BiosDir->Header.Checksum != Fletcher32 ((UINT16 *) &BiosDir->Header.TotalEntries, \
      (sizeof (PSP_DIRECTORY_HEADER) - OFFSET_OF (PSP_DIRECTORY_HEADER, TotalEntries)
        + BiosDir->Header.TotalEntries * sizeof (BIOS_DIRECTORY_ENTRY)) / 2)) {
    return FALSE;
  }
  return TRUE;
}


BOOLEAN
GetComboDir (
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
IsZen2OrLater (
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
ShouldFirstGenEfsBeIgnored (
  IN FIRMWARE_ENTRY_TABLEV2      *FirmwareTableBase
  )
{
  BOOLEAN    FirstGenFfsStructure;
  FirstGenFfsStructure = FALSE;
  //check if it's first generation of EFS structure
  if ((FirmwareTableBase->Config & 0xFF) == 0xFF) {
      FirstGenFfsStructure = TRUE;
  }

  if (FirstGenFfsStructure && IsZen2OrLater ()) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
GetEfs (
  IN OUT   FIRMWARE_ENTRY_TABLEV2     *Efs
  )
{
  UINT32                      i;
  UINT32                      j;
  FIRMWARE_ENTRY_TABLEV2      FirmwareTableBase;
  UINT32                      LocationNum;

  ASSERT (Efs != NULL);
  CONST UINT32 RomSigOffsetTable[] =  {
    0x020000,  //  --> 16MB base
    0x820000,  //  --> 8MB base
    0xC20000,  //  --> 4MB base
    0xE20000,  //  --> 2MB base
    0xF20000,  //  --> 1MB base
    0xFA0000   //  --> 512KB base
  };
  if (UseFwId ()) {
    //For system using FwId, only 0x20000 is valid entry
    LocationNum = 1;
  } else {
    LocationNum = sizeof (RomSigOffsetTable) / sizeof (UINT32);
  }
  //search beyond ROM2 to find ROMSIG
  for (i = 0; i < 4; i++) {
    for (j = 0; j < LocationNum; j++) {
      //Get the ROM SIG from SPI ROM
      if (FchSpiRomRead ((RomSigOffsetTable[j] + 0x1000000 * i), (UINT8 *)&FirmwareTableBase,
          sizeof (FIRMWARE_ENTRY_TABLEV2)) != TRUE) {
        ASSERT (FALSE);
        return FALSE;
      }
      // Search flash for unique signature 0x55AA55AA
      if (FirmwareTableBase.Signature  == FIRMWARE_TABLE_SIGNATURE) {
        if (ShouldFirstGenEfsBeIgnored (&FirmwareTableBase) == TRUE) {
          continue;
        }

        CopyMem (Efs, &FirmwareTableBase, sizeof (FIRMWARE_ENTRY_TABLEV2));
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
  This function is to get the PSP level 2 directory buffer.

  @param[out]  pointer to PSP level 2 directory base

  @retval TRUE            Successfully get the valid PSP level 2 directory.
  @retval FALSE           Valid PSP level 2 directory is not found.

**/
BOOLEAN
GetPspLv2DirBaseV2 (
  IN       UINT64             PspLevel2BaseAddress,
  IN OUT   PSP_DIRECTORY     **PspLv2Dir
  )
{
  BOOLEAN                  PspDirValid;
  PSP_DIRECTORY            *PspRegionDir;
  UINT8                    *PspDirBuffer;

  PspDirValid = FALSE;
  PspRegionDir = NULL;
  PspDirBuffer = NULL;
  if (PspLevel2BaseAddress != 0) {
    if (GetHeapBufferByHandle (AMD_PSP_L2_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &PspDirBuffer) != TRUE) {
      return FALSE;
    }

    // Get the PSP Dir
    if (MapSpiDataToBuffer ((UINT32)PspLevel2BaseAddress, PspDirBuffer, MaxPspDirSize) != TRUE) {
      return FALSE;
    }
    PspRegionDir = (PSP_DIRECTORY*)PspDirBuffer;

    PspDirValid = ValidatePspDir (PspRegionDir, PSP_LV2_DIRECTORY_HEADER_SIGNATURE, FALSE);
    if (PspDirValid) {
      *PspLv2Dir = PspRegionDir;
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
GetPspDirBaseV2 (
  IN OUT   PSP_DIRECTORY     **PspDir
  )
{
  UINT32                      i;
  UINT32                      j;
  FIRMWARE_ENTRY_TABLEV2      FirmwareTableBase;
  UINT32                      PspChipId;
  BOOLEAN                     PspDirSigValid;
  UINT8                       *PspDirBuffer;
  UINT32                      PspDirBase;
  DIR_INFO                    *DirInfo;
  UINT32                      LocationNum;

  CONST UINT32 RomSigOffsetTable[] =  {
    0x020000,  //  --> 16MB base
    0x820000,  //  --> 8MB base
    0xC20000,  //  --> 4MB base
    0xE20000,  //  --> 2MB base
    0xF20000,  //  --> 1MB base
    0xFA0000   //  --> 512KB base
  };
  PspChipId = 0;
  PspDirBuffer = NULL;
  PspDirBase = 0;
  DirInfo    = NULL;

  if (GetDirInfoEntry (&DirInfo) == FALSE) {
    //Unsupported Family detected
    ASSERT (FALSE);
    return (FALSE);
  }
  PspChipId = DirInfo->PspId;

  if (UseFwId ()) {
    //For system using FwId, only 0x20000 is valid entry
    LocationNum = 1;
  } else {
    LocationNum = sizeof (RomSigOffsetTable) / sizeof (UINT32);
  }

  //search beyond ROM2 to find ROMSIG
  for (i = 0; i < 4; i++) {
    for (j = 0; j < LocationNum; j++) {
      //Get the ROM SIG from SPI ROM
      if (FchSpiRomRead ((RomSigOffsetTable[j] + 0x1000000 * i), (UINT8 *)&FirmwareTableBase,
          sizeof (FIRMWARE_ENTRY_TABLEV2)) != TRUE) {
        ASSERT (FALSE);
        return FALSE;
      }
      // Search flash for unique signature 0x55AA55AA
      if (FirmwareTableBase.Signature  == FIRMWARE_TABLE_SIGNATURE) {
        if (ShouldFirstGenEfsBeIgnored (&FirmwareTableBase) == TRUE) {
          continue;
        }
        if (GetHeapBufferByHandle (AMD_PSP_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &PspDirBuffer) != TRUE) {
          return FALSE;
        }
        if (MapSpiDataToBuffer (FirmwareTableBase.PspDirBase, PspDirBuffer, MaxPspDirSize) != TRUE) {
          return FALSE;
        }
        //Check if PSP combo structure
        if (GetComboDir ((UINT32)(UINTN)PspDirBuffer, PspChipId, PSP_COMBO_DIRECTORY_COOKIE, &PspDirBase) == TRUE) {
          if(MapSpiDataToBuffer(PspDirBase, PspDirBuffer, MaxPspDirSize) != TRUE) {
            return FALSE;
          }
        }
        PspDirSigValid = FALSE;
        PspDirSigValid = ValidatePspDir ((PSP_DIRECTORY *) (UINTN)PspDirBuffer, PSP_DIRECTORY_HEADER_SIGNATURE, TRUE);
        if (PspDirSigValid) {
          *PspDir = (PSP_DIRECTORY *) (UINTN)PspDirBuffer;
          return TRUE;
        } else {
          if (DirInfo->IshStructVersion == ISH_VERSION_2) {
            //Start from ISH version 2, it start to support PspDirBackupBase
            if (MapSpiDataToBuffer (FirmwareTableBase.PspDirBackupBase, PspDirBuffer, MaxPspDirSize) != TRUE) {
              return FALSE;
            }
            PspDirSigValid = ValidatePspDir ((PSP_DIRECTORY *) (UINTN)PspDirBuffer,
                                              PSP_DIRECTORY_HEADER_SIGNATURE, TRUE);
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
    }
  }
  return (FALSE);
}

/**
  This function is to get the BIOS level 2 directory buffer.

  @param[out]  pointer to BIOS level 2 directory base

  @retval TRUE            Successfully get the valid BIOS directory base address.
  @retval FALSE           Valid BIOS directory is not found.

**/
BOOLEAN
GetBiosLv2DirBaseV2 (
  IN       UINT64             BiosLevel2BaseAddress,
  IN OUT   BIOS_DIRECTORY     **BiosLv2Dir
  )
{
  BOOLEAN               BiosDirValid;
  UINT8                 *BiosDirBuffer;
  BIOS_DIRECTORY        *BiosDir;

  BiosDirValid = FALSE;
  BiosDirBuffer = NULL;
  BiosDir = NULL;

  if (BiosLevel2BaseAddress != 0) {
    // get buffer
    if (GetHeapBufferByHandle (AMD_PSP_L2_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &BiosDirBuffer) != TRUE) {
      return FALSE;
    }

    // Get the Level2 Dir
    if (MapSpiDataToBuffer ((UINT32)BiosLevel2BaseAddress, BiosDirBuffer, MaxPspDirSize) != TRUE) {
      return FALSE;
    }

    BiosDir = (BIOS_DIRECTORY*)BiosDirBuffer;
    BiosDirValid = ValidateBiosDir (BiosDir, BIOS_LV2_DIRECTORY_HEADER_SIGNATURE);

    if (BiosDirValid) {
      *BiosLv2Dir = BiosDir;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  This function is to get the BIOS Level 1 directory.

  @param[out]  pointer to BIOS directory base

  @retval TRUE            Successfully get the valid BIOS directory base address.
  @retval FALSE           Valid BIOS directory is not found.

**/
BOOLEAN
GetBiosDirBaseV2 (
  IN OUT   BIOS_DIRECTORY     **BiosDir
  )
{
  UINT32                      i;
  UINT32                      j;
  FIRMWARE_ENTRY_TABLEV2      FirmwareTableBase;
  UINT32                      PspChipId;
  BOOLEAN                     BiosDirSigValid;
  BOOLEAN                     ComboBisoDirSupport;
  UINT8                      *BiosDirBuffer;
  UINT32                      BiosDirBase;
  UINT32                      BiosDirEfsOffset;
  DIR_INFO                    *DirInfo;
  UINT32                      LocationNum;

  CONST UINT32 RomSigOffsetTable[] =  {
    0x020000,  //  --> 16MB base
    0x820000,  //  --> 8MB base
    0xC20000,  //  --> 4MB base
    0xE20000,  //  --> 2MB base
    0xF20000,  //  --> 1MB base
    0xFA0000   //  --> 512KB base
  };

  PspChipId = 0;
  ComboBisoDirSupport = FALSE;
  BiosDirBuffer = NULL;
  BiosDirBase = 0;
  BiosDirEfsOffset = 0;
  DirInfo = NULL;

  if (GetDirInfoEntry (&DirInfo) == FALSE) {
    //Unsupported Family detected
    ASSERT (FALSE);
    return (FALSE);
  }
  PspChipId = DirInfo->PspId;
  ComboBisoDirSupport = DirInfo->ComboBisoDirSupport;
  BiosDirEfsOffset = DirInfo->BiosDirEfsOffset;
  if (UseFwId ()) {
    //For system using FwId, only 0x20000 is valid entry
    LocationNum = 1;
  } else {
    LocationNum = sizeof (RomSigOffsetTable) / sizeof (UINT32);
  }
  //search beyond ROM2 to find ROMSIG
  for (i = 0; i < 4; i++) {
    for (j = 0; j < LocationNum; j++) {
      //Get the ROM SIG from SPI ROM
      if (FchSpiRomRead ((RomSigOffsetTable[j] + 0x1000000 * i), (UINT8 *)&FirmwareTableBase,
                          sizeof (FIRMWARE_ENTRY_TABLEV2)) != TRUE) {
        return FALSE;
      }
      // Search flash for unique signature 0x55AA55AA
      if (FirmwareTableBase.Signature  == FIRMWARE_TABLE_SIGNATURE) {
        if (ShouldFirstGenEfsBeIgnored (&FirmwareTableBase) == TRUE) {
            continue;
         }

        BiosDirBase = *((UINT32 *) ((UINT8 *)&FirmwareTableBase + BiosDirEfsOffset));
        if (GetHeapBufferByHandle (AMD_PSP_DIRECTORY_BUFFER_HANDLE, MaxPspDirSize, &BiosDirBuffer) != TRUE) {
          return FALSE;
        }

        //If support Combo BIOS directory, try to get from combo 1st
        if (ComboBisoDirSupport) {
          //Get the BIOS COMBO Dir
          if (MapSpiDataToBuffer (FirmwareTableBase.NewBiosDirBase, BiosDirBuffer, MaxPspDirSize) != TRUE) {
            return FALSE;
          }
          GetComboDir ((UINT32)(UINTN)BiosDirBuffer, PspChipId, BIOS_COMBO_DIRECTORY_COOKIE, &BiosDirBase);
        }

        //Get the BIOS Dir
        if (MapSpiDataToBuffer (BiosDirBase, BiosDirBuffer, MaxPspDirSize) != TRUE) {
          return FALSE;
        }

        BiosDirSigValid = FALSE;
        BiosDirSigValid = ValidateBiosDir ((BIOS_DIRECTORY *) (UINTN)BiosDirBuffer, BIOS_DIRECTORY_HEADER_SIGNATURE);
        ASSERT (BiosDirSigValid);
        if (BiosDirSigValid) {
          *BiosDir = (BIOS_DIRECTORY *)(UINTN)BiosDirBuffer;
          return TRUE;
        } else {
          return FALSE;
        }
      }
    }
  }
  return (FALSE);
}

/**
  This function is to check if PSP AB recovery layout is supported or not,
  no matter legacy A/B recovery(RN) or multiple slots A/B recovery layout (VN & MR)

  @param[in]   pointer to PSP directory base
  @param[out]  pointer to PSP region A base
  @param[out]  pointer to PSP region B base

  @retval TRUE            PSP AB recovery is supported
  @retval FALSE           PSP AB recovery is not supported

**/
BOOLEAN
IsABrecovery (
  IN         PSP_DIRECTORY               *PspDir,
  IN OUT     UINT64                      *PspRegionAEntryAddress,
  IN OUT     UINT64                      *PspRegionBEntryAddress
)
{
  UINT64                  TempPspRegionAEntryAddress = 0;
  UINT64                  TempPspRegionBEntryAddress = 0;
  BOOLEAN                 PspRegionAEntryExist = FALSE;
  BOOLEAN                 PspRegionBEntryExist = FALSE;
  UINTN                   i;
  IMAGE_SLOT_INFO         *Slot;

  if (PspDir == NULL) {
    return FALSE;
  }
  if (PspDir->Header.TotalEntries > TOTAL_ENTRIES_MAX) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP Entries exceeds the limit\n");
    return FALSE;
  }
  if (IsMultiSlotLayout (PspDir)) {
    if (GetImageSlotInfo (PspDir, &Slot) == TRUE) {
      *PspRegionAEntryAddress = Slot->SlotAHeader.ImageSlotAddr;
      *PspRegionBEntryAddress = Slot->SlotBHeader.ImageSlotAddr;
      return TRUE;
    }
  } else {
     //Check Entry 0x48, 0x4A
    for (i = 0; i < PspDir->Header.TotalEntries; i++) {
      if (DirTypeCompare (PspDir->PspEntry[i].Type, PSP_REGION_A_DIR)) {
        TempPspRegionAEntryAddress = PspDir->PspEntry[i].Location;
        PspRegionAEntryExist = TRUE;
      } else if (DirTypeCompare (PspDir->PspEntry[i].Type, PSP_REGION_B_DIR)) {
        TempPspRegionBEntryAddress = PspDir->PspEntry[i].Location;
        PspRegionBEntryExist = TRUE;
      }

      //AB recovery must have both type PSP_REGION_A_DIR and PSP_REGION_B_DIR
      if (PspRegionAEntryExist && PspRegionBEntryExist) {
        *PspRegionAEntryAddress = TempPspRegionAEntryAddress;
        *PspRegionBEntryAddress = TempPspRegionBEntryAddress;
        return TRUE;
      }
    }
  }
  return FALSE;
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
GetLevel2PSPEntryInfo (
  IN       UINT32                      EntryType,
  IN       UINT64                      PspLevel2BaseAddress,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize
  )
{
  UINTN                    i;
  PSP_DIRECTORY_ENTRY_TYPE EntryTypeValue;
  PSP_DIRECTORY            *PspRegionDir;

  EntryTypeValue.Value = EntryType;

  if (GetPspLv2DirBaseV2 (PspLevel2BaseAddress, &PspRegionDir) == TRUE) {
  // Loop and compare the EntryType
    for (i = 0; i < PspRegionDir->Header.TotalEntries; i++) {
      if (UseFwId ()) {
        if (PspRegionDir->PspEntry[i].Type.FwIdField.FwId == EntryTypeValue.FwIdField.FwId) {
          *EntryAddress = TranslateEntryLocation (PspRegionDir->PspEntry[i].Location, PspLevel2BaseAddress,
                                                  (UINT32)(PspLevel2BaseAddress & 0xFFFFFFFF));
          *EntrySize = PspRegionDir->PspEntry[i].Size;
          return (TRUE);
        }
      } else {
        if ((PspRegionDir->PspEntry[i].Type.Field.Type == EntryTypeValue.Field.Type) &&
          (PspRegionDir->PspEntry[i].Type.Field.SubProgram == EntryTypeValue.Field.SubProgram) &&
          (PspRegionDir->PspEntry[i].Type.Field.Instance == EntryTypeValue.Field.Instance)) {
          *EntryAddress = TranslateEntryLocation (PspRegionDir->PspEntry[i].Location, PspLevel2BaseAddress,
                              (UINT32)(PspLevel2BaseAddress & 0xFFFFFFFF));
          *EntrySize = PspRegionDir->PspEntry[i].Size;
          return (TRUE);
        }
      }
    }
  }
  return (FALSE);
}

/**
  This function is to get the PSP entry information for given PSP entry type.

  @param[in]   value of given PSP entry type
  @param[out]  pointer to PSP entry address
  @param[out]  pointer to PSP entry size

  @retval TRUE            The given entry type is found
  @retval FALSE           The given entry type is not found

**/
BOOLEAN
EFIAPI
PSPEntryInfoV2 (
  IN       UINT32                      EntryType,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize
  )
{
  PSP_DIRECTORY               *PspDir;
  UINT64                      PspLevel2BaseAddress;
  UINTN                       i;
  BOOLEAN                     Lv2DirExist;
  BOOLEAN                     ABrecoveryEnable;
  UINT64                      Level2EntryAddress;
  UINT64                      PspRegionAEntryAddress;
  UINT64                      PspRegionBEntryAddress;
  UINT32                      IgnoredEntrySize;
  PSP_DIRECTORY_ENTRY_TYPE    EntryTypeValue;
  BOOLEAN                     EntryFound;

  PspDir = NULL;
  PspLevel2BaseAddress = 0;
  Lv2DirExist = FALSE;
  ABrecoveryEnable = FALSE;
  Level2EntryAddress = 0;
  PspRegionAEntryAddress = 0;
  PspRegionBEntryAddress = 0;
  EntryTypeValue.Value = EntryType;

  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    return FALSE;
  }
  if (PspDir == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE (" PSPDir is NULL \n");
    return FALSE;
  }


  // Check if A/B recovery schema 1st, as A/B has different search logic
  if ((EntryType != PSP_REGION_A_DIR) && (EntryType != PSP_REGION_B_DIR)) {
    ABrecoveryEnable = IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress);
  }

  //System using firmware id only support A/B recovery scheme
  if (ABrecoveryEnable) {
    // Check read from Partition A or B by reading recovery flag
    if (CheckPspRecoveryFlagSmn () == FALSE) {
      // Get L2A Dir
      PspLevel2BaseAddress = PspRegionAEntryAddress;
    } else {
      // Get L2B Dir
      PspLevel2BaseAddress = PspRegionBEntryAddress;
    }
  } else {
    // Check the existence of Level 2 DIR for non-AB recovery mode
    if (EntryType != PSP_DIR_LV2) {
      Lv2DirExist = PSPEntryInfoV2 (PSP_DIR_LV2, &Level2EntryAddress, &IgnoredEntrySize);
    }

    // Try to load entry from level2 if exist
    if (Lv2DirExist) {
      PspLevel2BaseAddress = Level2EntryAddress;
    }
  }

  if (PspLevel2BaseAddress != 0) {
    if (GetLevel2PSPEntryInfo (EntryType, PspLevel2BaseAddress, EntryAddress, EntrySize) == TRUE) {
      return (TRUE);
    }
  }

  EntryFound = FALSE;
  // If level 2 not exists, or can't find the entry in level 2, try in Level 1
  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    if (UseFwId ()) {
      if (PspDir->PspEntry[i].Type.FwIdField.FwId == EntryTypeValue.FwIdField.FwId) {
        EntryFound = TRUE;
      }
    } else {
      if ((PspDir->PspEntry[i].Type.Field.Type == EntryTypeValue.Field.Type) &&
        (PspDir->PspEntry[i].Type.Field.SubProgram == EntryTypeValue.Field.SubProgram) &&
        (PspDir->PspEntry[i].Type.Field.Instance == EntryTypeValue.Field.Instance)) {
        EntryFound = TRUE;
      }
    }
    if (EntryFound) {
      *EntryAddress = TryToTranslateOffsetToPhysicalAddress (PspDir->PspEntry[i].Location);
      *EntrySize = PspDir->PspEntry[i].Size;
      return (TRUE);
    }
  }
  return FALSE;
}

/**
 * Get PSP Entry SCS binary information
 *
 *
 *
 * @param[in,out] EntryAddress        Address of SCS binary
 * @param[in,out] EntrySize           Size of the SCS binary
 */
BOOLEAN
GetPspEntryScsBinaryV2 (
  IN OUT   UINT64                      *EntryAddress,
  IN       UINT32                      *EntrySize
  )
{
  return PSPEntryInfoV2 (AMD_SCS_BINARY, EntryAddress, EntrySize);
}

/**
 * Get PSP Entry PSP NV data information
 *
 *
 *
 * @param[in,out] EntryAddress        Address of PSP NV data
 * @param[in,out] EntrySize           Size of the PSP NV data
 */
BOOLEAN
GetPspEntryPspNvDataV2 (
  IN OUT   UINT64                      *EntryAddress,
  IN       UINT32                      *EntrySize
  )
{
  return PSPEntryInfoV2 (PSP_NV_DATA, EntryAddress, EntrySize);
}

/**
 *
 *  Get BIOS Directory Entry 's properties from Level 2
 *
 *  @param[in]     EntryType        BIOS Directory Entry type
 *  @param[in]     EntryInstance    If input with INSTANCE_IGNORED, will return 1st Entry type matched
 *                                  If input with Non INSTANCE_IGNORED,
 *                                  will return the entry which both Type & Instance matched
 *  @param[in]     SubProgram       If input with SUBPROGRAM_IGNORED, will return 1st Entry type & Instance matched
 *                                  If input with Non SUBPROGRAM_IGNORED,
 *                                  will return the entry which all Type & Instance & SubProgram matched
 *  @param[in]     DirectoryType    For programs with multiple slot layout(VN/MR/RMB), specify DIR_TYPE_PSP_LV2,
 *                                  it will find BIOS entry in PSP L2,
                                    For programs without multiple slot layout(RN/CZN/MTS), specify DIR_TYPE_BIOS_LV2,
                                    it will find BIOS entry in BIOS L2.
                                    Note that in VN/MR, BIOS L2 is part of PSP L2 directory
 *  @param[in]     Level2BaseAddress  if DirectoryType=DIR_TYPE_PSP_LV2,
                                      please specify PSP L2 base address(only for VN/MR and beyond),
 *                                    if DirectoryType=DIR_TYPE_BIOS_LV2,
                                      please specify BIOS L2 base address(for programs before VN, such as RN/CZN/MTS)
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
GetLevel2BIOSEntryInfo (
  IN       UINT32                      EntryType,
  IN       UINT8                       EntryInstance,
  IN       UINT8                       SubProgram,
  IN       DIRECTORY_TYPE              DirectoryType,
  IN       UINT64                      Level2BaseAddress,
  IN OUT   TYPE_ATTRIB                 *TypeAttrib,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize,
  IN OUT   UINT64                      *EntryDest
  )
{
  UINTN                 i;
  BOOLEAN               Found;
  BIOS_DIRECTORY        *BiosLevel2Dir;
  UINT64                BIOSLevel2BaseAddress;
  UINT32                IgnoredEntrySize;
  UINT32                ImageSlotAddr;
  UINT32                BiosDirEntryId;
  Found = FALSE;
  BIOSLevel2BaseAddress = 0;
  IgnoredEntrySize = 0;
  ImageSlotAddr = 0;

  // Get BIOS L2 base address
  if (DirectoryType == DIR_TYPE_PSP_LV2) {
    ImageSlotAddr = (UINT32)Level2BaseAddress;
    // if BIOS L2 is part of PSP directory L2, try to get BIOS L2 base address
    if (UseFwId ()) {
      BiosDirEntryId = ConvertEntryTypeToFwId (BIOS_REGION_DIR);
    } else {
      BiosDirEntryId = BIOS_REGION_DIR;
    }
    if ((!GetLevel2PSPEntryInfo (BiosDirEntryId, Level2BaseAddress, &BIOSLevel2BaseAddress,
                                &IgnoredEntrySize)) == TRUE) {
      return FALSE;
    }
  } else {
    BIOSLevel2BaseAddress = Level2BaseAddress;
  }

  if (GetBiosLv2DirBaseV2 (BIOSLevel2BaseAddress, &BiosLevel2Dir) == TRUE) {
    for (i = 0; i < BiosLevel2Dir->Header.TotalEntries; i++) {
      if (BiosLevel2Dir->BiosEntry[i].TypeAttrib.Type == EntryType) {
        if (EntryInstance == INSTANCE_IGNORED) {
           if ((SubProgram == SUBPROGRAM_IGNORED) ||
               (SubProgram == BiosLevel2Dir->BiosEntry[i].TypeAttrib.SubProgram)) {
               Found = TRUE;
           }
        } else if (BiosLevel2Dir->BiosEntry[i].TypeAttrib.Instance == EntryInstance) {
           if ((SubProgram == SUBPROGRAM_IGNORED) ||
               (SubProgram == BiosLevel2Dir->BiosEntry[i].TypeAttrib.SubProgram)) {
               Found = TRUE;
           }
        }
        if (Found == TRUE) {
          *TypeAttrib = BiosLevel2Dir->BiosEntry[i].TypeAttrib;
          *EntrySize = BiosLevel2Dir->BiosEntry[i].Size;
          *EntryDest = BiosLevel2Dir->BiosEntry[i].Destination;
          *EntryAddress = TranslateEntryLocation (BiosLevel2Dir->BiosEntry[i].Location, BIOSLevel2BaseAddress,
                                                  ImageSlotAddr);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

/**
 *
 *  Get BIOS Directory Entry 's properties by 3 Attributes: EntryType, EntryInstance, SubProgram.
 *
 *  @param[in]     EntryType        BIOS Directory Entry type
 *  @param[in]     EntryInstance    If input with INSTANCE_IGNORED, will return 1st Entry type matched
 *                                  If input with Non INSTANCE_IGNORED,
 *                                  will return the entry which both Type & Instance matched
 *  @param[in]     SubProgram       If input with SUBPROGRAM_IGNORED, will return 1st Entry type & Instance matched
 *                                  If input with Non SUBPROGRAM_IGNORED,
 *                                  will return the entry which all Type & Instance & SubProgram matched
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
BIOSEntryInfoByAttributes (
  IN       UINT8                       EntryType,
  IN       UINT8                       EntryInstance,
  IN       UINT8                       SubProgram,
  IN OUT   TYPE_ATTRIB                 *TypeAttrib,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize,
  IN OUT   UINT64                      *EntryDest
  )
{
  BIOS_DIRECTORY          *BiosDir;
  UINT64                  BiosLevel2BaseAddress;
  PSP_DIRECTORY           *PspDir;
  UINTN                   i;
  BOOLEAN                 Found;
  BOOLEAN                 ABrecoveryEnable;
  UINT64                  Level2EntryAddress;
  UINT64                  BiosRegionEntryAddress;
  UINT64                  PspRegionAEntryAddress;
  UINT64                  PspRegionBEntryAddress;
  TYPE_ATTRIB             IgnoredTypeAttrib;
  UINT32                  IgnoredEntrySize;
  UINT64                  IgnoredEntryDest;
  UINT32                  ActiveImageSlotAddr;

  BiosDir = NULL;
  BiosLevel2BaseAddress = 0;
  PspDir = NULL;
  ABrecoveryEnable = FALSE;
  Level2EntryAddress = 0;
  BiosRegionEntryAddress = 0;
  PspRegionAEntryAddress = 0;
  PspRegionBEntryAddress = 0;
  Found = FALSE;
  ActiveImageSlotAddr = 0;

  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    return FALSE;
  }

  //Check if A/B recovery schema 1st, as A/B has different search logic
  ABrecoveryEnable = IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress);
  if (ABrecoveryEnable) {
    // For multiple slot layout(VN, MR), the BIOS L2 directory is part of PSP L2, we can find BIOS entry from PSP L2
    if (IsMultiSlotLayout (PspDir)) {
      if (CheckPspRecoveryFlagSmn () == FALSE) {
        //Get L2A Dir
        ActiveImageSlotAddr = (UINT32)(PspRegionAEntryAddress & 0xFFFFFFFF);
      } else {
        //Get L2B Dir
        ActiveImageSlotAddr = (UINT32)(PspRegionBEntryAddress & 0xFFFFFFFF);
      }
    } else {
       // For legacy AB recovery like RN/CZN, we still need to find BIOS entry from BIOS directory L2,
       //and BIOS L2 base (0x49) is from Psp Directory instead of EFS (RomSig)
      if (PSPEntryInfoV2 (BIOS_REGION_DIR, &BiosRegionEntryAddress, &IgnoredEntrySize) == TRUE) {
        BiosLevel2BaseAddress = BiosRegionEntryAddress;
      }
    }
  } else if (GetBiosDirBaseV2 (&BiosDir) == TRUE) {
    //Check the existence of Level 2 DIR
    if (EntryType != BIOS_DIR_LV2) {
      if (BIOSEntryInfo (BIOS_DIR_LV2, INSTANCE_IGNORED, &IgnoredTypeAttrib, &Level2EntryAddress, &IgnoredEntrySize,
                         &IgnoredEntryDest) == TRUE) {
        BiosLevel2BaseAddress = Level2EntryAddress;
      }
    }
  }

  //find entry in level 2
  if (ActiveImageSlotAddr != 0)
  {
    if (GetLevel2BIOSEntryInfo (EntryType, EntryInstance, SubProgram,
        DIR_TYPE_PSP_LV2, ActiveImageSlotAddr,
        TypeAttrib, EntryAddress, EntrySize, EntryDest) == TRUE) {
        return (TRUE);
    }
  } else if (BiosLevel2BaseAddress != 0) {
    if (GetLevel2BIOSEntryInfo (EntryType, EntryInstance, SubProgram,
        DIR_TYPE_BIOS_LV2, BiosLevel2BaseAddress,
        TypeAttrib, EntryAddress, EntrySize, EntryDest) == TRUE) {
        return (TRUE);
    }
  }

  //If level 2 not exists, or can't find the entry in level 2, try in Level 1
  if (BiosDir != NULL) {
    for (i = 0; i < BiosDir->Header.TotalEntries; i++) {
      if (BiosDir->BiosEntry[i].TypeAttrib.Type == EntryType) {
        if (EntryInstance == INSTANCE_IGNORED) {
            if ((SubProgram == SUBPROGRAM_IGNORED) || (SubProgram == BiosDir->BiosEntry[i].TypeAttrib.SubProgram)) {
                Found = TRUE;
            }
        } else if (BiosDir->BiosEntry[i].TypeAttrib.Instance == EntryInstance) {
            if ((SubProgram == SUBPROGRAM_IGNORED) || (SubProgram == BiosDir->BiosEntry[i].TypeAttrib.SubProgram)) {
                Found = TRUE;
            }
        }
        if (Found == TRUE) {
          *TypeAttrib = BiosDir->BiosEntry[i].TypeAttrib;
          *EntryAddress = TryToTranslateOffsetToPhysicalAddress (BiosDir->BiosEntry[i].Location);
          *EntrySize = BiosDir->BiosEntry[i].Size;
          *EntryDest = BiosDir->BiosEntry[i].Destination;
          return (TRUE);
        }
      }
    }
  }
  return (FALSE);
}

/**
 *
 *  Get BIOS Directory Entry 's properties by EntryType and EntryInstance
 *  This function will ignore SubProgram, if you care about SubProgram, call BIOSEntryInfoByAttributes instead.
 *
 *  @param[in]     EntryType    BIOS Directory Entry type
 *  @param[in]     EntryInstance    If input with INSTANCE_IGNORED, will return 1st Entry type matched
 *                                  If input with Non INSTANCE_IGNORED,
 *                                  will return the entry which both Type & Instance matched
 *  @param[in,out] TypeAttrib   TypeAttrib of entry
 *  @param[in,out] EntryAddress Address of entry
 *  @param[in,out] EntrySize    Size of entry
 *  @param[in,out] EntryDest    Destination of entry
 *
 *  @retval TRUE   Success to get the Entry 's properties
 *  @retval FALSE  Fail to get the Entry 's properties
 *
 **/
BOOLEAN
BIOSEntryInfo (
  IN       UINT8                       EntryType,
  IN       UINT8                       EntryInstance,
  IN OUT   TYPE_ATTRIB                 *TypeAttrib,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize,
  IN OUT   UINT64                      *EntryDest
  )
{
  return BIOSEntryInfoByAttributes (EntryType, EntryInstance, SUBPROGRAM_IGNORED, TypeAttrib, EntryAddress, EntrySize,
                                    EntryDest);
}

BOOLEAN
GetPspLv2BaseAddr (
  IN OUT UINT64                      *PspLv2BaseAddress
  )
{
  PSP_DIRECTORY               *PspDir;
  BOOLEAN                     ABrecoveryEnable;
  UINT64                      Level2EntryAddress;
  UINT64                      PspRegionAEntryAddress;
  UINT64                      PspRegionBEntryAddress;
  UINT32                      IgnoredEntrySize;

  PspDir = NULL;
  ABrecoveryEnable = FALSE;
  Level2EntryAddress = 0;
  PspRegionAEntryAddress = 0;
  PspRegionBEntryAddress = 0;


  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    IDS_HDT_CONSOLE_PSP_TRACE (" Cannot get PSPDir \n");
    return FALSE;
  }
  if (PspDir == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE (" PSPDir is NULL \n");
    return FALSE;
  }
  // Check if A/B recovery schema 1st, as A/B has different search logic
  ABrecoveryEnable = IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress);
  if (ABrecoveryEnable) {
    // Check read from Partition A or B by reading recovery flag
    if (CheckPspRecoveryFlagSmn () == FALSE) {
      // Get L2A Dir
      *PspLv2BaseAddress = PspRegionAEntryAddress;
      return TRUE;
    } else {
      // Get L2B Dir
      *PspLv2BaseAddress = PspRegionBEntryAddress;
      return TRUE;
    }
  } else {
    // Check the existence of Level 2 DIR for non-AB recovery mode
    if (PSPEntryInfoV2 (PSP_DIR_LV2, &Level2EntryAddress, &IgnoredEntrySize) == TRUE) {
      *PspLv2BaseAddress = Level2EntryAddress;
      return TRUE;
    }
  }
  return FALSE;
}

BOOLEAN
GetPspBiosLv2BaseAddr (
  IN OUT UINT64                      *PspLv2BaseAddress,
  IN OUT UINT64                      *BiosLv2BaseAddress
  )
{
  PSP_DIRECTORY               *PspDir;
  BOOLEAN                     PspLv2BaseAddrFound;
  BOOLEAN                     BiosLv2BaseAddrFound;
  BOOLEAN                     ABrecoveryEnable;
  UINT64                      Level2EntryAddress;
  UINT64                      PspRegionAEntryAddress;
  UINT64                      PspRegionBEntryAddress;
  UINT32                      IgnoredEntrySize;

  UINT64                      BiosRegionEntryAddress;
  TYPE_ATTRIB                 IgnoredTypeAttrib;
  UINT64                      IgnoredEntryDest;
  UINT32                      BiosDirEntryId;

  PspDir = NULL;
  PspLv2BaseAddrFound = FALSE;
  BiosLv2BaseAddrFound = FALSE;
  ABrecoveryEnable = FALSE;
  Level2EntryAddress = 0;
  PspRegionAEntryAddress = 0;
  PspRegionBEntryAddress = 0;
  BiosRegionEntryAddress = 0;

  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    IDS_HDT_CONSOLE_PSP_TRACE (" Cannot get PSPDir \n");
    return FALSE;
  }
  if (PspDir == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE (" PSPDir is NULL \n");
    return FALSE;
  }
  // Check if A/B recovery schema 1st, as A/B has different search logic
  ABrecoveryEnable = IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress);
  if (ABrecoveryEnable) {
    // Check read from Partition A or B by reading recovery flag
    if (CheckPspRecoveryFlagSmn () == FALSE) {
      // Get L2A Dir
      *PspLv2BaseAddress = PspRegionAEntryAddress;
      PspLv2BaseAddrFound = TRUE;
    } else {
      // Get L2B Dir
      *PspLv2BaseAddress = PspRegionBEntryAddress;
      PspLv2BaseAddrFound = TRUE;
    }

    // For legacy AB recovery like RN/CZN, we still need to find BIOS entry from BIOS directory L2,
    // and BIOS L2 base (0x49) is from Psp Directory
    if (UseFwId ()) {
      BiosDirEntryId = ConvertEntryTypeToFwId (BIOS_REGION_DIR);
    } else {
      BiosDirEntryId = BIOS_REGION_DIR;
    }

    if (PSPEntryInfoV2 (BiosDirEntryId, &BiosRegionEntryAddress, &IgnoredEntrySize) == TRUE) {
      *BiosLv2BaseAddress = BiosRegionEntryAddress;
      BiosLv2BaseAddrFound = TRUE;
    }
  } else {
    // Check the existence of Level 2 DIR for non-AB recovery mode
    if (PSPEntryInfoV2 (PSP_DIR_LV2, &Level2EntryAddress, &IgnoredEntrySize) == TRUE) {
      *PspLv2BaseAddress = Level2EntryAddress;
      PspLv2BaseAddrFound = TRUE;
    }

    // Check the existence of Level 2 DIR
    if (BIOSEntryInfo (BIOS_DIR_LV2, INSTANCE_IGNORED, &IgnoredTypeAttrib, &Level2EntryAddress, &IgnoredEntrySize,
                        &IgnoredEntryDest) == TRUE) {
      *BiosLv2BaseAddress = Level2EntryAddress;
      BiosLv2BaseAddrFound = TRUE;
    }
  }
  return (PspLv2BaseAddrFound && BiosLv2BaseAddrFound);
}

/**
  Check PSP Recovery Flag using SMN to remove MMIO initial dependency
  Target will set Recovery flag if some PSP entry point by PSP directory has been corrupted.

  @retval BOOLEAN  0: Recovery Flag is cleared, 1: Recovery Flag has been set

**/
BOOLEAN
CheckPspRecoveryFlagSmn (
  VOID
  )
{
  PSP_MBOX_V2_CMD PspMboxCmd;

  if (PcdGetBool (PcdAmdPspRecoveryFlagDetectEnable) == FALSE) {
      return FALSE;
  }

  SmnRegisterRead (0, MP0_C2PMSG_28_SMN_ADDR,  (VOID *) &PspMboxCmd.Value);
  IDS_HDT_CONSOLE_PSP_TRACE ("RecoveryFlag %x\n", PspMboxCmd.Value);
  ASSERT (PspMboxCmd.Value != 0xFFFFFFFF);
  return (BOOLEAN) (PspMboxCmd.Field.Recovery);
}

/**
  Return the PSP Entry Address of given PSP entry type and level

  @param[in]        EntryType             Value of given PSP entry type, for system using fw id defined in PSP_DIRECTORY_FW_ID,
                                          or else PSP_DIRECTORY_ENTRY_TYPE_ID
  @param[in]        IsLevel2              The given entry is in PSP level 2 directory
  @param[in]        ActiveImageSlotAddr   The active image slot address, used starting from VN/MR
  @param[in, out]   EntryAddress          Pointer to PSP entry address

  @retval TRUE      The given entry type is found
  @retval FALSE     The given entry type is not found
**/
BOOLEAN
GetPspEntryAddress (
  IN      UINT16     EntryType,
  IN      BOOLEAN   IsLevel2,
  IN OUT  UINT64    *EntryAddress
  )
{
  PSP_DIRECTORY               *PspDir = NULL;
  UINTN                       i;
  UINT64                      Level2EntryAddress = 0;
  UINT32                      IgnoredEntrySize = 0;
  UINT64                      PspLevel2BaseAddress = 0;
  BOOLEAN                     Lv2DirExist = FALSE;
  BOOLEAN                     ABrecoveryEnable = FALSE;
  UINT64                      PspRegionAEntryAddress = 0;
  UINT64                      PspRegionBEntryAddress = 0;
  PSP_DIRECTORY_ENTRY_TYPE    DirEntryType;

  DirEntryType.Value = EntryType;
  if (GetPspDirBaseV2 (&PspDir) != TRUE) {
    return FALSE;
  }

  if (IsLevel2){
    // Check if A/B recovery schema 1st, as A/B has different search logic
    if ((DirTypeCompare (DirEntryType, PSP_REGION_A_DIR) == FALSE) &&
        (DirTypeCompare (DirEntryType, PSP_REGION_B_DIR) == FALSE)) {
      ABrecoveryEnable = IsABrecovery (PspDir, &PspRegionAEntryAddress, &PspRegionBEntryAddress);
    }

    if (ABrecoveryEnable) {
      //Check read from Partition A or B by reading recovery flag
      if (CheckPspRecoveryFlagSmn () == FALSE) {
        //Get L2A Dir
        PspLevel2BaseAddress = PspRegionAEntryAddress;
      } else {
        //Get L2B Dir
        PspLevel2BaseAddress = PspRegionBEntryAddress;
      }
    } else {
      //Check the existence of Level 2 DIR for non-AB recovery mode
      if (EntryType != PSP_DIR_LV2) {
        Lv2DirExist = PSPEntryInfoV2 (PSP_DIR_LV2, &Level2EntryAddress, &IgnoredEntrySize);
      }

      //Try to load entry from level2 if exist
      if (Lv2DirExist) {
        PspLevel2BaseAddress = Level2EntryAddress;
      }
    }

    if (PspLevel2BaseAddress != 0) {
      if (GetLevel2PSPEntryInfo (EntryType, PspLevel2BaseAddress, EntryAddress, &IgnoredEntrySize) == TRUE) {
        return (TRUE);
      }
    }

  } else {
    for (i = 0; i < PspDir->Header.TotalEntries; i++) {
      if (PspDir->PspEntry[i].Type.Field.Type == EntryType){
        *EntryAddress = TryToTranslateOffsetToPhysicalAddress (PspDir->PspEntry[i].Location);
        return TRUE;
      }
    }
  }

  return FALSE;
}
