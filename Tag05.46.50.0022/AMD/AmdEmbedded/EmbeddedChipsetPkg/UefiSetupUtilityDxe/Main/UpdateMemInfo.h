/** @file

;******************************************************************************
;* Copyright (c) 2016, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _UPDATE_MEM_INFO_H
#define _UPDATE_MEM_INFO_H

#include <Uefi.h>
#include <SetupUtility.h>

#include <IndustryStandard/SmBios.h>

#include <Protocol/Smbios.h>
#include <Protocol/DataHub.h>

#include <Guid/DataHubRecords.h>

//[-start-220311-IB14740224-modify]//
typedef struct {
  EFI_STRING_ID                         ChannelStringId;
  EFI_STRING_ID                         DimmStringId;
  EFI_STRING_ID                         SizeStringId;
} UPDATE_INFO_STRUCT;
//[-end-220311-IB14740224-modify]//

#endif
