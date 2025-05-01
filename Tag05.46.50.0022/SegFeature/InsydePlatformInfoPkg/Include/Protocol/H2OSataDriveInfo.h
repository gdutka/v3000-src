/** @file
  Header file of H2OSataDriveInfo.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __H2O_SATA_DRIVE_INFO_H__
#define __H2O_SATA_DRIVE_INFO_H__

#include <Uefi.h>

#include <Protocol/AtaPassThru.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/ScsiPassThruExt.h>

#include <IndustryStandard/Atapi.h>
#include <IndustryStandard/Pci.h>

#ifndef STATIC_ASSERT
#ifdef MDE_CPU_EBC
  #define STATIC_ASSERT(Expression, Message)
#elif defined(_MSC_EXTENSIONS)
  #define STATIC_ASSERT static_assert
#else
  #define STATIC_ASSERT _Static_assert
#endif
#endif

#define MAX_SMART_ATTRIBUTE_ENTRY   30

#pragma pack(1)
typedef struct {
  UINT8   Id;
  UINT16  Flags;
  UINT8   Value;
  UINT8   Raw[8];
} ATA_SMART_ATTRIBUTE_ENTRY;

STATIC_ASSERT (
  sizeof (ATA_SMART_ATTRIBUTE_ENTRY) == 12,
  "ATA_SMART_ATTRIBUTE_ENTRY is expected to be exactly 12 bytes long."
  );

typedef struct {
  UINT16                      Version;
  ATA_SMART_ATTRIBUTE_ENTRY   Attrib[MAX_SMART_ATTRIBUTE_ENTRY];  ///< offset 2h
  UINT8                       OffLineDataCollectionSts;           ///< offset 362h
  UINT8                       SelfTestExecutionSts;               ///< offset 363h
  UINT8                       VendorSpecific_1[2];                ///< offset 364h
  UINT8                       VendorSpecific_2;                   ///< offset 366h
  UINT8                       OffLineDataCollectionCap;           ///< offset 367h
  UINT16                      SmartCap;                           ///< offset 368h
  UINT8                       ErrorLoggingCap;                    ///< offset 370h
  UINT8                       VendorSpecific_3;                   ///< offset 371h
  UINT8                       ShortSelfTestRoutineRecPollingTime;
  UINT8                       ExtSelfTestRoutineRecPollingTime;
  UINT8                       ConSelfTestRoutineRecPollingTime;
  UINT16                      ExtSelfTestRoutineRecPollingTime2;
  UINT8                       Reserved_1[9];                      ///< offset 377h
  UINT8                       VendorSpecific_4[125];              ///< offset 386h
  UINT8                       Checksum;                           ///< offset 511h
} ATA_SMART_DATA;

STATIC_ASSERT (
  sizeof (ATA_SMART_DATA) == 512,
  "ATA_SMART_DATA is expected to be exactly 512 bytes long."
  );
#pragma pack()

typedef struct {
  UINT32                          Signature;
  LIST_ENTRY                      Link;
  EFI_HANDLE                      DeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_ATA_PASS_THRU_PROTOCOL      *AtaPassThru;
  EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ExtScsiPassThru;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  PCI_TYPE00                      PciConfigSpace;
  LIST_ENTRY                      ChildList;
  UINT32                          SerialNumber;
} BUS_CONTROLLER_INFO;

#define BUS_CONTROLLER_INFO_SIGNATURE SIGNATURE_32 ('b', 'c', 'i', 'f')

#define BUS_CONTROLLER_INFO_FROM_LINK(a) \
  CR (a, BUS_CONTROLLER_INFO, Link, BUS_CONTROLLER_INFO_SIGNATURE)

typedef struct {
  UINT32                          Signature;
  LIST_ENTRY                      Link;
  BUS_CONTROLLER_INFO             *Parent;
  LIST_ENTRY                      ChildLink;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  ATA_IDENTIFY_DATA               *IdentifyData;
  ATA_SMART_DATA                  *SmartData;
  EFI_STATUS                      SmartReturnStatus;
  UINT32                          SerialNumber;
} DISK_DRIVE_INFO;

#define DISK_DRIVE_INFO_SIGNATURE SIGNATURE_32 ('d', 'd', 'i', 'f')

#define DISK_DRIVE_INFO_FROM_LINK(a) \
  CR (a, DISK_DRIVE_INFO, Link, DISK_DRIVE_INFO_SIGNATURE)

#define DISK_DRIVE_INFO_FROM_CHILD_LINK(a) \
  CR (a, DISK_DRIVE_INFO, ChildLink, DISK_DRIVE_INFO_SIGNATURE)

//
// Definition of the H2oSataDriveInfo Protocol
//
typedef struct _H2O_SATA_DRIVE_INFO_PROTOCOL H2O_SATA_DRIVE_INFO_PROTOCOL;

/**
  Get the ControllerInfoPool.

  @param[in]    This                The pointer to the instance of SataDriveInfo Protocol.
  @param[out]   ControllerInfoPool  The pointer to ControllerInfoPool.

  @retval EFI_SUCCESS             Successfully get the ControllerInfoPool.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.

**/
typedef
EFI_STATUS
(EFIAPI *H2O_SATA_DRIVE_INFO_GET_CONTROLLER_INFO_POOL)(
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **ControllerInfoPool
  );

/**
  Get the DriveInfoPool.

  @param[in]    This            The pointer to the instance of SataDriveInfo Protocol.
  @param[out]   DriveInfoPool   The pointer to DriveInfoPool.

  @retval EFI_SUCCESS             Successfully get the DriveInfoPool.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.

**/
typedef
EFI_STATUS
(EFIAPI *H2O_SATA_DRIVE_INFO_GET_DRIVE_INFO_POOL)(
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **DriveInfoPool
  );

/**
  Helper function to rescan and rebuild the pools.

  @param[in]  This  Points to the H2O_SATA_DRIVE_INFO_PROTOCOL.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval others                  Other failure occurs.

**/
typedef
VOID
(EFIAPI *H2O_SATA_DRIVE_INFO_REFRESH)(
  IN        H2O_SATA_DRIVE_INFO_PROTOCOL  *This
  );

struct _H2O_SATA_DRIVE_INFO_PROTOCOL {
  UINT32                                        Size;
  H2O_SATA_DRIVE_INFO_GET_CONTROLLER_INFO_POOL  GetControllerInfoPool;
  H2O_SATA_DRIVE_INFO_GET_DRIVE_INFO_POOL       GetDriveInfoPool;
  H2O_SATA_DRIVE_INFO_REFRESH                   Refresh;
};

// {01050563-f42c-4d70-9345-73c9cc0fd4c4}
#define H2O_SATA_DRIVE_INFO_PROTOCOL_GUID \
  { \
    0x01050563, 0xf42c, 0x4d70, { 0x93, 0x45, 0x73, 0xc9, 0xcc, 0x0f, 0xd4, 0xc4 } \
  }

extern EFI_GUID gH2OSataDriveInfoProtocolGuid;

#endif