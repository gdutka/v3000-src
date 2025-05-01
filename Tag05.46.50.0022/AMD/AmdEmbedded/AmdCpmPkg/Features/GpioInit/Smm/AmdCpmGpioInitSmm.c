/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmSmm.h>

CPM_DEFINE_gBS;
CPM_DEFINE_gSmst;

AMD_CPM_TABLE_PROTOCOL                  *gCpmTableSmmProtocol;


EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AmdCpmGpioInitS3RestoreCallback,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdCpmGpioInitSmmSyncSmmCpmTable (
  IN       VOID                                    *This,
  IN       AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_ID      SyncId,
  IN       AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_FLOW    SyncFlow
  )
{
  AMD_CPM_PCIE_CLOCK_TABLE            *ClonedPcieClockTablePtr;
  AMD_CPM_PCIE_CLOCK_TABLE            *SmmPcieClockTablePtr;
  AMD_CPM_PCIE_CLOCK_TABLE            *RuntimePcieClockTablePtr;
  
  AMD_CPM_PCIE_CLOCK_ITEM             *SmmTablePtr;
  AMD_CPM_PCIE_CLOCK_ITEM             *ClonedTablePtr;
  AMD_CPM_PCIE_CLOCK_ITEM             *RuntimeTablePtr;

  switch (SyncFlow) {

  case AMD_CPM_SYNC_SMM_CPM_TABLE_CHECK_RUNTIME:
    //
    // #SMM.3.5: Boot firmware SMM module MUST copy the communication buffer to SMRAM before the check,
    // to resist TOC/TOU or DMA attacks.
    // So need to copy Runtime table to a Temp SMM CPM table before the check and then sync SMM table.
    //

    //
    // TODO: Need to according to program to check ClkReq / ClkReqExt valid value and ClkId order should be same
    // 
    break;

  case AMD_CPM_SYNC_SMM_CPM_TABLE_FROM_RUNTIME:
    ClonedPcieClockTablePtr = gCpmTableSmmProtocol->SmmPublicFunction.GetClonedRuntimeTablePtr (
                                                                        gCpmTableSmmProtocol, 
                                                                        CPM_SIGNATURE_PCIE_CLOCK
                                                                        );
    SmmPcieClockTablePtr = gCpmTableSmmProtocol->CommonFunction.GetTablePtr (
                                                                  gCpmTableSmmProtocol, 
                                                                  CPM_SIGNATURE_PCIE_CLOCK
                                                                  );
    if (ClonedPcieClockTablePtr != NULL && SmmPcieClockTablePtr != NULL) {
      ClonedTablePtr = &ClonedPcieClockTablePtr->Item[0];
      SmmTablePtr    = &SmmPcieClockTablePtr->Item[0];

      while (SmmTablePtr->ClkId != CPM_END_OF_TABLE) {
        if (SmmTablePtr->ClkId == ClonedTablePtr->ClkId) {
          SmmTablePtr->ClkReq = ClonedTablePtr->ClkReq;
          SmmTablePtr->ClkReqExt = ClonedTablePtr->ClkReqExt;
        }
        SmmTablePtr++;
        ClonedTablePtr++;
      }
    }
    break;

  case AMD_CPM_SYNC_SMM_CPM_TABLE_TO_RUNTIME:
    RuntimePcieClockTablePtr = gCpmTableSmmProtocol->SmmPublicFunction.GetRuntimeTablePtr (
                                                                         gCpmTableSmmProtocol, 
                                                                         CPM_SIGNATURE_PCIE_CLOCK
                                                                         );
    SmmPcieClockTablePtr = gCpmTableSmmProtocol->CommonFunction.GetTablePtr (
                                                                  gCpmTableSmmProtocol, 
                                                                  CPM_SIGNATURE_PCIE_CLOCK
                                                                  );
    if (RuntimePcieClockTablePtr != NULL && SmmPcieClockTablePtr != NULL) {
      RuntimeTablePtr = &RuntimePcieClockTablePtr->Item[0];
      SmmTablePtr    = &SmmPcieClockTablePtr->Item[0];

      while (SmmTablePtr->ClkId != CPM_END_OF_TABLE) {
        if (SmmTablePtr->ClkId == RuntimeTablePtr->ClkId) {
          RuntimeTablePtr->ClkReq = SmmTablePtr->ClkReq;
          RuntimeTablePtr->ClkReqExt = SmmTablePtr->ClkReqExt;
        }
        RuntimeTablePtr++;
        SmmTablePtr++;
      }
    }

    break;
  }
  
  return EFI_SUCCESS;
}



/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM GPIO Init SMM driver
 *
 * This function registers the SMI handlers to set internal PCIe clock when resume
 * from S3.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
AmdCpmGpioInitSmmEntryPoint (
  IN      EFI_HANDLE                  ImageHandle,
  IN      EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                          Status;
  CPM_SMM_SW_DISPATCH_PROTOCOL        *SwDispatch;
  CPM_SMM_SW_DISPATCH_CONTEXT         SwContext;
  EFI_HANDLE                          SwHandle;

  If_CpmInSmm (ImageHandle, SystemTable, gBS, gSmst, Status) {
    //
    // Initialize global variables
    //
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
    Status = gSmst->SmmLocateProtocol (
                    &gAmdCpmTableSmmProtocolGuid,
                    NULL,
                    (VOID**)&gCpmTableSmmProtocol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-1-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    gCpmTableSmmProtocol->CommonFunction.PostCode (CpmTpGpioInitSmmDriverBegin);

    DEBUG((DEBUG_INFO, "FEA-SMM-%a-ModernStandbyEn: %d\n",
      __FUNCTION__, gCpmTableSmmProtocol->MainTablePtr->ModernStandbyEn));

    if (gCpmTableSmmProtocol->MainTablePtr->ModernStandbyEn != 0) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-Unsupport S3 resume\n", __FUNCTION__));
      gCpmTableSmmProtocol->CommonFunction.PostCode (CpmTpGpioInitSmmDriverEnd);
      return EFI_SUCCESS;
    }
    
    //
    //  Locate SMM SW dispatch protocol
    //
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-CpmSmmLocateProtocol-gCpmSmmSwDispatchProtocolGuid\n", __FUNCTION__));
    Status = CpmSmmLocateProtocol (
                    &gCpmSmmSwDispatchProtocolGuid,
                    NULL,
                    (VOID**)&SwDispatch
                    );
    ASSERT_EFI_ERROR (Status);

    SwContext.SwSmiInputValue = CFG_CPM_ACPI_RESTORE_SW_SMI;
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-Register-AmdCpmGpioInitS3RestoreCallback\n", __FUNCTION__));
    Status = SwDispatch->Register (
                    SwDispatch,
                    AmdCpmGpioInitS3RestoreCallback,
                    &SwContext,
                    &SwHandle
                    );

    gCpmTableSmmProtocol->CommonFunction.PostCode (CpmTpGpioInitSmmDriverEnd);

    Status = gCpmTableSmmProtocol->SmmPublicFunction.RegisterSyncFunction (
                                                       gCpmTableSmmProtocol,
                                                       AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_GPIO_INIT_S3_RESOTRE_CALLBACK,
                                                       AmdCpmGpioInitSmmSyncSmmCpmTable
                                                       );
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  RECORD_TIME_DXE (EndAmdCpmGpioInitSmmDriver);
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End-2-Status=%r\n", __FUNCTION__, Status));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIE Clock Init
 *
 * This function sets internal PCIe clock.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */

VOID
AmdCpmSmmPcieClockInit (
  IN      AMD_CPM_TABLE_PROTOCOL      *CpmTableProtocolPtr
  )
{
  AMD_CPM_PCIE_CLOCK_TABLE            *CpmPcieClockTablePtr;
  AMD_CPM_PCIE_CLOCK_ITEM             *TablePtr;
  UINT8                               SlotStatus;
  BOOLEAN                             ClockType;
  AMD_CPM_EXPRESS_CARD_TABLE          *ExpressCardTablePtr;
  AMD_CPM_OTHER_HOTPLUG_CARD_TABLE    *OtherHotplugTablePtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE        *ZeroPowerOddTablePtr;
  AMD_CPM_SAVE_CONTEXT_TABLE          *SaveContextTablePtr;
  UINT8                               i;
  UINT32                              PcieSlotStatus;
  UINT32                              PcieSlotStatusHigh;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  i = 0;
  PcieSlotStatus = 0;
  PcieSlotStatusHigh = 0;

  CpmPcieClockTablePtr                = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_CLOCK);
  SaveContextTablePtr                 = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  ClockType                           = (BOOLEAN) CpmTableProtocolPtr->CommonFunction.GetStrap (CpmTableProtocolPtr) & BIT1;

  if (SaveContextTablePtr && ClockType && CpmPcieClockTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (SaveContextTablePtr+ClockType+CpmPcieClockTablePtr+UnusedGppClkOffEn)\n", __FUNCTION__));
    PcieSlotStatusHigh = (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x07);
    PcieSlotStatusHigh = (PcieSlotStatusHigh << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x06);
    PcieSlotStatusHigh = (PcieSlotStatusHigh << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x05);
    PcieSlotStatusHigh = (PcieSlotStatusHigh << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x04);
    PcieSlotStatus = (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x03);
    PcieSlotStatus = (PcieSlotStatus << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x02);
    PcieSlotStatus = (PcieSlotStatus << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x01);
    PcieSlotStatus = (PcieSlotStatus << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x00);
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-1-PcieSlotStatus=%016X\n", __FUNCTION__, PcieSlotStatus));
    TablePtr = &CpmPcieClockTablePtr->Item[0];
    DEBUG((DEBUG_INFO, "\n==============================================================================\n"));
    while (TablePtr->ClkId != CPM_END_OF_TABLE) {
      SlotStatus = 0;
      if (TablePtr->SlotCheck & BIT1) {
        SlotStatus = CpmTableProtocolPtr->CommonFunction.DetectDevice (CpmTableProtocolPtr, TablePtr->DeviceId, NULL) ? BIT1 : 0;
      }
      if (TablePtr->SlotCheck & (BIT3 + BIT2 + BIT0)) {
        if (i < 8) {
          SlotStatus |=  (UINT8) ((PcieSlotStatus >> (4 * i)) & 0xF);
        } else {
          SlotStatus |=  (UINT8) ((PcieSlotStatusHigh >> (4 * (i - 8))) & 0xF);
        }
      }
      DEBUG((DEBUG_INFO, "  Item[%X] Socket=%02X Die=%02X Bridge=%02X Miscid=%02X", i, \
            (TablePtr->SpecialFunctionId >> 24) & 0xF, (TablePtr->SpecialFunctionId >> 20) & 0xF, \
            (TablePtr->SpecialFunctionId >> 16) & 0xF, (TablePtr->SpecialFunctionId >> 28) & 0xF));
      DEBUG((DEBUG_INFO, " ClkId=%02X ClkReq=%02X ClkIdExt=%02X ClkReqExt=%02X DeviceId=%02X", \
            TablePtr->ClkId, TablePtr->ClkReq, TablePtr->ClkIdExt, TablePtr->ClkReqExt, TablePtr->DeviceId));
      DEBUG((DEBUG_INFO, " Device=%02X  Function=%02X  SlotCheck=%02X  SpecialFunctionId=%X  SlotStatus=%02X\n", \
            TablePtr->Device, TablePtr->Function, TablePtr->SlotCheck, (TablePtr->SpecialFunctionId & 0xFFFF), SlotStatus));
      if ((SlotStatus > 0) && (((SlotStatus & 0xFE) & TablePtr->SlotCheck) == 0)) {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-SlotStatus > 0 & ((SlotStatus & 0xFE) & TablePtr->SlotCheck) = 0\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-Set ClkReq & ClkReqExt = 0xFF\n", __FUNCTION__));
        TablePtr->ClkReq = 0xFF;
        TablePtr->ClkReqExt = 0xFF;
      }
      if ((SlotStatus == 0) && ((TablePtr->SlotCheck & (BIT1 + BIT0)) > 0) && CpmTableProtocolPtr->MainTablePtr->UnusedGppClkOffEn) {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-SlotStatus=0 & (SlotCheck&(BIT1+BIT0))>0 & UnusedGppClkOffEn=1\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-Set ClkReq & ClkReqExt = 0\n", __FUNCTION__));
        TablePtr->ClkReq = 0;
        TablePtr->ClkReqExt = 0;
      }
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-GetClkReq\n", __FUNCTION__));
      if (CpmTableProtocolPtr->CommonFunction.GetClkReq (CpmTableProtocolPtr, (UINT8) ((TablePtr->SpecialFunctionId >> 24) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 20) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 16) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 28) & 0xF), TablePtr->ClkId) == 1) {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-L1SS is enabled, don't do SetClkReq\n", __FUNCTION__));
      } else {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetClkReq\n", __FUNCTION__));
        CpmTableProtocolPtr->CommonFunction.SetClkReq (CpmTableProtocolPtr, (UINT8) ((TablePtr->SpecialFunctionId >> 24) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 20) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 16) & 0xF), (UINT8) ((TablePtr->SpecialFunctionId >> 28) & 0xF), TablePtr->ClkId, TablePtr->ClkReq);
      }
      TablePtr++;
      i++;
    }
    DEBUG((DEBUG_INFO, "\n==============================================================================\n"));
  }
  ExpressCardTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
  if (ExpressCardTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (ExpressCardTablePtr)\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSciTrig    Pin = %X    Value = %X\n", __FUNCTION__, ExpressCardTablePtr->EventPin, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin) ? 0 : 1));
    CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ExpressCardTablePtr->EventPin,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin) ? 0 : 1
                                                );
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSci    Pin = %X\n", __FUNCTION__, ExpressCardTablePtr->EventPin));
    CpmTableProtocolPtr->CommonFunction.SetGeventSci (
                                                CpmTableProtocolPtr,
                                                ExpressCardTablePtr->EventPin
                                                );
  }
  OtherHotplugTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_OTHER_HOTPLUG_CARD);
  if (OtherHotplugTablePtr) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (OtherHotplugTablePtr)\n", __FUNCTION__));
    if ((OtherHotplugTablePtr->Number > 0) && (OtherHotplugTablePtr->EventSource0 == 0)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (Number > 0) & (OtherHotplugTablePtr->EventSource0 == 0)\n", __FUNCTION__));
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSciTrig    Pin = %X    Value = %X\n", __FUNCTION__, OtherHotplugTablePtr->EventPin0, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin0) ? 0 : 1));
      CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                OtherHotplugTablePtr->EventPin0,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin0) ? 0 : 1
                                                );
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSci    Pin = %X\n", __FUNCTION__, OtherHotplugTablePtr->EventPin0));
      CpmTableProtocolPtr->CommonFunction.SetGeventSci (
                                                CpmTableProtocolPtr,
                                                OtherHotplugTablePtr->EventPin0
                                                );
    }
    if ((OtherHotplugTablePtr->Number > 1) && (OtherHotplugTablePtr->EventSource1 == 0)) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (Number > 1) & (OtherHotplugTablePtr->EventSource1 == 0)\n", __FUNCTION__));
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSciTrig    Pin = %X    Value = %X\n", __FUNCTION__, OtherHotplugTablePtr->EventPin1, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin1) ? 0 : 1));
      CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                OtherHotplugTablePtr->EventPin1,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, OtherHotplugTablePtr->EventPin1) ? 0 : 1
                                                );
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSci    Pin = %X\n", __FUNCTION__, OtherHotplugTablePtr->EventPin1));
      CpmTableProtocolPtr->CommonFunction.SetGeventSci (
                                                CpmTableProtocolPtr,
                                                OtherHotplugTablePtr->EventPin1
                                                );
    }
  }
  if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & (BIT0 | BIT1)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (ZeroPowerOddEn & (BIT0 | BIT1))\n", __FUNCTION__));
    ZeroPowerOddTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    if (ZeroPowerOddTablePtr) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (ZeroPowerOddTablePtr)\n", __FUNCTION__));
      if ((CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT0) && (ZeroPowerOddTablePtr->EventSource1 == 0)) {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-if ((ZeroPowerOddEn & BIT0) + (EventSource1 == 0))\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSciTrig    Pin = %X    Value = %X\n", __FUNCTION__, ZeroPowerOddTablePtr->EventPin1, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin1) ? 0 : 1));
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin1,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin1) ? 0 : 1
                                                );
      }
      if ((CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT1) && (ZeroPowerOddTablePtr->EventSource2 == 0)) {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-if ((ZeroPowerOddEn & BIT1) + (EventSource2 == 0))\n", __FUNCTION__));
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGeventSciTrig    Pin = %X    Value = %X\n", __FUNCTION__, ZeroPowerOddTablePtr->EventPin2, CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin2) ? 0 : 1));
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin2,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin2) ? 0 : 1
                                                );
      }
    }
  }
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The Function to init GEVENT pins
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */

VOID
AmdCpmSmmGeventInit (
  IN      AMD_CPM_TABLE_PROTOCOL      *CpmTableProtocolPtr
  )
{
  AMD_CPM_GEVENT_INIT_TABLE       *GeventTablePtr;
  AMD_CPM_GEVENT_ITEM             *TablePtr;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  if (CpmTableProtocolPtr) {
    GeventTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GEVENT_INIT);

    if (GeventTablePtr) {
      DEBUG((DEBUG_INFO, "FEA-SMM-%a-if (GeventTablePtr)\n", __FUNCTION__));
      TablePtr = &GeventTablePtr->GeventList[0];
      while (TablePtr->Pin != CPM_END_OF_TABLE) {
        DEBUG((DEBUG_INFO, "FEA-SMM-%a-SetGevent    Pin = %X    Value=%X\n", __FUNCTION__, TablePtr->Pin, TablePtr->Setting.Raw));
        CpmTableProtocolPtr->CommonFunction.SetGevent (CpmTableProtocolPtr, TablePtr->Pin, TablePtr->Setting.Raw);
        TablePtr++;
      }
    }
  }
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * SMI Handler to set internal PCIe clock
 *
 * This function restores internal PCIe clock when resume from S3.
 *
 */

EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AmdCpmGpioInitS3RestoreCallback,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  EFI_STATUS                         Status;
  AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_ID SyncId;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  SyncId =   AMD_CPM_SYNC_SMM_CPM_TABLE_SYNC_GPIO_INIT_S3_RESOTRE_CALLBACK;
  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableFromRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-AmdCpmSmmPcieClockInit\n", __FUNCTION__));
  AmdCpmSmmPcieClockInit (gCpmTableSmmProtocol);
  DEBUG((DEBUG_INFO, "FEA-SMM-%a-AmdCpmSmmGeventInit\n", __FUNCTION__));
  AmdCpmSmmGeventInit (gCpmTableSmmProtocol);

  Status = gCpmTableSmmProtocol->SmmPublicFunction.SyncSmmCpmTableToRuntime (
                                                     gCpmTableSmmProtocol,
                                                     SyncId
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "FEA-SMM-%a-EFI_ERROR\n", __FUNCTION__));
    return Status;
  }

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
