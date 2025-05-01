/** @file
  Definitions for FCH SPI access Library Instance

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _FCH_MMIO_SPI_ACCESS_H_
#define _FCH_MMIO_SPI_ACCESS_H_

#include <Library/SpiAccessLib.h>

//
// Operation index
//
#define SPI_OPCODE_INDEX_READ_ID        0
#define SPI_OPCODE_INDEX_ERASE          1
#define SPI_OPCODE_INDEX_WRITE          2
#define SPI_OPCODE_INDEX_WRITE_S        3
#define SPI_OPCODE_INDEX_READ           4
#define SPI_OPCODE_INDEX_READ_S         5
#define SPI_OPCODE_INDEX_LOCK           6
#define SPI_OPCODE_INDEX_UNLOCK         7
#define SPI_PREFIX_INDEX_WRITE_EN       0
#define SPI_PREFIX_INDEX_WRITE_S_EN     1

//
// Block lock bit definitions
//
#define SPI_WRITE_LOCK                  0x01
#define SPI_FULL_ACCESS                 0x00

#define MAX_SPI_WIRTE_BYTES_PER_TIME    64

#define MAX_SPI_FIFO_SIZE  (MAX_SPI_WIRTE_BYTES_PER_TIME + 8) // SPIx[C7:80] FIFO[72:0]

#define FLASH_DEVICE_SIZE_128K     0x01
#define FLASH_DEVICE_SIZE_256K     0x02
#define FLASH_DEVICE_SIZE_512K     0x03
#define FLASH_DEVICE_SIZE_1024K    0x04
#define FLASH_DEVICE_SIZE_2048K    0x05
#define FLASH_DEVICE_SIZE_4096K    0x06
#define FLASH_DEVICE_SIZE_8192K    0x07
#define FLASH_DEVICE_SIZE_16384K   0x08
#define FLASH_DEVICE_SIZE_32768K   0x09
#define FLASH_DEVICE_SIZE_65536K   0x0A

#endif
