/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/MpmFuncLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/PcdLib.h>


#define FILECODE LIBRARY_MPMFUNCLIB_MPMFUNCLIB_FILECODE

/**
  Locate Mpm PrivateData, the data is stored in AGESA HEAP
  * @param[in,out]  Data  Point to the MPM global prviate data

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
LocateMpmPrivateData (
  MPM_PRVIATE_HEAP_DATA_V1 **Data
  )
{
  EFI_STATUS            EfiStatus;
  AGESA_STATUS          AStatus;
  LOCATE_HEAP_PTR       LocateHeapParams;

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  LocateHeapParams.BufferHandle = AMD_MANAGEABILITY_CFG_HANDLE;
  LocateHeapParams.BufferPtr = NULL;
  AStatus = HeapLocateBuffer (&LocateHeapParams, NULL);
  EfiStatus = (AStatus == AGESA_SUCCESS) ? EFI_SUCCESS : EFI_OUT_OF_RESOURCES;
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("HEAP locate Fail\n");
    return EfiStatus;
  }
  *Data =  (MPM_PRVIATE_HEAP_DATA_V1 *) LocateHeapParams.BufferPtr;
  return EFI_SUCCESS;
}

/**
  Dump Mpm PrivateData, the data is stored in AGESA HEAP
  * @param[in,out]  Data  Point to the MPM global prviate data

   @retval EFI_STATUS              0: Success, NonZero Error
**/
VOID
DumpMpmPrivateData (
  MPM_PRVIATE_HEAP_DATA_V1 *Data
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("MPM_PRVIATE_HEAP_DATA_V1\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t Version %x\n", Data->Version);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t WirelessTxrPending %x\n", Data->WirelessTxrPending);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t WirelessKvmPending %x\n", Data->WirelessKvmPending);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t ManageabilityConfig %x\n", Data->ManageabilityConfig);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t MpmDramAddr %x\n", Data->MpmDramAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t PldmBufferAddr %x\n", Data->PldmBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t PldmBufferOffset %x\n", Data->PldmBufferOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t PldmBufferSize %x\n", Data->PldmBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t AlertBufferAddr %x\n", Data->AlertBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t AlertBufferOffset %x\n", Data->AlertBufferOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t AlertBufferSize %x\n", Data->AlertBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextBufferAddr %x\n", Data->TextBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextBufferOffset %x\n", Data->TextBufferOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextBufferSize %x\n", Data->TextBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmBufferAddr %x\n", Data->KvmBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmBufferOffset %x\n", Data->KvmBufferOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmBufferSize %x\n", Data->KvmBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextTxBufferAddr %x\n", Data->TextTxBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextTxBufferSize %x\n", Data->TextTxBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextRxBufferAddr %x\n", Data->TextRxBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TextRxBufferSize %x\n", Data->TextRxBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t CmnBufferAddr %x\n", Data->CmnBufferAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t CmnBufferOffset %x\n", Data->CmnBufferOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t CmnBufferSize %x\n", Data->CmnBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TcrDidVid %x\n", Data->TcrDidVid);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TcrBufOffset %x\n", Data->TcrBufOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TcrBufSize %x\n", Data->TcrBufSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t TcrBufAddr %x\n", Data->TcrBufAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmDidVid %x\n", Data->KvmDidVid);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmKeyboardBufOffset %x\n", Data->KvmKeyboardBufOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmKeyboardBufSize %x\n", Data->KvmKeyboardBufSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmKeyboardBufAddr %x\n", Data->KvmKeyboardBufAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmMouseBufOffset %x\n", Data->KvmMouseBufOffset);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmMouseBufSize %x\n", Data->KvmMouseBufSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t KvmMouseBufAddr %x\n", Data->KvmMouseBufAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t WirelessDmaBufAddr %x\n", Data->WirelessDmaBufAddr);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t WirelessKvmFlag %x\n", Data->WirelessKvmFlag);
}

/**
  Initial Mpm PrivateData, the data is stored in AGESA HEAP
  * @param[in,out]  Data  Point to the MPM global prviate data

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
InitMpmPrivateData (
  MPM_PRVIATE_HEAP_DATA_V1 **Data
  )
{
  EFI_STATUS            EfiStatus;
  AGESA_STATUS          AStatus;
  MANAGEABILITY_CFG     Cfg;
  ALLOCATE_HEAP_PARAMS  AllocateHeapParams;
  MPM_PRVIATE_HEAP_DATA_V1 *MpmData;
  BIOS_MPM_MBOX_REG0_FIELD PendingTask;
  MBOX_QUERY_TEXT    QueryTxt;
  MBOX_QUERY_KVM     QueryKvm;

  IDS_HDT_CONSOLE_PSP_TRACE ("InitMpmPrivateData\n");

  AllocateHeapParams.RequestedBufferSize = sizeof (MPM_PRVIATE_HEAP_DATA_V1);
  AllocateHeapParams.BufferHandle = AMD_MANAGEABILITY_CFG_HANDLE;
  AllocateHeapParams.Persist = HEAP_RUNTIME_SYSTEM_MEM;
  AllocateHeapParams.BufferPtr = NULL;
  AStatus = HeapAllocateBuffer (&AllocateHeapParams, NULL);
  EfiStatus = (AStatus == AGESA_SUCCESS) ? EFI_SUCCESS : EFI_OUT_OF_RESOURCES;
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("HEAP Allocation Fail\n");
    goto Exit;
  }
  //Fill the initial data, all value will be set to 0xFFFFFFFF
  MpmData = (MPM_PRVIATE_HEAP_DATA_V1 *) AllocateHeapParams.BufferPtr;
  SetMem (MpmData, sizeof (MPM_PRVIATE_HEAP_DATA_V1), 0xFF);
  MpmData->Version = 1;
  //Message to PSP to get MPM related config
  EfiStatus = PspMboxManageabilityCfg (&Cfg);
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PspMboxManageabilityCfg Fail\n");
    goto Exit;
  }
  MpmData->ManageabilityConfig = Cfg.ManageabilityConfig;
  MpmData->MpmDramAddr = (((UINT64)Cfg.MpmDramAddrHi) << 32) + Cfg.MpmDramAddrLo;
  //If fuse disabled turn off the MPM switch
  if ((Cfg.ManageabilityConfig & (BIOS2PSP_MPM_AIM_T_MASK || BIOS2PSP_MPM_WM_FUSE_ENABLE_MASK)) != (BIOS2PSP_MPM_AIM_T_MASK || BIOS2PSP_MPM_WM_FUSE_ENABLE_MASK)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("MPM fuse disabled\n");
    //If Init fail, disable MPM feature.
    PcdSetBoolS (PcdMpmEnable, FALSE);
    //Deallocate heap to make the data invalid
    HeapDeallocateBuffer (AMD_MANAGEABILITY_CFG_HANDLE, NULL);
    goto Exit;
  }
  //Message to PSP to get Pending tasks and CmnBufer
  EfiStatus = BiosMpmCmdInit (&PendingTask, &MpmData->CmnBufferOffset, &MpmData->CmnBufferSize);
  if (EFI_ERROR (EfiStatus) || ((MpmData->CmnBufferOffset + MpmData->CmnBufferSize) > MPM_DRAM_TOTAL_SIZE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdInit Fail\n");
    //If Init fail, disable MPM feature.
    PcdSetBoolS (PcdMpmEnable, FALSE);
    //Deallocate heap to make the data invalid
    HeapDeallocateBuffer (AMD_MANAGEABILITY_CFG_HANDLE, NULL);
    goto Exit;
  }
  //Convert format
  MpmData->WirelessTxrPending = PendingTask.WirelessTxrPending ? 1: 0;
  MpmData->WirelessKvmPending = PendingTask.WirelessKvmPending ? 1: 0;
  MpmData->WirelessKvmFlag = PendingTask.WirelessKvmPending ? 1: 0; //this flag will be set to 0 if there is wired kvm request
  MpmData->CmnBufferAddr = MpmData->MpmDramAddr + MpmData->CmnBufferOffset;
  //Message to PSP to get command buffer address
  EfiStatus = BiosMpmCmdGetPldmBufferOffsetSize (&MpmData->PldmBufferOffset, &MpmData->PldmBufferSize);
  if (EFI_ERROR (EfiStatus) || ((MpmData->PldmBufferOffset + MpmData->PldmBufferSize) > MPM_DRAM_TOTAL_SIZE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdGetPldmBufferOffsetSize Fail\n");
    goto Exit;
  }
  MpmData->PldmBufferAddr = MpmData->MpmDramAddr + MpmData->PldmBufferOffset;

  EfiStatus = BiosMpmCmdGetAlertBufferOffsetSize (&MpmData->AlertBufferOffset, &MpmData->AlertBufferSize);
  if (EFI_ERROR (EfiStatus) || ((MpmData->AlertBufferOffset + MpmData->AlertBufferSize) > MPM_DRAM_TOTAL_SIZE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdGetAlertBufferOffsetSize Fail\n");
    goto Exit;
  }
  MpmData->AlertBufferAddr = MpmData->MpmDramAddr + MpmData->AlertBufferOffset;

  //Get Text Redirection Tx/Rx Buffer if WirelessTxrPending
  if ((MpmData->WirelessTxrPending) && (PcdGetBool (PcdMpmWirelessTcrEnable) == TRUE)) {
    EfiStatus = BiosMpmCmdGetTextBufferOffsetSize (&MpmData->TextBufferOffset , &MpmData->TextBufferSize);
    if (EFI_ERROR (EfiStatus) || ((MpmData->TextBufferOffset + MpmData->TextBufferSize) > MPM_DRAM_TOTAL_SIZE)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdGetTextBufferOffsetSize Fail\n");
      goto Exit;
    }
    MpmData->TextBufferAddr = MpmData->MpmDramAddr + MpmData->TextBufferOffset;
    //Send query command to get more information. e.g. VID, DID, RingBuf
    ZeroMem (&QueryTxt, sizeof (MBOX_QUERY_TEXT));
    BiosMpmCmdQueryText (&QueryTxt);
    //todo add assertion back
    // ASSERT (QueryTxt.DidVid != 0);
    // ASSERT (QueryTxt.TcrBufSize != 0);
    MpmData->TcrDidVid = QueryTxt.DidVid;
    MpmData->TcrBufOffset = QueryTxt.TcrBufOffset;
    MpmData->TcrBufSize = QueryTxt.TcrBufSize;
    MpmData->TcrBufAddr = QueryTxt.TcrBufOffset + MpmData->MpmDramAddr;
    MpmData->TextRxBufferAddr = MpmData->TcrBufAddr + OFFSET_OF (TCR_RING_BUFFER_T, RxTcrRingBuf);
    MpmData->TextRxBufferSize = MPM_TEXT_MAX_RING_DATA_SIZE;
    MpmData->TextTxBufferAddr = MpmData->TcrBufAddr + OFFSET_OF (TCR_RING_BUFFER_T, TxTcrRingBuf);
    MpmData->TextTxBufferSize = MPM_TEXT_MAX_RING_DATA_SIZE;
    if (((MpmData->TextRxBufferAddr + MpmData->TextRxBufferSize) > (MpmData->MpmDramAddr + MPM_DRAM_TOTAL_SIZE)) || \
      (MpmData->TextTxBufferAddr + MpmData->TextTxBufferSize) > (MpmData->MpmDramAddr + MPM_DRAM_TOTAL_SIZE)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("TextBuffer is larger than the MPM Dram total size!\n");
      goto Exit;
    }
  }

  EfiStatus = BiosMpmCmdGetKvmBufferOffsetSize (&MpmData->KvmBufferOffset, &MpmData->KvmBufferSize);
  if (EFI_ERROR (EfiStatus) || ((MpmData->KvmBufferOffset + MpmData->KvmBufferSize) > MPM_DRAM_TOTAL_SIZE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdGetKvmBufferOffsetSize Fail\n");
    goto Exit;
  }
  MpmData->KvmBufferAddr = MpmData->MpmDramAddr + MpmData->KvmBufferOffset;
  //If KVM pending bit is set, Send query command to get more information. e.g. VID, DID, RingBuf
  //Note, KVM pending bit only imply wireless KVM is pending
  if (MpmData->WirelessKvmPending && (PcdGetBool (PcdMpmWirelessKvmEnable) == TRUE)) {
    ZeroMem (&QueryKvm, sizeof (MBOX_QUERY_KVM));
    BiosMpmCmdQueryKvm (&QueryKvm);
    //todo add assertion back
    // ASSERT (QueryKvm.DidVid != 0);
    // ASSERT (QueryKvm.KeyboardBufSize != 0);
    // ASSERT (QueryKvm.MouseBufSize != 0);
    MpmData->KvmDidVid = QueryKvm.DidVid;
    MpmData->KvmKeyboardBufOffset = QueryKvm.KeyboardBufOffset;
    MpmData->KvmKeyboardBufSize = QueryKvm.KeyboardBufSize;
    MpmData->KvmKeyboardBufAddr = MpmData->MpmDramAddr + MpmData->KvmKeyboardBufOffset;

    MpmData->KvmMouseBufOffset = QueryKvm.MouseBufOffset;
    MpmData->KvmMouseBufSize = QueryKvm.MouseBufSize;
    MpmData->KvmMouseBufAddr = MpmData->MpmDramAddr + MpmData->KvmMouseBufOffset;
    if (((MpmData->KvmKeyboardBufOffset + MpmData->KvmKeyboardBufSize) > MPM_DRAM_TOTAL_SIZE) || \
      (MpmData->KvmMouseBufOffset + MpmData->KvmMouseBufSize) > MPM_DRAM_TOTAL_SIZE) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Kvm Keyboard or Mouse Buffer is larger than the MPM Dram total size!\n");
      goto Exit;
    }
  }

Exit:
  IDS_HDT_CONSOLE_PSP_TRACE ("InitMpmPrivateData Exit: %x\n", EfiStatus);
  if (EfiStatus == EFI_SUCCESS) {
    if (Data != NULL) {
      *Data = MpmData;
    }
    DumpMpmPrivateData (MpmData);
  }
  return EfiStatus;
}
