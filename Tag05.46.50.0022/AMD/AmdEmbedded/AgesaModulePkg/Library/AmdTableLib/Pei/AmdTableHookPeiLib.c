/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 *
 * AMD Register Table Related Functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "PiPei.h"
#include <Filecode.h>
#include <Library/BaseLib.h>
#include <Library/AmdTableLib.h>
#include "Library/PeiServicesTablePointerLib.h"
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/SocLogicalIdPpi.h>

#define FILECODE LIBRARY_AMDTABLELIB_PEI_AMDTABLEHOOKPEILIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

PEI_AMD_NBIO_SMU_SERVICES_PPI                 *NbioSmuServices = NULL;
UINTN                                          SystemDieNumber = 0xFF;


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

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the SMN Register Entry.
 *
 * @TableEntryTypeMethod{::SmnRegisters}.
 *
 * Read - Modify - Write the SMN register, clearing masked bits, and setting the data bits.
 *
 * @param[in]     InstanceId        Zero based die index to modify
 * @param[in]     RegisterIndex     Register address
 * @param[in]     RegisterANDValue  Bitwise AND value
 * @param[in]     RegisterORValue   Bitwise OR value
 *
 */
VOID
SetSmnEntryLib (
  IN       UINT32                  InstanceId,
  IN       UINT32                  RegisterIndex,
  IN       UINT32                  RegisterANDValue,
  IN       UINT32                  RegisterORValue
  )
{
  UINTN                                 DieLoop;
  CONST EFI_PEI_SERVICES                **PeiServices;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServices;

  PeiServices = GetPeiServicesTablePointer();
  if (NbioSmuServices == NULL) {
    // Get NbioSmuServices
    (**PeiServices).LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &NbioSmuServices);
  }

  if (InstanceId == SMN_ON_ALL_DIES) {
    if (SystemDieNumber == 0xFF) {
      // Get SystemDieNumber
      (*PeiServices)->LocatePpi (PeiServices, &gAmdFabricTopologyServices2PpiGuid, 0, NULL, &FabricTopologyServices);
      FabricTopologyServices->GetSystemInfo (NULL, &SystemDieNumber, NULL, NULL, NULL);
    }
    for (DieLoop = 0; DieLoop < SystemDieNumber; DieLoop++) {
      NbioSmuServices->SmuRegisterRMW (NbioSmuServices, DieLoop, RegisterIndex, RegisterANDValue, RegisterORValue);
    }
  } else {
    NbioSmuServices->SmuRegisterRMW (NbioSmuServices, InstanceId, RegisterIndex, RegisterANDValue, RegisterORValue);
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Retrieve the SoC Logical ID of the current core.
 *
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 * @param[out]    SocLogicalId            Current core's SoC Logical ID
 *
 */
VOID
GetSocLogicalIdOnCurrentCore (
  IN       AMD_CONFIG_PARAMS  *StdHeader,
     OUT   SOC_LOGICAL_ID     *SocLogicalId
  )
{
  CONST EFI_PEI_SERVICES       **PeiServices;
  AMD_PEI_SOC_LOGICAL_ID_PPI   *SocLogicalIdPpi;

  PeiServices = GetPeiServicesTablePointer();

  (**PeiServices).LocatePpi (PeiServices, &gAmdSocLogicalIdPpiGuid, 0, NULL, &SocLogicalIdPpi);
  SocLogicalIdPpi->GetLogicalIdOnCurrentCore (SocLogicalId);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Retrieve the Core Logical ID of the current core.
 *
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 * @param[out]    CoreLogicalId           Current core's Core Logical ID
 *
 */
VOID
GetCoreLogicalIdOnCurrentCore (
  IN       AMD_CONFIG_PARAMS  *StdHeader,
     OUT   CORE_LOGICAL_ID    *CoreLogicalId
  )
{
  CONST EFI_PEI_SERVICES       **PeiServices;
  AMD_PEI_SOC_LOGICAL_ID_PPI   *SocLogicalIdPpi;

  PeiServices = GetPeiServicesTablePointer();

  (**PeiServices).LocatePpi (PeiServices, &gAmdSocLogicalIdPpiGuid, 0, NULL, &SocLogicalIdPpi);
  SocLogicalIdPpi->GetCoreLogicalIdOnCurrentCore (CoreLogicalId);
}

/* -----------------------------------------------------------------------------*/
/**
 * Returns the platform features list of the currently running processor core.
 *
 * @param[in]       StdHeader         Config handle for library and services.
 * @param[out]      Features          The Features supported by this platform
 *
 */
VOID
GetPlatformFeatures (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
     OUT   PLATFORM_FEATS         *Features
  )
{
  SOC_LOGICAL_ID  SocLogicalId;

  GetSocLogicalIdOnCurrentCore (StdHeader, &SocLogicalId);
  GetBasePlatformFeatures (StdHeader, &SocLogicalId, Features);
}

/*-------------------------------------------------------------------------------------*/
/**
 * Returns the performance profile features list of the currently running processor core.
 *
 * @param[in]       StdHeader         Header for library and services
 * @param[out]      Features          The performance profile features supported by this platform
 *
 */
VOID
GetPerformanceFeatures (
  IN       AMD_CONFIG_PARAMS  *StdHeader,
     OUT   PROFILE_FEATS      *Features
  )
{
  GetBasePerformanceFeatures (StdHeader, Features);
}

