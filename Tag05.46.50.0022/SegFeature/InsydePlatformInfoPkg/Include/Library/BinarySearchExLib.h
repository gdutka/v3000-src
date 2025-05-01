/** @file
  Binary search routines

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef H2O_BINARY_SEARCH_EX_LIB_H_
#define H2O_BINARY_SEARCH_EX_LIB_H_

#include <Base.h>

typedef
INTN
(EFIAPI *SEARCH_COMPARE_FUNCTION) (
  IN CONST VOID  *Key,
  IN CONST VOID  *Element,
  IN VOID        *Context OPTIONAL
  );

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
  );

VOID *
BinarySearch (
  IN CONST VOID               *Key,
  IN CONST VOID               *Array,
  IN UINTN                    NumOfElements,
  IN UINTN                    ElementSize,
  IN SEARCH_COMPARE_FUNCTION  CompareFunction,
  IN VOID                     *Context OPTIONAL
  );

#endif // H2O_BINARY_SEARCH_EX_LIB_H_
