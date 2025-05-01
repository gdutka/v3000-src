/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include "CbsHookPeiLib.h"
#include <Ppi/AmdSocPcdInitPpi.h>
#include <Library/AmdBaseLib.h>

typedef struct _CBS_CONFIG AMD_SYSTEM_CONFIGURATION;

STATIC PEI_CBS_HOOK_PPI mCbsHookInstance =
{
  CbsHookFunc
};

EFI_GUID SystemConfigurationGuid = AMD_SYSTEM_CONFIGURATION_GUID;

STATIC EFI_PEI_PPI_DESCRIPTOR mCbsHookInstancePpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCbsHookPpiGuid,
  &mCbsHookInstance
};

EFI_STATUS
EFIAPI
AgesaPcdInitReadyCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_PEI_NOTIFY_DESCRIPTOR   mAgesaPcdInitReadyCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPcdInitReadyPpiGuid,
  AgesaPcdInitReadyCallback
};

VOID CbsVariableLoadDefault(
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableService,
  UINT8 *pVariableData
)
{
  EFI_STATUS   Status;
  CBS_CONFIG   TempVariable;
  UINT8        *tempBuffer;
  UINT32       RecordOffset;
  UINTN        VariableSize;

  tempBuffer = (UINT8*)&TempVariable;
  VariableSize = 0;
  Status = VariableService->GetVariable (VariableService,L"AmdSetup", &SystemConfigurationGuid, NULL, &VariableSize, pVariableData);
  if ((Status == EFI_BUFFER_TOO_SMALL) && (VariableSize == sizeof (AMD_SYSTEM_CONFIGURATION))) {
    Status = VariableService->GetVariable (VariableService,L"AmdSetup", &SystemConfigurationGuid, NULL, &VariableSize, pVariableData);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "No CBS Variable, Force loading CBS variable default value.(Status=0x%x)\n", Status));
      CbsWriteDefalutValue(pVariableData);
    } else {
      CbsWriteDefalutValue(tempBuffer);
      DEBUG ((EFI_D_ERROR, "Found CBS Variable: oID=0x%08x, nID=0x%08x, Offset = 0x%08x\n",
              (UINT32) *(UINT32 *)pVariableData,
              (UINT32) *(UINT32 *)tempBuffer,
              (UINT32) *(UINT32*)(pVariableData + sizeof(UINT32))
              ));
      if ((UINT32) *(UINT32 *)pVariableData != (UINT32) *(UINT32 *)tempBuffer) {
        RecordOffset = (UINT32) *(UINT32*)(pVariableData + sizeof(UINT32));
        if ((RecordOffset > 32) && (RecordOffset < sizeof (CBS_CONFIG))) {
          LibAmdMemCopy ((VOID *)&tempBuffer[32], (VOID *)&pVariableData[32], RecordOffset - 32, NULL);
        }
        LibAmdMemCopy ((VOID *)pVariableData, (VOID *)tempBuffer, sizeof (CBS_CONFIG), NULL);
      }
    }
  } else {
      DEBUG ((EFI_D_ERROR, "No CBS Variable, Force loading CBS variable default value.(Status=0x%x)\n", Status));
      CbsWriteDefalutValue(pVariableData);
  }
}

EFI_STATUS
CbsGetHobData (
   OUT   CBS_CONFIG   **CbsVariable
  )
{
  EFI_STATUS             Status;
  EFI_PEI_HOB_POINTERS   Hob;

  Status = EFI_NOT_FOUND;

  PeiServicesGetHobList ((VOID **)&Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
        CompareGuid ( &Hob.Guid->Name, &gAmdCbsVariableHobGuid)) {
      *CbsVariable = (CBS_CONFIG*) (Hob.Raw + sizeof (EFI_HOB_GENERIC_HEADER) + sizeof (EFI_GUID));
      Status = EFI_SUCCESS;
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  return Status;
}

EFI_STATUS
CbsHookFunc (
  IN       EFI_PEI_SERVICES     **PeiServices,
  IN OUT   VOID                 *AMD_PARAMS,
  IN       UINTN                IdsHookId
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  CBS_CONFIG                      CbsVariable;
  EFI_STATUS                      Status;
  CBS_CONFIG                      *CbsVariableHob;
  PEI_CBS_COMPONENT_STRUCT        *pCbsFunctionTable;
  PEI_CBS_FUNCTION_STRUCT         *pCurTable;
  CPUID_DATA                      CpuId;
  UINT64                          SocFamilyID;

  CbsVariableHob = NULL;
  (**PeiServices).SetMem (&CbsVariable, sizeof (AMD_SYSTEM_CONFIGURATION), 0);
  Status = CbsGetHobData (&CbsVariableHob);
  if (Status == EFI_SUCCESS) {
    LibAmdMemCopy ((VOID *)&CbsVariable,
                   (VOID *)CbsVariableHob,
                   sizeof (CBS_CONFIG),
                   NULL
                   );
    DEBUG ((EFI_D_ERROR, "CbsHookFunc:: Get CBS Variable from HOB(IdsHookId=0x%x)\n", IdsHookId));
  } else {
    Status = (**PeiServices).LocatePpi (PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, &Variable);
    if (!EFI_ERROR(Status)) {
      CbsVariableLoadDefault (Variable, (UINT8*) &CbsVariable);
    } else {
      CbsWriteDefalutValue((UINT8 *) &CbsVariable);
    }
    CbsComboIdentify ((UINT8 *) &CbsVariable);
    BuildGuidDataHob (&gAmdCbsVariableHobGuid, (VOID *)&CbsVariable, sizeof (CBS_CONFIG));
    DEBUG ((EFI_D_ERROR, "CbsHookFunc:: Create CBS Variable HOB, CBS_UID=0x%x\n", CbsVariable.Header.CbsVariableStructUniqueValue));
  }
  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  SocFamilyID = CpuId.EAX_Reg & RAW_FAMILY_ID_MASK;

  pCbsFunctionTable = &CbsCommonFuncTable[0];
  while (pCbsFunctionTable->pFuncTable != NULL) {
    if ((pCbsFunctionTable->Family == SocFamilyID) || (pCbsFunctionTable->Family == 0xFFFFFFFF)) {
      // Dispatch CBS function in PEI_CBS_FUNCTION_STRUCT.
      pCurTable = pCbsFunctionTable->pFuncTable;
      while (pCurTable->CBSFuncPtr != NULL) {
        if ((pCurTable->IdsHookId == IdsHookId) &&
            ((pCurTable->Family == SocFamilyID) || (pCurTable->Family == 0xFFFFFFFF))) {
          pCurTable->CBSFuncPtr(PeiServices, AMD_PARAMS, &CbsVariable);
        }
        pCurTable++;
      }
    }
    pCbsFunctionTable++;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AgesaPcdInitReadyCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  EFI_STATUS                      Status;
  CBS_CONFIG                      CbsVariable;
  CBS_CONFIG                      *CbsVariableHob;

  (**PeiServices).SetMem (&CbsVariable, sizeof (AMD_SYSTEM_CONFIGURATION), 0);
  Status = CbsGetHobData (&CbsVariableHob);
  if (Status == EFI_SUCCESS) {
    LibAmdMemCopy ((VOID *)&CbsVariable,
                   (VOID *)CbsVariableHob,
                   sizeof (CBS_CONFIG),
                   NULL
                   );
    DEBUG ((EFI_D_ERROR, "AgesaPcdInitReadyCallback:: Get CBS Variable from HOB\n"));
  } else {
    Status = (**PeiServices).LocatePpi (PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, &Variable);
    if (!EFI_ERROR(Status)) {
      CbsVariableLoadDefault (Variable, (UINT8*)&CbsVariable);
    } else {
      CbsWriteDefalutValue((UINT8 *)&CbsVariable);
    }
  }

  //Call Auto Gen code to set AGESA PCDs
  CbsSetAgesaPcds (&CbsVariable);
  return (EFI_SUCCESS);
}

EFI_STATUS CbsInitEntry(
  IN CONST  EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS   Status;

  Status = (**PeiServices).InstallPpi (PeiServices, &mCbsHookInstancePpiList);
  Status = (**PeiServices).NotifyPpi (PeiServices, &mAgesaPcdInitReadyCallback);

  return Status;
}

EFI_STATUS
CbsInterfaceFunc (
  IN  EFI_PEI_SERVICES       **PeiServices,
  IN  OUT VOID               *AMD_PARAMS,
  IN      UINTN              IdsHookId
  )
{
  EFI_STATUS          Status;
  PEI_CBS_HOOK_PPI    *CbsHookPtr;
  
  Status = EFI_SUCCESS;
  if (PeiServices) {
    Status = (*PeiServices)->LocatePpi (
                               PeiServices,
                               &gAmdCbsHookPpiGuid,
                               0,
                               NULL,
                               &CbsHookPtr
                               );
    if (!EFI_ERROR (Status)) {
      Status = CbsHookPtr->CbsHookInterface (PeiServices, AMD_PARAMS, IdsHookId);
    }
  }
  return Status;
}

