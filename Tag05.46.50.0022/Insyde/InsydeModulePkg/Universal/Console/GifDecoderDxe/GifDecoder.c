/** @file
  This code supports a the private implementation
  of the GIF Decoder protocol

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

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include "GifDecoder.h"

GIF_DECODER_INSTANCE    mPrivateData = {
  GIF_DECODER_INSTANCE_SIGNATURE,
  NULL,
  {
   CreateAnimationFromMem,
   DestroyAnimation,
   NextAnimationFrame,
   RefreshAnimation,
  },
  {
   sizeof (EFI_GIF_DECODER2_PROTOCOL),
   CreateAnimationFromMem2,
   DestroyAnimation2,
   NextAnimationFrame2,
   RefreshAnimation2,
   ConvertAnimationToBlt2
  }
};


/**
 Install Driver to produce Gif Decoder protocol.

 @param [in]   ImageHandle
 @param [in]   SystemTable

 @retval EFI_SUCCESS            Gif Decoder protocol installed
 @return Other        No protocol installed, unload driver.

**/
EFI_STATUS
EFIAPI
GifDecoderInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivateData.Handle,
                  &gEfiGifDecoderProtocolGuid,
                  &mPrivateData.GifDecoder,
                  &gEfiGifDecoder2ProtocolGuid,
                  &mPrivateData.GifDecoder2,
                  NULL
                  );
  return Status;
}

/**
 Create animation from file image buffer

 @param [in]   This             Pointer to protocol instance.
 @param [in]   FileData
 @param [in]   FileSize
 @param [in]   Data             Save private data information.
 @param [out]  Animation        Contain all decompress images which are BLT format in GIF image data.

 @retval EFI_SUCCESS            create success
 @retval EFI_INVALID_PARAMETER  invalid parameter
 @retval EFI_OUT_OF_RESOURCES   allocate memory fail
 @retval EFI_UNSUPPORTED        format error

**/
EFI_STATUS
EFIAPI
CreateAnimationFromMem (
  IN      EFI_GIF_DECODER_PROTOCOL     *This,
  IN      UINT8                        *FileData,
  IN      UINTN                        FileSize,
  IN      VOID                         *Data,
     OUT  ANIMATION                    **Animation
  )
{
  return CreateAnimationFromMem2 (
           &mPrivateData.GifDecoder2,
           FileData,
           FileSize,
           Data,
           Animation
           );
}

/**
 Draw next animation frame

 @param [in]   This             Pointer to protocol instance.
 @param [in]   Entry            Animation Refresh Entry for animation information in screen
 @param [in]   Gop              Graphics output protocol interface

 @retval EFI_SUCCESS            draw frame success
 @retval EFI_NOT_FOUND          no next frame to draw
 @retval EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
NextAnimationFrame (
  IN      EFI_GIF_DECODER_PROTOCOL     *This,
  IN      ANIMATION_REFRESH_ENTRY      *Entry,
  IN      EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  )
{
  return NextAnimationFrame2 (
           &mPrivateData.GifDecoder2,
           Entry,
           Gop
           );
}

/**
 By record tick and delay time to determinate draw image

 @param [in]   This             Pointer to protocol instance.
 @param [in]   AnimationRefrshEntry  Pointer to animation refresh entry
 @param [in]   Gop              Point to graphics output protocol
 @param [in]   CpuFrequency     Cpu Frequency for calculate tick elapsed time

 @retval EFI_SUCCESS            animation is playing
 @retval EFI_NOT_FOUND          animation is stopping
 @retval EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
RefreshAnimation (
  IN      EFI_GIF_DECODER_PROTOCOL     *This,
  IN      ANIMATION_REFRESH_ENTRY      *AnimationRefrshEntry,
  IN      EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
  IN      UINT64                       CpuFrequency
  )
{
  return RefreshAnimation2 (
           &mPrivateData.GifDecoder2,
           AnimationRefrshEntry,
           Gop,
           CpuFrequency
           );
}

/**
 Draw next animation frame

 @param [in]   This             Pointer to protocol instance.
 @param [in]   Animation        Contain all decompress images which are BLT format in GIF image data.

 @retval EFI_SUCCESS            free memory success
 @retval EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
DestroyAnimation (
  IN      EFI_GIF_DECODER_PROTOCOL     *This,
  IN      ANIMATION                    *Animation
  )
{
  return DestroyAnimation2 (
           &mPrivateData.GifDecoder2,
           Animation
           );
}
/**
 Create animation from file image buffer

 @param [in]   This             Pointer to protocol instance.
 @param [in]   FileData
 @param [in]   FileSize
 @param [in]   Data             Save private data information.
 @param [out]  Animation        Contain all decompress images which are BLT format in GIF image data.

 @retval EFI_SUCCESS            create success
 @retval EFI_INVALID_PARAMETER  invalid parameter
 @retval EFI_OUT_OF_RESOURCES   allocate memory fail
 @retval EFI_UNSUPPORTED        format error

**/
EFI_STATUS
EFIAPI
CreateAnimationFromMem2 (
  IN      EFI_GIF_DECODER2_PROTOCOL    *This,
  IN      UINT8                        *FileData,
  IN      UINTN                        FileSize,
  IN      VOID                         *Data,
     OUT  ANIMATION                    **Animation
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiCreateAnimationFromMem (
             FileData,
             FileSize,
             Data,
             Animation
             );

  return Status;
}

/**
 Draw next animation frame

 @param [in]   This             Pointer to protocol instance.
 @param [in]   Entry            Animation Refresh Entry for animation information in screen
 @param [in]   Gop              Graphics output protocol interface

 @retval EFI_SUCCESS            draw frame success
 @retval EFI_NOT_FOUND          no next frame to draw
 @retval EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
NextAnimationFrame2 (
  IN      EFI_GIF_DECODER2_PROTOCOL    *This,
  IN      ANIMATION_REFRESH_ENTRY      *Entry,
  IN      EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiNextAnimationFrame (
             Entry,
             Gop
             );

  return Status;
}

/**
 By record tick and delay time to determinate draw image

 @param [in]   This             Pointer to protocol instance.
 @param [in]   AnimationRefrshEntry  Pointer to animation refresh entry
 @param [in]   Gop              Point to graphics output protocol
 @param [in]   CpuFrequency     Cpu Frequency for calculate tick elapsed time

 @retval EFI_SUCCESS            animation is playing
 @retval EFI_NOT_FOUND          animation is stopping
 @retval EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
RefreshAnimation2 (
  IN      EFI_GIF_DECODER2_PROTOCOL    *This,
  IN      ANIMATION_REFRESH_ENTRY      *AnimationRefrshEntry,
  IN      EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
  IN      UINT64                       CpuFrequency
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiRefreshAnimation (
             AnimationRefrshEntry,
             Gop,
             CpuFrequency
             );

  return Status;
}

/**
 Draw next animation frame

 @param [in]   This             Pointer to protocol instance.
 @param [in]   Animation        Contain all decompress images which are BLT format in GIF image data.

 @retval EFI_SUCCESS            free memory success
 @retval EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
DestroyAnimation2 (
  IN      EFI_GIF_DECODER2_PROTOCOL    *This,
  IN      ANIMATION                    *Animation
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiDestroyAnimation (Animation);

  return Status;
}

/**
 Convert all animation frames to BLT buffer.

 @param [in]      This             Pointer to protocol instance.
 @param [in]      Animation        Contain all decompress images which are BLT format in GIF image data.
 @param [in]      BltBufferSize    BLT buffer size.
 @param [in, out] BltBuffer        Pointer to BLT buffer.

 @retval EFI_SUCCESS               Convert all animation frames success
 @retval Others                    Convert animation failed
**/
EFI_STATUS
EFIAPI
ConvertAnimationToBlt2 (
  IN     EFI_GIF_DECODER2_PROTOCOL     *This,
  IN     ANIMATION                     *Animation,
  IN     UINTN                         BltBufferSize,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer
  )
{
  EFI_STATUS                            Status;

  Status = H2OHiiConvertAnimationToBlt (Animation, BltBufferSize, BltBuffer);

  return Status;
}

