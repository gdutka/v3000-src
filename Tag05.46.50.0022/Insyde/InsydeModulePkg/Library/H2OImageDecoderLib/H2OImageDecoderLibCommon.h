/** @file
  This library class provides services to decode image.
  
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

#ifndef _H2O_IMAGE_DECODER_LIB_COMMON_H_
#define _H2O_IMAGE_DECODER_LIB_COMMON_H_

#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/H2OImageDecoderLib.h>

VOID *
EFIAPI
H2OImageDecoderLibAllocateMem (
  IN UINTN  AllocationSize
  );

VOID
EFIAPI
H2OImageDecoderLibFreeMem (
  IN VOID   *Buffer,
  IN UINTN  Size
  );

VOID *
EFIAPI
H2OImageDecoderLibAllocateZeroMem (
  IN UINTN  AllocationSize
  );

#endif