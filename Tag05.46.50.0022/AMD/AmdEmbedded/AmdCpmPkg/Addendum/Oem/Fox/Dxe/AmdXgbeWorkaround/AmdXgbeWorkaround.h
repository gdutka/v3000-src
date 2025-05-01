/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_XGBE_WORKAROUND_H_
#define _AMD_XGBE_WORKAROUND_H_
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>

#define ENET_PORT_PROPERTY_3  0x1D00Cul
#define XGB01_1_TX_DISABLE BIT11
#define XGB00_0_TX_DISABLE BIT15

#define SFP_01_PWR_CTRL      BIT0
#define SFP_00_PWR_CTRL      BIT1
#define TIM_INT             BIT2
#define EEPROM_WP           BIT3
#define EN_2_5V             BIT4
#define ALL_DONE            BIT5
#define READ_EN             BIT6
#define SMB_EN              BIT7


#define NUMBER_OF_ETH_PORTS 2

#define PCA9535_ADDRESS       0x20
#define PORT_I2C_MUTEX        0x1D080ul
#define INT_EN                0x1D078ul

typedef struct {
    UINT32 Xgmac0BaseAdd;
    UINT32 Xgmac1BaseAdd;
} XGBE_DATA;


typedef struct XGBE_DATA XgbeData;

#endif
