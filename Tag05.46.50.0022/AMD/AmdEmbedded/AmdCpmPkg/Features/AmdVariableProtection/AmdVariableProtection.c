/*****************************************************************************
 *
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HiiConfigRouting.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <AmdVariableProtectionItem.h>
#include <Library/AmdVariableProtectionLib.h>

EFI_HII_EXTRACT_CONFIG                    mOriginalHiiConfigRoutingExtractConfig = NULL;

EFI_STATUS
EFIAPI
AmdVariableProtectionHiiConfigRoutingExtractConfig (
  IN  CONST EFI_HII_CONFIG_ROUTING_PROTOCOL  *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                              Status;

  if (AmdVariableProtectionLibGetLockState()) {
    AmdVariableProtectionLibSetLockState (AMD_VAR_UNLOCK);
  }

  //
  // efivarstore type in VFR will use HiiConfigRouting.ExtractConfig
  //
  Status = mOriginalHiiConfigRoutingExtractConfig  (
             This,
             Request,
             Progress,
             Results
             );

  return Status;
}

VOID
EFIAPI
AmdVariableProtectionConfigRoutingCallback (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  EFI_STATUS                           Status;
  EFI_HII_CONFIG_ROUTING_PROTOCOL      *HiiConfigRouting;

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));


  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **)&HiiConfigRouting
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a : Could not locate HiiConfigRouting protocol! %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  mOriginalHiiConfigRoutingExtractConfig = HiiConfigRouting->ExtractConfig;
  HiiConfigRouting->ExtractConfig = AmdVariableProtectionHiiConfigRoutingExtractConfig;

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));

  return;
}

VOID
EFIAPI
AmdVariableProtectionLockAtReadyToBoot (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  if (!AmdVariableProtectionLibGetLockState()) {
    AmdVariableProtectionLibSetLockState (AMD_VAR_LOCK);
  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD Variable Protection DXE driver
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdVariableProtectionEntry (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                           Status;
  EFI_BOOT_MODE                        BootMode;
  UINTN                                Index;
  AMD_VARIABLE_PROTECTION_ITEM         *ItemList = NULL;
  UINTN                                ItemListNum = 0;
  CHAR16                               VariableName[MAX_AMD_VARIABLE_PROTECTION_VARIABLE_NAME];
  VOID                                 *Registration;
  EFI_EVENT                            ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));

  if (!PcdGetBool(PcdAmdVariableProtection)){
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End Amd Variable Protection Not Enabled\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  BootMode = GetBootModeHob();
  if ((BootMode == BOOT_ON_FLASH_UPDATE) || (BootMode == BOOT_IN_RECOVERY_MODE)) {
    DEBUG ((DEBUG_INFO,
      "%a : BootMode is %a, don't lock variables\n",
      __FUNCTION__,
      (BootMode == BOOT_ON_FLASH_UPDATE) ? "BOOT_ON_FLASH_UPDATE" : "BOOT_IN_RECOVERY_MODE"
      ));
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  Status = AmdVariableProtectionLibInitialize ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a : call AmdVariableProtectionLibInitialize() failed: %r\n", __FUNCTION__, Status));
    if (Status != EFI_ALREADY_STARTED) {
      ASSERT_EFI_ERROR (Status);
    }
    return Status;
  }

  ItemListNum = PcdGetSize (PcdAmdVariableProtectionTable) / sizeof (AMD_VARIABLE_PROTECTION_ITEM);
  ItemList    = (AMD_VARIABLE_PROTECTION_ITEM *) PcdGetPtr (PcdAmdVariableProtectionTable);

  for (Index = 0; Index < ItemListNum; Index++) {

    if (ItemList[Index].VariableName[0] == '\0') {
      continue;
    }

    // Make sure string array end with '\0'
    CopyMem (VariableName, ItemList[Index].VariableName, MAX_AMD_VARIABLE_PROTECTION_VARIABLE_NAME);
    VariableName[MAX_AMD_VARIABLE_PROTECTION_VARIABLE_NAME - 1] = L'\0';

    Status = AmdVariableProtectionLibLockVariable (
               VariableName,
               (EFI_GUID *) ItemList[Index].VendorGuid
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a : AmdVariableProtectionLibLockVariable() fail, %s:%g! %r\n",
        __FUNCTION__,
        VariableName,
        (EFI_GUID *) ItemList[Index].VendorGuid,
        Status
        ));
      ASSERT_EFI_ERROR (Status);
      continue;
    }

    DEBUG ((EFI_D_ERROR, "PcdAmdVariableProtectionTable.ItemList[%d].VariableName: %s\n",
      Index,
      VariableName
      ));
    DEBUG ((EFI_D_ERROR, "PcdAmdVariableProtectionTable.ItemList[%d].VendorGuid: %g\n",
      Index,
      (EFI_GUID *) ItemList[Index].VendorGuid
      ));

  }

  EfiCreateProtocolNotifyEvent (
    &gEfiHiiConfigRoutingProtocolGuid,
    TPL_NOTIFY,
    AmdVariableProtectionConfigRoutingCallback,
    NULL,
    &Registration
    );

  //
  // Register locked variable ReadyToBootEvent to enable variable protection again after unlock at setup menu.
  //

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  AmdVariableProtectionLockAtReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: CreateEvent gEfiEventReadyToBootGuid Status=%r\n", __FUNCTION__, Status));
  }

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));

  return Status;
}
