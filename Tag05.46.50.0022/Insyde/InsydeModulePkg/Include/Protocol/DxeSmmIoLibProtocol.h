/** @file
  A COMBINE_SMM driver for an abstract interface of IO library.

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

#ifndef _H2O_DXE_SMM_IO_LIB_PROTOCOL_H_
#define _H2O_DXE_SMM_IO_LIB_PROTOCOL_H_


#define H2O_DXE_SMM_IO_LIB_PROTOCOL_GUID \
  { \
    0xa01376f8, 0x1929, 0x4f7d, 0x87, 0x27, 0x8c, 0xa7, 0x6f, 0xca, 0x1c, 0xb9 \
  }
typedef struct  _H2O_DXE_SMM_IO_LIB_PROTOCOL  H2O_DXE_SMM_IO_LIB_PROTOCOL;


typedef
UINT64
(EFIAPI *H2O_DXE_SMM_IO_READ) (
  IN UINTN                           Port, 
  IN UINTN                           Size
);

typedef
UINT64
(EFIAPI *H2O_DXE_SMM_IO_WRITE) (
  IN UINTN                           Port, 
  IN UINTN                           Size,
  IN UINTN                           InputData
);

typedef
UINT64
(EFIAPI *H2O_DXE_SMM_MMIO_READ) (
  IN UINTN                           Address, 
  IN UINTN                           Size
);

typedef
UINT64
(EFIAPI *H2O_DXE_SMM_MMIO_WRITE) (
  IN UINTN                           Address, 
  IN UINTN                           Size,
  IN UINT64                           Value
);

typedef
VOID
(EFIAPI *H2O_DXE_SMM_IO_READ_FIFO) (
  IN UINTN                           Port, 
  IN UINTN                           Size,
  IN UINTN                           Count,
  OUT VOID                           *Buffer
);

typedef
VOID
(EFIAPI *H2O_DXE_SMM_IO_WRITE_FIFO) (
  IN UINTN                           Port, 
  IN UINTN                           Size,
  IN UINTN                           Count,  
  IN VOID                            *Buffer
);


struct _H2O_DXE_SMM_IO_LIB_PROTOCOL {
  H2O_DXE_SMM_IO_READ       IoRead;
  H2O_DXE_SMM_IO_WRITE      IoWrite;
  H2O_DXE_SMM_MMIO_READ     MmioRead;
  H2O_DXE_SMM_MMIO_WRITE    MmioWrite;
  H2O_DXE_SMM_IO_READ_FIFO  IoReadFifo;
  H2O_DXE_SMM_IO_WRITE_FIFO IoWriteFifo;
};


extern EFI_GUID gH2ODxeSmmIoLibProtocolGuid;
#endif  
