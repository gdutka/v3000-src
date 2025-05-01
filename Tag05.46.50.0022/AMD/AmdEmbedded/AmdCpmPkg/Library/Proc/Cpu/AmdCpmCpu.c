/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmBase.h>
#include <OptionCpuInstall.h>
#include CPM_PPI_DEFINITION (AmdCpmTablePpi)
#include CPM_INDUSTRYSTANDARD_DEFINITION (Pci22)

#define BIXBY_DID     0x57A01022
#define SWUS_DID      0x148D1022

#define PT_XHCI_ID_A 0x43B91022
#define PT_XHCI_ID_B 0x43BA1022
#define PT_XHCI_ID_C 0x43BB1022
#define PT_XHCI_ID_D 0x43BC1022
#define PT_XHCI_ID_5 0x43D01022
#define PT_XHCI_ID_6 0x43D11022
#define PT_XHCI_ID_7 0x43D21022
#define PT_XHCI_ID_8 0x43D51022
#define PT_XHCI_ID_9 0x43D81022
#define PT_XHCI_ID_10 0x43D31022
#define PT_XHCI_ID_26B 0x43D61022

#define PROM19_XHCI_A 0x43EC1022
#define PROM19_XHCI_B 0x43ED1022
#define PROM19_XHCI_C 0x43EE1022
#define PROM19_XHCI_D 0x43EF1022
#define PROM19_XHCI_E 0x43F01022
#define PROM19_XHCI_F 0x43F11022
#define PROM19_XHCI_G 0x43F21022
#define PROM19_XHCI_H 0x43F31022

CPU_REVISION_ITEM *CpuRevisionTable[] = {
  OPTION_CPM_CPU_REVISION_ID_ON
  OPTION_CPM_CPU_REVISION_ID_TN
  OPTION_CPM_CPU_REVISION_ID_KV
  OPTION_CPM_CPU_REVISION_ID_KB
  OPTION_CPM_CPU_REVISION_ID_ML
  OPTION_CPM_CPU_REVISION_ID_CZ
  OPTION_CPM_CPU_REVISION_ID_NL
  OPTION_CPM_CPU_REVISION_ID_AM
  OPTION_CPM_CPU_REVISION_ID_ST
  OPTION_CPM_CPU_REVISION_ID_BR
  OPTION_CPM_CPU_REVISION_ID_ZP
  OPTION_CPM_CPU_REVISION_ID_RV
  OPTION_CPM_CPU_REVISION_ID_RV2
  OPTION_CPM_CPU_REVISION_ID_SSP
  OPTION_CPM_CPU_REVISION_ID_MTS
  OPTION_CPM_CPU_REVISION_ID_RN
  OPTION_CPM_CPU_REVISION_ID_VN
  OPTION_CPM_CPU_REVISION_ID_RMB
  OPTION_CPM_CPU_REVISION_ID_RPL
  OPTION_CPM_CPU_REVISION_ID_STP
  NULL
};

PCIE_BRIDGE_NAME *PcieBridgeName[] = {
  OPTION_CPM_PCIE_BRIDGE_NAME_ON
  OPTION_CPM_PCIE_BRIDGE_NAME_TN
  OPTION_CPM_PCIE_BRIDGE_NAME_KV
  OPTION_CPM_PCIE_BRIDGE_NAME_KB
  OPTION_CPM_PCIE_BRIDGE_NAME_ML
  OPTION_CPM_PCIE_BRIDGE_NAME_CZ
  OPTION_CPM_PCIE_BRIDGE_NAME_NL
  OPTION_CPM_PCIE_BRIDGE_NAME_AM
  OPTION_CPM_PCIE_BRIDGE_NAME_ST
  OPTION_CPM_PCIE_BRIDGE_NAME_BR
  OPTION_CPM_PCIE_BRIDGE_NAME_ZP
  OPTION_CPM_PCIE_BRIDGE_NAME_RV
  OPTION_CPM_PCIE_BRIDGE_NAME_RV2
  OPTION_CPM_PCIE_BRIDGE_NAME_SSP
  OPTION_CPM_PCIE_BRIDGE_NAME_MTS
  OPTION_CPM_PCIE_BRIDGE_NAME_RN
  OPTION_CPM_PCIE_BRIDGE_NAME_VN
  OPTION_CPM_PCIE_BRIDGE_NAME_RMB
  OPTION_CPM_PCIE_BRIDGE_NAME_RPL
  OPTION_CPM_PCIE_BRIDGE_NAME_STP
  NULL
};

UINT8 AmdCpmSsdtSupport = AMD_CPM_SSDT_SUPPORT;


/*---------------------------------------------------------------------------------------*/
/**
 * Check RV OPN number
 *
 * @retval    TRUE    This is RV B4 part
 *            FALSE   This is not RV B4 part
 *
 */
BOOLEAN
CpmCheckRvOpnB4 (
  IN       VOID                        *This
  )
{
  UINT32              TempData32;
  UINT32              PcdGetRVFamilyOPN;
  AMD_CPM_MAIN_TABLE  *MainTablePtr;

  MainTablePtr = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  PcdGetRVFamilyOPN = MainTablePtr->PcdGetRVFamilyOPN;
  TempData32 =  (PcdGetRVFamilyOPN >> 30) & 0x3;

  if (TempData32 == 2) {
    return TRUE;
  } else if (TempData32 == 3) {
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get SBI (Sideband Interface) Address in PCI MMIO Base
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    SBI Address
 *
 */
UINT32
CpmGetSbTsiAddr (
  IN       VOID                        *This
  )
{
  AMD_CPM_MAIN_TABLE  *MainTablePtr;
  MainTablePtr = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  return (MainTablePtr->PcieMemIoBaseAddr + 0x000C31E8);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check whether thermal function is enabled in current CPU or APU
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    TRUE    Both of ThermtpEn and HtcCapable are enabled
 *            FALSE   Either of ThermtpEn or HtcCapable is not enabled
 *
 */
BOOLEAN
CpmIsThermalSupport (
  IN       VOID                        *This
  )
{
  AMD_CPM_TABLE_PPI   *CpmTablePpiPtr = This;
  UINT32              ThermtpEn;
  UINT32              HtcCapable;

  ThermtpEn   = CpmTablePpiPtr->CommonFunction.PciRead32 (CpmTablePpiPtr, 0, 0x18, 3, 0xE4) & BIT5;      // D18F3xE4 Thermtrip Status [5]: ThermtpEn
  HtcCapable  = CpmTablePpiPtr->CommonFunction.PciRead32 (CpmTablePpiPtr, 0, 0x18, 3, 0xE8) & BIT10;     // D18F3xE8 Northbridge Capabilities [10]: HtcCapable

  return (BOOLEAN) ((ThermtpEn != 0) && (HtcCapable != 0));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check whether thermal function is enabled in current CPU or APU
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The CPU Revision Item for current platform
 *
 */
CPU_REVISION_ITEM*
CpmGetCpuRevisionItem (
  IN       VOID           *This
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  CPU_REVISION_ITEM       **CpuRevisionPtr;
  CPU_REVISION_ITEM       *CpuRevisionItemPtr;
  CPU_REVISION_ITEM       *Item;
  UINT32                  Value;
  UINT32                  ApuSupportMask;

  ApuSupportMask = PcdGet32 (ApuSupportMask);
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  CpuRevisionPtr = &CpuRevisionTable[0];
  CommonFunctionPtr->CpuidRead(This, &Value, NULL);             // AMD_CPUID_FMF
  if ((Value & 0x0FFF0F00) == 0x00810F00) {                     // CPM_CPU_REVISION_ID_RV
    if (CpmCheckRvOpnB4(This)) {
      Value = 0x00820F00;                                       // Treat it as RV2
    }
  }
  Item = 0;
  while (*CpuRevisionPtr && !Item) {
    CpuRevisionItemPtr = *CpuRevisionPtr;
    while (CpuRevisionItemPtr->Mask) {
      if (!(ApuSupportMask & (1 << CpuRevisionItemPtr->CpuRevision))) {
        break;
      }
      if ((CpuRevisionItemPtr->Mask & Value) == CpuRevisionItemPtr->Value) {
        Item = CpuRevisionItemPtr;
        break;
      }
      CpuRevisionItemPtr ++;
    }
    CpuRevisionPtr ++;
  }
  return Item;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Current PCIe Bridge Name Table
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The PCIe Bridge Name Table
 *
 */
PCIE_BRIDGE_NAME*
CpmGetPcieBridgeNameTable (
  IN       VOID           *This
  )
{
  PCIE_BRIDGE_NAME **PcieNameTablePtr;
  PCIE_BRIDGE_NAME *PcieNameItemPtr;
  CPU_REVISION_ITEM *ItemPtr;
  UINT8 PcieBridgeType;
  PcieBridgeType = 0;
  ItemPtr = CpmGetCpuRevisionItem (This);
  if (ItemPtr) {
    PcieBridgeType = ItemPtr->PcieRevision;
  }

  PcieNameTablePtr = &PcieBridgeName[0];
  PcieNameItemPtr = 0;
  while (*PcieNameTablePtr) {
    if ((*PcieNameTablePtr)->PcieRevision == PcieBridgeType) {
      PcieNameItemPtr = *PcieNameTablePtr;
      break;
    }
    PcieNameTablePtr ++;
  }
  return PcieNameItemPtr;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Promontory Pcie Bridge Bus/Device/Function Address
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval              The Promontory Pcie Bridge Bus/Device/Function Address
 * @retval    0x0       The is no Promontory
 *
 */
UINT32
CpmGetPtBrDevAddr (
  IN       VOID                       *This
  )
{
  AMD_CPM_TABLE_PPI                   *CpmTablePpiPtr = This;
  AMD_CPM_CORE_TOPOLOGY_TABLE         *AmdCpmCoreTopologyPtr;
  AMD_CPM_CORE_TOPOLOGY_LIST          *TablePtr;
  PCIE_BRIDGE_NAME                    *BridgeNamePtr;
  UINT32                              DeviceID;
  UINT32                              BridgeAddress;

  AmdCpmCoreTopologyPtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (This, CPM_SIGNATURE_CORE_TOPOLOGY);
  DeviceID = 0;
  BridgeAddress = 0;
  BridgeNamePtr = CpmGetPcieBridgeNameTable (This);

  if (AmdCpmCoreTopologyPtr) {
    while (BridgeNamePtr->Device != 0) {
      TablePtr = &AmdCpmCoreTopologyPtr->CoreList[0];
      while (TablePtr->Socket != CPM_END_OF_TABLE) {
        DeviceID = CpmTablePpiPtr->CommonFunction.GetPcieEndPointDeviceID (CpmTablePpiPtr, TablePtr->Bus, BridgeNamePtr->Device, BridgeNamePtr->Function);
        if (DeviceID == PT_XHCI_ID_A || DeviceID == PT_XHCI_ID_B || DeviceID == PT_XHCI_ID_C || DeviceID == PT_XHCI_ID_D || \
            DeviceID == PT_XHCI_ID_5 || DeviceID == PT_XHCI_ID_6 || DeviceID == PT_XHCI_ID_7 || DeviceID == PT_XHCI_ID_8 || \
            DeviceID == PT_XHCI_ID_9 || DeviceID == PT_XHCI_ID_10 || DeviceID == PT_XHCI_ID_26B || \
            DeviceID == PROM19_XHCI_A || DeviceID == PROM19_XHCI_B || DeviceID == PROM19_XHCI_C || DeviceID == PROM19_XHCI_D || \
            DeviceID == PROM19_XHCI_E || DeviceID == PROM19_XHCI_F || DeviceID == PROM19_XHCI_G || DeviceID == PROM19_XHCI_H) {
          BridgeAddress = CpmTablePpiPtr->MainTablePtr->PcieMemIoBaseAddr + (TablePtr->Bus << 20) + (BridgeNamePtr->Device << 15) + (BridgeNamePtr->Function << 12);
          DEBUG((DEBUG_INFO, "  LIB-CPU-%a-1-OK-BridgeAddress = 0x%08X\n", __FUNCTION__, BridgeAddress));
          return BridgeAddress;
        }
        TablePtr++;
      }
      BridgeNamePtr ++;
    }
  } else {
    while (BridgeNamePtr->Device != 0) {
      DeviceID = CpmTablePpiPtr->CommonFunction.GetPcieEndPointDeviceID (CpmTablePpiPtr, 0, BridgeNamePtr->Device, BridgeNamePtr->Function);
      if (DeviceID == PT_XHCI_ID_A || DeviceID == PT_XHCI_ID_B || DeviceID == PT_XHCI_ID_C || DeviceID == PT_XHCI_ID_D || \
          DeviceID == PT_XHCI_ID_5 || DeviceID == PT_XHCI_ID_6 || DeviceID == PT_XHCI_ID_7 || DeviceID == PT_XHCI_ID_8 || \
          DeviceID == PT_XHCI_ID_9 || DeviceID == PT_XHCI_ID_10 || DeviceID == PT_XHCI_ID_26B || \
          DeviceID == PROM19_XHCI_A || DeviceID == PROM19_XHCI_B || DeviceID == PROM19_XHCI_C || DeviceID == PROM19_XHCI_D || \
          DeviceID == PROM19_XHCI_E || DeviceID == PROM19_XHCI_F || DeviceID == PROM19_XHCI_G || DeviceID == PROM19_XHCI_H) {
        BridgeAddress = CpmTablePpiPtr->MainTablePtr->PcieMemIoBaseAddr + (BridgeNamePtr->Device << 15) + (BridgeNamePtr->Function << 12);
        DEBUG((DEBUG_INFO, "  LIB-CPU-%a-2-OK-BridgeAddress = 0x%08X\n", __FUNCTION__, BridgeAddress));
        return BridgeAddress;
      }
      BridgeNamePtr ++;
    }
  }
  DEBUG((DEBUG_INFO, "  LIB-CPU-%a-3-FAIL-BridgeAddress = 0x%08X\n", __FUNCTION__, BridgeAddress));
  return BridgeAddress;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Clear Pcie Bridge Bus Number
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Bus:      Bus Number
 *
 */
VOID
CpmClearBrBusNumber (
  IN       VOID                       *This,
  IN       UINT8                      Bus
  )
{
  AMD_CPM_TABLE_PPI                   *CpmTablePpiPtr = This;
  AMD_CPM_CORE_TOPOLOGY_TABLE         *AmdCpmCoreTopologyPtr;
  AMD_CPM_CORE_TOPOLOGY_LIST          *TablePtr;
  PCIE_BRIDGE_NAME                    *BridgeNamePtr;
  UINT32                              Data32;

  AmdCpmCoreTopologyPtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (This, CPM_SIGNATURE_CORE_TOPOLOGY);
  TablePtr = &AmdCpmCoreTopologyPtr->CoreList[0];

  if (AmdCpmCoreTopologyPtr) {
    while (TablePtr->Socket != CPM_END_OF_TABLE) {
      if (TablePtr->Bus == Bus) {
        BridgeNamePtr = CpmGetPcieBridgeNameTable (This);
        while (BridgeNamePtr->Device != 0) {
          Data32 = CpmTablePpiPtr->CommonFunction.PciRead32 (This, TablePtr->Bus, BridgeNamePtr->Device, BridgeNamePtr->Function, 0x18);
          if ((Data32 != 0) && (Data32 != 0xFFFFFFFF)) {
            CpmTablePpiPtr->CommonFunction.PciWrite32 (This, TablePtr->Bus, BridgeNamePtr->Device, BridgeNamePtr->Function, 0x18, 0);
          }
          BridgeNamePtr ++;
        }
      }
      TablePtr++;
    }
  }

  DEBUG((DEBUG_INFO, "  LIB-CPU-%a-End\n", __FUNCTION__));
  return;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Bixby Upstream Bridge Bus Number
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket:   Socket Number
 * @param[in] Die:      Die Number
 * @param[in] Bridge:   Bridge Number
 *
 * @retval              Bus Number
 *
 */
UINT8
CpmGetBxbBusNumber (
  IN       VOID                       *This
  )
{
  AMD_CPM_TABLE_PPI                   *CpmTablePpiPtr = This;
  PCIE_BRIDGE_NAME                    *BridgeNamePtr;
  UINT32                              Value;
  UINT32                              Address;
  UINT32                              BridgeAddress;
  UINT32                              TempBus;
  UINT8                               Bus;
  UINT8                               ChildBus;

  Bus = CpmTablePpiPtr->MainTablePtr->BxbBusNumDetEn;
  ChildBus = 0;
  Address = 0;
  Value = 0;

  if (Bus == 0) {
    DEBUG((DEBUG_INFO, "  LIB-CPU-%a-1-FAIL-Bus = 0xFF\n", __FUNCTION__));
    return 0xFF;
  }

  if (Bus != 0xFF) {
    Address = PcieAddress(CpmTablePpiPtr, Bus, 0, 0, PCI_VENDOR_ID_OFFSET);       // 0x0000 - Device/Vendor ID
    Value = CpmTablePpiPtr->CommonFunction.MmioRead32 (Address);
    if (((Value & 0xFFF0FFFF) == BIXBY_DID) || (Value == SWUS_DID)) {
      DEBUG((DEBUG_INFO, "  LIB-CPU-%a-2-OK-Bus = 0x%02X\n", __FUNCTION__, Bus));
      return Bus;
    }
  }

  BridgeAddress = CpmTablePpiPtr->MainTablePtr->BxbBrMmioBaseAddr;
  if (BridgeAddress == 0) {
    DEBUG((DEBUG_INFO, "  LIB-CPU-%a-3-FAIL-Bus = 0xFF\n", __FUNCTION__));
    return 0xFF;
  }

  TempBus = CpmTablePpiPtr->CommonFunction.MmioRead32 (BridgeAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);         // 0x0018 - Primary Bus Number
  if ((TempBus != 0xFFFFFFFF) && ((TempBus & 0xFFFFFF) != 0)) {
    Bus = (UINT8) ((TempBus >> 8) & 0xFF);
    Address = PcieAddress(CpmTablePpiPtr, Bus, 0, 0, PCI_VENDOR_ID_OFFSET);                                             // 0x0000 - Device/Vendor ID
    Value = CpmTablePpiPtr->CommonFunction.MmioRead32 (Address);
    if (((Value & 0xFFF0FFFF) == BIXBY_DID) || (Value == SWUS_DID)) {
      DEBUG((DEBUG_INFO, "  LIB-CPU-%a-4-OK-Bus = 0x%02X\n", __FUNCTION__, Bus));
      CpmTablePpiPtr->MainTablePtr->BxbBusNumDetEn = Bus;
      DEBUG((DEBUG_INFO, "  LIB-CPU-%a-4-OK-Update BxbBusNumDetEn = 0x%02X\n", __FUNCTION__, Bus));
      return Bus;
    } else {
      // Existing bus in configuration space can't use any more, clear it and re-assign next
      TempBus &= 0xFF000000;
      CpmTablePpiPtr->CommonFunction.MmioWrite32(BridgeAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, TempBus);       // 0x0018 - Primary Bus Number
      CpmTablePpiPtr->MainTablePtr->BxbOriginalBusNum = 0;
    }
  }

  if ((TempBus & 0xFFFFFF) == 0) {
    CpmTablePpiPtr->MainTablePtr->BxbOriginalBusNum = (TempBus & 0xFFFFFF) + 0xAA000000;
    Bus = (UINT8) ((BridgeAddress >> 20) & 0xFF);
    TempBus &= 0xFF000000;
    TempBus |= ((UINT32) (Bus + 1) << 16) + ((UINT32) (Bus + 1) << 8) + (UINT32) Bus;
    CpmTablePpiPtr->CommonFunction.MmioWrite32 (BridgeAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, TempBus);       // 0x0018 - Primary Bus Number
    Bus++;
    Address = PcieAddress(CpmTablePpiPtr, Bus, 0, 0, PCI_VENDOR_ID_OFFSET);                                             // 0x0000 - Device/Vendor ID
    Value = CpmTablePpiPtr->CommonFunction.MmioRead32 (Address);
    if (((Value & 0xFFF0FFFF) == BIXBY_DID) || (Value == SWUS_DID)) {
      DEBUG((DEBUG_INFO, "  LIB-CPU-%a-5-OK-Assign Temp Bus = 0x%02X\n", __FUNCTION__, Bus));
      return Bus;
    }
    TempBus &= 0xFF000000;
    CpmTablePpiPtr->CommonFunction.MmioWrite32 (BridgeAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, TempBus);       // 0x0018 - Primary Bus Number
    CpmTablePpiPtr->MainTablePtr->BxbOriginalBusNum = 0;
  }

  if (CpmTablePpiPtr->MainTablePtr->BxbBusNumDetEn != 0xFF) {
    Bus = (UINT8) ((BridgeAddress >> 20) & 0xFF);
    BridgeNamePtr = CpmGetPcieBridgeNameTable (This);
    while (BridgeNamePtr->Device != 0) {
      Address = PcieAddress(CpmTablePpiPtr, Bus, BridgeNamePtr->Device, BridgeNamePtr->Function, PCI_VENDOR_ID_OFFSET);   // 0x0000 - Device/Vendor ID
      Value = CpmTablePpiPtr->CommonFunction.MmioRead32 (Address);
      if ((Value != 0xFFFFFFFF) && (Value != 0)) {
        TempBus = CpmTablePpiPtr->CommonFunction.MmioRead32 (Address + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);           // 0x0018 - Primary Bus Number
        if ((TempBus & 0xFFFFFF) != 0) {
          ChildBus = (UINT8) ((TempBus >> 8) & 0xFF);
          BridgeAddress = PcieAddress(CpmTablePpiPtr, ChildBus, 0, 0, PCI_VENDOR_ID_OFFSET);                              // 0x0000 - Device/Vendor ID
          Value = CpmTablePpiPtr->CommonFunction.MmioRead32 (BridgeAddress);
          if (((Value & 0xFFF0FFFF) == BIXBY_DID) || (Value == SWUS_DID)) {
            CpmTablePpiPtr->MainTablePtr->BxbBusNumDetEn = ChildBus;
            CpmTablePpiPtr->MainTablePtr->BxbBrMmioBaseAddr = Address;
            DEBUG((DEBUG_INFO, "  LIB-CPU-%a-6-OK-Bus = 0x%02X\n", __FUNCTION__, ChildBus));
            DEBUG((DEBUG_INFO, "  LIB-CPU-%a-6-OK-Update BxbBusNumDetEn = 0x%02X\n", __FUNCTION__, ChildBus));
            DEBUG((DEBUG_INFO, "  LIB-CPU-%a-6-OK-Update BxbBrMmioBaseAddr = 0x%08X\n", __FUNCTION__, Address));
            return ChildBus;
          }
        } else {
          CpmTablePpiPtr->MainTablePtr->BxbOriginalBusNum = (TempBus & 0xFFFFFF) + 0xAA000000;
          TempBus &= 0xFF000000;
          TempBus |= ((UINT32) (Bus + 1) << 16) + ((UINT32) (Bus + 1) << 8) + (UINT32) Bus;
          CpmTablePpiPtr->CommonFunction.MmioWrite32 (Address + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, TempBus);         // 0x0018 - Primary Bus Number
          ChildBus = Bus + 1;
          BridgeAddress = PcieAddress(CpmTablePpiPtr, ChildBus, 0, 0, PCI_VENDOR_ID_OFFSET);                              // 0x0000 - Device/Vendor ID
          Value = CpmTablePpiPtr->CommonFunction.MmioRead32 (BridgeAddress);
          if (((Value & 0xFFF0FFFF) == BIXBY_DID) || (Value == SWUS_DID)) {
            CpmTablePpiPtr->MainTablePtr->BxbBrMmioBaseAddr = Address;
            DEBUG((DEBUG_INFO, "  LIB-CPU-%a-7-OK-Assign Temp Bus = 0x%02X\n", __FUNCTION__, ChildBus));
            DEBUG((DEBUG_INFO, "  LIB-CPU-%a-7-OK-Update BxbBrMmioBaseAddr = 0x%08X\n", __FUNCTION__, Address));
            return ChildBus;
          }
          TempBus &= 0xFF000000;
          CpmTablePpiPtr->CommonFunction.MmioWrite32 (Address + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, TempBus);         // 0x0018 - Primary Bus Number
          CpmTablePpiPtr->MainTablePtr->BxbOriginalBusNum = 0;
        } // End of if ((TempBus & 0xFFFFFF) != 0)
      } // End of if ((Value != 0xFFFFFFFF) && (Value != 0))
      BridgeNamePtr ++;
    } // End of while (BridgeNamePtr->Device != 0)
  } // End of if (CpmTablePpiPtr->MainTablePtr->BxbBusNumDetEn != 0xFF)
  DEBUG((DEBUG_INFO, "  LIB-CPU-%a-8-FAIL-Bus = 0xFF\n", __FUNCTION__));
  return 0xFF;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get PCIe Bridge Name in ASL code
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Device    The PCI Device Number of PCIe Bridge
 * @param[in] Function  The PCI Function Number of PCIe Bridge
 *
 * @retval    The PCIe Bridge Name in ASL code
 *
 */
UINT32
CpmGetPcieAslName (
  IN       VOID                       *This,
  IN       UINT8                      Device,
  IN       UINT8                      Function
  )
{
  PCIE_BRIDGE_NAME  *BridgeNamePtr;
  UINT32  Name;

  Name = 0;
  BridgeNamePtr = CpmGetPcieBridgeNameTable (This);
  while (BridgeNamePtr->Device != 0) {
    if (BridgeNamePtr->Device == Device && BridgeNamePtr->Function == Function) {
      switch (BridgeNamePtr->NameId) {
      case 0x10:
        Name = PcdGet32 (PcieGfx0AslName);
        break;
      case 0x11:
        Name = PcdGet32 (PcieGfx1AslName);
        break;
      case 0x12:
        Name = PcdGet32 (PcieGfx2AslName);
        break;
      case 0x13:
        Name = PcdGet32 (PcieGfx3AslName);
        break;
      case 0x14:
        Name = PcdGet32 (PcieGfx4AslName);
        break;
      case 0x20:
        Name = PcdGet32 (PcieGpp0AslName);
        break;
      case 0x21:
        Name = PcdGet32 (PcieGpp1AslName);
        break;
      case 0x22:
        Name = PcdGet32 (PcieGpp2AslName);
        break;
      case 0x23:
        Name = PcdGet32 (PcieGpp3AslName);
        break;
      case 0x24:
        Name = PcdGet32 (PcieGpp4AslName);
        break;
      case 0x25:
        Name = PcdGet32 (PcieGpp5AslName);
        break;
      case 0x26:
        Name = PcdGet32 (PcieGpp6AslName);
        break;
      case 0x27:
        Name = PcdGet32 (PcieGpp7AslName);
        break;
      case 0x28:
        Name = PcdGet32 (PcieGpp8AslName);
        break;
      case 0x29:
        Name = PcdGet32 (PcieGpp9AslName);
        break;
      case 0x2A:
        Name = PcdGet32 (PcieGppAAslName);
        break;
      case 0x2B:
        Name = PcdGet32 (PcieGppBAslName);
        break;
      case 0x2C:
        Name = PcdGet32 (PcieGppCAslName);
        break;
      case 0x2D:
        Name = PcdGet32 (PcieGppDAslName);
        break;
      case 0x2E:
        Name = PcdGet32 (PcieGppEAslName);
        break;
      case 0x2F:
        Name = PcdGet32 (PcieGppFAslName);
        break;
      case 0x30:
        Name = PcdGet32 (PcieGp17AslName);
        break;
      case 0x31:
        Name = PcdGet32 (PcieGp18AslName);
        break;
      case 0x32:
        Name = PcdGet32 (PcieGp28AslName);
        break;
      case 0x33:
        Name = PcdGet32 (PcieGp38AslName);
        break;
      case 0x38:
        Name = PcdGet32 (PcieGp13AslName);
        break;
      case 0x39:
        Name = PcdGet32 (PcieGp14AslName);
        break;
      case 0x40:
        Name = PcdGet32 (PcieGppGAslName);
        break;
      case 0x41:
        Name = PcdGet32 (PcieGppHAslName);
        break;
      }
      if (!Name) {
        Name = BridgeNamePtr->Name;
      }
      break;
    }
    BridgeNamePtr ++;
  }
  return Name;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get CPU Revision Id
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The CPU Revision Id
 *
 */
UINT8
CpmGetCpuRevisionId (
  IN       VOID                       *This
  )
{
  CPU_REVISION_ITEM *ItemPtr;
  UINT8             Revision;

  Revision = 0xFF;
  ItemPtr = CpmGetCpuRevisionItem (This);
  if (ItemPtr) {
    Revision = ItemPtr->CpuRevision;
  }

  return Revision;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Register CPM Common CPU Function
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 */
VOID
EFIAPI
CpmRegisterCpu (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT64                  Value;
  DEBUG((DEBUG_INFO, "  LIB-CPU-%a-Start\n", __FUNCTION__));
  MainTablePtr = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  if (!MainTablePtr->PcieMemIoBaseAddr) {
    Value = CommonFunctionPtr->MsrRead (0xC0010058);
    MainTablePtr->PcieMemIoBaseAddr = (UINT32) (Value & 0xFFF00000);
    DEBUG((DEBUG_INFO, "  LIB-CPU-%a-PcieMemIoBaseAddr = 0x%08X\n", __FUNCTION__, MainTablePtr->PcieMemIoBaseAddr));
  }
  if (MainTablePtr->PcdGetRVFamilyOPN != PcdGet32 (PcdGetRVFamilyOPN)) {
    MainTablePtr->PcdGetRVFamilyOPN = PcdGet32 (PcdGetRVFamilyOPN);
    DEBUG((DEBUG_INFO, "  LIB-CPU-%a-PcdGetRVFamilyOPN = 0x%08X\n", __FUNCTION__, MainTablePtr->PcdGetRVFamilyOPN));
  }
  CommonFunctionPtr->GetCpuRevisionId = CpmGetCpuRevisionId;
  if (!MainTablePtr->CpmCpuRevisionId) {
    MainTablePtr->CpmCpuRevisionId = CommonFunctionPtr->GetCpuRevisionId (This);
    DEBUG((DEBUG_INFO, "  LIB-CPU-%a-CpmCpuRevisionId = 0x%02X\n", __FUNCTION__, MainTablePtr->CpmCpuRevisionId));
  }
  if ((MainTablePtr->BxbBrMmioBaseAddr != 0) && ((MainTablePtr->BxbBrMmioBaseAddr & 0xF0000000) == 0)) {
    MainTablePtr->BxbBrMmioBaseAddr += MainTablePtr->PcieMemIoBaseAddr;
    DEBUG((DEBUG_INFO, "  LIB-CPU-%a-BxbBrMmioBaseAddr = 0x%08X\n", __FUNCTION__, MainTablePtr->BxbBrMmioBaseAddr));
  }
  CommonFunctionPtr->GetSbTsiAddr     = CpmGetSbTsiAddr;
  CommonFunctionPtr->IsThermalSupport = CpmIsThermalSupport;
  CommonFunctionPtr->GetPcieAslName   = CpmGetPcieAslName;
  CommonFunctionPtr->GetPtBrDevAddr   = CpmGetPtBrDevAddr;
  CommonFunctionPtr->GetBxbBusNumber  = CpmGetBxbBusNumber;
  CommonFunctionPtr->ClearBrBusNumber = CpmClearBrBusNumber;
  DEBUG((DEBUG_INFO, "  LIB-CPU-%a-End\n", __FUNCTION__));
  return;
}
