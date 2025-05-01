/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ACP_ACPAON_H_
#define _ACP_ACPAON_H_


/***********************************************************
* Register Name : ACP_DEVICE_STATE
* Register Description :
* ACP DEVICE STATE Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register indicates the Power state of the ACP device .0= D0 state ,1=D1 state, 2= D2 state, 3= D3 state
#define ACP_DEVICE_STATE_ACP_DEVICE_STATE_OFFSET      0
#define ACP_DEVICE_STATE_ACP_DEVICE_STATE_MASK        0x3

// Bitfield Description : 
#define ACP_DEVICE_STATE_Reserved_31_2_OFFSET      2
#define ACP_DEVICE_STATE_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            ACP_DEVICE_STATE:2;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} ACP_DEVICE_STATE_STRUCT;

#define SMN_ACP_DEVICE_STATE_ADDRESS    0x1241404UL


/***********************************************************
* Register Name : ACP_I2S_WAKE_EN
* Register Description :
* ACP I2S Wake Enable
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP I2S Wake Feature Enable.  If This wake bit and ACP_PME_EN is set, up on receiving HIGH on CODEC_GPI PAD, it will trigger ACP_Wake Interrupt to FCH.  Interrupt will be lowered only either any of ACP_PME_EN, ACP_I2S_WAKE_EN or CODEC_GPI is Low.
// 0 - Disable I2S Wake
// 1 - Enable I2S Wake
#define ACP_I2S_WAKE_EN_ACP_I2S_WAKE_EN_OFFSET      0
#define ACP_I2S_WAKE_EN_ACP_I2S_WAKE_EN_MASK        0x1

// Bitfield Description : ACP GPIO Wake mode by external GPIO event.
// 2'b00 - Reserved.
// 2'b01 - Reserved.
// 2'b10 - wake dsp on level HIGH detection from GPIO interrupt
// 2'b11 - wake dsp on level LOW detection from GPIO interrupt.
#define ACP_I2S_WAKE_EN_ACP_GPIO_INTR_WAKE_MODE_OFFSET      1
#define ACP_I2S_WAKE_EN_ACP_GPIO_INTR_WAKE_MODE_MASK        0x6

// Bitfield Description : This bit indicates whether the GPIO interrupt need to route to ACP -DSP or FCH.
// 0-Route to DSP.
// 1-Route to FCH basedon PME EN and ACP Device state
#define ACP_I2S_WAKE_EN_ACP_GPIO_INTR_WAKE_ROUTING_MODE_OFFSET      3
#define ACP_I2S_WAKE_EN_ACP_GPIO_INTR_WAKE_ROUTING_MODE_MASK        0x8

// Bitfield Description : 
#define ACP_I2S_WAKE_EN_Reserved_31_4_OFFSET      4
#define ACP_I2S_WAKE_EN_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            ACP_I2S_WAKE_EN:1;
    UINT32                            ACP_GPIO_INTR_WAKE_MODE:2;
    UINT32                            ACP_GPIO_INTR_WAKE_ROUTING_MODE:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} ACP_I2S_WAKE_EN_STRUCT;

#define SMN_ACP_I2S_WAKE_EN_ADDRESS    0x124145cUL


/***********************************************************
* Register Name : ACP_PAD_DRIVE_STRENGTH_CTRL
* Register Description :
* ACP Audio PADs Drive Strength control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each consicutive 2bits corresponds to drive strength control of respective Audio Pad.
// Configuration Options:
// 00 - GPIO Operation 1.2V Only; All driver transistors turned on 100% drive; Z mode is Z40
// 01 - GPIO Operation 1.5V & 1.8V; 83.4% drive; Z mode is Z40
// 10 - GPIO Operation 1.2V / 1.5V / 1.8V; 66.7% drive; Z mode is Z60
// 11 - GPIO Operation 1.2V / 1.5V / 1.8V; Only 80 Ohms driver is on, 50% drive; Z mode is Z80
// bits [1:0]   - BP_AZ_BITCLK_SW1_MCLK_TDM0_BCLK_DMIC_DATA1
// bits [3:2]   - BP_AZ_RST_L_SW0_MDATA1_TDM0_DIN
// bits [5:4]   -  BP_AZ_SYNC_SW1_MDATA0_TDM0_FRM_DMIC_DATA2
// bits [7:6]   - BP_AZ_SDOUT_SW0_MDATA2_TDM0_DOUT
// bits [9:8] - BP_AZ_SDIN0_SW0_MDATA3_CODEC_GPI
// bits [11:10] -  BP_AZ_SDIN1_SW0_MCLK_TDM1_BCLK_DMIC_DATA1
// bits [13:12] -  BP_AZ_SDIN2_SW0_MDATA0_TDM1_DOUT_DMIC_DATA2
// bits [15:14] - BP_TDM1_FRM_DMIC_DATA1
// bits [17:16] - BP_TDM1_DIN_DMIC_DATA2
// bits [19:18] - BP_TDM2_BCLK_DMIC_DATA3
// bits [21:20] - BP_TDM2_DOUT
// bits [23:22] - BP_TDM2_DIN
// bits [25:24] - BP_TDM2_FRM
// bits [27:26] -  BP_DMIC_CLK
// bits [29:28] - BP_DMIC_DATA0_I2S_REFCLK
#define ACP_PAD_DRIVE_STRENGTH_CTRL_ACP_PAD_DRIVE_STRENGTH_CTRL_OFFSET      0
#define ACP_PAD_DRIVE_STRENGTH_CTRL_ACP_PAD_DRIVE_STRENGTH_CTRL_MASK        0x3fffffff

// Bitfield Description : 
#define ACP_PAD_DRIVE_STRENGTH_CTRL_Reserved_31_30_OFFSET      30
#define ACP_PAD_DRIVE_STRENGTH_CTRL_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ACP_PAD_DRIVE_STRENGTH_CTRL:30;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} ACP_PAD_DRIVE_STRENGTH_CTRL_STRUCT;

#define SMN_ACP_PAD_DRIVE_STRENGTH_CTRL_ADDRESS    0x124144cUL


/***********************************************************
* Register Name : ACP_PAD_PULLDOWN_CTRL
* Register Description :
* ACP Audio PADs PullDown control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit corresponds to pulldown control of respective Audio Pad.  Setting this bit will put the pad under pulldown.
// bit 0 -BP_AZ_BITCLK_SW1_MCLK_TDM0_BCLK_DMIC_DATA1
// bit 1 - BP_AZ_RST_L_SW0_MDATA1_TDM0_DIN
// bit 2 -  BP_AZ_SYNC_SW1_MDATA0_TDM0_FRM_DMIC_DATA2
// bit 3 -BP_AZ_SDOUT_SW0_MDATA2_TDM0_DOUT
// bit 4 - BP_AZ_SDIN0_SW0_MDATA3_CODEC_GPI
// bit 5 - BP_AZ_SDIN1_SW0_MCLK_TDM1_BCLK_DMIC_DATA1
// bit 6 - BP_AZ_SDIN2_SW0_MDATA0_TDM1_DOUT_DMIC_DATA2
// bit 7 - BP_TDM1_FRM_DMIC_DATA1
// bit 8 - BP_TDM1_DIN_DMIC_DATA2
// bit 9 - BP_TDM2_BCLK_DMIC_DATA3
// bit 10 - BP_TDM2_DOUT
// bit 11 - BP_TDM2_DIN
// bit 12 - BP_TDM2_FRM
// bit 13 - BP_DMIC_CLK
// bit 14 - BP_DMIC_DATA0_I2S_REFCLK
#define ACP_PAD_PULLDOWN_CTRL_ACP_PAD_PULLDOWN_CTRL_OFFSET      0
#define ACP_PAD_PULLDOWN_CTRL_ACP_PAD_PULLDOWN_CTRL_MASK        0x7fff

// Bitfield Description : 
#define ACP_PAD_PULLDOWN_CTRL_Reserved_31_15_OFFSET      15
#define ACP_PAD_PULLDOWN_CTRL_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            ACP_PAD_PULLDOWN_CTRL:15;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} ACP_PAD_PULLDOWN_CTRL_STRUCT;

#define SMN_ACP_PAD_PULLDOWN_CTRL_ADDRESS    0x1241448UL


/***********************************************************
* Register Name : ACP_PAD_PULLUP_CTRL
* Register Description :
* ACP Audio PADs Pullup  control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit corresponds to pullup control of respective Audio Pad.  Setting this bit will put the pad under pullup.
// bit 0 -BP_AZ_BITCLK_SW1_MCLK_TDM0_BCLK_DMIC_DATA1
// bit 1 - BP_AZ_RST_L_SW0_MDATA1_TDM0_DIN
// bit 2 -  BP_AZ_SYNC_SW1_MDATA0_TDM0_FRM_DMIC_DATA2
// bit 3 -BP_AZ_SDOUT_SW0_MDATA2_TDM0_DOUT
// bit 4 - BP_AZ_SDIN0_SW0_MDATA3_CODEC_GPI
// bit 5 - BP_AZ_SDIN1_SW0_MCLK_TDM1_BCLK_DMIC_DATA1
// bit 6 - BP_AZ_SDIN2_SW0_MDATA0_TDM1_DOUT_DMIC_DATA2
// bit 7 - BP_TDM1_FRM_DMIC_DATA1
// bit 8 - BP_TDM1_DIN_DMIC_DATA2
// bit 9 - BP_TDM2_BCLK_DMIC_DATA3
// bit 10 - BP_TDM2_DOUT
// bit 11 - BP_TDM2_DIN
// bit 12 - BP_TDM2_FRM
// bit 13 - BP_DMIC_CLK
// bit 14 - BP_DMIC_DATA0_I2S_REFCLK
#define ACP_PAD_PULLUP_CTRL_ACP_PAD_PULLUP_CTRL_OFFSET      0
#define ACP_PAD_PULLUP_CTRL_ACP_PAD_PULLUP_CTRL_MASK        0x7fff

// Bitfield Description : 
#define ACP_PAD_PULLUP_CTRL_Reserved_31_15_OFFSET      15
#define ACP_PAD_PULLUP_CTRL_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            ACP_PAD_PULLUP_CTRL:15;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} ACP_PAD_PULLUP_CTRL_STRUCT;

#define SMN_ACP_PAD_PULLUP_CTRL_ADDRESS    0x1241444UL


/***********************************************************
* Register Name : ACP_PAD_SCHMEN_CTRL
* Register Description :
* ACP Audio PADs Drive Strength control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit corresponds to schmen control of respective Audio Pad.  Setting this bit will put the pad under pulldown.
// bit 0 -BP_AZ_BITCLK_SW1_MCLK_TDM0_BCLK_DMIC_DATA1
// bit 1 - BP_AZ_RST_L_SW0_MDATA1_TDM0_DIN
// bit 2 -  BP_AZ_SYNC_SW1_MDATA0_TDM0_FRM_DMIC_DATA2
// bit 3 -BP_AZ_SDOUT_SW0_MDATA2_TDM0_DOUT
// bit 4 - BP_AZ_SDIN0_SW0_MDATA3_CODEC_GPI
// bit 5 - BP_AZ_SDIN1_SW0_MCLK_TDM1_BCLK_DMIC_DATA1
// bit 6 - BP_AZ_SDIN2_SW0_MDATA0_TDM1_DOUT_DMIC_DATA2
// bit 7 - BP_TDM1_FRM_DMIC_DATA1
// bit 8 - BP_TDM1_DIN_DMIC_DATA2
// bit 9 - BP_TDM2_BCLK_DMIC_DATA3
// bit 10 - BP_TDM2_DOUT
// bit 11 - BP_TDM2_DIN
// bit 12 - BP_TDM2_FRM
// bit 13 - BP_DMIC_CLK
// bit 14 - BP_DMIC_DATA0_I2S_REFCLK
#define ACP_PAD_SCHMEN_CTRL_ACP_PAD_SCHMEN_CTRL_OFFSET      0
#define ACP_PAD_SCHMEN_CTRL_ACP_PAD_SCHMEN_CTRL_MASK        0x7fff

// Bitfield Description : 
#define ACP_PAD_SCHMEN_CTRL_Reserved_31_15_OFFSET      15
#define ACP_PAD_SCHMEN_CTRL_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            ACP_PAD_SCHMEN_CTRL:15;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} ACP_PAD_SCHMEN_CTRL_STRUCT;

#define SMN_ACP_PAD_SCHMEN_CTRL_ADDRESS    0x1241450UL


/***********************************************************
* Register Name : ACP_PIN_CONFIG
* Register Description :
* ACP_PIN_CONFIGURATION
* Visibility : 0x2
************************************************************/

// Bitfield Description : PIN CONFIGURATION.
// 0 - Reserved;
// 1 - HEDT/Workstation(SPrx package) ; HDA(3SDI).
// 2 -  HEDT/Workstation(SPrx package) ; HDA(1SDI) + SW0(1MDATA).
// 3 - HEDT/Workstation(SPrx package) ; SW0(4MDATA)+ SW1(1MDATA).
// 4 - Mainstream Desktop (AM5 Package);HDA(3SDI) +  PDM(2CH).
// 5 -Mainstream Desktop (AM5 Package);HDA(1SDI) +  PDM(6CH) .
// 6 -Mainstream Desktop (AM5 Package) ;HDA(1SDI) + SW0(1MDATA) + PDM(2CH).
// 7 - MainstreamDesktop (AM5 Package);SW0(4MDATA) + PDM(6CH) .
// 8 - MainstreamDesktop (AM5 Package);SW0(4MDATA) +SW1(1MDATA)+ PDM(2CH) .
// 9 - mainstream notebook;3I2S + 1 REFCLK + 1 INTR.
// 10 - mainstream notebook;HDA(3SDI) +  PDM(6CH) +I2S .
// 11 - mainstream notebook;HDA(3SDI) +  PDM(8CH)  .
// 12 - mainstream notebook;HDA(1SDI) + SW(1MDATA) + PDM(6CH) + I2S .
// 13 - mainstream notebook;SW0(4MDATA) + SW1(1MDATA) +PDM(6CH) + I2S .
// 14 - mainstream notebook;SW0(4MDATA) + SW1(1MDATA) + PDM(8CH) .
// 15 - Reserved .
#define ACP_PIN_CONFIG_ACP_PIN_CONFIG_OFFSET      0
#define ACP_PIN_CONFIG_ACP_PIN_CONFIG_MASK        0xf

// Bitfield Description : 
#define ACP_PIN_CONFIG_Reserved_31_4_OFFSET      4
#define ACP_PIN_CONFIG_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            ACP_PIN_CONFIG:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} ACP_PIN_CONFIG_STRUCT;

#define SMN_ACP_PIN_CONFIG_ADDRESS    0x1241440UL


/***********************************************************
* Register Name : ACP_PME_EN
* Register Description :
* ACP PME Enable
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP PME Enable.  Setting this bit will raise a wake interrupt to FCH if any of SW/I2S WAKE feature is Enabled
#define ACP_PME_EN_ACP_PME_EN_OFFSET      0
#define ACP_PME_EN_ACP_PME_EN_MASK        0x1

// Bitfield Description : 
#define ACP_PME_EN_Reserved_31_1_OFFSET      1
#define ACP_PME_EN_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            ACP_PME_EN:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ACP_PME_EN_STRUCT;

#define SMN_ACP_PME_EN_ADDRESS    0x1241400UL


/***********************************************************
* Register Name : ACP_SW1_WAKE_EN
* Register Description :
* ACP SoundWire1 Wake Enable
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP SoundWire1 (P1 tile) Wake Feature Enable. If This wake bit and ACP_PME_EN is set, up on getting HIGH on SoundWire Data0 pin will trigger ACP_Wake Interrupt to FCH. Interrupt will be lowered only either any of ACP_PME_EN, ACP_SW1_WAKE_EN or SoundWire Data0 pin is Low.
// 0 - Disable SW Wake
// 1 - Enable SW Wake
#define ACP_SW1_WAKE_EN_ACP_SW1_WAKE_EN_OFFSET      0
#define ACP_SW1_WAKE_EN_ACP_SW1_WAKE_EN_MASK        0x1

// Bitfield Description : 
#define ACP_SW1_WAKE_EN_Reserved_31_1_OFFSET      1
#define ACP_SW1_WAKE_EN_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            ACP_SW1_WAKE_EN:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ACP_SW1_WAKE_EN_STRUCT;

#define SMN_ACP_SW1_WAKE_EN_ADDRESS    0x1241460UL


/***********************************************************
* Register Name : ACP_SW_PAD_KEEPER_EN
* Register Description :
* ACP SoundWire Audio PADs Bus Keeper Enable
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit corresponds to Keeper Enable of respective Audio Pad.  Setting this bit will Enable the Bus Keeper feature of respective SoundWire pad.
// bit 0 -BP_AZ_SDIN2_SW0_MDATA0_TDM1_DOUT_DMIC_DATA2
// bit 1 - BP_AZ_RST_L_SW0_MDATA1_TDM0_DIN
// bit 2 -  BP_AZ_SDOUT_SW0_MDATA2_TDM0_DOUT
// bit 3 - BP_AZ_SDIN0_SW0_MDATA3_CODEC_GPI
// bit 4 - BP_AZ_SYNC_SW1_MDATA0_TDM0_FRM_DMIC_DATA2
#define ACP_SW_PAD_KEEPER_EN_ACP_SW_PAD_KEEPER_EN_OFFSET      0
#define ACP_SW_PAD_KEEPER_EN_ACP_SW_PAD_KEEPER_EN_MASK        0x1f

// Bitfield Description : 
#define ACP_SW_PAD_KEEPER_EN_Reserved_31_5_OFFSET      5
#define ACP_SW_PAD_KEEPER_EN_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            ACP_SW_PAD_KEEPER_EN:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} ACP_SW_PAD_KEEPER_EN_STRUCT;

#define SMN_ACP_SW_PAD_KEEPER_EN_ADDRESS    0x1241454UL


/***********************************************************
* Register Name : ACP_SW_WAKE_EN
* Register Description :
* ACP SoundWire Wake Enable
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP SoundWire (P2 tile) Wake Feature Enable. If This wake bit and ACP_PME_EN is set, up on getting HIGH on SoundWire Data0 pin will trigger ACP_Wake Interrupt to FCH. Interrupt will be lowered only either any of ACP_PME_EN, ACP_SW_WAKE_EN or SoundWire Data0 pin is Low.
// 0 - Disable SW Wake
// 1 - Enable SW Wake
#define ACP_SW_WAKE_EN_ACP_SW_WAKE_EN_OFFSET      0
#define ACP_SW_WAKE_EN_ACP_SW_WAKE_EN_MASK        0x1

// Bitfield Description : 
#define ACP_SW_WAKE_EN_Reserved_31_1_OFFSET      1
#define ACP_SW_WAKE_EN_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            ACP_SW_WAKE_EN:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ACP_SW_WAKE_EN_STRUCT;

#define SMN_ACP_SW_WAKE_EN_ADDRESS    0x1241458UL


/***********************************************************
* Register Name : AZ_DEVICE_STATE
* Register Description :
* AZ DEVICE STATE Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register indicates the Power state of the AZ device . 0= D0 state ,1=D1 state, 2= D2 state, 3= D3 state
#define AZ_DEVICE_STATE_AZ_DEVICE_STATE_OFFSET      0
#define AZ_DEVICE_STATE_AZ_DEVICE_STATE_MASK        0x3

// Bitfield Description : 
#define AZ_DEVICE_STATE_Reserved_31_2_OFFSET      2
#define AZ_DEVICE_STATE_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            AZ_DEVICE_STATE:2;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} AZ_DEVICE_STATE_STRUCT;

#define SMN_AZ_DEVICE_STATE_ADDRESS    0x1241408UL

#endif /* _ACP_ACPAON_H_ */
