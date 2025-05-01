/** @file
  I/O Library. This file has compiler specifics for GCC as there is no
  ANSI C standard for doing IO.

  GCC - uses EFIAPI assembler. __asm__ calls GAS. __volatile__ makes sure the
  compiler puts the assembler in this exact location. The complex GNUC
  operations are not optimzed. It would be possible to also write these
  with EFIAPI assembler.

  We don't advocate putting compiler specifics in libraries or drivers but there
  is no other way to make this work.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include "DxeSmmH2OIoLib.h"

/**
  Reads an 8-bit I/O port.

  Reads the 8-bit I/O port specified by Port. The 8-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to read.

  @return The value read.

**/
UINT8
EFIAPI
IoRead8 (
  IN      UINTN                     Port
  )
{
  UINT8   Data;

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Data = mDxeSmmIoLibProtocol->IoRead(Port, SMM_IO_UINT8);
  return Data;
}

/**
  Writes an 8-bit I/O port.

  Writes the 8-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @return The value written the I/O port.

**/
UINT8
EFIAPI
IoWrite8 (
  IN      UINTN                     Port,
  IN      UINT8                     Value
  )
{

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->IoWrite(Port, SMM_IO_UINT8, Value);
  return Value;;
}

/**
  Reads a 16-bit I/O port.

  Reads the 16-bit I/O port specified by Port. The 16-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.

  If 16-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 16-bit boundary, then ASSERT().

  @param  Port  The I/O port to read.

  @return The value read.

**/
UINT16
EFIAPI
IoRead16 (
  IN      UINTN                     Port
  )
{
  UINT16   Data;

  ASSERT ((Port & 1) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Data = mDxeSmmIoLibProtocol->IoRead(Port, SMM_IO_UINT16);
  return Data;
}

/**
  Writes a 16-bit I/O port.

  Writes the 16-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.

  If 16-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 16-bit boundary, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @return The value written the I/O port.

**/
UINT16
EFIAPI
IoWrite16 (
  IN      UINTN                     Port,
  IN      UINT16                    Value
  )
{
  ASSERT ((Port & 1) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->IoWrite(Port, SMM_IO_UINT16, Value);
  return Value;;
}

/**
  Reads a 32-bit I/O port.

  Reads the 32-bit I/O port specified by Port. The 32-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.

  If 32-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 32-bit boundary, then ASSERT().

  @param  Port  The I/O port to read.

  @return The value read.

**/
UINT32
EFIAPI
IoRead32 (
  IN      UINTN                     Port
  )
{
  UINT32   Data;

  ASSERT ((Port & 3) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Data = mDxeSmmIoLibProtocol->IoRead(Port, SMM_IO_UINT32);
  return Data;
}

/**
  Writes a 32-bit I/O port.

  Writes the 32-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.

  If 32-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 32-bit boundary, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @return The value written the I/O port.

**/
UINT32
EFIAPI
IoWrite32 (
  IN      UINTN                     Port,
  IN      UINT32                    Value
  )
{
  ASSERT ((Port & 3) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = mDxeSmmIoLibProtocol->IoWrite(Port, SMM_IO_UINT32, Value);
  return Value;
}

