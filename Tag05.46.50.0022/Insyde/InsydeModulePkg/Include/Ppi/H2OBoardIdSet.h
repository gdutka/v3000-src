/** @file
  The definition of H2O board identifier set PPI.

;******************************************************************************
;* Copyright (c) 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_BOARD_ID_SET_PPI_H_
#define _H2O_BOARD_ID_SET_PPI_H_

#define H2O_BOARD_ID_SET_PPI_GUID \
  { \
    0xcd196962, 0x2e90, 0x4b9c, { 0x91, 0xc1, 0x35, 0x43, 0xea, 0xe0, 0x08, 0x7f } \
  }

#define H2O_BOARD_ID_DEPEX_PPI_GUID \
  { \
    0xfce1829c, 0x9919, 0x11e9, { 0xa2, 0xa3, 0x2a, 0x2a, 0xe2, 0xdb, 0xcc, 0xe3 } \
  }
// One of these PPis is installed immediately after the board identifier has been set.
// There are actually 256 board identifiers, replacing the 00 in the GUID above, immediately before the XX.

extern EFI_GUID gH2OBoardIdSetPpiGuid;
extern EFI_GUID gH2OBoardIdDepexPpiGuid;

#endif
