/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/MpmBiosCmdLib.h>

#define FILECODE LIBRARY_MPMBIOSCMDLIB_MPMBIOSCMDLIB_FILECODE
#define SMN_MPMMMIOPUBLIC_BASE  0xFC00000
#define MPM_BIOS_CMD_REG        0x10568 //MPM_C2PMSG_26
#define MPM_BIOS_TIMER_INTERVAL 10000000U //1S

UINT32
GetMpmBiosCmdRegOffset (
  UINT32 RegId
  )
{
  UINT32 mRegOffsetTbl[4] = {
   0x10500,// MPM_C2PMSG_0
   0x10560,// MPM_C2PMSG_24
   0x10564,// MPM_C2PMSG_25
   0x10568,// MPM_C2PMSG_26
  };

  return mRegOffsetTbl[RegId];
}

/**
  BIOS get command from MPM
  * @param[in,out]

   @retval EFI_STATUS              0: Success, NonZero Error
**/
VOID
EFIAPI
PollingMpmCmdCallback(
  IN  EFI_EVENT          Event,
  IN  VOID               *Context
)
{
  MPM_BIOS_MBOX MboxReg26;
  SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + MPM_BIOS_CMD_REG, &MboxReg26.Value);
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdGetPowerOperations MboxReg26 = 0x%x\n", MboxReg26.Value);
  if (MboxReg26.Field.CommandId !=0) {
    //BIOS set ready bit to 0
    MboxReg26.Field.Ready = 0;
    SmnRegisterWrite (0, SMN_MPMMMIOPUBLIC_BASE + MPM_BIOS_CMD_REG, &MboxReg26.Value, 0);
    switch (MboxReg26.Field.CommandId) {
      case MPM_BIOS_CMD_COLD_REBOOT:
        gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
        MboxReg26.Field.Status = MPM_BIOS_NO_ERROR;
        break;
      case MPM_BIOS_CMD_WARM_REBOOT:
        gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
        MboxReg26.Field.Status = MPM_BIOS_NO_ERROR;
        break;
      case MPM_BIOS_CMD_SHUT_DOWN:
        gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
        MboxReg26.Field.Status = MPM_BIOS_NO_ERROR;
        break;
      default:
        break;
    }
    //set Ready to 1, status, and clear CommandId
    MboxReg26.Field.Ready = 1;
    MboxReg26.Field.CommandId = 0;
    SmnRegisterWrite (0, SMN_MPMMMIOPUBLIC_BASE + MPM_BIOS_CMD_REG, &MboxReg26.Value, 0);
    return;
  }
  return;
}

/**
  BIOS polling power operations command from MPM in wireless KVM/TCR mode
  * @param[in,out]

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
RegisterMpmBiosCmdTimerEvent (VOID)
{
  EFI_STATUS   Status;
  EFI_EVENT    PollingPowerOperationEvent;
  //Create timer event to polling the power operation in KVM BIOS mode
  Status = gBS->CreateEvent (
             EVT_TIMER | EVT_NOTIFY_SIGNAL,
             TPL_CALLBACK,
             PollingMpmCmdCallback,
             NULL,
             &PollingPowerOperationEvent
             );
  if (!EFI_ERROR(Status)) {
    Status = gBS->SetTimer (PollingPowerOperationEvent, TimerPeriodic, MPM_BIOS_TIMER_INTERVAL);
    if (EFI_ERROR (Status)) {
      gBS->CloseEvent (PollingPowerOperationEvent);
      return Status;
    }
  }
  return Status;
}
