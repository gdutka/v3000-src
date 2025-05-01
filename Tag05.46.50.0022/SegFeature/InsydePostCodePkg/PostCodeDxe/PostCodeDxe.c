/** @file
  PostCodeDxe driver implementation

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

#include "PostCodeDxe.h"

#define MAX_SERIAL_TERMINAL     10

#define SafeFreePool(a) \
            if (a != NULL) { \
              FreePool (a);  \
            }
// 
// mSerialPostCodeMode indicates Console Redirection show mode
// serial mode  = 0   
// console mode  = 1  (default)
//
UINT8                            mSerialPostCodeMode;
BOOLEAN                          mCrEnable;

BOOLEAN                          mIsVgaFbClean;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *mVgaConOut;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *mSerialConOut[MAX_SERIAL_TERMINAL];
UINTN                            mSerialConOutCount;
BOOLEAN                          mConsoleOutDevDiscovered;
VOID                             *mSerialConOutRegisteration;

BOOLEAN                          mIsPcdPerformanceCounterReadyTickSet;

//
// error handling var
//
EFI_EVENT                         mProtoNotifyEnabkeFeatureEvent;
EFI_EVENT                         mProtoNotifyDisableFeatureEvent;
H2O_CP_HANDLE                     *mEnableFeatureCpHandlerArr;
H2O_CP_HANDLE                     *mDisableFeatureCpHandlerArr;
UINTN                             mEnableFeatureCpHandlerCount;
UINTN                             mDisableFeatureCpHandlerCount;
UINTN                             mEnableFeatureCpHandlerMaxNum;
UINTN                             mDisableFeatureCpHandlerMaxNum;
EFI_EVENT                         mReadyToBootProcessEvent;
EFI_EVENT                         mReadyToBootDisableEvent;
EFI_EVENT                         mConOutEvent;



/**
  get base timestamp tick from gPerformanceCounterReadyHobGuid HOB set by BaseTimeStampLib.c

  @return UINT64 base timestamp got from gPerformanceCounterReadyHobGuid HOB
*/
UINT64
EFIAPI
GetPerformanceCounterReadyTick (
  VOID
)
{
  EFI_HOB_GUID_TYPE *GuidHob;

  GuidHob = GetFirstGuidHob (&gPerformanceCounterReadyHobGuid);

  return *((UINT64 *) GET_GUID_HOB_DATA (GuidHob));
}

/**
  set base timestamp tick to PcdPerformanceCounterReadyTick which will later be used by BaseTimeStampLib.c

  HobList is not available for smm driver after gEfiDxeSmmReadyToLockProtocolGuid is installed,
  so get base timestamp tick in dxe and set it in pcd 
  so that base timestamp tick will be accessible to PostCodeSmm driver 

  @return EEFI_SUCCESS succeeded in setting PcdPerformanceCounterReadyTick
  @return others       failed to set PcdPerformanceCounterReadyTick
*/
EFI_STATUS
EFIAPI
SetPcdPerformanceCounterReadyTick (
  VOID
)
{
  EFI_STATUS Status;
  UINT64     Tick;

  Tick = GetPerformanceCounterReadyTick ();
  Status = PcdSet64S (PcdPerformanceCounterReadyTick, Tick);
  if (!EFI_ERROR (Status)) {
    mIsPcdPerformanceCounterReadyTickSet = TRUE;
  }

  return Status;
}

/**
  Show postcode by vga console out

  @param [in]  PostCode  The postcode to show on the screen
*/
VOID 
VgaConOutPostCode (
  IN UINTN     PostCode
  )
{
  ConsoleShowPostCode (mVgaConOut, PostCode);
}

/**
  Clean Vga console out
*/
VOID 
VgaConOutClean (
  VOID
)
{
  ConsoleCleanPostCode (mVgaConOut);
}

/**
  Show postcode by serial console out
*/
VOID 
SerialConOutPostCode (
  IN UINTN     PostCode
  )
{
  UINTN  Index;

  for (Index = 0; Index < mSerialConOutCount; Index++) {
    if (mSerialConOut[Index] == NULL) {
      continue;
    }
    
    ConsoleShowPostCode (mSerialConOut[Index], PostCode);
  }
}

/**
  Clean serial console out
*/
VOID 
SerialConOutClean (
  VOID
  )
{
  UINTN                             Index;

  for (Index = 0; Index < mSerialConOutCount; Index++) {
    if (mSerialConOut[Index] == NULL) {
      continue;
    }
    
     ConsoleCleanPostCode (mSerialConOut[Index]);
  }
}

/**
  Show postcode by vga frame buffer

  @param [in]  PostCode  The postcode to show on the screen
*/
VOID 
EFIAPI
VgaFbPostCodeShow (
  IN UINTN     PostCode
  )
{
  UINTN       CharCount;
  CHAR8       String[BUFFER_SIZE];

  //
  // Use Graphic Library to print PostCode first
  // GraphicVgaShowString can detect current backgrond color, and use it.
  //
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
  clean postcode in vga frame buffer
*/
VOID
EFIAPI
VgaFbPostCodeClean (
  VOID
  )
{
  CHAR8       String[BUFFER_SIZE];
  UINTN       CharCount;

  CharCount = GetPostCodeString (ScreenClearStr, 0, BUFFER_SIZE, String);
  
  GraphicVgaShowString (
   (UINT16) (GRAPHIC_VGA_MAX_COL - CharCount - 2), 
   (UINT16) (GRAPHIC_VGA_MAX_ROW - 2), 
   GRAPHIC_VGA_WHITE, 
   GRAPHIC_VGA_BLACK, 
   String
   );
}

/**
  Check if the device path is for VGA device.

  @param[in]  DevicePath        Pointer to the Device Path protocol of the option ROM.

  @retval     TRUE              The input device path is for VGA device.
              FALSE             The input device path is not for VGA device.
**/
BOOLEAN
EFIAPI
IsVgaDevice (
  IN CONST  EFI_DEVICE_PATH_PROTOCOL     *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL                                *DevPath;
  EFI_DEVICE_PATH_PROTOCOL                                *DevPathPtr;
  EFI_STATUS                                              Status = EFI_NOT_FOUND;
  EFI_HANDLE                                              Handle;
  EFI_PCI_IO_PROTOCOL                                     *PciIo;
  UINT16                                                  ClassCode;
  BOOLEAN                                                 IsVgaDevice;

  DevPath = DuplicateDevicePath (DevicePath);
  DevPathPtr = DevPath;

  PciIo = NULL;
  Status = gBS->LocateDevicePath (&gEfiPciIoProtocolGuid, &DevPath, &Handle);
  if (!EFI_ERROR (Status)) {
    Status = gBS->HandleProtocol (Handle, &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
  }

  gBS->FreePool(DevPathPtr);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  IsVgaDevice = FALSE;
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, 0xA, 1, &ClassCode);
  if (!EFI_ERROR (Status)) {
    if ((ClassCode == 0x0001) || (ClassCode == 0x0300)) {
      IsVgaDevice = TRUE;
    }
  }

  return IsVgaDevice;
}

/**
  Check if the device path is terminal type or not.

  @param Instance  Pointer to the checked device path.

  @retval TRUE     It is a terminal type.
  @retval FALSE    It is not a terminal type.

**/
BOOLEAN
EFIAPI
IsTerminalDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL    *Instance
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *Node;
  BOOLEAN                     IsTerminal;
  VENDOR_DEVICE_PATH          *Vendor;
  UART_DEVICE_PATH            *Uart;

  IsTerminal = FALSE;
  Vendor = NULL;
  Uart = NULL;

  for (Node = Instance; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    //
    // Vendor points to the node before the End node
    //
    Vendor = (VENDOR_DEVICE_PATH *)Node;

    if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH) &&
        (DevicePathSubType (Node) == MSG_UART_DP)) {
      Uart = (UART_DEVICE_PATH *)Node;
    }
  }

  if (Vendor != NULL &&
      DevicePathType (Vendor) == MESSAGING_DEVICE_PATH &&
      DevicePathSubType (Vendor) == MSG_VENDOR_DP &&
      Uart != NULL) {
    IsTerminal = TRUE;
  }

  return IsTerminal;
}

/**
  Locate SimpleTextOut(STO) when gEfiConsoleOutDeviceGuid is installed
  If it locates successfully, this driver can start to use STO to show PostCode on screen

  @param [in]  Event     Event whose notification function is being invoked
  @param [in]  Context   Pointer to the notification function's context
  
**/
VOID
EFIAPI
ConsoleOutCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                       Status;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  EFI_HANDLE                       Handle;
  EFI_HANDLE                       *Buffer;
  UINTN                            BufferSize;

  Status = EFI_SUCCESS;

  while (TRUE) {

    BufferSize = sizeof (Handle);
    Buffer     = &Handle;
    
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mSerialConOutRegisteration,
                    &BufferSize,
                    Buffer
                    );
    if (EFI_ERROR(Status)) {
      return;
    }

    Status = gBS->HandleProtocol (Handle, &gEfiDevicePathProtocolGuid, &DevicePath);
    if (EFI_ERROR(Status)) {
      continue;
    }
    
    Status = gBS->HandleProtocol (Handle, &gEfiSimpleTextOutProtocolGuid, &ConOut);
    if (EFI_ERROR(Status)) {
      continue;
    }

    if (IsVgaDevice(DevicePath)){
      mVgaConOut = ConOut;
      mConsoleOutDevDiscovered = TRUE;

    } else if (IsTerminalDevice (DevicePath)) {
      if (mSerialConOutCount < MAX_SERIAL_TERMINAL) {
        
        mSerialConOut[mSerialConOutCount] = ConOut;
        if (mSerialConOutCount < MAX_SERIAL_TERMINAL - 1) {
          mSerialConOutCount++;
        }
      }      
    }
  };

}


/**
  Register notify function to locate STO when console out devices are installed
    
  @retval EFI_SUCCESS   ConsoleOutCallback notify registeration succeeded.
  @retval others        ConsoleOutCallback notify registeration failed.

**/
EFI_STATUS
EFIAPI
RegisterProtocolForConOut (
  VOID
)
{
  EFI_STATUS  Status;
  VOID        *Registration;
  
  Status = EFI_SUCCESS;
  
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ConsoleOutCallback,
                  NULL,
                  &mConOutEvent
                  );
  if(EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiConsoleOutDeviceGuid,
                  mConOutEvent,
                  &Registration
                  );

  mSerialConOutRegisteration = Registration;

  return Status;
}

/**
  Unregister all protocol notify registed for event by closing the event.
**/
VOID
EFIAPI
UnRegisterProtocolConOut (
  VOID
)
{
  gBS->CloseEvent (mConOutEvent);

  return;
}

/**
 The PostCode feature handler which will be called by PostCodeFeatureHandlerEntry ()
 
 When PostCode is written to port 80, the processor will enter debug exception
 And the PostCodeFeatureHandlerEntry () will be called

 @param [in]  InterruptType  Exception type.
 @param [in]  SystemContext  System context data.

**/
VOID
EFIAPI 
PostCodeFeatureHandler (
  IN CONST  EFI_EXCEPTION_TYPE InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT SystemContext
  )
{
  UINT64  PostCode;
  UINT8   PostCodeLength;

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
  // set base time stamp pcd which will be used in PostCodeSmm driver
  //
  if (!mIsPcdPerformanceCounterReadyTickSet) {
    //
    // PostCodeDxe tried to record base time stamp when PostCodeDxeEntryPoint is executed 
    // but failed because Pcd services were not yet available,
    // so set base time stamp pcd here
    //
    SetPcdPerformanceCounterReadyTick ();
  }

  //
  // No VGA Console out protocol
  //
  if (!mConsoleOutDevDiscovered) {
    if (FeaturePcdGet (PcdH2OPostCodeSerialPortSupported)) {
      SerialPortPostCode (PostCode);
    }

    if (FeaturePcdGet (PcdH2OPostCodeScreenSupported)) {
      VgaFbPostCodeShow (PostCode);
      mIsVgaFbClean = FALSE;
    }
    
  } else {
  //
  // VGA Console device is discovered
  //
    if (FeaturePcdGet (PcdH2OPostCodeSerialPortSupported)) {
      if (!mCrEnable || mSerialPostCodeMode == SERIAL_MODE) {
        SerialPortPostCode (PostCode);
      } else {
        SerialConOutPostCode (PostCode);
      }
    }

    if (FeaturePcdGet (PcdH2OPostCodeScreenSupported)) {
      if (!mIsVgaFbClean) {
        VgaFbPostCodeClean();
        mIsVgaFbClean = TRUE;
      }
      
      VgaConOutPostCode (PostCode);
    }
  }
      
  //
  // When exits exception, cleans the trap status
  //
  ClearTrapStatus (Breakpoint0);
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

  Count    = 0;

  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = NotifyGuidList;
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {    
    Count++;
    NotifyGuid++;
  }
  
  return Count;
}

/**
  Release pool allocated by PostCode driver
**/
VOID
ReleaseResources (
  VOID
  )
{
  SafeFreePool (mEnableFeatureCpHandlerArr);
  SafeFreePool (mDisableFeatureCpHandlerArr);
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
  EFI_STATUS        Status;
  UINT8             *CrConfig;
  UINT8             *SerialPostCodeModeVar;
  
  Status                               = EFI_SUCCESS;
  mIsVgaFbClean                        = TRUE;
  mSerialConOutCount                   = 0;
  mConsoleOutDevDiscovered             = FALSE;
  mProtoNotifyEnabkeFeatureEvent       = NULL;
  mProtoNotifyDisableFeatureEvent      = NULL;
  mEnableFeatureCpHandlerArr           = NULL;
  mDisableFeatureCpHandlerArr          = NULL;
  mEnableFeatureCpHandlerCount         = 0;
  mDisableFeatureCpHandlerCount        = 0;
  mConOutEvent                         = NULL;
  mReadyToBootProcessEvent             = NULL;
  mReadyToBootDisableEvent             = NULL;

  mIsPcdPerformanceCounterReadyTickSet =  FALSE;

  CrConfig                            = NULL;
  SerialPostCodeModeVar               = NULL;
  
  mEnableFeatureCpHandlerMaxNum = CountNotifyCp ((EFI_GUID*) PcdGetPtr (PcdH2OPostCodeTrapDebugPortCpNotifyList));
  mDisableFeatureCpHandlerMaxNum = CountNotifyCp ((EFI_GUID*) PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortCpNotifyList));
  
  mEnableFeatureCpHandlerArr = (H2O_CP_HANDLE*) AllocateZeroPool (sizeof (H2O_CP_HANDLE) * mEnableFeatureCpHandlerMaxNum);
  if (mEnableFeatureCpHandlerArr == NULL && mEnableFeatureCpHandlerMaxNum != 0) {
    return EFI_OUT_OF_RESOURCES;
  }

  mDisableFeatureCpHandlerArr = (H2O_CP_HANDLE*) AllocateZeroPool (sizeof (H2O_CP_HANDLE) * mDisableFeatureCpHandlerMaxNum);
  if (mDisableFeatureCpHandlerArr == NULL && mDisableFeatureCpHandlerMaxNum != 0) {
    ReleaseResources ();
    return EFI_OUT_OF_RESOURCES;
  }

  Status = GetVariable2 (L"CrConfig", &gH2OCrConfigurationAliasGuid, (VOID**)&CrConfig, NULL);
  if (EFI_ERROR (Status)) {
    mCrEnable = FALSE;
  } else {
    mCrEnable = *CrConfig;
    SafeFreePool (CrConfig);
  }

  Status = GetVariable2 (POSTCODE_MODE_VARIABLE_NAME, &gH2OPostcodeSerialModeVariableGuid, (VOID**)&SerialPostCodeModeVar, NULL);
  if (!EFI_ERROR (Status)) {
    mSerialPostCodeMode = *SerialPostCodeModeVar;
    SafeFreePool (SerialPostCodeModeVar); 
  } else {
    mSerialPostCodeMode = FixedPcdGet8 (PcdH2OPostcodeSerialModeDefault);
  }

  SetPcdPerformanceCounterReadyTick ();  

  return EFI_SUCCESS;
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
  // This driver uses breakpoint 0 to trap debug port
  //
  DebugPort = PcdGet16 (PcdH2OPostCodeDebugPortAddress);
  SetUpDebugRegister (Breakpoint0, IoReadOrWrite, OneByte, DebugPort);
}

/**
  To enable Dxe PostCode features

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  
*/
VOID
EFIAPI
EnableFeatureCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EnableFeature ();  
}

/**
  To disable Dxe PostCode features by cleaning debug registers and disabling breakpoint 0

  @param [in]  Event     Event whose notification function is being invoked
  @param [in]  Context   
  
*/
VOID
EFIAPI
DisableFeatureCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  ClearDebugRegister (Breakpoint0);
}

/**
  Unregister all protocol notify registed for Enablefeature callback function by closing event.
**/
VOID
EFIAPI
UnRegisterProtocolEnableFeature (
  VOID
)
{
  gBS->CloseEvent (mProtoNotifyEnabkeFeatureEvent);

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
  gBS->CloseEvent (mProtoNotifyDisableFeatureEvent);

  return;
}

/**
  Register Enable Features "Protocol" notify function to oenable PostCode features
  
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
  EFI_GUID    ZeroGuid;
  EFI_GUID    *NotifyGuid;
  VOID        *Registration;

  Status = EFI_SUCCESS;

  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EnableFeatureCallback,
                  NULL,
                  &mProtoNotifyEnabkeFeatureEvent
                  );
  if(EFI_ERROR(Status)) {
    return Status;
  }
  NotifyGuid = (EFI_GUID*) PcdGetPtr (PcdH2OPostCodeTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = gBS->RegisterProtocolNotify (
                    NotifyGuid,
                    mProtoNotifyEnabkeFeatureEvent,
                    &Registration
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
  Register Disable Features "Protocol" notify function to disable PostCode features
    
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
  EFI_GUID    ZeroGuid;
  EFI_GUID    *NotifyGuid;
  VOID        *Registration;
  
  Status = EFI_SUCCESS;

  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  DisableFeatureCallback,
                  NULL,
                  &mProtoNotifyDisableFeatureEvent
                  );
  if(EFI_ERROR(Status)) {
    return Status;
  }  
  NotifyGuid = (EFI_GUID*) PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = gBS->RegisterProtocolNotify (
                    NotifyGuid,
                    mProtoNotifyDisableFeatureEvent,
                    &Registration
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
  Unregister all checkpoint registed for DisableFeature callback function by unregistering cphandle
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
  Register Enable Features "CheckPoint" notify function to enable PostCode features
    
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
  EFI_GUID       ZeroGuid;
  EFI_GUID       *NotifyGuid;
  
  Status = EFI_SUCCESS;

  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = PcdGetPtr (PcdH2OPostCodeTrapDebugPortCpNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status  = H2OCpRegisterHandler (
                NotifyGuid,
                EnableFeatureCallback,
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
  Register Disable Features "CheckPoint" notify function to disable PostCode features
    
  @retval EFI_SUCCESS   Trap debug port clean notify registeration succeeded.
  @retval others        Trap debug port clean notify registeration failed.

**/
EFI_STATUS
EFIAPI
RegisterCpForDisableFeature (
  VOID
)
{
  EFI_STATUS     Status;
  EFI_GUID       ZeroGuid;
  EFI_GUID       *NotifyGuid;

  Status = EFI_SUCCESS;
  
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  NotifyGuid = (EFI_GUID*) PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortCpNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status  = H2OCpRegisterHandler (
                NotifyGuid,
                DisableFeatureCallback,
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
  intention for disabling trap 0x80 at ready to boot

  @retval EFI_SUCCESS  Succeeded in creating a ready to boot event
  
*/
EFI_STATUS
EFIAPI
RegisterEventForDisableFeature (
  VOID
)
{  
  return EfiCreateEventReadyToBootEx (TPL_CALLBACK, DisableFeatureCallback, NULL, &mReadyToBootDisableEvent);
}

/**
  Disable Dxe PostCode features by cleaning debug registers and clean creen.

  @param [in]  Event     Event whose notification function is being invoked
  @param [in]  Context   Pointer to the notification function's context
  
*/
VOID
EFIAPI
ReadyToBootCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  ClearDebugRegister (Breakpoint0);

  if (!mIsVgaFbClean) {
    VgaFbPostCodeClean ();
  }
  
  VgaConOutClean ();
  SerialConOutClean ();
}

/**
  Register notify function to disable PostCode feature and clean screen
    
  @retval EFI_SUCCESS   ReadyToBootCallback notify registeration succeeded.
  @retval others        ReadyToBootCallback notify registeration failed.

**/
EFI_STATUS
EFIAPI
ReadyToBootProcess (
  VOID
)
{   
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             ReadyToBootCallback,
             NULL,
             &mReadyToBootProcessEvent
             );

  return Status;
}

/**
  To see if PostCodeDxe is enabled.

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
  
  PostCodeDxeEnalbe = FALSE;
  PostCodeDxeEnalbePtr = &PostCodeDxeEnalbe;

  //
  // Check variable
  //
  VarName = (CHAR16*) PcdGetPtr (PcdH2OPostCodeDxeEnableVarName);
  VarGuid = gH2OPostCodeDxeEnableVarGuid;
  Status = GetVariable2 (VarName, &VarGuid, &PostCodeDxeEnalbePtr, NULL);
  if (!EFI_ERROR(Status)) {
    return PostCodeDxeEnalbe;
  }

  //
  // Check PCD
  //
  PostCodeDxeEnalbe = PcdGetBool (PcdH2OPostCodeDxeEnable);
  
  return PostCodeDxeEnalbe;
}

/**
 Initialize the debug exception entry of IDT and debuger registers to trap port 80
 Register notify function to check IDT debug exception entry 
 Register notify function to disable to trap port 80
 Register notify function to get the GOP 

 @param [in]           ImageHandle
 @param [in]           SystemTable

 @retval EFI_SUCCESS   Initializes successfully.
 @return Other         Registration failed.

**/
EFI_STATUS
EFIAPI
PostCodeDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  BOOLEAN     RegisterProtocolEnable;
  BOOLEAN     RegisterProtocolDisable;
  BOOLEAN     RegisterCpEnable;
  BOOLEAN     RegisterCpDisable;

  RegisterProtocolEnable   = FALSE;
  RegisterProtocolDisable  = FALSE;
  RegisterCpEnable         = FALSE;
  RegisterCpDisable        = FALSE;
  
  if (!IsFeatureEnable ()) {
    return EFI_UNSUPPORTED;
  }
  
  Status = InitGlobalVar ();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  
  EnableFeature ();
  
  Status = RegisterProtocolForEnableFeature ();
  if(EFI_ERROR(Status)) {
    goto Error;
  }  
  RegisterProtocolEnable = TRUE;

  Status = RegisterCpForEnableFeature ();
  if(EFI_ERROR(Status)) {
    goto Error;
  }  
  RegisterCpEnable = TRUE;

  Status = RegisterProtocolForDisableFeature ();
  if(EFI_ERROR(Status)) {
    goto Error;
  }  
  RegisterProtocolDisable = TRUE;
  
  Status = RegisterCpForDisableFeature ();
  if(EFI_ERROR(Status)) {
    goto Error;
  }
  RegisterCpDisable = TRUE;

  Status = RegisterEventForDisableFeature ();
  if(EFI_ERROR(Status)) {
    goto Error;
  }

  Status = ReadyToBootProcess ();
  if(EFI_ERROR(Status)) {
    goto Error;
  }

  if (FeaturePcdGet (PcdH2OPostCodeScreenSupported)) {
    Status = RegisterProtocolForConOut ();
    if(EFI_ERROR(Status)) {
      goto Error;
    } 
  }

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

  if (mReadyToBootDisableEvent != NULL) {
    gBS->CloseEvent (mReadyToBootDisableEvent); 
  }

  if (mReadyToBootProcessEvent != NULL) {
    gBS->CloseEvent (mReadyToBootProcessEvent); 
  }

  if (mConOutEvent != NULL) {  
    UnRegisterProtocolConOut ();
  }

  ClearDebugRegister (Breakpoint0); 

  ReleaseResources ();
  
  return Status;
}

