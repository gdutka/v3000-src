/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _APCB_V3_TOKEN_UID_H_
#define _APCB_V3_TOKEN_UID_H_
//
// @APCB_START
//

//
// @APCB_TOKEN
//

// =================================================================================================
// CBS CMN APCB Tokens (BOOLEAN)
// -------------------------------------------------------------------------------------------------
/** BOOLEAN_TRUE_FALSE_VALIDVAL
*   Value range for boolean type with True and False usage
*/
typedef enum {
  _FALSE = 0,      ///< False
  _TRUE = 1,       ///< True
} BOOLEAN_TRUE_FALSE_VALIDVAL;

/** BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
*   Value range for boolean type with Enable and Disable usage
*/
typedef enum {
  BOOLEAN_ENABLE_1_DISABLE_0_DISABLE = 0,      ///< Disable
  BOOLEAN_ENABLE_1_DISABLE_0_ENABLE = 1,       ///< Enable
} BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL;

/** BOOLEAN_ENABLE_0_DISABLE_1_VALIDVAL
*   Value range for boolean type with Enable and Disable usage
*/
typedef enum {
  BOOLEAN_ENABLE_0_DISABLE_1_DISABLE = 1,      ///< Disable
  BOOLEAN_ENABLE_0_DISABLE_1_ENABLE = 0,       ///< Enable
} BOOLEAN_ENABLE_0_DISABLE_1_VALIDVAL;

/** BOOLEAN_DISABLE_0_AUTO_VALIDVAL
*   Value range for boolean type with Disable and Auto usage
*/
typedef enum {
  BOOLEAN_DISABLE_0_AUTO_VALIDVAL_DISABLED = 0,///<Disabled
  BOOLEAN_DISABLE_0_AUTO_VALIDVAL_AUTO = 0xFF, ///<Auto
} BOOLEAN_DISABLE_0_AUTO_VALIDVAL;

/** BOOLEAN_ENABLE_1_AUTO_VALIDVAL
*   Value range for boolean type with Enable and Auto usage
*/
typedef enum {
  BOOLEAN_ENABLE_1_AUTO_VALIDVAL_DISABLED = 1,///<Enabled
  BOOLEAN_ENABLE_1_AUTO_VALIDVAL_AUTO = 0xFF, ///<Auto
} BOOLEAN_ENABLE_1_AUTO_VALIDVAL;

// -------------------------------------------------------------------------------------------------
/// @GROUPID: APCB_GROUP_TOKEN
/// @TYPEID: APCB_TYPE_TOKEN_BOOLEAN
/// @DATATYPE: BOOLEAN
/// @SIZE: 1

/// @brief PSP TP Port control
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_PSP_TP_PORT                                        0x0460ABE8  // bool

/// @brief PSP_ERROR_DISPLAY
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_PSP_ERROR_DISPLAY                                  0xDC33FF21  // bool

/// @brief PSP_EVENT_LOG_DISPLAY
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_PSP_EVENT_LOG_DISPLAY                              0x0C473E1C  // bool

/// @brief PSP_STOP_ON_ERROR
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_PSP_STOP_ON_ERROR                                  0xE7024A21  // bool

/// @brief MEMHOLEREMAPPING
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMHOLEREMAPPING                                   0x6A133AC5  // bool

/// @brief LIMITMEMORYTOBELOW1TB
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_LIMITMEMORYTOBELOW1TB                              0x5E71E6D8  // bool

/// @brief ENABLECHIPSELECTINTLV
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLECHIPSELECTINTLV                              0x6F81A115  // bool

/// @brief ENABLECHANNELINTLV
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLECHANNELINTLV                                 0x48254F73  // bool

/// @brief ENABLEECCFEATURE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEECCFEATURE                                   0xFA35F040  // bool

/// @brief ENABLEPOWERDOWN
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEPOWERDOWN                                    0xBBB185A2  // bool

/// @brief ENABLEPARITY
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEPARITY                                       0x3CB8CBD2  // bool

/// @brief ENABLEBANKSWIZZLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEBANKSWIZZLE                                  0x6414D160  // bool

/// @brief ENABLEMEMCLR
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEMEMCLR                                       0xC6ACDB37  // bool

/// @brief MEMRESTORECTL
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMRESTORECTL                                      0xFEDB01F8  // bool

/// @brief ISCAPSULEMODE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ISCAPSULEMODE                                      0x96176308  // bool

/// @brief AMPENABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_AMPENABLE                                          0x592CB3CA  // bool

/// @brief DRAMDOUBLEREFRESHRATE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATE                              0x44D40026  // bool

/// @brief ECCREDIRECTION
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ECCREDIRECTION                                     0xDEDE0E09  // bool

/// @brief ECCSYNCFLOOD
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ECCSYNCFLOOD                                       0x88BD40C2  // bool

/// @brief DQSTRAININGCONTROL
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DQSTRAININGCONTROL                                 0x3CAAA3FA  // bool

/// @brief UMAABOVE4G
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_UMAABOVE4G                                         0x77E41D2A  // bool

/// @brief MEMORYALLCLOCKSON
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYALLCLOCKSON                                  0xB95E0555  // bool

/// @brief IGNORESPDCHECKSUM
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_IGNORESPDCHECKSUM                                  0x7D369DBC  // bool

/// @brief MEMORYMODEUNGANGED
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYMODEUNGANGED                                 0x03CE1180  // bool

/// @brief MEMORYQUADRANKCAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYQUADRANKCAPABLE                              0xE6DFD3DC  // bool

/// @brief MEMORYRDIMMCAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYRDIMMCAPABLE                                 0x81726666  // bool

/// @brief MEMORYLRDIMMCAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYLRDIMMCAPABLE                                0x014FBF20  // bool

/// @brief MEMORYUDIMMCAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYUDIMMCAPABLE                                 0x3CF8A8EC  // bool

/// @brief MEMORYSODIMMCAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEMORYSODIMMCAPABLE                                0x7C61C187  // bool

/// @brief DIMMTYPEDDDR3CAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DIMMTYPEDDDR3CAPABLE                               0x0789210C  // bool

/// @brief DIMMTYPELPDDDR3CAPABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DIMMTYPELPDDDR3CAPABLE                             0xAD96AA30  // bool

/// @brief ENABLEBANKGROUPSWAP
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEBANKGROUPSWAP                                0x46920968  // bool

/// @brief ODTSCMDTHROTEN
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ODTSCMDTHROTEN                                     0xC0736395  // bool

/// @brief SWCMDTHROTEN
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_SWCMDTHROTEN                                       0xA29C1CF9  // bool

/// @brief FORCEPWRDOWNTHROTEN
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN                                0x10849D6C  // bool

/// @brief ENABLEMEMPSTATE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEMEMPSTATE                                    0x56B93947  // bool

/// @brief ENABLEMEMPSTATE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEMEMPSTATE_DDR                                0x572B5FC7  // bool

/// @brief ENABLEMEMPSTATE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEMEMPSTATE_LPDDR                              0x4360B03A  // bool

/// @brief DDRROUTEBALANCEDTEE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DDRROUTEBALANCEDTEE                                0xE68C363D  // bool

/// @brief MEM_TEMP_CONTROLLED_REFRESH_EN
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_REFRESH_EN                     0xF051E1C4  // bool

/// @brief ENABLEBANKGROUPSWAPALT
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLEBANKGROUPSWAPALT                             0xA89D1BE8  // bool

/// @brief UECC_RETRY_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_UECC_RETRY_DDR4                                    0xBFF00125  // bool

/// @brief PCIE_RESET_CONTROL
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_PCIE_RESET_CONTROL                                 0xF7BB3451  // bool, data in x86, SSP code doesn't use. //Obsolete.

/// @brief MEM_MBIST_AGGR_STATIC_LANE_CTRL
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL                    0x77E6F2C9  // bool

/// @brief DRAMDOUBLEREFRESHRATEEN
/// @details Type:BOOLEAN
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATEEN                            0x974E8E7C  // unused

/// @brief CBS_CMN_MEM_CTRLLER_DRAM_ECC_EN_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_EN_DDR4               0x9C6D6993  // bool

/// @brief POST_PACKAGE_REPAIR_ENABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_POST_PACKAGE_REPAIR_ENABLE                         0xCDC03E4E  // bool

/// @brief MEM_MBIST_TGT_STATIC_LANE_CTRL
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL                     0xE1CC135E  // bool

/// @brief MEM_TEMP_CONTROLLED_EXTENDED_REFRESH_EN
/// @details Type:BOOLEAN
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_EXTENDED_REFRESH_EN            0xF402F423  // Obsolete.

/// @brief CBS_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_DISABLE_0_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR4           0x77A2B63A  // bool in code, uint8 in x86 data //bool

/// @brief CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR4           0x229EDFE4  // bool in code, uint8 in x86 data //bool

/// @brief CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR4                 0x0EF08D04  // bool in code, uint8 in x86 data //bool

/// @brief CBS_CMN_MEM_ADDRESS_HASH_CS_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_CS_DDR4                   0xA01AB8A0  // bool in code, uint8 in x86 data //bool

/// @brief CBS_CMN_MEM_SPD_READ_OPTIMIZATION_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_SPD_READ_OPTIMIZATION_DDR4             0x8D3AB10E  // bool in code, uint8 in x86 data //bool

/// @brief CBS_CMN_MEM_MBIST_EN
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_EN                               0x0D0AE86F  // uint8 //bool

/// @brief CBS_CMN_MEM_UECC_RETRY_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_UECC_RETRY_DDR4                        0x42216E4B  // Duplicate with APCB_TOKEN_UID_UECC_RETRY_DDR4 //bool

/// @brief CBS_CMN_MEM_DATA_POISONING_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_POISONING_DDR4                    0x598E8661  // uint8 in x86 data, not used in code //bool

/// @brief CBS_CMN_MEM_OVERCLOCK_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_DDR4                         0x593292BE  // uint8 in x86 data, parent control in code //bool

/// @brief CBS_CMN_MEM_ADDRESS_HASH_RM_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_RM_DDR4                   0x50A6D6CA  // bool

/// @brief CBS_CMN_MEM_CTRLLER_DATA_SCRAMBLE_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DATA_SCRAMBLE_DDR4             0x6301BD18  // bool

/// @brief CBS_CMN_MEM_GEAR_DOWN_MODE_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_GEAR_DOWN_MODE_DDR4                    0x68444D72  // bool

/// @brief CBS_CMN_MEM_MBIST_AGGRESSORS
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_AGGRESSORS                       0xD98B8639  // bool

/// @brief CBS_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT         0xF4943BAD  // bool

/// @brief CBS_CMN_MEM_CTRLLER_PWR_DN_EN_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PWR_DN_EN_DDR4                 0x2D3BB9D8  // bool

/// @brief CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_ALT_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_ALT_DDR4       0xAE6507DA  // bool

/// @brief DF_PROBE_FILTER_ENABLE
/// @details Type:BOOLEAN
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_DF_PROBE_FILTER                                    0x6597C573  // uint8 //uint8 //ToDo: Auto:3

/// @brief PSP_ENABLE_DEBUG_MODE
/// @details Type:BOOLEAN
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE                              0xD1091CD0  // uint8 //uint8 //ToDo: Auto:3

/// @brief MEM_CTRLLER_WR_CRC_EN_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_WR_CRC_EN_DDR4                 0x94451A4B  // bool

/// @brief MEM_DIS_MEM_ERR_INJ
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DIS_MEM_ERR_INJ                        0xFF756025  // bool

/// @brief MEM_RCD_PARITY_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_RCD_PARITY_DDR4                        0x7E23FE78  // bool

/// @brief MEM_WRITE_CRC_RETRY_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_WRITE_CRC_RETRY_DDR4                   0x25FB6EA6  // bool

/// @brief MEM_EARLY_PAGE_ACTIVATE_DDR4
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_CBS_DBG_MEM_EARLY_PAGE_ACTIVATE_DDR4               0x3D1FB3FF  // bool, debug items

/// @brief CCX PPIN opt-in
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_CCX_PPIN_OPT_IN                                    0x6A6700FD  // bool

/// @brief Group D platform selection
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_GROUP_D_PLATFORM                                   0x68318493  // bool

/// @brief Force Data Mask Disable
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_FORCE_DATA_MASK_DISABLE_DDR4                   0xD68482B3  //bool

/// @brief DisRefCmdThrotCnt
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DISREFCMDTHROTCNT                                  0x8F84DCB4

/// @brief OdtsEn
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ODTSEN                                             0xAEB3F914

/// @brief SPD read optimization
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_SPD_READ_OPTIMIZATION_DDR4                     0x6816F949

/// @brief Display Splash Screen
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ENABLE_VGA_PROGRAM                                 0x6570EACE

/// @brief Forces to enable OC mode which will disable FCLK DPM and Memory Pstates
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_GNB_FCLK_OC_FLAG                                   0x8A109D3F

/// @brief DXIO RefClk ShutDown
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_CBS_DBG_GNB_DXIO_REFCLK_SHUTDOWN                   0xE242F457

/// @brief DXIO PMA Power Gating
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_CBS_DBG_GNB_DXIO_PMA_POWER_GATING                  0xE32565BA

/// @brief DXIO PMA Clock Gating
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_CBS_DBG_GNB_DXIO_PMC_CLOCK_GATING                  0x9B958974

/// @brief DXIO Clock Gating
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_CBS_DBG_GNB_DXIO_CLOCK_GATING                      0x3C9C005F

/// @brief DXIO Static Power Gating
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_CBS_DBG_GNB_DXIO_STATIC_POWER_GATING               0x312A8528

/// @brief Enables GPU Translation Cache
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_GPU_TRANSLATION_CACHE                              0xEEED60BA  //bool

/// @brief MEM VDDIO control
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_OC_VDDIO_CONTROL                               0x6CD36DBE

/// @brief Discard OC condition and enforce to enable MEM VDDIO adjustment
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_OC_VDDIO_CONTROL_ENFORCE_ON                    0x72958E2C

/// @brief DFE Read Training
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_TRAIN_DFE_DDR4                     0x36A4BB5B  //bool

/// @brief FFE Write Training
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_TRAIN_FFE_DDR4                     0x0D46186D  //bool

/// @brief Enable LN2 mode
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_GNB_SMU_LN2_MODE                                   0x359597F5

/// @brief eSPI ABL init
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ESPI_ABL_INIT_ENABLE                               0x87958B5A  // uint8

/// @brief eSPI Port 80H decoding Enable
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ESPI_IO80_ENABLE                                   0x99ACD54C  // uint8

/// @brief eSPI KBC Port 60h/64h decoding Enable
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_ESPI_KBC6064_ENABLE                                0xFFF62D7C  // uint8

/// @brief Disable NVDIMM-N feature
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DISABLE_NVDIMM_N_FEATURE                           0x941A92D4

/// @brief Disable NVDIMM-N feature
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DISABLE_NVDIMM_N_FEATURE               0xB424AFF0

/// @brief Trimming the p-state level to make P0=P1
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_SMU_DPM_LEVEL_TRIMMING                             0xCCFBCC5D

/// @brief Degrade MP3 frequency to 333/333/667
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_SMU_DPM_MP3_DEGRADE                                0xFCF03E05

/// @brief Modern Standby Enable
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MODERN_STANDBY_ENABLE                              0xDB80C84F

/// @brief SubUrgRefLowerBound
/// @details Type:BOOLEAN
/// @li 1~6
  #define APCB_TOKEN_UID_MEM_SUBURGREFLOWERBOUND                            0xE7562AB6

/// @brief UrgRefLimit
/// @details Type:BOOLEAN
/// @li 1~6
  #define APCB_TOKEN_UID_MEM_URGREFLIMIT                                    0x133332DF

/// @brief Control UMC Powergating
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_SMU_UMC_POWERGATING                                0xAD1520BD

/// @brief CBS_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_DDR        0xDA15A5C2

/// @brief CBS_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_DDR        0xF78BCCAF

/// @brief CBS_CMN_MEM_CTRLLER_DRAM_ECC_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_EN_DDR                0xDEC0D8D5

/// @brief CBS_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_DISABLE_0_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR            0xE61E19F4

/// @brief CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR            0x45CDCFCB

/// @brief CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR                  0xD9BBF276

/// @brief CBS_CMN_MEM_ADDRESS_HASH_CS_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_CS_DDR                    0x49C72716

/// @brief CBS_CMN_MEM_SPD_READ_OPTIMIZATION_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_SPD_READ_OPTIMIZATION_DDR              0x99A22CA8

/// @brief CBS_CMN_MEM_UECC_RETRY_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_UECC_RETRY_DDR                         0x67DC532C

/// @brief CBS_CMN_MEM_DATA_POISONING_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_POISONING_DDR                     0xFD484A8C

/// @brief APCB_TOKEN_UID_MEM_DATA_POISONING_DDR
/// @details Type:UINT8
/// @see UINT8_Disable_0_Enable_1_Auto_0xFF
  #define APCB_TOKEN_UID_MEM_DATA_POISONING_DDR                             0x86140D77

/// @brief CBS_CMN_MEM_ADDRESS_HASH_RM_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_RM_DDR                    0x57A7E73E

/// @brief CBS_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR                 0x8D67F9E4

/// @brief CBS_CMN_MEM_GEAR_DOWN_MODE_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_GEAR_DOWN_MODE_DDR                     0x395438F9

/// @brief CBS_CMN_MEM_CTRLLER_PWR_DN_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PWR_DN_EN_DDR                  0x2013102F

/// @brief CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_ALT_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_ALT_DDR        0xADAAFD5D

/// @brief MEM_CTRLLER_WR_CRC_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_WR_CRC_EN_DDR                  0x458A2BE6

/// @brief MEM_RCD_PARITY_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_RCD_PARITY_DDR                         0x354CA7F3

/// @brief MEM_WRITE_CRC_RETRY_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_WRITE_CRC_RETRY_DDR                    0x000D149E

/// @brief MEM_EARLY_PAGE_ACTIVATE_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_CBS_DBG_MEM_EARLY_PAGE_ACTIVATE_DDR                0x4EC767D2

/// @brief Force Data Mask Disable
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_FORCE_DATA_MASK_DISABLE_DDR                    0x16B6952B

/// @brief SPD read optimization
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_SPD_READ_OPTIMIZATION_DDR                      0x3C4374C3

/// @brief DFE Read Training
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_TRAIN_DFE_DDR                      0x909D9E12

/// @brief FFE Write Training
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_TRAIN_FFE_DDR                      0x6A6633A1

/// @brief APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_WCKALWAYSON_EN_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_WCKALWAYSON_EN_DDR        0x2DC869E1

/// @brief DRAM Latency Enhance
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_LATENCY_ENHANCE                           0x1D0A5E46

/// @brief DXIO VGA API ENABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DXIO_VGA_API_ENABLE                                0xBD5AA3C6

/// @brief APCB_TOKEN_UID_MEM_BANK_SWAP_ENABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_BANK_SWAP_ENABLE                               0x4ACA0A6A

/// @brief Wireless Manageability: Enable/Disable
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_WIRELESS_MANAGEABILITY                             0x441D2982  // bool

/// @brief KVM for Wired Manageability
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY                            0x0202CB32  // bool

/// @brief KVM for Wireless Manageability
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY                         0x58E10F2C  // bool

/// @brief If MPM has been provisioned
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MPM_SYSTEM_PROVISIONED                             0x2A52DA0F  // bool

/// @brief GpioIndex of AC Power Plug-in, used for determine the M event
/// @details Type:BOOLEAN
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_AC_GPIO_INDEX                                  0xE5DB8ED5  // uint8

/// @brief GpioIndex of WLAN (DASH) packet arrival event, used for determine the M event
/// @details Type:BOOLEAN
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_GPIO_INDEX                                0x1FB3B92F  // uint8

/// @brief DVFSC Mode Enable
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DVFSC_ENABLE                                   0xDDF6989D

/// @brief DFE Read Training
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_PMU_TRAIN_RXDFE                                0xB8EF0923

/// @brief DFE Write Training
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_PMU_TRAIN_TXDFE                                0x13C068FA

/// @brief Performance Tracing
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_PERFORMANCE_TRACING                                0xF27A10F0  // bool

/// @brief CBS_CCX_CORE_CTRL
/// @details Type:UINT8
/// @li 0~10 = UINT8 Valid Range
  #define APCB_TOKEN_UID_CCX_CORE_CTRL_DUMMY                                0xF5F9A373  //uint8

/// @brief MEM_OC_VDD_CONTROL
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_MEM_OC_VDD_CONTROL                                 0xEC25ADAC

/// @brief ESPI_CRC_ENABLE
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_ESPI_CRC_ENABLE                                    0x46237614  // bool

/// @brief ABL DDR DIMM SPD CRC verify
/// @details This feature controls the DDR DIMM SPD CRC verify or not
/// @details Permitted Choices (Type: List) (Default: 0x1 Enabled)
/// @li 0 = Skip verifing DDR DIMM SPD CRC
/// @li 1 = ABL verify DDR DIMM SPD CRC
  #define APCB_TOKEN_UID_MEM_SPD_VERIFY_CRC                                 0x87B6D2D4

/// @brief ABL DDR DIMM SPD read retry count on CRC failure
/// @details Controls how many SPD read retry while SPD CRC check failure
/// @li 0~8 = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_SPD_READ_RETRY_COUNT_ON_CRC_FAILURE            0x55372EFE

/// @brief MEM_CS_INTERLEAVE_DDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_DISABLE_0_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CS_INTERLEAVE_DDR                              0xA196A838  // bool
      
// =================================================================================================
// CBS CMN APCB Tokens (1BYTE)
// -------------------------------------------------------------------------------------------------
/** ENABLE_DISABLE_AUTO_OPTION
*   Value range for UINT8 type with Enable, Disable and Auto usage
*/
typedef enum {
  ENABLE_DISABLE_AUTO_OPTION_DISABLED = 0,///<Disabled
  ENABLE_DISABLE_AUTO_OPTION_ENABLED = 1,///<Enabled
  ENABLE_DISABLE_AUTO_OPTION_AUTO = 3,///<Auto
} ENABLE_DISABLE_AUTO_OPTION;

/** MANUAL_AUTO_0X00_VALIDVAL
*   Value range for UINT8 type with AUTO: 0x00 and ENABLE: 0x01 usage
*/
typedef enum {
  MANUAL_AUTO_0X00_VALIDVAL_AUTO = 0x00,      ///< AUTO
  MANUAL_AUTO_0X00_VALIDVAL_MANUAL = 1,       ///< MANUAL
} MANUAL_AUTO_0X00_VALIDVAL;

/** MANUAL_AUTO_0XFF_VALIDVAL
*   Value range for UINT8 type with AUTO: 0xFF and ENABLE: 0x01 usage
*/
typedef enum {
  MANUAL_AUTO_0XFF_VALIDVAL_AUTO = 0xFF,      ///< AUTO
  MANUAL_AUTO_0XFF_VALIDVAL_MANUAL = 1,       ///< MANUAL
} MANUAL_AUTO_0XFF_VALIDVAL;

typedef enum {
  WORKLOAD_PROFILE_DISABLED = 0,
  WORKLOAD_PROFILE_CPU_INTENSIVE = 1,
  WORKLOAD_PROFILE_JAVA_THROUGHPUT = 2,
  WORKLOAD_PROFILE_JAVA_LATENCY = 3,
  WORKLOAD_PROFILE_POWER_EFFICIENCY = 4,
  WORKLOAD_PROFILE_MEMORY_THROUGHPUT_INTENSIVE = 5,
  WORKLOAD_PROFILE_STORAGE_IO_INTENSIVE = 6,
  WORKLOAD_PROFILE_NIC_THROUGHPUT_INTENSIVE = 7,
  WORKLOAD_PROFILE_NIC_LATENCY_SENSITIVE = 8,
  WORKLOAD_PROFILE_ACCELERATOR_THROUGHPUT = 9,
  WORKLOAD_PROFILE_VMWARE_VSPHERE_OPTIMIZED = 10,
  WORKLOAD_PROFILE_LINUX_KVM_OPTIMIZED = 11,
  WORKLOAD_PROFILE_CONTAINER_OPTIMIZED = 12,
  WORKLOAD_PROFILE_RDBMS_OPTIMIZED = 13,
  WORKLOAD_PROFILE_BIG_DATA_ANALYTICS_OPTIMIZED = 14,
  WORKLOAD_PROFILE_IOT_GATEWAY = 15,
  WORKLOAD_PROFILE_HPC_OPTIMIZED = 16,
  WORKLOAD_PROFILE_OPENSTACK_NFV = 17,
  WORKLOAD_PROFILE_OPENSTACK_FOR_REALTIME_KERNEL = 18
} WORKLOAD_PROFILE_VALIDVAL;

// -------------------------------------------------------------------------------------------------
/// @GROUPID: APCB_GROUP_TOKEN
/// @TYPEID: APCB_TYPE_TOKEN_1BYTE
/// @DATATYPE: UINT8
/// @SIZE: 1

/// @brief CCX_CPB
/// @details Type:UINT8
/// @li 0x00~0xFF = UINT8 Valid Value
  #define APCB_TOKEN_UID_CCX_CPB                                            0x9DA294C2

/// @brief CBS_CMN_CCX_SMT_CTRL
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_3_VALIDVAL
  #define APCB_TOKEN_UID_CCX_SMT_CTRL                                       0xB0A37D17  //uint8

/// @brief CBS_CCX_CORE_CTRL
/// @details Type:UINT8
/// @li 0~10 = UINT8 Valid Range
  #define APCB_TOKEN_UID_CCX_CORE_CTRL                                      0x8239B491  //uint8

/// @DESC: AOD_CCD_CORE_CTRL
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_3_VALIDVAL
  #define APCB_TOKEN_UID_AOD_CCD_CORE_CTRL                                  0x530BB305  //uint8

/// @DESC: AOD_CCD0_CORE_CTRL
/// @details Type:UINT8
/// @li 0~254 = UINT8 Valid Range
  #define APCB_TOKEN_UID_AOD_CCD0_CORE_CTRL                                 0x3F7F8A94  //uint8

/// @brief CCX_SEV_ASID_COUNT
/// @details Type:UINT8
/// @see CPU_SEV_ASID_COUNT_VALIDVAL
  #define APCB_TOKEN_UID_CCX_SEV_ASID_COUNT                                 0x55876720  //uint8

/// @brief APCB_TOKEN_UID_MEM_ECC_EN_DDR
/// @details Type:UINT8
/// @see ENABLE_1_DISABLE_0_AUTO_0xFF
  #define APCB_TOKEN_UID_MEM_ECC_EN_DDR                                     0x5F1DE498

/** CPU_SEV_ASID_COUNT_VALIDVAL
*   Value range for APCB_TOKEN_UID_CCX_SEV_ASID_COUNT
*/
typedef enum {
  CPU_SEV_ASID_COUNT_253ASIDS = 0,///<253 ASIDs
  CPU_SEV_ASID_COUNT_509ASIDS = 1,///<509 ASIDs
  CPU_SEV_ASID_COUNT_AUTO = 3,///<Auto
} CPU_SEV_ASID_COUNT_VALIDVAL;

/// @brief XTRIG7_WORKAROUND
/// @details Type:UINT8
/// @see CPU_XTRIG7_WORKAROUND_VALIDVAL
  #define APCB_TOKEN_UID_XTRIG7_WORKAROUND                                  0x2762E77B  //uint8
/** CPU_XTRIG7_WORKAROUND_VALIDVAL
*   Value range for APCB_TOKEN_UID_XTRIG7_WORKAROUND
*/
typedef enum {
  CPU_XTRIG7_WORKAROUND_AUTO = 0xFF,///<Auto
  CPU_XTRIG7_WORKAROUND_NOWORKAROUND = 0,///<No Workaround
  CPU_XTRIG7_WORKAROUND_BRONZEWORKAROUND = 1,///<Bronze Workaround
} CPU_XTRIG7_WORKAROUND_VALIDVAL;

/// @brief MEM_MBIST_AGGRESSORS_CHNL
/// @details Type:UINT8
/// @see MEM_MBIST_AGGRESSORS_CHNL_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL                          0xDCD1444A  //uint8
/** MEM_MBIST_AGGRESSORS_CHNL_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL
*/
typedef enum {
  MEM_MBIST_AGGRESSORS_CHNL_DISABLED = 0,///<Disabled
  MEM_MBIST_AGGRESSORS_CHNL_1AGGRESSORCHANNEL = 1,///<1 Aggressor Channel
  MEM_MBIST_AGGRESSORS_CHNL_3AGGRESSORCHANNELS = 3,///<3 Aggressor Channels
  MEM_MBIST_AGGRESSORS_CHNL_7AGGRESSORCHANNELS = 7,///<7 Aggressor Channels
} MEM_MBIST_AGGRESSORS_CHNL_VALIDVAL;

/// @brief MEM_MBIST_PATTERN_LENGTH
/// @details Type:UINT8
/// @li 0~12 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH                           0xAE7BAEDD  //uint8

/// @brief MEM_MBIST_PATTERN_SELECT
/// @details Type:UINT8
/// @see MEM_MBIST_PATTERN_SELECT_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT                           0xF527EBF8  //uint8
/** MEM_MBIST_PATTERN_SELECT_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT
*/
typedef enum {
  MEM_MBIST_PATTERN_SELECT_PRBS = 0,///<PRBS
  MEM_MBIST_PATTERN_SELECT_SSO = 1,///<SSO
  MEM_MBIST_PATTERN_SELECT_BOTH = 2,///<Both
} MEM_MBIST_PATTERN_SELECT_VALIDVAL;

/// @brief MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC
/// @details Type:UINT8
/// @li 0x00~0x0A = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_ECC                 0x57122E99  //uint8

/// @brief MEM_MBIST_AGGR_STATIC_LANE_VAL
/// @details Type:UINT8
/// @li 0x00~0x0A = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_VAL                     0x4474D416  //uint8

/// @brief MEM_MBIST_TGT_STATIC_LANE_SEL_ECC
/// @details Type:UINT8
/// @li 0x00~0x0A = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_ECC                  0xA6E92CEE  //uint8

/// @brief MEM_MBIST_TGT_STATIC_LANE_VAL
/// @details Type:UINT8
/// @li 0x00~0x0A = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_VAL                      0x4D7E0206  //uint8

/// @brief MEM_CTRLLER_DIS_BURST_PPND_REFRESH
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_DIS_BURST_PPND_REFRESH                 0xE0542A4C  //uint8

/// @brief MEM_SUB_URG_REF_LOWER_BOUND
/// @details Type:UINT8
/// @li 0x01~0x06 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_SUB_URG_REF_LOWER_BOUND                        0x51A5C3E7  //uint8

/// @brief MEM_URG_REF_LIMIT
/// @details Type:UINT8
/// @li 0x01~0x06 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_URG_REF_LIMIT                                  0xADA93321  //uint8

/// @brief MEM_MAX_ACTIVATE_COUNT1
/// @details Type:UINT8
/// @see MEM_MAX_ACTIVATE_COUNT1_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MAX_ACTIVATE_COUNT1                            0x57A07B47  //uint8

/** MEM_MAX_ACTIVATE_COUNT1_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MAX_ACTIVATE_COUNT1
*/
typedef enum {
  MEM_MAX_ACTIVATE_COUNT1_AUTO = 0xFF,///<Auto
  MEM_MAX_ACTIVATE_COUNT1_UNTESTEDMAC = 0,///<0
  MEM_MAX_ACTIVATE_COUNT1_700K = 1,///<1
  MEM_MAX_ACTIVATE_COUNT1_600K = 2,///<2
  MEM_MAX_ACTIVATE_COUNT1_500K = 3,///<3
  MEM_MAX_ACTIVATE_COUNT1_400K = 4,///<4
  MEM_MAX_ACTIVATE_COUNT1_300K = 5,///<5
  MEM_MAX_ACTIVATE_COUNT1_200K = 6,///<6
  MEM_MAX_ACTIVATE_COUNT1_UNLIMITEDMAC = 8,///<8
} MEM_MAX_ACTIVATE_COUNT1_VALIDVAL;

/// @brief MEM_MBIST_DATA_EYE_TYPE
/// @details Type:UINT8
/// @see MEM_MBIST_DATA_EYE_TYPE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE                            0x4E2EDC1B  //uint8
/** MEM_MBIST_DATA_EYE_TYPE_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_TYPE
*/
typedef enum {
  MEM_MBIST_DATA_EYE_TYPE_1DVOLTAGESWEEP = 0,///<1D Voltage Sweep
  MEM_MBIST_DATA_EYE_TYPE_1DTIMINGSWEEP = 1,///<1D Timing Sweep
  MEM_MBIST_DATA_EYE_TYPE_2DFULLDATAEYE = 2,///<2D Full Data Eye
  MEM_MBIST_DATA_EYE_TYPE_WORSTCASEMARGINONLY = 3,///<Worst Case Margin Only
} MEM_MBIST_DATA_EYE_TYPE_VALIDVAL;

/// @brief MEM_MBIST_WORST_CASE_GRAN
/// @details Type:UINT8
/// @see MEM_MBIST_WORST_CASE_GRAN_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN                          0x23B0B6A1  //uint8
/** MEM_MBIST_WORST_CASE_GRAN_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_WORST_CASE_GRAN
*/
typedef enum {
  MEM_MBIST_WORST_CASE_GRAN_PERCHIPSELECT = 0,///<Per Chip Select
  MEM_MBIST_WORST_CASE_GRAN_PERNIBBLE = 1,///<Per Nibble
} MEM_MBIST_WORST_CASE_GRAN_VALIDVAL;

/// @brief MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP
/// @details Type:UINT8
/// @see MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP               0x35D6A4F8  //uint8
/** MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP
*/
typedef enum {
  MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_1 = 1,///<1
  MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_2 = 2,///<2
  MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_4 = 4,///<4
} MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_VALIDVAL;

/// @brief MEM_MBIST_READ_DATA_EYE_TIMING_STEP
/// @details Type:UINT8
/// @see MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP                0x58CCD28A  //uint8
/** MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP
*/
typedef enum {
  MEM_MBIST_READ_DATA_EYE_TIMING_STEP_1 = 1,///<1
  MEM_MBIST_READ_DATA_EYE_TIMING_STEP_2 = 2,///<2
  MEM_MBIST_READ_DATA_EYE_TIMING_STEP_4 = 4,///<4
} MEM_MBIST_READ_DATA_EYE_TIMING_STEP_VALIDVAL;

/// @brief MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP
/// @details Type:UINT8
/// @see MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP              0xCDA61022  //uint8
/** MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP
*/
typedef enum {
  MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_1 = 1,///<1
  MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_2 = 2,///<2
  MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_4 = 4,///<4
} MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_VALIDVAL;

/// @brief MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP
/// @details Type:UINT8
/// @see MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP               0xD9025142  //uint8
/** MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP
*/
typedef enum {
  MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_1 = 1,///<1
  MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_2 = 2,///<2
  MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_4 = 4,///<4
} MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_VALIDVAL;

/// @brief MEM_MBIST_DATA_EYE_EXECUTION_REPEAT_COUNT
/// @details Type:UINT8
/// @li 0~10 = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_DATA_EYE_EXECUTION_REPEAT_COUNT          0x8E4BDAD7  //uint8

/// @brief FCH_CONSOLE_OUT_BASIC_ENABLE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_BASIC_ENABLE                       0xA0903F98  //Obsolete.

/// @brief _DF_SYNC_FLOOD_PROP
/// @details Type:UINT8
/// @see DF_SYNC_FLOOD_PROP_VALIDVAL
  #define APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP                                 0x49639134  //uint8

/// @brief _DF_EXT_IP_SYNC_FLOOD_PROP
/// @details Type:UINT8
/// @see DF_EXT_IP_SYNC_FLOOD_PROP_VALIDVAL
  #define APCB_TOKEN_UID_DF_EXT_IP_SYNC_FLOOD_PROP                          0xFFFE0B07  //uint8

/// @brief CBS_DF_CMN_MPx_EXCEPTION_RECOVERY_MODE
/// @details Type:UINT8
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_DF_CMN_MPx_EXCEPTION_RECOVERY_MODE             0x90A34B23

/// @brief CBS_DF_CMN_MPx_EXCEPTION_RECOVERY_MODE
/// @details Type:UINT8
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_DF_CMN_MEM_CLEAR_TIMEOUT                       0x87811DA7

/** DF_SYNC_FLOOD_PROP_VALIDVAL
*   Value range for APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP
*/
typedef enum {
  DF_SYNC_FLOOD_PROP_SYNCFLOODDISABLED = 1,///<Sync flood disabled
  DF_SYNC_FLOOD_PROP_SYNCFLOODENABLED = 0,///<Sync flood enabled
  DF_SYNC_FLOOD_PROP_AUTO = 0xFF,///<Auto
} DF_SYNC_FLOOD_PROP_VALIDVAL;

/// @brief DF_GMI_ENCRYPT
/// @details Type:UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_DF_GMI_ENCRYPT                                     0x08A45920  // uint8 //uint8

/// @brief DF_XGMI_ENCRYPT
/// @details Type:UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_DF_XGMI_ENCRYPT                                    0x6BD32F1C  // uint8 //uint8

/// @brief DF_SAVE_RESTORE_MEM_ENCRYPT
/// @details Type:UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT                        0x7B3D1F75  // uint8 //uint8

/// @brief DF_BOTTOMIO
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_DF_BOTTOMIO                                        0x8FB98529  // uint8

/// @brief AUTOREFFINEGRANMODE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_AUTOREFFINEGRANMODE                                0x190305DF  // uint8 //Obsolete.

/// @brief AUTOREFMODE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_AUTOREFMODE                                        0x5B10198C  // uint8

typedef enum {
  APCB_REF_MODE_AUTO = 0,
  APCB_REF_MODE_ALLBANK,
  APCB_REF_MODE_PERBANK,
  APCB_REF_MODE_MIXED,
} APCB_REF_MODE;

/// @brief CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR
/// @details Type:UINT8
/// @see UINT8_DISABLE_0_SWAPCPU_1_SWAPAPU_2_SWAPALT_3
  #define APCB_TOKEN_UID_MEM_BANK_SWAP_MODE_DDR                             0x7EF11128  //uint8

/// @brief SOLDERDOWNDRAM
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_SOLDERDOWNDRAM                                     0x5B8F861D  // uint8 , not used in SSP //Obsolete.

/// @brief MEM_MBIST_TESTMODE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TESTMODE                                 0x567A1FC0  // uint8 //Obsolete.

/// @brief MEM_MBIST_AGGRESOR_ON
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESOR_ON                              0x032361C4  // uint8 //Obsolete.

/// @brief MEM_CPU_VREF_RANGE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CPU_VREF_RANGE                                 0x7627CB6D  // uint8 //Obsolete.

/// @brief MEM_DRAM_VREF_RANGE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_DRAM_VREF_RANGE                                0xA8769655  // uint8 //Obsolete.

/// @brief MEM_NVDIMM_POWER_SOURCE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE                            0x286D0075  // uint8 //Obsolete.

/// @brief MEM_PS_ERROR_HANDLING
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_PS_ERROR_HANDLING                              0x6C4CCF38  // uint8 //Obsolete.

/// @brief MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT                 0xCFF56411  // uint8 //Obsolete.

/// @brief BMC_INIT_BEFORE_DRAM
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_BMC_INIT_BEFORE_DRAM                               0xFA94EE37  // bool

/// @brief BMC_SOCKET_NUMBER
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BMC_SOCKET_NUMBER                                  0x846573F9  // uint8 //Obsolete.

/// @brief BMC_START_LANE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BMC_START_LANE                                     0xB88D87DF  // uint8 //Obsolete.

/// @brief BMC_END_LANE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BMC_END_LANE                                       0x143F3963  // uint8 //Obsolete.

/// @brief SECOND_PCIE_LINK_START_LANE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_START_LANE                        0xB1648033  // uint8

/// @brief SECOND_PCIE_LINK_END_LANE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_END_LANE                          0x3BF323FD  // uint8

/// @brief BMC_DEVICE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BMC_DEVICE                                         0xD5BC5FC9  // uint8 //Obsolete.

/// @brief BMC_FUNCTION
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BMC_FUNCTION                                       0x1DE4DD61  // uint8 //Obsolete.

/// @brief Select second PCIe Link Gen Speed
/// @details Type:UINT8
/// @see BMC_LINK_SPEED
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_SPEED                             0x8723750F

/// @brief Select second PCIe Link Maximum Payload Size
/// @details Type:UINT8
/// @li MAX_PAYLOAD_SIZE 0x0=128Bytes 0x1=256Bytes 0x2=512Bytes 0x3=1024Bytes 0x4=2048Bytes 0x5=4096Bytes
  #define APCB_TOKEN_UID_SECOND_PCIE_LINK_MAX_PAYLOAD                       0xE02DF04B  // uint8

/// @brief Select BMC Gen Speed
/// @details Type:UINT8
/// @see BMC_LINK_SPEED
  #define APCB_TOKEN_UID_BMC_LINK_SPEED                                     0x9C790F4B  // uint8
/** BMC_LINK_SPEED
*   Value range for APCB_TOKEN_UID_BMC_LINK_SPEED
*/
typedef enum {
  PCIE_GEN1_SPEED = 1,    ///< PCIE Gen1 Speed
  PCIE_GEN2_SPEED = 2,    ///< PCIE Gen2 Speed
} BMC_LINK_SPEED;

/// @brief Select RCB checks received completions for incorrect attributes
/// @details Type:UINT8
/// @see BMC_RCB_WRONG_ATTR_DIS
  #define APCB_TOKEN_UID_BMC_RCB_WRONG_ATTR_DIS                             0xAE7F0DF4  // uint8
/** BMC_RCB_WRONG_ATTR_DIS
*   Value range for APCB_TOKEN_UID_BMC_RCB_WRONG_ATTR_DIS
*/
typedef enum {
  WRONG_ATTR_EN = 0,           ///< Enable RCB checks incorrect attributes
  WRONG_ATTR_DIS = 1,          ///< Disable RCB checks incorrect attributes
  WRONG_ATTR_SKIP = 0xFF,      ///< Skip override setting
} BMC_RCB_WRONG_ATTR_DIS;

/// @brief Select Gen2 Tx DeEmphasis
/// @details Type:UINT8
/// @see BMC_GEN2_TX_DEEMPHASIS
  #define APCB_TOKEN_UID_BMC_GEN2_TX_DEEMPHASIS                             0xF30D142D  // uint8

/// @brief Select Enable BMC Legacy VGA IO Port
/// @details Type:UINT8
/// @li True = Enable, False = Disable
  #define APCB_TOKEN_UID_BMC_LEGACY_IO_ENABLE                               0x468D2CFA  // bool

/// @brief BMC Legacy VGA IO Port Number
/// @details Type:UINT8
/// @li 0~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_BMC_LEGACY_IO_PORT                                 0x06E06198  // uint16

/// @brief BMC Legacy VGA IO Port Range
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BMC_LEGACY_IO_PORT_SIZE                            0xFC3F2520  // uint8

/// @brief Replace Re-locatable I/O Space BAR address by Legacy VGA IO Port
/// @details Type:UINT8
/// @li 0~0x6 = Select BAR0 ~ BAR6
  #define APCB_TOKEN_UID_BMC_LEGACY_IO_BAR_REPLACE                          0x2C81A37F  // uint8

/// @brief Select Enable Second PCIe link
/// @details Type:UINT8
/// @li True = Enable, False = Disable
  #define APCB_TOKEN_UID_CONFIGURE_SECOND_PCIE_LINK                         0x71428092  // bool

/// @brief FCH_CONSOLE_OUT_ENABLE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE                             0xDDB759DA  // uint8

  /// @brief CONSOLE_OUT_BOOT_MODE
  /// @li 0~0xFF = UITN8 Valid Range
  /// BIT[0]  - Normal boot
  /// BIT[1]  - S3/S0i3
  #define APCB_TOKEN_UID_CONSOLE_OUT_BOOT_MODE                              0x4B993A83  // uint8

/// @brief FCH_CONSOLE_OUT_SERIAL_PORT
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT                        0xFFF9F34D  // uint8 //Obsolete.

/// @brief Select Legacy Uart (SIO or eSPI) IO base
/// @details Type:UINT8
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_IO                     0x95DC6839  // uint8
/** FCH_CONSOLE_OUT_SERIAL_PORT_IO
*   Value range for APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_IO
*/
typedef enum {
  FCH_CONSOLE_OUT_SERIAL_PORT_IO_3F8 = 0,///<0x3F8
  FCH_CONSOLE_OUT_SERIAL_PORT_IO_2F8 = 1,///<0x2F8
  FCH_CONSOLE_OUT_SERIAL_PORT_IO_3E8 = 2,///<0x3E8
  FCH_CONSOLE_OUT_SERIAL_PORT_IO_2E8 = 3,///<0x2E8
} FCH_CONSOLE_OUT_SERIAL_PORT_IO;

/// @brief ABL Serial Baud Rate
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ABL_SERIAL_BAUD_RATE                               0xAE46CEA4  // uint8

/// @brief Data Fabric Memory Interleaving Control
/// @details Type:UINT8
/// @see DF_MEM_INTERLEAVING_VALIDVAL
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING                                0xCE0176EF  // uint8 //uint8
/** DF_MEM_INTERLEAVING_VALIDVAL
*  Value range for APCB_TOKEN_UID_DF_MEM_INTERLEAVING
*/
typedef enum {
  DF_MEM_INTLV_NONE     = 0,      ///< No interleave
  DF_MEM_INTLV_CHANNEL  = 1,      ///< Channel interleave
  DF_MEM_INTLV_DIE      = 2,      ///< Die interleave
  DF_MEM_INTLV_SOCKET   = 3,      ///< Socket interleave
  DF_MEM_INTLV_AUTO     = 7,      ///< Auto interleave
} DF_MEM_INTERLEAVING_VALIDVAL;
//#define macro(
//#define conts (A * B *C) ///< @VALUE:3

/// @brief MEM_DATA_POISON
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DATA_POISON                                    0x48959473  // bool, Obsolete.

/// @brief MEM_DATA_SCRAMBLE
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DATA_SCRAMBLE                                  0x98ACA5B4  // bool, Obsolete.

/// @brief MEM_MBIST_HALT_ON_ERROR
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_HALT_ON_ERROR                            0xB1940F25  // bool, Obsolete.

/// @brief MEM_MBIST_TEST_ENABLE
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_TEST_ENABLE                              0xDF5502C8  // bool, Obsolete.

/// @brief MEM_HEAL_BIST_EN
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_HEAL_BIST_ENABLE                               0xFBA23A28  // uint8 //bool

/// @brief MEM_HEAL_TEST_SELECT
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_HEAL_TEST_SELECT                               0x59082CF2  // uint8 //bool

/// @brief MEM_HEAL_PPR_TYPE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_HEAL_PPR_TYPE                                  0x54181A61  // uint8 //bool
//
/// @brief Data Fabric DRAM Interleave Size
/// @details Type:UINT8
/// @see DF_MEM_INTLV_SIZE_VALIDVAL
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE                           0x2606C42E  //uint8
/** DF_MEM_INTLV_SIZE_VALIDVAL
*  Value range for APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE
*/
typedef enum {
  DF_MEM_INTLV_SIZE_256BYTES = 0,  ///< Interleave size 256 bytes
  DF_MEM_INTLV_SIZE_512BYTES = 1,  ///< Interleave size 512 bytes
  DF_MEM_INTLV_SIZE_1KB = 2,       ///< Interleave size 1024 bytes
  DF_MEM_INTLV_SIZE_2KB = 3,       ///< Interleave size 2048 bytes
  DF_MEM_INTLV_SIZE_4KB = 4,       ///< Interleave size 4096 bytes
  DF_MEM_INTLV_SIZE_AUTO = 7,      ///< Auto
} DF_MEM_INTLV_SIZE_VALIDVAL;

/** DF_DRAM_INTLV_SIZE_VAL
*  Value range for APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE
*/
typedef enum {
  DF_DRAM_INTLV_SIZE_256 = 0,     ///< Interleave size 256 bytes
  DF_DRAM_INTLV_SIZE_512 = 1,     ///< Interleave size 512 bytes
  DF_DRAM_INTLV_SIZE_1024 = 2,    ///< Interleave size 1024 bytes
  DF_DRAM_INTLV_SIZE_2048 = 3,    ///< Interleave size 2048 bytes
  DF_DRAM_INTLV_SIZE_4096 = 4,    ///< Interleave size 4096 bytes
} DF_DRAM_INTLV_SIZE_VAL;

/// @brief UMAMODE
/// @details Type:UINT8
/// @see UMAMODE_VAL
  #define APCB_TOKEN_UID_UMAMODE                                            0x1FB35295
/** UMAMODE_VAL
*  Value range for APCB_TOKEN_UID_UMAMODE
*/
typedef enum {
  UMAMODE_NONE = 0,        ///< UMA disabled
  UMAMODE_SPECIFIED = 1,   ///< UMA configuration will be pulled from other options
  UMAMODE_AUTO = 2,        ///< Auto
} UMAMODE_VAL;

/// @brief UMAVERSION
/// @details Type:UINT8
/// @see UMAVERSION_VAL
  #define APCB_TOKEN_UID_UMAVERSION                                         0xA00D4B57
/** UMAVERSION_VAL
*  Value range for APCB_TOKEN_UID_UMAVERSION
*/
typedef enum {
  UMAVERSION_LEGACY = 0,         ///< Legacy
  UMAVERSION_NON_LEGACY = 1,     ///< Non-legacy
  UMAVERSION_HYBRID_SECURE = 2,  ///< Hybrid secure
} UMAVERSION_VAL;

/// @brief UMADISPLAYRESOLUTION
/// @details Type:UINT8
/// @see UMADISPLAYRESOLUTION_VAL
  #define APCB_TOKEN_UID_UMADISPLAYRESOLUTION                               0x04CBD6B1
/** UMADISPLAYRESOLUTION_VAL
*  Value range for APCB_TOKEN_UID_UMADISPLAYRESOLUTION
*/
typedef enum {
  UMADISPLAYRESOLUTION_1920x1080_AND_BELOW = 0,  ///< 1920x1080 and below
  UMADISPLAYRESOLUTION_2560x1600 = 1,            ///< 2560x1600
  UMADISPLAYRESOLUTION_3840x2160 = 2,            ///< 3840x2160
} UMADISPLAYRESOLUTION_VAL;

/// @brief PMUTRAINMODE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_PMUTRAINMODE                                       0xBD4A6AFC  // uint8 //Obsolete.

/// @brief DISPLAY_PMU_TRAIN_RESULTS
/// @details Type:UINT8
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DISPLAY_PMU_TRAIN_RESULTS                          0x9E36A9D4  // boolean

/// @brief ODTSCMDTHROTCYC
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ODTSCMDTHROTCYC                                    0x69318E90  // uint8 //Obsolete.

/// @brief SWCMDTHROTCYC
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_SWCMDTHROTCYC                                      0xDCEC8FCB  // uint8 //Obsolete.

/// @brief DF_REMAP_AT_1TB
/// @details Type:UINT8
/// @see DF_REMAP_AT_1TB_VALIDVAL
  #define APCB_TOKEN_UID_DF_REMAP_AT_1TB                                    0x35EE96F3  // uint8
/* DF_REMAP_AT_1TB_VALIDVAL
 *  Value range for APCB_TOKEN_UID_DF_REMAP_AT_1TB_VALUE
 */
typedef enum {
  DF_REMAP_AT_1TB_FALSE = 0,    ///< Memory under the 1TB IOMMU 'hole' is lost
  DF_REMAP_AT_1TB_TRUE,         ///< Memory under the 1TB IOMMU 'hole' is remapped to above 1TB boundary
  DF_REMAP_AT_1TB_AUTO = 0xFF   ///< Auto
} DF_REMAP_AT_1TB_VALIDVAL;

/// @brief FCH_SMBUS_SPEED
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_FCH_SMBUS_SPEED                                    0x24473329

/// @brief MEM_TRAINING_HDTCTRL
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL                               0xAF6D3A6F

/// @brief MEM_RESTORE_VALID_DAYS
/// @details Type:UINT8
/// @li 0~0xFFFFFFFF = UINT32 Valid Range
  #define APCB_TOKEN_UID_MEM_RESTORE_VALID_DAYS                             0x6BD70482

/// @brief CBS_CMN_MEM_CTRLLER_BANK_SWIZZLE_DDR4
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_SWIZZLE_DDR4              0xA701EEF7  // uint8 in x86 data //Obsolete.

/// @brief CBS_CMN_MEM_ADDRESS_HASHING_DDR4
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASHING_DDR4                   0xDB22DC8F  // uint8 in x86 data //Obsolete.

/// @brief CBS_CMN_MEM_SPEED_DDR4
/// @details Type:UINT8
/// @see MEM_SPEED_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_SPEED_DDR4                             0xE0604CE9  // uint8 //uint8
/** MEM_SPEED_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_SPEED_DDR4
*/
typedef enum {
  MEM_SPEED_DDR4_AUTO = 0xFF,///<Auto
  MEM_SPEED_DDR4_667MHZ = 0x14,///<667MHz
  MEM_SPEED_DDR4_800MHZ = 0x18,///<800MHz
  MEM_SPEED_DDR4_933MHZ = 0x1C,///<933MHz
  MEM_SPEED_DDR4_1067MHZ = 0x20,///<1067MHz
  MEM_SPEED_DDR4_1200MHZ = 0x24,///<1200MHz
  MEM_SPEED_DDR4_1333MHZ = 0x28,///<1333MHz
  MEM_SPEED_DDR4_1400MHZ = 0x2A,///<1400MHz
  MEM_SPEED_DDR4_1467MHZ = 0x2C,///<1467MHz
  MEM_SPEED_DDR4_1533MHZ = 0x2E,///<1533MHz
  MEM_SPEED_DDR4_1600MHZ = 0x30,///<1600MHz
  MEM_SPEED_DDR4_1667MHZ = 0x32,///<1667MHz
  MEM_SPEED_DDR4_1733MHZ = 0x34,///<1733MHz
  MEM_SPEED_DDR4_1800MHZ = 0x36,///<1800MHz
  MEM_SPEED_DDR4_1867MHZ = 0x38,///<1867MHz
  MEM_SPEED_DDR4_1933MHZ = 0x3A,///<1933MHz
  MEM_SPEED_DDR4_2000MHZ = 0x3C,///<2000MHz
  MEM_SPEED_DDR4_333MHZ = 4,///<333MHz
  MEM_SPEED_DDR4_400MHZ = 6,///<400MHz
  MEM_SPEED_DDR4_533MHZ = 0xA,///<533MHz
  MEM_SPEED_DDR4_1050MHZ = 0x19,///<1050MHz
  MEM_SPEED_DDR4_1066MHZ = 0x1A,///<1066MHz
} MEM_SPEED_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TCL_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TCL_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCL_DDR4                        0xB2D12C4E  // uint8 //uint8
/** MEM_TIMING_TCL_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCL_DDR4
*/
typedef enum {
  MEM_TIMING_TCL_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TCL_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TCL_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TCL_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TCL_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TCL_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TCL_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TCL_DDR4_0FHCLK = 0xF,///<0Fh Clk
  MEM_TIMING_TCL_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TCL_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TCL_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TCL_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TCL_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TCL_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TCL_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TCL_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TCL_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TCL_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TCL_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TCL_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TCL_DDR4_1BHCLK = 0x1B,///<1Bh Clk
  MEM_TIMING_TCL_DDR4_1CHCLK = 0x1C,///<1Ch Clk
  MEM_TIMING_TCL_DDR4_1DHCLK = 0x1D,///<1Dh Clk
  MEM_TIMING_TCL_DDR4_1EHCLK = 0x1E,///<1Eh Clk
  MEM_TIMING_TCL_DDR4_1FHCLK = 0x1F,///<1Fh Clk
  MEM_TIMING_TCL_DDR4_20HCLK = 0x20,///<20h Clk
  MEM_TIMING_TCL_DDR4_21HCLK = 0x21,///<21h Clk
} MEM_TIMING_TCL_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRCDRD_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRCDRD_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDRD_DDR4                     0xB22E12F7  // uint8 //uint8
/** MEM_TIMING_TRCDRD_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDRD_DDR4
*/
typedef enum {
  MEM_TIMING_TRCDRD_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRCDRD_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRCDRD_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRCDRD_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRCDRD_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TRCDRD_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TRCDRD_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TRCDRD_DDR4_0FHCLK = 0xF,///<0Fh Clk
  MEM_TIMING_TRCDRD_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TRCDRD_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TRCDRD_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TRCDRD_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TRCDRD_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TRCDRD_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TRCDRD_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TRCDRD_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TRCDRD_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TRCDRD_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TRCDRD_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TRCDRD_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRCDRD_DDR4_1BHCLK = 0x1B,///<1Bh Clk
} MEM_TIMING_TRCDRD_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRCDWR_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRCDWR_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDWR_DDR4                     0xB3840F07  // uint8 //uint8
/** MEM_TIMING_TRCDWR_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDWR_DDR4
*/
typedef enum {
  MEM_TIMING_TRCDWR_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRCDWR_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRCDWR_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRCDWR_DDR4_0XACLK = 0xA,///<0xA Clk
  MEM_TIMING_TRCDWR_DDR4_0XBCLK = 0xB,///<0xB Clk
  MEM_TIMING_TRCDWR_DDR4_0XCCLK = 0xC,///<0xC Clk
  MEM_TIMING_TRCDWR_DDR4_0XDCLK = 0xD,///<0xD Clk
  MEM_TIMING_TRCDWR_DDR4_0XECLK = 0xE,///<0xE Clk
  MEM_TIMING_TRCDWR_DDR4_0XFCLK = 0xF,///<0xF Clk
  MEM_TIMING_TRCDWR_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TRCDWR_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TRCDWR_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TRCDWR_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TRCDWR_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TRCDWR_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TRCDWR_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TRCDWR_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TRCDWR_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TRCDWR_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TRCDWR_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TRCDWR_DDR4_1BHCLK = 0x1B,///<1Bh Clk
} MEM_TIMING_TRCDWR_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRP_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRP_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRP_DDR4                        0xA2D655C8  // uint8 //uint8
/** MEM_TIMING_TRP_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRP_DDR4
*/
typedef enum {
  MEM_TIMING_TRP_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRP_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRP_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRP_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRP_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TRP_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TRP_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TRP_DDR4_0FHCLK = 0xF,///<0Fh Clk
  MEM_TIMING_TRP_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TRP_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TRP_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TRP_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TRP_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRP_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TRP_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TRP_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TRP_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TRP_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TRP_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TRP_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TRP_DDR4_1BHCLK = 0x1B,///<1Bh Clk
} MEM_TIMING_TRP_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRAS_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRAS_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRAS_DDR4                       0x03E98A84  // uint8 //uint8
/** MEM_TIMING_TRAS_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRAS_DDR4
*/
typedef enum {
  MEM_TIMING_TRAS_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TRAS_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TRAS_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TRAS_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TRAS_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TRAS_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TRAS_DDR4_1BHCLK = 0x1B,///<1Bh Clk
  MEM_TIMING_TRAS_DDR4_1CHCLK = 0x1C,///<1Ch Clk
  MEM_TIMING_TRAS_DDR4_1DHCLK = 0x1D,///<1Dh Clk
  MEM_TIMING_TRAS_DDR4_1EHCLK = 0x1E,///<1Eh Clk
  MEM_TIMING_TRAS_DDR4_1FHCLK = 0x1F,///<1Fh Clk
  MEM_TIMING_TRAS_DDR4_20HCLK = 0x20,///<20h Clk
  MEM_TIMING_TRAS_DDR4_21HCLK = 0x21,///<21h Clk
  MEM_TIMING_TRAS_DDR4_22HCLK = 0x22,///<22h Clk
  MEM_TIMING_TRAS_DDR4_23HCLK = 0x23,///<23h Clk
  MEM_TIMING_TRAS_DDR4_24HCLK = 0x24,///<24h Clk
  MEM_TIMING_TRAS_DDR4_25HCLK = 0x25,///<25h Clk
  MEM_TIMING_TRAS_DDR4_26HCLK = 0x26,///<26h Clk
  MEM_TIMING_TRAS_DDR4_27HCLK = 0x27,///<27h Clk
  MEM_TIMING_TRAS_DDR4_28HCLK = 0x28,///<28h Clk
  MEM_TIMING_TRAS_DDR4_29HCLK = 0x29,///<29h Clk
  MEM_TIMING_TRAS_DDR4_2AHCLK = 0x2A,///<2Ah Clk
  MEM_TIMING_TRAS_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRAS_DDR4_2BHCLK = 0x2B,///<2Bh Clk
  MEM_TIMING_TRAS_DDR4_2CHCLK = 0x2C,///<2Ch Clk
  MEM_TIMING_TRAS_DDR4_2DHCLK = 0x2D,///<2Dh Clk
  MEM_TIMING_TRAS_DDR4_2EHCLK = 0x2E,///<2Eh Clk
  MEM_TIMING_TRAS_DDR4_2FHCLK = 0x2F,///<2Fh Clk
  MEM_TIMING_TRAS_DDR4_30HCLK = 0x30,///<30h Clk
  MEM_TIMING_TRAS_DDR4_31HCLK = 0x31,///<31h Clk
  MEM_TIMING_TRAS_DDR4_32HCLK = 0x32,///<32h Clk
  MEM_TIMING_TRAS_DDR4_33HCLK = 0x33,///<33h Clk
  MEM_TIMING_TRAS_DDR4_34HCLK = 0x34,///<34h Clk
  MEM_TIMING_TRAS_DDR4_35HCLK = 0x35,///<35h Clk
  MEM_TIMING_TRAS_DDR4_36HCLK = 0x36,///<36h Clk
  MEM_TIMING_TRAS_DDR4_37HCLK = 0x37,///<37h Clk
  MEM_TIMING_TRAS_DDR4_38HCLK = 0x38,///<38h Clk
  MEM_TIMING_TRAS_DDR4_39HCLK = 0x39,///<39h Clk
  MEM_TIMING_TRAS_DDR4_3AHCLK = 0x3A,///<3Ah Clk
} MEM_TIMING_TRAS_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TSME_DDR4
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TSME_DDR4                              0xBC3F4EED  // uint8 in x86 data, not used in code //Obsolete.

/// @brief CBS_CMN_MEM_TIMING_TRC_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRC_CTRL_DDR4                   0xBBC7849A  // uint8 in x86 data, not used in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TRC_DDR4
/// @details Type:UINT8
/// @li 0x1D~0x87 = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRC_DDR4                        0x5B082A2B  // uint8 in data //uint8

/// @brief CBS_CMN_MEM_TIMING_TRRD_S_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRRD_S_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_S_DDR4                     0x50B2D463  // uint8 //uint8
/** MEM_TIMING_TRRD_S_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_S_DDR4
*/
typedef enum {
  MEM_TIMING_TRRD_S_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRRD_S_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TRRD_S_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TRRD_S_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TRRD_S_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TRRD_S_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRRD_S_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRRD_S_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRRD_S_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRRD_S_DDR4_0CHCLK = 0xC,///<0Ch Clk
} MEM_TIMING_TRRD_S_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRRD_L_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRRD_L_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_L_DDR4                     0xFC23535E  // uint8 //uint8
/** MEM_TIMING_TRRD_L_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_L_DDR4
*/
typedef enum {
  MEM_TIMING_TRRD_L_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRRD_L_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TRRD_L_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TRRD_L_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TRRD_L_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TRRD_L_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRRD_L_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRRD_L_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRRD_L_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRRD_L_DDR4_0CHCLK = 0xC,///<0Ch Clk
} MEM_TIMING_TRRD_L_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TFAW_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TFAW_CTRL_DDR4                  0xB36B7473  // uint8 in x86 data, used as 2nd level parent control in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TFAW_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TFAW_DDR4                       0xDE1D6178  // uint8 //uint8

/// @brief CBS_CMN_MEM_TIMING_TWTR_S_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TWTR_S_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_S_DDR4                     0xEB6A9FE9  // uint8 //uint8
/** MEM_TIMING_TWTR_S_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_S_DDR4
*/
typedef enum {
  MEM_TIMING_TWTR_S_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TWTR_S_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TWTR_S_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TWTR_S_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TWTR_S_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TWTR_S_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TWTR_S_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TWTR_S_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TWTR_S_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TWTR_S_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TWTR_S_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TWTR_S_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TWTR_S_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TWTR_S_DDR4_0EHCLK = 0xE,///<0Eh Clk
} MEM_TIMING_TWTR_S_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TWTR_L_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TWTR_L_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_L_DDR4                     0x5782D479  // uint8 //uint8
/** MEM_TIMING_TWTR_L_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_L_DDR4
*/
typedef enum {
  MEM_TIMING_TWTR_L_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TWTR_L_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TWTR_L_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TWTR_L_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TWTR_L_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TWTR_L_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TWTR_L_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TWTR_L_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TWTR_L_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TWTR_L_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TWTR_L_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TWTR_L_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TWTR_L_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TWTR_L_DDR4_0EHCLK = 0xE,///<0Eh Clk
} MEM_TIMING_TWTR_L_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TWR_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWR_CTRL_DDR4                   0x5C26F15B  // uint8 //uint8

/// @brief CBS_CMN_MEM_TIMING_TWR_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWR_DDR4                        0x4FC9F17E  // uint8 //uint8

/// @brief CBS_CMN_MEM_TIMING_TRCPAGE_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_CTRL_DDR4               0x2BC2CA69  // uint8 //uint8

/// @brief CBS_CMN_MEM_TIMING_TRCPAGE_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_DDR4                    0x1A82F1F6  // uint8 (new data), uint16 (old data in x86), uint8 in code. //uint8

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR4            0xC71A49AA  // uint8 in x86 data, 2nd level parent control in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_L_DDR4
/// @details Type:UINT8
/// @li 0x01~0x0F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_L_DDR4                 0x6A594A63  // uint8 //uint8

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR4            0x91371469  // uint8 , 2nd level parent control in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_L_DDR4
/// @details Type:UINT8
/// @li 0x01~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_L_DDR4                 0xA148AB5F  // uint8 in x86 data, uint32 varible in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TRFC_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC_CTRL_DDR4                  0x86ED48B5  // uint8 , 2nd level parent control in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TRFC2_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC2_CTRL_DDR4                 0x6D8E4884  // uint8 , 2nd level parent control in code //uint8

/// @brief CBS_CMN_MEM_TIMING_TRFC4_CTRL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC4_CTRL_DDR4                 0xFBB240FD  // uint8 , 2nd level parent control in code //uint8

/// @brief CBS_CMN_MEM_OVERCLOCK_FAIL_CNT
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_FAIL_CNT                     0x92FECF1C  // uint8 //Obsolete.

/// @brief CBS_CMN_MEM_CTRLLER_PROC_ODT_DDR4
/// @details Type:UINT8
/// @see MEM_CTRLLER_PROC_ODT_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PROC_ODT_DDR4                  0xE74CFD9B  // uint8 //uint8
/** MEM_CTRLLER_PROC_ODT_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PROC_ODT_DDR4
*/
typedef enum {
  MEM_CTRLLER_PROC_ODT_DDR4_AUTO = 0xFF,///<Auto
  MEM_CTRLLER_PROC_ODT_DDR4_HIGHIMPEDANCE = 0,///<High Impedance
  MEM_CTRLLER_PROC_ODT_DDR4_480OHM = 1,///<480 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_240OHM = 2,///<240 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_160OHM = 3,///<160 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_120OHM = 8,///<120 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_96OHM = 9,///<96 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_80OHM = 10,///<80 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_686OHM = 11,///<68.6 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_60OHM = 0x18,///<60 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_533OHM = 0x19,///<53.3 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_48OHM = 0x1A,///<48 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_436OHM = 0x1B,///<43.6 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_40OHM = 0x38,///<40 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_369OHM = 0x39,///<36.9 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_343OHM = 0x3A,///<34.3 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_32OHM = 0x3B,///<32 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_30OHM = 0x3E,///<30 ohm
  MEM_CTRLLER_PROC_ODT_DDR4_282OHM = 0x3F,///<28.2 ohm
} MEM_CTRLLER_PROC_ODT_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_MBIST_TESTMODE
/// @details Type:UINT8
/// @see MEM_MBIST_TESTMODE_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_TESTMODE                         0x4D0C053F  //uint8
/** MEM_MBIST_TESTMODE_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_TESTMODE
*/
typedef enum {
  MEM_MBIST_TESTMODE_INTERFACEMODE = 0,///<Interface Mode
  MEM_MBIST_TESTMODE_DATAEYEMODE = 1,///<Data Eye Mode
} MEM_MBIST_TESTMODE_VALIDVAL;

/// @brief CCX_P0_SETTING
/// @details Type:UINT8
/// @see CPU_PST_CUSTOM_P0_VALIDVAL
  #define APCB_TOKEN_UID_CCX_P0_SETTING                                     0x83CF6A10  //uint8
/** CPU_PST_CUSTOM_P0_VALIDVAL
*   Value range for APCB_TOKEN_UID_CCX_P0_SETTING
*/
typedef enum {
  CPU_PST_CUSTOM_P0_CUSTOM = 1,///<Custom
  CPU_PST_CUSTOM_P0_AUTO = 2,///<Auto
} CPU_PST_CUSTOM_P0_VALIDVAL;

/// @brief CCX_P0_FID
/// @details Type:UINT8
/// @li 0x10~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CCX_P0_FID                                         0x6B492DDA  //uint8

/// @brief CCX_P0_DID
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CCX_P0_DID                                         0x09C5BCCC  //uint8

/// @brief CCX_P0_VID
/// @details Type:UINT8
/// @li 0x00~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CCX_P0_VID                                         0x65370189  //uint8

/// @brief DF_MEM_CLEAR
/// @details Type:UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_DF_MEM_CLEAR                                       0x9D177E57  // uint8 in code, uint8 and bool in data //uint8

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRDRD_SC_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_DDR4                   0x2E199CAA  //uint8
/** MEM_TIMING_TRDRD_SC_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_DDR4
*/
typedef enum {
  MEM_TIMING_TRDRD_SC_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TRDRD_SC_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TRDRD_SC_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TRDRD_SC_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TRDRD_SC_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TRDRD_SC_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TRDRD_SC_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TRDRD_SC_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRDRD_SC_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRDRD_SC_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRDRD_SC_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRDRD_SC_DDR4_AUTO = 0xFF,///<Auto
} MEM_TIMING_TRDRD_SC_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SD_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRDRD_SD_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SD_DDR4                   0x6AD7647F  //uint8
/** MEM_TIMING_TRDRD_SD_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SD_DDR4
*/
typedef enum {
  MEM_TIMING_TRDRD_SD_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TRDRD_SD_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TRDRD_SD_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TRDRD_SD_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TRDRD_SD_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TRDRD_SD_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TRDRD_SD_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRDRD_SD_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRDRD_SD_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRDRD_SD_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRDRD_SD_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRDRD_SD_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TRDRD_SD_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TRDRD_SD_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TRDRD_SD_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TRDRD_SD_DDR4_0FHCLK = 0xF,///<0Fh Clk
} MEM_TIMING_TRDRD_SD_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRDRD_DD_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRDRD_DD_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_DD_DDR4                   0xA1E46AFC  //uint8
/** MEM_TIMING_TRDRD_DD_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_DD_DDR4
*/
typedef enum {
  MEM_TIMING_TRDRD_DD_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TRDRD_DD_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TRDRD_DD_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TRDRD_DD_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TRDRD_DD_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TRDRD_DD_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TRDRD_DD_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRDRD_DD_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRDRD_DD_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRDRD_DD_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRDRD_DD_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRDRD_DD_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TRDRD_DD_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TRDRD_DD_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TRDRD_DD_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TRDRD_DD_DDR4_0FHCLK = 0xF,///<0Fh Clk
} MEM_TIMING_TRDRD_DD_DDR4;

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TWRWR_SC_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_DDR4                   0x4DE9DD1F  //uint8
/** MEM_TIMING_TWRWR_SC_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_DDR4
*/
typedef enum {
  MEM_TIMING_TWRWR_SC_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TWRWR_SC_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TWRWR_SC_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TWRWR_SC_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TWRWR_SC_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TWRWR_SC_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TWRWR_SC_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TWRWR_SC_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TWRWR_SC_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TWRWR_SC_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TWRWR_SC_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TWRWR_SC_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TWRWR_SC_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TWRWR_SC_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TWRWR_SC_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TWRWR_SC_DDR4_0FHCLK = 0xF,///<0Fh Clk
} MEM_TIMING_TWRWR_SC_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SD_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TWRWR_SD_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SD_DDR4                   0xA95E4B3B  //uint8
/** MEM_TIMING_TWRWR_SD_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SD_DDR4
*/
typedef enum {
  MEM_TIMING_TWRWR_SD_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TWRWR_SD_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TWRWR_SD_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TWRWR_SD_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TWRWR_SD_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TWRWR_SD_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TWRWR_SD_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TWRWR_SD_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TWRWR_SD_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TWRWR_SD_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TWRWR_SD_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TWRWR_SD_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TWRWR_SD_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TWRWR_SD_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TWRWR_SD_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TWRWR_SD_DDR4_0FHCLK = 0xF,///<0Fh Clk
} MEM_TIMING_TWRWR_SD_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TWRWR_DD_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TWRWR_DD_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_DD_DDR4                   0x9E720EC8  //uint8
/** MEM_TIMING_TWRWR_DD_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_DD_DDR4
*/
typedef enum {
  MEM_TIMING_TWRWR_DD_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TWRWR_DD_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TWRWR_DD_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TWRWR_DD_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TWRWR_DD_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TWRWR_DD_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TWRWR_DD_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TWRWR_DD_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TWRWR_DD_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TWRWR_DD_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TWRWR_DD_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TWRWR_DD_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TWRWR_DD_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TWRWR_DD_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TWRWR_DD_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TWRWR_DD_DDR4_0FHCLK = 0xF,///<0Fh Clk
} MEM_TIMING_TWRWR_DD_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRDWR_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRDWR_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDWR_DDR4                      0x7636900E  //uint8
/** MEM_TIMING_TRDWR_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDWR_DDR4
*/
typedef enum {
  MEM_TIMING_TRDWR_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TRDWR_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TRDWR_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TRDWR_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TRDWR_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TRDWR_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TRDWR_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRDWR_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRDWR_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRDWR_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRDWR_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TRDWR_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TRDWR_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TRDWR_DDR4_0FHCLK = 0xF,///<0Fh Clk
  MEM_TIMING_TRDWR_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TRDWR_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TRDWR_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TRDWR_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TRDWR_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TRDWR_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TRDWR_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TRDWR_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TRDWR_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TRDWR_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TRDWR_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TRDWR_DDR4_1BHCLK = 0x1B,///<1Bh Clk
  MEM_TIMING_TRDWR_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRDWR_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TRDWR_DDR4_1CHCLK = 0x1C,///<1Ch Clk
  MEM_TIMING_TRDWR_DDR4_1DHCLK = 0x1D,///<1Dh Clk
  MEM_TIMING_TRDWR_DDR4_1EHCLK = 0x1E,///<1Eh Clk
  MEM_TIMING_TRDWR_DDR4_1FHCLK = 0x1F,///<1Fh Clk
} MEM_TIMING_TRDWR_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TWRRD_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TWRRD_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRRD_DDR4                      0x8FD5A3D2  //uint8
/** MEM_TIMING_TWRRD_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRRD_DDR4
*/
typedef enum {
  MEM_TIMING_TWRRD_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TWRRD_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TWRRD_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TWRRD_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TWRRD_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TWRRD_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TWRRD_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TWRRD_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TWRRD_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TWRRD_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TWRRD_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TWRRD_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TWRRD_DDR4_0CH = 0xC,///<0Ch
  MEM_TIMING_TWRRD_DDR4_0DH = 0xD,///<0Dh
  MEM_TIMING_TWRRD_DDR4_0EH = 0xE,///<0Eh
  MEM_TIMING_TWRRD_DDR4_0FH = 0xF,///<0Fh
} MEM_TIMING_TWRRD_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TRTP_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TRTP_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRTP_DDR4                       0x88B9F33B  //uint8
/** MEM_TIMING_TRTP_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRTP_DDR4
*/
typedef enum {
  MEM_TIMING_TRTP_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TRTP_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TRTP_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TRTP_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TRTP_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TRTP_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TRTP_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TRTP_DDR4_5CLK = 0x5,///<5 Clk
  MEM_TIMING_TRTP_DDR4_6CLK = 0x6,///<6 Clk
  MEM_TIMING_TRTP_DDR4_7CLK = 0x7,///<7 Clk
  MEM_TIMING_TRTP_DDR4_0EHCLK = 0xE,///<0Eh Clk
} MEM_TIMING_TRTP_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TCWL_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TCWL_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCWL_DDR4                       0xBDBA463D  //uint8
/** MEM_TIMING_TCWL_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCWL_DDR4
*/
typedef enum {
  MEM_TIMING_TCWL_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TCWL_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TCWL_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TCWL_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TCWL_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TCWL_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TCWL_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TCWL_DDR4_0FHCLK = 0xF,///<0Fh Clk
  MEM_TIMING_TCWL_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TCWL_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TCWL_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TCWL_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TCWL_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TCWL_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TCWL_DDR4_16HCLK = 0x16,///<16h Clk
} MEM_TIMING_TCWL_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR4               0x9CC11046  //uint8

/// @brief CBS_CMN_MEM_CTRLLER_RTT_NOM_DDR4
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_NOM_DDR4                   0xBFFF404B  //uint8
/** MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_NOM_DDR4
*/
typedef enum {
  MEM_CTRLLER_RTT_NOM_DDR4_RTT_NOMDISABLE = 0,///<Rtt_Nom Disable
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ4 = 1,///<RZQ/4
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ2 = 2,///<RZQ/2
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ6 = 3,///<RZQ/6
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ1 = 4,///<RZQ/1
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ5 = 5,///<RZQ/5
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ3 = 6,///<RZQ/3
  MEM_CTRLLER_RTT_NOM_DDR4_RZQ7 = 7,///<RZQ/7
  MEM_CTRLLER_RTT_NOM_DDR4_AUTO = 0xFF,///<Auto
} MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_CTRLLER_RTT_WR_DDR4
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_WR_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_WR_DDR4                    0xA80353D7  //uint8
/** MEM_CTRLLER_RTT_WR_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_WR_DDR4
*/
typedef enum {
  MEM_CTRLLER_RTT_WR_DDR4_DYNAMICODTOFF = 0,///<Dynamic ODT Off
  MEM_CTRLLER_RTT_WR_DDR4_RZQ2 = 1,///<RZQ/2
  MEM_CTRLLER_RTT_WR_DDR4_RZQ1 = 2,///<RZQ/1
  MEM_CTRLLER_RTT_WR_DDR4_HIZ = 3,///<Hi-Z
  MEM_CTRLLER_RTT_WR_DDR4_RZQ3 = 4,///<RZQ/3
  MEM_CTRLLER_RTT_WR_DDR4_AUTO = 0xFF,///<Auto
} MEM_CTRLLER_RTT_WR_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_CTRLLER_RTT_PARK_DDR4
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_PARK_DDR4                  0x58BF5D3E  //uint8
/** MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_PARK_DDR4
*/
typedef enum {
  MEM_CTRLLER_RTT_PARK_DDR4_RTT_PARKDISABLE = 0,///<Rtt_PARK Disable
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ4 = 1,///<RZQ/4
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ2 = 2,///<RZQ/2
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ6 = 3,///<RZQ/6
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ1 = 4,///<RZQ/1
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ5 = 5,///<RZQ/5
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ3 = 6,///<RZQ/3
  MEM_CTRLLER_RTT_PARK_DDR4_RZQ7 = 7,///<RZQ/7
  MEM_CTRLLER_RTT_PARK_DDR4_AUTO = 0xFF,///<Auto
} MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_TIMING_TCKE_DDR4
/// @details Type:UINT8
/// @see MEM_TIMING_TCKE_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCKE_DDR4                       0x5EF92FB0  //uint8
/** MEM_TIMING_TCKE_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCKE_DDR4
*/
typedef enum {
  MEM_TIMING_TCKE_DDR4_AUTO = 0xFF,///<Auto
  MEM_TIMING_TCKE_DDR4_1CLK = 1,///<1 Clk
  MEM_TIMING_TCKE_DDR4_2CLK = 2,///<2 Clk
  MEM_TIMING_TCKE_DDR4_3CLK = 3,///<3 Clk
  MEM_TIMING_TCKE_DDR4_4CLK = 4,///<4 Clk
  MEM_TIMING_TCKE_DDR4_5CLK = 5,///<5 Clk
  MEM_TIMING_TCKE_DDR4_6CLK = 6,///<6 Clk
  MEM_TIMING_TCKE_DDR4_7CLK = 7,///<7 Clk
  MEM_TIMING_TCKE_DDR4_8CLK = 8,///<8 Clk
  MEM_TIMING_TCKE_DDR4_9CLK = 9,///<9 Clk
  MEM_TIMING_TCKE_DDR4_0AHCLK = 0xA,///<0Ah Clk
  MEM_TIMING_TCKE_DDR4_0BHCLK = 0xB,///<0Bh Clk
  MEM_TIMING_TCKE_DDR4_0CHCLK = 0xC,///<0Ch Clk
  MEM_TIMING_TCKE_DDR4_0DHCLK = 0xD,///<0Dh Clk
  MEM_TIMING_TCKE_DDR4_0EHCLK = 0xE,///<0Eh Clk
  MEM_TIMING_TCKE_DDR4_0FHCLK = 0xF,///<0Fh Clk
  MEM_TIMING_TCKE_DDR4_10HCLK = 0x10,///<10h Clk
  MEM_TIMING_TCKE_DDR4_11HCLK = 0x11,///<11h Clk
  MEM_TIMING_TCKE_DDR4_12HCLK = 0x12,///<12h Clk
  MEM_TIMING_TCKE_DDR4_13HCLK = 0x13,///<13h Clk
  MEM_TIMING_TCKE_DDR4_14HCLK = 0x14,///<14h Clk
  MEM_TIMING_TCKE_DDR4_15HCLK = 0x15,///<15h Clk
  MEM_TIMING_TCKE_DDR4_16HCLK = 0x16,///<16h Clk
  MEM_TIMING_TCKE_DDR4_17HCLK = 0x17,///<17h Clk
  MEM_TIMING_TCKE_DDR4_18HCLK = 0x18,///<18h Clk
  MEM_TIMING_TCKE_DDR4_19HCLK = 0x19,///<19h Clk
  MEM_TIMING_TCKE_DDR4_1AHCLK = 0x1A,///<1Ah Clk
  MEM_TIMING_TCKE_DDR4_1BHCLK = 0x1B,///<1Bh Clk
  MEM_TIMING_TCKE_DDR4_1CHCLK = 0x1C,///<1Ch Clk
  MEM_TIMING_TCKE_DDR4_1DHCLK = 0x1D,///<1Dh Clk
  MEM_TIMING_TCKE_DDR4_1EHCLK = 0x1E,///<1Eh Clk
  MEM_TIMING_TCKE_DDR4_1FHCLK = 0x1F,///<1Fh Clk
} MEM_TIMING_TCKE_DDR4_VALIDVAL;

/// @brief CBS_CMN_CLDO_VDDP_CTL
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_CLDO_VDDP_CTL                              0xE033C709  //uint8

/// @brief CBS_CMN_MEM_CTRLLER2_T_MODE_DDR4
/// @details Type:UINT8
/// @see MEM_CTRLLER2_T_MODE_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER2_T_MODE_DDR4                   0x0677966C  //uint8
/** MEM_CTRLLER2_T_MODE_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER2_T_MODE_DDR4
*/
typedef enum {
  MEM_CTRLLER2_T_MODE_DDR4_1T = 0,///<1T
  MEM_CTRLLER2_T_MODE_DDR4_2T = 1,///<2T
  MEM_CTRLLER2_T_MODE_DDR4_AUTO = 0xFF,///<Auto
} MEM_CTRLLER2_T_MODE_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_CAD_BUS_TIMING_CTL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_TIMING_CTL_DDR4                0x264B7625  //uint8

/// @brief CBS_CMN_MEM_ADDR_CMD_SETUP_DDR4
/// @details Type:UINT8
/// @li 0x00~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_SETUP_DDR4                    0x1E974F51  //uint8

/// @brief CBS_CMN_MEM_CS_ODT_SETUP_DDR4
/// @details Type:UINT8
/// @li 0x00~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CS_ODT_SETUP_DDR4                      0x3B1B99E9  //uint8

/// @brief CBS_CMN_MEM_CKE_SETUP_DDR4
/// @details Type:UINT8
/// @li 0x00~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CKE_SETUP_DDR4                         0x37D13B99  //uint8

/// @brief CBS_CMN_MEM_CAD_BUS_DRV_STREN_CTL_DDR4
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_DRV_STREN_CTL_DDR4             0xDB047BEB  //uint8

/// @brief CBS_CMN_MEM_CAD_BUS_CLK_DRV_STREN_DDR4
/// @details Type:UINT8
/// @see MEM_CAD_BUS_CLK_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CLK_DRV_STREN_DDR4             0xD8ADC426  //uint8
/** MEM_CAD_BUS_CLK_DRV_STREN_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CLK_DRV_STREN_DDR4
*/
typedef enum {
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_AUTO = 0xFF,///<Auto
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_1200OHM = 0,///<120.0 Ohm
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_600OHM = 1,///<60.0 Ohm
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_400OHM = 3,///<40.0 Ohm
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_300OHM = 7,///<30.0 Ohm
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_240OHM = 15,///<24.0 Ohm
  MEM_CAD_BUS_CLK_DRV_STREN_DDR4_200OHM = 31,///<20.0 Ohm
} MEM_CAD_BUS_CLK_DRV_STREN_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4
/// @details Type:UINT8
/// @see MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4        0xF91E8072  //uint8
/** MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4
*/
typedef enum {
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_AUTO = 0xFF,///<Auto
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_1200OHM = 0,///<120.0 Ohm
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_600OHM = 1,///<60.0 Ohm
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_400OHM = 3,///<40.0 Ohm
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_300OHM = 7,///<30.0 Ohm
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_240OHM = 15,///<24.0 Ohm
  MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_200OHM = 31,///<20.0 Ohm
} MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4
/// @details Type:UINT8
/// @see MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4          0x7BBE9AF1  //uint8
/** MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4
*/
typedef enum {
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_AUTO = 0xFF,///<Auto
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_1200OHM = 0,///<120.0 Ohm
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_600OHM = 1,///<60.0 Ohm
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_400OHM = 3,///<40.0 Ohm
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_300OHM = 7,///<30.0 Ohm
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_240OHM = 15,///<24.0 Ohm
  MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_200OHM = 31,///<20.0 Ohm
} MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_VALIDVAL;

/// @brief CBS_CMN_MEM_CAD_BUS_CKE_DRV_STREN_DDR4
/// @details Type:UINT8
/// @see MEM_CAD_BUS_CKE_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CKE_DRV_STREN_DDR4             0x1014364F  //uint8
/** MEM_CAD_BUS_CKE_DRV_STREN_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CKE_DRV_STREN_DDR4
*/
typedef enum {
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_AUTO = 0xFF,///<Auto
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_1200OHM = 0,///<120.0 Ohm
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_600OHM = 1,///<60.0 Ohm
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_400OHM = 3,///<40.0 Ohm
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_300OHM = 7,///<30.0 Ohm
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_240OHM = 15,///<24.0 Ohm
  MEM_CAD_BUS_CKE_DRV_STREN_DDR4_200OHM = 31,///<20.0 Ohm
} MEM_CAD_BUS_CKE_DRV_STREN_DDR4_VALIDVAL;

/// @brief CBS_DF_CMN_SYNC_FLOOD_PROP
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_DF_CMN_SYNC_FLOOD_PROP                         0x55CE1C39  //Obsolete

/// @brief CBS_CMN_MEM_OVERCLOCK_DIMM_CONFIG_CHECK
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_DIMM_CONFIG_CHECK            0x3EE00E21  //Obsolete

/// @brief CBS_CMN_MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4
/// @details Type:UINT8
/// @see MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4      0x325AEF95  //Obsolete
/** MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4
*/
typedef enum {
  MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_X4 = 0,///<x4
  MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_X8 = 1,///<x8
  MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_X16 = 2,///<x16
  MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_AUTO = 0xFF,///<Auto
} MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_VALIDVAL;

/// @brief MEM_TSME_ENABLE
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_TSME_ENABLE                                    0xD1FA6660  // bool, bool //ToDo: Auto: 0xFF

/// @brief MEM_ADDR_CMD_PARITY_ERROR_MAX_REPLAY_D
/// @details Type:UINT8
/// @li 0x00~0x3F = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_ERROR_MAX_REPLAY_DDR4  0x04E6A482

/// @brief MEM_ADDR_CMD_PARITY_RETRY_DDR4
/// @details Type:UINT8
/// @see MEM_ADDR_CMD_PARITY_RETRY_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_RETRY_DDR4             0xBE8BEBCE
/** MEM_ADDR_CMD_PARITY_RETRY_DDR4_VALIDVAL
*   Value range for APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_RETRY_DDR4
*/
typedef enum {
  MEM_ADDR_CMD_PARITY_RETRY_DDR4_AUTO = 0xFF,///<Auto
  MEM_ADDR_CMD_PARITY_RETRY_DDR4_DISABLED = 0,///<Disabled
  MEM_ADDR_CMD_PARITY_RETRY_DDR4_ENABLED = 1,///<Enabled
} MEM_ADDR_CMD_PARITY_RETRY_DDR4_VALIDVAL;

/// @brief MEM_WRITE_CRC_ERROR_MAX_REPLAY_DDR4
/// @details Type:UINT8
/// @li 0x01~0x08 = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_WRITE_CRC_ERROR_MAX_REPLAY_DDR4        0x74A08BEC  //uint8

/// @brief CPU_SYNC_FLOOD_ON_MCA
/// @details Type:UINT8
/// @li 0x00~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CPU_SYNC_FLOOD_ON_MCA                              0x96664127

/// @brief xGMI Link Configuration
/// @details Type:UINT8
/// @see DF_XGMI_LINK_CFG_VALIDVAL
  #define APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG                                 0xB0B6AD3E  // uint8
  #define   DF3_XGMI2_LINK_CFG_2LINK  0
  #define   DF3_XGMI2_LINK_CFG_3LINK  1
  #define   DF3_XGMI2_LINK_CFG_4LINK  2
/** DF_XGMI_LINK_CFG_VALIDVAL
*   Value range for APCB_TOKEN_UID_DF3_XGMI2_LINK_CFG
*/
typedef enum {
  DF_XGMI_LINK_CFG_AUTO = 3,///<Auto
  DF_XGMI_LINK_CFG_2XGMILINKS = 0,///<2 xGMI Links
  DF_XGMI_LINK_CFG_3XGMILINKS = 1,///<3 xGMI Links
  DF_XGMI_LINK_CFG_4XGMILINKS = 2,///<4 xGMI Links
} DF_XGMI_LINK_CFG_VALIDVAL;

/// @brief DF_3LINK_MAX_XGMI_SPEED
/// @details Type:UINT8
/// @see DF_XGMI_LINK_SPEED
  #define APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED                            0x53BA449B  // uint8

/// @brief DF_4LINK_MAX_XGMI_SPEED
/// @details Type:UINT8
/// @see DF_XGMI_LINK_SPEED
  #define APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED                            0x3F307CB3  // uint8
/* DF_XGMI_LINK_SPEED
 *  Value range for APCB_TOKEN_UID_DF_3LINK_MAX_XGMI_SPEED_VALUE
 *  Value range for APCB_TOKEN_UID_DF_4LINK_MAX_XGMI_SPEED_VALUE
 */
typedef enum {
  DF_XGMI_LINK_SPEED_640 = 0,       ///< 6.4 Gbps
  DF_XGMI_LINK_SPEED_746,           ///< 7.467 Gbps
  DF_XGMI_LINK_SPEED_853,           ///< 8.533 Gbps
  DF_XGMI_LINK_SPEED_960,           ///< 9.6 Gbps
  DF_XGMI_LINK_SPEED_1066,          ///< 10.667 Gbps
  DF_XGMI_LINK_SPEED_1100,          ///< 11 Gbps
  DF_XGMI_LINK_SPEED_1200,          ///< 12 Gbps
  DF_XGMI_LINK_SPEED_1300,          ///< 13 Gbps
  DF_XGMI_LINK_SPEED_1400,          ///< 14 Gbps
  DF_XGMI_LINK_SPEED_1500,          ///< 15 Gbps
  DF_XGMI_LINK_SPEED_1600,          ///< 16 Gbps
  DF_XGMI_LINK_SPEED_1700,          ///< 17 Gbps
  DF_XGMI_LINK_SPEED_1800,          ///< 18 Gbps
  DF_XGMI_LINK_SPEED_1900,          ///< 19 Gbps
  DF_XGMI_LINK_SPEED_2000,          ///< 20 Gbps
  DF_XGMI_LINK_SPEED_2100,          ///< 21 Gbps
  DF_XGMI_LINK_SPEED_2200,          ///< 22 Gbps
  DF_XGMI_LINK_SPEED_2300,          ///< 23 Gbps
  DF_XGMI_LINK_SPEED_2400,          ///< 24 Gbps
  DF_XGMI_LINK_SPEED_2500,          ///< 25 Gbps
  DF_XGMI_LINK_SPEED_AUTO = 0xFF    ///< Auto
} DF_XGMI_LINK_SPEED;

/// @brief DF_DRAM_NPS
/// @details Type:UINT8
/// @see DF_DRAM_NPS_VALIDVAL
  #define APCB_TOKEN_UID_DF_DRAM_NPS                                        0x2CF3DAC9  // uint8
/* DF_DRAM_NPS_VALIDVAL
 *  Value range for APCB_TOKEN_UID_DF_DRAM_NPS_VALUE
 */
typedef enum {
  DF_DRAM_NPS_0 = 0,      ///< No NUMA nodes
  DF_DRAM_NPS_1,          ///< 1 NUMA node per socket
  DF_DRAM_NPS_2,          ///< 2 NUMA node per socket
  DF_DRAM_NPS_4,          ///< 4 NUMA node per socket
  DF_DRAM_NPS_AUTO = 7    ///< Auto
} DF_DRAM_NPS_VALIDVAL;

/// @brief DF_INVERT_DRAM_MAP
/// @details Type:UINT8
/// @li 0x00~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_DF_INVERT_DRAM_MAP                                 0x6574B2C0  // uint8

/// @brief DF_SPF_WAY_CTRL
/// @details Type:UINT8
/// @li 0x00~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_DF_SPF_WAY_CTRL                                    0x71094904  // uint8

/// @brief PCIE_RESET_PIN_SELECT
/// @details Type:UINT8
/// @li 0x00~0x4 = UITN8 Valid Range
  #define APCB_TOKEN_UID_PCIE_RESET_PIN_SELECT                              0x8C0B2DE9  // uint8

/// @brief UCLK DIV1 MODE
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_UCLK_DIV1_MODE                     0xF0947E37  // uint8

/// @brief VDDCR_SOC_VID
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDCR_SOC_VID                      0x4CAD640C  // uint8

/// @brief FCLK FREQUENCY
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_FCLK_FREQUENCY                     0xF1823F38  // uint8

/// @brief UCLK FREQUENCY
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_UCLK_FREQUENCY                     0x75C8500B  // uint8

/// @brief MEMCLK FREQUENCY
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_MEMCLK_FREQUENCY                   0xA31DB44A  // uint8

/// @brief DXIO Save Restore Modes
/// @details Type:UINT8
/// @li 0~0x3 = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_DBG_GNB_DXIO_SAVE_RESTORE_MODES                0x3039166E

/// @brief RCD PARITY
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_RCD_PARITY                                         0x647D7662

/// @brief DRAM Address Command Parity Retry
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_DRAM_ADDRESS_COMMAND_PARITY_RETRY                  0x3E7C51F8

/// @brief Max Parity Error Replay
/// @details Type:UINT8
/// @li 0~0x3f = UITN8 Valid Range
  #define APCB_TOKEN_UID_MAX_PARITY_ERROR_REPLAY                            0xC9E9A1C9

/// @brief Write CRC enable
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CTRLLER_WR_CRC                                 0x7D1C6E46

/// @brief DRAM Write CRC Enable and Retry Limit
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_DRAM_WRITE_CRC_ENABLE_AND_RETRY_LIMIT              0xC73A7692

/// @brief Max Write CRC Error Replay
/// @details Type:UINT8
/// @li 0~0x3f = UITN8 Valid Range
  #define APCB_TOKEN_UID_MAX_WRITE_CRC_ERROR_REPLAY                         0x6BB1ACF9

/// @brief PMU Pattern Bits Control
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_PATTERN_BITS_CTRL_DDR4             0x46D97109  // uint8

/// @brief PMU Pattern Bits
/// @details Type:UINT8
/// @li 0~0xA = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_PATTERN_BITS_DDR4                  0xC2FBEAFF  // uint8

/// @brief DF_PSTATE_MODE_SELECT
/// @details Type:UINT8
/// @li 0x00~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_DF_PSTATE_MODE_SELECT                              0xAEB84B12  // uint8
/** APCB_TOKEN_UID_DF_PSTATE_MODE_SELECT_VALIDVAL
*   Value range for APCB_TOKEN_UID_DF_PSTATE_MODE_SELECT
*/
typedef enum {
  GNB_SMU_DF_PSTATE_MODE_SELECT_NORMAL = 0,///<Normal
  GNB_SMU_DF_PSTATE_MODE_SELECT_LIMIT_HIGHEST = 1,///<Limit Highest
  GNB_SMU_DF_PSTATE_MODE_SELECT_LIMITALL = 2,///<Limit All
  GNB_SMU_DF_PSTATE_MODE_SELECT_AUTO = 0xFF,///<Auto
} GNB_SMU_DF_PSTATE_MODE_SELECT_VALIDVAL;

/// @brief RollWindowDepth
/// @details Type:UINT8
/// @li 01~0xff = UITN8 Valid Range
  #define APCB_TOKEN_UID_ROLLWINDOWDEPTH                                    0x5985083A

/// @brief DF_XGMI_TXEQ_MODE
/// @details Type:UINT8
/// @see DF_XGMI_TXEQ_MODE_VALIDVAL
  #define APCB_TOKEN_UID_DF_XGMI_TXEQ_MODE                                  0xADE79549  // uint8
/* DF_XGMI_TXEQ_MODE_VALIDVAL
 *  Value range for APCB_TOKEN_UID_DF_XGMI_TXEQ_MODE
 */
typedef enum {
  DF_XGMI_TXEQ_MODE_DISABLED = 0,
  DF_XGMI_TXEQ_MODE_ENABLED_BY_LANE,
  DF_XGMI_TXEQ_MODE_ENABLED_BY_LINK,
  DF_XGMI_TXEQ_MODE_ENABLED_BY_LINK_PLUS_RX_VETTING,
  DF_XGMI_TXEQ_MODE_AUTO = 0xFF
} DF_XGMI_TXEQ_MODE_VALIDVAL;

/// @brief eSPI Operating Clock
/// @details Type:UINT8
/// @li 0~0x2 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_CLOCK                                         0x58077CCD  // uint8
/** ESPI_CLOCKVAL
*   Value range for APCB_TOKEN_UID_ESPI_CLOCK
*/
typedef enum {
  ESPI_CLOCKVAL_16MHZ,            ///< 16.6MHz
  ESPI_CLOCKVAL_33MHZ,            ///< 33MHz
  ESPI_CLOCKVAL_66MHZ,            ///< 66MHz
} ESPI_CLOCKVAL;

/// @brief eSPI IO mode
/// @details Type:UINT8
/// @li 0~0x2 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IOMODE                                        0xF78C9C14  // uint8
/** ESPI_IOMODEVAL
*   Value range for APCB_TOKEN_UID_ESPI_IOMODE
*/
typedef enum {
  ESPI_IOMODEVAL_SINGLE,            ///< Single I/O
  ESPI_IOMODEVAL_DUAL,              ///< Dual I/O
  ESPI_IOMODEVAL_QUAD,              ///< Quad I/O
} ESPI_IOMODEVAL;

/// @brief eSPI Alert mode
/// @details Type:UINT8
/// @li 0~0x1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_ALERT_MODE                                    0xFED2F627  // uint8
/** ESPI_ALERT_MODE_VAL
*   Value range for APCB_TOKEN_UID_ESPI_ALERT_MODE
*/
typedef enum {
  ESPI_ALERT_MODE_VAL_NO_ALERT_PIN,            ///< I/O[1] pin is used to signal the Alert event
  ESPI_ALERT_MODE_VAL_ALERT_PIN,               ///< A dedicated Alert# pin is used to signal the Alert event
} ESPI_ALERT_MODE_VAL;

/// @brief eSPI IO Range0 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE0_SIZE                                 0x506C4570  // uint8

/// @brief eSPI IO Range1 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE1_SIZE                                 0x3308F8E4  // uint8

/// @brief eSPI IO Range2 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE2_SIZE                                 0x8B45D00A  // uint8

/// @brief eSPI IO Range3 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE3_SIZE                                 0xACAA0899  // uint8

/// @brief eSPI IO Range4 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE4_SIZE                                 0x1560AFCB  // uint8

/// @brief eSPI IO Range5 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE5_SIZE                                 0x3B223BF1  // uint8

/// @brief eSPI IO Range6 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE6_SIZE                                 0x7801E1AB  // uint8

/// @brief eSPI IO Range7 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE7_SIZE                                 0xE47EC687  // uint8

/// @brief eSPI IO Range8 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE8_SIZE                                 0xA6200CAD  // uint8

/// @brief eSPI IO Range9 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE9_SIZE                                 0xE705933A  // uint8

/// @brief eSPI IO Range10 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE10_SIZE                                0xC70B513B  // uint8

/// @brief eSPI IO Range11 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE11_SIZE                                0x179833A8  // uint8

/// @brief eSPI IO Range12 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE12_SIZE                                0xD59C6E95  // uint8

/// @brief eSPI IO Range13 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE13_SIZE                                0xEA00A451  // uint8

/// @brief eSPI IO Range14 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE14_SIZE                                0x51167E2C  // uint8

/// @brief eSPI IO Range15 Size
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE15_SIZE                                0xF2327F4F  // uint8

/// @brief eSPI Clock pin selection
/// @details Type:UINT8
/// @li 0~0x1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_CLOCK_PIN_SELECTION                           0x7762A4EF  // uint8
/** ESPI_CLOCK_PIN_SELECTION_VAL
*   Value range for APCB_TOKEN_UID_ESPI_CS_SELECTION
*/
typedef enum {
  ESPI_CLOCK_PIN_SELECTION_VAL_SPI_CLOCK,                ///< Use SPI clock
  ESPI_CLOCK_PIN_SELECTION_VAL_GPIO86,                  ///< Use GPIO86
} ESPI_CLOCK_PIN_SELECTION_VAL;

/// @brief eSPI Data Pin set
/// @details Type:UINT8
/// @li 0~0x1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_DATA_SELECTION                                0x59994737  // uint8
/** ESPI_DATA_SELECTION_VAL
*   Value range for APCB_TOKEN_UID_ESPI_DATA_SELECTION
*/
typedef enum {
  ESPI_DATA_SELECTION_VAL_SPI_DATA1,               ///< Use SPI_DATA1
  ESPI_DATA_SELECTION_VAL_SPI_DATA2,               ///< Use SPI_DATA2
} ESPI_DATA_SELECTION_VAL;

/// @brief GPIO pin for eSPI Data Pin selection
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO                                 0x1773FBE3  // uint8

/// @brief GPIO pin for eSPI Data Pin Value
/// @details Type:UINT8
/// @li 0~0x1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO_OUTPUT                          0x69EC8C7E  // uint8
/** ESPI_DATA_SEL_GPIO_OUTPUT_VAL
*   Value range for APCB_TOKEN_UID_ESPI_DATA_SEL_GPIO_OUTPUT
*/
typedef enum {
  ESPI_DATA_SEL_GPIO_OUTPUT_VAL_LOW,               ///< Output Low
  ESPI_DATA_SEL_GPIO_OUTPUT_VAL_HIGH,              ///< Output High
} ESPI_DATA_SEL_GPIO_OUTPUT_VAL;

/// @brief eSPI CS pin selection
/// @details Type:UINT8
/// @li 0~0x1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_CS_SELECTION                                  0x84EF2C57  // uint8
/** ESPI_CS_SELECTION_VAL
*   Value range for APCB_TOKEN_UID_ESPI_CS_SELECTION
*/
typedef enum {
  ESPI_CS_SELECTION_VAL_GPIO30,                ///< Use GPIO30 as eSPI_CS
  ESPI_CS_SELECTION_VAL_GPIO31,                ///< Use GPIO31 as eSPI_CS
} ESPI_CS_SELECTION_VAL;

/// @DESC: I2C 0 RxSel Schmit Trigger 0 - 3.3V, 1 - 1.8v, 2 - 1.1v
/// @RANGE: 0x00~0x02
  #define APCB_TOKEN_UID_I2C_0_RX_SEL                                       0x845741F6  // uint8
/// @DESC: I2C 1 RxSel Schmit Trigger 0 - 3.3V, 1 - 1.8v, 2 - 1.1v
/// @RANGE: 0x00~0x02
  #define APCB_TOKEN_UID_I2C_1_RX_SEL                                       0x29BF8CD7  // uint8
/// @DESC: I2C 2 RxSel Schmit Trigger 0 - 3.3V, 1 - 1.8v, 2 - 1.1v
/// @RANGE: 0x00~0x02
  #define APCB_TOKEN_UID_I2C_2_RX_SEL                                       0x2D61B4AF  // uint8
/// @DESC: I2C 3 RxSel Schmit Trigger 0 - 3.3V, 1 - 1.8v, 2 - 1.1v
/// @RANGE: 0x00~0x01
  #define APCB_TOKEN_UID_I2C_3_RX_SEL                                       0x9F2BBA39  // uint8
/// @DESC: I2C 4 RxSel Schmit Trigger 0 - 3.3V, 1 - 1.8v, 2 - 1.1v
/// @RANGE: 0x00~0x01
  #define APCB_TOKEN_UID_I2C_4_RX_SEL                                       0xA14EE34C  // uint8
/// @DESC: I2C 5 RxSel Schmit Trigger 0 - 3.3V, 1 - 1.8v, 2 - 1.1v
/// @RANGE: 0x00~0x01
  #define APCB_TOKEN_UID_I2C_5_RX_SEL                                       0x607C8A7D  // uint8

typedef enum {
  I2C_RX_SEL_SCHMIT_TRIGGER_3_3V,                ///< 3.3V schmit trigger
  I2C_RX_SEL_SCHMIT_TRIGGER_1_8V,                ///< 1.8V schmit trigger
  I2C_RX_SEL_SCHMIT_TRIGGER_1_1V                 ///< 1.1V schmit trigger
} I2C_RX_SEL_SCHMIT_TRIGGER;

/// @brief DF_SYS_STORAGE_AT_TOP_OF_MEM
/// @details Type:UINT8
/// @see DF_SYS_STORAGE_AT_TOP_OF_MEM_VALIDVAL
  #define APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM                       0x249E08D5  // uint8
/* DF_SYS_STORAGE_AT_TOP_OF_MEM_VALIDVAL
 *  Value range for APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM
 */
typedef enum {
  DF_SYS_STORAGE_AT_TOP_OF_MEM_DISTRIBUTED = 0,
  DF_SYS_STORAGE_AT_TOP_OF_MEM_CONSOLIDATED = 1,
  DF_SYS_STORAGE_AT_TOP_OF_1ST_MEM_CONSOLIDATED = 2,
  DF_SYS_STORAGE_AT_TOP_OF_MEM_AUTO = 0xFF
} DF_SYS_STORAGE_AT_TOP_OF_MEM_VALIDVAL;

/// @brief The number of training failure/retries required before boot from recovery mode
/// @details Type:UINT8
/// @li 0~10 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_OVERCLOCK_FAIL_CNT                             0xE5325C7B  //uint8

/// @brief MEM_CTRLLER_MR6_VREF_DQ_CTRL_DDR4
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_MR6_VREF_DQ_CTRL_DDR4                  0x8738783C  // uint8

/// @brief MEM_CTRLLER_MR6_VREF_DQ_DDR4
/// @details Type:UINT8
/// @li 0x00~0x7F = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CTRLLER_MR6_VREF_DQ_DDR4                       0x6649F4D1  // uint8

/// @brief MEM_CTRLLER_CPU_VREF_TRAINING_SEED_CTR
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_CPU_VREF_TRAINING_SEED_CTRL_DDR4       0x6C95D783  // uint8

/// @brief MEM_CTRLLER_CPU_VREF_TRAINING_SEED_DDR
/// @details Type:UINT8
/// @li 0x00~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CTRLLER_CPU_VREF_TRAINING_SEED_DDR4            0xC7389840  // uint8

/// @brief PSP_RPMC_ENABLE
/// @details Type:UINT8
/// @li 0x00~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_PSP_RPMC_ENABLE                                    0x4967F4FC  // uint8
/* PSP_RPMC_ENABLE
 *  Value range for APCB_TOKEN_UID_PSP_RPMC_ENABLE
 */
typedef enum {
  PSP_RPMC_USAGE_DISABLE = 0x00, ///< Control RPMC usage, disable RPMC usage
  PSP_RPMC_USAGE_ENABLE  = 0x01, ///< Control RPMC usage, enable RPMC usage
} PSP_RPMC_SWITCH_VAL;

/// @DESC: PSP_RPMC_RECOVERY_TYPE
/// @RANGE: 0x00~0xFF = UITN8 Valid Range
/// @details Type:UINT8, currently only 0 or 1 is acceptable
/// 0 : halt the system
/// 1 : set the recovery flag to notify BIOS
  #define APCB_TOKEN_UID_RPMC_RECOVERY_TYPE                                 0xCFEB85E2

/// @brief SYSTEM_TPM_CONFIG
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG                              0x016DA8CA
/* SYSTEM_TPM_CONFIG_VAL
 *  Value range for APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG
 */
typedef enum {
  SYSTEM_TPM_CONFIG_DTPM     = 0x00, ///< dTPM
  SYSTEM_TPM_CONFIG_PSP_FTPM = 0x01, ///< PSP FTPM
  SYSTEM_TPM_CONFIG_HSP_FTPM = 0x02, ///< HSP FTPM
  SYSTEM_TPM_CONFIG_NONE     = 0xFF, ///< None of TPM
} SYSTEM_TPM_CONFIG_VAL;

/// @brief SYSTEM_TPM_ALG_SMX
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX                             0x87EE0442
/* SYSTEM_TPM_ALG_VAL
 *  Value range for APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX
 */
typedef enum {
  SYSTEM_TPM_ALG_SMX_UNSUPPORTED = 0x00, ///< Doesn't support SM2, SM3 and SM4 crypto algorithms
  SYSTEM_TPM_ALG_SMX_SUPPORTED   = 0x01, ///< Fully support SM2, SM3 and SM4 crypto algorithms
} SYSTEM_TPM_ALG_VAL;

/// @brief PSP_FTPM_NVSTORAGE_SIZE
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_PSP_FTPM_NVSTORAGE_SIZE                            0xD085E8C2
/* PSP_FTPM_NVSTORAGE_SIZE_VAL
 *  Value range for APCB_TOKEN_UID_PSP_FTPM_NVSTORAGE_SIZE
 */
typedef enum {
  PSP_FTPM_NVSTORAGE_16KB     = 0x00, ///< 16KB actual NV storage for PSP-fTPM
  PSP_FTPM_NVSTORAGE_EXTENDED = 0x01, ///< 32KB and up actual NV storage for PSP-fTPM
} PSP_FTPM_NVSTORAGE_SIZE_VAL;

/// @brief MEM_DDR5_SPDHUB_CONFIG
/// @details Type:UINT8
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_DDR5_SPDHUB_CONFIG                             0x264EC130
/* DDR5_SPDHUB_CONFIG_VAL
 *  Value range for APCB_TOKEN_UID_MEM_DDR5_SPDHUB_CONFIG
 */
typedef enum {
  DDR5_SPDHUB_CONFIG_SMBUS   = 0x00, ///< Use SMBUS interface
  DDR5_SPDHUB_CONFIG_I2C     = 0x01, ///< Use I2C interface
  DDR5_SPDHUB_CONFIG_I3C     = 0x03, ///< Use I3C interface
  DDR5_SPDHUB_CONFIG_NONE    = 0xFF, ///< Do not use SPD HUB
} DDR5_SPDHUB_CONFIG_VAL;

/// @brief ACTION_ON_BIST_FAILURE
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ACTION_ON_BIST_FAILURE                             0xCBC2C0DD  // uint8

/// @brief RX_DAT_CHN_DLY
/// @details Type:UINT8
/// @li 0~1 = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_DBG_MEM_RX_DAT_CHN_DLY_DDR4                    0x1AB002CD  // uint8

/// @brief Override DIMM SPD Byte 7 [3:0] Maximum Activate Count
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MAXACTIVATECOUNT                               0x0853CDAA  // uint8

/// @brief CBS_CMN_MEM_CTRLLER_BANK_SWIZZLE_DDR
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_SWIZZLE_DDR               0xBF78F843

/// @brief CBS_CMN_MEM_ADDRESS_HASHING_DDR
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASHING_DDR                    0x3401D057

/// @brief CBS_CMN_MEM_TSME_DDR
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TSME_DDR                               0x69495CCF

/// @brief CBS_CMN_MEM_TIMING_TRCPAGE_CTRL_DDR
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_CTRL_DDR                0xC7A73078

/// @brief CBS_CMN_MEM_TIMING_TRCPAGE_DDR
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_DDR                     0xA74DD140

/// @brief CBS_CMN_MEM_TIMING_TRFC4_CTRL_DDR
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC4_CTRL_DDR                  0xE43E804C

/// @brief CBS_CMN_MEM_CTRLLER_PROC_ODT_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_PROC_ODT_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PROC_ODT_DDR                   0xFA375429

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_PROC_ODT_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_PROC_ODT_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_PROC_ODT_DDR              0xA87EB112  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_PROC_ODT_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_PROC_ODT_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PROC_ODT_DDR                                   0xB88F40C8  //uint8

/// @brief CBS_DBG_MEM_DQ_DRV_DDR
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_DQ_DRV_DDR                                     0x62985EDE

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_PROC_DATA_DRV_STREN_DDR
/// @details Type:UINT8
/// @li AUTO_HIGHIMP_480_25.3_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_PROC_DATA_DRV_STREN_DDR   0x8D8EF02B  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_DRAM_DATA_DRV_STREN_DDR
/// @details Type:UINT8
/// @li AUTO_HIGHIMP_480_25.3_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_DRAM_DATA_DRV_STREN_DDR   0xBF7F5D46  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR                0xE335CE83

/// @brief CBS_CMN_MEM_CTRLLER_RTT_NOM_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_NOM_DDR                    0x1FDB5F4F

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_NOM_WR_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_RTT_NOM_WR_DDR            0x16ABBD88  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_NOM_WR_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RTT_NOM_WR_DDR                                 0xA085FAD9  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_NOM_RD_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_RTT_NOM_RD_DDR            0xC54625F1  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_NOM_RD_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_NOM_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RTT_NOM_RD_DDR                                 0x668AC7E7  //uint8

/// @brief CBS_CMN_MEM_CTRLLER_RTT_WR_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_WR_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_WR_DDR                     0x27E462C3

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_WR_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_WR_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_RTT_WR_DDR                0xA7E3B203  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_WR_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_WR_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RTT_WR_DDR                                     0x29AD741F  //uint8

/// @brief CBS_CMN_MEM_CTRLLER_RTT_PARK_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_RTT_PARK_DDR                   0x410489B5

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_PARK_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_RTT_PARK_DDR              0x7401B5A7  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_PARK_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RTT_PARK_DDR                                   0xDF253FE7  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_PARK_DQS_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_RTT_PARK_DQS_DDR          0xF578C986  //uint8

/// @brief CBS_CMN_MEM_DATA_BUS_CFG_RTT_PARK_DQS_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_RTT_PARK_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RTT_PARK_DQS_DDR                               0xFFEDC3F7  //uint8

/// @brief CBS_CMN_MEM_TIMING_TCKE_DDR
/// @details Type:UINT8
/// @see MEM_TIMING_TCKE_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCKE_DDR                        0x60B493BF

/// @brief CBS_CMN_MEM_CTRLLER2_T_MODE_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER2_T_MODE_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER2_T_MODE_DDR                    0x5D0B9C09

/// @brief CBS_CMN_MEM_CAD_BUS_TIMING_CTL_DDR
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_TIMING_CTL_DDR                 0xCFB272BC

/// @brief CBS_CMN_MEM_ADDR_CMD_SETUP_DDR
/// @details Type:UINT8
/// @li 0x00~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_SETUP_DDR                     0x9D423CBF

/// @brief CBS_CMN_MEM_CS_ODT_SETUP_DDR
/// @details Type:UINT8
/// @li 0x00~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CS_ODT_SETUP_DDR                       0xABB0BEE9

/// @brief CBS_CMN_MEM_CKE_SETUP_DDR
/// @details Type:UINT8
/// @li 0x00~0x3F = UITN8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CKE_SETUP_DDR                          0xC4F3518B

/// @brief CBS_CMN_MEM_CAD_BUS_DRV_STREN_CTL_DDR
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_DRV_STREN_CTL_DDR              0x29C6AE21

/// @brief MEM_CLK_DRV_DDR
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CLK_DRV_DDR                                    0x275C400A

/// @brief CBS_CMN_MEM_CAD_BUS_CLK_DRV_STREN_DDR
/// @details Type:UINT8
/// @see MEM_CAD_BUS_CLK_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CLK_DRV_STREN_DDR              0x5B0E38A9

/// @brief MEM_CA_DRV_DDR
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CA_DRV_DDR                                     0x82648A4F

/// @brief CBS_CMN_MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR
/// @details Type:UINT8
/// @see MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_ADDR_CMD_DRV_STREN_DDR         0x115E63E1

/// @brief MEM_CS_DRV_DDR
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CS_DRV_DDR                                     0x4891D431

/// @brief CBS_CMN_MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR
/// @details Type:UINT8
/// @see MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CS_ODT_DRV_STREN_DDR           0x5FC2ED30

/// @brief CBS_CMN_MEM_CAD_BUS_CKE_DRV_STREN_DDR
/// @details Type:UINT8
/// @see MEM_CAD_BUS_CKE_DRV_STREN_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_CKE_DRV_STREN_DDR              0x61516BAE

/// @brief CBS_CMN_MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_ECC_SYMBOL_SIZE_DDR       0xB93247AE

/// @brief MEM_ADDR_CMD_PARITY_ERROR_MAX_REPLAY_D
/// @details Type:UINT8
/// @li 0x00~0x3F = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_ERROR_MAX_REPLAY_DDR   0x6A714420

/// @brief MEM_ADDR_CMD_PARITY_RETRY_DDR
/// @details Type:UINT8
/// @see MEM_ADDR_CMD_PARITY_RETRY_DDR4_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_PARITY_RETRY_DDR              0xA4FAB841

/// @brief MEM_WRITE_CRC_ERROR_MAX_REPLAY_DDR
/// @details Type:UINT8
/// @li 0x01~0x08 = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_WRITE_CRC_ERROR_MAX_REPLAY_DDR         0x430E5465

/// @brief PMU Pattern Bits Control
/// @details Type:UINT8
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_PATTERN_BITS_CTRL_DDR              0x960F8ACC

/// @brief PMU Pattern Bits
/// @details Type:UINT8
/// @li 0~0xA = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CTRLLER_PMU_PATTERN_BITS_DDR                   0xE57A2D86

/// @brief MEM_CTRLLER_MR6_VREF_DQ_CTRL_DDR4
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_MR6_VREF_DQ_CTRL_DDR                   0x786A38B8

/// @brief MEM_CTRLLER_MR6_VREF_DQ_DDR4
/// @details Type:UINT8
/// @li 0x00~0x7F = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_CTRLLER_MR6_VREF_DQ_DDR                        0x2C4105BA

/// @brief RX_DAT_CHN_DLY
/// @details Type:UINT8
/// @li 0~1 = UINT8 Valid Range
  #define APCB_TOKEN_UID_CBS_DBG_MEM_RX_DAT_CHN_DLY_DDR                     0x1B1898A9

/// @brief Refresh Management Enable
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_RFM_ENABLE                             0xC225771F

/// @brief Tcksrx += (Trfc/n * (UMC_Number % 4))), n = 3, 4
/// @details Type:UINT8
/// @li 0, 3 or 4
  #define APCB_TOKEN_UID_MEM_SELF_REFRESH_EXIT_STAGGERING                   0xBC52E5F7

/// @brief POWER_SENSORS_ROUTING_SELECT
/// @details Type:UINT8
/// @li UINT8_0x00~0x01_WALLE_or_APU_MP2
  #define APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT                  0x80344623

/// @brief WORKLOAD_PROFILE
/// @details Type:UINT8
/// @li 0~18 = UINT8 Valid Range
  #define APCB_TOKEN_UID_WORKLOAD_PROFILE                                   0x22F4299F

/// @brief MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR            0x2186BE3E
/** MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR
*/
typedef enum {
  MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_0 = 0,///<0
  MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_1 = 1,///<1
  MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_AUTO = 0xFF,///<Auto
} MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_VALIDVAL;

/// @brief MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR
/// @details Type:UINT8
/// @see MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR              0x281B13F8
/** MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR
*/
typedef enum {
  MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_0 = 0,///<0
  MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_1 = 1,///<1
  MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_AUTO = 0xFF,///<Auto
} MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_VALIDVAL;

/// @brief APCB_TOKEN_UID_MEM_TIMING_LPDLY_LPDDR
/// @details Type:UINT8
/// @li 1~31 = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_LPDLY_LPDDR                             0x9EA78C5A

/// @brief APCB_TOKEN_UID_MEM_TIMING_LPEXITDLY_LPDDR
/// @details Type:UINT8
/// @li 1~63 = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_LPEXITDLY_LPDDR                         0xD2227C5B

/// @brief MEM_DRAM_IO_ODT_STREN_CA_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_ODT_STREN_CA_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CA_LPDDR                     0x702A0139
/** MEM_DRAM_IO_ODT_STREN_CA_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CA_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_ODT_STREN_CA_LPDDR_VALIDVAL;

/// @brief MEM_DRAM_IO_ODT_STREN_CK_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_ODT_STREN_CK_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CK_LPDDR                     0x9C937893
/** MEM_DRAM_IO_ODT_STREN_CK_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CK_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_ODT_STREN_CK_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_ODT_STREN_CK_LPDDR_VALIDVAL;

/// @brief MEM_DRAM_IO_ODT_STREN_CS_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_ODT_STREN_CS_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CS_LPDDR                     0xAF7E855E
/** MEM_DRAM_IO_ODT_STREN_CS_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CS_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_ODT_STREN_CS_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_ODT_STREN_CS_LPDDR_VALIDVAL;

/// @brief MEM_DRAM_IO_ODT_STREN_WCK_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR                    0x4A1F6DC4
/** MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_VALIDVAL;

/// @brief MEM_DRAM_IO_ODT_STREN_DQ_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR                     0x86B135CF
/** MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_VALIDVAL;

/// @brief MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR                   0x20A3F92D
/** MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR_VALIDVAL;

/// @brief MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR
/// @details Type:UINT8
/// @see MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR                   0xF673E171
/** MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR
*/
typedef enum {
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_AUTO = 0xFF, ///<Auto
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ0 = 0,    ///<Disable
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ1 = 1,
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ2 = 2,
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ3 = 3,
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ4 = 4,
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ5 = 5,
  MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_RZQ6 = 6,
} MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR_VALIDVAL;

/// @brief CBS_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_ECC_DIS_ERR_INJECTION_DDR                      0xF77A05F1

/// @brief CBS_CMN_MEM_POWER_DOWN_EN_DDR
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_CTRLLER_POWER_DOWN_EN_DDR                      0x6E6B60F7

/// @brief CBS_CMN_TSME_ENABLE_DDR
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_TSME_ENABLE_DDR                                0xF0869ECA

/// @brief RRW CTRL
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_RRW_MEMTEST_CTRL_DDR                           0xD0C34BDD

/// @brief RRW CTRL
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_RRW_MEMTEST_CTRL_LPDDR                         0xD5958BA4

/// @brief CBS_DBG_MEM_DRV_IMP_DDR
/// @details Type:UINT8
/// @see UINT8_MANUAL_AUTO_0XFF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_DRV_IMP_DDR                                    0xC4765F1F

/// @brief CBS_DBG_MEM_PHY_PLL_BYPASS_DDR
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_PHY_PLL_BYPASS_DDR                             0xD60CD394

/// @brief FCH spread spectrum
/// @details Type:UINT8
/// @li 0x00~0x01 = UINT8 Valid Range
  #define APCB_TOKEN_UID_FCH_ACPI_SPREAD_SPECTRUM                           0x4367CBD2

/// @brief ODTL Timing Sync Mode
/// @details Type:UINT8
/// @see MEM_ODTL_TIMING_SYNC_VALIDVAL
  #define APCB_TOKEN_UID_MEM_ODTL_TIMING_SYNC_DDR                           0x28C76B90
/** MEM_ODTL_TIMING_SYNC_VALIDVAL_VALIDVAL
*   Values for APCB_TOKEN_UID_MEM_ODTL_TIMING_SYNC_DDR
*/
typedef enum {
  MEM_ODTL_TIMING_SYNC_DDR_AUTO     = 0xFF, ///<Auto
  MEM_ODTL_TIMING_SYNC_DDR_DIASBLE  = 0,    ///<Default operation
  MEM_ODTL_TIMING_SYNC_DDR_ENABLE   = 1,    ///<Enable ODTL sync mode
} MEM_ODTL_TIMING_SYNC_VALIDVAL;

/// @brief DRAM PDA Enumerate ID Programming Mode
/// @details Type:UINT8
/// @see MEM_DRAM_PDA_ENUM_ID_PROG_MODE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR                 0x2BF8781F
/** MEM_DRAM_PDA_ENUM_ID_PROG_MODE_VALIDVAL
*   Values for APCB_TOKEN_UID_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR
*/
typedef enum {
  MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_AUTO     = 0xFF, ///<Auto
  MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_TOGGLING = 0,    ///<Continuous DQS toggling PDA enumeration mode
  MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_LEGACY   = 1,    ///<Legacy PDA enumeration mode
} MEM_DRAM_PDA_ENUM_ID_PROG_MODE_VALIDVAL;

/// @brief MEM_REFRESH_MODE_LPDDR
/// @details Type:UINT8
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_REFRESH_MODE_LPDDR                             0xB39F1A71

/// @brief MEM_RFM_EN_LPDDR
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RFM_EN_LPDDR                                   0xAFA7DC53

/// @brief MEM_WCK_ALWAYS_ON_EN_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_WCK_ALWAYS_ON_EN_LPDDR                         0xD22366B6

/// @brief MEM_DVFSC_EN_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DVFSC_EN_LPDDR                                 0x384C0404

/// @brief MEM_DVFSQ_EN_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DVFSQ_EN_LPDDR                                 0x7315D0A3

/// @brief MEM_DRAM_RD_LINK_ECC_EN_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RD_LINK_ECC_EN_LPDDR                           0x31477340

/// @brief MEM_DRAM_WR_LINK_ECC_EN_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_WR_LINK_ECC_EN_LPDDR                           0x03F0E174

/// @brief MEM_DIS_MEM_ERR_INJ_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DIS_MEM_ERR_INJ_LPDDR                          0xDDA755D1

/// @brief MEM_TSME_ENABLE_LPDDR
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_TSME_ENABLE_LPDDR                              0xFD5F7B2C

/// @brief MEM_DATA_SCRAMBLE_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DATA_SCRAMBLE_LPDDR                            0x0BDCD99F

/// @brief MEM_CS_INTERLEAVE_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_CS_INTERLEAVE_LPDDR                            0x9BCA7E95

/// @brief MEM_BANK_SWAP_ENABLE_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_BANK_SWAP_ENABLE_LPDDR                         0x23C93AD4

/// @brief MEM_BANK_GROUP_SWAP_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_BANK_GROUP_SWAP_LPDDR                          0x76606793

/// @brief MEM_ADDRESS_HASH_BANK_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_ADDRESS_HASH_BANK_LPDDR                        0xFC8EC75F

/// @brief MEM_ADDRESS_HASH_CS_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_ADDRESS_HASH_CS_LPDDR                          0x1BE96E84

/// @brief MEM_ADDRESS_HASH_RM_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_AUTO_VALIDVAL
  #define APCB_TOKEN_UID_MEM_ADDRESS_HASH_RM_LPDDR                          0xBB2B9B2E

/// @brief MEM_ABL_STOP_ON_FAILURE_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_ABL_STOP_ON_FAILURE_LPDDR                      0x5CC75EF9

/// @brief MEM_PMU_TRAIN_DFE_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_PMU_TRAIN_DFE_LPDDR                            0x120FF492

/// @brief MEM_FORCE_DATA_MASK_DISABLE_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_FORCE_DATA_MASK_DISABLE_LPDDR                  0xB41B384B

/// @brief MEM_RESTORE_CTL_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RESTORE_CTL_LPDDR                              0xDBDD3CA0

/// @brief MEM_DRAM_ECC_EN_LPDDR
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAM_ECC_EN_LPDDR                              0xB4C8DCA4

/// @brief ENABLECHANNELINTLV
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_ENABLECHANNELINTLV_LPDDR                       0x643AA6E2

/// @brief Phy Power Saving
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_PHY_POWER_SAVING_DDR                           0xD911315B  //uint8

/// @brief AUTOREFFINEGRANMODE
/// @details Type:UINT8
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_AUTOREFFINEGRANMODE_LPDDR                      0xB4F4F68E

/// @brief MEM_TRAINING_HDTCTRL_LPDDR
/// @details Type:UINT8
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL_LPDDR                         0xE603AC2C

/// @brief DRAM Double Refresh Rate
/// @details Type:UINT8
/// @see MEM_DRAM_REFRESH_RATE_VALIDVAL
  #define APCB_TOKEN_UID_MEM_DRAMDOUBLEREFRESHRATE_LPDDR                    0xE3137850

/// @brief MBIST Verbose Logging Enable
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_ENABLE_VERBOSE_LOGGING_DDR               0x93925F86

/// @brief ODTLon_WR_Offset
/// @details Type:UINT8
/// @li 1~7
  #define APCB_TOKEN_UID_MEM_ODTLONWROFFSET_DDR                             0xE8CF84A7

/// @brief ODTLoff_WR_Offset
/// @details Type:UINT8
/// @li 1~7
  #define APCB_TOKEN_UID_MEM_ODTLOFFWROFFSET_DDR                            0xA6382EA3

/// @brief ODTLon_WR_NT_Offset
/// @details Type:UINT8
/// @li 1~7
  #define APCB_TOKEN_UID_MEM_ODTLONWRNTOFFSET_DDR                           0x28E1DBD9

/// @brief ODTLoff_WR_NT_Offset
/// @details Type:UINT8
/// @li 1~7
  #define APCB_TOKEN_UID_MEM_ODTLOFFWRNTOFFSET_DDR                          0x68AF0155

/// @brief ODTLon_RD_NT_Offset
/// @details Type:UINT8
/// @li 2~7
  #define APCB_TOKEN_UID_MEM_ODTLONRDNTOFFSET_DDR                           0x9F6F9C2F

/// @brief ODTLoff_RD_NT_Offset
/// @details Type:UINT8
/// @li 2~7
  #define APCB_TOKEN_UID_MEM_ODTLOFFRDNTOFFSET_DDR                          0x56FE44AD

/// @brief DDR RMP Enable
/// @details Type:UINT8
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RMP_ENABLE_DDR                                 0xE753A04C

/** BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
*   Value range for boolean type with Enable and Disable usage
*/
typedef enum {
  RMP_PROFILE_INDEX_0 = 0,   ///< RMP Profile 0
  RMP_PROFILE_INDEX_1 = 1,   ///< RMP Profile 1
  RMP_PROFILE_AUTO = 0xFF,   ///< RMP Frofile Auto
} RMP_PROFILE_0_PROFILE_1_AUTO_0xFF_VALIDVAL;
/// @brief DDR RMP Profile Index
/// @details Type:UINT8
/// @see RMP_PROFILE_0_PROFILE_1_AUTO_0xFF_VALIDVAL
  #define APCB_TOKEN_UID_MEM_RMP_PROFILE_INDEX_DDR                          0x244E47D0

/// @brief Memory Clock Enabling/Disabling during PHY Low Power Mode
/// @details Type: UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_MEM_CLK_DISABLE_IN_PHY_LOW_POWER_MODE_LPDDR        0x44B6AEC5

/// @brief Memory Clock Disable during PHY Low Power Mode
/// @details Type: UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_MEM_CLK_DISABLE_IN_PHY_LOW_POWER_MODE_DDR          0x30587849

/// @brief Enable DFI data low power requests during PHY Low Power Mode
/// @details Type: UINT8
/// @see ENABLE_DISABLE_AUTO_OPTION
  #define APCB_TOKEN_UID_MEM_DFI_DATA_LP_REQ_IN_PHY_LOW_POWER_MODE_DDR      0xBAD028FF

/// @brief MEM_ACTIVE_MEM_TIMING_SETTING_DDR
/// @details Type:UINT8
/// @li 1 = Enable
  #define APCB_TOKEN_UID_MEM_ACTIVE_MEM_TIMING_SETTING_DDR                  0x4E2F0DDE

/// @brief MEM_PHY_CALIBRATION_INTERVAL_LPDDR
/// @details Type:UINT8
/// @li 3:0 CalInterval: In mS. csrValue : Interval (mS) - 0 : 0
///     - 1 : 0.013 - 2 : 0.10 - 3 : 1 - 4 : 2 - 5 : 3 - 6
///     : 4 - 7 : 8 - 8 : 10 - 9 : 20 - 10-15 : Reserved
  #define APCB_TOKEN_UID_MEM_PHY_CALIBRATION_INTERVAL_LPDDR                 0x70B7878A

// =================================================================================================
// CBS CMN APCB Tokens (2BYTE)
// -------------------------------------------------------------------------------------------------
/// @GROUPID: APCB_GROUP_TOKEN
/// @TYPEID: APCB_TYPE_TOKEN_2BYTES
/// @DATATYPE: UINT16
/// @SIZE: 2

/// @brief SCRUBDRAMRATE
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_SCRUBDRAMRATE                                      0x9ADDDD6B  // uint16 //Obsolete

/// @brief SCRUBL2RATE
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_SCRUBL2RATE                                        0x2266C144  // uint16 //Obsolete

/// @brief SCRUBL3RATE
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_SCRUBL3RATE                                        0xC0279AE0  // uint16 //Obsolete

/// @brief SCRUBICRATE
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_SCRUBICRATE                                        0x99639EE4  // uint16 //Obsolete

/// @brief SCRUBDCRATE
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_SCRUBDCRATE                                        0xB398DAA0  // uint16 //Obsolete

/// @brief ECCSYMBOLSIZE
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ECCSYMBOLSIZE                                      0x302D5C04  // uint16 //Obsolete

/// @brief DIMMSENSORCONF
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_DIMMSENSORCONF                                     0x51E7B610  // uint16 //Obsolete

/// @brief DIMMSENSORUPPER
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_DIMMSENSORUPPER                                    0xB5AF557A  // uint16 //Obsolete
//
/// @brief DIMMSENSORUPPER3DS
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_3DSDIMMSENSORUPPER                                 0x2DB877E4  // uint16 //Obsolete

/// @brief DIMMSENSORLOWER
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_DIMMSENSORLOWER                                    0xC5EA38A0  // uint16 //Obsolete

/// @brief DIMMSENSORCRITICAL
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_DIMMSENSORCRITICAL                                 0x38E9BF5D  // uint16 //Obsolete

/// @brief DIMMSENSORCRITICAL3DS
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_3DSDIMMSENSORCRITICAL                              0x16B77F73  // uint16 //Obsolete

/// @brief TEST
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_TEST                                               0x11D0001E  // uint16 //Obsolete

/// @brief CBS_CMN_MEM_TIMING_TRFC_DDR4
/// @details Type:UINT16
/// @see MANUAL_AUTO_0X00_VALIDVAL
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC_DDR4                       0x49714C55  // uint16 //Obsolete

/// @brief CBS_CMN_MEM_TIMING_TRFC2_DDR4
/// @details Type:UINT16
/// @li 0x003C~0x03DE = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC2_DDR4                      0xEF6CF7B3  // uint16 //uint16

/// @brief CBS_CMN_MEM_TIMING_TRFC4_DDR4
/// @details Type:UINT16
/// @li 0x003C~0x03DE = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC4_DDR4                      0x4A05C5E2  // uint16 //uint16

/// @brief PSP_SYSHUB_WDT_INTERVAL
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_PSP_SYSHUB_WDT_INTERVAL                            0xEDB5E4C9  // uint16

/// @brief OVERCLOCK_VDDP_VOLTAGE
/// @details Type:UINT16
/// @li 0x0000~0x07FF = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDP_VOLTAGE                       0xAB4BABF4  // uint16

/// @brief OVERCLOCK_VDDG_VOLTAGE
/// @details Type:UINT16
/// @li 0x0000~0x07FF = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDG_VOLTAGE                       0xEE9F031C  // uint16

/// @brief OVERCLOCK_IOD_VDDG_VOLTAGE
/// @details Type:UINT16
/// @li 0x0000~0x07FF = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE                   0x93CA7EA3  // uint16

/// @brief OVERCLOCK_CCD_VDDG_VOLTAGE
/// @details Type:UINT16
/// @li 0x0000~0x07FF = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE                   0xC286E57A  // uint16

/// @brief MEM_ADDRESS_HASH_BANK_2_COL_XOR
/// @details Type:UINT16
/// @li 0x0000~0x1FFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_ADDRESS_HASH_BANK_2_COL_XOR                    0x7A2B20AE

/// @brief eSPI IO Range0 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE0_BASE                                 0xA1B71439  // uint16

/// @brief eSPI IO Range1 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE1_BASE                                 0x10397E9A  // uint16

/// @brief eSPI IO Range2 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE2_BASE                                 0x632A9B05  // uint16

/// @brief eSPI IO Range3 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE3_BASE                                 0x9DC3A759  // uint16

/// @brief eSPI IO Range4 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE4_BASE                                 0x0EF919F3  // uint16

/// @brief eSPI IO Range5 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE5_BASE                                 0x599510A4  // uint16

/// @brief eSPI IO Range6 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE6_BASE                                 0x050E0264  // uint16

/// @brief eSPI IO Range7 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE7_BASE                                 0xAF639895  // uint16

/// @brief eSPI IO Range8 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE8_BASE                                 0x6A886359  // uint16

/// @brief eSPI IO Range9 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE9_BASE                                 0x8F8064A2  // uint16

/// @brief eSPI IO Range10 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE10_BASE                                0xCF830F2F  // uint16

/// @brief eSPI IO Range11 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE11_BASE                                0x1A4EDE5A  // uint16

/// @brief eSPI IO Range12 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE12_BASE                                0xBBB93104  // uint16

/// @brief eSPI IO Range13 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE13_BASE                                0x31928E0F  // uint16

/// @brief eSPI IO Range14 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE14_BASE                                0x77F37B0E  // uint16

/// @brief eSPI IO Range15 Base
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_IORANGE15_BASE                                0x20FD5C31  // uint16

/// @brief eSPI MMIO Range0 Size
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE0_SIZE                               0x28B271A1  // uint16

/// @brief eSPI MMIO Range1 Size
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE1_SIZE                               0x2712CD64  // uint16

/// @brief eSPI MMIO Range2 Size
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE2_SIZE                               0x0DC96471  // uint16

/// @brief eSPI MMIO Range3 Size
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE3_SIZE                               0x9EB1D9FC  // uint16

/// @brief eSPI MMIO Range4 Size
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE4_SIZE                               0xAABC6F12  // uint16

/// @brief FCH_GPP_CLK_DIS_MAP
/// @details Type:UINT16
/// @see FCH_GPP_CLK_MAP
  #define APCB_TOKEN_UID_FCH_GPP_CLK_MAP                                    0xCD7E6983  // uint16

/// @brief APCB_TOKEN_UID_DF_PSTATE_P0_CTL
/// @details Type:UINT8
/// @see MANUAL_AUTO_0XFF_VALIDVAL
  #define APCB_TOKEN_UID_DF_PSTATE_P0_CTL                                   0x9EE8EBB1

/// @brief APCB_TOKEN_UID_DF_PSTATE_FCLK_P0
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_DF_PSTATE_FCLK_P0                                  0x5DECCDA2

/// @brief APCB_TOKEN_UID_DF_PSTATE_MEMCLK_P0
/// @details Type:UINT16
/// @li 0x0000~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_DF_PSTATE_MEMCLK_P0                                0x9FC0EF29

/* FCH_GPP_CLK_MAP
 *  Value range for APCB_TOKEN_UID_FCH_GPP_CLK_MAP
 */
typedef enum {
  GPP_ALL_CLK_ON = 0x0000,
  GPP_ALL_CLK_AUTO = 0xFFFF,
  // S0
  S0_GPP0_CLK_OFF = 1,
  S0_GPP1_CLK_OFF = (1 << 1),
  S0_GPP2_CLK_OFF = (1 << 3),
  S0_GPP3_CLK_OFF = (1 << 4),
  S0_GPP4_CLK_OFF = (1 << 2),
  // S1
  S1_GPP0_CLK_OFF = 1 << 8,
  S1_GPP1_CLK_OFF = (1 << (1 + 8)),
  S1_GPP2_CLK_OFF = (1 << (3 + 8)),
  S1_GPP3_CLK_OFF = (1 << (4 + 8)),
  S1_GPP4_CLK_OFF = (1 << (2 + 8))
} FCH_GPP_CLK_MAP;

/// @brief UMC PwrDownDly
/// @details Type:UINT16
/// @li 0~0xFFF = UINT16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_PWRDOWNDLY                             0x1EBE755A

/// @brief CBS_CMN_MEM_TIMING_TRFC4_DDR
/// @details Type:UINT16
/// @li 0x003C~0x03DE = UITN16 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC4_DDR                       0xA99EB994

/// @brief MEM_PHY_IO_DRV_STREN_CS_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_CS_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CS_LPDDR                      0xE28F5775
/** MEM_PHY_IO_DRV_STREN_CS_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CS_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_CS_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_CS_LPDDR_400  = 400,
  MEM_PHY_IO_DRV_STREN_CS_LPDDR_100  = 100,
  MEM_PHY_IO_DRV_STREN_CS_LPDDR_67   = 67,
  MEM_PHY_IO_DRV_STREN_CS_LPDDR_50   = 50,
} MEM_PHY_IO_DRV_STREN_CS_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_DRV_STREN_CKE_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_CKE_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CKE_LPDDR                     0x8AA8A1F2
/** MEM_PHY_IO_DRV_STREN_CKE_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CKE_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_CKE_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_CKE_LPDDR_400  = 400,
  MEM_PHY_IO_DRV_STREN_CKE_LPDDR_100  = 100,
  MEM_PHY_IO_DRV_STREN_CKE_LPDDR_67   = 67,
  MEM_PHY_IO_DRV_STREN_CKE_LPDDR_50   = 50,
} MEM_PHY_IO_DRV_STREN_CKE_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_DRV_STREN_CK_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_CK_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CK_LPDDR                      0xBDE963CD
/** MEM_PHY_IO_DRV_STREN_CK_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CK_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_CK_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_CK_LPDDR_120  = 120,
  MEM_PHY_IO_DRV_STREN_CK_LPDDR_60   = 60,
  MEM_PHY_IO_DRV_STREN_CK_LPDDR_40   = 40,
  MEM_PHY_IO_DRV_STREN_CK_LPDDR_30   = 30,
  MEM_PHY_IO_DRV_STREN_CK_LPDDR_0    = 0,
} MEM_PHY_IO_DRV_STREN_CK_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_DRV_STREN_WCK_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_WCK_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_WCK_LPDDR                     0xA146B2DF
/** MEM_PHY_IO_DRV_STREN_WCK_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_WCK_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_WCK_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_WCK_LPDDR_120  = 120,
  MEM_PHY_IO_DRV_STREN_WCK_LPDDR_60   = 60,
  MEM_PHY_IO_DRV_STREN_WCK_LPDDR_40   = 40,
  MEM_PHY_IO_DRV_STREN_WCK_LPDDR_30   = 30,
  MEM_PHY_IO_DRV_STREN_WCK_LPDDR_0    = 0,
} MEM_PHY_IO_DRV_STREN_WCK_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_DRV_STREN_CA_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_CA_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CA_LPDDR                      0xEF426B33
/** MEM_PHY_IO_DRV_STREN_CA_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CA_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_CA_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_CA_LPDDR_120  = 120,
  MEM_PHY_IO_DRV_STREN_CA_LPDDR_60   = 60,
  MEM_PHY_IO_DRV_STREN_CA_LPDDR_40   = 40,
  MEM_PHY_IO_DRV_STREN_CA_LPDDR_30   = 30,
  MEM_PHY_IO_DRV_STREN_CA_LPDDR_0    = 0,
} MEM_PHY_IO_DRV_STREN_CA_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_DRV_STREN_DQ_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_DQ_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_DQ_LPDDR                      0x3AD85DC1
/** MEM_PHY_IO_DRV_STREN_DQ_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_DQ_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_DQ_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_DQ_LPDDR_120  = 120,
  MEM_PHY_IO_DRV_STREN_DQ_LPDDR_60   = 60,
  MEM_PHY_IO_DRV_STREN_DQ_LPDDR_40   = 40,
  MEM_PHY_IO_DRV_STREN_DQ_LPDDR_30   = 30,
  MEM_PHY_IO_DRV_STREN_DQ_LPDDR_0    = 0,
} MEM_PHY_IO_DRV_STREN_DQ_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_DRV_STREN_DQS_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_DRV_STREN_DQS_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_DQS_LPDDR                     0xFC6FAA3F
/** MEM_PHY_IO_DRV_STREN_DQS_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_DQS_LPDDR
*/
typedef enum {
  MEM_PHY_IO_DRV_STREN_DQS_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_DRV_STREN_DQS_LPDDR_120  = 120,
  MEM_PHY_IO_DRV_STREN_DQS_LPDDR_60   = 60,
  MEM_PHY_IO_DRV_STREN_DQS_LPDDR_40   = 40,
  MEM_PHY_IO_DRV_STREN_DQS_LPDDR_30   = 30,
  MEM_PHY_IO_DRV_STREN_DQS_LPDDR_0    = 0,
} MEM_PHY_IO_DRV_STREN_DQS_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_ODT_STREN_PROC_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_ODT_STREN_PROC_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_LPDDR                    0xB75AFE34
/** MEM_PHY_IO_ODT_STREN_PROC_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_LPDDR
*/
typedef enum {
  MEM_PHY_IO_ODT_STREN_PROC_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_ODT_STREN_PROC_LPDDR_120  = 120,
  MEM_PHY_IO_ODT_STREN_PROC_LPDDR_60   = 60,
  MEM_PHY_IO_ODT_STREN_PROC_LPDDR_40   = 40,
  MEM_PHY_IO_ODT_STREN_PROC_LPDDR_30   = 30,
  MEM_PHY_IO_ODT_STREN_PROC_LPDDR_0    = 0,
} MEM_PHY_IO_ODT_STREN_PROC_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR                 0xB92B6C4D
/** MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR
*/
typedef enum {
  MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_120  = 120,
  MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_60   = 60,
  MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_40   = 40,
  MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_30   = 30,
  MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_0    = 0,
} MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR                0x966EB383
/** MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR
*/
typedef enum {
  MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_120  = 120,
  MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_60   = 60,
  MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_40   = 40,
  MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_30   = 30,
  MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_0    = 0,
} MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_VALIDVAL;

/// @brief MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR
/// @details Type:UINT16
/// @see MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_VALIDVAL
  #define APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR                 0x0EBBF545
/** MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_VALIDVAL
*   Value range for APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR
*/
typedef enum {
  MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_AUTO = 0xFFFF, ///<Auto
  MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_120  = 120,
  MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_60   = 60,
  MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_40   = 40,
  MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_30   = 30,
  MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_0    = 0,
} MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_VALIDVAL;

/// @brief CBS_CMN_MEM_TARGET_SPEED_DDR
/// @details Type:UINT16
/// @li 3200~8400 (Stepping of 400) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TARGET_SPEED_DDR                               0xD06DBAFB

/// @brief CBS_CMN_MEM_TIMING_TCL_DDR
/// @details Type:UINT16
/// @li 22 ~ 64 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TCL_DDR                                 0xFC195E5C

/// @brief CBS_CMN_MEM_TIMING_TRCD_DDR
/// @details Type:UINT16
/// @li 8 ~ 62 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRCD_DDR                                0xE31F1FCE

/// @brief CBS_CMN_MEM_TIMING_TRP_DDR
/// @details Type:UINT16
/// @li 8 ~ 62 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRP_DDR                                 0x0DB8CB47

/// @brief CBS_CMN_MEM_TIMING_TRAS_DDR
/// @details Type:UINT16
/// @li 30 ~ 126 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRAS_DDR                                0x5E367ACA

/// @brief CBS_CMN_MEM_TIMING_TRC_DDR
/// @details Type:UINT16
/// @li 32 ~ 255 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRC_DDR                                 0x7F877269

/// @brief CBS_CMN_MEM_TIMING_TWR_DDR
/// @details Type:UINT16
/// @li 48 ~ 96 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWR_DDR                                 0x082D6E55

/// @brief CBS_CMN_MEM_TIMING_TRFC_DDR
/// @details Type:UINT16
/// @li 50 ~ 4095 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRFC1_DDR                               0xBBEED68E

/// @brief CBS_CMN_MEM_TIMING_TRFC2_DDR
/// @details Type:UINT16
/// @li 50 ~ 4095 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRFC2_DDR                               0x2C5A76B2

/// @brief CBS_CMN_MEM_TIMING_TRFCSB_DDR
/// @details Type:UINT16
/// @li 50 ~ 2047 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRFCSB_DDR                              0x32EB3B5B

/// @brief CBS_CMN_MEM_TIMING_TCWL_DDR
/// @details Type:UINT16
/// @li 9 ~ 22 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TCWL_DDR                                0x809E089B

/// @brief CBS_CMN_MEM_TIMING_TRTP_DDR
/// @details Type:UINT16
/// @li 5 ~ 14 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRTP_DDR                                0xE5AEA7D3

/// @brief CBS_CMN_MEM_TIMING_TRRD_L_DDR
/// @details Type:UINT16
/// @li 4 ~ 12 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRRDL_DDR                               0xA715EE94

/// @brief CBS_CMN_MEM_TIMING_TRRD_S_DDR
/// @details Type:UINT16
/// @li 4 ~ 12 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRRDS_DDR                               0x89251A4E

/// @brief CBS_CMN_MEM_TIMING_TFAW_DDR
/// @details Type:UINT16
/// @li 6 ~ 54 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TFAW_DDR                                0xD8690C08

/// @brief CBS_CMN_MEM_TIMING_TWTR_L_DDR
/// @details Type:UINT16
/// @li 2 ~ 14 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWTRL_DDR                               0xABA4CB4A

/// @brief CBS_CMN_MEM_TIMING_TWTR_S_DDR
/// @details Type:UINT16
/// @li 2 ~ 14 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWTRS_DDR                               0xF65089FD

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_L_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDSCL_DDR                            0x796F2C65

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDSC_DDR                             0x91364DC6

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SD_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDSD_DDR                             0x627427BD

/// @brief CBS_CMN_MEM_TIMING_TRDRD_DD_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDDD_DDR                             0x08E99B01

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_L_DDR
/// @details Type:UINT16
/// @li 1 ~ 63 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRSCL_DDR                            0x86E6A99A

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRSC_DDR                             0x275A3185

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SD_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRSD_DDR                             0x99B04933

/// @brief CBS_CMN_MEM_TIMING_TWRWR_DD_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRDD_DDR                             0x10B50ABC

/// @brief CBS_CMN_MEM_TIMING_TWRRD_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRRD_DDR                               0x77AF3F93

/// @brief CBS_CMN_MEM_TIMING_TRDWR_DDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDWR_DDR                               0x1382572D

/// @brief CBS_CMN_MEM_TARGET_SPEED_LPDDR
/// @details Type:UINT16
/// @li 3200~8400 (Stepping of 400) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TARGET_SPEED_LPDDR                             0x1C59A1B1

/// @brief CBS_CMN_MEM_TIMING_TCL_LPDDR
/// @details Type:UINT16
/// @li 22 ~ 64 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TCL_LPDDR                               0x2EF3CD21

/// @brief CBS_CMN_MEM_TIMING_TRCD_LPDDR
/// @details Type:UINT16
/// @li 8 ~ 62 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRCD_LPDDR                              0x9CA47358

/// @brief CBS_CMN_MEM_TIMING_TRP_LPDDR
/// @details Type:UINT16
/// @li 8 ~ 62 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRP_LPDDR                               0x6A6ACD56

/// @brief CBS_CMN_MEM_TIMING_TRAS_LPDDR
/// @details Type:UINT16
/// @li 30 ~ 126 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRAS_LPDDR                              0x084730D0

/// @brief CBS_CMN_MEM_TIMING_TRC_LPDDR
/// @details Type:UINT16
/// @li 32 ~ 255 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRC_LPDDR                               0x15E647DE

/// @brief CBS_CMN_MEM_TIMING_TWR_LPDDR
/// @details Type:UINT16
/// @li 48 ~ 96 (Stepping of 2) = UINT16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWR_LPDDR                               0xBD264FD2

/// @brief CBS_CMN_MEM_TIMING_TRFC_LPDDR
/// @details Type:UINT16
/// @li 50 ~ 4095 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRFC1_LPDDR                             0x3FEB31E1

/// @brief CBS_CMN_MEM_TIMING_TRFC2_LPDDR
/// @details Type:UINT16
/// @li 50 ~ 4095 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRFC2_LPDDR                             0x5E2741F3

/// @brief CBS_CMN_MEM_TIMING_TRFCSB_LPDDR
/// @details Type:UINT16
/// @li 50 ~ 2047 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRFCSB_LPDDR                            0x721CF47D

/// @brief CBS_CMN_MEM_TIMING_TCWL_LPDDR
/// @details Type:UINT16
/// @li 9 ~ 22 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TCWL_LPDDR                              0x3680AA03

/// @brief CBS_CMN_MEM_TIMING_TRTP_LPDDR
/// @details Type:UINT16
/// @li 5 ~ 14 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRTP_LPDDR                              0x47040585

/// @brief CBS_CMN_MEM_TIMING_TRRD_L_LPDDR
/// @details Type:UINT16
/// @li 4 ~ 12 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRRDL_LPDDR                             0xCAA1DE2B

/// @brief CBS_CMN_MEM_TIMING_TRRD_S_LPDDR
/// @details Type:UINT16
/// @li 4 ~ 12 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRRDS_LPDDR                             0x227B1257

/// @brief CBS_CMN_MEM_TIMING_TFAW_LPDDR
/// @details Type:UINT16
/// @li 6 ~ 54 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TFAW_LPDDR                              0x990FE8EC

/// @brief CBS_CMN_MEM_TIMING_TWTR_L_LPDDR
/// @details Type:UINT16
/// @li 2 ~ 14 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWTRL_LPDDR                             0x75191370

/// @brief CBS_CMN_MEM_TIMING_TWTR_S_LPDDR
/// @details Type:UINT16
/// @li 2 ~ 14 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWTRS_LPDDR                             0x46747213

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_L_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDSCL_LPDDR                          0x9D6E4E32

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SC_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDSC_LPDDR                           0x4D13A654

/// @brief CBS_CMN_MEM_TIMING_TRDRD_SD_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDSD_LPDDR                           0x4B360D3B

/// @brief CBS_CMN_MEM_TIMING_TRDRD_DD_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDRDDD_LPDDR                           0xA828ED33

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_L_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 63 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRSCL_LPDDR                          0x4F440415

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SC_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRSC_LPDDR                           0x90AD08F1

/// @brief CBS_CMN_MEM_TIMING_TWRWR_SD_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRSD_LPDDR                           0x5A12D87D

/// @brief CBS_CMN_MEM_TIMING_TWRWR_DD_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRWRDD_LPDDR                           0x58DB7DC4

/// @brief CBS_CMN_MEM_TIMING_TWRRD_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TWRRD_LPDDR                             0x1901D9F4

/// @brief CBS_CMN_MEM_TIMING_TRDWR_LPDDR
/// @details Type:UINT16
/// @li 1 ~ 15 = UITN16 Valid Range
  #define APCB_TOKEN_UID_MEM_TIMING_TRDWR_LPDDR                             0xCC0AC9BA

/// @brief GNB_SMU_VDDCR_SOC_VID_SVI3
/// @details Type:UINT16
/// @li 0 ~ 0x1FF = SVI3 VID Range
  #define APCB_TOKEN_UID_GNB_SMU_VDDCR_SOC_VID_SVI3                         0x86C4F334

/// @brief VDDIO Override
/// @details Type:UINT16
/// @li 800~1435
  #define APCB_TOKEN_UID_MEM_OC_VDDIO                                       0x66CAD59B

/// @brief VPP Override
/// @details Type:UINT16
/// @li 1500~2135
  #define APCB_TOKEN_UID_MEM_OC_VPP                                         0x0386C490

// =================================================================================================
// CBS CMN APCB Tokens (4BYTE)
// -------------------------------------------------------------------------------------------------
/// @GROUPID: APCB_GROUP_TOKEN
/// @TYPEID: APCB_TYPE_TOKEN_4BYTES
/// @DATATYPE: UINT32
/// @SIZE: 4

/// @brief CCX_MIN_SEV_ASID
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_CCX_MIN_SEV_ASID                                   0xA7C33753  // uint32

/// @brief DF_PCI_MMIO_SIZE
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_DF_PCI_MMIO_SIZE                                   0x3D9B7D7B  // uint32

/// @brief DF_CAKE_CRC_THRESH_PERF_BOUNDS
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS                     0x9258CF45  // uint32

/// @brief USERTIMINGMODE
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_USERTIMINGMODE                                     0xFC560D7D  // uint32

/// @brief MEMCLOCKVALUE
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_MEMCLOCKVALUE                                      0xCC83F65F  // uint32

/// @brief UMASIZE
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_UMASIZE                                            0x37B1F8CF  // uint32

/// @brief UMAALIGNMENT
/// @details Type:UINT32
/// @see UMAALIGNMENT_VAL
  #define APCB_TOKEN_UID_UMAALIGNMENT                                       0x57DDF512  // uint32

/// @DESC: Persistent Memory Base Address Low
/// @RANGE: 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_MEM_RESERVE_ADDR_LOW                               0x131A97A7  // uint32

/// @DESC: Persistent Memory Base Address High
/// @RANGE: 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_MEM_RESERVE_ADDR_HIGH                              0x1A5A44F6  // uint32

/// @DESC: Persistent Memory Size Low
/// @RANGE: 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_MEM_RESERVE_SIZE_LOW                               0x6A3EC09C  // uint32

/// @DESC: Persistent Memory Size High
/// @RANGE: 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_MEM_RESERVE_SIZE_HIGH                              0x2348ED7B  // uint32

/** UMAALIGNMENT_VAL
*  Value range for APCB_TOKEN_UID_UMAALIGNMENT
*/
typedef enum {
  UMAALIGNMENT_NONE = 0x00FFFFFF,      ///< NO UMA aligned
  UMAALIGNMENT_128MB = 0x00FFF800,     ///< UMA 128MB aligned
  UMAALIGNMENT_256MB = 0x00FFF000,     ///< UMA 256MB aligned
  UMAALIGNMENT_512MB = 0x00FFE000,     ///< UMA 512MB aligned
} UMAALIGNMENT_VAL;

/// @brief MEMORYBUSFREQUENCYLIMIT
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_MEMORYBUSFREQUENCYLIMIT                            0x34970A3C  // uint32

/// @brief POWERDOWNMODE
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_POWERDOWNMODE                                      0x23DD2705  // uint32

/// @brief CBS_CMN_MEM_MBIST_SUB_TEST
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Rang
  #define APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_SUB_TEST                         0x5BFB50B8  // uint in x86 data, not used in code //Obsolete

/// @brief CBS_CMN_CLDOVDD_PVOLTAGE
/// @details Type:UINT32
/// @li 0x000002BC~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_CBS_CMN_CLDOVDD_PVOLTAGE                           0x7915D708  //uint32

/// @brief MEM_MBIST_AGGR_STATIC_LANE_SEL_L32
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_L32                 0x745218AD  //uint32

/// @brief MEM_MBIST_AGGR_STATIC_LANE_SEL_U32
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_SEL_U32                 0xFAC9F48F  //uint32

/// @brief MEM_MBIST_TGT_STATIC_LANE_SEL_L32
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_L32                  0x81880D15  //uint32

/// @brief MEM_MBIST_TGT_STATIC_LANE_SEL_U32
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_SEL_U32                  0xAF669F33  //uint32

/// @brief PCIE_RESET_GPIO_PIN_SELECT
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_PCIE_RESET_GPIO_PIN_SELECT                         0x596663AC  //uint32

/// @brief AP base address when fetch from SPI ROM
/// @details Type:UINT32
/// @li Lowest Available Memory~Top Of Memory = UITN32 Valid Range, must be 0x10000 aligned
  #define APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE                         0xD403EA0E  // uint32

/// @brief eSPI MMIO Range0 Base
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE0_BASE                               0x29F49AF8  // uint32

/// @brief eSPI MMIO Range1 Base
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE1_BASE                               0xD1DE10BD  // uint32

/// @brief eSPI MMIO Range2 Base
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE2_BASE                               0x5831DE1A  // uint32

/// @brief eSPI MMIO Range3 Base
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE3_BASE                               0x419A9D4F  // uint32

/// @brief eSPI MMIO Range4 Base
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_ESPI_MMIORANGE4_BASE                               0x8321057B  // uint32

/// @brief FCH_ROM3_BASE_HIGH
/// @details Type:UINT32
/// @li 0x00000000~0x0000FFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_FCH_ROM3_BASE_HIGH                                 0x3E7D5274  //uint32

/// @brief Signature for CBS to keep the hash value
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_CBS_SYNC_SIGNATURE                                 0xB331378E  // uint32

/// @brief PSP_MEASURE_CONFIG
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_PSP_MEASURE_CONFIG                                 0xDD3AD029  //uint32

/// @brief BOARD_TYPE
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_BOARD_TYPE                                         0xB3E6A551  //uint8

/// @brief BOARD_NAME
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_AER_BOARD_NAME                                     0x352F0A80

/// @brief MULTI DRAM SUPPORT
/// @details Type:UINT32
/// @li
  #define APCB_TOKEN_UID_MULTIDRAM_SUPPORT                                  0xB35277CD

/// @brief MEM_SELF_HEAL_BIST_TIMEOUT
/// @details Type:UINT32
/// @li 0x00000000~0x0000FFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_MEM_SELF_HEAL_BIST_TIMEOUT                         0xBE7597D4  //uint32

/// @brief APCB_TOKEN_UID_DXIO_PHY_PARAM_VGA
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UINT32 Valid Range
  #define APCB_TOKEN_UID_DXIO_PHY_PARAM_VGA                                 0xDE09C43B

/// @brief APCB_TOKEN_UID_DXIO_PHY_PARAM_POLE
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UINT32 Valid Range
  #define APCB_TOKEN_UID_DXIO_PHY_PARAM_POLE                                0xB189447E

/// @brief APCB_TOKEN_UID_DXIO_PHY_PARAM_DC
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UINT32 Valid Range
  #define APCB_TOKEN_UID_DXIO_PHY_PARAM_DC                                  0x20667C30

/// @brief APCB_TOKEN_UID_DXIO_PHY_PARAM_IQOFC
/// @details Type:UINT32
/// @li -4 ~ 4 = INT32 Valid Range
  #define APCB_TOKEN_UID_DXIO_PHY_PARAM_IQOFC                               0x7E6069C5

/// @brief Disable Global Throttle
/// @details Type:UINT32
/// @li Enable or Disable
  #define APCB_TOKEN_UID_DF_DBG_GLB_TH_EN                                   0xC687AE9F

/// @brief Master Bandwidth Throttle control EnableEnable
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_ENABLE                       0xA11D447D
/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_CCM0_CT                      0xAB3C6DDA

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_CCM0_CTW                     0xC656ACF3

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_GCM_CT                       0xB8BA3433

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_GCM_CTW                      0x0EA8120F

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_NCM2_CT                      0x7C8F0A6B

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_NCM2_CTW                     0x23B63395

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_NCM1_CT                      0xDDA62DD7

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_NCM1_CTW                     0x08F35463

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_IOM0_CT                      0x2BB392EE

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_IOM0_CTW                     0x6F624E39

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_NCM0_CT                      0xFDE5919D

/// @brief Master Bandwidth Throttle control
/// @details Type:UINT32
/// @li 0~0xFF
  #define APCB_TOKEN_UID_DF_DBG_MEM_REQ_BW_CTL_NCM0_CTW                     0x366E6494

/// @brief Data Fabric Memory Interleaving 2 + 2 Control
/// @details Type:UINT32
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_DF_MEM_INTERLEAVING_2P2                            0xD3EF8B6A

/// @brief USB0 DRD mode
/// @details Type:UINT32
/// @see UINT8_XHCI_DRD_Auto
  #define APCB_TOKEN_UID_CBS_USB0_DRDMODE                                   0x85868031

/// @brief USB2 DRD mode
/// @details Type:UINT32
/// @see UINT8_XHCI_DRD_Auto
  #define APCB_TOKEN_UID_CBS_USB2_DRDMODE                                   0x772B8862

/// @brief USB0, USB2 DRD mode
/// @details Type:UINT32
/// @see UINT8_XHCI_DRD_Auto
  #define APCB_TOKEN_UID_CBS_USB_DRDMODE                                    0xDF4E9838



/// @brief eSPI IRQ polarity bitmap
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UINT32 Valid Range
  #define APCB_TOKEN_UID_ESPI_IRQ_POLARITY                                  0xD1045017  // uint32

/// @brief eSPI IRQ mask bitmap
/// @details Type:UINT32
/// @li 0x00000000~0xFFFFFFFF = UINT32 Valid Range
  #define APCB_TOKEN_UID_ESPI_IRQ_MASK                                      0x3AFFA42F  // uint32

/// @brief eSPI Plt Reset Deassert
/// @details Type:UINT32
/// @li 0~0x1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ESPI_PLT_RESET_DEASSERT                            0x32E7DEA9

/// @brief MBIST Enable
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_EN_DDR                                   0x7DCB2DA5

/// @brief MBIST Aggressors
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_DDR                           0xB46EF9AB

/// @brief MBIST TESTMODE
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TESTMODE_DDR                             0x96DF25CA

/// @brief CBS_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR             0x3B782D55

/// @brief MEM_MBIST_AGGRESSORS_CHNL
/// @details Type:UINT32
/// @see MEM_MBIST_AGGRESSORS_CHNL_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_DDR                      0x02FC8CA9

/// @brief MEM_MBIST_PATTERN_LENGTH
/// @details Type:UINT32
/// @li 0~12 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_DDR                       0x108BB3E6

/// @brief MEM_MBIST_PATTERN_SELECT
/// @details Type:UINT32
/// @see MEM_MBIST_PATTERN_SELECT_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_DDR                       0x5988CFA6

/// @brief MBIST Enable
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_EN_LPDDR                                 0xEFD1D8D1

/// @brief MBIST Aggressors
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_LPDDR                         0x23098151

/// @brief MBIST TESTMODE
/// @details Type:UINT32
/// @li 0~0xFF = UINT8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_TESTMODE_LPDDR                           0x78AA7AAF

/// @brief CBS_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_LPDDR           0x7DA9CAE7

/// @brief MEM_MBIST_AGGRESSORS_CHNL
/// @details Type:UINT32
/// @see MEM_MBIST_AGGRESSORS_CHNL_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_LPDDR                    0x120F148F

/// @brief MEM_MBIST_PATTERN_LENGTH
/// @details Type:UINT32
/// @li 0~12 = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_LPDDR                     0xEA22A3CF

/// @brief MEM_MBIST_PATTERN_SELECT
/// @details Type:UINT32
/// @see MEM_MBIST_PATTERN_SELECT_VALIDVAL
  #define APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_LPDDR                     0xC5E6BE8A

/// @brief MPM_WLAN_POWEREN_CONTROL
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_CONTROL                           0x4DBEA2A3
/** SECOND_PCIE_POWEREN_CONTROL_VALIDVAL
*   Value range for APCB_TOKEN_UID_SECOND_PCIE_POWEREN_CONTROL
*/
typedef enum {
  MPM_WLAN_POWEREN_CONTROL_FCH = 1,
  MPM_WLAN_POWEREN_CONTROL_EC = 2,
  MPM_WLAN_POWEREN_CONTROL_IOEXPANDER = 3,
} MPM_WLAN_POWEREN_CONTROL_VALIDVAL;

/// @brief MPM_WLAN_POWEREN_CMDSTSPORT
/// @details Type:UINT32
/// @li 0~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_CMDSTSPORT                        0x1999009D

/// @brief MPM_WLAN_POWEREN_DATAPORT
/// @details Type:UINT32
/// @li 0~0xFFFF = UITN16 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATAPORT                          0xAA5C1C4A

/// @brief MPM_WLAN_POWEREN_OFFSET
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_OFFSET                            0x4C11FA58

/// @brief MPM_WLAN_POWEREN_MASK
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_MASK                              0x5D5BE0F2

/// @brief MPM_WLAN_POWEREN_DATA
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_DATA                              0xD3DBC3D6

/// @brief MPM_WLAN_POWEREN_GPIOPIN
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIOPIN                           0xE6F6A078

/// @brief MPM_WLAN_POWEREN_GPIODATA
/// @details Type:UINT32
/// @li 0~0xFFFFFFFF = UITN32 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_GPIODATA                          0xAA556408

/// @brief MPM_WLAN_POWEREN_IOMUXEN
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXEN                           0x34E04419

/// @brief MPM_WLAN_POWEREN_IOMUXDATA
/// @details Type:UINT32
/// @li 0~0xFF = UITN8 Valid Range
  #define APCB_TOKEN_UID_MPM_WLAN_POWEREN_IOMUXDATA                         0x6EF93600

/// @brief MEM_RESTORE_RTC_DEFAULT_DATE
/// @details Type:UINT32
/// @li 8-bit BCD value : 0x00YYMMDD
  #define APCB_TOKEN_UID_MEM_RESTORE_RTC_DEFAULT_DATE                       0x34787D41

/// @brief  HSP UART enable/disable
/// @details Type:UINT32
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_HSP_UART_ENABLE                                    0x7004F583

typedef enum {
  HSP_UART_PORT_0 = 0,
  HSP_UART_PORT_1 = 1,
  HSP_UART_PORT_2 = 2,
  HSP_UART_PORT_3 = 3,
  HSP_UART_PORT_4 = 4,
  HSP_UART_PORT_5 = 5,
  HSP_UART_PORT_6 = 6,
  HSP_UART_PORT_7 = 7,
  HSP_UART_PORT_MAX,
} APCB_HSP_UART_PORT_VALIDVAL;

/// @brief  HSP UART Port
/// @details Type:UINT8
/// @see APCB_HSP_UART_PORT_VALIDVAL
  #define APCB_TOKEN_UID_HSP_UART_PORT                                      0xBF2DF767

/// @brief  HSP FIPS enable/disable
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_HSP_FIPS_ENABLE                                    0xF90AFB27
/// @brief  HSP FIPS zeroization enable/disable
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_HSP_FIPS_ZEROIZATION_ENABLE                        0x408898DC

/// @brief  HSP ACPI fence enable/disable, 0: PCI Fence, 1: ACPI Fence
///         This for debug only, no need to set in production
///         Only valid for RMB B0, for other family or program the Fence will be set automatically
/// @details Type:BOOLEAN
/// @see BOOLEAN_ENABLE_1_DISABLE_0_VALIDVAL
  #define APCB_TOKEN_UID_HSP_ACPI_FENCE_ENABLE                              0x2CF56294

/// @brief Intrusion detection feature is enabled/disabled, 0: feature disabled, 1: feature enabled
/// @details Type:UINT8
  #define APCB_TOKEN_UID_INTRUSION_DETECTION_ENABLE                         0x43DE5F56

/// @brief Intrusion event should be logged or not when intrusion is detected
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_INTRUSION_COUNTERMEASURE_LOG_EVENT                 0x2F6408DE

/// @brief TPM data should be cleared or not when intrusion is detected
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_INTRUSION_COUNTERMEASURE_CLEAR_TPM                 0x89C9977B

/// @brief System should be powered off or not when intrusion is detected
/// @details Type:BOOLEAN
/// @see BOOLEAN_TRUE_FALSE_VALIDVAL
  #define APCB_TOKEN_UID_INTRUSION_COUNTERMEASURE_POWER_OFF_SYSTEM          0x4C35B70B

/// @brief APCB_TOKEN_UID_MEM_REDIRECT_SCRUB_DDR
/// @details Type:UINT8
/// @see UINT8_Disable_0_Enable_1_Auto_0xFF
  #define APCB_TOKEN_UID_MEM_REDIRECT_SCRUB_DDR                             0x5E0BAD65  //uint8

/// @brief APCB_TOKEN_UID_MEM_REDIRECT_SCRUB_LIMIT_DDR
/// @details Type:UINT8
/// @see 8scrubs_0_4scrubs_1_2scrubs_2_1scrub_3_Auto_0xFF
  #define APCB_TOKEN_UID_MEM_REDIRECT_SCRUB_LIMIT_DDR                       0xB2AF7696  //uint8

/// @brief APCB_TOKEN_UID_MEM_PATROL_SCRUB_DDR
/// @details Type:UINT8
/// @see UINT8_Disable_0_Enable_1_Auto_0xFF
  #define APCB_TOKEN_UID_MEM_PATROL_SCRUB_DDR                               0x4EA08B9D  //uint8

/// @brief ACTION_ON_BIST_FAILURE
/// @details Type:UINT8
/// @li 0~1 = UITN8 Valid Range
  #define APCB_TOKEN_UID_ACTION_ON_BIST_FAILURE                             0xCBC2C0DD  // uint8

/// @brief Read CRC Enable
/// @details Type:UINT8
/// @see ENABLE_1_DISABLE_0_AUTO_FF_VALIDVAL_INTERNAL
  #define APCB_TOKEN_UID_MEM_RD_CRC_EN_DDR                                  0x836033C6

/// @brief Max Read CRC Error Replay
/// @details Type:UINT8
/// @li 1~0x3f = UITN8 Valid Range
  #define APCB_TOKEN_UID_MEM_MAX_READ_CRC_ERROR_REPLAY                      0x29ADC904
#endif // _APCB_V3_TOKEN_UID_H_

