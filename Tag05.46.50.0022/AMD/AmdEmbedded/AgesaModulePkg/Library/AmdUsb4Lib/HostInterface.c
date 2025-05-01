/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/RingDescriptor.h>
#include <AmdUsb4/HostInterface.h>
#include <Library/BaseMemoryLib.h>
#include "Library/AmdUsb4Lib.h"
#include <Library/TimerLib.h>

#define FILECODE LIBRARY_AMDUSB4LIB_HOSTINTERFACE_FILECODE

/**
  Init RingN for both TX and RX ring registers.
  All memory resource should have been allocated already.

  @param  pCM              Pointer to the internal CM data structure.
  @param  RingNumber       Ring number that's going to be init.
  @param  TxRingSize       Tx Ring Size in Descriptor number.
  @param  RxRingSize       Rx Ring Size in Descriptor number.
  @param  TxBufferSize     Tx Data Buffer for each Descriptor in Byte.
  @param  RxBufferSize     Rx Data Buffer for each Descriptor in Byte.

  @retval EFI_SUCCESS           Find USB4 Host successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.
  @retval EFI_NOT_READY         Ring related Memory resource has not been allocated.

  // 1. Get Ring Base. Caculate from pCM->MMIO
  // 2. Zero out Memory allocated in pCM->RingTxBase.
  // 3. Update H/I Tx Ring[N] registers: base and ring size
  // 4. Zero out Memory allocated in pCM->RingRxBase
  // 5. Update H/I Rx Ring[N] registers: base and ring size
  // 6. Get Allocated Tx Data buffer in pCM->RingTxDataBufferBase
  // 7. Init Tx descriptors to point to the Data buffer
  // 8. Get Allocate Rx Data buffer in pCM->RingRxDataBufferBase
  // 9. Init Rx descriptors to point to the Data buffer
  // 10. Update H/I Rx Ring[N] register for Buffer Size

**/
EFI_STATUS
AmdUsb4HostInterfaceRingSetup (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber,
  IN     UINT32                  TxRingSize,
  IN     UINT32                  RxRingSize,
  IN     UINT32                  TxBufferSize,
  IN     UINT32                  RxBufferSize
  )
{
  UINT32      Index;
  UINT64      BufferBase;
  volatile AMD_USB4_TX_DESCRIPTOR_RING  *pTxRing;
  volatile AMD_USB4_RX_DESCRIPTOR_RING  *pRxRing;
  AMD_USB4_DESCRIPTOR *pRingDescriptor;

  BufferBase = 0;

  if ((TxRingSize == 0) || (TxRingSize > MAX_RING_DESCRIPTOR_NUM )) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Tx Ring Size is invalid.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ((RxRingSize == 0) || (RxRingSize > MAX_RING_DESCRIPTOR_NUM )) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Rx Ring Size is invalid.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ((pCM->RingTxBase[RingNumber] == 0) || (pCM->RingTxDataBufferBase[RingNumber] == 0)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Tx Ring is not allocated.\n");
    return EFI_NOT_READY;
  }

  if ((pCM->RingRxBase[RingNumber] == 0) || (pCM->RingRxDataBufferBase[RingNumber] == 0)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Rx Ring is not allocated.\n");
    return EFI_NOT_READY;
  }

  //
  // 1. Get Ring Base. Caculate from Host Interface MMIO base from pCM->MMIO
  //
  pTxRing = (AMD_USB4_TX_DESCRIPTOR_RING*)(UINTN)(pCM->HIMmioBase + TX_BASE_ADDRESS_LOW (RingNumber));
  pRxRing = (AMD_USB4_RX_DESCRIPTOR_RING*)(UINTN)(pCM->HIMmioBase + RX_BASE_ADDRESS_LOW (RingNumber));

  //
  // 2. Zero out Memory allocated in pCM->RingTxBase
  //
  pRingDescriptor = (AMD_USB4_DESCRIPTOR *)(UINTN)(pCM->RingTxBase[RingNumber]);
  ZeroMem (pRingDescriptor, TxRingSize * sizeof (AMD_USB4_DESCRIPTOR));

  //
  // 3. Update H/I Tx Ring[N] registers: base and ring size
  //
  pTxRing->Address_Low        = (UINT32) (pCM->RingDevTxBase[RingNumber]);
  pTxRing->Address_High       = (UINT32) ((pCM->RingDevTxBase[RingNumber]) >> 32);
  pTxRing->RingSize           = TxRingSize;

  //
  // 4. Zero out Memory allocated in pCM->RingRxBase
  //
  pRingDescriptor = (AMD_USB4_DESCRIPTOR *)(UINTN)(pCM->RingRxBase[RingNumber]);
  ZeroMem (pRingDescriptor, RxRingSize * sizeof (AMD_USB4_DESCRIPTOR));

  //
  // 5. Update H/I Rx Ring[N]
  //
  pRxRing->Address_Low        = (UINT32) (pCM->RingDevRxBase[RingNumber]);
  pRxRing->Address_High       = (UINT32) ((pCM->RingDevRxBase[RingNumber]) >> 32);
  pRxRing->RingSize           = RxRingSize;

  //
  // 6. Zero out allocated Tx data buffer
  //
  BufferBase = pCM->RingTxDataBufferBase[RingNumber];
  ZeroMem ((void *)(UINTN)BufferBase, TxRingSize * TxBufferSize);
  BufferBase = pCM->RingDevTxDataBufferBase[RingNumber];
  pRingDescriptor = (AMD_USB4_DESCRIPTOR *)(UINTN)(pCM->RingTxBase[RingNumber]);

  //
  // 7. Init each Tx descriptor in Tx Ring
  //
  for (Index = 0; Index < TxRingSize; Index++) {
    pRingDescriptor->AddressLow  = (UINT32)BufferBase;
    pRingDescriptor->AddressHigh = (UINT32)(BufferBase >> 32);
    pRingDescriptor->DescriptorData.TxDesc.RequestStatus = 1;

    BufferBase += (UINT64)TxBufferSize;
    pRingDescriptor++;
  }

  //
  // 8. Zero out allocated Rx data  buffer
  //
  BufferBase = pCM->RingRxDataBufferBase[RingNumber];
  ZeroMem ((void *)(UINTN)BufferBase, RxRingSize * RxBufferSize);
  BufferBase = pCM->RingDevRxDataBufferBase[RingNumber];
  pRingDescriptor = (AMD_USB4_DESCRIPTOR *)(UINTN)(pCM->RingRxBase[RingNumber]);

  //
  // 9. Init each Rx descriptor in Rx Ring
  //
  for (Index = 0; Index < RxRingSize; Index++) {
    pRingDescriptor->AddressLow  = (UINT32)BufferBase;
    pRingDescriptor->AddressHigh = (UINT32)(BufferBase >> 32);
    pRingDescriptor->DescriptorData.RxHostDesc.RequestStatus = 1;

    BufferBase += (UINT64)RxBufferSize;
    pRingDescriptor++;
  }

  //
  // 10. Update H/I Rx Ring[N] register for Buffer Size
  //
  pRxRing->DataBufferSize = RxBufferSize;


  //
  // 11. Init RX Ring Index
  //
  pCM->RingRxProducerIndex[RingNumber] = pRxRing->DescIndex.Consumer_Producer_Index.Producer_Index;
  pCM->RingRxConsumerIndex[RingNumber] = ( pCM->RingRxProducerIndex[RingNumber] + (UINT16)RxRingSize - 1 ) %
                                           ((UINT16)RxRingSize);
  pRxRing->DescIndex.Consumer_Producer_Index.Consumer_Index = pCM->RingRxConsumerIndex[RingNumber];

  return EFI_SUCCESS;
}

/**
  Enable RingN.

  @param  pCM                     Pointer to the internal CM data structure.
  @param  RingNumber              The ring number thats going to be init.
  @param  RawMode                 Raw mode or not (Frame mode).
  @param  E2eFlowControlEnable    Enable E2E Flow Control or not.
  @param  NoSnoopFlag             Set No-snoop attribute or not.
  @param  E2eHopID                E2E Credit Grant Packet HopID when E2E Flow Control enable.

  @retval EFI_SUCCESS           Find USB4 Host successfully.


  // 1. Enable Tx Ring
  //    1) Update H/I Tx Ring[N] attribute: E2E Flow Control, Raw Mode, No-snoop Flag.
  //    2) Set Ring Valid bit = 1
  // 2. Enable Rx Ring
  //    1) Update H/I Tx Ring[N] attribute: E2E Flow Control, Raw Mode, No-snoop Flag, E2E HopID.
  //    2) Set Ring Valid bit = 1
  //    * Producer and Consumer Index = 0 when update Ring Size

**/
EFI_STATUS
AmdUsb4HostInterfaceRingEnable (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber,
  IN     BOOLEAN                 RawMode,
  IN     BOOLEAN                 E2eFlowControlEnable,
  IN     BOOLEAN                 NoSnoopFlag,
  IN     UINT32                  E2eHopID
  )
{
  volatile AMD_USB4_TX_RING_CONTROL_REGISTER  *TxRingControl;
  volatile AMD_USB4_RX_RING_CONTROL_REGISTER  *RxRingControl;

  //
  // Get Host Interface MMIO base from pCM->MMIO
  //
  TxRingControl = (AMD_USB4_TX_RING_CONTROL_REGISTER *)(UINTN)(pCM->HIMmioBase + TX_RING_CONTROL (RingNumber));
  RxRingControl = (AMD_USB4_RX_RING_CONTROL_REGISTER *)(UINTN)(pCM->HIMmioBase + RX_RING_CONTROL (RingNumber));

  //
  // Tx Ring
  //
  TxRingControl->Ring_Control_Register.E2E_Flow_Control_Enable = (E2eFlowControlEnable) ? 1 : 0;
  TxRingControl->Ring_Control_Register.RAW                     = (RawMode) ? 1 : 0;
  TxRingControl->Ring_Control_Register.NS                      = (NoSnoopFlag) ? 1 : 0;

  TxRingControl->Ring_Control_Register.Ring_Valid              = 1;

  //
  // Rx Ring
  //
  if (E2eFlowControlEnable) {
    RxRingControl->Rx_Ring_Control_Register.E2E_Flow_Control_Enable = 1;
    RxRingControl->Rx_Ring_Control_Register.TxE2EHopId = E2eHopID;
  } else {
    RxRingControl->Rx_Ring_Control_Register.E2E_Flow_Control_Enable = 0;
  }
  RxRingControl->Rx_Ring_Control_Register.RAW                     = (RawMode) ? 1 : 0;
  RxRingControl->Rx_Ring_Control_Register.NS                      = (NoSnoopFlag) ? 1 : 0;

  RxRingControl->Rx_Ring_Control_Register.Ring_Valid              = 1;

  return EFI_SUCCESS;
}

/**
  Init Host Interface Ring0

  @param  pCM              Pointer to the internal CM data structure.
  @param  TxRingSize       Tx Ring Size in descriptor.
  @param  RxRingSize       Rx Ring Size in descriptor.
  @param  TxBufferSize     Tx Data Buffer in Byte.
  @param  RxBufferSize     Rx Data Buffer in Byte.

  @retval EFI_SUCCESS           Find USB4 Host successfully.
  @retval EFI_INVALID_PARAMETER Some parameter is not valid.
  @retval EFI_NOT_FOUND         Do not find USB4 Host.

**/
EFI_STATUS
AmdUsb4HostInterfaceRing0Init (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  TxRingSize,
  IN     UINT32                  RxRingSize,
  IN     UINT32                  TxBufferSize,
  IN     UINT32                  RxBufferSize
  )
{
  EFI_STATUS        Status;
  volatile UINT32   *PtrHiReset;

  Status      = EFI_SUCCESS;
  PtrHiReset  = (UINT32*)(UINTN)(pCM->HIMmioBase + HI_RESET);

#if AMD_USB4_DEBUG_RESET_HOST_INTERFACE
  IDS_HDT_CONSOLE_USB4_DEBUG ("Host Interface Reset is at 0x%x.\n", PtrHiReset);
  *PtrHiReset = 1;
  IDS_HDT_CONSOLE_USB4_DEBUG ("Host Interface Reset Delay 10ms. PMTimer=0x%x\n", GetACPITimer ());
  MicroSecondDelay (15000);
  IDS_HDT_CONSOLE_USB4_DEBUG ("Host Interface Reset Delay 10ms. PMTimer=0x%x\n", GetACPITimer ());
  IDS_HDT_CONSOLE_USB4_ASSERT (!((*PtrHiReset) & BIT0));
#endif

  Status = AmdUsb4HostInterfaceRingSetup (
             pCM,
             0,
             TxRingSize,
             RxRingSize,
             TxBufferSize,
             RxBufferSize
             );

  Status = AmdUsb4HostInterfaceRingEnable (
             pCM,
             0,
             RAW_MODE_ENABLE,
             E2E_FLOW_DISABLE,
             NO_SNOOP_DISABLE,
             0
             );

  return Status;
}
