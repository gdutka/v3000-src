/** @file

;******************************************************************************
;* Copyright (c) 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _IHISI_CHIPSET_UPDATE_FLASH_SEQUENCE_H_
#define _IHISI_CHIPSET_UPDATE_FLASH_SEQUENCE_H_

#include "IhisiSmm.h"

#include <Library/SeamlessRecoveryLib.h>
#include <Library/BvdtLib.h>

#include <Guid/PreventFlashPowerLoss.h>

EFI_STATUS
EFIAPI
ChipsetFbtsUpdateExtendPlatform (
  VOID
  );

EFI_STATUS
UpdateExtendPlatformBuildDateTimeInfo (
  IN OUT EXTEND_PLATFORM_DATA_ITEM  *ExtInfoDataItemPtr
  );

EFI_STATUS
ChipsetUpdateExtendPlatformBiosUpdateSequence (
  IN OUT EXTEND_PLATFORM_DATA_ITEM  *ExtInfoDataItemPtr
  );

EFI_STATUS
GetRomSizeFromBlockMap (
  OUT  UINT32  *RomSize
  );

EFI_STATUS
ChipsetFbtsUpdateExtendPlatformInit (
  VOID
  );
#endif
