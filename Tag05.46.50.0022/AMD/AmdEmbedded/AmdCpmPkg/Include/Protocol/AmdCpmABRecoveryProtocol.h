/******************************************************************************
    Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.

*****************************************************************************/

#ifndef _AMD_CPM_ABRECOVERY_PROTOCOL_H_
#define _AMD_CPM_ABRECOVERY_PROTOCOL_H_

//
// Extern the GUID for protocol users
//
extern EFI_GUID gAmdCpmABRecoveryProtocolGuid;

typedef
EFI_STATUS
(EFIAPI *REBUILD_SHARED_B)(
  IN  EFI_PHYSICAL_ADDRESS    *BiosImageMemoryAddr
  );

//
// Protocol Interface Structure
//
typedef struct _AMD_CPM_ABRECOVERY_PROTOCOL
{
  REBUILD_SHARED_B    RebuildSharedB;
} AMD_CPM_ABRECOVERY_PROTOCOL;

#define AMD_CPM_ABRECOVERY_PROTOCOL_REVISION 0x00

#endif // _AMD_CPM_ABRECOVERY_PROTOCOL_H_
