/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Configure Atomic Ops for endpoints
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2015-02-04 00:39:01 -0600 (Wed, 04 Feb 2015) $
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


#define FILECODE LIBRARY_PCIEMISCCOMMLIB_PCIEATOMICOPSLIB_FILECODE
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
SetPcieAopsCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  PCIE_DEVICE_TYPE      DeviceType;
  UINT8                 PcieCapPtr;
  UINT32                Value;
  UINT32                CapVersion;

  IDS_HDT_CONSOLE (GNB_TRACE, "  SetPcieAopsCallback for Device = %d:%d:%d\n",
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
      GnbLibPciScanSecondaryBus (Device, ScanData);
      break;
    case  PcieDeviceEndPoint:
      PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, NULL);
      if (PcieCapPtr != 0) {
        GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, NULL);
        CapVersion = (Value >> 16) & 0xF;
        if (CapVersion > 1) {
          GnbLibPciRead (
            Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
            AccessWidth32,
            &Value,
            NULL
          );
          Value |= BIT6;      // AtomicOp Requester Enable
          GnbLibPciWrite (
            Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
            AccessS3SaveWidth32,
            &Value,
            NULL
            );
        }
      }
      break;
    default:
      break;
    }
  }
  return SCAN_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init 10-Bit TAG on all active ports (Include CBS)
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 */

VOID
PcieAtomicOpsPortInitCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer
  )
{
  GNB_PCI_SCAN_DATA    ScanData;
  UINT8                 PcieCapPtr;
  UINT32                Value;
  UINT32                CapVersion;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAopsPortInitCallback Enter\n");
  if ((Engine->Type.Port.PortData.MiscControls.CsLink != 1) &&
      (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {

    PcieCapPtr = GnbLibFindPciCapability (Engine->Type.Port.Address.AddressValue, PCIE_CAP_ID, NULL);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Device(%d:%d:%d), PCIE_CAP_PTR at 0x%x\n",
                     Engine->Type.Port.Address.Address.Bus,
                     Engine->Type.Port.Address.Address.Device,
                     Engine->Type.Port.Address.Address.Function,
                     PcieCapPtr
                     );
    if (PcieCapPtr != 0) {
      GnbLibPciRead (Engine->Type.Port.Address.AddressValue | PcieCapPtr, AccessWidth32, &Value, NULL);
      CapVersion = (Value >> 16) & 0xF;
      if (CapVersion > 1) {
        GnbLibPciRead (
          Engine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_DEVICE_CAP2_REGISTER),
          AccessWidth32,
          &Value,
          NULL
        );
        if (0 != (Value & (BIT7 | BIT8))) {
          ScanData.GnbScanCallback = SetPcieAopsCallback;
          ScanData.StdHeader = NULL;
          GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &ScanData);
        }
      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAopsPortInitCallback Exit\n");
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure Atomic Ops on PCIE interface (Include CBS)
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieAtomicOpsInit (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG   *PcieEngine;
  PCIe_WRAPPER_CONFIG  *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAtomicOpsInit Enter\n");
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        PcieAtomicOpsPortInitCallback (PcieEngine, NULL);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAtomicOpsInit Exit\n");
}

