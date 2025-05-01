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
#include <Library/BaseMemoryLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/FchMmioSpiAccessLib.h>
#include <Library/RomArmorSpiAccessLib.h>
//[-start-220905-IB14740261-add]//
#include <Library/SpiAccessInitLib.h>
//[-end-220905-IB14740261-add]//
#include <SpiAccess.h>

/**
  Detect and Initialize SPI flash part OpCode and other parameter through FCH

  @param FlashDevice            pointer to FLASH_DEVICE structure

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to Recoginize the SPI device

**/
EFI_STATUS
EFIAPI
SpiRecognize (
  IN FLASH_DEVICE                       *FlashDevice
  )
{
  EFI_STATUS                            Status;

  Status = RomArmorSpiRecognize (FlashDevice);
  if (Status == EFI_NOT_READY) {
    Status = FchMmioSpiRecognize (FlashDevice);
  }

  return Status;
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
SpiErase(
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINTN                     FlashAddress,
  IN  UINTN                     Size
  )
{
  EFI_STATUS                    Status;

  Status = RomArmorSpiErase (FlashDevice, FlashAddress, Size);
  if (Status == EFI_NOT_READY) {
    Status = FchMmioSpiErase(FlashDevice, FlashAddress, Size);
  }

  return Status;
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
SpiProgram (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINT8                     *FlashAddress,
  IN  UINT8                     *SrcAddress,
  IN  UINTN                     *SPIBufferSize,
  IN  UINTN                     LbaWriteAddress
  )
{
  EFI_STATUS                    Status;

  Status = RomArmorSpiProgram (FlashDevice, FlashAddress, SrcAddress, SPIBufferSize, LbaWriteAddress);
  if (Status == EFI_NOT_READY) {
    Status = FchMmioSpiProgram (FlashDevice, FlashAddress, SrcAddress, SPIBufferSize, LbaWriteAddress);
  }

  return Status;
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
SpiRead (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINT8                     *DstAddress,
  IN  UINT8                     *FlashAddress,
  IN  UINTN                     BufferSize
  )
{
  EFI_STATUS                            Status;

  Status = RomArmorSpiRead (FlashDevice, DstAddress, FlashAddress, BufferSize);
  if (Status == EFI_NOT_READY) {
    Status = FchMmioSpiRead (FlashDevice, DstAddress, FlashAddress, BufferSize);
  }

  return Status;
}


/**
  Get flash number from SPI Descriptor.

  @param[out] FlashNumber       Number of SPI flash devices returned.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval EFI_UNSUPPORTED       The SPI flash is not in Descriptor mode.
**/
EFI_STATUS
GetSpiFlashNumber (
  OUT   UINT8    *FlashNumber
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Offset the BIOS address from top of 4G memory address to correct BIOS region
  described in SPI Descriptor.

  @param[in]  MemoryAddress     BIOS memory mapped address.
  @param[out] BiosRegionAddress Flash ROM start address + BIOS address in flash ROM.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval EFI_UNSUPPORTED       The SPI flash is not in Descriptor mode.
**/
EFI_STATUS
MemoryToBiosRegionAddress (
  IN UINTN       MemoryAddress,
  OUT UINTN      *BiosRegionAddress
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get flash table from platform.

  @param[in, out] DataBuffer    IN: the input buffer address.
                                OUT:the flash region table from rom file.

  @retval EFI_SUCCESS           Function successfully returned.
**/
EFI_STATUS
GetSpiPlatformFlashTable (
  IN OUT   UINT8    *DataBuffer
  )
{
//[-start-220318-IB14740227-modify]//
  FLASH_REGION          *FlashRegionPtr;
  
  FlashRegionPtr = (FLASH_REGION *)DataBuffer;
  //[-start-230914-IB20840023-add]//
  FlashRegionPtr->Type = BIOS_REGION;
  FlashRegionPtr->Offset = 0;
  FlashRegionPtr->Size = PcdGet32 (PcdFlashAreaSize);
  FlashRegionPtr->Access = ACCESS_AVAILABLE;
  FlashRegionPtr++;
  //[-end-230914-IB20840023-add]//
  FlashRegionPtr->Type = FLASH_REGION_TYPE_OF_EOS;

  return EFI_SUCCESS;
//[-end-220318-IB14740227-modify]//
}

//[-start-220905-IB14740261-add]//
/**
  SpiAccessLib Library Class Constructor

  @retval EFI_SUCCESS           Module initialized successfully
  @retval Others                Module initialization failed

**/
EFI_STATUS
EFIAPI
SpiAccessLibInit (
  VOID
  )
{
  SpiAccessInit();

  return EFI_SUCCESS;
}
//[-end-220905-IB14740261-add]//