/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "MpmSerialIo.h"
#include <Library/MpmDevLib.h>
#include <Library/MpmBiosCmdLib.h>
#include <cpuRegisters.h>

#define FILECODE MPM_MPMSERIALIODXE_MPMSERIALIODXE_FILECODE


static EFI_HANDLE                          *mMpmDevPciIoHandle = NULL;
static EFI_PCI_IO_PROTOCOL                 *mMpmDevPciIo = NULL;
static MPM_DEV_PROTOCOL                    *mMpmDevProtocol = NULL;

UINT32 gMpmTxWritePointer = 0;
UINT32 gMpmRxReadPointer  = 0;

SERIAL_DEVICE_PATH mSerialDevicePath = {
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, { sizeof (VENDOR_DEVICE_PATH), 0} },
    EDKII_SERIAL_PORT_LIB_VENDOR_GUID
  },
  {
    { MESSAGING_DEVICE_PATH, MSG_UART_DP, { sizeof (UART_DEVICE_PATH), 0} },
    0,  // Reserved
    0,  // BaudRate
    0,  // DataBits
    0,  // Parity
    0 // StopBits
  },
  { END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, { sizeof (EFI_DEVICE_PATH_PROTOCOL), 0 }
  }
};


EFI_SERIAL_IO_MODE mSerialIoMode = {
  0,        // ControlMask
  1000000,  // Timeout
  115200,   // BaudRate
  1,        // ReceiveFifoDepth
  0,        // DataBits
  0,        // Parity
  0         // StopBits
};

EFI_SERIAL_IO_PROTOCOL mSerialIoTemplate = {
  SERIAL_IO_INTERFACE_REVISION,
  SerialReset,
  SerialSetAttributes,
  SerialSetControl,
  SerialGetControl,
  SerialWrite,
  SerialRead,
  &mSerialIoMode
};


/**
  Check if the RingBuffer is empty or not

  @param  [in] RingBuffer   The pointer of MPM_TEXT_RING_BUFFER structure

  @retval TRUE              The RingBuffer is empty
  @retval FALSE             The RingBuffer is not empty

**/
BOOLEAN
IsMpmTextRingBufferEmpty(
  IN MPM_TEXT_RING_BUFFER *RingBuffer
)
{
  return RingBuffer->ReadPointer == RingBuffer->WritePointer ? TRUE : FALSE;
}


/**
  Read one byte from the RingBuffer

  @param  [in] RingBuffer   The pointer of MPM_TEXT_RING_BUFFER structure
  @param  [in] RingBufferSize The size of the RingBuffer
  @param  [out] Data        The data read from the RingBuffer

  @retval EFI_SUCCESS
  @retval EFI_NOT_READY     RingBuffer is empty

**/
EFI_STATUS
ReadOneByteFromRingBuffer (
  IN MPM_TEXT_RING_BUFFER *RingBuffer,
  IN UINT32               RingBufferSize,
  IN OUT UINT8            *Data
)
{
  if (IsMpmTextRingBufferEmpty(RingBuffer)) {
    return EFI_NOT_READY;
  }

  *Data = RingBuffer->Data[gMpmRxReadPointer];
  gMpmRxReadPointer = (gMpmRxReadPointer + 1) % (RingBufferSize - 8); //Minus RP+WP size
  RingBuffer->ReadPointer = gMpmRxReadPointer;

  return EFI_SUCCESS;
}

/**
  Write one byte to the RingBuffer

  @param  [in] RingBuffer   The pointer of MPM_TEXT_RING_BUFFER structure
  @param  [in] RingBufferSize The size of the RingBuffer
  @param  [out] Data        The data to be written to the RingBuffer

  @retval EFI_SUCCESS

**/
EFI_STATUS
WriteOneByteToRingBuffer (
  IN MPM_TEXT_RING_BUFFER *RingBuffer,
  IN UINT32               RingBufferSize,
  IN UINT8                Data
)
{
  RingBuffer->WritePointer = gMpmTxWritePointer;
  RingBuffer->Data[gMpmTxWritePointer] = Data;
  gMpmTxWritePointer = (gMpmTxWritePointer + 1) % (RingBufferSize - 8); ////Minus RP+WP size

  return EFI_SUCCESS;
}

/**
  Send one byte data to MPM via PSP MpmMbox interface

  @param  This        Protocol instance pointer.
  @param  [in] Data   The Data to be sent to MPM

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR

**/
EFI_STATUS
MpmTextWriteOneByte(
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT8    Data
)
{
  EFI_STATUS    Status;
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);

  Status = WriteOneByteToRingBuffer((MPM_TEXT_RING_BUFFER *)SerialIoDev->TxBuffer,
                                    SerialIoDev->TxBufferSize,
                                    Data);
  return Status;
}

/**
  Read one byte data from MPM via PSP MpmMbox interface

  @param  This        Protocol instance pointer.
  @param  [in] *Data  The Data read from MPM

  @retval EFI_SUCCESS
  @retval EFI_NOT_READY

**/
EFI_STATUS
MpmTextReadyOneByte (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINT8   *Data
)
{
  EFI_STATUS    Status;
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);

  Status = ReadOneByteFromRingBuffer((MPM_TEXT_RING_BUFFER *)SerialIoDev->RxBuffer,
                                      SerialIoDev->RxBufferSize,
                                      Data);
  return Status;
}

/**
  Reset the serial device.

  @param  This              Protocol instance pointer.

  @retval EFI_SUCCESS       The device was reset.
  @retval EFI_DEVICE_ERROR  The serial device could not be reset.

**/
EFI_STATUS
EFIAPI
SerialReset (
  IN EFI_SERIAL_IO_PROTOCOL *This
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", __FUNCTION__);

  //
  // Go set the current attributes
  //
  Status = This->SetAttributes (
                   This,
                   This->Mode->BaudRate,
                   This->Mode->ReceiveFifoDepth,
                   This->Mode->Timeout,
                   (EFI_PARITY_TYPE) This->Mode->Parity,
                   (UINT8) This->Mode->DataBits,
                   (EFI_STOP_BITS_TYPE) This->Mode->StopBits
                   );

  //
  // The serial device may not support some of the attributes. To prevent
  // later failure, always return EFI_SUCCESS when SetAttributes is returning
  // EFI_INVALID_PARAMETER.
  //
  if (Status == EFI_INVALID_PARAMETER) {
    return EFI_SUCCESS;
  }

  SerialSetControl (This, EFI_SERIAL_REQUEST_TO_SEND | EFI_SERIAL_DATA_TERMINAL_READY);

  return Status;
}

/**
  Sets the baud rate, receive FIFO depth, transmit/receive time out, parity,
  data bits, and stop bits on a serial device.

  @param  This             Protocol instance pointer.
  @param  BaudRate         The requested baud rate. A BaudRate value of 0 will use the the
                           device's default interface speed.
  @param  ReceiveFifoDepth The requested depth of the FIFO on the receive side of the
                           serial interface. A ReceiveFifoDepth value of 0 will use
                           the device's default FIFO depth.
  @param  Timeout          The requested time out for a single character in microseconds.
                           This timeout applies to both the transmit and receive side of the
                           interface. A Timeout value of 0 will use the device's default time
                           out value.
  @param  Parity           The type of parity to use on this serial device. A Parity value of
                           DefaultParity will use the device's default parity value.
  @param  DataBits         The number of data bits to use on the serial device. A DataBits
                           value of 0 will use the device's default data bit setting.
  @param  StopBits         The number of stop bits to use on this serial device. A StopBits
                           value of DefaultStopBits will use the device's default number of
                           stop bits.

  @retval EFI_SUCCESS           The device was reset.
  @retval EFI_INVALID_PARAMETER One or more attributes has an unsupported value.
  @retval EFI_DEVICE_ERROR      The serial device is not functioning correctly.

**/
EFI_STATUS
EFIAPI
SerialSetAttributes (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT64                 BaudRate,
  IN UINT32                 ReceiveFifoDepth,
  IN UINT32                 Timeout,
  IN EFI_PARITY_TYPE        Parity,
  IN UINT8                  DataBits,
  IN EFI_STOP_BITS_TYPE     StopBits
  )
{
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);
  //
  // Update the device path
  //
  SerialIoDev->SerialDevicePath.Uart.BaudRate = BaudRate;
  SerialIoDev->SerialDevicePath.Uart.DataBits = DataBits;
  SerialIoDev->SerialDevicePath.Uart.Parity   = (UINT8) Parity;
  SerialIoDev->SerialDevicePath.Uart.StopBits = (UINT8) StopBits;

  return EFI_SUCCESS;
}

/**
  Set the control bits on a serial device

  @param  This             Protocol instance pointer.
  @param  Control          Set the bits of Control that are settable.

  @retval EFI_SUCCESS      The new control bits were set on the serial device.
  @retval EFI_UNSUPPORTED  The serial device does not support this operation.
  @retval EFI_DEVICE_ERROR The serial device is not functioning correctly.

**/
EFI_STATUS
EFIAPI
SerialSetControl (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT32                 Control
  )
{
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);
  //
  // The control bits that can be set are :
  //     EFI_SERIAL_DATA_TERMINAL_READY: 0x0001  // WO
  //     EFI_SERIAL_REQUEST_TO_SEND: 0x0002  // WO
  //     EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE: 0x1000  // RW
  //     EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE: 0x2000  // RW
  //     EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE: 0x4000 // RW
  //


  if ((Control & (~(EFI_SERIAL_REQUEST_TO_SEND | EFI_SERIAL_DATA_TERMINAL_READY |
                    EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE | EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE |
                    EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE))) != 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%a Control(0x%x) is not allowed to be set\n", __FUNCTION__, Control);
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("%a Control=0x%x\n", __FUNCTION__, Control);


  if ((Control & EFI_SERIAL_DATA_TERMINAL_READY) == EFI_SERIAL_DATA_TERMINAL_READY) {
    SerialIoDev->SerialControl |= EFI_SERIAL_DATA_TERMINAL_READY;
  }

  if ((Control & EFI_SERIAL_REQUEST_TO_SEND) == EFI_SERIAL_REQUEST_TO_SEND) {
    SerialIoDev->SerialControl |= EFI_SERIAL_REQUEST_TO_SEND;
  }

  if ((Control & EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE) == EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE) {
    SerialIoDev->SerialControl |= EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE;
  }

  if ((Control & EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE) == EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE) {
    SerialIoDev->SerialControl |= EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE;
  }

  if ((Control & EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE) == EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE) {
    SerialIoDev->SerialControl |= EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE;
  }

  return EFI_SUCCESS;
}

/**
  Retrieves the status of the control bits on a serial device

  @param  This              Protocol instance pointer.
  @param  Control           A pointer to return the current Control signals from the serial device.

  @retval EFI_SUCCESS       The control bits were read from the serial device.
  @retval EFI_DEVICE_ERROR  The serial device is not functioning correctly.

**/
EFI_STATUS
EFIAPI
SerialGetControl (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  OUT UINT32                *Control
  )
{
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);

  if (IsMpmTextRingBufferEmpty(SerialIoDev->RxBuffer)) {
    SerialIoDev->SerialControl |= EFI_SERIAL_INPUT_BUFFER_EMPTY;
  } else {
    SerialIoDev->SerialControl &= ~EFI_SERIAL_INPUT_BUFFER_EMPTY;
  }

  *Control = SerialIoDev->SerialControl;


  return EFI_SUCCESS;
}

/**
  Writes data to a serial device.

  @param  This              Protocol instance pointer.
  @param  BufferSize        On input, the size of the Buffer. On output, the amount of
                            data actually written.
  @param  Buffer            The buffer of data to write

  @retval EFI_SUCCESS       The data was written.
  @retval EFI_DEVICE_ERROR  The device reported an error.
  @retval EFI_TIMEOUT       The data write was stopped due to a timeout.

**/
EFI_STATUS
EFIAPI
SerialWrite (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINTN              *BufferSize,
  IN VOID                   *Buffer
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);

  for (Index = 0; Index < *BufferSize; Index++) {
    Status = MpmTextWriteOneByte(This, ((UINT8 *)Buffer)[Index]);
    if (EFI_ERROR(Status)) {
      *BufferSize = Index;
      return Status;
    }
  }

  return Status;
}

/**
  Reads data from a serial device.

  @param  This              Protocol instance pointer.
  @param  BufferSize        On input, the size of the Buffer. On output, the amount of
                            data returned in Buffer.
  @param  Buffer            The buffer to return the data into.

  @retval EFI_SUCCESS       The data was read.
  @retval EFI_DEVICE_ERROR  The device reported an error.
  @retval EFI_TIMEOUT       The data write was stopped due to a timeout.

**/
EFI_STATUS
EFIAPI
SerialRead (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINTN              *BufferSize,
  OUT VOID                  *Buffer
  )
{
  EFI_STATUS    Status;
  MPM_SERIAL_IO_DEV *SerialIoDev;

  SerialIoDev = MPM_SERIAL_IO_DEV_FROM_THIS(This);

  *BufferSize = 0;

  Status = MpmTextReadyOneByte(This, (UINT8 *)Buffer);
  if (EFI_ERROR(Status)) {
    *BufferSize = 0;
  } else {
    *BufferSize = 1;
  }

  return EFI_SUCCESS;
}

/**
 * @brief Enable Bus Master for WLAN device and the root port which WLAN device is connected
 * MPM FW need read/write WLAN Memory/IO space, that need the BusMaster enable bit set in the RootBridge
 * which the WLAN device is connected, in case OEM doesn't enable BusMaster bit, so do it in MPM TCR driver
 * @param DeviceBusNumber WLAN device bus number
 * @return EFI_STATUS
 */

EFI_STATUS
TcrWlanRootBridgeBusMasterEnable (
  IN UINTN  DeviceBusNumber
)
{
   EFI_STATUS           Status;
   EFI_HANDLE           *PciIoHandles;
   UINTN                NoPciIoHandles;
   UINTN                Index;
   EFI_PCI_IO_PROTOCOL  *PciIo;
   UINTN                SegmentNumber;
   UINTN                BusNumber;
   UINTN                DeviceNumber;
   UINTN                FunctionNumber;
   UINT32               ClassCodeValue;
   UINT8                SubordinateBusNumber;
   UINT8                CommandValue;

   IDS_HDT_CONSOLE_PSP_TRACE ("TcrWlanRootBridgeBusMasterEnable DeviceBusNumber is %d\n", DeviceBusNumber);
   Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  for (Index = 0; Index < NoPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
        PciIoHandles[Index],
        &gEfiPciIoProtocolGuid,
        &PciIo
        );
    if (EFI_ERROR (Status)) {
      continue;
    }
    PciIo->GetLocation(
              PciIo,
              &SegmentNumber,
              &BusNumber,
              &DeviceNumber,
              &FunctionNumber
    );
    PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      CLASS_CODE_REGISTER_OFFSET,
      1,
      &ClassCodeValue
      );
    if ((ClassCodeValue & HOST_BRIDGE_CLASS_CODE) == HOST_BRIDGE_CLASS_CODE) {
      PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint8,
        SUBORDINATE_BUS_NUMBER,
        1,
        &SubordinateBusNumber
      );
      if (SubordinateBusNumber == DeviceBusNumber) {
        IDS_HDT_CONSOLE_PSP_TRACE ("SubordinateBusNumber is %d\n", SubordinateBusNumber);
        IDS_HDT_CONSOLE_PSP_TRACE ("RootBridge connected to WLAN device is found, set Bus master enable\n");
        CommandValue = COMMAND_REGISTER_VALUE;
        Status = PciIo->Pci.Write (
                   PciIo,
                   EfiPciIoWidthUint32,
                   COMMAND_REGISTER_OFFSET,
                   1,
                   &CommandValue
                 );
      }
    }
  }
  return Status;
}

/**
 * @brief Detect TCR requested NIC through PCI scan
 *
 * @return EFI_STATUS
 */
EFI_STATUS
DetectMpmTextDevice (
  VOID
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_HANDLE           *PciIoHandles;
  UINTN                NoPciIoHandles;
  UINTN                Index;
  UINT32               TcrDevNum;
  EFI_STATUS           Status;
  EFI_HANDLE           *DevPciIoHandle;
  UINT32               TcrRequest;
  UINTN                SegmentNumber;
  UINTN                BusNumber;
  UINTN                DeviceNumber;
  UINTN                FunctionNumber;


  PciIoHandles = NULL;
  PciIo = NULL;
  TcrRequest = TCR_NOT_REQUESTED;

  IDS_HDT_CONSOLE_PSP_TRACE ("DetectMpmTextDevice\n");
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("%x PciIoHandles for Scan\n", NoPciIoHandles);

  //Loop All PCI handles
  for (Index = 0; Index < NoPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
        PciIoHandles[Index],
        &gEfiPciIoProtocolGuid,
        &PciIo
        );
    if (EFI_ERROR (Status)) {
      continue;
    }
    DevPciIoHandle = PciIoHandles[Index];
    //Search if current handle have TCR request
    for (TcrDevNum = 0; TcrDevNum < GetMpmDevProtocolNum (); TcrDevNum++) {
      //TCR only support wireless
      if (mMpmDevProtocols [TcrDevNum]->DeviceType == MPM_NIC_WIRELESS) {
        if (mMpmDevProtocols [TcrDevNum]->IsSupported (PciIoHandles[Index])) {
          mMpmDevProtocols [TcrDevNum]->ReadTcrRequest (DevPciIoHandle, &TcrRequest);
          //We found the device with request bit set, it what we are searching for, exit the loop
          if (TcrRequest == TCR_REQUESTED) {
            mMpmDevPciIoHandle = DevPciIoHandle;
            mMpmDevPciIo = PciIo;
            mMpmDevProtocol = mMpmDevProtocols [TcrDevNum];
            //Enable Root Port Bus Master Enable bit which WLAN device is connected
            //Get the Root Port Bus/Dev/Fun which WLAN device connected
            Status = PciIo->GetLocation(
                              PciIo,
                              &SegmentNumber,
                              &BusNumber,
                              &DeviceNumber,
                              &FunctionNumber
            );
            if (EFI_ERROR(Status)) {
               return Status;
            }
            IDS_HDT_CONSOLE_PSP_TRACE ("WLAN device Bus number is %d\n", BusNumber);
            TcrWlanRootBridgeBusMasterEnable (BusNumber);
            break;
          }
        }
      }
    }
    //Exit main loop
    if (TcrRequest == TCR_REQUESTED) {
      break;
    }
  }

  if (TcrRequest == TCR_NOT_REQUESTED) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}
/**
 * @brief Prepare parameters BIOS_MPM_CMD_START_TEXT required, then Send BIOS_MPM_CMD_START_TEXT command
 *
 * @return EFI_STATUS
 */
VOID
EFIAPI
MpmStartTcrCallBack (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS              Status;
  MBOX_START_TEXT         StartTextPara;
  VOID                    *TempBuffer;
  UINTN                   BufferSize;
  VOID                    *Mapping;
  EFI_PHYSICAL_ADDRESS    MappedAddr;
  MPM_PRVIATE_HEAP_DATA_V1 *MpmData;
  EFI_HANDLE            MpmTcrQuestHandle = NULL;
  UINT32                  MpmMmioBase = 0;
  UINTN                   TcrMpmC2pMsg2HpaAddr = 0;

  Status = DetectMpmTextDevice ();
  //We found the device
  if (Status == EFI_SUCCESS) {
    PspMboxBiosMpmPciAccess (ENABLE_MPM_PCI_ACC, WireLessFlag);
    ZeroMem (&StartTextPara, sizeof (MBOX_START_TEXT));
    Status = mMpmDevProtocol->GetMmioBase (mMpmDevPciIoHandle, &StartTextPara.NetCardPciCfgAddr);
    IDS_HDT_CONSOLE_PSP_TRACE ("NetCardPciCfgAddr %x\n", StartTextPara.NetCardPciCfgAddr);

    //Buffer for DMA
    Status = LocateMpmPrivateData (&MpmData);
    if (EFI_ERROR (Status)) {
      return;
    }
    TempBuffer = (VOID *) (UINTN) MpmData->WirelessDmaBufAddr;
    StartTextPara.TcrDmaHpaAddrLo = (UINT32) ((UINT64) (UINTN) TempBuffer  & 0xFFFFFFFF);
    StartTextPara.TcrDmaHpaAddrHi = (UINT32) ((UINT64) (UINTN) TempBuffer >> 32);
    //DMAr related
    StartTextPara.DMArEnabled = PcdGetBool(PcdIvInfoDmaReMap);
    StartTextPara.PcieMmioCfgBaseAddr = AsmReadMsr64 (MSR_MMIO_Cfg_Base) & 0x0000FFFFFFF00000;
    if (StartTextPara.DMArEnabled) {
      BufferSize = MPM_DMA_HPA_BUF_SIZE;
      Status = mMpmDevPciIo->Map (mMpmDevPciIo,
                      EfiPciIoOperationBusMasterCommonBuffer,
                      TempBuffer,
                      &BufferSize,
                      &MappedAddr,
                      &Mapping
                      );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("mMpmDevPciIo->Map failed\n");
        return;
      }
      StartTextPara.TcrDmaGpaAddrLo = (UINT32) (MappedAddr & 0xFFFFFFFF);
      StartTextPara.TcrDmaGpaAddrHi = (UINT32) (MappedAddr >> 32);
      IDS_HDT_CONSOLE_PSP_TRACE (" GPA:TcrDmaGpaAddrHi = 0x%08x%08x, TcrDmaGpaAddrLo = 0x%08x%08x, BufferSize = 0x%x\n", (UINT32)(MappedAddr >> 32), (UINT32)MappedAddr, BufferSize);

      MpmMmioBase = GetMpmBarAddr();
      if (MpmMmioBase == 0) {
        IDS_HDT_CONSOLE_PSP_TRACE ("MPM MMIO not initialized\n");
        return;
      }
      BufferSize = MPM_C2PMSG_2_SIZE;
      TcrMpmC2pMsg2HpaAddr = MpmMmioBase + MPM_C2PMSG_2_OFFSET; //Wlan FW get interrupt number from this register
      StartTextPara.TcrMpmC2pMsg2HpaAddrLo = (UINT32) (TcrMpmC2pMsg2HpaAddr & 0xFFFFFFFF);
      StartTextPara.TcrMpmC2pMsg2HpaAddrHi = (UINT32) (TcrMpmC2pMsg2HpaAddr >> 32);
      TempBuffer = (VOID *) (UINTN) TcrMpmC2pMsg2HpaAddr;
      Status = mMpmDevPciIo->Map (mMpmDevPciIo,
                      EfiPciIoOperationBusMasterCommonBuffer,
                      (VOID *)TempBuffer,
                      &BufferSize,
                      &MappedAddr,
                      &Mapping
                      );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("TCR: MpmDevPciIo->Map error\n");
        return;
      }
      StartTextPara.TcrMpmC2pMsg2GpaAddrLo = (UINT32) (MappedAddr & 0xFFFFFFFF);
      StartTextPara.TcrMpmC2pMsg2GpaAddrHi = (UINT32) (MappedAddr >> 32);
      IDS_HDT_CONSOLE_PSP_TRACE (" TCR: MpmC2pMsg2 HPA address: 0x%x, MpmC2pMsg2 GPA:MappedAddr = 0x%x, BufferSize = 0x%x\n", TcrMpmC2pMsg2HpaAddr, MappedAddr, BufferSize);
    }
    Status = BiosMpmCmdStartText (&StartTextPara);
    if (!EFI_ERROR (Status)) {
      Status = gBS->InstallProtocolInterface (
                  &MpmTcrQuestHandle,
                  &gAmdMpmTcrInitializedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
      IDS_HDT_CONSOLE_PSP_TRACE ("Install gAmdMpmTcrInitializedProtocolGuid Status %r\n", Status);
    }
  }
  return;
}

EFI_STATUS
EFIAPI
InitMpmSerialIo (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS            Status;
  EFI_HANDLE            SerialHandle = NULL;
  MPM_SERIAL_IO_DEV     *SerialIoDev;
  BOOLEAN               IsTaskPending;
  EFI_EVENT             StartTcrEvent = NULL;
  VOID                  *Registration;

  AGESA_TESTPOINT (TpMpmSerialIoEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", __FUNCTION__);
  if (PcdGetBool (PcdMpmWirelessTcrEnable) == FALSE) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }


  // Check MPM Enable
  if (!IsMpmEnabled()) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = BiosMpmIsTextRedirectPendingTask (&IsTaskPending);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("GetPending Task Fail\n");
    goto Exit;
  }
  //Exit the driver, if no pending TCR request
  if (IsTaskPending == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("No pending TCR request\n");
    goto Exit;
  }

  // allocate private data
  SerialIoDev = &mMpmSerialIoDev;
  gBS->SetMem(SerialIoDev, sizeof(MPM_SERIAL_IO_DEV), 0x00);

  // setup device instance
  SerialIoDev->Signature = MPM_SERIAL_IO_DEV_SIGNATURE;

  SerialIoDev->SerialIo = mSerialIoTemplate;
  SerialIoDev->SerialIo.Mode = &mSerialIoMode;
  SerialIoDev->SerialDevicePath = mSerialDevicePath;

  SerialIoDev->SerialDevicePath.Uart.BaudRate = PcdGet64 (PcdUartDefaultBaudRate);
  SerialIoDev->SerialDevicePath.Uart.DataBits = PcdGet8 (PcdUartDefaultDataBits);
  SerialIoDev->SerialDevicePath.Uart.Parity   = PcdGet8 (PcdUartDefaultParity);
  SerialIoDev->SerialDevicePath.Uart.StopBits = PcdGet8 (PcdUartDefaultStopBits);

  SerialIoDev->SerialIo.Mode->BaudRate = PcdGet64 (PcdUartDefaultBaudRate);
  SerialIoDev->SerialIo.Mode->DataBits = (UINT32) PcdGet8 (PcdUartDefaultDataBits);
  SerialIoDev->SerialIo.Mode->Parity   = (UINT32) PcdGet8 (PcdUartDefaultParity);
  SerialIoDev->SerialIo.Mode->StopBits = (UINT32) PcdGet8 (PcdUartDefaultStopBits);
  SerialIoDev->SerialIo.Mode->ReceiveFifoDepth = (UINT32)PcdGet16 (PcdUartDefaultReceiveFifoDepth);

  Status = BiosMpmGetTxrTxBufferAddrSize (
                                          &SerialIoDev->TxBuffer,
                                          &SerialIoDev->TxBufferSize);
  if (EFI_ERROR(Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmGetTxrTxBufferAddrSize Fail\n");
    goto Exit;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SerialIoDev->TxBuffer = 0x%lx, Size = 0x%x\n", (UINT64*)SerialIoDev->TxBuffer, SerialIoDev->TxBufferSize);
  ZeroMem (SerialIoDev->TxBuffer, SerialIoDev->TxBufferSize);
  Status = BiosMpmGetTxrRxBufferAddrSize (
                                          &SerialIoDev->RxBuffer,
                                          &SerialIoDev->RxBufferSize);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmGetTxrRxBufferAddrSize Fail\n");
    goto Exit;
  }
  ZeroMem (SerialIoDev->RxBuffer, SerialIoDev->RxBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("SerialIoDev->RxBuffer = 0x%lx, Size = 0x%x\n", (UINT64*)SerialIoDev->RxBuffer, SerialIoDev->RxBufferSize);
  //
  // Issue a reset to initialize the Serial Port
  //
  Status = SerialIoDev->SerialIo.Reset(&SerialIoDev->SerialIo);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SerialIo.Reset Fail\n");
    goto Exit;
  }

  SerialIoDev->SerialControl = EFI_SERIAL_DATA_TERMINAL_READY \
                                | EFI_SERIAL_REQUEST_TO_SEND \
                                | EFI_SERIAL_CARRIER_DETECT \
                                | EFI_SERIAL_RING_INDICATE \
                                | EFI_SERIAL_DATA_SET_READY \
                                | EFI_SERIAL_OUTPUT_BUFFER_EMPTY \
                                | EFI_SERIAL_INPUT_BUFFER_EMPTY;

  IDS_HDT_CONSOLE_PSP_TRACE ("MpmSerialIoDriverStart SerialDevicePath: %s\n",
                      ConvertDevicePathToText ((EFI_DEVICE_PATH *)&SerialIoDev->SerialDevicePath, FALSE, FALSE));

  StartTcrEvent = EfiCreateProtocolNotifyEvent (
                    &gAmdMpmWifiFwloadedProtocolGuid,
                    TPL_CALLBACK,
                    MpmStartTcrCallBack,
                    NULL,
                    &Registration
                  );
  if (StartTcrEvent == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("MpmSerialIo create event failed\n");
    goto Exit;
  }
  //
  // Make a new handle with Serial IO protocol and its device path on it.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &SerialHandle,
                  &gEfiSerialIoProtocolGuid,
                  &SerialIoDev->SerialIo,
                  &gEfiDevicePathProtocolGuid,
                  &SerialIoDev->SerialDevicePath,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("InstallMultipleProtocolInterfaces Fail\n");
    goto Exit;
  }

  RegisterMpmBiosCmdTimerEvent();
Exit:
  AGESA_TESTPOINT (TpMpmSerialIoExit, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmSerialIo Exit: %r\n", Status);
  return Status;
}
