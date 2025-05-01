/** @file
  DXE USB handle auxiliary routines.

;******************************************************************************
;* Copyright 2020 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef USB_HANDLE_ROUTINE_LIB_H_
#define USB_HANDLE_ROUTINE_LIB_H_

#include <Uefi.h>

#include <IndustryStandard/Usb.h>

#include <Protocol/DevicePath.h>
#include <Protocol/UsbIo.h>

/**
  Returns an array of handles containing the matching USB class code.

  @param[in]  DescriptorType  USB Descriptor type that the class code should
                              match against. The only valid values are
                              USB_DESC_TYPE_DEVICE (1) and
                              USB_DESC_TYPE_INTERFACE (4).
                              INTERFACE_ASSOCIATION (11) is not yet supported.
  @param[in]  Class           The base class code to match. Specifying a
                              negative value here matches all classes.
  @param[in]  SubClass        The subclass code to match. Specifying a
                              negative value here matches all subclasses.
  @param[in]  Protocol        The USB Protocol code to match. Specifying a
                              negative value here matches all protocol codes.
  @param[out] NoHandles       The number of handles returned in Buffer.
  @param[out] Buffer          A pointer to the buffer to return the requested
                              array of handles. It is the caller's
                              responsibility to call FreePool() when the caller
                              no longer requires the contents of Buffer.

  @retval EFI_SUCCESS           The array of handles was returned in Buffer,
                                and the number of handles in Buffer was
                                returned in NoHandles. 
  @retval EFI_NOT_FOUND         No handles match the search.
  @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the
                                matching results.
  @retval EFI_INVALID_PARAMETER NoHandles is NULL.
  @retval EFI_INVALID_PARAMETER Buffer is NULL.
  @retval EFI_INVALID_PARAMETER Class, SubClass or Protocol contains a value
                                greater than what's permitted in USB
                                Specification (0xFF).

**/
EFI_STATUS
EFIAPI
LocateUsbHandlesByClassCode (
  IN UINT8        DescriptorType,
  IN INTN         Class,
  IN INTN         SubClass,
  IN INTN         Protocol,
  OUT UINTN       *NoHandles,
  OUT EFI_HANDLE  **Buffer
  );

/**

  @param[in]  Handle

**/
BOOLEAN
IsUsbHubHandle (
  IN EFI_HANDLE  Handle
  );

/**

  @param[in]  UsbIo
  @param[in]  UsbSpecVersion
  @param[out] HubDescriptor
  @param[out] TransferResult

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_TIMEOUT
  @retval EFI_DEVICE_ERROR

**/
EFI_STATUS
GetUsbHubDescriptor (
  IN EFI_USB_IO_PROTOCOL  *UsbIo,
  IN UINT16               UsbSpecVersion OPTIONAL,
  OUT VOID                **HubDescriptor,
  OUT UINT32              *TransferResult
  );

/**

  @param[in]  DevicePath
  @param[out] UsbDevicePathOffset
  @param[out] UsbDevicePathLength

**/
VOID
FindUsbDevicePathNodeAndLength (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINTN                          *UsbDevicePathOffset,
  OUT UINTN                          *UsbDevicePathLength OPTIONAL
  );

/**

  @param[in]  UsbDeviceHandle
  @param[out] NumUsbInterfaces
  @param[out] UsbInterfaceHandles

  @retval  EFI_SUCCESS
  @retval  EFI_UNSUPPORTED       UsbDeviceHandle does not support Device Path
                                 Protocol.
  @retval  EFI_NOT_FOUND
  @retval  EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
LocateUsbInterfaceHandlesOfDevice (
  IN EFI_HANDLE   UsbDeviceHandle,
  OUT UINT8       *NumUsbInterfaces OPTIONAL,
  OUT EFI_HANDLE  **UsbInterfaceHandles OPTIONAL
  );

#endif // USB_HANDLE_ROUTINE_LIB_H_
