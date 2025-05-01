/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIO_WORKAROUND_TABLE_H_
#define _NBIO_WORKAROUND_TABLE_H_

// --------------------------------------------------
// ACP Sub Class
// --------------------------------------------------
  #define NBIO_WA_ACP_SUB_CLASS_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_NBIF0INTERNAL_RCC_DEV0_EPF5_STRAP13_ADDRESS, \
      RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F5_MASK, \
      (0x80 << RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F5_OFFSET) \
      ),


// --------------------------------------------------
// PCIE CV test
// --------------------------------------------------
  #define NBIO_WA_PCI_CV_TEST_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_DEV0_NBIF0PORT0DEC_DN_PCIE_CNTL_ADDRESS, \
      DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK, \
      (0x1 << DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_DEV1_NBIF0PORT1DEC_DN_PCIE_CNTL_ADDRESS, \
      DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK, \
      (0x1 << DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_DEV2_NBIF0PORT2DEC_DN_PCIE_CNTL_ADDRESS, \
      DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK, \
      (0x1 << DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET) \
      ),


// --------------------------------------------------
// Completion Timeout feature
// --------------------------------------------------
  #define NBIO_WA_CPL_TIMEOUT_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP3_ADDRESS, \
      RCC_DEV0_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV0_MASK, \
      (0x1 << RCC_DEV0_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV0_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP3_ADDRESS, \
      RCC_DEV1_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV1_MASK, \
      (0x1 << RCC_DEV1_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV1_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP3_ADDRESS, \
      RCC_DEV2_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV2_MASK, \
      (0x1 << RCC_DEV2_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV2_OFFSET) \
      ),

// --------------------------------------------------
// RIOMMU disable timeout
// --------------------------------------------------
  // RIOMMU::RIOMMU_MISC_CNTRL_3
  // RIOMMU::SHDW_RIOMMU_INV_CNTRL
  // RIOMMU::RIOMMU_REFCLK_MODE
  #define NBIO_WA_RIOMMU_DIS_TIMEOUT_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_RIOMMU_MISC_CNTRL_3_ADDRESS, \
      RIOMMU_MISC_CNTRL_3_INV_TIMEOUT_MASK, \
      (0x0 << RIOMMU_MISC_CNTRL_3_INV_TIMEOUT_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_SHDW_RIOMMU_INV_CNTRL_ADDRESS, \
      SHDW_RIOMMU_INV_CNTRL_MULTI_INV_LIM_MASK | \
      SHDW_RIOMMU_INV_CNTRL_MULTI_INVACK_LIM_MASK, \
      (0x0 << SHDW_RIOMMU_INV_CNTRL_MULTI_INV_LIM_OFFSET) | \
      (0x0 << SHDW_RIOMMU_INV_CNTRL_MULTI_INVACK_LIM_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_RIOMMU_REFCLK_MODE_ADDRESS, \
      RIOMMU_REFCLK_MODE_MODE_100MHZ_MASK | \
      RIOMMU_REFCLK_MODE_MODE_25MHZ_MASK, \
      (0x0 << RIOMMU_REFCLK_MODE_MODE_100MHZ_OFFSET) | \
      (0x1 << RIOMMU_REFCLK_MODE_MODE_25MHZ_OFFSET) \
      ),


// --------------------------------------------------
// Program rIOMMU TW Coherent bit
// --------------------------------------------------
  #define NBIO_WA_RIOMMU_TW_COHERENT_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_RIOMMU_TW_CONTROL_ADDRESS, \
      RIOMMU_TW_CONTROL_TWForceCoherent_MASK, \
      (1 << RIOMMU_TW_CONTROL_TWForceCoherent_OFFSET) \
      ),


// --------------------------------------------------
// NBIFMM::BIFC_MISC_CTRL0
// --------------------------------------------------
  // PME_TURNOFF_MODE
  #define NBIO_WA_PME_TURNOFF_MODE_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_NBIF0_BIFC_MISC_CTRL0_ADDRESS, \
      BIFC_MISC_CTRL0_PME_TURNOFF_MODE_MASK, \
      (1 << BIFC_MISC_CTRL0_PME_TURNOFF_MODE_OFFSET) \
      ),

// --------------------------------------------------
// PCIECORE::RSMU_MASTER_CONTROL
// --------------------------------------------------
  // RSMU_MASTER_MESSAGE_SEND_ENABLE
  #define NBIO_WA_RSMU_MASTER_MESSAGE_SEND_ENABLE_TBL \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_PCIE0_RSMU_MASTER_CONTROL_ADDRESS, \
      RSMU_MASTER_CONTROL_RSMU_MASTER_MESSAGE_SEND_ENABLE_MASK, \
      (1 << RSMU_MASTER_CONTROL_RSMU_MASTER_MESSAGE_SEND_ENABLE_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ONE_ADDR_TYPE, \
      SMN_PCIE1_RSMU_MASTER_CONTROL_ADDRESS, \
      RSMU_MASTER_CONTROL_RSMU_MASTER_MESSAGE_SEND_ENABLE_MASK, \
      (1 << RSMU_MASTER_CONTROL_RSMU_MASTER_MESSAGE_SEND_ENABLE_OFFSET) \
      ),


#endif /* _NBIO_WORKAROUND_TABLE_H_ */
