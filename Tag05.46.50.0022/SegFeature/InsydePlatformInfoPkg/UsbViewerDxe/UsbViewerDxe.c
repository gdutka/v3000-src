/** @file
  Entry point and main driver routines of H2O USB Viewer

;******************************************************************************
;* Copyright (c) 2022 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/SetupUtilityApplication.h> // InsydeModulePkg
#include <Protocol/H2OUsbTextLookup.h>        // InsydePlatformInfoPkg

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HiiLib.h>                      // MdeModulePkg
#include <Library/VariableLib.h>                 // InsydeModulePkg
#include <Library/DeviceInfoLookupDataLib.h>     // InsydePlatformInfoPkg
#include <Library/DxeSetupVariableLib.h>

#include "UsbViewerDxe.h"
#include "UsbViewerSetup.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

extern EFI_GUID gH2ODeviceInfoTokenSpaceGuid; // AutoGen.c (PCD token space)

H2O_USB_VIEWER_PRIVATE_DATA mH2OUsbViewerData = {
  H2O_USB_VIEWER_PRIVATE_SIGNATURE,
  {             // ConfigAccess (protocol)
    H2OUsbViewerExtractConfig,
    H2OUsbViewerRouteConfig,
    H2OUsbViewerFormCallback
  },
  STRING_TOKEN (STR_USB_DEVICE_FORM_SUBTITLE),
  STRING_TOKEN (STR_DISCONNECTED_DEVICE_PROMPT),
  STRING_TOKEN (STR_USB_DEVICE_COUNT_SUBTITLE),
  STRING_TOKEN (STR_USB_HOST_CONTROLLER_FORM),
  {
    STRING_TOKEN (STR_USB_DEPTH_0_DEVICE_FORM),
    STRING_TOKEN (STR_USB_DEPTH_1_DEVICE_FORM),
    STRING_TOKEN (STR_USB_DEPTH_2_DEVICE_FORM),
    STRING_TOKEN (STR_USB_DEPTH_3_DEVICE_FORM),
    STRING_TOKEN (STR_USB_DEPTH_4_DEVICE_FORM),
    STRING_TOKEN (STR_USB_DEPTH_5_DEVICE_FORM)
  },
  {             // ViewOption
    FALSE,      //   FlatView
    FALSE,      //   ShowHubInformation
    FALSE,      //   ShowFullPortNumbers
    0x0000      //   SelectedLangId
  },
  NULL,         // ServiceHandle
  NULL,         // HiiHandle
  NULL,         // UsbTextLookup
  NULL,         // DeviceInfoLookup
  NULL,         // FormRefreshEvent
  NULL,         // StatementStringIds
  NULL,         // GotoOpEntries
  NULL,         // DownstreamCountEntries
  NULL,         // SelectedHostControllerHandle
  { NULL },     // SelectedUsbDeviceHandleStack
  { 0, { 0 } }, // SelectedPortNumberChain
  { 0, { 0 } }, // DisconnectedPortNumberChain
  0,            // NumIfrStatementsAllocated
  0,            // NumGotoOpsAllocated
  0,            // LastDeviceGotoOpIndex
  0x0000,       // ActiveForm
  0,            // FormOpenCount
};

HII_VENDOR_DEVICE_PATH  mHiiUsbDeviceTreeViewerVendorDevPath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    H2O_USB_VIEWER_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

EFI_STATUS
GetProtocolWithCorrectStatus (
  IN EFI_HANDLE      Handle,
  IN CONST EFI_GUID  *Protocol,
  OUT VOID           **Interface OPTIONAL
  )
{
  UINT32      Attributes;
  EFI_STATUS  Status;

  if (Interface != NULL) {
    Attributes = EFI_OPEN_PROTOCOL_GET_PROTOCOL;
  } else {
    Attributes = EFI_OPEN_PROTOCOL_TEST_PROTOCOL;
  }

  Status = gBS->OpenProtocol (
                  Handle,
                  (EFI_GUID *) Protocol,
                  Interface,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  Attributes
                  );
  if (Status == EFI_INVALID_PARAMETER) {
    //
    // If the handle is destroyed before OpenProtocol() can open it, the
    // function returns EFI_INVALID_PARAMETER. This behavior is unspecified in
    // UEFI specification but seen in EDK II's implementation (MdeModulePkg).
    // Change to a status that makes more sense in DEBUG() output.
    //
    Status = EFI_UNSUPPORTED;
  }
  return Status;
}

/**
  Free the data and the Hii Handle.

  @return  Void.

**/
STATIC
VOID
CleanUpUsbViewerPrivateData (
  IN H2O_USB_VIEWER_PRIVATE_DATA  *PrivateData
  )
{
  if (PrivateData->ServiceHandle == NULL) {
    return;
  }

  if (PrivateData->DownstreamCountEntries != NULL) {
    FreePool (PrivateData->DownstreamCountEntries);
    PrivateData->DownstreamCountEntries = NULL;
  }

  if (PrivateData->GotoOpEntries != NULL) {
    FreePool (PrivateData->GotoOpEntries);
    PrivateData->GotoOpEntries = NULL;
    PrivateData->NumGotoOpsAllocated = 0;
  }

  if (PrivateData->StatementStringIds != NULL) {
    FreePool (PrivateData->StatementStringIds);
    PrivateData->StatementStringIds = NULL;
    PrivateData->NumIfrStatementsAllocated = 0;
  }

  if (PrivateData->FormRefreshEvent != NULL) {
    gBS->CloseEvent (PrivateData->FormRefreshEvent);
    PrivateData->FormRefreshEvent = NULL;
  }

  PrivateData->ActiveForm = 0x0000;

  return;
}

/**
  Unloads this driver and its installed protocol.

  @implements EFI_IMAGE_UNLOAD

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
H2OUsbViewerUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  CleanUpUsbViewerPrivateData (&mH2OUsbViewerData);

  if (mH2OUsbViewerData.ServiceHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mH2OUsbViewerData.ServiceHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiUsbDeviceTreeViewerVendorDevPath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mH2OUsbViewerData.ConfigAccess,
           NULL
           );
    mH2OUsbViewerData.ServiceHandle = NULL;
  }

  if (mH2OUsbViewerData.HiiHandle != NULL) {
    HiiRemovePackages (mH2OUsbViewerData.HiiHandle);
  }

  gBS->CloseEvent (mH2OUsbViewerData.SetupUtilityEvent);

  return EFI_SUCCESS;
}

EFI_STATUS
H2OUsbViewerGetDefaultViewOption (
  OUT H2O_USB_VIEWER_VIEW_OPTION  *ViewOptionBuffer
  )
{
  EFI_STATUS  Status;
  UINTN       DataSize;

  ASSERT (ViewOptionBuffer != NULL);

  DataSize = sizeof (ViewOptionBuffer);
  Status = CommonGetDefaultVariable (
             (CHAR16 *) L"H2OUsbViewerViewOption",
             &gH2OUsbViewerFormsetGuid,
             (H2O_BOARD_ID) LibPcdGetSku (),
             NULL, // Attributes
             &DataSize,
             ViewOptionBuffer
             );
  if (EFI_ERROR (Status)) {
    Status = CommonGetDefaultVariable (
               (CHAR16 *) L"H2OUsbViewerViewOption",
               &gH2OUsbViewerFormsetGuid,
               0,    // SkuId
               NULL, // Attributes
               &DataSize,
               ViewOptionBuffer
               );
  }
  return Status;
}

/**
  To collect data and initialize the Usb Device Tree Viewer formset after entering setup utility application

  @implements EFI_EVENT_NOTIFY

  @param[in]  Event                   Event
  @param[in]  Context                 Context

  No return value.
**/
VOID
EFIAPI
H2OUsbViewerNotifier (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                              Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL  *SetupUtilityApp;
  EFI_STATUS                              WarnStatus;
  UINTN                                   DataSize;
  H2O_USB_VIEWER_VIEW_OPTION              ViewOptionBuffer;
  EFI_TPL                                 OldTpl;

  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  NULL, // Registration
                  (VOID **) &SetupUtilityApp
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (SetupUtilityApp->VfrDriverState == ShutdownSetupUtility) {
    //
    // Load ViewOptions from EFI Variable storage
    //
    DataSize = sizeof (ViewOptionBuffer);
    WarnStatus = gRT->GetVariable (
                        (CHAR16 *) L"H2OUsbViewerViewOption",
                        &gH2OUsbViewerFormsetGuid,
                        NULL, // Attributes
                        &DataSize,
                        &ViewOptionBuffer
                        );
    if (EFI_ERROR (WarnStatus)) {
      WarnStatus = H2OUsbViewerGetDefaultViewOption (&ViewOptionBuffer);
    }
    if (!EFI_ERROR (WarnStatus)) {
      OldTpl = gBS->RaiseTPL (H2O_USB_VIEWER_FORM_REFRESH_TPL);
      mH2OUsbViewerData.ViewOption = ViewOptionBuffer;
      gBS->RestoreTPL (OldTpl);
    }
    CleanUpUsbViewerPrivateData (&mH2OUsbViewerData);
  }
  return;
}

/**
  The entry point for the driver.

  @implements EFI_IMAGE_ENTRY_POINT

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
H2OUsbViewerEntry (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS                   Status;
  EFI_HII_HANDLE               HiiHandle;
  VOID                         *Registration;
  UINTN                        BufferSize;
  EFI_STRING                   ConfigRequestHdr;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mH2OUsbViewerData.ServiceHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiUsbDeviceTreeViewerVendorDevPath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mH2OUsbViewerData.ConfigAccess,
                  (EFI_GUID *) NULL
                  );
  if (EFI_ERROR (Status)) {
    CleanUpUsbViewerPrivateData (&mH2OUsbViewerData);
    mH2OUsbViewerData.ServiceHandle = NULL;
    return Status;
  }

  //
  // Publish our HII data
  //
  HiiHandle = HiiAddPackages (
                &gH2OUsbViewerFormsetGuid,
                mH2OUsbViewerData.ServiceHandle,
                UsbViewerDxeStrings,
                UsbViewerVfrBin,
                NULL
                );
  if (HiiHandle == NULL) {
    CleanUpUsbViewerPrivateData (&mH2OUsbViewerData);
    mH2OUsbViewerData.ServiceHandle = NULL;
    return Status;
  }
  mH2OUsbViewerData.HiiHandle = HiiHandle;

  //
  // Locate H2O USB Text Lookup protocol. (Optional dependency)
  //
  Status = gBS->LocateProtocol (
                  &gH2OUsbTextLookupProtocolGuid,
                  NULL, // Registration
                  (VOID **) &mH2OUsbViewerData.UsbTextLookup
                  );
  if (EFI_ERROR (Status)) {
    mH2OUsbViewerData.UsbTextLookup = NULL;
  }

  //
  // Set up H2O Device Info Lookup data. (Optional dependency)
  //
  mH2OUsbViewerData.DeviceInfoLookup = NULL;
  InitDeviceInfoLookupProtocol (&mH2OUsbViewerData.DeviceInfoLookup);
  if (mH2OUsbViewerData.DeviceInfoLookup != NULL &&
      mH2OUsbViewerData.DeviceInfoLookup->Usb == NULL)
  {
    DEBUG ((
      EFI_D_WARN,
      "%a: H2ODeviceInfoLookupProtocol does not support USB lookup interface.\n",
      gEfiCallerBaseName
      ));
    mH2OUsbViewerData.DeviceInfoLookup = NULL;
  }

  //
  // Check the status of the setup variable. Note that it has a read-only
  // default value stored in the variable defaults region.
  //
  BufferSize = sizeof (H2O_USB_VIEWER_VIEW_OPTION);
  Status = CommonGetVariable (
             L"H2OUsbViewerViewOption",
             &gH2OUsbViewerFormsetGuid,
             &BufferSize,
             &mH2OUsbViewerData.ViewOption
             );
  if (EFI_ERROR (Status) ||
      !IsVariableInVariableStoreRegion (L"H2OUsbViewerViewOption", &gH2OUsbViewerFormsetGuid)) {
    BufferSize = sizeof (H2O_USB_VIEWER_VIEW_OPTION);
    Status = InitVarDefault (
               L"H2OUsbViewerViewOption",
               &gH2OUsbViewerFormsetGuid,
               &BufferSize,
               &mH2OUsbViewerData.ViewOption
               );
    if (EFI_ERROR (Status)) {
      H2OUsbViewerUnload (ImageHandle);
      return Status;
    }
  }

  ConfigRequestHdr = HiiConstructConfigHdr (
                       &gH2OUsbViewerFormsetGuid,
                       L"H2OUsbViewerViewOption",
                       mH2OUsbViewerData.ServiceHandle
                       );
  if (ConfigRequestHdr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiValidateSettings (ConfigRequestHdr)) {
    FreePool (ConfigRequestHdr);
    H2OUsbViewerUnload (ImageHandle);
    return EFI_INVALID_PARAMETER;
  }

  FreePool (ConfigRequestHdr);

  //
  // When execute Setup Utility application, install HII data
  //
  mH2OUsbViewerData.SetupUtilityEvent = EfiCreateProtocolNotifyEvent (
                                          &gEfiSetupUtilityApplicationProtocolGuid,
                                          TPL_CALLBACK - 1,
                                          H2OUsbViewerNotifier,
                                          NULL,
                                          &Registration
                                          );
  if (mH2OUsbViewerData.SetupUtilityEvent == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Can't create notify event SetupUtilityEvent\n"));
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}

