/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Configure SRIS for endpoints
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
#include  <Library/GnbPciAccLib.h>
#include  <Library/GnbCommonLib.h>
#include  <GnbRegistersRMB.h>

#define FILECODE NBIO_PCIE_RMB_DXE_AMDNBIOPCIESRIS_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

typedef struct {
  GNB_PCI_SCAN_DATA                 ScanData;
  UINT16                            CapVersion;
  UINT16                            SecExtCapPtr;
  UINT32                            DevAddr;
  PCIE_LINK_CAP2_REGISTER_STRUCT    LinkCap2;
  BOOLEAN                           ProgFlag;
} PCIE_SRIS_CAPABILITY_DATA;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Evaluate device Sris Capability
 *
 *
 *
 * @param[in]     Device          PCI Address
 * @param[in,out] ScanData        Scan configuration data
 * @retval                        Scan Status of 0
 */

SCAN_STATUS
STATIC
PcieGetSrisCapabilityCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  SCAN_STATUS                       ScanStatus;
  PCIE_SRIS_CAPABILITY_DATA         UpStmDevData;
  PCIE_SRIS_CAPABILITY_DATA         *UpStmDevPtr;
  PCIE_DEVICE_TYPE                  DeviceType;
  PCIE_LINK_CAP2_REGISTER_STRUCT    LinkCap2;
  PCIE_LINK_CTRL3_REGISTER_STRUCT   LinkCtrl3;
  UINT8                             EnLowerSkpOsGenSpd;
  UINT8                             PcieCapPtr;
  UINT16                            SecExtCapPtr;
  UINT16                            CapVersion;

  ScanStatus = SCAN_SUCCESS;
  LinkCap2.Value = 0;
  CapVersion = 0;
  SecExtCapPtr = 0;
  EnLowerSkpOsGenSpd = 0;

  UpStmDevPtr = (PCIE_SRIS_CAPABILITY_DATA*) ScanData;

  IDS_HDT_CONSOLE (GNB_TRACE, "  %a for Device = %d:%d:%d\n",
    __FUNCTION__,
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    );

  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "  -- PCIE_CAP_ID PcieCapPtr = 0x%x\n", PcieCapPtr);
  SecExtCapPtr = GnbLibFindPcieExtendedCapability (Device.AddressValue, PCIE_SEC_EXT_CAP_CAP_ID, ScanData->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "  -- PCIE_SEC_EXT_CAP_CAP_ID SecExtCapPtr = 0x%x\n", SecExtCapPtr);

  if (PcieCapPtr != 0) {
    GnbLibPciRead ( Device.AddressValue | (PcieCapPtr + 2) , AccessWidth16, &CapVersion, ScanData->StdHeader);
    CapVersion &= 0xF;
    if (CapVersion > 1) {
      GnbLibPciRead ( Device.AddressValue | (PcieCapPtr + PCIE_LINK_CAP2_REGISTER) , AccessWidth32, &LinkCap2.Value, ScanData->StdHeader);
    }
  }

  DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "  -- DeviceType = 0x%x\n", DeviceType);
  switch (DeviceType) {
  case  PcieDeviceRootComplex:
  case  PcieDeviceDownstreamPort:
    UpStmDevData.ScanData.StdHeader = ScanData->StdHeader;
    UpStmDevData.ScanData.GnbScanCallback = ScanData->GnbScanCallback;
    UpStmDevData.CapVersion = CapVersion;
    UpStmDevData.SecExtCapPtr = SecExtCapPtr;
    UpStmDevData.DevAddr = Device.AddressValue;
    UpStmDevData.LinkCap2.Value = LinkCap2.Value;
    UpStmDevData.ProgFlag = FALSE;
    GnbLibPciScanSecondaryBus (Device, &UpStmDevData.ScanData);
    break;

  case  PcieDeviceUpstreamPort:
  case  PcieDeviceEndPoint:
  case  PcieDeviceLegacyEndPoint:
    if ((UpStmDevPtr->CapVersion > 1) && (CapVersion > 1)) {

      // Program UpStream Device
      if ((UpStmDevPtr->LinkCap2.Field.LowerSkpOsGenSupportSpdVec != 0) && (LinkCap2.Field.LowerSkpOsRcpSupportedSpdVec != 0) &&\
          (UpStmDevPtr->DevAddr != 0) && (UpStmDevPtr->SecExtCapPtr != 0) && (!UpStmDevPtr->ProgFlag)) {

        EnLowerSkpOsGenSpd = (UpStmDevPtr->LinkCap2.Field.LowerSkpOsGenSupportSpdVec < LinkCap2.Field.LowerSkpOsRcpSupportedSpdVec) ?\
                              UpStmDevPtr->LinkCap2.Field.LowerSkpOsGenSupportSpdVec : LinkCap2.Field.LowerSkpOsRcpSupportedSpdVec;

        GnbLibPciRead ( UpStmDevPtr->DevAddr | (UpStmDevPtr->SecExtCapPtr + PCIE_LINK_CTRL3_REGISTER),
                        AccessWidth16, &LinkCtrl3.Value, ScanData->StdHeader );
        LinkCtrl3.Field.EnableLowerSkpOsGenVec = EnLowerSkpOsGenSpd;
        IDS_HDT_CONSOLE (GNB_TRACE, "  -- UpStreamDevice PCIE_LINK_CTRL3_REGISTER Addr=0x%x, Val=0x%x\n",\
                         UpStmDevPtr->DevAddr | (UpStmDevPtr->SecExtCapPtr + PCIE_LINK_CTRL3_REGISTER), LinkCtrl3.Value);
        GnbLibPciWrite (UpStmDevPtr->DevAddr | (UpStmDevPtr->SecExtCapPtr + PCIE_LINK_CTRL3_REGISTER),
                        AccessWidth16, &LinkCtrl3.Value, ScanData->StdHeader);

        UpStmDevPtr->ProgFlag = TRUE;
      }

      // Program DownStream Device
      if ((UpStmDevPtr->LinkCap2.Field.LowerSkpOsRcpSupportedSpdVec != 0) &&\
          (LinkCap2.Field.LowerSkpOsGenSupportSpdVec != 0) && (SecExtCapPtr != 0)) {

        EnLowerSkpOsGenSpd = (UpStmDevPtr->LinkCap2.Field.LowerSkpOsRcpSupportedSpdVec < LinkCap2.Field.LowerSkpOsGenSupportSpdVec) ?\
                              UpStmDevPtr->LinkCap2.Field.LowerSkpOsRcpSupportedSpdVec : LinkCap2.Field.LowerSkpOsGenSupportSpdVec;

        GnbLibPciRead ( Device.AddressValue | (SecExtCapPtr + PCIE_LINK_CTRL3_REGISTER),
                        AccessWidth16, &LinkCtrl3.Value, ScanData->StdHeader);
        LinkCtrl3.Field.EnableLowerSkpOsGenVec = EnLowerSkpOsGenSpd;
        IDS_HDT_CONSOLE (GNB_TRACE, "  -- DownStreamDevice PCIE_LINK_CTRL3_REGISTER Addr=0x%x, Val=0x%x\n",\
                         Device.AddressValue | (SecExtCapPtr + PCIE_LINK_CTRL3_REGISTER), LinkCtrl3.Value);
        GnbLibPciWrite (Device.AddressValue | (SecExtCapPtr + PCIE_LINK_CTRL3_REGISTER),
                        AccessWidth16, &LinkCtrl3.Value, ScanData->StdHeader);
      }
    }
    break;
  default:
    break;
  }

  return ScanStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all active ports
 *
 *
 *
 *
 * @param[in]       GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]       Engine         Pointer to engine config descriptor
 * @param[in, out]  Buffer         Not used
 *
 */

VOID
PcieSrisInitCallback (
  IN       GNB_HANDLE            *GnbHandle,
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer
  )
{
  PCIE_SRIS_CAPABILITY_DATA  PcieSrisCapabilityData;

  IDS_HDT_CONSOLE (GNB_TRACE, "  %a for Device = %d:%d:%d\n",
    __FUNCTION__,
    Engine->Type.Port.Address.Address.Bus,
    Engine->Type.Port.Address.Address.Device,
    Engine->Type.Port.Address.Address.Function
    );

  if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
    if ((Engine->Type.Port.SrisEnableMode == 1) && (Engine->Type.Port.SrisAutoDetectMode != 1)) {
      PcieSrisCapabilityData.ScanData.StdHeader = NULL;
      PcieSrisCapabilityData.ScanData.GnbScanCallback = PcieGetSrisCapabilityCallback;
      GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieSrisCapabilityData.ScanData);
    }
  }

}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure SRIS on PCIE interface
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieSrisInit (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG   *PcieEngine;
  PCIe_WRAPPER_CONFIG  *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if ((PcieLibIsEngineAllocated (PcieEngine)) &&\
          ((PcieEngine->Type.Port.SrisEnableMode == 1) && (PcieEngine->Type.Port.SrisAutoDetectMode != 1))) {
        PcieSrisInitCallback (GnbHandle, PcieEngine, NULL );
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}


