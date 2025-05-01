/** @file
  CrVariableUpdate

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
#include "CrVariableUpdate.h"

CHAR16                       *mConInVarName  = L"ConIn";
CHAR16                       *mConOutVarName = L"ConOut";
EFI_GUID                     *mConVarGuid    = &gEfiGlobalVariableGuid;
CR_DEVICES_SETTING_HEAD_NODE mCrVarDeviceHead;

extern UINT32                       mCrBaudRateTable[];

VOID
InitializeCrDeviceList (
  VOID
  )
{
  InitializeListHead (&mCrVarDeviceHead.Link);
  mCrVarDeviceHead.CrDevVarCount = 0;
}

/**
  Status is Exist and Port enable

  @param

  @retval

**/
EFI_DEVICE_PATH*
BuildCrActiveDevicePath (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *DevPath;
  EFI_DEVICE_PATH_PROTOCOL    *PrevDevPath;
  UINTN                       DeviceCount;

  LIST_ENTRY                  *Link;
  CR_DEVICES_SETTING_NODE     *CrDevNode;

  DevPath     = NULL;
  PrevDevPath = NULL;
  DeviceCount = 0;

  Link = GetFirstNode (&mCrVarDeviceHead.Link);

  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);
    //
    // **Note: The network device is independent control.
    //         So didn't add device path here.
    //
    if ( CrDevNode->CrSetting->Exist                == FALSE   ||
         CrDevNode->CrSetting->Attribute.PortEnable == FALSE
         ) {
      Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
      continue;
    }
    DevPath = AppendDevicePathInstance (PrevDevPath, GetDevicePathFromCrDevSetting (CrDevNode->CrSetting));
    CrSafeFreePool (PrevDevPath);
    PrevDevPath = DevPath;

    CrDevNode->AddIntoConVar = TRUE;
    Link                     = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }
  return DevPath;
}

//[-start-211214-IB18180097-add]//
/**
  Return the device path instances which
  are in CR list from console variable

**/
EFI_DEVICE_PATH*
BuildCrExclusiveDevicePath (
  IN  CHAR16                    *ConVarName,
  IN  EFI_GUID                  *ConVarGuid
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *VarConsoleInstance;
  EFI_DEVICE_PATH_PROTOCOL    *VarConsoleHead;
  EFI_DEVICE_PATH_PROTOCOL    *VarConsole;
  EFI_DEVICE_PATH_PROTOCOL    *PrevDevPath;
  EFI_DEVICE_PATH_PROTOCOL    *DevPath;
  UINTN                       DevicePathSize;
  UINTN                       VarSize;
  EFI_STATUS                  Status;

  DevPath     = NULL;
  PrevDevPath = NULL;
  VarSize     = 0;

  Status = CommonGetVariableDataAndSize (ConVarName, ConVarGuid, &VarSize, (VOID**)(&VarConsoleHead));
  if (EFI_ERROR (Status)) {
    return NULL;
  }
    
  VarConsole     = VarConsoleHead;

  while (VarConsole != NULL) {
    
    VarConsoleInstance = GetNextDevicePathInstance (&VarConsole, &DevicePathSize);
    if (VarConsoleInstance != NULL) {
      if (IsUartDevice (VarConsoleInstance) && IsCrDevice (VarConsoleInstance)) { 
        PrevDevPath = DevPath;
        DevPath     = AppendDevicePathInstance (PrevDevPath, VarConsoleInstance);
        CrSafeFreePool (PrevDevPath);
      }
    }
  }

  CrSafeFreePool (VarConsoleHead);

  return DevPath;
}
//[-end-211214-IB18180097-add]//

BOOLEAN
IsDevNodeInvalidOrAreadyUsed (
  IN CR_DEVICES_SETTING_NODE     *CrDevNode
  )
{
  if (CrDevNode->CrSetting->ItemType == ASTERISK_ITEM  ||
      CrDevNode->CrSetting->ItemType == INVALID_ITEM   ||
      CrDevNode->AddIntoConVar       == TRUE
    ) {
    return TRUE;
  }
  return FALSE;
}

VOID
UpdateDevicesExistStatus (
  VOID
  )
{
  LIST_ENTRY                  *Link;
  CR_DEVICES_SETTING_NODE     *CrDevNode;
  EFI_DEVICE_PATH             *DevPath;
  EFI_HANDLE                  Handle;
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "\nCr << %a >>\n", __FUNCTION__));

  Link   = GetFirstNode (&mCrVarDeviceHead.Link);
  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);

    if (IsDevNodeInvalidOrAreadyUsed (CrDevNode) ) {
      Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
      continue;
    }

    DevPath = GetDevicePathFromCrDevSetting (CrDevNode->CrSetting);
    Status = gBS->LocateDevicePath (&gH2OCrUart16550AccessProtocolGuid, &DevPath, &Handle);

    DEBUG ((DEBUG_INFO, " Not used CrDevVar%d : %s, Locate 16550:%r\n", CrDevNode->VarIndex, CrDevNode->CrSetting->DevName, Status));

    if (Status == EFI_SUCCESS && CrDevNode->CrSetting->Exist == FALSE) {
      DEBUG ((DEBUG_INFO, "  ->Now appear\n"));
      CrDevNode->CrSetting->Exist = TRUE;
      UpdateOrDeleteCrDevVarWithDevInfo (CrDevNode->VarIndex ,CrDevNode->CrSetting);

    } else if (EFI_ERROR(Status) && CrDevNode->CrSetting->Exist == TRUE){
      DEBUG ((DEBUG_INFO, "  ->Now disappear\n"));
      CrDevNode->CrSetting->Exist = FALSE;
      UpdateOrDeleteCrDevVarWithDevInfo (CrDevNode->VarIndex ,CrDevNode->CrSetting);
    }

    Link = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }
}

VOID
CheckAndSaveConnectResult (
  IN  CHAR16                    *ConVarName,
  IN  EFI_GUID                  *ConVarGuid
  )
{
  EFI_STATUS                  Status;
  UINTN                       VarSize;
  LIST_ENTRY                  *Link;
  CR_DEVICES_SETTING_NODE     *CrDevNode;
  EFI_DEVICE_PATH             *VarConsole;

  DEBUG ((DEBUG_INFO, "\nCr << %a >>\n", __FUNCTION__));

  VarConsole = NULL;
  VarSize    = 0;

  Status = CommonGetVariableDataAndSize ( 
             ConVarName, 
             ConVarGuid, 
             &VarSize, 
             (VOID **)&VarConsole
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  Link = GetFirstNode (&mCrVarDeviceHead.Link);
  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);

    if (CrDevNode->AddIntoConVar == FALSE) {
      Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
      continue;
    }

    if (!IsInclusiveOfSelectDevPath (VarConsole, GetDevicePathFromCrDevSetting (CrDevNode->CrSetting))) {
      DEBUG ((EFI_D_INFO, " CrDevVar%d :%s not exist\n", CrDevNode->VarIndex, CrDevNode->CrSetting->DevName));
      CrDevNode->AddIntoConVar                   = FALSE;
      CrDevNode->CrSetting->Exist                = FALSE;
      CrDevNode->CrSetting->Attribute.PortEnable = FALSE;
      //
      // If device not exist and this device not in PCD, then delete this variable
      //
      UpdateOrDeleteCrDevVarWithDevInfo (CrDevNode->VarIndex ,CrDevNode->CrSetting);
      //
      // Sync CR device variable to CrConfig variable
      //
      UpdateDeviceSettingToCrConfig ();
    }

    Link = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }
  CrSafeFreePool (VarConsole);
}

VOID
CreateCrDeviceVarList (
  VOID
  )
{
  UINT8                       Index;
  CR_DEVICE_SETTING           *CrSetting;
  CR_DEVICES_SETTING_NODE     *Node;
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "\nCr << %a >>\n", __FUNCTION__));

  if (!mCrEnable) {
    mCrVarDeviceHead.CrDevVarCount = 0;
    return;
  }

  for (Index = 0; Index < MAX_CR_DEVICE_VARIABLE; Index++) {
    Status = GetCrDevInfoFromVariable (Index, &CrSetting);
    if (EFI_ERROR(Status) || CrSetting == NULL) {
      continue;
    }

    Node = AllocateZeroPool (sizeof (CR_DEVICES_SETTING_NODE));
    if (Node == NULL) {
      CrSafeFreePool (CrSetting);
      ASSERT (0);
      break;
    }

    Node->CrSetting = CrSetting;
    Node->VarIndex  = Index;
    InsertTailList (&mCrVarDeviceHead.Link, &Node->Link);
    mCrVarDeviceHead.CrDevVarCount ++;
  }
}

VOID
FreeCrDevicesVarList (
  VOID
  )
{
  LIST_ENTRY                  *Link;
  CR_DEVICES_SETTING_NODE     *CrDevNode;

  Link = GetFirstNode (&mCrVarDeviceHead.Link);

  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);
    Link      = RemoveEntryList (Link);
    gBS->FreePool (CrDevNode->CrSetting);
    gBS->FreePool (CrDevNode);
  }
}

//[-start-211214-IB18180097-remove]//
//VOID
//CreateNewActiveCrDevice (
//  VOID
//  )
//{
//  EFI_DEVICE_PATH   *NewCrActiveDevice;
//  UINTN             Size;
//
//  NewCrActiveDevice = NULL;
//  NewCrActiveDevice = BuildCrActiveDevicePath ();
//
//  Size = (NewCrActiveDevice == NULL)? 0 : GetDevicePathSize (NewCrActiveDevice);
//
//  CommonSetVariable (
//    CR_ACTIVE_VARIABLE_NAME,
//    &gH2OCrConfigurationGuid,
//    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
//    Size,
//    NewCrActiveDevice
//    );
//  CrSafeFreePool (NewCrActiveDevice);
//}
//[-end-211214-IB18180097-remove]//

BOOLEAN
IsChildDevicePath (
  IN EFI_DEVICE_PATH  *ChildDevPath,
  IN EFI_DEVICE_PATH  *ParentDevPath
  )
{
  UINTN  Size;

  Size = GetDevicePathSize (ParentDevPath) - sizeof (EFI_DEVICE_PATH);

  if (CompareMem (ChildDevPath, ParentDevPath, Size) == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
IsDevicePathAlreadyInCrDevVar (
  IN EFI_DEVICE_PATH  *SubsetPath
  )
{
  LIST_ENTRY                  *Link;
  CR_DEVICES_SETTING_NODE     *CrDevNode;

  Link = GetFirstNode (&mCrVarDeviceHead.Link);

  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);

    if (CrDevNode->CrSetting->ItemType == INVALID_ITEM  ||
        CrDevNode->CrSetting->ItemType == ASTERISK_ITEM
        ) {
      Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
      continue;
    }

    if (IsChildDevicePath (GetDevicePathFromCrDevSetting (CrDevNode->CrSetting), SubsetPath)) {
      return TRUE;
    }
    Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }
  return FALSE;
}

BOOLEAN
IsChildDevPathOfAsteriskDevPath (
  IN  EFI_DEVICE_PATH *ChildDevPath
  )
{
  LIST_ENTRY                  *Link;
  CR_DEVICES_SETTING_NODE     *CrDevNode;

  Link = GetFirstNode (&mCrVarDeviceHead.Link);

  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);

    if (CrDevNode->CrSetting->ItemType == ASTERISK_ITEM) {
      return IsChildDevicePath (ChildDevPath, GetDevicePathFromCrDevSetting (CrDevNode->CrSetting) );
    }
    Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }
  return FALSE;

}

VOID
FindAsteriskDevicePath (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  EFI_DEVICE_PATH                *ChildDevPath;
  UINTN                          HandleCount;
  UINTN                          Index;
  H2O_UART_16550_ACCESS_PROTOCOL *Interface;

  DEBUG ((DEBUG_INFO, "\nCr << %a >>\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gH2OCrUart16550AccessProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID **)&ChildDevPath);
    DEBUG ((DEBUG_INFO, " 16550 DevPath: %s\n", ConvertDevicePathToText ((const EFI_DEVICE_PATH *)ChildDevPath, FALSE, TRUE)));
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (IsDevicePathAlreadyInCrDevVar (ChildDevPath)) {
      DEBUG ((EFI_D_INFO, "  ->AlreadyInCrDevVar\n"));
      continue;
    }

    if (IsChildDevPathOfAsteriskDevPath (ChildDevPath) == FALSE) {
      DEBUG ((EFI_D_INFO, "  ->Not Child DPath Of Asterisk DPath.\n"));
      continue;
    }

    Status = gBS->HandleProtocol (HandleBuffer[Index], &gH2OCrUart16550AccessProtocolGuid, (VOID **)&Interface);
    if (EFI_ERROR(Status)) {
      continue;
    }

    CreateNewCrDeviceVariable (
      Interface->DeviceInfo->DeviceName,
      ChildDevPath
      );

    mCrVarDeviceHead.CrDevVarCount++;
  }

  CrSafeFreePool(HandleBuffer);
}


/**


  @param

  @retval

**/
VOID
//[-start-211214-IB18180097-modify]//
UpdateConInConOutVar (
//[-end-211214-IB18180097-modify]//
  VOID
  )
{
//[-start-211214-IB18180097-modify]//
  EFI_DEVICE_PATH   *ActiveDevPath;
  EFI_DEVICE_PATH   *ConInExclusive;
  EFI_DEVICE_PATH   *ConOutExclusive;

  ActiveDevPath     = NULL;
  ConInExclusive    = NULL;
  ConOutExclusive   = NULL;

  ActiveDevPath     = BuildCrActiveDevicePath ();
  ConInExclusive    = BuildCrExclusiveDevicePath (mConInVarName,  mConVarGuid);
  ConOutExclusive   = BuildCrExclusiveDevicePath (mConOutVarName, mConVarGuid);

  if (mCrVarDeviceHead.CrDevVarCount == 0 || mCrEnable == FALSE) {
    
    //
    // Delete Console Redirection device path from ComIn/Out Variable
    //
    
    UpdateConsoleVariable (mConInVarName,  mConVarGuid, NULL, ConInExclusive);
    UpdateConsoleVariable (mConOutVarName, mConVarGuid, NULL, ConOutExclusive);
  } else {
  
    //
    // Update Console Redirection device path from ComIn/Out Variable
    //
    
    UpdateConsoleVariable (mConInVarName,  mConVarGuid, ActiveDevPath, ConInExclusive);
    UpdateConsoleVariable (mConOutVarName, mConVarGuid, ActiveDevPath, ConOutExclusive);
  }

  CrSafeFreePool (ActiveDevPath);
  CrSafeFreePool (ConInExclusive);
  CrSafeFreePool (ConOutExclusive);
//[-end-211214-IB18180097-modify]//
}


EFI_STATUS
UpdateConsoleVariable (
  IN  CHAR16                    *ConVarName,
  IN  EFI_GUID                  *ConVarGuid,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
  )
{
  EFI_STATUS                Status;
  UINTN                     VarSize;
  UINTN                     DevicePathSize;
  BOOLEAN                   VarChanged;
  EFI_DEVICE_PATH_PROTOCOL  *VarConsole;
  EFI_DEVICE_PATH_PROTOCOL  *OldVarConsole;
  EFI_DEVICE_PATH_PROTOCOL  *NewDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *PrevDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *VarConsoleInstance;
  EFI_DEVICE_PATH_PROTOCOL  *CustomizedInstance;


  VarChanged      = FALSE;
  VarConsole      = NULL;
  DevicePathSize  = 0;
  NewDevicePath   = NULL;
  Status          = EFI_UNSUPPORTED;
  VarSize         = 0;

  if (CustomizedConDevicePath == ExclusiveDevicePath) {
    return EFI_UNSUPPORTED;
  }

  Status = CommonGetVariableDataAndSize ( 
             ConVarName, 
             ConVarGuid, 
             &VarSize, 
             (VOID **)&VarConsole
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Get variable %s: %r.\n", __FUNCTION__, ConVarName, Status));
    return Status;
  }

  OldVarConsole = VarConsole;

  if (ExclusiveDevicePath != NULL && VarConsole != NULL) {
    while (VarConsole != NULL) {

      VarConsoleInstance = GetNextDevicePathInstance (&VarConsole, &DevicePathSize);
      if (VarConsoleInstance != NULL) {
        if (!IsInclusiveOfSelectDevPath (ExclusiveDevicePath, VarConsoleInstance)) {
          PrevDevicePath = NewDevicePath;
          NewDevicePath = AppendDevicePathInstance (PrevDevicePath, VarConsoleInstance);
          if  (PrevDevicePath != NULL)
            gBS->FreePool (PrevDevicePath);
        }
        else {
          VarChanged = TRUE;
        }
        gBS->FreePool (VarConsoleInstance);
      }
    }
  }
  else {
    NewDevicePath = DuplicateDevicePath (VarConsole);
  }

  while (CustomizedConDevicePath != NULL) {

    CustomizedInstance = GetNextDevicePathInstance (&CustomizedConDevicePath, &DevicePathSize);
    if (CustomizedInstance != NULL) {
      if (!IsInclusiveOfSelectDevPath (NewDevicePath, CustomizedInstance)) {
        VarChanged = TRUE;
        PrevDevicePath = NewDevicePath;
        NewDevicePath = AppendDevicePathInstance (PrevDevicePath, CustomizedInstance);
        if  (PrevDevicePath != NULL)
          gBS->FreePool (PrevDevicePath);
      }
  
      gBS->FreePool (CustomizedInstance);
    }

  }

  if (OldVarConsole != NULL) {
    gBS->FreePool (OldVarConsole);
  }

  //
  // Update the variable of the default console
  //
  if (VarChanged) {
    UINT32    Attributes;
    CHAR16    *StringPtr;

    StringPtr = ConVarName;

    //
    // If the variable includes "Dev" at last, we consider
    // it does not support NV attribute.
    //
    while (*StringPtr != L'\0') {
      StringPtr++;
    }

    if ( (((INTN)((UINTN)StringPtr - (UINTN)ConVarName) / sizeof (CHAR16)) > 3) &&
        (*(StringPtr - 3) == L'D') &&
        (*(StringPtr - 2) == L'e') &&
        (*(StringPtr - 1) == L'v')
       ) {
      Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    } else {
      Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
    }

    CommonSetVariable (
      ConVarName,
      ConVarGuid,
      Attributes,
      GetDevicePathSize (NewDevicePath),
      NewDevicePath
      );

    return EFI_SUCCESS;
  }


  return EFI_ABORTED;

}

/**
  Function compares a device path data structure to that of all the nodes of a
  second device path instance.

  @param  DevPathPool                 A pointer to a multi-instance device path data
                                structure.
  @param  Single                A pointer to a single-instance device path data
                                structure.

  @retval TRUE                  If the Single device path is contained within Multi device path.
  @retval FALSE                 The Single device path is not match within Multi device path.

**/
BOOLEAN
EFIAPI
IsInclusiveOfSelectDevPath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevPathPool,
  IN  EFI_DEVICE_PATH_PROTOCOL  *SelectDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInst;
  UINTN                     Size;

  if (DevPathPool == NULL || SelectDevPath  == NULL) {
    return FALSE;
  }

  DevicePath      = DevPathPool;
  DevicePathInst  = GetNextDevicePathInstance (&DevicePath, &Size);

  //
  // Search for the match of 'Single' in 'DevPathPool'
  //
  while (DevicePathInst != NULL) {
    //
    // If the single device path is found in multiple device paths,
    // return success
    //
    if (CompareMem (SelectDevPath, DevicePathInst, Size) == 0) {
      FreePool (DevicePathInst);
      return TRUE;
    }

    FreePool (DevicePathInst);
    DevicePathInst = GetNextDevicePathInstance (&DevicePath, &Size);
  }

  return FALSE;
}

//[-start-211209-IB18180097-add]//
/**

  Function for checking the uart device need to be controlled by console redirection

  @param  UartDevPath           A pointer to a single-instance device path data structure

**/
BOOLEAN
EFIAPI
IsCrDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL  *UartDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *CrDevPath;
  CR_DEVICES_SETTING_NODE   *CrDevNode;
  LIST_ENTRY                *Link;
  UINT8                     ItemType;
  BOOLEAN                   Found;


  if (!IsUartDevice (UartDevPath)) {
    return FALSE;
  }

  Found = FALSE;

  Link = GetFirstNode (&mCrVarDeviceHead.Link);
  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);
    CrDevPath = GetDevicePathFromCrDevSetting (CrDevNode->CrSetting);
    ItemType  = CrDevNode->CrSetting->ItemType;

    switch (ItemType) {
      
    case NORMAL_ITEM:
      if (IsUartDeviceSame (UartDevPath, CrDevPath)) {
        Found = TRUE;
      }
      break;

    case ASTERISK_ITEM:
      if (IsDpHeadMatch (UartDevPath, CrDevPath)) {
        Found = TRUE;
      }
      break;
      
    case INVALID_ITEM:
    default:
      break;
      
    }

    if (Found == TRUE) {
      break;
    }
    
    Link = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }

  return Found;
}

/**

  Function for checking this device path is for uart or not

  @param  DevPath               A pointer to a single-instance device path data structure

**/
BOOLEAN
EFIAPI
IsUartDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevPathNode;

  DevPathNode = DevPath;

  while (!IsDevicePathEnd (DevPathNode)) {
    
    if ((DevPathNode->Type == MESSAGING_DEVICE_PATH) &&
        (DevPathNode->SubType == MSG_UART_DP)) {
      return TRUE;
    }
    
    DevPathNode = NextDevicePathNode (DevPathNode);
  }

  return FALSE;
}

/**

  Function for checking BaseDevPath include SelectDevPath or not

  @param  SelectDevPath         A pointer to a single-instance device path data structure
  @param  BaseDevPath           A pointer to a single-instance device path data structure

**/
BOOLEAN
EFIAPI
IsDpHeadMatch (
  IN  EFI_DEVICE_PATH_PROTOCOL  *SelectDevPath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *BaseDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *BasePathNode;
  EFI_DEVICE_PATH_PROTOCOL  *SelectDpNode;
  UINTN                     Size;

  SelectDpNode   = SelectDevPath;
  BasePathNode   = BaseDevPath;

  while (!IsDevicePathEnd (BasePathNode) && !IsDevicePathEnd (SelectDpNode)) {
  
    Size = DevicePathNodeLength (SelectDpNode);
    
    if (CompareMem (SelectDpNode, BasePathNode, Size) != 0) {
      return FALSE;
    }
    
    BasePathNode   = NextDevicePathNode (BasePathNode);
    SelectDpNode   = NextDevicePathNode (SelectDpNode);
  }

  if (IsDevicePathEnd (BasePathNode)) {
    return  TRUE;
  }

  return FALSE;
}

BOOLEAN
IsUartDevNode (
  IN EFI_DEVICE_PATH      *DevNode
  )
{
  if (DevNode->Type == MESSAGING_DEVICE_PATH &&
      DevNode->SubType == MSG_UART_DP) {
    return TRUE;
  }

  return FALSE;
}

/**

  Function for checking two device paths direct to the same device or not

  @param  SelectDevPath         A pointer to a single-instance device path data structure
  @param  CmpToDevPath          A pointer to a single-instance device path data structure

**/
BOOLEAN
EFIAPI
IsUartDeviceSame (
  IN  EFI_DEVICE_PATH_PROTOCOL  *SelectDevPath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CmpToDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  EFI_DEVICE_PATH_PROTOCOL  *SelectDpNode;
  UINTN                     Size;

  DevicePathNode  = CmpToDevPath;
  SelectDpNode    = SelectDevPath;

  while (!IsDevicePathEnd (DevicePathNode) && !IsDevicePathEnd (SelectDpNode)) {
  
    Size = DevicePathNodeLength (SelectDpNode);

    if (IsUartDevNode(DevicePathNode) && IsUartDevNode(SelectDpNode)) {
      return TRUE;
    }
    
    if (CompareMem (SelectDpNode, DevicePathNode, Size) != 0) {
      return FALSE;
    }
    
    DevicePathNode = NextDevicePathNode (DevicePathNode);
    SelectDpNode   = NextDevicePathNode (SelectDpNode);
  }

  return FALSE;
}
//[-end-211209-IB18180097-add]//

/**
 Compare two device path instances

 @param [in] DevicePath1        The first device path to be compared
 @param [in] DevicePath2        The second device path to be compared

 @return TRUE                   The two device paths are the same
 @retval FALSE                  The two device paths are not the same

**/
BOOLEAN
IsEqualDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath1,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath2
  )
{
  UINTN     DevicePathSize1;
  UINTN     DevicePathSize2;
  UINTN     Value;

  DevicePathSize1 = GetDevicePathSize (DevicePath1);
  DevicePathSize2 = GetDevicePathSize (DevicePath2);

  if (DevicePathSize1 != DevicePathSize2)
    return FALSE;

  if (DevicePathSize1 == 0)
    return TRUE;

  Value = CompareMem (DevicePath1, DevicePath2, DevicePathSize1);

  if (Value == 0)
    return TRUE;

  return FALSE;
}

/**
 Verify DataBits value

 @param [in] DataBits        The value which user want to set to DataBits field

 @return UINT8               The valid DataBits value

**/
UINT8
VerifyDataBits (
  IN UINT8   DataBits
  )
{
  if ((DataBits == 7) || (DataBits == 8)) {
    return DataBits;
  } else {
    return 8;
  }
}

/**
 Verify StopBits value

 @param [in] StopBits        The value which user want to set to StopBits field

 @return UINT8               The valid StopBits value

**/
UINT8
VerifyStopBits (
  IN UINT8   StopBits
  )
{
  if ((StopBits == OneStopBit) || (StopBits == TwoStopBits)) {
    return StopBits;
  } else {
    return OneStopBit;
  }
}

/**
 Verify Parity value

 @param [in] Parity        The value which user want to set to Parity field

 @return UINT8             The valid Parity value

**/
UINT8
VerifyParity (
  IN UINT8   Parity
  )
{
  if ((Parity >= NoParity) && (Parity <= SpaceParity)) {
    return Parity;
  } else {
    return NoParity;
  }
}

VOID
UpdateCrDeviceSetting (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL              *DevPath;
  EFI_DEVICE_PATH_PROTOCOL              *TempPath;
  UART_DEVICE_PATH                      *Uart;
  CR_DEVICES_SETTING_NODE               *CrDevNode;
  EFI_STATUS                            Status;
  LIST_ENTRY                            *Link;
  BOOLEAN                               UpdateCrDev;
  UINT8                                 Index;
  H2O_SERIAL_ATTRIBS                    SerialAttribs;
  
  DEBUG ((DEBUG_INFO, "\nCr << %a >>\n", __FUNCTION__));

  DevPath     = NULL;
  TempPath    = NULL;
  Uart        = NULL;
  UpdateCrDev = FALSE;

  Link = GetFirstNode (&mCrVarDeviceHead.Link);

  while (!IsNull (&mCrVarDeviceHead.Link, Link)) {
    CrDevNode = CR_DEVICE_SETTING_FROM_THIS (Link);
    
    if ((CrDevNode->CrSetting->Exist == FALSE)   ||
        (CrDevNode->CrSetting->Attribute.PortEnable == FALSE)) {
      Link  = GetNextNode (&mCrVarDeviceHead.Link, Link);
      continue;
    }

    DevPath = GetDevicePathFromCrDevSetting (CrDevNode->CrSetting);
    DEBUG ((DEBUG_INFO, " %s\n", CrDevNode->CrSetting->DevName));
    DEBUG ((DEBUG_INFO, " Old Setting\n"));
    DEBUG ((DEBUG_INFO, "   Exist:%d, ItemType:%d, PortEnable:%d, Global:%d, TerminalType:%d, BaudRate:%d, Parity:%d, DataBits:%d, StopBits:%d, FlowControl:%d\n",
               CrDevNode->CrSetting->Exist,
               CrDevNode->CrSetting->ItemType,
               CrDevNode->CrSetting->Attribute.PortEnable,
               CrDevNode->CrSetting->Attribute.UseGlobalSetting,
               CrDevNode->CrSetting->Attribute.TerminalType,
               CrDevNode->CrSetting->Attribute.BaudRate,
               CrDevNode->CrSetting->Attribute.Parity,
               CrDevNode->CrSetting->Attribute.DataBits,
               CrDevNode->CrSetting->Attribute.StopBits,
               CrDevNode->CrSetting->Attribute.FlowControl
               ));
    DEBUG ((DEBUG_INFO, "   DevicePath: %s\n", ConvertDevicePathToText ((const EFI_DEVICE_PATH *)DevPath, FALSE, TRUE)));
    
    SerialAttribs.Size                = sizeof (H2O_SERIAL_ATTRIBS);
    SerialAttribs.BaudRate            = mCrBaudRateTable[CrDevNode->CrSetting->Attribute.BaudRate];
    SerialAttribs.Parity              = CrDevNode->CrSetting->Attribute.Parity;
    SerialAttribs.StopBits            = CrDevNode->CrSetting->Attribute.StopBits;
    SerialAttribs.DataBits            = CrDevNode->CrSetting->Attribute.DataBits;
    SerialAttribs.Present             = CrDevNode->CrSetting->Attribute.PortEnable;
    SerialAttribs.Timeout             = 0;
    SerialAttribs.ReceiveFifoDepth    = 0;
    SerialAttribs.TransmitFifoDepth   = 0;
    SerialAttribs.DevicePath          = DevPath;
    SerialAttribs.MaxDevicePathSize   = 60;
    
    Status = H2OGetSerialAttribsConfig (&SerialAttribs);
    if (Status != EFI_MEDIA_CHANGED && Status != EFI_UNSUPPORTED) {
      CrDevNode->CrSetting->Exist                = FALSE;
      CrDevNode->CrSetting->Attribute.PortEnable = FALSE;
    } else {
      for (Index = 0; mCrBaudRateTable[Index] != 0; Index++) {
        if (SerialAttribs.BaudRate == mCrBaudRateTable[Index]) {
          break;
        }
      }
      CrDevNode->CrSetting->Attribute.BaudRate         = (mCrBaudRateTable[Index] == 0) ? --Index : Index;
      CrDevNode->CrSetting->Attribute.Parity           = VerifyParity (SerialAttribs.Parity);
      CrDevNode->CrSetting->Attribute.DataBits         = VerifyDataBits (SerialAttribs.DataBits);
      CrDevNode->CrSetting->Attribute.StopBits         = VerifyStopBits (SerialAttribs.StopBits);
      CrDevNode->CrSetting->Attribute.PortEnable       = (SerialAttribs.Present == 0) ? 0 : 1;
      if (Status == EFI_MEDIA_CHANGED) {
        CrDevNode->CrSetting->Attribute.UseGlobalSetting = 0;
      }
      if (CrDevNode->CrSetting->Attribute.PortEnable == FALSE) {
        CrDevNode->CrSetting->Exist = FALSE;
      }

      DEBUG ((DEBUG_INFO, " New CR Setting\n"));
      DEBUG ((DEBUG_INFO, "   Exist:%d, ItemType:%d, PortEnable:%d, Global:%d, TerminalType:%d, BaudRate:%d, Parity:%d, DataBits:%d, StopBits:%d, FlowControl:%d\n",
                 CrDevNode->CrSetting->Exist,
                 CrDevNode->CrSetting->ItemType,
                 CrDevNode->CrSetting->Attribute.PortEnable,
                 CrDevNode->CrSetting->Attribute.UseGlobalSetting,
                 CrDevNode->CrSetting->Attribute.TerminalType,
                 CrDevNode->CrSetting->Attribute.BaudRate,
                 CrDevNode->CrSetting->Attribute.Parity,
                 CrDevNode->CrSetting->Attribute.DataBits,
                 CrDevNode->CrSetting->Attribute.StopBits,
                 CrDevNode->CrSetting->Attribute.FlowControl
                 ));

      UpdateCrDev = TRUE;
    }

    if (UpdateCrDev) {
      UpdateOrDeleteCrDevVarWithDevInfo (CrDevNode->VarIndex ,CrDevNode->CrSetting);

      //
      // Sync CR device variable to CrConfig variable
      //
      UpdateDeviceSettingToCrConfig ();

      //
      // Update UART node information
      //
      TempPath = DevPath;
      while (!IsDevicePathEnd(TempPath)) {
        if (TempPath->Type == MESSAGING_DEVICE_PATH && TempPath->SubType == MSG_UART_DP) {
          Uart = (UART_DEVICE_PATH *)TempPath;
          Uart->BaudRate = mCrBaudRateTable[CrDevNode->CrSetting->Attribute.BaudRate];
          Uart->DataBits = CrDevNode->CrSetting->Attribute.DataBits;
          Uart->StopBits = CrDevNode->CrSetting->Attribute.StopBits;
          Uart->Parity   = CrDevNode->CrSetting->Attribute.Parity;
          break;
        }
        TempPath = NextDevicePathNode(TempPath);
      }
      DEBUG ((DEBUG_INFO, "  New DevicePath: %s\n", ConvertDevicePathToText (GetDevicePathFromCrDevSetting (CrDevNode->CrSetting), FALSE, TRUE)));
      
      UpdateCrDev = FALSE;
    }
    
    Link                     = GetNextNode (&mCrVarDeviceHead.Link, Link);
  }
  
  return;
}

