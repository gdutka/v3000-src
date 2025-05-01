/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ACPI CRAT.
 *
 * This function collects CRAT information.
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
#include "Library/AmdBaseLib.h"
#include <Library/AmdIdsHookLib.h>
#include "Library/UefiBootServicesTableLib.h"
#include <Protocol/AmdAcpiCratServicesProtocol.h>
#include <Protocol/FabricNumaServices2.h>
#include "Filecode.h"


#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICACPICRAT_FILECODE

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
FabricCreateCratMemoryInfo (
  IN       AMD_FABRIC_ACPI_CRAT_SERVICES_PROTOCOL     *This,
  IN       CRAT_HEADER                                *CratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

VOID
CreateCratMemoryEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
STATIC AMD_FABRIC_ACPI_CRAT_SERVICES_PROTOCOL  mFabricAcpiCratServicesProtocol = {
  0x1,
  FabricCreateCratMemoryInfo
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
 *  FabricRmbAcpiCratProtocolInstall
 *
 *  Description:
 *    This function will install the CRAT services protocol
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
FabricRmbAcpiCratProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return (gBS->InstallProtocolInterface (
               &ImageHandle,
               &gAmdFabricAcpiCratServicesProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &mFabricAcpiCratServicesProtocol
               ));
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  FabricCreateCratMemoryInfo
 *
 *  Description:
 *    This function adds the system's CRAT memory entries to the CRAT table and updates
 *    the table headers total entry count accordingly.
 *
 *  Parameters:
 *    @param[in]      This                       CRAT services
 *    @param[in]      CratHeaderStructPtr        CRAT table header pointer
 *    @param[in, out] TableEnd                   Current CRAT table pointer to store the memory structures to
 *
 *    @retval         EFI_SUCCESS                Memory structures published successfully
 *    @retval         EFI_ABORTED                A problem was encountered
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
FabricCreateCratMemoryInfo (
  IN       AMD_FABRIC_ACPI_CRAT_SERVICES_PROTOCOL     *This,
  IN       CRAT_HEADER                                *CratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  )
{
  UINT32      MemEntryCount;
  EFI_STATUS  Status;

  Status = FabricCreateSystemAcpiDomainData (TableEnd, &CreateCratMemoryEntry, &MemEntryCount);
  CratHeaderStructPtr->TotalEntries += MemEntryCount;
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CreateCratMemoryEntry
 *
 *  Description:
 *    This function writes one memory entry to the CRAT table and updates the pointer.
 *
 *  Parameters:
 *    @param[in, out] TablePointer               Current CRAT table pointer to store the memory structures to
 *    @param[in]      Domain                     This region's domain
 *    @param[in]      RegionBaseLow              Lower 32 bits of the region's base address
 *    @param[in]      RegionBaseHigh             Upper 32 bits of the region's base address
 *    @param[in]      RegionSizeLow              Lower 32 bits of the region's size
 *    @param[in]      RegionSizeHigh             Upper 32 bits of the region's size
 *
 *---------------------------------------------------------------------------------------
 */
VOID
CreateCratMemoryEntry (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  )
{
  CRAT_MEMORY  *MemoryEntry;

  MemoryEntry = (CRAT_MEMORY *) *TablePointer;
  *TablePointer += sizeof (CRAT_MEMORY);

  MemoryEntry->Type = CRAT_MEMORY_TYPE;
  MemoryEntry->Length = sizeof (CRAT_MEMORY);
  MemoryEntry->ProximityDomain = Domain;
  MemoryEntry->BaseAddressLow = RegionBaseLow;
  MemoryEntry->BaseAddressHigh = RegionBaseHigh;
  MemoryEntry->LengthLow = RegionSizeLow;
  MemoryEntry->LengthHigh = RegionSizeHigh;
  MemoryEntry->Flags.Enabled = 1;
  ///@todo get from memory protocol
  MemoryEntry->Width = 64;
}

