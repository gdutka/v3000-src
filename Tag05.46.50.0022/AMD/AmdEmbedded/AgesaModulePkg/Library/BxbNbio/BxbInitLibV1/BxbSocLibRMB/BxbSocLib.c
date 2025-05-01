/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <AMD.h>
#include <GnbDxio.h>
#include <GnbRegistersRMB.h>
#include <Library/PcieConfigLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/NbioHandleLib.h>

#define FILECODE LIBRARY_BXBNBIO_BXBINITLIBV1_BXBSOCLIBRMB_BXBSOCLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS           0x13b31004UL
#define SMN_PCIE1DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS           0x13b33004UL

#define SMN_PCIE0_FUNC0_PCIE_LC_STATE0_ADDRESS                 0x11140294UL
#define SMN_PCIE1_FUNC0_PCIE_LC_STATE0_ADDRESS                 0x11240294UL


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * SOC Specific function to read LC_STATE_0 of an engine
 *
 *
 *
 * @param[in]  GnbHandle    Pointer to GNB_HANDLE for this NBIO instance
 * @param[in]  Engine       Pointer to ENGINE_CONFIG
 * @param[in]  Value        Pointer to UINT32 value to store LC_STATE_0
 */

VOID
SocReadLcState0 (
  IN       GNB_HANDLE             *GnbHandle,
  IN       PCIe_ENGINE_CONFIG     *Engine,
  IN       UINT32                 *Value
  )
{
  PCIe_WRAPPER_CONFIG     *Wrapper;
  UINT32                  RegAddress;

  Wrapper = PcieConfigGetParentWrapper(Engine);

  IDS_HDT_CONSOLE (MAIN_FLOW, " WrapId: %x \n", Wrapper->WrapId);
  IDS_HDT_CONSOLE (MAIN_FLOW, " Engine->Type.Port.PortId: %x \n", Engine->Type.Port.PortId);

  if (Wrapper->WrapId == 0) {
    RegAddress = SMN_PCIE0_FUNC0_PCIE_LC_STATE0_ADDRESS + ((Engine->Type.Port.PortId % 8) << 12);
    IDS_HDT_CONSOLE (MAIN_FLOW, "0");
  } else  {
    RegAddress = SMN_PCIE1_FUNC0_PCIE_LC_STATE0_ADDRESS + ((Engine->Type.Port.PortId % 8) << 12);
    IDS_HDT_CONSOLE (MAIN_FLOW, "1");
  }

//  RegAddress = SMN_PCIE1_FUNC0_PCIE_LC_STATE0_ADDRESS + ((Engine->Type.Port.PortId % 8) << 12);
  IDS_HDT_CONSOLE (MAIN_FLOW, " RegAddress: %x \n", RegAddress);
  SmnRegisterRead(GnbHandle->Address.Address.Bus, RegAddress, Value);

  return;
}


/*----------------------------------------------------------------------------------------*/
/**
 * SOC Specific function to enable port visibility
 *
 *
 *
 * @param[in]  GnbHandle    Pointer to GNB_HANDLE for this NBIO instance
 * @param[in]  Engine       Pointer to ENGINE_CONFIG
 */

VOID
SocEnableEngineVisibility (
  IN       GNB_HANDLE             *GnbHandle,
  IN       PCIe_ENGINE_CONFIG     *Engine
  )
{
  UINT8         LogicalBridgeID;
  BOOLEAN       IsController1 = FALSE;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry \n", __FUNCTION__);
  IDS_HDT_CONSOLE (MAIN_FLOW, "LogicalBridgeId : %x \n", Engine->Type.Port.LogicalBridgeId);
  LogicalBridgeID = Engine->Type.Port.LogicalBridgeId;
  if (Engine->Type.Port.LogicalBridgeId >= 3) {
    LogicalBridgeID = LogicalBridgeID - 3;
    IsController1 = TRUE;
  }
  if (IsController1){
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_PCIE1DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS + (LogicalBridgeID  << 10),
                    (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK | IOHC_Bridge_CNTL_CfgDis_MASK |
                     IOHC_Bridge_CNTL_CrsEnable_MASK),
                    (1 << IOHC_Bridge_CNTL_CrsEnable_OFFSET),
                    0   // Change to flags if used in DXE
                    );
    IDS_HDT_CONSOLE (MAIN_FLOW, "SMN IOHC_Bridge_CNTL: %x \n", SMN_PCIE1DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS
                         + (LogicalBridgeID  << 10));
  } else {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS + (LogicalBridgeID  << 10),
                    (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK | IOHC_Bridge_CNTL_CfgDis_MASK
                    | IOHC_Bridge_CNTL_CrsEnable_MASK),
                    (1 << IOHC_Bridge_CNTL_CrsEnable_OFFSET),
                    0   // Change to flags if used in DXE
                    );
    IDS_HDT_CONSOLE (MAIN_FLOW, "SMN IOHC_Bridge_CNTL: %x \n", SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS +
                     (LogicalBridgeID  << 10));
  }

  return;
}

