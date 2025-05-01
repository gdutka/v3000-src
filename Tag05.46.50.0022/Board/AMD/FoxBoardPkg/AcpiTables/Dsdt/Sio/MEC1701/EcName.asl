/** @file

;******************************************************************************
;* Copyright (c) 2013, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

Mutex(Z009,0)
Mutex(QEVT,0)
OperationRegion(ERAM, EmbeddedControl, 0, 0xFF)
Field(ERAM, ByteAcc, NoLock, Preserve)
{
  Offset(0x31),
  PGSR, 8,       // EC RAM [00h .. 30h] are reused space. Set 'PGSR' to a valid Page ID
                 // to active a page. Software needs to set 'PGSR' to FFh after use.
                 // Please refer to EC_debug_interface_of_RMB.docx for detils.
                 // Page ID: 
                 //    00 - Board ID
                 //    89 - additional battery info
                 //
                 //    A2 - eSPI VW aggregater
                 //    B0 - PD information
                 //    D0 - EC SIGs
                 //    D1 - PD Version
                 //    E0 - eSPI Capabilities config
                 //    E1 - eSPI PC bus mastering
                 //    E2 - eSPI VW registers
                 //    E3 - eSPI OOB message
                 //    E4 - SPI S0 sharing test

  Offset(0x60),  // UCSI
  VERS,16,       // PPM->OPM Version         ( 2 bytes)
      ,16,       // Reserved                 ( 2 bytes)
  CCI, 32,       // PPM->OPM CCI indicator   ( 4 bytes)
  CTRL,64,       // OPM->PPM Control message ( 8 bytes)
  MSGI,128,      // OPM->PPM Message In      (16 bytes)
  MSGO,128,      // PPM->OPM Message Out     (16 bytes)

  Offset(0x90),  // PMIC/VR
  PMC0,8,        // register index
  PMC1,8,        // value write to/read from PMIC/VR
  PMC2,8,        // on write, this field indicates the 8-bit PMIC/VR slave device address. 
                 //           LSB is 1 to trigger read operation, 0 to trigger write operation.
                 // on read,  returns the status of last operation. 
                 //           0xAC - success, 0xE2 - error, 0xCC - ongoing

  /* Board ID */
  Offset(0x93),  // ECRAMx93
  PCBR,4,        // PCB revision, 0 - '00A', 1 - '00B' ...
  BRID,4,        // Board ID
                 // 0 - D595 Lilac
                 // 1 - D597 Mayan
                 // 2 - D596 Lilac DAP
                 // 3 - D598 Mayan DAP
                 // 4 - D599 Moonglow
                 // 5 - D884 Lilac_KD
                 // 6 - D883 Mayan_KD
                 // E - In case Board ID EEPROM 0x40 ~ 0x43 are empty
  SKU0,8,        // SKU
  HWLE,8,        // HW Level (or REV)
  SRK0,8,        // Special Rework 0
  SRK1,8,        // Special Rework 1
  SRK2,8,        // Special Rework 2
  SRK3,8,        // Special Rework 3
  SRK4,8,        // Special Rework 4
  SRK5,8,        // Special Rework 5
  MMID,8,        // 8-bit Memory ID

  /* EC native GPIOs */
  Offset(0xA0),   // ECRAMxA0
  GPO0,8,         // Native EC GPIO group 0
                  // Bit[0] - AC_POWER_PRSNT_EC#
                  // Bit[1] - EVAL_PWRGD
                  // Bit[2] - UART_DONGLE_PRSENT#
                  // Bit[3] - SD_RST#_AUX
                  // Bit[4:5] - Reserved
                  // Bit[6] - APU_ALERT#
                  // Bit[7] - SYSTEM_ALW_PG

                  // ECRAMxA1
  GPO1,8,         // Native EC GPIO group 1
                  // Bit[0] - X1_SLOT_PWREN
                  // Bit[1] - EVAL_PWREN
                  // Bit[2:7] - Reserved

                  // ECRAMxA2
  GPO2,8,         // Native EC GPIO group 2
                  // Bit[0] - EC_PWRBRK#
                  // Bit[1] - X1_SLOT_RST#_AUX
                  // Bit[2] - EVAL_19V_EN
                  // Bit[3] - EC_AC_PD#
                  // Bit[4] - PD_SOURCE_ON#
                  // Bit[5] - EVAL_SLOT_PWR
                  // Bit[6] - EC_BOMACO_EN
                  // Bit[7] - CHG_PROCHOT#

                  // ECRAMxA3
  GPO3,8,         // Native EC GPIO group 3
                  // Bit[0] - EVAL_RST#_AUX
                  // Bit[1] - LOM_RESET#_AUX
                  // Bit[2] - DT_RESET#_AUX
                  // Bit[3] - Reserved
                  // Bit[4] - WWAN_RST#_AUX
                  // Bit[5] - WLAN_RESET#_AUX
                  // Bit[6:7] - Reserved

  /* IO expanders */
  Offset(0xA7),   // ECRAMxA7
  GPI7,8,         // GPIO group 7 (Output) U117, GPIO_IOEXP_GP0, 0x21, LOW byte
                  //                NetName        BufType POR_Def  Comment
                  // Bit[0] - IO0_0 TPM_S0I3#      OD      1        To drive this signal low in S0i3 state
                  // Bit[1] - IO0_1 TPM_PWR_EN     OD      1        To enable TPM power, high active
                  // Bit[2] - IO0_2 DT_PWREN       OD      1        To enable DT slot power, high active
                  // Bit[3] - IO0_3 DP0_HPD        OD      1        To mimic DP port Plug in/out
                  // Bit[4] - IO0_4 DP1_HPD        OD      1        To mimic DP port Plug in/out
                  // Bit[5] - IO0_5 WWAN_MODULE_RST# OD    1        To reset WWAN module, high active
                  // Bit[6] - IO0_6 ADAPTER_OFF_EC O       0        High to gating AC adapter source
                  // Bit[7] - IO0_7 DC_S5_ALW_EN_CTRL I    x        High to always keep S5 power in DC mode

                  // ECRAMxA8
  GPI8,8,         // GPIO group 8 (Output) U117, GPIO_IOEXP_GP0, 0x21, HIGH byte
                  //                NetName        BufType POR_Def  Comment
                  // Bit[0] - IO1_0 SMBUS0_BUFF_EN OD      1        To enable SMBus0 buffer
                  // Bit[1] - IO1_1 SMBUS1_BUFF_EN OD      1        To enable SMBus1 buffer
                  // Bit[2] - IO1_2 MUX0_S0        O       0        To control 4:1 I2C MUX0 routing
                  // Bit[3] - IO1_3 MUX0_S1        O       0        To control 4:1 I2C MUX0 routing
                  // Bit[4] - IO1_4 MUX1_S0        O       0        To control 4:1 I2C MUX1 routing
                  // Bit[5] - IO1_5 MUX1_S1        O       1        To control 4:1 I2C MUX1 routing
                  // Bit[6] - IO1_6 MUX2_S0        O       0        To control 4:1 I2C MUX2 routing
                  // Bit[7] - IO1_7 MUX2_S1        O       0        To control 4:1 I2C MUX2 routing

                  // ECRAMxA9
  GPI9,8,         // GPIO group 9 (Output) U124, GPIO_IOEXP_GP1, 0x22, LOW byte
                  //                NetName        BufType POR_Def  Comment
                  // Bit[0] - IO0_0 TPAD_DISABLE#  OD      1        To disable touch pad
                  // Bit[1] - IO0_1 FPR_LOCK#      OD      1        To drive FP to low power state
                  // Bit[2] - IO0_2 FPR_OFF#       OD      1        To enable FP wake up
                  // Bit[3] - IO0_3 FPR_PWREN      OD      1        To enable FP power, high active
                  // Bit[4] - IO0_4 FPR_RSVD       I       x        Reserved for FP side band control
                  // Bit[5] - IO0_5 M2_SSD1_PWREN  OD      1        To enable M2 SSD1 power
                  // Bit[6] - IO0_6 TPNL_EN        OD      1        To enable Touch panel function
                  // Bit[7] - IO0_7 TPNL_PWR_EN    OD      1        To enable Touch panel power

                  // ECRAMxAA
  GPIA,8,         // GPIO group 10 (Output) U124, GPIO_IOEXP_GP1, 0x22, HIGH byte
                  //                NetName        BufType POR_Def  Comment
                  // Bit[0] - IO1_0 ODD_PWR_EN     OD      1        (Lilac Only) To enable ODD power, high active
                  // Bit[1] - IO1_1 HDD_PWR_EN     OD      1        (Lilac Only) To enable HDD power, high active
                  // Bit[2] - IO1_2 USB_CAM_PWREN  OD      1        To enable USB CAM power, high active
                  // Bit[3] - IO1_3 WWAN_PWREN     OD      1        To enable WWAN power, high active
                  // Bit[4] - IO1_4 WLAN_PWREN     OD      1        To enable WLAN power, high active
                  // Bit[5] - IO1_5 SD_PWREN       OD      1        To enable SD controller power
                  // Bit[6] - IO1_6 M2_SSD0_PWREN  OD      1        To enable M.2 SSD0 power
                  // Bit[7] - IO1_7 LOM_PWREN      OD      1        To enable LOM power

                  // ECRAMxAB
  GPIB,8,         // GPIO group 11 (Output) U116, GPIO_IOEXP_GP2, 0x23, LOW byte
                  //                NetName        BufType POR_Def  Comment
                  // Bit[0] - IO0_0 WL_RADIO_DISABLE# OD   1        To disable WLAN's radio, low active
                  // Bit[1] - IO0_1 BAT_LED#          OD   1        Battery indicator
                  // Bit[2] - IO0_2 BT_RADIO_DISABLE# OD   1        To disable Bluetooth radio, low active
                  // Bit[3] - IO0_3 KBC_LOW_BAT#      OD   1        To notify APU for battery low
                  // Bit[4] - IO0_4 M2_BT_WAKE        O    0        To wake BT module
                  // Bit[5] - IO0_5 GNSS_RADIO_DISABLE#  OD  1      To disable GNSS radio
                  // Bit[6] - IO0_6 MAIN_RADIO_DISABLE#  OD  1      To disable WWAN radio
                  // Bit[7] - IO0_7 WWAN_CARD_POWER_OFF# O   1      To control WWAN power on/off

                  // ECRAMxAC
  GPOC,8,         // GPIO group 12 (Output) U116, GPIO_IOEXP_GP2, 0x23, High byte
                  //                NetName          BufType POR_Def  Comment
                  // Bit[0] - IO1_0 DEPOP_MUTE#      O       0        To mute codec POP noise
                  // Bit[1] - IO1_1 DT#_M2SSD1_SW    OD      1        Switching lanes between
                  //                                                  (LOW = DT, HIGH = M2SSD1)
                  // Bit[2] - IO1_2 PWRGRP_WALLE#_MP2_SEL O  0        SPDT switch
                  //                                                  (low = WALLE, high = APU MP2)
                  // Bit[3] - IO1_3 EVAL_EC_SMBUS1#_SW OD    0        Switching EVAL card I2C to
                  //                                                  (LOW = APU SMBUS1, HIGH = EC)
                  // Bit[4] - IO1_4 WWAN#_X1_SW      OD      0        Switching lanes between
                  //                                                  (LOW = WWAN, HIGH = X1 slot)
                  // Bit[5] - IO1_5 ODD#_M2SSD1_SW   OD      0        (Lilac Only) Switching lanes between
                  //                                                  (LOW = ODD slot, HIGH = M2SSD1)
                  // Bit[6] - IO1_6 M2SSD1#_HDD_SW (Lilac) 
                  //                                 OD      0        Switching lanes between 
                  //                                                  (LOW = M2SSD1, HIGH = HDD slot)
                  //                M2SSD1#_DT_SW  (Mayan)
                  //                                 OD      0        Switching lanes between 
                  //                                                  (LOW = M2SSD1, HIGH = DT slot)
                  // Bit[7] - IO1_7 WWAN#_LOM_SW     OD      1        Switching lanes between 
                  //                                                  (LOW = WWAN, HIGH = LOM)

                  // ECRAMxAD
  GPOD,8,         // GPIO group 12 (Input) U121, GPIO_IOEXP_GP3, 0x20, Low Byte
                  //                NetName          BufType POR_Def  Comment
                  // Bit[0] - P0_0  XGBE_CTRL2_3V3   O       0        Reserved for XGBE side band control
                  // Bit[1] - P0_1  XGBE_CTRL1_3V3   O       0        Reserved for XGBE side band control
                  // Bit[2] - P0_2  FCHDEBUGBUS_EN   I       x        Notify EC that FCH debug mode is enabled, high active
                  // Bit[3] - P0_3  DBG_CARD_PSNT#_3V3 I     x        To notify EC, the FPGA debug card is present. low active
                  // Bit[4] - P0_4  LID_CLOSED#      I       x        Notify EC that Lid is closed, low active
                  // Bit[5] - P0_5  DOCK_IN#         I       x        Notify EC,dock is present, low active
                  // Bit[6] - P0_6  WWAN_RADIO_SW#   I       x        Notify EC to toggle enable/disable WIFI radio, falling edge active
                  // Bit[7] - P0_7  WIFI_RADIO_SW#   I       x        Notify EC to toggle enable/disable WIFI radio, falling edge active

                  // ECRAMxAE
  GPOE,8,         // GPIO group 13 (Input) U121, GPIO_IOEXP_GP3, 0x20, High Byte
                  //                NetName          BufType POR_Def  Comment
                  // Bit[0] - P1_0  ALERT#_M2_SSD_3V3 I      x        Notify EC that M.2 SSD0 (I2C slave) need service from EC
                  // Bit[1] - P1_1  EXT_TALERT#      I       x        To nofity EC, an overheating event is occurred. low active
                  // Bit[2] - P1_2  DIGIT_DISP_SW    I       x        Notify EC to toggle debug post code or Board basic info, falling edge active
                  // Bit[3] - P1_3  EVAL_PRSNT#_EC   I       x        Notify EC,Eval Card is present, low active
                  // Bit[4] - P1_4  M2_SSD1_PEDET    I       x        Notify EC the M.2 SSD1 type, low--SATA SSD, high--PCIE SSD
                  // Bit[5] - P1_5  EVAL_NEW#_LEGACY I       x        Notify EC, New or Legacy Eval card is used. low--new, high--legacy.
                  // Bit[6] - P1_6  ODD_PLUGIN#            (Lilac)  I Notify EC for ODD_PLUGIN event
                  //                FP7_PKG_IDENTIFICATION (Mayan)  I Used for FP7/FP7r2 detection
                  // Bit[7] - P1_7  FCH_ODD_DA#            (Lilac)  I Notify EC that ODD device attention is assert, low active
                  //                MEMQ_SHOOTING_EC_APU   (Mayan)  O Used for FP7/FP7r2 detection

  Offset(0xB2),   // ECRAMxB2
  PHL0,8,         // Bit[7:0] - P3T Limit setting
                  //            (default) if this field is 0xFF - means the P3T setting will follow DC Level or the Max 216W if AC prespent.
                  //            Others not 0xFF                 - Fix the P3T limit to the specific setting. (i.e. 0xB4 means 180W)
  PHL1,8,         // Bit[6:0] - Fake DC level, valid settings 0 ~ 100; This option used for specifing battery remain percentage in Fake DC case.
                  //            It takes effect only if a) AC/DC switch is enabled; b) battery is not connected; c) switched to Fake DC
                  //            If AC/DC switch is enabled with real battery, EC will report actual battery level.
                  // Bit[7]   - Reserved
                  //
  PHL2,8,         // Charger prochot setting (it will take effect if AC and/or DC PROCHOT# are enabled, i.e. SW02 bit6 and bit3)
                  // Bit[1:0] - Prochot Debounce for AC or Fake-DC case
                  //            2'b00 - 7us (default); 2'b01 - 100us; 2'b11 - 500us; 2'b11 - 1ms
                  // Bit[4:2] - Prochot Duration 3'b000 - 10ms; 3'b001 - 20ms; 3'b010 - 15ms;
                  //                             3'b011 - 5ms (default); 3'b100 - 1ms; 3'b101 - 500us; 3'b110 - 100us; 3'b111 - 0s
                  // Bit[6:5] - Prochot Debounce for DC case (the real battery)
                  //            2'b00 - 7us; 2'b01 - 100us (default); 2'b11 - 500us; 2'b11 - 1ms
                  // Bit[7]   - To enable P3T Limit (APML 0x3C command to PMFW);
                  //            It will take effect only if DC PROCHOT# is enabled, i.e. SW02 bit6 is 1
                  //              1 - enabled; 0 - disabled (default)
                  //
                  //            Setting is calculated base on DCProchoL, Unit in mW.
                  //              AC case := P3T limit is fixed at 216000mW (the max. of RMB)
                  //              Pure DC := P3T limit is set to DCProchotL * 11.55V
                  //              Fake DC := P3T limit is set to DCProchotL * 19V
                  //                         (DCProchotL is determined by PHL2)
                  //
  PHL3,8,         // Charger prochot setting
                  // Bit[5:0] - DC Procht trigger level (it will take effect only if DC PROCHOT# is enabled, i.e. SW02 bit6 is 1)
                  //            0x3F := (default) 
                  //                    Set DCProchotL to 12A if battery is not present
                  //                    Set DCProchotL to 5A  if battery is present
                  //            0    := if battery is present, ties the DCProchotL to real battery level
                  //                             Battery 100% ~ 90%, =>   10A
                  //                             Battery 90% ~ 80%,  => 9.66A
                  //                             Battery 80% ~ 70%,  =>  9.2A
                  //                             Battery 70% ~ 60%,  =>    9A
                  //                             Battery 60% ~ 50%,  => 8.75A
                  //                             Battery 50% ~ 40%,  => 8.47A
                  //                             Battery 40% ~ 30%,  => 8.33A
                  //                             Battery 30% ~ 20%,  =>  8.2A
                  //                             Battery 20% ~ 10%,  => 7.98A
                  //                             Battery < 10%       =>    5A
                  //                    if battery is not present, ties the DCProchotL to Fake DC level as specified by PHL1
                  //                             Fake DC 100% ~ 90%, => 6.1A
                  //                             Fake DC 90%  ~ 80%, => 5.9A
                  //                             Fake DC 80%  ~ 70%, => 5.6A
                  //                             Fake DC 70%  ~ 60%, => 5.5A
                  //                             Fake DC 60%  ~ 50%, => 5.3A
                  //                             Fake DC 50%  ~ 40%, => 5.2A
                  //                             Fake DC 40%  ~ 30%, => 5.1A
                  //                             Fake DC 30%  ~ 20%, => 5.0A
                  //                             Fake DC 20%  ~ 10%, => 4.9A
                  //                             Fake DC 10%  ~ 5%,  => 4.7A
                  //                             Fake DC 5%   ~ 0%,  => 3A
                  //             1 ~ 0x3E := Specific the high byte of DCProchotL threshold. Low byte fix at zero.
                  //                             e.g. 0x28 stand for 10240mA; 0x14 stands for 5120mA
                  // Bit[7:6] - Reserved

                  // ECRAMxB6
  SW01,8,         // Switch control 1
                  // Bit[0]   - Charger mode - 0: NVDC (default); 1: BY PASS
                  // Bit[1]   - UCSI tunnel  - 0: disabled (default); 1: enabled
                  // Bit[2]   - STT onboard sensor reporting  - 0: disabled (default); 1: enabled
                  // Bit[3]   - STT eval card sensor reporting  - 0: disabled (default); 1: enabled
                  // Bit[4]   - Wireless manageability enabled - 0: disabled (default); 1: enabled
                  // Bit[5]   - KBC wake enabled in S0i3 - 0: disabled; 1: enabled (default)
                  // Bit[6]   - Keep WLAN power in S3  - 0: disabled (default); 1: enabled
                  // Bit[7]   - Keep WLAN power in S4  - 0: disabled (default); 1: enabled
  SW02,8,         // Switch control 2
                  // Bit[0]   - PwrBtn WA for slide-to-shutdown - 0: disabled (default); 1: enabled
                  // Bit[2:1] - Silicon version of USBC - 0: n\a; 1: RN TV; 2: RMB_A0 (default); 3: RMB_B0 USB4
                  // Bit[3]   - Charger PROCHOT# to throttle APU in AC only case - 0: disabled (default); 1: enabled
                  // Bit[4]   - SSD0 D3-Cold enable - 0: disabled (default); 1: enabled
                  // Bit[5]   - SSD1 D3-Cold enable - 0: disabled (default); 1: enabled
                  // Bit[6]   - Charger PROCHOT# to throttle APU in DC only case - 0: disabled; 1: enabled (default)
                  // Bit[7]   - Modern Standby - 0: S3; 1: MS (S0i3)

  /* EC FW version */
  Offset (0xB8),  // 5 + 4 + 6
  ECRV, 40,       // EC firmware revision  "0.x.x"
  ECDT, 32,       // EC firmware timestamp "9B02"
  ECTS, 48,       // EC firmware timestamp "230653"

  /* AC/DC switch */
  Offset(0xC7),
  DCTM, 8,        // Set DC time for AC/DC switch, maximum is 255 seconds
  ACTM, 8,        // Set AC time for AC/DC switch, maximum is 255 seconds

  /* DPTC margin */
  Offset(0xC9),   // Thermal margin of Dynamic Power and Thermal Control
                  // To perfrom an atomic read, host should always read the integer part first.
  MDEC, 8,        // decimal part
  MINT, 8,        // integer part

  /* System Event Flags */
  Offset(0xCB),
  SEFS, 8,        // Bit[0]   - 1, THERMALTRIP# had triggered, write-1-to-clear
                  // Bit[7:1] - Reserved

  Offset(0xCF),   // Miscellaneous Status and Control
  STAS,8,         // Bit[0]   - Reserved
                  // Bit[1]   - 1, BIOS enable AC/DC switch
                  //            0, BIOS disable AC/DC switch
                  // Bit[2]   - 1, BIOS enable ACPI mode
                  //            0, BIOS disable ACPI mode
                  // Bit[5:3] - Reserved
                  // Bit[7:6] - Sleep Type
                  //          This field should be set everytime while system prepares to sleep.
                  //            2'b00, n\a, reserved for S0i3
                  //            2'b01, Sleep S3 (not S0i3)
                  //            2'b10, Sleep S4
                  //            2'b11, Sleep S5

  /* Control Method Batteries */
  /* !! NOTE !!
   * To atomic write a double-byte word, write the LOW byte first
   * To atomic read  a double-byte word, read the HIGH byte first
   */
  Offset (0xD0),  // ACPI_BATTERY_BASE
  MERL, 8,        // LSB of Measurement Accuracy  - RO for BIX
  MERH, 8,        // MSB of Measurement Accuracy  - RO for BIX

  BLOL, 8,        // LSB of battery Low threshold - RW
  BCRH, 8,        // MSB of battery Low threshold - RW

  BCRL, 8,        // LSB of battery Critical threshold - RW
  BLOH, 8,        // MSB of battery Critical threshold - RW

  BTPL, 8,        // LSB of BTP (Battery Trip Point) - RW
  BTPH, 8,        // MSB of BTP (Battery Trip Point) - RW

  BTML, 8,        // LSB of battery tempurature
  BTMH, 8,        // MSB of battery tempurature

  BMEL, 8,        // LSB of Battery max error Counter
  BMEH, 8,        // MSB of Battery max error Counter

  BCCL, 8,        // LSB of battery cycle counter
  BCCH, 8,        // MSB of battery cycle counter

  BTCL, 8,        // LSB of battery terminal current (0x0A)
  BTCH, 8,        // MSB of battery terminal current

  BTVL, 8,        // LSB of battery terminal voltage (0x09)
  BTVH, 8,        // MSB of battery terminal voltage

  DEVL, 8,        // LSB of battery design voltage() (0x19)
  DEVH, 8,        // MSB of battery design voltage()

  CAPL, 8,        // LSB of battery remaining capacity (0x0F)
  CAPH, 8,        // MSB of battery remaining capacity

  LFCL, 8,        // LSB of fully charged battery capacity (0x10)
  LFCH, 8,        // MSB of fully charged battery capacity

  DECL, 8,        // LSB of battery design capacity() (0x18)
  DECH, 8,        // MSB of battery design capacity()

  BCTR, 8,        // _DSM F1 charge throttle (0 to 100)

  BCON, 8,        // Indiactes how many batteries are supported by the system
  BTST, 8,        // Battery selector - to select other batteries is more than one battery are supported

  BATS, 8,        // Battery status
                  // Bit[0] = 1 if battery is charging or 0 for discharging
                  // Bit[7] = 1 if battery was tripped. Write 1 to clear. 
                  //          _Q3E should be fired until this bit is cleared.
  CHSH, 8,        // Charger status
                  // Bit[6] = 1 if battery is connected
                  // Bit[7] = 1 if AC is connected
  BALT, 8,        // Bit[7:0] = Each bit stands for one battery. 
                  //            An one indicates the battery has alert. Read to clear

  Offset(0xF0),
  KBFG, 8,
  SYSK, 8,        // Store uPEP hook function ID

  Offset(0xF2),
  /* To atomic read  a double-byte word, read the HIGH byte first */
  DIEL, 8,        // Die temp low byte   SB_TSIx[10]
  DIEH, 8,        // Die temp high byte  SB_TSIx[01]
                  //
                  // Number encoding examples
                  //                 High       Low
                  //  0.000 degree C 0000_0000b 0000_0000b
                  //  1.000 degree C 0000_0001b 0000_0000b
                  // 25.125 degree C 0001_1001b 0010_0000b
                  // 50.875 degree C 0011_0010b 1110_0000b
                  // 90.000 degree C 0101_1010b 0000_0000b
                  //
  TSRL, 8,        // TMP432 sensor local temp low byte
  TSRH, 8,        // TMP432 sensor local temp high byte
  TS1L, 8,        // TMP432 remote Q1 temp low byte
  TS1H, 8,        // TMP432 remote Q1 temp high byte
  TS2L, 8,        // TMP432 remote Q2 temp low byte
  TS2H, 8,        // TMP432 remote Q2 temp high byte
  
  Offset(0xFE),
  USBS, 8,        // USBC Status
                  // Bit[0] = 1 if USBC crossbar ready or 0 not ready
                  // Bit[1] = 1 force reconfigure USBC crossbar
                  // Bit[2] = 1 Rembrandt A0 (USBC crossbar version A0)
                  // Bit[3] = 1 Rembrandt B0 (USBC crossbar version B0)
                  // Bit[4] = 1 Disable USB4 or 0 enable USB4
                  // Bit[5] = 1 Disable TBT3 or 0 enable TBT3
                  // Bit[6] = 1 Disable USBC ports or 0 enable USBC ports
}

