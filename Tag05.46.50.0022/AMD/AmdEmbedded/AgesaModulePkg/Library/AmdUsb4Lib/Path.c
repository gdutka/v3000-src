/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>
#include "Library/AmdUsb4Lib.h"

#define FILECODE LIBRARY_AMDUSB4LIB_PATH_FILECODE


#define MAX_USB4_CM_STACK_NUM 64

//
// DisplayPort Required Bandwidth
// Values based on Table 10-7 in USB4 Spec, however manual calculations are used
// Values multipled by USB4_RAW_BANDWIDTH_SCALING
//
CONST UINT32 DpRequiredBandwidth[4][3] = {
                                          {1296, 2592, 5184},
                                          {2160, 4320, 8640},
                                          {4320, 8640, 17280},
                                          {6480, 12960, 25920}
                                         };

#define CR(Record, TYPE, Field)  ((TYPE *) ((CHAR8 *) (Record) - (CHAR8 *) &(((TYPE *) 0)->Field)))
#define AMD_USB4_CM_DP_ADAPTER_FROM_ENTRY(a)               CR (a, AMD_USB4_CM_DP_ADAPTER, Link);

/**
 * Teardown Path tunnel.

 * @param[in]  *pCM                Pointer to global AMD_USB4_CM structure.
 * @param[in]  *pIngressAdapter    Pointer to Adapter or LaneIngress Adapter.
 * @param[in]  InputHopID          Input Hop ID.

 * @param[out] *pIngressAdapter    Pointer to Adapter or LaneIngress Adapter.

 * @retval EFI_NOT_READY           Something wrong in Transmit or Receive routine.
 * @retval EFI_NO_RESPONSE         Send out WRITE_REQUEST packet, but do not get paired WRITE_RESPONSE or NOTIFICATION.
 * @retval EFI_INVALID_PARAMETER   Input parameter is not good.
 * @retval EFI_DEVICE_ERROR        HW error.
**/
EFI_STATUS
PathTeardown (
  IN      AMD_USB4_CM       *pCM,
  IN OUT AMD_USB4_CM_CELL   *pIngressAdapter,
  IN     UINT32             InputHopID
  )
{
  EFI_STATUS                Status;
  AMD_USB4_PATHN_CS_0       AdapterPathNCs0And;
  AMD_USB4_PATHN_CS_0       AdapterPathNCs0Or;
  UINT32                    DataSize;
  UINT32                    *DataPtr;

  Status                    = EFI_SUCCESS;
  AdapterPathNCs0And.Value  = 0x0;
  AdapterPathNCs0Or.Value   = 0x0;
  DataSize                  = 0;
  DataPtr                   = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  //
  // 1. Teardown Adapter Tunneling Path
  //

  // 1.1 Clear Ingress Adapter Path PATH_CS_0.Valid bit[31]
  AdapterPathNCs0And.Value = 0x7FFFFFFF;
  AdapterPathNCs0Or.Value = 0x00000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Disable Router [TopologyID: 0x%lx] Adapter [%d] Path."
    " AdapterPathNCs0And=0x%lx, AdapterPathNCs0Or=0x%lx\n",
    pIngressAdapter->Header.TopologyID,
    pIngressAdapter->Header.AdapterNum,
    *((UINT64*)(&AdapterPathNCs0And)),
    *((UINT64*)(&AdapterPathNCs0Or))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pIngressAdapter->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pIngressAdapter->Header.AdapterNum,                  // AdapterNum
             InputHopID<<1,                                       // Address
             1,                                                   // SizeInDW
             (UINT32*)&(AdapterPathNCs0And),                      // AndMask
             (UINT32*)&(AdapterPathNCs0Or)                        // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  //
  // Poll AMD_USB4_PATHN_CS_1.Pending Packets until cleared to ensure all Path's Transport Layer Packets have cleared
  //
  do {
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pIngressAdapter->Header.TopologyID,
               CS_TARGET_PATH,                            // CsType
               pIngressAdapter->Header.AdapterNum,        // AdapterNum
               (InputHopID<<1) + 1,                       // ReadAddress
               DataSize,                                  // ReadSize
               (UINT32**)&DataPtr,                        // DataOut
               &DataSize                                  // DataSize
               );

    if ((EFI_ERROR (Status)) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        "Polling AMD_USB4_PATHN_CS_1.Pending Packets - Failed to read AMD_USB4_PATHN_CS_1 Register.\n"
        );
      return EFI_DEVICE_ERROR;
    }
  } while ((((AMD_USB4_PATHN_CS_1 *)DataPtr)->Field.PendingPackets != 0));

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Disable Path for USB3 tunneling
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
DisableUsb3Tunneling (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pParentUsb3DownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_USB3_ADAPTER  *pChildAdapterUsb3Up;
  AMD_USB4_ADP_USB3_CS_0    Usb3AdpCapCs0And;
  AMD_USB4_ADP_USB3_CS_0    Usb3AdpCapCs0Or;

  Status                    = EFI_SUCCESS;

  if (pChildRouter != NULL) {
    pChildAdapterUsb3Up     = (AMD_USB4_CM_USB3_ADAPTER*)(pChildRouter->pAdapterUsb3Up);
  } else {
    pChildAdapterUsb3Up     = NULL;
  }

  Usb3AdpCapCs0And.Value    = 0x0;
  Usb3AdpCapCs0Or.Value     = 0x0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  //
  // 1. Disconnect and Disable Transport Layer Packets on USB3 Source and Destination adapters
  //

  // 1.1 Child Router USB3 Upstream Adapter
  // Clear ADP_USB3_CS_0.PathEnable bit[31]
  // Set ADP_USB3_CS_0.Valid bit[30]
  if (pChildRouter != NULL) {
    Usb3AdpCapCs0And.Value = 0x3FFFFFFF;
    Usb3AdpCapCs0Or.Value  = 0x40000000;

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Clear Child Router [0x%lx] USB3 Upstream Adapter [%d] ADP_USB3_CS_0.PathEnable bit[31]."
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

    pChildAdapterUsb3Up->TunnelOn = 0;
  }

  // 1.2 Parent Router USB3 Downstream Adapter
  // Clear ADP_USB3_CS_0.PathEnable bit[31]
  // Set ADP_USB3_CS_0.Valid bit[30]
  Usb3AdpCapCs0And.Value = 0x3FFFFFFF;
  Usb3AdpCapCs0Or.Value  = 0x40000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Clear Parent Router [0x%lx] USB3 Downstream Adapter [%d] ADP_USB3_CS_0.PathEnable bit[31]."
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

  pParentUsb3DownAdp->TunnelOn = 0;

  Usb3PathTeardown (pCM, pParentUsb3DownAdp, pChildRouter);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Teardown Path for PCIe tunneling
 *
 * @param[in]  *pCM                   Point to global structure AMD_USB4_CM
 * @param[in]  *pParentPcieDownAdp    Point to Parent PCIe Downstream Adapter
 * @param[in]  *pChildRouter          Point to Child Router Data structure
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_INVALID_PARAMETER      Invalid parameters.
 * @retval EFI_DEVICE_ERROR           HW error.
**/
EFI_STATUS
DisablePcieTunneling (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_PCIE_ADAPTER              *pParentPcieDownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  )
{
  EFI_STATUS                                      Status;
  AMD_USB4_CM_PCIE_ADAPTER                        *pChildAdapterPcieUp;
  AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY  PcieAdpCapCs0And;
  AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY  PcieAdpCapCs0Or;

  Status                    = EFI_SUCCESS;

  if (pChildRouter != NULL) {
    pChildAdapterPcieUp     = (AMD_USB4_CM_PCIE_ADAPTER*)(pChildRouter->pAdapterPcieUp);
  } else {
    pChildAdapterPcieUp     = NULL;
  }

  PcieAdpCapCs0And.Value    = 0x0;
  PcieAdpCapCs0Or.Value     = 0x0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  //
  // 1. Disconnect and Disable Transport Layer Packets on PCIe Source and Destination adapters
  //

  // 1.1 Parent Router PCIe Upstream Adapter
  // Clear ADP_PCIE_CS_0.PathEnable bit[31]
  PcieAdpCapCs0And.Value              = 0x7FFFFFFF;
  PcieAdpCapCs0Or.Value               = 0x00000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Clear Parent Router [0x%lx] PCIe Downstream Adapter [%d] ADP_PCIE_CS_0.PathEnable bit[31]."
    " PcieAdpCapCs0And=0x%x, PcieAdpCapCs0Or=0x%x\n",
    pParentPcieDownAdp->Header.TopologyID,
    pParentPcieDownAdp->Header.AdapterNum,
    PcieAdpCapCs0And.Value,
    PcieAdpCapCs0Or.Value
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentPcieDownAdp->Header.TopologyID,
             CS_TARGET_ADAPTER,                       // CsType
             pParentPcieDownAdp->Header.AdapterNum,   // AdapterNum
             pParentPcieDownAdp->PcieCapability,      // Address
             1,                                       // SizeInDW
             &(PcieAdpCapCs0And.Value),               // AndMask
             &(PcieAdpCapCs0Or.Value)                 // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pParentPcieDownAdp->TunnelOn = 0;

  // 1.2 Child Router PCIe Upstream Adapter
  // Clear ADP_PCIE_CS_0.PathEnable bit[31]
  if ( pChildAdapterPcieUp ) {
    PcieAdpCapCs0And.Value              = 0x7FFFFFFF;
    PcieAdpCapCs0Or.Value               = 0x00000000;

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Clear Child Router [0x%lx] PCIe Upstream Adapter [%d] ADP_PCIE_CS_0.PathEnable bit[31]."
      " PcieAdpCapCs0And=0x%x, PcieAdpCapCs0Or=0x%x\n",
      pChildAdapterPcieUp->Header.TopologyID,
      pChildAdapterPcieUp->Header.AdapterNum,
      PcieAdpCapCs0And.Value,
      PcieAdpCapCs0Or.Value
      );
    Status = AmdUsb4Ring0RMWCS (
               pCM,
               pChildAdapterPcieUp->Header.TopologyID,
               CS_TARGET_ADAPTER,                       // CsType
               pChildAdapterPcieUp->Header.AdapterNum,  // AdapterNum
               pChildAdapterPcieUp->PcieCapability,     // Address
               1,                                       // SizeInDW
               &(PcieAdpCapCs0And.Value),               // AndMask
               &(PcieAdpCapCs0Or.Value)                 // OrValue
               );
    IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
    if ( EFI_ERROR (Status) )
      return Status;

    pChildAdapterPcieUp->TunnelOn = 0;
  }
  PciePathTeardown (pCM, pParentPcieDownAdp, pChildRouter);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Disable Path for DisplayPort tunneling
 *
 * @param[in]  *pCM                   Pointer to global structure AMD_USB4_CM
 * @param[in]  *pDpOutAdp             Pointer to DP OUT Adapter
 * @param[in]  *pDeepestChildRouter   Pointer to deepest Child Router
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_INVALID_PARAMETER      Invalid parameters.
 * @retval EFI_DEVICE_ERROR           HW error.
 * @retval EFI_NOT_FOUND              Did not find DP OUT in List
**/
EFI_STATUS
DisableDpTunneling (
  IN     AMD_USB4_CM             *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER  *pDpOutAdp,
  IN     AMD_USB4_CM_ROUTER      *pDeepestChildRouter
  )
{
  EFI_STATUS                     Status;
  LIST_ENTRY                     *DpOutNode;
  AMD_USB4_CM_DP_ADAPTER         *pDpIn;
  AMD_USB4_CM_ROUTER             *pDpInParentRouter;
  AMD_USB4_CM_DP_ADAPTER         *pDpOut;
  UINT32                         AndMask;
  AMD_USB4_ADP_DP_CS_0           DpInAdpCs0Or;
  AMD_USB4_ADP_DP_OUT_CS_0       DpOutAdpCs0Or;
  BOOLEAN                        FindDpOutInList;
  UINT32                         CompletionMetadata;
  UINT32                         *RouterOpData;
  UINT32                         CompletionStatus;
  AMD_USB4_CM_USB3_ADAPTER       *pParentUsb3DownAdp;
  AMD_USB4_CM_LANE_ADAPTER       *pParentDFPLaneAdapter;
  AMD_USB4_CM_ROUTER             *pChildRouter;
  AMD_USB4_CM_LANE_ADAPTER       *pChildUFPLaneAdapter;
  AMD_USB4_CM_USB3_ADAPTER       *pChildAdapterUsb3Up;

  Status               = EFI_SUCCESS;
  DpOutNode            = NULL;
  pDpIn                = NULL;
  pDpInParentRouter    = NULL;
  pDpOut               = NULL;
  Status               = EFI_SUCCESS;
  AndMask              = 0;
  DpInAdpCs0Or.Value   = 0;
  DpOutAdpCs0Or.Value  = 0;
  FindDpOutInList      = FALSE;
  CompletionMetadata   = 0;
  RouterOpData         = NULL;
  CompletionStatus     = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  //
  // Locate DP OUT Adapter and DP IN Adapter from list
  //
  if (IsListEmpty (&pCM->DpInPluggedList) || IsListEmpty (&pCM->DpOutPluggedList)) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("No set of DP adapters to be unpaired.\n");
    return EFI_NOT_FOUND;
  }

  DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);

  do {
    pDpOut = AMD_USB4_CM_DP_ADAPTER_FROM_ENTRY (DpOutNode);
    if ((pDpOut->Header.TopologyID == pDpOutAdp->Header.TopologyID)
      && (pDpOut->Header.AdapterNum == pDpOutAdp->Header.AdapterNum))
    {
      IDS_HDT_CONSOLE_USB4_DEBUG ("DP OUT Adapter found in List.\n");

      FindDpOutInList = TRUE;
      break;
    }
    DpOutNode = GetNextNode (&pCM->DpOutPluggedList, DpOutNode);
  } while (!IsNull (&pCM->DpOutPluggedList, DpOutNode));

  if (!FindDpOutInList) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("DP OUT Adapter not found in List.\n");
    return EFI_NOT_FOUND;
  }

  pDpIn = pDpOut->pPairedDpAdapter;

  if (pDpIn->AdapterState == ADAPTER_STATE_PAIRED) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("DP Link %x: ", pDpIn->DpLinkNumber);
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Unpaired DP Link: ");
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "DP IN Adapter 0x%x paired with DP OUT Adapter 0x%x. Pointer check:"
    " pDpIn->pPairedDpAdapter 0x%x pDpOut->pPairedDpAdapter 0x%x\n",
    pDpIn,
    pDpOut,
    pDpIn->pPairedDpAdapter,
    pDpOut->pPairedDpAdapter
    );

  if (pDpIn->AdapterState == ADAPTER_STATE_PAIRED) {
    //
    // 1. Disconnect and Disable Transport Layer Packets on USB3 Source and Destination adapters
    //

    //
    // Disable DP OUT Video Aux Enable and Video Enable
    //
    AndMask = 0x3FFFFFFF;
    DpOutAdpCs0Or.Value = 0x00000000;
    Status = AmdUsb4Ring0RMWCS (
               pCM,
               pDpOutAdp->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) pDpOutAdp->Header.AdapterNum,
               (UINT32) pDpOutAdp->CapabilityOffset,
               1,
               &AndMask,
               &(DpOutAdpCs0Or.Value)
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write to DP OUT's AMD_USB4_ADP_DP_CS_0 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    //
    // Disable DP IN Video Aux Enable and Video Enable
    //
    AndMask = 0x3FFFFFFF;
    DpInAdpCs0Or.Value = 0x00000000;
    Status = AmdUsb4Ring0RMWCS (
               pCM,
               pDpIn->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) pDpIn->Header.AdapterNum,
               (UINT32) pDpIn->CapabilityOffset,
               1,
               &AndMask,
               &(DpInAdpCs0Or.Value)
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write to DP IN's AMD_USB4_ADP_DP_CS_0 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    pCM->NumPairedAdapters--;
  }

  //
  // Teardown DP Path
  //
  Status = DpPathTeardown (pCM, pDpIn, pDpOutAdp, (AMD_USB4_CM_CELL *) pDpIn, pDeepestChildRouter, FALSE);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to Teardown DP Path.\n");
    return Status;
  }

  //
  // Deallocate resource for DP IN Adapter
  //
  /*
  Status = RouterOperation (
             pCM,
             (AMD_USB4_CM_ROUTER *) pDpIn->Header.Parent,
             DEALLOCATE_DP_RESOURCE,
             pDpIn->Header.AdapterNum,
             &CompletionMetadata,
             &RouterOpData,
             &CompletionStatus
             );
  */
  Status = RouterOperation2 (
             pCM,
             (AMD_USB4_CM_ROUTER *) pDpIn->Header.Parent,
             DEALLOCATE_DP_RESOURCE,
             pDpIn->Header.AdapterNum,
             0,
             NULL,
             &CompletionMetadata,
             &CompletionStatus,
             &RouterOpData
             );

  IDS_HDT_CONSOLE_USB4_DEBUG ("   Deallocate DP Resource Status: %r, ReturnStatus: 0x%x\n", Status, CompletionStatus);

  if (!EFI_ERROR (Status) && ( CompletionStatus == 0 ) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("DP Source is detected, and DP Resource was deallocated.\n");
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("DP Source is detected, however DP Resource was not deallocated.\n");
  }

  //
  // Update USB3 bandwidth
  // Since DP Link released bandwidth, it can be redistributed to USB3 Tunneling
  // This does not need to happen if DP Tunneling setup fails as USB3 Tunneling has not been initialized yet.
  //
  if (pDpIn->AdapterState == ADAPTER_STATE_PAIRED) {
    pDpInParentRouter = (AMD_USB4_CM_ROUTER *)(pDpIn->Header.Parent);

    if (pDpInParentRouter->IsHost) {
      pParentUsb3DownAdp = (AMD_USB4_CM_USB3_ADAPTER *)(pDpInParentRouter->pAdapterUsb3Down);
      pParentDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)(pParentUsb3DownAdp->AdpHeader.pLinkPartner);
      pChildRouter = (AMD_USB4_CM_ROUTER *)(pParentDFPLaneAdapter->AdpHeader.pLinkPartner);
      pChildAdapterUsb3Up = (AMD_USB4_CM_USB3_ADAPTER *)(pChildRouter->pAdapterUsb3Up);
      pChildUFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)(pChildRouter->pAdapterUFP);

      IDS_HDT_CONSOLE_USB4_DEBUG ("pDpInParentRouter = %x\n", pDpInParentRouter);
      IDS_HDT_CONSOLE_USB4_DEBUG ("pParentUsb3DownAdp = %x\n", pParentUsb3DownAdp);
      IDS_HDT_CONSOLE_USB4_DEBUG ("pParentDFPLaneAdapter = %x\n", pParentDFPLaneAdapter);
      IDS_HDT_CONSOLE_USB4_DEBUG ("pChildRouter = %x\n", pChildRouter);
      IDS_HDT_CONSOLE_USB4_DEBUG ("pChildAdapterUsb3Up = %x\n", pChildAdapterUsb3Up);
      IDS_HDT_CONSOLE_USB4_DEBUG ("pChildUFPLaneAdapter = %x\n", pChildUFPLaneAdapter);

      Status = Usb3Bandwidth (
                 pCM,
                 pDpInParentRouter,
                 pParentUsb3DownAdp,
                 pChildAdapterUsb3Up,
                 pParentDFPLaneAdapter,
                 pChildUFPLaneAdapter
                 );
      IDS_HDT_CONSOLE_USB4_DEBUG ("Setup USB3 Bandwidth Status = %r\n", Status);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "DP IN Adapter is not present in Host Router. "
        "Reallocation of USB4 bandwidth for USB3 Tunneling is not needed.\n"
      );
    }
  }

  //
  // Unpair DP Adapters
  //
  pDpOut->pPairedDpAdapter = NULL;
  pDpIn->pPairedDpAdapter = NULL;

  pDpOut->AdapterState = ADAPTER_STATE_PLUGGED;
  pDpIn->AdapterState = ADAPTER_STATE_PLUGGED;

  pDpOut->DpLinkNumber = 0;
  pDpIn->DpLinkNumber = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a End\n", __FUNCTION__);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * EnableUsb3Tunneling
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 *
 * @retval    VOID
 **/
VOID
EnableUsb3Tunneling (
  IN  AMD_USB4_CM       *pCM
  )
{
  AMD_USB4_CM_ROUTER        *pNode;
  AMD_USB4_CM_USB3_ADAPTER  *pUsb3DownstreamAdapter;
  AMD_USB4_CM_LANE_ADAPTER  *pDFPLaneAdapter;
  AMD_USB4_CM_ROUTER        *Stack[MAX_USB4_CM_STACK_NUM];
  INT32                     Num;

  pNode                   = &(pCM->PtrCmData[0].Router);  // It is Host Router
  pUsb3DownstreamAdapter  = NULL;
  pDFPLaneAdapter         = NULL;
  Num                     = -1;

  IDS_HDT_CONSOLE_USB4_INFO ("Traverse Whole Domain for Usb3 Tunneling - Start\n");

  // Push the Host Router
  Stack[++Num] = pNode;
  while ( Num >= 0 ) {
    // Pop
    pNode = Stack[Num--];
    if ( pNode ) {
      if ( pNode->Usb3Support ) {
        IDS_HDT_CONSOLE_USB4_INFO ("Router [0x%lx] support Usb3 Tunneling.\n", pNode->Header.TopologyID);
        //
        // Check if this router has child router, push them
        //

        // 1. Get first Usb3 Downstream Adapter pointer
        pUsb3DownstreamAdapter = (AMD_USB4_CM_USB3_ADAPTER*)pNode->pAdapterUsb3Down;

        // 2. Go through the DFP link list
        while ( pUsb3DownstreamAdapter ) {
          // Check if the DFP Lane Adapter connects a Router and the child router support Usb3 Tunneling
          // Then init the Path and push stack.

          // Get paired DFP Lane Adapter
          pDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)(pUsb3DownstreamAdapter->AdpHeader.pLinkPartner);
          if ( pDFPLaneAdapter != NULL ) {
            if ( pDFPLaneAdapter->AdpHeader.pLinkPartner != NULL
              && pDFPLaneAdapter->AdpHeader.pLinkPartner->Router.Usb3Support )
            {
              IDS_HDT_CONSOLE_USB4_INFO (
                "DFP Lane Adapter[%d] connect a child router[0x%lx] and it support Usb3 Tunneling.\n",
                pDFPLaneAdapter->Header.AdapterNum,
                pDFPLaneAdapter->AdpHeader.pLinkPartner->Router.Header.TopologyID
                );

              // Process the Usb3 Path Setup
              Usb3PathSetup (
                pCM,
                pUsb3DownstreamAdapter,                                       // Parent Usb3 DownStream Adapter
                (AMD_USB4_CM_ROUTER*)pDFPLaneAdapter->AdpHeader.pLinkPartner  // Child Router
                );

              // Push Child Router to Stack
              if ( pDFPLaneAdapter->AdpHeader.pLinkPartner ) {
                Stack[++Num] = (AMD_USB4_CM_ROUTER*)(pDFPLaneAdapter->AdpHeader.pLinkPartner);
              } else {
                IDS_HDT_CONSOLE_USB4_INFO (
                  "DFP (Topology ID: 0x%lx) Lane Adapter[%d] do not have router connected.\n",
                  pDFPLaneAdapter->Header.TopologyID,
                  pDFPLaneAdapter->Header.AdapterNum
                  );
              }
            } else {
              IDS_HDT_CONSOLE_USB4_INFO (
                "DFP Lane Adapter[ToplogyID: 0x%lx, Adapter Num: %d] do not connect a child router (0x%x) [ToplogyID: 0x%lx]"
                " or the child router do not support Usb3 Tunneling (0x%x).\n",
                pDFPLaneAdapter->Header.TopologyID,
                pDFPLaneAdapter->Header.AdapterNum,
                pDFPLaneAdapter->AdpHeader.pLinkPartner,
                (pDFPLaneAdapter->AdpHeader.pLinkPartner != NULL) ? pDFPLaneAdapter->AdpHeader.pLinkPartner->Header.TopologyID : 0xFF,
                (pDFPLaneAdapter->AdpHeader.pLinkPartner != NULL) ? pDFPLaneAdapter->AdpHeader.pLinkPartner->Router.Usb3Support : 0xFF
                );
            }
          }

          // Get next sibling DFP Lane Adapter
          pUsb3DownstreamAdapter = (AMD_USB4_CM_USB3_ADAPTER*)(pUsb3DownstreamAdapter->AdpHeader.pSiblingAdapter);
        }
      } else {
        IDS_HDT_CONSOLE_USB4_INFO ("Router [0x%lx] do not support Usb3 Tunneling.\n", pNode->Header.TopologyID);
      }
    }
  }

  IDS_HDT_CONSOLE_USB4_INFO ("Traverse Whole Domain for Usb3 Tunneling - End\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * EnablePcieTunneling
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 *
 * @retval    VOID
 **/
VOID
EnablePcieTunneling (
  IN  AMD_USB4_CM       *pCM
  )
{
  AMD_USB4_CM_ROUTER        *pNode;
  AMD_USB4_CM_PCIE_ADAPTER  *pPcieDownStreamAdapter;
  AMD_USB4_CM_LANE_ADAPTER  *pDFPLaneAdapter;
  AMD_USB4_CM_ROUTER        *Stack[MAX_USB4_CM_STACK_NUM];
  INT32                     Num;

  AMD_USB4_CM_ROUTER        *Tbt3Stack[MAX_USB4_CM_STACK_NUM];
  UINT32                    Tbt3Num;

  pNode                   = &(pCM->PtrCmData[0].Router);  // It is Host Router
  pPcieDownStreamAdapter  = NULL;
  pDFPLaneAdapter         = NULL;
  Num                     = -1;
  Tbt3Num                 = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("Traverse Whole Domain for Pcie Tunneling - Start\n");

  // Push the Host Router
  Stack[++Num] = pNode;
  while ( Num >= 0 ) {
    // Pop
    pNode = Stack[Num--];
    if ( pNode ) {
      if ( pNode->PcieSupport ) {
        IDS_HDT_CONSOLE_USB4_INFO ("Router [0x%lx] support Pcie Tunneling.\n", pNode->Header.TopologyID);
        //
        // Check if this router has child router, push them
        //

        // 1. Get first Pcie Downstream Adapter pointer
        pPcieDownStreamAdapter = (AMD_USB4_CM_PCIE_ADAPTER*)pNode->pAdapterPcieDown;

        // 2. Go through the DFP link list
        while ( pPcieDownStreamAdapter ) {
          // Check if the DFP Lane Adapter connects a Router and the child router support Pcie Tunneling
          // Then init the Path and push stack.

          //// Get paired DFP Lane Adapter
          pDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)(pPcieDownStreamAdapter->AdpHeader.pLinkPartner);
          if ( pDFPLaneAdapter != NULL ) {
            if ( pDFPLaneAdapter->AdpHeader.pLinkPartner != NULL
              && pDFPLaneAdapter->AdpHeader.pLinkPartner->Router.PcieSupport )
            {
              IDS_HDT_CONSOLE_USB4_INFO (
                "DFP Lane Adapter[%d] connect a child router[0x%lx] and it support Pcie Tunneling.\n",
                pDFPLaneAdapter->Header.AdapterNum,
                pDFPLaneAdapter->AdpHeader.pLinkPartner->Router.Header.TopologyID
                );

              // Process the Pcie Path Setup
              PciePathSetup (
                pCM,
                pPcieDownStreamAdapter,                                       // Parent Pcie DownStream Adapter
                (AMD_USB4_CM_ROUTER*)pDFPLaneAdapter->AdpHeader.pLinkPartner  // Child Router
                );

              // Push Child Router to Stack
              if ( pDFPLaneAdapter->AdpHeader.pLinkPartner ) {
                Stack[++Num] = (AMD_USB4_CM_ROUTER*)(pDFPLaneAdapter->AdpHeader.pLinkPartner);

                // If the Child Router is Tbt3 Router, push it to stack. We need enable it's xHC later.
                if ( !IsUSB4Router ((AMD_USB4_CM_ROUTER*)(pDFPLaneAdapter->AdpHeader.pLinkPartner)) ) {
                  Tbt3Stack[Tbt3Num++] = (AMD_USB4_CM_ROUTER*)(pDFPLaneAdapter->AdpHeader.pLinkPartner);
                }
              } else {
                IDS_HDT_CONSOLE_USB4_INFO (
                  "DFP (Topology ID: 0x%lx) Lane Adapter[%d] do not have router connected.\n",
                  pDFPLaneAdapter->Header.TopologyID,
                  pDFPLaneAdapter->Header.AdapterNum
                  );
              }
            } else {
              IDS_HDT_CONSOLE_USB4_INFO (
                "DFP Lane Adapter[ToplogyID: 0x%lx, Adapter Num: %d] do not connect a child router (0x%x) [ToplogyID: 0x%lx]"
                " or the child router do not support Pcie Tunneling (0x%x).\n",
                pDFPLaneAdapter->Header.TopologyID,
                pDFPLaneAdapter->Header.AdapterNum,
                pDFPLaneAdapter->AdpHeader.pLinkPartner,
                (pDFPLaneAdapter->AdpHeader.pLinkPartner != NULL) ? pDFPLaneAdapter->AdpHeader.pLinkPartner->Header.TopologyID : 0xFF,
                (pDFPLaneAdapter->AdpHeader.pLinkPartner != NULL) ? pDFPLaneAdapter->AdpHeader.pLinkPartner->Router.PcieSupport : 0xFF
                );
            }
          }

          // Get next sibling DFP Lane Adapter
          pPcieDownStreamAdapter = (AMD_USB4_CM_PCIE_ADAPTER*)(pPcieDownStreamAdapter->AdpHeader.pSiblingAdapter);
        }
      } else {
        IDS_HDT_CONSOLE_USB4_INFO ("Router [0x%lx] do not support Pcie Tunneling.\n", pNode->Header.TopologyID);
      }
    }
  }

  // TBT3 xHC Enablement after PCIE tunnel is ready.
  Tbt3EnablexHC (pCM, Tbt3Stack, Tbt3Num);

  IDS_HDT_CONSOLE_USB4_INFO ("Traverse Whole Domain for Pcie Tunneling - End\n");
}

/**
 * Enable DisplayPort Tunneling
 *
 * @param[in] *pCM                Pointer to global structure AMD_USB4_CM
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_UNSUPPORTED        DP Tunneling over USB4 is not supported
 * @retval EFI_DEVICE_ERROR       Read/Write error with register
 * @retval EFI_NOT_FOUND          No Displayport Source/Sink pair found
**/
EFI_STATUS
EnableDpTunneling (
  IN     AMD_USB4_CM    *pCM
  )
{
  LIST_ENTRY                       *DpInNode;
  LIST_ENTRY                       *DpOutNode;
  AMD_USB4_CM_DP_ADAPTER           *pDpIn;
  AMD_USB4_DP_LOCAL_CAP            *pDpInLocalCap;
  AMD_USB4_DP_REMOTE_CAP           *pDpInRemoteCap;
  AMD_USB4_CM_DP_ADAPTER           *pDpOut;
  AMD_USB4_ADP_DP_OUT_LOCAL_CAP    *pDpOutLocalCap;
  AMD_USB4_ADP_DP_OUT_REMOTE_CAP   *pDpOutRemoteCap;
  AMD_USB4_CM_ROUTER               *pAdapterParentRouter;
  AMD_USB4_CM_LANE_ADAPTER         *pUpstreamRouterDFP;
  EFI_STATUS                       Status;
  UINT32                           MinUsb4Bandwidth;
  BOOLEAN                          ValidPath;
  UINT32                           PathMaximalLinkRate;
  UINT32                           PathMaximalLaneCount;
  UINT32                           *DataPtr;
  UINT32                           DataSize;
  UINT32                           DpBandwidth;
  BOOLEAN                          IsDpBandwidthAllocated;
  LIST_ENTRY                       *DpNodeTemp;
  BOOLEAN                          IsPendingDpLink;
  UINT32                           AndMask;
  AMD_USB4_ADP_DP_OUT_STATUS_CTRL  DpOutCsStatusCtrl;
  AMD_USB4_ADP_DP_CS_0             DpInAdpCs0Or;
  AMD_USB4_ADP_DP_OUT_CS_0         DpOutAdpCs0Or;
  AMD_USB4_DP_REMOTE_CAP           DpInCsRemoteCap;
  AMD_USB4_ADP_DP_OUT_REMOTE_CAP   DpOutCsRemoteCap;
  UINT32                           CompletionMetadata;
  UINT32                           *RouterOpData;
  UINT32                           CompletionStatus;


  DpInNode                = NULL;
  DpOutNode               = NULL;
  pDpIn                   = NULL;
  pDpInLocalCap           = NULL;
  pDpInRemoteCap          = NULL;
  pDpOut                  = NULL;
  pDpOutLocalCap          = NULL;
  pDpOutRemoteCap         = NULL;
  pAdapterParentRouter    = NULL;
  pUpstreamRouterDFP      = NULL;
  Status                  = EFI_SUCCESS;
  MinUsb4Bandwidth        = 0;
  ValidPath               = TRUE;
  PathMaximalLinkRate     = 0;
  PathMaximalLaneCount    = 0;
  DataPtr                 = NULL;
  DataSize                = 0;
  DpBandwidth             = 0;
  IsDpBandwidthAllocated  = FALSE;
  DpNodeTemp              = NULL;
  IsPendingDpLink         = FALSE;
  AndMask                 = 0;
  DpOutCsStatusCtrl.Value = 0;
  DpInAdpCs0Or.Value      = 0;
  DpOutAdpCs0Or.Value     = 0;
  DpInCsRemoteCap.Value   = 0;
  DpOutCsRemoteCap.Value  = 0;
  CompletionMetadata      = 0;
  RouterOpData            = NULL;
  CompletionStatus        = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  if (IsListEmpty (&pCM->DpInPluggedList) || IsListEmpty (&pCM->DpOutPluggedList)) {
    IDS_HDT_CONSOLE_USB4_INFO ("No set of DP adapters to be paired.\n");
    return EFI_NOT_FOUND;
  }

  DpInNode  = GetFirstNode (&pCM->DpInPluggedList);
  DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);

  //
  // Pair DP IN and DP OUT adapters
  //
  do {
    pDpIn = AMD_USB4_CM_DP_ADAPTER_FROM_ENTRY (DpInNode);
    pDpOut = AMD_USB4_CM_DP_ADAPTER_FROM_ENTRY (DpOutNode);

    IDS_HDT_CONSOLE_USB4_INFO (
      "DP Link %x: Pairing DP IN Adapter Number %x (0x%x - TopologyID 0x%lx)"
      " with DP OUT Adapter Number %x (0x%x - TopologyID 0x%lx)\n",
      pCM->NumPairedAdapters + 1,
      pDpIn->Header.AdapterNum,
      pDpIn,
      pDpIn->Header.TopologyID,
      pDpOut->Header.AdapterNum,
      pDpOut,
      pDpOut->Header.TopologyID
      );

    //
    // Locate a DP IN and DP OUT adapters in the plugged state
    //
    if (pDpIn->AdapterState == ADAPTER_STATE_PLUGGED) {
      if (pDpOut->AdapterState == ADAPTER_STATE_PLUGGED) {

        //
        // Allocate resource for DP IN Adapter
        //
        /*
        Status = RouterOperation (
                   pCM,
                   (AMD_USB4_CM_ROUTER *) pDpIn->Header.Parent,
                   ALLOCATE_DP_RESOURCE,
                   pDpIn->Header.AdapterNum,
                   &CompletionMetadata,
                   &RouterOpData,
                   &CompletionStatus
                   );
        */

        Status = RouterOperation2 (
                   pCM,
                   (AMD_USB4_CM_ROUTER *) pDpIn->Header.Parent,
                   ALLOCATE_DP_RESOURCE,
                   pDpIn->Header.AdapterNum,
                   0,
                   NULL,
                   &CompletionMetadata,
                   &CompletionStatus,
                   &RouterOpData
                   );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   Allocate DP Resource Status: %r, ReturnStatus: 0x%x\n",
          Status,
          CompletionStatus
          );

        if ((!EFI_ERROR (Status)) && (CompletionStatus == 0)) {
          IDS_HDT_CONSOLE_USB4_INFO (" DP Source is detected, and DP Resource was allocated.\n");
        } else {
          IDS_HDT_CONSOLE_USB4_INFO (" DP Source is detected, however DP Resource was not allocated.\n");
          DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
          DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
          continue;
        }

        pAdapterParentRouter = (AMD_USB4_CM_ROUTER*) pDpOut->Header.Parent;

        //
        // Determine minimum available USB4 bandwidth on all links between DP IN and DP OUT adapter.
        //
        ValidPath = TRUE;
        MinUsb4Bandwidth = 0;
        IsDpBandwidthAllocated = FALSE;
        PathMaximalLinkRate = 0;
        PathMaximalLaneCount = 0;

        while (pAdapterParentRouter->Header.TopologyID != pDpIn->Header.TopologyID) {
          //
          // The only requirement for a DP path is that they terminate with a DP IN and DP OUT adapter
          // For Pre-OS CM, we are assuming DP IN will be in a path following DP OUT's Topology ID.
          //
          if (pAdapterParentRouter->IsHost) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Reached Host Router without locating DP IN Adapter\n");
            ValidPath = FALSE;
            break;
          }

          pUpstreamRouterDFP = (AMD_USB4_CM_LANE_ADAPTER*) pAdapterParentRouter->Header.Parent;

          // todo: Determine if we need this check here or if USB Link will account for it
          //if (((AMD_USB4_CM_LANE_ADAPTER) pAdapterParentRouter->pAdapterUFP)->LaneAdp_CurrentSpeed !=
          //    pUpstreamRouterDFP->LaneAdp_CurrentSpeed)
          //{
          //  return EFI_DEVICE_ERROR;
          //}

          //
          // Determine Minimum USB4 Bandwidth on Path
          // The first DisplayPort Link uses the maximum configuration. The second is constrained to
          // the remaining bandwidth.
          //
          if ((MinUsb4Bandwidth == 0) || (pUpstreamRouterDFP->AvailableBandwidth < MinUsb4Bandwidth)) {
            MinUsb4Bandwidth = pUpstreamRouterDFP->AvailableBandwidth;
          }

          //
          // Determine if this is first or second DisplayPort Link on this Path
          //
          if ((pUpstreamRouterDFP->TotalDownstreamDpPathBandwidth)
            || (pUpstreamRouterDFP->TotalUpstreamDpPathBandwidth))
          {
            IsDpBandwidthAllocated = TRUE;
          }

          pAdapterParentRouter = (AMD_USB4_CM_ROUTER*) pUpstreamRouterDFP->Header.Parent;
        }

        //
        // If Host Router is reached without locating DP IN Adapter, continue to next DP OUT Adapter in Plugged list
        // If end of DP OUT Plugged list is reached, traverse DP IN Plugged list and reset DP OUT Plugged list pointer
        //
        if (ValidPath == FALSE) {
          DpOutNode = GetNextNode (&pCM->DpOutPluggedList, DpOutNode);
          if (IsNull (&pCM->DpOutPluggedList, DpOutNode)) {
            IDS_HDT_CONSOLE_USB4_INFO (
              "No remaining DP OUT adapters to be paired, locating next DP IN adapter.\n"
              );
            DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
            DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
          }
          continue;
        }

        //
        // Determine Path's Maximal Link Rate and Maximal Lane count based on minimum available USB4 Raw Bandwidth
        // This will be used to update DP IN DP_REMOTE_CAP.Maximal Link Rate and DP IN DP_REMOTE_CAP.Maximal Lane Count
        // The first DisplayPort Link uses the maximum configuration.
        // The second is constrained to the remaining bandwidth.
        //
        IDS_HDT_CONSOLE_USB4_INFO ("MinUsb4Bandwidth for path: %d\n", MinUsb4Bandwidth);
        if (IsDpBandwidthAllocated == FALSE) {

          //
          // Determine if another DP IN/DP OUT pair are present to be paired to adjust Link Rate and Lane Count
          // This impacts a USB4 Link older than Gen 3x2 (40 Gbps)
          //
          DpNodeTemp = GetNextNode (&pCM->DpInPluggedList, DpInNode);
          if (!IsNull (&pCM->DpInPluggedList, DpNodeTemp)) {
            DpNodeTemp = GetNextNode (&pCM->DpOutPluggedList, DpOutNode);
            if (!IsNull (&pCM->DpOutPluggedList, DpNodeTemp)) {
              IsPendingDpLink = TRUE;
            }
          }

          if (MinUsb4Bandwidth == (USB4_RAW_BANDWIDTH_40GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                   GUARD_BAND_BANDWIDTH_DENOMINATOR)
          {
            // 25.92 Gpbs
            PathMaximalLinkRate = LINK_RATE_8_1_GHZ;
            PathMaximalLaneCount = LANE_COUNT_4;
          } else if (MinUsb4Bandwidth == (USB4_RAW_BANDWIDTH_20GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                          GUARD_BAND_BANDWIDTH_DENOMINATOR)
          {
            if (!IsPendingDpLink) {
              // 17.28 Gpbs
              PathMaximalLinkRate = LINK_RATE_5_4_GHZ;
              PathMaximalLaneCount = LANE_COUNT_4;
            } else {
              // 12.96 Gbps
              PathMaximalLinkRate = LINK_RATE_8_1_GHZ;
              PathMaximalLaneCount = LANE_COUNT_2;
            }
          } else if (MinUsb4Bandwidth == (USB4_RAW_BANDWIDTH_10GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                          GUARD_BAND_BANDWIDTH_DENOMINATOR)
          {
            if (!IsPendingDpLink) {
              // 8.64 Gpbs
              PathMaximalLinkRate = LINK_RATE_2_7_GHZ;
              PathMaximalLaneCount = LANE_COUNT_4;
            } else {
              // 4.32 Gbps
              PathMaximalLinkRate = LINK_RATE_2_7_GHZ;
              PathMaximalLaneCount = LANE_COUNT_2;
            }
          } else {
            IDS_HDT_CONSOLE_USB4_ERROR (
              "Unsupported bandwidth: %d for first DisplayPort Link\n",
              MinUsb4Bandwidth
              );
            return EFI_UNSUPPORTED;
          }
        } else {
          if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_8_1_GHZ][LANE_COUNT_4]) {
            // 25.92 Gpbs
            PathMaximalLinkRate = LINK_RATE_8_1_GHZ;
            PathMaximalLaneCount = LANE_COUNT_4;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_5_4_GHZ][LANE_COUNT_4]) {
            // 17.28 Gbps
            PathMaximalLinkRate = LINK_RATE_5_4_GHZ;
            PathMaximalLaneCount = LANE_COUNT_4;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_8_1_GHZ][LANE_COUNT_2]) {
            // 12.96 Gbps
            PathMaximalLinkRate = LINK_RATE_8_1_GHZ;
            PathMaximalLaneCount = LANE_COUNT_2;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_2_7_GHZ][LANE_COUNT_4]) {
            // 8.64 Gbps
            PathMaximalLinkRate = LINK_RATE_2_7_GHZ;
            PathMaximalLaneCount = LANE_COUNT_4;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_8_1_GHZ][LANE_COUNT_1]) {
            // 6.48 Gbps
            PathMaximalLinkRate = LINK_RATE_8_1_GHZ;
            PathMaximalLaneCount = LANE_COUNT_1;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_1_62_GHZ][LANE_COUNT_4]) {
            // 5.184 Gbps
            PathMaximalLinkRate = LINK_RATE_1_62_GHZ;
            PathMaximalLaneCount = LANE_COUNT_4;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_2_7_GHZ][LANE_COUNT_2]) {
            // 4.32 Gbps
            PathMaximalLinkRate = LINK_RATE_2_7_GHZ;
            PathMaximalLaneCount = LANE_COUNT_2;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_1_62_GHZ][LANE_COUNT_2]) {
            // 2.592 Gbps
            PathMaximalLinkRate = LINK_RATE_1_62_GHZ;
            PathMaximalLaneCount = LANE_COUNT_2;
          } else if (MinUsb4Bandwidth >= DpRequiredBandwidth[LINK_RATE_2_7_GHZ][LANE_COUNT_1]) {
            // 2.16 Gbps
            PathMaximalLinkRate = LINK_RATE_2_7_GHZ;
            PathMaximalLaneCount = LANE_COUNT_1;
          } else {
            // 1.296 Gbps
            PathMaximalLinkRate = LINK_RATE_1_62_GHZ;
            PathMaximalLaneCount = LANE_COUNT_1;
          }
        }
        IDS_HDT_CONSOLE_USB4_INFO (
          "DP IN Remote Capability: Maximal Link Rate for path updated to: 0x%x\n",
          PathMaximalLinkRate
          );
        IDS_HDT_CONSOLE_USB4_INFO (
          "DP IN Remote Capability: Maximal Lane Count for path updated to: 0x%x\n",
          PathMaximalLaneCount
          );

        //
        // Determine DisplayPort Required Bandwidth
        // Note: The second DisplayPort Link is constrained to the remaining bandwidth.
        //
        DpBandwidth = DpRequiredBandwidth[PathMaximalLinkRate][PathMaximalLaneCount];
        IDS_HDT_CONSOLE_USB4_INFO ("DP Path Bandwidth: %d\n", DpBandwidth);

        //
        // Verify DP Adapter Supported USB4 1.0 Spec for DP Tunneling
        //
        if ((pDpIn->ProtocolAdapterVersion < 0x4) || (pDpOut->ProtocolAdapterVersion < 0x4)) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Protocol Adapter does not support USB4 1.0 Spec.\n");
          DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
          DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
          continue;
        }

        if (IsTbt3DpPerformCMHandshake ((AMD_USB4_CM_ROUTER*) pDpOut->Header.Parent)) {
          if (!IsUSB4Router ((AMD_USB4_CM_ROUTER*) pDpOut->Header.Parent)) {
            Tbt3DpInitCMHandshake (pCM, pDpOut);
          }

          //
          // Set DP OUT DP_STATUS_CTRL.UF to indicate DP IN Adapter is a USB4 DP IN Adapter
          //
          DpOutCsStatusCtrl.Value = 0x00000000;
          DpOutCsStatusCtrl.Field.DPInAdapterUSB4Flag = 1;
          AndMask = 0x00000000;
          AndMask = (UINT32)(~(DpOutCsStatusCtrl.Value));
          Status = AmdUsb4Ring0RMWCS (
                     pCM,
                     pDpOut->Header.TopologyID,
                     CS_TARGET_ADAPTER,
                     (UINT32) pDpOut->Header.AdapterNum,
                     (UINT32) pDpOut->CapabilityOffset + 6,
                     1,
                     &AndMask,
                     &(DpOutCsStatusCtrl.Value)
                     );
          if ( EFI_ERROR (Status) ) {
            IDS_HDT_CONSOLE_USB4_ERROR (
              "Failed to write AMD_USB4_ADP_DP_OUT_STATUS_CTRL.DPInAdapterUSB4Flag Register.\n"
              );
            return EFI_DEVICE_ERROR;
          }

          //
          // Set DP OUT DP_STATUS_CTRL.CMHS for Connection Manager Handshake
          //
          DpOutCsStatusCtrl.Value = 0x00000000;
          DpOutCsStatusCtrl.Field.CMHandShake = 1;
          AndMask = 0x00000000;
          AndMask = (UINT32)(~(DpOutCsStatusCtrl.Value));
          Status = AmdUsb4Ring0RMWCS (
                     pCM,
                     pDpOut->Header.TopologyID,
                     CS_TARGET_ADAPTER,
                     (UINT32) pDpOut->Header.AdapterNum,
                     (UINT32) pDpOut->CapabilityOffset + 6,
                     1,
                     &AndMask,
                     &(DpOutCsStatusCtrl.Value)
                     );
          if ( EFI_ERROR (Status) ) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write AMD_USB4_ADP_DP_OUT_STATUS_CTRL.CMHandShake Register.\n");
            return EFI_DEVICE_ERROR;
          }

          //
          // Poll AMD_USB4_ADP_DP_OUT_STATUS_CTRL.CMHandShake until DP OUT adapter resets value
          // @todo: There is no time given for this. Use 10000 uS?
          //
          DataSize = 1;
          do {
            Status = AmdUsb4Ring0ReadCS (
                       pCM,
                       pDpOut->Header.TopologyID,
                       CS_TARGET_ADAPTER,                     // CsType
                       (UINT32) pDpOut->Header.AdapterNum,    // AdapterNum
                       (UINT32) pDpOut->CapabilityOffset + 6, // ReadAddress
                       DataSize,                              // ReadSize
                       (UINT32**)&DataPtr,                    // DataOut
                       &DataSize                              // DataSize
                       );
            if ((EFI_ERROR (Status)) || (DataSize != 1)) {
              IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read AMD_USB4_ADP_DP_OUT_STATUS_CTRL Register.\n");
              return EFI_DEVICE_ERROR;
            }
          } while ((((AMD_USB4_ADP_DP_OUT_STATUS_CTRL *) DataPtr)->Field.CMHandShake));
        }

        //
        // Copy DP IN Adapter DP_LOCAL_CAP data to DP OUT Adapter DP_REMOTE_CAP
        //
        DpOutCsRemoteCap.Value = 0x00000000;
        DpOutCsRemoteCap.Field.ProtocolAdapterVersion   = pDpIn->ProtocolAdapterVersion;
        DpOutCsRemoteCap.Field.MaximalDPCDRev           = pDpIn->MaximalDPCDRev;
        DpOutCsRemoteCap.Field.MaximalLinkRate          = pDpIn->MaximalLinkRate;
        DpOutCsRemoteCap.Field.MaximalLaneCount         = pDpIn->MaximalLaneCount;
        DpOutCsRemoteCap.Field.MSTCapability            = pDpIn->MSTCapability;
        DpOutCsRemoteCap.Field.TPS3Capability           = pDpIn->TPS3Capability;
        DpOutCsRemoteCap.Field.TPS4Capability           = pDpIn->TPS4Capability;
        DpOutCsRemoteCap.Field.FECNotSupported          = pDpIn->FECNotSupported;
        DpOutCsRemoteCap.Field.SecondarySplitCapability = pDpIn->SecondarySplitCapability;
        DpOutCsRemoteCap.Field.LTTPRNotSupported        = pDpIn->LTTPRNotSupported;
        DpOutCsRemoteCap.Field.DSCNotSupported          = pDpIn->DSCNotSupported;
        AndMask = 0x00000000;
        AndMask = (UINT32)(~(DpOutCsRemoteCap.Value));
        Status = AmdUsb4Ring0RMWCS (
                   pCM,
                   pDpOut->Header.TopologyID,
                   CS_TARGET_ADAPTER,
                   (UINT32) pDpOut->Header.AdapterNum,
                   (UINT32) pDpOut->CapabilityOffset + 5,
                   1,
                   &AndMask,
                   &(DpOutCsRemoteCap.Value)
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write AMD_USB4_ADP_DP_OUT_REMOTE_CAP.CMHandShake Register.\n");
          return EFI_DEVICE_ERROR;
        }

        if (!IsUSB4Router ((AMD_USB4_CM_ROUTER*) pDpOut->Header.Parent)) {
            Tbt3DpDisableLTTPRSupport (pCM, pDpOut);
        }

        //
        // Copy DP OUT Adapter DP_LOCAL_CAP data to DP IN Adapter DP_REMOTE_CAP
        //
        DpInCsRemoteCap.Value = 0x00000000;
        DpInCsRemoteCap.Field.ProtocolAdapterVersion   = pDpOut->ProtocolAdapterVersion;
        DpInCsRemoteCap.Field.MaximalDPCDRev           = pDpOut->MaximalDPCDRev;
        DpInCsRemoteCap.Field.MaximalLinkRate          = PathMaximalLinkRate;
        DpInCsRemoteCap.Field.MaximalLaneCount         = PathMaximalLaneCount;
        DpInCsRemoteCap.Field.MSTCapability            = pDpOut->MSTCapability;
        DpInCsRemoteCap.Field.TPS3Capability           = pDpOut->TPS3Capability;
        DpInCsRemoteCap.Field.TPS4Capability           = pDpOut->TPS4Capability;
        DpInCsRemoteCap.Field.FECNotSupported          = pDpOut->FECNotSupported;
        DpInCsRemoteCap.Field.SecondarySplitCapability = pDpOut->SecondarySplitCapability;
        DpInCsRemoteCap.Field.LTTPRNotSupported        = pDpOut->LTTPRNotSupported;
        DpInCsRemoteCap.Field.DSCNotSupported          = pDpOut->DSCNotSupported;
        AndMask = 0x00000000;
        AndMask = (UINT32)(~(DpInCsRemoteCap.Value));
        Status = AmdUsb4Ring0RMWCS (
                   pCM,
                   pDpIn->Header.TopologyID,
                   CS_TARGET_ADAPTER,
                   (UINT32) pDpIn->Header.AdapterNum,
                   (UINT32) pDpIn->CapabilityOffset + 5,
                   1,
                   &AndMask,
                   &(DpInCsRemoteCap.Value)
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write AMD_USB4_ADP_DP_OUT_REMOTE_CAP.CMHandShake Register.\n");
          return EFI_DEVICE_ERROR;
        }

        //
        // DP IN and DP OUT adapters will potentially be paired
        //
        pDpIn->pPairedDpAdapter = pDpOut;
        pDpOut->pPairedDpAdapter = pDpIn;
        pDpIn->DpLinkNumber = pCM->NumPairedAdapters + 1;
        pDpOut->DpLinkNumber = pCM->NumPairedAdapters + 1;

        //
        // Setup DP Paths
        //
        Status = DpPathSetup (
                   pCM,
                   pDpIn,
                   pDpOut,
                   (AMD_USB4_CM_CELL_HEADER *) pDpOut,
                   pCM->NumPairedAdapters,
                   DpBandwidth
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to setup path: %r\n", Status);
          Status = DisableDpTunneling (pCM, pDpOut, (AMD_USB4_CM_ROUTER *) pDpOut->Header.Parent);
          if (EFI_ERROR (Status)) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Failed to clean up path: %r\n", Status);
          }
          DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
          DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
          continue;
        }

        //
        // Enable DP IN Video Aux Enable and Video Enable
        //
        DpInAdpCs0Or.Value = 0x00000000;
        DpInAdpCs0Or.Field.AUXEnable = 1;
        DpInAdpCs0Or.Field.VideoEnable = 1;
        AndMask = 0x00000000;
        AndMask = (UINT32)(~(DpInAdpCs0Or.Value));
        Status = AmdUsb4Ring0RMWCS (
                   pCM,
                   pDpIn->Header.TopologyID,
                   CS_TARGET_ADAPTER,
                   (UINT32) pDpIn->Header.AdapterNum,
                   (UINT32) pDpIn->CapabilityOffset,
                   1,
                   &AndMask,
                   &(DpInAdpCs0Or.Value)
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write to DP IN's AMD_USB4_ADP_DP_CS_0 Register.\n");
          Status = DisableDpTunneling (pCM, pDpOut, (AMD_USB4_CM_ROUTER *) pDpOut->Header.Parent);
          if (EFI_ERROR (Status)) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Failed to clean up path: %r\n", Status);
          }
          return EFI_DEVICE_ERROR;
        }

        //
        // Enable DP OUT Video Aux Enable and Video Enable
        //
        DpOutAdpCs0Or.Value = 0x00000000;
        DpOutAdpCs0Or.Field.AUXEnable = 1;
        DpOutAdpCs0Or.Field.VideoEnable = 1;
        AndMask = 0x00000000;
        AndMask = (UINT32)(~(DpOutAdpCs0Or.Value));
        Status = AmdUsb4Ring0RMWCS (
                   pCM,
                   pDpOut->Header.TopologyID,
                   CS_TARGET_ADAPTER,
                   (UINT32) pDpOut->Header.AdapterNum,
                   (UINT32) pDpOut->CapabilityOffset,
                   1,
                   &AndMask,
                   &(DpOutAdpCs0Or.Value)
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write to DP OUT's AMD_USB4_ADP_DP_CS_0 Register.\n");
          Status = DisableDpTunneling (pCM, pDpOut, (AMD_USB4_CM_ROUTER *) pDpOut->Header.Parent);
          if (EFI_ERROR (Status)) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Failed to clean up path: %r\n", Status);
          }
          return EFI_DEVICE_ERROR;
        }

        //
        // Dummy read that helps ensure proper operation of the DP OUT Adapter in the TBT3 Router
        //
        if (!IsUSB4Router ((AMD_USB4_CM_ROUTER *) pDpOut->Header.Parent)) {
          Status = AmdUsb4Ring0ReadCS (
                     pCM,
                     pDpOut->Header.TopologyID,
                     CS_TARGET_ADAPTER,                     // CsType
                     (UINT32) pDpOut->Header.AdapterNum,    // AdapterNum
                     (UINT32) pDpOut->CapabilityOffset + 1, // ReadAddress
                     DataSize,                              // ReadSize
                     (UINT32**)&DataPtr,                    // DataOut
                     &DataSize                              // DataSize
                     );
          if ((EFI_ERROR (Status)) || (DataSize != 1)) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read AMD_USB4_ADP_DP_OUT_STATUS_CTRL Register.\n");
            return EFI_DEVICE_ERROR;
          }
        }

        //
        // DP IN and DP OUT adapters have completed pairing
        //
        pDpIn->AdapterState = ADAPTER_STATE_PAIRED;
        pDpOut->AdapterState = ADAPTER_STATE_PAIRED;

        IDS_HDT_CONSOLE_USB4_INFO (
          "DP IN Adapter 0x%x paired with DP OUT Adapter 0x%x. Pointer check:"
          " pDpIn->pPairedDpAdapter 0x%x pDpOut->pPairedDpAdapter 0x%x\n",
          pDpIn,
          pDpOut,
          pDpIn->pPairedDpAdapter,
          pDpOut->pPairedDpAdapter
          );

        pCM->NumPairedAdapters++;
        IDS_HDT_CONSOLE_USB4_INFO (
          "DP Link %x: Successfully paired DP IN Adapter Number %x (0x%x - TopologyID 0x%lx)"
          " with DP OUT Adapter Number %x (0x%x - TopologyID 0x%lx)\n",
          pCM->NumPairedAdapters,
          pDpIn->Header.AdapterNum,
          pDpIn,
          pDpIn->Header.TopologyID,
          pDpOut->Header.AdapterNum,
          pDpOut,
          pDpOut->Header.TopologyID
          );

        //
        // Update Maximal Lane Count and Link Rate
        // - DP IN Adapter may reduce the Maximal Link Rate and Lane count after DPRX read during DPTX discovery.
        // - Updated Maximal Lane Count and Link Rate are valid after DP_COMMON_CAP.DPRXCapabilitiesReadDone is set.
        // @todo DMUB should be setting DP_COMMON_CAP.DPRXCapabilitiesReadDone, however, this is being handled in GOP,
        // @todo which is loaded after PCI enumeration. Removing reallocation sequence for Pre-OS CM.
        //
        /*
        DataSize = 1;
        do {
          Status = AmdUsb4Ring0ReadCS (
                     pCM,
                     pDpIn->Header.TopologyID,
                     CS_TARGET_ADAPTER,                     // CsType
                     (UINT32) pDpIn->Header.AdapterNum,     // AdapterNum
                     (UINT32) pDpIn->CapabilityOffset + 7,  // ReadAddress
                     DataSize,                              // ReadSize
                     (UINT32**)&DataPtr,                    // DataOut
                     &DataSize                              // DataSize
                     );

          if ((EFI_ERROR (Status)) || (DataSize != 1)) {
            IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read AMD_USB4_DP_COMMON_CAP Register.\n");
            return EFI_DEVICE_ERROR;
          }

          if ((((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Field.DPRXCapabilitiesReadDone) == 0) {
            IDS_HDT_CONSOLE_USB4_ERROR (
              "DP IN's DP_COMMON_CAP.DPRXCapabilitiesReadDone is not set. DP_COMMON_CAP: %x\n",
              ((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Value
              );
          }
        } while ((((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Field.DPRXCapabilitiesReadDone) == 0);

        pDpIn->MaximalLinkRateCommon = ((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Field.MaximalLinkRate;
        pDpOut->MaximalLinkRateCommon = ((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Field.MaximalLinkRate;
        pDpIn->MaximalLaneCountCommon = ((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Field.MaximalLaneCount;
        pDpOut->MaximalLaneCountCommon = ((AMD_USB4_DP_COMMON_CAP *) DataPtr)->Field.MaximalLaneCount;

        if ( (pDpIn->MaximalLinkRateCommon < pDpIn->MaximalLinkRate)
          || (pDpIn->MaximalLaneCountCommon < pDpIn->MaximalLaneCount))
        {
          IDS_HDT_CONSOLE_USB4_INFO (
            "DP Path bandwidth has been freed\n"
            "Adapter:\n"
            "  Maximal Link Rate   = %d\n"
            "  Maximal Lane Count  = %d\n"
            "  DP Bandwidth        = %d\n"
            "Common Capabilities:\n"
            "  Maximal Link Rate   = %d\n"
            "  Maximal Lane Count  = %d\n"
            "  DP Bandwidth        = %d\n",
            pDpIn->MaximalLinkRate,
            pDpIn->MaximalLaneCount,
            DpBandwidth,
            pDpIn->MaximalLinkRateCommon,
            pDpIn->MaximalLaneCountCommon,
            DpRequiredBandwidth[pDpIn->MaximalLinkRateCommon][pDpIn->MaximalLaneCountCommon]
            );

          DpBandwidthReallocation (
            pCM,
            pDpIn,
            pDpOut,
            (AMD_USB4_CM_CELL_HEADER *) pDpOut,
            DpRequiredBandwidth[pDpIn->MaximalLinkRateCommon][pDpIn->MaximalLaneCountCommon]
            );
        }
        */

        //
        // Traverse DP IN Plugged list and reset DP OUT Plugged list pointer
        //
        DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
        DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
      } else {
        //
        // Traverse DP OUT Plugged list
        // If end of DP OUT Plugged list is reached, traverse DP IN Plugged list and reset DP OUT Plugged list pointer
        //
        IDS_HDT_CONSOLE_USB4_INFO ("DP OUT Adapter not in Plugged state, locating next DP OUT adapter.\n");
        DpOutNode = GetNextNode (&pCM->DpOutPluggedList, DpOutNode);
        if (IsNull (&pCM->DpOutPluggedList, DpOutNode)) {
          IDS_HDT_CONSOLE_USB4_INFO (
            "No remaining DP OUT adapters to be paired, locating next DP IN adapter.\n"
            );
          DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
          DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
        }
      }
    } else {
      IDS_HDT_CONSOLE_USB4_INFO ("DP IN Adapter not in Plugged state, locating next DP IN adapter.\n");
      DpInNode = GetNextNode (&pCM->DpInPluggedList, DpInNode);
      DpOutNode = GetFirstNode (&pCM->DpOutPluggedList);
    }
  } while (!IsNull (&pCM->DpInPluggedList, DpInNode));

  IDS_HDT_CONSOLE_USB4_INFO ("No remaining DP IN adapters to be paired.\n");

  return EFI_SUCCESS;
}
