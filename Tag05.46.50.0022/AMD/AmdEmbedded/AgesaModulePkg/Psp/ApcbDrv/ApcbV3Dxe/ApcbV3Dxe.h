/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * APCB DXE Driver Header
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  APCB
 * @e \$Revision$   @e \$Date$
 *
 */
#include <Protocol/AmdApcbProtocol.h>

#ifndef _AMD_APCB_DXE_H_
#define _AMD_APCB_DXE_H_

VOID
mApcbSetActiveInstance (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8             Instance
  );

EFI_STATUS
EFIAPI
mApcbFlushData (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  );

EFI_STATUS
EFIAPI
mApcbAcquireMutex (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  );

EFI_STATUS
EFIAPI
mApcbReleaseMutex (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  );

EFI_STATUS
EFIAPI
mApcbGetTokenBool (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        *PriorityLevel,
  IN       UINT32            Uid,
  IN OUT   BOOLEAN           *bValue
  );

EFI_STATUS
EFIAPI
mApcbSetTokenBool (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel,
  IN       UINT32            Uid,
  IN       BOOLEAN           bValue
  );

EFI_STATUS
EFIAPI
mApcbGetToken8 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        *PriorityLevel,
  IN       UINT32            Uid,
  IN OUT   UINT8             *Value8
  );

EFI_STATUS
EFIAPI
mApcbSetToken8 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel,
  IN       UINT32            Uid,
  IN       UINT8             Value8
  );

EFI_STATUS
EFIAPI
mApcbGetToken16 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        *PriorityLevel,
  IN       UINT32            Uid,
  IN OUT   UINT16            *Value16
  );

EFI_STATUS
EFIAPI
mApcbSetToken16 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel,
  IN       UINT32            Uid,
  IN       UINT16            Value16
  );

EFI_STATUS
EFIAPI
mApcbGetToken32 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        *PriorityLevel,
  IN       UINT32            Uid,
  IN OUT   UINT32            *Value32
  );

EFI_STATUS
EFIAPI
mApcbSetToken32 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel,
  IN       UINT32            Uid,
  IN       UINT32            Value32
  );

EFI_STATUS
EFIAPI
mApcbGetType (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        *PriorityLevel,
  IN       UINT16                       GroupId,
  IN       UINT16                       TypeId,
  IN       UINT16                       InstanceId,
      OUT  UINT8                        **DataBuf,
      OUT  UINT32                       *DataSize
  );

EFI_STATUS
EFIAPI
mApcbSetType (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel,
  IN       UINT16                       GroupId,
  IN       UINT16                       TypeId,
  IN       UINT16                       InstanceId,
  IN       UINT8                        *DataBuf,
  IN       UINT32                       DataSize
  );

EFI_STATUS
EFIAPI
mApcbPurgeTokens (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT32                       *ApcbTokenList,
  IN       UINT32                       TokenCount
  );

EFI_STATUS
EFIAPI
mApcbPurgeAllTokens (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel
  );

EFI_STATUS
EFIAPI
mApcbPurgeAllTypes (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        PriorityLevel
  );

EFI_STATUS
EFIAPI
mApcbGetDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN OUT   DPPR_REPAIR_ENTRY_V2 *Entry,
  IN OUT   UINT32            *NumOfEntries
  );

EFI_STATUS
EFIAPI
mApcbClearDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  );

EFI_STATUS
EFIAPI
mApcbAddDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       DPPR_REPAIR_ENTRY_V2 *Entry
  );

EFI_STATUS
EFIAPI
mApcbRemoveDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       DPPR_REPAIR_ENTRY_V2 *Entry
  );

EFI_STATUS
EFIAPI
mApcbSetConfigParameter (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT16            TokenId,
  IN OUT   UINT32            *SizeInByte,
  IN OUT   UINT64            *Value
  );

EFI_STATUS
EFIAPI
mApcbGetConfigParameter (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT16            TokenId,
  IN OUT   UINT32            *SizeInByte,
  IN OUT   UINT64            *Value
  );

VOID
EFIAPI
AmdPspApcbUpdateShadowDimmConfig (
  IN       EFI_EVENT Event,
  IN       VOID      *Context
  );

#endif //_AMD_APCB_DXE_H_


