/** @file
  SpiAccessLib implementation for SPI Flash Type devices

;******************************************************************************
;* Copyright 2020 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/FdSupportLib.h>

/**
  Detect and Initialize SPI flash part OpCode and other parameter through FCH

  @param FlashDevice            pointer to FLASH_DEVICE structure

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to Recoginize the SPI device

**/
EFI_STATUS
EFIAPI
RomArmorSpiRecognize (
  IN FLASH_DEVICE                       *FlashDevice
  )
{
//  DEBUG ( (EFI_D_INFO, "%a NULL Start\n", __FUNCTION__));
  return EFI_NOT_READY;
}

/**
  Erase the SPI flash device from LbaWriteAddress through FCH

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
  IN  UINTN                     FlashAddress,
  IN  UINTN                     Size
  )
{
//  DEBUG ( (EFI_D_INFO, "%a NULL Start\n", __FUNCTION__));
  return EFI_NOT_READY;
}

/**
  Write the SPI flash device with given address and size through FCH

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
RomArmorSpiProgram (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINT8                     *FlashAddress,
  IN  UINT8                     *SrcAddress,
  IN  UINTN                     *SPIBufferSize,
  IN  UINTN                     LbaWriteAddress
  )
{
//  DEBUG ( (EFI_D_INFO, "%a NULL Start\n", __FUNCTION__));
  return EFI_NOT_READY;
}

/**
  Read the SPI flash device with given address and size through FCH

  @param FlashDevice            pointer to FLASH_DEVICE structure
  @param DstAddress             Destination buffer address
  @param FlashAddress           The flash device address to be read
  @param BufferSize             The size to be read

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to erase the target address

**/
EFI_STATUS
RomArmorSpiRead (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINT8                     *DstAddress,
  IN  UINT8                     *FlashAddress,
  IN  UINTN                     BufferSize
  )
{
//  DEBUG ( (EFI_D_INFO, "%a NULL Start\n", __FUNCTION__));
  return EFI_NOT_READY;
}
