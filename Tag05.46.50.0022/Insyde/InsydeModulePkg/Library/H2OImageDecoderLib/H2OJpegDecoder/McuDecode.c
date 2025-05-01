/** @file

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

/*++
*   Copyright (c) 2017 A. Tarpai
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
--*/

/*++

Copyright (c)  1999 - 2002 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  McuDecode.c

Abstract:
  This code decode the JFIF image.
--*/

#include "McuDecode.h"



#define GetByteBits(CurByte, BitPos, Bits)\
          (((CurByte) >> ((BitPos)- (Bits) + 1)) & (0xFF >> (8 - (Bits))))

#define GetWordBits(CurByte, BitPos, Bits)\
          (((CurByte) >> ((BitPos)- (Bits) + 1)) & (0xFFFF >> (16 - (Bits))))

#define GetRGB(x)     ((x)<0?0:((x)>255?255:x))

#define GetMax(x,y)   ((x)>(y)?(x):(y))

#define GetRate(x,y)  ((y)==1?(x):((y)==2?((x)>>1):((y)==4?((x)>>2):0)))


extern EFI_JPEG_DECODER_DATA   mDecoderData;

UINT8 ZigZag[8][8]={
              { 0, 1, 5, 6,14,15,27,28},
              { 2, 4, 7,13,16,26,29,42},
              { 3, 8,12,17,25,30,41,43},
              { 9,11,18,24,37,40,44,53},
              {10,19,23,32,39,45,52,54},
              {20,22,33,38,46,51,55,60},
              {21,34,37,47,50,56,59,61},
              {35,36,48,49,57,58,62,63}};

/**
 Get the bit of mDecoderData.BitPos in the mDecoderData.CurByte,
 mDecoderData.BitPos is between 0 and 7.

 @param [out]  IsEnd            When reading a new byte from the image, the image is end or not

 @return 0 or 1        The bit value of the mDecoderData.BitPos in the mDecoderData.CurByte.

**/
UINT8
GetNextBit (
  OUT    BOOLEAN                        *IsEnd
  )
{
  UINT8    value;
  value = (mDecoderData.CurByte >> mDecoderData.BitPos) & 0x01;

  *IsEnd = FALSE;
  if (mDecoderData.BitPos > 0) {
    mDecoderData.BitPos--;
  } else {
    *IsEnd = ReadByte ();
    mDecoderData.BitPos = 7;
  }
  return value;
}

/**
 Get the N bits of mDecoderData.BitPos in the mDecoderData.CurByte,

 @param[in]   NBits            Number of bits to read
 @param[out]  IsEnd            When reading a new byte from the image, the image is end or not

 @return The bits value of the mDecoderData.BitPos in the mDecoderData.CurByte.
**/
STATIC
UINT16
GetNBits (
  IN  UINTN                     NBits,
  OUT BOOLEAN                   *IsEnd
  )
{
  UINT16                        Value;

  for (Value = 0; NBits > 0 && !(*IsEnd); NBits--) {
    Value = 2 * Value + GetNextBit (IsEnd);
  }
  return Value;
}

/**
 Read a byte from the mDecoderData.ImagePtr to the mDecoderData.CurByte,


 @retval TRUE                   The mDecoderData.CurByte is image end.
 @retval FALSE                  The mDecoderData.CurByte is not image end.

**/
BOOLEAN
ReadByte (
  )
{
  BOOLEAN    IsEnd;

  if (mDecoderData.ImagePtr >= mJfifData.EndImagePtr) {
    return TRUE;
  }

  IsEnd = FALSE;
  mDecoderData.CurByte = *(mDecoderData.ImagePtr);
  if (*(mDecoderData.ImagePtr) == 0xFF) {
    if (*(mDecoderData.ImagePtr + 1) == 0x00) {
      mDecoderData.ImagePtr += 2;
    } else if (*(mDecoderData.ImagePtr + 1) == 0xD9) {
      //
      //arrived the End of JFIF image 0xFFD9
      //
      IsEnd = TRUE;
    }
  } else {
    mDecoderData.ImagePtr++;
  }
  return IsEnd;
}

/**
 Read the value decoded by Huffman table.

 @param[in]  HuffTableIndex     The indexes of Huffman table
 @param[out] IsEnd              The image is end or not
 @param[out] DecoderStatus      The detail status of the block decoding
 @param[out] HuffValue          Pointer to the value decoded by Huffman table

 @retval EFI_SUCCESS            Read the value successfully.
 @retval EFI_UNSUPPORTED        Data is end or huffman table layer is larger than 16.
**/
STATIC
EFI_STATUS
ReadHuffmanValue (
  IN  UINT8                        HuffTableIndex,
  OUT BOOLEAN                      *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS      *DecoderStatus,
  OUT UINT8                        *HuffValue
  )
{
  INT16    Code;
  UINT8    CurBit;
  UINT8    LayerIndex;
  UINT8    ValIndex;

  //
  //Decode the Huffman code
  //
  *IsEnd = FALSE;
  Code = 0;
  LayerIndex = 0;
  do {
    if (*IsEnd) {
      *DecoderStatus = EFI_JPEG_DECODEDATA_ERROR;
      return EFI_UNSUPPORTED;
    }

    LayerIndex++;
    CurBit = GetNextBit(IsEnd);
    Code = (Code << 1) + CurBit;
    if (LayerIndex > 16) {
      *DecoderStatus = EFI_JPEG_DECODEHT_ERROR;
      return EFI_UNSUPPORTED;
    }
  } while (mJfifData.HuffTable[HuffTableIndex].MaxCode[LayerIndex] < Code ||
           mJfifData.HuffTable[HuffTableIndex].Codes[LayerIndex] == 0);

  ValIndex = mJfifData.HuffTable[HuffTableIndex].FirstCode[LayerIndex];
  ValIndex = ValIndex + (UINT8)(Code - mJfifData.HuffTable[HuffTableIndex].MinCode[LayerIndex]);
  *HuffValue = mJfifData.HuffTable[HuffTableIndex].HuffmanVal[ValIndex];

  return EFI_SUCCESS;
}

/**
 Decode the block element from the image(Base on the mDecoderData.ImagePtr).


 @retval EFI_SUCCESS            The element decoded successfully
 @retval EFI_UNSUPPORTED        The element decoding is failed, detail status refers "DecoderStatus"

**/
EFI_STATUS
ElementDecode (
  IN        UINT8                        HTIndex,
     OUT    UINT8                        *ZeroCount,
     OUT    INT16                        *DecodedVal,
     OUT    BOOLEAN                      *IsEnd,
     OUT    EFI_JPEG_DECODER_STATUS      *DecoderStatus
  )
{
  UINT16   CurWord;
  UINT8    HuffVal;
  UINT8    ValBits;
  UINT16   DecodingVal;
  UINT16   ReferVal;
  EFI_STATUS Status;

  Status = ReadHuffmanValue (HTIndex, IsEnd, DecoderStatus, &HuffVal);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //get the HuffVal's bits and get the value
  //
  *ZeroCount = HuffVal >> 4;
  ValBits = HuffVal & 0x0F;
  if (ValBits == 0) {
   *DecodedVal = 0;
   *DecoderStatus = EFI_JPEG_DECODE_SUCCESS;
    return EFI_SUCCESS;
  }
  ReferVal = 1 << (ValBits - 1);
  if (mDecoderData.BitPos >= (ValBits - 1)) {
    DecodingVal = GetByteBits(mDecoderData.CurByte, mDecoderData.BitPos, ValBits);
    if (mDecoderData.BitPos > (ValBits - 1)) {
      mDecoderData.BitPos = mDecoderData.BitPos - ValBits;
    } else {
      mDecoderData.BitPos = 7;
      if (ReadByte ()) {
        *DecoderStatus = EFI_JPEG_DECODEDATA_ERROR;
        return EFI_UNSUPPORTED;
      }
    }
  } else {
    DecodingVal = GetByteBits(
                    mDecoderData.CurByte,
                    mDecoderData.BitPos,
                    (mDecoderData.BitPos + 1)
                    );
    ValBits = ValBits - (mDecoderData.BitPos + 1);
    if (ValBits >= 8) {
      if (ReadByte ()) {
        *DecoderStatus = EFI_JPEG_DECODEDATA_ERROR;
        return EFI_UNSUPPORTED;
      }
      CurWord = mDecoderData.CurByte;
      if (ReadByte ()) {
        *DecoderStatus = EFI_JPEG_DECODEDATA_ERROR;
        return EFI_UNSUPPORTED;
      }
      CurWord = (CurWord << 8) + mDecoderData.CurByte;
      DecodingVal = (DecodingVal << ValBits) + GetWordBits(CurWord, 15, ValBits);
      mDecoderData.BitPos = 15 - ValBits;
    } else {
      if (ReadByte ()) {
        *DecoderStatus = EFI_JPEG_DECODEDATA_ERROR;
        return EFI_UNSUPPORTED;
      }
      DecodingVal = (DecodingVal << ValBits) + GetByteBits(mDecoderData.CurByte, 7, ValBits);
      mDecoderData.BitPos = 7 - ValBits;
    }
  }
  //
  //the decode value is negative.
  //
  if ( DecodingVal < ReferVal) {
    ReferVal = (0xFFFF << (HuffVal & 0x0F)) + 1;
    DecodingVal = DecodingVal + ReferVal;
  }
  *DecodedVal = DecodingVal << mJfifData.SosData.Al;
  return EFI_SUCCESS;
}

/**
 Refine the block data for AC coefficients with Huffman coding.

 @param[out] BlockBuff          The pointer of the block's decoded data
 @param[out] IsEnd              The image is end or not

 @retval EFI_SUCCESS            The block decoded successfully
 @retval EFI_UNSUPPORTED        The block decoding is failed, detail status refers "DecoderStatus"

**/
EFI_STATUS
BlockDecodeAcRefine (
  OUT INT16                        *BlockBuff,
  OUT BOOLEAN                      *IsEnd
  )
{
  if (*BlockBuff != 0) {
    if (GetNextBit (IsEnd) != 0) {
      *BlockBuff += (*BlockBuff > 0) ? mJfifData.SosData.Al2 : -((INT16) mJfifData.SosData.Al2);
    }
  }
  return *BlockBuff;
}

/**
 Decode the block data from the image by AC coefficients with Huffman coding using successive approximation.

 @param[out]     BlockBuff         The pointer of the block's decoded data
 @param[in]      DcAcHTIndex       The indexes of this block's AC, DC Huffman table
 @param[out]     IsEnd             The image is end or not
 @param[out]     DecoderStatus     The detail status of the block decoding
 @param[in, out] DecoderBlockData  Pointer to decoder block data

 @retval EFI_SUCCESS               The block decoded successfully
**/
EFI_STATUS
BlockDecodeAcSuccHuff (
  OUT INT16                              *BlockBuff,
  IN  UINT8                              DcAcHTIndex,
  OUT BOOLEAN                            *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS            *DecoderStatus,
  IN  EFI_JPEG_DECODER_BLOCK_DATA        *DecoderBlockData
  )
{
  UINT8                                  HuffTableIndex;
  UINTN                                  Index;
  UINT8                                  HuffValue;
  EFI_STATUS                             Status;
  UINT8                                  NBits;
  INT16                                  Value;

  Index = mJfifData.SosData.Ss;

  if (*DecoderBlockData->DcValPtr == 0) {
    HuffTableIndex = (DcAcHTIndex & 0x0F) + JPEG_HUFFMAN_TABLE_AC_BASE;

    for (;; Index++) {
      Status = ReadHuffmanValue (HuffTableIndex, IsEnd, DecoderStatus, &HuffValue);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Value = HuffValue & 0xF;
      NBits = HuffValue >> 4;

      if (Value != 0) {
        Value = GetNextBit(IsEnd) ? mJfifData.SosData.Al2 : -((INT16) mJfifData.SosData.Al2);
      } else {
        if (NBits < 15) {
          if (NBits > 0) {
            *DecoderBlockData->DcValPtr = GetNBits (NBits, IsEnd) + ~(-1 << NBits);
          }
          break;
        }
      }

      for (; ;Index++) {
        if (!BlockDecodeAcRefine (&BlockBuff[Index], IsEnd)) {
          if (!NBits--) {
            break;
          }
        }
      }
      BlockBuff[Index] = Value;

      if (Index == mJfifData.SosData.Se) {
        return EFI_SUCCESS;
      }
    }
  } else {
    (*DecoderBlockData->DcValPtr)--;
  }

  for (; Index <= mJfifData.SosData.Se; Index++) {
    BlockDecodeAcRefine (&BlockBuff[Index], IsEnd);
  }
  return EFI_SUCCESS;
}

/**
 Decode the block data from the image by AC coefficients with Huffman coding.

 @param[out]     BlockBuff         The pointer of the block's decoded data
 @param[in]      DcAcHTIndex       The indexes of this block's AC, DC Huffman table
 @param[out]     IsEnd             The image is end or not
 @param[out]     DecoderStatus     The detail status of the block decoding
 @param[in, out] DecoderBlockData  Pointer to decoder block data

 @retval EFI_SUCCESS               The block decoded successfully
 @retval Other                     The block decoding is failed, detail status refers "DecoderStatus"
**/
EFI_STATUS
BlockDecodeAcHuff (
  OUT INT16                              *BlockBuff,
  IN  UINT8                              DcAcHTIndex,
  OUT BOOLEAN                            *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS            *DecoderStatus,
  IN  EFI_JPEG_DECODER_BLOCK_DATA        *DecoderBlockData
  )
{
  UINT8                                  HuffTableIndex;
  UINT8                                  Index;
  UINT8                                  NBits;
  INT16                                  Value;
  EFI_STATUS                             Status;

  if (*DecoderBlockData->DcValPtr == 0) {
    HuffTableIndex = (DcAcHTIndex & 0x0F) + JPEG_HUFFMAN_TABLE_AC_BASE;

    for (Index = mJfifData.SosData.Ss; ; Index++) {
      Status = ElementDecode (HuffTableIndex, &NBits, &Value, IsEnd, DecoderStatus);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if (*DecoderStatus == EFI_JPEG_DECODE_SUCCESS && Value == 0) {
        if (NBits < 15) {
          if (NBits > 0) {
            *DecoderBlockData->DcValPtr = GetNBits (NBits, IsEnd) + ~(-1 << NBits);
          }
          return EFI_SUCCESS;
        }
      }

      Index += NBits;
      BlockBuff[Index] = Value;
      if (Index >= mJfifData.SosData.Se) {
        return EFI_SUCCESS;
      }
    }
  } else {
    (*DecoderBlockData->DcValPtr)--;
  }

  return EFI_SUCCESS;
}

/**
 Decode the block data from the image by DC coefficients with Huffman coding using successive approximation.

 @param[out]     BlockBuff          The pointer of the block's decoded data
 @param[in]      DcAcHTIndex        The indexes of this block's AC, DC Huffman table
 @param[out]     IsEnd              The image is end or not
 @param[out]     DecoderStatus      The detail status of the block decoding
 @param[in, out] DecoderBlockData   Pointer to decoder block data

 @retval EFI_SUCCESS                The block decoded successfully
**/
EFI_STATUS
BlockDecodeDcSuccHuff (
  OUT INT16                              *BlockBuff,
  IN  UINT8                              DcAcHTIndex,
  OUT BOOLEAN                            *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS            *DecoderStatus,
  IN  EFI_JPEG_DECODER_BLOCK_DATA        *DecoderBlockData
  )
{
  *BlockBuff |= GetNextBit(IsEnd) << mJfifData.SosData.Al;
  return EFI_SUCCESS;
}

/**
 Decode the block data from the image by DC coefficients with Huffman coding.

 @param[out]     BlockBuff          The pointer of the block's decoded data
 @param[in]      DcAcHTIndex        The indexes of this block's AC, DC Huffman table
 @param[out]     IsEnd              The image is end or not
 @param[out]     DecoderStatus      The detail status of the block decoding
 @param[in, out] DecoderBlockData   Pointer to decoder block data

 @retval EFI_SUCCESS                The block decoded successfully
 @retval Other                      The block decoding is failed, detail status refers "DecoderStatus"
**/
EFI_STATUS
BlockDecodeDcHuff (
  OUT INT16                              *BlockBuff,
  IN  UINT8                              DcAcHTIndex,
  OUT BOOLEAN                            *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS            *DecoderStatus,
  IN  EFI_JPEG_DECODER_BLOCK_DATA        *DecoderBlockData
  )
{
  UINT8                                  HuffTableIndex;
  EFI_STATUS                             Status;
  UINT8                                  ZeroCount;
  INT16                                  DecodedVal;

  HuffTableIndex = DcAcHTIndex >> 4;
  Status = ElementDecode (HuffTableIndex, &ZeroCount, &DecodedVal, IsEnd, DecoderStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *BlockBuff = DecodedVal;

  BlockBuff[0] = BlockBuff[0] + *(DecoderBlockData->DcValPtr);
  *(DecoderBlockData->DcValPtr) = BlockBuff[0];
  return EFI_SUCCESS;
}

/**
 Decode the block data from the image.

 @param [out]     BlockBuff          The pointer of the block's decoded data
 @param [in]      DcAcHTIndex        The indexes of this block's AC, DC Huffman table
 @param [out]     IsEnd              The image is end or not
 @param [out]     DecoderStatus      The detail status of the block decoding
 @param [in, out] DecoderBlockData   Pointer to decoder block data

 @retval EFI_SUCCESS            The block decoded successfully
 @retval EFI_UNSUPPORTED        The block decoding is failed, detail status refers "DecoderStatus"

**/
EFI_STATUS
BlockDecode (
     OUT    INT16                        *BlockBuff,
  IN        UINT8                        DcAcHTIndex,
     OUT    BOOLEAN                      *IsEnd,
     OUT    EFI_JPEG_DECODER_STATUS      *DecoderStatus,
  IN OUT    EFI_JPEG_DECODER_BLOCK_DATA  *DecoderBlockData
  )
{
  EFI_STATUS          Status;
  UINT8               Count;
  UINT8               HTIndex;
  UINT8               ZeroCount;
  INT16               DecodedVal;

  Count = 0;
  //
  //DC decode
  //
  HTIndex = DcAcHTIndex >> 4;
  Status=ElementDecode (HTIndex, &ZeroCount, &DecodedVal, IsEnd, DecoderStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  BlockBuff[Count++] = DecodedVal;

  //
  //AC decode
  //
  while (Count<64)
  {
    HTIndex = (DcAcHTIndex & 0x0F) + JPEG_HUFFMAN_TABLE_AC_BASE;
    Status = ElementDecode (HTIndex, &ZeroCount, &DecodedVal, IsEnd, DecoderStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if (DecodedVal == 0) {
      if (ZeroCount == 15) {
        Count += 16;
      } else {
        Count = 64;
      }
    } else {
      Count = Count + ZeroCount;
      if (Count > 63) {
        *DecoderStatus = EFI_JPEG_DECODEAC_ERROR;
        return EFI_UNSUPPORTED;
      }
      BlockBuff[Count++] = DecodedVal;
    }
  }

  BlockBuff[0] = BlockBuff[0] + *(DecoderBlockData->DcValPtr);
  *(DecoderBlockData->DcValPtr) = BlockBuff[0];
  return EFI_SUCCESS;
}

/**
 Restart interval marker.

 @param[out]  IsEnd            The image is end or not
 @param[out]  DecoderStatus    The detail status of the MCU decoding

 @retval EFI_SUCCESS           Restart interval marker successfully
 @retval EFI_UNSUPPORTED       File is end
**/
EFI_STATUS
RestartIntervalMarker (
  OUT BOOLEAN                      *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS      *DecoderStatus
  )
{
  if ((mDecoderData.ImagePtr + 1 >= mJfifData.EndImagePtr) ||
      *(mDecoderData.ImagePtr) != 0xFF ||
      *(mDecoderData.ImagePtr + 1) > 0xD7 ||
      *(mDecoderData.ImagePtr + 1) < 0xD0) {
    *DecoderStatus = EFI_JPEG_DECODERST_ERROR;
    return EFI_UNSUPPORTED;
  }

  mDecoderData.ImagePtr += 2;
  mDecoderData.DcVal[0] = 0;
  mDecoderData.DcVal[1] = 0;
  mDecoderData.DcVal[2] = 0;
  mDecoderData.BitPos = 7;
  if (ReadByte()) {
    *IsEnd = TRUE;
    *DecoderStatus = EFI_JPEG_DECODEDATA_ERROR;
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
 Decode the MCU data from the image, get the MCU's blocks data.

 @param [out]  McuDstBuff       The pointer of the MCU's decoded data
 @param [in]   IntervalFlag     When image has RST marker, the flag of the interval restart
 @param [out]  IsEnd            The image is end or not
 @param [out]  DecoderStatus    The detail status of the MCU decoding

 @retval EFI_SUCCESS            The MCU decoded successfully
 @retval EFI_UNSUPPORTED        The MCU decoding is failed, detail status refers "DecoderStatus"

**/
EFI_STATUS
McuRetrieve (
     OUT    INT16                        *McuDstBuff,
  IN        BOOLEAN                      IntervalFlag,
     OUT    BOOLEAN                      *IsEnd,
     OUT    EFI_JPEG_DECODER_STATUS      *DecoderStatus
  )
{
  EFI_STATUS   Status;
  INT16        BlockBuff[64];
  INT16        *McuBuffPtr;
  UINT8        i;

  McuBuffPtr = McuDstBuff;

  if (IntervalFlag) {
    Status = RestartIntervalMarker (IsEnd, DecoderStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  for (i = 0; i < mDecoderData.Blocks; i++) {
    SetMem (&BlockBuff, sizeof(BlockBuff), 0);
    Status = BlockDecode (
               BlockBuff,
               mDecoderData.BlocksInMcu[i].DcAcHTIndex,
               IsEnd,
               DecoderStatus,
               &mDecoderData.BlocksInMcu[i]
               );
    if (EFI_ERROR (Status)) {
    	return Status;
    }

    CopyMem(McuBuffPtr, BlockBuff, 128);
    McuBuffPtr += 64;
  }

  return EFI_SUCCESS;
}

/**
 Retrieve the MCU data from blocks buffer.

 @param[in]  McuIndex           The MCU index
 @param[out] McuDstBuff         The pointer of the MCU's decoded data

 @retval EFI_SUCCESS            Retrieve the MCU data successfully
 @retval EFI_UNSUPPORTED        The MCU decoding is failed, detail status refers "DecoderStatus"
**/
EFI_STATUS
McuRetrieveFromBlocksBuffer (
  IN  UINTN                   McuIndex,
  OUT INT16                   *McuDstBuff
  )
{
  UINTN                       McuY;
  UINTN                       McuX;
  UINTN                       SampleIndex;
  EFI_JPEG_SAMPLE_DATA        *SamplesData;
  INT16                       *McuBuffPtr;
  DATA_UNIT                   *DataUnit;
  UINTN                       Y;
  UINTN                       X;

  if (mJfifData.Sof0Data.McuWidth == 0) {
    return EFI_NOT_FOUND;
  }

  McuY = McuIndex / mJfifData.Sof0Data.McuWidth;
  McuX = McuIndex % mJfifData.Sof0Data.McuWidth;

  McuBuffPtr = McuDstBuff;
  for (SampleIndex = 0; SampleIndex < mJfifData.Sof0Data.Components; SampleIndex++) {
    SamplesData = &mJfifData.Sof0Data.Samples[SampleIndex];

    DataUnit = SamplesData->BlocksBuffer +
               McuY * SamplesData->BlocksWidthInBuffer * SamplesData->Vi +
               McuX * SamplesData->Hi;

    for (Y = 0; Y < SamplesData->Vi; Y++) {
      for (X = 0; X < SamplesData->Hi; X++) {
        CopyMem (McuBuffPtr, DataUnit[SamplesData->BlocksWidthInBuffer * Y + X], sizeof(DATA_UNIT));
        McuBuffPtr += 64;
      }
    }
  }
  return EFI_SUCCESS;
}

/**
 Retrieve the MCU data from SOS to blocks buffer.

 @param[in]  McuIndex           The MCU index
 @param[in]  IntervalFlag       When image has RST marker, the flag of the interval restart
 @param[out] IsEnd              The image is end or not
 @param[out] DecoderStatus      The detail status of the MCU decoding

 @retval EFI_SUCCESS            Retrieve the MCU data successfully
 @retval Other                  The MCU decoding is failed, detail status refers "DecoderStatus"
**/
EFI_STATUS
McuRetrieveToBlocksBuffer (
  IN  EFI_JPEG_SOS_BLOCK_DECODE          BlockDecodeFn,
  IN  UINTN                              McuIndex,
  IN  BOOLEAN                            IntervalFlag,
  OUT BOOLEAN                            *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS            *DecoderStatus
  )
{
  EFI_STATUS                             Status;
  UINTN                                  McuY;
  UINTN                                  McuX;
  UINTN                                  BlockIndex;
  INT16                                  *BlockBuff;
  DATA_UNIT                              *DataUnit;
  UINTN                                  Y;
  UINTN                                  X;
  UINT8                                  SampleIndex;
  EFI_JPEG_SAMPLE_DATA                   *SamplesData;

  if (IntervalFlag) {
    Status = RestartIntervalMarker (IsEnd, DecoderStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (mJfifData.Sof0Data.McuWidth == 0) {
    return EFI_NOT_FOUND;
  }

  McuY = McuIndex / mJfifData.Sof0Data.McuWidth;
  McuX = McuIndex % mJfifData.Sof0Data.McuWidth;

  for (SampleIndex = 0, BlockIndex = 0; SampleIndex < mJfifData.SosData.Ns; SampleIndex++) {
    SamplesData = mJfifData.SosData.Samples[SampleIndex];

    DataUnit = SamplesData->BlocksBuffer +
               McuY * SamplesData->BlocksWidthInBuffer * SamplesData->Vi +
               McuX * SamplesData->Hi;

    for (Y = 0; Y < SamplesData->Vi; Y++) {
      for (X = 0; X < SamplesData->Hi; X++, BlockIndex++) {
        BlockBuff = (INT16 *) DataUnit[SamplesData->BlocksWidthInBuffer * Y + X];

        Status = BlockDecodeFn (
                   BlockBuff,
                   mDecoderData.BlocksInMcu[BlockIndex].DcAcHTIndex,
                   IsEnd,
                   DecoderStatus,
                   &mDecoderData.BlocksInMcu[BlockIndex]
                   );
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }
    }
  }
  return EFI_SUCCESS;
}

/**
 Retrieve the Block data from SOS to blocks buffer.

 @param[in]  BlockIndex         The Block index
 @param[in]  IntervalFlag       When image has RST marker, the flag of the interval restart
 @param[out] IsEnd              The image is end or not
 @param[out] DecoderStatus      The detail status of the MCU decoding

 @retval EFI_SUCCESS            Retrieve the MCU data successfully
 @retval Other                  The MCU decoding is failed, detail status refers "DecoderStatus"
**/
EFI_STATUS
BlockRetrieveToBlocksBuffer (
  IN  EFI_JPEG_SOS_BLOCK_DECODE          BlockDecodeFn,
  IN  UINTN                              BlockIndex,
  IN  BOOLEAN                            IntervalFlag,
  OUT BOOLEAN                            *IsEnd,
  OUT EFI_JPEG_DECODER_STATUS            *DecoderStatus
  )
{
  EFI_STATUS                             Status;
  EFI_JPEG_SAMPLE_DATA                   *SamplesData;
  DATA_UNIT                              *DataUnit;

  if (IntervalFlag) {
    Status = RestartIntervalMarker (IsEnd, DecoderStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  SamplesData = mJfifData.SosData.Samples[0];
  if (SamplesData->BlocksWidth == 0) {
    return EFI_NOT_FOUND;
  }
  DataUnit = SamplesData->BlocksBuffer +
             (BlockIndex / SamplesData->BlocksWidth) * SamplesData->BlocksWidthInBuffer +
             (BlockIndex % SamplesData->BlocksWidth);

  BlockIndex = BlockIndex % mDecoderData.Blocks;
  Status = BlockDecodeFn (
             (INT16 *) DataUnit,
             mDecoderData.BlocksInMcu[BlockIndex].DcAcHTIndex,
             IsEnd,
             DecoderStatus,
             &mDecoderData.BlocksInMcu[BlockIndex]
             );
  return Status;
}

/**
 Dequantize and De-Zigzag the block data.

 @param [in]   BlockSrcBuff     The pointer of the source data
 @param [out]  BlockDstBuff     The pointer of the destination data
 @param [in]   BlockIndex       The index of the block in the MCU

 @retval EFI_SUCCESS            Dequantize and De-Zigzag the block data successfully.
 @retval EFI_ABORTED            Index value exceed the buffer length or memory pointer is NULL.
**/
EFI_STATUS
BlockDequantDezigzag (
  IN        INT16                        *BlockSrcBuff,
     OUT    INT16                        *BlockDstBuff,
  IN        UINT8                         BlockIndex
  )
{
  UINT8       i,j;
  UINT8       QtIndex;
  UINT8       ZigZagTag;
  INT16       QtValue;
  UINT8       *BlockQtBuffPtr;

  if (BlockIndex >= ARRAY_SIZE (mDecoderData.BlocksInMcu)) {
    return EFI_ABORTED;
  }
  QtValue = 0;
  QtIndex = mDecoderData.BlocksInMcu[BlockIndex].QTIndex;
  if (QtIndex >= ARRAY_SIZE (mJfifData.DqtPtr)) {
    return EFI_ABORTED;
  }
  BlockQtBuffPtr = mJfifData.DqtPtr[QtIndex];
  if (BlockQtBuffPtr == NULL) {
    return EFI_ABORTED;
  }

  for (i = 0; i < 8; i++) {
  	for ( j = 0; j < 8; j++) {
      ZigZagTag = ZigZag[i][j];
      BlockDstBuff[i * 8 + j] = BlockSrcBuff[ZigZagTag] * BlockQtBuffPtr[ZigZagTag];
    }
  }
  return EFI_SUCCESS;
}


/**
 Do the IDCT with the block data, and add the offset(128) to Y component.



**/
VOID
BlockIDctAddoffset (
  IN  OUT   INT16                        *BlockSrcDstBuff,
  IN        UINT8                         BlockIndex
  )
{
  UINT8  Index1;
  UINT8  Index2;
  UINT8  col;
  INT16  Offset;
  INT16  BlockTemp[64];
  INT16  *Src;
  INT16  *Dst;
  INT32  c0;
  INT32  c1;
  INT32  c2;
  INT32  c3;
  INT32  c4;
  INT32  c5;
  INT32  c6;
  INT32  c7;
  INT32  x0;
  INT32  x1;
  INT32  x2;
  INT32  x3;
  INT32  x4;
  INT32  x5;
  INT32  x6;
  INT32  x7;

  Offset = 0;
  c0 = Cos0;
  //
  // Index1 == 0: T=A*[Y], (row of A)*(col of Y)
  //  calculate the values of T row by row, but store the values
  //  col by col, so get the matrix of T' (T' is the inverse matrix of T)
  // Index1 == 1: [X]=T*A'=(A*T')', (row of A)*(col of T')
  //  calculate the values of [X] row by row, but stored col by col
  //
  for (Index1 = 0; Index1 < 2; Index1++) {

    if ( Index1 == 0) {
      Src = (INT16 *)BlockSrcDstBuff;
      Dst = (INT16 *)BlockTemp;
    } else {
      Src = (INT16 *)BlockTemp;
      Dst = (INT16 *)BlockSrcDstBuff;
      if (BlockIndex < mJfifData.Sof0Data.Samples[0].Blocks) {
        //
        //this compenent is Y, and need to add 128 offset
        //
        Offset = 128;
      }
    }

    for (Index2 = 0; Index2 < 4; Index2++) {
      switch (Index2) {
        case 0:
          c1 = Cos1;
          c2 = Cos2;
          c3 = Cos3;
          c4 = Cos4;
          c5 = Cos5;
          c6 = Cos6;
          c7 = Cos7;
          break;
        case 1:
          c1 =  Cos3;
          c2 =  Cos6;
          c3 = -Cos7;
          c4 = -Cos4;
          c5 = -Cos1;
          c6 = -Cos2;
          c7 = -Cos5;
          break;
        case 2:
          c1 =  Cos5;
          c2 = -Cos6;
          c3 = -Cos1;
          c4 = -Cos4;
          c5 =  Cos7;
          c6 =  Cos2;
          c7 =  Cos3;
          break;
        default:
          c1 =  Cos7;
          c2 = -Cos2;
          c3 = -Cos5;
          c4 =  Cos4;
          c5 =  Cos3;
          c6 = -Cos6;
          c7 = -Cos1;
      }
      //
      //get the values of Dst's col(j) and col(7-j)
      //
      for (col = 0; col <8; col++) {
        x0 = c0 * Src[col];
        x1 = c1 * Src[8 + col];
        x2 = c2 * Src[16 + col];
        x3 = c3 * Src[24 + col];
        x4 = c4 * Src[32 + col];
        x5 = c5 * Src[40 + col];
        x6 = c6 * Src[48 + col];
        x7 = c7 * Src[56 + col];

        Dst[col*8 + Index2] = (INT16)(((x0 + x2 + x4 + x6) + (x1 + x3 + x5 + x7)) >> 12) + Offset;
        Dst[col*8 + (7 - Index2)] = (INT16)(((x0 + x2 + x4 + x6) - (x1 + x3 + x5 + x7)) >> 12) + Offset;
      }
    }
  }
}


/**
 Decode the MCU data.

 @param [in]   McuSrcBuff       The pointer of the source data
 @param [out]  McuDstBuff       The pointer of the destination data

 @retval EFI_SUCCESS            Decode the MCU data successfully.
 @retval Others                 Fail to dequantize.
**/
EFI_STATUS
McuDecode (
  IN        INT16                        *McuSrcBuff,
     OUT    INT16                        *McuDstBuff
  )
{
  UINT8     Index1;
  UINT8     Index2;
  UINT8     Index3;
  UINT8     ComponentId;
  UINT8     HIndex;
  UINT8     VIndex;
  UINT16    TempValue;
  INT16     *BlockPtr;
  INT16     McuTempBuff[10*64];
  INT16     *McuTempBuffPtr;
  INT16     *McuDstBuffPtr;
  EFI_STATUS Status;

  McuDstBuffPtr = NULL;
  McuTempBuffPtr = (INT16 *)&McuTempBuff;

  for (Index1 = 0; Index1 < mDecoderData.Blocks; Index1++) {
    Status = BlockDequantDezigzag (McuSrcBuff + Index1 * 64, McuTempBuffPtr + Index1 * 64, Index1);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    BlockIDctAddoffset (McuTempBuffPtr + Index1 * 64, Index1);

    if ( Index1 < mJfifData.Sof0Data.Samples[0].Blocks ) {
      McuDstBuffPtr = McuDstBuff;
      ComponentId = 0;
    } else if (Index1 < (mJfifData.Sof0Data.Samples[0].Blocks + mJfifData.Sof0Data.Samples[1].Blocks)) {
      McuDstBuffPtr = McuDstBuff + 4*64;
      ComponentId = 1;
    } else {
      McuDstBuffPtr = McuDstBuff + 8*64;
      ComponentId = 2;
    }
    HIndex = mDecoderData.BlocksInMcu[Index1].HiViIndex >> 4;
    VIndex = mDecoderData.BlocksInMcu[Index1].HiViIndex & 0x0F;
    BlockPtr = McuTempBuffPtr + Index1 * 64;
    for (Index2 = 0; Index2 < 8; Index2++) {
      for(Index3 = 0; Index3 < 8; Index3++) {
        TempValue = (VIndex * 8 + Index2) * mJfifData.Sof0Data.Samples[ComponentId].Hi * 8 \
                     + HIndex * 8 \
                     + Index3;
        McuDstBuffPtr[TempValue] = *BlockPtr;
        BlockPtr++;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 Store the MCU data into the UGA buffer.

 @param [in]   McuSrcBuff       The pointer of the source data
 @param [in, out] DstBuff       The pointer of the destination data
 @param [in, out] CurHPixel     The current horizontal pixel index of the MCU
 @param [in, out] CurVLine      The current vertical line index of the MCU


**/
VOID
StoreMcuToUgaBlt (
  IN        INT16                        *McuSrcBuff,
  IN  OUT   UINT8                        **DstBuff,
  IN  OUT   UINT16                       *CurHPixel,
  IN  OUT   UINT16                       *CurVLine
  )
{
  INT16             Y;
  INT16             Cb;
  INT16             Cr;
  INT16             R;
  INT16             G;
  INT16             B;
  UINT16            TempIndex;
  UINT8             HiMax;
  UINT8             Index1;
  UINT8             Index2;
  UINT8             ViMax;
  UINT8             YHiRate;
  UINT8             CbHiRate;
  UINT8             CrHiRate;
  UINT8             YViRate;
  UINT8             CbViRate;
  UINT8             CrViRate;
  EFI_UGA_PIXEL     *BltBuffer;
  EFI_UGA_PIXEL     *Blt;

  HiMax = GetMax(
            GetMax(mJfifData.Sof0Data.Samples[0].Hi, mJfifData.Sof0Data.Samples[1].Hi),
            mJfifData.Sof0Data.Samples[2].Hi
            );
  ViMax = GetMax(
            GetMax(mJfifData.Sof0Data.Samples[0].Vi, mJfifData.Sof0Data.Samples[1].Vi),
            mJfifData.Sof0Data.Samples[2].Vi
            );
  YHiRate  = GetRate(HiMax, mJfifData.Sof0Data.Samples[0].Hi);
  CbHiRate = GetRate(HiMax, mJfifData.Sof0Data.Samples[1].Hi);
  CrHiRate = GetRate(HiMax, mJfifData.Sof0Data.Samples[2].Hi);
  YViRate  = GetRate(ViMax, mJfifData.Sof0Data.Samples[0].Vi);
  CbViRate = GetRate(ViMax, mJfifData.Sof0Data.Samples[1].Vi);
  CrViRate = GetRate(ViMax, mJfifData.Sof0Data.Samples[2].Vi);

  BltBuffer = (EFI_UGA_PIXEL *)*DstBuff;
  for (Index1 = 0; Index1 < ViMax * 8; Index1++) {
    if ((*CurVLine + Index1) < mJfifData.Sof0Data.Height) {
      Blt = &BltBuffer[(*CurVLine + Index1)*mJfifData.Sof0Data.Width + *CurHPixel];
      for (Index2 = 0; Index2 < HiMax * 8; Index2++) {
        if ((*CurHPixel + Index2) < mJfifData.Sof0Data.Width) {

          TempIndex = GetRate(Index1, YViRate) * 8 * mJfifData.Sof0Data.Samples[0].Hi \
                        + GetRate(Index2, YHiRate);
          Y = McuSrcBuff[TempIndex];

          TempIndex = 4 * 64 \
                        + GetRate (Index1, CbViRate) * 8 * mJfifData.Sof0Data.Samples[1].Hi \
                        + GetRate (Index2, CbHiRate);
          Cb = McuSrcBuff[TempIndex];

          TempIndex = 8 * 64 \
                        + GetRate (Index1, CrViRate) * 8 * mJfifData.Sof0Data.Samples[2].Hi \
                        + GetRate (Index2, CrHiRate);
          Cr = McuSrcBuff[TempIndex];

          R = (Y + ((1436 * Cr)>> 10));
          G = (Y - ((352 * Cb + 731 * Cr) >> 10));
          B = (Y + ((1815 * Cb)>> 10));

          Blt->Red = (UINT8)GetRGB(R);
          Blt->Green = (UINT8)GetRGB(G);
          Blt->Blue = (UINT8)GetRGB(B);

          Blt++;
        } else {
          break;
   		  }
      }
    } else {
      break;
    }
  }

  //
  // Modify the current Horizontal Pixel and Vertical Pixel
  //
  *CurHPixel += HiMax * 8;
  if (*CurHPixel >= mJfifData.Sof0Data.Width) {
    *CurHPixel = 0;
    *CurVLine += ViMax * 8;
  }
}
