/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _AMD_CBS_FORM_ID_H_
#define _AMD_CBS_FORM_ID_H_

  #define CBS_CONFIGURATION_VARSTORE_ID                                           0x5000
// Section ID
  #define SETUP_CBS_ROOT_LABLE                                                    0x7000
  #define SETUP_CPU_COMMON_OPTIONS_LABLE                                          0x7001
  #define SETUP_DF_COMMON_OPTIONS_LABLE                                           0x7002
  #define SETUP_UMC_COMMON_OPTIONS_LABLE                                          0x7003
  #define SETUP_NBIO_COMMON_OPTIONS_LABLE                                         0x7004
  #define SETUP_FCH_COMMON_OPTIONS_LABLE                                          0x7005
  #define SETUP_SMU_COMMON_OPTIONS_LABLE                                          0x7006
  #define SETUP_SOC_MISCELLANEOUS_CONTROL_LABLE                                   0x7007
  #define SETUP_PROM21_CHIPSET_COMMON_OPTIONS_LABLE                               0x7008
  #define SETUP_PERFORMANCE_LABLE                                                 0x7009
  #define SETUP_PREFETCHER_SETTINGS_LABLE                                         0x700A
  #define SETUP_CORE_WATCHDOG_LABLE                                               0x700B
  #define SETUP_CCD_CORE_THREAD_ENABLEMENT_LABLE                                  0x700C
  #define SETUP_CUSTOM_CORE_PSTATES_LABLE                                         0x7010
  #define SETUP_PSTATE_LEGAL_DISCLAIMER_DECLINE_LABLE                             0x7011
  #define SETUP_PSTATE_LEGAL_DISCLAIMER_ACCEPT_LABLE                              0x7012
  #define SETUP_MEMORY_ADDRESSING_LABLE                                           0x7016
  #define SETUP_DDR_OPTIONS_LABLE                                                 0x7017
  #define SETUP_LPDDR_OPTIONS_LABLE                                               0x7018
  #define SETUP_DDR_TIMING_CONFIGURATION_LABLE                                    0x7019
  #define SETUP_DDR_BUS_CONFIGURATION_LABLE                                       0x701A
  #define SETUP_DDR_CONTROLLER_CONFIGURATION_LABLE                                0x701B
  #define SETUP_DDR_RAS_LABLE                                                     0x701C
  #define SETUP_DDR_SECURITY_LABLE                                                0x701D
  #define SETUP_DDR_ADDRESSING_OPTIONS_LABLE                                      0x701E
  #define SETUP_DDR_TRAINING_OPTIONS_LABLE                                        0x701F
  #define SETUP_DDR_MEMORY_MBIST_LABLE                                            0x7020
  #define SETUP_DDR_DTC_LEGAL_DISCLAIMER_DECLINE_LABLE                            0x7021
  #define SETUP_DDR_DTC_LEGAL_DISCLAIMER_ACCEPT_LABLE                             0x7022
  #define SETUP_DDR_SPD_TIMING_LABLE                                              0x7023
  #define SETUP_DDR_NON_SPD_TIMING_LABLE                                          0x7024
  #define SETUP_DDR_POWER_OPTIONS_LABLE                                           0x7025
  #define SETUP_DDR_ECC_CONFIGURATION_LABLE                                       0x7026
  #define SETUP_DRAM_SCRUBBERS_LABLE                                              0x7027
  #define SETUP_DDR_DATA_EYE_LABLE                                                0x702B
  #define SETUP_LPDDR_TIMING_CONFIGURATION_LABLE                                  0x702C
  #define SETUP_LPDDR_CONTROLLER_CONFIGURATION_LABLE                              0x702D
  #define SETUP_LPDDR_BUS_CONFIGURATION_LABLE                                     0x702E
  #define SETUP_LPDDR_RAS_LABLE                                                   0x702F
  #define SETUP_LPDDR_SECURITY_LABLE                                              0x7030
  #define SETUP_LPDDR_ADDRESSING_OPTIONS_LABLE                                    0x7031
  #define SETUP_LPDDR_TRAINING_OPTIONS_LABLE                                      0x7032
  #define SETUP_LPDDR_MEMORY_MBIST_LABLE                                          0x7033
  #define SETUP_LPDDR_DTC_LEGAL_DISCLAIMER_DECLINE_LABLE                          0x7034
  #define SETUP_LPDDR_DTC_LEGAL_DISCLAIMER_ACCEPT_LABLE                           0x7035
  #define SETUP_LPDDR_SPD_TIMING_LABLE                                            0x7036
  #define SETUP_LPDDR_NON_SPD_TIMING_LABLE                                        0x7037
  #define SETUP_LPDDR_POWER_OPTIONS_LABLE                                         0x7038
  #define SETUP_LPDDR_DATA_EYE_LABLE                                              0x703B
  #define SETUP_NBIO_RAS_COMMON_OPTIONS_LABLE                                     0x703E
  #define SETUP_GFX_CONFIGURATION_LABLE                                           0x7040
  #define SETUP_AUDIO_CONFIGURATION_LABLE                                         0x7041
  #define SETUP_PORT_CAPABILITY_ALL_PORTS_LABLE                                   0x7042
  #define SETUP_POWER_MANAGEMENT_FEATURES_ALL_PORTS_LABLE                         0x7043
  #define SETUP_PCIE_CORE_LABLE                                                   0x7044
  #define SETUP_PMM_GENERAL_LABLE                                                 0x7045
  #define SETUP_I3C_I2C_CONFIGURATION_OPTIONS_LABLE                               0x7047
  #define SETUP_SATA_CONFIGURATION_OPTIONS_LABLE                                  0x7048
  #define SETUP_USB_CONFIGURATION_OPTIONS_LABLE                                   0x7049
  #define SETUP_AC_POWER_LOSS_OPTIONS_LABLE                                       0x704A
  #define SETUP_UART_CONFIGURATION_OPTIONS_LABLE                                  0x704B
  #define SETUP_ESPI_CONFIGURATION_OPTIONS_LABLE                                  0x704C
  #define SETUP_XGBE_CONFIGURATION_OPTIONS_LABLE                                  0x704D
  #define SETUP_USB4_CONFIGURATION_OPTIONS_LABLE                                  0x704E
  #define SETUP_SPI_CONFIGURATION_OPTIONS_LABLE                                   0x704F
  #define SETUP_FCH_RAS_OPTIONS_LABLE                                             0x7050
  #define SETUP_USB0_2_DOT_0_PORT_ENABLE_LABLE                                    0x7054
  #define SETUP_USB1_2_DOT_0_PORT_ENABLE_LABLE                                    0x7055
  #define SETUP_USB2_2_DOT_0_PORT_ENABLE_LABLE                                    0x7056
  #define SETUP_USB3_2_DOT_0_PORT_ENABLE_LABLE                                    0x7057
  #define SETUP_USB4_2_DOT_0_PORT_ENABLE_LABLE                                    0x7058
  #define SETUP_USB0_3_DOT_1_PORT_ENABLE_LABLE                                    0x7059
  #define SETUP_USB1_3_DOT_1_PORT_ENABLE_LABLE                                    0x705A
  #define SETUP_USB3_3_DOT_1_PORT_ENABLE_LABLE                                    0x705B
  #define SETUP_USB4_3_DOT_1_PORT_ENABLE_LABLE                                    0x705C
  #define SETUP_USB4_RT0_LABLE                                                    0x705E
  #define SETUP_USB4_RT1_LABLE                                                    0x705F
  #define SETUP_SMARTSHIFT_CONTROL_LABLE                                          0x7063
  #define SETUP_FIRMWARE_ANTI_ROLLBACK_FAR_LABLE                                  0x7064
  #define SETUP_PLUTON_HSP_OPTIONS_LABLE                                          0x7065
  #define SETUP_AIM_T_OPTIONS_LABLE                                               0x7066
  #define SETUP_INTRUSION_DETECTION_LABLE                                         0x7067
  #define SETUP_PROM21_CHIPSET_PCIE_PORT_CONFIGURATION_OPTIONS_LABLE              0x706F
  #define SETUP_PROM21_CHIPSET_SATA_CONFIGURATION_OPTIONS_LABLE                   0x7070
  #define SETUP_PROM21_CHIPSET_USB_CONFIGURATION_OPTIONS_LABLE                    0x7071
  #define SETUP_PROM21L_DOT_1_2_3_USB_PORT_CONFIGURATION_OPTIONS_LABLE            0x7072
  #define SETUP_PROM21L_DOT_4_USB_PORT_CONFIGURATION_OPTIONS_LABLE                0x7073
  #define SETUP_SECONDARY_PROM21_CHIPSET_PCIE_PORT_CONFIGURATION_OPTIONS_LABLE    0x7074
  #define SETUP_SECONDARY_PROM21_CHIPSET_SATA_CONFIGURATION_OPTIONS_LABLE         0x7075
  #define SETUP_SECONDARY_PROM21_CHIPSET_USB_CONFIGURATION_OPTIONS_LABLE          0x7076
  #define SETUP_SECONDARY_PROM21L_DOT_1_2_3_USB_PORT_CONFIGURATION_OPTIONS_LABLE  0x7077
  #define SETUP_SECONDARY_PROM21L_DOT_4_USB_PORT_CONFIGURATION_OPTIONS_LABLE      0x7078

// KEY function ID
  #define KEY_CBS_CMN_CPU_OPCACHE_GRAYOUT_FLAG                                    0x700D  //CbsCmnCpuOpcacheGrayoutFlag
  #define KEY_CBS_CMN_CPU_OPCACHE_CTRL                                            0x700E  //CbsCmnCpuOpcacheCtrl
  #define KEY_CBS_CMN_CPU_OC_MODE                                                 0x700F  //CbsCmnCpuOcMode
  #define KEY_CBS_CPU_PST_CUSTOM_P0                                               0x7013  //CbsCpuPstCustomP0
  #define KEY_CBS_CPU_PST0_VID                                                    0x7014  //CbsCpuPst0Vid
  #define KEY_CBS_DBG_CPU_GEN_CPU_WDT                                             0x7015  //CbsDbgCpuGenCpuWdt
  #define KEY_CBS_CMN_MEM_TSME_ENABLE_DDR                                         0x7028  //CbsCmnMemTsmeEnableDdr
  #define KEY_CBS_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR                          0x7029  //CbsCmnMemDataCtrlDataScrambleEnDdr
  #define KEY_CBS_CMN_MEM_SPD_READ_RETRY_COUNT_CRC                                0x702A  //CbsCmnMemSpdReadRetryCountCrc
  #define KEY_CBS_CMN_MEM_TSME_LPDDR                                              0x7039  //CbsCmnMemTsmeLpddr
  #define KEY_CBS_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR                             0x703A  //CbsCmnMemCtrllerDataScrambleLpddr
  #define KEY_CBS_CMN_GNB_NB_IOMMU                                                0x703C  //CbsCmnGnbNbIOMMU
  #define KEY_CBS_CMN_SCPC_ATTR_CNTL                                              0x703D  //CbsCmnScpcAttrCntl
  #define KEY_CBS_CMN_GNB_PCIE_PSPP_POLICY                                        0x703F  //CbsCmnGnbPciePsppPolicy
  #define KEY_CBS_CMN_GNB_GFX_NB_AZALIA                                           0x7046  //CbsCmnGnbGfxNbAzalia
  #define KEY_CBS_CMN_FCH_SATA_ENABLE                                             0x7051  //CbsCmnFchSataEnable
  #define KEY_CBS_CMN_FCH_SATA_AUTO_SHUT_DOWN                                     0x7052  //CbsCmnFchSataAutoShutDown
  #define KEY_CBS_CMN_FCH_SATA_CLASS                                              0x7053  //CbsCmnFchSataClass
  #define KEY_CBS_CMN_FCH_ESPI_CRC                                                0x705D  //CbsCmnFchEspiCrc
  #define KEY_CBS_CMN_GNB_SMU_SYSTEM_CONFIG                                       0x7060  //CbsCmnGnbSMUSystemConfig
  #define KEY_CBS_CMN_SOC_FAR_ENFORCED                                            0x7068  //CbsCmnSocFarEnforced
  #define KEY_CBS_CMN_SOC_SPL_FUSE                                                0x7069  //CbsCmnSocSplFuse
  #define KEY_CBS_CMN_SOC_SPL_VALUE_IN_TBL                                        0x706A  //CbsCmnSocSplValueInTbl
  #define KEY_CBS_CMN_SOC_INIT_SPL_FOR_FAR                                        0x706B  //CbsCmnSocInitSplForFar
  #define KEY_CBS_CMN_SOC_FAR_SWITCH                                              0x706C  //CbsCmnSocFarSwitch
  #define KEY_CBS_CMN_SO_C_MISC_MPM_SUPPORT                                       0x706D  //CbsCmnSoCMiscMpmSupport
  #define KEY_CBS_CMN_SOC_MISC_MPM_ATTR_CNTL                                      0x706E  //CbsCmnSocMiscMpmAttrCntl

// Label CBS ID
  #define LABEL_CBS_CBS_CMN_GNB_SMU_SYSTEM_CONFIG_START                           0x7061  //CbsCmnGnbSMUSystemConfig Start Label
  #define LABEL_CBS_CBS_CMN_GNB_SMU_SYSTEM_CONFIG_END                             0x7062  //CbsCmnGnbSMUSystemConfig End Label

#endif // _AMD_CBS_FORM_ID_H_
