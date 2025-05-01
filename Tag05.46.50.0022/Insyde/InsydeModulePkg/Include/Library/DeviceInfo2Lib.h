/** @file
  Device Info 2 Library Definition.
;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _DEVICE_INFO_2_LIB_H_
#define _DEVICE_INFO_2_LIB_H_

#include <Protocol/DevicePath.h>
#include <DeviceInfo.h>

/**
 This function returns basic information about a device.

 @param[in]  TokenNumber        Unsigned integer that specifies the PCD token associated with a device's information.
 @param[out] DevicePath         Pointer to a returned pointer to a device path that indicates the device's device path.
                                The format of the returned device path is consistent with the format specified in the
                                UEFI specification. The device path must not be freed by the caller.
 @param[out] DeviceAttribCount  Pointer to a returned unsigned integer that indicates the number of device attributes
                                associated with the device indicated by DevicePath.

 @retval EFI_SUCCESS            Function returned successfully.
 @retval EFI_INVALID_PARAMETER  DevicePath was NULL or DeviceAttribCount was NULL.
 @retval EFI_VOLUME_CORRUPTED   The format of the device path in the PCD was invalid.
 @retval EFI_NOT_FOUND          There is no device information associated with the specified TokenNumber.
**/
EFI_STATUS
H2OGetDeviceInfo (
  IN  UINTN                     TokenNumber,
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT UINT32                    *DeviceAttribCount
  );

/**
 This function returns a name and value of attribute associated with the device associated with the PCD token number.

 @param[in]  TokenNumber        Unsigned integer that specifies the PCD token associated with a device's information.
 @param[in]  DeviceAttribIndex  Unsigned integer that specifies the index of the device attribute. This value must be
                                less than the value returned by the parameter DeviceAttribCount from GetDeviceInfo.
                                Indices are numbered starting with 0 and continue until the last attribute. There are no gaps.
 @param[out] DeviceAttribName   Pointer to a returned pointer to a null-terminated ASCII string that indicates the name
                                of the device attribute associated with the device.
                                The string value must not be freed by the caller.
 @param[out] DeviceAttribValue  Pointer to a returned pointer to a null-terminated ASCII string that indicates the value
                                of the device attribute associated with the device.
                                The string value must not be freed by the caller.

 @retval EFI_SUCCESS            Function returned successfully.
 @retval EFI_INVALID_PARAMETER  DeviceAttribName was NULL or DeviceAttribValue was NULL.
 @retval EFI_NOT_FOUND          There is no device information associated with the specified TokenNumber.
**/
EFI_STATUS
H2OGetDeviceAttrib (
  IN  UINTN                     TokenNumber,
  IN  UINT32                    DeviceAttribIndex,
  OUT CHAR8                     **DeviceAttribName,
  OUT CHAR8                     **DeviceAttribValue
  );

/**
 This function returns the device attribute value associated with the device associated with the PCD token number
 and the device attribute name.

 @param[in]  TokenNumber        Unsigned integer that specifies the PCD token associated with a device's information.
 @param[in]  DeviceAttribName   Pointer to a null-terminated ASCII string that specifies the name of the device
                                attribute associated with the device.
 @param[out] DeviceAttribValue  Pointer to a returned pointer to a null-terminated ASCII string that indicates the value
                                of the device attribute associated with the device.
                                The string value must not be freed by the caller.

 @retval EFI_SUCCESS            Function returned successfully.
 @retval EFI_INVALID_PARAMETER  DeviceAttribName was NULL or DeviceAttribValue was NULL.
 @retval EFI_NOT_FOUND          There is no device information associated with the specified TokenNumber.
**/
EFI_STATUS
H2OGetDeviceInfoAttribByName (
  IN  UINTN                     TokenNumber,
  IN  CONST CHAR8               *DeviceAttribName,
  OUT CHAR8                     **DeviceAttribValue
  );

/**
 This function update the device path of the device associated with the PCD token number.
 Since the maximum size of PCD is defined at build time and can not be change during runtime,
 caller should make sure that PCD size is enough for updating new device path.

 @param[in]  TokenNumber        Unsigned integer that specifies the PCD token associated with a device's information.
 @param[in]  DevicePath         A pointer to a device path

 @retval EFI_SUCCESS            Update device path to PCD successfully.
 @retval EFI_INVALID_PARAMETER  The input DevicePath is invalid.
 @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
 @retval EFI_NOT_FOUND          There is no device information associated with the specified TokenNumber.
**/
EFI_STATUS
H2OSetDeviceInfoDevicePath (
  IN UINTN                            TokenNumber,
  IN EFI_DEVICE_PATH_PROTOCOL         *DevicePath
  );

/**
 This function updates the attribute value of the device associated with the PCD token number.

 @param[in] TokenNumber         Unsigned integer that specifies the PCD token associated with a device's information.
 @param[in] DeviceAttribName    A pointer to attribute name
 @param[in] DeviceAttribValue   A pointer to attribute value

 @retval EFI_SUCCESS            Update attribute value to PCD successfully.
 @retval EFI_INVALID_PARAMETER  DeviceAttribName or DeviceAttribValue is NULL or TokenNumber is invalid.
 @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
 @retval EFI_NOT_FOUND          Attribute name is not found in PCD.
 @retval Others                 Failed to set PCD to database.
**/
EFI_STATUS
H2OSetDeviceInfoAttribByName (
  IN UINTN                             TokenNumber,
  IN CONST CHAR8                       *DeviceAttribName,
  IN CONST CHAR8                       *DeviceAttribValue
  );

#endif

