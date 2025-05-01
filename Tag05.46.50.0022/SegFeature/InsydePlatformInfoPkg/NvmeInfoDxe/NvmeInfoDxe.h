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

#ifndef NVME_INFO_DXE_H_
#define NVME_INFO_DXE_H_

#include <Uefi.h>

#include <Protocol/H2ONvmeInfo.h>

typedef struct {
  UINT32                             Signature;
  //
  // Produced protocols
  //
  H2O_NVME_INFO_PROTOCOL             NvmeInfo;
  
  //
  // Port name information may be added in the future here.
  //
} NVME_INFO_DATA;

#define NVME_INFO_SIGNATURE SIGNATURE_32 ('N', 'V', 'E', 'I')
#define NVME_INFO_FROM_LINK(link)    CR (link, NVME_INFO_DATA, NvmeInfo, NVME_INFO_SIGNATURE)


//
// This structure is used internally by NvmeInfoDxe to detect duplicate (same)
// controller among multiple EFI NVM Express Pass Thru Protocol instances.
// This is loosely based on the "NQN Construction for Older NVM Subsystems"
// format. Refer to sections "NVMe Qualified Names" and "Unique Identifier" of
// NVM Express Base Specification.
//
typedef struct {
  //
  // Bytes 0 - 63 of Identify Controller data structure
  //
  UINT16  PciVendorId;
  UINT16  PciSubsystemVendorId;
  UINT8   SerialNumber[20];
  UINT8   ModelNumber[40];
  //
  // Bytes 768 - 1023 of Identify Controller data structure
  //
  UINT8   NvmeQualifiedName[256];
} H2O_NVME_INFO_UNIQUE_IDENTIFIER;

/**
  Collect NVMe controller information

**/
EFI_STATUS
EFIAPI
CollectControllers (
  OUT UINTN                           *NumOfControllers,
  OUT H2O_NVME_INFO_CONTROLLER_ENTRY  **ControllerEntries
  );

/**
  Get Identify Controller data

**/
EFI_STATUS
EFIAPI
IdentifyController (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_CONTROLLER_DATA         **IdentifyControllerData
  );

/**
  Get Identify Namespace data

**/
EFI_STATUS
EFIAPI
IdentifyNamespace (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_NAMESPACE_DATA          **IdentifyNamespaceData
  );

EFI_STATUS
EFIAPI
GetSmartHealthInfo (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_SMART_HEALTH_INFO_LOG         **SmartHealthInfo
  );

#endif // NVME_INFO_DXE_H_