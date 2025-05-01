/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "OemApcbV3SmmService.h"
#include <OemApcbV3SmmServiceCommunication.h>
#include <Library/AmdPspDxeSmmBufLib.h>
#include <Library/BaseMemoryLib.h>

#define OEM_SMM_COMMUNICATION_BUFFER_SIZE (4 * 1024)

STATIC EFI_HANDLE mOemApcbV3SmmServiceCommHandle = NULL;

/**
 * @brief SMM communication handler used to handle the SMM service request from normal world
 *        Used for updating APCB.
 *
 * @param[in]         DispatchHandle
 * @param[in]         Context
 * @param[in][out]    *CommBuffer
 * @param[in][out]    *CommBufferSize
 *
 * @return            EFI_STATUS
 */
EFI_STATUS
EFIAPI
OemApcbV3SmmServiceCommHandleFun (
  IN       EFI_HANDLE  DispatchHandle,
  IN       CONST VOID  *Context,
  IN OUT   VOID    *CommBuffer,
  IN OUT   UINTN   *CommBufferSize
  )
{
  EFI_STATUS Status;
  AMD_APCB_SERVICE_PROTOCOL                       *ApcbSmmProtocol;
  APCB_SMM_COMMUNICATION_CMN                      *SmmCommParameterCommon;
  UINTN                                           TempCommBufferSize;
  UINT8                                           *TempCommBuffer;

  Status          = EFI_UNSUPPORTED;
  ApcbSmmProtocol = NULL;

  //
  // If input is invalid, stop processing this SMI
  //
  if (CommBuffer == NULL || CommBufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TempCommBufferSize = *CommBufferSize;
  // too small buffer size may allow TOC/TOU attack, too big buffer size may cause stack overflow or heap overwritten
  if (TempCommBufferSize < sizeof (APCB_SMM_COMMUNICATION_CMN) || TempCommBufferSize > AMD_PSP_SMM_TMP_BUFFER_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("OemApcbV3SmmServiceCommHandleFun communication Buffer Size invalid!\n");
    return EFI_INVALID_PARAMETER;
  }

  SmmCommParameterCommon = (APCB_SMM_COMMUNICATION_CMN *) CommBuffer;

  if (!SmmIsBufferOutsideSmmValid((UINTN)CommBuffer, TempCommBufferSize)) {
    DEBUG ((EFI_D_ERROR, "\t[%a] SMM communication data buffer in SMRAM or overflow!\n", __FUNCTION__));
    Status = EFI_INVALID_PARAMETER;
    SmmCommParameterCommon->Status = Status;
    ASSERT (!EFI_ERROR(Status));
    return Status;
  }

  // To ensure the security, an communication buffer should be allocated in the SMRAM.
  // #SMM.3.5: Boot firmware SMM module MUST copy the communication buffer to SMRAM before the check,
  // to resist TOC/TOU or DMA attacks.
  TempCommBuffer = GetAmdPspSmmRunTimeBufferAddress ();
  if (TempCommBuffer == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("TempCommBuffer Address is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem ((VOID *) TempCommBuffer, AMD_PSP_SMM_TMP_BUFFER_SIZE);
  CopyMem ((VOID *) TempCommBuffer, (VOID *) CommBuffer, TempCommBufferSize);

  ///
  /// TODO: use TempCommBuffer instead of using CommBuffer directly to avoid TOC/TOU attack
  ///

  Status = gSmst->SmmLocateProtocol(&gAmdApcbSmmServiceProtocolGuid, NULL, (VOID **)&ApcbSmmProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\t[%a] gAmdApcbSmmServiceProtocolGuid Locate fail, exit\n", __FUNCTION__));
    SmmCommParameterCommon->Status = Status;
    ASSERT (!EFI_ERROR(Status));
    return Status;
  }

  if (ApcbSmmProtocol == NULL) {
    DEBUG ((EFI_D_ERROR, "\t[%a] ApcbSmmProtocol is NULL, exit\n", __FUNCTION__));
    Status = EFI_UNSUPPORTED;
    SmmCommParameterCommon->Status = Status;
    ASSERT (!EFI_ERROR(Status));
    return Status;
  }

  ///
  /// TODO: Implement Code to Access Apcb Data Base.
  ///       Use ApcbSmmProtocol for all APCB related service.
  ///       e.g. ApcbSmmProtocol->ApcbSetToken8();
  ///

  SmmCommParameterCommon->Status = Status;
  // copy the content of buffer in SMRAM back into the buffer outside of SMRAM
  CopyMem ((VOID *) CommBuffer, (VOID *) TempCommBuffer, TempCommBufferSize);

  return Status;
}

/**
 * @brief Main Entry for this driver.
 * @details Register SMM communication handler used to update APCB
 *          After Smm Ready to Lock.
 *
 * @param[in] ImageHandle     Image handle this driver.
 * @param[in] SystemTable     Pointer to SystemTable.
 *
 * @return  EFI_STATUS
**/
EFI_STATUS
EFIAPI
OemApcbV3SmmServiceEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  VOID        *SmmCommBuffer;

  Status = EFI_UNSUPPORTED;

  DEBUG ((EFI_D_INFO, "%a Start...\n", __FUNCTION__));

  //Allocate RT buffer for Smm communication before EndOfDxe Event to satisfie WSMT test
  SmmCommBuffer = NULL;
  Status = gBS->AllocatePool (EfiRuntimeServicesData, OEM_SMM_COMMUNICATION_BUFFER_SIZE, &SmmCommBuffer);
  if(EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "\t[%a] Allocate RT Buffer Fail, Status: %r\n", __FUNCTION__, Status));
    ASSERT (!EFI_ERROR(Status));
    goto Exit;
  }

  //Save to PCD database
  PcdSet64S (PcdOemSmmCommunicationAddress, (UINT64) (UINTN) SmmCommBuffer);

  mOemApcbV3SmmServiceCommHandle = NULL;
  Status = gSmst->SmiHandlerRegister (
                    OemApcbV3SmmServiceCommHandleFun,
                    &gOemApcbV3SmmServiceCommHandleGuid,
                    &mOemApcbV3SmmServiceCommHandle
                    );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\t[%a] mOemApcbV3SmmServiceCommHandle Register Fail, Status: %r\n", __FUNCTION__, Status));
    FreePool (SmmCommBuffer);
    SmmCommBuffer = NULL;
    PcdSet64S (PcdOemSmmCommunicationAddress, (UINT64) (UINTN) 0x0);
    ASSERT (!EFI_ERROR(Status));
    goto Exit;
  }

Exit:
  DEBUG ((EFI_D_INFO, "%a Exit...\n", __FUNCTION__));

  return Status;
}

