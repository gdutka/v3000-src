/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/IdsLib.h>
#include <Library/BaseLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>
#include <Library/AmdUsb4Lib.h>

#define FILECODE LIBRARY_AMDUSB4LIB_DPTUNNELING14_FILECODE

#define CR(Record, TYPE, Field)  ((TYPE *) ((CHAR8 *) (Record) - (CHAR8 *) &(((TYPE *) 0)->Field)))
#define AMD_USB4_CM_DP_ADAPTER_FROM_ENTRY(a)               CR (a, AMD_USB4_CM_DP_ADAPTER, Link);

/**
 * Teardown DisplayPort Tunneling
 *
 * @param[in] *pCM                Pointer to global structure AMD_USB4_CM
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_UNSUPPORTED        DP Tunneling over USB4 is not supported
 * @retval EFI_DEVICE_ERROR       Read/Write error with register
 * @retval EFI_NOT_FOUND          No Displayport Source/Sink pair found
**/
EFI_STATUS
DpPathTeardown (
  IN     AMD_USB4_CM             *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER  *pDpIn,
  IN     AMD_USB4_CM_DP_ADAPTER  *pDpOut,
  IN     AMD_USB4_CM_CELL        *pIngressAdapter,
  IN     AMD_USB4_CM_ROUTER      *pDeepestChildRouter,
  IN     BOOLEAN                  IsDpInAuxPath
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_ROUTER        *pParentRouter;
  UINT64                    TopologyID;
  UINT8                     *pPathDFPAdpNum;
  UINT8                     PathDFPAdpNum;
  AMD_USB4_CM_LANE_ADAPTER  *pDFPLaneAdapter;
  UINT8                     Index;
  BOOLEAN                   FoundAdapter;
  UINT32                    DpMainPathHopId;
  UINT32                    DpInAuxPathHopId;
  UINT32                    DpOutAuxPathHopId;
  UINT32                    TempPathHopId;
  UINT16                    BufferAllocationDpMain;

  Status                  = EFI_SUCCESS;
  pParentRouter           = NULL;
  TopologyID              = 0;
  pPathDFPAdpNum          = NULL;
  PathDFPAdpNum           = 0;
  pDFPLaneAdapter         = NULL;
  Index                   = 0;
  FoundAdapter            = FALSE;
  DpMainPathHopId         = (UINT32) DP_MAIN_PATH_HOPID + ((pDpIn->DpLinkNumber - 1) * 3);
  DpInAuxPathHopId        = (UINT32) DP_IN_AUX_PATH_HOPID + ((pDpIn->DpLinkNumber - 1) * 3);
  DpOutAuxPathHopId       = (UINT32) DP_OUT_AUX_PATH_HOPID + ((pDpOut->DpLinkNumber - 1) * 3);
  TempPathHopId           = 0;
  pParentRouter           = (AMD_USB4_CM_ROUTER *)pIngressAdapter->Header.Parent;
  BufferAllocationDpMain  = 0;

  //
  // Teardown DP Path
  // NOTE: If Router that enumerated DP OUT Adapter has been disconnected, will traverse to deepest router on path
  // Recursive code flow:
  // 1. DP Main Path will be torn down as we recursively traverse DP IN Adapter to DP OUT Adapter Path
  // 2. DP OUT Aux Path will be torn down as we pop off the stack
  // 3. DP IN Aux Path will be torn down as we recursively traverse DP IN Adapter to DP OUT Adapter Path
  //

  if ((pParentRouter->Header.TopologyID == pDpIn->Header.TopologyID)) {
    //
    // DP Main Path and DP IN Aux Path Segment: DP IN Adapter to USB4 Port Egress
    // Teardown DP Main Path and DP IN Aux Path in DP IN Parent Router
    //
    if (IsDpInAuxPath) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Teardown DP IN Aux Path\n");
      TempPathHopId = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_INPUT_HOPID;
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Teardown DP Main Path\n");
      TempPathHopId = (UINT32) DP_MAIN_DP_ADAPTER_TO_LANE_INPUT_HOPID;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  Current Router TopologyID: 0x%lx\n"
      "  HopID: 0x%lx\n",
      pParentRouter->Header.TopologyID,
      TempPathHopId
      );
    PathTeardown (
      pCM,
      (AMD_USB4_CM_CELL *) pDpIn,
      TempPathHopId
      );

    if (!IsDpInAuxPath) {
      if ( !IsUSB4Router (pParentRouter) ) {
        Status = DpBufferDeallocation (
                   pCM,
                   pDpIn,
                   (AMD_USB4_CM_CELL_HEADER *) pDpIn,
                   4,
                   12,
                   TRUE
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR (" Failed to deallocate buffer.\n");
          return Status;
        }
      }
    }

    if (IsDpInAuxPath) {
      pDpIn->AuxInTunnelOn = 0;
    } else {
      pDpIn->MainTunnelOn = 0;
    }
  } else {
    //
    // DP Main Path and DP IN Aux Path Segment: DP IN Adapter to USB4 Port Egress//DP OUT Adpater
    // Teardown DP Main Path and DP IN Aux Path in Pass-through USB4 Hub and DP OUT's Parent Adapter
    //

    if (!IsDpInAuxPath) {
      DpBandwidthDeallocation (pDpIn, (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter);
    }

    if (IsDpInAuxPath) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Teardown DP IN Aux Path\n");
      TempPathHopId = (UINT32) DpInAuxPathHopId;
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Teardown DP Main Path\n");
      TempPathHopId = (UINT32) DpMainPathHopId;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  Current Router TopologyID: 0x%lx\n"
      "  HopID: 0x%lx\n",
      pParentRouter->Header.TopologyID,
      TempPathHopId
      );
    PathTeardown (
      pCM,
      (AMD_USB4_CM_CELL *) pIngressAdapter,
      TempPathHopId
      );

    if (!IsDpInAuxPath) {
      if ( IsUSB4Router (pParentRouter) ) {
        Status = DpBufferDeallocation (
                   pCM,
                   pDpIn,
                   (AMD_USB4_CM_CELL_HEADER *) pIngressAdapter,
                   pParentRouter->BufferAllocationMinDPAux,
                   pParentRouter->BufferAllocationMinDPMain,
                   TRUE
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR (" Failed to deallocate buffer.\n");
          return Status;
        }
      } else {
        if (Tbt3RouterType (pParentRouter) == TBT3_TYPE_AR) {
          BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pIngressAdapter)->LaneAdp_CurrentWidth ? 72 : 28;
        } else if (Tbt3RouterType (pParentRouter) == TBT3_TYPE_TR) {
          BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pIngressAdapter)->LaneAdp_CurrentWidth ? 56 : 26;
        } else {
          IDS_HDT_CONSOLE_USB4_ERROR (" Unknown Router type: %x.\n", Tbt3RouterType (pParentRouter));
          return EFI_DEVICE_ERROR;
        }
        IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
        Status = DpBufferDeallocation (
                   pCM,
                   pDpIn,
                   (AMD_USB4_CM_CELL_HEADER *) pIngressAdapter,
                   1,
                   BufferAllocationDpMain,
                   TRUE
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer.\n");
          return Status;
        }
      }
    }

    if (IsDpInAuxPath) {
      if (pDpIn->DpLinkNumber == 1) {
        ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpAuxInTunnel1On = 0;
      } else if (pDpIn->DpLinkNumber == 2) {
        ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpAuxInTunnel2On = 0;
      }
    } else {
      if (pDpIn->DpLinkNumber == 1) {
        ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpMainTunnel1On = 0;
      } else if (pDpIn->DpLinkNumber == 2) {
        ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpMainTunnel2On = 0;
      }
    }
  }

  //
  // Using the Topology ID of the DP Out Adapter's Parent Router (or deepest child router on path),
  // traverse to the next Child Router on Path and recursively continue to teardown paths in Pass-through USB4 Hubs
  //
  TopologyID      = pDeepestChildRouter->Header.TopologyID;
  pPathDFPAdpNum  = (UINT8*)&TopologyID;
  PathDFPAdpNum   = pPathDFPAdpNum[(pParentRouter->Depth)];
  pDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)pParentRouter->pAdapterDFP;

  if (PathDFPAdpNum != 0) {
    for (Index = 0; Index < pParentRouter->DFPAmount; Index++) {
      if (pDFPLaneAdapter == NULL) {
        IDS_HDT_CONSOLE_USB4_ERROR (" DFP Lane Adapter is NULL\n");
        return EFI_NOT_FOUND;
      }

      if (pDFPLaneAdapter->Header.AdapterNum == PathDFPAdpNum) {
        FoundAdapter = TRUE;
        break;
      } else {
        pDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)pDFPLaneAdapter->AdpHeader.pSiblingAdapter;
      }
    }

    if (!FoundAdapter) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Could not locate DFP Lane Adapter\n");
      return EFI_NOT_FOUND;
    }

    //
    // Begin teardown of DP OUT Aux Path
    // Recursive call to tear down DP OUT Aux Path
    //
    Status = DpPathTeardown (
               pCM,
               pDpIn,
               pDpOut,
               (AMD_USB4_CM_CELL *)((AMD_USB4_CM_ROUTER *)pDFPLaneAdapter->AdpHeader.pLinkPartner)->pAdapterUFP,
               pDeepestChildRouter,
               IsDpInAuxPath
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to traverse path and teardown DP Path.\n");
      return EFI_DEVICE_ERROR;
    }
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG (" Reached last Child Router on Path.\n");
  }

  //
  // DP IN Aux Path has been torn down. Return as no paths remain to be torn down.
  //
  if (IsDpInAuxPath) {
    return EFI_SUCCESS;
  }

  //
  // DP OUT Aux Path Segment: DP OUT Adapter to USB4 Port Egress
  // Teardown DP OUT Aux Path in DP OUT Adapter's Parent Router
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("Teardown DP OUT Aux Path\n");

  if (pParentRouter->Header.TopologyID == pDpOut->Header.TopologyID) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Teardown DP OUT Aux Path\n"
      "  Current Router TopologyID: 0x%lx\n"
      "  HopID: 0x%lx\n",
      pParentRouter->Header.TopologyID,
      (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_INPUT_HOPID
      );
    PathTeardown (
      pCM,
      (AMD_USB4_CM_CELL *)pDpOut,
      (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_INPUT_HOPID
      );

    pDpOut->AuxOutTunnelOn = 0;
  } else {
    //
    // DP OUT Aux Segment: USB4 Port Ingress to USB4 Port Egress/DP IN Adpater
    // Teardown DP OUT Aux Path in Pass-through USB4 Hub and DP IN's Parent Adapter
    //
    if (pDFPLaneAdapter == NULL) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DFP Lane Adapter is NULL\n");
      return EFI_NOT_FOUND;
    }

    DpBandwidthDeallocation (pDpIn, (AMD_USB4_CM_LANE_ADAPTER *) pDFPLaneAdapter);

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Teardown DP OUT Aux Path\n"
      "  Current Router TopologyID: 0x%lx\n"
      "  HopID: 0x%lx\n",
      pParentRouter->Header.TopologyID,
      DpOutAuxPathHopId
      );
      PathTeardown (
        pCM,
        (AMD_USB4_CM_CELL *)pDFPLaneAdapter,
        (UINT32) DpOutAuxPathHopId
        );

    //
    // Buffer deallocation
    //
    if ( IsUSB4Router (pParentRouter) ) {
      Status = DpBufferDeallocation (
                 pCM,
                 pDpIn,
                 (AMD_USB4_CM_CELL_HEADER *) pDFPLaneAdapter,
                 pParentRouter->BufferAllocationMinDPAux,
                 pParentRouter->BufferAllocationMinDPMain,
                 FALSE);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
        return Status;
      }
    } else {
      if (pParentRouter->Header.TopologyID == pDpIn->Header.TopologyID) {
        IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
        Status = DpBufferDeallocation (
                   pCM,
                   pDpIn,
                   (AMD_USB4_CM_CELL_HEADER *) pDFPLaneAdapter,
                   1,
                   12,
                   FALSE
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
          return Status;
        }
      } else {
        if (Tbt3RouterType (pParentRouter) == TBT3_TYPE_AR) {
          BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pDFPLaneAdapter)->LaneAdp_CurrentWidth ? 72 : 28;
        } else if (Tbt3RouterType (pParentRouter) == TBT3_TYPE_TR) {
          BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pDFPLaneAdapter)->LaneAdp_CurrentWidth ? 56 : 26;
        } else {
          IDS_HDT_CONSOLE_USB4_ERROR (" Unknown Router type: %x.\n", Tbt3RouterType (pParentRouter));
          return EFI_DEVICE_ERROR;
        }
        IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
        Status = DpBufferDeallocation (
                   pCM,
                   pDpIn,
                   (AMD_USB4_CM_CELL_HEADER *) pDFPLaneAdapter,
                   1,
                   BufferAllocationDpMain,
                   FALSE
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer.\n");
          return Status;
        }
      }
    }

    if (pDpIn->DpLinkNumber == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pDFPLaneAdapter)->DpAuxOutTunnel1On = 0;
    } else if (pDpIn->DpLinkNumber == 2) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pDFPLaneAdapter)->DpAuxOutTunnel2On = 0;
    }
  }

  //
  // Recursive call to tear down DP IN Aux Path
  //
  if ((pParentRouter->Header.TopologyID == pDpIn->Header.TopologyID)) {
    Status = DpPathTeardown (
               pCM,
               pDpIn,
               pDpOut,
               (AMD_USB4_CM_CELL *) pDpIn,
               pDeepestChildRouter,
               TRUE
               );
  }

  return Status;
}

/**
 * Configure DisplayPort path
 *
 * @param[in]  *pCM                               Pointer to global structure AMD_USB4_CM.
 * @param[in]  TopologyID                         Topology ID
 * @param[in]  *pIngressAdapter                   Pointer to Ingress Adapter or DP Adapter
 * @param[in]  InputHopID                         Ingress Adapter's InputHopID
 * @param[in]  *pEgressAdapter                    Pointer to Egress Adapter or DP Adapter
 * @param[in]  OutputHopID                        Egress Adapter's InputHopID
 * @param[in]  Valid                              Valid bit
 * @param[in]  PathCreditsAllocated               Path Credits Allocated
 * @param[in]  CounterEnable                      Counter Enable
 * @param[in]  Weight                             Weight
 * @param[in]  Priority                           Priority
 * @param[in]  IngressFlowControlFlag             IngressFlowControlFlag
 * @param[in]  IngresSharedBufferingEnableFlag    IngressFlowControlFlag
 * @param[in]  EgressFlowControlFlag              EgressFlowControlFlag
 * @param[in]  EgressSharedBufferingEnableFlag    EgressSharedBufferingEnableFlag
 *
 * @retval EFI_SUCCESS                            Success.
 * @retval EFI_UNSUPPORTED                        Path from DP IN to DP Out in same router is not supported.
**/
EFI_STATUS
DpPathConfiguration (
  IN     AMD_USB4_CM                          *pCM,
  IN     UINT64                               TopologyID,
  IN     AMD_USB4_CM_CELL_HEADER              *pIngressAdapter,
  IN     UINT32                               InputHopID,
  IN     AMD_USB4_CM_CELL_HEADER              *pEgressAdapter,
  IN     UINT32                               OutputHopID,
  IN     UINT32                               Valid,
  IN     UINT32                               PathCreditsAllocated,
  IN     UINT32                               CounterEnable,
  IN     UINT32                               Weight,
  IN     UINT32                               Priority,
  IN     UINT32                               IngressFlowControlFlag,
  IN     UINT32                               IngresSharedBufferingEnableFlag,
  IN     UINT32                               EgressFlowControlFlag,
  IN     UINT32                               EgressSharedBufferingEnableFlag
  )
{
  EFI_STATUS                Status;
  UINT16                    MaxInputHopID;
  UINT16                    MaxOutputHopID;

  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER  DpPathAnd;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER  DpPathOr;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER      LanePathAnd;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER      LanePathOr;

  Status          = EFI_SUCCESS;
  MaxInputHopID   = 0;
  MaxOutputHopID  = 0;

  DpPathAnd.PathNProtocolCs0.Value                                  = 0xFFFFFFFF;
  DpPathAnd.PathNProtocolCs1.Value                                  = 0xFFFFFFFF;
  DpPathAnd.PathNProtocolCs0.Field.OutputAdapter                    = 0;
  DpPathAnd.PathNProtocolCs0.Field.OutputHopID                      = 0;
  DpPathAnd.PathNProtocolCs0.Field.Valid                            = 0;
  DpPathAnd.PathNProtocolCs1.Field.CounterEnable                    = 0;
  DpPathAnd.PathNProtocolCs1.Field.Weight                           = 0;
  DpPathAnd.PathNProtocolCs1.Field.Priority                         = 0;
  DpPathAnd.PathNProtocolCs1.Field.IngressFlowControlFlag           = 1;
  DpPathAnd.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag  = 1;
  DpPathAnd.PathNProtocolCs1.Field.EgressFlowControlFlag            = 0;
  DpPathAnd.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag  = 0;

  IDS_HDT_CONSOLE_USB4_INFO (
    " PathNProtocolCs0 %x    PathNProtocolCs1 %x.\n",
    DpPathAnd.PathNProtocolCs0.Value,
    DpPathAnd.PathNProtocolCs1.Value
    );

  if ( !IsUSB4Router ((AMD_USB4_CM_ROUTER *) pIngressAdapter->Parent) ) {
    DpPathAnd.PathNProtocolCs0.Field.PathCreditsAllocated             = 0;
    DpPathAnd.PathNProtocolCs1.Field.IngressFlowControlFlag           = 0;
    DpPathAnd.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag  = 0;

    IDS_HDT_CONSOLE_USB4_INFO (
      "Updated for TBT3: PathNProtocolCs0 %x    PathNProtocolCs1 %x.\n",
      DpPathAnd.PathNProtocolCs0.Value,
      DpPathAnd.PathNProtocolCs1.Value
      );
  }

  DpPathOr.PathNProtocolCs0.Value  = 0x00000000;
  DpPathOr.PathNProtocolCs1.Value  = 0x00000000;

  LanePathAnd.PathNLaneCs0.Value                                  = 0xFFFFFFFF;
  LanePathAnd.PathNLaneCs1.Value                                  = 0xFFFFFFFF;
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

  LanePathOr.PathNLaneCs0.Value  = 0x00000000;
  LanePathOr.PathNLaneCs1.Value  = 0x00000000;

  if (pEgressAdapter->CellType == CELL_TYPE_LANE_ADAPTER) {
    MaxOutputHopID = ((AMD_USB4_CM_LANE_ADAPTER *) pEgressAdapter)->AdpHeader.MaxOutputHopId;
  } else {
    MaxOutputHopID = ((AMD_USB4_CM_DP_ADAPTER *) pEgressAdapter)->AdpHeader.MaxOutputHopId;
  }
  if (MaxOutputHopID < OutputHopID) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Egress Adapter's Max Output HopID %d is smaller than %d.\n",
      MaxOutputHopID,
      OutputHopID
      );
    IDS_DEADLOOP ();
  }

  if (pIngressAdapter->CellType == CELL_TYPE_LANE_ADAPTER) {
    MaxInputHopID = ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->AdpHeader.MaxInputHopId;
  } else {
    MaxInputHopID = ((AMD_USB4_CM_DP_ADAPTER *) pIngressAdapter)->AdpHeader.MaxInputHopId;
  }
  if (MaxInputHopID < InputHopID) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Ingress Adapter's Max Input HopID %d is smaller than %d.\n",
      MaxInputHopID,
      InputHopID
      );
    IDS_DEADLOOP ();
  }

  //
  // Based on the Ingress Adapter type, setup DisplayPort Adapter Path or Lane Adapter Path
  //
  if ((pIngressAdapter->CellType == CELL_TYPE_DP_IN_ADAPTER)
    || (pIngressAdapter->CellType == CELL_TYPE_DP_OUT_ADAPTER))
  {
    DpPathOr.PathNProtocolCs0.Value                                  = 0x00000000;
    DpPathOr.PathNProtocolCs1.Value                                  = 0x00000000;
    DpPathOr.PathNProtocolCs0.Field.OutputAdapter                    = pEgressAdapter->AdapterNum;
    if (pEgressAdapter->CellType == CELL_TYPE_LANE_ADAPTER) {
      DpPathOr.PathNProtocolCs0.Field.OutputAdapter                  = pEgressAdapter->AdapterNum;
    } else if ((pEgressAdapter->CellType == CELL_TYPE_DP_IN_ADAPTER)
      || (pEgressAdapter->CellType == CELL_TYPE_DP_OUT_ADAPTER))
    {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Path between DP In Adapter and DP Out Adapter is not supported in the same router"
        );
      return EFI_UNSUPPORTED;
    }
    DpPathOr.PathNProtocolCs0.Field.OutputHopID                      = OutputHopID;
    DpPathOr.PathNProtocolCs0.Field.Valid                            = Valid;
    DpPathOr.PathNProtocolCs0.Field.PathCreditsAllocated             = PathCreditsAllocated;
    DpPathOr.PathNProtocolCs1.Field.CounterEnable                    = CounterEnable;
    DpPathOr.PathNProtocolCs1.Field.Weight                           = Weight;
    DpPathOr.PathNProtocolCs1.Field.Priority                         = Priority;
    DpPathOr.PathNProtocolCs1.Field.IngressFlowControlFlag           = IngressFlowControlFlag;
    DpPathOr.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag  = IngresSharedBufferingEnableFlag;
    DpPathOr.PathNProtocolCs1.Field.EgressFlowControlFlag            = EgressFlowControlFlag;
    DpPathOr.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag  = EgressSharedBufferingEnableFlag;

    IDS_HDT_CONSOLE_USB4_INFO (
      " DpPathAnd=0x%lx, DpPathOr=0x%lx\n",
      *((UINT64*)(&DpPathAnd)),
      *((UINT64*)(&DpPathOr))
      );

    Status = AmdUsb4Ring0RMWCS (
               pCM,
               TopologyID,
               CS_TARGET_PATH,                                                    // CsType
               ((AMD_USB4_CM_LANE_ADAPTER*) pIngressAdapter)->Header.AdapterNum,  // AdapterNum
               InputHopID << 1,                                                   // Address
               2,                                                                 // SizeInDW
               (UINT32*)(&DpPathAnd),                                             // AndMask
               (UINT32*)(&DpPathOr)                                               // OrValue
               );
  } else if (pIngressAdapter->CellType == CELL_TYPE_LANE_ADAPTER) {
    LanePathOr.PathNLaneCs0.Value                                  = 0x00000000;
    LanePathOr.PathNLaneCs0.Value                                  = 0x00000000;
    LanePathOr.PathNLaneCs0.Field.OutputAdapter                    = pEgressAdapter->AdapterNum;
    LanePathOr.PathNLaneCs0.Field.OutputHopID                      = OutputHopID;
    LanePathOr.PathNLaneCs0.Field.Valid                            = Valid;
    LanePathOr.PathNLaneCs0.Field.PathCreditsAllocated             = PathCreditsAllocated;
    LanePathOr.PathNLaneCs1.Field.CounterEnable                    = CounterEnable;
    LanePathOr.PathNLaneCs1.Field.Weight                           = Weight;
    LanePathOr.PathNLaneCs1.Field.Priority                         = Priority;
    LanePathOr.PathNLaneCs1.Field.IngressFlowControlFlag           = IngressFlowControlFlag;
    LanePathOr.PathNLaneCs1.Field.IngresSharedBufferingEnableFlag  = IngresSharedBufferingEnableFlag;
    LanePathOr.PathNLaneCs1.Field.EgressFlowControlFlag            = EgressFlowControlFlag;
    LanePathOr.PathNLaneCs1.Field.EgressSharedBufferingEnableFlag  = EgressSharedBufferingEnableFlag;

    IDS_HDT_CONSOLE_USB4_INFO (
      " LanePathAnd=0x%lx, LanePathOr=0x%lx\n",
      *((UINT64*)(&LanePathAnd)),
      *((UINT64*)(&LanePathOr))
      );

    Status = AmdUsb4Ring0RMWCS (
               pCM,
               TopologyID,
               CS_TARGET_PATH,                                                    // CsType
               ((AMD_USB4_CM_LANE_ADAPTER*) pIngressAdapter)->Header.AdapterNum,  // AdapterNum
               InputHopID << 1,                                                   // Address
               2,                                                                 // SizeInDW
               (UINT32*)(&LanePathAnd),                                           // AndMask
               (UINT32*)(&LanePathOr)                                             // OrValue
               );
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 * Setup Path for DisplayPort tunneling
 *
 * @param[in]  *pCM                 Pointer to global structure AMD_USB4_CM.
 * @param[in]  *pDpIn               Pointer to a DP IN Adapter.
 * @param[in]  *pDpIn               Pointer to a DP OUT Adapter.
 * @param[in]  *pIngressAdapter     Pointer to a path Ingress Adapter.
 * @param[in]  NumPairedAdapters    Number of current DP Adapters paired.
 * @param[in]  DpPathBandwidth      Bandwidth for DisplayPort Tunneling Main-Link Path.
 *
 * @retval EFI_SUCCESS              Success.
 * @retval EFI_DEVICE_ERROR         Failed to traverse path and locate DP IN Adapter.
 * @retval EFI_UNSUPPORTED          Reached Host router without locating DP IN Adapter.
 * @retval RETURN_BUFFER_TOO_SMALL  Can not allocate minimum buffers for DP Paths.
**/
EFI_STATUS
DpPathSetup (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER               *pDpIn,
  IN     AMD_USB4_CM_DP_ADAPTER               *pDpOut,
  IN     AMD_USB4_CM_CELL_HEADER              *pIngressAdapter,
  IN     UINT32                                NumPairedAdapters,
  IN     UINT32                                DpPathBandwidth
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_ROUTER        *pRouter;
  AMD_USB4_CM_CELL_HEADER  *pNextIngressAdapter;
  UINT32                    DpMainPathHopId;
  UINT32                    DpInAuxPathHopId;
  UINT32                    DpOutAuxPathHopId;
  UINT32                    DataSize;
  UINT32                    *DataPtr;
  UINT32                    IFCFlag;
  UINT32                    ISEFlag;
  UINT32                    EFCFlag;
  UINT16                    BufferAllocationDpMain;
  UINT32                    PathCreditsAllocated;

  Status                  = EFI_SUCCESS;
  pRouter                 = (AMD_USB4_CM_ROUTER *) pIngressAdapter->Parent;
  pNextIngressAdapter     = NULL;
  DpMainPathHopId         = (UINT32) DP_MAIN_PATH_HOPID + (NumPairedAdapters * 3);
  DpInAuxPathHopId        = (UINT32) DP_IN_AUX_PATH_HOPID + (NumPairedAdapters * 3);
  DpOutAuxPathHopId       = (UINT32) DP_OUT_AUX_PATH_HOPID + (NumPairedAdapters * 3);
  DataSize                = 0;
  DataPtr                 = NULL;
  IFCFlag                 = 0;
  ISEFlag                 = 0;
  EFCFlag                 = 0;
  BufferAllocationDpMain  = 0;
  PathCreditsAllocated    = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  //
  // Establish DP Paths
  // DP OUT Aux Path will be established as we recursively traverse from DP OUT Adapter to DP IN Adapter
  // DP Main Path and DP IN Aux Path will be established as we pop off the stack
  //
  // Recursive code flow:
  // DP OUT ADAPTER'S PARENT ROUTER
  // -1. DP OUT Aux Segment: DP OUT Adapter to USB4 Port Egress
  // -8. DP Main Segment: USB4 Port Ingress to DP OUT Adapter
  // -9. DP IN Aux Segment: USB4 Port Ingress to DP OUT Adapter
  //
  // PASS_THROUGH USB4 HUB(s)
  // -2. DP OUT Aux Segment: USB4 Port Ingress to USB4 Port Egress
  // -6. DP Main Segment: USB4 Port Ingress to USB4 Port Egress
  // -7. DP IN Aux Segment: USB4 Port Ingress to USB4 Port Egress
  //
  // DP IN ADAPTER'S PARENT ROUTER
  // -3. DP OUT Aux Segment: USB4 Port Ingress to DP IN Adapter
  // -4. DP Main Segment: DP IN Adapter to USB4 Port Egress
  // -5. DP IN Aux Segment: DP IN Adapter to USB4 Port Egress
  //
  if (pRouter->Header.TopologyID == pDpOut->Header.TopologyID) {
    //
    // 1. DP OUT Aux Segment: DP OUT Adapter to USB4 Port Egress
    // Establish DP OUT Aux Path Segment in DP OUT Adapter's Parent Router
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "1. DP OUT Aux Segment: DP OUT Adapter %x (TopologyID 0x%lx) to"
      " USB4 Port Egress Lane Adapter %d (TopologyID 0x%lx)\n",
      pDpOut->Header.AdapterNum,
      pDpOut->Header.TopologyID,
      pRouter->UpstreamAdapter,
      pRouter->Header.TopologyID
      );

    if ( IsUSB4Router (pRouter) ) {
      IFCFlag = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_IFC;
      ISEFlag = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_ISE;
      PathCreditsAllocated = 0;
    } else {
      IFCFlag = 1;
      ISEFlag = 0;
      PathCreditsAllocated = 7;
    }

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pDpOut,                                    // *pIngressAdapter
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_INPUT_HOPID,                       // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pRouter->pAdapterUFP,                      // *pEgressAdapter
               (UINT32) DpOutAuxPathHopId,                                           // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_WEIGHT,                            // Weight
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_PRIORITY,                          // Priority
               (UINT32) IFCFlag,                                                     // IngressFlowControlFlag
               (UINT32) ISEFlag,                                                     // IngresSharedBufferingEnableFlag
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_EFC,                               // EgressFlowControlFlag
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_ESE                                // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    pDpOut->AuxOutTunnelOn = 1;

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "1. DP OUT Aux Segment: DP OUT Adapter to USB4 Port Egress Lane Adapter Status = %r\n",
      Status
      );

    pNextIngressAdapter = pRouter->Header.Parent;

    //
    // Continue recursively along path enumerating Path on Pass-through USB4 Hub locate DP IN
    //
    Status = DpPathSetup (pCM, pDpIn, pDpOut, pNextIngressAdapter, NumPairedAdapters, DpPathBandwidth);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to traverse path and locate DP IN Adapter.\n");
      return EFI_DEVICE_ERROR;
    }

    //
    // Recursive traversal to DP IN Adapter is complete and we've return to DP OUT Adapter
    // Establish DP Main Path and DP IN Aux Patch
    //

    //
    // 8. DP Main Segment: USB4 Port Ingress to DP OUT Adapter
    // Establish DP Main Path in DP OUT Adapter's Parent Router
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "8. DP Main Segment: USB4 Port Ingress Lane Adapter %x (TopologyID 0x%lx) to"
      " DP OUT Adapter %d (TopologyID 0x%lx)\n",
      pRouter->UpstreamAdapter,
      pRouter->Header.TopologyID,
      pDpOut->Header.AdapterNum,
      pDpOut->Header.TopologyID
      );

    //
    // Buffer allocation
    //
    if ( IsUSB4Router (pRouter) ) {
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pRouter->pAdapterUFP,
                 NumPairedAdapters,
                 pRouter->BufferAllocationMinDPAux,
                 pRouter->BufferAllocationMinDPMain,
                 TRUE);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
        return Status;
      }

    } else {
      if (Tbt3RouterType (pRouter) == TBT3_TYPE_AR) {
        BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pRouter->pAdapterUFP)->LaneAdp_CurrentWidth ? 72 : 28;
      } else if (Tbt3RouterType (pRouter) == TBT3_TYPE_TR) {
        BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pRouter->pAdapterUFP)->LaneAdp_CurrentWidth ? 56 : 26;
      } else {
        IDS_HDT_CONSOLE_USB4_ERROR (" Unknown Router type: %x.\n", Tbt3RouterType (pRouter));
        return EFI_DEVICE_ERROR;
      }
      IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pRouter->pAdapterUFP,
                 NumPairedAdapters,
                 1,
                 BufferAllocationDpMain,
                 TRUE
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer.\n");
        return Status;
      }
    }

    if ( IsUSB4Router (pRouter) ) {
      PathCreditsAllocated = 0;
    } else {
      PathCreditsAllocated = BufferAllocationDpMain;
    }

    //
    // Bandwidth allocation
    //
    DpBandwidthAllocation (DpPathBandwidth, (AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP, NumPairedAdapters);

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pRouter->pAdapterUFP,                      // *pIngressAdapter
               (UINT32) DpMainPathHopId,                                             // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pDpOut,                                    // *pEgressAdapter
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_OUTPUT_HOPID,                     // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) 0,                                                           // PathCreditsAllocated
               //(UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_WEIGHT,                           // Weight
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_PRIORITY,                         // Priority
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_IFC,                              // IngressFlowControlFlag
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_ISE,                              // IngresSharedBufferingEnableFlag
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_EFC,                              // EgressFlowControlFlag
               (UINT32) DP_MAIN_LANE_TO_DP_ADAPTER_ESE                               // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    if (NumPairedAdapters == 0) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpMainTunnel1On = 1;
    } else if (NumPairedAdapters == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpMainTunnel2On = 1;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "8. DP Main Segment: USB4 Port Ingress Lane Adapter to DP OUT Adapter Status = %r\n",
      Status
      );

    //
    // 9. DP IN Aux Segment: USB4 Port Ingress to DP OUT Adapter
    // Establish DP IN Aux Path in DP OUT Adapter's Parent Router
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "9. DP IN Aux Segment: USB4 Port Ingress Lane Adapter %x (TopologyID 0x%lx) to"
      " DP OUT Adapter %d (TopologyID 0x%lx)\n",
      pRouter->UpstreamAdapter,
      pRouter->Header.TopologyID,
      pDpOut->Header.AdapterNum,
      pDpOut->Header.TopologyID
      );

    if ( IsUSB4Router (pRouter) ) {
      EFCFlag = (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_EFC;
      PathCreditsAllocated = (UINT32) pRouter->BufferAllocationMinDPAux;
    } else {
      PathCreditsAllocated = 1;
      EFCFlag = 1;
    }

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pRouter->pAdapterUFP,                      // *pIngressAdapter
               (UINT32) DpInAuxPathHopId,                                            // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pDpOut,                                    // *pEgressAdapter
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_OUTPUT_HOPID,                      // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_WEIGHT,                            // Weight
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_PRIORITY,                          // Priority
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_IFC,                               // IngressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_ISE,                               // IngresSharedBufferingEnableFlag
               (UINT32) EFCFlag,                                                     // EgressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_ESE                                // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    if (NumPairedAdapters == 0) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpAuxInTunnel1On = 1;
    } else if (NumPairedAdapters == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpAuxInTunnel2On = 1;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "9. DP IN Aux Segment: USB4 Port Ingress Lane Adapter to DP OUT Adapter Status = %r\n",
      Status
      );
  } else if ((pRouter->Header.TopologyID != pDpIn->Header.TopologyID)
    && (pRouter->Header.TopologyID != pDpOut->Header.TopologyID))
  {
    //
    // 2. DP OUT Aux Segment: USB4 Port Ingress to USB4 Port Egress
    // Establish DP OUT Aux Path in Pass-through USB4 Hub
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "2. DP OUT Aux Segment: USB4 Port Ingress Lane Adapter %x (TopologyID 0x%lx) to"
      " USB4 Port Egress Lane Adapter %d (TopologyID 0x%lx)\n",
      pIngressAdapter->AdapterNum,
      pIngressAdapter->TopologyID,
      pRouter->UpstreamAdapter,
      pRouter->Header.TopologyID
      );

    //
    // The only requirement for a DP path is that they terminate with a DP IN and DP OUT adapter
    // For Pre-OS CM, we are assuming DP IN will be in a path following DP OUT's Topology ID.
    //
    if (pRouter->IsHost) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Reached Host router without locating DP IN Adapter\n");
      return EFI_UNSUPPORTED;
    }

    //
    // Buffer allocation
    //
    if ( IsUSB4Router (pRouter) ) {
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pIngressAdapter,
                 NumPairedAdapters,
                 pRouter->BufferAllocationMinDPAux,
                 pRouter->BufferAllocationMinDPMain,
                 FALSE);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
        return Status;
      }
    } else {
      if (Tbt3RouterType (pRouter) == TBT3_TYPE_AR) {
        BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pRouter->pAdapterUFP)->LaneAdp_CurrentWidth ? 72 : 28;
      } else if (Tbt3RouterType (pRouter) == TBT3_TYPE_TR) {
        BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pRouter->pAdapterUFP)->LaneAdp_CurrentWidth ? 56 : 26;
      } else {
        IDS_HDT_CONSOLE_USB4_ERROR (" Unknown Router type: %x.\n", Tbt3RouterType (pRouter));
        return EFI_DEVICE_ERROR;
      }
      IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pIngressAdapter,
                 NumPairedAdapters,
                 1,
                 BufferAllocationDpMain,
                 FALSE
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer.\n");
        return Status;
      }
    }

    if ( IsUSB4Router (pRouter) ) {
      PathCreditsAllocated = (UINT32) pRouter->BufferAllocationMinDPAux;
    } else {
      PathCreditsAllocated = 1;
    }

    //
    // Bandwidth allocation
    //
    DpBandwidthAllocation (DpPathBandwidth, (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter, NumPairedAdapters);

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               pIngressAdapter,                                                      // *pIngressAdapter
               (UINT32) DpOutAuxPathHopId,                                           // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pRouter->pAdapterUFP,                      // *pEgressAdapter
               (UINT32) DpOutAuxPathHopId,                                           // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_AUX_LANE_TO_LANE_WEIGHT,                                  // Weight
               (UINT32) DP_AUX_LANE_TO_LANE_PRIORITY,                                // Priority
               (UINT32) DP_AUX_LANE_TO_LANE_IFC,                                     // IngressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_LANE_ISE,                                     // IngresSharedBufferingEnableFlag
               (UINT32) DP_AUX_LANE_TO_LANE_EFC,                                     // EgressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_LANE_ESE                                      // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    if (NumPairedAdapters == 0) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpAuxOutTunnel1On = 1;
    } else if (NumPairedAdapters == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpAuxOutTunnel2On = 1;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "2. DP OUT Aux Segment: USB4 Port Ingress Lane Adapter to USB4 Port Egress Lane Adapter Status = %r\n",
      Status
      );

    //
    // Continue recursively enumerating Path on Pass-through USB4 Hub until DP IN Adapter's Router is located
    //
    pNextIngressAdapter = (AMD_USB4_CM_CELL_HEADER *) pRouter->Header.Parent;

    Status = DpPathSetup (pCM, pDpIn, pDpOut, pNextIngressAdapter, NumPairedAdapters, DpPathBandwidth);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Failed to traverse path and locate DP IN Adapter. Current router: %x\n",
        pRouter->Header.TopologyID
        );
      return EFI_DEVICE_ERROR;
    }

    //
    // Recursive traversal to DP IN Adapter is complete.
    // Establish DP Main Path and DP IN Aux Patch, and then pop off stack to return to DP OUT Adapter
    //

    //
    // 6. DP Main Segment: USB4 Port Ingress to USB4 Port Egress
    // Establish DP Main Path in Pass-through USB4 Hub
    // pIngressAdapter becomes the Egress adapter as we pop off the stack
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "6. DP Main Segment: USB4 Port Ingress Lane Adapter %x (TopologyID 0x%lx) to"
      " USB4 Port Egress Lane Adapter Lane Adapter %d (TopologyID 0x%lx)\n",
      pRouter->UpstreamAdapter,
      pRouter->Header.TopologyID,
      pIngressAdapter->AdapterNum,
      pIngressAdapter->TopologyID
      );

    //
    // Buffer allocation
    //
    if ( IsUSB4Router (pRouter) ) {
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pRouter->pAdapterUFP,
                 NumPairedAdapters,
                 pRouter->BufferAllocationMinDPAux,
                 pRouter->BufferAllocationMinDPMain,
                 TRUE);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
        return Status;
      }
    } else {
      if (Tbt3RouterType (pRouter) == TBT3_TYPE_AR) {
        BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pRouter->pAdapterUFP)->LaneAdp_CurrentWidth ? 72 : 28;
      } else if (Tbt3RouterType (pRouter) == TBT3_TYPE_TR) {
        BufferAllocationDpMain = ((AMD_USB4_CM_LANE_ADAPTER*) pRouter->pAdapterUFP)->LaneAdp_CurrentWidth ? 56 : 26;
      } else {
        IDS_HDT_CONSOLE_USB4_ERROR (" Unknown Router type: %x.\n", Tbt3RouterType (pRouter));
        return EFI_DEVICE_ERROR;
      }
      IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pRouter->pAdapterUFP,
                 NumPairedAdapters,
                 1,
                 BufferAllocationDpMain,
                 TRUE
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer.\n");
        return Status;
      }
    }

    if ( IsUSB4Router (pRouter) ) {
      PathCreditsAllocated = 0;
    } else {
      PathCreditsAllocated = BufferAllocationDpMain;
    }

    //
    // Bandwidth allocation
    //
    DpBandwidthAllocation (DpPathBandwidth, (AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP, NumPairedAdapters);

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pRouter->pAdapterUFP,                      // *pIngressAdapter
               (UINT32) DpMainPathHopId,                                             // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pIngressAdapter,                           // *pEgressAdapter
               (UINT32) DpMainPathHopId,                                             // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) 0,                                                           // PathCreditsAllocated
               //(UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_MAIN_LANE_TO_LANE_WEIGHT,                                 // Weight
               (UINT32) DP_MAIN_LANE_TO_LANE_PRIORITY,                               // Priority
               (UINT32) DP_MAIN_LANE_TO_LANE_IFC,                                    // IngressFlowControlFlag
               (UINT32) DP_MAIN_LANE_TO_LANE_ISE,                                    // IngresSharedBufferingEnableFlag
               (UINT32) DP_MAIN_LANE_TO_LANE_EFC,                                    // EgressFlowControlFlag
               (UINT32) DP_MAIN_LANE_TO_LANE_ESE                                     // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    if (NumPairedAdapters == 0) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpMainTunnel1On = 1;
    } else if (NumPairedAdapters == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpMainTunnel2On = 1;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "6. DP Main Segment: USB4 Port Ingress Lane Adapter to USB4 Port Egress Lane Adapter Status = %r\n",
      Status
      );

    //
    // Establish DP IN Aux Path in Pass-through USB4 Hub
    // 7. DP IN Aux Segment: USB4 Port Ingress to USB4 Port Egress
    // pIngressAdapter becomes the Egress adapter as we pop off the stack
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "7. DP IN Aux Segment: USB4 Port Ingress Lane Adapter %x (TopologyID 0x%lx) to"
      " USB4 Port Egress Lane Adapter %d (TopologyID 0x%lx)\n",
      pRouter->UpstreamAdapter,
      pRouter->Header.TopologyID,
      pIngressAdapter->AdapterNum,
      pIngressAdapter->TopologyID
      );

    if ( IsUSB4Router (pRouter) ) {
      PathCreditsAllocated = (UINT32) pRouter->BufferAllocationMinDPAux;
    } else {
      PathCreditsAllocated = 1;
    }

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pRouter->pAdapterUFP,                      // *pIngressAdapter
               (UINT32) DpInAuxPathHopId,                                            // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pIngressAdapter,                           // *pEgressAdapter
               (UINT32) DpInAuxPathHopId,                                            // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_AUX_LANE_TO_LANE_WEIGHT,                                  // Weight
               (UINT32) DP_AUX_LANE_TO_LANE_PRIORITY,                                // Priority
               (UINT32) DP_AUX_LANE_TO_LANE_IFC,                                     // IngressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_LANE_ISE,                                     // IngresSharedBufferingEnableFlag
               (UINT32) DP_AUX_LANE_TO_LANE_EFC,                                     // EgressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_LANE_ESE                                      // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("DisplayPort path could not be configured.\n");
      return Status;
    }

    if (NumPairedAdapters == 0) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpAuxInTunnel1On = 1;
    } else if (NumPairedAdapters == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP)->DpAuxInTunnel2On = 1;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "7. DP IN Aux Segment: USB4 Port Ingress Lane Adapter to USB4 Port Egress Lane Adapter Status = %r\n",
      Status
      );
  } else if (pRouter->Header.TopologyID == pDpIn->Header.TopologyID) {
    //
    // 3. DP OUT Aux Segment: USB4 Port Ingress to DP IN Adapter
    // Establish DP OUT Aux Path in DP IN Adapter's Parent Router
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "3. DP OUT Aux Segment: USB4 Port Ingress Lane Adapter %x (TopologyID 0x%lx) to"
      " DP IN Adapter %d (TopologyID 0x%lx)\n",
      pIngressAdapter->AdapterNum,
      pIngressAdapter->TopologyID,
      pDpIn->Header.AdapterNum,
      pDpIn->Header.TopologyID
      );

    //
    // Buffer allocation
    //
    if ( IsUSB4Router (pRouter) ) {
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pIngressAdapter,
                 NumPairedAdapters,
                 pRouter->BufferAllocationMinDPAux,
                 pRouter->BufferAllocationMinDPMain,
                 FALSE);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer.\n");
        return Status;
      }
    } else {
      IDS_HDT_CONSOLE_USB4_INFO (" BufferAllocationDpMain: %x.\n", BufferAllocationDpMain);
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pIngressAdapter,
                 NumPairedAdapters,
                 1,
                 12,
                 FALSE
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
        return Status;
      }
    }

    if ( IsUSB4Router (pRouter) ) {
      EFCFlag = (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_EFC;
      PathCreditsAllocated = (UINT32) pRouter->BufferAllocationMinDPAux;
    } else {
      PathCreditsAllocated = 1;
      EFCFlag = 1;
    }

    //
    // Bandwidth allocation
    //
    DpBandwidthAllocation (DpPathBandwidth, (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter, NumPairedAdapters);

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               pIngressAdapter,                                                      // *pIngressAdapter
               (UINT32) DpOutAuxPathHopId,                                           // InputHopID
               (AMD_USB4_CM_CELL_HEADER*) pDpIn,                                     // *pEgressAdapter
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_OUTPUT_HOPID,                      // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_WEIGHT,                            // Weight
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_PRIORITY,                          // Priority
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_IFC,                               // IngressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_ISE,                               // IngresSharedBufferingEnableFlag
               (UINT32) EFCFlag,                                                     // EgressFlowControlFlag
               (UINT32) DP_AUX_LANE_TO_DP_ADAPTER_ESE                                // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    if (NumPairedAdapters == 0) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpAuxOutTunnel1On = 1;
    } else if (NumPairedAdapters == 1) {
      ((AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter)->DpAuxOutTunnel2On = 1;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "3. DP OUT Aux Segment: USB4 Port Ingress Lane Adapter to DP IN Adapter Status = %r\n",
      Status
      );

    //
    // Recursive traversal to DP IN Adapter is complete.
    // Establish DP Main Path and DP IN Aux Patch, and then pop off stack to return to DP OUT Adapter
    //

    //
    // 4. DP Main Segment: DP IN Adapter to USB4 Port Egress
    // Establish DP Main Path in DP IN Parent Router
    // pIngressAdapter becomes the Egress adapter as we pop off the stack
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "4. DP Main Segment: DP IN Adapter %x (TopologyID 0x%lx) to"
      " USB4 Port Egress Lane Adapter %d (TopologyID 0x%lx)\n",
      pDpIn->Header.AdapterNum,
      pDpIn->Header.TopologyID,
      pIngressAdapter->AdapterNum,
      pIngressAdapter->TopologyID
      );

    if ( IsUSB4Router (pRouter) ) {
      IFCFlag = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_IFC;
      ISEFlag = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_ISE;
      PathCreditsAllocated = 0;
    } else {
      IFCFlag = 0;
      ISEFlag = 0;
      PathCreditsAllocated = 12;

      //
      // Buffer allocation
      //
      Status = DpBufferAllocation (
                 pCM,
                 (AMD_USB4_CM_CELL_HEADER *) pDpIn,
                 NumPairedAdapters,
                 4,
                 12,
                 TRUE
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_USB4_ERROR (" Failed to allocate buffer = %r\n", Status);
        return Status;
      }
    }

    Status = DpPathConfiguration (
               pCM,                                                                   // Global CM Structure
               pRouter->Header.TopologyID,                                            // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pDpIn,                                      // *pIngressAdapter
               (UINT32) DP_MAIN_DP_ADAPTER_TO_LANE_INPUT_HOPID,                       // InputHopID
               pIngressAdapter,                                                       // *pEgressAdapter
               (UINT32) DpMainPathHopId,                                              // OutputHopID
               (UINT32) 1,                                                            // Valid
               (UINT32) 0,                                                            // PathCreditsAllocated
               //(UINT32) PathCreditsAllocated,                                         // PathCreditsAllocated
               (UINT32) 0,                                                            // CounterEnable
               (UINT32) DP_MAIN_DP_ADAPTER_TO_LANE_WEIGHT,                            // Weight
               (UINT32) DP_MAIN_DP_ADAPTER_TO_LANE_PRIORITY,                          // Priority
               (UINT32) IFCFlag,                                                      // IngressFlowControlFlag
               (UINT32) ISEFlag,                                                      // IngresSharedBufferingEnableFlag
               (UINT32) DP_MAIN_DP_ADAPTER_TO_LANE_EFC,                               // EgressFlowControlFlag
               (UINT32) DP_MAIN_DP_ADAPTER_TO_LANE_ESE                                // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    pDpIn->MainTunnelOn = 1;

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "4. DP Main Segment: DP IN Adapter to USB4 Port Egress Lane Adapter Status = %r\n",
      Status);

    //
    // 5. DP IN Aux Segment: DP IN Adapter to USB4 Port Egress
    // Establish DP IN Aux Path in DP IN Parent Router
    // pIngressAdapter becomes the Egress adapter as we pop off the stack
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "5. DP IN Aux Segment: DP IN Adapter %x (TopologyID 0x%lx) to"
      " USB4 Port Egress Lane Adapter %d (TopologyID 0x%lx)\n",
      pDpIn->Header.AdapterNum,
      pDpIn->Header.TopologyID,
      pIngressAdapter->AdapterNum,
      pIngressAdapter->TopologyID
      );

    if ( IsUSB4Router (pRouter) ) {
      IFCFlag = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_IFC;
      ISEFlag = (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_ISE;
      PathCreditsAllocated = 0;
    } else {
      IFCFlag = 1;
      ISEFlag = 0;
      PathCreditsAllocated = 4;
    }

    Status = DpPathConfiguration (
               pCM,                                                                  // Global CM Structure
               pRouter->Header.TopologyID,                                           // TopologyID
               (AMD_USB4_CM_CELL_HEADER*) pDpIn,                                     // *pIngressAdapter
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_INPUT_HOPID,                       // InputHopID
               pIngressAdapter,                                                      // *pEgressAdapter
               (UINT32) DpInAuxPathHopId,                                            // OutputHopID
               (UINT32) 1,                                                           // Valid
               (UINT32) PathCreditsAllocated,                                        // PathCreditsAllocated
               (UINT32) 0,                                                           // CounterEnable
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_WEIGHT,                            // Weight
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_PRIORITY,                          // Priority
               (UINT32) IFCFlag,                                                     // IngressFlowControlFlag
               (UINT32) ISEFlag,                                                     // IngresSharedBufferingEnableFlag
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_EFC,                               // EgressFlowControlFlag
               (UINT32) DP_AUX_DP_ADAPTER_TO_LANE_ESE                                // EgressSharedBufferingEnableFlag
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" DisplayPort path could not be configured.\n");
      return Status;
    }

    pDpIn->AuxInTunnelOn = 1;

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "5. DP IN Aux Segment: DP IN Adapter to USB4 Port Egress Lane Adapter Status = %r\n",
      Status
      );
  }

  return EFI_SUCCESS;
}


UINT32
FindCap (
  IN     AMD_USB4_CM                          *pCM,
  IN OUT AMD_USB4_CM_DP_ADAPTER               *pAdapter,
  IN     UINT8                                CapPtr,
  IN     UINT32                               RequestCapID
  )
{
  EFI_STATUS  Status;
  UINT32      CapabilityPointer;
  UINT32      ReturnCapPtr;
  UINT32      *CapabilityReg;
  UINT32      DataSize;
  UINT8       NextCapabilityPointer;
  UINT8       CapabilityID;
  BOOLEAN     IsFindCap;

  Status                = EFI_SUCCESS;
  CapabilityPointer     = (UINT32)CapPtr;
  CapabilityReg         = NULL;
  DataSize              = 0;
  NextCapabilityPointer = 0;
  CapabilityID          = 0;
  IsFindCap             = FALSE;
  ReturnCapPtr          = 0;


  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start. Find CapID 0x%x\n", __FUNCTION__, RequestCapID);

  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               pAdapter->Header.AdapterNum,                         // AdapterNum
               CapabilityPointer,                                   // ReadAddress
               1,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Error reading capabilities\n", __FUNCTION__);
      break;
    }

    NextCapabilityPointer = (UINT8)(*CapabilityReg);
    CapabilityID  = (UINT8)((*CapabilityReg) >> 8);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " Router 0x%lx Adp %d CapabilityID = 0x%x, NextCapabilityPointer = 0x%x\n",
      pAdapter->Header.TopologyID,
      pAdapter->Header.AdapterNum,
      CapabilityID,
      NextCapabilityPointer
      );

    if ( CapabilityID == RequestCapID ) {
      IDS_HDT_CONSOLE_USB4_DEBUG (" Find Cap ID.\n");
      IsFindCap = TRUE;
      ReturnCapPtr = CapabilityPointer;
      //break;
    }

    CapabilityPointer = NextCapabilityPointer;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a End.\n", __FUNCTION__);
  return IsFindCap ? ReturnCapPtr : 0;
}


/**
  Enumerate DisplayPort Adapter.

  @param[in]  *pCM              Pointer to global AMD_USB4_CM structure.
  @param[in]  AdapterNum        Adapter number to be read.
  @param[in]  pRouter           Pointer to global AMD_USB4_CM_ROUTER structure.
  @param[in]  pAdapter          Pointer to global AMD_USB4_CM_ADAPTER structure.
  @param[in]  *pAdapterCs       Pointer to Adapter Configuration Space data

  @param[out]  *pRouter         Pointer to global AMD_USB4_CM_ROUTER structure.
  @param[out]  *pAdapter        Pointer to global AMD_USB4_CM_ADAPTER structure.

  @retval EFI_SUCCESS           Successfully enumerated DP Adapter.
  @retval EFI_INVALID_PARAMETER Failure in receiving valid parameters. One or more parameters are NULL.
  @retval EFI_DEVICE_ERROR      Failure in reading Configuration Space.
**/
EFI_STATUS
EnumerateDpAdapter (
  IN     AMD_USB4_CM                          *pCM,
  IN     UINT32                               AdapterNum,
  IN OUT AMD_USB4_CM_ROUTER                   *pRouter,
  IN OUT AMD_USB4_CM_DP_ADAPTER               *pAdapter,
  IN     AMD_USB4_ADAPTER_CONFIGURATION_SPACE *pAdapterCs
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  UINT32                                *DataPtr;
  UINT32                                CompletionMetadata;
  UINT32                                *RouterOpData;
  UINT32                                CompletionStatus;
  UINT8                                 CapabilityPointer;
  UINT8                                 CapabilityID;
  UINT32                                *CapabilityReg;
  UINT8                                 NextCapabilityPointer;

  Status                 = EFI_SUCCESS;
  DataSize               = 0;
  DataPtr                = NULL;
  CompletionMetadata     = 0;
  RouterOpData           = NULL;
  CompletionStatus       = 0;
  CapabilityPointer      = 0;
  CapabilityReg          = NULL;
  CapabilityID           = 0;
  NextCapabilityPointer  = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ((pCM == NULL) || (pRouter == NULL) || (pAdapter == NULL) || (pAdapterCs == NULL)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  //no need SetMem (pAdapter, sizeof (AMD_USB4_CM_DP_ADAPTER), 0);

  if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == DPInAdapter ) {
    pAdapter->Header.CellType = CELL_TYPE_DP_IN_ADAPTER;
  } else {
    pAdapter->Header.CellType = CELL_TYPE_DP_OUT_ADAPTER;
  }
  pAdapter->Header.TopologyID = pRouter->Header.TopologyID;
  pAdapter->Header.AdapterNum = (UINT8)AdapterNum;
  pAdapter->Header.Parent     = (AMD_USB4_CM_CELL_HEADER*)pRouter;

  pAdapter->AdpHeader.MaxInputHopId  = (UINT16)(pAdapterCs->AdpCs5.Field.MaxInputHopID);
  pAdapter->AdpHeader.MaxOutputHopId = (UINT16)(pAdapterCs->AdpCs5.Field.MaxOutputHopID);

  pAdapter->AdapterState             = ADAPTER_STATE_PRESENT;

  CapabilityPointer = (UINT8) pAdapterCs->AdpCs1.Field.NextCapabilityPointer;
  while (CapabilityPointer != 0) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               AdapterNum,                                          // AdapterNum
               CapabilityPointer,                                   // ReadAddress
               1,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Error reading capabilities\n");
      return EFI_DEVICE_ERROR;
    }

    NextCapabilityPointer  = (UINT8)(*CapabilityReg);
    CapabilityID  = (UINT8)((*CapabilityReg) >> 8);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " CapabilityID = 0x%x, NextCapabilityPointer = 0x%x\n",
      CapabilityID,
      NextCapabilityPointer
      );
    switch (CapabilityID) {
      // DP_IN_ADAPTER_CONFIG_CAP_ID and DP_OUT_ADAPTER_CONFIG_CAP_ID are both 0x4
      case (DP_IN_ADAPTER_CONFIG_CAP_ID):
        IDS_HDT_CONSOLE_USB4_DEBUG ("Get DP Adapter Configuration Capability (0x%x)\n", CapabilityPointer);
        pAdapter->CapabilityOffset = CapabilityPointer;
        //
        // Update pointer to read Adapter Configuration Capability
        //
        DataSize = sizeof (AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
        Status = AmdUsb4Ring0ReadCS (
                   pCM,
                   pRouter->Header.TopologyID,
                   CS_TARGET_ADAPTER,
                   AdapterNum,
                   CapabilityPointer,
                   DataSize,
                   (UINT32**)&DataPtr,
                   &DataSize
                   );
        if ((EFI_ERROR (Status))
          || ((sizeof (AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32)) != DataSize))
        {
          //IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read DP IN Adapter Configuration Capability Register.\n");
          IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read DP Adapter Capability Registers.\n");
          return EFI_DEVICE_ERROR;
        }

        pAdapter->HPDStatus = ((AMD_USB4_DP_OUT_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->AdpDpCs2.Field.HPDStatus;

        //
        // Retain relevant Adapter Configuration Capability information to be shared when pairing DP adapters
        //
        pAdapter->ProtocolAdapterVersion   =
        ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.ProtocolAdapterVersion;
        pAdapter->MaximalDPCDRev           =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.MaximalDPCDRev;
        pAdapter->MaximalLinkRate          =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.MaximalLinkRate;
        pAdapter->MaximalLaneCount         =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.MaximalLaneCount;
        pAdapter->MSTCapability            =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.MSTCapability;
        pAdapter->TPS3Capability           =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.TPS3Capability;
        pAdapter->TPS4Capability           =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.TPS4Capability;
        pAdapter->FECNotSupported          =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.FECNotSupported;
        pAdapter->SecondarySplitCapability =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.SecondarySplitCapability;
        pAdapter->LTTPRNotSupported        =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.LTTPRNotSupported;
        pAdapter->DSCNotSupported          =
          ((AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->DpLocalCap.Field.DSCNotSupported;
        break;

      case (TMU_ADAPTER_CONFIG_CAP_ID):
        if (!IsUSB4Router (pRouter)) {
          pAdapter->TBT3DpTmuCapabilityOffset = CapabilityPointer;
          Status = Tbt3DpTmuConfiguration (pCM, pRouter, pAdapter);
          if (EFI_ERROR (Status)) {
            return Status;
          }
        } else {
          IDS_HDT_CONSOLE_USB4_WARNING (
            "A TMU Adapter Configuration Capability should not be present for a DP Adapter in a USB4 Router.\n"
           );
        }
        break;

      case (VENDOR_SPECIFIC_CAP_ID):
        if (!IsUSB4Router (pRouter) && 1 == (((*CapabilityReg)>>16) & 0xF) ) {
          pAdapter->TBT3DpVSCCapabilityOffset = CapabilityPointer;
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "pAdapter->TBT3DpVSCCapabilityOffset = 0x%x\n",
            pAdapter->TBT3DpVSCCapabilityOffset
           );
        } else {
          IDS_HDT_CONSOLE_USB4_WARNING (
            "A VSC Capability should not be present for a DP Adapter in a USB4 Router.\n"
           );
        }
        break;

      default:
        IDS_HDT_CONSOLE_USB4_DEBUG ("The CapabilityID = 0x%x does not belong to DP Adapter. Skip it.\n", CapabilityID);
        break;
      }

      CapabilityPointer = NextCapabilityPointer;
    }

#if AMD_USB4_DEBUG_DP_HOTPLUG_DISABLE
  // Set Hot plug to disable
  DisableHotplugEvent (
    pCM,
    pRouter->Header.TopologyID,
    AdapterNum
    );
#endif

  //
  // Allocate DP adapter and increase DP adapter amount in Router CM Data Structure.
  //
  if ( pAdapter->Header.CellType == CELL_TYPE_DP_IN_ADAPTER ) {
    //
    // Detect DP Source and available resource for DP IN Adapter
    //

    /*
    Status = RouterOperation (
               pCM,
               pRouter,
               QUERY_DP_RESOURCE_AVAILABILITY,
               AdapterNum,
               &CompletionMetadata,
               &RouterOpData,
               &CompletionStatus
               );
    */

    Status = RouterOperation2 (
               pCM,
               pRouter,
               QUERY_DP_RESOURCE_AVAILABILITY,
               AdapterNum,
               0,
               NULL,
               &CompletionMetadata,
               &CompletionStatus,
               &RouterOpData
               );

    IDS_HDT_CONSOLE_USB4_DEBUG ("   Query Dp Resource Status: %r, ReturnStatus: 0x%x\n", Status, CompletionStatus);

    if ((!EFI_ERROR (Status)) && (CompletionStatus == 0)) {
      //
      // Update Adapter State and add to Plugged Linked List
      //
      pAdapter->AdapterState = ADAPTER_STATE_PLUGGED;
      InsertTailList (&pCM->DpInPluggedList, &pAdapter->Link);
      IDS_HDT_CONSOLE_USB4_DEBUG ("DP IN Adapter added to Plugged List\n");
    } else {
      IDS_HDT_CONSOLE_USB4_INFO (
        " No DP Source is detected and/or a resource is not available to be allocated to the DP IN Adapter.\n"
        );
    }

    pRouter->DpInAmount++;
  } else if ( pAdapter->Header.CellType == CELL_TYPE_DP_OUT_ADAPTER ) {
    //
    // Verify if DP Sink is connected
    //
    if (pAdapter->HPDStatus) {
      //
      // Update Adapter State and add to Plugged Linked List
      //
      pAdapter->AdapterState = ADAPTER_STATE_PLUGGED;
      InsertTailList (&pCM->DpOutPluggedList, &pAdapter->Link);
      IDS_HDT_CONSOLE_USB4_DEBUG ("DP OUT Adapter added to Plugged List\n");
    } else {
      IDS_HDT_CONSOLE_USB4_INFO (" No DP Sink is connected.\n");
    }

    pRouter->DpOutAmount++;
  }

  pAdapter->AdpHeader.pSiblingAdapter     = NULL;
  pAdapter->AdpHeader.pLinkPartner        = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("DisplayPort Adapter Information:\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Address                  = 0x%x\n", pAdapter);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TopologyID               = 0x%x\n", pAdapter->Header.TopologyID);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterNum               = 0x%x\n", pAdapter->Header.AdapterNum);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CellType                 = 0x%x\n", pAdapter->Header.CellType);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxInputHopId            = 0x%x\n", pAdapter->AdpHeader.MaxInputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxOutputHopId           = 0x%x\n", pAdapter->AdpHeader.MaxOutputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  ProtocolAdapterVersion   = 0x%x\n", pAdapter->ProtocolAdapterVersion);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaximalDPCDRev           = 0x%x\n", pAdapter->MaximalDPCDRev);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaximalLinkRate          = 0x%x\n", pAdapter->MaximalLinkRate);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaximalLaneCount         = 0x%x\n", pAdapter->MaximalLaneCount);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MSTCapability            = 0x%x\n", pAdapter->MSTCapability);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TPS3Capability           = 0x%x\n", pAdapter->TPS3Capability);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TPS4Capability           = 0x%x\n", pAdapter->TPS4Capability);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  FECNotSupported          = 0x%x\n", pAdapter->FECNotSupported);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  SecondarySplitCapability = 0x%x\n", pAdapter->SecondarySplitCapability);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  LTTPRNotSupported        = 0x%x\n", pAdapter->LTTPRNotSupported);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  DSCNotSupported          = 0x%x\n", pAdapter->DSCNotSupported);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterState             = 0x%x\n", pAdapter->AdapterState);

  return EFI_SUCCESS;
}


BOOLEAN
FindDpOutInList (
  IN     AMD_USB4_CM            *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER *pDpOutAdapter
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         FindDpOutInList;
  LIST_ENTRY                      *DpOutNode;
  AMD_USB4_CM_DP_ADAPTER          *pDpOut;

  Status            = EFI_SUCCESS;
  DpOutNode         = GetFirstNode (&pCM->DpOutPluggedList);
  pDpOut            = NULL;
  FindDpOutInList   = FALSE;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == DpOutNode ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" No Dp out device.\n");
  } else {
    do {
      pDpOut = AMD_USB4_CM_DP_ADAPTER_FROM_ENTRY (DpOutNode);
      if ( pDpOut->Header.TopologyID == pDpOutAdapter->Header.TopologyID
        && pDpOut->Header.AdapterNum == pDpOutAdapter->Header.AdapterNum ) {
        IDS_HDT_CONSOLE_USB4_DEBUG (" Find same Dp out adapter. No need to continue.\n");
        FindDpOutInList = TRUE;
        break;
      }
      DpOutNode = GetNextNode (&pCM->DpOutPluggedList, DpOutNode);
    } while (!IsNull (&pCM->DpOutPluggedList, DpOutNode));
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a End with Return value 0x%x\n", __FUNCTION__, FindDpOutInList);

  return FindDpOutInList;
}


/**
  Process Dp out Hotplug event.

  @param[in]  *pCM              Pointer to global AMD_USB4_CM structure.
  @param[out]  *pAdapter        Pointer to global AMD_USB4_CM_ADAPTER structure.
**/
VOID
ProcessDpOutHotplug (
  IN     AMD_USB4_CM            *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER *pAdapter
  )
{
  EFI_STATUS                Status;
  UINT32                    DataSize;

  AMD_USB4_DP_OUT_ADAPTER_CONFIGURATION_CAPABILITY  *AdpDpCs;

  Status    = EFI_SUCCESS;
  AdpDpCs   = NULL;
  DataSize  = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( pAdapter->Header.CellType != CELL_TYPE_DP_OUT_ADAPTER ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" This is not DP out adapter.\n");
    return;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (" Read DP out CS.\n");
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pAdapter->Header.AdapterNum,
             pAdapter->CapabilityOffset,
             sizeof (AMD_USB4_DP_OUT_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32),
             (UINT32**)&AdpDpCs,
             &DataSize
             );
  IDS_HDT_CONSOLE_USB4_DEBUG (" Read DP out CS Status=%r, DataSize=%d.\n", Status, DataSize);
  if ( EFI_SUCCESS == Status
    && ( sizeof (AMD_USB4_DP_OUT_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) == DataSize ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" AdpDpCs2->Field.HPDStatus=0x%x\n", AdpDpCs->AdpDpCs2.Field.HPDStatus);
    if ( AdpDpCs->AdpDpCs2.Field.HPDStatus ) {
      if ( FALSE == FindDpOutInList (pCM, pAdapter) ) {
        pAdapter->AdapterState             = ADAPTER_STATE_PLUGGED;
        pAdapter->ProtocolAdapterVersion   = AdpDpCs->DpLocalCap.Field.ProtocolAdapterVersion;
        pAdapter->MaximalDPCDRev           = AdpDpCs->DpLocalCap.Field.MaximalDPCDRev;
        pAdapter->MaximalLinkRate          = AdpDpCs->DpLocalCap.Field.MaximalLinkRate;
        pAdapter->MaximalLaneCount         = AdpDpCs->DpLocalCap.Field.MaximalLaneCount;
        pAdapter->MSTCapability            = AdpDpCs->DpLocalCap.Field.MSTCapability;
        pAdapter->TPS3Capability           = AdpDpCs->DpLocalCap.Field.TPS3Capability;
        pAdapter->FECNotSupported          = AdpDpCs->DpLocalCap.Field.FECNotSupported;
        pAdapter->SecondarySplitCapability = AdpDpCs->DpLocalCap.Field.SecondarySplitCapability;
        pAdapter->LTTPRNotSupported        = AdpDpCs->DpLocalCap.Field.LTTPRNotSupported;
        pAdapter->DSCNotSupported          = AdpDpCs->DpLocalCap.Field.DSCNotSupported;
        InsertTailList (&pCM->DpOutPluggedList, &pAdapter->Link);
        IDS_HDT_CONSOLE_USB4_DEBUG (" Add DP adapter %d to List.\n", pAdapter->Header.AdapterNum);
      } else {
        IDS_HDT_CONSOLE_USB4_DEBUG (" DP adapter %d is already in List.\n", pAdapter->Header.AdapterNum);
      }
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG (" No DP Sink is connected to DP adapter %d.\n", pAdapter->Header.AdapterNum);
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a End\n", __FUNCTION__);
}


/**
  Go through Hotplug queue.
  Re-read DP OUT adapter register value If there is hot plug-in event.

  @param[in]  *pCM              Pointer to global AMD_USB4_CM structure.
**/
VOID
PreEnableDpTunneling (
  IN     AMD_USB4_CM    *pCM
  )
{
  EFI_STATUS        Status;
  UINT32            i;
  AMD_USB4_CM_CELL  *PtrCell;

  Status  = EFI_SUCCESS;
  i       = 0;
  PtrCell = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  for (i = 0; i < MAX_HP_QUEUE_SIZE; i++) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Hot plug queue [%d] RouterHigh 0x%x RouterLow 0x%x AdapterNum %d, UPG %d.\n",
      i,
      MASK_CM_BIT (pCM->HotPlugQueue[0][i].TopologyIDHigh),
      pCM->HotPlugQueue[0][i].TopologyIDLow,
      pCM->HotPlugQueue[0][i].AdapterNumber,
      pCM->HotPlugQueue[0][i].UPG
      );
    if ( pCM->HotPlugQueue[0][i].Valid == 1 && pCM->HotPlugQueue[0][i].UPG == 0 ) {
      Status = AmdUsb4FindAdapterByAdpNum (
                 pCM,
                 MASK_CM_BIT (pCM->HotPlugQueue[0][i].TopologyIDHigh),
                 pCM->HotPlugQueue[0][i].TopologyIDLow,
                 pCM->HotPlugQueue[0][i].AdapterNumber,
                 &PtrCell
                 );
      if ( Status == EFI_SUCCESS && PtrCell->Header.CellType == CELL_TYPE_DP_OUT_ADAPTER ) {
        ProcessDpOutHotplug (
          pCM,
          (AMD_USB4_CM_DP_ADAPTER*)PtrCell
          );
      }
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a End\n", __FUNCTION__);
}

