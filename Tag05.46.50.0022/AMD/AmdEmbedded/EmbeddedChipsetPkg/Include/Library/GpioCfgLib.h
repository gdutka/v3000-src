/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _GPIO_CFG_LIB_H_
#define _GPIO_CFG_LIB_H_

#include <AmdCpmBase.h>
#include <H2OBoardId.h>

#define GPIO_FUNCTION_0     0
#define GPIO_FUNCTION_1     1
#define GPIO_FUNCTION_2     2
#define GPIO_FUNCTION_3     3
#define GPIO_FUNCTION_4     4
#define GPIO_FUNCTION_5     5
#define GPIO_FUNCTION_6     6
#define GPIO_FUNCTION_7     7
#define GPIO_PD_EN          0
#define GPIO_PU_EN          1
#define GPIO_PU_PD_DIS      2
#define GPIO_PU_EN_8K       3
#define GPIO_OUTPUT_LOW     0
#define GPIO_OUTPUT_HIGH    1
#define GPIO_INPUT          2
#define GPIO_NA             3
#define GPIO_STICKY_DIS     0
#define GPIO_STICKY_EN      1

#define NONE          15

#define GPIO_V1       0
#define GPIO_V2       1
#define GPIO_V3       2
#define EC_GPIO       3
#define EC_GPIO_V2    4
#define SMBUS_GPIO    5

#pragma pack(push, 1)


typedef struct {
AMD_CPM_GPIO_ITEM               GpioList;
} H2O_GPIO_INIT_CONFIG;

//
// Defines GPIO_INIT_CONFIG to be the same as reference code, GPIO initialization struceure
//
typedef H2O_GPIO_INIT_CONFIG H2O_GPIO_INIT_STRUCT;

#pragma pack(pop)

//
// Defines H2O_GPIO_INIT macro to initialze all elements in the structure
//
#define H2O_GPIO_INIT(type,socket, die, bridge, gpio, function, output, pullup, sticky) \
{(type == 5 ? ((0x600 + (socket << 7) + (die << 4) + gpio)) : (type == 4 ? (0x200 + gpio) : (type == 3) ? gpio : (type == 2 ? \
((socket << 15) + (die << 13) + (bridge << 11) + gpio) : (type == 1 ? ((socket << 15) + (die << 13) + (0 << 11) + gpio) : gpio)))) , \
(type == 5 ? output : (type == 4 ? output : (type == 3) ? output : (type == 2 ? (function <= 3 ? 0x800 : 0) + (function << 8) + (output < 3 ? \
(output < 2 ? output : 0x02) : 0x08) + ((pullup == 0 ? 0x40 : 0) + (pullup == 1 ? 0x20 : 0) + (pullup == 3 ? 0x24 : 0) + 0x80) : (type == 1 ? \
(function <= 3 ? 0x800 : 0) + (function << 8) + (output < 3 ? (output < 2 ? output : 0x02) : 0x08) + ((pullup == 0 ? 0x40 : \
0) + (pullup == 1 ? 0x20 : 0) + (pullup == 3 ? 0x24 : 0) + 0x80) : (function <= 3 ? 0x800 : 0) + (function << 8) + (output < 3 ? \
(output < 2 ? output : 0x02) : 0x08) + ((sticky == 1) || (pullup < 4) ? ((sticky == 1 ? 0x10 : 0) + (pullup == 0 ? 0x40 : 0) + (pullup == 1 ? \
0x20 : 0) + (pullup == 3 ? 0x24 : 0) + 0x80) : 0)))))}

//
// Defines H2O_GET_BOARD_GPIO macro that verifies that the board is supported and currently excuting
//

#define H2O_GET_BOARD_GPIO(NAME) \
{ \
  if(Board == H2O_BOARD_ID_##NAME) { \
  *GpioInitStruct = mGpioTable##NAME; \
  *GpioInitStructCount = mGpioTable##NAME##Count; \
  return EFI_SUCCESS; \
  } \
}

//
// To get build-time GPIO configura&tion
//
EFI_STATUS
GetGpioCfg(
  IN      H2O_BOARD_ID                     Board,
  OUT     H2O_GPIO_INIT_STRUCT             **GpioInitStruct,
  OUT     UINT32                           *GpioInitStructCount
  );



#endif
