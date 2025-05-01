/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>

#include <Library/PciLib.h>
#include <IndustryStandard/Pci22.h>

#include <Library/FabricResourceManagerLib.h>

#include <AmdUsb4.h>
#include <AmdUsb4/RingDescriptor.h>
#include <Library/AmdUsb4Lib.h>

#define FILECODE USB4_AMDUSB4COMMON_AMDUSB4COMMON_FILECODE

extern
EFI_STATUS
AmdUsb4AllocateBuffer (
  IN      UINTN                 Size,
      OUT EFI_PHYSICAL_ADDRESS  *Buffer
  );


extern
VOID
AmdUsb4FreeBuffer (
  IN      EFI_PHYSICAL_ADDRESS       Buffer,
  IN      UINTN                      Size
  );


//---------------------------------------
//
//Platform Input -> need further update
//
//---------------------------------------
UINT32  Usb4HostNumber = AMD_USB4_HOST_AMOUNT;
AMD_USB4_HOST_PCI_ADDRESS  Usb4HostList[] = {
                             {0, {0x03, 0x08}, 0, 0, 0, {0x05, 0x00}, FALSE},
                             {0, {0x03, 0x08}, 0, 0, 0, {0x06, 0x00}, FALSE},
                             };

UINTN
AmdUsb4Ring0Size (
  IN     UINT32                  RingNumber
  )
{
  if ( RingNumber == 0 ) {
    return (UINTN)((TX_RING0_SIZE + RX_RING0_SIZE) * (sizeof (AMD_USB4_DESCRIPTOR) + MAX_CONTROL_PACKET_SIZE ));
  } else {
    return 0;
  }
}


/**
  Assign Secondary Bus Number for USB4 upstream Pci Bridge.

  @param  pAmdUsb4Data          AMD USB4 Data Structure.
  @param  Usb4Host              USB4 Host Address Structure.

  @retval EFI_SUCCESS           Set Secondary Bus successfully.
  @retval EFI_INVALID_PARAMETER Do not find Upstream PCI bridge.

**/
EFI_STATUS
AmdUsb4SetHostUpstreamPcieBus (
  IN OUT AMD_USB4_HOST_DATA         *pAmdUsb4Data,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
  )
{
  UINT8         Bus;
  UINT8         Device;
  UINT8         Function;
  UINT32        PciClassCode;
  UINT32        PciDw18SecBus;

  Bus      = Usb4Host->HostPciePortBus;
  Device   = Usb4Host->HostPciePortDevFun.DeviceNum;
  Function = Usb4Host->HostPciePortDevFun.FunctionNum;
  //
  // Check Class code for PCI Bridge
  //
  PciClassCode = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x08));
  PciClassCode &= 0xFFFF0000;

  if (PciClassCode == 0x06040000) {
    // Update Secondary Bus Number for PCI Bridge
    PciDw18SecBus = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x18));
    if ((PciDw18SecBus & 0x0000FF00) != 0) {
      Usb4Host->HostBus = (UINT8) ((PciDw18SecBus & 0x0000FF00) >> 8);
      Usb4Host->HostBusAssigned = TRUE;
      IDS_HDT_CONSOLE (
        MAIN_FLOW,
        "[AmdUsb4SetHostUpstreamPcieBus] USB4 bus number already exist!!! Bus = %x\n",
        Usb4Host->HostBus
        );
      return EFI_SUCCESS;
    } else {
      PciDw18SecBus &= 0xFF0000FF;
      PciDw18SecBus |= ((pAmdUsb4Data->TempPcieBus + AMD_USB4_TEMP_BUS_BASE) << 8) +
                         ((pAmdUsb4Data->TempPcieBus + AMD_USB4_TEMP_BUS_BASE) << 16);
      Usb4Host->HostBus = (UINT8) (pAmdUsb4Data->TempPcieBus + AMD_USB4_TEMP_BUS_BASE);
      PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x18), PciDw18SecBus);
      (pAmdUsb4Data->TempPcieBus)++;
      IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4SetHostUpstreamPcieBus] Set USB4 Bus number = %x\n", Usb4Host->HostBus);
      return EFI_SUCCESS;
    }
  } else {
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
      "[AmdUsb4SetHostUpstreamPcieBus] USB4 Host not on PCIe bridge. [bus, device, function] = [%x, %x, %x]\n",
      Bus,
      Device,
      Function
      );
    return EFI_INVALID_PARAMETER;
  }
}


/**
  Check Class Code for USB4 Host Controller.

  @param  Usb4Host              USB4 Host Address.

  @retval TRUE                  USB4 Host
  @retval FALSE                 NOT USB4 Host

**/
BOOLEAN
AmdUsb4CheckHost (
  IN     AMD_USB4_HOST_PCI_ADDRESS  Usb4Host
  )
{
  UINT32      PciClassCode;

  PciClassCode = PciRead32 (PCI_LIB_ADDRESS (
                              Usb4Host.HostBus,
                              Usb4Host.HostDevFun.DeviceNum,
                              Usb4Host.HostDevFun.FunctionNum,
                              0x08
                              ));
  PciClassCode &= 0xFFFFFF00;

  if (PciClassCode == 0x0C034000) {
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
      "[AmdUsb4CheckHost] Find USB4 Host on bus %x, dev %x, function %x!\n",
      Usb4Host.HostBus,
      Usb4Host.HostDevFun.DeviceNum,
      Usb4Host.HostDevFun.FunctionNum
      );
    return TRUE;
  } else {
    return FALSE;
  }
}


EFI_STATUS
AmdUsb4ResetHostUpstreamPcieBus (
  IN     AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
  )
{
  UINT8         Bus;
  UINT8         Device;
  UINT8         Function;
  UINT32        PciDw18SecBus;

  Bus      = Usb4Host->HostPciePortBus;
  Device   = Usb4Host->HostPciePortDevFun.DeviceNum;
  Function = Usb4Host->HostPciePortDevFun.FunctionNum;

  PciDw18SecBus = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x18));
  PciDw18SecBus &= 0xFF0000FF;
  if (Usb4Host->HostBusAssigned) {
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
      "[AmdUsb4ResetHostUpstreamPcieBus] USB4 bus number already exist!!! Do not reset!!! Bus = %x\n",
      Usb4Host->HostBus
      );
  } else {
    PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x18), PciDw18SecBus);
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4ResetHostUpstreamPcieBus] Reset Bus number!\n");
  }
  return EFI_SUCCESS;
}


/**
  Get USB4 Host Controller list.

  @param  pAmdUsbData           Pointer to AMD USB4 Data.
  @param  Usb4HostNum           USB4 Host number in system.
  @param  Usb4Host              USB Host List.

  @retval EFI_SUCCESS           Find USB4 Host successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.
  @retval EFI_NOT_FOUND         Do not find USB4 Host.

  // 1. Loop for the list pointed by Usb4Host
  // 2. Enable upstream PCIe port/bridge -> set secondary Bus
  // 3. Read PCI register (Bus#Dev#Fun#) to Check USB4 PCI class code 0ch 03h 40h.
  // 4. if not USB4 class code
  //    1) clear up PCI bridge secondary Bus
  //    2) skip to next USB4 Host
  // 5. if Class code match:
  //    1) Init all upstream bridge bus assignment and memory decoding.
  //    2) Assign below 4GB MMIO BAR address for USB4 devices.
  //    3) Enable USB4 device PCI.CS.Command(Offset 0x04).Bus_Master(Bit[2]).
  //  6. go back to step #1 until all controllers in the list
  //  7. Return Usb4 Hosts number and Host list.

**/
EFI_STATUS
AmdUsb4GetUsb4Hosts (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN OUT UINT32                     *Usb4HostNum,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
  )
{
  UINT32                         Usb4HostControllerNumber;
  UINT32                         CurrentUsb4HostIndex;
  AMD_USB4_HOST_PCI_ADDRESS      CurrentUsb4Host;
  AMD_USB4_HOST_PCI_ADDRESS      LocalUsb4Host[AMD_USB4_MAX_HOST_NUM];
  EFI_STATUS                     Status;

  Status = EFI_SUCCESS;
  Usb4HostControllerNumber = *Usb4HostNum;
  pAmdUsbData->Usb4HostNum = 0;
  pAmdUsbData->TempPcieBus = 0;

  ZeroMem (LocalUsb4Host, sizeof (AMD_USB4_HOST_PCI_ADDRESS) * AMD_USB4_MAX_HOST_NUM);

  if (Usb4HostControllerNumber > AMD_USB4_MAX_HOST_NUM) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4GetUsb4Hosts] Too many USB4 Host = %x\n", Usb4HostControllerNumber);
    return EFI_INVALID_PARAMETER;
  }

  if (Usb4Host == NULL) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4GetUsb4Hosts] Invalid input of USB4 Host list!\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Copy USB4 Host list to local array
  //
  CopyMem (LocalUsb4Host, Usb4Host, Usb4HostControllerNumber * sizeof (AMD_USB4_HOST_PCI_ADDRESS));

  //
  // Loop the input USB4 Host List
  //
  for (CurrentUsb4HostIndex = 0; CurrentUsb4HostIndex < Usb4HostControllerNumber; CurrentUsb4HostIndex++) {
    CurrentUsb4Host = LocalUsb4Host[CurrentUsb4HostIndex];

    //
    // Enable Upstream PCIe bus
    //
    Status = AmdUsb4SetHostUpstreamPcieBus (
               pAmdUsbData,
               &CurrentUsb4Host
               );
    if (Status != EFI_SUCCESS) {
      continue;
    }

    //
    // Check Class Code
    //
    if (AmdUsb4CheckHost (CurrentUsb4Host)) {
      //
      // Allocate MMIO resource Later
      //
      Usb4Host[pAmdUsbData->Usb4HostNum] = CurrentUsb4Host;
      (pAmdUsbData->Usb4HostNum)++;
    } else {
      //
      // Reset Upstream Bus
      //
      Status = AmdUsb4ResetHostUpstreamPcieBus (&CurrentUsb4Host);
      if (Status != EFI_SUCCESS) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4GetUsb4Hosts] Restore Upstream Bus Error!\n");
      }
    }
  }

  if (pAmdUsbData->Usb4HostNum == 0) {
    Status = EFI_NOT_FOUND;
  } else {
    *Usb4HostNum = pAmdUsbData->Usb4HostNum;
    Status = EFI_SUCCESS;
  }

  return Status;
}


/**
  Allocate MMIO resource for USB4 Host Controller.

  @param  pAmdUsbData           Pointer to AMD USB4 Data.
  @param  Usb4HostCout          Number of USB Host.
  @param  Usb4MmioBase64        Pointer to return Assigned MMIO base.

  @retval EFI_SUCCESS           Find USB4 Host successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.

**/
EFI_STATUS
AmdUsb4HostAllocateMMIO (
  IN     AMD_USB4_HOST_DATA  *pAmdUsbData,
  IN     UINT8               HostBus,
  IN     UINT8               Usb4HostCount,
     OUT UINT64              *Usb4MmioBase64
  )
{
  UINT64                    Usb4HostMmioBase;
  UINT64                    Length;
  EFI_STATUS                Status;
  FABRIC_TARGET             MmioTarget;
  FABRIC_MMIO_ATTRIBUTE     Attributes;

  if ((Usb4HostCount == 0) || (Usb4MmioBase64 == NULL)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostAllocateMMIO] Invalid Parameter\n");
    return EFI_INVALID_PARAMETER;
  }
  // Allocate MMIO region for USB4 Host
  Usb4HostMmioBase = 0;
  Length = AMD_USB4_HOST_MMIO_SIZE;
  Length = MultU64x32(Length, Usb4HostCount);
  MmioTarget.TgtType = TARGET_PCI_BUS;
  MmioTarget.PciBusNum = HostBus;
  Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
  Status = FabricAllocateMmio (&Usb4HostMmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostAllocateMMIO] Cannot allocate MMIO!!!\n");
    return Status;
  }

  *Usb4MmioBase64 = Usb4HostMmioBase;
  Status = EFI_SUCCESS;
  return Status;
}


/**
  Enable MMIO resource for the bridge which has USB4 Host Controller.

  @param  pAmdUsbData           Pointer to AMD USB4 Data.
  @param  Usb4Host              USB4 Host address.
  @param  Usb4MmioBase          MMIO base.
  @param  MmioSize              MMIO Size.

  @retval EFI_SUCCESS           Find USB4 Host successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.

**/
EFI_STATUS
AmdUsb4HostBridgeEnableMMIO (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN     AMD_USB4_HOST_PCI_ADDRESS  Usb4Host,
  IN     UINT32                     Usb4HostMmioBase,
  IN     UINT32                     MmioSize
  )
{
  UINT8                     Bus;
  UINT8                     Device;
  UINT8                     Function;
  UINT32                    PciBridgeMemBase;
  UINT32                    PciCmdSts;

  if ((Usb4HostMmioBase == 0) || (MmioSize == 0)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostBridgeEnableMMIO] Invalid Parameter\n");
    return EFI_INVALID_PARAMETER;
  }

  // Enable Memory range on upstream bridge
  Bus      = Usb4Host.HostPciePortBus;
  Device   = Usb4Host.HostPciePortDevFun.DeviceNum;
  Function = Usb4Host.HostPciePortDevFun.FunctionNum;
  PciBridgeMemBase = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x20));
  // Do we need to save original value?
  PciBridgeMemBase = (Usb4HostMmioBase >> 16) & 0xFFF0;     // Memory Base
  PciBridgeMemBase |= (((Usb4HostMmioBase + MmioSize) >> 16) & 0xFFF0) << 16; // Memory Limit
  PciWrite32 (
    PCI_LIB_ADDRESS (Bus, Device, Function, 0x20),
    PciBridgeMemBase
    );
  IDS_HDT_CONSOLE (
    MAIN_FLOW,
    "[AmdUsb4HostEnableMMIO] Enable Memory range = %x on Upstream PCIe Bus: %x, Dev: %x, Function: %x\n",
    PciBridgeMemBase,
    Bus,
    Device,
    Function
    );

  // Enable upstream bridge memory decode and BM
  PciCmdSts = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET));
  PciCmdSts |= EFI_PCI_COMMAND_MEMORY_SPACE + EFI_PCI_COMMAND_BUS_MASTER;
  PciWrite32 (
    PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET),
    PciCmdSts
    );

  return EFI_SUCCESS;
}


/**
  Set USB4 PCI device max payload.

  * @param[in]  Bus             PCI Bus number
  * @param[in]  Device          PCI Device number
  * @param[in]  Function        PCI Function number
  * @param[in]  MaxPayload      Max payload value       0 - 128; 1 - 256; 2 - 512; 3 - 1024; 4 - 2048; 5 - 4096
  *
  * @retval VOID

**/
VOID
SetMaxPayload (
  IN       UINTN                 Bus,
  IN       UINTN                 Device,
  IN       UINTN                 Function,
  IN       UINTN                 MaxPayload
  )
{
  UINT32  Temp32;
  UINT16  Temp16;
  UINT8   Offset;

  Temp32  = 0;
  Temp16  = 0;
  Offset  = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Entry\n", __FUNCTION__);

  Temp32 = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_VENDOR_ID_OFFSET));
  if ( 0xFFFFFFFF == Temp32 ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("  PCI Device %x:%x.%x is not accessible\n", Bus, Device, Function);
    return;
  }

  // Find PCI Express Capability
  Offset = (UINT8)PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CAPBILITY_POINTER_OFFSET));
  while ( Offset ) {
    Temp32 = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, (UINTN)Offset));
    if ( ( Temp32 & 0xFF ) == 0x10 ) {
      break;
    } else {
      Offset = (UINT8)(Temp32 >> 8);
    }
  }

  if ( !Offset ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "  PCI Device %x:%x.%x do not have PCI Express Capability\n",
      Bus,
      Device,
      Function
      );
    return;
  } else {
    IDS_HDT_CONSOLE_USB4_INFO (
      "  Find PCI Device %x:%x.%x PCI Express Capability at offset 0x%x\n",
      Bus,
      Device,
      Function,
      Offset
      );
  }

  // Check if this device support max payload size
  // Device Capabilities Register (Offset 04h)
  Temp32 = PciRead16 (PCI_LIB_ADDRESS (Bus, Device, Function, (UINTN)(Offset+4)));
  Temp32 &= 7;
  IDS_HDT_CONSOLE_USB4_INFO (
    "  PCI Device %x:%x.%x Device Capabilities Register Max_Payload_Size Supported is 0x%x,"
    " Parameter Payload Size is 0x%x\n",
    Bus,
    Device,
    Function,
    Temp32,
    MaxPayload
    );
  if ( MaxPayload > (UINTN)Temp32 ) {
    MaxPayload = (UINTN)Temp32;
    IDS_HDT_CONSOLE_USB4_INFO (
      "  Parameter Payload size is large than Max_Payload_Size. We use Max_Payload_Size. 0x%x\n",
      MaxPayload
      );
  }

  Offset += 8;    // Device Control Register (Offset 08h)

  // Set max payload
  Temp16 = PciRead16 (PCI_LIB_ADDRESS (Bus, Device, Function, (UINTN)Offset));
  IDS_HDT_CONSOLE_USB4_INFO ("  PCI Device %x:%x.%x Offset 0x%x = 0x%x\n", Bus, Device, Function, Offset, Temp16);
  Temp16 &= ~(BIT5+BIT6+BIT7);
  Temp16 |= (UINT16)(MaxPayload<<5);
  IDS_HDT_CONSOLE_USB4_INFO ("  Set Offset 0x%x value to 0x%x\n", Offset, Temp16);
  PciWrite32 (
    PCI_LIB_ADDRESS (Bus, Device, Function, (UINTN)Offset),
    Temp16
    );
  IDS_HDT_CONSOLE_USB4_INFO (
    "  Read Back PCI Device %x:%x.%x Offset 0x%x value is 0x%x\n",
    Bus,
    Device,
    Function,
    Offset,
    PciRead16 (PCI_LIB_ADDRESS (Bus, Device, Function, (UINTN)Offset))
    );
}


/**
  Enable MMIO BAR for USB4 Host Controller.

  @param  pAmdUsbData           Pointer to AMD USB4 Data.
  @param  Usb4HostCout          Number of USB Host.
  @param  Usb4MmioBase64        MMIO base.
  @param  MmioSize              MMIO Size.

  @retval EFI_SUCCESS           Find USB4 Host successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.

**/
EFI_STATUS
AmdUsb4HostEnableMMIO (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN     AMD_USB4_HOST_PCI_ADDRESS  Usb4Host,
  IN     UINT32                     Usb4HostMmioBase
  )
{
  UINT8                     Bus;
  UINT8                     Device;
  UINT8                     Function;
  UINT32                    PciCmdSts;

  if (Usb4HostMmioBase == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostEnableMMIO] Invalid Parameter\n");
    return EFI_INVALID_PARAMETER;
  }

  // Assign MMIO to USB4 HOST
  Bus      = Usb4Host.HostBus;
  Device   = Usb4Host.HostDevFun.DeviceNum;
  Function = Usb4Host.HostDevFun.FunctionNum;
  PciWrite32 (
    PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET),
    Usb4HostMmioBase
    );
  IDS_HDT_CONSOLE (
    MAIN_FLOW,
    "[AmdUsb4HostEnableMMIO] Allocate MMIO = %x to USB4 Host on Bus: %x, Dev: %x, Function: %x\n",
    Usb4HostMmioBase,
    Bus,
    Device,
    Function
    );

  // Enable USB4 memory decode and BM
  PciCmdSts = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET));
  PciCmdSts |= EFI_PCI_COMMAND_MEMORY_SPACE + EFI_PCI_COMMAND_BUS_MASTER;
  PciWrite32 (
    PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET),
    PciCmdSts
    );

  // Set USB4 Max Payload to 256 bytes.
  SetMaxPayload (
    Bus,
    Device,
    Function,
    1             // 256 bytes max payload size
    );
  return EFI_SUCCESS;
}


/**
  Set USB4 Host Controller MMIO.

  @param  pAmdUsbData           Pointer to AMD USB4 Data.
  @param  Usb4HostNum           USB4 Host number in system.
  @param  Usb4Host              USB Host List.

  @retval EFI_SUCCESS           Allocate MMIO successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.

  // 1. Loop for the list pointed by Usb4Host
  // 2. Check how many USB4 Host on each bus
  // 3. Loop for Usb4Host list again for each bus
  // 4. Allocate MMIO resource for the bus
  // 5. Allocate MMIO resource for each Host under the bus
  // 6. Return Usb4 Hosts number and Host list.

**/
EFI_STATUS
AmdUsb4InitHostMmio (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN OUT UINT32                     *Usb4HostNum,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
  )
{
  UINT8                          UsbHostBusCount;
  UINT32                         UsbHostBusIndex;
  UINT32                         Usb4HostControllerNumber;
  UINT32                         Usb4HostEnabled;
  UINT32                         CurrentUsbHostIndexOnBus;
  UINT32                         CurrentUsb4HostIndex;
  UINT64                         Usb4HostMmioBase;
  AMD_USB4_HOST_PCI_ADDRESS      LocalUsb4Host[AMD_USB4_MAX_HOST_NUM];
  AMD_USB4_HOST_BUS_INFO         UsbHostBusInfo[AMD_USB4_MAX_HOST_NUM];
  EFI_STATUS                     Status;

  Status = EFI_SUCCESS;
  Usb4HostControllerNumber = *Usb4HostNum;
  CurrentUsbHostIndexOnBus = 0;
  UsbHostBusCount = 0;
  Usb4HostEnabled = 0;

  if (Usb4HostControllerNumber > AMD_USB4_MAX_HOST_NUM) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4InitHostMmio] Too many USB4 Host = %x\n", Usb4HostControllerNumber);
    return EFI_INVALID_PARAMETER;
  }

  if (Usb4HostControllerNumber == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4InitHostMmio]  USB4 Host number = 0!\n");
    return EFI_INVALID_PARAMETER;
  }

  if (Usb4Host == NULL) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4InitHostMmio] Invalid input of USB4 Host list!\n");
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (UsbHostBusInfo, sizeof (AMD_USB4_HOST_BUS_INFO) * AMD_USB4_MAX_HOST_NUM);

  //
  // Copy USB4 Host list to local array
  //
  CopyMem (LocalUsb4Host, Usb4Host, Usb4HostControllerNumber * sizeof (AMD_USB4_HOST_PCI_ADDRESS));

  //
  // Loop the input USB4 Host List for Host Bus
  //   start from first Host
  //
  UsbHostBusInfo[0].HostBus = LocalUsb4Host[0].HostBus;
  UsbHostBusInfo[0].HostCount = 1;

  for (CurrentUsb4HostIndex = 1; CurrentUsb4HostIndex < Usb4HostControllerNumber; CurrentUsb4HostIndex++) {
    //Hosts under same PCIe bridge and same bus need to be listed consecutively
    if (UsbHostBusInfo[UsbHostBusCount].HostBus == LocalUsb4Host[CurrentUsb4HostIndex].HostBus) {
      UsbHostBusInfo[UsbHostBusCount].HostCount++;
    } else {
      UsbHostBusCount++;
      UsbHostBusInfo[UsbHostBusCount].HostBus = LocalUsb4Host[CurrentUsb4HostIndex].HostBus;
      UsbHostBusInfo[UsbHostBusCount].HostCount = 1;
    }
  }

  //
  // Loop for each bus
  //
  UsbHostBusCount      = 0;
  CurrentUsb4HostIndex = 0;
  UsbHostBusIndex      = 0;
  do {
    if (UsbHostBusInfo[UsbHostBusIndex].HostCount == 0){
      IDS_HDT_CONSOLE (
        MAIN_FLOW,
        "[AmdUsb4InitHostMmio] ZERO Host on HostBus %x!\n",
        UsbHostBusInfo[UsbHostBusCount].HostBus
        );
      return EFI_DEVICE_ERROR;
    }
    Status = AmdUsb4HostAllocateMMIO (
               pAmdUsbData,
               LocalUsb4Host[CurrentUsb4HostIndex].HostPciePortBus,
               UsbHostBusInfo[UsbHostBusCount].HostCount,
               &Usb4HostMmioBase
               );
    if (Status == EFI_SUCCESS) {
      Status = AmdUsb4HostBridgeEnableMMIO (
                 pAmdUsbData,
                 LocalUsb4Host[CurrentUsb4HostIndex],
                 (UINT32)Usb4HostMmioBase,
                 AMD_USB4_HOST_MMIO_SIZE * (UINT32)(UsbHostBusInfo[UsbHostBusCount].HostCount)
                 );
      if (Status == EFI_SUCCESS) {
        for (UsbHostBusIndex = 0; UsbHostBusIndex < UsbHostBusInfo[UsbHostBusCount].HostCount; UsbHostBusIndex++) {
          Status = AmdUsb4HostEnableMMIO (
                     pAmdUsbData,
                     LocalUsb4Host[CurrentUsb4HostIndex + UsbHostBusIndex],
                     (UINT32)Usb4HostMmioBase + AMD_USB4_HOST_MMIO_SIZE * UsbHostBusIndex
                     );
          if (Status == EFI_SUCCESS) {
            // Update Usb4Host list
            Usb4Host[Usb4HostEnabled] = LocalUsb4Host[CurrentUsb4HostIndex + UsbHostBusIndex];
            // Update pAmdUsbData
            pAmdUsbData->Usb4HostMmioBase[Usb4HostEnabled] = Usb4HostMmioBase +
                                                             AMD_USB4_HOST_MMIO_SIZE *
                                                             UsbHostBusIndex;
            Usb4HostEnabled++;
            pAmdUsbData->Usb4HostNum = Usb4HostEnabled;
            *Usb4HostNum             = Usb4HostEnabled;
          } else {
            //Cannot enable MMIO for this Host
            IDS_HDT_CONSOLE (
              MAIN_FLOW,
              "[AmdUsb4InitHostMmio] Cannot enable MMIO for USB4 Host on Bus %x, Dev %x, Function %x!\n",
              LocalUsb4Host[CurrentUsb4HostIndex + UsbHostBusIndex].HostBus,
              LocalUsb4Host[CurrentUsb4HostIndex + UsbHostBusIndex].HostDevFun.DeviceNum,
              LocalUsb4Host[CurrentUsb4HostIndex + UsbHostBusIndex].HostDevFun.FunctionNum
              );
          }
        }
      } else {
        //Cannot enable MMIO for this bridge
        IDS_HDT_CONSOLE (
          MAIN_FLOW,
          "[AmdUsb4InitHostMmio] Cannot enable MMIO for Bridge on Bus %x, Dev %x, Function %x!\n",
          LocalUsb4Host[CurrentUsb4HostIndex].HostPciePortBus,
          LocalUsb4Host[CurrentUsb4HostIndex].HostPciePortDevFun.DeviceNum,
          LocalUsb4Host[CurrentUsb4HostIndex].HostPciePortDevFun.FunctionNum
          );
      }
    } else {
      //cannot allocate MMIO for this bus
      IDS_HDT_CONSOLE (
        MAIN_FLOW,
        "[AmdUsb4InitHostMmio] Cannot allocate MMIO for HostBus %x!\n",
        UsbHostBusInfo[UsbHostBusCount].HostBus
        );
    }

    CurrentUsb4HostIndex += (UINT32)UsbHostBusInfo[UsbHostBusCount].HostCount;
    UsbHostBusCount++;
  } while (CurrentUsb4HostIndex < Usb4HostControllerNumber);

  if (Usb4HostEnabled == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: No USB4 MMIO enabled!\n");
    return EFI_NOT_FOUND;
  }

  return Status;
}


/**
  Allocate System Memory for Ring operation.

  @param  pCM              Pointer to the internal CM data structure.
  @param  RingNumber       Ring number that's going to be init.
  @param  TxRingSize       Tx Ring Size in Descriptor.
  @param  RxRingSize       Rx Ring Size in Descriptor.
  @param  TxBufferSize     Tx Data Buffer for each Descriptor in Byte.
  @param  RxBufferSize     Rx Data Buffer for each Descriptor in Byte.

  @retval EFI_SUCCESS           Allocate memory successfully.
  @retval EFI_INVALID_PARAMETER

**/
EFI_STATUS
AmdUsb4AllocateRingMemory (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Buffer;

  Status      = EFI_SUCCESS;
  Buffer      = 0;

  Status = AmdUsb4AllocateBuffer (
             AmdUsb4Ring0Size(RingNumber),
             &Buffer
             );

  if ( EFI_ERROR(Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Allocate memory for Ring.\n");
    return EFI_OUT_OF_RESOURCES;
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Allocate memory for Ring with size 0x%x(%d pages) at address 0x%lx\n",
      AmdUsb4Ring0Size(RingNumber),
      EFI_SIZE_TO_PAGES(AmdUsb4Ring0Size(RingNumber)),
      Buffer
      );
  }

  pCM->RingTxBase[RingNumber]           = (UINT64)(UINTN)Buffer;
  pCM->RingRxBase[RingNumber]           = pCM->RingTxBase[RingNumber] + TX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR);
  pCM->RingTxDataBufferBase[RingNumber] = pCM->RingRxBase[RingNumber] + RX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR);
  pCM->RingRxDataBufferBase[RingNumber] = pCM->RingTxDataBufferBase[RingNumber] + TX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE;

  pCM->RingDevTxBase[RingNumber]           = pCM->RingTxBase[RingNumber];
  pCM->RingDevRxBase[RingNumber]           = pCM->RingRxBase[RingNumber];
  pCM->RingDevTxDataBufferBase[RingNumber] = pCM->RingTxDataBufferBase[RingNumber];
  pCM->RingDevRxDataBufferBase[RingNumber] = pCM->RingRxDataBufferBase[RingNumber];

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Ring[%d]:\nTx Desc Base address: 0x%lx, Size=0x%x\n"
    "Rx Desc Base address: 0x%lx, Size=0x%x\n"
    "Tx Buff Base address: 0x%lx, Size=0x%x\n"
    "Rx Buff Base address: 0x%lx, Size=0x%x\n",
    RingNumber,
    pCM->RingTxBase[RingNumber],
    TX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR),
    pCM->RingRxBase[RingNumber],
    RX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR),
    pCM->RingTxDataBufferBase[RingNumber],
    TX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE,
    pCM->RingRxDataBufferBase[RingNumber],
    RX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE
    );
  return EFI_SUCCESS;
}


/**
  Update USB3/PCIe/DP controller information that is connected
  to USB4 Router. It needs to check Usb4HostList and HostIndex.

  @param  pCM                   Pointer to CM Data with the Host.
  @param  Usb4HostIndex         Index of USB Host.

  @retval EFI_SUCCESS           Find Controller info.
  @retval EFI_NOT_FOUND         Cannot find Controllers info.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.

**/
EFI_STATUS
AmdUsb4HostConnectionInfo (
  IN     AMD_USB4_CM         *pCM,
  IN     UINT32              Usb4HostIndex
  )
{
  CPUID_DATA  CpuId;
  UINT8       Family;
  UINT8       ExtFamily;
  UINT8       Model;
  UINT8       ExtModel;
  UINT8       Stepping;
  EFI_STATUS  Status;

  Family    = 0;
  ExtFamily = 0;
  Model     = 0;
  ExtModel  = 0;
  Stepping  = 0;
  Status    = EFI_NOT_FOUND;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Enter\n", __FUNCTION__);

  if (pCM == NULL) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostConnectionInfo] Invalid Parameter\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check for RMB B0/PHX
  //
  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ( ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x4) && (Model == 0x4)) // RMB B0
    || ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x7)))                  // PHX
  {
    if ((Usb4HostList[Usb4HostIndex].HostPciePortBus == 0)
      && (Usb4HostList[Usb4HostIndex].HostPciePortDevFun.FunctionNum == 3)
      && (Usb4HostList[Usb4HostIndex].HostPciePortDevFun.DeviceNum == 8)
      && (Usb4HostList[Usb4HostIndex].HostDevFun.FunctionNum == 5)) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostConnectionInfo] USB4 RT0\n");
        pCM->Usb3Controller[0].Bus = Usb4HostList[Usb4HostIndex].HostBus;
        pCM->Usb3Controller[0].DevFun.FunctionNum = 3;
        pCM->Usb3Controller[0].DevFun.DeviceNum = 0;

        pCM->PcieController[0].Bus = 0;
        pCM->PcieController[0].DevFun.FunctionNum = 1;
        pCM->PcieController[0].DevFun.DeviceNum = 3;

        return EFI_SUCCESS;
      }

    if ((Usb4HostList[Usb4HostIndex].HostPciePortBus == 0)
      && (Usb4HostList[Usb4HostIndex].HostPciePortDevFun.FunctionNum == 3)
      && (Usb4HostList[Usb4HostIndex].HostPciePortDevFun.DeviceNum == 8)
      && (Usb4HostList[Usb4HostIndex].HostDevFun.FunctionNum == 6)) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "[AmdUsb4HostConnectionInfo] USB4 RT1\n");
        pCM->Usb3Controller[0].Bus = Usb4HostList[Usb4HostIndex].HostBus;
        pCM->Usb3Controller[0].DevFun.FunctionNum = 4;
        pCM->Usb3Controller[0].DevFun.DeviceNum = 0;

        pCM->PcieController[0].Bus = 0;
        pCM->PcieController[0].DevFun.FunctionNum = 1;
        pCM->PcieController[0].DevFun.DeviceNum = 4;

        return EFI_SUCCESS;
      }
  }

  return Status;

}


VOID
AmdUsb4DeAllocateRingMemory (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber
  )
{
  IDS_HDT_CONSOLE_USB4_INFO ("%a Enter\n", __FUNCTION__);

  if ( NULL != pCM->RingTxBase[RingNumber] ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Free memory 0x%x\n", pCM->RingTxBase[RingNumber]);
    AmdUsb4FreeBuffer (
      pCM->RingTxBase[RingNumber],
      AmdUsb4Ring0Size (RingNumber)
      );
    pCM->RingTxBase[RingNumber] = 0;
  }

  IDS_HDT_CONSOLE_USB4_INFO ("%a Exit\n", __FUNCTION__);
}

#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
VOID
WorkaroundForCVTest (
  IN  AMD_USB4_CM     *pCM,
  IN  UINT32          RingNumber
  )
{
  EFI_STATUS    Status;
  UINT32        DataSize;
  UINT32        Data32;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);

  // Register TMU_RTR_CS_3, Field TSPacketInterval, value 0x00000010, is not default (expected: 0x00000000)
  DataSize = 1;
  Data32   = 0;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             0,
             2,
             0,
             0x43,
             &DataSize,
             &Data32
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status %r\n", Status);

  // Register ROUTER_CS_26, Field Opcode, value 0x00000011, is not default (expected: 0x00000000)
  DataSize = 1;
  Data32   = 0;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             0,
             2,
             0,
             26,
             &DataSize,
             &Data32
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status %r\n", Status);

  // Register ROUTER_CS_25, Field Metadata, value 0x00000000, is not default (expected: 0x00000000)
  DataSize = 1;
  Data32   = 0;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             0,
             2,
             0,
             25,
             &DataSize,
             &Data32
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status %r\n", Status);

  // Register ROUTER_CS_4, Field Notification Timeout, value 0x000000FF, is not default (expected: 0x0000000A)
  // Register ROUTER_CS_4, Field Connection Manager USB4 Version _CMUV_, value 0x00000010, is not default (expected: 0x00000000)
  DataSize = 1;
  Data32   = 0xA;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             0,
             2,
             0,
             4,
             &DataSize,
             &Data32
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status %r\n", Status);

  // Register ROUTER_CS_3, Field TopologyID Valid _V_, value 0x00000001, is not default (expected: 0x00000000)
  DataSize = 1;
  Data32   = 0;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             0,
             2,
             0,
             3,
             &DataSize,
             &Data32
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status %r\n", Status);

  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}
#endif

VOID
AmdDisableUsb4Ring (
  IN  AMD_USB4_CM     *pCM,
  IN  UINT32          RingNumber
  )
{
  EFI_STATUS    Status;
  UINT32        TxRingControl;
  UINT32        RxRingControl;

  Status        = EFI_SUCCESS;
  TxRingControl = 0;
  RxRingControl = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);

  if ( pCM == NULL ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   pCM is NULL.\n");
    return;
  }


  TxRingControl = *(volatile UINT32*)(UINTN)(pCM->HIMmioBase + 0x19800 + (RingNumber * 0x20));
  RxRingControl = *(volatile UINT32*)(UINTN)(pCM->HIMmioBase + 0x29800 + (RingNumber * 0x20));

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Tx Ring Control = 0x%x, Rx Ring Control = 0x%x\n",
    TxRingControl,
    RxRingControl
    );

  *(volatile UINT32*)(UINTN)(pCM->HIMmioBase + 0x19800 + (RingNumber * 0x20)) &= (UINT32)(~BIT31);
  *(volatile UINT32*)(UINTN)(pCM->HIMmioBase + 0x29800 + (RingNumber * 0x20)) &= (UINT32)(~BIT31);
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Again Tx Ring Control = 0x%x, Rx Ring Control = 0x%x\n",
    *(volatile UINT32*)(UINTN)(pCM->HIMmioBase + 0x19800 + (RingNumber * 0x20)),
    *(volatile UINT32*)(UINTN)(pCM->HIMmioBase + 0x29800 + (RingNumber * 0x20))
    );

  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}


VOID
AmdUsb4UpperBridgeMMIOSaveRestore (
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *pAmdUsb4HostPciAddress,
  IN     UINT32                     HostAmount,
  IN     BOOLEAN                    Action
  )
{
  UINT32  i;
  UINT32  Bus;
  UINT32  Dev;
  UINT32  Fn;

  i       = 0;
  Bus     = 0;
  Dev     = 0;
  Fn      = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("[%a] Entry with %d, %d\n", __FUNCTION__, Action, HostAmount);

  for (i = 0; i < HostAmount; i++) {
    Bus = pAmdUsb4HostPciAddress[i].HostPciePortBus;
    Dev = pAmdUsb4HostPciAddress[i].HostPciePortDevFun.DeviceNum;
    Fn  = pAmdUsb4HostPciAddress[i].HostPciePortDevFun.FunctionNum;
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "[%a] [%d] Bus: %d, Dev: %d, Fn: %d, 0x00: 0x%x,0x04: 0x%x, 0x18: 0x%x, 0x20: 0x%x, BusAssigned: 0x%x\n",
      __FUNCTION__,
      i,
      Bus,
      Dev,
      Fn,
      PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x00)),
      PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x04)),
      PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x18)),
      PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x20)),
      pAmdUsb4HostPciAddress[i].HostBusAssigned
      );

    if ( Action ) { // Save
      if ( PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x00)) != 0xFFFFFFFF ) {
        pAmdUsb4HostPciAddress[i].HostPciePort04h = PciRead16 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x04));
        pAmdUsb4HostPciAddress[i].HostPciePort20h = PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x20));
        if ( pAmdUsb4HostPciAddress[i].HostPciePort20h != 0 && pAmdUsb4HostPciAddress[i].HostPciePort20h != 0xFFFFFFFF ) {
          pAmdUsb4HostPciAddress[i].HostBusAssigned = TRUE;
        }
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "[%a] [%d] Save Bus: %d, Dev: %d, Fn: %d, 0x04: 0x%x, 0x18: 0x%x, 0x20: 0x%x, BusAssigned: 0x%x\n",
          __FUNCTION__,
          i,
          Bus,
          Dev,
          Fn,
          pAmdUsb4HostPciAddress[i].HostPciePort04h,
          PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x18)),
          pAmdUsb4HostPciAddress[i].HostPciePort20h,
          pAmdUsb4HostPciAddress[i].HostBusAssigned
          );
      }
    } else {        // Restore
      if ( pAmdUsb4HostPciAddress[i].HostBusAssigned ) {
        PciWrite16 (
          PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x04),
          pAmdUsb4HostPciAddress[i].HostPciePort04h
          );
        PciWrite32 (
          PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x20),
          pAmdUsb4HostPciAddress[i].HostPciePort20h
          );
      }
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "[%a] [%d] Restore Bus: %d, Dev: %d, Fn: %d, 0x04: 0x%x, 0x18: 0x%x, 0x20: 0x%x, BusAssigned: 0x%x\n",
        __FUNCTION__,
        i,
        Bus,
        Dev,
        Fn,
        PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x04)),
        PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x18)),
        PciRead32 (PCI_LIB_ADDRESS (Bus, Dev, Fn, 0x20)),
        pAmdUsb4HostPciAddress[i].HostBusAssigned
        );
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("[%a] Exit\n", __FUNCTION__);
}
