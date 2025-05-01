/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * IOMMU IVRS Table Creation
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
#include  <AGESA.h>
#include  <Gnb.h>
#include  <Filecode.h>
#include  <Library/IdsLib.h>
#include  <Library/GnbCommonLib.h>

#include  "IvrsLibV2.h"

#define FILECODE NBIO_LIBRARY_IVRSLIBV2_IVRSDEBUGDUMP_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Dump IVRS table
 *
 *
 * @param[in]     Ivrs            Pointer to IVRS table
 */

VOID
GnbIommuIvrsTableDump (
  IN       VOID                 *Ivrs
  )
{
  UINT8   *Block;
  UINT8   *Entry;
  Block = (UINT8 *) Ivrs + sizeof (IOMMU_IVRS_HEADER);
  IDS_HDT_CONSOLE (GNB_TRACE, "<----------  IVRS Table Start -----------> \n");
  IDS_HDT_CONSOLE (GNB_TRACE, "  IVInfo           = 0x%08x\n", ((IOMMU_IVRS_HEADER *) Ivrs)-> IvInfo);
  while (Block < ((UINT8 *) Ivrs + ((IOMMU_IVRS_HEADER *) Ivrs)->TableLength)) {
    if (*Block == IvrsIvhdBlock10h || *Block == IvrsIvhdBlock11h || *Block == IvrsIvhdrBlock40h) {
      IDS_HDT_CONSOLE (GNB_TRACE, "  <-------------IVHD Block Start -------->\n");
      IDS_HDT_CONSOLE (GNB_TRACE, "  Flags            = 0x%02x\n", ((IVRS_IVHD_ENTRY *) Block)->Flags);
      IDS_HDT_CONSOLE (GNB_TRACE, "  Type             = 0x%02x\n", ((IVRS_IVHD_ENTRY *) Block)->Type);
      IDS_HDT_CONSOLE (GNB_TRACE, "  DeviceId         = 0x%04x\n", ((IVRS_IVHD_ENTRY *) Block)->DeviceId);
      IDS_HDT_CONSOLE (GNB_TRACE, "  CapabilityOffset = 0x%02x\n", ((IVRS_IVHD_ENTRY *) Block)->CapabilityOffset);
      IDS_HDT_CONSOLE (GNB_TRACE, "  BaseAddress      = 0x%08x%08x\n", (UINT32) (((IVRS_IVHD_ENTRY *) Block)->BaseAddress >> 32), (UINT32) ((IVRS_IVHD_ENTRY *) Block)->BaseAddress);
      IDS_HDT_CONSOLE (GNB_TRACE, "  PCI Segment      = 0x%04x\n", ((IVRS_IVHD_ENTRY *) Block)->PciSegment);
      IDS_HDT_CONSOLE (GNB_TRACE, "  IommuInfo        = 0x%04x\n", ((IVRS_IVHD_ENTRY *) Block)->IommuInfo);
      if (*Block == IvrsIvhdBlock11h) {
        IDS_HDT_CONSOLE (GNB_TRACE, "  IommuAttributes  = 0x%08x\n", ((IVRS_IVHD_ENTRY_11H *) Block)->IommuAttributes);
        IDS_HDT_CONSOLE (GNB_TRACE, "  IommuEfr         = 0x%08x\n", ((IVRS_IVHD_ENTRY_11H *) Block)->IommuEfr);
        Entry = Block + sizeof (IVRS_IVHD_ENTRY_11H);
      } else if ( *Block == IvrsIvhdrBlock40h) {
        IDS_HDT_CONSOLE (GNB_TRACE, "  IommuAttributes  = 0x%08x\n", ((IVRS_IVHD_ENTRY_40H *) Block)->IommuAttributes);
        IDS_HDT_CONSOLE (GNB_TRACE, "  IommuEfr         = 0x%16x\n", ((IVRS_IVHD_ENTRY_40H *) Block)->IommuEfr);
        Entry = Block + sizeof (IVRS_IVHD_ENTRY_40H);
      }
    else{
        IDS_HDT_CONSOLE (GNB_TRACE, "  IommuEfr         = 0x%08x\n", ((IVRS_IVHD_ENTRY_10H *) Block)->IommuEfr);
        Entry = Block + sizeof (IVRS_IVHD_ENTRY_10H);
      }
      IDS_HDT_CONSOLE (GNB_TRACE, "  <-------------IVHD Block Device Entries Start -------->\n");
      while (Entry < (Block + ((IVRS_IVHD_ENTRY *) Block)->Length)) {
        IDS_HDT_CONSOLE (GNB_TRACE, "  ");
        switch (*Entry) {
        case IvhdEntrySelect:
        case IvhdEntryEndRange:
          GnbLibDebugDumpBuffer (Entry, 4, 1, 4);
          Entry = Entry + 4;
          break;
        case IvhdEntryStartRange:
          GnbLibDebugDumpBuffer (Entry, 8, 1, 8);
          Entry = Entry + 8;
          break;
        case IvhdEntryAliasStartRange:
          GnbLibDebugDumpBuffer (Entry, 12, 1, 12);
          Entry = Entry + 12;
          break;
        case IvhdEntryAliasSelect:
        case IvhdEntryExtendedSelect:
        case IvhdEntrySpecialDevice:
          GnbLibDebugDumpBuffer (Entry, 8, 1, 8);
          Entry = Entry + 8;
          break;
        case IvhdEntryPadding:
          Entry = Entry + 4;
          break;
        case IvhdEntryF0Type:
          IDS_HDT_CONSOLE (GNB_TRACE, "  UID length = %d\n", *(Entry+21));
          GnbLibDebugDumpBuffer (Entry, *(Entry + 21) + 22, 1, *(Entry + 21) + 22);
          Entry = Entry + 22 + *(Entry + 21);
          break;
        default:
          IDS_HDT_CONSOLE (GNB_TRACE, "  Unsupported entry type = %d\n", *Entry);
          ASSERT (FALSE);
        }
      }
      IDS_HDT_CONSOLE (GNB_TRACE, "  <-------------IVHD Block Device Entries End -------->\n");
      IDS_HDT_CONSOLE (GNB_TRACE, "  <-------------IVHD Block End ---------->\n");
    } else if (
        (*Block == IvrsIvmdBlock) ||
        (*Block == IvrsIvmdBlockRange) ||
        (*Block == IvrsIvmdBlockSingle) ||
        (*Block == IvrsIvmdrBlock) ||
        (*Block == IvrsIvmdrBlockSingle)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "  <-------------IVMD Block Start -------->\n");
      IDS_HDT_CONSOLE (GNB_TRACE, "  Flags            = 0x%02x\n", ((IVRS_IVMD_ENTRY *) Block)->Flags);
      IDS_HDT_CONSOLE (GNB_TRACE, "  DeviceId         = 0x%04x\n", ((IVRS_IVMD_ENTRY *) Block)->DeviceId);
      switch (*Block) {
      case IvrsIvmdBlock:
      case IvrsIvmdrBlock:
        IDS_HDT_CONSOLE (GNB_TRACE, "  Applies to all devices\n");
        break;
      case IvrsIvmdBlockSingle:
      case IvrsIvmdrBlockSingle:
        IDS_HDT_CONSOLE (GNB_TRACE, "  Applies to a single device\n");
        break;
      default:
        IDS_HDT_CONSOLE (GNB_TRACE, "  DeviceId End   = 0x%04x\n", ((IVRS_IVMD_ENTRY *) Block)->AuxiliaryData);
      }
      IDS_HDT_CONSOLE (GNB_TRACE, "  StartAddress     = 0x%08x%08x\n", (UINT32) (((IVRS_IVMD_ENTRY *) Block)->BlockStart >> 32), (UINT32) ((IVRS_IVMD_ENTRY *) Block)->BlockStart);
      IDS_HDT_CONSOLE (GNB_TRACE, "  BockLength       = 0x%08x%08x\n", (UINT32) (((IVRS_IVMD_ENTRY *) Block)->BlockLength >> 32), (UINT32) ((IVRS_IVMD_ENTRY *) Block)->BlockLength);
      IDS_HDT_CONSOLE (GNB_TRACE, "  <-------------IVMD Block End ---------->\n");
    }
    Block = Block + ((IVRS_IVHD_ENTRY *) Block)->Length;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "<----------  IVRS Table Raw Data --------> \n");
  GnbLibDebugDumpBuffer (Ivrs, ((IOMMU_IVRS_HEADER *) Ivrs)->TableLength, 1, 16);
  IDS_HDT_CONSOLE (GNB_TRACE, "\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "<----------  IVRS Table End -------------> \n");
}

