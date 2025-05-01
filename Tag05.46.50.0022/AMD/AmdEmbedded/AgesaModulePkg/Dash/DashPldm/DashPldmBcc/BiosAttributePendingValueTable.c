/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "DashPldmBcc.h"
#include "BiosAttributePendingValueTable.h"

PLDM_PENDING_VALUE_STATUS mPldmPendingValueStatus;


/**
  Update the EFI variable, L"BootOrder" based on the remote "BIOS Attribute Pending Value Table" data.

  @param [in] mPldmProtocol     Pointer to the instance of "AMD PLDM PROTOCOL"
  @param [in out] Updated       TRUE - Boot options have been updated successfully. FLASE - No action.

  @retval EFI_SUCCESS           Successful
  @retval EFI_INVALID_PARAMETER Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES  Unable to allocate memory.
  @retval EFI_DEVICE_ERROR      Unexpected data from the remote device.
  @retval EFI_MEDIA_CHANGED     Boot Options or Boot Order has been changed.

**/
EFI_STATUS
EFIAPI
UpdatePendingBootConfigSetting (
  IN AMD_DASH_PLDM_PROTOCOL *mPldmProtocol,
  IN OUT BOOLEAN *Updated
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("%a, Enter %r\n", __FUNCTION__);
  if (mPldmProtocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Updated = FALSE;

  EFI_STATUS Status;
  BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_SETTING BiosBootConfigSetting;
  UINT32 BbcsSize = sizeof (BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_SETTING);
  UINT16 *NewBootOrder = NULL;
  UINT16 *BootOrderList = NULL;
  UINTN  BootOrderListSize;
  UINT8  Index;

  Status = mPldmProtocol->DashGetBiosTable (
                            mPldmProtocol,
                            DASH_PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE,
                            (DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BiosBootConfigSetting,
                            &BbcsSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("GetBiosTable: DASH_PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE Status=%r\n", Status);
  if (EFI_ERROR (Status)) goto Exit;

  mPldmPendingValueStatus = PLDM_PENDING_VALUE_STATUS_REJECTED;

  if ((BiosBootConfigSetting.BapvteHeader.BiosAttributeHandle != PLDM_BIOS_ATTRIBUTE_HANDLE_BOOT_CONFIG_SETTING) ||
      (BiosBootConfigSetting.BapvteHeader.BiosAttributeType != PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSBootConfigSetting) ||
      (BiosBootConfigSetting.OrderAndFailThroughMode != BIOS_ATTRIBUTE_PENDING_VALUE_ORDERANDFAILTHROUGHMODE_OrderedAndFailThrough)) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  if ((BiosBootConfigSetting.BootConfigType == BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Default) ||\
    (BiosBootConfigSetting.BootConfigType == BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Onetime)) {
    // Setup the new Boot Order List according to the pending value settings.
    NewBootOrder = (UINT16*) AllocatePool (BiosBootConfigSetting.NumberOfPendingBootSourceSettings * sizeof (UINT16));
    if (NewBootOrder == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    // Get the original Boot Order List.
    Status = GetEfiGlobalVariable2 (L"BootOrder", (VOID **) &BootOrderList, &BootOrderListSize);
    if (EFI_ERROR (Status) || (BootOrderList == NULL) || (BootOrderListSize == 0)) {
      Status = EFI_NOT_FOUND;
      goto Exit;
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("Dump original Boot Order:\n");
    IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN) BootOrderList, (UINT32)BootOrderListSize);
    // Note: The boot media change should be addressed, or else we may meet the array overrun problem with BootOrderList.
    if (BootOrderListSize/sizeof(UINT16) != BiosBootConfigSetting.NumberOfPendingBootSourceSettings) {
      Status = EFI_MEDIA_CHANGED;
      goto Exit;
    }

    for (Index = 0; Index < BootOrderListSize/sizeof(UINT16); Index ++) {
      NewBootOrder[Index] = BootOrderList[BiosBootConfigSetting.BootSourceStringHandleIndex[Index]];
    }

    IDS_HDT_CONSOLE_PSP_TRACE ("Dump new Boot Order:\n");
    IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN) NewBootOrder, (UINT32)BootOrderListSize);

    if (BiosBootConfigSetting.BootConfigType == BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Default) {
      Status = gRT->SetVariable (
                    L"BootOrder",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    BiosBootConfigSetting.NumberOfPendingBootSourceSettings * sizeof (UINT16),
                    NewBootOrder
                    );
      IDS_HDT_CONSOLE_PSP_TRACE ("UpdatePendingBootConfigSetting BootOrder Status = %r: \n", Status);
      if (EFI_ERROR (Status)) goto Exit;

      *Updated = TRUE;
      mPldmPendingValueStatus = PLDM_PENDING_VALUE_STATUS_ACCEPTED;
    } else {
      Status = gRT->SetVariable (
                  L"BootNext",
                  &gEfiGlobalVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                  sizeof (UINT16),
                  NewBootOrder
                  );
      IDS_HDT_CONSOLE_PSP_TRACE ("UpdatePendingBootConfigSetting BootNext Status = %r: \n", Status);
      if (EFI_ERROR (Status)) goto Exit;
      mPldmPendingValueStatus = PLDM_PENDING_VALUE_STATUS_ACCEPTED;
    }
  }

Exit:
  IDS_HDT_CONSOLE_PSP_TRACE ("mPldmPendingValueStatus =%d\n", mPldmPendingValueStatus);
  if (mPldmPendingValueStatus == PLDM_PENDING_VALUE_STATUS_ACCEPTED) {
    UINT16 AttributeHandles[] = {PLDM_BIOS_ATTRIBUTE_HANDLE_BOOT_CONFIG_SETTING}; // Currently, only "BiosBootConfigSetting"/BootOrder is designated for processing.
    mPldmProtocol->DashAcceptBiosAttributesPendingValues (mPldmProtocol, sizeof(AttributeHandles)/sizeof(*AttributeHandles), AttributeHandles);
  }
  else if (mPldmPendingValueStatus == PLDM_PENDING_VALUE_STATUS_REJECTED) {
    mPldmProtocol->DashAcceptBiosAttributesPendingValues (mPldmProtocol, 0, NULL);
  }

  if (BootOrderList != NULL) {
    FreePool (BootOrderList);
    BootOrderList = NULL;
  }
  if (NewBootOrder != NULL) {
    FreePool (NewBootOrder);
    NewBootOrder = NULL;
  }

  return Status;
}

