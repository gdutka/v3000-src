/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric IDS Hook implementation
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */

#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <IdsHookId.h>
#include <Filecode.h>

#define FILECODE LIBRARY_FABRICIDSHOOKRMBLIB_DXE_FABRICIDSHOOKRMBLIBDXE_FILECODE

#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define FABRIC_RMB_IDS_HOOKS_INT_DXE
#else
  #include "Internal/FabricIdsHookRmbLibIntDxe.h"
#endif

IDS_HOOK_ELEMENT FabricRmbIdsHooksDxe[] = {
  FABRIC_RMB_IDS_HOOKS_INT_DXE
  IDS_HOOKS_END
};

IDS_HOOK_TABLE FabricRmbIdsHookTableDxe = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  FabricRmbIdsHooksDxe
};


AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &FabricRmbIdsHookTableDxe;
  return AGESA_SUCCESS;
}


