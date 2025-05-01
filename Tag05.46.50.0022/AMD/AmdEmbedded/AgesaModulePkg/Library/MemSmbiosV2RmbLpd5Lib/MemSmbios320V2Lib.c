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
#include "mtspd4.h"
#include "MemDmi.h"
#include "Filecode.h"

#define FILECODE LIBRARY_MEMSMBIOSV2RMBLPD5LIB_MEMSMBIOS320V2LIB_FILECODE

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
  ASSERT (SpdData != NULL);

  if ((SpdData[SPD_BASE_MODULE_TYPE] & MOD_TYPE_HYBRID) != 0 &&
      (SpdData[SPD_BASE_MODULE_TYPE] & MOD_TYPE_HYBRID_TYPE) == 0x10) {
    return TRUE;
  } else {
    return FALSE;
  }
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
  UINT16    SpdByte;
  UINT8     FunctionClass_9_8;
  UINT8     FunctionClass_7_5;
  BOOLEAN   ByteAddressableNoEnergyBacked;

  ASSERT (SpdData != NULL);

  if (IsNvHybridDimm (SpdData)) {
    if ((SpdData[SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_LSB] & (NAND_MSK << NAND_SHIFT)) != 0) {
      if ((SpdData[SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_LSB] & (SDRAM_MSK << SDRAM_SHIFT)) != 0) {
        //
        // NAND Flash + DRAM
        //
        ByteAddressableNoEnergyBacked = FALSE;
        for (SpdByte = SPD_NVDIMM_FUNCTION_0_INTERFACE_DESCRIPTORS_LSB; SpdByte <= SPD_NVDIMM_FUNCTION_7_INTERFACE_DESCRIPTORS_LSB; SpdByte += 2) {
          if (0 == (SpdData[SpdByte + 1] & (FUNCTION_INTERFACE_IMPLEMENTED_MSK << FUNCTION_INTERFACE_IMPLEMENTED_SHIFT))) {
            //
            // No more functions
            //
            break;
          }

          FunctionClass_9_8 = (SpdData[SpdByte + 1] >> FUNCTION_CLASS_HIGH_SHIFT) & FUNCTION_CLASS_HIGH_MSK;
          FunctionClass_7_5 = (SpdData[SpdByte] >> FUNCTION_CLASS_LOW_SHIFT) & FUNCTION_CLASS_LOW_MSK;
          switch ((FunctionClass_9_8 << 3) + FunctionClass_7_5) {
            case 0:
              //
              // Undefined function
              //
              break;
            case 1:
              //
              // Byte addressable energy backed
              //
              break;
            case 2:
              //
              // Block addressed
              //
              break;
            case 3:
              //
              // Byte addressable, no energy backed
              //
              ByteAddressableNoEnergyBacked = TRUE;
              break;
          }
        }
        return ByteAddressableNoEnergyBacked ? NvDimmPType : NvDimmFType;
      } else {
        //
        // NAND Flash only
        //
        return NvDimmFType;
      }
    } else {
      ASSERT (FALSE);
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
  UINT16                                        SpdByte;
  UINT8                                         FunctionClass_9_8;
  UINT8                                         FunctionClass_7_5;
  DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY_VAR  Capability;

  ASSERT (SpdData != NULL);

  Capability.AsUint16 = 0;

  if (IsNvHybridDimm (SpdData)) {
    for (SpdByte = SPD_NVDIMM_FUNCTION_0_INTERFACE_DESCRIPTORS_LSB; SpdByte <= SPD_NVDIMM_FUNCTION_7_INTERFACE_DESCRIPTORS_LSB; SpdByte += 2) {
      //
      // Function Class Bit 9-8 defined as always 0 in Table 161 - Function Classess and Function Interfaces
      //
      if (0 == (SpdData[SpdByte + 1] & (FUNCTION_INTERFACE_IMPLEMENTED_MSK << FUNCTION_INTERFACE_IMPLEMENTED_SHIFT))) {
        //
        // No more functions
        //
        break;
      }

      FunctionClass_9_8 = (SpdData[SpdByte + 1] >> FUNCTION_CLASS_HIGH_SHIFT) & FUNCTION_CLASS_HIGH_MSK;
      FunctionClass_7_5 = (SpdData[SpdByte] >> FUNCTION_CLASS_LOW_SHIFT) & FUNCTION_CLASS_LOW_MSK;
      switch ((FunctionClass_9_8 << 3) + FunctionClass_7_5) {
        case 0:
          //
          // Undefined function
          //
          break;
        case 1:
          //
          // Byte addressable energy backed
          //
          Capability.AsBitmap.ByteAccessiblePersistentMemory = 1;
          break;
        case 2:
          //
          // Block addressed
          //
          Capability.AsBitmap.BlockAccessiblePersistentMemory = 1;
          break;
        case 3:
          //
          // Byte addressable, no energy backed
          //
          Capability.AsBitmap.ByteAccessiblePersistentMemory = 1;
          break;
      }
    }
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

  if (IsNvHybridDimm (SpdData)) {
    return (SpdData[SPD_NVDIMM_MODULE_PRODUCT_ID_MSB] << 8) + SpdData[SPD_NVDIMM_MODULE_PRODUCT_ID_LSB];
  } else {
    return 0;
  }
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
  ASSERT (SpdData != NULL);

  return (SpdData[SPD_MANUFACTURER_ID_MSB] << 8) + SpdData[SPD_MANUFACTURER_ID_LSB];
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
  ASSERT (SpdData != NULL);

  if (IsNvHybridDimm (SpdData)) {
    return (SpdData[SPD_NVDIMM_SUBSYSTEM_CONTROLLER_MANUFACTURER_ID_MSB] << 8) + SpdData[SPD_NVDIMM_SUBSYSTEM_CONTROLLER_MANUFACTURER_ID_LSB];
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

  if (IsNvHybridDimm (SpdData)) {
    return (SpdData[SPD_NVDIMM_SUBSYSTEM_CONTROLLER_ID_MSB] << 8) + SpdData[SPD_NVDIMM_SUBSYSTEM_CONTROLLER_ID_LSB];
  } else {
    return 0;
  }
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
  IN OUT    CHAR8                        *FirmwareVersion
  )
{
  UINT8   Revision;

  ASSERT (SpdData != NULL);
  ASSERT (FirmwareVersion != NULL);

  if (IsNvHybridDimm (SpdData)) {
    Revision = SpdData[SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE];
    FirmwareVersion[0] = Revision / 100 + '0';
    FirmwareVersion[1] = (Revision % 100) / 10 + '0';
    FirmwareVersion[2] = Revision % 10 + '0';
    FirmwareVersion[3] = '\0';
  } else {
    FirmwareVersion[0] = '\0';
  }
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

/* -----------------------------------------------------------------------------*/
/**
 *  InitSmbios33Type17
 *
 *  Description:
 *     Initialize the SMBIOS 3.3 fields of Type 17
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
InitSmbios33Type17 (
  IN      BOOLEAN                       DimmPresent,
  IN      UINT8                         *SpdData,
  IN OUT  TYPE17_DMI_INFO               *T17
  )
{
  ASSERT (SpdData != NULL);
  ASSERT (T17 != NULL);

  //
  // For compatibility with older SMBIOS parsers, memory devices slower than
  // 65,535 MT/s should represent their speed using the Speed and Configured Memory Speed fields,
  // leaving the Extended Speed and Extended Configured Memory Speed fields set to 0.
  //
  T17->ExtendedSpeed                 = 0;
  T17->ExtendedConfiguredMemorySpeed = 0;
}
