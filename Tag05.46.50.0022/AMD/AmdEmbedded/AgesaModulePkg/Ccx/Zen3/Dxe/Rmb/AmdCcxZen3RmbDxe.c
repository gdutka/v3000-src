/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


#include <PiDxe.h>
#include <Protocol/MpService.h>
#include <Library/CcxRolesLib.h>
#include <Library/CcxBaseX86Lib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/CcxSmmAccess2Lib.h>
#include <Library/DxeCcxBaseX86ServicesLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdMpServicesPreReqProtocol.h>
#include <Protocol/AmdCcxProtocol.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/AmdNbioServicesProtocol.h>
#include <Protocol/VariableLock.h>
#include "CcxZen3AcpiServicesDxe.h"
#include "CcxZen3SmbiosDxe.h"
#include <Filecode.h>

#define FILECODE CCX_ZEN3_DXE_RMB_AMDCCXZEN3RMBDXE_FILECODE

extern EFI_GUID gAmdDownCoreStatusGuid;

/*----------------------------------------------------------------------------------------
 *                               D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_EVENT            CcxZen3InitWithMpServicesEvent;
EFI_EVENT            CcxZen3RmbOcCallbackEvent;
EFI_EVENT            CcxZen3RmbOcCallbackOnMpServicesEvent;


VOID                 *mRegistrationForCcxZen3InitWithMpServicesEvent;
VOID                 *mRegistrationForCcxZen3RmbOcCallbackEvent;
VOID                 *mRegistrationForCcxZen3OcOnMpServicesCallbackEvent;
DXE_AMD_CCX_INIT_COMPLETE_PROTOCOL    mCcxDxeInitCompleteProtocol;
DXE_AMD_CCX_OC_COMPLETE_PROTOCOL      mCcxOcCompleteProtocol;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
EFIAPI
CcxZen3InitWithMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
CcxZen3RmbOcCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
CcxZen3RmbOcCallbackOnMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
CcxReadyToBoot (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
CcxCheckDownCoreStatus (
  );

VOID
EFIAPI
CcxCheckDownCoreStatusCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

/* -----------------------------------------------------------------------------*/
/**
 *
 *  AmdCcxZen3RmbDxeInit
 *
 *  @param[in]  ImageHandle     EFI Image Handle for the DXE driver
 *  @param[in]  SystemTable     Pointer to the EFI system table
 *
 *  Description:
 *    Zen3 RMB Driver Entry.  Initialize the core complex.
 *
 *  @retval EFI_STATUS
 *
 */
EFI_STATUS
EFIAPI
AmdCcxZen3RmbDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                               Status;
  EFI_STATUS                               CalledStatus;
  AMD_CONFIG_PARAMS                        StdHeader;
  EFI_HANDLE                               Handle;
  DXE_AMD_MP_SERVICES_PREREQ_PROTOCOL      AmdMpServicesPreReqProtocol;
  EFI_EVENT                                ReadyToBootEvent;
  ALLOCATE_HEAP_PARAMS                     AllocParams;
  EFI_EVENT                                EfiVariableWriteEvent;
  VOID                                    *RegistrationForEfiVariableWriteEvent;
  VOID                                    *IgnoredProtocol;
  EFI_MP_SERVICES_PROTOCOL                *MpServices;

  AGESA_TESTPOINT (TpCcxDxeEntry, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3RmbDxeInit Entry\n");

  Status = EFI_SUCCESS;

  if (CcxIsBsp (&StdHeader)) {

    // Check downcore status, and trigger a cold reset if needed
    CalledStatus = gBS->LocateProtocol (&gEfiVariableWriteArchProtocolGuid, NULL, &IgnoredProtocol);
    if (!EFI_ERROR (CalledStatus)) {
      CcxCheckDownCoreStatus ();
    } else {
      //
      // Set up call back after NV variable service are available.
      //
      CalledStatus = gBS->CreateEventEx (
                             EVT_NOTIFY_SIGNAL,
                             TPL_NOTIFY,
                             CcxCheckDownCoreStatusCallback,
                             NULL,
                             NULL,
                             &EfiVariableWriteEvent
                             );
      ASSERT (CalledStatus == EFI_SUCCESS);
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      CalledStatus = gBS->RegisterProtocolNotify (
                            &gEfiVariableWriteArchProtocolGuid,
                            EfiVariableWriteEvent,
                            &(RegistrationForEfiVariableWriteEvent)
                            );
      ASSERT (CalledStatus == EFI_SUCCESS);
      Status = (CalledStatus > Status) ? CalledStatus : Status;
    }

    // Publish CCX services protocol
    CcxBaseServicesProtocolInstall (ImageHandle, SystemTable);

    // Install gAmdMpServicesPreReqProtocolGuid protocol
    AmdMpServicesPreReqProtocol.Revision = AMD_MP_SERVICES_PREREQ_PROTOCOL_REVISION;
    Handle = NULL;
    CalledStatus = gBS->InstallProtocolInterface (
                            &Handle,
                            &gAmdMpServicesPreReqProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &AmdMpServicesPreReqProtocol
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    //
    // Set up call back after MP services are available.
    //
    CalledStatus = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&MpServices);
    if (!EFI_ERROR (CalledStatus)) {
      Status = CcxSmmAccess2ProtocolInstall ();
      if (Status != EFI_SUCCESS) {
        IDS_HDT_CONSOLE (CPU_TRACE, "   WARNING! CcxSmmAccess2ProtocolInstall was not installed\n");
      }

      IDS_HOOK (IDS_HOOK_CCX_AFTER_PWR_MNG, NULL, NULL);
    } else {
      CalledStatus = gBS->CreateEventEx (
                               EVT_NOTIFY_SIGNAL,
                               TPL_NOTIFY,
                               CcxZen3InitWithMpServices,
                               NULL,
                               NULL,
                               &CcxZen3InitWithMpServicesEvent
                               );
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      CalledStatus = gBS->RegisterProtocolNotify (
                              &gEfiMpServiceProtocolGuid,
                              CcxZen3InitWithMpServicesEvent,
                              &(mRegistrationForCcxZen3InitWithMpServicesEvent)
                              );
      Status = (CalledStatus > Status) ? CalledStatus : Status;
    }

    //
    // Set up call back for OverClock.
    //
    CalledStatus = gBS->CreateEventEx (
                             EVT_NOTIFY_SIGNAL,
                             TPL_NOTIFY,
                             CcxZen3RmbOcCallback,
                             NULL,
                             NULL,
                             &CcxZen3RmbOcCallbackEvent
                             );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    CalledStatus = gBS->RegisterProtocolNotify (
                            &gAmdNbioSmuInitCompleteProtocolGuid,
                            CcxZen3RmbOcCallbackEvent,
                            &(mRegistrationForCcxZen3RmbOcCallbackEvent)
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;


    CalledStatus = gBS->CreateEventEx (
               EVT_NOTIFY_SIGNAL,
               TPL_NOTIFY,
               CcxReadyToBoot,
               NULL,
               &gEfiEventReadyToBootGuid,
               &ReadyToBootEvent
               );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI CPU SSDT services protocol
    CalledStatus = CcxZen3AcpiCpuSsdtServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx CRAT services protocol
    CalledStatus = CcxZen3CratServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx SRAT services protocol
    CalledStatus = CcxZen3SratServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx PCCT services protocol
     CalledStatus = CcxZen3PcctServicesProtocolInstall (ImageHandle, SystemTable);
     Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx CPPC services protocol
     CalledStatus = CcxZen3CppcServicesProtocolInstall (ImageHandle, SystemTable);
     Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx RAS services protocol
    CalledStatus = CcxZen3RasServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish SMBIOS services protocol
    CalledStatus = CcxZen3SmbiosServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Install gAmdCcxDxeInitCompleteProtocolGuid protocol
    mCcxDxeInitCompleteProtocol.Revision = AMD_CCX_PROTOCOL_REVISION;
    Handle = NULL;
    AGESA_TESTPOINT (TpCcxDxeBeforeInstallCcxDoneProtocol, NULL);
    CalledStatus = gBS->InstallProtocolInterface (
                            &Handle,
                            &gAmdCcxDxeInitCompleteProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &mCcxDxeInitCompleteProtocol
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;
    AGESA_TESTPOINT (TpCcxDxeAfterInstallCcxDoneProtocol, NULL);

    // Save PcdAmdCcxCfgPFEHEnable to heap so it could be gotten in SMI handler
    AllocParams.RequestedBufferSize = sizeof (BOOLEAN);
    AllocParams.BufferHandle = AMD_PFEH_HANDLE;
    AllocParams.Persist = HEAP_SYSTEM_MEM;

    if (HeapAllocateBuffer (&AllocParams, NULL) == AGESA_SUCCESS) {
      *((BOOLEAN *) AllocParams.BufferPtr) = PcdGetBool (PcdAmdCcxCfgPFEHEnable);
    }

    AGESA_TESTPOINT (TpCcxIdsAfterApLaunch, NULL);
    IDS_HOOK (IDS_HOOK_CCX_AFTER_AP_LAUNCH, NULL, NULL);
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3RmbDxeInit End\n");

  AGESA_TESTPOINT (TpCcxDxeExit, NULL);

  return (Status);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3InitWithMpServices
 *
 *  @param[in] Event        The event that invoked this routine
 *  @param[in] Context      Unused
 *
 *  Description:
 *    This routine runs necessary routines across all APs
 *
 */
VOID
EFIAPI
CcxZen3InitWithMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                Status;

  AGESA_TESTPOINT (TpCcxDxeMpCallbackEntry, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitWithMpServices Entry\n");

  Status = CcxSmmAccess2ProtocolInstall ();
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (CPU_TRACE, "   WARNING! CcxSmmAccess2ProtocolInstall was not installed\n");
  }

  IDS_HOOK (IDS_HOOK_CCX_AFTER_PWR_MNG, NULL, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitWithMpServices Exit\n");

  AGESA_TESTPOINT (TpCcxDxeMpCallbackExit, NULL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbOcCallback
 *
 *  @param[in] Event        The event that invoked this routine
 *  @param[in] Context      Unused
 *
 *  Description:
 *    This routine runs necessary routines for OC callback
 *
 */
VOID
EFIAPI
CcxZen3RmbOcCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  EFI_STATUS                 Status;
  EFI_HANDLE                 Handle;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  if (!EFI_ERROR (Status)) {

    IDS_HOOK (IDS_HOOK_CCX_CUSTOM_PSTATES, NULL, NULL);

    // Install gAmdCcxOcCompleteProtocolGuid protocol after custom Pstate feature
    mCcxOcCompleteProtocol.Revision = 0;
    Handle = NULL;
    gBS->InstallProtocolInterface (
            &Handle,
            &gAmdCcxOcCompleteProtocolGuid,
            EFI_NATIVE_INTERFACE,
            &mCcxOcCompleteProtocol
            );
  } else {
    gBS->CreateEventEx (
                        EVT_NOTIFY_SIGNAL,
                        TPL_NOTIFY,
                        CcxZen3RmbOcCallbackOnMpServices,
                        NULL,
                        NULL,
                        &CcxZen3RmbOcCallbackOnMpServicesEvent
                        );

    gBS->RegisterProtocolNotify (
                        &gEfiMpServiceProtocolGuid,
                        CcxZen3RmbOcCallbackOnMpServicesEvent,
                        &(mRegistrationForCcxZen3OcOnMpServicesCallbackEvent)
                        );
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbOcCallbackOnMpServices
 *
 *  @param[in] Event        The event that invoked this routine
 *  @param[in] Context      Unused
 *
 *  Description:
 *    This routine runs necessary routines for OC callback
 *
 */
VOID
EFIAPI
CcxZen3RmbOcCallbackOnMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_HANDLE Handle;

  AGESA_TESTPOINT (TpCcxDxeOcCallbackEntry, NULL);
  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3RmbOcCallback Entry\n");

  IDS_HOOK (IDS_HOOK_CCX_CUSTOM_PSTATES, NULL, NULL);

  // Install gAmdCcxOcCompleteProtocolGuid protocol after custom Pstate feature
  mCcxOcCompleteProtocol.Revision = 0;
  Handle = NULL;
  gBS->InstallProtocolInterface (
           &Handle,
           &gAmdCcxOcCompleteProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &mCcxOcCompleteProtocol
           );

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3RmbOcCallback Exit\n");
  AGESA_TESTPOINT (TpCcxDxeOcCallbackExit, NULL);
}

/*---------------------------------------------------------------------------------------*/
/**
 * CcxReadyToBoot
 *
 * Calls CcxReadyToBoot
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval        VOID
 */
VOID
EFIAPI
CcxReadyToBoot (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                   Status;
  EFI_SMM_CONTROL2_PROTOCOL    *SmmControl;
  UINT8                        SmiDataValue;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxReadyToBoot");
  AGESA_TESTPOINT (TpCcxDxeRtbCallBackEntry, NULL);

  Status = gBS->LocateProtocol (
                  &gEfiSmmControl2ProtocolGuid,
                  NULL,
                  (VOID **)&SmmControl
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  SmiDataValue = PcdGet8 (PcdAmdCcxS3SaveSmi);

  SmmControl->Trigger (
                    SmmControl,
                    &SmiDataValue,
                    NULL,
                    0,
                    0
                    );

  gBS->CloseEvent (Event);

  IDS_HOOK (IDS_HOOK_CCX_READY_TO_BOOT, NULL, NULL);

  AGESA_TESTPOINT (TpCcxDxeRtbCallBackExit, NULL);
}

/*---------------------------------------------------------------------------------------*/
/**
 * CcxCheckDownCoreStatusCallback
 *
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval        VOID
 */
VOID
EFIAPI
CcxCheckDownCoreStatusCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  CcxCheckDownCoreStatus ();
  gBS->CloseEvent (Event);
}

/*---------------------------------------------------------------------------------------*/
/**
 * CcxCheckDownCoreStatus
 *
 *
 *    @retval        VOID
 */
VOID
CcxCheckDownCoreStatus (
  )
{
  BOOLEAN          AgesaDownCored;
  BOOLEAN          NvDownCored;
  UINTN            VariableSize;
  EFI_STATUS       Status;
  LOCATE_HEAP_PTR  LocateHeapParams;
  EDKII_VARIABLE_LOCK_PROTOCOL *VariableLock;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxCheckDownCoreStatus Entry\n");

  LocateHeapParams.BufferHandle = AMD_DOWN_CORE_STATUS_HANDLE;
  AgesaDownCored = (BOOLEAN) (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS);

  NvDownCored = FALSE;
  VariableSize = sizeof (BOOLEAN);
  Status = gRT->GetVariable (
                  L"DownCoreStatus",
                  &gAmdDownCoreStatusGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) &NvDownCored
                  );
  if ((AgesaDownCored != NvDownCored) || (Status != EFI_SUCCESS)) {
    VariableSize = sizeof (BOOLEAN);
    Status = gRT->SetVariable (
                    L"DownCoreStatus",
                    &gAmdDownCoreStatusGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    VariableSize,
                    (VOID *) &AgesaDownCored
                    );
    // Lock variable after set
    Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **) &VariableLock);
    if (!EFI_ERROR (Status)) {
      Status = VariableLock->RequestToLock (VariableLock, L"DownCoreStatus", &gAmdDownCoreStatusGuid);
    }
    if (NvDownCored && !AgesaDownCored) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  downcored -> auto, issuing cold reset\n");
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxCheckDownCoreStatus Exit\n");
}
