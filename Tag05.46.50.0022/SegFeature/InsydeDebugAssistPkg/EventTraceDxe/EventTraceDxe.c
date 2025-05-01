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

#include "EventTraceDxe.h"

LIST_ENTRY                              mDb = {&mDb, &mDb};
EFI_LOCK                                mDbLock;
EFI_CREATE_EVENT                        mDxeCoreCreateEvent = NULL;
EFI_CREATE_EVENT_EX                     mDxeCoreCreateEventEx = NULL;
EFI_CLOSE_EVENT                         mDxeCoreCloseEvent = NULL;
EFI_DRIVER_ARP_SERVICE_PROTOCOL         *mDriverArpService = NULL;
EVENT_TRACE_DXE_TRACE_MAP               mEventTable[] = {
//  TraceBitField                ,           Type                           , Enabld
  {PCD_TIMER_SIGNAL_EVT,            (UINT32) (EVT_TIMER | EVT_NOTIFY_SIGNAL), FALSE},
  {PCD_WAIT_EVT,                    (UINT32) (EVT_NOTIFY_WAIT),               FALSE},
  {PCD_SIGNAL_EVT,                  (UINT32) (EVT_NOTIFY_SIGNAL),             FALSE},
  {PCD_EXIT_BOOT_SERVICES_EVT,      (UINT32) (EVT_SIGNAL_EXIT_BOOT_SERVICES), FALSE},
  {PCD_VIRTUAL_ADDRESS_CHANGE_EVT,  (UINT32) (EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE), FALSE},
  {PCD_TIMER_WAIT_EVT,              (UINT32) (EVT_TIMER | EVT_NOTIFY_WAIT),   FALSE}
};

GUID                                    *mEvtGroupList = NULL;
UINTN                                   mNumberOfEvtGroupList = 0;

EVENT_TRACE_DXE_TARGET_INFO             *mWhiteList = NULL, *mBlackList = NULL;
UINTN                                   mNumberOfWhiteList = 0, mNumberOfBlackList = 0;

#define EVENT_TRACE_DXE_TABLE_ENTIRES (sizeof (mEventTable) / sizeof (EVENT_TRACE_DXE_TRACE_MAP))

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
  )
{
  EFI_STATUS                            Status = RETURN_NOT_READY;
  UINTN                                 NumberOfInfo;

  //
  if ((NotifyFunction == NULL) || (Info == NULL)) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  //
  //
  if (mDriverArpService == NULL) {
    Status = gBS->LocateProtocol (
                      &gH2ODebugAssistDriverArpServiceProtocolGuid,
                      NULL,
                      (VOID *) &mDriverArpService
                      );
    if (RETURN_ERROR (Status)) {
      return RETURN_NOT_READY;
    }
  }

  NumberOfInfo = 1;
  ZeroMem ((VOID *) Info, sizeof (DRIVER_ARP_SERVICE_IMAGE_INFO));
  Status = mDriverArpService->RetrieveImageByAddress (
                                mDriverArpService,
                                (EFI_PHYSICAL_ADDRESS)(UINTN) NotifyFunction,
                                Info,
                                &NumberOfInfo
                                );

  return Status;
}

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
  )
{
  UINT32                                Index;
  UINT32                                Index2;
  EFI_STATUS                            Status = RETURN_SUCCESS;
  BOOLEAN                               IsTargetInList = FALSE;
  EVT_TRACE_NETHOD                      TraceMethod = EvtTraceMethodAll;

  DRIVER_ARP_SERVICE_IMAGE_INFO         Info;

  // We don't have to trace an event w/ out notify function
  if (NotifyFunction == NULL)
    return FALSE;

  // If tracing targets of event group are specificed, but the event is not
  // within a group
  if ((mEvtGroupList != NULL) && (EventGroup == NULL))
    return FALSE;

  // If the trace specific drivers but w/ an empty list, then just return false
  TraceMethod = PcdGet32 (PcdH2ODebugAssistEvtTraceMethod);
  if ((TraceMethod == EvtTraceMethodSpecificDrvs) && (mWhiteList == NULL)) {
    return FALSE;
  }

  // Check if programmer wants to trace the event type
  for (Index = 0; Index < EVENT_TRACE_DXE_TABLE_ENTIRES; Index++) {
    if (Type == mEventTable[Index].Type) {
      break;
    }
  }
  if (Index >= EVENT_TRACE_DXE_TABLE_ENTIRES) {
    // Out of pre-defined event types
    return FALSE;
  } else if (!mEventTable[Index].Enabled) {
    // Programmer doesn't need to trace this type of event
    return FALSE;
  }

  //
  // Check is specific event group
  //
  if (mEvtGroupList != NULL) {
    for (Index2 = 0; Index2 < mNumberOfEvtGroupList; Index2++) {
      if (CompareGuid (&mEvtGroupList[Index2], EventGroup)) {
        IsTargetInList = TRUE;
        break;
      }
    }

    if (!IsTargetInList) {
      return IsTargetInList;
    }
  }

  //
  switch (TraceMethod) {
  //
  case EvtTraceMethodSpecificDrvs:
    //
    IsTargetInList = FALSE;
    Status = EventTraceDxeGetImageInfo (NotifyFunction, &Info);
    if (RETURN_ERROR (Status)) {
      return FALSE; // Assume the event is created by non-acceptable driver
    }

    //
    for (Index2 = 0; Index2 < mNumberOfWhiteList; Index2++) {
      //
      if (
        (!CompareGuid (&gZeroGuid, mWhiteList[Index2].FileGuid)) &&
        (CompareGuid (&Info.ImageFileGuid, mWhiteList[Index2].FileGuid))
        ) {
        IsTargetInList = TRUE;
        break;
      }
      //
      if ((Info.ImageFileName != NULL) && (mWhiteList[Index2].FileName[0] != '\0')) {
        if (AsciiStrCmp (Info.ImageFileName, mWhiteList[Index2].FileName) == 0) {
          IsTargetInList = TRUE;
          break;
        }
      }
    }

    return IsTargetInList;
    break;

  case EvtTraceMethodNotSpecificDrvs:
    //
    IsTargetInList = FALSE;
    Status = EventTraceDxeGetImageInfo (NotifyFunction, &Info);
    if (RETURN_ERROR (Status)) {
      return TRUE; // Assume the event is created by acceptable driver
    }

    //
    for (Index2 = 0; Index2 < mNumberOfBlackList; Index2++) {
      //
      if (
        (!CompareGuid (&gZeroGuid, mWhiteList[Index2].FileGuid)) &&
        (CompareGuid (&Info.ImageFileGuid, mWhiteList[Index2].FileGuid))
        ) {
        IsTargetInList = TRUE;
        break;
      }
      //
      if ((Info.ImageFileName != NULL) && (mBlackList[Index2].FileName[0] != '\0')) {
        if (AsciiStrCmp (Info.ImageFileName, mBlackList[Index2].FileName) == 0) {
          IsTargetInList = TRUE;
          break;
        }
      }
    }

    return !IsTargetInList;
    break;

  case EvtTraceMethodAll:
  case EvtTraceMethodMax:
  default:
    return mEventTable[Index].Enabled;
    break;
  }

}

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
  )
{
  EFI_STATUS  Status;
  LIST_ENTRY  *List, *Next;
  EVENT_TRACE_DXE_CONTEXT *Context, Local;

  Status = EFI_SUCCESS;

  //
  // Do any additional cleanup that is required for this driver
  //
  gBS->CreateEvent    = mDxeCoreCreateEvent;
  gBS->CreateEventEx  = mDxeCoreCreateEventEx;
  gBS->CloseEvent     = mDxeCoreCloseEvent;

  //
  // TODO: Restore hooked notify functions ...
  //
  if (!IsListEmpty (&mDb)) {
    for (List = mDb.ForwardLink; List != &mDb; List = Next) {

      //
      Next = List->ForwardLink; // Backup the next link before remove it.
      Context = (EVENT_TRACE_DXE_DB_FROM_LIST (List))->Context;
      Local = *Context;

      Status = EventTraceDxeCloseEvent (Local.Event);
      ASSERT_EFI_ERROR (Status);

      // BUGBUG: Origin function will loss event here ...
      Status = gBS->CreateEventEx (
                      Local.Type,
                      Local.NotifyTpl,
                      Local.NotifyFunction,
                      Local.NotifyContext,
                      Local.EventGroup,
                      Local.Event
                      );
      ASSERT_EFI_ERROR (Status);
    }
  }

  //
  if (mEvtGroupList != NULL)
    FreePool (mEvtGroupList);

  //
  if (mWhiteList != NULL) {
    if (mWhiteList[0].FileName != NULL)
      FreePool ((VOID *) mWhiteList[0].FileName);
    FreePool (mWhiteList);
  }
  if (mBlackList != NULL) {
    if (mBlackList[0].FileName != NULL)
      FreePool ((VOID *) mBlackList[0].FileName);
    FreePool (mBlackList);
  }

  return RETURN_UNSUPPORTED;
}

/**

  Notify function for Loaded Image protocol  installed

  @param  Event    The Event that is being processed
  @param  Context  The Event Context

**/
VOID
EFIAPI
EventTraceDxePrintEventType (
  IN UINT32       Type
  )
{
  DEBUG ((DEBUG_INFO, "[EventTraceDxe]  "));

  if ((Type & EVT_TIMER) == EVT_TIMER)
    DEBUG ((DEBUG_INFO, "Timer Event | "));
  if ((Type & EVT_RUNTIME) == EVT_RUNTIME)
    DEBUG ((DEBUG_INFO, "Runtime Event | "));
  if ((Type & EVT_NOTIFY_WAIT) == EVT_NOTIFY_WAIT)
    DEBUG ((DEBUG_INFO, "Notify Wait Event | "));
  if ((Type & EVT_NOTIFY_SIGNAL) == EVT_NOTIFY_SIGNAL)
    DEBUG ((DEBUG_INFO, "Notify Signal Event | "));
  if ((Type & EVT_SIGNAL_EXIT_BOOT_SERVICES) == EVT_SIGNAL_EXIT_BOOT_SERVICES)
    DEBUG ((DEBUG_INFO, "Signal Exit Boot Services Event | "));
  if ((Type & EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE) == EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE)
    DEBUG ((DEBUG_INFO, "Signal Virtual Address Change Event | "));
  if ((Type & EVT_RUNTIME_CONTEXT) == EVT_RUNTIME_CONTEXT)
    DEBUG ((DEBUG_INFO, "Runtime Context Event | "));

  DEBUG ((DEBUG_INFO, "\n"));

  return;
}

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
  )
{
  EFI_STATUS                            Status;
  BOOLEAN                               TraceEnabled;
  EVENT_TRACE_DXE_CONTEXT               *EvtTraceDxeContext;

  DRIVER_ARP_SERVICE_IMAGE_INFO         Info;

  //
  Status  = RETURN_SUCCESS;
  EvtTraceDxeContext = (EVENT_TRACE_DXE_CONTEXT *) Context;
  TraceEnabled = EvtTraceDxeContext->TraceEnabled;

  ASSERT (EvtTraceDxeContext != NULL);
  ASSERT (EvtTraceDxeContext->NotifyFunction != NULL);

  //
  // Output trace messages
  //
  if (TraceEnabled) {

    EvtTraceDxeContext->SignalCount ++;

    if (
      (PcdGet32 (PcdH2ODebugAssistEvtTraceTimes) != 0) &&
      (EvtTraceDxeContext->SignalCount >= PcdGet32 (PcdH2ODebugAssistEvtTraceTimes))
      ) {
      EvtTraceDxeContext->TraceEnabled = FALSE;
    }

    EventTraceDxePrintEventType (EvtTraceDxeContext->Type);
    DEBUG ((DEBUG_INFO, 
      "[EventTraceDxe] Notify fun@ 0x%0lx",
      (UINTN) EvtTraceDxeContext->NotifyFunction
      ));

    //
    Status = EventTraceDxeGetImageInfo (EvtTraceDxeContext->NotifyFunction, &Info);
    if (!RETURN_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, 
        " (offset:0x%0lx) (%g) (%a).\n",
        ((EFI_PHYSICAL_ADDRESS)(UINTN) EvtTraceDxeContext->NotifyFunction - Info.ImageBase),
        &Info.ImageFileGuid,
        (Info.ImageFileName == NULL ? "<Unknow>" : Info.ImageFileName)
        ));
    }
//    else {
//      DEBUG ((DEBUG_INFO, "\n[EventTraceDxe] Retrieve image's file name & GUID fail (%r).\n", Status));
//    }
  }

  // Invoke event callback function
  EvtTraceDxeContext->NotifyFunction (Event, EvtTraceDxeContext->NotifyContext);

  if (TraceEnabled)
    DEBUG ((DEBUG_INFO, "[EventTraceDxe] Leave fun@ 0x%0lx ...\n", (UINTN) EvtTraceDxeContext->NotifyFunction));

  //
  return;
}

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
  )
{
  ASSERT (mDxeCoreCreateEvent != NULL);
  return EventTraceDxeCreateEventEx (Type, NotifyTpl, NotifyFunction, NotifyContext, NULL, Event);
}

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
  )
{
  EFI_STATUS                            Status = RETURN_SUCCESS;
  EVENT_TRACE_DXE_CONTEXT               *EvtTraceDxeContext = NULL;
  EVENT_TRACE_DXE_DB                    *EvtDb = NULL;

  ASSERT (mDxeCoreCreateEventEx != NULL);

  //
  // Check if traced event type enabled
  //
  if (IsTraceThisEvent (Type, NotifyFunction, EventGroup)) {
    //
    // Trace the event
    //
    EvtTraceDxeContext  = \
      (EVENT_TRACE_DXE_CONTEXT *) AllocateZeroPool (sizeof (EVENT_TRACE_DXE_CONTEXT));

    if (EvtTraceDxeContext != NULL) {
      Status = mDxeCoreCreateEventEx (Type,
                                      NotifyTpl,
                                      EventTraceDxeNotifyFunction,
                                      (VOID *) EvtTraceDxeContext,
                                      EventGroup,
                                      Event);
      if (!RETURN_ERROR (Status)) {
        EvtTraceDxeContext->Type            = Type;
        EvtTraceDxeContext->TraceEnabled    = TRUE;
        EvtTraceDxeContext->NotifyTpl       = NotifyTpl;
        EvtTraceDxeContext->NotifyFunction  = NotifyFunction;
        EvtTraceDxeContext->NotifyContext   = (VOID *) NotifyContext;
        EvtTraceDxeContext->EventGroup      = (EFI_GUID *) EventGroup;
        EvtTraceDxeContext->Event           = Event;

        // Add this new trace into database
        EvtDb = (EVENT_TRACE_DXE_DB *) AllocateZeroPool (sizeof (EVENT_TRACE_DXE_DB));
        ASSERT (EvtDb != NULL);

        if (EvtDb != NULL) {
          EvtDb->Signature  = EVENT_TRACE_DXE_DB_SIGNATURE;
          EvtDb->Context    = EvtTraceDxeContext;

          EfiAcquireLock (&mDbLock);
          InsertTailList (&mDb, &(EvtDb->List));
          EfiReleaseLock (&mDbLock);
        }

        return Status;
      }
    } else {
      Status = RETURN_OUT_OF_RESOURCES;
    }

    //
    if (RETURN_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "[EventTraceDxe] Can't trace notify function@ 0x%0lx, because %r.\n",
        NotifyFunction,
        Status
        ));
    }
  }

  //
  return mDxeCoreCreateEventEx (Type, NotifyTpl, NotifyFunction, NotifyContext, EventGroup, Event);
}

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
  )
{
  LIST_ENTRY                            *List = NULL;
  EVENT_TRACE_DXE_CONTEXT               *EvtTraceDxeContext = NULL;

  ASSERT (mDxeCoreCloseEvent != NULL);

  //
  EfiAcquireLock (&mDbLock);

  //
  for (List = mDb.ForwardLink; List != &mDb; List = List->ForwardLink) {
    EvtTraceDxeContext = (EVENT_TRACE_DXE_DB_FROM_LIST (List))->Context;
    if (UserEvent == EvtTraceDxeContext->Event) {
      break;
    }
  }

  EfiReleaseLock (&mDbLock);

//[-start-190418-IB05820463-add]//
  if (EvtTraceDxeContext == NULL) {
    return EFI_NOT_FOUND;
  }
//[-end-190418-IB05820463-add]//
  
  if (UserEvent == EvtTraceDxeContext->Event) {

    EfiAcquireLock (&mDbLock);
    RemoveEntryList (List);
    EfiReleaseLock (&mDbLock);

    FreePool (EvtTraceDxeContext);
    FreePool (List);
  }

  return mDxeCoreCloseEvent (UserEvent);
}

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
  )
{
  EFI_STATUS                            Status;
  UINT32                                Index;
  UINT32                                TraceBitMap;
  EFI_LOADED_IMAGE_PROTOCOL             *Img;
  EVT_TRACE_NETHOD                      TraceMethod;
  UINTN                                 PcdSize;
  VOID                                  *Buffer;
  UINTN                                 *NumberPtr;
  CHAR8                                 *StrPtr;
  EVENT_TRACE_DXE_TARGET_INFO           **TargetListPtr;

  //
  Status      = EFI_SUCCESS;
  Img         = NULL;
//  TraceMethod = EvtTraceMethodAll;
  PcdSize     = 0;
  Buffer      = NULL;
  NumberPtr   = NULL;
  StrPtr      = NULL;
  TargetListPtr = NULL;

  InitializeListHead (&mDb);
  EfiInitializeLock (&mDbLock, TPL_HIGH_LEVEL);

  //
  // Prepare the tracing event type table
  //
  DEBUG ((DEBUG_INFO, "[EventTraceDxe] Enable tracing types:\n"));
  TraceBitMap = PcdGet32 (PcdH2ODebugAssistEvtTraceTypeBitMap);
  for (Index = 0; Index < EVENT_TRACE_DXE_TABLE_ENTIRES; Index++) {
    if ((TraceBitMap & mEventTable[Index].TraceBitField) != 0) {
      mEventTable[Index].Enabled = TRUE;
      EventTraceDxePrintEventType (mEventTable[Index].Type);
    }
  }

  //
  // Prepare the event group list
  //
  PcdSize = LibPcdGetExSize (&gH2ODebugAssistTokenSpaceGuid,
                            PcdTokenEx (&gH2ODebugAssistTokenSpaceGuid, PcdH2ODebugAssistEvtTraceGroupList));
  if (PcdSize >= sizeof (GUID)) {
    Buffer = AllocateZeroPool (PcdSize);
    if (Buffer != NULL) {
      CopyMem (Buffer, PcdGetExPtr (&gH2ODebugAssistTokenSpaceGuid, PcdH2ODebugAssistEvtTraceGroupList), PcdSize);

      mEvtGroupList = (GUID *) Buffer;
      mNumberOfEvtGroupList = (PcdSize / sizeof (GUID));

      DEBUG ((DEBUG_INFO, "[EventTraceDxe] Trace specific event groups only:\n"));
      for (Index = 0; Index < mNumberOfEvtGroupList; Index++) {
        DEBUG ((DEBUG_INFO, "[EventTraceDxe] %g\n", &mEvtGroupList[Index]));
      }
    }
  }

  //
  // Prepare the white or black tracing drivers' list (according to the trace method)
  //
  PcdSize = 0;
  TraceMethod = PcdGet32 (PcdH2ODebugAssistEvtTraceMethod);
  if (TraceMethod == EvtTraceMethodSpecificDrvs) {

    DEBUG ((DEBUG_INFO, "[EventTraceDxe] Trace specific drivers only:\n"));

    PcdSize = LibPcdGetExSize (&gH2ODebugAssistTokenSpaceGuid,
                              PcdTokenEx (&gH2ODebugAssistTokenSpaceGuid, PcdH2ODebugAssistEvtTraceList));
    NumberPtr = &mNumberOfWhiteList;
    TargetListPtr = &mWhiteList;
  } else if (TraceMethod == EvtTraceMethodNotSpecificDrvs) {

    DEBUG ((DEBUG_INFO, "[EventTraceDxe] Trace not specific drivers:\n"));

    PcdSize = LibPcdGetExSize (&gH2ODebugAssistTokenSpaceGuid,
                              PcdTokenEx (&gH2ODebugAssistTokenSpaceGuid, PcdH2ODebugAssistEvtNotTraceList));
    NumberPtr = &mNumberOfBlackList;
    TargetListPtr = &mBlackList;
  }

  //
  if (PcdSize > sizeof (GUID)) {
    Buffer = AllocateZeroPool (PcdSize);
    if (Buffer != NULL) {
      CopyMem (Buffer, PcdGetExPtr (&gH2ODebugAssistTokenSpaceGuid, PcdH2ODebugAssistEvtTraceList), PcdSize);

      for (Index = 0, StrPtr = (CHAR8 *) Buffer;
          (UINTN) StrPtr < ((UINTN) Buffer + PcdSize);
          Index ++) {
        StrPtr = (CHAR8 *) ((UINTN) StrPtr + AsciiStrSize (StrPtr) + sizeof (GUID));
      }

      *NumberPtr = Index;
      *TargetListPtr = (EVENT_TRACE_DXE_TARGET_INFO *) AllocateZeroPool (
                                                        (*NumberPtr * sizeof (EVENT_TRACE_DXE_TARGET_INFO)));
//[-start-191009-IB05820475-modify]//
      if (*TargetListPtr != NULL) {
        for (Index = 0, StrPtr = (CHAR8 *) Buffer; Index < *NumberPtr; Index ++) {
          (*TargetListPtr)[Index].FileName = StrPtr;
          (*TargetListPtr)[Index].FileGuid = (GUID *) ((UINTN) StrPtr + AsciiStrSize (StrPtr));

          DEBUG ((DEBUG_INFO, "[EventTraceDxe] %a, %g\n", StrPtr, (*TargetListPtr)[Index].FileGuid));

          StrPtr = (CHAR8 *) ((UINTN) (*TargetListPtr)[Index].FileGuid + sizeof (GUID));
        }
      }
//[-end-191009-IB05820475-modify]//
    }
  }

  //
  Status = gBS->OpenProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **) &Img,
                  ImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!RETURN_ERROR (Status)) {
    Img->Unload = EventTraceDxeUnload;
  }

  //
  mDxeCoreCreateEvent   = gBS->CreateEvent;
  mDxeCoreCreateEventEx = gBS->CreateEventEx;
  mDxeCoreCloseEvent    = gBS->CloseEvent;
  gBS->CreateEvent      = EventTraceDxeCreateEvent;
  gBS->CreateEventEx    = EventTraceDxeCreateEventEx;
  gBS->CloseEvent       = EventTraceDxeCloseEvent;

  return RETURN_SUCCESS;
}

