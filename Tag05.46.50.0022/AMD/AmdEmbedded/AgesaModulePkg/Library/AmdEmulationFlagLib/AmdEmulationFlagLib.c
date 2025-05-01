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
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/AmdBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/AmdEmulationFlagLib.h>

#define FILECODE LIBRARY_AMDEMULATIONFLAGLIB_AMDEMULATIONFLAGLIB_FILECODE

 /*----------------------------------------------------------------------------------------
  *                   D E F I N I T I O N S    A N D    M A C R O S
  *----------------------------------------------------------------------------------------
  */

 /*----------------------------------------------------------------------------------------
  *                  T Y P E D E F S     A N D     S T R U C T U R E S
  *----------------------------------------------------------------------------------------
  */
static CHAR16* PresiliconControlStrings[] = {
  L"NO_FLAGS",
  L"PRESIL_SKIP_ALL_USBCONTROLLER_ACCESS",
  L"PRESIL_SKIP_ALL_USBPHY_ACCESS",
  L"PRESIL_SKIP_DXIO_INITIALIZAION",
  L"PRESIL_FASTSIM_GIO",
  L"PRESIL_FASTSIM_DXIO",
  L"PRESIL_FASTSIM_DFLT_TBL",
  L"PRESIL_FASTSIM_SMU_MSGS",
  L"PRESIL_DISABLE_EC",
  L"PRESIL_FASTSIM_PEI_LOG",
  L"PRESIL_FASTSIM_DXE_LOG",
};

 /*----------------------------------------------------------------------------------------
  *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
  *----------------------------------------------------------------------------------------
  */

 /**
  *      Get Emulation switch flag
  *
  *
  *  @param[in, out]      PresilCtrlPtr    Printer to Amd emulation flag heap buffer
  *  @return BOOLEAN    Returns TRUE if in emulatoin run. False if run in HW
  *
  **/
BOOLEAN
GetEmulationFlag (
  IN OUT PRESILICON_CONTROL_STRUCT **PresilCtrlPtr
)
{
  UINT8            *EnvFlagesHeapBuffer = NULL;
  LOCATE_HEAP_PTR             LocateHeapParams;
  PRESILICON_CONTROL_STRUCT  *TempPresilCtrlPtr = NULL;

  LocateHeapParams.BufferHandle = AMD_EMULATION_FLAG;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    EnvFlagesHeapBuffer = LocateHeapParams.BufferPtr;
  }else {
    ASSERT (LocateHeapParams.BufferPtr != NULL);
    return FALSE;
  }

  TempPresilCtrlPtr = (PRESILICON_CONTROL_STRUCT *)EnvFlagesHeapBuffer;

  *PresilCtrlPtr = TempPresilCtrlPtr;
  return (TempPresilCtrlPtr->EnvType == ENV_HW ? 0: 1);
}


/**
 *
 *  presil_CheckEnv
 *
 *  Check the external environment to see if it matches the
 *  value passed in. Also, check the enabled control flags to
 *  see if the flag that is passed in is true. If these
 *  conditions are met, this functions will return true,
 *  enabling the woerkaround.
 *
 *  @param[in]        Env       Which environment need to be check.
 *  @param[in]        CtrlFlag  Which control flag need to be check.
 *  @return BOOLEAN   Returns TRUE if enviroent/control flag are matchs with request. Retruns False if mismatch.
 * */
BOOLEAN
EmulationFlagCheck (
  UINT8 Env,
  UINT8 CtrlFlag
  )
{
  UINT8                     *EnvFlagesHeapBuffer = NULL;
  LOCATE_HEAP_PTR           LocateHeapParams;
  PRESILICON_CONTROL_STRUCT *PresilCtrlPtr = NULL;

  LocateHeapParams.BufferHandle = AMD_EMULATION_FLAG;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    EnvFlagesHeapBuffer = LocateHeapParams.BufferPtr;
  }else {
    ASSERT (EnvFlagesHeapBuffer != NULL);
    return FALSE;
  }

  PresilCtrlPtr = (PRESILICON_CONTROL_STRUCT*)EnvFlagesHeapBuffer;

  if (CtrlFlag >= NUM_CONTROL_FLAGS) {
    ASSERT (CtrlFlag < NUM_CONTROL_FLAGS);
    return FALSE;
  }

  if (((Env == ENV_ALL) || PresilCtrlPtr->EnvType == (PRESILICON_ENV_TYPE)Env) && (PresilCtrlPtr->ControlFlags[CtrlFlag] == TRUE)) {
    return TRUE;
  }

  return FALSE;
}

VOID
DisplayEmulationFlag (
  VOID
)
{
  UINT8            *EnvFlagesHeapBuffer = NULL;
  LOCATE_HEAP_PTR             LocateHeapParams;
  PRESILICON_CONTROL_STRUCT  *PresilCtrlPtr = NULL;
  UINT8                     CtrlFlag;

  LocateHeapParams.BufferHandle = AMD_EMULATION_FLAG;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    EnvFlagesHeapBuffer = LocateHeapParams.BufferPtr;
  }else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "HeapLocateBuffer failed!!!\n");
    ASSERT (LocateHeapParams.BufferPtr != NULL);
    return;
  }

  PresilCtrlPtr = (PRESILICON_CONTROL_STRUCT *)EnvFlagesHeapBuffer;

  IDS_HDT_CONSOLE (MAIN_FLOW, "Environment Type: %s\n", (PresilCtrlPtr->EnvType == ENV_EMULATION? L"Emulation" : PresilCtrlPtr->EnvType == ENV_HW ? L"HW" : L"None"));

  IDS_HDT_CONSOLE (MAIN_FLOW, "  Control Flags\n");
  for(CtrlFlag = NO_FLAGS+1; CtrlFlag < NUM_CONTROL_FLAGS; CtrlFlag++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  %s = %s\n", PresiliconControlStrings[CtrlFlag], PresilCtrlPtr->ControlFlags[CtrlFlag] ? L"TRUE" : L"FALSE");
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "\n");

  return;
}
