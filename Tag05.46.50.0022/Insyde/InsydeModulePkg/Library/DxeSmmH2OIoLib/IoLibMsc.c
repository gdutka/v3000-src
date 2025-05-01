/** @file
  I/O Library. This file has compiler specifics for Microsft C as there is no
  ANSI C standard for doing IO.

  MSC - uses intrinsic functions and the optimize will remove the function call
  overhead.

  We don't advocate putting compiler specifics in libraries or drivers but there
  is no other way to make this work.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/



#include "DxeSmmH2OIoLib.h"
H2O_DXE_SMM_IO_LIB_PROTOCOL *mDxeSmmIoLibProtocol;

//
// _ReadWriteBarrier() forces memory reads and writes to complete at the point
// in the call. This is only a hint to the compiler and does emit code.
// In past versions of the compiler, _ReadWriteBarrier was enforced only
// locally and did not affect functions up the call tree. In Visual C++
// 2005, _ReadWriteBarrier is enforced all the way up the call tree.
//

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
  UINT8                             Value;

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = (UINT8)mDxeSmmIoLibProtocol->IoRead(Port, SMM_IO_UINT8);
  return Value;
}

/**
  Writes an 8-bit I/O port.

  Writes the 8-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @return The value written to the I/O port.

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
  return Value;
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
  UINT16                            Value;

  ASSERT ((Port & 1) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = (UINT16)mDxeSmmIoLibProtocol->IoRead(Port, SMM_IO_UINT16);
  return Value;
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

  @return The value written to the I/O port.

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

  return Value;
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
  UINT32                            Value;

  ASSERT ((Port & 3) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = (UINT32)mDxeSmmIoLibProtocol->IoRead(Port, SMM_IO_UINT32);
  return Value;
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

  @return The value written to the I/O port.

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

  mDxeSmmIoLibProtocol->IoWrite(Port, SMM_IO_UINT32, Value);
  return Value;
}
