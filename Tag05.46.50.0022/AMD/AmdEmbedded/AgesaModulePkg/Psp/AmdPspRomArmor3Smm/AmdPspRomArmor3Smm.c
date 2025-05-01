/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "AmdPspRomArmor3Smm.h"
#include <Library/IdsLib.h>
#include <Filecode.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/FchSpiAccessLib.h>
#include <Library/SmmMemLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdHeapLib.h>
#include <Library/AmdPspDxeSmmBufLib.h>
#include <Library/AmdPspBaseLibV2.h>

#define FILECODE PSP_AMDPSPROMARMOR3SMM_AMDPSPROMARMOR3SMM_FILECODE
#define ACPIMMIO32(x) (*(volatile UINT32*)(UINTN)(x))

#if AMD_PSP_SMM_TMP_BUFFER_SIZE < AMD_SMM_COMMUNICATION_BUFFER_SIZE
  #error AMD_PSP_SMM_TMP_BUFFER_SIZE need larger or equal to AMD_SMM_COMMUNICATION_BUFFER_SIZE
#endif

BOOLEAN                             *mArmorLibPspMboxSmmFlagAddr = NULL;
MBOX_ROM_ARMOR_FLASH_COMMAND_BUFFER mMboxRomArmorCommandBuffer;
EFI_HANDLE                          mPspSmmCommHandle;
extern EFI_GUID                     gPspFlashAccSmmCommReadyProtocolGuid;
EFI_SPI_NOR_FLASH_PROTOCOL          *mProtocol;
BIOS_MBOX                           *mPspToBiosMbox;
EFI_BOOT_MODE                       mBootMode;

/**
 * @brief List writable PSP entry types which will use this driver
 *
 */
UINT32 WritablePspEntryTypes[] = {
  // fTPM NVRAM
  0x4,
  // PSP NVRAM for RPMC
  0x54
};

/**
 * @brief List writable BIOS entry types which will use this driver
 *
 */
UINT32 WritableBiosEntryTypes[] = {
  // Agesa PSP Customization Block
  0x60,
  // APOB NV Copy
  0x63
};

ENTRY_REGION                          mWritableEntryRegions[MAX_WRITABLE_ENTRY_REGIONS];
UINT32                                mEntryRegionIndex = 0;

/**
  Read the 3 byte manufacture and device ID from the SPI flash.

  This routine must be called at or below TPL_NOTIFY.
  This routine reads the 3 byte manufacture and device ID from the flash part
  filling the buffer provided.

  @param[in]  This    Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data structure.
  @param[out] Buffer  Pointer to a 3 byte buffer to receive the manufacture and
                      device ID.



  @retval EFI_SUCCESS            The manufacture and device ID was read
                                 successfully.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL
  @retval EFI_DEVICE_ERROR       Invalid data received from SPI flash part.

**/
EFI_STATUS
EFIAPI
GetFlashId (
  IN  CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  OUT UINT8                             *Buffer
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read data from the SPI flash at not fast speed

  This routine must be called at or below TPL_NOTIFY.
  This routine reads data from the SPI part in the buffer provided.

  @param[in]  This           Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data
                             structure.
  @param[in]  FlashAddress   Address in the flash to start reading
  @param[in]  LengthInBytes  Read length in bytes
  @param[out] Buffer         Address of a buffer to receive the data

  @retval EFI_SUCCESS            The data was read successfully.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL, or
                                 FlashAddress >= This->FlashSize, or
                                 LengthInBytes > This->FlashSize - FlashAddress

**/
EFI_STATUS
EFIAPI
LfReadData (
  IN  CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  IN  UINT32                            FlashAddress,
  IN  UINT32                            LengthInBytes,
  OUT UINT8                             *Buffer
  )
{
  return EFI_UNSUPPORTED;
}
/**
  Read the flash status register.

  This routine must be called at or below TPL_NOTIFY.
  This routine reads the flash part status register.

  @param[in]  This           Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data
                             structure.
  @param[in]  LengthInBytes  Number of status bytes to read.
  @param[out] FlashStatus    Pointer to a buffer to receive the flash status.

  @retval EFI_SUCCESS  The status register was read successfully.

**/
EFI_STATUS
EFIAPI
ReadStatus (
  IN  CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  IN  UINT32                            LengthInBytes,
  OUT UINT8                             *FlashStatus
  )
{
  return EFI_UNSUPPORTED;
}
/**
  Write the flash status register.

  This routine must be called at or below TPL_N OTIFY.
  This routine writes the flash part status register.

  @param[in] This           Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data
                            structure.
  @param[in] LengthInBytes  Number of status bytes to write.
  @param[in] FlashStatus    Pointer to a buffer containing the new status.

  @retval EFI_SUCCESS           The status write was successful.
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate the write buffer.

**/
EFI_STATUS
EFIAPI
WriteStatus (
  IN CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  IN UINT32                            LengthInBytes,
  IN UINT8                             *FlashStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read data from the SPI flash.

  This routine must be called at or below TPL_NOTIFY.
  This routine reads data from the SPI part in the buffer provided.

  @param[in]  This           Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data
                             structure.
  @param[in]  FlashAddress   Address in the flash to start reading
  @param[in]  LengthInBytes  Read length in bytes
  @param[out] Buffer         Address of a buffer to receive the data

  @retval EFI_SUCCESS            The data was read successfully.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL, or
                                 FlashAddress >= This->FlashSize, or
                                 LengthInBytes > This->FlashSize - FlashAddress

**/
EFI_STATUS
EFIAPI
ReadData (
  IN  CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  IN  UINT32                            FlashAddress,
  IN  UINT32                            LengthInBytes,
  OUT UINT8                             *Buffer
  )
{
  EFI_STATUS Status;

  IDS_HDT_CONSOLE_PSP_TRACE ("SPI ReadData FlashAddress=0x%x, LengthInBytes=0x%x, Buffer = 0x%x\n", FlashAddress, LengthInBytes, Buffer);

  Status = EFI_DEVICE_ERROR;
  if (Buffer == NULL ||
      FlashAddress >= This->FlashSize ||
      LengthInBytes > This->FlashSize - FlashAddress) {
    return EFI_INVALID_PARAMETER;
  }

  if (FchSpiRomReadEx (FlashAddress, Buffer, LengthInBytes)) {
    Status = EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("SPI ReadData Buffer[0] = 0x%x, Status = %r\n", Buffer[0], Status);

  return Status;
}

/**
  Write data to the SPI flash.

  This routine must be called at or below TPL_NOTIFY.
  This routine breaks up the write operation as necessary to write the data to
  the SPI part.

  @param[in] This           Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data
                            structure.
  @param[in] FlashAddress   Address in the flash to start writing
  @param[in] LengthInBytes  Write length in bytes
  @param[in] Buffer         Address of a buffer containing the data

  @retval EFI_SUCCESS            The data was written successfully.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL, or
                                 FlashAddress >= This->FlashSize, or
                                 LengthInBytes > This->FlashSize - FlashAddress
  @retval EFI_OUT_OF_RESOURCES   Insufficient memory to copy buffer.

**/
EFI_STATUS
EFIAPI
WriteData (
  IN CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  IN UINT32                            FlashAddress,
  IN UINT32                            LengthInBytes,
  IN UINT8                             *Buffer
  )
{
  EFI_STATUS  Status;
  UINT32 ByteCounter;
  UINT32 CurrentAddress;
  UINT32 Length;
  UINT8 *CurrentBuffer;

  Status = EFI_DEVICE_ERROR;
  if (Buffer == NULL ||
      LengthInBytes == 0 ||
      FlashAddress >= This->FlashSize ||
      LengthInBytes > This->FlashSize - FlashAddress) {
    return EFI_INVALID_PARAMETER;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("SPI WriteData FlashAddress=0x%x, LengthInBytes=0x%x, Buffer = 0x%x, Buffer[0] = 0x%x\n", FlashAddress, LengthInBytes, Buffer, Buffer[0]);
  mMboxRomArmorCommandBuffer.Transaction = WRITE_ACCESS;
  for (ByteCounter = 0; ByteCounter < LengthInBytes;) {
    CurrentAddress = FlashAddress + ByteCounter;
    CurrentBuffer = Buffer + ByteCounter;
    Length = LengthInBytes - ByteCounter;
    if (Length > SIZE_4KB) {
      Length = SIZE_4KB;
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("  CurrentAddress=0x%x, Length=0x%x\n", CurrentAddress, Length);
    CopyMem (mMboxRomArmorCommandBuffer.Buffer, CurrentBuffer, Length);
    mMboxRomArmorCommandBuffer.Offset = CurrentAddress;
    mMboxRomArmorCommandBuffer.Size = Length;
    Status = PspMboxBiosCmdArmorSpiTransction (&mMboxRomArmorCommandBuffer, mArmorLibPspMboxSmmFlagAddr);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SPI Write Error:%r\n", Status);
      ASSERT (FALSE);
      break;
    }
    ByteCounter += Length;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SPI Write Exit\n");

  return Status;
}

/**
  Efficiently erases one or more 4KiB regions in the SPI flash.

  This routine must be called at or below TPL_NOTIFY.
  This routine uses a combination of 4 KiB and larger blocks to erase the
  specified area.

  @param[in] This          Pointer to an EFI_SPI_NOR_FLASH_PROTOCOL data
                           structure.
  @param[in] FlashAddress  Address within a 4 KiB block to start erasing
  @param[in] BlockCount    Number of 4 KiB blocks to erase

  @retval EFI_SUCCESS            The erase was completed successfully.
  @retval EFI_INVALID_PARAMETER  FlashAddress >= This->FlashSize, or
                                 BlockCount * 4 KiB
                                   > This->FlashSize - FlashAddress

**/
EFI_STATUS
EFIAPI
Erase (
  IN CONST EFI_SPI_NOR_FLASH_PROTOCOL  *This,
  IN UINT32                            FlashAddress,
  IN UINT32                            BlockCount
  )
{
  EFI_STATUS  Status;
  UINT32 EraseLength;
  UINT32 CurrentAddress;
  UINT32 BlockCounter;

  Status = EFI_DEVICE_ERROR;
  EraseLength = BlockCount * SIZE_4KB;
  // Align start Address to 4KB
  CurrentAddress = FlashAddress & ~(SIZE_4KB - 1);
  if (BlockCount == 0 ||
      CurrentAddress >= This->FlashSize ||
      EraseLength > This->FlashSize - CurrentAddress) {
    return EFI_INVALID_PARAMETER;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SPI Erase FlashAddress=%x, BlockCount=%x\n", CurrentAddress, BlockCount);

  mMboxRomArmorCommandBuffer.Transaction = ERASE;
  for (BlockCounter= 0; BlockCounter < BlockCount;) {
    mMboxRomArmorCommandBuffer.Offset = CurrentAddress + (BlockCounter * SIZE_4KB);

    if (PcdGetBool(PcdAmdPsp64KFlashEraseSupportForRomArmor) == TRUE) {
      if ((mMboxRomArmorCommandBuffer.Offset % SIZE_64KB == 0) && ((BlockCount - BlockCounter) >= (SIZE_64KB / SIZE_4KB))) {
        mMboxRomArmorCommandBuffer.Size = SIZE_64KB;
        BlockCounter += (SIZE_64KB / SIZE_4KB);
      } else {
        mMboxRomArmorCommandBuffer.Size = SIZE_4KB;
        BlockCounter++;
      }
    } else {
      mMboxRomArmorCommandBuffer.Size = SIZE_4KB;
      BlockCounter++;
    }

    Status = PspMboxBiosCmdArmorSpiTransction (&mMboxRomArmorCommandBuffer, mArmorLibPspMboxSmmFlagAddr);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SPI Erase Error:%r\n", Status);
      ASSERT (FALSE);
      break;
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SPI Erase Exit\n");

  return Status;
}

VOID
FillPspWritableRegions (
  IN PSP_DIRECTORY               *PspDir,
  IN UINT64                      DirectoryHdrAddr
) {
  UINTN                       i;
  UINT32                      j;
  UINT64                      EntryAddress;
  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    for (j = 0; j < sizeof(WritablePspEntryTypes) / sizeof(UINT32); j++) {
      if (PspDir->PspEntry[i].Type.Field.Type == WritablePspEntryTypes[j]) {
        if (mEntryRegionIndex < MAX_WRITABLE_ENTRY_REGIONS) {
          EntryAddress = TranslateEntryLocation (PspDir->PspEntry[i].Location, DirectoryHdrAddr,
                                                  (UINT32)(DirectoryHdrAddr & 0xFFFFFFFF));
          mWritableEntryRegions[mEntryRegionIndex].Address = EntryAddress;
          mWritableEntryRegions[mEntryRegionIndex].Size = PspDir->PspEntry[i].Size;
          mEntryRegionIndex++;
        } else {
          IDS_HDT_CONSOLE_PSP_TRACE ("\nindex overflow for writable entry regions(Current=%d, Max=%d)",
                                        mEntryRegionIndex, MAX_WRITABLE_ENTRY_REGIONS);
        }
      }
    }
  }
}

VOID
FillBiosWritableRegions (
  IN BIOS_DIRECTORY              *BiosDir,
  IN UINT64                      DirectoryHdrAddr
) {
  UINTN                       i;
  UINT32                      j;
  UINT64                      EntryAddress;
  for (i = 0; i < BiosDir->Header.TotalEntries; i++) {
    for (j = 0; j < sizeof(WritableBiosEntryTypes) / sizeof(UINT32); j++) {
      if (BiosDir->BiosEntry[i].TypeAttrib.Type == WritableBiosEntryTypes[j]) {
        if (mEntryRegionIndex < MAX_WRITABLE_ENTRY_REGIONS) {
          EntryAddress = TranslateEntryLocation (BiosDir->BiosEntry[i].Location, DirectoryHdrAddr,
                                                  (UINT32)(DirectoryHdrAddr & 0xFFFFFFFF));
          mWritableEntryRegions[mEntryRegionIndex].Address = EntryAddress;
          mWritableEntryRegions[mEntryRegionIndex].Size = BiosDir->BiosEntry[i].Size;
          mEntryRegionIndex++;
          } else {
            IDS_HDT_CONSOLE_PSP_TRACE ("\nindex overflow for writable entry regions(Current=%d, Max=%d)",
                                        mEntryRegionIndex, MAX_WRITABLE_ENTRY_REGIONS);
        }
      }
    }
  }
}

VOID
InitWritableEntryRegions (
  )
{
  PSP_DIRECTORY               *PspDir;
  BIOS_DIRECTORY              *BiosDir;
  UINTN                       i;
  UINT64                      PspLv2BaseAddress;
  UINT64                      BiosLv2BaseAddress;

  PspDir = NULL;
  BiosDir = NULL;
  PspLv2BaseAddress = 0;
  BiosLv2BaseAddress = 0;
  mEntryRegionIndex = 0;

  if (GetPspBiosLv2BaseAddr (&PspLv2BaseAddress, &BiosLv2BaseAddress) == TRUE) {
    if (GetPspLv2DirBaseV2 (PspLv2BaseAddress, &PspDir) == TRUE) {
      FillPspWritableRegions (PspDir, PspLv2BaseAddress);
    }

    if (GetBiosLv2DirBaseV2 (BiosLv2BaseAddress, &BiosDir) == TRUE) {
      FillBiosWritableRegions (BiosDir, BiosLv2BaseAddress);
    }
  } else { // cannot find PSP & BIOS L2, then find them in L1
    if (GetPspDirBaseV2 (&PspDir) == TRUE) {
      FillPspWritableRegions (PspDir, 0);
    }

    if (GetBiosDirBaseV2 (&BiosDir) == TRUE) {
      FillBiosWritableRegions (BiosDir, 0);
    }
  }

  // print writable regions
  IDS_HDT_CONSOLE_PSP_TRACE ("\nWritable regions:");
  for (i = 0; i < mEntryRegionIndex; i++) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\n%lx-%lx", mWritableEntryRegions[i].Address,
    (mWritableEntryRegions[i].Address + mWritableEntryRegions[i].Size - 1));
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\n");
}

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
RomArmor3SmmCommunicateHandler (
  IN       EFI_HANDLE  DispatchHandle,
  IN       CONST VOID  *Context,
  IN OUT   VOID    *CommBuffer,
  IN OUT   UINTN   *CommBufferSize
  )
{
  PSP_SMM_COMMUNICATION_CMN *Buffer;
  PSP_SMM_COMM_RW_FLASH *RwBuffer;
  PSP_SMM_COMM_ERASEFLASH *EraseBuffer;
  EFI_STATUS Status;
  UINTN TempCommBufferSize;
  UINT8 *TempCommBuffer;

  Status = EFI_UNSUPPORTED;

  //
  // If input is invalid, stop processing this SMI
  //
  if (CommBuffer == NULL || CommBufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TempCommBufferSize = *CommBufferSize;
  // too small buffer size may allow TOC/TOU attack, too big buffer size may cause stack overflow or heap overwritten
  if (TempCommBufferSize < sizeof (PSP_SMM_COMMUNICATION_CMN) || TempCommBufferSize > AMD_PSP_SMM_TMP_BUFFER_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("RomArmor3SmmCommunicateHandler Command Buffer Size invalid!\n");
    return EFI_INVALID_PARAMETER;
  }
  if (!SmmIsBufferOutsideSmmValid((UINTN) CommBuffer, TempCommBufferSize)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SMM communication data buffer is in SMRAM or overlapped with SMRAM!\n");
    Status = EFI_INVALID_PARAMETER;
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

  Buffer = (PSP_SMM_COMMUNICATION_CMN *) TempCommBuffer;
  IDS_HDT_CONSOLE_PSP_TRACE ("RomArmor3SmmCommunicateHandler ID %x\n", Buffer->id);

  switch (Buffer->id) {
  case PSP_SMM_COMM_ID_GET_BLOCK_SIZE:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_GETBLKSIZE)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("RomArmor3SmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    ((PSP_SMM_COMM_GETBLKSIZE *) Buffer)->BlockSize = SIZE_4KB;
    Status = EFI_SUCCESS;
    break;
  case PSP_SMM_COMM_ID_READ_FLASH:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_RW_FLASH)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("RomArmor3SmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    RwBuffer = (PSP_SMM_COMM_RW_FLASH *) Buffer;
    /// The data to write to RwBuffer->Buffer cannot be outside of TempCommBuffer
    if (RwBuffer->NumBytes > AMD_PSP_SMM_TMP_BUFFER_SIZE - sizeof (PSP_SMM_COMM_RW_FLASH) + sizeof (UINT8)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("The size (0x%X) of data to read is too large (2-2)!\n", RwBuffer->NumBytes);
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    Status = ReadData (mProtocol, (UINT32) RwBuffer->FlashAddress, (UINT32) RwBuffer->NumBytes, RwBuffer->Buffer);
    break;
  case PSP_SMM_COMM_ID_WRITE_FLASH:
    if (TempCommBufferSize < sizeof (PSP_SMM_COMM_RW_FLASH)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("RomArmor3SmmCommunicateHandler Command Buffer Size invalid!\n");
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    RwBuffer = (PSP_SMM_COMM_RW_FLASH *) Buffer;
    /// The data to read from RwBuffer->Buffer cannot be outside of TempCommBuffer
    if (RwBuffer->NumBytes > AMD_PSP_SMM_TMP_BUFFER_SIZE - sizeof (PSP_SMM_COMM_RW_FLASH) + sizeof (UINT8)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("The size (0x%X) of data to write is too large (3-2)!\n", RwBuffer->NumBytes);
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    if (IsWritableRegion (RwBuffer->FlashAddress, RwBuffer->NumBytes)) {
      Status = WriteData (mProtocol, (UINT32) RwBuffer->FlashAddress, (UINT32) RwBuffer->NumBytes, RwBuffer->Buffer);
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
    if (IsWritableRegion (EraseBuffer->FlashAddress, EraseBuffer->NumBytes)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SPI Erase FlashAddress=%x, NumBytes=%x\n", EraseBuffer->FlashAddress, EraseBuffer->NumBytes);
      Status = Erase (mProtocol, (UINT32) EraseBuffer->FlashAddress, (UINT32) EraseBuffer->NumBytes / SIZE_4KB);
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

  default:
    ASSERT (FALSE);
  }

  // Copy the content of buffer in SMRAM back into the buffer outside of SMRAM
  CopyMem ((VOID *) CommBuffer, (VOID *) TempCommBuffer, TempCommBufferSize);

  return Status;
}

EFI_STATUS
EFIAPI
P2CmboxFakeSmiCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT   *MiscRegisterContext
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("P2CmboxFakeSmiCallback enter\n");

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) &= ~BIT25;  // Deassert fakeSts0

  if(mPspToBiosMbox->MboxCmd == MboxPspCmdSpiGetBlockSize) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP -> BIOS: Get Block size info CMD is received\n");
    mPspToBiosMbox->MboxCmd  = 0;     //Clear the command register
    mPspToBiosMbox->MboxSts.CommandReady = TRUE;
    mPspToBiosMbox->Buffer.Dflt.Header.Status = 0;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("P2CmboxFakeSmiCallback Exit\n");
  return EFI_SUCCESS;
}

EFI_STATUS
EnablePspFakeStsSmi (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  FCH_SMM_MISC_DISPATCH_PROTOCOL    *AmdFchSmmMiscDispatch;
  EFI_HANDLE                        MiscHandle;
  FCH_SMM_MISC_REGISTER_CONTEXT     MiscRegisterContext;
  EFI_STATUS                        Status;
  UINT32                            OrMask;
  UINT32                            AndMask;

  // enable PSP SMM via Fake Sts0
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = BIT1;  //Clear FakeSts0
  OrMask = BIT1;
  AndMask = 0xFFFFFFFFul;
  S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84), &OrMask, &AndMask);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) &= ~BIT25;  // Deassert fakeSts0
  OrMask = 0;
  AndMask = (UINT32) ~BIT25;
  S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98), &OrMask, &AndMask);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8) &= ~(BIT3 + BIT2);  // Enable SMI [3:2] = 1
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8) |= BIT2;
  OrMask = BIT2;
  AndMask = (UINT32) ~(BIT3 + BIT2);
  S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8), &OrMask, &AndMask);

  IDS_HDT_CONSOLE_PSP_TRACE ("EnablePspFakeStsSmi entered\n");

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG84;
  MiscRegisterContext.SmiStatusBit = BIT1;
  MiscRegisterContext.Order        = 0x80;

  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmMiscDispatchProtocolGuid,
                    NULL,
                    (VOID **) &AmdFchSmmMiscDispatch
                    );
  IDS_HDT_CONSOLE_PSP_TRACE ("SmmLocateProtocol Status = %r\n", Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = AmdFchSmmMiscDispatch->Register (
                                      AmdFchSmmMiscDispatch,
                                      &P2CmboxFakeSmiCallback,
                                      &MiscRegisterContext,
                                      &MiscHandle
                                      );
  IDS_HDT_CONSOLE_PSP_TRACE ("Register Status = %r\n", Status);
  return Status;
}

/**
 * @brief Set the Rom Armor 3 Flag In Hsti State Heap so that we can get the update to date HstiState in SMM
 *
 * @return EFI_STATUS EFI_SUCCESS: execution success, EFI_DEVICE_ERROR: execution failed
 */
EFI_STATUS
EFIAPI
SetRomArmor3FlagInHstiStateHeap ()
{
  UINT32                      *HstiStateBuffer;
  if (GetHeapBufferByHandle (AMD_PSP_HSTI_STATE_BUFFER_HANDLE, sizeof (UINT32), (UINT8 **)&HstiStateBuffer) != TRUE) {
     return EFI_DEVICE_ERROR;
  }

  *HstiStateBuffer |= PSP_ROM_ARMOR_ENFORCED;
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
AmdRomArmor3Init (
  IN OUT EFI_SPI_NOR_FLASH_PROTOCOL *Protocol
  )
{
  MBOX_ROM_ARMOR_ENFORCE              RomArmor3EnforceBuffer;
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;
  SMM_REQ_BUFFER                      SmmInfoReq;
  EFI_STATUS                          Status;
  EFI_BOOT_MODE                       BootMode;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a - ENTRY\n", __FUNCTION__);

  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, (VOID **) &PspMboxSmmBufferAddressProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mArmorLibPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
  IDS_HDT_CONSOLE_PSP_TRACE ("\tmPspMboxSmmFlagAddr 0x%x\n", mArmorLibPspMboxSmmFlagAddr);

  //
  // Enable RomArmor3
  //
  ZeroMem (&RomArmor3EnforceBuffer, sizeof (MBOX_ROM_ARMOR_ENFORCE));
  BootMode = GetBootModeHob();
  if ((BootMode == BOOT_ON_FLASH_UPDATE) || (BootMode == BOOT_IN_RECOVERY_MODE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Capsule Update \n");
    RomArmor3EnforceBuffer.CapsuleUpdate = 1;
  }
  Status = PspMboxBiosCmdArmorEnterSmmOnlyMode2 (&RomArmor3EnforceBuffer, mArmorLibPspMboxSmmFlagAddr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  SetRomArmor3FlagInHstiStateHeap ();
  Protocol->FlashSize = RomArmor3EnforceBuffer.FlashSize;

  //Prepare SMM_REQ_BUFFER
  ZeroMem (&SmmInfoReq, sizeof (SmmInfoReq));
  mPspToBiosMbox = (BIOS_MBOX *) AllocatePages (1);
  ZeroMem (mPspToBiosMbox, SIZE_4KB);

  mPspToBiosMbox->MboxCmd      = 0;
  mPspToBiosMbox->MboxSts.CommandReady = TRUE;
  SmmInfoReq.PSPSmmDataRegion =   (UINT64)mPspToBiosMbox;
  SmmInfoReq.PspSmmDataLength =   SIZE_4KB;
  //Get TsegBase, TsegSize
  // mTsegMask = (~(mTsegMask & MSR_SMMMASK_TSEGMASK_BITS) + 1) & MSR_SMMMASK_TSEGMASK_BITS;
  SmmInfoReq.SMMBase = AsmReadMsr64 (MSR_SMMADDR);
  SmmInfoReq.SMMBase &= MSR_SMMADDR_TSEGBASE_BITS;
  SmmInfoReq.SMMMask = AsmReadMsr64 (MSR_SMMMASK);
  SmmInfoReq.PspMboxSmmBufferAddress = (UINT64) (UINTN) PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
  SmmInfoReq.PspMboxSmmFlagAddress = (UINT64) (UINTN) PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
  //Fill SmmRegisterInfo

  SmmInfoReq.SmmTrigInfo.Address = ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98;
  SmmInfoReq.SmmTrigInfo.AddressType = SMM_TRIGGER_MEM;
  SmmInfoReq.SmmTrigInfo.ValueWidth = SMM_TRIGGER_DWORD;
  SmmInfoReq.SmmTrigInfo.ValueAndMask = (UINT32) ~BIT25;
  SmmInfoReq.SmmTrigInfo.ValueOrMask = BIT25;

  SmmInfoReq.SmmRegInfo.SmiEnb.Address = ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98;
  SmmInfoReq.SmmRegInfo.SmiEnb.AddressType = SMM_TRIGGER_MEM;
  SmmInfoReq.SmmRegInfo.SmiEnb.ValueWidth = SMM_TRIGGER_DWORD;
  SmmInfoReq.SmmRegInfo.SmiEnb.RegBitMask = BIT31;
  SmmInfoReq.SmmRegInfo.SmiEnb.ExpectValue = 0;

  SmmInfoReq.SmmRegInfo.Eos.Address = ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98;
  SmmInfoReq.SmmRegInfo.Eos.AddressType = SMM_TRIGGER_MEM;
  SmmInfoReq.SmmRegInfo.Eos.ValueWidth = SMM_TRIGGER_DWORD;
  SmmInfoReq.SmmRegInfo.Eos.RegBitMask = BIT28;
  SmmInfoReq.SmmRegInfo.Eos.ExpectValue = BIT28;

  SmmInfoReq.SmmRegInfo.FakeSmiEn.Address = ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8;
  SmmInfoReq.SmmRegInfo.FakeSmiEn.AddressType = SMM_TRIGGER_MEM;
  SmmInfoReq.SmmRegInfo.FakeSmiEn.ValueWidth = SMM_TRIGGER_DWORD;
  SmmInfoReq.SmmRegInfo.FakeSmiEn.RegBitMask = BIT3 + BIT2;
  SmmInfoReq.SmmRegInfo.FakeSmiEn.ExpectValue = BIT2;

  PspMboxBiosCmdSmmInfo (&SmmInfoReq);

  IDS_HDT_CONSOLE_PSP_TRACE ("%a - EXIT\n", __FUNCTION__);
  return Status;
}

EFI_STATUS
EFIAPI
AmdRomArmor3S3Init (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  MBOX_ROM_ARMOR_ENFORCE RomArmor3EnforceBuffer;
  ZeroMem (&RomArmor3EnforceBuffer, sizeof (MBOX_ROM_ARMOR_ENFORCE));
  return PspMboxBiosCmdArmorEnterSmmOnlyMode2 (&RomArmor3EnforceBuffer, mArmorLibPspMboxSmmFlagAddr);
}

EFI_STATUS
AmdRomArmor3S3InitCallback (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  EFI_STATUS        Status;
  FCH_SMM_SW_DISPATCH2_PROTOCOL      *SwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT        SwContext;
  EFI_HANDLE                         SwHandle;

  Status = gSmst->SmmLocateProtocol (&gFchSmmSwDispatch2ProtocolGuid, NULL, (VOID **) &SwDispatch);

  SwContext.AmdSwValue  = PcdGet8 (PcdFchOemBeforePciRestoreSwSmi);
  SwContext.Order       = 0x80;
  Status = SwDispatch->Register (
                          SwDispatch,
                          AmdRomArmor3S3Init,
                          &SwContext,
                          &SwHandle
                          );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("  AmdRomArmorS3Init is not registered\n");
    return Status;
  }
  return EFI_SUCCESS;
}

/**
  Entry point of RomArmor3 SMM driver.

  @param ImageHandle  Image handle of this driver.
  @param SystemTable  Pointer to standard EFI system table.

  @retval EFI_SUCCESS       Succeed.
  @retval EFI_DEVICE_ERROR  Fail to install EFI_SPI_SMM_NOR_FLASH_PROTOCOL.
**/
EFI_STATUS
EFIAPI
AmdPspRomArmor3SmmEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                         Status;
  EFI_HANDLE                         Handle;
  FCH_SMM_SW_DISPATCH2_PROTOCOL      *SwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT        SwContext;
  EFI_HANDLE                         SwHandle;
  EFI_HANDLE                         PspFlashAccSmmCommReadyHandle;
  VOID                               *Registration;
  FCH_SMM_MISC_DISPATCH_PROTOCOL     *AmdFchSmmMiscDispatch;

  if (PcdGet8 (PcdAmdPspRomArmorSelection) != 3) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("%a - ENTRY\n", __FUNCTION__);
  mBootMode = GetBootModeHob();
  InitWritableEntryRegions ();
  if (PcdGetBool(PcdAmdPspRomArmorSpiConfigRegLock) == FALSE) {
    PspMboxSetCfg();
  }

  mProtocol = AllocateZeroPool (sizeof (EFI_SPI_NOR_FLASH_PROTOCOL));
  ASSERT (mProtocol != NULL);
  if (mProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AmdRomArmor3Init (mProtocol);
  if (EFI_ERROR (Status)) {
    FreePool ((void*) mProtocol);
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Flash Size = 0x%x\n", mProtocol->FlashSize);

  ZeroMem (&mMboxRomArmorCommandBuffer, sizeof (mMboxRomArmorCommandBuffer));
  mMboxRomArmorCommandBuffer.Header.TotalSize = sizeof (mMboxRomArmorCommandBuffer);
  mMboxRomArmorCommandBuffer.Buffer = AllocateRuntimePages (1);
  ASSERT (mMboxRomArmorCommandBuffer.Buffer != NULL);
  if (mMboxRomArmorCommandBuffer.Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSwDispatch2ProtocolGuid,
                  NULL,
                  (VOID **) &SwDispatch
                  );
  if (!EFI_ERROR (Status)) {
    // Install S3 resume handler to enable RomArmor3 in S3 resume
    SwContext.AmdSwValue  = PcdGet8 (PcdFchOemBeforePciRestoreSwSmi);
    SwContext.Order       = 0x80;
    Status = SwDispatch->Register (
                            SwDispatch,
                            AmdRomArmor3S3Init,
                            &SwContext,
                            &SwHandle
                            );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("  AmdRomArmorS3Init is not registered\n");
      return Status;
    }
  } else {
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gFchSmmSwDispatch2ProtocolGuid,
                      AmdRomArmor3S3InitCallback,
                      &Registration
                      );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\tFailed to register callback function AmdRomArmor3S3InitCallback: %r\n", Status);
      ASSERT (Status == EFI_SUCCESS);
    }
  }

  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmMiscDispatchProtocolGuid,
                    NULL,
                    (VOID **) &AmdFchSmmMiscDispatch
                    );
  if (!EFI_ERROR (Status)) {
    Status = EnablePspFakeStsSmi (NULL, NULL, NULL);
    if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\tEnablePspFakeStsSmi returned error: %r\n", Status);
    }
  } else {
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gFchSmmMiscDispatchProtocolGuid,
                      EnablePspFakeStsSmi,
                      &Registration
                      );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\tFailed to register callback function gFchSmmMiscDispatchProtocolGuid: %r\n", Status);
      ASSERT (Status == EFI_SUCCESS);
    }
  }

  mProtocol->SpiPeripheral = NULL;
  mProtocol->GetFlashid = GetFlashId;
  mProtocol->ReadData = ReadData;
  mProtocol->LfReadData = LfReadData;
  mProtocol->ReadStatus = ReadStatus;
  mProtocol->WriteStatus = WriteStatus;
  mProtocol->WriteData = WriteData;
  mProtocol->Erase = Erase;
  mProtocol->EraseBlockBytes = SIZE_4KB;
  Handle = NULL;

  Status = gSmst->SmmInstallProtocolInterface(
                    &Handle,
                    &gEfiSpiSmmNorFlashProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    mProtocol
                    );

  mPspSmmCommHandle = NULL;
  Status = gSmst->SmiHandlerRegister (RomArmor3SmmCommunicateHandler, &gPspSmmCommHandleGuid, &mPspSmmCommHandle);
  if (Status == EFI_SUCCESS) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Install PspFlashAccSmmCommReady Protocol\n");
    PspFlashAccSmmCommReadyHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                                  &PspFlashAccSmmCommReadyHandle,
                                  &gPspFlashAccSmmCommReadyProtocolGuid,
                                  EFI_NATIVE_INTERFACE,
                                  NULL
                                  );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Install fail\n");
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Register fail\n");
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("%a: EXIT - Status=%r\n", __FUNCTION__, Status);
  return Status;
}
