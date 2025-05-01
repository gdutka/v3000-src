/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX Zen3 SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   AMD CCX Zen3 SMM  Driver
 * @e \$Revision$   @e \$Date$
 *
 */
#include <AGESA.h>
#include <PiSmm.h>
#include <Library/BaseLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/IdsLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/CcxSetMcaLib.h>
#include <Library/UefiLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/HobLib.h>
#include <Protocol/AmdPspResumeServiceProtocol.h>
#include <Protocol/FchSmmSwDispatch2.h>
#include <Protocol/FchSmmSxDispatch2.h>
#include <PspResumeMemInfo.h>
#include <cpuRegisters.h>
#include <Filecode.h>

#define FILECODE CCX_ZEN3_SMM_AMDCCXZEN3SMM_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
CcxZen3Pfeh (
  );

VOID
CcxZen3PfehAp (
  IN       VOID *Buffer
  );

VOID
CcxZen3S3ResumeLateAp (
  IN       VOID *Buffer
  );

/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
/// for MSR S3 save/restore
typedef struct {
  IN  UINT32 MsrAddr;     ///< MSR address
  IN  UINT64 MsrData;     ///< MSR data
  IN  UINT64 MsrMask;     ///< Mask to be used before data write. Set every bit of all save/restore fields.
} S3_SAVE_MSR;


/*----------------------------------------------------------------------------------------
 *               G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
S3_SAVE_MSR     mMsrSaveRestoreTable[] =
{
  { 0xC0011093, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0011097, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010296, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  }, // Cstate_Config
  { 0xC0010064, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010015, 0x0000000000000000, 0xFFFFFFFD7FFFFFFE  }, // HWCR, bit[0][SmmLock], bit[31][SmmBaseLock], bit[33]SmmPgCfgLock should not be restored.
                                                           // IBV may clear this bit at very early of S3 resume path.
                                                           // This register must be restored after all Pstate registers (0xC001_00[64:6B]) have been restored.
                                                           // Because bit21[LockTscToCurrentP0] must be restored after customizing Pstates.
                                                           // Bit[33]SmmPgCfgLock can only be restored in SMM not on non-Smm S3 resume.
  { CPU_LIST_TERMINAL               }
};

UINT64                UcodePatch = 0;
BOOLEAN               mPfehMode = FALSE;
BOOLEAN               mPcdAmdPspS3WakeFromSmm;

/*----------------------------------------------------------------------------------------
 *               L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
CcxZen3Pfeh (
  )
{
  UINT16          i;

  CcxZen3PfehAp (NULL);

//[-start-220225-IB14740218-modify]//
  for (i = 0; i < gSmst->NumberOfCpus; i++) {
    if (i != gSmst->CurrentlyExecutingCpu) {
      gSmst->SmmStartupThisAp (
              CcxZen3PfehAp,
              i,
              NULL
              );
    }
  }
//[-end-220225-IB14740218-modify]//
}

VOID
CcxZen3PfehAp (
  IN       VOID *Buffer
  )
{
  // Platform First Error Handling
  if (mPfehMode) {
    AsmWriteMsr64 (0xC0010121, 0xFFFFFFFFFFFFFFFF);
    AsmWriteMsr64 (0xC0010122, 0);
    AsmMsrOr64 (0xC0010120, BIT0);
  } else {
    AsmWriteMsr64 (0xC0010121, 0);
    AsmWriteMsr64 (0xC0010122, 0);
    AsmMsrAnd64 (0xC0010120, ~(UINT64)BIT0);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Save MSRs to mMsrSaveRestoreTable
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
CcxZen3S3SaveSmi (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  UINTN  i;

  // Platform First Error Handling
  CcxZen3Pfeh ();

  for (i = 0; mMsrSaveRestoreTable[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    mMsrSaveRestoreTable[i].MsrData = AsmReadMsr64 (mMsrSaveRestoreTable[i].MsrAddr);
  }

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function for restore MSRs from mMsrSaveRestoreTable
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
CcxZen3S3RestoreSmi (
  IN  RESUME_TYPE   ResumeType,
  IN  VOID          *Context
  )
{
  UINTN  i;

  if (CcxIsBsp (NULL)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3S3RestoreSmi\n");

    // Clear MSRC001_1023[49, TwCfgCombineCr0Cd]
    AsmMsrAnd64 (0xC0011023, ~((UINT64) BIT49));

    // Workaround PLAT-11398
    // BSP gets ucode patch address from scratch MSR
    UcodePatch = AsmReadMsr64 (0xC001100C);
  }

  // Set ApicEn
  AsmMsrOr32 (0x1B, BIT11);

  for (i = 0; mMsrSaveRestoreTable[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    IDS_HDT_CONSOLE (CPU_TRACE, "Restoring MSR %X with %lX\n", mMsrSaveRestoreTable[i].MsrAddr, mMsrSaveRestoreTable[i].MsrData);
    AsmMsrAndThenOr64 (mMsrSaveRestoreTable[i].MsrAddr, ~(mMsrSaveRestoreTable[i].MsrMask), (mMsrSaveRestoreTable[i].MsrData & mMsrSaveRestoreTable[i].MsrMask));
  }

  // Mca initialization
  CcxSetMca ();

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * All threads need to perform the following at late S3 resume:
 * - Force recalculation of TSC
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
CcxZen3S3ResumeLateSmi (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  UINT16          i;

  IDS_HDT_CONSOLE (CPU_TRACE, "CcxZen3S3ResumeLateSmi Entry\n");

  CcxZen3S3ResumeLateAp (NULL);

//[-start-220225-IB14740218-modify]//
  for (i = 0; i < gSmst->NumberOfCpus; i++) {
    if (i != gSmst->CurrentlyExecutingCpu) {
      gSmst->SmmStartupThisAp (
              CcxZen3S3ResumeLateAp,
              i,
              NULL
              );
    }
  }
//[-end-220225-IB14740218-modify]//

  IDS_HDT_CONSOLE (CPU_TRACE, "CcxZen3S3ResumeLateSmi Exit\n");
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * This routine runs after S3 boot script has been executed and does the following:
 * - Force recalculation of TSC after custom Pstates registers have been restored
 */
/*----------------------------------------------------------------------------------------*/
VOID
CcxZen3S3ResumeLateAp (
  IN       VOID *Buffer
  )
{
  UINT64 LocalMsrRegister;

  // Force recalc of TSC
  LocalMsrRegister = AsmReadMsr64 (0xC0010064);
  AsmWriteMsr64 (0xC0010064, LocalMsrRegister);

  // Clear and set again CpbDis
  LocalMsrRegister = AsmReadMsr64 (0xC0010015);
  if ((LocalMsrRegister & BIT25) == BIT25) {
    AsmMsrAnd64 (0xC0010015, ~((UINT64) BIT25));
    AsmMsrOr64 (0xC0010015, BIT25);
  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Perform WBINVD just before entering S3
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
CcxZen3S3EntryAp (
  IN       VOID *Buffer
  )
{
  AsmWbinvd ();
}

/*----------------------------------------------------------------------------------------*/
/**
 * All threads need to perform WBINVD just before entering S3
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
CcxS3Callback (
  IN       EFI_HANDLE                          DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT   *DispatchContext,
  IN OUT   VOID                                *CommBuffer OPTIONAL,
  IN OUT   UINTN                               *CommBufferSize  OPTIONAL
  )
{
  UINT16      i;
  UINT64      MsrValue;

  IDS_HDT_CONSOLE (CPU_TRACE, "CCX S3 Callback\n");
  // Read value of MSRC001_0015[21] and update the value in mMsrSaveRestoreTable
  MsrValue = (AsmReadMsr64 (0xC0010015) & 0x0000000000200000);
  IDS_HDT_CONSOLE (CPU_TRACE, "C0010015 %X\n", MsrValue);
  for (i = 0; mMsrSaveRestoreTable[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    if (mMsrSaveRestoreTable[i].MsrAddr == 0xC0010015) {
      mMsrSaveRestoreTable[i].MsrData |= MsrValue;
      break;
    }
  }


  CcxZen3S3EntryAp (NULL);
//[-start-220225-IB14740218-modify]//
  for (i = 0; i < gSmst->NumberOfCpus; i++) {
    if (i != gSmst->CurrentlyExecutingCpu) {
      gSmst->SmmStartupThisAp (
              CcxZen3S3EntryAp,
              i,
              NULL
              );
    }
  }
//[-end-220225-IB14740218-modify]//

  return (EFI_SUCCESS);
}


/*----------------------------------------------------------------------------------------*/
/**
 * AMD Zen3 driver main entry point
 *
 *
 * @param[in]       ImageHandle           Image Handle
 * @param[in]       SystemTable           Pointer to system globals
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
AmdCcxZen3SmmDriverEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                      Status;
  PSP_RESUME_SERVICE_PROTOCOL    *PspResume;
  FCH_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT     SwContext;
  EFI_HANDLE                      SwHandle;
  FCH_SMM_SX_DISPATCH2_PROTOCOL  *SxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT     SxContext;
  EFI_HANDLE                      SxHandle;
  LOCATE_HEAP_PTR                 LocateHeapParams;
  VOID                            *GuidHob;
  EFI_SMRAM_DESCRIPTOR            *SmramDescriptor;
  SMM_S3_RESUME_STATE             *SmmS3ResumeState;

  AGESA_TESTPOINT (TpCcxSmmEntry, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3Smm Entry\n");

  mPcdAmdPspS3WakeFromSmm = PcdGetBool (PcdAmdPspS3WakeFromSmm);
  if (!mPcdAmdPspS3WakeFromSmm) {
    GuidHob = GetFirstGuidHob (&gEfiAcpiVariableGuid);
    if (GuidHob != NULL) {
      SmramDescriptor = (EFI_SMRAM_DESCRIPTOR *) GET_GUID_HOB_DATA (GuidHob);
      SmmS3ResumeState = (SMM_S3_RESUME_STATE *)(UINTN)SmramDescriptor->CpuStart;
      ((PSP_SMM_HDR_DATA*)SmmS3ResumeState)->CcxDataRegion = (EFI_PHYSICAL_ADDRESS)mMsrSaveRestoreTable;
    }
  }

  LocateHeapParams.BufferHandle = AMD_PFEH_HANDLE;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    mPfehMode = *((BOOLEAN *) LocateHeapParams.BufferPtr);
  }

  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSwDispatch2ProtocolGuid,
                  NULL,
                  &SwDispatch
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  Locate gFchSmmSwDispatch2ProtocolGuid failed\n");
    return EFI_SUCCESS;
  }

  // Install Save MSRs SMI handler
  SwContext.AmdSwValue  = PcdGet8 (PcdAmdCcxS3SaveSmi);
  SwContext.Order       = 0x80;
  Status = SwDispatch->Register (
                          SwDispatch,
                          CcxZen3S3SaveSmi,
                          &SwContext,
                          &SwHandle
                          );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3S3SaveSmi is not registered\n");
    return EFI_SUCCESS;
  }

  // Install S3 resume late handler
  SwContext.AmdSwValue  = PcdGet8 (PcdFchOemBeforePciRestoreSwSmi);
  SwContext.Order       = 0x80;
  Status = SwDispatch->Register (
                          SwDispatch,
                          CcxZen3S3ResumeLateSmi,
                          &SwContext,
                          &SwHandle
                          );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3S3ResumeLateSmi is not registered\n");
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSxDispatch2ProtocolGuid,
                  NULL,
                  &SxDispatch
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Register a callback on S3 entry as one of the last hooks to run
  SxContext.Type  = SxS3;
  SxContext.Phase = SxEntry;
  SxContext.Order = 0xFE;

  IDS_HDT_CONSOLE (CPU_TRACE, "\tRegister Sleep SMI\n");
  Status = SxDispatch->Register (
                          SxDispatch,
                          CcxS3Callback,
                          &SxContext,
                          &SxHandle
                          );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (mPcdAmdPspS3WakeFromSmm) {
    // Install Restore MSRs callback
    Status = gSmst->SmmLocateProtocol (
                    &gAmdPspResumeServiceProtocolGuid,
                    NULL,
                    &PspResume
                    );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  Locate gAmdPspResumeServiceProtocolGuid failed\n");
      return EFI_SUCCESS;
    }
    PspResume->Register (PspResume, CcxZen3S3RestoreSmi, mMsrSaveRestoreTable, PSP_RESUME_CALLBACK_HIGHEST_LEVEL, PspResumeCallBackFlagAllCores, NULL);
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3Smm End\n");

  AGESA_TESTPOINT (TpCcxSmmExit, NULL);

  return EFI_SUCCESS;
}



