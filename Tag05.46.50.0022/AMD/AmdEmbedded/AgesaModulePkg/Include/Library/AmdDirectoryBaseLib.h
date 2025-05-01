/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
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
#ifndef _AMD_DIRECTORY_BASELIB_H_
#define _AMD_DIRECTORY_BASELIB_H_

#include <AMD.h>
#include <AmdPspDirectory.h>

#define IS_ADDRESS_MODE_1(a) (((a) >> 62) == 1 ? TRUE : FALSE)  //relative to BIOS image base 0
#define IS_ADDRESS_MODE_2(a) (((a) >> 62) == 2 ? TRUE : FALSE)  //relative to current directory header
#define IS_ADDRESS_MODE_3(a) (((a) >> 62) == 3 ? TRUE : FALSE)  //relative to active image slot address (as of now, active image slot address is equal to PSP L2 base address)
#define IS_SPI_OFFSET(a) (((a) & 0xFF000000) != 0xFF000000 ? TRUE : FALSE)

#define MaxDirEntryNumber 64
#define MaxPspDirSize sizeof(PSP_DIRECTORY_HEADER) + (sizeof(BIOS_DIRECTORY_ENTRY) * MaxDirEntryNumber)

#define ALIGNMENT_4K                    BASE_4KB
#define ALIGN_CHECK(addr, alignment)    ((((UINTN)(addr)) & ((alignment) - 1)) == 0)
#define ALIGN_4K_CHECK(addr)            ALIGN_CHECK((addr), ALIGNMENT_4K)

#define IS_VALID_ADDR32(addr)           (((UINT32)(addr) != 0) && (UINT32)(addr) != 0xFFFFFFFF)
#define MaxImageSlotInfoSize      sizeof(IMAGE_SLOT_INFO)

#define MP0PUBLIC0_SMN_ADDR    0x03800000
#define MP0_C2PMSG_28_SMN_ADDR (MP0PUBLIC0_SMN_ADDR + 0x10570)
#define MP0_C2PMSG_29_SMN_ADDR (MP0PUBLIC0_SMN_ADDR + 0x10574)
#define MP0_C2PMSG_30_SMN_ADDR (MP0PUBLIC0_SMN_ADDR + 0x10578)

#pragma pack (push, 1)

/// Directory type
typedef enum _DIRECTORY_TYPE {
  DIR_TYPE_PSP_LV2               = 0,         ///< Level 2 PSP DIR
  DIR_TYPE_BIOS_LV2              = 1,         ///< Level 2 BIOS DIR
} DIRECTORY_TYPE;

/**
 * @brief PSP/BIOS entry region with start address and size
 *
 */
typedef struct {
  UINT64 Address;
  UINT32 Size;
} ENTRY_REGION;

/// RECOVERY_REASON_VERSION
typedef enum {
  RECOVERY_REASON_VERSION_IGNORE = 0xFFFFFFFFul, //before RN
  RECOVERY_REASON_VERSION_1 = 1,   //RN, CZN
  RECOVERY_REASON_VERSION_2 = 2,   //Starting from VN
} RECOVERY_REASON_VERSION;

/**
 * @brief directory level for recovery reason, please refer to RECOVERY_REASON_V2
 *
 */
typedef enum {
  DIRECTORY_LEVEL_PSP_L1_ENTRY = 1,       /// b'01--The entry is from PSP directory L1
  DIRECTORY_LEVEL_PSP_L2_ENTRY = 2,       /// b'10--The entry is from PSP directory L2
  DIRECTORY_LEVEL_BIOS_L2_ENTRY = 3,      /// b'11--The entry is from BIOS directory L2
  DIRECTORY_LEVEL_PSP_L1_HEADER = 4,      /// b'100--PSP L1 directory header
  DIRECTORY_LEVEL_PSP_L2_HEADER = 5,      /// b'101--PSP L2 directory header
  DIRECTORY_LEVEL_BIOS_L2_HEADER = 6,     /// b'110--BIOS directory L2 header
  DIRECTORY_LEVEL_IMAGE_SLOT_HEADER = 7,  /// b'111--Image Slot Header
} RECOVERY_REASON_DIRECTORY_LEVEL;

/// PSP Recovery Reason V1
typedef struct {
  UINT32 EntryType: 16;          ///< [0:15], Entry type ID of the binary in PSP/BIOS entry whose corruption caused recovery
  UINT32 DirectoryLevel: 2;      ///< [16:17],b'01--The entry is from PSP directory L1
                                 ///          b'10--The entry is from PSP directory L2
                                 ///          b'11--The entry is from BIOS directory L2
  UINT32 Instance: 4;            ///< [18:21],the instance number of the corrupted entry
  UINT32 PartitionNumber: 3;     ///< [22:24],Which partition this log is from
  UINT32 Reserved: 7;            ///< [25:31] Reserve for future use
} RECOVERY_REASON_V1;

/// PSP Recovery Reason V2
/// PSP RECOVERY_REASON_V2 structure modified as per PSP v0.28.A0.69
typedef struct {
  UINT32 EntryType: 8;           ///< [0:7],  Entry type ID of the binary in PSP/BIOS entry whose corruption caused recovery
  UINT32 Instance: 4;            ///< [8:11],the instance number of the corrupted entry
  UINT32 SubProgramL: 4;         ///< [12:15], SubProgram Low
  UINT32 DirectoryLevel: 4;      ///< [16:20], refer to RECOVERY_REASON_DIRECTORY_LEVEL
                                 ///          b'01--The entry is from PSP directory L1
                                 ///          b'10--The entry is from PSP directory L2
                                 ///          b'11--The entry is from BIOS directory L2
                                 ///          b'100--PSP L1 directory header
                                 ///          b'101--PSP L2 directory header
                                 ///          b'110--BIOS directory L2 header
                                 ///          b'111--Image Slot Header
  UINT32 Reserved0: 2;           ///< [20:21] Reserve for future use
  UINT32 PartitionNumber: 3;     ///< [22:24],Which partition this log is from
  UINT32 Reserved1: 3;           ///< [25:27] Reserve for future use
  UINT32 SubProgramH: 4;         ///< [28:31] SubProgramH
} RECOVERY_REASON_V2;

/// PSP_DIRECTORY_ID scheme selection
typedef enum {
  PSP_DIRECTORY_ID_USE_ENTRY_TYPE = 1, // Used in most client, server program
  PSP_DIRECTORY_ID_USE_FW_ID      = 2, // Only used in very few programs, e.g. MI300
} PSP_DIRECTORY_ID_SCHEME;

#define ZP_PSP_CHIP_ID   0xBC090000  ///< ZP Chip ID in combo structure
#define RV_PSP_CHIP_ID   0xBC0A0000  ///< RV Chip ID in combo structure
#define RV2_PSP_CHIP_ID  0xBC0A0100  ///< RV2 Chip ID in combo structure
#define SSP_PSP_CHIP_ID  0xBC0B0000  ///< SSP Chip ID in combo structure
#define MTS_PSP_CHIP_ID  0xBC0B0500  ///< MTS Chip ID in combo structure
#define RN_PSP_CHIP_ID   0xBC0C0000  ///< RN Chip ID in combo structure
#define VMR_PSP_CHIP_ID  0xBC0B0500  ///< VMR Chip ID in combo structure
#define GN_PSP_CHIP_ID   0xBC0B0D00  ///< GN Chip ID in combo structure
#define CZN_PSP_CHIP_ID  0xBC0C0140  ///< CZN Chip ID in combo structure
#define BA_PSP_CHIP_ID   0xBC0B0F00  ///< BA Chip ID in combo structure
#define MR_PSP_CHIP_ID   0xBC0B0800  ///< MR Chip ID in combo structure
#define VN_PSP_CHIP_ID   0xBC0B0800  ///< VN Chip ID in combo structure
#define RMB_PSP_CHIP_ID  0xBC0D0200  ///< RMB chip ID
#define RS_PSP_CHIP_ID   0XBC0D0111  ///< Stone chip id
#define RPL_PSP_CHIP_ID  0xBC0D0300  ///< RPL chip ID
#define PHX_PSP_CHIP_ID  0xBC0D0400  ///< PHX chip ID
#define MDN_PSP_CHIP_ID  0xBC0D0900  ///< MDN chip ID
#define MI3_PSP_CHIP_ID  0xBC0D0600  ///< MI300 chip ID

#pragma pack (pop)

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
  );

BOOLEAN
MapSpiDataToBuffer (
  IN       UINT32                      Address,
  IN OUT   VOID                        *Buffer,
  IN       UINT32                      Size
  );

BOOLEAN
ValidatePspDir (
  IN       PSP_DIRECTORY     *PspDir,
  IN       UINT32            Signature,
  IN       BOOLEAN           IsPSPL1Dir
  );

BOOLEAN
ValidateBiosDir (
  IN       BIOS_DIRECTORY     *BiosDir,
  IN       UINT32     Signature
  );

BOOLEAN
GetEfs (
  IN OUT   FIRMWARE_ENTRY_TABLEV2     *Efs
  );

/**
 * @brief translate entry offset to correct location based on address mode
 *
 * @param EntryLocation     the location of the entry before translation
 * @param DirectoryHdrAddr  directory header address
 * @param ImageSlotAddr     image slot address if applicable, if no image slot, leave it as 0
 * @return UINT64           return translated entry location
 */
UINT64
TranslateEntryLocation (
  IN       UINT64                      EntryLocation,
  IN       UINT64                      DirectoryHdrAddr,
  IN       UINT32                      ImageSlotAddr
  );

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
  );

BOOLEAN
GetPspDirBaseV2 (
  IN OUT   PSP_DIRECTORY     **PspDir
  );

/**
  This function is to get the BIOS level 2 directory buffer.

  @param[out]  pointer to BIOS level 2 directory base

  @retval TRUE            Successfully get the valid BIOS level 2 irectory.
  @retval FALSE           Valid BIOS level 2 directory is not found.

**/
BOOLEAN
GetBiosLv2DirBaseV2 (
  IN       UINT64             BiosLevel2BaseAddress,
  IN OUT   BIOS_DIRECTORY     **BiosLv2Dir
  );

BOOLEAN
GetBiosDirBaseV2 (
  IN OUT   BIOS_DIRECTORY     **BiosDir
  );

  /**
  This function is to get the PSP entry information for given PSP entry type

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
  );

#define INSTANCE_IGNORED 0xFF
#define SUBPROGRAM_IGNORED 0xFF

/**
 *
 *  Get BIOS Directory Entry 's properties by EntryType and EntryInstance
 *  this function will ignore SubProgram, if you care about SubProgram, call BIOSEntryInfoByAttributes instead.
 *
 *  @param[in]     EntryType        BIOS Directory Entry type
 *  @param[in]     EntryInstance    If input with INSTANCE_IGNORED, will return 1st Entry type matched
 *                                  If input with Non INSTANCE_IGNORED, will return the entry which both Type & Instance matched
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
BIOSEntryInfo (
  IN       UINT8                       EntryType,
  IN       UINT8                       EntryInstance,
  IN OUT   TYPE_ATTRIB                 *TypeAttrib,
  IN OUT   UINT64                      *EntryAddress,
  IN OUT   UINT32                      *EntrySize,
  IN OUT   UINT64                      *EntryDest
  );

/**
 * @brief Get the PSP level 2 Base Address
 *
 * @param PspLv2BaseAddress  the pointer to save the PSP level 2 base address
 * @return BOOLEAN           return TRUE if correct address found, otherise, return FALSE
 */
BOOLEAN
GetPspLv2BaseAddr (
  IN OUT UINT64                      *PspLv2BaseAddress
  );

/**
 * @brief Get the Base Addresses of both PSP level 2 and BIOS level 2
 *
 * @param PspLv2BaseAddress   the pointer to save the PSP level 2 base address
 * @param BiosLv2BaseAddress  the pointer to save the BIOS level 2 base address
 * @return BOOLEAN            return TRUE if correct addresses found, otherise, return FALSE
 */
BOOLEAN
GetPspBiosLv2BaseAddr (
  IN OUT UINT64                      *PspLv2BaseAddress,
  IN OUT UINT64                      *BiosLv2BaseAddress
  );

/**
 *
 *  Get BIOS Directory Entry 's properties by 3 Attributes: EntryType, EntryInstance, SubProgram.
 *
 *  @param[in]     EntryType        BIOS Directory Entry type
 *  @param[in]     EntryInstance    If input with INSTANCE_IGNORED, will return 1st Entry type matched
 *                                  If input with Non INSTANCE_IGNORED, will return the entry which both Type & Instance matched
 *  @param[in]     SubProgram       If input with SUBPROGRAM_IGNORED, will return 1st Entry type & Instance matched
 *                                  If input with Non SUBPROGRAM_IGNORED, will return the entry which all Type & Instance & SubProgram matched
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
  );

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
  );

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
  );

/**
  This function checks if the PSP L1 is A/B recovery layout,
  no matter legacy A/B recovery(RN) or multiple slots A/B recovery layout (VN & MR)
**/
BOOLEAN
IsABrecovery (
  IN         PSP_DIRECTORY               *PspL1Dir,
  IN OUT     UINT64                      *PspRegionAEntryAddress,
  IN OUT     UINT64                      *PspRegionBEntryAddress
  );

/**
  This function checks if the PSP L1 is multi slot layout (VN & MR)

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has multi slots (>=2)
  @retval FALSE            the PSP L1 directory has not multi slots (>=2)

**/
BOOLEAN
IsMultiSlotLayout (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  );

/**
  This function checks if the PSP L1 is two slot A/B recovery layout (VN)

  @param[in]  pointer to PSP L1 directory base

  @retval TRUE            the PSP L1 directory has 2 slots with A/B recovery
  @retval FALSE            the PSP L1 directory doesn't have 2 slots with A/B recovery

**/
BOOLEAN
IsTwoSlotAbRecovery (
  IN PSP_DIRECTORY       *PspL1Dir   //PSP L1 directory
  );

BOOLEAN
GetPspEntryScsBinaryV2 (
  IN OUT   UINT64                      *EntryAddress,
  IN       UINT32                      *EntrySize
  );

BOOLEAN
GetPspEntryPspNvDataV2 (
  IN OUT   UINT64                      *EntryAddress,
  IN       UINT32                      *EntrySize
  );

/**
  Check PSP Recovery Flag using SMN to remove MMIO initial dependency
  Target will set Recovery flag if some PSP entry point by PSP directory has been corrupted.

  @retval BOOLEAN  0: Recovery Flag is cleared, 1: Recovery Flag has been set

**/
BOOLEAN
CheckPspRecoveryFlagSmn (
  VOID
  );

BOOLEAN
GetImageSlotInfo (
  IN PSP_DIRECTORY       *PspL1Dir,
  IN OUT IMAGE_SLOT_INFO **Slot
);

BOOLEAN
GetPspEntryAddress (
  IN      UINT16     EntryType,
  IN      BOOLEAN   IsLevel2,
  IN OUT  UINT64    *EntryAddress
  );

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
  );

/**
 * @brief Set A/B Recovery Reason to BIOS RAM
 *
 * @param RecoveryReason  you need to convert RECOVERY_REASON_V1 or RECOVERY_REASON_V2 to UINT32 before this call
 * @return EFI_STATUS     0: Success, NonZero Error
 */
EFI_STATUS
SetRecoveryReason (
  UINT32      RecoveryReason
  );

#endif // _AMD_DIRECTORY_BASELIB_H_


