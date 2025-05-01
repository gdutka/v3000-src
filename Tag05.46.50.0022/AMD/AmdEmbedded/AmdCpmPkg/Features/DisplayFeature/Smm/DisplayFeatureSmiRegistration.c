/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmSmm.h>
#include <Features/DisplayFeature/Include/DisplayCbsLib.h>
#include <AmdCpmTable.h>

#define RetryTimer            2 * 10    // Max 2 second retry timer

CPM_EXTERN_gBS;

UINT32                          mDgpuSsid;
UINT32                          mDgpuAudioSsid;

extern  AMD_CPM_TABLE_PROTOCOL              *gCpmTableSmmProtocol;
extern  AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *gCpmDisplayFeatureSmmProtocol;
AMD_CPM_DISPLAY_FEATURE_PROTOCOL            *gCpmDisplayFeatureRuntimeProtocol;

EFI_STATUS
EFIAPI
AmdCpmDisplayFeatureSmmReadyToLock (
  IN CONST EFI_GUID                *Protocol,
  IN VOID                          *Interface,
  IN EFI_HANDLE                    Handle
  )
{
  EFI_STATUS                              Status;
  CPM_DISPLAY_FEATURE_PRIVATE             *DisplayFeatureData;
  AMD_CPM_GETTABLEPTR_FN                  GetTablePtr;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  gCpmDisplayFeatureRuntimeProtocol = gCpmDisplayFeatureSmmProtocol;

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (AMD_CPM_DISPLAY_FEATURE_PROTOCOL),
                    (VOID **) &gCpmDisplayFeatureSmmProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-ERROR-End\n", __FUNCTION__));
    return Status;
  }

  //
  // clone Runtime Display Feature Protocol to SMM
  //
  gCpmDisplayFeatureSmmProtocol->Revision         = gCpmDisplayFeatureRuntimeProtocol->Revision;
  gCpmDisplayFeatureSmmProtocol->TableProtocolPtr = gCpmTableSmmProtocol;
  gCpmTableSmmProtocol->CommonFunction.CopyMem (
                                         &(gCpmDisplayFeatureSmmProtocol->DisplayFeatureData),
                                         &(gCpmDisplayFeatureRuntimeProtocol->DisplayFeatureData),
                                         sizeof (CPM_DISPLAY_FEATURE_PRIVATE)
                                         );

  DisplayFeatureData = &(gCpmDisplayFeatureSmmProtocol->DisplayFeatureData);

  GetTablePtr = gCpmTableSmmProtocol->CommonFunction.GetTablePtr;
  DisplayFeatureData->DisplayFeatureTablePtr = (VOID *)GetTablePtr (gCpmTableSmmProtocol, CPM_SIGNATURE_DISPLAY_FEATURE);
  DisplayFeatureData->DevicePathTablePtr     = (VOID *)GetTablePtr (gCpmTableSmmProtocol, CPM_SIGNATURE_DEVICE_PATH);
  DisplayFeatureData->SpecificSsidTablePtr   = (VOID *)GetTablePtr (gCpmTableSmmProtocol, CPM_SIGNATURE_SPECIFIC_SSID);
  DisplayFeatureData->RebrandDualGraphicsSsidTablePtr  = (VOID *)GetTablePtr (gCpmTableSmmProtocol, CPM_SIGNATURE_REBRAND_DUAL_GRAPHICS_SSID);

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

VOID
AmdCpmSaveGfxResource (
  IN        CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr,
  IN        UINT8                           i,
     OUT    AMD_CPM_PCI_BPMEM               *RootBridgeData,
     OUT    AMD_CPM_PCI_BPMEM               *UpStBridgeData,
     OUT    AMD_CPM_PCI_BPMEM               *DnStBridgeData,
     OUT    AMD_CPM_PCI_DPMEM               *DevicePciData
  )
{
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  RootBridgeData->BridgeCommand = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x04 );

  RootBridgeData->BusNumberReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x18 );

  RootBridgeData->BridgeNonPrefetchable = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x20 );

  RootBridgeData->LimitBaseLowReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x24 );

  RootBridgeData->BaseHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x28 );

  RootBridgeData->LimitHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x2C );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x18,
                    DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg );

  if (DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg != 0) {
    UpStBridgeData->BridgeCommand = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x04 );

    UpStBridgeData->BusNumberReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18 );

    UpStBridgeData->BridgeNonPrefetchable = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x20 );

    UpStBridgeData->LimitBaseLowReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x24 );

    UpStBridgeData->BaseHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x28 );

    UpStBridgeData->LimitHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x2C );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18,
                      DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg );
  }

  if ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg != 0) && (DisplayFeatureDataPtr->GfxBridgeDnSt[i].BusNumberReg != 0)){
    DnStBridgeData->BridgeCommand = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x04 );

    DnStBridgeData->BusNumberReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18 );

    DnStBridgeData->BridgeNonPrefetchable = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x20 );

    DnStBridgeData->LimitBaseLowReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x24 );

    DnStBridgeData->BaseHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x28 );

    DnStBridgeData->LimitHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x2C );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18,
                      DisplayFeatureDataPtr->GfxBridgeDnSt[i].BusNumberReg );
  }

  DevicePciData->DeviceIdReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x00 );

  DevicePciData->DeviceCommand = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x04 );

  DevicePciData->DoorbellLowReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x18 );

  DevicePciData->DoorbellHighReg = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x1C );

  DevicePciData->DeviceNonPrefetchable = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x24 );
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
}

VOID
AmdCpmProgramGfxResource (
  IN        CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr,
  IN        UINT8                           i
  )
{
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x18,
                    DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x20,
                    DisplayFeatureDataPtr->GfxBridgePMem[i].BridgeNonPrefetchable );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x24,
                    DisplayFeatureDataPtr->GfxBridgePMem[i].LimitBaseLowReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x28,
                    DisplayFeatureDataPtr->GfxBridgePMem[i].BaseHighReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x2C,
                    DisplayFeatureDataPtr->GfxBridgePMem[i].LimitHighReg );

  gCpmTableSmmProtocol->CommonFunction.PciOr8 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x04,
                    0x02 );

  if (DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg != 0) {
    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18,
                      DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x20,
                      DisplayFeatureDataPtr->GfxBridgeUpSt[i].BridgeNonPrefetchable );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x24,
                      DisplayFeatureDataPtr->GfxBridgeUpSt[i].LimitBaseLowReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x28,
                      DisplayFeatureDataPtr->GfxBridgeUpSt[i].BaseHighReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x2C,
                      DisplayFeatureDataPtr->GfxBridgeUpSt[i].LimitHighReg );

    gCpmTableSmmProtocol->CommonFunction.PciOr8 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x04,
                      0x02 );
  }

  if ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg != 0) && (DisplayFeatureDataPtr->GfxBridgeDnSt[i].BusNumberReg != 0)){
    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18,
                      DisplayFeatureDataPtr->GfxBridgeDnSt[i].BusNumberReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x20,
                      DisplayFeatureDataPtr->GfxBridgeDnSt[i].BridgeNonPrefetchable );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x24,
                      DisplayFeatureDataPtr->GfxBridgeDnSt[i].LimitBaseLowReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x28,
                      DisplayFeatureDataPtr->GfxBridgeDnSt[i].BaseHighReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x2C,
                      DisplayFeatureDataPtr->GfxBridgeDnSt[i].LimitHighReg );

    gCpmTableSmmProtocol->CommonFunction.PciOr8 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x04,
                      0x02 );
  }

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x18,
                    DisplayFeatureDataPtr->GfxDevicePMem[i].DoorbellLowReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x1C,
                    DisplayFeatureDataPtr->GfxDevicePMem[i].DoorbellHighReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x24,
                    DisplayFeatureDataPtr->GfxDevicePMem[i].DeviceNonPrefetchable );

  gCpmTableSmmProtocol->CommonFunction.PciOr8 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x04,
                    0x02 );
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
}

VOID
AmdCpmRestoreGfxResource (
  IN        CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr,
  IN        UINT8                           i,
     OUT    AMD_CPM_PCI_BPMEM               *RootBridgeData,
     OUT    AMD_CPM_PCI_BPMEM               *UpStBridgeData,
     OUT    AMD_CPM_PCI_BPMEM               *DnStBridgeData,
     OUT    AMD_CPM_PCI_DPMEM               *DevicePciData
  )
{
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x04,
                    DevicePciData->DeviceCommand );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x18,
                    DevicePciData->DoorbellLowReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x1C,
                    DevicePciData->DoorbellHighReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[i].Pfa.Function,
                    0x24,
                    DevicePciData->DeviceNonPrefetchable );

  if ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg != 0) && (DisplayFeatureDataPtr->GfxBridgeDnSt[i].BusNumberReg != 0)){
    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x04,
                      DnStBridgeData->BridgeCommand );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18,
                      DnStBridgeData->BusNumberReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x20,
                      DnStBridgeData->BridgeNonPrefetchable );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x24,
                      DnStBridgeData->LimitBaseLowReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x28,
                      DnStBridgeData->BaseHighReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x2C,
                      DnStBridgeData->LimitHighReg );
  }

  if (DisplayFeatureDataPtr->GfxBridgeUpSt[i].BusNumberReg != 0) {
    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x04,
                      UpStBridgeData->BridgeCommand );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x18,
                      UpStBridgeData->BusNumberReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x20,
                      UpStBridgeData->BridgeNonPrefetchable );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x24,
                      UpStBridgeData->LimitBaseLowReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x28,
                      UpStBridgeData->BaseHighReg );

    gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                      (UINT8) ((DisplayFeatureDataPtr->GfxBridgePMem[i].BusNumberReg >> 8) & 0xFF),
                      0,
                      0,
                      0x2C,
                      UpStBridgeData->LimitHighReg );
  }

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x04,
                    RootBridgeData->BridgeCommand );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x18,
                    RootBridgeData->BusNumberReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x20,
                    RootBridgeData->BridgeNonPrefetchable );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x24,
                    RootBridgeData->LimitBaseLowReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x28,
                    RootBridgeData->BaseHighReg );

  gCpmTableSmmProtocol->CommonFunction.PciWrite32 ( gCpmTableSmmProtocol,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Device,
                    DisplayFeatureDataPtr->GfxBridgePfa[i].Pfa.Function,
                    0x2C,
                    RootBridgeData->LimitHighReg );
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to disable audio in dGPU when PowerXpress is enabled and sets SSID
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmDisableDgpuAudioInPXSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  CPM_DISPLAY_FEATURE_PRIVATE *DisplayFeatureDataPtr;
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  UINT64                      Mmiobase;
  UINT32                      RomStrapOffset;
  UINT8                       DgpuCase;
  UINT32                      CurrentPowerXpress;
  UINT32                      DisableDgpuAudioInPX;
  UINT32                      IsDgpuPrimary;

  AMD_CPM_PCI_BPMEM           RootBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           UpStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           DnStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_DPMEM           DevicePciData  = {0, 0, 0, 0, 0};

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  DisplayFeatureDataPtr   = &gCpmDisplayFeatureSmmProtocol->DisplayFeatureData;
  MainTablePtr            = gCpmTableSmmProtocol->MainTablePtr;
  CurrentPowerXpress      = DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress;
  DisableDgpuAudioInPX    = MainTablePtr->DisplayFeature.Config.DisableDgpuAudioInPX;
  IsDgpuPrimary           = MainTablePtr->DisplayFeature.Config.IsDgpuPrimary;

  if (DisableDgpuAudioInPX) {
    if (DisplayFeatureDataPtr->GfxDevicePfa[1].Raw) {
      AmdCpmSaveGfxResource (DisplayFeatureDataPtr, 1, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
      AmdCpmProgramGfxResource (DisplayFeatureDataPtr, 1);

      if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x67C0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x67DF)) {
        DgpuCase = 0x10;      // Ellesmere
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x67E0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x67FF)) {
        DgpuCase = 0x11;      // Baffin
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6900) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x695F)) {
        DgpuCase = 0x12;      // Iceland(Weston), Tonga
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6980) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x699F)) {
        DgpuCase = 0x13;      // Lexa
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6860) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x687F)) {
        DgpuCase = 0x20;      // Greenland
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x69A0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x69BF)) {
        DgpuCase = 0x21;      // Vega12
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7310) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x731F)) {
        DgpuCase = 0x22;      // Navi10
      } else if (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) == 0x7330) {
        DgpuCase = 0x22;      // Navi10
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7340) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x734F)) {
        DgpuCase = 0x23;      // Navi14
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x73A0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x73FF)) {
        DgpuCase = 0x24;      // Navi21 / Navi22 / Navi23
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7420) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x743F)) {
        DgpuCase = 0x25;      // Navi24
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7440) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x749F)) {
        DgpuCase = 0x26;      // Navi31 / Navi32 / Navi33
      } else {
        DgpuCase = 0x0;       // Old GPU
      }

      if (DgpuCase >= 0x10) {
        // BAR5
        Mmiobase = (UINT64) gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x24 );
      } else {
        // BAR2
        Mmiobase = (UINT64) gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x1C );

        Mmiobase <<= 32;

        Mmiobase |= (UINT64) gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x18 );
      }

      if ((Mmiobase != 0xFFFFFFFFFFFFFFFF) && (Mmiobase != 0)) {
        if (DgpuCase >= 0x26) {
          // Bit28 for STRAP_FUNC_EN_DEV0_F1
          RomStrapOffset = 0x3514;
          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xEFFFFFFF );
        } else if (DgpuCase >= 0x24) {
          // Bit28 for STRAP_FUNC_EN_DEV0_F1
          RomStrapOffset = 0x34E8;
          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xEFFFFFFF );
        } else if (DgpuCase >= 0x20) {
          // Bit28 for STRAP_FUNC_EN_DEV0_F1
          RomStrapOffset = 0x34E0;
          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xEFFFFFFF );
        } else if (DgpuCase >= 0x10) {
          // Bit1 for STRAP_BIF_AUDIO_EN
          RomStrapOffset = 0x5418;
          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xFFFFFFFD );
        } else {
          // Bit12 for STRAP_BIF_AUDIO_EN
          RomStrapOffset = 0x540C;
          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xFFFFEFFF );
        }
      }

      AmdCpmRestoreGfxResource (DisplayFeatureDataPtr, 1, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
    }
  }
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to disable USB port in dGPU
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmDisableDgpuUsbPortSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  CPM_DISPLAY_FEATURE_PRIVATE *DisplayFeatureDataPtr;
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  UINT32                      DisableDgpuUsbPort;
  UINT32                      Mmiobase;
  UINT32                      RomStrapOffset;
  UINT8                       DgpuCase;
  UINT32                      UsbCommandReg;
  UINT16                      Retry;

  AMD_CPM_PCI_BPMEM           RootBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           UpStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           DnStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_DPMEM           DevicePciData  = {0, 0, 0, 0, 0};

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  DisplayFeatureDataPtr   = &gCpmDisplayFeatureSmmProtocol->DisplayFeatureData;
  MainTablePtr            = gCpmTableSmmProtocol->MainTablePtr;
  DisableDgpuUsbPort      = MainTablePtr->DisplayFeature.Config.DisableDgpuUsbPort;

  Mmiobase = 0;
  RomStrapOffset = 0x5818C;
  DgpuCase = 0;
  UsbCommandReg = 0;
  Retry = 0;

  if (DisableDgpuUsbPort) {
    if (DisplayFeatureDataPtr->GfxDevicePfa[1].Raw) {
      AmdCpmSaveGfxResource (DisplayFeatureDataPtr, 1, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
      AmdCpmProgramGfxResource (DisplayFeatureDataPtr, 1);
      if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x67C0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x67DF)) {
        DgpuCase = 0x10;      // Ellesmere
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x67E0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x67FF)) {
        DgpuCase = 0x11;      // Baffin
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6900) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x695F)) {
        DgpuCase = 0x12;      // Iceland(Weston), Tonga
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6980) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x699F)) {
        DgpuCase = 0x13;      // Lexa
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6860) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x687F)) {
        DgpuCase = 0x20;      // Greenland
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x69A0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x69BF)) {
        DgpuCase = 0x21;      // Vega12
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7310) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x731F)) {
        DgpuCase = 0x22;      // Navi10
      } else if (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) == 0x7330) {
        DgpuCase = 0x22;      // Navi10
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7340) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x734F)) {
        DgpuCase = 0x23;      // Navi14
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x73A0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x73FF)) {
        DgpuCase = 0x24;      // Navi21 / Navi22 / Navi23
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7420) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x743F)) {
        DgpuCase = 0x25;      // Navi24
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7440) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x749F)) {
        DgpuCase = 0x26;      // Navi31 / Navi32 / Navi33
      } else {
        DgpuCase = 0x0;       // Old GPU
      }

      if (DgpuCase >= 0x22) {
        // BAR5
        Mmiobase = gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x24 );

        if ((Mmiobase != 0xFFFFFFFF) && (Mmiobase != 0)) {
          // System BIOS polls on bit#31 of the Command register until it is 1, to ensure that
          // the security processor is ready to handle USB port disable command.
          Mmiobase &= 0xFFFFFFF0;
          Retry = 0;
          do {
            UsbCommandReg = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (Mmiobase + RomStrapOffset);
            Retry++;
            gCpmTableSmmProtocol->CommonFunction.Stall (gCpmTableSmmProtocol, 100000);
          } while (((UsbCommandReg & 0x80000000) != 0x80000000) && (Retry < RetryTimer));

          // System BIOS programs the Command register to a value of 0x01000000 to send a
          // command to security firmware to disable USB ports.
          gCpmTableSmmProtocol->CommonFunction.MmioWrite32 (Mmiobase + RomStrapOffset, 0x01000000);

          // System BIOS polls on bit#31 of the Command register until it is 1 to confirm that
          // the request is executed.
          Retry = 0;
          do {
            UsbCommandReg = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (Mmiobase + RomStrapOffset);
            Retry++;
            gCpmTableSmmProtocol->CommonFunction.Stall (gCpmTableSmmProtocol, 100000);
          } while (((UsbCommandReg & 0x80000000) != 0x80000000) && (Retry < RetryTimer));

          // System BIOS checks status return code in bits [15:0] of the Command register. A
          // value of 0 indicates successful response.
          if (((UsbCommandReg & 0x0000FFFF) == 0x00000000) && (Retry < RetryTimer)) {
            // System BIOS reads the Parameter register. A value of 1 indicates an external USB
            // port is present.
            UsbCommandReg = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (Mmiobase + RomStrapOffset + 4);
            if (UsbCommandReg == 1) {
              // System BIOS programs the Command register to a value of 0x00400000 to send a
              // command to security firmware to disable USB ports.
              gCpmTableSmmProtocol->CommonFunction.MmioWrite32 (Mmiobase + RomStrapOffset, 0x00400000);

              // System BIOS polls on bit#31 of the Command register until it is 1 to confirm that
              // the request is executed.
              Retry = 0;
              do {
                UsbCommandReg = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (Mmiobase + RomStrapOffset);
                Retry++;
                gCpmTableSmmProtocol->CommonFunction.Stall (gCpmTableSmmProtocol, 100000);
              } while (((UsbCommandReg & 0x80000000) != 0x80000000) && (Retry < RetryTimer));

              // System BIOS checks status return code in bits [15:0] of the Command register. A
              // value of 0 indicates successful response.
            } // End of if (UsbCommandReg == 1)
          } // End of if (((UsbCommandReg & 0x0000FFFF) == 0x00000000) && (Retry < RetryTimer))
        } // End of if ((Mmiobase != 0xFFFFFFFF) && (Mmiobase != 0))
      } // End of if (DgpuCase >= 0x22)
      AmdCpmRestoreGfxResource (DisplayFeatureDataPtr, 1, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
    } // End of if (DisplayFeatureDataPtr->GfxDevicePfa[1].Raw)
  } // End of if (DisableDgpuUsbPort)
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to set auto uma mode in smm
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmDisplayFeatureSetAutoUmaModeSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  EFI_STATUS                                Status;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  Status = UpdateAmdCbsUmaVersion ();

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to set UMA size ID in smm
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmDisplayFeatureSetUmaSizeIdSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  EFI_STATUS                                Status;
  AMD_CPM_MAIN_TABLE                        *MainTablePtr;
  UINT8                                     SmiData;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  MainTablePtr            = gCpmTableSmmProtocol->MainTablePtr;
  SmiData = gCpmTableSmmProtocol->CommonFunction.IoRead8 (MainTablePtr->SwSmiPort + 1);
  if (SmiData == 0) {
    return EFI_UNSUPPORTED;
  }
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-UMA Size ID: 0x%x\n", __FUNCTION__, SmiData));
  Status = UpdateAmdCbsUmaSizeId (SmiData);

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-%r\n", __FUNCTION__, Status));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to enable dGPU Smbus slave device when PowerXpress is enabled
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmEnableDgpuSmbusInPXSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  CPM_DISPLAY_FEATURE_PRIVATE *DisplayFeatureDataPtr;
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  UINT64                      Mmiobase;
  UINT32                      RomStrapOffset;
  UINT8                       DgpuCase;
  UINT32                      CurrentPowerXpress;
  UINT32                      CfgEnableDgpuSmbusInPX;
  UINT32                      IsDgpuPrimary;

  AMD_CPM_PCI_BPMEM           RootBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           UpStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           DnStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_DPMEM           DevicePciData  = {0, 0, 0, 0, 0};

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  DisplayFeatureDataPtr   = &gCpmDisplayFeatureSmmProtocol->DisplayFeatureData;
  MainTablePtr            = gCpmTableSmmProtocol->MainTablePtr;
  CurrentPowerXpress      = DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress;
  CfgEnableDgpuSmbusInPX  = MainTablePtr->DisplayFeature.Config.EnableDgpuSmbusInPX;
  IsDgpuPrimary           = MainTablePtr->DisplayFeature.Config.IsDgpuPrimary;

  if (CfgEnableDgpuSmbusInPX && !IsDgpuPrimary) {
    if (DisplayFeatureDataPtr->GfxDevicePfa[1].Raw) {
      AmdCpmSaveGfxResource (DisplayFeatureDataPtr, 1, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
      AmdCpmProgramGfxResource (DisplayFeatureDataPtr, 1);

      if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x67C0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x67DF)) {
        DgpuCase = 0x10;      // Ellesmere
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x67E0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x67FF)) {
        DgpuCase = 0x11;      // Baffin
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6900) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x695F)) {
        DgpuCase = 0x12;      // Iceland(Weston), Tonga
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6980) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x699F)) {
        DgpuCase = 0x13;      // Lexa
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x6860) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x687F)) {
        DgpuCase = 0x20;      // Greenland
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x69A0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x69BF)) {
        DgpuCase = 0x21;      // Vega12
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7310) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x731F)) {
        DgpuCase = 0x22;      // Navi10
      } else if (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) == 0x7330) {
        DgpuCase = 0x22;      // Navi10
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7340) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x734F)) {
        DgpuCase = 0x23;      // Navi14
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x73A0) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x73FF)) {
        DgpuCase = 0x24;      // Navi21 / Navi22 / Navi23
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7420) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x743F)) {
        DgpuCase = 0x25;      // Navi24
      } else if ((((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) >= 0x7440) && (((DevicePciData.DeviceIdReg >> 16) & 0xFFFF) <= 0x749F)) {
        DgpuCase = 0x26;      // Navi31 / Navi32 / Navi33
      } else {
        DgpuCase = 0x0;       // Old GPU
      }

      if (DgpuCase >= 0x10) {
        // BAR5
        Mmiobase = (UINT64) gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x24 );
      } else {
        // BAR2
        Mmiobase = (UINT64) gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x1C );

        Mmiobase <<= 32;

        Mmiobase |= (UINT64) gCpmTableSmmProtocol->CommonFunction.PciRead32 ( gCpmTableSmmProtocol,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                          DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                          0x18 );
      }

      if ((Mmiobase != 0xFFFFFFFFFFFFFFFF) && (Mmiobase != 0)) {
        if (DgpuCase >= 0x20) {
          // Bit8 for SMBUS_ADDR
//          RomStrapOffset = 0x5A208;
//          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xFFFFFEFF );
        } else if (DgpuCase >= 0x10) {
          // Bit1 for STRAP_BIF_SMBUS_DIS
          RomStrapOffset = 0x5780;
          gCpmTableSmmProtocol->CommonFunction.MmioAnd32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xFFFFFFFD );
        } else {
          // Bit0 for STRAP_BIF_SMB_EN
          // Bit[7:1] for STRAP_BIF_SMB_SLV_ADR
          RomStrapOffset = 0x53F4;
          gCpmTableSmmProtocol->CommonFunction.MmioAndThenOr32 ( (Mmiobase & 0xFFFFFFFFFFFFFFF0) + RomStrapOffset, 0xFFFFFF00, 0x00000083 );
        }
      }

      AmdCpmRestoreGfxResource (DisplayFeatureDataPtr, 1, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
    }
  }
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to set Sub-System ID
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmSetGpuSsidSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  AMD_CPM_DISPLAY_FEATURE_TABLE   *DisplayFeatureTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE       *DevicePathTablePtr;
  AMD_CPM_SPECIFIC_SSID_TABLE     *SpecificSsidTablePtr;
  AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE  *RebrandDualGraphicsSsidTablePtr;
  AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE2 *RebrandDualGraphicsSsidTable2Ptr;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Func;
  UINT16                          VendorId;
  UINT16                          VendorId2;
  UINT16                          DeviceId;
  UINT16                          DeviceId2;
  UINT8                           Index;
  UINT32                          Ssid;
  UINT32                          NewSsid;
  UINT16                          i;
  CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr;
  UINT8                           RevId;
  UINT16                          iSsid;
  UINT32                          RebrandDualGraphics;
  UINT8                           FunctionDisableMask;

  AMD_CPM_PCI_BPMEM           RootBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           UpStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_BPMEM           DnStBridgeData = {0, 0, 0, 0, 0, 0};
  AMD_CPM_PCI_DPMEM           DevicePciData  = {0, 0, 0, 0, 0};

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  DisplayFeatureDataPtr   = &gCpmDisplayFeatureSmmProtocol->DisplayFeatureData;
  MainTablePtr              = gCpmTableSmmProtocol->MainTablePtr;
  RebrandDualGraphics       = MainTablePtr->DisplayFeature.Config.RebrandDualGraphics;
  DisplayFeatureTablePtr    = DisplayFeatureDataPtr->DisplayFeatureTablePtr;
  DevicePathTablePtr        = DisplayFeatureDataPtr->DevicePathTablePtr;
  SpecificSsidTablePtr      = DisplayFeatureDataPtr->SpecificSsidTablePtr;
  RebrandDualGraphicsSsidTablePtr   = DisplayFeatureDataPtr->RebrandDualGraphicsSsidTablePtr;
  RebrandDualGraphicsSsidTable2Ptr  = DisplayFeatureDataPtr->RebrandDualGraphicsSsidTablePtr;

  if (DisplayFeatureTablePtr) {

    FunctionDisableMask = DisplayFeatureTablePtr->FunctionDisableMask;
    if (RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr) {
      if (RebrandDualGraphicsSsidTablePtr->Header.FormatRevision != 0) {
        FunctionDisableMask |= BIT0;
      }
    }

    for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
      if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
        break;
      }
      if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu && DevicePathTablePtr->Path[Index].IsDgpu) {
        Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
        Dev       = DevicePathTablePtr->Path[Index].Device.Device;
        Func      = DevicePathTablePtr->Path[Index].Device.Function;
        DeviceId  = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x02);
        DeviceId2 = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func + 1, 0x02);
        if ((mDgpuSsid != 0) && (DeviceId != 0xFFFF) && DisplayFeatureTablePtr->DgpuSsidSvid) {
          gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x4C, mDgpuSsid);
        }
        if ((mDgpuAudioSsid != 0) && (DeviceId2 != 0xFFFF) && DisplayFeatureTablePtr->DgpuSsidSvid) {
          gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func + 1, 0x4C, mDgpuAudioSsid);
        }
      } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu && DevicePathTablePtr->Path[Index].IsDgpu)
    } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)

    if (!(FunctionDisableMask & BIT0)) {

      Ssid = DisplayFeatureDataPtr->Ssid;

      for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
        if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu) {
          if (Index < 3) {
            AmdCpmSaveGfxResource (DisplayFeatureDataPtr, Index, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
            AmdCpmProgramGfxResource (DisplayFeatureDataPtr, Index);
          }
          Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
          Dev       = DevicePathTablePtr->Path[Index].Device.Device;
          Func      = DevicePathTablePtr->Path[Index].Device.Function;
          VendorId  = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x00);
          DeviceId  = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x02);
          DeviceId2 = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func + 1, 0x02);
          NewSsid   = Ssid | (UINT32)VendorId;
          if (SpecificSsidTablePtr) {
            for (i = 0; i < AMD_SPECIFIC_SSID_DEVICE_SIZE; i++) {
              if (SpecificSsidTablePtr->Item[i].VendorId == 0xFFFF) {
                break;
              } else {
                if ((SpecificSsidTablePtr->Item[i].VendorId == VendorId) && (SpecificSsidTablePtr->Item[i].DeviceId == DeviceId) && DisplayFeatureTablePtr->DgpuSsidSvid) {
                  gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x4C, NewSsid);
                }
                if ((DeviceId2 != 0xFFFF) && (SpecificSsidTablePtr->Item[i].VendorId == VendorId) && (SpecificSsidTablePtr->Item[i].DeviceId == DeviceId2) && DisplayFeatureTablePtr->DgpuSsidSvid) {
                  gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func + 1, 0x4C, NewSsid);
                }
              }
            }
          } else if ((mDgpuSsid == 0) && DisplayFeatureTablePtr->DgpuSsidSvid) {
            gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x4C, NewSsid);
          }
          if (Index < 3) {
            AmdCpmRestoreGfxResource (DisplayFeatureDataPtr, Index, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
          }
        } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu)
      }
    }

    if (RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr) {
      Ssid = DisplayFeatureDataPtr->Ssid2;
      switch (RebrandDualGraphicsSsidTablePtr->Header.FormatRevision) {
      case 0:
        if (Ssid != 0xFFFF0000) {
          for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
            if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
              break;
            }
            if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu) {
              if (Index < 3) {
                AmdCpmSaveGfxResource (DisplayFeatureDataPtr, Index, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
                AmdCpmProgramGfxResource (DisplayFeatureDataPtr, Index);
              }
              Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
              Dev       = DevicePathTablePtr->Path[Index].Device.Device;
              Func      = DevicePathTablePtr->Path[Index].Device.Function;
              VendorId  = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x00);
              VendorId2 = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x2C);
              DeviceId  = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x02);
              NewSsid   = Ssid | (UINT32)VendorId2;

              for (i = 0; i < AMD_REBRAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE; i++) {
                if (RebrandDualGraphicsSsidTablePtr->Item[i].VendorId == 0xFFFF) {
                  break;
                } else {
                  if ((RebrandDualGraphicsSsidTablePtr->Item[i].VendorId == VendorId) &&
                      (RebrandDualGraphicsSsidTablePtr->Item[i].DeviceId == DeviceId) &&
                      (RebrandDualGraphicsSsidTablePtr->Item[i].IsDgpu == 1) &&
                      DisplayFeatureTablePtr->DgpuSsidSvid) {
                    gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x4C, NewSsid);
                    break;
                  }
                }
              }
              if (Index < 3) {
                AmdCpmRestoreGfxResource (DisplayFeatureDataPtr, Index, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
              }
            }
          }
        } // End of if (Ssid != 0xFFFF0000)
        // End of case 0
        break;

      case 1:
        if (Ssid != 0xFFFF0000) {
          iSsid = (UINT16) (Ssid >> 16);
          for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
            if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
              break;
            }
            if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu) {
              if (Index < 3) {
                AmdCpmSaveGfxResource (DisplayFeatureDataPtr, Index, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
                AmdCpmProgramGfxResource (DisplayFeatureDataPtr, Index);
              }
              Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
              Dev       = DevicePathTablePtr->Path[Index].Device.Device;
              Func      = DevicePathTablePtr->Path[Index].Device.Function;
              DeviceId  = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x02);
              VendorId2 = gCpmTableSmmProtocol->CommonFunction.PciRead16 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x2C);
              RevId     = gCpmTableSmmProtocol->CommonFunction.PciRead8 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x08);
              if (RebrandDualGraphicsSsidTable2Ptr->Svid == 0 || RebrandDualGraphicsSsidTable2Ptr->Svid == 0xFFFF) {
                NewSsid   = Ssid | (UINT32)VendorId2;
              } else {
                NewSsid   = Ssid | (UINT32)RebrandDualGraphicsSsidTable2Ptr->Svid;
              }

              for (i = 0; i < AMD_REBRAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE; i++) {
                if (RebrandDualGraphicsSsidTable2Ptr->Item[i].dDeviceId == 0xFFFF) {
                  break;
                } else {
                  if ((RebrandDualGraphicsSsidTable2Ptr->Item[i].dRevId == RevId) &&
                      (RebrandDualGraphicsSsidTable2Ptr->Item[i].iSsid == iSsid) &&
                      (RebrandDualGraphicsSsidTable2Ptr->Item[i].dDeviceId == DeviceId) &&
                      DisplayFeatureTablePtr->DgpuSsidSvid) {
                    gCpmTableSmmProtocol->CommonFunction.PciWrite32 (gCpmTableSmmProtocol, Bus, Dev, Func, 0x4C, NewSsid);
                    break;
                  }
                }
              }
              if (Index < 3) {
                AmdCpmRestoreGfxResource (DisplayFeatureDataPtr, Index, &RootBridgeData, &UpStBridgeData, &DnStBridgeData, &DevicePciData);
              }
            }
          }
        } // End of if (Ssid != 0xFFFF0000)
        // End of case 1
        break;

      } // End of Switch
    } // End of if (RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr)
  } // End of if (DisplayFeatureTablePtr)
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to set GPIO for display output
 *
 * @param[in]     DispatchHandle  The handle of this callback, obtained when registering
 * @param[in]     DispatchContext Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
 *
 * @retval        EFI_SUCCESS     Callback function successfully
 * @retval        EFI_ERROR       Callback function failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmSetGpuDisplayOutputSmm (
  IN        EFI_HANDLE                      DispatchHandle,
  IN        CPM_SMM_SW_DISPATCH_CONTEXT     *DispatchContext
  )
{
  CPM_DISPLAY_FEATURE_PRIVATE   *DisplayFeatureDataPtr;
  AMD_CPM_MAIN_TABLE            *MainTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE     *DevicePathTablePtr;
  AMD_CPM_GPIO_DEVICE_POWER_TABLE *GpioDevicePowerTablePtr;
  UINT8                         i;
  UINT8                         GpioDeviceId;
  UINT8                         DeviceId;
  UINT8                         Mode;
  UINT8                         Index;
  UINT32                        IsDgpuPrimary;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  DisplayFeatureDataPtr   = &gCpmDisplayFeatureSmmProtocol->DisplayFeatureData;
  MainTablePtr            = gCpmTableSmmProtocol->MainTablePtr;
  IsDgpuPrimary           = MainTablePtr->DisplayFeature.Config.IsDgpuPrimary;
  DevicePathTablePtr      = DisplayFeatureDataPtr->DevicePathTablePtr;
  GpioDevicePowerTablePtr = gCpmTableSmmProtocol->CommonFunction.GetTablePtr (gCpmTableSmmProtocol, CPM_SIGNATURE_GPIO_DEVICE_POWER);

  if (GpioDevicePowerTablePtr && DevicePathTablePtr) {
    for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
      if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
        break;
      }
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist == 1) {
        if (DevicePathTablePtr->Path[i].IsDgpu == IsDgpuPrimary) {
          DeviceId  = DevicePathTablePtr->Path[i].DeviceId;
          Mode      = DevicePathTablePtr->Path[i].Mode;
          if (DeviceId != 0xFF && DeviceId != 0x00) {
            for (Index = 0; Index < AMD_GPIO_DEVICE_POWER_SIZE; Index++) {
              GpioDeviceId = GpioDevicePowerTablePtr->DevicePowerList[Index].DeviceId;
              if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
                break;
              }
              if (GpioDeviceId == DeviceId &&
                  GpioDevicePowerTablePtr->DevicePowerList[Index].Mode == Mode &&
                  GpioDevicePowerTablePtr->DevicePowerList[Index].Type == 0) {
                gCpmTableSmmProtocol->CommonFunction.SetGpio ( gCpmTableSmmProtocol,
                                                              ((UINT32) GpioDevicePowerTablePtr->DevicePowerList[Index].Config.SetGpio.Misc << 24) + (UINT32) GpioDevicePowerTablePtr->DevicePowerList[Index].Config.SetGpio.Pin,
                                                              GpioDevicePowerTablePtr->DevicePowerList[Index].Config.SetGpio.Value );
              }
            }
          }
          break;
        }
      }
    }
  }
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMI Handler to restore display feature
 *
 * This function disables audio in dGPU when PowerXpress is enabled and sets SSID
 * when resume from S3.
 *
 */

EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AmdCpmDisplayFeatureRestoreSmm,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  EFI_STATUS                                         Status;
  AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_ID                 SyncId;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  SyncId = AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_DISPLAY_FEATURE_RESTORE_SMM;
  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableFromRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  AmdCpmDisableDgpuAudioInPXSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);
  AmdCpmDisableDgpuUsbPortSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);
  AmdCpmEnableDgpuSmbusInPXSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);
  AmdCpmSetGpuSsidSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);
  AmdCpmSetGpuDisplayOutputSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);


  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableToRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * SMI Handler to Disable Dgpu Audio
 *
 * This function disables audio in dGPU when PX/HG is enabled
 *
 */

EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AmdCpmDisableDgpuAudioSwSmiSmm,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  EFI_STATUS                                         Status;
  AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_ID                 SyncId;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  SyncId = AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_DISABLE_DGPU_AUDIO_SW_SMI_SMM;
  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableFromRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  AmdCpmDisableDgpuAudioInPXSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);
  AmdCpmSetGpuSsidSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);

  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableToRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * SMI Handler to Set Auto Uma Mode
 *
 * This function Set Auto Uma Mode when ATCS Function 7 is issued by driver
 *
 */

EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AmdCpmDisplayFeatureSetAutoUmaModeCallBackSmm,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  EFI_STATUS                                         Status;
  AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_ID                 SyncId;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  SyncId =   AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_DISPLAY_FEATURE_SET_AUTO_UMA_MODE_CALLBACK_SMM;
  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableFromRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  AmdCpmDisplayFeatureSetAutoUmaModeSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);

  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableToRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMI Handler to Set UMA Size ID
 *
 * This function Set Uma Size ID when ATCS Function 10 is issued by driver
 *
 */

EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AmdCpmDisplayFeatureSetUmaSizeIdCallBackSmm,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  EFI_STATUS                                         Status;
  AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_ID                 SyncId;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  SyncId =   AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_DISPLAY_FEATURE_SET_UMA_SIZE_ID_CALLBACK_SMM;
  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableFromRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  AmdCpmDisplayFeatureSetUmaSizeIdSmm (DispatchHandle, (CPM_SMM_SW_DISPATCH_CONTEXT*)DispatchContext);

  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableToRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }


  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * AMD CPM Display Feature SMM Register Center
 *
 * This function registers the SMI handler to disable audio in dGPU when PowerXpress is enabled
 * and set SSID when resume from S3.
 *
 * @retval        EFI_SUCCESS    Function initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */


EFI_STATUS
AmdCpmDisplayFeatureSmmRegistrationCenter (
  VOID
  )
{
  EFI_STATUS                          Status;
  CPM_SMM_SW_DISPATCH_PROTOCOL        *SwDispatch;
  CPM_SMM_SW_DISPATCH_CONTEXT         SwContext;
  EFI_HANDLE                          SwHandle;
  AMD_CPM_MAIN_TABLE                  *MainTablePtr;
  UINT8                               CpuRevisionId;
  VOID                                *Registration;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  MainTablePtr        = gCpmTableSmmProtocol->MainTablePtr;
  CpuRevisionId       = MainTablePtr->CpmCpuRevisionId;

  mDgpuSsid      = PcdGet32 (PcdCpmDgpuSsid);
  mDgpuAudioSsid = PcdGet32 (PcdCpmDgpuAudioSsid);

  //
  //  Locate SMM SW dispatch protocol
  //
  Status = CpmSmmLocateProtocol ( &gCpmSmmSwDispatchProtocolGuid,
                                  NULL,
                                  (VOID**)&SwDispatch );
  ASSERT_EFI_ERROR (Status);

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-ModernStandbyEn: %d\n",
    __FUNCTION__, gCpmTableSmmProtocol->MainTablePtr->ModernStandbyEn));

  if (gCpmTableSmmProtocol->MainTablePtr->ModernStandbyEn == 0) {
    SwContext.SwSmiInputValue = CFG_CPM_ACPI_RESTORE_SW_SMI;
    Status = SwDispatch->Register ( SwDispatch,
                                    AmdCpmDisplayFeatureRestoreSmm,
                                    &SwContext,
                                    &SwHandle );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-1-EFI_ERROR\n", __FUNCTION__));
      return Status;
    }
  }

  SwContext.SwSmiInputValue = CFG_CPM_ACPI_DISABLE_DGPU_AUDIO_SW_SMI;
  Status = SwDispatch->Register ( SwDispatch,
                                  AmdCpmDisableDgpuAudioSwSmiSmm,
                                  &SwContext,
                                  &SwHandle );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-2-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  if ((CpuRevisionId == CPM_CPU_REVISION_ID_CZ) || (CpuRevisionId == CPM_CPU_REVISION_ID_ST) || \
      (CpuRevisionId == CPM_CPU_REVISION_ID_BR) || (CpuRevisionId == CPM_CPU_REVISION_ID_RV) || \
      (CpuRevisionId == CPM_CPU_REVISION_ID_RV2)) {
    SwContext.SwSmiInputValue = CFG_CPM_SWSMICMD_SETAUTOUMAMODE;
    Status = SwDispatch->Register ( SwDispatch,
                                    AmdCpmDisplayFeatureSetAutoUmaModeCallBackSmm,
                                    &SwContext,
                                    &SwHandle );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-3-EFI_ERROR\n", __FUNCTION__));
      return Status;
    }
  }

  SwContext.SwSmiInputValue = CFG_CPM_SWSMICMD_SETUMASIZEID;
  Status = SwDispatch->Register ( SwDispatch,
                                  AmdCpmDisplayFeatureSetUmaSizeIdCallBackSmm,
                                  &SwContext,
                                  &SwHandle );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-4-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmReadyToLockProtocolGuid,
                    AmdCpmDisplayFeatureSmmReadyToLock,
                    &Registration
                    );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-5-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-6-EFI_SUCCESS\n", __FUNCTION__));
  return EFI_SUCCESS;
}
