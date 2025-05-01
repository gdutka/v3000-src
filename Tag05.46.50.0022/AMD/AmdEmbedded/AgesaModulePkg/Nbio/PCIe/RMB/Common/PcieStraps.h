/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#ifndef _PCIE_STRAPS_H_
#define _PCIE_STRAPS_H_

#include <AMD.h>
#include <GnbDxio.h>

#define RSMU_SW_STRAPRX_ADDR_PCIE0_INDEX    0x90467AC
#define RSMU_SW_STRAPRX_ADDR_PCIE0_DATA     0x90467B0
#define RSMU_SW_STRAPRX_ADDR_PCIE1_INDEX    0x90477AC
#define RSMU_SW_STRAPRX_ADDR_PCIE1_DATA     0x90477B0
#define RSMU_SW_STRAPRX_ADDR_PCIE2_INDEX    0x90487AC
#define RSMU_SW_STRAPRX_ADDR_PCIE2_DATA     0x90487B0
#define RSMU_SW_STRAPRX_ADDR_PCIE3_INDEX    0x90497AC
#define RSMU_SW_STRAPRX_ADDR_PCIE3_DATA     0x90497B0

#define SHORT_CTRLLR_STRAP(INDEX, DATA)    {shortCtrllerStrap, INDEX, DATA}
#define SHORT_PERPORT_STRAP(INDEX, DATA)   {shortPerPortStrap, 0, INDEX, DATA}
#define SHORT_ALLPORT_STRAP(INDEX, DATA)   {shortAllPortStrap, INDEX, DATA}
#define LONG_CTRLLR_STRAP(INDEX, DATA)     {longCtrllerStrap, INDEX, 0, DATA}
#define LONG_PERPORT_STRAP(INDEX, DATA)    {longPerPortStrap, 0, INDEX, 0, DATA}
#define LONG_ALLPORT_STRAP(INDEX, DATA)    {longAllPortStrap, INDEX, 0, DATA}


VOID
WritePcieStrap (
  IN   GNB_HANDLE   *GnbHandle,
  IN   UINT16       StrapIndex,
  IN   UINT32       Value,
  IN   UINT8        Wrapper
  );

///
/// SSP Fuse Map
///
/// INST0 =  / PCIE0
/// INST1 =  / PCIE1
///

#define FUSE_INSTANCE_MULTIPLIER    0x1000
#define FUSE_ACCESS_LOCATION        0xFFFE0000

#define PORT_STRAP_INDEX(INDEX, PORT) (INDEX + (PORT * (HOLD_TRAINING_B_INDEX - HOLD_TRAINING_A_INDEX)))

//
// Fuse data, generated directly from the strap data spreadsheet
//

#define STRAP_BIF_LINK_CONFIG_INDEX                              0x0   ///< STRAP_BIF_LINK_CONFIG
#define STRAP_BIF_LINK_CONFIG_SIZE                               6     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CONFIG_PERMUTATION_INDEX                       0x1   ///< STRAP_BIF_CONFIG_PERMUTATION
#define STRAP_BIF_CONFIG_PERMUTATION_SIZE                        4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_CHIP_BIF_MODE_INDEX                                0x2   ///< STRAP_CHIP_BIF_MODE
#define STRAP_CHIP_BIF_MODE_SIZE                                 5     ///< WriteDis = N, DefaultVal = 0x1F

#define BIF_LINK_CONFIG_RESERVED_INDEX                           0x3   ///< BIF_LINK_CONFIG_RESERVED
#define BIF_LINK_CONFIG_RESERVED_SIZE                            17    ///< WriteDis = N, DefaultVal = 0x0

#define PCIE0_ROMSTRAP_WRITE_DISABLE_INDEX                       0x4   ///< write disable strap for rom
#define PCIE1_ROMSTRAP_WRITE_DISABLE_INDEX                       0x4   ///< write disable strap for rom
#define PCIE0_ROMSTRAP_WRITE_DISABLE_SIZE                        1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_MSI_EN_INDEX                                   0x5   ///< STRAP_BIF_MSI_EN
#define STRAP_BIF_MSI_EN_SIZE                                    1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_AER_EN_INDEX                                   0x6   ///< STRAP_BIF_AER_EN
#define STRAP_BIF_AER_EN_SIZE                                    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_GEN2_COMPLIANCE_INDEX                          0x7   ///< STRAP_BIF_GEN2_COMPLIANCE
#define STRAP_BIF_GEN2_COMPLIANCE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_CLK_PM_EN_INDEX                                0x8   ///< Sets the value of LINK_CAP.CLOCK_POWER_MANAGEMENT
#define STRAP_BIF_CLK_PM_EN_SIZE                                 1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_EN_DEC_TO_HIDDEN_REG_INDEX                     0x9   ///< STRAP_BIF_EN_DEC_TO_HIDDEN_REG
#define STRAP_BIF_EN_DEC_TO_HIDDEN_REG_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LEGACY_DEVICE_TYPE_EN_INDEX                    0xa   ///< STRAP_BIF_LEGACY_DEVICE_TYPE_EN
#define STRAP_BIF_LEGACY_DEVICE_TYPE_EN_SIZE                     1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_MSTCPL_TIMEOUT_EN_INDEX                        0xb   ///< STRAP_BIF_MSTCPL_TIMEOUT_EN
#define STRAP_BIF_MSTCPL_TIMEOUT_EN_SIZE                         1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_FORCE_MASTER_TIMEOUT_EN_INDEX                  0xc   ///< STRAP_BIF_FORCE_MASTER_TIMEOUT_EN
#define STRAP_BIF_FORCE_MASTER_TIMEOUT_EN_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DBG_I2C_EN_INDEX                               0xd   ///< STRAP_BIF_DBG_I2C_EN
#define STRAP_BIF_DBG_I2C_EN_SIZE                                1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_TPH_SUPPORTED_INDEX                            0xe   ///< Provides an override for STRAP_TPH_SUPPORTED
#define STRAP_BIF_TPH_SUPPORTED_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_NO_SOFT_RESET_INDEX                            0xf   ///< STRAP_BIF_NO_SOFT_RESET
#define STRAP_BIF_NO_SOFT_RESET_SIZE                             1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_MULTI_FUNC_EN_INDEX                            0x10  ///< STRAP_BIF_MULTI_FUNC_EN
#define STRAP_BIF_MULTI_FUNC_EN_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_EXTENDED_TAG_ECN_EN_INDEX                      0x11  ///< STRAP_BIF_EXTENDED_TAG_ECN_EN
#define STRAP_BIF_EXTENDED_TAG_ECN_EN_SIZE                       1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_DPC_EN_INDEX                                   0x12  ///< STRAP_BIF_DPC_EN
#define STRAP_BIF_DPC_EN_SIZE                                    1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_DLF_EN_INDEX                                   0x13  ///< Enable Data Link Feature Extended Capability
#define STRAP_BIF_DLF_EN_SIZE                                    1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_16GT_EN_INDEX                                  0x14  ///< Enable Physical Layer 16.0 GT/s Extended Capability
#define STRAP_BIF_16GT_EN_SIZE                                   1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_MARGINING_EN_INDEX                             0x15  ///< Enable 16.0 GT/s Margining Extended Capability
#define STRAP_BIF_MARGINING_EN_SIZE                              1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_32GT_EN_INDEX                                  0x16  ///< Enable Physical Layer 32.0 GT/s Extended Capability
#define STRAP_BIF_32GT_EN_SIZE                                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_NPEM_EN_INDEX                                  0x17  ///< Enable NPEM Extended Capability
#define STRAP_BIF_NPEM_EN_SIZE                                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_VC_EN_INDEX                                    0x18  ///< STRAP_BIF_VC_EN
#define STRAP_BIF_VC_EN_SIZE                                     1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_2VC_EN_INDEX                                   0x19  ///< STRAP_BIF_2VC_EN
#define STRAP_BIF_2VC_EN_SIZE                                    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DSN_EN_INDEX                                   0x1a  ///< STRAP_BIF_DSN_EN
#define STRAP_BIF_DSN_EN_SIZE                                    1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_ARI_EN_INDEX                                   0x1b  ///< Provides an override for STRAP_BIF_ARI_EN
#define STRAP_BIF_ARI_EN_SIZE                                    1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_F0_EN_INDEX                                    0x1c  ///< STRAP_BIF_F0_EN
#define STRAP_BIF_F0_EN_SIZE                                     1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_D1_SUPPORT_INDEX                               0x1d  ///< STRAP_BIF_D1_SUPPORT
#define STRAP_BIF_D1_SUPPORT_SIZE                                1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_D2_SUPPORT_INDEX                               0x1e  ///< STRAP_BIF_D2_SUPPORT
#define STRAP_BIF_D2_SUPPORT_SIZE                                1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_MST_ADR64_EN_INDEX                             0x1f  ///< STRAP_BIF_MST_ADR64_EN
#define STRAP_BIF_MST_ADR64_EN_SIZE                              1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_TL_ALT_BUF_EN_INDEX                            0x20  ///< Provides an override for STRAP_TL_ALT_BUF_EN
#define STRAP_BIF_TL_ALT_BUF_EN_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LTR_SUPPORTED_INDEX                            0x21  ///< Provides an override for STRAP_LTR_SUPPORTED
#define STRAP_BIF_LTR_SUPPORTED_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_OBFF_SUPPORTED_INDEX                           0x22  ///< Indicates support for Optimized Buffer Flush/Fill
#define STRAP_BIF_OBFF_SUPPORTED_SIZE                            2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SYMALIGN_MODE_INDEX                            0x23  ///< STRAP_BIF_SYMALIGN_MODE
#define STRAP_BIF_SYMALIGN_MODE_SIZE                             1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_SYMALIGN_HW_DEBUG_INDEX                        0x24  ///< STRAP_BIF_SYMALIGN_HW_DEBUG
#define STRAP_BIF_SYMALIGN_HW_DEBUG_SIZE                         1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_BYPASS_SCRAMBLER_INDEX                         0x25  ///< STRAP_BIF_BYPASS_SCRAMBLER
#define STRAP_BIF_BYPASS_SCRAMBLER_SIZE                          1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_RXERR_LMT_INDEX                         0x26  ///< rxerr limit of rxdatavalid drop
#define STRAP_BIF_DESKEW_RXERR_LMT_SIZE                          3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_EMPTYMODE_INDEX                         0x27  ///< support empty mode
#define STRAP_BIF_DESKEW_EMPTYMODE_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DESKEW_TS2ONLY_INDEX                           0x28  ///< only deskew on TS2
#define STRAP_BIF_DESKEW_TS2ONLY_SIZE                            1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_UNLK_RPT_INDEX                          0x29  ///< BLKUNLK_RPT
#define STRAP_BIF_DESKEW_UNLK_RPT_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_ALLSKP_INDEX                            0x2a  ///< do deskew on all SKP
#define STRAP_BIF_DESKEW_ALLSKP_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_RESET_INDEX                             0x2b  ///< use ltssm to reset deskew logic
#define STRAP_BIF_DESKEW_RESET_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DESKEW_SKP_RMV_INDEX                           0x2c  ///< remove all skp symbols on data path
#define STRAP_BIF_DESKEW_SKP_RMV_SIZE                            1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DESKEW_EI_GAP_INDEX                            0x2d  ///< a clock number of time for avoiding garbage data from the link during entry of IE
#define STRAP_BIF_DESKEW_EI_GAP_SIZE                             6     ///< WriteDis = N, DefaultVal = 0x20

#define STRAP_BIF_DESKEW_EI_END_INDEX                            0x2e  ///< delete the symbol data after the last IDL in EIOS in 2 symbol
#define STRAP_BIF_DESKEW_EI_END_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DESKEW_TSONLY_INDEX                            0x2f  ///< only do deskew on TSx when receiving both TSx and SKP
#define STRAP_BIF_DESKEW_TSONLY_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_OFFSET_INDEX                            0x30  ///< narrow down the deskew windown
#define STRAP_BIF_DESKEW_OFFSET_SIZE                             3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_SKPCHK_INDEX                            0x31  ///< check enable for disparity and LFSR checking in skp os in Gen3
#define STRAP_BIF_DESKEW_SKPCHK_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DESKEW_PTRRST_INDEX                            0x32  ///< reset read pointer to write pointer after deskew failed
#define STRAP_BIF_DESKEW_PTRRST_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DESKEW_PH_AFTER_DESKEW_INDEX                   0x33  ///< keep doing phase shift for LC after deskewed
#define STRAP_BIF_DESKEW_PH_AFTER_DESKEW_SIZE                    1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DESKEW_BLKERR_RPT_INDEX                        0x34  ///< enable for block sync header check
#define STRAP_BIF_DESKEW_BLKERR_RPT_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DC_BALANCE_IGNORE_SKP_INDEX                    0x35  ///< Synopsis request to ignore DC balance calculation of SKP ordered set
#define STRAP_BIF_DC_BALANCE_IGNORE_SKP_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_RXP_HW_DEBUG_INDEX                                 0x36  ///< reserved strap bits
#define STRAP_RXP_HW_DEBUG_SIZE                                  4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_CHECK_DATA_RATE_INDEX                       0x37  ///< STRAP_BIF_LC_CHECK_DATA_RATE
#define STRAP_BIF_LC_CHECK_DATA_RATE_SIZE                        1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_ALWAYS_USE_FAST_TXCLK_INDEX                    0x38  ///< STRAP_BIF_ALWAYS_USE_FAST_TXCLK
#define STRAP_BIF_ALWAYS_USE_FAST_TXCLK_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_PLL_CMP_FREQ_MODE_INDEX                            0x39  ///< STRAP_PLL_CMP_FREQ_MODE
#define STRAP_PLL_CMP_FREQ_MODE_SIZE                             2     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_FORCE_GEN2_MODE_INDEX                          0x3a  ///< STRAP_BIF_FORCE_GEN2_MODE
#define STRAP_BIF_FORCE_GEN2_MODE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LINK_SPEED_CORE_RESERVED_INDEX                 0x3b  ///< BIF_STRAP_LINK_SPEED_CORE_RESERVED
#define BIF_STRAP_LINK_SPEED_CORE_RESERVED_SIZE                  27    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_RXEQEVAL_AFTER_BYPASSED_EQ_EN_INDEX         0x3c  ///< STRAP_BIF_LC_RXEQEVAL_AFTER_BYPASSED_EQ_EN
#define STRAP_BIF_LC_RXEQEVAL_AFTER_BYPASSED_EQ_EN_SIZE          1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_LOOPBACK_RXEQEVAL_EN_INDEX                  0x3d  ///< STRAP_BIF_LC_LOOPBACK_RXEQEVAL_EN
#define STRAP_BIF_LC_LOOPBACK_RXEQEVAL_EN_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_UPCONFIGURE_SUPPORT_INDEX                   0x3e  ///< STRAP_BIF_LC_UPCONFIGURE_SUPPORT
#define STRAP_BIF_LC_UPCONFIGURE_SUPPORT_SIZE                    1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_LC_UPCONFIGURE_DIS_INDEX                       0x3f  ///< STRAP_BIF_LC_UPCONFIGURE_DIS
#define STRAP_BIF_LC_UPCONFIGURE_DIS_SIZE                        1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_LC_DONT_DEASSERT_RX_EN_IN_TEST_INDEX           0x40  ///< STRAP_BIF_LC_DONT_DEASSERT_RX_EN_IN_TEST
#define STRAP_BIF_LC_DONT_DEASSERT_RX_EN_IN_TEST_SIZE            1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LC_MISC_CORE_RESERVED_2_INDEX                  0x41  ///< BIF_STRAP_LC_MISC_CORE_RESERVED_2
#define BIF_STRAP_LC_MISC_CORE_RESERVED_2_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SELECT_DEEMPHASIS_INDEX                     0x42  ///< STRAP_BIF_LC_SELECT_DEEMPHASIS
#define STRAP_BIF_LC_SELECT_DEEMPHASIS_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LC_MISC_CORE_RESERVED_3_INDEX                  0x43  ///< BIF_STRAP_LC_MISC_CORE_RESERVED_3
#define BIF_STRAP_LC_MISC_CORE_RESERVED_3_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LINK_BW_NOTIFICATION_CAP_EN_INDEX              0x44  ///< STRAP_BIF_LINK_BW_NOTIFICATION_CAP_EN
#define STRAP_BIF_LINK_BW_NOTIFICATION_CAP_EN_SIZE               1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_REVERSE_ALL_INDEX                              0x45  ///< STRAP_BIF_REVERSE_ALL
#define STRAP_BIF_REVERSE_ALL_SIZE                               1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_FORCE_GEN3_MODE_INDEX                          0x46  ///< STRAP_BIF_FORCE_GEN3_MODE
#define STRAP_BIF_FORCE_GEN3_MODE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_GEN3_COMPLIANCE_INDEX                          0x47  ///< Enable PCIe Base 3.1 compliant features
#define STRAP_BIF_GEN3_COMPLIANCE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_GEN4_COMPLIANCE_INDEX                          0x48  ///< Enable PCIe Base 4.0 compliant features
#define STRAP_BIF_GEN4_COMPLIANCE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_GEN5_COMPLIANCE_INDEX                          0x49  ///< Enable PCIe Base 5.0 compliant features
#define STRAP_BIF_GEN5_COMPLIANCE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ECRC_GEN_EN_INDEX                              0x4a  ///< Provides an override for STRAP_ECRC_GEN_EN
#define STRAP_BIF_ECRC_GEN_EN_SIZE                               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ECRC_CHECK_EN_INDEX                            0x4b  ///< Provides an override for STRAP_F0_ECRC_CHECK_EN
#define STRAP_BIF_ECRC_CHECK_EN_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_AUTO_DISABLE_SPEED_SUPPORT_MAX_FAIL_SEL_INDEX  0x4c  ///< Sets the maximum number of training failures to occur before disabling speed capability
#define STRAP_BIF_LC_AUTO_DISABLE_SPEED_SUPPORT_MAX_FAIL_SEL_SIZE  2     ///< WriteDis = N, DefaultVal = 0x2

#define BIF_STRAP_LC_MISC_CORE_RESERVED_4_INDEX                  0x4d  ///< BIF_STRAP_LC_MISC_CORE_RESERVED_4
#define BIF_STRAP_LC_MISC_CORE_RESERVED_4_SIZE                   14    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_PORT_ORDER_EN_INDEX                         0x4e  ///< STRAP_BIF_LC_PORT_ORDER_EN
#define STRAP_BIF_LC_PORT_ORDER_EN_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_IO_ERR_INDEX                         0x4f  ///< STRAP_BIF_RX_IGNORE_IO_ERR
#define STRAP_BIF_RX_IGNORE_IO_ERR_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_BE_ERR_INDEX                         0x50  ///< STRAP_BIF_RX_IGNORE_BE_ERR
#define STRAP_BIF_RX_IGNORE_BE_ERR_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_MSG_ERR_INDEX                        0x51  ///< STRAP_BIF_RX_IGNORE_MSG_ERR
#define STRAP_BIF_RX_IGNORE_MSG_ERR_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_ERROR_IGNORE_RESERVED_1_INDEX                  0x52  ///< BIF_STRAP_ERROR_IGNORE_RESERVED_1
#define BIF_STRAP_ERROR_IGNORE_RESERVED_1_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_CFG_ERR_INDEX                        0x53  ///< STRAP_BIF_RX_IGNORE_CFG_ERR
#define STRAP_BIF_RX_IGNORE_CFG_ERR_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_CPL_ERR_INDEX                        0x54  ///< STRAP_BIF_RX_IGNORE_CPL_ERR
#define STRAP_BIF_RX_IGNORE_CPL_ERR_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_EP_ERR_INDEX                         0x55  ///< STRAP_BIF_RX_IGNORE_EP_ERR
#define STRAP_BIF_RX_IGNORE_EP_ERR_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_LEN_MISMATCH_ERR_INDEX               0x56  ///< STRAP_BIF_RX_IGNORE_LEN_MISMATCH_ERR
#define STRAP_BIF_RX_IGNORE_LEN_MISMATCH_ERR_SIZE                1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_MAX_PAYLOAD_ERR_INDEX                0x57  ///< STRAP_BIF_RX_IGNORE_MAX_PAYLOAD_ERR
#define STRAP_BIF_RX_IGNORE_MAX_PAYLOAD_ERR_SIZE                 1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_TC_ERR_INDEX                         0x58  ///< STRAP_BIF_RX_IGNORE_TC_ERR
#define STRAP_BIF_RX_IGNORE_TC_ERR_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_ERROR_IGNORE_RESERVED_2_INDEX                  0x59  ///< BIF_STRAP_ERROR_IGNORE_RESERVED_2
#define BIF_STRAP_ERROR_IGNORE_RESERVED_2_SIZE                   2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RX_IGNORE_AT_ERR_INDEX                         0x5a  ///< STRAP_BIF_RX_IGNORE_AT_ERR
#define STRAP_BIF_RX_IGNORE_AT_ERR_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_ERROR_IGNORE_RESERVED_3_INDEX                  0x5b  ///< BIF_STRAP_ERROR_IGNORE_RESERVED_3
#define BIF_STRAP_ERROR_IGNORE_RESERVED_3_SIZE                   3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ERR_REPORTING_DIS_INDEX                        0x5c  ///< STRAP_BIF_ERR_REPORTING_DIS
#define STRAP_BIF_ERR_REPORTING_DIS_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CPL_ABORT_ERR_EN_INDEX                         0x5d  ///< STRAP_BIF_CPL_ABORT_ERR_EN
#define STRAP_BIF_CPL_ABORT_ERR_EN_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_INTERNAL_ERR_EN_INDEX                          0x5e  ///< STRAP_BIF_INTERNAL_ERR_EN
#define STRAP_BIF_INTERNAL_ERR_EN_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_MARGIN_IGNORE_C_SKP_INDEX                      0x5f  ///< STRAP_BIF_MARGIN_IGNORE_C_SKP
#define STRAP_BIF_MARGIN_IGNORE_C_SKP_SIZE                       1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN_INDEX                   0x60  ///< STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN
#define STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN_SIZE                    1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_SURPRISE_DOWN_ERR_REPORTING_CAPABLE_INDEX      0x61  ///< STRAP_BIF_SURPRISE_DOWN_ERR_REPORTING_CAPABLE
#define STRAP_BIF_SURPRISE_DOWN_ERR_REPORTING_CAPABLE_SIZE       1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_ERROR_IGNORE_RESERVED_4_INDEX                  0x62  ///< BIF_STRAP_ERROR_IGNORE_RESERVED_4
#define BIF_STRAP_ERROR_IGNORE_RESERVED_4_SIZE                   10    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_CDR_TEST_OFF_INDEX                          0x63  ///< STRAP_BIF_LC_CDR_TEST_OFF
#define STRAP_BIF_LC_CDR_TEST_OFF_SIZE                           12    ///< WriteDis = Y, DefaultVal = 0x60

#define STRAP_BIF_LC_CDR_TEST_SETS_INDEX                         0x64  ///< STRAP_BIF_LC_CDR_TEST_SETS
#define STRAP_BIF_LC_CDR_TEST_SETS_SIZE                          12    ///< WriteDis = Y, DefaultVal = 0x18

#define STRAP_BIF_LC_CDR_SET_TYPE_INDEX                          0x65  ///< STRAP_BIF_LC_CDR_SET_TYPE
#define STRAP_BIF_LC_CDR_SET_TYPE_SIZE                           2     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_FORCE_CDR_MODE_INDEX                           0x66  ///< STRAP_BIF_FORCE_CDR_MODE
#define STRAP_BIF_FORCE_CDR_MODE_SIZE                            1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_TEST_RESERVED_INDEX                            0x67  ///< BIF_STRAP_TEST_RESERVED
#define BIF_STRAP_TEST_RESERVED_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_TEST_TOGGLE_MODE_INDEX                         0x68  ///< STRAP_BIF_TEST_TOGGLE_MODE
#define STRAP_BIF_TEST_TOGGLE_MODE_SIZE                          1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_TEST_TOGGLE_PATTERN_INDEX                      0x69  ///< STRAP_BIF_TEST_TOGGLE_PATTERN
#define STRAP_BIF_TEST_TOGGLE_PATTERN_SIZE                       1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_TX_TEST_ALL_INDEX                              0x6a  ///< STRAP_BIF_TX_TEST_ALL
#define STRAP_BIF_TX_TEST_ALL_SIZE                               2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_VENDOR_ID_INDEX                                0x6b  ///< STRAP_BIF_VENDOR_ID
#define STRAP_BIF_VENDOR_ID_SIZE                                 16    ///< WriteDis = Y, DefaultVal = 0x1022

#define STRAP_BIF_BASE_CLASS_INDEX                               0x6c  ///< STRAP_BIF_BASE_CLASS
#define STRAP_BIF_BASE_CLASS_SIZE                                8     ///< WriteDis = Y, DefaultVal = 0x6

#define STRAP_BIF_SUB_CLASS_INDEX                                0x6d  ///< STRAP_BIF_SUB_CLASS
#define STRAP_BIF_SUB_CLASS_SIZE                                 8     ///< WriteDis = Y, DefaultVal = 0x4

#define STRAP_BIF_MAJOR_REV_ID_INDEX                             0x6e  ///< STRAP_BIF_MAJOR_REV_ID
#define STRAP_BIF_MAJOR_REV_ID_SIZE                              4     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_MINOR_REV_ID_INDEX                             0x6f  ///< STRAP_BIF_MINOR_REV_ID
#define STRAP_BIF_MINOR_REV_ID_SIZE                              4     ///< WriteDis = Y, DefaultVal = 0x0

#define BIF_STRAP_REV_ID_RESERVED_INDEX                          0x70  ///< BIF_STRAP_REV_ID_RESERVED
#define BIF_STRAP_REV_ID_RESERVED_SIZE                           24    ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_I2C_SLV_ADR_INDEX                              0x71  ///< STRAP_BIF_I2C_SLV_ADR
#define STRAP_BIF_I2C_SLV_ADR_SIZE                               7     ///< WriteDis = N, DefaultVal = 0x8

#define BIF_STRAP_I2C_CNTL_RESERVED_INDEX                        0x72  ///< BIF_STRAP_I2C_CNTL_RESERVED
#define BIF_STRAP_I2C_CNTL_RESERVED_SIZE                         25    ///< WriteDis = N, DefaultVal = 0x0

#define INT_LINKAUTONOMOUSBWINT_INDEX                            0x73  ///< INT_LINKAUTONOMOUSBWINT
#define INT_LINKAUTONOMOUSBWINT_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x1

#define BIF_INT_CNTRL_RESERVED_INDEX                             0x74  ///< BIF_INT_CNTRL_RESERVED
#define BIF_INT_CNTRL_RESERVED_SIZE                              31    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ACS_EN_INDEX                                   0x75  ///< Provides an override for STRAP_ACS_EN
#define STRAP_BIF_ACS_EN_SIZE                                    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ACS_SOURCE_VALIDATION_INDEX                    0x76  ///< STRAP_BIF_ACS_SOURCE_VALIDATION
#define STRAP_BIF_ACS_SOURCE_VALIDATION_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ACS_TRANSLATION_BLOCKING_INDEX                 0x77  ///< STRAP_BIF_ACS_TRANSLATION_BLOCKING
#define STRAP_BIF_ACS_TRANSLATION_BLOCKING_SIZE                  1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P_INDEX                0x78  ///< STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P
#define STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P_SIZE                 1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT_INDEX              0x79  ///< STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT
#define STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT_SIZE               1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_ACS_P2P_REQUEST_REDIRECT_INDEX                 0x7a  ///< STRAP_BIF_ACS_P2P_REQUEST_REDIRECT
#define STRAP_BIF_ACS_P2P_REQUEST_REDIRECT_SIZE                  1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_ACS_UPSTREAM_FORWARDING_INDEX                  0x7b  ///< STRAP_BIF_ACS_UPSTREAM_FORWARDING
#define STRAP_BIF_ACS_UPSTREAM_FORWARDING_SIZE                   1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_SDP_UNIT_ID_INDEX                              0x7c  ///< STRAP_BIF_SDP_UNIT_ID
#define STRAP_BIF_SDP_UNIT_ID_SIZE                               7     ///< WriteDis = Y, DefaultVal = 0x10

#define STRAP_BIF_TX_PORT_ACCESS_TIMER_SKEW_INDEX                0x7d  ///< STRAP_BIF_TX_PORT_ACCESS_TIMER_SKEW
#define STRAP_BIF_TX_PORT_ACCESS_TIMER_SKEW_SIZE                 4     ///< WriteDis = N, DefaultVal = 0x1

#define BIF_STRAP_ACS_RESERVED_INDEX                             0x7e  ///< BIF_STRAP_ACS_RESERVED
#define BIF_STRAP_ACS_RESERVED_SIZE                              14    ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_PM_RESERVED_1_INDEX                            0x7f  ///< BIF_STRAP_PM_RESERVED_1
#define BIF_STRAP_PM_RESERVED_1_SIZE                             1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_PME_SUPPORT_INDEX                              0x80  ///< STRAP_BIF_PME_SUPPORT
#define STRAP_BIF_PME_SUPPORT_SIZE                               5     ///< WriteDis = Y, DefaultVal = 0x19

#define STRAP_BIF_REDUCE_LANES_INDEX                             0x81  ///< STRAP_BIF_REDUCE_LANES
#define STRAP_BIF_REDUCE_LANES_SIZE                              16    ///< WriteDis = N, DefaultVal = 0xF000

#define STRAP_BIF_ISOLATED_RESET_INDEX                           0x82  ///< STRAP_BIF_ISOLATED_RESET
#define STRAP_BIF_ISOLATED_RESET_SIZE                            1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LTR_RSMU_ACK_MASK_INDEX                        0x83  ///< Provides override to use ltr interrupt for PD changes
#define STRAP_BIF_LTR_RSMU_ACK_MASK_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_WAIT_FOR_SLOW_DOMAIN_INDEX                     0x84  ///< allows PG to wait for slow clock to see reset before allowing traffic
#define STRAP_BIF_WAIT_FOR_SLOW_DOMAIN_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_PM_RESERVED_2_INDEX                            0x85  ///< BIF_STRAP_PM_RESERVED_2
#define BIF_STRAP_PM_RESERVED_2_SIZE                             7     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_KILL_GEN3_INDEX                                0x86  ///< Kill 8.0 GT/s data rate and PCIe Base 3.1 compliant features
#define STRAP_BIF_KILL_GEN3_SIZE                                 1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_MC_EN_INDEX                                    0x87  ///< STRAP_BIF_MC_EN
#define STRAP_BIF_MC_EN_SIZE                                     1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_STRAP_F0_ATOMIC_EN_INDEX                       0x88  ///< Provides an override for STRAP_F0_ATOMIC_EN
#define STRAP_BIF_STRAP_F0_ATOMIC_EN_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_STRAP_F0_ATOMIC_ROUTING_EN_INDEX               0x89  ///< Provides an override for STRAP_F0_ATOMIC_ROUTING_EN
#define STRAP_BIF_STRAP_F0_ATOMIC_ROUTING_EN_SIZE                1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_MSI_MULTI_CAP_INDEX                            0x8a  ///< Multiple Message Capable register is read to determine the number of requested messages
#define STRAP_BIF_MSI_MULTI_CAP_SIZE                             3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_STRAP_F0_NO_RO_ENABLED_P2P_PASSING_INDEX       0x8b  ///< Provides an override for STRAP_F0_RO_ENABLED_PR_PR_PASSING for Root-Complex.Controls DEVICE_CAP2.RO_ENABLED_PR_PR_PASSING register
#define STRAP_BIF_STRAP_F0_NO_RO_ENABLED_P2P_PASSING_SIZE        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_MSI_MAP_EN_INDEX                               0x8c  ///< Multiple Message Enable register is written to indicate the number of allocated messages
#define STRAP_BIF_MSI_MAP_EN_SIZE                                1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_PHY_CALIB_RST_INDEX                                0x8d  ///< STRAP_PHY_CALIB_RST
#define STRAP_PHY_CALIB_RST_SIZE                                 1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CFG_REG_RESET_ONLY_INDEX                       0x8e  ///< STRAP_BIF_CFG_REG_RESET_ONLY
#define STRAP_BIF_CFG_REG_RESET_ONLY_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LINK_DOWN_RESET_EN_INDEX                       0x8f  ///< STRAP_BIF_LINK_DOWN_RESET_EN
#define STRAP_BIF_LINK_DOWN_RESET_EN_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_KILL_GEN4_INDEX                                0x90  ///< Kill 16.0 GT/s data rate and PCIe Base 4.0 compliant features
#define STRAP_BIF_KILL_GEN4_SIZE                                 1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_STATIC_PG_EN_INDEX                             0x91  ///< STRAP_BIF_STATIC_PG_EN
#define STRAP_BIF_STATIC_PG_EN_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CFG_FW_PG_EXIT_CNTL_INDEX                      0x92  ///< STRAP_BIF_CFG_FW_PG_EXIT_CNTL
#define STRAP_BIF_CFG_FW_PG_EXIT_CNTL_SIZE                       2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LIVMIN_EXIT_CNTL_INDEX                         0x93  ///< STRAP_BIF_LIVMIN_EXIT_CNTL
#define STRAP_BIF_LIVMIN_EXIT_CNTL_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_KILL_GEN5_INDEX                                0x94  ///< Kill 32.0 GT/s data rate and PCIe Base 5.0 compliant features
#define STRAP_BIF_KILL_GEN5_SIZE                                 1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RCV_OVFL_EN_INDEX                              0x95  ///< Enables Receiver Overflow Capability
#define STRAP_BIF_RCV_OVFL_EN_SIZE                               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DRS_SUPPORTED_INDEX                            0x96  ///< Enables DRS Capability
#define STRAP_BIF_DRS_SUPPORTED_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_FRS_SUPPORTED_INDEX                            0x97  ///< Enables FRS Capability
#define STRAP_BIF_FRS_SUPPORTED_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_FRS_QUEUE_EN_INDEX                             0x98  ///< Enables FRS Queueing Capability
#define STRAP_BIF_FRS_QUEUE_EN_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_FRS_QUEUE_MAX_DEPTH_INDEX                      0x99  ///< Sets FRS Queue Max Depth
#define STRAP_BIF_FRS_QUEUE_MAX_DEPTH_SIZE                       2     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_RTR_EN_INDEX                                   0x9a  ///< Enables RTR Capability
#define STRAP_BIF_RTR_EN_SIZE                                    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_IMMEDIATE_READINESS_EN_INDEX                   0x9b  ///< Sets RTR Immediate Readiness Bit
#define STRAP_BIF_IMMEDIATE_READINESS_EN_SIZE                    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_RTR_RESET_TIME_INDEX                           0x9c  ///< Sets RTR Reset Time
#define STRAP_BIF_RTR_RESET_TIME_SIZE                            2     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_LCLK_DEEPSLEEP_OVR_INDEX                       0x9d  ///< STRAP_BIF_LCLK_DEEPSLEEP_OVR
#define STRAP_BIF_LCLK_DEEPSLEEP_OVR_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_MSI_EXT_MSG_DATA_CAP_INDEX                     0x9e  ///< STRAP_BIF_MSI_EXT_MSG_DATA_CAP
#define STRAP_BIF_MSI_EXT_MSG_DATA_CAP_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CTO_LOG_CAPABLE_INDEX                          0x9f  ///< STRAP_BIF_CTO_LOG_CAPABLE
#define STRAP_BIF_CTO_LOG_CAPABLE_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_FEATURE_EN_2_RESERVED_INDEX                    0xa0  ///< BIF_STRAP_FEATURE_EN_2_RESERVED
#define BIF_STRAP_FEATURE_EN_2_RESERVED_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_MODE_SUPPORTED_INDEX                  0xa1  ///< Indicates if the PHY supports CCIX ESM Mode
#define STRAP_BIF_CCIX_ESM_MODE_SUPPORTED_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_PHY_REACH_LEN_CAP_INDEX               0xa2  ///< STRAP_BIF_CCIX_ESM_PHY_REACH_LEN_CAP
#define STRAP_BIF_CCIX_ESM_PHY_REACH_LEN_CAP_SIZE                2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_RECAL_NEEDED_INDEX                    0xa3  ///< STRAP_BIF_CCIX_ESM_RECAL_NEEDED
#define STRAP_BIF_CCIX_ESM_RECAL_NEEDED_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_CALIB_TIME_INDEX                      0xa4  ///< STRAP_BIF_CCIX_ESM_CALIB_TIME
#define STRAP_BIF_CCIX_ESM_CALIB_TIME_SIZE                       3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_QUICK_EQ_TIMEOUT_INDEX                0xa5  ///< STRAP_BIF_CCIX_ESM_QUICK_EQ_TIMEOUT
#define STRAP_BIF_CCIX_ESM_QUICK_EQ_TIMEOUT_SIZE                 3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_EXT_EQ_PHASE2_TIMEOUT_INDEX           0xa6  ///< STRAP_BIF_CCIX_ESM_EXT_EQ_PHASE2_TIMEOUT
#define STRAP_BIF_CCIX_ESM_EXT_EQ_PHASE2_TIMEOUT_SIZE            3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_ESM_EXT_EQ_PHASE3_TIMEOUT_INDEX           0xa7  ///< STRAP_BIF_CCIX_ESM_EXT_EQ_PHASE3_TIMEOUT
#define STRAP_BIF_CCIX_ESM_EXT_EQ_PHASE3_TIMEOUT_SIZE            3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_DSP_20GT_TX_PRESET_INDEX  0xa8  ///< Downstream Port 20.0 GT/s ESM Transmitter Preset
#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_DSP_20GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0xF

#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_USP_20GT_TX_PRESET_INDEX  0xa9  ///< Upstream Port 20.0 GT/s ESM Transmitter Preset
#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_USP_20GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0xF

#define STRAP_BIF_CCIX_OPT_TLP_FMT_SUPPORT_INDEX                 0xaa  ///< STRAP_BIF_CCIX_OPT_TLP_FMT_SUPPORT
#define STRAP_BIF_CCIX_OPT_TLP_FMT_SUPPORT_SIZE                  1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_1_RESERVED_INDEX                          0xab  ///< STRAP_BIF_CCIX_1_RESERVED
#define STRAP_BIF_CCIX_1_RESERVED_SIZE                           7     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_VENDOR_ID_INDEX                           0xac  ///< STRAP_BIF_CCIX_VENDOR_ID
#define STRAP_BIF_CCIX_VENDOR_ID_SIZE                            16    ///< WriteDis = N, DefaultVal = 0x1E2C

#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_DSP_25GT_TX_PRESET_INDEX  0xad  ///< Downstream Port 25.0 GT/s ESM Transmitter Preset
#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_DSP_25GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0xF

#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_USP_25GT_TX_PRESET_INDEX  0xae  ///< Upstream Port 25.0 GT/s ESM Transmitter Prese
#define STRAP_BIF_ESM_LANE_EQUALIZATION_CNTL_USP_25GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0xF

#define STRAP_BIF_CCIX_2_RESERVED_INDEX                          0xaf  ///< STRAP_BIF_CCIX_2_RESERVED
#define STRAP_BIF_CCIX_2_RESERVED_SIZE                           8     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_PI_HW_DEBUG_INDEX                              0xb0  ///< PI HW strap
#define STRAP_BIF_PI_HW_DEBUG_SIZE                               32    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DEVICE_SERIAL_NUM_LSB_INDEX                    0xb1  ///< STRAP_BIF_DEVICE_SERIAL_NUM_LSB
#define STRAP_BIF_DEVICE_SERIAL_NUM_LSB_SIZE                     32    ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_DEVICE_SERIAL_NUM_MSB_INDEX                    0xb2  ///< STRAP_BIF_DEVICE_SERIAL_NUM_MSB
#define STRAP_BIF_DEVICE_SERIAL_NUM_MSB_SIZE                     32    ///< WriteDis = N, DefaultVal = 0x000C8700

#define STRAP_BIF_SUBSYS_VEN_ID_INDEX                            0xb3  ///< STRAP_BIF_SUBSYS_VEN_ID
#define STRAP_BIF_SUBSYS_VEN_ID_SIZE                             16    ///< WriteDis = N, DefaultVal = 0x1022

#define STRAP_BIF_SUBSYS_ID_INDEX                                0xb4  ///< STRAP_BIF_SUBSYS_ID
#define STRAP_BIF_SUBSYS_ID_SIZE                                 16    ///< WriteDis = N, DefaultVal = 0x1234

#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_INDEX  0xb5  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_SIZE  3     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_INDEX  0xb6  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_SIZE  3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_INDEX  0xb7  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_INDEX  0xb8  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET_INDEX  0xb9  ///< Downstream Port 16.0 GT/s Transmitter Preset
#define STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET_INDEX  0xba  ///< Upstream Port 16.0 GT/s Transmitter Preset
#define STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_32GT_TX_PRESET_INDEX  0xbb  ///< Downstream Port 32.0 GT/s Transmitter Preset
#define STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_32GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_32GT_TX_PRESET_INDEX  0xbc  ///< Upstream Port 32.0 GT/s Transmitter Preset
#define STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_32GT_TX_PRESET_SIZE  4     ///< WriteDis = N, DefaultVal = 0x1

#define BIF_LANE_EQUALIZATION_CNTL_RESERVED_INDEX                0xbd  ///< BIF_LANE_EQUALIZATION_CNTL_RESERVED
#define BIF_LANE_EQUALIZATION_CNTL_RESERVED_SIZE                 2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_QUICKSIM_START_INDEX                           0xbe  ///< STRAP_BIF_QUICKSIM_START
#define STRAP_BIF_QUICKSIM_START_SIZE                            1     ///< WriteDis = N, DefaultVal = 0x0

#define WRP_MISC_STRAP_RESERVED_INDEX                            0xbf  ///< WRP_MISC_STRAP_RESERVED
#define WRP_MISC_STRAP_RESERVED_SIZE                             31    ///< WriteDis = N, DefaultVal = 0x0

#define TX_MISC_STRAP_RESERVED_INDEX                             0xc0  ///< TX_MISC_STRAP_RESERVED
#define TX_MISC_STRAP_RESERVED_SIZE                              32    ///< WriteDis = N, DefaultVal = 0x0

#define LC_MISC_STRAP_RESERVED_INDEX                             0xc1  ///< LC_MISC_STRAP_RESERVED
#define LC_MISC_STRAP_RESERVED_SIZE                              32    ///< WriteDis = N, DefaultVal = 0x0

#define RXPI_MISC_STRAP_RESERVED_INDEX                           0xc2  ///< RXPI_MISC_STRAP_RESERVED
#define RXPI_MISC_STRAP_RESERVED_SIZE                            32    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ESM_AT_12P6_12P8G_INDEX                        0xc3  ///< STRAP_BIF_ESM_AT_12P6_12P8G
#define STRAP_BIF_ESM_AT_12P6_12P8G_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_12P1_12P5G_INDEX                        0xc4  ///< STRAP_BIF_ESM_AT_12P1_12P5G
#define STRAP_BIF_ESM_AT_12P1_12P5G_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_11P1_12P0G_INDEX                        0xc5  ///< STRAP_BIF_ESM_AT_11P1_12P0G
#define STRAP_BIF_ESM_AT_11P1_12P0G_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_9P6_11P0G_INDEX                         0xc6  ///< STRAP_BIF_ESM_AT_9P6_11P0G
#define STRAP_BIF_ESM_AT_9P6_11P0G_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_MIN_TIME_IN_EI_INDEX                       0xc7  ///< STRAP_BIF_ESM_MIN_TIME_IN_EI
#define STRAP_BIF_ESM_MIN_TIME_IN_EI_SIZE                        9     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ESM_AT_16P0G_INDEX                             0xc8  ///< STRAP_BIF_ESM_AT_16P0G
#define STRAP_BIF_ESM_AT_16P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_17P0G_INDEX                             0xc9  ///< STRAP_BIF_ESM_AT_17P0G
#define STRAP_BIF_ESM_AT_17P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_18P0G_INDEX                             0xca  ///< STRAP_BIF_ESM_AT_18P0G
#define STRAP_BIF_ESM_AT_18P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_19P0G_INDEX                             0xcb  ///< STRAP_BIF_ESM_AT_19P0G
#define STRAP_BIF_ESM_AT_19P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_20P0G_INDEX                             0xcc  ///< STRAP_BIF_ESM_AT_20P0G
#define STRAP_BIF_ESM_AT_20P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_21P0G_INDEX                             0xcd  ///< STRAP_BIF_ESM_AT_21P0G
#define STRAP_BIF_ESM_AT_21P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_22P0G_INDEX                             0xce  ///< STRAP_BIF_ESM_AT_22P0G
#define STRAP_BIF_ESM_AT_22P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_23P0G_INDEX                             0xcf  ///< STRAP_BIF_ESM_AT_23P0G
#define STRAP_BIF_ESM_AT_23P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_24P0G_INDEX                             0xd0  ///< STRAP_BIF_ESM_AT_24P0G
#define STRAP_BIF_ESM_AT_24P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_AT_25P0G_INDEX                             0xd1  ///< STRAP_BIF_ESM_AT_25P0G
#define STRAP_BIF_ESM_AT_25P0G_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ESM_RESERVED_INDEX                             0xd2  ///< STRAP_BIF_ESM_RESERVED
#define STRAP_BIF_ESM_RESERVED_SIZE                              9     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_MSI_EN_INDEX                              0xd3  ///< STRAP_BIF_SWUS_MSI_EN
#define STRAP_BIF_SWUS_MSI_EN_SIZE                               1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_SWUS_VC_EN_INDEX                               0xd4  ///< STRAP_BIF_SWUS_VC_EN
#define STRAP_BIF_SWUS_VC_EN_SIZE                                1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_DSN_EN_INDEX                              0xd5  ///< STRAP_BIF_SWUS_DSN_EN
#define STRAP_BIF_SWUS_DSN_EN_SIZE                               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_AER_EN_INDEX                              0xd6  ///< STRAP_BIF_SWUS_AER_EN
#define STRAP_BIF_SWUS_AER_EN_SIZE                               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_ECRC_CHECK_EN_INDEX                       0xd7  ///< STRAP_BIF_SWUS_ECRC_CHECK_EN
#define STRAP_BIF_SWUS_ECRC_CHECK_EN_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_ECRC_GEN_EN_INDEX                         0xd8  ///< STRAP_BIF_SWUS_ECRC_GEN_EN
#define STRAP_BIF_SWUS_ECRC_GEN_EN_SIZE                          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_CPL_ABORT_ERR_EN_INDEX                    0xd9  ///< STRAP_BIF_SWUS_CPL_ABORT_ERR_EN
#define STRAP_BIF_SWUS_CPL_ABORT_ERR_EN_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_SWUS_STRAP_F0_ATOMIC_EN_INDEX                  0xda  ///< STRAP_BIF_SWUS_STRAP_F0_ATOMIC_EN
#define STRAP_BIF_SWUS_STRAP_F0_ATOMIC_EN_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_STRAP_F0_ATOMIC_ROUTING_EN_INDEX          0xdb  ///< STRAP_BIF_SWUS_STRAP_F0_ATOMIC_ROUTING_EN
#define STRAP_BIF_SWUS_STRAP_F0_ATOMIC_ROUTING_EN_SIZE           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_STRAP_F0_NO_RO_ENABLED_P2P_PASSING_INDEX  0xdc  ///< STRAP_BIF_SWUS_STRAP_F0_NO_RO_ENABLED_P2P_PASSING
#define STRAP_BIF_SWUS_STRAP_F0_NO_RO_ENABLED_P2P_PASSING_SIZE   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_ERR_REPORTING_DIS_INDEX                   0xdd  ///< STRAP_BIF_SWUS_ERR_REPORTING_DIS
#define STRAP_BIF_SWUS_ERR_REPORTING_DIS_SIZE                    1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_SWUS_NO_SOFT_RESET_INDEX                       0xde  ///< STRAP_BIF_SWUS_NO_SOFT_RESET
#define STRAP_BIF_SWUS_NO_SOFT_RESET_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_D2_SUPPORT_INDEX                          0xdf  ///< STRAP_BIF_SWUS_D2_SUPPORT
#define STRAP_BIF_SWUS_D2_SUPPORT_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_D1_SUPPORT_INDEX                          0xe0  ///< STRAP_BIF_SWUS_D1_SUPPORT
#define STRAP_BIF_SWUS_D1_SUPPORT_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_LTR_SUPPORTED_INDEX                       0xe1  ///< STRAP_BIF_SWUS_LTR_SUPPORTED
#define STRAP_BIF_SWUS_LTR_SUPPORTED_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_ARI_EN_INDEX                              0xe2  ///< STRAP_BIF_SWUS_ARI_EN
#define STRAP_BIF_SWUS_ARI_EN_SIZE                               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_SUBSYS_VEN_ID_INDEX                       0xe3  ///< STRAP_BIF_SWUS_SUBSYS_VEN_ID
#define STRAP_BIF_SWUS_SUBSYS_VEN_ID_SIZE                        16    ///< WriteDis = N, DefaultVal = 0x1022

#define STRAP_BIF_SWUS_SUB_CLASS_INDEX                           0xe4  ///< STRAP_BIF_SWUS_SUB_CLASS
#define STRAP_BIF_SWUS_SUB_CLASS_SIZE                            8     ///< WriteDis = N, DefaultVal = 0x4

#define STRAP_BIF_SWUS_BASE_CLASS_INDEX                          0xe5  ///< STRAP_BIF_SWUS_BASE_CLASS
#define STRAP_BIF_SWUS_BASE_CLASS_SIZE                           8     ///< WriteDis = N, DefaultVal = 0x6

#define STRAP_BIF_SWUS_MAJOR_REV_ID_INDEX                        0xe6  ///< STRAP_BIF_SWUS_MAJOR_REV_ID
#define STRAP_BIF_SWUS_MAJOR_REV_ID_SIZE                         4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_MINOR_REV_ID_INDEX                        0xe7  ///< STRAP_BIF_SWUS_MINOR_REV_ID
#define STRAP_BIF_SWUS_MINOR_REV_ID_SIZE                         4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_PME_SUPPORT_INDEX                         0xe8  ///< STRAP_BIF_SWUS_PME_SUPPORT
#define STRAP_BIF_SWUS_PME_SUPPORT_SIZE                          5     ///< WriteDis = N, DefaultVal = 0x19

#define STRAP_BIF_SWUS_OBFF_SUPPORTED_INDEX                      0xe9  ///< STRAP_BIF_SWUS_OBFF_SUPPORTED
#define STRAP_BIF_SWUS_OBFF_SUPPORTED_SIZE                       2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_TX_CHK_FC_FOR_L1_DIS_INDEX                     0xea  ///< Disables FC check prior to L1 entry
#define STRAP_BIF_TX_CHK_FC_FOR_L1_DIS_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS_SSID_EN_INDEX                             0xeb  ///< STRAP_BIF_SWUS_SSID_EN
#define STRAP_BIF_SWUS_SSID_EN_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_SWUS3_RESERVED_INDEX                           0xec  ///< STRAP_BIF_SWUS3_RESERVED
#define STRAP_BIF_SWUS3_RESERVED_SIZE                            31    ///< WriteDis = N, DefaultVal = 0x0

#define HOLD_TRAINING_A_INDEX                                    0xed  ///< HOLD_TRAINING_A
#define HOLD_TRAINING_A_SIZE                                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_HOLD_TRAINING_MODE_A_INDEX                  0xee  ///< STRAP_BIF_LC_HOLD_TRAINING_MODE_A
#define STRAP_BIF_LC_HOLD_TRAINING_MODE_A_SIZE                   3     ///< WriteDis = N, DefaultVal = 0x2

#define STRAP_BIF_PORT_AUTO_RELEASE_HOLD_TRAINING_A_INDEX        0xef  ///< STRAP_BIF_PORT_AUTO_RELEASE_HOLD_TRAINING_A
#define STRAP_BIF_PORT_AUTO_RELEASE_HOLD_TRAINING_A_SIZE         1     ///< WriteDis = N, DefaultVal = 0x0

#define PCIE_HOLD_TRAINING_A_RESERVED_INDEX                      0xf0  ///< PCIE_HOLD_TRAINING_A_RESERVED
#define PCIE_HOLD_TRAINING_A_RESERVED_SIZE                       27    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_DIS_A_INDEX          0xf1  ///< STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_DIS_A
#define STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_DIS_A_SIZE           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_16GT_DIS_A_INDEX     0xf2  ///< STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_16GT_DIS_A
#define STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_16GT_DIS_A_SIZE      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_32GT_DIS_A_INDEX     0xf3  ///< STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_32GT_DIS_A
#define STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_32GT_DIS_A_SIZE      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_INIT_SPEED_NEG_IN_L0s_EN_A_INDEX            0xf4  ///< STRAP_BIF_LC_INIT_SPEED_NEG_IN_L0s_EN_A
#define STRAP_BIF_LC_INIT_SPEED_NEG_IN_L0s_EN_A_SIZE             1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_LC_INIT_SPEED_NEG_IN_L1_EN_A_INDEX             0xf5  ///< STRAP_BIF_LC_INIT_SPEED_NEG_IN_L1_EN_A
#define STRAP_BIF_LC_INIT_SPEED_NEG_IN_L1_EN_A_SIZE              1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_LC_TARGET_LINK_SPEED_OVERRIDE_EN_A_INDEX       0xf6  ///< STRAP_BIF_LC_TARGET_LINK_SPEED_OVERRIDE_EN_A
#define STRAP_BIF_LC_TARGET_LINK_SPEED_OVERRIDE_EN_A_SIZE        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_BYPASS_EQ_8GT_A_INDEX                       0xf7  ///< STRAP_BIF_LC_BYPASS_EQ_8GT_A
#define STRAP_BIF_LC_BYPASS_EQ_8GT_A_SIZE                        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_8GT_A_INDEX             0xf8  ///< STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_8GT_A
#define STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_8GT_A_SIZE              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_EQ_SEARCH_MODE_8GT_A_INDEX                  0xf9  ///< STRAP_BIF_LC_EQ_SEARCH_MODE_8GT_A
#define STRAP_BIF_LC_EQ_SEARCH_MODE_8GT_A_SIZE                   2     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_LC_BYPASS_EQ_16GT_A_INDEX                      0xfa  ///< STRAP_BIF_LC_BYPASS_EQ_16GT_A
#define STRAP_BIF_LC_BYPASS_EQ_16GT_A_SIZE                       1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_16GT_A_INDEX            0xfb  ///< STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_16GT_A
#define STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_16GT_A_SIZE             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_EQ_SEARCH_MODE_16GT_A_INDEX                 0xfc  ///< STRAP_BIF_LC_EQ_SEARCH_MODE_16GT_A
#define STRAP_BIF_LC_EQ_SEARCH_MODE_16GT_A_SIZE                  2     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_TARGET_LINK_SPEED_A_INDEX                      0xfd  ///< STRAP_BIF_TARGET_LINK_SPEED_A
#define STRAP_BIF_TARGET_LINK_SPEED_A_SIZE                       3     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_LC_BYPASS_EQ_32GT_A_INDEX                      0xfe  ///< STRAP_BIF_LC_BYPASS_EQ_32GT_A
#define STRAP_BIF_LC_BYPASS_EQ_32GT_A_SIZE                       1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_32GT_A_INDEX            0xff  ///< STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_32GT_A
#define STRAP_BIF_LC_BYPASS_EQ_REQ_PHASE_32GT_A_SIZE             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_EQ_SEARCH_MODE_32GT_A_INDEX                 0x100  ///< STRAP_BIF_LC_EQ_SEARCH_MODE_32GT_A
#define STRAP_BIF_LC_EQ_SEARCH_MODE_32GT_A_SIZE                  2     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_LC_BYPASS_EQ_TO_HIGH_RATE_SUPPORT_A_INDEX      0x101  ///< Controls if support for Equalization Bypass to Highest Rate is advertized in the config space.
#define STRAP_BIF_LC_BYPASS_EQ_TO_HIGH_RATE_SUPPORT_A_SIZE       1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ADVERTISE_EQ_TO_HIGH_RATE_SUPPORT_A_INDEX   0x102  ///< Controls if the TSXs sent prior to LinkUp advertise Equalization Bypass to Highest Rate.
#define STRAP_BIF_LC_ADVERTISE_EQ_TO_HIGH_RATE_SUPPORT_A_SIZE    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_NO_EQ_NEEDED_SUPPORT_A_INDEX                0x103  ///< Controls if support for No Equalization Needed is advertized in the config space.
#define STRAP_BIF_LC_NO_EQ_NEEDED_SUPPORT_A_SIZE                 1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ADVERTISE_NO_EQ_NEEDED_SUPPORT_A_INDEX      0x104  ///< Controls if the TSXs sent prior to LinkUp advertise No Equalization Needed.
#define STRAP_BIF_LC_ADVERTISE_NO_EQ_NEEDED_SUPPORT_A_SIZE       1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LINK_SPEED_PORT_A_RESERVED_4_INDEX             0x105  ///< BIF_STRAP_LINK_SPEED_PORT_A_RESERVED_4
#define BIF_STRAP_LINK_SPEED_PORT_A_RESERVED_4_SIZE              7     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_L0S_INACTIVITY_A_INDEX                      0x106  ///< STRAP_BIF_LC_L0S_INACTIVITY_A
#define STRAP_BIF_LC_L0S_INACTIVITY_A_SIZE                       4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_L1_INACTIVITY_A_INDEX                       0x107  ///< STRAP_BIF_LC_L1_INACTIVITY_A
#define STRAP_BIF_LC_L1_INACTIVITY_A_SIZE                        4     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_ASPM_A_RESERVED_1_INDEX                        0x108  ///< BIF_STRAP_ASPM_A_RESERVED_1
#define BIF_STRAP_ASPM_A_RESERVED_1_SIZE                         4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ELEC_IDLE_MODE_A_INDEX                      0x109  ///< STRAP_BIF_LC_ELEC_IDLE_MODE_A
#define STRAP_BIF_LC_ELEC_IDLE_MODE_A_SIZE                       2     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_PM_SUPPORT_A_INDEX                             0x10a  ///< STRAP_BIF_PM_SUPPORT_A
#define STRAP_BIF_PM_SUPPORT_A_SIZE                              2     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_BIF_L1_EXIT_LATENCY_A_INDEX                        0x10b  ///< STRAP_BIF_L1_EXIT_LATENCY_A
#define STRAP_BIF_L1_EXIT_LATENCY_A_SIZE                         3     ///< WriteDis = N, DefaultVal = 0x6

#define STRAP_BIF_L0S_EXIT_LATENCY_A_INDEX                       0x10c  ///< STRAP_BIF_L0S_EXIT_LATENCY_A
#define STRAP_BIF_L0S_EXIT_LATENCY_A_SIZE                        3     ///< WriteDis = N, DefaultVal = 0x3

#define STRAP_ENABLE_SIGNAL_EXIT_L1_A_INDEX                      0x10d  ///< STRAP_ENABLE_SIGNAL_EXIT_L1_A
#define STRAP_ENABLE_SIGNAL_EXIT_L1_A_SIZE                       1     ///< WriteDis = N, DefaultVal = 0x1

#define BIF_STRAP_ASPM_A_RESERVED_2_INDEX                        0x10e  ///< BIF_STRAP_ASPM_A_RESERVED_2
#define BIF_STRAP_ASPM_A_RESERVED_2_SIZE                         9     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_LINK_BW_NOTIFICATION_DETECT_MODE_A_INDEX    0x10f  ///< STRAP_BIF_LC_LINK_BW_NOTIFICATION_DETECT_MODE_A
#define STRAP_BIF_LC_LINK_BW_NOTIFICATION_DETECT_MODE_A_SIZE     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_AUTO_REJECT_AFTER_TIMEOUT_A_INDEX           0x110  ///< Controls whether the EQ logic eliminates requested coefficients that led to loss of symbol lock.
#define STRAP_BIF_LC_AUTO_REJECT_AFTER_TIMEOUT_A_SIZE            1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_EX_SEARCH_TRAVERSAL_MODE_A_INDEX            0x111  ///< Selects the path traversed by the equalization algorithm during exhaustive search. Applies to all speeds.
#define STRAP_BIF_LC_EX_SEARCH_TRAVERSAL_MODE_A_SIZE             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_REFCLK_OFF_NO_RCVR_LANES_A_INDEX            0x112  ///< Controls if refClkReq is de-asserted on lanes if they are turned off due to not detecting a receiver.
#define STRAP_BIF_LC_REFCLK_OFF_NO_RCVR_LANES_A_SIZE             1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_USE_LONG_SERIAL_QUICKSIM_TIMEOUTS_A_INDEX   0x113  ///< Controls if longer timeout values are used in quicksim mode. Should be set to 1 in simulation only.
#define STRAP_BIF_LC_USE_LONG_SERIAL_QUICKSIM_TIMEOUTS_A_SIZE    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DE_EMPHASIS_SEL_A_INDEX                        0x114  ///< STRAP_BIF_DE_EMPHASIS_SEL_A
#define STRAP_BIF_DE_EMPHASIS_SEL_A_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_RTM1_PRESENCE_DET_SUPP_A_INDEX              0x115  ///< Retimer Presence Detect Supported
#define STRAP_BIF_LC_RTM1_PRESENCE_DET_SUPP_A_SIZE               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_RTM2_PRESENCE_DET_SUPP_A_INDEX              0x116  ///< Two Retimers Presence Detect Supported
#define STRAP_BIF_LC_RTM2_PRESENCE_DET_SUPP_A_SIZE               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_TEST_TIMER_SEL_A_INDEX                      0x117  ///< STRAP_BIF_LC_TEST_TIMER_SEL_A
#define STRAP_BIF_LC_TEST_TIMER_SEL_A_SIZE                       2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_MARG_USES_SOFTWARE_A_INDEX                  0x118  ///< Margining uses Driver Software
#define STRAP_BIF_LC_MARG_USES_SOFTWARE_A_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_AUTO_DISABLE_SPEED_SUPPORT_EN_A_INDEX       0x119  ///< STRAP_BIF_LC_AUTO_DISABLE_SPEED_SUPPORT_EN_A
#define STRAP_BIF_LC_AUTO_DISABLE_SPEED_SUPPORT_EN_A_SIZE        1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SPC_MODE_2P5GT_A_INDEX                      0x11a  ///< STRAP_BIF_LC_SPC_MODE_2P5GT_A
#define STRAP_BIF_LC_SPC_MODE_2P5GT_A_SIZE                       2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SPC_MODE_5GT_A_INDEX                        0x11b  ///< STRAP_BIF_LC_SPC_MODE_5GT_A
#define STRAP_BIF_LC_SPC_MODE_5GT_A_SIZE                         2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SPC_MODE_8GT_A_INDEX                        0x11c  ///< STRAP_BIF_LC_SPC_MODE_8GT_A
#define STRAP_BIF_LC_SPC_MODE_8GT_A_SIZE                         2     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_SPC_MODE_16GT_A_INDEX                       0x11d  ///< STRAP_BIF_LC_SPC_MODE_16GT_A
#define STRAP_BIF_LC_SPC_MODE_16GT_A_SIZE                        2     ///< WriteDis = N, DefaultVal = 0x2

#define STRAP_BIF_LC_SPC_MODE_32GT_A_INDEX                       0x11e  ///< STRAP_BIF_LC_SPC_MODE_32GT_A
#define STRAP_BIF_LC_SPC_MODE_32GT_A_SIZE                        2     ///< WriteDis = N, DefaultVal = 0x2

#define STRAP_BIF_LC_SRIS_EN_A_INDEX                             0x11f  ///< STRAP_BIF_LC_SRIS_EN_A
#define STRAP_BIF_LC_SRIS_EN_A_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SRIS_AUTODETECT_EN_A_INDEX                  0x120  ///< STRAP_BIF_LC_SRIS_AUTODETECT_EN_A
#define STRAP_BIF_LC_SRIS_AUTODETECT_EN_A_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_TX_SWING_A_INDEX                            0x121  ///< STRAP_BIF_LC_TX_SWING_A
#define STRAP_BIF_LC_TX_SWING_A_SIZE                             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ACCEPT_ALL_PRESETS_A_INDEX                  0x122  ///< STRAP_BIF_LC_ACCEPT_ALL_PRESETS_A
#define STRAP_BIF_LC_ACCEPT_ALL_PRESETS_A_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ACCEPT_ALL_PRESETS_TEST_A_INDEX             0x123  ///< STRAP_BIF_LC_BIF_LC_ACCEPT_ALL_PRESETS_TEST_A
#define STRAP_BIF_LC_ACCEPT_ALL_PRESETS_TEST_A_SIZE              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_FOM_TIME_A_INDEX                            0x124  ///< STRAP_BIF_LC_FOM_TIME_A
#define STRAP_BIF_LC_FOM_TIME_A_SIZE                             2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SAFE_EQ_SEARCH_A_INDEX                      0x125  ///< STRAP_BIF_LC_SAFE_EQ_SEARCH_A
#define STRAP_BIF_LC_SAFE_EQ_SEARCH_A_SIZE                       1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_SET_TRANSMITTER_PRECODE_REQUEST_A_INDEX     0x126  ///< Controls if a Precoding Request will be sent to the link partner during the next 32GT Equalization procedure.
#define STRAP_BIF_LC_SET_TRANSMITTER_PRECODE_REQUEST_A_SIZE      1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LC_MISC_PORT_RESERVED_A_INDEX                  0x127  ///< RESERVED
#define BIF_STRAP_LC_MISC_PORT_RESERVED_A_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_TRAINING_BITS_REQUIRED_A_INDEX              0x128  ///< Controls the number of consecutive sets with matching Training Control bits that must be received in Configuration and Recovery.Idle prior to transition to training state.
#define STRAP_BIF_LC_TRAINING_BITS_REQUIRED_A_SIZE               2     ///< WriteDis = N, DefaultVal = 0x2

#define STRAP_BIF_LC_SAVE_RESTORE_EN_A_INDEX                     0x129  ///< Controls if the controller comes out of detect in coefficient Restore mode. If set to 1 software must perform the restore procedure before any speed changes to Gen3 of Gen4 can be made.
#define STRAP_BIF_LC_SAVE_RESTORE_EN_A_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_RXRECOVER_EN_A_INDEX                        0x12a  ///< Controls if the MAC_PCS_rxRecover signal can be generated for reset of receiver CDR logic.
#define STRAP_BIF_LC_RXRECOVER_EN_A_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_RXRECOVER_IN_POLL_ACTIVE_EN_A_INDEX         0x12b  ///< Controls if RxRecover can be asserted in Polling.Active if no TS1s are seen. Only applies if LC_RXRECOVER_EN is also set to 1.
#define STRAP_BIF_LC_RXRECOVER_IN_POLL_ACTIVE_EN_A_SIZE          1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_RXRECOVER_IN_CONFIG_EN_A_INDEX              0x12c  ///< Controls if RxRecover can be asserted in Configuration states if TS1s are expected
#define STRAP_BIF_LC_RXRECOVER_IN_CONFIG_EN_A_SIZE               1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_DSC_CHECK_COEFFS_IN_RLOCK_A_INDEX           0x12d  ///< Controls if received coefficient values are checked with expected final coefficients after equalization.
#define STRAP_BIF_LC_DSC_CHECK_COEFFS_IN_RLOCK_A_SIZE            1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_LC_EXTEND_EIEOS_MODE_A_INDEX                   0x12e  ///< Controls when the Reset EIEOS Interval Count bit is asserted when PCIE_LC_CNTL4.LC_EXTEND_EIEOS is 1.
#define STRAP_BIF_LC_EXTEND_EIEOS_MODE_A_SIZE                    1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ALT_RX_EQ_IN_PROGRESS_EN_A_INDEX            0x12f  ///< Controls when RxEqInProgress is used to indicate that Link partner Tx settings may be in flux.
#define STRAP_BIF_LC_ALT_RX_EQ_IN_PROGRESS_EN_A_SIZE             1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_EXTEND_EIEOS_A_INDEX                        0x130  ///< Controls the value of Reset EIEOS Interval Count bit in any subsequently transmitted TS1s at 8GT/s or 16GT/s.
#define STRAP_BIF_LC_EXTEND_EIEOS_A_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ALWAYS_PERFORM_GEN3_PRESET_CONVERSION_A_INDEX  0x131  ///< Controls if Gen3 preset conversion is always done
#define STRAP_BIF_LC_ALWAYS_PERFORM_GEN3_PRESET_CONVERSION_A_SIZE  1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ALWAYS_PERFORM_GEN4_PRESET_CONVERSION_A_INDEX  0x132  ///< Controls if Gen4 preset conversion is always done
#define STRAP_BIF_LC_ALWAYS_PERFORM_GEN4_PRESET_CONVERSION_A_SIZE  1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ALWAYS_PERFORM_GEN5_PRESET_CONVERSION_A_INDEX  0x133  ///< Controls if Gen5 preset conversion is always done
#define STRAP_BIF_LC_ALWAYS_PERFORM_GEN5_PRESET_CONVERSION_A_SIZE  1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_8GT_A_INDEX           0x134  ///< STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_8GT_A
#define STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_8GT_A_SIZE            2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_16GT_A_INDEX          0x135  ///< STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_16GT_A
#define STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_16GT_A_SIZE           2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_32GT_A_INDEX          0x136  ///< STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_32GT_A
#define STRAP_BIF_LC_ENH_PRESET_SEARCH_SEL_32GT_A_SIZE           2     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LC_MISC_PORT2_RESERVED_A_INDEX                 0x137  ///< RESERVED
#define BIF_STRAP_LC_MISC_PORT2_RESERVED_A_SIZE                  13    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_PRESET_MASK_8GT_A_INDEX                     0x138  ///< STRAP_BIF_LC_PRESET_MASK_8GT_A
#define STRAP_BIF_LC_PRESET_MASK_8GT_A_SIZE                      10    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_PRESET_MASK_16GT_A_INDEX                    0x139  ///< STRAP_BIF_LC_PRESET_MASK_16GT_A
#define STRAP_BIF_LC_PRESET_MASK_16GT_A_SIZE                     10    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_PRESET_MASK_32GT_A_INDEX                    0x13a  ///< STRAP_BIF_LC_PRESET_MASK_32GT_A
#define STRAP_BIF_LC_PRESET_MASK_32GT_A_SIZE                     10    ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LC_MISC_PORT3_A_RESERVED_INDEX                 0x13b  ///< BIF_STRAP_MISC_LC_PORT2_A_RESERVED
#define BIF_STRAP_LC_MISC_PORT3_A_RESERVED_SIZE                  2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_POISONED_ADVISORY_NONFATAL_A_INDEX             0x13c  ///< STRAP_BIF_POISONED_ADVISORY_NONFATAL_A
#define STRAP_BIF_POISONED_ADVISORY_NONFATAL_A_SIZE              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_MAX_PAYLOAD_SUPPORT_A_INDEX                    0x13d  ///< STRAP_BIF_MAX_PAYLOAD_SUPPORT_A
#define STRAP_BIF_MAX_PAYLOAD_SUPPORT_A_SIZE                     3     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_FIRST_RCVD_ERR_LOG_A_INDEX                     0x13e  ///< STRAP_BIF_FIRST_RCVD_ERR_LOG_A
#define STRAP_BIF_FIRST_RCVD_ERR_LOG_A_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_EXTENDED_FMT_SUPPORTED_A_INDEX                 0x13f  ///< STRAP_BIF_EXTENDED_FMT_SUPPORTED_A
#define STRAP_BIF_EXTENDED_FMT_SUPPORTED_A_SIZE                  1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_E2E_PREFIX_EN_A_INDEX                          0x140  ///< STRAP_BIF_E2E_PREFIX_EN_A
#define STRAP_BIF_E2E_PREFIX_EN_A_SIZE                           1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_BCH_ECC_EN_A_INDEX                             0x141  ///< STRAP_BIF_BCH_ECC_EN_A
#define STRAP_BIF_BCH_ECC_EN_A_SIZE                              1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_MC_ECRC_REGEN_SUPP_A_INDEX                     0x142  ///< STRAP_BIF_MC_ECRC_REGEN_SUPP_A
#define STRAP_BIF_MC_ECRC_REGEN_SUPP_A_SIZE                      1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_LOWER_SKP_OS_GEN_SUPPORT_A_INDEX                   0x143  ///< STRAP_LOWER_SKP_OS_GEN_SUPPORT_A
#define STRAP_LOWER_SKP_OS_GEN_SUPPORT_A_SIZE                    5     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_LOWER_SKP_OS_RCV_SUPPORT_A_INDEX                   0x144  ///< STRAP_LOWER_SKP_OS_RCV_SUPPORT_A
#define STRAP_LOWER_SKP_OS_RCV_SUPPORT_A_SIZE                    5     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A_INDEX        0x145  ///< DEVICE_CAP2.TEN_BIT_TAG_COMPLETER_SUPPORTED[16]. PCIe Base r4.0 feature.
#define STRAP_BIF_TEN_BIT_TAG_COMPLETER_SUPPORTED_A_SIZE         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A_INDEX        0x146  ///< DEVICE_CAP2.TEN_BIT_TAG_REQUESTER_SUPPORTED[17]. PCIe Base r4.0 feature.
#define STRAP_BIF_TEN_BIT_TAG_REQUESTER_SUPPORTED_A_SIZE         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CCIX_EN_A_INDEX                                0x147  ///< CCIX Transport Designated Vendor-Specific Extended Capability
#define STRAP_BIF_CCIX_EN_A_SIZE                                 1     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_MISC_PORT_A_RESERVED_INDEX                     0x148  ///< BIF_STRAP_MISC_PORT_A_RESERVED
#define BIF_STRAP_MISC_PORT_A_RESERVED_SIZE                      12    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LANE_NEGOTIATION_A_INDEX                       0x149  ///< Link Negotiation Modes: 0 = Compliant mode; widest possible link 1 = Compliant mode; fix missing lane 0 2 = Compliant mode; reverse only  3 = Compliant mode; reverse only; don't require the sets to be contiguous 4 = Old mode; reverse only 5 = Easy training mode; reverse only - means to train reliably chacking as little as possible to get up. 6 = Reliable mode; reverse only - means to reliably train; in a reliable system 7 = Reserved.
#define STRAP_BIF_LANE_NEGOTIATION_A_SIZE                        3     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_LINK_TRAINING_A_RESERVED_1_INDEX               0x14a  ///< BIF_STRAP_LINK_TRAINING_A_RESERVED_1
#define BIF_STRAP_LINK_TRAINING_A_RESERVED_1_SIZE                4     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_BYPASS_RCVR_DET_A_INDEX                        0x14b  ///< STRAP_BIF_BYPASS_RCVR_DET_A
#define STRAP_BIF_BYPASS_RCVR_DET_A_SIZE                         1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_FORCE_COMPLIANCE_A_INDEX                       0x14c  ///< STRAP_BIF_FORCE_COMPLIANCE_A
#define STRAP_BIF_FORCE_COMPLIANCE_A_SIZE                        1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_COMPLIANCE_DIS_A_INDEX                         0x14d  ///< STRAP_BIF_COMPLIANCE_DIS_A
#define STRAP_BIF_COMPLIANCE_DIS_A_SIZE                          1     ///< WriteDis = Y, DefaultVal = 0x0

#define STRAP_BIF_LC_X12_NEGOTIATION_DIS_A_INDEX                 0x14e  ///< STRAP_BIF_LC_X12_NEGOTIATION_DIS_A
#define STRAP_BIF_LC_X12_NEGOTIATION_DIS_A_SIZE                  1     ///< WriteDis = Y, DefaultVal = 0x1

#define STRAP_BIF_REVERSE_LANES_A_INDEX                          0x14f  ///< STRAP_BIF_REVERSE_LANES_A
#define STRAP_BIF_REVERSE_LANES_A_SIZE                           1     ///< WriteDis = Y, DefaultVal = 0x0

#define BIF_STRAP_LINK_TRAINING_A_RESERVED_2_INDEX               0x150  ///< BIF_STRAP_LINK_TRAINING_A_RESERVED_2
#define BIF_STRAP_LINK_TRAINING_A_RESERVED_2_SIZE                1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LC_ENHANCED_HOT_PLUG_EN_A_INDEX                0x151  ///< STRAP_BIF_LC_ENHANCED_HOT_PLUG_EN_A
#define STRAP_BIF_LC_ENHANCED_HOT_PLUG_EN_A_SIZE                 1     ///< WriteDis = Y, DefaultVal = 0x0

#define BIF_STRAP_LINK_TRAINING_A_RESERVED_3_INDEX               0x152  ///< BIF_STRAP_LINK_TRAINING_A_RESERVED_3
#define BIF_STRAP_LINK_TRAINING_A_RESERVED_3_SIZE                11    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_FTS_yTSx_COUNT_A_INDEX                         0x153  ///< STRAP_BIF_FTS_yTSx_COUNT_A
#define STRAP_BIF_FTS_yTSx_COUNT_A_SIZE                          2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_INITIAL_N_FTS_A_INDEX                          0x154  ///< STRAP_BIF_INITIAL_N_FTS_A
#define STRAP_BIF_INITIAL_N_FTS_A_SIZE                           8     ///< WriteDis = N, DefaultVal = 0x18

#define STRAP_BIF_ROM_DEVICE_ID_A_INDEX                          0x155  ///< STRAP_BIF_ROM_DEVICE_ID_A //change value to 0x0 so defined device_id can be picked up from wrapper_misc file
#define STRAP_BIF_ROM_DEVICE_ID_A_SIZE                           16    ///< WriteDis = N, DefaultVal = 0x0

#define PORT_IS_SB_A_INDEX                                       0x156  ///< PORT_IS_SB_A
#define PORT_IS_SB_A_SIZE                                        1     ///< WriteDis = N, DefaultVal = 0x0

#define PCIE_WRP_PORT_MISC_STRAP_A_RESERVED_INDEX                0x157  ///< PCIE_WRP_PORT_MISC_STRAP_A_RESERVED
#define PCIE_WRP_PORT_MISC_STRAP_A_RESERVED_SIZE                 15    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_PCI_PM_L1_2_SUPPORTED_A_INDEX                  0x158  ///< PCI-PM L1.2 is supported when Set
#define STRAP_BIF_PCI_PM_L1_2_SUPPORTED_A_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_PCI_PM_L1_1_SUPPORTED_A_INDEX                  0x159  ///< PCI-PM L1.1 is supported when Set
#define STRAP_BIF_PCI_PM_L1_1_SUPPORTED_A_SIZE                   1     ///< WriteDis = N, DefaultVal = 0x1

#define STRAP_BIF_ASPM_L1_2_SUPPORTED_A_INDEX                    0x15a  ///< ASPM L1.2 is supported when Set
#define STRAP_BIF_ASPM_L1_2_SUPPORTED_A_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_ASPM_L1_1_SUPPORTED_A_INDEX                    0x15b  ///< ASPM L1.1 is supported when Set
#define STRAP_BIF_ASPM_L1_1_SUPPORTED_A_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A_INDEX              0x15c  ///< STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A
#define STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A_SIZE               1     ///< WriteDis = N, DefaultVal = 0x1

#define BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_1_INDEX             0x15d  ///< BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_1
#define BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_1_SIZE              3     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_CM_RESTORE_TIME_A_INDEX                        0x15e  ///< Sets value of Tcommonmode (in us) which must be used by the Downstream Port for timing the re-establishment of common mode
#define STRAP_BIF_CM_RESTORE_TIME_A_SIZE                         8     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_T_P_ON_SCALE_A_INDEX                           0x15f  ///< STRAP_BIF_T_P_ON_SCALE_A
#define STRAP_BIF_T_P_ON_SCALE_A_SIZE                            2     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_2_INDEX             0x160  ///< BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_2
#define BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_2_SIZE              2     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_T_P_ON_VALUE_A_INDEX                           0x161  ///< STRAP_BIF_T_P_ON_VALUE_A
#define STRAP_BIF_T_P_ON_VALUE_A_SIZE                            5     ///< WriteDis = N, DefaultVal = 0x5

#define BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_3_INDEX             0x162  ///< BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_3
#define BIF_STRAP_PM_P1_SUBSTATES_A_RESERVED_3_SIZE              7     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LOCAL_DLF_SUPPORTED_A_INDEX                    0x163  ///< Data Link Feature Capabilities - Local Data Link Feature Supported (Base 4.0 compliance)
#define STRAP_BIF_LOCAL_DLF_SUPPORTED_A_SIZE                     1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_DLF_EXCHANGE_EN_A_INDEX                        0x164  ///< Data Link Feature Capabilities - Data Link Feature Exchange Enable (Base 4.0 compliance)
#define STRAP_BIF_DLF_EXCHANGE_EN_A_SIZE                         1     ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_LOCAL_DLF_HDR_SCALE_MODE_A_INDEX               0x165  ///< STRAP_BIF_LOCAL_DLF_HDR_SCALE_MODE
#define STRAP_BIF_LOCAL_DLF_HDR_SCALE_MODE_A_SIZE                2     ///< WriteDis = N, DefaultVal = 0x0

#define BIF_STRAP_DLF_CAP_A_RESERVED_1_INDEX                     0x166  ///< RESERVED for Data Link Feature Capabilities
#define BIF_STRAP_DLF_CAP_A_RESERVED_1_SIZE                      28    ///< WriteDis = N, DefaultVal = 0x0

#define STRAP_BIF_PORT_OFFSET_A_INDEX                            0x167  ///< STRAP_BIF_PORT_OFFSET_A
#define STRAP_BIF_PORT_OFFSET_A_SIZE                             4     ///< WriteDis = N, DefaultVal = 0x0

#define HOLD_TRAINING_B_INDEX                                    0x168  ///< HOLD_TRAINING_B

#endif /* _PCIE_STRAPS_H_ */

