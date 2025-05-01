/*
*******************************************************************************

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************
*/
#ifndef _SPI_LOCK_SMI_H_
#define _SPI_LOCK_SMI_H_
#include <Uefi.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/FchSmmSwDispatch2.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmVariable.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>


#define MSR_SMM_BASE                   (0xC0010111UL)

EFI_GUID AmdTestVariableGuid  = { 0xaab3d61e, 0xf7f2, 0x11e1, {0xb7, 0x97, 0x2c, 0x41, 0x38, 0x0a, 0x0b, 0xcc} };

//
// Need to check each IBV's SW value setting, can't duplicate define
//
typedef enum {
  TAMPER_PAGE_TABLE_SMI      = 0xD5,
  TAMPER_CPL0_STACK_SMI      = 0xD6,
  TAMPER_CPL0_HEAP_SMI       = 0xD7,
  TAMPER_SMIENTRY_REGION_SMI = 0xD8,
  TAMPER_SMMSUPV_DRIVER_MEM_REGION_SMI = 0xD9,
  TAMPER_GDT_MEM_REGION_SMI  = 0XDA,
  TAMPER_SMM_SAVESTAE_REGION_SMI = 0XDB,
  EXECUTE_PRIVATE_INSTRUCTION_SMI = 0xDC,
  IO_ACCESS_VOLIATE_SECURE_POLICY_SMI = 0xDD,
  MSR_ACCESS_VOLIATE_SECURE_POLICY_SMI = 0xDE,
}TEST_SMI_PORT_TABLE;

VOID
AsmIoWrite8 (
  IN      UINT16        Address,
  IN      UINT8         Data
  );

#endif //_SPI_LOCK_SMI_H_
