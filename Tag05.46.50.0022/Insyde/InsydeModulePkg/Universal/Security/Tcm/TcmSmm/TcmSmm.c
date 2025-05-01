/** @file
  It updates TCM items in ACPI table and registers SMI callback function for ClearMemory.

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "TcmSmm.h"

TCG_NVS                    *mTcgNvs;

/**
  This fucntion uses to read saved CPU double word register by CPU index

  @param[in]   SmmCpu        EFI_SMM_CPU_PROTOCOL instance
  @param[in]   RegisterNum   Register number which want to get
  @param[in]   CpuIndex      CPU index number to get register.
  @param[out]  RegisterData  pointer to output register data

  @retval EFI_SUCCESS        Read double word register successfully
  @return Other              Any error occured while disabling all secure boot SMI functions successful.

**/
STATIC
EFI_STATUS
GetDwordRegisterByCpuIndex (
  IN  EFI_SMM_CPU_PROTOCOL              *SmmCpu,
  IN  EFI_SMM_SAVE_STATE_REGISTER       RegisterNum,
  IN  UINTN                             CpuIndex,
  OUT UINT32                            *RegisterData
  )
{
  UINTN                       Width;

  Width = sizeof (UINT32);
  *RegisterData = 0;

  if (RegisterNum == EFI_SMM_SAVE_STATE_REGISTER_CS ||
      RegisterNum == EFI_SMM_SAVE_STATE_REGISTER_DS ||
      RegisterNum == EFI_SMM_SAVE_STATE_REGISTER_ES ||
      RegisterNum == EFI_SMM_SAVE_STATE_REGISTER_SS ||
      RegisterNum == EFI_SMM_SAVE_STATE_REGISTER_FS ||
      RegisterNum == EFI_SMM_SAVE_STATE_REGISTER_GS) {
    Width = sizeof (UINT16);
  }
  return SmmCpu->ReadSaveState (
                   SmmCpu,
                   Width,
                   RegisterNum,
                   CpuIndex,
                   RegisterData
                   );
}

/**
  Get the MOR value from register ESI.

  @retval     The value of ESI.

**/
STATIC
UINT32
GetMorInt1AhEsi (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_SMM_CPU_PROTOCOL                  *SmmCpu;
  UINTN                                 Index;
  UINT32                                Eax;
  UINT32                                Edx;
  UINT32                                Esi;

  //
  // Locate Smm Cpu protocol for Cpu save state manipulation
  //
  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmCpuProtocolGuid,
                    NULL,
                    (VOID **)&SmmCpu
                    );
  if (EFI_ERROR (Status)) {
    return 0;
  }

  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    Status = GetDwordRegisterByCpuIndex (SmmCpu, EFI_SMM_SAVE_STATE_REGISTER_RAX, Index, &Eax);
    ASSERT_EFI_ERROR (Status);
    Status = GetDwordRegisterByCpuIndex (SmmCpu, EFI_SMM_SAVE_STATE_REGISTER_RDX, Index, &Edx);
    ASSERT_EFI_ERROR (Status);
    //
    // Find out which CPU triggered PnP SMI
    //
    if (((Eax & 0xff) == SMM_TCG_MOR_CALL) && 
        ((Edx & 0xffff) == PcdGet16 (PcdSoftwareSmiPort))) {
      //
      // Cpu found!
      //
      break;
    }
  }
  if (Index == gSmst->NumberOfCpus) {
    //
    // Error out due to CPU not found
    //
    return 0;
  }

  Status = GetDwordRegisterByCpuIndex (SmmCpu, EFI_SMM_SAVE_STATE_REGISTER_RSI, Index, &Esi);
  ASSERT_EFI_ERROR (Status);
  
  return Esi;
}

/**
  TCM measure and log data, and extend the measurement result into a specific PCR.

  @param[in]  PcrIndex         PCR Index.
  @param[in]  EventType        Event type.
  @param[in]  EventLog         Measurement event log.
  @param[in]  LogLen           Event log length in bytes.
  @param[in]  HashData         The start of the data buffer to be hashed, extended.
  @param[in]  HashDataLen      The length, in bytes, of the buffer referenced by HashData

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_UNSUPPORTED       TCM device not available.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.
**/
EFI_STATUS
TcmMeasureAndLogData (
  IN UINT32             PcrIndex,
  IN UINT32             EventType,
  IN VOID               *EventLog,
  IN UINT32             LogLen,
  IN VOID               *HashData,
  IN UINT64             HashDataLen
  )
{
  EFI_STATUS                Status;
  EFI_TCG_PROTOCOL          *TcgProtocol;
  TCM_PCR_EVENT             *TcmEvent;
  EFI_PHYSICAL_ADDRESS      EventLogLastEntry;
  UINT32                    EventNumber;

  TcmEvent = NULL;

  //
  // TCM active/deactive state is checked in HashLogExtendEvent
  //
  Status = gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, (VOID **) &TcgProtocol);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (TCM_PCR_EVENT_HDR) + LogLen,
                  (VOID **)&TcmEvent
                  );
  if (EFI_ERROR(Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  TcmEvent->PCRIndex  = PcrIndex;
  TcmEvent->EventType = EventType;
  TcmEvent->EventSize = LogLen;
  CopyMem (&TcmEvent->Event[0], EventLog, LogLen);
  EventNumber = 1;
  Status = TcgProtocol->HashLogExtendEvent (
                          TcgProtocol,
                          (EFI_PHYSICAL_ADDRESS)(UINTN)HashData,
                          HashDataLen,
                          TCM_ALG_SM3_256,
                          (TCG_PCR_EVENT *)TcmEvent,
                          &EventNumber,
                          &EventLogLastEntry
                          );

  gBS->FreePool (TcmEvent);

  return Status;
}

/**
  Software SMI callback for MemoryClear which is called from ACPI method.

  Caution: This function may receive untrusted input.
  Variable and ACPINvs are external input, so this function will validate
  its data structure to be valid value.

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context         Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The interrupt was handled successfully.

**/
EFI_STATUS
EFIAPI
MemoryClearCallback (
  IN EFI_HANDLE                  DispatchHandle,
  IN CONST VOID                  *Context,
  IN OUT VOID                    *CommBuffer,
  IN OUT UINTN                   *CommBufferSize
  )
{
  EFI_STATUS                     Status;
  UINTN                          DataSize;
  UINT8                          MorControl;
  UINT32                         *Esi;

  MorControl = 0;
  mTcgNvs->MemoryClear.ReturnCode = MOR_REQUEST_SUCCESS;
  if (mTcgNvs->MemoryClear.Parameter == ACPI_FUNCTION_DSM_MEMORY_CLEAR_INTERFACE) {
    MorControl = (UINT8) mTcgNvs->MemoryClear.Request;
  } else if (mTcgNvs->MemoryClear.Parameter == ACPI_FUNCTION_PTS_CLEAR_MOR_BIT) {
    DataSize = sizeof (UINT8);
    Status = CommonGetVariable (
               MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
               &gEfiMemoryOverwriteControlDataGuid,
               &DataSize,
               &MorControl
               );
    if (EFI_ERROR (Status)) {
      mTcgNvs->MemoryClear.ReturnCode = MOR_REQUEST_GENERAL_FAILURE;
      DEBUG ((EFI_D_ERROR, "[TCM] Get MOR variable failure! Status = %r\n", Status));
      return EFI_SUCCESS;
    }

    if (MOR_CLEAR_MEMORY_VALUE (MorControl) == 0x0) {
      return EFI_SUCCESS;
    }
    MorControl &= ~MOR_CLEAR_MEMORY_BIT_MASK;
  } else {
    Esi = (UINT32 *)(UINTN)GetMorInt1AhEsi ();
    if ((Esi != NULL) && (*Esi == (UINT32)SIGNATURE_32 ('T', 'M', 'O', 'R'))) {
      MorControl = (UINT8)*(UINT32 *)(Esi + 1);
      DataSize = sizeof (UINT8);
      Status = CommonSetVariable (
                 MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                 &gEfiMemoryOverwriteControlDataGuid,
                 EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                 DataSize,
                 &MorControl
                 );
      return Status;
    }
  }

  DataSize = sizeof (UINT8);
  Status = CommonSetVariable (
             MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
             &gEfiMemoryOverwriteControlDataGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             DataSize,
             &MorControl
             );
  if (EFI_ERROR (Status)) {
    mTcgNvs->MemoryClear.ReturnCode = MOR_REQUEST_GENERAL_FAILURE;
    DEBUG ((EFI_D_ERROR, "[TCM] Set MOR variable failure! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}

/**
  Find the operation region in TCG ACPI table by given Name and Size,
  and initialize it if the region is found.

  @param[in, out] Table          The TCM item in ACPI table.
  @param[in]      Name           The name string to find in TCM table.
  @param[in]      Size           The size of the region to find.

  @return                        The allocated address for the found region.

**/
VOID *
AssignOpRegion (
  EFI_ACPI_DESCRIPTION_HEADER    *Table,
  UINT32                         Name,
  UINT16                         Size
  )
{
  EFI_STATUS                     Status;
  AML_OP_REGION_32_8             *OpRegion;
  EFI_PHYSICAL_ADDRESS           MemoryAddress;

  MemoryAddress = SIZE_4GB - 1;

  //
  // Patch some pointers for the ASL code before loading the SSDT.
  //
  for (OpRegion  = (AML_OP_REGION_32_8 *) (Table + 1);
       OpRegion <= (AML_OP_REGION_32_8 *) ((UINT8 *) Table + Table->Length);
       OpRegion  = (AML_OP_REGION_32_8 *) ((UINT8 *) OpRegion + 1)) {
    if ((OpRegion->OpRegionOp  == AML_EXT_REGION_OP) &&
        (OpRegion->NameString  == Name) &&
        (OpRegion->DWordPrefix == AML_DWORD_PREFIX) &&
        (OpRegion->BytePrefix  == AML_BYTE_PREFIX)) {

      Status = gBS->AllocatePages(AllocateMaxAddress, EfiACPIMemoryNVS, EFI_SIZE_TO_PAGES (Size), &MemoryAddress);
      ASSERT_EFI_ERROR (Status);
      ZeroMem ((VOID *)(UINTN)MemoryAddress, Size);
      OpRegion->RegionOffset = (UINT32) (UINTN) MemoryAddress;
      OpRegion->RegionLen    = (UINT8) Size;
      break;
    }
  }

  return (VOID *) (UINTN) MemoryAddress;
}

/**
  Initialize and publish TCM items in ACPI table.

  @retval   EFI_SUCCESS     The TCG ACPI table is published successfully.
  @retval   Others          The TCG ACPI table is not published.

**/
EFI_STATUS
PublishAcpiTable (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;
  UINTN                          TableKey;
  EFI_ACPI_DESCRIPTION_HEADER    *Table;
  UINTN                          TableSize;

  Status = GetSectionFromFv (
             &gEfiCallerIdGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &Table,
             &TableSize
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Measure to PCR[0] with event EV_POST_CODE ACPI DATA
  //
  TcmMeasureAndLogData (
    0,
    EV_POST_CODE,
    EV_POSTCODE_INFO_ACPI_DATA,
    ACPI_DATA_LEN,
    Table,
    TableSize
    );

  ASSERT (Table->OemTableId == SIGNATURE_64 ('T', 'c', 'm', 'T', 'a', 'b', 'l', 'e'));
  CopyMem (Table->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Table->OemId) );
  mTcgNvs = AssignOpRegion (Table, SIGNATURE_32 ('T', 'N', 'V', 'S'), (UINT16) sizeof (TCG_NVS));
  ASSERT (mTcgNvs != NULL);
  if (mTcgNvs == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Publish the TCM ACPI table
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TableKey = 0;
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        Table,
                        TableSize,
                        &TableKey
                        );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  The driver's entry point.

  It install callbacks for MemoryClear, and locate
  SMM variable to be used in the callback function.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval Others          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
InitializeTcgSmm (
  IN EFI_HANDLE                  ImageHandle,
  IN EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT    SwContext;
  EFI_HANDLE                     SwHandle;

  if (!CompareGuid (PcdGetPtr (PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTcmGuid)) {
    DEBUG ((EFI_D_ERROR, "No TCM instance required!\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGetBool (PcdTpmHide)) {
    DEBUG((EFI_D_INFO, "TCM is hidden\n"));
    return EFI_UNSUPPORTED;
  }

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_INSTALL_TCG_ACPI_TABLE) != 0) {
    return EFI_UNSUPPORTED;
  }

  Status = PublishAcpiTable ();
  ASSERT_EFI_ERROR (Status);

  //
  // Get the Sw dispatch protocol and register SMI callback functions.
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  ASSERT_EFI_ERROR (Status);

  SwContext.SwSmiInputValue = (UINTN) SMM_TCG_MOR_CALL;
  Status = SwDispatch->Register (SwDispatch, MemoryClearCallback, &SwContext, &SwHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mTcgNvs->MemoryClear.SoftwareSmi = (UINT8) SwContext.SwSmiInputValue;

  return EFI_SUCCESS;
}

