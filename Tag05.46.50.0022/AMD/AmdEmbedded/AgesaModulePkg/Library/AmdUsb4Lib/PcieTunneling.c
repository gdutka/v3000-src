/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/PciLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>
#include "Library/AmdUsb4Lib.h"

#define FILECODE LIBRARY_AMDUSB4LIB_PCIETUNNELING_FILECODE

EFI_STATUS
AmdUsb4PciePathInitHandle (
  IN      AMD_USB4_CM                           *pCM,
  IN      AMD_USB4_CM_ROUTER                    *pHostRouter,
  IN      AMD_USB4_CM_PCIE_ADAPTER              *pPcieDownAdp
);

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
PciePathTeardown (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_PCIE_ADAPTER              *pParentPcieDownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  )
{
  EFI_STATUS                                      Status;
  AMD_USB4_CM_LANE_ADAPTER                        *pParentDFPLaneAdapter;
  AMD_USB4_CM_LANE_ADAPTER                        *pChildUFPLaneAdapter;
  AMD_USB4_CM_PCIE_ADAPTER                        *pChildAdapterPcieUp;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  Status                    = EFI_SUCCESS;
  pParentDFPLaneAdapter     = (AMD_USB4_CM_LANE_ADAPTER*)(pParentPcieDownAdp->AdpHeader.pLinkPartner);

  if (pChildRouter != NULL) {
    pChildUFPLaneAdapter    = (AMD_USB4_CM_LANE_ADAPTER*)(pChildRouter->pAdapterUFP);
    pChildAdapterPcieUp     = (AMD_USB4_CM_PCIE_ADAPTER*)(pChildRouter->pAdapterPcieUp);
  } else {
    pChildUFPLaneAdapter    = NULL;
    pChildAdapterPcieUp     = NULL;
  }

  //
  // 1. Teardown PCIe Tunneling Downstream Path
  //

  // 1.1 Clear Parent Router PCIe Downstream Adapter Path CS PATH_CS_0.Valid bit[31]
  IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Parent Router PCIe Downstream Adapter Path\n");

  Status = PathTeardown (
             pCM,
             (AMD_USB4_CM_CELL *) pParentPcieDownAdp,
             8
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
    return Status;
  }

  pParentPcieDownAdp->PathValid = 0;

  //
  // Skip Path teardown in Child Router if it has been disconnected
  //
  if (pChildRouter != NULL) {
    // 1.2 Setup Child Router UFP Lane Adapter Path PATH_CS_0.Valid bit[31]
    IDS_HDT_CONSOLE_USB4_DEBUG ("Setup Child Router UFP Lane Adapter Path\n");

    Status = PathTeardown (
               pCM,
               (AMD_USB4_CM_CELL *) pChildUFPLaneAdapter,
               PCIE_DOWNSTREAM_PATH_HOPID
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
      return Status;
    }

    pChildUFPLaneAdapter->PcieTunnelOn = 0;

    //
    // 2. Teardown PCIe Upstream Path
    //

    // 2.1 Clear Child Router PCIe Upstream Adapter Path PATH_CS_0.Valid bit[31]
    IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Child Router PCIe Upstream Adapter Path\n");

    Status = PathTeardown (
               pCM,
               (AMD_USB4_CM_CELL *) pChildAdapterPcieUp,
               8
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
      return Status;
    }

    pChildAdapterPcieUp->PathValid = 0;
  }

  // 2.2 Clear Parent Router DFP Lane Adapter Path PATH_CS_0.Valid bit[31]
  IDS_HDT_CONSOLE_USB4_DEBUG ("Clear Parent Router DFP Lane Adapter Path\n");

  Status = PathTeardown (
             pCM,
             (AMD_USB4_CM_CELL *) pParentDFPLaneAdapter,
             PCIE_UPSTREAM_PATH_HOPID
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to teardown path in adapter.\n");
    return Status;
  }

  pParentDFPLaneAdapter->PcieTunnelOn = 0;

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Setup Path for PCIe tunneling
 *
 * @param[in]  *pCM                   Point to global structure AMD_USB4_CM
 * @param[in]  *pParentPcieDownAdp    Point to Parent PCIe Downstream Adapter
 * @param[in]  *pChildRouter          Point to Child Router Data structure
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_INVALID_PARAMETER      Invalid parameters.
 * @retval EFI_DEVICE_ERROR           HW error.
 * @retval EFI_BUFFER_TOO_SMALL       Can not allocate minimum buffers for PCIe Path.

**/

EFI_STATUS
PciePathSetup (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_PCIE_ADAPTER              *pParentPcieDownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  )
{
  EFI_STATUS                Status;
  UINT32                    ParentCredits;
  UINT32                    ChildCredits;
  AMD_USB4_CM_LANE_ADAPTER  *pParentDFPLaneAdapter;
  AMD_USB4_CM_ROUTER        *pParentRouter;
  AMD_USB4_CM_LANE_ADAPTER  *pChildUFPLaneAdapter;
  AMD_USB4_CM_PCIE_ADAPTER  *pChildAdapterPcieUp;

  AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY                PcieAdpCapCs0And;
  AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY                PcieAdpCapCs0Or;

  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER  PciePathAnd;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER  PciePathOr;

  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER      LanePathAnd;
  AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER      LanePathOr;

  Status                = EFI_SUCCESS;
  ParentCredits         = 0;
  ChildCredits          = 0;
  pParentDFPLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*)(pParentPcieDownAdp->AdpHeader.pLinkPartner);
  pParentRouter         = (AMD_USB4_CM_ROUTER*)(pParentDFPLaneAdapter->Header.Parent);
  pChildUFPLaneAdapter  = (AMD_USB4_CM_LANE_ADAPTER*)(pChildRouter->pAdapterUFP);
  pChildAdapterPcieUp   = (AMD_USB4_CM_PCIE_ADAPTER*)(pChildRouter->pAdapterPcieUp);

  PcieAdpCapCs0And.Value              = 0x7FFFFFFF;
  PcieAdpCapCs0Or.Value               = 0x80000000;

  PciePathOr.PathNProtocolCs0.Value = 0;
  PciePathOr.PathNProtocolCs1.Value = 0;

  LanePathOr.PathNLaneCs0.Value     = 0;
  LanePathOr.PathNLaneCs1.Value     = 0;

  PciePathAnd.PathNProtocolCs0.Value = 0xFFFFFFFF;
  PciePathAnd.PathNProtocolCs1.Value = 0xFFFFFFFF;
  PciePathAnd.PathNProtocolCs0.Field.OutputAdapter                    = 0;
  PciePathAnd.PathNProtocolCs0.Field.OutputHopID                      = 0;
  PciePathAnd.PathNProtocolCs0.Field.Valid                            = 0;
  PciePathAnd.PathNProtocolCs1.Field.CounterEnable                    = 0;
  PciePathAnd.PathNProtocolCs1.Field.Weight                           = 0;
  PciePathAnd.PathNProtocolCs1.Field.Priority                         = 0;
  PciePathAnd.PathNProtocolCs1.Field.EgressFlowControlFlag            = 0;
  PciePathAnd.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag  = 0;

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
  Status = PcieBufferAllocation (
             pParentDFPLaneAdapter,
             pParentRouter,
             pChildUFPLaneAdapter,
             pChildRouter
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to allocate PCIe buffers = %r\n", Status);
    return Status;
  }

  //
  // 1. Poll the ADP_PCIE_CS_0.LTSSM field in both PCIe Adapters until both are 0h
  //    (Indicating Detect state).
  //    Before procedure "EnumeratePcieAdapter", PCIe Adapter should be ready.
  //    So we just poll the LTSSM in Data Structure.
  //    If any of the 2 LTSSMs value is not 0, return EFI_DEVICE_ERROR
  //
  //    The connection manager doesn't pool the LTSSM state on a TBT3 Router.
  //
  if ( ( IsUSB4Router (pParentRouter) && 0 != pParentPcieDownAdp->PcieAdp_LTSSM )
    || ( IsUSB4Router (pChildRouter) && 0 != pChildAdapterPcieUp->PcieAdp_LTSSM ) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "One of the LTSSM value in PCIe Downstream Adapter and PCIe Upstream Adapter is not zero."
      " Quit the PCIe Path Setup\n"
      );
      return EFI_DEVICE_ERROR;
  }

  //
  // 2. Setup PCIe Downstream Path
  //

  // 2.1 Setup Parent Router PCIe Downstream Adapter Path CS
  if ( pParentDFPLaneAdapter->AdpHeader.MaxOutputHopId < PCIE_DOWNSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Parent DFP Lane max input HopID is %d which is smaller than %d.\n",
      pParentDFPLaneAdapter->AdpHeader.MaxOutputHopId,
      PCIE_DOWNSTREAM_PATH_HOPID
      );
    IDS_DEADLOOP ();
  }

  PciePathAnd.PathNProtocolCs0.Value = 0xFFFFFFFF;
  PciePathAnd.PathNProtocolCs1.Value = 0xFFFFFFFF;

  PciePathAnd.PathNProtocolCs0.Field.OutputAdapter                    = 0;
  PciePathAnd.PathNProtocolCs0.Field.OutputHopID                      = 0;
  PciePathAnd.PathNProtocolCs0.Field.Valid                            = 0;
  PciePathAnd.PathNProtocolCs1.Field.CounterEnable                    = 0;
  PciePathAnd.PathNProtocolCs1.Field.Weight                           = 0;
  PciePathAnd.PathNProtocolCs1.Field.Priority                         = 0;
  PciePathAnd.PathNProtocolCs1.Field.EgressFlowControlFlag            = 0;
  PciePathAnd.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag  = 0;

  PciePathOr.PathNProtocolCs0.Value   = 0x00000000;
  PciePathOr.PathNProtocolCs1.Value   = 0x00000000;

  PciePathOr.PathNProtocolCs0.Field.OutputAdapter                   = (UINT32)pParentDFPLaneAdapter->Header.AdapterNum;
  PciePathOr.PathNProtocolCs0.Field.OutputHopID                     = PCIE_DOWNSTREAM_PATH_HOPID;
  PciePathOr.PathNProtocolCs0.Field.Valid                           = 1;
  PciePathOr.PathNProtocolCs1.Field.CounterEnable                   = 0;
  PciePathOr.PathNProtocolCs1.Field.Weight                          = PCIE_TO_LANE_WEIGHT;
  PciePathOr.PathNProtocolCs1.Field.Priority                        = PCIE_TO_LANE_PRIORITY;
  PciePathOr.PathNProtocolCs1.Field.EgressFlowControlFlag           = PCIE_TO_LANE_EFC;
  PciePathOr.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag = PCIE_TO_LANE_ESE;

  if ( !IsUSB4Router (pParentRouter) ) {
    PciePathAnd.PathNProtocolCs0.Field.PathCreditsAllocated         = 0;
    PciePathOr.PathNProtocolCs0.Field.PathCreditsAllocated          = 7;

    PciePathAnd.PathNProtocolCs1.Field.IngressFlowControlFlag       = 0;
    PciePathOr.PathNProtocolCs1.Field.IngressFlowControlFlag        = 1;

    PciePathAnd.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag  = 0;
    PciePathOr.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag   = 0;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Parent Router [0x%lx] PCIe DownStream Adapter [%d] Path. PciePathAnd=0x%lx, PciePathOr=0x%lx\n",
    pParentPcieDownAdp->Header.TopologyID,
    pParentPcieDownAdp->Header.AdapterNum,
    *((UINT64*)(&PciePathAnd)),
    *((UINT64*)(&PciePathOr))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentPcieDownAdp->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pParentPcieDownAdp->Header.AdapterNum,               // AdapterNum
             8<<1,                                                // Address
             2,                                                   // SizeInDW
             (UINT32*)&(PciePathAnd),                             // AndMask
             (UINT32*)&(PciePathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pParentPcieDownAdp->PathValid = 1;

  // 2.2 Setup Child Router UFP Lane Adapter Path CS
  if ( pChildUFPLaneAdapter->AdpHeader.MaxInputHopId < PCIE_DOWNSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "UFP Lane max input HopID is %d which is smaller than %d.\n",
      pChildUFPLaneAdapter->AdpHeader.MaxOutputHopId,
      PCIE_DOWNSTREAM_PATH_HOPID
      );
    IDS_DEADLOOP ();
  }

  LanePathOr.PathNLaneCs0.Value = 0x00000000;
  LanePathOr.PathNLaneCs1.Value = 0x00000000;

  LanePathOr.PathNLaneCs0.Field.OutputAdapter                   = (UINT32)pChildAdapterPcieUp->Header.AdapterNum;
  LanePathOr.PathNLaneCs0.Field.OutputHopID                     = 8;
  LanePathOr.PathNLaneCs0.Field.Valid                           = 1;
  LanePathOr.PathNLaneCs0.Field.PathCreditsAllocated            = pChildUFPLaneAdapter->PcieCredits;
  LanePathOr.PathNLaneCs1.Field.CounterEnable                   = 0;
  LanePathOr.PathNLaneCs1.Field.Weight                          = LANE_TO_PCIE_WEIGHT;
  LanePathOr.PathNLaneCs1.Field.Priority                        = LANE_TO_PCIE_PRIORITY;
  LanePathOr.PathNLaneCs1.Field.IngressFlowControlFlag          = LANE_TO_PCIE_IFC;
  LanePathOr.PathNLaneCs1.Field.IngresSharedBufferingEnableFlag = LANE_TO_PCIE_ISE;
  LanePathOr.PathNLaneCs1.Field.EgressFlowControlFlag           = LANE_TO_PCIE_EFC;
  LanePathOr.PathNLaneCs1.Field.EgressSharedBufferingEnableFlag = LANE_TO_PCIE_ESE;

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
             PCIE_DOWNSTREAM_PATH_HOPID<<1,                       // Address
             2,                                                   // SizeInDW
             (UINT32*)&(LanePathAnd),                             // AndMask
             (UINT32*)&(LanePathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pChildUFPLaneAdapter->PcieTunnelOn = 1;

  //
  // 3. Setup PCIe Upstream Path
  //

  // 3.1 Setup Child Router PCIe Upstream Adapter Path CS
  if ( pChildUFPLaneAdapter->AdpHeader.MaxOutputHopId < PCIE_UPSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Child UPF Lane Adapter max Output HopID is %d which is smaller than %d.\n",
      pChildUFPLaneAdapter->AdpHeader.MaxOutputHopId,
      PCIE_UPSTREAM_PATH_HOPID
      );
    IDS_DEADLOOP ();
  }

  PciePathAnd.PathNProtocolCs0.Value = 0xFFFFFFFF;
  PciePathAnd.PathNProtocolCs1.Value = 0xFFFFFFFF;

  PciePathAnd.PathNProtocolCs0.Field.OutputAdapter                    = 0;
  PciePathAnd.PathNProtocolCs0.Field.OutputHopID                      = 0;
  PciePathAnd.PathNProtocolCs0.Field.Valid                            = 0;
  PciePathAnd.PathNProtocolCs1.Field.CounterEnable                    = 0;
  PciePathAnd.PathNProtocolCs1.Field.Weight                           = 0;
  PciePathAnd.PathNProtocolCs1.Field.Priority                         = 0;
  PciePathAnd.PathNProtocolCs1.Field.EgressFlowControlFlag            = 0;
  PciePathAnd.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag  = 0;

  PciePathOr.PathNProtocolCs0.Value   = 0x00000000;
  PciePathOr.PathNProtocolCs1.Value   = 0x00000000;

  PciePathOr.PathNProtocolCs0.Field.OutputAdapter                   = (UINT32)pChildUFPLaneAdapter->Header.AdapterNum;
  PciePathOr.PathNProtocolCs0.Field.OutputHopID                     = PCIE_UPSTREAM_PATH_HOPID;
  PciePathOr.PathNProtocolCs0.Field.Valid                           = 1;
  PciePathOr.PathNProtocolCs1.Field.CounterEnable                   = 0;
  PciePathOr.PathNProtocolCs1.Field.Weight                          = PCIE_TO_LANE_WEIGHT;
  PciePathOr.PathNProtocolCs1.Field.Priority                        = PCIE_TO_LANE_PRIORITY;
  PciePathOr.PathNProtocolCs1.Field.EgressFlowControlFlag           = PCIE_TO_LANE_EFC;
  PciePathOr.PathNProtocolCs1.Field.EgressSharedBufferingEnableFlag = PCIE_TO_LANE_ESE;

  if ( !IsUSB4Router (pChildRouter) ) {
    PciePathAnd.PathNProtocolCs0.Field.PathCreditsAllocated         = 0;
    PciePathOr.PathNProtocolCs0.Field.PathCreditsAllocated          = 7;

    PciePathAnd.PathNProtocolCs1.Field.IngressFlowControlFlag       = 0;
    PciePathOr.PathNProtocolCs1.Field.IngressFlowControlFlag        = 1;

    PciePathAnd.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag  = 0;
    PciePathOr.PathNProtocolCs1.Field.IngresSharedBufferingEnableFlag   = 0;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Child Router [0x%lx] PCIe Upstream Adapter [%d] Path. PciePathAnd=0x%lx, PciePathOr=0x%lx\n",
    pChildAdapterPcieUp->Header.TopologyID,
    pChildAdapterPcieUp->Header.AdapterNum,
    *((UINT64*)(&PciePathAnd)),
    *((UINT64*)(&PciePathOr))
    );
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pChildAdapterPcieUp->Header.TopologyID,
             CS_TARGET_PATH,                                      // CsType
             pChildAdapterPcieUp->Header.AdapterNum,              // AdapterNum
             8<<1,                                                // Address
             2,                                                   // SizeInDW
             (UINT32*)&(PciePathAnd),                             // AndMask
             (UINT32*)&(PciePathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pChildAdapterPcieUp->PathValid = 1;

  // 3.2 Setup Parent Router DFP Lane Adapter Path CS
  if ( pParentDFPLaneAdapter->AdpHeader.MaxInputHopId < PCIE_UPSTREAM_PATH_HOPID ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Parent DFP Lane max input HopID is %d which is smaller than %d.\n",
      pParentDFPLaneAdapter->AdpHeader.MaxInputHopId,
      PCIE_UPSTREAM_PATH_HOPID);
    IDS_DEADLOOP ();
  }

  LanePathOr.PathNLaneCs0.Value = 0x00000000;
  LanePathOr.PathNLaneCs1.Value = 0x00000000;

  LanePathOr.PathNLaneCs0.Field.OutputAdapter                   = (UINT32)pParentPcieDownAdp->Header.AdapterNum;
  LanePathOr.PathNLaneCs0.Field.OutputHopID                     = 8;
  LanePathOr.PathNLaneCs0.Field.Valid                           = 1;
  LanePathOr.PathNLaneCs0.Field.PathCreditsAllocated            = pParentDFPLaneAdapter->PcieCredits;
  LanePathOr.PathNLaneCs1.Field.CounterEnable                   = 0;
  LanePathOr.PathNLaneCs1.Field.Weight                          = LANE_TO_PCIE_WEIGHT;
  LanePathOr.PathNLaneCs1.Field.Priority                        = LANE_TO_PCIE_PRIORITY;
  LanePathOr.PathNLaneCs1.Field.IngressFlowControlFlag          = LANE_TO_PCIE_IFC;
  LanePathOr.PathNLaneCs1.Field.IngresSharedBufferingEnableFlag = LANE_TO_PCIE_ISE;
  LanePathOr.PathNLaneCs1.Field.EgressFlowControlFlag           = LANE_TO_PCIE_EFC;
  LanePathOr.PathNLaneCs1.Field.EgressSharedBufferingEnableFlag = LANE_TO_PCIE_ESE;

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
             PCIE_UPSTREAM_PATH_HOPID<<1,                         // Address
             2,                                                   // SizeInDW
             (UINT32*)&(LanePathAnd),                             // AndMask
             (UINT32*)&(LanePathOr)                               // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Status = %r\n", Status);
  if ( EFI_ERROR (Status) )
    return Status;

  pParentDFPLaneAdapter->PcieTunnelOn = 1;

  //
  // 4. Enable Transport Layer Packets on the PCIe Upstream Adapter
  //    Setting the Child Router PCIe Upstream Adapter ADP_PCIE_CS_0.Path Enable bit to 1b.
  //
  PcieAdpCapCs0And.Value              = 0x7FFFFFFF;
  PcieAdpCapCs0Or.Value               = 0x80000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Child Router [0x%lx] PCIe Upstream Adapter [%d] ADP_PCIE_CS_0.PathEnable bit[31] to 1b."
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

  pChildAdapterPcieUp->TunnelOn = 1;

  //
  // 5. Enable Transport Layer Packets on the PCIe Downstream Adapter
  //    Setting the Parent Router PCIe Downstream Adapter ADP_PCIE_CS_0.Path Enable bit to 1b.
  //
  PcieAdpCapCs0And.Value              = 0x7FFFFFFF;
  PcieAdpCapCs0Or.Value               = 0x80000000;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Set Parent Router [0x%lx] PCIe Downstream Adapter [%d] ADP_PCIE_CS_0.PathEnable bit[31] to 1b."
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

  pParentPcieDownAdp->TunnelOn = 1;

  // Check Additional requirement for Host Router
  if (pParentRouter->IsHost) {
    AmdUsb4PciePathInitHandle (pCM, pParentRouter, pParentPcieDownAdp);
  }

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Path for PCIe tunneling
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter         Point to Router Cm Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
PciePathEnable (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter
  )
{
  // TODO

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Additional sequence to enable PCIe Path on Host Router
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  *pHostRouter     Point to Host Router Cm Data structure
 * @param[in]  *pPcieDownAdp    Point to Host Router PCIe Downstream Adp
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
AmdUsb4PciePathInitHandle (
  IN      AMD_USB4_CM                           *pCM,
  IN      AMD_USB4_CM_ROUTER                    *pHostRouter,
  IN      AMD_USB4_CM_PCIE_ADAPTER              *pPcieDownAdp
  )
{
  UINT8               Timeout;
  UINT8               PcieIndex;
  UINT8               PcieRcBus;
  UINT8               PcieRcDev;
  UINT8               PcieRcFun;
  UINT32              SlotCntlSts;
  EFI_STATUS          Status;
  AMD_USB4_CM_CELL    *pAdapterPcieDown;

  PcieIndex        = 0;
  PcieRcBus        = 0;
  PcieRcDev        = 0;
  PcieRcFun        = 0;
  SlotCntlSts      = 0;
  Timeout          = 100;      // 1ms in 10us interval
  Status           = EFI_SUCCESS;
  pAdapterPcieDown = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pHostRouter || NULL == pPcieDownAdp || NULL == pCM ) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  // Check Host Router
  if ((pCM->HostRouterPciAddress.HostPciePortBus == 0)
       && (pCM->HostRouterPciAddress.HostPciePortDevFun.FunctionNum == 3)
       && (pCM->HostRouterPciAddress.HostPciePortDevFun.DeviceNum == 8)) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("AMD Soc Host Router!\n");
    pAdapterPcieDown = pHostRouter->pAdapterPcieDown;
    for (PcieIndex = 0; PcieIndex < AMD_USB4_MAX_HOST_PCIE; PcieIndex++) {
      if ((AMD_USB4_CM_CELL *)pPcieDownAdp == pAdapterPcieDown) {
        break;
      }
      if (pAdapterPcieDown->PCIEAdapter.AdpHeader.pSiblingAdapter == NULL) {
        IDS_HDT_CONSOLE_USB4_ERROR ("Cannot find this PCIe Downstream Adp in Host Router!\n");
        return EFI_DEVICE_ERROR;
      }
      pAdapterPcieDown = pAdapterPcieDown->PCIEAdapter.AdpHeader.pSiblingAdapter;
    }

    if (PcieIndex == AMD_USB4_MAX_HOST_PCIE) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot find this PCIe Downstream Adp in Host Router!!!\n");
      return EFI_DEVICE_ERROR;
    }

    PcieRcBus = pCM->PcieController[PcieIndex].Bus;
    PcieRcDev = pCM->PcieController[PcieIndex].DevFun.DeviceNum;
    PcieRcFun = pCM->PcieController[PcieIndex].DevFun.FunctionNum;

    if ((PcieRcBus ==0) && (PcieRcDev == 0) && (PcieRcFun ==0)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("No PCIe Root Port info found!\n");
      return EFI_DEVICE_ERROR;
    }

    // read SLOT_CNTL_STATUS
    SlotCntlSts = PciRead32 (PCI_LIB_ADDRESS (PcieRcBus, PcieRcDev, PcieRcFun, 0x70));
    do {
      // check for DL_STATE_CHANGED
      if (SlotCntlSts & BIT24) {
        SlotCntlSts &= 0x0000FFFF;                           // reserve SLOT_CNTL setting
        SlotCntlSts |= BIT19 + BIT24;                        // write 1 to clear
        PciWrite32 (
          PCI_LIB_ADDRESS (PcieRcBus, PcieRcDev, PcieRcFun, 0x70),
          SlotCntlSts
          );
        IDS_HDT_CONSOLE_USB4_DEBUG ("Detected and CLeared DL_STATE_CHANGED!\n");
        return EFI_SUCCESS;
      }

      // delay and read SLOT_STS again
      MicroSecondDelay (10);
      SlotCntlSts = PciRead32 (PCI_LIB_ADDRESS (PcieRcBus, PcieRcDev, PcieRcFun, 0x70));
      Timeout--;
    } while (Timeout);

    // Time out return error
    IDS_HDT_CONSOLE_USB4_DEBUG ("Do NOT detect DL_STATE_CHANGED! TIME OUT!\n");

    IDS_HDT_CONSOLE_USB4_DEBUG ("CLeared PRESENCE_DETECT_CHANGED and DL_STATE_CHANGED anyway!\n");
    SlotCntlSts &= 0x0000FFFF;                           // reserve SLOT_CNTL setting
    SlotCntlSts |= BIT19 + BIT24;                        // write 1 to clear
    PciWrite32 (
      PCI_LIB_ADDRESS (PcieRcBus, PcieRcDev, PcieRcFun, 0x70),
      SlotCntlSts
      );
    return EFI_TIMEOUT;
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate PCIe Adapter
 *
 * @param[in]  *pCM               Point to global structure AMD_USB4_CM
 * @param[in]  AdapterNum         Aapter Number
 * @param[in]  *pRouter           Point to Router CM Data structure
 * @param[in]  *pAdapter          Point to Adapter CM Data structure
 * @param[in]  *pAdapterCs        Point to Adapter Configuration Space data
 *
 * @param[out]  *pRouter          Point to Router CM Data structure
 * @param[out]  *pAdapter         Point to Adapter CM Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR       HW error.
**/
EFI_STATUS
EnumeratePcieAdapter (
  IN      AMD_USB4_CM                           *pCM,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_PCIE_ADAPTER              *pAdapter,
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

  //pAdapter is Zero out by default
  if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == UpStreamPCIeAdapter ) {
    pAdapter->Header.CellType = CELL_TYPE_PCIE_UP_ADAPTER;
  } else {
    pAdapter->Header.CellType = CELL_TYPE_PCIE_DOWN_ADAPTER;
  }
  if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == UpStreamPCIeAdapter ) {
    pAdapter->Header.CellType = CELL_TYPE_PCIE_UP_ADAPTER;
  } else {
    pAdapter->Header.CellType = CELL_TYPE_PCIE_DOWN_ADAPTER;
  }
  pAdapter->Header.TopologyID = pRouter->Header.TopologyID;
  pAdapter->Header.AdapterNum = (UINT8)AdapterNum;
  pAdapter->Header.Parent     = (AMD_USB4_CM_CELL_HEADER*)pRouter;

  pAdapter->AdpHeader.MaxInputHopId       = (UINT16)(pAdapterCs->AdpCs5.Field.MaxInputHopID);
  pAdapter->AdpHeader.MaxOutputHopId      = (UINT16)(pAdapterCs->AdpCs5.Field.MaxOutputHopID);

  pAdapter->PcieCapability  = pAdapterCs->AdpCs1.Field.NextCapabilityPointer;
  pAdapter->PathValid       = 0;
  pAdapter->TunnelOn        = 0;

  if ( pAdapter->PcieCapability == 0 ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("PCIe Adapter Capability offset is 0.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // PCIe Adapter just have one capability.
  //
  DataSize = sizeof (AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
  // Read PCIe Capability Registers
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
  if (EFI_ERROR (Status)
    || ( sizeof (AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) != DataSize )
  {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read PCIe Adapter Capability Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  if ( DataPtr && ((AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->Field.CapabilityID == 4 ) {
    pAdapter->PcieAdp_Link   = ((AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->Field.Link;
    pAdapter->PcieAdp_TXEI   = ((AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->Field.TxE1;
    pAdapter->PcieAdp_RXEI   = ((AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->Field.RxE1;
    pAdapter->PcieAdp_RST    = ((AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->Field.RST;
    pAdapter->PcieAdp_LTSSM  = ((AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY*)DataPtr)->Field.LTTSM;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR ("PCIe Adapter Capability ID is not 4.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Increase PCIe adapter amount in Router CM Data Structure.
  //
  if ( pAdapter->Header.CellType == CELL_TYPE_PCIE_UP_ADAPTER ) {
    pRouter->PcieUpAmount++;
  } else if ( pAdapter->Header.CellType == CELL_TYPE_PCIE_DOWN_ADAPTER ) {
    pRouter->PcieDownAmount++;
  }

  pAdapter->AdpHeader.pSiblingAdapter     = NULL;
  pAdapter->AdpHeader.pLinkPartner        = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("PCIe Adapter Information:\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterNum     = 0x%lx\n",       pAdapter->Header.AdapterNum);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CellType       = 0x%x\n",        pAdapter->Header.CellType);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxInputHopId  = 0x%x\n",        pAdapter->AdpHeader.MaxInputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxOutputHopId = 0x%x\n",        pAdapter->AdpHeader.MaxOutputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Link           = 0x%x\n",        pAdapter->PcieAdp_Link);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TxE1           = 0x%x\n",        pAdapter->PcieAdp_TXEI);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  RxE1           = 0x%x\n",        pAdapter->PcieAdp_RXEI);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  RST            = 0x%x\n",        pAdapter->PcieAdp_RST);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  LTTSM          = 0x%x\n",        pAdapter->PcieAdp_LTSSM);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  PcieCapability = 0x%x\n",        pAdapter->PcieCapability);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  pRouter->PcieUpAmount   = 0x%x\n",  pRouter->PcieUpAmount);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  pRouter->PcieDownAmount = 0x%x\n",  pRouter->PcieDownAmount);

  return EFI_SUCCESS;
}

