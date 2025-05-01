/** @file
  Header file of H2OSDEmmcInfo.

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

#ifndef __H2O_SD_EMMC_INFO_H__
#define __H2O_SD_EMMC_INFO_H__

#include <Uefi.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SdPassThru.h>
#include <Protocol/BlockIo.h>


#define H2O_SD_EMMC_INFO_PROTOCOL_GUID \
  { \
    0xab685acb, 0xb0f9, 0x42ca, { 0xcc, 0xb3, 0x3a, 0xef, 0x51, 0x95, 0x6e, 0x24 } \
  }

typedef struct _H2O_SD_EMMC_INFO_PROTOCOL  H2O_SD_EMMC_INFO_PROTOCOL;

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath;
  EFI_HANDLE                  Handle;
  CARD_INFO                   *CardInfo;
  EFI_BLOCK_IO_PROTOCOL       *BlockIo;
} SD_EMMC_INFO;

//
// OEM Port data strucrue
//
typedef struct {
  UINT32                    Size;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  CHAR16                    *PortName;
  UINT32                    TokenNumber;
} OEM_PORT_NAME_INFO;

typedef
EFI_STATUS
(EFIAPI *GET_SD_EMMC_INFO) (
  IN  H2O_SD_EMMC_INFO_PROTOCOL   *This,
  OUT UINTN                       *SDEmmcNum,
  OUT SD_EMMC_INFO                **SDEmmcInfoBuffer
);

typedef
EFI_STATUS
(EFIAPI *GET_OEM_PORT_NAME_INFO) (
  IN  H2O_SD_EMMC_INFO_PROTOCOL   *This,
  OUT UINTN                       *OemPortsAmount,
  OUT OEM_PORT_NAME_INFO          **OemPortNameInfoBuffer
); 

typedef
EFI_STATUS
(EFIAPI *SD_EMMC_INFO_REFRESH) (
  IN  H2O_SD_EMMC_INFO_PROTOCOL  *This
);

struct _H2O_SD_EMMC_INFO_PROTOCOL {
  GET_SD_EMMC_INFO        GetSDEmmcInfo;
  GET_OEM_PORT_NAME_INFO  GetOemPortNameInfo;
  SD_EMMC_INFO_REFRESH    Refresh;
};

extern EFI_GUID gH2OSDEmmcInfoProtocolGuid;

#endif