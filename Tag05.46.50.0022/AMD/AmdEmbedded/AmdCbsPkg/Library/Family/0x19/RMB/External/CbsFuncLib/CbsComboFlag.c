/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include "AmdSoc.h"
#include "AmdCbsVariable.h"

typedef struct {
  UINT16 RawFamily;                ///< CPUID Fn0000_0001_EAX[BaseFamily] + [ExtFamily]
  UINT8  RawExtModel;              ///< CPUID Fn0000_0001_EAX[ExtModel]
  UINT8  RawBaseModel;             ///< CPUID Fn0000_0001_EAX[BaseModel]
  UINT8  RawStepping;              ///< CPUID Fn0000_0001_EAX[Stepping]
  UINT8  RawPkgType;               ///< CPUID Fn0000_0001_EBX[PkgType]
  UINT8  ComboFlag;
} COMBO_FLAG_XLAT;

#define COMBO_FLAG_UNKNOWN  254
#define COMBO_FLAG_RESERVED 255
#define COMBO_FLAG_XLAT_TERMINATOR   {'x', 'x', 'x', 'x', 'x', COMBO_FLAG_UNKNOWN}

COMBO_FLAG_XLAT CbsComboFlagTable[] =
{
  // Family, ExtModel, BaseModel, Stepping, PkgType, ComboFlag
  {  0x19,   0x4,      0,         'x',      1,       0},  // Rembrandt FP7 A0
  {  0x19,   0x4,      0,         'x',      2,       1},  // Rembrandt FP7r2 A0
  {  0x19,   0x4,      0,         'x',      0,       2},  // Rembrandt AM5 A0
  {  0x19,   0x4,      4,         'x',      4,       3},  // Rembrandt FP7 B0
  {  0x19,   0x4,      4,         'x',      5,       4},  // Rembrandt FP7r2 B0
  {  0x19,   0x4,      4,         'x',      3,       5},  // Rembrandt AM5 B0
  COMBO_FLAG_XLAT_TERMINATOR
};

VOID
CbsComboIdentify (
  IN UINT8   *IfrData
  )
{
  UINT8      RawPkgType;
  UINT8      RawStepping;
  UINT8      RawBaseModel;
  UINT8      RawExtModel;
  UINT16     RawFamily;
  UINT32     EAX_Reg;
  UINT32     EBX_Reg;
  UINT32     ECX_Reg;
  UINT32     EDX_Reg;
  CBS_CONFIG *Setup_Config;
  COMBO_FLAG_XLAT *ComboFlagXlat;

  Setup_Config = (CBS_CONFIG *)IfrData;

  AsmCpuid (
      0x80000001,
      &EAX_Reg,
      &EBX_Reg,
      &ECX_Reg,
      &EDX_Reg
      );
  // get Raw CPUID
  RawFamily    = (UINT16) (((EAX_Reg & CPUID_BASE_FAMILY_MASK) >> 8) +
                           ((EAX_Reg & CPUID_EXT_FAMILY_MASK)  >> 20));
  RawBaseModel = (UINT8)   ((EAX_Reg & CPUID_BASE_MODEL_MASK)  >> 4);
  RawExtModel  = (UINT8)   ((EAX_Reg & CPUID_EXT_MODEL_MASK)   >> 16);
  RawStepping  = (UINT8)    (EAX_Reg & CPUID_STEPPING_MASK);
  RawPkgType   = (EBX_Reg >> 28) & 0x0F;

  ComboFlagXlat = &CbsComboFlagTable[0];
  DEBUG ((EFI_D_ERROR, "Family %x, ExtModel %x, BaseModel %x, Stepping %x, PkgType %x, ", RawFamily, RawExtModel, RawBaseModel, RawStepping, RawPkgType));

  while (ComboFlagXlat->ComboFlag != COMBO_FLAG_UNKNOWN) {
    if (((RawFamily    == ComboFlagXlat->RawFamily)    || (ComboFlagXlat->RawFamily    == 'x')) &&
        ((RawExtModel  == ComboFlagXlat->RawExtModel)  || (ComboFlagXlat->RawExtModel  == 'x')) &&
        ((RawBaseModel == ComboFlagXlat->RawBaseModel) || (ComboFlagXlat->RawBaseModel == 'x')) &&
        ((RawStepping  == ComboFlagXlat->RawStepping)  || (ComboFlagXlat->RawStepping  == 'x')) &&
        ((RawPkgType   == ComboFlagXlat->RawPkgType)   || (ComboFlagXlat->RawPkgType   == 'x'))) {
      Setup_Config->CbsComboFlag = ComboFlagXlat->ComboFlag;
      DEBUG ((EFI_D_ERROR, "ComboFlag %x\n", Setup_Config->CbsComboFlag));
      break;
    }
    ComboFlagXlat++;
  }

  Setup_Config->Header.CbsComboChipsetFlag = PcdGet16 (PcdChipsetIdentifiedId);
  Setup_Config->Header.CbsChipsetVisibleFlag0 = PcdGet8 (PcdChipsetVisableFlag0);
  Setup_Config->Header.CbsChipsetVisibleFlag1 = PcdGet8 (PcdChipsetVisableFlag1);
  DEBUG ((EFI_D_ERROR, "  CbsComboChipsetFlag 0x%04X\n", Setup_Config->Header.CbsComboChipsetFlag));
  DEBUG ((EFI_D_ERROR, "  CbsChipsetVisibleFlag0 0x%02X\n", Setup_Config->Header.CbsChipsetVisibleFlag0));
  DEBUG ((EFI_D_ERROR, "  CbsChipsetVisibleFlag1 0x%02X\n", Setup_Config->Header.CbsChipsetVisibleFlag1));

}


