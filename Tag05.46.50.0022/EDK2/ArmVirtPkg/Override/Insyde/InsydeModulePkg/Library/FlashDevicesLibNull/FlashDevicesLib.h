/** @file
  Header file for Flash Device Support Library Class

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _FLASH_DEVICES_LIB_H_
#define _FLASH_DEVICES_LIB_H_

#include <Uefi.h>
#include <Guid/H2OFlashDevice.h>

#define MAX_STRING          0x20

typedef enum {
  LpcFlashDeviceType1,
  LpcFlashDeviceType2,
  LpcFlashDeviceType3,
  SpiFlashDeviceType
} H2O_FLASH_DEVICE_TYPE;

#pragma pack(1)

typedef struct _H2O_FLASH_DEVICE_BLOCK {
  UINT16 Size;            ///< size of each block, in 256 byte chunks.
  UINT16 Count;           ///< number of blocks.
} H2O_FLASH_DEVICE_BLOCK;

typedef struct _H2O_FLASH_DEVICE {
  UINT32 Size;            ///< Size of structure.
  VOID   *DeviceTypeData;
  UINT8  DeviceType;      ///< Flash Device Type.
  UINT8  Reserved[3];
  UINT32 Id;              ///< Flash Device Identifier.
  UINT32 ExtId;
  H2O_FLASH_DEVICE_BLOCK BlockMap[1];
  CHAR8  VendorName[MAX_STRING];
  CHAR8  DeviceName[MAX_STRING];  
} H2O_FLASH_DEVICE;


//
// Format of this structure must match the last part of H2O_FLASH_DEVICE.
//
typedef struct _H2O_PCD_FLASH_DEVICE {
  UINT8  DeviceType;      ///< Flash Device Type.
  UINT8  Reserved[3];     ///< Reserved. Must be 0.

  UINT32 Id;              ///< Flash Device Identifier.
  UINT32 ExtId;
//H2O_FLASH_DEVICE_BLOCK BlockMap[1];
} H2O_PCD_FLASH_DEVICE;
#pragma pack()


#endif
