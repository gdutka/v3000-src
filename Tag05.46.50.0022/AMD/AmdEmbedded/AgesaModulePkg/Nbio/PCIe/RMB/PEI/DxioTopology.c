/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * DxioTopology - Update DXIO topology structure
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioBaseZPPei
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */

#include <AMD.h>
#include <Gnb.h>
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
#include <GnbRegistersRMB.h>

//From AgesaPkg
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieComplexPpi.h>
#include <Ppi/NbioPcieTopologyPpi.h>

#include <Guid/GnbPcieInfoHob.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/NbioPcieTrainingPpi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieMiscCommLib.h>
//Dependent on AgesaPkg
#include <Library/DxioLibV2.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/MemoryAllocationLib.h>

#include "AmdNbioPciePei.h"

#define FILECODE        NBIO_PCIE_RMB_PEI_DXIOTOPOLOGY_FILECODE

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
STATIC DXIO_PORT_DESCRIPTOR TemplateTopologyPort =
  {
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioUnusedEngine, 0, 0, DxioHotplugDisabled, 0),
    DXIO_PORT_DATA_INITIALIZER_PCIE (
      DxioPortEnabled,                      // Port Present
      0,                                    // Requested Device
      0,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // ASPM
      DxioAspmDisabled,                     // ASPM L1.1 disabled
      DxioAspmDisabled,                     // ASPM L1.2 disabled
      DxioClkPmSupportDisabled              // Clock PM
    )
  };
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to user engine descriptor
 *
 *
 * @param[in]  EngineDescriptor   Pointer to engine descriptor
 */
VOID
DxioUserDescriptorConfigDump (
  IN      DXIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{
  IDS_HDT_CONSOLE (PCIE_MISC, "  Engine Type - %a\n",
    (EngineDescriptor->EngineData.EngineType == DxioPcieEngine) ? "PCIe Port" : (
    (EngineDescriptor->EngineData.EngineType == DxioSATAEngine) ? "SATA Port" : (
    (EngineDescriptor->EngineData.EngineType == DxioEthernetEngine) ? "Ethernet Port" : (
    (EngineDescriptor->EngineData.EngineType == DxioUSB_OVER_PCIE) ? "DxioUSB_OVER_PCIE" : (
    (EngineDescriptor->EngineData.EngineType == DxioUnusedEngine) ? "Unused" : "Invalid"))))
    );
  IDS_HDT_CONSOLE (PCIE_MISC, "    Start Phy Lane - %d\n    End   Phy Lane - %d\n",
    EngineDescriptor->EngineData.StartLane,
    EngineDescriptor->EngineData.EndLane
  );
  IDS_HDT_CONSOLE (PCIE_MISC, "    Hotplug - %d\n", EngineDescriptor->EngineData.HotPluggable);
  IDS_HDT_CONSOLE (PCIE_MISC, "    GpioGroupId - %d\n", EngineDescriptor->EngineData.GpioGroupId);
  if (EngineDescriptor->EngineData.EngineType == PciePortEngine) {
    IDS_HDT_CONSOLE (PCIE_MISC, "    PortPresent         - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.PortPresent);
    IDS_HDT_CONSOLE (PCIE_MISC, "    DeviceNumber        - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.DeviceNumber);
    IDS_HDT_CONSOLE (PCIE_MISC, "    FunctionNumber      - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.FunctionNumber);
    IDS_HDT_CONSOLE (PCIE_MISC, "    LinkSpeedCapability - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkSpeedCapability);
    IDS_HDT_CONSOLE (PCIE_MISC, "    ClkPmSupport        - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls.ClkPmSupport);
    IDS_HDT_CONSOLE (PCIE_MISC, "    LinkAspm            - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspm);
    IDS_HDT_CONSOLE (PCIE_MISC, "    LinkAspmL1.1        - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspmL1_1);
    IDS_HDT_CONSOLE (PCIE_MISC, "    LinkAspmL1.2        - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspmL1_2);
    IDS_HDT_CONSOLE (PCIE_MISC, "    ClkReq              - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.ClkReq);
    IDS_HDT_CONSOLE (PCIE_MISC, "    LinkHotplug         - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkHotplug);
    IDS_HDT_CONSOLE (PCIE_MISC, "    SB link             - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls.SbLink);
    IDS_HDT_CONSOLE (PCIE_MISC, "    IsMasterPLL         - %d\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.IsMasterPLL);
    IDS_HDT_CONSOLE (PCIE_MISC, "    MiscControls        - 0x%02x\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls);
    IDS_HDT_CONSOLE (PCIE_MISC, "    SlotPowerLimit      - 0x%02x\n", ((DXIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.SlotPowerLimit);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to debug out
 *
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 */
VOID
DxioUserConfigDump (
  IN      DXIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;
  UINTN                     Index;
  DXIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;

  IDS_HDT_CONSOLE (PCIE_MISC, "<---------- PCIe User Config Start------------->\n");
  IDS_HDT_CONSOLE(PCIE_MISC, " BMC Link Location is %x\n", ComplexDescriptor->BmcLinkLocation);
  LocalDescriptor = ComplexDescriptor;
  Index = 0;
  while (LocalDescriptor != NULL) {
    IDS_HDT_CONSOLE (PCIE_MISC, "<---------------- Socket %d ----------------->\n", Index);
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      DxioUserDescriptorConfigDump (EngineDescriptor);
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
    Index++;
  }
  IDS_HDT_CONSOLE (PCIE_MISC, "<---------- PCIe User Config End-------------->\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialization settings that must be set in the topology structure
 *
 *
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 */

VOID
DxioTopologyWorkarounds (
  IN      DXIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;
  UINTN                     Index;
  DXIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  LocalDescriptor = ComplexDescriptor;
  Index = 0;
  while (LocalDescriptor != NULL) {
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      if (EngineDescriptor->EngineData.EngineType == DxioPcieEngine) {
        PcieGenSpeedWorkaround (EngineDescriptor);
        // Temporarily disable hotplug
        //EngineDescriptor->Port.LinkHotplug = 0;
        // Enable TurnOffUnusedLanes
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 1;
        // PT Link Early training must be at Gen1
        if (EngineDescriptor->Port.MiscControls.SbLink == 1) {
          if (EngineDescriptor->Port.LinkSpeedCapability == DxioGenMaxSupported) {
            PcdSet8S (PcdPTTargetSpeed, DxioGen3);
          } else {
            PcdSet8S  (PcdPTTargetSpeed, EngineDescriptor->Port.LinkSpeedCapability);
          }
        } else if (EngineDescriptor->EngineData.EngineType == DxioSATAEngine) {
          // Set SATA Channel Type
          EngineDescriptor->Port.MiscControls.ChannelType = PcdGet8 (PcdCfgSataPhyTuning);

        }
        if (PcdGetBool (PcdPcieLinkComplianceModeAllPorts)) {
          EngineDescriptor->Port.MiscControls.LinkComplianceMode = 1;
        }
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
    Index++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to user engine descriptor
 *
 *
 * @param[in]  EngineDescriptor   Pointer to engine descriptor
 */
VOID
DxioManageEngineDescriptor (
  IN      DXIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{
  PORT_PARAM      *PortParam;
  DXIO_PHY_PARAM  *PhyParam;
  UINT32          SataGen1Settings;
  UINT32          SataGen2Settings;
  UINT32          SataGen3Settings;

  SataGen1Settings = 0;
  SataGen2Settings = 0;
  SataGen3Settings = 0;
  PortParam = (PORT_PARAM *) &(EngineDescriptor->PortParams);
  PhyParam = (DXIO_PHY_PARAM *) &(EngineDescriptor->Phy);
  // Locate first empty PHY Parameter
  while (PhyParam->ParamType != 0) {
    if (EngineDescriptor->EngineData.EngineType == DxioSATAEngine) {
      IDS_HDT_CONSOLE (PCIE_MISC, "EngineType - 0x%x\n    ParamType - 0x%x\n    ParamValue - 0x%x\n", \
                       EngineDescriptor->EngineData.EngineType, PhyParam->ParamType, PhyParam->ParamValue);
      switch (PhyParam->ParamType) {
      case GEN1_txX_eq_pre:
        SataGen1Settings |= PhyParam->ParamValue & 0xFF;
        break;
      case GEN1_txX_eq_main:
        SataGen1Settings |= (PhyParam->ParamValue << 8) & 0xFF00;
        break;
      case GEN1_txX_eq_post:
        SataGen1Settings |= (PhyParam->ParamValue << 16) & 0xFF0000;
        break;
      case GEN1_rxX_eq_ctle_boost:
        SataGen1Settings |= (PhyParam->ParamValue << 24) & 0x1F000000;
        break;
      case GEN1_rxX_eq_ctle_pole:
        SataGen1Settings |= (PhyParam->ParamValue << 29) & 0xE0000000;
        break;
      case GEN2_txX_eq_pre:
        SataGen2Settings |= PhyParam->ParamValue & 0xFF;
        break;
      case GEN2_txX_eq_main:
        SataGen2Settings |= (PhyParam->ParamValue << 8) & 0xFF00;
        break;
      case GEN2_txX_eq_post:
        SataGen2Settings |= (PhyParam->ParamValue << 16) & 0xFF0000;
        break;
      case GEN2_rxX_eq_ctle_boost:
        SataGen2Settings |= (PhyParam->ParamValue << 24) & 0x1F000000;
        break;
      case GEN2_rxX_eq_ctle_pole:
        SataGen2Settings |= (PhyParam->ParamValue << 29) & 0xE0000000;
        break;
      case GEN3_txX_eq_pre:
        SataGen3Settings |= PhyParam->ParamValue & 0xFF;
        break;
      case GEN3_txX_eq_main:
        SataGen3Settings |= (PhyParam->ParamValue << 8) & 0xFF00;
        break;
      case GEN3_txX_eq_post:
        SataGen3Settings |= (PhyParam->ParamValue << 16) & 0xFF0000;
        break;
      case GEN3_rxX_eq_ctle_boost:
        SataGen3Settings |= (PhyParam->ParamValue << 24) & 0x1F000000;
        break;
      case GEN3_rxX_eq_ctle_pole:
        SataGen3Settings |= (PhyParam->ParamValue << 29) & 0xE0000000;
        break;
      default:
        break;
      }
    }
    PhyParam++;
  }
  if (SataGen1Settings) {
    PcdSet32S  (PcdCfgDxioSataGen1Settings, SataGen1Settings);
  }
  if (SataGen2Settings) {
    PcdSet32S  (PcdCfgDxioSataGen2Settings, SataGen2Settings);
  }
  if (SataGen3Settings) {
    PcdSet32S  (PcdCfgDxioSataGen3Settings, SataGen3Settings);
  }

  while (PortParam->ParamType != 0) {
    IDS_HDT_CONSOLE (PCIE_MISC, "    ParamType - 0x%x\n    ParamValue - 0x%x\n", \
                                     PortParam->ParamType, PortParam->ParamValue);
    switch (PortParam->ParamType) {
    case PP_DEVICE:
      EngineDescriptor->Port.DeviceNumber = (UINT8) PortParam->ParamValue;
      break;
    case PP_FUNCTION:
      EngineDescriptor->Port.FunctionNumber = (UINT8) PortParam->ParamValue;
      break;
    case PP_PORT_PRESENT:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.PortPresent = 1;
      } else {
        EngineDescriptor->Port.PortPresent = 0;
      }
      break;
    case PP_LINK_SPEED_CAP:
      EngineDescriptor->Port.LinkSpeedCapability = (UINT8) PortParam->ParamValue;
        break;
    case PP_LINK_ASPM:
      EngineDescriptor->Port.LinkAspm = (UINT8) PortParam->ParamValue;
        break;
    case PP_HOTPLUG_TYPE:
      EngineDescriptor->Port.LinkHotplug = (UINT8) PortParam->ParamValue;
        break;
    case PP_CLKREQ:
      EngineDescriptor->Port.ClkReq = (UINT8) PortParam->ParamValue;
        break;
    case PP_ASPM_L1_1:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.LinkAspmL1_1 = 1;
      } else {
        EngineDescriptor->Port.LinkAspmL1_1 = 0;
      }
      break;
    case PP_ASPM_L1_2:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.LinkAspmL1_2 = 1;
      } else {
        EngineDescriptor->Port.LinkAspmL1_2 = 0;
      }
      break;
    case PP_COMPLIANCE:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.MiscControls.LinkComplianceMode = 1;
      } else {
        EngineDescriptor->Port.MiscControls.LinkComplianceMode = 0;
      }
      break;
    case PP_SAFE_MODE:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.MiscControls.LinkSafeMode = 1;
      } else {
        EngineDescriptor->Port.MiscControls.LinkSafeMode = 0;
      }
      break;
    case PP_CHIPSET_LINK:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.MiscControls.SbLink = 1;
      } else {
        EngineDescriptor->Port.MiscControls.SbLink = 0;
      }
      break;
    case PP_CLOCK_PM:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.MiscControls.ClkPmSupport = 1;
      } else {
        EngineDescriptor->Port.MiscControls.ClkPmSupport = 0;
      }
      break;
    case PP_CHANNELTYPE:
      EngineDescriptor->Port.MiscControls.ChannelType = (UINT8) PortParam->ParamValue;
      break;
    case PP_TURN_OFF_UNUSED_LANES:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 1;
      } else {
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 0;
      }
      break;
    case PP_APIC_GROUPMAP:
      EngineDescriptor->Port.ApicDeviceInfo.GroupMap = (UINT8) PortParam->ParamValue;
      break;
    case PP_APIC_SWIZZLE:
      EngineDescriptor->Port.ApicDeviceInfo.Swizzle = (UINT8) PortParam->ParamValue;
      break;
    case PP_APIC_BRIDGEINT:
      EngineDescriptor->Port.ApicDeviceInfo.BridgeInt = (UINT8) PortParam->ParamValue;
      break;
    case PP_MASTER_PLL:
      if (PortParam->ParamValue == TRUE) {
        EngineDescriptor->Port.IsMasterPLL = 1;
      } else {
        EngineDescriptor->Port.IsMasterPLL = 0;
      }
      break;
    case PP_SLOT_NUM:
      EngineDescriptor->Port.SlotNum = PortParam->ParamValue;
      break;
    case PP_PHY_PARAM:
      *(UINT16 *) PhyParam = PortParam->ParamValue;
      PhyParam++;
      break;
    case PP_ESM:
      EngineDescriptor->Port.EsmControl = (UINT8)PortParam->ParamValue;
      break;
    case PP_CCIX:
      EngineDescriptor->Port.CcixControl = (UINT8)PortParam->ParamValue;
      break;
    case PP_PSPP_DC:
      EngineDescriptor->Port.PsppPolicyDC = (UINT16)PortParam->ParamValue;
      break;
    case PP_PSPP_AC:
      EngineDescriptor->Port.PsppPolicyAC = (UINT16)PortParam->ParamValue;
      break;
    default:
      break;
    }
    PortParam++;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback for NbioSmuServicesPpi installation
 *
 *
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 */

VOID
DxioManageTopology (
  IN      DXIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;
  UINTN                     Index;
  DXIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  LocalDescriptor = ComplexDescriptor;
  Index = 0;
  while (LocalDescriptor != NULL) {
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      DxioManageEngineDescriptor (EngineDescriptor);
      if (EngineDescriptor->EngineData.EngineType == PciePortEngine) {
        switch (PcdGet8 (PcdPsppPolicy)) {
        case 1:  // Performance
          break;
        case 2:  // Balanced
          break;
        case 3:  // Power Saving
          break;
        default:
          break;
        }
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
    Index++;
  }
  DxioUserConfigDump (ComplexDescriptor);
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}


/*----------------------------------------------------------------------------------------*/
/**
  GetDescriptorHandle
    A hash function that generates a handle from a descriptor

  @param InDescriptor  Pointer to DXIO_PORT_DESCRIPTOR

  @return UINT64       Hash value meant for comparison,
                       searching etc
**/
UINT64
GetDescriptorHandle (
  IN DXIO_PORT_DESCRIPTOR   *InDescriptor
  )
{
  union {
    UINT8     BitArray[8];
    UINT64    Value;
  } Handle;

  Handle.Value = 0;
  Handle.BitArray[0] = InDescriptor->EngineData.DxioStartLane;
  Handle.BitArray[1] = InDescriptor->EngineData.DxioEndLane;
  Handle.BitArray[2] = InDescriptor->EngineData.EngineType;
  Handle.BitArray[3] = InDescriptor->EngineData.GpioGroupId;

  return Handle.Value;
}

/**
  CreateTopologyTable
    This function will create a topology table with X number of entries, using the default settings for all entries.
    All devices will be initially unused devices  i.e. they will be ignored when passed in to the DXIO init.

  @param SocketNumber      Socket number of topology entries to allocate
  @param NumberOfEntries   Number of topology entries to allocate.
                           Do not include table termination entry which is added automatically
  @param AllocateSuccess   Function was able to allocate table, where True/False is Success/Failure respectively.
                           Pointer is passed in.
  @param TablePointer      Pointer to the newly created table. Memory does not need to be allocated beforehand

  @return EFI_STATUS       Always returns EFI_SUCCESS
**/
EFI_STATUS
CreateTopologyTable (
  IN  UINTN                           SocketNumber,
  IN  UINTN                           NumberOfEntries,
  OUT BOOLEAN                         *AllocateSuccess,
  OUT DXIO_COMPLEX_DESCRIPTOR         **InTablePointer
  )
{
  UINTN                      Counter;
  DXIO_COMPLEX_DESCRIPTOR    *ComplexTable;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry, attempting to allocate table with %d entries\n", \
                                          __FUNCTION__, NumberOfEntries);
  // Simple Bounds checks on input parameters
  ASSERT (NumberOfEntries < 128);
  ASSERT (SocketNumber <= 1);

  // Increment NumberOfEntries by one to account for DESCRIPTOR_TERMINATE_LIST
  NumberOfEntries++;

  // Allocate memory for the table
  ComplexTable = (DXIO_COMPLEX_DESCRIPTOR *)AllocateZeroPool (sizeof(DXIO_COMPLEX_DESCRIPTOR));
  if (ComplexTable == NULL) {
    *AllocateSuccess = FALSE;
    return EFI_SUCCESS;
  }
  ComplexTable->PciePortList = (DXIO_PORT_DESCRIPTOR *)
                                        AllocateZeroPool (sizeof (DXIO_PORT_DESCRIPTOR) * NumberOfEntries);
  if (ComplexTable->PciePortList == NULL) {
    *AllocateSuccess = FALSE;
    return EFI_SUCCESS;
  }

  ComplexTable->SocketId = SocketNumber;
  ComplexTable->Flags = DESCRIPTOR_TERMINATE_LIST;

  // Initialize with a template (unused type)
  for (Counter = 0; Counter < NumberOfEntries; Counter++) {
    ComplexTable->PciePortList[Counter] = TemplateTopologyPort;
  }

  // Terminate list
  ComplexTable->PciePortList[NumberOfEntries - 1].Flags = DESCRIPTOR_TERMINATE_LIST;

  *AllocateSuccess = TRUE;
  *InTablePointer = ComplexTable;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit, Success. NumberOfEntries = %d\n", __FUNCTION__, NumberOfEntries);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
  InitializeTopologyEntry
    This function will initialize the first Unused entry using default values for the specified engine type

  @param TablePointer        Pointer to table containing entries to work with
  @param EngineType          Engine Type
  @param StartLane           Start lane number
  @param EndLane             End lane number
  @param ResetId             Reset ID number
  @param Device              Requested PCI Device (not applicable for non PCI devices)
  @param Function            Requested PCI Function (not applicable for non PCI devices)
  @param InitializeSuccess   Function was able to initialize entry, where True/False is Success/Failure respectively.
  @param EntryHandle         Unique handle ID for entry, pointer is passed in.

  @return EFI_STATUS         Always returns EFI_SUCCESS
**/
EFI_STATUS
InitializeTopologyEntry (
  IN  DXIO_COMPLEX_DESCRIPTOR         **InTablePointer,
  IN  UINT8                           EngineType,
  IN  UINT8                           StartLane,
  IN  UINT8                           EndLane,
  IN  UINT8                           ResetId,
  IN  UINT8                           Device,
  IN  UINT8                           Function,
  OUT BOOLEAN                         *InitializeSuccess,
  OUT UINT64                          *EntryHandle
  )
{
  DXIO_PORT_DESCRIPTOR       *DescriptorEntries;
  UINTN                      Counter;
  BOOLEAN                    FoundEntry;
  DXIO_COMPLEX_DESCRIPTOR    *ComplexTable;

  ComplexTable = *InTablePointer;

  // Checks on parameters
  ASSERT(ComplexTable != NULL);
  ASSERT (EngineType < DxioMaxPcieEngine);
  ASSERT (StartLane <= 129);
  ASSERT (EndLane <= 129);
  ASSERT (ResetId < 129);
  ASSERT (InitializeSuccess != NULL);

  DescriptorEntries = NULL;
  Counter = 0;
  FoundEntry = FALSE;

  // Make sure entries are available
  if (ComplexTable->PciePortList == NULL) {
    *InitializeSuccess = FALSE;
    return EFI_SUCCESS;
  }

  // Find an unused entry
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    // Entry is unused if it is of unused type and startlane and endlane are both 0
    if ((ComplexTable->PciePortList[Counter].EngineData.EngineType == DxioUnusedEngine) &&
        (ComplexTable->PciePortList[Counter].EngineData.StartLane == 0) &&
        (ComplexTable->PciePortList[Counter].EngineData.EndLane == 0)) {
      FoundEntry = TRUE;
      break;
    }
    Counter++;
  }

  // No entries available
  if (FoundEntry == FALSE) {
    *InitializeSuccess = FALSE;
    IDS_HDT_CONSOLE (GNB_TRACE, "%a No entries available\n", __FUNCTION__);
    return EFI_SUCCESS;
  }

  ComplexTable->PciePortList[Counter].EngineData.EngineType = EngineType;
  ComplexTable->PciePortList[Counter].EngineData.StartLane = StartLane;
  ComplexTable->PciePortList[Counter].EngineData.EndLane = EndLane;
  ComplexTable->PciePortList[Counter].EngineData.GpioGroupId = ResetId;
  // TODO : What else do we need to do per engine type.
  switch (EngineType) {
  case DxioUnusedEngine:
    break;
  case DxioPcieEngine:
    ComplexTable->PciePortList[Counter].Port.DeviceNumber = Device;
    ComplexTable->PciePortList[Counter].Port.FunctionNumber = Function;
    break;
  case DxioSATAEngine:
    break;
  case DxioUSB_OVER_PCIE:
    break;
  default:
    ASSERT(FALSE);
    break;
  }

  DxioUserDescriptorConfigDump(&(ComplexTable->PciePortList[Counter]));
  *EntryHandle = GetDescriptorHandle(&(ComplexTable->PciePortList[Counter]));
  *InitializeSuccess = TRUE;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit, Success\n", __FUNCTION__);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**

  AddParameter
    This function will add a parameter initializer to the generic parameter list of the specified topology entry

  @param TablePointer             PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI instance
                                  Pointer to table containing entries to work with
  @param EntryHandle              Unique handle ID associated with a given entry
  @param Parameter                Parameter to add to entry
  @param AddParamSuccess          Function was able to add parameter, where True/False is Success/Failure respectively.
                                  Pointer is passed in

  @return EFI_STATUS              Always returns EFI_SUCCESS
**/
EFI_STATUS
AddParameter (
  IN  DXIO_COMPLEX_DESCRIPTOR         **TablePointer,
  IN  UINT64                          EntryHandle,
  IN  PORT_PARAM                      Parameter,
  OUT BOOLEAN                         *AddParamSuccess
  )
{
  DXIO_COMPLEX_DESCRIPTOR    *ComplexTable;
  UINTN                      Counter;
  UINTN                      ParamIndex;
  BOOLEAN                    FoundEntry;

  // Checks on parameters
  ASSERT (*TablePointer != NULL);

  ComplexTable = *TablePointer;
  Counter = 0;
  FoundEntry = FALSE;

  // Make sure entries are available
  if (ComplexTable->PciePortList == NULL) {
    *AddParamSuccess = FALSE;
    return EFI_SUCCESS;
  }

  // Find entry associated with handle
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    if (GetDescriptorHandle(&(ComplexTable->PciePortList[Counter])) == EntryHandle) {
      FoundEntry = TRUE;
      break;
    }
    Counter++;
  }

  // No matching entry
  if (FoundEntry == FALSE) {
    *AddParamSuccess = FALSE;
    return EFI_SUCCESS;
  }

  // Find unused parameter entry
  for (ParamIndex = 0; ParamIndex < PCIE_PORT_PARAMETER_COUNT; ParamIndex++) {
    if (ComplexTable->PciePortList[Counter].PortParams.PhyParam[ParamIndex].ParamType == 0)
    {
      ComplexTable->PciePortList[Counter].PortParams.PhyParam[ParamIndex] = Parameter;
    }
  }

  *AddParamSuccess = TRUE;

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
  RemoveEntry
    This function will mark a previously created entry as unused and remove any parameters in the list.

  @param TablePointer             PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI instance
                                  Pointer to table containing entries to work with EntryHandle
  @param EntryHandle              Unique handle ID associated with a given entry
  @param RemoveEntrySuccess       Function was able to remove entry, where True/False is Success/Failure respectively.
                                  Pointer Is passed in

  @return EFI_STATUS              Always returns EFI_SUCCESS
**/
EFI_STATUS
RemoveEntry (
  IN  DXIO_COMPLEX_DESCRIPTOR         **TablePointer,
  IN  UINT64                          EntryHandle,
  OUT BOOLEAN                         *RemoveEntrySuccess
  )
{
  DXIO_COMPLEX_DESCRIPTOR    *ComplexTable;
  UINTN                      Counter;
  BOOLEAN                    FoundEntry;

  // Checks on parameters
  ASSERT (*TablePointer != NULL);

  ComplexTable = *TablePointer;
  Counter = 0;
  FoundEntry = FALSE;

  // Make sure entries are available
  if (ComplexTable->PciePortList == NULL) {
    *RemoveEntrySuccess = FALSE;
    return EFI_SUCCESS;
  }

  // Find entry associated with handle
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    if (GetDescriptorHandle(&(ComplexTable->PciePortList[Counter])) == EntryHandle) {
      FoundEntry = TRUE;
      break;
    }

    Counter++;
  }

  // No matching entry
  if (FoundEntry == FALSE) {
    *RemoveEntrySuccess = FALSE;
    return EFI_SUCCESS;
  }

  // Re copy empty entry template over disbanded entry
  ComplexTable->PciePortList[Counter] = TemplateTopologyPort;

  *RemoveEntrySuccess = TRUE;

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
  IncreaseTableSize
   This function wouldis used to grow the table beyond the initial allocation. New memory is allocated and the old
   table is copied over. Do not use on any memory not associated with the heap.

  @param TablePointer             PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI instance
                                  Pointer to table containing entries to work with
  @param NewSize                  New size of the table, in entries. Must be greater than the current size
  @param IncreaseSuccess          Returns false if there is an issue with the input parameters
                                  or the allocation process. True if success.

  @return EFI_STATUS              Always returns EFI_SUCCESS;
**/
EFI_STATUS
IncreaseTableSize (
  IN  DXIO_COMPLEX_DESCRIPTOR         **TablePointer,
  IN  UINTN                           NewSize,
  OUT BOOLEAN                         *IncreaseSuccess
  )
{
  DXIO_COMPLEX_DESCRIPTOR    *ComplexTable;
  UINTN                      Counter;
  UINTN                      NewListIndex;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);

  // Checks on parameters
  ASSERT (*TablePointer != NULL);
  ASSERT (IncreaseSuccess != NULL);
  ASSERT (NewSize < 256);

  ComplexTable = *TablePointer;
  Counter = 0;
  *IncreaseSuccess = FALSE;

  // Make sure entries are available
  if (ComplexTable->PciePortList == NULL) {
    *IncreaseSuccess = FALSE;
    return EFI_SUCCESS;
  }

  // Figure out how many entries are currently in the table, including terminator (1 based quantity)
  do {
    Counter++;
  } while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );

  // Account for terminator at the end in new list size
  Counter++;
  NewSize++;

  // Make sure table is actually requested to get larger
  if (NewSize < Counter) {
    *IncreaseSuccess = FALSE;
    IDS_HDT_CONSOLE (GNB_TRACE, "%a Requested table size is smaller than existing size.\n", __FUNCTION__);
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Reallocating from %d to %d entries\n", __FUNCTION__, Counter, NewSize);
  // Finally, allocate the memory
  ComplexTable->PciePortList = ReallocatePool (sizeof (DXIO_PORT_DESCRIPTOR) * Counter,
                                               sizeof (DXIO_PORT_DESCRIPTOR) * NewSize, ComplexTable->PciePortList);

  ComplexTable->PciePortList[Counter - 1].Flags &= ~DESCRIPTOR_TERMINATE_LIST;
  // Copy template entry to additional entries, overwritign terminator entry in initial list
  for (NewListIndex = Counter; NewListIndex < (NewSize - 1); NewListIndex++) {
    ComplexTable->PciePortList[NewListIndex] = TemplateTopologyPort;
  }

  // Program terminator entry in new list
  ComplexTable->PciePortList[NewSize - 1].Flags = DESCRIPTOR_TERMINATE_LIST;

  *IncreaseSuccess = TRUE;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}

///
/// PPI Initialization
///
PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI mAmdNbioPcieTopologyPpi = {
  AMD_NBIO_PCIE_TOPOLOGY_REVISION,
  CreateTopologyTable,
  InitializeTopologyEntry,
  AddParameter,
  RemoveEntry,
  IncreaseTableSize
  };




