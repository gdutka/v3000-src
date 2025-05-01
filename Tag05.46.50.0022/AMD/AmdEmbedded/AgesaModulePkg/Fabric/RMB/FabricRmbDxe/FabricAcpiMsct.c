/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ACPI MSCT.
 *
 * This function implements routines for MSCT creation.
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

#include "AGESA.h"
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>
#include <Protocol/FabricNumaServices2.h>
#include <Protocol/AmdAcpiSratServicesProtocol.h>
#include <Protocol/AmdAcpiMsctServicesProtocol.h>
#include "FabricAcpiDomainInfo.h"
#include "Filecode.h"

#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICACPIMSCT_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define MAX_PROXIMITY_DOMAINS MAX_REPORTED_DOMAINS

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
EFIAPI
FabricGetMsctInfo (
  IN       AMD_FABRIC_ACPI_MSCT_SERVICES_PROTOCOL  *This,
     OUT   UINT32                                  *NumPopProxDomains,
     OUT   UINT32                                  *MaxNumProxDomains,
     OUT   MSCT_PROX_DOMAIN_INFO                  **MsctInfo
  );

STATIC AMD_FABRIC_ACPI_MSCT_SERVICES_PROTOCOL mFabricAcpiMsctServicesProtocol = {
  0x1,
  FabricGetMsctInfo
};
STATIC MSCT_PROX_DOMAIN_INFO                  mMsctDomains[MAX_PROXIMITY_DOMAINS + 1];
extern BOOLEAN       mIsMixedDimmConfig;


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

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
 *  FabricRmbAcpiMsctProtocolInstall
 *
 *  Description:
 *    This function will install the MSCT services protocol
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
FabricRmbAcpiMsctProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return (gBS->InstallProtocolInterface (
               &ImageHandle,
               &gAmdFabricAcpiMsctServicesProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &mFabricAcpiMsctServicesProtocol
               ));
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetMsctInfo
 *
 *  Description:
 *    This function will return the information necessary to generate MSCT
 *
 *  Parameters:
 *    @param[in]      This                       Pointer to AMD_FABRIC_ACPI_MSCT_SERVICES_PROTOCOL instance
 *    @param[out]     NumPopProxDomains          Number of populated proximity domains
 *    @param[out]     MaxNumProxDomains          Maximum number of proximity domains on the system
 *    @param[out]     MsctInfo                   Structure that describes MSCT proximity domains
 *
 *    @retval         EFI_ABORTED                Services protocol not available
 *                    EFI_SUCCESS                MSCT data successfully gathered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricGetMsctInfo (
  IN       AMD_FABRIC_ACPI_MSCT_SERVICES_PROTOCOL *This,
     OUT   UINT32                                 *NumPopProxDomains,
     OUT   UINT32                                 *MaxNumProxDomains,
     OUT   MSCT_PROX_DOMAIN_INFO                  **MsctInfo
  )
{
  UINT8                                    MaxProxDomains;
  UINTN                                    NumberOfSockets;
  UINTN                                    NumberOfCcds;
  UINTN                                    NumberOfComplexes;
  UINTN                                    NumberOfCores;
  UINTN                                    NumberOfThreads;
  UINT32                                   NumberOfThreadsOnSocket;
  UINT32                                   NumberOfThreadsInSystem;
  UINT32                                   NumberOfThreadsOnCcx;
  UINT32                                   NumaNodesPerSocket;
  UINT32                                   i;
  UINT32                                   j;
  UINT32                                   CurrDomain;
  UINT32                                   NumberOfDomainsInSystem;
  UINT32                                   MemoryInfoCtr;
  DOMAIN_TYPE2                             CurrDomainType;
  DOMAIN_INFO2                            *DomainInfo;
  MEMORY_INFO                             *MemoryInfo;
  FABRIC_NUMA_SERVICES2_PROTOCOL          *FabricNumaServices;
  AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL  *FabricSratServices;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL  *CoreTopology;
  BOOLEAN                                  CcxAsNuma;

  if (gBS->LocateProtocol (&gAmdFabricNumaServices2ProtocolGuid, NULL, (VOID **) &FabricNumaServices) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (gBS->LocateProtocol (&gAmdFabricAcpiSratServicesProtocolGuid, NULL, (VOID **) &FabricSratServices) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, (VOID **) &CoreTopology) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (CoreTopology->GetCoreTopologyOnDie (CoreTopology, 0, 0, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (FabricNumaServices->GetDomainInfo (FabricNumaServices, &NumberOfDomainsInSystem, &DomainInfo, &CcxAsNuma) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  if (FabricNumaServices->GetPhysDomainInfo (FabricNumaServices, NULL, NULL, &NumaNodesPerSocket, NULL) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  MaxProxDomains = FabricGetMaxDomains ();
  if (mIsMixedDimmConfig) {
    ASSERT (MaxProxDomains <= (MAX_PROXIMITY_DOMAINS + 1));
  } else {
    ASSERT (MaxProxDomains <= MAX_PROXIMITY_DOMAINS);
  }

  NumberOfThreadsOnCcx = (UINT32) (NumberOfCores * NumberOfThreads);
  NumberOfThreadsOnSocket = (UINT32) (NumberOfThreadsOnCcx * NumberOfComplexes * NumberOfCcds);
  NumberOfThreadsInSystem = ((UINT32) NumberOfSockets * NumberOfThreadsOnSocket);

  // Initialize mMsctDomains
  for (i = 0; i < MaxProxDomains; i++) {
    mMsctDomains[i].ProxDomain = 0;
    mMsctDomains[i].MaxProcCap = 0;
    mMsctDomains[i].MaxMemCap = 0;
  }

  for (i = 0; i < NumberOfDomainsInSystem ; i++) {
    CurrDomainType = DomainInfo[i].Type;
    ASSERT (CurrDomainType < MaxNumaDomainType2);

    mMsctDomains[i].ProxDomain = i;

    if (CurrDomainType == NumaSLink) {
      // S-Link domains have zero cores
      mMsctDomains[i].MaxProcCap = 0;
    } else if (CcxAsNuma) {
      // Each CCX is a NUMA domain
      mMsctDomains[i].MaxProcCap = NumberOfThreadsOnCcx;
    } else {
      if (NumaNodesPerSocket == 0) {
        mMsctDomains[i].MaxProcCap = NumberOfThreadsInSystem;
      } else {
        mMsctDomains[i].MaxProcCap = NumberOfThreadsOnSocket / NumaNodesPerSocket;
      }
    }
    if (mIsMixedDimmConfig) {
      mMsctDomains[NumberOfDomainsInSystem - 1].MaxProcCap = 0;
    }
  }

  // Obtain memory affinity information for each domain
  if (FabricSratServices->GetMemoryInfo (FabricSratServices, &MemoryInfoCtr, &MemoryInfo) == EFI_SUCCESS) {
    // Update MaxMemCap field
    for (i = 0; i < MemoryInfoCtr; i++) {
      CurrDomain = MemoryInfo[i].Domain;

      for (j = 0; j < NumberOfDomainsInSystem; j++) {
        if (CurrDomain == mMsctDomains[j].ProxDomain) {
          mMsctDomains[j].MaxMemCap += MemoryInfo[i].RegionSize;
          break;
        }
      }
    }
  }

  *MsctInfo = &mMsctDomains[0];
  *NumPopProxDomains = NumberOfDomainsInSystem;
  *MaxNumProxDomains = MaxProxDomains;

  return EFI_SUCCESS;
}

