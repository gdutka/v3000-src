/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Base.h>
#include "AGESA.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Protocol/PspMboxSmmBufferAddressProtocol.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPP2CMBOXV2_AMDPSPP2CMBOXV2SMMBUFFER_FILECODE


#define PSP_SMM_BUFFER_ALIGNMENT    (0x20u) // Must be 32-byte aligned
#define PSP_MBOX_SMM_BUFFER_OFFSET  0u
#define PSP_MBOX_SMM_FLAG_OFFSET    0x00000C00 //Start from 3K


PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL mPspMboxSmmBufferAddressProtocol;

EFI_STATUS
EFIAPI
AmdPspP2CmboxV2SmmBufferEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINT8                           *LocalPspMboxSmmBuffer;
  UINT8                           *PspMboxSmmBuffer;
  BOOLEAN                         *PspMboxSmmFlagAddr;
  EFI_HANDLE                      Handle;
  UINT32                          PspSmmBufferPages;

  LocalPspMboxSmmBuffer = NULL;
  PspMboxSmmBuffer = NULL;
  PspMboxSmmFlagAddr = NULL;
  PspSmmBufferPages = 0;

  AGESA_TESTPOINT (TpPspP2CmboxV2Entry, NULL);
  //We are now in SMM
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.AmdPspP2CmboxSmmBuffer Enter\n");
  PspSmmBufferPages = PcdGet32 (PcdAmdPspSmmBufferPages);

  // Allocate SMM buffer to hold C2P message after bootdone
  LocalPspMboxSmmBuffer = AllocateAlignedPages(PspSmmBufferPages, PSP_SMM_BUFFER_ALIGNMENT);
  ZeroMem (LocalPspMboxSmmBuffer, PspSmmBufferPages * EFI_PAGE_SIZE);
  ASSERT (LocalPspMboxSmmBuffer != NULL);
  if (LocalPspMboxSmmBuffer != NULL) {
    //Assign the address for C2P RT buffer, C2P SMM flag
    PspMboxSmmBuffer = LocalPspMboxSmmBuffer + PSP_MBOX_SMM_BUFFER_OFFSET;
    PspMboxSmmFlagAddr = LocalPspMboxSmmBuffer + PSP_MBOX_SMM_FLAG_OFFSET;
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPspMboxSmmBuffer 0x%x\n", PspMboxSmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPspMboxSmmFlagAddr 0x%x\n", PspMboxSmmFlagAddr);
    //Saving the address through SMM protocol
    Handle = NULL;
    mPspMboxSmmBufferAddressProtocol.PspMboxSmmBuffer = PspMboxSmmBuffer;
    mPspMboxSmmBufferAddressProtocol.PspMboxSmmFlagAddr = PspMboxSmmFlagAddr;
    gSmst->SmmInstallProtocolInterface (
                      &Handle,
                      &gPspMboxSmmBufferAddressProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mPspMboxSmmBufferAddressProtocol
                      );
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspP2CmboxSmmBuffer Exit\n");
  AGESA_TESTPOINT (TpPspP2CmboxV2Exit, NULL);

  return EFI_SUCCESS;
}



