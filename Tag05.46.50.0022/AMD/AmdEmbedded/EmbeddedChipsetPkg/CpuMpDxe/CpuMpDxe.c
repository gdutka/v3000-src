/** @file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "CpuMpDxe.h"
#include "SVM.h"

BOOLEAN mSvmEnabled;
BOOLEAN mSvmLock;

/**
  Enable or diable SVM feature of each APs.

  @param Buffer         The pointer to private data buffer.

**/
VOID
ApEnableDisableSvm (
  IN VOID  *Buffer
  )
{
  EnableDisableSVM (mSvmEnabled, mSvmLock);
}

/**
  Entry Point for CpuMp DXE driver.

  @param ImageHandle    The image handle.
  @param SystemTable    The system table.

  @retval  EFI_SUCEESS
  @retval  Other        Some error occurs.

**/
EFI_STATUS
CpuMpDxeEntry (
  IN EFI_HANDLE                     ImageHandle,
  IN EFI_SYSTEM_TABLE               *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_MP_SERVICES_PROTOCOL          *MpServices;

  mSvmEnabled = PcdGet8 (PcdH2OSvmEnable);
  mSvmLock    = PcdGet8 (PcdH2OSvmLock);

  AsmWbinvd ();

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID**)&MpServices);

  if (!EFI_ERROR (Status)) {
    Status = MpServices->StartupAllAPs (
                           MpServices,
                           ApEnableDisableSvm,
                           FALSE,
                           NULL,
                           0,
                           NULL,
                           NULL
                           );
    EnableDisableSVM (mSvmEnabled, mSvmLock);
  }

  return Status;
}
