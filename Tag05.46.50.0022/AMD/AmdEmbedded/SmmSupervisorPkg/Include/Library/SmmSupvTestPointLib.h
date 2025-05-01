/** @file
 * SmmSupv Post Code definition

Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SMMSUPV_TEST_POINT_LIB_H_
#define _SMMSUPV_TEST_POINT_LIB_H_

#include <SmmSupvPostCode.h>

/// <PostCodePrefix> 0xB0005000
#define SMMSUPV_POST_CODE_PREFIX 0xB0005000ul
#define SMMSUPV_DEBUG_PORT       0x80

VOID
SmmSupvTestPoint (
  IN  SMMSUPV_POST_CODE  TestPoint
  );
/**
 *  Make a Progress Report to the User.
 *
 *  This Macro is always enabled. The default action is to write the TestPoint value
 *  to an I/O port. The I/O port is 8 bits in size and the default address is 0x80.
 *
 *  @param[in]      TestPoint       The value for display indicating progress
 *
 **/

#define TESTPOINT(TestPoint) SmmSupvTestPoint ((TestPoint))
#endif  // _SMMSUPV_TEST_POINT_LIB_H_


