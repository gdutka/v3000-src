/** @file
  OEM can provide a way to get replace string for OEM depend.
  It's like DMI Tool but difference.

;******************************************************************************
;* Copyright (c) 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/DxeOemSvcKernelLib.h>
#include <Library/BaseLib.h>
#include <Library/CommonPciLib.h>
#include <Library/PciExpressLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

/**
  This service will be call by each time process SMBIOS string.
  OEM can provide a way to get replace string for OEM depend.
  
  @param[in]  Type           SMBIOS type number.
  @param[in]  FieldOffset    Field offset.
  @param[out] StrLength      String length.
  @param[out] String         String point.
  
  @retval    EFI_UNSUPPORTED       Returns unsupported by default.
  @retval    EFI_SUCCESS           N/A.
  @retval    EFI_MEDIA_CHANGED     The value of OUT parameter is changed. 
**/
EFI_STATUS
OemSvcDxeGetSmbiosReplaceString (
  IN  EFI_SMBIOS_TYPE   Type,
  IN  UINT8             FieldOffset,
  OUT UINTN            *StrLength,
  OUT CHAR8           **String
  )
{
//[-start-220225-IB14740219-remove]//
//  STATIC  CHAR8         MayanRMB[9] = { 'M', 'a', 'y', 'a', 'n', '-', 'R', 'M', 'B'};
//  STATIC  CHAR8         LilacRMB[9] = { 'L', 'i', 'l', 'a', 'c', '-', 'R', 'M', 'B'};
//  UINTN                 StrLenMayanRMB = (UINTN) (sizeof (MayanRMB) / sizeof (CHAR8));
//  UINTN                 StrLenLilacRMB = (UINTN) (sizeof (LilacRMB) / sizeof (CHAR8));
//
//  UINT16                PlatformId;
//
//  if ((Type == EFI_SMBIOS_TYPE_SYSTEM_INFORMATION && (FieldOffset == 0x05)) || ((Type == EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION) && (FieldOffset == 0x05))) {
//    //
//    // Update Product name
//    //
//    PlatformId = PcdGet16 (OriginalPlatformId) & 0xF;
//    switch (PlatformId) {
//      case 0: //Lilac
//        *String = LilacRMB;
//        *StrLength = StrLenLilacRMB;
//        break;
//
//      case 1: //Mayan
//        *String = MayanRMB;
//        *StrLength = StrLenMayanRMB;
//        break;
//      
//      default:
//        *String = MayanRMB;
//        *StrLength = StrLenMayanRMB;      
//        break;
//    }
//    return EFI_SUCCESS;
//  }
//[-end-220225-IB14740219-remove]//
  return EFI_UNSUPPORTED;
}
