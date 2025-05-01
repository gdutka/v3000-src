/*
****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/


#include <Filecode.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/AmdErrorLogLib.h>
#include <Protocol/AmdErrorLogServiceProtocol.h>
#include "AmdErrorLogDisplayDxe.h"
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include "Porting.h"

#define FILECODE ERRORLOG_AMDERRORLOGDISPLAYDXE_AMDERRORLOGDISPLAYDXE_FILECODE

#define ABL_CCD_BIST_FAILURE 0x4065
#define CCD_MAX_CCDS_PER_DIE       8  // Program dependent

extern  EFI_BOOT_SERVICES       *gBS;

//
// Driver Global Data
//
STATIC AMD_ERROR_BUFFER *ErrorLogBuffer;

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This function logs AGESA Errors into the Error log.
 *
 * It will put the information in a circular buffer consisting of 128 such log
 * entries. If the buffer gets full, then the next Error log entry will be written
 * over the oldest Error log entry.
 *
 * @param[in]   PeiServices, ClearBuffer
 * @param[out]  ErrorLogDataPtr
 *
 */
EFI_STATUS
EFIAPI
AmdAquireErrorLogWithFlagDxe (
  IN       DXE_AMD_ERROR_LOG_SERVICES_PROTOCOL   *This,
  OUT      ERROR_LOG_DATA_STRUCT *ErrorLogDataPtr,
  IN       BOOLEAN ClearBuffer
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;

  AquireErrorLog (ErrorLogBuffer, ErrorLogDataPtr, ClearBuffer);
  //Reset Error Log buffer
  if (ClearBuffer) {
    ErrorLogBufferInit(ErrorLogBuffer);
  }

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This function aquires gAmdErrorLogServiceProtocolGuid and display any error of
 * ABL_MEM_PMU_TRAIN_ERROR & ABL_MEM_AGESA_MEMORY_TEST_ERROR to serial console & video
 * screen
 *
 */
VOID
DisplayAmdErrorLogCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  DXE_AMD_ERROR_LOG_SERVICES_PROTOCOL   *ErrorLogServices;
  ERROR_LOG_DATA_STRUCT   *ErrorLogDataPtr;
  UINT32                  i;
  UINT32                  Ccd;
  EFI_STATUS              Status;
  BOOLEAN                 anyError;

  Status = gBS->LocateProtocol(&gAmdErrorLogServiceProtocolGuid, NULL, &ErrorLogServices);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate gAmdErrorLogServiceProtocolGuid fail at DisplayAmdErrorLogCallBack\n");
    return;
  }
  if (!FeaturePcdGet (PcdAmdIdsDebugPrintEnable) && !FeaturePcdGet (PcdAgesaPrintEventLogToConsole)) {
    return;
  }

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (ERROR_LOG_DATA_STRUCT), (VOID **)&ErrorLogDataPtr);
  if (EFI_ERROR (Status)) {
    return;
  }
  ZeroMem (ErrorLogDataPtr, sizeof (ERROR_LOG_DATA_STRUCT));

  ErrorLogServices->AmdAquireErrorLogWithFlagDxe (ErrorLogServices, ErrorLogDataPtr, FALSE);

  // ABL_CCD_BIST_FAILURE
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_CCD_BIST_FAILURE == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n**********************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_CCD_BIST_FAILURE == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        for(Ccd = 0; Ccd < CCD_MAX_CCDS_PER_DIE; Ccd++){
          if(ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & (1 << Ccd)){
            Print (L"* CCD BIST error at Socket %d Die %d CCD %d\n", (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 0xFF, (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xFF, Ccd);
          }
        }
      }
    }
    Print (L"**********************************************************\n");
  }

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  Status = gBS->FreePool (ErrorLogDataPtr);
}

/*********************************************************************************
 * Name: AmdErrorLogDisplayDxeInit
 *
 * Description
 *   Entry point of the AMD Error Log DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
AmdErrorLogDisplayDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  EFI_EVENT           DisplayAmdErrorLogEvent;
  VOID                *Registration;

  DEBUG ((EFI_D_ERROR, "*****************************DXE Error Log Display Driver Entry*********************\n"));

  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL,
      TPL_NOTIFY,
      DisplayAmdErrorLogCallBack,
      NULL,
      NULL,
      &DisplayAmdErrorLogEvent
      );

  Status = gBS->RegisterProtocolNotify (
      &gAmdNbioIommuProtocolGuid,
      DisplayAmdErrorLogEvent,
      &Registration
      );

  DEBUG ((EFI_D_ERROR, "*****************************DXE Error Log Display Driver Exit*********************\n"));
  return (Status);
}

