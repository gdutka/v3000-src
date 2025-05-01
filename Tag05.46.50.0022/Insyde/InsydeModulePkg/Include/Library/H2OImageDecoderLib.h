/** @file
  This library class provides services to decode image.
  
;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_IMAGE_DECODER_LIB_H_
#define _H2O_IMAGE_DECODER_LIB_H_

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/GifDecoder.h>
#include <Protocol/JpegDecoder.h>
#include <Protocol/TgaDecoder.h>
#include <Protocol/UgaDraw.h>

EFI_STATUS
EFIAPI
H2OHiiBmpDecode (
  IN   UINT8                         *ImageData,
  IN   UINTN                         ImageDataSize,
  OUT  UINT8                         **DecodedData,
  OUT  UINTN                         *DecodedDataSize,
  OUT  UINTN                         *Height,
  OUT  UINTN                         *Width
  );

EFI_STATUS
EFIAPI
H2OHiiJpegDecode (
  IN   UINT8                         *ImageData,
  IN   UINTN                         ImageDataSize,
  OUT  UINT8                         **DecodedData,
  OUT  UINTN                         *DecodedDataSize,
  OUT  UINTN                         *Height,
  OUT  UINTN                         *Width,
  OUT  EFI_JPEG_DECODER_STATUS       *DecoderStatus
  );

EFI_STATUS
EFIAPI
H2OHiiJpegDecoderGetMarkerData (
  IN     UINT8                         *Start,
  IN     UINT8                         *End,
  IN OUT EFI_JPEG_MARKER_TYPE          *MarkerType,
  OUT    UINT8                         **MarkerData,
  OUT    UINT32                        *DataSize,
  OUT    UINT8                         **Next  OPTIONAL
  );

EFI_STATUS
EFIAPI
H2OHiiPcxDecode (
  IN   UINT8                         *ImageData,
  IN   UINTN                         ImageDataSize,
  OUT  UINT8                         **DecodedData,
  OUT  UINTN                         *DecodedDataSize,
  OUT  UINTN                         *Height,
  OUT  UINTN                         *Width
  );

EFI_STATUS
EFIAPI
H2OHiiTgaDecode (
  IN   UINT8                        *ImageData,
  IN   UINTN                        ImageDataSize,
  OUT  UINT8                        **DecodedData,
  OUT  UINTN                        *DecodedDataSize,
  OUT  UINTN                        *PixelHeight,
  OUT  UINTN                        *PixelWidth,
  OUT  TGA_FORMAT                   *TgaFormat,
  OUT  BOOLEAN                      *HasAlphaChannel
  );

EFI_STATUS
EFIAPI
H2OHiiTgaCalculateBltImage (
  IN      UINT8                        *SourceImageData,
  IN      UINTN                        SourceImageDataSize,
  IN OUT  UINT8                        *DestinationImageData,
  IN      UINTN                        DestinationImageDataSize,
  IN      TGA_FORMAT                   TgaFormat
  );

EFI_STATUS
H2OHiiPngDecode (
  IN   UINT8                         *ImageData,
  IN   UINTN                         ImageDataSize,
  OUT  UINT8                         **DecodedData,
  OUT  UINTN                         *DecodedDataSize,
  OUT  UINTN                         *Height,
  OUT  UINTN                         *Width
  );

EFI_STATUS
EFIAPI
H2OHiiCreateAnimationFromMem (
  IN   UINT8                        *FileData,
  IN   UINTN                        FileSize,
  IN   VOID                         *Data, 
  OUT  ANIMATION                    **Animation
  );

EFI_STATUS
EFIAPI
H2OHiiDestroyAnimation (
  IN  ANIMATION                    *Animation
  );

EFI_STATUS
EFIAPI
H2OHiiNextAnimationFrame (
  IN  ANIMATION_REFRESH_ENTRY      *Entry,
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop 
  );

EFI_STATUS 
EFIAPI
H2OHiiRefreshAnimation (
  IN  ANIMATION_REFRESH_ENTRY      *AnimationRefrshEntry,
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
  IN  UINT64                       CpuFrequency  
  );

EFI_STATUS
EFIAPI
H2OHiiConvertAnimationToBlt (
  IN     ANIMATION                     *Animation,
  IN     UINTN                         BltBufferSize,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer
  );

#endif
