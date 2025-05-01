/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_SOC_H_
#define _AMD_SOC_H_

/// CPUID related registers
#define AMD_CPUID_FMF               0x80000001ul  // Family Model Features information
#define AMD_CPUID_APICID_LPC_BID    0x00000001ul  // Local APIC ID, Logical Processor Count, Brand ID

#define CPUID_STEPPING_MASK      0x0000000Ful
#define CPUID_BASE_MODEL_MASK    0x000000F0ul
#define CPUID_BASE_FAMILY_MASK   0x00000F00ul
#define CPUID_EXT_MODEL_MASK     0x000F0000ul
#define CPUID_EXT_FAMILY_MASK    0x0FF00000ul

#define RAW_FAMILY_ID_MASK (UINT32) (CPUID_EXT_FAMILY_MASK | CPUID_EXT_MODEL_MASK | CPUID_BASE_FAMILY_MASK)

/// Family 19 package type
#define F19_GN_RAW_ID   0x00A00F00ul
#define F19_RS_RAW_ID   0x00A10F00ul
#define F19_VMR_RAW_ID  0x00A20F00ul
#define F19_BA_RAW_ID   0x00A30F00ul
#define F19_RMB_RAW_ID  0x00A40F00ul
#define F19_RMB_A0_RAW_ID           F19_RMB_RAW_ID
#define F19_RMB_B0_RAW_ID           0x00A40F40ul
#define F19_CZN_RAW_ID  0x00A50F00ul
#define F19_RPL_RAW_ID  0x00A60F00ul
#define F19_PHX_RAW_ID  0x00A70F00ul
#define F19_MI3_RAW_ID  0x00A80F00ul
#define F19_RSDN_RAW_ID 0x00AA0F00ul

/// Family 19 package type
typedef enum {
  ZEN3_FP6   = 0,           ///< Mobile
  ZEN4_AM5   = 0,           ///< Desktop
  ZEN3_SP4   = 1,           ///< Embedded
  ZEN3_AM4   = 2,           ///< Desktop
  ZEN3_SP4r3 = 3,           ///< Embedded
  ZEN3_SP3   = 4,           ///< Server
  ZEN4_SP5   = 4,           ///< Server
  ZEN4_SH5   = 3,           ///< Server
  ZEN4_SP6   = 4,           ///< Server, Workstation
  ZEN3_SP3r4 = 5,           ///< Server
  ZEN3_SP3r3 = 7,           ///< Desktop
  ZEN4_TR5   = 8,           ///< HEDT
  ZEN3_UNKNOWN  = 0xFF      ///< Unknown package type
} FAMILY19_PACKAGE_TYPE;

#define ZEN3_PKG_FP6      0
#define ZEN3_PKG_AM5      0
#define ZEN4_PKG_AM5      0
#define ZEN3_PKG_FP7      1
#define ZEN3_PKG_FP7r2    2
#define ZEN3_PKG_AM5_B0     3
#define ZEN3_PKG_FP7_B0     4
#define ZEN3_PKG_FP7r2_B0   5
#define ZEN3_PKG_SP4      1
#define ZEN3_PKG_AM4      2
#define ZEN3_PKG_SP4r3    3
#define ZEN3_PKG_SP3      4
#define ZEN4_PKG_SP5      4
#define ZEN4_PKG_SH5      3
#define ZEN4_PKG_SP6      4
#define ZEN3_PKG_SP3r3    7
#define ZEN4_PKG_FP8      1
#define ZEN4_PKG_FP8r2    2
#define ZEN4_PKG_TR5      8

/// Family 17 package type
#define F17_ZP_RAW_ID   0x00800F00ul

/// Zeppelin package type
typedef enum {
  ZP_SP4   = 1,           ///< Embedded, two ZP die MCM, single and dual socket
  ZP_AM4   = 2,           ///< Desptop, single die, single socket
  ZP_SP4r2 = 3,           ///< Embedded, single die, single socket
  ZP_SP3   = 4,           ///< Server, four ZP die MCM, single socket
  TR_SP3r2 = 7,           ///< Two TR die MCM, single socket
  ZP_UNKNOWN  = 0xFF      ///< Unknown package type
} FAMILY17_ZP_PACKAGE_TYPE;

#define ZP_PKG_SP4       1
#define ZP_PKG_AM4       2
#define ZP_PKG_SP4r2     3
#define ZP_PKG_SP3       4
#define ZP_PKG_SP3r2     7


#define F17_SSP_RAW_ID  0x00830F00ul
#define F17_MTS_RAW_ID  0x00870F00ul
/// Starship package type
typedef enum {
  SSP_SP4   = 1,          ///< Embedded, two SSP die MCM, single and dual socket
  SSP_AM4   = 2,          ///< Desptop, single die, single socket
  SSP_SP4r2 = 3,          ///< Embedded, single die, single socket
  SSP_SP3   = 4,          ///< Server, four SSP die MCM, single socket
  CP_SP3r3  = 7,          ///< Two CP die MCM, single socket
  SSP_UNKNOWN  = 0xFF     ///< Unknown package type
} FAMILY17_SSP_PACKAGE_TYPE;

#define SSP_PKG_SP4      1
#define SSP_PKG_AM4      2
#define SSP_PKG_SP4r2    3
#define SSP_PKG_SP3      4
#define CP_PKG_SP3r3     7

#define F17_MDN_RAW_ID  0x008A0F00ul

/// Mendocino package type

#define MDN_PKG_FT6      1

#define F15_BR_RAW_ID   0x00660F00ul

/// Bristol package type
typedef enum {
  BR_FP4   = 0,           ///< Notebook BGA Package
  BR_AM4   = 2,           ///< Performance class with DDR4 support
  BR_FM2r2 = 3,           ///< DesktopuPGA Package
  BR_UNKNOWN  = 0xFF      ///< Unknown package type
} BR_PACKAGE_TYPE;

#define BR_PKG_FP4       0
#define BR_PKG_AM4       2
#define BR_PKG_FM2r2     3

#define F17_RV_RAW_ID   0x00810F00ul
#define F17_RV2_RAW_ID  0x00820F00ul
#define F17_PIC_RAW_ID  0x00810F80ul
#define F17_PCO_RAW_ID  F17_PIC_RAW_ID

/// Raven package type
typedef enum {
  RV_FP5   = 0,           ///< Notebook BGA Package
  RV_AM4   = 2,           ///< Performance class with DDR4 support
  RV_FT5   = 3,           ///< Smaller form factor for BGA with LPDDR4
  RV_UNKNOWN  = 0xFF      ///< Unknown package type
} RV_PACKAGE_TYPE;

#define RV_PKG_FP5       0
#define RV_PKG_AM4       2
#define RV_PKG_FT5       3

#define F17_RN_RAW_ID   0x00860F00ul
#define F17_LCN_RAW_ID  0x00860F80ul

/// Renior package type
typedef enum {
  RN_FP6   = 0,           ///< Notebook BGA Package
  RN_AM4   = 2,           ///< Performance class with DDR4 support
  RN_UNKNOWN  = 0xFF      ///< Unknown package type
} RN_PACKAGE_TYPE;

#define RN_PKG_FP6       0
#define RN_PKG_AM4       2

#define F17_MR_RAW_ID   0x00890F80ul
#define F17_VN_RAW_ID   0x00890F00ul
#define PKG_FF3         3

/// CPU Base Model definition
// RMB B0
#define F19_RMB_B0_BASE_MODEL               0x40ul
#define F19_RMB_B0_FAMILY_ID_BASE_MODEL     (F19_RMB_RAW_ID | F19_RMB_B0_BASE_MODEL)
#define F19_RMB_A0_FAMILY_ID_BASE_MODEL     F19_RMB_RAW_ID

/// SOC ID Structure
typedef struct _SOC_ID_STRUCT {
  UINT32            SocFamilyID;    ///< SOC family ID
  UINT8             PackageType;    ///< SOC package type
} SOC_ID_STRUCT;

//Move to separate public header file.
typedef enum {
  PCD_BOOL = 0,             ///< PCD data type : boolean
  PCD_UINT8,                ///< PCD data type : byte
  PCD_UINT16,               ///< PCD data type : word
  PCD_UINT32,               ///< PCD data type : dword
  PCD_UINT64,               ///< PCD data type : qword
  PCD_PTR                   ///< PCD data type : pointer
} AMD_PCD_DATA_TYPE;

/**
 * An AMD AGESA Configuration Parameter Group List.
 */

typedef struct {
  UINTN BufferSize;         ///< PCD PTR buffer size
  void   *Buffer;           ///< PCD PTR buffer pointer
} AMD_PCD_PTR;


typedef struct {
  EFI_GUID *AmdConfigurationParameterPcdGuid;     ///< PCD GUID
  UINTN AmdConfigurationParameterPcdTokenNumber;  ///< PCD Token number
  UINT64   Value;                                 ///< Configurtion Parameter overwrite value.
  AMD_PCD_DATA_TYPE AmdPcdDataType;               ///< Configurtion Parameter PCD data type.
} AMD_PCD_LIST;

typedef struct {
  EFI_GUID *PlalformGroupGuid;          ///< Group List GUID pass from platform BIOS
  UINT32   NumberOfPcdEntries;          ///< number of PCD in list.
  AMD_PCD_LIST   *ListEntry;            ///< Group List version.
} AMD_GROUP_LIST_HEADER;

typedef struct {
  UINT32   Version;                     ///< Group List version.
  UINT32   NumberOfGroupsList;          ///< number of groups list.
  AMD_GROUP_LIST_HEADER   *GroupList;   ///< Group List entry
  EFI_GUID *PlalformGroupGuid;          ///< Group List GUID pass from platform BIOS
} AMD_PCD_PLATFORM_GROUP_LIST;

#endif  // _AMD_SOC_H_


