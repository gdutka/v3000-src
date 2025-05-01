/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * NbioTopMem - Set top of memory for NBIO
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioBaseZPPei
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <AMD.h>
#include <PiPei.h>
#include <Filecode.h>
#include <GnbRegistersRMB.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Guid/AmdMemoryInfoHob.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Library/GnbPciAccLib.h>


#define FILECODE        NBIO_NBIOBASE_RMB_PEI_NBIOTOPMEM_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

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

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Memory config complete callback
 * This function should use the information from memory config to initialize NBIO top of memory
 *
 *
 *
 * @param[in]  PeiServices        EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor   EFI_PEI_NOTIFY_DESCRIPTOR pointer
 * @param[in]  Ppi                VOID pointer to the PPI that generated this callback
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
MemoryConfigDoneCallbackPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                      Status;
  GNB_HANDLE                      *GnbHandle;
  AMD_CONFIG_PARAMS               *StdHeader;
  EFI_PEI_HOB_POINTERS            Hob;
  AMD_MEMORY_INFO_HOB             *AmdMemoryInfoHob;
  UINT32                          Index;
  AMD_MEMORY_RANGE_DESCRIPTOR     *Range;
  PCIe_PLATFORM_CONFIG            *Pcie;
  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *PcieServicesPpi;
  GNB_PCIE_INFORMATION_DATA_HOB   *PciePlatformConfigHob;
  UINT64                          MsrData;
  UINT64                          GnbTom2;
  UINT64                          GnbTom3;
  NB_TOP_OF_DRAM_SLOT1_STRUCT     D0F0_090;
  NB_LOWER_TOP_OF_DRAM2_STRUCT    LOWER_TOP_OF_DRAM2;
  NB_UPPER_TOP_OF_DRAM2_STRUCT    UPPER_TOP_OF_DRAM2;
  NB_TOP_OF_DRAM3_STRUCT          NBMISC_0138;

  AGESA_TESTPOINT (TpMemoryConfigDoneCallbackPpiEntry, NULL);
  GnbHandle = NULL;
  StdHeader = NULL;
  // Need topology structure to get GnbHandle
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieServicesPpiGuid,
                             0,
                             NULL,
                             (VOID **)&PcieServicesPpi
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);

  while (GnbHandle != NULL) {
    // Read TOP_OF_DRAM registers
    GnbLibPciRead (
      GnbHandle->Address.AddressValue | PCICFG_NBIO_IOHUB_NB_TOP_OF_DRAM_SLOT1_ADDRESS,
      AccessWidth32,
      &D0F0_090.Value,
      NULL
      );
    SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_NB_LOWER_TOP_OF_DRAM2_ADDRESS), &LOWER_TOP_OF_DRAM2.Value);
    SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_NB_UPPER_TOP_OF_DRAM2_ADDRESS), &UPPER_TOP_OF_DRAM2.Value);

    // Locate AMD_MEMORY_INFO_HOB
    AmdMemoryInfoHob = NULL;
    Status = PeiServicesGetHobList (&Hob.Raw);
    while (!END_OF_HOB_LIST (Hob)) {
      if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
          CompareGuid ( &Hob.Guid->Name, &gAmdMemoryInfoHobGuid)) {
          AmdMemoryInfoHob = (AMD_MEMORY_INFO_HOB *) (Hob.Raw +
                              sizeof (EFI_HOB_GENERIC_HEADER) +
                              sizeof (EFI_GUID));
        Status = EFI_SUCCESS;
        break;
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }

    // Get the STB,TOOL,BIOS reserved region
    if (AmdMemoryInfoHob != NULL) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "AMD_MEMORY_INFO_HOB at 0x%x\n", AmdMemoryInfoHob);
      IDS_HDT_CONSOLE (MAIN_FLOW, "  Version: 0x%x\n", AmdMemoryInfoHob->Version);
      IDS_HDT_CONSOLE (MAIN_FLOW, "  NumberOfDescriptor: 0x%x\n", AmdMemoryInfoHob->NumberOfDescriptor);
      for (Index = 0; Index < AmdMemoryInfoHob->NumberOfDescriptor; Index++) {
        Range = (AMD_MEMORY_RANGE_DESCRIPTOR*)&AmdMemoryInfoHob->Ranges[Index];
        IDS_HDT_CONSOLE (MAIN_FLOW, "  Range: %d\n", Index);
        IDS_HDT_CONSOLE (MAIN_FLOW, "    Base: 0x%lx\n", Range->Base);
        IDS_HDT_CONSOLE (MAIN_FLOW, "    Size: 0x%lx\n", Range->Size);
        IDS_HDT_CONSOLE (MAIN_FLOW, "    Attribute: 0x%x\n", Range->Attribute);
        if ((Range->Attribute) == AMD_MEMORY_ATTRIBUTE_Reserved_SmuFeatures) {
         IDS_HDT_CONSOLE (MAIN_FLOW, "    SmuFeatures \n");
         PcdSet32S (PcdReservedSmuMemoryHi, (UINT32)RShiftU64 (Range->Base, 32));
         PcdSet32S (PcdReservedSmuMemoryLo, (UINT32)((Range->Base) & 0xFFFFFFFF));
        }
      }
    }
    //Read memory size below 4G from MSR C001_001A
    MsrData = AsmReadMsr64 (TOP_MEM);
    //Write to NB register 0x90
    D0F0_090.Field.TOP_OF_DRAM = ((UINT32)MsrData & 0xFF800000) >> 23;     //Keep bits 31:23

    GnbTom2 = 0;
    GnbTom3 = 0;
    MsrData = AsmReadMsr64 (SYS_CFG);
    if ((MsrData & BIT21) != 0) {
      //If SYS_CFG(MtrrTom2En) then configure GNB TOM2 and TOM3
      //Read memory size above 4G from TOP_MEM2 (MSR C001_001D)
      MsrData = AsmReadMsr64 (TOP_MEM2);
      IDS_HDT_CONSOLE (GNB_TRACE, "MSR TOP_MEM2[63:32] is 0x%08x\n", RShiftU64 (MsrData, 32));
      IDS_HDT_CONSOLE (GNB_TRACE, "MSR TOP_MEM2[31:0] is 0x%08x\n", MsrData);
      if ((MsrData & (UINT64)0x0000FFFFFFC00000) > ((UINT64)0x0000010000000000)) {
        // If TOP_MEM2 is above 1TB, enable GNB TOM2 and TOM3
        // Set TOM2 for below 1TB limit
        GnbTom2 = 0x000000FD00000000;
        // TOM3 is INCLUSIVE, so set it to TOM - 1 using bits 51:22
        GnbTom3 = (MsrData - 1) & (UINT64)0x000FFFFFFFC00000;
      } else {
        // If TOP_MEM2 is below 1TB, set TOM2 using bits 40:23
        GnbTom2 = MsrData & (UINT64)0x000001FFFF800000;       //Keep bits 40:23
        // If TOP_MEM2 is below 1TB, disable GNB TOM3
        GnbTom3 = 0;
      }
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSetTomRMB setting GnbTom2 to 0x%lx\n", GnbTom2);
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSetTomRMB setting GnbTom3 to 0x%lx\n", GnbTom3);
    if (GnbTom2 != 0) {
      // Write memory size[40:32] to indirect register 1A[7:0]
      UPPER_TOP_OF_DRAM2.Field.UPPER_TOM2 = (UINT32) (RShiftU64 (GnbTom2, 32) & 0xFF);
      // Write memory size[31:23] to indirect register 19[31:23] and enable memory through bit 0
      LOWER_TOP_OF_DRAM2.Field.LOWER_TOM2 = ((UINT32)GnbTom2 & 0xFF800000) >> 23;
      LOWER_TOP_OF_DRAM2.Field.ENABLE = 1;
    }

    if (GnbTom3 != 0) {
      // Above 1TB addressing TOM3 if MSR TOM is above 1TB
      NBMISC_0138.Field.TOM3_LIMIT = (UINT32) RShiftU64 (GnbTom3, 22);
      NBMISC_0138.Field.TOM3_ENABLE = 1;
    }
      // Write TOP_OF_DRAM registers
    GnbLibPciWrite (GnbHandle->Address.AddressValue | PCICFG_NBIO_IOHUB_NB_TOP_OF_DRAM_SLOT1_ADDRESS, AccessWidth32, &D0F0_090.Value, NULL);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_NB_LOWER_TOP_OF_DRAM2_ADDRESS), &LOWER_TOP_OF_DRAM2.Value, 0);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_NB_UPPER_TOP_OF_DRAM2_ADDRESS), &UPPER_TOP_OF_DRAM2.Value, 0);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_NB_TOP_OF_DRAM3_ADDRESS), &NBMISC_0138.Value, 0);
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SMN_NB_PCI_CTRL_ADDRESS),
                     (UINT32) ~(NB_PCI_CTRL_MMIOEnable_MASK),
                     (1 << NB_PCI_CTRL_MMIOEnable_OFFSET),
                     0
                     );

    GnbHandle = GnbGetNextHandle (GnbHandle);
  }
  AGESA_TESTPOINT (TpMemoryConfigDoneCallbackPpiExit, NULL);

  return EFI_SUCCESS;
}




