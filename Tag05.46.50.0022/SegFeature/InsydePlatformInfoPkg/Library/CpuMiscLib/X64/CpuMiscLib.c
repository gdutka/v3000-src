/** @file
  Implementation of CpuMiscLib in X64.

;******************************************************************************
;* Copyright (c) 2021 - 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <Uefi.h>
#include <Register/Amd/Cpuid.h>
#include <Register/Intel/Cpuid.h>

#include <Library/BaseLib.h>
#include <Library/CpuMiscLib.h>

//
// Hygon processors
//
#define CPUID_SIGNATURE_GENUINE_HYGON_EBX  SIGNATURE_32 ('H', 'y', 'g', 'o')
#define CPUID_SIGNATURE_GENUINE_HYGON_EDX  SIGNATURE_32 ('n', 'G', 'e', 'n')
#define CPUID_SIGNATURE_GENUINE_HYGON_ECX  SIGNATURE_32 ('u', 'i', 'n', 'e')

UINT8
EFIAPI
IdentifyManufacture (
  VOID
  )
{
  EFI_CPUID_REGISTER          Cpu = {0};

  AsmCpuid (
    CPUID_SIGNATURE,
    NULL,
    &(Cpu.RegEbx),
    &(Cpu.RegEcx),
    &(Cpu.RegEdx)
    );

  if (Cpu.RegEbx == CPUID_SIGNATURE_GENUINE_INTEL_EBX &&
      Cpu.RegEdx == CPUID_SIGNATURE_GENUINE_INTEL_EDX &&
      Cpu.RegEcx == CPUID_SIGNATURE_GENUINE_INTEL_ECX) {
    return ProcessorManufacturerIntel;
  } else if (
     (Cpu.RegEbx == CPUID_SIGNATURE_AUTHENTIC_AMD_EBX &&
      Cpu.RegEdx == CPUID_SIGNATURE_AUTHENTIC_AMD_EDX &&
      Cpu.RegEcx == CPUID_SIGNATURE_AUTHENTIC_AMD_ECX) ||
     (Cpu.RegEbx == CPUID_SIGNATURE_GENUINE_HYGON_EBX &&
      Cpu.RegEdx == CPUID_SIGNATURE_GENUINE_HYGON_EDX &&
      Cpu.RegEcx == CPUID_SIGNATURE_GENUINE_HYGON_ECX)) {
    return ProcessorManufacturerAmdHygon;
  } else {
    return ProcessorManufacturerUnknown;
  }
}
