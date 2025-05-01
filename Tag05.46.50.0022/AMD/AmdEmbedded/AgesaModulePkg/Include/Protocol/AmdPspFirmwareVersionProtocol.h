/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP FIRMWARE VERSION PROTOCOL prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 */
#ifndef _AMD_PSP_FIRMWARE_VERSION_PROTOCOL_H_
#define _AMD_PSP_FIRMWARE_VERSION_PROTOCOL_H_

#include <AmdPspDirectory.h>

extern EFI_GUID gAmdPspFirmwareVersionProtocolGuid;

typedef struct _AMD_PSP_FIRMWARE_VERSION_PROTOCOL AMD_PSP_FIRMWARE_VERSION_PROTOCOL;

typedef enum  {
  FirmwareTypePspBootLoader,
  FirmwareTypePspRecoveryBootLoader,
  FirmwareTypeSmu,
  FirmwareTypeAegsaBootLoader,
  FirmwareTypeApcb,
  FirmwareTypeApob,
  FirmwareTypeAppb,
  FirmwareTypeSev,
  FirmwareTypePhy,
  FirmwareTypeMpio,
  FirmwareTypeMmpdma,
  FirmwareTypePm,
  FirmwareTypeGmi,
  FirmwareTypeRib,
} AMD_PSP_FIRMWARE_TYPE;

typedef enum  {
  FirmwareLevel1,
  FirmwareLevel2
} AMD_PSP_FIRMWARE_LEVEL;

typedef
EFI_STATUS
(EFIAPI *GET_FIRMWARE_VERSION) (
  IN     AMD_PSP_FIRMWARE_VERSION_PROTOCOL  *This,
  IN     AMD_PSP_FIRMWARE_TYPE              FirmwareType,
  IN     AMD_PSP_FIRMWARE_LEVEL             FirmwareLevel,
  IN OUT UINT32                             *FirmwareVersion
  );

typedef struct _AMD_PSP_FIRMWARE_VERSION_PROTOCOL {
  GET_FIRMWARE_VERSION            GetFirmwareVersion;
} AMD_PSP_FIRMWARE_VERSION_PROTOCOL;

#endif //_AMD_PSP_FIRMWARE_VERSION_PROTOCOL_H_


