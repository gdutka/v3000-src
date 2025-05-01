/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/AmdCbsHookPpi.h>
#include <Library/AmdIdsHookExtLib.h>

typedef struct _CBS_IDS_HOOK_PARAMS {
  VOID *Handle;
  VOID *Data;
} CBS_IDS_HOOK_PARAMS;

typedef struct _CBS_IDS_HOOK_DATA_GETIDSNV {
  VOID *IdsNvTable;
  UINT32 *IdsNvTableSize;
} CBS_IDS_HOOK_DATA_GETIDSNV;

IDS_HOOK_STATUS
IdsHookExtEntry (
  HOOK_ID HookId,
  VOID *Handle,
  VOID *Data
  )
{
  CBS_IDS_HOOK_PARAMS  CbsIdsHookParams;
  EFI_STATUS           Status;
  EFI_PEI_SERVICES     **PeiServices;
  PEI_CBS_HOOK_PPI     *CbsHookPtr;

  CbsIdsHookParams.Handle = Handle;
  CbsIdsHookParams.Data = Data;
  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdCbsHookPpiGuid,
                             0,
                             NULL,
                             &CbsHookPtr
                             );
  if (!EFI_ERROR (Status)) {
    Status = CbsHookPtr->CbsHookInterface (PeiServices, &CbsIdsHookParams, HookId);
  }

  return (EFI_ERROR (Status) ? IDS_HOOK_ERROR : IDS_HOOK_SUCCESS);
}

IDS_HOOK_STATUS
GetIdsNvTable (
  IN OUT   VOID *IdsNvTable,
  IN OUT   UINT32 *IdsNvTableSize
  )
{
  CBS_IDS_HOOK_DATA_GETIDSNV IdsHookData;
  //Use the special reserved HOOK to get the IDS NV table
  IdsHookData.IdsNvTable = IdsNvTable;
  IdsHookData.IdsNvTableSize = IdsNvTableSize;

  return IdsHookExtEntry (IDS_HOOK_GET_NV_TABLE, NULL, &IdsHookData);
}

