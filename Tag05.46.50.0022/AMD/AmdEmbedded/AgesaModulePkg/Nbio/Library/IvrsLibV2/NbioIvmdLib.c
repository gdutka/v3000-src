/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe ALIB
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


#include  <GnbDxio.h>
#include  <GnbIommu.h>
#include  <Library/IdsLib.h>
#include  <Library/AmdBaseLib.h>

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <AGESA.h>
#include  <Gnb.h>
#include  <Library/IdsLib.h>
#include  <Library/GnbPcieConfigLib.h>
#include  <Include/GnbIommu.h>


#include  "IvrsLibV2.h"

#define FILECODE NBIO_LIBRARY_IVRSLIBV2_NBIOIVMDLIB_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Create IVMD entry
 *
 *
 * @param[in]  Type            Root type for IVMD (IvrsIvmdBlock or IvrsIvmdrBlock)
 * @param[in]  StartDevice     Device ID of start device range
 *                             Use 0x0000 for ALL
 * @param[in]  EndDevice       Device ID of end device range
 *                             Use 0xFFFF for ALL
 *                             Use == StartDevice for specific device
 * @param[in]  BlockAddress    Address of memory block to be excluded
 * @param[in]  BlockLength     Length of memory block go be excluded
 * @param[in]  Ivmd            Pointer to IVMD entry
 *
 */
STATIC
VOID
GnbIvmdAddEntry (
  IN       IVRS_BLOCK_TYPE      Type,
  IN       UINT16               StartDevice,
  IN       UINT16               EndDevice,
  IN       UINT64               BlockAddress,
  IN       UINT64               BlockLength,
  IN       IVRS_IVMD_ENTRY      *Ivmd
  )
{
  Ivmd->Flags = IVMD_FLAG_EXCLUSION_RANGE;
  Ivmd->Length = sizeof (IVRS_IVMD_ENTRY);
  Ivmd->DeviceId = StartDevice;
  Ivmd->AuxiliaryData = 0x0;
  Ivmd->Reserved = 0x0000000000000000;
  Ivmd->BlockStart = BlockAddress;
  Ivmd->BlockLength = BlockLength;
  if (Type == IvrsIvmdBlock) {
    if (StartDevice == EndDevice) {
      Ivmd->Type = IvrsIvmdBlockSingle;
    } else if ((StartDevice == 0x0000) && (EndDevice == 0xFFFF)) {
      Ivmd->Type = IvrsIvmdBlock;
    } else {
      Ivmd->Type = IvrsIvmdBlockRange;
      Ivmd->AuxiliaryData = EndDevice;
    }
  } else {
    if (StartDevice == EndDevice) {
      Ivmd->Type = IvrsIvmdrBlockSingle;
    } else {
      Ivmd->Type = IvrsIvmdrBlock;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build IVMD list
 *
 *
 * @param[in]  Type            Entry type
 * @param[in]  Ivrs            IVRS table pointer
 *
 */

AGESA_STATUS
GnbBuildIvmdList (
  IN       IVRS_BLOCK_TYPE            Type,
  IN       VOID                       *Ivrs
  )
{
  IOMMU_EXCLUSION_RANGE_DESCRIPTOR  *IvrsExclusionRangeList;
  IVRS_IVMD_ENTRY                   *Ivmd;
  UINT16                            StartId;
  UINT16                            EndId;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBuildIvmdList Entry\n");
  IvrsExclusionRangeList = (IOMMU_EXCLUSION_RANGE_DESCRIPTOR *) (UINTN) PcdGet64 (PcdAmdIvrsExclusionRangePtr);
  IDS_HDT_CONSOLE (GNB_TRACE, "IvrsExclusionRangeList %x\n", IvrsExclusionRangeList);

  if (IvrsExclusionRangeList != NULL) {
    // Process the entire IvrsExclusionRangeList here and create an IVMD for eache entry
    IDS_HDT_CONSOLE (GNB_TRACE, "Process Exclusion Range List\n");
    while ((IvrsExclusionRangeList->Flags & DESCRIPTOR_TERMINATE_LIST) == 0) {
      if ((IvrsExclusionRangeList->Flags & DESCRIPTOR_IGNORE) == 0) {
        // Address of IVMD entry
        Ivmd = (IVRS_IVMD_ENTRY*) ((UINT8 *)Ivrs + ((IOMMU_IVRS_HEADER *) Ivrs)->TableLength);
        StartId =
          (IvrsExclusionRangeList->RequestorIdStart.Bus << 8) +
          (IvrsExclusionRangeList->RequestorIdStart.Device << 3) +
          (IvrsExclusionRangeList->RequestorIdStart.Function);
        EndId =
          (IvrsExclusionRangeList->RequestorIdEnd.Bus << 8) +
          (IvrsExclusionRangeList->RequestorIdEnd.Device << 3) +
          (IvrsExclusionRangeList->RequestorIdEnd.Function);
        GnbIvmdAddEntry (
          Type,
          StartId,
          EndId,
          IvrsExclusionRangeList->RangeBaseAddress,
          IvrsExclusionRangeList->RangeLength,
          Ivmd);
        // Add entry size to existing table length
        ((IOMMU_IVRS_HEADER *)Ivrs)->TableLength += sizeof (IVRS_IVMD_ENTRY);
        IDS_HDT_CONSOLE (GNB_TRACE, "TableLength %x\n", ((IOMMU_IVRS_HEADER *)Ivrs)->TableLength);
        IDS_HDT_CONSOLE (GNB_TRACE, "Bus %x\n", IvrsExclusionRangeList->RequestorIdEnd.Bus);
        IDS_HDT_CONSOLE (GNB_TRACE, "Device %x\n", IvrsExclusionRangeList->RequestorIdEnd.Device);
        IDS_HDT_CONSOLE (GNB_TRACE, "RangeBaseAddress %x\n", IvrsExclusionRangeList->RangeBaseAddress);
      }
      // Point to next entry in IvrsExclusionRangeList
      IvrsExclusionRangeList++;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBuildIvmdList Exit\n");
  return AGESA_SUCCESS;
}



