/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>
#include <ApcbV3TokenUid.h>

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_TOKEN,                           // GroupId
  APCB_TYPE_TOKEN_4BYTES,                     // TypeId
  sizeof(ApcbTypeHeader),                     // SizeOfType, will be fixed up by tool
  0,                                          // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_TOKEN,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_ASCENDING,
    APCB_TYPE_ATTR_UNITSIZE_TOKEN_V3,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,
    APCB_TYPE_ATTR_KEYSIZE_TOKEN_V3,
    APCB_TYPE_ATTR_KEYPOS_TOKEN_V3,
    BLDCFG_APCB_DATA_BOARD_MASK
  }
};  // SizeOfType will be fixed up by tool


APCB_TOKEN_PAIR_UINT32 TokeList[] = {
// Two macros can be used for token definitions
//  APCB_TOKEN_U32 (APCB_TOKEN_UID_NAME, APCB_TOKEN_UID_NAME_VALUE),
//  APCB_TOKEN_VAL_U32(APCB_TOKEN_UID_NAME),  // Expand to use APCB_TOKEN_UID_NAME_VALUE

  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_CCX_MIN_SEV_ASID),                                                          //< override by original CBS token APCB_TOKEN_UID_CBS_CMN_CPU_SEV_ASID_SPACE_LIMIT     //< MIN_SEV_ASID
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_XTRIG7_WORKAROUND),                                                         //< XTRIG7_WORKAROUND
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_DF_PCI_MMIO_SIZE),                                                          //< Size in bytes of space used for PCI MMIO
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS),                                            //< Percentage of performance to sacrifice for CAKE CRC in 0.00001% units
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_USERTIMINGMODE),                                                            //< UserTimingMode - User Memclock Mode.
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_MEMCLOCKVALUE),                                                             //< MemClockValue -  Memory Clock Value.
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_UMASIZE),                                                                   //< UmaSize - The size of shared graphics dram (16-bits)
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_UMAALIGNMENT),                                                              //< CfgUmaAlignment -  Uma alignment
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT),                                                   //< CfgMemoryBusFrequencyLimit -  Memory Bus Frequency Limit.
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_POWERDOWNMODE),                                                             //< CfgPowerDownMode -  Power Down Mode.
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_PCIE_RESET_GPIO_PIN_SELECT),                                                //< Pcie Reset GPIO Pin Select
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIODATA),                                                 //< MPM M Event WLAN POWEREN GPIODATA
  APCB_TOKEN_VAL_U32 (APCB_TOKEN_UID_MEM_RESTORE_RTC_DEFAULT_DATE),                                              //< 8-bit BCD value for default RTC date
//  APCB_TOKEN_U32 (0x00000000, 0xFFFFFFFF) // Null token, remove this when there's other token defined
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

