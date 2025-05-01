/*****************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP SYSTEM FIRMWARE FEATUR SETS PROTOCOL prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 */
#ifndef _AMD_PSP_SFFS_PROTOCOL_H_
#define _AMD_PSP_SFFS_PROTOCOL_H_

#include <AmdPspDirectory.h>

extern EFI_GUID gAmdPspSffsProtocolGuid;

typedef struct _AMD_PSP_SFFS_PROTOCOL AMD_PSP_SFFS_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *GET_SFFS_FUSE_VALUE) (
  IN     AMD_PSP_SFFS_PROTOCOL  *This,
  IN OUT UINT32                 *SffsFuse
  );

typedef struct _AMD_PSP_SFFS_PROTOCOL {
  GET_SFFS_FUSE_VALUE          GetSffsFuseValue;
} AMD_PSP_SFFS_PROTOCOL;

#endif //_AMD_PSP_SFFS_PROTOCOL_H_


