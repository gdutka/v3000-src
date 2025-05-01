/** @file
//
//Brief description of file's purpose. Detailed description of file's purpose.
//
//;******************************************************************************
//;* Copyright (c) 2014-2014, Insyde Software Corporation. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
//
//
//other copy right statement
*/
/** @file
  TODO: Brief Description of UEFI Driver EventTraceDxe
  
  TODO: Detailed Description of UEFI Driver EventTraceDxe

  TODO: Copyright for UEFI Driver EventTraceDxe
  
  TODO: License for UEFI Driver EventTraceDxe

**/

#ifndef __EFI_EVENT_TRACE_DXE_H__
#define __EFI_EVENT_TRACE_DXE_H__

#include <Uefi.h>

//
// Libraries
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

//
// UEFI Driver Model Protocols
//

//
// Consumed Protocols
//
#include <Protocol/LoadedImage.h>
#include <Protocol/DriverArpService.h>

//
// Produced Protocols
//

//
// Guids
//
#include <Guid/ZeroGuid.h>

//
// Driver Version
//

//
// Protocol instances
//

//
// Include files with function prototypes
//
#pragma pack(1)

typedef struct _EVENT_TRACE_DXE_CONTEXT_ {
  UINT32                        Type;
  UINT32                        SignalCount;
  BOOLEAN                       TraceEnabled;
  UINT8                         Reserved[3];
  EFI_TPL                       NotifyTpl;
  EFI_EVENT_NOTIFY              NotifyFunction;
  VOID                          *NotifyContext;
  EFI_GUID                      *EventGroup;
  EFI_EVENT                     Event;
} EVENT_TRACE_DXE_CONTEXT;

typedef struct _EVENT_TRACE_DXE_DB_ {
  UINT32                        Signature;
  UINT32                        Reserved;
  EVENT_TRACE_DXE_CONTEXT       *Context;
  LIST_ENTRY                    List;
} EVENT_TRACE_DXE_DB;

#define EVENT_TRACE_DXE_DB_SIGNATURE  SIGNATURE_32 ('e', 'v', 't', 'd')
#define EVENT_TRACE_DXE_DB_FROM_LIST(a) \
  CR (a, EVENT_TRACE_DXE_DB, List, EVENT_TRACE_DXE_DB_SIGNATURE)

typedef struct _EVENT_TRACE_DXE_TYPE_MAP_ {
  UINT32                        TraceBitField;
  UINT32                        Type;
  BOOLEAN                       Enabled;
} EVENT_TRACE_DXE_TRACE_MAP;

typedef enum {
  EvtTraceMethodAll             = 0,
  EvtTraceMethodSpecificDrvs,
  EvtTraceMethodNotSpecificDrvs,
  EvtTraceMethodMax
} EVT_TRACE_NETHOD;

typedef struct _EVENT_TRACE_DXE_TARGET_INFO_ {
  CHAR8                         *FileName;
  GUID                          *FileGuid;
} EVENT_TRACE_DXE_TARGET_INFO;

#pragma pack()

#define PCD_TIMER_SIGNAL_EVT            0x00000001
#define PCD_WAIT_EVT                    0x00000002
#define PCD_SIGNAL_EVT                  0x00000004
#define PCD_EXIT_BOOT_SERVICES_EVT      0x00000008
#define PCD_VIRTUAL_ADDRESS_CHANGE_EVT  0x00000010
#define PCD_TIMER_WAIT_EVT              0x00000020

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
EventTraceDxeGetImageInfo (
  IN  EFI_EVENT_NOTIFY                  NotifyFunction,
  OUT DRIVER_ARP_SERVICE_IMAGE_INFO     *Info
  );

/**
  Creates an event.

  @param  Type                   The type of event to create and its mode and
                                 attributes
  @param  NotifyTpl              The task priority level of event notifications
  @param  NotifyFunction         Pointer to the events notification function

  @retval TRUE                   The event is enabled for tracing
  @retval FALSE                  The event is disabled for tracing

**/
BOOLEAN
EFIAPI
IsTraceThisEvent (
  IN UINT32                   Type,
  IN EFI_EVENT_NOTIFY         NotifyFunction,
  IN CONST EFI_GUID           *EventGroup     OPTIONAL
  );

/**
  Unloads an image.

  @param  ImageHandle           Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
EFI_STATUS 
EFIAPI
EventTraceDxeUnload (
  IN EFI_HANDLE  ImageHandle
  );

/**

  Notify function for Loaded Image protocol  installed

  @param  Event    The Event that is being processed
  @param  Context  The Event Context

**/
VOID
EFIAPI
EventTraceDxePrintEventType (
  IN UINT32       Type
  );

/**

  Notify function for Loaded Image protocol  installed

  @param  Event    The Event that is being processed
  @param  Context  The Event Context

**/
VOID
EFIAPI
EventTraceDxeNotifyFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  Creates an event.

  @param  Type                   The type of event to create and its mode and
                                 attributes
  @param  NotifyTpl              The task priority level of event notifications
  @param  NotifyFunction         Pointer to the events notification function
  @param  NotifyContext          Pointer to the notification functions context;
                                 corresponds to parameter "Context" in the
                                 notification function
  @param  Event                  Pointer to the newly created event if the call
                                 succeeds; undefined otherwise

  @retval EFI_SUCCESS            The event structure was created
  @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated

**/
EFI_STATUS
EFIAPI
EventTraceDxeCreateEvent (
  IN UINT32                   Type,
  IN EFI_TPL                  NotifyTpl,
  IN EFI_EVENT_NOTIFY         NotifyFunction, OPTIONAL
  IN VOID                     *NotifyContext, OPTIONAL
  OUT EFI_EVENT               *Event
  );

/**
  Creates an event in a group.

  @param  Type                   The type of event to create and its mode and
                                 attributes
  @param  NotifyTpl              The task priority level of event notifications
  @param  NotifyFunction         Pointer to the events notification function
  @param  NotifyContext          Pointer to the notification functions context;
                                 corresponds to parameter "Context" in the
                                 notification function
  @param  EventGroup             GUID for EventGroup if NULL act the same as
                                 gBS->CreateEvent().
  @param  Event                  Pointer to the newly created event if the call
                                 succeeds; undefined otherwise

  @retval EFI_SUCCESS            The event structure was created
  @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated

**/
EFI_STATUS
EFIAPI
EventTraceDxeCreateEventEx (
  IN UINT32                   Type,
  IN EFI_TPL                  NotifyTpl,
  IN EFI_EVENT_NOTIFY         NotifyFunction, OPTIONAL
  IN CONST VOID               *NotifyContext, OPTIONAL
  IN CONST EFI_GUID           *EventGroup,    OPTIONAL
  OUT EFI_EVENT               *Event
  );

/**
  Closes an event and frees the event structure.

  @param  UserEvent              Event to close

  @retval EFI_INVALID_PARAMETER  Parameters are not valid.
  @retval EFI_SUCCESS            The event has been closed

**/
EFI_STATUS
EFIAPI
EventTraceDxeCloseEvent (
  IN EFI_EVENT    UserEvent
  );

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
EventTraceDxeDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif

