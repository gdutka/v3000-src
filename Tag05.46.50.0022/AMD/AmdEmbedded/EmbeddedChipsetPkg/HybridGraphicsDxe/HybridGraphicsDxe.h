/** @file
  This header file is for Hybrid Graphics Feature DXE driver.

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _HYBRID_GRAPHICS_DXE_H_
#define _HYBRID_GRAPHICS_DXE_H_

#include <Guid/HobList.h>
#include <Guid/HgInfoHob.h>
#include <Guid/HybridGraphicsVariable.h>
#include <IndustryStandard/Acpi.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CommonPciLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciExpressLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
// #include <Protocol/PlatformNvsArea.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/LegacyRegion2.h>
#include <Protocol/PciIo.h>
#include <Protocol/HybridGraphicsEvent.h>
#include <Protocol/HybridGraphicsInfo.h>
#include <Protocol/HgDataInfo.h>
#include <IndustryStandard/Pci22.h>
#include <HybridGraphicsDefine.h>

#define MemoryRead16(Address)                 *(UINT16 *)(Address)
#define MemoryRead8(Address)                  *(UINT8 *)(Address)

#pragma pack (push, 1)
typedef struct _HG_DXE_INFORMATION_DATA {
  BINARY_INFORMATION                          Vbios;
  BINARY_INFORMATION                          DgpuMxmBinFile;
  BINARY_INFORMATION                          Dgpu2MxmBinFile;
  UINT8                                       DgpuBridgeBus;
  UINTN                                       DgpuBridgeDevice;
  UINTN                                       DgpuBridgeFunction;
  UINT8                                       Dgpu2BridgeBus;
  UINT8                                       Dgpu2BridgeDevice;
  UINT8                                       Dgpu2BridgeFunction;
  UINT8                                       DgpuBus;
  UINT8                                       Dgpu2Bus;
  UINT16                                      DgpuVendorId;
  UINT16                                      DgpuDeviceId;
  UINT16                                      Dgpu2VendorId;
  UINT8                                       NvDgpuGen;
  UINT8                                       OptimusFlag;
  UINT8                                       AmdSecondaryGrcphicsCommandRegister;
  UINT8                                       NvidiaSecondaryGrcphicsCommandRegister;
  BOOLEAN                                     Dgpu2GpioSupport;
  VOID                                        *HgOpRegionAddress;
} HG_DXE_INFORMATION_DATA;
#pragma pack (pop)

#pragma pack (push, 1)
//
// This structure must match with the structure in AmdSsdt.asl AMD OpRegion "AOPR"
//
typedef struct {
  UINT8       SlaveDgpuSupport;             // Slave dGPU Support or not
  CHAR8       ExpansionMode;                // Expansion Mode
} OPERATION_REGION_AMD;
#pragma pack (pop)

#pragma pack (push, 1)
//
// This structure must match with the structure in NvidiaSsdt.asl NVIDIA OpRegion "NOPR"
//
typedef struct {
  UINT8       DgpuHotPlugSupport;           // Optimus dGPU HotPlug Support or not
  UINT8       DgpuPowerControlSupport;      // Optimus dGPU Power Control Support or not
  UINT8       GpsFeatureSupport;            // GPS Feature Support or not
  UINT8       VenturaFeatureSupport;        // Ventura Feature Support or not
  UINT8       NbciFeatureSupport;           // NBCI Feature Support or not
  UINT8       OptimusGc6Support;            // Optimus GC6 Feature Support or not
  UINT8       OptimusGc6NvsrSupport;        // Optimus GC6 NVSR Feature Support or not
  UINT8       NpcfFeatureSupport;           // NPCF Feature Support or not
  UINT8       N20CTGPFeatureSupport;        // N20 CTGP Feature Support or not  
  UINT8       SlaveDgpuSupport;             // Slave dGPU Support or not
  UINT8       SpbConfig;                    // SpbConfig
  UINT8       ExpansionMode;                // Expansion Mode
  UINT32      MxmBinarySize;                // MXM bin file Size (bits)
  CHAR8       MxmBinaryBuffer[0x1000];      // MXM Bin file 4K
  UINT32      SlaveMxmBinarySize;           // Slave MXM bin file Size (bits)
  CHAR8       SlaveMxmBinaryBuffer[0x1000]; // Slave MXM Bin file 4K
  UINT32      Gc6FbEn;
  UINT32      Gc6Event;
  UINT32      PowerGood;                    // Power Good pin
  UINT32      DdsMuxModePin;                // MUX_MODE_PIN: PCH GPIO for display switch MUX
  UINT32      DdsLcdForceResetPin;          // LCD_FORCE_RESET_PIN: PCH GPIO for LCD force reset pin
} OPERATION_REGION_NVIDIA;
#pragma pack ()

#pragma pack (push, 1)
//
// This structure must match with the structure in NvidiaSsdt.asl NVIDIA OpRegion "NOPR"
//
typedef struct {
  UINT32      RVBS;                         // Runtime VBIOS Image Size
  CHAR8       VBOIS[0x40000];               // VBIOS 256KB
} OPERATION_REGION_VBIOS;
#pragma pack ()

#endif
