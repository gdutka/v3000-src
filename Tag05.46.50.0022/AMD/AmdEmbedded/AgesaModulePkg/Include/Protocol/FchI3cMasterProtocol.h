/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef __I3C_MASTER_PROTOCOL_H__
#define __I3C_MASTER_PROTOCOL_H__

#include <Protocol/FchI3cProtocol.h>

#define EFI_DXE_I3C_MASTER_PROTOCOL_GUID \
  { 0xc0368b99, 0x4703, 0x4883, { 0x82, 0x1d, 0x28, 0xbc, 0xd7, 0x63, 0x9f, 0x8b }}

typedef struct _EFI_DXE_I3C_MASTER_PROTOCOL EFI_DXE_I3C_MASTER_PROTOCOL;

/**
  Set the frequency for the I3C clock line.

  @param This                   Pointer to an EFI_DXE_I3C_MASTER_PROTOCOL structure.
  @param BusClockHertz          Pointer to the requested I3C bus clock frequency in Hertz.
                                Upon return this value contains the actual frequency
                                in use by the I3C controller.

  @retval EFI_SUCCESS           The bus frequency was set successfully.
  @retval EFI_INVALID_PARAMETER BusClockHertz is NULL
  @retval EFI_UNSUPPORTED       The controller does not support this frequency.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DXE_I3C_MASTER_PROTOCOL_SET_BUS_FREQUENCY) (
  IN EFI_DXE_I3C_MASTER_PROTOCOL   *This,
  IN UINTN                    *BusClockHertz
  );

/**
  Reset the I3C controller and configure it for use.

  @param  This                  Pointer to an EFI_DXE_I3C_MASTER_PROTOCOL structure.

  @retval EFI_SUCCESS           The reset completed successfully.
  @retval EFI_DEVICE_ERROR      The reset operation failed.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DXE_I3C_MASTER_PROTOCOL_RESET) (
  IN CONST EFI_DXE_I3C_MASTER_PROTOCOL  *This
  );

/**
  Start an I3C transaction on the host controller.

  @param  This                   Pointer to an EFI_DXE_I3C_MASTER_PROTOCOL structure.
  @param  SlaveAddress           Address of the device on the I3C bus.
                                 Set the I3C_ADDRESSING_10_BIT when using 10-bit addresses,
                                 clear this bit for 7-bit addressing.
                                 Bits 0-6 are used for 7-bit I3C slave addresses and
                                 bits 0-9 are used for 10-bit I3C slave addresses.
  @param  RequestPacket          Pointer to an EFI_I3C_REQUEST_PACKET structure describing the I3C transaction.

  @retval EFI_SUCCESS             The transaction completed successfully.
  @retval EFI_BAD_BUFFER_SIZE     The RequestPacket->LengthInBytes value is too large.
  @retval EFI_DEVICE_ERROR       There was an I3C error (NACK) during the transaction.
  @retval EFI_INVALID_PARAMETER   RequestPacket is NULL
  @retval EFI_NO_RESPONSE         The I3C device is not responding to the slave address.
                                 EFI_DEVICE_ERROR will be returned if the controller cannot distinguish when the NACK occurred.
  @retval EFI_NOT_FOUND           Reserved bit set in the SlaveAddress parameter
  @retval EFI_OUT_OF_RESOURCES   Insufficient memory for I3C transaction
  @retval EFI_UNSUPPORTED         The controller does not support the requested transaction.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DXE_I3C_MASTER_PROTOCOL_START_REQUEST) (
  IN CONST EFI_DXE_I3C_MASTER_PROTOCOL     *This,
  IN UINTN                            SlaveAddress,
  IN EFI_I3C_REQUEST_PACKET           *RequestPacket
  );

///
/// This PROTOCOL manipulates the I3C host controller to perform transactions as a master on the I3C bus
/// using the current state of any switches or multiplexers in the I3C bus.
///
struct _EFI_DXE_I3C_MASTER_PROTOCOL {
  EFI_DXE_I3C_MASTER_PROTOCOL_SET_BUS_FREQUENCY   SetBusFrequency;
  EFI_DXE_I3C_MASTER_PROTOCOL_RESET               Reset;
  EFI_DXE_I3C_MASTER_PROTOCOL_START_REQUEST       StartRequest;
  CONST EFI_I3C_CONTROLLER_CAPABILITIES      *I3cControllerCapabilities;
  EFI_GUID                                   Identifier;
  UINT32 ControllerNum;
};

extern EFI_GUID gAmdFchSNI3cDepexProtocolGuid;

#endif