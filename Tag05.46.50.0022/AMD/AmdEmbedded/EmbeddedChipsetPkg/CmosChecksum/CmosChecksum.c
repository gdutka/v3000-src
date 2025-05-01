/** @file
 DXE Chipset Services driver.

 This file contains only one function that is CmosChecksum().
 The function CmosChecksum() check whether CMOS 0x10 ~ 0x2F = 0, if so, modify CMOS 0x10 ~ 0x2F = 0xFF.

***************************************************************************
* Copyright (c) 2013, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <Library/CmosLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>

EFI_STATUS
EFIAPI
CmosChecksum (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT8   Address;
  UINT8   Temp;

  // After CMOS clear, the case of CMOS 0x10 ~ 0x2F = 0 results in checksum OK, which causes BIOS setup can't load default.
  // Check whether CMOS 0x10 ~ 0x2F = 0, if so, modify CMOS 0x10 ~ 0x2F = 0xFF
  for (Address = 0x10; Address < 0x30; Address++) {
    Temp = ReadCmos8 (Address);
    if (Temp != 0) break;
  }
  if (Address == 0x30) {
    for (Address = 0x10; Address < 0x30; Address++) {
      Temp = 0xFF;
      WriteCmos8 (Address, Temp);
    }
  }

  return EFI_SUCCESS;
}