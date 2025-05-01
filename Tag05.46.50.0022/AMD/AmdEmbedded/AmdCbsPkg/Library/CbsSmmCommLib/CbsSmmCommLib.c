/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <AmdCbsSmmCommunication.h>
#include <Protocol/SmmCommunication.h>
#include <Library/CbsSmmCommLib.h>
#include <Library/UefiBootServicesTableLib.h>


/**
 * @brief Call SMM communication with gCbsSmmCommHandleGuid
 *        Smm communication handler will call UpdateCbsApcbTokens which used to update all CBS APCB items
 *
 * @param IfrData
 * @return EFI_STATUS
 */
EFI_STATUS
CbsSmmCommUpdateApcb (
  IN UINTN   IfrDataSize,
  IN UINT8   *IfrData
)
{
  EFI_SMM_COMMUNICATE_HEADER      *SmmCommBuff;
  CBS_SMM_COMM_UPDATE_APCB        *CbsUpdateData;
  UINTN                           SmmCommBufferSize;
  EFI_SMM_COMMUNICATION_PROTOCOL  *SmmCommunication;
  EFI_STATUS                      Status;
  //
  // Update APCB in SMM
  //
  DEBUG ((EFI_D_ERROR, "Do Smm update Apcb\n"));
  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
  ASSERT (Status == EFI_SUCCESS);

  SmmCommBuff = (EFI_SMM_COMMUNICATE_HEADER *)(UINTN) PcdGet64 (PcdCbsSmmCommunicationAddress);
  CbsUpdateData = (CBS_SMM_COMM_UPDATE_APCB *)SmmCommBuff->Data;
  if ((CBS_SMM_COMM_UPDATE_APCB_BUFFER_SIZE + IfrDataSize) > PcdGet64 (PcdCbsSmmCommunicationBufferSize)) {
    DEBUG ((EFI_D_ERROR, "CBS SMM Communication buffer size is not enough! Required buffer size is %d bytes\n", CBS_SMM_COMM_UPDATE_APCB_BUFFER_SIZE + IfrDataSize));
    return EFI_BUFFER_TOO_SMALL;
  }
  CopyGuid (&(SmmCommBuff->HeaderGuid), &gCbsSmmCommHandleGuid);
  SmmCommBuff->MessageLength = sizeof(CBS_SMM_COMM_UPDATE_APCB) + IfrDataSize;

  CbsUpdateData->id = CBS_SMM_COMM_ID_UPDATE_APCB;
  CopyMem ((VOID *)&CbsUpdateData->CbsVariable[0], (VOID *)IfrData, IfrDataSize);

  SmmCommBufferSize = CBS_SMM_COMM_UPDATE_APCB_BUFFER_SIZE + IfrDataSize;
  Status = SmmCommunication->Communicate (SmmCommunication, SmmCommBuff, &SmmCommBufferSize);
  DEBUG ((EFI_D_ERROR, "Smm update Apcb : %r\n", Status));

  return Status;
}


