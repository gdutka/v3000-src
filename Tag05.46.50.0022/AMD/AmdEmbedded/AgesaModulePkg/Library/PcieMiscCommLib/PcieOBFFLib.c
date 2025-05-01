/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe Optimized Buffer Flush and Fill
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2020-05-12 21:00:43 -0600 (Tues, 12 May 2020) $
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

#define FILECODE LIBRARY_PCIEMISCCOMMLIB_PCIEOBFFLIB_FILECODE

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
  BOOLEAN                 OBFFEn;
  PCI_ADDR                DownstreamPort;
} PCIE_OBFF_DATA;
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
GetPcieOBFFCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  SCAN_STATUS           ScanStatus;
  PCIE_OBFF_DATA        *PcieOBFFData;
  PCIE_DEVICE_TYPE      DeviceType;
  UINT8                 PcieCapPtr;
  UINT32                Value;
  UINT32                CapVersion;

  ScanStatus = SCAN_SUCCESS;
  PcieOBFFData = (PCIE_OBFF_DATA*) ScanData;
  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "    Device ID = %d:%d:%d, PCIE_CAP_ID at 0x%x\n",
                   Device.Address.Bus,
                   Device.Address.Device,
                   Device.Address.Function,
                   PcieCapPtr
                   );
  GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, ScanData->StdHeader);
  CapVersion = (Value >> 16) & 0xF;
  if ((PcieCapPtr != 0) && (CapVersion > 1)) {
    GnbLibPciRead (
      Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CAP2_REGISTER),
      AccessWidth32,
      &Value,
      ScanData->StdHeader
    );
    IDS_HDT_CONSOLE (GNB_TRACE, "     - PCIE_DEVICE_CAP2 Value = 0x%x\n", Value);
    if ((Value & (BIT18 | BIT19)) == 0) { // OBFF_SUPPORTED
      PcieOBFFData->OBFFEn = FALSE;
    }
    DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
    switch (DeviceType) {
    case  PcieDeviceRootComplex:
    case  PcieDeviceDownstreamPort:
    case  PcieDeviceUpstreamPort:
      GnbLibPciScanSecondaryBus (Device, &PcieOBFFData->ScanData);
      break;
    case  PcieDeviceEndPoint:
    case  PcieDeviceLegacyEndPoint:
      break;
    default:
      break;
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "  GetPcieOBFFCallback for Device = %d:%d:%d  (OBFF_SUPPORTED=%d)\n",
      Device.Address.Bus,
      Device.Address.Device,
      Device.Address.Function,
      PcieOBFFData->OBFFEn
      );
  } else {
    PcieOBFFData->OBFFEn = FALSE;
    IDS_HDT_CONSOLE (GNB_TRACE, "  GetPcieOBFFCallback for Device = %d:%d:%d  PCIE_CAP_ID not found.\n",
      Device.Address.Bus,
      Device.Address.Device,
      Device.Address.Function
      );
  }

  return ScanStatus;
}

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
SetPcieOBFFCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  PCIE_OBFF_DATA        *PcieOBFFData;
  PCIE_DEVICE_TYPE      DeviceType;
  UINT8                 PcieCapPtr;
  UINT32                Value;
  UINT32                CapVersion;

  PcieOBFFData = (PCIE_OBFF_DATA*) ScanData;
  IDS_HDT_CONSOLE (GNB_TRACE, "  SetPcieOBFFCallback for Device = %d:%d:%d to %d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function,
    PcieOBFFData->OBFFEn
    );

  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
  GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, ScanData->StdHeader);
  CapVersion = (Value >> 16) & 0xF;
  if ((PcieCapPtr != 0) && (CapVersion > 1)) {
    DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
    if (PcieOBFFData->OBFFEn) {
      switch (DeviceType) {
      case  PcieDeviceRootComplex:
      case  PcieDeviceDownstreamPort:
      case  PcieDeviceUpstreamPort:
        PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
        GnbLibPciRead (
          Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
          AccessWidth32,
          &Value,
          ScanData->StdHeader
        );
        Value |= (BIT13|BIT14);
        GnbLibPciWrite (
          Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
          AccessS3SaveWidth32,
          &Value,
          ScanData->StdHeader
          );
        IDS_HDT_CONSOLE (GNB_TRACE, "     - Set Device CNTL2 Value = 0x%x\n", Value);

        GnbLibPciScanSecondaryBus (Device, &PcieOBFFData->ScanData);
        break;
      case  PcieDeviceEndPoint:
      case  PcieDeviceLegacyEndPoint:
        PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
        GnbLibPciRead (
          Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
          AccessWidth32,
          &Value,
          ScanData->StdHeader
        );
        Value |= (BIT13|BIT14);
        GnbLibPciWrite (
          Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
          AccessS3SaveWidth32,
          &Value,
          ScanData->StdHeader
          );
        IDS_HDT_CONSOLE (GNB_TRACE, "     - Set Device CNTL2 Value = 0x%x\n", Value);
        break;
      default:
        break;
      }
    }
  }
  return SCAN_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init LTR on all active ports
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 */

VOID
PcieOBFFInitCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer
  )
{
  PCIE_OBFF_DATA PcieOBFFData;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieOBFFInitCallback Enter\n");
  PcieOBFFData.OBFFEn = TRUE;
  PcieOBFFData.ScanData.StdHeader = NULL;

  PcieOBFFData.ScanData.GnbScanCallback = GetPcieOBFFCallback;
  GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieOBFFData.ScanData);

  PcieOBFFData.ScanData.GnbScanCallback = SetPcieOBFFCallback;
  GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieOBFFData.ScanData);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieOBFFInitCallback Exit\n");
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to enable Latency Tolerance Reporting
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/
VOID
PcieOBFFInterface (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "Pcie OBFF Interface Enter\n");
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        PcieOBFFInitCallback (PcieEngine, NULL);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Pcie OBFF Interface Exit\n");
}


