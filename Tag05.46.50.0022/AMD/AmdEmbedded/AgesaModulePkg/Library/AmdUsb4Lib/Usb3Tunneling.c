/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>
#include "Library/AmdUsb4Lib.h"

#define FILECODE LIBRARY_AMDUSB4LIB_USB3TUNNELING_FILECODE

/*----------------------------------------------------------------------------------------*/
/**
 * Teardown Path for USB3 tunneling
 *
 * @param[in]  *pCM                   Point to global structure AMD_USB4_CM
 * @param[in]  *pParentUsb3DownAdp    Point to Parent USB3 Downstream Adapter
 * @param[in]  *pChildRouter          Point to Child Router Data structure
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_INVALID_PARAMETER      Invalid parameters.
 * @retval EFI_DEVICE_ERROR           HW error.
**/
EFI_STATUS
Usb3PathTeardown (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pParentUsb3DownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_LANE_ADAPTER  *pParentDFPLaneAdapter;
  AMD_USB4_CM_LANE_ADAPTER  *pChildUFPLaneAdapter;
  AMD_USB4_CM_USB3_ADAPTER  *pChildAdapterUsb3Up;
  AMD_USB4_PATHN_CS_0       Usb3PathNCs0And;
  AMD_USB4_PATHN_CS_0       Usb3PathNCs0Or;
  AMD_USB4_PATHN_CS_0       LanePathNCs0And;
  AMD_USB4_PATHN_CS_0       LanePathNCs0Or;
  UINT32                    DataSize;
  UINT32                    *DataPtr;

  Status                    = EFI_SUCCESS;
  pParentDFPLaneAdapter     = (AMD_USB4_CM_LANE_ADAPTER*)(pParentUsb3DownAdp->AdpHeader.pLinkPartner);

  if (pChildRouter != NULL) {
    pChildUFPLaneAdapter    = (AMD_USB4_CM_LANE_ADAPTER*)(pChildRouter->pAdapterUFP);
    pChildAdapterUsb3Up     = (AMD_USB4_CM_USB3_ADAPTER*)(pChildRouter->pAdapterUsb3Up);
  } else {
    pChildUFPLaneAdapter    = NULL;
    pChildAdapterUsb3Up     = NULL;
  }

  Usb3PathNCs0And.Value     = 0x0;
  Usb3PathNCs0Or.Value      = 0x0;

  LanePathNCs0And.Value     = 0x0;
  LanePathNCs0Or.Value      = 0x0;

  DataSize                  = 0;
  DataPtr                   = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  //
  // 1. Teardown USB3 Tunneling Downstream Path
  //

  // 1.1 Clear Parent Router USB3 Downstream Adapter Path PATH_CS_0.Valid bit[31]
  IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Parent Router USB3 Downstream Adapter Path\n");

  Status = PathTeardown (
             pCM,
             (AMD_USB4_CM_CELL *) pParentUsb3DownAdp,
             8
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
    return Status;
  }

  pParentUsb3DownAdp->PathValid = 0;

  //
  // Skip Path teardown in Child Router if it has been disconnected
  //
  if (pChildRouter != NULL) {
    // 1.2 Clear Child Router UFP Lane Adapter Path PATH_CS_0.Valid bit[31]
    IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Child Router UFP Lane Adapter Path\n");

    Status = PathTeardown (
               pCM,
               (AMD_USB4_CM_CELL *) pChildUFPLaneAdapter,
               USB3_DOWNSTREAM_PATH_HOPID
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
      return Status;
    }

    pChildUFPLaneAdapter->Usb3TunnelOn = 0;

    //
    // 2. Teardown USB3 Tunneling Upstream Path
    //

    // 2.1 Clear Child Router USB3 Upstream Adapter Path PATH_CS_0.Valid bit[31]
    IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Child Router USB3 Upstream Adapter Path\n");

    Status = PathTeardown (
               pCM,
               (AMD_USB4_CM_CELL *) pChildAdapterUsb3Up,
               8
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
      return Status;
    }

    pChildAdapterUsb3Up->PathValid = 0;
  }

  // 2.2 Clear Parent Router DFP Lane Adapter Path PATH_CS_0.Valid bit[31]
  IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Parent Router DFP Lane Adapter Path\n");

  Status = PathTeardown (
             pCM,
             (AMD_USB4_CM_CELL *) pParentDFPLaneAdapter,
             USB3_UPSTREAM_PATH_HOPID
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
    return Status;
  }

  pParentDFPLaneAdapter->Usb3TunnelOn = 0;

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Setup Path for USB3 tunneling
 *
 * @param[in]  *pCM                   Point to global structure AMD_USB4_CM
 * @param[in]  *pParentUsb3DownAdp    Point to Parent USB3 Downstream Adapter
 * @param[in]  *pChildRouter          Point to Child Router Data structure
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_INVALID_PARAMETER      Invalid parameters.
 * @retval EFI_DEVICE_ERROR           HW error.
 * @retval EFI_BUFFER_TOO_SMALL       Can not allocate minimum buffers for USB3 Path.

**/

EFI_STATUS
Usb3PathSetup (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pParentUsb3DownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_LANE_ADAPTER  *pParentDFPLaneAdapter;
  AMD_USB4_CM_ROUTER        *pParentRouter;
  AMD_USB4_CM_LANE_ADAPTER  *pChildUFPLaneAdapter;
  AMD_USB4_CM_USB3_ADAPTER  *pChildAdapterUsb3Up;

  AMD_USB4_ADP_USB3_CS_0                                        Usb3AdpCapCs0And;
  AMD_USB4_ADP_USB3_CS_0                                        Usb3AdpCapCs0Or;

  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER  Usb3PathAnd;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER  Usb3PathOr;

  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER      LanePathAnd;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER      LanePathOr;

  Status                = EFI_SUCCESS;
  pParentDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)(pParentUsb3DownAdp->AdpHeader.pLinkPartner);
  pParentRouter         = (AMD_USB4_CM_ROUTER*)(pParentDFPLaneAdapter->Header.Parent);
  pChildUFPLaneAdapter  = (AMD_USB4_CM_LANE_ADAPTER*)(pChildRouter->pAdapterUFP);
  pChildAdapterUsb3Up   = (AMD_USB4_CM_USB3_ADAPTER*)(pChildRouter->pAdapterUsb3Up);

  Usb3AdpCapCs0And.Value              = 0x3FFFFFFF;
  Usb3AdpCapCs0Or.Value               = 0xC0000000;

  Usb3PathOr.PathNProtocolCs0.Value   = 0;
  Usb3PathOr.PathNProtocolCs1.Value   = 0;

  LanePathOr.PathNLaneCs0.Value       = 0;
  LanePathOr.PathNLaneCs1.Value       = 0;

  Usb3PathAnd.PathNProtocolCs0.Value  = 0xFFFFFFFF;
  Usb3PathAnd.PathNProtocolCs1.Value  = 0xFFFFFFFF;
  Usb3PathAnd.PathNProtocolCs0.Field.OutputAdapter                    = 0;
  Usb3PathAnd.PathNProtocolCs0.Field.OutputHopID                      = 0;
  Usb3PathAnd.PathNProtocolCs0.Field.Valid                            = 0;
  Usb3PathAnd.PathNProtocolCs1.Field.CounterEnable                    = 0;
  Usb3PathAnd.PathNProtocolCs1.Field.Weight                           = 0;
  Usb3PathAnd.PathNProtocolCs1.Field.Priority                         = 0;
  Usb3PathAnd.PathNProtocolCs1.Field.EgressFlowControlFlag            = 0;
  Usb3PathAnd.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag  = 0;

  LanePathAnd.PathNLaneCs0.Value = 0xFFFFFFFF;
  LanePathAnd.PathNLaneCs1.Value = 0xFFFFFFFF;
  LanePathAnd.PathNLaneCs0.Field.OutputAdapter                    = 0;
  LanePathAnd.PathNLaneCs0.Field.OutputHopID                      = 0;
  LanePathAnd.PathNLaneCs0.Field.Valid                            = 0;
  LanePathAnd.PathNLaneCs0.Field.PathCreditsAllocated             = 0;
  LanePathAnd.PathNLaneCs1.Field.CounterEnable                    = 0;
  LanePathAnd.PathNLaneCs1.Field.Weight                           = 0;
  LanePathAnd.PathNLaneCs1.Field.Priority                         = 0;
  LanePathAnd.PathNLaneCs1.Field.IngressFlowControlFlag           = 0;
  LanePathAnd.PathNLaneCs1.Field.IngresSharedBufferingEnableFlag  = 0;
  LanePathAnd.PathNLaneCs1.Field.EgressFlowControlFlag            = 0;
  LanePathAnd.PathNLaneCs1.Field.EgressSharedBufferingEnableFlag  = 0;

  //
  // Buffer allocation
  //
  Status = Usb3BufferAllocation (
             pParentDFPLaneAdapter,
             (UINT32)(pParentRouter->BufferAllocationMaxUSB3),
             pChildUFPLaneAdapter,
             (UINT32)(pChildRouter->BufferAllocationMaxUSB3)
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to allocate USB3 buffers = %r\n", Status);
    return Status;
  }

  //
  // Setup bandwidth
  //
  if (pParentRouter->IsHost) {
    Status = Usb3Bandwidth (
               pCM,
               pParentRouter,
               pParentUsb3DownAdp,
               pChildAdapterUsb3Up,
               pParentDFPLaneAdapter,
               pChildUFPLaneAdapter
               );
    IDS_HDT_CONSOLE_USB4_DEBUG ("Setup USB3 Bandwidth Status = %r\n", Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // 1. Setup USB3 Tunneling Downstream Path
  //
  // 1.1 Setup Parent Router USB3 Downstream Adapter Path CS
  if ( pParentDFPLaneAdapter->AdpHeader.MaxOutputHopId < USB3_DOWNSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Parent DFP Lane max output HopID is %d which is smaller than %d.\n",
      pParentDFPLaneAdapter->AdpHeader.MaxOutputHopId,
      USB3_DOWNSTREAM_PATH_HOPID
      );
    IDS_DEADLOOP ();
  }

  Usb3PathOr.PathNProtocolCs0.Value   = 0x00000000;
  Usb3PathOr.PathNProtocolCs1.Value   = 0x00000000;

  Usb3PathOr.PathNProtocolCs0.Field.OutputAdapter                   = (UINT32)pParentDFPLaneAdapter->Header.AdapterNum;
  Usb3PathOr.PathNProtocolCs0.Field.OutputHopID                     = USB3_DOWNSTREAM_PATH_HOPID;
  Usb3PathOr.PathNProtocolCs0.Field.Valid                           = 1;
  Usb3PathOr.PathNProtocolCs1.Field.CounterEnable                   = 0;
  Usb3PathOr.PathNProtocolCs1.Field.Weight                          = USB3_TO_LANE_WEIGHT;
  Usb3PathOr.PathNProtocolCs1.Field.Priority                        = USB3_TO_LANE_PRIORITY;
  Usb3PathOr.PathNProtocolCs1.Field.EgressFlowControlFlag           = USB3_TO_LANE_EFC;
  Usb3PathOr.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag = USB3_TO_LANE_ESE;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Parent Router [0x%lx] USB3 DownStream Adapter [%d] Path. Usb3PathAnd=0x%lx, Usb3PathOr=0x%lx\n",
    pParentUsb3DownAdp->Header.TopologyID,
    pParentUsb3DownAdp->Header.AdapterNum,
    *((UINT64*)(&Usb3PathAnd)),
    *((UINT64*)(&Usb3PathOr))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentUsb3DownAdp->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pParentUsb3DownAdp->Header.AdapterNum,               // AdapterNum
             8<<1,                                                // Address
             2,                                                   // SizeInDW
             (UINT32*)&(Usb3PathAnd),                             // AndMask
             (UINT32*)&(Usb3PathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pParentUsb3DownAdp->PathValid = 1;

  // 1.2 Setup Child Router UFP Lane Adapter Path CS
  if ( pChildUFPLaneAdapter->AdpHeader.MaxInputHopId < USB3_DOWNSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "UFP Lane max input HopID is %d which is smaller than %d.\n",
      pChildUFPLaneAdapter->AdpHeader.MaxOutputHopId,
      USB3_DOWNSTREAM_PATH_HOPID
      );
    IDS_DEADLOOP ();
  }

  LanePathOr.PathNLaneCs0.Value = 0x00000000;
  LanePathOr.PathNLaneCs1.Value = 0x00000000;

  LanePathOr.PathNLaneCs0.Field.OutputAdapter                   = (UINT32)pChildAdapterUsb3Up->Header.AdapterNum;
  LanePathOr.PathNLaneCs0.Field.OutputHopID                     = 8;
  LanePathOr.PathNLaneCs0.Field.Valid                           = 1;
  LanePathOr.PathNLaneCs0.Field.PathCreditsAllocated            = pChildUFPLaneAdapter->Usb3Credits;
  LanePathOr.PathNLaneCs1.Field.CounterEnable                   = 0;
  LanePathOr.PathNLaneCs1.Field.Weight                          = LANE_TO_USB3_WEIGHT;
  LanePathOr.PathNLaneCs1.Field.Priority                        = LANE_TO_USB3_PRIORITY;
  LanePathOr.PathNLaneCs1.Field.IngressFlowControlFlag          = LANE_TO_USB3_IFC;
  LanePathOr.PathNLaneCs1.Field.IngresSharedBufferingEnableFlag = LANE_TO_USB3_ISE;
  LanePathOr.PathNLaneCs1.Field.EgressFlowControlFlag           = LANE_TO_USB3_EFC;
  LanePathOr.PathNLaneCs1.Field.EgressSharedBufferingEnableFlag = LANE_TO_USB3_ESE;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Child Router [0x%lx] UFP Lane Adapter [%d] Path. LanePathAnd=0x%lx, LanePathOr=0x%lx\n",
    pChildUFPLaneAdapter->Header.TopologyID,
    pChildUFPLaneAdapter->Header.AdapterNum,
    *((UINT64*)(&LanePathAnd)),
    *((UINT64*)(&LanePathOr))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pChildUFPLaneAdapter->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pChildUFPLaneAdapter->Header.AdapterNum,             // AdapterNum
             USB3_DOWNSTREAM_PATH_HOPID<<1,                       // Address
             2,                                                   // SizeInDW
             (UINT32*)&(LanePathAnd),                             // AndMask
             (UINT32*)&(LanePathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pChildUFPLaneAdapter->Usb3TunnelOn = 1;

  //
  // 2. Setup USB3 Tunneling Upstream Path
  //
  // 2.1 Setup Child Router USB3 Upstream Adapter Path CS
  if ( pChildUFPLaneAdapter->AdpHeader.MaxOutputHopId < USB3_UPSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Child UPF Lane Adapter max Output HopID is %d which is smaller than %d.\n",
      pChildUFPLaneAdapter->AdpHeader.MaxOutputHopId,
      USB3_UPSTREAM_PATH_HOPID
      );
    IDS_DEADLOOP ();
  }

  Usb3PathOr.PathNProtocolCs0.Value   = 0x00000000;
  Usb3PathOr.PathNProtocolCs1.Value   = 0x00000000;

  Usb3PathOr.PathNProtocolCs0.Field.OutputAdapter                   = (UINT32)pChildUFPLaneAdapter->Header.AdapterNum;
  Usb3PathOr.PathNProtocolCs0.Field.OutputHopID                     = USB3_UPSTREAM_PATH_HOPID;
  Usb3PathOr.PathNProtocolCs0.Field.Valid                           = 1;
  Usb3PathOr.PathNProtocolCs1.Field.CounterEnable                   = 0;
  Usb3PathOr.PathNProtocolCs1.Field.Weight                          = USB3_TO_LANE_WEIGHT;
  Usb3PathOr.PathNProtocolCs1.Field.Priority                        = USB3_TO_LANE_PRIORITY;
  Usb3PathOr.PathNProtocolCs1.Field.EgressFlowControlFlag           = USB3_TO_LANE_EFC;
  Usb3PathOr.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag = USB3_TO_LANE_ESE;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Child Router [0x%lx] USB3 Upstream Adapter [%d] Path. Usb3PathAnd=0x%lx, Usb3PathOr=0x%lx\n",
    pChildAdapterUsb3Up->Header.TopologyID,
    pChildAdapterUsb3Up->Header.AdapterNum,
    *((UINT64*)(&Usb3PathAnd)),
    *((UINT64*)(&Usb3PathOr))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pChildAdapterUsb3Up->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pChildAdapterUsb3Up->Header.AdapterNum,              // AdapterNum
             8<<1,                                                // Address
             2,                                                   // SizeInDW
             (UINT32*)&(Usb3PathAnd),                             // AndMask
             (UINT32*)&(Usb3PathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pChildAdapterUsb3Up->PathValid = 1;

  // 2.2 Setup Parent DFP Lane Adapter Path CS
  if ( pParentDFPLaneAdapter->AdpHeader.MaxInputHopId < USB3_UPSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Parent DFP Lane max input HopID is %d which is smaller than %d.\n",
      pParentDFPLaneAdapter->AdpHeader.MaxInputHopId,
      USB3_UPSTREAM_PATH_HOPID);
    IDS_DEADLOOP ();
  }

  LanePathOr.PathNLaneCs0.Value = 0x00000000;
  LanePathOr.PathNLaneCs1.Value = 0x00000000;

  LanePathOr.PathNLaneCs0.Field.OutputAdapter                   = (UINT32)pParentUsb3DownAdp->Header.AdapterNum;
  LanePathOr.PathNLaneCs0.Field.OutputHopID                     = 8;
  LanePathOr.PathNLaneCs0.Field.Valid                           = 1;
  LanePathOr.PathNLaneCs0.Field.PathCreditsAllocated            = pParentDFPLaneAdapter->Usb3Credits;
  LanePathOr.PathNLaneCs1.Field.CounterEnable                   = 0;
  LanePathOr.PathNLaneCs1.Field.Weight                          = LANE_TO_USB3_WEIGHT;
  LanePathOr.PathNLaneCs1.Field.Priority                        = LANE_TO_USB3_PRIORITY;
  LanePathOr.PathNLaneCs1.Field.IngressFlowControlFlag          = LANE_TO_USB3_IFC;
  LanePathOr.PathNLaneCs1.Field.IngresSharedBufferingEnableFlag = LANE_TO_USB3_ISE;
  LanePathOr.PathNLaneCs1.Field.EgressFlowControlFlag           = LANE_TO_USB3_EFC;
  LanePathOr.PathNLaneCs1.Field.EgressSharedBufferingEnableFlag = LANE_TO_USB3_ESE;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Parent Router [0x%lx] DFP Lane Adapter [%d] Path. LanePathAnd=0x%lx, LanePathOr=0x%lx\n",
    pParentDFPLaneAdapter->Header.TopologyID,
    pParentDFPLaneAdapter->Header.AdapterNum,
    *((UINT64*)(&LanePathAnd)),
    *((UINT64*)(&LanePathOr))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentDFPLaneAdapter->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pParentDFPLaneAdapter->Header.AdapterNum,            // AdapterNum
             USB3_UPSTREAM_PATH_HOPID<<1,                         // Address
             2,                                                   // SizeInDW
             (UINT32*)&(LanePathAnd),                             // AndMask
             (UINT32*)&(LanePathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pParentDFPLaneAdapter->Usb3TunnelOn = 1;

  //
  // 3. Enable Transport Layer Packets on the USB3 Downstream Adapter
  //

  // 3.1 Set Parent Router USB3 Downstream Adapter ADP_USB3_CS_0.PathEnable bit[31] to 1b
  Usb3AdpCapCs0And.Value              = 0x7FFFFFFF;
  Usb3AdpCapCs0Or.Value               = 0x80000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Parent Router [0x%lx] USB3 Downstream Adapter [%d] ADP_USB3_CS_0.PathEnable bit[31] to 1b."
    " Usb3AdpCapCs0And=0x%x, Usb3AdpCapCs0Or=0x%x\n",
    pParentUsb3DownAdp->Header.TopologyID,
    pParentUsb3DownAdp->Header.AdapterNum,
    Usb3AdpCapCs0And.Value,
    Usb3AdpCapCs0Or.Value
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentUsb3DownAdp->Header.TopologyID,
             CS_TARGET_ADAPTER,                       // CsType
             pParentUsb3DownAdp->Header.AdapterNum,   // AdapterNum
             pParentUsb3DownAdp->USB3Capability,      // Address
             1,                                       // SizeInDW
             &(Usb3AdpCapCs0And.Value),               // AndMask
             &(Usb3AdpCapCs0Or.Value)                 // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  // 3.2 Set Parent Router USB3 Downstream Adapter ADP_USB3_CS_0.Valid bit[30] to 1b
  Usb3AdpCapCs0And.Value              = 0xBFFFFFFF;
  Usb3AdpCapCs0Or.Value               = 0x40000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Parent Router [0x%lx] USB3 Downstream Adapter [%d] ADP_USB3_CS_0.Valid bit[30] to 1b."
    " Usb3AdpCapCs0And=0x%x, Usb3AdpCapCs0Or=0x%x\n",
    pParentUsb3DownAdp->Header.TopologyID,
    pParentUsb3DownAdp->Header.AdapterNum,
    Usb3AdpCapCs0And.Value,
    Usb3AdpCapCs0Or.Value
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentUsb3DownAdp->Header.TopologyID,
             CS_TARGET_ADAPTER,                       // CsType
             pParentUsb3DownAdp->Header.AdapterNum,   // AdapterNum
             pParentUsb3DownAdp->USB3Capability,      // Address
             1,                                       // SizeInDW
             &(Usb3AdpCapCs0And.Value),               // AndMask
             &(Usb3AdpCapCs0Or.Value)                 // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pParentUsb3DownAdp->TunnelOn = 1;

  //
  // 4. Enable Transport Layer Packets on the USB3 Upstream Adapter
  //

  // 4.1 Set Child USB3 Upstream Adapter ADP_USB3_CS_0.PathEnable bit[31] to 1b
  Usb3AdpCapCs0And.Value              = 0x7FFFFFFF;
  Usb3AdpCapCs0Or.Value               = 0x80000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Child Router [0x%lx] USB3 Upstream Adapter [%d] ADP_USB3_CS_0.PathEnable bit[31] to 1b."
    " Usb3AdpCapCs0And=0x%x, Usb3AdpCapCs0Or=0x%x\n",
    pChildAdapterUsb3Up->Header.TopologyID,
    pChildAdapterUsb3Up->Header.AdapterNum,
    Usb3AdpCapCs0And.Value,
    Usb3AdpCapCs0Or.Value
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pChildAdapterUsb3Up->Header.TopologyID,
             CS_TARGET_ADAPTER,                       // CsType
             pChildAdapterUsb3Up->Header.AdapterNum,  // AdapterNum
             pChildAdapterUsb3Up->USB3Capability,     // Address
             1,                                       // SizeInDW
             &(Usb3AdpCapCs0And.Value),               // AndMask
             &(Usb3AdpCapCs0Or.Value)                 // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  // 4.2 Set Child USB3 Upstream Adapter ADP_USB3_CS_0.Valid bit[30] to 1b
  Usb3AdpCapCs0And.Value              = 0xBFFFFFFF;
  Usb3AdpCapCs0Or.Value               = 0x40000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Child Router [0x%lx] USB3 Upstream Adapter [%d] ADP_USB3_CS_0.Valid bit[30] to 1b."
    " Usb3AdpCapCs0And=0x%x, Usb3AdpCapCs0Or=0x%x\n",
    pChildAdapterUsb3Up->Header.TopologyID,
    pChildAdapterUsb3Up->Header.AdapterNum,
    Usb3AdpCapCs0And.Value,
    Usb3AdpCapCs0Or.Value
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pChildAdapterUsb3Up->Header.TopologyID,
             CS_TARGET_ADAPTER,                       // CsType
             pChildAdapterUsb3Up->Header.AdapterNum,  // AdapterNum
             pChildAdapterUsb3Up->USB3Capability,     // Address
             1,                                       // SizeInDW
             &(Usb3AdpCapCs0And.Value),               // AndMask
             &(Usb3AdpCapCs0Or.Value)                 // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pChildAdapterUsb3Up->TunnelOn = 1;


  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Enable Path for USB3 tunneling
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter         Point to Router CM Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
Usb3PathEnable (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter
  )
{
  // TODO

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate USB3 Adapter
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  AdapterNum       USB4 controller has N rings. This parameter define Ring Number
 * @param[in]  *pRouter         Point to Router CM Data structure
 * @param[in]  *pAdapter        Point to Adapter CM Data structure
 * @param[in]  *pAdapterCs      Point to Adapter Configuration Space data ( 60 DWs )
 *
 * @param[out]  *pRouter         Point to Router CM Data structure
 * @param[out]  *pAdapter        Point to Adapter CM Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
EnumerateUsb3Adapter (
  IN      AMD_USB4_CM                           *pCM,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
  )
{
  EFI_STATUS                                Status;
  UINT32                                    DataSize;
  UINT32                                    *DataPtr;

  Status            = EFI_SUCCESS;
  DataSize          = 0;
  DataPtr           = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pAdapter || NULL == pCM || NULL == pAdapterCs ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  //no need to clear again SetMem (pAdapter, sizeof (AMD_USB4_CM_USB3_ADAPTER), 0);

  if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == UpStreamUSB3Adapter ) {
    pAdapter->Header.CellType = CELL_TYPE_USB3_UP_ADAPTER;
  } else {
    pAdapter->Header.CellType = CELL_TYPE_USB3_DOWN_ADAPTER;
  }
  pAdapter->Header.TopologyID = pRouter->Header.TopologyID;
  pAdapter->Header.AdapterNum = (UINT8)AdapterNum;
  pAdapter->Header.Parent     = (AMD_USB4_CM_CELL_HEADER*)pRouter;

  pAdapter->AdpHeader.MaxInputHopId       = (UINT16)(pAdapterCs->AdpCs5.Field.MaxInputHopID);
  pAdapter->AdpHeader.MaxOutputHopId      = (UINT16)(pAdapterCs->AdpCs5.Field.MaxOutputHopID);

  pAdapter->USB3Capability  = pAdapterCs->AdpCs1.Field.NextCapabilityPointer;
  pAdapter->PathValid       = 0;
  pAdapter->TunnelOn        = 0;

  if ( pAdapter->USB3Capability == 0 ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Usb3 Adapter Capability offset is 0.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // USB3 Adapter just have one capability.
  //
  DataSize = sizeof (AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
  // Read USB3 Capability Registers
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                   // CsType
             AdapterNum,                                          // AdapterNum
             pAdapterCs->AdpCs1.Field.NextCapabilityPointer,      // ReadAddress
             DataSize,                                            // ReadSize
             (UINT32**)&DataPtr,                                  // DataOut
             &DataSize                                            // DataSize
             );
  if (EFI_ERROR (Status) || ( sizeof (AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read USB3 Adapter Capability Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  if ( DataPtr && ((AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->AdpUSB3_Cs0.Field.CapabilityID == 4 ) {
    pAdapter->ActualLinkRate        =
      ((AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->AdpUSB3_Cs4.Field.ActualLinkRate;
    pAdapter->USB3LinkValid         =
      ((AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->AdpUSB3_Cs4.Field.USb3LinkValid;
    pAdapter->PortLinkState         =
      ((AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->AdpUSB3_Cs4.Field.PortLinkState;
    pAdapter->MaxSupportedLinkRate  =
      ((AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->AdpUSB3_Cs4.Field.MaximumSupportedLinkRate;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR ("Usb3 Adapter Capability ID is not 4.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Increase USB3 adapter amount in Router CM Data Structure. and enable Router CM Data Structure USB3 support bit.
  //
  if ( pAdapter->Header.CellType == CELL_TYPE_USB3_UP_ADAPTER ) {
    pRouter->Usb3UpAmount++;
  } else if ( pAdapter->Header.CellType == CELL_TYPE_USB3_DOWN_ADAPTER ) {
    pRouter->Usb3DownAmount++;
  }

  pAdapter->AdpHeader.pSiblingAdapter     = NULL;
  pAdapter->AdpHeader.pLinkPartner        = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("USB3 Adapter Information:\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterNum     = 0x%x\n",        pAdapter->Header.AdapterNum);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CellType       = 0x%x\n",        pAdapter->Header.CellType);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxInputHopId  = 0x%x\n",        pAdapter->AdpHeader.MaxInputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxOutputHopId = 0x%x\n",        pAdapter->AdpHeader.MaxOutputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  ActualLinkRate = 0x%x\n",        pAdapter->ActualLinkRate);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  USB3LinkValid  = 0x%x\n",        pAdapter->USB3LinkValid);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  PortLinkState  = 0x%x\n",        pAdapter->PortLinkState);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxSupportedLinkRate = 0x%x\n",  pAdapter->MaxSupportedLinkRate);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  USB3Capability  = 0x%x\n",       pAdapter->USB3Capability);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  pRouter->Usb3UpAmount   = 0x%x\n",  pRouter->Usb3UpAmount);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  pRouter->Usb3DownAmount = 0x%x\n",  pRouter->Usb3DownAmount);

  return EFI_SUCCESS;
}
