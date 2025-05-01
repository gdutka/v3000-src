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
#include  <GnbDxio.h>
#include  <Filecode.h>
#include  <Library/IdsLib.h>
#include  <Library/GnbPcieConfigLib.h>
#include  <Library/GnbCommonLib.h>
#include  <Library/PcieMiscCommLib.h>


#define FILECODE LIBRARY_PCIEMISCCOMMLIB_PCIEAERSETTING_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Evaluate device
 *
 *
 *
 * @param[in]     Device          PCI Address
 * @param[in,out] ScanData        Scan configuration data
 * @retval                        Scan Status of 0
 */

SCAN_STATUS
SetPcieAerCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  PCIE_DEVICE_TYPE                      DeviceType;
  UINT8                                 PcieCapPtr;
  UINT32                                Value;
  UINT32                                CapVersion;
  PCICFG_SPACE_BRIDGE_CONTROL_STRUCT    BridgeControlReg;
  PCIE_DEVICE_CTRL_REGISTER_STRUCT      DeviceControlReg;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter for 0x%x:0x%x:0x%x\n",
    __FUNCTION__,
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    );

  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, NULL);
  GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, NULL);
  CapVersion = (Value >> 16) & 0xF;
  if ((PcieCapPtr != 0) && (CapVersion > 1)) {
    DeviceType = GnbLibGetPcieDeviceType (Device, NULL);
    switch (DeviceType) {
    case  PcieDeviceRootComplex:
    case  PcieDeviceDownstreamPort:
    case  PcieDeviceUpstreamPort:
      // Set Serr_En in BRIDGE_CONTROL
      // IDS_HDT_CONSOLE(MAIN_FLOW, "%a Bridge BDF = %d %d %d\n", __FUNCTION__, Bus, Dev, Fun);
      GnbLibPciRead(
        Device.AddressValue | PCICFG_SPACE_BRIDGE_CONTROL_OFFSET,
        AccessWidth16,
        &(BridgeControlReg.Value),
        NULL
        );
      BridgeControlReg.Field.SerrEnable = 1;
      GnbLibPciWrite(
        Device.AddressValue | PCICFG_SPACE_BRIDGE_CONTROL_OFFSET,
        AccessWidth16,
        &(BridgeControlReg.Value),
        NULL
        );
      GnbLibPciScanSecondaryBus (Device, ScanData);
      break;
    case  PcieDeviceEndPoint:
      if (PcieCapPtr != 0) {
        // Set CORR_ERR_EN, NONFATAL_ERR_EN, FATAL_ERR_EN in DEVICE_CNTRL
        // IDS_HDT_CONSOLE(MAIN_FLOW, "  %a Device BDF = %d %d %d\n", __FUNCTION__, Bus, Dev, Fun);
        GnbLibPciRead (
          Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CTRL_REGISTER),
          AccessWidth16,
          &(DeviceControlReg.Value),
          NULL
        );
        DeviceControlReg.Field.CorrectableErrorReportingEn = 1;
        DeviceControlReg.Field.NonFatalErrorReportingEn = 1;
        DeviceControlReg.Field.FatalErrorReportingEn = 1;
        GnbLibPciWrite (
          Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CTRL_REGISTER),
          AccessWidth16,
          &(DeviceControlReg.Value),
          NULL
        );
      }
      break;
    default:
      break;
    }
  }
  return SCAN_SUCCESS;
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure AER for firmware first on PCIE interface
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieAerPortInit (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG   *Engine;
  PCIe_WRAPPER_CONFIG  *Wrapper;
  GNB_PCI_SCAN_DATA    ScanData;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  Wrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (Wrapper != NULL) {
    Engine = PcieConfigGetChildEngine (Wrapper);
    while (Engine != NULL) {
      if (PcieLibIsEngineAllocated (Engine) &&
           (Engine->Type.Port.PortData.MiscControls.CsLink != 1) &&
           (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {
                ScanData.GnbScanCallback = SetPcieAerCallback;
                ScanData.StdHeader = NULL;
                GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &ScanData);
      }
      Engine = PcieLibGetNextDescriptor (Engine);
    }
    Wrapper = PcieLibGetNextDescriptor (Wrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure AER for Firmware First on a single root port
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieAerNbifInit (
  IN       PCI_ADDR   NbifAddress
  )
{
  GNB_PCI_SCAN_DATA    ScanData;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
    ScanData.GnbScanCallback = SetPcieAerCallback;
    ScanData.StdHeader = NULL;
    GnbLibPciScan (NbifAddress, NbifAddress, &ScanData);
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

