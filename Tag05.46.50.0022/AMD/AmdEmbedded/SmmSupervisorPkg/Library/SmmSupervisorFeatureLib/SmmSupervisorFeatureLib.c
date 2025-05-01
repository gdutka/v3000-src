/**
* Copyright (C) 2017-2023 Advanced Micro Devices, Inc. All rights reserved.
*
* SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/SmmSupervisorFeatureLib.h>
#include <Protocol/SmmBase2.h>


// Smm Supervisor related
STATIC SMM_SUPERVISOR_PROTOCOL  *mSmmSupervisorProtocol = NULL;
STATIC EFI_MP_SERVICES_PROTOCOL *mMpService;
extern UINT8             SmiRendezvousEntry;

UINTN                     mNumberOfCPUs = 0;
//
// Variables used by SMI Handler
//
extern IA32_DESCRIPTOR   gSmiHandlerIdtr;

///
/// Offset of SMM handler from SMBASE
///
#define SMM_HANDLER_OFFSET           0x8000

STATIC
EFI_STATUS
InitSmst (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_SMM_BASE2_PROTOCOL  *InternalSmmBase2;

  InternalSmmBase2 = NULL;
  //
  // Retrieve SMM Base2 Protocol,  Do not use gBS from UefiBootServicesTableLib on purpose
  // to prevent inclusion of gBS, gST, and gImageHandle from SMM Drivers unless the
  // SMM driver explicitly declares that dependency.
  //
  Status = SystemTable->BootServices->LocateProtocol (
                                        &gEfiSmmBase2ProtocolGuid,
                                        NULL,
                                        (VOID **)&InternalSmmBase2
                                        );
  ASSERT_EFI_ERROR (Status);
  ASSERT (InternalSmmBase2 != NULL);

  //
  // We are in SMM, retrieve the pointer to SMM System Table
  //
  InternalSmmBase2->GetSmstLocation (InternalSmmBase2, &gSmst);
  ASSERT (gSmst != NULL);

  return EFI_SUCCESS;
}


/**
  The constructor function

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS      The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmSupervisorFeatureLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;
  UINTN                NumberOfEnabledCPUs = 0;

  if (gBS == NULL) {
    gBS = SystemTable->BootServices;
  }

  if (gSmst == NULL) {
    InitSmst (ImageHandle, SystemTable);
  }

  if (!PcdGetBool (PcdSmmIsolationEnable)) {
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmLocateProtocol (&gSmmSupervisorProtocolGuid, NULL, (VOID **)&mSmmSupervisorProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to locate Smm Supervisor protocol - %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return EFI_SUCCESS;
  }

  Status = mSmmSupervisorProtocol->TurnOnSmmIsoAtEvent (FixedPcdGet8 (PcdTurnOnSmmIsolationEvent));
  if (EFI_ERROR(Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a Failed to configure SMM isolation enablement to %d - %r\n",
      __FUNCTION__,
      FixedPcdGet8 (PcdTurnOnSmmIsolationEvent),
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mMpService);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to locate MpService Protocol - %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = mMpService->GetNumberOfProcessors (
                        mMpService,
                        &mNumberOfCPUs,
                        &NumberOfEnabledCPUs
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Return the size, in bytes, of a custom SMI Handler in bytes.  If 0 is
  returned, then a custom SMI handler is not provided by this library,
  and the default SMI handler must be used.

  @retval 0    Use the default SMI handler.
  @retval > 0  Use the SMI handler installed by SmmCpuFeaturesInstallSmiHandler()
               The caller is required to allocate enough SMRAM for each CPU to
               support the size of the custom SMI handler.
**/
UINTN
EFIAPI
SupervisorGetSmiHandlerSize (
  VOID
  )
{
  if (mSmmSupervisorProtocol != NULL) {
    return mSmmSupervisorProtocol->SmiHandlerSize;
  } else {
    return 0;
  }
}

/**
  Install a custom SMI handler for the CPU specified by CpuIndex.  This function
  is only called if SmmCpuFeaturesGetSmiHandlerSize() returns a size is greater
  than zero and is called by the CPU that was elected as monarch during System
  Management Mode initialization.

  @param[in] CpuIndex   The index of the CPU to install the custom SMI handler.
                        The value must be between 0 and the NumberOfCpus field
                        in the System Management System Table (SMST).
  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
  @param[in] SmiStack   The stack to use when an SMI is processed by the
                        the CPU specified by CpuIndex.
  @param[in] StackSize  The size, in bytes, if the stack used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtBase    The base address of the GDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtSize    The size, in bytes, of the GDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtBase    The base address of the IDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtSize    The size, in bytes, of the IDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] Cr3        The base address of the page tables to use when an SMI
                        is processed by the CPU specified by CpuIndex.
**/
VOID
EFIAPI
SupervisorInstallSmiHandler (
  IN UINTN   CpuIndex,
  IN UINT32  SmBase,
  IN VOID    *SmiStack,
  IN UINTN   StackSize,
  IN UINTN   GdtBase,
  IN UINTN   GdtSize,
  IN UINTN   IdtBase,
  IN UINTN   IdtSize,
  IN UINT32  Cr3
  )
{
  VOID                      *SmmEntryAddr;
  UINT32                    SmmEntrySize;

  SmmEntryAddr = NULL;

  if (mSmmSupervisorProtocol != NULL) {
    mSmmSupervisorProtocol->InstallSmiHandler (
      CpuIndex,
      SmBase,
      SmiStack,
      StackSize,
      Cr3,
      GdtBase,
      GdtSize,
      (UINT64)&gSmiHandlerIdtr,
      (UINT64)&SmiRendezvousEntry,
      (UINT8 **)&SmmEntryAddr,
      &SmmEntrySize
    );
  } else {
    return;
  }

  ASSERT (SmmEntryAddr != NULL);
  gSmiHandlerIdtr.Base  = IdtBase;
  gSmiHandlerIdtr.Limit = (UINT16)(IdtSize - 1);

  //
  // Copy template to CPU specific SMI handler location
  //
  CopyMem (
    (VOID*)((UINTN)SmBase + SMM_HANDLER_OFFSET),
    SmmEntryAddr,
    SmmEntrySize
    );

}

VOID
EFIAPI
SupervisorSmmRelocationComplete (
  VOID
  )
{
  UINTN                     CpuIndex;
  BASE_LIBRARY_JUMP_BUFFER  **JumpBuffer = NULL;

  if (mSmmSupervisorProtocol->Version >= SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT) {
    // This feature has been deprecated since SMM_SUPV_PROTOCOL_VER_VIOLATION_ACT
    // Do nothing if applicable
    return;
  }

  JumpBuffer = AllocatePool (mNumberOfCPUs * sizeof (UINTN));
  if (JumpBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Allocate JumpBuffer for all cores failed\n", __FUNCTION__));
    ASSERT (FALSE);
    return;
  } else {
    DEBUG ((DEBUG_INFO, "%a Allocated JumpBuffer for all cores %p\n", __FUNCTION__, JumpBuffer));
  }

  mSmmSupervisorProtocol->ErrorReportJumpBuffer = (UINTN) JumpBuffer;

  for (CpuIndex = 0; CpuIndex < mNumberOfCPUs; CpuIndex++) {
    JumpBuffer[CpuIndex] = AllocateZeroPool (sizeof (BASE_LIBRARY_JUMP_BUFFER));
    if (JumpBuffer[CpuIndex] == NULL) {
      ASSERT (FALSE);
    } else {
      DEBUG ((DEBUG_INFO, "%a Allocated JumpBuffer for core index %d - %p\n", __FUNCTION__, CpuIndex, JumpBuffer[CpuIndex]));
    }
  }
}
