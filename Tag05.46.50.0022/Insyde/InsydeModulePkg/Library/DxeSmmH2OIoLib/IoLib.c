/** @file
  Common I/O Library routines.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DxeSmmH2OIoLib.h"
/**
  Reads a 64-bit I/O port.

  Reads the 64-bit I/O port specified by Port. The 64-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.

  If 64-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 64-bit boundary, then ASSERT().

  @param  Port  The I/O port to read.

  @return The value read.

**/
UINT64
EFIAPI
IoRead64 (
  IN      UINTN                     Port
  )
{
  ASSERT (FALSE);
  return 0;
}

/**
  Writes a 64-bit I/O port.

  Writes the 64-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.

  If 64-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 64-bit boundary, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @return The value written the I/O port.

**/
UINT64
EFIAPI
IoWrite64 (
  IN      UINTN                     Port,
  IN      UINT64                    Value
  )
{
  ASSERT (FALSE);
  return 0;
}


/**
  Reads an 8-bit MMIO register.

  Reads the 8-bit MMIO register specified by Address. The 8-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 8-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT8
EFIAPI
MmioRead8 (
  IN      UINTN                     Address
  )
{
  UINT8                             Value;

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = (UINT8)mDxeSmmIoLibProtocol->MmioRead(Address, SMM_IO_UINT8);
  return Value;
}

/**
  Writes an 8-bit MMIO register.

  Writes the 8-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 8-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

  @return Value.

**/
UINT8
EFIAPI
MmioWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  )
{
  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->MmioWrite(Address, SMM_IO_UINT8, Value);
  return Value;
}

/**
  Reads a 16-bit MMIO register.

  Reads the 16-bit MMIO register specified by Address. The 16-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 16-bit MMIO register operations are not supported, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT16
EFIAPI
MmioRead16 (
  IN      UINTN                     Address
  )
{
  UINT16                            Value;

  ASSERT ((Address & 1) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = (UINT16)mDxeSmmIoLibProtocol->MmioRead(Address, SMM_IO_UINT16);
  return Value;
}

/**
  Writes a 16-bit MMIO register.

  Writes the 16-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 16-bit MMIO register operations are not supported, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

  @return Value.

**/
UINT16
EFIAPI
MmioWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                    Value
  )
{
  ASSERT ((Address & 1) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->MmioWrite(Address, SMM_IO_UINT16, Value);
  return Value;
}

/**
  Reads a 32-bit MMIO register.

  Reads the 32-bit MMIO register specified by Address. The 32-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 32-bit MMIO register operations are not supported, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT32
EFIAPI
MmioRead32 (
  IN      UINTN                     Address
  )
{
  UINT32                            Value;

  ASSERT ((Address & 3) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = (UINT32)mDxeSmmIoLibProtocol->MmioRead(Address, SMM_IO_UINT32);
  return Value;
}

/**
  Writes a 32-bit MMIO register.

  Writes the 32-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 32-bit MMIO register operations are not supported, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

  @return Value.

**/
UINT32
EFIAPI
MmioWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
  ASSERT ((Address & 3) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->MmioWrite(Address, SMM_IO_UINT32, Value);
  return Value;
}

/**
  Reads a 64-bit MMIO register.

  Reads the 64-bit MMIO register specified by Address. The 64-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 64-bit MMIO register operations are not supported, then ASSERT().
  If Address is not aligned on a 64-bit boundary, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT64
EFIAPI
MmioRead64 (
  IN      UINTN                     Address
  )
{
  UINT64                            Value;

  ASSERT ((Address & 7) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  Value = mDxeSmmIoLibProtocol->MmioRead(Address, SMM_IO_UINT64);

  return Value;
}

/**
  Writes a 64-bit MMIO register.

  Writes the 64-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 64-bit MMIO register operations are not supported, then ASSERT().
  If Address is not aligned on a 64-bit boundary, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
UINT64
EFIAPI
MmioWrite64 (
  IN      UINTN                     Address,
  IN      UINT64                    Value
  )
{
  ASSERT ((Address & 7) == 0);

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->MmioWrite((UINT64)Address, SMM_IO_UINT64, Value);
  return Value;
}


/**
  Reads an 8-bit I/O port fifo into a block of memory.

  Reads the 8-bit I/O fifo port specified by Port.
  The port is read Count times, and the read data is
  stored in the provided Buffer.

  This function must guarantee that all I/O read and write operations are
  serialized.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port    The I/O port to read.
  @param  Count   The number of times to read I/O port.z`
  @param  Buffer  The buffer to store the read data into.

**/

VOID
EFIAPI
IoReadFifo8 (
  IN      UINTN                     Port,
  IN      UINTN                     Count,
  OUT     VOID                      *Buffer
  )
{
  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->IoReadFifo(Port, SMM_IO_UINT8, Count, Buffer);
}


/**
  Writes a block of memory into an 8-bit I/O port fifo.

  Writes the 8-bit I/O fifo port specified by Port.
  The port is written Count times, and the write data is
  retrieved from the provided Buffer.

  This function must guarantee that all I/O write and write operations are
  serialized.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port    The I/O port to write.
  @param  Count   The number of times to write I/O port.
  @param  Buffer  The buffer to retrieve the write data from.

**/
VOID
EFIAPI
IoWriteFifo8 (
  IN      UINTN                     Port,
  IN      UINTN                     Count,
  IN      VOID                      *Buffer
  )
{
  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }
  
  mDxeSmmIoLibProtocol->IoWriteFifo(Port, SMM_IO_UINT8, Count, Buffer);
}


/**
  Reads a 16-bit I/O port fifo into a block of memory.

  Reads the 16-bit I/O fifo port specified by Port.
  The port is read Count times, and the read data is
  stored in the provided Buffer.

  This function must guarantee that all I/O read and write operations are
  serialized.

  If 16-bit I/O port operations are not supported, then ASSERT().

  @param  Port    The I/O port to read.
  @param  Count   The number of times to read I/O port.
  @param  Buffer  The buffer to store the read data into.

**/
VOID
EFIAPI
IoReadFifo16 (
  IN      UINTN                     Port,
  IN      UINTN                     Count,
  OUT     VOID                      *Buffer
  )
{
  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->IoReadFifo(Port, SMM_IO_UINT16, Count, Buffer);
}


/**
  Writes a block of memory into a 16-bit I/O port fifo.

  Writes the 16-bit I/O fifo port specified by Port.
  The port is written Count times, and the write data is
  retrieved from the provided Buffer.

  This function must guarantee that all I/O write and write operations are
  serialized.

  If 16-bit I/O port operations are not supported, then ASSERT().

  @param  Port    The I/O port to write.
  @param  Count   The number of times to write I/O port.
  @param  Buffer  The buffer to retrieve the write data from.

**/
VOID
EFIAPI
IoWriteFifo16 (
  IN      UINTN                     Port,
  IN      UINTN                     Count,
  IN      VOID                      *Buffer
  )
{
  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }
  
  mDxeSmmIoLibProtocol->IoWriteFifo(Port, SMM_IO_UINT16, Count, Buffer);
}


/**
  Reads a 16-bit I/O port fifo into a block of memory.

  Reads the 16-bit I/O fifo port specified by Port.
  The port is read Count times, and the read data is
  stored in the provided Buffer.

  This function must guarantee that all I/O read and write operations are
  serialized.

  If 16-bit I/O port operations are not supported, then ASSERT().

  @param  Port    The I/O port to read.
  @param  Count   The number of times to read I/O port.
  @param  Buffer  The buffer to store the read data into.

**/
VOID
EFIAPI
IoReadFifo32 (
  IN      UINTN                     Port,
  IN      UINTN                     Count,
  OUT     VOID                      *Buffer
  )
{
  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }

  mDxeSmmIoLibProtocol->IoReadFifo(Port, SMM_IO_UINT32, Count, Buffer);
}


/**
  Writes a block of memory into a 16-bit I/O port fifo.

  Writes the 16-bit I/O fifo port specified by Port.
  The port is written Count times, and the write data is
  retrieved from the provided Buffer.

  This function must guarantee that all I/O write and write operations are
  serialized.

  If 16-bit I/O port operations are not supported, then ASSERT().

  @param  Port    The I/O port to write.
  @param  Count   The number of times to write I/O port.
  @param  Buffer  The buffer to retrieve the write data from.

**/
VOID
EFIAPI
IoWriteFifo32 (
  IN      UINTN                     Port,
  IN      UINTN                     Count,
  IN      VOID                      *Buffer
  )
{

  if (!mDxeSmmIoLibProtocol) {
    ASSERT (FALSE);
  }
  
  mDxeSmmIoLibProtocol->IoWriteFifo(Port, SMM_IO_UINT32, Count, Buffer);
}

