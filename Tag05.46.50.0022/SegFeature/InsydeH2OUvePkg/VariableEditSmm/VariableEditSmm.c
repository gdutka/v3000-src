/** @file
  Content file contains function definitions for Variable Edit Smm driver

;******************************************************************************
;* Copyright (c) 2016, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
**/

#include "VariableEditSmm.h"

//
// Factory default Support Function
//
STATIC
IHISI_REGISTER_TABLE
H2OUVE_SIMSERVICE_REGISTER_TABLE[] = {
  //
  // AH=52h, H2O UEFI variable edit SMI service - Confirm the legality of the variables.
  //
  { UveVariableConfirm, "S52OemH2OSmiCallBac", H2OSmiCallBack                 }, \
  { UveVariableConfirm, "S52OemSetupCBSync00", SetupCallbackSyncCallbackIhisi }, \

  //
  // AH=53h, H2O UEFI variable edit SMI service - Boot information-related services.
  //
  { UveBootInfoService, "S53OemBootInfoSvc00", BootInfoService         }
};

H2O_IHISI_PROTOCOL                      *mH2OIhisi           = NULL;
STATIC CONST  BOOLEAN                   mPcdIhisiRegisterTableValid = (BOOLEAN)(FixedPcdGetPtrSize (PcdH2OUveIhisiRegisterTable) % sizeof (PCD_IHISI_REGISTER_TABLE) == 0);

/**
  Helper function to get IHISI function priority.

  @param[in]  Function            Pointer to ihisi register table.
  @param[out] Priority            Pointer to function priority

  @retval EFI_SUCCESS             Get function priority successfully.
  @retval EFI_INVALID_PARAMETER   Function or Priority is NULL.
  @retval EFI_NOT_FOUND           Cannot find function priority in PCD table.

**/
STATIC
EFI_STATUS
GetSubFunctionPriorityFromPcd (
  IN    IHISI_REGISTER_TABLE     *Function,
  OUT   UINT8                     *Priority
  )
{
  UINT8                         *PcdTable;
  UINTN                         PcdOffest;
  UINTN                         PcdSize;
  UINTN                         SignatureSize;

  if (Function == NULL || Priority == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PcdTable   = (UINT8 *)PcdGetPtr (PcdH2OUveIhisiRegisterTable);
  PcdSize    = FixedPcdGetPtrSize (PcdH2OUveIhisiRegisterTable);

  SignatureSize = AsciiStrSize (Function->FuncSignature);
  PcdOffest = 0;
  while (PcdOffest + sizeof (UINT8) + SignatureSize + sizeof (UINT8) <= PcdSize) {
    if (PcdTable[PcdOffest] == Function->CmdNumber &&
        CompareMem (PcdTable + PcdOffest + sizeof (UINT8), Function->FuncSignature, SignatureSize) ==0) {
      *Priority = PcdTable[PcdOffest + sizeof (UINT8) + SignatureSize];
      return EFI_SUCCESS;
    }
    //
    // Get next function in IHISI registered table.
    // NOTE: The FuncSignature size in PcdH2OUveIhisiRegisterTable isn't always 20 bytes so we
    //       need add extra check to get next function in PcdH2OUveIhisiRegisterTable.
    //
    PcdOffest ++;
    while (PcdOffest < PcdSize && PcdTable[PcdOffest] != 0) {
      PcdOffest++;
    }
    //
    // Skip end of char in function signature (1 byte) and function priority (1 byte)
    //
    PcdOffest += 2;
  }
  return EFI_NOT_FOUND;
}

/**
  Register IHISI sub function if SubFuncTable CmdNumber/AsciiFuncGuid define in PcdIhisiRegisterTable list.

  @param[out] SubFuncTable        Pointer to ihisi register table.
  @param[out] TableCount          SubFuncTable count

  @retval EFI_SUCCESS        Function succeeded.
  @return Other              Error occurred in this function.
**/
EFI_STATUS
RegisterIhisiSubFunction (
  IHISI_REGISTER_TABLE          *SubFuncTable,
  UINT16                        TableCount
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    PriorityStatus;
  UINT8                         PcdPriority;
  UINTN                         Index;

  if (!mPcdIhisiRegisterTableValid) {
    DEBUG ((EFI_D_ERROR, "PcdIhisiRegisterTable of description not follow PCD_IHISI_REGISTER_TABLE definition, \
                          it may cause some of IHISI function register fail \n"));
  }
  Status = EFI_SUCCESS;
  for (Index = 0; Index < TableCount; Index ++) {
    PcdPriority        = 0x80;
    PriorityStatus = GetSubFunctionPriorityFromPcd (&SubFuncTable[Index], &PcdPriority);
    if (EFI_ERROR (PriorityStatus)) {
      continue;
    }
    Status = mH2OIhisi->RegisterCommand (SubFuncTable[Index].CmdNumber, SubFuncTable[Index].IhisiFunction, PcdPriority);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "IHISI command :0x%X, priority : 0x%X, that already has a registered function\n", \
                            SubFuncTable[Index].CmdNumber, PcdPriority));
      ASSERT (FALSE);
    }
  }
  return Status;
}

/**
  Register IHISI sub function for H2OUvePkg.

  @retval EFI_SUCCESS        Function succeeded.
  @return Other              Error occurred in this function.
**/
EFI_STATUS
InstallH2OUveIhisiServices (
  VOID
  )
{
  EFI_STATUS                            Status;
  IHISI_REGISTER_TABLE                  *SubFuncTable;
  UINT16                                TableCount;

  Status = gSmst->SmmLocateProtocol (
                    &gH2OIhisiProtocolGuid,
                    NULL,
                    (VOID **) &mH2OIhisi
                    );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a:%d] Status:%r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }


  SubFuncTable = H2OUVE_SIMSERVICE_REGISTER_TABLE;
  TableCount = sizeof(H2OUVE_SIMSERVICE_REGISTER_TABLE)/sizeof(H2OUVE_SIMSERVICE_REGISTER_TABLE[0]);
  Status = RegisterIhisiSubFunction (SubFuncTable, TableCount);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[%a:%d] Status:%r\n", __FUNCTION__, __LINE__, Status));
  }

  return Status;
}

/**
  Check status of function disabled variable, install IHISI sub function and initialize SysPassword Driver.

 @param [in]   ImageHandle      Pointer to the loaded image protocol for this driver
 @param [in]   SystemTable      Pointer to the EFI System Table

 @retval EFI_SUCCESS
 @retval EFI_UNSUPPORTED        VarEditFunDis variable is exist and has been seted TRUE.
 @return Other                  Error occurred in this function.

**/
EFI_STATUS
EFIAPI
SetupCallbackSyncEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                                Status;
  VARIABLE_EDIT_FUNCTION_DISABLE_SETTING    VarEditFunDis;
  UINTN                                     VarEditFunDisSize;
  EFI_SMM_VARIABLE_PROTOCOL                 *SmmVariable;

  DEBUG ((DEBUG_INFO, "->SetupCallbackSyncEntryPoint\n"));

  ZeroMem (&VarEditFunDis, sizeof (VARIABLE_EDIT_FUNCTION_DISABLE_SETTING));

  VarEditFunDisSize = sizeof (VARIABLE_EDIT_FUNCTION_DISABLE_SETTING);

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmVariableProtocolGuid,
                    NULL,
                    (VOID **)&SmmVariable
                    );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a:%d] Status:%r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }
  Status = SmmVariable->SmmGetVariable (
                          H2OUVE_VARSTORE_NAME,
                          &gH2OUveVarstoreGuid,
                          NULL,
                          &VarEditFunDisSize,
                          &VarEditFunDis
                          );

  if ( VarEditFunDisSize != sizeof (VARIABLE_EDIT_FUNCTION_DISABLE_SETTING)) {
    DEBUG ((DEBUG_INFO, "Please check sizeof (VARIABLE_EDIT_FUNCTION_DISABLE_SETTING) in Variable Store."));
  }
  if ( (!EFI_ERROR (Status)) &&
       (VarEditFunDis.VariableEditFunDis == TRUE) &&
       (VarEditFunDisSize == sizeof (VARIABLE_EDIT_FUNCTION_DISABLE_SETTING))
     ) {
    DEBUG ((DEBUG_INFO, "[%a:%d] VariableEditor Function Disabled.\n", __FUNCTION__, __LINE__));
    return EFI_UNSUPPORTED;
  }

  UpdateVariableEditBootTypeInfo();

  Status = InstallH2OUveIhisiServices ();
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[%a:%d] Fail to install the H2OUVE IHISI service.\n", __FUNCTION__, __LINE__));
    return Status;
  }

  return EFI_SUCCESS;
}

