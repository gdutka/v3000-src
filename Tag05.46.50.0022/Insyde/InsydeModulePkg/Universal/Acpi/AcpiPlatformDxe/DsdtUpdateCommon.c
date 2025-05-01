/** @file

;******************************************************************************
;* Copyright (c) 2012, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/H2OLib.h>

#include <IndustryStandard/Acpi.h>

/**
  Update Dsdt table content
  For Memdbg.asl

  @param  Table           The table to update

  @return VOID

**/
VOID
DsdtUpdateCommon (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table
)
{
  UINT8               *DsdtPointer;
  UINT32              *Signature;
  
  for (DsdtPointer = (UINT8 *)((UINTN)Table + sizeof (EFI_ACPI_DESCRIPTION_HEADER)); 
                DsdtPointer <= (UINT8 *)((UINTN)Table + (UINTN)(Table->Length)); DsdtPointer++) {
    Signature = (UINT32*) DsdtPointer;
    switch (*Signature) {      
    case (SIGNATURE_32 ('M', 'D', 'B', 'G')):
      if (IsAmlOpRegionObject (DsdtPointer)) {
        UINT32  DbgSize;
        UINT32  *DbgBuffer;
        
        DbgSize = 0x1000 + 4;   // See DBG_SIZE in MemDbg.asl
        DbgBuffer = H2OAcpiNvsAllocate(DbgSize);
        ASSERT (DbgBuffer != NULL);
        SetOpRegion (DsdtPointer, DbgBuffer, DbgSize);
      }
      return;
    }
  }
  return;
  
}

