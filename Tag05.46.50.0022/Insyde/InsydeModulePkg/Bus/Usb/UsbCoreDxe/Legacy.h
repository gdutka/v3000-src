/** @file
  Legacy Functions Header for USB

;******************************************************************************
;* Copyright (c) 2012 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _LEGACY_H
#define _LEGACY_H

EFI_STATUS
EFIAPI
GetLegacySupportProvider (
  IN     EFI_USB3_HC_PROTOCOL           **Usb3Hc
  );

EFI_STATUS
EFIAPI
InsertKbcKeyCode (
  IN     UINT8                          QueueSlot,
  IN     UINT8                          *KeyCode,
  IN     UINTN                          Length
  );

EFI_STATUS
EFIAPI
KbcTrapProcessor (
  IN     UINTN                          TrapType,
  IN     UINT8                          Data
  );

EFI_STATUS
EFIAPI
InsertLegacySupportProvider (
  IN     EFI_USB3_HC_PROTOCOL           *Usb3Hc
  );

EFI_STATUS
EFIAPI
RemoveLegacySupportProvider (
  IN     EFI_USB3_HC_PROTOCOL           *Usb3Hc
  );

EFI_STATUS
EFIAPI
InsertPeriodicTimer (
  IN     UINTN                          Type,
  IN     EFI_USB3_HC_CALLBACK           Callback,
  IN     VOID                           *Context,
  IN     UINTN                          MilliSecond,
  IN OUT VOID                           **Handle
  );

EFI_STATUS
EFIAPI
RemovePeriodicTimer (
  IN     VOID                           *Handle
  );

EFI_STATUS
EFIAPI
InsertPeriodicTimerProvider (
  IN     EFI_USB3_HC_PROTOCOL           *Usb3Hc
  );

EFI_STATUS
EFIAPI
RemovePeriodicTimerProvider (
  IN     EFI_USB3_HC_PROTOCOL           *Usb3Hc
  );

EFI_STATUS
EFIAPI
EnterCriticalSection (
  VOID
  );

EFI_STATUS
EFIAPI
LeaveCriticalSection (
  VOID
  );

EFI_STATUS
EFIAPI
ProcessDeferredProcedure (
  );

EFI_STATUS
EFIAPI
DispatchHcCallback (
  IN     EFI_USB3_HC_CALLBACK           Callback,
  IN     UINTN                          Event,
  IN     VOID                           *Context
  );

EFI_STATUS
EFIAPI
RegisterUsbBindingProtocol (
  IN     EFI_USB_BINDING_SUPPORTED      Support,
  IN     EFI_USB_BINDING_START          Start,
  IN     EFI_USB_BINDING_STOP           Stop
  );

EFI_STATUS
EFIAPI 
GetUsbDevices (
  IN     UINTN                          Type,
  IN OUT UINTN                          *Count,
  IN OUT USB_DEVICE                     ***Devices
  );

EFI_STATUS
EFIAPI
InsertUsbDevice (
  IN     USB_DEVICE                     *UsbDevice
  );
  
EFI_STATUS
EFIAPI 
RemoveUsbDevice (
  IN     EFI_USB_IO_PROTOCOL            *UsbIo
  );

EFI_STATUS
EFIAPI
ConnectUsbDevices (
  IN     EFI_USB_IO_PROTOCOL            *UsbIo,
  IN     EFI_DEVICE_PATH_PROTOCOL       *DevicePath
  );

EFI_STATUS
EFIAPI
DisconnectUsbDevices (
  IN     EFI_USB_IO_PROTOCOL            *UsbIo
  );

VOID
EFIAPI
NonSmmCallback (
  IN    EFI_EVENT                       Event,
  IN    VOID                            *Context
  );

EFI_STATUS
EFIAPI
RegisterNonSmmCallback (
  IN     EFI_USB3_HC_CALLBACK           Callback,
  IN     UINTN                          Event,
  IN     VOID                           *Context
  );

VOID
EFIAPI
StartupPeriodicTimerSource (
  );

VOID
EFIAPI
ShutdownPeriodicTimerSource (
  );

VOID
EFIAPI
SwitchPeriodicTimerSource (
  IN     UINTN                          Type
  );

EFI_STATUS
EFIAPI
CheckDeviceDetached (
  IN     EFI_USB_IO_PROTOCOL            *UsbIo
  );

EFI_STATUS
EFIAPI 
InsertAddressConvertTable (
  IN     UINTN                          Act,
  IN     VOID                           *BaseAddress,
  IN     UINTN                          Count
  );

EFI_STATUS
EFIAPI
RemoveAddressConvertTable (
  IN     UINTN                          Act,
  IN     VOID                           *BaseAddress
  );

EFI_STATUS
EFIAPI
AddressConvert (
  IN     UINTN                          Type,
  IN     VOID                           *Address,
  OUT    VOID                           **ConvertedAddress
  );

EFI_STATUS
EFIAPI
AddressPatching (
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerInstalledCallback (
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerGetCapability (
  IN     EFI_USB3_HC_PROTOCOL                   *This,
  OUT    UINT8                                  *MaxSpeed,
  OUT    UINT8                                  *PortNumber,
  OUT    UINT8                                  *Is64BitCapable
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerInterruptTransfer (
  IN     EFI_USB3_HC_PROTOCOL                   *This,
  IN     UINT8                                  DeviceAddress,
  IN     UINT8                                  EndPointAddress,
  IN     UINT8                                  DeviceSpeed,
  IN     UINTN                                  MaximumPacketLength,
  IN OUT VOID                                   *Data,
  IN OUT UINTN                                  *DataLength,
  IN OUT UINT8                                  *DataToggle,
  IN     UINTN                                  TimeOut,
  IN     EFI_USB3_HC_TRANSACTION_TRANSLATOR     *Translator,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK        CallBackFunction OPTIONAL,
  IN     VOID                                   *Context         OPTIONAL,
  IN     UINTN                                  PollingInterval  OPTIONAL,
  OUT    UINT32                                 *TransferResult
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerCancelIo (
  IN     EFI_USB3_HC_PROTOCOL                   *This,
  IN     UINT8                                  DeviceAddress,
  IN     UINT8                                  EndPointAddress,
  IN     UINTN                                  PollingInterval,
  IN OUT UINT8                                  *DataToggle
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerQueryIo (
  IN     EFI_USB3_HC_PROTOCOL                   *This,
  IN     UINT8                                  DeviceAddress,
  IN     UINT8                                  EndPointAddress,
  IN     UINTN                                  PollingInterval
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerRegisterEvent (
  IN     EFI_USB3_HC_PROTOCOL                   *This,
  IN     UINTN                                  EventType,
  IN     EFI_USB3_HC_CALLBACK                   Callback,
  IN     VOID                                   *Context,
  IN OUT VOID                                   **Handle
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerUnregisterEvent (
  IN     EFI_USB3_HC_PROTOCOL                   *This,
  IN     VOID                                   *Handle
  );

EFI_STATUS
EFIAPI
RootPeriodicTimerHandler (
  IN     EFI_HANDLE                             Handle,
  IN     CONST VOID                             *Context,
  IN OUT VOID                                   *CommBuffer,
  IN OUT UINTN                                  *CommBufferSize
  );

#endif