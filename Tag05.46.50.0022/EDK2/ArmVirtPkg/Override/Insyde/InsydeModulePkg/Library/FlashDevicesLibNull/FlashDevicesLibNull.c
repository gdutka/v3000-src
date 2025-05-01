/** @file
  Flash Device Initialization and Recognition.

;******************************************************************************
;* Copyright (c) 2013 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/FdSupportLib.h>
#include "FlashDevicesLib.h"

/**
  Convert H2O_FLASH_DEVICE structure to FLASH_DEVICE structure

  @param H2OFlashDevice         pointer to H2O_FLASH_DEVICE structure
  @param FlashDevice            pointer to FLASH_DEVICE structure

  @retval EFI_SUCCESS           Flash device structure is successfully converted
  @retval EFI_INVALID_PARAMETER Parameters given are invalid (NULL)

**/
EFI_STATUS
ConvertFlashDevice (
  H2O_FLASH_DEVICE      *H2OFlashDevice,
  FLASH_DEVICE          *FlashDevice
  )
{
  return EFI_SUCCESS;
}


/**
  Detect and Initialize SPI flash part OpCode and other parameter through PCH

  @param FlashDevice            pointer to FLASH_DEVICE structure

  @retval EFI_SUCCESS           The SPI device was successfully recognized
  @retval EFI_UNSUPPORTED       The flash device is not supported by this function
  @retval EFI_DEVICE_ERROR      Failed to Recognize the SPI device

**/
EFI_STATUS
EFIAPI
RecognizeFlashDevice (
  IN H2O_FLASH_DEVICE            *H2OFlashDevice
  )
{
  return FlashRecognize ();
}


/**
  Recognize Flash Device through PCDex Token number

  @param PcdExTokenNumber       pointer to PCDex Token number

  @retval H2O_FLASH_DEVICE*     pointer to H2O_FLASH_DEVICE, NULL if not recognize any flash device

**/
H2O_FLASH_DEVICE *
RecognizeFlashDeviceByPcdEx (
   UINTN                *PcdExTokenNumber
  )
{
  return NULL;
}


H2O_FLASH_DEVICE *
DetectH2OFlashDevice (
  VOID
  )
{
  return NULL;
}

/**
  FlashDevicesLib Library Class Constructor

  @retval EFI_SUCCESS:   Module initialized successfully
  @retval Others     :   Module initialization failed

**/
EFI_STATUS
EFIAPI
FlashDevicesLibInit (
  VOID
  )
{
  DetectH2OFlashDevice();
  return EFI_SUCCESS;
}
