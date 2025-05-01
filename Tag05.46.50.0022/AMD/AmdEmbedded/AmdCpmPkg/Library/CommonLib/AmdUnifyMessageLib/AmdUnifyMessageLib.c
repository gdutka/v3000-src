/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include "AmdUnifyMessageLib.h"

extern AMD_UNIFY_MESSAGE_INSTANCE  AmdUnifyMessagePort80;
extern AMD_UNIFY_MESSAGE_INSTANCE  AmdUnifyMessageEspiUart;
extern AMD_UNIFY_MESSAGE_INSTANCE  AmdUnifyMessageFchUart;

AMD_UNIFY_MESSAGE_INSTANCE* AmdUnifyMessageInstances[] = {
  &AmdUnifyMessagePort80,
  &AmdUnifyMessageEspiUart,
  &AmdUnifyMessageFchUart,
  NULL
};

BOOLEAN
AumPassMessage (
  UINT8   MessageId,
  ...
  )
{
  VA_LIST     Args;

  AMD_UNIFY_MESSAGE_INSTANCE           **pAumMessagePtr;
  pAumMessagePtr = &AmdUnifyMessageInstances[0];

  for (UINT8 i = 0; pAumMessagePtr[i] != NULL; i++) {
    if (! pAumMessagePtr[i]->Support()) continue;
    VA_START (Args, MessageId);

    switch (MessageId) {
      case AMD_UNIFY_MESSAGE_INITIALIZE:
        pAumMessagePtr[i]->Initialize();
        break;
      case AMD_UNIFY_MESSAGE_WRITE:
        {
          UINT8     *Buffer;
          UINTN     NumberOfBytes;
          Buffer = VA_ARG (Args, UINT8 *);
          NumberOfBytes = VA_ARG (Args, UINTN);
          pAumMessagePtr[i]->Write(Buffer, NumberOfBytes);
        }
        break;
      case AMD_UNIFY_MESSAGE_READ:
        {
          UINT8     *Buffer;
          UINTN     NumberOfBytes;
          Buffer = VA_ARG (Args, UINT8 *);
          NumberOfBytes = VA_ARG (Args, UINTN);
          pAumMessagePtr[i]->Read(Buffer, NumberOfBytes);
        }
        break;
      case AMD_UNIFY_MESSAGE_POLL:
        pAumMessagePtr[i]->Poll();
        break;
      case AMD_UNIFY_MESSAGE_SETCONTROL:
        break;
      case AMD_UNIFY_MESSAGE_GETCONTROL:
        break;
      case AMD_UNIFY_MESSAGE_SETATTRIBUTES:
        break;
      default:
        break;
    }
    VA_END(Args);
  }
  return TRUE;
}

RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_INITIALIZE);
  return RETURN_SUCCESS;
}

UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_WRITE, Buffer, NumberOfBytes);
  return NumberOfBytes;
}

UINTN
EFIAPI
SerialPortRead (
  OUT UINT8   *Buffer,
  IN  UINTN   NumberOfBytes
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_READ, Buffer, NumberOfBytes);
  return NumberOfBytes;
}

BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_POLL);
  return TRUE;
}

RETURN_STATUS
EFIAPI
SerialPortSetControl (
  IN UINT32 Control
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_SETCONTROL);
  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
SerialPortGetControl (
  OUT UINT32 *Control
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_GETCONTROL);
  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
SerialPortSetAttributes (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits
  )
{
  AumPassMessage(AMD_UNIFY_MESSAGE_SETATTRIBUTES);
  return RETURN_SUCCESS;
}

