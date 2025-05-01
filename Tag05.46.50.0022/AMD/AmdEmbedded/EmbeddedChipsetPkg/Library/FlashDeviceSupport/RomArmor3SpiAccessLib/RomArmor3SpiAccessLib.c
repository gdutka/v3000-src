/** @file
  RomArmorSpiAccessLib implementation for SPI Flash Type devices

;******************************************************************************
;* Copyright 2022 Insyde Software Corp. All Rights Reserved.
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
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FdSupportLib.h>
#include <Library/SpiAccessLib.h>
#include <Protocol/SpiSmmNorFlash.h>
#include <Protocol/SmmBase2.h>
#include <RomArmor3SpiAccess.h>

extern EFI_GUID                  gPspFlashAccSmmCommReadyProtocolGuid;
EFI_SPI_SMM_NOR_FLASH_PROTOCOL   *mSpiSmmNorFlashProtocol = NULL;
EFI_SMM_SYSTEM_TABLE2            *mSmst2 = NULL;

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
  EFI_STATUS                    Status;
  VOID                          *PspFlashAccSmmCommReady;

//  DEBUG ( (EFI_D_INFO, "%a Start\n", __FUNCTION__));

  if (FlashDevice == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (mSmst2 != NULL) {
//      DEBUG ( (EFI_D_INFO, "Try to locate Rom Armor Protocol at %a\n",__FUNCTION__));
    if (mSpiSmmNorFlashProtocol == NULL) {
      Status = mSmst2->SmmLocateProtocol (
                         &gEfiSpiSmmNorFlashProtocolGuid,
                         NULL,
                         (VOID**)&mSpiSmmNorFlashProtocol
                         );
    } else {
      //
      // Workaround: BIOS can't access SPI controller to read SFDP header when RomArmor v3 is enabled
      //
      Status = EFI_SUCCESS;
    }
  } else {
    Status = gBS->LocateProtocol (
                    &gPspFlashAccSmmCommReadyProtocolGuid,
                    NULL,
                    &PspFlashAccSmmCommReady
                    );
  }

  if (Status != EFI_SUCCESS) {
    Status = EFI_NOT_READY;
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
RomArmorSpiErase (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINTN                     FlashAddress,
  IN  UINTN                     Size
  )
{
  EFI_STATUS                    Status;
  EFI_PHYSICAL_ADDRESS          DstAddress;
  SPI_CONFIG_BLOCK              *SpiConfigBlock;

//  DEBUG ( (EFI_D_INFO, "%a Start\n", __FUNCTION__));

  if (FlashDevice == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (mSpiSmmNorFlashProtocol == NULL) {
    if (mSmst2 != NULL) {
//      DEBUG ( (EFI_D_INFO, "Try to locate Rom Armor Protocol at %a\n",__FUNCTION__));
      Status = mSmst2->SmmLocateProtocol (&gEfiSpiSmmNorFlashProtocolGuid, NULL, (VOID**)&mSpiSmmNorFlashProtocol);
    }
    if (mSpiSmmNorFlashProtocol == NULL) {
//      DEBUG ( (EFI_D_INFO, "Rom Armor not Active\n"));
      return EFI_NOT_READY;
    }
  }

  SpiConfigBlock = (SPI_CONFIG_BLOCK *)FlashDevice->TypeSpecificInfo;

  if (PcdGetBool (PcdBiosImageInBottomSpiRom) &&
      (FlashAddress >= PcdGet32 (PcdFlashAreaBaseAddress))) {
    DstAddress = (UINT32)(FlashAddress - PcdGet32 (PcdFlashAreaBaseAddress));
  } else {
    DstAddress = (UINTN)(FlashAddress - (0x100000000 - (UINT64)SpiConfigBlock->DeviceSize));
  }
  DstAddress &= (UINT32)(SpiConfigBlock->DeviceSize - 1);

  Status = mSpiSmmNorFlashProtocol->Erase (
                                      mSpiSmmNorFlashProtocol,
                                      (UINT32)DstAddress,
                                      (UINT32)(Size / mSpiSmmNorFlashProtocol->EraseBlockBytes)
                                      );

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
RomArmorSpiProgram (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINT8                     *FlashAddress,
  IN  UINT8                     *SrcAddress,
  IN  UINTN                     *SPIBufferSize,
  IN  UINTN                     LbaWriteAddress
  )
{
  EFI_STATUS                    Status;
  UINTN                         DstAddress;
  UINT32                        SwappedAddress;
  SPI_CONFIG_BLOCK              *SpiConfigBlock;

//  DEBUG ( (EFI_D_INFO, "%a Start\n", __FUNCTION__));

  if (FlashDevice == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (mSpiSmmNorFlashProtocol == NULL) {
    if (mSmst2 != NULL) {
//      DEBUG ( (EFI_D_INFO, "Try to locate Rom Armor Protocol at %a\n",__FUNCTION__));
      Status = mSmst2->SmmLocateProtocol (&gEfiSpiSmmNorFlashProtocolGuid, NULL, (VOID**)&mSpiSmmNorFlashProtocol);
    }
    if (mSpiSmmNorFlashProtocol == NULL) {
      return EFI_NOT_READY;
    }
  }

  SpiConfigBlock = (SPI_CONFIG_BLOCK *)FlashDevice->TypeSpecificInfo;

  DstAddress     = (UINTN) FlashAddress;

  if (PcdGetBool (PcdBiosImageInBottomSpiRom) &&
      ((UINTN)DstAddress >= PcdGet32 (PcdFlashAreaBaseAddress))) {
    SwappedAddress = (UINT32)(DstAddress - PcdGet32 (PcdFlashAreaBaseAddress));
  } else {
    SwappedAddress = (UINT32)(DstAddress - (0x100000000 - (UINT64)SpiConfigBlock->DeviceSize));
  }
  SwappedAddress &= (UINT32)(SpiConfigBlock->DeviceSize - 1);

  Status = mSpiSmmNorFlashProtocol->WriteData (
                                      mSpiSmmNorFlashProtocol,
                                      SwappedAddress,
                                      (UINT32)*SPIBufferSize,
                                      SrcAddress
                                      );
  AsmWbinvd ();

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
RomArmorSpiRead (
  IN  FLASH_DEVICE              *FlashDevice,
  IN  UINT8                     *DstAddress,
  IN  UINT8                     *FlashAddress,
  IN  UINTN                     BufferSize
  )
{
  EFI_STATUS                    Status;
  SPI_CONFIG_BLOCK              *SpiConfigBlock;
  UINTN                         Address;
  UINT32                        SwappedAddress;

//  DEBUG ( (EFI_D_INFO, "%a Start\n", __FUNCTION__));

  if ((DstAddress == NULL) && (BufferSize == 0)) {
    return EFI_UNSUPPORTED;
  }

  if (mSpiSmmNorFlashProtocol == NULL) {
    if (mSmst2 != NULL) {
//      DEBUG ( (EFI_D_INFO, "Try to locate Rom Armor Protocol at %a\n",__FUNCTION__));
      Status = mSmst2->SmmLocateProtocol (&gEfiSpiSmmNorFlashProtocolGuid, NULL, (VOID**)&mSpiSmmNorFlashProtocol);
    }
    if (mSpiSmmNorFlashProtocol == NULL) {
      return EFI_NOT_READY;
    }
  }

  SpiConfigBlock = (SPI_CONFIG_BLOCK *)FlashDevice->TypeSpecificInfo;

  Address             = (UINTN) FlashAddress;

  if (PcdGetBool (PcdBiosImageInBottomSpiRom) &&
      ((UINTN)Address >= PcdGet32 (PcdFlashAreaBaseAddress))) {
    SwappedAddress = (UINT32)(Address - PcdGet32 (PcdFlashAreaBaseAddress));
  } else {
    SwappedAddress = (UINT32)(Address - (0x100000000 - (UINT64)SpiConfigBlock->DeviceSize));
  }
  SwappedAddress &= (UINT32)(SpiConfigBlock->DeviceSize - 1);

  Status = mSpiSmmNorFlashProtocol->ReadData (
                                      mSpiSmmNorFlashProtocol,
                                      (UINT32)SwappedAddress,
                                      (UINT32)BufferSize,
                                      DstAddress
                                      );
  AsmWbinvd ();

  return Status;
}

EFI_STATUS
EFIAPI
RomArmorLibInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_SMM_BASE2_PROTOCOL            *SmmBase2;
  BOOLEAN                           InSmm = FALSE; 

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**) &SmmBase2);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Can't Locate SmmBase2 Protocol\n"));
    return EFI_SUCCESS;
  }

  SmmBase2->InSmm (SmmBase2, &InSmm);
  if (!InSmm) {
    DEBUG ((DEBUG_INFO, "Not In SMM\n"));
    return EFI_SUCCESS;
  }
  Status = SmmBase2->GetSmstLocation (SmmBase2, &mSmst2);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Can't get Smst Location\n"));
    return EFI_SUCCESS;
  }

  Status = mSmst2->SmmLocateProtocol (&gEfiSpiSmmNorFlashProtocolGuid, NULL, (VOID**)&mSpiSmmNorFlashProtocol);
  DEBUG ((DEBUG_INFO, "Locate Rom Armor Protocol result: %r\n", Status));

  return EFI_SUCCESS;
}
