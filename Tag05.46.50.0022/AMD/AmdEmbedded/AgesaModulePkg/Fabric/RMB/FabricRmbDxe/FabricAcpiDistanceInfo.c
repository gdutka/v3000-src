/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ACPI distance information for SLIT/CDIT.
 *
 * This function collects distance information for SLIT/CDIT.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Porting.h"
#include "AMD.h"
#include <Library/AmdBaseLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Protocol/FabricNumaServices2.h>
#include <Filecode.h>


#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICACPIDISTANCEINFO_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define DISTANCE_TO_SELF  10
#define XGMI_PENALTY      10
#define TCDX_PENALTY       2

#define SLIT_DEGREE_AUTO_LOCAL_DISTANCE   12
#define SLIT_DEGREE_AUTO_REMOTE_DISTANCE  0x20

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetDistanceInfo
 *
 *  Description:
 *    This function publishes the SLIT and CDIT distance information
 *
 *  Parameters:
 *    @param[in, out] Distance                   Current ACPI table pointer to store the distance matrix
 *    @param[in]      DomainCount                Number of unique NUMA domains
 *    @param[in]      DomainInfo                 Information about the NUMA domains
 *
 *    @retval         EFI_SUCCESS                Distance matrix stored
 *    @retval         EFI_ABORTED                A problem was encountered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricGetDistanceInfo (
  IN OUT   UINT8         *Distance,
  IN       UINT32         DomainCount,
  IN       DOMAIN_INFO2  *DomainInfo
  )
{
  UINT8                  OverrideSameSocketDistance;
  UINT8                  OverrideRemoteSocketDistance;
  UINT8                  SLinkLocal;
  UINT8                  SLinkRemote;
  UINT32                 i;
  UINT32                 j;
  BOOLEAN                UseHardware;

  // Distance to report for SLink connected memory attached to the same socket
  SLinkLocal = PcdGet8 (PcdAmdFabricSlitSLinkLocalDistance);

  // Distance to report for SLink connected memory attached to the other socket
  SLinkRemote = PcdGet8 (PcdAmdFabricSlitSLinkRemoteDistance);

  switch (PcdGet8 (PcdAmdFabricSlitDegree)) {
  case 1:
    UseHardware = FALSE;
    OverrideSameSocketDistance = DISTANCE_TO_SELF;
    OverrideRemoteSocketDistance = DISTANCE_TO_SELF;
    break;
  case 2:
    UseHardware = FALSE;
    OverrideSameSocketDistance = PcdGet8 (PcdAmdFabric2ndDegreeSlitDistance);
    OverrideRemoteSocketDistance = OverrideSameSocketDistance;
    ASSERT (OverrideSameSocketDistance >= DISTANCE_TO_SELF);
    break;
  case 3:
    UseHardware = FALSE;
    OverrideSameSocketDistance = PcdGet8 (PcdAmdFabric3rdDegreeSlitLocalDistance);
    OverrideRemoteSocketDistance = PcdGet8 (PcdAmdFabric3rdDegreeSlitRemoteDistance);
    ASSERT (OverrideSameSocketDistance >= DISTANCE_TO_SELF);
    ASSERT (OverrideRemoteSocketDistance >= DISTANCE_TO_SELF);
    break;
  case 0xFF:
    UseHardware = FALSE;
    OverrideSameSocketDistance = SLIT_DEGREE_AUTO_LOCAL_DISTANCE;
    OverrideRemoteSocketDistance = SLIT_DEGREE_AUTO_REMOTE_DISTANCE;
    break;
  default:
    ASSERT (FALSE);
    // Intentional fall through to hardware case after asserting
  case 0:
    UseHardware = TRUE;
    OverrideSameSocketDistance = 0;
    OverrideRemoteSocketDistance = 0;
    break;
  }

  for (i = 0; i < DomainCount; i++) {
    ASSERT (DomainInfo[i].Type < MaxNumaDomainType2);
    for (j = 0; j < DomainCount; j++) {
      if ((i == j) || (DomainInfo[i].PhysicalDomain == DomainInfo[j].PhysicalDomain)) {
        // Case 1: distance to self
        *Distance = DISTANCE_TO_SELF;
      } else {
        ASSERT (DomainInfo[j].Type < MaxNumaDomainType2);
        if ((DomainInfo[i].Type == NumaDram) && (DomainInfo[j].Type == NumaDram)) {
          // The two domains contain cores and possibly DRAM
          if ((DomainInfo[i].SocketMap & DomainInfo[j].SocketMap) != 0) {
            // Case 2: same socket but different physical domains
            *Distance = UseHardware ? (DISTANCE_TO_SELF + TCDX_PENALTY) : OverrideSameSocketDistance;
          } else {
            // Case 3: different sockets
            *Distance = UseHardware ? (DISTANCE_TO_SELF + XGMI_PENALTY) : OverrideRemoteSocketDistance;
          }
        } else if ((DomainInfo[i].Type == NumaSLink) && (DomainInfo[j].Type == NumaSLink)) {
          // Case 4: Neither domain contains cores.  Mark as unreachable
          *Distance = 0xFF;
        } else {
          // Case 5: One domain is SLink and the other is conventional DRAM
          *Distance = ((DomainInfo[i].SocketMap & DomainInfo[j].SocketMap) != 0) ? SLinkLocal : SLinkRemote;
        }
      }
      Distance++;
    }
  }

  return EFI_SUCCESS;
}

