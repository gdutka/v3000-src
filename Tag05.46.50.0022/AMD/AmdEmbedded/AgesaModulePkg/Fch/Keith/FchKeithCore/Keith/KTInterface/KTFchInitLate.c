/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/

#include "FchPlatform.h"
#include "KTFchTaskLauncher.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTINTERFACE_KTFCHINITLATE_FILECODE

FCH_TASK_ENTRY  *FchInitLateTasksTable[] = {
        &FchInitLateSpi,
        &FchInitLateSata,
        &FchInitLateHwAcpi,
        &FchInitLateUsbXhci,
        &FchInitLateTriggerSmi,
        NULL
      };

AGESA_STATUS
FchInitLate (
  IN      FCH_DATA_BLOCK     *LateParams
  );

/*----------------------------------------------------------------------------------------*/
/**
 *  FchInitLate - Prepare Fch to boot to OS.
 *
 *
 *
 * @param[in] LateParams
 *
 */
AGESA_STATUS
FchInitLate (
  IN      FCH_DATA_BLOCK     *LateParams
  )
{
  AGESA_STATUS        Status;

  IDS_HDT_CONSOLE (FCH_TRACE, "  FchInitLate Enter... \n");
  Status = FchTaskLauncher (&FchInitLateTasksTable[0], LateParams, TpFchInitLateDispatching);
  IDS_HDT_CONSOLE (FCH_TRACE, "  FchInitLate Exit... Status = [0x%x]\n", Status);
  return Status;
}


