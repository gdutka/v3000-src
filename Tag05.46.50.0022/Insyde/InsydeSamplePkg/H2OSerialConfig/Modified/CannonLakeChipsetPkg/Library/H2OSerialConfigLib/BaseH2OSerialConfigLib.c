/** @file
  16550 UART Serial Port library functions

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <Uefi.h>
#include <Library/H2OSerialConfigLib.h>
#include <Library/BaseMemoryLib.h>

//
// Device Path define
//
#define DevicePathType(a)           ( ((a)->Type) )
#define DevicePathSubType(a)        ( ((a)->SubType) )

#define UART_16550_COMPATIBLE_HW_GUID \
  { \
    0xe76fd4e9, 0x0a30, 0x4ca9, {0x95, 0x40, 0xd7, 0x99, 0x53, 0x4c, 0xc4, 0xff} \
  }

STATIC EFI_GUID mH2OUart16550CompatibleHwGuid = UART_16550_COMPATIBLE_HW_GUID;


/**
  This function returns serial device information, based on the usage.

  @param  SerialDevice      On entry, this points to the default serial device information
                            about the serial device as discovered by the caller.
                            On exit, this points to the updated serial device information.

  @retval EFI_UNSUPPORTED   Caller can us the current contents of the returned structure to initialize the device.
  @retval EFI_MEDIA_CHANGED Caller can us the current contents of the returned structure to initialize the device.
  @retval EFI_SUCCESS       Caller don't do any serial port initialization because the output is disabled,
                            or the device is not present.

**/
RETURN_STATUS
EFIAPI
H2OGetSerialDeviceConfig (
  IN OUT H2O_SERIAL_DEVICE *SerialDevice
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  //
  // The Base libary maybe be used in SEC/PEI early phase, it mean some service isn't ready.
  // If you want to use variable or hob service, you should create another library.
  //

  //
  // User specific code
  //
  DevicePath = SerialDevice->DevicePath;
  if ((DevicePathType (DevicePath) == ACPI_DEVICE_PATH) &&
      (DevicePathSubType (DevicePath) == ACPI_DP)) {

    if (((ACPI_HID_DEVICE_PATH *)DevicePath)->UID == 0) {
      SerialDevice->ClockRate = PcdGet32 (PcdSerialIoUartInputClock);
      return EFI_MEDIA_CHANGED;
    }
  } else if ((DevicePathType (DevicePath) == HARDWARE_DEVICE_PATH) &&
             (DevicePathType (DevicePath) == HW_VENDOR_DP) &&
             CompareGuid (&((VENDOR_DEVICE_PATH *)DevicePath)->Guid, &mH2OUart16550CompatibleHwGuid)) {

    if (((MMIO_UART_DEVICE_PATH *)DevicePath)->UID == 0) {
      SerialDevice->ClockRate = PcdGet32 (PcdSerialIoUartInputClock);
      return EFI_MEDIA_CHANGED;
    }
  }

  return EFI_UNSUPPORTED;
}



/**
  This function returns serial attribute information, based on the usage.

  @param  SerialDevice      On entry, this points to the default serial attribute information
                            about the serial device as discovered by the caller.
                            On exit, this points to the updated serial attribute information.

  @retval EFI_UNSUPPORTED   Caller can us the current contents of the returned structure to initialize the device.
  @retval EFI_MEDIA_CHANGED Caller can us the current contents of the returned structure to initialize the device.
  @retval EFI_SUCCESS       Caller don't do any serial port initialization because the output is disabled,
                            or the device is not present.

**/
RETURN_STATUS
EFIAPI
H2OGetSerialAttribsConfig (
  IN OUT H2O_SERIAL_ATTRIBS *SerialAttribs
  )
{
  return EFI_UNSUPPORTED;
}

