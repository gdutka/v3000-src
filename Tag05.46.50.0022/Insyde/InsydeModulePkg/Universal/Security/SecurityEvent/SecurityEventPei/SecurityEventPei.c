/** @file
  SecurityEventPei

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

#include <PiPei.h>
#include <Guid/H2OCp.h>
#include <Guid/H2OSecurityEventHob.h>
#include <Guid/H2OStatusCodeDataTypeId.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/VariableLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

/**
  SecurityEventPei Entry Point
  Transform security event variable data into HOB.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval  EFI_SUCEESS  SecurityEventPei has finished initializing.

  @retval  Other        Return status from CommonGetVariableDataAndSize

**/
EFI_STATUS
EFIAPI
SecurityEventPeiEntry (
  IN       EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS              Status;
  UINTN                   VariableSize;
  UINTN                   Buffersize;
  CHAR8                   *DescPtr;
  UINTN                   DescSize;
  H2O_SECURITY_EVENT_VAR  *SecurityEventVarPtr;
  H2O_SECURITY_EVENT_HOB  *SecurityEventHob;
  CHAR8                   *DataPtr;
  CHAR8                   *HobExtraData;
  CHAR8                   *HobExtraDesc;

  if (FeaturePcdGet (PcdH2OPeiCpSecurityEventDetectSupported)) {
    H2O_PEI_CP_SECURITY_EVENT_DETECT_DATA    SecurityEventDetectData;

    SecurityEventDetectData.Size   = sizeof (H2O_PEI_CP_SECURITY_EVENT_DETECT_DATA);
    SecurityEventDetectData.Status = H2O_CP_TASK_NORMAL;
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OPeiCpSecurityEventDetectGuid));
    Status = H2OCpTrigger (&gH2OPeiCpSecurityEventDetectGuid, &SecurityEventDetectData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", SecurityEventDetectData.Status));
  }

  Status = CommonGetVariableDataAndSize (
             H2O_SECURITY_EVENT_VARIABLE_NAME,
             &gH2OSecurityEventHobGuid,
             &VariableSize,
             (VOID **) &SecurityEventVarPtr
             );
  if (!EFI_ERROR (Status)) {
    while (VariableSize >= sizeof (H2O_SECURITY_EVENT_VAR)) {
      DescPtr = (CHAR8 *) ((UINTN) SecurityEventVarPtr + sizeof (H2O_SECURITY_EVENT_VAR) + (UINTN) SecurityEventVarPtr->Event.EventDataSize);
      DescSize = (DescPtr != NULL ? AsciiStrSize(DescPtr) : 0);
      Buffersize = sizeof (H2O_SECURITY_EVENT_HOB) + (UINTN) SecurityEventVarPtr->Event.EventDataSize + DescSize;
      SecurityEventHob = BuildGuidHob (&gH2OSecurityEventHobGuid, Buffersize);
      if (SecurityEventHob == NULL) {
        break;
      }
      CopyGuid (&SecurityEventHob->Hdr.Name, &gH2OSecurityEventHobGuid);
      SecurityEventHob->Event.Size = sizeof (H2O_SECURITY_EVENT);
      CopyMem (&SecurityEventHob->Event.Event, &SecurityEventVarPtr->Event, sizeof (H2O_SECURITY_EVENT_DATA));
      if (SecurityEventVarPtr->Event.EventDataSize > 0) {
        DataPtr = (CHAR8 *) ((UINTN) SecurityEventVarPtr + sizeof (H2O_SECURITY_EVENT_VAR));
        HobExtraData = (CHAR8 *) ((UINTN) SecurityEventHob + sizeof (H2O_SECURITY_EVENT_HOB));
        CopyMem (HobExtraData, DataPtr, SecurityEventVarPtr->Event.EventDataSize);
      }
      if (DescSize > 0) {
        HobExtraDesc = (CHAR8 *) ((UINTN) SecurityEventHob + sizeof (H2O_SECURITY_EVENT_HOB) + (UINTN) SecurityEventVarPtr->Event.EventDataSize);
        CopyMem (HobExtraDesc, DescPtr, DescSize);
      }

      VariableSize = VariableSize - sizeof (H2O_SECURITY_EVENT_VAR) - SecurityEventVarPtr->Event.EventDataSize - DescSize;
      SecurityEventVarPtr = (H2O_SECURITY_EVENT_VAR *) ((UINT8 *) SecurityEventVarPtr + sizeof (H2O_SECURITY_EVENT_VAR) + SecurityEventVarPtr->Event.EventDataSize + DescSize);
    }
  }

  return Status;
}
