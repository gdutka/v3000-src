/*****************************************************************************
  *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
  *******************************************************************************
  *
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * MemSmbios320V2Lib.c
 *
 * Memory DMI table support for Hybrid DIMMs
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main)
 * @e \$Revision: 329870 $ @e \$Date: 2015-10-29 13:13:29 +0800 (Thu, 29 Oct 2015) $
 *
 **/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "Library/IdsLib.h"
#include "Library/BaseLib.h"
#include "mtspd5.h"
#include "MemDmi.h"
#include "Filecode.h"

#define FILECODE LIBRARY_MEMSMBIOSV2RMBD5LIB_MEMSMBIOS320V2LIB_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

// Hybrid Memory parameters are defined in bytes 192~253

/* -----------------------------------------------------------------------------*/
/**
 *  IsNvHybridDimm
 *
 *  Description:
 *     Check if this is a NV hybrid DIMM
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          BOOLEAN
 *
 */
STATIC
BOOLEAN
IsNvHybridDimm (
  IN        UINT8                       *SpdData
  )
{
  SPD_BASE_CONFIG_0_S*  BaseConfig0;
  ASSERT (SpdData != NULL);

  BaseConfig0 = (SPD_BASE_CONFIG_0_S*) &(SpdData[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
  return (BaseConfig0->KeyByte2.Field.Hybrid == SpdHybrid);
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetDimmType
 *
 *  Description:
 *     Get the type of NVDIMM
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          UINT8
 *
 */
STATIC
UINT8
GetDimmType (
  IN        UINT8                       *SpdData
  )
{
  SPD_BASE_CONFIG_0_S*  BaseConfig0;

  ASSERT (SpdData != NULL);
  BaseConfig0 = (SPD_BASE_CONFIG_0_S*) &(SpdData[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
  if (IsNvHybridDimm (SpdData)) {
    switch (BaseConfig0->KeyByte2.Field.HybridMedia) {
      case SpdHybridMediaNvdimmN:
        return NvDimmNType;
      case SpdHybridMediaNvdimmP:
        return NvDimmPType;
    }
  }
  return DramType;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetDimmOperatingModeCap
 *
 *  Description:
 *     Get the operating mode capability of NVDIMM
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          UINT8
 *
 */
STATIC
UINT16
GetDimmOperatingModeCap (
  IN        UINT8                       *SpdData
  )
{
  DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY_VAR  Capability;

  ASSERT (SpdData != NULL);

  Capability.AsUint16 = 0;

  if (IsNvHybridDimm (SpdData)) {
    // TODO supported operating modes for NVDIMM
  } else {
    //
    // DRAM
    //
    Capability.AsBitmap.VolatileMemory = 1;
  }

  return Capability.AsUint16;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetNvDimmModuleProductId
 *
 *  Description:
 *     Get the module product ID of NVDIMM
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          UINT16          - The Module Product Identifier
 *
 */
STATIC
UINT16
GetNvDimmModuleProductId (
  IN        UINT8                       *SpdData
  )
{
  ASSERT (SpdData != NULL);

  // TODO NVDIMM Module Product ID
  return 0;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetDimmModuleManufacturerId
 *
 *  Description:
 *     Get the module manufacturer ID
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          UINT16          - The Manufacturer Identifier
 *
 */
STATIC
UINT16
GetDimmModuleManufacturerId (
  IN        UINT8                       *SpdData
  )
{
  SPD_MANUFACTURING_INFO_S *MfgInfo;

  ASSERT (SpdData != NULL);
  MfgInfo     = (SPD_MANUFACTURING_INFO_S *)&(SpdData[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN]);
  return MfgInfo->ModuleMfgId.Value;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetNvDimmSubsystemControllerManufacturerId
 *
 *  Description:
 *     Get the manufacturer ID of NVDIMM Subsystem Controller
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          UINT16          - The Manufacturer Identifier of NVDIMM Subsystem Controller
 *
 */
STATIC
UINT16
GetNvDimmSubsystemControllerManufacturerId (
  IN        UINT8                       *SpdData
  )
{
  SPD_NVDIMM_N_ANNEX_S   *ModuleParms;

  ASSERT (SpdData != NULL);
  ModuleParms = (SPD_NVDIMM_N_ANNEX_S *)&(SpdData[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN]);
  if (IsNvHybridDimm (SpdData)) {
    return ModuleParms->DbMfgId.Value;
  } else {
    return 0;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetNvDimmSubsystemControllerId
 *
 *  Description:
 *     Get the ID of NVDIMM Subsystem Controller
 *
 *  Parameters:
 *    @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *    @retval          UINT16          - The Identifier of NVDIMM Subsystem Controller
 *
 */
STATIC
UINT16
GetNvDimmSubsystemControllerId (
  IN        UINT8                       *SpdData
  )
{
  ASSERT (SpdData != NULL);

  // TODO NVDIMM Subsystem Controller ID
  return 0;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetNvDimmFirmwareVersion
 *
 *  Description:
 *     Get the Firmware Version of NVDIMM Subsystem Controller
 *
 *  Parameters:
 *    @param[in]       *SpdData               - Pointer to the SPD data array
 *    @param[in, out]  *FirmwareVersion       - Firmware version
 *
 *    @retval
 *
 */
STATIC
VOID
GetNvDimmFirmwareVersion (
  IN        UINT8                       *SpdData,
  IN OUT    CHAR8                       *FirmwareVersion
  )
{
  ASSERT (SpdData != NULL);
  ASSERT (FirmwareVersion != NULL);

  // TODO NVDIMM Subsystem Controller FW Version
  FirmwareVersion[0] = '\0';
}

/* -----------------------------------------------------------------------------*/
/**
 *  InitSmbios32Type17
 *
 *  Description:
 *     Initialize the SMBIOS 3.2 fields of Type 17
 *
 *  Parameters:
 *    @param[in]       *DimmPresent         - DIMM Present
 *    @param[in]       *SpdData             - Pointer to the SPD data array
 *    @param[in, out]  *T17                 - Pointer to TYPE17_DMI_INFO
 *
 *    @retval          UINT8
 *
 */
VOID
InitSmbios32Type17 (
  IN      BOOLEAN                       DimmPresent,
  IN      UINT8                         *SpdData,
  IN OUT  TYPE17_DMI_INFO               *T17
  )
{
  UINT32         MemorySize;

  ASSERT (SpdData != NULL);
  ASSERT (T17 != NULL);

  if (DimmPresent) {
    T17->MemoryTechnology                           = GetDimmType (SpdData);
    T17->MemoryOperatingModeCapability.AsUint16     = GetDimmOperatingModeCap (SpdData);
    GetNvDimmFirmwareVersion (SpdData, &T17->FirmwareVersion[0]);
    T17->ModuleManufacturerId                       = GetDimmModuleManufacturerId (SpdData);
    T17->ModuleProductId                            = GetNvDimmModuleProductId (SpdData);
    T17->MemorySubsystemControllerManufacturerId    = GetNvDimmSubsystemControllerManufacturerId (SpdData);
    T17->MemorySubsystemControllerProductId         = GetNvDimmSubsystemControllerId (SpdData);
    if (0x7FFF == T17->MemorySize) {
      MemorySize = T17->ExtSize;
    } else {
      MemorySize = T17->MemorySize;
    }
    if (IsNvHybridDimm (SpdData)) {
      T17->NonvolatileSize                          = MultU64x32 (MemorySize, 0x100000); // Convert Mega size to Byte size
      T17->VolatileSize                             = 0;
      T17->CacheSize                                = 0;
      T17->LogicalSize                              = 0;
    } else {
      T17->NonvolatileSize                          = 0;
      T17->VolatileSize                             = MultU64x32 (MemorySize, 0x100000); // Convert Mega size to Byte size
      T17->CacheSize                                = 0;
      T17->LogicalSize                              = 0;
    }
  } else {
    T17->MemoryTechnology                           = UnknownType;
    T17->MemoryOperatingModeCapability.AsBitmap.Unknown = 1;
    T17->FirmwareVersion[0]                         = '\0';
    T17->ModuleManufacturerId                       = 0;
    T17->ModuleProductId                            = 0;
    T17->MemorySubsystemControllerManufacturerId    = 0;
    T17->MemorySubsystemControllerProductId         = 0;
    T17->NonvolatileSize                            = 0;
    T17->VolatileSize                               = 0;
    T17->CacheSize                                  = 0;
    T17->LogicalSize                                = 0;
  }
}
