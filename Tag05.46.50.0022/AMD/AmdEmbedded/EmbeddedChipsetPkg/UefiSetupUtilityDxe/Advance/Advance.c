/** @file

;******************************************************************************
;* Copyright (c) 2015 - 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "Advance.h"
//#include <Library/CrVfrConfigLib.h>
#include <Library/PcdLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Guid/AmdCbsConfig.h>
#include "AmdCbsVariable.h"
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/AdmiSecureBoot.h>
#include <Protocol/SmmBase2.h>
#include <Library/VariableLib.h>
#include <Protocol/NonVolatileVariable.h>
#include <Library/ApcbLibV3.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3TokenUid.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3Priority.h>
#include <Protocol/SmmCommunication.h>

EFI_CALLBACK_INFO                         *mAdvCallBackInfo;
extern EFI_GUID gAmdApcbDxeServiceProtocolGuid;

STATIC UINT8    mDefaultAthenData[] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xdd, 0x26, 0x02, 0x97, 0x9d, 0x44, 0x1f, 0x64, 0x95, 0xf8, 0x88, 0x46, 0xef, 0x9f, 0xb2,
  0xa1, 0x6c, 0xbc, 0x89, 0x1c, 0x29, 0xad, 0x4e, 0x5f, 0x67, 0xe0, 0xdb, 0xeb, 0x2f, 0xa4, 0xc0, 0x3f, 0x44, 0x72, 0x5c, 0x93, 0x39, 0x6d, 0xe5, 0x1c, 0xe2, 0x2b, 0xcb, 0x76, 0x11, 0x71, 0x4f,
  0xc7, 0xe7, 0xa2, 0xde, 0x1e, 0x07, 0x56, 0x92, 0xfc, 0x63, 0x61, 0xf1, 0x06, 0xbb, 0x77, 0x26, 0xb8, 0x99, 0x7c, 0x0e, 0xb8, 0x81, 0x24, 0x2e, 0x72, 0x78, 0x8c, 0xfa, 0x6a, 0x71, 0x5d, 0x08,
  0x42, 0xda, 0xde, 0x86, 0xfe, 0x99, 0x92, 0xf7, 0xf8, 0xa0, 0x3d, 0x1f, 0x52, 0x0b, 0xb8, 0xc6, 0x1b, 0xd5, 0x39, 0xcb, 0x34, 0xdb, 0x2b, 0x25, 0x57, 0x44, 0x72, 0x02, 0x93, 0x67, 0x2a, 0x82,
  0x90, 0xa2, 0x5a, 0x76, 0xcb, 0x81, 0x7d, 0x42, 0x39, 0x1a, 0xf8, 0x54, 0xe0, 0x42, 0xbf, 0x49, 0x36, 0x1c, 0x4e, 0x88, 0xda, 0x5c, 0x7b, 0xd4, 0x67, 0xf6, 0x82, 0x30, 0xe4, 0x18, 0xf9, 0xb3,
  0xe8, 0xab, 0xa4, 0xca, 0x53, 0x82, 0xcf, 0xf4, 0xac, 0x83, 0x4b, 0xd6, 0xd4, 0xa1, 0xc0, 0x01, 0xfc, 0x16, 0x9d, 0xca, 0xc5, 0x17, 0x2e, 0x70, 0xff, 0xaf, 0x86, 0xc8, 0x43, 0x02, 0xde, 0x52,
  0xb1, 0x23, 0xf0, 0x2f, 0xd8, 0x0a, 0x46, 0xd3, 0xaf, 0x6b, 0x7e, 0x77, 0xf6, 0x72, 0x00, 0x66, 0x69, 0x66, 0x37, 0x71, 0x19, 0xc7, 0x7b, 0xf0, 0xd1, 0x41, 0xfd, 0x2b, 0x9b, 0xa1, 0x14, 0xb5,
  0xb3, 0x55, 0xb9, 0x96, 0x53, 0x9f, 0x29, 0x63, 0xb2, 0x38, 0x2e, 0x29, 0x10, 0x16, 0x8a, 0xbd, 0x2e, 0xed, 0x3b, 0xdc, 0xb5, 0x40, 0x6f, 0xab, 0x26, 0xea, 0xb5, 0x28, 0x36, 0x6b, 0x4a, 0x68,
  0x85, 0xd2, 0xb1, 0xff, 0xbf, 0xe6, 0x0a, 0xd3, 0x8c, 0xe5, 0x01, 0xda, 0xfd, 0x65, 0x75, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

VOID
SetHddPswdModifiedFlag ();

EFI_STATUS
EFIAPI
UpdateAPCBWithTpmConfig (
  IN UINT8                         SystemTpmConfig
  )
{
  EFI_STATUS                      Status;
  UINT8                           ApcbTpmConfig;
  UINT8                           ApcbPurpose;
  BOOLEAN                         IsTpmConfigChanged;
  AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol;

  DEBUG ((DEBUG_INFO, "Enter %a: SystemTpmConfig %x \n", __FUNCTION__, SystemTpmConfig));

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, (VOID**)&ApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"Unable to locate APCB Protocol \n"));
    return Status;
  }

   // Since it is select on IBV Setup item, it need to sync back to PSP APOB correct value
  switch (SystemTpmConfig) {
    case 0:          // BIOS Setup Disable
      SystemTpmConfig = 0xFF;
      break;
    case 1:          // BIOS Setup d-TPM
      SystemTpmConfig = 0x00;
      break;
    case 2:          // BIOS Setup AMD PSP fTPM
      SystemTpmConfig = 0x01;
      break;
    case 3:          // BIOS Setup AMD HSP
      SystemTpmConfig = 0x02;
      break;
    default:
      SystemTpmConfig = 0xFF;
      break;
  }

  IsTpmConfigChanged = FALSE;
  ASSERT (ApcbDxeServiceProtocol != NULL);

  //step 2: sync TPM config
  //system TPM config is set by SBIOS in PEI phase
  //read APCB TPM config
  ApcbTpmConfig = SYSTEM_TPM_CONFIG_NONE;
  Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, &ApcbTpmConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"1.Unable to get APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG \n"));
  } else {
    if (SystemTpmConfig != ApcbTpmConfig) { //ABL TPM config is out of date, update it and reset system
      DEBUG ((DEBUG_INFO,"SystemTpmConfig: %d, ApcbTpmConfig: %d, update TPM config in APCB \n", SystemTpmConfig, ApcbTpmConfig));
      Status = ApcbDxeServiceProtocol->ApcbSetToken8 (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, SystemTpmConfig);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO,"Unable to set APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG \n"));
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, &ApcbTpmConfig);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO,"2.Unable to get APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG \n"));
        } else {
          if (SystemTpmConfig == ApcbTpmConfig) { //update successfully
            IsTpmConfigChanged = TRUE;
          } else { //update failed
            DEBUG ((DEBUG_INFO,"Critial error: update to APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG doesn't take effect\n"));
          }
        }
      }
    }
  }

  if (IsTpmConfigChanged) {
    //Write back updated data to SPI
    ApcbDxeServiceProtocol->ApcbFlushData (ApcbDxeServiceProtocol);
    DEBUG ((DEBUG_INFO,"TPM config changed...\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
EnableAmdCbsScpcSubOptions ( IN UINT8 ScpcLevels, IN BOOLEAN HspCoreEnable)
{
  EFI_STATUS Status;
  CBS_CONFIG *Buffer;
  UINTN      BufferSize;
  AMD_APCB_SERVICE_PROTOCOL      *ApcbDxeServiceProtocol;

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, (VOID**)&ApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Unable to locate APCB Protocol"));
    return Status;
  }
  Status = ApcbDxeServiceProtocol->ApcbAcquireMutex (ApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "ApcbAcquireMutex failed, Status:%r\n", Status));
    return Status;
  }
  Buffer       = NULL;
  BufferSize   = sizeof (CBS_CONFIG);
  Buffer  = AllocateZeroPool (BufferSize);

//[-start-230817-IB20840016-add]//
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
//[-end-230817-IB20840016-add]//

  Status = gRT->GetVariable (
                  CBS_SYSTEM_CONFIGURATION_NAME,
                  &gCbsSystemConfigurationGuid,
                  NULL,
                  &BufferSize,
                  Buffer
                  );
  if (Status == EFI_SUCCESS) {
    if (ScpcLevels == 3) {
      Buffer->CbsCmnMemTsmeEnableDdr = 1; //DDR5 for Lilac
      Buffer->CbsCmnMemTsmeLpddr = 1;     //LPDDR5 for Mayan
      Buffer->CbsCmnGpuHostTranslation = 1;
      Buffer->CbsCmnDmarSupport = 1;
      Buffer->CbsCmnDmaProtection = 1;
      ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TSME_ENABLE, 1);
      ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TSME_ENABLE_DDR, 1);
      ApcbDxeServiceProtocol->ApcbFlushData (ApcbDxeServiceProtocol);
      ApcbDxeServiceProtocol->ApcbReleaseMutex (ApcbDxeServiceProtocol);
    }
    if (ScpcLevels == 2) {
      Buffer->CbsCmnDmarSupport = 1;
      Buffer->CbsCmnDmaProtection = 1;
    }
    Buffer->CbsCmnGnbNbIOMMU = 1;
    if (HspCoreEnable) {
      Buffer->CbsCmnSoCMiscX86FwSupport = 1;
    }
    Status = gRT->SetVariable (
                  CBS_SYSTEM_CONFIGURATION_NAME,
                  &gCbsSystemConfigurationGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  BufferSize,
                  Buffer
                  );
  }
  FreePool (Buffer);
  return Status;
}

/**
  This function uses to check system whether supports SMM mode or not.

  @retval TRUE        System supports SMM mode
  @retval FALSE       System doesn't support SMM mode.
**/
STATIC
BOOLEAN
IsSmmModeSupported (
  VOID
  )
{
  EFI_SMM_BASE2_PROTOCOL              *SmmBase;
  BOOLEAN                             SmmSupport;
  EFI_STATUS                          Status;

  SmmSupport =FALSE;
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &SmmBase
                  );
  if (!EFI_ERROR (Status)) {
    SmmSupport = TRUE;
  }

  return SmmSupport;
}

/**
  Initial communication buffer header.

  @param[in] CommunicationBuffer         Input communication buffer.
  @param[in] CommunicationBufferSize     Size of communication buffer in bytes.
**/
STATIC
VOID
InitCommunicationBufferHeader (
  IN VOID         *CommunicationBuffer,
  IN UINTN        CommunicationBufferSize
  )
{
  EFI_SMM_COMMUNICATE_HEADER    *SmmCommunicateHeader;

  if (CommunicationBuffer == NULL || CommunicationBufferSize < SMM_COMMUNICATE_HEADER_SIZE + sizeof (SMM_VAR_BUFFER)) {
    return;
  }
  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) CommunicationBuffer;
  ZeroMem (SmmCommunicateHeader, SMM_COMMUNICATE_HEADER_SIZE + sizeof (SMM_VAR_BUFFER));
  CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gEfiSmmVariableProtocolGuid);
  SmmCommunicateHeader->MessageLength = SMM_COMMUNICATE_BUFFER_SIZE;
}

/**
 Using runtime data type memory to communicate information with variable driver
 in SMM.

 @param[in] InputBuff            Buffer to communicate information with variable driver.
 @param[in] DataSize             Size of the InPutBuff in bytes.
 @param[in] SubFunNum            sub function number.

 @retval EFI_SUCCESS             Communicate information with variable driver successfully.
 @retval EFI_OUT_OF_RESOURCES    There are not enough memory to communicate information.
**/
EFI_STATUS
SmmSecureBootCallWithRuntimeMemory (
  IN     UINT8            *InputBuff,
  IN     UINTN            DataSize,
  IN     UINT8            SubFunNum
  )
{
  NON_VOLATILE_VARIABLE_PROTOCOL     *NonVolatileVariableProtocol;
  VOID                               *VariableBuffer;
  UINTN                              VariableBufferSize;
  EFI_STATUS                         Status;
  EFI_TPL                            Tpl;
  EFI_SMM_COMMUNICATION_PROTOCOL     *SmmCommunication;
  SMM_VAR_BUFFER                     *SmmVarBuffer;

  Status = gBS->LocateProtocol (
               &gEfiNonVolatileVariableProtocolGuid,
               NULL,
               (VOID **) &NonVolatileVariableProtocol
               );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = gBS->LocateProtocol (
               &gEfiSmmCommunicationProtocolGuid,
               NULL,
               (VOID **) &SmmCommunication
               );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = NonVolatileVariableProtocol->GetRuntimeVariableBuffer (&VariableBuffer, &VariableBufferSize);
  if (EFI_ERROR (Status) || VariableBufferSize < DataSize) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Because the VariableBuffer will be used in many places, raise TPL to prevent from the data in this
  // buffer is destroyed in other events.
  //
  Tpl = gBS->RaiseTPL (TPL_NOTIFY);
  InitCommunicationBufferHeader (VariableBuffer, VariableBufferSize);
  SmmVarBuffer = (SMM_VAR_BUFFER *)((UINTN) VariableBuffer + SMM_COMMUNICATE_HEADER_SIZE);
  SmmVarBuffer->DataSize   = DataSize;
  SmmVarBuffer->AccessType = SubFunNum;
  SmmVarBuffer->Status = EFI_UNSUPPORTED;
  CopyMem (SmmVarBuffer + 1, InputBuff, DataSize);
  Status = SmmCommunication->Communicate (SmmCommunication, VariableBuffer, &VariableBufferSize);
  gBS->RestoreTPL (Tpl);
  return EFI_SUCCESS;
}

VOID
EnableSecureBoot ()
{
  UINTN BufferSize = sizeof (mDefaultAthenData);

  if (IsSmmModeSupported ()) {
     SmmSecureBootCallWithRuntimeMemory (
       mDefaultAthenData,
       BufferSize,
       RESTORE_FACTORY_DEFAULT_FUN_NUM
     );
  }
}
/**
 This is the callback function for the Advance Menu.

 @param [in]   This
 @param [in]   Action
 @param [in]   QuestionId
 @param [in]   Type
 @param [in]   Value
 @param [out]  ActionRequest


**/
EFI_STATUS
AdvanceCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                            Status;
  CHAR16                                *StringPtr;
  CHIPSET_CONFIGURATION                 *MyIfrNVData;
  EFI_HII_HANDLE                        HiiHandle;
  EFI_CALLBACK_INFO                     *CallbackInfo;
  EFI_INPUT_KEY                         Key;
  SETUP_UTILITY_CONFIGURATION           *SUCInfo;
  UINT8                                 DeviceKind;
  UINTN                                 BufferSize;
  EFI_GUID                              VarStoreGuid = SYSTEM_CONFIGURATION_GUID;
  UINT8                                 Levels = 0;
  BOOLEAN                               HspCoreEnable = 0;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return AdvanceCallbackRoutineByAction (This, Action, QuestionId, Type, Value, ActionRequest);
  }

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
  CallbackInfo = EFI_CALLBACK_INFO_FROM_THIS (This);
  BufferSize = GetVarStoreSize (CallbackInfo->HiiHandle, &CallbackInfo->FormsetGuid, &VarStoreGuid, "SystemConfig");

  BufferSize = PcdGet32 (PcdSetupConfigSize);
  Status = SetupVariableConfig (
             &VarStoreGuid,
             L"SystemConfig",
             BufferSize,
             (UINT8 *) gSUBrowser->SCBuffer,
             TRUE
             );

  MyIfrNVData = (CHIPSET_CONFIGURATION *) gSUBrowser->SCBuffer;
  Status    = EFI_SUCCESS;
  StringPtr = NULL;
  SUCInfo   = gSUBrowser->SUCInfo;

  HiiHandle = CallbackInfo->HiiHandle;


  switch (QuestionId) {

  case KEY_SERIAL_PORT_EN:
    //
    //  Check IRQ conflict between serial port and pci solt.
    //
    if (CheckSioAndPciSoltConflict (MyIfrNVData, (UINT8 *)&(MyIfrNVData->PciSlot4), &DeviceKind)) {
      StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(ISA_AND_PCISOLT_CONFLICT_STRING), NULL);
      gSUBrowser->H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, StringPtr);
      gBS->FreePool (StringPtr);
      switch (DeviceKind) {

      case 0:
        MyIfrNVData->ComPortA = 0;
        break;

      case 1:
        MyIfrNVData->ComPortB = 0;
        break;
      }
    }
    if (CheckSioConflict (MyIfrNVData)) {
      StringPtr=HiiGetString (HiiHandle, STRING_TOKEN(CONFLICT_STRING), NULL);
      gSUBrowser->H2ODialog->ConfirmDialog (
                               2,
                               FALSE,
                               0,
                               NULL,
                               &Key,
                               StringPtr
                               );
      gBS->FreePool (StringPtr);
    }
    break;

  case KEY_CHIPSET_EXTENDED_CONFIG:
    if (MyIfrNVData->ExtendedConfig == 0) {
      MyIfrNVData->SdRamFrequency    = 0;
      MyIfrNVData->SdRamTimeCtrl     = 0;
      MyIfrNVData->SdRamRasActToPre  = 6;
      MyIfrNVData->SdRamCasLatency   = 25;
      MyIfrNVData->SdRamRasCasDelay  = 3;
      MyIfrNVData->SdRamRasPrecharge = 3;
    }
    break;

  case KEY_CHIPSET_SDRAM_TIME_CTRL:
    switch (MyIfrNVData->SdRamTimeCtrl) {

    case 0:         // AUTO
      MyIfrNVData->SdRamRasActToPre = 6;
      MyIfrNVData->SdRamCasLatency = 25;
      MyIfrNVData->SdRamRasCasDelay = 3;
      MyIfrNVData->SdRamRasPrecharge = 3;
      break;

    case 1:         // MANUAL_AGRESSIVE
      MyIfrNVData->SdRamRasActToPre  = 5;
      MyIfrNVData->SdRamCasLatency   = 20;
      MyIfrNVData->SdRamRasCasDelay  = 2;
      MyIfrNVData->SdRamRasPrecharge = 2;
      break;
    }
    SUCInfo->DoRefresh = TRUE;
    break;

  case KEY_PARALLEL_PORT_EN:
    if (MyIfrNVData->Lpt1 > 1) {
      //
      // when LPT enable,check IRQ conflict
      //
      switch (CheckLptAndPciSoltConflict(MyIfrNVData, (UINT8 *)&(MyIfrNVData->PciSlot4))) {
      //
      // case 3: IRQ5 and IRQ7 have conflict.
      // case 2: IRQ7 have conflict.
      // case 1: IRQ5 have conflict.
      // case 0: NO conflict
      case 3:
        MyIfrNVData->Lpt1 = 0;
        StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(LPT_BOTH_IRQ_CONFLICT_STRING), NULL);
        gSUBrowser->H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, StringPtr);
        gBS->FreePool (StringPtr);
        break;
		
      case 1:
        StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(LPT_IRQ5_CONFLICT_STRING), NULL);
        gSUBrowser->H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, StringPtr);
        gBS->FreePool (StringPtr);
        break;
		
      case 2:
        StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(LPT_IRQ7_CONFLICT_STRING), NULL);
        gSUBrowser->H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, StringPtr);
        gBS->FreePool (StringPtr);
        break;
		
      default:
        break;
      }
    } else if (MyIfrNVData->Lpt1 == 1 ) {
      //
      // when LPT auto,check IRQ Conflict.
      //
      switch (CheckLptAndPciSoltConflict(MyIfrNVData, (UINT8 *)&(MyIfrNVData->PciSlot4))) {
        
		case 3:
          MyIfrNVData->Lpt1 = 0;
          StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(LPT_BOTH_IRQ_CONFLICT_STRING), NULL);
          gSUBrowser->H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, StringPtr);
          gBS->FreePool (StringPtr);
          break;
		  
        default:
          break;
      }
    }
    break;
	
  case KEY_PCI_SLOT_4_IRQ_SET:
  case KEY_PCI_SLOT_5_IRQ_SET:
  case KEY_PCI_SLOT_6_IRQ_SET:
    if (CheckPciSioConflict (MyIfrNVData, (UINT8 *)&(MyIfrNVData->PciSlot4), &DeviceKind)) {
   	  switch (DeviceKind) {

      case 1:
        StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(COMPORTA_CONFLICT_STRING), NULL);
        break;
		
      case 2:
        StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(COMPORTB_CONFLICT_STRING), NULL);
        break;
		
      case 3:
        StringPtr = HiiGetString (HiiHandle, STRING_TOKEN(PARALLEL_PORT_CONFLICT_STRING), NULL);
        break;
		
      }
      gSUBrowser->H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, StringPtr);
      gBS->FreePool (StringPtr);
    }
    break;

  case KEY_UMA_MODE:
    SUCInfo->DoRefresh = TRUE;
  break;

  case KEY_PLUG_IN_DISPLAY_SELECTION1:
  case KEY_PLUG_IN_DISPLAY_SELECTION2:
    PlugInVideoDisplaySelectionOption (QuestionId, Value);
    break;

//[-start-220329-IB14740229-remove]//
//  case USB_CONTROLLER0_PORT0:
//    MyIfrNVData->UsbPort1 = 0;
//    MyIfrNVData->UsbPort2 = 0;
//    MyIfrNVData->UsbPort3 = 0;
//    break;
//  case USB_CONTROLLER1_PORT0:
//    MyIfrNVData->UsbPort5 = 0;
//    MyIfrNVData->UsbPort6 = 0;
//    MyIfrNVData->UsbPort7 = 0;
//    break;
//[-end-220329-IB14740229-remove]//

  case SCPC_LEVELS_ID:
    Levels = MyIfrNVData->ScpcLevels;
    if ((MyIfrNVData->ScpcLevels > 3) && (MyIfrNVData->ScpcLevels < 7)) {
      Levels = MyIfrNVData->ScpcLevels - 3;
      HspCoreEnable = 1;
    }
    if ((MyIfrNVData->ScpcLevels > 6) && (MyIfrNVData->ScpcLevels < 10)) {
      Levels = MyIfrNVData->ScpcLevels - 6;
      HspCoreEnable = 1;
    }
    if ((MyIfrNVData->ScpcLevels > 9) && (MyIfrNVData->ScpcLevels < 12)) {
      Levels = MyIfrNVData->ScpcLevels - 9;
    }

//[-start-230817-IB20840016-modify]//
    Status = PcdSet8S (PcdScpcLevel, Levels);
    ASSERT_EFI_ERROR (Status);
//[-end-230817-IB20840016-modify]//
    //Turn on the level3 sub-options: dTPM, Smm Isolation, SecureBIO, SMM Paging, SVM
    switch (Levels) {
      case 1:
        MyIfrNVData->EnableTPM = 1;
        if ((MyIfrNVData->ScpcLevels > 3) && (MyIfrNVData->ScpcLevels < 7)) {
          MyIfrNVData->EnableTPM = 3;
        }
        if ((MyIfrNVData->ScpcLevels > 9) && (MyIfrNVData->ScpcLevels < 12)) {
          MyIfrNVData->EnableTPM = 2;
        }
        MyIfrNVData->EnableSVM = 1;
        MyIfrNVData->SVMLock = 1;
        MyIfrNVData->SmmIsolationSupport = 0;
        // MyIfrNVData->VirtualDrtmDeviceSupport = 0;
//[-start-230817-IB20840016-modify]//
        Status = PcdSetBoolS (PcdCfgIommuSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
//[-end-230817-IB20840016-modify]//
        EnableAmdCbsScpcSubOptions (1, HspCoreEnable);
        break;

      case 2:
        MyIfrNVData->EnableTPM = 1;
        if ((MyIfrNVData->ScpcLevels > 3) && (MyIfrNVData->ScpcLevels < 7)) {
          MyIfrNVData->EnableTPM = 3;
        }
        if ((MyIfrNVData->ScpcLevels > 9) && (MyIfrNVData->ScpcLevels < 12)) {
          MyIfrNVData->EnableTPM = 2;
        }
        MyIfrNVData->EnableSVM = 1;
        MyIfrNVData->SVMLock = 1;
        MyIfrNVData->SecureBioCamSupport = 1;
        MyIfrNVData->SmmIsolationSupport = 0;
        // MyIfrNVData->VirtualDrtmDeviceSupport = 0;
//[-start-230817-IB20840016-modify]//
        Status = PcdSetBoolS (PcdCfgIommuSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdIvInfoDmaReMap, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdDmaProtection, TRUE);
        ASSERT_EFI_ERROR (Status);
//[-end-230817-IB20840016-modify]//
        EnableAmdCbsScpcSubOptions (2, HspCoreEnable);
        EnableSecureBoot ();
        break;

      case 3:
        MyIfrNVData->EnableTPM = 1;
        if ((MyIfrNVData->ScpcLevels > 3) && (MyIfrNVData->ScpcLevels < 7)) {
          MyIfrNVData->EnableTPM = 3;
        }
        MyIfrNVData->SmmIsolationSupport = 1;
        MyIfrNVData->EnableSVM = 1;
        MyIfrNVData->SVMLock = 1;
        MyIfrNVData->SMMCoreLock = 1;
        MyIfrNVData->EnableSmmProtection = 1;
        // MyIfrNVData->VirtualDrtmDeviceSupport = 1;
        MyIfrNVData->SecureBioCamSupport = 1;
//[-start-230817-IB20840016-modify]//
        Status = PcdSetBoolS (PcdCfgIommuSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdIvInfoDmaReMap, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdDmaProtection, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdModernStandbyEnableForLevel3, TRUE);
        ASSERT_EFI_ERROR (Status);   
//[-end-230817-IB20840016-modify]//
        EnableAmdCbsScpcSubOptions (3, HspCoreEnable);
        EnableSecureBoot ();
        break;

      default:
        break;
    }
    break;

  case ENABLE_TPM_ID:
     UpdateAPCBWithTpmConfig(MyIfrNVData->EnableTPM);
    break;

  default:
    Status = HotKeyCallBack (
               This,
               Action,
               QuestionId,
               Type,
               Value,
               ActionRequest
               );
    break;
  }

  BufferSize = PcdGet32 (PcdSetupConfigSize);
  SetupVariableConfig (
    &VarStoreGuid,
    L"SystemConfig",
    BufferSize,
    (UINT8 *) gSUBrowser->SCBuffer,
    FALSE
    );

  return Status;
}

EFI_STATUS
AdvanceCallbackRoutineByAction (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                            Status;
  EFI_CALLBACK_INFO                     *CallbackInfo;
  UINTN                                 BufferSize;
  EFI_GUID                              VarStoreGuid = SYSTEM_CONFIGURATION_GUID;

  if ((This == NULL) ||
      ((Value == NULL) &&
       (Action != EFI_BROWSER_ACTION_FORM_OPEN) &&
       (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
      (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
  CallbackInfo   = EFI_CALLBACK_INFO_FROM_THIS (This);
  BufferSize     = GetVarStoreSize (CallbackInfo->HiiHandle, &CallbackInfo->FormsetGuid, &VarStoreGuid, "SystemConfig");
  Status         = EFI_UNSUPPORTED;

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    if (QuestionId == 0) {
      Status = SetupVariableConfig (
                 &VarStoreGuid,
                 L"SystemConfig",
                 BufferSize,
                 (UINT8 *) gSUBrowser->SCBuffer,
                 FALSE
                 );
    }
    break;

  case EFI_BROWSER_ACTION_FORM_CLOSE:
    if (QuestionId == 0) {
      Status = SetupVariableConfig (
                 &VarStoreGuid,
                 L"SystemConfig",
                 BufferSize,
                 (UINT8 *) gSUBrowser->SCBuffer,
                 TRUE
                 );
    }
    break;

  case EFI_BROWSER_ACTION_CHANGING:
    Status = EFI_SUCCESS;
    break;

  case EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING:
    if (QuestionId == KEY_SCAN_F9) {
      Status = HotKeyCallBack (
                This,
                Action,
                QuestionId,
                Type,
                Value,
                ActionRequest
                );
      Status = SetupVariableConfig (
                  &VarStoreGuid,
                  L"SystemConfig",
                  BufferSize,
                  (UINT8 *) gSUBrowser->SCBuffer,
                  FALSE
                  );
    }
    //
    // avoid GetQuestionDefault execute ExtractConfig
    //
    return EFI_SUCCESS;

  default:
    break;
  }

  return Status;
}

EFI_STATUS
InstallAdvanceCallbackRoutine (
  IN EFI_HANDLE                             DriverHandle,
  IN EFI_HII_HANDLE                         HiiHandle
  )
{
  EFI_STATUS                                Status;
  EFI_GUID                                  FormsetGuid = FORMSET_ID_GUID_ADVANCE;

  mAdvCallBackInfo = AllocatePool (sizeof(EFI_CALLBACK_INFO));
  if (mAdvCallBackInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mAdvCallBackInfo->Signature                    = EFI_CALLBACK_INFO_SIGNATURE;
  mAdvCallBackInfo->DriverCallback.ExtractConfig = gSUBrowser->ExtractConfig;
  mAdvCallBackInfo->DriverCallback.RouteConfig   = gSUBrowser->RouteConfig;
  mAdvCallBackInfo->DriverCallback.Callback      = AdvanceCallbackRoutine;
  mAdvCallBackInfo->HiiHandle                    = HiiHandle;
  
  CopyGuid (&mAdvCallBackInfo->FormsetGuid, &FormsetGuid);

  //
  // Install protocol interface
  //
  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAdvCallBackInfo->DriverCallback
                  );
  ASSERT_EFI_ERROR (Status);

  Status = InitAdvanceMenu (HiiHandle);

  return Status;
}

EFI_STATUS
UninstallAdvanceCallbackRoutine (
  IN EFI_HANDLE                             DriverHandle
  )
{
  EFI_STATUS     Status;

  if (mAdvCallBackInfo == NULL) {
    return EFI_SUCCESS;
  }
  Status = gBS->UninstallProtocolInterface (
                  DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mAdvCallBackInfo->DriverCallback
                  );
  ASSERT_EFI_ERROR (Status);
  gBS->FreePool (mAdvCallBackInfo);
  mAdvCallBackInfo = NULL;
  return Status;
}

EFI_STATUS
InitAdvanceMenu (
  IN EFI_HII_HANDLE                         HiiHandle
  )
{
  EFI_STATUS                                Status;

//[-start-220422-IB14740235-add]//
  Status = EFI_SUCCESS;
//[-end-220422-IB14740235-add]//

  //
  // Check and update IDE configuration.
  //
  gSUBrowser->SUCInfo->PrevSataCnfigure = ((CHIPSET_CONFIGURATION *)gSUBrowser->SCBuffer)->SataCnfigure;

//[-start-220422-IB14740235-remove]//
//  Status = InitSataConfig (
//             HiiHandle,
//             (CHIPSET_CONFIGURATION *)gSUBrowser->SCBuffer
//             );
//  ASSERT_EFI_ERROR (Status);
//
//  Status = InitNvmeConfig (
//             HiiHandle,
//             (CHIPSET_CONFIGURATION *)gSUBrowser->SCBuffer
//             );
//  ASSERT_EFI_ERROR (Status);
//[-end-220422-IB14740235-remove]//

  //
  // Auto detect available output device.
  //
  PlugInVideoDisplaySelectionLabel (HiiHandle);

  return Status;
}
