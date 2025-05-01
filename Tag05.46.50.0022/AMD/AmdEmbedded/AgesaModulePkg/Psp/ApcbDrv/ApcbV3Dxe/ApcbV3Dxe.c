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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

//#include "AGESA.h"
#include "Porting.h"
#include "SocLogicalId.h"
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Priority.h>
#include "ApcbV3Dxe.h"
#include <Library/ApcbLibV3.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/SmmCommunication.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Filecode.h>
#include <AmdApcbSmmCommunication.h>

#define FILECODE PSP_APCBDRV_APCBV3DXE_APCBV3DXE_FILECODE

extern EFI_GUID gAmdApcbDxeServiceProtocolGuid;

EFI_STATUS
DefRet (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

STATIC AMD_APCB_SERVICE_PROTOCOL mApcbDxeServiceProtocol = {
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

EFI_EVENT                 ApcbUpdateShadowDimmConfigEvent;
VOID                      *mRegistrationForApcbUpdateShadowDimmConfigEvent;
BOOLEAN                   mSmmLocked = FALSE;

/*++

Routine Description:

 *  This function initialize and return a buffer for SMM communicate.
 *
 *  @param[in]     Id               SMM communicate function ID
 *  @param[in]     MessageLength    MessageLength of SMM communicate buffer
 *
--*/
EFI_SMM_COMMUNICATE_HEADER *
ApcbSmmCommunicateCommonInitializeBuffer(
  IN     UINT32                      id,
  IN     UINTN                       MessageLength
)
{
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMMUNICATION_CMN      *ApcbSmmComm;

  SmmCommBuff = (EFI_SMM_COMMUNICATE_HEADER *)(UINTN) PcdGet64 (PcdAmdSmmCommunicationAddress);
  if (SmmCommBuff == NULL) {
    return NULL;
  }

  CopyGuid (&(SmmCommBuff->HeaderGuid), &gApcbSmmCommHandleGuid);
  SmmCommBuff->MessageLength = MessageLength;
  ApcbSmmComm = (APCB_SMM_COMMUNICATION_CMN *)SmmCommBuff->Data;
  ApcbSmmComm->id = id;

  return SmmCommBuff;
}

/*++

Routine Description:

 *  This function flush the APCB data back to SPI ROM
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMMUNICATION_CMN      *ApcbSmmComm;
  UINTN                           SmmCommBufferSize;

  //
  // Write back Shadow Copy in DRAM
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("Write back APCB to SPI\n");
  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_NO_PARAMETER_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_FLUSH_DATA, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);
      ApcbSmmComm= (APCB_SMM_COMMUNICATION_CMN *)(SmmCommBuff->Data);

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmComm->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }
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
 *          EFI_NOT_FOUND  Can't find the according APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER  *SmmCommAccessConfig;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_CONFIG, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      SmmCommAccessConfig = (APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER *)(SmmCommBuff->Data);
      SmmCommAccessConfig->TokenId = TokenId;
      SmmCommAccessConfig->SizeInByte = *SizeInByte;
      SmmCommAccessConfig->Value = *Value;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      if (!EFI_ERROR(SmmCommAccessConfig->Status)) {
        *SizeInByte = SmmCommAccessConfig->SizeInByte;
        *Value = SmmCommAccessConfig->Value;
      }
      return SmmCommAccessConfig->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }
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
 *          EFI_NOT_FOUND  Can't find the according APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER  *SmmCommAccessConfig;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_SET_CONFIG, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      SmmCommAccessConfig = (APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER *)(SmmCommBuff->Data);
      SmmCommAccessConfig->TokenId = TokenId;
      SmmCommAccessConfig->SizeInByte = *SizeInByte;
      SmmCommAccessConfig->Value = *Value;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return SmmCommAccessConfig->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }
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
 *  @retval EFI_ACCESS_DENIED  APCB cannot be accessed
 *
 **/
EFI_STATUS
EFIAPI
mApcbAcquireMutex (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  )
{
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMMUNICATION_CMN      *ApcbSmmComm;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_NO_PARAMETER_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_ACQUIRE_MUTEX, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);
      ApcbSmmComm= (APCB_SMM_COMMUNICATION_CMN *)(SmmCommBuff->Data);

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmComm->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }
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
 *
 **/
EFI_STATUS
EFIAPI
mApcbReleaseMutex (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This
  )
{
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMMUNICATION_CMN      *ApcbSmmComm;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_NO_PARAMETER_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_RELEASE_MUTEX, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);
      ApcbSmmComm= (APCB_SMM_COMMUNICATION_CMN *)(SmmCommBuff->Data);

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmComm->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }
  ApcbReleaseMutex ();

  return EFI_SUCCESS;
}

/**
 *
 * Routine Description:
 *
 *   This function gets an Boolean APCB token value
 *
 *  @param[in]          This               Pointer to the instance of APCB Protocol
 *  @param[out]         Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]          Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]         bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Can't find the APCB token
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
  UINT8                           CurrentPriorityLevel;
  UINT8                           StartingPriorityLevel;
  UINT8                           EndingPriorityLevel;
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_TOKEN_BOOL, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Uid = Uid;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      if (!EFI_ERROR(ApcbSmmAccessToken->Status)) {
        *Purpose = ApcbSmmAccessToken->Purpose;
        *bValue = (BOOLEAN)ApcbSmmAccessToken->Value;
      }
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Can't set the APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_SET_TOKEN_BOOL, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Purpose = Purpose;
      ApcbSmmAccessToken->Uid = Uid;
      ApcbSmmAccessToken->Value = (UINT32)bValue;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]          This               Pointer to the instance of APCB Protocol
 *  @param[out]         Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]          Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]         Value8             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Can't find the APCB token
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
  UINT8                           CurrentPriorityLevel;
  UINT8                           StartingPriorityLevel;
  UINT8                           EndingPriorityLevel;
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_TOKEN_8, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Uid = Uid;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      if (!EFI_ERROR(ApcbSmmAccessToken->Status)) {
        *Purpose = ApcbSmmAccessToken->Purpose;
        *Value8 = (UINT8)ApcbSmmAccessToken->Value;
      }
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     Value8             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Can't set the APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_SET_TOKEN_8, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Purpose = Purpose;
      ApcbSmmAccessToken->Uid = Uid;
      ApcbSmmAccessToken->Value = (UINT32)Value8;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]          This               Pointer to the instance of APCB Protocol
 *  @param[out]         Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]          Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]         bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Can't find the APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_TOKEN_16, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Uid = Uid;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      if (!EFI_ERROR(ApcbSmmAccessToken->Status)) {
        *Purpose = ApcbSmmAccessToken->Purpose;
        *Value16 = (UINT16)ApcbSmmAccessToken->Value;
      }
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     Value16            Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Can't set the APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_SET_TOKEN_16, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Purpose = Purpose;
      ApcbSmmAccessToken->Uid = Uid;
      ApcbSmmAccessToken->Value = (UINT32)Value16;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]          This               Pointer to the instance of APCB Protocol
 *  @param[out]         Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]          Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[out]         bValue             Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *          EFI_NOT_FOUND  Can't find the APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_TOKEN_32, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Uid = Uid;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      if (!EFI_ERROR(ApcbSmmAccessToken->Status)) {
        *Purpose = ApcbSmmAccessToken->Purpose;
        *Value32 = ApcbSmmAccessToken->Value;
      }
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
 *  @param[in]     Uid                APCB token UID defined in ApcbV3TokenUid.h
 *  @param[in]     Value32            Value of APCB token ID
 *
 *  @retval EFI_SUCCESS           Set APCB value successfully
 *          EFI_OUT_OF_RESOURCES  Can't set the APCB token
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TOKEN      *ApcbSmmAccessToken;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_SET_TOKEN_32, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessToken = (APCB_SMM_COMM_ACCESS_TOKEN *)(SmmCommBuff->Data);
      ApcbSmmAccessToken->Purpose = Purpose;
      ApcbSmmAccessToken->Uid = Uid;
      ApcbSmmAccessToken->Value = Value32;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmAccessToken->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]          This               Pointer to the instance of APCB Protocol
 *  @param[out]         Purpose            Purpose of the APCB type where the data is retrieved
 *  @param[in]          GroupId            Group ID
 *  @param[in]          TypeId             Type ID
 *  @param[in]          InstanceId         Instance ID
 *  @param[out]         DataBuf            Pointer to the type data
 *  @param[out]         DataSize           Pointer to the size of the type data
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TYPE       *ApcbSmmAccessType;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TYPE_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_TYPE, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessType = (APCB_SMM_COMM_ACCESS_TYPE *)(SmmCommBuff->Data);
      ApcbSmmAccessType->GroupId = GroupId;
      ApcbSmmAccessType->TypeId = TypeId;
      ApcbSmmAccessType->InstanceId = InstanceId;
      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      if (!EFI_ERROR(ApcbSmmAccessType->Status)) {
        *Purpose = ApcbSmmAccessType->Purpose;
        *DataSize = ApcbSmmAccessType->DataSize;
        *DataBuf = &(ApcbSmmAccessType->DataBuffer[0]);
      }
      return ApcbSmmAccessType->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 * @param[in]     Purpose            Purpose of the current operation
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_TYPE       *ApcbSmmAccessType;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_TYPE_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_SET_TYPE, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmAccessType = (APCB_SMM_COMM_ACCESS_TYPE *)(SmmCommBuff->Data);
      ApcbSmmAccessType->Purpose = Purpose;
      ApcbSmmAccessType->GroupId = GroupId;
      ApcbSmmAccessType->TypeId = TypeId;
      ApcbSmmAccessType->InstanceId = InstanceId;
      ApcbSmmAccessType->DataSize = DataSize;
      ASSERT (DataSize <= sizeof(ApcbSmmAccessType->DataBuffer));
      CopyMem (ApcbSmmAccessType->DataBuffer, DataBuf, DataSize);

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmAccessType->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
 *
 *  @retval EFI_SUCCESS           All tokens are purged
 *          EFI_NOT_FOUND         All tokens are not purged
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_PURGE_TOKENS      *ApcbSmmPurgeTokens;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_PURGE_TOKENS_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_PURGE_TOKENS, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmPurgeTokens = (APCB_SMM_COMM_PURGE_TOKENS *)(SmmCommBuff->Data);
      ApcbSmmPurgeTokens->Purpose = Purpose;
      ApcbSmmPurgeTokens->ApcbTokenList = ApcbTokenList;
      ApcbSmmPurgeTokens->TokenCount = TokenCount;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmPurgeTokens->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
 *
 *  @retval EFI_SUCCESS           All tokens are purged
 *          EFI_NOT_FOUND         All tokens are not purged
 *
 **/
EFI_STATUS
EFIAPI
mApcbPurgeAllTokens (
  IN       AMD_APCB_SERVICE_PROTOCOL    *This,
  IN       UINT8                        Purpose
  )
{
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_PURGE_ALL        *ApcbSmmPurgeAll;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_PURGE_ALL_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_PURGE_ALL_TOKENS, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmPurgeAll = (APCB_SMM_COMM_PURGE_ALL *)(SmmCommBuff->Data);
      ApcbSmmPurgeAll->Purpose = Purpose;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmPurgeAll->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
 *  @param[in]     Purpose            Purpose of the current operation
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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_PURGE_ALL        *ApcbSmmPurgeAll;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_PURGE_ALL_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_PURGE_ALL_TYPES, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbSmmPurgeAll = (APCB_SMM_COMM_PURGE_ALL *)(SmmCommBuff->Data);
      ApcbSmmPurgeAll->Purpose = Purpose;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmPurgeAll->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
  EFI_STATUS                    Status;
  UINT8                         i;
  APCB_DPPRCL_REPAIR_ENTRY_V2   *RepairEntry;
  UINT32                        SizeInByte;
  UINT8                         CurrentPriorityLevel;
  UINT8                         StartingPriorityLevel;
  UINT8                         EndingPriorityLevel;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *ApcbAccessDramPostPkgRepairEntry;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_GET_DRAM_POST_PKG_REPAIR_ENTRY, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *)(SmmCommBuff->Data);
      ApcbAccessDramPostPkgRepairEntry->Entry = Entry;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));

      if (!EFI_ERROR(ApcbAccessDramPostPkgRepairEntry->Status)) {
        *NumOfEntries = ApcbAccessDramPostPkgRepairEntry->NumOfEntries;
      }
      return ApcbAccessDramPostPkgRepairEntry->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMMUNICATION_CMN      *ApcbSmmComm;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_NO_PARAMETER_BUFFER_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_CLEAR_DRAM_POST_PKG_REPAIR_ENTRY, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);
      ApcbSmmComm= (APCB_SMM_COMMUNICATION_CMN *)(SmmCommBuff->Data);

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbSmmComm->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }
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
  APCB_DPPRCL_REPAIR_ENTRY_V2     RepairEntry;
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *ApcbAccessDramPostPkgRepairEntry;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_ADD_DRAM_POST_PKG_REPAIR_ENTRY, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *)(SmmCommBuff->Data);
      ApcbAccessDramPostPkgRepairEntry->Entry = Entry;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbAccessDramPostPkgRepairEntry->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }


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
  EFI_STATUS                      Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *ApcbAccessDramPostPkgRepairEntry;
  UINTN                           SmmCommBufferSize;

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock) && mSmmLocked) {
    if (PcdGetBool(PcdTransferApcbDxeToSmmAfterSmmLock)) {
      Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
      ASSERT (!EFI_ERROR(Status));

      SmmCommBufferSize = APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY_SIZE;
      SmmCommBuff = ApcbSmmCommunicateCommonInitializeBuffer (CBS_APCB_COMM_ID_REMOVE_DRAM_POST_PKG_REPAIR_ENTRY, SmmCommBufferSize);
      ASSERT (SmmCommBuff != NULL);

      ApcbAccessDramPostPkgRepairEntry = (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY *)(SmmCommBuff->Data);
      ApcbAccessDramPostPkgRepairEntry->Entry = Entry;

      Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
      ASSERT (!EFI_ERROR(Status));
      return ApcbAccessDramPostPkgRepairEntry->Status;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Reject due to SMM locked\n");
      return EFI_ABORTED;
    }
  }

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

/**
 *
 *  Dxe SmmReadyToLock callback
 *
 **/
VOID
EFIAPI
ApcbV3DxeRTBCallBack (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("APCB V3 DXE ApcbRTBDxeCallBack\n");
  mSmmLocked = TRUE;
}

EFI_STATUS
EFIAPI
AmdApcbV3DxeDriverEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;
  VOID                        *Registration;
  EFI_EVENT                   DxeSmmReadyToLockEvent;

  AGESA_TESTPOINT (TpApcbDxeEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("APCB V3 DXE Driver Entry\n");
  Handle = NULL;
  //Install APCB service Protocol
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdApcbDxeServiceProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mApcbDxeServiceProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  //
  // Set up call back after GnbMidDxe.
  //
  Status = gBS->CreateEventEx (
                          EVT_NOTIFY_SIGNAL,
                          TPL_NOTIFY,
                          AmdPspApcbUpdateShadowDimmConfig,
                          NULL,
                          NULL,
                          &ApcbUpdateShadowDimmConfigEvent
                          );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  Status = gBS->RegisterProtocolNotify (
                          &gPspFlashAccSmmCommReadyProtocolGuid,
                          ApcbUpdateShadowDimmConfigEvent,
                          &(mRegistrationForApcbUpdateShadowDimmConfigEvent)
                          );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  if (PcdGetBool(PcdLockApcbDxeAfterSmmLock)) {
    //
    // Set up call back on SmmReadyToLock
    //
    Status = gBS->CreateEventEx (
                           EVT_NOTIFY_SIGNAL,
                           TPL_NOTIFY,
                           ApcbV3DxeRTBCallBack,
                           NULL,
                           NULL,
                           &DxeSmmReadyToLockEvent
                           );
    ASSERT (!EFI_ERROR (Status));

    Status = gBS->RegisterProtocolNotify (
                          &gEfiDxeSmmReadyToLockProtocolGuid,
                          DxeSmmReadyToLockEvent,
                          &Registration
                          );
    ASSERT (!EFI_ERROR (Status));
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("APCB V3 DXE Driver Exit\n");
  AGESA_TESTPOINT (TpApcbDxeExit, NULL);
  return EFI_SUCCESS;
}


