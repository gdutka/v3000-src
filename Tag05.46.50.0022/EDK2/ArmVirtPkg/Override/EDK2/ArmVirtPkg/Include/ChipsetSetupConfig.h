/** @file
  Chipset Setup Configuration Definitions

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

#ifndef _CHIPSET_SETUP_CONFIG_H_
#define _CHIPSET_SETUP_CONFIG_H_
#ifndef VFRCOMPILE
#include <Uefi/UefiInternalFormRepresentation.h>
#endif
#include <KernelSetupConfig.h>

#pragma pack(1)
//
//  Setup Utility Structure
//
//-----------------------------------------------------------------------------------------------------------------
// Important!!! The following setup utility structure should be syncronize with OperationRegion MBOX in mailbox.asi.
// If you do NOT follow it, you may face on unexpected issue. The total size are 1200 bytes.
// (Kernel 230 bytes + OEM 70 bytes + ODM 100 bytes + Chipset 800 bytes)
//-----------------------------------------------------------------------------------------------------------------

typedef struct {
  //
  // Kernel system configuration (offset 0~229, total 230 bytes)
  //
  #define _IMPORT_KERNEL_SETUP_
  #include <KernelSetupData.h>
  #undef _IMPORT_KERNEL_SETUP_

  // OEM_Start
  // Offset 230~299, total 70 bytes;     // This area must sync to SetupConfig.h
  UINT8         OEMRSV[70];                       // Reserve for OEM team
  // OEM_End

  // ODM_Start
  // Offset 300~399, total 100 bytes;    // This are must sync to SetupConfig.h
  UINT8         ODMRSV[100];                      // Reserve for ODM
  // ODM_End

  //
  // Chipset system configuration (offset 400~1199, total 800 bytes)
  //
  #define _IMPORT_CHIPSET_SPECIFIC_SETUP_
  #include <ChipsetSpecificSetupData.h>
  #undef _IMPORT_CHIPSET_SPECIFIC_SETUP_

} CHIPSET_CONFIGURATION;
#pragma pack()

#endif
