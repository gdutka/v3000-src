/** @file
  BOOTService DXE driver for SMART Report Status Code

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/AtaPassThru.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SmmCommunication.h>

#include <Guid/EventGroup.h>
#include <Guid/PiSmmCommunicationRegionTable.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SmmReportStatusCodeHandler.h>
#include <Protocol/SmmBase2.h>
#include <Library/DevicePathLib.h>
#include <Library/H2OCpLib.h>


EFI_RSC_HANDLER_PROTOCOL      *mRscHandlerProtocol       = NULL;
EFI_SMM_RSC_HANDLER_PROTOCOL  *mSmmRscHandlerProtocol    = NULL;
EFI_SMM_SYSTEM_TABLE2         *mSmst                     = NULL;



/**
  Smart Status code worker that will handle reported SMART related status

  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or software entity.
                           This included information about the class and subclass that is used to
                           classify the entity as well as an operation.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to H2O DDT successfully.

**/
EFI_STATUS
EFIAPI
SmartStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )
{
  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;  
  SATA_DEVICE_PATH                     *SataDevPath;
  NVME_NAMESPACE_DEVICE_PATH           *NVMeDevPath;

  if (Value != (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_NOTSUPPORTED)  &&
      Value != (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_DISABLE)       &&
      Value != (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_OVERTHRESHOLD)  &&
      Value != (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_UNDERTHRESHOLD) 
      ) {
     return EFI_SUCCESS;
  }
  

  DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)(Data + 1);
  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH && DevicePath->SubType == MSG_SATA_DP) {
      SataDevPath = (SATA_DEVICE_PATH *) DevicePath;
      return EFI_SUCCESS;
    } 
    DevicePath = NextDevicePathNode (DevicePath);
  }


  DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)(Data + 1);
  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH && DevicePath->SubType == MSG_NVME_NAMESPACE_DP) {
      NVMeDevPath = (NVME_NAMESPACE_DEVICE_PATH *) DevicePath;
      return EFI_SUCCESS;
    } 
    DevicePath = NextDevicePathNode (DevicePath);
  }
  

  return EFI_SUCCESS;
}




/**
  Unregister status code callback functions only available at boot time from
  report status code router when exiting boot services.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.
**/
VOID
EFIAPI
UnregisterBootTimeHandlers (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  mRscHandlerProtocol->Unregister (SmartStatusCodeReportWorker);
}

/**
  Notification function for ReportStatusCode handler Protocol

  This routine is the notification function for SMART Registration Protocol

  @param EFI_EVENT              Event of the notification
  @param Context                not used in this function

  @retval none

**/
VOID
EFIAPI
RscHandlerCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS      Status;

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mRscHandlerProtocol
                  );

  if (Status == EFI_SUCCESS) {
    mRscHandlerProtocol->Register (SmartStatusCodeReportWorker, TPL_CALLBACK);
    Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UnregisterBootTimeHandlers,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mExitBootServicesEvent
                  );
    ASSERT_EFI_ERROR (Status);

    gBS->CloseEvent(Event);
  }
}

/**
  Notification function for ReportStatusCode handler Protocol

  This routine is the notification function for Irsi Registration Protocol

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
SmmRscHandlerCallBack (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  EFI_STATUS      Status;

  Status = mSmst->SmmLocateProtocol (
                  &gEfiSmmRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mSmmRscHandlerProtocol
                  );

  if (!EFI_ERROR(Status)) {
    Status = mSmmRscHandlerProtocol->Register (SmartStatusCodeReportWorker);
  }
  ASSERT_EFI_ERROR (Status);
  return Status;
}



/**
  Entry point of Smart Status Code Driver.

  This function is the entry point of this DXE Status Code Driver.
  It initializes registers status code handlers, and registers event for EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
SmartStatusHandlerSampleEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID                                *Registration;
  EFI_STATUS                          Status;
  EFI_SMM_BASE2_PROTOCOL              *SmmBase;
  BOOLEAN                             InSmm;

  
  InSmm        = FALSE;
  SmmBase      = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **)&SmmBase
                  );
  if (!EFI_ERROR (Status)) {
    SmmBase->InSmm (SmmBase, &InSmm);
  }

  if (!InSmm) {
    EfiCreateProtocolNotifyEvent (
      &gEfiRscHandlerProtocolGuid,
      TPL_CALLBACK,
      RscHandlerCallBack,
      NULL,
      &Registration
      );
  } else {

    //
    // In SMM mode
    //
    Status = SmmBase->GetSmstLocation(SmmBase, &mSmst);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = mSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mSmmRscHandlerProtocol
                    );
  
    if (!EFI_ERROR(Status)) {
      Status = mSmmRscHandlerProtocol->Register (SmartStatusCodeReportWorker);
      ASSERT_EFI_ERROR (Status);
    } else {
       Status = mSmst->SmmRegisterProtocolNotify (
                  &gEfiSmmRscHandlerProtocolGuid,
                  SmmRscHandlerCallBack,
                  &Registration
                  );
       ASSERT_EFI_ERROR (Status);
    }
  
  }

  return EFI_SUCCESS;
}
