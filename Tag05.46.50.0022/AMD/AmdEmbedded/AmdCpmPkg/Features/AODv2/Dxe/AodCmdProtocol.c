/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
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
#include "AodDxe.h"
AOD_MOUDLE_NAME ("AodCmdProtocol")

/**
  This function used to sort data from min to max

  @param[in,out]     Data          The data pointer
  @param[in]         DataCount     The data count

**/
VOID
AodDataSort (
  IN OUT UINT32   *Data,
  IN     UINT32   DataCount
  )
{
  UINTN i,j;
  UINT32 Temp;
  if (DataCount < 4) return;

  for (i = 0; i < (DataCount-1); i++) {
    for (j = 0; j < (DataCount- 1 -i); j++) {
      if (Data[j] > Data[j+1]) {
        Temp = Data[j+1];
        Data[j+1] = Data[j];
        Data[j] = Temp;
      }
    }
  }
}
/**
  This function used to check current discrete cmd data list is Linear or not,
  If Linear , update the discrete data format to Linear

  @param[in,out]     Data          The data pointer
  @param[in]         DataCount     The data count

  @retval EFI_INVALID_PARAMETER input pointer is NULL or cmd is not discrete and data count less than 4
  @retval EFI_UNSUPPORTED input discrete cmd data is not linear
  @retval EFI_OUT_OF_RESOURCES allocate source failed
  @retval EFI_SUCCESS successful change discrete cmd to linear
**/
EFI_STATUS
EFIAPI
AodLinearCheck (
  IN  AOD_CMD      *pCmd
  )
{
  UINT32               Step = 0;
  UINT32               Step1 = 0;
  UINTN                Index;
  UINT32               *pTemp;

  if (pCmd == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (pCmd->Property == LINER_DATA) {
    return EFI_INVALID_PARAMETER;
  }
  if (pCmd->DataCount < 4) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Sort Data (min-max)
  //
  AodDataSort (pCmd->pValue, pCmd->DataCount);

  //
  // matched the step
  //
  Step = pCmd->pValue[1] - pCmd->pValue[0];
  for (Index = 0; Index < pCmd->DataCount-1; Index ++) {
    Step1 = pCmd->pValue[Index+1] - pCmd->pValue[Index];
    if (Step1 != Step) return EFI_UNSUPPORTED;
  }
  pTemp = AllocateZeroPool (3*sizeof(UINT32));
  if (pTemp == NULL) return EFI_OUT_OF_RESOURCES;
  pCmd->Property = LINER_DATA;
  pTemp[0] = pCmd->pValue[0];
  pTemp[1] = pCmd->pValue[pCmd->DataCount-1];
  pTemp[2] = Step;
  FreePool (pCmd->pValue);
  pCmd->pValue = pTemp;
  pCmd->DataCount = 3;
  return EFI_SUCCESS;
}
/**
  This function used to Register a AOD cmd into cmd list instance

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[in]     pCmdName           Pointer to AOD Cmd name

  @retval EFI_INVALID_PARAMETER input protocol is NULL
  @retval EFI_ALREADY_STARTED   this cmdid already been registerd
  @retval EFI_OUT_OF_RESOURCES allocate source failed
  @retval EFI_SUCCESS successful register the cmd
**/
EFI_STATUS
EFIAPI
AodCmdRegister (
  IN    AOD_CMD_PROTOCOL                    *pThis,
  IN    UINT32                               CmdId,
  IN    CHAR8                                *pName
  )
{
  AOD_CMD                                      *pCmd;
  AOD_DXE_PRIVATE_DATA                         *pInstance;
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *Link;

  if (pThis == NULL) { //safety check
    return EFI_INVALID_PARAMETER;
  }
  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);

  //
  // Dispatch all CMD in the cmd list
  //
  if (!IsListEmpty (&pInstance->CmdList)) {
    Link  = GetFirstNode (&pInstance->CmdList);
    while (!IsNull (&pInstance->CmdList, Link)) {
      pCmd = AOD_CMD_FROM_LINK (Link);
      if (pCmd->CmdId == CmdId) {
        return EFI_ALREADY_STARTED;
      }
      Link = GetNextNode (&pInstance->CmdList, Link);
    }
  }
  //
  // Allcoate new instance for ssdt install table
  //
  pCmd = (AOD_CMD *)AllocateZeroPool (sizeof (AOD_CMD));
  if (pCmd == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Init CMD
  //
  pCmd->Signature    = AOD_CMD_SIGNATURE;
  pCmd->CmdId        = CmdId;
  pCmd->pName        = AllocateCopyPool (AsciiStrSize(pName), pName);
  if (pCmd->pName  == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrExit;
  }
  pCmd->pValue = NULL;
  InsertTailList (&pInstance->CmdList, &pCmd->Link);
  pCmd->CurrentNvsIndex = pInstance->CmdCount;
  pInstance->CmdCount ++;
  return EFI_SUCCESS;
ErrExit:
  if (pCmd->pName != NULL) {
    FreePool (pCmd);
  }
  if (pCmd != NULL) {
    FreePool (pCmd);
  }
  return Status;
}

/**
  Unregister a AOD cmd out of cmd list instance

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId

  @retval EFI_INVALID_PARAMETER input protocol is NULL
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful unregister the cmd
**/
EFI_STATUS
EFIAPI
AodCmdUnRegister (
  IN    AOD_CMD_PROTOCOL                    *pThis,
  IN    UINT32                               CmdId
  )
{

  AOD_DXE_PRIVATE_DATA                         *pInstance;
  LIST_ENTRY                                   *Link;
  AOD_CMD                                      *pCmd;

  if (pThis == NULL) { //safety check
    return EFI_INVALID_PARAMETER;
  }

  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);
  //
  // Dispatch all instace in the cmd list
  //
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (pCmd != NULL) {
      if (pCmd->CmdId == CmdId) {
        RemoveEntryList (Link);
        //
        // Free resource
        //
        if (pCmd->pName != NULL) {
          FreePool (pCmd->pName);
        }
        if (pCmd->pValue != NULL) {
          FreePool (pCmd->pValue);
        }
        FreePool (pCmd);
        return EFI_SUCCESS;
      }
    }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }

  return EFI_NOT_FOUND;
}

/**
  Unregister a AOD cmd out of cmd list instance

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[in]     Property           Data Property: 0 for discrete, 1 for linear
  @param[in]     DataCount          Data Count: for linear data, it must equal to 3
  @param[in]     pValue             Data pointer

  @retval EFI_INVALID_PARAMETER input protocol/data pointer is NULL or DataCount is Zero
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful unregister the cmd
**/
EFI_STATUS
EFIAPI
AodCmdUpdateValue (
  IN    AOD_CMD_PROTOCOL                   *pThis,
  IN    UINT32                              CmdId,
  IN    UINT8                               Property,
  IN    UINT32                              DataCount,
  IN    UINT32                             *pValue
  )
{
  AOD_CMD                                      *pCmd;
  AOD_DXE_PRIVATE_DATA                         *pInstance;
  LIST_ENTRY                                   *Link;
  UINT32                                       *pTemp = NULL;
  BOOLEAN                                      IsFound = FALSE;

  if ((pThis == NULL) || (pValue == NULL) || (DataCount == 0)) { //safety check
    return EFI_INVALID_PARAMETER;
  }
  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);
  //
  // Dispatch all instace in the cmd list
  //
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (pCmd->CmdId == CmdId) {
      IsFound = TRUE;
      break;
    }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }
  if (!IsFound) return EFI_NOT_FOUND;

  //
  // Copy Value
  //
  pTemp = AllocateCopyPool (DataCount*sizeof(UINT32), pValue);
  if (pTemp == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  pCmd->Property = Property;
  if (pCmd->pValue != NULL) FreePool (pCmd->pValue);
  pCmd->pValue      = pTemp;
  pCmd->DataCount   = DataCount;

  //
  // data linear check
  //
  AodLinearCheck (pCmd);
  return EFI_SUCCESS;
}

/**
  Update Command Id Current support value

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[in]     Current            Current value


  @retval EFI_INVALID_PARAMETER input protocol/data pointer is NULL or DataCount is Zero
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful update current value
**/
EFI_STATUS
EFIAPI
AodCmdUpdateCurrent (
  IN    AOD_CMD_PROTOCOL                   *pThis,
  IN    UINT32                              CmdId,
  IN    UINT32                              Current
  )
{
  AOD_CMD                                      *pCmd;
  AOD_DXE_PRIVATE_DATA                         *pInstance;
  LIST_ENTRY                                   *Link;
  BOOLEAN                                      IsFound = FALSE;
  UINT32                                       Index;

  if (pThis == NULL) { //safety check
    return EFI_INVALID_PARAMETER;
  }
  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);
  //
  // Dispatch all instace in the cmd list
  //
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (pCmd->CmdId == CmdId) {
      IsFound = TRUE;
      break;
    }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }
  if (!IsFound) return EFI_NOT_FOUND;

  //
  // if count not equal to zero, make sure the input current is valid
  //
  if (pCmd->DataCount) {
    if (pCmd->Property == 0) {//discrete
      for (Index = 0; Index < pCmd->DataCount; Index++) {
        if (Current == *(pCmd->pValue + Index)) {
          pCmd->Current = Current;
          return EFI_SUCCESS;
        }
      }
    } else {//linear
      if ((Current >= *pCmd->pValue) && (Current <= *(pCmd->pValue+1))) {
        pCmd->Current = Current;
        return EFI_SUCCESS;
      }
    }
  } else {
    pCmd->Current = Current;
    return EFI_SUCCESS;
  }
  return EFI_INVALID_PARAMETER;
}
/**
  Query Command Id information

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[out]    Current            Current value
  @param[out]    Current            Current value
  @param[out]    Index              Command Index
  @param[out]    Property           Property: 0 for discrete, 1 for linear
  @param[out]    DataCount          Supported Data Count
  @param[out]    Data               Pointer to supported Data

  @retval EFI_INVALID_PARAMETER input protocol/data pointer is NULL or DataCount is Zero
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful
**/
EFI_STATUS
EFIAPI
AodCmdQueryInfo (
  IN    AOD_CMD_PROTOCOL                    *pThis,
  IN    UINT32                              CmdId,
  OUT   UINT32                              *Current,
  OUT   UINT32                              *Index,
  OUT   UINT8                               *Property,
  OUT   UINT32                              *DataCount,
  OUT   UINT32                              **Data
  )
{
  AOD_CMD                                      *pCmd;
  AOD_DXE_PRIVATE_DATA                         *pInstance;
  LIST_ENTRY                                   *Link;
  BOOLEAN                                      IsFound = FALSE;
  UINT32                                       *Temp;

  if (pThis == NULL) { //safety check
    return EFI_INVALID_PARAMETER;
  }
  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);
  //
  // Dispatch all instace in the cmd list
  //
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (pCmd->CmdId == CmdId) {
      IsFound = TRUE;
      break;
    }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }
  if (!IsFound) return EFI_NOT_FOUND;
  if (Current != NULL)  *Current = pCmd->Current;
  if (Index != NULL) *Index = pCmd->CurrentNvsIndex;
  if (Property != NULL) *Property = pCmd->Property;
  if (DataCount != NULL) *DataCount = pCmd->DataCount;
  if (Data != NULL) {
    Temp = AllocateZeroPool (pCmd->DataCount * sizeof(UINT32));
    if (Temp == NULL) return EFI_OUT_OF_RESOURCES;
    CopyMem (Temp, pCmd->pValue, pCmd->DataCount * sizeof(UINT32));
    *Data = Temp;
  }
  return EFI_SUCCESS;
}
/**
  This function used to install AOD cmd portocol

  @param[in]     Private          Private data pointer of AodDxe

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AodInstallCmdProtocol (
  IN  AOD_DXE_PRIVATE_DATA                    *Private
  )
{

  EFI_STATUS                 Status;
  AOD_CMD_PROTOCOL           *CmdProtocol;
  EFI_HANDLE                 Handle = NULL;
  AOD_DEBUG ((EFI_D_ERROR, "started\n"));

  //
  //  Install Aod Cmd protocol
  //
  InitializeListHead (&Private->CmdList);
  CmdProtocol = &Private->CmdProtocol;
  CmdProtocol->Register             = AodCmdRegister;
  CmdProtocol->UnRegister           = AodCmdUnRegister;
  CmdProtocol->UpdateValue          = AodCmdUpdateValue;
  CmdProtocol->UpdateCurrent        = AodCmdUpdateCurrent;
  CmdProtocol->QueryInfo            = AodCmdQueryInfo;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gAmdAodCmdProtocolGuid,
                  CmdProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Install %g Status =%r\n", &gAmdAodCmdProtocolGuid, Status));
    return Status;
  }

  Status = AodDispatchInitTable (AOD_BUILD_WMI_CMD, &Private->CmdProtocol);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  AOD_BUILD_WMI_CMD Status =%r\n",  Status));
    return Status;
  }
  AOD_DEBUG ((EFI_D_ERROR, "end\n"));
  return EFI_SUCCESS;
}




