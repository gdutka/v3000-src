/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Cf9Reset.h"
#define FILECODE FCH_KEITH_FCHKEITHCF9RESETDXE_CF9RESET_FILECODE

extern EFI_GUID gEfiAmdAgesaSpecificWarmResetGuid;
EFI_EVENT              mVirtualNotifyEvent;
PHYSICAL_ADDRESS       mFchAcpiMmio;

VOID SpecificWarmResetSystem (
    IN EFI_RESET_TYPE ResetType
){
  UINT8   InitialData;
  UINT8   OutputData;

  DEBUG ((DEBUG_INFO, "[SpecificWarmResetSystem] ResetSystem invoked:  ResetType = %d\n", ResetType));

  InitialData = HARDSTARTSTATE;
  OutputData = HARDRESET;

  ACPIMMIO8(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG10) &= 0xFD;  //clear ToggleAllPwrGoodOnCf9

  IoWrite8 (FCH_IOMAP_REGCF9, InitialData);
  IoWrite8 (FCH_IOMAP_REGCF9, OutputData);
  //
  // Given we should have reset getting here would be bad
  //
  FCH_DEADLOOP();
}

//
// The handle onto which the Reset Architectural Protocol is installed
//
EFI_HANDLE  mResetHandle = NULL;

VOID
EFIAPI
Cf9ResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
/*++

Routine Description:

  Reset the system.

Arguments:

    ResetType - warm or cold
    ResetStatus - possible cause of reset
    DataSize - Size of ResetData in bytes
    ResetData - Optional Unicode string
    For details, see efiapi.h

Returns:
  Does not return if the reset takes place.
  EFI_INVALID_PARAMETER   If ResetType is invalid.

--*/
{
  UINT8      InitialData;
  UINT8      OutputData;
  //UINT8      PwrRsrCfg;
  UINT16     AcpiGpeBase;
  UINT16     AcpiPm1StsBase;
  UINT16     AcpiPm1CntBase;
  UINT32     Gpe0Enable;
  UINT16     PmCntl;
  UINT16     PwrSts;
  BOOLEAN    IsFullHardReset = FALSE;
  UINTN      ResetDataStringSize;
  EFI_GUID   *ResetTypeGuid;

  DEBUG ((DEBUG_INFO, "[FchCf9Reset] ResetSystem invoked:  ResetType = %d\n", ResetType));
  switch (ResetType) {
  //
  // For update resets, the reset data is a null-terminated string followed
  // by a VOID * to the capsule descriptors. Get the pointer and set the
  // capsule variable before we do a warm reset. Per the EFI 1.10 spec, the
  // reset data is only valid if ResetStatus != EFI_SUCCESS.
  //
  case EfiResetWarm:
    InitialData = HARDSTARTSTATE;
    OutputData  = HARDRESET;
    break;

  case EfiResetCold:
    IsFullHardReset = PcdGetBool (PcdFchFullHardReset);
    if (IsFullHardReset){
      InitialData = FULLSTARTSTATE;
      OutputData  = FULLRESET;
    }else{
      InitialData = HARDSTARTSTATE;
      OutputData  = HARDRESET;
    }
    //IoWrite8 (FCH_IOMAP_REGCD6, 0x10);
    //PwrRsrCfg = IoRead8 (FCH_IOMAP_REGCD7);
    //PwrRsrCfg = PwrRsrCfg | BIT1; //set ToggleAllPwrGoodOnCf9
    //IoWrite8 (FCH_IOMAP_REGCD7, PwrRsrCfg);
    ACPIMMIO8(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG10) |= BIT1;
    break;

  case EfiResetPlatformSpecific:
    InitialData = HARDSTARTSTATE;
    OutputData  = HARDRESET;
    if ((DataSize >= sizeof(EFI_GUID)) && (ResetData != NULL)){
      ResetDataStringSize = StrnSizeS (ResetData, (DataSize / sizeof(CHAR16)));
      if ((ResetDataStringSize < DataSize) && ((DataSize - ResetDataStringSize) >= sizeof(EFI_GUID))){
        ResetTypeGuid = (EFI_GUID *)((UINT8 *)ResetData + ResetDataStringSize);
        if (CompareGuid(&gEfiAmdAgesaSpecificWarmResetGuid, ResetTypeGuid)){
          SpecificWarmResetSystem (EfiResetWarm);
        }
      }
    }
    break;

  case EfiResetShutdown:
    // Disable all GPE0 Event
    // FchReadPmio (FCH_PMIOA_REG68, AccessWidth16, &AcpiGpeBase);
    AcpiGpeBase = ACPIMMIO16(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG68);
    AcpiGpeBase += 4; //Get enable base
    Gpe0Enable  = 0;
    IoWrite32 (AcpiGpeBase, Gpe0Enable);

    // Clear Power Button status.
    // FchReadPmio (FCH_PMIOA_REG60, AccessWidth16, &AcpiPm1StsBase);
    AcpiPm1StsBase = ACPIMMIO16(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG60);
    PwrSts  = BIT8 | BIT15; //Clear WakeStatus with PwrBtnStatus
    IoWrite16 (AcpiPm1StsBase, PwrSts);

    // Transform system into S5 sleep state
    // FchReadPmio (FCH_PMIOA_REG62, AccessWidth16, &AcpiPm1CntBase);
    AcpiPm1CntBase = ACPIMMIO16(mFchAcpiMmio + PMIO_BASE + FCH_PMIOA_REG62);
    PmCntl  = IoRead16 (AcpiPm1CntBase);
    PmCntl  = (PmCntl & ~SLP_TYPE) | SUS_S5 | SLP_EN;
    IoWrite16 (AcpiPm1CntBase, PmCntl);
    return ;

  default:
    return ;
  }

  IoWrite8 (FCH_IOMAP_REGCF9, InitialData);
  IoWrite8 (FCH_IOMAP_REGCF9, OutputData);

  //
  // Given we should have reset getting here would be bad
  //
  FCH_DEADLOOP();
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed.
  @param[in]    Context The Event Context.
**/
VOID
EFIAPI
VirtualMemoryNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  //
  // Update global for Runtime Services Table and IO
  //
  gRT->ConvertPointer (0, (VOID **) &mFchAcpiMmio);
}

/********************************************************************************
 * Name: InitializeCf9Reset
 *
 * Description
 *   Cf9ResetDxe Entrypoint
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
InitializeCf9Reset (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
/*++

Routine Description:

  Initialize the state information for the Timer Architectural Protocol

Arguments:

  ImageHandle of the loaded driver
  Pointer to the System Table

Returns:

  Status

  EFI_SUCCESS           - thread can be successfully created
  EFI_OUT_OF_RESOURCES  - cannot allocate protocol data structure
  EFI_DEVICE_ERROR      - cannot create the timer service

--*/
{
  EFI_STATUS  Status;
  UINT32      ProtocolPointer;

  //
  // Check if the Reset Architectural Protocol is already installed in the system
  //
  Status = gBS->LocateProtocol (&gEfiResetArchProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if ( Status != EFI_NOT_FOUND) {
    //Already installed
    return EFI_UNSUPPORTED;
  }

  mFchAcpiMmio = ACPI_MMIO_BASE;
  //
  // Register SetVirtualAddressMap () notify function
  //
  Status = gBS->CreateEvent (
                  EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
                  TPL_NOTIFY,
                  VirtualMemoryNotifyEvent,
                  NULL,
                  &mVirtualNotifyEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Hook the runtime service table
  //
  SystemTable->RuntimeServices->ResetSystem = Cf9ResetSystem;

  //
  // Now install the Reset RT AP on a new handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mResetHandle,
                  &gEfiResetArchProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
