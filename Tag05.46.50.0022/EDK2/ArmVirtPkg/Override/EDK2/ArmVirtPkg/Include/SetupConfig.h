/** @file

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

#ifndef _SETUP_CONFIG_H_
#define _SETUP_CONFIG_H_
#ifndef VFRCOMPILE
#include <Uefi/UefiInternalFormRepresentation.h>
#endif
#include <ChipsetSetupConfig.h>

#pragma pack(1)

//
//  Setup Utility Structure
//
//-----------------------------------------------------------------------------------------------------------------
// Important!!! The following setup utility structure should be syncronize with OperationRegion MBOX in mailbox.asi.
// If you do NOT follow it, you may face on unexpected issue. The total size are 2633bytes.
// (Kernel 230bytes + OEM 70bytes + ODM 100bytes + Chipset 2233bytes)
//-----------------------------------------------------------------------------------------------------------------

typedef struct {
  //
  // Kernel system configuration (offset 0~229, total 230 bytes)
  //
  #define _IMPORT_KERNEL_SETUP_
  #include <KernelSetupData.h>
  #undef _IMPORT_KERNEL_SETUP_

//==========================================================================================================
//==========================================================================================================
//----------------------------------------------------------------------------------------------------------
// Start area for OEM team. The following area is used by OEM team to modify.
// The total size of variable in this part are fixed (70bytes). That means if you need to add or remove
// variables, please modify the OEMRSV buffer size as well.
//
// Waring!!!==>This area size must sync to ChipsetSetupConfig.h
//----------------------------------------------------------------------------------------------------------
//OEM_Start
//Offset(230);
  UINT8         OEMRSV[70];                       // Reserve for OEM team
//OEM_End
//----------------------------------------------------------------------------------------------------------
// End of area for OEM team use.
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
// Start area for ODM team. The following area is used by ODM to modify.
// The total size of variable in this part are fixed (100bytes). That means if you need to add or remove
// variables, please modify the ODMRSV buffer size as well.
//
// Waring!!!==>This area size must sync to ChipsetSetupConfig.h
//----------------------------------------------------------------------------------------------------------
//ODM_Start
//Offset(300);
  UINT8         ODMRSV[100];                      // Reserve for ODM
//OEM_End
//----------------------------------------------------------------------------------------------------------
// End of area for ODM team use.
//----------------------------------------------------------------------------------------------------------

  //
  // Chipset system configuration (offset 400~2632, total 2233bytes)
  //
  #define _IMPORT_CHIPSET_SPECIFIC_SETUP_
  #include <ChipsetSpecificSetupData.h>
  #undef _IMPORT_CHIPSET_SPECIFIC_SETUP_

} SYSTEM_CONFIGURATION;
#pragma pack()


#define C_ASSERT(expn) typedef char __C_ASSERT__[(expn)?1:-1]

#endif
