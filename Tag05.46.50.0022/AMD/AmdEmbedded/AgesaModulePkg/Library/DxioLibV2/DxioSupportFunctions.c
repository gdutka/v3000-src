/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * DXIO Support Functions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
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
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/DxioLibV2.h>
#include <Library/SmnAccessLib.h>
#include "DxioLibLocal.h"

#define FILECODE LIBRARY_DXIOLIBV2_DXIOSUPPORTFUNCTIONS_FILECODE
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

#define SMN_RSMUPCIE0_RSMU_COUNTER_0_PCIE0_ADDRESS             0x90461dcUL
#define SMN_RSMUPCIE0_RSMU_COUNTER_1_PCIE0_ADDRESS             0x90461e0UL

/*----------------------------------------------------------------------------------------*/
/**
 * DXIO MAC Reset Workaround
 *
 *
 *
 * @param[in]  PeiServices     Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 * @param[in]  Pcie            Pointer to the AGESA platfom complex
 * @param[in]  UserConfig      Pointer to the platform BIOS supplied platform configuration
 * @retval     AGESA_STATUS
 */

VOID
DxioSleep (
  IN     GNB_HANDLE     *GnbHandle,
  IN     UINT32         Milliseconds
  )
{

  UINT32 HiValue, LoValue;
  UINT32 CurTime, TarTime;
  UINT32 Unit;

  Unit = 100000; // 100Mhz / 1000(Milliseconds)
  Unit >>= 10;

  SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_RSMUPCIE0_RSMU_COUNTER_0_PCIE0_ADDRESS, &LoValue);
  SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_RSMUPCIE0_RSMU_COUNTER_1_PCIE0_ADDRESS, &HiValue);
  CurTime = ((HiValue & 0x3FF) << 22) + (LoValue >> 10);
  CurTime /= Unit;

  do {
    SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_RSMUPCIE0_RSMU_COUNTER_0_PCIE0_ADDRESS, &LoValue);
    SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_RSMUPCIE0_RSMU_COUNTER_1_PCIE0_ADDRESS, &HiValue);
    TarTime = ((HiValue & 0x3FF) << 22) + (LoValue >> 10);
    TarTime /= Unit;
  } while (Milliseconds > (TarTime - CurTime));

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
DxioGetUserCfgForSocket (
  IN      UINT8                     SocketNumber,
  IN      DXIO_COMPLEX_DESCRIPTOR   *UserComplex,
  IN OUT  DXIO_PORT_DESCRIPTOR      **PortList
  )
{
  AGESA_STATUS               AgesaStatus;
  DXIO_COMPLEX_DESCRIPTOR    *UserCfg;

  AgesaStatus = AGESA_ERROR;
  UserCfg = UserComplex;

  while (UserCfg != NULL) {
    if (UserCfg->SocketId == SocketNumber) {
      *PortList = UserCfg->PciePortList;
      AgesaStatus = AGESA_SUCCESS;
      break;
    }
    UserCfg = PcieConfigGetNextDataDescriptor(UserCfg);
  }
  ASSERT (AgesaStatus == AGESA_SUCCESS);
  return AgesaStatus;
}

