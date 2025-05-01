/** @file
  Header file of H2ONvneDriveInfo.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef H2O_NVME_INFO_H_
#define H2O_NVME_INFO_H_

#include <Uefi.h>

#include <IndustryStandard/Nvme.h>

#include <Protocol/DiskInfo.h>
#include <Protocol/NvmExpressPassthru.h>

#define NVME_GENERIC_TIMEOUT            EFI_TIMER_PERIOD_SECONDS (5)

#define H2O_NVME_INFO_PROTOCOL_GUID \
  { \
    0x526ecbdf, 0xe628, 0x4c96, { 0xb2, 0xec, 0x30, 0xce, 0x45, 0x52, 0xba, 0x74 } \
  }

typedef struct _H2O_NVME_INFO_PROTOCOL  H2O_NVME_INFO_PROTOCOL;

typedef struct {
  EFI_HANDLE       Handle;
  UINT32           NamespaceId;
  UINT32           NumOfValidNamespaces;
  UINT8            IsRaidController:1;
  UINT8            Reserved:7;
} H2O_NVME_INFO_CONTROLLER_ENTRY;


typedef
EFI_STATUS
(EFIAPI *H2O_NVME_INFO_COLLECT_CONTROLLERS) (
  OUT UINTN                           *NumOfControllers,
  OUT H2O_NVME_INFO_CONTROLLER_ENTRY  **ControllerEntries
  );

/**
  Get Identify Controller data

**/
typedef
EFI_STATUS
(EFIAPI *H2O_NVME_INFO_IDENTIFY_CONTROLLER) (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_CONTROLLER_DATA         **IdentifyControllerData
  );

/**


**/
typedef
EFI_STATUS
(EFIAPI *H2O_NVME_INFO_IDENTIFY_NAMESPACE) (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_NAMESPACE_DATA          **IdentifyNamespaceData
  );

typedef
EFI_STATUS
(EFIAPI *H2O_NVME_INFO_GET_SMART_HEALTH_INFO) (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_SMART_HEALTH_INFO_LOG         **SmartHealthInfo
  );

struct _H2O_NVME_INFO_PROTOCOL {
  H2O_NVME_INFO_COLLECT_CONTROLLERS    CollectControllers;
  H2O_NVME_INFO_IDENTIFY_CONTROLLER    IdentifyController;
  H2O_NVME_INFO_IDENTIFY_NAMESPACE     IdentifyNamespace;
  H2O_NVME_INFO_GET_SMART_HEALTH_INFO  GetSmartHealthInfo;
};

extern EFI_GUID gH2ONvmeInfoProtocolGuid;

#endif
