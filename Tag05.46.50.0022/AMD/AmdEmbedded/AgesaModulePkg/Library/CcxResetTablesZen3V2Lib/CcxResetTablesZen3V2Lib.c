/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCX Reset Tables Library - Zen3 V2 executed in PEI phase
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 *
 */
/*++
Module Name:

  CcxResetTablesZen3V2Lib.c
  Apply register table settings

Abstract:
--*/

#include <AGESA.h>
#include <Library/AmdTableLibV2.h>
#include <SocLogicalId.h>
#include <Filecode.h>

#define FILECODE LIBRARY_CCXRESETTABLESZEN3V2LIB_CCXRESETTABLESZEN3V2LIB_FILECODE

CONST UINT8 ROMDATA CcxZen3AllCoreRegistersAfterApLaunch[] = {
// To enable x2APIC mode, we have to set ApicEn first then set x2ApicEn
// Transition from disabled mode to x2APIC mode is illegal per x2APIC spec

// APIC_BAR (0x0000001B)
// bit[11]    ApicEn = 1
  MAKE_MSR_ENTRY (0x0000001B, 0x0000000000000800, 0x0000000000000800),

// APIC_BAR (0x0000001B)
// bit[10]    x2ApicEn = 1
  MAKE_MSR_PLATFORM_FEAT_ENTRY (AMD_PF_X2APIC, 0x0000001B, 0x0000000000000400, 0x0000000000000400),

// HWCR (0xC0010015)
// bit[32]    FastTprLoweringDis = 1
// bit[27]    EffFreqReadOnlyLock = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0010015, 0x0000000108000000, 0x0000000108000000),

// MCA_CTL_MASK_LS (0xC0010400)
// bit[21]    SystemReadDataErrorWcb = 1
// bit[20]    SystemReadDataErrorScb = 1
// bit[19]    SystemReadDataErrorLoad = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0010400, 0x0000000000380000, 0x0000000000380000),

// MCA_CTL_MASK_IF (0xC0010401)
// bit[16]    L2TlbMultiHit = 1
// bit[11]    L2BtbMultiHit = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0010401, 0x0000000000010800, 0x0000000000010800),

// MCA_CTL_MASK_L2 (0xC0010402)
// bit[3]     Hwa = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0010402, 0x0000000000000008, 0x0000000000000008),

// MCA_CTL_MASK_FP (0xC0010406)
// bit[6]     Hwa = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0010406, 0x0000000000000040, 0x0000000000000040),

// CPUID_7_Features (0xC0011002)
// bit[11]    Reserved = 0
// bit[9]     Reserved = 0
// bit[4]     Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011002, 0x0000000000000000, 0x0000000000000A10),

// Reserved (0xC00110DC)
// bit[22]    Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110DC, 0x0000000000000000, 0x0000000000400000),

// Reserved (0xC00110DE)
// bit[6]     Reserved = 1
// bit[5]     Reserved = 0
// bit[4]     Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110DE, 0x0000000000000040, 0x0000000000000070),

// Reserved (0xC00110DF)
// bit[36]    Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110DF, 0x0000000000000000, 0x0000001000000000),

// MCA_CONFIG_LS (0xC0002004)
// bit[39]    Reserved = 0
// bit[34]    LogDeferredInMcaStat = 1
// bit[32]    McaXEnable = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0002004, 0x0000000500000000, 0x0000008500000000),

// MCA_CONFIG_IF (0xC0002014)
// bit[39]    Reserved = 0
// bit[32]    McaXEnable = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0002014, 0x0000000100000000, 0x0000008100000000),

// MCA_CONFIG_L2 (0xC0002024)
// bit[39]    Reserved = 0
// bit[34]    LogDeferredInMcaStat = 1
// bit[33]    TransparentErrorLoggingEnable = 0
// bit[32]    McaXEnable = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0002024, 0x0000000500000000, 0x0000008700000000),

// MCA_CONFIG_DE (0xC0002034)
// bit[39]    Reserved = 0
// bit[32]    McaXEnable = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0002034, 0x0000000100000000, 0x0000008100000000),

// MCA_CONFIG_EX (0xC0002054)
// bit[39]    Reserved = 0
// bit[32]    McaXEnable = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0002054, 0x0000000100000000, 0x0000008100000000),

// MCA_CONFIG_FP (0xC0002064)
// bit[39]    Reserved = 0
// bit[32]    McaXEnable = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0002064, 0x0000000100000000, 0x0000008100000000),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

STATIC CONST UINT8 ROMDATA CcxZen3ComputeUnitRegistersAfterApLaunch[] =
{
//  M S R    T a b l e s
// ----------------------

// SYS_CFG (0xC0010010)
// bit[20]    MtrrVarDramEn = 1
// bit[18]    MtrrFixDramEn = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0010010, 0x0000000000140000, 0x0000000000140000),

// Reserved (0xC0011020)
// bit[54]    Reserved = 0
// bit[52:50] Reserved = 1
// bit[46]    Reserved = 1
// bit[43]    Reserved = 1
// bit[36]    Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011020, 0x0004480000000000, 0x005C481000000000),

// Reserved (0xC0011021)
// bit[53:51] Reserved = 0
// bit[50:49] Reserved = 0
// bit[48]    Reserved = 0
// bit[33]    Reserved = 0
// bit[8:7]   Reserved = 2
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011021, 0x0000000000000100, 0x003F000200000180),

// Reserved (0xC0011023)
// bit[55]    Reserved = 0
// bit[49]    Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011023, 0x0002000000000000, 0x0082000000000000),

// Reserved (0xC0011029)
// bit[59]    Reserved = 0
// bit[33]    Reserved = 1
// bit[32]    Reserved = 1
// bit[28]    Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011029, 0x0000000310000000, 0x0800000310000000),

// Reserved (0xC001102A)
// bit[16]    Reserved = 1
// bit[15]    Reserved = 1
// bit[7]     Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC001102A, 0x0000000000018080, 0x0000000000018080),

// Reserved (0xC001102C)
// bit[58]    Reserved = 1
// bit[55]    Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC001102C, 0x0400000000000000, 0x0480000000000000),

// Reserved (0xC001102D)
// bit[57]    Reserved = 0
// bit[47]    Reserved = 0
// bit[36:35] Reserved = 3
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC001102D, 0x0000001800000000, 0x0200801800000000),

// Reserved (0xC001102E)
// bit[14]    Reserved = 0
// bit[6]     Reserved = 1
// bit[5]     Reserved = 0
// bit[4:2]   Reserved = 0
// bit[1]     Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC001102E, 0x0000000000000040, 0x000000000000407E),

// Reserved (0xC00110E2)
// bit[49]    Reserved = 1
// bit[32]    Reserved = 0
// bit[19]    Reserved = 1
// bit[4]     Reserved = 1
// bit[3]     Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110E2, 0x0002000000080018, 0x0002000100080018),

// Reserved (0xC00110E5)
// bit[62]    Reserved = 0
// bit[60]    Reserved = 1
// bit[57]    Reserved = 1
// bit[56]    Reserved = 0
// bit[42]    Reserved = 0
// bit[30]    Reserved = 1
// bit[26]    Reserved = 1
// bit[19]    Reserved = 1
// bit[13]    Reserved = 1
// bit[8]     Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110E5, 0x1200000044082100, 0x5300040044082100),

// Reserved (0xC00110E6)
// bit[12]    Reserved = 0
// bit[0]     Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110E6, 0x0000000000000000, 0x0000000000001001),

// Reserved (0xC00110E7)
// bit[7]     Reserved = 1
// bit[3]     Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110E7, 0x0000000000000080, 0x0000000000000088),

// Reserved (0xC00110E9)
// bit[24]    Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110E9, 0x0000000001000000, 0x0000000001000000),

// Reserved (0xC00110EA)
// bit[23:22] Reserved = 1
// bit[7:4]   Reserved = 0
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC00110EA, 0x0000000000400000, 0x0000000000C000F0),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

CONST UINT8 ROMDATA CcxZen3ComplexRegistersAfterApLaunch[] = {
// Reserved (0xC0011093)
// bit[23]    Reserved = 0
// bit[22]    Reserved = 1
// bit[20:19] Reserved = 2
// bit[18]    Reserved = 0
// bit[12:10] Reserved = 5
// bit[9:7]   Reserved = 5
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011093, 0x0000000000501680, 0x0000000000DC1F80),

// Reserved (0xC0011097)
// bit[12]    Reserved = 1
// bit[11:9]  Reserved = 6
// bit[8:6]   Reserved = 4
// bit[2:1]   Reserved = 3
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_RMB, AMD_REV_F19_RMB_ALL, 0xC0011097, 0x0000000000001D06, 0x0000000000001FC6),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

CONST REGISTER_TABLE ROMDATA CcxZen3ComplexRegTableAfterApLaunch = {
  ComplexPrimary,
  CcxZen3ComplexRegistersAfterApLaunch,
};

CONST REGISTER_TABLE ROMDATA CcxZen3CURegTableAfterApLaunch = {
  ComputeUnitPrimary,
  CcxZen3ComputeUnitRegistersAfterApLaunch,
};

CONST REGISTER_TABLE ROMDATA CcxZen3AllCoreRegTableAfterApLaunch = {
  AllCores,
  CcxZen3AllCoreRegistersAfterApLaunch,
};

CONST REGISTER_TABLE ROMDATA *CcxZen3RegisterTablesAfterApLaunch[] = {
  &CcxZen3AllCoreRegTableAfterApLaunch,
  &CcxZen3CURegTableAfterApLaunch,
  &CcxZen3ComplexRegTableAfterApLaunch,
  NULL
};

CONST REGISTER_TABLE ROMDATA *CcxZen3RegisterTablesAfterApLaunchSecureS3[] = {
  NULL
};

CONST REGISTER_TABLE_AT_GIVEN_TP ROMDATA CcxZen3RegTableListAtGivenTP[] = {
  {AmdRegisterTableTpAfterApLaunch, CcxZen3RegisterTablesAfterApLaunch},
  {AmdRegisterTableTpAfterApLaunchSecureS3, CcxZen3RegisterTablesAfterApLaunchSecureS3},
  {MaxAmdRegisterTableTps, NULL}
};


/*++

Routine Description:

  Zen3 Register table programming

Arguments:
  SleepType
  AMD_CONFIG_PARAMS *

Returns:

--*/
VOID
CcxProgramTablesAtReset (
  IN       UINT8               SleepType,
  IN       ENTRY_CRITERIA     *InitializedCriteria,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  REGISTER_TABLE_TIME_POINT  Timepoint;

  Timepoint = (SleepType == 0x03) ? AmdRegisterTableTpAfterApLaunchSecureS3 : AmdRegisterTableTpAfterApLaunch;
  SetRegistersFromTablesAtGivenTimePoint (StdHeader, (REGISTER_TABLE_AT_GIVEN_TP *) &CcxZen3RegTableListAtGivenTP[0], Timepoint, InitializedCriteria);
}
