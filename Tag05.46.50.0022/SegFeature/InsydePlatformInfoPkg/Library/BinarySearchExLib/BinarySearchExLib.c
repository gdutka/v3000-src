/** @file
  Binary search routines

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

#include <Base.h>

#include <Library/DebugLib.h>
#include <Library/BinarySearchExLib.h>

/**
  Finds an element equals to Key in a sorted array. If the element is not
  found, returns the index indicating where the element would reside if the
  element were inserted into the array.

  @param[in]  Key
  @param[in]  Array
  @param[in]  NumOfElements
  @param[in]  ElementSize
  @param[in]  CompareFunction
  @param[in]  Context
  @param[out] ArrayIndex

  @retval RETURN_SUCCESS
  @retval RETURN_NOT_FOUND
  @retval RETURN_INVALID_PARAMETER

**/
RETURN_STATUS
BinarySearchEx (
  IN CONST VOID               *Key,
  IN CONST VOID               *Array,
  IN UINTN                    NumOfElements,
  IN UINTN                    ElementSize,
  IN SEARCH_COMPARE_FUNCTION  CompareFunction,
  IN VOID                     *Context OPTIONAL,
  OUT UINTN                   *ArrayIndex OPTIONAL
  )
{
  UINTN       StartIndex;
  UINTN       EndIndex;
  UINTN       Index;
  CONST VOID  *Element;
  INTN        CompareResult;

  if (ArrayIndex != NULL) {
    *ArrayIndex = 0;
  }

  StartIndex = 0;
  EndIndex   = NumOfElements;
  while (StartIndex < EndIndex) {
    if ((Array == NULL) || (CompareFunction == NULL)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a() - The Array/CompareFunction is NULL, please check input Param\n",
        __FUNCTION__
        ));
      return RETURN_INVALID_PARAMETER;
    }
    Index = StartIndex + (EndIndex - StartIndex) / 2;
    Element = (CONST VOID *) ((CONST UINT8 *) Array + Index * ElementSize);
    //
    // Note: In C89, a bsearch() comparison function expects Key as first
    // argument, and a pointer to an element in Array as second argument.
    //
    CompareResult = CompareFunction (Key, Element, Context);
    if (CompareResult > 0) {
      StartIndex = ++Index;
    } else if (CompareResult < 0) {
      EndIndex = Index;
    }
    if (ArrayIndex != NULL) {
      *ArrayIndex = Index;
    }
    if (CompareResult == 0) {
      return RETURN_SUCCESS;
    }
  }
  return RETURN_NOT_FOUND;
}

VOID *
BinarySearch (
  IN CONST VOID               *Key,
  IN CONST VOID               *Array,
  IN UINTN                    NumOfElements,
  IN UINTN                    ElementSize,
  IN SEARCH_COMPARE_FUNCTION  CompareFunction,
  IN VOID                     *Context OPTIONAL
  )
{
  RETURN_STATUS  Status;
  UINTN          Index;

  Status = BinarySearchEx (
             Key,
             Array,
             NumOfElements,
             ElementSize,
             CompareFunction,
             Context,
             &Index
             );
  if (RETURN_ERROR (Status)) {
    return NULL;
  }
  return (VOID *) ((CONST UINT8 *) Array + Index * ElementSize);
}
