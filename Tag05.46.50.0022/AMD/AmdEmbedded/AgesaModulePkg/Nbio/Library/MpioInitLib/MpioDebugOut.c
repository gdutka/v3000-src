/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
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
#include <Library/GnbLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/MpioInitLib.h>
#include "MpioLibLocal.h"

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIODEBUGOUT_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to Platform Topology
 *
 *
 * @param[in]  AskEntry.desc   Pointer to Platform Topology entry engine descriptor
 */
VOID
STATIC
DxioEntryConfigDump (
  IN       FW_ASK_STRUCT    *AskEntry
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "\n");

  IDS_HDT_CONSOLE (GNB_TRACE, "  Engine Type - %a\n",
    (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) ? "PCIe Port" : (
    (AskEntry->desc.ctrlType == ASK_TYPE_SATA) ? "SATA Port" : (
    (AskEntry->desc.ctrlType == ASK_TYPE_USB) ? "USB Port" : "Invalid"))
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "    Start Phy Lane - %d\n    End   Phy Lane - %d\n",
    AskEntry->desc.startLaneIndex,
    (AskEntry->desc.startLaneIndex + AskEntry->desc.numberOfLanes - 1)
  );
  IDS_HDT_CONSOLE (GNB_TRACE, "    reversed - %d\n", AskEntry->desc.reversed);
  IDS_HDT_CONSOLE (GNB_TRACE, "    gpioHandle - %d\n", AskEntry->desc.gpioHandle);
  if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
    IDS_HDT_CONSOLE (GNB_TRACE, "    PortPresent - %d\n    maxLinkSpeedCap - %d\n    targetLinkSpeed = %d\n" ,
      AskEntry->desc.link_attributes.portPresent,
      AskEntry->desc.link_attributes.maxLinkSpeedCap,
      AskEntry->desc.link_attributes.targetLinkSpeed
    );
    IDS_HDT_CONSOLE (GNB_TRACE, "    Port - %d\n", AskEntry->status.port);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump the data being sent to SMU
 *
 *
 * @param[in]  PlatformDescriptor   Pointer to platform descriptor
 * @param[in]  PlatformDescSize     Size of platform descriptor
 * @param[in]  AncillaryData        Pointer to platform descriptor
 * @param[in]  AncillaryDataSize    Size of platform descriptor
 */
VOID
MpioDebugDump (
  IN       MPIO_DATA       *MpioData
  )
{
  FW_ASK_STRUCT       *AskPointer;
  UINT32              AskCount;
  UINT32              Index;

  AskPointer = MpioData->MpioAsk;
  AskCount = MpioData->MpioAskCount;
  IDS_HDT_CONSOLE (GNB_TRACE, "Number of Asks is %d\n", AskCount);
  for (Index = 0;Index < AskCount; Index++) {
    DxioEntryConfigDump (AskPointer);
    AskPointer++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Sending this Ask to MPIO...\n");
  GnbLibDebugDumpBuffer ((VOID*) MpioData->MpioAsk, MpioData->MpioAskCount * sizeof (FW_ASK_STRUCT), 1, 16);
  IDS_HDT_CONSOLE (GNB_TRACE, "\n");
//  if (AncillaryDataSize != 0) {
//    IDS_HDT_CONSOLE (GNB_TRACE, "Sending this Ancillary Data to MPIO...\n");
//    GnbLibDebugDumpBuffer ((VOID*) AncillaryData, AncillaryDataSize << 2, 3, 8);
//    IDS_HDT_CONSOLE (GNB_TRACE, "\n");
//  } else {
//    IDS_HDT_CONSOLE (GNB_TRACE, "No ancillary data to send\n");
//  }
  return;
}




