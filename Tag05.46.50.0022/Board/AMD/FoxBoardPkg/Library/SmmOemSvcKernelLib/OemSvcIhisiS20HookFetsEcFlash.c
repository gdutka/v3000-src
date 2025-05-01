/** @file
 This function offers an interface to Hook IHISI Sub function AH=20h,function "FetsEcFlash"
;******************************************************************************
;* Copyright (c) 2015, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#include <Protocol/SmmFwBlockService.h>
#include <Library/FlashRegionLib.h>
#include <Library/SmmOemSvcKernelLib.h>

extern EFI_SMM_FW_BLOCK_SERVICE_PROTOCOL   *mSmmFwBlockService;
extern BOOLEAN                              mInPOST;

/**
 This function offers an interface to Hook IHISI Sub function AH=20h,function "FetsEcFlash"
 Function uses to flash EC part.

 @param[in]         FlashingDataBuffer  Double pointer to data buffer.
 @param[in]         SizeToFlash         Data size by bytes want to flash.
 @param[in]         DestBlockNo         Dsstination block number.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter or hook code.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcIhisiS20HookFetsEcFlash (
  IN UINT8          *FlashingDataBuffer,
  IN UINTN          SizeToFlash,
  IN UINT8          DestBlockNo
  )
{
  EFI_STATUS                      Status;
  UINTN                           WriteSize;
  UINT8                           EraseCount;
  UINT8                           WriteCount;
  UINTN                           RomBaseAddress;
  UINTN                           UnsignedRegionBase;
  BOOLEAN                         InUnsignedRegion;
  UINT8                          *WriteDataBuffer;
  
  
  WriteDataBuffer   = FlashingDataBuffer;
  WriteSize         = SizeToFlash;
  RomBaseAddress    = (DestBlockNo * SizeToFlash) | PcdGet32 (PcdH2OFlashFvEcBase);
  
  InUnsignedRegion  = FALSE;
  if (PcdGetBool (PcdSecureFlashSupported)) {
    Status = mSmmFwBlockService->ConvertToSpiAddress(
                                   mSmmFwBlockService,
                                   (UINTN) FdmGetAddressById(&gH2OFlashMapRegionFvGuid, &gH2OFlashMapRegionUnsignedFvGuid, 1),
                                   &UnsignedRegionBase
                                   );
    if (!EFI_ERROR (Status)) {
      //
      // Check Write address is in the Unsigned Region or not
      //
      if ((RomBaseAddress >= UnsignedRegionBase) && \
          ((RomBaseAddress + WriteSize) <= (UnsignedRegionBase + PcdGet32 (PcdEndOfFlashUnsignedFvRegionTotalSize)))) {
        InUnsignedRegion = TRUE;
      }
    }

    if (!mInPOST && !InUnsignedRegion) {
      Status = IHISI_FBTS_WRITE_FAILED;
      goto WriteDone;
    }
  }

  EraseCount      = 0;
  WriteCount      = 0;
  do {
    Status = mSmmFwBlockService->EraseBlocks (
                                   mSmmFwBlockService,
                                   RomBaseAddress,
                                   (UINTN *) &WriteSize
                                   );
    if (!EFI_ERROR (Status)) {
      EraseCount = 0;
      Status = mSmmFwBlockService->Write (
                                     mSmmFwBlockService,
                                     RomBaseAddress,
                                     (UINTN *) &WriteSize,
                                     WriteDataBuffer
                                     );
      if (!EFI_ERROR (Status)) {
        goto WriteDone;
      } else {
        Status = IHISI_FBTS_WRITE_FAILED;
        WriteCount++;
      }
    } else {
      Status = IHISI_FBTS_ERASE_FAILED;
      EraseCount++;
    }
  } while ((EraseCount < 100) && (WriteCount < 100));

WriteDone:
  
  return Status;
}
