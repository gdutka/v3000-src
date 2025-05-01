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

#include <Protocol/AmdApcbProtocol.h>
#include <ApcbCommon.h>

#define APCB_TOKEN_UID_UMAMODE              0x1FB35295
#define APCB_TYPE_PURPOSE_NORMAL            6

extern EFI_SMM_SYSTEM_TABLE2                *gSmst;
extern AMD_CPM_TABLE_PROTOCOL               *gCpmTableSmmProtocol;

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
    return FALSE;
  }
  return TRUE;
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
  AMD_APCB_SERVICE_PROTOCOL                 *mApcbSmmServiceProtocol;
  UINT16                                    TokenId;
  UINT32                                    SizeInByte;
  UINT64                                    Value;
  UINT8                                     CpuRevisionId;

  MainTablePtr        = gCpmTableSmmProtocol->MainTablePtr;
  CpuRevisionId       = MainTablePtr->CpmCpuRevisionId;

  Status = EFI_SUCCESS;
  TokenId = 0;
  SizeInByte = 0;
  Value = 0;

  if (CpuRevisionId > CPM_CPU_REVISION_ID_ZP) {
    Status = CpmSmmLocateProtocol ( &gAmdApcbSmmServiceProtocolGuid,
                                    NULL,
                                    &mApcbSmmServiceProtocol );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    TokenId = APCB_ID_CBS_CMN_GNB_GFX_UMA_MODE;
    SizeInByte = 1;             // UINT8 Size
    Value = 2;                  // UMA_AUTO = 2
    Status = mApcbSmmServiceProtocol->ApcbSetCbsParameter ( mApcbSmmServiceProtocol,
                                                            TokenId,
                                                            &SizeInByte,
                                                            &Value );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    TokenId = APCB_ID_CBS_CMN_GNB_GFX_UMA_VERSION;
    SizeInByte = 1;             // UINT8 Size
    Value = 0;                  // UMA_LEGACY = 0
    Status = mApcbSmmServiceProtocol->ApcbSetCbsParameter ( mApcbSmmServiceProtocol,
                                                            TokenId,
                                                            &SizeInByte,
                                                            &Value );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = mApcbSmmServiceProtocol->ApcbFlushData ( mApcbSmmServiceProtocol );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } // End of if (CpuRevisionId > CPM_CPU_REVISION_ID_ZP)

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
  EFI_STATUS                                Status;
  AMD_APCB_SERVICE_PROTOCOL                 *mApcbSmmServiceProtocol;
  UINT64                                    Value;

  Status = EFI_SUCCESS;
  Value = 0;

  Status = CpmSmmLocateProtocol ( &gAmdApcbSmmServiceProtocolGuid,
                                    NULL,
                                    &mApcbSmmServiceProtocol );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mApcbSmmServiceProtocol->ApcbAcquireMutex (mApcbSmmServiceProtocol);

  if (UmaSizeId == 2) {
    Value = 3;                  // UMA_GAME_OPTIMIZED
  } else {
    Value = 2;                  // UMA_AUTO
  }

  Status = mApcbSmmServiceProtocol->ApcbSetToken8 (
               mApcbSmmServiceProtocol,
               2,
               APCB_TOKEN_UID_UMAMODE,
               (UINT8) Value
               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mApcbSmmServiceProtocol->ApcbFlushData ( mApcbSmmServiceProtocol );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mApcbSmmServiceProtocol->ApcbReleaseMutex (mApcbSmmServiceProtocol);

  return Status;
}
