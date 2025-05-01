/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


#ifndef _AMD_USB4_LIB_H_
#define _AMD_USB4_LIB_H_

#include "AmdUsb4.h"
#include "AmdUsb4/ConfigurationSpace.h"


//==========================================================================================
//
// USB4 Console out
//
//==========================================================================================

#define IDS_HDT_CONSOLE_USB4_ASSERT(Expression)                                             \
  do {                                                                                      \
    if (!(Expression)) {                                                                    \
      IDS_HDT_CONSOLE (MAIN_FLOW, "ASSERT %a (%d): %a\n",__FILE__, __LINE__, #Expression);  \
      IDS_DEADLOOP ();                                                                      \
    }                                                                                       \
  } while (FALSE)

#define IDS_HDT_CONSOLE_USB4_ERROR(Expression, ...)                     \
  do {                                                                  \
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: " Expression, ##__VA_ARGS__);   \
  } while (FALSE)

#define IDS_HDT_CONSOLE_USB4_INFO(Expression, ...)                      \
  do {                                                                  \
    IDS_HDT_CONSOLE (MAIN_FLOW, "INFO: " Expression, ##__VA_ARGS__);    \
  } while (FALSE)

#define IDS_HDT_CONSOLE_USB4_PRINT(Expression, ...)                     \
  do {                                                                  \
    IDS_HDT_CONSOLE (MAIN_FLOW, Expression, ##__VA_ARGS__);             \
  } while (FALSE)

#define IDS_HDT_CONSOLE_USB4_WARNING(Expression, ...)                   \
  do {                                                                  \
    IDS_HDT_CONSOLE (MAIN_FLOW, "WARNING: " Expression, ##__VA_ARGS__); \
  } while (FALSE)

#define IDS_HDT_CONSOLE_USB4_DEBUG(Expression, ...)                     \
  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {                 \
    do {                                                                \
      IDS_HDT_CONSOLE (MAIN_FLOW, Expression, ##__VA_ARGS__);           \
    } while (FALSE);                                                    \
  }

#define CONTROL_PACKET_RESPONSE_TIMEOUT 10000   // 6.4.4 Control Packet Reliability
                                                // A Connection Manager can optionally implement a timeout mechanism and may retry a Control
                                                // Packet if no response is received within the timeout interval . If a Connection Manager
                                                // implements a timeout, it is recommended that the timeout is 10 ms + or - 1 ms for Control
                                                // Packets within a Domain and is at least 1 second for Inter-Domain Control Packets.


#define ACPI_MMIO_BASE          0xFED80000ul
#define PMIO_BASE               0x300           // DWORD
#define FCH_PMIOA_REG64         0x64            // AcpiPmTmrBlk
#define NUM_BITS_IN_ACPI_TIMER  32


typedef enum {
  TBT3_TYPE_AR      = 0,
  TBT3_TYPE_TR      = 1,
  TBT3_TYPE_UNKNOWN = 0xFF
} TBT3_ROUTER_TYPE;


#define ROUTER_READY_TIMEOUT_US    100000
#define LINK_BONDING_TIMEOUT       1000
#define UPDATE_LOCAL_TIME_TIMEOUT  10000
#define DOWNSTREAM_PORT_RESET_WAIT 10000             // 10ms wait for Downstream Port Reset
#define LANE_ADP_DFP_RESET_WAIT    3500000

//=============================================================================
//
// Host Interface Operation
//
//==========================================================================================

EFI_STATUS
AmdUsb4HostInterfaceRingSetup (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber,
  IN     UINT32                  TxRingSize,
  IN     UINT32                  RxRingSize,
  IN     UINT32                  TxBufferSize,
  IN     UINT32                  RxBufferSize
  );

EFI_STATUS
AmdUsb4HostInterfaceRingEnable (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber,
  IN     BOOLEAN                 RawMode,
  IN     BOOLEAN                 E2eFlowControlEnable,
  IN     BOOLEAN                 NoSnoopFlag,
  IN     UINT32                  E2eHopID
  );

EFI_STATUS
AmdUsb4HostInterfaceRing0Init (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  TxRingSize,
  IN     UINT32                  RxRingSize,
  IN     UINT32                  TxBufferSize,
  IN     UINT32                  RxBufferSize
  );

//==========================================================================================
//
// Adapter Functions
//
//==========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 * Find next Adapter with specific type
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  StartAdapterNum  Adapter number to start with
 * @param[in]  *pRouter         Point to Router Cm Data structure
 * @param[in]  *pAdapter        Point to Adapter Cm Data structure
 *
 * @param[out]  *pAdapter        Point to Adapter Cm Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_NOT_FOUND          Cannot find Adapter.
**/
EFI_STATUS
FindNextAdapter (
  IN      AMD_USB4_CM             *pCM,
  IN      UINT32                  StartAdapterNum,
  IN      AMD_USB4_CM_ROUTER      *pRouter,
  IN      AMD_USB4_CM_CELL_TYPE   CellType,
  IN  OUT AMD_USB4_CM_CELL        *pAdapter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Find First DFP Lane Adapter
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter         Point to Router CM Data structure
 * @param[in]  *pAdapter        Point to Adapter CM Data structure
 *
 * @param[out]  *pAdapter        Point to DFP Lane Adapter
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_NOT_FOUND          Cannot find Adapter.
**/
EFI_STATUS
FindDFPLaneAdapter (
  IN      AMD_USB4_CM                     *pCM,
  IN      AMD_USB4_CM_ROUTER              *pRouter,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER        *pAdapter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all adapters in router
 *
 * @param[in] *pCM                 Pointer to connection manager structure
 * @param[in] pRouter              Pointer to router configuration space structure
 *
 * @retval EFI_SUCCESS             Adapters enumerated successfully.
 * @retval EFI_DEVICE_ERROR        Error reading configuration space.
 * @retval EFI_OUT_OF_RESOURCES    No resources.
**/
EFI_STATUS
Usb4EnumerateAllAdapters (
  AMD_USB4_CM         *pCM,
  AMD_USB4_CM_ROUTER  *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
* Link Adapters after enumerate all adapters
*
* @param[in] *pCM                 Pointer to Connection Manager structure
* @param[in] *pRouter             Pointer to router configuration space structure
*
* @retval EFI_SUCCESS             Adapters enumerated successfully.
* @retval EFI_INVALID_PARAMETER   Error reading configuration space.
**/
EFI_STATUS
Usb4LinkAdapters (
  IN     AMD_USB4_CM         *pCM,
  IN     AMD_USB4_CM_ROUTER  *pRouter
  );


//==========================================================================================
//
// Router Functions
//
//==========================================================================================

VOID
AnyDeviceRouterConnected (
  IN     AMD_USB4_CM            *pCM,
  IN     AMD_USB4_CM_ROUTER     *pHostRouter
  );

BOOLEAN
IsUSB4Router (
  IN     AMD_USB4_CM_ROUTER *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Host Router Enumeration
 *
 * @param[in] *pCm            Point to global structure AMD_USB4_CM
 * @param[in] *pRouter        Point to Router structure
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
**/
EFI_STATUS
HostRouterEnumeration (
  IN     AMD_USB4_CM          *pCm,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );

EFI_STATUS
RouterOperation2 (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     UINT16                               OpCode,
  IN     UINT32                               OperationMetadata,
  IN     UINT32                               OpSize,
  IN     UINT32                               *OpData,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               *CompletionStatus,
     OUT UINT32                               **Data
  );

EFI_STATUS
RouterOperation (
  IN     AMD_USB4_CM                          *pCm,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     UINT16                               OpCode,
  IN     UINT32                               OperationMetadata,
  IN     UINT32                               *CompletionMetadata,
  OUT    UINT32                               **Data,
  OUT    UINT32                               *CompletionStatus
);

/*----------------------------------------------------------------------------------------*/
/**
 * Host Router Initialization
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pRouter        Point to Router structure
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
**/
EFI_STATUS
HostRouterInitialization (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Device Router detection and Initialization
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pParentRouter  Point to Parent Router
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
DeviceRouterInitialization (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pParentRouter
  );

//==========================================================================================
//
// Port Link Functions
//
//==========================================================================================

EFI_STATUS
AmdUsb4PortOperation (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     UINT32                     Target,
  IN     UINT32                     RetimerIndex,
  IN     UINT32                     OpCode,
  IN     UINT32                     OpSize,
  IN     UINT32                     *OpData,
  IN     UINT32                     *pMetadata,
     OUT UINT32                     *CompletionMetadata,
     OUT UINT32                     **Data
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Downstream Port Reset
 *
 * @param[in] *pCm            Point to global structure AMD_USB4_CM
 * @param[in] *pLaneAdapter   Point to Lane Adapter
 * @param[in] PortCs19        Preset Value of PORT_CS_19
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4DownstreamPortReset (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     AMD_USB4_PORT_CS19         *pPortCs19
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Link Bonding
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4PortLinkBonding (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Link Management including Speed change
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 * @param[in] *LinkSpeedGen3       Requirement of Gen3 speed
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4PortLinkManage (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter,
  IN     BOOLEAN                    LinkSpeedGen3
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Link Configuration before Router Enumeration
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDfpLane0Adapter    Point to Down Facing Port Lane0 Adapter
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
**/
EFI_STATUS
AmdUsb4PortLinkConfigure (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pDfpLane0Adapter
  );

//==========================================================================================
//
// Path Functions
//
//==========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 * EnableUsb3Tunneling
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
**/
VOID
EnableUsb3Tunneling (
  IN  AMD_USB4_CM       *pCM
  );

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
  );

/*----------------------------------------------------------------------------------------*/
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
  );

//==========================================================================================
//
// Usb3 Tunneling Functions
//
//==========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 * Setup Path for Usb3 tunneling
 *
 * @param[in]  *pCM                   Point to global structure AMD_USB4_CM
 * @param[in]  *pParentUsb3DownAdp    Point to Parent Usb3 Downstream Adapter
 * @param[in]  *pChildRouter          Point to Child Router Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/
EFI_STATUS
Usb3PathSetup (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pParentUsb3DownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Path for Usb3 tunneling
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter         Point to Router Cm Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/
EFI_STATUS
Usb3PathEnable (
  IN      AMD_USB4_CM                           *pCm,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate Usb3 Adapter
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  AdapterNum       USB4 controller has N rings. This paramter define Ring Number
 * @param[in]  *pRouter         Point to Router Cm Data structure
 * @param[in]  *pAdapter        Point to Adapter Cm Data structure
 * @param[in]  *pAdapterCs      Point to Adapter Configuration Space data ( 60 DWs )
 *
 * @param[out]  *pRouter         Point to Router Cm Data structure
 * @param[out]  *pAdapter        Point to Adapter Cm Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/
EFI_STATUS
EnumerateUsb3Adapter (
  IN      AMD_USB4_CM                           *pCm,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
  );

//==========================================================================================
//
// Tx/Rx Flows
//
//==========================================================================================

EFI_STATUS
AmdUsb4HostInterfaceTxFlowRawMode (
  IN          AMD_USB4_CM  *pCM,
  IN          UINT32       RingNumber,
  IN          UINT32       Pdf,
  IN          UINT32       DataSize,
  IN          UINT32       *Data,           // Control Packet Payload, do not include header
  IN          BOOLEAN      InterruptEnable
  );

EFI_STATUS
AmdUsb4HostInterfaceRxFlowRawMode (
  IN          AMD_USB4_CM  *pCM,
  IN          UINT32       RingNumber,
  OUT         UINT32       *Pdf,
  OUT         UINT32       *DataSize,
  OUT         UINT32       *Data,           // Control Packet Payload, do not include header
  OUT         UINT32       *RemainPacket
  );

EFI_STATUS
AmdUsb4HostInterfaceRing0Tx (
  IN    AMD_USB4_CM  *pCM,
  IN    UINT32       Pdf,
  IN    UINT32       DataSize,
  IN    UINT32       *Data
  );

EFI_STATUS
AmdUsb4HostInterfaceRing0Rx (
  IN    AMD_USB4_CM   *pCM,
  IN    UINT32        *Pdf,
  IN    UINT32        *DataSize,
  IN    UINT32        *Data,
  IN    UINT32        *RemainPacket
  );


//==========================================================================================
//
// Core.c
//
//==========================================================================================

VOID
AmdUsb4ClearReceiveRing (
  IN     AMD_USB4_CM    *pCM
  );

EFI_STATUS
AmdUsb4FindRouter (
  IN     AMD_USB4_CM      *pCM,
  IN     UINT32           RouteStringHigh,
  IN     UINT32           RouteStringLow,
     OUT AMD_USB4_CM_CELL **PtrCell
  );

EFI_STATUS
AmdUsb4FindAdapterByCellType (
  IN     AMD_USB4_CM            *pCM,
  IN     UINT32                 RouteStringHigh,
  IN     UINT32                 RouteStringLow,
  IN     AMD_USB4_CM_CELL_TYPE  CellType,
     OUT AMD_USB4_CM_CELL       **PtrCell
  );

EFI_STATUS
AmdUsb4FindAdapterByAdpNum (
  IN     AMD_USB4_CM      *pCM,
  IN     UINT32           RouteStringHigh,
  IN     UINT32           RouteStringLow,
  IN     UINT32           AdapterNum,
     OUT AMD_USB4_CM_CELL **PtrCell
  );

VOID
AmdUsb4Ring0PushHotPlugInfoQueue (
  IN     AMD_USB4_CM    *pCm,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         UPG
  );

VOID
AmdUsb4Ring0PushNotificationQueue (
  IN     AMD_USB4_CM    *pCm,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         EventCode
  );

EFI_STATUS
AmdUsb4Ring0HotPlugAck (
  IN     AMD_USB4_CM    *pCm,
  IN     UINT32         RouteStringHigh,
  IN     UINT32         RouteStringLow,
  IN     UINT32         AdapterNum,
  IN     UINT32         UPG
  );

EFI_STATUS
AmdUsb4Ring0ReadCS (
  IN     AMD_USB4_CM    *pCm,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         ReadAddress,
  IN     UINT32         ReadSize,
  OUT    UINT32         **DataOut,
  OUT    UINT32         *DataSize
  );

EFI_STATUS
AmdUsb4Ring0WriteCS (
  IN     AMD_USB4_CM    *pCm,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         RegisterAddress,
  IN     UINT32         *WriteDataSize,
  IN     UINT32         *WriteData
  );

#if AMD_USB4_DEBUG_LANE_HOTPLUG_DISABLE || AMD_USB4_DEBUG_DP_HOTPLUG_DISABLE
VOID
DisableHotplugEvent (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         AdapterNum
  );
#endif

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
  );

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
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Write Configuration Space Register
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
  );


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
  );

//==========================================================================================
//
// Pcie Tunneling Functions
//
//==========================================================================================

/*----------------------------------------------------------------------------------------*/
/**
 * Setup Path for PCIe tunneling
 *
 * @param[in]  *pCm                   Point to global structure AMD_USB4_CM
 * @param[in]  *pParentPcieDownAdp    Point to Parent Pcie Downstream Adapter
 * @param[in]  *pChildRouter          Point to Child Router Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
PciePathSetup (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_PCIE_ADAPTER              *pParentPcieDownAdp,
  IN  OUT AMD_USB4_CM_ROUTER                    *pChildRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate PCIe Adapter
 *
 * @param[in]  *pCm               Point to global structure AMD_USB4_CM
 * @param[in]  AdapterNum         Aapter Number
 * @param[in]  *pRouter           Point to Router Cm Data structure
 * @param[in]  *pAdapter          Point to Adapter Cm Data structure
 * @param[in]  *pAdapterCs        Point to Adapter Configuration Space data
 *
 * @param[out]  *pRouter          Point to Router Cm Data structure
 * @param[out]  *pAdapter         Point to Adapter Cm Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR       HW error.
**/
EFI_STATUS
EnumeratePcieAdapter (
  IN      AMD_USB4_CM                           *pCm,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_PCIE_ADAPTER              *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
  );


//==========================================================================================
//
// Dp Tunneling14 Functions
//
//==========================================================================================



/**
 * Setup Path for DisplayPort tunneling
 *
 * @param[in]  *pCM                 Pointer to global structure AMD_USB4_CM.
 * @param[in]  *pDpIn               Pointer to a DP IN Adapter.
 * @param[in]  *pDpIn               Pointer to a DP OUT Adapter.
 * @param[in]  *pIngressAdapter     Pointer a path Ingress Adapter.
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
  );

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
  );

/**
  Process Dp out Hotplug event.

  @param[in]  *pCM              Pointer to global AMD_USB4_CM structure.
  @param[out]  *pAdapter        Pointer to global AMD_USB4_CM_ADAPTER structure.
**/
VOID
ProcessDpOutHotplug (
  IN     AMD_USB4_CM            *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER *pAdapter
  );

/**
  Go through Hotplug queue.
  Re-read DP OUT adapter register value If there is hot plug-in event.

  @param[in]  *pCM              Pointer to global AMD_USB4_CM structure.
**/
VOID
PreEnableDpTunneling (
  IN     AMD_USB4_CM    *pCM
  );

//==========================================================================================
//
// Tbt3 Functions
//
//==========================================================================================

/**
 * @brief   Enable Tbt3 internal XHCI controller.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    **pRouter
 * @param   UINT32                Router Amount
 *
 * @return  VOID
 */
VOID
Tbt3EnablexHC (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   **pRouter,
  IN     UINT32                               RouterAmount
  );

EFI_STATUS
Tbt3RouterEnumeration (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     AMD_USB4_ROUTER_CONFIGURATION_SPACE  *pRouterCs
  );


/**
 * @brief   Enumerate TBT3 Host Interface Adapter
 *
 * @param   AMD_USB4_CM                           *pCM
 * @param   UINT32                                AdapterNum
 * @param   AMD_USB4_CM_ROUTER                    *pRouter
 * @param   AMD_USB4_CM_HI_ADAPTER                *pAdapter
 * @param   AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
Tbt3EnumerateHIAdapter (
  IN      AMD_USB4_CM                           *pCM,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_HI_ADAPTER                *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
  );

/**
 * @brief   Read Tbt3 DROM
 *
 * @param   AMD_USB4_CM              *pCM
 * @param   AMD_USB4_CM_HI_ADAPTER   *pHIAdapter
 * @param   UINT32                   Offset
 * @param   UINT32                   Size
 * @param   UINT32                   *Data
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
TBT3ReadDrom (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_HI_ADAPTER   *pHIAdapter,
  IN     UINT32                   Offset,
  IN     UINT32                   Size,
     OUT UINT32                   *Data
  );

/**
 * @brief   Enable TBT3 Lane Adapter
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
Tbt3LaneAdapterEnable (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter
  );

/**
 * @brief   Check TBT3 router type.
 *
 * @param   AMD_USB4_CM_ROUTER     *pRouter
 * @return  TBT3_ROUTER_TYPE       TBT3_TYPE_AR           AR type
 *                                 TBT3_TYPE_TR           TR type
 *                                 TBT3_TYPE_UNKNOW       Unknow type
 */
TBT3_ROUTER_TYPE
Tbt3RouterType (
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );

/**
 * @brief   Check if there is router connect to this TBT3 router.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 *
 * @return  BOOLEAN               TRUE
 *                                FALSE
 */
BOOLEAN
Tbt3RouterDetected (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER *pAdapter
  );

/**
 * @brief   Disable TMU by writing 0b to TMU_RTR_CS_3.TSPacketInterval.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
Tbt3RouterTmuDisable (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );

EFI_STATUS
Tbt3QueryDPResource (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  );

EFI_STATUS
Tbt3AllocDPResource (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  );

EFI_STATUS
Tbt3DeallocDPResource (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  );

EFI_STATUS
Tbt3SetDpAdapterHopId (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Tbt3 TMU
 *
 * @param[in]  *pCm        Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter    Point to Current Router
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_DEVICE_ERROR           Failed.
 * @retval EFI_INVALID_PARAMETER      Invalid Parameter
**/
EFI_STATUS
Tbt3RouterTmuEnable (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );


VOID
Tbt3RouterUpdateUDM (
  IN     AMD_USB4_CM_ROUTER   *pRouter,
  IN     UINT32               AdapterNum,
  IN     UINT32               EnableUniDirectionalMode
  );

BOOLEAN
IsTbt3Router (
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );

BOOLEAN
Tbt3LinkControlLaneBondingPossible (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Setup Tbt3
 *
 * @param[in]  *pCm                   Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter    Point to Parent Pcie Downstream Adapter
 *
 * @retval EFI_SUCCESS           Success.
 * @retval EFI_DEVICE_ERROR      Failed.  Router return Error Event code.
 *                                        WriteData[0] will have the Event code.
 *
 * @retval EFI_NOT_READY         Failed.  Something wrong in Transmit or Receive routine.
 * @retval EFI_NO_RESPONSE       Failed   Send out WRITE_REQUEST packet, but do not get paired WRITE_RESPONSE or NOTIFICATION.
 * @retval EFI_INVALID_PARAMETER Failed   Input parameter is not good.
**/
EFI_STATUS
Tbt3RouterProvision (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  );

//==========================================================================================
//
// DROM Functions
//
//==========================================================================================
/*----------------------------------------------------------------------------------------*/
/**
 * Read DROM
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pRouter        Point to Router
 *
 * @retval EFI_SUCCESS              Success
 * @retval EFI_INVALID_PARAMETER    Input parameters is invalid
 * @retval EFI_DEVICE_ERROR         Device error
 * @retval EFI_UNSUPPORTED          Router do not support DROM or it is not a USB4 router
 * @retval EFI_OUT_OF_RESOURCES     Not enough memory hold DROM image
**/
EFI_STATUS
ReadDRom (
  IN  AMD_USB4_CM         *pCM,
  IN  AMD_USB4_CM_ROUTER  *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Look up DROM unused adapter entry with specific adapter number
 *
 * @param[in] *pRouter        Point to Router
 * @param[in] AdapterNumber   Adapter Number
 *
 * @retval TRUE               The adapter is unused
 * @retval FALSE              The adapter is not in DROM unused adapter entries
**/
BOOLEAN
IsDROMUnusedAdapter (
  IN  AMD_USB4_CM_ROUTER  *pRouter,
  IN  UINT32              AdapterNumber
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Look up DROM DP Adapter entry with specific adapter number
 *
 * @param[in] *pRouter          Point to Router
 * @param[in] DpAdapterNumber   DP Adapter Number
 *
 * @retval UINT32               Perferred Lane Adapter number;
                                0     - No preferred lane adapter
                                other - Preferred lane adapter number
**/
UINT32
GetDROMDPPreferLaneAdapter (
  IN  AMD_USB4_CM_ROUTER  *pRouter,
  IN  UINT32              DpAdapterNumber
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Get DROM TMU information
 *
 * @param[in] *pRouter          Point to Router
 *
 * @retval Pointer              Pointer of AMD_USB4_DROM_TMU_ENTRY
                                NULL - There is no DROM TMU entry.
                                Other - Return AMD_USB4_CM_ROUTER->DromTMU
**/
AMD_USB4_DROM_TMU_ENTRY*
GetDROMTMU (
  IN  AMD_USB4_CM_ROUTER  *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Disable Router from sending TMU Packet on the link
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuDisableTimeSync (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Router to send TMU Packet on the link
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuEnableTimeSync (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Set Local time
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to Router
 *
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuSetLocalTime (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Host Router TMU
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pHostRouter         Point to Host Router
 * @param[in] TmuMode              TMU mode
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4HostRouterTmuEnable (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pHostRouter,
  IN     UINT8                      TmuMode
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Device Router TMU
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to Router
 * @param[in] TmuMode              TMU mode
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4DeviceRouterTmuEnable (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter,
  IN     UINT8                      TmuMode
  );

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Configure Device Router TMU
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to Router
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4DeviceRouterTmuInit (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter
  );


//==========================================================================================
//
// Common Functions
//
//==========================================================================================

UINT32
GetACPITimer ();

UINT32
Usb4InternalCalculateCrc32 (
  IN  UINT8   *buffer,
  IN  UINT32  len
  );

EFI_STATUS
EFIAPI
Usb4CalculateCrc (
  IN  VOID*     pBuffer,
  IN  UINTN     DataSize,
  IN  INT8      CrcLength,
  IN  BOOLEAN   BitSwap,
  OUT UINT32    *CrcOut
  );

VOID
Usb4DumpData (
  IN  VOID    *ptr,
  IN  UINT32  iSize
  );

EFI_STATUS
StartTimeout (
  IN USB4_TIMEOUT *Timeout,
  IN UINTN        Time
  );

EFI_STATUS
HasItTimedOut (
  IN USB4_TIMEOUT *Timeout
  );

//==========================================================================================
//
// Bandwidth Functions
//
//==========================================================================================
/*----------------------------------------------------------------------------------------*/
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
  );

/**
 * Deallocate credits for DisplayPort tunneling
 *
 * @param[in]  *pCM                       Pointer to global structure AMD_USB4_CM.
 * @param[in]  pDpIn                      Pointer to DP IN Adapter.
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
  );

/*----------------------------------------------------------------------------------------*/
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
  );

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
  );

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
  );

/*----------------------------------------------------------------------------------------*/
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
  IN     UINT32                               ParentRouterMaxUsb3Buffers,
  IN OUT AMD_USB4_CM_LANE_ADAPTER            *pChildUFPLaneAdapter,
  IN     UINT32                               ChildRouterMaxUsb3Buffers
  );

/*----------------------------------------------------------------------------------------*/
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
  );

/*----------------------------------------------------------------------------------------*/
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
  );

/*----------------------------------------------------------------------------------------*/
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
  );

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
**/
EFI_STATUS
Usb3BandwidthDeallocation (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_USB3_ADAPTER              *pHostUsb3DownAdp,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER              *pHostDFPLaneAdapter
  );

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
  );

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
  );

/**
 * Disable Path for DisplayPort tunneling
 *
 * @param[in]  *pCM                   Pointer to global structure AMD_USB4_CM
 * @param[in]  *pParentUsb3DownAdp    Pointer to DP OUT Adapter
 * @param[in]  *pChildRouter          Pointer to deepest Child Router
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
  );

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
  );

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
  );

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
  );

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
  );

/**
 * @brief   Get specific capability id offset.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 * @param   UINT32                CapabilityID
 *
 * @return  UINT32                Zero means not found.
 */
UINT32
Tbt3GetRouterVSCOffset (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     UINT8                    NextCapability,
  IN     UINT8                    FindCapabilityID,
  IN     UINT8                    FindVSCID
  );

EFI_STATUS
Tbt3EnumerateHIAdapter (
  IN      AMD_USB4_CM                           *pCM,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_HI_ADAPTER                *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
  );

EFI_STATUS
Tbt3DpTmuConfiguration (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpOutAdapter
  );

UINT32
GetCapability (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_CELL         *pCell,
  IN     UINT8                    NextCapability,
  IN     UINT8                    FindCapabilityID
  );

BOOLEAN
IsTbt3DpPerformCMHandshake (
  IN     AMD_USB4_CM_ROUTER       *pRouter
  );

EFI_STATUS
Tbt3DpInitCMHandshake (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER   *pAdapter
  );

EFI_STATUS
Tbt3DpDisableLTTPRSupport (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpOut
  );

/*
EFI_STATUS
Tbt3PortRegionSetting (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter
  );
*/

EFI_STATUS
Tbt3DFPReset (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter
  );

EFI_STATUS
Tbt3FindHostInterfaceAdapter (
  IN     AMD_USB4_CM             *pCM,
  IN     AMD_USB4_CM_ROUTER      *pRouter,
  IN     AMD_USB4_CM_HI_ADAPTER  *pHIAdapter
  );

#endif
