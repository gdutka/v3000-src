/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * PspP2Cmbox Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD PSP UEFI Driver
 * @e sub-project   PspP2Cmbox Driver
 * @e \$Revision$   @e \$Date$
 *
 */
#include "AmdPspP2CmboxV2.h"
#include <AmdPspSmmCommunication.h>
#include <Library/AmdPspFlashAccLib.h>
#include <Library/AmdPspDxeSmmBufLib.h>
#include <Library/SmmMemLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPP2CMBOXV2_AMDPSPFLASHACCSMMHDLR_FILECODE

extern UINTN                                 mBlockSize;
extern PSP_NV_DESCRIPTOR                     mPspNvDb[];
extern EFI_HANDLE                            mPspSmmCommHandle;
extern ENTRY_REGION                          mWritableEntryRegions[MAX_WRITABLE_ENTRY_REGIONS];
extern UINT32                                mEntryRegionIndex;
extern EFI_BOOT_MODE                         mBootMode;

#if AMD_PSP_SMM_TMP_BUFFER_SIZE < AMD_SMM_COMMUNICATION_BUFFER_SIZE
  #error AMD_PSP_SMM_TMP_BUFFER_SIZE need larger or equal to AMD_SMM_COMMUNICATION_BUFFER_SIZE
#endif

BOOLEAN
IsWritableRegion (
  IN       UINTN                   AddressToWrite,
  IN OUT   UINTN                   NumBytes
  )
{
  UINTN                        i;
  UINTN                        FlashAddressEnd;
  UINTN                        EntryAddressEnd;

  // check boot mode
  if ((mBootMode == BOOT_ON_FLASH_UPDATE) || (mBootMode == BOOT_IN_RECOVERY_MODE)) {
    return TRUE;
  }

  // Overflow check
  FlashAddressEnd = AddressToWrite + NumBytes - 1;
  if (FlashAddressEnd <= AddressToWrite) {
    return FALSE;
  }

  // check writable
  for (i = 0; i < mEntryRegionIndex; i++) {
    EntryAddressEnd = mWritableEntryRegions[i].Address + mWritableEntryRegions[i].Size - 1;
    if ((EntryAddressEnd > mWritableEntryRegions[i].Address) &&
        (AddressToWrite >= mWritableEntryRegions[i].Address) &&
        (FlashAddressEnd <= EntryAddressEnd)) {
      return TRUE;
    }
  }
  return FALSE;
}

EFI_STATUS
EFIAPI
PspFlashAccSmmCommunicateHandler (
  IN       EFI_HANDLE  DispatchHandle,
  IN       CONST VOID  *Context,
  IN OUT   VOID    *CommBuffer,
  IN OUT   UINTN   *CommBufferSize
  )
{
  PSP_SMM_COMMUNICATION_CMN *Buffer;
  PSP_SMM_COMM_RW_FLASH *RwBuffer;
  PSP_SMM_COMM_ERASEFLASH *EraseBuffer;
  PSP_SMM_COMM_RPMC_INCMC *RpmcIncMcBuffer;
  PSP_SMM_COMM_RPMC_REQMC *RpmcReqMcBuffer;
  EFI_STATUS Status;
  UINTN TempCommBufferSize;
  UINT8 *TempCommBuffer;

  //
  // If input is invalid, stop processing this SMI
  //
  if (CommBuffer == NULL || CommBufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TempCommBufferSize = *CommBufferSize;
  // too small buffer size may allow TOC/TOU attack, too big buffer size may cause stack overflow or heap overwritten
  if (TempCommBufferSize < sizeof (PSP_SMM_COMMUNICATION_CMN) || TempCommBufferSize > AMD_PSP_SMM_TMP_BUFFER_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler communication Buffer Size invalid!\n");
    return EFI_INVALID_PARAMETER;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN) CommBuffer, TempCommBufferSize)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SMM communication data buffer is in SMRAM or overlapped with SMRAM!\n");
    return EFI_INVALID_PARAMETER;
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

  Status = EFI_UNSUPPORTED;
  Buffer = (PSP_SMM_COMMUNICATION_CMN *) TempCommBuffer;
  IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler ID %x\n", Buffer->id);

  switch (Buffer->id) {
  case PSP_SMM_COMM_ID_GET_BLOCK_SIZE:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_GETBLKSIZE)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    Status = PspGetFlashBlockSize (&((PSP_SMM_COMM_GETBLKSIZE *) Buffer)->BlockSize);
    break;
  case PSP_SMM_COMM_ID_READ_FLASH:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_RW_FLASH)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    RwBuffer = (PSP_SMM_COMM_RW_FLASH *) Buffer;
    /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
    if (RwBuffer->FlashAddress >= MAX_SUPPORTED_FLASH_SIZE &&
        !SmmIsBufferOutsideSmmValid ((UINTN) RwBuffer->FlashAddress, RwBuffer->NumBytes)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SMM communication data buffer is in SMRAM or overlapped with SMRAM (2-1)!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    /// The data to write to RwBuffer->Buffer cannot be outside of TempCommBuffer
    if (RwBuffer->NumBytes > AMD_PSP_SMM_TMP_BUFFER_SIZE - sizeof (PSP_SMM_COMM_RW_FLASH) + sizeof (UINT8)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("The size (0x%X) of data to read is too large (2-2)!\n", RwBuffer->NumBytes);
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    Status = PspReadFlash (RwBuffer->FlashAddress, &RwBuffer->NumBytes, RwBuffer->Buffer);
    break;
  case PSP_SMM_COMM_ID_WRITE_FLASH:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_RW_FLASH)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    RwBuffer = (PSP_SMM_COMM_RW_FLASH *) Buffer;
    /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
    if (RwBuffer->FlashAddress >= MAX_SUPPORTED_FLASH_SIZE &&
        !SmmIsBufferOutsideSmmValid ((UINTN) RwBuffer->FlashAddress, RwBuffer->NumBytes)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SMM communication data buffer is in SMRAM or overlapped with SMRAM (3-1)!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    /// The data to read from RwBuffer->Buffer cannot be outside of TempCommBuffer
    if (RwBuffer->NumBytes > AMD_PSP_SMM_TMP_BUFFER_SIZE - sizeof (PSP_SMM_COMM_RW_FLASH) + sizeof (UINT8)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("The size (0x%X) of data to write is too large (3-2)!\n", RwBuffer->NumBytes);
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if (IsWritableRegion (RwBuffer->FlashAddress, RwBuffer->NumBytes)) {
      Status = PspWriteFlash (RwBuffer->FlashAddress, &RwBuffer->NumBytes, RwBuffer->Buffer);
    } else {
      Status = EFI_WRITE_PROTECTED;
      IDS_HDT_CONSOLE_PSP_TRACE ("Unwritable region detected: %lx-%lx\n",
        RwBuffer->FlashAddress, (RwBuffer->FlashAddress + RwBuffer->NumBytes - 1));
    }
    break;
  case PSP_SMM_COMM_ID_ERASE_FLASH:
    if(TempCommBufferSize < sizeof (PSP_SMM_COMM_ERASEFLASH)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    EraseBuffer = (PSP_SMM_COMM_ERASEFLASH *) Buffer;
    /// Check the buffer is valid per processor architecture and not overlap with SMRAM.
    if (EraseBuffer->FlashAddress >= MAX_SUPPORTED_FLASH_SIZE &&
        !SmmIsBufferOutsideSmmValid ((UINTN) EraseBuffer->FlashAddress, EraseBuffer->NumBytes)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SMM communication data buffer is in SMRAM or overlapped with SMRAM (4-1)!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if (IsWritableRegion (EraseBuffer->FlashAddress, EraseBuffer->NumBytes)) {
      Status = PspEraseFlash (EraseBuffer->FlashAddress, &EraseBuffer->NumBytes);
    } else {
      Status = EFI_WRITE_PROTECTED;
      IDS_HDT_CONSOLE_PSP_TRACE ("Unwritable region detected: %lx-%lx\n",
        EraseBuffer->FlashAddress, (EraseBuffer->FlashAddress + EraseBuffer->NumBytes - 1));
    }
    break;
  case PSP_SMM_COMM_ID_CLOSE_INTERFACE:
    if (mPspSmmCommHandle != NULL) {
      Status = gSmst->SmiHandlerUnRegister (mPspSmmCommHandle);
    } else {
      Status = EFI_INVALID_PARAMETER;
      IDS_HDT_CONSOLE_PSP_TRACE ("mPspSmmCommHandle NULL\n");
    }
    break;
  case PSP_SMM_COMM_ID_RPMC_INCMC:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_RPMC_INCMC)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    RpmcIncMcBuffer = (PSP_SMM_COMM_RPMC_INCMC *) Buffer;
    Status = PspRpmcIncMc (RpmcIncMcBuffer->CounterAddr, RpmcIncMcBuffer->CounterData, &(RpmcIncMcBuffer->Signature[0]));
    break;
  case PSP_SMM_COMM_ID_RPMC_REQMC:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_RPMC_REQMC)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspFlashAccSmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    RpmcReqMcBuffer = (PSP_SMM_COMM_RPMC_REQMC *) Buffer;
    Status = PspRpmcReqMc (RpmcReqMcBuffer->CounterAddr, &RpmcReqMcBuffer->Tag[0], &RpmcReqMcBuffer->Signature[0],
                           &(RpmcReqMcBuffer->CounterData), &RpmcReqMcBuffer->OutputSignature[0]);
    break;
  default:
    ASSERT (FALSE);
  }

  // Copy the content of buffer in SMRAM back into the buffer outside of SMRAM
  CopyMem ((VOID *) CommBuffer, (VOID *) TempCommBuffer, TempCommBufferSize);

  return Status;
}

