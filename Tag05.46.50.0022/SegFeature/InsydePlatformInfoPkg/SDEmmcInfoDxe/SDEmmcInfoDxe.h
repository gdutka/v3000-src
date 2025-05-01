/** @file

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __SD_EMMC_INFO_DXE__H__
#define __SD_EMMC_INFO_DXE__H__

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/SortLib.h>
#include <Library/PrintLib.h>
#include <Library/H2OCpLib.h>
#include <Protocol/H2OSDEmmcInfo.h>
#include <Guid/H2OCp.h>


typedef struct {
  UINTN           SDEmmcControllerCount;
  SD_EMMC_INFO    *SDEmmcInfoBuffer;
} H2O_SD_EMMC_INFORMATION;

typedef struct {
  UINTN                 OemPortsAmount;
  OEM_PORT_NAME_INFO    *OemPortNameInfos;
} H2O_OEM_PORT_NAME_INFORMATION;

typedef struct _SD_EMMC_PRIVATE_DATA {
  EFI_HANDLE                      ProtocolHandle;
  UINT32                          Signature;
  H2O_SD_EMMC_INFO_PROTOCOL       SDEmmcInfoProtcol;
  H2O_SD_EMMC_INFORMATION         SDEmmcInfo;
  H2O_OEM_PORT_NAME_INFORMATION   OemPortNameInfo;
} SD_EMMC_PRIVATE_DATA;

#define SD_EMMC_INFO_SIGNATURE             SIGNATURE_32 ('S', 'D', 'E', 'M')
#define SD_EMMC_PRIVATE_INFO_FROM_THIS(a)  CR (a, SD_EMMC_PRIVATE_DATA, SDEmmcInfoProtcol, SD_EMMC_INFO_SIGNATURE)

#endif