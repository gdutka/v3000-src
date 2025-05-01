/** @file
  PostCodePei module implementation

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

#include <PostCodePei.h>

/**
 To enable PEI PostCode features

 @param [in]   PeiServices        General purpose services available to every PEIM.
 @param [in]   NotifyDescriptor
 @param [in]   Ppi

 @retval EFI_SUCCESS              Disable PostCode feature successfully.

**/
EFI_STATUS
EFIAPI 
EnableFeatureCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  UINT16 DebugPort;
  
  //
  // Register PostCodeFeatureHandlerEntry () to the debug exception entry of IDT
  // for performing PostCode Feature
  //
  RegisterIdtHandler (EXCEPT_IA32_DEBUG, (UINTN) PostCodeFeatureHandlerEntry);

  //
  // Configure the debug registers to trap debug port 
  // This module uses breakpoint 0 to trap debug port 
  //
  DebugPort = PcdGet16 (PcdH2OPostCodeDebugPortAddress);
  SetUpDebugRegister (Breakpoint0, IoReadOrWrite, OneByte, DebugPort);
  
  return EFI_SUCCESS; 
}

/**
 To enable PEI PostCode features

 @param [in]   PeiServices        General purpose services available to every PEIM.
 @param [in]   NotifyDescriptor
 @param [in]   Ppi

 @retval EFI_SUCCESS              Disable PostCode feature successfully.

**/
EFI_STATUS
EFIAPI 
CpEnableFeatureCallback (
  IN EFI_EVENT                          Event,
  IN H2O_CP_HANDLE                      Handle
  )
{
  UINT16 DebugPort;
  
  //
  // Register PostCodeFeatureHandlerEntry () to the debug exception entry of IDT
  // for performing PostCode Feature
  //
  RegisterIdtHandler (EXCEPT_IA32_DEBUG, (UINTN) PostCodeFeatureHandlerEntry);

  //
  // Configure the debug registers to trap debug port 
  // This module uses breakpoint 0 to trap debug port 
  //
  DebugPort = PcdGet16 (PcdH2OPostCodeDebugPortAddress);
  SetUpDebugRegister (Breakpoint0, IoReadOrWrite, OneByte, DebugPort);
  
  return EFI_SUCCESS; 
}


/**
 To disable PEI PostCode features

 @param [in]   PeiServices        General purpose services available to every PEIM.
 @param [in]   NotifyDescriptor
 @param [in]   Ppi

 @retval EFI_SUCCESS              Disable PostCode feature successfully.

**/
EFI_STATUS
EFIAPI 
DisableFeatureCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  //
  // Clean the debug registers to disable breakpoint 0
  //
  ClearDebugRegister (Breakpoint0);
  
  return EFI_SUCCESS; 
}
/**
 To disable PEI PostCode features

 @param [in]   PeiServices        General purpose services available to every PEIM.
 @param [in]   NotifyDescriptor
 @param [in]   Ppi

 @retval EFI_SUCCESS              Disable PostCode feature successfully.

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
  
  return EFI_SUCCESS; 
}


/**
 The PostCode feature handler which will be called by PostCodeFeatureHandlerEntry ()
 
 When PostCode is written to port 80, the processor will enter debug exception
 And the PostCodeFeatureHandlerEntry () will be called

 @param [in]   PostCode   The PostCode which was sent to port 80

**/
VOID
EFIAPI 
PostCodeFeatureHandler (
  IN CONST UINT32 Eax,
  IN CONST UINT32 Eip
  )
{
  CHAR8  String[BUFFER_SIZE]; 
  UINT32 PostCode;
  UINT8  PostCodeLength;
  UINTN  CharCount; 
  UINTN  PostCodeMask;

  //
  // Check if this trap was triggered by reading the debug port
  //
  if (IsTrappedByReadPort(Eip)) {
    //
    // When exits exception, cleans the trap status
    //
    ClearTrapStatus (Breakpoint0);
    return;
  }

  //
  // Get the length of the Post Code which was outputted to debug port
  //
  PostCodeLength = GetPostCodeLength (Eip);

  //
  // Get the Post Code from EAX register 
  // Post Code is outputted by "out" instruction and the outputted value will be put to the EAX register
  // It may not be that all bytes of the EAX register are Post Code, so we need to filter the EAX register
  //
  switch (PostCodeLength) {
  case 1:
    PostCodeMask = 0xff;
    break;
    
  case 2:
    PostCodeMask = 0xffff;
    break;
    
  case 4:
    PostCodeMask = 0xffffffff;
    break;

  default:
    PostCodeMask = 0xffffffff;
  }
  PostCode = Eax & PostCodeMask;

  //
  // Send PostCode to serial port
  //
  if (FeaturePcdGet (PcdH2OPostCodeSerialPortSupported)) {
    SerialPortPostCode (PostCode);
  }

  //
  // Print PostCode on screen
  //
  if (FeaturePcdGet (PcdH2OPostCodeScreenSupported)) {
    //
    // Use Graphic Library to print PostCode
    // 
    CharCount = GetPostCodeString (
                  ScreenDisplayStr,
                  PostCode,
                  BUFFER_SIZE,
                  String
                  );
    GraphicVgaShowString (
      (UINT16) (GRAPHIC_VGA_MAX_COL - CharCount - 2), 
      (UINT16) (GRAPHIC_VGA_MAX_ROW - 2), 
      GRAPHIC_VGA_WHITE, 
      GRAPHIC_VGA_BLACK, 
      String
      );
  }

  //
  // When exits exception, cleans the trap status
  //
  ClearTrapStatus (Breakpoint0); 
}

/**
 Initialize the debug exception entry of IDT and debuger registers to trap port 80
 Register notify ppi to disable to trap port 80

 @param [in]   FileHandle
 @param [in]   PeiServices   The PEI core services table.

 @retval EFI_SUCCESS         Initializes successfully.
 @return Other               Error occurs while notifying PPI.

**/
EFI_STATUS
EFIAPI
PostCodePeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{ 
  EFI_STATUS                Status;
  EFI_GUID                  ZeroGuid;
  EFI_GUID                  *NotifyGuid;
  EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor;
  UINT16                    DebugPort;
  H2O_CP_HANDLE             CpHandle;

  //
  // Initialize the ZeroGuid
  //
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  //
  // Register PostCodeFeatureHandlerEntry () to the debug exception entry of IDT
  // for performing PostCode Feature
  //
  RegisterIdtHandler (EXCEPT_IA32_DEBUG, (UINTN) PostCodeFeatureHandlerEntry);

  //
  // Configure the debug registers to trap debug port
  // This module uses breakpoint 0 to trap debug port
  //
  DebugPort = PcdGet16 (PcdH2OPostCodeDebugPortAddress);
  SetUpDebugRegister (Breakpoint0, IoReadOrWrite, OneByte, DebugPort);

  //
  // Register Enable Features "Protocol" notify function
  // To enable PostCode features
  //
  NotifyGuid = PcdGetPtr (PcdH2OPostCodeTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = (**PeiServices).AllocatePool (
                               PeiServices,
                               sizeof (EFI_PEI_NOTIFY_DESCRIPTOR),
                               &NotifyDescriptor
                               );
    if (EFI_ERROR (Status)) {
      goto Error;
    }

    NotifyDescriptor->Flags  = EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | 
                               EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    NotifyDescriptor->Guid   = NotifyGuid;
    NotifyDescriptor->Notify = EnableFeatureCallback;
    
    Status = (**PeiServices).NotifyPpi (PeiServices, NotifyDescriptor);
    if (EFI_ERROR (Status)) {
      goto Error;
    }
    
    NotifyGuid++;
  }

  //
  // Register Enable Features "CheckPoint" notify function
  // To enable PostCode features
  //
  NotifyGuid = PcdGetPtr (PcdH2OPostCodeTrapDebugPortCpNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = H2OCpRegisterHandler (
               NotifyGuid,
               CpEnableFeatureCallback,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    if(EFI_ERROR(Status)) {
      goto Error;
    }  
    NotifyGuid++;
  }
  //
  // Register Disable Features "Protocol" notify function 
  // To disable PostCode features
  //
  NotifyGuid = PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = (**PeiServices).AllocatePool (
                               PeiServices,
                               sizeof (EFI_PEI_NOTIFY_DESCRIPTOR),
                               &NotifyDescriptor
                               );
    if (EFI_ERROR (Status)) {
      goto Error;
    }

    NotifyDescriptor->Flags  = EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | 
                               EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    NotifyDescriptor->Guid   = NotifyGuid;
    NotifyDescriptor->Notify = DisableFeatureCallback;
    
    Status = (**PeiServices).NotifyPpi (PeiServices, NotifyDescriptor);
    if (EFI_ERROR (Status)) {
      goto Error;
    }
    
    NotifyGuid++;
  }
  //
  // Register Enable Features "CheckPoint" notify function
  // To disable PostCode features
  //
  NotifyGuid = PcdGetPtr (PcdH2OPostCodeNonTrapDebugPortCpNotifyList);
  while (!CompareGuid (NotifyGuid, &ZeroGuid)) {
    Status = H2OCpRegisterHandler (
               NotifyGuid,
               CpDisableFeatureCallback,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    if(EFI_ERROR(Status)) {
      goto Error;
    }  
    NotifyGuid++;
  }

  //
  // Initialize serial port
  //
  if (FeaturePcdGet (PcdH2OPostCodeSerialPortSupported)) {
    SerialPortInitialize ();
  }
  
  return EFI_SUCCESS;
  
Error:
  ClearDebugRegister (Breakpoint0);
  return Status;
}

