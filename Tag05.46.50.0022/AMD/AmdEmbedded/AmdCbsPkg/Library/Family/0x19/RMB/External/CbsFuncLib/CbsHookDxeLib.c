/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Uefi.h>
#include "PiDxe.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include "CbsHookDxeLib.h"
#include "AmdSoc.h"

typedef struct _CBS_CONFIG AMD_SYSTEM_CONFIGURATION;
extern EFI_RUNTIME_SERVICES *gRT;

CBS_CONFIG *pCbsConfig;
static CBS_DXE_PROTOCOL mCbsProtocolInstance=
{
  CbsHookFunc,
  0
};

EFI_STATUS
CbsGetHobData (
   OUT   CBS_CONFIG   **CbsVariable
  )
{
  EFI_STATUS          Status;
  VOID                *HobList;
  VOID                *HobData;

  HobList = NULL;
  //
  // Get data from HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (!EFI_ERROR (Status)) {
    HobData = GetNextGuidHob (&gAmdCbsVariableHobGuid, HobList);
    if (HobData != NULL) {
      *CbsVariable = (CBS_CONFIG *) GET_GUID_HOB_DATA (HobData);
    } else {
      Status = EFI_NOT_FOUND;
    }
  }

  return Status;
}

VOID
CbsVariableLoadDefault(
  UINT8 *pVariableData
  )
{
  EFI_STATUS   Status;
  UINT32       RecordOffset;
  CBS_CONFIG   TempVariable;
  UINT8        *tempBuffer;
  UINTN        BufferSize;

  tempBuffer = (UINT8*)&TempVariable;
  BufferSize = 0;

  Status = gRT->GetVariable (CBS_SYSTEM_CONFIGURATION_NAME,
                             &gCbsSystemConfigurationGuid,
                             NULL,
                             &BufferSize,
                             pVariableData
                             );
  if ((Status == EFI_BUFFER_TOO_SMALL) && (BufferSize == sizeof(CBS_CONFIG))) {
    Status = gRT->GetVariable (CBS_SYSTEM_CONFIGURATION_NAME,
                               &gCbsSystemConfigurationGuid,
                               NULL,
                               &BufferSize,
                               pVariableData
                               );
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
    DEBUG ((EFI_D_ERROR, "No CBS Variable, Force loading CBS variable default value.(OrgSize=0x%x, NewSize=0x%x)\n", BufferSize, sizeof(CBS_CONFIG)));
    CbsWriteDefalutValue(pVariableData);
  }
}

EFI_STATUS
CbsHookFunc (
  IN       EFI_BOOT_SERVICES    *BootServices,
  IN OUT   VOID                 *AMD_PARAMS,
  IN       UINTN                IdsHookId
  )
{
  DXE_CBS_COMPONENT_STRUCT   *pCbsFunctionTable;
  DXE_CBS_FUNCTION_STRUCT    *pCurTable;
  CPUID_DATA                 CpuId;
  UINT64                     SocFamilyID;

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
      // Dispatch CBS function in DXE_CBS_FUNCTION_STRUCT.
      pCurTable = pCbsFunctionTable->pFuncTable;
      while (pCurTable->CBSFuncPtr != NULL) {
        if ((pCurTable->IdsHookId == IdsHookId) &&
          ((pCurTable->Family == SocFamilyID) || (pCurTable->Family == 0xFFFFFFFF))) {
          pCurTable->CBSFuncPtr(BootServices,AMD_PARAMS,pCbsConfig);
        }
        pCurTable++;
      }
    }
    pCbsFunctionTable++;
  }
  return EFI_SUCCESS;
}

EFI_STATUS CbsInitEntry(
  IN  EFI_BOOT_SERVICES *BootServices
  )
{
  EFI_STATUS   Status;
  EFI_HANDLE   Handle;
  CBS_CONFIG   *CbsVariableHob;

  pCbsConfig = AllocateZeroPool (sizeof(CBS_CONFIG));
  if (pCbsConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = CbsGetHobData (&CbsVariableHob);
  if (!EFI_ERROR (Status)) {
    LibAmdMemCopy ((VOID *)pCbsConfig,
                   (VOID *)CbsVariableHob,
                   sizeof (CBS_CONFIG),
                   NULL
                   );
    DEBUG ((EFI_D_ERROR, "CbsInitEntry:: Get CBS Variable from HOB, CBS_UID=0x%x\n", pCbsConfig->Header.CbsVariableStructUniqueValue));
  } else {
    CbsVariableLoadDefault ((UINT8*)pCbsConfig);
  }

  Handle = NULL;
  Status = BootServices->InstallProtocolInterface ( &Handle,
                                        &gCbsProtocolGuid,
                                        EFI_NATIVE_INTERFACE,
                                        &mCbsProtocolInstance);
  return Status;
}

EFI_STATUS
CbsInterfaceFunc (
  IN  EFI_BOOT_SERVICES      *BootServices,
  IN  OUT VOID               *AMD_PARAMS,
  IN      UINTN              IdsHookId
  )
{
  EFI_STATUS                Status;
  CBS_DXE_PROTOCOL          *pCbsProtocol;

  Status = BootServices->LocateProtocol (&gCbsProtocolGuid, NULL, &pCbsProtocol);
  if (!EFI_ERROR(Status)) {
    Status = pCbsProtocol->CbsHookInterface(BootServices, AMD_PARAMS, IdsHookId);
  }
  return Status;
}

