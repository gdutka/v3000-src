/** @file
    This library provides Helper functions for HddPassword Table


;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <Uefi.h>
#include <Protocol/HddPasswordService.h>
#include <Guid/HddPasswordVariable.h>
#include <Guid/H2OCp.h>
#include <Library/H2OHddPasswordTableLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DebugLib.h>
#include <Library/VariableLib.h>
#include <Library/SavedPasswordStringProcessLib.h>

/**
  Calculate the number of Hdd password tables.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.
  @param[in]        HddPasswordTableSize     HddPasswordTable size.

  @retval The number of tables.

**/
UINT32
NumOfHddPasswordTable (
  IN  HDD_PASSWORD_TABLE                  *HddPasswordTable,
  IN  UINTN                               HddPasswordTableSize
  )
{
  UINT32                               HddPasswordCount;
  HDD_PASSWORD_TABLE                  *HddPasswordTablePtr;

  if (HddPasswordTable == NULL || HddPasswordTableSize == 0) {
    return 0;
  }

  HddPasswordCount = 0;
  HddPasswordTablePtr = (HDD_PASSWORD_TABLE *)HddPasswordTable;
  while (((UINTN)HddPasswordTable + HddPasswordTableSize) > (UINTN)HddPasswordTablePtr) {
    HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr);
    HddPasswordCount++;
  }

  return HddPasswordCount;
}


/**
  Returns the next entry of the HDD password table.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.

  @retval           The pointer to the next table.

**/
HDD_PASSWORD_TABLE *
GetNextTableEntry (
  IN  HDD_PASSWORD_TABLE                *HddPasswordTablePtr
  )
{
  if (HddPasswordTablePtr == NULL) {
    return NULL;
  }
  return (HDD_PASSWORD_TABLE *)((UINTN)(HddPasswordTablePtr + 1) + HddPasswordTablePtr->ExtDataSize + HddPasswordTablePtr->DevicePathSize);
}


/**
  Returns the next entry of the HDD password table.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.

  @retval           The pointer to the next table.

**/
HDD_PASSWORD_TABLE *
GetTableEntryWithIndex (
  IN  HDD_PASSWORD_TABLE                *HddPasswordTablePtr,
  IN  UINTN                             Index
  )
{

  UINTN Count;  
  if (HddPasswordTablePtr == NULL) {
    return NULL;
  }


  for (Count = 0; Count < Index; Count++) {
    HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr);
  }
  return HddPasswordTablePtr;
}


/**
  Zero out HddPasswordTable by policy.

  @param[in]        HddPasswordTable         Point to HddPasswordTable.
  @param[in]        HddPasswordTableSize     Size of HddPasswordTable.

**/
VOID
ZeroMemByPolicy (
  IN  HDD_PASSWORD_TABLE                    *HddPasswordTable,
  IN  UINTN                                 HddPasswordTableSize
  )
{
  HDD_PASSWORD_TABLE                    *HddPasswordTablePtr;
  TCG_OPAL_EXT_DATA                     *TcgExtDataPtr;
  UINT16                                DevicePathSize;
  
  if (HddPasswordTable == NULL || HddPasswordTableSize == 0) {
    return;
  }
  
  HddPasswordTablePtr = (HDD_PASSWORD_TABLE *)HddPasswordTable;
  while (((UINTN)HddPasswordTable + HddPasswordTableSize) > (UINTN)HddPasswordTablePtr) {
    DevicePathSize = HddPasswordTablePtr->DevicePathSize;
    TcgExtDataPtr = (TCG_OPAL_EXT_DATA *)(HddPasswordTablePtr + 1);
    if (TcgExtDataPtr->Signature == TCG_OPAL_EXT_DATA_SIGNATURE) {
      //
      // Keep the ExtData but zero out password.
      //
      ZeroMem ((VOID *)HddPasswordTablePtr->PasswordStr, sizeof (HddPasswordTablePtr->PasswordStr));
      ZeroMem ((VOID *)&HddPasswordTablePtr->PasswordType, sizeof (HddPasswordTablePtr->PasswordType));
    } else {
      ZeroMem ((VOID *)HddPasswordTablePtr, sizeof (HDD_PASSWORD_TABLE) + HddPasswordTablePtr->ExtDataSize);
    }

    //
    // Keep devicepath size
    //
    if (DevicePathSize != 0) {
      HddPasswordTablePtr->DevicePathSize = DevicePathSize;
    }
    HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr);
  }
}

/**
  Return ExtDataSize.

  @retval           The Size of ExtData is going to be used
**/
UINT16
GetExtDataSize (
  VOID
  )
{
  if (PcdGetBool (PcdH2OHddPasswordTcgOpalSupported)) {
    return sizeof (TCG_OPAL_EXT_DATA);
  }
  return 0;
}


/**
  Get HDDPassword Table size

  @param[in]        NumOfHdd     The number of HDDs.
  @param[in]        TableSize    The size of table allocated.
  @param[in]        HddPasswordTableSize  The size of HddPassword table 
  @param[in]        HddInfoArray          Hdd password info pointer 

  @retval           Tablesize                This Hddpassword is valid.
**/
UINT32
GetHddPasswordTableSize (
  IN     UINTN                          HddPasswordCount,
  IN     HDD_PASSWORD_TABLE             *HddPasswordTable
  )
{
  UINT32                                 HddPasswordTableSize;
  HDD_PASSWORD_TABLE                    *HddPasswordTablePtr;
  UINTN                                 Index;

  if (HddPasswordTable == NULL) {
    return 0;
  }
  
  HddPasswordTablePtr = NULL;
  HddPasswordTableSize = 0;
  for (Index = 0; Index < HddPasswordCount; Index++) {
    HddPasswordTablePtr = GetTableEntryWithIndex(HddPasswordTable, Index);
    HddPasswordTableSize += HddPasswordTablePtr->DevicePathSize;
    HddPasswordTableSize += GetExtDataSize ();
    HddPasswordTableSize += sizeof(HDD_PASSWORD_TABLE);
  }
  return HddPasswordTableSize;
}

/**
  extract device path from end of each Hddpassword table.

  @param[in]        HddPasswordTable         Point to HddPasswordTable.

  @retval           The pointer to devicepath.

**/
EFI_DEVICE_PATH_PROTOCOL*
ExtractDpFromHddPasswordTable (
  IN  HDD_PASSWORD_TABLE                    *HddPasswordTable
  )
{
  TCG_OPAL_EXT_DATA                     *TcgExtDataPtr;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePathFromHddPasswordTable;    
  if (HddPasswordTable == NULL) {
    return NULL;
  }

  TcgExtDataPtr = (TCG_OPAL_EXT_DATA *)(HddPasswordTable + 1);
  DevicePathFromHddPasswordTable    = (EFI_DEVICE_PATH_PROTOCOL*)((UINTN)TcgExtDataPtr + HddPasswordTable->ExtDataSize);
  return DevicePathFromHddPasswordTable;
}


/**
  Internal function to  trigger checkpoint.
  @param[in]        HddPasswordTable               HddPassword table from caller 
  @param[in out]        HddPasswordTableSize           Input - Size of Hddpassword table from caller, 
                                                       Output - if set table fail, return 0

  @retval EFI_SUCCESS             Trigger boot device select checkpoint successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
**/
EFI_STATUS
TriggerCpHddPasswordSetTable (
IN HDD_PASSWORD_TABLE                     *HddPasswordTable,
IN OUT  UINT32                            HddPasswordCount
  )
{
  H2O_BASE_CP_HDD_PASSWORD_SET_TABLE_DATA  BaseCpHddPasswordSetTableData;
  EFI_STATUS                               Status;

  BaseCpHddPasswordSetTableData.Size     = sizeof (H2O_BASE_CP_HDD_PASSWORD_SET_TABLE_DATA);
  BaseCpHddPasswordSetTableData.Status   = H2O_BDS_TASK_NORMAL;
  BaseCpHddPasswordSetTableData.HddPasswordCount = HddPasswordCount;
  BaseCpHddPasswordSetTableData.HddPasswordTable = HddPasswordTable;
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBaseCpHddPasswordSetTableGuid));
  Status = H2OCpTrigger (&gH2OBaseCpHddPasswordSetTableGuid, &BaseCpHddPasswordSetTableData);
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BaseCpHddPasswordSetTableData.Status));
  return Status;
}



/**
  Internal function to  trigger checkpoint.
  @param[out]        HddPasswordTable               HddPassword table that will output to caller
  @param[out]        HddPasswordTableSize              Size of Hddpassword table which output to caller

  @retval EFI_SUCCESS             Trigger boot device select checkpoint successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
**/
EFI_STATUS
TriggerCpHddPasswordGetTable (
IN OUT  HDD_PASSWORD_TABLE                    **HddPasswordTable,
IN OUT  UINT32                                *HddPasswordCount
  )
{
  H2O_BASE_CP_HDD_PASSWORD_GET_TABLE_DATA  BaseCpHddPasswordGetTableData;
  EFI_STATUS                               Status;
  
  BaseCpHddPasswordGetTableData.Size     = sizeof (H2O_BASE_CP_HDD_PASSWORD_GET_TABLE_DATA);
  BaseCpHddPasswordGetTableData.Status   = H2O_CP_TASK_NORMAL;
  BaseCpHddPasswordGetTableData.HddPasswordStatus = EFI_SUCCESS;
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBaseCpHddPasswordGetTableGuid));
  Status = H2OCpTrigger (&gH2OBaseCpHddPasswordGetTableGuid, &BaseCpHddPasswordGetTableData);
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BaseCpHddPasswordGetTableData.Status));
  if (BaseCpHddPasswordGetTableData.Status == H2O_CP_TASK_UPDATE)  {
    *HddPasswordTable = BaseCpHddPasswordGetTableData.HddPasswordTable;
    *HddPasswordCount = BaseCpHddPasswordGetTableData.HddPasswordCount;
  }
  return BaseCpHddPasswordGetTableData.HddPasswordStatus;
}

/**
  To install HDD password services for all supported devices.

  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBaseCpHddPasswordSetTableCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  EFI_STATUS                               Status;
  HDD_PASSWORD_TABLE                       *HddPasswordTable;
  UINT32                                   HddPasswordCount;
  H2O_BASE_CP_HDD_PASSWORD_SET_TABLE_DATA  *BaseCpHddPasswordSetTableData;
  EFI_GUID                                 H2OBaseCpHddPasswordSetTableGuid;
  UINTN                                    HddPasswordTableSize;
  Status = H2OCpLookup(Handle, (VOID**)&BaseCpHddPasswordSetTableData, &H2OBaseCpHddPasswordSetTableGuid);
  if (EFI_ERROR(Status)) {
    return;
  }
  HddPasswordCount = BaseCpHddPasswordSetTableData->HddPasswordCount;
  HddPasswordTable     = BaseCpHddPasswordSetTableData->HddPasswordTable;
  HddPasswordTableSize = GetHddPasswordTableSize(HddPasswordCount, HddPasswordTable);
  if (PcdGetBool (PcdH2OHddPasswordEncryptedUsingTpm)) {
    SavedPasswordStringProcess (
            EncryptProcess,
            HddPasswordTable,
            HddPasswordTableSize
            );
  }
  Status = CommonSetVariable (
             SAVE_HDD_PASSWORD_VARIABLE_NAME,
             &gSaveHddPasswordGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             HddPasswordTableSize,
             HddPasswordTable
             );
  BaseCpHddPasswordSetTableData->Status = H2O_CP_TASK_UPDATE;
  BaseCpHddPasswordSetTableData->HddPasswordStatus = Status; 
}


/**
  To install HDD password services for all supported devices.

  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBaseCpHddPasswordGetTableCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  EFI_STATUS                               Status;
  HDD_PASSWORD_TABLE                       *HddPasswordTable;
  UINTN                                    HddPasswordTableSize;
  H2O_BASE_CP_HDD_PASSWORD_GET_TABLE_DATA  *BaseCpHddPasswordGetTableData;
  EFI_GUID                                 H2OBaseCpHddPasswordGetTableGuid;
  HddPasswordTableSize = 0;
  HddPasswordTable     = NULL;
  Status = H2OCpLookup(Handle, (VOID**)&BaseCpHddPasswordGetTableData, &H2OBaseCpHddPasswordGetTableGuid);
  if (EFI_ERROR(Status)) {
    return;
  }
  Status = CommonGetVariableDataAndSize (
             SAVE_HDD_PASSWORD_VARIABLE_NAME,
             &gSaveHddPasswordGuid,
             &HddPasswordTableSize,
             (VOID **) &HddPasswordTable
             );
  if (PcdGetBool (PcdH2OHddPasswordEncryptedUsingTpm)) {
    SavedPasswordStringProcess (
        DecryptProcess,
        HddPasswordTable,
        HddPasswordTableSize
        );
  }
  BaseCpHddPasswordGetTableData->Status = H2O_CP_TASK_UPDATE;
  BaseCpHddPasswordGetTableData->HddPasswordCount = NumOfHddPasswordTable(HddPasswordTable, HddPasswordTableSize);
  BaseCpHddPasswordGetTableData->HddPasswordTable = HddPasswordTable;
  BaseCpHddPasswordGetTableData->HddPasswordStatus = Status;
}

