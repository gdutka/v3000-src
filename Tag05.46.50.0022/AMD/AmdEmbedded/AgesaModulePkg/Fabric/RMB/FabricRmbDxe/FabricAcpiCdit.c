/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ACPI CDIT.
 *
 * This function collects CDIT information.
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
#include "Library/AmdBaseLib.h"
#include <Library/BaseFabricTopologyLib.h>
#include <Library/AmdIdsHookLib.h>
#include "Library/UefiBootServicesTableLib.h"
#include <Protocol/AmdAcpiCditServicesProtocol.h>
#include <Protocol/FabricNumaServices2.h>
#include <FabricAcpiTable.h>
#include "Filecode.h"


#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICACPICDIT_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricGetCditInfo (
  IN       AMD_FABRIC_ACPI_CDIT_SERVICES_PROTOCOL  *This,
  IN OUT   UINT8                                   *DomainCount,
  IN OUT   UINT8                                   *Distance
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

STATIC AMD_FABRIC_ACPI_CDIT_SERVICES_PROTOCOL  mFabricAcpiCditServicesProtocol = {
  0x1,
  FabricGetCditInfo
};

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRmbAcpiCditProtocolInstall
 *
 *  Description:
 *    This function will install the CDIT services protocol
 *
 *  Parameters:
 *    @param[in]      ImageHandle                Image handle
 *    @param[in]      SystemTable                EFI system table
 *
 *    @retval         EFI_SUCCESS                Services protocol installed
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricRmbAcpiCditProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return (gBS->InstallProtocolInterface (
               &ImageHandle,
               &gAmdFabricAcpiCditServicesProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &mFabricAcpiCditServicesProtocol
               ));
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetCditInfo
 *
 *  Description:
 *    This function publishes the CDIT distance information
 *
 *  Parameters:
 *    @param[in]      This                       CDIT services
 *    @param[out]     DomainCount                Number of unique NUMA domains
 *    @param[in, out] Distance                   Current CDIT table pointer to store the distance matrix
 *
 *    @retval         EFI_SUCCESS                Distance matrix stored
 *    @retval         EFI_ABORTED                A problem was encountered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricGetCditInfo (
  IN       AMD_FABRIC_ACPI_CDIT_SERVICES_PROTOCOL  *This,
     OUT   UINT8                                   *DomainCount,
  IN OUT   UINT8                                   *Distance
  )
{
  UINT32                           NumOfDomain;
  DOMAIN_INFO2                    *DomainInfo;
  FABRIC_NUMA_SERVICES2_PROTOCOL  *FabricNuma;

  // Locate Fabric NUMA Services 2 Protocol
  if (gBS->LocateProtocol (&gAmdFabricNumaServices2ProtocolGuid, NULL, (VOID **) &FabricNuma) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  FabricNuma->GetDomainInfo (FabricNuma, &NumOfDomain, &DomainInfo, NULL);
  *DomainCount = (UINT8) NumOfDomain;

  return FabricGetDistanceInfo (Distance, NumOfDomain, DomainInfo);
}

