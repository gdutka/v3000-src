/** @file
  Include file for RecoveryFchInit Peim.

;******************************************************************************
;* Copyright (c) 2013 - 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _RECOVERY_FCH_INIT_PEI_H_
#define _RECOVERY_FCH_INIT_PEI_H_
#include <PiPei.h>

#include <IndustryStandard/Pci.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <Library/FabricResourceManagerLib.h>
#include  <Ppi/EndOfPeiPhase.h>
//[-start-220823-IB14740257-add]//
#include <Library/DeviceInfo2Lib.h>
#include <Library/PrintLib.h>
//[-end-220823-IB14740257-add]//
#define CLASSCODE_REGISTER              0x08

EFI_STATUS
RecoveryInitGppPortDevice (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN OUT UINT32                  *MmioResource
  );

#endif
