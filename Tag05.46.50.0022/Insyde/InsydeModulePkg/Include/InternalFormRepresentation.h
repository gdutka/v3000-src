/** @file
  HII Form Representation

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

#ifndef _INTERNAL_FORM_REPRESENTATION_H_
#define _INTERNAL_FORM_REPRESENTATION_H_

#include <Uefi/UefiInternalFormRepresentation.h>
#include <H2OBoardId.h>

#define IMAGE_TOKEN(t)     t
#define ANIMATION_TOKEN(t) t


#define H2O_HII_IIBT_IMAGE_32BIT       0x80
#define H2O_IMAGE_ALPHA_CHANNEL        0x80000000

//
//  Insyde multi board image package extension. See BTR section 3.23.2.2.
//  Image EXT1 block which has this type2 value will introduce a H2O_HII_IIBT_BOARDS_BLOCK.
//
#define H2O_HII_IIBT_BOARDS                  0x81

#pragma pack(1)

typedef struct _H2O_HII_RGBA_PIXEL {
  UINT8                        b;
  UINT8                        g;
  UINT8                        r;
  UINT8                        a;
} H2O_HII_RGBA_PIXEL;

typedef struct _H2O_HII_IIBT_IMAGE_32BIT_BLOCK {
  EFI_HII_IMAGE_BLOCK              Header;
  UINT8                            BlockType2;
  UINT32                           Length;
  UINT16                           Width;
  UINT16                           Height;
  H2O_HII_RGBA_PIXEL               Bitmap[1];
} H2O_HII_IIBT_IMAGE_32BIT_BLOCK;

//
//  Insyde multi board image package extension.
//  Image blocks next to this header are for a specific board.
//
typedef struct _H2O_HII_IIBT_BOARDS_BLOCK {
  EFI_HII_IIBT_EXT1_BLOCK  Hdr;
  EFI_IMAGE_ID             ImageId;
  H2O_BOARD_ID             Boards[1];
} H2O_HII_IIBT_BOARDS_BLOCK;

#pragma pack()


#endif

