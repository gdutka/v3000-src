/** @file
  This driver will install Uart16550Access protocol on the contorller handle for PCI HS UART device.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "PciHsUartDxe.h"

#define PCI_HSU_DEVICE_NAME_SIZE                    50
#define PCI_HSU_DEVICE_NAME                         L"PCI_HS_UART"
#define PCI_HSU_FIFO_DEPTH                          16
#define PCI_HS_UART_DEVICE_DATA_SIGNATURE           SIGNATURE_32 ('P','I','U','R')
#define PCI_HS_UART_DEVICE_DATA_FROM_THIS(This)     CR (This, PCI_HS_UART_DEVICE_DATA, U16550Access, PCI_HS_UART_DEVICE_DATA_SIGNATURE)

#define UART_READ_SCR(Base, Width) MmioRead8 (Base + (UART_REG_SCR * Width))
#define UART_WRITE_SCR(Base, Width, Data) MmioWrite8 (Base + (UART_REG_SCR * Width), Data);

//
// data type definitions
//
typedef struct {
  UINT32                            Signature;
  EFI_PCI_IO_PROTOCOL               *PciIo;
  UART_16550_DEVICE_INFO            DeviceInfo;
  H2O_UART_16550_ACCESS_PROTOCOL    U16550Access;
} PCI_HS_UART_DEVICE_DATA;

EFI_DRIVER_BINDING_PROTOCOL gHsUartControllerDriver = {
  PciHsUartControllerDriverSupport,
  PciHsUartControllerDriverStart,
  PciHsUartControllerDriverStop,
  0x0A,
  NULL,
  NULL
};

extern EFI_COMPONENT_NAME_PROTOCOL  gPciHsUartComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL gPciHsUartComponentName2;

EFI_STATUS
PciHsUartControllerDriverSupport (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS              Status;
  EFI_PCI_IO_PROTOCOL     *PciIo;
  PCI_TYPE00              PciHeader;
  PCI_HSUART_LIST         *DevList;
  UINTN                   Seg;
  UINTN                   Bus;
  UINTN                   Dev;
  UINTN                   Fun;

  //
  // Test the PciIo Protocol is not be open by driver.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PciIo->GetLocation (PciIo, &Seg, &Bus, &Dev, &Fun);
  if (EFI_ERROR(Status)) {
    goto Exit;
  }

  //
  // Read Pci configuration to Identication the high speed UART
  //
  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof (PciHeader),
                        &PciHeader
                        );

  if (EFI_ERROR(Status)) {
    goto Exit;
  }

  //
  // Check Device ID & PFA support
  //
  DevList = FixedPcdGetPtr (PcdH2OCrPciHsUartDeviceList);

  Status = EFI_UNSUPPORTED;
  while (DevList->VenderID != 0xFFFF) {

    if (DevList->Bus != 0 || DevList->Device != 0 || DevList->Function != 0) {
      //
      // Check PFA
      //
      if (DevList->Bus == Bus && DevList->Device == Dev && DevList->Function == Fun) {
        Status = EFI_SUCCESS;
      } else {
        DevList++;
        continue;
      }
    }

    if (DevList->DeviceID != 0 || DevList->VenderID != 0) {
      //
      // Check Device ID and Vender ID
      //
      if ((DevList->DeviceID == PciHeader.Hdr.DeviceId) && (DevList->VenderID == PciHeader.Hdr.VendorId)) {
        Status = EFI_SUCCESS;
      } else {
        Status = EFI_UNSUPPORTED;
      }
    }

    if (Status == EFI_SUCCESS) {
      break;
    }

    DevList++;
  };

Exit:

  gBS->CloseProtocol (
          Controller,
          &gEfiPciIoProtocolGuid,
          This->DriverBindingHandle,
          Controller
          );

  return Status;
}

/**
  Use scratchpad register to test if this serial port is present.

  @param UartBaseAddress       UART base address
  @param RegisterAccessWidth   Register access width

  @return TRUE   This serial port is present
  @return FALSE  This serial port is not present
**/
BOOLEAN
TestSCR (
  IN UINTN  UartBaseAddress,
  IN UINT8  RegisterAccessWidth
  )
{

  UINT8           Temp;
  BOOLEAN         Status;

  Status = TRUE;

  //
  // Save SCR reg
  //
  Temp = UART_READ_SCR (UartBaseAddress, RegisterAccessWidth);

  UART_WRITE_SCR (UartBaseAddress, RegisterAccessWidth, 0xAA);

  if (UART_READ_SCR (UartBaseAddress, RegisterAccessWidth) != 0xAA) {
    Status = FALSE;
  }

  UART_WRITE_SCR (UartBaseAddress, RegisterAccessWidth, 0x55);

  if (UART_READ_SCR (UartBaseAddress, RegisterAccessWidth) != 0x55) {
    Status = FALSE;
  }

  //
  // Restore SCR
  //
  UART_WRITE_SCR (UartBaseAddress, RegisterAccessWidth, Temp);

  return Status;
}

EFI_STATUS
InitPciHsUartDeviceData (
  PCI_HS_UART_DEVICE_DATA     *HsUartDev,
  EFI_PCI_IO_PROTOCOL         *PciIo
  )
{
  EFI_STATUS                            Status;
  EFI_STATUS                            ChkStatus;
  UART_16550_DEVICE_INFO                *DevInfo;
  PCI_TYPE00                            PciHeader;
  UINTN                                 Segment;
  UINTN                                 Bus;
  UINTN                                 Device;
  UINTN                                 Function;
  UINTN                                 NameSize;
  UINTN                                 BarNum;
  UINT64                                BaseAddress;
  CHAR16                                *Name;
  UINT16                                UID;
  UINTN                                 Index;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR     *Resources;
  UINT8                                 RegisterAccessWidth;

  BarNum = 0;

  //
  // Read pci configuration space for setting some field.
  //
  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof(PciHeader),
                        &PciHeader
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Loop the 6 Bar to find the first Bar that describe Mmio space
  //
  ChkStatus = EFI_UNSUPPORTED;
  for (Index = 0; Index < PCI_MAX_BAR; Index++) {
    Status = PciIo->GetBarAttributes (
                      PciIo,
                      (UINT8) Index,
                      NULL,
                      (VOID**)&Resources
                      );
    if (!EFI_ERROR(Status)) {
      if (Resources->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        BarNum = Index;
        ChkStatus = EFI_SUCCESS;
        break;
      }
    }
  }

  if (ChkStatus != EFI_SUCCESS) {
    return ChkStatus;
  }
  
  if ((PciHeader.Device.Bar[BarNum] & 0x06) == 0x04) {
    //
    // If BarNum equal 5, may cause read out of array PciHeader.Device.Bar[6].
    // For 64bits MMIO bar, it should be consist of BAR[0][1], BAR[1][2], BAR[2][3], BAR[3][4], BAR[4][5]. 
    //
    if (BarNum == 5) {
      return EFI_UNSUPPORTED;
    }
    
    //
    // 64bit Memory mapping IO address
    //
    ASSERT (sizeof (UINTN) == sizeof (UINT64));
    BaseAddress = PciHeader.Device.Bar[BarNum] & 0xFFFFF000;
    BaseAddress += (UINTN) (LShiftU64 ((UINT64)PciHeader.Device.Bar[BarNum + 1], 32));
  } else {
    //
    // 32bit Memory mapping IO address
    //
    BaseAddress = PciHeader.Device.Bar[BarNum] & 0xFFFFFFF0;
  }

  //
  // Test to write SCR register to judge the register access width is DWORD or BYTE
  //
  RegisterAccessWidth = 4;
  if (!TestSCR (BaseAddress, RegisterAccessWidth)) {
    RegisterAccessWidth = 1;
    if (!TestSCR (BaseAddress, RegisterAccessWidth)) {
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Convert PCI location to UID
  //
  Status = PciIo->GetLocation (
                    PciIo,
                    &Segment,
                    &Bus,
                    &Device,
                    &Function
                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  UID = (UINT16)CREATE_UID (Bus, Device, Function);

  //
  // Set Device Name
  //
  NameSize = PCI_HSU_DEVICE_NAME_SIZE;
  Name     = AllocateZeroPool (NameSize);

  UnicodeSPrint (
    Name,
    NameSize,
    L"%s (0x%x,0x%x)",
    PCI_HSU_DEVICE_NAME,
    Device,
    Function
    );
  //
  // Initialize PCI_HS_UART_DEVICE_DATA structure
  //
  HsUartDev->Signature = PCI_HS_UART_DEVICE_DATA_SIGNATURE;
  HsUartDev->PciIo     = PciIo;

  DevInfo = &HsUartDev->DeviceInfo;
  DevInfo->DeviceName        = Name;
  DevInfo->DeviceType        = PCI_HS_SERIAL_DEVICE;
  DevInfo->UID               = UID;
  DevInfo->RegisterByteWidth = RegisterAccessWidth;
  DevInfo->BaseAddressType   = UBAT_MEMORY;
  DevInfo->BaseAddress       = BaseAddress;
  DevInfo->FifoSize          = PCI_HSU_FIFO_DEPTH;
  DevInfo->SerialClockFreq   = (UINTN)FixedPcdGet32(PcdH2OCrHsUartSerialClock);
  DevInfo->SampleRate        = FixedPcdGet16(PcdH2OCrHsUartSampleRate);
  DevInfo->LegacySupport     = TRUE;

  HsUartDev->U16550Access.RegRead    = PciHsUartRegRead;
  HsUartDev->U16550Access.RegWrite   = PciHsUartRegWrite;
  HsUartDev->U16550Access.DeviceInfo = DevInfo;

  return EFI_SUCCESS;
}

EFI_STATUS
PciHsUartControllerDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_HS_UART_DEVICE_DATA   *HsUartDev;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Allocate memory for device Private data
  //
  HsUartDev = AllocateZeroPool(sizeof(PCI_HS_UART_DEVICE_DATA));
  if (HsUartDev == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Error;
  }
  SetMem (HsUartDev, 0, sizeof (PCI_HS_UART_DEVICE_DATA));

  //
  // Initialize the device Private data
  //
  Status = InitPciHsUartDeviceData (HsUartDev, PciIo);
  if (EFI_ERROR(Status)) {
    goto Error;
  }

  //
  // Install Uart16550Access protocol on the contorller handle
  //
  Status = gBS->InstallProtocolInterface (
                  &Controller,
                  &gH2OCrUart16550AccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &HsUartDev->U16550Access
                  );
  if (EFI_ERROR(Status)) {
    goto Error;
  }

  return EFI_SUCCESS;

Error:

  if (HsUartDev != NULL) {
    FreePool (HsUartDev);
  }

  gBS->CloseProtocol (
          Controller,
          &gEfiPciIoProtocolGuid,
          This->DriverBindingHandle,
          Controller
          );
  return Status;
}

EFI_STATUS
PciHsUartControllerDriverStop (
  IN EFI_DRIVER_BINDING_PROTOCOL        *This,
  IN  EFI_HANDLE                        Controller,
  IN  UINTN                             NumberOfChildren,
  IN  EFI_HANDLE                        *ChildHandleBuffer
  )
{
  EFI_STATUS                  Status;
  VOID                        *Protocol;
  PCI_HS_UART_DEVICE_DATA     *UartDevData;

  Status = gBS->OpenProtocol (
                Controller,
                &gH2OCrUart16550AccessProtocolGuid,
                (VOID**)&Protocol,
                This->DriverBindingHandle,
                Controller,
                EFI_OPEN_PROTOCOL_GET_PROTOCOL
                );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  UartDevData = PCI_HS_UART_DEVICE_DATA_FROM_THIS (Protocol);
  Status = gBS->UninstallProtocolInterface (
                Controller,
                &gH2OCrUart16550AccessProtocolGuid,
                &UartDevData->U16550Access
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiPciIoProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  gBS->FreePool (UartDevData);

  return EFI_SUCCESS;
}

/**
 Install Driver to produce UART_16550_ACCESS protocol.

 @param    ImageHandle
 @param    SystemTable

 @retval EFI_SUCCESS            CrPolicy protocol installed
 @return Other                       No protocol installed.

**/
EFI_STATUS
PciHsUartRegRead (
  H2O_UART_16550_ACCESS_PROTOCOL    *This,
  UINT16                            Index,
  UINT8                             *Data
  )
{
  PCI_HS_UART_DEVICE_DATA   *DevData;
  UINT8                     Width;

  Width   = This->DeviceInfo->RegisterByteWidth;
  DevData = PCI_HS_UART_DEVICE_DATA_FROM_THIS (This);
  
  *Data = MmioRead8 (DevData->DeviceInfo.BaseAddress + Index * Width);

  return EFI_SUCCESS;
}

/**
 Install Driver to produce UART_16550_ACCESS protocol.

 @param    ImageHandle
 @param    SystemTable

 @retval EFI_SUCCESS            CrPolicy protocol installed
 @return Other                       No protocol installed.

**/
EFI_STATUS
PciHsUartRegWrite (
  H2O_UART_16550_ACCESS_PROTOCOL    *This,
  UINT16                            Index,
  UINT8                             Data
  )
{
  PCI_HS_UART_DEVICE_DATA   *DevData;
  UINT8                     Width;

  Width   = This->DeviceInfo->RegisterByteWidth;
  DevData = PCI_HS_UART_DEVICE_DATA_FROM_THIS (This);

  MmioWrite8 (DevData->DeviceInfo.BaseAddress + Index * Width, Data);

  return EFI_SUCCESS;
}

/**
 Install Driver to produce UART_16550_ACCESS protocol.

 @param    ImageHandle
 @param    SystemTable

 @retval EFI_SUCCESS            CrPolicy protocol installed
 @return Other                       No protocol installed.

**/
EFI_STATUS
EFIAPI
PciHsUartEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS      Status;

  Status = EfiLibInstallDriverBindingComponentName2 (
               ImageHandle,
               SystemTable,
               &gHsUartControllerDriver,
               ImageHandle,
               &gPciHsUartComponentName,
               &gPciHsUartComponentName2
               );

  ASSERT_EFI_ERROR (Status);

  return Status;

}


