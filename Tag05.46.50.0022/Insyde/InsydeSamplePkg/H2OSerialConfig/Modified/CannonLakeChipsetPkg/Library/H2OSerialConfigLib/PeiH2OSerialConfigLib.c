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
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Guid/MemoryAllocationHob.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

//
// Device Path define
//
#define DevicePathType(a)           ( ((a)->Type) )
#define DevicePathSubType(a)        ( ((a)->SubType) )

#pragma pack (1)

typedef struct {
  //
  // User can define specific hob fields
  //
  UINT32                    ClockRate;
  union {
    EFI_DEVICE_PATH_PROTOCOL Header;
    ACPI_HID_DEVICE_PATH     AcpiHid;
    MMIO_UART_DEVICE_PATH    MmioNode;
  } DevicePath;
} H2O_SERIAL_DEVICE_INFO_HOB;

#pragma pack ()

//
// User can define specific customize hob guid
//
#define H2O_SERIAL_DEVICE_INFO_HOB_GUID \
  { \
    0x7e8b3261, 0x533f, 0x4b00, {0x9b, 0xa1, 0x10, 0x5e, 0xe, 0x60, 0x8e, 0x6c } \
  }

STATIC EFI_GUID mH2OSerialDeviceInfoHobGuid = H2O_SERIAL_DEVICE_INFO_HOB_GUID;

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
STATIC
RETURN_STATUS
IntenalH2OGetSerialDeviceConfig (
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
  H2O_SERIAL_DEVICE_INFO_HOB    *DeviceHob;
  EFI_HOB_GUID_TYPE             *GuidHob;
  EFI_STATUS                    Status;
  INTN                          Result;

  if (SerialDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (SerialDevice->DevicePath == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // User specific code begin
  //
  //
  // User specific code end
  //
  DeviceHob = NULL;
  for (GuidHob = GetFirstGuidHob (&mH2OSerialDeviceInfoHobGuid);
       GuidHob != NULL;
       GuidHob = GetNextGuidHob (&mH2OSerialDeviceInfoHobGuid, GET_NEXT_HOB (GuidHob))) {
    DeviceHob = GET_GUID_HOB_DATA (GuidHob);

    //
    // User specific code begin
    //
    if (*(UINT16 *) SerialDevice->DevicePath->Length <= sizeof (DeviceHob->DevicePath)) {
      Result = CompareMem (
                 &DeviceHob->DevicePath,
                 SerialDevice->DevicePath,
                 *(UINT16 *) SerialDevice->DevicePath->Length
                 );

      if (Result == 0) {
        break;
      }
    }
    //
    // User specific code end
    //
  }

  if (GuidHob != NULL) {
    SerialDevice->ClockRate = DeviceHob->ClockRate;
    return EFI_MEDIA_CHANGED;
  }

  Status = IntenalH2OGetSerialDeviceConfig (SerialDevice);
  if (Status != EFI_MEDIA_CHANGED) {
    return Status;
  }

  //
  // User specific code begin
  //
  if (*(UINT16 *) SerialDevice->DevicePath->Length <= sizeof (DeviceHob->DevicePath)) {

    DeviceHob = (H2O_SERIAL_DEVICE_INFO_HOB*) BuildGuidHob (
                                                &mH2OSerialDeviceInfoHobGuid,
                                                sizeof (H2O_SERIAL_DEVICE_INFO_HOB)
                                                );
    //
    // BuildGuidHob
    //
    if (DeviceHob != NULL) {
      CopyMem (
        &DeviceHob->DevicePath,
        SerialDevice->DevicePath,
        *(UINT16 *)SerialDevice->DevicePath->Length
        );
      DeviceHob->ClockRate = SerialDevice->ClockRate;
    }
  }
  //
  // User specific code end
  //


  return EFI_MEDIA_CHANGED;
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

