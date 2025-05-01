/*
*******************************************************************************

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************
*/

#include "SupvTestCase.h"

  FCH_SMM_SW_DISPATCH2_PROTOCOL         *mAmdSwDispatch;
  #define SMM_ENTRY_BASE 0x8000
EFI_STATUS
EFIAPI
TamperPageTable (
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  // Address get from log
  MmioWrite32(0x7DEE6000,0xAFAFAFAF);

  return Status;
}

EFI_STATUS
RegisterTamperPageTable (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_PAGE_TABLE_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperPageTable,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
TamperCpl0Stack (
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  MmioWrite8(0x7D666000,0xAA);
  return Status;
}

EFI_STATUS
RegisterTamperCpl0Stack (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_CPL0_STACK_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperCpl0Stack,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
TamperCpl0Heap(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  MmioWrite8(0x7DF0C718,0xAA);
  return Status;
}

EFI_STATUS
RegisterTamperCpl0Heap (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_CPL0_HEAP_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperCpl0Heap,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
TamperSmiEntryRegion(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64 SmmBaseAddress;
  UINT64 SmiEntryBase;

  SmmBaseAddress = AsmReadMsr64 (MSR_SMM_BASE);
  SmiEntryBase = SmmBaseAddress + SMM_ENTRY_BASE;

  // Should hang here
  MmioWrite32(SmiEntryBase, 0xAFAFAFAF);

  return Status;
}

EFI_STATUS
EFIAPI
RegisterTamperSmiEntryRegion (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_SMIENTRY_REGION_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperSmiEntryRegion,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
TamperSmmsupvDriverMemRegion(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  MmioWrite32(0x73F20018, 0xAFAFAFAF);
  return Status;
}

EFI_STATUS
RegisterTamperSmmsupvDriverMemRegion (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_SMMSUPV_DRIVER_MEM_REGION_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperSmmsupvDriverMemRegion,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
TamperGdtMemRegion(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;

  MmioWrite32(0x7AED8000, 0xAFAFAFAF);
  return Status;
}

EFI_STATUS
RegisterTamperGdtMemRegion (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_GDT_MEM_REGION_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperGdtMemRegion,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
TamperSmmSaveStaeRegion(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64 SaveStateBase;

  SaveStateBase = 0x7D912000 + 0xFE00;

  MmioWrite32(SaveStateBase, 0xAFAFAFAF);
  return Status;
}

EFI_STATUS
RegisterTamperSmmSaveStaeRegion (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = TAMPER_SMM_SAVESTAE_REGION_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            TamperSmmSaveStaeRegion,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
ExecutePrivateInstruction(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  AsmIoWrite8 (0xCF8, 0xAA);
  return Status;
}

EFI_STATUS
RegisterExecutePrivateInstruction (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = EXECUTE_PRIVATE_INSTRUCTION_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            ExecutePrivateInstruction,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}


EFI_STATUS
EFIAPI
IOAccessVoliateSecurePolicy(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;

  IoWrite8(0x3F8, 0xAA);
  return Status;
}

EFI_STATUS
RegisterIOAccessVoliateSecurePolicy (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = IO_ACCESS_VOLIATE_SECURE_POLICY_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            IOAccessVoliateSecurePolicy,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
MSRAccessVoliateSecurePolicy(
  IN       EFI_HANDLE                      DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT     *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT              *SwContext,
  IN OUT   UINTN                           *SizeOfSwContext
)
{
  EFI_STATUS  Status = EFI_SUCCESS;

  AsmWriteMsr64 (0xC0000080, 0xAF);
  return Status;
}

EFI_STATUS
RegisterMSRAccessVoliateSecurePolicy (
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE                            Handle;
  FCH_SMM_SW_REGISTER_CONTEXT           SwContext;

  //
  // Need to check each IBV's SW value setting, can't duplicate define
  //
  SwContext.AmdSwValue = MSR_ACCESS_VOLIATE_SECURE_POLICY_SMI;
  SwContext.Order = 0x80;
  Status = mAmdSwDispatch->Register (
                            mAmdSwDispatch,
                            MSRAccessVoliateSecurePolicy,
                            &SwContext,
                            &Handle
                            );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}

EFI_STATUS
CorruptSmmSupvCodeSection (
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "CorruptSmmSupvCodeSection\n"));



  //
  // 0x12F0 is get from .map
  //
  MmioWrite8((UINT32)(UINTN)0x7DEE6000 + 0x12F0, 0xAF);

  return Status;
}

EFI_STATUS
CorruptSmiEntryBin (
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64  SmmBaseAddress;
  UINT64  SmiEntryBase;
  UINT64  CorruptEntryAddress;

  SmmBaseAddress = AsmReadMsr64 (MSR_SMM_BASE);
  SmiEntryBase = SmmBaseAddress + SMM_ENTRY_BASE;

  CorruptEntryAddress = SmiEntryBase + 0x37;

  MmioWrite8(CorruptEntryAddress , 0x8F);

  return Status;
}

EFI_STATUS
MpServicesTest (
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  UINTN                     ProcessorNum;
  UINTN                     EnabledProcessorNum;


  DEBUG ((DEBUG_INFO, "MpServicesTest\n"));
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  ASSERT (!EFI_ERROR (Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = MpServices->GetNumberOfProcessors(
                         MpServices,
                         &ProcessorNum,
                         &EnabledProcessorNum
                         );
  if (EFI_ERROR(Status)){
    DEBUG ((DEBUG_INFO, "MpServices Failed. %x\n", Status));
    ASSERT(FALSE);
    return Status;
  }

  return Status;
}

EFI_STATUS
SmmSetVariableTest (
  )
{
  EFI_STATUS                   Status;
  EFI_SMM_VARIABLE_PROTOCOL    *SmmVariable;
  BOOLEAN                      Test = TRUE;


  DEBUG ((DEBUG_INFO, "SmmSetVariableTest\n"));
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmVariableProtocolGuid,
                  NULL,
                  (VOID**)&SmmVariable
                  );
  if (EFI_ERROR(Status)){
    DEBUG ((DEBUG_INFO, "gEfiSmmVariableProtocolGuid Failed. %x\n", Status));
    ASSERT(FALSE);
    return Status;
  }
  Status = SmmVariable->SmmSetVariable (
                          L"Test",
                          &AmdTestVariableGuid,
                          EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                          sizeof (BOOLEAN),
                          &Test
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "SmmSetVariableTest failure! Status = %r\n", Status));
    return EFI_SUCCESS;
  }
  return Status;
}

EFI_STATUS
SupvTestCaseEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;

  //
  //  Locate the SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (&gFchSmmSwDispatch2ProtocolGuid, NULL, &mAmdSwDispatch);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  RegisterTamperPageTable();
  RegisterTamperCpl0Stack();
  RegisterTamperCpl0Heap();
  RegisterTamperSmiEntryRegion();
  RegisterTamperSmmsupvDriverMemRegion();
  RegisterTamperGdtMemRegion();
  RegisterTamperSmmSaveStaeRegion();
  RegisterExecutePrivateInstruction();
  RegisterIOAccessVoliateSecurePolicy();
  RegisterMSRAccessVoliateSecurePolicy();


  // CorruptSmiEntryBin();
  // CorruptSmmSupvCodeSection();
  MpServicesTest();
  SmmSetVariableTest();
  return Status;
}