/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdFtpmLib.h>
#include <Protocol/AmdFtpmProtocol.h>
#include <Library/AmdHeapLib.h>
#include <Library/PciLib.h>
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci22.h>
#include <Library/SmnAccessLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Gnb.h>
#include <Hsp.h>
#include <Library/HspLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDHSPFTPMDXE_AMDHSPFTPMDXE_FILECODE

#define HSP_PCIE_BAR0_SMN           0x10152010
#define IRQ_PIN_ASSERTION_REGISTER  0x02800020  // IOAPIC::IRQ_PIN_ASSERTION_REGISTER  ( VanGogh and RMB use same address )

#define HSP_MMIO_DECODE_SIG         0x5A5AA5A5

#define HSP_IRQ_AMOUNT              5

#pragma  pack (push, 1)
// HSP Tcg Logs
typedef struct {
  UINTN     BridgeBus;
  UINTN     BridgeDevice;
  UINTN     BridgeFn;
  UINTN     HspBus;
  UINTN     HspDevice;
  UINTN     HspFn;
  UINT32    Flag;                 // 0x5a5aa5a5 means the structure value is valid, others invalid.
  UINT32    BridgeSecBus;
  UINT32    BridgeSubBus;
  UINT32    BridgeMemBaseLimit;
  UINT32    HspMmio;
  UINT16    BridgeCmd;
  UINT16    HspCmd;
} HSP_MMIO_DECODE;
#pragma  pack (pop)


FTPM_INFO         mFtpmInfo;
HSP_MMIO_DECODE   mHspMmioDecode;
BOOLEAN           mIsHspMmioAvailable;

/**
 * @brief   HSP Save Pcie Configuration Space Register Function
 *
 * @param   VOID            VOID
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 *                          EFI_DEVICE_ERROR           Device Error
 *                          EFI_UNSUPPORTED            Function Unsupported
 */
EFI_STATUS
EFIAPI
HSPSavePcieConfigSpaceRegister (
  VOID
)
{
  EFI_STATUS                    Status;
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  HSP_SAVE_PCIE_CONFIG_SPACE    PreHspSavePcieConfigSpace;
  HSP_SAVE_PCIE_CONFIG_SPACE    CurHspSavePcieConfigSpace;
  HSP_SAVE_PCIE_CONFIG_SPACE    *pPreHspPcieCfgSpace;
  HSP_SAVE_PCIE_CONFIG_SPACE    *pCurHspPcieCfgSpace;
  PCI_TYPE_GENERIC              PciTypeGeneric;
  PCI_DEVICE_INDEPENDENT_REGION *PciHdr;
  PCI_BRIDGE_CONTROL_REGISTER   *PciBridgeContrl;
  PCI_DEVICE_HEADER_TYPE_REGION *PciDeviceHdrType;
  UINTN                         VariableSize;

  Status                     = EFI_SUCCESS;
  HandleCount                = 0;
  HandleBuffer               = NULL;
  pPreHspPcieCfgSpace        = &PreHspSavePcieConfigSpace;
  pCurHspPcieCfgSpace        = &CurHspSavePcieConfigSpace;
  PciHdr                     = NULL;
  PciBridgeContrl            = NULL;
  PciDeviceHdrType           = NULL;
  VariableSize               = 0;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.HSPSavePcieConfigSpaceRegister\n");

  ZeroMem ((VOID *) pPreHspPcieCfgSpace, sizeof(HSP_SAVE_PCIE_CONFIG_SPACE));
  ZeroMem ((VOID *) pCurHspPcieCfgSpace, sizeof(HSP_SAVE_PCIE_CONFIG_SPACE));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tLocateHandleBuffer: gEfiPciIoProtocolGuid, Status: %r\n", Status);
    return Status;
  }

  if (HandleBuffer == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHandleBuffer is NULL\n");
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {

    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = PciIo->Pci.Read (
                      PciIo,
                      EfiPciIoWidthUint32,
                      0,
                      sizeof (PCI_TYPE_GENERIC)/sizeof (UINT32),
                      &PciTypeGeneric
                      );
    if (EFI_ERROR (Status)) {
      continue;
    }

    PciHdr = &PciTypeGeneric.Device.Hdr;

    // It is HSP Pcie Brdge
    if ((PciHdr->VendorId == AMD_VID) && (PciHdr->DeviceId == HSP_BRIDGE_DID)) {
      PciBridgeContrl = &PciTypeGeneric.Bridge.Bridge;

      // Check Flag
      pCurHspPcieCfgSpace->CheckFlag                    |= HSP_PCICFG_BRIDGE_UEFI_VARIABLE_CHECK_FLAG;

      // Offset: 0x20 ~ 0x21, MemoryBase
      pCurHspPcieCfgSpace->HspPcieBridgeReg.MemoryBase   = PciBridgeContrl->MemoryBase;

      // Offset: 0x22 ~ 0x23, MemoryBase
      pCurHspPcieCfgSpace->HspPcieBridgeReg.MemoryLimit  = PciBridgeContrl->MemoryLimit;
    }

    // It is HSP Pcie Device
    if ((PciHdr->VendorId == AMD_VID) && (PciHdr->DeviceId == HSP_DEVICE_DID)) {
      PciDeviceHdrType = &PciTypeGeneric.Device.Device;

      // Check Flag
      pCurHspPcieCfgSpace->CheckFlag            |= HSP_PCICFG_DEVICE_UEFI_VARIABLE_CHECK_FLAG;

      // Offset: 0x10 ~ 0x13, BAR0
      pCurHspPcieCfgSpace->HspPcieDeviceReg.Bar0 = PciDeviceHdrType->Bar[0];

    }

    if (pCurHspPcieCfgSpace->CheckFlag == HSP_PCICFG_UEFI_VARIABLE_CHECK_FLAG) {
      break;
    }
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP PCIe Bridge:              \n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCheckFlag             = 0x%08x\n", pCurHspPcieCfgSpace->CheckFlag);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBridge MemoryBase     = 0x%08x\n", pCurHspPcieCfgSpace->HspPcieBridgeReg.MemoryBase);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBridge MemoryLimit    = 0x%08x\n", pCurHspPcieCfgSpace->HspPcieBridgeReg.MemoryLimit);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP PCIe Device:              \n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tDevice Bar0           = 0x%08x\n", pCurHspPcieCfgSpace->HspPcieDeviceReg.Bar0);

  VariableSize = sizeof (HSP_SAVE_PCIE_CONFIG_SPACE);
  Status = gRT->GetVariable (
                  HSP_PCICFG_S3_SAVE_UEFI_VARIABLE_NAME,
                  &gAmdHspPciCfgS3SaveUefiVariableGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) pPreHspPcieCfgSpace
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tGet Previous Variable Status = %r\n", Status);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPrevious CheckFlag           = 0x%08x\n", pPreHspPcieCfgSpace->CheckFlag);

  VariableSize = sizeof (HSP_SAVE_PCIE_CONFIG_SPACE);
  if (CompareMem ((VOID *) pPreHspPcieCfgSpace, (VOID *) pCurHspPcieCfgSpace, VariableSize) != 0) {
    VariableSize = sizeof (HSP_SAVE_PCIE_CONFIG_SPACE);
    Status = gRT->SetVariable (
                    HSP_PCICFG_S3_SAVE_UEFI_VARIABLE_NAME,
                    &gAmdHspPciCfgS3SaveUefiVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    VariableSize,
                    (VOID *) pCurHspPcieCfgSpace
                    );

    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Update Current Pcie Space Variable Status: %r\n", Status);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }

  return Status;
}

/*++

SetPciMmioDecoding

Routine Description:
  Set memory decoding.

Arguments:

Returns:

--*/
STATIC
VOID
SetPciMmioDecoding ()
{
  volatile UINT32       *PtrMmioBase;
  UINT32                MmioBase32;
  UINTN                 BridgeBus;
  UINTN                 BridgeDevice;
  UINTN                 BridgeFn;
  UINTN                 HspTempBus;
  UINTN                 HspDevice;
  UINTN                 HspFn;
  UINT32                Temp32;
  UINT16                Temp16;
  UINT32                HspMmioLength;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.SetPciMmioDecoding\n");

  mHspMmioDecode.Flag = 0;

  MmioBase32    = PcdGet32 (PcdAmdHspMmioBase);
  PtrMmioBase   = (volatile UINT32*)(UINTN)MmioBase32;

  BridgeBus     = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 8 ) & 0xFF;
  BridgeDevice  = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 3 ) & 0x1F;
  BridgeFn      = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 0 ) & 0x07;
  HspTempBus    = 0xFF;
  HspDevice     = ( PcdGet16 (PcdAmdHspBDF) >> 3 ) & 0x1F;
  HspFn         = ( PcdGet16 (PcdAmdHspBDF) >> 0 ) & 0x07;
  Temp32        = 0;
  Temp16        = 0;
  HspMmioLength = 0;

  if (PcdGetBool (PcdAmdHspAcpiFenceEnable) == TRUE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tIt's not HSP PCI device, this routine is skipped\n");
    return;
  }

  if ( mIsHspMmioAvailable ) {
    IDS_HDT_CONSOLE_PSP_TRACE (
      "\t\tHsp Mmio is fine. Just return. 0x%x\n",
      *((volatile UINT32*)(UINTN)MmioBase32)
      );
    return;
  }

  // Check if upstream bridge is a PCI bridge
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_REVISION_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPCI bridge class code is 0x%x\n", Temp32);

  if ( ( Temp32 & 0xFFFFFF00 ) != ( (PCI_CLASS_BRIDGE<<24) | (PCI_CLASS_BRIDGE_P2P<<16) | (PCI_IF_BRIDGE_P2P<<8) ) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Bus %d Dev %d Fn %d is not a PCI bridge. Quit\n", BridgeBus, BridgeDevice, BridgeFn);
    return;
  }

  // Get HSP PCIe device bus number
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBridge Bus Register: 0x%x\n", Temp32);
  HspTempBus = ( Temp32 >> 8 ) & 0xFF;
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP PCIe Bus %d\n", HspTempBus);

  // Check HSP Pcie device
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_VENDOR_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tWe expect VidDiv is 0x%x and we get 0x%x\n", HSP_VIDDID, Temp32);
  if ( Temp32 != HSP_VIDDID ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: It is not HSP device\n");
    return;
  }

  mHspMmioDecode.BridgeBus    = BridgeBus;
  mHspMmioDecode.BridgeDevice = BridgeDevice;
  mHspMmioDecode.BridgeFn     = BridgeFn;
  mHspMmioDecode.HspBus       = HspTempBus;
  mHspMmioDecode.HspDevice    = HspDevice;
  mHspMmioDecode.HspFn        = HspFn;

  // Read and save old HSP BAR0
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_BASE_ADDRESSREG_OFFSET));
  Temp32 &= ~0xF;
  mHspMmioDecode.HspMmio = Temp32;
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tOld HSP BAR0 is 0x%x\n", Temp32);

  // Get HSP Pcie device BAR0 Size
  PciWrite32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_BASE_ADDRESSREG_OFFSET), 0xFFFFFFFF);
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_BASE_ADDRESSREG_OFFSET));
  if ( Temp32 == 0xFFFFFFFF ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("   ERROR: BAR0 return all FF\n");
    return;
  } else {
    HspMmioLength = (UINT32)(~Temp32 + 1);
    IDS_HDT_CONSOLE_PSP_TRACE ("   We get MMIO size 0x%x. (Raw data: 0x%x)\n", HspMmioLength, Temp32);
  }

  // Set HSP BAR0
  PciWrite32 (
    PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_BASE_ADDRESSREG_OFFSET),
    MmioBase32
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back new HSP BAR0 0x%x\n",
    PciRead32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_BASE_ADDRESSREG_OFFSET))
    );

  // Read and save old HSP cmd
  Temp16 = PciRead16 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_COMMAND_OFFSET));
  mHspMmioDecode.HspCmd = Temp16;
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tOld HSP Cmd is 0x%x\n", Temp16);

  // Set HSP Cmd
  PciWrite16 (
    PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_COMMAND_OFFSET),
    Temp16 | ( EFI_PCI_COMMAND_MEMORY_SPACE + EFI_PCI_COMMAND_BUS_MASTER )
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back new HSP Cmd 0x%x\n",
    PciRead16 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_COMMAND_OFFSET))
    );

  // Read Bridge Memory Base & Limit
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, 0x20));
  mHspMmioDecode.BridgeMemBaseLimit = Temp32;
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tOld Upstream Bridge Limit/Base 0x%x\n", Temp32);

  // Set Brdige Memory Base & Limit
  Temp32 = ( MmioBase32 >> 16 ) & 0xFFF0;                                       // Memory Base
  Temp32 |= ( ( ( MmioBase32 + HspMmioLength ) >> 16 ) & 0xFFF0 ) << 16;            // Memory Limit
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tUpstream Bridge Limit/Base 0x%x\n", Temp32);
  PciWrite32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, 0x20), Temp32);
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back Upstream Bridge Limit/Base 0x%x\n",
    PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, 0x20))
    );

  // Read and save old Bridge cmd
  Temp16 = PciRead16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_COMMAND_OFFSET));
  mHspMmioDecode.BridgeCmd = Temp16;
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tOld Bridge Cmd is 0x%x\n", Temp16);

  // Set Bridge Cmd
  Temp16 |= BIT1 + BIT2;
  PciWrite16 (
    PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_COMMAND_OFFSET),
    Temp16
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back new Bridge Cmd 0x%x\n",
    PciRead16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_COMMAND_OFFSET))
    );

  mHspMmioDecode.Flag = HSP_MMIO_DECODE_SIG;

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tTest: *PtrMmioBase = 0x%x\n", *PtrMmioBase);
}


/*++

RestorePciMmioDecoding

Routine Description:
  Restore memory decoding.

Arguments:

Returns:

--*/
STATIC
VOID
RestorePciMmioDecoding ()
{
  volatile UINT32       *PtrMmioBase;
  UINT32                MmioBase32;
  UINTN                 BridgeBus;
  UINTN                 BridgeDevice;
  UINTN                 BridgeFn;
  UINTN                 HspTempBus;
  UINTN                 HspDevice;
  UINTN                 HspFn;
  UINT32                Temp32;
  UINT16                Temp16;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.RestorePciMmioDecoding\n");

  MmioBase32    = PcdGet32 (PcdAmdHspMmioBase);
  PtrMmioBase   = (volatile UINT32*)(UINTN)MmioBase32;

  BridgeBus     = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 8 ) & 0xFF;
  BridgeDevice  = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 3 ) & 0x1F;
  BridgeFn      = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 0 ) & 0x07;
  HspTempBus    = 0x7F;
  HspDevice     = ( PcdGet16 (PcdAmdHspBDF) >> 3 ) & 0x1F;
  HspFn         = ( PcdGet16 (PcdAmdHspBDF) >> 0 ) & 0x07;
  Temp32        = 0;
  Temp16        = 0;

  if (PcdGetBool (PcdAmdHspAcpiFenceEnable) == TRUE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tIt's not HSP PCI device, this routine is skipped\n");
    return;
  }

  if ( mIsHspMmioAvailable ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHsp Mmio is fine. Just return. 0x%x\n", *PtrMmioBase);
    return;
  }

  if ( HSP_MMIO_DECODE_SIG != mHspMmioDecode.Flag ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tmHspMmioDecode.Flag is not 0x%x which means the value is invalid. Skip restore.\n", HSP_MMIO_DECODE_SIG);
    return;
  }

  // Check if upstream bridge is a PCI bridge
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (mHspMmioDecode.BridgeBus, mHspMmioDecode.BridgeDevice, mHspMmioDecode.BridgeFn, PCI_REVISION_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPCI bridge class code is 0x%x\n", Temp32);

  if ( ( Temp32 & 0xFFFFFF00 ) != ( (PCI_CLASS_BRIDGE<<24) | (PCI_CLASS_BRIDGE_P2P<<16) | (PCI_IF_BRIDGE_P2P<<8) ) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Bus %d Dev %d Fn %d is not a PCI bridge. Quit\n", mHspMmioDecode.BridgeBus, mHspMmioDecode.BridgeDevice, mHspMmioDecode.BridgeFn);
    return;
  }

  // Check HSP Pcie device
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (mHspMmioDecode.HspBus, mHspMmioDecode.HspDevice, mHspMmioDecode.HspFn, PCI_VENDOR_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tWe expect VidDiv is 0x%x and we get 0x%x\n", HSP_VIDDID, Temp32);
  if ( Temp32 != HSP_VIDDID ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: It is not HSP device\n");
    return;
  }

  // Restore Bridge Cmd
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRestore Bridge Cmd to value 0x%x\n", mHspMmioDecode.BridgeCmd);
  PciWrite16 (
    PCI_LIB_ADDRESS (mHspMmioDecode.BridgeBus, mHspMmioDecode.BridgeDevice, mHspMmioDecode.BridgeFn, PCI_COMMAND_OFFSET),
    mHspMmioDecode.BridgeCmd
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back Bridge Cmd 0x%x\n",
    PciRead16 (PCI_LIB_ADDRESS (mHspMmioDecode.BridgeBus, mHspMmioDecode.BridgeDevice, mHspMmioDecode.BridgeFn, PCI_COMMAND_OFFSET))
    );

  // Restore Bridge Memory Base & Limit
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRestore Bridge Memory Base & Limit to value 0x%x\n", mHspMmioDecode.BridgeMemBaseLimit);
  PciWrite32 (
    PCI_LIB_ADDRESS (mHspMmioDecode.BridgeBus, mHspMmioDecode.BridgeDevice, mHspMmioDecode.BridgeFn, 0x20),
    mHspMmioDecode.BridgeMemBaseLimit
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back Bridge Memory Base & Limit 0x%x\n",
    PciRead32 (PCI_LIB_ADDRESS (mHspMmioDecode.BridgeBus, mHspMmioDecode.BridgeDevice, mHspMmioDecode.BridgeFn, 0x20))
    );

  // Restore Hsp Cmd
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRestore Hsp Cmd to value 0x%x\n", mHspMmioDecode.HspCmd);
  PciWrite16 (
    PCI_LIB_ADDRESS (mHspMmioDecode.HspBus, mHspMmioDecode.HspDevice, mHspMmioDecode.HspFn, PCI_COMMAND_OFFSET),
    mHspMmioDecode.HspCmd
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back Hsp Cmd 0x%x\n",
    PciRead16 (PCI_LIB_ADDRESS (mHspMmioDecode.HspBus, mHspMmioDecode.HspDevice, mHspMmioDecode.HspFn, PCI_COMMAND_OFFSET))
    );

  // Restore Hsp BAR0
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRestore Hsp BAR0 to value 0x%x\n", mHspMmioDecode.HspMmio);
  PciWrite32 (
    PCI_LIB_ADDRESS (mHspMmioDecode.HspBus, mHspMmioDecode.HspDevice, mHspMmioDecode.HspFn, PCI_BASE_ADDRESSREG_OFFSET),
    mHspMmioDecode.HspMmio
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tRead back Hsp BAR0 0x%x\n",
    PciRead32 (PCI_LIB_ADDRESS (mHspMmioDecode.HspBus, mHspMmioDecode.HspDevice, mHspMmioDecode.HspFn, PCI_BASE_ADDRESSREG_OFFSET))
    );
}


/**
 * @brief   Find HSP PCI device and get its BAR0 value.
 *
 * @param   pBaseAddress    HSP BAR0 value.
 * @return  EFI_STATUS      EFI_SUCCESS              Success
 *                          EFI_INVALID_PARAMETER    Invalid parameters
 *                          EFI_DEVICE_ERROR         HSP device error
 */
STATIC
EFI_STATUS
GetHSPBARAddress (
  IN OUT UINT64  *pBaseAddress
  )
{
  EFI_STATUS            Status;
  UINT32                Temp32;
  UINTN                 BridgeBus;
  UINTN                 BridgeDevice;
  UINTN                 BridgeFn;
  UINTN                 HspTempBus;
  UINTN                 HspDevice;
  UINTN                 HspFn;
  UINT64                HspMmioBase;

  Status        = EFI_SUCCESS;
  Temp32        = 0;
  BridgeBus     = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 8 ) & 0xFF;
  BridgeDevice  = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 3 ) & 0x1F;
  BridgeFn      = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 0 ) & 0x07;
  HspTempBus    = 0;
  HspDevice     = ( PcdGet16 (PcdAmdHspBDF) >> 3 ) & 0x1F;
  HspFn         = ( PcdGet16 (PcdAmdHspBDF) >> 0 ) & 0x07;
  HspMmioBase   = 0;

  if ( pBaseAddress == NULL ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Invalid parameter. Quit\n");
    return EFI_INVALID_PARAMETER;
  }

  // Check if upstream bridge
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_REVISION_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPCI bridge class code is 0x%x\n", Temp32);
  if ( ( Temp32 & 0xFFFFFF00 ) != ( (PCI_CLASS_BRIDGE<<24) | (PCI_CLASS_BRIDGE_P2P<<16) | (PCI_IF_BRIDGE_P2P<<8) ) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Bus %d Dev %d Fn %d is not a PCI bridge. Quit\n", BridgeBus, BridgeDevice, BridgeFn);
    return EFI_DEVICE_ERROR;
  }

  // Get HSP bus number
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDevice, BridgeFn, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
  HspTempBus = ( Temp32 >> 8 ) & 0xFF;
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP bus number 0x%x\n", HspTempBus);

  // Check HSP device
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_VENDOR_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tWe expect VidDiv is 0x%x and we get 0x%x\n", HSP_VIDDID, Temp32);
  if ( Temp32 != HSP_VIDDID ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: It is not HSP device\n");
    return EFI_DEVICE_ERROR;
  }

  // Get HSP BAR0
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspTempBus, HspDevice, HspFn, PCI_BASE_ADDRESSREG_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP BAR0 is 0x%x\n", Temp32);
  if ( Temp32 == 0xFFFFFFFF || Temp32 == 0 ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: HSP BAR0 is not valid.\n");
    return EFI_DEVICE_ERROR;
  }

  *pBaseAddress = (UINT64)(Temp32 & (~(0xF)));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t*pBaseAddress = 0x%lx.\n", *pBaseAddress);

  return (EFI_SUCCESS);
}


/**
 * @brief   Return HSP mailbox base address to SBIOS.
 *          SBIOS should call this procedure after PCI Enumeration Complete.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   FtpmInfo        HSP fTPM information.
 * @return  EFI_STATUS      EFI_SUCCESS              Success
 *                          EFI_NOT_READY            HSP information is not ready
 *                          EFI_INVALID_PARAMETER    Invalid parameters
 */
EFI_STATUS
GetHspfTPMInfo (
  IN     AMD_FTPM_PROTOCOL     *This,
  IN OUT FTPM_INFO             *FtpmInfo
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.GetHspfTPMInfo\n");

  // Validate Input parameters
  if ( NULL == FtpmInfo ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInvalid parameters (HSP.Ftpm.GetHspfTPMInfo)\n");
    return (EFI_INVALID_PARAMETER);
  }

  if ( 0 == mFtpmInfo.HSP_info.TPM_L0_Address ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHsp MMIO base is not ready.\n");
    return (EFI_NOT_READY);
  }

  gBS->CopyMem (FtpmInfo, &mFtpmInfo, sizeof (FTPM_INFO));

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPrint All Base Address:\n", FtpmInfo->HSP_info.TPM_L0_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.TPM_L0_Address         = 0x%lx\n", FtpmInfo->HSP_info.TPM_L0_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.TPM_L0_C2H_MSG_Address = 0x%lx\n", FtpmInfo->HSP_info.TPM_L0_C2H_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.TPM_L0_H2C_MSG_Address = 0x%lx\n", FtpmInfo->HSP_info.TPM_L0_H2C_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT0_Address           = 0x%lx\n", FtpmInfo->HSP_info.VLT0_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT0_C2H_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT0_C2H_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT0_H2C_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT0_H2C_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT1_Address           = 0x%lx\n", FtpmInfo->HSP_info.VLT1_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT1_C2H_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT1_C2H_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT1_H2C_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT1_HSC_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT2_Address           = 0x%lx\n", FtpmInfo->HSP_info.VLT2_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT2_C2H_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT2_C2H_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT2_H2C_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT2_H2C_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT3_Address           = 0x%lx\n", FtpmInfo->HSP_info.VLT3_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT3_C2H_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT3_C2H_MSG_Address);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\tFtpmInfo->HSP_info.VLT3_H2C_MSG_Address   = 0x%lx\n", FtpmInfo->HSP_info.VLT3_H2C_MSG_Address);

  return (EFI_SUCCESS);
}


/**
 * @brief   Return HSP TPM status.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   FtpmStatus      fTPM status.
 * @return  EFI_STATUS      EFI_SUCCESS              HSP fTPM is ready.
 *                          EFI_UNSUPPORTED          HSP fTPM is not ready.
 */
EFI_STATUS
GetHspfTPMStatus (
  IN       AMD_FTPM_PROTOCOL    *This,
  IN OUT   UINTN                *FtpmStatus
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.GetHspfTPMStatus\n");

  SetPciMmioDecoding ();

  Status = FtpmGetInfo (FtpmStatus);

  RestorePciMmioDecoding ();

  return (Status);
}


/**
 * @brief   Send TPM CRB command.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   CommandBuffer   Command buffer address.
 * @param   CommandSize     Command buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Command executed successfully
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_TIMEOUT                Command fail due the time out
 *                          EFI_DEVICE_ERROR           Command fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 */
EFI_STATUS
SendHspfTPMCommand (
  IN     AMD_FTPM_PROTOCOL    *This,
  IN     VOID                 *CommandBuffer,
  IN     UINT32               CommandSize
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.SendHspfTPMCommand\n");

  SetPciMmioDecoding ();

  Status = FtpmSendCommand (
             CommandBuffer,
             CommandSize
             );

  RestorePciMmioDecoding ();

  return (Status);
}


/**
 * @brief   Get TPM CRB command response data.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   ResponseBuffer  buffer address.
 * @param   ResponseSize    buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Command executed successfully
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_TIMEOUT                Command fail due the time out
 *                          EFI_DEVICE_ERROR           Command fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 */
EFI_STATUS
GetHspfTPMResponse (
  IN       AMD_FTPM_PROTOCOL     *This,
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINT32                *ResponseSize
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.GetHspfTPMResponse\n");

  if (!ResponseBuffer) {
    return (EFI_NOT_FOUND);
  }

  SetPciMmioDecoding ();

  Status =  FtpmGetResponse (
              ResponseBuffer,
              ResponseSize
              );

  RestorePciMmioDecoding ();

  return (Status);
}


/**
 * @brief   Get Tcg logs from PSP.
 *           1. Retrieve Tcg logs from AGESA heap.
 *           2. Return Tcg logs to the caller.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   ResponseBuffer  buffer address.
 * @param   ResponseSize    buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 *                          EFI_DEVICE_ERROR           Get Tcg Log fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Input Response Size is too small for Tcg Logs.
 *                          EFI_NOT_FOUND         -    Do not find AGESA heap for Tcg Logs.
 */
EFI_STATUS
GetHspfTPMTcgLogs (
  IN     AMD_FTPM_PROTOCOL     *This,
  IN OUT VOID                  *ResponseBuffer,
  IN OUT UINTN                 *ResponseSize
  )
{
  LOCATE_HEAP_PTR LocateHeapParams;
  PTCGLOGS        pTcgLogs;

  pTcgLogs  = NULL;
  ZeroMem (&LocateHeapParams, sizeof (LOCATE_HEAP_PTR));

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.GetHspfTPMTcgLogs\n");

  LocateHeapParams.BufferHandle = AMD_HSP_GET_TCG_LOGS;
  if ( HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS ) {
    pTcgLogs = (PTCGLOGS)LocateHeapParams.BufferPtr;

    if ( !EFI_ERROR (pTcgLogs->Status) ) {
      if ( 0 == *ResponseSize ) {
        // Caller need the Size, Just return the size
        *ResponseSize = (UINTN)(pTcgLogs->TcgLogsSize);
        return (EFI_SUCCESS);
      } else {
        if ( (UINTN)(pTcgLogs->TcgLogsSize) > *ResponseSize ) {
          IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRespnse buffer size is too small for Tcg logs (HSP.Ftpm.GetHspfTPMTcgLogs)\n");
          return (EFI_BUFFER_TOO_SMALL);
        } else {
          *ResponseSize = (UINTN)(pTcgLogs->TcgLogsSize);
          if ( pTcgLogs->TcgLogsSize ) {
            CopyMem (ResponseBuffer, pTcgLogs + 1 , pTcgLogs->TcgLogsSize);
          }
          return (EFI_SUCCESS);
        }
      }
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPSP show error (HSP.Ftpm.GetHspfTPMTcgLogs)\n");
      return (EFI_DEVICE_ERROR);
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tNo TCG logs (HSP.Ftpm.GetHspfTPMTcgLogs)\n");
    return (EFI_NOT_FOUND);
  }
}


/**
 * @brief   Execute TPM CRB command.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   CommandBuffer   Command buffer address.
 * @param   CommandSize     Command buffer size.
 * @param   ResponseBuffer  Response buffer address.
 * @param   ResponseSize    Response buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Command executed successfully
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_TIMEOUT                Command fail due the time out
 *                          EFI_DEVICE_ERROR           Command fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 */
EFI_STATUS
ExecuteHspfTPMCommand (
  IN       AMD_FTPM_PROTOCOL    *This,
  IN       VOID                 *CommandBuffer,
  IN       UINT32                CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINT32               *ResponseSize
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.DXE.ExecuteHspfTPMCommand\n");

  SetPciMmioDecoding ();

  Status = FtpmExecuteCommand (
             CommandBuffer,
             CommandSize,
             ResponseBuffer,
             ResponseSize
             );

  RestorePciMmioDecoding ();

  return (Status);
}


AMD_FTPM_PROTOCOL mAmdHspfTPMProtocol = {
  ExecuteHspfTPMCommand,
  GetHspfTPMStatus,
  SendHspfTPMCommand,
  GetHspfTPMResponse,
  GetHspfTPMTcgLogs,
  GetHspfTPMInfo
};


/**
 * @brief   Notify function for event group EFI_EVENT_GROUP_READY_TO_BOOT.
 *
 * @param   Event            The registered event
 * @param   Context
 */
VOID
EFIAPI
HSPReadyToBootEventNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS  Status;

  Status      = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.HSPReadyToBootEventNotify\n");

  if (PcdGetBool (PcdAmdHspAcpiFenceEnable) == FALSE) {

    HSPSavePcieConfigSpaceRegister ();

#if HSP_HIDE_PCIE_DEVICE
    // 1. Suppress HSP PCIe device.
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSuppress HSP PCIe device by set NBIFMM::RCC_DEV2_EPF2_STRAP0.BIT[28] to 0.\n");
    ShowHspPciDevice (FALSE, GNB_REG_ACC_FLAG_S3SAVE);
#endif
  }

  // 2. Use x86 VTL0 ( HSP channel #1 ) to send SPI_SET_OUTPUT_IRQ to HSP firmware
  Status= SendIrq2Hsp (
            IRQ_PIN_ASSERTION_REGISTER,
            &(mFtpmInfo.HSP_info.Gsi[0]),
            HSP_IRQ_AMOUNT
            );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP.Send IRQ to HSP firmware %r\n", Status);

  // 3. Set VTL0 TPM Control Areas CommandBufferAddress/Size, ResponseBufferAddress/Size
  if ( PcdGet8 (PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_HSP_FTPM ) {
    SetVTL0TpmBaseSize ();
  }

  gBS->CloseEvent (Event);
}


/**
 * @brief   Notify function for getting HSP base address.
 *          Update base address for ACPI table.
 *
 * @param   Event            The registered event
 * @param   Context
 */
VOID
EFIAPI
HSPPciEnumerationCompleteEventNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS  Status;
  UINT64      HSP_BAR0;
  VOID        *Interface;
  EFI_HANDLE  Handle;

  Status    = EFI_SUCCESS;
  HSP_BAR0  = 0;
  Interface = NULL;
  Handle    = NULL;

  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.HSPPciEnumerationCompleteEventNotify\n");

  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tgEfiPciEnumerationCompleteProtocolGuid is not installed yet.\n");
    return;
  }

  mIsHspMmioAvailable = FALSE;

  Status = GetHSPBARAddress (&HSP_BAR0);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP_BAR0 = 0x%lx\n", HSP_BAR0);

  if ( !EFI_ERROR (Status) ) {
    // Get base address
    mFtpmInfo.HSP_info.TPM_L0_Address         = HSP_BAR0 + GetHspChannelOffset (0x00);
    mFtpmInfo.HSP_info.TPM_L0_C2H_MSG_Address = HSP_BAR0 + GetHspChannelOffset (0x01);
    mFtpmInfo.HSP_info.TPM_L0_H2C_MSG_Address = HSP_BAR0 + GetHspChannelOffset (0x02);

    mFtpmInfo.HSP_info.VLT0_Address           = HSP_BAR0 + GetHspChannelOffset (0x10);
    mFtpmInfo.HSP_info.VLT0_C2H_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x11);
    mFtpmInfo.HSP_info.VLT0_H2C_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x12);

    mFtpmInfo.HSP_info.VLT1_Address           = HSP_BAR0 + GetHspChannelOffset (0x20);
    mFtpmInfo.HSP_info.VLT1_C2H_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x21);
    mFtpmInfo.HSP_info.VLT1_HSC_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x22);

    mFtpmInfo.HSP_info.VLT2_Address           = HSP_BAR0 + GetHspChannelOffset (0x30);
    mFtpmInfo.HSP_info.VLT2_C2H_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x31);
    mFtpmInfo.HSP_info.VLT2_H2C_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x32);

    mFtpmInfo.HSP_info.VLT3_Address           = HSP_BAR0 + GetHspChannelOffset (0x40);
    mFtpmInfo.HSP_info.VLT3_C2H_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x41);
    mFtpmInfo.HSP_info.VLT3_H2C_MSG_Address   = HSP_BAR0 + GetHspChannelOffset (0x42);

    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdHspAcpiProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mFtpmInfo
                    );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Hsp ACPI protocol Status %r\n", Status);
    ASSERT_EFI_ERROR (Status);
  }

  gBS->CloseEvent (Event);
}


/**
 * @brief   Notify function for Pci Io Protocol.
 *          Enable HSP PCIe Device Memory Attribute.
 *
 * @param   Event            The registered event
 * @param   Context
 */
VOID
EFIAPI
HSPPciIoNotify (
  IN  EFI_EVENT        Event,
  IN  VOID             *Context
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  *HandleBuffer;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  EFI_PCI_IO_PROTOCOL         *HspPciIo;
  EFI_PCI_IO_PROTOCOL         *HspParentPciIo;
  UINTN                       HandleCount;
  UINTN                       Index;
  UINT32                      Temp32;
  UINT64                      Temp64;
  UINT32                      HSPBAR0;
  UINT32                      i;

  UINTN                       Seg;
  UINTN                       Bus;
  UINTN                       Dev;
  UINTN                       Fn;

  Status                = EFI_SUCCESS;
  HandleBuffer          = NULL;
  HspPciIo              = NULL;
  HspParentPciIo        = NULL;
  HandleCount           = 0;
  Index                 = 0;
  Temp32                = 0;
  Temp64                = 0;
  HSPBAR0               = 0;
  i                     = 0;
  Seg                   = 0;
  Bus                   = 0;
  Dev                   = 0;
  Fn                    = 0;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.HSPPciIoNotify\n");

  //
  // Get all PCI IO protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tStatus = %r, HandleCount = %d\n", Status, HandleCount);

  //
  // Loop all PCI devices
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID**)&PciIo
                    );
    if ( EFI_ERROR (Status) ) {
      continue;
    }

    if ( HspPciIo == NULL ) {
      // Check if it is HSP device
      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            PCI_VENDOR_ID_OFFSET,
                            1,
                            &Temp32
                            );
      if ( !EFI_ERROR (Status) && ( Temp32 == HSP_VIDDID ) ) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\tFind HSP Device.\n");
        HspPciIo = PciIo;

        // Get HSP BAR0
        Status = HspPciIo->Pci.Read (
                                 HspPciIo,
                                 EfiPciIoWidthUint32,
                                 PCI_BASE_ADDRESSREG_OFFSET,
                                 1,
                                 &HSPBAR0
                                 );
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\tGet HSP BAR0. Status: %r, BAR0: 0x%x\n", Status, HSPBAR0);
        if ( EFI_ERROR (Status) ) {
          HspPciIo = NULL;
        } else {
          HSPBAR0 &= 0xFFFFFFF0;
        }
      }
    }

    if ( HspParentPciIo == NULL ) {
      // Check if it is HSP Upsteam Port
      Status = PciIo->GetLocation (
                        PciIo,
                        &Seg,
                        &Bus,
                        &Dev,
                        &Fn
                        );
      if ( !EFI_ERROR (Status)
        && ( Bus == (( PcdGet16 (PcdAmdHspUpBridgeBDF)>>8)&0xFF) )
        && ( Dev == ((PcdGet16 (PcdAmdHspUpBridgeBDF)>>3)&0x1F) )
        && ( Fn  == ((PcdGet16 (PcdAmdHspUpBridgeBDF)>>0)&0x07) ) ) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\tFind HSP Device Upstream Port.\n");
        HspParentPciIo = PciIo;
      }
    }

    if ( HspPciIo != NULL && HspParentPciIo != NULL ) {
      break;
    }
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }

  if ( HspPciIo && HspParentPciIo ) {
    // Enable HSP PCIe Device Memory decoding.
    Status = HspPciIo->Attributes (
                         HspPciIo,
                         EfiPciIoAttributeOperationSupported,
                         0,
                         &Temp64
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP Supported = 0x%lx\n", Temp64);

    Status = HspPciIo->Attributes (
                         HspPciIo,
                         EfiPciIoAttributeOperationGet,
                         0,
                         &Temp64
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP Attributes = 0x%lx\n", Temp64);


    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tEnable HSP memory attribute.\n");
    Status = HspPciIo->Attributes (
                         HspPciIo,
                         EfiPciIoAttributeOperationEnable,
                         EFI_PCI_IO_ATTRIBUTE_MEMORY,
                         NULL
                         );

    Status = HspPciIo->Attributes (
                         HspPciIo,
                         EfiPciIoAttributeOperationGet,
                         0,
                         &Temp64
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRead HSP Attributes again 0x%lx\n", Temp64);

    // Enable HSP Upstream PCIe Port Memory decoding.
    Status = HspParentPciIo->Attributes (
                         HspParentPciIo,
                         EfiPciIoAttributeOperationSupported,
                         0,
                         &Temp64
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP Upstream PCIe Port Supported = 0x%lx\n", Temp64);

    Status = HspParentPciIo->Attributes (
                         HspParentPciIo,
                         EfiPciIoAttributeOperationGet,
                         0,
                         &Temp64
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP Upstream PCIe Port Attributes = 0x%lx\n", Temp64);


    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tEnable HSP Upstream PCIe Port memory attribute.\n");
    Status = HspParentPciIo->Attributes (
                         HspParentPciIo,
                         EfiPciIoAttributeOperationEnable,
                         EFI_PCI_IO_ATTRIBUTE_MEMORY,
                         NULL
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tStatus: %r\n", Status);

    Status = HspParentPciIo->Attributes (
                         HspParentPciIo,
                         EfiPciIoAttributeOperationGet,
                         0,
                         &Temp64
                         );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRead HSP Upstream PCIe Port Attributes again 0x%lx\n", Temp64);

    // Set mIsHspMmioAvailable to TRUE. So there is no need to set MMIO any more.
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSet mIsHspMmioAvailable to TRUE.\n");
    mIsHspMmioAvailable = TRUE;

    // Update MMIO PCD value
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSet PcdAmdHspMmioBase to 0x%x.\n", HSPBAR0);
    PcdSet32S (PcdAmdHspMmioBase, HSPBAR0);

#if HSP_DEBUG_ENABLE
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRead MMIO 0x%x : 0x%x\n", HSPBAR0, *((volatile UINT32*)(UINTN)HSPBAR0));
#endif

    gBS->CloseEvent (Event);
  }
}


/**
 * @brief   Notify function for End of DXE.
 *
 * @param   Event            The registered event
 * @param   Context

 * @return
 */
VOID
EFIAPI
HSPEndOfDxe (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.HSPEndOfDxe\n");

  mIsHspMmioAvailable = FALSE;

  gBS->CloseEvent (Event);
}

/*++

ConfigureHspAcpiFtpmAddressInfo

Routine Description:
  Configure HSP FTPM address info for HSP ACPI Device.

Arguments:

Returns:

--*/
STATIC
VOID
ConfigureHspAcpiFtpmAddressInfo (
  VOID
  )
{
  UINT64                      HspMmioBase;
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;

  HspMmioBase = (UINT64)PcdGet32 (PcdAmdHspMmioBase);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tConfigureHspAcpiFtpmAddressInfo HspMmioBase:0x%x\n", HspMmioBase);

  mFtpmInfo.HSP_info.TPM_L0_Address         = HspMmioBase + GetHspChannelOffset (0x00);
  mFtpmInfo.HSP_info.TPM_L0_C2H_MSG_Address = HspMmioBase + GetHspChannelOffset (0x01);
  mFtpmInfo.HSP_info.TPM_L0_H2C_MSG_Address = HspMmioBase + GetHspChannelOffset (0x02);

  mFtpmInfo.HSP_info.VLT0_Address           = HspMmioBase + GetHspChannelOffset (0x10);
  mFtpmInfo.HSP_info.VLT0_C2H_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x11);
  mFtpmInfo.HSP_info.VLT0_H2C_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x12);

  mFtpmInfo.HSP_info.VLT1_Address           = HspMmioBase + GetHspChannelOffset (0x20);
  mFtpmInfo.HSP_info.VLT1_C2H_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x21);
  mFtpmInfo.HSP_info.VLT1_HSC_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x22);

  mFtpmInfo.HSP_info.VLT2_Address           = HspMmioBase + GetHspChannelOffset (0x30);
  mFtpmInfo.HSP_info.VLT2_C2H_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x31);
  mFtpmInfo.HSP_info.VLT2_H2C_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x32);

  mFtpmInfo.HSP_info.VLT3_Address           = HspMmioBase + GetHspChannelOffset (0x40);
  mFtpmInfo.HSP_info.VLT3_C2H_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x41);
  mFtpmInfo.HSP_info.VLT3_H2C_MSG_Address   = HspMmioBase + GetHspChannelOffset (0x42);
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdHspAcpiProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mFtpmInfo
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Hsp ACPI protocol Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);
}

/*++

ConfigureHspPciFtpmAddressInfo

Routine Description:
  Configure HSP FTPM address info for HSP PCI Device.

Arguments:

Returns:

--*/
EFI_STATUS
ConfigureHspPciFtpmAddressInfo ()
{
  EFI_STATUS                  Status;
  EFI_EVENT                   Evt;
  VOID                        *Registration;

  Status              = EFI_SUCCESS;
  Evt                 = NULL;
  Registration        = NULL;
  //
  // Register notify function to get HSP PCI MMIO base address.
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall gEfiPciEnumerationCompleteProtocolGuid callback\n");
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_NOTIFY,
    HSPPciEnumerationCompleteEventNotify,
    NULL,
    &Registration
    );

  //
  // Register notify function on gEfiPciIoProtocolGuid.
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall gEfiPciIoProtocolGuid Event.\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  HSPPciIoNotify,
                  NULL,
                  &gEfiPciIoProtocolGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall gEfiPciIoProtocolGuid Event Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRegister gEfiPciIoProtocolGuid Event.\n");
  Status = gBS->RegisterProtocolNotify (
                  &gEfiPciIoProtocolGuid,
                  Evt,
                  &Registration
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRegister gEfiPciIoProtocolGuid Event Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
 * @brief   HSP DXE Driver Entry point.
 *
 * @param   ImageHandle
 * @param   SystemTable
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_DEVICE_ERROR           HSP device error
 */
EFI_STATUS
AmdHspfTPMDxeDriverEntry (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_HANDLE          Handle;
  EFI_EVENT           Evt;
  VOID                *Registration;
  UINT32              TpmCapabilities;

  Status              = EFI_SUCCESS;
  Handle              = NULL;
  Evt                 = NULL;
  Registration        = NULL;
  TpmCapabilities     = 0;

  if ((PcdGetBool (PcdAmdPspEnable) == FALSE) ||
      (PcdGetBool (PcdAmdHspCoreEnable) == FALSE)) {
    return EFI_SUCCESS;
  }

  AGESA_TESTPOINT (TpHspfTpmDxeEntry, NULL);

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.FtpmDxe Enter\n");

  Status = CheckFtpmCaps (&TpmCapabilities);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Get fTPM cap fail\n");
    return Status;
  }
  if ((TpmCapabilities & PSP_CAP_TPM_SUPPORTED) == 0) {
    return EFI_UNSUPPORTED;
  }

  if (!PcdGet32 (PcdAmdHspMmioBase)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: No HSP BAR0 MMIO Base. Something wrong with HSP.\n");
    return EFI_DEVICE_ERROR;
  }

  mIsHspMmioAvailable = TRUE;

  ZeroMem (&mFtpmInfo, sizeof (FTPM_INFO));
  mFtpmInfo.HSP_info.Gsi[0] = 25;  // HSP is expected to use redirection table entry from 25 to 29.
  mFtpmInfo.HSP_info.Gsi[1] = 26;
  mFtpmInfo.HSP_info.Gsi[2] = 27;
  mFtpmInfo.HSP_info.Gsi[3] = 28;
  mFtpmInfo.HSP_info.Gsi[4] = 29;

#if HSP_DEBUG_ENABLE
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPcdGet8 (PcdAmdPspSystemTpmConfig)=0x%x\n", PcdGet8 (PcdAmdPspSystemTpmConfig));
#endif

  // For HSP, BIOS should always report ACPI table for it.
  if (PcdGetBool (PcdAmdHspAcpiFenceEnable) == TRUE) {
    ConfigureHspAcpiFtpmAddressInfo ();
  } else {
    Status = ConfigureHspPciFtpmAddressInfo ();
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Error: Unable to Configure HSP FTPM address\n");
      return Status;
    }
  }

  //
  // Install protocol to pass Ftpm info to rest of BIOS drivers
  //
  if ( PcdGet8 (PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_HSP_FTPM ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall HspTis protocol\n");
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdFtpmProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mAmdHspfTPMProtocol
                    );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall HspTis protocol Status %r\n", Status);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Register notify function on ReadyToBoot Event.
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ReadyToBoot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  HSPReadyToBootEventNotify,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ReadyToBoot Event Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Legacy Boot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  HSPReadyToBootEventNotify,
                  NULL,
                  &gEfiEventLegacyBootGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Legacy Boot Event Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  //
  // Register notify function on End of Dxe.
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRegister notify function on End of Dxe.\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  HSPEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRegister notify function on End of Dxe Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.FtpmDxe Exit\n");
  AGESA_TESTPOINT (TpHspfTpmDxeExit, NULL);

  return Status;
}

