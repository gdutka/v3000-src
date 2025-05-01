/** @file

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

#include <Library/H2ODebugTraceErrorLevel.h>
#include <Library/PcdLib.h>

/**
  This function returns whether the H2O_DEBUG_TRACE_OEM_SVC 
  bit is set from PcdH2ODebugTraceErrorLevel.

  @retval TRUE   H2O_DEBUG_TRACE_OEM_SVC bit is set.
  @retval FALSE  H2O_DEBUG_TRACE_OEM_SVC bit isn't set.
**/
BOOLEAN
DebugOemSvcEnable (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet32(PcdH2ODebugTraceErrorLevel) & (UINT32) H2O_DEBUG_TRACE_OEM_SVC) != 0);
}

/**
  This function returns whether the H2O_DEBUG_TRACE_CP 
  bit is set from PcdH2ODebugTraceErrorLevel.

  @retval TRUE   H2O_DEBUG_TRACE_CP bit is set.
  @retval FALSE  H2O_DEBUG_TRACE_CP bit isn't set.
**/
BOOLEAN
DebugCpEnable (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet32(PcdH2ODebugTraceErrorLevel) & (UINT32) H2O_DEBUG_TRACE_CP) != 0);
}