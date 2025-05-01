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
  APCB_TYPE_TOKEN_1BYTE,                      // TypeId
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


APCB_TOKEN_PAIR_UINT8 TokeList[] = {
// Two macros can be used for token definitions
//  APCB_TOKEN_U8 (APCB_TOKEN_UID_NAME, APCB_TOKEN_UID_NAME_VALUE),
//  APCB_TOKEN_VAL_U8(APCB_TOKEN_UID_NAME), // Expand to use APCB_TOKEN_UID_NAME_VALUE

  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE),                              //< ENABLE_DEBUG_MODE
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_GMI_ENCRYPT),                                     //< GMI encryption
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_XGMI_ENCRYPT),                                    //< xGMI encryption
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT),                        //< Save/Restore memory encryption
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_PROBE_FILTER),                                    //< SPF
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_BOTTOMIO),                                        //< Bottom IO
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_MEM_INTERLEAVING),                                //< Channel, Die, Socket interleaving control
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE),                           //< Address bit to interleave on
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_MEM_CLEAR),                                       //< Enable/Disable MemClear
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG),                                 //< Specify the number of xGMI links connected to the other socket
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_DRAM_NPS),                                        //< Nodes per socket
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_INVERT_DRAM_MAP),                                 //< Low addresses go to lowest numbered memory channels or highest numbered memory channels
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED),                            //< 4-Link max xGMI speed
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED),                            //< 3-Link max xGMI speed
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_REMAP_AT_1TB),                                    //< Attempt to remap the memory just below the 1TB boundary
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_UMAMODE),                                            //< UmaMode - Uma Mode
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PMUTRAINMODE),                                       //< PmuTrainMode - PMU Training Mode
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_ODTSCMDTHROTCYC),                                    //< UMC::CH::ThrottleCtrl[OdtsCmdThrotCyc]
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_SWCMDTHROTCYC),                                      //< UMC::CH::ThrottleCtrl[SwCmdThrotCyc]
//  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DIMMSENSORRESOLUTION),                               //< DIMM temperature sensor register index BSU16 (8), Temperature resolution register
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_AUTOREFFINEGRANMODE),                                //< UMC::CH::SpazCtrl_AutoRefFineGranMode
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_AUTOREFMODE),                                        //< Auto refresh mode
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE),                              //< MBIST Test Enable/Disable
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_TESTMODE),                                 //< MBIST SubTest Type
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON),                              //< Aggressor Enable/Disable
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR),                            //< Halt on Error in case MBIST Test failed
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_CPU_VREF_RANGE),                                 //< CPU Vref margins in percent (%)
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE),                                //< DRAM Vref margins in percent (%)
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE),                            //< NVDIMM power source, 1: device, 2: host
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_DATA_POISON),                                    //< Data Poison enabling 1: Enabled, 0: Disabled
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_DATA_SCRAMBLE),                                  //< Data Scramble enabling 1: Enabled, 0: Disabled
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_PS_ERROR_HANDLING),                              //< Platform specific error handling
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT),                 //< MBIST per bit slave die report
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL),                          //< Enable Read Aggressor Channels
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH),                           //< Read Pattern Length
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT),                           //< Read Pattern Select
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC),                 //< Aggressor ECC Lanes Mask
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL),                     //< Aggressor DQ Lanes value, set as 0 or 1
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC),                  //< Target ECC Lanes Mask
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL),                      //< Target DQ Lanes value set as 0 or 1
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE),                            //< Read Data Type
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN),                          //< Read Data Eye worst case margins, per cs or per nibble
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP),               //< Read Data Eye Voltage Step i.e. 1,2 and 4
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP),                //< Read Data Eye Timing Step  i.e. 1, 2 and 4
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP),              //< Write Data Eye Voltage Step i.e. 1,2 and 4
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP),               //< Write Data Eye Timing Step i.e. 1,2 and 4
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_BMC_SOCKET_NUMBER),                                  //< BMC SOcket Number
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_BMC_START_LANE),                                     //< BMC Start Lane
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_BMC_END_LANE),                                       //< BMC End Lane
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE),                        //< 2nd link Start Lane
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE),                          //< 2nd link End Lane
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_BMC_DEVICE),                                         //< BMC Device Number
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_BMC_FUNCTION),                                       //< BMC Function Number
//  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_BMC_LINK_SPEED),                                     //< BMC Link Speed
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE),
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT),
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE),
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_CCX_SEV_ASID_COUNT),                                 //< SEV ASID COUNT
//  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PCIE_RESET_PIN_SELECT),                              //< PCIE Reset Pin Select
//  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_ABL_SERIAL_BAUD_RATE),                                                                //< ABL Serial Baud Rate
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_FCH_SMBUS_SPEED),                                    //< FCH Smbus speed
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL),                               //< Memory training MsgBlock->HdtCtrl
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_ENABLEBANKGROUPSWAP),                                //< Select bank group swapping type
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL),                               //< Memory training MsgBlock->HdtCtrl
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_RCD_PARITY),                                         //< CBS item: RCD Parity
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DRAM_ADDRESS_COMMAND_PARITY_RETRY),                  //< CBS item: DRAM Address Command Parity Retry
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MAX_PARITY_ERROR_REPLAY),                            //< CBS item: Max Parity Error Replay
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MEM_CTRLLER_WR_CRC),                                 //< CBS item: Write CRC Enable
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DRAM_WRITE_CRC_ENABLE_AND_RETRY_LIMIT),              //< CBS item: DRAM Write CRC Enable and Retry Limit
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MAX_WRITE_CRC_ERROR_REPLAY),                         //< CBS item: Max Write CRC Error Replay
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_UECC_RETRY_DDR4),                                    //< CBS item: DRAM UECC Retry
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG),                              //< Platform TPM selection
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX),                             //< Platform TPM crypto algorithms
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PSP_FTPM_NVSTORAGE_SIZE),                            //< Platform fTPM NV storage size
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_PSP_RPMC_ENABLE),                                    //< Platform RPMC usage
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_RPMC_RECOVERY_TYPE),                                 //< RPMC recovery type
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT),                  //< Platform Power Sensors slection
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_FCH_ACPI_SPREAD_SPECTRUM),                           //< FCH Acpi Spread Spectrum
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_I2C_0_RX_SEL),                                       //< FCH I2C0 RxSel
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_I2C_1_RX_SEL),                                       //< FCH I2C1 RxSel
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_I2C_2_RX_SEL),                                       //< FCH I2C2 RxSel
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_I2C_3_RX_SEL),                                       //< FCH I2C3 RxSel
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_I2C_4_RX_SEL),                                       //< FCH I2C4 RxSel
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_AC_GPIO_INDEX),                                  //< MPM M Event AC GPIO
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_GPIO_INDEX),                                //< MPM M Event WLAN GPIO
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_CONTROL),                           //< MPM M Event WLAN POWEREN CONTROL
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_OFFSET),                            //< MPM M Event WLAN POWEREN OFFSET
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_MASK),                              //< MPM M Event WLAN POWEREN MASK
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATA),                              //< MPM M Event WLAN POWEREN DATA
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIOPIN),                           //< MPM M Event WLAN POWEREN GPIOPIN
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXEN),                           //< MPM M Event WLAN POWEREN IOMUXEN
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXDATA),                         //< MPM M Event WLAN POWEREN IOMUXDATA
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_CPU_SYNC_FLOOD_ON_MCA),                              //< Sync Flood on MCA
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_EXT_IP_SYNC_FLOOD_PROP),                          //< Disable DF to external downstream IP SyncFloodPropagation
  APCB_TOKEN_VAL_U8 (APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP)                                  //< Disable DF sync flood propagation
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

