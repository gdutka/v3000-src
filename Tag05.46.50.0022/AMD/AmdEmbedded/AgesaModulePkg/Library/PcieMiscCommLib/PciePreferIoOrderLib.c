/**
 *  @file PciePreferIoOrderLib.c
 *  @brief PCIe Preferred IO Ordering
 */
/*****************************************************************************
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
#include  <Library/IdsLib.h>
#include  <Library/GnbPcieConfigLib.h>
#include  <Library/GnbCommonLib.h>
#include  <Library/PcieMiscCommLib.h>
#include  <Library/PcdLib.h>

#define FILECODE LIBRARY_PCIEMISCCOMMLIB_PCIEPREFERIOORDERLIB_FILECODE
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
  BOOLEAN                 PreferIoDeviceFound;
  UINT32                  PreferIoBusDevFun;
} PCIE_PREFERIO_CAPABILITY_DATA;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Scan Preferred IO Device callback
 *
 * @param[in]     Device          PCI Address
 * @param[in,out] ScanData        Scan configuration data
 * @retval                        Scan Status of 0
 */

SCAN_STATUS
STATIC
ScanPreferIoDevice (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  )
{
  PCIE_PREFERIO_CAPABILITY_DATA     *PciePreferIoCapData;
  PCIE_DEVICE_TYPE                  DeviceType;
  PCI_ADDR                          TargetDev;
  SCAN_STATUS                       ScanStatus;

  TargetDev.AddressValue = 0;
  ScanStatus = SCAN_SUCCESS;
  PciePreferIoCapData = (PCIE_PREFERIO_CAPABILITY_DATA*) ScanData;
  IDS_HDT_CONSOLE (GNB_TRACE, " -Scan PreferIoDevice = %d:%d:%d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    );

  DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
  switch (DeviceType) {
  case  PcieDeviceRootComplex:
  case  PcieDeviceDownstreamPort:
  case  PcieDeviceUpstreamPort:
    GnbLibPciScanSecondaryBus (Device, &PciePreferIoCapData->ScanData);
    break;
  case  PcieDeviceLegacyEndPoint:
  case  PcieDeviceEndPoint:
    TargetDev.Address.Bus = (PciePreferIoCapData->PreferIoBusDevFun >> 16) & 0xFF;
    TargetDev.Address.Device = (PciePreferIoCapData->PreferIoBusDevFun >> 8) & 0x1F;
    TargetDev.Address.Function = PciePreferIoCapData->PreferIoBusDevFun & 0x7;
    if (Device.AddressValue == TargetDev.AddressValue) {
      IDS_HDT_CONSOLE (GNB_TRACE, " -Found PreferIoDevice = %d:%d:%d\n",
        Device.Address.Bus,
        Device.Address.Device,
        Device.Address.Function
        );
      PciePreferIoCapData->PreferIoDeviceFound = TRUE;
      ScanStatus = SCAN_SKIP_BUSES | SCAN_SKIP_DEVICES | SCAN_SKIP_FUNCTIONS;
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
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 *
 */

VOID
STATIC
PciePreferIoOrderCallback (
  IN  PCIe_ENGINE_CONFIG    *Engine,
  IN  PROGRAM_PIO_REGISTER  ProgRegister
  )
{
  PCIE_PREFERIO_CAPABILITY_DATA     PciePreferIoCapData;
  GNB_HANDLE                        *GnbHandle;

  if (ProgRegister == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Invalid Parameter -ExecutePreferredIo\n");
    return;
  }
  if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS) && (PcdGet32(PcdAmdPreferredIODevice) != 0)) {
    PciePreferIoCapData.PreferIoDeviceFound = FALSE;
    PciePreferIoCapData.PreferIoBusDevFun = PcdGet32(PcdAmdPreferredIODevice);
    IDS_HDT_CONSOLE (GNB_TRACE, "PreferIo BusDevFun = 0x%08x\n", PciePreferIoCapData.PreferIoBusDevFun);
    PciePreferIoCapData.ScanData.StdHeader = NULL;
    PciePreferIoCapData.ScanData.GnbScanCallback = ScanPreferIoDevice;
    GnbLibPciScan (Engine->Type.Port.Address, Engine->Type.Port.Address, &PciePreferIoCapData.ScanData);

    if (PciePreferIoCapData.PreferIoDeviceFound) {
      GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
      ProgRegister (GnbHandle);
    }
  }
}

/**----------------------------------------------------------------------------------------*/
/**
 * Preferred IO Ordering Enable
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/

VOID
PciePreferredIoOrder (
  IN  GNB_HANDLE            *GnbHandle,
  IN  PROGRAM_PIO_REGISTER  ProgRegister
  )
{
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;

  if ((!PcdGet32(PcdAmdPreferredIODevice)) && (PcdGet16(PcdAmdPreferredIOBus) == 0xFFFF)) {
    return;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePreferredIoOrder - Enter\n");

  if (ProgRegister == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Invalid Parameter - ProgRegister\n");
    return;
  }
  if (PcdGet16(PcdAmdPreferredIOBus) != 0xFFFF) {
    if (((UINT8)PcdGet16(PcdAmdPreferredIOBus) >= (UINT8)GnbHandle->Address.Address.Bus) &&
        ((UINT8)PcdGet16(PcdAmdPreferredIOBus) <= (UINT8)GnbHandle->BusNumberLimit)) {
      IDS_HDT_CONSOLE (GNB_TRACE, " -Set Enabled PreferredIoBus = %d\n", GnbHandle->Address.Address.Bus);
      ProgRegister (GnbHandle);
    }
    return;
  }

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        PciePreferIoOrderCallback (PcieEngine, ProgRegister);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePreferredIoOrder - Exit\n");
}


