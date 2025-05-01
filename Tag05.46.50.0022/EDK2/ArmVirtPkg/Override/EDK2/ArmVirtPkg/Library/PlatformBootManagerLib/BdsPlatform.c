/** @file
 This file include all platform action which can be customized by IBV/OEM.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "PlatformBm.h"
#include <Protocol/UgaSplash.h>
#include <Library/GenericBdsLib.h>
#include <Library/OemGraphicsLib.h>
#include <ChipsetSetupConfig.h>
#include <PostCode.h>
#include <Guid/H2OCp.h>
#include <Protocol/GenericMemoryTest.h>
#include <Protocol/ConsoleControl.h>
#include <Protocol/BootLogo.h>
#include <Library/H2OCpLib.h>
#include <Library/PlatformBdsLib.h>
#include <Library/VariableLib.h>
#include <Library/DxeOemSvcKernelLib.h>
#include <Library/GenericUtilityLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/PlatformBootManagerLib.h>

#define DP_TYPE_MESSAGING  0x03
#define DP_SUBTYPE_USB     0x05

EFI_STATUS
BdsMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  * Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  * Single,
  IN  EFI_DEVICE_PATH_PROTOCOL  **NewDevicePath OPTIONAL,
  IN  BOOLEAN                   Delete
  )
/*++

Routine Description:
  Function compares a device path data structure to that of all the nodes of a
  second device path instance.

Arguments:
  Multi        - A pointer to a multi-instance device path data structure.

  Single       - A pointer to a single-instance device path data structure.

  NewDevicePath - If Delete is TRUE, this parameter must not be null, and it
                  points to the remaining device path data structure.
                  (remaining device path = Multi - Single.)

  Delete        - If TRUE, means removing Single from Multi.
                  If FALSE, the routine just check whether Single matches
                  with any instance in Multi.

Returns:

  The function returns EFI_SUCCESS if the Single is contained within Multi.
  Otherwise, EFI_NOT_FOUND is returned.

--*/
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath1;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath2;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInst;
  UINTN                     Size;

  //
  // The passed in DevicePath should not be NULL
  //
  if ((!Multi) || (!Single)) {
    return EFI_NOT_FOUND;
  }
  //
  // if performing Delete operation, the NewDevicePath must not be NULL.
  //
  TempDevicePath1 = NULL;

  DevicePath      = Multi;
  DevicePathInst  = GetNextDevicePathInstance (&DevicePath, &Size);

  //
  // search for the match of 'Single' in 'Multi'
  //
  while (DevicePathInst) {
    if (CompareMem (Single, DevicePathInst, Size) == 0) {
      if (!Delete) {
        gBS->FreePool (DevicePathInst);
        return EFI_SUCCESS;
      }
    } else {
      if (Delete) {
        TempDevicePath2 = AppendDevicePathInstance (
                            TempDevicePath1,
                            DevicePathInst
                            );
        gBS->FreePool (TempDevicePath1);
        TempDevicePath1 = TempDevicePath2;
      }
    }

    gBS->FreePool (DevicePathInst);
    DevicePathInst = GetNextDevicePathInstance (&DevicePath, &Size);
  }

  if (Delete) {
    *NewDevicePath = TempDevicePath1;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

BOOLEAN
FoundUSBConsoleOutput (
  OUT EFI_DEVICE_PATH_PROTOCOL  **UsbConsoleOuputDevPath
  )
{
  EFI_STATUS                            Status;
  UINTN                                 NumberOfHandles;
  EFI_HANDLE                            *HandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL              *GopDevicePath;
  UINTN                                 Index;
  EFI_DEVICE_PATH_PROTOCOL              *DevPathNode;
  EFI_DEVICE_PATH_PROTOCOL              *NewDevPath;
  BOOLEAN                               FoundUsbConsole;

  //
  // init locals
  //
  NumberOfHandles = 0;
  HandleBuffer = NULL;
  GopDevicePath = NULL;
  Index = 0;
  DevPathNode = NULL;
  NewDevPath = NULL;
  FoundUsbConsole = FALSE;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                );
  if (EFI_ERROR (Status)) {
    return FoundUsbConsole;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                  &gEfiDevicePathProtocolGuid,
                  (VOID*)&GopDevicePath
                  );
    if (EFI_ERROR (Status)) {
      continue;
    }

    DevPathNode = GopDevicePath;
    while (!IsDevicePathEnd (DevPathNode)) {
      if (DevicePathType (DevPathNode) == DP_TYPE_MESSAGING &&
          DevicePathSubType (DevPathNode) == DP_SUBTYPE_USB
          ) {
        NewDevPath = AppendDevicePathInstance (*UsbConsoleOuputDevPath, GopDevicePath);
        if (*UsbConsoleOuputDevPath != NULL) {
          FreePool (*UsbConsoleOuputDevPath);
        }
        *UsbConsoleOuputDevPath = NewDevPath;
        FoundUsbConsole = TRUE;
        break;
      }
      DevPathNode = NextDevicePathNode (DevPathNode);
    }
  }
  FreePool (HandleBuffer);

  return FoundUsbConsole;
}

VOID
SetUsbConsoleOutToConOutVar (
  IN EFI_DEVICE_PATH_PROTOCOL  *UsbConsoleOuputDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *VarConOut;
  UINTN                     DevPathSize;
  EFI_DEVICE_PATH_PROTOCOL  *NextDevPathInst;
  EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_STATUS                Status;
  UINTN                     Size;

  //
  //init locals
  //
  VarConOut = NULL;
  DevPathSize = 0;
  NextDevPathInst = NULL;
  TempDevicePath = NULL;
  RemainingDevicePath = UsbConsoleOuputDevPath;
  Size = 0;

  VarConOut = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevPathSize
                );

  do {
    NextDevPathInst = GetNextDevicePathInstance (&RemainingDevicePath, &Size);
    if (NextDevPathInst != NULL) {
      Status = BdsMatchDevicePaths(VarConOut, NextDevPathInst, NULL, FALSE);
      if (EFI_ERROR (Status)) {
        TempDevicePath = AppendDevicePathInstance (VarConOut, NextDevPathInst);
        if (VarConOut != NULL) {
          FreePool (VarConOut);
        }
        VarConOut = TempDevicePath;
      }
      FreePool (NextDevPathInst);
    }
  } while (RemainingDevicePath != NULL);

  gRT->SetVariable (
        L"ConOut",
        &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
        GetDevicePathSize (VarConOut),
        VarConOut
        );
}

/**
  Internal function to initalize H2O_BDS_CP_DISPLAY_BEFORE_DATA data.

  @param[in]  QuietBoot           Boolean value to indicate quiet boot is enabled or disabled.
  @param[out] CpData              A Pointer to H2O_BDS_CP_DISPLAY_BEFORE_DATA instance to initialized.

  @retval EFI_SUCCESS             Init H2O_BDS_CP_DISPLAY_BEFORE_DATA data successfully.
  @retval EFI_INVALID_PARAMETER   CpData is NULL.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory for H2O_BDS_CP_DISPLAY_BEFORE_DATA interface failed.
**/
STATIC
EFI_STATUS
InitBeforeDisplayData (
  IN  BOOLEAN                                QuietBoot,
  OUT H2O_BDS_CP_DISPLAY_BEFORE_DATA         **CpData
  )
{
  EFI_STATUS                           Status;
  VOID                                 *Interface;
  H2O_BDS_CP_DISPLAY_BEFORE_DATA       *BdsDisplayBeforeData;

  if (CpData == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  BdsDisplayBeforeData = AllocateZeroPool (sizeof (H2O_BDS_CP_DISPLAY_BEFORE_DATA));
  if (BdsDisplayBeforeData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (BdsDisplayBeforeData, sizeof (H2O_BDS_CP_DISPLAY_BEFORE_DATA));
  BdsDisplayBeforeData->Size   = sizeof (H2O_BDS_CP_DISPLAY_BEFORE_DATA);
  BdsDisplayBeforeData->Status = H2O_BDS_TASK_NORMAL;
  if (QuietBoot) {
    BdsDisplayBeforeData->Features |= H2O_BDS_CP_DISPLAY_LOGO;
    Status = gBS->LocateProtocol (&gEfiBootLogoProtocolGuid, NULL, (VOID **) &Interface);
    if (!EFI_ERROR (Status)) {
      BdsDisplayBeforeData->Features |= H2O_BDS_CP_DISPLAY_BGRT;
    }
  } else {
    BdsDisplayBeforeData->Features |= H2O_BDS_CP_DISPLAY_INFO;
  }
  *CpData = BdsDisplayBeforeData;
  return EFI_SUCCESS;
}

/**
  Internal function to initalize H2O_BDS_CP_DISPLAY_BEFORE_DATA
  data and trigger gH2OBdsCpDisplayBeforeGuid checkpoint.

  @param[in] QuietBoot            Boolean value to indicate quiet boot is enabled or disabled.

  @retval EFI_SUCCESS             Trigger gH2OBdsCpDisplayBeforeGuid checkpoint successfully.
  @retval EFI_ALREADY_STARTED     The gH2OBdsCpDisplayBeforeGuid checkpoint has been triggered.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
  @return Other                   Other error occurred while triggering gH2OBdsCpDisplayBeforeGuid
                                  checkpoint.
**/
STATIC
EFI_STATUS
TriggerCpDisplayBefore (
  IN  BOOLEAN       QuietBoot
  )
{
  EFI_STATUS                           Status;
  H2O_BDS_CP_DISPLAY_BEFORE_DATA       *BdsDisplayBeforeData;
  EFI_HANDLE                           Handle;

  Status = gBS->LocateProtocol (
                  &gH2OBdsCpDisplayBeforeGuid,
                  NULL,
                  (VOID **) &BdsDisplayBeforeData
                  );
  ASSERT (Status != EFI_SUCCESS);
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  Status = InitBeforeDisplayData (QuietBoot, &BdsDisplayBeforeData);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  //
  // Install checpoint data to protocol for later usage.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  (EFI_GUID *) &gH2OBdsCpDisplayBeforeGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID *) BdsDisplayBeforeData
                  );
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpDisplayBeforeGuid));
  Status = H2OCpTrigger (&gH2OBdsCpDisplayBeforeGuid, BdsDisplayBeforeData);
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsDisplayBeforeData->Status));
  return Status;
}

STATIC
EFI_STATUS
PlatformBdsConnectConsole (
  VOID
  )
{
  CHIPSET_CONFIGURATION                      *SystemConfiguration;
  EFI_DEVICE_PATH_PROTOCOL                   *UsbConsoleOuputDevPath = NULL;

  SystemConfiguration = (CHIPSET_CONFIGURATION *) CommonGetVariableData (L"Setup", &gSystemConfigurationGuid);
  if (SystemConfiguration == NULL) {
    return EFI_NOT_FOUND;
  }

  BdsLibConnectI2cDevice ();
  if (!(SystemConfiguration->UsbHotKeySupport == 0 && BdsLibIsWin8FastBootActive ())) {
    BdsLibConnectUsbHID ();
    if (FoundUSBConsoleOutput (&UsbConsoleOuputDevPath)) {
      SetUsbConsoleOutToConOutVar (UsbConsoleOuputDevPath);
      FreePool (UsbConsoleOuputDevPath);
    }
  } else {
    BdsLibConnectUsbHIDNotifyRegister ();
  }

  BdsLibConnectAllDefaultConsoles ();

  FreePool ((VOID *) SystemConfiguration);
  return EFI_SUCCESS;
}


/**
 Perform the platform diagnostic, such like test memory. OEM/IBV also
 can customize this function to support specific platform diagnostic.

 @param[in]         MemoryTestLevel     The memory test intensive level
 @param[in]         QuietBoot           Indicate if need to enable the quiet boot
 @param[in]         BaseMemoryTest      A pointer to BdsMemoryTest()
*/
VOID
PlatformBdsDiagnostics (
  IN EXTENDMEM_COVERAGE_LEVEL    MemoryTestLevel,
  IN BOOLEAN                     QuietBoot,
  IN BASEM_MEMORY_TEST           BaseMemoryTest
  )
{
  EFI_STATUS                        Status;
  UINT8                             PrintLine;
  EFI_OEM_BADGING_SUPPORT_PROTOCOL  *Badging;
  UINT16                            FunctionKey;
  BOOLEAN                           KeyPressed;
  BOOLEAN                           EnableAmtSol;

  PrintLine = 0;
  EnableAmtSol = FALSE;

  //
  // PostCode = 0x1D, Display logo or system information
  //
  POST_CODE (BDS_DISPLAY_LOGO_SYSTEM_INFO);

  BdsLibOnStartOfBdsDiagnostics ();

  //
  // OemServices
  //
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcDisplayLogo \n"));
  Status = OemSvcDisplayLogo (&QuietBoot);
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcDisplayLogo Status: %r\n", Status));

  if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
    TriggerCpDisplayBefore (QuietBoot);
  }

  if (QuietBoot) {
    EnableQuietBoot (&gEfiUgaSplashProtocolGuid);
  }
  BdsLibSetHotKeyDelayTime ();

  //
  // Perform system diagnostic
  //
  if (!QuietBoot) {
    gST->ConOut->ClearScreen (gST->ConOut);
  }

  BdsLibGetHotKey (&FunctionKey, &KeyPressed);
  Status = gBS->LocateProtocol (&gEfiOEMBadgingSupportProtocolGuid, NULL, (VOID **)&Badging);
  if (!EFI_ERROR (Status)) {
    if (KeyPressed) {
      if (QuietBoot) {
        ShowOemString (Badging, TRUE, (UINT8) FunctionKey);
      } else {
        BdsLibShowOemStringInTextMode (TRUE, (UINT8) FunctionKey);
      }
    }
  }
  //
  // Perform system diagnostic
  //
  if (!QuietBoot) {
    gST->ConOut->ClearScreen (gST->ConOut);
    if (KeyPressed) {
      Status = BdsLibShowOemStringInTextMode (TRUE, (UINT8)FunctionKey);
    } else {
      Status = BdsLibShowOemStringInTextMode (FALSE, 0);
    }

    gST->ConOut->SetCursorPosition (gST->ConOut, 0, 0);
    Status = ShowSystemInfo (&PrintLine);
  }
  Status = BaseMemoryTest (MemoryTestLevel, (UINTN) (PrintLine + 2));
  if (EFI_ERROR (Status)) {
    DisableQuietBoot ();
  }
  return;
}

/**
  Platform Bds Boot Selection

  @param  Selection     HotKey Selection

  @retval EFI_STATUS

**/
EFI_STATUS
PlatformBdsBootSelection (
  VOID
  )
{
  //
  // Disable Hot Key event and check Hot Key result
  //
  BdsLibStopHotKeyEvent ();

  gST->ConOut->EnableCursor (gST->ConOut, FALSE);

  BdsLibStartSetupUtility (TRUE);

  return EFI_SUCCESS;
}

/**
  Platform Bds init. Include the platform firmware vendor, revision
  and so crc check.

**/
VOID
EFIAPI
PlatformBdsInit (
  VOID
  )
{
}

/**
  This function locks platform flash that is not allowed to be updated during normal boot path.
  The flash layout is platform specific.
**/
VOID
EFIAPI
PlatformBdsLockNonUpdatableFlash (
  VOID
  )
{
}

VOID
EFIAPI
PlatformBdsPolicyBehavior (
  IN LIST_ENTRY                      *DriverOptionList OPTIONAL,
  IN LIST_ENTRY                      *BootOptionList OPTIONAL,
  IN PROCESS_CAPSULES                ProcessCapsules,
  IN BASEM_MEMORY_TEST               BaseMemoryTest
  )
{
  CHIPSET_CONFIGURATION              *SystemConfiguration;
  EXTENDMEM_COVERAGE_LEVEL           MemoryTestLevel;

  SystemConfiguration = (CHIPSET_CONFIGURATION *) CommonGetVariableData (L"Setup", &gSystemConfigurationGuid);
  if (SystemConfiguration == NULL) {
    return;
  }

  PlatformBootManagerBeforeConsole ();

  PlatformBdsConnectConsole ();
  MemoryTestLevel = (SystemConfiguration->QuickBoot == TRUE) ? IGNORE : EXTENSIVE;
  PlatformBdsDiagnostics (
    MemoryTestLevel,
    SystemConfiguration->QuietBoot,
    BaseMemoryTest
    );

  if (BdsLibIsWin8FastBootActive ()) {
    BdsLibConnectTargetDev ();
  } else {
    EfiBootManagerConnectAll ();
  }

  BdsLibEnumerateAllBootOption (FALSE, NULL);

  PlatformBdsBootSelection ();

  FreePool ((VOID *) SystemConfiguration);
}

VOID
PlatformBdsEnterFrontPage (
  IN UINT16                 TimeoutDefault,
  IN BOOLEAN                ConnectAllHappened
  )
{
  LaunchBootOptionByDevicePath ((EFI_DEVICE_PATH_PROTOCOL *) &gH2OFrontPageDevicePath);
}

