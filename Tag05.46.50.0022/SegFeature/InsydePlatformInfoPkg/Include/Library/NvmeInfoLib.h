/** @file
  NVMe Drive Info Library Definition.

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

#ifndef NVME_INFO_LIB_H_
#define NVME_INFO_LIB_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <IndustryStandard/Nvme.h>

#include <Protocol/DevicePath.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/H2ONvmeInfo.h>

#define PCI_BASE_CLASS_CODE_STORAGE_CONTROLLER 0x01
#define PCI_SUB_CLASS_CODE_RAID_CONTROLLER     0x04

/**
  Check the Class Code to determine if the NVMe is Raid Controller.

  @retval TRUE        It is Raid Controller.
  @retval FALSE       It's not Raid Controller.

**/
BOOLEAN
IsNvmeRaidController (
  IN EFI_HANDLE   BcHandle
  );

EFI_DEVICE_PATH_PROTOCOL *
CreateNvmeNamespaceDevicePath (
  IN H2O_NVME_INFO_PROTOCOL              *NvmeInfo,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN CONST EFI_DEVICE_PATH_PROTOCOL      *DevicePathPrefix,
  IN UINT32                              NamespaceId
  );

/**
**/
CHAR16 *
GetNvmeSerialNumberFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  );

/**
**/
CHAR16 *
GetNvmeModelNumberFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  );

/**
**/
CHAR16 *
GetNvmeFwRevisionFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  );

/**
**/
CHAR16 *
GetNvmeIeeeOuiFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  );

/**
  Retrieves capacity and LBA data size from an NVMe Identify Namespace data.

  Retrieves capacity in number of logical blocks and LBA data size in bytes
  from NVMe Identify Namespace data structure.

**/
VOID
GetNvmeNamespaceCapacity (
  IN CONST NVME_ADMIN_NAMESPACE_DATA  *NamespaceData,
  OUT UINT32                          *BlockSize,
  OUT UINT64                          *NumberOfBlocks
  );

/**
  Converts IEEE EUI-64 data to a Unicode string.

  Converts IEEE Extended Unique Identifier (EUI-64) data to a Unicode string.

  @param[in]  Eui64  Pointer to the EUI-64 value. The value must be in
                     big endian.

  @retval  NULL    Not enough memory to allocate the string.
  @retval  Others  Converted string.

**/
CHAR16 *
ConvertEui64ToString (
  IN CONST UINT8  Eui64[8]
  );

#endif
