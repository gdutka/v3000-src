/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "MpmKvmMouse.h"
#include <Filecode.h>

#define FILECODE MPM_MPMKVM_MPMKVMMOUSE_FILECODE

KVM_MOUSE_DEV           mKvmMouseDev;
KVM_MOUSE_BUFFER        *mMRingBuffer = NULL;
UINT32                  gMpmMsWritePointer = 0;
UINT32                  gMpmMsReadPointer  = 0;

BOOLEAN
IsKvmMouseBufEmpty (
  IN  KVM_MOUSE_BUFFER        *MRingBuffer
  )
{
  gMpmMsWritePointer = MRingBuffer->WritePointer;
  return (BOOLEAN) (gMpmMsWritePointer == gMpmMsReadPointer);
}

EFI_STATUS
EFIAPI
InitializeMsRingBuffer (
  IN OUT KVM_MOUSE_BUFFER           **MRingBuffer
  )
{
  *MRingBuffer = mMRingBuffer;
  return EFI_SUCCESS;
}

VOID
EFIAPI
KvmGetNextPointer(
  IN OUT UINT32          *Pointer
  )
{
  gMpmMsReadPointer = (gMpmMsReadPointer + 1 == KVM_MOUSE_MAX_COUNT) ? 0 : gMpmMsReadPointer + 1;
  *Pointer = gMpmMsReadPointer;
}

