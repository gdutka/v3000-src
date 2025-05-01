/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>

VOID
AmdCpmDisplayFeatureInitLate (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr
  );

BOOLEAN
EFIAPI
AmdCpmSmartMuxSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmIGpuSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmDGpuSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmDGpuD3ColdCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmMxmOverTSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmDisplayConnectEventSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * AmdCpmDisplayFeatureInitLateEntry
 *
 * This function is called as part of CPM DXE Driver Initialization. It gets called
 * each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmDisplayFeatureInitLateEntry (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  STATIC BOOLEAN  AmdCpmDisplayFeatureInitLateEntryInvoked = FALSE;

  RECORD_TIME_DXE (BeginAmdCpmDisplayFeatureLateInit);

  if (!AmdCpmDisplayFeatureInitLateEntryInvoked) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
    AmdCpmDisplayFeatureInitLate (Context);
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
  }

  AmdCpmDisplayFeatureInitLateEntryInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmDisplayFeatureLateInit);
}


/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to set Sub-System Id on iGPU and dGPU, update NV Data for
 * Display Feature and install Display Feature SSDT tables for iGPU and dGPU.
 *
 * @param[in]     CpmDisplayFeatureProtocolPtr  Pointer to AMD CPM Display Feature Protocol
 *
 */

VOID
AmdCpmDisplayFeatureInitLate (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr
  )
{
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_NV_DATA_PROTOCOL        *pAmdCpmNvDataProtocol;
  AMD_CPM_DISPLAY_FEATURE_TABLE   *DisplayFeatureTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE       *DevicePathTablePtr;
  AMD_CPM_SPECIFIC_SSID_TABLE     *SpecificSsidTablePtr;
  AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE  *RebrandDualGraphicsSsidTablePtr;
  AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE2 *RebrandDualGraphicsSsidTable2Ptr;
  AMD_CPM_PCIE_TOPOLOGY_TABLE     *PcieTopologyTablePtr;
  AMD_CPM_DXIO_TOPOLOGY_TABLE     *DxioTopologyTablePtr;
  PCIe_PORT_DESCRIPTOR            *PciePortDescriptorPtr;
  DXIO_PORT_DESCRIPTOR            *DxioPortDescriptorPtr;
  AMD_CPM_GFX_DETECT              GfxDetect;
  AMD_CPM_EXTERNAL_GFX_TABLE      ExtGfxTable;
  EFI_STATUS                      Status;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Func;
  UINT16                          VendorId;
  UINT16                          VendorId2;
  UINT16                          DeviceId;
  UINT16                          DeviceId2;
  UINT32                          Index;
  AMD_CPM_NV_DATA_STRUCT          *eNvStore;
  UINT32                          VbiosAddr;
  UINT32                          VbiosSize;
  UINT32                          Ssid;
  UINT32                          NewSsid;
  UINT32                          DgpuSsid;
  UINT32                          DgpuAudioSsid;
  UINT16                          j;
  UINT8                           i;
  UINT8                           iGpuIndex;
  UINT8                           dGpuIndex;
  UINT32                          Buffer[36];
  CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr;
  UINT8                           SciMap;
  UINT8                           RevId;
  UINT16                          iSsid;
  UINT32                          PowerXpressFixedMode;
  UINT32                          PowerXpressDynamicMode;
  UINT32                          HyperCrossFire;
  UINT32                          ForceAtpxAtifInDgpu;
  UINT32                          IsDgpuPrimary;
  UINT32                          IsBrightnessByDriver;
  UINT32                          DisableDgpuAudioInPX;
  UINT32                          DualGraphicsNotSupported;
  UINT32                          PulseGeneratorSupport;
  UINT32                          RebrandDualGraphics;
  UINT32                          FullPciEmulationSupport;
  UINT32                          DetachableGraphicsSupport;
  UINT32                          D3ColdSupport;
  UINT32                          HybridGraphicsSupport;
  UINT32                          CurrentPowerXpress;
  UINT32                          CurrentHyperCrossFire;
  UINT32                          CurrentHybridGraphics;
  UINT64                          OemTableIdSig;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  Status        = EFI_SUCCESS;
  Bus           = 0;
  Dev           = 0;
  Func          = 0;
  VendorId      = 0;
  DeviceId      = 0;
  eNvStore      = 0;
  DgpuSsid      = PcdGet32 (PcdCpmDgpuSsid);
  DgpuAudioSsid = PcdGet32 (PcdCpmDgpuAudioSsid);

  CpmTableProtocolPtr = CpmDisplayFeatureProtocolPtr->TableProtocolPtr;
  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpDisplayFeatureDxeInitLateBegin);

  Status = gBS->LocateProtocol (  &gAmdCpmNvDataProtocolGuid,
                                  NULL,
                                  (VOID**)&pAmdCpmNvDataProtocol );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-1-EFI_ERROR\n", __FUNCTION__));
    return;
  }

  eNvStore                  = pAmdCpmNvDataProtocol->NvDataPtr;
  PcieTopologyTablePtr      = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_TOPOLOGY);
  DxioTopologyTablePtr      = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DXIO_TOPOLOGY);

  DisplayFeatureDataPtr     = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;
  MainTablePtr              = CpmTableProtocolPtr->MainTablePtr;
  PowerXpressFixedMode      = MainTablePtr->DisplayFeature.Config.PowerXpressFixedMode;
  PowerXpressDynamicMode    = MainTablePtr->DisplayFeature.Config.PowerXpressDynamicMode;
  HyperCrossFire            = MainTablePtr->DisplayFeature.Config.HyperCrossFire;
  IsBrightnessByDriver      = MainTablePtr->DisplayFeature.Config.IsBrightnessByDriver;
  DisableDgpuAudioInPX      = MainTablePtr->DisplayFeature.Config.DisableDgpuAudioInPX;
  DualGraphicsNotSupported  = MainTablePtr->DisplayFeature.Config.DualGraphicsNotSupported;
  PulseGeneratorSupport     = MainTablePtr->DisplayFeature.Config.PulseGeneratorSupport;
  RebrandDualGraphics       = MainTablePtr->DisplayFeature.Config.RebrandDualGraphics;
  FullPciEmulationSupport   = MainTablePtr->DisplayFeature.Config.FullPciEmulationSupport;
  DetachableGraphicsSupport = MainTablePtr->DisplayFeature.Config.DetachableGraphicsSupport;
  D3ColdSupport             = MainTablePtr->DisplayFeature.Config.D3ColdSupport;
  HybridGraphicsSupport     = MainTablePtr->DisplayFeature.Config.HybridGraphicsSupport;
  DisplayFeatureTablePtr    = DisplayFeatureDataPtr->DisplayFeatureTablePtr;
  DevicePathTablePtr        = DisplayFeatureDataPtr->DevicePathTablePtr;
  SpecificSsidTablePtr      = DisplayFeatureDataPtr->SpecificSsidTablePtr;
  RebrandDualGraphicsSsidTablePtr   = DisplayFeatureDataPtr->RebrandDualGraphicsSsidTablePtr;
  RebrandDualGraphicsSsidTable2Ptr  = DisplayFeatureDataPtr->RebrandDualGraphicsSsidTablePtr;
  CurrentPowerXpress        = DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress;
  CurrentHyperCrossFire     = DisplayFeatureDataPtr->CurrentFeature.Mask.HyperCrossFire;
  CurrentHybridGraphics     = DisplayFeatureDataPtr->CurrentFeature.Mask.HybridGraphics;

  if ((DisplayFeatureTablePtr) && (DevicePathTablePtr)) {
    DisplayFeatureTablePtr->PcdSetS0i3PmeTurnOffDelay = PcdGet32 (PcdSetS0i3PmeTurnOffDelay);
    if (RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr) {
      if (RebrandDualGraphicsSsidTablePtr->Header.FormatRevision != 0) {
        DisplayFeatureTablePtr->FunctionDisableMask |= BIT0;
      }
    }

    VbiosAddr = DisplayFeatureDataPtr->VBiosImage;
    if (VbiosAddr != 0xFFFFFFFF && VbiosAddr != 0) {
      VbiosSize = (*((UINT8*) (EFI_PHYSICAL_ADDRESS) VbiosAddr + 2)) << 9;
      eNvStore->CpmAtrmRomSize = (UINT32)VbiosSize;
      if (VbiosSize != 0 && VbiosSize <= 0x10000) {
        CpmTableProtocolPtr->CommonFunction.CopyMem ( &eNvStore->CpmAtrmRomImage[0],
                                                      (VOID *) (EFI_PHYSICAL_ADDRESS)VbiosAddr,
                                                      (UINTN)VbiosSize );
      }
    }

    eNvStore->CpmDisplayFeatureConfig = MainTablePtr->DisplayFeature.Raw;

    if (DisplayFeatureTablePtr->AtifFun21Support != 0) {
      ExtGfxTable.NumDevice        = 0;
      ExtGfxTable.DeviceStructSize = 4;
      for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
        if (DevicePathTablePtr->Path[i].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[i].IsDgpu && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
          ExtGfxTable.Device[ExtGfxTable.NumDevice].Flags = (UINT16) (DevicePathTablePtr->Path[i].FeatureMask.Mask.ExternalGfxPort | \
                                                            (DevicePathTablePtr->Path[i].FeatureMask.Mask.HideXConnectGui << 1) | \
                                                            (DevicePathTablePtr->Path[i].FeatureMask.Mask.RunTimePMandD3 << 2) | \
                                                            (DevicePathTablePtr->Path[i].FeatureMask.Mask.SupportAtifAtpx << 3));
          ExtGfxTable.Device[ExtGfxTable.NumDevice].Bus   = (UINT16) DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus;
          ExtGfxTable.NumDevice++;
        }
        if (ExtGfxTable.NumDevice == 3) {
          break;
        }
      }
      DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ExtGfxTable.NumDevice = %d  ExtGfxTable.DeviceStructSize = %d\n", __FUNCTION__, ExtGfxTable.NumDevice, ExtGfxTable.DeviceStructSize));
      for (i = 0; i < ExtGfxTable.NumDevice; i++) {
        DEBUG ((DEBUG_INFO, "FEA-DXE-%a-ExtGfxTable.Device[%d].Flags = 0x%X  ExtGfxTable.Device[%d].Bus = 0x%X\n", __FUNCTION__, i, ExtGfxTable.Device[i].Flags, i, ExtGfxTable.Device[i].Bus));
      }

      if (ExtGfxTable.NumDevice != 0) {
        CpmTableProtocolPtr->CommonFunction.CopyMem ( &eNvStore->CpmAtifFun21ReturnTable[0],
                                                      &ExtGfxTable,
                                                      sizeof (AMD_CPM_EXTERNAL_GFX_TABLE));             // AtifFun21ReturnTable
      }
    } // End of if (DisplayFeatureTablePtr->AtifFun21Support != 0)

    // ATCS
    eNvStore->CpmAtcsSupportedFunctionMask = 0x2EF;        // AtcsSupportedFunctionMask; // Func 1, 2, 3, 4, 6, 7, 8, 10 supported
    if (DisplayFeatureTablePtr->AtcsFunction9Support) {
      eNvStore->CpmAtcsSupportedFunctionMask |= 0x100;
      eNvStore->CpmAtcsFunction9Buffer[0] = 9;            // Structure Size (WORD)= 1 WORD + 7 BYTE
      eNvStore->CpmAtcsFunction9Buffer[1] = 0;
      for (i = 0; i < 7; i++) {
        eNvStore->CpmAtcsFunction9Buffer[i+2] = DisplayFeatureTablePtr->AtcsFunction9Buffer[i];
      }
    }

    // AtpxSupportedFunctionMask
    if (DisplayFeatureTablePtr->DisplayMuxDeviceId) {
      // Set Bit[11] to enable ATPX Function 12
      eNvStore->CpmAtpxSupportedFunctionMask |= 0x00000800;
    }
    if (CurrentPowerXpress) {
      // Set Bit[1] and Bit[0] to enable ATPX Function 2 and ATPX Function 1
      eNvStore->CpmAtpxSupportedFunctionMask |= 0x00000003;
    } else if (CurrentHybridGraphics) {
      // Set Bit[0] to enable ATPX Function 1
      eNvStore->CpmAtpxSupportedFunctionMask |= 0x00000001;
    }
    if (DisplayFeatureTablePtr->MacoGpio != 0) {
      // Set Bit[9] to enable ATPX Function 10
      eNvStore->CpmAtpxSupportedFunctionMask |= 0x00000200;
    }
    if (FullPciEmulationSupport) {
      // Set Bit[10] to enable ATPX Function 11
      eNvStore->CpmAtpxSupportedFunctionMask |= 0x00000400;
    }

    // AtpxFlags
    if (DisplayFeatureTablePtr->MuxFlag & 7) {
      eNvStore->CpmAtpxFlags |= (DisplayFeatureTablePtr->MuxFlag & 7) << 18;
    }
    if (PowerXpressDynamicMode == 1) {
      eNvStore->CpmAtpxFlags |= 0x00000480;                                                           // PX Dynamic Mode Support Full dGPU Powerdown
    } else if (PowerXpressDynamicMode == 2) {
      eNvStore->CpmAtpxFlags |= 0x00000080;
    } else if (HybridGraphicsSupport) {
      eNvStore->CpmAtpxFlags |= 0x00004000;
    }
    if (DualGraphicsNotSupported) {
      eNvStore->CpmAtpxFlags |= 0x00000100;
    }
    if (!PulseGeneratorSupport) {
      eNvStore->CpmAtpxFlags |= 0x00000800;
    }
    if (FullPciEmulationSupport) {
      eNvStore->CpmAtpxFlags |= 0x00008000;
    }
    if (DetachableGraphicsSupport) {
      eNvStore->CpmAtpxFlags |= 0x00010000;
    }

    // ATIF
    eNvStore->CpmAtifSupportedNotificationMask |= DisplayFeatureTablePtr->AtifSupportedNotificationMask;// AtifSupportedNotificationMask
    if (CurrentPowerXpress || CurrentHybridGraphics) {
      eNvStore->CpmAtifSupportedNotificationMask |= 0x40;
    } else {
      eNvStore->CpmAtifSupportedNotificationMask &= 0xFFFFFFDF;
    }
    if (IsBrightnessByDriver) {
      eNvStore->CpmAtifSupportedNotificationMask |= 0x80;
    } else {
      eNvStore->CpmAtifSupportedNotificationMask &= 0xFFFFFF7F;
    }
    if (DisplayFeatureTablePtr->MxmOverTempEvent == 0xFF) {
      eNvStore->CpmAtifSupportedNotificationMask &= 0xFFFFFFF7;
    }
    if (DisplayFeatureTablePtr->DisplayConnectEvent != 0xFF) {
      eNvStore->CpmAtifSupportedNotificationMask |= 0x100;
    }
    eNvStore->CpmAtifFunctionBitVector = 0x8003;          // Enable AtifFunctionBitVector Bit[15], Bit[1] and Bit[0] to support Function 16 / 2 / 1
    if ((DisplayFeatureTablePtr->AtifNotifyCommandCode >= 0xD0) && (DisplayFeatureTablePtr->AtifNotifyCommandCode <= 0xD9)) {
      eNvStore->CpmAtifFlags = ((UINT32) DisplayFeatureTablePtr->AtifNotifyCommandCode << 24) + 0x2;
    } else {
      eNvStore->CpmAtifFlags = 0x81000001;
    }

    if ((DisplayFeatureTablePtr->AtifFun21Support != 0) && (ExtGfxTable.NumDevice != 0)) {
      eNvStore->CpmAtifFunctionBitVector |= 0x100000;     // Enable AtifFunctionBitVector Bit[20] to support Function 21
    } else {
      ZeroMem (&eNvStore->CpmAtifFun21ReturnTable[0], sizeof (AMD_CPM_EXTERNAL_GFX_TABLE));
    }

    if (DisplayFeatureTablePtr->AtifFun22Support != 0) {
      eNvStore->CpmAtifFunctionBitVector |= 0x200000;     // Enable AtifFunctionBitVector Bit[21] to support Function 22
      eNvStore->CpmAtifSupportedNotificationMask |= 0x1000;
    }

    if (DisplayFeatureTablePtr->AtifFunction23Support) {
      eNvStore->CpmAtifFunctionBitVector |= 0x400000;     // Enable AtifFunctionBitVector Bit[22] to support Function 23
      eNvStore->CpmAtifFunction23Buffer[0] = 9;           // Structure Size (WORD)= 1 WORD + 7 BYTE
      eNvStore->CpmAtifFunction23Buffer[1] = 0;
      for (i = 0; i < 7; i++) {
        eNvStore->CpmAtifFunction23Buffer[i+2] = DisplayFeatureTablePtr->AtifFunction23Buffer[i];
      }
    }

    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtpxSupportedFunctionMask     = 0x%08X\n", __FUNCTION__, eNvStore->CpmAtpxSupportedFunctionMask));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtpxFlags                     = 0x%08X\n", __FUNCTION__, eNvStore->CpmAtpxFlags));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtcsSupportedFunctionMask     = 0x%08X\n", __FUNCTION__, eNvStore->CpmAtcsSupportedFunctionMask));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtifSupportedNotificationMask = 0x%08X\n", __FUNCTION__, eNvStore->CpmAtifSupportedNotificationMask));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtifFunctionBitVector         = 0x%08X\n", __FUNCTION__, eNvStore->CpmAtifFunctionBitVector));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtifFlags                     = 0x%08X\n", __FUNCTION__, eNvStore->CpmAtifFlags));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtcsFunction9Buffer  [ ", __FUNCTION__));
    for (i = 0; i < 9; i++) {
      DEBUG((DEBUG_INFO, "%d ", eNvStore->CpmAtcsFunction9Buffer[i]));
    }
    DEBUG ((DEBUG_INFO, "]\n"));
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-eNvStore->CpmAtifFunction23Buffer [ ", __FUNCTION__));
    for (i = 0; i < 9; i++) {
      DEBUG((DEBUG_INFO, "%d ", eNvStore->CpmAtifFunction23Buffer[i]));
    }
    DEBUG ((DEBUG_INFO, "]\n"));

    GfxDetect.NumDevice         = 0;
    GfxDetect.DeviceStructSize  = 8;
    for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
      if (DevicePathTablePtr->Path[i].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid == 0) {
        break;
      }
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
        if (CurrentPowerXpress || CurrentHybridGraphics) {
          GfxDetect.Device[GfxDetect.NumDevice].Flags = DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga << 2;
        } else {
          GfxDetect.Device[GfxDetect.NumDevice].Flags = DevicePathTablePtr->Path[i].FeatureMask.Mask.Removable | (DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga << 2);
        }
        GfxDetect.Device[GfxDetect.NumDevice].Bus     = (UINT16)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus;
        GfxDetect.Device[GfxDetect.NumDevice].Device  = DevicePathTablePtr->Path[i].Device.Device;
        GfxDetect.NumDevice++;
      }
      if (GfxDetect.NumDevice == 3) {
        break;
      }
    }

    CpmTableProtocolPtr->CommonFunction.CopyMem ( &eNvStore->CpmAtifDeviceListBuffer[0],
                                                  &GfxDetect,
                                                  sizeof(AMD_CPM_GFX_DETECT) );                         // AtifDeviceListBuffer

    eNvStore->CpmiGpuP2pBridgePfa = DisplayFeatureDataPtr->GfxBridgePfa[0].Raw;                         // iGpuP2pBridgePfa
    eNvStore->CpmiGpuP2pDevicePfa = DisplayFeatureDataPtr->GfxDevicePfa[0].Raw;                         // iGpuP2pDevicePfa
    eNvStore->CpmdGpuP2pBridgePfa = DisplayFeatureDataPtr->GfxBridgePfa[1].Raw;                         // dGpuP2pBridgePfa
    eNvStore->CpmdGpuP2pDevicePfa = DisplayFeatureDataPtr->GfxDevicePfa[1].Raw;                         // dGpuP2pDevicePfa

    eNvStore->CpmdGpuAspmLxEnable = 0;                                                                  // dGpuAspmLxEnable
    if (PcieTopologyTablePtr && eNvStore->CpmdGpuP2pBridgePfa) {
      PciePortDescriptorPtr   = &PcieTopologyTablePtr->Port[0];
      while (TRUE) {
        if (PciePortDescriptorPtr->Port.DeviceNumber == DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device) {
          if (PciePortDescriptorPtr->Port.FunctionNumber == DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function) {
            eNvStore->CpmdGpuAspmLxEnable = CpmTableProtocolPtr->CommonFunction.PciRead8 (  CpmTableProtocolPtr,
                                                                                            DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus,
                                                                                            DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device,
                                                                                            DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function,
                                                                                            0x68 );
            break;
          }
        }
        if (PciePortDescriptorPtr->Flags & DESCRIPTOR_TERMINATE_LIST) {
          break;
        }
        PciePortDescriptorPtr ++;
      }
    }

    if (DxioTopologyTablePtr && eNvStore->CpmdGpuP2pBridgePfa) {
      DxioPortDescriptorPtr   = &DxioTopologyTablePtr->Port[0];
      while (TRUE) {
        if (DxioPortDescriptorPtr->Port.DeviceNumber == DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device) {
          if (DxioPortDescriptorPtr->Port.FunctionNumber == DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function) {
            eNvStore->CpmdGpuAspmLxEnable = CpmTableProtocolPtr->CommonFunction.PciRead8 (  CpmTableProtocolPtr,
                                                                                            DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus,
                                                                                            DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device,
                                                                                            DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function,
                                                                                            0x68 );
            break;
          }
        }
        if (DxioPortDescriptorPtr->Flags & DESCRIPTOR_TERMINATE_LIST) {
          break;
        }
        DxioPortDescriptorPtr ++;
      }
    }

    iGpuIndex = 0xFF;
    dGpuIndex = 0xFF;
    i = 0;
    while (i < AMD_DISPLAY_DEVICE_PATH_SIZE) {
      if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
        break;
      }
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[i].IsDgpu) {
        iGpuIndex = i;
      }
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[i].IsDgpu && DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga) {
        dGpuIndex = i;
      }
      i++;
    }
    if (iGpuIndex == 0xFF) {
      MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 1;
    }
    if (dGpuIndex == 0xFF) {
      i = 0;
      while (i < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
          break;
        }
        if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[i].IsDgpu) {
          dGpuIndex = i;
          break;
        }
        i++;
      }
    }
    if (dGpuIndex == 0xFF) {
      MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 0;
      i = 0;
      while (i < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
          break;
        }
        if (DevicePathTablePtr->Path[i].IsDgpu) {
          dGpuIndex = i;
          break;
        }
        i++;
      }
    }
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a-iGpuIndex = 0x%02X  dGpuIndex = 0x%02X\n", __FUNCTION__, iGpuIndex, dGpuIndex));

    ForceAtpxAtifInDgpu = MainTablePtr->DisplayFeature.Config.ForceAtpxAtifInDgpu;
    IsDgpuPrimary = MainTablePtr->DisplayFeature.Config.IsDgpuPrimary;

    eNvStore->CpmdGpuAudioDisable = 0;
    if (DisableDgpuAudioInPX) {
      eNvStore->CpmdGpuAudioDisable = CFG_CPM_ACPI_DISABLE_DGPU_AUDIO_SW_SMI;
    }

    eNvStore->CpmDisplayFeatureConfig = MainTablePtr->DisplayFeature.Raw;

    for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
      if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
        break;
      }
      if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu && DevicePathTablePtr->Path[Index].IsDgpu) {
        Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
        Dev       = DevicePathTablePtr->Path[Index].Device.Device;
        Func      = DevicePathTablePtr->Path[Index].Device.Function;
        DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x02);
        DeviceId2 = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func + 1, 0x02);
        if ((DgpuSsid != 0) && (DeviceId != 0xFFFF) && DisplayFeatureTablePtr->DgpuSsidSvid) {
          CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func, 0x4C, DgpuSsid);
        }
        if ((DgpuAudioSsid != 0) && (DeviceId2 != 0xFFFF) && DisplayFeatureTablePtr->DgpuSsidSvid) {
          CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func + 1, 0x4C, DgpuAudioSsid);
        }
      } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu && DevicePathTablePtr->Path[Index].IsDgpu)
    } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)

    if (!(DisplayFeatureTablePtr->FunctionDisableMask & BIT0)) {
      if (PowerXpressFixedMode || PowerXpressDynamicMode || HybridGraphicsSupport) {
        Ssid = 0x01240000;
      } else if (HyperCrossFire) {
        if (IsDgpuPrimary) {
          Ssid = 0x01250000;
        } else {
          Ssid = 0x01260000;
        }
      } else {
        Ssid = 0x01230000;
      }

      DisplayFeatureDataPtr->Ssid = Ssid;

      for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
        if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu) {
          Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
          Dev       = DevicePathTablePtr->Path[Index].Device.Device;
          Func      = DevicePathTablePtr->Path[Index].Device.Function;
          VendorId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                      Bus,
                                                                      Dev,
                                                                      Func,
                                                                      0x00 );
          DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                      Bus,
                                                                      Dev,
                                                                      Func,
                                                                      0x02 );
          DeviceId2 = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                      Bus,
                                                                      Dev,
                                                                      Func + 1,
                                                                      0x02 );
          NewSsid   = Ssid | (UINT32)VendorId;
          if (SpecificSsidTablePtr) {
            for (j = 0; j < AMD_SPECIFIC_SSID_DEVICE_SIZE; j++) {
              if (SpecificSsidTablePtr->Item[j].VendorId == 0xFFFF) {
                break;
              } else {
                if ((SpecificSsidTablePtr->Item[j].VendorId == VendorId) && (SpecificSsidTablePtr->Item[j].DeviceId == DeviceId) && DisplayFeatureTablePtr->DgpuSsidSvid) {
                  CpmTableProtocolPtr->CommonFunction.PciWrite32 (  CpmTableProtocolPtr,
                                                                    Bus,
                                                                    Dev,
                                                                    Func,
                                                                    0x4C,
                                                                    NewSsid );
                }
                if ((DeviceId2 != 0xFFFF) && (SpecificSsidTablePtr->Item[j].VendorId == VendorId) && (SpecificSsidTablePtr->Item[j].DeviceId == DeviceId2) && DisplayFeatureTablePtr->DgpuSsidSvid) {
                  CpmTableProtocolPtr->CommonFunction.PciWrite32 (  CpmTableProtocolPtr,
                                                                    Bus,
                                                                    Dev,
                                                                    Func + 1,
                                                                    0x4C,
                                                                    NewSsid );
                }
              }
            }
          } else if ((DgpuSsid == 0) && DisplayFeatureTablePtr->DgpuSsidSvid) {
            CpmTableProtocolPtr->CommonFunction.PciWrite32 (  CpmTableProtocolPtr,
                                                              Bus,
                                                              Dev,
                                                              Func,
                                                              0x4C,
                                                              NewSsid );
          }
        } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].FeatureMask.Mask.NonAmdGpu)
      } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)
    } // End of if (!(DisplayFeatureTablePtr->FunctionDisableMask & BIT0))

    if (RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr) {
      switch (RebrandDualGraphicsSsidTablePtr->Header.FormatRevision) {
      case 0:
        Ssid = 0xFFFF0000;
        for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
          if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
            break;
          }
          if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].IsDgpu) {
            Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
            Dev       = DevicePathTablePtr->Path[Index].Device.Device;
            Func      = DevicePathTablePtr->Path[Index].Device.Function;
            VendorId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                        Bus,
                                                                        Dev,
                                                                        Func,
                                                                        0x00 );
            DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                        Bus,
                                                                        Dev,
                                                                        Func,
                                                                        0x02 );

            for (j = 0; j < AMD_REBRAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE; j++) {
              if (RebrandDualGraphicsSsidTablePtr->Item[j].VendorId == 0xFFFF) {
                break;
              } else {
                if ((RebrandDualGraphicsSsidTablePtr->Item[j].VendorId == VendorId) &&
                    (RebrandDualGraphicsSsidTablePtr->Item[j].DeviceId == DeviceId) &&
                    (RebrandDualGraphicsSsidTablePtr->Item[j].IsDgpu == 0)) {
                    Ssid = ((UINT32) DeviceId) << 16;
                  break;
                }
              }
            }
          } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].IsDgpu)
        } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)
        DisplayFeatureDataPtr->Ssid2 = Ssid;
        if (Ssid != 0xFFFF0000) {
          for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
            if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
              break;
            }
            if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu) {
              Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
              Dev       = DevicePathTablePtr->Path[Index].Device.Device;
              Func      = DevicePathTablePtr->Path[Index].Device.Function;
              VendorId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                          Bus,
                                                                          Dev,
                                                                          Func,
                                                                          0x00 );
              VendorId2 = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                          Bus,
                                                                          Dev,
                                                                          Func,
                                                                          0x2C );
              DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                          Bus,
                                                                          Dev,
                                                                          Func,
                                                                          0x02 );
              NewSsid   = Ssid | (UINT32)VendorId2;

              for (j = 0; j < AMD_REBRAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE; j++) {
                if (RebrandDualGraphicsSsidTablePtr->Item[j].VendorId == 0xFFFF) {
                  break;
                } else {
                  if ((RebrandDualGraphicsSsidTablePtr->Item[j].VendorId == VendorId) &&
                      (RebrandDualGraphicsSsidTablePtr->Item[j].DeviceId == DeviceId) &&
                      (RebrandDualGraphicsSsidTablePtr->Item[j].IsDgpu == 1) &&
                      DisplayFeatureTablePtr->DgpuSsidSvid) {
                    CpmTableProtocolPtr->CommonFunction.PciWrite32 (  CpmTableProtocolPtr,
                                                                      Bus,
                                                                      Dev,
                                                                      Func,
                                                                      0x4C,
                                                                      NewSsid );
                    break;
                  }
                }
              }
            } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu)
          } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)
        } // End of if (Ssid != 0xFFFF0000)
      case 1:
        Ssid = 0xFFFF0000;
        for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
          if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
            break;
          }
          if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].IsDgpu) {
            Bus   = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
            Dev   = DevicePathTablePtr->Path[Index].Device.Device;
            Func  = DevicePathTablePtr->Path[Index].Device.Function;
            iSsid = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                    Bus,
                                                                    Dev,
                                                                    Func,
                                                                    0x2E );
            Ssid  = ((UINT32)iSsid) << 16;
          } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].IsDgpu)
        } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)
        DisplayFeatureDataPtr->Ssid2 = Ssid;
        if (Ssid != 0xFFFF0000) {
          iSsid = (UINT16) (Ssid >> 16);
          for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
            if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
              break;
            }
            if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu) {
              Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
              Dev       = DevicePathTablePtr->Path[Index].Device.Device;
              Func      = DevicePathTablePtr->Path[Index].Device.Function;
              DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                          Bus,
                                                                          Dev,
                                                                          Func,
                                                                          0x02 );
              VendorId2 = CpmTableProtocolPtr->CommonFunction.PciRead16 ( CpmTableProtocolPtr,
                                                                          Bus,
                                                                          Dev,
                                                                          Func,
                                                                          0x2C );
              RevId     = CpmTableProtocolPtr->CommonFunction.PciRead8 (  CpmTableProtocolPtr,
                                                                          Bus,
                                                                          Dev,
                                                                          Func,
                                                                          0x08 );
              if (RebrandDualGraphicsSsidTable2Ptr->Svid == 0 || RebrandDualGraphicsSsidTable2Ptr->Svid == 0xFFFF) {
                NewSsid   = Ssid | (UINT32)VendorId2;
              } else {
                NewSsid   = Ssid | (UINT32)RebrandDualGraphicsSsidTable2Ptr->Svid;
              }

              for (j = 0; j < AMD_REBRAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE; j++) {
                if (RebrandDualGraphicsSsidTable2Ptr->Item[j].dDeviceId == 0xFFFF) {
                  break;
                } else {
                  if ((RebrandDualGraphicsSsidTable2Ptr->Item[j].dRevId == RevId) &&
                      (RebrandDualGraphicsSsidTable2Ptr->Item[j].iSsid == iSsid) &&
                      (RebrandDualGraphicsSsidTable2Ptr->Item[j].dDeviceId == DeviceId) &&
                      DisplayFeatureTablePtr->DgpuSsidSvid) {
                    CpmTableProtocolPtr->CommonFunction.PciWrite32 (  CpmTableProtocolPtr,
                                                                      Bus,
                                                                      Dev,
                                                                      Func,
                                                                      0x4C,
                                                                      NewSsid );
                    break;
                  }
                }
              } // End of for (j = 0; j < AMD_REBRAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE; j++)
            } // End of if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu)
          } // End of for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++)
        } // End of if (Ssid != 0xFFFF0000)
      } // End of switch (RebrandDualGraphicsSsidTablePtr->Header.FormatRevision)
    } // End of if (RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr)

    // Patch PX ASL SSDT Table
    if (!(DisplayFeatureTablePtr->FunctionDisableMask & BIT1)) {
      for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
        if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
          ZeroMem (Buffer, sizeof (Buffer));
          if ((DevicePathTablePtr->Path[i].IsDgpu) && (i == dGpuIndex)) {
            DEBUG((DEBUG_INFO, "FEA-DXE-%a-DevicePathTablePtr->Path[%d].IsDgpu == 1\n", __FUNCTION__, i));
            if (DevicePathTablePtr->Path[i].FeatureMask.Mask.MultiBridge == 0) {
              Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                DevicePathTablePtr->Path[i].Bridge.Function );
              if (DevicePathTablePtr->Path[i].FeatureMask.Mask.D3Cold && D3ColdSupport) {
                Buffer[1] = 1;
              }
              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','D','G','1');   // ID = CPMDFDG1
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFDG1 from DGpuSsdt_DGpuScope1\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmDGpuSsdtCallBack,
                                                                  &Buffer[0] );
            } else {
              Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                DevicePathTablePtr->Path[i].Bridge.Function );
              if (DevicePathTablePtr->Path[i].FeatureMask.Mask.D3Cold && D3ColdSupport) {
                Buffer[1] = 1;
              }
              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','D','G','2');   // ID = CPMDFDG2
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFDG2 from DGpuSsdt_DGpuScope2\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmDGpuSsdtCallBack,
                                                                  &Buffer[0] );
            }
            if (DevicePathTablePtr->Path[i].FeatureMask.Mask.D3Cold && D3ColdSupport) {
              Buffer[1] = (UINT32) DevicePathTablePtr->Path[i].DeviceIdVcc;
              Buffer[2] = (UINT32) DevicePathTablePtr->Path[i].Bridge.Device;
              Buffer[3] = (UINT32) DevicePathTablePtr->Path[i].Bridge.Function;
              Buffer[4] = (UINT32) DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus;
              if (DevicePathTablePtr->Path[i].FeatureMask.Mask.NonAmdGpu) {
                Buffer[5] = 0;
                Buffer[6] = 0;
              } else {
                Buffer[5] = CpmTableProtocolPtr->CommonFunction.PciRead32 ( CpmTableProtocolPtr,
                                                                            (UINT8)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus,
                                                                            0,
                                                                            0,
                                                                            0x2C );
                Buffer[6] = CpmTableProtocolPtr->CommonFunction.PciRead32 ( CpmTableProtocolPtr,
                                                                            (UINT8)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus,
                                                                            0,
                                                                            1,
                                                                            0x2C );
              }
              Buffer[7] = 1;

              Buffer[8] = DisplayFeatureDataPtr->GfxBridgePMem[1].BridgeCommand;
              Buffer[9] = DisplayFeatureDataPtr->GfxBridgePMem[1].BusNumberReg;
              Buffer[10] = DisplayFeatureDataPtr->GfxBridgePMem[1].BridgeNonPrefetchable;
              Buffer[11] = DisplayFeatureDataPtr->GfxBridgePMem[1].LimitBaseLowReg;
              Buffer[12] = DisplayFeatureDataPtr->GfxBridgePMem[1].BaseHighReg;
              Buffer[13] = DisplayFeatureDataPtr->GfxBridgePMem[1].LimitHighReg;
              Buffer[14] = 1;

              Buffer[15] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BridgeCommand;
              Buffer[16] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BusNumberReg;
              Buffer[17] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BridgeNonPrefetchable;
              Buffer[18] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].LimitBaseLowReg;
              Buffer[19] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BaseHighReg;
              Buffer[20] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].LimitHighReg;
              Buffer[21] = 1;

              Buffer[22] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BridgeCommand;
              Buffer[23] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BusNumberReg;
              Buffer[24] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BridgeNonPrefetchable;
              Buffer[25] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].LimitBaseLowReg;
              Buffer[26] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BaseHighReg;
              Buffer[27] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].LimitHighReg;
              Buffer[28] = 1;

              Buffer[29] = DisplayFeatureDataPtr->GfxDevicePMem[1].DeviceCommand;
              Buffer[30] = DisplayFeatureDataPtr->GfxDevicePMem[1].DoorbellLowReg;
              Buffer[31] = DisplayFeatureDataPtr->GfxDevicePMem[1].DoorbellHighReg;
              Buffer[32] = DisplayFeatureDataPtr->GfxDevicePMem[1].DeviceNonPrefetchable;
              Buffer[33] = DisplayFeatureDataPtr->GfxDevicePMem[1].DeviceIdReg;
              Buffer[34] = 1;
              Buffer[35] = (UINT32)(PcdGetBool(NonZ10CapPcieZstateSupport));

              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','3','C','L','D');   // ID = CPMD3CLD
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMD3CLD from DGpuSsdt_DGpuD3Cold\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmDGpuD3ColdCallBack,
                                                                  &Buffer[0] );
            }
            // End of if ((DevicePathTablePtr->Path[i].IsDgpu) && (i == dGpuIndex))
          } else if ((dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE) && (i == iGpuIndex)) {
            DEBUG((DEBUG_INFO, "FEA-DXE-%a-dGpuIndex = 0x%02X < AMD_DISPLAY_DEVICE_PATH_SIZE = 0x%X\n", __FUNCTION__, dGpuIndex, AMD_DISPLAY_DEVICE_PATH_SIZE));
            Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                              DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                                              DevicePathTablePtr->Path[dGpuIndex].Bridge.Function );
            if (DisplayFeatureDataPtr->VBiosFlag && (CurrentPowerXpress || CurrentHyperCrossFire || CurrentHybridGraphics) && !IsDgpuPrimary) {
              Buffer[1] = 1;
            } else {
              Buffer[1] = 0;
            }
            Buffer[2] = 0;
            if (DevicePathTablePtr->Path[i].Bridge.Device == 0) {
              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','I','G','1');   // ID = CPMDFIG1
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFIG1 from IGpuSsdt_IGpuGlobalMethod1\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmIGpuSsdtCallBack,
                                                                  &Buffer[0] );
            } else {
              Buffer[2] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                DevicePathTablePtr->Path[i].Bridge.Function );
              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','I','G','2');   // ID = CPMDFIG2
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFIG2 from IGpuSsdt_IGpuGlobalMethod2\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmIGpuSsdtCallBack,
                                                                  &Buffer[0] );
            }
            iGpuIndex = 0;
            // End of else if ((dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE) && (i == iGpuIndex))
          } else if ((dGpuIndex == 0xFF) && (i == iGpuIndex)) {
            DEBUG((DEBUG_INFO, "FEA-DXE-%a-dGpuIndex = 0xFF\n", __FUNCTION__));
            Buffer[0] = 0;
            if (DisplayFeatureDataPtr->VBiosFlag && (CurrentPowerXpress || CurrentHyperCrossFire || CurrentHybridGraphics) && !IsDgpuPrimary) {
              Buffer[1] = 1;
            } else {
              Buffer[1] = 0;
            }
            Buffer[2] = 0;
            if (DevicePathTablePtr->Path[i].Bridge.Device == 0) {
              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','I','G','3');   // ID = CPMDFIG3
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFIG3 from IGpuSsdt_IGpuGlobalMethod3\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmIGpuSsdtCallBack,
                                                                  &Buffer[0] );
            } else {
              Buffer[2] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                DevicePathTablePtr->Path[i].Bridge.Function );
              OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','I','G','4');   // ID = CPMDFIG4
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFIG4 from IGpuSsdt_IGpuGlobalMethod4\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmIGpuSsdtCallBack,
                                                                  &Buffer[0] );
            }
            iGpuIndex = 0;
          } // End of else if ((dGpuIndex == 0xFF) && (i == iGpuIndex))
          ZeroMem (Buffer, sizeof (Buffer));
          if ((ForceAtpxAtifInDgpu) && (DevicePathTablePtr->Path[i].IsDgpu) && (i == dGpuIndex)) {
            if (DevicePathTablePtr->Path[i].FeatureMask.Mask.MultiBridge == 0) {
              if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.NonAmdGpu) {
                Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                  DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                  DevicePathTablePtr->Path[i].Bridge.Function );
              OemTableIdSig = SIGNATURE_64 ('C','D','F','A','A','D','G','1');   // ID = CDFAADG1
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CDFAADG1 from AtpxAtifSsdt_AtpxAtifDGpu1\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmDGpuSsdtCallBack,
                                                                  &Buffer[0] );
              }
            } else {
              Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                DevicePathTablePtr->Path[i].Bridge.Function );
              OemTableIdSig = SIGNATURE_64 ('C','D','F','A','A','D','G','2');   // ID = CDFAADG2
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CDFAADG2 from AtpxAtifSsdt_AtpxAtifDGpu2\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmDGpuSsdtCallBack,
                                                                  &Buffer[0] );
            }
          } else if ((!DevicePathTablePtr->Path[i].IsDgpu) && (i == iGpuIndex)) {
            Buffer[0] = 0;
            if (DevicePathTablePtr->Path[i].Bridge.Device == 0) {
              Buffer[1] = 0;
              Buffer[2] = 0;
              OemTableIdSig = SIGNATURE_64 ('C','D','F','A','A','I','G','1');   // ID = CDFAAIG1
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CDFAAIG1 from AtpxAtifSsdt_AtpxAtifIGpu1\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmIGpuSsdtCallBack,
                                                                  &Buffer[0] );
            } else {
              Buffer[1] = 0;
              Buffer[2] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                                DevicePathTablePtr->Path[i].Bridge.Function );
              OemTableIdSig = SIGNATURE_64 ('C','D','F','A','A','I','G','2');   // ID = CDFAAIG2
              DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CDFAAIG2 from AtpxAtifSsdt_AtpxAtifIGpu2\n", __FUNCTION__));
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                                  &gEfiCallerIdGuid,
                                                                  &OemTableIdSig,
                                                                  AmdCpmIGpuSsdtCallBack,
                                                                  &Buffer[0] );
            }
          }
        } else {
          if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid == 0) {
            break;
          }
        }
      } // End of for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++)

      if (iGpuIndex == 0xFF && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[dGpuIndex].FeatureMask.Mask.NonAmdGpu) {
          ZeroMem (Buffer, sizeof (Buffer));
          Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                            DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                                            DevicePathTablePtr->Path[dGpuIndex].Bridge.Function );
          Buffer[1] = 0;
          Buffer[2] = 0;
          OemTableIdSig = SIGNATURE_64 ('C','P','M','D','F','I','G','5');   // ID = CPMDFIG5
          DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMDFIG5 from IGpuSsdt_IGpuGlobalMethod5\n", __FUNCTION__));
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                              &gEfiCallerIdGuid,
                                                              &OemTableIdSig,
                                                              AmdCpmIGpuSsdtCallBack,
                                                              &Buffer[0] );
        }
      } // End of if (iGpuIndex == 0xFF && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE)

      if (DisplayFeatureTablePtr->MxmOverTempEvent != 0xFF && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[dGpuIndex].FeatureMask.Mask.NonAmdGpu) {
          ZeroMem (Buffer, sizeof (Buffer));
          SciMap = CpmTableProtocolPtr->CommonFunction.GetSciMap (  CpmTableProtocolPtr,
                                                                    DisplayFeatureTablePtr->MxmOverTempEvent );
          if ((SciMap & 0xF) < 0xA) {
            Buffer[0] = 0x3000 + (((SciMap) & 0xF) << 8);
          } else {
            Buffer[0] = 0x4000 + ((((SciMap) & 0xF) - 0x9) << 8);
          }
          if (((SciMap >> 4) & 0xF) < 0xA) {
            Buffer[0] |= 0x30 + ((SciMap >> 4) & 0xF);
          } else {
            Buffer[0] |= 0x40 + (((SciMap >> 4) & 0xF) - 0x9);
          }
          Buffer[0] = (Buffer[0] << 16) + (((UINT32)'L') << 8) + (UINT32)'_';
          OemTableIdSig = SIGNATURE_64 ('D','F','D','G','O','V','T','P');   // ID = DFDGOVTP
          DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = DFDGOVTP from DGpuSsdt_DGpuOverTemperature\n", __FUNCTION__));
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                              &gEfiCallerIdGuid,
                                                              &OemTableIdSig,
                                                              AmdCpmMxmOverTSsdtCallBack,
                                                              &Buffer[0] );
        }
      } // End of if (DisplayFeatureTablePtr->MxmOverTempEvent != 0xFF && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE)

      if (DisplayFeatureTablePtr->DisplayConnectEvent != 0xFF && (CurrentPowerXpress || CurrentHybridGraphics) && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[dGpuIndex].FeatureMask.Mask.NonAmdGpu) {
          ZeroMem (Buffer, sizeof (Buffer));
          SciMap = CpmTableProtocolPtr->CommonFunction.GetSciMap (  CpmTableProtocolPtr,
                                                                    DisplayFeatureTablePtr->DisplayConnectEvent );
          if ((SciMap & 0xF) < 0xA) {
            Buffer[0] = 0x3000 + (((SciMap) & 0xF) << 8);
          } else {
            Buffer[0] = 0x4000 + ((((SciMap) & 0xF) - 0x9) << 8);
          }
          if (((SciMap >> 4) & 0xF) < 0xA) {
            Buffer[0] |= 0x30 + ((SciMap >> 4) & 0xF);
          } else {
            Buffer[0] |= 0x40 + (((SciMap >> 4) & 0xF) - 0x9);
          }
          Buffer[0] = (Buffer[0] << 16) + (((UINT32)'L') << 8) + (UINT32)'_';
          Buffer[1] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                            DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                                            DevicePathTablePtr->Path[dGpuIndex].Bridge.Function );
          OemTableIdSig = SIGNATURE_64 ('D','F','D','G','C','N','E','V');   // ID = DFDGCNEV
          DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = DFDGCNEV from DGpuSsdt_DGpuConnectEvent\n", __FUNCTION__));
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                              &gEfiCallerIdGuid,
                                                              &OemTableIdSig,
                                                              AmdCpmDisplayConnectEventSsdtCallBack,
                                                              &Buffer[0] );
        }
      } // End of if (DisplayFeatureTablePtr->DisplayConnectEvent != 0xFF && (CurrentPowerXpress || CurrentHybridGraphics) && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE)

      if ((DisplayFeatureTablePtr->MuxFlag & BIT4) == BIT4 && iGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[dGpuIndex].FeatureMask.Mask.NonAmdGpu) {
          ZeroMem (Buffer, sizeof (Buffer));
          OemTableIdSig = SIGNATURE_64 ('C','P','M','S','M','M','U','X');   // ID = CPMSMMUX
          Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                            DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                                            DevicePathTablePtr->Path[dGpuIndex].Bridge.Function );
          Buffer[1] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                            DevicePathTablePtr->Path[iGpuIndex].Bridge.Device,
                                                                            DevicePathTablePtr->Path[iGpuIndex].Bridge.Function );
          Buffer[2] = (UINT32) DevicePathTablePtr->Path[dGpuIndex].DeviceId;
          Buffer[3] = (UINT32) DevicePathTablePtr->Path[dGpuIndex].Mode;
          Buffer[4] = (UINT32) DisplayFeatureTablePtr->SmartMux_HID;
          Buffer[5] = (UINT32) (DisplayFeatureTablePtr->SmartMux_HID >> 32);
          Buffer[6] = (UINT32) DisplayFeatureTablePtr->SmartMuxAcpiMethodLocation;
          DEBUG((DEBUG_INFO, "FEA-DXE-%a-AddSsdtTable-ID = CPMSMMUX from SmartMuxSsdt_SmartMux\n", __FUNCTION__));
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID *) CpmTableProtocolPtr,
                                                              &gEfiCallerIdGuid,
                                                              &OemTableIdSig,
                                                              AmdCpmSmartMuxSsdtCallBack,
                                                              &Buffer[0] );
        }
      } // End of if ((DisplayFeatureTablePtr->MuxFlag & BIT4) == BIT4 && iGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE && dGpuIndex < AMD_DISPLAY_DEVICE_PATH_SIZE)
    } // End of if (!(DisplayFeatureTablePtr->FunctionDisableMask & BIT1))
  } // End of if ((DisplayFeatureTablePtr) && (DevicePathTablePtr))

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpDisplayFeatureDxeInitLateEnd);

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-2\n", __FUNCTION__));
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for Smart Mux
 *
 * This function is used to update the name of PCIe bridge for iGPU and dGPU in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
AmdCpmSmartMuxSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;

  BufferPtr = (UINT32*)Context;

  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):     // ASL code: \_SB.PCI0.PB2
    if (*BufferPtr) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PB2_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'B', 'C', '_'):     // ASL code: \_SB.PCI0.PBC
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PBC_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *(BufferPtr + 1);
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '0', 'C'):     // ASL code: CpmMuxSelDeviceId
    if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
      *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 2));
      DEBUG ((DEBUG_INFO, "\t%a(%d) M30C is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '0', 'D'):     // ASL code: CpmDgpuMuxSetting
    if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
      *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 3));
      DEBUG ((DEBUG_INFO, "\t%a(%d) M30D is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '0', 'E'):     // ASL code: CpmMuxSelDeviceIdInIgpu
    if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
      *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 2));
      DEBUG ((DEBUG_INFO, "\t%a(%d) M30E is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '0', 'F'):     // ASL code: CpmDgpuMuxSettingInIgpu
    if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
      *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 3));
      DEBUG ((DEBUG_INFO, "\t%a(%d) M30F is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('_', 'H', 'I', 'D'):     // ASL code: _HID
    if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
      if ((*(BufferPtr + 4) != CPM_SIGNATURE_32 ('S', 'M', 'U', 'X')) || (*(BufferPtr + 5) != CPM_SIGNATURE_32 ('1', '2', '3', '4'))) {
        *(UINT32 *) (((UINT8 *) AmlObjPtr) + 5) = *(BufferPtr + 4);
        *(UINT32 *) (((UINT8 *) AmlObjPtr) + 9) = *(BufferPtr + 5);
        DEBUG ((DEBUG_INFO, "\t%a(%d) _HID is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('A', 'M', 'Q', 'U'):     // ASL code: \_SB.MUX1.AMQU
    if (*(BufferPtr + 6) & BIT0) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('D', 'M', 'Q', 'U');
      DEBUG ((DEBUG_INFO, "\t%a(%d) AMQU is updated to DMQU\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('A', 'M', 'C', 'F'):     // ASL code: \_SB.MUX1.AMCF
    if (*(BufferPtr + 6) & BIT0) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('D', 'M', 'C', 'F');
      DEBUG ((DEBUG_INFO, "\t%a(%d) AMCF is updated to DMCF\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('B', 'M', 'Q', 'U'):     // ASL code: \_SB.PCI0.PB2_.SWUS.SWDS.VGA.BMQU
    if (*(BufferPtr + 6) & BIT1) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('D', 'M', 'Q', 'U');
      DEBUG ((DEBUG_INFO, "\t%a(%d) BMQU is updated to DMQU\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('B', 'M', 'C', 'F'):     // ASL code: \_SB.PCI0.PB2_.SWUS.SWDS.VGA.BMCF
    if (*(BufferPtr + 6) & BIT1) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('D', 'M', 'C', 'F');
      DEBUG ((DEBUG_INFO, "\t%a(%d) BMCF is updated to DMCF\n", __FUNCTION__, __LINE__));
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for iGPU
 *
 * This function is used to update the name of PCIe bridge for dGPU in ASL
 * and renames XTRM to ATRM if PowerXpress is enabled.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
AmdCpmIGpuSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;

  BufferPtr = (UINT32*)Context;

  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('X', 'T', 'R', 'M'):     // ASL code: Method(XTRM,2,Serialized)
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) XTRM is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('A', 'T', 'R', 'M');
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):     // ASL code: \_SB.PCI0.PB2
    if (*BufferPtr) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PB2_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'B', 'C', '_'):     // ASL code: \_SB.PCI0.PBC
    if (*(BufferPtr + 2)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PBC_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *(BufferPtr + 2);
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for dGPU
 *
 * This function is used to update the name of PCIe bridge for dGPU in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
AmdCpmDGpuSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):     // ASL code: \_SB.PCI0.PB2
    if (*BufferPtr) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PB2_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'P', 'R', '0'):     // ASL code: Name(XPR0, Package() {CpmDgpuPowerResourceVcc}) // Power resources required for D0
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) XPR0 is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'P', 'R', '0');
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'P', 'R', '2'):     // ASL code: Name(XPR2, Package() {CpmDgpuPowerResourceVcc}) // Power resources required for D2
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) XPR2 is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'P', 'R', '2');
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'P', 'R', '3'):     // ASL code: Name(XPR3, Package() {CpmDgpuPowerResourceVcc}) // Power resources required for D3
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) XPR3 is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'P', 'R', '3');
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'S', '0', 'W'):     // ASL code: Name(XS0W, 4)
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) XS0W is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'S', '0', 'W');
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for MXM_OVERT
 *
 * This function is used to update the name of GPE method in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
AmdCpmMxmOverTSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('M', '2', '1', '7'):     // ASL code: CpmMxmOverTempEventMethod: M217
    if (*BufferPtr) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) M217 is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for Discrete GPU
 * connect/disconnect event
 *
 * This function is used to update the name of GPE method in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
AmdCpmDisplayConnectEventSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('M', '2', '1', '8'):     // ASL code: CpmDisplayConnectEventMethod: M218
    if (*BufferPtr) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) M218 is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):     // ASL code: \_SB.PCI0.PB2
    if (*(BufferPtr + 1)) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PB2_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *(BufferPtr + 1);
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for dGPU D3Cold
 *
 * This function is used to update the name of PCIe bridge for dGPU in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
AmdCpmDGpuD3ColdCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  UINT8     *DataPtr;
  BufferPtr = (UINT32*)Context;
  DataPtr = (UINT8*)AmlObjPtr;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):     // ASL code: \_SB.PCI0.PB2
    if (*BufferPtr) {
      DEBUG ((DEBUG_INFO, "\t%a(%d) PB2_ is updated\n", __FUNCTION__, __LINE__));
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  case CPM_SIGNATURE_32 ('M', '2', '3', '6'):     // ASL code: CpmDgpuPowerResourceBuffer: M236
    if (*(BufferPtr + 1)) {
      if (*(BufferPtr + 7)) {
// In DGpuD3Cold.asl
//            CreateByteField(CpmDgpuPowerResourceBuffer, 0, CpmDgpuPowerResourceDeviceId)
//            CreateByteField(CpmDgpuPowerResourceBuffer, 1, CpmBridgeDevice)
//            CreateByteField(CpmDgpuPowerResourceBuffer, 2, CpmBridgeFunction)
//            CreateByteField(CpmDgpuPowerResourceBuffer, 3, CpmDgpuBus)
//            CreateDwordField(CpmDgpuPowerResourceBuffer, 4, CpmDgpuSsidVid0)
//            CreateDwordField(CpmDgpuPowerResourceBuffer, 8, CpmDgpuSsidVid1)
// In AmdCpmDisplayFeatureInitLate.c
//                Buffer[1] = (UINT32) DevicePathTablePtr->Path[i].DeviceIdVcc;
//                Buffer[2] = (UINT32) DevicePathTablePtr->Path[i].Bridge.Device;
//                Buffer[3] = (UINT32) DevicePathTablePtr->Path[i].Bridge.Function;
//                Buffer[4] = (UINT32) DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus;
//                Buffer[5] = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, (UINT8)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus, 0, 0, 0x2C);
//                Buffer[6] = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, (UINT8)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus, 0, 1, 0x2C);
//                Buffer[7] = 1;
        DEBUG ((DEBUG_INFO, "\t%a(%d) M236 is updated\n", __FUNCTION__, __LINE__));
        *((UINT8*)AmlObjPtr + 8) = *(UINT8*)(BufferPtr + 1);
        *((UINT8*)AmlObjPtr + 9) = *(UINT8*)(BufferPtr + 2);
        *((UINT8*)AmlObjPtr + 10) = *(UINT8*)(BufferPtr + 3);
        *((UINT8*)AmlObjPtr + 11) = *(UINT8*)(BufferPtr + 4);
        *((UINT32*)AmlObjPtr + 3) = *(BufferPtr + 5);
        *((UINT32*)AmlObjPtr + 4) = *(BufferPtr + 6);
        *(BufferPtr + 7) = 0;
      }
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '5', '0'):     // ASL code: CpmdGpuBridgePMemPostBuffer: M350
    if (*(BufferPtr + 1)) {
      if (*(BufferPtr + 14)) {
// In DGpuD3Cold.asl
//            CreateDwordField (CpmdGpuBridgePMemPostBuffer, 0,  CpmBridgePMemPostCommand)
//            CreateDwordField (CpmdGpuBridgePMemPostBuffer, 4,  CpmBridgePMemPostBusNumberReg)
//            CreateDwordField (CpmdGpuBridgePMemPostBuffer, 8,  CpmBridgePMemPostNonPrefetchable)
//            CreateDwordField (CpmdGpuBridgePMemPostBuffer, 12, CpmBridgePMemPostLimitBaseLowReg)
//            CreateDwordField (CpmdGpuBridgePMemPostBuffer, 16, CpmBridgePMemPostBaseHighReg)
//            CreateDwordField (CpmdGpuBridgePMemPostBuffer, 20, CpmBridgePMemPostLimitHighReg)
// In AmdCpmDisplayFeatureInitLate.c
//              Buffer[8] = DisplayFeatureDataPtr->GfxBridgePMem[1].BridgeCommand;
//              Buffer[9] = DisplayFeatureDataPtr->GfxBridgePMem[1].BusNumberReg;
//              Buffer[10] = DisplayFeatureDataPtr->GfxBridgePMem[1].BridgeNonPrefetchable;
//              Buffer[11] = DisplayFeatureDataPtr->GfxBridgePMem[1].LimitBaseLowReg;
//              Buffer[12] = DisplayFeatureDataPtr->GfxBridgePMem[1].BaseHighReg;
//              Buffer[13] = DisplayFeatureDataPtr->GfxBridgePMem[1].LimitHighReg;
//              Buffer[14] = 1;
        DEBUG ((DEBUG_INFO, "\t%a(%d) M350 is updated\n", __FUNCTION__, __LINE__));
        *((UINT32*)AmlObjPtr + 2) = *(BufferPtr + 8);
        *((UINT32*)AmlObjPtr + 3) = *(BufferPtr + 9);
        *((UINT32*)AmlObjPtr + 4) = *(BufferPtr + 10);
        *((UINT32*)AmlObjPtr + 5) = *(BufferPtr + 11);
        *((UINT32*)AmlObjPtr + 6) = *(BufferPtr + 12);
        *((UINT32*)AmlObjPtr + 7) = *(BufferPtr + 13);
        *(BufferPtr + 14) = 0;
      }
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '5', '1'):     // ASL code: CpmdGpuBridgeUpStPostBuffer: M351
    if (*(BufferPtr + 1)) {
      if (*(BufferPtr + 21)) {
// In DGpuD3Cold.asl
//            CreateDwordField (CpmdGpuBridgeUpStPostBuffer, 0,  CpmBridgeUpStPostCommand)
//            CreateDwordField (CpmdGpuBridgeUpStPostBuffer, 4,  CpmBridgeUpStPostBusNumberReg)
//            CreateDwordField (CpmdGpuBridgeUpStPostBuffer, 8,  CpmBridgeUpStPostNonPrefetchable)
//            CreateDwordField (CpmdGpuBridgeUpStPostBuffer, 12, CpmBridgeUpStPostLimitBaseLowReg)
//            CreateDwordField (CpmdGpuBridgeUpStPostBuffer, 16, CpmBridgeUpStPostBaseHighReg)
//            CreateDwordField (CpmdGpuBridgeUpStPostBuffer, 20, CpmBridgeUpStPostLimitHighReg)
// In AmdCpmDisplayFeatureInitLate.c
//              Buffer[15] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BridgeCommand;
//              Buffer[16] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BusNumberReg;
//              Buffer[17] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BridgeNonPrefetchable;
//              Buffer[18] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].LimitBaseLowReg;
//              Buffer[19] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].BaseHighReg;
//              Buffer[20] = DisplayFeatureDataPtr->GfxBridgeUpSt[1].LimitHighReg;
//              Buffer[21] = 1;
        DEBUG ((DEBUG_INFO, "\t%a(%d) M351 is updated\n", __FUNCTION__, __LINE__));
        *((UINT32*)AmlObjPtr + 2) = *(BufferPtr + 15);
        *((UINT32*)AmlObjPtr + 3) = *(BufferPtr + 16);
        *((UINT32*)AmlObjPtr + 4) = *(BufferPtr + 17);
        *((UINT32*)AmlObjPtr + 5) = *(BufferPtr + 18);
        *((UINT32*)AmlObjPtr + 6) = *(BufferPtr + 19);
        *((UINT32*)AmlObjPtr + 7) = *(BufferPtr + 20);
        *(BufferPtr + 21) = 0;
      }
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '5', '2'):     // ASL code: CpmdGpuBridgeDnStPostBuffer: M352
    if (*(BufferPtr + 1)) {
      if (*(BufferPtr + 28)) {
// In DGpuD3Cold.asl
//            CreateDwordField (CpmdGpuBridgeDnStPostBuffer, 0,  CpmBridgeDnStPostCommand)
//            CreateDwordField (CpmdGpuBridgeDnStPostBuffer, 4,  CpmBridgeDnStPostBusNumberReg)
//            CreateDwordField (CpmdGpuBridgeDnStPostBuffer, 8,  CpmBridgeDnStPostNonPrefetchable)
//            CreateDwordField (CpmdGpuBridgeDnStPostBuffer, 12, CpmBridgeDnStPostLimitBaseLowReg)
//            CreateDwordField (CpmdGpuBridgeDnStPostBuffer, 16, CpmBridgeDnStPostBaseHighReg)
//            CreateDwordField (CpmdGpuBridgeDnStPostBuffer, 20, CpmBridgeDnStPostLimitHighReg)
// In AmdCpmDisplayFeatureInitLate.c
//              Buffer[22] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BridgeCommand;
//              Buffer[23] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BusNumberReg;
//              Buffer[24] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BridgeNonPrefetchable;
//              Buffer[25] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].LimitBaseLowReg;
//              Buffer[26] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].BaseHighReg;
//              Buffer[27] = DisplayFeatureDataPtr->GfxBridgeDnSt[1].LimitHighReg;
//              Buffer[28] = 1;
        DEBUG ((DEBUG_INFO, "\t%a(%d) M352 is updated\n", __FUNCTION__, __LINE__));
        *((UINT32*)AmlObjPtr + 2) = *(BufferPtr + 22);
        *((UINT32*)AmlObjPtr + 3) = *(BufferPtr + 23);
        *((UINT32*)AmlObjPtr + 4) = *(BufferPtr + 24);
        *((UINT32*)AmlObjPtr + 5) = *(BufferPtr + 25);
        *((UINT32*)AmlObjPtr + 6) = *(BufferPtr + 26);
        *((UINT32*)AmlObjPtr + 7) = *(BufferPtr + 27);
        *(BufferPtr + 28) = 0;
      }
    }
    break;
  case CPM_SIGNATURE_32 ('M', '3', '5', '3'):     // ASL code: CpmdGpuDevicePMemPostBuffer: M353
    if (*(BufferPtr + 1)) {
      if (*(BufferPtr + 34)) {
// In DGpuD3Cold.asl
//            CreateDwordField (CpmdGpuDevicePMemPostBuffer, 0,  CpmDevicePMemPostCommand)
//            CreateDwordField (CpmdGpuDevicePMemPostBuffer, 4,  CpmDevicePMemPostDoorbellLowReg)
//            CreateDwordField (CpmdGpuDevicePMemPostBuffer, 8,  CpmDevicePMemPostDoorbellHighReg)
//            CreateDwordField (CpmdGpuDevicePMemPostBuffer, 12, CpmDevicePMemPostNonPrefetchable)
//            CreateDwordField (CpmdGpuDevicePMemPostBuffer, 16, CpmDevicePMemPostDeviceId)
// In AmdCpmDisplayFeatureInitLate.c
//              Buffer[29] = DisplayFeatureDataPtr->GfxDevicePMem[1].DeviceCommand;
//              Buffer[30] = DisplayFeatureDataPtr->GfxDevicePMem[1].DoorbellLowReg;
//              Buffer[31] = DisplayFeatureDataPtr->GfxDevicePMem[1].DoorbellHighReg;
//              Buffer[32] = DisplayFeatureDataPtr->GfxDevicePMem[1].DeviceNonPrefetchable;
//              Buffer[33] = DisplayFeatureDataPtr->GfxDevicePMem[1].DeviceIdReg;
//              Buffer[34] = 1;
        DEBUG ((DEBUG_INFO, "\t%a(%d) M353 is updated\n", __FUNCTION__, __LINE__));
        *((UINT32*)AmlObjPtr + 2) = *(BufferPtr + 29);
        *((UINT32*)AmlObjPtr + 3) = *(BufferPtr + 30);
        *((UINT32*)AmlObjPtr + 4) = *(BufferPtr + 31);
        *((UINT32*)AmlObjPtr + 5) = *(BufferPtr + 32);
        *((UINT32*)AmlObjPtr + 6) = *(BufferPtr + 33);
        *(BufferPtr + 34) = 0;
      }
    }
    break;
  case CPM_SIGNATURE_32 ('M', '5', '0', '0'):     // ASL code: CpmNonZ10CapablePcieZstateSupport: M500
    if (*(BufferPtr)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        DEBUG ((DEBUG_INFO, "\t%a(%d) M500 is updated\n", __FUNCTION__, __LINE__));
        *(((UINT8*) AmlObjPtr) + 4) = (UINT8)(*(BufferPtr + 35));
      }
    }
    break;
  }
  return FALSE;
}

