/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


#include <Base.h>
#include <Uefi.h>

#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>

#include <Guid/MdeModuleHii.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HiiLib.h>
#include <Library/AmdCbsSetupLib.h>
#include <Protocol/AmdCbsHookProtocol.h>
#include "AmdCbsVariable.h"
#include "AmdCbsFormID.h"
#include "AmdSoc.h"

#include "Porting.h"
#include "Addendum/Apcb/Inc/RMB/APCB.h"
#include "ApcbCommon.h"
#include "Protocol/AmdApcbProtocol.h"
#include "Library/AmdPspBaseLibV2.h"
#include "Library/CbsBctSmmLib.h"

extern  EFI_GUID gAmdApcbSmmServiceProtocolGuid;

EFI_STATUS
UpdateCbsApcbTokens (
  VOID *CbsVariable,
  AMD_APCB_SERVICE_PROTOCOL *ApcbProtocol
  );

EFI_STATUS
EFIAPI
CbsBctSetVariable (
  IN CBS_BCT_SMM_PROTOCOL    *This,
  IN UINT8                   *IfrData
)
{
  EFI_STATUS                      Status;
  UINT32                          EAX_Reg;
  UINT32                          EBX_Reg;
  UINT32                          ECX_Reg;
  UINT32                          EDX_Reg;
  UINT64                          SocFamilyID;
  AMD_APCB_SERVICE_PROTOCOL       *mApcbSmmServiceProtocol;

  AsmCpuid (
      0x80000001,
      &EAX_Reg,
      &EBX_Reg,
      &ECX_Reg,
      &EDX_Reg
      );
  SocFamilyID = EAX_Reg & RAW_FAMILY_ID_MASK;
  Status = EFI_SUCCESS;

  if (SocFamilyID != F15_BR_RAW_ID) {
    DEBUG ((EFI_D_ERROR, "Update CBS linked APCB data\n"));
    Status = gSmst->SmmLocateProtocol (
                &gAmdApcbSmmServiceProtocolGuid,
                NULL,
                &mApcbSmmServiceProtocol
                );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //Call the auto generated code
    UpdateCbsApcbTokens (IfrData, mApcbSmmServiceProtocol);
  }
  return Status;
}

EFI_STATUS
EFIAPI
CbsBctWriteFlash (
)
{
  EFI_STATUS                      Status;
  AMD_APCB_SERVICE_PROTOCOL       *mApcbSmmServiceProtocol;

  Status = gSmst->SmmLocateProtocol (
              &gAmdApcbSmmServiceProtocolGuid,
              NULL,
              &mApcbSmmServiceProtocol
              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mApcbSmmServiceProtocol->ApcbFlushData (mApcbSmmServiceProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
