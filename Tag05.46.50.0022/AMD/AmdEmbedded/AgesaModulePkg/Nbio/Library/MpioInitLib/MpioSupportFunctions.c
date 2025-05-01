/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/**
 *  @file MpioSupportFunctions.c
 *  @brief Support functions for MPIO initialization library
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <PiPei.h>
#include <AmdPcieComplex.h>
#include <GnbDxio.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Library/BaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/MpioInitLib.h>
#include "MpioLibLocal.h"

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOSUPPORTFUNCTIONS_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct {
  FW_ASK_STRUCT        *AskEntry;
  PCIe_ENGINE_CONFIG   *Engine;
} ENGINE_LOCATION_CALLBACK;
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 */
VOID
FindEngineCallback (
  IN     PCIe_ENGINE_CONFIG   *Engine,
  IN OUT VOID                 *Buffer,
  IN     PCIe_WRAPPER_CONFIG  *Wrapper
  )
{
  ENGINE_LOCATION_CALLBACK    *EngineStruct;

  EngineStruct = (ENGINE_LOCATION_CALLBACK *) Buffer;
  if (EngineStruct->AskEntry->status.port == Engine->Type.Port.PortId) {
    EngineStruct->Engine = Engine;
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 */
VOID
FindWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      GNB_HANDLE            *GnbHandle
  )
{
  ENGINE_LOCATION_CALLBACK    *EngineStruct;

  EngineStruct = (ENGINE_LOCATION_CALLBACK *) Buffer;
  if ((EngineStruct->AskEntry->desc.startLaneIndex >= Wrapper->StartDxioLane) &&
      (EngineStruct->AskEntry->desc.startLaneIndex <= Wrapper->EndDxioLane)) {
    PcieConfigRunProcForAllEnginesInWrapper (
      DESCRIPTOR_PCIE_ENGINE,
      FindEngineCallback,
      EngineStruct,
      Wrapper
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 */
PCIe_ENGINE_CONFIG *
MpioFindEngineForAsk (
  IN      GNB_HANDLE          *GnbHandle,
  IN      FW_ASK_STRUCT       *AskEntry
  )
{
  ENGINE_LOCATION_CALLBACK  EngineStruct;
  GNB_HANDLE                *Handle;

  Handle = GnbHandle;
  EngineStruct.AskEntry = AskEntry;
  EngineStruct.Engine = NULL;
  while (Handle != NULL) {
    PcieConfigRunProcForAllWrappersInNbio (
      DESCRIPTOR_ALL_WRAPPERS,
      FindWrapperCallback, (VOID *)
      &EngineStruct,
      Handle
      );
    if (EngineStruct.Engine != NULL) {
      break;
    }
    Handle = GnbGetNextHandle (Handle);
  }
  return (EngineStruct.Engine);
}



/*----------------------------------------------------------------------------------------*/

/**
  Shortcut function to call SmuDxioServiceRequest with just function parameters (no array of arguments)


  @param SmuServicesPpi   Instance of PPI
  @param InstanceId       The unique identifier of the SMU instance associated with this socket (for SSP, the socket number)
  @param ServiceRequest   The service request identifie
  @param DxioArg1         Arg 1
  @param DxioArg2         Arg 2
  @param DxioArg3         Arg 3
  @param DxioArg4         Arg 4
  @param DxioArg5         Arg 5

  @return UINT8           Returns DXIO status
**/
UINT8
DxioLibServiceRequest (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN       UINT32                          InstanceId,
  IN       UINT32                          ServiceRequest,
  IN       UINT32                          DxioArg1,
  IN       UINT32                          DxioArg2,
  IN       UINT32                          DxioArg3,
  IN       UINT32                          DxioArg4,
  IN       UINT32                          DxioArg5
  )
{
  UINT32                    SmuArg[6];

  LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
  SmuArg[1] = DxioArg1;
  SmuArg[2] = DxioArg2;
  SmuArg[3] = DxioArg3;
  SmuArg[4] = DxioArg4;
  SmuArg[5] = DxioArg5;

  SmuServicesPpi->SmuDxioServiceRequest (SmuServicesPpi, InstanceId, ServiceRequest, SmuArg, SmuArg);

  if (DXIO_MBOX_RETURN_POSTED_DEFRD == (SmuArg[0] >> 8)) {
    SmuArg[0] = DXIO_MBOX_RETVAL_OK;    // Special case where DXIO sometimes doesn't update Arg0 on GO
  }
  // Check for any errors returned and notify caller
  if (SmuArg[0] != DXIO_MBOX_RETVAL_OK) {
    IDS_HDT_CONSOLE (GNB_TRACE, "%a : Request %x returned status %x which != DXIO_MBOX_RETVAL_OK\n",
                       __FUNCTION__,
                       ServiceRequest,
                       SmuArg[0]
                       );
  }

  return (UINT8)SmuArg[0];
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Function to find the topology structure for a specific socket
 *
 *
 *
 * @param[in]     SocketNumber    Socket number to look for
 * @param[in]     UserComplex     Pointer to the top of the topology complex
 * @param[in]     PortList        Pointer to address of port list (for return)
 */
AGESA_STATUS
MpioGetTopologyForSocket (
  IN      UINT8                     SocketNumber,
  IN      DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  IN OUT  DXIO_PORT_DESCRIPTOR      **PortList
  )
{
  AGESA_STATUS               AgesaStatus;
  DXIO_COMPLEX_DESCRIPTOR    *SocketTopology;

  AgesaStatus = AGESA_ERROR;
  SocketTopology = PlatformTopology;

  while (SocketTopology != NULL) {
    if (SocketTopology->SocketId == SocketNumber) {
      *PortList = SocketTopology->PciePortList;
      AgesaStatus = AGESA_SUCCESS;
      break;
    }
    SocketTopology = PcieConfigGetNextDataDescriptor(SocketTopology);
  }
  ASSERT (AgesaStatus == AGESA_SUCCESS);
  return AgesaStatus;
}

