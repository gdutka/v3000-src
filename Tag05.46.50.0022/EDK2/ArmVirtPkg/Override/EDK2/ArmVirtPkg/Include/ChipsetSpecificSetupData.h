/** @file
  Chipset Setup Configuration Data

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifdef _IMPORT_CHIPSET_SPECIFIC_SETUP_

//==========================================================================================================
//==========================================================================================================
//----------------------------------------------------------------------------------------------------------
// Start area for Chipset team use. The following area is used by Chipset team to modify.
// The total size of variable in this part are fixed (800 bytes). That means if you need to add or remove
// variables, please modify the ChipsetRSV buffer size as well.
//----------------------------------------------------------------------------------------------------------
  UINT8   ChipsetRSV[800];                                             //Offset 429 - 1199
//----------------------------------------------------------------------------------------------------------
// End of area for Chipset team use.
//----------------------------------------------------------------------------------------------------------
#endif
