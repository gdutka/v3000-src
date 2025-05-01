/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmSmm.h>
#include <Features/DisplayFeature/Include/DisplayCbsLib.h>

CPM_DEFINE_gBS;
CPM_DEFINE_gSmst;

AMD_CPM_TABLE_PROTOCOL                  *gCpmTableSmmProtocol;
AMD_CPM_DISPLAY_FEATURE_PROTOCOL        *gCpmDisplayFeatureSmmProtocol;

EFI_STATUS
AmdCpmDisplayFeatureSmmRegistrationCenter (
  VOID
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Display Feature SMM driver
 *
 * This function registers the SMI handler to disable audio in dGPU when PowerXpress
 * is enabled and set SSID when resume from S3.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
AmdCpmDisplayFeatureSmmEntryPoint (
  IN        EFI_HANDLE                  ImageHandle,
  IN        EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                            Status;

  If_CpmInSmm (ImageHandle, SystemTable, gBS, gSmst, Status) {

    DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
    //
    // Initialize global variables
    //
    Status = gSmst->SmmLocateProtocol (
                    &gAmdCpmTableSmmProtocolGuid,
                    NULL,
                    (VOID**)&gCpmTableSmmProtocol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-1-EFI_ERROR\n", __FUNCTION__));
      return Status;
    }

    Status = gBS->LocateProtocol (
                    &gAmdCpmDisplayFeatureProtocolGuid,
                    NULL,
                    (VOID**)&gCpmDisplayFeatureSmmProtocol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-2-EFI_ABORTED\n", __FUNCTION__));
      return EFI_ABORTED;
    }

    if (DisplayFamilyIdentify() == FALSE) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-3-EFI_SUCCESS\n", __FUNCTION__));
      return EFI_SUCCESS;
    }

    gCpmTableSmmProtocol->CommonFunction.PostCode (CpmTpDisplayFeatureSmmDriverBegin);

    Status = AmdCpmDisplayFeatureSmmRegistrationCenter ();

    gCpmTableSmmProtocol->CommonFunction.PostCode (CpmTpDisplayFeatureSmmDriverEnd);
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-4-Status=%r\n", __FUNCTION__, Status));
  }

  RECORD_TIME_DXE (EndAmdCpmDisplayFeatureSmmDriver);

  return Status;
}
