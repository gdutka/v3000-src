/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Features/AODv2/Include/AodSetup.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include <Protocol/SmmCommunication.h>
#include <Features/AODv2/Include/Aod.h>
AOD_MOUDLE_NAME ("AodBaseDxeLib")
/**
  Retrieve "AodSetup" config

 @param[out] AodConfiguration        The Pointer to the structure of AOD_CONFIG

 @retval EFI_SUCCESS            The AOD configuration is successfully retrieved

**/
EFI_STATUS
EFIAPI
AodGetConfiguration (
  OUT VOID          **ppAodConfiguration
  )
{
  EFI_STATUS Status;
  UINT8      *Buffer;
  UINTN      BufferSize;

  Buffer       = NULL;
  BufferSize   = 0;
  Status = gRT->GetVariable (
                  AOD_SETUP_VARIABLE_NAME,
                  &gAodSystemConfigurationGuid,
                  NULL,
                  &BufferSize,
                  Buffer
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Buffer = AllocateZeroPool (BufferSize);
    if (Buffer == NULL) return EFI_OUT_OF_RESOURCES;
    Status = gRT->GetVariable (
                    AOD_SETUP_VARIABLE_NAME,
                    &gAodSystemConfigurationGuid,
                    NULL,
                    &BufferSize,
                    Buffer
                    );
    if (EFI_ERROR (Status)) FreePool (Buffer);
  }
  *ppAodConfiguration = Buffer;
  return Status;
}


/**
  Set "AodSetup" config

  @param[out] pAodConfiguration     The Pointer to the structure of AOD_CONFIG
  @param[out] VarSize               VarSize


  @retval EFI_SUCCESS            The AOD CONFIG is successfully set

**/
EFI_STATUS
EFIAPI
AodSetConfiguration (
  IN  VOID          *pAodConfiguration,
  IN  UINTN         VarSize
  )
{
  EFI_STATUS Status;
  Status = gRT->SetVariable (
                AOD_SETUP_VARIABLE_NAME,
                &gAodSystemConfigurationGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                VarSize,
                pAodConfiguration
                );
  return Status;
}

/**
 Retrieve AOD setup configuration data

 @param[out]       AodCoreInfo   Pointer to the structure of AOD_SETUP_POLICY,
                                 this pointer must be allocated with sizeof(AOD_SETUP_POLICY)
                                 before being called

 @retval EFI_SUCCESS            The AOD configuration is successfully retrieved
 @retval EFI_INVALID_PARAMETER  NULL pointer for input KernelConfig paramater
 @return EFI_NOT_FOUND          can't found the guid hob
**/
EFI_STATUS
EFIAPI
AodGetCoreInfo (
  OUT AOD_CORE_INFO     *AodCoreInfo
  )
{
  VOID                            *GuidHob;
  UINT8                           *Buffer;
  if (AodCoreInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GuidHob = GetFirstGuidHob (&gAodSystemConfigurationGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  Buffer = (UINT8 *)GET_GUID_HOB_DATA (GuidHob);
  CopyMem (AodCoreInfo, Buffer, GET_GUID_HOB_DATA_SIZE(GuidHob));
  return EFI_SUCCESS;
}

/**
  Set AOD Setup Policy

  @param[in]       AodCoreInfo --  Pointer to the structure of AOD_CORE_INFO

 @retval EFI_UNSUPPORTED  Policy don't supported update in DXE
**/
EFI_STATUS
EFIAPI
AodSetCoreInfo (
  IN AOD_CORE_INFO        *AodCoreInfo
  )
{
  return EFI_UNSUPPORTED;
}



/**
  Get the free Index of the Aod Init Table list

  @param  Table                 Pointer to the Aod Init Table List.
  @param  Free                  The free

  @retval EFI_SUCCESS           Table initialization successfully.
          EFI_OUT_OF_RESOURCE   All slots are occupied without free one.
**/
EFI_STATUS
AodGetFreeInitIndex (
  IN  VOID    **Table,
  OUT UINT8    *Free
  )
{
  UINT8     Index;
  BOOLEAN   IsFound = FALSE;
  for (Index = 0; Index < AOD_MAX_INIT_NUM; Index ++) {
    if (Table [Index] == NULL) {
      IsFound = TRUE;
      break;
    }
  }

  if (!IsFound) {
    AOD_DEBUG ((DEBUG_ERROR, "Can't Found free Index\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  *Free = Index;
  return EFI_SUCCESS;
}
/**
  Add Aod init table into list

  @param  AodInitTable        Pointer to the array of the Aod Init Table.

  @retval  EFI_SUCCESS             Table add successfully.
  @retval  EFI_INVALID_PARAMETER   Input is Null
**/
EFI_STATUS
AodAddInitTable (
  IN  AOD_INIT_TABLE  *AodInitTable
  )
{
  EFI_STATUS      Status;
  VOID            **Table;
  UINT8           FreeIndex;
  VOID            *Pointer;
  if (AodInitTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Table = (VOID **) (UINTN) PcdGet64(PcdAodDxeInitTableList);
  if (Table == NULL) {
    Pointer = (VOID *) AllocateZeroPool (sizeof (UINT64) * AOD_MAX_INIT_NUM);
    if (Pointer == NULL) {
      AOD_DEBUG ((DEBUG_ERROR, "Allocate Aod Dxe Init Pointer Failed\n"));
      return EFI_OUT_OF_RESOURCES;
    }
    PcdSet64S (PcdAodDxeInitTableList, (UINTN) Pointer);
    Table = (VOID **) (UINTN) PcdGet64 (PcdAodDxeInitTableList);
  }

  Status = AodGetFreeInitIndex (Table, &FreeIndex);

  Table [FreeIndex] = (VOID *) AodInitTable;

  return EFI_SUCCESS;
}

/**
  Aod Init Dispatch function

  @param  Phase            Aod Init Phase identify
  @param  Context          Aod Init Context Pointer

  @retval     EFI_SUCCESS          The function completes successfully.
  @retval     EFI_UNSUPPORTED      The AodInitTableList is not init properly.
  @retval     Others By callee
**/
EFI_STATUS
AodDispatchInitTable (
  IN  UINT8           Phase,
  IN  VOID            *Context
  )
{
  EFI_STATUS        Status;
  VOID              **Table;
  UINT8             Index;
  AOD_INIT_TABLE    *AodInitTable;
  UINT16            AodDispatchFlag = 0;

  Table = (VOID **) (UINTN) PcdGet64 (PcdAodDxeInitTableList);
  if (Table == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Scan and Dispatch Aod Init function if ComboFlag and Phase matched.
  //
  Status = EFI_NOT_FOUND;
  for (Index = 0; Index < AOD_MAX_INIT_NUM; Index ++) {
    if (Table [Index] == NULL) {
      break;
    }
    AOD_DEBUG ((DEBUG_INFO, "Table [%d] = 0x%x\n", Index, Table [Index]));
    AodInitTable = (AOD_INIT_TABLE *) Table[Index];
    //
    // Get the ComboFlag
    //
    while (TRUE) {
      if (AodInitTable->Phase == AOD_UNKNOWN_INIT) break;
      if (AodInitTable->Phase == AOD_DISPATCH_INIT) {
        Status = AodInitTable->Init (&AodDispatchFlag);
        if (EFI_ERROR (Status)) {
          return Status;
        }
        break;
      }
      AodInitTable ++;
    }
    AodInitTable = (AOD_INIT_TABLE *) Table[Index];
    //
    // Dispatch
    //
    AOD_DEBUG ((DEBUG_INFO, " |- [Need] AodDispatchFlag = 0x%02x Phase = 0x%x\n", AodDispatchFlag, Phase));
    while (TRUE) {
      if (AodInitTable->Phase == AOD_UNKNOWN_INIT) break;
        AOD_DEBUG ((DEBUG_INFO, " |---- [Get] AodDispatchFlag = 0x%02x Phase = 0x%x\n", \
          AodInitTable->AodDispatchFlag, AodInitTable->Phase));
      if (((AodDispatchFlag == AodInitTable->AodDispatchFlag) || (AodInitTable->AodDispatchFlag == 0)) && \
         (AodInitTable->Phase == Phase)) {
        Status = AodInitTable->Init (Context);
        AOD_DEBUG ((DEBUG_INFO, " |---- [run] AodDispatchFlag = 0x%02x Phase = 0x%x, Status = %r\n", \
          AodDispatchFlag, AodInitTable->Phase, Status));
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }
      AodInitTable ++;
    }
  }
  return Status;
}
UINT32 *mRamPointer = NULL;
/**
  this function is used to print debug info.
  @param[in]  DebugLevel          Con Out message level
  @param[in]  pFileName           Current file name.
  @param[in]  pFunctionName       Current function name.
  @param[in]  CmdId               WMI cmdid
  @param[in]  Format               WMI cmdid
  @param  ...             - A variable argument list whose contents are accessed based on the format string specified by Format.
  @retval none
**/
VOID
EFIAPI
AodDebugMsg (
  IN  UINTN               DebugLevel,
  IN  CONST CHAR8         *pFileName,
  IN  CONST CHAR8         *pFunctionName,
  IN  UINTN               LineNumber,
  IN  CONST CHAR8         *pSocName,
  IN  CONST CHAR8         *Format,
  ...
  )
{
  EFI_STATUS        Status;
  CHAR8             Buffer[0x200];
  CHAR8             Prefix[0x50];
  UINTN             PhyAddress = 0;
  VA_LIST           Marker;
  UINTN             VariableSize;


  //
  //Display the file name, current line number, procedure name,
  //if cmdid not equal to zero, display cmdid two
  //
  if (pSocName != NULL) {
    AsciiSPrint (Prefix, sizeof(Prefix), "[AOD] %20a[%04d] %30a[%4a]: ", pFileName, LineNumber, pFunctionName, \
      pSocName);
  } else {
    AsciiSPrint (Prefix, sizeof(Prefix), "[AOD] %20a[%04d] %30a      : ", pFileName, LineNumber, pFunctionName);
  }


  //
  // Convert the DEBUG() message to an ASCII String
  //
  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);
  //
  // Send the string to common DEBUG
  //
  DEBUG ((DebugLevel, "%a%a", Prefix, Buffer));
  //
  // Send the string to ram debug
  //
  if (mRamPointer == NULL) {
    VariableSize = 8;
    Status = gRT->GetVariable (
                    L"AodDebugStruct",
                    &gAodSystemConfigurationGuid,
                    NULL,
                    &VariableSize,
                    (VOID *) &PhyAddress
                    );
    if (Status == EFI_NOT_FOUND) {
      mRamPointer = (UINT32 *)AllocateRuntimePages (EFI_SIZE_TO_PAGES (RAM_DEBUG_LENGTH));
      if (mRamPointer == NULL) {
        return;
      }
      CopyMem (mRamPointer, (VOID *)(UINTN)PcdGet32 (PcdAodRamDebugAddress), RAM_DEBUG_LENGTH);
      PhyAddress = (UINTN) mRamPointer;
      VariableSize = 8;
      Status = gRT->SetVariable (
                      L"AodDebugStruct",
                      &gAodSystemConfigurationGuid,
                      EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      VariableSize,
                      &PhyAddress
                      );
      DEBUG ((EFI_D_INFO, "PhyAddress @ %lx\n", PhyAddress));
      if (EFI_ERROR (Status)) {
        return;
      }
    }
  }
  mRamPointer  = (UINT32 *)(UINTN)PhyAddress;
  if (mRamPointer == NULL) return;
  AodRamDebugWrite (mRamPointer, Prefix,  AsciiStrLen (Prefix));

  AodRamDebugWrite (mRamPointer, Buffer,  AsciiStrLen (Buffer));
}

/**
  Add Aod Setup Callback
  @param[in]       List           --  Aod Callback link list
  @param[in]       UpdateForm     --  Aod Callback update front package
  @param[in]       Callback       --  Aod Callback Routine

 @retval EFI_OUT_OF_RESOURCES  Allocate Pool failed
**/
EFI_STATUS
EFIAPI
AodAddSetupCallback (
  IN  LIST_ENTRY                        *List,
  IN  BOOLEAN                           UpdateForm,
  IN  CALLBACK_ITEM_EX                  Callback
  )
{
  AOD_SETUP_CALLBACK       *pNode;

  pNode = AllocateZeroPool (sizeof (AOD_SETUP_CALLBACK));
  if (pNode == NULL) return EFI_OUT_OF_RESOURCES;

  pNode->Signature = AOD_DXE_SETUP_CALLBACK_SIGNATURE;
  pNode->Callback  = Callback;
  pNode->UpdateForm  = UpdateForm;
  InsertTailList (List, &pNode->Link);
  return EFI_SUCCESS;
}

/**
  Send the Data in communicate Buffer to SMM.
  @param[in]   Id                     AOD_SMM_COMM_FUNCTION_ID.
  @param[in]   Buffer                 Points to the Data in the communicate Buffer.
  @param[in]   DataSize               This Size of the function Header and the Data.

  @retval      EFI_SUCCESS            Success is returned from the functin in SMM.
  @retval      Others                 Failure

**/
EFI_STATUS
EFIAPI
AodSendCommunicateBuffer (
  IN      UINT8                             Id,
  IN      VOID                              *Data,
  IN      UINTN                             DataSize
  )
{
  EFI_STATUS                                Status;
  UINTN                                     CommSize;
  EFI_SMM_COMMUNICATE_HEADER                *SmmCommunicateHeader;
  AOD_SMM_COMMUNICATE_HEADER                *AodSmmCommunicateHeader;
  EFI_SMM_COMMUNICATION_PROTOCOL            *SmmCommunication;
  AOD_NV_AREA_UPDATED_PROTOCOL              *AodNvsProtocol;
  UINT8                                     *Buffer;

  if ((Data == NULL) || (DataSize == 0) || (Id >= AOD_SMM_COMM_ID_MAX)) return EFI_INVALID_PARAMETER;
  //
  // locate EFI_SMM_COMMUNICATION_PROTOCOL
  //
  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get Smm Communicate Region
  //
  CommSize = DataSize + OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data) + OFFSET_OF (AOD_SMM_COMMUNICATE_HEADER, Data);
  Status = gBS->LocateProtocol (&gAodNvAreaUpdatedProtocolGuid, NULL, (VOID **) &AodNvsProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (AodNvsProtocol->AodNvAreaPtr->AodSmmCommBufSize == 0) return EFI_NOT_READY;
  if (CommSize > AodNvsProtocol->AodNvAreaPtr->AodSmmCommBufSize) return EFI_INVALID_PARAMETER;
  Buffer = (VOID*)(UINTN)AodNvsProtocol->AodNvAreaPtr->AodSmmCommBufAddr;
  if (Buffer == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Init AOD Smm Communicate Header and Data
  //
  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) Buffer;
  CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gAodSystemConfigurationGuid);                     // HeaderGuid
  SmmCommunicateHeader->MessageLength = DataSize + OFFSET_OF (AOD_SMM_COMMUNICATE_HEADER, Data);  // MessageLength

  AodSmmCommunicateHeader = (AOD_SMM_COMMUNICATE_HEADER *) SmmCommunicateHeader->Data;
  AodSmmCommunicateHeader->Id = Id;                                                               // Dispatch ID
  AodSmmCommunicateHeader->RetStatus = EFI_SUCCESS;                                               // Return Status
  CopyMem ((VOID*)AodSmmCommunicateHeader->Data, Data, DataSize);

  //
  // Send Communicate Buffer to SMM
  //
  Status = SmmCommunication->Communicate (SmmCommunication, Buffer, &CommSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return  AodSmmCommunicateHeader->RetStatus;
}