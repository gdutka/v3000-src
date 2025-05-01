/** @file
  Provide variable cache related functions

;******************************************************************************
;* Copyright (c) 2015 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#include "VariableCache.h"

/**
  This function is used to disable variable SMM mode cache mechanism in protected mode or SMM mode.
**/
STATIC
VOID
DisableSmmModeVariableCache (
  VOID
  )
{
  //
  // Needn't disable cache at runtime if support SMM mode, due to always set variable through SMI at runtime.
  //
  if (!VariableAtRuntime () && mVariableModuleGlobal->SmmCodeReady && mSmst == NULL) {
    InitCommunicationBufferHeader ();
    mVariableModuleGlobal->SmmVarBuf->Signature = DISABLE_VARIABLE_CACHE_SIGNATURE;
    mVariableModuleGlobal->SmmVarBuf->AccessType = DISABLE_VARIABLE_CACHE_SMI_FUN_NUM;
    SendCommunicateBuffer ();
  } else if (mSmst != NULL) {
    mVariableModuleGlobal->NonVolatileVariableCache = NULL;
  }
  return;
}

/**
  This function is used to disable variable protected mode cache mechanism in protected mode or SMM mode.
**/
STATIC
VOID
DisableProtectedModeVariableCache (
  VOID
  )
{
  if (mSmst != NULL) {
    if (mSmmVariableGlobal->ProtectedModeVariableModuleGlobal != NULL) {
      mSmmVariableGlobal->ProtectedModeVariableModuleGlobal->NonVolatileVariableCache = NULL;
    }
  } else {
    mVariableModuleGlobal->NonVolatileVariableCache = NULL;
  }
  return;
}

/**
  This function is used to disable variable whole cache mechanism in protected mode or SMM mode.
**/
VOID
DisableVariableCache (
  VOID
  )
{
  DisableProtectedModeVariableCache ();
  DisableSmmModeVariableCache ();
  return;
}

/**
  This function uses to disable variable cache address

  @return EFI_SUCCESS    Disable all secure boot SMI functions successful.
  @return Other          Any error occurred while disabling all secure boot SMI functions successful.

**/
EFI_STATUS
SmmDisableVariableCache (
  VOID
  )
{

  SMM_VAR_BUFFER   *VariableBuffer;

  //
  // Check signature to prevent from other application disables cache.
  // If this situation occurred, it may cause cache data isn't the same between
  // protected mode and SMM mode. the worst case, system may write incorrect data to
  // variable store.
  //

  VariableBuffer = mVariableModuleGlobal->SmmVarBuf;
  if (VariableBuffer->Signature != DISABLE_VARIABLE_CACHE_SIGNATURE || VariableBuffer->DataSize != 0) {
    return EFI_UNSUPPORTED;
  }

  mVariableModuleGlobal->NonVolatileVariableCache = NULL;
  return EFI_SUCCESS;
}


/**
  Convert memory address saves non-volatile data to real non-volatile data address.

  @param Global                  VARIABLE_GLOBAL pointer
  @param PtrTrack                [in]: Current pointer to Variable track pointer structure in cache or non-volatile that contains variable information.
                                 [out]: Current pointer to Variable track pointer structure in non-volatile that contains variable information.
  @retval EFI_INVALID_PARAMETER  Input parameter is invalid.
  @retval EFI_SUCCESS            Convert address successful.
**/
EFI_STATUS
ConvertCacheAddressToPhysicalAddress (
  IN     VARIABLE_GLOBAL          *Global,
  IN OUT VARIABLE_POINTER_TRACK   *PtrTrack
  )
{
  if (PtrTrack == NULL || Global == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Needn't convert volatile, variable in HOB or address has been converted. Just return success.
  //
  if (PtrTrack->Volatile ||
      (UINTN) PtrTrack->StartPtr == Global->NonVolatileVariableBase + GetVariableStoreHeaderSize () ||
      (mVariableModuleGlobal->HobVariableBase != 0 &&
      (UINTN) PtrTrack->StartPtr == mVariableModuleGlobal->HobVariableBase + GetVariableStoreHeaderSize ())) {
    return EFI_SUCCESS;
  }

  ASSERT (mVariableModuleGlobal->NonVolatileVariableCache != NULL);
  PtrTrack->StartPtr = (VARIABLE_HEADER *) ((UINTN) Global->NonVolatileVariableBase + GetVariableStoreHeaderSize ());
  PtrTrack->EndPtr   = GetNonVolatileEndPointer ((VARIABLE_STORE_HEADER *) (UINTN) Global->NonVolatileVariableBase);
  if (PtrTrack->CurrPtr != NULL) {
    PtrTrack->CurrPtr = (VARIABLE_HEADER *) ((UINTN) Global->NonVolatileVariableBase +
                        (UINTN) PtrTrack->CurrPtr - (UINTN) mVariableModuleGlobal->NonVolatileVariableCache);
  }

  return EFI_SUCCESS;
}

/**
  This function uses to flush current non-volatile data to variable cache

  @param  CacheBaseAddress         pointer to variable cache base address.
  @param  NonVolatileBaseAddress  pointer to non-volatile base address.
  @param  CacheSize               Variable cache size.
  @param  LastVariableOffset      Pointer to save last variable offset.

  @return EFI_SUCCESS             Flush non-volatile data to variable cache successful.
  @return EFI_INVALID_PARAMETER   CacheBaseAddress or NonVolatileBaseAddress is NULL.
  @return EFI_BUFFER_TOO_SMALL    CacheSize is too small.

**/
EFI_STATUS
FlushVariableCache (
  IN  UINT8    *CacheBaseAddress,
  IN  UINT8    *NonVolatileBaseAddress,
  IN  UINTN    CacheSize,
  OUT UINTN    *LastVariableOffset
  )
{
  VARIABLE_HEADER                       *NextVariable;

  if (CacheBaseAddress == NULL || NonVolatileBaseAddress == NULL || LastVariableOffset == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NextVariable  = (VARIABLE_HEADER *) (NonVolatileBaseAddress + GetVariableStoreHeaderSize ());
  while (IsValidVariableHeader (NextVariable)) {
    NextVariable = GetNextVariablePtr (NextVariable);
  }

  *LastVariableOffset = (UINTN) NextVariable - (UINTN) NonVolatileBaseAddress;
  if (CacheSize < *LastVariableOffset) {
    return  EFI_BUFFER_TOO_SMALL;
  }

  SetMem (CacheBaseAddress, CacheSize, 0xff);
  CopyMem (CacheBaseAddress, NonVolatileBaseAddress, *LastVariableOffset);
  return EFI_SUCCESS;
}


