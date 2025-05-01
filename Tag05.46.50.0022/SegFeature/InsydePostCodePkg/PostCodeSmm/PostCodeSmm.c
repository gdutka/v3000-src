/** @file
  PostCodeSmm driver implementation

;******************************************************************************
;* Copyright (c) 2016 - 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PostCodeSmm.h>

#define SafeFreePool(a) \
            if (a != NULL) { \
              FreePool (a);  \
            }

//
// To indicate if the debug registers need to be initialized
// when the processor enters SMM
//
BOOLEAN                      mEnableFeature;

//
// The interface which is provided for SmmCorePlatformHookLib to 
// set up the debug registers
//
SMM_DEBUG_REGISTER_CONFIG_PROTOCOL mSmmDebugRegisterConfig = {
  SmmSetUpDebugRegister
};

//
// error handling var
//
H2O_CP_HANDLE                *mEnableFeatureCpHandlerArr;
H2O_CP_HANDLE                *mDisableFeatureCpHandlerArr;
UINTN                        mEnableFeatureCpHandlerCount;
UINTN                        mDisableFeatureCpHandlerCount;
UINTN                        mEnableFeatureCpHandlerMaxNum;
UINTN                        mDisableFeatureCpHandlerMaxNum;

VOID                         **mEnableFeatureRegisterationArr;
VOID                         **mDisableFeatureRegisterationArr;
UINTN                        mEnableFeatureRegisterationCount;
UINTN                        mDisableFeatureRegisterationCount;
UINTN                        mEnableFeatureRegisterationMaxNum;
UINTN                        mDisableFeatureRegisterationMaxNum;

/**
  This function initializes the debug registers to trap port 80 
  when mEnableFeature is true
*/
VOID
EFIAPI 
SmmSetUpDebugRegister (
  VOID
  )
{
  UINT16 DebugPort;
  
  //
  // When driver disables PostCode feature, the debug registers will not be configured
  //
  if (!mEnableFeature) {
    return;
  }

  //
  // Configure the debug registers to trap debug port
  // This driver uses breakpoint 0 to trap debug port
  //
  DebugPort = PcdGet16 (PcdH2OPostCodeDebugPortAddress);
  SetUpDebugRegister (Breakpoint0, IoReadOrWrite, OneByte, DebugPort);
}


/**
  Show postcode by vga frame buffer
  
  Use Graphic Library to print PostCode first
  GraphicVgaShowString can detect current backgrond color, and use it.
    
  @param [in]  PostCode  The postcode to show on the screen
 **/
VOID
EFIAPI
VgaFbPostCodeShow (
  UINT64  PostCode
)
{
  UINTN       CharCount;
  CHAR8       String[BUFFER_SIZE];

  CharCount = GetPostCodeString (ScreenDisplayStr, PostCode, BUFFER_SIZE, String);
  GraphicVgaShowString (
    (UINT16) (GRAPHIC_VGA_MAX_COL - CharCount - 2), 
    (UINT16) (GRAPHIC_VGA_MAX_ROW - 2), 
    GRAPHIC_VGA_WHITE, 
    GRAPHIC_VGA_BLACK, 
    String
    );
}

/**
 The PostCode feature handler which will be called by PostCodeFeatureHandlerEntry ()
 
 When PostCode is written to port 80, the processor will enter debug exception
 And the PostCodeFeatureHandlerEntry () will be called

 @param [in]  InterruptType  Exception type
 @param [in]  SystemContext  System context data

**/
VOID
EFIAPI 
PostCodeFeatureHandler (
  IN CONST  EFI_EXCEPTION_TYPE InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT SystemContext
  )
{
  UINT64      PostCode;
  UINT8       PostCodeLength;

  //
  // Check if this trap was triggered by reading the debug port
  //
  if (IsTrappedByReadPort(SystemContext.SystemContextX64->Rip)) {
    //
    // When exits exception, cleans the trap status
    //
    ClearTrapStatus (Breakpoint0);
    return;
  }

  //
  // Get the length of the Post Code which was outputted to debug port
  //
  PostCodeLength = GetPostCodeLength (SystemContext.SystemContextX64->Rip);  
   
  //
  // Get the Post Code from RAX register 
  // Post Code is outputted by "out" instruction and the outputted value will be put to the RAX register
  // It may not be that all bytes of the RAX register are Post Code, so we need to filter the RAX register
  //
  PostCode = SystemContext.SystemContextX64->Rax & ~(POST_CODE_MASK << (PostCodeLength * 8));

  //
  // Send Post Code to serial port
  //
  if (FeaturePcdGet (PcdH2OPostCodeSerialPortSupported)) {
    SmmSerialPortPostCode (PostCode);
  }

  //
  // Print Post Code on screen
  //
  if (FeaturePcdGet (PcdH2OPostCodeScreenSupported)) {
    VgaFbPostCodeShow (PostCode);
  }

  //
  // When exits exception, cleans the trap status
  //
  ClearTrapStatus (Breakpoint0);
}

/**
  To enable Dxe PostCode features
*/
VOID
EFIAPI
EnableFeature (
  VOID
  )
{
  UINT16 DebugPort;
  
  //
  // Register PostCodeFeatureHandlerEntry () to the debug exception entry of IDT
  // for performing PostCode Feature
  //
  RegisterIdtHandler (EXCEPT_X64_DEBUG, (UINTN) PostCodeFeatureHandlerEntry);

  //
  // Configure the debug registers to trap debug port
  // This driver uses breakpoint0 to trap debug port
  //
  DebugPort = PcdGet16 (PcdH2OPostCodeDebugPortAddress);
  SetUpDebugRegister (Breakpoint0, IoReadOrWrite, OneByte, DebugPort);
}

/**
  Callback function for enabling SMM PostCode features
  
  @param [in] Protocol    Points to the protocol's unique identifier
  @param [in] Interface   Points to the interface instance
  @param [in] Handle      The handle on which the interface was installed

  @retval EFI_SUCCESS     EnableFeatureCallback runs successfully
 **/
EFI_STATUS
EFIAPI
EnableFeatureCallback (
  IN CONST EFI_GUID   *Protocol,
  IN       VOID       *Interface,
  IN       EFI_HANDLE Handle
  )
{
  EnableFeature ();

  //
  // Set mEnableFeature to indicate the debug registers need to be initialized
  // when the processor enters SMM
  //
  mEnableFeature = TRUE;
  
  return EFI_SUCCESS;
}

/**
  Callback function for enabling SMM PostCode features
  
   @param[in] Event   The Event this notify function registered to.
   @param[in] Handle  the handle associated with a previously registered checkpoint handler.

  @retval EFI_SUCCESS     CpEnableFeatureCallback runs successfully
 **/
EFI_STATUS
EFIAPI
CpEnableFeatureCallback (
  IN EFI_EVENT      Event,
  IN H2O_CP_HANDLE  CpHandle
  )
{
  EnableFeature ();

  //
  // Set mEnableFeature to indicate the debug registers need to be initialized
  // when the processor enters SMM
  //
  mEnableFeature = TRUE;
  
  return EFI_SUCCESS;
}


/**
  Callback function for disabling SMM PostCode features
  
  @param [in] Protocol    Points to the protocol's unique identifier
  @param [in] Interface   Points to the interface instance
  @param [in] Handle      The handle on which the interface was installed

  @retval EFI_SUCCESS     DisableFeatureCallback runs successfully
 **/
EFI_STATUS
EFIAPI
DisableFeatureCallback (
  IN CONST EFI_GUID   *Protocol,
  IN       VOID       *Interface,
  IN       EFI_HANDLE Handle
  )
{
  
  //
  // Clean the debug registers to disable breakpoint 0
  //
  ClearDebugRegister (Breakpoint0);

  //
  // If mEnableFeature is false, the debug registers will not be initialized 
  // when the processor enters SMM
  //
  mEnableFeature = FALSE; 
  
  return EFI_SUCCESS;
}
/**
  Callback function for disabling SMM PostCode features
  
  @param[in] Event   The Event this notify function registered to.
  @param[in] Handle  the handle associated with a previously registered checkpoint handler.

  @retval EFI_SUCCESS  DisableFeatureCallback runs successfully
 **/
EFI_STATUS
EFIAPI
CpDisableFeatureCallback (
  IN EFI_EVENT                          Event,
  IN H2O_CP_HANDLE                      Handle
  )
{
  //
  // Clean the debug registers to disable breakpoint 0
  //
  ClearDebugRegister (Breakpoint0);

  //
  // If mEnableFeature is false, the debug registers will not be initialized 
  // when the processor enters SMM
  //
  mEnableFeature = FALSE; 
  
  return EFI_SUCCESS;
}

/**
  Unregister all protocol notify registed for EnableFeature callback function by closing the event.
**/
VOID
EFIAPI
UnRegisterProtocolEnableFeature (
  VOID
)
{
  EFI_GUID    *NotifyGuid;
  EFI_GUID    ZeroGuid;
  UINTN       Idx;
  
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));
  Idx = 0;
 
  NotifyGuid = PcdGetPtr (PcdH2OPostCodeTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    gSmst->SmmRegisterProtocolNotify (
      NotifyGuid,
      NULL,   // unregister
      &mEnableFeatureRegisterationArr[Idx++]
      );
      
    if (Idx >= mEnableFeatureRegisterationCount) {
      break;
    }

    NotifyGuid++;
  }

  return;
}

/**
  Unregister all protocol notify registed for DisableFeature callback function by closing event.
**/
VOID
EFIAPI
UnRegisterProtocolDisableFeature (
  VOID
)
{
  EFI_GUID    *NotifyGuid;
  EFI_GUID    ZeroGuid;
  UINTN       Idx;
  
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));
  Idx = 0;
 
  NotifyGuid = PcdGetPtr (PcdH2OPostCodeTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    gSmst->SmmRegisterProtocolNotify (
      NotifyGuid,
      NULL,  // unregister
      &mDisableFeatureRegisterationArr[Idx++]
      );
      
    if (Idx >= mDisableFeatureRegisterationCount) {
      break;
    }

    NotifyGuid++;
  }

  return;
}


/**
  Register Enable Features "Protocol" notify function to enable smm PostCode features
  
 @retval EFI_SUCCESS   Trap debug port notify registeration succeeded.
 @retval others        Trap debug port notify registeration failed.

**/
EFI_STATUS
EFIAPI
RegisterProtocolForEnableFeature (
  VOID
)
{
  EFI_STATUS  Status;
  EFI_GUID    *NotifyGuid;
  EFI_GUID    ZeroGuid;
  
  Status = EFI_SUCCESS;
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = PcdGetPtr (PcdH2OPostCodeTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = gSmst->SmmRegisterProtocolNotify (
                      NotifyGuid,
                      EnableFeatureCallback,
                      &mEnableFeatureRegisterationArr[mEnableFeatureRegisterationCount++]
                      );
    if(EFI_ERROR(Status)) {
      UnRegisterProtocolEnableFeature ();
      return Status;
    }    
    NotifyGuid++;
  }

  return Status;
}

/**
  Register Disable Features "Protocol" notify function to disable smm PostCode features
    
  @retval EFI_SUCCESS   Trap debug port clean notify registeration succeeded.
  @retval others        Trap debug port clean notify registeration failed.

**/
EFI_STATUS
EFIAPI
RegisterProtocolForDisableFeature (
  VOID
)
{
  EFI_STATUS  Status;
  EFI_GUID    *NotifyGuid;
  EFI_GUID    ZeroGuid;
  
  Status = EFI_SUCCESS;
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = gSmst->SmmRegisterProtocolNotify (
                      NotifyGuid,
                      DisableFeatureCallback,
                      &mDisableFeatureRegisterationArr[mDisableFeatureRegisterationCount++]
                      );
    if(EFI_ERROR(Status)) {
      UnRegisterProtocolDisableFeature ();
      return Status;
    }    
    NotifyGuid++;
  }

  return Status;
}


/**
  Unregister all checkpoint registed for EnableFeature callback function by unregistering cphandle
**/
VOID
EFIAPI
UnRegisterCpEnableFeature (
  VOID
)
{
  UINTN Idx;

  for (Idx = 0; Idx < mEnableFeatureCpHandlerCount; Idx++) {
    H2OCpUnregisterHandler (mEnableFeatureCpHandlerArr[Idx]);
  }
}

/**
  Unregister all checkpoint registed for EnableFeature callback function by unregistering cphandle
**/
VOID
EFIAPI
UnRegisterCpDisableFeature (
  VOID
)
{
  UINTN Idx;

  for (Idx = 0; Idx < mEnableFeatureCpHandlerCount; Idx++) {
    H2OCpUnregisterHandler (mDisableFeatureCpHandlerArr[Idx]);
  }
}

/**
  Register Enable Features "CheckPoint" notify function to enable smm PostCode features
    
 @retval EFI_SUCCESS   Trap debug port notify registeration succeeded.
 @retval others        Trap debug port notify registeration failed.

**/
EFI_STATUS
EFIAPI
RegisterCpForEnableFeature (
  VOID
)
{
  EFI_STATUS     Status;
  EFI_GUID       *NotifyGuid;
  EFI_GUID       ZeroGuid;
  
  Status = EFI_SUCCESS;
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));
 
  NotifyGuid = (EFI_GUID*) PcdGetPtr (PcdH2OPostCodeTrapDebugPortCpNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status  = H2OCpRegisterHandler (
                NotifyGuid,
                CpEnableFeatureCallback,
                H2O_CP_MEDIUM,
                &mEnableFeatureCpHandlerArr[mEnableFeatureCpHandlerCount++]
                );
    if(EFI_ERROR(Status)) {
      UnRegisterCpEnableFeature ();
      return Status;
    }    
    NotifyGuid++;
  }

  return Status;
}

/**
  Register Disable Features "CheckPoint" notify function to disable smm PostCode features
    
  @retval EFI_SUCCESS   Trap debug port notify registeration succeeded.
  @retval others        Trap debug port notify registeration failed.

**/
EFI_STATUS
EFIAPI
RegisterCpForDisableFeature (
  VOID
)
{
  EFI_STATUS     Status;
  EFI_GUID       *NotifyGuid;
  EFI_GUID       ZeroGuid;
  
  Status = EFI_SUCCESS;
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = (EFI_GUID*) PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortCpNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status  = H2OCpRegisterHandler (
                NotifyGuid,
                CpDisableFeatureCallback,
                H2O_CP_MEDIUM,
                &mDisableFeatureCpHandlerArr[mDisableFeatureCpHandlerCount++]
                );
    if(EFI_ERROR(Status)) {
      UnRegisterCpDisableFeature ();
      return Status;
    }    
    NotifyGuid++;
  }

  return Status;
}


/**
 Count how many check point will be registered 

 @retval others
**/
UINTN
EFIAPI
CountNotifyCp (
  EFI_GUID *NotifyGuidList
)
{
  UINTN     Count;
  EFI_GUID  ZeroGuid;
  EFI_GUID  *NotifyGuid;

  Count = 0;

  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = NotifyGuidList;
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {    
    Count++;
    NotifyGuid++;
  }
  
  return Count;
}

/**
  Release pool allocated by PostCodeSmm driver

**/
VOID
ReleaseResources (
  VOID
  )
{
  SafeFreePool (mEnableFeatureCpHandlerArr);
  SafeFreePool (mDisableFeatureCpHandlerArr);
  SafeFreePool (mEnableFeatureRegisterationArr);
  SafeFreePool (mDisableFeatureRegisterationArr);
}

/**
 Initialize global variables.
**/
EFI_STATUS
EFIAPI
InitGlobalVar (
  VOID
)
{   
  //
  // Set mEnableFeature to indicate the debug registers need to be initialized
  // when the processor enters SMM
  //
  mEnableFeature                    = TRUE;
  mEnableFeatureCpHandlerArr        = NULL;
  mDisableFeatureCpHandlerArr       = NULL;
  mEnableFeatureCpHandlerCount      = 0;
  mDisableFeatureCpHandlerCount     = 0;
  mEnableFeatureRegisterationCount  = 0;
  mDisableFeatureRegisterationCount = 0;
  
  mEnableFeatureCpHandlerMaxNum = CountNotifyCp ((EFI_GUID*) PcdGetPtr (PcdH2OPostCodeTrapDebugPortCpNotifyList));
  mDisableFeatureCpHandlerMaxNum = CountNotifyCp ((EFI_GUID*) PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortCpNotifyList));
  mEnableFeatureRegisterationMaxNum = CountNotifyCp ((EFI_GUID*) PcdGetPtr (PcdH2OPostCodeTrapDebugPortNotifyList));
  mDisableFeatureRegisterationMaxNum = CountNotifyCp ((EFI_GUID*) PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortNotifyList));

  mEnableFeatureCpHandlerArr = (H2O_CP_HANDLE*) AllocateZeroPool (sizeof (H2O_CP_HANDLE) * mEnableFeatureCpHandlerMaxNum);
  if (mEnableFeatureCpHandlerArr == NULL && mEnableFeatureCpHandlerMaxNum != 0) {
    return EFI_OUT_OF_RESOURCES;
  }

  mDisableFeatureCpHandlerArr = (H2O_CP_HANDLE*) AllocateZeroPool (sizeof (H2O_CP_HANDLE) * mDisableFeatureCpHandlerMaxNum);
  if (mDisableFeatureCpHandlerArr == NULL && mDisableFeatureCpHandlerMaxNum != 0) { 
    ReleaseResources ();
    return EFI_OUT_OF_RESOURCES;
  }

  mEnableFeatureRegisterationArr = (VOID**) AllocateZeroPool (sizeof (H2O_CP_HANDLE) * mEnableFeatureRegisterationMaxNum);
  if (mEnableFeatureRegisterationArr == NULL && mEnableFeatureRegisterationMaxNum != 0) {
    ReleaseResources ();
    return EFI_OUT_OF_RESOURCES;
  }

    mDisableFeatureRegisterationArr = (VOID**) AllocateZeroPool (sizeof (H2O_CP_HANDLE) * mDisableFeatureRegisterationMaxNum);
  if (mDisableFeatureRegisterationArr == NULL && mDisableFeatureRegisterationMaxNum != 0) {
    ReleaseResources ();
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  To see if PostCodeSmmDxe is enabled.

  @retval TRUE   PostCodeDxe is enabled.
  @retval FALSE  PostCodeDxe is disabled.

**/
BOOLEAN
IsFeatureEnable (
  VOID
  )
{
  EFI_STATUS  Status;
  CHAR16      *VarName;
  EFI_GUID    VarGuid;  
  BOOLEAN     PostCodeDxeEnalbe;  
  BOOLEAN     *PostCodeDxeEnalbePtr;
  
  Status               = EFI_SUCCESS;
  PostCodeDxeEnalbe    = FALSE;
  PostCodeDxeEnalbePtr = &PostCodeDxeEnalbe;
  //
  // Check variable
  //
  VarName = (CHAR16*) PcdGetPtr (PcdH2OPostCodeSmmDxeEnableVarName);
  VarGuid = gH2OPostCodeSmmDxeEnableVarGuid;
  Status = GetVariable2 (VarName, &VarGuid, &PostCodeDxeEnalbePtr, NULL);
  if (!EFI_ERROR(Status)) {
    return PostCodeDxeEnalbe;
  }

  //
  // Check PCD
  //
  PostCodeDxeEnalbe = PcdGetBool (PcdH2OPostCodeSmmDxeEnable);
  
  return PostCodeDxeEnalbe;
}

/**
 Initialize the debug exception entry of IDT and debuger registers to trap port 80
 Install gH2OPostCodeSmmDebugRegProtocolGuid protocol
 Register notify function to disable to trap port 80

 @param [in]           ImageHandle
 @param [in]           SystemTable

 @retval EFI_SUCCESS   Initializes successfully
 @return Other         Registration failed

**/
EFI_STATUS
EFIAPI
PostCodeSmmEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS           Status;
  EFI_HANDLE           Handle;
  BOOLEAN              RegisterProtocolEnable;
  BOOLEAN              RegisterProtocolDisable;
  BOOLEAN              RegisterCpEnable;
  BOOLEAN              RegisterCpDisable;
  
  Status                   = EFI_SUCCESS;
  RegisterProtocolEnable   = FALSE;
  RegisterProtocolDisable  = FALSE;
  RegisterCpEnable         = FALSE;
  RegisterCpDisable        = FALSE;
  

  if (!IsFeatureEnable ()) {
    return EFI_UNSUPPORTED;
  }

  Status = InitGlobalVar ();
  if (EFI_ERROR(Status)) {
    ReleaseResources ();
    return Status;
  }

  EnableFeature ();


  //
  // Install protocol to provide SmmCorePlatformHookLib to initialize the debug registers
  //
  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gH2OPostCodeSmmDebugRegProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mSmmDebugRegisterConfig
                    );
  if( EFI_ERROR(Status)) {
    goto Error;
  }
  
  Status = RegisterProtocolForEnableFeature ();
  if( EFI_ERROR(Status)) {
    goto Error;
  }
  RegisterProtocolEnable = TRUE;

  Status = RegisterCpForEnableFeature ();
  if( EFI_ERROR(Status)) {
    goto Error;
  }
  RegisterCpEnable = TRUE;
  
  Status = RegisterProtocolForDisableFeature ();
  if( EFI_ERROR(Status)) {
    goto Error;
  }
  RegisterProtocolDisable = TRUE;
  
  Status = RegisterCpForDisableFeature ();
  if( EFI_ERROR(Status)) {
    goto Error;
  }
  RegisterCpDisable = TRUE;

  return EFI_SUCCESS;

    
Error:
  if (RegisterProtocolEnable) {
    UnRegisterProtocolEnableFeature ();
  }

  if (RegisterCpEnable) {
    UnRegisterCpEnableFeature ();
  }

  if (RegisterProtocolDisable) {
    UnRegisterProtocolDisableFeature ();
  }

  if (RegisterCpDisable) {
    UnRegisterCpDisableFeature ();
  }
  
  ClearDebugRegister (Breakpoint0);

  ReleaseResources ();

  return Status;
}

