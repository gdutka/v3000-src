/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *
 ***************************************************************************/

#include <Uefi.h>
#include <Guid/GlobalVariable.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmdPspKvmServiceProtocol.h>
#include <Protocol/ResetNotification.h>
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/AmdAfterConsoleEvent.h>
#include <Library/MpmMboxLib.h>
#include <Library/PcdLib.h>
#include <MpmPldmSmbios.h>

#define AMD_RMB_PSP_VID       0x1022
#define AMD_RMB_PSP_DID       0x1649

EFI_STATUS
EFIAPI
InitializeKvmCallback (
  VOID
  );

AMD_AFTER_CONSOLE_EVENT_PROTOCOL mDxeAmdAfterConsoleEventProtocol = {
  InitializeKvmCallback,
  0x80
};

/**
 This routine is called to properly shutdown KVM.

  @param[in]  ResetType         The type of reset to perform.
  @param[in]  ResetStatus       The status code for the reset.
  @param[in]  DataSize          The size, in bytes, of ResetData.
  @param[in]  ResetData         For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                string, optionally followed by additional binary data.
                                The string is a description that the caller may use to further
                                indicate the reason for the system reset.
                                For a ResetType of EfiResetPlatformSpecific the data buffer
                                also starts with a Null-terminated string that is followed
                                by an EFI_GUID that describes the specific type of reset to perform.
**/
VOID
EFIAPI
PspKvmTerminateCallBack (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  EFI_STATUS                   Status;
  AMD_PSP_KVM_SERVICE_PROTOCOL *mAmdPspKvmService;

  Status = gBS->LocateProtocol (&gAmdPspKvmServiceProtocolGuid, NULL, (VOID **)&mAmdPspKvmService);
  if (Status != EFI_SUCCESS) {
    return;
  }

  Status = mAmdPspKvmService->TerminateKvm ();
  DEBUG((DEBUG_INFO, "TerminateKvm Status = %r\n", Status));

  return;
}

/**
 This routine is called to properly shutdown TCR.

  @param[in]  ResetType         The type of reset to perform.
  @param[in]  ResetStatus       The status code for the reset.
  @param[in]  DataSize          The size, in bytes, of ResetData.
  @param[in]  ResetData         For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                string, optionally followed by additional binary data.
                                The string is a description that the caller may use to further
                                indicate the reason for the system reset.
                                For a ResetType of EfiResetPlatformSpecific the data buffer
                                also starts with a Null-terminated string that is followed
                                by an EFI_GUID that describes the specific type of reset to perform.
**/
VOID
EFIAPI
PspTcrTerminateCallBack (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  DEBUG((DEBUG_INFO, "BiosMpmCmdTerminateTcr\n"));
  BiosMpmCmdTerminateTcr();
  return;
}

/**

  Connect PSP controller for install Kvm keyboard & mouse protocol.

**/
EFI_STATUS
ConnectPspKvmKbMs (
  VOID
  )
{
  EFI_STATUS                         Status;
  UINTN                              NumberOfPciIoHandles;
  UINTN                              Index;
  EFI_HANDLE                         *PciIoHandles;
  EFI_PCI_IO_PROTOCOL                *PciIo;
  PCI_DEVICE_INDEPENDENT_REGION      DeviceHdr;

  DEBUG((DEBUG_INFO, "ConnectPspKvmKbMs Entry...\n"));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NumberOfPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "  Locate PciIo handle buffer failed: %r.\n", Status));
    return Status;
  }

  for (Index = 0; Index < NumberOfPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
                    PciIoHandles[Index],
                    &gEfiPciIoProtocolGuid,
                    &PciIo
                    );
    if (!EFI_ERROR(Status)) {
      Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof (DeviceHdr),
                        &DeviceHdr
                        );
      if (!EFI_ERROR(Status)) {
        if ((AMD_RMB_PSP_VID == DeviceHdr.VendorId) && (AMD_RMB_PSP_DID == DeviceHdr.DeviceId)) {
          DEBUG((DEBUG_INFO, "    Found PSP Controller, Connecting...\n"));
          return gBS->ConnectController (PciIoHandles[Index], NULL, NULL, TRUE);
        }
      }
    }
  }
  DEBUG((DEBUG_INFO, "ConnectPspKvmKbMs Exit...\n"));
  return EFI_SUCCESS;
}

/**
  Call Back function for gAmdMpmKvmInitializedProtocolGuid and gAmdMpmTcrInitializedProtocolGuid.
  This function will prompt the POST F1/F2 Screen

  @param Event  Event whose notification function is being invoked.
  @param Context  Pointer to the notification function's context.

**/
VOID
EFIAPI
MpmKvmServiceCallBack (
  IN     EFI_EVENT                      Event,
  IN     VOID                           *Context
  )
{
  EFI_STATUS                       Status;
  VOID                             *Interface;
  UINT64                           OsIndication;
  UINTN                            DataSize;
  EFI_INPUT_KEY                    Key;
  EFI_RESET_NOTIFICATION_PROTOCOL  *ResetNotify;

  //always take KVM as priority if both KVM and TCR are requested
  Status = gBS->LocateProtocol (&gAmdMpmKvmInitializedProtocolGuid, NULL, &Interface);
  DEBUG ((DEBUG_INFO, "Locate gAmdMpmKvmInitializedProtocolGuid Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    ConnectPspKvmKbMs ();

    do {
      CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE, &Key, L"Press F1 : Continue   ", L"Press F2 : Enter Setup", NULL);
    } while ((Key.ScanCode != SCAN_F1) && (Key.ScanCode != SCAN_F2));

    if (Key.ScanCode == SCAN_F2) {
      // Set "OsIndications" Variable to enter Front Page,
      // This variable is the standard variable that oem can use
      DataSize = sizeof (UINT64);
      Status = gRT->GetVariable (
                      EFI_OS_INDICATIONS_VARIABLE_NAME,
                      &gEfiGlobalVariableGuid,
                      NULL,
                      &DataSize,
                      &OsIndication
                      );
      if (EFI_ERROR (Status)) {
        OsIndication = 0;
      }
      OsIndication |= EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
      Status = gRT->SetVariable (
                      EFI_OS_INDICATIONS_VARIABLE_NAME,
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      sizeof(UINT64),
                      &OsIndication
                      );
    }

    Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
    ASSERT_EFI_ERROR (Status);
    Status = ResetNotify->RegisterResetNotify (ResetNotify, PspKvmTerminateCallBack);
  } else {
    Status = gBS->LocateProtocol (&gAmdMpmTcrInitializedProtocolGuid, NULL, &Interface);
     DEBUG ((DEBUG_INFO, "Locate gAmdMpmKvmInitializedProtocolGuid Status = %r\n", Status));
    if (!EFI_ERROR (Status)) {
      // Set "OsIndications" Variable to enter Front Page,
      // This variable is the standard variable that oem can use
      DataSize = sizeof (UINT64);
      Status = gRT->GetVariable (
                      EFI_OS_INDICATIONS_VARIABLE_NAME,
                      &gEfiGlobalVariableGuid,
                      NULL,
                      &DataSize,
                      &OsIndication
                      );
      if (EFI_ERROR (Status)) {
        OsIndication = 0;
      }
      OsIndication |= EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
      Status = gRT->SetVariable (
                      EFI_OS_INDICATIONS_VARIABLE_NAME,
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      sizeof(UINT64),
                      &OsIndication
                      );
    }
    Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
    ASSERT_EFI_ERROR (Status);
    Status = ResetNotify->RegisterResetNotify (ResetNotify, PspTcrTerminateCallBack);
  }
  return;
}

/**
   Callback function to initialize kvm

**/
EFI_STATUS
EFIAPI
InitializeKvmCallback (
  )
{
  VOID                             *KvmRegistration;
  VOID                             *TcrRegistration;
  DEBUG ((DEBUG_INFO, "InitializeKvmCallback enter\n"));

  EfiCreateProtocolNotifyEvent (
    &gAmdMpmKvmInitializedProtocolGuid,
    TPL_CALLBACK,
    MpmKvmServiceCallBack,
    NULL,
    &KvmRegistration
  );

  EfiCreateProtocolNotifyEvent (
    &gAmdMpmTcrInitializedProtocolGuid,
    TPL_CALLBACK,
    MpmKvmServiceCallBack,
    NULL,
    &TcrRegistration
  );
  DEBUG ((DEBUG_INFO, "InitializeKvmCallback exit\n"));
  return EFI_SUCCESS;
}

/**
  Update MPM related SMBIOS table (A1/A2).
  SMBIOS table A1/A2 are MPM specific table, it will shows the MPM BIOS options status
  User infomations and AIM-T Configuration data.
  Most of the data is filled by MPM FW, BIOS will update the portion BiosOption and AimtOemStatus.
**/
VOID
EFIAPI
UpdateMpmSmbiosEntry (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                        Status;
  SMBIOS_TABLE_TYPEA1               *TypeA1Record;
  EFI_SMBIOS_PROTOCOL               *Smbios;
  EFI_SMBIOS_HANDLE                 SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER           *Record;

  DEBUG ((DEBUG_INFO, "UpdateMpmSmbiosEntry enter\n"));
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    return;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  while (TRUE) {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (Record->Type == 0xA1) {
      TypeA1Record = (SMBIOS_TABLE_TYPEA1 *) Record;
      if ((PcdGetBool (PcdMpmWirelessManageability) == TRUE) && (PcdGetBool (PcdMpmEnable) == TRUE)) {
        TypeA1Record->BiosOption.Field.AimtOption = 0x1;
        TypeA1Record->BiosOption.Field.WirelessManageabilityOption = 1;
         TypeA1Record->AimtOemStatus.Field.AimtEnableInBios = 1;
         TypeA1Record->AimtOemStatus.Field.WirelessManageabilityStatus = 1;
        if (PcdGetBool (PcdMpmWirelessKvmEnable) == TRUE) {
          TypeA1Record->BiosOption.Field.WirelessManageabilityOption = 3;
          TypeA1Record->AimtOemStatus.Field.WirelessManageabilityStatus = 3;
        }
        if (PcdGetBool (PcdMpmWiredKvmEnable) == TRUE) {
          TypeA1Record->BiosOption.Field.WiredManageabilityOption = 2;
          TypeA1Record->AimtOemStatus.Field.WiredManageabilityStatus = 2;
        }
      } else {
        TypeA1Record->BiosOption.Field.AimtOption = 0;
        TypeA1Record->BiosOption.Field.WirelessManageabilityOption = 0;
        TypeA1Record->AimtOemStatus.Field.AimtEnableInBios = 0;
        TypeA1Record->AimtOemStatus.Field.WirelessManageabilityStatus = 0;
      }
      DEBUG ((DEBUG_INFO, "UpdateMpmSmbios A1 table successfully\n"));
      break;
    }
  }
  DEBUG ((DEBUG_INFO, "UpdateMpmSmbiosEntry exit\n"));
}

/**
  Entry point of KvmPlatformDxe driver.

  @param ImageHandle  Image handle of this driver.
  @param SystemTable  Pointer to standard EFI system table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
KvmPlatformDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_EVENT  MpmReadyToBootEvent;

  if ((PcdGetBool (PcdMpmWirelessManageability) == TRUE) && (PcdGetBool (PcdMpmEnable) == TRUE)) {
    Status = EfiCreateEventReadyToBootEx (
               TPL_NOTIFY,
               UpdateMpmSmbiosEntry,
               NULL,
               &MpmReadyToBootEvent
               );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Create EfiCreateEventReadyToBootEx event UpdateMpmSmbiosEntry failed.\n"));
    }
  }

  //Boot to F1/F2 post screen when there is KVM or TCR requested.
  //User can press F1 to continue boot, press F2 to boot BIOS setup
  //Register gAmdMpmKvmInitializedProtocolGuid and gAmdMpmTcrInitializedProtocolGuid callback function after gAmdAfterConsoleEventProtocolGuid
  //Please note gAmdAfterConsoleEventProtocolGuid is used for AMD EDK2 source code, OEM can register the call back function after their console driver
  //gAmdMpmKvmInitializedProtocolGuid: Agesa will install this protocol when there is KVM requested from MPM
  //gAmdMpmTcrInitializedProtocolGuid: Agesa will install this protocol when there is TCR requested from MPM
  if ((PcdGetBool (PcdMpmWiredKvmEnable) == FALSE) &&\
   (PcdGetBool (PcdMpmWirelessKvmEnable) == FALSE) &&\
   (PcdGetBool(PcdMpmWirelessTcrEnable) == FALSE)) {
    return EFI_UNSUPPORTED;
  }
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gAmdAfterConsoleEventProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mDxeAmdAfterConsoleEventProtocol
                  );

  return Status;
}

