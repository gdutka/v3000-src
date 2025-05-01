/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

//
// OEM CUSTOMIZABLE DEFINITIONS. Any changes must be done here...
//

#ifndef _APCB_DEFAULTS_H_
#define _APCB_DEFAULTS_H_

#ifndef BLDCFG_FAMILY_ALL
  #define BLDCFG_FAMILY_ALL                         AMD_FAMILY_17_ALL
#endif
#ifndef BLDCFG_FAMILY_APCB
  #define BLDCFG_FAMILY_APCB                        AMD_F17_ALL_APCB
#endif

#ifndef BLDCFG_EXTERNAL_PARAMS_DEF_ENABLED
  #define BLDCFG_EXTERNAL_PARAMS_DEF_ENABLED        TRUE
#endif
#ifndef BLDCFG_EXTERNAL_PARAMS_ENABLED
  #define BLDCFG_EXTERNAL_PARAMS_ENABLED            FALSE
#endif
#ifndef BLDCFG_INTERNAL_PARAMS_DEF_ENABLED
  #define BLDCFG_INTERNAL_PARAMS_DEF_ENABLED        TRUE
#endif
#ifndef BLDCFG_INTERNAL_PARAMS_ENABLED
  #define BLDCFG_INTERNAL_PARAMS_ENABLED            FALSE
#endif


#ifndef APCB_TOKEN_UID_BOTTOMIO_VALUE
  #define APCB_TOKEN_UID_BOTTOMIO_VALUE                                        0xE0
#endif
#ifndef APCB_TOKEN_UID_MEMHOLEREMAPPING_VALUE
  #define APCB_TOKEN_UID_MEMHOLEREMAPPING_VALUE                                TRUE
#endif
#ifndef APCB_TOKEN_UID_LIMITMEMORYTOBELOW1TB_VALUE
  #define APCB_TOKEN_UID_LIMITMEMORYTOBELOW1TB_VALUE                           TRUE
#endif
#ifndef APCB_TOKEN_UID_USERTIMINGMODE_VALUE
  #define APCB_TOKEN_UID_USERTIMINGMODE_VALUE                                  TIMING_MODE_AUTO
#endif
#ifndef APCB_TOKEN_UID_MEMCLOCKVALUE_VALUE
  #define APCB_TOKEN_UID_MEMCLOCKVALUE_VALUE                                   DDR2400_FREQUENCY
#endif
#ifndef APCB_TOKEN_UID_ENABLECHIPSELECTINTLV_VALUE
  #define APCB_TOKEN_UID_ENABLECHIPSELECTINTLV_VALUE                           FALSE
#endif
#ifndef APCB_TOKEN_UID_ENABLECHANNELINTLV_VALUE
  #define APCB_TOKEN_UID_ENABLECHANNELINTLV_VALUE                              FALSE
#endif
#ifndef APCB_TOKEN_UID_ENABLEECCFEATURE_VALUE
  #define APCB_TOKEN_UID_ENABLEECCFEATURE_VALUE                                FALSE
#endif
#ifndef APCB_TOKEN_UID_ENABLEPOWERDOWN_VALUE
  #define APCB_TOKEN_UID_ENABLEPOWERDOWN_VALUE                                 TRUE
#endif
#ifndef APCB_TOKEN_UID_ENABLEPARITY_VALUE
  #define APCB_TOKEN_UID_ENABLEPARITY_VALUE                                    TRUE
#endif
#ifndef APCB_TOKEN_UID_ENABLEBANKSWIZZLE_VALUE
  #define APCB_TOKEN_UID_ENABLEBANKSWIZZLE_VALUE                               TRUE
#endif
#ifndef APCB_TOKEN_UID_ENABLEMEMCLR_VALUE
  #define APCB_TOKEN_UID_ENABLEMEMCLR_VALUE                                    FALSE
#endif
#ifndef APCB_TOKEN_UID_ENABLEMEMPSTATE_VALUE
  #define APCB_TOKEN_UID_ENABLEMEMPSTATE_VALUE                                 TRUE
#endif
#ifndef APCB_TOKEN_UID_ENABLEMEMPSTATE_DDR_VALUE
  #define APCB_TOKEN_UID_ENABLEMEMPSTATE_DDR_VALUE                             TRUE
#endif
#ifndef APCB_TOKEN_UID_UMAMODE_VALUE
  #define APCB_TOKEN_UID_UMAMODE_VALUE                                         UMA_AUTO
#endif
#ifndef APCB_TOKEN_UID_UMASIZE_VALUE
  #define APCB_TOKEN_UID_UMASIZE_VALUE                                         32
#endif
#ifndef APCB_TOKEN_UID_MEMRESTORECTL_VALUE
  #define APCB_TOKEN_UID_MEMRESTORECTL_VALUE                                   TRUE
#endif
#ifndef APCB_TOKEN_UID_MEM_RESTORE_RTC_DEFAULT_DATE_VALUE
  #define APCB_TOKEN_UID_MEM_RESTORE_RTC_DEFAULT_DATE_VALUE                    0x00200101
#endif
#ifndef APCB_TOKEN_UID_ISCAPSULEMODE_VALUE
  #define APCB_TOKEN_UID_ISCAPSULEMODE_VALUE                                   TRUE
#endif
#ifndef APCB_TOKEN_UID_AMPENABLE_VALUE
  #define APCB_TOKEN_UID_AMPENABLE_VALUE                                       TRUE
#endif
#ifndef APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATE_VALUE
  #define APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATE_VALUE                           FALSE
#endif
#ifndef APCB_TOKEN_UID_PMUTRAINMODE_VALUE
  #define APCB_TOKEN_UID_PMUTRAINMODE_VALUE                                    PMU_TRAIN_1D_2D
#endif
#ifndef APCB_TOKEN_UID_ECCREDIRECTION_VALUE
  #define APCB_TOKEN_UID_ECCREDIRECTION_VALUE                                  FALSE
#endif
#ifndef APCB_TOKEN_UID_SCRUBDRAMRATE_VALUE
  #define APCB_TOKEN_UID_SCRUBDRAMRATE_VALUE                                   0
#endif
#ifndef APCB_TOKEN_UID_SCRUBL2RATE_VALUE
  #define APCB_TOKEN_UID_SCRUBL2RATE_VALUE                                     0
#endif
#ifndef APCB_TOKEN_UID_SCRUBL3RATE_VALUE
  #define APCB_TOKEN_UID_SCRUBL3RATE_VALUE                                     0
#endif
#ifndef APCB_TOKEN_UID_SCRUBICRATE_VALUE
  #define APCB_TOKEN_UID_SCRUBICRATE_VALUE                                     0
#endif
#ifndef APCB_TOKEN_UID_SCRUBDCRATE_VALUE
  #define APCB_TOKEN_UID_SCRUBDCRATE_VALUE                                     0
#endif
#ifndef APCB_TOKEN_UID_ECCSYNCFLOOD_VALUE
  #define APCB_TOKEN_UID_ECCSYNCFLOOD_VALUE                                    0
#endif
#ifndef APCB_TOKEN_UID_ECCSYMBOLSIZE_VALUE
  #define APCB_TOKEN_UID_ECCSYMBOLSIZE_VALUE                                   SYMBOL_SIZE_X16
#endif
#ifndef APCB_TOKEN_UID_UECC_RETRY_DDR4_VALUE
  #define APCB_TOKEN_UID_UECC_RETRY_DDR4_VALUE                                 FALSE
#endif
#ifndef APCB_TOKEN_UID_DQSTRAININGCONTROL_VALUE
  #define APCB_TOKEN_UID_DQSTRAININGCONTROL_VALUE                              TRUE
#endif
#ifndef APCB_TOKEN_UID_UMAABOVE4G_VALUE
  #define APCB_TOKEN_UID_UMAABOVE4G_VALUE                                      TRUE
#endif
#ifndef APCB_TOKEN_UID_UMAALIGNMENT_VALUE
  #define APCB_TOKEN_UID_UMAALIGNMENT_VALUE                                    UMA_4MB_ALIGNED
#endif
#ifndef APCB_TOKEN_UID_MEMORYALLCLOCKSON_VALUE
  #define APCB_TOKEN_UID_MEMORYALLCLOCKSON_VALUE                               TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT_VALUE
  #define APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT_VALUE                         DDR2400_FREQUENCY
#endif
#ifndef APCB_TOKEN_UID_POWERDOWNMODE_VALUE
  #define APCB_TOKEN_UID_POWERDOWNMODE_VALUE                                   POWER_DOWN_BY_CHANNEL
#endif
#ifndef APCB_TOKEN_UID_IGNORESPDCHECKSUM_VALUE
  #define APCB_TOKEN_UID_IGNORESPDCHECKSUM_VALUE                               TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYMODEUNGANGED_VALUE
  #define APCB_TOKEN_UID_MEMORYMODEUNGANGED_VALUE                              TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYQUADRANKCAPABLE_VALUE
  #define APCB_TOKEN_UID_MEMORYQUADRANKCAPABLE_VALUE                           TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYRDIMMCAPABLE_VALUE
  #define APCB_TOKEN_UID_MEMORYRDIMMCAPABLE_VALUE                              TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYLRDIMMCAPABLE_VALUE
  #define APCB_TOKEN_UID_MEMORYLRDIMMCAPABLE_VALUE                             TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYUDIMMCAPABLE_VALUE
  #define APCB_TOKEN_UID_MEMORYUDIMMCAPABLE_VALUE                              TRUE
#endif
#ifndef APCB_TOKEN_UID_MEMORYSODIMMCAPABLE_VALUE
  #define APCB_TOKEN_UID_MEMORYSODIMMCAPABLE_VALUE                             TRUE
#endif
#ifndef APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATEEN_VALUE
  #define APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATEEN_VALUE                         FALSE
#endif
#ifndef APCB_TOKEN_UID_DIMMTYPEDDDR3CAPABLE_VALUE
  #define APCB_TOKEN_UID_DIMMTYPEDDDR3CAPABLE_VALUE                            FALSE
#endif
#ifndef APCB_TOKEN_UID_DIMMTYPELPDDDR3CAPABLE_VALUE
  #define APCB_TOKEN_UID_DIMMTYPELPDDDR3CAPABLE_VALUE                          FALSE
#endif
#ifndef APCB_TOKEN_UID_ENABLEBANKGROUPSWAP_VALUE
  #define APCB_TOKEN_UID_ENABLEBANKGROUPSWAP_VALUE                             2
#endif
#ifndef APCB_TOKEN_UID_ODTSCMDTHROTEN_VALUE
  #define APCB_TOKEN_UID_ODTSCMDTHROTEN_VALUE                                  FALSE
#endif
#ifndef APCB_TOKEN_UID_SWCMDTHROTEN_VALUE
  #define APCB_TOKEN_UID_SWCMDTHROTEN_VALUE                                    0
#endif
#ifndef APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN_VALUE
  #define APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN_VALUE                             0
#endif
#ifndef APCB_TOKEN_UID_ODTSCMDTHROTCYC_VALUE
  #define APCB_TOKEN_UID_ODTSCMDTHROTCYC_VALUE                                 0
#endif
#ifndef APCB_TOKEN_UID_SWCMDTHROTCYC_VALUE
  #define APCB_TOKEN_UID_SWCMDTHROTCYC_VALUE                                   0
#endif
#ifndef APCB_TOKEN_UID_DIMMSENSORCONF_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORCONF_VALUE                                  0
#endif
#ifndef APCB_TOKEN_UID_DIMMSENSORUPPER_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORUPPER_VALUE                                 0
#endif
#ifndef APCB_TOKEN_UID_DIMMSENSORLOWER_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORLOWER_VALUE                                 0
#endif
#ifndef APCB_TOKEN_UID_DIMMSENSORCRITICAL_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORCRITICAL_VALUE                              0
#endif
#ifndef APCB_TOKEN_UID_DIMMSENSORRESOLUTION_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORRESOLUTION_VALUE                            0
#endif
#ifndef APCB_TOKEN_UID_AUTOREFFINEGRANMODE_VALUE
  #define APCB_TOKEN_UID_AUTOREFFINEGRANMODE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_AUTOREFMODE_VALUE
  #define APCB_TOKEN_UID_AUTOREFMODE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE_VALUE                           FALSE
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_TESTMODE_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TESTMODE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON_VALUE                           FALSE
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR_VALUE                         TRUE
#endif

#ifndef APCB_TOKEN_UID_MEM_CPU_VREF_RANGE_VALUE
  #define APCB_TOKEN_UID_MEM_CPU_VREF_RANGE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE_VALUE
  #define APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_VALUE              0
#endif

//Data Eye APCB customization options.
#ifndef APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_VALUE                       0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_VALUE                        3
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_VALUE                        0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL_VALUE                 0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC_VALUE              0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL_VALUE                  0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL_VALUE                  0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC_VALUE               0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL_VALUE                   0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE_VALUE                         1
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN_VALUE                       0
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALUE            1
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALUE             1
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALUE           1
#endif

#ifndef APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALUE            1
#endif

#ifndef APCB_TOKEN_UID_POST_PACKAGE_REPAIR_ENABLE_VALUE
  #define APCB_TOKEN_UID_POST_PACKAGE_REPAIR_ENABLE_VALUE                      FALSE
#endif

#ifndef APCB_TOKEN_UID_MEM_TSME_ENABLE_DDR_VALUE
  #define APCB_TOKEN_UID_MEM_TSME_ENABLE_DDR_VALUE                             FALSE
#endif

#ifndef APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE_VALUE
  #define APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE_VALUE                         0
#endif

#ifndef APCB_TOKEN_UID_MEM_DATA_POISON_VALUE
  #define APCB_TOKEN_UID_MEM_DATA_POISON_VALUE                                 TRUE
#endif

#ifndef APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_REFRESH_EN_VALUE
  #define APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_REFRESH_EN_VALUE                  TRUE
#endif

#ifndef APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_EXTENDED_REFRESH_EN_VALUE
  #define APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_EXTENDED_REFRESH_EN_VALUE         FALSE
#endif

#ifndef APCB_TOKEN_UID_MEM_DATA_SCRAMBLE_VALUE
  #define APCB_TOKEN_UID_MEM_DATA_SCRAMBLE_VALUE                               TRUE
#endif

#ifndef APCB_TOKEN_UID_ENABLEBANKGROUPSWAPALT_VALUE
  #define APCB_TOKEN_UID_ENABLEBANKGROUPSWAPALT_VALUE                          FALSE
#endif

#ifndef APCB_TOKEN_UID_MEM_PS_ERROR_HANDLING_VALUE
  #define APCB_TOKEN_UID_MEM_PS_ERROR_HANDLING_VALUE                           0
#endif

#ifndef APCB_TOKEN_UID_MEM_FORCE_DATA_MASK_DISABLE_DDR4_VALUE
  #define APCB_TOKEN_UID_MEM_FORCE_DATA_MASK_DISABLE_DDR4_VALUE                FALSE
#endif

#ifndef APCB_TOKEN_UID_FCH_ACPI_SPREAD_SPECTRUM_VALUE
  #define APCB_TOKEN_UID_FCH_ACPI_SPREAD_SPECTRUM_VALUE                        TRUE
#endif

#ifndef APCB_TOKEN_UID_I2C_0_RX_SEL_VALUE
  #define APCB_TOKEN_UID_I2C_0_RX_SEL_VALUE                                    I2C_RX_SEL_SCHMIT_TRIGGER_1_8V
#endif

#ifndef APCB_TOKEN_UID_I2C_1_RX_SEL_VALUE
  #define APCB_TOKEN_UID_I2C_1_RX_SEL_VALUE                                    I2C_RX_SEL_SCHMIT_TRIGGER_1_8V
#endif

#ifndef APCB_TOKEN_UID_I2C_2_RX_SEL_VALUE
  #define APCB_TOKEN_UID_I2C_2_RX_SEL_VALUE                                    I2C_RX_SEL_SCHMIT_TRIGGER_1_8V
#endif

#ifndef APCB_TOKEN_UID_I2C_3_RX_SEL_VALUE
  #define APCB_TOKEN_UID_I2C_3_RX_SEL_VALUE                                    I2C_RX_SEL_SCHMIT_TRIGGER_1_8V
#endif

#ifndef APCB_TOKEN_UID_I2C_4_RX_SEL_VALUE
  #define APCB_TOKEN_UID_I2C_4_RX_SEL_VALUE                                    I2C_RX_SEL_SCHMIT_TRIGGER_1_8V
#endif

#ifndef APCB_TOKEN_UID_ESPI_PLT_RESET_DEASSERT_VALUE
  #define APCB_TOKEN_UID_ESPI_PLT_RESET_DEASSERT_VALUE                         TRUE
#endif

#ifndef APCB_TOKEN_UID_ESPI_ABL_INIT_ENABLE_VALUE
  #define APCB_TOKEN_UID_ESPI_ABL_INIT_ENABLE_VALUE                            TRUE
#endif

#ifndef APCB_TOKEN_UID_ESPI_IOMODE_VALUE
  #define APCB_TOKEN_UID_ESPI_IOMODE_VALUE                                     ESPI_IOMODEVAL_SINGLE
#endif

#ifndef APCB_TOKEN_UID_ESPI_CLOCK_VALUE
  #define APCB_TOKEN_UID_ESPI_CLOCK_VALUE                                      ESPI_CLOCKVAL_16MHZ
#endif

#ifndef APCB_TOKEN_UID_ESPI_IO80_ENABLE_VALUE
  #define APCB_TOKEN_UID_ESPI_IO80_ENABLE_VALUE                                TRUE
#endif

#ifndef APCB_TOKEN_UID_ESPI_KBC6064_ENABLE_VALUE
  #define APCB_TOKEN_UID_ESPI_KBC6064_ENABLE_VALUE                             TRUE
#endif

#ifndef APCB_TOKEN_UID_ESPI_CLOCK_PIN_SELECTION_VALUE
  #define APCB_TOKEN_UID_ESPI_CLOCK_PIN_SELECTION_VALUE                         ESPI_CLOCK_PIN_SELECTION_VAL_GPIO86
#endif

#ifndef APCB_TOKEN_UID_ESPI_DATA_SELECTION_VALUE
  #define APCB_TOKEN_UID_ESPI_DATA_SELECTION_VALUE                              ESPI_DATA_SELECTION_VAL_SPI_DATA2
#endif

#ifndef APCB_TOKEN_UID_ESPI_CS_SELECTION_VALUE
  #define APCB_TOKEN_UID_ESPI_CS_SELECTION_VALUE                                ESPI_CS_SELECTION_VAL_GPIO30
#endif

#ifndef APCB_TOKEN_UID_ESPI_ALERT_MODE_VALUE
  #define APCB_TOKEN_UID_ESPI_ALERT_MODE_VALUE                                  ESPI_ALERT_MODE_VAL_ALERT_PIN
#endif

#ifndef APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO_VALUE
  #define APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO_VALUE                               70
#endif

#ifndef APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO_OUTPUT_VALUE
  #define APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO_OUTPUT_VALUE                        ESPI_DATA_SEL_GPIO_OUTPUT_VAL_HIGH
#endif

#ifndef APCB_TOKEN_UID_ESPI_IRQ_POLARITY_VALUE
  #define APCB_TOKEN_UID_ESPI_IRQ_POLARITY_VALUE                                0
#endif

#ifndef APCB_TOKEN_UID_ESPI_BUS_MASTE_EN_VALUE
  #define APCB_TOKEN_UID_ESPI_BUS_MASTE_EN_VALUE                                TRUE
#endif

#ifndef APCB_TOKEN_UID_ESPI_TEMPRTC_VWEN_VALUE
  #define APCB_TOKEN_UID_ESPI_TEMPRTC_VWEN_VALUE                                FALSE
#endif

#ifndef APCB_TOKEN_UID_ESPI_VW_INDEX_SEL_VALUE
  #define APCB_TOKEN_UID_ESPI_VW_INDEX_SEL_VALUE                                0x00
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIO_CPUTEMP_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIO_CPUTEMP_VALUE                                0xFEC20200
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIO_RTCTIME_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIO_RTCTIME_VALUE                                0xFEC20250
#endif

#ifndef APCB_TOKEN_UID_ESPI_IRQ_MASK_VALUE
  #define APCB_TOKEN_UID_ESPI_IRQ_MASK_VALUE                                    0xFFFFFF
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE0_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE0_SIZE_VALUE                               7
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE0_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE0_BASE_VALUE                               0x3f8
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE1_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE1_SIZE_VALUE                               0xFF
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE1_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE1_BASE_VALUE                               0x600
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE2_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE2_SIZE_VALUE                               7
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE2_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE2_BASE_VALUE                               0x2f8
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE3_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE3_SIZE_VALUE                               7
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE3_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE3_BASE_VALUE                               0x3e8
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE4_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE4_SIZE_VALUE                               7
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE4_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE4_BASE_VALUE                               0x2e8
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE5_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE5_SIZE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE5_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE5_BASE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE6_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE6_SIZE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE6_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE6_BASE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE7_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE7_SIZE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE7_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE7_BASE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE8_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE8_SIZE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE8_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE8_BASE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE9_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE9_SIZE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE9_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE9_BASE_VALUE                               0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE10_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE10_SIZE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE10_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE10_BASE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE11_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE11_SIZE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE11_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE11_BASE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE12_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE12_SIZE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE12_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE12_BASE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE13_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE13_SIZE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE13_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE13_BASE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE14_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE14_SIZE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE14_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE14_BASE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE15_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE15_SIZE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_IORANGE15_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_IORANGE15_BASE_VALUE                              0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE0_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE0_SIZE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE0_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE0_BASE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE1_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE1_SIZE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE1_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE1_BASE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE2_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE2_SIZE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE2_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE2_BASE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE3_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE3_SIZE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE3_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE3_BASE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE4_SIZE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE4_SIZE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_ESPI_MMIORANGE4_BASE_VALUE
  #define APCB_TOKEN_UID_ESPI_MMIORANGE4_BASE_VALUE                             0
#endif

#ifndef APCB_TOKEN_UID_SMU_DPM_LEVEL_TRIMMING_VALUE
  #define APCB_TOKEN_UID_SMU_DPM_LEVEL_TRIMMING_VALUE                           FALSE
#endif

#ifndef APCB_TOKEN_UID_SMU_DPM_MP3_DEGRADE_VALUE
  #define APCB_TOKEN_UID_SMU_DPM_MP3_DEGRADE_VALUE                              FALSE
#endif

#ifndef APCB_TOKEN_UID_MODERN_STANDBY_ENABLE_VALUE
  #define APCB_TOKEN_UID_MODERN_STANDBY_ENABLE_VALUE                            TRUE
#endif

#ifndef BLDCFG_MEM_CLK_MAP
  #define BLDCFG_MEM_CLK_MAP            MEMCLK_DIS_MAP(ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00), // MemClkDisMap
#endif
#ifndef BLDCFG_CKE_TRI_MAP
  #define BLDCFG_CKE_TRI_MAP            CKE_TRI_MAP(ANY_SOCKET, ANY_CHANNEL, 0x05, 0x0A, 0xFF, 0xFF),                           // CkeTriMap
#endif
#ifndef BLDCFG_ODT_TRI_MAP
  #define BLDCFG_ODT_TRI_MAP            ODT_TRI_MAP(ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00),                           // OdtTriMap
#endif
#ifndef BLDCFG_CS_TRI_MAP
  #define BLDCFG_CS_TRI_MAP             CS_TRI_MAP(ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),    // CsTriMap
#endif
#ifndef BLDCFG_NUM_DIMMS_SUPPORTED
  #define BLDCFG_NUM_DIMMS_SUPPORTED    NUMBER_OF_DIMMS_SUPPORTED(ANY_SOCKET, ANY_CHANNEL, 2),                                  // NumberDimmSupported
#endif
#ifndef BLDCFG_NUM_SOLDERED_DOWN_DIMM_SUPPORTED
  #define BLDCFG_NUM_SOLDERED_DOWN_DIMM_SUPPORTED                                              // NumberSolderedDownDimmsSupported
#endif
#ifndef BLDCFG_NUM_CS_SUPPORTED
  #define BLDCFG_NUM_CS_SUPPORTED                                                              // NumberChipSelectsSupported
#endif

#ifndef BLDCFG_NUM_CHNL_SUPPORTED
  #define BLDCFG_NUM_CHNL_SUPPORTED     NUMBER_OF_CHANNELS_SUPPORTED(ANY_SOCKET, 2),                                            // NumberChannelsSupported
#endif
#ifndef BLDCFG_BUS_SPEED_OVERRIDE
  #define BLDCFG_BUS_SPEED_OVERRIDE                                                             // OverrideBusSpeed
#endif
#ifndef BLDCFG_DRAM_TECH_OVERRIDE
  #define BLDCFG_DRAM_TECH_OVERRIDE                                                             // DramTechnology
#endif
#ifndef BLDCFG_SOLDRD_DOWN_DIMM_TYPE
  #define BLDCFG_SOLDRD_DOWN_DIMM_TYPE                                                          // SolderedDownSoDimmType
#endif
#ifndef BLDCFG_MEM_POWER_POLICY_OVERRIDE
  #define BLDCFG_MEM_POWER_POLICY_OVERRIDE                                                      // MemoryPowerPolicyOverride
#endif
#ifndef BLDCFG_NUM_MOTHERBOARD_LAYERS
  #define BLDCFG_NUM_MOTHERBOARD_LAYERS                                                         // MotherboardLayers
#endif



#ifndef BLDCFG_MEMORY_QUADRANK_TYPE
  #define BLDCFG_MEMORY_QUADRANK_TYPE               QUADRANK_UNBUFFERED
#endif
#ifndef BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING
  #define BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING    FALSE
#endif

#ifndef BLDCFG_ONLINE_SPARE
  #define BLDCFG_ONLINE_SPARE                       FALSE
#endif
#ifndef BLDCFG_USE_BURST_MODE
  #define BLDCFG_USE_BURST_MODE                     FALSE
#endif
#ifndef BLDCFG_HEAP_DRAM_ADDRESS
  #define BLDCFG_HEAP_DRAM_ADDRESS                  0xB0000ul
#endif
#ifndef BLDCFG_1GB_ALIGN
  #define BLDCFG_1GB_ALIGN                          FALSE
#endif
#ifndef BLDCFG_UMA_ALIGNMENT
  #define BLDCFG_UMA_ALIGNMENT                      UMA_4MB_ALIGNED
#endif
#ifndef BLDCFG_DIMM_TYPE_PACKAGE
  #define BLDCFG_DIMM_TYPE_PACKAGE                  UDIMM_PACKAGE
#endif

//MPM
#ifndef APCB_TOKEN_UID_WIRELESS_MANAGEABILITY_VALUE
  #define APCB_TOKEN_UID_WIRELESS_MANAGEABILITY_VALUE      FALSE
#endif

#ifndef APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY_VALUE
  #define APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY_VALUE     FALSE
#endif

#ifndef APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY_VALUE
  #define APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY_VALUE  FALSE
#endif

//
// Please Review the below SMBUS Address, based off your schematics and comment out lines which doesnt belongs
// to the project for which this fie is being customized
//

#ifndef BLDCFG_SPD_P0_CH_A_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_A_DIMM0_ADDRESS                                         0xA0
#endif
#ifndef BLDCFG_SPD_P0_CH_A_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_A_DIMM1_ADDRESS                                         0xA4
#endif

#ifndef BLDCFG_SPD_P0_CH_B_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_B_DIMM0_ADDRESS                                         0xA2
#endif
#ifndef BLDCFG_SPD_P0_CH_B_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_B_DIMM1_ADDRESS                                         0xA6
#endif

#ifndef BLDCFG_SPD_P0_CH_C_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_C_DIMM0_ADDRESS                                         0xA8
#endif
#ifndef BLDCFG_SPD_P0_CH_C_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_C_DIMM1_ADDRESS                                         0xAC
#endif

#ifndef BLDCFG_SPD_P0_CH_D_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_D_DIMM0_ADDRESS                                         0xAA
#endif
#ifndef BLDCFG_SPD_P0_CH_D_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_D_DIMM1_ADDRESS                                         0xAE
#endif

#ifndef BLDCFG_SPD_P0_CH_E_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_E_DIMM0_ADDRESS                                         0xA0
#endif
#ifndef BLDCFG_SPD_P0_CH_E_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_E_DIMM1_ADDRESS                                         0xA4
#endif

#ifndef BLDCFG_SPD_P0_CH_F_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_F_DIMM0_ADDRESS                                         0xA2
#endif
#ifndef BLDCFG_SPD_P0_CH_F_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_F_DIMM1_ADDRESS                                         0xA6
#endif

#ifndef BLDCFG_SPD_P0_CH_G_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_G_DIMM0_ADDRESS                                         0xA8
#endif
#ifndef BLDCFG_SPD_P0_CH_G_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_G_DIMM1_ADDRESS                                         0xAC
#endif

#ifndef BLDCFG_SPD_P0_CH_H_DIMM0_ADDRESS
  #define BLDCFG_SPD_P0_CH_H_DIMM0_ADDRESS                                         0xAA
#endif
#ifndef BLDCFG_SPD_P0_CH_H_DIMM1_ADDRESS
  #define BLDCFG_SPD_P0_CH_H_DIMM1_ADDRESS                                         0xAE
#endif

#ifndef BLDCFG_I2C_MUX_P0_ADRRESS
  #define BLDCFG_I2C_MUX_P0_ADRRESS                                               0x94
#endif

#ifndef BLDCFG_I2C_MUX_P0_COMMAND_CODE
  #define BLDCFG_I2C_MUX_P0_COMMAND_CODE                                          0x3
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_A_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_A_DIMM0_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_A_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_A_DIMM1_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_B_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_B_DIMM0_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_B_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_B_DIMM1_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_C_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_C_DIMM0_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_C_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_C_DIMM1_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_D_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_D_DIMM0_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_D_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_D_DIMM1_DATA_BYTE                                  0x80
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_E_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_E_DIMM0_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_E_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_E_DIMM1_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_F_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_F_DIMM0_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_F_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_F_DIMM1_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_G_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_G_DIMM0_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_G_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_G_DIMM1_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_H_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_H_DIMM0_DATA_BYTE                                  0x40
#endif

#ifndef BLDCFG_I2C_MUX_P0_CH_H_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P0_CH_H_DIMM1_DATA_BYTE                                  0x40
#endif


#ifndef BLDCFG_SPD_P1_CH_A_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_A_DIMM0_ADDRESS                                         0xA0
#endif
#ifndef BLDCFG_SPD_P1_CH_A_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_A_DIMM1_ADDRESS                                         0xA4
#endif

#ifndef BLDCFG_SPD_P1_CH_B_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_B_DIMM0_ADDRESS                                         0xA2
#endif
#ifndef BLDCFG_SPD_P1_CH_B_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_B_DIMM1_ADDRESS                                         0xA6
#endif

#ifndef BLDCFG_SPD_P1_CH_C_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_C_DIMM0_ADDRESS                                         0xA8
#endif
#ifndef BLDCFG_SPD_P1_CH_C_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_C_DIMM1_ADDRESS                                         0xAC
#endif

#ifndef BLDCFG_SPD_P1_CH_D_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_D_DIMM0_ADDRESS                                         0xAA
#endif
#ifndef BLDCFG_SPD_P1_CH_D_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_D_DIMM1_ADDRESS                                         0xAE
#endif

#ifndef BLDCFG_SPD_P1_CH_E_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_E_DIMM0_ADDRESS                                         0xA0
#endif
#ifndef BLDCFG_SPD_P1_CH_E_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_E_DIMM1_ADDRESS                                         0xA4
#endif

#ifndef BLDCFG_SPD_P1_CH_F_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_F_DIMM0_ADDRESS                                         0xA2
#endif
#ifndef BLDCFG_SPD_P1_CH_F_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_F_DIMM1_ADDRESS                                         0xA6
#endif

#ifndef BLDCFG_SPD_P1_CH_G_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_G_DIMM0_ADDRESS                                         0xA8
#endif
#ifndef BLDCFG_SPD_P1_CH_G_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_G_DIMM1_ADDRESS                                         0xAC
#endif

#ifndef BLDCFG_SPD_P1_CH_H_DIMM0_ADDRESS
  #define BLDCFG_SPD_P1_CH_H_DIMM0_ADDRESS                                         0xAA
#endif
#ifndef BLDCFG_SPD_P1_CH_H_DIMM1_ADDRESS
  #define BLDCFG_SPD_P1_CH_H_DIMM1_ADDRESS                                         0xAE
#endif

#ifndef BLDCFG_I2C_MUX_P1_ADRRESS
  #define BLDCFG_I2C_MUX_P1_ADRRESS                                               0x94
#endif

#ifndef BLDCFG_I2C_MUX_P1_COMMAND_CODE
  #define BLDCFG_I2C_MUX_P1_COMMAND_CODE                                          0x3
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_A_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_A_DIMM0_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_A_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_A_DIMM1_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_B_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_B_DIMM0_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_B_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_B_DIMM1_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_C_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_C_DIMM0_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_C_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_C_DIMM1_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_D_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_D_DIMM0_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_D_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_D_DIMM1_DATA_BYTE                                  0x20
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_E_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_E_DIMM0_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_E_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_E_DIMM1_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_F_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_F_DIMM0_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_F_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_F_DIMM1_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_G_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_G_DIMM0_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_G_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_G_DIMM1_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_H_DIMM0_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_H_DIMM0_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_I2C_MUX_P1_CH_H_DIMM1_DATA_BYTE
  #define BLDCFG_I2C_MUX_P1_CH_H_DIMM1_DATA_BYTE                                  0x10
#endif

#ifndef BLDCFG_MEM_LAYOOUT_TYPE
  #define BLDCFG_MEM_LAYOOUT_TYPE           MEMORY_LAYOUT_TYPE_DIMM
#endif

#ifndef BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_A_DIMM_0_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_A_DIMM_0_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_A_DIMM_1_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_A_DIMM_1_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_A_DIMM_1_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_A_DIMM_1_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_A_DIMM_1_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_A_DIMM_1_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_B_DIMM_0_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_B_DIMM_0_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_B_DIMM_0_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_B_DIMM_0_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_B_DIMM_0_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_B_DIMM_0_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_B_DIMM_1_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_B_DIMM_1_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_B_DIMM_1_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_B_DIMM_1_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_B_DIMM_1_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_B_DIMM_1_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_C_DIMM_0_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_C_DIMM_0_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_C_DIMM_0_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_C_DIMM_0_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_C_DIMM_0_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_C_DIMM_0_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_C_DIMM_1_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_C_DIMM_1_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_C_DIMM_1_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_C_DIMM_1_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_C_DIMM_1_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_C_DIMM_1_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_D_DIMM_0_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_D_DIMM_0_SPD_VALID                                FALSE
#endif

#ifndef BLDCFG_DRAMDOWN_CH_D_DIMM_0_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_D_DIMM_0_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_D_DIMM_0_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_D_DIMM_0_SPD_DATA                                 0x00
#endif

#ifndef BLDCFG_DRAMDOWN_CH_D_DIMM_1_SPD_VALID
  #define BLDCFG_DRAMDOWN_CH_D_DIMM_1_SPD_VALID                                FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_D_DIMM_1_DIMM_PRESENT
  #define BLDCFG_DRAMDOWN_CH_D_DIMM_1_DIMM_PRESENT                             FALSE
#endif
#ifndef BLDCFG_DRAMDOWN_CH_D_DIMM_1_SPD_DATA
  #define BLDCFG_DRAMDOWN_CH_D_DIMM_1_SPD_DATA                                 0x00
#endif
#ifndef APCB_TOKEN_UID_CCX_MIN_SEV_ASID_VALUE
  #define APCB_TOKEN_UID_CCX_MIN_SEV_ASID_VALUE                                1
#endif

#ifndef APCB_TOKEN_UID_CCX_SEV_ASID_COUNT_VALUE
  #define APCB_TOKEN_UID_CCX_SEV_ASID_COUNT_VALUE                              0x1FD
#endif

#ifndef APCB_TOKEN_UID_XTRIG7_WORKAROUND_VALUE
  #define APCB_TOKEN_UID_XTRIG7_WORKAROUND_VALUE                               1
#endif

#ifndef APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE_VALUE
  #define APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE_VALUE                           0
#endif

#ifndef APCB_TOKEN_UID_PSP_TP_PORT_VALUE
  #define APCB_TOKEN_UID_PSP_TP_PORT_VALUE                                     TRUE
#endif

#ifndef APCB_TOKEN_UID_PSP_ERROR_DISPLAY_VALUE
  #define APCB_TOKEN_UID_PSP_ERROR_DISPLAY_VALUE                               TRUE
#endif

#ifndef APCB_TOKEN_UID_PSP_EVENT_LOG_DISPLAY_VALUE
  #define APCB_TOKEN_UID_PSP_EVENT_LOG_DISPLAY_VALUE                           TRUE
#endif

#ifndef APCB_TOKEN_UID_PSP_STOP_ON_ERROR_VALUE
  #define APCB_TOKEN_UID_PSP_STOP_ON_ERROR_VALUE                               FALSE
#endif

#ifndef APCB_TOKEN_UID_PSP_SYSHUB_WDT_INTERVAL_VALUE
  #define APCB_TOKEN_UID_PSP_SYSHUB_WDT_INTERVAL_VALUE                         2600
#endif

#ifndef APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG_VALUE
  #define APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG_VALUE                           0xff
#endif

#ifndef APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX_VALUE
  #define APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX_VALUE                          0
#endif

#ifndef APCB_TOKEN_UID_PSP_FTPM_NVSTORAGE_SIZE_VALUE
  #define APCB_TOKEN_UID_PSP_FTPM_NVSTORAGE_SIZE_VALUE                         0
#endif

#ifndef APCB_TOKEN_UID_PSP_RPMC_ENABLE_VALUE
  #define APCB_TOKEN_UID_PSP_RPMC_ENABLE_VALUE                                 0xff
#endif

#ifndef APCB_TOKEN_UID_RPMC_RECOVERY_TYPE_VALUE
  #define APCB_TOKEN_UID_RPMC_RECOVERY_TYPE_VALUE                              0xff
#endif

#ifndef APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT_VALUE
  #define APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT_VALUE               0
#endif

#ifndef APCB_TOKEN_UID_DF_GMI_ENCRYPT_VALUE
  #define APCB_TOKEN_UID_DF_GMI_ENCRYPT_VALUE                                  3
#endif
#ifndef APCB_TOKEN_UID_DF_XGMI_ENCRYPT_VALUE
  #define APCB_TOKEN_UID_DF_XGMI_ENCRYPT_VALUE                                 3
#endif
#ifndef APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT_VALUE
  #define APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT_VALUE                     3
#endif
#ifndef APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM_VALUE
  #define APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM_VALUE                    3
#endif
#ifndef APCB_TOKEN_UID_DF_PROBE_FILTER_VALUE
  #define APCB_TOKEN_UID_DF_PROBE_FILTER_VALUE                                 3
#endif
#ifndef APCB_TOKEN_UID_DF_BOTTOMIO_VALUE
  #define APCB_TOKEN_UID_DF_BOTTOMIO_VALUE                                     0xE0
#endif
#ifndef APCB_TOKEN_UID_DF_MEM_INTERLEAVING_VALUE
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_VALUE                             7
#endif
#ifndef APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE_VALUE
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE_VALUE                        7
#endif
#ifndef APCB_TOKEN_UID_DF_CHAN_INTLV_HASH_VALUE
  #define APCB_TOKEN_UID_DF_CHAN_INTLV_HASH_VALUE                              3
#endif
#ifndef APCB_TOKEN_UID_DF_PCI_MMIO_SIZE_VALUE
  #define APCB_TOKEN_UID_DF_PCI_MMIO_SIZE_VALUE                                0x10000000
#endif
#ifndef APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS_VALUE
  #define APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS_VALUE                  100  // 0.001%
#endif
#ifndef APCB_TOKEN_UID_DF_MEM_CLEAR_VALUE
  #define APCB_TOKEN_UID_DF_MEM_CLEAR_VALUE                                    3
#endif
#ifndef APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG_VALUE
  #define APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG_VALUE                              3
#endif
#ifndef APCB_TOKEN_UID_DF_DRAM_NPS_VALUE
  #define APCB_TOKEN_UID_DF_DRAM_NPS_VALUE                                     7
#endif
#ifndef APCB_TOKEN_UID_DF_INVERT_DRAM_MAP_VALUE
  #define APCB_TOKEN_UID_DF_INVERT_DRAM_MAP_VALUE                              3
#endif
#ifndef APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED_VALUE
  #define APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED_VALUE                         0xFF
#endif
#ifndef APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED_VALUE
  #define APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED_VALUE                         0xFF
#endif
#ifndef APCB_TOKEN_UID_DF_REMAP_AT_1TB_VALUE
  #define APCB_TOKEN_UID_DF_REMAP_AT_1TB_VALUE                                 0xFF
#endif

//MPM Definitions
#ifndef APCB_TOKEN_UID_MPM_AC_GPIO_INDEX_VALUE
  #define APCB_TOKEN_UID_MPM_AC_GPIO_INDEX_VALUE                               0xFF
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_GPIO_INDEX_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_GPIO_INDEX_VALUE                             0xFF
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_CONTROL_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_CONTROL_VALUE                        0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_CMDSTSPORT_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_CMDSTSPORT_VALUE                     0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATAPORT_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATAPORT_VALUE                       0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_OFFSET_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_OFFSET_VALUE                         0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_MASK_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_MASK_VALUE                           0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATA_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATA_VALUE                           0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIOPIN_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIOPIN_VALUE                        0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIODATA_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIODATA_VALUE                       0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXEN_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXEN_VALUE                        0
#endif

#ifndef APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXDATA_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXDATA_VALUE                      0
#endif

#ifndef APCB_TOKEN_UID_CPU_SYNC_FLOOD_ON_MCA_VALUE
  #define APCB_TOKEN_UID_CPU_SYNC_FLOOD_ON_MCA_VALUE                           0x0F
#endif

#ifndef APCB_TOKEN_UID_DF_EXT_IP_SYNC_FLOOD_PROP_VALUE
  #define APCB_TOKEN_UID_DF_EXT_IP_SYNC_FLOOD_PROP_VALUE                       0x0F
#endif

#ifndef APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP_VALUE
  #define APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP_VALUE                              0x0F
#endif

//
// GNB Specific Definitions
//
#ifndef APCB_TOKEN_UID_BMC_SOCKET_NUMBER_VALUE
  #define APCB_TOKEN_UID_BMC_SOCKET_NUMBER_VALUE                               0x0F
#endif
#ifndef APCB_TOKEN_UID_BMC_START_LANE_VALUE
  #define APCB_TOKEN_UID_BMC_START_LANE_VALUE                                  0xFF
#endif
#ifndef APCB_TOKEN_UID_BMC_END_LANE_VALUE
  #define APCB_TOKEN_UID_BMC_END_LANE_VALUE                                    0xFF
#endif

#ifndef APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE_VALUE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE_VALUE                     0xFF
#endif
#ifndef APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE_VALUE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE_VALUE                       0xFF
#endif

#ifndef APCB_TOKEN_UID_PCIE_RESET_GPIO_PIN_SELECT_VALUE
  #define APCB_TOKEN_UID_PCIE_RESET_GPIO_PIN_SELECT_VALUE                      0xFFFFFFFF
#endif

#ifndef APCB_TOKEN_UID_BMC_DEVICE_VALUE
  #define APCB_TOKEN_UID_BMC_DEVICE_VALUE                                      0
#endif

#ifndef APCB_TOKEN_UID_BMC_FUNCTION_VALUE
  #define APCB_TOKEN_UID_BMC_FUNCTION_VALUE                                    0
#endif

#ifndef APCB_TOKEN_UID_BMC_LINK_SPEED_VALUE
  #define APCB_TOKEN_UID_BMC_LINK_SPEED_VALUE                                  0
#endif

#ifndef APCB_TOKEN_UID_PCIE_RESET_CONTROL_VALUE
  #define APCB_TOKEN_UID_PCIE_RESET_CONTROL_VALUE                              TRUE
#endif
//
// UART Enable Disable through SOC UART/ LPC-SUPER IO
// 0 - LPC UART, 1 - SOC UART0
//
#ifndef APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE_VALUE
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE_VALUE                          0
#endif

#ifndef APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_VALUE
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_VALUE                     0
#endif

#ifndef APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE_VALUE
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE_VALUE                    0
#endif

#ifndef APCB_TOKEN_UID_FCH_SMBUS_SPEED_VALUE
  #define APCB_TOKEN_UID_FCH_SMBUS_SPEED_VALUE                                 0x2a    // Frequency = 66 MHz/(SMBusTiming * 4). Default 0x2a --> 392 khz
#endif

#ifndef APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL_VALUE
  #define APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL_VALUE                            0xff
#endif

#ifndef APCB_TOKEN_UID_MEM_TIMING_RCD_F0RC0F_NLADD_OVERRIDE_VALUE
  #define APCB_TOKEN_UID_MEM_TIMING_RCD_F0RC0F_NLADD_OVERRIDE_VALUE            0xff
#endif

#ifndef APCB_TOKEN_UID_GROUP_D_PLATFORM_VALUE
  #define APCB_TOKEN_UID_GROUP_D_PLATFORM_VALUE                                FALSE
#endif

#ifndef APCB_TOKEN_UID_GPU_TRANSLATION_CACHE_VALUE
  #define APCB_TOKEN_UID_GPU_TRANSLATION_CACHE_VALUE                           TRUE
#endif

#ifndef APCB_TOKEN_UID_PCIE_RESET_GPIO_PIN_SELECT_VALUE
  #define APCB_TOKEN_UID_PCIE_RESET_GPIO_PIN_SELECT_VALUE                      0xFFFFFFFF
#endif

#ifndef APCB_TOKEN_UID_RCD_PARITY_VALUE
  #define APCB_TOKEN_UID_RCD_PARITY_VALUE                                      FALSE
#endif

#ifndef APCB_TOKEN_UID_DRAM_ADDRESS_COMMAND_PARITY_RETRY_VALUE
  #define APCB_TOKEN_UID_DRAM_ADDRESS_COMMAND_PARITY_RETRY_VALUE               FALSE
#endif

#ifndef APCB_TOKEN_UID_MAX_PARITY_ERROR_REPLAY_VALUE
  #define APCB_TOKEN_UID_MAX_PARITY_ERROR_REPLAY_VALUE                         8
#endif

#ifndef APCB_TOKEN_UID_MEM_CTRLLER_WR_CRC_VALUE
  #define APCB_TOKEN_UID_MEM_CTRLLER_WR_CRC_VALUE                              FALSE
#endif

#ifndef APCB_TOKEN_UID_DRAM_WRITE_CRC_ENABLE_AND_RETRY_LIMIT_VALUE
  #define APCB_TOKEN_UID_DRAM_WRITE_CRC_ENABLE_AND_RETRY_LIMIT_VALUE           FALSE
#endif

#ifndef APCB_TOKEN_UID_MAX_WRITE_CRC_ERROR_REPLAY_VALUE
  #define APCB_TOKEN_UID_MAX_WRITE_CRC_ERROR_REPLAY_VALUE                      8
#endif

#ifndef APCB_TOKEN_UID_UECC_RETRY_DDR4_VALUE
  #define APCB_TOKEN_UID_UECC_RETRY_DDR4_VALUE                                 FALSE
#endif

//#define BLDCFG_CONOUTCTRL_BREAKPOINT          FALSE
//#define BLDCFG_CONOUTCTRL_BREAKPOINT_ALLDIE   FALSE

#ifndef APCB_TOKEN_UID_MEM_RESERVE_ADDR_LOW_VALUE
  #define APCB_TOKEN_UID_MEM_RESERVE_ADDR_LOW_VALUE                            0x0
#endif

#ifndef APCB_TOKEN_UID_MEM_RESERVE_ADDR_HIGH_VALUE
  #define APCB_TOKEN_UID_MEM_RESERVE_ADDR_HIGH_VALUE                           0x1
#endif

#ifndef APCB_TOKEN_UID_MEM_RESERVE_SIZE_LOW_VALUE
  #define APCB_TOKEN_UID_MEM_RESERVE_SIZE_LOW_VALUE                            0x100000
#endif

#ifndef APCB_TOKEN_UID_MEM_RESERVE_SIZE_HIGH_VALUE
  #define APCB_TOKEN_UID_MEM_RESERVE_SIZE_HIGH_VALUE                           0
#endif

#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_ENABLE          TRUE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_FLOW          TRUE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_STATUS          TRUE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_SET_REG          TRUE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_GET_REG          FALSE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_PMU          TRUE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_PMU_SRAM_READ          FALSE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_PMU_SRAM_WRITE          FALSE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_TEST_VERBOSE          FALSE
#define BLDCFG_CONOUTCTRL_ABL_CONSOLE_FILTER_MEM_BASIC_OUTPUT          TRUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_EN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_CS_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_SPD_READ_OPTIMIZATION_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_EN_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_UECC_RETRY_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_POISONING_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_RM_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DATA_SCRAMBLE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_GEAR_DOWN_MODE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_AGGRESSORS_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PWR_DN_EN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_ALT_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_WR_CRC_EN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_DIS_MEM_ERR_INJ_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_RCD_PARITY_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_WRITE_CRC_RETRY_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_DBG_MEM_EARLY_PAGE_ACTIVATE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CCX_PPIN_OPT_IN_VALUE

//Alias:
//#define APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP_VALUE

//Alias:
//#define APCB_TOKEN_UID_SOLDERDOWNDRAM_VALUE

//Alias:
//#define APCB_TOKEN_UID_DF_ENABLE_CHAN_INTLV_HASH_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_SWIZZLE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASHING_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_SPEED_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDRD_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDWR_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRP_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRAS_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TSME_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRC_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRC_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_S_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_L_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TFAW_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TFAW_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_S_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_L_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWR_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWR_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_L_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_L_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC2_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC4_CTRL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_FAIL_CNT_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PROC_ODT_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_TESTMODE_VALUE

//Alias:
//#define APCB_TOKEN_UID_CCX_P0_SETTING_VALUE

//Alias:
//#define APCB_TOKEN_UID_CCX_P0_FID_VALUE

//Alias:
//#define APCB_TOKEN_UID_CCX_P0_DID_VALUE

//Alias:
//#define APCB_TOKEN_UID_CCX_P0_VID_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SD_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_DD_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SD_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_DD_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDWR_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRRD_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRTP_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCWL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_NOM_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_WR_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_PARK_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCKE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_CLDO_VDDP_CTL_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER2_T_MODE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_TIMING_CTL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_SETUP_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CS_ODT_SETUP_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CKE_SETUP_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_DRV_STREN_CTL_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CLK_DRV_STREN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CKE_DRV_STREN_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_DF_CMN_SYNC_FLOOD_PROP_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_DIMM_CONFIG_CHECK_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_CPU_SMU_PSP_PROD_MODE_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_ERROR_MAX_REPLAY_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_RETRY_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_WRITE_CRC_ERROR_MAX_REPLAY_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_DF_CMN_CAKE_CRC_PERF_BOUNDS_VALUE

//Alias:
//#define APCB_TOKEN_UID_CPU_SYNC_FLOOD_ON_MCA_VALUE

//Alias:
//#define APCB_TOKEN_UID_DF_SPF_WAY_CTRL_VALUE

//Alias:
//#define APCB_TOKEN_UID_TEST_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC2_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC4_DDR4_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_SUB_TEST_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_CLDOVDD_PVOLTAGE_VALUE

//Alias:
//#define APCB_TOKEN_UID_PCIE_RESET_PIN_SELECT_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_FCLK_FREQUENCY_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_UCLK_DIV1_MODE_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_UCLK_FREQUENCY_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDCR_SOC_VID_VALUE

//Alias:
//#define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDP_VOLTAGE_VALUE

//Alias:
//#define APCB_TOKEN_UID_DDRROUTEBALANCEDTEE_VALUE

//Alias:
//#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32_VALUE

//Alias:
//#define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32_VALUE

//Alias:
//#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_U32_VALUE

//Alias:
//#define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_L32_VALUE

// =================================================================================================
// BOARD MASK definition - This should be defined by ApcbCutomizedBoardDefinitions.h for each board folder
// =================================================================================================
#ifndef BLDCFG_APCB_DATA_BOARD_MASK
  #error BLDCFG_APCB_DATA_BOARD_MASK is not defined. Check ApcbCutomizedBoardDefinitions.h in each board folder
#endif

// =================================================================================================
// Legacy APCB Customization Definiation Support
// =================================================================================================
#ifdef BLDCFG_TIMING_MODE_SELECT
  #undef APCB_TOKEN_UID_USERTIMINGMODE_VALUE
  #define APCB_TOKEN_UID_USERTIMINGMODE_VALUE                                    BLDCFG_TIMING_MODE_SELECT
#endif

#ifdef BLDCFG_MEMORY_CLOCK_SELECT
  #undef APCB_TOKEN_UID_MEMCLOCKVALUE_VALUE
  #define APCB_TOKEN_UID_MEMCLOCKVALUE_VALUE                                     BLDCFG_MEMORY_CLOCK_SELECT
#endif

#ifdef BLDCFG_MEMORY_ENABLE_CHIPSELECT_INTERLEAVING
  #undef APCB_TOKEN_UID_ENABLECHIPSELECTINTLV_VALUE
  #define APCB_TOKEN_UID_ENABLECHIPSELECTINTLV_VALUE                             BLDCFG_MEMORY_ENABLE_CHIPSELECT_INTERLEAVING
#endif

#ifdef BLDCFG_ENABLE_ECC_FEATURE
  #undef APCB_TOKEN_UID_ENABLEECCFEATURE_VALUE
  #define APCB_TOKEN_UID_ENABLEECCFEATURE_VALUE                                  BLDCFG_ENABLE_ECC_FEATURE
#endif

#ifdef BLDCFG_MEMORY_POWER_DOWN
  #undef APCB_TOKEN_UID_ENABLEPOWERDOWN_VALUE
  #define APCB_TOKEN_UID_ENABLEPOWERDOWN_VALUE                                   BLDCFG_MEMORY_POWER_DOWN
#endif

#ifdef BLDCFG_PARITY_CONTROL
  #undef APCB_TOKEN_UID_ENABLEPARITY_VALUE
  #define APCB_TOKEN_UID_ENABLEPARITY_VALUE                                      BLDCFG_PARITY_CONTROL
#endif

#ifdef BLDCFG_BANK_SWIZZLE
  #undef APCB_TOKEN_UID_ENABLEBANKSWIZZLE_VALUE
  #define APCB_TOKEN_UID_ENABLEBANKSWIZZLE_VALUE                                 BLDCFG_BANK_SWIZZLE
#endif

#ifdef BLDCFG_MEMORY_RESTORE_CONTROL
  #undef APCB_TOKEN_UID_MEMRESTORECTL_VALUE
  #define APCB_TOKEN_UID_MEMRESTORECTL_VALUE                                     BLDCFG_MEMORY_RESTORE_CONTROL
#endif

#ifdef BLDCFG_PMU_TRAINING_MODE
  #undef APCB_TOKEN_UID_PMUTRAINMODE_VALUE
  #define APCB_TOKEN_UID_PMUTRAINMODE_VALUE                                      BLDCFG_PMU_TRAINING_MODE
#endif

#ifdef BLDCFG_ECC_SYMBOL_SIZE
  #undef APCB_TOKEN_UID_ECCSYMBOLSIZE_VALUE
  #define APCB_TOKEN_UID_ECCSYMBOLSIZE_VALUE                                     BLDCFG_ECC_SYMBOL_SIZE
#endif

#ifdef BLDCFG_MEMORY_ALL_CLOCKS_ON
  #undef APCB_TOKEN_UID_MEMORYALLCLOCKSON_VALUE
  #define APCB_TOKEN_UID_MEMORYALLCLOCKSON_VALUE                                 BLDCFG_MEMORY_ALL_CLOCKS_ON
#endif

#ifdef BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT
  #undef APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT_VALUE
  #define APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT_VALUE                           BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT
#endif

#ifdef BLDCFG_POWER_DOWN_MODE
  #undef APCB_TOKEN_UID_POWERDOWNMODE_VALUE
  #define APCB_TOKEN_UID_POWERDOWNMODE_VALUE                                     BLDCFG_POWER_DOWN_MODE
#endif

#ifdef BLDCFG_IGNORE_SPD_CHECKSUM
  #undef APCB_TOKEN_UID_IGNORESPDCHECKSUM_VALUE
  #define APCB_TOKEN_UID_IGNORESPDCHECKSUM_VALUE                                 BLDCFG_IGNORE_SPD_CHECKSUM
#endif

#ifdef BLDCFG_BANK_SWAP_GROUP_ENABLE
  #undef APCB_TOKEN_UID_ENABLEBANKGROUPSWAP_VALUE
  #define APCB_TOKEN_UID_ENABLEBANKGROUPSWAP_VALUE                               BLDCFG_BANK_SWAP_GROUP_ENABLE
#endif

#ifdef BLDCFG_ODT_CMD_THROT_ENABLE
  #undef APCB_TOKEN_UID_ODTSCMDTHROTEN_VALUE
  #define APCB_TOKEN_UID_ODTSCMDTHROTEN_VALUE                                    BLDCFG_ODT_CMD_THROT_ENABLE
#endif

#ifdef BLDCFG_SW_CMD_THROT_ENABLE
  #undef APCB_TOKEN_UID_SWCMDTHROTEN_VALUE
  #define APCB_TOKEN_UID_SWCMDTHROTEN_VALUE                                      BLDCFG_SW_CMD_THROT_ENABLE
#endif

#ifdef BLDCFG_FORCE_POWER_DOWN_THROT_ENABLE
  #undef APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN_VALUE
  #define APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN_VALUE                               BLDCFG_FORCE_POWER_DOWN_THROT_ENABLE
#endif

#ifdef BLDCFG_ODT_CMD_THROT_CYCLE
  #undef APCB_TOKEN_UID_ODTSCMDTHROTCYC_VALUE
  #define APCB_TOKEN_UID_ODTSCMDTHROTCYC_VALUE                                   BLDCFG_ODT_CMD_THROT_CYCLE
#endif

#ifdef BLDCFG_SW_CMD_THROT_CYCLE
  #undef APCB_TOKEN_UID_SWCMDTHROTCYC_VALUE
  #define APCB_TOKEN_UID_SWCMDTHROTCYC_VALUE                                     BLDCFG_SW_CMD_THROT_CYCLE
#endif

#ifdef BLDCFG_DIMM_SENSOR_CONFIGURATION
  #undef APCB_TOKEN_UID_DIMMSENSORCONF_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORCONF_VALUE                                    BLDCFG_DIMM_SENSOR_CONFIGURATION
#endif

#ifdef BLDCFG_DIMM_SENSOR_UPPER
  #undef APCB_TOKEN_UID_DIMMSENSORUPPER_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORUPPER_VALUE                                   BLDCFG_DIMM_SENSOR_UPPER
#endif

#ifdef BLDCFG_DIMM_SENSOR_LOWER
  #undef APCB_TOKEN_UID_DIMMSENSORLOWER_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORLOWER_VALUE                                   BLDCFG_DIMM_SENSOR_LOWER
#endif

#ifdef BLDCFG_DIMM_SENSOR_CRITICAL
  #undef APCB_TOKEN_UID_DIMMSENSORCRITICAL_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORCRITICAL_VALUE                                BLDCFG_DIMM_SENSOR_CRITICAL
#endif

#ifdef BLDCFG_DIMM_SENSOR_RESOLUTION
  #undef APCB_TOKEN_UID_DIMMSENSORRESOLUTION_VALUE
  #define APCB_TOKEN_UID_DIMMSENSORRESOLUTION_VALUE                              BLDCFG_DIMM_SENSOR_RESOLUTION
#endif

#ifdef BLDCFG_AUTO_REF_FINE_GRAN_MODE
  #undef APCB_TOKEN_UID_AUTOREFFINEGRANMODE_VALUE
  #define APCB_TOKEN_UID_AUTOREFFINEGRANMODE_VALUE                               BLDCFG_AUTO_REF_FINE_GRAN_MODE
#endif

#ifdef BLDCFG_ENABLE_MEMPSTATE
  #undef APCB_TOKEN_UID_ENABLEMEMPSTATE_VALUE
  #define APCB_TOKEN_UID_ENABLEMEMPSTATE_VALUE                                   BLDCFG_ENABLE_MEMPSTATE
#endif

#ifdef BLDCFG_ENABLE_DDRROUTEBALANCEDTEE
  #undef APCB_TOKEN_UID_DDRROUTEBALANCEDTEE_VALUE
  #define APCB_TOKEN_UID_DDRROUTEBALANCEDTEE_VALUE                               BLDCFG_ENABLE_DDRROUTEBALANCEDTEE
#endif

#ifdef BLDCFG_MEM_NVDIMM_POWER_SOURCE
  #undef APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE_VALUE
  #define APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE_VALUE                           BLDCFG_MEM_NVDIMM_POWER_SOURCE
#endif

#ifdef BLDCFG_MEM_DATA_POISON
  #undef APCB_TOKEN_UID_MEM_DATA_POISON_VALUE
  #define APCB_TOKEN_UID_MEM_DATA_POISON_VALUE                                   BLDCFG_MEM_DATA_POISON
#endif

#ifdef BLDCFG_MEM_DATA_SCRAMBLE
  #undef APCB_TOKEN_UID_MEM_DATA_SCRAMBLE_VALUE
  #define APCB_TOKEN_UID_MEM_DATA_SCRAMBLE_VALUE                                 BLDCFG_MEM_DATA_SCRAMBLE
#endif

#ifdef BLDCFG_MEM_MBIST_TEST_ENABLE
  #undef APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE_VALUE                             BLDCFG_MEM_MBIST_TEST_ENABLE
#endif

#ifdef BLDCFG_MEM_MBIST_TESTMODE
  #undef APCB_TOKEN_UID_MEM_MBIST_TESTMODE_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TESTMODE_VALUE                                BLDCFG_MEM_MBIST_TESTMODE
#endif

#ifdef BLDCFG_MEM_MBIST_AGGRESOR_ON
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON_VALUE                             BLDCFG_MEM_MBIST_AGGRESOR_ON
#endif

#ifdef BLDCFG_MEM_MBIST_HALT_ON_ERROR
  #undef APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR_VALUE                           BLDCFG_MEM_MBIST_HALT_ON_ERROR
#endif

#ifdef BLDCFG_MEM_MBIST_CPU_VREF_RANGE
  #undef APCB_TOKEN_UID_MEM_CPU_VREF_RANGE_VALUE
  #define APCB_TOKEN_UID_MEM_CPU_VREF_RANGE_VALUE                                BLDCFG_MEM_MBIST_CPU_VREF_RANGE
#endif

#ifdef BLDCFG_MEM_MBIST_DRAM_VREF_RANGE
  #undef APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE_VALUE
  #define APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE_VALUE                               BLDCFG_MEM_MBIST_DRAM_VREF_RANGE
#endif

#ifdef BLDCFG_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
  #undef APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_VALUE                BLDCFG_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
#endif

#ifdef BLDCFG_MEM_MBIST_AGGRESSORS
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_VALUE                         BLDCFG_MEM_MBIST_AGGRESSORS
#endif

#ifdef BLDCFG_MEM_MBIST_PATTERN_LENGTH
  #undef APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_VALUE                          BLDCFG_MEM_MBIST_PATTERN_LENGTH
#endif

#ifdef BLDCFG_MEM_MBIST_PATTERN_SELECT
  #undef APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_VALUE                          BLDCFG_MEM_MBIST_PATTERN_SELECT
#endif

#ifdef BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_CTRL
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL_VALUE                   BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_CTRL
#endif

#ifdef BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32_VALUE                BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32
#endif

#ifdef BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32_VALUE                BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32
#endif

#ifdef BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC_VALUE                BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC
#endif

#ifdef BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_VAL
  #undef APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL_VALUE                    BLDCFG_MEM_MBIST_AGGR_STATIC_LANE_VAL
#endif

#ifdef BLDCFG_MEM_MBIST_TGT_STATIC_LANE_CTRL
  #undef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL_VALUE                    BLDCFG_MEM_MBIST_TGT_STATIC_LANE_CTRL
#endif

#ifdef BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_U32
  #undef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_U32_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_U32_VALUE                 BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_U32
#endif

#ifdef BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_L32
  #undef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_L32_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_L32_VALUE                 BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_L32
#endif

#ifdef BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC
  #undef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC_VALUE                 BLDCFG_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC
#endif

#ifdef BLDCFG_MEM_MBIST_TGT_STATIC_LANE_VAL
  #undef APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL_VALUE                     BLDCFG_MEM_MBIST_TGT_STATIC_LANE_VAL
#endif

#ifdef BLDCFG_MEM_MBIST_DATA_EYE_TYPE
  #undef APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE_VALUE                           BLDCFG_MEM_MBIST_DATA_EYE_TYPE
#endif

#ifdef BLDCFG_MEM_MBIST_WORST_CASE_GRAN
  #undef APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN_VALUE                         BLDCFG_MEM_MBIST_WORST_CASE_GRAN
#endif

#ifdef BLDCFG_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP
  #undef APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALUE              BLDCFG_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP
#endif

#ifdef BLDCFG_MEM_MBIST_READ_DATA_EYE_TIMING_STEP
  #undef APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALUE               BLDCFG_MEM_MBIST_READ_DATA_EYE_TIMING_STEP
#endif

#ifdef BLDCFG_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP
  #undef APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALUE             BLDCFG_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP
#endif

#ifdef BLDCFG_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP
  #undef APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALUE
  #define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALUE              BLDCFG_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP
#endif

#ifdef BLDCFG_MEM_TSME_ENABLE
  #undef APCB_TOKEN_UID_MEM_TSME_ENABLE_VALUE
  #define APCB_TOKEN_UID_MEM_TSME_ENABLE_VALUE                                   BLDCFG_MEM_TSME_ENABLE
#endif

#ifdef BLDCFG_CCX_MIN_SEV_ASID
  #undef APCB_TOKEN_UID_CCX_MIN_SEV_ASID_VALUE
  #define APCB_TOKEN_UID_CCX_MIN_SEV_ASID_VALUE                                  BLDCFG_CCX_MIN_SEV_ASID
#endif

#ifdef BLDCFG_CCX_SEV_ASID_COUNT
  #undef APCB_TOKEN_UID_CCX_SEV_ASID_COUNT_VALUE
  #define APCB_TOKEN_UID_CCX_SEV_ASID_COUNT_VALUE                                BLDCFG_CCX_SEV_ASID_COUNT
#endif

#ifdef BLDCFG_CCX_XTRIG7_WORKAROUND
  #undef APCB_TOKEN_UID_XTRIG7_WORKAROUND_VALUE
  #define APCB_TOKEN_UID_XTRIG7_WORKAROUND_VALUE                                 BLDCFG_CCX_XTRIG7_WORKAROUND
#endif

#ifdef BLDCFG_PSP_ENABLE_DEBUG_MODE
  #undef APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE_VALUE
  #define APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE_VALUE                             BLDCFG_PSP_ENABLE_DEBUG_MODE
#endif

#ifdef BLDCFG_PSP_SYSHUB_WDT_INTERVAL
  #undef APCB_TOKEN_UID_PSP_SYSHUB_WDT_INTERVAL_VALUE
  #define APCB_TOKEN_UID_PSP_SYSHUB_WDT_INTERVAL_VALUE                           BLDCFG_PSP_SYSHUB_WDT_INTERVAL
#endif

#ifdef BLDCFG_DF_GMI_ENCRYPT
  #undef APCB_TOKEN_UID_DF_GMI_ENCRYPT_VALUE
  #define APCB_TOKEN_UID_DF_GMI_ENCRYPT_VALUE                                    BLDCFG_DF_GMI_ENCRYPT
#endif

#ifdef BLDCFG_DF_XGMI_ENCRYPT
  #undef APCB_TOKEN_UID_DF_XGMI_ENCRYPT_VALUE
  #define APCB_TOKEN_UID_DF_XGMI_ENCRYPT_VALUE                                   BLDCFG_DF_XGMI_ENCRYPT
#endif

#ifdef BLDCFG_DF_SAVE_RESTORE_MEM_ENCRYPT
  #undef APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT_VALUE
  #define APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT_VALUE                       BLDCFG_DF_SAVE_RESTORE_MEM_ENCRYPT
#endif

#ifdef BLDCFG_DF_SYS_STORAGE_AT_TOM
  #undef APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM_VALUE
  #define APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM_VALUE                      BLDCFG_DF_SYS_STORAGE_AT_TOM
#endif

#ifdef BLDCFG_DF_PROBE_FILTER_ENABLE
  #undef APCB_TOKEN_UID_DF_PROBE_FILTER_VALUE
  #define APCB_TOKEN_UID_DF_PROBE_FILTER_VALUE                                   BLDCFG_DF_PROBE_FILTER_ENABLE
#endif

#ifdef BLDCFG_DF_BOTTOM_IO
  #undef APCB_TOKEN_UID_DF_BOTTOMIO_VALUE
  #define APCB_TOKEN_UID_DF_BOTTOMIO_VALUE                                       BLDCFG_DF_BOTTOM_IO
#endif

#ifdef BLDCFG_DF_MEM_INTERLEAVING
  #undef APCB_TOKEN_UID_DF_MEM_INTERLEAVING_VALUE
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_VALUE                               BLDCFG_DF_MEM_INTERLEAVING
#endif

#ifdef BLDCFG_DF_DRAM_INTLV_SIZE
  #undef APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE_VALUE
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE_VALUE                          BLDCFG_DF_DRAM_INTLV_SIZE
#endif

#ifdef BLDCFG_DF_CHAN_INTLV_HASH_EN
  #undef APCB_TOKEN_UID_DF_CHAN_INTLV_HASH_VALUE
  #define APCB_TOKEN_UID_DF_CHAN_INTLV_HASH_VALUE                                BLDCFG_DF_CHAN_INTLV_HASH_EN
#endif

#ifdef BLDCFG_DF_PCI_MMIO_SIZE
  #undef APCB_TOKEN_UID_DF_PCI_MMIO_SIZE_VALUE
  #define APCB_TOKEN_UID_DF_PCI_MMIO_SIZE_VALUE                                  BLDCFG_DF_PCI_MMIO_SIZE
#endif

#ifdef BLDCFG_DF_CAKE_CRC_THRESH_PERF_BOUNDS
  #undef APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS_VALUE
  #define APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS_VALUE                    BLDCFG_DF_CAKE_CRC_THRESH_PERF_BOUNDS
#endif

#ifdef BLDCFG_DF_MEM_CLEAR
  #undef APCB_TOKEN_UID_DF_MEM_CLEAR_VALUE
  #define APCB_TOKEN_UID_DF_MEM_CLEAR_VALUE                                      BLDCFG_DF_MEM_CLEAR
#endif

#ifdef BLDCFG_DF3_XGMI2_LINK_CFG
  #undef APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG_VALUE
  #define APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG_VALUE                                BLDCFG_DF3_XGMI2_LINK_CFG
#endif

#ifdef BLDCFG_DF_DRAM_NPS
  #undef APCB_TOKEN_UID_DF_DRAM_NPS_VALUE
  #define APCB_TOKEN_UID_DF_DRAM_NPS_VALUE                                       BLDCFG_DF_DRAM_NPS
#endif

#ifdef BLDCFG_DF_INVERT_DRAM_MAP
  #undef APCB_TOKEN_UID_DF_INVERT_DRAM_MAP_VALUE
  #define APCB_TOKEN_UID_DF_INVERT_DRAM_MAP_VALUE                                BLDCFG_DF_INVERT_DRAM_MAP
#endif

#ifdef BLDCFG_DF_4LINK_MAX_XGMI_SPEED
  #undef APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED_VALUE
  #define APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED_VALUE                           BLDCFG_DF_4LINK_MAX_XGMI_SPEED
#endif

#ifdef BLDCFG_DF_3LINK_MAX_XGMI_SPEED
  #undef APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED_VALUE
  #define APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED_VALUE                           BLDCFG_DF_3LINK_MAX_XGMI_SPEED
#endif

#ifdef BLDCFG_DF_REMAP_AT_1TB
  #undef APCB_TOKEN_UID_DF_REMAP_AT_1TB_VALUE
  #define APCB_TOKEN_UID_DF_REMAP_AT_1TB_VALUE                                   BLDCFG_DF_REMAP_AT_1TB
#endif

#ifdef BLDCFG_BMC_SOCKET_INFO
  #undef APCB_TOKEN_UID_BMC_SOCKET_NUMBER_VALUE
  #define APCB_TOKEN_UID_BMC_SOCKET_NUMBER_VALUE                                 BLDCFG_BMC_SOCKET_INFO
#endif

#ifdef BLDCFG_BMC_START_LANE
  #undef APCB_TOKEN_UID_BMC_START_LANE_VALUE
  #define APCB_TOKEN_UID_BMC_START_LANE_VALUE                                    BLDCFG_BMC_START_LANE
#endif

#ifdef BLDCFG_BMC_END_LANE
  #undef APCB_TOKEN_UID_BMC_END_LANE_VALUE
  #define APCB_TOKEN_UID_BMC_END_LANE_VALUE                                      BLDCFG_BMC_END_LANE
#endif

#ifdef BLDCFG_SECOND_PCIE_LINK_START_LANE
  #undef APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE_VALUE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE_VALUE                       BLDCFG_SECOND_PCIE_LINK_START_LANE
#endif

#ifdef BLDCFG_SECOND_PCIE_LINK_END_LANE
  #undef APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE_VALUE
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE_VALUE                         BLDCFG_SECOND_PCIE_LINK_END_LANE
#endif

#ifdef BLDCFG_BMC_BMC_DEVICE
  #undef APCB_TOKEN_UID_BMC_DEVICE_VALUE
  #define APCB_TOKEN_UID_BMC_DEVICE_VALUE                                        BLDCFG_BMC_BMC_DEVICE
#endif

#ifdef BLDCFG_BMC_FUNCTION
  #undef APCB_TOKEN_UID_BMC_FUNCTION_VALUE
  #define APCB_TOKEN_UID_BMC_FUNCTION_VALUE                                      BLDCFG_BMC_FUNCTION
#endif

#ifdef BLDCFG_PCIE_RESET_CONTROL
  #undef APCB_TOKEN_UID_PCIE_RESET_CONTROL_VALUE
  #define APCB_TOKEN_UID_PCIE_RESET_CONTROL_VALUE                                BLDCFG_PCIE_RESET_CONTROL
#endif

#ifdef BLDCFG_FCH_CONSOLE_OUT_ENABLE
  #undef APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE_VALUE
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE_VALUE                            BLDCFG_FCH_CONSOLE_OUT_ENABLE
#endif

#ifdef BLDCFG_FCH_CONSOLE_OUT_SERIAL_PORT
  #undef APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_VALUE
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_VALUE                       BLDCFG_FCH_CONSOLE_OUT_SERIAL_PORT
#endif

#ifdef BLDCFG_FCH_CONSOLE_OUT_BASIC_ENABLE
  #undef APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE_VALUE
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE_VALUE                      BLDCFG_FCH_CONSOLE_OUT_BASIC_ENABLE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_CONTROL_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_CONTROL_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_CONTROL_VALUE                          BLDCFG_MPM_WLAN_POWEREN_CONTROL_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_CMDSTSPORT_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_CMDSTSPORT_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_CMDSTSPORT_VALUE                       BLDCFG_MPM_WLAN_POWEREN_CMDSTSPORT_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_DATAPORT_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATAPORT_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATAPORT_VALUE                         BLDCFG_MPM_WLAN_POWEREN_DATAPORT_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_OFFSET_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_OFFSET_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_OFFSET_VALUE                           BLDCFG_MPM_WLAN_POWEREN_OFFSET_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_MASK_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_MASK_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_MASK_VALUE                             BLDCFG_MPM_WLAN_POWEREN_MASK_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_DATA_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATA_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATA_VALUE                             BLDCFG_MPM_WLAN_POWEREN_DATA_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_GPIOPIN_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIOPIN_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIOPIN_VALUE                          BLDCFG_MPM_WLAN_POWEREN_GPIOPIN_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_GPIODATA_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIODATA_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIODATA_VALUE                         BLDCFG_MPM_WLAN_POWEREN_GPIODATA_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_IOMUXEN_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXEN_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXEN_VALUE                          BLDCFG_MPM_WLAN_POWEREN_IOMUXEN_VALUE
#endif

#ifdef BLDCFG_MPM_WLAN_POWEREN_IOMUXDATA_VALUE
  #undef APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXDATA_VALUE
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXDATA_VALUE                        BLDCFG_MPM_WLAN_POWEREN_IOMUXDATA_VALUE
#endif

#endif   //ifndef  _APCB_DEFAULTS_H_
