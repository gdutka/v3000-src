/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdNbioPcieServicesProtocol.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Guid/HobList.h>
#include <Protocol/PciIo.h>
#include <Library/PcieConfigLib.h>
#include <Library/PcieMiscCommLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/GnbPciLib.h>
#include <GnbRegistersRMB.h>
#include <Library/SmnAccessLib.h>
#include "AmdNbioPcieAer.h"
#include "AmdNbioPcieDxe.h"
#include <Protocol/SocLogicalIdProtocol.h>
#include <Protocol/AmdNbioPcieAerProtocol.h>

#define FILECODE        NBIO_PCIE_RMB_DXE_AMDNBIOPCIEDXE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern AMD_NBIO_PCIE_AER_PROTOCOL mAmdNbioPcieAerProtocol;

VOID
PcieAspmBlackListInterface (
  IN       GNB_HANDLE  *GnbHandle
  );

VOID
PcieL1ExitLatencyInterface (
  IN       GNB_HANDLE  *GnbHandle
  );

EFI_STATUS
AmdPcieGetTopology (
  IN       DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL  *This,
     OUT   UINT32                               **DebugOptions
  )
{
  EFI_STATUS                           Status;
  GNB_PCIE_INFORMATION_DATA_HOB        *GnbPcieInfoDataHob;
  //
  // Get information data from HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &GnbPcieInfoDataHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  GnbPcieInfoDataHob = GetNextGuidHob (&gGnbPcieHobInfoGuid, GnbPcieInfoDataHob);
  if (GnbPcieInfoDataHob == NULL) {
    return EFI_NOT_FOUND;
  }

  *DebugOptions = (UINT32*) GnbPcieInfoDataHob;
  return Status;
}

DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL mDxeAmdNbioPcieServicesProtocol = {
  AmdPcieGetTopology
};

/*----------------------------------------------------------------------------------------*/
/**
 * Clock gating
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */

VOID
PciePwrClockGating (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_WRAPPER_CONFIG            *Wrapper;
  CPM_CONTROL_STRUCT             PcieDir460;
  PCIE_CONFIG_CNTL_STRUCT        PcieDir044;
  PCIE_SDP_CTRL_STRUCT           PcieCore18C;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  Wrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (Wrapper != NULL) {
    if (Wrapper->WrapId != 0xFF) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Enable Clock Gating for Wrapper %x\n", Wrapper->WrapId);
      // LCLK Gating
      SmnRegisterRead ( GnbHandle->Address.Address.Bus, WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_CPM_CONTROL_ADDRESS), &PcieDir460.Value);
      SmnRegisterRead ( GnbHandle->Address.Address.Bus, WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_CONFIG_CNTL_ADDRESS), &PcieDir044.Value);
      SmnRegisterRead ( GnbHandle->Address.Address.Bus, WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_SDP_CTRL_ADDRESS), &PcieCore18C.Value);

      if (Wrapper->Features.LclkGating == 1) {
        PcieDir460.Field.LCLK_DYN_GATE_ENABLE = 0x1;
        PcieDir460.Field.LCLK_GATE_TXCLK_FREE = 0x0;
        PcieDir460.Field.LCLK_DYN_GATE_LATENCY = 0x1;
        PcieDir460.Field.LCLK_GATE_ALLOW_IN_L1 = 0x1;
      }
      // TXCLK+REFCLK
      if (Wrapper->Features.ClkGating == 1) {
        // TXCLK_PERM gating enable
        PcieDir044.Field.DYN_CLK_LATENCY = 0xF;

        // TXCLK DYN gating enable
        PcieDir460.Field.TXCLK_DYN_GATE_ENABLE = 0x1;
        PcieDir460.Field.TXCLK_DYN_GATE_LATENCY = 0x1;

        // TXCLK REGS gating enable
        PcieDir460.Field.TXCLK_REGS_GATE_ENABLE = 0x1;
        PcieDir460.Field.TXCLK_REGS_GATE_LATENCY = 0x1;
        // TXCLK PRBS gating enable
        PcieDir460.Field.TXCLK_PRBS_GATE_ENABLE = 0x1;

        // TXCLK LCNT gating enable
        PcieDir460.Field.TXCLK_LCNT_GATE_ENABLE = 0x1;

        // TXCLK PIF1X gating enable
        // PcieDir460.Field.TXCLK_PIF_GATE_ENABLE = 0x1;

        // REFCLK REGS gating enable
        PcieDir460.Field.REFCLK_REGS_GATE_ENABLE = 0x1;
        PcieDir460.Field.REFCLK_XSTCLK_ENABLE = 0x0;
        PcieDir460.Field.REFCLK_REGS_GATE_LATENCY = 0x1;
        PcieDir460.Field.CLKREQb_UNGATE_TXCLK_ENABLE = 0x1;

        PcieDir460.Field.IGNORE_REGS_IDLE_IN_PG = 0x1;
      }

      SmnRegisterWrite (GnbHandle->Address.Address.Bus, WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_CPM_CONTROL_ADDRESS), &PcieDir460.Value, GNB_REG_ACC_FLAG_S3SAVE);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus, WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_CONFIG_CNTL_ADDRESS), &PcieDir044.Value, GNB_REG_ACC_FLAG_S3SAVE);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus, WRAP_SPACE(GnbHandle, Wrapper, SMN_PCIE0_PCIE_SDP_CTRL_ADDRESS), &PcieCore18C.Value, GNB_REG_ACC_FLAG_S3SAVE);
    }

    if (Wrapper->WrapId == 0) {
      // Core 0 (GFXcontroller)
      //
      // PCIe SDP (CI_SLV_REQR_FULL_DISCONNECT_EN, CI_SLV_REQR_PART_DISCONNECT_EN, CI_MSTSDP_CLKGATE_ONESIDED_ENABLE, CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL)
      //
      if (PcdGetBool(PcdGfxPcieSdpControl) == TRUE) {
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             SMN_PCIE0_PCIE_SDP_CTRL_ADDRESS,
             (UINT32) ~(PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_MASK |
             PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_MASK),
             (UINT32)(0 << PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_OFFSET |
             1 << PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_OFFSET),
             GNB_REG_ACC_FLAG_S3SAVE);
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             SMN_PCIE0_PCIE_MST_CTRL_2_ADDRESS,
             (UINT32) ~(PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_MASK |
             PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_MASK),
             (UINT32)(1 << PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_OFFSET |
             1 << PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_OFFSET),
             GNB_REG_ACC_FLAG_S3SAVE);
     } else {
       SmnRegisterRMW (GnbHandle->Address.Address.Bus,
            SMN_PCIE0_PCIE_SDP_CTRL_ADDRESS,
            (UINT32) ~(PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_MASK |
            PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_MASK),
            0,
            GNB_REG_ACC_FLAG_S3SAVE);
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             SMN_PCIE0_PCIE_MST_CTRL_2_ADDRESS,
             (UINT32) ~(PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_MASK |
             PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_MASK),
             0,
             GNB_REG_ACC_FLAG_S3SAVE);
      }
      //
      // TXCLK DYN PORT Gating:
      //
      // Enable: TXCLK_DYN_PORT_X_GATE_ENABLE LC_CPM_CONTROL_0[15:0] = 0xFFFF  Disable: (0x0000)
      // if (PcdGetBool (PcdGfxTxClkDynPortGating) == TRUE) {
      //   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
      //        NBIO_SPACE(GnbHandle, SMN_PCIE0_LC_CPM_CONTROL_0_ADDRESS),
      //        (UINT32) ~(0xFFFF),  //Bit0 to Bit15
      //        0xFFFF, GNB_REG_ACC_FLAG_S3SAVE);
      // }
      // // TXCLK DYN TR PORT Gating:
      // // TXCLK_DYN_TR_PORT_X_GATE_ENABLE, LC_CPM_CONTROL_0[31:16] = 0xFFFF
      // if (PcdGetBool (PcdGfxTxClkDynTrPortGating) == TRUE) {
      //   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
      //        NBIO_SPACE(GnbHandle, SMN_PCIE0_LC_CPM_CONTROL_0_ADDRESS),
      //        (UINT32) ~(0xFFFF0000),  //Bit16 to Bit 31
      //        0xFFFF0000, GNB_REG_ACC_FLAG_S3SAVE);
      // }
      // // TXCLK RXP CLK EN Port Gating:
      // // Enable: LC_CPM_CONTROL_1[31:16]  (0xFFFF)  LC_CPM_CONTROL_1[15] = 1;
      // if (PcdGetBool (PcdGfxTxClkRxpClkEnPortGating) == TRUE) {
      //   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
      //      NBIO_SPACE(GnbHandle, SMN_PCIE0_LC_CPM_CONTROL_1_ADDRESS),
      //      (UINT32) ~(0xFFFF0000 | BIT15),  //Bit0 to Bit15
      //      0xFFFF8000, GNB_REG_ACC_FLAG_S3SAVE);
      // }
    }
    if (Wrapper->WrapId == 1) {
      // Core 1 (GPP controller)
      //
      // PCIe SDP (CI_SLV_REQR_FULL_DISCONNECT_EN, CI_SLV_REQR_PART_DISCONNECT_EN, CI_MSTSDP_CLKGATE_ONESIDED_ENABLE, CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL)
      //
      if (PcdGetBool (PcdGppPcieSdpControl) == TRUE) {
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             SMN_PCIE1_PCIE_SDP_CTRL_ADDRESS,
             (UINT32) ~(PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_MASK |
             PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_MASK),
             (UINT32)(0 << PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_OFFSET |
             1 << PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_OFFSET),
             GNB_REG_ACC_FLAG_S3SAVE);
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             SMN_PCIE1_PCIE_MST_CTRL_2_ADDRESS,
             (UINT32) ~(PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_MASK |
             PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_MASK),
             (UINT32)(1 << PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_OFFSET |
             1 << PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_OFFSET),
             GNB_REG_ACC_FLAG_S3SAVE);
     } else {
       SmnRegisterRMW (GnbHandle->Address.Address.Bus,
            SMN_PCIE1_PCIE_SDP_CTRL_ADDRESS,
            (UINT32) ~(PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_MASK |
            PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_MASK),
            0,
            GNB_REG_ACC_FLAG_S3SAVE);
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             SMN_PCIE1_PCIE_MST_CTRL_2_ADDRESS,
             (UINT32) ~(PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_MASK |
             PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_MASK),
             0,
             GNB_REG_ACC_FLAG_S3SAVE);
      }
      //
      // TXCLK DYN PORT Gating:
      //
      // Enable: TXCLK_DYN_PORT_X_GATE_ENABLE LC_CPM_CONTROL_0[15:0] = 0xFFFF  Disable: (0x0000)
      // if (PcdGetBool (PcdGppTxClkDynPortGating) == TRUE) {
      //   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
      //        NBIO_SPACE(GnbHandle, SMN_PCIE1_LC_CPM_CONTROL_0_ADDRESS),
      //        (UINT32) ~(0xFFFF),  //Bit0 to Bit15
      //        0xFFFF, GNB_REG_ACC_FLAG_S3SAVE);
      // }
      // // TXCLK DYN TR PORT Gating:
      // // TXCLK_DYN_TR_PORT_X_GATE_ENABLE, LC_CPM_CONTROL_0[31:16] = 0xFFFF
      // if (PcdGetBool (PcdGppTxClkDynTrPortGating) == TRUE) {
      //   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
      //        NBIO_SPACE(GnbHandle, SMN_PCIE1_LC_CPM_CONTROL_0_ADDRESS),
      //        (UINT32) ~(0xFFFF0000),  //Bit16 to Bit 31
      //        0xFFFF0000, GNB_REG_ACC_FLAG_S3SAVE);
      // }
      // // TXCLK RXP CLK EN Port Gating:
      // // Enable: LC_CPM_CONTROL_1[31:16]  (0xFFFF)  LC_CPM_CONTROL_1[15] = 1;
      // if (PcdGetBool (PcdGppTxClkRxpClkEnPortGating) == TRUE) {
      //   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
      //        NBIO_SPACE(GnbHandle, SMN_PCIE1_LC_CPM_CONTROL_1_ADDRESS),
      //        (UINT32) ~(0xFFFF8000),  //Bit15 to Bit31
      //        0xFFFF8000, GNB_REG_ACC_FLAG_S3SAVE);
      // }
    }
    Wrapper = PcieLibGetNextDescriptor (Wrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/*----------------------------------------------------------------------------------------*/
/**
 * T_POWER_ON FCH copy
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */
VOID
PcieTPowerOnFchCopy (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT16                 PcieCapPtr;
  UINT32                 Value;
  UINT32                 SmnAddr;
  BOOLEAN                FchCopy;
  PCIe_ENGINE_CONFIG    *PcieEngine;
  PCIe_WRAPPER_CONFIG   *PcieWrapper;
  PCIE_LC_L1_PM_SUBSTATE_STRUCT LcL1PmSubstate;
  PCIE_LC_L1_PM_SUBSTATE3_STRUCT LcL1PmSubstate3;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a:\n", __FUNCTION__);
  FchCopy = FALSE;

  if (PcdGet8 (PcdCfgPCIeClkReqMode) == 1) {
    (*(volatile UINT32*) (UINTN) (0xFED80E44)) |= BIT0;
    return;
  }

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine) &&
          PcieConfigCheckPortStatus (PcieEngine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
        if ((PcieEngine->Type.Port.ClkReq != 0) && (PcieEngine->Type.Port.LinkAspmL1_2 != 0)) {
          // If the port is connected to a CLKREQ pad controlled by an instance of AMD-FCH and
          // L1.2 is supported by this PCIe port
          PcieCapPtr = GnbLibFindPcieExtendedCapability (PcieEngine->Type.Port.Address.AddressValue, PCIE_L1_PM_SUB_CAP_ID, NULL);

          if (PcieCapPtr != 0) {
            GnbLibPciRead (
              PcieEngine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_L1_PM_SUB_CAP),
              AccessWidth32,
              &Value,
              NULL
              );
            if ((Value & BIT4) &&
                ((Value & BIT0) || (Value & BIT2))) {
              // L1.2 supported

              // Get address of shadow register
              LcL1PmSubstate3.Value = 0;
              // Here, we use MMIO address of FCH::MISC::clkcntrlf0/clkcntrlf4
              // if we should use SMN address, please use below code
              LcL1PmSubstate3.Field.T_POWER_ON_FCH_TARGET_ADDRESS_LO = ((PcieEngine->Type.Port.ClkReq - 1) > 3) ? 0x2D01EF4 : 0x2D01EF0;
              LcL1PmSubstate3.Field.T_POWER_ON_FCH_TARGET_ADDRESS_LO |= ((PcieEngine->Type.Port.ClkReq -1 ) % 4);

              // Write the full address into PCIE_LC_L1_PM_SUBSTATE3.T_POWER_ON_FCH_TARGET_ADDRESS_LO
              SmnAddr = PORT_SPACE (GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId, SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS);
              SmnRegisterWrite (GnbHandle->Address.Address.Bus, SmnAddr, &LcL1PmSubstate3.Value, GNB_REG_ACC_FLAG_S3SAVE);

              // Trigger the write to shadow copy
              SmnAddr = PORT_SPACE (GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId, SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS);
              SmnRegisterRead (GnbHandle->Address.Address.Bus, SmnAddr, &LcL1PmSubstate.Value);
              LcL1PmSubstate.Field.T_POWER_ON_FCH_COPY_EN = 1;
              LcL1PmSubstate.Field.T_POWER_ON_FCH_COPY_TRIGGER = 1;
              SmnRegisterWrite (GnbHandle->Address.Address.Bus, SmnAddr, &LcL1PmSubstate.Value, GNB_REG_ACC_FLAG_S3SAVE);

              FchCopy = TRUE;
            }
          }
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  // FCH::MISC::miscclkcntl1[0] = 0x1
  // FCH_MISCx40_MiscClkCntl1
  if (FchCopy) {
    (*(volatile UINT32*) (UINTN) (0xFED80E44)) &= ~BIT0;
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * Dynamic Phy Power gating
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */

VOID
PcieDynamicPhyPowerGating (
  IN       GNB_HANDLE  *GnbHandle
  )
{

  PCIe_WRAPPER_CONFIG            *Wrapper;
  UINT16                         Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  Wrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (Wrapper != NULL) {
    if (Wrapper->WrapId == 0) {
      //
      // PCIe Dynamic PG
      //
      // Core 0 (GFX controller)
      if (PcdGetBool (PcdGfxPcieDynPortGating) == TRUE) {
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE0_PCIE_PGMST_CNTL_ADDRESS),
             (UINT32) ~(PCIE_PGMST_CNTL_CFG_PG_EN_MASK |
             PCIE_PGMST_CNTL_CFG_PG_HYSTERESIS_MASK |
             PCIE_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_MASK |
             PCIE_PGMST_CNTL_PG_EXIT_TIMER_MASK),
             (UINT32)((1 << PCIE_PGMST_CNTL_CFG_PG_EN_OFFSET) |
             (0x0 << PCIE_PGMST_CNTL_CFG_PG_HYSTERESIS_OFFSET) |
             (0x0 << PCIE_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_OFFSET) |
             (0x0F << PCIE_PGMST_CNTL_PG_EXIT_TIMER_OFFSET)),
             GNB_REG_ACC_FLAG_S3SAVE);

        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE0_PCIE_PGSLV_CNTL_ADDRESS),
             (UINT32) ~(PCIE_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_MASK),
             (0x4 << PCIE_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_OFFSET),
             GNB_REG_ACC_FLAG_S3SAVE);
        //CPM_CONTROL
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE0_CPM_CONTROL_ADDRESS),
             (UINT32) ~(CPM_CONTROL_L1_PWR_GATE_ENABLE_MASK |
             CPM_CONTROL_L1_1_PWR_GATE_ENABLE_MASK |
             CPM_CONTROL_L1_2_PWR_GATE_ENABLE_MASK |
             CPM_CONTROL_PG_EARLY_WAKE_ENABLE_MASK |
             CPM_CONTROL_REGS_IDLE_TO_PG_LATENCY_MASK),
             (UINT32)((1 << CPM_CONTROL_L1_PWR_GATE_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_L1_1_PWR_GATE_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_L1_2_PWR_GATE_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_PG_EARLY_WAKE_ENABLE_OFFSET) |
             (0x2 << CPM_CONTROL_REGS_IDLE_TO_PG_LATENCY_OFFSET)),
             GNB_REG_ACC_FLAG_S3SAVE);

        //CPM_CONTROL_Ext
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE0_CPM_CONTROL_EXT_ADDRESS),
             (UINT32) ~(CPM_CONTROL_EXT_DELAY_HOLD_TRAINING_ENABLE_MASK |
             CPM_CONTROL_EXT_LCLK_DS_MODE_MASK |
             CPM_CONTROL_EXT_LCLK_DS_ENABLE_MASK |
             CPM_CONTROL_EXT_HOTPLUG_ALLOW_LCLK_GATING_EN_MASK),
             (UINT32)((1 << CPM_CONTROL_EXT_DELAY_HOLD_TRAINING_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_EXT_LCLK_DS_MODE_OFFSET) |
             (1 << CPM_CONTROL_EXT_LCLK_DS_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_EXT_HOTPLUG_ALLOW_LCLK_GATING_EN_OFFSET)),
             GNB_REG_ACC_FLAG_S3SAVE);

        for (Index = 0; Index < 3; Index++) {
          // PCIE0FUNC0_PCIE_LC_CNTL12
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        SMN_FUNC0_PCIE0_PCIE_LC_CNTL9_ADDRESS + (0x1000 * Index),
                        (UINT32) ~(PCIE_LC_CNTL9_LC_REFCLKREQ_IN_HOLD_TRAINING_MASK),
                        (1 << PCIE_LC_CNTL9_LC_REFCLKREQ_IN_HOLD_TRAINING_OFFSET),
                        GNB_REG_ACC_FLAG_S3SAVE
                        );
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS + (0x1000 * Index),
                        (UINT32) ~(PCIE_LC_L1_PM_SUBSTATE_LC_FORCE_L1_PG_EXIT_ON_REG_WRITE_MASK),
                        (1 << PCIE_LC_L1_PM_SUBSTATE_LC_FORCE_L1_PG_EXIT_ON_REG_WRITE_OFFSET),
                        GNB_REG_ACC_FLAG_S3SAVE
                        );
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        SMN_FUNC0_PCIE0_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS + (0x1000 * Index),
                        (UINT32) ~(0xFFFFFFFF),
                        0,
                        GNB_REG_ACC_FLAG_S3SAVE
                        );
        }
      }
    }

    if (Wrapper->WrapId == 1) {
      // Core 1: (GPP Controller)
      if (PcdGetBool (PcdGppPcieDynPortGating) == TRUE) {
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE1_PCIE_PGMST_CNTL_ADDRESS),
             (UINT32) ~(PCIE_PGMST_CNTL_CFG_PG_EN_MASK |
             PCIE_PGMST_CNTL_CFG_PG_HYSTERESIS_MASK |
             PCIE_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_MASK |
             PCIE_PGMST_CNTL_PG_EXIT_TIMER_MASK),
             (UINT32)((1 << PCIE_PGMST_CNTL_CFG_PG_EN_OFFSET) |
             (0 << PCIE_PGMST_CNTL_CFG_PG_HYSTERESIS_OFFSET) |
             (0 << PCIE_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_OFFSET) |
             (0x0F << PCIE_PGMST_CNTL_PG_EXIT_TIMER_OFFSET)),
             GNB_REG_ACC_FLAG_S3SAVE);

        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE1_PCIE_PGSLV_CNTL_ADDRESS),
             (UINT32) ~(PCIE_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_MASK),
             (0x4 << PCIE_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_OFFSET),
             GNB_REG_ACC_FLAG_S3SAVE);
        //CPM_CONTROL
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE1_CPM_CONTROL_ADDRESS),
             (UINT32) ~(CPM_CONTROL_L1_PWR_GATE_ENABLE_MASK |
             CPM_CONTROL_L1_1_PWR_GATE_ENABLE_MASK |
             CPM_CONTROL_L1_2_PWR_GATE_ENABLE_MASK |
             CPM_CONTROL_PG_EARLY_WAKE_ENABLE_MASK |
             CPM_CONTROL_REGS_IDLE_TO_PG_LATENCY_MASK),
             (UINT32)((1 << CPM_CONTROL_L1_PWR_GATE_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_L1_1_PWR_GATE_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_L1_2_PWR_GATE_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_PG_EARLY_WAKE_ENABLE_OFFSET) |
             (0x2 << CPM_CONTROL_REGS_IDLE_TO_PG_LATENCY_OFFSET)),
             GNB_REG_ACC_FLAG_S3SAVE);

        //CPM_CONTROL_Ext
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
             NBIO_SPACE(GnbHandle, SMN_PCIE1_CPM_CONTROL_EXT_ADDRESS),
             (UINT32) ~(CPM_CONTROL_EXT_DELAY_HOLD_TRAINING_ENABLE_MASK |
             CPM_CONTROL_EXT_LCLK_DS_MODE_MASK |
             CPM_CONTROL_EXT_LCLK_DS_ENABLE_MASK |
             CPM_CONTROL_EXT_HOTPLUG_ALLOW_LCLK_GATING_EN_MASK),
             (UINT32)((1 << CPM_CONTROL_EXT_DELAY_HOLD_TRAINING_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_EXT_LCLK_DS_MODE_OFFSET) |
             (1 << CPM_CONTROL_EXT_LCLK_DS_ENABLE_OFFSET) |
             (1 << CPM_CONTROL_EXT_HOTPLUG_ALLOW_LCLK_GATING_EN_OFFSET)),
             GNB_REG_ACC_FLAG_S3SAVE);

        for (Index = 0; Index < 6; Index++) {
          // PCIE0FUNC0_PCIE_LC_CNTL12
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        SMN_FUNC0_PCIE1_PCIE_LC_CNTL9_ADDRESS + (0x1000 * Index),
                        (UINT32) ~(PCIE_LC_CNTL9_LC_REFCLKREQ_IN_HOLD_TRAINING_MASK),
                        (1 << PCIE_LC_CNTL9_LC_REFCLKREQ_IN_HOLD_TRAINING_OFFSET),
                        GNB_REG_ACC_FLAG_S3SAVE
                        );
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        SMN_FUNC0_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS + (0x1000 * Index),
                        (UINT32) ~(PCIE_LC_L1_PM_SUBSTATE_LC_FORCE_L1_PG_EXIT_ON_REG_WRITE_MASK),
                        (1 << PCIE_LC_L1_PM_SUBSTATE_LC_FORCE_L1_PG_EXIT_ON_REG_WRITE_OFFSET),
                        GNB_REG_ACC_FLAG_S3SAVE
                        );
          SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        SMN_FUNC0_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS + (0x1000 * Index),
                        (UINT32) ~(0xFFFFFFFF),
                        0,
                        GNB_REG_ACC_FLAG_S3SAVE
                        );
        }
      }
    }
    Wrapper = PcieLibGetNextDescriptor (Wrapper);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}
/*----------------------------------------------------------------------------------------*/
/**
 * Nbio PCIE B0 Setting
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */

VOID
NbioPcieBoSetting (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  // USB4 Mask BAD DLLP status.
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
       NBIO_SPACE(GnbHandle, SMN_FUNC0_PCIE2_PCIE_CORR_ERR_MASK_ADDRESS),
       (UINT32) ~(PCIE_CORR_ERR_MASK_BAD_DLLP_MASK_MASK),
       (0x1 << PCIE_CORR_ERR_MASK_BAD_DLLP_MASK_OFFSET),
       GNB_REG_ACC_FLAG_S3SAVE);

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
       NBIO_SPACE(GnbHandle, SMN_FUNC0_PCIE3_PCIE_CORR_ERR_MASK_ADDRESS),
       (UINT32) ~(PCIE_CORR_ERR_MASK_BAD_DLLP_MASK_MASK),
       (0x1 << PCIE_CORR_ERR_MASK_BAD_DLLP_MASK_OFFSET),
       GNB_REG_ACC_FLAG_S3SAVE);
/*
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
       NBIO_SPACE(GnbHandle, SMN_PCIE2_PCIE_PGMST_CNTL_ADDRESS),
       (UINT32) ~(PCIE_PGMST_CNTL_CFG_PG_EN_MASK),
       (0x1 << PCIE_PGMST_CNTL_CFG_PG_EN_OFFSET),
       GNB_REG_ACC_FLAG_S3SAVE);

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
       NBIO_SPACE(GnbHandle, SMN_PCIE3_PCIE_PGMST_CNTL_ADDRESS),
       (UINT32) ~(PCIE_PGMST_CNTL_CFG_PG_EN_MASK),
       (0x1 << PCIE_PGMST_CNTL_CFG_PG_EN_OFFSET),
       GNB_REG_ACC_FLAG_S3SAVE);
*/

  // Program after SLOT_IMPLEMENTED = 1 (USB4 default hotplug)
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_FUNC0_PCIE2_PCIEP_HPGI_ADDRESS,
    (UINT32)~(PCIEP_HPGI_REG_HPGI_HOOK_MASK),
    1 << PCIEP_HPGI_REG_HPGI_HOOK_OFFSET,
    GNB_REG_ACC_FLAG_S3SAVE
    );

  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_FUNC0_PCIE3_PCIEP_HPGI_ADDRESS,
    (UINT32)~(PCIEP_HPGI_REG_HPGI_HOOK_MASK),
    1 << PCIEP_HPGI_REG_HPGI_HOOK_OFFSET,
    GNB_REG_ACC_FLAG_S3SAVE
    );

  // Lock PCICFG HW init value.
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_PCIE2_PCIE_CNTL_ADDRESS,
    (UINT32) ~(PCIE_CNTL_HWINIT_WR_LOCK_MASK),
    (0x1 << PCIE_CNTL_HWINIT_WR_LOCK_OFFSET),
    GNB_REG_ACC_FLAG_S3SAVE
    );

  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_PCIE3_PCIE_CNTL_ADDRESS,
    (UINT32) ~(PCIE_CNTL_HWINIT_WR_LOCK_MASK),
    (0x1 << PCIE_CNTL_HWINIT_WR_LOCK_OFFSET),
    GNB_REG_ACC_FLAG_S3SAVE
    );

}

/*----------------------------------------------------------------------------------------*/
/**
 *  Controls filtering of CLKREQb signal in LC in order to avoid false L1 substate entries/exits.
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */
VOID
PcieL12ClkReqFilterEn (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT16                         PcieCapPtr;
  UINT32                         Value;
  UINT32                         SmnAddr;
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;
  PCIE_LC_L1_PM_SUBSTATE_STRUCT  LcL1PmSubstate;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a:\n", __FUNCTION__);

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine) &&
          PcieConfigCheckPortStatus (PcieEngine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
        if ((PcieEngine->Type.Port.ClkReq != 0) && (PcieEngine->Type.Port.LinkAspmL1_2 != 0)) {
          // If the port is connected to a CLKREQ pad controlled by an instance of AMD-FCH and
          // L1.2 is supported by this PCIe port
          PcieCapPtr = GnbLibFindPcieExtendedCapability (PcieEngine->Type.Port.Address.AddressValue, PCIE_L1_PM_SUB_CAP_ID, NULL);
          if (PcieCapPtr != 0) {
            GnbLibPciRead (
              PcieEngine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_L1_PM_SUB_CAP),
              AccessWidth32,
              &Value,
              NULL
              );
            if ((Value & PCIE_L1_PM_SUB_CAP_L1_PM_SUB_SUPPORTED_MASK) &&
                ((Value & PCIE_L1_PM_SUB_CAP_PCI_PM_L1_2_SUPPORTED_MASK) || (Value & PCIE_L1_PM_SUB_CAP_ASPM_L1_2_SUPPORTED_MASK))) {
              // L1.2 supported
              SmnAddr = PORT_SPACE (GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId, SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS);
              SmnRegisterRead (GnbHandle->Address.Address.Bus, SmnAddr, &LcL1PmSubstate.Value);
              LcL1PmSubstate.Field.LC_CLKREQ_FILTER_EN = PcieEngine->Type.Port.ClkReqFilterEn;
              SmnRegisterWrite (GnbHandle->Address.Address.Bus, SmnAddr, &LcL1PmSubstate.Value, GNB_REG_ACC_FLAG_S3SAVE);
            }
          }
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Lock registers
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */

VOID
PcieLockRegisters (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_WRAPPER_CONFIG            *Wrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  Wrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (Wrapper != NULL) {
    if (Wrapper->WrapId == 0) {
      SmnRegisterRMW(GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_PCIE0_PCIE_CNTL_ADDRESS), (UINT32)~(BIT0), BIT0, GNB_REG_ACC_FLAG_S3SAVE);
    }
    if (Wrapper->WrapId == 1) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_PCIE1_PCIE_CNTL_ADDRESS), (UINT32) ~(BIT0), BIT0, GNB_REG_ACC_FLAG_S3SAVE);
    }
    Wrapper = PcieLibGetNextDescriptor (Wrapper);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Clear RAS registers
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */

VOID
PcieClearRASRegisters (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT32                          Value;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  IDS_HDT_CONSOLE (GNB_TRACE, "  --RBIndex=%x\n", GnbHandle->RBIndex);
  SmnRegisterRead(GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle,
    SMN_SYNCFLOOD_STATUS_ADDRESS), &Value);
  SmnRegisterWrite(GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle,
    SMN_SYNCFLOOD_STATUS_ADDRESS), &Value, GNB_REG_ACC_FLAG_S3SAVE);
  SmnRegisterRead(GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle,
    SMN_APML_STATUS_ADDRESS), &Value);
  SmnRegisterWrite(GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle,
    SMN_APML_STATUS_ADDRESS), &Value, GNB_REG_ACC_FLAG_S3SAVE);
  SmnRegisterRead(GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle,
    SMN_APML_CONTROL_ADDRESS), &Value);
    Value &= ~(APML_CONTROL_APML_OutputDis_MASK);
  SmnRegisterWrite(GnbHandle->Address.Address.Bus, NBIO_SPACE (GnbHandle,
    SMN_APML_CONTROL_ADDRESS), &Value, GNB_REG_ACC_FLAG_S3SAVE);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdPcieMiscInit
 *
 *  Description:
 *     Performs further PCIe initialization, mostly stuff that could not be done in PEI
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
EFIAPI
AmdPcieMiscInit (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                          Status;
  PCIe_PLATFORM_CONFIG                *Pcie;
  DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL *PcieServicesProtocol;
  GNB_PCIE_INFORMATION_DATA_HOB       *PciePlatformConfigHobData;
  GNB_HANDLE                          *GnbHandle;
  AMD_SOC_LOGICAL_ID_PROTOCOL         *SocLogicalId;
  SOC_LOGICAL_ID                      LogicalId;
  BOOLEAN                             IsB0 = FALSE;

  AGESA_TESTPOINT (TpAmdPcieMiscInitEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  Status = gBS->LocateProtocol (&gAmdSocLogicalIdProtocolGuid, NULL, (VOID **) &SocLogicalId);
  SocLogicalId->GetLogicalIdOnCurrentCore (SocLogicalId, &LogicalId);

  if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
    IsB0 = TRUE;
  }

  Status = gBS->LocateProtocol (
                  &gAmdNbioPcieServicesProtocolGuid,
                  NULL,
                  &PcieServicesProtocol
                  );

  if (Status == EFI_SUCCESS) {
    PcieServicesProtocol->PcieGetTopology (PcieServicesProtocol, (UINT32 **) &PciePlatformConfigHobData);
    Pcie = &(PciePlatformConfigHobData->PciePlatformConfigHob);
    GnbHandle = NbioGetHandle (Pcie);
    while (GnbHandle != NULL) {
      AmdPsppConfig (Pcie);
      // Slot power limit
      PcieSlotPowerLimitInterface (GnbHandle);
      // Enabling PCIe Core Power Saving Features
      PciePwrClockGating (GnbHandle);
      // Dynamic phy power gating
      PcieDynamicPhyPowerGating (GnbHandle);
      // configure MaxReadRequestSize on PCIE interface
      PcieMaxReadRequestInterface (GnbHandle);
      // configure MaxPayloadSize on PCIE interface
      PcieMaxPayloadInterface (GnbHandle);
      // configure CI_MAX_CPL_PAYLOAD_SIZE on PCIE core
      //*NBIO_TODO : Need to port this from ZPTrunk to master in PcieMaxCplPayloadLib
      //*NBIO_TODO PcieMaxCplPayloadInterface (GnbHandle);
      // enable Clock Power Managment
      PcieClkPmInterface (GnbHandle);
      if (TRUE == PcdGetBool (PcdCfgPcieTbtSupport)) {
        PcieTbtInitV2 (GnbHandle);
      }
      PcieSrisInit (GnbHandle);
      if(PcdGetBool (PcdCfgPCIeLTREnable) == TRUE) {
        PcieLTRInterfaceV2 (GnbHandle);
        PcieTPowerOnFchCopy (GnbHandle);
      }
      PcieClkReqInterface (GnbHandle);
      PcieAspmBlackListInterface (GnbHandle);
      PcieAspmL1SSInterface (GnbHandle);
      PcieOBFFInterface (GnbHandle);
      PcieAspmInterface (GnbHandle);
     // Set L1 Exit Latency for PT
      PcieL1ExitLatencyInterface (GnbHandle);
      // program coomon clock configuration
      PcieCommClkCfgInterface (GnbHandle);
      // Enable SmartShift
      AmdAPlusAConfig (Pcie);
      // Clear RAS status registers
      PcieClearRASRegisters (GnbHandle);
      NbioPcieAerInit (GnbHandle);
      NbioRASControl (GnbHandle);
      if (IsB0 == TRUE) {
        NbioPcieBoSetting (GnbHandle);
      }
      PcieL12ClkReqFilterEn (GnbHandle);
      PcieLockRegisters (GnbHandle);

      if (PcdGetBool(PcdCfgIommuSupport)) {
        // If IOMMU is enabled set BUS_MASTER bit
        SmnRegisterRMW (GnbHandle->Address.Address.Bus, SMN_IOMMU_COMMAND_ADDRESS, (UINT32) ~(IOMMU_COMMAND_BUS_MASTER_EN_MASK), (1 << IOMMU_COMMAND_BUS_MASTER_EN_OFFSET), GNB_REG_ACC_FLAG_S3SAVE);
      }
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }

  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpAmdPcieMiscInitExit, NULL);

  return;
}

EFI_STATUS
EFIAPI
AmdNbioPcieDxeEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_HANDLE          Handle;
  EFI_EVENT           PciIoEvent;
  VOID                *Registration;

  AGESA_TESTPOINT (TpNbioPCIeDxeEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdNbioPcieServicesProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mDxeAmdNbioPcieServicesProtocol
                  );

  Handle = NULL;
  //
  // produce AMD_NBIO_PCIE_AER_PROTOCOL
  //
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdNbioPcieAerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAmdNbioPcieAerProtocol
                  );
  //
  // Register the event handling function for AmdPcieMiscInit
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_CALLBACK,
             AmdPcieMiscInit,
             NULL,
             NULL,
             &PciIoEvent
             );

  Status = gBS->RegisterProtocolNotify (
              &gEfiPciEnumerationCompleteProtocolGuid,
              PciIoEvent,
              &Registration
              );

  IDS_HDT_CONSOLE (MAIN_FLOW, "% Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpNbioPCIeDxeExit, NULL);

  return EFI_SUCCESS;
}


