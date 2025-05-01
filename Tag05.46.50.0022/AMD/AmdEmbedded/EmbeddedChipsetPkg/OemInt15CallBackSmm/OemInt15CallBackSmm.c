/** @file
  Provide INT 15 SMM Callback function

;******************************************************************************
;* Copyright (c) 2013-2018, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <OemInt15CallBackSmm.h>
#include <Library/SmmOemSvcChipsetLib.h>

//
// Sample for install multiple Int15 function
//
UINT16 mINT15FunctionNumber[] = {
  0x9999,
  0xFFFF   // End of table
};

EFI_STATUS
EFIAPI
InstallOemInt15CallBackFunction (
  IN CONST EFI_GUID                       *Protocol,
  IN VOID                                 *Interface,
  IN EFI_HANDLE                           Handle
  )
{
  EFI_STATUS                            Status;
  EFI_SMM_INT15_SERVICE_PROTOCOL        *SmmInt15Service = NULL;  
  UINT32                                Index = 0;
  UINT16                                *OemInt15VbiosFunctionHook;
  UINT16                                Size;

  OemInt15VbiosFunctionHook = NULL;
  Size = 0;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmInt15ServiceProtocolGuid, NULL, (VOID **)&SmmInt15Service);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) { 
    return Status;
  }
  
  //
  // Install multiple Int15 function to OemInt15CallBack
  //
  for (Index = 0; (Index < (sizeof (mINT15FunctionNumber) / sizeof (UINT16))) && (mINT15FunctionNumber[Index] != 0xFFFF); Index++) {
    
    Status = SmmInt15Service->InstallInt15ProtocolInterface (
                                SmmInt15Service,
                                mINT15FunctionNumber[Index],
                                OemInt15CallBack,
                                NULL
                                );

    if (Status == EFI_ALREADY_STARTED) { 
      //
      // use new callback function to replace original callback function
      //
      Status = SmmInt15Service->ReinstallInt15ProtocolInterface (
                                  SmmInt15Service,
                                  mINT15FunctionNumber[Index],
                                  OemInt15CallBack,
                                  NULL
                                  );
      return Status;
    }
  }

  Status = OemSvcGetOemInt15VbiosFunctionlist (&OemInt15VbiosFunctionHook, &Size);
  DEBUG_OEM_SVC ((EFI_D_ERROR | EFI_D_INFO, "SmmOemSvcChipsetLib OemSvcGetOemInt15VbiosFunctionlist, Status : %r\n", Status));
  if (Status == EFI_MEDIA_CHANGED){
    for (Index = 0; Index < Size; Index++) {
      Status = SmmInt15Service->InstallInt15ProtocolInterface (
                                  SmmInt15Service,
                                  OemInt15VbiosFunctionHook[Index],
                                  OemInt15CallBack,
                                  NULL
                                  );
      if (Status == EFI_ALREADY_STARTED) {
        //
        // use new callback function to replace original one
        //
        Status = SmmInt15Service->ReinstallInt15ProtocolInterface (
                                    SmmInt15Service,
                                    OemInt15VbiosFunctionHook[Index],
                                    OemInt15CallBack,
                                    NULL
                                    );
        return Status;
      }
    }
  }
  
  return EFI_SUCCESS;
}

/**
  Initializes the OEM INT15 SMM Dispatcher

  @param[in]  ImageHandle       Pointer to the loaded image protocol for this driver.
  @param[in]  SystemTable       Pointer to the EFI System Table

  @retval EFI_SUCCESS           The call returned successfully.

**/
EFI_STATUS
EFIAPI
OemINT15EntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_SMM_INT15_SERVICE_PROTOCOL        *SmmInt15Service = NULL;  
  EFI_EVENT                             InstallOemInt15CallBackFunctionEvent;
  
  Status = gSmst->SmmLocateProtocol (&gEfiSmmInt15ServiceProtocolGuid, NULL, (VOID **)&SmmInt15Service);

  if (!EFI_ERROR(Status)) {
    InstallOemInt15CallBackFunction (&gEfiSmmInt15ServiceProtocolGuid, NULL, NULL);
  } else {
    Status = gSmst->SmmRegisterProtocolNotify (
      &gEfiSmmInt15ServiceProtocolGuid,
      InstallOemInt15CallBackFunction,
      &InstallOemInt15CallBackFunctionEvent
      );
    ASSERT_EFI_ERROR (Status);
  }
  
  return EFI_SUCCESS;
}

/**
  Oem INT15 callback 
  
  @param[in]  Event             The triggered event.
  @param[in]  Context           Context for this event.

**/
STATIC
VOID
OemInt15CallBack (
  IN  OUT EFI_IA32_REGISTER_SET   *CpuRegs, 
  IN      VOID                    *Context
  )
{
  EFI_STATUS                      Status;
  UINT8                           Data8;

  if (!CpuRegs->X.AX) {
    return;
  }

  Status = OemSvcVbiosHookCallBack (CpuRegs->X.AX, CpuRegs, Context);
  DEBUG_OEM_SVC ((EFI_D_ERROR | EFI_D_INFO, "SmmOemSvcChipsetLib OemSvcVbiosHookCallBack, Status : %r\n", Status));
  if (Status == EFI_SUCCESS) {
    return;
  }

  switch (CpuRegs->X.AX) {
  case 0x9999:
    if (CpuRegs->X.BX == 0xdead) {
      //
      // Hook CSM reboot
      //
      Data8 = 0x6;
      gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT8, 0xCF9, 1, &Data8);
      CpuRegs->X.AX = 1;
    }
    break;

  default:
    //
    // to do 
    //
    break;
  }

  return;
}
