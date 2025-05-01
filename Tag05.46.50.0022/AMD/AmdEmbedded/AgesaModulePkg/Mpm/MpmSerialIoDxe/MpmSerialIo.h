/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef MPM_SERIAL_IO_H
#define MPM_SERIAL_IO_H

#include <Uefi/UefiSpec.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MpmLib.h>
#include <Library/MpmFuncLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdPspMboxLibV2.h>

#include <Protocol/SerialIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Guid/SerialPortLibVendor.h>

#define MPM_SERIAL_IO_DEV_SIGNATURE       SIGNATURE_32 ('M', 'S', 'I', 'D')
#define WireLessFlag 1

typedef struct {
  VENDOR_DEVICE_PATH        Guid;
  UART_DEVICE_PATH          Uart;
  EFI_DEVICE_PATH_PROTOCOL  End;
} SERIAL_DEVICE_PATH;


typedef struct {
  UINTN                     Signature;

  EFI_UNICODE_STRING_TABLE  *ControllerNameTable;

  EFI_HANDLE                Handle;
  EFI_SERIAL_IO_PROTOCOL    SerialIo;

  SERIAL_DEVICE_PATH        SerialDevicePath;

  UINT32                    SerialControl;

  VOID                      *TxBuffer;
  UINT32                    TxBufferSize;
  VOID                      *RxBuffer;
  UINT32                    RxBufferSize;
} MPM_SERIAL_IO_DEV;

#define MPM_SERIAL_IO_DEV_FROM_THIS(a)  CR (a, MPM_SERIAL_IO_DEV, SerialIo, MPM_SERIAL_IO_DEV_SIGNATURE)

#define COMMAND_REGISTER_OFFSET 0x4
#define COMMAND_REGISTER_VALUE  0x7
#define CLASS_CODE_REGISTER_OFFSET 0x8
#define HOST_BRIDGE_CLASS_CODE 0x06040000
#define SUBORDINATE_BUS_NUMBER 0x1A

static MPM_SERIAL_IO_DEV  mMpmSerialIoDev;

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
);

/**
  Read one byte data from MPM via PSP MpmMbox interface

  @param  This          Protocol instance pointer.
  @param  [in] *Data   The Data read from MPM

  @retval EFI_SUCCESS
  @retval EFI_NOT_READY

**/
EFI_STATUS
MpmTextReadyOneByte (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINT8   *Data
);

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );


#endif /* MPM_SERIAL_IO_H */
