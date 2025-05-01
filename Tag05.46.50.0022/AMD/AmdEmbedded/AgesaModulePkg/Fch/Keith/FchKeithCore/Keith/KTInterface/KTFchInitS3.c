/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/

#include "FchPlatform.h"
#include "KTFchTaskLauncher.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTINTERFACE_KTFCHINITS3_FILECODE

FCH_TASK_ENTRY *FchInitS3EarlyTasksTable[] = {
        &FchInitEnvHwAcpi,
        &FchInitEnvLpc,
        &FchInitEnvSpi,
        &FchInitEnvSd,
        &FchInitEnvUsbXhci,
        &FchInitEnvSata,
        &FchInitEnvAb,
        &FchInitEnvXGbe,
        NULL
};


FCH_TASK_ENTRY *FchInitS3LateTasksTable[] = {
        &FchInitLateSpi,
        &FchInitLateSata,
        &FchInitLateHwAcpi,
        NULL
};


VOID
FchInitS3EarlyRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
  );

VOID
FchInitS3LateRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
  );
/*----------------------------------------------------------------------------------------*/
/**
 * FchInitS3EarlyRestore - Config Fch before ACPI S3 resume PCI config device restore
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */

VOID
FchInitS3EarlyRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
  )
{
  FchDataPtr->Misc.S3Resume = 1;
  FchTaskLauncher (&FchInitS3EarlyTasksTable[0], FchDataPtr, TpFchInitS3EarlyDispatching);
  FchDataPtr->Misc.S3Resume = 0;
  AGESA_TESTPOINT (TpFchInitS3EarlyDispatchingEnd, NULL);
}

/*----------------------------------------------------------------------------------------*/
/**
 * FchInitS3LateRestore - Config Fch after ACPI S3 resume PCI config device restore
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */

VOID
FchInitS3LateRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
  )
{
  FchDataPtr->Misc.S3Resume = 1;
  FchTaskLauncher (&FchInitS3LateTasksTable[0], FchDataPtr, TpFchInitS3LateDispatching);
  FchDataPtr->Misc.S3Resume = 0;
  AGESA_TESTPOINT (TpFchInitS3LateDispatchingEnd, NULL);
}


