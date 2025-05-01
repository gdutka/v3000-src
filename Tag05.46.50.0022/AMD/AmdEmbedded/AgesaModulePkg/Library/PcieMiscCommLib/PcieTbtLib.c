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


#define FILECODE LIBRARY_PCIEMISCCOMMLIB_PCIETBTLIB_FILECODE
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
  BOOLEAN                 TbtCompleterEn;
  BOOLEAN                 TbtRequesterEn;
} PCIE_TBT_DATA;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

UINT16  TbtBlacklistDeviceTable[] = {
//  0x15B3, 0x1019, 0, 0,
//  0x15B3, 0x101B, 0, 0,
  0xFFFF, 0xFFFF, 0, 0      //End of Table
};
/*----------------------------------------------------------------------------------------*/
/**
 * Pcie 10-bit Tag Black List
 *
 *
 *
 * @param[in] Device              PCI_ADDR of PCIe Device to evaluate
 * @param[in] PcieTbtData         Pointer to PCIE_TBT_DATA
 * @param[in] StdHeader           Standard configuration header
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
PcieTbtBlackListFeature (
  IN       PCI_ADDR               Device,
  IN       PCIE_TBT_DATA          *PcieTbtData,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32  TargetDeviceId;
  UINTN   i;
  UINT32  DeviceId;
  UINT32  VendorId;

  GnbLibPciRead (Device.AddressValue, AccessWidth32, &TargetDeviceId, StdHeader);
  for (i = 0; i < (sizeof (TbtBlacklistDeviceTable) / sizeof (UINT16)); i = i + 4) {
    VendorId = TbtBlacklistDeviceTable[i];
    if (VendorId == 0xFFFF) {
      break;
    }
    DeviceId = TbtBlacklistDeviceTable[i + 1];
    if (VendorId == (UINT16)TargetDeviceId) {
      if (DeviceId == 0xFFFF || DeviceId == (TargetDeviceId >> 16)) {
        PcieTbtData->TbtCompleterEn = (BOOLEAN) TbtBlacklistDeviceTable[i + 2];
        PcieTbtData->TbtRequesterEn = (BOOLEAN) TbtBlacklistDeviceTable[i + 3];
        IDS_HDT_CONSOLE (GNB_TRACE, "  10-bit Tag: 0x%x TbtCompleterEn=%d TbtRequesterEn=%d\n", TargetDeviceId, PcieTbtData->TbtCompleterEn, PcieTbtData->TbtRequesterEn);
      }
    }
  }
  return AGESA_SUCCESS;
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
GetPcieTbtCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  SCAN_STATUS           ScanStatus;
  PCIE_TBT_DATA         *PcieTbtData;
  PCIE_DEVICE_TYPE      DeviceType;
  UINT8                 PcieCapPtr;
  UINT32                Value;
  UINT32                CapVersion;
  UINT8                 SecondaryBus;
  PCI_ADDR              PciDevice;

  ScanStatus = SCAN_SUCCESS;
  PcieTbtData = (PCIE_TBT_DATA*) ScanData;
  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Device(%d:%d:%d), PCIE_CAP_PTR at 0x%x\n",
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
    IDS_HDT_CONSOLE (GNB_TRACE, "    PCIE_DEVICE_CAP2 Value = 0x%x\n", Value);
    if (0 == (Value & BIT16)) {
      PcieTbtData->TbtCompleterEn = FALSE;
    }
    if (0 == (Value & BIT17)) {
      PcieTbtData->TbtRequesterEn = FALSE;
    }
    DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
    IDS_HDT_CONSOLE (GNB_TRACE, "    Device Type = 0x%x\n", DeviceType);
    switch (DeviceType) {
    case  PcieDeviceRootComplex:
    case  PcieDeviceDownstreamPort:
    case  PcieDeviceUpstreamPort:
      GnbLibPciRead (Device.AddressValue | 0x19, AccessWidth8, &SecondaryBus, ScanData->StdHeader);
      if (SecondaryBus != 0) {
        PciDevice.AddressValue = MAKE_SBDFO (0, SecondaryBus, 0, 0, 0);
        if (GnbLibPciIsDevicePresent (PciDevice.AddressValue, ScanData->StdHeader)) {
          GnbLibPciScanSecondaryBus (Device, &PcieTbtData->ScanData);
        } else {
          PcieTbtData->TbtCompleterEn = FALSE;
          PcieTbtData->TbtRequesterEn = FALSE;
        }
      } else {
        PcieTbtData->TbtCompleterEn = FALSE;
        PcieTbtData->TbtRequesterEn = FALSE;
      }
      break;
    case  PcieDeviceEndPoint:
    case  PcieDeviceLegacyEndPoint:
      break;
    default:
      break;
    }
    PcieTbtBlackListFeature (Device, (PCIE_TBT_DATA*) ScanData, ScanData->StdHeader);
    IDS_HDT_CONSOLE (GNB_TRACE, "  GetPcieTbtCallback for Device = %d:%d:%d  (TbtCompleterEn=%d - TbtRequesterEn=%d)\n",
      Device.Address.Bus,
      Device.Address.Device,
      Device.Address.Function,
      PcieTbtData->TbtCompleterEn,
      PcieTbtData->TbtRequesterEn
      );
  } else {
    PcieTbtData->TbtCompleterEn = FALSE;
    PcieTbtData->TbtRequesterEn = FALSE;
    IDS_HDT_CONSOLE (GNB_TRACE, "  GetPcieTbtCallback for Device = %d:%d:%d  PCIE_CAP_ID not found.\n",
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
SetPcieTbtCallback (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  PCIE_TBT_DATA         *PcieTbtData;
  PCIE_DEVICE_TYPE      DeviceType;
  UINT8                 PcieCapPtr;
  UINT32                Value;
  UINT32                CapVersion;

  PcieTbtData = (PCIE_TBT_DATA*) ScanData;
  IDS_HDT_CONSOLE (GNB_TRACE, "  SetPcieTbtCallback for Device = %d:%d:%d to TbtCompleterEn=%d - TbtRequesterEn=%d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function,
    PcieTbtData->TbtCompleterEn,
    PcieTbtData->TbtRequesterEn
    );

  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
  GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, ScanData->StdHeader);
  CapVersion = (Value >> 16) & 0xF;
  if ((PcieCapPtr != 0) && (CapVersion > 1)) {
    DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
    switch (DeviceType) {
    case  PcieDeviceRootComplex:
    case  PcieDeviceDownstreamPort:
    case  PcieDeviceUpstreamPort:
      GnbLibPciScanSecondaryBus (Device, &PcieTbtData->ScanData);
      if (PcieTbtData->TbtRequesterEn || PcieTbtData->TbtCompleterEn) {
        PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
        GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, ScanData->StdHeader);
        CapVersion = (Value >> 16) & 0xF;
        if ((PcieCapPtr != 0) && (CapVersion > 1)) {
          GnbLibPciRead (
            Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CAP2_REGISTER),
            AccessWidth32,
            &Value,
            ScanData->StdHeader
          );
          if (Value & BIT17) {
            GnbLibPciRead (
              Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
              AccessWidth32,
              &Value,
              ScanData->StdHeader
            );
            Value |= BIT12;
            GnbLibPciWrite (
              Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
              AccessS3SaveWidth32,
              &Value,
              ScanData->StdHeader
              );
          }
        }
      }
      break;
    case  PcieDeviceEndPoint:
    case  PcieDeviceLegacyEndPoint:
      if (PcieTbtData->TbtRequesterEn) {
        PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, ScanData->StdHeader);
        GnbLibPciRead (Device.AddressValue | PcieCapPtr, AccessWidth32, &Value, ScanData->StdHeader);
        CapVersion = (Value >> 16) & 0xF;
        if ((PcieCapPtr != 0) && (CapVersion > 1)) {
          GnbLibPciRead (
            Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CAP2_REGISTER),
            AccessWidth32,
            &Value,
            ScanData->StdHeader
          );
          if (Value & BIT17) {
            GnbLibPciRead (
              Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
              AccessWidth32,
              &Value,
              ScanData->StdHeader
            );
            Value |= BIT12;
            GnbLibPciWrite (
              Device.AddressValue | (PcieCapPtr + PCIE_DEVICE_CNTL2_REGISTER),
              AccessS3SaveWidth32,
              &Value,
              ScanData->StdHeader
              );
          }
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
 * Callback to init 10-Bit TAG on all active ports
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 */

VOID
PcieTbtPortInitCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer
  )
{
  PCIE_TBT_DATA PcieTbtData;

  if ((Engine->Type.Port.PortData.MiscControls.CsLink != 1) &&
      (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtPortInitCallback Enter\n");
    PcieTbtData.TbtRequesterEn = TRUE;
    PcieTbtData.TbtCompleterEn = TRUE;
    PcieTbtData.ScanData.StdHeader = NULL;

    PcieTbtData.ScanData.GnbScanCallback = GetPcieTbtCallback;
    GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieTbtData.ScanData);

    if (Engine->Type.Port.PortFeatures.TenBitTagControl == 0) {
      PcieTbtData.TbtRequesterEn = FALSE;
      PcieTbtData.TbtCompleterEn = FALSE;
    }

    if (PcieTbtData.TbtRequesterEn || PcieTbtData.TbtCompleterEn) {
      PcieTbtData.ScanData.GnbScanCallback = SetPcieTbtCallback;
      GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieTbtData.ScanData);
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtPortInitCallback Exit\n");
  }
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure 10-Bit TAG on PCIE interface
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieTbtInit (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG   *PcieEngine;
  PCIe_WRAPPER_CONFIG  *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtInit Enter\n");
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        PcieTbtPortInitCallback (PcieEngine, NULL);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtInit Exit\n");
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
PcieTbtPortInitCallbackV2 (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer
  )
{
  PCIE_TBT_DATA PcieTbtData;

  if ((Engine->Type.Port.PortData.MiscControls.CsLink != 1) &&
      (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtPortInitCallback Enter\n");
    PcieTbtData.TbtRequesterEn = TRUE;
    PcieTbtData.TbtCompleterEn = TRUE;
    PcieTbtData.ScanData.StdHeader = NULL;

    PcieTbtData.ScanData.GnbScanCallback = GetPcieTbtCallback;
    GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieTbtData.ScanData);


    if (Engine->Type.Port.PortFeatures.TenBitTagControl == 0) {
      PcieTbtData.TbtRequesterEn = FALSE;
      PcieTbtData.TbtCompleterEn = FALSE;
    }

    if (PcieTbtData.TbtRequesterEn || PcieTbtData.TbtCompleterEn) {
      PcieTbtData.ScanData.GnbScanCallback = SetPcieTbtCallback;
      GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PcieTbtData.ScanData);
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtPortInitCallback Exit\n");
  }
}

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to configure 10-Bit TAG on PCIE interface (Include CBS)
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PcieTbtInitV2 (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG   *PcieEngine;
  PCIe_WRAPPER_CONFIG  *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtInit Enter\n");
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        PcieTbtPortInitCallbackV2 (PcieEngine, NULL);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTbtInit Exit\n");
}

