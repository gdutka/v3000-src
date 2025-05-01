/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/RingDescriptor.h>
#include <AmdUsb4/HostInterface.h>
#include <AmdUsb4/ControlPacket.h>
#include <Library/AmdUsb4Lib.h>

#define FILECODE LIBRARY_AMDUSB4LIB_CORE_FILECODE

VOID
AmdUsb4Ring0PushHotPlugInfoQueue (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         UPG
  )
{
  EFI_STATUS        Status;
  UINT32            i;
  BOOLEAN           IsFind;
  AMD_USB4_CM_CELL  *PtrCell;

  Status  = EFI_SUCCESS;
  i       = 0;
  IsFind  = FALSE;
  PtrCell = NULL;

  if ( UPG != UPG_HOT_PLUG ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Only record hot plug in event.\n");
    return;
  }

  for (i = 0; i < MAX_HP_QUEUE_SIZE; i++) {
    if ( pCM->HotPlugQueue[0][i].Valid          == 1
      && pCM->HotPlugQueue[0][i].TopologyIDLow  == RouteStringLow
      && pCM->HotPlugQueue[0][i].TopologyIDHigh == MASK_CM_BIT (RouteStringHigh)
      && pCM->HotPlugQueue[0][i].AdapterNumber  == AdapterNum
      && pCM->HotPlugQueue[0][i].UPG            == UPG )
    {
      IsFind = TRUE;
      pCM->HotPlugQueue[0][i].PMTimer = GetACPITimer ();
      break;
    }
  }

  if ( !IsFind ) {
      // Add Hot Plug event to Queue
      for (i = 0; i < MAX_HP_QUEUE_SIZE; i++) {
        if ( pCM->HotPlugQueue[0][i].Valid == 0 ) {
          pCM->HotPlugQueue[0][i].Valid          = 1;
          pCM->HotPlugQueue[0][i].TopologyIDLow  = RouteStringLow;
          pCM->HotPlugQueue[0][i].TopologyIDHigh = MASK_CM_BIT (RouteStringHigh);
          pCM->HotPlugQueue[0][i].AdapterNumber  = AdapterNum;
          pCM->HotPlugQueue[0][i].UPG            = UPG;
          pCM->HotPlugQueue[0][i].PMTimer        = GetACPITimer ();
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "Record %d RouterHigh 0x%x RouterLow 0x%x AdapterNum %d PMTimer=0x%x.\n",
            i,
            MASK_CM_BIT (RouteStringHigh),
            RouteStringLow,
            AdapterNum,
            pCM->HotPlugQueue[0][i].PMTimer
            );
          break;
        }
      }
  }
}


VOID
AmdUsb4Ring0ClearHotPlugInfoQueue (
  IN     AMD_USB4_CM    *pCM
  )
{
  UINT32  i;

  for (i = 0; i < MAX_HP_QUEUE_SIZE; i++) {
    pCM->HotPlugQueue[0][i].Valid = 0;
  }
}


BOOLEAN
AmdUsb4Ring0FindHotPlugEvent (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         UPG
  )
{
  UINT32  i;

  for (i = 0; i < MAX_HP_QUEUE_SIZE; i++) {
    if ( pCM->HotPlugQueue[0][i].TopologyIDLow  == RouteStringLow
      && pCM->HotPlugQueue[0][i].AdapterNumber  == AdapterNum
      && pCM->HotPlugQueue[0][i].UPG            == UPG
      && pCM->HotPlugQueue[0][i].Valid          == 1
      && pCM->HotPlugQueue[0][i].TopologyIDHigh == MASK_CM_BIT (RouteStringHigh) ) {
      return TRUE;
    }
  }

  return FALSE;
}


VOID
AmdUsb4Ring0PushNotificationQueue (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         EventCode
  )
{
  // TODO
  return;
}


EFI_STATUS
AmdUsb4FindRouter (
  IN     AMD_USB4_CM      *pCM,
  IN     UINT32           RouteStringHigh,
  IN     UINT32           RouteStringLow,
     OUT AMD_USB4_CM_CELL **PtrCell
  )
{
  EFI_STATUS       Status;
  UINT32           i;
  AMD_USB4_CM_CELL *pCell;

  Status = EFI_NOT_FOUND;
  i      = 0;
  pCell  = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);
  IDS_HDT_CONSOLE_USB4_DEBUG ("CmDataIndex = %d\n", pCM->CmDataIndex);

  *PtrCell = NULL;

  for (i=0; i<pCM->CmDataIndex; i++) {
    pCell = &(pCM->PtrCmData[i]);
    if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "[%d] RouteStringHigh = 0x%x, RouteStringLow = 0x%x, "
        "TopologyID_High = 0x%x, TopologyID_Low = 0x%x, CellType = 0x%x\n",
        i,
        RouteStringHigh,
        RouteStringLow,
        (UINT32)((pCell->Header.TopologyID)>>32),
        (UINT32)(pCell->Header.TopologyID),
        pCell->Header.CellType
        );
    }

    if ( MASK_CM_BIT ((UINT32)((pCell->Header.TopologyID)>>32)) == MASK_CM_BIT (RouteStringHigh)
      && ((UINT32)(pCell->Header.TopologyID)) == RouteStringLow
      && ( pCell->Header.CellType == CELL_TYPE_HOST_ROUTER || pCell->Header.CellType == CELL_TYPE_DEVICE_ROUTER ) )
    {
      *PtrCell = pCell;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Find Router.\n");
      Status = EFI_SUCCESS;
      break;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return Status;
}


/**
 * @brief   Find Adapter by Toplogy ID and Adapter Number
 *
 * @param   AMD_USB4_CM          *pCM
 * @param   UINT32                RouteStringHigh
 * @param   UINT32                RouteStringLow
 * @param   UINT32                AdapterNum
 * @param   AMD_USB4_CM_CELL      **PtrCell
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
AmdUsb4FindAdapterByAdpNum (
  IN     AMD_USB4_CM      *pCM,
  IN     UINT32           RouteStringHigh,
  IN     UINT32           RouteStringLow,
  IN     UINT32           AdapterNum,
     OUT AMD_USB4_CM_CELL **PtrCell
  )
{
  EFI_STATUS       Status;
  UINT32           i;
  AMD_USB4_CM_CELL *pCell;

  Status = EFI_NOT_FOUND;
  i      = 0;
  pCell  = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  *PtrCell = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pCM->CmDataIndex = %d, find RouterHigh = 0x%x, RouterLow = 0x%x, AdapterNum = %d\n",
    pCM->CmDataIndex,
    RouteStringHigh,
    RouteStringLow,
    AdapterNum
    );
  for (i=0; i<pCM->CmDataIndex; i++) {
    pCell = &(pCM->PtrCmData[i]);
    if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "[%d] TopologyID = 0x%lx, AdapterNum = %d\n",
        i,
        pCell->Header.TopologyID,
        pCell->Header.AdapterNum
        );
    }
    if ( MASK_CM_BIT ((UINT32)((pCell->Header.TopologyID)>>32)) == MASK_CM_BIT (RouteStringHigh)
      && ((UINT32)(pCell->Header.TopologyID)) == RouteStringLow
      && pCell->Header.AdapterNum == AdapterNum )
    {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Find cell\n");
      *PtrCell = pCell;
      Status = EFI_SUCCESS;
      break;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return Status;
}


/**
 * @brief   Find Adapter by Toplogy ID and Cell Type
 *
 * @param   AMD_USB4_CM          *pCM
 * @param   UINT32                RouteStringHigh
 * @param   UINT32                RouteStringLow
 * @param   UINT32                AdapterNum
 * @param   AMD_USB4_CM_CELL      **PtrCell
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
AmdUsb4FindAdapterByCellType (
  IN     AMD_USB4_CM            *pCM,
  IN     UINT32                 RouteStringHigh,
  IN     UINT32                 RouteStringLow,
  IN     AMD_USB4_CM_CELL_TYPE  CellType,
     OUT AMD_USB4_CM_CELL       **PtrCell
  )
{
  EFI_STATUS       Status;
  UINT32           i;
  AMD_USB4_CM_CELL *pCell;

  Status = EFI_NOT_FOUND;
  i      = 0;
  pCell  = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  *PtrCell = NULL;

  for (i=0; i<pCM->CmDataIndex; i++) {
    pCell = &(pCM->PtrCmData[i]);
    if ( MASK_CM_BIT ((UINT32)((pCell->Header.TopologyID)>>32)) == MASK_CM_BIT (RouteStringHigh)
      && ((UINT32)(pCell->Header.TopologyID)) == RouteStringLow
      && pCell->Header.CellType == CellType ) {
      *PtrCell = pCell;
      Status = EFI_SUCCESS;
      break;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return Status;
}


EFI_STATUS
AmdUsb4Ring0HotPlugAck (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         UPG
)
{
  EFI_STATUS                           Status;
  AMD_USB4_NOTIFICATION_PACKET_PAYLOAD Hpa;
  //AMD_USB4_CM_CELL                     *PtrCellAdapter;
  //AMD_USB4_CM_CELL                     *PtrCellRouter;

  Status          = EFI_SUCCESS;
  //PtrCellAdapter  = NULL;
  //PtrCellRouter   = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  /*
  Status = AmdUsb4FindRouter (
             pCM,
             RouteStringHigh,
             RouteStringLow,
             &PtrCellRouter
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("  Cannot find Router.\n");
    return Status;
  }

  if ( !IsUSB4Router ((AMD_USB4_CM_ROUTER*)PtrCellRouter)
    && ( UPG_HOT_UNPLUG == UPG ) ) {
    // A Connection Manager should be able to handle Unplug Events. A Connection
    // Manager shall not send Control Packets to any unplugged Routers within 800 ms
    // after the Unplug Event.
    // Since Pre-OS CM do not enable interrupt, we skip hot unplug ack for TBT3.
    IDS_HDT_CONSOLE_USB4_DEBUG ("  Do not handle unplug event for TBT3.\n");
    return EFI_SUCCESS;
  }
  */

  SetMem (&Hpa, sizeof (AMD_USB4_NOTIFICATION_PACKET_PAYLOAD), 0);

  Hpa.EventCode       = HP_ACK;
  Hpa.RouteStringHigh = RouteStringHigh;
  Hpa.RouteStringLow  = RouteStringLow;
  Hpa.AdapterNum      = AdapterNum;
  Hpa.PG              = ( UPG == UPG_HOT_UNPLUG ) ? PG_HOT_UNPLUG_EVENT: PG_HOT_PLUG_EVENT;

  Hpa.CRC = Usb4InternalCalculateCrc32 (
              (UINT8*)&Hpa,
              GET_SIZE_FOR_CRC32 (AMD_USB4_NOTIFICATION_PACKET_PAYLOAD)
              );

  Status = AmdUsb4HostInterfaceRing0Tx (
             pCM,
             PDF_NOTIFICATION,    // Notification Packet
             sizeof (AMD_USB4_NOTIFICATION_PACKET_PAYLOAD),
             (UINT32*)&Hpa
             );
  IDS_HDT_CONSOLE_USB4_INFO (
    "send out CONTROL_PACKET Hot Plug ACK       RouteStringHigh=0x%08x RouteStringLow=0x%08x AdapterNum=0x%x UPG=0x%x Status=%r PMTimer=0x%08x\n",
    RouteStringHigh,
    RouteStringLow,
    AdapterNum,
    UPG,
    Status,
    GetACPITimer ()
    );

#if AMD_USB4_DEBUG_POSTCODE
  AGESA_TESTPOINT (
    USB4_DEBUG_PC_HOTPLUG,
    NULL
    );
  AGESA_TESTPOINT (
    Hpa.RouteStringHigh,
    NULL
    );
  AGESA_TESTPOINT (
    Hpa.RouteStringLow,
    NULL
    );
  AGESA_TESTPOINT (
    Hpa.AdapterNum | (Hpa.PG << 16),
    NULL
    );
#endif

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Clear Receive Ring control packets.
 *
 * @param[in] *pCM             Point to global structure AMD_USB4_CM
 * @param[in] TopologyID       Route String.

 * @retval VOID                No Return.
**/

VOID
AmdUsb4ClearReceiveRing (
  IN     AMD_USB4_CM    *pCM
  )
{
  EFI_STATUS    Status;
  UINT32        ReceivedPDF;
  UINT32        ReceivedSize;
  UINT32        RemainPacket;
  UINT32        *pControlPacketRxBuffer;
  USB4_TIMEOUT  Timeout;
  INT32         TimeoutPacketNum;

  AMD_USB4_READ_REQUEST_PAYLOAD  *pReadRequestPacket;

  Status                  = EFI_SUCCESS;
  pReadRequestPacket      = NULL;
  pControlPacketRxBuffer  = NULL;
  TimeoutPacketNum        = (INT32)PcdGet32 (PcdAmdUsb4TimeoutPacketNumber);
  ReceivedPDF             = 0;
  ReceivedSize            = 0;
  RemainPacket            = 0;
  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  StartTimeout (&Timeout, CONTROL_PACKET_RESPONSE_TIMEOUT);

  // 1. Try to get READ_RESPONSE packet.
  pControlPacketRxBuffer = pCM->PtrControlPacketClearBuffer;
  IDS_HDT_CONSOLE_USB4_ASSERT (pControlPacketRxBuffer!=NULL);

  do {
    if ( TimeoutPacketNum >= 0 ) {
      TimeoutPacketNum--;
      IDS_HDT_CONSOLE_USB4_DEBUG (
        " TimeoutPacketNum = %d\n",
        TimeoutPacketNum
        );
    }

    SetMem (pControlPacketRxBuffer, MAX_CONTROL_PACKET_SIZE, 0);

    Status = AmdUsb4HostInterfaceRing0Rx (
               pCM,
               &ReceivedPDF,
               &ReceivedSize,
               pControlPacketRxBuffer,
               &RemainPacket
               );
    if ( EFI_DEVICE_ERROR == Status ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to Read receive ring.\n");
      break;
    }

    if ( EFI_NOT_FOUND == Status ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Receive ring is empty.\n");
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("   Received PDF: %d.\n", ReceivedPDF);

    if ( ReceivedPDF == 1 ) { // Read Response
      IDS_HDT_CONSOLE_USB4_INFO (
        "[ClearReceiveRing] Get Read Response RouteStringHigh=0x%x RouteStringLow=0x%x SN=0x%x CS=0x%x"
        " AdpNum=0x%x Address=0x%x Size=0x%x PMTimer=0x%08x\n",
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.SequenceNumber,
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.ConfigurationSpace,
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.AdapterNum,
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Address,
        ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Size,
        GetACPITimer ()
        );
    } else if ( ReceivedPDF == 2 ) { // Write Response
      // Output debug info for unknown Write Response
      IDS_HDT_CONSOLE_USB4_INFO (
        "[ClearReceiveRing] Get Write Response RouteStringHigh=0x%x RouteStringLow=0x%x SN=0x%x CS=0x%x"
        " AdpNum=0x%x Address=0x%x Size=0x%x PMTimer=0x%08x\n",
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.SequenceNumber,
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.ConfigurationSpace,
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.AdapterNum,
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Address,
        ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Size,
        GetACPITimer ()
        );
    } else if ( ReceivedPDF == 3 ) { // Notification
      IDS_HDT_CONSOLE_USB4_INFO (
        "[ClearReceiveRing] Get Notification Packet RouteStringHigh=0x%x RouteStringLow=0x%x EventCode=0x%x AdapterNum=0x%x PG=0x%x PMTimer=0x%08x\n",
        ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
        ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
        ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode,
        ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
        ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->PG,
        GetACPITimer ()
        );
    } else if ( ReceivedPDF == 5 ) { // Hot plug
      IDS_HDT_CONSOLE_USB4_INFO (
        "[ClearReceiveRing] Get Hot Plug Packet RouteStringHigh=0x%x RouteStringLow=0x%x AdapterNum=0x%x UPG=0x%x (%a) PMTimer=0x%08x\n",
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG,
        (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG) ? "Hot Unplug Event" : "Hot Plug Event",
        GetACPITimer ()
        );

      AmdUsb4Ring0HotPlugAck (
        pCM,
        MASK_CM_BIT (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh),
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG
        );

      AmdUsb4Ring0PushHotPlugInfoQueue (
        pCM,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
        ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG
        );

    } else {
      // if other ReceivedPdf, log debug info
      IDS_HDT_CONSOLE_USB4_ERROR (
        "Receive unsupported Control Packet!. PDF=0x%x, Size=0x%x.\n",
        ReceivedPDF,
        ReceivedSize
        );
      Usb4DumpData (pControlPacketRxBuffer, ReceivedSize);
    }
  }  while ( ( HasItTimedOut (&Timeout) != EFI_TIMEOUT ) || ( TimeoutPacketNum >= 0 ) );

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Read Configuration Space Register
 *
 * @param[in] *pCM             Point to global structure AMD_USB4_CM
 * @param[in] TopologyID       Route String.
 * @param[in] CsType           Configuration Space want to read.
 *                             0 - Path CS; 1 - Adapter CS; 2 - Router CS; 3 - Counters CS;
 * @param[in] AdapterNum       Adapter Number want to read
 * @param[in] ReadAddress      Offset of the CS want to read
 * @param[in] ReadSize         Number of double words want to read
 *
 * @param[out] **DataOut       Pointer of the pointer to the data buffer with read data.
 * @param[out] *DataSize       Number of double words of the data.
 *
 * @retval EFI_SUCCESS           Success with data out.
 *                               Data is in **DataOut and *DataSize has the actual read size
 * @retval EFI_DEVICE_ERROR      Failed with data out.
 *                               Router return Error Event code. *DataSize will have the Event code.
 * @retval EFI_NOT_READY         Failed.  Something wrong in Transmit or Receive routine.
 * @retval EFI_NO_RESPONSE       Failed   Send out READ_REQUEST packet,but do not get
 *                                        paired READ_RESPONSE or NOTIFICATION.
 * @retval EFI_INVALID_PARAMETER Failed   Input parameter is not good.
**/

EFI_STATUS
AmdUsb4Ring0ReadCS (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         ReadAddress,
  IN     UINT32         ReadSize,
     OUT UINT32         **DataOut,
     OUT UINT32         *DataSize
  )
{
  EFI_STATUS    Status;
  UINT32        ReceivedPDF;
  UINT32        ReceivedSize;
  UINT32        RemainPacket;
  UINT32        *pControlPacketRxBuffer;
  USB4_TIMEOUT  Timeout;
  INT32         TimeoutPacketNum;
  UINT32        AdapterNumber;
  UINT32        i;

  AMD_USB4_READ_REQUEST_PAYLOAD  *pReadRequestPacket;

  Status                  = EFI_SUCCESS;
  pReadRequestPacket      = NULL;
  pControlPacketRxBuffer  = NULL;
  TimeoutPacketNum        = (INT32)PcdGet32 (PcdAmdUsb4TimeoutPacketNumber);
  i                       = 0;
  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));

  IDS_HDT_CONSOLE_USB4_DEBUG ("\n******************** %a Start ********************\n", __FUNCTION__);

  if ( NULL == pCM->PtrControlPacketTxBuffer || NULL == pCM->PtrControlPacketRxBuffer ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " No Pre-allocated memory Buffer. (pCM->pControlPacketTxBuffer or pCM->pControlPacketRxBuffer)\n"
      );
    return EFI_INVALID_PARAMETER;
  }

  if ( ReadSize > MAX_CONTROL_PACKET_PAYLOAD_SIZE_IN_DW || 0 == ReadSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("The Read Size field shall be greater than 0 and less than or equal to 60.\n");
    return EFI_INVALID_PARAMETER;
  }

  AmdUsb4ClearReceiveRing (pCM);

  // 1. Build up Read Request Control Packet Payload
  pReadRequestPacket = (AMD_USB4_READ_REQUEST_PAYLOAD*)pCM->PtrControlPacketTxBuffer;
  SetMem (pReadRequestPacket, MAX_CONTROL_PACKET_SIZE, 0);

  pReadRequestPacket->RouteStringLow    = (UINT32)TopologyID;
  pReadRequestPacket->RouteStringHigh   = (UINT32)(TopologyID >> 32) & 0xFFFFFF & (~BIT31);

  AdapterNumber = ( CsType == CS_TARGET_ROUTER ) ? 0 : AdapterNum;

  pReadRequestPacket->ReadRequest.Field.ConfigurationSpace  = CsType;
  pReadRequestPacket->ReadRequest.Field.AdapterNum          = AdapterNumber;
  pReadRequestPacket->ReadRequest.Field.Address             = ReadAddress;
  pReadRequestPacket->ReadRequest.Field.Size                = ReadSize;
  pReadRequestPacket->ReadRequest.Field.SequenceNumber      = CONTROL_PACKET_DEFAULT_SN;

  pReadRequestPacket->CRC = Usb4InternalCalculateCrc32 (
                              (UINT8*)pReadRequestPacket,
                              GET_SIZE_FOR_CRC32 (AMD_USB4_READ_REQUEST_PAYLOAD)
                              );

  // 2. Send out READ_REQUEST packet.
  IDS_HDT_CONSOLE_USB4_INFO (
    "send out CONTROL_PACKET READ_REQUEST   >>> RouteString=0x%016lx PDF=0x%x CS=0x%x"
    " AdpNum=0x%02x Address=0x%04x Size=0x%02x SN=0x%x PMTimer=0x%08x\n",
    TOPOLOGICID64 (                        \
      pReadRequestPacket->RouteStringHigh, \
      pReadRequestPacket->RouteStringLow   \
      ),
    1,
    pReadRequestPacket->ReadRequest.Field.ConfigurationSpace,
    pReadRequestPacket->ReadRequest.Field.AdapterNum,
    pReadRequestPacket->ReadRequest.Field.Address,
    pReadRequestPacket->ReadRequest.Field.Size,
    pReadRequestPacket->ReadRequest.Field.SequenceNumber,
    GetACPITimer ()
    );

#if AMD_USB4_DEBUG_POSTCODE
  AGESA_TESTPOINT (
    USB4_DEBUG_PC_READREQUEST,
    NULL
    );
  AGESA_TESTPOINT (
    pReadRequestPacket->RouteStringHigh,
    NULL
    );
  AGESA_TESTPOINT (
    pReadRequestPacket->RouteStringLow,
    NULL
    );
  AGESA_TESTPOINT (
    pReadRequestPacket->ReadRequest.Value,
    NULL
    );
#endif

  Status = AmdUsb4HostInterfaceRing0Tx (
             pCM,
             1,          // PDF of ReadRequest
             sizeof (AMD_USB4_READ_REQUEST_PAYLOAD),
             (UINT32*)pReadRequestPacket
             );

  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write transmit ring.\n");
    return EFI_NOT_READY;
  }

  StartTimeout (&Timeout, CONTROL_PACKET_RESPONSE_TIMEOUT);

  // 3. Try to get READ_RESPONSE packet.
  pControlPacketRxBuffer = pCM->PtrControlPacketRxBuffer;
  do {
    SetMem (pControlPacketRxBuffer, MAX_CONTROL_PACKET_SIZE, 0);

    Status = AmdUsb4HostInterfaceRing0Rx (
               pCM,
               &ReceivedPDF,
               &ReceivedSize,
               pControlPacketRxBuffer,
               &RemainPacket
               );
    if ( EFI_DEVICE_ERROR == Status ) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to Read receive ring.\n");
      return EFI_NOT_READY;
    } else if ( EFI_NOT_FOUND == Status ) {
      IDS_HDT_CONSOLE_USB4_INFO (" receive ring is empty.\n");
    } else if ( EFI_SUCCESS == Status ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Received PDF: %d.\n", ReceivedPDF);

      if ( ReceivedPDF == 1 ) { // Read Response
        // 1) Check SN, CS, AdapterNum, ReadAddress match input
        //    a) if not match, output debug info and continue for next Rx Packet
        // 2) if (ReadResponse.Size != RegisterSize) Log but still proceed to next step
        // 3) *ReadSize = ReceivedSize
        // 4) Read Data in pControlPacketRxBuffer
        // 5) Status = EFI_SUCCESS, , Return = TRUE, go to step 11

        // Compare TopologyID, SN, CS, AdapterNum, Address
        if ( (MASK_CM_BIT ((UINT32)(TopologyID>>32)) ==
                MASK_CM_BIT (((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh))
          && (((UINT32)TopologyID) == ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow) )
        {
          if ( (CONTROL_PACKET_DEFAULT_SN  ==
                  ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.SequenceNumber)
            && (CsType ==
                  ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.ConfigurationSpace)
            && (ReadAddress ==
                  ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Address)
            && (AdapterNumber ==
                  ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.AdapterNum
                  || CsType == CS_TARGET_ROUTER) )

          {
            if ( ReadSize != ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Size ) {
              IDS_HDT_CONSOLE_USB4_WARNING ("Get Read Response, but read size is not match\n");
            }
            *DataOut = &(((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadDataAndCRC[0]);
            *DataSize = ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Size;
            IDS_HDT_CONSOLE_USB4_DEBUG ("Get read Response, size is 0x%x. we are good\n", *DataSize);
            IDS_HDT_CONSOLE_USB4_DEBUG ("******************** %a End ********************\n\n", __FUNCTION__);
            IDS_HDT_CONSOLE_USB4_INFO (
              "send out CONTROL_PACKET READ_RESPONSE  <<< RouteString=0x%016lx         CS=0x%x"
              " AdpNum=0x%02x Address=0x%04x Size=0x%02x SN=0x%x PMTimer=0x%08x",
              TOPOLOGICID64 (                                                               \
                ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh, \
                ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow   \
                ),
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.ConfigurationSpace,
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.AdapterNum,
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Address,
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Size,
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.SequenceNumber,
              GetACPITimer ()
              );
#if AMD_USB4_DEBUG_POSTCODE
            AGESA_TESTPOINT (
              USB4_DEBUG_PC_READRESPONSE,
              NULL
              );
            AGESA_TESTPOINT (
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
              NULL
              );
            AGESA_TESTPOINT (
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
              NULL
              );
            AGESA_TESTPOINT (
              ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Value,
              NULL
              );
#endif
            if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
              IDS_HDT_CONSOLE_USB4_PRINT (" Data: ");
              for (i = 0; i<*DataSize; i++) {
                IDS_HDT_CONSOLE_USB4_PRINT (" [%02d, 0x%08x]", i, (*DataOut)[i]);
              }
              IDS_HDT_CONSOLE_USB4_PRINT ("\n");
              Usb4DumpData (*DataOut, (*DataSize)<<2);
            } else {
              IDS_HDT_CONSOLE_USB4_PRINT ("\n");
            }

            return EFI_SUCCESS;
          } else {
            IDS_HDT_CONSOLE_USB4_WARNING ("Get Read Response, but SN or CS or AptNum or Address is not match\n");
          }
        } else {
          IDS_HDT_CONSOLE_USB4_WARNING (
            "Get a someone else's Read Response. RouteStringHigh=0x%x RouteStringLow=0x%x\n",
            ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
            ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow
            );
        }
      } else if ( ReceivedPDF == 2 ) { // Write Response
        // Output debug info for unknown Write Response
        IDS_HDT_CONSOLE_USB4_WARNING (
          "Read Request should not get Write Response. RouteStringHigh=0x%x RouteStringLow=0x%x"
          " Address=0x%x WriteSize=0x%x AdapterNum=0x%x CS=0x%x SN=0x%x\n",
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Address,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Size,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.AdapterNum,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.ConfigurationSpace,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.SequenceNumber
          );
      } else if ( ReceivedPDF == 3 ) { // Notification
        IDS_HDT_CONSOLE_USB4_INFO (
          "Get Notification Packet RouteStringHigh=0x%x RouteStringLow=0x%x EventCode=0x%x AdapterNum=0x%x PG=0x%x PMTimer=0x%08x\n",
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->PG,
          GetACPITimer ()
          );

        // 1) if RouteString and AdapterNumber match input
        //    1.a) Status = EFI_DEVICE_ERROR, Return = TRUE
        //    1.b) Notification Payload is in pControlPacketRxBuffer
        //    1.c) should be only ERR_ADDR/ERR_CONN/ERR_NUA/ERR_ENUM
        //    1.d) go to step 11 to return and to be handled by calling side
        // 2) else if RouteString and AdapterNumber not match
        //    2.a) Save Notification data into pCM->PendingNotificationQueue
        //    2.b) go to step 11 for next incoming Control Packet
        if ( (MASK_CM_BIT ((UINT32)(TopologyID>>32)) ==
                 MASK_CM_BIT ( ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh) )
          && (((UINT32)TopologyID) == ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow)
          && (AdapterNumber == ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum) )
        {
          switch (((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode) {
            case ERR_ADDR:
            case ERR_CONN:
            case ERR_NUA:
            case ERR_ENUM:
              break;
            default:
              // TODO with ERR_LINK/ERR_HEC/ERR_FC/ERR_PLUG
              break;
          }
          IDS_HDT_CONSOLE_USB4_ERROR (
            "Read Request failed. EventCode=0x%x\n",
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode
            );
          *DataSize = ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode;
          return EFI_DEVICE_ERROR;
        } else {
          AmdUsb4Ring0PushNotificationQueue (
            pCM,
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode
            );
          return EFI_DEVICE_ERROR;
        }
      } else if ( ReceivedPDF == 5 ) { // Hot plug
        IDS_HDT_CONSOLE_USB4_INFO (
          "Get Hot Plug Packet RouteStringHigh=0x%x RouteStringLow=0x%x AdapterNum=0x%x UPG=0x%x (%a) PMTimer=0x%08x\n",
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG,
          (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG) ? "Hot Unplug Event" : "Hot Plug Event",
          GetACPITimer ()
          );

        // 1) Send HotPlug Acknowledge Notification
        AmdUsb4Ring0HotPlugAck (
          pCM,
          MASK_CM_BIT (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh),
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG
          );

        // 2) Save Hot Plug information in HotPlugInfoQueue
        AmdUsb4Ring0PushHotPlugInfoQueue (
          pCM,
          MASK_CM_BIT (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh),
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG
          );
      } else {
        // if other ReceivedPdf, log debug info
        IDS_HDT_CONSOLE_USB4_ERROR (
          "Receive unsupported Control Packet! PDF=0x%x Size=0x%x PMTimer=0x%08x\n",
          ReceivedPDF,
          ReceivedSize,
          GetACPITimer ()
          );
        Usb4DumpData (pControlPacketRxBuffer, ReceivedSize);
      }
    } else {
      IDS_HDT_CONSOLE_USB4_ERROR ("Invalid Path\n");
      IDS_HDT_CONSOLE_USB4_ASSERT (FALSE);
    }

    TimeoutPacketNum--;
    IDS_HDT_CONSOLE_USB4_DEBUG (" TimeoutPacketNum = %d\n", TimeoutPacketNum);

  }  while ( ( HasItTimedOut (&Timeout) != EFI_TIMEOUT ) || ( TimeoutPacketNum > 0 ) );

  IDS_HDT_CONSOLE_USB4_ERROR (" Timeout and do not get response.\n");
  return EFI_NO_RESPONSE;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Write Configuration Space Register
 *
 * @param[in]      *pCM             Point to global structure AMD_USB4_CM
 * @param[in]      TopologyID       Route String.
 * @param[in]      CsType           Configuration Space want to read.
 *                                  0 - Path CS; 1 - Adapter CS; 2 - Router CS; 3 - Counters CS;
 * @param[in]      AdapterNum       Adapter Number want to read
 * @param[in]      RegisterAddress  Offset of the CS want to read
 * @param[in][out] *WriteDataSize   Number of double words want to write
 * @param[in][out] *WriteData       Point to data buffer need to be written.
 *
 * @retval EFI_SUCCESS           Success. *WriteDataSize will have the actual write size (double words).
 * @retval EFI_DEVICE_ERROR      Failed.  Router return Error Event code.
 *                                        WriteData[0] will have the Event code.
 *
 * @retval EFI_NOT_READY         Failed.  Something wrong in Transmit or Receive routine.
 * @retval EFI_NO_RESPONSE       Failed   Send out WRITE_REQUEST packet, but do not get
 *                                        paired WRITE_RESPONSE or NOTIFICATION.
 * @retval EFI_INVALID_PARAMETER Failed   Input parameter is not good.
**/

EFI_STATUS
AmdUsb4Ring0WriteCS (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         RegisterAddress,
  IN     UINT32         *WriteDataSize,
  IN     UINT32         *WriteData
  )
{
  EFI_STATUS    Status;
  UINT32        ReceivedPDF;
  UINT32        ReceivedSize;
  UINT32        RemainPacket;
  UINT32        ActualWriteRequestSizeinDW;
  UINT32        *pControlPacketRxBuffer;
  USB4_TIMEOUT  Timeout;
  INT32         TimeoutPacketNum;
  UINT32        AdapterNumber;
  UINT32        i;

  AMD_USB4_WRITE_REQUEST_PAYLOAD  *pWriteRequestPacket;

  Status                  = EFI_SUCCESS;
  pWriteRequestPacket     = NULL;
  pControlPacketRxBuffer  = NULL;
  TimeoutPacketNum        = (INT32)PcdGet32 (PcdAmdUsb4TimeoutPacketNumber);
  i                       = 0;
  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));

  IDS_HDT_CONSOLE_USB4_DEBUG ("\n******************** %a Start ********************\n", __FUNCTION__);

  if ( NULL == pCM->PtrControlPacketTxBuffer || NULL == pCM->PtrControlPacketRxBuffer ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " No Pre-allocated memory Buffer. (pCM->pControlPacketTxBuffer or pCM->pControlPacketRxBuffer)\n"
      );
    return EFI_INVALID_PARAMETER;
  }

  if ( *WriteDataSize > MAX_CONTROL_PACKET_PAYLOAD_SIZE_IN_DW || 0 == *WriteDataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" The Write Size field shall be greater than 0 and less than or equal to 60.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( NULL == WriteData ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Input parameter Writedata pointer is 0.\n");
    return EFI_INVALID_PARAMETER;
  }

  AmdUsb4ClearReceiveRing (pCM);

  // 1. Build up Write Request Control Packet Payload
  pWriteRequestPacket = (AMD_USB4_WRITE_REQUEST_PAYLOAD*)pCM->PtrControlPacketTxBuffer;
  SetMem (pWriteRequestPacket, MAX_CONTROL_PACKET_SIZE, 0);

  ActualWriteRequestSizeinDW =
    *WriteDataSize +
    CONTROL_PACKET_CRC_SIZE_IN_DW +
    (( sizeof (AMD_USB4_WRITE_REQUEST_PAYLOAD) -
        sizeof ( ((AMD_USB4_WRITE_REQUEST_PAYLOAD*)0)->WriteDataAndCRC )) /
          sizeof (UINT32));

  pWriteRequestPacket->RouteStringLow    = (UINT32)TopologyID;
  pWriteRequestPacket->RouteStringHigh   = (UINT32)(TopologyID >> 32) & 0xFFFFFF & (~BIT31);

  AdapterNumber = ( CsType == CS_TARGET_ROUTER ) ? 0 : AdapterNum;

  pWriteRequestPacket->WriteRequest.Field.ConfigurationSpace  = CsType;
  pWriteRequestPacket->WriteRequest.Field.AdapterNum          = AdapterNumber;
  pWriteRequestPacket->WriteRequest.Field.Address             = RegisterAddress;
  pWriteRequestPacket->WriteRequest.Field.SequenceNumber      = CONTROL_PACKET_DEFAULT_SN;
  pWriteRequestPacket->WriteRequest.Field.Size                = *WriteDataSize;

  CopyMem (pWriteRequestPacket->WriteDataAndCRC, WriteData, (*WriteDataSize)<<2);

  pWriteRequestPacket->WriteDataAndCRC[*WriteDataSize] =
    Usb4InternalCalculateCrc32 (
      (UINT8*)pWriteRequestPacket,
      ((ActualWriteRequestSizeinDW-CONTROL_PACKET_CRC_SIZE_IN_DW)<<2)
      );

  // 2. Send out WRITE_REQUEST packet.
  IDS_HDT_CONSOLE_USB4_INFO (
    "send out CONTROL_PACKET WRITE_REQUEST  >>> RouteString=0x%016lx PDF=0x%x CS=0x%x"
    " AdpNum=0x%02x Address=0x%04x Size=0x%02x SN=0x%x PMTimer=0x%08x",
    TOPOLOGICID64 (                         \
      pWriteRequestPacket->RouteStringHigh, \
      pWriteRequestPacket->RouteStringLow   \
      ),
    2,
    pWriteRequestPacket->WriteRequest.Field.ConfigurationSpace,
    pWriteRequestPacket->WriteRequest.Field.AdapterNum,
    pWriteRequestPacket->WriteRequest.Field.Address,
    pWriteRequestPacket->WriteRequest.Field.Size,
    pWriteRequestPacket->WriteRequest.Field.SequenceNumber,
    GetACPITimer ()
    );

#if AMD_USB4_DEBUG_POSTCODE
  AGESA_TESTPOINT (
    USB4_DEBUG_PC_WRITEREQUEST,
    NULL
    );
  AGESA_TESTPOINT (
    pWriteRequestPacket->RouteStringHigh,
    NULL
    );
  AGESA_TESTPOINT (
    pWriteRequestPacket->RouteStringLow,
    NULL
    );
  AGESA_TESTPOINT (
    pWriteRequestPacket->WriteRequest.Value,
    NULL
    );
#endif

  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    IDS_HDT_CONSOLE_USB4_PRINT (" Data: ");
    for (i = 0; i<*WriteDataSize; i++) {
      IDS_HDT_CONSOLE_USB4_PRINT (" [%02d, 0x%08x]", i, WriteData[i]);
    }
    IDS_HDT_CONSOLE_USB4_PRINT ("\n");
    Usb4DumpData (&(pWriteRequestPacket->WriteDataAndCRC[0]), (*WriteDataSize)<<2);
  } else {
    IDS_HDT_CONSOLE_USB4_PRINT ("\n");
  }

  Status = AmdUsb4HostInterfaceRing0Tx (
             pCM,
             2,          // PDF of ReadRequest
             ActualWriteRequestSizeinDW<<2,   // Packet Size in bytes
             (UINT32*)pWriteRequestPacket
             );

  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write transmit ring.\n");
    return EFI_NOT_READY;
  }

  StartTimeout (&Timeout, CONTROL_PACKET_RESPONSE_TIMEOUT);  // 6.4.4 Control Packet Reliability 10ms

  // 3. Try to get WRITE_RESPONSE packet.
  pControlPacketRxBuffer = pCM->PtrControlPacketRxBuffer;
  do {
    SetMem (pControlPacketRxBuffer, MAX_CONTROL_PACKET_SIZE, 0);

    Status = AmdUsb4HostInterfaceRing0Rx (
               pCM,
               &ReceivedPDF,
               &ReceivedSize,
               pControlPacketRxBuffer,
               &RemainPacket
               );
    if ( EFI_DEVICE_ERROR == Status ) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to Read receive ring.\n");
      return EFI_NOT_READY;
    } else if ( EFI_NOT_FOUND == Status ) {
      IDS_HDT_CONSOLE_USB4_DEBUG (" receive ring is empty.\n");
    } else if ( EFI_SUCCESS == Status ) {
      if ( ReceivedPDF == 2 ) { // Write Response
        // 1) Check RouteString, AdapterNum, SN, CS, WriteAddress match input
        //    if not match, output debug info and continue to step #11
        // 2) if (WriteResponse.WriteSize != WriteSize) Log but still proceed to next step
        // 3) Status = EFI_SUCCESS, Return = TRUE, go to step #11
        IDS_HDT_CONSOLE_USB4_INFO (
          "send out CONTROL_PACKET WRITE_RESPONSE <<< RouteString=0x%016lx         CS=0x%x"
          " AdpNum=0x%02x Address=0x%04x Size=0x%02x SN=0x%x PMTimer=0x%08x\n",
          TOPOLOGICID64 (                                                                \
            ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh, \
            ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow   \
            ),
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.ConfigurationSpace,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.AdapterNum,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Address,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Size,
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.SequenceNumber,
          GetACPITimer ()
          );
#if AMD_USB4_DEBUG_POSTCODE
        AGESA_TESTPOINT (
          USB4_DEBUG_PC_WRITERESPONSE,
          NULL
          );
        AGESA_TESTPOINT (
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          NULL
          );
        AGESA_TESTPOINT (
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          NULL
          );
        AGESA_TESTPOINT (
          ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Value,
          NULL
          );
#endif
        if ( (MASK_CM_BIT ((UINT32)(TopologyID>>32)) ==
                 MASK_CM_BIT (((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh))
          && (((UINT32)TopologyID)  ==
                ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow)  )
        {
          if ( (CONTROL_PACKET_DEFAULT_SN  ==
                   ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.SequenceNumber)
            && (CsType          ==
                  ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.ConfigurationSpace)
            && (AdapterNumber   ==
                  ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.AdapterNum)
            && (RegisterAddress ==
                  ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Address) )
          {
            if ( *WriteDataSize !=
                   ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Size )
            {
              IDS_HDT_CONSOLE_USB4_WARNING (
                "Get Write Response, but write size is not match. WriteSize=%d, Actual WriteSize=%d.\n",
                *WriteDataSize,
                ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Size
                );
              *WriteDataSize = ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->WriteResponse.Field.Size;
            }
            IDS_HDT_CONSOLE_USB4_DEBUG (" Get Write Response, we are good\n");
            IDS_HDT_CONSOLE_USB4_DEBUG ("******************** %a End ********************\n\n", __FUNCTION__);
            return EFI_SUCCESS;
          } else {
            IDS_HDT_CONSOLE_USB4_WARNING (
              "Get Write Response and RouteStr match, but SN or CS or AptNum or Address is not match\n"
              );
          }
        } else {
          IDS_HDT_CONSOLE_USB4_WARNING (
            "Got someone else's Write Response. RouteStringHigh=0x%x, RouteStringLow=0x%x\n",
            ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
            ((AMD_USB4_WRITE_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow
            );
        }
      } else if ( ReceivedPDF == 1 ) { // Read Response
        // 1)  Output debug info for unknown Read Response
        // 2)  Continue to step 11
        IDS_HDT_CONSOLE_USB4_WARNING (
          "Write Request should not get Read Response. RouteStringHigh=0x%x, RouteStringLow=0x%x,"
          "  Address=0x%x Size=0x%x AdapterNum=0x%x CS=0x%x SN=0x%x PMTimer=0x%08x\n",
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Address,
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.Size,
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.AdapterNum,
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.ConfigurationSpace,
          ((AMD_USB4_READ_RESPONSE_PAYLOAD*)pControlPacketRxBuffer)->ReadResponse.Field.SequenceNumber,
          GetACPITimer ()
          );
      } else if ( ReceivedPDF == 3 ) { // Notification
        IDS_HDT_CONSOLE_USB4_INFO (
          "Get Notification Packet RouteStringHigh=0x%x RouteStringLow=0x%x EventCode=0x%x AdapterNum=0x%x PG=0x%x PMTimer=0x%08x\n",
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->PG,
          GetACPITimer ()
          );
        // 1) if RouteString and AdapterNumber match input
        //    1.a) Status = EFI_DEVICE_ERROR, Return = TRUE
        //    1.b) Notification Payload is in pControlPacketRxBuffer
        //    1.c) should be only ERR_ADDR/ERR_CONN/ERR_NUA/ERR_ENUM
        //    1.d) go to step 11 to return and to be handled by calling side
        // 2) else if RouteString and AdapterNumber not match
        //    2.a) Save Notification data into pCM->PendingNotificationQueue
        //    2.b) go to step 11 for next incoming Control Packet
        if ( (MASK_CM_BIT (pWriteRequestPacket->RouteStringHigh) ==
                 MASK_CM_BIT (((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh))
          && (pWriteRequestPacket->RouteStringLow ==
                ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow)
          && (AdapterNumber ==
                ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum) )
        {
          switch (((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode) {
            case ERR_ADDR:
            case ERR_CONN:
            case ERR_NUA:
            case ERR_ENUM:
              break;
            default:
              // TODO with ERR_LINK/ERR_HEC/ERR_FC/ERR_PLUG
              break;
          }
          IDS_HDT_CONSOLE_USB4_ERROR (
            "Write Request failed. EventCode=0x%x\n",
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode
            );
          *WriteData = ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode;
          return EFI_DEVICE_ERROR;
        } else {
          AmdUsb4Ring0PushNotificationQueue (
            pCM,
            MASK_CM_BIT (((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh),
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
            ((AMD_USB4_NOTIFICATION_PACKET_PAYLOAD*)pControlPacketRxBuffer)->EventCode
            );
          return EFI_DEVICE_ERROR;
        }
      } else if ( ReceivedPDF == 5 ) { // Hot plug
        IDS_HDT_CONSOLE_USB4_INFO (
          "Get Hot Plug Packet RouteStringHigh=0x%x RouteStringLow=0x%x AdapterNum=0x%x UPG=0x%x (%a) PMTimer=0x%08x\n",
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG,
          (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG) ? "Hot Unplug Event" : "Hot Plug Event",
          GetACPITimer ()
          );

        // 1) Send HotPlug Acknowledge Notification
        AmdUsb4Ring0HotPlugAck (
          pCM,
          MASK_CM_BIT (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh),
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG
          );

        // 2) Save Hot Plug information in HotPlugInfoQueue
        AmdUsb4Ring0PushHotPlugInfoQueue (
          pCM,
          MASK_CM_BIT (((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringHigh),
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->RouteStringLow,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->AdapterNum,
          ((AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD*)pControlPacketRxBuffer)->UPG
          );
      } else {
        // if other ReceivedPdf, log debug info
        IDS_HDT_CONSOLE_USB4_ERROR (
          " Receive unsupported Control Packet! PDF=0x%x Size=0x%x PMTimer=0x%08x\n",
          ReceivedPDF,
          ReceivedSize,
          GetACPITimer ()
          );
        Usb4DumpData (pControlPacketRxBuffer, ReceivedSize);
      }
    } else {
      IDS_HDT_CONSOLE_USB4_ERROR ("Invalid Path\n");
      IDS_HDT_CONSOLE_USB4_ASSERT (FALSE);
    }

    TimeoutPacketNum--;
    IDS_HDT_CONSOLE_USB4_DEBUG (" TimeoutPacketNum = %d\n", TimeoutPacketNum);

  }  while ( ( HasItTimedOut (&Timeout) != EFI_TIMEOUT ) || ( TimeoutPacketNum > 0 ) );

  IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Timeout and do not get response.\n");
  return EFI_NO_RESPONSE;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Read Modify Write Configuration Space Register
 *
 * @param[in]      *pCM             Point to global structure AMD_USB4_CM
 * @param[in]      TopologyID       Route String.
 * @param[in]      CsType           Configuration Space want to read. 0 - Path CS; 1 - Adapter CS; 2 - Router CS; 3 - Counters CS;
 * @param[in]      AdapterNum       Adapter Number want to read
 * @param[in]      Address          Offset of the CS want to read
 * @param[in]      SizeInDW         Size in DW
 * @param[in]      *AndMask         And Mask
 * @param[in]      *OrValue         Or value
 *
 * @retval EFI_SUCCESS           Success. *WriteDataSize will have the actual write size (double words).
 * @retval EFI_DEVICE_ERROR      Failed.  Router return Error Event code.
 *                                        WriteData[0] will have the Event code.
 *
 * @retval EFI_NOT_READY         Failed.  Something wrong in Transmit or Receive routine.
 * @retval EFI_NO_RESPONSE       Failed   Send out WRITE_REQUEST packet, but do not get paired WRITE_RESPONSE or NOTIFICATION.
 * @retval EFI_INVALID_PARAMETER Failed   Input parameter is not good.
**/
EFI_STATUS
AmdUsb4Ring0RMWCS (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         Address,
  IN     UINT32         SizeInDW,
  IN     UINT32         *AndMask,
  IN     UINT32         *OrValue
  )
{
  EFI_STATUS                Status;
  UINT32                    i;
  UINT32                    DataSize;
  UINT32                    *DataPtr;

  Status    = EFI_SUCCESS;
  i         = 0;
  DataSize  = 0;
  DataPtr   = NULL;

  if ( NULL == pCM || NULL == AndMask || NULL == OrValue ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Read/Modify/Write Input parameter has NULL pointer.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( SizeInDW > 60 ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Read/Modify/Write Cannot operate size %d\n", SizeInDW);
    return EFI_INVALID_PARAMETER;
  }

  IDS_HDT_CONSOLE_USB4_INFO (
    "Read/Modify/Write TopologyID 0x%lx, CsType 0x%x, AdpNum 0x%x, Address 0x%x, AndMask 0x%08x OrValue 0x%08x PMTimer=0x%08x\n",
    TopologyID,
    CsType,
    AdapterNum,
    Address,
    *AndMask,
    *OrValue,
    GetACPITimer ()
    );

  Status = AmdUsb4Ring0ReadCS (
             pCM,
             TopologyID,
             CsType,        // CsType
             AdapterNum,    // Adapter Number
             Address,       // Read Address
             SizeInDW,      // Read Size
             &DataPtr,      // Data Out
             &DataSize      // Data Size
             );

  if ( !EFI_ERROR (Status) && ( SizeInDW == DataSize ) ) {
    if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
      // Print Read DWs
      IDS_HDT_CONSOLE_USB4_INFO ("   Read %d DWs:\n", DataSize);
      for (i = 0; i < DataSize; i++) {
        IDS_HDT_CONSOLE_USB4_INFO ("   #%d 0x%08x\n", i, DataPtr[i]);
      }

      // And and Or
      IDS_HDT_CONSOLE_USB4_INFO ("   Modify %d DWs:\n", DataSize);
      for (i = 0; i < DataSize; i++) {
        DataPtr[i] = ( DataPtr[i] & AndMask[i] ) | OrValue[i];
        IDS_HDT_CONSOLE_USB4_INFO ("   #%d 0x%08x\n", i, DataPtr[i]);
      }
    } else {
      for (i = 0; i < DataSize; i++) {
        DataPtr[i] = ( DataPtr[i] & AndMask[i] ) | OrValue[i];
      }
    }

    Status = AmdUsb4Ring0WriteCS (
               pCM,
               TopologyID,
               CsType,      // CsType
               AdapterNum,  // Adapter Number
               Address,     // Write Address
               &DataSize,   // Data Size
               DataPtr      // Data Pointer
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Write CS return Status %r\n", Status);
    }
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read. Status: %r, DataSize: %d \n", Status, DataSize);
  }

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Wait for bit.
 *
 * @param[in]      *pCM             Point to global structure AMD_USB4_CM
 * @param[in]      TopologyID       Route String.
 * @param[in]      CsType           Configuration Space want to read. 0 - Path CS; 1 - Adapter CS; 2 - Router CS; 3 - Counters CS;
 * @param[in]      AdapterNum       Adapter Number want to read
 * @param[in]      ReadAddress      Offset of the CS want to read
 * @param[in]      BitMap           Bit need to logic AND
 * @param[in]      BitValue         Bit Value
 * @param[in]      TimeOutuSec      Timeout for uSecond.
 *
 * @retval EFI_SUCCESS           Success. *WriteDataSize will have the actual write size (double words).
 * @retval EFI_DEVICE_ERROR      Failed.  Router return Error Event code.
 * @retval EFI_TIMEOUT           Failed   Input parameter is not good.
**/
EFI_STATUS
AmdUsb4ReadBitTimeout (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         ReadAddress,
  IN     UINT32         BitMap,
  IN     UINT32         BitValue,
  IN     UINTN          TimeOutuSec
  )
{
  EFI_STATUS    Status;
  USB4_TIMEOUT  Timeout;
  UINT32        *PtrTemp32;
  UINT32        DataSize;

  Status    = EFI_TIMEOUT;
  PtrTemp32 = NULL;
  DataSize  = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));
  StartTimeout (&Timeout, TimeOutuSec);

  do {
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               TopologyID,
               CsType,                // CsType
               AdapterNum,            // AdapterNum
               ReadAddress,           // ReadAddress
               1,                     // ReadSize 1 DWs
               &PtrTemp32,            // DataOut
               &DataSize              // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        "   Failed to read Router[0x%lx] CsType[%d] AdapterNum[%d] Offset[%d].\n",
        TopologyID,
        CsType,
        AdapterNum,
        ReadAddress
        );
      Status = EFI_DEVICE_ERROR;
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("   Read Value=0x%x.\n", *PtrTemp32);

    if ( ((*PtrTemp32) & BitMap) == BitValue ) {
      Status = EFI_SUCCESS;
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("   Waiting...\n");

    Status = HasItTimedOut (&Timeout);
  }  while ( ( Status != EFI_TIMEOUT ) );

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit with Status %r\n", __FUNCTION__, Status);

  return Status;
}


#if AMD_USB4_DEBUG_LANE_HOTPLUG_DISABLE || AMD_USB4_DEBUG_DP_HOTPLUG_DISABLE
VOID
DisableHotplugEvent (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         AdapterNum
  )
{
  EFI_STATUS    Status;
  UINT32        AndMask;
  UINT32        OrValue;

  Status  = EFI_SUCCESS;
  AndMask = (UINT32)(~BIT31);
  OrValue = (UINT32)BIT31;

  IDS_HDT_CONSOLE_USB4_DEBUG ("Disable Router 0x%lx Adapter %d HOT PLUG EVENT\n", TopologyID, AdapterNum);

  Status = AmdUsb4Ring0RMWCS (
             pCM,
             TopologyID,
             CS_TARGET_ADAPTER,
             AdapterNum,
             5,
             1,
             &AndMask,
             &OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Disable HOT PLUG EVENT Status %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to disable HOT PLUG EVENT.\n");
  }
}
#endif
