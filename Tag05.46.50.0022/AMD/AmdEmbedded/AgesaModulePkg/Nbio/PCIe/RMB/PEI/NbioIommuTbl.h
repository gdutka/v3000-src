/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIO_IOMMU_TABLE_H_
#define _NBIO_IOMMU_TABLE_H_
// --------------------------------------------------
// 13.3.1.2.2 IOMMU L1 Initialization
// --------------------------------------------------

  // IOMMUL1::L1_FEATURE_CNTRL[EXE_lock_bit] = 1h.
  // IOMMUL1::L1_FEATURE_CNTRL[PMR_lock_bit] = 1h.
  #define NBIO_IOMMU_L1_INIT_TBL \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_IOAGR_L1_FEATURE_CNTRL_ADDRESS, \
      L1_FEATURE_CNTRL_EXE_lock_bit_MASK | \
      L1_FEATURE_CNTRL_PMR_lock_bit_MASK, \
      1 << L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET | \
      1 << L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_PCIE0_L1_FEATURE_CNTRL_ADDRESS, \
      L1_FEATURE_CNTRL_EXE_lock_bit_MASK | \
      L1_FEATURE_CNTRL_PMR_lock_bit_MASK, \
      1 << L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET | \
      1 << L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET \
      ),
  // End of NBIO_IOMMU_L1_INIT_TBL


// --------------------------------------------------
// 13.3.1.2.3 IOMMU L2 Initialization
// --------------------------------------------------

  // IOMMUL2::L2_ERR_RULE_CONTROL_0[ERRRuleLock0] = 1h
  // IOMMUL2::L2_ERR_RULE_CONTROL_3[ERRRuleLock1] = 1h.
  // IOMMUL2::L2_RIOMMU_INV_CNTRL[RIOMMU_MULTI_INVACK_LIM] = 1Fh.
  // IOMMUL2::L2_RIOMMU_INV_CNTRL[RIOMMU_MULTI_INV_LIM]=1Fh.
  #define NBIO_IOMMU_L2_INIT_TBL \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_L2_ERR_RULE_CONTROL_0_ADDRESS, \
      L2_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK, \
      0x1 << L2_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_L2_ERR_RULE_CONTROL_3_ADDRESS, \
      L2_ERR_RULE_CONTROL_3_ERRRuleLock1_MASK, \
      0x1 << L2_ERR_RULE_CONTROL_3_ERRRuleLock1_OFFSET \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_L2_RIOMMU_INV_CNTRL_ADDRESS, \
      L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INVACK_LIM_MASK | \
      L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INV_LIM_MASK, \
      (0x0 << L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INVACK_LIM_OFFSET) | \
      (0x0 << L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INV_LIM_OFFSET) \
      ),
  // End of NBIO_IOMMU_L2_INIT_TBL


// --------------------------------------------------
// 13.3.1.2.4 IOMMU Clock Gating
// --------------------------------------------------

  // set default of L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable to 1

  // IOMMUL1::L1_CLKCNTRL_0[L1_CLKGATE_HYSTERESIS] = 20h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_DMA_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_CACHE_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_PERF_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_MEMORY_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_REG_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_HOSTREQ_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_DMARSP_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_HOSTRSP_CLKGATE_EN] = 1h.
  // IOMMUL1::L1_CLKCNTRL_0[L1_L2_CLKGATE_EN] = 1h.
  // IOMMUL2::L2_L2A_CK_GATE_CONTROL[CKGateL2ARegsDisable] = 0h.
  // IOMMUL2::L2_L2A_CK_GATE_CONTROL[CKGateL2ADynamicDisable] = 0h.
  // IOMMUL2::L2_L2A_CK_GATE_CONTROL[CKGateL2ACacheDisable] = 0h.
  // IOMMUL2::L2_L2B_CK_GATE_CONTROL[CKGateL2BRegsDisable] = 0h.
  // IOMMUL2::L2_L2B_CK_GATE_CONTROL[CKGateL2BDynamicDisable] = 0h.
  // IOMMUL2::L2_L2B_CK_GATE_CONTROL[CKGateL2BMiscDisable] = 0h.
  // IOMMUL2::L2_L2B_CK_GATE_CONTROL[CKGateL2BCacheDisable] = 0h.
  #define NBIO_IOMMU_CLOCK_GATING_TBL \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_L2_L2B_CK_GATE_CONTROL_ADDRESS, \
      L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK, \
      (0x1 << L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdIommuL1ClockGatingEnable), \
      TRUE, \
      SMN_IOAGR_L1_CLKCNTRL_0_ADDRESS, \
      L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK | \
      L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK | \
      L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK | \
      L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | \
      L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK, \
      (0x20 << L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdIommuL1ClockGatingEnable), \
      TRUE, \
      SMN_PCIE0_L1_CLKCNTRL_0_ADDRESS, \
      L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK | \
      L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK | \
      L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK | \
      L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | \
      L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK, \
      (0x20 << L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) | \
      (0x1 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdIommuL2ClockGatingEnable), \
      TRUE, \
      SMN_L2_L2A_CK_GATE_CONTROL_ADDRESS, \
      L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK | L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK | L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK, \
      (0x0 << L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET) | \
      (0x0 << L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET) | \
      (0x0 << L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdIommuL2ClockGatingEnable), \
      TRUE, \
      SMN_L2_L2B_CK_GATE_CONTROL_ADDRESS, \
      L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK | L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK | \
      L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK | L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK, \
      (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET) | \
      (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET) | \
      (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET) | \
      (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET) \
      ),
  // End of NBIO_IOMMU_CLOCK_GATING_TBL


// --------------------------------------------------
// 13.3.1.2.6 IOMMU L2 Dynamic Power Gating
// --------------------------------------------------

  // IOMMUL2::L2_PWRGATE_CNTRL_REG_0[IP_PG_thres] = 800h.
  // IOMMUL2::L2_PWRGATE_CNTRL_REG_3[IP_PG_en] = 1h.
  // IOMMUL2::L2_L2A_MEMPWR_GATE_1[L2AREG_CACHE_PGMEM_SEL] = 1h.
  // IOMMUL2::L2_L2B_MEMPWR_GATE_1[L2BREG_CACHE_PGMEM_SEL] = 1h.
  #define NBIO_IOMMU_L2_DYNAMIC_POWER_GATING_TBL \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUDynamicPgEnable), \
      TRUE, \
      SMN_L2_PWRGATE_CNTRL_REG_0_ADDRESS, \
      L2_PWRGATE_CNTRL_REG_0_IP_PG_thres_MASK, \
      (0x800 << L2_PWRGATE_CNTRL_REG_0_IP_PG_thres_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUDynamicPgEnable), \
      TRUE, \
      SMN_L2_PWRGATE_CNTRL_REG_3_ADDRESS, \
      L2_PWRGATE_CNTRL_REG_3_IP_PG_en_MASK, \
      (0x1 << L2_PWRGATE_CNTRL_REG_3_IP_PG_en_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUDynamicPgEnable), \
      TRUE, \
      SMN_L2_L2A_MEMPWR_GATE_1_ADDRESS, \
      L2_L2A_MEMPWR_GATE_1_L2AREG_CACHE_PGMEM_SEL_MASK, \
      (0x1 << L2_L2A_MEMPWR_GATE_1_L2AREG_CACHE_PGMEM_SEL_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUL2MemoryPGEnable), \
      TRUE, \
      SMN_L2_L2B_MEMPWR_GATE_1_ADDRESS, \
      L2_L2B_MEMPWR_GATE_1_L2BREG_CACHE_PGMEM_SEL_MASK, \
      (0x1 << L2_L2B_MEMPWR_GATE_1_L2BREG_CACHE_PGMEM_SEL_OFFSET) \
      ),
  // End of NBIO_IOMMU_L2_DYNAMIC_POWER_GATING_TBL


// --------------------------------------------------
// 13.3.1.2.7 IOMMU L2 Memory Power Gating
// --------------------------------------------------

  // IOMMUL2::L2_L2A_MEMPWR_GATE_1[L2AREG_LS_EN] = 1h.
  // IOMMUL2::L2_L2A_MEMPWR_GATE_1[L2AREG_DS_EN] = 1h.
  // IOMMUL2::L2_L2A_MEMPWR_GATE_1[L2AREG_SD_EN] = 1h.
  // IOMMUL2::L2_L2B_MEMPWR_GATE_1[L2BREG_LS_EN] = 1h.
  // IOMMUL2::L2_L2B_MEMPWR_GATE_1[L2BREG_DS_EN] = 1h.
  // IOMMUL2::L2_L2B_MEMPWR_GATE_1[L2BREG_SD_EN] = 1h.
  #define NBIO_IOMMU_L2_MEMORY_POWER_GATING_TBL \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUL2MemoryPGEnable), \
      TRUE, \
      SMN_L2_L2A_MEMPWR_GATE_1_ADDRESS, \
      L2_L2A_MEMPWR_GATE_1_L2AREG_LS_EN_MASK | \
      L2_L2A_MEMPWR_GATE_1_L2AREG_DS_EN_MASK | \
      L2_L2A_MEMPWR_GATE_1_L2AREG_SD_EN_MASK, \
      (0x1 << L2_L2A_MEMPWR_GATE_1_L2AREG_LS_EN_OFFSET) | \
      (0x1 << L2_L2A_MEMPWR_GATE_1_L2AREG_DS_EN_OFFSET) | \
      (0x1 << L2_L2A_MEMPWR_GATE_1_L2AREG_SD_EN_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUL2MemoryPGEnable), \
      TRUE, \
      SMN_L2_L2B_MEMPWR_GATE_1_ADDRESS, \
      L2_L2B_MEMPWR_GATE_1_L2BREG_LS_EN_MASK | \
      L2_L2B_MEMPWR_GATE_1_L2BREG_DS_EN_MASK | \
      L2_L2B_MEMPWR_GATE_1_L2BREG_SD_EN_MASK, \
      (0x1 << L2_L2B_MEMPWR_GATE_1_L2BREG_LS_EN_OFFSET) | \
      (0x1 << L2_L2B_MEMPWR_GATE_1_L2BREG_DS_EN_OFFSET) | \
      (0x1 << L2_L2B_MEMPWR_GATE_1_L2BREG_SD_EN_OFFSET) \
      ),
  // End of NBIO_IOMMU_L2_MEMORY_POWER_GATING_TBL


// --------------------------------------------------
// IOMMU L1 Memory Power Gating
// --------------------------------------------------
  // IOMMUL1::IOAGR::L1_PGMEM_CTRL_1[L1_LS_EN] = 1h
  // IOMMUL1::IOAGR::L1_PGMEM_CTRL_1[L1_DS_EN] = 1h
  // IOMMUL1::IOAGR::L1_PGMEM_CTRL_1[L1_SD_EN] = 1h
  // IOMMUL1::IOAGR::L1_PGMEM_CTRL_1[L1_IP_PGMEM_SEL] = 1h
  // IOMMUL1::PCIE0::L1_PGMEM_CTRL_1[L1_LS_EN] = 1h
  // IOMMUL1::PCIE0::L1_PGMEM_CTRL_1[L1_DS_EN] = 1h
  // IOMMUL1::PCIE0::L1_PGMEM_CTRL_1[L1_SD_EN] = 1h
  // IOMMUL1::PCIE0::L1_PGMEM_CTRL_1[L1_IP_PGMEM_SEL] = 1h
  #define NBIO_IOMMU_L1_MEMORY_POWER_GATING_TBL \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUL1MemoryPGEnable), \
      TRUE, \
      SMN_PCIE0_L1_PGMEM_CTRL_1_ADDRESS, \
      L1_PGMEM_CTRL_1_L1_LS_EN_MASK | \
      L1_PGMEM_CTRL_1_L1_DS_EN_MASK | \
      L1_PGMEM_CTRL_1_L1_SD_EN_MASK | \
      L1_PGMEM_CTRL_1_L1_IP_PGMEM_SEL_MASK, \
      (0x1 << L1_PGMEM_CTRL_1_L1_LS_EN_OFFSET) | \
      (0x1 << L1_PGMEM_CTRL_1_L1_DS_EN_OFFSET) | \
      (0x1 << L1_PGMEM_CTRL_1_L1_SD_EN_OFFSET) | \
      (0x1 << L1_PGMEM_CTRL_1_L1_IP_PGMEM_SEL_OFFSET) \
      ), \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdCfgIOMMUL1MemoryPGEnable), \
      TRUE, \
      SMN_IOAGR_L1_PGMEM_CTRL_1_ADDRESS, \
      L1_PGMEM_CTRL_1_L1_LS_EN_MASK | \
      L1_PGMEM_CTRL_1_L1_DS_EN_MASK | \
      L1_PGMEM_CTRL_1_L1_SD_EN_MASK | \
      L1_PGMEM_CTRL_1_L1_IP_PGMEM_SEL_MASK, \
      (0x1 << L1_PGMEM_CTRL_1_L1_LS_EN_OFFSET) | \
      (0x1 << L1_PGMEM_CTRL_1_L1_DS_EN_OFFSET) | \
      (0x1 << L1_PGMEM_CTRL_1_L1_SD_EN_OFFSET) | \
      (0x1 << L1_PGMEM_CTRL_1_L1_IP_PGMEM_SEL_OFFSET) \
      ),
  // End of NBIO_IOMMU_L1_MEMORY_POWER_GATING_TBL

#endif /* _NBIO_IOMMU_TABLE_H_ */
