/** @file
  H2OBoardId define

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

#ifndef _H2O_BOARD_ID_H_
#define _H2O_BOARD_ID_H_

#define H2O_DEFAULT_BOARD_ID  0xFFFFFFFFFFFFFFFF
#define H2O_64_HIGH_BIT_MASK  0xFFFFFFFF00000000

//
//SKU_ID is defined in ...\\EDK2\MdeModulePkg\Include\Guid\PcdDataBaseSignatureGuid.h
//Sync variable type with original define if necessary.
//
#ifndef SKU_ID
typedef UINT64 SKU_ID;
#endif

typedef SKU_ID H2O_BOARD_ID;

#endif
