/** @file
  The instance of PCX Decoder Library

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "H2OImageDecoderLibCommon.h"
#include <Library/DebugLib.h>
#include <Guid/Pcx.h>

//  256 color, 256*3 bytes Palette.
#define COLOR_NUMBER          256
#define PALETTE_SIZE          0x300

/**
 Convert a *.PCX graphics image to a UGA blt buffer.
 If it isn't a PCX format graphics, it will return error.

 @param [in]   This
 @param [in]   ImageData        Pointer to PCX file
 @param [in]   ImageDataSize    Number of bytes in ImageData
 @param [out]  DecodedData      Buffer containing UGA version of BmpImage.
 @param [out]  DecodedDataSize  Size of DecodedData in bytes.
 @param [out]  Height           Height of DecodedData/BmpImage in pixels
 @param [out]  Width            Width of DecodedData/BmpImage in pixels

 @retval EFI_SUCCESS            DecodedData and DecodedDataSize are returned.
 @retval EFI_UNSUPPORTED        ImageData is not a valid *.PCX image

**/
EFI_STATUS
EFIAPI
H2OHiiPcxDecode (
  IN     UINT8                         *ImageData,
  IN     UINTN                         ImageDataSize,
     OUT UINT8                         **DecodedData,
     OUT UINTN                         *DecodedDataSize,
     OUT UINTN                         *Height,
     OUT UINTN                         *Width
  )
{
  UINT8                                 *PcxBuffer = NULL;
  EFI_UGA_PIXEL                         *ImageBuffer = NULL;
  PCX_IMAGE_HEADER                      *PcxHeader = NULL;
  EFI_UGA_PIXEL                         Palette[COLOR_NUMBER];
  UINTN                                 NPlane;
  UINTN                                 BytesPerLine;
  UINTN                                 TotalBytes;
  UINTN                                 LineBreak;
  UINTN                                 Index;
  UINTN                                 Total;
  UINTN                                 Counter;
  UINTN                                 Tmp;
  UINT64                                BltBufferSize;
  UINTN                                 CompressedDataSize;
  UINT8                                 *CompressedData;
  UINT8                                 *ScanLine;
  UINTN                                 ScanLineSize;
  UINTN                                 Xaxis;
  UINTN                                 Yaxis;
  UINT8                                 Value;

  if (ImageData == NULL || DecodedData == NULL || DecodedDataSize == NULL || Height == NULL || Width == NULL ||
      ImageDataSize <= sizeof (PCX_IMAGE_HEADER)) {
    return EFI_UNSUPPORTED;
  }

  Total = 0;
  Index = 0;
  LineBreak = 0;
  CompressedDataSize = ImageDataSize - sizeof (PCX_IMAGE_HEADER);
  //
  //  check if it is a PCX format file.
  //  Manufacturer  0x0A  - ZSoft
  //  Version       0x05  - Version 3.0
  //  Encodeing     0x01  - run length encoding
  //  BitsPerPixel  0x08  - Number of bits to represent a pixel
  //
  PcxHeader = (PCX_IMAGE_HEADER *)ImageData;
  PcxBuffer = (UINT8 *)ImageData;

  if ((PcxHeader->Manufacturer != 0x0A) ||
      (PcxHeader->Version != 0x05) ||
      (PcxHeader->Encoding != 0x01) ||
      (PcxHeader->BitsPerPixel != 0x08)) {
    return EFI_UNSUPPORTED;
  }
  //
  //  Count Width,Height...from header
  //
  if (PcxHeader->Xmax < PcxHeader->Xmin ||
      PcxHeader->Ymax < PcxHeader->Ymin) {
    return EFI_UNSUPPORTED;
  }
  *Width = PcxHeader->Xmax - PcxHeader->Xmin + 0x1;
  *Height= PcxHeader->Ymax - PcxHeader->Ymin + 0x1;
  NPlane = PcxHeader->NPlanes;
  BytesPerLine = PcxHeader->BytesPerLine;
  TotalBytes = NPlane * BytesPerLine;
  //
  //  read form the end of Header
  //
  CompressedData = &PcxBuffer[sizeof(PCX_IMAGE_HEADER)];
  BltBufferSize = MultU64x32 ((UINT64) (*Width), (UINT32) (*Height) * (UINT32) sizeof (EFI_UGA_PIXEL));
  if (BltBufferSize > MAX_UINT32) {
    return EFI_UNSUPPORTED;
  }
  ImageBuffer = (EFI_UGA_PIXEL *)H2OImageDecoderLibAllocateMem ((UINTN) BltBufferSize);
  ASSERT (ImageBuffer != NULL);
  if (ImageBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // 24/32 -Bit .PCX Files.
  // 24 bit images are stored as version 5 or above as 8 bit, 3 plane images.
  // 32 bit images are stored as version 5 or above as 8 bit, 4 plane images.
  // 24/32 bit images do not contain a palette.
  // Bit planes are ordered as lines of red, green, blue in that order.
  //
  ScanLine = NULL;
  ScanLineSize = 0;
  if (((PcxHeader->NPlanes == 0x03) || (PcxHeader->NPlanes == 0x04)) && (PcxHeader->BitsPerPixel == 0x08)) {
    ScanLineSize = TotalBytes + 0x32;
    if (PcxHeader->NPlanes == 0x03) {
      if (ScanLineSize <= *Width + (BytesPerLine << 1)) {
        goto Error;
      }
    } else {
      if (ScanLineSize <= *Width + BytesPerLine * 3) {
        goto Error;
      }
    }

    ScanLine = (UINT8 *)H2OImageDecoderLibAllocateZeroMem (ScanLineSize);
    if (ScanLine == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    for (Yaxis = 0; Yaxis < *Height; Yaxis++) {
      Tmp = 0;
      while (Tmp < TotalBytes) {
        if (Index >= CompressedDataSize) {
          goto Error;
        }

        Counter = 1;
        Value   = CompressedData[Index++];
        if ((Value & 0xC0) == 0xC0) {
          if (Index >= CompressedDataSize) {
            goto Error;
          }
          Counter = (UINTN) (Value & 0x3F);
          Value = CompressedData[Index++];
        }
        while ((Tmp < TotalBytes) && Counter--) {
          ScanLine[Tmp++] = Value;
        }
      }

      if (PcxHeader->NPlanes == 0x03) {
        for (Xaxis = 0; Xaxis < *Width; Xaxis++) {
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Red   = ScanLine[Xaxis];
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Green = ScanLine[Xaxis + BytesPerLine];
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Blue  = ScanLine[Xaxis + (BytesPerLine << 1)];
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Reserved = 0xFF;
        }
      } else {
        for (Xaxis = 0; Xaxis < *Width; Xaxis++) {
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Red   = ScanLine[Xaxis];
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Green = ScanLine[Xaxis + BytesPerLine];
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Blue  = ScanLine[Xaxis + (BytesPerLine << 1)];
          ImageBuffer[(Yaxis * (*Width)) + Xaxis].Reserved = ScanLine[Xaxis +  BytesPerLine * 3];
        }
      }
    }

    *DecodedData = (UINT8 *)ImageBuffer;
    *DecodedDataSize = (UINTN) BltBufferSize;
    H2OImageDecoderLibFreeMem (ScanLine, ScanLineSize);
    return EFI_SUCCESS;
  }

  //
  //  Set palette from the end of pcx image.
  //
  if (CompressedDataSize <= PALETTE_SIZE) {
    goto Error;
  }
  for (Tmp = 0; Tmp < COLOR_NUMBER; Tmp ++) {
    Palette[Tmp].Red   = PcxBuffer[ImageDataSize - PALETTE_SIZE + Tmp * 3];
    Palette[Tmp].Green = PcxBuffer[ImageDataSize - PALETTE_SIZE + Tmp * 3 + 1];
    Palette[Tmp].Blue  = PcxBuffer[ImageDataSize - PALETTE_SIZE + Tmp * 3 + 2];
    Palette[Tmp].Reserved = 0;
  }

  while (Total < ((*Width) * (*Height))) {
    //
    //  skip the blank area at the end of scanline
    //
    if (LineBreak == (*Width)) {
      if (LineBreak < TotalBytes) {
        Index += (TotalBytes - LineBreak);
      }
      LineBreak = 0;
    }
    //
    //  decode the color map
    //
    if (Index >= CompressedDataSize) {
      goto Error;
    }
    if ((CompressedData[Index] & 0xC0) == 0xC0) {
      Counter = (UINTN) (CompressedData[Index++] & 0x3F);
      if (Index >= CompressedDataSize) {
        goto Error;
      }
      for (Tmp = 0; Tmp < Counter; Tmp++) {
        ImageBuffer[Total] = Palette[CompressedData[Index]];
        Total++;
        LineBreak++;
        if (LineBreak == (*Width)) {
          break;
        }
      }
    } else {
      ImageBuffer[Total] = Palette[CompressedData[Index]];
      Total++;
      LineBreak++;
    }
    Index++;
  }
  //
  //  Set return buffer
  //
  *DecodedData = (UINT8 *)ImageBuffer;
  *DecodedDataSize = (UINTN) BltBufferSize;
  return EFI_SUCCESS;

Error:
  if (ScanLine != NULL) {
    H2OImageDecoderLibFreeMem (ScanLine, ScanLineSize);
  }
  H2OImageDecoderLibFreeMem (ImageBuffer, (UINTN) BltBufferSize);
  return EFI_UNSUPPORTED;
}
