/** @file
SDEmmcInfoSetupUtilityDxe's header

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __SD_EMMC_INFO_SETUP_UTILITY_DXE_H__
#define __SD_EMMC_INFO_SETUP_UTILITY_DXE_H__

#include <Uefi.h>
#include <Base.h>
#include <Guid/SDEmmcInfoSetupUtilityHii.h>
#include <Guid/MdeModuleHii.h>

#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/H2OFormBrowser.h>
#include <Protocol/H2OSDEmmcInfo.h>
#include <Protocol/SetupUtilityApplication.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/HiiExLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SDEmmcInfoLib.h>
#include <Library/VariableLib.h>
#include <Library/DxeSetupVariableLib.h>

#include "SDEmmcInfoFormPolicy.h"


#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

typedef enum {
  ByCardType   = 0,
  ByOemPortName
} DISPLAY_MODE;

typedef enum {
  PlugIn   = 0,
  PlugOut,
  Unsupport
} CHANGING_TYPE;

typedef struct {
  UINT32                             Signature;
  EFI_HANDLE                         DriverHandle;
  EFI_HII_HANDLE                     HiiHandle;
  H2O_SD_EMMC_INFO_PROTOCOL          *Protocol;
  SD_EMMC_SETUP_CONFIGURATION            Configuration;
  EFI_HII_CONFIG_ACCESS_PROTOCOL     ConfigAccess;
  EFI_HII_CONFIG_ROUTING_PROTOCOL    *HiiConfigRouting;
  EFI_EVENT                          SetupUtilityEvent;
} SD_EMMC_SETUP_UTILITY_PRIVATE_DATA;

typedef struct _SD_EMMC_INFO_PARSER_TABLE {
  EFI_STRING_ID           PromptStringId;
  EFI_STRING_ID           HelpStringId;
  INFO_PARSER_FUNCTION    InfoParserFunction;
} SD_EMMC_INFO_PARSER_TABLE;

extern UINT8  SDEmmcInfoSetupUtilityDxeVfrBin[];
extern UINT8  SDEmmcInfoSetupUtilityDxeStrings[];

#define MAX_DATA_STRING                               100
#define NAME_STR_SIZE                                 20
#define SD_EMMC_SETUP_UTILITY_PRIVATE_SIGNATURE       SIGNATURE_32 ('S', 'E', 'S', 'U')
#define SD_EMMC_SETUP_UTILITY_PRIVATE_FROM_THIS(a)    CR (a, SD_EMMC_SETUP_UTILITY_PRIVATE_DATA, ConfigAccess, SD_EMMC_SETUP_UTILITY_PRIVATE_SIGNATURE)

#endif
