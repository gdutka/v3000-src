/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/BaseLib.h>
#include <AGESA.h>
#include <cpuRegisters.h>
#include <SocLogicalId.h>
#include "BaseSocLogicalIdXlatZen3DieLib.h"
#include <Filecode.h>
#include <Library/SmnAccessLib.h>
#include <CcxRegistersZen3.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE  LIBRARY_BASESOCLOGICALIDXLATZEN3DIELIB_BASESOCLOGICALIDXLATZEN3DIELIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
FindApplicableLogicalTableEntries (
  IN       UINT32                       RawId,
     OUT   SOC_LOGICAL_FAMILY_XLAT    **Family,
     OUT   SOC_LOGICAL_REVISION_XLAT  **Revision
  );

STATIC CONST SOC_LOGICAL_REVISION_XLAT ROMDATA SocGnLogicalRevisionTable[] =
{
  // NOTE: the highest supported stepping should be the first one
  // Chagall share same ExtFamily and ExtModel with Genesis so need to put in one table
  {0x1, 0x1, AMD_REV_F19_GN_B1, AMD_CORE_F19_ZEN3_B1},
  {0x1, 0x0, AMD_REV_F19_GN_B0, AMD_CORE_F19_ZEN3_B0},
  {0x8, 0x2, AMD_REV_F19_CGL_B2, AMD_CORE_F19_ZEN3_B2},
  {0x8, 0x0, AMD_REV_F19_CGL_B0, AMD_CORE_F19_ZEN3_B0},
  {0x0, 0x0, AMD_REV_F19_GN_A0, AMD_CORE_F19_ZEN3_A0},
  SOC_LOGICAL_REVISION_XLAT_TERMINATOR
};

STATIC CONST SOC_LOGICAL_REVISION_XLAT ROMDATA SocBaLogicalRevisionTable[] =
{
  // NOTE: the highest supported stepping should be the first one
  {0x0, 0x0, AMD_REV_F19_BA_A0_B1CCD, AMD_CORE_F19_ZEN3_B1},
  {0x0, 0x0, AMD_REV_F19_BA_A0_B0CCD, AMD_CORE_F19_ZEN3_B0},
  SOC_LOGICAL_REVISION_XLAT_TERMINATOR
};


STATIC CONST SOC_LOGICAL_REVISION_XLAT ROMDATA SocVmrLogicalRevisionTable[] =
{
  // NOTE: the highest supported stepping should be the first one
  {0x1, 0x2, AMD_REV_F19_VMR_B2, AMD_CORE_F19_ZEN3_B2},
  {0x1, 0x0, AMD_REV_F19_VMR_B0, AMD_CORE_F19_ZEN3_B0},
  {0x0, 0x0, AMD_REV_F19_VMR_A0, AMD_CORE_F19_ZEN3_A0},
  SOC_LOGICAL_REVISION_XLAT_TERMINATOR
};

STATIC CONST SOC_LOGICAL_REVISION_XLAT ROMDATA SocCznLogicalRevisionTable[] =
{
  // NOTE: the highest supported stepping should be the first one
  {0x0, 0x0, AMD_REV_F19_CZN_A0, AMD_CORE_F19_ZEN3_B0},
  SOC_LOGICAL_REVISION_XLAT_TERMINATOR
};

STATIC CONST SOC_LOGICAL_REVISION_XLAT ROMDATA SocRmbLogicalRevisionTable[] =
{
  // NOTE: the highest supported stepping should be the first one
  {0x4, 0x1, AMD_REV_F19_RMB_B1, AMD_CORE_F19_ZEN3_B1},
  {0x4, 0x0, AMD_REV_F19_RMB_B0, AMD_CORE_F19_ZEN3_B0},
  {0x0, 0x0, AMD_REV_F19_RMB_A0, AMD_CORE_F19_ZEN3_B0},
  SOC_LOGICAL_REVISION_XLAT_TERMINATOR
};

STATIC CONST SOC_LOGICAL_FAMILY_XLAT ROMDATA SocZen3LogicalFamilyTable[] =
{
  {0x19, 0x4, AMD_FAMILY_19_RMB, AMD_CORE_FAMILY_19_ZEN3, SocRmbLogicalRevisionTable},  // Family 19h, Models 40h - 4Fh
  {0x19, 0x5, AMD_FAMILY_19_CZN, AMD_CORE_FAMILY_19_ZEN3, SocCznLogicalRevisionTable},  // Family 19h, Models 50h - 5Fh
  {0x19, 0x3, AMD_FAMILY_19_BA,  AMD_CORE_FAMILY_19_ZEN3, SocBaLogicalRevisionTable},   // Family 19h, Models 30h - 3Fh
  {0x19, 0x2, AMD_FAMILY_19_VMR, AMD_CORE_FAMILY_19_ZEN3, SocVmrLogicalRevisionTable},  // Family 19h, Models 20h - 2Fh  {0x19, 0x3, AMD_FAMILY_19_BA,  AMD_CORE_FAMILY_19_ZEN3, SocBaLogicalRevisionTable},   // Family 19h, Models 30h - 3Fh
  {0x19, 0x0, AMD_FAMILY_19_GN,  AMD_CORE_FAMILY_19_ZEN3, SocGnLogicalRevisionTable},   // Family 19h, Models 00h - 0Fh
  SOC_LOGICAL_FAMILY_XLAT_TERMINATOR
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

 /*----------------------------------------------------------------------------------------
  *                          E X P O R T E D    F U N C T I O N S
  *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Translates the given raw ID into the appropriate logical family / logical revision
 *
 * @param[in]     RawId       Raw CPU ID to convert
 * @param[out]    LogicalId   Logical family and logical revision for the given raw ID
 *
 * @retval        TRUE        Logical ID was successfully found
 * @retval        FALSE       Family is unknown
 */
BOOLEAN
BaseSocConvertRawToLogical (
  IN       UINT32          RawId,
     OUT   SOC_LOGICAL_ID  *LogicalId
  )
{
  SOC_LOGICAL_FAMILY_XLAT    *LogicalFamilyTable;
  SOC_LOGICAL_REVISION_XLAT  *LogicalRevisionTable;
  BOOLEAN                     LogicalIdValid;

  LogicalIdValid = FindApplicableLogicalTableEntries (RawId, &LogicalFamilyTable, &LogicalRevisionTable);
  if (LogicalIdValid) {
    LogicalId->Family = LogicalFamilyTable->LogicalFamily;
    LogicalId->Revision = LogicalRevisionTable->LogicalRevision;
  }

  return LogicalIdValid;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Translates the given raw ID into the appropriate logical family / logical revision
 *
 * @param[in]     RawId       Raw CPU ID to convert
 * @param[out]    LogicalId   Logical family and logical revision for the given raw ID
 *
 * @retval        TRUE        Logical ID was successfully found
 * @retval        FALSE       Family is unknown
 */
BOOLEAN
BaseCoreConvertRawToLogical (
  IN       UINT32           RawId,
     OUT   CORE_LOGICAL_ID  *LogicalId
  )
{
  SOC_LOGICAL_FAMILY_XLAT    *LogicalFamilyTable;
  SOC_LOGICAL_REVISION_XLAT  *LogicalRevisionTable;
  BOOLEAN                     LogicalIdValid;

  LogicalIdValid = FindApplicableLogicalTableEntries (RawId, &LogicalFamilyTable, &LogicalRevisionTable);
  if (LogicalIdValid) {
    LogicalId->CoreFamily = LogicalFamilyTable->LogicalCoreFamily;
    LogicalId->CoreRevision = LogicalRevisionTable->LogicalCoreRev;
  }

  return LogicalIdValid;
}


/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Finds the appropriate logical ID information based on SoC family / model / stepping
 *
 * @param[in]     RawId       Raw family / model / stepping
 * @param[out]    Family      Pointer to valid family table entry if retval is TRUE
 * @param[out]    Revision    Pointer to valid revision table entry if retval is TRUE
 *
 * @retval        TRUE        Logical table entries were successfully found
 * @retval        FALSE       Family is unknown
 */
BOOLEAN
FindApplicableLogicalTableEntries (
  IN       UINT32                       RawId,
     OUT   SOC_LOGICAL_FAMILY_XLAT    **Family,
     OUT   SOC_LOGICAL_REVISION_XLAT  **Revision
  )
{
  SOC_LOGICAL_FAMILY_XLAT    *LogicalFamilyTable;
  SOC_LOGICAL_REVISION_XLAT  *LogicalRevisionTable;
  UINT16                      RawFamily;
  UINT8                       RawStepping;
  UINT8                       RawBaseModel;
  UINT8                       RawExtModel;
  BOOLEAN                     EntriesFound;
  UINT32                      Data;

  EntriesFound = FALSE;

  // get Raw CPUID
  RawFamily    = (UINT16) (((RawId & CPUID_BASE_FAMILY_MASK) >> CPUID_BASE_FAMILY_OFFSET) +
                           ((RawId & CPUID_EXT_FAMILY_MASK)  >> CPUID_EXT_FAMILY_OFFSET));
  RawBaseModel = (UINT8)   ((RawId & CPUID_BASE_MODEL_MASK)  >> CPUID_BASE_MODEL_OFFSET);
  RawExtModel  = (UINT8)   ((RawId & CPUID_EXT_MODEL_MASK)   >> CPUID_EXT_MODEL_OFFSET);
  RawStepping  = (UINT8)    (RawId & CPUID_STEPPING_MASK);

  // get logical CPUID
  LogicalFamilyTable = (SOC_LOGICAL_FAMILY_XLAT *) SocZen3LogicalFamilyTable;
  while (LogicalFamilyTable->LogicalRevisionTable != NULL) {
    if ((RawFamily == LogicalFamilyTable->RawFamily) &&
        (RawExtModel == LogicalFamilyTable->RawExtModel)) {
      LogicalRevisionTable = (SOC_LOGICAL_REVISION_XLAT *) LogicalFamilyTable->LogicalRevisionTable;  // get logical revision table
      EntriesFound = TRUE;
      *Family = LogicalFamilyTable;
      *Revision = LogicalRevisionTable;     // initialize SOC_LOGICAL_ID.Revision with the first one (should be
                                            // the highest supported one)
      while (LogicalRevisionTable->LogicalRevision != AMD_REVISION_UNKNOWN) {
        if ((RawBaseModel == LogicalRevisionTable->RawBaseModel) &&
            (RawStepping == LogicalRevisionTable->RawStepping)) {
          *Revision = LogicalRevisionTable;
          // if BA A0 check if b1ccd
          if ((LogicalFamilyTable->LogicalFamily == AMD_FAMILY_19_BA) &&
              (LogicalRevisionTable->LogicalRevision == AMD_REV_F19_BA_A0_B1CCD)) {
            SmnRegisterRead (0, SMN_FUSE_CCD_DATA_453_ADDRESS, &Data);
            if (Data & BIT1) {
              break;
            }
          } else {
            break;
          }
        }
        LogicalRevisionTable++;
      }
      break;
    }
    LogicalFamilyTable++;
  }

  return EntriesFound;
}
