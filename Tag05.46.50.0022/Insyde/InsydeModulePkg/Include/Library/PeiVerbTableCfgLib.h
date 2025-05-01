/** @file

;******************************************************************************
;* Copyright (c) 2016 - 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _PEI_VERB_TABLE_CONFIG_LIB_H_
#define _PEI_VERB_TABLE_CONFIG_LIB_H_

#include <Ppi/VerbTable.h>

/**
  This function searches fdm extension area if board-id > 0xff,
  or it searches fdm original area. If target not found, it searches
  default item using board-id "0".

  @param[out]     VerbTable    A pointer to the address of common verbtable entry.

  @retval EFI_SUCCESS          Get verbtable successfully.
  @retval EFI_NOT_FOUND        Target not found.
**/
EFI_STATUS
EFIAPI
GetVerbTableEx (
  OUT COMMON_CHIPSET_AZALIA_VERB_TABLE  **VerbTableHeaderDataAddress
  );

/**
  Find matched region by board-id, and contruct a COMMON_CHIPSET_AZALIA_VERB_TABLE.
  If match not found, upate VerbTableHeaderDataAddress to NULL

  @param[out]  VerbTableHeaderDataAddress  A pointer to address of verbtable.

  @retval EFI_SUCCESS          Get verbtable entry successfully.
  @retval EFI_NOT_FOUND        Target not found.
**/
EFI_STATUS
EFIAPI
GetVerbTable (
  OUT COMMON_CHIPSET_AZALIA_VERB_TABLE  **VerbTableHeaderDataAddress
  );
#endif

