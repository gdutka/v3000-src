/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2003 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


@file:

  Platform.c

@brief:

  Platform Initialization Driver.

Revision History

--*/

#include <Platform.h>
#include <ChipsetCmos.h>
#include <PttPtpRegs.h>

#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PttHeciLib.h>
#include <Library/VariableLib.h>
#include <Protocol/SaPolicy.h>
#include <Protocol/Ps2Policy.h>
#include <Protocol/PlatformNvsArea.h>
#include <Protocol/SmbusHc.h>
#include <Protocol/SmmSmbus.h>
#include <Protocol/PchReset.h>
#include <PchResetPlatformSpecific.h>
#include <Guid/H2OCp.h>
#include <Library/H2OCpLib.h>
#include <Guid/TcmInstance.h>
//
// This need to remove when kernel define it.
//
#include <Library/DxeOemSvcChipsetLib.h>
//#include <Library/PchPlatformLib.h>
  #include <Library/PchInfoLib.h>

#include <Protocol/HeciProtocol.h>
#include <Library/DxeMeLib.h>
#include <Library/ConfigBlockLib.h>
#include <ConfigBlock/MePeiConfig.h>
//
// For the function call, InstallSmbiosFviOemType
//
#include <Library/DxeSmbiosFirmwareVersionInfoLib.h>
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *mPciRootBridgeIo;

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PS2_POLICY_PROTOCOL         mPs2PolicyData = {
  EFI_KEYBOARD_NUMLOCK,
  (EFI_PS2_INIT_HARDWARE) Ps2InitHardware
};

PLATFORM_NVS_AREA_PROTOCOL        mPlatformNvsAreaProtocol = {0};

/**
  Check if PTT is enabled.

  @retval TRUE  PTT enabled
  @retval FALSE PTT not enabled
**/
BOOLEAN
IsPttEnabled (
  VOID
  )
{
  UINT32                         TpmStsFtif;

  TpmStsFtif = MmioRead32 (R_PTT_TXT_STS_FTIF);
  if ((TpmStsFtif & V_FTIF_FTPM_PRESENT) == V_FTIF_FTPM_PRESENT) {
    DEBUG ((DEBUG_INFO, "IsPttEnabled - PTT is enabled\n"));
    return TRUE;
  }
  return FALSE;
}

/**
  SetupUtility Protocol Installation Event notification handler.
  Disable Setup->PTTEnable.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
SetupUtilityProtocolInstallationEvent (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                        Status;
  VOID                              *Interface;
  CHIPSET_CONFIGURATION             *SetupVariable;

  Status = gBS->LocateProtocol (&gEfiSetupUtilityProtocolGuid, NULL, (VOID **)&Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  gBS->CloseEvent (Event);

  SetupVariable = NULL;
  SetupVariable = CommonGetVariableData (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid);
  ASSERT (SetupVariable != NULL);

  if (SetupVariable != NULL && SetupVariable->PTTEnable != 0) {
    //
    // Disable PTT by design.
    //
    SetupVariable->PTTEnable = 0;
    Status = SetVariableToSensitiveVariable (
               SETUP_VARIABLE_NAME,
               &gSystemConfigurationGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               PcdGet32 (PcdSetupConfigSize),
               SetupVariable
               );
    DEBUG ((EFI_D_INFO, "EstablishDtpm: Disable PTT by design. Set Setup Variable (SetupVariable->PTTEnable = 0) -%r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  gBS->FreePool (SetupVariable);
}

/**
  Disable PTT in order to detect DTPM.

  @param[in,out] SetupVariable   Pointer to Setup variable.

  @retval EFI_SUCCESS            Operation completed successfully
  @retval Others                 Operation failed
**/
STATIC
EFI_STATUS
EstablishDtpm (
  IN OUT CHIPSET_CONFIGURATION      *SetupVariable
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           PttState;
  PCH_RESET_DATA                    ResetData;
  BOOLEAN                           PttCapability;
  VOID                              *Registration;

  Status = PttHeciGetCapability (&PttCapability);
  DEBUG ((EFI_D_INFO, "EstablishDtpm: PttHeciGetCapability - %r (PttCapability = %x)\n",Status, PttCapability));
  ASSERT_EFI_ERROR (Status);

  Status = PttHeciGetState (&PttState);
  DEBUG ((EFI_D_INFO, "EstablishDtpm: PttHeciGetState - %r (PttState = %x)\n",Status, PttState));
  ASSERT_EFI_ERROR (Status);
  if (PttCapability && PttState) {
    //
    // Disable PTT in order to detect DTPM existence
    //
    Status = PttHeciSetState (FALSE);
    DEBUG ((EFI_D_INFO, "EstablishDtpm: Disable PTT in order to detect DTPM existence\n"));
    DEBUG ((EFI_D_INFO, "EstablishDtpm: SetPttState (FALSE) - %r\n", Status));
    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "EstablishDtpm: Need to restart system, since we just changed PTT stater\n"));
      //
      // Need reset system, since we just changed PTT state
      //
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
      gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
      CpuDeadLoop ();
    }
  }

  if (CompareGuid (PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm12Guid) ||
      CompareGuid (PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm20DtpmGuid) ||
      CompareGuid (PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTcmGuid)) {
    DEBUG ((EFI_D_INFO, "EstablishDtpm : DTPM presented on first boot.\n"));
    //
    // Disable PTTEnable once SetupUtilityProtocol is ready
    //
    EfiCreateProtocolNotifyEvent (
      &gEfiSetupUtilityProtocolGuid,
      TPL_CALLBACK,
      SetupUtilityProtocolInstallationEvent,
      NULL,
      (VOID **)&Registration
      );
    //
    // Disable PTTEnable for following flow
    //
    SetupVariable->PTTEnable = 0;
  }

  return EFI_SUCCESS;
}

/**
 After recovery complete, change reset system type to specific to do PCH global reset.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
CpRecoveryCompleteCallback (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  EFI_STATUS                           Status;
  H2O_BDS_CP_RECOVERY_COMPLETE_DATA    *BdsRecoveryCompleteData;
  STATIC PCH_RESET_DATA                ResetData;

  Status = H2OCpLookup (Handle, (VOID **) &BdsRecoveryCompleteData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG_CP ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG_CP ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }
  if (BdsRecoveryCompleteData->Size < OFFSET_OF(H2O_BDS_CP_RECOVERY_COMPLETE_DATA, ResetDataSize)) {
    DEBUG_CP ((DEBUG_WARN, "Checkpoint Data Does Not Contain Member (%a)\n", "ResetDataSize"));
    DEBUG_CP ((DEBUG_WARN, "   %a\n", __FUNCTION__));
    return;
  }

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

  BdsRecoveryCompleteData->RequestAction = H2O_BDS_CP_RECOVERY_COMPLETE_PLATFORM_SPECIFIC_RESET;
  BdsRecoveryCompleteData->ResetData     = (UINT8 *) &ResetData;
  BdsRecoveryCompleteData->ResetDataSize = sizeof (ResetData);
  BdsRecoveryCompleteData->Status        = H2O_CP_TASK_UPDATE;
}

/**

  This is the standard EFI driver point for the Platform Driver. This
  driver is responsible for setting up any platform specific policy or
  initialization information.

  @param  ImageHandle      Handle for the image of this driver
  @param  SystemTable      Pointer to the EFI System Table

  @retval EFI_SUCCESS      Policy decisions set

**/
EFI_STATUS
EFIAPI
DxePlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        Handle;
  CHIPSET_CONFIGURATION             *SetupVariable;
  BOOLEAN                           Flag;
  PLATFORM_NVS_AREA                 *PlatformNvsAreaPtr;
  OEM_PLATFORM_NVS_AREA             *OemPlatformNvsAreaPtr;
  EFI_BOOT_MODE                     BootMode;
  EFI_EVENT                         ReadyToBootEvent;
  UINTN                             DataSize;
  BOOLEAN                           PttState;
  PCH_RESET_DATA                    ResetData;
  ME_SETUP_STORAGE                  MeSetupStorage;
  UINT32                            VariableAttr;
  BOOLEAN                           PttTargetState;
  BOOLEAN                           PttCapability;
  ME_SETUP                          MeSetup;
  HECI_PROTOCOL                     *Heci;
  UINT32                            MeMode;
  UINT32                            UnconfigOnRtcClear;
  UINT8                             FirstBootAfterFlash;
  VOID                              *Registration;

  VariableAttr   = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
  //
  // Callback for SMBIOS type 0xDD (221)
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiSmbiosProtocolGuid,
    TPL_CALLBACK,
    InstallSmbiosFviOemTypeCallback,
    NULL,
    &Registration
    );

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, (VOID **) &mPciRootBridgeIo);
  ASSERT_EFI_ERROR (Status);

  SetupVariable = NULL;
  SetupVariable = CommonGetVariableData (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid);
  ASSERT (SetupVariable != NULL);

  BootMode = GetBootModeHob();
  DataSize = sizeof (ME_SETUP_STORAGE);
  ZeroMem (&MeSetupStorage, sizeof (ME_SETUP_STORAGE));
  Status = gRT->GetVariable (L"MeSetupStorage", &gMeSetupVariableGuid, NULL, &DataSize, &MeSetupStorage);
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof (ME_SETUP);
  ZeroMem (&MeSetup, sizeof (ME_SETUP));
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &MeSetup
                  );
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof(UINT8);
  Status = gRT->GetVariable (
                  L"FirstBootAfterFlash",
                  &gEfiGenericVariableGuid,
                  NULL,
                  &DataSize,
                  &FirstBootAfterFlash);

  if (EFI_ERROR (Status)) {
    //
    // Prefer using DTPM by design. Process if required.
    //
    if (FeaturePcdGet (PcdH2OPreferDtpmBootSupported)) {
      Status = EstablishDtpm (SetupVariable);
      DEBUG ((DEBUG_INFO, "DxePlatformEntryPoint: EstablishDtpm - %r\n", Status));
    }

    DataSize = sizeof(UINT8);
    FirstBootAfterFlash = 1;
    Status = gRT->SetVariable (
                    L"FirstBootAfterFlash",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof(UINT8),
                    &FirstBootAfterFlash
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
    if (!EFI_ERROR (Status)) {
      Status = Heci->GetMeMode (&MeMode);
      ASSERT_EFI_ERROR(Status);
      if (MeMode == ME_MODE_NORMAL) {
        MeSetupStorage.MeStateControl = 1;
        Status = HeciGetUnconfigOnRtcClearDisableMsg(&UnconfigOnRtcClear);
        if (!EFI_ERROR (Status)) {
          if ( UnconfigOnRtcClear != MeSetup.MeUnconfigOnRtcClear) {
            Status = HeciSetUnconfigOnRtcClearDisableMsg ((UINT32)MeSetup.MeUnconfigOnRtcClear);
            DEBUG ((DEBUG_INFO, "Sync MeUnconfigOnRtcClear of MeSetupStorage to ME: %r\n", Status));
            ASSERT_EFI_ERROR(Status);
          }
        }
      }
    }
  }

  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    if ((BOOLEAN)MeSetupStorage.PttState != IsPttEnabled ()) {
      //
      // MeSetupStorage.PttState mismatch the current PTT policy
      // Get the exact PTT state from ME
      //
      Status = PttHeciGetState (&PttState);
      DEBUG ((EFI_D_INFO, "DxePlatformEntryPoint: PttHeciGetState (%x): %r\n", PttState, Status));
      ASSERT_EFI_ERROR (Status);
      MeSetupStorage.PttState = PttState;
      Status = gRT->SetVariable (
                      L"MeSetupStorage",
                      &gMeSetupVariableGuid,
                      VariableAttr,
                      sizeof (ME_SETUP_STORAGE),
                      (VOID *)&MeSetupStorage
                      );
      ASSERT_EFI_ERROR (Status);
    }
    //
    // Handle PTT state according to Setup
    //
    //
    // Always disable PTT feature when "TPM Hidden" was choosen
    //
    PttTargetState = (BOOLEAN)SetupVariable->PTTEnable && !PcdGetBool (PcdTpmHide);
    if ((BOOLEAN)MeSetupStorage.PttState != PttTargetState) {
      //
      // Check whether PTT is supported before changing PTT state
      //
      Status = PttHeciGetCapability (&PttCapability);
      DEBUG ((EFI_D_INFO, "DxePlatformEntryPoint: PttHeciGetCapability - %r (PttCapability = %x)\n", Status, PttCapability));
      ASSERT_EFI_ERROR (Status);

      if (PttCapability) {
        Status = PttHeciSetState (PttTargetState);
        DEBUG ((EFI_D_INFO, "DxePlatformEntryPoint: PttHeciSetState (%x): %r\n", PttTargetState, Status));
        ASSERT_EFI_ERROR (Status);
        if (!EFI_ERROR (Status)) {
          CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
          StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
          gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
        }
      }
    }
  }

  if (FeaturePcdGet (PcdH2OBdsCpRecoveryCompleteSupported)) {
    H2O_CP_HANDLE                       CpHandle;

    Status = H2OCpRegisterHandler (
               &gH2OBdsCpRecoveryCompleteGuid,
               CpRecoveryCompleteCallback,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpRecoveryCompleteGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpRecoveryCompleteGuid, Status));
  }

  //
  // Initialize Setup Configuration for Overclocking
  //
//   Status = gRT->SetVariable (
//                   L"Setup",
//                   &GuidId,
//                   EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
//                   PcdGet32 (PcdSetupConfigSize),
//                   (VOID *) SetupVariable
//                   );
  Flag = TRUE;
  DEBUG_OEM_SVC ((DEBUG_INFO, "Dxe OemChipsetServices Call: OemSvcHookPlatformDxe (Flag = TRUE)\n"));
  Status = OemSvcHookPlatformDxe (SetupVariable, mPciRootBridgeIo, Flag);
  DEBUG_OEM_SVC ((DEBUG_INFO, "Dxe OemChipsetServices OemSvcHookPlatformDxe  Status: %r\n", Status));
  if (Status != EFI_SUCCESS ) {
    if (SetupVariable->NumLock == 0) {
      mPs2PolicyData.KeyboardLight &= (~EFI_KEYBOARD_NUMLOCK);
    }
    //
    //
    // Install protocol to to allow access to this Policy.
    //
    Handle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gEfiPs2PolicyProtocolGuid,
                    &mPs2PolicyData,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);
    //
    // Install Legacy USB setup policy protocol depending upon
    // whether or not Legacy USB setup options are enabled or not.
    //
    UsbLegacyPlatformInstall();
    //
    // TBD or do in other places.
    // 1. Clear BIS Credentials.
    // 2. Chassis Intrusion, Date/Time, EventLog, Pnp O/S,
    // 3. Boot (BBS, USB, PXE, VGA)
    //
    Flag = FALSE;
    DEBUG_OEM_SVC ((DEBUG_INFO, "Dxe OemChipsetServices Call: OemSvcHookPlatformDxe (Flag = FALSE)\n"));
    Status = OemSvcHookPlatformDxe (SetupVariable, mPciRootBridgeIo, Flag);
    DEBUG_OEM_SVC ((DEBUG_INFO, "Dxe OemChipsetServices OemSvcHookPlatformDxe Status: %r\n", Status));
   }
  DataSize = sizeof (PLATFORM_NVS_AREA);
  Status = gBS->AllocatePool(
                  EfiACPIMemoryNVS,
                  DataSize,
                  (VOID **)&PlatformNvsAreaPtr
                  );
  if (EFI_ERROR(Status)) {
    gBS->FreePool (SetupVariable);
    return Status;
  }
  ZeroMem (PlatformNvsAreaPtr, DataSize);
  mPlatformNvsAreaProtocol.Area = PlatformNvsAreaPtr;

  DataSize = sizeof (OEM_PLATFORM_NVS_AREA);
  Status = gBS->AllocatePool(
                  EfiACPIMemoryNVS,
                  DataSize,
                  (VOID **)&OemPlatformNvsAreaPtr
                  );
  if (EFI_ERROR(Status)) {
    gBS->FreePool (SetupVariable);
    return Status;
  }
  ZeroMem (OemPlatformNvsAreaPtr, DataSize);
  mPlatformNvsAreaProtocol.OemArea = OemPlatformNvsAreaPtr;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
            &Handle,
            &gPlatformNvsAreaProtocolGuid,
            &mPlatformNvsAreaProtocol,
            NULL
            );
  if (FeaturePcdGet(PcdMemSpdProtectionSupported)){
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               ProtectMemorySPD,
               NULL,
               &ReadyToBootEvent
               );
    ASSERT_EFI_ERROR ( Status );
  }
  gBS->FreePool (SetupVariable);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ProtectMemorySPD (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                       Status;
  EFI_STATUS                       Status1;
  EFI_STATUS                       Status2;
  EFI_SMBUS_HC_PROTOCOL            *SmbusController;
  SA_POLICY_PROTOCOL               *DxePlatformSaPolicy;
  UINT8                            ChannelASlotMap;
  UINT8                            ChannelBSlotMap;
  BOOLEAN                          SlotPresent;
  UINT8                            Dimm;
  EFI_SMBUS_DEVICE_ADDRESS         SmbusSlaveAddress;
  UINTN                            SmbusOffset;
  UINTN                            SmbusLength;
  UINT8                            Data8;
  MEMORY_DXE_CONFIG                *MemoryDxeConfig= NULL;
  DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "Protecting Memory SPD Process - Start.\n" ) );
  SmbusController       = NULL;
  DxePlatformSaPolicy   = NULL;
  ChannelASlotMap       = 0;
  ChannelBSlotMap       = 0;
  SlotPresent           = FALSE;
  Dimm                  = 0;
  SmbusOffset           = 0;
  SmbusLength           = 0;
  Data8                 = 0;
  ChannelASlotMap       = 0x03;
  ChannelBSlotMap       = 0x03;
  Status = gBS->LocateProtocol ( &gEfiSmmSmbusProtocolGuid, NULL, (VOID **)&SmbusController );
  ASSERT_EFI_ERROR ( Status );
  Status1 = gBS->LocateProtocol ( &gSaPolicyProtocolGuid, NULL, (VOID **)&DxePlatformSaPolicy );
  ASSERT_EFI_ERROR ( Status1 );
  Status2 = GetConfigBlock ((VOID *)DxePlatformSaPolicy, &gMemoryDxeConfigGuid, (VOID *)&MemoryDxeConfig);
  ASSERT_EFI_ERROR ( Status2 );

  if ( !(EFI_ERROR ( Status) || EFI_ERROR ( Status1)|| EFI_ERROR ( Status2))  ) {
    ChannelASlotMap = MemoryDxeConfig->ChannelASlotMap;
    ChannelBSlotMap = MemoryDxeConfig->ChannelBSlotMap;
    for ( Dimm = 0 ; Dimm < SA_MC_MAX_SOCKETS ; Dimm = Dimm + 1 ) {
      //
      // Use channel slot map to check whether the Socket is supported in this SKU, some SKU only has 2 Sockets totally
      //
      SlotPresent = FALSE;
      if  ( Dimm < 2 ) {
        if ( ChannelASlotMap & ( 1 << Dimm ) ) {
          SlotPresent = TRUE;
        }
      } else {
        if ( ChannelBSlotMap & ( 1 << ( Dimm - 2 ) ) ) {
          SlotPresent = TRUE;
        }
      }
      if ( !SlotPresent ) {
        DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "  DIMM %x is not in channel slot map.\n", Dimm ) );
        continue;
      }
      //
      // Assign default address, then be overrided by platform policy
      //
      SmbusSlaveAddress.SmbusDeviceAddress = ( SA_MC_DIMM0_SPD_ADDRESS >> 1 ) + Dimm;
      SmbusSlaveAddress.SmbusDeviceAddress = ( DIMM0_SPD_ADDRESS >> 1 ) + Dimm;
      if ( DxePlatformSaPolicy != NULL ) {
        SmbusSlaveAddress.SmbusDeviceAddress = ( MemoryDxeConfig->SpdAddressTable[Dimm] ) >> 1;
      }
      //
      // BUGBUG :
      //
      //   This step is designed by HuronRiver CRB.
      //
      //   The device can only be read with a non-protected Memory SPD.
      //
      //   Take care if your platform has different behavior.
      //
      SmbusSlaveAddress.SmbusDeviceAddress = SmbusSlaveAddress.SmbusDeviceAddress & ( ~BIT6 );
      SmbusSlaveAddress.SmbusDeviceAddress = SmbusSlaveAddress.SmbusDeviceAddress | ( BIT5 );
      SmbusOffset                          = 0x00;
      SmbusLength                          = 0x01;
      Data8                                = 0x00;
      Status = SmbusController->Execute (
                 SmbusController,
                 SmbusSlaveAddress,
                 SmbusOffset,
                 EfiSmbusReadByte,
                 FALSE,
                 &SmbusLength,
                 &Data8
                 );
      if ( !EFI_ERROR ( Status ) ) {
        Status = SmbusController->Execute (
                   SmbusController,
                   SmbusSlaveAddress,
                   SmbusOffset,
                   EfiSmbusWriteByte,
                   FALSE,
                   &SmbusLength,
                   &Data8
                   );
        ASSERT_EFI_ERROR ( Status );
        if ( EFI_ERROR ( Status ) ) {
          DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "  Protect command failure on DIMM %x.\n", Dimm ) );
        }
      } else {
        DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "  No memory plugged in DIMM %x, or it has been protected before.\n", Dimm ) );
      }
    }
  } else if(EFI_ERROR ( Status))  {
    DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "  Locate SMBUS Protocol Failure!!\n" ) );
  } else if(EFI_ERROR ( Status1))  {
    DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "  Locate Sa Policy Protocol Failure!!\n" ) );
  } else if(EFI_ERROR ( Status2)){
    DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "  Locate Memory Dxe Config Failure!!\n" ) );
  }
  DEBUG ( ( EFI_D_INFO | EFI_D_ERROR, "Protecting Memory SPD Process - Done.\n" ) );
  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to perform InstallSmbiosFviOemType
  after SmbiosProtocol was installed.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
InstallSmbiosFviOemTypeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "InstallSmbiosFviOemTypeCallback Start\n"));
  //
  // For SMBIOS type 0xDD (221)
  //
  Status = InstallSmbiosFviOemType ();// todo: remove it once Bios Guard convert the HOB generation.
  if (!EFI_ERROR (Status)) {
    gBS->CloseEvent (Event);
  }

  return;
}
