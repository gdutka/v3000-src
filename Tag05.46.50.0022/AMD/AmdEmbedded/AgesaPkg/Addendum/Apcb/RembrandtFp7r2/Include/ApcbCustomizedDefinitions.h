/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

//
// OEM CUSTOMIZABLE DEFINITIONS. Any changes must be done here...
//

#ifndef _APCB_CUSTOMIZED_DEFINITIONS_H_
#define _APCB_CUSTOMIZED_DEFINITIONS_H_

// =================================================================================================
// APCB Token Value Customization
// =================================================================================================

//Alias: BLDCFG_TIMING_MODE_SELECT
#define APCB_TOKEN_UID_USERTIMINGMODE_VALUE                                    TIMING_MODE_AUTO

//Alias: BLDCFG_MEMORY_CLOCK_SELECT
#define APCB_TOKEN_UID_MEMCLOCKVALUE_VALUE                                     DDR3200_FREQUENCY

//Alias: BLDCFG_MEMORY_ENABLE_CHIPSELECT_INTERLEAVING
#define APCB_TOKEN_UID_ENABLECHIPSELECTINTLV_VALUE                             TRUE

//Alias: BLDCFG_ENABLE_ECC_FEATURE
#define APCB_TOKEN_UID_ENABLEECCFEATURE_VALUE                                  TRUE

//Alias: BLDCFG_MEMORY_POWER_DOWN
#define APCB_TOKEN_UID_ENABLEPOWERDOWN_VALUE                                   TRUE

//Alias: BLDCFG_PARITY_CONTROL
#define APCB_TOKEN_UID_ENABLEPARITY_VALUE                                      FALSE

//Alias: BLDCFG_BANK_SWIZZLE
#define APCB_TOKEN_UID_ENABLEBANKSWIZZLE_VALUE                                 TRUE

//Alias: BLDCFG_MEMORY_RESTORE_CONTROL
#define APCB_TOKEN_UID_MEMRESTORECTL_VALUE                                     FALSE

//Alias: BLDCFG_PMU_TRAINING_MODE
#define APCB_TOKEN_UID_PMUTRAINMODE_VALUE                                      PMU_TRAIN_1D_2D

//Alias: BLDCFG_ECC_SYMBOL_SIZE
#define APCB_TOKEN_UID_ECCSYMBOLSIZE_VALUE                                     SYMBOL_SIZE_X16

//Alias: BLDCFG_MEMORY_ALL_CLOCKS_ON
#define APCB_TOKEN_UID_MEMORYALLCLOCKSON_VALUE                                 TRUE

//Alias: BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT
#define APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT_VALUE                           DDR4400_FREQUENCY

//Alias: BLDCFG_POWER_DOWN_MODE
#define APCB_TOKEN_UID_POWERDOWNMODE_VALUE                                     POWER_DOWN_BY_CHANNEL

//Alias: BLDCFG_IGNORE_SPD_CHECKSUM
#define APCB_TOKEN_UID_IGNORESPDCHECKSUM_VALUE                                 TRUE

//Alias: BLDCFG_BANK_SWAP_GROUP_ENABLE
#define APCB_TOKEN_UID_ENABLEBANKGROUPSWAP_VALUE                               2 // BANK_GROUP_SWAP_APU

//Alias: BLDCFG_ODT_CMD_THROT_ENABLE
#define APCB_TOKEN_UID_ODTSCMDTHROTEN_VALUE                                    TRUE

//Alias: BLDCFG_SW_CMD_THROT_ENABLE
#define APCB_TOKEN_UID_SWCMDTHROTEN_VALUE                                      0

//Alias: BLDCFG_FORCE_POWER_DOWN_THROT_ENABLE
#define APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN_VALUE                               0

//Alias: BLDCFG_ODT_CMD_THROT_CYCLE
#define APCB_TOKEN_UID_ODTSCMDTHROTCYC_VALUE                                   0

//Alias: BLDCFG_SW_CMD_THROT_CYCLE
#define APCB_TOKEN_UID_SWCMDTHROTCYC_VALUE                                     0

//Alias: BLDCFG_DIMM_SENSOR_CONFIGURATION
#define APCB_TOKEN_UID_DIMMSENSORCONF_VALUE                                    0

//Alias: BLDCFG_DIMM_SENSOR_UPPER
#define APCB_TOKEN_UID_DIMMSENSORUPPER_VALUE                                   0

//Alias: BLDCFG_DIMM_SENSOR_LOWER
#define APCB_TOKEN_UID_DIMMSENSORLOWER_VALUE                                   0

//Alias: BLDCFG_DIMM_SENSOR_CRITICAL
#define APCB_TOKEN_UID_DIMMSENSORCRITICAL_VALUE                                0

//Alias: BLDCFG_DIMM_SENSOR_RESOLUTION
#define APCB_TOKEN_UID_DIMMSENSORRESOLUTION_VALUE                              0

//Alias: BLDCFG_AUTO_REF_FINE_GRAN_MODE
#define APCB_TOKEN_UID_AUTOREFFINEGRANMODE_VALUE                               0

#define APCB_TOKEN_UID_AUTOREFMODE_VALUE                               0

//Alias: BLDCFG_ENABLE_MEMPSTATE
#define APCB_TOKEN_UID_ENABLEMEMPSTATE_VALUE                                   TRUE

//Alias: BLDCFG_ENABLE_DDRROUTEBALANCEDTEE
#define APCB_TOKEN_UID_DDRROUTEBALANCEDTEE_VALUE                               FALSE

//Alias: BLDCFG_MEM_NVDIMM_POWER_SOURCE
#define APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE_VALUE                           1

//Alias: BLDCFG_MEM_DATA_POISON
#define APCB_TOKEN_UID_MEM_DATA_POISON_VALUE                                   TRUE

//Alias: BLDCFG_MEM_DATA_SCRAMBLE
#define APCB_TOKEN_UID_MEM_DATA_SCRAMBLE_VALUE                                 TRUE

//
// MBIST Items
// MBIST SubTestType: 0-BInterface Mode, 1 - Data Eye
//
//Alias: BLDCFG_MEM_MBIST_TEST_ENABLE
#define APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE_VALUE                             FALSE

//Alias: BLDCFG_MEM_MBIST_SUBTEST_TYPE
#define APCB_TOKEN_UID_MEM_MBIST_TESTMODE_VALUE                                 0

//Alias: BLDCFG_MEM_MBIST_AGGRESOR_ON
#define APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON_VALUE                             FALSE

//Alias: BLDCFG_MEM_MBIST_HALT_ON_ERROR
#define APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR_VALUE                           TRUE

//Alias: BLDCFG_MEM_MBIST_CPU_VREF_RANGE
#define APCB_TOKEN_UID_MEM_CPU_VREF_RANGE_VALUE                                0

//Alias: BLDCFG_MEM_MBIST_DRAM_VREF_RANGE
#define APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE_VALUE                               0

//Alias: BLDCFG_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
#define APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_VALUE                0

// Read Data Eye APCB customization options.
//Alias: BLDCFG_MEM_MBIST_AGGRESSORS
#define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_VALUE                         0  // Read Aggressor Channels
                                                                                  // 0 - Disabled
                                                                                  // 1 - 1 Aggressor Channels
                                                                                  // 3 - 3 Aggressor Channels
                                                                                  // 7 - 7 Aggressor Channel

//Alias: BLDCFG_MEM_MBIST_PATTERN_LENGTH
#define APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_VALUE                          3  // 3-12

//Alias: BLDCFG_MEM_MBIST_PATTERN_SELECT
#define APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_VALUE                          0  // 0- PRBS, 1- SSO, 2- Both

//Alias: BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_CTRL
#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL_VALUE                   0

//Alias: BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32
//#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32_VALUE                0  // 32 bit DQ Lane mask Uppoer 32

//Alias: BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32
//#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32_VALUE                0  // 32 bit DQ Lane mask Lower 32

//Alias: BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC
#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC_VALUE                0  // 8 bit ECC Lane mask

//Alias: BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_VAL
#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL_VALUE                    0  // Pattern 0 or 1

//Alias: BLDCFG_MEM_MBIST_TGT_STATIC_LANE_CTRL
#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL_VALUE                    0

//Alias: BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_U32
//#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_U32_VALUE                 0  // 32 bit DQ Lane mask Upper 32

//Alias: BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_L32
//#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_L32_VALUE                 0  // 32 bit DQ Lane mask Lower 32

//Alias: BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC
#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC_VALUE                 0  // Target ECC Lane Mask - 8 bit

//Alias: BLDCFG_MEM_MBIST_TGT_STATIC_LANE_VAL
#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL_VALUE                     0  // Pattern 0 or 1

//Alias: BLDCFG_MEM_MBIST_DATA_EYE_TYPE
#define APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE_VALUE                           1  //  Data Eye type
                                                                                  // 0 - 1D Voltage Sweep
                                                                                  // 1 - 1D Timing Sweep
                                                                                  // 2 - 2D Full Data Eye
                                                                                  // 3 - Worst Case Margin Only

//Alias: BLDCFG_MEM_MBIST_WORST_CASE_GRAN
#define APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN_VALUE                         0  // Worst Case Granularity.
                                                                                  // 0 - Per Chipselect
                                                                                  // 1 - Per Nibble

//Alias: BLDCFG_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP
#define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALUE              1  // Voltage Step, Options 1, 2 and 4

//Alias: BLDCFG_MEM_MBIST_READ_DATA_EYE_TIMING_STEP
#define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALUE               1  // Timing Step, Options 1, 2 and 4

//Alias: BLDCFG_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP
#define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALUE             1  // Voltage Step, Options 1, 2 and 4

//Alias: BLDCFG_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP
#define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALUE              1  // Timing Step, Options 1, 2 and 4

//Alias: BLDCFG_MEM_TSME_ENABLE
#define APCB_TOKEN_UID_MEM_TSME_ENABLE_VALUE                                   FALSE


//
// CCX specific settings
//
//Alias: BLDCFG_CCX_MIN_SEV_ASID
#define APCB_TOKEN_UID_CCX_MIN_SEV_ASID_VALUE                                  1

//Alias: BLDCFG_CCX_SEV_ASID_COUNT
#define APCB_TOKEN_UID_CCX_SEV_ASID_COUNT_VALUE                                0x1FD

//
// PSP Debug Mode settings
//
//Alias: BLDCFG_PSP_ENABLE_DEBUG_MODE
#define APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE_VALUE                             1

//Alias: BLDCFG_PSP_SYSHUB_WDT_INTERVAL
#define APCB_TOKEN_UID_PSP_SYSHUB_WDT_INTERVAL_VALUE                           2600

//
// DF specific settings
//
//Alias: BLDCFG_DF_GMI_ENCRYPT
#define APCB_TOKEN_UID_DF_GMI_ENCRYPT_VALUE                                    ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_XGMI_ENCRYPT
#define APCB_TOKEN_UID_DF_XGMI_ENCRYPT_VALUE                                   ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_SAVE_RESTORE_MEM_ENCRYPT
#define APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT_VALUE                       ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_SYS_STORAGE_AT_TOM
#define APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM_VALUE                      ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_PROBE_FILTER_ENABLE
#define APCB_TOKEN_UID_DF_PROBE_FILTER_VALUE                                   ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_BOTTOM_IO
#define APCB_TOKEN_UID_DF_BOTTOMIO_VALUE                                       0xC0

//Alias: BLDCFG_DF_MEM_INTERLEAVING
#define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_VALUE                               DF_MEM_INTLV_DIE

//Alias: BLDCFG_DF_DRAM_INTLV_SIZE
#define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE_VALUE                          DF_MEM_INTLV_SIZE_AUTO

//Alias: BLDCFG_DF_CHAN_INTLV_HASH_EN
#define APCB_TOKEN_UID_DF_CHAN_INTLV_HASH_VALUE                                ENABLE_DISABLE_AUTO_OPTION_DISABLED

//Alias: BLDCFG_DF_PCI_MMIO_SIZE
#define APCB_TOKEN_UID_DF_PCI_MMIO_SIZE_VALUE                                  0x8000000

//Alias: BLDCFG_DF_CAKE_CRC_THRESH_PERF_BOUNDS
#define APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS_VALUE                    100  // 0.001%

//Alias: BLDCFG_DF_MEM_CLEAR
#define APCB_TOKEN_UID_DF_MEM_CLEAR_VALUE                                      ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF3_XGMI2_LINK_CFG
#define APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG_VALUE                                ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_DRAM_NPS
#define APCB_TOKEN_UID_DF_DRAM_NPS_VALUE                                       DF_DRAM_NPS_AUTO

//Alias: BLDCFG_DF_INVERT_DRAM_MAP
#define APCB_TOKEN_UID_DF_INVERT_DRAM_MAP_VALUE                                ENABLE_DISABLE_AUTO_OPTION_AUTO

//Alias: BLDCFG_DF_4LINK_MAX_XGMI_SPEED
#define APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED_VALUE                           DF_XGMI_LINK_SPEED_AUTO

//Alias: BLDCFG_DF_3LINK_MAX_XGMI_SPEED
#define APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED_VALUE                           DF_XGMI_LINK_SPEED_AUTO

//Alias: BLDCFG_DF_REMAP_AT_1TB
#define APCB_TOKEN_UID_DF_REMAP_AT_1TB_VALUE                                   DF_REMAP_AT_1TB_AUTO

//
// GNB Specific Configurations
//

#define BLDCFG_EARLY_PCIE_TRAINING      FALSE

#if BLDCFG_EARLY_PCIE_TRAINING == TRUE
  //Alias: BLDCFG_BMC_SOCKET_INFO
  #define APCB_TOKEN_UID_BMC_SOCKET_NUMBER_VALUE                                 0x00
  //Alias: BLDCFG_BMC_START_LANE
  #define APCB_TOKEN_UID_BMC_START_LANE_VALUE                                    0xFF
  //Alias: BLDCFG_BMC_END_LANE
  #define APCB_TOKEN_UID_BMC_END_LANE_VALUE                                      0xFF
  //Alias: BLDCFG_SECOND_PCIE_LINK_START_LANE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE_VALUE                       12
  //Alias: BLDCFG_SECOND_PCIE_LINK_END_LANE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE_VALUE                         12
#else
  //Alias: BLDCFG_BMC_SOCKET_INFO
  #define APCB_TOKEN_UID_BMC_SOCKET_NUMBER_VALUE                                 0x0F
  //Alias: BLDCFG_BMC_START_LANE
  #define APCB_TOKEN_UID_BMC_START_LANE_VALUE                                    0xFF
  //Alias: BLDCFG_BMC_END_LANE
  #define APCB_TOKEN_UID_BMC_END_LANE_VALUE                                      0xFF
  //Alias: BLDCFG_SECOND_PCIE_LINK_START_LANE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE_VALUE                       0xFF
  //Alias: BLDCFG_SECOND_PCIE_LINK_END_LANE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE_VALUE                         0xFF
#endif

//Alias: BLDCFG_BMC_BMC_DEVICE
#define APCB_TOKEN_UID_BMC_DEVICE_VALUE                                        0

//Alias: BLDCFG_BMC_FUNCTION
#define APCB_TOKEN_UID_BMC_FUNCTION_VALUE                                      0


// Enables control of PCIe reset as GPIO26
// If PCIe reset (GPIO26) is used for PCIe slot reset, this should be set to TRUE
//Alias: BLDCFG_PCIE_RESET_CONTROL
#define APCB_TOKEN_UID_PCIE_RESET_CONTROL_VALUE                                TRUE

//
// UART Enable Disable through SOC UART/ LPC-SUPER IO
// 0 - LPC UART, 1 - SOC UART0
//
//Alias: BLDCFG_FCH_CONSOLE_OUT_ENABLE
#define APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE_VALUE                            0

//Alias: BLDCFG_FCH_CONSOLE_OUT_SERIAL_PORT
#define APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_VALUE                       0

//Alias: BLDCFG_FCH_CONSOLE_OUT_BASIC_ENABLE
#define APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE_VALUE                      0

//#define BLDCFG_CONOUTCTRL_BREAKPOINT          FALSE
//#define BLDCFG_CONOUTCTRL_BREAKPOINT_ALLDIE   FALSE

//Alias: APCB_TOKEN_CONFIG_MEM_RESERVE_ADDR_LOW
  #define APCB_TOKEN_UID_MEM_RESERVE_ADDR_LOW_VALUE                            0x0

//Alias: APCB_TOKEN_CONFIG_MEM_RESERVE_ADDR_HIGH
  #define APCB_TOKEN_UID_MEM_RESERVE_ADDR_HIGH_VALUE                           0x1

//Alias: APCB_TOKEN_CONFIG_MEM_RESERVE_SIZE_LOW
  #define APCB_TOKEN_UID_MEM_RESERVE_SIZE_LOW_VALUE                            0x100000

//Alias: APCB_TOKEN_CONFIG_MEM_RESERVE_SIZE_HIGH
  #define APCB_TOKEN_UID_MEM_RESERVE_SIZE_HIGH_VALUE                           0x00

// =================================================================================================
// APCB Type Data Structure Member Customization
// =================================================================================================

//
// MEM Platform Specific Overrides
//
#define NUMBER_OF_CHANNEL_PER_SOCKET  2
#define NUMBER_OF_DIMMS_PER_CHANNEL   2

#define BLDCFG_NUM_CHNL_SUPPORTED     NUMBER_OF_CHANNELS_SUPPORTED(ANY_SOCKET, NUMBER_OF_CHANNEL_PER_SOCKET),                     // NumberChannelsSupported
#define BLDCFG_NUM_DIMMS_SUPPORTED    NUMBER_OF_DIMMS_SUPPORTED(ANY_SOCKET, ANY_CHANNEL, NUMBER_OF_DIMMS_PER_CHANNEL),            // NumberDimmSupported
#define BLDCFG_MEM_CLK_MAP            MEMCLK_DIS_MAP(ANY_SOCKET, ANY_CHANNEL, 0x00, 0x01, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00),    // MemClkDisMap
#define BLDCFG_CKE_TRI_MAP            CKE_TRI_MAP   (ANY_SOCKET, ANY_CHANNEL, 0x00, 0x01, 0x02, 0x03),                            // CkeTriMap
#define BLDCFG_ODT_TRI_MAP            ODT_TRI_MAP   (ANY_SOCKET, ANY_CHANNEL, 0x00, 0x01, 0x02, 0x03),                            // OdtTriMap
#define BLDCFG_CS_TRI_MAP             CS_TRI_MAP    (ANY_SOCKET, ANY_CHANNEL, 0x00, 0x01, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00),    // CsTriMap
// #define BLDCFG_NUM_SOLDERED_DOWN_DIMM_SUPPORTED      NUMBER_OF_SOLDERED_DOWN_DIMMS_SUPPORTED (ANY_SOCKET, ANY_CHANNEL, 0x00),  // NumberSolderedDownDimmsSupported
// #define BLDCFG_NUM_CS_SUPPORTED       NUMBER_OF_CHIP_SELECTS_SUPPORTED (ANY_SOCKET, ANY_CHANNEL, 0x00),                        // NumberChipSelectsSupported
// #define BLDCFG_BUS_SPEED_OVERRIDE     OVERRIDE_DDR_BUS_SPEED (ANY_SOCKET, ANY_CHANNEL, 0x00, 0x00),                            // OverrideBusSpeed
// #define BLDCFG_DRAM_TECH_OVERRIDE     DRAM_TECHNOLOGY (ANY_SOCKET, 0x00),                                                      // DramTechnology
// #define BLDCFG_SOLDRD_DOWN_DIMM_TYPE  SOLDERED_DOWN_SODIMM_TYPE (ANY_SOCKET, ANY_CHANNEL),                                     // SolderedDownSoDimmType
// #define BLDCFG_MEM_POWER_POLICY_OVERRIDE  MEMORY_POWER_POLICY_OVERRIDE (0x00),                                                 // MemoryPowerPolicyOverride
#define BLDCFG_NUM_MOTHERBOARD_LAYERS MOTHER_BOARD_LAYERS(LAYERS_4),                                                           // MotherboardLayers

//
// Please Review the below SMBUS Address, based off your schematics and comment out lines which doesn't belong
// to the project for which this file is being customized
//

#define BLDCFG_SPD_P0_CH_A_DIMM0_ADDRESS             0xA0
#define BLDCFG_SPD_P0_CH_A_DIMM1_ADDRESS             0xA4
#define BLDCFG_SPD_P0_CH_B_DIMM0_ADDRESS             0xA2
#define BLDCFG_SPD_P0_CH_B_DIMM1_ADDRESS             0xA6

#define BLDCFG_I2C_MUX_P0_ADRRESS                   0x94
#define BLDCFG_I2C_MUX_P0_COMMAND_CODE              0x03

#define BLDCFG_I2C_MUX_P0_CH_A_DIMM0_DATA_BYTE      0x80
#define BLDCFG_I2C_MUX_P0_CH_A_DIMM1_DATA_BYTE      0x80
#define BLDCFG_I2C_MUX_P0_CH_B_DIMM0_DATA_BYTE      0x80
#define BLDCFG_I2C_MUX_P0_CH_B_DIMM1_DATA_BYTE      0x80

//
// DramDown SPD configurations
//

#define BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_VALID     FALSE
#define BLDCFG_DRAMDOWN_CH_A_DIMM_0_DIMM_PRESENT  FALSE
#define BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_DATA  \
      0x22, 0x00, 0x0C, 0x01, 0x84, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x03, 0x00, 0x00,\
      0x00, 0x00, 0x0A, 0x0C, 0x34, 0x00, 0x00, 0x00, 0x78, 0x78, 0x78, 0x11, 0x18, 0x90, 0x20, 0x08,\
      0x00, 0x05, 0x70, 0x03, 0x00, 0xC8, 0x28, 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF4, 0x6F,\
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA7, 0x2D



#endif   //ifndef  _APCB_CUSTOMIZED_DEFINITIONS_H_
