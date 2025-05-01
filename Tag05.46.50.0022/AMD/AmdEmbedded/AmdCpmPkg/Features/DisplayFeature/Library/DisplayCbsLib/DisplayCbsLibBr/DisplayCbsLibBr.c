/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmBase.h>
#include <Protocol/SmmBase2.h>

#include <Protocol/AmdCpmDisplayFeatureProtocol/AmdCpmDisplayFeatureProtocol.h>
#include <Protocol/AmdCpmTableProtocol/AmdCpmTableProtocol.h>

#include <Protocol/SmmVariable.h>
#include <AmdCbsConfig.h>
#include <ResourceZP/AmdCbsVariable.h>

EFI_SMM_SYSTEM_TABLE2                       *gSmst;
extern AMD_CPM_TABLE_PROTOCOL               *gCpmTableSmmProtocol;
extern EFI_GUID                             gCbsSystemConfigurationGuid;
extern EFI_GUID                             gEfiSmmVariableProtocolGuid;

/*----------------------------------------------------------------------------------------*/
/**
 * Identify Bristal APU Revision
 *
 * @retval        TRUE           Bristal APU
 * @retval        FALSE          Not Bristal APU
 */

BOOLEAN
DisplayFamilyIdentify (
  )
{
  AMD_CPM_MAIN_TABLE                        *MainTablePtr;
  UINT8                                     CpuRevisionId;

  MainTablePtr        = gCpmTableSmmProtocol->MainTablePtr;
  CpuRevisionId       = MainTablePtr->CpmCpuRevisionId;

  if ((CpuRevisionId == CPM_CPU_REVISION_ID_CZ) || (CpuRevisionId == CPM_CPU_REVISION_ID_ST) || (CpuRevisionId == CPM_CPU_REVISION_ID_BR)) {
    return TRUE;
  }
  return FALSE;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Update Cbs Uma Version option
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
UpdateAmdCbsUmaVersion (
  )
{
  EFI_STATUS                                Status;
  AMD_CPM_MAIN_TABLE                        *MainTablePtr;
  UINTN                                     VarSize;
  CBS_CONFIG                                CbsConfig;
  UINT32                                    Attributes;
  EFI_SMM_VARIABLE_PROTOCOL                 *SmmVariable;
  BOOLEAN                                   IsVariableUpdated;
  UINT8                                     CpuRevisionId;

  Status = EFI_SUCCESS;
  IsVariableUpdated = FALSE;
  VarSize = sizeof(CBS_CONFIG);

  MainTablePtr        = gCpmTableSmmProtocol->MainTablePtr;
  CpuRevisionId       = MainTablePtr->CpmCpuRevisionId;

  if ((CpuRevisionId == CPM_CPU_REVISION_ID_CZ) || (CpuRevisionId == CPM_CPU_REVISION_ID_ST) || (CpuRevisionId == CPM_CPU_REVISION_ID_BR)) {
    Status = gSmst->SmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                        NULL,
                                        (VOID **)&SmmVariable );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = SmmVariable->SmmGetVariable (CBS_SYSTEM_CONFIGURATION_NAME, &gCbsSystemConfigurationGuid, &Attributes, &VarSize, &CbsConfig);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (CbsConfig.CbsCmnGnbGfxUmaVersion != 0) {
      CbsConfig.CbsCmnGnbGfxiGPU_CONTROL = 0x02;
      CbsConfig.CbsCmnGnbGfxUmaVersion = 0;
      IsVariableUpdated = TRUE;
    }

    if (IsVariableUpdated == TRUE) {
      Status = SmmVariable->SmmSetVariable (CBS_SYSTEM_CONFIGURATION_NAME, &gCbsSystemConfigurationGuid, Attributes, VarSize, &CbsConfig);
    }
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update Cbs Uma Size ID
 *
 * @param[in]     UmaSizeId      UMA Size ID
 *                               0: Default
 *                               1: Gaming
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
UpdateAmdCbsUmaSizeId (
  IN UINT8 UmaSizeId
  )
{
  return EFI_UNSUPPORTED;
}

