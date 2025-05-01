/** @file
  H2ODebugTraceErrorLevelLib include file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_DEBUG_TRACE_ERROR_LEVEL_LIB_H_
#define _H2O_DEBUG_TRACE_ERROR_LEVEL_LIB_H_

//
// Defined bits for PcdH2ODebugTraceErrorLevel
//
#define H2O_DEBUG_TRACE_OEM_SVC    0x00000001
#define H2O_DEBUG_TRACE_CP         0x00000002

BOOLEAN
DebugOemSvcEnable (
  VOID
  );

BOOLEAN
DebugCpEnable (
  VOID
  );

#endif