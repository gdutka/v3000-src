/** @file
  This code supports a the private implementation
  of the PNG Decoder protocol

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

#include "PngDecoder.h"

PNG_DECODER_INSTANCE    mPrivateData = {
  PNG_DECODER_INSTANCE_SIGNATURE,
  NULL,
  PngDecoderDecodeImage
};

/**
 Convert a *.PNG graphics image to a UGA blt buffer.
 If it isn't a PNG format graphics, it will return error.

 @param [in]   This
 @param [in]   ImageData        Pointer to PNG file
 @param [in]   ImageDataSize    Number of bytes in ImageData
 @param [out]  DecodedData      Buffer containing UGA version of BmpImage.
 @param [out]  DecodedDataSize  Size of DecodedData in bytes.
 @param [out]  Height           Height of DecodedData/BmpImage in pixels
 @param [out]  Width            Width of DecodedData/BmpImage in pixels

 @retval EFI_SUCCESS            DecodedData and DecodedDataSize are returned.
 @retval EFI_UNSUPPORTED        ImageData is not a valid *.PNG image

**/
EFI_STATUS
PngDecoderDecodeImage (
  IN     EFI_PNG_DECODER_PROTOCOL      *This,
  IN     UINT8                         *ImageData,
  IN     UINTN                         ImageDataSize,
     OUT UINT8                         **DecodedData,
     OUT UINTN                         *DecodedDataSize,
     OUT UINTN                         *Height,
     OUT UINTN                         *Width
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiPngDecode (
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
 Install Driver to produce PNG Decoder protocol.

 @param [in]   ImageHandle
 @param [in]   SystemTable

 @retval EFI_SUCCESS            PNG Decoder protocol installed
 @return Other                  No protocol installed, unload driver.

**/
EFI_STATUS
PngDecoderInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;

  Status = gBS->InstallProtocolInterface (
                  &mPrivateData.Handle,
                  &gEfiPngDecoderProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPrivateData.PngDecoder
                  );
  return Status;
}
