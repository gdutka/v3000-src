/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_CPM_OEM_INIT_PEIM_H_
#define _AMD_CPM_OEM_INIT_PEIM_H_

#define AMD_CPM_OEM_REVISION    0x1080

#define GPIO_STICKY_DIS     0
#define GPIO_STICKY_EN      1
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
#define GPIO_OUTPUT_LOW     0
#define GPIO_OUTPUT_HIGH    1
#define GPIO_INPUT          2
#define GPIO_NA             3


#define EVENT_DISABLE               0
#define EVENT_ENABLE                1
#define SCITRIG_LOW                 0
#define SCITRIG_HI                  1
#define SCITRIG_AUTO                3
#define SCILEVEL_EDGE               0
#define SCILEVEL_LEVEL              1
#define SMISCI_DISABLE              0
#define SMISCI_ENABLE               1
#define SCIS0_DISABLE               0
#define SCIS0_ENABLE                1
#define SCIMAP_00                   0
#define SCIMAP_01                   1
#define SCIMAP_02                   2
#define SCIMAP_03                   3
#define SCIMAP_04                   4
#define SCIMAP_05                   5
#define SCIMAP_06                   6
#define SCIMAP_07                   7
#define SCIMAP_08                   8
#define SCIMAP_09                   9
#define SCIMAP_10                   10
#define SCIMAP_11                   11
#define SCIMAP_12                   12
#define SCIMAP_13                   13
#define SCIMAP_14                   14
#define SCIMAP_15                   15
#define SCIMAP_16                   16
#define SCIMAP_17                   17
#define SCIMAP_18                   18
#define SCIMAP_19                   19
#define SCIMAP_20                   20
#define SCIMAP_21                   21
#define SCIMAP_22                   22
#define SCIMAP_23                   23
#define SMITRIG_LOW                 0
#define SMITRIG_HI                  1
#define SMICONTROL_DISABLE          0
#define SMICONTROL_SMI              1
#define SMICONTROL_NMI              2
#define SMICONTROL_IRQ13            3

#define MISC_BASE                   0xE00
#define FCH_MISC_REG00              0x00        // ClkCntrl0
#define FCH_MISC_REG04              0x04        // ClkCntrl1

// MXM defination
#define MXM_PRESENT                 (0x200 + (8*0x0E) + 3)    // ECRAM xAE[3],"EVAL_PRSNT#EC
#define MXM_DETECT_VALUE            0                         // Low Active
#define MXM_RESET                   (0x200 + (8*0x03) + 0)    // ECRAM xA3[0], "EVAL_REST#_AUX"
#define MXM_POWER_ENABLE            (0x200 + (8*0x01) + 1)    // ECRAM xA1[1], "EVAL_PWREN"
#define MXM_POWER_ON                (0x200 + (8*0x02) + 5)    // ECRAM xA2[5], "EVAL_SLOT_PWR"
#define MXM_POWER_GOOD              (0x200 + (8*0x00) + 1)    // ECRAM xA0[1], "EVAL_PWRGD"

// Smart Mux 1.5 and Smart Mux 2.0 defination
#define SMART_MUX_PWM_EN            (0x200 + (8*0x09) + 1)    // ECRAM xA9[1], "IO0_1 FPR_LOCK#"
#define SMART_MUX_PANEL_BKEN        (0x200 + (8*0x09) + 2)    // ECRAM xA9[2], "IO0_2 FPR_OFF#"
#define SMART_MUX_RESET             (0x200 + (8*0x09) + 3)    // ECRAM xA9[3], "IO0_3 FPR_PWREN"
#define SMART_MUX_SWITCH            (0x200 + (8*0x09) + 4)    // ECRAM xA9[4], "IO0_4 FPR_RSVD"

// SSD
#define SSD1_PRESENT                (0x200 + (8*0x0E) + 4)    // ECRAM xAE[4], M.2_SSD1_PEDET
#define SSD_RESET                   79                        // FCH GPIO79, M2_SSD_RST#_Aux
#define SSD1_POWER_EN               (0x200 + (8*0x09) + 5)    // ECRAM xA9[5], M.2_SSD1_PWREN
#define SSD0_POWER_EN               (0x200 + (8*0x0A) + 6)    // ECRAM xAA[6], M2_SSO0_PWREN

// ODD
#define ODD_PWREN                   (0x200 + (8*0x0A) + 0)     // ECRAM xAA[0], ODD_PWREN

// SD Controller
#define SD_PWR_EN                   (0x200 + (8*0x0A) + 5)    // ECRAM xAA[5], SD_PWREN
#define SD_RESET                    (0x200 + (8*0x00) + 3)    // ECRAM xA0[3], SD_RST#_AUX

// DT power control
#define DT_RESET                    (0x200 + (8*0x03) + 2)    // ECRAM xA3[2], DT_RESET#_AUX
#define DT_POWER_EN                 (0x200 + (8*0x07) + 2)    // ECRAM xA7[2], DT_POWER_EN
#define DTDEVICE_BRIDGE_DEV         1
#define DTDEVICE_BRIDGE_FUN         2
#define DT_X1_POWER_EN              (0x200 + (8*0x01) + 0)    // ECRAM xA1[0], X1_SLOT_PWREN

// WLAN
#define WLAN_RESET                  (0x200 + (8*0x03) + 5)    // ECRAM xA3[5], WLAN_RESET#_AUX
#define WLAN_POWER_EN               (0x200 + (8*0x0A) + 4)    // ECRAM xAA[4], WLAN_PWREN
#define BT_RADIO_EN                 (0x200 + (8*0x0B) + 2)    // ECRAM xAB[2], BT_RADIO_DISABLE#
#define WL_RADIO_EN                 (0x200 + (8*0x0B) + 0)    // ECRAM xAB[0], WL_RADIO_DISABLE#

// WWAN
#define WWAN_RESET                  (0x200 + (8*0x03) + 4)    // ECRAM xA3[4], WWAN_RESET#_AUX
#define WWAN_MODULE_RESET           (0x200 + (8*0x07) + 5)    // ECRAM xA7[5], WWAN_MODULE_RST#
#define WWAN_POWER_EN               (0x200 + (8*0x0A) + 3)    // ECRAM xAA[3], WWAN_PWREN
#define WWAN_CARD_POWER_OFF_L       (0x200 + (8*0x0B) + 7)    // ECRAM xAB[7], WWAN_CARD_POWER_OFF#
#define WWAN_MAIN_RADIO_EN          (0x200 + (8*0x0B) + 6)    // ECRAM xAB[6], MAIN_RADIO_DISABLE#
#define GNSS_MAIN_RADIO_EN          (0x200 + (8*0x0B) + 5)    // ECRAM xAB[5], GNSS_RADIO_DISABLE#

// LOM
#define LAN_RESET                   (0x200 + (8*0x03) + 1)    // ECRAM xA3[1], LOM_RESET#_AUX
#define LAN_POWER_EN                (0x200 + (8*0x0A) + 7)    // ECRAM xAA[7], LOM_PWREN

//
// ECRAMxA0
//
#define ECRAMxA0                    0xA0
#define SD_RST_AUX                  BIT3

//
// ECRAMxA1
//
#define ECRAMxA1                    0xA1
#define X1_SLOT_PWREN               BIT0
#define EVAL_PWREN                  BIT1

//
// ECRAMxA2
//
#define ECRAMxA2                    0xA2
#define X1_SLOT_RST_AUX             BIT1
#define EVAL_19V_EN                 BIT2
#define EVAL_SLOT_PWR               BIT5

//
// ECRAMxA3
//
#define ECRAMxA3                    0xA3
#define EVAL_RST_AUX                BIT0
#define LOM_RESET_AUX               BIT1
#define DT_RESET_AUX                BIT2
#define WWAN_RST_AUX                BIT4
#define WLAN_RESET_AUX              BIT5

//
// ECRAMxA4
//
#define ECRAMxA4                    0xA4

//
// ECRAMxA5
//
#define ECRAMxA5                    0xA5

//
// ECRAMxA6
//
#define ECRAMxA6                    0xA6

//
// ECRAMxA7
//
#define ECRAMxA7                    0xA7
#define TPM_SOI3_RST                BIT0
#define TPM_PWR_EN                  BIT1
#define DT_PWREN                    BIT2
#define WWAN_MODULE_RST             BIT5

//
// ECRAMxA8
//
#define ECRAMxA8                    0xA8
#define SMBUS0_BUFF_EN              BIT0
#define SMBUS1_BUFF_EN              BIT1
#define MUX0_S0                     BIT2
#define MUX0_S1                     BIT3
#define MUX1_S0                     BIT4
#define MUX1_S1                     BIT5
#define MUX2_S0                     BIT6
#define MUX2_S1                     BIT7

//
// ECRAMxA9
//
#define ECRAMxA9                    0xA9
#define TPAD_EN                     BIT0
#define FPR_LOCK_DIS                BIT1
#define FPR_WAKE                    BIT2
#define FPR_PWR_EN                  BIT3
#define FPR_RSVD                    BIT4
#define M2SSD1_PWREN                BIT5
#define TPNL_EN                     BIT6
#define TPNL_PWR_EN                 BIT7

//
// ECRAMxAA
//
#define ECRAMxAA                    0xAA
#define ODD_PWR_EN                  BIT0
#define HDD_PWR_EN                  BIT1
#define USB_CAM_PWRON               BIT2       // Lilac Did not Use
#define WWAN_PWR_EN                 BIT3
#define WLAN_PWR_EN                 BIT4
#define SD_PWR_BIT_EN               BIT5
#define M2SSD0_PWREN                BIT6
#define LOM_PWREN                   BIT7

//
// ECRAMxAB
//
#define ECRAMxAB                    0xAB
#define WL_RADIO_EN_BIT             BIT0
#define BT_RADIO_EN_BIT             BIT2
#define WWAN_CARD_POWER_EN          BIT7

//
// ECRAMxAC
//
#define ECRAMxAC                    0xAC
#define DT_M2SSD1_SW                BIT1        // 0: DT,         1: M2SSD1
#define PWRGRP_WALLE_MP2_SEL        BIT2        // 0: WALLE,      1: APU MP2
#define EVAL_APU_SMBUS_EC_SEL       BIT3        // 0: APU SMBUS1, 1: EC I2C
#define WWAN_PCIEx1_SW              BIT4        // 0: WWAN,       1: PCIEx1
#define ODD_M2SSD1_SW               BIT5        // 0: ODD,        1: M2SSD1
#define M2SSD1_HDD_SW               BIT6        // 0: M2SSD1,     1: HDD (Lilac)
                                                // 0: M2SSD1,     1: DT  (Mayan)
#define WWAN_LOM_SW                 BIT7        // 0: WWAN,       1: LOM

//
// ECRAMxAD
//
#define ECRAMxAD                    0xAD
#define DOCK_UNPLUG                 BIT5    //  ECRAMxAD[5] - DOCK_IN#

//
// ECRAMxAE
//
#define ECRAMxAE                    0xAE
#define ALERT_M2_SSD_3V3_DIS        BIT0
#define EXT_TALERT_DIS              BIT1
#define EVAL_PRSNT_EC_DIS           BIT3
#define M2_SSD1_PEDET               BIT4

//
// ECRAMxB3
//
#define ECRAMxB3                    0xB3

//
// ECRAMxB4
//
#define ECRAMxB4                    0xB4

//
// ECRAMxB5
//
#define ECRAMxB5                    0xB5

//
// ECRAMxB6
//
#define ECRAMxB6                    0xB6
#define CHARGER_MODE_SW             BIT0    // ECRAMxB6[0] - Charger mode - 0: NVDC, 1: BY PASS
#define UCSI_TUNNEL_SW              BIT1    // ECRAMxB6[1] - UCSI tunnel  - 0: disabled, 1: enabled
#define STT_ONBOARD_SENSOR_SW       BIT2    // ECRAMxB6[2] - STT onboard sensor - 0: disabled, 1: enabled
#define STT_EVALCARD_SENSOR_SW      BIT3    // ECRAMxB6[3] - STT eval card sensor - 0: disabled, 1: enabled
#define WILS_MGN_EN                 BIT4    // ECRAMxB6[4] - Wireless manageability enabled - 0: disabled, 1: enabled
#define WLAN_PWR_S3                 BIT6    // ECRAMxB6[6] - Wireless power on S3
#define WLAN_PWR_S4                 BIT7    // ECRAMxB6[7] - Wireless power on S4

//
// ECRAMxB7
//
#define ECRAMxB7                    0xB7
#define SSD0_D3_COLD_EN             BIT4
#define SSD1_D3_COLD_EN             BIT5

//
// ECRAMxC7
//
// DC time for AC/DC switch
#define ECRAMxC7                    0xC7

//
// ECRAMxC8
//
// AC time for AC/DC switch
#define ECRAMxC8                    0xC8

//
// ECRAMxC9
//
// DPTC margin
#define ECRAMxC9                    0xC9

//
// ECRAMxCD
//
#define ECRAMxCD                    0xCD
#define EC_POSTLED_ON               BIT0
#define EC_SMART_MUX_MASK           (BIT2 | BIT3)   // 0: disable, 1: Hybrid Graphics Mode, 2:Discrete Mode

//
// ECRAMxCF
//
// Miscellaneous status and control
#define ECRAMxCF                    0xCF
#define AC_DC_SWITCH_EN             BIT1
#define EN_EC_ACPI                  BIT2

//
// ECRAMxFE
//
#define ECRAMxFE                    0xFE
#define FCH_INIT_PEI                BIT0       // ECRAMxFE[0]
#define FCH_RMB_A0_SKU              BIT2
#define FCH_RMB_B0_SKU              BIT3
#define USB_EN_DIS                  BIT4
#define TBT3_ALT_EN_DIS             BIT5
#define USBC_PORT_DIS               BIT6

//
// TODO: Need to check the follow ZPODD define, It is not list in Mayan/Lilac EC defined
//
// ZPODD defination
#define ZPODD_DETECT_PIN            0x200 + 118 // ECRAM xAE[6]
#define ZPODD_DETECT_VALUE          0           // Low Active
#define ZPODD_POWER_CONTROL         0x200 + 80  // ECRAM xAA[0]

#define ZPODD_DA_GPIO               119         // ECRAM xAE[7]
#define ZPODD_PLUGIN_GPIO           118         // ECRAM xAE[6]
#define ZPODD_DUMMY_EVENT           31          // Set to Dummy Gevent31
#define ZPODD_SATA_MASK             0x0F
#define ZPODD_SATA_PORT             0x01
#define ZPODD_DA_SOURCE             2           // ECRAM GPIO Pin
#define ZPODD_PLUGIN_SOURCE         2           // ECRAM GPIO Pin
#define ZPODD_DA_FALLING            0x42        // ECRAM _Q42 QEvent
#define ZPODD_DA_RISING             0x43        // ECRAM _Q43 QEvent
#define ZPODD_PLUGIN_FALLING        0x40        // ECRAM _Q40 QEvent
#define ZPODD_PLUGIN_RISING         0x41        // ECRAM _Q41 QEvent


// MACO_EN GPIO pin defination
#define BOMACO_EN                   (0x200 + (8*0x02) + 6)    // ECRAM xA2[6], EC_BOMACO_EN

// TBT SLOT
#define TBT_RTD3_PWR                (0x200 + (8*0x03) + 3)    // ECRAM xA3[3], EC Reserved.

#define OEM_EC_CMD_STATUS_PORT      0x666
#define OEM_EC_CMD_DATA_PORT        0x662
#define OEM_EC_TIME_OUT             0x10000
#define OEM_EC_KEY_OBF              1
#define OEM_EC_KEY_IBF              2

#define EC_STATUS_INDEX             0xCF
#define TIEC_DC_TIMER               0xC7
#define TIEC_AC_TIMER               0xC8

#define AMD_PSP_KVM_DISABLE       0x0
#define AMD_PSP_KVM_ENABLE        0x1
#define AMD_PSP_KVM_AUTO          0x3

#define AMD_PSP_KVM_ABSOLUTE      0x0
#define AMD_PSP_KVM_SIMPLE        0x1

// delay is microseconds.
#define PHY_RESET_DELAY 10000


#define PHY_RESET_GPIO            0x1B
#define I2C_SMBUS_GPIO_19         0x13
#define I2C_SMBUS_GPIO_20         0x14
// Unified BIOS support
#define MP0_C2PMSG_97_ADDR                 0x3810A84ul
#define MP0_C2PMSG_98_ADDR                 0x3810A88ul

#define DxioInputParserGetNextDescriptor(Descriptor) (Descriptor != NULL ? ((((Descriptor->Flags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : (++Descriptor))) : NULL)
#define GPIO_CONTROL_REG(function, output, pullup, sticky) (function <= 3 ? 0x800 : 0) + \
                                                           (function << 8) +  \
                                                           (output < 3 ? (output < 2 ? output : 0x02) : 0x08) + \
                                                           ((sticky == 1) || (pullup < 3) ? ((sticky == 1 ? 0x10 : 0) + (pullup == 0 ? 0x40 : 0) + \
                                                           (pullup == 1 ? 0x20 : 0) + 0x80) : 0)
//
// C2PMSG_97  (SMN address 0x3810A84)
//
typedef enum {
  ETH_PORT_0 = 0x0,
  ETH_PORT_1,
  MAX_ETH_PORTS
} XGBE_MAX_PORTS;
typedef enum {
  XGBE_PHY_MODE_RJ45 = 0x0,
  XGBE_PHY_MODE_SFP_PLUS,
  XGBE_PHY_MODE_BACKPLANE
} XGBE_PORT_PHY_MODES;
typedef enum {
  XGBE_RESERVED = 0x0,
  XGBE_10G_1G_BACKPLANE,
  XGBE_2_5G_BACKPLANE,
  XGBE_SOLDERED_DOWN_1000BASE_T,
  XGBE_SOLDERED_DOWN_1000BASE_X,
  XGBE_SOLDERED_DOWN_NBASE_T,
  XGBE_SOLDERED_DOWN_10GBASE_T,
  XGBE_SOLDERED_DOWN_10GBASE_R,
  XGBE_SFP_PLUS_CONNECTOR,
  XGBE_SGMII_BACKPLANE
} XGBE_PORT_PROPERTY0_PLATFORM_CONFIG;

typedef enum {
  XGBE_PORT_SPEED_10G  = 0x10,
  XGBE_PORT_SPEED_2_5G = 0x8,
  XGBE_PORT_SPEED_1G   = 0x4,
  XGBE_PORT_SPEED_100M = 0x2,
  XGBE_PORT_SPEED_10M = 0x1,
} XGBE_PORT_PROPERTY0_SPEED_CONFIG;
typedef enum {
  XGBE_BACKPLANE_CONNECTION = 0x4,
  XGBE_CONNECTION_MDIO_PHY  = 0x2,
  XGBE_SFP_PLUS_CONNECTION  = 0x1,
  XGBE_PORT_NOT_USED        = 0x0
} XGBE_PORT_PORPERTY0_CONNECTION_TYPE;

typedef enum {
  XGBE_PATH_SELECT_DISABLE,
  XGBE_ON_BOARD_POARTS ,
  XGBE_ADD_IN_CARD ,
} XGBE_PATH_SELECT;


typedef enum {
  XGBE_AIC_H551_SFP,
  XGBE_AIC_H552_MVL ,
  XGBE_ADD_H553_BCM ,
} AIC_SELECT;

typedef enum {
  XGBE_AIC3_REVA ,
  XGBE_AIC3_REVB,
} AIC_REVISION;

typedef enum {
  IOMUX_CHANNEL_0,
  IOMUX_CHANNEL_1 ,
  IOMUX_CHANNEL_2 ,
  IOMUX_CHANNEL_3,
} IOMUX_CHANNEL_SELECT;

#define AIC3_10G 0

typedef union {
  struct {
    ///< Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite.  Replaces UserDataPattern0 force
    UINT32 Environment:4;
    ///< Core CL selector: Varies from release to release.  Replaces DF scratch 30[23:0] force
    UINT32 CoreClNum:3;
    ///< Disable DEVINIT and use SWMRA instead. Replaces UmcCfgEco[19] force
    UINT32 DisablePmuDevinitUseSwmra:1;
    ///< OS backdoor load to DRAM.  New feature bit.
    UINT32 OsBackdoorLoad:1;
    ///< When set FW will skip all USB Control init communication from the FW
    UINT32 SkipAllUSBControllerAccess:1;
    ///< When set FW will skip all USB phy communication from the FW
    UINT32 SkipAllUSBPhyAccess:1;
    ///<When set FW will skip DXIO Initialization from the FW>
    UINT32 SkipDxioInitializaion:1;
    ///< Reserved
    UINT32 :20;
  } Field;
  UINT32  Value;
} PRESIL_CTRL0;

#define PRESIL_CTRL_ENV_HW            0
#define PRESIL_CTRL_ENV_SOC_EMULATION 1
#define PRESIL_CTRL_ENV_COBRA         2
#define PRESIL_CTRL_ENV_GIO           3
#define PRESIL_CTRL_ENV_GC_LITE       4

/// Physical or virtual on-board device Id.
typedef enum {
  DEVICE_ID_TEMP =                    0x00,
  DEVICE_ID_MXM =                     0x04,       ///< Device Id for MXM
  DEVICE_ID_MXM2 =                    0x05,       ///< Device Id for MXM2
  DEVICE_ID_LAN =                     0x06,       ///< Device Id for DT MPCIE
  DEVICE_ID_MPCIE1 =                  0x07,       ///< Device Id for On-board LAN
  DEVICE_ID_DDI3 =                    0x08,       ///< Device Id for DDI3
  DEVICE_ID_EXPRESSCARD =             0x09,       ///< Device Id for Express Card
  DEVICE_ID_HDD2 =                    0x0B,       ///< Device Id for HDD2
  DEVICE_ID_BT =                      0x0C,       ///< Device Id for BlueTooth
  DEVICE_ID_WIRELESS =                0x0D,       ///< Device Id for WLAN, WWAN, Mini PCIE on DMC slot
  DEVICE_ID_RADIO =                   0x0E,       ///< Device Id for Radio of WLAN, WWAN, Mini PCIE on DMC slot
  DEVICE_ID_VGAMUXVCC =               0x0F,       ///< Device Id for virtual device to switch VGA_MUX_VCC
  DEVICE_ID_SWINGMODE =               0x10,       ///< Device Id for virtual device to switch PEX_STD_SW#
  DEVICE_ID_POWERLEVEL =              0x11,       ///< Device Id for virtual device to switch FCH_PWR_LV
  DEVICE_ID_VGAMUXSEL =               0x12,       ///< Device Id for virtual device to switch VGA_MUX_SEL
  DEVICE_ID_DT =                      0x13,       ///< Device Id for DT MPCIE
  DEVICE_ID_MPCIE2 =                  0x14,       ///< Device Id for Mini PCIE 2
  DEVICE_ID_ODD =                     0x39,       ///< Device Id for ODD
  DEVICE_ID_NFC =                     0x15,       ///< Device Id for NFC
  DEVICE_ID_SD =                      0x16,       ///< Device Id for SD
  DEVICE_ID_M2_CONNECT =              0X17,       ///< Device M2 connect
  DEVICE_ID_M2_CONNECT2 =             0x18,       ///< Device M2 connect2
  DEVICE_ID_WWAN =                    0x19,       ///< Device Id for WWAN
  DEVICE_ID_DP0 =                     0x1A,       ///< Device Id for Dp0
  DEVICE_ID_SSD =                     0x1B,       ///< Device Id for SSD
  DEVICE_ID_RCAM =                    0x1C,       ///< Device Id for Rear Camera
  DEVICE_ID_FCAM =                    0x1D,       ///< Device Id for Front Camera
  DEVICE_ID_WLAN =                    0x1E,       ///< Device Id for WLAN
  DEVICE_ID_LOM =                     0x1F,       ///< Device Id for LoM
  DEVICE_ID_USB_TYPE_C0 =             0x20,       ///< Device Id for USB Type C Port 0
  DEVICE_ID_USB_TYPE_C1 =             0x21,       ///< Device Id for USB Type C Port 1
  DEVICE_ID_WIFI =                    0x22,       ///< Device Id for WIFI
  DEVICE_ID_TB =                      0x23,       ///< Device Id for Thunderbolt
  DEVICE_ID_DTx1 =                    0x24,
  DEVICE_ID_SSD1 =                    0x25,
  DEVICE_ID_UMI,
} CPM_GPIO_DEVICE_ID;


/// Configuration values for RV GppClk in Fch

typedef enum {
  GPP_CLK0 =                          0x00,       ///< GPP_CLK0
  GPP_CLK1 =                          0x01,       ///< GPP_CLK1
  GPP_CLK4 =                          0x02,       ///< GPP_CLK4
  GPP_CLK2 =                          0x03,       ///< GPP_CLK2
  GPP_CLK3 =                          0x04,       ///< GPP_CLK3
  GPP_CLK5 =                          0x05,       ///< GPP_CLK5
  GPP_CLK6 =                          0x06,       ///< GPP_CLK6
  GPP_SKIP =                          0xFE,       ///< SKIP
} CPM_GPP_CLK;

/// Configuration values for SrcClk in ClkGen

typedef enum {
  SRC_CLK0 =                          0x00,       ///< SRC0
  SRC_CLK1,                                       ///< SRC1
  SRC_CLK2,                                       ///< SRC2
  SRC_CLK3,                                       ///< SRC3
  SRC_CLK4,                                       ///< SRC4
  SRC_CLK5,                                       ///< SRC5
  SRC_CLK6,                                       ///< SRC6
  SRC_CLK7,                                       ///< SRC7
  SRC_CLK8,                                       ///< SRC8
  SRC_CLK9,                                       ///< SRC9
  SRC_CLK10,                                      ///< SRC10
  SRC_CLK11,                                      ///< SRC11
  SRC_SKIP =                          0xFE,       ///< SKIP
} CPM_SRC_CLK;

/// Configuration values for RV ClkReq

typedef enum {
  CLK_DISABLE =                       0x00,       ///< CLK_DISABLE
  CLK_REQ0    =                       0x01,       ///< CLK_REQ0
  CLK_REQ1    =                       0x02,       ///< CLK_REQ1
  CLK_REQ2    =                       0x03,       ///< CLK_REQ2
  CLK_REQ3    =                       0x04,       ///< CLK_REQ3
  CLK_REQ4    =                       0x05,       ///< CLK_REQ4
  CLK_REQ5    =                       0x06,       ///< CLK_REQ5
  CLK_REQ6    =                       0x07,       ///< CLK_REQ6
  CLK_ENABLE  =                       0xFF,       ///< CLK_ENABLE
} CPM_CLK_REQ;

typedef enum {
  PLATFORM_SelLilac     =                0x00,
  PLATFORM_SelMayan     =                0x01,
  PLATFORM_SelLilacDAP  =                0x02,
  PLATFORM_SelMayanDAP  =                0x03,
//  PLATFORM_SelMoonglow  =               0x04,
  PLATFORM_SelLilacKD   =               0x05,
  PLATFORM_SelMayanKD   =               0x06,
} PLATFORM_SELECTION;

UINT8
OemEcRead (
  IN UINT8                        Index
  );

STATIC
EFI_STATUS
OemWaitKbcIbe (
  IN UINT16                     CommandState
  );

STATIC
EFI_STATUS
OemWaitKbcObf (
  IN UINT16                       CommandState
  );

STATIC
EFI_STATUS
OemReadKbc (
  IN UINT16                       CommandState,
  IN OUT UINT8                    *Data
  );

STATIC
EFI_STATUS
OemWriteKbc (
  IN UINT16                       CommandState,
  IN UINT8                        Data
  );

EFI_STATUS
EFIAPI
AmdXgbeMacAddr (
    IN CONST EFI_PEI_SERVICES     **PeiServices
  );

EFI_STATUS
EFIAPI
XgbeMdioPhyReset (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

EFI_STATUS
EFIAPI
ConfigureRfMuxforDPPorts (
  IN CONST EFI_PEI_SERVICES** PeiServices
  );

EFI_STATUS
EFIAPI
ProgramI2CGpioToSmbus (
  IN CONST EFI_PEI_SERVICES** PeiServices
  );

EFI_STATUS
EFIAPI
AmdCpmOemInitPeimNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
ConfigureEthernetDxioPorts (
  IN EFI_PEI_SERVICES**     PeiServices,
  IN DXIO_PORT_DESCRIPTOR** DxioPort
  );

  EFI_STATUS UpdateXgbeTxPolarity(
  IN EFI_PEI_SERVICES** PeiServices,
  IN UINT32             PortNum ,
  IN BOOLEAN            TxPol
  );

EFI_STATUS
EFIAPI
UpdateXgbeFchPcds (
  IN EFI_PEI_SERVICES** PeiServices,
  IN DXIO_PORT_DESCRIPTOR** DxioPort
  );
#endif
