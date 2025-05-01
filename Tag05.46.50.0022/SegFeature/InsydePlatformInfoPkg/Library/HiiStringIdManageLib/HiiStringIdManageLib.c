/** @file


;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>               // MdeModulePkg
#include <Library/HiiStringIdManageLib.h> // InsydePlatformInfoPkg

#include "HiiStringIdManagePrivateData.h"

//
// Override ALIGN_VALUE() macro that was defined in <Base.h>
// The Alignment parameter must be a power of two. Our code is shorter.
//
#undef ALIGN_VALUE
#define ALIGN_VALUE(Value, Alignment)  (((Value) + ((Alignment) - 1)) & ~((Alignment) - 1))

//
// gRotateBufferMinAddressAlignment is a compile time constant.
//
STATIC CONST UINT8  gRotateBufferMinAddressAlignment = sizeof (UINT16);

STATIC
VOID
DebugPrintStringIdList (
  IN UINTN                ErrorLevel,
  IN CONST EFI_STRING_ID  *StringIdList,
  IN UINTN                StringIdListLength
  )
{
  CHAR8  StringBuffer[81];
  UINTN  Index;
  UINTN  Offset;

  if ((!DebugPrintEnabled ()) || (!DebugPrintLevelEnabled (ErrorLevel))) {
    return;
  }

  if (StringIdList == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The StringIdList is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  for (Index = 0; Index < StringIdListLength; Index++) {
    Offset = (Index % 16 == 0) ? 0 : ((Index % 16) * 5 - 1);
    AsciiSPrint (
      StringBuffer + Offset,
      sizeof (StringBuffer) - Offset,
      "%a%04x\n",
      (Index % 16 == 0) ? "" : ((Index % 16 == 8) ? "-" : " "),
      (unsigned int) StringIdList[Index]
      );

    if (Index % 16 == 15 || Index == StringIdListLength - 1) {
      DEBUG ((ErrorLevel, StringBuffer));
    }
  }
}

STATIC
UINT16
InternalGetUnusedStringIdCount (
  IN CONST HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData
  )
{
  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  if (PrivateData->StringIdPoolLength < PrivateData->UnusedStringIdStartIndex) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Error!! StringIdPoolLength (0x%x) Less than UnusedStringIdStartIndex (0x%x)\n",
      __FUNCTION__,
      PrivateData->StringIdPoolLength,
      PrivateData->UnusedStringIdStartIndex
      ));
    return 0;
  }

  return (PrivateData->StringIdPoolLength -
    PrivateData->UnusedStringIdStartIndex);
}

VOID
DebugPrintStringIdManagePrivateData (
  IN UINTN                                    ErrorLevel,
  IN CONST HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData
  )
{
  UINT16  UnusedStringIdCount;
  UINT16  *StartIndices;
  UINTN   Index;

  if ((!DebugPrintEnabled ()) || (!DebugPrintLevelEnabled (ErrorLevel))) {
    return;
  }

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  UnusedStringIdCount = InternalGetUnusedStringIdCount (PrivateData);
  DEBUG ((
    ErrorLevel,
    "HiiStringIdManagePrivateData\n"
    "HII handle: %p\n"
    "Number of forms: %u\n"
    "Total number of string IDs: %u\n"
    "Number of string IDs unused: %u\n",
    PrivateData->HiiHandle,
    (unsigned int) PrivateData->NumberOfForms,
    (unsigned int) PrivateData->StringIdPoolLength,
    (unsigned int) UnusedStringIdCount
    ));

  if (PrivateData->StringIdPool == NULL) {
    return;
  }

  DebugPrintStringIdList (
    ErrorLevel,
    &PrivateData->StringIdPool[PrivateData->UnusedStringIdStartIndex],
    UnusedStringIdCount
    );

  StartIndices = (UINT16 *) (PrivateData + 1);

  for (Index = 0; Index < PrivateData->NumberOfForms; Index++) {
    ASSERT (*(StartIndices + Index - 1) >= StartIndices[Index]);
    DEBUG ((
      ErrorLevel,
      "Form %u, number of string IDs: %u\n",
      (unsigned int) Index,
      (unsigned int) (*(StartIndices + Index - 1) - StartIndices[Index])
      ));
    DebugPrintStringIdList (
      ErrorLevel,
      &PrivateData->StringIdPool[StartIndices[Index]],
      *(StartIndices + Index - 1) - StartIndices[Index]
      );
  }
}

/**

  @param[in]  HiiHandle
  @param[in]  NumberOfForms

  @retval NULL
  @retval Others

**/
HII_STRING_ID_MANAGER
CreateHiiStringIdManager (
  IN EFI_HII_HANDLE  HiiHandle,
  IN UINT16          NumberOfForms
  )
{
  UINTN                              AllocateSize;
  UINT8                              *Buffer;
  HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData;

  ASSERT (HiiHandle != NULL);
  ASSERT (NumberOfForms > 0);

  AllocateSize = sizeof (HII_STRING_ID_MANAGE_PRIVATE_DATA) +
                   NumberOfForms * sizeof (UINT16);
  Buffer = AllocateZeroPool (AllocateSize);
  if (Buffer == NULL) {
    return NULL;
  }

  PrivateData = (HII_STRING_ID_MANAGE_PRIVATE_DATA *) Buffer;

  PrivateData->HiiHandle     = HiiHandle;
  PrivateData->NumberOfForms = NumberOfForms;
  ASSERT (PrivateData->StringIdPool == NULL);
  ASSERT (PrivateData->StringIdPoolLength == 0);
  ASSERT (PrivateData->UnusedStringIdStartIndex == 0);

  return (HII_STRING_ID_MANAGER) PrivateData;
}

/**

  @param[in]  HiiStringIdManager

  @return Void.

**/
VOID
DestroyHiiStringIdManager (
  IN HII_STRING_ID_MANAGER  HiiStringIdManager
  )
{
  HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData;

  if (HiiStringIdManager == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiStringIdManager is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  PrivateData = (HII_STRING_ID_MANAGE_PRIVATE_DATA *) HiiStringIdManager;

  if (PrivateData->StringIdPool != NULL) {
    FreePool (PrivateData->StringIdPool);
  }
  FreePool (HiiStringIdManager);
}

/**

  @param[in]  HiiStringIdManager
  @param[in]  FormIndex
  @param[out] DynamicStringCount
  @param[out] StringIdList

  @return Void.

**/
VOID
GetFormDynamicStringCount (
  IN CONST HII_STRING_ID_MANAGER  HiiStringIdManager,
  IN UINT16                       FormIndex,
  OUT UINT16                      *DynamicStringCount,
  OUT CONST EFI_STRING_ID         **StringIdList
  )
{
  HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData;
  UINT16                             *StartIndices;

  if ((HiiStringIdManager == NULL) || (DynamicStringCount == NULL) || (StringIdList == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiStringIdManager/DynamicStringCount/StringIdList is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  *StringIdList = NULL;

  PrivateData = (HII_STRING_ID_MANAGE_PRIVATE_DATA *) HiiStringIdManager;
  StartIndices = (UINT16 *) (PrivateData + 1);

  ASSERT (StartIndices - 1 == &PrivateData->UnusedStringIdStartIndex);

  ASSERT (FormIndex < PrivateData->NumberOfForms);

  ASSERT (*(StartIndices + FormIndex - 1) >= StartIndices[FormIndex]);
  *DynamicStringCount =
    *(StartIndices + FormIndex - 1) - StartIndices[FormIndex];

  if (*(StartIndices + FormIndex - 1) > StartIndices[FormIndex]) {
    *StringIdList = &PrivateData->StringIdPool[StartIndices[FormIndex]];
  }
}

/**

  @param[in]  HiiStringIdManager

  @return

**/
UINT16
GetUnusedStringIdCount (
  IN CONST HII_STRING_ID_MANAGER  HiiStringIdManager
  )
{
  HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData;

  if (HiiStringIdManager == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiStringIdManager is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  PrivateData = (HII_STRING_ID_MANAGE_PRIVATE_DATA *) HiiStringIdManager;

  return InternalGetUnusedStringIdCount (PrivateData);
}

/**

  @param[in,out] PrivateData
  @param[in]     NewPoolLength

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
ExpandStringIdPool (
  IN OUT HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData,
  IN UINT32                                 NewPoolLength
  )
{
  CHAR8          *Languages;
  EFI_STATUS     Status;
  UINT8          EfiAllocatePoolAlignment;
  UINTN          OldPoolSize;
  EFI_STRING_ID  *NewPool;
  UINTN          NewPoolSize;
  UINTN          CharIndex;
  EFI_STRING_ID  StringId;

  if (PrivateData->StringIdPoolLength >= NewPoolLength) {
    return EFI_SUCCESS;
  }

  Languages = HiiGetSupportedLanguages (PrivateData->HiiHandle);
  if (Languages == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // If the specified NewPoolLength is too large, or when ReallocatePool fails,
  // this function will still allocate string IDs in case the caller function/
  // form needs "as many string IDs as possible up to the specified length".
  //

  Status = EFI_SUCCESS;

  EfiAllocatePoolAlignment = 8;

  OldPoolSize = ALIGN_VALUE (
                  PrivateData->StringIdPoolLength * sizeof (*NewPool),
                  EfiAllocatePoolAlignment
                  );
  if (NewPoolLength > MAX_UINT16) {
    Status = EFI_OUT_OF_RESOURCES;
    NewPoolLength = MAX_UINT16;
  }
  NewPoolSize = ALIGN_VALUE (
                  NewPoolLength * sizeof (*NewPool),
                  EfiAllocatePoolAlignment
                  );
  ASSERT (OldPoolSize <= NewPoolSize);
  if (OldPoolSize < NewPoolSize) {
    NewPool = ReallocatePool (
                OldPoolSize,
                NewPoolSize,
                PrivateData->StringIdPool
                );
    if (NewPool != NULL) {
      ASSERT (IsZeroBuffer (((UINT8 *) NewPool) + OldPoolSize, NewPoolSize - OldPoolSize));
      PrivateData->StringIdPool = NewPool;
    } else {
      Status = EFI_OUT_OF_RESOURCES;
      NewPoolLength = (UINT32) (OldPoolSize / sizeof (*NewPool));
    }
  }

  //
  // Create string for one language.
  //
  // HiiSetString() from EDK II HiiLib could leak a string ID when creating a
  // string for multiple languages but not all languages for the string are set
  // successfully.
  //
  CharIndex = 0;
  while (Languages[CharIndex] != '\0' && Languages[CharIndex] != ';') {
    CharIndex++;
  }
  Languages[CharIndex] = '\0';

  while (PrivateData->StringIdPoolLength < NewPoolLength) {
    StringId = HiiSetString (
                 PrivateData->HiiHandle,
                 STRING_TOKEN (0x0000),
                 (EFI_STRING) L"",
                 Languages
                 );
    if (StringId == 0x0000) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    PrivateData->StringIdPool[PrivateData->StringIdPoolLength] = StringId;
    ++(PrivateData->StringIdPoolLength);
  }

  FreePool (Languages);
  return Status;
}

/**
  @param[in,out] StartOfBuffer
  @param[in]     OffsetToMiddle
  @param[in]     OffsetToEnd

  @return Void.

**/
STATIC
VOID
RotateBuffer (
  IN OUT VOID  *StartOfBuffer,
  IN UINTN     OffsetToMiddle,
  IN UINTN     OffsetToEnd
  )
{
  UINT8        MinAddressAlignment;
  UINTN        SwapOffset;
  UINT8        *SwapPtr;
  CONST UINT8  *EndPtr;
  UINTN        TempN;
  UINT32       Temp32;
  UINT16       Temp16;
  UINT8        Temp8;

  if (StartOfBuffer == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The StartOfBuffer is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // MinAddressAlignment is a compile time constant.
  //
  MinAddressAlignment = gRotateBufferMinAddressAlignment;
  if (MinAddressAlignment < 1) {
    MinAddressAlignment = 1;
  }
  MinAddressAlignment =
    (MinAddressAlignment ^ (MinAddressAlignment - 1)) / 2 + 1;

  //
  // Assert buffer is non null and aligned.
  //
  ASSERT (((UINTN) StartOfBuffer & (MinAddressAlignment - 1)) == 0);
  ASSERT ((OffsetToMiddle & (MinAddressAlignment - 1)) == 0);
  ASSERT ((OffsetToEnd & (MinAddressAlignment - 1)) == 0);

  while (OffsetToMiddle > 0 && OffsetToMiddle < OffsetToEnd) {
    if (OffsetToMiddle <= OffsetToEnd / 2) {
      SwapOffset = OffsetToMiddle;
      SwapPtr = (UINT8 *) StartOfBuffer;

      StartOfBuffer = ((UINT8 *) StartOfBuffer) + SwapOffset;
      OffsetToEnd = OffsetToEnd - OffsetToMiddle;
    } else {
      SwapOffset = OffsetToEnd - OffsetToMiddle;
      SwapPtr = ((UINT8 *) StartOfBuffer) + OffsetToMiddle - SwapOffset;

      OffsetToEnd = OffsetToMiddle;
      OffsetToMiddle = OffsetToMiddle - SwapOffset;
    }

    EndPtr = SwapPtr + SwapOffset;
    while (SwapPtr < EndPtr) {
      if (MinAddressAlignment >= sizeof (UINTN)) {
        TempN = *(UINTN *) SwapPtr;
        *(UINTN *) SwapPtr = *(UINTN *) (SwapPtr + SwapOffset);
        *(UINTN *) (SwapPtr + SwapOffset) = TempN;
        SwapPtr = SwapPtr + sizeof (TempN);
        continue;
      }

      if (MinAddressAlignment >= sizeof (UINT32)) {
        Temp32 = *(UINT32 *) SwapPtr;
        *(UINT32 *) SwapPtr = *(UINT32 *) (SwapPtr + SwapOffset);
        *(UINT32 *) (SwapPtr + SwapOffset) = Temp32;
        SwapPtr = SwapPtr + sizeof (Temp32);
        continue;
      }

      if (MinAddressAlignment >= sizeof (UINT16)) {
        Temp16 = *(UINT16 *) SwapPtr;
        *(UINT16 *) SwapPtr = *(UINT16 *) (SwapPtr + SwapOffset);
        *(UINT16 *) (SwapPtr + SwapOffset) = Temp16;
        SwapPtr = SwapPtr + sizeof (Temp16);
        continue;
      }

      Temp8 = *(UINT8 *) SwapPtr;
      *(UINT8 *) SwapPtr = *(UINT8 *) (SwapPtr + SwapOffset);
      *(UINT8 *) (SwapPtr + SwapOffset) = Temp8;
      SwapPtr = SwapPtr + sizeof (Temp8);
      continue;
    }
  }
}

/**

  @param[in,out] PrivateData
  @param[in]     FormIndex
  @param[in]     StringCountAdjustment

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES

**/
STATIC
EFI_STATUS
InternalAdjustFormStringCount (
  IN OUT HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData,
  IN UINT16                                 FormIndex,
  IN INT32                                  StringCountAdjustment
  )
{
  UINT16      *StartIndices;
  UINT32      NewPoolLength;
  EFI_STATUS  Status;
  UINTN       RotateStartIndex;
  UINTN       RotateMidIndex;
  UINTN       RotateEndIndex;
  UINTN       Index;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (StringCountAdjustment == 0) {
    return EFI_SUCCESS;
  }

  StartIndices = (UINT16 *) (PrivateData + 1);

  if (StringCountAdjustment > 0) {
    NewPoolLength =
      PrivateData->UnusedStringIdStartIndex + (UINT32) StringCountAdjustment;
    Status = ExpandStringIdPool (PrivateData, NewPoolLength);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    ASSERT (NewPoolLength <= MAX_UINT16);
    RotateStartIndex = *(StartIndices + FormIndex - 1);
    RotateMidIndex = PrivateData->UnusedStringIdStartIndex - RotateStartIndex;
    RotateEndIndex = NewPoolLength - RotateStartIndex;
  } else {
    RotateMidIndex = (UINTN) -StringCountAdjustment;
    RotateStartIndex = *(StartIndices + FormIndex - 1) - RotateMidIndex;
    RotateEndIndex = PrivateData->UnusedStringIdStartIndex - RotateStartIndex;
  }

  RotateBuffer (
    &PrivateData->StringIdPool[RotateStartIndex],
    RotateMidIndex * sizeof (*PrivateData->StringIdPool),
    RotateEndIndex * sizeof (*PrivateData->StringIdPool)
    );

  //
  // StartIndices[FormIndex] value remains unchanged.
  //
  for (Index = 0; Index <= FormIndex; Index++) {
    ASSERT ((INT32) *(StartIndices + Index - 1) + StringCountAdjustment >= 0);
    *(StartIndices + Index - 1) =
      (UINT16) ((INT32) *(StartIndices + Index - 1) + StringCountAdjustment);
  }

  return EFI_SUCCESS;
}

/**

  @param[in,out] HiiStringIdManager
  @param[in]     FormIndex
  @param[in]     DynamicStringCount
  @param[out]    StringIdList

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
SetFormDynamicStringCount (
  IN OUT HII_STRING_ID_MANAGER  HiiStringIdManager,
  IN UINT16                     FormIndex,
  IN UINT16                     DynamicStringCount,
  OUT CONST EFI_STRING_ID       **StringIdList OPTIONAL
  )
{
  HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData;
  UINT16                             *StartIndices;
  INT32                              Difference;
  EFI_STATUS                         Status;

  if (StringIdList != NULL) {
    *StringIdList = NULL;
  }

  if (HiiStringIdManager == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiStringIdManager is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (HII_STRING_ID_MANAGE_PRIVATE_DATA *) HiiStringIdManager;
  StartIndices = (UINT16 *) (PrivateData + 1);

  ASSERT (StartIndices - 1 == &PrivateData->UnusedStringIdStartIndex);

  ASSERT (FormIndex < PrivateData->NumberOfForms);

  ASSERT (*(StartIndices + FormIndex - 1) >= StartIndices[FormIndex]);
  Difference = (INT32) DynamicStringCount + (INT32) StartIndices[FormIndex] -
                 (INT32) *(StartIndices + FormIndex - 1);

  Status = InternalAdjustFormStringCount (PrivateData, FormIndex, Difference);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (StringIdList != NULL &&
      *(StartIndices + FormIndex - 1) > StartIndices[FormIndex])
  {
    *StringIdList = &PrivateData->StringIdPool[StartIndices[FormIndex]];
  }

  return EFI_SUCCESS;
}

/**

  @param[in,out] HiiStringIdManager
  @param[in]     FormIndex

  @retval 0
  @retval Others

**/
EFI_STRING_ID
AllocateStringIdForForm (
  IN OUT HII_STRING_ID_MANAGER  HiiStringIdManager,
  IN UINT16                     FormIndex
  )
{
  HII_STRING_ID_MANAGE_PRIVATE_DATA  *PrivateData;
  UINT16                             *StartIndices;
  EFI_STATUS                         Status;

  if (HiiStringIdManager == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiStringIdManager is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0x0000;
  }

  PrivateData = (HII_STRING_ID_MANAGE_PRIVATE_DATA *) HiiStringIdManager;
  StartIndices = (UINT16 *) (PrivateData + 1);

  ASSERT (StartIndices - 1 == &PrivateData->UnusedStringIdStartIndex);

  ASSERT (FormIndex < PrivateData->NumberOfForms);

  Status = InternalAdjustFormStringCount (PrivateData, FormIndex, 1);
  if (EFI_ERROR (Status)) {
    return 0x0000;
  }

  ASSERT (*(StartIndices + FormIndex - 1) >= 1);
  return PrivateData->StringIdPool[*(StartIndices + FormIndex - 1) - 1];
}
