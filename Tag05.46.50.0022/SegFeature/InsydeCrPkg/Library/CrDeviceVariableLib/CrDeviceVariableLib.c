/** @file
  This Library will install CrDeviceVariableLib for reference.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <InternalCrDeviceVariableLib.h>

extern CHAR16  mVarName[];

extern UART_DEVICE_PATH              mUartNode;
extern UART_FLOW_CONTROL_DEVICE_PATH mFlowControlNode;
extern VENDOR_DEVICE_PATH            mTerminalNode;

//[-start-220208-IB04880004-add]//
CR_CONFIGURATION    *mCrConfig;
CR_DEVICE_PCD       *mPcdCrDevices;
UINTN               mDeviceCount;
UINTN               mOthersIndex = 0xff;

EFI_STATUS
CrDeviceSettingLoadDefault (
  IN     UINTN              DevIndex,
  IN OUT CR_DEVICE_SETTING  *Dev
  )
{
  EFI_STATUS          Status;
  CR_DEVICE_PCD       *PcdCrDevice;
  CHAR16              *TextDevPath;
  UINTN               VarSize;

  DEBUG ((DEBUG_INFO, "Cr << %a >>\n", __FUNCTION__));

  VarSize = 0;

  if (mPcdCrDevices == NULL) {
    Status = ParsingCrDevicePcdValue ( &mPcdCrDevices, &mDeviceCount);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "  ERROR!!!  ParsingCrDevicePcdValue Status=%r, 0x%lx\n", Status, Status));
      return EFI_DEVICE_ERROR;
    }
  }

  if (mCrConfig == NULL) {
    Status = CommonGetVariableDataAndSize (CONSOLE_REDIRECTION_VARSTORE_DEFAULT_NAME, &gH2OCrConfigurationGuid, &VarSize, (VOID**)(&mCrConfig));
    if (EFI_ERROR (Status) || (VarSize < sizeof (CR_CONFIGURATION))) {
      DEBUG ((DEBUG_ERROR, "  ERROR!!! CrConfigDefault variable not found. Load global default failed.\n"));
      return EFI_DEVICE_ERROR;
    }
  }

  DEBUG ((DEBUG_INFO, "  DevIndex=%d,  CrDevNode Name=%s\n", DevIndex, Dev->DevName));

  if (Dev->ItemType == ASTERISK_ITEM) {
    mOthersIndex = DevIndex;
    return EFI_SUCCESS;
  }

  if (DevIndex < mDeviceCount) {
    // Set in PcdCrDeviceList
    PcdCrDevice = &mPcdCrDevices[DevIndex];
    if (StrCmp (PcdCrDevice->DevName, Dev->DevName) != 0) {
      DEBUG ((DEBUG_ERROR, "  ERROR!!! PcdCrDevice->DevName != Dev->DevName\n"));
    } else {
    
      TextDevPath = ConvertDevicePathToText ( PcdCrDevice->DevPath, FALSE, FALSE);
      DEBUG ((DEBUG_INFO, "  PcdCrDevice DevPath=%s\n", TextDevPath));
      if (TextDevPath != NULL) {
        FreePool (TextDevPath);
      }
      
      ParsingDevicePath2Attribute (PcdCrDevice->DevPath, &Dev->Attribute);
      Dev->Attribute.PortEnable       = PcdCrDevice->PortEnable;
      Dev->Attribute.UseGlobalSetting = PcdCrDevice->UseGlobal;
    }
    
  } else {
    //
    // Others alway use global settings, The Enable is set depend on PcdCrDeviceList Others Enable field.

    //
    if (mOthersIndex >= mDeviceCount) {
     DEBUG ((DEBUG_ERROR, "  ERROR!!! PcdH2OCrDevice \"Others\" not found. The PnP serial devices enable/disable will keep old value.\n"));
     
    } else {
      PcdCrDevice = &mPcdCrDevices[mOthersIndex];
      Dev->Attribute.PortEnable = PcdCrDevice->PortEnable;
    }
    
    Dev->Attribute.UseGlobalSetting = TRUE;
    
    Dev->Attribute.TerminalType = mCrConfig->GlobalTerminalType;
    Dev->Attribute.BaudRate     = mCrConfig->GlobalBaudRate;
    Dev->Attribute.Parity       = mCrConfig->GlobalParity;
    Dev->Attribute.DataBits     = mCrConfig->GlobalDataBits;
    Dev->Attribute.StopBits     = mCrConfig->GlobalStopBits;
    Dev->Attribute.FlowControl  = mCrConfig->GlobalFlowControl;
  }

  return EFI_SUCCESS;
}
//[-end-220208-IB04880004-add]//

/**

  Set the N-th CrDevice Setting to variable.

  @param [IN] Index    The index of CrDevice Variable
  @param [IH]DevInfo   The CR device Setting.

  @retval EFI_SUCCESS     successfully
  @retval EFI_NOT_FOUND   The CrDevice[Index] was not found.

**/
EFI_STATUS
EFIAPI
UpdateOrDeleteCrDevVarWithDevInfo (
  IN  UINT8                    Index,
  IN  CR_DEVICE_SETTING        *DevInfo
  )
{
  EFI_STATUS         Status;
  UINTN              VarSize;
  CR_DEVICE_VAR      *CrDeviceVar;
  DPATH_UPDATE       UpdateType;

  if (DevInfo == NULL || Index >= MAX_CR_DEVICE_VARIABLE) {
    return EFI_INVALID_PARAMETER;
  }
  CrDeviceVar = NULL;
  Status = CommonGetVariableDataAndSize (CrDevVariableName (Index), &gH2OCrConfigurationGuid, &VarSize, (VOID**)(&CrDeviceVar));
  if (!EFI_ERROR (Status)) {

    if (CrDeviceVar->Exist == TRUE) {
      Status = CalculateNewVariableSize (&CrDeviceVar, &VarSize, DevInfo->Attribute.FlowControl, &UpdateType);
      if (EFI_ERROR (Status)) {
        goto exit;
      }
      Status = ParsingAttribute2DevicePath (GetDevicePathFromVariable (CrDeviceVar), &(DevInfo->Attribute), UpdateType);
      if (EFI_ERROR (Status)) {
        ASSERT (0);
        goto exit;
      }
    }

    //
    // Parsing other setting
    //
    CrDeviceVar->Exist      = DevInfo->Exist;
    CrDeviceVar->PortEnable = DevInfo->Attribute.PortEnable;
    CrDeviceVar->UseGlobal  = DevInfo->Attribute.UseGlobalSetting;

    //
    // Delete this variable when the device not in PCD list and non-exist
    //
    if (CrDeviceVar->Exist == FALSE && CrDeviceVar->SetInPCD == FALSE) {
      VarSize = 0;
    }

    //
    // Save CR Device Variable
    //
    Status = SetVariableToSensitiveVariable (
               mVarName,
               &gH2OCrConfigurationGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
               VarSize,
               (VOID *) CrDeviceVar
               );
    ASSERT (!EFI_ERROR(Status));

    DEBUG ((DEBUG_INFO, "  Update Variable %s : %s, Size:%d\n" ,mVarName, DevInfo->DevName, VarSize));
    DEBUG ((DEBUG_INFO, "    Exist:%d, ItemType:%d, PortEnable:%d, Global:%d, TerminalType:%d, BaudRate:%d, Parity:%d, DataBits:%d, StopBits:%d, FlowControl:%d\n",
             DevInfo->Exist,
             DevInfo->ItemType,
             DevInfo->Attribute.PortEnable,
             DevInfo->Attribute.UseGlobalSetting,
             DevInfo->Attribute.TerminalType,
             DevInfo->Attribute.BaudRate,
             DevInfo->Attribute.Parity,
             DevInfo->Attribute.DataBits,
             DevInfo->Attribute.StopBits,
             DevInfo->Attribute.FlowControl
             ));
  }
exit:
  if (CrDeviceVar != NULL) {
    FreePool (CrDeviceVar);
  }

  return Status;
}

EFI_DEVICE_PATH*
EFIAPI
GetDevicePathFromCrDevSetting (
  IN CR_DEVICE_SETTING *Buffer
  )
{
  return (EFI_DEVICE_PATH*)GetNextColumnPtr (Buffer->DevName);
}

/**

  Get the N-th CrDevice setting from variable.

  @param [IN] Index    The index of CrDevice Variable
  @param [OUT]DevInfo  The CR device Setting.
                       **This buffer is allocated with AllocatePool().It is the caller's responsibility
                       to call FreePool()when the caller no longer requires the contents of Buffer.

  @retval EFI_SUCCESS     successfully
  @retval EFI_NOT_FOUND   The CrDevice[Index] was not found.

**/
EFI_STATUS
EFIAPI
GetCrDevInfoFromVariable (
  IN  UINT8                    Index,
  OUT CR_DEVICE_SETTING        **DevInfo
  )
{
  EFI_STATUS         Status;
  UINTN              BufferSize;
  UINTN              DevPathSize;
  CR_DEVICE_VAR      *CrDeviceVar;
  EFI_DEVICE_PATH    *DevPath;
  CR_DEVICE_SETTING  *Buffer;

  if (DevInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Buffer      = NULL;
  CrDeviceVar = NULL;

  //
  // Get variable raw data
  //
  Status = CommonGetVariableDataAndSize (CrDevVariableName (Index), &gH2OCrConfigurationGuid, &BufferSize, (VOID**)(&CrDeviceVar));
  if ((!EFI_ERROR(Status)) && (CrDeviceVar != NULL)) {

    DevPath     = GetDevicePathFromVariable (CrDeviceVar);
    DevPathSize = GetDevicePathSize (DevPath);
    //
    // Allocate Device setting buffer
    //
    Buffer = (CR_DEVICE_SETTING *)AllocateZeroPool (
                                    sizeof (CR_DEVICE_SETTING) +
                                    StrSize (&CrDeviceVar->DevName) +
                                    DevPathSize
                                    );
    if (Buffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Parsing device path setting
    //
    if (CrDeviceVar->Exist == TRUE) {
      Status = ParsingDevicePath2Attribute (DevPath, &(Buffer->Attribute));
      if (EFI_ERROR (Status)) {
        ASSERT (0);
        FreePool (Buffer);
        goto exit;
      }
    }

    //
    // Parsing other setting
    //
    Buffer->Exist                       = CrDeviceVar->Exist;
    Buffer->ItemType                    = CrDeviceVar->ItmeType;
    Buffer->Attribute.PortEnable        = CrDeviceVar->PortEnable;
    Buffer->Attribute.UseGlobalSetting  = CrDeviceVar->UseGlobal;

    CopyMem(Buffer->DevName, &(CrDeviceVar->DevName), StrSize(&CrDeviceVar->DevName));
    CopyMem(GetDevicePathFromCrDevSetting (Buffer) , DevPath, DevPathSize);

    DEBUG ((DEBUG_INFO, " <%a> %s : %s\n", __FUNCTION__, mVarName, Buffer->DevName));
    DEBUG ((DEBUG_INFO, "   Exist:%d, ItemType:%d, PortEnable:%d, Global:%d, TerminalType:%d, BaudRate:%d, Parity:%d, DataBits:%d, StopBits:%d, FlowControl:%d\n",
               Buffer->Exist,
               Buffer->ItemType,
               Buffer->Attribute.PortEnable,
               Buffer->Attribute.UseGlobalSetting,
               Buffer->Attribute.TerminalType,
               Buffer->Attribute.BaudRate,
               Buffer->Attribute.Parity,
               Buffer->Attribute.DataBits,
               Buffer->Attribute.StopBits,
               Buffer->Attribute.FlowControl
               ));
    DEBUG ((DEBUG_INFO, "   DevicePath: %s\n", ConvertDevicePathToText ((const EFI_DEVICE_PATH *)DevPath, FALSE, TRUE)));
  }
exit:
  if (CrDeviceVar != NULL) {
    FreePool (CrDeviceVar);
  }
  *DevInfo = Buffer;

  return Status;
}

/**

  Create CrDevice variables from PcdCrDevices.

  @param [IN]LoadDefault If True then always restore variable from PCD.
                         If False then create variable frome PCD when the variable didn't exist

  @retval EFI_SUCCESS    successfully
  @retval Other          Fail.

**/
EFI_STATUS
EFIAPI
PcdDevicesToVariables (
  IN BOOLEAN LoadDefault
  )
{
  CR_DEVICE_PCD               *PcdCrDevice;
  UINTN                       Count;
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "\nCr << %a >> Load default %d\n", __FUNCTION__, LoadDefault));

  Status = ParsingCrDevicePcdValue (&PcdCrDevice, &Count);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = CreateDeviceVariableFromPcd (PcdCrDevice, Count, LoadDefault);
  if (!EFI_ERROR (Status)) {
    //
    // If load default setting, sync CrDevVar#### variables setting to CrConfig variable,
    // else always sync CrConfig variable setting to CrDevVar#### variables.
    //
    if (LoadDefault) {
      Status = SyncDeviceVarToCrConfigVar ();
    } else {
      Status = SyncCrConfigVarToDeviceVar ();
    }
  }

  FreeDevicePathPool (PcdCrDevice, Count);

  FreePool (PcdCrDevice);

  return Status;
}

EFI_STATUS
EFIAPI
CreateNewCrDeviceVariable (
  IN  CHAR16                    *DevName,
  IN  EFI_DEVICE_PATH           *DevPath
  )
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH          *NewDevPath;
  CR_DEVICE_VAR            *CrDeviceVar;
  UINT8                    *CrDeviceVarRaw8;
  UINTN                    DPathOffset;
  UINTN                    VarSize;
  UINTN                    BufferSize;
  UINTN                    Index;

  NewDevPath  = NULL;
  CrDeviceVar = NULL;
  BufferSize  = 0;

  for (Index = 0; Index < MAX_CR_DEVICE_VARIABLE; Index++) {
    if(!CheckCrDeviceVariableExist (CrDevVariableName (Index))) {
      break;
    }
  }
  if (Index == MAX_CR_DEVICE_VARIABLE) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (mFlowControlNode.FlowControlMap != 0) {
    NewDevPath = AppendMultiDevicePathNode (DevPath, 3, &mUartNode, &mFlowControlNode, &mTerminalNode);
  } else {
    NewDevPath = AppendMultiDevicePathNode (DevPath, 2, &mUartNode, &mTerminalNode);
  }

  VarSize     = CalculateCrDeviceVarSize (DevName, NewDevPath, &DPathOffset);
  CrDeviceVar = (CR_DEVICE_VAR*)PrepareEnoughCrDeviceVariable (CrDeviceVar, VarSize, &BufferSize);
  if (CrDeviceVar == NULL) {
    ASSERT (0);
    Status =  EFI_OUT_OF_RESOURCES;
    goto exit;
  }

  //
  // Parpare CR device header, name, device path.
  //
  CrDeviceVarRaw8         = (UINT8*)CrDeviceVar;
  CrDeviceVar->Exist      = TRUE;
  CrDeviceVar->UseGlobal  = TRUE;
  CrDeviceVar->PortEnable = FALSE;
  CrDeviceVar->ItmeType   = NORMAL_ITEM;
  CrDeviceVar->SetInPCD   = FALSE;

  CopyMem (&(CrDeviceVar->DevName), DevName, StrSize(DevName));
  CopyMem (&CrDeviceVarRaw8[DPathOffset], NewDevPath, GetDevicePathSize (NewDevPath));

  Status = SetVariableToSensitiveVariable (
             mVarName,
             &gH2OCrConfigurationGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             VarSize,
             (VOID*) CrDeviceVar
             );

  DEBUG ((DEBUG_INFO, "  Create New Variable %s : %s, Size:%d\n" ,mVarName, &CrDeviceVar->DevName, VarSize));
  DEBUG ((DEBUG_INFO, "    DPath: %s\n", ConvertDevicePathToText ((const EFI_DEVICE_PATH *)GetDevicePathFromVariable (CrDeviceVar), FALSE, TRUE)));

exit:
  if (NewDevPath != NULL) {
    FreePool(NewDevPath);
  }
  if (CrDeviceVar != NULL) {
    FreePool(CrDeviceVar);
  }
  return Status;
}

EFI_STATUS
EFIAPI
UpdateDeviceSettingToCrConfig (
  VOID
  )
{
  return SyncDeviceVarToCrConfigVar ();
}

/**

  For BIOS upgrade/downgrade case, if CrconFig variable size doesn't match with CR_CONFIGURATION structure size.
  Retrieve the default settings from default variable store region and create new CrConfig variable. Then, sync 
  the value from old CrConfig variale.

  @param [IN]*OldCrConfigVar  Pointer to old CrConfig variable buffer.
  @param [IN]OldCrConfigSize  The size of old CrConfig variable.

  @retval EFI_SUCCESS    Successfully
  @retval Other          Fail.

**/
EFI_STATUS
DealCrConfigVariableUpdate (
  IN UINT8          *OldCrConfigVar,
  IN UINTN          OldCrConfigSize
  )
{
  EFI_STATUS             Status;
  CR_CONFIGURATION       *NewCrConfig;
  UINTN                  NewVarSize;

  NewVarSize  = sizeof (CR_CONFIGURATION);
  NewCrConfig = (CR_CONFIGURATION*)AllocatePool (sizeof (CR_CONFIGURATION));
  if (NewCrConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Delete old CrConfig variable before excuting ExtractCrConfigDefault function to get default CR_COONFIG value.
  //
  Status = SetVariableToSensitiveVariable (
             CONSOLE_REDIRECTION_VARSTORE_NAME,
             &gH2OCrConfigurationGuid,
             0,
             0,
             NULL
             );
  if (EFI_ERROR (Status)) {
    FreePool (NewCrConfig);
    return Status;
  }
  
  ExtractCrConfigDefault ((UINT8*)NewCrConfig);

  if (OldCrConfigSize < NewVarSize) {
    CopyMem (NewCrConfig, OldCrConfigVar, OldCrConfigSize);
  } else {
    CopyMem (NewCrConfig, OldCrConfigVar, NewVarSize);
  }

  Status = SetVariableToSensitiveVariable (
             CONSOLE_REDIRECTION_VARSTORE_NAME,
             &gH2OCrConfigurationGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             sizeof (CR_CONFIGURATION),
             NewCrConfig
             );

  FreePool (NewCrConfig);

  //
  // Because the device setting is stored in CrDevVar### variables and earlier version of CR doesn't sync device setting to CrConfig variable
  // unless user has entered into Setup once. So, we need to check if CrDevVar### variables exists, sync the device setting to CrConfig variable.
  //
  Status = SyncDeviceVarToCrConfigVar ();

  return Status;
}

/**
  
  This function will lock CrDevVar### variables by EdkiiVariableLockProtocol..

  @retval EFI_SUCCESS    Successfully
  @retval Other          Fail.

**/
EFI_STATUS
LockCrDeviceVar (
  VOID
  )
{
  EFI_STATUS                    Status;
  CR_DEVICE_SETTING             *CrDevSetting;
  UINT8                         Index;
  EDKII_VARIABLE_LOCK_PROTOCOL  *VariableLock;

  DEBUG ((DEBUG_INFO, "\nCr << %a >> \n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gEdkiiVariableLockProtocolGuid,
                  NULL,
                  (VOID **) &VariableLock
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CrDeviceVariableLib] Fail to locate EdkiiVariableLockProtocol. (%r)\n", Status));
    return Status;
  }

  for (Index = 0;  Index < MAX_CR_DEVICE_VARIABLE; Index++) {
    CrDevSetting = NULL;
    Status = GetCrDevInfoFromVariable (Index, &CrDevSetting);
    if (EFI_ERROR(Status) || CrDevSetting == NULL) {
      continue;
    }

    Status = VariableLock->RequestToLock (VariableLock, mVarName, &gH2OCrConfigurationGuid);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[CrDeviceVariableLib] Fail to lock %s variable. (%r)\n", mVarName, Status));
    }

    FreePool (CrDevSetting);
  }

  return EFI_SUCCESS;
}

