/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ACPI SRAT.
 *
 * This function collects SRAT information.
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
#include "FabricAcpiTable.h"
#include "FabricAcpiDomainInfo.h"
#include <Library/BaseLib.h>
#include "Library/AmdBaseLib.h"
#include <Library/BaseFabricTopologyLib.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>
#include <Library/AmdIdsHookLib.h>
#include "Library/UefiBootServicesTableLib.h"
#include <Protocol/AmdAcpiSratServicesV2Protocol.h>
#include <Protocol/AmdAcpiSratServicesProtocol.h>
#include <Protocol/FabricNumaServices2.h>
#include <Protocol/FabricTopologyServices2.h>
#include "Filecode.h"


#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICACPISRAT_FILECODE

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
FabricCreateSratMemoryInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL     *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

EFI_STATUS
EFIAPI
FabricCreateSratGiasInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

EFI_STATUS
EFIAPI
FabricCreateSratGpuMemInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

EFI_STATUS
EFIAPI
FabricGetSratMemoryInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL     *This,
     OUT   UINT32                                     *NumberOfDomains,
     OUT   MEMORY_INFO                               **MemoryInfo
  );

EFI_STATUS
EFIAPI
FabricGetSratGiasInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL          *This,
     OUT   EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE **GiasInfo
  );

VOID
CreateSratMemoryEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  );

VOID
CreateSratGiasEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    ProximityDomain,
  IN       UINT8     DeviceHandleType,
  IN       UINT16    PciSegment,
  IN       UINT16    PciBdfNumber
  );

VOID
CreateSratGpuMemoryEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  );

EFI_STATUS
FabricGetGiasInfo (
     OUT   EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE **GiasInfo
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
STATIC AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  mFabricAcpiSratServicesV2Protocol = {
  0x1,
  FabricCreateSratGpuMemInfo,
  FabricCreateSratGiasInfo,
  FabricGetSratGiasInfo
};

STATIC AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL  mFabricAcpiSratServicesProtocol = {
  0x1,
  FabricCreateSratMemoryInfo,
  FabricGetSratMemoryInfo
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
 *  FabricRmbAcpiSratProtocolInstall
 *
 *  Description:
 *    This function will install the SRAT services protocol
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
FabricRmbAcpiSratProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return (gBS->InstallProtocolInterface (
               &ImageHandle,
               &gAmdFabricAcpiSratServicesProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &mFabricAcpiSratServicesProtocol
               ));
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricRnAcpiSratProtocolv2Install
 *
 *  Description:
 *    This function will install the SRAT services protocol
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
FabricRmbAcpiSratV2ProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE *SystemTable
  )
{
  return (gBS->InstallProtocolInterface (
                 &ImageHandle,
                 &gAmdFabricAcpiSratServicesV2ProtocolGuid,
                 EFI_NATIVE_INTERFACE,
                 &mFabricAcpiSratServicesV2Protocol
                 ));
}
/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateSratMemoryInfo
 *
 *  Description:
 *    This function adds the system's SRAT memory entries to the SRAT table
 *
 *  Parameters:
 *    @param[in]      This                       SRAT services
 *    @param[in]      SratHeaderStructPtr        SRAT table header pointer
 *    @param[in, out] TableEnd                   Current SRAT table pointer to store the memory structures to
 *
 *    @retval         EFI_SUCCESS                Memory structures published successfully
 *    @retval         EFI_ABORTED                A problem was encountered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricCreateSratMemoryInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL     *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  )
{
  return FabricCreateSystemAcpiDomainData (TableEnd, &CreateSratMemoryEntry, NULL);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateSratGiasInfo
 *
 *  Description:
 *    This function adds the system's SRAT GIAS  entries to the SRAT table
 *
 *  Parameters:
 *    @param[in]      This                       SRAT services
 *    @param[in]      SratHeaderStructPtr        SRAT table header pointer
 *    @param[in, out] TableEnd                   Current SRAT table pointer to store the memory structures to
 *
 *    @retval         EFI_SUCCESS                Memory structures published successfully
 *    @retval         EFI_ABORTED                A problem was encountered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricCreateSratGiasInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  )
{
  return FabricCreateGiasAcpiDomainData (TableEnd, &CreateSratGiasEntry, NULL);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateSratGpuMemInfo
 *
 *  Description:
 *    This function adds the system's SRAT GPU Mem  entries to the SRAT table
 *
 *  Parameters:
 *    @param[in]      This                       SRAT services
 *    @param[in]      SratHeaderStructPtr        SRAT table header pointer
 *    @param[in, out] TableEnd                   Current SRAT table pointer to store the memory structures to
 *
 *    @retval         EFI_SUCCESS                Memory structures published successfully
 *    @retval         EFI_ABORTED                A problem was encountered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricCreateSratGpuMemInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  )
{
  return FabricCreateGpuMemAcpiDomainData (TableEnd, &CreateSratGpuMemoryEntry, NULL);
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetSratMemoryInfo
 *
 *  Description:
 *    This function returns information about the domains declared via the SRAT
 *
 *  Parameters:
 *    @param[in]      This                       SRAT services
 *    @param[out]     NumberOfDomains            Number of memory entries in the SRAT
 *    @param[out]     MemoryInfo                 Information about the SRAT memory entries
 *
 *    @retval         EFI_SUCCESS                Requested info valid
 *    @retval         EFI_INVALID_PARAMETER      Requested info not returned
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricGetSratMemoryInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL      *This,
     OUT   UINT32                                      *NumberOfDomains,
     OUT   MEMORY_INFO                                **MemoryInfo
  )
{
  if ((NumberOfDomains == NULL) && (MemoryInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return FabricGetMemoryInfo (NumberOfDomains, MemoryInfo);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricGetSratGiasInfo
 *
 *  Description:
 *    This function returns information about the domains declared via the SRAT
 *
 *  Parameters:
 *    @param[in]      This                       SRAT services
 *    @param[out]     NumberOfDomains            Number of memory entries in the SRAT
 *    @param[out]     MemoryInfo                 Information about the SRAT memory entries
 *
 *    @retval         EFI_SUCCESS                Requested info valid
 *    @retval         EFI_INVALID_PARAMETER      Requested info not returned
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricGetSratGiasInfo (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL          *This,
     OUT   EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE **GiasInfo
  )
{
  if ((GiasInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return FabricGetGiasInfo (GiasInfo);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CreateSratGiasEntry
 *
 *  Description:
 *    This function writes one GIAS entry to the SRAT table and updates the pointer.
 *
 *  Parameters:
 *    @param[in, out] TablePointer               Current SRAT table pointer to store the memory structures to
 *    @param[in]      ProximityDomain            This region's domain
 *    @param[in]      DeviceHandleType
 *    @param[in]      PciSegment
 *    @param[in]      PciBdfNumber
 *
 *---------------------------------------------------------------------------------------
 */
VOID
CreateSratGiasEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    ProximityDomain,
  IN       UINT8     DeviceHandleType,
  IN       UINT16    PciSegment,
  IN       UINT16    PciBdfNumber
  )
{
  EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE* GiasEntry;

  GiasEntry = (EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE*)*TablePointer;
  *TablePointer += sizeof(EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE);

  GiasEntry->Type = SRAT_GIAS_TYPE;
  GiasEntry->Length = 32;
  GiasEntry->ProximityDomain = ProximityDomain;
  GiasEntry->DeviceHandleType = DeviceHandleType;
  GiasEntry->DeviceHandle.Pci.PciSegment = PciSegment;
  GiasEntry->DeviceHandle.Pci.PciBdfNumber = PciBdfNumber;
  GiasEntry->Flags = 1; // Bit 0, Enabled. If clear, the OSPM ignores the contents of the Generic Initiator Affinity Structure.
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CreateSratGpuMemoryEntry
 *
 *  Description:
 *    This function writes one GPU memory entry to the SRAT table and updates the pointer.
 *
 *  Parameters:
 *    @param[in, out] TablePointer               Current SRAT table pointer to store the memory structures to
 *    @param[in]      Domain                     This region's domain
 *    @param[in]      RegionBaseLow              Lower 32 bits of the region's base address
 *    @param[in]      RegionBaseHigh             Upper 32 bits of the region's base address
 *    @param[in]      RegionSizeLow              Lower 32 bits of the region's size
 *    @param[in]      RegionSizeHigh             Upper 32 bits of the region's size
 *
 *---------------------------------------------------------------------------------------
 */
VOID
CreateSratGpuMemoryEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  )
{
  EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE* MemoryEntry;

  MemoryEntry = (EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE*)*TablePointer;
  *TablePointer += sizeof(SRAT_MEMORY);

  MemoryEntry->Type = SRAT_MEMORY_TYPE;
  MemoryEntry->Length = sizeof(EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE);
  MemoryEntry->ProximityDomain = Domain;
  MemoryEntry->AddressBaseLow = RegionBaseLow;
  MemoryEntry->AddressBaseHigh = RegionBaseHigh;
  MemoryEntry->LengthLow = RegionSizeLow;
  MemoryEntry->LengthHigh = RegionSizeHigh;
  MemoryEntry->Flags = 1;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CreateSratMemoryEntry
 *
 *  Description:
 *    This function writes one memory entry to the SRAT table and updates the pointer.
 *
 *  Parameters:
 *    @param[in, out] TablePointer               Current SRAT table pointer to store the memory structures to
 *    @param[in]      Domain                     This region's domain
 *    @param[in]      RegionBaseLow              Lower 32 bits of the region's base address
 *    @param[in]      RegionBaseHigh             Upper 32 bits of the region's base address
 *    @param[in]      RegionSizeLow              Lower 32 bits of the region's size
 *    @param[in]      RegionSizeHigh             Upper 32 bits of the region's size
 *
 *---------------------------------------------------------------------------------------
 */
VOID
CreateSratMemoryEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  )
{
  SRAT_MEMORY  *MemoryEntry;

  MemoryEntry = (SRAT_MEMORY *) *TablePointer;
  *TablePointer += sizeof (SRAT_MEMORY);

  MemoryEntry->Type = SRAT_MEMORY_TYPE;
  MemoryEntry->Length = sizeof (SRAT_MEMORY);
  MemoryEntry->ProximityDomain = Domain;
  MemoryEntry->BaseAddressLow = RegionBaseLow;
  MemoryEntry->BaseAddressHigh = RegionBaseHigh;
  MemoryEntry->LengthLow = RegionSizeLow;
  MemoryEntry->LengthHigh = RegionSizeHigh;
  MemoryEntry->Flags.Enabled = 1;
}

