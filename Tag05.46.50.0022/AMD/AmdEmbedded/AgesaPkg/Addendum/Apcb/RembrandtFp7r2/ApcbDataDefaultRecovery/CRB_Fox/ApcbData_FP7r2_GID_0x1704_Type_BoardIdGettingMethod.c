/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

/*****************************************************************************
 *
 * Warning: BoardIdGettingMethod applies to multiple APCB only, please remove it
 * from ApcbCreate.bat if only 1 APCB instance is needed
 *
 *****************************************************************************
 */

#include <MyPorting.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

#define BOARD_ID_METHOD_ENV_ENCODE  0x10

/// For Environment Encode method
typedef struct _PSP_GET_BOARD_ID_FROM_ENV_ENCODE_RMB_STRUCT {
  IN       UINT16            AccessMethod;      ///< 1: Smbus, 2: EEPROM, 3: FCH GPIO, 4: IO, 5: APCI RD_EC, 0xF: USER_CONFIG, 0x10: ENV_ENCODE
  IN       UINT16            StructSize;        ///< Size in byte of this struct
  IN       ID_APCB_MAPPING   IdApcbMapping[4];  ///< Board ID to APCB Instance mapping
} PSP_GET_BOARD_ID_FROM_ENV_ENCODE_RMB_STRUCT;

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_PSP,                             // GroupId
  APCB_PSP_TYPE_BOARD_ID_GETTING_METHOD,      // TypeId
  sizeof(ApcbTypeHeader),                     // SizeOfType, will be fixed up by tool
  0,                                          // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT | APCB_PRIORITY_TYPE_MASK_HARD_FORCE, // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK               // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

PSP_GET_BOARD_ID_FROM_ENV_ENCODE_RMB_STRUCT  BoardIdGettingMethodEnvEncode =
{
  BOARD_ID_METHOD_ENV_ENCODE,
  sizeof (BoardIdGettingMethodEnvEncode),
  {
    {0xFF, 0x01, 0}, // PRESIL_CTRL_ENV_SOC_EMULATION
    {0xFF, 0x02, 0}, // PRESIL_CTRL_ENV_COBRA
    {0xFF, 0x03, 0}, // PRESIL_CTRL_ENV_GIO
    {0xFF, 0x04, 0}, // PRESIL_CTRL_ENV_GC_LITE
  }
};

// PSP_GET_BOARD_ID_FROM_GPIO_STRUCT BoardIdGettingMethodGpio =
// {
//   // AccessMethod
//   //    0x01 - Smbus
//   //    0x02 - EEPROM
//   //    0x03 - FCH GPIO
//   //    0x04 - IO
//   //    0x05 - APCI RD_EC
//   //    0x0F - USER_CONFIG
//   //    0x10 - RESERVED
//   BOARD_ID_METHOD_GPIO,

//   // GPIO pin config
//   //    Column 1 - FCH GPIO number of the board ID
//   //    Column 2 - Value write to IOMUX to configure this GPIO pin
//   //    Column 3 - Value write to GPIOBankCtl[23:16] to configure this GPIO pin
//   104,  0x01, 0x00,     // ID_BIT0
//   105,  0x01, 0x00,     // ID_BIT1
//   106,  0x01, 0x00,     // ID_BIT2
//   107,  0x01, 0x00,     // ID_BIT3

//   // Board ID to APCB Instance mapping
//   // {BitMask, BitVal, ID}
//   {
//     {0x0F, 0x08, 1},      // ID  1000b = Instance 1
//     {0x0F, 0x00, 2},      // ID  0000b = Instance 2
//   }
// }

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

