/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH registers definition
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 309083 $   @e \$Date: 2014-12-09 09:28:24 -0800 (Tue, 09 Dec 2014) $
 *
 */
#define FCH_REVISION_KEITH                  "0.8.0.1"
#define FCH_ID_KEITH                        "FCH_801"
#define FCH_VERSION_KEITH                   0x0801

#define KEITH_MAX_FCH_SUPPORT               1

#define AMD_D0F0   ((0 << 3) + 0)
#define AMD_D18F0  ((0x18 << 3) + 0)

#define SOC_RMBID                             0x14B51022ul
#define DF_RMBID                              0x16791022ul

//CPU Package Type

#define KEITH_FCH_SATA_DID                  0x7901
#define KEITH_FCH_SATA_AHCI_DID             0x7901
#define KEITH_FCH_SATA_AMDAHCI_DID          0x7904
#define KEITH_FCH_SATA_RAID_DID             0x7916
#define KEITH_FCH_SATA_RAID_DID2            0x7917

#define KEITH_FCH_USB_XHCI_DID              0x163A
#define KEITH_FCH_SMBUS_DID                 0x790B      // Dev 0x14 Func 0
#define KEITH_FCH_LPC_DID                   0x790E      // Dev 0x14 Func 3
#define KEITH_FCH_SD_DID                    0x7906      // Dev 0x14 Func 7

//
// SD controller
//
#define KEITH_SD_BUS_DEV_FUN                ((0x14 << 3) + 6)
#define KEITH_SD_PCI_DEV                    20
#define KEITH_SD_PCI_FUNC                   6

//
// SATA
//
#define KEITH_SATA_CONTROLLER_NUM           1
#define KEITH_SATA_PORT_NUM                 2

//SATA0 RSMU registers
#define FCH_KT_SMN_SATA_RSMU_BASE            0x090A8000ul
#define FCH_KT_SMN_SATA_CONTROL_BASE         0x03100000ul
#define FCH_KT_SMN_SATA_CONTROL_RSMU         FCH_KT_SMN_SATA_CONTROL_BASE
#define FCH_KT_SMN_SATA_CONTROL_BAR5         FCH_KT_SMN_SATA_CONTROL_BASE + 0x1000
#define FCH_KT_SMN_SATA_CONTROL_SLOR         FCH_KT_SMN_SATA_CONTROL_BASE + 0x1800

//SATA1 Address Offset
#define FCH_KT_SMN_SATA_RSMU_BASE_STEP       0x1000ul
#define FCH_KT_SMN_SATA_CONTROL_STEP         0x100000ul

#define FCH_KT_SATA_RSMU_COLD_RESETB         FCH_KT_SMN_SATA_RSMU_BASE + 0x04
#define FCH_KT_SATA_RSMU_HARD_RESETB         FCH_KT_SMN_SATA_RSMU_BASE + 0x08

#define FCH_KT_SATA_MISC_CONTROL             FCH_KT_SMN_SATA_CONTROL_RSMU + 0x00     //0x00
#define FCH_KT_SATA_OOB_CONTROL              FCH_KT_SMN_SATA_CONTROL_RSMU + 0x02     //0x02
#define FCH_KT_SATA_AOAC_CONTROL             FCH_KT_SMN_SATA_CONTROL_RSMU + 0x04     //0x04
#define FCH_KT_SATA_EVENT_SELECT             FCH_KT_SMN_SATA_CONTROL_RSMU + 0x08     //0x08
#define FCH_KT_SATA_NBIF_CONTROL             FCH_KT_SMN_SATA_CONTROL_RSMU + 0x0A     //0x0A
//  Sata0: nBIF0 DEV1 Func0
#define FCH_KT_SMN_SATA0_NBIF_STRAP0         0x10135000ul
#define FCH_KT_SMN_SATA0_NBIF_STRAP2         0x10135008ul
#define FCH_KT_SMN_SATA0_NBIF_STRAP3         0x1013500Cul
#define FCH_KT_SMN_SATA0_NBIF_STRAP4         0x10135010ul
#define FCH_KT_SMN_SATA0_NBIF_STRAP13        0x10135034ul
#define FCH_KT_SATA0_SMN_PCICFG              0x10148000ul
//  Sata1
#define FCH_KT_SMN_SATA1_NBIF_STRAP0         0x10135200ul
#define FCH_KT_SMN_SATA1_NBIF_STRAP2         0x10135208ul
#define FCH_KT_SMN_SATA1_NBIF_STRAP3         0x1013520Cul
#define FCH_KT_SMN_SATA1_NBIF_STRAP4         0x10135210ul
#define FCH_KT_SMN_SATA1_NBIF_STRAP13        0x10135234ul
#define FCH_KT_SATA1_SMN_PCICFG              0x10149000ul
#define FCH_KT_SMN_SATA_NBIF_STRAP_STEP      0x200ul
#define FCH_KT_SATA1_SMN_PCICFG_STEP         0x1000ul
//  Sata: NBIF1DEVINDCFG IOHC_Bridge_CNTL
#define FCH_KT_NBIF1DEVINDCFG_IOHC_Bridge_CNTL               0x13B38404ul
#define FCH_KT_NBIF1DEVINDCFG_IOHC_Bridge_CNTL_NBIO_STEP     0x00100000ul
#define FCH_KT_NBIF1DEVINDCFG_IOHC_Bridge_CNTL_PORT_STEP     0x00000400ul

#define FCH_KT_RCC_DEV1_PORT_STRAP0           0x10131200ul

//
// USB
//
//  RMB has 3 XHCI controller
// All register in SMU message should be offset instead of full address
#define FCH_KT_USB0_SMN_BASE                 0x16C00000ul        //0x16C00000ul
#define FCH_KT_USB1_SMN_BASE                 0x16E00000ul        //0x16E00000ul
#define FCH_KT_USB2_SMN_BASE                 0x17000000ul        //0x17000000ul

//  RMB B0 2 more XHCI controller
#define FCH_KT_USB3_SMN_BASE                 0x17200000ul        //0x17200000ul
#define FCH_KT_USB4_SMN_BASE                 0x19500000ul        //0x19500000ul

#define FCH_KT_USB_CONTROLLER_SMN_STEP       0x200000ul          //0x16E00000ul

//USB RSMU registers
#define FCH_KT_USB_RSMU_PGFSM_CNTL           0x0004D520ul        //0x0004D520ul
#define FCH_KT_USB_RSMU_PGFSM_WR_DATA        0x0004D524ul        //0x0004D524ul
#define FCH_KT_USB_RSMU_PGFSM_RD_DATA        0x0004D528ul        //0x0004D528ul

//USB3 IP registers
#define FCH_KT_USBOFFSET_GRXTHRCFG           0xC10Cul            //0xC10C
#define FCH_KT_USBOFFSET_GUCTL1              0xC11Cul            //0xC11C
#define FCH_KT_USBOFFSET_GUCTL               0xC12Cul            //0xC12C
#define FCH_KT_USBOFFSET_GSYSBLKWINCTRL      0xC1B0ul            //0xC1B0
#define FCH_KT_USBOFFSET_GUSB3PIPECTL0       0xC2C0ul            //0xC2C0
#define FCH_KT_USBOFFSET_GUSB3PIPECTL1       0xC2C4ul            //0xC2C4
#define FCH_KT_USBOFFSET_GSMACCTL            0xC604ul            //0xC604
#define FCH_KT_USBOFFSET_GUCTL2              0xC608ul            //0xC608
#define FCH_KT_USBOFFSET_GUCTL3              0xC60Cul            //0xC60C
#define FCH_KT_USBOFFSET_S0LU1LFPSRXTIM      0xD000ul            //0xD000
#define FCH_KT_USBOFFSET_S0LU3LFPSRXTIM      0xD010ul            //0xD010
#define FCH_KT_USBOFFSET_S0LSKIPFREQ         0xD020ul            //0xD020
#define FCH_KT_USBOFFSET_S0LLUCTL            0xD024ul            //0xD024
#define FCH_KT_USBOFFSET_S0LPTMDPDELAY       0xD028ul            //0xD028
#define FCH_KT_USBOFFSET_S1LU1LFPSRXTIM      0xD080ul            //0xD080
#define FCH_KT_USBOFFSET_S1LU3LFPSRXTIM      0xD090ul            //0xD090
#define FCH_KT_USBOFFSET_S1LSKIPFREQ         0xD0A0ul            //0xD0A0
#define FCH_KT_USBOFFSET_S1LLUCTL            0xD0A4ul            //0xD0A4
#define FCH_KT_USBOFFSET_S1LPTMDPDELAY       0xD0A8ul            //0xD0A8

#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS0_LCSR_TX_DEEMPH      0xD060ul            //0xD060
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS0_LCSR_TX_DEEMPH_1    0xD064ul            //0xD064
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS0_LCSR_TX_DEEMPH_2    0xD068ul            //0xD068
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS0_LCSR_TX_DEEMPH_3    0xD06Cul            //0xD06C
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS1_LCSR_TX_DEEMPH      0xD0E0ul            //0xD0E0
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS1_LCSR_TX_DEEMPH_1    0xD0E4ul            //0xD0E4
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS1_LCSR_TX_DEEMPH_2    0xD0E8ul            //0xD0E8
#define FCH_KT_FCHUSBDWCUSB31CNTR0_LINK_REGS1_LCSR_TX_DEEMPH_3    0xD0ECul            //0xD0EC

//USB2 PHY Registers
#define FCH_KT_USB20_PHY_SMN_STEP            0x400ul             //0x400
#define FCH_KT_USB20_LANEPARACTL0            0x11C000ul          //0x11C000
#define FCH_KT_USB20_LANEPARACTL1            0x11C004ul          //0x11C004

//USB3 PHY Parameter Registers
#define FCH_KT_USB3_PHY_SMN_STEP             0x400ul             //0x400
#define FCH_KT_USB3_PHYPARACTL0              0x168000ul          //0x168000
#define FCH_KT_USB3_PHYPARACTL10             0x168028ul          //0x168028
#define FCH_KT_USB3_PHYPARACTL16             0x168040ul          //0x168040

//USB31LANEPARAMCTLREGCNTR0
#define FCH_KT_USB3_LANEPARACTL1_S0          0x16C004ul          //0x16C004
#define FCH_KT_USB3_LANEPARACTL1_S1          0x16C404ul          //0x16C404

//USB3 PHY Internal Registers
#define FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN      0x120088ul          //0x120088


//USB TCA Registers
#define FCH_KT_USBOFFSET_TCA_TCPC            0x1F8814ul          //0x1F8814

//USB3 CONTAINER Registers
#define FCH_SMN_USB_CONTAINER                0x00180000ul        //changed to offset
#define FCH_KT_USB_SHADOWED_POWERSTATE       FCH_SMN_USB_CONTAINER + 0x14         //0x14
#define FCH_KT_USB_RESET                     FCH_SMN_USB_CONTAINER + 0x100        //0x100
  #define FCH_KT_USB_S5_RESET                 (1 << 0x00)
  #define FCH_KT_USB_CONTROLLER_RESET         (1 << 0x01)
  #define FCH_KT_USB_PHY_RESET                (1 << 0x02)
#define FCH_KT_USB_PGFSM_OVERRIDE            FCH_SMN_USB_CONTAINER + 0x104        //0x104
#define FCH_KT_USB_CURRENT_PWR_STS           FCH_SMN_USB_CONTAINER + 0x108        //0x108
#define FCH_KT_USB_DEVICE_REMOVABLE          FCH_SMN_USB_CONTAINER + 0x114        //0x114
#define FCH_KT_USB_INTERRUPT_CONTROL         FCH_SMN_USB_CONTAINER + 0x118        //0x118
#define FCH_KT_USB_PORT_OCMAPPING0           FCH_SMN_USB_CONTAINER + 0x120        //0x120
#define FCH_KT_USB_PORT_DISABLE0             FCH_SMN_USB_CONTAINER + 0x12C        //0x12C
#define FCH_KT_USB_PORT_CONTROL              FCH_SMN_USB_CONTAINER + 0x130        //0x130
#define FCH_KT_USB_BCCONFIG1_CONTROL0        FCH_SMN_USB_CONTAINER + 0x280        //0x280

#define FCH_KT_USB0_NBIF_STRAP0              0x10134600ul
#define FCH_KT_USB0_SMN_PCICFG               0x10143000ul
#define FCH_KT_USB1_NBIF_STRAP0              0x10134800ul
#define FCH_KT_USB1_SMN_PCICFG               0x10144000ul
#define FCH_KT_USB2_NBIF_STRAP0              0x10136000ul
#define FCH_KT_USB2_NBIF_STRAP2              0x10136008ul
#define FCH_KT_USB2_NBIF_STRAP3              0x1013600Cul
#define FCH_KT_USB2_NBIF_STRAP4              0x10136010ul
#define FCH_KT_USB2_NBIF_STRAP13             0x10136034ul
#define FCH_KT_USB2_SMN_PCICFG               0x10150000ul

#define FCH_KT_RCC_DEV2_PORT_STRAP0          0x10131400ul
#define FCH_KT_NBIF_VWR_VCHG_DIS_CTRL        0x1013A204ul

#define FCH_KT_USB_T_D3ENTRY                 5000                 // 5ms Timeout for D3 entry

//
// USB4
//

//  RMB has 2 USB4 Rpouter
#define FCH_KT_USB4_ROUTER0_SMN_BASE         0x19700000ul        //0x19700000ul
#define FCH_KT_USB4_ROUTER1_SMN_BASE         0x19800000ul        //0x19800000ul
#define FCH_KT_USB4_RT0_PHY_SMN_BASE         0x19900000ul        //0x19900000ul
#define FCH_KT_USB4_RT1_PHY_SMN_BASE         0x19A00000ul        //0x19A00000ul

#define FCH_KT_RT0_NBIF_STRAP0              0x10136A00ul
#define FCH_KT_RT1_NBIF_STRAP0              0x10136C00ul
#define FCH_KT_HC3_NBIF_STRAP0              0x10136600ul
#define FCH_KT_HC7_NBIF_STRAP0              0x10136800ul

#define FCH_KT_USB_CONFIG_USB4RT0           0x5
#define FCH_KT_USB_CONFIG_USB4PHY0          0x6
#define FCH_KT_USB_CONFIG_USB4RT1           0x7
#define FCH_KT_USB_CONFIG_USB4PHY1          0x8
#define FCH_KT_USB_CONFIG_RT_STEP           0X2

#define FCH_KT_USB4_CFG_CONFIG_SPACE_HIA_MAX_CNTR0        0xD3834ul
#define FCH_KT_USB4_CFG_CONFIG_SPACE_MAX_OUTPUT_HOPID_CNTR0    0xD3848ul
#define FCH_KT_USB4_CFG_CMD_FSM_CNTR_CNTR0                0xD384Cul
#define FCH_KT_USB4_S0_HIA_Path1_ConfigurationB_CNTR0     0xD440Cul
#define FCH_KT_USB4_S0_HIA_Path2_ConfigurationB_CNTR0     0xD4414ul
#define FCH_KT_USB4_S0_HIA_Path3_ConfigurationB_CNTR0     0xD441Cul
#define FCH_KT_USB4_S0_HIA_Path4_ConfigurationB_CNTR0     0xD4424ul
#define FCH_KT_USB4_S0_HIA_Path5_ConfigurationB_CNTR0     0xD442Cul
#define FCH_KT_USB4_S0_HIA_Path6_ConfigurationB_CNTR0     0xD4434ul
#define FCH_KT_USB4_S0_HIA_MiscControl_1_CNTR0            0xD44ECul
#define FCH_KT_USB4_TMU_PRIVATE_CS_14_CNTR0               0xD4838ul
#define FCH_KT_USB4_TMU_PRIVATE_CS_18_CNTR0               0xD4848ul
#define FCH_KT_USB4_TL_HIA_ADP_CS_1_OWR_CNTR0             0xD5104ul
#define FCH_KT_USB4_TL_HIA_ADP_CS_5_OWR_CNTR0             0xD5114ul
#define FCH_KT_USB4_CIO_P0_EG_CONFIG3_CNTR0               0xD5240ul
#define FCH_KT_USB4_CIO_P0_ECO_Dummy0_CNTR0               0xD5278ul
#define FCH_KT_USB4_CFG_LL_PORT_CNTR                      0xD820Cul
#define FCH_KT_USB4_CFG_LL_PORT_TIMER_PARAMS              0xD8240ul
#define FCH_KT_USB4_CFG_LL_PRTNR_PORT_SB_LL_CONFIG        0xD825Cul
#define FCH_KT_USB4_CFG_LL_OVRD_PORT_SB_LL_CONFIGURATION  0xD8264ul
#define FCH_KT_USB4_CFG_LL_OVRD_ADP_CS                    0xD8270ul
#define FCH_KT_USB4_CFG_LL_INTERRUPT_EN                   0xD8278ul
#define FCH_KT_USB4_CFG_FW_PORT_OP_CTRL                   0xD8348ul
#define FCH_KT_USB4_LL_CMNHS_LASM_CTR0_CNTR0              0xD8600ul
#define FCH_KT_USB4_LL_CMNHS_LPM_CTR_STATUS_CNTR0         0xD8604ul
#define FCH_KT_USB4_LL_HS_TXFFE_OVERRIDE_REG2CNTR0        0xD8618ul
#define FCH_KT_USB4_LL_HS_INTERRUPT_EN_CNTR0              0xD89B0ul

#define FCH_KT_USB4_TBT3_NOT_SUPPORT_CNTR0  0xE7B04ul
#define FCH_KT_USB4_ADP_HIDE_CNTR0          0xE7B08ul
#define FCH_KT_USB4_TUNNEL_DISABLE_CNTR0    0xE7B0Cul

#define FCH_KT_USB4_MSG_S5_LL_PIPE_TO       0xF880Cul

#define FCH_KT_USB4_USB_PD_Slave_Control    0x70680ul
#define FCH_KT_USB4_USB_PD_Interrupt_Mask   0x706A0ul

#define FCH_KT_USB4_TCA_CLK_RST             0x70800ul
#define FCH_KT_USB4_TCA_CTRLSYNCMODE_CFG1   0x70824ul

//
// USB4 PCIe Controller
//
#define FCH_KT_PCIE2_BRIDGE_CNTL             0x13B35004ul
#define FCH_KT_PCIE3_BRIDGE_CNTL             0x13B35404ul

//
// WLAN
//
#define FCH_KT_WLAN_SMN_BASE                 0x05700000ul         //0x05700000ul

#define FCH_KT_WLAN_PHY_GLB_SETTING_CTRL4    FCH_KT_WLAN_SMN_BASE + 0x0110130ul         //0x0110130ul
#define FCH_KT_WLAN_STOP_CLK_REG             FCH_KT_WLAN_SMN_BASE + 0x010000Cul         //0x010000Cul
#define FCH_KT_WLAN_CLK_REG                  FCH_KT_WLAN_SMN_BASE + 0x010001Cul         //0x010001Cul
#define FCH_KT_WLAN_PWR_STATE_REG            FCH_KT_WLAN_SMN_BASE + 0x0100024ul         //0x0100024ul

//XGBE registers
#define FCH_KT_SMN_XGBE_MMIO0_BASE           0x1631F000ul

#define FCH_KT_XGBE_PAD_MUX0                 FCH_KT_SMN_XGBE_MMIO0_BASE + 0x00
#define FCH_KT_XGBE_PAD_MUX1                 FCH_KT_SMN_XGBE_MMIO0_BASE + 0x04
#define FCH_KT_XGBE_PORT0_CLK_GATE           FCH_KT_SMN_XGBE_MMIO0_BASE + 0x30
#define FCH_KT_XGBE_PORT0_PROPERTY0          FCH_KT_SMN_XGBE_MMIO0_BASE + 0x800
#define FCH_KT_XGBE_PORT0_PROPERTY1          FCH_KT_SMN_XGBE_MMIO0_BASE + 0x804
#define FCH_KT_XGBE_PORT0_PROPERTY2          FCH_KT_SMN_XGBE_MMIO0_BASE + 0x808
#define FCH_KT_XGBE_PORT0_PROPERTY3          FCH_KT_SMN_XGBE_MMIO0_BASE + 0x80C
#define FCH_KT_XGBE_PORT0_PROPERTY4          FCH_KT_SMN_XGBE_MMIO0_BASE + 0x810
#define FCH_KT_XGBE_PORT0_PROPERTY5          FCH_KT_SMN_XGBE_MMIO0_BASE + 0x814
#define FCH_KT_XGBE_PORT0_MAC_ADDR_LOW       FCH_KT_SMN_XGBE_MMIO0_BASE + 0x880
#define FCH_KT_XGBE_PORT0_MAC_ADDR_HIGH      FCH_KT_SMN_XGBE_MMIO0_BASE + 0x884

#define FCH_NBIFSHUBMM0x00011000_SYSHUB_DS_CTRL_SHUBCLK 0x01411000ul
#define FCH_DMA_CL3_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE BIT19

#define FCH_KT_RCC_DEV1_EPF2_STRAP0          0x10135400ul
#define FCH_KT_SMN_XGBE0_NBIF_STRAP0         0x10234800ul

//FCH register SMN address
#define FCH_SMN_SMI_BASE                     0x02D01200ul
#define FCH_SMN_IOMUX_BASE                   0x02D01D00ul
#define FCH_SMN_MISC_BASE                    0x02D01E00ul
#define FCH_SMN_GPIO_BASE                    0x02D02500ul

#define FCH_KT_IRQ_INTA         0x00
#define FCH_KT_IRQ_INTB         0x01
#define FCH_KT_IRQ_INTC         0x02
#define FCH_KT_IRQ_INTD         0x03
#define FCH_KT_IRQ_GENINT2      0x05
#define FCH_KT_IRQ_SCI          0x10    // SCI
#define FCH_KT_IRQ_SMBUS0       0x11    // SMBUS0
#define FCH_KT_IRQ_ASF          0x12    // ASF
#define FCH_KT_IRQ_SD           0x17    // SD
#define FCH_KT_IRQ_SDIO         0x1A    // SDIO
#define FCH_KT_IRQ_GPIOa        0x21
#define FCH_KT_IRQ_GPIOb        0x22
#define FCH_KT_IRQ_GPIOc        0x23
#define FCH_KT_IRQ_SATA         0x41    // SATA
#define FCH_KT_IRQ_EMMC         0x43    // EMMC
#define FCH_KT_IRQ_GPIO         0x62    // GPIO
#define FCH_KT_IRQ_I2C0         0x70    // I2C0
#define FCH_KT_IRQ_I2C1         0x71    // I2C1
#define FCH_KT_IRQ_I2C2         0x72    // I2C2
#define FCH_KT_IRQ_I2C3         0x73    // I2C3
#define FCH_KT_IRQ_UART0        0x74    // UART0
#define FCH_KT_IRQ_UART1        0x75    // UART1
#define FCH_KT_IRQ_I2C4         0x76    // I2C4
#define FCH_KT_IRQ_UART4        0x77    // UART4
#define FCH_KT_IRQ_UART2        0x78    // UART2
#define FCH_KT_IRQ_UART3        0x79    // UART3

//SciMap
#define FCH_SMI_SciMap0         0x40                          // SciMap0
#define FCH_SMI_KT_GENINT1      (FCH_SMI_SciMap0 + 0)         // SciMap0
#define FCH_SMI_KT_GENINT2      (FCH_SMI_SciMap0 + 1)         // SciMap1
#define FCH_SMI_KT_AGPIO3       (FCH_SMI_SciMap0 + 2)         // SciMap2
#define FCH_SMI_KT_LpcPme       (FCH_SMI_SciMap0 + 3)         // SciMap3
#define FCH_SMI_KT_AGPIO4       (FCH_SMI_SciMap0 + 4)         // SciMap4
#define FCH_SMI_KT_LpcPd        (FCH_SMI_SciMap0 + 5)         // SciMap5
#define FCH_SMI_KT_SPKR         (FCH_SMI_SciMap0 + 6)         // SciMap6
#define FCH_SMI_KT_AGPIO5       (FCH_SMI_SciMap0 + 7)         // SciMap7
#define FCH_SMI_KT_WAKE         (FCH_SMI_SciMap0 + 8)         // SciMap8
#define FCH_SMI_KT_LpcSmi       (FCH_SMI_SciMap0 + 9)         // SciMap9
#define FCH_SMI_KT_AGPIO6       (FCH_SMI_SciMap0 + 10)        // SciMap10
#define FCH_SMI_KT_AGPIO76      (FCH_SMI_SciMap0 + 11)        // SciMap11
#define FCH_SMI_KT_UsbOc0       (FCH_SMI_SciMap0 + 12)        // SciMap12
#define FCH_SMI_KT_UsbOc1       (FCH_SMI_SciMap0 + 13)        // SciMap13
#define FCH_SMI_KT_UsbOc2       (FCH_SMI_SciMap0 + 14)        // SciMap14
#define FCH_SMI_KT_UsbOc3       (FCH_SMI_SciMap0 + 15)        // SciMap15
#define FCH_SMI_KT_AGPIO23      (FCH_SMI_SciMap0 + 16)        // SciMap16
#define FCH_SMI_KT_EspiReset    (FCH_SMI_SciMap0 + 17)        // SciMap17
#define FCH_SMI_KT_FanIn0       (FCH_SMI_SciMap0 + 18)        // SciMap18
#define FCH_SMI_KT_SysReset     (FCH_SMI_SciMap0 + 19)        // SciMap19
#define FCH_SMI_KT_AGPIO40      (FCH_SMI_SciMap0 + 20)        // SciMap20
#define FCH_SMI_KT_PwrBtn       (FCH_SMI_SciMap0 + 21)        // SciMap21
#define FCH_SMI_KT_AGPIO9       (FCH_SMI_SciMap0 + 22)        // SciMap22
#define FCH_SMI_KT_AGPIO8       (FCH_SMI_SciMap0 + 23)        // SciMap23
#define FCH_SMI_KT_Mp2Wake      (FCH_SMI_SciMap0 + 24)        // SciMap24
#define FCH_SMI_KT_Mp2Gpio0     (FCH_SMI_SciMap0 + 25)        // SciMap25
#define FCH_SMI_KT_eSPI         (FCH_SMI_SciMap0 + 26)        // SciMap26
#define FCH_SMI_KT_EspiWakePme  (FCH_SMI_SciMap0 + 27)        // SciMap27
#define FCH_SMI_KT_Mp2Gpio1     (FCH_SMI_SciMap0 + 25)        // SciMap28
#define FCH_SMI_KT_NbGppPme     (FCH_SMI_SciMap0 + 29)        // SciMap29
#define FCH_SMI_KT_NbGppHp      (FCH_SMI_SciMap0 + 30)        // SciMap30
#define FCH_SMI_KT_FakeSts0     (FCH_SMI_SciMap0 + 33)        // SciMap33
#define FCH_SMI_KT_FakeSts1     (FCH_SMI_SciMap0 + 34)        // SciMap34
#define FCH_SMI_KT_FakeSts2     (FCH_SMI_SciMap0 + 35)        // SciMap35
#define FCH_SMI_KT_CioFchPme0   (FCH_SMI_SciMap0 + 37)        // SciMap37
#define FCH_SMI_KT_CioFchPme1   (FCH_SMI_SciMap0 + 38)        // SciMap38
#define FCH_SMI_KT_AzaliaPme    (FCH_SMI_SciMap0 + 39)        // SciMap39
#define FCH_SMI_KT_GpioInt      (FCH_SMI_SciMap0 + 41)        // SciMap41
#define FCH_SMI_KT_Xhc2Wake     (FCH_SMI_SciMap0 + 42)        // SciMap42
#define FCH_SMI_KT_AltHPET      (FCH_SMI_SciMap0 + 43)        // SciMap43
#define FCH_SMI_KT_FanThGevent  (FCH_SMI_SciMap0 + 44)        // SciMap44
#define FCH_SMI_KT_ASF          (FCH_SMI_SciMap0 + 45)        // SciMap45
#define FCH_SMI_KT_I2sWake      (FCH_SMI_SciMap0 + 46)        // SciMap46
#define FCH_SMI_KT_Smbus0       (FCH_SMI_SciMap0 + 47)        // SciMap47
#define FCH_SMI_KT_TWRN         (FCH_SMI_SciMap0 + 48)        // SciMap48
#define FCH_SMI_KT_TrafficMon   (FCH_SMI_SciMap0 + 49)        // SciMap49
#define FCH_SMI_KT_LLB          (FCH_SMI_SciMap0 + 50)        // SciMap50
#define FCH_SMI_KT_PwrBtnEvt    (FCH_SMI_SciMap0 + 51)        // SciMap51
#define FCH_SMI_KT_PROHOT       (FCH_SMI_SciMap0 + 52)        // SciMap52
#define FCH_SMI_KT_ApuHw        (FCH_SMI_SciMap0 + 53)        // SciMap53
#define FCH_SMI_KT_ApuSci       (FCH_SMI_SciMap0 + 54)        // SciMap54
#define FCH_SMI_KT_RasEvt       (FCH_SMI_SciMap0 + 55)        // SciMap55
#define FCH_SMI_KT_Xhc0Wake     (FCH_SMI_SciMap0 + 56)        // SciMap56
#define FCH_SMI_KT_Xhc1Wake     (FCH_SMI_SciMap0 + 57)        // SciMap57
#define FCH_SMI_KT_AcDcTimer    (FCH_SMI_SciMap0 + 58)        // SciMap58
#define FCH_SMI_KT_Xhc3Wake     (FCH_SMI_SciMap0 + 61)        // SciMap61
#define FCH_SMI_KT_Xhc4Wake     (FCH_SMI_SciMap0 + 62)        // SciMap62

#define USB4_X4NS_CMN_USB32DPALT_GEN1_WIDTH10                  1
#define USB4_X4NS_CMN_USB32DPALT_GEN1_WIDTH20                  2
#define USB4_X4NS_CMN_USB32DPALT_GEN2_WIDTH16                  3
#define USB4_X4NS_CMN_USB32DPALT_GEN2_WIDTH32                  4
#define USB4_X4NS_CMN_USB4_GEN2_WIDTH16                        5
#define USB4_X4NS_CMN_USB4_GEN2_WIDTH32                        6
#define USB4_X4NS_CMN_USB4_GEN3_WIDTH16                        7
#define USB4_X4NS_CMN_USB4_GEN3_WIDTH32                        8
#define USB4_X4NS_CMN_DP_RBR_WIDTH10                           9
#define USB4_X4NS_CMN_DP_RBR_WIDTH20                           10
#define USB4_X4NS_CMN_DP_RBR_WIDTH40                           11
#define USB4_X4NS_CMN_DP_HBR1_WIDTH10                          12
#define USB4_X4NS_CMN_DP_HBR1_WIDTH20                          13
#define USB4_X4NS_CMN_DP_HBR1_WIDTH40                          14
#define USB4_X4NS_CMN_DP_HBR2_WIDTH10                          15
#define USB4_X4NS_CMN_DP_HBR2_WIDTH20                          16
#define USB4_X4NS_CMN_DP_HBR2_WIDTH40                          17
#define USB4_X4NS_CMN_DP_HBR3_WIDTH10                          18
#define USB4_X4NS_CMN_DP_HBR3_WIDTH20                          19
#define USB4_X4NS_CMN_DP_HBR3_WIDTH40                          20
#define USB4_X4NS_CMN_DP_UHBR10_WIDTH16                        21
#define USB4_X4NS_CMN_DP_UHBR10_WIDTH32                        22
#define USB4_X4NS_CMN_DP_UHBR13P5_WIDTH16                      23
#define USB4_X4NS_CMN_DP_UHBR13P5_WIDTH32                      24
#define USB4_X4NS_CMN_DP_UHBR20_WIDTH16                        25
#define USB4_X4NS_CMN_DP_UHBR20_WIDTH32                        26
#define USB4_X4NS_CMN_EDP_EDP2P16_WIDTH10                      27
#define USB4_X4NS_CMN_EDP_EDP2P16_WIDTH20                      28
#define USB4_X4NS_CMN_EDP_EDP2P16_WIDTH40                      29
#define USB4_X4NS_CMN_EDP_EDP2P43_WIDTH10                      30
#define USB4_X4NS_CMN_EDP_EDP2P43_WIDTH20                      31
#define USB4_X4NS_CMN_EDP_EDP2P43_WIDTH40                      32
#define USB4_X4NS_CMN_EDP_EDP3P24_WIDTH10                      33
#define USB4_X4NS_CMN_EDP_EDP3P24_WIDTH20                      34
#define USB4_X4NS_CMN_EDP_EDP3P24_WIDTH40                      35
#define USB4_X4NS_CMN_EDP_EDP4P32_WIDTH10                      36
#define USB4_X4NS_CMN_EDP_EDP4P32_WIDTH20                      37
#define USB4_X4NS_CMN_EDP_EDP4P32_WIDTH40                      38
#define USB4_X4NS_CMN_EDP_EDP6P48_WIDTH10                      39
#define USB4_X4NS_CMN_EDP_EDP6P48_WIDTH20                      40
#define USB4_X4NS_CMN_EDP_EDP6P48_WIDTH40                      41
#define USB4_X4NS_CMN_EDP_EDP6P75_WIDTH10                      42
#define USB4_X4NS_CMN_EDP_EDP6P75_WIDTH20                      43
#define USB4_X4NS_CMN_EDP_EDP6P75_WIDTH40                      44
#define USB4_X4NS_CMN_THUNDERBOLT3_TBT10P3125G_WIDTH16         45
#define USB4_X4NS_CMN_THUNDERBOLT3_TBT10P3125G_WIDTH32         46
#define USB4_X4NS_CMN_THUNDERBOLT3_TBT20P625G_WIDTH16          47
#define USB4_X4NS_CMN_THUNDERBOLT3_TBT20P625G_WIDTH32          48
#define USB4_X4NS_CMN_GENERIC_CNTX_A                           49
#define USB4_X4NS_CMN_GENERIC_CNTX_B                           50

#define USB4_X4NS_MPLLA_USB32DPALT_GEN1_WIDTH10                1
#define USB4_X4NS_MPLLA_USB32DPALT_GEN1_WIDTH20                2
#define USB4_X4NS_MPLLA_USB32DPALT_GEN2_WIDTH16                3
#define USB4_X4NS_MPLLA_USB32DPALT_GEN2_WIDTH32                4
#define USB4_X4NS_MPLLA_USB4_GEN2_WIDTH16                      5
#define USB4_X4NS_MPLLA_USB4_GEN2_WIDTH32                      6
#define USB4_X4NS_MPLLA_USB4_GEN3_WIDTH16                      7
#define USB4_X4NS_MPLLA_USB4_GEN3_WIDTH32                      8
#define USB4_X4NS_MPLLA_DP_UHBR10_WIDTH16                      9
#define USB4_X4NS_MPLLA_DP_UHBR10_WIDTH32                      10
#define USB4_X4NS_MPLLA_DP_UHBR20_WIDTH16                      11
#define USB4_X4NS_MPLLA_DP_UHBR20_WIDTH32                      12
#define USB4_X4NS_MPLLA_THUNDERBOLT3_TBT10P3125G_WIDTH16       13
#define USB4_X4NS_MPLLA_THUNDERBOLT3_TBT10P3125G_WIDTH32       14
#define USB4_X4NS_MPLLA_THUNDERBOLT3_TBT20P625G_WIDTH16        15
#define USB4_X4NS_MPLLA_THUNDERBOLT3_TBT20P625G_WIDTH32        16
#define USB4_X4NS_MPLLA_GENERIC_CNTX_A                         17
#define USB4_X4NS_MPLLA_GENERIC_CNTX_B                         18

#define USB4_X4NS_MPLLB_DP_RBR_WIDTH10                         1
#define USB4_X4NS_MPLLB_DP_RBR_WIDTH20                         2
#define USB4_X4NS_MPLLB_DP_RBR_WIDTH40                         3
#define USB4_X4NS_MPLLB_DP_HBR1_WIDTH10                        4
#define USB4_X4NS_MPLLB_DP_HBR1_WIDTH20                        5
#define USB4_X4NS_MPLLB_DP_HBR1_WIDTH40                        6
#define USB4_X4NS_MPLLB_DP_HBR2_WIDTH10                        7
#define USB4_X4NS_MPLLB_DP_HBR2_WIDTH20                        8
#define USB4_X4NS_MPLLB_DP_HBR2_WIDTH40                        9
#define USB4_X4NS_MPLLB_DP_HBR3_WIDTH10                        10
#define USB4_X4NS_MPLLB_DP_HBR3_WIDTH20                        11
#define USB4_X4NS_MPLLB_DP_HBR3_WIDTH40                        12
#define USB4_X4NS_MPLLB_DP_UHBR13P5_WIDTH16                    13
#define USB4_X4NS_MPLLB_DP_UHBR13P5_WIDTH32                    14
#define USB4_X4NS_MPLLB_EDP_EDP2P16_WIDTH10                    15
#define USB4_X4NS_MPLLB_EDP_EDP2P16_WIDTH20                    16
#define USB4_X4NS_MPLLB_EDP_EDP2P16_WIDTH40                    17
#define USB4_X4NS_MPLLB_EDP_EDP2P43_WIDTH10                    18
#define USB4_X4NS_MPLLB_EDP_EDP2P43_WIDTH20                    19
#define USB4_X4NS_MPLLB_EDP_EDP2P43_WIDTH40                    20
#define USB4_X4NS_MPLLB_EDP_EDP3P24_WIDTH10                    21
#define USB4_X4NS_MPLLB_EDP_EDP3P24_WIDTH20                    22
#define USB4_X4NS_MPLLB_EDP_EDP3P24_WIDTH40                    23
#define USB4_X4NS_MPLLB_EDP_EDP4P32_WIDTH10                    24
#define USB4_X4NS_MPLLB_EDP_EDP4P32_WIDTH20                    25
#define USB4_X4NS_MPLLB_EDP_EDP4P32_WIDTH40                    26
#define USB4_X4NS_MPLLB_EDP_EDP6P48_WIDTH10                    27
#define USB4_X4NS_MPLLB_EDP_EDP6P48_WIDTH20                    28
#define USB4_X4NS_MPLLB_EDP_EDP6P48_WIDTH40                    29
#define USB4_X4NS_MPLLB_EDP_EDP6P75_WIDTH10                    30
#define USB4_X4NS_MPLLB_EDP_EDP6P75_WIDTH20                    31
#define USB4_X4NS_MPLLB_EDP_EDP6P75_WIDTH40                    32
#define USB4_X4NS_MPLLB_GENERIC_CNTX_A                         33
#define USB4_X4NS_MPLLB_GENERIC_CNTX_B                         34

#define USB4_X4NS_TX_USB32DPALT_GEN1_WIDTH10                   1
#define USB4_X4NS_TX_USB32DPALT_GEN1_WIDTH20                   2
#define USB4_X4NS_TX_USB32DPALT_GEN2_WIDTH16                   3
#define USB4_X4NS_TX_USB32DPALT_GEN2_WIDTH32                   4
#define USB4_X4NS_TX_USB4_GEN2_WIDTH16                         5
#define USB4_X4NS_TX_USB4_GEN2_WIDTH32                         6
#define USB4_X4NS_TX_USB4_GEN3_WIDTH16                         7
#define USB4_X4NS_TX_USB4_GEN3_WIDTH32                         8
#define USB4_X4NS_TX_DP_RBR_WIDTH10                            9
#define USB4_X4NS_TX_DP_RBR_WIDTH20                            10
#define USB4_X4NS_TX_DP_RBR_WIDTH40                            11
#define USB4_X4NS_TX_DP_HBR1_WIDTH10                           12
#define USB4_X4NS_TX_DP_HBR1_WIDTH20                           13
#define USB4_X4NS_TX_DP_HBR1_WIDTH40                           14
#define USB4_X4NS_TX_DP_HBR2_WIDTH10                           15
#define USB4_X4NS_TX_DP_HBR2_WIDTH20                           16
#define USB4_X4NS_TX_DP_HBR2_WIDTH40                           17
#define USB4_X4NS_TX_DP_HBR3_WIDTH10                           18
#define USB4_X4NS_TX_DP_HBR3_WIDTH20                           19
#define USB4_X4NS_TX_DP_HBR3_WIDTH40                           20
#define USB4_X4NS_TX_DP_UHBR10_WIDTH16                         21
#define USB4_X4NS_TX_DP_UHBR10_WIDTH32                         22
#define USB4_X4NS_TX_DP_UHBR13P5_WIDTH16                       23
#define USB4_X4NS_TX_DP_UHBR13P5_WIDTH32                       24
#define USB4_X4NS_TX_DP_UHBR20_WIDTH16                         25
#define USB4_X4NS_TX_DP_UHBR20_WIDTH32                         26
#define USB4_X4NS_TX_EDP_EDP2P16_WIDTH10                       27
#define USB4_X4NS_TX_EDP_EDP2P16_WIDTH20                       28
#define USB4_X4NS_TX_EDP_EDP2P16_WIDTH40                       29
#define USB4_X4NS_TX_EDP_EDP2P43_WIDTH10                       30
#define USB4_X4NS_TX_EDP_EDP2P43_WIDTH20                       31
#define USB4_X4NS_TX_EDP_EDP2P43_WIDTH40                       32
#define USB4_X4NS_TX_EDP_EDP3P24_WIDTH10                       33
#define USB4_X4NS_TX_EDP_EDP3P24_WIDTH20                       34
#define USB4_X4NS_TX_EDP_EDP3P24_WIDTH40                       35
#define USB4_X4NS_TX_EDP_EDP4P32_WIDTH10                       36
#define USB4_X4NS_TX_EDP_EDP4P32_WIDTH20                       37
#define USB4_X4NS_TX_EDP_EDP4P32_WIDTH40                       38
#define USB4_X4NS_TX_EDP_EDP6P48_WIDTH10                       39
#define USB4_X4NS_TX_EDP_EDP6P48_WIDTH20                       40
#define USB4_X4NS_TX_EDP_EDP6P48_WIDTH40                       41
#define USB4_X4NS_TX_EDP_EDP6P75_WIDTH10                       42
#define USB4_X4NS_TX_EDP_EDP6P75_WIDTH20                       43
#define USB4_X4NS_TX_EDP_EDP6P75_WIDTH40                       44
#define USB4_X4NS_TX_THUNDERBOLT3_TBT10P3125G_WIDTH16          45
#define USB4_X4NS_TX_THUNDERBOLT3_TBT10P3125G_WIDTH32          46
#define USB4_X4NS_TX_THUNDERBOLT3_TBT20P625G_WIDTH16           47
#define USB4_X4NS_TX_THUNDERBOLT3_TBT20P625G_WIDTH32           48
#define USB4_X4NS_TX_GENERIC_CNTX_A                            49
#define USB4_X4NS_TX_GENERIC_CNTX_B                            50

#define USB4_X4NS_RX_USB32DPALT_GEN1_WIDTH10                   1
#define USB4_X4NS_RX_USB32DPALT_GEN1_WIDTH20                   2
#define USB4_X4NS_RX_USB32DPALT_GEN2_WIDTH16                   3
#define USB4_X4NS_RX_USB32DPALT_GEN2_WIDTH32                   4
#define USB4_X4NS_RX_USB4_GEN2_WIDTH16                         5
#define USB4_X4NS_RX_USB4_GEN2_WIDTH32                         6
#define USB4_X4NS_RX_USB4_GEN3_WIDTH16                         7
#define USB4_X4NS_RX_USB4_GEN3_WIDTH32                         8
#define USB4_X4NS_RX_THUNDERBOLT3_TBT10P3125G_WIDTH16          9
#define USB4_X4NS_RX_THUNDERBOLT3_TBT10P3125G_WIDTH32          10
#define USB4_X4NS_RX_THUNDERBOLT3_TBT20P625G_WIDTH16           11
#define USB4_X4NS_RX_THUNDERBOLT3_TBT20P625G_WIDTH32           12
#define USB4_X4NS_RX_GENERIC_CNTX_A                            13
#define USB4_X4NS_RX_GENERIC_CNTX_B                            14


#pragma pack (1)

typedef struct _MPLLA_CONTEXT_ {
  UINT16  multiplier : 12;
  UINT16  div_clk_en : 1;
  UINT16  fb_clk_div4_en : 1;
  UINT16  div16p5_clk_en : 1;
  UINT16  short_lock_en : 1;
  UINT16  div_multiplier : 8;
  UINT16  tx_clk_div : 3;
  UINT16  word_clk_div : 2;
  UINT16  ctl_buf_bypass : 1;
  UINT16  bw_threshold : 2;
  UINT16  bw_low : 16;
  UINT16  bw_high : 16;
  UINT16  ssc_peak_lsb : 16;
  UINT16  ssc_step_size_lsb : 16;
  UINT16  ssc_peak_msb : 4;
  UINT16  ssc_step_size_msb : 5;
  UINT16  ssc_up_spread : 1;
  UINT16  ref_clk_mplla_div : 3;
  UINT16  lc_freqsel : 1;
  UINT16  frac_en : 1;
  UINT16  reserved_15_15 : 1;
  UINT16  frac_den : 16;
  UINT16  frac_quot : 16;
  UINT16  frac_rem : 16;
  UINT16  Spare[2];
}MPLLA_CONTEXT;

typedef struct _MPLLB_CONTEXT_ {
  UINT16  multiplier : 12;
  UINT16  div_clk_en : 1;
  UINT16  tx_clk_div : 3;
  UINT16  div_multiplier : 8;
  UINT16  word_clk_div : 2;
  UINT16  cal_dac_code : 5;
  UINT16  reserved_15_15 : 1;
  UINT16  cp_int : 7;
  UINT16  cp_prop : 7;
  UINT16  freq_vco : 2;
  UINT16  cp_int_gs : 7;
  UINT16  cp_prop_gs : 7;
  UINT16  v2i : 2;
  UINT16  ssc_peak_lsb : 16;
  UINT16  ssc_step_size_lsb : 16;
  UINT16  ssc_peak_msb : 4;
  UINT16  ssc_step_size_msb : 5;
  UINT16  ssc_up_spread : 1;
  UINT16  ref_clk_mpllb_div : 3;
  UINT16  frac_en : 1;
  UINT16  reserved_15_14 : 2;
  UINT16  frac_den : 16;
  UINT16  frac_quot : 16;
  UINT16  frac_rem : 16;
  UINT16  hdmi_div : 3;
  UINT16  hdmi_pixel_clk_div : 2;
  UINT16  reserved_15_5 : 11;
  UINT16  Spare;
}MPLLB_CONTEXT;

typedef struct _CMN_CONTEXT_ {
  UINT16  sup_misc : 8;
  UINT16  tx_vboost_lvl : 3;
  UINT16  reserved_15_11 : 5;
  UINT16  rx_vref_ctrl : 5;
  UINT16  txdn_term_offset : 9;
  UINT16  reserved_15_14 : 2;
  UINT16  rx_term_offset : 5;
  UINT16  txup_term_offset : 9;
  UINT16  reserved_15_14_2 : 2;
  UINT16  unique_id : 10;
  UINT16  reserved_15_10 : 6;
}CMN_CONTEXT;

typedef struct _TX_CONTEXT_ {
  UINT16  rate : 3;
  UINT16  width : 3;
  UINT16  align_wide_xfer_en : 1;
  UINT16  mpllb_sel : 1;
  UINT16  vreg_tx_bypass : 1;
  UINT16  vboost_en : 1;
  UINT16  iboost_lvl : 4;
  UINT16  drv_en_kr : 1;
  UINT16  offcan_cont : 1;
  UINT16  misc : 8;
  UINT16  dcc_ctrl_range : 4;
  UINT16  dcc_bypass : 1;
  UINT16  term_ctrl : 3;
  UINT16  tx_unique_id : 8;
  UINT16  reserved_15_8 : 8;
  UINT16  Spare;
}TX_CONTEXT;

typedef struct _RX_CONTEXT_ {
  UINT16  eq_att_lvl : 3;
  UINT16  eq_vga_gain : 4;
  UINT16  eq_ctle_ofst_sel : 2;
  UINT16  eq_ctle_boost : 5;
  UINT16  eq_ctle_pole : 2;
  UINT16  eq_ctle_zero : 3;
  UINT16  eq_afe_rate : 3;
  UINT16  eq_afe_bias_tia : 5;
  UINT16  eq_afe_bias : 2;
  UINT16  eq_afe_vcm_adj : 3;
  UINT16  eq_dfe_tap1 : 8;
  UINT16  reserved_15_8 : 8;
  UINT16  dfe_bypass : 1;
  UINT16  adapt_sel : 1;
  UINT16  adapt_mode : 2;
  UINT16  misc : 8;
  UINT16  delta_iq : 4;
  UINT16  cdr_vco_config : 12;
  UINT16  dcc_ctrl_range : 4;
  UINT16  rate : 3;
  UINT16  ref_ld_val : 7;
  UINT16  div16p5_clk_en : 1;
  UINT16  cdr_ppm_max : 5;
  UINT16  width : 3;
  UINT16  vco_ld_val : 13;
  UINT16  sigdet_lf_threshold : 3;
  UINT16  sigdet_hf_threshold : 3;
  UINT16  sigdet_lfps_filter_en : 1;
  UINT16  term_ctrl : 3;
  UINT16  dcc_bypass : 1;
  UINT16  vreg_clk_bypass : 1;
  UINT16  adapt_cont : 1;
  UINT16  offcan_cont : 1;
  UINT16  reserved_15_14 : 2;
  UINT16  unique_id : 10;
  UINT16  reserved_15_10 : 6;
  UINT16  Spare[3];
}RX_CONTEXT;

typedef struct _C20_N6_PHY_TUNING_SET_ {
  UINT64              phy_list;               // list of PHYs to apply

  UINT64              mplla_context_list;     // list of MPLLA context to apply
  MPLLA_CONTEXT       mplla_context_var;      // context var field structure for MPLLA
  MPLLA_CONTEXT       mplla_context_var_mask; // context var_mask field structure for MPLLA

  UINT64              mpllb_context_list;     // list of MPLLB context to apply
  MPLLB_CONTEXT       mpllb_context_var;      // context var field structure for MPLLB
  MPLLB_CONTEXT       mpllb_context_var_mask; // context var_mask field structure for MPLLB

  UINT64              cmn_context_list;       // list of CMN context to apply
  CMN_CONTEXT         cmn_context_var;        // context var field structure for CMN
  CMN_CONTEXT         cmn_context_var_mask;   // context var_mask field structure for CMN

  UINT64              tx_context_list;        // list of TX context to apply
  TX_CONTEXT          tx_context_var;         // context var field structure for TX
  TX_CONTEXT          tx_context_var_mask;    // context var_mask field structure for TX

  UINT64              rx_context_list;        // list of RX context to apply
  RX_CONTEXT          rx_context_var;         // context var field structure for RX
  RX_CONTEXT          rx_context_var_mask;    // context var_mask field structure for RX
}C20_N6_PHY_TUNING_SET;

#pragma pack ()

