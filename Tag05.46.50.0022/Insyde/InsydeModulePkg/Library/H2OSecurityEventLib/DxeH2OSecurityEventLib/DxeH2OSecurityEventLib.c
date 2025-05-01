/** @file
  This library provides wrapper functions for common platform security event actions.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/H2OSecurityEventLib.h>
#include <Library/PcdLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/VariableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PostCodeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/H2OLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Guid/H2OCp.h>
#include <Guid/H2OStatusCodeDataTypeId.h>

/**
  This function handles the standard reporting process.

  @param[in]   Event                  Pointer to the security event information structure.
  @param[out]  EventAction            Points to unsigned integer that specifies the action to take for the event.

  @retval      EFI_SUCCESS            Function completed successfully.
  @retval      EFI_INVALID_PARAMETER  The parameters are invalid.
  @retval      EFI_OUT_OF_RESOURCES   No enough buffer could be allocated.
**/
EFI_STATUS
H2OSecurityEventReport (
  IN  CONST H2O_SECURITY_EVENT  *Event,
  OUT UINT32                    *EventAction
  )
{
  H2O_SECURITY_EVENT_VAR                    *EventVar;
  H2O_SECURITY_EVENT                        EventData;
  H2O_STATUS_CODE_SECURITY_EVENT_DATA       *StatusCodeData;
  EFI_STATUS                                Status;
  UINT8                                     *ExtratData;
  CHAR8                                     *ExtraDesc;
  CHAR8                                     *VarExtraData;
  CHAR8                                     *VarExtraDesc;
  UINTN                                     DescSize;
  UINTN                                     Buffersize;

  if (Event == NULL || EventAction == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem (&EventData, Event, sizeof (H2O_SECURITY_EVENT));

  if (FeaturePcdGet (PcdH2OBaseCpSecurityEventReportSupported)) {
    H2O_BASE_CP_SECURITY_EVENT_REPORT_DATA    SecurityEventReportData;

    SecurityEventReportData.Size   = sizeof (H2O_BASE_CP_SECURITY_EVENT_REPORT_DATA);
    SecurityEventReportData.Status = H2O_CP_TASK_NORMAL;
    CopyMem (&SecurityEventReportData.Event, Event, sizeof (H2O_SECURITY_EVENT));
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBaseCpSecurityEventReportGuid));
    Status = H2OCpTrigger (&gH2OBaseCpSecurityEventReportGuid, &SecurityEventReportData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", SecurityEventReportData.Status));
    if (SecurityEventReportData.Status == H2O_CP_TASK_UPDATE) {
      CopyMem (&EventData, &SecurityEventReportData.Event, sizeof (H2O_SECURITY_EVENT));
    } else if (SecurityEventReportData.Status == H2O_CP_TASK_SKIP) {
      goto Done;
    }
  }

  *EventAction = EventData.Event.Action;
  DescSize = (EventData.EventDesc == NULL) ? 0 : MIN (AsciiStrSize(EventData.EventDesc), H2O_SECURITY_EVENT_DESC_SIZE);

  if ((EventData.Event.Action & H2O_SECURITY_EVENT_ACTION_POSTCODE) != 0) {
    PostCodeWithDescription (EventData.Event.PostCode, EventData.EventDesc);
  }

  if ((EventData.Event.Action & H2O_SECURITY_EVENT_ACTION_STATUS_CODE) != 0) {
    Buffersize = sizeof (H2O_STATUS_CODE_SECURITY_EVENT_DATA) + EventData.Event.EventDataSize + DescSize;
    StatusCodeData = AllocatePool (Buffersize);
    if (StatusCodeData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    StatusCodeData->Hdr.HeaderSize = sizeof (EFI_STATUS_CODE_DATA);
    StatusCodeData->Hdr.Size       = (UINT16 ) (Buffersize - sizeof (EFI_STATUS_CODE_DATA));
    CopyGuid (&StatusCodeData->Hdr.Type, &gEfiStatusCodeDataTypeH2OSecurityEventGuid);
    CopyMem (&StatusCodeData->Event, &EventData.Event, sizeof (H2O_SECURITY_EVENT_DATA));
    if (EventData.Event.EventDataSize > 0) {
      ExtratData = (UINT8 *) ((UINTN) StatusCodeData + sizeof (H2O_STATUS_CODE_SECURITY_EVENT_DATA));
      CopyMem (ExtratData, EventData.EventData, EventData.Event.EventDataSize);
    }
    if (EventData.EventDesc != NULL && DescSize > 0) {
      ExtraDesc = (CHAR8 *) ((UINTN) StatusCodeData + sizeof (H2O_STATUS_CODE_SECURITY_EVENT_DATA) + (UINTN) EventData.Event.EventDataSize);
      CopyMem (ExtraDesc, EventData.EventDesc, DescSize - 1);
      ExtraDesc[DescSize - 1] = 0;
    }

    REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
      StatusCodeData->Event.StatusCodeType,
      StatusCodeData->Event.StatusCodeValue,
      (VOID *) StatusCodeData,
      Buffersize
      );

    H2OFreePool ((VOID **) &StatusCodeData);
  }

  if ((EventData.Event.Action & H2O_SECURITY_EVENT_ACTION_LOG) != 0) {
    Buffersize = sizeof (H2O_SECURITY_EVENT_VAR) + (UINTN) EventData.Event.EventDataSize + DescSize;
    EventVar = AllocatePool (Buffersize);
    if (EventVar == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (&EventVar->Event, &EventData.Event, sizeof (H2O_SECURITY_EVENT_DATA));
    if (EventData.Event.EventDataSize > 0) {
      VarExtraData = (CHAR8 *) ((UINTN) EventVar + sizeof (H2O_SECURITY_EVENT_VAR));
      CopyMem (VarExtraData, EventData.EventData, EventData.Event.EventDataSize);
    }
    if (EventData.EventDesc != NULL && DescSize > 0) {
      VarExtraDesc = (CHAR8 *) ((UINTN) EventVar + sizeof (H2O_SECURITY_EVENT_VAR) + (UINTN) EventData.Event.EventDataSize);
      CopyMem (VarExtraDesc, EventData.EventDesc, DescSize - 1);
      VarExtraDesc[DescSize - 1] = 0;
    }
    Status = CommonSetVariable (
               H2O_SECURITY_EVENT_VARIABLE_NAME,
               &gH2OSecurityEventHobGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_APPEND_WRITE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
               Buffersize,
               EventVar
               );

    H2OFreePool ((VOID **) &EventVar);
  }

  if ((EventData.Event.Action & H2O_SECURITY_EVENT_ACTION_SHUTDOWN) != 0) {
    ResetShutdown ();
  }
  if ((EventData.Event.Action & H2O_SECURITY_EVENT_ACTION_RESET) != 0) {
    ResetWarm ();
  }

Done:
  return EFI_SUCCESS;
}
