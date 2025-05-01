/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <AmdCpmBase.h>

#define PLATFORM_OSC_ACPI_OEM_TABLE_ID   SIGNATURE_64('C', 'P', 'M', 'M', 'S', 'O', 'S', 'C')

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update PlatformOsc SSDT Table
 *
 * This function is used to update MSDB and U4PC
 *
 * @param[in]  This           Pointer to Protocol
 * @param[in]  AmlObjPtr      The AML Object Buffer
 * @param[in]  Context        The Parameter Buffer
 *
 * @retval     TRUE           SSDT Table has been updated completely
 * @retval     FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AcpiPlatformOscCallBack (
  IN VOID   *This,
  IN VOID   *AmlObjPtr,
  IN VOID   *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('M', 'S', 'D', 'B'):
    if (*(BufferPtr + 0)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8*) AmlObjPtr) + 4) = (UINT8)(*(BufferPtr + 0));
        DEBUG ((DEBUG_INFO, "  %a(%d) MSDB is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('U', '4', 'P', 'C'):
    if (*(BufferPtr + 1)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8*) AmlObjPtr) + 5) = (UINT8)(*(BufferPtr + 1));
        DEBUG ((DEBUG_INFO, "  %a(%d) U4PC is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
VOID
EFIAPI
AmdCpmPlatformOscSsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  EFI_STATUS               Status;
  UINT64                   AcpiPlatformOscTableId;
  UINT32                   Buffer[2] = {0x00};
  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-Start\n", __FUNCTION__, __LINE__));

  AcpiPlatformOscTableId = PLATFORM_OSC_ACPI_OEM_TABLE_ID;

  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID**)&CpmTableProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-Locate gAmdCpmTableProtocolGuid: %r\n", __FUNCTION__, __LINE__, Status));
    return;
  }


  Buffer[0] = PcdGetBool(PcdModernStandbyEnable);
  Buffer[1] = PcdGet8(PcdUsb4PlatformOSCControl);

  DEBUG ((DEBUG_INFO, "  MSDB = %d\n", Buffer[0]));
  DEBUG ((DEBUG_INFO, "  U4PC = 0x%X\n", Buffer[1]));

  Status = CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                (VOID*)CpmTableProtocolPtr,
                                                &gEfiCallerIdGuid,
                                                &AcpiPlatformOscTableId,
                                                AcpiPlatformOscCallBack,
                                                Buffer
                                                );
  if (!EFI_ERROR (Status) && (Event != NULL)) gBS->CloseEvent (Event);
  DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-End\n", __FUNCTION__, __LINE__));
  return;
}

EFI_STATUS
EFIAPI
AmdCpmPlatformOscTableInstallEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-Start\n", __FUNCTION__, __LINE__));

  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_CALLBACK,
                  AmdCpmPlatformOscSsdtInstall,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-Create ReadyToBootEvent : %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "FEA-DXE-%a(%d)-End\n", __FUNCTION__, __LINE__));
  return EFI_SUCCESS;
}
