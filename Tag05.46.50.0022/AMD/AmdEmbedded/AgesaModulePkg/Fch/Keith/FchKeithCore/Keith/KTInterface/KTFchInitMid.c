/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/

#include "FchPlatform.h"
#include "KTFchTaskLauncher.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTINTERFACE_KTFCHINITMID_FILECODE

FCH_TASK_ENTRY  *FchInitMidTasksTable[] = {
        &FchInitMidHwAcpi,
        NULL
      };

AGESA_STATUS
FchInitMid (
  IN      FCH_DATA_BLOCK     *FchParams
  );

/**
 *  FchInitMid - Config Fch after PCI emulation
 *
 *
 *
 * @param[in] FchParams Fch configuration structure pointer.
 *
 */
AGESA_STATUS
FchInitMid (
  IN      FCH_DATA_BLOCK     *FchParams
  )
{
  AGESA_STATUS        Status;

  IDS_HDT_CONSOLE (FCH_TRACE, "  FchInitMid Enter... \n");
  Status = FchTaskLauncher (&FchInitMidTasksTable[0], FchParams, TpFchInitMidDispatching);
  IDS_HDT_CONSOLE (FCH_TRACE, "  FchInitMid Exit... Status = [0x%x]\n", Status);
  return Status;
}


