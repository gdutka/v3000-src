/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DeBugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/OemApcbV3SmmServiceCommLib.h>
#include <Protocol/SmmCommunication.h>
#include <OemApcbV3SmmServiceCommunication.h>


/**
 * @brief Update Apcb Data
 *
 * @param[in][out]    *Value
 *
 * @return            EFI_STATUS
 */
EFI_STATUS
OemApcbV3SmmServiceCommUpdateApcb (
  IN VOID   *Value
)
{
  EFI_STATUS Status;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  APCB_SMM_COMM_UPDATE_VALUE      *ApcbSmmUpdateValue;

  Status             = EFI_UNSUPPORTED;
  SmmCommunication   = NULL;
  SmmCommBuff        = NULL;
  ApcbSmmUpdateValue = NULL;

  SmmCommBuff = (EFI_SMM_COMMUNICATE_HEADER *)(UINTN) PcdGet64 (PcdOemSmmCommunicationAddress);
  if (SmmCommBuff == NULL) {
    DEBUG ((EFI_D_ERROR, "\t[%a] SmmCommBuff is NULL, exit\n", __FUNCTION__));
    return Status;
  }

  CopyGuid (&(SmmCommBuff->HeaderGuid), &gOemApcbV3SmmServiceCommHandleGuid);
  SmmCommBuff->MessageLength = APCB_SMM_COMM_ACCESS_VALUE_BUFFER_SIZE;

  ApcbSmmUpdateValue        = (APCB_SMM_COMM_UPDATE_VALUE *) SmmCommBuff->Data;
  ApcbSmmUpdateValue->Value = *(UINT32 *)Value;

  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "\t[%a] gEfiSmmCommunicationProtocolGuid Locate fail, exit\n", __FUNCTION__));
    ASSERT (!EFI_ERROR(Status));
    return Status;
  }

  // SmmCommBuff will be passed to OemApcbV3SmmServiceCommHandleFun
  Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBuff->MessageLength);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "\t[%a] SmmCommunication fail, exit\n", __FUNCTION__));
    ASSERT (!EFI_ERROR(Status));
    return Status;
  }

  Status = ApcbSmmUpdateValue->Status;

  return Status;
}

