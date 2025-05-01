/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/**
 *  @file MpioUbmTopology.c
 *  @brief UBM Interface Handler
 */

#include <AMD.h>
#include <Gnb.h>
#include <GnbDxio.h>
#include <PiPei.h>
#include <GnbDxio.h>
#include <AmdPcieComplex.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/GnbLib.h>
#include <MpioLib.h>
#include <Library/MpioInitLib.h>

//From AgesaPkg
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieTopologyPpi.h>

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOUBMTOPOLOGY_FILECODE
/*----------------------------------------------------------------------------------------*/
/**
  Count the number of socket entries so we allocate the right size of a table

  @param Socket0Entries   Pointer to variable to store socket 0 entries
  @param Entries          Pointer to array of structures containing topology data
**/

STATIC
UINTN
CountEntries (
  IN  DXIO_COMPLEX_DESCRIPTOR         *ComplexTable
  )
{
  UINTN        Counter;

  Counter = 0;
  // Figure out how many entries are currently in the table, including terminator (1 based quantity)
  do {
    Counter++;
  } while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );
  return ++Counter;
}

/**
 *  @brief UBM Device Discovery
 *
 *  @param [in] PeiServices Pointer to PEI Services
 *  @param [in] HfcDescriptor Pointer to an HFC Descriptor
 *  @param [in] HfcIndexList HFC Index List
 *  @param [in] HfcListSize HFC List Size
 *  @return Return description
 *
 *  @details More details
 */

VOID
HfcDescriptorDebugDump (
  IN     MPIO_UBM_HFC_DESCRIPTOR  *HfcDescriptor
  )
{
  UINT32                          HfcIndex;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);

  GnbLibDebugDumpBuffer ((VOID*) HfcDescriptor, 4, 1, 16);
  IDS_HDT_CONSOLE (GNB_TRACE, "\n");
  GnbLibDebugDumpBuffer ((VOID*) &(HfcDescriptor->HfcPortList), sizeof(MPIO_UBM_HFC_DESCRIPTOR) - 4, 1, sizeof(MPIO_HFC_PORT));

  for (HfcIndex = 0; HfcIndex < HfcDescriptor->Size; HfcIndex++) {
    IDS_HDT_CONSOLE (GNB_TRACE, "HFC Entry\n");
    IDS_HDT_CONSOLE (GNB_TRACE, "   StartLane:             %x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.Start_Lane);
    IDS_HDT_CONSOLE (GNB_TRACE, "   SlotNum:               %x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.SlotNum);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_GPIOx_TYPE:         %d\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.Type);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_GPIOx_ADDRESS:      0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.Addr);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_GPIOx_I2C_RESET:    %d\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.I2c_Reset);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_GPIOx_BP_TYPE:      %d\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.Bp_Type);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_UBM_SWITCH0_ADDR:   0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[0].Addr);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_UBM_SWITCH0_SELECT: 0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[0].Select);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_UBM_SWITCH1_ADDR:   0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[1].Addr);
    IDS_HDT_CONSOLE (GNB_TRACE, "   PP_UBM_SWITCH1_SELECT: 0x%x\n\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[1].Select);
  }

  return;
}


/*----------------------------------------------------------------------------------------*/
/**
 *  @brief Primary UBM discovery flow
 *
 *  @param [in] ComplexDescriptor Description for ComplexDescriptor
 *  @param [in] HfcDescriptor Description for HfcDescriptor
 *  @param [in] HfcStartLanes Description for HfcStartLanes
 *  @param [in] HfcListSize Description for HfcListSize
 *  @param [in] Pcie Description for Pcie
 *  @return Return description
 *
 *  @details More details
 */
VOID
MpioUbmDiscovery (
  IN     DXIO_COMPLEX_DESCRIPTOR   **ComplexDescriptor,
  IN     MPIO_UBM_HFC_DESCRIPTOR   *HfcDescriptor,
  IN     UINT8                     *HfcStartLanes,
  IN     UINT8                     HfcListSize,
  IN     PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  DXIO_COMPLEX_DESCRIPTOR         *ComplexTable;
  UINTN                           EntryCount;
  EFI_STATUS                      Status;
  BOOLEAN                         Result;
  UINT64                          EntryHandle;
  CONST EFI_PEI_SERVICES          **PeiServices;
  PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI  *TopologyProtocol;

  DFC_DESCRIPTOR                  *Dfc;

  UINT32                          HfcIndex;
  UINT32                          DfcIndex;
  UINT32                          DfcCount;
  PCI_ADDR                        PciAddress;
  UINT32                          Response;
  UINT32                          MpioArg[6];
  UINT8                           StartLane;
  UINT8                           EndLane;
  UINT8                           ResetId;
  PORT_PARAM                      PortParam;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);

  if ((HfcDescriptor == NULL) || (HfcStartLanes == NULL) || (Pcie == 0)) {
    return;
  }

  HfcDescriptorDebugDump (HfcDescriptor);

  PciAddress.AddressValue = 0;

  NbioMpioServiceCommonInitArguments (MpioArg);
  MpioArg[0] = (UINT32) &HfcDescriptor->HfcPortList[0];
  MpioArg[1] = (UINT32) HfcListSize;
  Response = MpioServiceRequest (PciAddress, BIOS_MPIO_MSG_I2C_ENUMERATE, MpioArg, 0);
  IDS_HDT_CONSOLE (GNB_TRACE,
    "  MPIO Response = 0x%x\n",
    Response);
  IDS_HDT_CONSOLE (GNB_TRACE,
    "  MPIO Args = 0x%x - 0x%x - 0x%x - 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3]);
  if ((Response & 0xFF) != BIOSSMC_Result_OK) {
    return;
  }

  // Get instance of Topology services PPI
  PeiServices = GetPeiServicesTablePointer ();
  Status = (**PeiServices).LocatePpi (
    PeiServices,
    &gAmdNbioPcieTopologyPpiGuid,
    0,
    NULL,
    (VOID **)&TopologyProtocol
    );
  if (Status != EFI_SUCCESS) {
    return;
  }

  ComplexTable = *ComplexDescriptor;
  Result = FALSE;
  EntryCount = CountEntries (ComplexTable);
  IDS_HDT_CONSOLE (GNB_TRACE, " Entry Count returned = %d\n", EntryCount);

  for (HfcIndex = 0; HfcIndex < HfcListSize; HfcIndex++) {
    DfcIndex = 0;
    DfcCount = 1;
    do {
      // Get DFC Entry for this HFC
      NbioMpioServiceCommonInitArguments (MpioArg);
      MpioArg[0] = HfcIndex;
      MpioArg[1] = DfcIndex;
      Response = MpioServiceRequest (PciAddress, BIOS_MPIO_MSG_I2C_DEVICE_GET, MpioArg, 0);
      IDS_HDT_CONSOLE (GNB_TRACE,
        "  MPIO Response = 0x%x\n",
        Response);
      IDS_HDT_CONSOLE (GNB_TRACE,
        "  MPIO Args = 0x%x - 0x%x - 0x%x - 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3]);
      if (((Response & 0xFF) == BIOSSMC_Result_OK) || ((Response & 0xFF) == 0)) {
        Dfc = (DFC_DESCRIPTOR *) &MpioArg[1];
        IDS_HDT_CONSOLE (GNB_TRACE, "  hfc_idx =    0x%x\n", Dfc->hfc_idx);
        IDS_HDT_CONSOLE (GNB_TRACE, "  num_dfcs =   0x%x\n", Dfc->num_dfcs);
        IDS_HDT_CONSOLE (GNB_TRACE, "  lane_start = 0x%x\n", Dfc->lane_start);
        IDS_HDT_CONSOLE (GNB_TRACE, "  lane_width = 0x%x\n", Dfc->lane_width);
        IDS_HDT_CONSOLE (GNB_TRACE, "  genspeed =   0x%x\n", Dfc->genspeed);
        IDS_HDT_CONSOLE (GNB_TRACE, "  state =      0x%x\n", Dfc->state);
        IDS_HDT_CONSOLE (GNB_TRACE, "  features =   0x%x\n", Dfc->features);
        IDS_HDT_CONSOLE (GNB_TRACE, "  slot =       0x%x\n", Dfc->slot);
        if (DfcIndex == 0) {
          DfcCount = Dfc->num_dfcs;
          IDS_HDT_CONSOLE (GNB_TRACE, "DfcCount is %d\n", DfcCount);
          // Add entries for this HFC
          EntryCount += Dfc->num_dfcs;
          IDS_HDT_CONSOLE (GNB_TRACE, "New EntryCount is %d\n", EntryCount);
          Status = TopologyProtocol->IncreaseTableSize (&ComplexTable, EntryCount, &Result);
          if ((Status != EFI_SUCCESS) || (Result == FALSE)) {
            IDS_HDT_CONSOLE (GNB_TRACE, " IncreaseTableSize Returned ERROR!!!\n", __FUNCTION__);
            return;
          }  else {
            IDS_HDT_CONSOLE (GNB_TRACE, "IncreaseTableSize SUCCESS!\n", DfcCount);
          }

        }
        // Add entry for this DFC
        StartLane = (HfcStartLanes[HfcIndex]) + (Dfc->lane_start);
        //StartLane = Dfc->lane_start;
        EndLane = StartLane + (Dfc->lane_width) - 1;
        ResetId = 0;//TBD
        IDS_HDT_CONSOLE (GNB_TRACE, " Calling InitializeTopologyEntry for DfcIndex = \n", DfcIndex);
        if (Dfc->state.Field.type == BIOS_DFC_INFO_TYPE_SATA_SAS) {
          Status = TopologyProtocol->InitializeTopologyEntry (
            &ComplexTable,
            DxioSATAEngine,
            StartLane,
            EndLane,
            ResetId,
            0,
            0,
            &Result,
            &EntryHandle);
        } else {
          if ((Dfc->state.Field.type == BIOS_DFC_INFO_TYPE_QUAD_PCI) ||
             (Dfc->state.Field.type == BIOS_DFC_INFO_TYPE_EMPTY)) {
            Status = TopologyProtocol->InitializeTopologyEntry (
              &ComplexTable,
              DxioPcieEngine,
              StartLane,
              EndLane,
              ResetId,
              0,
              0,
              &Result,
              &EntryHandle);
          }
        }
        if ((Status != EFI_SUCCESS) || (Result == FALSE)) {
          IDS_HDT_CONSOLE (GNB_TRACE, " InitializeTopologyEntry Returned ERROR!!!\n", __FUNCTION__);
          return;
        }
        if (Dfc->state.Field.type == BIOS_DFC_INFO_TYPE_EMPTY) {
          PortParam.ParamType = PP_PORT_PRESENT;
          PortParam.ParamValue = DxioEndpointNotPresent;
          Status = TopologyProtocol->AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
        }
        // Add parameters to Topology entry
        // Status = TopologyProtocol->AddParameter (&ComplexTable, EntryHandle, PP_, &Result);
        if ((Dfc->state.Field.type == BIOS_DFC_INFO_TYPE_QUAD_PCI) ||
            (Dfc->state.Field.type == BIOS_DFC_INFO_TYPE_EMPTY)) {
              PortParam.ParamType = PP_HOTPLUG_TYPE;
              PortParam.ParamValue = DxioHotplugUBM;
              Status = TopologyProtocol->AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = PP_UBM_HFC_INDEX;
              PortParam.ParamValue = Dfc->hfc_idx;
              Status = TopologyProtocol->AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = PP_UBM_DFC_INDEX;
              PortParam.ParamValue = (UINT16) DfcIndex;
              Status = TopologyProtocol->AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = PP_SLOT_NUM;
              PortParam.ParamValue = (UINT16) Dfc->slot;
              Status = TopologyProtocol->AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
        }
        DfcIndex++;
      }
    } while (DfcIndex < DfcCount);
  }
  // Update address of Complex to caller
  *ComplexDescriptor = ComplexTable;
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to user engine descriptor
 *
 *
 * @param[in]  EngineDescriptor   Pointer to engine descriptor
 */
VOID
AddHfcDescriptor (
  IN      DXIO_PORT_DESCRIPTOR      *EngineDescriptor,
  IN OUT  MPIO_UBM_HFC_DESCRIPTOR   *HfcDescriptor,
  IN OUT  UINT8                     *HfcStartLanes,
  IN OUT  UINT8                     *HfcListSize
  )
{
  PORT_PARAM      *PortParam;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  // Verify input parameters
  if ((HfcDescriptor == NULL) || (HfcStartLanes == NULL) || (HfcListSize == NULL)) {
    return;
  }

  HfcDescriptor->Size += sizeof (MPIO_HFC_PORT);
  HfcStartLanes[*HfcListSize] = EngineDescriptor->EngineData.StartLane;
  HfcDescriptor->HfcPortList[*HfcListSize].NodeType = MPIO_I2C_NODE_TYPE_UBM;
  HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Addr = UBM_INVALID_SWITCH_ADDR;
  HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Addr = UBM_INVALID_SWITCH_ADDR;
  HfcDescriptor->HfcPortList[*HfcListSize].Expander.Addr = UBM_INVALID_SWITCH_ADDR;
  HfcDescriptor->HfcPortList[*HfcListSize].Expander.Bp_Type = UBM_GPIO_NOT_USED;
  HfcDescriptor->HfcPortList[*HfcListSize].Expander.I2c_Reset = UBM_GPIO_NOT_USED;

  IDS_HDT_CONSOLE (GNB_TRACE, "Adding this entry\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "  HfcListSize = 0x%x\n", *HfcListSize);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StartLane   = 0x%x\n", HfcStartLanes[*HfcListSize]);

  PortParam = (PORT_PARAM *) &(EngineDescriptor->PortParams);
  // Process port parameters until a an empty is found
  while (PortParam->ParamType != 0) {
    switch (PortParam->ParamType) {
    case PP_I2C_EXPANDER_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.Type = (UINT8) (PortParam->ParamValue);
      break;
    case PP_I2C_EXPANDER_ADDRESS:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.Addr = (UINT8) (PortParam->ParamValue);
      break;
    case PP_GPIOx_BP_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.Bp_Type = (UINT8) (PortParam->ParamValue);
      break;
    case PP_GPIOx_I2C_RESET:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.I2c_Reset = (UINT8) (PortParam->ParamValue);
      break;
    case PP_START_LANE:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.Start_Lane = (UINT8) (PortParam->ParamValue);
      break;
    case PP_SLOT_NUM:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.SlotNum = (UINT8) (PortParam->ParamValue);
      break;
    case PP_UBM_SWITCH0_ADDR:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Addr = (UINT8) (PortParam->ParamValue);
      break;
    case PP_UBM_SWITCH0_SELECT:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Select = (UINT8) 1 << (PortParam->ParamValue);
      break;
    case PP_UBM_SWITCH0_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Type = (UINT8) (PortParam->ParamValue);
      break;
    case PP_UBM_SWITCH1_ADDR:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Addr = (UINT8) (PortParam->ParamValue);
      break;
    case PP_UBM_SWITCH1_SELECT:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Select = (UINT8) 1 << (PortParam->ParamValue);
      break;
    case PP_UBM_SWITCH1_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Type = (UINT8) (PortParam->ParamValue);
      break;
    default:
      break;
    }
    PortParam++;
  }
  (*HfcListSize)++;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback for Topology Management
 *
 *
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 */

VOID
ParseTopologyForUbm (
  IN      DXIO_COMPLEX_DESCRIPTOR     **ComplexDescriptor,
  IN      PCIe_PLATFORM_CONFIG        *Pcie
  )
{
  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;
  UINTN                     Index;
  DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology;
  MPIO_UBM_HFC_DESCRIPTOR   *HfcDescriptor;
  UINT8                     HfcStartLanes[32];
  UINT8                     HfcListSize;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  PlatformTopology = *ComplexDescriptor;
  Index = 0;
  while (PlatformTopology != NULL) {
    EngineDescriptor = PlatformTopology->PciePortList;
    HfcDescriptor = NULL;
    HfcListSize = 0;
    while (EngineDescriptor != NULL) {
      if (EngineDescriptor->EngineData.EngineType == DxioUBMHFCEngine) {
          // Allocate HFC_DESCRIPTOR_LIST on first entry
        if (HfcDescriptor == NULL) {
          IDS_HDT_CONSOLE (GNB_TRACE, "First entry found, allocating HFC_DESCRIPTOR_LIST\n");
          HfcDescriptor = (MPIO_UBM_HFC_DESCRIPTOR*) AllocateZeroPool (sizeof(MPIO_UBM_HFC_DESCRIPTOR));
          if (HfcDescriptor == NULL) {
            return;  //@TODO return error?
          }
          HfcDescriptor->Size = offsetof (MPIO_UBM_HFC_DESCRIPTOR, HfcPortList);
        }
        IDS_HDT_CONSOLE (GNB_TRACE, "Adding descriptor to list\n");
        AddHfcDescriptor (EngineDescriptor, HfcDescriptor, &HfcStartLanes[0], &HfcListSize);
        EngineDescriptor->EngineData.EngineType = DxioUnusedEngine;
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    if ((HfcDescriptor != NULL) && (HfcListSize != 0)) {
      // Send HfcDescriptor to MPIO and get DFC topology back
      HfcDescriptor->Size = (UINT32) HfcListSize;
      MpioUbmDiscovery (ComplexDescriptor, HfcDescriptor, HfcStartLanes, HfcListSize, Pcie);
    }
    PlatformTopology = PcieConfigGetNextDataDescriptor (PlatformTopology);
    Index++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}


