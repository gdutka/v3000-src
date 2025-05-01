/*
 ******************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Uefi/UefiBaseType.h>
#include <AGESA.h>
#include <Library/BaseLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/FchBaseLib.h>
#include <PspResumeMemInfo.h>
#include <Filecode.h>

#define FILECODE  LIBRARY_CCXNONSMMRESUMESECLIB_CCXNONSMMRESUMESEC_FILECODE

#define CAPSULE_UPDATE_SIGNATURE 0xCA9501EFul

typedef struct {
  IN  UINT32 MsrAddr;     ///< MSR address
  IN  UINT64 MsrData;     ///< MSR data
  IN  UINT64 MsrMask;     ///< Mask to be used before data write. Set every bit of all save/restore fields.
} S3_SAVE_MSR;

//  Restore MSR register values
VOID
NonSmmS3ResumeInit (PSP_SMM_HDR_DATA *PspSmmHdrData)
{
  S3_SAVE_MSR *MsrSaveRestoreTable = (S3_SAVE_MSR *)PspSmmHdrData->CcxDataRegion;
  BOOLEAN     CapsuleUpdateFlag = PspSmmHdrData->CapsuleUpdateSignature == CAPSULE_UPDATE_SIGNATURE;
  BOOLEAN     IsBsp = CcxIsBsp (NULL);
  UINT32      i;

  if (IsBsp) {
    // Clear MSRC001_1023[49, TwCfgCombineCr0Cd]
    AsmMsrAnd64 (0xC0011023, ~((UINT64) BIT49));
  }

  // Set ApicEn
  AsmMsrOr32 (0x1B, BIT11);

  for (i = 0; MsrSaveRestoreTable[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    AsmMsrAndThenOr64 (MsrSaveRestoreTable[i].MsrAddr, ~(MsrSaveRestoreTable[i].MsrMask), (MsrSaveRestoreTable[i].MsrData & MsrSaveRestoreTable[i].MsrMask));
  }

  if (IsBsp && CapsuleUpdateFlag) {
    FchWriteSleepType (0);
  }

  if (!IsBsp) {
    if (PspSmmHdrData->CcxEarlyInit == 0) {
      CpuDeadLoop ();
    }

    // BSP will be called later from CcxPei.
    // CcxEarlyInit will be initialized in CcxPei. APs are launched from CcxPei.
    ((CCX_EARLY_INIT)PspSmmHdrData->CcxEarlyInit) ();
  }
}
