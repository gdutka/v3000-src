/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <Library/FchSpiAccessLib.h>
#include <Library/BaseLib.h>

EFI_GUID PMF_DEVICE_GUID = {
  0x662E317E, 0x1E42, 0x49ED, 0x81, 0x9D, 0x97, 0x04, 0x08, 0xBA, 0xEF, 0x72
};

VOID
EFIAPI
AmdCpmPmfLateEntry (
  IN      EFI_EVENT           Event,
  IN      VOID                *Context
  );

/**
 *---------------------------------------------------------------------------------------
 *
 *  CpmDxeDriverEntryPoint
 *
 *  Description:
 *     Entry point for the cpu driver.
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
AmdCpmPmfDxeEntry (
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
                              AmdCpmPmfLateEntry,
                              NULL,
                              &gEfiEventReadyToBootGuid,
                              &ReadyToBootEvent
                              );

  return Status;
}

BOOLEAN
EFIAPI
AmdCpmPmfSsdtCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  AMD_CPM_TABLE_PROTOCOL  *TableProtocolPtr;
  UINT32                  *BufferPtr;

  TableProtocolPtr = This;
  BufferPtr = (UINT32*)Context;

  switch ( *((UINT32*)AmlObjPtr)) {
    case (CPM_SIGNATURE_32 ('P', 'F', '0', 'A')): // Name (PF0A, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr) + sizeof (AMD_CPM_TABLE_COMMON_HEADER);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', '3', 'A')):  // Name (PF3A, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 1);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', '5', 'A')): // Name (PF5A, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 2);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', '6', 'A')): // Name (PF6A, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 3);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', '9', 'A')): // Name (PF9A, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 4);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', 'B', 'A')): // Name (PFBA, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 5);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', 'C', 'A')): // Name (PFCA, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 6);
      }
      break;

    case (CPM_SIGNATURE_32 ('P', 'F', 'D', 'A')): // Name (PFDA, 0x55AA55AA)
      if (*(UINT8*)((UINT8*)AmlObjPtr - 1) == 0x08) {
        *(UINT32 *)((UINT8*)AmlObjPtr + 5) = *(BufferPtr + 7);
      }
      break;

    case (CPM_SIGNATURE_32 ('O', 'P', 'M', '4')):
      if (*(BufferPtr + 8)) {
        *((UINT32*)AmlObjPtr) = *(BufferPtr + 8);
      }
      break;

    case (CPM_SIGNATURE_32 ('O', 'P', 'M', '7')):
      if (*(BufferPtr + 9)) {
        *((UINT32*)AmlObjPtr) = *(BufferPtr + 9);
      }
      break;

    case (CPM_SIGNATURE_32 ('O', 'P', 'M', '8')):
      if (*(BufferPtr + 10)) {
        *((UINT32*)AmlObjPtr) = *(BufferPtr + 10);
      }
      break;

    case (CPM_SIGNATURE_32 ('O', 'P', 'M', 'A')):
      if (*(BufferPtr + 11)) {
        *((UINT32*)AmlObjPtr) = *(BufferPtr + 11);
      }
      break;

    case (CPM_SIGNATURE_32 ('O', 'P', 'M', 'E')):
      if (*(BufferPtr + 12)) {
        *((UINT32*)AmlObjPtr) = *(BufferPtr + 12);
      }
      break;

    case (CPM_SIGNATURE_32 ('X', 'C', 'R', 'S')):
      if (*(BufferPtr + 13)) {
        *(UINT32 *)((UINT8*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'C', 'R', 'S');
      }
      break;

    case 0xAAAAAAAA:
      if (*(BufferPtr + 13)) {
        *(UINT32 *)((UINT8*)AmlObjPtr) = *(BufferPtr + 13);
      }
      break;
  }
  return FALSE;
}

VOID
EFIAPI
AmdCpmPmfLateEntry (
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
  AMD_CPM_PMF_TABLE               *AmdPmfTablePtr;
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  UINT32                          Buffer[20];
  EFI_PHYSICAL_ADDRESS            StaticBinReservedMemAddr;
  BOOLEAN                         StaticBinAvailable = FALSE;

  if (!InitlateInvoked) {
    Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                    NULL,
                                    (VOID**)&CpmTableProtocolPtr
                                    );
    if (EFI_ERROR (Status)) {
      return;
    }

    if ((PcdGet32(PcdCpmPmfStaticPolicyBinRomOffset) != 0xFFFFFFFF) && (PcdGet32(PcdCpmPmfStaticPolicySize)!= 0xFFFFFFFF)) {
      StaticBinReservedMemAddr = 0xFFFFFFFF; //Below 4G
      Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES (300 * 1024),
                    &StaticBinReservedMemAddr
                    );
      if (!EFI_ERROR(Status)) {
        StaticBinAvailable = TRUE;
        FchSpiRomRead(PcdGet32(PcdCpmPmfStaticPolicyBinRomOffset), (UINT8 *)StaticBinReservedMemAddr, (PcdGet32(PcdCpmPmfStaticPolicySize) < (300*1024)) ? PcdGet32(PcdCpmPmfStaticPolicySize) : (300*1024));
      }
    }

    AmdPmfTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PMF);
    if (AmdPmfTablePtr) {
      ZeroMem (Buffer, sizeof (Buffer));
      Buffer[0]  = (UINT32) (UINT64) AmdPmfTablePtr;
      Buffer[1]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn3Settings;
      Buffer[2]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn5Settings;
      Buffer[3]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn6Settings;
      Buffer[4]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn9Settings;
      Buffer[5]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn11Settings;
      Buffer[6]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn12Settings;
      Buffer[7]  = (UINT32) (UINT64) &AmdPmfTablePtr->ApmfFn13Settings;
      Buffer[8]  = AmdPmfTablePtr->NotifySbiosHeartbeatAcpiMethodObjName;
      Buffer[9]  = AmdPmfTablePtr->SetFanTableIndexAcpiMethodObjName;
      Buffer[10] = AmdPmfTablePtr->SliderEventNotificationAcpiMethodObjName;
      Buffer[11] = AmdPmfTablePtr->NotifyInBagPolicyUpdateAcpiMethodObjName;
      Buffer[12] = AmdPmfTablePtr->NotifySmartPcSolutoinUpdateAcpiMethodObjName;
      Buffer[13] = (StaticBinAvailable) ? (UINT32)(StaticBinReservedMemAddr & 0xFFFFFFFF) : 0;
      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                      CpmTableProtocolPtr,
                                                      &PMF_DEVICE_GUID,
                                                      NULL,
                                                      AmdCpmPmfSsdtCallBack,
                                                      &Buffer[0]
                                                      );
    }
  }

  InitlateInvoked = TRUE;
  return;
}
