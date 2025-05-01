/** @file
 Implementation for EFI_HII_IMAGE_PROTOCOL.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

 Copyright (c) 2007 - 2008, Intel Corporation. All rights reserved.<BR>
 This program and the accompanying materials
 are licensed and made available under the terms and conditions of the BSD License
 which accompanies this distribution.  The full text of the license may be found at
 http://opensource.org/licenses/bsd-license.php

 THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

*/

#include "HiiDatabase.h"
#include <Protocol/JpegDecoder.h>
#include <Protocol/PngDecoder.h>

EFI_JPEG_DECODER_PROTOCOL          *mJpegDecoder = NULL;
EFI_PNG_DECODER_PROTOCOL           *mPngDecoder  = NULL;

/**
 Get the imageid of last image block: EFI_HII_IIBT_END_BLOCK when input
 ImageId is zero, otherwise return the address of the
 corresponding image block with identifier specified by ImageId.

 This is a internal function.

 @param[in]      ImageBlock      Points to the beginning of a series of image blocks stored in order.
 @param[in, out] ImageId         If input ImageId is 0, output the image id of the EFI_HII_IIBT_END_BLOCK;
                                 else use this id to find its corresponding image block address.

 @return The image block address when input ImageId is not zero; otherwise return NULL.
**/
UINT8*
GetImageIdOrAddress (
  IN  UINT8           *ImageBlock,
  IN OUT EFI_IMAGE_ID *ImageId
  )
{
  EFI_IMAGE_ID                   ImageIdCurrent;
  UINT8                          *ImageBlockHdr;
  UINT8                          Length8;
  UINT16                         Length16;
  UINT32                         Length32;
  EFI_HII_IIBT_IMAGE_1BIT_BLOCK  Iibt1bit;
  EFI_HII_IIBT_IMAGE_4BIT_BLOCK  Iibt4bit;
  EFI_HII_IIBT_IMAGE_8BIT_BLOCK  Iibt8bit;
  UINT16                         Width;
  UINT16                         Height;

  ASSERT (ImageBlock != NULL && ImageId != NULL);

  ImageBlockHdr  = ImageBlock;
  ImageIdCurrent = 1;

  while (((EFI_HII_IMAGE_BLOCK *) ImageBlock)->BlockType != EFI_HII_IIBT_END) {
    if (*ImageId > 0) {
      if (*ImageId == ImageIdCurrent) {
        //
        // If the found image block is a duplicate block, update the ImageId to
        // find the previous defined image block.
        //
        if (((EFI_HII_IMAGE_BLOCK *) ImageBlock)->BlockType == EFI_HII_IIBT_DUPLICATE) {
          CopyMem (ImageId, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (EFI_IMAGE_ID));
          ASSERT (*ImageId != ImageIdCurrent);
          ImageBlock = ImageBlockHdr;
          ImageIdCurrent = 1;
          continue;
        }

        return ImageBlock;
      }
      if (*ImageId < ImageIdCurrent) {
        //
        // Can not find the specified image block in this image.
        //
        return NULL;
      }
    }
    switch (((EFI_HII_IMAGE_BLOCK *) ImageBlock)->BlockType) {
    case EFI_HII_IIBT_EXT1:
      Length8 = *(ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK) + sizeof (UINT8));
      ImageBlock += Length8;
      break;
    case EFI_HII_IIBT_EXT2:
      CopyMem (
        &Length16,
        ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK) + sizeof (UINT8),
        sizeof (UINT16)
        );
      ImageBlock += Length16;

      break;
    case EFI_HII_IIBT_EXT4:
      CopyMem (
        &Length32,
        ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK) + sizeof (UINT8),
        sizeof (UINT32)
        );
      ImageBlock += Length32;
      break;

    case EFI_HII_IIBT_IMAGE_1BIT:
    case EFI_HII_IIBT_IMAGE_1BIT_TRANS:
      CopyMem (&Iibt1bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK));
      ImageBlock += sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK) - sizeof (UINT8) +
                    BITMAP_LEN_1_BIT (Iibt1bit.Bitmap.Width, Iibt1bit.Bitmap.Height);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_IMAGE_4BIT:
    case EFI_HII_IIBT_IMAGE_4BIT_TRANS:
      CopyMem (&Iibt4bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK));
      ImageBlock += sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK) - sizeof (UINT8) +
                    BITMAP_LEN_4_BIT (Iibt4bit.Bitmap.Width, Iibt4bit.Bitmap.Height);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_IMAGE_8BIT:
    case EFI_HII_IIBT_IMAGE_8BIT_TRANS:
      CopyMem (&Iibt8bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK));
      ImageBlock += sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK) - sizeof (UINT8) +
                    BITMAP_LEN_8_BIT (Iibt8bit.Bitmap.Width, Iibt8bit.Bitmap.Height);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_IMAGE_24BIT:
    case EFI_HII_IIBT_IMAGE_24BIT_TRANS:
      CopyMem (&Width, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (UINT16));
      CopyMem (
        &Height,
        ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK) + sizeof (UINT16),
        sizeof (UINT16)
        );
      ImageBlock += sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) - sizeof (EFI_HII_RGB_PIXEL) +
                    BITMAP_LEN_24_BIT (Width, Height);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_DUPLICATE:
      ImageBlock += sizeof (EFI_HII_IIBT_DUPLICATE_BLOCK);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_IMAGE_JPEG:
      CopyMem (&Length32, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (UINT32));
      ImageBlock += Length32 + sizeof (EFI_HII_IIBT_JPEG_BLOCK) - sizeof (UINT8);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_IMAGE_PNG:
      CopyMem (&Length32, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (UINT32));
      ImageBlock += Length32 + sizeof (EFI_HII_IIBT_PNG_BLOCK) - sizeof (UINT8);
      ImageIdCurrent++;
      break;

    case EFI_HII_IIBT_SKIP1:
      Length8 = *(ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK));
      ImageBlock += sizeof (EFI_HII_IIBT_SKIP1_BLOCK);
      ImageIdCurrent = (UINT16) (ImageIdCurrent + Length8);
      break;

    case EFI_HII_IIBT_SKIP2:
      CopyMem (&Length16, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (UINT16));
      ImageBlock += sizeof (EFI_HII_IIBT_SKIP2_BLOCK);
      ImageIdCurrent = (UINT16) (ImageIdCurrent + Length16);
      break;

    default:
      //
      // Unknown image blocks can not be skipped, processing halts.
      //
      ASSERT (FALSE);
    }
  }

  //
  // When ImageId is zero, return the imageid of last image block: EFI_HII_IIBT_END_BLOCK.
  //
  if (*ImageId == 0) {
    *ImageId = ImageIdCurrent;
    return ImageBlock;
  }

  return NULL;
}

/**
 Convert pixels from EFI_GRAPHICS_OUTPUT_BLT_PIXEL to EFI_HII_RGB_PIXEL style.

 This is a internal function.

 @param[out] BitMapOut              Pixels in EFI_HII_RGB_PIXEL format.
 @param[in]  BitMapIn               Pixels in EFI_GRAPHICS_OUTPUT_BLT_PIXEL format.
 @param[in]  PixelNum               The number of pixels to be converted.
**/
VOID
CopyGopToRgbPixel (
  OUT EFI_HII_RGB_PIXEL              *BitMapOut,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BitMapIn,
  IN  UINTN                          PixelNum
  )
{
  UINTN Index;

  ASSERT (BitMapOut != NULL && BitMapIn != NULL);

  for (Index = 0; Index < PixelNum; Index++) {
    CopyMem (BitMapOut + Index, BitMapIn + Index, sizeof (EFI_HII_RGB_PIXEL));
  }
}

/**
 Convert pixels from EFI_HII_RGB_PIXEL to EFI_GRAPHICS_OUTPUT_BLT_PIXEL style.

 This is a internal function.

 @param[out] BitMapOut              Pixels in EFI_GRAPHICS_OUTPUT_BLT_PIXEL format.
 @param[in]  BitMapIn               Pixels in EFI_HII_RGB_PIXEL format.
 @param[in]  PixelNum               The number of pixels to be converted.
**/
VOID
CopyRgbToGopPixel (
  OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BitMapOut,
  IN  EFI_HII_RGB_PIXEL              *BitMapIn,
  IN  UINTN                          PixelNum
  )
{
  UINTN Index;

  ASSERT (BitMapOut != NULL && BitMapIn != NULL);

  for (Index = 0; Index < PixelNum; Index++) {
    CopyMem (BitMapOut + Index, BitMapIn + Index, sizeof (EFI_HII_RGB_PIXEL));
  }
}

/**
 Output pixels in "1 bit per pixel" format to an image.

 This is a internal function.

 @param[in, out] Image                  Points to the image which will store the pixels.
 @param[in]      Data                   Stores the value of output pixels, 0 or 1.
 @param[in]      PaletteInfo            PaletteInfo which stores the color of the output
                                        pixels. First entry corresponds to color 0 and
                                        second one to color 1.
 @retval EFI_SUCCESS                    The new image was generated successfully.
 @retval EFI_OUT_OF_RESOURCES           The bitmap could not be generated because there was not enough memory.
**/
EFI_STATUS
Output1bitPixel (
  IN OUT EFI_IMAGE_INPUT             *Image,
  IN UINT8                           *Data,
  IN EFI_HII_IMAGE_PALETTE_INFO      *PaletteInfo
  )
{
  UINT16                             Xpos;
  UINT16                             Ypos;
  UINTN                              OffsetY;
  UINT8                              Index;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      *BitMapPtr;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      PaletteValue[2];
  EFI_HII_IMAGE_PALETTE_INFO         *Palette;
  UINT16                             PaletteSize;
  UINT8                              Byte;

  ASSERT (Image != NULL && Data != NULL && PaletteInfo != NULL);

  BitMapPtr = Image->Bitmap;

  //
  // First entry corresponds to color 0 and second entry corresponds to color 1.
  //
  CopyMem (&PaletteSize, PaletteInfo, sizeof (UINT16));
  PaletteSize += sizeof (UINT16);
  Palette = AllocateZeroPool (PaletteSize);
  ASSERT (Palette != NULL);
  if (Palette == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Palette, PaletteInfo, PaletteSize);

  ZeroMem (PaletteValue, sizeof (PaletteValue));
  CopyRgbToGopPixel (&PaletteValue[0], &Palette->PaletteValue[0], 1);
  CopyRgbToGopPixel (&PaletteValue[1], &Palette->PaletteValue[1], 1);
  FreePool (Palette);

  //
  // Convert the pixel from one bit to corresponding color.
  //
  for (Ypos = 0; Ypos < Image->Height; Ypos++) {
    OffsetY = BITMAP_LEN_1_BIT (Image->Width, Ypos);
    //
    // All bits in these bytes are meaningful
    //
    for (Xpos = 0; Xpos < Image->Width / 8; Xpos++) {
      Byte = *(Data + OffsetY + Xpos);
      for (Index = 0; Index < 8; Index++) {
        if ((Byte & (1 << Index)) != 0) {
          BitMapPtr[Ypos * Image->Width + Xpos * 8 + (8 - Index - 1)] = PaletteValue[1];
        } else {
          BitMapPtr[Ypos * Image->Width + Xpos * 8 + (8 - Index - 1)] = PaletteValue[0];
        }
      }
    }

    if (Image->Width % 8 != 0) {
      //
      // Padding bits in this byte should be ignored.
      //
      Byte = *(Data + OffsetY + Xpos);
      for (Index = 0; Index < Image->Width % 8; Index++) {
        if ((Byte & (1 << (8 - Index - 1))) != 0) {
          BitMapPtr[Ypos * Image->Width + Xpos * 8 + Index] = PaletteValue[1];
        } else {
          BitMapPtr[Ypos * Image->Width + Xpos * 8 + Index] = PaletteValue[0];
        }
      }
    }
  }
  return EFI_SUCCESS;
}

/**
 Output pixels in "4 bit per pixel" format to an image.

 This is a internal function.

 @param[in, out] Image                  Points to the image which will store the pixels.
 @param[in]      Data                   Stores the value of output pixels, 0 ~ 15.
 @param[in]      PaletteInfo            PaletteInfo which stores the color of the output
                                        pixels. Each entry corresponds to a color within [0, 15].
 @retval EFI_SUCCESS                    The new image was generated successfully.
 @retval EFI_OUT_OF_RESOURCES           The bitmap could not be generated because there was not enough memory.
**/
EFI_STATUS
Output4bitPixel (
  IN OUT EFI_IMAGE_INPUT             *Image,
  IN UINT8                           *Data,
  IN EFI_HII_IMAGE_PALETTE_INFO      *PaletteInfo
  )
{
  UINT16                             Xpos;
  UINT16                             Ypos;
  UINTN                              OffsetY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      *BitMapPtr;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      PaletteValue[16];
  EFI_HII_IMAGE_PALETTE_INFO         *Palette;
  UINT16                             PaletteSize;
  UINT16                             PaletteNum;
  UINT8                              Byte;

  ASSERT (Image != NULL && Data != NULL && PaletteInfo != NULL);

  BitMapPtr = Image->Bitmap;

  //
  // The bitmap should allocate each color index starting from 0.
  //
  CopyMem (&PaletteSize, PaletteInfo, sizeof (UINT16));
  PaletteSize += sizeof (UINT16);
  Palette = AllocateZeroPool (PaletteSize);
  ASSERT (Palette != NULL);
  if (Palette == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Palette, PaletteInfo, PaletteSize);
  PaletteNum = (UINT16)(Palette->PaletteSize / sizeof (EFI_HII_RGB_PIXEL));

  ZeroMem (PaletteValue, sizeof (PaletteValue));
  CopyRgbToGopPixel (PaletteValue, Palette->PaletteValue, MIN (PaletteNum, ARRAY_SIZE (PaletteValue)));
  FreePool (Palette);

  //
  // Convert the pixel from 4 bit to corresponding color.
  //
  for (Ypos = 0; Ypos < Image->Height; Ypos++) {
    OffsetY = BITMAP_LEN_4_BIT (Image->Width, Ypos);
    //
    // All bits in these bytes are meaningful
    //
    for (Xpos = 0; Xpos < Image->Width / 2; Xpos++) {
      Byte = *(Data + OffsetY + Xpos);
      BitMapPtr[Ypos * Image->Width + Xpos * 2]     = PaletteValue[Byte >> 4];
      BitMapPtr[Ypos * Image->Width + Xpos * 2 + 1] = PaletteValue[Byte & 0x0F];
    }

    if (Image->Width % 2 != 0) {
      //
      // Padding bits in this byte should be ignored.
      //
      Byte = *(Data + OffsetY + Xpos);
      BitMapPtr[Ypos * Image->Width + Xpos * 2]     = PaletteValue[Byte >> 4];
    }
  }
  return EFI_SUCCESS;
}

/**
  Output pixels in "8 bit per pixel" format to an image.

  This is a internal function.

  @param[in, out] Image              Points to the image which will store the pixels.
  @param[in]      Data               Stores the value of output pixels, 0 ~ 255.
  @param[in]      PaletteInfo        PaletteInfo which stores the color of the output
                                     pixels. Each entry corresponds to a color within [0, 255].
 @retval EFI_SUCCESS                    The new image was generated successfully.
 @retval EFI_OUT_OF_RESOURCES           The bitmap could not be generated because there was not enough memory.
**/
EFI_STATUS
Output8bitPixel (
  IN OUT EFI_IMAGE_INPUT             *Image,
  IN UINT8                           *Data,
  IN EFI_HII_IMAGE_PALETTE_INFO      *PaletteInfo
  )
{
  UINT16                             Xpos;
  UINT16                             Ypos;
  UINTN                              OffsetY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      *BitMapPtr;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      PaletteValue[256];
  EFI_HII_IMAGE_PALETTE_INFO         *Palette;
  UINT16                             PaletteSize;
  UINT16                             PaletteNum;
  UINT8                              Byte;

  ASSERT (Image != NULL && Data != NULL && PaletteInfo != NULL);

  BitMapPtr = Image->Bitmap;

  //
  // The bitmap should allocate each color index starting from 0.
  //
  CopyMem (&PaletteSize, PaletteInfo, sizeof (UINT16));
  PaletteSize += sizeof (UINT16);
  Palette = AllocateZeroPool (PaletteSize);
  ASSERT (Palette != NULL);
  if (Palette == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Palette, PaletteInfo, PaletteSize);
  PaletteNum = (UINT16)(Palette->PaletteSize / sizeof (EFI_HII_RGB_PIXEL));
  ZeroMem (PaletteValue, sizeof (PaletteValue));
  CopyRgbToGopPixel (PaletteValue, Palette->PaletteValue, MIN (PaletteNum, ARRAY_SIZE (PaletteValue)));
  FreePool (Palette);

  //
  // Convert the pixel from 8 bits to corresponding color.
  //
  for (Ypos = 0; Ypos < Image->Height; Ypos++) {
    OffsetY = BITMAP_LEN_8_BIT (Image->Width, Ypos);
    //
    // All bits are meaningful since the bitmap is 8 bits per pixel.
    //
    for (Xpos = 0; Xpos < Image->Width; Xpos++) {
      Byte = *(Data + OffsetY + Xpos);
      BitMapPtr[OffsetY + Xpos] = PaletteValue[Byte];
    }
  }
  return EFI_SUCCESS;
}

/**
 Output pixels in "24 bit per pixel" format to an image.

 This is a internal function.

 @param[in, out] Image          Points to the image which will store the pixels.
 @param[in]      Data           Stores the color of output pixels, allowing 16.8
                                millions colors.
**/
VOID
Output24bitPixel (
  IN OUT EFI_IMAGE_INPUT             *Image,
  IN EFI_HII_RGB_PIXEL               *Data
  )
{
  UINT16                             Ypos;
  UINTN                              OffsetY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      *BitMapPtr;

  ASSERT (Image != NULL && Data != NULL);

  BitMapPtr = Image->Bitmap;

  for (Ypos = 0; Ypos < Image->Height; Ypos++) {
    OffsetY = BITMAP_LEN_8_BIT (Image->Width, Ypos);
    CopyRgbToGopPixel (&BitMapPtr[OffsetY], &Data[OffsetY], Image->Width);
  }

}

#define muldiv255(a,b)		((unsigned char)((((a)+1)*(b))>>8))		/* very fast, 92% accurate*/

/**
 Convert the image from EFI_IMAGE_INPUT to EFI_IMAGE_OUTPUT format.

 This is a internal function.

 @param[in]      BltBuffer      Buffer points to bitmap data of incoming image.
 @param[in]      BltX           Specifies the offset from the left and top edge of
                                the output image of the first pixel in the image.
 @param[in]      BltY           Specifies the offset from the left and top edge of
                                the output image of the first pixel in the image.
 @param[in]      Width          Width of the incoming image, in pixels.
 @param[in]      Height         Height of the incoming image, in pixels.
 @param[in]      Transparent    If TRUE, all "off" pixels in the image will be
                                drawn using the pixel value from blt and all other
                                pixels will be copied.
 @param[in, out] Blt            Buffer points to bitmap data of output image.

 @retval EFI_SUCCESS            The image was successfully converted.
 @retval EFI_INVALID_PARAMETER  Any incoming parameter is invalid.
**/
EFI_STATUS
ImageToBlt (
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BltBuffer,
  IN UINTN                           BltX,
  IN UINTN                           BltY,
  IN UINTN                           Width,
  IN UINTN                           Height,
  IN BOOLEAN                         Transparent,
  IN BOOLEAN                         AlphaSupport,
  IN OUT EFI_IMAGE_OUTPUT            **Blt
  )
{
  EFI_IMAGE_OUTPUT                   *ImageOut;
  UINTN                              Xpos;
  UINTN                              Ypos;
  UINTN                              OffsetY1; // src buffer
  UINTN                              OffsetY2; // dest buffer
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      SrcPixel;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      ZeroPixel;
  UINT8                              Alpha;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL      DstPixel;

  if (BltBuffer == NULL || Blt == NULL || *Blt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ImageOut = *Blt;

  if (Width + BltX > ImageOut->Width) {
    return EFI_INVALID_PARAMETER;
  }
  if (Height + BltY > ImageOut->Height) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&ZeroPixel, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  if (AlphaSupport) {
    for (Ypos = 0; Ypos < Height; Ypos++) {
      OffsetY1 = Width * Ypos;
      OffsetY2 = ImageOut->Width * (BltY + Ypos);
      for (Xpos = 0; Xpos < Width; Xpos++) {
        SrcPixel = BltBuffer[OffsetY1 + Xpos];
        DstPixel = ImageOut->Image.Bitmap[OffsetY2 + BltX + Xpos];
        Alpha    = SrcPixel.Reserved;
        DstPixel.Blue += muldiv255 (Alpha, SrcPixel.Blue - DstPixel.Blue);
        DstPixel.Green += muldiv255 (Alpha, SrcPixel.Green - DstPixel.Green);
        DstPixel.Red += muldiv255 (Alpha, SrcPixel.Red - DstPixel.Red);
        ImageOut->Image.Bitmap[OffsetY2 + BltX + Xpos] = DstPixel;
      }
    }
    return EFI_SUCCESS;
  }

  for (Ypos = 0; Ypos < Height; Ypos++) {
    OffsetY1 = Width * Ypos;
    OffsetY2 = ImageOut->Width * (BltY + Ypos);
    for (Xpos = 0; Xpos < Width; Xpos++) {
      SrcPixel = BltBuffer[OffsetY1 + Xpos];
      if (Transparent) {
        if (CompareMem (&SrcPixel, &ZeroPixel, 3) != 0) {
          ImageOut->Image.Bitmap[OffsetY2 + BltX + Xpos] = SrcPixel;
        }
      } else {
        ImageOut->Image.Bitmap[OffsetY2 + BltX + Xpos] = SrcPixel;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 This function adds the image Image to the group of images owned by PackageList, and returns
 a new image identifier (ImageId).

 @param[in]  This               A pointer to the EFI_HII_IMAGE_PROTOCOL instance.
 @param[in]  PackageList        Handle of the package list where this image will
                                be added.
 @param[out] ImageId            On return, contains the new image id, which is
                                unique within PackageList.
 @param[in]  Image              Points to the image.

 @retval EFI_SUCCESS            The new image was added successfully.
 @retval EFI_NOT_FOUND          The specified PackageList could not be found in
                                database.
 @retval EFI_OUT_OF_RESOURCES   Could not add the image due to lack of resources.
 @retval EFI_INVALID_PARAMETER  Image is NULL or ImageId is NULL.
**/
EFI_STATUS
EFIAPI
HiiNewImage (
  IN  CONST EFI_HII_IMAGE_PROTOCOL   *This,
  IN  EFI_HII_HANDLE                 PackageList,
  OUT EFI_IMAGE_ID                   *ImageId,
  IN  CONST EFI_IMAGE_INPUT          *Image
  )
{
  HII_DATABASE_PRIVATE_DATA           *Private;
  LIST_ENTRY                          *Link;
  HII_DATABASE_RECORD                 *DatabaseRecord;
  HII_DATABASE_PACKAGE_LIST_INSTANCE  *PackageListNode;
  HII_IMAGE_PACKAGE_INSTANCE          *ImagePackage;
  UINT8                               *ImageBlock;
  UINTN                               BlockSize;
  UINT8                               *NewBlock;
  UINT8                               *NewBlockPtr;
  UINTN                               NewBlockSize;
  EFI_IMAGE_INPUT                     *ImageIn;

  if (This == NULL || ImageId == NULL || Image == NULL || Image->Bitmap == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsHiiHandleValid (PackageList)) {
    return EFI_NOT_FOUND;
  }

  Private = HII_IMAGE_DATABASE_PRIVATE_DATA_FROM_THIS (This);

  //
  // Get the specified package list
  //

  PackageListNode = NULL;

  for (Link = Private->DatabaseList.ForwardLink;
       Link != &Private->DatabaseList;
       Link = Link->ForwardLink
      ) {
    DatabaseRecord = CR (Link, HII_DATABASE_RECORD, DatabaseEntry, HII_DATABASE_RECORD_SIGNATURE);
    if (DatabaseRecord->Handle == PackageList) {
      PackageListNode = DatabaseRecord->PackageList;
      break;
    }
  }

  if (PackageListNode == NULL) {
    return EFI_NOT_FOUND;
  }

  ImageIn = (EFI_IMAGE_INPUT *) Image;

  NewBlockSize = sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) - sizeof (EFI_HII_RGB_PIXEL) +
                 BITMAP_LEN_24_BIT (ImageIn->Width, ImageIn->Height);

  //
  // Get the image package in the package list,
  // or create a new image package if image package does not exist.
  //
  if (PackageListNode->ImagePkg != NULL) {
    ImagePackage = PackageListNode->ImagePkg;

    //
    // Output the image id of the incoming image being inserted, which is the
    // image id of the EFI_HII_IIBT_END block of old image package.
    //
    *ImageId = 0;
    GetImageIdOrAddress (ImagePackage->ImageBlock, ImageId);

    //
    // Update the package's image block by appending the new block to the end.
    //
    BlockSize  = ImagePackage->ImageBlockSize + NewBlockSize;
    ImageBlock = (UINT8 *) AllocateZeroPool (BlockSize);
    if (ImageBlock == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Copy the original content.
    //
    CopyMem (
      ImageBlock,
      ImagePackage->ImageBlock,
      ImagePackage->ImageBlockSize - sizeof (EFI_HII_IIBT_END_BLOCK)
      );
    FreePool (ImagePackage->ImageBlock);
    ImagePackage->ImageBlock = ImageBlock;
    ImageBlock += ImagePackage->ImageBlockSize - sizeof (EFI_HII_IIBT_END_BLOCK);
    //
    // Temp memory to store new block.
    //
    NewBlock = AllocateZeroPool (NewBlockSize);
    if (NewBlock == NULL) {
      FreePool (ImagePackage->ImageBlock);
      return EFI_OUT_OF_RESOURCES;
    }
    NewBlockPtr = NewBlock;

    //
    // Update the length record.
    //
    ImagePackage->ImageBlockSize = (UINT32) BlockSize;
    ImagePackage->ImagePkgHdr.Header.Length += (UINT32) NewBlockSize;
    PackageListNode->PackageListHdr.PackageLength += (UINT32) NewBlockSize;

  } else {
    //
    // The specified package list does not contain image package.
    // Create one to add this image block.
    //
    ImagePackage = (HII_IMAGE_PACKAGE_INSTANCE *) AllocateZeroPool (sizeof (HII_IMAGE_PACKAGE_INSTANCE));
    if (ImagePackage == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Output the image id of the incoming image being inserted, which is the
    // first image block so that id is initially to one.
    //
    *ImageId = 1;
    BlockSize    = sizeof (EFI_HII_IIBT_END_BLOCK) + NewBlockSize;
    //
    // Fill in image package header.
    //
    ImagePackage->ImagePkgHdr.Header.Length     = (UINT32) BlockSize + sizeof (EFI_HII_IMAGE_PACKAGE_HDR);
    ImagePackage->ImagePkgHdr.Header.Type       = EFI_HII_PACKAGE_IMAGES;
    ImagePackage->ImagePkgHdr.ImageInfoOffset   = sizeof (EFI_HII_IMAGE_PACKAGE_HDR);
    ImagePackage->ImagePkgHdr.PaletteInfoOffset = 0;

    //
    // Fill in palette info.
    //
    ImagePackage->PaletteBlock    = NULL;
    ImagePackage->PaletteInfoSize = 0;

    //
    // Fill in image blocks.
    //
    ImagePackage->ImageBlockSize = (UINT32) BlockSize;
    ImagePackage->ImageBlock = (UINT8 *) AllocateZeroPool (BlockSize);
    if (ImagePackage->ImageBlock == NULL) {
      FreePool (ImagePackage);
      return EFI_OUT_OF_RESOURCES;
    }
    ImageBlock = ImagePackage->ImageBlock;

    //
    // Temp memory to store new block.
    //
    NewBlock = AllocateZeroPool (NewBlockSize);
    if (NewBlock == NULL) {
      FreePool (ImagePackage->ImageBlock);
      FreePool (ImagePackage);
      return EFI_OUT_OF_RESOURCES;
    }
    NewBlockPtr = NewBlock;

    //
    // Insert this image package.
    //
    PackageListNode->ImagePkg = ImagePackage;
    PackageListNode->PackageListHdr.PackageLength += ImagePackage->ImagePkgHdr.Header.Length;
  }

  //
  // Append the new block here
  //
  if (ImageIn->Flags == EFI_IMAGE_TRANSPARENT) {
    *NewBlock = EFI_HII_IIBT_IMAGE_24BIT_TRANS;
  } else {
    *NewBlock = EFI_HII_IIBT_IMAGE_24BIT;
  }
  NewBlock++;
  CopyMem (NewBlock, &ImageIn->Width, sizeof (UINT16));
  NewBlock += sizeof (UINT16);
  CopyMem (NewBlock, &ImageIn->Height, sizeof (UINT16));
  NewBlock += sizeof (UINT16);
  CopyGopToRgbPixel ((EFI_HII_RGB_PIXEL *) NewBlock, ImageIn->Bitmap, (UINTN) ImageIn->Width * ImageIn->Height);

  CopyMem (ImageBlock, NewBlockPtr, NewBlockSize);
  FreePool (NewBlockPtr);

  //
  // Append the block end
  //
  ImageBlock += NewBlockSize;
  ((EFI_HII_IIBT_END_BLOCK *) (ImageBlock))->Header.BlockType = EFI_HII_IIBT_END;

  return EFI_SUCCESS;
}

/**
 This function retrieves the image specified by ImageId which is associated with
 the specified PackageList and copies it into the buffer specified by Image.

 @param[in]  This               A pointer to the EFI_HII_IMAGE_PROTOCOL instance.
 @param[in]  PackageList        Handle of the package list where this image will
                                be searched.
 @param[in]  ImageId            The image's id,, which is unique within
                                PackageList.
 @param[out] Image              Points to the image.

 @retval EFI_SUCCESS            The new image was returned successfully.
 @retval EFI_NOT_FOUND          The image specified by ImageId is not in the database.
                                The specified PackageList is not in the database.
 @retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small to hold the image.
 @retval EFI_INVALID_PARAMETER  The Image or ImageSize was NULL.
 @retval EFI_OUT_OF_RESOURCES   The bitmap could not be retrieved because there was not enough memory.
**/
EFI_STATUS
EFIAPI
HiiGetImage (
  IN  CONST EFI_HII_IMAGE_PROTOCOL   *This,
  IN  EFI_HII_HANDLE                 PackageList,
  IN  EFI_IMAGE_ID                   ImageId,
  OUT EFI_IMAGE_INPUT                *Image
  )
{
  HII_DATABASE_PRIVATE_DATA           *Private;
  LIST_ENTRY                          *Link;
  HII_DATABASE_RECORD                 *DatabaseRecord;
  HII_DATABASE_PACKAGE_LIST_INSTANCE  *PackageListNode;
  HII_IMAGE_PACKAGE_INSTANCE          *ImagePackage;
  UINT8                               *ImageBlock;
  EFI_IMAGE_ID                        LocalImageId;
  UINT8                               BlockType;
  EFI_HII_IIBT_IMAGE_1BIT_BLOCK       Iibt1bit;
  UINT16                              Width;
  UINT16                              Height;
  UINTN                               ImageLength;
  BOOLEAN                             Flag;
  UINT8                               *PaletteInfo;
  UINT8                               PaletteIndex;
  UINT16                              PaletteSize;
  EFI_STATUS                          Status;
  EFI_JPEG_DECODER_STATUS             DecoderStatus;
  UINT8                               *Blt;
  UINTN                               BltSize;
  UINTN                               BltWidth;
  UINTN                               BltHeight;
  EFI_HII_IIBT_JPEG_BLOCK             *JpegBlock;
  EFI_HII_IIBT_PNG_BLOCK              *PngBlock;
  EFI_HII_IIBT_EXT4_BLOCK             *Ext4Block;
  H2O_HII_IIBT_IMAGE_32BIT_BLOCK      *Iibt32Bit;

  if (This == NULL || Image == NULL || ImageId < 1) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsHiiHandleValid (PackageList)) {
    return EFI_NOT_FOUND;
  }

  Private = HII_IMAGE_DATABASE_PRIVATE_DATA_FROM_THIS (This);

  //
  // Get the specified package list and image package.
  //
  PackageListNode = NULL;
  for (Link = Private->DatabaseList.ForwardLink;
       Link != &Private->DatabaseList;
       Link = Link->ForwardLink
      ) {
    DatabaseRecord = CR (Link, HII_DATABASE_RECORD, DatabaseEntry, HII_DATABASE_RECORD_SIGNATURE);
    if (DatabaseRecord->Handle == PackageList) {
      PackageListNode = DatabaseRecord->PackageList;
      break;
    }
  }
  if (PackageListNode == NULL) {
    return EFI_NOT_FOUND;
  }
  ImagePackage = PackageListNode->ImagePkg;
  if (ImagePackage == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Find the image block specified by ImageId
  //
  LocalImageId = ImageId;

  ImageBlock = GetImageIdOrAddress (ImagePackage->ImageBlock, &LocalImageId);
  if (ImageBlock == NULL) {
    return EFI_NOT_FOUND;
  }

  Flag      = FALSE;
  BlockType = *ImageBlock;
  ZeroMem (Image, sizeof(EFI_IMAGE_INPUT));

  switch (BlockType) {

  case EFI_HII_IIBT_EXT4:
    Ext4Block = (EFI_HII_IIBT_EXT4_BLOCK *) ImageBlock;
    if (Ext4Block->BlockType2 != H2O_HII_IIBT_IMAGE_32BIT) {
      return EFI_UNSUPPORTED;
    }

    Iibt32Bit = (H2O_HII_IIBT_IMAGE_32BIT_BLOCK *) ImageBlock;
    Width = Iibt32Bit->Width;
    Height = Iibt32Bit->Height;
    ImageLength = sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * ((UINTN) Width * Height);
    Image->Bitmap = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) AllocateCopyPool (ImageLength, Iibt32Bit->Bitmap);
    if (Image->Bitmap == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    Image->Flags = H2O_IMAGE_ALPHA_CHANNEL;
    Image->Width  = Width;
    Image->Height = Height;

    return EFI_SUCCESS;
    break;

  case EFI_HII_IIBT_IMAGE_JPEG:
    if (mJpegDecoder == NULL) {
      Status = gBS->LocateProtocol (
                      &gEfiJpegDecoderProtocolGuid,
                      NULL,
                      (VOID **)&mJpegDecoder
                      );
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }
    }

    Blt       = NULL;
    BltSize   = 0;
    JpegBlock = (EFI_HII_IIBT_JPEG_BLOCK *) ImageBlock;
    Status = mJpegDecoder->DecodeImage (
                             mJpegDecoder,
                             JpegBlock->Data,
                             JpegBlock->Size,
                             &Blt,
                             &BltSize,
                             &BltHeight,
                             &BltWidth,
                             &DecoderStatus
                             );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Image->Bitmap = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)Blt;
    Image->Width  = (UINT16)BltWidth;
    Image->Height = (UINT16)BltHeight;
    Image->Flags  = 0;
    return EFI_SUCCESS;
    break;

  case EFI_HII_IIBT_IMAGE_PNG:
    if (mPngDecoder == NULL) {
      Status = gBS->LocateProtocol (
                      &gEfiPngDecoderProtocolGuid,
                      NULL,
                      (VOID **)&mPngDecoder
                      );
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }
    }

    Blt      = NULL;
    BltSize  = 0;
    PngBlock = (EFI_HII_IIBT_PNG_BLOCK *) ImageBlock;
    Status = mPngDecoder->DecodeImage (
                            mPngDecoder,
                            PngBlock->Data,
                            PngBlock->Size,
                            &Blt,
                            &BltSize,
                            &BltHeight,
                            &BltWidth
                            );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Image->Bitmap = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)Blt;
    Image->Width  = (UINT16)BltWidth;
    Image->Height = (UINT16)BltHeight;
    Image->Flags  = 0;
    return EFI_SUCCESS;

  case EFI_HII_IIBT_IMAGE_1BIT_TRANS:
  case EFI_HII_IIBT_IMAGE_4BIT_TRANS:
  case EFI_HII_IIBT_IMAGE_8BIT_TRANS:
    Flag = TRUE;
    //
    // fall through
    //
  case EFI_HII_IIBT_IMAGE_1BIT:
  case EFI_HII_IIBT_IMAGE_4BIT:
  case EFI_HII_IIBT_IMAGE_8BIT:
    //
    // Use the common block code since the definition of these structures is the same.
    //
    CopyMem (&Iibt1bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK));
    ImageLength = sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) *
                  ((UINTN) Iibt1bit.Bitmap.Width * Iibt1bit.Bitmap.Height);
    Image->Bitmap = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) AllocateZeroPool (ImageLength);
    if (Image->Bitmap == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if (Flag) {
      Image->Flags = EFI_IMAGE_TRANSPARENT;
    }
    Image->Width  = Iibt1bit.Bitmap.Width;
    Image->Height = Iibt1bit.Bitmap.Height;

    PaletteInfo = ImagePackage->PaletteBlock + sizeof (EFI_HII_IMAGE_PALETTE_INFO_HEADER);
    for (PaletteIndex = 1; PaletteIndex < Iibt1bit.PaletteIndex; PaletteIndex++) {
      CopyMem (&PaletteSize, PaletteInfo, sizeof (UINT16));
      PaletteInfo += PaletteSize + sizeof (UINT16);
    }
    ASSERT (PaletteIndex == Iibt1bit.PaletteIndex);

    //
    // Output bitmap data
    //
    if (BlockType == EFI_HII_IIBT_IMAGE_1BIT || BlockType == EFI_HII_IIBT_IMAGE_1BIT_TRANS) {
      Status = Output1bitPixel (
                 Image,
                 (UINT8 *) (ImageBlock + sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK) - sizeof (UINT8)),
                 (EFI_HII_IMAGE_PALETTE_INFO *) PaletteInfo
                 );
    } else if (BlockType == EFI_HII_IIBT_IMAGE_4BIT || BlockType == EFI_HII_IIBT_IMAGE_4BIT_TRANS) {
      Status = Output4bitPixel (
                 Image,
                 (UINT8 *) (ImageBlock + sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK) - sizeof (UINT8)),
                 (EFI_HII_IMAGE_PALETTE_INFO *) PaletteInfo
                 );
    } else {
      Status =  Output8bitPixel (
                  Image,
                  (UINT8 *) (ImageBlock + sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK) - sizeof (UINT8)),
                  (EFI_HII_IMAGE_PALETTE_INFO *) PaletteInfo
                  );
    }

    return Status;
    break;

  case EFI_HII_IIBT_IMAGE_24BIT_TRANS:
    Flag = TRUE;
    //
    // fall through
    //
  case EFI_HII_IIBT_IMAGE_24BIT:
    CopyMem (&Width, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (UINT16));
    CopyMem (
      &Height,
      ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK) + sizeof (UINT16),
      sizeof (UINT16)
      );
    ImageLength = sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * ((UINTN) Width * Height);
    Image->Bitmap = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) AllocateZeroPool (ImageLength);
    if (Image->Bitmap == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if (Flag) {
      Image->Flags = EFI_IMAGE_TRANSPARENT;
    }
    Image->Width  = Width;
    Image->Height = Height;

    //
    // Output the bimap data directly.
    //
    Output24bitPixel (
      Image,
      (EFI_HII_RGB_PIXEL *) (ImageBlock + sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) - sizeof (EFI_HII_RGB_PIXEL))
      );
    return EFI_SUCCESS;
    break;

  default:
    return EFI_NOT_FOUND;
    break;
  }
}

/**
 This function updates the image specified by ImageId in the specified PackageListHandle to
 the image specified by Image.

 @param[in] This                A pointer to the EFI_HII_IMAGE_PROTOCOL instance.
 @param[in] PackageList         The package list containing the images.
 @param[in] ImageId             The image's id,, which is unique within PackageList.
 @param[in] Image               Points to the image.

 @retval EFI_SUCCESS            The new image was updated successfully.
 @retval EFI_NOT_FOUND          The image specified by ImageId is not in the database.
                                The specified PackageList is not in the database.
 @retval EFI_INVALID_PARAMETER  The Image was NULL.
**/
EFI_STATUS
EFIAPI
HiiSetImage (
  IN CONST EFI_HII_IMAGE_PROTOCOL    *This,
  IN EFI_HII_HANDLE                  PackageList,
  IN EFI_IMAGE_ID                    ImageId,
  IN CONST EFI_IMAGE_INPUT           *Image
  )
{
  HII_DATABASE_PRIVATE_DATA           *Private;
  LIST_ENTRY                          *Link;
  HII_DATABASE_RECORD                 *DatabaseRecord;
  HII_DATABASE_PACKAGE_LIST_INSTANCE  *PackageListNode;
  HII_IMAGE_PACKAGE_INSTANCE          *ImagePackage;
  UINT8                               *ImageBlock;
  EFI_IMAGE_ID                        LocalImageId;
  UINT8                               BlockType;
  EFI_HII_IIBT_IMAGE_1BIT_BLOCK       Iibt1bit;
  EFI_HII_IIBT_IMAGE_4BIT_BLOCK       Iibt4bit;
  EFI_HII_IIBT_IMAGE_8BIT_BLOCK       Iibt8bit;
  UINT16                              Width;
  UINT16                              Height;
  UINT32                              BlockSize;
  UINT32                              NewBlockSize;
  UINT32                              OldBlockSize;
  EFI_IMAGE_INPUT                     *ImageIn;
  UINT8                               *NewBlock;
  UINT8                               *NewBlockPtr;
  UINT8                               *Block;
  UINT8                               *BlockPtr;
  UINT32                               Part1Size;
  UINT32                               Part2Size;

  if (This == NULL || Image == NULL || ImageId < 1 || Image->Bitmap == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsHiiHandleValid (PackageList)) {
    return EFI_NOT_FOUND;
  }

  Private = HII_IMAGE_DATABASE_PRIVATE_DATA_FROM_THIS (This);

  //
  // Get the specified package list and image package.
  //
  PackageListNode = NULL;
  for (Link = Private->DatabaseList.ForwardLink;
       Link != &Private->DatabaseList;
       Link = Link->ForwardLink
      ) {
    DatabaseRecord = CR (Link, HII_DATABASE_RECORD, DatabaseEntry, HII_DATABASE_RECORD_SIGNATURE);
    if (DatabaseRecord->Handle == PackageList) {
      PackageListNode = DatabaseRecord->PackageList;
      break;
    }
  }
  if (PackageListNode == NULL) {
    return EFI_NOT_FOUND;
  }
  ImagePackage = PackageListNode->ImagePkg;
  if (ImagePackage == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Find the image block specified by ImageId
  //
  LocalImageId = ImageId;
  ImageBlock = GetImageIdOrAddress (ImagePackage->ImageBlock, &LocalImageId);
  if (ImageBlock == NULL) {
    return EFI_NOT_FOUND;
  }

  BlockType = *ImageBlock;

  //
  // Get the size of original image block. Use some common block code here
  // since the definition of some structures is the same.
  //
  switch (BlockType) {
  case EFI_HII_IIBT_IMAGE_JPEG:
    OldBlockSize = sizeof (EFI_HII_IIBT_JPEG_BLOCK) - sizeof (UINT8) +
                   ((EFI_HII_IIBT_JPEG_BLOCK *)ImageBlock)->Size;
    break;

  case EFI_HII_IIBT_IMAGE_PNG:
    OldBlockSize = sizeof (EFI_HII_IIBT_PNG_BLOCK) - sizeof (UINT8) +
                   ((EFI_HII_IIBT_PNG_BLOCK *)ImageBlock)->Size;
    break;

  case EFI_HII_IIBT_IMAGE_1BIT:
  case EFI_HII_IIBT_IMAGE_1BIT_TRANS:
    CopyMem (&Iibt1bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK));
    OldBlockSize = sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK) - sizeof (UINT8) +
                   BITMAP_LEN_1_BIT (Iibt1bit.Bitmap.Width, Iibt1bit.Bitmap.Height);
    break;
  case EFI_HII_IIBT_IMAGE_4BIT:
  case EFI_HII_IIBT_IMAGE_4BIT_TRANS:
    CopyMem (&Iibt4bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK));
    OldBlockSize = sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK) - sizeof (UINT8) +
                   BITMAP_LEN_4_BIT (Iibt4bit.Bitmap.Width, Iibt4bit.Bitmap.Height);
    break;
  case EFI_HII_IIBT_IMAGE_8BIT:
  case EFI_HII_IIBT_IMAGE_8BIT_TRANS:
    CopyMem (&Iibt8bit, ImageBlock, sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK));
    OldBlockSize = sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK) - sizeof (UINT8) +
                   BITMAP_LEN_8_BIT (Iibt8bit.Bitmap.Width, Iibt8bit.Bitmap.Height);
    break;
  case EFI_HII_IIBT_IMAGE_24BIT:
  case EFI_HII_IIBT_IMAGE_24BIT_TRANS:
    CopyMem (&Width, ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK), sizeof (UINT16));
    CopyMem (
      &Height,
      ImageBlock + sizeof (EFI_HII_IMAGE_BLOCK) + sizeof (UINT16),
      sizeof (UINT16)
      );
    OldBlockSize = sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) - sizeof (EFI_HII_RGB_PIXEL) +
                   BITMAP_LEN_24_BIT (Width , Height);
    break;
  default:
    return EFI_NOT_FOUND;
    break;
  }

  //
  // Create the new image block according to input image.
  //
  ImageIn = (EFI_IMAGE_INPUT *) Image;
  NewBlockSize = sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) - sizeof (EFI_HII_RGB_PIXEL) +
                 BITMAP_LEN_24_BIT (ImageIn->Width, ImageIn->Height);
  NewBlock = (UINT8 *) AllocateZeroPool (NewBlockSize);
  if (NewBlock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewBlockPtr = NewBlock;
  if ((ImageIn->Flags & EFI_IMAGE_TRANSPARENT) == EFI_IMAGE_TRANSPARENT) {
    *NewBlockPtr = EFI_HII_IIBT_IMAGE_24BIT_TRANS;
  } else {
    *NewBlockPtr = EFI_HII_IIBT_IMAGE_24BIT;
  }
  NewBlockPtr++;

  CopyMem (NewBlockPtr, &ImageIn->Width, sizeof (UINT16));
  NewBlockPtr += sizeof (UINT16);
  CopyMem (NewBlockPtr, &ImageIn->Height, sizeof (UINT16));
  NewBlockPtr += sizeof (UINT16);

  CopyGopToRgbPixel ((EFI_HII_RGB_PIXEL *) NewBlockPtr, ImageIn->Bitmap, (UINTN)ImageIn->Width * ImageIn->Height);

  //
  // Adjust the image package to remove the original block firstly then add the new block.
  //
  BlockSize = ImagePackage->ImageBlockSize + NewBlockSize - OldBlockSize;
  Block = (UINT8 *) AllocateZeroPool (BlockSize);
  if (Block == NULL) {
    FreePool (NewBlock);
    return EFI_OUT_OF_RESOURCES;
  }

  BlockPtr  = Block;
  Part1Size = (UINT32) (ImageBlock - ImagePackage->ImageBlock);
  Part2Size = ImagePackage->ImageBlockSize - Part1Size - OldBlockSize;
  CopyMem (BlockPtr, ImagePackage->ImageBlock, Part1Size);
  BlockPtr += Part1Size;
  CopyMem (BlockPtr, NewBlock, NewBlockSize);
  BlockPtr += NewBlockSize;
  CopyMem (BlockPtr, ImageBlock + OldBlockSize, Part2Size);

  FreePool (ImagePackage->ImageBlock);
  FreePool (NewBlock);
  ImagePackage->ImageBlock     = Block;
  ImagePackage->ImageBlockSize = BlockSize;
  ImagePackage->ImagePkgHdr.Header.Length += NewBlockSize - OldBlockSize;
  PackageListNode->PackageListHdr.PackageLength += NewBlockSize - OldBlockSize;

  return EFI_SUCCESS;

}

#define muldiv255(a,b)		((unsigned char)((((a)+1)*(b))>>8))		/* very fast, 92% accurate*/

/**
 This function renders an image to a bitmap or the screen using the specified
 color and options. It draws the image on an existing bitmap, allocates a new
 bitmap or uses the screen. The images can be clipped.

 @param[in]      This           A pointer to the EFI_HII_IMAGE_PROTOCOL instance.
 @param[in]      Flags          Describes how the image is to be drawn.
 @param[in]      Image          Points to the image to be displayed.
 @param[in, out] Blt            If this points to a non-NULL on entry, this points
                                to the image, which is Width pixels wide and
                                Height pixels high.  The image will be drawn onto
                                this image and  EFI_HII_DRAW_FLAG_CLIP is implied.
                                If this points to a  NULL on entry, then a buffer
                                will be allocated to hold  the generated image and
                                the pointer updated on exit. It is the caller's
                                responsibility to free this buffer.
 @param[in]      BltX           Specifies the offset from the left and top edge of
                                the  output image of the first pixel in the image.
 @param[in]      BltY           Specifies the offset from the left and top edge of
                                the  output image of the first pixel in the image.

 @retval EFI_SUCCESS            The image was successfully drawn.
 @retval EFI_OUT_OF_RESOURCES   Unable to allocate an output buffer for Blt.
 @retval EFI_INVALID_PARAMETER  The Image or Blt was NULL.
 @retval EFI_INVALID_PARAMETER  Any combination of Flags is invalid.
**/
EFI_STATUS
EFIAPI
HiiDrawImage (
  IN CONST EFI_HII_IMAGE_PROTOCOL    *This,
  IN EFI_HII_DRAW_FLAGS              Flags,
  IN CONST EFI_IMAGE_INPUT           *Image,
  IN OUT EFI_IMAGE_OUTPUT            **Blt,
  IN UINTN                           BltX,
  IN UINTN                           BltY
  )
{
  EFI_STATUS                          Status;
  HII_DATABASE_PRIVATE_DATA           *Private;
  BOOLEAN                             Transparent;
  EFI_IMAGE_INPUT                     *ImageIn;
  EFI_IMAGE_OUTPUT                    *ImageOut;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *BltBuffer;
  UINTN                               BufferLen;
  UINTN                               Width;
  UINTN                               Height;
  UINTN                               Xpos;
  UINTN                               Ypos;
  UINTN                               OffsetY1;
  UINTN                               OffsetY2;
  EFI_FONT_DISPLAY_INFO               *FontInfo;
  UINTN                               Index;
  BOOLEAN                             AlphaChannel;
  UINT8                               Alpha;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *Src;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *Dst;


  if (This == NULL || Image == NULL || Blt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Flags & EFI_HII_DRAW_FLAG_CLIP) == EFI_HII_DRAW_FLAG_CLIP && *Blt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Flags & EFI_HII_DRAW_FLAG_TRANSPARENT) == EFI_HII_DRAW_FLAG_TRANSPARENT) {
    return EFI_INVALID_PARAMETER;
  }

  ImageIn = (EFI_IMAGE_INPUT *) Image;

  //
  // Check whether the image will be drawn transparently or opaquely.
  //
  Transparent = FALSE;
  if ((Flags & EFI_HII_DRAW_FLAG_TRANSPARENT) == EFI_HII_DRAW_FLAG_FORCE_TRANS) {
    Transparent = TRUE;
  } else if ((Flags & EFI_HII_DRAW_FLAG_TRANSPARENT) == EFI_HII_DRAW_FLAG_FORCE_OPAQUE){
    Transparent = FALSE;
  } else {
    //
    // Now EFI_HII_DRAW_FLAG_DEFAULT is set, whether image will be drawn depending
    // on the image's transparency setting.
    //
    if ((ImageIn->Flags & EFI_IMAGE_TRANSPARENT) == EFI_IMAGE_TRANSPARENT) {
      Transparent = TRUE;
    }
  }

  //
  // if EFI_HII_DRAW_FLAG_FORCE_OPAQUE is set in Flags,
  // it sill need processing alpha channel
  //
  AlphaChannel = FALSE;
  if ((ImageIn->Flags & H2O_IMAGE_ALPHA_CHANNEL) == H2O_IMAGE_ALPHA_CHANNEL) {
    AlphaChannel = TRUE;
  }

  //
  // Image cannot be drawn transparently if Blt points to NULL on entry.
  // Currently output to Screen transparently is not supported, either.
  //
  if (Transparent) {
    if (*Blt == NULL) {
      return EFI_INVALID_PARAMETER;
    }
  }

  Private = HII_IMAGE_DATABASE_PRIVATE_DATA_FROM_THIS (This);

  //
  // When Blt points to a non-NULL on entry, this image will be drawn onto
  // this bitmap or screen pointed by "*Blt" and EFI_HII_DRAW_FLAG_CLIP is implied.
  // Otherwise a new bitmap will be allocated to hold this image.
  //
  if (*Blt != NULL) {
    //
    // Clip the image by (Width, Height)
    //

    Width  = ImageIn->Width;
    Height = ImageIn->Height;

    if (Width > (*Blt)->Width - BltX) {
      Width = (*Blt)->Width - BltX;
    }
    if (Height > (*Blt)->Height - BltY) {
      Height = (*Blt)->Height - BltY;
    }

    BufferLen = Width * Height * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) AllocateZeroPool (BufferLen);
    if (BltBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if ((AlphaChannel || Transparent)  && ((Flags & EFI_HII_DIRECT_TO_SCREEN) == EFI_HII_DIRECT_TO_SCREEN)) {
      //
      // Read image from screen
      //
      Status = (*Blt)->Image.Screen->Blt (
                                       (*Blt)->Image.Screen,
                                       BltBuffer,
                                       EfiBltVideoToBltBuffer,
                                       BltX,
                                       BltY,
                                       0,
                                       0,
                                       Width,
                                       Height,
                                       0
                                       );
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }

    if (AlphaChannel) {
      for (Ypos = 0; Ypos < Height; Ypos++) {
        OffsetY1 = ImageIn->Width * Ypos;
        OffsetY2 = Width * Ypos;
        for (Xpos = 0; Xpos < Width; Xpos++) {
          Src = &ImageIn->Bitmap[OffsetY1 + Xpos];
          Dst = &BltBuffer[OffsetY2 + Xpos];

          Alpha = Src->Reserved;
          Dst->Blue  += muldiv255 (Alpha, Src->Blue  - Dst->Blue);
          Dst->Green += muldiv255 (Alpha, Src->Green - Dst->Green);
          Dst->Red   += muldiv255 (Alpha, Src->Red   - Dst->Red);
        }
      }
    } else {
      if (Width == ImageIn->Width && Height == ImageIn->Height) {
        CopyMem (BltBuffer, ImageIn->Bitmap, BufferLen);
      } else {
        for (Ypos = 0; Ypos < Height; Ypos++) {
          OffsetY1 = ImageIn->Width * Ypos;
          OffsetY2 = Width * Ypos;
          for (Xpos = 0; Xpos < Width; Xpos++) {
            BltBuffer[OffsetY2 + Xpos] = ImageIn->Bitmap[OffsetY1 + Xpos];
          }
        }
      }
    }

    //
    // Draw the image to existing bitmap or screen depending on flag.
    //
    if ((Flags & EFI_HII_DIRECT_TO_SCREEN) == EFI_HII_DIRECT_TO_SCREEN) {
      //
      // Caller should make sure the current UGA console is grarphic mode.
      //

      //
      // Write the image directly to the output device specified by Screen.
      //
      Status = (*Blt)->Image.Screen->Blt (
                                       (*Blt)->Image.Screen,
                                       BltBuffer,
                                       EfiBltBufferToVideo,
                                       0,
                                       0,
                                       BltX,
                                       BltY,
                                       Width,
                                       Height,
                                       0
                                       );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      //
      // Draw the image onto the existing bitmap specified by Bitmap.
      //
      Status = ImageToBlt (
                 BltBuffer,
                 BltX,
                 BltY,
                 Width,
                 Height,
                 Transparent,
                 AlphaChannel,
                 Blt
                 );

    }

    FreePool (BltBuffer);
    return Status;

  } else {
    //
    // Allocate a new bitmap to hold the incoming image.
    //
    Width  = ImageIn->Width  + BltX;
    Height = ImageIn->Height + BltY;

    BufferLen = Width * Height * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    BltBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) AllocateZeroPool (BufferLen);
    if (BltBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    ImageOut = (EFI_IMAGE_OUTPUT *) AllocateZeroPool (sizeof (EFI_IMAGE_OUTPUT));
    if (ImageOut == NULL) {
      FreePool (BltBuffer);
      return EFI_OUT_OF_RESOURCES;
    }
    ImageOut->Width        = (UINT16) Width;
    ImageOut->Height       = (UINT16) Height;
    ImageOut->Image.Bitmap = BltBuffer;

    //
    // BUGBUG: Now all the "blank" pixels are filled with system default background
    // color. Not sure if it need to be updated or not.
    //
    Status = GetSystemFont (Private, &FontInfo, NULL);
    if (EFI_ERROR (Status)) {
      FreePool (BltBuffer);
      FreePool (ImageOut);
      return Status;
    }
    for (Index = 0; Index < Width * Height; Index++) {
      BltBuffer[Index] = FontInfo->BackgroundColor;
    }
    FreePool (FontInfo);

    //
    // Draw the incoming image to the new created image.
    //
    *Blt = ImageOut;
    return ImageToBlt (
             ImageIn->Bitmap,
             BltX,
             BltY,
             ImageIn->Width,
             ImageIn->Height,
             Transparent,
             AlphaChannel,
             Blt
             );

  }
}

/**
 This function renders an image to a bitmap or the screen using the specified
 color and options. It draws the image on an existing bitmap, allocates a new
 bitmap or uses the screen. The images can be clipped.

 @param[in]      This           A pointer to the EFI_HII_IMAGE_PROTOCOL instance.
 @param[in]      Flags          Describes how the image is to be drawn.
 @param[in]      PackageList    The package list in the HII database to search for
                                the  specified image.
 @param[in]      ImageId        The image's id, which is unique within
                                PackageList.
 @param[in, out] Blt            If this points to a non-NULL on entry, this points
                                to the image, which is Width pixels wide and
                                Height pixels high. The image will be drawn onto
                                this image and
                                EFI_HII_DRAW_FLAG_CLIP is implied. If this points
                                to a  NULL on entry, then a buffer will be
                                allocated to hold  the generated image and the
                                pointer updated on exit. It is the caller's
                                responsibility to free this buffer.
 @param[in]  BltX               Specifies the offset from the left and top edge of
                                the  output image of the first pixel in the image.
 @param[in]  BltY               Specifies the offset from the left and top edge of
                                the  output image of the first pixel in the image.

 @retval EFI_SUCCESS            The image was successfully drawn.
 @retval EFI_OUT_OF_RESOURCES   Unable to allocate an output buffer for Blt.
 @retval EFI_INVALID_PARAMETER  The Blt was NULL.
 @retval EFI_NOT_FOUND          The image specified by ImageId is not in the database.
                                The specified PackageList is not in the database.
**/
EFI_STATUS
EFIAPI
HiiDrawImageId (
  IN CONST EFI_HII_IMAGE_PROTOCOL    *This,
  IN EFI_HII_DRAW_FLAGS              Flags,
  IN EFI_HII_HANDLE                  PackageList,
  IN EFI_IMAGE_ID                    ImageId,
  IN OUT EFI_IMAGE_OUTPUT            **Blt,
  IN UINTN                           BltX,
  IN UINTN                           BltY
  )
{
  EFI_STATUS                          Status;
  EFI_IMAGE_INPUT                     Image;

  //
  // Check input parameter.
  //
  if (This == NULL || Blt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsHiiHandleValid (PackageList)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get the specified Image.
  //
  Status = HiiGetImage (This, PackageList, ImageId, &Image);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Draw this image.
  //
  Status = HiiDrawImage (This, Flags, &Image, Blt, BltX, BltY);
  if (Image.Bitmap != NULL) {
    FreePool (Image.Bitmap);
  }
  return Status;
}

/**
 Insyde multi board image package extension.

 A destructor function of H2O_HII_BOARD_IMAGE_BLOCK_LIST.
 This function is called when list H2O_HII_BOARD_IMAGE_BLOCK_LIST is nolonger required.
 This function traverse whole data structure of input list, free all the allocated memory.
 If total "malloc" count greater than "free" count, it means mamory leak.
 This is an internal function.

 @param[in, out]     List  A pointer to the first entry of abstract data type
                     that built in heap.
**/
VOID
BoardImageTableListDestructor (
  IN OUT H2O_HII_IMAGE_TABLE_ENTRY  *List
  )
{
  LIST_ENTRY                 *Entry;
  LIST_ENTRY                 *Node;
  H2O_HII_IMAGE_TABLE_ENTRY  *CurrentImageTableEntry;
  H2O_HII_IMAGE_BLOCK_NODE   *CurrentImageBlockNode;

  if (List != NULL) {
    while (!IsListEmpty (&List->TableEntry)) {
      Entry = List->TableEntry.ForwardLink;
      CurrentImageTableEntry = GET_H2O_HII_IMAGE_TABLE_ENTRY_FROM_THIS (Entry);
      if (!CurrentImageTableEntry->IsSlaveEntry && \
           CurrentImageTableEntry->AssociateImageBlockList != NULL) {
        //
        //  Free current AssociateImageBlockList.
        //
        while (!IsListEmpty (CurrentImageTableEntry->AssociateImageBlockList)) {
          Node = CurrentImageTableEntry->AssociateImageBlockList->ForwardLink;
          CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (Node);
          RemoveEntryList (&CurrentImageBlockNode->ImageBlockNodeEntry);
          ZeroMem ((VOID *)CurrentImageBlockNode, sizeof (H2O_HII_IMAGE_BLOCK_NODE));
          FreePool (CurrentImageBlockNode);
        }
        CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (CurrentImageTableEntry->AssociateImageBlockList);
        ZeroMem ((VOID *)CurrentImageBlockNode, sizeof (H2O_HII_IMAGE_BLOCK_NODE));
        FreePool (CurrentImageBlockNode);
      }
      RemoveEntryList (&CurrentImageTableEntry->TableEntry);
      ZeroMem ((VOID *)CurrentImageTableEntry, sizeof (H2O_HII_IMAGE_TABLE_ENTRY));
      FreePool (CurrentImageTableEntry);
    }
    //
    //  The last remaining image table entry should be destroyed too.
    //
    CurrentImageTableEntry = List;
    //
    //  The AssociateImageBlockList of this last remaining entry should be destroyed too.
    //
    if (!CurrentImageTableEntry->IsSlaveEntry && \
         CurrentImageTableEntry->AssociateImageBlockList != NULL) {
      while (!IsListEmpty (CurrentImageTableEntry->AssociateImageBlockList)) {
        Node = CurrentImageTableEntry->AssociateImageBlockList->ForwardLink;
        CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (Node);
        RemoveEntryList (&CurrentImageBlockNode->ImageBlockNodeEntry);
        ZeroMem ((VOID *)CurrentImageBlockNode, sizeof (H2O_HII_IMAGE_BLOCK_NODE));
        FreePool (CurrentImageBlockNode);
      }
      CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (CurrentImageTableEntry->AssociateImageBlockList);
      ZeroMem (CurrentImageBlockNode, sizeof (H2O_HII_IMAGE_BLOCK_NODE));
      FreePool (CurrentImageBlockNode);
    }
    ZeroMem ((VOID *)CurrentImageTableEntry, sizeof (H2O_HII_IMAGE_TABLE_ENTRY));
    FreePool (CurrentImageTableEntry);
  }

}

/**
 Insyde multi board image package extension.

 Search within image table entries using board id as key to check if corresponding entry exsists.

 This is an internal function.

 @param[in]      List      A pointer to the first entry.
                           It also means caller want to search within this list.
 @param[in]      BoardId   The value indicate that which board user want to check.
 @param[out]     Entry     Update a pointer to corresponding entry if match found,
                           or NULL if target not exist.

 @retval         EFI_SUCCESS            Find a match and successfully updated a pointer to entry.
 @retval         EFI_INVALID_PARAMETER  List is NULL or Entry is NULL in your args.
 @retval         EFI_NOT_FOUND          Entry not found or get failed.

**/
EFI_STATUS
SelectImageEntryWorker (
  IN  H2O_HII_IMAGE_TABLE_ENTRY      *List,
  IN  H2O_BOARD_ID                   BoardId,
  OUT H2O_HII_IMAGE_TABLE_ENTRY      **Entry
  )
{
  H2O_HII_IMAGE_TABLE_ENTRY            *CurrentImageTableEntry;
  LIST_ENTRY                           *Link;
  LIST_ENTRY                           *FirstTableEntry;

  CurrentImageTableEntry = NULL;

  //
  //  If List is NULL, it means no entry has been created.
  //
  if (List != NULL) {
    //
    //  Check first image table entry.
    //  (H2O_HII_BOARD_IMAGE_BLOCK_LIST.ImageTableEntry point to first H2O_HII_IMAGE_TABLE_ENTRY)
    //
    if (List->ThisBoard == BoardId) {
      if (Entry != NULL) {
        *Entry = List;
      }
      return EFI_SUCCESS;
    }
    //
    //  Check the others.
    //
    FirstTableEntry = &List->TableEntry;
    for (Link = FirstTableEntry->ForwardLink; Link != FirstTableEntry; Link = Link->ForwardLink) {
      CurrentImageTableEntry = GET_H2O_HII_IMAGE_TABLE_ENTRY_FROM_THIS (Link);
      if (CurrentImageTableEntry->ThisBoard == BoardId) {
        if (Entry != NULL) {
          *Entry = CurrentImageTableEntry;
        }
        return EFI_SUCCESS;
      }
    }
  }

  return EFI_NOT_FOUND;
}

/**
 Insyde multi board image package extension.

 Select a entry in input list ADT using board id as key.
 Create a node and append it to the specified entry.
 This entry should be master who has the authority to control the AssociateImageBlockList.
 This is an internal function.

 @param[in, out]   List                    A pointer to the first entry.
 @param[in]        Length                  The size of corresponding block in source package.
 @param[in]        Type                    The type of corresponding block in source package.
 @param[in]        ImageBlockHeader        The address of corresponding block in source package.
 @param[in]        ImageInstance           The address of image of corresponding block in
                                           source package. NULL is valid if this block contains
                                           no image.
 @param[in]        VirtualImageId          The image id in whole package view.
 @param[in]        BoardId                 The board id to specify which entry to append.

 @retval           EFI_SUCCESS             Image block node is created and successfully
                                           appended to specified board entry.
 @retval           EFI_INVALID_PARAMETER   List is NULL or ImageBlockHeader is NULL or
                                           ImageInstance is NULL.
                                           The entry found by board id is not a master entry.

 @retval           EFI_OUT_OF_RESOURCES    Unable to allocate memory for new node.

**/
EFI_STATUS
AppendImageBlockWorker (
  IN OUT H2O_HII_IMAGE_TABLE_ENTRY      *List,
  IN     UINT32                         Length,
  IN     UINT8                          Type,
  IN     UINT8                          *ImageBlockHeader,
  IN     UINT8                          *ImageInstance,
  IN     EFI_IMAGE_ID                   VirtualImageId,
  IN     H2O_BOARD_ID                   BoardId
  )
{
  EFI_STATUS                            Status;
  H2O_HII_IMAGE_TABLE_ENTRY             *CurrentImageTableEntry;
  H2O_HII_IMAGE_BLOCK_NODE              *Node;

  if (List == NULL || ImageBlockHeader == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  CurrentImageTableEntry = NULL;
  Node = NULL;

  Status = SelectImageEntryWorker (
             List,
             BoardId,
             &CurrentImageTableEntry
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Only the master entries are allowed to hold image block list.
  //  It means image block node can only be appended to master entry.
  //  Appending a node to slave entry could be a violation of this method.
  //
  if (CurrentImageTableEntry->IsSlaveEntry == TRUE) {
    return EFI_INVALID_PARAMETER;
  }

  Node = AllocateZeroPool (sizeof (H2O_HII_IMAGE_BLOCK_NODE));
  if (Node == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Node->Signature = H2O_HII_IMAGE_BLOCK_NODE_SIGNATURE;
  Node->ImageBlockSize      = Length;
  Node->ImageBlockType      = Type;
  Node->ImageBlock          = ImageBlockHeader;
  Node->ImageInstance       = ImageInstance;
  Node->VirtualImageId      = VirtualImageId;
  InitializeListHead (&Node->ImageBlockNodeEntry);
  if (CurrentImageTableEntry->AssociateImageBlockList == NULL) {
    //
    //  Make sure this is the first node appended to this entry.
    //
    if (CurrentImageTableEntry->ImageBlockListNodeCount != 0) {
      return EFI_INVALID_PARAMETER;
    }
    CurrentImageTableEntry->AssociateImageBlockList = &Node->ImageBlockNodeEntry;
    CurrentImageTableEntry->ImageBlockListNodeCount += 1;
  } else {
    InsertTailList (CurrentImageTableEntry->AssociateImageBlockList, &Node->ImageBlockNodeEntry);
    CurrentImageTableEntry->ImageBlockListNodeCount += 1;
  }

  return EFI_SUCCESS;
}

/**
  Insyde multi board image package extension.

  This function helps create entry instances, appends it to prior existed entry
  when first entry is created.
  This is an internal function.

  @param[in, out]    List           The address of the pointer to the first entry.
                                    When it is the first time creating this list,
                                    the point will be updated with the addr of the first entry.
  @param[in]         BoardId        The board id indicate that which board this new entry belongs to.
  @param[in]         VirtualImageId The image id in whole package view.
  @param[out]        NewEntry       Update a pointer to new created entry.

  @retval            EFI_SUCCESS           New entry created successfully.
  @retval            EFI_OUT_OF_RESOURCES  Unable to allocate resource for new entry.

**/
EFI_STATUS
AddImageEntryWorker (
  IN OUT H2O_HII_IMAGE_TABLE_ENTRY      **List,
  IN     H2O_BOARD_ID                   BoardId,
  IN     EFI_IMAGE_ID                   VirtualImageId,
  OUT    H2O_HII_IMAGE_TABLE_ENTRY      **NewEntry
  )
{
  H2O_HII_IMAGE_TABLE_ENTRY             *ImageTableEntry;
  H2O_HII_IMAGE_TABLE_ENTRY             *PriorImageTableEntry;

  ImageTableEntry = NULL;
  PriorImageTableEntry = NULL;

  ImageTableEntry = AllocateZeroPool (sizeof (H2O_HII_IMAGE_TABLE_ENTRY));
  if (ImageTableEntry == NULL) {
    *NewEntry = NULL;
    return EFI_OUT_OF_RESOURCES;
  }

  //
  //  Initialize this new entry.
  //
  ImageTableEntry->Signature               = H2O_HII_IMAGE_TABLE_ENTRY_SIGNATURE;
  ImageTableEntry->ImageBlockListNodeCount = 0;
  ImageTableEntry->AssociateImageBlockList = NULL;
  InitializeListHead (&ImageTableEntry->TableEntry);
  ImageTableEntry->ThisBoard               = BoardId;
  ImageTableEntry->IsSlaveEntry            = TRUE;

  //
  //  Output a pointer to this new entry.
  //
  *NewEntry = ImageTableEntry;

  //
  //  ImageTableEntry will point to the first created entry,
  //  the others should be appendded one by one follow the first entry.
  //
  if (*List == NULL) {
    *List = ImageTableEntry;
  } else {
    PriorImageTableEntry = *List;
    InsertTailList (&PriorImageTableEntry->TableEntry, &ImageTableEntry->TableEntry);
  }

  DEBUG ((EFI_D_INFO, "--------New entry created--------[Board ID : 0x%02X]\n", ImageTableEntry->ThisBoard));

  return EFI_SUCCESS;
}


/**
 Insyde multi board image package extension.

 This function classify all info provided by a image block,
 Entry and node are created and placed in correct place base on these info.
 This is an internal function.

   "*ImageTableEntry"        "*AssociateImageBlockList"
 +----------------------+
 \Entry(default 0x00)    >--[node]-[node]-...
 +----------------------+
            \
 +----------------------+
 \Entry(URvp7   0x0B)    >--[node]-[node]-...
 +----------------------+
            \
 +----------------------+
 \Entry(A0Rvp3  0x04)    >--[node]-[node]-...
 +----------------------+      \
            \                  \ (Co-use)
 +----------------------+      \
 \Entry(ULpddr3Rvp5 0x0A)>-----+
 +----------------------+

 @param[in, out] List                 The address of the pointer to the first entry,
                                      all the entries and nodes will be added under this list.
 @param[in]      Length               The size of corresponding block in source package.
 @param[in]      Type                 The type of corresponding block in source package.
 @param[in]      ImageBlockHeader     The address of corresponding block in source package.
 @param[in]      ImageInstance        The address of image of corresponding block in
                                      source package. NULL is valid if this block contains
                                      no image.
 @param[in]      AssociateBoardsBlock The address of corresponding boards block that tells
                                      what boards this block belongs to.
 @param[in]      VirtualImageId       The image id in whole package view.

 @retval         EFI_SUCCESS      Input info of a specific image block classified successfully.
 @retval         EFI_UNSUPPORTED  The data AssociateBoardsBlock point to is incorrect.
                                  Any other wrong input data cause incorrect data structure created.

**/
EFI_STATUS
ClassifyImageBlockWorker (
  IN OUT H2O_HII_IMAGE_TABLE_ENTRY      **List,
  IN     UINT32                         Length,
  IN     UINT8                          Type,
  IN     UINT8                          *ImageBlockHeader,
  IN     UINT8                          *ImageInstance,
  IN     H2O_HII_IIBT_BOARDS_BLOCK      *AssociateBoardsBlock,
  IN     EFI_IMAGE_ID                   VirtualImageId
  )
{
  EFI_STATUS                            Status;
  H2O_HII_IMAGE_TABLE_ENTRY             *CurrentImageTableEntry;
  H2O_HII_IMAGE_TABLE_ENTRY             *MasterImageTableEntry;
  UINT32                                Count;
  UINT32                                Index;
  H2O_BOARD_ID                          CurrentBoardId;
  H2O_BOARD_ID                          MasterBoardId;
  LIST_ENTRY                            *OriginalImageBlockList;

  Status = EFI_SUCCESS;
  CurrentImageTableEntry = NULL;
  MasterImageTableEntry = NULL;

  //
  //  typedef struct _H2O_HII_IIBT_BOARDS_BLOCK {
  //    EFI_HII_IIBT_EXT1_BLOCK  Hdr;
  //    EFI_IMAGE_ID             ImageId;
  //    H2O_BOARD_ID             Boards[...];
  //  } H2O_HII_IIBT_BOARDS_BLOCK;
  //
  //  Calculate length of Boards[...].
  //
  Count = AssociateBoardsBlock->Hdr.Length - sizeof (EFI_HII_IIBT_EXT1_BLOCK) - sizeof (EFI_IMAGE_ID);

  //
  // If Count >= sizeof(H2O_BOARD_ID), it means AssociateBoardsBlock is the H2O_HII_IIBT_BOARDS_BLOCK.
  // It needs to calculate the Boards number.
  //
  Count = (Count >= sizeof (H2O_BOARD_ID)) ? (Count / sizeof (H2O_BOARD_ID)):Count;

  //
  //  Iterate all associate board id of this current block.
  //
  for (Index = 0; Index < Count; Index++) {

    CurrentBoardId = AssociateBoardsBlock->Boards[Index];
    //
    //  Each entry only created once.
    //
    if (SelectImageEntryWorker (*List, CurrentBoardId, NULL) == EFI_NOT_FOUND) {
      //
      //  ImageTableEntry will point to first created entry,
      //  the others should append to the first entry one by one.
      //
      Status = AddImageEntryWorker (
                 List,
                 CurrentBoardId,
                 VirtualImageId,
                 &CurrentImageTableEntry
                 );
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }
  //
  //  Image block should be appended to only one entry, the one is master.
  //  The rest of entries who co-use AssociateImageBlockList with master are slaves.
  //

  //
  //  NOTE :
  //  All boards within a same associate boards block co-use AssociateImageBlockList.
  //  (Their images are identical)
  //  Only one entry of them needs to hold AssociateImageBlockList,
  //  so let first board be the master entry, the others are slaves point to the master.
  //  Or we can say, they share the same AssociateImageBlockList.
  //

  //
  //  Set first board as master.
  //
  MasterBoardId = AssociateBoardsBlock->Boards[0];
  //
  //  Find target entry by key(board-id).
  //
  Status = SelectImageEntryWorker (
             *List,
             MasterBoardId,
             &MasterImageTableEntry
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  //  Change entry's identity to master first.
  //  Because we only allow master entry to control AssociateImageBlockList.
  //
  MasterImageTableEntry->IsSlaveEntry = FALSE;
  //
  //  Let master entry holds this block.
  //  Specify a board id, use this id as key to find entry, then append
  //  a image block node to this entry.
  //
  Status = AppendImageBlockWorker (
             *List,
             Length,
             Type,
             ImageBlockHeader,
             ImageInstance,
             VirtualImageId,
             MasterBoardId
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Set pointer of friend entries to master.
  //  This procedure runs only when this boards block
  //  contains more then one board(Count greater then 1).
  //  Index 0 is master.
  //
  //  if CurrentSection.TotoalIdCound > 1
  //    for Id = CurrentSection.SecondId to CurrentSection.LastId
  //      ListObject.ImageEntry[Id].ImageBlockList = ListObject.ImageEntry[CurrentSection.FirstId].ImageBlockList
  //
  for (Index = 1; Index < Count; Index++) {
    CurrentBoardId = AssociateBoardsBlock->Boards[Index];
    Status = SelectImageEntryWorker (
               *List,
               CurrentBoardId,
               &CurrentImageTableEntry
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    OriginalImageBlockList = CurrentImageTableEntry->AssociateImageBlockList;
    //
    //  If "AssociateImageBlockList" of this entry is null, then this is entry is ceated first time.
    //  Setup where it should point to. If it is not NULL (Allready setup), check its correctness.
    //  If the pointer is changed, it could be a violation from caller.
    //
    if (OriginalImageBlockList == NULL) {
      //
      //  Make slave entry co use "AssociateImageBlockList" with master entry.
      //
      CurrentImageTableEntry->AssociateImageBlockList = MasterImageTableEntry->AssociateImageBlockList;
    } else if (OriginalImageBlockList != MasterImageTableEntry->AssociateImageBlockList) {
      //
      //  Fatal Error!
      //  Should not change pointer when its association is set up.
      //  Redirection of pointer in any non-NULL entry means boardid
      //  is duplicated between boards blocks.
      //
      return EFI_UNSUPPORTED;
    }
    //
    //  Finally, sync ImageBlockListNodeCount of current slave entry with master.
    //
    CurrentImageTableEntry->ImageBlockListNodeCount = MasterImageTableEntry->ImageBlockListNodeCount;
  }

  return Status;
}

/**
 Insyde multi board image package extension.

 Find out AssociateImageBlockList specified by board id, and restore its content.

 Use board id to select image table entry, then get image block list of this entry,
 interate nodes of this list.
 Each node contains pointers that tells the information it is mapping to.
 If the node is a duplicate of other boards, restore info of this node.
 If the node is a duplicate of local board(within same board), node will not be restored.
 Finally, updated the pre-calculated packagesize.
 The restored image block list can be used to build up a image package.
 This is an internal function.

 @param[in, out]    List            A pointer to the first entry,
                                    Funciotn will traverse data struture from this entry.
 @param[in]         BoardId         A board id to specify which package to restore.
 @param[out]        PackageSize     Output length of image package data for this board.
                                    This length does not include package end block.

 @retval            EFI_SUCCESS            The specified image block list is successfully restored.
 @retval            EFI_INVALID_PARAMETER  Argument List is NULL or PackageSize is NULL.
 @retval            EFI_UNSUPPORTED        The PackageSize calculation is not correct.

**/
EFI_STATUS
RestoreImageBlockWorker (
  IN OUT H2O_HII_IMAGE_TABLE_ENTRY      *List,
  IN     H2O_BOARD_ID                   BoardId,
  OUT    UINT32                         *PackageSize
  )
{
  EFI_STATUS                            Status;
  H2O_HII_IMAGE_TABLE_ENTRY             *TargetImageTableEntry;
  H2O_HII_IMAGE_BLOCK_NODE              *CurrentImageBlockNode;
  LIST_ENTRY                            *Link;
  LIST_ENTRY                            *SourceEntry;
  LIST_ENTRY                            *SourceNode;
  INT32                                 Index;
  INT32                                 NodeIndex;
  BOOLEAN                               IsDuplicateBlockRestored;
  H2O_HII_IMAGE_TABLE_ENTRY             *SourceImageTableEntry;
  H2O_HII_IMAGE_BLOCK_NODE              *SourceImageBlockNode;
  UINT32                                ImagePkgeSizeForThisBoard;
  UINT32                                VerifySize;

  if (List == NULL || PackageSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  TargetImageTableEntry = NULL;
  ImagePkgeSizeForThisBoard = 0;

  //
  //  Select entry by board.
  //
  Status = SelectImageEntryWorker (List, BoardId, &TargetImageTableEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Iterate all node of this image block list,
  //  Restore node who duplicated with the other board.
  //  Local duplicate node should not restored, as they will be in the same package.
  //
  Link = TargetImageTableEntry->AssociateImageBlockList;
  for (Index = 0; Index < TargetImageTableEntry->ImageBlockListNodeCount; Index++) {
    CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (Link);
    //
    //  This "if" clause do all the work of restore duplicate block.
    //
    if (CurrentImageBlockNode->ImageBlockType == EFI_HII_IIBT_DUPLICATE) {
      //
      //  Find out duplicated source in other board entries.
      //
      SourceEntry = &List->TableEntry;
      IsDuplicateBlockRestored = FALSE;
      do {
        SourceImageTableEntry = GET_H2O_HII_IMAGE_TABLE_ENTRY_FROM_THIS (SourceEntry);
        //
        //  Only search in master entry.
        //
        if (!SourceImageTableEntry->IsSlaveEntry) {
          SourceNode = SourceImageTableEntry->AssociateImageBlockList;
          for (NodeIndex = 0; NodeIndex < SourceImageTableEntry->ImageBlockListNodeCount ; NodeIndex++) {
            SourceImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (SourceNode);
            //
            //  Match found, restore content.
            //
            if (SourceImageBlockNode->VirtualImageId == ((EFI_HII_IIBT_DUPLICATE_BLOCK *)(CurrentImageBlockNode->ImageBlock))->ImageId) {
              //
              //  NOTE : Compatible with Build Technical Reference 3.23.2.1.
              //         H2O_HII_IIBT_IMAGE_32BIT_BLOCK is a ext4 type which contains image instance.
              //
              CurrentImageBlockNode->ImageBlockSize = SourceImageBlockNode->ImageBlockSize;
              CurrentImageBlockNode->ImageBlockType = SourceImageBlockNode->ImageBlockType;
              CurrentImageBlockNode->ImageBlock     = SourceImageBlockNode->ImageBlock;
              CurrentImageBlockNode->ImageInstance  = SourceImageBlockNode->ImageInstance;
              //  Should not update VirtualImageId, it is a key.
              IsDuplicateBlockRestored = TRUE;
              break;
            }
            SourceNode = SourceNode->ForwardLink;
          }
        }

        if (IsDuplicateBlockRestored) {
          break;
        }

        SourceEntry = SourceEntry->ForwardLink;
      } while (SourceEntry != &List->TableEntry);
    }
    ImagePkgeSizeForThisBoard += CurrentImageBlockNode->ImageBlockSize;
    Link = Link->ForwardLink;
  }

  //
  //  Verify size.
  //
  VerifySize = 0;
  Link = TargetImageTableEntry->AssociateImageBlockList;
  for (Index = 0; Index < TargetImageTableEntry->ImageBlockListNodeCount; Index++) {
    CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (Link);
    VerifySize += CurrentImageBlockNode->ImageBlockSize;
    Link = Link->BackLink;
  }

  if (VerifySize != ImagePkgeSizeForThisBoard) {
    return EFI_UNSUPPORTED;
  }

  *PackageSize = ImagePkgeSizeForThisBoard;

  return EFI_SUCCESS;
}

/**
 Insyde multi board image package extension.

 Construct a image package data for one board, the board is specified by caller.
 The output package data is a pure package data without end block.
 This is an internal function.

 @param[in, out]  List              A pointer to the first entry.
                                    Funciotn will traverse data struture start at this entry.
 @param[in]       PlatformBoardId   A board id to specify which package to restore.
 @param[out]      PackageData       Update a pointer to a package data with out end block.
 @param[out]      PackageDataLength The size of output package data.
                                    Length is package data specific, not iclude end block.
 @retval     EFI_SUCCESS            Target image package data is created successfully.
 @retval     EFI_INVALID_PARAMETER  List is NULL or PackageData is NULL or PackageDataLength
                                    is NULL.

**/
EFI_STATUS
ConstructImagePkg (
  IN OUT H2O_HII_IMAGE_TABLE_ENTRY      *List,
  IN     H2O_BOARD_ID                   PlatformBoardId,
  OUT    UINT8                          **PackageData,
  OUT    UINT32                         *PackageDataLength
  )
{
  EFI_STATUS                            Status;
  H2O_BOARD_ID                          BoardId;
  UINT32                                DataLength;
  UINT8                                 *BinBuffer;
  H2O_HII_IMAGE_TABLE_ENTRY             *CurrentImageTableEntry;
  INT32                                 Index;
  H2O_HII_IMAGE_BLOCK_NODE              *CurrentImageBlockNode;
  LIST_ENTRY                            *Node;
  UINT32                                Offset;
  UINT32                                VerifySize;
  UINT32                                Length;

  if (List == NULL || PackageData == NULL || PackageDataLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status     = EFI_UNSUPPORTED;
  BoardId    = PlatformBoardId;
  DataLength = 0;
  BinBuffer  = NULL;
  Offset     = 0;
  VerifySize = 0;
  //
  //  Pre process target image block list.
  //  Use board id to find target list, then restore it,
  //  and get real data length.
  //
  Status = RestoreImageBlockWorker (List, BoardId, &DataLength);
  //
  //  Add error handling - when input board id not found, use default imgpkg.
  //
  if (Status == EFI_NOT_FOUND) {
    //
    //  Use default board value 0 to get info again.
    //
    BoardId = 0x00;
    Status = RestoreImageBlockWorker (List, BoardId, &DataLength);
  }

  if (EFI_ERROR (Status)) {
    *PackageData = NULL;
    return Status;
  }

  //
  //  Create empty buffer. Length equals to real data length.
  //
  BinBuffer = AllocateZeroPool (DataLength);
  if (BinBuffer == NULL) {
    *PackageData = NULL;
    return EFI_OUT_OF_RESOURCES;
  }

  //
  //  Retrieve the pointer of image table entry according to board id.
  //  (We allready restore the image block list for this board.)
  //
  Status = SelectImageEntryWorker (List, BoardId, &CurrentImageTableEntry);
  if (EFI_ERROR (Status)) {
    FreePool (BinBuffer);
    *PackageData = NULL;
    return Status;
  }

  //
  //  Iterate all nodes in the image block list of the entry we selected.
  //  For each node, get mapped data and copy them to new buffer.
  //
  Node = CurrentImageTableEntry->AssociateImageBlockList;
  for (Index = 0; Index < CurrentImageTableEntry->ImageBlockListNodeCount; Index++) {
    CurrentImageBlockNode = GET_H2O_HII_IMAGE_BLOCK_NODE_FROM_THIS (Node);
    //
    //  Buffer overflow should not occurred if data length calculated correctly in early stage.
    //
    if (DataLength - Offset < CurrentImageBlockNode->ImageBlockSize) {
      //
      //  Not using status EFI_BUFFER_TOO_SMALL
      //  because size calculation error means process went wrong in early stage.
      //  "Do NOT" handle this error using any re-allocate mamory mechanism.
      //
      FreePool (BinBuffer);
      *PackageData = NULL;
      return EFI_UNSUPPORTED;
    }

    //
    //  Following procedure copy data from source separately.
    //
    //  Set block header.
    //
    VerifySize += CurrentImageBlockNode->ImageBlockSize;
    CopyMem (BinBuffer + Offset, &CurrentImageBlockNode->ImageBlockType, sizeof (UINT8));
    Offset += sizeof (UINT8);
    //
    //  Set block content.
    //
    switch (CurrentImageBlockNode->ImageBlockType) {
    case EFI_HII_IIBT_EXT1:
    case EFI_HII_IIBT_EXT2:
      Length = CurrentImageBlockNode->ImageBlockSize - sizeof (UINT8);
      CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
      Offset += Length;
      break;

    case EFI_HII_IIBT_EXT4:
      //
      //  Compatible with Build Technical Reference 3.23.2.1.
      //  We already reserved buffer size of a skip block for each H2O_HII_IIBT_IMAGE_32BIT_BLOCK.
      //
      if (((EFI_HII_IIBT_EXT4_BLOCK *)(CurrentImageBlockNode->ImageBlock))->BlockType2 == H2O_HII_IIBT_IMAGE_32BIT) {
        Length = ((EFI_HII_IIBT_EXT4_BLOCK *)(CurrentImageBlockNode->ImageBlock))->Length - sizeof (UINT8);
        //  Fatal error checking, incorrect buffer size info from header.
        if (CurrentImageBlockNode->ImageBlockSize - sizeof (EFI_HII_IIBT_SKIP1_BLOCK) - sizeof (UINT8) != Length) {
          DEBUG ((EFI_D_ERROR, "[FatalError]---BlockLength-info-error-when-construct-imgpkg--\n"));
          ASSERT(FALSE);
        }
        CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
        Offset += Length;
        //
        //  Create a skip1 block. Skip count set to 1.
        //
        //  typedef struct _EFI_HII_IIBT_SKIP1_BLOCK {
        //    EFI_HII_IMAGE_BLOCK          Header;
        //    UINT8                        SkipCount;
        //  } EFI_HII_IIBT_SKIP1_BLOCK;
        //
        *(BinBuffer + Offset) = EFI_HII_IIBT_SKIP1;
        *(BinBuffer + Offset + sizeof (UINT8)) = 1;
        Offset += sizeof (EFI_HII_IIBT_SKIP1_BLOCK);
      } else {
        Length = CurrentImageBlockNode->ImageBlockSize - sizeof (UINT8);
        CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
        Offset += Length;
      }
      break;

    //
    //  Following case are currently using same procedure,
    //  intentionally taer them apart into three groups.
    //
    case EFI_HII_IIBT_IMAGE_1BIT:
    case EFI_HII_IIBT_IMAGE_1BIT_TRANS:
    case EFI_HII_IIBT_IMAGE_4BIT:
    case EFI_HII_IIBT_IMAGE_4BIT_TRANS:
    case EFI_HII_IIBT_IMAGE_8BIT:
    case EFI_HII_IIBT_IMAGE_8BIT_TRANS:
    case EFI_HII_IIBT_IMAGE_24BIT:
    case EFI_HII_IIBT_IMAGE_24BIT_TRANS:
      Length = CurrentImageBlockNode->ImageBlockSize - sizeof (UINT8);
      CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
      Offset += Length;
      break;

    case EFI_HII_IIBT_DUPLICATE:
    case EFI_HII_IIBT_SKIP1:
    case EFI_HII_IIBT_SKIP2:
      Length = CurrentImageBlockNode->ImageBlockSize - sizeof (UINT8);
      CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
      Offset += Length;
      break;

    case EFI_HII_IIBT_IMAGE_JPEG:
      Length = CurrentImageBlockNode->ImageBlockSize - sizeof (UINT8);
      CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
      Offset += Length;
      break;

    case EFI_HII_IIBT_IMAGE_PNG:
      Length = CurrentImageBlockNode->ImageBlockSize - sizeof (UINT8);
      CopyMem (BinBuffer + Offset, CurrentImageBlockNode->ImageBlock + sizeof (UINT8), Length);
      Offset += Length;
      break;

    default:
      break;
    }

    //
    //  Verify current water level.
    //
    if (VerifySize != Offset) {
      FreePool (BinBuffer);
      *PackageData = NULL;
      return EFI_UNSUPPORTED;
    }

    Node = Node->ForwardLink;
  }

  //
  //  Finally check size again. This is the total size.
  //
  if (VerifySize != DataLength) {
    FreePool (BinBuffer);
    *PackageData = NULL;
    return EFI_UNSUPPORTED;
  }

  *PackageData = BinBuffer;
  *PackageDataLength = DataLength;

  return EFI_SUCCESS;
}

/**
 Insyde multi board image package extension.

 This is the "MAIN" function for parsing multi-image package.
 Input a image package(multi), output a image package(single) according to 'PcdH2OBoardId'.
 The output package is a pure package data without "end block".
 If corresponding package not found in source, system will use default package patch it.

 @param[in]  FirstImageBlockHeader  A pointer to source image package from package list buffer.

 @param[out] PackageData            Output an address of a new buffer of parsed package data
                                    if whole progress runs successfully.
                                    Output a NULL if process failed.
                                    This buffer should be freed by caller when image package
                                    installation finished.
 @param[out] PackageDataLength      Output a length of new buffer.

 @retval     EFI_SUCCESS            EFI_SUCCESS.
 @retval     EFI_INVALID_PARAMETER  FirstImageBlockHeader is NULL or PackageData is NULL or
                                    PackageDataLength is NULL.
 @retval     EFI_UNSUPPORTED        Input source is not a multi image package
                                    (The first block should be a H2O_HII_IIBT_BOARDS_BLOCK).
                                    Binary format error or violation occured during the parsing.
**/
EFI_STATUS
ClassifyMultiBoardImageBlock (
  IN  CONST UINT8  *FirstImageBlockHeader,
  OUT VOID         **PackageData,
  OUT UINT32       *PackageDataLength
  )
{
  H2O_HII_IMAGE_TABLE_ENTRY      *MultiImageEntryList;
  EFI_STATUS                     Status;
  H2O_BOARD_ID                   PlatformBoardId;
  UINT8                          *CurrentImageBlock;
  UINT32                         BlockLength;
  H2O_HII_IIBT_BOARDS_BLOCK      DefaultBoardsBlock;
  H2O_HII_IIBT_BOARDS_BLOCK      *CurrentBoardsBlock;
  EFI_IMAGE_ID                   CurrentImageId;
  UINT32                         JumpSize;
  EFI_HII_IIBT_IMAGE_1BIT_BLOCK  Iibt1Bit;
  EFI_HII_IIBT_IMAGE_4BIT_BLOCK  Iibt4Bit;
  EFI_HII_IIBT_IMAGE_8BIT_BLOCK  Iibt8Bit;
  EFI_HII_IIBT_IMAGE_24BIT_BLOCK Iibt24Bit;
  UINT16                         SkipCount;
  EFI_IMAGE_ID                   FirstImageIdOfThisBlock;

  if (FirstImageBlockHeader == NULL || PackageData == NULL || PackageDataLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MultiImageEntryList = NULL;
  Status = EFI_UNSUPPORTED;

  //
  //  Let PcdH2OBoardId decide which image package will be used.
  //
  PlatformBoardId = PcdGet64 (PcdH2OBoardId);

  CurrentImageBlock = (UINT8 *)FirstImageBlockHeader;
  CurrentBoardsBlock = NULL;
  BlockLength = 0;
  CurrentImageId = 1;
  JumpSize = 0;

  //
  //  Compatible with situation that fisrt block is not a boards block.
  //  Current image buffer "doesn't" have a boards block at the head.
  //  This virtual boards block represents the defualt boards block attached at
  //  the head of multi image package. See image c array in module's AutoGen.c.
  //  This virtual block also maintain the compatibility for Rev 5.2 and 5.3 co-using build.exe.
  //
  DefaultBoardsBlock.Hdr.Header.BlockType = EFI_HII_IIBT_EXT1;
  DefaultBoardsBlock.Hdr.BlockType2       = H2O_HII_IIBT_BOARDS;
  DefaultBoardsBlock.Hdr.Length           = 0x06; //  sizeof (H2O_HII_IIBT_BOARDS_BLOCK)
  DefaultBoardsBlock.ImageId              = 0x00; //  Current usage reserved.
  DefaultBoardsBlock.Boards[0]            = 0x00; //  Default 0x00
  //
  //  Set CurrentBoardsBlock to virtual default block first.
  //  CurrentBoardsBlock pointer updates whenever
  //  boards block has been encountered while parsing image package buffer.
  //
  CurrentBoardsBlock = (H2O_HII_IIBT_BOARDS_BLOCK *)&DefaultBoardsBlock;

  while (*CurrentImageBlock != EFI_HII_IIBT_END) {
    switch (*CurrentImageBlock) {
    case EFI_HII_IIBT_EXT1:
      BlockLength = ((EFI_HII_IIBT_EXT1_BLOCK *)CurrentImageBlock)->Length;
      //
      //  Recored boards block :
      //  Following image blocks will associate with this boards info until next
      //  boards block found in source buffer.
      //  Boards block will not be added to list.
      //
      if (((H2O_HII_IIBT_BOARDS_BLOCK *)CurrentImageBlock)->Hdr.BlockType2 == H2O_HII_IIBT_BOARDS) {
        CurrentBoardsBlock = (H2O_HII_IIBT_BOARDS_BLOCK *)CurrentImageBlock;
      } else {
        //
        //  Normal Ext1 block should remain the same.
        //  For other Ext1 block who has special usage(future extension),
        //  the conditional branch should be added prior to this "else" clause.
        //
        Status = ClassifyImageBlockWorker (
                   &MultiImageEntryList,
                   BlockLength,
                   EFI_HII_IIBT_EXT1,
                   CurrentImageBlock,
                   NULL,
                   CurrentBoardsBlock,
                   0 // Doesnt require image id.
                   );
        if (EFI_ERROR (Status)) {
          goto Clean;
        }
      }
      JumpSize += BlockLength;
      break;

    case EFI_HII_IIBT_EXT2:
      BlockLength = ((EFI_HII_IIBT_EXT2_BLOCK *)CurrentImageBlock)->Length;
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 EFI_HII_IIBT_EXT2,
                 CurrentImageBlock,
                 NULL,
                 CurrentBoardsBlock,
                 0 // Doesnt require image id.
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      break;

    case EFI_HII_IIBT_EXT4:
      //
      //  NOTE : Compatible with Build Technical Reference 3.23.2.1.
      //
      //  When system performs get image in hii data base,
      //  H2O_HII_IIBT_IMAGE_32BIT_BLOCK does not increment image id, it will be followed by
      //  a skip block to help increace image id.
      //
      //  In the data structure under the MultiImageEntryList,
      //  the SKIP block right next to H2O_HII_IIBT_IMAGE_32BIT_BLOCK will not be add here
      //  to keep the virtual key of this node unique. The CurrentImageId increased 1 as
      //  we ignored a SKIP block. This ensures the following block get correct id(global view).
      //
      //  When parser performs restore and construct image package method,
      //  the skip block will be appended to H2O_HII_IIBT_IMAGE_32BIT_BLOCK.
      //
      //  The other normal EXT4 blocks will be added with NULL id.
      //
      if (((H2O_HII_IIBT_IMAGE_32BIT_BLOCK *)CurrentImageBlock)->BlockType2 == H2O_HII_IIBT_IMAGE_32BIT) {
        BlockLength = ((EFI_HII_IIBT_EXT4_BLOCK *)CurrentImageBlock)->Length;
        //
        //  This skip block is unique next to H2O_HII_IIBT_IMAGE_32BIT_BLOCK,
        //  tools should not mixup this block with the others.
        //
        if (((EFI_HII_IIBT_SKIP1_BLOCK *)(CurrentImageBlock + BlockLength))->SkipCount > 1) {
          Status = EFI_UNSUPPORTED;
          goto Clean;
        }
        //
        //  Jump over a skip block. Total length is incremented by the size of a skip1.
        //
        BlockLength += sizeof (EFI_HII_IIBT_SKIP1_BLOCK);
        Status = ClassifyImageBlockWorker (
                   &MultiImageEntryList,
                   BlockLength,
                   EFI_HII_IIBT_EXT4,
                   CurrentImageBlock,
                   CurrentImageBlock + sizeof (H2O_HII_IIBT_IMAGE_32BIT_BLOCK) - sizeof (H2O_HII_RGBA_PIXEL),
                   CurrentBoardsBlock,
                   CurrentImageId
                   );
        if (EFI_ERROR (Status)) {
          goto Clean;
        }
        JumpSize += BlockLength;
        CurrentImageId++;
      } else {
        BlockLength = ((EFI_HII_IIBT_EXT4_BLOCK *)CurrentImageBlock)->Length;
        //
        //  For normal ext block,
        //  should not increment CurrentImageId,
        //  should not jump over its neighborhood.
        //
        Status = ClassifyImageBlockWorker (
                   &MultiImageEntryList,
                   BlockLength,
                   EFI_HII_IIBT_EXT4,
                   CurrentImageBlock,
                   NULL,
                   CurrentBoardsBlock,
                   0 // Doesnt require image id.
                   );
        if (EFI_ERROR (Status)) {
          goto Clean;
        }
        JumpSize += BlockLength;
      }
      break;

    case EFI_HII_IIBT_IMAGE_1BIT:
    case EFI_HII_IIBT_IMAGE_1BIT_TRANS:
      CopyMem (&Iibt1Bit, CurrentImageBlock, sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK));
      BlockLength = sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK) + BITMAP_LEN_1_BIT (Iibt1Bit.Bitmap.Width, Iibt1Bit.Bitmap.Height) - sizeof (UINT8);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 *CurrentImageBlock,
                 CurrentImageBlock,
                 CurrentImageBlock + sizeof (EFI_HII_IIBT_IMAGE_1BIT_BLOCK) - sizeof (UINT8),
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_IMAGE_4BIT:
    case EFI_HII_IIBT_IMAGE_4BIT_TRANS:
      CopyMem (&Iibt4Bit, CurrentImageBlock, sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK));
      BlockLength = sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK) + BITMAP_LEN_4_BIT (Iibt4Bit.Bitmap.Width, Iibt4Bit.Bitmap.Height) - sizeof (UINT8);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 *CurrentImageBlock,
                 CurrentImageBlock,
                 CurrentImageBlock + sizeof (EFI_HII_IIBT_IMAGE_4BIT_BLOCK) - sizeof (UINT8),
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_IMAGE_8BIT:
    case EFI_HII_IIBT_IMAGE_8BIT_TRANS:
      CopyMem (&Iibt8Bit, CurrentImageBlock, sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK));
      BlockLength = sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK) + BITMAP_LEN_8_BIT (Iibt8Bit.Bitmap.Width, Iibt8Bit.Bitmap.Height) - sizeof (UINT8);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 *CurrentImageBlock,
                 CurrentImageBlock,
                 CurrentImageBlock + sizeof (EFI_HII_IIBT_IMAGE_8BIT_BLOCK) - sizeof (UINT8),
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_IMAGE_24BIT:
    case EFI_HII_IIBT_IMAGE_24BIT_TRANS:
      CopyMem (&Iibt24Bit, CurrentImageBlock, sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK));
      BlockLength = sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) + BITMAP_LEN_24_BIT (Iibt24Bit.Bitmap.Width, Iibt24Bit.Bitmap.Height) - sizeof (EFI_HII_RGB_PIXEL);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 *CurrentImageBlock,
                 CurrentImageBlock,
                 CurrentImageBlock + sizeof (EFI_HII_IIBT_IMAGE_24BIT_BLOCK) - sizeof (EFI_HII_RGB_PIXEL),
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_DUPLICATE:
      BlockLength = sizeof (EFI_HII_IIBT_DUPLICATE_BLOCK);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 EFI_HII_IIBT_DUPLICATE,
                 CurrentImageBlock,
                 NULL,
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_IMAGE_JPEG:
      //  Reference:
      //  typedef struct _EFI_HII_IIBT_JPEG_BLOCK {
      //    EFI_HII_IMAGE_BLOCK          Header;  //  UINT8 BlockType.
      //    UINT32                       Size;    //  Image file size alone.
      //    UINT8                        Data[1]; //  First byte of image.
      //  } EFI_HII_IIBT_JPEG_BLOCK;
      //
      BlockLength = ((EFI_HII_IIBT_JPEG_BLOCK *)CurrentImageBlock)->Size + sizeof (EFI_HII_IIBT_JPEG_BLOCK) - sizeof (UINT8);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 EFI_HII_IIBT_IMAGE_JPEG,
                 CurrentImageBlock,
                 CurrentImageBlock + sizeof (EFI_HII_IIBT_JPEG_BLOCK) - sizeof (UINT8),
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_IMAGE_PNG:
      BlockLength = ((EFI_HII_IIBT_PNG_BLOCK *)CurrentImageBlock)->Size + sizeof (EFI_HII_IIBT_PNG_BLOCK) - sizeof (UINT8);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 EFI_HII_IIBT_IMAGE_PNG,
                 CurrentImageBlock,
                 CurrentImageBlock + sizeof (EFI_HII_IIBT_PNG_BLOCK) - sizeof (UINT8),
                 CurrentBoardsBlock,
                 CurrentImageId
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId++;
      break;

    case EFI_HII_IIBT_SKIP1:
      FirstImageIdOfThisBlock = CurrentImageId;
      SkipCount = ((EFI_HII_IIBT_SKIP1_BLOCK *)CurrentImageBlock)->SkipCount;
      BlockLength = sizeof (EFI_HII_IIBT_SKIP1_BLOCK);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 EFI_HII_IIBT_SKIP1,
                 CurrentImageBlock,
                 NULL,
                 CurrentBoardsBlock,
                 FirstImageIdOfThisBlock
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId = CurrentImageId + SkipCount;
      break;

    case EFI_HII_IIBT_SKIP2:
      FirstImageIdOfThisBlock = CurrentImageId;
      SkipCount = ((EFI_HII_IIBT_SKIP2_BLOCK *)CurrentImageBlock)->SkipCount;
      BlockLength = sizeof (EFI_HII_IIBT_SKIP2_BLOCK);
      Status = ClassifyImageBlockWorker (
                 &MultiImageEntryList,
                 BlockLength,
                 EFI_HII_IIBT_SKIP2,
                 CurrentImageBlock,
                 NULL,
                 CurrentBoardsBlock,
                 FirstImageIdOfThisBlock
                 );
      if (EFI_ERROR (Status)) {
        goto Clean;
      }
      JumpSize += BlockLength;
      CurrentImageId = CurrentImageId + SkipCount;
      break;

    default:
      //
      //  Should not run to here if data in source buffer is correct.
      //
      Status = EFI_UNSUPPORTED;
      goto Clean;
      //
      //  A good habit to add break.
      //
      break;
    }
    CurrentImageBlock = (UINT8 *)FirstImageBlockHeader + JumpSize;
  }

  //
  // Not found any image, directly return
  //
  if (MultiImageEntryList == NULL) {
    *PackageData = NULL;
    PackageDataLength = 0;
    return EFI_SUCCESS;
  }

  //
  //  Construct a image package by board.
  //
  Status = ConstructImagePkg (
             MultiImageEntryList,
             PlatformBoardId,
             (UINT8 **)PackageData,
             PackageDataLength
             );

Clean:
  BoardImageTableListDestructor (MultiImageEntryList);
  return Status;
}
