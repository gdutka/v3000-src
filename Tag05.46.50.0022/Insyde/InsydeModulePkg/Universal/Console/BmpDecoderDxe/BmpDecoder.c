/** @file
  This code supports a the private implementation
  of the BMP Decoder protocol

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "BmpDecoder.h"

BMP_DECODER_INSTANCE    mPrivateData = {
  BMP_DECODER_INSTANCE_SIGNATURE,
  NULL,
  { BmpDecoderDecodeImage }
};

/**
 Convert a *.BMP graphics image to a GOP/UGA blt buffer. If a NULL Blt buffer
 is passed in a GopBlt buffer will be allocated by this routine. If a GopBlt
 buffer is passed in it will be used if it is big enough.

 @param[in]  This               Pointer to BMP protocol instance
 @param[in]  ImageData          Pointer to BMP file
 @param[in]  ImageDataSize      Number of bytes in BMP file
 @param[out] DecodedData        Buffer containing GOP version of BMP file
 @param[out] DecodedDataSize    Size of DecodedData in bytes.
 @param[out] Height             Height of DecodedData/ImageData in pixels
 @param[out] Width              Width of DecodedData/ImageData in pixels

 @retval EFI_SUCCESS            GopBlt and GopBltSize are returned.
 @retval EFI_UNSUPPORTED        BmpImage is not a valid *.BMP image
 @retval EFI_BUFFER_TOO_SMALL   The passed in GopBlt buffer is not big enough.
                                GopBltSize will contain the required size.
 @retval EFI_OUT_OF_RESOURCES   No enough buffer to allocate

**/
EFI_STATUS
EFIAPI
BmpDecoderDecodeImage (
  IN     EFI_BMP_DECODER_PROTOCOL     *This,
  IN     UINT8                         *ImageData,
  IN     UINTN                         ImageDataSize,
     OUT UINT8                         **DecodedData,
     OUT UINTN                         *DecodedDataSize,
     OUT UINTN                         *Height,
     OUT UINTN                         *Width
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiBmpDecode (
             ImageData, 
             ImageDataSize,
             DecodedData,
             DecodedDataSize,
             Height,
             Width
             );
  
  return Status;
}

/**
 Install Driver to produce PCX Decoder protocol.

 @param [in]   ImageHandle
 @param [in]   SystemTable

 @retval EFI_SUCCESS            PCX Decoder protocol installed
 @return Other        No protocol installed, unload driver.

**/
EFI_STATUS
EFIAPI
BmpDecoderInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;

  Status = gBS->InstallProtocolInterface (
                  &mPrivateData.Handle,
                  &gEfiBmpDecoderProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPrivateData.BmpDecoder
                  );
  return Status;
}
