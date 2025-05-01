/** @file
  This library class provides helper functions to access the Device Information.

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "DeviceInfo2LibInternal.h"

EFI_GUID mInternalDeviceInfo2ListGuid = {0x7975d1db, 0x33cc, 0x4503, 0x8b, 0x24, 0x24, 0xbe, 0xc1, 0x98, 0x13, 0x57};

/**
 Update device info data from PCDs in gH2ODeviceInfo2TokenSpaceGuid

 @param[in]  TokenNumber        Unsigned integer that specifies the PCD token associated with a device's information.
 @param[in, out] DevInfo        Pointer to a device info data list which will be updated from PCD data.

 @retval EFI_SUCCESS            Update device info from PCD successfully.
 @retval EFI_INVALID_PARAMETER  DevInfo is not for the PCD of TokenNumber
 @retval EFI_ABORTED            Can not find valid PCD by the input TokenNumber
 @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
**/
STATIC
EFI_STATUS
UpdateDevInfofromPcdData (
  IN UINTN                             TokenNumber,
  IN OUT H2O_DEVICE_INFO_PCD_DATA      *DevInfo
  )
{
  UINTN                                DevicePathSize;
  UINTN                                PcdDataSize;
  CHAR8                                *PcdData;
  UINT32                               AttribCount;
  UINTN                                Offset;
  CHAR8                                *AttribPtr;
  CHAR8                                *ValuePtr;

  DevInfo->PcdTokenNum = TokenNumber;

  //
  // The PCD data consists of one or more null-terminated ASCII strings.
  // The first element specify the binary-encoded device path of the device.
  // The second and following strings specify an attribute for that device in the format attribute=value.
  //
  PcdDataSize = LibPcdGetExSize (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  PcdData     = LibPcdGetExPtr (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  if (PcdData == NULL || PcdDataSize <= sizeof ("") || PcdData[PcdDataSize - 1] != CHAR_NULL) {
    return EFI_ABORTED;
  }

  if (DevInfo->DevPath != NULL && DevInfo->DevPathBufferSize < PcdDataSize) {
    FreePool (DevInfo->DevPath);
    DevInfo->DevPath = NULL;
  }

  if (DevInfo->DevPath == NULL) {
    PcdData = AllocateCopyPool (PcdDataSize, PcdData);
    if (PcdData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    DevInfo->DevPath = (EFI_DEVICE_PATH_PROTOCOL *)PcdData;
    DevInfo->DevPathBufferSize = PcdDataSize;
  } else {
    CopyMem (DevInfo->DevPath, PcdData, PcdDataSize);
  }

  DevicePathSize = GetDevicePathSize (DevInfo->DevPath);
  AttribCount    = 0;
  Offset         = DevicePathSize;
  while (Offset < PcdDataSize) {
    Offset += AsciiStrSize ((CHAR8 *)DevInfo->DevPath + Offset);
    AttribCount++;
  }
  if (AttribCount == 0) {
    return EFI_SUCCESS;
  }

  if ((DevInfo->AttribCount != AttribCount)) {
    if (DevInfo->AttribAsciiStrPtrArray != NULL) {
      FreePool(DevInfo->AttribAsciiStrPtrArray);
    }
    if (DevInfo->ValueAsciiStrPtrArray != NULL) {
      FreePool(DevInfo->ValueAsciiStrPtrArray);
    }
    DevInfo->AttribAsciiStrPtrArray = AllocateZeroPool (AttribCount * sizeof (CHAR8 *));
    DevInfo->ValueAsciiStrPtrArray  = AllocateZeroPool (AttribCount * sizeof (CHAR8 *));
    if (DevInfo->AttribAsciiStrPtrArray == NULL ||
        DevInfo->ValueAsciiStrPtrArray  == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  AttribCount = 0;
  Offset      = DevicePathSize;
  while (Offset < PcdDataSize) {
    AttribPtr = (CHAR8 *)DevInfo->DevPath + Offset;
    Offset += AsciiStrSize ((CHAR8 *)DevInfo->DevPath + Offset);

    ValuePtr = AsciiStrStr (AttribPtr, "=");
    ASSERT (ValuePtr != NULL);
    if (ValuePtr == NULL) {
      continue;
    }
    *ValuePtr = CHAR_NULL;
    ValuePtr++;

    DevInfo->AttribAsciiStrPtrArray[AttribCount] = AttribPtr;
    DevInfo->ValueAsciiStrPtrArray[AttribCount]  = ValuePtr;
    AttribCount++;
  }
  DevInfo->AttribCount = AttribCount;

  return EFI_SUCCESS;
}

/**
 Enumerate all device info PCDs in gH2ODeviceInfo2TokenSpaceGuid.

 @param[out] DevInfoList        Pointer to a returned pointer to a device info data list

 @retval EFI_SUCCESS            Enumerate all device info PCDs successfully.
 @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
 @retval Other                  Fail to locate EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL.
**/
EFI_STATUS
EnumerateAllDevInfoPcd (
  OUT H2O_DEVICE_INFO_PCD_DATA         **DevInfoList
  )
{
  UINTN                                TokenNumber;
  UINTN                                PcdDataSize;
  CHAR8                                *PcdData;
  UINT32                               PcdCount;

  for (TokenNumber  = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, 0), PcdCount = 0;
       TokenNumber != 0;
       TokenNumber  = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber)) {
    if (LibPcdGetExSize (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber) <= sizeof ("")) {
      continue;
    }
    PcdCount++;
  }
  if (PcdCount == 0) {
    *DevInfoList = NULL;
    return EFI_SUCCESS;
  }

  *DevInfoList = AllocateZeroPool ((PcdCount + 1) * sizeof (H2O_DEVICE_INFO_PCD_DATA));
  if (*DevInfoList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (TokenNumber  = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, 0), PcdCount = 0;
       TokenNumber != 0;
       TokenNumber  = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber)) {
    PcdDataSize = LibPcdGetExSize (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
    PcdData     = LibPcdGetExPtr (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
    if (PcdData == NULL || PcdDataSize <= sizeof ("") || PcdData[PcdDataSize - 1] != CHAR_NULL) {
      continue;
    }

    UpdateDevInfofromPcdData (TokenNumber, &((*DevInfoList)[PcdCount++]));
  }

  return EFI_SUCCESS;
}

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
  IN  UINTN                            TokenNumber,
  OUT EFI_DEVICE_PATH_PROTOCOL         **DevicePath,
  OUT UINT32                           *DeviceAttribCount
  )
{
  H2O_DEVICE_INFO_PCD_DATA             *DevInfoPcdData;
  EFI_DEVICE_PATH_PROTOCOL             *PcdData;

  if (DevicePath == NULL || DeviceAttribCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevInfoPcdData = GetDevInfoPcdData (TokenNumber);
  if (DevInfoPcdData == NULL) {
    return EFI_NOT_FOUND;
  }

  if (DevInfoPcdData->DevPath == NULL) {
    return EFI_VOLUME_CORRUPTED;
  }

  //
  // Because device path in PCD may be updated, check if need to update Device info data from PCD.
  //
  PcdData = LibPcdGetExPtr (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  if (PcdData != NULL &&
      (GetDevicePathSize (PcdData) != GetDevicePathSize (DevInfoPcdData->DevPath) ||
       CompareMem (PcdData, DevInfoPcdData->DevPath, GetDevicePathSize (DevInfoPcdData->DevPath))!= 0)) {
    UpdateDevInfofromPcdData (TokenNumber, DevInfoPcdData);
  }

  *DevicePath        = DevInfoPcdData->DevPath;
  *DeviceAttribCount = DevInfoPcdData->AttribCount;

  return EFI_SUCCESS;
}

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
  IN  UINTN                            TokenNumber,
  IN  UINT32                           DeviceAttribIndex,
  OUT CHAR8                            **DeviceAttribName,
  OUT CHAR8                            **DeviceAttribValue
  )
{
  H2O_DEVICE_INFO_PCD_DATA             *DevInfoPcdData;

  if (DeviceAttribName == NULL || DeviceAttribValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevInfoPcdData = GetDevInfoPcdData (TokenNumber);
  if (DevInfoPcdData == NULL) {
    return EFI_NOT_FOUND;
  }

  if (DevInfoPcdData->AttribCount <= DeviceAttribIndex ||
      DevInfoPcdData->AttribAsciiStrPtrArray == NULL   ||
      DevInfoPcdData->ValueAsciiStrPtrArray  == NULL) {
    return EFI_NOT_FOUND;
  }

  *DeviceAttribName  = DevInfoPcdData->AttribAsciiStrPtrArray[DeviceAttribIndex];
  *DeviceAttribValue = DevInfoPcdData->ValueAsciiStrPtrArray[DeviceAttribIndex];
  return EFI_SUCCESS;
}

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
  IN  UINTN                            TokenNumber,
  IN  CONST CHAR8                      *DeviceAttribName,
  OUT CHAR8                            **DeviceAttribValue
  )
{
  H2O_DEVICE_INFO_PCD_DATA             *DevInfoPcdData;
  UINT32                               Index;

  if (DeviceAttribName == NULL || DeviceAttribValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevInfoPcdData = GetDevInfoPcdData (TokenNumber);
  if (DevInfoPcdData == NULL) {
    return EFI_NOT_FOUND;
  }

  if (DevInfoPcdData->AttribAsciiStrPtrArray == NULL ||
      DevInfoPcdData->ValueAsciiStrPtrArray  == NULL) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; Index < DevInfoPcdData->AttribCount; Index++) {
    if (AsciiStrCmp (DevInfoPcdData->AttribAsciiStrPtrArray[Index], DeviceAttribName) == 0) {
      *DeviceAttribValue = DevInfoPcdData->ValueAsciiStrPtrArray[Index];
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

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
  )
{
  EFI_STATUS                           Status;
  H2O_DEVICE_INFO_PCD_DATA             *DevInfoPcdData;
  UINTN                                DevicePathSize;
  UINTN                                PcdDataSize;
  CHAR8                                *PcdData;
  CHAR8                                *NewPcdData;
  UINTN                                NewPcdDataSize;
  UINTN                                PcdDevPathSize;
  UINTN                                PcdAttrAsciiStrSize;

  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevicePathSize = GetDevicePathSize (DevicePath);
  if (DevicePathSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  DevInfoPcdData = GetDevInfoPcdData (TokenNumber);
  if (DevInfoPcdData == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Update PCD first. If PCD update successfully, then update Device info.
  //
  PcdData = LibPcdGetExPtr (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  PcdDataSize = LibPcdGetExSize (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  if (PcdData == NULL || PcdDataSize <= sizeof ("") || PcdData[PcdDataSize - 1] != CHAR_NULL) {
    return EFI_INVALID_PARAMETER;
  }
  PcdDevPathSize = GetDevicePathSize ((EFI_DEVICE_PATH_PROTOCOL*) PcdData);
  PcdAttrAsciiStrSize = PcdDataSize - PcdDevPathSize;
  NewPcdDataSize = DevicePathSize + PcdAttrAsciiStrSize;
  NewPcdData = AllocateZeroPool(NewPcdDataSize);
  if (NewPcdData == NULL){
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem(NewPcdData, DevicePath, DevicePathSize);
  CopyMem((NewPcdData + DevicePathSize), (PcdData + PcdDevPathSize), PcdAttrAsciiStrSize);
  Status = LibPcdSetExPtrS (
             &gH2ODeviceInfo2TokenSpaceGuid,
             TokenNumber,
             &NewPcdDataSize,
             NewPcdData
             );
  FreePool(NewPcdData);
  if (!EFI_ERROR (Status)) {
    UpdateDevInfofromPcdData (TokenNumber, DevInfoPcdData);
  }

  return Status;
}

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
  )
{
  CHAR8                                *PcdData;
  UINTN                                PcdDataSize;
  UINTN                                DeviceAttribNameLen;
  UINTN                                DeviceAttribValueSize;
  UINTN                                Offset;
  CHAR8                                *PcdAttribPtr;
  CHAR8                                *PcdValuePtr;
  CHAR8                                *NewPcdDataPtr;
  UINTN                                NewPcdDataSize;
  H2O_DEVICE_INFO_PCD_DATA             *DevInfoPcdData;
  EFI_STATUS                           Status;

  if (DeviceAttribName == NULL || DeviceAttribValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PcdData     = LibPcdGetExPtr (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  PcdDataSize = LibPcdGetExSize (&gH2ODeviceInfo2TokenSpaceGuid, TokenNumber);
  if (PcdData == NULL || PcdDataSize <= sizeof ("") || PcdData[PcdDataSize - 1] != CHAR_NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Parse PCD to get current value string, compare with new value string and then calculate the new PCD size.
  //
  DeviceAttribNameLen   = AsciiStrLen (DeviceAttribName);
  DeviceAttribValueSize = AsciiStrSize (DeviceAttribValue);
  NewPcdDataSize = PcdDataSize;
  PcdValuePtr = NULL;
  Offset = GetDevicePathSize ((EFI_DEVICE_PATH_PROTOCOL *) PcdData);
  while (Offset < PcdDataSize) {
    PcdAttribPtr = PcdData + Offset;
    Offset += AsciiStrSize (PcdAttribPtr);
    if ((AsciiStrnCmp (PcdAttribPtr, DeviceAttribName, DeviceAttribNameLen) != 0) ||
        (PcdAttribPtr[DeviceAttribNameLen] != '=')) {
      continue;
    }

    PcdValuePtr = &PcdAttribPtr[DeviceAttribNameLen + 1];
    if (DeviceAttribValueSize > AsciiStrSize (PcdValuePtr)) {
      NewPcdDataSize += DeviceAttribValueSize - AsciiStrSize (PcdValuePtr);
    }
    break;
  }
  if (PcdValuePtr == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Make sure DevInfoPcdData->DevPath buffer can save new PCD data and then update it to PCD database.
  //
  DevInfoPcdData = GetDevInfoPcdData (TokenNumber);
  if (DevInfoPcdData == NULL) {
    return EFI_NOT_FOUND;
  }
  if (DevInfoPcdData->DevPath == NULL ||
      DevInfoPcdData->DevPathBufferSize < NewPcdDataSize) {
    NewPcdDataPtr = AllocatePool (NewPcdDataSize);
    if (NewPcdDataPtr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    if (DevInfoPcdData->DevPath != NULL) {
      FreePool (DevInfoPcdData->DevPath);
    }
    DevInfoPcdData->DevPath           = (EFI_DEVICE_PATH_PROTOCOL *) NewPcdDataPtr;
    DevInfoPcdData->DevPathBufferSize = NewPcdDataSize;
  }

  NewPcdDataPtr = (CHAR8 *) DevInfoPcdData->DevPath;
  CopyMem (NewPcdDataPtr, PcdData, PcdValuePtr - PcdData);
  NewPcdDataPtr += PcdValuePtr - PcdData;
  CopyMem (NewPcdDataPtr, DeviceAttribValue, DeviceAttribValueSize);
  NewPcdDataPtr += DeviceAttribValueSize;
  CopyMem (NewPcdDataPtr, PcdData + Offset, PcdDataSize - Offset);
  NewPcdDataPtr += PcdDataSize - Offset;

  NewPcdDataSize = (UINTN) (NewPcdDataPtr - (CHAR8 *) DevInfoPcdData->DevPath);
  Status = LibPcdSetExPtrS (
             &gH2ODeviceInfo2TokenSpaceGuid,
             TokenNumber,
             &NewPcdDataSize,
             (CHAR8 *) DevInfoPcdData->DevPath
             );
  if (!EFI_ERROR (Status)) {
    UpdateDevInfofromPcdData (TokenNumber, DevInfoPcdData);
  }

  return Status;
}

