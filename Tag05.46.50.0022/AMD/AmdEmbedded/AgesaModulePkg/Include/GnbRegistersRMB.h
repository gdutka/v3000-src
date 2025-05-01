/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
#ifndef _GNBREGISTERSRMB_H_
#define _GNBREGISTERSRMB_H_

#include "GnbRegistersRMB/ACP_ACPAON.h"
#include "GnbRegistersRMB/ACP_ACPCLKRST.h"
#include "GnbRegistersRMB/ACP_AZALIAIP.h"
#include "GnbRegistersRMB/CLK_CLK1.h"
#include "GnbRegistersRMB/DCIO_DCIO.h"
#include "GnbRegistersRMB/IOAPIC.h"
#include "GnbRegistersRMB/IOHC.h"
#include "GnbRegistersRMB/IOAGR.h"
#include "GnbRegistersRMB/IOMMUL1.h"
#include "GnbRegistersRMB/IOMMUL2.h"
#include "GnbRegistersRMB/IOMMUMMIO.h"
#include "GnbRegistersRMB/MMHUB_PCTL.h"
#include "GnbRegistersRMB/MP_MP0CRU.h"
#include "GnbRegistersRMB/MP_MP1CRU.h"
#include "GnbRegistersRMB/NBIFEPF0CFG.h"
#include "GnbRegistersRMB/NBIFEPFCFG.h"
#include "GnbRegistersRMB/NBIFEPFNCFG.h"
#include "GnbRegistersRMB/NBIFRCCFG.h"
#include "GnbRegistersRMB/NBIFMM.h"
#include "GnbRegistersRMB/PCIECORE.h"
#include "GnbRegistersRMB/PCIEPORT.h"
#include "GnbRegistersRMB/PCIERCCFG.h"
#include "GnbRegistersRMB/SMU_SMUIO.h"
#include <GnbRegistersRMB/SMU_FUSE.h>
#include "GnbRegistersRMB/SST.h"
#include "GnbRegistersRMB/SYSHUBMM.h"
#include "GnbRegistersRMB/UVD_UVDPG.h"
#include "GnbRegistersRMB/RIOMMU.h"
#include "GnbRegistersRMB/HDA_AZF0ROOT.h"
#include "GnbRegistersRMB/GC_GCVMSHAREDPF.h"
#include "GnbRegistersRMB/MMUTCL2_MMVMSHAREDPF.h"
#include "GnbRegistersRMB/GLOBALREGS.h"


#ifndef NBIO_SPACE
  #define  NBIO_SPACE(HANDLE, ADDRESS)   (ADDRESS)
#endif

#ifndef WRAP_SPACE
  #define  WRAP_SPACE(HANDLE, WRAPPER, ADDRESS)   (ADDRESS + (WRAPPER->WrapId << 20))
#endif

#ifndef PORT_SPACE
  #define  PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (WRAPPER->WrapId << 20) + (PORTINDEX << 12))
#endif


/// CAUTION: This is very specific to Rembrandt IOHC Bridge registers accessed by LogicalBridgeId.
#ifndef IOHC_BRIDGE_SPACE
  #define  IOHC_BRIDGE_SPACE(HANDLE, ENGINE, ADDRESS)   (ADDRESS + ((ENGINE->Type.Port.LogicalBridgeId < 5) ? \
  (ENGINE->Type.Port.LogicalBridgeId << 10) : ((1 << 13) + ((ENGINE->Type.Port.LogicalBridgeId - 5) << 10))))
#endif

#ifndef IOHC_SPACE
  #define  IOHC_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (WRAPPER * 0xA0) + (PORTINDEX * 0x20))
#endif

//
// Need to find new #defines in ppr generated files for these values
//
#define PCIE_CAP_LIST_ADDRESS                                 0x5A

#define SLOT_CAP_ADDRESS                                       0x6C
//
#define SLOT_CAP_ATTN_BUTTON_PRESENT_OFFSET                   0
#define SLOT_CAP_ATTN_BUTTON_PRESENT_WIDTH                    1
#define SLOT_CAP_ATTN_BUTTON_PRESENT_MASK                     0x1
#define SLOT_CAP_PWR_CONTROLLER_PRESENT_OFFSET                1
#define SLOT_CAP_PWR_CONTROLLER_PRESENT_WIDTH                 1
#define SLOT_CAP_PWR_CONTROLLER_PRESENT_MASK                  0x2
#define SLOT_CAP_MRL_SENSOR_PRESENT_OFFSET                    2
#define SLOT_CAP_MRL_SENSOR_PRESENT_WIDTH                     1
#define SLOT_CAP_MRL_SENSOR_PRESENT_MASK                      0x4
#define SLOT_CAP_ATTN_INDICATOR_PRESENT_OFFSET                3
#define SLOT_CAP_ATTN_INDICATOR_PRESENT_WIDTH                 1
#define SLOT_CAP_ATTN_INDICATOR_PRESENT_MASK                  0x8
#define SLOT_CAP_PWR_INDICATOR_PRESENT_OFFSET                 4
#define SLOT_CAP_PWR_INDICATOR_PRESENT_WIDTH                  1
#define SLOT_CAP_PWR_INDICATOR_PRESENT_MASK                   0x10
#define SLOT_CAP_HOTPLUG_SURPRISE_OFFSET                      5
#define SLOT_CAP_HOTPLUG_SURPRISE_WIDTH                       1
#define SLOT_CAP_HOTPLUG_SURPRISE_MASK                        0x20
#define SLOT_CAP_HOTPLUG_CAPABLE_OFFSET                       6
#define SLOT_CAP_HOTPLUG_CAPABLE_WIDTH                        1
#define SLOT_CAP_HOTPLUG_CAPABLE_MASK                         0x40
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET                  7
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_WIDTH                   8
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_MASK                    0x7f80
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET                  15
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_WIDTH                   2
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK                    0x18000
#define SLOT_CAP_ELECTROMECH_INTERLOCK_PRESENT_OFFSET         17
#define SLOT_CAP_ELECTROMECH_INTERLOCK_PRESENT_WIDTH          1
#define SLOT_CAP_ELECTROMECH_INTERLOCK_PRESENT_MASK           0x20000
#define SLOT_CAP_NO_COMMAND_COMPLETED_SUPPORTED_OFFSET        18
#define SLOT_CAP_NO_COMMAND_COMPLETED_SUPPORTED_WIDTH         1
#define SLOT_CAP_NO_COMMAND_COMPLETED_SUPPORTED_MASK          0x40000
#define SLOT_CAP_PHYSICAL_SLOT_NUM_WIDTH                      13
//
// NBIO_TODO: Need to use the pcilib offsets?
// DEVICE_CNTL2
#define DEVICE_CNTL2_ADDRESS                                   0x80
#define DEVICE_CNTL2_CPL_TIMEOUT_VALUE_OFFSET                  0
#define DEVICE_CNTL2_CPL_TIMEOUT_VALUE_WIDTH                   4
#define DEVICE_CNTL2_CPL_TIMEOUT_VALUE_MASK                    0xf

#define SLOT_CNTL_ADDRESS                                      0x70

#define SLOT_CNTL_ATTN_BUTTON_PRESSED_EN_OFFSET                0
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_EN_WIDTH                 1
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_EN_MASK                  0x1
#define SLOT_CNTL_PWR_FAULT_DETECTED_EN_OFFSET                 1
#define SLOT_CNTL_PWR_FAULT_DETECTED_EN_WIDTH                  1
#define SLOT_CNTL_PWR_FAULT_DETECTED_EN_MASK                   0x2
#define SLOT_CNTL_MRL_SENSOR_CHANGED_EN_OFFSET                 2
#define SLOT_CNTL_MRL_SENSOR_CHANGED_EN_WIDTH                  1
#define SLOT_CNTL_MRL_SENSOR_CHANGED_EN_MASK                   0x4
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_EN_OFFSET            3
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_EN_WIDTH             1
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_EN_MASK              0x8
#define SLOT_CNTL_COMMAND_COMPLETED_INTR_EN_OFFSET             4
#define SLOT_CNTL_COMMAND_COMPLETED_INTR_EN_WIDTH              1
#define SLOT_CNTL_COMMAND_COMPLETED_INTR_EN_MASK               0x10
#define SLOT_CNTL_HOTPLUG_INTR_EN_OFFSET                       5
#define SLOT_CNTL_HOTPLUG_INTR_EN_WIDTH                        1
#define SLOT_CNTL_HOTPLUG_INTR_EN_MASK                         0x20
#define SLOT_CNTL_ATTN_INDICATOR_CNTL_OFFSET                   6
#define SLOT_CNTL_ATTN_INDICATOR_CNTL_WIDTH                    2
#define SLOT_CNTL_ATTN_INDICATOR_CNTL_MASK                     0xc0
#define SLOT_CNTL_PWR_INDICATOR_CNTL_OFFSET                    8
#define SLOT_CNTL_PWR_INDICATOR_CNTL_WIDTH                     2
#define SLOT_CNTL_PWR_INDICATOR_CNTL_MASK                      0x300
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_OFFSET                   10
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_WIDTH                    1
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_MASK                     0x400
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_CNTL_OFFSET            11
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_CNTL_WIDTH             1
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_CNTL_MASK              0x800
#define SLOT_CNTL_DL_STATE_CHANGED_EN_OFFSET                   12
#define SLOT_CNTL_DL_STATE_CHANGED_EN_WIDTH                    1
#define SLOT_CNTL_DL_STATE_CHANGED_EN_MASK                     0x1000
//Reserved 3
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_OFFSET                   16
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_WIDTH                    1
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_MASK                     0x10000
#define SLOT_CNTL_PWR_FAULT_DETECTED_OFFSET                    17
#define SLOT_CNTL_PWR_FAULT_DETECTED_WIDTH                     1
#define SLOT_CNTL_PWR_FAULT_DETECTED_MASK                      0x20000
#define SLOT_CNTL_MRL_SENSOR_CHANGED_OFFSET                    18
#define SLOT_CNTL_MRL_SENSOR_CHANGED_WIDTH                     1
#define SLOT_CNTL_MRL_SENSOR_CHANGED_MASK                      0x40000
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_OFFSET               19
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_WIDTH                1
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_MASK                 0x80000
#define SLOT_CNTL_COMMAND_COMPLETED_OFFSET                     20
#define SLOT_CNTL_COMMAND_COMPLETED_WIDTH                      1
#define SLOT_CNTL_COMMAND_COMPLETED_MASK                       0x100000
#define SLOT_CNTL_MRL_SENSOR_STATE_OFFSET                      21
#define SLOT_CNTL_MRL_SENSOR_STATE_WIDTH                       1
#define SLOT_CNTL_MRL_SENSOR_STATE_MASK                        0x200000
#define SLOT_CNTL_PRESENCE_DETECT_STATE_OFFSET                 22
#define SLOT_CNTL_PRESENCE_DETECT_STATE_WIDTH                  1
#define SLOT_CNTL_PRESENCE_DETECT_STATE_MASK                   0x400000
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_STATUS_OFFSET          23
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_STATUS_WIDTH           1
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_STATUS_MASK            0x800000
#define SLOT_CNTL_DL_STATE_CHANGED_OFFSET                      24
#define SLOT_CNTL_DL_STATE_CHANGED_WIDTH                       1
#define SLOT_CNTL_DL_STATE_CHANGED_MASK                        0x1000000

// Bitfield Description : Controls the number of TS2s require sent before changing the Link's state.
#define PCIE_LC_CNTL2_LC_MORE_TS2_EN_OFFSET      8
#define PCIE_LC_CNTL2_LC_MORE_TS2_EN_MASK        0x100

#define PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_OFFSET                          14
#define PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_WIDTH                           2
#define PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_MASK                            0xc000

#define SMN_0x13B31004_BridgeDis_OFFSET                        0
#define SMN_0x13B31004_BridgeDis_WIDTH                         1
#define SMN_0x13B31004_BridgeDis_MASK                          0x1

#define LINK_CTL_STS_DL_ACTIVE_OFFSET                          29

// NBIO_TODO: These seem to conflict with the PPR
#define PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_OFFSET            1
#define PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_WIDTH             1
#define PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_MASK              0x2
#define PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_OFFSET   2
#define PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_WIDTH    1
#define PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_MASK     0x4

#endif /*_GNBREGISTERSRMB_H_*/


