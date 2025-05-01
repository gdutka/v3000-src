/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include "AmdCpmPciHotPlugInitDxe.h"

//
// Instantiation of Driver private data. Size = 0x16
//
PCIE_HOT_PLUG_DEVICE_PATH DefaultGppDp [1] = {
  {
    ACPI(0x0),
    PCI(1, 1),
    END
  }
};

//
// Instantiation of Driver private data. Size = 0x1C
//
PCIE_HOT_PLUG_DEVICE_PATH_SWUS DefaultSwUsDp [1] = {
  {
    ACPI(0x0),
    PCI(1, 1),
    PCI(0, 0),
    END
  }
};

//
// Instantiation of Driver private data. Size = 0x22
//
PCIE_HOT_PLUG_DEVICE_PATH_SWDS DefaultSwDsDp [1] = {
  {
    ACPI(0x0),
    PCI(1, 1),
    PCI(0, 0),
    PCI(0, 0),
    END
  }
};

UINTN                       NumberOfHotplugBridge = 0;
EFI_HPC_LOCATION            DefaultHpcLocation [AMD_RES_RES_SIZE];
EFI_HPC_LOCATION            *HpcLocationTbl;
HOT_PLUG_BRIDGE_INFO_NEW    HotPlugBridgeInfo [AMD_RES_RES_SIZE];
HOT_PLUG_BRIDGE_INFO_NEW    *HotPlugBridgeInfoTable;

VOID
AmdCpmGetHotplugBridgeInfo (
  OUT HOT_PLUG_BRIDGE_INFO_NEW       **GetHotPlugBridgeInfoTable
  )
{

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  *GetHotPlugBridgeInfoTable = HotPlugBridgeInfo;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
  return;
}

EFI_STATUS
EFIAPI
AmdCpmPciHotPlugInitEntryPoint (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                    Status;
  AMD_CPM_TABLE_PROTOCOL        *CpmTableProtocolPtr;
  AMD_CPM_PCI_RES_RES_TABLE     *PciResResTablePtr;
  AMD_CPM_THUNDERBOLT_TABLE     *ThunderboltTablePtr;
  AMD_CPM_CORE_TOPOLOGY_TABLE   *AmdCpmCoreTopologyPtr;
  UINT8                         i;
  UINT8                         j;
  UINT8                         SocketDieBridge = 0;
  UINT8                         Size = 0;
  UINT8                         MultiBridgeNum = 0;
  UINT64                        TempLowMmio;
  UINT64                        TempHihgMmio;
  VOID                          *TempDpTbl;
  PCI_HOT_PLUG_INSTANCE         *PciHotPlug;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));

  if (!PcdGetBool (PcdAmdCpmPciHotPlugSupport)) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-PcdAmdCpmPciHotPlugSupport = FALSE-End\n", __FUNCTION__, __LINE__));
    return EFI_SUCCESS;
  }

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-LocateProtocol-gAmdCpmTableProtocolGuid\n", __FUNCTION__));
  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**) &CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-End-0-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpPciHotPlugDxeEntryBegin);
  PciResResTablePtr     = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCI_RESERVED_RESOURCE);
  ThunderboltTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_THUNDERBOLT);
  AmdCpmCoreTopologyPtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_CORE_TOPOLOGY);

  if (PciResResTablePtr) {
    DEBUG ((DEBUG_INFO, "\nFEA-DXE-%a-PCI Reserved Resource Table\n", __FUNCTION__));
    for (i = 0; i < AMD_RES_RES_SIZE; i++) {
      if (PciResResTablePtr->ResRes[i].DevType == CPM_END_OF_TABLE) {
        break;
      }
      if (ThunderboltTablePtr) {
        if ((ThunderboltTablePtr->TBSupport == 0) && ((PciResResTablePtr->ResRes[i].DevType & DEVICE_MASK) == TBT3_GPP)) {
          PciResResTablePtr->ResRes[i].DevType = DEVICE_DISABLE;
        }
        if (ThunderboltTablePtr->TBSupport && ((PciResResTablePtr->ResRes[i].DevType & DEVICE_MASK) == TBT3_GPP)) {
          if ((PciResResTablePtr->ResRes[i].SocketDieBridge != ThunderboltTablePtr->SocketDieBridge) ||
              (PciResResTablePtr->ResRes[i].Gpp.Device      != ThunderboltTablePtr->Device) ||
              (PciResResTablePtr->ResRes[i].Gpp.Function    != ThunderboltTablePtr->Function)) {
            PciResResTablePtr->ResRes[i].SocketDieBridge = ThunderboltTablePtr->SocketDieBridge;
            PciResResTablePtr->ResRes[i].Gpp.Device      = ThunderboltTablePtr->Device;
            PciResResTablePtr->ResRes[i].Gpp.Function    = ThunderboltTablePtr->Function;
          }
          if (ThunderboltTablePtr->ResourceSize) {
            TempLowMmio = PciResResTablePtr->ResRes[i].ResNonPrefetchableMmio & 0x3FFFFF;
            TempHihgMmio = (PciResResTablePtr->ResRes[i].ResNonPrefetchableMmio & ~(UINT64)0x3FFFFF) >> 1;
            PciResResTablePtr->ResRes[i].ResNonPrefetchableMmio = TempHihgMmio + TempLowMmio;
            PciResResTablePtr->ResRes[i].ResPrefetchableMmio >>= 1;
          }
        }
      } else {
        if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_MASK) == TBT3_GPP) {
          PciResResTablePtr->ResRes[i].DevType = DEVICE_DISABLE;
        }
      }
      DEBUG ((DEBUG_INFO, "  { 0x%02X, 0x%02X,", PciResResTablePtr->ResRes[i].DevType, PciResResTablePtr->ResRes[i].SocketDieBridge));
      DEBUG ((DEBUG_INFO, " {%d, %d},", PciResResTablePtr->ResRes[i].Gpp.Device, PciResResTablePtr->ResRes[i].Gpp.Function));
      DEBUG ((DEBUG_INFO, " {%d, %d},", PciResResTablePtr->ResRes[i].SwUs.Device, PciResResTablePtr->ResRes[i].SwUs.Function));
      DEBUG ((DEBUG_INFO, " {%d, %d},", PciResResTablePtr->ResRes[i].SwDs.Device, PciResResTablePtr->ResRes[i].SwDs.Function));
      DEBUG ((DEBUG_INFO, " 0x%02X, 0x%04X,", PciResResTablePtr->ResRes[i].ResBus, PciResResTablePtr->ResRes[i].ResIo));
      DEBUG ((DEBUG_INFO, " 0x%016LX, 0x%016LX,", PciResResTablePtr->ResRes[i].ResNonPrefetchableMmio, PciResResTablePtr->ResRes[i].AlignResNonPrefetch));
      DEBUG ((DEBUG_INFO, " 0x%016LX, 0x%016LX }\n", PciResResTablePtr->ResRes[i].ResPrefetchableMmio, PciResResTablePtr->ResRes[i].AlignResPrefetch));
    }
    DEBUG ((DEBUG_INFO, "\n"));

    if (AmdCpmCoreTopologyPtr) {
      for (MultiBridgeNum = 0; MultiBridgeNum < AMD_CPM_CORE_TOPOLOGY_SIZE; MultiBridgeNum++) {
        if (AmdCpmCoreTopologyPtr->CoreList[MultiBridgeNum].Socket == CPM_END_OF_TABLE) {
          break;
        }
      }
    }
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-MultiBridgeNum = %d\n", __FUNCTION__, MultiBridgeNum));

    AmdCpmGetHotplugBridgeInfo (&HotPlugBridgeInfoTable);

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ZeroMem-HotPlugBridgeInfo-Address = 0x%p, Size = 0x%X\n", __FUNCTION__, HotPlugBridgeInfo, sizeof (HotPlugBridgeInfo)));
    ZeroMem (HotPlugBridgeInfo, sizeof (HotPlugBridgeInfo));

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ZeroMem-DefaultHpcLocation-Address = 0x%p, Size = 0x%X\n", __FUNCTION__, DefaultHpcLocation, sizeof (DefaultHpcLocation)));
    ZeroMem (DefaultHpcLocation, sizeof (DefaultHpcLocation));

    //
    // Calculate how many hot plug bridge we have.
    //
    DEBUG ((DEBUG_INFO, "\nFEA-DXE-%a-HotPlugBridgeInfoTable\n", __FUNCTION__));
    for (i = 0, j = 0; i < AMD_RES_RES_SIZE; i++) {
      DEBUG ((DEBUG_INFO, "\n  i = %d, j = %d\n", i, j));
      if (PciResResTablePtr->ResRes[i].DevType == CPM_END_OF_TABLE) {
        HotPlugBridgeInfoTable[j].NodeUID = CPM_END_OF_TABLE;
        if (i != j) {
          PciResResTablePtr->ResRes[j].DevType = CPM_END_OF_TABLE;
        }
        break;
      }

      if (PciResResTablePtr->ResRes[i].DevType == DEVICE_DISABLE) {
        continue;
      }

      if (MultiBridgeNum > 1) {
        SocketDieBridge = PciResResTablePtr->ResRes[i].SocketDieBridge;
        if (SocketDieBridge < MultiBridgeNum) {
          HotPlugBridgeInfoTable[j].NodeUID                             = MultiBridgeNum - 1 - SocketDieBridge;
          HotPlugBridgeInfoTable[j].BusNum                              = AmdCpmCoreTopologyPtr->CoreList[SocketDieBridge].Bus;
        }
      }
      HotPlugBridgeInfoTable[j].BusNum                                  += (PciResResTablePtr->ResRes[i].DevType & 0x0F);
      if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_DEPTH_MASK) == 0x00) {
        HotPlugBridgeInfoTable[j].DevNum                                = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Device;
        HotPlugBridgeInfoTable[j].FuncNum                               = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Function;
        Size = sizeof (PCIE_HOT_PLUG_DEVICE_PATH);
      } else if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_DEPTH_MASK) == 0x01) {
        HotPlugBridgeInfoTable[j].DevNum                                = (UINT8) PciResResTablePtr->ResRes[i].SwUs.Device;
        HotPlugBridgeInfoTable[j].FuncNum                               = (UINT8) PciResResTablePtr->ResRes[i].SwUs.Function;
        Size = sizeof (PCIE_HOT_PLUG_DEVICE_PATH_SWUS);
      } else if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_DEPTH_MASK) == 0x02) {
        HotPlugBridgeInfoTable[j].DevNum                                = (UINT8) PciResResTablePtr->ResRes[i].SwDs.Device;
        HotPlugBridgeInfoTable[j].FuncNum                               = (UINT8) PciResResTablePtr->ResRes[i].SwDs.Function;
        Size = sizeof (PCIE_HOT_PLUG_DEVICE_PATH_SWDS);
      }
      HotPlugBridgeInfoTable[j].ReservedBusCount                        = PciResResTablePtr->ResRes[i].ResBus;
      HotPlugBridgeInfoTable[j].ReservedIoRange                         = PciResResTablePtr->ResRes[i].ResIo;
      HotPlugBridgeInfoTable[j].ReservedNonPrefetchableMmio             = PciResResTablePtr->ResRes[i].ResNonPrefetchableMmio;
      HotPlugBridgeInfoTable[j].AlignemntOfReservedNonPrefetchableMmio  = PciResResTablePtr->ResRes[i].AlignResNonPrefetch;
      HotPlugBridgeInfoTable[j].ReservedPrefetchableMmio                = PciResResTablePtr->ResRes[i].ResPrefetchableMmio;
      HotPlugBridgeInfoTable[j].AlignemntOfReservedPrefetchableMmio     = PciResResTablePtr->ResRes[i].AlignResPrefetch;

      DEBUG ((DEBUG_INFO, "  { 0x%02X, 0x%02X,", HotPlugBridgeInfoTable[j].NodeUID, HotPlugBridgeInfoTable[j].BusNum));
      DEBUG ((DEBUG_INFO, " {%d, %d},", HotPlugBridgeInfoTable[j].DevNum, HotPlugBridgeInfoTable[j].FuncNum));
      DEBUG ((DEBUG_INFO, " 0x%02X, 0x%04X,", HotPlugBridgeInfoTable[j].ReservedBusCount, HotPlugBridgeInfoTable[j].ReservedIoRange));
      DEBUG ((DEBUG_INFO, " 0x%016LX, 0x%016LX,", HotPlugBridgeInfoTable[j].ReservedNonPrefetchableMmio, HotPlugBridgeInfoTable[j].AlignemntOfReservedNonPrefetchableMmio));
      DEBUG ((DEBUG_INFO, " 0x%016LX, 0x%016LX }\n", HotPlugBridgeInfoTable[j].ReservedPrefetchableMmio, HotPlugBridgeInfoTable[j].AlignemntOfReservedPrefetchableMmio));

      DEBUG ((DEBUG_INFO, "FEA-DXE-%a-AllocatePool-TempDpTbl-Size = 0x%X", __FUNCTION__, Size));
      Status = gBS->AllocatePool (EfiBootServicesData, Size, &TempDpTbl);
      if (EFI_ERROR (Status)) {
        //
        // Fail to allocate memory for table, treat as no hot plug bridge.
        //
        DEBUG ((DEBUG_INFO, "\nFEA-DXE-%a(%d)-End-1-Status = %r\n", __FUNCTION__, __LINE__, Status));
        CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpPciHotPlugDxeEntryEnd);
        return EFI_SUCCESS;
      }
      DEBUG ((DEBUG_INFO, ", Address = 0x%p\n", TempDpTbl));

      DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ZeroMem-TempDpTbl-Address = 0x%p, Size = 0x%X\n", __FUNCTION__, TempDpTbl, Size));
      ZeroMem (TempDpTbl, Size);

      if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_DEPTH_MASK) == 0x00) {
        DefaultGppDp[0].PciRootBridgeNode.UID     = (UINT32) HotPlugBridgeInfoTable[j].NodeUID;
        DefaultGppDp[0].PciRootPortNode.Device    = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Device;
        DefaultGppDp[0].PciRootPortNode.Function  = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Function;

        DEBUG ((DEBUG_INFO, "  GPP  PciRootBridgeNode  { 0x%02X, 0x%02X,", DefaultGppDp[0].PciRootBridgeNode.Header.Type, DefaultGppDp[0].PciRootBridgeNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultGppDp[0].PciRootBridgeNode.Header.Length[0], DefaultGppDp[0].PciRootBridgeNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%08X, 0x%08X },\n", DefaultGppDp[0].PciRootBridgeNode.HID, DefaultGppDp[0].PciRootBridgeNode.UID));
        DEBUG ((DEBUG_INFO, "  GPP  PciRootPortNode    { 0x%02X, 0x%02X,", DefaultGppDp[0].PciRootPortNode.Header.Type, DefaultGppDp[0].PciRootPortNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultGppDp[0].PciRootPortNode.Header.Length[0], DefaultGppDp[0].PciRootPortNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultGppDp[0].PciRootPortNode.Device, DefaultGppDp[0].PciRootPortNode.Function));
        DEBUG ((DEBUG_INFO, "  GPP  EndDeviceNode      { 0x%02X, 0x%02X,", DefaultGppDp[0].EndDeviceNode.Type, DefaultGppDp[0].EndDeviceNode.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultGppDp[0].EndDeviceNode.Length[0], DefaultGppDp[0].EndDeviceNode.Length[1]));

        CpmTableProtocolPtr->CommonFunction.CopyMem (TempDpTbl, &DefaultGppDp[0], Size);

      } else if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_DEPTH_MASK) == 0x01) {
        DefaultSwUsDp[0].PciRootBridgeNode.UID    = (UINT32) HotPlugBridgeInfoTable[j].NodeUID;
        DefaultSwUsDp[0].PciRootPortNode.Device   = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Device;
        DefaultSwUsDp[0].PciRootPortNode.Function = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Function;
        DefaultSwUsDp[0].PciEpSwUsNode.Device     = (UINT8) PciResResTablePtr->ResRes[i].SwUs.Device;
        DefaultSwUsDp[0].PciEpSwUsNode.Function   = (UINT8) PciResResTablePtr->ResRes[i].SwUs.Function;

        DEBUG ((DEBUG_INFO, "  SWUS PciRootBridgeNode  { 0x%02X, 0x%02X,", DefaultSwUsDp[0].PciRootBridgeNode.Header.Type, DefaultSwUsDp[0].PciRootBridgeNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwUsDp[0].PciRootBridgeNode.Header.Length[0], DefaultSwUsDp[0].PciRootBridgeNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%08X, 0x%08X },\n", DefaultSwUsDp[0].PciRootBridgeNode.HID, DefaultSwUsDp[0].PciRootBridgeNode.UID));
        DEBUG ((DEBUG_INFO, "  SWUS PciRootPortNode    { 0x%02X, 0x%02X,", DefaultSwUsDp[0].PciRootPortNode.Header.Type, DefaultSwUsDp[0].PciRootPortNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwUsDp[0].PciRootPortNode.Header.Length[0], DefaultSwUsDp[0].PciRootPortNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultSwUsDp[0].PciRootPortNode.Device, DefaultSwUsDp[0].PciRootPortNode.Function));
        DEBUG ((DEBUG_INFO, "  SWUS PciEpSwUsNode      { 0x%02X, 0x%02X,", DefaultSwUsDp[0].PciEpSwUsNode.Header.Type, DefaultSwUsDp[0].PciEpSwUsNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwUsDp[0].PciEpSwUsNode.Header.Length[0], DefaultSwUsDp[0].PciEpSwUsNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultSwUsDp[0].PciEpSwUsNode.Device, DefaultSwUsDp[0].PciEpSwUsNode.Function));
        DEBUG ((DEBUG_INFO, "  SWUS EndDeviceNode      { 0x%02X, 0x%02X,", DefaultSwUsDp[0].EndDeviceNode.Type, DefaultSwUsDp[0].EndDeviceNode.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n\n", DefaultSwUsDp[0].EndDeviceNode.Length[0], DefaultSwUsDp[0].EndDeviceNode.Length[1]));

        CpmTableProtocolPtr->CommonFunction.CopyMem (TempDpTbl, &DefaultSwUsDp[0], Size);

      } else if ((PciResResTablePtr->ResRes[i].DevType & DEVICE_DEPTH_MASK) == 0x02) {
        DefaultSwDsDp[0].PciRootBridgeNode.UID    = (UINT32) HotPlugBridgeInfoTable[j].NodeUID;
        DefaultSwDsDp[0].PciRootPortNode.Device   = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Device;
        DefaultSwDsDp[0].PciRootPortNode.Function = (UINT8) PciResResTablePtr->ResRes[i].Gpp.Function;
        DefaultSwDsDp[0].PciEpSwUsNode.Device     = (UINT8) PciResResTablePtr->ResRes[i].SwUs.Device;
        DefaultSwDsDp[0].PciEpSwUsNode.Function   = (UINT8) PciResResTablePtr->ResRes[i].SwUs.Function;
        DefaultSwDsDp[0].PciEpSwDsNode.Device     = (UINT8) PciResResTablePtr->ResRes[i].SwDs.Device;
        DefaultSwDsDp[0].PciEpSwDsNode.Function   = (UINT8) PciResResTablePtr->ResRes[i].SwDs.Function;

        DEBUG ((DEBUG_INFO, "  SWDS PciRootBridgeNode  { 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciRootBridgeNode.Header.Type, DefaultSwDsDp[0].PciRootBridgeNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciRootBridgeNode.Header.Length[0], DefaultSwDsDp[0].PciRootBridgeNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%08X, 0x%08X },\n", DefaultSwDsDp[0].PciRootBridgeNode.HID, DefaultSwDsDp[0].PciRootBridgeNode.UID));
        DEBUG ((DEBUG_INFO, "  SWDS PciRootPortNode    { 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciRootPortNode.Header.Type, DefaultSwDsDp[0].PciRootPortNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciRootPortNode.Header.Length[0], DefaultSwDsDp[0].PciRootPortNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultSwDsDp[0].PciRootPortNode.Device, DefaultSwDsDp[0].PciRootPortNode.Function));
        DEBUG ((DEBUG_INFO, "  SWDS PciEpSwUsNode      { 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciEpSwUsNode.Header.Type, DefaultSwDsDp[0].PciEpSwUsNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciEpSwUsNode.Header.Length[0], DefaultSwDsDp[0].PciEpSwUsNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultSwDsDp[0].PciEpSwUsNode.Device, DefaultSwDsDp[0].PciEpSwUsNode.Function));
        DEBUG ((DEBUG_INFO, "  SWDS PciEpSwDsNode      { 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciEpSwDsNode.Header.Type, DefaultSwDsDp[0].PciEpSwDsNode.Header.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X,", DefaultSwDsDp[0].PciEpSwDsNode.Header.Length[0], DefaultSwDsDp[0].PciEpSwDsNode.Header.Length[1]));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n", DefaultSwDsDp[0].PciEpSwDsNode.Device, DefaultSwDsDp[0].PciEpSwDsNode.Function));
        DEBUG ((DEBUG_INFO, "  SWDS EndDeviceNode      { 0x%02X, 0x%02X,", DefaultSwDsDp[0].EndDeviceNode.Type, DefaultSwDsDp[0].EndDeviceNode.SubType));
        DEBUG ((DEBUG_INFO, " 0x%02X, 0x%02X },\n\n", DefaultSwDsDp[0].EndDeviceNode.Length[0], DefaultSwDsDp[0].EndDeviceNode.Length[1]));

        CpmTableProtocolPtr->CommonFunction.CopyMem (TempDpTbl, &DefaultSwDsDp[0], Size);
      }

      DEBUG ((DEBUG_INFO, "FEA-DXE-%a-DefaultHpcLocation[%d].HpcDevicePath & HpbDevicePath = 0x%p\n", __FUNCTION__, j, TempDpTbl));
      DefaultHpcLocation[j].HpcDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) TempDpTbl;
      DefaultHpcLocation[j].HpbDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) TempDpTbl;

      if (i != j) {
        PciResResTablePtr->ResRes[j].DevType = PciResResTablePtr->ResRes[i].DevType;
        PciResResTablePtr->ResRes[j].SocketDieBridge = PciResResTablePtr->ResRes[i].SocketDieBridge;
        PciResResTablePtr->ResRes[j].Gpp.Device = PciResResTablePtr->ResRes[i].Gpp.Device;
        PciResResTablePtr->ResRes[j].Gpp.Function = PciResResTablePtr->ResRes[i].Gpp.Function;
        PciResResTablePtr->ResRes[j].SwUs.Device = PciResResTablePtr->ResRes[i].SwUs.Device;
        PciResResTablePtr->ResRes[j].SwUs.Function = PciResResTablePtr->ResRes[i].SwUs.Function;
        PciResResTablePtr->ResRes[j].SwDs.Device = PciResResTablePtr->ResRes[i].SwDs.Device;
        PciResResTablePtr->ResRes[j].SwDs.Function = PciResResTablePtr->ResRes[i].SwDs.Function;
        PciResResTablePtr->ResRes[j].ResBus = PciResResTablePtr->ResRes[i].ResBus;
        PciResResTablePtr->ResRes[j].ResIo = PciResResTablePtr->ResRes[i].ResIo;
        PciResResTablePtr->ResRes[j].ResNonPrefetchableMmio = PciResResTablePtr->ResRes[i].ResNonPrefetchableMmio;
        PciResResTablePtr->ResRes[j].AlignResNonPrefetch = PciResResTablePtr->ResRes[i].AlignResNonPrefetch;
        PciResResTablePtr->ResRes[j].ResPrefetchableMmio = PciResResTablePtr->ResRes[i].ResPrefetchableMmio;
        PciResResTablePtr->ResRes[j].AlignResPrefetch = PciResResTablePtr->ResRes[i].AlignResPrefetch;
      }
      j++;
    }

    for (i = 0; i < j; i++) {
      DEBUG ((DEBUG_INFO, "  DefaultHpcLocation[%d].HpcDevicePath = 0x%08X, HpbDevicePath = 0x%08X\n", i, DefaultHpcLocation[i].HpcDevicePath, DefaultHpcLocation[i].HpbDevicePath));
    }

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-NumberOfHotplugBridge = 0x%X\n", __FUNCTION__, j));
    NumberOfHotplugBridge = j;

    //
    // Prepare HotPlugBridgeInfoTable
    //
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-AllocatePool-HpcLocationTbl-Size = 0x%X", __FUNCTION__, (sizeof (EFI_HPC_LOCATION) * j)));
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    (sizeof (EFI_HPC_LOCATION) * j),
                    (VOID **)&HpcLocationTbl
                    );
    if (EFI_ERROR (Status)) {
      //
      // Fail to allocate memory for table, treat as no hot plug bridge.
      //
      DEBUG ((DEBUG_INFO, "\nFEA-DXE-%a(%d)-End-2-Status = %r\n", __FUNCTION__, __LINE__, Status));
      CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpPciHotPlugDxeEntryEnd);
      return EFI_SUCCESS;
    }
    DEBUG ((DEBUG_INFO, ", Address = 0x%p\n", HpcLocationTbl));

    //
    // Initial Table default
    //
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ZeroMem-HpcLocationTbl-Address = 0x%p, Size = 0x%X\n", __FUNCTION__, HpcLocationTbl, (sizeof (EFI_HPC_LOCATION) * j)));
    ZeroMem (HpcLocationTbl, (sizeof (EFI_HPC_LOCATION) * j));

    for (i = 0; i < j; i++) {
      CpmTableProtocolPtr->CommonFunction.CopyMem ((VOID *) &HpcLocationTbl[i].HpcDevicePath, (VOID *) &DefaultHpcLocation[i].HpcDevicePath, sizeof (EFI_HPC_LOCATION));
    }

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-AllocatePool-PciHotPlug-Size = 0x%X", __FUNCTION__, sizeof (PCI_HOT_PLUG_INSTANCE)));
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (PCI_HOT_PLUG_INSTANCE),
                    (VOID **)&PciHotPlug
                    );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, ", Address = 0x%p\n", PciHotPlug));

    //
    // Initialize driver private data.
    //
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ZeroMem-PciHotPlug-Address = 0x%p, Size = 0x%X\n", __FUNCTION__, PciHotPlug, sizeof (PCI_HOT_PLUG_INSTANCE)));
    ZeroMem (PciHotPlug, sizeof (PCI_HOT_PLUG_INSTANCE));

    PciHotPlug->Signature = EFI_PCIHOTPLUG_DRIVER_PRIVATE_SIGNATURE;
    PciHotPlug->HotPlugInitProtocol.GetRootHpcList      = AmdCpmGetRootHpcList;
    PciHotPlug->HotPlugInitProtocol.InitializeRootHpc   = AmdCpmInitializeRootHpc;
    PciHotPlug->HotPlugInitProtocol.GetResourcePadding  = AmdCpmGetResourcePadding;

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-InstallProtocolInterface-gEfiPciHotPlugInitProtocolGuid\n", __FUNCTION__));
    Status = gBS->InstallProtocolInterface (
                    &PciHotPlug->Handle,
                    &gEfiPciHotPlugInitProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &PciHotPlug->HotPlugInitProtocol
                    );
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-End-3-Status = %r\n", __FUNCTION__, __LINE__, Status));
  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpPciHotPlugDxeEntryEnd);
  return EFI_SUCCESS;
}

/**
  This procedure returns a list of Root Hot Plug controllers that require
  initialization during boot process

  @param[in] This            The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[in] HpcCount        The number of Root HPCs returned.
  @param[in] HpcList         The list of Root HPCs. HpcCount defines the number of elements in this list.

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
AmdCpmGetRootHpcList (
  IN EFI_PCI_HOT_PLUG_INIT_PROTOCOL    *This,
  OUT UINTN                            *HpcCount,
  OUT EFI_HPC_LOCATION                 **HpcList
  )
{
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));

  *HpcCount = (UINTN) NumberOfHotplugBridge;
  *HpcList  = HpcLocationTbl;
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-HpcCount %X\n", __FUNCTION__, NumberOfHotplugBridge));

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-End-EFI_SUCCESS\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  This procedure Initializes one Root Hot Plug Controller
  This process may casue initialization of its subordinate buses

  @param[in] This            The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[in] HpcDevicePath   The Device Path to the HPC that is being initialized.
  @param[in] HpcPciAddress   The address of the Hot Plug Controller function on the PCI bus.
  @param[in] Event           The event that should be signaled when the Hot Plug Controller initialization is complete.
                             Set to NULL if the caller wants to wait until the entire initialization process is complete.
                             The event must be of the type EFI_EVT_SIGNAL.
  @param[in] HpcState        The state of the Hot Plug Controller hardware. The type EFI_Hpc_STATE is defined in section 3.1.

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
AmdCpmInitializeRootHpc (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL      *This,
  IN  EFI_DEVICE_PATH_PROTOCOL            *HpcDevicePath,
  IN  UINT64                              HpcPciAddress,
  IN  EFI_EVENT                           Event, OPTIONAL
  OUT EFI_HPC_STATE                       *HpcState
  )
{
  EFI_STATUS                        Status;
  UINT8                             Index;
  AMD_CPM_TABLE_PROTOCOL            *CpmTableProtocolPtr = NULL;
  AMD_CPM_THUNDERBOLT_TABLE         *ThunderboltTablePtr = NULL;
  UINT32                            Address;
  UINT32                            VidDid;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Start-HpcDevicePath = 0x%p, HpcPciAddress = 0x%LX\n", __FUNCTION__, HpcDevicePath, HpcPciAddress));

  for (Index = 0; Index < NumberOfHotplugBridge; Index++) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-HpcLocationTbl[%d].HpcDevicePath = 0x%LX\n", __FUNCTION__, Index, HpcLocationTbl[Index].HpcDevicePath));
    if (HpcLocationTbl[Index].HpcDevicePath == HpcDevicePath) {
      break;
    }
  }

  if ((Index < NumberOfHotplugBridge) && ((HpcPciAddress >> 24) != HotPlugBridgeInfoTable[Index].BusNum)) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-HPC Bus number is not the same\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Original HotPlugBridgeInfoTable[%d].BusNum = %X\n", __FUNCTION__, Index, HotPlugBridgeInfoTable[Index].BusNum));
    HotPlugBridgeInfoTable[Index].BusNum = (UINT8) (HpcPciAddress >> 24);
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Override HotPlugBridgeInfoTable[%d].BusNum = %X\n", __FUNCTION__, Index, HotPlugBridgeInfoTable[Index].BusNum));

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-LocateProtocol-gAmdCpmTableProtocolGuid\n", __FUNCTION__));
    Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                    NULL,
                                    (VOID**) &CpmTableProtocolPtr
                                    );
    ASSERT_EFI_ERROR (Status);

    ThunderboltTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_THUNDERBOLT);

    if (ThunderboltTablePtr) {
      if (ThunderboltTablePtr->TBSupport && (ThunderboltTablePtr->RootPortBus != (HotPlugBridgeInfoTable[Index].BusNum - 1))) {
        Address = CpmTableProtocolPtr->MainTablePtr->PcieMemIoBaseAddr + (HotPlugBridgeInfoTable[Index].BusNum << 20);
        VidDid = CpmTableProtocolPtr->CommonFunction.MmioRead32 (Address + 0x00);   // Check Vendor ID & Device ID
        if ((VidDid & 0xFF00FFFF) == 0x15008086) {
          // TBT3 AR/TR HR PCIe bridge
          ThunderboltTablePtr->RootPortBus = HotPlugBridgeInfoTable[Index].BusNum - 1;
          DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Override ThunderboltTablePtr->RootPortBus to 0x%X\n", __FUNCTION__, ThunderboltTablePtr->RootPortBus));
        }
      }
    }
  }

  if (Event) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-if (Event)\n", __FUNCTION__));
    gBS->SignalEvent (Event);
  }

  *HpcState = EFI_HPC_STATE_INITIALIZED;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-End-EFI_SUCCESS\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Returns the resource padding required by the PCI bus that is controlled by the specified Hot Plug Controller.

  @param[in] This               The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol. initialized.
  @param[in] HpcDevicePath      The Device Path to the Hot Plug Controller.
  @param[in] HpcPciAddress      The address of the Hot Plug Controller function on the PCI bus.
  @param[in] HpcState           The state of the Hot Plug Controller hardware. The type EFI_HPC_STATE is defined in section 3.1.
  @param[in] Padding            This is the amount of resource padding required by the PCI bus under the control of the specified Hpc.
                                Since the caller does not know the size of this buffer, this buffer is allocated by the callee and freed by the caller.
  @param[in] Attribute          Describes how padding is accounted for.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AmdCpmGetResourcePadding (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN  UINT64                          HpcPciAddress,
  OUT EFI_HPC_STATE                   *HpcState,
  OUT VOID                            **Padding,
  OUT EFI_HPC_PADDING_ATTRIBUTES      *Attributes
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *PaddingResource;
  EFI_STATUS                        Status;
  UINT64                            PciAddress;
  UINTN                             Index;
  UINTN                             Size;
  BOOLEAN                           FindHotPlugBridgeInfo;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Start-HpcPciAddress 0x%LX\n", __FUNCTION__, HpcPciAddress));
  Index = 0;
  FindHotPlugBridgeInfo = FALSE;
  while (HotPlugBridgeInfoTable[Index].NodeUID != CPM_END_OF_TABLE) {
    PciAddress = EFI_PCI_ADDRESS (HotPlugBridgeInfoTable[Index].BusNum, HotPlugBridgeInfoTable[Index].DevNum, HotPlugBridgeInfoTable[Index].FuncNum, 0);
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-Index %X PciAddress 0x%LX\n", __FUNCTION__, Index, PciAddress));
    if (HpcPciAddress == PciAddress) {
      DEBUG ((DEBUG_INFO, "FEA-DXE-%a-HpcPciAddress == PciAddress\n", __FUNCTION__));
      FindHotPlugBridgeInfo = TRUE;
      break;
    }
    Index++;
  }

  if (!FindHotPlugBridgeInfo) {
    Size = sizeof (EFI_ACPI_END_TAG_DESCRIPTOR);
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-AllocatePool-PaddingResource-Size = 0x%X\n", __FUNCTION__, Size));
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    Size,
                    (VOID **)&PaddingResource
                    );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, ", Address = 0x%p\n", PaddingResource));

    *Padding = (VOID *) PaddingResource;

    ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Desc     = ACPI_END_TAG_DESCRIPTOR;
    ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Checksum = 0x0;

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-if (!FindHotPlugBridgeInfo)\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-End-1-Status = EFI_SUCCESS\n", __FUNCTION__, __LINE__));
    return EFI_SUCCESS;
  }

  Size = 4 * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR);
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-AllocatePool-PaddingResource-Size = 0x%X", __FUNCTION__, Size));
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  Size,
                  (VOID **)&PaddingResource
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, ", Address = 0x%p\n", PaddingResource));

  *Padding = (VOID *) PaddingResource;

  //
  // Padding for bus
  //
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ZeroMem-PaddingResource-Address = 0x%p, Size = 0x%X\n", __FUNCTION__, PaddingResource, Size));
  ZeroMem (PaddingResource, Size);
  *Attributes                   = EfiPaddingPciBus;

  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_BUS;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrRangeMax = 0;
  PaddingResource->AddrLen      = HotPlugBridgeInfoTable[Index].ReservedBusCount - 1;
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_BUS AddrLen 0x%LX\n", __FUNCTION__, PaddingResource->AddrLen));
  //
  // Pad 0 Bus
  //
  // Padding for non-prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
  PaddingResource->AddrSpaceGranularity = 32;
  PaddingResource->SpecificFlag         = 0;
  //
  // Pad non-prefetchable
  //
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = HotPlugBridgeInfoTable[Index].ReservedNonPrefetchableMmio;
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_MEM (non-prefetchable) AddrLen 0x%LX\n", __FUNCTION__, PaddingResource->AddrLen));
  //
  // Force minimum 4KByte alignment for Virtualization technology for Directed I/O
  //
  if (PaddingResource->AddrLen < SIZE_4KB) {
    PaddingResource->AddrRangeMax = SIZE_4KB - 1;
  } else if (PaddingResource->AddrLen > SIZE_16MB) {
    PaddingResource->AddrRangeMax = HotPlugBridgeInfoTable[Index].AlignemntOfReservedNonPrefetchableMmio - 1;
  } else  {
    PaddingResource->AddrRangeMax = 1;
  }
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_MEM (non-prefetchable) AddrRangeMax 0x%LX\n", __FUNCTION__, PaddingResource->AddrRangeMax));
  //
  // Padding for prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
  PaddingResource->AddrSpaceGranularity = 64;
  PaddingResource->SpecificFlag         = 06;
  //
  // Pad prefetchable
  //
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = HotPlugBridgeInfoTable[Index].ReservedPrefetchableMmio;
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_MEM (prefetchable) AddrLen 0x%LX\n", __FUNCTION__, PaddingResource->AddrLen));
  //
  // Force minimum 4KByte alignment for Virtualization technology for Directed I/O
  //
  if (PaddingResource->AddrLen < SIZE_4KB) {
    PaddingResource->AddrRangeMax = SIZE_4KB - 1;
  } else if (PaddingResource->AddrLen > SIZE_16MB) {
    PaddingResource->AddrRangeMax = HotPlugBridgeInfoTable[Index].AlignemntOfReservedPrefetchableMmio - 1;
  } else  {
    PaddingResource->AddrRangeMax = 1;
  }
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_MEM (prefetchable) AddrRangeMax 0x%LX\n", __FUNCTION__, PaddingResource->AddrRangeMax));
  //
  // Padding for I/O
  //
  PaddingResource++;
  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_IO;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = HotPlugBridgeInfoTable[Index].ReservedIoRange;
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_IO AddrLen 0x%LX\n", __FUNCTION__, PaddingResource->AddrLen));
  //
  // Pad 4K of IO
  //
  PaddingResource->AddrRangeMax = HotPlugBridgeInfoTable[Index].ReservedIoRange / SIZE_4KB;
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ACPI_ADDRESS_SPACE_TYPE_IO AddrRangeMax 0x%LX\n", __FUNCTION__, PaddingResource->AddrRangeMax));
  //
  // Alignment
  //
  // Terminate the entries.
  //
  PaddingResource++;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Desc     = ACPI_END_TAG_DESCRIPTOR;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Checksum = 0x0;

  *HpcState = EFI_HPC_STATE_INITIALIZED | EFI_HPC_STATE_ENABLED;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-End-3-Status = EFI_SUCCESS\n", __FUNCTION__, __LINE__));
  return EFI_SUCCESS;
}

