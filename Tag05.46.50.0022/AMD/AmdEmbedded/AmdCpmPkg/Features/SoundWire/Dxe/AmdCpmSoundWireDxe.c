/******************************************************************************
  Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
*****************************************************************************/

#include <AmdCpmDxe.h>
#include <Library/PcdLib.h>

VOID
EFIAPI
AmdCpmSoundWireInitLate (
  IN      EFI_EVENT       Event,
  IN      VOID            *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Sound Wire DXE driver
 *
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmSoundWireDxeEntryPoint (
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       ReadyToBootEvent;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  if (PcdGet8(PcdSoundWireSsdtSupport) == 0){
    DEBUG((DEBUG_INFO, "SoundWireSsdtSupport is disable.\n"));
    return EFI_SUCCESS;
  }

  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_CALLBACK,
                  AmdCpmSoundWireInitLate,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * The function to load Sound Wire SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmSoundWireInitLate (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  EFI_STATUS                      Status;
  UINT64                          OemTableIdSig;

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      InitlateInvoked = TRUE;
      return;
    }

    if (PcdGet8(PcdSoundWireSsdtSupport) == 1) {
      OemTableIdSig = SIGNATURE_64('S','W','_','A','G','G','R','E');
    } else if (PcdGet8(PcdSoundWireSsdtSupport) == 2) {
      OemTableIdSig = SIGNATURE_64('S','W','_','N','O','N','A','G');
    } else {
      return;
    }

    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                        &gEfiCallerIdGuid,
                                                        &OemTableIdSig,
                                                        NULL,
                                                        NULL);
  }

  InitlateInvoked = TRUE;

  return;
}
