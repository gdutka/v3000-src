/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>

EFI_GUID HID_WIRELESS_BUTTON_SSDT_GUID = {
  0x421C6346, 0x80E8, 0x458E, 0xB7, 0x97, 0x92, 0x63, 0x7D, 0xCA, 0x41, 0xA5
};

VOID
EFIAPI
AmdCpmHidWirelessButtonSsdtInstall (
  IN      EFI_EVENT           Event,
  IN      VOID                *Context
  );

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdCpmWirelessButtonHidDxeEntry
 *
 *  Description:
 *     Entry point for the HID Wireless Botton driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         PMODULE_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AmdCpmWirelessButtonHidDxeEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->CreateEventEx (
                              CPM_EVENT_NOTIFY_SIGNAL,
                              CPM_TPL_NOTIFY,
                              AmdCpmHidWirelessButtonSsdtInstall,
                              NULL,
                              &gEfiEventReadyToBootGuid,
                              &ReadyToBootEvent
                              );

  return Status;
}

BOOLEAN
EFIAPI
AmdCpmHidWirelessButtonSsdtCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32                  *BufferPtr;

  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
    case (CPM_SIGNATURE_32 ('E', 'C', '0', '_')):
      if (*BufferPtr) {
        *(UINT32*) AmlObjPtr = *(UINT32*)BufferPtr;
      }
      break;

    case (CPM_SIGNATURE_32 ('L', 'P', 'C', '0')):
      if (*(BufferPtr + 1)) {
        *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 2);
      }
      break;

    case (CPM_SIGNATURE_32 ('X', 'Q', '2', '8')):
      if (*(BufferPtr + 2)) {
        *(UINT8*)AmlObjPtr = '_';
        *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(BufferPtr + 2);
      }
      break;
  }
  return FALSE;
}

VOID
EFIAPI
AmdCpmHidWirelessButtonSsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
/*++

Routine Description:

  Invoke AmdinitLate entry point. This function gets called
  each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

Arguments & Return Values: Standard event handling function prototype

--*/
{
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  AMD_CPM_WIRELESS_BUTTON_TABLE   *WirelessBottonTablePtr;
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  UINT32                          Buffer[3];

  if (!InitlateInvoked) {
    Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                    NULL,
                                    (VOID**)&CpmTableProtocolPtr
                                    );
    if (EFI_ERROR (Status)) {
      return;
    }

    MainTablePtr = CpmTableProtocolPtr->MainTablePtr;
    WirelessBottonTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_WIRELESS_BUTTON);
    if ((MainTablePtr->WirelessButtonEn == 3) && WirelessBottonTablePtr) {
      ZeroMem (Buffer, sizeof (Buffer));
      Buffer[0] = CFG_AMD_CPM_ASL_EC_NAME;
      Buffer[1] = CFG_AMD_CPM_ASL_LPC_NAME;
      Buffer[2] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (WirelessBottonTablePtr->HidNotifyQEvent);
      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                      CpmTableProtocolPtr,
                                                      &HID_WIRELESS_BUTTON_SSDT_GUID,
                                                      NULL,
                                                      AmdCpmHidWirelessButtonSsdtCallBack,
                                                      &Buffer[0]
                                                      );
    }
  }

  InitlateInvoked = TRUE;
  return;
}
