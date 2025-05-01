/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#include "AodDxe.h"
AOD_MOUDLE_NAME ("AodDxe")

//
// Global Variables.
//
HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath0 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    AOD_SETUP_FORMSET_GUID
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
EFI_GUID mFormSetGuid = AOD_SETUP_FORMSET_GUID;

//[-start-220906-IB14740262-add]//
EFI_STATUS
AodUpdateApcbTokens (
  VOID *AodVariable,
  VOID *vApcbProtocol
  );

EFI_STATUS
InitializeAodApcbDefault (
  IN UINT8   *IfrData
  )
{
  EFI_STATUS                      Status;
  AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol;

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &ApcbDxeServiceProtocol);
  if (!EFI_ERROR (Status)) {
    AodUpdateApcbTokens ((VOID *)IfrData, ApcbDxeServiceProtocol);
  } else {
    DEBUG ((EFI_D_ERROR, "Unable to locate APCB Protocol\n"));
  }

  return Status;
}
//[-end-220906-IB14740262-add]//

/**
  Entry point of the AMD AOD Init DXE driver

  @param[in]     ImageHandle    Pointer to the firmware file system header
  @param[in]     SystemTable    Pointer to System table

  @retval        EFI_SUCCESS    Module initialized successfully
  @retval        EFI_ERROR      Initialization failed (see error for more details)

**/
EFI_STATUS
EFIAPI
AodDxeEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_HII_DATABASE_PROTOCOL               *HiiDatabase;
  EFI_HII_STRING_PROTOCOL                 *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL         *HiiConfigRouting;
  AOD_DXE_PRIVATE_DATA                    *Private;
  VOID                                    *AodConfig;
  AOD_VARIABLE_HEADER                     AodConfigHeader;
  EFI_HANDLE                              *Handles;

  AOD_DEBUG ((EFI_D_ERROR, "Started\n"));
  //
  // Check And Build Aod Variable
  //
  Status = AodGetConfiguration (&AodConfig);
  AOD_DEBUG ((EFI_D_ERROR, "AodGetConfiguration Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    //
    // This WA for VARSTORE already in NVRAM
    //
    ZeroMem (&AodConfigHeader, sizeof (AOD_VARIABLE_HEADER));
    Status = AodDispatchInitTable (AOD_UPDATE_VARIABLE, &AodConfigHeader);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error, Status = %r\n", Status));
      return Status;
    }
    if (CompareMem (&AodConfigHeader, AodConfig, sizeof (AOD_VARIABLE_HEADER))) {
      AOD_DEBUG ((EFI_D_ERROR, "  Variable Header mismatched\n"));
//[-start-220906-IB14740262-modify]//
      Status = AodDispatchInitTable (AOD_UPDATE_VARIABLE, AodConfig);
      if (EFI_ERROR (Status)) {
        AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error, Status = %r\n", Status));
        return Status;
      }
      Status = AodSetConfiguration (AodConfig, ((AOD_VARIABLE_HEADER *)AodConfig)->AodVariableSize);
      if (EFI_ERROR (Status)) {
        AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error,VarSize=%04x Status = %r\n", \
          ((AOD_VARIABLE_HEADER *)AodConfig)->AodVariableSize, Status));
        return Status;
      }
      Status = InitializeAodApcbDefault (AodConfig);
      if (EFI_ERROR (Status)) {
        AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Sync APCB Error, Status = %r\n", Status));
        return Status;
      }
      Status = gBS->InstallProtocolInterface (
                      &ImageHandle,
                      &gH2OAodApcbSyncDoneProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      NULL
                      );
//[-end-220906-IB14740262-modify]//
    }
  }
  if (EFI_ERROR (Status)) {
    AodConfig = AllocateZeroPool (0x10000);
    if (AodConfig == NULL) return EFI_OUT_OF_RESOURCES;
    Status = AodDispatchInitTable (AOD_CONFIG_LOAD_DEFAULT, AodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error, Status = %r\n", Status));
      return Status;
    }
    Status = AodDispatchInitTable (AOD_UPDATE_VARIABLE, AodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error, Status = %r\n", Status));
      return Status;
    }
    Status = AodSetConfiguration (AodConfig, ((AOD_VARIABLE_HEADER *)AodConfig)->AodVariableSize);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error,VarSize=%04x Status = %r\n", \
        ((AOD_VARIABLE_HEADER *)AodConfig)->AodVariableSize, Status));
      return Status;
    }
  }
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error, Status = %r\n", Status));
    return Status;
  }
  //
  // Debug for all AodConfig Setup Settings
  //
  Status = AodDispatchInitTable (AOD_CONFIG_DEBUG, AodConfig);

  Private = AllocateZeroPool (sizeof (AOD_DXE_PRIVATE_DATA));
  if (Private == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Locate Hii Database protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Locate %g Protocol Status =%r\n", &gEfiHiiDatabaseProtocolGuid, Status));
    return Status;
  }

  //
  // Locate HiiString protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Locate %g Protocol Status =%r\n", &gEfiHiiStringProtocolGuid, Status));
    return Status;
  }

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **) &HiiConfigRouting);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Locate %g Protocol Status =%r\n", &gEfiHiiConfigRoutingProtocolGuid, Status));
    return Status;
  }

  Private->Signature   = AOD_DXE_PRIVATE_SIGNATURE;
  Status = AodGetConfiguration (&Private->IfrData);
  ASSERT_EFI_ERROR (Status);
  ASSERT (Private->IfrData != NULL);


  Private->ConfigAccess.ExtractConfig = AodExtractConfig;
  Private->ConfigAccess.RouteConfig   = AodRouteConfig;
  Private->ConfigAccess.Callback      = AodCallback;
  Private->HiiDatabase                = HiiDatabase;
  Private->HiiString                  = HiiString;
  Private->HiiConfigRouting           = HiiConfigRouting;
  Private->FormsetManageProtocol.LoadDefault = AodLoadDefault;
  //
  //  Install Hii Config access protocol
  //
  Private->DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &Private->ConfigAccess,
                  &gAmdAodFormsetManageProtocolGuid,
                  &Private->FormsetManageProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Install %g Protocol Status =%r\n", &gEfiHiiConfigAccessProtocolGuid, Status));
    return Status;
  }

  AOD_DEBUG ((EFI_D_INFO, "  Handle @ %x\n",  (UINTN)Private->DriverHandle));


  //
  // Publish the HII package list
  //
  Status = AodDispatchInitTable (AOD_UPDATE_HIIRESOURCE, Private->DriverHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Publish Hii Callback
  //
  InitializeListHead (&Private->CallbackList);
  AodDispatchInitTable (AOD_UPDATE_HII_CALLBACK, &Private->CallbackList);

  //
  // We assume only have one AOD EFI_HII_HANDLE
  //
  Private->HiiHandle = NULL;
  Handles = HiiGetHiiHandles (&mFormSetGuid);
  if (Handles == NULL) {
    return EFI_NOT_FOUND;
  }
  if (Handles[0] != NULL) {
    Private->HiiHandle = Handles[0];
  }
  AOD_DEBUG ((EFI_D_INFO, "  Add HiiPkg @ %x\n",  (UINTN)Private->HiiHandle));

  if (Private->HiiHandle == NULL) {
    AOD_DEBUG ((EFI_D_ERROR, "  AodFormset %g is not found in HiiPackage\n", &mFormSetGuid));
    gBS->UninstallMultipleProtocolInterfaces (
                  &Private->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &Private->ConfigAccess,
                  NULL
                  );
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AodInstallAcpi (Private);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Install Acpi failed status = %r\n",  Status));
    return Status;
  }

  Status = AodInstallCmdProtocol (Private);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Install Acpi failed status = %r\n", Status));
    return Status;
  }
  AOD_DEBUG ((EFI_D_INFO, "  Handle @ %x\n",  (UINTN)Private->DriverHandle));
  AOD_DEBUG ((EFI_D_ERROR, "End\n"));
  return EFI_SUCCESS;
}
