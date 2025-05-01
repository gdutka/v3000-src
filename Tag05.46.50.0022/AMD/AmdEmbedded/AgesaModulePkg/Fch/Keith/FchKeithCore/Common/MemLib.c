/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_COMMON_MEMLIB_FILECODE


/**
 * ReadMem - Read FCH BAR Memory
 *
 * @param[in] Address    - Memory BAR address
 * @param[in] OpFlag     - Access width
 * @param[in] *ValuePtr  - In/Out value pointer
 *
 */
VOID
ReadMem (
  IN  UINT32     Address,
  IN  UINT8      OpFlag,
  IN  VOID       *ValuePtr
  )
{
  OpFlag = OpFlag & 0x7f;

  switch ( OpFlag ) {
  case AccessWidth8:
    *((UINT8*)ValuePtr) = *((volatile UINT8*) ((UINTN)Address));
    break;

  case AccessWidth16:
    *((UINT16*)ValuePtr) = *((volatile UINT16*) ((UINTN)Address));
    break;

  case AccessWidth32:
    *((UINT32*)ValuePtr) = *((volatile UINT32*) ((UINTN)Address));
    break;

  default:
    ASSERT (FALSE);
    break;
  }
}

/**
 * WriteMem - Write FCH BAR Memory
 *
 * @param[in] Address    - Memory BAR address
 * @param[in] OpFlag     - Access width
 * @param[in] *ValuePtr  - In/Out Value pointer
 *
 */
VOID
WriteMem (
  IN  UINT32     Address,
  IN  UINT8      OpFlag,
  IN  VOID       *ValuePtr
  )
{
  OpFlag = OpFlag & 0x7f;

  switch ( OpFlag ) {
  case AccessWidth8 :
    *((volatile UINT8*) ((UINTN)Address)) = *((UINT8*)ValuePtr);
    break;

  case AccessWidth16:
    *((volatile UINT16*) ((UINTN)Address)) = *((UINT16*)ValuePtr);
    break;

  case AccessWidth32:
    *((volatile UINT32*) ((UINTN)Address)) = *((UINT32*)ValuePtr);
    break;

  default:
    ASSERT (FALSE);
    break;
  }
}

/**
 * RwMem - Read & Write FCH BAR Memory
 *
 * @param[in] Address    - Memory BAR address
 * @param[in] OpFlag     - Access width
 * @param[in] Mask       - Mask Value of data
 * @param[in] Data       - Write data
 *
 */
VOID
RwMem (
  IN  UINT32     Address,
  IN  UINT8      OpFlag,
  IN  UINT32     Mask,
  IN  UINT32     Data
  )
{
  UINT32 Result;

  ReadMem (Address, OpFlag, &Result);
  Result = (Result & Mask) | Data;
  WriteMem (Address, OpFlag, &Result);
  ReadMem (Address, OpFlag, &Result);
}


