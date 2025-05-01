/** @file
  Chipset Configuration Helper Library

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _AMD_MCM_RB_UPDATE_LIB_H_
#define _AMD_MCM_RB_UPDATE_LIB_H_

EFI_STATUS
GetSystemInfoFromFabric (
  OUT UINTN           *NumberOfSockets,
  OUT UINTN           *NumberOfDies,
  OUT UINTN           *NumberOfRootBridges
  );

EFI_STATUS
UpdateNonPciRegisters (
  VOID
  );

#endif

