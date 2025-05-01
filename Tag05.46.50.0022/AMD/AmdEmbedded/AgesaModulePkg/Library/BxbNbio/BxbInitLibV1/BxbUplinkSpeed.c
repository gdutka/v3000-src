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
#include <PiPei.h>
#include <AMD.h>
#include <GnbDxio.h>
#include <AmdPcieComplex.h>
#include <CcxRegistersVh.h>
#include <GnbRegistersSSP.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/GnbLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/BxbAccessLib.h>
#include <Library/BxbSmuV11Lib.h>
#include <Library/BxbInitLib.h>
#include <Library/DxioLibV2.h>
#include <Filecode.h>

#define FILECODE LIBRARY_BXBNBIO_BXBINITLIBV1_BXBUPLINKSPEED_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

VOID
SocReadLcState0 (
  IN       GNB_HANDLE             *GnbHandle,
  IN       PCIe_ENGINE_CONFIG     *Engine,
  IN       UINT32                 *Value
  );


VOID
SocEnableEngineVisibility (
  IN       GNB_HANDLE             *GnbHandle,
  IN       PCIe_ENGINE_CONFIG     *Engine
  );

#define bmcLinkInit_params_lanes      0
#define bmcLinkInit_setParam          47
#define bmcLinkInit_go                48

#define RSMU_SW_STRAPRX_ADDR_PCIE0_INDEX    0x90497AC
#define RSMU_SW_STRAPRX_ADDR_PCIE0_DATA     0x90497B0
#define FUSE_ACCESS_LOCATION                0xFFFE0000

#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_INDEX  0x9f  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_INDEX    0xa0  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_INDEX       0xa1  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET
#define STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_INDEX         0xa2  ///< STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET
#define STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET_INDEX                   0xa3  ///< Downstream Port 16.0 GT/s Transmitter Preset
#define STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET_INDEX                   0xa4  ///< Upstream Port 16.0 GT/s Transmitter Preset
#define STRAP_BIF_SWUS_LTR_SUPPORTED_INDEX                                          0xc8  ///< STRAP_BIF_SWUS_LTR_SUPPORTED

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/// SetBmcLinkInit Argument Structure
typedef union {
  struct {                                                              ///<
    UINT32                                                  COMMAND:8 ; ///<
    UINT32                                              RETURN_TYPE:2 ; ///<
    UINT32                                               PARAM_TYPE:6 ; ///<
    UINT32                                                   PARAM1:8 ; ///<
    UINT32                                                   PARAM2:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} SetBmcLinkInit_STRUCT;

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to write pcie soft straps
 *
 *
 *
 * @param[in]  GnbHandle         The associated Gnb Handle
 * @param[in]  StrapIndex        Strap index
 * @param[out] Value             Pointer to byte
 * @param[in]  Wrapper           Pcie wrapper number
 */
STATIC
VOID
WriteBixbyStrap (
  IN   UINT32       BusNumber,
  IN   UINT8        StrapIndex,
  IN   UINT32       Value
  )
{
  UINT32 Index;

  Index = FUSE_ACCESS_LOCATION;
  Index += StrapIndex;

  BxbRegisterWrite (BusNumber, RSMU_SW_STRAPRX_ADDR_PCIE0_INDEX, &Index, 0);
  BxbRegisterWrite (BusNumber, RSMU_SW_STRAPRX_ADDR_PCIE0_DATA, &Value, 0);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Engine Callback for port configuration before bifurcation
 *
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
BxbUplinkConfigBeforeTraining (
  IN      UINT32      BixbyBus
  )
{

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  // Program equalization parameters into their corresponding strap. These are written per wrapper
  // but programmed in the per engine call. Different lane CBS values within a wrapper should be
  // avoided to prevent overwriting the desired value for the whole wrapper.

  // Gen3 US/DS Preset
  WriteBixbyStrap (BixbyBus, STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_INDEX, 7);
  WriteBixbyStrap (BixbyBus, STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_INDEX, 3);
  WriteBixbyStrap (BixbyBus, STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_INDEX, 7);
  WriteBixbyStrap (BixbyBus, STRAP_BIF_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_INDEX, 3);

  // Gen4 US/DS Preset
  WriteBixbyStrap (BixbyBus, STRAP_BIF_LANE_EQUALIZATION_CNTL_DSP_16GT_TX_PRESET_INDEX, 7);
  WriteBixbyStrap (BixbyBus, STRAP_BIF_LANE_EQUALIZATION_CNTL_USP_16GT_TX_PRESET_INDEX, 5);

  // LTR Support
  WriteBixbyStrap (BixbyBus, STRAP_BIF_SWUS_LTR_SUPPORTED_INDEX, 1);

  // These are all programmed per engine
  // Gen3 Bypass Phase3 EQ
  BxbRegisterRMW (
    BixbyBus,
    SMN_NBIO3PCIE0FUNC0_PCIE_LC_CNTL4_ADDRESS,
    (UINT32) ~(PCIE_LC_CNTL4_LC_BYPASS_EQ_REQ_PHASE_8GT_MASK),
    0 << PCIE_LC_CNTL4_LC_BYPASS_EQ_REQ_PHASE_8GT_OFFSET,
    0
    );

  // Gen3 Bypass Phase2/3 EQ
  BxbRegisterRMW (
    BixbyBus,
    SMN_NBIO3PCIE0FUNC0_PCIE_LC_CNTL4_ADDRESS,
    (UINT32) ~(PCIE_LC_CNTL4_LC_USC_EQ_NOT_REQD_8GT_MASK),
    0 << PCIE_LC_CNTL4_LC_USC_EQ_NOT_REQD_8GT_OFFSET,
    0
    );
  // Gen3 EQ Search Mode
  BxbRegisterRMW (
    BixbyBus,
    SMN_NBIO3PCIE0FUNC0_PCIE_LC_CNTL4_ADDRESS,
    (UINT32) ~(PCIE_LC_CNTL4_LC_EQ_SEARCH_MODE_8GT_MASK),
    3 << PCIE_LC_CNTL4_LC_EQ_SEARCH_MODE_8GT_OFFSET,
    0
    );

  // Gen4 Bypass Phase3 EQ
  BxbRegisterRMW (
    BixbyBus,
    SMN_NBIO3PCIE0FUNC0_PCIE_LC_CNTL8_ADDRESS,
    (UINT32) ~(PCIE_LC_CNTL8_LC_BYPASS_EQ_REQ_PHASE_16GT_MASK),
    0 << PCIE_LC_CNTL8_LC_BYPASS_EQ_REQ_PHASE_16GT_OFFSET,
    0
    );
  // Gen4 Bypass Phase2/3 EQ
  BxbRegisterRMW (
    BixbyBus,
    SMN_NBIO3PCIE0FUNC0_PCIE_LC_CNTL8_ADDRESS,
    (UINT32) ~(PCIE_LC_CNTL8_LC_USC_EQ_NOT_REQD_16GT_MASK),
    0 << PCIE_LC_CNTL8_LC_USC_EQ_NOT_REQD_16GT_OFFSET,
    0
    );

  // Gen4 EQ Search Mode
  BxbRegisterRMW (
    BixbyBus,
    SMN_NBIO3PCIE0FUNC0_PCIE_LC_CNTL8_ADDRESS,
    (UINT32) ~(PCIE_LC_CNTL8_LC_EQ_SEARCH_MODE_16GT_MASK),
    3 << PCIE_LC_CNTL8_LC_EQ_SEARCH_MODE_16GT_OFFSET,
    0
    );
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create internal PCIe configuration topology
 *
 *
 *
 * @param[in]  ComplexData       Pointer to Bixby Complex Data
 * @param[in]  ComplexDataSize   Size of Bixby Complex Data
 * @param[in]  Pcie              Pointer PCIe_PLATFORM_CONFIG structure
 * @retval     EFI_STATUS
 */

VOID
BixbySpeedConfig (
  IN       GNB_HANDLE      *GnbHandle,
  IN       PCI_ADDR        EarlyTrainAddress
  )
{
  UINT32                    SmuArg[6];
  UINT32                    RetVal;
  SetBmcLinkInit_STRUCT     SetBmcLinkInit;
  PCI_ADDR                  BixbyAddress;
  UINT32                    BixbyRootBus;

  AGESA_TESTPOINT (TpNbioPCIePeiEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry, EarlyTrainAddress = 0x%x\n", __FUNCTION__, EarlyTrainAddress.AddressValue);

  // Find Bixby location and assign secondary address
  BixbyRootBus = GnbHandle->Address.Address.Bus + 5;
  BixbyAddress.AddressValue = MAKE_SBDFO (0, BixbyRootBus, 0, 0, 0);

  GnbLibPciRMW (
    EarlyTrainAddress.AddressValue | 0x18,
    AccessWidth32,
    0xFF0000FF,
    (BixbyRootBus << 16) + (BixbyRootBus << 8),
    (AMD_CONFIG_PARAMS *) NULL
    );

  BxbUplinkConfigBeforeTraining (BixbyRootBus);

  SetBmcLinkInit.Value = 0;
  SetBmcLinkInit.Field.COMMAND = bmcLinkInit_setParam;
  SetBmcLinkInit.Field.RETURN_TYPE = DXIO_MBOX_RETURN_NONPOSTED;
  SetBmcLinkInit.Field.PARAM_TYPE = bmcLinkInit_params_lanes;
  SetBmcLinkInit.Field.PARAM1 = 8;
  SetBmcLinkInit.Field.PARAM2 = 15;

  LibAmdMemFill (SmuArg, 0x00, 24, NULL);
  SmuArg[0] = SetBmcLinkInit.Value;
  RetVal = BxbSmuServiceRequestV11(
    BixbyAddress,
    BIOSSMC_MSG_DxioTestMessage,
    SmuArg,
    0
    );

  SetBmcLinkInit.Value = 0;
  SetBmcLinkInit.Field.COMMAND = bmcLinkInit_go;
  SetBmcLinkInit.Field.RETURN_TYPE = DXIO_MBOX_RETURN_POSTED_DEFRD;

  LibAmdMemFill (SmuArg, 0x00, 24, NULL);
  SmuArg[0] = SetBmcLinkInit.Value;
  BxbSmuServiceSendRequestV11(
    BixbyAddress,
    BIOSSMC_MSG_DxioTestMessage,
    SmuArg,
    0
    );

  GnbLibPciRMW (
    EarlyTrainAddress.AddressValue | 0x18,
    AccessWidth32, 0xFF0000FF,
    (0 << 16) + (0 << 8),
    (AMD_CONFIG_PARAMS *) NULL
    );

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Clean up programming from early BMC initialization
 *
 *
 *
 * @param[in]     GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]     DieNumber      InstanceId of the Die that contains the BMC link
 */
BOOLEAN
BixbyFindEarlyLink (
  IN       DXIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor,
  IN       GNB_HANDLE                *GnbHandle
  )
{
  PCIE_LC_STATE0_STRUCT   LcState0;
  PCI_ADDR                EarlyTrainPort;
  BOOLEAN                 BixbyPresent;
  BOOLEAN                 SbLinkPresent;
  DXIO_COMPLEX_DESCRIPTOR *LocalDescriptor;
  DXIO_PORT_DESCRIPTOR    *EngineDescriptor;
  PCIe_ENGINE_CONFIG      *Engine;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  BixbyPresent = FALSE;

  SbLinkPresent = FALSE;
  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      if (EngineDescriptor->EngineData.EngineType == PciePortEngine) {
        IDS_HDT_CONSOLE (GNB_TRACE, "PCIe Entry in Topology at lanes %d - %d\n",
                         EngineDescriptor->EngineData.StartLane,
                         EngineDescriptor->EngineData.EndLane);
        if (EngineDescriptor->Port.MiscControls.SbLink == 1) {
          EngineDescriptor->Port.MiscControls.SbLink = 0;
          EngineDescriptor->Port.CsLink = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, "SbLink found\n");
          SbLinkPresent = TRUE;
        } else {
          EngineDescriptor->Port.CsLink = 0;
        }
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
  }

  Engine = PcieConfigGetChildEngine(GnbHandle);
  while (Engine != NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Read from port %d using address D%dF%d DATA = ",
                     Engine->Type.Port.PortId,
                     Engine->Type.Port.NativeDevNumber,
                     Engine->Type.Port.NativeFunNumber
                     );
    GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
    SocReadLcState0 (GnbHandle, Engine, &LcState0.Value);
    IDS_HDT_CONSOLE (GNB_TRACE, "0x%x\n", LcState0.Value);

    if ((LcState0.Field.LC_CURRENT_STATE > 0xF) && (LcState0.Field.LC_CURRENT_STATE < 0x1C)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Found early train link at Device %d, Function %d\n", Engine->Type.Port.NativeDevNumber, Engine->Type.Port.NativeFunNumber);
      Engine->Type.Port.PortData.MiscControls.CsLink = 1;
      BixbyPresent = TRUE;
      ComplexDescriptor->BmcLinkLocation = 0;
      EarlyTrainPort.AddressValue = MAKE_SBDFO (0, (UINT32) GnbHandle->Address.Address.Bus, Engine->Type.Port.NativeDevNumber, Engine->Type.Port.NativeFunNumber, 0);
      if (SbLinkPresent == FALSE) {
        IDS_HDT_CONSOLE (GNB_TRACE, "SbLink not found in topology - using default config\n");
        PcieConfigSetDescriptorFlags (Engine, DESCRIPTOR_ALLOCATED);
        Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;
        Engine->EngineData.EndLane = 15;
        Engine->EngineData.StartLane = 8;
        Engine->Type.Port.Address.AddressValue = EarlyTrainPort.AddressValue;
        Engine->Type.Port.PortData.DeviceNumber = Engine->Type.Port.NativeDevNumber;
        Engine->Type.Port.PortData.FunctionNumber = Engine->Type.Port.NativeFunNumber;
        ComplexDescriptor->BmcLinkLocation = 0xFF;
      }
      SocEnableEngineVisibility (GnbHandle, Engine);
      BixbySpeedConfig (GnbHandle, EarlyTrainPort);
      break;
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (Engine, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return (BixbyPresent);
}


