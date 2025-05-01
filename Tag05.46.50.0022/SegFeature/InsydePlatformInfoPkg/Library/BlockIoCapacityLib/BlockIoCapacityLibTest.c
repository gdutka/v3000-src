/** @file
  

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

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BlockIoCapacityLib.h>

/**
  Function to test the data correctness of Uint128ToDecimalString().

  If any test data fails, then ASSERT().

  @return  Void.

**/
VOID
Uint128ToDecimalStringTest (
  VOID
  )
{
  CHAR8  Str[UINT128_STR_MAX_SIZE];

  //
  //                     (Lower               Upper,              Str)
  //
  Uint128ToDecimalString (0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, Str);
  ASSERT (AsciiStrCmp (Str, "340282366920938463463374607431768211455") == 0);
  Uint128ToDecimalString (0x1C9E66C000000000, 0xE1B1E5F90F944D6E, Str);
  ASSERT (AsciiStrCmp (Str, "300000000000000000000000000000000000000") == 0);
  Uint128ToDecimalString (0x1C9E66BFFFFFFFFF, 0xE1B1E5F90F944D6E, Str);
  ASSERT (AsciiStrCmp (Str, "299999999999999999999999999999999999999") == 0);
  Uint128ToDecimalString (0xB34B9F1000000000, 0x00C097CE7BC90715, Str);
  ASSERT (AsciiStrCmp (Str,   "1000000000000000000000000000000000000") == 0);
  Uint128ToDecimalString (0xB34B9F0FFFFFFFFF, 0x00C097CE7BC90715, Str);
  ASSERT (AsciiStrCmp (Str,    "999999999999999999999999999999999999") == 0);
  Uint128ToDecimalString (0xFFFFFFFFFFFFFFFF, 0x00C097CE7BBFFFFF, Str);
  ASSERT (AsciiStrCmp (Str,    "999999999989086210756867854899871743") == 0);
  Uint128ToDecimalString (0x0000000000000000, 0x0000000000000001, Str);
  ASSERT (AsciiStrCmp (Str,                    "18446744073709551616") == 0);
  Uint128ToDecimalString (0xFFFFFFFFFFFFFFFF, 0x0000000000000000, Str);
  ASSERT (AsciiStrCmp (Str,                    "18446744073709551615") == 0);
  Uint128ToDecimalString (0x0DE0B6B3A7640000, 0x0000000000000000, Str);
  ASSERT (AsciiStrCmp (Str,                     "1000000000000000000") == 0);
  Uint128ToDecimalString (0x0DE0B6B3A763FFFF, 0x0000000000000000, Str);
  ASSERT (AsciiStrCmp (Str,                      "999999999999999999") == 0);
  Uint128ToDecimalString (0x0000000000000000, 0x0000000000000000, Str);
  ASSERT (AsciiStrCmp (Str,                                       "0") == 0);
}
