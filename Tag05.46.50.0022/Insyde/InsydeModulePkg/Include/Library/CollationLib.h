/** @file
  Provides interface to Unicode Collation protocol functions.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _COLLATION_LIB_H
#define _COLLATION_LIB_H
  
/**
  Check if strings are equal using Unicode Collation.
   
  @param[in] Str1                The first null-terminated string.
  @param[in] Str2                The second null-terminated string.
   
  @retval 0                      Strings are equal.
  @retval 1                      Str1 is lexically greater than Str2
  @retval -1                     Str1 is lexically less than Str2
  @retval -2                     Locate UnicodeCollation protocol fail.
**/
INT32 
CollationStriColl (
   IN CONST CHAR16 *Str1,
   IN CONST CHAR16 *Str2
   );

/**
  Check whether the string matches the pattern, using Unicode Collation

  @param[in] Str                 The string.
  @param[in] Pattern             The pattern.
   
  @retval    TRUE                Match
  @retval    FALSE               No match.
**/
BOOLEAN
CollationMetaiMatch(
  IN CONST CHAR16 *Str,
  IN CONST CHAR16 *Pattern
  );

/**
  Convert a string to all lower case characters.

  @param[in,out]   Str                 The string.
   
**/
 VOID
 CollationStrLwr(
   IN OUT CHAR16 *Str
   );

/**
  Convert a string to all upper case characters.
   
  @param[in,out]   Str                 The string.
   
**/
 VOID
 CollationStrUpr(
   IN OUT CHAR16 *Str
   );
    
#endif
