/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/* $NoKeywords:$ */
/**
 * @file
 *
 * Configure ECRC for endpoints
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

#define FILECODE LIBRARY_PCIEMISCCOMMLIB_PCIEECRCLIB_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct {
  GNB_PCI_SCAN_DATA       ScanData;
  BOOLEAN                 EndpointEcrcEnabled;
} PCIE_ECRC_DATA;

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
SetPcieEcrcCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  PCIE_DEVICE_TYPE                                  DeviceType;
  UINT16                                             PcieCapPtr;
  PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL  ErrCap;
  PCIE_ECRC_DATA                                    *PcieEcrcData;

  IDS_HDT_CONSOLE (GNB_TRACE, "  SetPcieEcrcCallback for Device = %d:%d:%d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    );

  PcieEcrcData = (PCIE_ECRC_DATA*) ScanData;
  PcieCapPtr = GnbLibFindPcieExtendedCapability (Device.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
  if (PcieCapPtr != 0) {
    DeviceType = GnbLibGetPcieDeviceType (Device, NULL);
    switch (DeviceType) {
    case  PcieDeviceRootComplex:
    case  PcieDeviceDownstreamPort:
    case  PcieDeviceUpstreamPort:
      GnbLibPciScanSecondaryBus (Device, ScanData);
      break;
    case  PcieDeviceEndPoint:
      GnbLibPciRead (
        Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
        AccessWidth32,
        &ErrCap,
        NULL
      );
      if (ErrCap.Field.EcrcGenerationCapable == 1) {
        PcieEcrcData->EndpointEcrcEnabled = TRUE;
        ErrCap.Field.EcrcGenerationEnable = 1;      // ECRC Generation Enable
        GnbLibPciWrite (
          Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
          AccessS3SaveWidth32,
          &ErrCap,
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
PcieEcrcPortInitCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer
  )
{
  PCIE_ECRC_DATA                                    EcrcData;
  UINT16                                            PcieCapPtr;
  PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL  ErrCap;
  PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY      ErrSev;

  EcrcData.EndpointEcrcEnabled = FALSE;
  EcrcData.ScanData.StdHeader = NULL;

  if ((Engine->Type.Port.PortData.MiscControls.CsLink != 1) &&
      (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEcrcPortInitCallback Enter\n");
    PcieCapPtr = GnbLibFindPcieExtendedCapability (Engine->Type.Port.Address.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Device(%d:%d:%d), PCIE_EXT_AER_CAP_ID at 0x%x\n",
                     Engine->Type.Port.Address.Address.Bus,
                     Engine->Type.Port.Address.Address.Device,
                     Engine->Type.Port.Address.Address.Function,
                     PcieCapPtr
                     );
    if (PcieCapPtr != 0) {
      GnbLibPciRead (
        Engine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
        AccessWidth32,
        &ErrCap,
        NULL
      );
      if (ErrCap.Field.EcrcCheckCapable == 1) {
        EcrcData.ScanData.GnbScanCallback = SetPcieEcrcCallback;
        GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &EcrcData.ScanData);
        if (TRUE == EcrcData.EndpointEcrcEnabled) {
          GnbLibPciRead (
            Engine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
            AccessWidth32,
            &ErrSev,
            NULL
          );
          ErrSev.Field.ECRCErrorSeverity = 1;
          GnbLibPciWrite (
            Engine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
            AccessS3SaveWidth32,
            &ErrSev,
            NULL
          );
          ErrCap.Field.EcrcCheckEnable = 1;
          GnbLibPciWrite (
            Engine->Type.Port.Address.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
            AccessS3SaveWidth32,
            &ErrCap,
            NULL
          );
        }

      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEcrcPortInitCallback Exit\n");
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure ECRC on PCIE interface (Include CBS)
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieEcrcInit (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG   *PcieEngine;
  PCIe_WRAPPER_CONFIG  *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEcrcInit Enter\n");
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        PcieEcrcPortInitCallback (PcieEngine, NULL);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEcrcInit Exit\n");
}

