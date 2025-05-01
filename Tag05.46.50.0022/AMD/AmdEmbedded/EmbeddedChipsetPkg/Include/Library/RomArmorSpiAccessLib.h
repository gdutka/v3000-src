/** @file


;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _ROM_ARMOR_SPI_ACCESS_LIB_H_
#define _ROM_ARMOR_SPI_ACCESS_ROM_H_

#include <Uefi.h>
#include <Library/FdSupportLib.h>
#include <Library/SpiAccessLib.h>

/**
  Detect and Initialize SPI flash part OpCode and other parameter through PCH

  @param FlashDevice            pointer to FLASH_DEVICE structure

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to Recoginize the SPI device

**/
EFI_STATUS
EFIAPI
RomArmorSpiRecognize (
  IN FLASH_DEVICE               *FlashDevice
  );

/**
  Write the SPI flash device with given address and size through PCH

  @param FlashDevice            pointer to FLASH_DEVICE structure
  @param FlashAddress           Destination Offset
  @param SrcAddress             Source Offset
  @param SPIBufferSize          The size for programming
  @param LbaWriteAddress        Write Address

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to erase the target address

**/
EFI_STATUS
EFIAPI
RomArmorSpiProgram (
  IN FLASH_DEVICE               *FlashDevice,
  IN UINT8                      *Dest,
  IN UINT8                      *Src,
  IN UINTN                      *NumBytes,
  IN UINTN                      LbaWriteAddress
  );

/**
  Read the SPI flash device with given address and size through PCH

  @param FlashDevice            pointer to FLASH_DEVICE structure
  @param DstAddress             Destination buffer address
  @param FlashAddress           The flash device address to be read
  @param BufferSize             The size to be read

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to erase the target address

**/
EFI_STATUS
EFIAPI
RomArmorSpiRead (
  IN FLASH_DEVICE               *FlashDevice,
  IN UINT8                      *Dest,
  IN UINT8                      *Src,
  IN UINTN                      Count
  );

/**
  Erase the SPI flash device from LbaWriteAddress through PCH

  @param FlashDevice            pointer to FLASH_DEVICE structure
  @param FlashAddress           Target address to be erased
  @param Size                   The size in bytes to be erased

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to erase the target address

**/
EFI_STATUS
EFIAPI
RomArmorSpiErase (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINTN                     LbaWriteAddress,
  IN  UINTN                     EraseBlockSize
  );

#endif
