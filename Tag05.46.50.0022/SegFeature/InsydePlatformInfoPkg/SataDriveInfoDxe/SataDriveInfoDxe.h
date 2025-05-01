/** @file
  Implementation of H2OSataDriveInfoDxe.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __SATA_DRIVE_INFO_DXE_H__
#define __SATA_DRIVE_INFO_DXE_H__

#include <Uefi.h>

#include <Protocol/H2OSataDriveInfo.h>

#define H2O_SATA_DRIVE_INFO_PRIVATE_SIGNATURE   SIGNATURE_32 ('h', 's', 'd', 'i')

typedef struct {
  UINT32                        Signature;
  EFI_HANDLE                    Handle;
  H2O_SATA_DRIVE_INFO_PROTOCOL  H2oSataDriveInfo;
  LIST_ENTRY                    ControllerInfoPool;
  LIST_ENTRY                    DriveInfoPool;
} H2O_SATA_DRIVE_INFO_PRIVATE_DATA;

#define H2O_SATA_DRIVE_INFO_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, H2O_SATA_DRIVE_INFO_PRIVATE_DATA, H2oSataDriveInfo, H2O_SATA_DRIVE_INFO_PRIVATE_SIGNATURE)

#endif
