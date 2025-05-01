/** @file
  This library locates the Memory Data Analysis protocol in library's constructor 
  to retrieve the protocol interface. If no protocol interface 
  is found, register a notify function to retrieve it.

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryDataAnalysisLib.h>
#include <Library/DebugLib.h>
#include <Protocol/MemoryDataAnalysis.h>

//
// The Memory Data Analysis protocol interface
//
EFI_MEMORY_DATA_ANALYSIS_PROTOCOL  *gMemoryDataAnalysisProtocol = NULL;

/**
  When the Memory Data Analysis protocol is installed, 
  locate it and cache the pointer of its interface.

  @param [in]  Event       The instance of EFI_EVENT.
  @param [in]  Context     The parameter passed in.
  
**/
VOID
EFIAPI
GetgMemoryDataAnalysisProtocol (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
                  &gH2ODebugAssistMemoryDataAnalysisProtocolGuid,
                  NULL,
                  (VOID **) &gMemoryDataAnalysisProtocol
                  );
  if (EFI_ERROR (Status)) {
    gMemoryDataAnalysisProtocol = NULL;
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  The constructor function locates the Memory Data Analysis protocol 
  and caches the pointer of its interface.
    
  If no protocol instance is found, register a notify function to retrieve it.
  It will ASSERT() if register notify function failure.
  It will always return EFI_SUCCESS.

  @param [in]  ImageHandle     The firmware allocated handle for the EFI image.
  @param [in]  SystemTable     A pointer to the EFI System Table.

  @retval EFI_SUCCESS          The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
MemoryDataAnalysisLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  VOID        *Registration;
    
  Status = gBS->LocateProtocol (
                  &gH2ODebugAssistMemoryDataAnalysisProtocolGuid,
                  NULL,
                  (VOID **) &gMemoryDataAnalysisProtocol
                  );
  if (EFI_ERROR (Status)) {
    gMemoryDataAnalysisProtocol = NULL;
    
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    GetgMemoryDataAnalysisProtocol,
                    NULL,
                    &Event
                    );
    if (!EFI_ERROR (Status)) {
      Status = gBS->RegisterProtocolNotify (
                      &gH2ODebugAssistMemoryDataAnalysisProtocolGuid,
                      Event,
                      &Registration
                      );
    }
    ASSERT_EFI_ERROR (Status);
  }
  
  return EFI_SUCCESS;
}

/**
  Analyze memory data and print them on serial port

  If each memory data(sizeof (UINTN)) is an address in the code section of a loaded image, 
  it will show the image information. 
  The actual analysis length will be aligned to sizeof (UINTN).

  Note: If the StartAddress is not aligned to sizeof (UINTN), 
        it will get incorrect image information.

  @param [in]  StartAddress     The start address of memory 
  @param [in]  Length           The length, in bytes, of memory 
  
  @retval EFI_SUCCESS           Analyze memory data successfully.
  
**/
EFI_STATUS
MemoryDataAnalysis (
  IN  EFI_PHYSICAL_ADDRESS    StartAddress,
  IN  UINTN                   Length
  )
{
  EFI_STATUS Status;

  if (gMemoryDataAnalysisProtocol == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = gMemoryDataAnalysisProtocol->MemoryDataAnalysis (
                                          StartAddress, 
                                          Length
                                          );

  return Status;
}

