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
#include <Library/AmdUsb4Lib.h>

#define FILECODE LIBRARY_AMDUSB4LIB_BANDWIDTH_FILECODE

EFI_STATUS
DpMainSetNonFlowControlBuffers (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     UINT8                                AdapterNum,
  IN     UINT32                               BufferAllocationDpMain
  )
{
  EFI_STATUS                Status;
  UINT32                    DataSize;
  UINT32                    *DataPtr;

  Status                  = EFI_SUCCESS;
  DataSize                = 0;
  DataPtr                 = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ADAPTER,                     // CsType
             (UINT32) AdapterNum,                   // AdapterNum
             4,                                     // ReadAddress
             DataSize,                              // ReadSize
             (UINT32**) &DataPtr,                   // DataOut
             &DataSize                              // DataSize
             );
  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read AMD_USB4_ADP_DP_OUT_STATUS_CTRL Register.\n");
    return EFI_DEVICE_ERROR;
  }

  ((AMD_USB4_ADP_CS4 *) DataPtr)->Field.NonFlowControlledBuffers = BufferAllocationDpMain;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) AdapterNum,
             4,
             &DataSize,
             DataPtr
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" Failed to write AMD_USB4_ADP_CS4.NonFlowControlledBuffers Register.\n");
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
 * Allocate credits for DisplayPort tunneling
 *
 * @param[in]  *pCM                       Pointer to global structure AMD_USB4_CM.
 * @param[in]  *pIngressAdapter           Pointer to a path Ingress Adapter.
 * @param[in]  DpStreamNum                Number of current DP Adapters paired.
 * @param[in]  BufferAllocationDpAux      Number of buffers to allocate for DP AUX.
 * @param[in]  BufferAllocationDpMain     Number of buffers to allocate for DP MAIN.
 * @param[in]  IsDpMainPath               Determine if currently DisplayPort Main Path Segment.
 *
 * @retval EFI_SUCCESS                    Success.
 * @retval RETURN_BUFFER_TOO_SMALL        Can not allocate minimum buffers for DP Paths.
**/
EFI_STATUS
DpBufferAllocation (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_CELL_HEADER             *pIngressAdapter,
  IN     UINT32                               DpStreamNum,
  IN     UINT16                               BufferAllocationDpAux,
  IN     UINT16                               BufferAllocationDpMain,
  IN     BOOLEAN                              IsDpMainPath
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_ROUTER        *pRouter;
  UINT32                    DataSize;
  UINT32                    *DataPtr;
  UINT32                    NumDpStreamAllocating;
  UINT32                    NumDpStreamSupported;
  UINT16                    Usb3Credits;
  UINT16                    PcieCredits;
  UINT16                    MinPcieCredits;
  UINT32                    TempDpCredits;
  AMD_USB4_CM_LANE_ADAPTER  *pIngressLaneAdapter;
  AMD_USB4_CM_DP_ADAPTER    *pDpAdapter;

  Status                  = EFI_SUCCESS;
  pRouter                 = (AMD_USB4_CM_ROUTER *) pIngressAdapter->Parent;
  DataSize                = 0;
  DataPtr                 = 0;
  NumDpStreamAllocating   = 0;
  NumDpStreamSupported    = 0;
  TempDpCredits           = 0;
  Usb3Credits             = 0;
  PcieCredits             = 0;
  MinPcieCredits          = MIN_PCIE_PATH_CREDITS;
  pIngressLaneAdapter     = NULL;
  pDpAdapter              = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  if (pIngressAdapter->CellType == CELL_TYPE_LANE_ADAPTER) {
    pIngressLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter;
  } else {
    pDpAdapter = (AMD_USB4_CM_DP_ADAPTER *) pIngressAdapter;
  }

  if (pIngressLaneAdapter != NULL) {
    ///
    /// Account for  DP Link #2 passing through a Lane Adapter without DP Link #1
    ///
    if (((DpStreamNum + 1) == 2) && (pIngressLaneAdapter->DpMainTunnel1On == 0)) {
      NumDpStreamAllocating   = DpStreamNum;
    } else {
      NumDpStreamAllocating   = DpStreamNum + 1;
    }

    TempDpCredits           = (UINT32)(BufferAllocationDpAux + BufferAllocationDpMain);

    if (IsUSB4Router (pRouter)) {
      Usb3Credits             = pRouter->BufferAllocationMaxUSB3;
      PcieCredits             = pRouter->BufferAllocationMaxPcie;
      MinPcieCredits          = MIN_PCIE_PATH_CREDITS;
    } else {
      Usb3Credits             = 0;
      PcieCredits             = ( pIngressLaneAdapter->LaneAdp_CurrentWidth ) ? 32 : 16;
      MinPcieCredits          = PcieCredits;
    }

    //
    // Determine number of DisplayPort Streams that can be supported
    //
    if (pIngressLaneAdapter->TotalBuffer >= (pIngressLaneAdapter->ControlPathCredits + Usb3Credits + PcieCredits)) {
      NumDpStreamSupported = ((pIngressLaneAdapter->TotalBuffer -
                               pIngressLaneAdapter->ControlPathCredits -
                               (UINT32)Usb3Credits -
                               (UINT32)PcieCredits) /
                              TempDpCredits);
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Number of DP Streams supported    = %x\n"
      "Number of DP Streams to allocate  = %x\n"
      "  Total Buffers                   = %x\n"
      "  Control Path Credits            = %x\n"
      "  USB3 Credits                    = %x\n"
      "  PCIe Credits                    = %x\n"
      "  DP Credits                      = %x\n",
      NumDpStreamSupported,
      NumDpStreamAllocating,
      pIngressLaneAdapter->TotalBuffer,
      pIngressLaneAdapter->ControlPathCredits,
      Usb3Credits,
      PcieCredits,
      TempDpCredits
      );

    //
    // Determine if number of DisplayPort streams supported needs to be increased
    // Determine if credits can be allocated by PCIe Tunneling
    //
    if (NumDpStreamAllocating > NumDpStreamSupported) {
      if (pIngressLaneAdapter->TotalBuffer >= (pIngressLaneAdapter->ControlPathCredits + Usb3Credits + MinPcieCredits)) {
        NumDpStreamSupported = ((UINT32)(pIngressLaneAdapter->TotalBuffer -
                                 pIngressLaneAdapter->ControlPathCredits -
                                 (UINT32)Usb3Credits -
                                 MinPcieCredits) /
                                TempDpCredits);
      }
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "WARNING: Allocate credits from PCIe Credits to increase Number of DP Streams supported.\n"
        "INFO: Recalculated Number of DP Streams supported = %x\n",
        NumDpStreamSupported
        );

      if ((NumDpStreamAllocating > NumDpStreamSupported)) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Not enough credits available to support additional DP Stream.\n");
        return RETURN_BUFFER_TOO_SMALL;
      }
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Buffer Allocation - Ingress Adapter: 0x%x - TopologyID: 0x%x\n"
      "  Buffers Required      = 0x%x\n"
      "  Remaining Buffers     = 0x%x\n"
      "  Total DP Credits      = 0x%x\n"
      "    Link 1: DP Main Path Credits  = 0x%x\n"
      "    Link 2: DP Main Path Credits  = 0x%x\n"
      "    Link 1: DP AUX Path Credits   = 0x%x\n"
      "    Link 2: DP AUX Path Credits   = 0x%x\n",
      pIngressLaneAdapter->Header.AdapterNum,
      pIngressLaneAdapter->Header.TopologyID,
      TempDpCredits,
      pIngressLaneAdapter->RemainingBuffers,
      pIngressLaneAdapter->TotalDpCredits,
      pIngressLaneAdapter->DpMainPath1Credits,
      pIngressLaneAdapter->DpMainPath2Credits,
      pIngressLaneAdapter->DpAuxPath1Credits,
      pIngressLaneAdapter->DpAuxPath2Credits
      );

    //
    // Determine if the number of credits needed for DP Tunneling can be allocated
    // In the event of multiple DP Streams, increase the amount of allocated DP Credits if available buffers remain
    //
    if ((IsDpMainPath) && (pIngressLaneAdapter->RemainingBuffers >= TempDpCredits)) {
      pIngressLaneAdapter->RemainingBuffers -= TempDpCredits;
      if (DpStreamNum == 0) {
        pIngressLaneAdapter->DpAuxPath1Credits += (UINT32) BufferAllocationDpAux;
        pIngressLaneAdapter->DpMainPath1Credits += (UINT32) BufferAllocationDpMain;
      } else if (DpStreamNum == 1) {
        pIngressLaneAdapter->DpAuxPath2Credits += (UINT32) BufferAllocationDpAux;
        pIngressLaneAdapter->DpMainPath2Credits += (UINT32) BufferAllocationDpMain;
      }
      pIngressLaneAdapter->TotalDpCredits = pIngressLaneAdapter->DpAuxPath1Credits +
                                            pIngressLaneAdapter->DpAuxPath2Credits +
                                            pIngressLaneAdapter->DpMainPath1Credits +
                                            pIngressLaneAdapter->DpMainPath2Credits;
    } else if ((!IsDpMainPath) && (pIngressLaneAdapter->RemainingBuffers >= (UINT32) BufferAllocationDpAux)) {
      pIngressLaneAdapter->RemainingBuffers -= (UINT32) BufferAllocationDpAux;
      if (DpStreamNum == 0) {
        pIngressLaneAdapter->DpAuxPath1Credits += (UINT32) BufferAllocationDpAux;
      } else if (DpStreamNum == 1) {
        pIngressLaneAdapter->DpAuxPath2Credits += (UINT32) BufferAllocationDpAux;
      }
      pIngressLaneAdapter->TotalDpCredits = pIngressLaneAdapter->DpAuxPath1Credits +
                                            pIngressLaneAdapter->DpAuxPath2Credits +
                                            pIngressLaneAdapter->DpMainPath1Credits +
                                            pIngressLaneAdapter->DpMainPath2Credits;
    } else {
      IDS_HDT_CONSOLE_USB4_ERROR (
       " Can not allocate minimum buffers for DP Paths.\n"
        "  Remaining Buffers             = 0x%x\n"
        "  Additional DP Credits Needed  = 0x%x\n",
        pIngressLaneAdapter->RemainingBuffers,
        TempDpCredits
        );
      return RETURN_BUFFER_TOO_SMALL;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Updated:\n"
      "  Remaining Buffers     = 0x%x\n"
      "  Total DP Credits      = 0x%x\n"
      "    Link 1: DP Main Path Credits  = 0x%x\n"
      "    Link 2: DP Main Path Credits  = 0x%x\n"
      "    Link 1: DP AUX Path Credits   = 0x%x\n"
      "    Link 2: DP AUX Path Credits   = 0x%x\n",
      pIngressLaneAdapter->RemainingBuffers,
      pIngressLaneAdapter->TotalDpCredits,
      pIngressLaneAdapter->DpMainPath1Credits,
      pIngressLaneAdapter->DpMainPath2Credits,
      pIngressLaneAdapter->DpAuxPath1Credits,
      pIngressLaneAdapter->DpAuxPath2Credits
      );
  }

  //
  // Flow Control Disabled Scheme for DP Main Path
  // Program ADP_CS4.Non-Flow Controlled buffers
  //
  if (IsDpMainPath) {
    if (pIngressLaneAdapter != NULL) {
      Status = DpMainSetNonFlowControlBuffers (
                 pCM,
                 pRouter,
                 pRouter->UpstreamAdapter,
                 pIngressLaneAdapter->DpMainPath1Credits + pIngressLaneAdapter->DpMainPath2Credits
                 );
    } else {
      Status = DpMainSetNonFlowControlBuffers (
                 pCM,
                 pRouter,
                 pDpAdapter->Header.AdapterNum,
                 BufferAllocationDpMain
                 );
    }

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Failed to write DP Main Path Non-Flow Controller Buffers.\n"
        );
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
 * Deallocate credits for DisplayPort tunneling
 *
 * @param[in]  *pCM                       Pointer to global structure AMD_USB4_CM.
 * @param[in]  *pIngressAdapter           Pointer to a path Ingress Adapter.
 * @param[in]  BufferAllocationDpAux      Number of buffers to allocate for DP AUX.
 * @param[in]  BufferAllocationDpMain     Number of buffers to allocate for DP MAIN.
 * @param[in]  IsDpMainPath               Determine if currently DisplayPort Main Path Segment.
 *
 * @retval EFI_SUCCESS                    Success.
 * @retval RETURN_BUFFER_TOO_SMALL        Can not allocate minimum buffers for DP Paths.
**/
EFI_STATUS
DpBufferDeallocation (
  IN     AMD_USB4_CM                         *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER              *pDpIn,
  IN     AMD_USB4_CM_CELL_HEADER             *pIngressAdapter,
  IN     UINT16                               BufferAllocationDpAux,
  IN     UINT16                               BufferAllocationDpMain,
  IN     BOOLEAN                              IsDpMainPath
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_ROUTER        *pRouter;
  UINT32                    TempDpCredits;
  AMD_USB4_CM_LANE_ADAPTER  *pIngressLaneAdapter;
  AMD_USB4_CM_DP_ADAPTER    *pDpAdapter;

  Status                  = EFI_SUCCESS;
  pRouter                 = (AMD_USB4_CM_ROUTER *) pIngressAdapter->Parent;
  TempDpCredits           = 0;
  pIngressLaneAdapter     = NULL;
  pDpAdapter              = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  if (pIngressAdapter->CellType == CELL_TYPE_LANE_ADAPTER) {
    pIngressLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter;
  } else {
    pDpAdapter = (AMD_USB4_CM_DP_ADAPTER *) pIngressAdapter;
  }

  if (pIngressLaneAdapter != NULL) {
    if (pDpIn->DpLinkNumber == 1) {
      TempDpCredits = pIngressLaneAdapter->DpAuxPath1Credits + pIngressLaneAdapter->DpMainPath1Credits;
    } else if (pDpIn->DpLinkNumber == 2) {
      TempDpCredits = pIngressLaneAdapter->DpAuxPath2Credits + pIngressLaneAdapter->DpMainPath2Credits;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Buffer Deallocation - Ingress Adapter: 0x%x - TopologyID: 0x%x\n"
      "  Buffers to free       = 0x%x\n"
      "  Remaining Buffers     = 0x%x\n"
      "  Total DP Credits      = 0x%x\n"
      "    Link 1: DP Main Path Credits  = 0x%x\n"
      "    Link 2: DP Main Path Credits  = 0x%x\n"
      "    Link 1: DP AUX Path Credits   = 0x%x\n"
      "    Link 2: DP AUX Path Credits   = 0x%x\n",
      pIngressLaneAdapter->Header.AdapterNum,
      pIngressLaneAdapter->Header.TopologyID,
      TempDpCredits,
      pIngressLaneAdapter->RemainingBuffers,
      pIngressLaneAdapter->TotalDpCredits,
      pIngressLaneAdapter->DpMainPath1Credits,
      pIngressLaneAdapter->DpMainPath2Credits,
      pIngressLaneAdapter->DpAuxPath1Credits,
      pIngressLaneAdapter->DpAuxPath2Credits
      );

    //
    // Determine the number of credits that will be deallocated
    //
    if (IsDpMainPath) {
      pIngressLaneAdapter->RemainingBuffers += TempDpCredits;
      if (pDpIn->DpLinkNumber == 1) {
        pIngressLaneAdapter->DpAuxPath1Credits = 0;
        pIngressLaneAdapter->DpMainPath1Credits = 0;
      } else if (pDpIn->DpLinkNumber == 2) {
        pIngressLaneAdapter->DpAuxPath2Credits = 0;
        pIngressLaneAdapter->DpMainPath2Credits = 0;
      }
      pIngressLaneAdapter->TotalDpCredits = pIngressLaneAdapter->DpAuxPath1Credits +
                                            pIngressLaneAdapter->DpAuxPath2Credits +
                                            pIngressLaneAdapter->DpMainPath1Credits +
                                            pIngressLaneAdapter->DpMainPath2Credits;
    } else if (!IsDpMainPath) {
      pIngressLaneAdapter->RemainingBuffers += TempDpCredits;
      if (pDpIn->DpLinkNumber == 1) {
        pIngressLaneAdapter->DpAuxPath1Credits = 0;
      } else if (pDpIn->DpLinkNumber == 2) {
        pIngressLaneAdapter->DpAuxPath2Credits = 0;
      }
      pIngressLaneAdapter->TotalDpCredits = pIngressLaneAdapter->DpAuxPath1Credits +
                                            pIngressLaneAdapter->DpAuxPath2Credits +
                                            pIngressLaneAdapter->DpMainPath1Credits +
                                            pIngressLaneAdapter->DpMainPath2Credits;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Updated:\n"
      "  Remaining Buffers     = 0x%x\n"
      "  Total DP Credits      = 0x%x\n"
      "    Link 1: DP Main Path Credits  = 0x%x\n"
      "    Link 2: DP Main Path Credits  = 0x%x\n"
      "    Link 1: DP AUX Path Credits   = 0x%x\n"
      "    Link 2: DP AUX Path Credits   = 0x%x\n",
      pIngressLaneAdapter->RemainingBuffers,
      pIngressLaneAdapter->TotalDpCredits,
      pIngressLaneAdapter->DpMainPath1Credits,
      pIngressLaneAdapter->DpMainPath2Credits,
      pIngressLaneAdapter->DpAuxPath1Credits,
      pIngressLaneAdapter->DpAuxPath2Credits
      );
  }

  //
  // Flow Control Disabled Scheme for DP Main Path
  // Program ADP_CS4.Non-Flow Controlled buffers
  //
  if (IsDpMainPath) {
    if (pIngressLaneAdapter != NULL) {
      Status = DpMainSetNonFlowControlBuffers (
                 pCM,
                 pRouter,
                 pRouter->UpstreamAdapter,
                 pIngressLaneAdapter->DpMainPath1Credits + pIngressLaneAdapter->DpMainPath2Credits
                 );
    } else {
      Status = DpMainSetNonFlowControlBuffers (
                 pCM,
                 pRouter,
                 pDpAdapter->Header.AdapterNum,
                 0
                 );
    }

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Failed to write DP Main Path Non-Flow Controller Buffers.\n"
        );
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
 * Deallocate bandwidth for DisplayPort tunneling
 *
 * @param[in]  pDpIn                Pointer to DP IN Adapter.
 * @param[in]  *pIngressAdapter     Pointer to a path Ingress Adapter.
**/
VOID
DpBandwidthDeallocation (
  IN     AMD_USB4_CM_DP_ADAPTER    *pDpIn,
  IN     AMD_USB4_CM_LANE_ADAPTER  *pIngressAdapter
  )
{
  UINT32      Bandwidth;

  Bandwidth = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  if (pDpIn->DpLinkNumber == 1) {
    Bandwidth = pIngressAdapter->DownstreamDpPath1Bandwidth;
  } else if (pDpIn->DpLinkNumber == 2) {
    Bandwidth = pIngressAdapter->DownstreamDpPath2Bandwidth;
  }

  //
  // Update Available Bandwidth and DisplayPort Bandwidth
  //
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Bandwidth:\n"
    "  DP Path Bandwidth to free from DP Link %x              = %d\n"
    "  Ingress Adapter's Available Bandwidth                 = %d\n"
    "  Ingress Adapter's Total Downstream DP Path Bandwidth  = %d\n"
    "    Link 1: Total Downstream DP Path Bandwidth          = %d\n"
    "    Link 2: Total Downstream DP Path Bandwidth          = %d\n",
    pDpIn->DpLinkNumber,
    Bandwidth,
    pIngressAdapter->AvailableBandwidth,
    pIngressAdapter->TotalDownstreamDpPathBandwidth,
    pIngressAdapter->DownstreamDpPath1Bandwidth,
    pIngressAdapter->DownstreamDpPath2Bandwidth
    );

  pIngressAdapter->AvailableBandwidth += Bandwidth;
  pIngressAdapter->TotalDownstreamDpPathBandwidth -= Bandwidth;
  if (pDpIn->DpLinkNumber == 1) {
    pIngressAdapter->DownstreamDpPath1Bandwidth -= Bandwidth;
  } else if (pDpIn->DpLinkNumber == 2) {
    pIngressAdapter->DownstreamDpPath2Bandwidth -= Bandwidth;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Updated:\n"
    "  Ingress Adapter's Available Bandwidth                 = %d\n"
    "  Ingress Adapter's Total Downstream DP Path Bandwidth  = %d\n"
    "    Link 1: Total Downstream DP Path Bandwidth          = %d\n"
    "    Link 2: Total Downstream DP Path Bandwidth          = %d\n",
    pIngressAdapter->AvailableBandwidth,
    pIngressAdapter->TotalDownstreamDpPathBandwidth,
    pIngressAdapter->DownstreamDpPath1Bandwidth,
    pIngressAdapter->DownstreamDpPath2Bandwidth
    );
}

/**
 * Setup bandwidth for DisplayPort tunneling
 *
 * @param[in]  Bandwidth            Bandwidth to be allocated.
 * @param[in]  *pIngressAdapter     Pointer to a path Ingress Adapter.
 * @param[in]  *NumPairedAdapters   Number of currently paired adapters.
**/
VOID
DpBandwidthAllocation (
  IN     UINT32                                Bandwidth,
  IN     AMD_USB4_CM_LANE_ADAPTER              *pIngressAdapter,
  IN     UINT32                                NumPairedAdapters
  )
{
  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  //
  // Update Available Bandwidth and DisplayPort Bandwidth
  //
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Bandwidth:\n"
    "  Requested DP Path Bandwidth                           = %d\n"
    "  Ingress Adapter's Available Bandwidth                 = %d\n"
    "  Ingress Adapter's Total Downstream DP Path Bandwidth  = %d\n"
    "    Link 1: Total Downstream DP Path Bandwidth          = %d\n"
    "    Link 2: Total Downstream DP Path Bandwidth          = %d\n",
    Bandwidth,
    pIngressAdapter->AvailableBandwidth,
    pIngressAdapter->TotalDownstreamDpPathBandwidth,
    pIngressAdapter->DownstreamDpPath1Bandwidth,
    pIngressAdapter->DownstreamDpPath2Bandwidth
    );

  pIngressAdapter->AvailableBandwidth -= Bandwidth;
  pIngressAdapter->TotalDownstreamDpPathBandwidth += Bandwidth;
  if (NumPairedAdapters == 0) {
    pIngressAdapter->DownstreamDpPath1Bandwidth += Bandwidth;
  } else if (NumPairedAdapters == 1) {
    pIngressAdapter->DownstreamDpPath2Bandwidth += Bandwidth;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Updated:\n"
    "  Ingress Adapter's Available Bandwidth                 = %d\n"
    "  Ingress Adapter's Total Downstream DP Path Bandwidth  = %d\n"
    "    Link 1: Total Downstream DP Path Bandwidth          = %d\n"
    "    Link 2: Total Downstream DP Path Bandwidth          = %d\n",
    pIngressAdapter->AvailableBandwidth,
    pIngressAdapter->TotalDownstreamDpPathBandwidth,
    pIngressAdapter->DownstreamDpPath1Bandwidth,
    pIngressAdapter->DownstreamDpPath2Bandwidth
    );
}

/**
 * Reallocate bandwidth for DisplayPort tunneling
 *
 * @param[in]  *pCM                 Pointer to global structure AMD_USB4_CM.
 * @param[in]  *pDpIn               Pointer to a DP IN Adapter.
 * @param[in]  *pDpIn               Pointer to a DP OUT Adapter.
 * @param[in]  *pIngressAdapter     Pointer to a path Ingress Adapter.
 * @param[in]  DpPathBandwidth      Bandwidth for DisplayPort Tunneling Main-Link Path.
**/
VOID
DpBandwidthReallocation (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER               *pDpIn,
  IN     AMD_USB4_CM_DP_ADAPTER               *pDpOut,
  IN     AMD_USB4_CM_CELL_HEADER              *pIngressAdapter,
  IN     UINT32                                DpPathBandwidth
  )
{
  AMD_USB4_CM_ROUTER        *pRouter;
  AMD_USB4_CM_CELL_HEADER  *pNextIngressAdapter;

  pRouter              = (AMD_USB4_CM_ROUTER *) pIngressAdapter->Parent;
  pNextIngressAdapter  = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

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

    //
    // Continue recursively along path enumerating Path on Pass-through USB4 Hub locate DP IN
    //
    pNextIngressAdapter = pRouter->Header.Parent;

    DpBandwidthReallocation (
      pCM,
      pDpIn,
      pDpOut,
      pNextIngressAdapter,
      DpPathBandwidth
      );

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
    // Bandwidth reallocation
    //
    DpBandwidthDeallocation (pDpIn, (AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP);
    DpBandwidthAllocation (
      DpPathBandwidth,
      (AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP,
      (pDpIn->DpLinkNumber - 1)
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
     // return EFI_UNSUPPORTED;
    }

    //
    // Bandwidth reallocation
    //
    DpBandwidthDeallocation (pDpIn, (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter);
    DpBandwidthAllocation (
      DpPathBandwidth,
      (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter,
      (pDpIn->DpLinkNumber - 1)
      );

    //
    // Continue recursively enumerating Path on Pass-through USB4 Hub until DP IN Adapter's Router is located
    //
    pNextIngressAdapter = (AMD_USB4_CM_CELL_HEADER *) pRouter->Header.Parent;

    DpBandwidthReallocation (
      pCM,
      pDpIn,
      pDpOut,
      pNextIngressAdapter,
      DpPathBandwidth
      );

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
    // Bandwidth reallocation
    //
    DpBandwidthDeallocation (pDpIn, (AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP);
    DpBandwidthAllocation (
      DpPathBandwidth,
      (AMD_USB4_CM_LANE_ADAPTER *) pRouter->pAdapterUFP,
      (pDpIn->DpLinkNumber - 1)
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
    // Bandwidth reallocation
    //
    DpBandwidthDeallocation (pDpIn, (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter);
    DpBandwidthAllocation (
      DpPathBandwidth,
      (AMD_USB4_CM_LANE_ADAPTER *) pIngressAdapter,
      (pDpIn->DpLinkNumber - 1)
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
  }
}

/**
 * Allocate buffers for USB3 tunneling
 *
 * @param[in]  *pParentDFPLaneAdapter      Pointer to Parent Down Facing Port Lane Adapter
 * @param[in]  ParentRouterMaxUsb3Buffers  Max USB3 Buffers preferred by Parent Router
 * @param[in]  *pChildUFPLaneAdapter       Pointer to Child Up Facing Port Lane Adapter
 * @param[in]  ChildRouterMaxUsb3Buffers   Max USB3 Buffers preferred by Parent Router
 *
 * @param[out]  *pParentDFPLaneAdapter     Pointer to Parent Down Facing Port Lane Adapter
 * @param[out]  *pChildUFPLaneAdapter      Pointer to Child Up Facing Port Lane Adapter
 *
 * @retval EFI_SUCCESS                     Success.
 * @retval EFI_BUFFER_TOO_SMALL            Can not allocate minimum buffers for USB3 Path.
**/
EFI_STATUS
Usb3BufferAllocation (
  IN OUT AMD_USB4_CM_LANE_ADAPTER            *pParentDFPLaneAdapter,
  IN     UINT32                              ParentRouterMaxUsb3Buffers,
  IN OUT AMD_USB4_CM_LANE_ADAPTER            *pChildUFPLaneAdapter,
  IN     UINT32                              ChildRouterMaxUsb3Buffers
  )
{
  EFI_STATUS                Status;
  UINT32                    ParentCredits;
  UINT32                    ChildCredits;

  Status                = EFI_SUCCESS;
  ParentCredits         = 0;
  ChildCredits          = 0;

  //
  // Determine the number of credits Parent/Child Lane Adapters need for USB3 Tunneling and if they can be allocated
  //
  if (pChildUFPLaneAdapter->RemainingBuffers >= ChildRouterMaxUsb3Buffers) {
    ChildCredits = ChildRouterMaxUsb3Buffers;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR (" Unable to guarantee maximum buffers for USB3 Tunneling.\n");
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Buffer Allocation - Child UFP Ingress Adapter:\n"
    "  Buffers Required      = 0x%x\n"
    "  Remaining Buffers     = 0x%x\n"
    "  USB3 Path Credits     = 0x%x\n",
    ChildRouterMaxUsb3Buffers,
    pChildUFPLaneAdapter->RemainingBuffers,
    pChildUFPLaneAdapter->Usb3Credits
    );

  if (ChildCredits != 0) {
    pChildUFPLaneAdapter->RemainingBuffers -= ChildCredits;
    pChildUFPLaneAdapter->Usb3Credits += ChildCredits;
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Updated:\n"
      "  Remaining Buffers     = 0x%x\n"
      "  USB3 Path Credits     = 0x%x\n",
      pChildUFPLaneAdapter->RemainingBuffers,
      pChildUFPLaneAdapter->Usb3Credits
      );
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR (
     " Can not allocate minimum buffers for USB3 Path.\n"
      "  Remaining Buffers                    = 0x%x\n"
      "  Additional USB3 Path Credits Needed  = 0x%x\n",
      pChildUFPLaneAdapter->RemainingBuffers,
      ChildRouterMaxUsb3Buffers
      );
    return EFI_BUFFER_TOO_SMALL;
  }

  if (pParentDFPLaneAdapter->RemainingBuffers >= ParentRouterMaxUsb3Buffers) {
    ParentCredits = ParentRouterMaxUsb3Buffers;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR (" Unable to guarantee maximum buffers for USB3 Tunneling.\n");
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Buffer Allocation - Parent DFP Ingress Adapter:\n"
    "  Buffers Required      = 0x%x\n"
    "  Remaining Buffers     = 0x%x\n"
    "  USB3 Path Credits     = 0x%x\n",
    ParentRouterMaxUsb3Buffers,
    pParentDFPLaneAdapter->RemainingBuffers,
    pParentDFPLaneAdapter->Usb3Credits
    );

  if (ParentCredits != 0) {
    pParentDFPLaneAdapter->RemainingBuffers -= ParentCredits;
    pParentDFPLaneAdapter->Usb3Credits += ParentCredits;
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Updated:\n"
      "  Remaining Buffers     = 0x%x\n"
      "  USB3 Path Credits     = 0x%x\n",
      pParentDFPLaneAdapter->RemainingBuffers,
      pParentDFPLaneAdapter->Usb3Credits
      );
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR (
     " Can not allocate minimum buffers for USB3 Path.\n"
      "  Remaining Buffers                    = 0x%x\n"
      "  Additional USB3 Path Credits Needed  = 0x%x\n",
      pParentDFPLaneAdapter->RemainingBuffers,
      ParentRouterMaxUsb3Buffers
      );

    //
    // Free allocated buffers from Child UFP Lane Adapter
    //
    pChildUFPLaneAdapter->RemainingBuffers += ChildCredits;
    pChildUFPLaneAdapter->Usb3Credits -= ChildCredits;

    return EFI_BUFFER_TOO_SMALL;
  }

  return EFI_SUCCESS;
}

/**
 * Configure bandwidth for USB3 Tunneling Path
 *
 * @param[in]  *pCM                         Pointer to global structure AMD_USB4_CM
 * @param[in]  *AMD_USB4_CM_USB3_ADAPTER    Pointer to USB3 Adapter
 * @param[in]  DownstreamBandwidth          Downstream Bandwidth to be allocated
 * @param[in]  UpstreamBandwidth            Upstream Bandwidth to be allocated
 *
 * @retval EFI_SUCCESS                      Success.
 * @retval EFI_DEVICE_ERROR                 HW error.
 * @retval EFI_UNSUPPORTED                  Unsupported router or bandwidth size.
**/
EFI_STATUS
Usb3BandwidthNegotiation (
  IN      AMD_USB4_CM                           *pCM,
  IN      AMD_USB4_CM_USB3_ADAPTER              *Usb3Adapter,
  IN      UINT32                                DownstreamBandwidth,
  IN      UINT32                                UpstreamBandwidth
  )
{
  EFI_STATUS          Status;
  UINT32              DataSize;
  UINT32              *DataPtr;
  UINT8               Scale;
  UINT8               DownstreamScale;
  UINT8               UpstreamScale;
  UINT64              AllocatedDownstream;
  UINT64              AllocatedUpstream;

  Status              = EFI_SUCCESS;
  DataSize            = 0;
  DataPtr             = NULL;
  Scale               = 0;
  DownstreamScale     = 0;
  UpstreamScale       = 0;
  AllocatedDownstream = 0;
  AllocatedUpstream   = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  if (((AMD_USB4_CM_ROUTER*) Usb3Adapter->Header.Parent)->IsHost == 0) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Bandwidth Negotiation is only supported for Host Router.\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Convert from gigabits per second to bytes per millisecond and removing the scaling
  // ((Gb/s * 1GB/8Gb * 1024MB/1GB * 1024KB/1MB * 1024B/1KB * 1/(512 * 2^0)) * 1s/1000ms) * USB4_RAW_BANDWIDTH_SCALING
  // (512 * 2^Scale) - Scale is ADP_USB3_CS_3.Scale
  //
  AllocatedDownstream = ((UINT64) DownstreamBandwidth) << 18;
  AllocatedDownstream = DivU64x64Remainder (
                          AllocatedDownstream,
                          MultU64x64 (((UINT64) 1000), ((UINT64) USB4_RAW_BANDWIDTH_SCALING)),
                          NULL
                          );

  AllocatedUpstream = ((UINT64) UpstreamBandwidth) << 18;
  AllocatedUpstream = DivU64x64Remainder (
                        AllocatedUpstream,
                        MultU64x64 (((UINT64) 1000), ((UINT64) USB4_RAW_BANDWIDTH_SCALING)),
                        NULL
                        );

  //
  // Determine Scale to set granularity of bandwidth
  // ConsumedDownstreamBandwidth and ConsumedupstreamBandwidth is 12 bits (4096)
  // 0x0 if Adapter's MaxSupportedLinkRate == 0
  //
  if (Usb3Adapter->MaxSupportedLinkRate != 0) {
    DownstreamScale = (UINT8)DivU64x64Remainder (
                               AllocatedDownstream,
                               0x1000,
                               NULL
                               );
    UpstreamScale = (UINT8)DivU64x64Remainder (
                             AllocatedUpstream,
                             0x1000,
                             NULL
                             );
    Scale = (DownstreamScale >= UpstreamScale) ? DownstreamScale : UpstreamScale;
    if (Scale >= 0x40) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Scale size is unsupported: %x. Maximum is 0x3F.\n", Scale);
      return EFI_UNSUPPORTED;
    }
    AllocatedDownstream = AllocatedDownstream >> Scale;
    AllocatedUpstream = AllocatedUpstream >> Scale;
  }

  if (AllocatedDownstream >= 0x1000) {
    IDS_HDT_CONSOLE_USB4_ERROR (" AllocatedDownstream size is unsupported: %x. Maximum is 0xFFF.\n", Scale);
    return EFI_UNSUPPORTED;
  }

  if (AllocatedUpstream >= 0x1000) {
    IDS_HDT_CONSOLE_USB4_ERROR (" AllocatedUpstream size is unsupported: %x. Maximum is 0xFFF.\n", Scale);
    return EFI_UNSUPPORTED;
  }

  IDS_HDT_CONSOLE_USB4_INFO (
    " Scale %x\n"
    "INFO: Allocated Downstream Bandwidth: %d bytes per millisecond (0x%x)\n"
    "INFO: Allocated Upstream Bandwidth: %d bytes per millisecond (0x%x)\n",
    Scale,
    AllocatedDownstream,
    AllocatedDownstream,
    AllocatedUpstream,
    AllocatedUpstream
    );

  //
  // Update USB3 Adapter AMD_USB4_ADP_USB3_CS_3.Scale
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                         // CsType
             (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
             (UINT32) Usb3Adapter->USB3Capability + 3,  // ReadAddress
             DataSize,                                  // ReadSize
             (UINT32**)&DataPtr,                        // DataOut
             &DataSize                                  // DataSize
             );

  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Setting Scale - Failed to read USB3 Adapter AMD_USB4_ADP_USB3_CS_3 Register.\n"
      );
    return EFI_DEVICE_ERROR;
  }

  ((AMD_USB4_ADP_USB3_CS_3 *)DataPtr)->Field.Scale = (UINT32) Scale;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) Usb3Adapter->Header.AdapterNum,
             (UINT32) Usb3Adapter->USB3Capability + 3,
             &DataSize,
             DataPtr
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Setting Scale - Failed to write AMD_USB4_ADP_USB3_CS_3 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Read Host Controller Acknowledge
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                         // CsType
             (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
             (UINT32) Usb3Adapter->USB3Capability + 1,  // ReadAddress
             DataSize,                                  // ReadSize
             (UINT32**)&DataPtr,                        // DataOut
             &DataSize                                  // DataSize
             );

  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Initial Host Controller Ack: Failed to read AMD_USB4_ADP_USB3_CS_1 Register.\n"
      );
    return EFI_DEVICE_ERROR;
  }

  //
  // Ensure Host Controller Acknowledgment was cleared
  // If Host Controller Acknowledgment was not cleared, Connection Manager Request was not cleared after read/update
  // Clear Connection Manager Request and ensure Host Controller Acknowledgment was cleared
  //
  if ((((AMD_USB4_ADP_USB3_CS_1 *)DataPtr)->Field.HostControllerAck != 0)) {
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               Usb3Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,                         // CsType
               (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
               (UINT32) Usb3Adapter->USB3Capability + 2,  // ReadAddress
               DataSize,                                  // ReadSize
               (UINT32**)&DataPtr,                        // DataOut
               &DataSize                                  // DataSize
               );

    if ((EFI_ERROR (Status)) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Connection Manager Request - Failed to read AMD_USB4_ADP_USB3_CS_2 Register.\n"
        );
      return EFI_DEVICE_ERROR;
    }

    ((AMD_USB4_ADP_USB3_CS_2 *)DataPtr)->Field.ConnectionManagerRequest = 0;

    Status = AmdUsb4Ring0WriteCS (
               pCM,
               Usb3Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) Usb3Adapter->Header.AdapterNum,
               (UINT32) Usb3Adapter->USB3Capability + 2,
               &DataSize,
               DataPtr
               );
    if (EFI_ERROR (Status) || DataSize != 1) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Connection Manager Request - Failed to write AMD_USB4_ADP_USB3_CS_2 Register.\n"
        );
      return EFI_DEVICE_ERROR;
    }

    //
    // Wait for Host Controller Acknowledgment to be cleared
    //
    do {
      DataSize = 1;
      Status = AmdUsb4Ring0ReadCS (
                 pCM,
                 Usb3Adapter->Header.TopologyID,
                 CS_TARGET_ADAPTER,                         // CsType
                 (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
                 (UINT32) Usb3Adapter->USB3Capability + 1,  // ReadAddress
                 DataSize,                                  // ReadSize
                 (UINT32**)&DataPtr,                        // DataOut
                 &DataSize                                  // DataSize
                 );

      if ((EFI_ERROR (Status)) || (DataSize != 1)) {
        IDS_HDT_CONSOLE_USB4_ERROR (
          " Clear Host Controller Acknowledgment - Failed to read AMD_USB4_ADP_USB3_CS_1 Register.\n"
          );
        return EFI_DEVICE_ERROR;
      }
    } while ((((AMD_USB4_ADP_USB3_CS_1 *)DataPtr)->Field.HostControllerAck != 0));
  }

  //
  // Set Connection Manager Request
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                         // CsType
             (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
             (UINT32) Usb3Adapter->USB3Capability + 2,  // ReadAddress
             DataSize,                                  // ReadSize
             (UINT32**)&DataPtr,                        // DataOut
             &DataSize                                  // DataSize
             );

  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Set Connection Manager Request - Failed to read AMD_USB4_ADP_USB3_CS_2 Register.\n"
      );
    return EFI_DEVICE_ERROR;
  }

  ((AMD_USB4_ADP_USB3_CS_2 *)DataPtr)->Field.ConnectionManagerRequest = 1;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) Usb3Adapter->Header.AdapterNum,
             (UINT32) Usb3Adapter->USB3Capability + 2,
             &DataSize,
             DataPtr
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Set Connection Manager Request - Failed to write AMD_USB4_ADP_USB3_CS_2 Register.\n"
      );
    return EFI_DEVICE_ERROR;
  }

  //
  // Wait for Host Controller Acknowledgment to be set
  //
  do {
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               Usb3Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,                         // CsType
               (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
               (UINT32) Usb3Adapter->USB3Capability + 1,  // ReadAddress
               DataSize,                                  // ReadSize
               (UINT32**)&DataPtr,                        // DataOut
               &DataSize                                  // DataSize
               );

    if ((EFI_ERROR (Status)) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Host Controller Acknowledgment - Failed to read AMD_USB4_ADP_USB3_CS_1 Register.\n"
        );
      return EFI_DEVICE_ERROR;
    }
  } while ((((AMD_USB4_ADP_USB3_CS_1 *)DataPtr)->Field.HostControllerAck != 1));

  //
  // Read Consumed Bandwidth
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                         // CsType
             (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
             (UINT32) Usb3Adapter->USB3Capability + 1,  // ReadAddress
             DataSize,                                  // ReadSize
             (UINT32**)&DataPtr,                        // DataOut
             &DataSize                                  // DataSize
             );

  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Consumed Bandwidth - Failed to read AMD_USB4_ADP_USB3_CS_1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_INFO (
    " Adapter Consumed Downstream Bandwidth %x\n",
    ((AMD_USB4_ADP_USB3_CS_1 *)DataPtr)->Field.ConsumedDownstreamBandwidth
    );
  IDS_HDT_CONSOLE_USB4_INFO (
    " Adapter Consumed Upstream Bandwidth %x\n",
    ((AMD_USB4_ADP_USB3_CS_1 *)DataPtr)->Field.ConsumedUpstreamBandwidth
    );

  //
  // Update USB3 Adapter
  // AMD_USB4_ADP_USB3_CS_2.AllocatedDownstreamBandwidth and AMD_USB4_ADP_USB3_CS_2.AllocatedUpstreamBandwidth
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                         // CsType
             (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
             (UINT32) Usb3Adapter->USB3Capability + 2,  // ReadAddress
             DataSize,                                  // ReadSize
             (UINT32**)&DataPtr,                        // DataOut
             &DataSize                                  // DataSize
             );

  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Allocated Bandwidth - Failed to read AMD_USB4_ADP_USB3_CS_2 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  ((AMD_USB4_ADP_USB3_CS_2 *)DataPtr)->Field.AllocatedDownstreamBandwidth = (UINT32) AllocatedDownstream;
  ((AMD_USB4_ADP_USB3_CS_2 *)DataPtr)->Field.AllocatedUpstreamBandwidth = (UINT32) AllocatedUpstream;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) Usb3Adapter->Header.AdapterNum,
             (UINT32) Usb3Adapter->USB3Capability + 2,
             &DataSize,
             DataPtr
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Allocated Bandwidth - Failed to write AMD_USB4_ADP_USB3_CS_2 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Clear Connection Manager Request
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                         // CsType
             (UINT32) Usb3Adapter->Header.AdapterNum,   // AdapterNum
             (UINT32) Usb3Adapter->USB3Capability + 2,  // ReadAddress
             DataSize,                                  // ReadSize
             (UINT32**)&DataPtr,                        // DataOut
             &DataSize                                  // DataSize
             );

  if ((EFI_ERROR (Status)) || (DataSize != 1)) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Connection Manager Request - Failed to read AMD_USB4_ADP_USB3_CS_2 Register.\n"
      );
    return EFI_DEVICE_ERROR;
  }

  ((AMD_USB4_ADP_USB3_CS_2 *)DataPtr)->Field.ConnectionManagerRequest = 0;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             Usb3Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) Usb3Adapter->Header.AdapterNum,
             (UINT32) Usb3Adapter->USB3Capability + 2,
             &DataSize,
             DataPtr
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      " Connection Manager Request - Failed to write AMD_USB4_ADP_USB3_CS_2 Register.\n"
      );
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
 * Setup bandwidth for USB3 tunneling
 *
 * @param[in]  *pCM                     Pointer to global structure AMD_USB4_CM
 * @param[in]  *pParentRouter           Pointer to Parent Router
 * @param[in]  *pParentUsb3DownAdp      Pointer to Parent Usb3 Downstream Adapter
 * @param[in]  *pChildUsb3UpAdp         Pointer to Child Usb3 Upstream Adapter
 * @param[in]  *pParentDFPLaneAdapter   Pointer to Parent DFP Lane Adapter
 * @param[in]  *pChildUFPLaneAdapter    Pointer to Child UFP Lane Adapter
 *
 * @param[out]  *pParentUsb3DownAdp     Pointer to Parent Usb3 Downstream Adapter
 * @param[out]  *pChildUsb3UpAdp        Pointer to Child Usb3 Upstream Adapter
 * @param[out]  *pParentDFPLaneAdapter  Pointer to Parent DFP Lane Adapter
 * @param[out]  *pChildUFPLaneAdapter   Pointer to Child UFP Lane Adapter
 *
 * @retval EFI_SUCCESS                  Success.
 * @retval EFI_DEVICE_ERROR             HW error.
**/
EFI_STATUS
Usb3Bandwidth (
  IN      AMD_USB4_CM                           *pCM,
  IN      AMD_USB4_CM_ROUTER                    *pHostRouter,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pHostUsb3DownAdp,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pChildUsb3UpAdp,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER              *pHostDFPLaneAdapter,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER              *pChildUFPLaneAdapter
  )
{
  EFI_STATUS                Status;
  UINT32                    AdapterMaxLinkRate;
  UINT32                    AdapterMaxBandwidth;

  Status                  = EFI_SUCCESS;
  AdapterMaxLinkRate      = 0;
  AdapterMaxBandwidth     = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  //
  // Set Link Rate to lowest Maximum Supported Link Rate of the two USB3 adapters
  //
  if (pHostUsb3DownAdp->MaxSupportedLinkRate <= pChildUsb3UpAdp->MaxSupportedLinkRate) {
    AdapterMaxLinkRate = pHostUsb3DownAdp->MaxSupportedLinkRate;
  } else {
    AdapterMaxLinkRate = pChildUsb3UpAdp->MaxSupportedLinkRate;
  }

  //
  // Determine bandwidth with guard band taken into account
  //
  if (AdapterMaxLinkRate == LINK_RATE_20_GBPS) {
    AdapterMaxBandwidth = (USB4_RAW_BANDWIDTH_20GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                          GUARD_BAND_BANDWIDTH_DENOMINATOR;
  } else if (AdapterMaxLinkRate == LINK_RATE_10_GBPS) {
    AdapterMaxBandwidth = (USB4_RAW_BANDWIDTH_10GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                          GUARD_BAND_BANDWIDTH_DENOMINATOR;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR (" Invalid Adapter Link Rate.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Allocated bandwidth will be adjusted to smallest value of:
  //   1. Adapter's Maximum Supported Bandwidth
  //   2. Path's Available Bandwidth with respect to USB3/PCIe weight ratio
  //
  IDS_HDT_CONSOLE_USB4_INFO (
    " Parent DFP Lane Adapter's Available Bandwidth %d. Paired USB3 Adapter pair's common bandwidth %d\n",
    pHostDFPLaneAdapter->AvailableBandwidth,
    AdapterMaxBandwidth
    );
  if (pHostDFPLaneAdapter->AvailableBandwidth >= AdapterMaxBandwidth) {
    if ((pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth + AdapterMaxBandwidth) >= AdapterMaxBandwidth) {
      pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth = AdapterMaxBandwidth;
    } else {
      pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth += AdapterMaxBandwidth;
    }

    if ((pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth + AdapterMaxBandwidth) >= AdapterMaxBandwidth) {
      pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth = AdapterMaxBandwidth;
    } else {
      pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth += AdapterMaxBandwidth;
    }

    pHostDFPLaneAdapter->AvailableBandwidth -= AdapterMaxBandwidth;
  } else {
    if ((pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth + pHostDFPLaneAdapter->AvailableBandwidth) >= AdapterMaxBandwidth) {
      pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth = AdapterMaxBandwidth;
    } else {
      pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth += pHostDFPLaneAdapter->AvailableBandwidth;
    }

    if ((pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth + pHostDFPLaneAdapter->AvailableBandwidth) >= AdapterMaxBandwidth) {
      pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth = AdapterMaxBandwidth;
    } else {
      pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth += pHostDFPLaneAdapter->AvailableBandwidth;
    }

    pHostDFPLaneAdapter->AvailableBandwidth = 0;
  }
  IDS_HDT_CONSOLE_USB4_INFO (
    " Allocating %d for downstream, %d for upstream.\n",
    pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
    pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth
    );

  //
  // If PCIe tunneling support is enabled, adjust the USB3 path bandwidth passed on Priority group weight
  //
  if (pHostRouter->PcieSupport) {
    pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth =
      ((pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth * USB3_TO_LANE_WEIGHT) /
       (USB3_TO_LANE_WEIGHT + PCIE_TO_LANE_WEIGHT));
    pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth =
      ((pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth * USB3_TO_LANE_WEIGHT) /
       (USB3_TO_LANE_WEIGHT + PCIE_TO_LANE_WEIGHT));
    IDS_HDT_CONSOLE_USB4_INFO (
      " PCIe tunneling support is enabled."
      " Adjusting USB3 allocated bandwidth to %d for downstream, %d for upstream.\n",
      pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
      pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth
      );
  }

  IDS_HDT_CONSOLE_USB4_INFO (
    " Parent DFP Lane Adapter's Available Bandwidth %d.\n",
    pHostDFPLaneAdapter->AvailableBandwidth
    );

  //
  // Initialize USB3 Down Adapter's Allocated Downstream Bandwidth and Allocated Upstream Bandwidth
  //
  Status = Usb3BandwidthNegotiation (
             pCM,
             pHostUsb3DownAdp,
             pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
             pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to adjust USB3 allocated bandwidth.\n");
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 * Deallocate bandwidth for USB3 tunneling
 *
 * @param[in]  *pCM                     Pointer to global structure AMD_USB4_CM
 * @param[in]  *pParentUsb3DownAdp      Pointer to Parent Usb3 Downstream Adapter
 * @param[in]  *pParentDFPLaneAdapter   Pointer to Parent DFP Lane Adapter
 *
 * @param[out]  *pParentUsb3DownAdp     Pointer to Parent Usb3 Downstream Adapter
 * @param[out]  *pParentDFPLaneAdapter  Pointer to Parent DFP Lane Adapter
 *
 * @retval EFI_SUCCESS                  Success.
 * @retval EFI_DEVICE_ERROR             HW error.
 * @retval EFI_BAD_BUFFER_SIZE          USB3 Allocated Upstream Bandwidth and Downstream Bandwidth not identical.
**/
EFI_STATUS
Usb3BandwidthDeallocation (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pHostUsb3DownAdp,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER              *pHostDFPLaneAdapter
  )
{
  EFI_STATUS                Status;
  UINT32                    DeallocatedBandwidth;

  Status                  = EFI_SUCCESS;
  DeallocatedBandwidth    = 0;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);

  IDS_HDT_CONSOLE (
    MAIN_FLOW,
    "Current bandwidth values:\n"
    "  Downstream         : %d\n"
    "  Upstream           : %d\n"
    "  Available Bandwidth: %d\n",
    pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
    pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth,
    pHostDFPLaneAdapter->AvailableBandwidth
    );

  if (pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth != pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth) {
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
      "WARNING: Allocated Upstream Bandwidth (%d) and Downstream Bandwidth (%d) should have been identical.\n",
      pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
      pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth
      );
    return EFI_BAD_BUFFER_SIZE;
  }

  //
  // Determine bandwidth to deallocate from Available Bandwidth with guard band taken into account
  //
  if (pHostUsb3DownAdp->MaxSupportedLinkRate == LINK_RATE_20_GBPS) {
    DeallocatedBandwidth = (USB4_RAW_BANDWIDTH_20GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                           GUARD_BAND_BANDWIDTH_DENOMINATOR;
  } else if (pHostUsb3DownAdp->MaxSupportedLinkRate == LINK_RATE_10_GBPS) {
    DeallocatedBandwidth = (USB4_RAW_BANDWIDTH_10GBPS * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                           GUARD_BAND_BANDWIDTH_DENOMINATOR;
  } else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid Adapter Link Rate.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Deallocated bandwidth from total Available Bandwidth, Downstream Bandwidth, and Upstream Bandwidth
  //
  pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth = 0;
  pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth = 0;
  pHostDFPLaneAdapter->AvailableBandwidth += DeallocatedBandwidth;

  IDS_HDT_CONSOLE (
    MAIN_FLOW,
    "Updated bandwidth values:\n"
    "  Downstream         : %d\n"
    "  Upstream           : %d\n"
    "  Available Bandwidth: %d\n",
    pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
    pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth,
    pHostDFPLaneAdapter->AvailableBandwidth
    );

  //
  // Update USB3 Down Adapter's Allocated Downstream Bandwidth and Allocated Upstream Bandwidth
  //
  Status = Usb3BandwidthNegotiation (
             pCM,
             pHostUsb3DownAdp,
             pHostDFPLaneAdapter->DownstreamUsb3PathBandwidth,
             pHostDFPLaneAdapter->UpstreamUsb3PathBandwidth
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Failed to adjust USB3 allocated bandwidth.\n");
    return Status;
  }

  return EFI_SUCCESS;
}


/**
 * Allocate buffers for PCIe tunneling
 *
 * @param[in]  *pParentDFPLaneAdapter      Pointer to Parent Down Facing Port Lane Adapter
 * @param[in]  pParentRouter               Pointer to Parent Router
 * @param[in]  *pChildUFPLaneAdapter       Pointer to Child Up Facing Port Lane Adapter
 * @param[in]  pChildRouter                Pointer to Child Router
 *
 * @param[out]  *pParentDFPLaneAdapter     Pointer to Parent Down Facing Port Lane Adapter
 * @param[out]  *pChildUFPLaneAdapter      Pointer to Child Up Facing Port Lane Adapter
 *
 * @retval EFI_SUCCESS                     Success.
 * @retval EFI_BUFFER_TOO_SMALL            Can not allocate minimum buffers for PCIe Path.
**/
EFI_STATUS
PcieBufferAllocation (
  IN OUT AMD_USB4_CM_LANE_ADAPTER            *pParentDFPLaneAdapter,
  IN     AMD_USB4_CM_ROUTER                  *pParentRouter,
  IN OUT AMD_USB4_CM_LANE_ADAPTER            *pChildUFPLaneAdapter,
  IN     AMD_USB4_CM_ROUTER                  *pChildRouter
  )
{
  EFI_STATUS                Status;
  UINT32                    ParentCredits;
  UINT32                    ParentMinPcieCredits;
  UINT32                    ParentMaxPcieCredits;
  UINT32                    ChildCredits;
  UINT32                    ChildMinPcieCredits;
  UINT32                    ChildMaxPcieCredits;

  Status                = EFI_SUCCESS;
  ParentCredits         = 0;
  ChildCredits          = 0;

  if (IsUSB4Router (pChildRouter)) {
    ChildMinPcieCredits = (UINT32)(MIN_PCIE_PATH_CREDITS);
    ChildMaxPcieCredits = (UINT32)(pChildRouter->BufferAllocationMaxPcie);
  } else {
    ChildMinPcieCredits = (pChildUFPLaneAdapter->LaneAdp_CurrentWidth) ? 32 : 16;
    ChildMaxPcieCredits = ChildMinPcieCredits;
  }

  if (IsUSB4Router (pParentRouter)) {
    ParentMinPcieCredits = (UINT32)(MIN_PCIE_PATH_CREDITS);
    ParentMaxPcieCredits = (UINT32)(pParentRouter->BufferAllocationMaxPcie);
  } else {
    ParentMinPcieCredits = (pParentDFPLaneAdapter->LaneAdp_CurrentWidth) ? 32 : 16;
    ParentMaxPcieCredits = ParentMinPcieCredits;
  }

  //
  // Determine the number of credits Parent/Child Lane Adapters need for PCIe Tunneling and if they can be allocated
  //
  if (pChildUFPLaneAdapter->RemainingBuffers >= ChildMinPcieCredits) {
    ChildCredits = MAX (
                     ChildMinPcieCredits,
                     MIN (ChildMaxPcieCredits, pChildUFPLaneAdapter->RemainingBuffers)
                     );
  }
  IDS_HDT_CONSOLE (
    MAIN_FLOW,
    "Buffer Allocation - Child UFP Ingress Adapter:\n"
    "  Max PCIe Buffer       = 0x%x\n"
    "  Min PCIe Buffer       = 0x%x\n"
    "  Remaining Buffers     = 0x%x\n\n"
    "  Buffers Requested     = 0x%x\n"
    "  PCIe Path Credits     = 0x%x\n",
    ChildMaxPcieCredits,
    ChildMinPcieCredits,
    pChildUFPLaneAdapter->RemainingBuffers,
    ChildCredits,
    pChildUFPLaneAdapter->PcieCredits
    );

  if ((ChildCredits != 0) && (pChildUFPLaneAdapter->RemainingBuffers >= ChildCredits)) {
    pChildUFPLaneAdapter->RemainingBuffers -= ChildCredits;
    pChildUFPLaneAdapter->PcieCredits += ChildCredits;
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
      "Updated:\n"
      "  Remaining Buffers     = 0x%x\n"
      "  PCIe Path Credits     = 0x%x\n",
      pChildUFPLaneAdapter->RemainingBuffers,
      pChildUFPLaneAdapter->PcieCredits
      );
  } else {
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
     "ERROR: Can not allocate minimum buffers for PCIe Path.\n"
      "  Remaining Buffers                    = 0x%x\n"
      "  Minimum PCIe Path Credits Needed     = 0x%x\n",
      pChildUFPLaneAdapter->RemainingBuffers,
      ChildMinPcieCredits
      );
    return EFI_BUFFER_TOO_SMALL;
  }

  if (pParentDFPLaneAdapter->RemainingBuffers >= ParentMinPcieCredits) {
    ParentCredits = MAX (
                      ParentMinPcieCredits,
                      MIN (ParentMaxPcieCredits, pParentDFPLaneAdapter->RemainingBuffers)
                      );
  }
  IDS_HDT_CONSOLE (
    MAIN_FLOW,
    "Buffer Allocation - Parent DFP Ingress Adapter:\n"
    "  Max PCIe Buffer       = 0x%x\n"
    "  Min PCIe Buffer       = 0x%x\n"
    "  Remaining Buffers     = 0x%x\n\n"
    "  Buffers Requested     = 0x%x\n"
    "  PCIe Path Credits     = 0x%x\n",
    ParentMaxPcieCredits,
    ParentMinPcieCredits,
    pParentDFPLaneAdapter->RemainingBuffers,
    ParentCredits,
    pParentDFPLaneAdapter->PcieCredits
    );

  if ((ParentCredits != 0) && (pParentDFPLaneAdapter->RemainingBuffers >= ParentCredits)) {
    pParentDFPLaneAdapter->RemainingBuffers -= ParentCredits;
    pParentDFPLaneAdapter->PcieCredits += ParentCredits;
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
      "Updated:\n"
      "  Remaining Buffers     = 0x%x\n"
      "  PCIe Path Credits     = 0x%x\n",
      pParentDFPLaneAdapter->RemainingBuffers,
      pParentDFPLaneAdapter->PcieCredits
      );
  } else {
    IDS_HDT_CONSOLE (
      MAIN_FLOW,
     "ERROR: Can not allocate minimum buffers for PCIe Path.\n"
      "  Remaining Buffers                    = 0x%x\n"
      "  Additional PCIe Path Credits Needed  = 0x%x\n",
      pParentDFPLaneAdapter->RemainingBuffers,
      ParentMinPcieCredits
      );

    //
    // Free allocated buffers from Child UFP Lane Adapter
    //
    pChildUFPLaneAdapter->RemainingBuffers += ChildCredits;
    pChildUFPLaneAdapter->PcieCredits -= ChildCredits;

    return EFI_BUFFER_TOO_SMALL;
  }

  return EFI_SUCCESS;
}

