/** @file
  This driver provides IHISI interface in SMM mode

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "ApcbTokensUpdateSmm.h"
#include <Library/BaseMemoryLib.h>

H2O_IHISI_PROTOCOL                   *mH2OIhisi           = NULL;
STATIC CONST  BOOLEAN                mPcdIhisiRegisterTableValid = (BOOLEAN)(FixedPcdGetPtrSize (PcdIhisiRegisterTable) % sizeof (PCD_IHISI_REGISTER_TABLE) == 0);


/**
  Register IHISI sub function for ApcbTokensUpdate.

  @retval EFI_SUCCESS        Function succeeded.
  @return Other              Error occurred in this function.
*/
EFI_STATUS
EFIAPI
ApcbTokensUpdateSmmEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
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

  GetFuncTable (&SubFuncTable, &TableCount);
  Status = RegisterIhisiSubFunction (SubFuncTable, TableCount);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[%a:%d] Status:%r\n", __FUNCTION__, __LINE__, Status));
  }

  return Status;
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
  UINT8                         *PcdTable;
  UINT8                         PcdPriority;
  UINT8                         EndChar;
  UINTN                         Index;
  UINTN                         PcdCount;
  UINTN                         PcdMaxCount;
  UINTN                         SignatureSize;
  BOOLEAN                       PcdFoundRegistered;

  Status             = EFI_SUCCESS;
  EndChar            = 0;
  PcdFoundRegistered = FALSE;

  PcdTable    = (UINT8 *)PcdGetPtr (PcdIhisiRegisterTable);
  PcdMaxCount = FixedPcdGetPtrSize (PcdIhisiRegisterTable) / sizeof (UINT8);
  if (!mPcdIhisiRegisterTableValid) {
    DEBUG ((EFI_D_ERROR, "PcdIhisiRegisterTable of description not follow PCD_IHISI_REGISTER_TABLE definition, \
                          it may cause some of IHISI function register fail \n"));
  }

  for (Index = 0; Index < TableCount; Index ++) {
    PcdCount   = 0;
    PcdPriority = 0x80;
    PcdFoundRegistered = FALSE;
    SignatureSize = AsciiStrnLenS (SubFuncTable[Index].FuncSignature, sizeof(SubFuncTable[Index].FuncSignature));

    //
    // Caculate PCD of address to find 1. CmdNumber 2. FuncSignature 3. Priority
    //
    do {
      if (SubFuncTable[Index].CmdNumber == *(PcdTable + PcdCount)) {
        PcdCount++;
        if (AsciiStrnCmp (SubFuncTable[Index].FuncSignature, (CHAR8 *) (PcdTable + PcdCount), SignatureSize) == 0) {
          if (EndChar == *(PcdTable + PcdCount + SignatureSize)) {
            PcdPriority = *(PcdTable + PcdCount + SignatureSize + 1);
            PcdFoundRegistered = TRUE;
            break;
          }
        }
      }
      PcdCount++;
    } while (PcdCount < PcdMaxCount);

    if (PcdFoundRegistered) {
      Status = mH2OIhisi->RegisterCommand (SubFuncTable[Index].CmdNumber, SubFuncTable[Index].IhisiFunction, PcdPriority);
      if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "IHISI command :0x%X, priority : 0x%X, that already has a registered function\n", SubFuncTable[Index].CmdNumber, PcdPriority));
        ASSERT (FALSE);
      }
    }
  }

  return Status;
}
