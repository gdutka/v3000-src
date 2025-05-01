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

#ifndef _PNG_DECODER_H_
#define _PNG_DECODER_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/H2OImageDecoderLib.h>
#include <Protocol/PngDecoder.h>

#define PNG_DECODER_INSTANCE_SIGNATURE   SIGNATURE_32('P','n','g','D')

typedef struct {
  UINT32                             Signature;
  EFI_HANDLE                         Handle;
  //
  // Produced protocol(s)
  //
  EFI_PNG_DECODER_PROTOCOL           PngDecoder;

} PNG_DECODER_INSTANCE;

#define PNG_DECODER_INSTANCE_FROM_THIS(This) \
  CR(This, PNG_DECODER_INSTANCE, PngDecoder, PNG_DECODER_INSTANCE_SIGNATURE)


EFI_STATUS
PngDecoderDecodeImage (
  IN     EFI_PNG_DECODER_PROTOCOL      *This,
  IN     UINT8                         *ImageData,
  IN     UINTN                         ImageDataSize,
     OUT UINT8                         **DecodedData,
     OUT UINTN                         *DecodedDataSize,
     OUT UINTN                         *Height,
     OUT UINTN                         *Width
  );

#endif
