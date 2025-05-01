/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/FchBaseLib.h>
#include "FchRegistersCommon.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASELIB_FCHSOCLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchGetSocId - Get Soc ID
 *
 *
 */
UINT32
FchGetSocId (
  VOID
  )
{
  UINT32    SocId;
  SocId = PciRead32((AMD_D0F0 << 12) + FCH_REG00);
  //ReadPci ((AMD_D0F0 << 16) + 0, AccessWidth32, &SocId, NULL);

  return SocId;
}

/*---------------------------------------------------------------------------------------*/
/**
 * @brief  Returns the raw family / model / stepping for the executing core
 *
 * @returns       Raw CPU ID for the executing core
 */
UINT32
FchGetRawId (
  VOID
  )
{
  CPUID_DATA CpuId;

  AsmCpuid (
      AMD_CPUID_FMF,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  return CpuId.EAX_Reg;
}

/*---------------------------------------------------------------------------------------*/
/**
 * @brief  Returns the family for the executing core
 *
 * @returns       Family ID for the executing core
 */
UINT32
FchGetFamilyId (
  VOID
  )
{
  CPUID_DATA    CpuId;
  UINT32        Family;

  AsmCpuid (
      AMD_CPUID_FMF,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  Family = (((CpuId.EAX_Reg & CPUID_EXT_FAMILY_MASK) >> 20) + ((CpuId.EAX_Reg & CPUID_BASE_FAMILY_MASK) >> 8));
  return Family;
}

/*---------------------------------------------------------------------------------------*/
/**
 * @brief  Returns the family for the executing core
 *
 * @returns       Model ID for the executing core
 */
UINT32
FchGetModelId (
  VOID
  )
{
  CPUID_DATA    CpuId;
  UINT32        Model;

  AsmCpuid (
      AMD_CPUID_FMF,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  Model = (((CpuId.EAX_Reg & CPUID_EXT_MODEL_MASK) >> 12) | ((CpuId.EAX_Reg & CPUID_BASE_MODEL_MASK) >> 4));
  return Model;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 * @brief  FchCheckAm4PkgType - Checks AM4 Package Type
 *
 *
 *
 */
BOOLEAN
FchCheckAm4PkgType (
  )
{
  CPUID_DATA CpuId;
  UINT8 PkgType;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  PkgType = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF;   // bit 31:28
  if (PkgType == 0x02) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 * @brief  FchCheckAm5PkgType - Checks AM5 Package Type
 *
 *
 *
 */
BOOLEAN
FchCheckAm5PkgType (
  )
{
  CPUID_DATA CpuId;
  UINT8 PkgType;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  PkgType = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF;   // bit 31:28
  if (PkgType == 0x00) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckZPB2Soc - Checks Zeppelin B2
 *
 *   NOTE:
 *
 *
 */
BOOLEAN
FchCheckZPB2Soc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;
  UINT8 Stepping;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x0) && (Model == 0x1) && (Stepping == 0x2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckSspSoc - Checks SSP
 *
 *   NOTE:
 *
 *
 */
BOOLEAN
FchCheckSspSoc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x3)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckGnSoc - Checks GN
 *
 *   NOTE:
 *
 *
 */
BOOLEAN
FchCheckGnSoc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckBaSoc - Checks Ba
 *
 *   NOTE:
 *
 *
 */
BOOLEAN
FchCheckBaSoc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x3)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckMtsSoc - Checks MTS
 *
 *
 *
 */
BOOLEAN
FchCheckMtsSoc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckMtsA0Soc - Checks MTS A0
 *
 *
 *
 */
BOOLEAN
FchCheckMtsA0Soc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x7) && (Model == 0x0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckMtsSoc - Checks VMR
 *
 *
 *
 */
BOOLEAN
FchCheckVmrSoc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckRnA0Soc - Checks RN A0
 *
 *
 *
 */
BOOLEAN
FchCheckRnA0Soc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;
  UINT8 Stepping;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x6) && (Model == 0x0) && (Stepping == 0x0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   FchCheckRplSoc - Checks RPL Soc
 *
 *
 *
 */
BOOLEAN
FchCheckRplSoc (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 ExtModel;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16

  if ((Family == 0xF) && (ExtFamily == 0xA) && (ExtModel == 0x6)) {
    return TRUE;
  } else {
    return FALSE;
  }
}
