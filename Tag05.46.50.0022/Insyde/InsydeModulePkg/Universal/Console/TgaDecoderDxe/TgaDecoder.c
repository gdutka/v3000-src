/** @file
  This code supports a the private implementation
  of the TGA Decoder protocol

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

#include "TgaDecoder.h"

TGA_DECODER_INSTANCE    mPrivateData = {
  TGA_DECODER_INSTANCE_SIGNATURE,
  NULL,
  {
   TgaDecoderDecodeImage,
   TgaCalculateBltImage
  }
};


/**
 Install Driver to produce Tga Decoder protocol.

 @param [in]   ImageHandle
 @param [in]   SystemTable

 @retval EFI_SUCCESS            TGA Decoder protocol installed
 @return Other        No protocol installed, unload driver.

**/
EFI_STATUS
EFIAPI
TgaDecoderInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;


  Status = gBS->InstallProtocolInterface (
                  &mPrivateData.Handle,
                  &gEfiTgaDecoderProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPrivateData.TgaDecoder
                  );
  return Status;
}

/**
 Convert a *.TGA graphics image to a TGA DecodedData buffer. If a NULL DecodedData buffer
 is passed in a DecodedData buffer will be allocated by this routine. If a DecodedData
 buffer is passed in it will be used if it is big enough.

 @param [in]   This             Pointer to protocol instance
 @param [in]   ImageData        Pointer to TGA file
 @param [in]   ImageDataSize    Number of bytes in ImageData
 @param [in, out] DecodedData   Buffer containing Tga pixle color map.
 @param [in, out] DecodedDataSize  Size of DecodedData in bytes.
 @param [out]  PixelHeight      Height of DecodedData in pixels
 @param [out]  PixelWidth       Width of DecodedData in pixels
 @param [out]  TgaFormat        TGA file format
 @param [out]  HasAlphaChannel  This image type has alpha channel or not

 @retval EFI_SUCCESS            DecodedData and DecodedDataSize are returned.
 @retval EFI_INVALID_PARAMETER  Input parameter is invalid
 @retval EFI_UNSUPPORTED        ImageData is not a valid *.TGA image
 @retval EFI_BUFFER_TOO_SMALL   The passed in DecodedData buffer is not big enough.
                                DecodedDataSize will contain the required size.
 @retval EFI_OUT_OF_RESOURCES   No enough buffer to allocate

**/
EFI_STATUS
EFIAPI
TgaDecoderDecodeImage (
  IN      EFI_TGA_DECODER_PROTOCOL     *This,
  IN      UINT8                        *ImageData,
  IN      UINTN                        ImageDataSize,
  IN OUT  UINT8                        **DecodedData,
  IN OUT  UINTN                        *DecodedDataSize,
     OUT  UINTN                        *PixelHeight,
     OUT  UINTN                        *PixelWidth,
     OUT  TGA_FORMAT                   *TgaFormat,
     OUT  BOOLEAN                      *HasAlphaChannel
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiTgaDecode (
             ImageData, 
             ImageDataSize,
             DecodedData,
             DecodedDataSize,
             PixelHeight,
             PixelWidth,
             TgaFormat,
             HasAlphaChannel
             );
  
  return Status;
}

/**
 Based on source Image and destionation image to Calculate the final blt
 buffer and the final blt data is put in DestinationImageData

 @param [in]   This             Pointer to protocol instance
 @param [in]   SourceImageData  Pointer to source image data (Converted data which is read
                                from file)
 @param [in]   SourceImageDataSize  Total size of source image data
 @param [in, out] DestinationImageData  IN:  Pointer to destination image data(read from video)
                                OUT: The final blt data
 @param [in]   DestinationImageDataSize  Size of DecodedData in bytes.
 @param [in]   TgaFormat        Tga file format

 @retval EFI_SUCCESS            Calculate final blt data successfully.
 @retval EFI_INVALID_PARAMETER  Input parameter is invalid
 @retval EFI_UNSUPPORTED        It doesn't support this TGA format

**/
EFI_STATUS
EFIAPI
TgaCalculateBltImage (
  IN      EFI_TGA_DECODER_PROTOCOL     *This,
  IN      UINT8                        *SourceImageData,
  IN      UINTN                        SourceImageDataSize,
  IN OUT  UINT8                        *DestinationImageData,
  IN      UINTN                        DestinationImageDataSize,
  IN      TGA_FORMAT                   TgaFormat
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiTgaCalculateBltImage (
             SourceImageData, 
             SourceImageDataSize,
             DestinationImageData,
             DestinationImageDataSize,
             TgaFormat
             );
  
  return Status;
}

