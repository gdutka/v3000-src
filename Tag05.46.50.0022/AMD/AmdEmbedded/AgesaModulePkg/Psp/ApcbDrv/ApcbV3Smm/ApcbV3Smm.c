/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/**
 * @file
 *
 * APCB DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  APCB
 * @e \$Revision$   @e \$Date$
 *
 */

#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmBase2.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/SmmMemLib.h>

//#include "AGESA.h"
#include "Porting.h"
#include "SocLogicalId.h"
// #include "Addendum/Apcb/Inc/SSP/APCB.h"
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Priority.h>
#include "ApcbV3Smm.h"
#include <Library/ApcbLibV3.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspDxeSmmBufLib.h>
#include <Filecode.h>
#include <AmdApcbSmmCommunication.h>

#define FILECODE PSP_APCBDRV_APCBV3SMM_APCBV3SMM_FILECODE

extern EFI_GUID gAmdApcbSmmServiceProtocolGuid;

EFI_STATUS
DefRet (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

STATIC AMD_APCB_SERVICE_PROTOCOL mApcbSmmServiceProtocol = {
  APCB_PROTOCOL_VERSION_3_0,
  mApcbFlushData,
  mApcbGetDramPostPkgRepairEntry,
  mApcbClearDramPostPkgRepairEntry,
  mApcbAddDramPostPkgRepairEntry,
  mApcbRemoveDramPostPkgRepairEntry,
  (FP_SET_ACTIVE_APCB_INSTANCE)DefRet,
  mApcbGetConfigParameter,
  mApcbSetConfigParameter,
  (FP_GET_CBS_PARAM)DefRet,
  (FP_SET_CBS_PARAM)DefRet,
  (FP_UPDATE_CBS_APCB_DATA)DefRet,
  mApcbAcquireMutex,
  mApcbReleaseMutex,
  mApcbGetTokenBool,
  mApcbSetTokenBool,
  mApcbGetToken8,
  mApcbSetToken8,
  mApcbGetToken16,
  mApcbSetToken16,
  mApcbGetToken32,
  mApcbSetToken32,
  mApcbGetType,
  mApcbSetType,
  mApcbPurgeAllTokens,
  mApcbPurgeAllTypes,
  mApcbPurgeTokens
};

STATIC EFI_HANDLE mApcbSmmCommHandle;

/*++

Routine Description:

  This function flush the APCB data back to SPI ROM
 *
 *  @param[in]     PriorityLevel      APCB priority level of the current operation
 *
--*/
EFI_STATUS
EFIAPI
mApcbFlushData (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  )
{
  //
  // Write back Shadow Copy in DRAM
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("Write back APCB to SPI\n");
  return AmdPspWriteBackApcbShadowCopy (TRUE);
}

/**
 *
 * Routine Description:
 *
 *   This function gets the APCB config parameter
 *
 *  @param[in]     TokenId      APCB token ID defined in APCB_COMMON_CONFIG_ID of ApcbCommon.h
 *  @param[in,out] SizeInByte   Set to non-NULL to get Size in bytes of the APCB token
 *  @param[in,out] Value        Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Cannot find the according APCB token
 *          EFI_INVALID_PARAMETER  Invalid parameters
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetConfigParameter (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT16            TokenId,
  IN OUT   UINT32            *SizeInByte,
  IN OUT   UINT64            *Value
  )
{
  if (ApcbV2GetConfigToken (TokenId, SizeInByte, Value)) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function sets the APCB config parameter
 *
 *  @param[in]     TokenId      APCB token ID defined in APCB_COMMON_CONFIG_ID of ApcbCommon.h
 *  @param[in,out] SizeInByte   Set to non-NULL to get Size in bytes of the APCB token
 *  @param[in,out] Value        Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Set APCB value successfully
 *          EFI_NOT_FOUND  Cannot find the according APCB token
 *          EFI_INVALID_PARAMETER  Invalid parameters
 *
 **/
EFI_STATUS
EFIAPI
mApcbSetConfigParameter (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT16            TokenId,
  IN OUT   UINT32            *SizeInByte,
  IN OUT   UINT64            *Value
  )
{
  if (ApcbV2SetConfigToken (TokenId, SizeInByte, Value)) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
 *
 * Routine Description:
 *
 *  This function acquires the Mutex for subsequent APCB operations
 *
 *  @retval EFI_ACCESS_DENIED  APCB cannot be accessed on the current core
 *
 **/
EFI_STATUS
EFIAPI
mApcbAcquireMutex (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  )
{
  if (ApcbAcquireMutex ()) {
    return EFI_SUCCESS;
  }

  return EFI_ACCESS_DENIED;
}

/**
 *
 * Routine Description:
 *
 * This function releases the Mutex for previous APCB operations
 *
 **/
EFI_STATUS
EFIAPI
mApcbReleaseMutex (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  )
{
  ApcbReleaseMutex ();

  return EFI_SUCCESS;
}

/**
 *
 * Routine Description:
 *
 *   This function gets an Boolean APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[out]    Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]    bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Cannot find the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetTokenBool (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
     OUT   UINT8                        *Purpose,
  IN       UINT32            Uid,
     OUT   BOOLEAN           *bValue
  )
{
  UINT8 CurrentPriorityLevel;
  UINT8 StartingPriorityLevel;
  UINT8 EndingPriorityLevel;

  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;

  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    if (ApcbGetBool (Uid, bValue)) {
      if (NULL != Purpose) {
        *Purpose = ApcbGetPurpose ();
      }
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
 *
 * Routine Description:
 *
 *   This function sets an Boolean APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Cannot set the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbSetTokenBool (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT32            Uid,
  IN       BOOLEAN           bValue
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbSetBool (Uid, bValue)) {
    return EFI_SUCCESS;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function gets an UINT8 APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[out]    Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]    Value8             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Cannot find the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetToken8 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
     OUT   UINT8                        *Purpose,
  IN       UINT32            Uid,
     OUT   UINT8             *Value8
  )
{
  UINT8 CurrentPriorityLevel;
  UINT8 StartingPriorityLevel;
  UINT8 EndingPriorityLevel;

  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;

  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    if (ApcbGet8 (Uid, Value8)) {
      if (NULL != Purpose) {
        *Purpose = ApcbGetPurpose ();
      }
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
 *
 * Routine Description:
 *
 *   This function sets an UINT8 APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     Value8             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Cannot set the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbSetToken8 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT32            Uid,
  IN       UINT8             Value8
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbSet8 (Uid, Value8)) {
    return EFI_SUCCESS;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function gets an UINT16 APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[out]    Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]    bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Cannot find the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetToken16 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
     OUT   UINT8                        *Purpose,
  IN       UINT32            Uid,
     OUT   UINT16            *Value16
  )
{
  UINT8 CurrentPriorityLevel;
  UINT8 StartingPriorityLevel;
  UINT8 EndingPriorityLevel;

  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;

  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    if (ApcbGet16 (Uid, Value16)) {
      if (NULL != Purpose) {
        *Purpose = ApcbGetPurpose ();
      }
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
 *
 * Routine Description:
 *
 *   This function sets an UINT16 APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     Value16            Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Cannot set the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbSetToken16 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT32            Uid,
  IN       UINT16            Value16
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbSet16 (Uid, Value16)) {
    return EFI_SUCCESS;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function gets an UINT32 APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[out]    Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]    bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Cannot find the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetToken32 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
     OUT   UINT8                        *Purpose,
  IN       UINT32            Uid,
     OUT   UINT32            *Value32
  )
{
  UINT8 CurrentPriorityLevel;
  UINT8 StartingPriorityLevel;
  UINT8 EndingPriorityLevel;

  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;

  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    if (ApcbGet32 (Uid, Value32)) {
      if (NULL != Purpose) {
        *Purpose = ApcbGetPurpose ();
      }
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
 *
 * Routine Description:
 *
 *   This function sets an UINT32 APCB token value
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     Value32            Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Cannot set the APCB token
 *
 **/
EFI_STATUS
EFIAPI
mApcbSetToken32 (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT32            Uid,
  IN       UINT32            Value32
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbSet32 (Uid, Value32)) {
    return EFI_SUCCESS;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }
}

/**
 *  This function retrieves the data of a specified type
 *
 * @param[in]     This               Pointer to the instance of APCB Protocol
 * @param[out]    Purpose            Purpose of the APCB type where the data is retrieved
 * @param[in]     GroupId            Group ID
 * @param[in]     TypeId             Type ID
 * @param[in]     InstanceId         Instance ID
 * @param[out]    DataBuf            Pointer to the type data
 * @param[out]    DataSize           Pointer to the size of the type data
 *
 * @retval        EFI_SUCCESS        The type data is retrieved successfully
 *                EFI_NOT_FOUND      The type data cannot be retrieved
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetType (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
      OUT  UINT8                        *Purpose,
  IN       UINT16                       GroupId,
  IN       UINT16                       TypeId,
  IN       UINT16                       InstanceId,
      OUT  UINT8                        **DataBuf,
      OUT  UINT32                       *DataSize
  )
{
  UINT8 CurrentPriorityLevel;
  UINT8 StartingPriorityLevel;
  UINT8 EndingPriorityLevel;

  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;

  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    if (ApcbGetType (GroupId, TypeId, InstanceId, DataBuf, DataSize)) {
      if (NULL != Purpose) {
        *Purpose = ApcbGetPurpose ();
      }
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
 *  This function sets the data of a specified type
 *
 * @param[in]     This               Pointer to the instance of APCB Protocol
 * @param[in]     Purpose            Purpose the current operation
 * @param[in]     GroupId            Group ID
 * @param[in]     TypeId             Type ID
 * @param[in]     InstanceId         Instance ID
 * @param[in]     DataBuf            Pointer to the type data
 * @param[in]     DataSize           Pointer to the size of the type data
 *
 * @retval        EFI_SUCCESS        The type data is set successfully
 *                EFI_NOT_FOUND      The type data cannot be retrieved
 *
 **/
EFI_STATUS
EFIAPI
mApcbSetType (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT16                       GroupId,
  IN       UINT16                       TypeId,
  IN       UINT16                       InstanceId,
  IN       UINT8                        *DataBuf,
  IN       UINT32                       DataSize
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbSetType (GroupId, TypeId, InstanceId, DataBuf, DataSize)) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function purges APCB tokens
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *  @param[in]     ApcbTokenList      ApcbTokenList to be purged
 *  @param[in]     TokenCount         Count of ApcbToken
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_NOT_FOUND         No APCB tokens found
 *
 **/
EFI_STATUS
EFIAPI
mApcbPurgeTokens (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose,
  IN       UINT32                       *ApcbTokenList,
  IN       UINT32                       TokenCount
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbPurgeTokens (ApcbTokenList, TokenCount)) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function purges all the APCB tokens
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_NOT_FOUND         No APCB tokens found
 *
 **/
EFI_STATUS
EFIAPI
mApcbPurgeAllTokens (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbPurgeAllTokens ()) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function purges the data of all types
 *
 *  @param[in]     This               Pointer to the instance of APCB Protocol
 *  @param[in]     Purpose            Purpose the current operation
 *
 *  @retval EFI_SUCCESS                  All types are purged
 *          EFI_OUT_OF_RESOURCES         All types are not purged
 *
 **/
EFI_STATUS
EFIAPI
mApcbPurgeAllTypes (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose
  )
{
  ApcbSetPurpose (Purpose);
  if (ApcbPurgeAllTypes ()) {
    return EFI_SUCCESS;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }
}

/**
 *
 * Routine Description:
 *
 *   This function retrieves the DRAM POST Package repair entries
 *
 *  @param[in, out]     Entry             pointer to the DRAM POST Package repair entries to be retrieved
 *  @param[in, out]     NumOfEntries      Number of DRAM POST Package repair entries to be returned
 *
 *  @retval EFI_SUCCESS        DRAM POST Package repair entries cleared successfully
 *
 **/
EFI_STATUS
EFIAPI
mApcbGetDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN OUT   DPPR_REPAIR_ENTRY_V2         *Entry,
  IN OUT   UINT32                       *NumOfEntries
  )
{
  EFI_STATUS                Status;
  UINT8                     i;
  APCB_DPPRCL_REPAIR_ENTRY_V2  *RepairEntry;
  UINT32                    SizeInByte;
  UINT8                     CurrentPriorityLevel;
  UINT8                     StartingPriorityLevel;
  UINT8                     EndingPriorityLevel;

  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;

  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    Status = ApcbGetDramPostPkgRepairEntries (&RepairEntry, &SizeInByte);
    if (EFI_SUCCESS == Status) {
      if (NumOfEntries != NULL) {
        *NumOfEntries = SizeInByte / sizeof (APCB_DPPRCL_REPAIR_ENTRY_V2);
      } else {
        return EFI_INVALID_PARAMETER;
      }

      if (Entry != NULL) {
        for (i = 0; i < *NumOfEntries; i ++, Entry ++, RepairEntry ++) {
          Entry->ddr.Valid             = RepairEntry->ddr.Valid;
          Entry->ddr.Bank              = RepairEntry->ddr.Bank;
          Entry->ddr.RankMultiplier    = RepairEntry->ddr.RankMultiplier;
          Entry->ddr.Device            = RepairEntry->ddr.Device;
          Entry->ddr.ChipSelect        = RepairEntry->ddr.ChipSelect;
          Entry->ddr.Column            = RepairEntry->ddr.Column;
          Entry->ddr.RepairType        = RepairEntry->ddr.RepairType;
          Entry->ddr.Row               = RepairEntry->ddr.Row;
          Entry->ddr.Socket            = RepairEntry->ddr.Socket;
          Entry->ddr.Channel           = RepairEntry->ddr.Channel;
          Entry->ddr.TargetDevice      = RepairEntry->ddr.TargetDevice;

          Entry->ddr.SubChannel        = RepairEntry->ddr.SubChannel;
          Entry->ddr.HardPPRDone       = RepairEntry->ddr.HardPPRDone;
          Entry->ddr.PPRUndo           = RepairEntry->ddr.PPRUndo;
          Entry->ddr.PPRLock           = RepairEntry->ddr.PPRLock;
          Entry->ddr.SerialNumber      = RepairEntry->ddr.SerialNumber;
          Entry->ddr.DeviceTypeToRepair = RepairEntry->ddr.DeviceTypeToRepair;
        }
      }
      return Status;
    }
  }

  return EFI_NOT_FOUND;
}

/**
 *
 * Routine Description:
 *
 *   This function clears the DRAM POST Package repair entries
 *
 *  @retval EFI_SUCCESS        DRAM POST Package repair entries cleared successfully
 *
 **/
EFI_STATUS
EFIAPI
mApcbClearDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  )
{
  ApcbSetPurpose (APCB_TYPE_PURPOSE_EVENT_LOGGING);
  return ApcbClearDramPostPkgRepairEntry ();
}

/**
 *
 * Routine Description:
 *
 *   This function adds a DRAM POST Package repair entry
 *
 *  @param[in]     Entry      DRAM POST Package repair entry to be added
 *
 *  @retval EFI_SUCCESS        DRAM POST Package repair entry added successfully
 *
 **/
EFI_STATUS
EFIAPI
mApcbAddDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       DPPR_REPAIR_ENTRY_V2 *Entry
  )
{
  APCB_DPPRCL_REPAIR_ENTRY_V2  RepairEntry;

  RepairEntry.ddr.Valid         = 1;
  RepairEntry.ddr.Bank          = Entry->ddr.Bank;
  RepairEntry.ddr.RankMultiplier= Entry->ddr.RankMultiplier;
  RepairEntry.ddr.Device        = Entry->ddr.Device;
  RepairEntry.ddr.ChipSelect    = Entry->ddr.ChipSelect;
  RepairEntry.ddr.Column        = Entry->ddr.Column;
  RepairEntry.ddr.RepairType    = Entry->ddr.RepairType;
  RepairEntry.ddr.Row           = Entry->ddr.Row;
  RepairEntry.ddr.Socket        = Entry->ddr.Socket;
  RepairEntry.ddr.Channel       = Entry->ddr.Channel;
  RepairEntry.ddr.TargetDevice  = Entry->ddr.TargetDevice;

  RepairEntry.ddr.SubChannel    = Entry->ddr.SubChannel;
  RepairEntry.ddr.HardPPRDone   = Entry->ddr.HardPPRDone;
  RepairEntry.ddr.PPRUndo       = Entry->ddr.PPRUndo;
  RepairEntry.ddr.PPRLock       = Entry->ddr.PPRLock;
  RepairEntry.ddr.SerialNumber  = Entry->ddr.SerialNumber;
  RepairEntry.ddr.DeviceTypeToRepair = Entry->ddr.DeviceTypeToRepair;

  ApcbSetPurpose (APCB_TYPE_PURPOSE_EVENT_LOGGING);
  return ApcbAddDramPostPkgRepairEntry (&RepairEntry);
}

/**
 *
 * Routine Description:
 *
 *  This function removes a DRAM POST Package repair entry
 *
 *  @param[in]     Entry      DRAM POST Package repair entry to be removed
 *
 *  @retval EFI_SUCCESS        DRAM POST Package repair entry removed successfully
 *
 **/
EFI_STATUS
EFIAPI
mApcbRemoveDramPostPkgRepairEntry (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       DPPR_REPAIR_ENTRY_V2 *Entry
  )
{
  APCB_DPPRCL_REPAIR_ENTRY_V2  RepairEntry;

  RepairEntry.ddr.Valid         = 1;
  RepairEntry.ddr.Bank          = Entry->ddr.Bank;
  RepairEntry.ddr.RankMultiplier= Entry->ddr.RankMultiplier;
  RepairEntry.ddr.Device        = Entry->ddr.Device;
  RepairEntry.ddr.ChipSelect    = Entry->ddr.ChipSelect;
  RepairEntry.ddr.Column        = Entry->ddr.Column;
  RepairEntry.ddr.RepairType    = Entry->ddr.RepairType;
  RepairEntry.ddr.Row           = Entry->ddr.Row;
  RepairEntry.ddr.Socket        = Entry->ddr.Socket;
  RepairEntry.ddr.Channel       = Entry->ddr.Channel;
  RepairEntry.ddr.TargetDevice  = Entry->ddr.TargetDevice;

  RepairEntry.ddr.SubChannel    = Entry->ddr.SubChannel;
  RepairEntry.ddr.HardPPRDone   = Entry->ddr.HardPPRDone;
  RepairEntry.ddr.PPRUndo       = Entry->ddr.PPRUndo;
  RepairEntry.ddr.PPRLock       = Entry->ddr.PPRLock;
  RepairEntry.ddr.SerialNumber  = Entry->ddr.SerialNumber;
  RepairEntry.ddr.DeviceTypeToRepair = Entry->ddr.DeviceTypeToRepair;

  ApcbSetPurpose (APCB_TYPE_PURPOSE_EVENT_LOGGING);
  return ApcbRemoveDramPostPkgRepairEntry (&RepairEntry);
}


EFI_STATUS
EFIAPI
ApcbSmmCommunicateHandler (
  IN       EFI_HANDLE  DispatchHandle,
  IN       CONST VOID  *Context,
  IN OUT   VOID    *CommBuffer,
  IN OUT   UINTN   *CommBufferSize
  )
{
  EFI_STATUS Status;
  APCB_SMM_COMMUNICATION_CMN                      *SmmCommParameterCommon;
  APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER           *SmmCommAccessConfigParameter;
  APCB_SMM_COMM_ACCESS_TOKEN                      *SmmCommAccessToken;
  APCB_SMM_COMM_ACCESS_TYPE                       *SmmCommAccessType;
  APCB_SMM_COMM_PURGE_TOKENS                      *SmmCommPurgeTokens;
  APCB_SMM_COMM_PURGE_ALL                         *SmmCommPurgeAll;
  APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *SmmCommAccessDramPostPkgRepairEntry;
  UINT8                                           *OutputBuffer;
  UINTN                                           TempCommBufferSize;
  UINT8                                           *TempCommBuffer;

  Status         = EFI_UNSUPPORTED;
  TempCommBuffer = NULL;
  //
  // If input is invalid, stop processing this SMI
  //
  if (CommBuffer == NULL || CommBufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TempCommBufferSize = *CommBufferSize;
  // too small buffer size may allow TOC/TOU attack, too big buffer size may cause stack overflow or heap overwritten
  if (TempCommBufferSize < sizeof (APCB_SMM_COMMUNICATION_CMN) || TempCommBufferSize > AMD_PSP_SMM_TMP_BUFFER_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
    return EFI_INVALID_PARAMETER;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN) CommBuffer, TempCommBufferSize)) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  // To ensure the security, an communication buffer should be allocated in the SMRAM.
  // #SMM.3.5: Boot firmware SMM module MUST copy the communication buffer to SMRAM before the check,
  // to resist TOC/TOU or DMA attacks.
  TempCommBuffer = GetAmdPspSmmRunTimeBufferAddress ();
  if (TempCommBuffer == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler: TempCommBuffer Address is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem ((VOID *) TempCommBuffer, AMD_PSP_SMM_TMP_BUFFER_SIZE);
  CopyMem ((VOID *) TempCommBuffer, (VOID *) CommBuffer, TempCommBufferSize);

  SmmCommParameterCommon = (APCB_SMM_COMMUNICATION_CMN *)TempCommBuffer;
  SmmCommAccessConfigParameter = (APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER *)TempCommBuffer;
  SmmCommAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)TempCommBuffer;
  SmmCommAccessType = (APCB_SMM_COMM_ACCESS_TYPE *)TempCommBuffer;
  SmmCommPurgeTokens = (APCB_SMM_COMM_PURGE_TOKENS *)TempCommBuffer;
  SmmCommPurgeAll = (APCB_SMM_COMM_PURGE_ALL *)TempCommBuffer;
  SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *)TempCommBuffer;
  IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler ID %x\n", SmmCommParameterCommon->id);

  switch (SmmCommParameterCommon->id) {
    case CBS_APCB_COMM_ID_FLUSH_DATA:
      Status = mApcbFlushData (&mApcbSmmServiceProtocol);
      break;
    case CBS_APCB_COMM_ID_GET_CONFIG:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbGetConfigParameter (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessConfigParameter->TokenId,
                    &SmmCommAccessConfigParameter->SizeInByte,
                    &SmmCommAccessConfigParameter->Value
                    );
      break;

    case CBS_APCB_COMM_ID_SET_CONFIG:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbSetConfigParameter (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessConfigParameter->TokenId,
                    &SmmCommAccessConfigParameter->SizeInByte,
                    &SmmCommAccessConfigParameter->Value
                    );
      break;

    case CBS_APCB_COMM_ID_ACQUIRE_MUTEX:
      Status = mApcbAcquireMutex (&mApcbSmmServiceProtocol);
      break;

    case CBS_APCB_COMM_ID_RELEASE_MUTEX:
      Status = mApcbReleaseMutex (&mApcbSmmServiceProtocol);
      break;

    case CBS_APCB_COMM_ID_GET_TOKEN_BOOL:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbGetTokenBool (
                    &mApcbSmmServiceProtocol,
                    &SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (BOOLEAN *)&SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_SET_TOKEN_BOOL:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbSetTokenBool (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (BOOLEAN)SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_GET_TOKEN_8:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbGetToken8 (
                    &mApcbSmmServiceProtocol,
                    &SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (UINT8 *)&SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_SET_TOKEN_8:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbSetToken8 (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (UINT8)SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_GET_TOKEN_16:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbGetToken16 (
                    &mApcbSmmServiceProtocol,
                    &SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (UINT16 *)&SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_SET_TOKEN_16:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbSetToken16 (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (UINT16)SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_GET_TOKEN_32:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbGetToken32 (
                    &mApcbSmmServiceProtocol,
                    &SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (UINT32 *)&SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_SET_TOKEN_32:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TOKEN)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbSetToken32 (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessToken->Purpose,
                    SmmCommAccessToken->Uid,
                    (UINT32)SmmCommAccessToken->Value
                    );
      break;

    case CBS_APCB_COMM_ID_GET_TYPE:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TYPE)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbGetType (
                    &mApcbSmmServiceProtocol,
                    &SmmCommAccessType->Purpose,
                    SmmCommAccessType->GroupId,
                    SmmCommAccessType->TypeId,
                    SmmCommAccessType->InstanceId,
                    &OutputBuffer,
                    &SmmCommAccessType->DataSize
                    );
      if (!EFI_ERROR(Status)) {
        ASSERT (SmmCommAccessType->DataSize <= sizeof(SmmCommAccessType->DataBuffer));
        CopyMem (SmmCommAccessType->DataBuffer, OutputBuffer, SmmCommAccessType->DataSize);
      }
      break;

    case CBS_APCB_COMM_ID_SET_TYPE:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_TYPE)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbSetType (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessType->Purpose,
                    SmmCommAccessType->GroupId,
                    SmmCommAccessType->TypeId,
                    SmmCommAccessType->InstanceId,
                    SmmCommAccessType->DataBuffer,
                    SmmCommAccessType->DataSize
                    );
      break;

    case CBS_APCB_COMM_ID_PURGE_TOKENS:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_PURGE_TOKENS)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
      SmmCommPurgeTokens = (APCB_SMM_COMM_PURGE_TOKENS *) TempCommBuffer;
      if (!SmmIsBufferOutsideSmmValid ((UINTN) SmmCommPurgeTokens->ApcbTokenList,
          sizeof (UINT32) * SmmCommPurgeTokens->TokenCount)) {
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      /// To ensure the security, an communication buffer should be allocated in the SMRAM, and copy
      /// the data of communication buffer to SMRAM buffer
      SmmCommPurgeTokens = (APCB_SMM_COMM_PURGE_TOKENS *) TempCommBuffer;
      Status = mApcbPurgeTokens (
                    &mApcbSmmServiceProtocol,
                    SmmCommPurgeTokens->Purpose,
                    SmmCommPurgeTokens->ApcbTokenList,
                    SmmCommPurgeTokens->TokenCount
                    );
      break;

    case CBS_APCB_COMM_ID_PURGE_ALL_TOKENS:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_PURGE_ALL)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbPurgeAllTokens (
                    &mApcbSmmServiceProtocol,
                    SmmCommPurgeAll->Purpose
                    );
      break;

    case CBS_APCB_COMM_ID_PURGE_ALL_TYPES:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_PURGE_ALL)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      Status = mApcbPurgeAllTypes (
                    &mApcbSmmServiceProtocol,
                    SmmCommPurgeAll->Purpose
                    );
      break;

    case CBS_APCB_COMM_ID_GET_DRAM_POST_PKG_REPAIR_ENTRY:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
      SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *) TempCommBuffer;
      if (!SmmIsBufferOutsideSmmValid ((UINTN) SmmCommAccessDramPostPkgRepairEntry->Entry,
          sizeof (DPPR_REPAIR_ENTRY_V2))) {
        Status = EFI_INVALID_PARAMETER;
        return Status;
      }
      /// To ensure the security, an communication buffer should be allocated in the SMRAM, and copy
      /// the data of communication buffer to SMRAM buffer
      SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *) TempCommBuffer;
      Status = mApcbGetDramPostPkgRepairEntry (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessDramPostPkgRepairEntry->Entry,
                    &SmmCommAccessDramPostPkgRepairEntry->NumOfEntries
                    );
      break;

    case CBS_APCB_COMM_ID_CLEAR_DRAM_POST_PKG_REPAIR_ENTRY:
      Status = mApcbClearDramPostPkgRepairEntry (
                    &mApcbSmmServiceProtocol
                    );
      break;

    case CBS_APCB_COMM_ID_ADD_DRAM_POST_PKG_REPAIR_ENTRY:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
      SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *) TempCommBuffer;
      if (!SmmIsBufferOutsideSmmValid ((UINTN) SmmCommAccessDramPostPkgRepairEntry->Entry,
          sizeof (DPPR_REPAIR_ENTRY_V2))) {
        Status = EFI_INVALID_PARAMETER;
        return Status;
      }
      /// To ensure the security, an communication buffer should be allocated in the SMRAM, and copy
      /// the data of communication buffer to SMRAM buffer
      SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *) TempCommBuffer;
      Status = mApcbAddDramPostPkgRepairEntry (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessDramPostPkgRepairEntry->Entry
                    );
      break;

    case CBS_APCB_COMM_ID_REMOVE_DRAM_POST_PKG_REPAIR_ENTRY:
      if (TempCommBufferSize < sizeof (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ApcbSmmCommunicateHandler Command Buffer Size invalid!\n");
        Status = EFI_INVALID_PARAMETER;
        break;
      }
      /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
      SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *) TempCommBuffer;
      if (!SmmIsBufferOutsideSmmValid ((UINTN) SmmCommAccessDramPostPkgRepairEntry->Entry,
          sizeof (DPPR_REPAIR_ENTRY_V2))) {
        Status = EFI_INVALID_PARAMETER;
        return Status;
      }
      /// To ensure the security, an communication buffer should be allocated in the SMRAM, and copy
      /// the data of communication buffer to SMRAM buffer
      SmmCommAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *) TempCommBuffer;
      Status = mApcbRemoveDramPostPkgRepairEntry (
                    &mApcbSmmServiceProtocol,
                    SmmCommAccessDramPostPkgRepairEntry->Entry
                    );
      break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  SmmCommParameterCommon->Status = Status;

  // copy the content of buffer in SMRAM back into the buffer outside of SMRAM
  CopyMem ((VOID *) CommBuffer, (VOID *) TempCommBuffer, TempCommBufferSize);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdApcbV3SmmDriverEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;

  AGESA_TESTPOINT (TpApcbSmmEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("APCB V3 SMM Driver Entry\n");
  Handle = NULL;
  //Install APCB service Protocol
  Status = gSmst->SmmInstallProtocolInterface (
               &Handle,
               &gAmdApcbSmmServiceProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &mApcbSmmServiceProtocol
               );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Register ApcbSmmCommunicateHandler\n");
    mApcbSmmCommHandle = NULL;
    Status = gSmst->SmiHandlerRegister (ApcbSmmCommunicateHandler, &gApcbSmmCommHandleGuid, &mApcbSmmCommHandle);
    ASSERT (!EFI_ERROR(Status));
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("APCB V3 SMM Driver Exit\n");
  AGESA_TESTPOINT (TpApcbSmmExit, NULL);
  return EFI_SUCCESS;
}


