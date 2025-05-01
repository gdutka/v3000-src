/** @file
  CPU common library definition.

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CPU_MISC_LIB_H_
#define _CPU_MISC_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/SmBios.h>

#if defined (MDE_CPU_IA32) || defined (MDE_CPU_X64)

typedef struct {
  UINT32 RegEax;
  UINT32 RegEbx;
  UINT32 RegEcx;
  UINT32 RegEdx;
} EFI_CPUID_REGISTER;

#endif // defined (MDE_CPU_IA32) || defined (MDE_CPU_X64)

typedef union {
  PROCESSOR_VOLTAGE ProcessorVoltageField;
  UINT8             ProcessorVoltage;
} PROCESSOR_VOLTAGE_CONVERSION;

typedef struct {
  UINT8  Associativity;
  CHAR16 *AssociativityString;
} CACHE_ASSOCIATIVITY_TABLE;

typedef enum {
  ProcessorManufacturerUnknown    = 0x00,
  ProcessorManufacturerIntel      = 0x01,
  ProcessorManufacturerAmdHygon   = 0x02,
  ProcessorManufacturerArm        = 0x03,
} PROCESSOR_MANUFACTURER_DATA;

/**
  Identify manufacture.

  @return   The value from PROCESSOR_MANUFACTURER_DATA.

**/
UINT8
EFIAPI
IdentifyManufacture (
  VOID
  );

#endif
