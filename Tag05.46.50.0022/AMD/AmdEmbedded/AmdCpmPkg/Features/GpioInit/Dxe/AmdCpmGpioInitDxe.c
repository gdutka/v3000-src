/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <Protocol/FchInitDonePolicyProtocol.h>

VOID
EFIAPI
AmdCpmGpioInitMid (
  IN      VOID             *Context
  );

VOID
EFIAPI
AmdCpmGpioInitMidNotifyEvent (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  );

VOID
EFIAPI
AmdCpmGpioInitLate (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  );

VOID
AmdCpmDxePcieClockInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  );

VOID
AmdCpmDxeGeventInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  );

VOID
AmdCpmGeventSmiControlInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM GPIO Init DXE driver
 *
 * This function sets GEVENT pins and internal PCIe clock.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmGpioInitDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;
  AMD_CPM_TABLE_PROTOCOL                  *CpmTableProtocolPtr;
  EFI_EVENT                               AfterInitEnvEvent;
  VOID                                    *mAfterInitEnvEvent;
  VOID                                    *DummyProtocol = NULL;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpGpioInitDxeDriverBegin);

  //
  // First test gFchInitDonePolicyProtocolGuid is installed or not.
  //
  Status = gBS->LocateProtocol (
                  &gFchInitDonePolicyProtocolGuid,
                  NULL,
                  &DummyProtocol
                  );
  if (EFI_ERROR (Status)) {
    //
    // If gFchInitDonePolicyProtocolGuid still not be installed, RegisterProtocolNotify
    //
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-CreateEvent-AfterInitEnvEvent-For-AmdCpmGpioInitMidNotifyEvent\n", __FUNCTION__));
    Status = gBS->CreateEvent (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmGpioInitMidNotifyEvent,
                    (VOID*)CpmTableProtocolPtr,
                    &AfterInitEnvEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-2-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    DEBUG((DEBUG_INFO, "FEA-DXE-%a-RegisterProtocolNotify-gFchInitDonePolicyProtocolGuid-For-AfterInitEnvEvent\n", __FUNCTION__));
    Status = gBS->RegisterProtocolNotify (
                    &gFchInitDonePolicyProtocolGuid,
                    AfterInitEnvEvent,
                    &mAfterInitEnvEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-3-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }
  } else {
    //
    // Already have gFchInitDonePolicyProtocolGuid, invoke AmdCpmGpioInitMid()
    //
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-AmdCpmGpioInitMid-2\n", __FUNCTION__));
    AmdCpmGpioInitMid (CpmTableProtocolPtr);
    AmdCpmGpioInitMid (CpmTableProtocolPtr);
  }

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-CreateEventEx-gEfiEventReadyToBootGuid-For-AmdCpmGpioInitLate\n", __FUNCTION__));
  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_NOTIFY,
                  AmdCpmGpioInitLate,
                  (VOID*)CpmTableProtocolPtr,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpGpioInitDxeDriverEnd);

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-4-Status=%r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init internal PCIe clock.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmGpioInitLate (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  )
{
  STATIC BOOLEAN  AmdCpmGpioInitLateInvoked = FALSE;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-From-AmdCpmGpioInitDxeEntryPoint-CreateEventEx-gEfiEventReadyToBootGuid\n"));
  RECORD_TIME_DXE (BeginAmdCpmGpioInitDxeDriver);

  if (!AmdCpmGpioInitLateInvoked) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-AmdCpmGeventSmiControlInit\n", __FUNCTION__));
    AmdCpmGeventSmiControlInit (Context);
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-AmdCpmDxePcieClockInit\n", __FUNCTION__));
    AmdCpmDxePcieClockInit (Context);
  }

  AmdCpmGpioInitLateInvoked = TRUE;
  RECORD_TIME_DXE (EndAmdCpmGpioInitDxeDriver);
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GEVENT pins
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmGpioInitMid (
  IN      VOID             *Context
  )
{
  STATIC BOOLEAN  AmdCpmGpioInitMidInvoked = FALSE;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start-AmdCpmGpioInitMidInvoked = %x\n", __FUNCTION__, AmdCpmGpioInitMidInvoked ? 1 : 0));
  RECORD_TIME_DXE (BeginAmdCpmGpioInitDxeDriver);

  if (!AmdCpmGpioInitMidInvoked) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-AmdCpmDxeGeventInit\n", __FUNCTION__));
    AmdCpmDxeGeventInit (Context);
  }

  AmdCpmGpioInitMidInvoked = TRUE;
  RECORD_TIME_DXE (EndAmdCpmGpioInitDxeDriver);
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GEVENT pins
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmGpioInitMidNotifyEvent (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  )
{
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-AmdCpmGpioInitMid\n", __FUNCTION__));
  AmdCpmGpioInitMid (Context);
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
}

VOID
AmdCpmDxeAddOneItemInClkList (
  IN      AMD_CPM_EXT_CLKGEN_TABLE    *ExtClkGenTablePtr,
  IN      AMD_CPM_EXT_CLKGEN_ITEM     *ListPtr,
  IN      UINT8                       *ListNum,
  IN      UINT8                       ClkNum,
  IN      UINT8                       ClkReq
  )
{
  AMD_CPM_EXT_CLKGEN_ITEM     *CurrentItemPtr;
  UINT8                       Index;
  UINT8                       Index2;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  if (ClkNum == 0xFE) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-1-ClkNum == 0xFE\n", __FUNCTION__));
    return;
  }
  for (Index = 0; Index < AMD_EXT_CLKGEN_SIZE; Index++) {
    if (ExtClkGenTablePtr->Item[Index].Function == CPM_END_OF_TABLE) {
      break;
    }
    if (ExtClkGenTablePtr->Item[Index].Function == ClkNum + 0x80 && ClkReq == 0) {
      CurrentItemPtr = &ExtClkGenTablePtr->Item[Index];
    } else if (ExtClkGenTablePtr->Item[Index].Function == ClkReq + 0x8F && ClkReq != 0xFF && ClkReq != 0) {
      CurrentItemPtr = &ExtClkGenTablePtr->Item[Index];
    } else {
      continue;
    }
    for (Index2 = 0; Index2 < (*ListNum); Index2++) {
      if (ListPtr[Index2].Offset == CurrentItemPtr->Offset) {
        ListPtr[Index2].AndMask &= CurrentItemPtr->AndMask;
        ListPtr[Index2].OrMask |= CurrentItemPtr->OrMask;
        break;
      }
    }
    if (Index2 == (*ListNum)) {
      ListPtr[Index2].Function = 0;
      ListPtr[Index2].Offset = CurrentItemPtr->Offset;
      ListPtr[Index2].AndMask = CurrentItemPtr->AndMask;
      ListPtr[Index2].OrMask = CurrentItemPtr->OrMask;
      (*ListNum) ++;
    }
  }
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-2\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to initialize internal PCIe clock.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */
VOID
AmdCpmDxePcieClockInit (
  IN      AMD_CPM_TABLE_PROTOCOL      *CpmTableProtocolPtr
  )
{
  AMD_CPM_PCIE_CLOCK_TABLE            *CpmPcieClockTablePtr;
  AMD_CPM_PCIE_CLOCK_ITEM             *TablePtr;
  UINT8                               SlotStatus;
  BOOLEAN                             ClockType;
  AMD_CPM_EXT_CLKGEN_TABLE            *ExtClkGenTablePtr;
  AMD_CPM_EXT_CLKGEN_ITEM             ClkGen[AMD_PCIE_CLOCK_SIZE];
  AMD_CPM_SAVE_CONTEXT_TABLE          *SaveContextTablePtr;
  UINT8                               Index;
  UINT8                               Data;
  UINT8                               i;
  UINT32                              PcieSlotStatus;
  UINT32                              PcieSlotStatusHigh;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  ExtClkGenTablePtr = NULL;
  Index = 0;
  PcieSlotStatus = 0;
  PcieSlotStatusHigh = 0;
  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpGpioInitDxePcieClockInitBegin);
  CpmPcieClockTablePtr  = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_CLOCK);
  SaveContextTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  ClockType             = (BOOLEAN) CpmTableProtocolPtr->CommonFunction.GetStrap (CpmTableProtocolPtr) & BIT1;
  if (!ClockType) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (!ClockType)\n", __FUNCTION__));
    ExtClkGenTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_EXT_CLKGEN);
    if (!ExtClkGenTablePtr) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-1-if (!ExtClkGenTablePtr)\n", __FUNCTION__));
      return;
    }
  }
  if (CpmPcieClockTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (CpmPcieClockTablePtr)\n", __FUNCTION__));
    TablePtr = &CpmPcieClockTablePtr->Item[0];
    i = 0;
    DEBUG((DEBUG_INFO, "\n==============================================================================\n"));
    while (TablePtr->ClkId != CPM_END_OF_TABLE) {
      SlotStatus = 0;
      if (TablePtr->SlotCheck & BIT1) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SlotCheck BIT[1] = 1-DetectDevice\n", __FUNCTION__));
        SlotStatus = CpmTableProtocolPtr->CommonFunction.DetectDevice (CpmTableProtocolPtr, TablePtr->DeviceId, NULL) ? BIT1 : 0;
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SlotStatus = 0x%02X\n", __FUNCTION__, SlotStatus));
      }
      if (TablePtr->SlotCheck & (BIT3 + BIT2 + BIT0)) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SlotCheck BIT[3,2,0] = 1-CheckPcieDevice\n", __FUNCTION__));
        SlotStatus |= CpmTableProtocolPtr->CommonFunction.CheckPcieDevice (CpmTableProtocolPtr, (UINT8) ((TablePtr->SpecialFunctionId >> 24) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 20) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 16) & 0xF), TablePtr->Device, TablePtr->Function);
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SlotStatus = 0x%02X\n", __FUNCTION__, SlotStatus));
      }
      DEBUG((DEBUG_INFO, "  Item[%X] Socket=%02X Die=%02X Bridge=%02X Miscid=%02X", i, \
            (TablePtr->SpecialFunctionId >> 24) & 0xF, (TablePtr->SpecialFunctionId >> 20) & 0xF, \
            (TablePtr->SpecialFunctionId >> 16) & 0xF, (TablePtr->SpecialFunctionId >> 28) & 0xF));
      DEBUG((DEBUG_INFO, " ClkId=%02X ClkReq=%02X ClkIdExt=%02X ClkReqExt=%02X DeviceId=%02X", \
            TablePtr->ClkId, TablePtr->ClkReq, TablePtr->ClkIdExt, TablePtr->ClkReqExt, TablePtr->DeviceId));
      DEBUG((DEBUG_INFO, " Device=%02X  Function=%02X  SlotCheck=%02X  SpecialFunctionId=%X  SlotStatus=%02X\n", \
            TablePtr->Device, TablePtr->Function, TablePtr->SlotCheck, (TablePtr->SpecialFunctionId & 0xFFFF), SlotStatus));
      if ((SlotStatus > 0) && (((SlotStatus & 0xFE) & TablePtr->SlotCheck) == 0)) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SlotStatus > 0 & ((SlotStatus & 0xFE) & TablePtr->SlotCheck) = 0\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-Set ClkReq & ClkReqExt = 0xFF\n", __FUNCTION__));
        TablePtr->ClkReq = 0xFF;
        TablePtr->ClkReqExt = 0xFF;
      }
      if ((SlotStatus == 0) && ((TablePtr->SlotCheck & (BIT1 + BIT0)) > 0) && CpmTableProtocolPtr->MainTablePtr->UnusedGppClkOffEn) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SlotStatus=0 & (SlotCheck&(BIT1+BIT0))>0 & UnusedGppClkOffEn=1\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-Set ClkReq & ClkReqExt = 0\n", __FUNCTION__));
        TablePtr->ClkReq = 0;
        TablePtr->ClkReqExt = 0;
      }
      if (i < 8) {
        PcieSlotStatus |= SlotStatus << (4 * i);
      } else {
        PcieSlotStatusHigh |= SlotStatus << (4 * (i - 8));
      }
      if (ClockType) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (ClockType)-GetClkReq\n", __FUNCTION__));
        if (CpmTableProtocolPtr->CommonFunction.GetClkReq (CpmTableProtocolPtr, (UINT8) ((TablePtr->SpecialFunctionId >> 24) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 20) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 16) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 28) & 0xF), TablePtr->ClkId) == 1) {
          DEBUG((DEBUG_INFO, "FEA-DXE-%a-L1SS is enabled, don't do SetClkReq\n", __FUNCTION__));
        } else {
          DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetClkReq\n", __FUNCTION__));
          CpmTableProtocolPtr->CommonFunction.SetClkReq (CpmTableProtocolPtr, (UINT8) ((TablePtr->SpecialFunctionId >> 24) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 20) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 16) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 28) & 0xF), TablePtr->ClkId, TablePtr->ClkReq);
        }
      } else {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (!ClockType)-AmdCpmDxeAddOneItemInClkList\n", __FUNCTION__));
        AmdCpmDxeAddOneItemInClkList (ExtClkGenTablePtr, &ClkGen[0], &Index, TablePtr->ClkIdExt, TablePtr->ClkReqExt);
      }
      TablePtr++;
      i++;
    }
    DEBUG((DEBUG_INFO, "\n==============================================================================\n"));

    if (SaveContextTablePtr) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (SaveContextTablePtr)\n", __FUNCTION__));
      if (CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, BootMode)) == CPM_BOOT_MODE_S0) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-BootMode = CPM_BOOT_MODE_S0\n", __FUNCTION__));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x00, (UINT8) (PcieSlotStatus));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x01, (UINT8) (PcieSlotStatus >> 8));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x02, (UINT8) (PcieSlotStatus >> 16));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x03, (UINT8) (PcieSlotStatus >> 24));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x04, (UINT8) (PcieSlotStatusHigh));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x05, (UINT8) (PcieSlotStatusHigh >> 8));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x06, (UINT8) (PcieSlotStatusHigh >> 16));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x07, (UINT8) (PcieSlotStatusHigh >> 24));
      }
    }
    if (!ClockType) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (!ClockType)\n", __FUNCTION__));
      i = 0;
      while (Index > 0) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-ReadSmbus\n", __FUNCTION__));
        CpmTableProtocolPtr->CommonFunction.ReadSmbus ( CpmTableProtocolPtr,
                                                        ExtClkGenTablePtr->SmbusSelect,
                                                        ExtClkGenTablePtr->SmbusAddress,
                                                        ClkGen[i].Offset,
                                                        1,
                                                        &Data);
        Data = (Data & ClkGen[i].AndMask) | ClkGen[i].OrMask;
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-WriteSmbus\n", __FUNCTION__));
        CpmTableProtocolPtr->CommonFunction.WriteSmbus (CpmTableProtocolPtr,
                                                        ExtClkGenTablePtr->SmbusSelect,
                                                        ExtClkGenTablePtr->SmbusAddress,
                                                        ClkGen[i].Offset,
                                                        1,
                                                        &Data);
        Index --;
        i ++;
      }
    }
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpGpioInitDxePcieClockInitEnd);

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End-2\n", __FUNCTION__));
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to initialize GEVENT pins.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */
VOID
AmdCpmDxeGeventInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  )
{
  AMD_CPM_GEVENT_INIT_TABLE       *GeventTablePtr;
  AMD_CPM_GEVENT_ITEM             *TablePtr;
  AMD_CPM_EXPRESS_CARD_TABLE      *ExpressCardTablePtr;
  AMD_CPM_OTHER_HOTPLUG_CARD_TABLE  *OtherHotplugTablePtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE    *ZeroPowerOddTablePtr;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  GeventTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GEVENT_INIT);
  if (GeventTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (GeventTablePtr)\n", __FUNCTION__));
    TablePtr = &GeventTablePtr->GeventList[0];
    while (TablePtr->Pin != CPM_END_OF_TABLE) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetGevent    Pin = %X    Value=%X\n", __FUNCTION__, TablePtr->Pin, TablePtr->Setting.Raw));
      CpmTableProtocolPtr->CommonFunction.SetGevent (CpmTableProtocolPtr, TablePtr->Pin, TablePtr->Setting.Raw);
      TablePtr++;
    }
  }

  ExpressCardTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
  if (ExpressCardTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (ExpressCardTablePtr)\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetGeventSciTrig    Pin = %X    Value=%X\n", __FUNCTION__, ExpressCardTablePtr->EventPin, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin) ? 0 : 1));
    CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ExpressCardTablePtr->EventPin,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin) ? 0 : 1
                                                );
  }

  OtherHotplugTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_OTHER_HOTPLUG_CARD);
  if (OtherHotplugTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (OtherHotplugTablePtr)\n", __FUNCTION__));
    if ((OtherHotplugTablePtr->Number > 0) && (OtherHotplugTablePtr->EventSource0 == 0)) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (Number > 0) & (OtherHotplugTablePtr->EventSource0 == 0)\n", __FUNCTION__));
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetGeventSciTrig    Pin = %X    Value=%X\n", __FUNCTION__, OtherHotplugTablePtr->EventPin0, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin0) ? 0 : 1));
      CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                OtherHotplugTablePtr->EventPin0,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin0) ? 0 : 1
                                                );
    }
    if ((OtherHotplugTablePtr->Number > 1) && (OtherHotplugTablePtr->EventSource1 == 0)) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (Number > 1) & (OtherHotplugTablePtr->EventSource1 == 0)\n", __FUNCTION__));
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetGeventSciTrig    Pin = %X    Value=%X\n", __FUNCTION__, OtherHotplugTablePtr->EventPin1, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin1) ? 0 : 1));
      CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                OtherHotplugTablePtr->EventPin1,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin1) ? 0 : 1
                                                );
    }
  }

  if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & (BIT0 | BIT1)) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (ZeroPowerOddEn & (BIT0 | BIT1))\n", __FUNCTION__));
    ZeroPowerOddTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    if (ZeroPowerOddTablePtr) {
      DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (ZeroPowerOddTablePtr)\n", __FUNCTION__));
      if ((CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT0) && (ZeroPowerOddTablePtr->EventSource1 == 0)) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-if ((ZeroPowerOddEn & BIT0) + (EventSource1 == 0))\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetGeventSciTrig    Pin = %X    Value=%X\n", __FUNCTION__, ZeroPowerOddTablePtr->EventPin1, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin1) ? 0 : 1));
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin1,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin1) ? 0 : 1
                                                );
      }
      if ((CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT1) && (ZeroPowerOddTablePtr->EventSource2 == 0)) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-if ((ZeroPowerOddEn & BIT1) + (EventSource2 == 0))\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetGeventSciTrig    Pin = %X    Value=%X\n", __FUNCTION__, ZeroPowerOddTablePtr->EventPin2, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin2) ? 0 : 1));
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin2,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin2) ? 0 : 1
                                                );
      }
    }
  }
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to initialize SMI Control for GEVENT pins.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */
VOID
AmdCpmGeventSmiControlInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  )
{
  AMD_CPM_GEVENT_INIT_TABLE       *GeventTablePtr;
  AMD_CPM_GEVENT_ITEM             *TablePtr;

  DEBUG((DEBUG_INFO, "FEA-DXE-%a-Start\n", __FUNCTION__));
  GeventTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GEVENT_INIT);
  if (GeventTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-DXE-%a-if (GeventTablePtr)\n", __FUNCTION__));
    TablePtr = &GeventTablePtr->GeventList[0];
    while (TablePtr->Pin != CPM_END_OF_TABLE) {
      if (TablePtr->Pin < 32) {
        DEBUG((DEBUG_INFO, "FEA-DXE-%a-SetSmiControl    Pin = %X    Value=%X\n", __FUNCTION__, (UINT8) TablePtr->Pin, TablePtr->Setting.Raw));
        CpmTableProtocolPtr->CommonFunction.SetSmiControl (CpmTableProtocolPtr, (UINT8) TablePtr->Pin, TablePtr->Setting.Raw);
      }
      TablePtr++;
    }
  }
  DEBUG((DEBUG_INFO, "FEA-DXE-%a-End\n", __FUNCTION__));
}
