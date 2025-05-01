/** @file

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>
#include <Library/IoLib.h>
//[-start-220224-IB14740217-add]//
#include <FchRegs.h>
#include <Library/AmdIdsDebugPrintLib.h>
//[-end-220224-IB14740217-add]//

EFI_STATUS
PeiEntry (
  IN EFI_FFS_FILE_HEADER    *FfsHeader,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
//[-start-220126-IB14740207-modify]//
  MmioWrite8 (ACPI_MMIO_BASE + PMIO_BASE + 0x00, 0xF7); // bit0 enable legacy IO
//[-end-220126-IB14740207-modify]//

//[-start-220224-IB14740217-modify]//
  AmdIdsInitUartIomux ();
  AmdIdsInitUartDivisor();
//[-end-220224-IB14740217-modify]//

  return EFI_SUCCESS;
}

