/** @file

;*******************************************************************************
;* Copyright (c) 2013 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include "Platform.h"
#include "UsbLegacy.h"
#include <Library/DxeServicesTableLib.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Library/MemRestore.h>
#include <Guid/AmdCbsConfig.h>
#include "AmdCbsVariable.h"

extern EFI_GUID gEfiNotLegacyBootProtocolGuid;
extern EFI_GUID gPlatformNvmeInstalledProtocolGuid;

EFI_PS2_POLICY_PROTOCOL  mPs2PolicyData = {
  EFI_KEYBOARD_NUMLOCK,
  (EFI_PS2_INIT_HARDWARE) Ps2SkipHWInit
};


/**

  Initial some platform specific register.

  @param  Event
  @param  *Context

**/
VOID
EFIAPI
PlatformSpecialRegInit (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                    Status;
  UINT64                        TopMem;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsAreaProtocol;
  EFI_GLOBAL_NVS_AREA           *GlobalNvsAreaPtr;


  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, (VOID**)&GlobalNvsAreaProtocol);
  if (!EFI_ERROR (Status)) {
    GlobalNvsAreaPtr = GlobalNvsAreaProtocol->Area;

    TopMem = AsmReadMsr64 (TOP_MEM);
    GlobalNvsAreaPtr->TopOfMemLo = (UINT8)(TopMem >> 16);
    GlobalNvsAreaPtr->TopOfMemHi = (UINT8)(TopMem >> 24);
    GlobalNvsAreaPtr->RomSize = PcdGet32 (PcdFlashAreaSize);
  }

  return;
}

/**

  For OEM to implement any required initialize just after PCI resource assignment.

  @param  Event
  @param  *Context

**/
VOID
EFIAPI
PciEnumCompleteCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS              Status;
  VOID                    *Interface;

  //
  // Insure the protocol really installed
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  (VOID **)&Interface
                  );
  if (Status != EFI_SUCCESS) {
    return ;
  }

  //
  // Close event for one time shot
  //
  gBS->CloseEvent (Event);

  Status = OemSvcDxePlatformAfterAllPciInitFinished ();

  return ;
}

EFI_STATUS
InstallGlobalNvsAreaProtocol (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  UINTN                         BufferSize;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsAreadProtocol;
  EFI_GLOBAL_NVS_AREA           *GlobalNvsAreaPtr;
  EFI_OEM_GLOBAL_NVS_AREA       *OemGlobalNvsAreaPtr;

  BufferSize = sizeof (EFI_GLOBAL_NVS_AREA_PROTOCOL);
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  BufferSize,
                  (VOID**)&GlobalNvsAreadProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  ZeroMem (GlobalNvsAreadProtocol, sizeof (EFI_GLOBAL_NVS_AREA_PROTOCOL));

  BufferSize = sizeof (EFI_GLOBAL_NVS_AREA);
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  BufferSize,
                  (VOID**)&GlobalNvsAreaPtr
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  ZeroMem (GlobalNvsAreaPtr, BufferSize);
  GlobalNvsAreadProtocol->Area = GlobalNvsAreaPtr;

  BufferSize = sizeof (EFI_OEM_GLOBAL_NVS_AREA);
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  BufferSize,
                  (VOID**)&OemGlobalNvsAreaPtr
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  ZeroMem (OemGlobalNvsAreaPtr, BufferSize);
  GlobalNvsAreadProtocol->OemArea = OemGlobalNvsAreaPtr;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiGlobalNvsAreaProtocolGuid,
                  GlobalNvsAreadProtocol,
                  NULL
                  );

  return Status;
}

/**
 * Mark the boot completion by memory context restore
 *
 * This function marks the boot completion by memory context restore
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval        VOID
 *
 */
VOID
EFIAPI
MemRestoreBootCompleteCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                  Status;
  EFI_SMBIOS_TABLE_HEADER     *Record;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  EFI_SMBIOS_PROTOCOL         *Smbios;
  SMBIOS_TABLE_TYPE16         *Type16Record;
  BOOLEAN                     EccCapable;

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  //Ecc WA
  EccCapable = FALSE;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );

  if (!EFI_ERROR(Status)) {
    do {
      Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
      if (EFI_ERROR(Status)) {
        break;
      }

      if (Record->Type == EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY) {
        Type16Record = (SMBIOS_TABLE_TYPE16 *) Record;

        if (Type16Record->MemoryErrorCorrection == 0x06) {
          EccCapable = TRUE;
          break;
        }
      }
    } while(Status == EFI_SUCCESS);
//[-start-240426-IB20840054-remove]//
    // if (EccCapable == TRUE) {
    //   DEBUG ((EFI_D_INFO, "[Platform MemRestore] Feature disabled due to EccCapable\n"));
    //   AmdMemRestoreDiscardCurrentMemContext ();
    //   return;
    // }
//[-end-240426-IB20840054-remove]//
  } else {
    ASSERT (FALSE);
  }

  AmdMemRestoreSignOff ();
}

/**
  Event handler for Exit Boot Service.

  @param  Event       The event that be siganlled when exiting boot service.
  @param  Context     Pointer to instance of BIOS_VIDEO_DEV.

**/
VOID
EFIAPI
SetHostAddrForDmaBufferExitBootServices (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  EFI_STATUS    Status;
  Status = PcdSetBoolS (PcdH2OIommuMapping, FALSE);
  ASSERT_EFI_ERROR (Status);
}

EFI_STATUS
EFIAPI
EnableAmdCbsScpcSubOptions (
  IN UINT8 ScpcLevels,
  IN BOOLEAN HspCoreEnable
)
{
  EFI_STATUS Status;
  CBS_CONFIG *Buffer;
  UINTN      BufferSize;

  Buffer       = NULL;
  BufferSize   = sizeof (CBS_CONFIG);
  Buffer  = AllocateZeroPool (BufferSize);

//[-start-230817-IB20840016-add]//
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
//[-end-230817-IB20840016-add]//

  Status = gRT->GetVariable (
                  CBS_SYSTEM_CONFIGURATION_NAME,
                  &gCbsSystemConfigurationGuid,
                  NULL,
                  &BufferSize,
                  Buffer
                  );
  if (Status == EFI_SUCCESS) {
    if (ScpcLevels == 3) {
      Buffer->CbsCmnMemTsmeEnableDdr = 1; //DDR5 for Lilac
      Buffer->CbsCmnMemTsmeLpddr = 1;     //LPDDR5 for Mayan
      Buffer->CbsCmnGpuHostTranslation = 1;
      Buffer->CbsCmnDmarSupport = 1;
      Buffer->CbsCmnDmaProtection = 1;
      Buffer->CbsCmnGnbNbIOMMU = 1;
    }
    if (ScpcLevels == 2) {
      Buffer->CbsCmnDmarSupport = 1;
      Buffer->CbsCmnDmaProtection = 1;
      Buffer->CbsCmnGnbNbIOMMU = 1;
    }
    if (HspCoreEnable) {
      Buffer->CbsCmnSoCMiscX86FwSupport = 1;
    }
    Status = gRT->SetVariable (
                  CBS_SYSTEM_CONFIGURATION_NAME,
                  &gCbsSystemConfigurationGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  BufferSize,
                  Buffer
                  );
  }
  FreePool (Buffer);
  return Status;
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
DxePlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        Handle;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRootBridgeIo;
  EFI_EVENT                         LastInitEvent;
  EFI_EVENT                         PciEnumCompleteEvent;
  EFI_EVENT                         ExitBootServicesEvent;
  VOID                              *Registration;
  EFI_BOOT_MODE                     BootMode;
  BOOLEAN                           Flag;
  KERNEL_CONFIGURATION              KernelSetup;
  CHIPSET_CONFIGURATION             ChipsetSetup;
  VOID                              *NvmeProtocol;
  EFI_EVENT                         AmdSmbiosDxeInitCompleteEvent;
  BOOLEAN                           HspCoreEnable = 0;

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, (VOID**)&PciRootBridgeIo);
  ASSERT_EFI_ERROR (Status);

  Status = GetKernelConfiguration (&KernelSetup);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetChipsetConfiguration (&ChipsetSetup);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Level3 settings, enable DMAr/TSME/HSP
  if (PcdGet8 (PcdScpcLevel) > 0) {
    if ((ChipsetSetup.ScpcLevels > 3) && (ChipsetSetup.ScpcLevels < 10)) {
      HspCoreEnable = 1;
    }
    EnableAmdCbsScpcSubOptions(PcdGet8 (PcdScpcLevel), HspCoreEnable);
  }

  BootMode = GetBootModeHob();

  //
  // Provide a unified hook point for OEM after Pci enumeration
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PciEnumCompleteCallback,
                  NULL,
                  &PciEnumCompleteEvent
                  );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiPciEnumerationCompleteProtocolGuid,
                    PciEnumCompleteEvent,
                    &Registration
                    );
    ASSERT_EFI_ERROR (Status);
  }

  Flag = TRUE;
  Status = OemSvcHookPlatformDxe (&ChipsetSetup, PciRootBridgeIo, Flag);
  DEBUG_OEM_SVC ( ( EFI_D_ERROR | EFI_D_INFO, "DxeOemSvcChipsetLib OemSvcHookPlatformDxe 1, Status : %r\n", Status ) );
  
  //
  // Initialize Platform Specific registers
  //
  ProcessRegTablePci (gPlatformRegs, PciRootBridgeIo, NULL);

  if (KernelSetup.NumLock == 0) {
    mPs2PolicyData.KeyboardLight &= (~EFI_KEYBOARD_NUMLOCK);
  }

  //
  // Install protocol to to allow access to this Policy.
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiPs2PolicyProtocolGuid, &mPs2PolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  //
  //Install protocol for raid driver dispatched under DUAL/UEFI BOOT mode
  //
   if (ChipsetSetup.BootType != LEGACY_BOOT_TYPE) {
     Handle = NULL;
     Status = gBS->InstallProtocolInterface (
                     &Handle,
                     &gEfiNotLegacyBootProtocolGuid,
                     EFI_NATIVE_INTERFACE,
                     NULL
                     );
   }

  if ((PcdGet8 (PcdNvmeRaid)) != 1) {
    Status = gBS->LocateProtocol (&gPlatformNvmeInstalledProtocolGuid, NULL, &NvmeProtocol);
    if (EFI_ERROR (Status)) {
      Handle = NULL;
      Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gPlatformNvmeInstalledProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
    }
  } 

  //
  // Install Legacy USB setup policy protocol depending upon
  // whether or not Legacy USB setup options are enabled or not.
  //
  UsbLegacyPlatformInstall ();

  InstallGlobalNvsAreaProtocol ();

  Flag = FALSE;
  Status = OemSvcHookPlatformDxe (&ChipsetSetup, PciRootBridgeIo, Flag);
  DEBUG_OEM_SVC ( ( EFI_D_ERROR | EFI_D_INFO, "DxeOemSvcChipsetLib OemSvcHookPlatformDxe 1, Status : %r\n", Status ) );

  Status = EfiCreateEventReadyToBootEx (
             TPL_NOTIFY,
             PlatformSpecialRegInit,
             NULL,
             &LastInitEvent
             );
  ASSERT_EFI_ERROR (Status);

  if (PcdGetBool (PcdModernStandbyEnable)) {
    Status = gBS->CreateEventEx (
              EVT_NOTIFY_SIGNAL,
              TPL_CALLBACK,
              SetHostAddrForDmaBufferExitBootServices,
              NULL,
              &gEfiEventExitBootServicesGuid,
              &ExitBootServicesEvent
              );
    ASSERT_EFI_ERROR (Status);
  }

  if (((KernelSetup.SataCnfigure == 2) ||
       (KernelSetup.SataCnfigure == 5)) &&
    (ChipsetSetup.AHCIMODE == 1)) {
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gEfiLoadAmdAhciUefiDriverProtocolGuid,
                    NULL,
                    NULL
                    );
  }
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MemRestoreBootCompleteCallback,
                  NULL,
                  &AmdSmbiosDxeInitCompleteEvent
                  );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gAmdSmbiosDxeInitCompleteProtocolGuid,
                    AmdSmbiosDxeInitCompleteEvent,
                    &Registration
                    );
    ASSERT_EFI_ERROR (Status);
  }
  return EFI_SUCCESS;
}
