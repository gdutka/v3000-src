/** @file
  The instance of JPEG Decode Library

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

#include "JfifDecode.h"
#include "McuDecode.h"

EFI_JPEG_JFIF_DATA      mJfifData;
EFI_JPEG_DECODER_DATA   mDecoderData;


/**
 Decodes SOS data into blocks buffer.

 @retval EFI_SUCCESS         Decodes SOS data successfully.
**/
STATIC
EFI_STATUS
DecodeSosData (
  VOID
  )
{
  BOOLEAN                    IntervalFlag;
  BOOLEAN                    IsEnd;
  UINTN                      Count;
  EFI_JPEG_DECODER_STATUS    DecodeStatus;
  EFI_JPEG_SOS_BLOCK_DECODE  BlockDecodeFn;

  if (mJfifData.SosData.Ss != 0) {
    if (mJfifData.SosData.Ah != 0) {
      BlockDecodeFn = BlockDecodeAcSuccHuff;
    } else {
      BlockDecodeFn = BlockDecodeAcHuff;
    }
  } else {
    if (mJfifData.SosData.Se != 0) {
      BlockDecodeFn = BlockDecode;
    } else {
      if (mJfifData.SosData.Ah != 0) {
        BlockDecodeFn = BlockDecodeDcSuccHuff;
      } else {
        BlockDecodeFn = BlockDecodeDcHuff;
      }
    }
  }

  IntervalFlag = FALSE;
  IsEnd = FALSE;
  Count = 0;
  InitDecoderData (FALSE);
  while (!IsEnd) {
    if (mJfifData.SosData.Ns == 3) {
      if (McuRetrieveToBlocksBuffer (BlockDecodeFn, Count++, IntervalFlag, &IsEnd, &DecodeStatus) != EFI_SUCCESS) {
        break;
      }
      if (Count >= mJfifData.Sof0Data.McuCount) {
        break;
      }
    } else {
      if (BlockRetrieveToBlocksBuffer(BlockDecodeFn, Count++, IntervalFlag, &IsEnd, &DecodeStatus) != EFI_SUCCESS) {
        break;
      }
      if (Count >= mJfifData.SosData.Samples[0]->BlocksCount) {
        break;
      }
    }

    if ((mJfifData.McuRestart)&&(Count % mJfifData.McuRestart==0)) {
      IntervalFlag = TRUE;
    } else {
      IntervalFlag = FALSE;
    }
  }

  return EFI_SUCCESS;
}

/**
 Get the SOS(start-of-scan) marker's data and initialize the relative globle parameters.

 @param[in] ImagePtr              The pointer of the SOS marker

 @retval EFI_JPEG_DECODE_SUCCESS  Get the markers data successfully.
 @retval EFI_JPEG_SOF_ERROR       Not have the one or three components info,
                                  The selector for component in scan is more than 3.
**/
STATIC
EFI_JPEG_DECODER_STATUS
GetSosData (
  IN UINT8                   *ImagePtr,
  IN UINTN                   ImageSize
  )
{
  UINT8                      *Ptr;
  UINTN                      Index;
  UINTN                      SampleIndex;

  if (ImageSize < 5) {
    return EFI_JPEG_SOS_ERROR;
  }
  mJfifData.SosData.Ns = ImagePtr[4];
  if (mJfifData.SosData.Ns != 3 && mJfifData.SosData.Ns != 1) {
    return EFI_JPEG_SOS_ERROR;
  }

  if (ImageSize < 5 + ((UINTN) mJfifData.SosData.Ns) * 2 + 3) {
    return EFI_JPEG_SOS_ERROR;
  }
  for (Index = 0, Ptr = ImagePtr + 5; Index < mJfifData.SosData.Ns; Index++, Ptr += 2) {
     for (SampleIndex = 0; SampleIndex < mJfifData.Sof0Data.Components; SampleIndex++) {
       if (mJfifData.Sof0Data.Samples[SampleIndex].Ci == *Ptr) {
         break;
       }
     }
     if (SampleIndex == mJfifData.Sof0Data.Components || *Ptr > 3) {
       return EFI_JPEG_SOS_ERROR;
     }

     mJfifData.SosData.Samples[Index]     = &mJfifData.Sof0Data.Samples[SampleIndex];
     mJfifData.SosData.Samples[Index]->Td = *(Ptr + 1) >> 4;
     mJfifData.SosData.Samples[Index]->Ta = *(Ptr + 1) & 0xF;

     if (mJfifData.SosData.Samples[Index]->Td > 3 ||
         mJfifData.SosData.Samples[Index]->Ta > 3) {
       return EFI_JPEG_SOS_ERROR;
     }
  }
  mJfifData.SosData.Ss  = *Ptr++;
  mJfifData.SosData.Se  = *Ptr++;
  mJfifData.SosData.Ah  = *Ptr >> 4;
  mJfifData.SosData.Al  = *Ptr++ & 0xF;
  mJfifData.SosData.Al2 = 1 << mJfifData.SosData.Al;

  if (mJfifData.Sof0Data.Ptr[1] == JPEG_SOF0) {
    if (mJfifData.SosData.Ss != 0  ||
        mJfifData.SosData.Se != 63 ||
        mJfifData.SosData.Ah != 0  ||
        mJfifData.SosData.Al != 0) {
      return EFI_JPEG_SOS_ERROR;
    }
  }

  return EFI_JPEG_DECODE_SUCCESS;
}

/**
 Get the SOF0(Start Of Frame, Baseline sequential DCT mode) marker's data.
 And initialize the relative globle parameters.


 @retval EFI_JPEG_DECODE_SUCCESS  Get the markers data successfully.
 @retval EFI_JPEG_SOF_ERROR     Not have the three components info,
                                The Sample precision is not 8 bits,
                                Does not have a image components in the frame,
                                The sample rate Hi, Vi is neither 1 nor 2,
                                The blocks number in a MCU is more than 10.

**/
EFI_JPEG_DECODER_STATUS
GetSof0Data (
  IN     UINT8                        *ImagePtr,
  IN     UINTN                        ImageSize
  )
{
  UINT8       Index;
  UINT8       Blocks;
  UINT8       HiMax;
  UINT8       ViMax;
  UINTN       SamplingCount;
  UINTN       BlocksCount;
  DATA_UNIT   *Buffer;
  //
  // ImagePtr[4]: sample precision, baseline sequential just support 8 bits.
  // ImagePtr[9]: number of image components in frame.
  //
  if (ImageSize < 10 || ImagePtr[4] != 8 || ImagePtr[9] == 0 || ImagePtr[9] > 3) {
    return EFI_JPEG_SOF_ERROR;
  }

  mJfifData.Sof0Data.Height = (ImagePtr[5] << 8) + ImagePtr[6];
  mJfifData.Sof0Data.Width = (ImagePtr[7] << 8) + ImagePtr[8];
  mJfifData.Sof0Data.Components = ImagePtr[9];

  Blocks = 0;
  HiMax  = 0;
  ViMax  = 0;
  if ((10 + ((UINTN) ImagePtr[9]) * 3) > ImageSize) {
    return EFI_JPEG_SOF_ERROR;
  }
  for (Index = 0; Index < ImagePtr[9]; Index++) {
    mJfifData.Sof0Data.Samples[Index].Hi = ImagePtr[11 + Index * 3] >> 4;
    mJfifData.Sof0Data.Samples[Index].Vi = ImagePtr[11 + Index * 3] & 0x0F;
    mJfifData.Sof0Data.Samples[Index].Blocks =
      mJfifData.Sof0Data.Samples[Index].Hi * mJfifData.Sof0Data.Samples[Index].Vi;
    mJfifData.Sof0Data.Samples[Index].QuanTable = ImagePtr[11 + Index * 3 + 1];
    mJfifData.Sof0Data.Samples[Index].Ci        = ImagePtr[11 + Index * 3 - 1];
    HiMax = MAX (HiMax, mJfifData.Sof0Data.Samples[Index].Hi);
    ViMax = MAX (ViMax, mJfifData.Sof0Data.Samples[Index].Vi);

    //
    // Just support 4:1:1,4:2:2, 4:2:1, 1:1:1, 2:1:1
    //
    if (mJfifData.Sof0Data.Samples[Index].Blocks != 1
         && mJfifData.Sof0Data.Samples[Index].Blocks != 2
         && mJfifData.Sof0Data.Samples[Index].Blocks != 4) {
      return EFI_JPEG_SOF_ERROR;
    }
    Blocks = Blocks + mJfifData.Sof0Data.Samples[Index].Blocks;
  }
  //
  // In a MCU, the blocks' number must not more than 10.
  //
  if (Blocks > 10) {
    return EFI_JPEG_SOF_ERROR;
  }

  if (ImagePtr[1] == JPEG_SOF2) {
    if (HiMax == 0 || ViMax == 0) {
      return EFI_JPEG_SOF_ERROR;
    }

    mJfifData.Sof0Data.McuWidth  = DIV_CEIL(mJfifData.Sof0Data.Width , HiMax * 8);
    mJfifData.Sof0Data.McuHeight = DIV_CEIL(mJfifData.Sof0Data.Height, ViMax * 8);
    mJfifData.Sof0Data.McuCount  = mJfifData.Sof0Data.McuWidth * mJfifData.Sof0Data.McuHeight;

    for (Index = 0, BlocksCount = 0; Index < mJfifData.Sof0Data.Components; Index++) {
      SamplingCount = DIV_CEIL(mJfifData.Sof0Data.Width  * mJfifData.Sof0Data.Samples[Index].Hi, HiMax);
      mJfifData.Sof0Data.Samples[Index].BlocksWidth  = DIV_CEIL(SamplingCount, 8);

      SamplingCount = DIV_CEIL(mJfifData.Sof0Data.Height * mJfifData.Sof0Data.Samples[Index].Vi, ViMax);
      mJfifData.Sof0Data.Samples[Index].BlocksHeight = DIV_CEIL(SamplingCount, 8);

      mJfifData.Sof0Data.Samples[Index].BlocksCount = mJfifData.Sof0Data.Samples[Index].BlocksWidth * mJfifData.Sof0Data.Samples[Index].BlocksHeight;

      mJfifData.Sof0Data.Samples[Index].BlocksWidthInBuffer = mJfifData.Sof0Data.McuWidth * mJfifData.Sof0Data.Samples[Index].Hi;
      mJfifData.Sof0Data.Samples[Index].BlocksCountInBuffer = mJfifData.Sof0Data.Samples[Index].BlocksWidthInBuffer * mJfifData.Sof0Data.McuHeight * mJfifData.Sof0Data.Samples[Index].Vi;

      BlocksCount += mJfifData.Sof0Data.Samples[Index].BlocksCountInBuffer;
    }

    if (BlocksCount == 0) {
      return EFI_JPEG_SOF_ERROR;
    }
    mJfifData.Sof0Data.BlocksBufferSize = sizeof(DATA_UNIT) * BlocksCount;
    mJfifData.Sof0Data.BlocksBuffer     = H2OImageDecoderLibAllocateMem (mJfifData.Sof0Data.BlocksBufferSize);
    if (mJfifData.Sof0Data.BlocksBuffer == NULL) {
      return EFI_JPEG_SOF_ERROR;
    }
    ZeroMem (mJfifData.Sof0Data.BlocksBuffer, mJfifData.Sof0Data.BlocksBufferSize);

    Buffer = (DATA_UNIT *) mJfifData.Sof0Data.BlocksBuffer;
    for (Index = 0; Index < mJfifData.Sof0Data.Components; Index++) {
      mJfifData.Sof0Data.Samples[Index].BlocksBuffer = Buffer;
      Buffer += mJfifData.Sof0Data.Samples[Index].BlocksCountInBuffer;
    }
  }

  return EFI_JPEG_DECODE_SUCCESS;
}

/**
 Get the HuffmanTable data. And initialize the relative globle parameters.

 @param [in]   ImagePtr         The pointer of the DHT (Definintion of Huffman Table) marker

 @retval EFI_JPEG_DECODE_SUCCESS  Get the Huffman table data successfully.
 @retval EFI_JPEG_HUFFMANTABLE_ERROR  The data of Huffman table is error.

**/
EFI_JPEG_DECODER_STATUS
GetHuffmanTable (
  IN     UINT8                        *ImagePtr,
  IN     UINTN                        ImageSize
  )
{
  UINT8    index1;
  UINT8    index2;
  UINT8    TableIndex;
  UINT8    ValIndex;
  UINT8    *CodesPtr;
  UINT8    *ValPtr;
  UINT8    *TablePtr;
  UINT8    *ImageEnd;
  BOOLEAN  IsZero;
  UINT16   HuffmanCode;
  UINT16   HuffmanValueSize;

  if (ImageSize < 4) {
    return EFI_JPEG_HUFFMANTABLE_ERROR;
  }
  TablePtr = ImagePtr + 4;
  ImageEnd = ImagePtr + MIN (ImageSize, (2 + ((UINTN)ImagePtr[2] << 8) + ImagePtr[3]));
  while(TablePtr < ImageEnd) {
    if (TablePtr + 17 > ImageEnd) {
      return EFI_JPEG_HUFFMANTABLE_ERROR;
    }

    TableIndex = (TablePtr[0] >> 4) * JPEG_HUFFMAN_TABLE_AC_BASE + (TablePtr[0] & 0x0F);
    if (TableIndex >= ARRAY_SIZE(mJfifData.HuffTable)) {
      return EFI_JPEG_HUFFMANTABLE_ERROR;
    }

    mJfifData.HuffTable[TableIndex].Ptr = ImagePtr;
    mJfifData.HuffTable[TableIndex].Number = TablePtr[0] & 0x0F;
    mJfifData.HuffTable[TableIndex].Type = TablePtr[0] >> 4;

    //
    //Initial the layer's ptr of the Huffman tree, CodesPtr[1] is the first Huffman tree's layer.
    //  then the index of the layer is just the bits' number of the layer's huffman code.
    //Initial  the value's ptr of Huffman tree, ValPtr[0] is the first Huffman tree's value
    //
    IsZero = TRUE;
    ValIndex = 0;
    HuffmanCode = 0;
    CodesPtr = TablePtr;
    ValPtr = TablePtr + 17;
    for (index1 = 1, HuffmanValueSize = 0; index1 < 17; index1++) {
      mJfifData.HuffTable[TableIndex].Codes[index1] = CodesPtr[index1];
      HuffmanValueSize += CodesPtr[index1];
    }
    if (ValPtr + HuffmanValueSize > ImageEnd) {
      return EFI_JPEG_HUFFMANTABLE_ERROR;
    }
    for (index1 = 1; index1 < 17; index1++) {
      if (IsZero && CodesPtr[index1] == 0) {
        continue;
      } else if (IsZero) {
        IsZero = FALSE;
      }
      HuffmanCode = HuffmanCode << 1;
      mJfifData.HuffTable[TableIndex].MinCode[index1] = HuffmanCode;
      for ( index2 = 0; index2 < CodesPtr[index1]; index2++, HuffmanCode++) {
        mJfifData.HuffTable[TableIndex].HuffmanVal[ValIndex] = ValPtr[ValIndex];
        ValIndex++;
      }
      mJfifData.HuffTable[TableIndex].MaxCode[index1] =
        mJfifData.HuffTable[TableIndex].MinCode[index1] + CodesPtr[index1] - 1;
      mJfifData.HuffTable[TableIndex].FirstCode[index1] = ValIndex - CodesPtr[index1];
    }
    TablePtr = ValPtr + ValIndex;
  }
  return EFI_JPEG_DECODE_SUCCESS;
}

/**
 Get the important marker data, and store them into the globle parameters,
 which will be used in the decoding.

 @param [in]   ImageData
 @param [in]   ImageDataSize    The length in byte of the JFIF image

 @retval EFI_JPEG_DECODE_SUCCESS  Get the markers data successfully.
 @retval EFI_JPEG_INVALID_PARAMETER  Not have the start marker of SOI,
 @retval EFI_JPEG_SOF_ERROR     The data of SOF is error,
 @retval EFI_JPEG_SOS_ERROR     The data of SOS is error,
 @retval EFI_JPEG_QUANTIZATIONTABLE_ERROR  The data of Quantization table is error,
 @retval EFI_JPEG_HUFFMANTABLE_ERROR  The data of Huffman table is error.

**/
EFI_JPEG_DECODER_STATUS
InitJfifData (
  IN     UINT8                         *ImageData,
  IN     UINTN                         ImageDataSize
  )
{
  EFI_JPEG_DECODER_STATUS    DecodeStatus;
  UINT32                     Index;
  UINT8                      DqtCount;
  UINT8                      *TempPtr;
  UINT8                      *ImagePtr;
  UINT8                      DqtPtrIndex;
  UINTN                      RemainingSize;

  Index = 0;
  ImagePtr  = ImageData;

  SetMem (&mJfifData, sizeof(mJfifData), 0);
  if (ImageDataSize < 2 || ImagePtr[0] != 0xFF || ImagePtr[1] != (UINT8) JPEG_SOI) {
    return EFI_JPEG_INVALID_PARAMETER;
  } else {
    mJfifData.EndImagePtr = ImageData + ImageDataSize;
    mJfifData.SoiPtr = ImagePtr;
    ImagePtr += 2;
  }

  //
  // Initialize the value of mJfifData
  //
  while (ImagePtr < (ImageData + ImageDataSize)) {
    RemainingSize = ImageDataSize - (UINTN) (ImagePtr - ImageData);
    if (RemainingSize < 2) {
      break;
    }

    if (ImagePtr[0] != 0xFF ||(ImagePtr[0] == 0xFF && ImagePtr[1] == 0x00)) {
      ImagePtr++;
      continue;
    }
    //
    // Check this marker is the supporting SOFn (n>0) or not
    //
    if (ImagePtr[1] > 0xc0 && ImagePtr[1] != JPEG_SOF2 && ImagePtr[1] != JPEG_DHT && ImagePtr[1] <= 0xcf) {
      return EFI_JPEG_SOF_ERROR;
    }

    switch (ImagePtr[1]) {
    case JPEG_SOF0:
    case JPEG_SOF2:
      mJfifData.Sof0Data.Ptr = ImagePtr;
      DecodeStatus = GetSof0Data (ImagePtr, RemainingSize);
      if (DecodeStatus != EFI_JPEG_DECODE_SUCCESS) {
        return DecodeStatus;
      }
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_SOS:
      mJfifData.SosPtr = ImagePtr;

      DecodeStatus = GetSosData (ImagePtr, RemainingSize);
      if (DecodeStatus != EFI_JPEG_DECODE_SUCCESS) {
        return DecodeStatus;
      }

      if (mJfifData.Sof0Data.Ptr[1] == JPEG_SOF2) {
        DecodeSosData ();
      }
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_DHT:
      DecodeStatus = GetHuffmanTable (ImagePtr, RemainingSize);
      if (DecodeStatus != EFI_JPEG_DECODE_SUCCESS) {
        return DecodeStatus;
      }
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_DQT:
      //
      // There will be several Quantization tables in one DQT segment
      // About the first Quantization table in the DQT:
      // ImagePtr[4]: the bits of [0:3], Pq(Quantization table element precision)
      // ImagePtr[4]: the bits of [4:7], Tq(Quantization table destination identifier)
      //
      if (RemainingSize < 4) {
        return EFI_JPEG_QUANTIZATIONTABLE_ERROR;
      }
      DqtCount = ((ImagePtr[2] << 8) + ImagePtr[3] - 2)/65;
      TempPtr = ImagePtr + 4;
      if (RemainingSize - 4 < 65 * (UINTN) DqtCount) {
        return EFI_JPEG_QUANTIZATIONTABLE_ERROR;
      }
      for (Index = 0; Index < DqtCount; Index++) {
        DqtPtrIndex = TempPtr[0] & 0x0F;
        if (DqtPtrIndex > 3 || (TempPtr[0] >> 4) != 0) {
          return EFI_JPEG_QUANTIZATIONTABLE_ERROR;
        }
        mJfifData.DqtPtr[DqtPtrIndex] = TempPtr + 1;
        TempPtr += 65;
      }
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_APP0:
      if (RemainingSize < 4) {
        return EFI_JPEG_INVALID_PARAMETER;
      }
      mJfifData.App0Ptr = ImagePtr;
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_APP1:
    case JPEG_APP2:
    case JPEG_APP3:
    case JPEG_APP4:
    case JPEG_APP5:
    case JPEG_APP6:
    case JPEG_APP7:
    case JPEG_APP8:
    case JPEG_APP9:
    case JPEG_APP10:
    case JPEG_APP11:
    case JPEG_APP12:
    case JPEG_APP13:
    case JPEG_APP14:
    case JPEG_APP15:
      if (RemainingSize < 4) {
        return EFI_JPEG_INVALID_PARAMETER;
      }
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_COM:
      if (RemainingSize < 4) {
        return EFI_JPEG_INVALID_PARAMETER;
      }
      mJfifData.ComPtr = ImagePtr;
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_DRI:
      if (RemainingSize < 6) {
        return EFI_JPEG_INVALID_PARAMETER;
      }
      mJfifData.McuRestart = (ImagePtr[4] << 8) + ImagePtr[5];
      ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      break;

    case JPEG_RST0:
    case JPEG_RST1:
    case JPEG_RST2:
    case JPEG_RST3:
    case JPEG_RST4:
    case JPEG_RST5:
    case JPEG_RST6:
    case JPEG_RST7:
      ImagePtr += 2;
      break;

    case JPEG_EOI:
      if (mJfifData.Sof0Data.Ptr == NULL ||
          mJfifData.SosPtr == NULL) {
        return EFI_JPEG_IMAGE_UNSUPPORTED;
      }
      return EFI_JPEG_DECODE_SUCCESS;

    default:
      if (RemainingSize < 4) {
        break;
      }
      if (*(ImagePtr + 2) == 0xFF) {
        ImagePtr += 2;
      } else {
        ImagePtr += 2 + (ImagePtr[2] << 8) + ImagePtr[3];
      }
      break;
    }
  }

  if (mJfifData.Sof0Data.Ptr == NULL ||
      mJfifData.SosPtr == NULL) {
    return EFI_JPEG_IMAGE_UNSUPPORTED;
  }
  return EFI_JPEG_DECODE_SUCCESS;
}

/**
 Using the globle parameter mJfifData to initialize the globle parameter of mDecoderData.

 @param[in] ComponentsFromSofData      If TRUE, sample data are from SOF. Otherwise, sample data are from SOS.
**/
VOID
InitDecoderData (
  IN BOOLEAN                           ComponentsFromSofData
  )
{
  UINT8                                Index1;
  UINT8                                Index2;
  UINT8                                Index3;
  UINT8                                Blocks;
  UINT8                                ComponentsNum;
  EFI_JPEG_SAMPLE_DATA                 *SampleData;
  //
  // Initialize the value of mDecoderData
  //
  mDecoderData.ImagePtr =
    mJfifData.SosPtr + 2 + ((UINTN)mJfifData.SosPtr[2] << 8) + mJfifData.SosPtr[3];
  ReadByte();
  mDecoderData.BitPos = 7;

  //
  //Initialize the blocks infomation in a MCU
  //
  ComponentsNum = ComponentsFromSofData ? mJfifData.Sof0Data.Components : mJfifData.SosData.Ns;
  Blocks = 0;
  for (Index3 = 0; Index3 < ComponentsNum; Index3++) {
    SampleData = ComponentsFromSofData ? &mJfifData.Sof0Data.Samples[Index3] : mJfifData.SosData.Samples[Index3];

    for (Index1 = 0; Index1 < SampleData->Vi; Index1++) {
      for (Index2 = 0; Index2 < SampleData->Hi; Index2++) {
        mDecoderData.BlocksInMcu[Blocks].DcAcHTIndex = *(mJfifData.SosPtr + 6 + Index3 * 2);
        mDecoderData.BlocksInMcu[Blocks].QTIndex = SampleData->QuanTable;
        mDecoderData.BlocksInMcu[Blocks].HiViIndex = (Index2 << 4) + Index1;
        mDecoderData.DcVal[Index3] = 0;
        mDecoderData.BlocksInMcu[Blocks].DcValPtr = &(mDecoderData.DcVal[Index3]);
        Blocks++;
      }
    }
  }
  mDecoderData.Blocks = Blocks;
}

/**
 Decodes a JFIF image into a UGA formatted image,
 and returns the decoded image, image's width and image's height

 @param [in]   This             Protocol instance structure
 @param [in]   ImageData        The data of the JFIF image, which will be decoded
 @param [in]   ImageDataSize    The size in bytes of ImageData
 @param [out]  DecodedData      The decoded data, this output parameter contains
                                a newly allocated memory space, and it is the
                                caller's responsibility to free this memory buffer.
 @param [out]  DecodedDataSize  The size in bytes of DecodedData
 @param [out]  Height           The height of the image's displaying
 @param [out]  Width            The width of the image's displaying
 @param [out]  DecoderStatus    The status of the decoding progress, defined in
                                \Protocol\JpegDecoder\JpegDecoder.h.

 @retval EFI_SUCCESS            The JFIF image is decoded successfully.
 @retval EFI_INVALID_PARAMETER  Either one of ImageData, Width, Height, DecodedDataSize
                                and DecoderStatus is NULL, or ImageDataSize is zero.
 @retval EFI_OUT_OF_RESOURCES   The memory for DecodedData could not be allocated.
 @retval EFI_UNSUPPORTED        The JFIF image can not be decoded, and the detail error info
                                will be returned by the output parameter DecoderStatus.

**/
EFI_STATUS
EFIAPI
H2OHiiJpegDecode (
  IN     UINT8                         *ImageData,
  IN     UINTN                         ImageDataSize,
     OUT UINT8                         **DecodedData,
     OUT UINTN                         *DecodedDataSize,
     OUT UINTN                         *Height,
     OUT UINTN                         *Width,
     OUT EFI_JPEG_DECODER_STATUS       *DecoderStatus
  )
{
  BOOLEAN               IntervalFlag;
  BOOLEAN               IsEnd;
  UINTN                 McuCount;
  INT16                 McuSrcBuff[10*64];
  INT16                 McuDstBuff[12*64];
  UINT16                CurHPixel;
  UINT16                CurVLine;
  UINT64                BltBufferSize;
  EFI_STATUS            Status;

  if (ImageData == NULL
       || ImageDataSize == 0
       || DecodedDataSize == NULL
       || Height == NULL
       || Width == NULL
       || DecoderStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *DecoderStatus = InitJfifData (ImageData, ImageDataSize);
  if (*DecoderStatus != EFI_JPEG_DECODE_SUCCESS) {
    return EFI_UNSUPPORTED;
  }

  if (mJfifData.Sof0Data.Width == 0 || mJfifData.Sof0Data.Height == 0) {
    return EFI_UNSUPPORTED;
  }
  *Width = mJfifData.Sof0Data.Width;
  *Height = mJfifData.Sof0Data.Height;
  BltBufferSize = MultU64x32 ((UINT64) (mJfifData.Sof0Data.Width * mJfifData.Sof0Data.Height), sizeof (EFI_UGA_PIXEL));
  if (BltBufferSize >= 0x100000000LL) {
    return EFI_UNSUPPORTED;
  }
  *DecodedDataSize = (UINTN) BltBufferSize;
  *DecodedData = H2OImageDecoderLibAllocateMem (*DecodedDataSize);
  if (*DecodedData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (*DecodedData, *DecodedDataSize);
  CurHPixel = 0;
  CurVLine = 0;
  IntervalFlag = FALSE;

  IsEnd = FALSE;
  McuCount = 0;
  InitDecoderData(TRUE);
  while (!IsEnd) {
    SetMem(&McuDstBuff, sizeof(McuDstBuff), 0);
    if (mJfifData.Sof0Data.BlocksBuffer != NULL) {
      McuRetrieveFromBlocksBuffer (McuCount++, (INT16 *)&McuSrcBuff);
      if (McuCount >= mJfifData.Sof0Data.McuCount) {
        IsEnd = TRUE;
        H2OImageDecoderLibFreeMem (mJfifData.Sof0Data.BlocksBuffer, mJfifData.Sof0Data.BlocksBufferSize);
      }
    } else {
      if (McuRetrieve((INT16 *)&McuSrcBuff, IntervalFlag, &IsEnd, DecoderStatus) != EFI_SUCCESS) {
        break;
      }
      McuCount++;
      if ((mJfifData.McuRestart)&&(McuCount % mJfifData.McuRestart==0)) {
        IntervalFlag = TRUE;
      } else {
        IntervalFlag = FALSE;
      }
    }
    Status = McuDecode ((INT16 *)&McuSrcBuff, (INT16 *)&McuDstBuff);
    if (EFI_ERROR (Status)) {
      H2OImageDecoderLibFreeMem (*DecodedData, *DecodedDataSize);
      return Status;
    }

    StoreMcuToUgaBlt ((INT16 *)&McuDstBuff, DecodedData, &CurHPixel, &CurVLine);
  }

  return EFI_SUCCESS;
}

/**
 Get a special Marker info of the JFIF image

 @param [in]   Start            The start of the JFIF image,
                                or the output Next parameter from a previous call.
 @param [in]   End              The end of the JFIF image.
 @param [in, out] MarkerType    The type of the marker in the JFIF image
 @param [out]  MarkerData       The pointer of the marker specified by the special MarkerType in the JFIF image.
 @param [out]  DataSize         The size in bytes of MarkerData (with the marker bytes and length bytes).
 @param [out]  Next             The next pointer following the "MarkerType" marker,
                                it is next marker pointer, or the compressed data pointer after SOS marker.

 @retval EFI_SUCCESS            The marker's information is retrieved sucessfully.
 @retval EFI_INVALID_PARAMETER  Either one of Start, End and DataSize is NULL,
                                or End is less than Start.
 @retval EFI_NOT_FOUND          The marker can not be found in the JFIF image.

**/
EFI_STATUS
EFIAPI
H2OHiiJpegDecoderGetMarkerData (
  IN     UINT8                         *Start,
  IN     UINT8                         *End,
  IN OUT EFI_JPEG_MARKER_TYPE          *MarkerType,
     OUT UINT8                         **MarkerData,
     OUT UINT32                        *DataSize,
     OUT UINT8                         **Next  OPTIONAL
  )
{
  UINT8                 *ImagePtr;
  UINT16                i;

  i = 0;
  ImagePtr = Start;
  if (Start == NULL
       || End == NULL
       || End < Start
       || DataSize == NULL) {

    return EFI_INVALID_PARAMETER;
  }

  while (ImagePtr < End) {
    //
    // The ImagePtr is not a Marker's pointer
    //
    if (*ImagePtr != 0xFF || (*ImagePtr == 0xFF && *(ImagePtr + 1) == 0x00)) {
      ImagePtr++;
      continue;
    }

    if (*(ImagePtr + 2) == 0xFF) {
      //
      // The ImagePtr is a marker's pointer, and this marker does not have data info
      //
      *DataSize = 2;
    } else {
      *DataSize = (*(ImagePtr + 2) << 8) + *(ImagePtr + 3) + 2;
    }
    if (*MarkerType == JPEG_ANY) {
      //
      //Find the first marker following the Start pointer which has data info.
      //
      *MarkerType = *(ImagePtr + 1);
      *MarkerData = ImagePtr;
      if (Next != NULL) {
        *Next = ImagePtr + *DataSize;
      }

      return EFI_SUCCESS;
    } else if ( *(ImagePtr + 1) == *MarkerType) {
      //
      // The ImagePtr is a marker's pointer, and this marker have info
      //
      *MarkerData = ImagePtr;
      if (Next != NULL) {
        *Next = ImagePtr + *DataSize;
      }

      return EFI_SUCCESS;
    } else {
      ImagePtr += *DataSize;
    }
  }
  *MarkerData = NULL;
  *DataSize = 0;
  if (Next != NULL) {
    *Next = NULL;
  }

  return EFI_NOT_FOUND;
}
