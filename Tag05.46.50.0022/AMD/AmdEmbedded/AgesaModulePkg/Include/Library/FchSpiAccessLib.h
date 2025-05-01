/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _FCH_SPI_ACCESS_LIB_H_
#define _FCH_SPI_ACCESS_LIB_H_

#define IOHC_NB_SMN_INDEX_2_BIOS             0x00B8
#define IOHC_NB_SMN_DATA_2_BIOS              0x00BC

#define FCH_ROM_INVALID_START_ADDRESS_2      0xFFFFFFFFul
#define FCH_ROM_START_ADDRESS_2              0xFF000000ul
#define FCH_ROM_END_ADDRESS_2                0xFFFFFFFFul

#define FCH_ROM_INVALID_START_ADDRESS_3      0xFFFFFFFFFFFFFFFFull
#define FCH_ROM_START_ADDRESS_3              0xFD00000000ul
#define FCH_ROM_END_ADDRESS_3                0xFD03FFFFFFul

#define FCH_ROM_SMN_START_ADDRESS            0x44000000ul
#define FCH_ROM_SMN_END_ADDRESS              0x47FFFFFFul

#define FCH_ROM_RANGE_MASK                   0xFFFFFFul

#define FCH_ROM_SIZE_16M                     0x1000000
#define FCH_ROM_SIZE_32M                     (2 * FCH_ROM_SIZE_16M)
#define FCH_ROM_SIZE_64M                     (4 * FCH_ROM_SIZE_16M)
#define FCH_ROM_SMN_MAX_BANK                 4
#define FCH_ROM_ROM2_MAX_BANK                4
#define FCH_ROM_ROM3_MAX_BANK                4


#define BYTE0                                0x00
#define BYTE1                                0x01
#define BYTE2                                0x02
#define BYTE3                                0x03
#define BYTE4                                0x04
#define BYTE5                                0x05
#define BYTE6                                0x06
#define BYTE7                                0x07
#define BYTE_MASK                            0xFF
#define BYTE_OFFSET                          0x08

BOOLEAN
FchSpiRomRead (
    IN      UINT32            Offset,
    OUT     UINT8             *Data,
    IN      UINT32            Length
  );

BOOLEAN
FchSpiRomReadEx (
    IN      UINTN             Location,
    OUT     UINT8             *Data,
    IN      UINT32            Length
  );

BOOLEAN
IsRom2Decoded (
  IN       UINT64        Offset
  );

BOOLEAN
GetRom2Info (
  OUT    UINT32  *Base,
  OUT    UINT32  *Size
  );

BOOLEAN
GetRom3Info (
  OUT    UINT64  *Base,
  OUT    UINT32  *Size
  );

UINT32
TransRom2Addr (
  IN     UINT32  HostAddr
  );

UINT32
TransRom3Addr (
  IN     UINT64  HostAddr
  );

#endif

