/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Legacy Interrupt Library  Header File
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 */

#ifndef _LEGACY_INTERRUPT_LIB_H_
#define _LEGACY_INTERRUPT_LIB_H_



/**
  Get the number of PIRQs this hardware supports.

  @param  This                  The protocol instance pointer.
  @param  NumberPirsq           The number of PIRQs that are supported.

  @retval EFI_SUCCESS           The number of PIRQs was returned successfully.

**/
EFI_STATUS
AmdGetNumberPirqs (
  OUT UINT8                                   *NumberPirqs
  );

/**
  Gets the PCI location associated with this protocol.

  @param  This                  The Protocol instance pointer.
  @param  Bus                   The PCI Bus.
  @param  Device                The PCI Device.
  @param  Function              The PCI Function.

  @retval EFI_SUCCESS           The Bus, Device, and Function were returned successfully.

**/
EFI_STATUS
AmdGetLocation (
  OUT UINT8                                   *Bus,
  OUT UINT8                                   *Device,
  OUT UINT8                                   *Function
  );

/**
  Read the PIRQ register and return the data

  @param  This                  The protocol instance pointer.
  @param  PirqNumber            The PIRQ register to read.
  @param  PirqData              The data read.

  @retval EFI_SUCCESS           The data was read.
  @retval EFI_INVALID_PARAMETER Invalid PIRQ number.

**/
EFI_STATUS
AmdReadPirq (
  IN  UINT8                                  PirqNumber,
  OUT UINT8                                  *PirqData
  );

/**
  Write the specified PIRQ register with the given data.

  @param  This                  The protocol instance pointer.
  @param  PirqNumber            A PIRQ register to read.
  @param  PirqData              The data to write.

  @retval EFI_SUCCESS           The PIRQ was programmed.
  @retval EFI_INVALID_PARAMETER Invalid PIRQ number.

**/
EFI_STATUS
AmdWritePirq (
  IN  UINT8                                  PirqNumber,
  IN UINT8                                   PirqData
  );

#endif
