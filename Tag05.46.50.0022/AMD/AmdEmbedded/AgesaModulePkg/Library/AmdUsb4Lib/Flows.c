/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/RingDescriptor.h>
#include <AmdUsb4/HostInterface.h>
#include <AmdUsb4/ControlPacket.h>
#include <Library/AmdUsb4Lib.h>

#define FILECODE LIBRARY_AMDUSB4LIB_FLOWS_FILECODE


/*----------------------------------------------------------------------------------------*/
/**
 * Transmit Flow Raw Mode
 *
 * @param[in] *pCM             Point to global structure AMD_USB4_CM
 * @param[in] RingNumber       USB4 controller has N rings. This parameter define Ring Number
 * @param[in] Pdf              Route String
 * @param[in] DataSize         Number of bytes
 * @param[in] *Data            Pointer to the Data
 * @param[in] InterruptEnable  Enable or disable Interrupt
 *
 * @retval EFI_SUCCESS           Success to send out the data.
 * @retval EFI_OUT_OF_RESOURCES  Transmit Ring is always full.
 * @retval EFI_DEVICE_ERROR -    Host Interface do not send out the data in expected time.
**/

EFI_STATUS
AmdUsb4HostInterfaceTxFlowRawMode (
  IN     AMD_USB4_CM  *pCM,
  IN     UINT32       RingNumber,
  IN     UINT32       Pdf,
  IN     UINT32       DataSize,
  IN     UINT32       *Data,            // Control Packet Payload, do not include header
  IN     BOOLEAN      InterruptEnable
  )
{
  EFI_STATUS                              Status;
  AMD_USB4_DESCRIPTOR                     *pTxDesc;
  volatile AMD_USB4_TX_DESCRIPTOR_RING    *pRingTx;
  UINT8                                   TimeOut;
  UINT32                                  RingSize;
  UINT32                                  *TxDataBuffer;
  UINT64                                  TxDataBufferBase;
  UINT32                                  DescriptorOffset;
  UINT16                                  ProducerIndex;
  UINT16                                  ConsumerIndex;
  UINT32                                  BufferIndex;

  pTxDesc = NULL;
  pRingTx = NULL;
  TxDataBufferBase = 0xFFFFFFFF;
  TimeOut = 0;
  RingSize = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Enter\n", __FUNCTION__);
  //
  // 1. Get Host Interface MMIO from pCM->MMIO
  //
  pRingTx = (AMD_USB4_TX_DESCRIPTOR_RING*)(UINTN)(pCM->HIMmioBase + TX_BASE_ADDRESS_LOW (RingNumber));
  //
  // 2. Check if Tx Ring full
  // read Consumer Index and Producer Index
  // wait until (Producer Index + 1) Mod RingSize < Consumer Index
  //
  TimeOut = DESCRIPTOR_TIMEOUT_US/10;
  RingSize = pRingTx->RingSize;
  Status = EFI_TIMEOUT;
  do {
    ProducerIndex = pRingTx->DescIndex.Consumer_Producer_Index.Producer_Index;
    ConsumerIndex = pRingTx->DescIndex.Consumer_Producer_Index.Consumer_Index;
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Check Ring full. Producer_Index=0x%x, Consumer_Index=0x%x\n",
      ProducerIndex,
      ConsumerIndex
      );
    if ( ( ( ProducerIndex + 1 ) % RingSize) != ConsumerIndex ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Transmit Ring is not full.\n");
      Status = EFI_SUCCESS;
      break;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("Wait Transmit Ring available. TimeOut %d\n", TimeOut);
    MicroSecondDelay (10);  // TODO - Decide the minimum delay time
  } while (--TimeOut);

  if (Status == EFI_TIMEOUT) {
    IDS_HDT_CONSOLE_USB4_ERROR ("%a Index Timeout. Transmit Ring is always full.\n", __FUNCTION__);
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // 3. Get current Tx descriptor (pTxDesc) pointed by Producer Index
  // Tx Ring Base = Ring[N] Base Address
  // Get current Producer Index = pCM->TxProducerIndex[N]
  //
  DescriptorOffset = (UINT32)(sizeof (AMD_USB4_DESCRIPTOR) * ProducerIndex);
  pTxDesc = (AMD_USB4_DESCRIPTOR *)(UINTN)(pCM->RingTxBase[RingNumber] + DescriptorOffset);
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("pTxDesc = 0x%lx\n", pTxDesc);
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("pTxDesc = 0x%x\n", pTxDesc);
  }
  //
  // 4. Update Tx Descriptor fields
  // pTxDesc->EofPdf = Pdf
  //
  pTxDesc->DescriptorData.TxDesc.EOFPDF = Pdf;
  //
  // pTxDesc->InterruptEnable = InterruptEnable
  //
  pTxDesc->DescriptorData.TxDesc.InterruptEnable = InterruptEnable;
  //
  // pTxDesc->RequestStatus = 1
  //
  pTxDesc->DescriptorData.TxDesc.RequestStatus = 1;
  //
  // pTxDesc->DataLenth = DataSize
  //
  pTxDesc->DescriptorData.TxDesc.DataLength = DataSize;

  pTxDesc->DescriptorData.TxDesc.DescriptorDone = 0;
  //
  // 5. Copy Data to data buffer pointed by Tx Descriptor
  //
  TxDataBufferBase  = (UINT64)(MAX_CONTROL_PACKET_SIZE * ProducerIndex);
  TxDataBufferBase += pCM->RingTxDataBufferBase[RingNumber];
  TxDataBufferBase += pTxDesc->DescriptorData.TxDesc.Offset;
  TxDataBuffer = (UINT32 *)(UINTN)TxDataBufferBase;
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("TxDataBuffer = 0x%lx\n", TxDataBuffer);
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("TxDataBuffer = 0x%x\n", TxDataBuffer);
  }

#if AMD_USB4_DEBUG_CLEAR_RING_BUFFER
  ZeroMem (TxDataBuffer, MAX_CONTROL_PACKET_SIZE);
#endif

  //
  // ByteSwap for TX data buffer
  //
  for (BufferIndex = 0; BufferIndex < DataSize / sizeof (UINT32); BufferIndex++) {
    TxDataBuffer[BufferIndex] = SwapBytes32 (Data[BufferIndex]);
  }

  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Memory dump for TX. ProducerIndex=%d, pTxDesc at 0x%lx, TxDataBuffer at 0x%lx.\n",
      ProducerIndex,
      pTxDesc,
      TxDataBuffer
      );
    Usb4DumpData (TxDataBuffer, DataSize);
  }

  //
  // 6. Increase Producer Index both in H/I Ring[N] and local copy in pCM
  //
  ConsumerIndex = pRingTx->DescIndex.Consumer_Producer_Index.Consumer_Index;
  pCM->RingTxProducerIndex[RingNumber] = ( ProducerIndex + 1 ) % ( pRingTx->RingSize );
  pRingTx->DescIndex.Consumer_Producer_Index.Producer_Index = pCM->RingTxProducerIndex[RingNumber];
  //
  // 7. Poll the Consumer Index, when changed,
  //    scan the Transmit Descriptors in host memory starting from the Consumer Index location.
  //    For each Transmit Descriptor with the Descriptor Done bit set to 1b,
  //    either free or recycle the buffer as appropriate.
  //
  TimeOut = DESCRIPTOR_TIMEOUT_US/10;
  Status = EFI_TIMEOUT;
  do {
    if ( pTxDesc->DescriptorData.TxDesc.DescriptorDone ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Host Interface has consumed the data\n");
      Status = EFI_SUCCESS;
      break;
    }
    MicroSecondDelay (10);
    IDS_HDT_CONSOLE_USB4_DEBUG ("Wait Consumer Index change and Descriptor Done = 1, timeout %d\n", TimeOut);
  } while (--TimeOut);

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "%a Old Consumer Index=0x%x, New Consumer Index=0x%x, New Producer_Index=0x%x, Descriptor Done = 0x%x\n",
    __FUNCTION__,
    ConsumerIndex,
    pRingTx->DescIndex.Consumer_Producer_Index.Consumer_Index,
    pRingTx->DescIndex.Consumer_Producer_Index.Producer_Index,
    pTxDesc->DescriptorData.TxDesc.DescriptorDone
    );

  if (Status == EFI_TIMEOUT)
  {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "%a DescriptorDone Timeout. Old Consumer Index=0x%x, New Consumer Index=0x%x, Descriptor Done = 0x%x\n",
      __FUNCTION__,
      ConsumerIndex,
      pRingTx->DescIndex.Consumer_Producer_Index.Consumer_Index,
      pTxDesc->DescriptorData.TxDesc.DescriptorDone
      );
    Status = EFI_DEVICE_ERROR;
  }
  //
  // 8. Free (Zero) Data buffer
  //
#if !AMD_USB4_DEBUG_CLEAR_RING_BUFFER
  ZeroMem (TxDataBuffer, DataSize);
#endif

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Success\n", __FUNCTION__);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Receive Flow Raw Mode
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  RingNumber       USB4 controller has N rings. This parameter define Ring Number
 *
 * @param[out] *Pdf             Return the PDF in the control packet
 * @param[out] *DataSize        Return the number of bytes
 * @param[out] *Data            Return the pointer of the data.
 * @param[out] *RemainPacket    Return the remain packet in Receive Ring.
 *
 * @retval EFI_SUCCESS          Success to retrieve the data.
 * @retval EFI_NOT_FOUND        Receive Ring is empty.
 * @retval EFI_DEVICE_ERROR -   Receive Ring is not empty, but packet of
 *                              Descriptor Done bit is not 1 or buffer OverFlow or CRC error.
**/

EFI_STATUS
AmdUsb4HostInterfaceRxFlowRawMode (
  IN     AMD_USB4_CM*  pCM,
  IN     UINT32        RingNumber,
     OUT UINT32        *Pdf,
     OUT UINT32        *DataSize,
     OUT UINT32        *Data,             // Control Packet Payload, do not include header
     OUT UINT32        *RemainPacket
  )
{
  EFI_STATUS                              Status;
  AMD_USB4_DESCRIPTOR                     *pRxDesc;       // Receive Descriptor (descriptor Header file)
  volatile AMD_USB4_RX_DESCRIPTOR_RING*   pRingRx;        // Receive Ring Structure (HI chapter)
  UINT8                                   TimeOut;
  UINT32                                  RingSize;
  UINT32                                  *RxDataBuffer;
  UINT64                                  RxDataBufferBase;
  UINT32                                  DescriptorOffset;
  UINT16                                  ProducerIndex;
  UINT16                                  ConsumerIndex;
  UINT32                                  BufferIndex;

  pRxDesc = NULL;
  pRingRx = NULL;
  TimeOut = 0;
  RingSize = 0;
  RxDataBufferBase = 0xFFFFFFFF;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Entry.\n", __FUNCTION__);
  //
  // 1. Get Host Interface MMIO from pCM->MMIO
  //
  pRingRx = (AMD_USB4_RX_DESCRIPTOR_RING*)(UINTN)(pCM->HIMmioBase + RX_BASE_ADDRESS_LOW (RingNumber));
  //
  // 2. Check incoming Packet
  //
  RingSize = pRingRx->RingSize;
  ConsumerIndex = pRingRx->DescIndex.Consumer_Producer_Index.Consumer_Index;
  ProducerIndex = pRingRx->DescIndex.Consumer_Producer_Index.Producer_Index;
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Receive Ring. ConsumerIndex = 0x%x, ProducerIndex = 0x%x.\n",
    ConsumerIndex,
    ProducerIndex
    );

  if ( ProducerIndex == pCM->RingRxProducerIndex[RingNumber] ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "There is !!!NO!!! control packet in Receive Ring"
      " #%d. register ProducerIndex = 0x%x, local copy ProducerIndex = 0x%x.\n",
      RingNumber,
      ProducerIndex,
      pCM->RingRxProducerIndex[RingNumber]
      );
    return EFI_NOT_FOUND;
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "There's control packets in Receive Ring #%d. register ProducerIndex = 0x%x, local copy ProducerIndex = 0x%x.\n",
      RingNumber,
      ProducerIndex,
      pCM->RingRxProducerIndex[RingNumber]
      );
  }

  //
  // 3. pRxDesc point to pCM->RingRxProducerIndex[N] + 1
  //
  DescriptorOffset = (UINT32)(sizeof (AMD_USB4_DESCRIPTOR) * (pCM->RingRxProducerIndex[RingNumber]));
  pRxDesc = (AMD_USB4_DESCRIPTOR *)(UINTN)(pCM->RingRxBase[RingNumber] + DescriptorOffset);
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("pRxDesc = 0x%lx\n", pRxDesc);
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("pRxDesc = 0x%x\n", pRxDesc);
  }
  //
  // 4. Poll pRxDesc->DescriptorDoneGet == 1
  //
  TimeOut = DESCRIPTOR_TIMEOUT_US/10;
  Status = EFI_TIMEOUT;

  do {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Receive Ring. pRxDesc->DescriptorData.RxHostAdaptLayerDesc.DescriptorDone = 0x%x.\n",
      pRxDesc->DescriptorData.RxHostAdaptLayerDesc.DescriptorDone
      );
    if (pRxDesc->DescriptorData.RxHostAdaptLayerDesc.DescriptorDone) {
      Status = EFI_SUCCESS;
      break;
    }
    MicroSecondDelay (10);  // TODO - Decide the minimum delay time
  } while (--TimeOut);

  if (Status == EFI_TIMEOUT) {
    IDS_HDT_CONSOLE_USB4_ERROR ("%a Descriptor Done Timeout.\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  //
  // 5. Check if BufferOverFlow or CRC error
  //
  if (pRxDesc->DescriptorData.RxHostAdaptLayerDesc.BufferOverflow ||
    pRxDesc->DescriptorData.RxHostAdaptLayerDesc.CRCError ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("%a BufferOverFlow/CRCError.\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  //
  // 6. Copy *Pdf = pRxDesc->EofPdf
  //
  *Pdf = pRxDesc->DescriptorData.RxHostAdaptLayerDesc.EOFPDF;
  //
  // 7. Copy *DataSize = pRxDesc->DataLength
  //
  *DataSize = pRxDesc->DescriptorData.RxHostAdaptLayerDesc.DataLength;
  //
  // 8. Copy Data to *Data
  //
  RxDataBufferBase  = (UINT64)(MAX_CONTROL_PACKET_SIZE * (pCM->RingRxProducerIndex[RingNumber]));
  RxDataBufferBase += pCM->RingRxDataBufferBase[RingNumber];
  RxDataBufferBase += pRxDesc->DescriptorData.RxHostAdaptLayerDesc.Offset;
  RxDataBuffer = (UINT32 *)(UINTN)RxDataBufferBase;
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("RxDataBuffer = 0x%lx\n", RxDataBuffer);
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("RxDataBuffer = 0x%x\n", RxDataBuffer);
  }

  //
  // Byte swap for RX data buffer
  //
  for (BufferIndex = 0; BufferIndex < ((*DataSize) / sizeof (UINT32)); BufferIndex++) {
    Data[BufferIndex] = SwapBytes32 (RxDataBuffer[BufferIndex]);
  }

  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    Usb4DumpData (RxDataBuffer, *DataSize);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "After byte swap dump. Data at 0x%lx\n",
      Data
      );
    Usb4DumpData (Data, *DataSize);
  }

  //
  // 9. Restore Rx Descriptor (pRxDesc) to default
  //
  SetMem ((void *)&(pRxDesc->DescriptorData), sizeof (AMD_USB4_DESCRIPTOR_DATA), 0);
  pRxDesc->DescriptorData.RxHostDesc.InterruptEnable  = 0;
  pRxDesc->DescriptorData.RxHostDesc.DescriptorDone   = 0;
  pRxDesc->DescriptorData.RxHostDesc.RequestStatus    = 1;
  pRxDesc->DescriptorData.RxHostDesc.Offset           = 0;

  // If assert here, means HW modify the address.
  if ( ((((UINT64)pRxDesc->AddressHigh)<<32) | (UINT64)pRxDesc->AddressLow) !=
        ( pCM->RingDevRxDataBufferBase[RingNumber] + ((pCM->RingRxProducerIndex[RingNumber])*MAX_CONTROL_PACKET_SIZE) ) )
  {
    IDS_HDT_CONSOLE_USB4_ERROR ("HW modify the Receive Ring Descriptor address\n");
    IDS_DEADLOOP ();
  }

  //
  // 10. Clear Data Buffer
  //
#if !AMD_USB4_DEBUG_CLEAR_RING_BUFFER
  ZeroMem (RxDataBuffer, *DataSize);
#endif

  //
  // 11. Increase Consumer Index both in H/I Ring[N] and pCM
  //
  pCM->RingRxConsumerIndex[RingNumber] = ( pCM->RingRxConsumerIndex[RingNumber] + 1 ) % ((UINT16)RingSize);
  pRingRx->DescIndex.Consumer_Producer_Index.Consumer_Index = pCM->RingRxConsumerIndex[RingNumber];
  pCM->RingRxProducerIndex[RingNumber] = ( pCM->RingRxProducerIndex[RingNumber] + 1 ) % ((UINT16)RingSize);
  //
  // 12. RemainPacket = ProducerIndex - ConsumerIndex
  //

  //// Read the Producer Index Register again to get latest Producer Index
  ProducerIndex = pRingRx->DescIndex.Consumer_Producer_Index.Producer_Index;
  if ( pCM->RingRxProducerIndex[RingNumber] <= ProducerIndex ) {
    *RemainPacket = ProducerIndex - pCM->RingRxProducerIndex[RingNumber];
  } else if ( pCM->RingRxProducerIndex[RingNumber] > ProducerIndex ) {
    *RemainPacket = ProducerIndex + (UINT16)RingSize - pCM->RingRxProducerIndex[RingNumber];
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("RemainPacket=%d\n", *RemainPacket);
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Success.\n", __FUNCTION__);

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Transmit Flow Raw Mode for Ring 0 without Interrupt Enabled.
 *
 * @param[in] *pCM             Point to global structure AMD_USB4_CM
 * @param[in] Pdf              Route String
 * @param[in] DataSize         Number of bytes
 * @param[in] *Data            Pointer to the Data
 *
 * @retval EFI_SUCCESS           Success to send out the data.
 * @retval EFI_OUT_OF_RESOURCES  Transmit Ring is always full.
 * @retval EFI_DEVICE_ERROR -    Host Interface do not send out the data in expected time.
**/

EFI_STATUS
AmdUsb4HostInterfaceRing0Tx (
  IN    AMD_USB4_CM  *pCM,
  IN    UINT32       Pdf,
  IN    UINT32       DataSize,
  IN    UINT32       *Data
  )
{
  return AmdUsb4HostInterfaceTxFlowRawMode (
           pCM,
           0,
           Pdf,
           DataSize,
           Data,
           FALSE
           );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Receive Flow Raw Mode for Ring 0.
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 *
 * @param[out] *Pdf             Return the PDF in the control packet
 * @param[out] *DataSize        Return the number of bytes
 * @param[out] *Data            Return the pointer of the data.
 * @param[out] *RemainPacket    Return the remain packet in Receive Ring.
 *
 * @retval EFI_SUCCESS          Success to retrieve the data.
 * @retval EFI_NOT_FOUND        Receive Ring is empty.
 * @retval EFI_DEVICE_ERROR -   Receive Ring is not empty, but packet of Descriptor Done bit is not 1 or buffer OverFlow or CRC error.
**/

EFI_STATUS
AmdUsb4HostInterfaceRing0Rx (
  IN    AMD_USB4_CM   *pCM,
  IN    UINT32        *Pdf,
  IN    UINT32        *DataSize,
  IN    UINT32        *Data,
  IN    UINT32        *RemainPacket
  )
{
  return AmdUsb4HostInterfaceRxFlowRawMode (
           pCM,
           0,
           Pdf,
           DataSize,
           Data,
           RemainPacket
           );
}
