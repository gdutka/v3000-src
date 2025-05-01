/*
 ******************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_AB_RECOVERY_BASE_LIB_H_
#define _AMD_AB_RECOVERY_BASE_LIB_H_

#include <AMD.h>
#include <AmdPspDirectory.h>
#include <Library/AmdPspBaseLibV2.h>

#define PSPL1DirectoryTableEntry    1
#define PSPL2DirectoryTableEntry    2
#define BIOSL2DirectoryTableEntry   3
#define PSPL1DirectoryHeader        4
#define ImageSlotHeader             7

#define MASK_SPIADDR_BIT24(SpiAddress) (SpiAddress & 0xFFFFFF)
#define BACKUP_ISHA_OFFSET          0x100

#define APOB_ENTRY                  0x61
#define PEI_ENTRY                   0x62
#define MAX_SLOTS                   0x8
#define UINT_MAX                    0xFFFFFFFF       /* Maximum unsigned value */

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
  UINT32                  FamilyRawId;          //CpuId.0x80000001.EAX_Reg & RAW_FAMILY_ID_MASK;
  UINT32                  PspId;                //Id defined by bootrom
  UINT32                  BiosDirEfsOffset;     //Used in the previous programs, when combo BIOS dir and A/B recovery is not supported, check BiosDirBase in FIRMWARE_ENTRY_TABLEV2
  BOOLEAN                 ComboBisoDirSupport;  //TRUE: Combo Dir supported, FALSE: Combo Dir not supported
  UINT32                  IshStructVersion;     //Version for ISH structure, 1: IMAGE_SLOT_HEADER, 2: IMAGE_SLOT_HEADER_V2, 0xFFFFFFF, ignored
  RECOVERY_REASON_VERSION RecoveryReasonVersion;//Version for Recovery Reason, 1: RECOVERY_REASON_VERSION_1, 2: RECOVERY_REASON_VERSION_2, 0xFFFFFFF, ignored
} DIR_INFO;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


UINT64
TryToTranslateOffsetToPhysicalAddress (
  IN      UINT64                      EntryLocation
  );

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
  );

BOOLEAN
VerifyPspDir (
  IN       PSP_DIRECTORY     *PspDir,
  IN       UINT32            Signature,
  IN       BOOLEAN           IsPSPL1Dir
  );

BOOLEAN
VerifyBiosDir (
  IN       BIOS_DIRECTORY     *BiosDir,
  IN       UINT32     Signature
  );

BOOLEAN
FetchEfs (
  IN OUT   FIRMWARE_ENTRY_TABLEV2     *Efs,
  IN       EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  );

BOOLEAN
FetchPspDirBaseV2 (
  IN OUT   PSP_DIRECTORY     **PspDir,
  IN       EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  );

BOOLEAN
FetchImageSlotInfo (
  IN PSP_DIRECTORY       *PspL1Dir,
  IN OUT IMAGE_SLOT_INFO **Slot,
  IN EFI_PHYSICAL_ADDRESS BiosBaseAddr
);

#define INSTANCE_IGNORED 0xFF
#define SUBPROGRAM_IGNORED 0xFF

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
  );

/**
  This function checks if the PSP L1 is multi slot layout (VN & MR)

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has multi slots (>=2)
  @retval FALSE            the PSP L1 directory has not multi slots (>=2)

**/
BOOLEAN
VerifyMultiSlotLayout (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  );

/**
  This function checks if the PSP L1 is two slot A/B recovery layout (VN)

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has 2 slots with A/B recovery
  @retval FALSE            the PSP L1 directory doesn't have 2 slots with A/B recovery

**/
BOOLEAN
VerifyTwoSlotAbRecovery (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  );

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
  );

BOOLEAN
FetchComboDir (
  IN       UINT32     ComboDirBase,
  IN       UINT32     PspChipId,
  IN       UINT32     ComboSig,
  IN OUT   UINT32     *DirBase
  );

BOOLEAN
GetSharedB
(
    IN PSP_DIRECTORY       *PspL1Dir,
    IN OUT IMAGE_SLOT_INFO **Slot
);

BOOLEAN
IsRMBA0 (
  VOID
);

BOOLEAN
ValidateAddressAndSize(
  IN OUT UINT64*                          SourceEntryAddress,
  IN OUT UINT32*                          SourceEntrySize,
  IN OUT UINT64*                          DestinationEntryAddress,
  IN OUT UINT32*                          DestinationEntrySize
);


#endif // _AMD_AB_RECOVERY_BASE_LIB_H_


