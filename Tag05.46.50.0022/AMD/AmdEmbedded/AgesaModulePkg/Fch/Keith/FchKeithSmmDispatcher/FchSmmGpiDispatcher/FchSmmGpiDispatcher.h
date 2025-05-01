/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#ifndef _FCH_SMM_GPI_DISPATCHER_H_
#define _FCH_SMM_GPI_DISPATCHER_H_

#include "FchSmmDispatcher.h"

#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/FchSmmGpiDispatch2.h>

#define MAX_GPI_VALUE       BIT31

extern  FCH_SMM_GPI_DISPATCH2_PROTOCOL gFchSmmGpiDispatch2Protocol;
extern  EFI_SMM_GPI_DISPATCH2_PROTOCOL gEfiSmmGpiDispatch2Protocol;


EFI_STATUS
EFIAPI
FchSmmGpiDispatchHandler (
  IN      EFI_HANDLE     SmmImageHandle,
  IN OUT  VOID           *CommunicationBuffer OPTIONAL,
  IN OUT  UINTN          *SourceSize OPTIONAL
  );

///
/// GPI SMI Node
///
typedef struct _FCH_SMM_GPI_NODE {
  EFI_HANDLE                              DispatchHandle;         ///< Dispatch Hangle
  EFI_SMM_GPI_REGISTER_CONTEXT            Context;                ///< Register context
  FCH_SMM_GPI_HANDLER_ENTRY_POINT2        CallBack2Function;      ///< SMM handler entry point 2
  struct _FCH_SMM_GPI_NODE                *FchGpiNodePtr;         ///< pointer to next node
} FCH_SMM_GPI_NODE;

extern  FCH_SMM_GPI_NODE                *HeadFchSmmGpiNodePtr;
#endif

