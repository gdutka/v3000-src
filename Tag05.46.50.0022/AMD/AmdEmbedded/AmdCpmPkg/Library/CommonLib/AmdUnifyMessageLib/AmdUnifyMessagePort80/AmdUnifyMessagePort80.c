/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include "AmdUnifyMessageLib.h"

UINT8 *Port80LocalBuffer = "Port80 --- ";

BOOLEAN AumPort80Support (VOID) {
  if (AmdIdsEmulationAutoDetect()) {
    // Return TRUE if the system is emulation.
    return TRUE;
  }
  if (PcdGet32 (PcdAmdUnifyMessage) & PCD_AUM_PORT80) {
    return TRUE;
  }
  return FALSE;
}

RETURN_STATUS AumPort80Initialize (VOID) {
  UINT8       ByteCount;
  IoWrite32 (0x80, SIGNATURE_32 ('R','T','S','_'));

  for (ByteCount = 0; ByteCount < 11; ByteCount++) {
    IoWrite8 (0x80, Port80LocalBuffer[ByteCount]);
  }
  IoWrite32 (0x80, SIGNATURE_32  ('D','N','E','_'));

  return EFI_SUCCESS;
}

UINTN AumPort80Write (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
)
{
  UINTN ByteCount;

  if ((Buffer == NULL) || (NumberOfBytes == 0)) {
    return 0;
  }

  IoWrite32 (0x80, SIGNATURE_32 ('R','T','S','_'));

  ByteCount = NumberOfBytes;
  for (; ByteCount != 0; ByteCount--, Buffer++) {
    IoWrite8 (0x80, *Buffer);
  }

  IoWrite32 (0x80, SIGNATURE_32  ('D','N','E','_'));

  return NumberOfBytes;
}

UINTN AumPort80Read (
  UINT8     *Buffer,
  UINTN     NumberOfBytes
  )
{
    return 0;
}

BOOLEAN AumPort80Poll (VOID) {
  return TRUE;
}

RETURN_STATUS AumPort80SetControl (UINT32 Control) {
  return TRUE;
}

RETURN_STATUS AumPort80GetControl (UINT32 *Control){
  return TRUE;
}

RETURN_STATUS AumPort80SetAttributes (
  UINT64 *BaudRate,
  UINT32 *ReceiveFifoDepth,
  UINT32 *Timeout,
  EFI_PARITY_TYPE *Parity,
  UINT8 *DataBits,
  EFI_STOP_BITS_TYPE *StopBits
  )
{
  return TRUE;
}

AMD_UNIFY_MESSAGE_INSTANCE  AmdUnifyMessagePort80 =
{
  AumPort80Support,
  AumPort80Initialize,
  AumPort80Write,
  AumPort80Read,
  AumPort80Poll,
  AumPort80SetControl,
  AumPort80GetControl,
  AumPort80SetAttributes
};
