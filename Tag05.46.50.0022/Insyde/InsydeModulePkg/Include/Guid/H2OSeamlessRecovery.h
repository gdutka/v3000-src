/** @file
  Definations for H2O seamless recovery
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

#ifndef __H2O_SEAMLESS_RECOVERY_H__
#define __H2O_SEAMLESS_RECOVERY_H__

#define H2O_SEAMLESS_RECOVERY_GUID \
  { \
    0xe83f7603, 0xa8ed, 0x4b9a, 0x8c, 0x16, 0xb0, 0x15, 0x29, 0xf6, 0x4b, 0x4f } \
  }

#define H2O_SEAMLESS_RECOVERY_DIGEST_GUID \
  { \
    0xfa07a1ea, 0x486d, 0x4b5e, 0xb9, 0x4d, 0x8e, 0x06, 0xe6, 0xfa, 0xd9, 0xdb } \
  }

#define UPDATE_PROGRESS_NAME                   L"UpdateProgress"
#define BACKUP_SBB_FILE_NAME                   L"BackupSbb.bin"
#define BACKUP_SBB_FILE_DIGEST_SHA256_NAME     L"BackupSbbDigestSha256"

typedef enum {
  FlashStart       = 0,
  FlashPbbR,
  FlashPbb,
  FlashSbb,
  FlashResiliency,
  FlashMax
} H2O_SYSTEM_FIRMWARE_UPDATE_PROGRESS;

extern EFI_GUID gH2OSeamlessRecoveryGuid;
extern EFI_GUID gH2OSeamlessRecoveryDigestGuid;

#endif
