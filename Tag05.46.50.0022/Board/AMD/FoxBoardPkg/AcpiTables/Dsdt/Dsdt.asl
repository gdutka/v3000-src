/** @file

;*******************************************************************************
;* Copyright (c) 2013, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include "Standard.asl"
#include <IndustryStandard/Acpi.h>
#include <AcpiHeaderDefaultValue.h>

DefinitionBlock (
  "Dsdt.aml",                      // AML file name
  "DSDT",                          // Table signature, DSDT
  FixedPcdGet8 (PcdH2ODsdtRevision),  // Compliance Revision
  "INSYDE",                        // OEM ID
  "H2O BIOS",                      // Table ID
  EFI_ACPI_CREATOR_REVISION
  )                                // OEM Revision
{

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"
  Include("PostCodeDef.asl")
  Include("OpRegDef.asl")
  Include("GloblNvs.asl")
  Include("OemGloblNvs.asl")
  Include("CheckOS.asl")
  Include("Memdbg.asl")
#ifdef MODERN_STANDBY_SUPPORT
 // #include "ModernStandby\PowerResourceDef.asl"
#endif
  //
  //  Processor Objects
  //
  Include("_PR.asl")
  //
  // System Sleep States
  //
  Include("_Sx.asl")
  //
  //  General Purpose Event
  //
  Include("Gpe.asl")
  //
  // for determing PIC mode
  //
  Include("_PIC.asl")
  //
  // System sleep down
  //
  Include("_PTS.asl")
  //
  // System Wake up
  //
  Include("_WAK.asl")
  //
  // System Bus
  //
  Include("_SB.asl")

  //
  // Connected Standby related code
  //
  // To do #include "CnStdby.asl"
  //
  // Fch D3Cold related code
  //
  Include("FchRmb.asi")
  
  //
  // Misc. devices on board
  //
//[-start-220217-IB14740213-remove]//
//  Include("OnBoardI2cUart.asl")
//  Include("OnBoardDevices.asl")
//[-end-220217-IB14740213-remove]//

  // Thermal device
  //Include("Thermal.asl")
}

