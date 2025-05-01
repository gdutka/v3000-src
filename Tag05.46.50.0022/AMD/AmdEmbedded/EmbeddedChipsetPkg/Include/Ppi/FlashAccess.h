/** @file
  Flash Access PPI definition

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


#ifndef _FLASH_ACCESS_PPI_H_
#define _FLASH_ACCESS_PPI_H_

#define FLASH_ACCESS_PPI_GUID \
  { \
    0x68412024, 0x80ca, 0x458c, {0x8d, 0x50, 0xa8, 0xa8, 0x72, 0x40, 0x99, 0xaf } \
  }

typedef struct _FLASH_ACCESS_PPI FLASH_ACCESS_PPI;

typedef
UINT32
(EFIAPI *PSP_GET_FLASH_BLOCK_SIZE) (
  OUT UINTN                                       *BlockSize
  );

typedef
EFI_STATUS
(EFIAPI *PSP_READ_FLASH) (
  IN UINTN                                        FlashAddress,
  IN OUT UINTN                                    *NumBytes,
  IN UINT8                                        *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *PSP_WRITE_FLASH) (
  IN UINTN                                        FlashAddress,
  IN OUT UINTN                                    *NumBytes,
  IN UINT8                                        *Buffer
  );


typedef
EFI_STATUS
(EFIAPI *PSP_ERASE_FLASH) (
  IN UINTN                                        FlashAddress,
  IN OUT UINTN                                    *NumBytes
  );

typedef
EFI_STATUS
(EFIAPI *PSP_RPMC_INC_MC) (
  IN UINT8   CounterAddr,
  IN UINT32  CounterData,
  IN UINT8   *Signature
  );

typedef
EFI_STATUS
(EFIAPI *PSP_RPMC_REQ_MC) (
  IN   UINT8   CounterAddr,
  IN   UINT8   *Tag,
  IN   UINT8   *Signature,
  IN  OUT UINT32  *CounterData,
  IN  OUT UINT8   *OutputSignature
  );

struct _FLASH_ACCESS_PPI {
  PSP_GET_FLASH_BLOCK_SIZE    PspGetFlashBlockSize;
  PSP_READ_FLASH              PspReadFlash;
  PSP_WRITE_FLASH             PspWriteFlash;
  PSP_ERASE_FLASH             PspEraseFlash;
  PSP_RPMC_INC_MC             PspRpmcIncMc;
  PSP_RPMC_REQ_MC             PspRpmcReqMc;
};

extern EFI_GUID gFlashAccessPpiGuid;

#endif
