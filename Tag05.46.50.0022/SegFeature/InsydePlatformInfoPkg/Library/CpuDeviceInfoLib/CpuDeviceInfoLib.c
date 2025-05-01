/** @file
  Implementation of CpuDeviceInfoLib.

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

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuDeviceInfoLib.h>
#include <Library/CpuMiscLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryDeviceInfoLib.h>
#include <Library/PrintLib.h>

#define MAX_STR_SIZE        0x100
#define CACHE_ASSO_INVALID  0x9

CACHE_ASSOCIATIVITY_TABLE CacheAssociativityTable[] = {
  {0x00, L"Disabled"},
  {0x01, L"1-Way"},
  {0x02, L"2-Way"},
  {0x04, L"4-Way"},
  {0x06, L"8-Way"},
  {0x08, L"16-Way"},
  {0x0A, L"32-Way"},
  {0x0B, L"48-Way"},
  {0x0C, L"64-Way"},
  {0x0D, L"96-Way"},
  {0x0E, L"128-Way"},
  {0x0F, L"Fully"}
};

/**
  The function are output customized current speed string format.

  @param[in]      CurrentSpeed             The current speed value.

  @retval         Current speed string.
**/
CHAR16 *
CpuIdStringFormat (
  IN     UINT32               ProcessorSignature
  )
{
  CHAR16          TempString[MAX_STR_SIZE] = {0};
  UINTN           BaseFamily;
  UINTN           ExtFamily;
  UINT8           Family;
  UINTN           BaseModel;
  UINTN           ExtModel;
  UINT8           Model;

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
  case ProcessorManufacturerArm:
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"0x%x", ProcessorSignature);
    break;

  case ProcessorManufacturerAmdHygon:
    BaseFamily = (ProcessorSignature & (BIT11 | BIT10 | BIT9 | BIT8)) >> 8;
    ExtFamily  = (ProcessorSignature & (BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20)) >> 20;
    Family     = (UINT8)(BaseFamily + ExtFamily);

    BaseModel = (ProcessorSignature & (BIT7 | BIT6 | BIT5 | BIT4)) >> 4;
    ExtModel  = (ProcessorSignature & (BIT19 | BIT18 | BIT17 | BIT16)) >> 16;
    Model     = (UINT8)((ExtModel << 4) | BaseModel);

    UnicodeSPrint (TempString, MAX_STR_SIZE, L"Family %xh, Model %xh", Family, Model);
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized current speed string format.

  @param[in]      CurrentSpeed             The current speed value.

  @retval         Current speed string.
**/
CHAR16 *
CurrentSpeedStringFormat (
  IN     UINT16               CurrentSpeed
  )
{
  CHAR16          TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (
    TempString,
    sizeof (TempString),
    L"%d MHz",
    CurrentSpeed
    );

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized TDP string format.

  @param[in]      CpuTDP                   The TDP value.

  @retval         TDP string.
**/
CHAR16 *
TDPStringFormat (
  IN     UINT32               CpuTDP
  )
{
  CHAR16          TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (
    TempString,
    sizeof (TempString),
    L"%d W",
    CpuTDP
    );

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized voltage string format.

  @param[in]      Voltage                  The voltage value.

  @retval         Voltage string.
**/
CHAR16 *
VoltageStringFormat (
  IN     UINT8                Voltage
  )
{
  CHAR16                       TempString[MAX_STR_SIZE] = {0};
  PROCESSOR_VOLTAGE_CONVERSION VoltageConversion;

  VoltageConversion.ProcessorVoltage = Voltage;
  if (VoltageConversion.ProcessorVoltageField.ProcessorVoltageIndicateLegacy == TRUE) {
    VoltageConversion.ProcessorVoltage &= ~BIT7;
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d.%d V", (VoltageConversion.ProcessorVoltage / 10), (VoltageConversion.ProcessorVoltage % 10));
  } else if (VoltageConversion.ProcessorVoltageField.ProcessorVoltageCapability5V) {
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"5 V");
  } else if (VoltageConversion.ProcessorVoltageField.ProcessorVoltageCapability3_3V) {
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"3.3 V");
  } else if (VoltageConversion.ProcessorVoltageField.ProcessorVoltageCapability2_9V) {
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"2.9 V");
  } else {
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"Unknown");
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized core L1 data cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L1 data cache string.
**/
CHAR16 *
CoreL1DataCacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity
  )
{
  CHAR16                      TempString[MAX_STR_SIZE] = {0};

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
  case ProcessorManufacturerArm:
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB", CacheSize);
    break;

  case ProcessorManufacturerAmdHygon:
    if ((CacheAssociativity != 0x00) && (CacheAssociativity != 0xFF)) {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/%d-Way", CacheSize, CacheAssociativity);
    } else if (CacheAssociativity == 0xFF) {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/Fully", CacheSize);
    } else {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/Reserved", CacheSize);
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized core L1 insruction cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L1 insruction cache string.
**/
CHAR16 *
CoreL1InsructionCacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity
  )
{
  CHAR16                      TempString[MAX_STR_SIZE] = {0};

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
  case ProcessorManufacturerArm:
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB", CacheSize);
    break;

  case ProcessorManufacturerAmdHygon:
    if ((CacheAssociativity != 0x00) && (CacheAssociativity != 0xFF)) {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/%d-Way", CacheSize, CacheAssociativity);
    } else if (CacheAssociativity == 0xFF) {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/Fully", CacheSize);
    } else {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/Reserved", CacheSize);
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized core L2 cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L2 cache string.
**/
CHAR16 *
CoreL2CacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity
  )
{
  CHAR16                      TempString[MAX_STR_SIZE] = {0};
  UINT8                       Index;
  UINTN                       CacheAssociativityTableCount;

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
  case ProcessorManufacturerArm:
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB", CacheSize);
    break;

  case ProcessorManufacturerAmdHygon:
    CacheAssociativityTableCount = (sizeof (CacheAssociativityTable) / sizeof (CACHE_ASSOCIATIVITY_TABLE));
    for (Index = 0; Index < CacheAssociativityTableCount; Index++) {
      if (CacheAssociativity == CacheAssociativityTable[Index].Associativity) {
        break;
      }
    }
    if (Index == CacheAssociativityTableCount) {
      Index = 0;
    }
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB/%s", CacheSize, CacheAssociativityTable[Index].AssociativityString);
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized core L3 cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.
  @param[in]      CacheNumWays             If CacheAssociativity != 0x9, this paramiter has no effect

  @retval         Core L3 cache string.
**/
CHAR16 *
CoreL3CacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity,
  IN     UINT32               CacheNumWays
  )
{
  CHAR16                      TempString[MAX_STR_SIZE] = {0};
  UINT8                       Index;
  UINTN                       CacheAssociativityTableCount;

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
  case ProcessorManufacturerArm:
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d KB", CacheSize);
    break;

  case ProcessorManufacturerAmdHygon:
    if (CacheAssociativity == CACHE_ASSO_INVALID) {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d MB/%d-Way", CacheSize, CacheNumWays);
    } else {
      CacheAssociativityTableCount = (sizeof (CacheAssociativityTable) / sizeof (CACHE_ASSOCIATIVITY_TABLE));
      for (Index = 0; Index < CacheAssociativityTableCount; Index++) {
        if (CacheAssociativity == CacheAssociativityTable[Index].Associativity) {
          break;
        }
      }
      if (Index == CacheAssociativityTableCount) {
        Index = 0;
      }
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d MB/%s", CacheSize, CacheAssociativityTable[Index].AssociativityString);
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized cache configuration string format.

  @param[in]      CacheConfiguration       The cache configuration value.

  @retval         Cache configuration string.
**/
CHAR16 *
CacheConfigurationStringFormat (
  IN     UINT16               CacheConfiguration
  )
{
  CHAR16          TempString[MAX_STR_SIZE] = {0};

  if (CacheConfiguration & 0x8000) {
    UnicodeValueToStringS (TempString, 0x20, 0, ((CacheConfiguration & ~0x8000) >> 4), 0);
    StrCatS (TempString, 0x20 / sizeof(CHAR16), L" MB");
  } else {
    UnicodeValueToStringS (TempString, 0x20, 0, CacheConfiguration, 0);
    StrCatS (TempString, 0x20 / sizeof(CHAR16), L" KB");
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized microcode version string format.

  @param[in]      MicrocodeVersion         The microcode version value.

  @retval         Microcode version string.
**/
CHAR16 *
MicrocodeVersionStringFormat (
  IN     UINT64               MicrocodeVersion
  )
{
  CHAR16          TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (TempString, MAX_STR_SIZE, L"%08x", MicrocodeVersion);

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized CPU stepping string format.

  @param[in]      CpuStepping              The CPU stepping value.

  @retval         CPU stepping string.
**/
CHAR16 *
CpuSteppingStringFormat (
  IN     UINT32               CpuStepping
  )
{
  CHAR16          TempString[MAX_STR_SIZE] = {0};
  UINTN           BaseModel;
  UINT8           Stepping;
  CHAR16          SteppingString = L'A';

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
  case ProcessorManufacturerArm:
    UnicodeSPrint (TempString, MAX_STR_SIZE, L"%x", CpuStepping);
    break;

  case ProcessorManufacturerAmdHygon:
    BaseModel = (CpuStepping & (BIT7 | BIT6 | BIT5 | BIT4)) >> 4;
    Stepping  =  CpuStepping & (BIT3 | BIT2 | BIT1 | BIT0);

    if (BaseModel < 26) {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%c%d", SteppingString + BaseModel, Stepping);
    } else {
      UnicodeSPrint (TempString, MAX_STR_SIZE, L"%d", Stepping);
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}
