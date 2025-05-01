/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ACPI Domain information for SRAT, CRAT, SLIT, CDIT, and MSCT.
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
#include "FabricAcpiDomainInfo.h"
#include "FabricAcpiTable.h"
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>
#include <Library/PciLib.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/FabricNumaServices2.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Addendum/Apcb/Inc/SSP/APCB.h>
#include <Addendum/Apcb/Inc/SSP/APOB.h>
#include <Library/AmdPspApobLib.h>
#include <Protocol/AmdAcpiSratServicesV2Protocol.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>
#include "Filecode.h"


#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICACPIDOMAININFO_FILECODE

#define RMB_NUM_CS_CCIX_BLOCKS 0

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define CS_CCIX_SOCKET_MAP (((1 << RMB_NUM_CS_CCIX_BLOCKS) - 1) << RMB_NUM_CS_UMC_BLOCKS)
#define CS_CCIX_SYSTEM_MAP (CS_CCIX_SOCKET_MAP | (CS_CCIX_SOCKET_MAP << NORMALIZED_SOCKET_SHIFT))

#define SOCKET_CS_MAP ((1 << NORMALIZED_SOCKET_SHIFT) - 1)

#define MAX_HOISTED            4  // VGA, TOM, 1TB, Mixed Dimm

#define RMB_MAX_HOST_BRIDGES_PER_SOCKET   (RMB_MAX_DIES_PER_SOCKET * RMB_MAX_HOST_BRIDGES_PER_DIE)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct {
  UINT32  Domain;
  UINT32  BaseLo;
  UINT32  BaseHi;
  UINT32  SizeLo;
  UINT32  SizeHi;
} ACPI_MEM_ENTRY_INFO;

typedef struct {
  UINT32   NormalizedMap;
  UINT32   RawBase;
  UINT32   RawLimit;
  UINT32   RawSize;
} MEM_REGION;

typedef struct {
  UINT32   Base;
  UINT32   Limit;
  BOOLEAN  MemoryLost;
} HOIST_REGION;

typedef struct {
  UINTN                  FabricId;  ///< System Fabric ID
  UINTN                  BusBase;   ///< Bus# for Root-complex
  UINT32                 RrIndex;   ///< Round-Robin Index
  PXM_DOMAIN_INFO        PxmInfo;   ///< Assigned PXM Domain(s)
} ROOT_COMPLEX_ASSIGNMENT;

EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE mAcpiGiasEntryInfo[1]; // iGPU

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
BuildDomainInfo (
  VOID
  );

EFI_STATUS
EFIAPI
FabricRmbGetDomainInfo (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
     OUT   UINT32                          *NumberOfDomainsInSystem,
     OUT   DOMAIN_INFO2                   **DomainInfo,
     OUT   BOOLEAN                         *CcxAsNumaDomain
  );

EFI_STATUS
EFIAPI
FabricRmbDomainXlat (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
  IN       UINTN                            Socket,
  IN       UINTN                            Die,
  IN       UINTN                            Ccd,
  IN       UINTN                            Ccx,
     OUT   UINT32                          *Domain
  );

EFI_STATUS
EFIAPI
FabricRmbGetPhysDomainInfo (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
     OUT   UINT32                          *NumberOfPhysDomainsInSystem,
     OUT   PHYS_DOMAIN_INFO               **PhysDomainInfo,
     OUT   UINT32                          *PhysNodesPerSocket,
     OUT   UINT32                          *NumberOfSystemSLinkDomains
  );

EFI_STATUS
FabricRmbGetPxmDomainInfo (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
  IN       PCI_ADDR                         RootPortBdf,
     OUT   PXM_DOMAIN_INFO                 *PxmDomainInfo
  );

UINT8
FabricGetMaxDomains (
  VOID
  );

VOID
BuildCcmInfo (
  IN       UINT32  SocketCount
  );

UINT32
GetPhysDomainOfCcd (
  IN       UINT32  Socket,
  IN       UINT32  PhysCcd
  );

UINT32
GetSocketMapFromNormalizedCsMap (
  IN       UINT32  NormalizedCsMap
  );

EFI_STATUS
FabricCreateSystemAcpiDomainData (
  IN OUT   UINT8                           **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_MEM_ENTRY    PublishMemEntry,
     OUT   UINT32                           *NumberOfEntriesPublished
  );

UINT32
CollectGiasAcpiDomainData (
  VOID
  );

EFI_STATUS
CollectSystemAcpiDomainData (
  VOID
  );

EFI_STATUS
FabricGetMemoryInfo (
     OUT   UINT32        *NumberOfDomains,
     OUT   MEMORY_INFO  **MemoryInfo
  );

VOID
StoreMemEntry (
     OUT   ACPI_MEM_ENTRY_INFO  *Entry,
  IN       UINT32                Domain,
  IN       UINT32                Base,
  IN       UINT32                Size
  );

UINT32
GetNumberOfSetBits (
  IN       UINT32   BitMap
  );

EFI_STATUS
GetNthBitSet (
  IN       UINT32   BitMap,
  IN       UINT32   N,
     OUT   UINT32  *BitPosition
  );

VOID
InitPxmDomainInfo (
  IN       UINTN             FabricId,
  IN       UINTN             NodesPerSocket,
  IN       UINT8             ActualNps,
  IN OUT   UINT8            *NumOfNodesAssigned,
     OUT   PXM_DOMAIN_INFO  *PxmDomainInfo
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN       mDomainInfoValid = FALSE;

UINT32        mNumberOfReportedDomains = 0;
DOMAIN_INFO2  mReportedDomainInfo[MAX_REPORTED_DOMAINS + 1];
UINT32        mReportedDomainCcxMap[MAX_REPORTED_DOMAINS + 1];
BOOLEAN       mCcxAsNuma = FALSE;

UINT32        mCcmCount[RMB_MAX_SOCKETS];
UINT32        mCcxPerCcm[RMB_MAX_SOCKETS];
UINT32        mLogToPhysCcm[RMB_MAX_SOCKETS][RMB_NUM_CCM_BLOCKS];

UINT32            mNumberOfPhysicalDomains = 0;
PHYS_DOMAIN_INFO  mPhysicalDomainInfo[MAX_PHYSICAL_DOMAINS];
UINT32            mPhysNps;
UINT32            mSystemSLinkCount;

UINT32               mNumberOfAcpiMemEntries = 0;
UINT32               mNumberOfAcpiGiasEntries = 0;
ACPI_MEM_ENTRY_INFO  mAcpiMemEntryInfo[(MAX_REPORTED_DOMAINS + MAX_HOISTED)];

UINT32      mMemoryInfoCtr = 0;
MEMORY_INFO mMemoryInfo[(MAX_REPORTED_DOMAINS + MAX_HOISTED)];

BOOLEAN   mIsPxmDomainInfoInit = FALSE;
BOOLEAN   mRrNumaDomain = FALSE;
UINTN     mSockets;
UINTN     mHostBridges[RMB_MAX_SOCKETS];
BOOLEAN   mIsMixedDimmConfig = FALSE;
BOOLEAN   mIsLowIntlvAbove = FALSE;
UINT32    mDiffIntlvLimitAddr = 0;

ROOT_COMPLEX_ASSIGNMENT mRootComplex[RMB_MAX_SOCKETS][RMB_MAX_HOST_BRIDGES_PER_SOCKET] = { { { 0 } } };

STATIC FABRIC_NUMA_SERVICES2_PROTOCOL  mFabricNumaServices2Protocol = {
  0x3,
  FabricRmbGetDomainInfo,
  FabricRmbDomainXlat,
  FabricRmbGetPhysDomainInfo,
  FabricRmbGetPxmDomainInfo
};

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRmbNumaServices2ProtocolInstall
 *
 *  Description:
 *    This function will install the NUMA services 2 protocol
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
FabricRmbNumaServices2ProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = BuildDomainInfo ();

  if (Status == EFI_SUCCESS) {
    Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gAmdFabricNumaServices2ProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mFabricNumaServices2Protocol
                  );
  }
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRmbGetDomainInfo
 *
 *  Description:
 *    This function returns information about the NUMA domains.
 *
 *  Parameters:
 *    @param[in]      This                       NUMA services
 *    @param[out]     NumberOfDomainsInSystem    Number of unique NUMA domains
 *    @param[out]     DomainInfo                 An array with information about each domain
 *    @param[out]     CcxAsNumaDomain            TRUE: each core complex is its own domain
 *                                               FALSE: physical mapping is employed
 *
 *    @retval         EFI_SUCCESS                Info returned is valid
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricRmbGetDomainInfo (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
     OUT   UINT32                          *NumberOfDomainsInSystem,
     OUT   DOMAIN_INFO2                   **DomainInfo,
     OUT   BOOLEAN                         *CcxAsNumaDomain
  )
{
  ASSERT (mDomainInfoValid);

  if ((NumberOfDomainsInSystem == NULL) && (DomainInfo == NULL) && (CcxAsNumaDomain == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfDomainsInSystem != NULL) {
    *NumberOfDomainsInSystem = mNumberOfReportedDomains;
    if (mIsMixedDimmConfig) {
      (*NumberOfDomainsInSystem) += 1;
    }
  }

  if (DomainInfo != NULL) {
    *DomainInfo = &mReportedDomainInfo[0];
  }

  if (CcxAsNumaDomain != NULL) {
    *CcxAsNumaDomain = mCcxAsNuma;
  }

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRmbDomainXlat
 *
 *  Description:
 *    This function translates a core's physical location to the appropriate NUMA domain.
 *
 *  Parameters:
 *    @param[in]      This                       NUMA services
 *    @param[in]      Socket                     Zero based socket that the core is attached to
 *    @param[in]      Die                        DF die on socket that the core is attached to
 *    @param[in]      Ccd                        Logical CCD the core is on
 *    @param[in]      Ccx                        Logical core complex
 *    @param[out]     Domain                     Domain the core belongs to
 *
 *    @retval         EFI_SUCCESS                Domain is valid
 *    @retval         EFI_INVALID_PARAMETER      No core at location
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricRmbDomainXlat (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
  IN       UINTN                            Socket,
  IN       UINTN                            Die,
  IN       UINTN                            Ccd,
  IN       UINTN                            Ccx,
     OUT   UINT32                          *Domain
  )
{
  UINT32  i;

  ASSERT (mDomainInfoValid);

  if (Socket >= FabricTopologyGetNumberOfProcessorsPresent ()) {
    return EFI_INVALID_PARAMETER;
  }

  if (Die >= FabricTopologyGetNumberOfDiesOnSocket (Socket)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Ccd >= mCcmCount[Socket]) {
    return EFI_INVALID_PARAMETER;
  }

  if (Ccx >= mCcxPerCcm[Socket]) {
    return EFI_INVALID_PARAMETER;
  }

  if (Domain == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (i = 0; i < mNumberOfReportedDomains; i++) {
    if ((mReportedDomainCcxMap[i] & ((1 << (UINT32) Ccx) << (mLogToPhysCcm[Socket][Ccd] * MAX_CCX_PER_CCM) << ((UINT32) Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
      break;
    }
  }

  if (i >= mNumberOfReportedDomains) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *Domain = i;
  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  Returns the maximum number of domains in the system
 *
 *    @return          UINT8   Maximum number of domains in the system
 *
 *---------------------------------------------------------------------------------------
 */
UINT8
FabricGetMaxDomains (
  VOID
  )
{
  UINT8                        MaxDomains;
  ASSERT (mDomainInfoValid);

  MaxDomains = (PcdGet8 (PcdAmdNumberOfPhysicalSocket) * ((UINT8) mSystemSLinkCount + (mCcxAsNuma ? (UINT8) (mCcmCount[0] * mCcxPerCcm[0]) : (UINT8) mPhysNps)));
  if (mIsMixedDimmConfig) {
    MaxDomains += 1;
  }

  return MaxDomains;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRmbGetPhysDomainInfo
 *
 *  Description:
 *    This function returns information about the physical domains requested via NPS
 *
 *  Parameters:
 *    @param[in]      This                          NUMA services
 *    @param[out]     NumberOfPhysDomainsInSystem   Number of valid domains in the system
 *    @param[out]     PhysDomainInfo                An array with information about each physical domain
 *    @param[out]     PhysNodesPerSocket            Actual NPS as determined by ABL (not including SLink)
 *    @param[out]     NumberOfSystemSLinkDomains    Number of domains describing SLink connected memory
 *
 *    @retval         EFI_SUCCESS                Requested info is valid
 *    @retval         EFI_INVALID_PARAMETER      No valid pointers passed
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricRmbGetPhysDomainInfo (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
     OUT   UINT32                          *NumberOfPhysDomainsInSystem,
     OUT   PHYS_DOMAIN_INFO               **PhysDomainInfo,
     OUT   UINT32                          *PhysNodesPerSocket,
     OUT   UINT32                          *NumberOfSystemSLinkDomains
  )
{
  ASSERT (mDomainInfoValid);

  if ((NumberOfPhysDomainsInSystem == NULL) && (PhysDomainInfo == NULL) && (PhysNodesPerSocket == NULL) && (NumberOfSystemSLinkDomains == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfPhysDomainsInSystem != NULL) {
    *NumberOfPhysDomainsInSystem = mNumberOfPhysicalDomains;
    if (mIsMixedDimmConfig) {
      (*NumberOfPhysDomainsInSystem) += 1;
    }
  }

  if (PhysDomainInfo != NULL) {
    *PhysDomainInfo = &mPhysicalDomainInfo[0];
  }

  if (PhysNodesPerSocket != NULL) {
    *PhysNodesPerSocket = mPhysNps;
  }

  if (NumberOfSystemSLinkDomains != NULL) {
    *NumberOfSystemSLinkDomains = mSystemSLinkCount;
  }

  return EFI_SUCCESS;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRmbGetPxmDomainInfo
 *
 *  Description:
 *    This function returns Proximity Domain information about a PCIe root-port bridge
 *
 *    Given a root-bridge as specified by its Bus, Device, Function, this function identifies:
 *    - the NBIO to which the root-bridge belongs,
 *    - the Quadrant to which the NBIO belongs,
 *    - and finally, the NUMA node (or nodes) associated with the Quadrant.
 *
 *    There are two cases to consider:
 *    1) PcdAmdFabricCcxAsNumaDomain is disabled (FALSE):
 *    In this case the Quadrant is associated with a single NUMA node,
 *    and the NUMA node to which the Quadrant belongs is based on the NPS setting.
 *
 *    2) PcdAmdFabricCcxAsNumaDomain is enabled (TRUE):
 *    In this case the Quadrant may be associated with one or up to four NUMA nodes,
 *    depending on the number of CCXes in the Quadrant.
 *
 *    Based on PCD  (PcdAmdFabricRoundRobinNumaDomainForCcx), this function
 *    implements a round-robin scheme to return a single NUMA node from the list of nodes
 *    associated with a given Quadrant. Otherwise, this function returns the complete list of
 *    nodes associated with a given Quadrant, allowing for platform-defined implementations.
 *
 *  Parameters:
 *    @param[in]   This             NUMA services
 *    @param[in]   RootPortBdf      BDF for root-port bridge in PCI_ADDR format.
 *    @param[out]  PxmDomainInfo    Pointer to a structure returning associated NUMA node(s).
 *
 *    @retval EFI_SUCCESS     The function completed successfully.
 *    @retval Other           The requested operation could not be completed.
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
FabricRmbGetPxmDomainInfo (
  IN       FABRIC_NUMA_SERVICES2_PROTOCOL  *This,
  IN       PCI_ADDR                         RootPortBdf,
     OUT   PXM_DOMAIN_INFO                 *PxmDomainInfo
  )
{
  EFI_STATUS                                Status;
  APOB_SYSTEM_NPS_INFO_TYPE_STRUCT         *NpsInfo;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL   *CoreTopology;
  UINTN                                     NumOfCcdsOnDie;
  UINTN                                     NumOfCcxsOnCcd;
  UINTN                                     NodesPerSocket;
  UINT8                                     NumOfNodesAssigned;
  UINT8                                     ActualNps;
  UINT32                                    i;
  UINT32                                    j;
  UINT32                                    k;

  // init variables
  Status             = EFI_SUCCESS;
  NpsInfo            = NULL;
  CoreTopology       = NULL;
  NumOfCcdsOnDie     = 0;
  NumOfCcxsOnCcd     = 0;
  NodesPerSocket     = 0;
  NumOfNodesAssigned = 0;
  ActualNps          = 0;

  // Need to intialize PXM array
  if (!mIsPxmDomainInfoInit) {
    // Get NPS setting
    Status = AmdPspGetApobEntryInstance(APOB_FABRIC, APOB_SYS_NPS_INFO_TYPE, 0, FALSE, (APOB_TYPE_HEADER **)&NpsInfo);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    ActualNps = NpsInfo->ActualNps;

    // Get CCXAsNUMA setting
    mCcxAsNuma = PcdGetBool(PcdAmdFabricCcxAsNumaDomain);

    if (mCcxAsNuma) {
      // Get Round Robin setting
      mRrNumaDomain = PcdGetBool(PcdAmdFabricRoundRobinNumaDomainForCcx);

      // Get core topology services protocol
      Status = gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopology);
      ASSERT (!EFI_ERROR (Status));
    }

    // Get number of sockets
    mSockets = FabricTopologyGetNumberOfProcessorsPresent ();

    for (i = 0; i < mSockets; i++) {
      // init number of nodes assigned for this socket
      NumOfNodesAssigned = 0;

      if (mCcxAsNuma) {
        // Get number of CCD per die & CCX per CCD
        Status = CoreTopology->GetCoreTopologyOnDie (CoreTopology, i, 0, &NumOfCcdsOnDie, &NumOfCcxsOnCcd, NULL, NULL);
        ASSERT (!EFI_ERROR (Status));
        if (EFI_ERROR(Status)) {
          return Status;
        }

        // NPS = CCX per socket
        // note: RMB only has one die per socket
        NodesPerSocket = NumOfCcdsOnDie * NumOfCcxsOnCcd;
      }

      // Get number of host (root-complex) bridges per socket
      mHostBridges[i] = FabricTopologyGetNumberOfRootBridgesOnSocket (i);

      // Set PXM info for each root-complex
      for (j = 0; j < mHostBridges[i]; j++) {
        mRootComplex[i][j].FabricId = FabricTopologyGetHostBridgeSystemFabricID (i, 0, j);
        mRootComplex[i][j].BusBase = FabricTopologyGetHostBridgeBusBase (i, 0, j);
        InitPxmDomainInfo (mRootComplex[i][j].FabricId, NodesPerSocket, ActualNps, &NumOfNodesAssigned, &mRootComplex[i][j].PxmInfo);
      }
    }
    mIsPxmDomainInfoInit = TRUE;
  }

  // Look for a matching root-bus in the PXM array
  for (i = 0; i < mSockets; i++) {
    for (j = 0; j < mHostBridges[i]; j++) {
      if (RootPortBdf.Address.Bus == mRootComplex[i][j].BusBase) {
        if (mRrNumaDomain && mRootComplex[i][j].PxmInfo.Count > 1) {
          // return a single domain with round-robin scheme
          PxmDomainInfo->Count = 1;
          PxmDomainInfo->Domain[0] = mRootComplex[i][j].PxmInfo.Domain[mRootComplex[i][j].RrIndex];
          mRootComplex[i][j].RrIndex += 1;
          if (mRootComplex[i][j].RrIndex == mRootComplex[i][j].PxmInfo.Count) {
            mRootComplex[i][j].RrIndex = 0;
          }
        } else {
          // return all pertinent domains (mutiple domains only for CCX as NUMA)
          PxmDomainInfo->Count = mRootComplex[i][j].PxmInfo.Count;
          ASSERT (PxmDomainInfo->Count < MAX_PXM_VALUES_PER_QUADRANT);
          for (k = 0; k < PxmDomainInfo->Count; k++) {
            PxmDomainInfo->Domain[k] = mRootComplex[i][j].PxmInfo.Domain[k];
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  BuildDomainInfo
 *
 *  Description:
 *    This function gathers data about the NUMA domains to be used by the protocol procedures
 *
 *    @retval         EFI_SUCCESS                Successful
 *    @retval         EFI_DEVICE_ERROR           An error was encountered.  Data is not valid
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
BuildDomainInfo (
  VOID
  )
{
  UINTN                              Ccd;
  UINTN                              Ccx;
  UINTN                              Domain;
  UINTN                              Socket;
  UINTN                              NumberOfCpus;
  UINT32                             i;
  UINT32                             LSbitSet;
  UINT32                             ReportedIndex;
  UINT32                             SetBitCount;
  UINT32                             SLinkBase;
  UINT32                             SocketShift;

  NumberOfCpus = FabricTopologyGetNumberOfProcessorsPresent ();

  mCcxAsNuma = PcdGetBool (PcdAmdFabricCcxAsNumaDomain);
  InitMixedDimmConfig ();

  // Fill module global CCM data
  BuildCcmInfo ((UINT32) NumberOfCpus);

  // Fill module global info about the 'physical' domains
  mNumberOfPhysicalDomains = 0;
  for (Socket = 0; Socket < NumberOfCpus; Socket++) {
    mPhysicalDomainInfo[mNumberOfPhysicalDomains++].NormalizedCsMap = (((1 << RMB_NUM_CS_UMC_BLOCKS) - 1) << ((UINT32) Socket * NORMALIZED_SOCKET_SHIFT));
  }
  mPhysNps = 1;

  mSystemSLinkCount = 0;

  if (mCcxAsNuma) {
    mNumberOfReportedDomains = mSystemSLinkCount;
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      mNumberOfReportedDomains += (mCcmCount[Socket] * mCcxPerCcm[Socket]);
    }
  } else {
    mNumberOfReportedDomains = mNumberOfPhysicalDomains;
  }

  // Build reported domain info
  LibAmdMemFill ((VOID *) mReportedDomainCcxMap, 0x00, sizeof (mReportedDomainCcxMap), NULL);

  if (mCcxAsNuma) {
    for (Domain = 0; Domain < MAX_PHYSICAL_DOMAINS; Domain++) {
      mPhysicalDomainInfo[Domain].SharingEntityCount = 0;
      mPhysicalDomainInfo[Domain].SharingEntityMap = 0;
    }
    // Start with non S-Link
    ReportedIndex = 0;
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      for (Ccd = 0; Ccd < mCcmCount[Socket]; Ccd++) {
        for (Ccx = 0; Ccx < mCcxPerCcm[Socket]; Ccx++) {
          mReportedDomainInfo[ReportedIndex].Type = NumaDram;
          mReportedDomainInfo[ReportedIndex].PhysicalDomain = GetPhysDomainOfCcd ((UINT32) Socket, mLogToPhysCcm[Socket][Ccd]);
          mReportedDomainInfo[ReportedIndex].SocketMap = GetSocketMapFromNormalizedCsMap (mPhysicalDomainInfo[mReportedDomainInfo[ReportedIndex].PhysicalDomain].NormalizedCsMap);
          mPhysicalDomainInfo[mReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityCount++;
          mPhysicalDomainInfo[mReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityMap |= (1 << ReportedIndex);
          mReportedDomainCcxMap[ReportedIndex] = (1 << (UINT32) Ccx) << (mLogToPhysCcm[Socket][Ccd] * MAX_CCX_PER_CCM) << ((UINT32) Socket * NORMALIZED_SOCKET_SHIFT);
          ReportedIndex++;
        }
      }
    }

    SLinkBase = mNumberOfPhysicalDomains - mSystemSLinkCount;
    for (i = 0; i < mSystemSLinkCount; i++) {
      mReportedDomainInfo[ReportedIndex].Type = NumaSLink;
      mReportedDomainInfo[ReportedIndex].SocketMap = GetSocketMapFromNormalizedCsMap (mPhysicalDomainInfo[i + SLinkBase].NormalizedCsMap);
      mReportedDomainInfo[ReportedIndex].PhysicalDomain = i + SLinkBase;
      mPhysicalDomainInfo[mReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityCount = 1;
      mPhysicalDomainInfo[mReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityMap = (1 << ReportedIndex);
      ReportedIndex++;
    }
    ASSERT (ReportedIndex == mNumberOfReportedDomains);
  } else {
    for (Domain = 0; Domain < mNumberOfPhysicalDomains; Domain++) {
      mReportedDomainInfo[Domain].Type = ((mPhysicalDomainInfo[Domain].NormalizedCsMap & CS_CCIX_SYSTEM_MAP) != 0) ? NumaSLink : NumaDram;
      mReportedDomainInfo[Domain].SocketMap = GetSocketMapFromNormalizedCsMap (mPhysicalDomainInfo[Domain].NormalizedCsMap);
      mReportedDomainInfo[Domain].PhysicalDomain = (UINT32) Domain;
      mPhysicalDomainInfo[Domain].SharingEntityCount = 1;
      mPhysicalDomainInfo[Domain].SharingEntityMap = (1 << (UINT32) Domain);
      if (mReportedDomainInfo[Domain].Type == NumaDram) {
        SetBitCount = GetNumberOfSetBits (mPhysicalDomainInfo[Domain].NormalizedCsMap);
        if (SetBitCount == 16) {
          //  NPS 0
          //  CS:     FF00FF
          //  CCX:  FFFFFFFF
          mReportedDomainCcxMap[Domain] = 0xFFFFFFFF;
        } else {
          //  NPS 1
          //  CS:     FF,   FF0000
          //  CCX:  FFFF, FFFF0000
          //  NPS 2
          //  CS:    F,   F0,  F0000,   F00000
          //  CCX:  FF, FF00, FF0000, FF000000
          //  NPS 4
          //  CS:   3,  C,  30,   C0, 30000,  c0000,  300000,   C00000
          //  CCX:  F, F0, F00, F000, F0000, F00000, F000000, F0000000
          LSbitSet = (SetBitCount != 0) ? (UINT32) LowBitSet32 (mPhysicalDomainInfo[Domain].NormalizedCsMap) : 0;
          if (LSbitSet >= 16) {
            SocketShift = 16;
            LSbitSet -= 16;
          } else {
            SocketShift = 0;
          }
          mReportedDomainCcxMap[Domain] = ((1 << (SetBitCount << 1)) - 1) << (LSbitSet << 1) << SocketShift;
        }
      }
    }
  }

  mDomainInfoValid = TRUE;
  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  BuildCcmInfo
 *
 *  Description:
 *    This function gathers data about CCMs to be used by the protocol procedures
 *
 *  Parameters:
 *    @param[in]      SocketCount                Number of processors installed
 *
 *---------------------------------------------------------------------------------------
 */
VOID
BuildCcmInfo (
  IN       UINT32  SocketCount
  )
{
  UINTN                                     i;
  UINT32                                    j;
  UINT32                                    CcmMap;
  PCSENABLE_REGISTER                        PcsEnable;
  CCX_ENABLE_REGISTER                       CcxEnable;

  for (i = 0; i < SocketCount; i++) {
    if ((SocketCount == 1) && (FabricTopologyGetNumberOfDiesOnSocket (0) == 1) && mIsMixedDimmConfig) {
      // Client didn't have CCXEnable and PcsEnable registers
      CcmMap = 1;
      mCcmCount[0]  = 0;
      mCcxPerCcm[0] = 1;
    } else {
      // D18F3x12C [CCX Enable Register] (DF::CCXEnable)
      CcxEnable.Value = FabricRegisterAccRead (i, 0, CCXENABLE_FUNC, CCXENABLE_REG, RMB_CCM0_INSTANCE_ID);
      mCcxPerCcm[i] = (CcxEnable.Field.CCXEn == 3) ? 2 : 1;
      // D18F1x330 [PCS Enable] (DF::PcsEnable)
      PcsEnable.Value = FabricRegisterAccRead (i, 0, PCSENABLE_FUNC, PCSENABLE_REG, FABRIC_REG_ACC_BC);
      mCcmCount[i] = 0;
      CcmMap = PcsEnable.Field.CcmPcsEnable;
    }
    for (j = 0; CcmMap != 0; CcmMap &= ~(1 << j), j++) {
      if ((CcmMap & (1 << j)) != 0) {
        ASSERT (j < RMB_NUM_CCM_BLOCKS);
        mLogToPhysCcm[i][mCcmCount[i]++] = j;
      }
    }
    ASSERT (mCcmCount[i] <= RMB_NUM_CCM_BLOCKS);
  }
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GetPhysDomainOfCcd
 *
 *  Description:
 *    This function returns the physical domain that the given CCD belongs to
 *
 *  Parameters:
 *    @param[in]      Socket                   Zero based socket number
 *    @param[in]      PhysCcd                  Physical CCD number
 *
 *    @return         Physical domain number
 *
 *---------------------------------------------------------------------------------------
 */
UINT32
GetPhysDomainOfCcd (
  IN       UINT32  Socket,
  IN       UINT32  PhysCcd
  )
{
  UINT32  i;

  for (i = 0; i < mNumberOfPhysicalDomains; i++) {
    if ((mPhysicalDomainInfo[i].NormalizedCsMap & ((1 << PhysCcd) << (Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
      break;
    }
  }
  ASSERT (i < mNumberOfPhysicalDomains);
  return i;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GetSocketMapFromNormalizedCsMap
 *
 *  Description:
 *    This function returns a bitmap of sockets belonging to the CS map
 *
 *  Parameters:
 *    @param[in]      NormalizedCsMap          CS map to check
 *
 *    @return         Socket bit map
 *
 *---------------------------------------------------------------------------------------
 */
UINT32
GetSocketMapFromNormalizedCsMap (
  IN       UINT32  NormalizedCsMap
  )
{
  UINTN   i;
  UINT32  SocketMap;

  SocketMap = 0;
  for (i = 0; i < RMB_MAX_SOCKETS; i++) {
    SocketMap |= ((NormalizedCsMap & (SOCKET_CS_MAP << ((UINT32) i * NORMALIZED_SOCKET_SHIFT))) != 0) ? (1 << (UINT32) i) : 0;
  }
  return SocketMap;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateGiasAcpiDomainData
 *
 *  Description:
 *    This function creates the SRAT GIAS structures
 *
 *  Parameters:
 *    @param[in, out] TableEnd                   Current ACPI table pointer
 *    @param[in]      PublishGiasEntry           Function to publish the data correctly for the desired table
 *    @param[out]     NumberOfEntriesPublished   Number of records created
 *
 *    @retval         EFI_SUCCESS                Table was updated successfully
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
FabricCreateGiasAcpiDomainData (
  IN OUT   UINT8                             **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_GIAS_ENTRY     PublishGiasEntry,
     OUT   UINT32                             *NumberOfEntriesPublished
  )
{
  UINTN                             i;

  mNumberOfAcpiGiasEntries = CollectGiasAcpiDomainData ();

  for (i = 0; i < mNumberOfAcpiGiasEntries; i++) {
    PublishGiasEntry (
      TableEnd,
      mAcpiGiasEntryInfo[i].ProximityDomain,
      mAcpiGiasEntryInfo[i].DeviceHandleType,
      mAcpiGiasEntryInfo[i].DeviceHandle.Pci.PciSegment,
      mAcpiGiasEntryInfo[i].DeviceHandle.Pci.PciBdfNumber
      );
  }

  if (NumberOfEntriesPublished != NULL) {
    *NumberOfEntriesPublished = mNumberOfAcpiGiasEntries;
  }

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateGpuMemAcpiDomainData
 *
 *  Description:
 *    This function creates the GPU  memory structures
 *
 *  Parameters:
 *    @param[in, out] TableEnd                   Current ACPI table pointer
 *    @param[in]      PublishMemEntry            Function to publish the data correctly for the desired table
 *    @param[out]     NumberOfEntriesPublished   Number of records created
 *
 *    @retval         EFI_SUCCESS                Table was updated successfully
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
FabricCreateGpuMemAcpiDomainData (
  IN OUT   UINT8                             **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_GPUMEM_ENTRY   PublishGpuMemEntry,
     OUT   UINT32                             *NumberOfEntriesPublished
  )
{
  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateSystemAcpiDomainData
 *
 *  Description:
 *    This function creates the SRAT and CRAT memory structures
 *
 *  Parameters:
 *    @param[in, out] TableEnd                   Current ACPI table pointer
 *    @param[in]      PublishMemEntry            Function to publish the data correctly for the desired table
 *    @param[out]     NumberOfEntriesPublished   Number of records created
 *
 *    @retval         EFI_SUCCESS                Table was updated successfully
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
FabricCreateSystemAcpiDomainData (
  IN OUT   UINT8                             **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_MEM_ENTRY      PublishMemEntry,
     OUT   UINT32                             *NumberOfEntriesPublished
  )
{
  UINTN       i;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  if (mNumberOfAcpiMemEntries == 0) {
    Status = CollectSystemAcpiDomainData ();
  }

  for (i = 0; i < mNumberOfAcpiMemEntries; i++) {
    PublishMemEntry (TableEnd,
                     mAcpiMemEntryInfo[i].Domain,
                     mAcpiMemEntryInfo[i].BaseLo,
                     mAcpiMemEntryInfo[i].BaseHi,
                     mAcpiMemEntryInfo[i].SizeLo,
                     mAcpiMemEntryInfo[i].SizeHi);
  }

  if (NumberOfEntriesPublished != NULL) {
    *NumberOfEntriesPublished = mNumberOfAcpiMemEntries;
  }

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CollectGiasAcpiDomainData
 *
 *  Description:
 *    This function gathers the GPU GIAS data to publish to the ACPI tables
 *
 *    @retval         Number Of Acpi GIAS Entries
 *
 *---------------------------------------------------------------------------------------
 */
UINT32
CollectGiasAcpiDomainData (
  VOID
  )
{
  UINT8       IgpuBusNumber;
  UINT16      IgpuDid;
  UINT16      Device = 0;
  UINT16      Function = 0;

  IgpuBusNumber = PciRead8 (PCI_LIB_ADDRESS (0, 8, 1, 0x19));
  IgpuDid = PciRead16 (PCI_LIB_ADDRESS (IgpuBusNumber, Device, Function, 2));
  if ((IgpuBusNumber == 0) || (PciRead16 (PCI_LIB_ADDRESS (IgpuBusNumber, Device, Function, 0)) != 0x1002) || \
      ((IgpuDid != 0x164D) && (IgpuDid != 0x1681))) { // check IGD VID/DID
    return 0;
  }

  mAcpiGiasEntryInfo[0].ProximityDomain = mNumberOfPhysicalDomains - 1;
  mAcpiGiasEntryInfo[0].DeviceHandleType = 1; // PCI device type
  mAcpiGiasEntryInfo[0].DeviceHandle.Pci.PciSegment = 0x0; // Shoreline sets segment as 0
  mAcpiGiasEntryInfo[0].DeviceHandle.Pci.PciBdfNumber = ((UINT16)IgpuBusNumber << 8) | (Device << 3) | (Function); // Construct BDF for the IGD

  return 1;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  CollectSystemAcpiDomainData
 *
 *  Description:
 *    This function gathers the data to publish to the ACPI tables
 *
 *    @retval         EFI_SUCCESS                Data collected successfully
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
CollectSystemAcpiDomainData (
  VOID
  )
{
  UINT32                       ActualSize;
  UINT32                       DramMapIndex;
  UINT32                       Entity;
  UINT32                       EntitySize;
  UINT32                       i;
  UINT32                       MemoryBase;
  UINT32                       NumberOfHoisted;
  UINT32                       PhysDomain;
  UINT32                       PreviousLimit;
  UINT32                       RegionBase;
  UINT32                       RegionSize;
  UINT32                       RegionTop;
  UINT32                       ReportedDomain;
  UINT32                       SizeRemaining;
  UINT32                       Domain;
  VGA_EN_REGISTER              VgaEn;
  DRAM_BASE_ADDRESS_REGISTER   DramBaseAddr;
  DRAM_HOLE_CONTROL_REGISTER   DramHoleCtrl;
  DRAM_LIMIT_ADDRESS_REGISTER  DramLimitAddr;
  MEM_REGION                   MemRegion[RMB_NUMBER_OF_DRAM_REGIONS];
  HOIST_REGION                 HoistRegion[MAX_HOISTED];
  BOOLEAN                      OneTBHoisted;
  UINT8                        ChnlIntlv[16] = {1, 2, 0, 4, 0, 8, 0, 16, 32, 0, 0, 0, 2, 4, 8, 0};

  // Initialize memory info structures
  mNumberOfAcpiMemEntries = 0;
  mMemoryInfoCtr = 0;
  NumberOfHoisted = 0;
  PreviousLimit = 0;
  ReportedDomain = 0;
  OneTBHoisted = FALSE;
  LibAmdMemFill ((VOID *) mMemoryInfo, 0x00, sizeof (mMemoryInfo), NULL);
  LibAmdMemFill ((VOID *) MemRegion, 0x00, sizeof (MemRegion), NULL);

  // Check for the VGA hole
  VgaEn.Value = FabricRegisterAccRead (0, 0, VGA_EN_FUNC, VGA_EN_REG, FABRIC_REG_ACC_BC);
  if (VgaEn.Field.VgaEn_VE == 1) {
    HoistRegion[NumberOfHoisted].MemoryLost = TRUE;
    HoistRegion[NumberOfHoisted].Base = 0xA;
    HoistRegion[NumberOfHoisted++].Limit = 0xC;
  }

  // Check for the MMIO hole below 4GB
  DramHoleCtrl.Value = FabricRegisterAccRead (0, 0, DRAMHOLECTRL_FUNC, DRAMHOLECTRL_REG, FABRIC_REG_ACC_BC);
  if (DramHoleCtrl.Field.DramHoleValid == 1) {
    HoistRegion[NumberOfHoisted].MemoryLost = FALSE;
    HoistRegion[NumberOfHoisted].Base = (DramHoleCtrl.Field.DramHoleBase << 8);
    HoistRegion[NumberOfHoisted++].Limit = 0x10000;
  }

  // Separate the different way interleaving memory regions
  if (mIsMixedDimmConfig && (mDiffIntlvLimitAddr != 0)) {
    HoistRegion[NumberOfHoisted].MemoryLost = FALSE;
    HoistRegion[NumberOfHoisted].Base = mDiffIntlvLimitAddr;
    HoistRegion[NumberOfHoisted++].Limit = mDiffIntlvLimitAddr;
  }

  // Collect information about the memory ranges discovered by ABL
  for (DramMapIndex = 0; DramMapIndex < RMB_NUMBER_OF_DRAM_REGIONS; DramMapIndex++) {
    DramBaseAddr.Value = FabricRegisterAccRead (0, 0, DRAMBASEADDR0_FUNC, (DRAMBASEADDR0_REG + (DramMapIndex * DRAM_REGION_REGISTER_OFFSET)), RMB_IOMS0_INSTANCE_ID);
    if (DramBaseAddr.Field.AddrRngVal == 1) {
      // Valid region.  Construct normalized map which acts as an overall enable flag while processing
      DramLimitAddr.Value = FabricRegisterAccRead (0, 0, DRAMLIMITADDR0_FUNC, (DRAMLIMITADDR0_REG + (DramMapIndex * DRAM_REGION_REGISTER_OFFSET)), RMB_IOMS0_INSTANCE_ID);
      MemRegion[DramMapIndex].NormalizedMap = (((1 << ChnlIntlv[DramBaseAddr.Field.IntLvNumChan]) - 1) <<
                                               (DramLimitAddr.Field.DstFabricID & (UINT32) ~(RMB_FABRIC_ID_SOCKET_SIZE_MASK << RMB_FABRIC_ID_SOCKET_SHIFT)));
      if (DramBaseAddr.Field.IntLvNumSockets == 1) {
        MemRegion[DramMapIndex].NormalizedMap |= (MemRegion[DramMapIndex].NormalizedMap << NORMALIZED_SOCKET_SHIFT);
      } else {
        MemRegion[DramMapIndex].NormalizedMap <<= (((DramLimitAddr.Field.DstFabricID >> RMB_FABRIC_ID_SOCKET_SHIFT) & RMB_FABRIC_ID_SOCKET_SIZE_MASK) != 0) ? NORMALIZED_SOCKET_SHIFT : 0;
      }
      MemRegion[DramMapIndex].RawBase = DramBaseAddr.Field.DramBaseAddr << 12;
      MemRegion[DramMapIndex].RawLimit = ((DramLimitAddr.Field.DramLimitAddr << 12) | 0xFFF) + 1;
      MemRegion[DramMapIndex].RawSize = MemRegion[DramMapIndex].RawLimit - MemRegion[DramMapIndex].RawBase;
      if (DramBaseAddr.Field.LgcyMmioHoleEn == 1) {
        ASSERT (DramHoleCtrl.Field.DramHoleValid == 1);
        MemRegion[DramMapIndex].RawSize -= (0x10000 - (DramHoleCtrl.Field.DramHoleBase << 8));
      }
      if ((MemRegion[DramMapIndex].RawBase == 0x1000000) && (PreviousLimit != 0x1000000) && !OneTBHoisted) {
        HoistRegion[NumberOfHoisted].MemoryLost = FALSE;
        HoistRegion[NumberOfHoisted].Base = PreviousLimit;
        HoistRegion[NumberOfHoisted++].Limit = 0x1000000;
        OneTBHoisted = TRUE;
      }
      PreviousLimit = MemRegion[DramMapIndex].RawLimit;
    }
  }

  // Walk the physical domains to declare the memory entries
  for (PhysDomain = 0; PhysDomain < mNumberOfPhysicalDomains; PhysDomain++) {
    // Determine overall region base and size
    RegionSize = 0;
    RegionBase = 0xFFFFFFFF;
    RegionTop = 0;
    for (DramMapIndex = 0; DramMapIndex < RMB_NUMBER_OF_DRAM_REGIONS; DramMapIndex++) {
      if ((MemRegion[DramMapIndex].NormalizedMap & mPhysicalDomainInfo[PhysDomain].NormalizedCsMap) != 0) {
        RegionSize += MemRegion[DramMapIndex].RawSize;
        RegionBase = (MemRegion[DramMapIndex].RawBase < RegionBase) ? MemRegion[DramMapIndex].RawBase : RegionBase;
        RegionTop = (MemRegion[DramMapIndex].RawLimit > RegionTop) ? MemRegion[DramMapIndex].RawLimit : RegionTop;
      }
    }

    if (RegionSize != 0) {
      // Spread the region across the shared domains
      ASSERT (mPhysicalDomainInfo[PhysDomain].SharingEntityCount != 0);
      EntitySize = RegionSize / mPhysicalDomainInfo[PhysDomain].SharingEntityCount;
      MemoryBase = RegionBase;
      for (Entity = 0; Entity < mPhysicalDomainInfo[PhysDomain].SharingEntityCount; Entity++) {
        // Get reported domain number
        if (GetNthBitSet (mPhysicalDomainInfo[PhysDomain].SharingEntityMap, Entity, &Domain) != EFI_SUCCESS) {
          return EFI_ABORTED;
        }
        // Piece together this entity's range
        SizeRemaining = EntitySize;
        for (i = 0; i < NumberOfHoisted; i++) {
          if (MemoryBase == HoistRegion[i].Base) {
            MemoryBase = HoistRegion[i].Limit;
          }
          if ((MemoryBase < HoistRegion[i].Base) && ((MemoryBase + SizeRemaining) > HoistRegion[i].Base)) {
            if (((MemoryBase >= mDiffIntlvLimitAddr) && mIsLowIntlvAbove) || ((MemoryBase < mDiffIntlvLimitAddr) && !mIsLowIntlvAbove)) {
              ReportedDomain = Domain + mNumberOfPhysicalDomains; // put the low interleaving region to other domain
            } else {
              ReportedDomain = Domain;
            }
            StoreMemEntry (&mAcpiMemEntryInfo[mNumberOfAcpiMemEntries++], ReportedDomain, MemoryBase, HoistRegion[i].Base - MemoryBase);
            ActualSize = (HoistRegion[i].MemoryLost ? (HoistRegion[i].Limit - MemoryBase) : (HoistRegion[i].Base - MemoryBase));
            SizeRemaining -= ActualSize;
            MemoryBase = HoistRegion[i].Limit;
            mMemoryInfo[mMemoryInfoCtr].Domain = ReportedDomain;
            mMemoryInfo[mMemoryInfoCtr++].RegionSize = LShiftU64 (((UINT64) ActualSize), 16);
          }
        }
        if (((MemoryBase >= mDiffIntlvLimitAddr) && mIsLowIntlvAbove) || ((MemoryBase < mDiffIntlvLimitAddr) && !mIsLowIntlvAbove)) {
          ReportedDomain = Domain + mNumberOfPhysicalDomains; // put the low interleaving region to other domain
        } else {
          ReportedDomain = Domain;
        }
        StoreMemEntry (&mAcpiMemEntryInfo[mNumberOfAcpiMemEntries++], ReportedDomain, MemoryBase, SizeRemaining);
        mMemoryInfo[mMemoryInfoCtr].Domain = ReportedDomain;
        mMemoryInfo[mMemoryInfoCtr++].RegionSize = LShiftU64 (((UINT64) SizeRemaining), 16);
        MemoryBase += SizeRemaining;
      }
    }
  }

  ASSERT (mNumberOfAcpiMemEntries <= (MAX_REPORTED_DOMAINS + MAX_HOISTED));
  ASSERT (mMemoryInfoCtr <= (MAX_REPORTED_DOMAINS + MAX_HOISTED));
  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetGiasInfo
 *
 *  Description:
 *    This function returns information about the Generic Initiator Affinity Structure (GIAS) declared via the SRAT
 *
 *  Parameters:
 *    @param[out]     GiasInfo                   Generic Initiator Affinity structure info
 *
 *    @retval         EFI_SUCCESS                Requested info valid
 *    @retval         EFI_INVALID_PARAMETER      Requested info not returned
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
FabricGetGiasInfo (
     OUT   EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE **GiasInfo
  )
{
  if (GiasInfo != NULL) {
    *GiasInfo = &mAcpiGiasEntryInfo[0];
  }

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetMemoryInfo
 *
 *  Description:
 *    This function returns information about the domains declared via the SRAT
 *
 *  Parameters:
 *    @param[out]     NumberOfDomains            Number of memory entries in the SRAT
 *    @param[out]     MemoryInfo                 Information about the SRAT memory entries
 *
 *    @retval         EFI_SUCCESS                Requested info valid
 *    @retval         EFI_INVALID_PARAMETER      Requested info not returned
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
FabricGetMemoryInfo (
     OUT   UINT32        *NumberOfDomains,
     OUT   MEMORY_INFO  **MemoryInfo
  )
{
  if (mMemoryInfoCtr == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if ((NumberOfDomains == NULL) && (MemoryInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfDomains != NULL) {
    *NumberOfDomains = mMemoryInfoCtr;
  }

  if (MemoryInfo != NULL) {
    *MemoryInfo = &mMemoryInfo[0];
  }

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  StoreMemEntry
 *
 *  Description:
 *    This function saves ACPI table entry information in the appropriate format
 *
 *  Parameters:
 *    @param[out]     Entry                      Location to save the entry to
 *    @param[in]      Domain                     NUMA domain number
 *    @param[in]      Base                       Bits 47:16 of the region's base address
 *    @param[in]      Size                       Bits 47:16 of the region's size
 *
 *---------------------------------------------------------------------------------------
 */
VOID
StoreMemEntry (
     OUT   ACPI_MEM_ENTRY_INFO  *Entry,
  IN       UINT32                Domain,
  IN       UINT32                Base,
  IN       UINT32                Size
  )
{
  Entry->Domain = Domain;
  Entry->BaseLo = (Base & 0x0000FFFF) << 16;
  Entry->BaseHi = (Base & 0xFFFF0000) >> 16;
  Entry->SizeLo = (Size & 0x0000FFFF) << 16;
  Entry->SizeHi = (Size & 0xFFFF0000) >> 16;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GetNumberOfSetBits
 *
 *  Description:
 *    This function returns the number of bits that are set in the given value
 *
 *  Parameters:
 *    @param[in]      BitMap                     Value to check
 *
 *    @return         The number of bits that are set
 *
 *---------------------------------------------------------------------------------------
 */
UINT32
GetNumberOfSetBits (
  IN       UINT32   BitMap
  )
{
  INTN    LSbSet;
  UINT32  Count;

  for (Count = 0, LSbSet = LowBitSet32 (BitMap); LSbSet != -1; Count++, BitMap >>= (UINT32) (LSbSet + 1), LSbSet = LowBitSet32 (BitMap));

  return Count;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GetNthBitSet
 *
 *  Description:
 *    This function returns the bit position of the given set bit count
 *
 *  Parameters:
 *    @param[in]      BitMap                     Value to check
 *    @param[in]      N                          0 - first bit set, 1 - second bit set, etc
 *    @param[out]     BitPosition                Bit position within the BitMap
 *
 *    @retval         EFI_SUCCESS                BitPosition valid
 *    @retval         EFI_INVALID_PARAMETER      Not enough bits are set in BitMap
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
GetNthBitSet (
  IN       UINT32   BitMap,
  IN       UINT32   N,
     OUT   UINT32  *BitPosition
  )
{
  INTN        LSbSet;
  UINT32      Found;
  EFI_STATUS  Status;

  Status = EFI_INVALID_PARAMETER;
  Found = 0;
  for (LSbSet = LowBitSet32 (BitMap); LSbSet != -1; LSbSet = LowBitSet32 (BitMap)) {
    if (Found == N) {
      *BitPosition = (UINT32) LSbSet;
      Status = EFI_SUCCESS;
      break;
    } else {
      Found++;
      BitMap ^= (1 << (UINT32) LSbSet);
    }
  }
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InitPxmDomainInfo
 *
 *  Description:
 *    This function returns the proximity domain(s) for the given fabric ID
 *
 *  Parameters:
 *    @param[in]      FabricId                   Fabric ID to get domain for
 *    @param[in]      NodesPerSocket             Number of nodes in the socket
 *    @param[in]      ActualNps                  Current NPS setting of system
 *    @param[in out]  NumOfNodesAssigned         Number of node(s)/domain(s) assigned
 *    @param[out]     PxmDomainInfo              Associated domain(s)
 *
 *---------------------------------------------------------------------------------------
 */
VOID
InitPxmDomainInfo (
  IN       UINTN             FabricId,
  IN       UINTN             NodesPerSocket,
  IN       UINT8             ActualNps,
  IN OUT   UINT8            *NumOfNodesAssigned,
     OUT   PXM_DOMAIN_INFO  *PxmDomainInfo
  )
{
  UINTN   NodesPerQuad;
  UINTN   BaseNode;
  UINTN   i;

  BaseNode = 0;

  // round nodes per quadrant up if not evenly distributed
  NodesPerQuad = NodesPerSocket/4 + ((NodesPerSocket%4) ? 1 : 0);

  // BaseNode = (socket#[0,1] * nps#[0,1,2,4]) + <node index for NPS#>
  // node index for NPS#:
  //   NPS0: 0xFF
  //   NPS1: 0
  //   NPS2: 0[IOMS0,IOMS1], 1[IOMS2,IOMS3]
  //   NPS4: 0[IOMS0], 1[IOMS1], 2[IOMS2], 3[IOMS3]
  // for CCX as NUMA: BaseNode = (socket#[0,1] * NodesPerSocket) + (Quadrant[0-3] * NodesPerQuad)
  switch (FabricId) {
    case RMB_IOMS0_FABRIC_ID:
      if (!mCcxAsNuma && (ActualNps == DF_DRAM_NPS0)) {
        BaseNode = 0xFF;
      } else {
        BaseNode = 0;
      }
      break;

    default:
      IDS_HDT_CONSOLE (MAIN_FLOW, "Error Invalid Host Bridge Fabric ID!\n");
      ASSERT (FALSE);
      break;
  }

  if (mCcxAsNuma) {
    PxmDomainInfo->Count = NodesPerQuad;
  } else {
    PxmDomainInfo->Count = 1;
  }
  ASSERT (PxmDomainInfo->Count < MAX_PXM_VALUES_PER_QUADRANT);
  for (i = 0; i < PxmDomainInfo->Count; i++) {
    PxmDomainInfo->Domain[i] = BaseNode + i;
    if (mCcxAsNuma && (++(*NumOfNodesAssigned) >= NodesPerSocket)) {
      break;
    }
  }
}

VOID
InitMixedDimmConfig (
  VOID
  )
{
  UINT32                       i;
  UINT32                       j;
  DRAM_BASE_ADDRESS_REGISTER   DramBaseAddr[RMB_NUMBER_OF_DRAM_REGIONS];
  DRAM_LIMIT_ADDRESS_REGISTER  DramLimitAddr[RMB_NUMBER_OF_DRAM_REGIONS];

  mIsMixedDimmConfig = FALSE;
  if (PcdGetBool (PcdAmdFabricMixedDimmConfigNumaDomain) == FALSE) {
    // Mixed DIMM config extended NUMA domain feature disabled
    IDS_HDT_CONSOLE (CPU_TRACE, "  Mixed DIMM config extended NUMA domain feature disabled.\n");
    return;
  }

  for (i = 0; i < RMB_NUMBER_OF_DRAM_REGIONS; i++) {
    DramBaseAddr[i].Value = FabricRegisterAccRead (0, 0, DRAMBASEADDR0_FUNC, (DRAMBASEADDR0_REG + (i * DRAM_REGION_REGISTER_OFFSET)), RMB_IOMS0_INSTANCE_ID);
    DramLimitAddr[i].Value = FabricRegisterAccRead (0, 0, DRAMLIMITADDR0_FUNC, (DRAMLIMITADDR0_REG + (i * DRAM_REGION_REGISTER_OFFSET)), RMB_IOMS0_INSTANCE_ID);
  }

  for (i = 0; i < (RMB_NUMBER_OF_DRAM_REGIONS - 1); i++) {
    if (DramBaseAddr[i].Field.AddrRngVal == 1) {
      mDiffIntlvLimitAddr = DramBaseAddr[i].Field.DramBaseAddr << 12;
      for (j = i + 1; j < RMB_NUMBER_OF_DRAM_REGIONS; j++) {
        if (DramBaseAddr[j].Field.AddrRngVal == 1) {
          if ((DramBaseAddr[j].Field.DramBaseAddr << 12) > mDiffIntlvLimitAddr) {
            mDiffIntlvLimitAddr = DramBaseAddr[j].Field.DramBaseAddr << 12;
          }
          if (DramBaseAddr[i].Field.IntLvNumChan != DramBaseAddr[j].Field.IntLvNumChan) {
            if (((DramBaseAddr[i].Field.IntLvNumChan > DramBaseAddr[j].Field.IntLvNumChan) && \
                 (DramLimitAddr[j].Field.DramLimitAddr > mDiffIntlvLimitAddr)) || \
                ((DramBaseAddr[i].Field.IntLvNumChan < DramBaseAddr[j].Field.IntLvNumChan) && \
                 (DramLimitAddr[i].Field.DramLimitAddr > mDiffIntlvLimitAddr))) {
              mIsLowIntlvAbove = TRUE;
            }
            mIsMixedDimmConfig =  TRUE;
          }
        }
      }
      break;
    }
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "  Mixed DIMM config extended NUMA domain feature enabled.\n");
  IDS_HDT_CONSOLE (CPU_TRACE, "  IsMixedDimmConfig is %s\n", mIsMixedDimmConfig ? L"TRUE" : L"FALSE");
}
