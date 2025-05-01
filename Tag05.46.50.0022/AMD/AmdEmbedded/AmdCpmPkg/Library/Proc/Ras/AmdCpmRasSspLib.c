/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/LocalApicLib.h>
#include "AmdRasRegistersSsp.h"
#include "AmdRas.h"
#include "AmdCpmRasSspLib.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

//MCA Instance Loop up table
MCA_INSTANCE_ID_ENTRY gUmcMcaInstanceTable[] = {
    {UMC0_MCA_INS_ID,0},
    {UMC1_MCA_INS_ID,1},
    {UMC2_MCA_INS_ID,2},
    {UMC3_MCA_INS_ID,3},
    {UMC4_MCA_INS_ID,4},
    {UMC5_MCA_INS_ID,5},
    {UMC6_MCA_INS_ID,6},
    {UMC7_MCA_INS_ID,7},
    {MCA_INSTANCE_ID_END, 0}
};

MCA_INSTANCE_ID_ENTRY gNbioMcaInstanceTable[] = {
    {NBIO0_MCA_INS_ID,0},
    {NBIO1_MCA_INS_ID,1},
    {NBIO2_MCA_INS_ID,2},
    {NBIO3_MCA_INS_ID,3},
    {MCA_INSTANCE_ID_END, 0}
};

MCA_INSTANCE_ID_ENTRY gPcieMcaInstanceTable[] = {
    {PCIE0_MCA_INS_ID,0},
    {PCIE1_MCA_INS_ID,1},
    {PCIE2_MCA_INS_ID,2},
    {PCIE3_MCA_INS_ID,3},
    {MCA_INSTANCE_ID_END, 0}
};

UINT32 SspIomsInstanceIds[] = {
  SSP_IOMS0_INSTANCE_ID,
  SSP_IOMS1_INSTANCE_ID,
  SSP_IOMS2_INSTANCE_ID,
  SSP_IOMS3_INSTANCE_ID
};


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * RasSmnRead - Read SMN register
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value       - Pointer to register value
 *
 */
VOID
RasSmnRead (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  *Value = PciRead32 (PciAddress);
}

VOID
RasSmnWrite (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  PciWrite32 (PciAddress, *Value);
}

VOID
RasSmnRW (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  )
{
  UINT32    RegValue;

  RasSmnRead ((UINTN)IohcBus, SmnAddress, &RegValue);
  RegValue &= AndMask;
  RegValue |= OrMask;
  RasSmnWrite ((UINTN)IohcBus, SmnAddress, &RegValue);
}

/**
 * RasSmnRead16 - Read SMN register in Word
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value16     - Pointer to register value
 *
 */
VOID
RasSmnRead16 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT16              *Value16
  )
{
  UINT32    RegIndex;
  UINT32    RegData;
  UINTN     PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  RegData = PciRead32 (PciAddress);

  *Value16 = RegData & 0xFFFF;
  if ((SmnAddress & 0x3) == 2) {
    *Value16 = RegData >> 16;
  }
}

VOID
RasSmnWrite16 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT16               *Value16
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  PciWrite16 (PciAddress + (UINTN) (SmnAddress & 0x3), *Value16);
}


/*----------------------------------------------------------------------------------------*/
/*
 * Check if device present
 *
 *
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @retval    TRUE            Device is present
 * @retval    FALSE           Device is not present
 */

BOOLEAN
RasPciIsDevicePresent (
  IN      UINT32              Address
  )
{
  UINT32  VendorId;
  VendorId = PciRead32 (Address);
  if (VendorId == 0xffffffff) {
    return FALSE;
  } else {
    return TRUE;
  }
}


/*----------------------------------------------------------------------------------------*/
/*
 * Check if device is bridge
 *
 *
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @retval    TRUE            Device is a bridge
 * @retval    FALSE           Device is not a bridge
 */

BOOLEAN
RasPciIsBridgeDevice (
  IN      UINT32              Address
  )
{
  UINT8   Header;
  Header = PciRead8 (Address | PCI_HEADER_TYPE_REG);
  if ((Header & 0x7f) == PCI_TO_PCI_BRIDGE) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 * Check if device is multifunction
 *
 *
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @retval    TRUE            Device is a multifunction device.
 * @retval    FALSE           Device is a single function device.
 *
 */
BOOLEAN
RasPciIsMultiFunctionDevice (
  IN      UINT32              Address
  )
{
  UINT8   Header;
  Header = PciRead8 (Address | PCI_HEADER_TYPE_REG);
  if ((Header & MULTI_FUNC_DEVICE_MASK) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 * Check if device is PCIe device
 *
 *
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @retval    TRUE            Device is a PCIe device
 * @retval    FALSE           Device is not a PCIe device
 *
 */

BOOLEAN
RasPciIsPcieDevice (
  IN      UINT32              Address
  )
{
  if (RasFindPciCapability (Address, PCIE_CAP_ID) != 0 ) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/*----------------------------------------------------------------------------------------*/
/*
 * Find PCI capability pointer
 *
 *
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @param[in] CapabilityId    PCI capability ID
 * @retval                    Register address of capability pointer
 *
 */

UINT8
RasFindPciCapability (
  IN      UINT32              Address,
  IN      UINT8               CapabilityId
  )
{
  UINT8     CapabilityPtr;
  UINT8     CurrentCapabilityId;
  CapabilityPtr = PCI_CAPABILITY_REG;
  if (!RasPciIsDevicePresent (Address)) {
    return  0;
  }
  while (CapabilityPtr != 0) {
    CapabilityPtr = PciRead8 (Address | CapabilityPtr);
    if (CapabilityPtr != 0) {
      CurrentCapabilityId = PciRead8 (Address | CapabilityPtr);
      if (CurrentCapabilityId == CapabilityId) {
        break;
      }
      CapabilityPtr++;
    }
  }
  return  CapabilityPtr;
}

/*----------------------------------------------------------------------------------------*/
/*
 * Find PCIe extended capability pointer
 *
 *
 *
 * @param[in] Address               PCI address (as described in PCI_ADDR)
 * @param[in] ExtendedCapabilityId  Extended PCIe capability ID
 * @param[in] Current Cap Ptr       Current PCIe extended capability pointer
 * @retval                          Register address of extended capability pointer
 *
 */


UINT16
RasFindPcieExtendedCapability (
  IN      UINT32              Address,
  IN      UINT16              ExtendedCapabilityId,
  IN      UINT16              CurrentCapabilityPtr
  )
{
  UINT16  CapabilityPtr;
  UINT32  ExtendedCapabilityIdBlock;
  if (RasPciIsPcieDevice (Address)) {
//    if (CurrentCapabilityPtr == 0xFFFF) {
//      CapabilityPtr = PCIE_EXTENDED_CAP_REG;
//    }
//    else {
//      CapabilityPtr =CurrentCapabilityPtr;
//    }
    CapabilityPtr = PCIE_EXTENDED_CAP_REG;
    ExtendedCapabilityIdBlock = PciRead32 (Address | CapabilityPtr);
    if ((ExtendedCapabilityIdBlock != 0) && ((UINT16)ExtendedCapabilityIdBlock != 0xffff)) {
      do {
        ExtendedCapabilityIdBlock = PciRead32 (Address | CapabilityPtr);
//        DEBUG ((EFI_D_ERROR, "   - Capability at 0x%x with type 0x%x\n", CapabilityPtr, (UINT16)ExtendedCapabilityIdBlock));
        if ((UINT16)ExtendedCapabilityIdBlock == ExtendedCapabilityId) {
          return  CapabilityPtr;
        }
        CapabilityPtr = (UINT16) ((ExtendedCapabilityIdBlock >> 20) & 0xfff);
      } while (CapabilityPtr !=  0);
    }
  }
  return  0;
}

/*----------------------------------------------------------------------------------------*/
/*
 * Scan range of device on PCI bus.
 *
 *
 *
 * @param[in] Start           Start address to start scan from
 * @param[in] End             End address of scan
 * @param[in] ScanData        Supporting data
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
RasPciScan (
  IN      PCI_ADDR            Start,
  IN      PCI_ADDR            End,
  IN      RAS_PCI_SCAN_DATA   *ScanData
  )
{
  UINTN       Bus;
  UINTN       Device;
  UINTN       LastDevice;
  UINTN       Function;
  UINTN       LastFunction;
  PCI_ADDR    PciDevice;
  SCAN_STATUS Status;

  for (Bus = Start.Address.Bus; Bus <= End.Address.Bus; Bus++) {
    Device = (Bus == Start.Address.Bus) ? Start.Address.Device : 0x00;
    LastDevice = (Bus == End.Address.Bus) ? End.Address.Device : 0x1F;
    for ( ; Device <= LastDevice; Device++) {
      if ((Bus == Start.Address.Bus) && (Device == Start.Address.Device)) {
        Function = Start.Address.Function;
      } else {
        Function = 0x0;
      }
      PciDevice.AddressValue = MAKE_SBDFO (0, Bus, Device, Function, 0);
      if (!RasPciIsDevicePresent (PciDevice.AddressValue)) {
        continue;
      }
      if (RasPciIsMultiFunctionDevice (PciDevice.AddressValue)) {
        if ((Bus == End.Address.Bus) && (Device == End.Address.Device)) {
          LastFunction = Start.Address.Function;
        } else {
          LastFunction = 0x7;
        }
      } else {
        LastFunction = 0x0;
      }
      for ( ; Function <= LastFunction; Function++) {
        PciDevice.AddressValue = MAKE_SBDFO (0, Bus, Device, Function, 0);
        if (RasPciIsDevicePresent (PciDevice.AddressValue)) {
          Status = ScanData->RasScanCallback (PciDevice, ScanData);
          if ((Status & SCAN_SKIP_FUNCTIONS) != 0) {
            Function = LastFunction + 1;
          }
          if ((Status & SCAN_SKIP_DEVICES) != 0) {
            Device = LastDevice + 1;
          }
          if ((Status & SCAN_SKIP_BUSES) != 0) {
            Bus = End.Address.Bus + 1;
          }
        }
      }
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Scan all subordinate buses
 *
 *
 * @param[in]   Bridge            PCI bridge address
 * @param[in,out] ScanData        Scan configuration data
 *
 */
VOID
RasPciScanSecondaryBus (
  IN       PCI_ADDR             Bridge,
  IN OUT   RAS_PCI_SCAN_DATA    *ScanData
  )
{
  PCI_ADDR  StartRange;
  PCI_ADDR  EndRange;
  UINT8     SecondaryBus;
  SecondaryBus = PciRead8 (Bridge.AddressValue | PCI_SECONDARY_BUS_REG);
  DEBUG ((EFI_D_ERROR, "  Secondry Bus Number: 0x%x\n", SecondaryBus));

  if (SecondaryBus != 0) {
    StartRange.AddressValue = MAKE_SBDFO (0, SecondaryBus, 0, 0, 0);
    EndRange.AddressValue = MAKE_SBDFO (0, SecondaryBus, 0x1f, 0x7, 0);
    RasPciScan (StartRange, EndRange, ScanData);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get PCIe device type
 *
 *
 *
 * @param[in] Device      PCI address of device.
 *
 * @retval    PCIE_DEVICE_TYPE
 */
 /*----------------------------------------------------------------------------------------*/

PCIE_DEVICE_TYPE
RasGetPcieDeviceType (
  IN      PCI_ADDR            Device
  )
{
  UINT8       PcieCapPtr;
  UINT8       Value;

  PcieCapPtr = RasFindPciCapability (Device.AddressValue, PCIE_CAP_ID);
  if (PcieCapPtr != 0) {
    Value = PciRead8 (Device.AddressValue | (PcieCapPtr + PCIE_CAP_REGISTER));
    return Value >> 4;
  }
  return PcieNotPcieDevice;
}

EFI_STATUS
RasPcieStsClr (
  IN       PCI_ADDR  PciCfgAddr,
  IN       UINT32    RetryCnt
  )
{
  UINT8         PcieCapPtr;
  UINT16        AerCapPtr;
  UINT16        SecPcieExtCapPtr;
  UINT16        PciStatus;
  UINT16        PciSecStatus;
  UINT32        PcieUncorrStatus;
  UINT32        PcieCorrStatus;
  UINT32        PcieRootStatus;
  UINT16        PcieDevStatus;
  UINT32        PcieLnkStatus;
  UINT32        RasRetryCounter;

  AerCapPtr = RasFindPcieExtendedCapability (PciCfgAddr.AddressValue, PCIE_EXT_AER_CAP_ID, 0xFFFF);
  SecPcieExtCapPtr = RasFindPcieExtendedCapability (PciCfgAddr.AddressValue, SEC_PCIE_EXT_CAP_ID, 0xFFFF);
  PcieCapPtr = RasFindPciCapability (PciCfgAddr.AddressValue, PCIE_CAP_ID);

  DEBUG ((EFI_D_ERROR, "[RAS]ClrSts @ Bus: 0x%x, Dev: 0x%x, Func: 0x%x\n", PciCfgAddr.Address.Bus, PciCfgAddr.Address.Device, PciCfgAddr.Address.Function));
  //Clear Status register
  RasRetryCounter = RetryCnt;
  if (AerCapPtr != 0) {
    PcieUncorrStatus = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_UNCORR_STATUS_PTR);
    PcieCorrStatus = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_CORR_STATUS_PTR);
    PcieRootStatus = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_ROOT_STATUS_PTR);
    while ((PcieUncorrStatus != 0) || (PcieCorrStatus != 0) || (PcieRootStatus != 0)) {
      if (RasRetry (&RasRetryCounter)) {
        break;
      }
      DEBUG ((EFI_D_ERROR, "[RAS]  PCIE UnCorr Error Status : 0x%08x\n", PcieUncorrStatus));
      PciWrite32(PciCfgAddr.AddressValue + AerCapPtr + PCIE_UNCORR_STATUS_PTR, PcieUncorrStatus);
      PcieUncorrStatus = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_UNCORR_STATUS_PTR);

      DEBUG ((EFI_D_ERROR, "[RAS]  PCIE Corr Error Status : 0x%08x\n", PcieCorrStatus));
      PciWrite32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_CORR_STATUS_PTR, PcieCorrStatus);
      PcieCorrStatus = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_CORR_STATUS_PTR);

      if (RasGetPcieDeviceType (PciCfgAddr) == PcieDeviceRootComplex) {
        DEBUG ((EFI_D_ERROR, "[RAS]  PCIE Root Error Status : 0x%08x\n", PcieRootStatus));
        PciWrite32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_ROOT_STATUS_PTR, PcieRootStatus);
        PcieRootStatus = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_ROOT_STATUS_PTR);
      }
    }
  }

  RasRetryCounter = RetryCnt;
  if (PcieCapPtr != 0) {
    PcieDevStatus = PciRead16 (PciCfgAddr.AddressValue + PcieCapPtr + PCIE_DEVICE_STATUS_PTR);
    while ((PcieDevStatus & 0xF) != 0) {
      if (RasRetry (&RasRetryCounter)) {
        break;
      }
      DEBUG ((EFI_D_ERROR, "[RAS]  PCIE Device Status : 0x%08x\n", PcieDevStatus));
      PciWrite16(PciCfgAddr.AddressValue + PcieCapPtr + PCIE_DEVICE_STATUS_PTR, PcieDevStatus);
      PcieDevStatus = PciRead16 (PciCfgAddr.AddressValue + PcieCapPtr + PCIE_DEVICE_STATUS_PTR);
    }
  }

  RasRetryCounter = RetryCnt;
  if (SecPcieExtCapPtr !=0 ) {
    PcieLnkStatus = PciRead32 (PciCfgAddr.AddressValue + SecPcieExtCapPtr + PCIE_LANE_ERR_STATUS_PTR);
    while (PcieLnkStatus != 0) {
      if (RasRetry (&RasRetryCounter)) {
        break;
      }
      DEBUG ((EFI_D_ERROR, "[RAS]  PCIE Lane Error Status : 0x%08x\n", PcieLnkStatus));
      PciWrite32 (PciCfgAddr.AddressValue + SecPcieExtCapPtr + PCIE_LANE_ERR_STATUS_PTR, PcieLnkStatus);
      PcieLnkStatus = PciRead32 (PciCfgAddr.AddressValue + SecPcieExtCapPtr + PCIE_LANE_ERR_STATUS_PTR);
    }
  }

  //Legacy PCI Error status
  RasRetryCounter = RetryCnt;
  PciStatus = PciRead16 (PciCfgAddr.AddressValue + PCI_STATUS_REG);
  while ((PciStatus & 0xF900) != 0) {
    if (RasRetry (&RasRetryCounter)) {
      break;
    }
    DEBUG ((EFI_D_ERROR, "[RAS]  PCI Status : 0x%04x\n", PciStatus));
    PciWrite16(PciCfgAddr.AddressValue + PCI_STATUS_REG, PciStatus);
    PciStatus = PciRead16 (PciCfgAddr.AddressValue + PCI_STATUS_REG);
  }

  RasRetryCounter = RetryCnt;
  PciSecStatus = PciRead16 (PciCfgAddr.AddressValue + PCI_SEC_STATUS_REG);
  while ((PciSecStatus & 0xF900) != 0) {
    if (RasRetry (&RasRetryCounter)) {
      break;
    }
    DEBUG ((EFI_D_ERROR, "[RAS]  PCI Secondary Status : 0x%04x\n", PciSecStatus));
    PciWrite16(PciCfgAddr.AddressValue + PCI_SEC_STATUS_REG, PciSecStatus);
    PciSecStatus = PciRead16 (PciCfgAddr.AddressValue + PCI_SEC_STATUS_REG);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
RasDpcErrHandle (
  IN       PCI_ADDR PciCfgAddr,
  IN       UINT32   RetryCnt
)
{
  UINT32        RasRetryCounter;
  DPC_CAP_REG   DpcCapReg;
  DPC_CNTL_REG  DpcCntlReg;
  DPC_CNTL_REG  DpcCntlRegBackup;
  DPC_STS_REG   DpcStsReg;
  UINT16        DpcCapPtr;
  UINT16        DataLinkSts;
  UINT8         PcieCapPtr;

  DpcCapPtr = RasFindPcieExtendedCapability (PciCfgAddr.AddressValue, DPC_EXT_CAP_ID, 0xFFFF);

  if (DpcCapPtr == 0 ) {
    return EFI_SUCCESS;
  }
  // 1. Check DPC status, if non-0 perform next step. If 0, end handle.
  DpcStsReg.Value = PciRead16 (PciCfgAddr.AddressValue + DpcCapPtr + DPC_STATUS_REG);
  if (DpcStsReg.Value == 0) {
    return EFI_SUCCESS;
  }

  // 2. Check DPC trigger and COR_ERR_EN bit = 1
  DpcCntlReg.Value = PciRead16 (PciCfgAddr.AddressValue + DpcCapPtr + DPC_CONTROL_REG);
  if ((DpcCntlReg.Field.DpcErrCorEnable == 0) || (DpcCntlReg.Field.DpcTriggerEnable == 0)) {
    return EFI_SUCCESS;
  }

  PcieCapPtr = RasFindPciCapability (PciCfgAddr.AddressValue, PCIE_CAP_ID);
  //Should only PCIE downstream and rootport device will get this point
  //Skip the PcieCapPtr return value check.

  // 3. Check Link Status register
  // Check Link Status Data Link Layer Link Active bit
  RasRetryCounter = RetryCnt;
  DataLinkSts = PciRead16 (PciCfgAddr.AddressValue + PcieCapPtr + PCIE_LINK_STATUS_REGISTER);
  while ((DataLinkSts & PCIE_DATA_LINK_LAYER_LINK_ACTIVE) != 0 ) {
    if (RasRetry (&RasRetryCounter)) {
      DEBUG ((EFI_D_ERROR, "  Data Link Layer Link Active = 1 after retried, action abort !!!"));
      return EFI_ABORTED;
    }
    MicroSecondDelay(1000);     //1 ms
    DataLinkSts = PciRead16 (PciCfgAddr.AddressValue + PcieCapPtr + PCIE_LINK_STATUS_REGISTER);
  }

  // 4. Check Capability Reg for RP Extensions for DPC.
  //    If = 1, Check DPC RP Busy bit, If = 0, skip Check DPC RP Busy bit
  DpcCapReg.Value = PciRead16 (PciCfgAddr.AddressValue + DpcCapPtr + DPC_CAPABILITY_REG);
  if (DpcCapReg.Field.RpExtensionsForDpc) {
    // Check DPC RP Busy bit
    RasRetryCounter = RetryCnt;
    while (DpcStsReg.Field.DpcRpBusy) {
      if (RasRetry (&RasRetryCounter)) {
        DEBUG ((EFI_D_ERROR, "  DPC RP Busy = 1 after retried, action abort !!!"));
        return EFI_ABORTED;
      }
      MicroSecondDelay(1000);       //1 ms
      DpcStsReg.Value = PciRead16 (PciCfgAddr.AddressValue + DpcCapPtr + DPC_STATUS_REG);
    }
  }

  // 5. Disable DPC trigger/COR_ERR_EN bit
  DpcCntlRegBackup.Value = DpcCntlReg.Value;
  DpcCntlReg.Field.DpcTriggerEnable = 0;
  DpcCntlReg.Field.DpcErrCorEnable = 0;
  PciWrite16(PciCfgAddr.AddressValue + DpcCapPtr + DPC_CONTROL_REG, DpcCntlReg.Value);

  // 6. Clear DPC status.
  PciWrite16(PciCfgAddr.AddressValue + DpcCapPtr + DPC_STATUS_REG, DpcStsReg.Value);

  // 7. Poll Data Link Layer Link Active until link becomes active ( = 1), upto 1 second waiting time.
  RasRetryCounter = PCIE_DATA_LINK_POLLING_COUNT;
  DataLinkSts = PciRead16 (PciCfgAddr.AddressValue + PcieCapPtr + PCIE_LINK_STATUS_REGISTER);
  while ((DataLinkSts & PCIE_DATA_LINK_LAYER_LINK_ACTIVE) == 0 ) {
    if (RasRetry (&RasRetryCounter)) {
      DEBUG ((EFI_D_ERROR, "  Data Link Layer Link Active = 0 after , action abort !!!"));
      return EFI_ABORTED;
    }
    MicroSecondDelay(10000);  //10ms
    DataLinkSts = PciRead16 (PciCfgAddr.AddressValue + PcieCapPtr + PCIE_LINK_STATUS_REGISTER);
  }

  // 8. Re-enable DPC trigger/COR_ERR_EN bit
  PciWrite16(PciCfgAddr.AddressValue + DpcCapPtr + DPC_CONTROL_REG, DpcCntlRegBackup.Value);

  return EFI_SUCCESS;
}


BOOLEAN
AerUnCorrErrCheck(
  PCI_ADDR      PciCfgAddr,
  UINT16        AerCapPtr
)
{
  UINT32            PcieErrorStatus;
  UINT32            PcieErrorMask;
  UINT32            PcieError;

  PcieErrorStatus = PciRead32(PciCfgAddr.AddressValue + AerCapPtr + PCIE_UNCORR_STATUS_PTR);
  PcieErrorMask = PciRead32(PciCfgAddr.AddressValue + AerCapPtr + PCIE_UNCORR_MASK_PTR);
  DEBUG ((EFI_D_ERROR, "  PCIE AER UnCorr Error Status: 0x%08x\n", PcieErrorStatus));
  DEBUG ((EFI_D_ERROR, "  PCIE AER UnCorr Error Mask: 0x%08x\n", PcieErrorMask));

  PcieError = PcieErrorStatus & ~PcieErrorMask;
  if (0 != PcieError) {
    DEBUG ((EFI_D_ERROR, "  PCIE AER UnCorr Error Found\n"));
    return TRUE;
  }
  return FALSE;
}


BOOLEAN
AerCorrErrCheck(
  PCI_ADDR      PciCfgAddr,
  UINT16        AerCapPtr
)
{
  UINT32            PcieErrorStatus;
  UINT32            PcieErrorMask;
  UINT32            PcieError;

  PcieErrorStatus = PciRead32(PciCfgAddr.AddressValue + AerCapPtr + PCIE_CORR_STATUS_PTR);
  PcieErrorMask = PciRead32(PciCfgAddr.AddressValue + AerCapPtr + PCIE_CORR_MASK_PTR);
  DEBUG ((EFI_D_ERROR, "  PCIE AER Corr Error Status: 0x%08x\n", PcieErrorStatus));
  DEBUG ((EFI_D_ERROR, "  PCIE AER Corr Error Mask: 0x%08x\n", PcieErrorMask));

  PcieError = PcieErrorStatus & ~PcieErrorMask;
  if (0 != PcieError) {
    DEBUG ((EFI_D_ERROR, "  PCIE AER Corr Error Found!!!\n"));
    return TRUE;
  }
  return FALSE;
}

BOOLEAN
AerErrCheck(
  PCI_ADDR      PciCfgAddr,
  UINT16        AerCapPtr
)
{
  BOOLEAN           ValidError;

  ValidError = AerUnCorrErrCheck (PciCfgAddr, AerCapPtr);
  if (ValidError) {
    return TRUE;
  }

  ValidError = AerCorrErrCheck (PciCfgAddr, AerCapPtr);
  if (ValidError) {
    return TRUE;
  }
  return FALSE;
}


BOOLEAN
RasPciErrCheck(
  IN       PCI_ADDR PciCfgAddr,
  OUT      BOOLEAN  *RootStatusSet,
  OUT      UINT32   *RootErrStatus
)
{
  ROOT_ERR_STS_REG  RootErrSts;
  UINT16            AerCapPtr;

  AerCapPtr = RasFindPcieExtendedCapability (PciCfgAddr.AddressValue, PCIE_EXT_AER_CAP_ID, 0xFFFF);

  DEBUG ((EFI_D_ERROR, "  Check PCIE Error Status @ Bus: 0x%x, Dev: 0x%x, Func: 0x%x\n", PciCfgAddr.Address.Bus, PciCfgAddr.Address.Device, PciCfgAddr.Address.Function));

  if (AerCapPtr != 0) {
    if (RasGetPcieDeviceType (PciCfgAddr) == PcieDeviceRootComplex) {
      RootErrSts.Value = PciRead32 (PciCfgAddr.AddressValue + AerCapPtr + PCIE_ROOT_STATUS_PTR);
      DEBUG ((EFI_D_ERROR, "  PCIE AER Root Error Status: 0x%08x\n", RootErrSts.Value));
      *RootErrStatus = RootErrSts.Value;

      if ((RootErrSts.Value & ROOT_ERROR_STATUS_MASK) == 0) {
        //No error found.
        return FALSE;
      }
      if (RootErrSts.Field.MultiErrCorReceived ||
          RootErrSts.Field.MultiErrFatalNonFatalReceived ||
          (RootErrSts.Field.ErrCorReceived & RootErrSts.Field.ErrFatalNonFatalReceived)) {
        //Set flag to check end-point device if multiple error recievied.
        *RootStatusSet = TRUE;
      }
      //Single error recieved, check root port AER register first.
      if (AerErrCheck(PciCfgAddr, AerCapPtr)) {
        return TRUE;
      }

      //Not root port error. Set flag for end-point device error status check.
      *RootStatusSet = TRUE;
      return FALSE;
    }

    //Non root complex PCIe device comes here.
    if (*RootStatusSet) {
      if (AerErrCheck(PciCfgAddr, AerCapPtr)) {
        return TRUE;
      }
    } //if (*RootStatusSet)
  }

  return FALSE;
}

EFI_STATUS
RasGetPlLogStrucHdr(
  IN       PCI_ADDR     PciCfgAddr,
  IN       UINT16       Offset,
  IN       UINT16       CapId,
  OUT      UINT16       *ErrLogOffset
)
{
  CCIX_ERROR_LOG      CcixErrorLog;

  switch (CapId) {
  case CCIX_COMMON_STRUCT_ID:
    CcixErrorLog.AsUint32 = PciRead32(PciCfgAddr.AddressValue + Offset + CCIX_DEV_ERROR_LOG_OFFSET);
    break;

  case CCIX_PORT_STRUCT_ID:
    CcixErrorLog.AsUint32 = PciRead32(PciCfgAddr.AddressValue + Offset + CCIX_PORT_ERR_LOG_OFFSET);
    break;

  case CCIX_LINK_STRUCT_ID:
    CcixErrorLog.AsUint32 = PciRead32(PciCfgAddr.AddressValue + Offset + CCIX_LINK_ERR_LOG_OFFSET);
    break;

  case CCIX_HA_STRUCT_ID:
    CcixErrorLog.AsUint32 = PciRead32(PciCfgAddr.AddressValue + Offset + CCIX_DEV_ERROR_LOG_OFFSET);
    break;

  case CCIX_RA_STRUCT_ID:
    CcixErrorLog.AsUint32 = PciRead32(PciCfgAddr.AddressValue + Offset + CCIX_DEV_ERROR_LOG_OFFSET);
    break;

  case CCIX_SA_STRUCT_ID:
    CcixErrorLog.AsUint32 = PciRead32(PciCfgAddr.AddressValue + Offset + CCIX_SA_ERR_LOG_OFFSET);
    break;

  default:  /* Unsupported */
    return EFI_UNSUPPORTED;
    break;
  }

  *ErrLogOffset = (UINT16)CcixErrorLog.AsBits.ErrLogOffset;

  return EFI_SUCCESS;
}

EFI_STATUS
RasGetPlLogInfo(
  IN       PCI_ADDR                     PciCfgAddr,
  OUT      CCIX_PROTOCOL_DVSEC_HEADER   *PlCapStatPtr,
  OUT      CCIX_PROTOCOL_DVSEC_HEADER   *PlCntlPtr,
  OUT      UINT16                       *CapOffset,
  OUT      UINT16                       *CntlOffset
)
{
  UINT16                      CcixCapPtr;
  PCIE_EXTENDED_CAP_HEADER    PcieExtCapHdr;
  PCIE_DVSEC_HEADER2          PcieDvsecHeader2;

  DEBUG ((EFI_D_ERROR, "  Check CCIX Error Status @ Bus: 0x%x, Dev: 0x%x, Func: 0x%x\n", PciCfgAddr.Address.Bus, PciCfgAddr.Address.Device, PciCfgAddr.Address.Function));
  CcixCapPtr = RasFindPcieExtendedCapability (PciCfgAddr.AddressValue, DVSEC_EXTENDED_CAP_ID, 0xFFFF);
  if (CcixCapPtr == 0) {
    //Not CCIX endpoint device
    return EFI_UNSUPPORTED;
  }

  PcieDvsecHeader2.AsUint32 = PciRead32(PciCfgAddr.AddressValue + CcixCapPtr + DVSEC_HEADER2_OFFSET);
  if (CCIX_PROTOCOL_DVSEC_ID != PcieDvsecHeader2.AsBits.DvsecId) {
    //Not protocol layer component, Find next DVSEC CAP ID
    PcieExtCapHdr.AsUint32 = PciRead32(PciCfgAddr.AddressValue + CcixCapPtr);
    if (PcieExtCapHdr.AsBits.NextCapPtr == 0) {
      return EFI_UNSUPPORTED;
    }
    //Find next DVSEC CAP ID
    CcixCapPtr = RasFindPcieExtendedCapability (PciCfgAddr.AddressValue, DVSEC_EXTENDED_CAP_ID, CcixCapPtr);
    PcieDvsecHeader2.AsUint32 = PciRead32(PciCfgAddr.AddressValue + CcixCapPtr + DVSEC_HEADER2_OFFSET);
    if (CCIX_PROTOCOL_DVSEC_ID != PcieDvsecHeader2.AsBits.DvsecId) {
      return EFI_UNSUPPORTED;
    }
  }

  //Protocol Layer component Cap identified, Get Capability & Status Register Offset
  PlCapStatPtr->AsUint32 = PciRead32(PciCfgAddr.AddressValue + CcixCapPtr + DVSEC_CCIX_PL_STATUS_OFFSET);
  *CapOffset = (UINT16)PlCapStatPtr->AsBits.Offset;

  //Protocol Layer component Cap identified, Get Control Register Offset
  PlCntlPtr->AsUint32 = PciRead32(PciCfgAddr.AddressValue + CcixCapPtr + DVSEC_CCIX_PL_CONTROL_OFFSET);
  *CntlOffset = (UINT16)PlCntlPtr->AsBits.Offset;

  return EFI_SUCCESS;
}

EFI_STATUS
RasCollectPlOffset(
  IN       PCI_ADDR                     Device,
  IN       UINT16                       StatOffset,
  IN       CCIX_PROTOCOL_DVSEC_HEADER   *PlCapStatPtr,
  OUT      UINT16                       *CmnOffset,
  OUT      UINT16                       *PortOffset
)
{
  PCIE_EXTENDED_CAP_HEADER    PlCapStsHdr;
  UINT16                      CurrentStatOffset;

  CurrentStatOffset = StatOffset;

  do {
    // Get Capability Header
    PlCapStsHdr.AsUint32 = PciRead32(Device.AddressValue + CurrentStatOffset);

    switch (PlCapStsHdr.AsBits.CapId) {
    case CCIX_COMMON_STRUCT_ID:
      *CmnOffset = CurrentStatOffset;
      break;

    case CCIX_PORT_STRUCT_ID:
      *PortOffset = CurrentStatOffset;
      break;
    }

    // Next Capability
    CurrentStatOffset = (UINT16)PlCapStsHdr.AsBits.NextCapPtr;

    if (CurrentStatOffset > (UINT16)(PlCapStatPtr->AsBits.Offset + PlCapStatPtr->AsBits.Size)) {
      CurrentStatOffset = 0;
    }

  } while (CurrentStatOffset != 0);
  return EFI_SUCCESS;
}

EFI_STATUS
RasCcixErrCheck(
  IN       PCI_ADDR           PciCfgAddr,
  IN       RAS_ERR_LOG_DATA   *ErrLogData
)
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT16                      CurrentStatOffset;
  UINT16                      CurrentCntlOffset;
  UINT16                      StatOffset;
  UINT16                      CntlOffset;
  UINT16                      ErrLogOffset;
  UINT16                      CmnStatOffset;
  UINT16                      PortStatOffset;
  PCIE_EXTENDED_CAP_HEADER    PlCapStsHdr;
  PCIE_EXTENDED_CAP_HEADER    PlCntlHdr;
  CCIX_PROTOCOL_DVSEC_HEADER  PlCapStatPtr;
  CCIX_PROTOCOL_DVSEC_HEADER  PlCntlPtr;
  PER_LOG_HEADER_DW0          PerLogHeaderDw0;
  CCIX_ERROR_LOG_DATA         CcixErrorLogData;

  Status = RasGetPlLogInfo(PciCfgAddr, &PlCapStatPtr, &PlCntlPtr, &StatOffset, &CntlOffset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ErrLogData->Buffer = &CcixErrorLogData;
  CmnStatOffset = 0;
  PortStatOffset = 0;

  CurrentStatOffset = StatOffset;
  CurrentCntlOffset = CntlOffset;

  RasCollectPlOffset(PciCfgAddr, StatOffset, &PlCapStatPtr, &CmnStatOffset, &PortStatOffset);

  do {
    // Get Capability Header
    PlCapStsHdr.AsUint32 = PciRead32(PciCfgAddr.AddressValue + CurrentStatOffset);
    PlCntlHdr.AsUint32 = PciRead32(PciCfgAddr.AddressValue + CurrentCntlOffset);

    Status = RasGetPlLogStrucHdr(PciCfgAddr, CurrentStatOffset, (UINT16)PlCapStsHdr.AsBits.CapId, &ErrLogOffset);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    PerLogHeaderDw0.Value = PciRead32(PciCfgAddr.AddressValue + ErrLogOffset);

    DEBUG ((DEBUG_INFO, " Error Log Offset: 0x%03X, CapId: 0x%04X (%a), CapVer: 0x%0X, NextCapPtr: 0x%03X\n, Log Size: 0x%X",
      ErrLogOffset,
      PlCapStsHdr.AsBits.CapId,
      PlCapStsHdr.AsBits.CapVersion,
      PlCapStsHdr.AsBits.NextCapPtr,
      PerLogHeaderDw0.Field.LogLen));

    CcixErrorLogData.CurrentStatOffset = CurrentStatOffset;
    CcixErrorLogData.CurrentCntlOffset = CurrentCntlOffset;
    CcixErrorLogData.ErrLogOffset = ErrLogOffset;
    CcixErrorLogData.CmnStatOffset = CmnStatOffset;
    CcixErrorLogData.PortStatOffset = PortStatOffset;

    //A Valid Protocol Layer Error Log found
    if (CCIX_PL_LOG_HEADER_SIZE < PerLogHeaderDw0.Field.LogLen) {
      ErrLogData->RasLogCallback(PciCfgAddr,ErrLogData);
    }

    // Next Capability
    CurrentStatOffset = (UINT16)PlCapStsHdr.AsBits.NextCapPtr;
    CurrentCntlOffset = (UINT16)PlCntlHdr.AsBits.NextCapPtr;

    if (CurrentStatOffset > (UINT16)(PlCapStatPtr.AsBits.Offset + PlCapStatPtr.AsBits.Size)) {
      CurrentStatOffset = 0;
    }
    if (CurrentCntlOffset > (UINT16)(PlCntlPtr.AsBits.Offset + PlCntlPtr.AsBits.Size)) {
      CurrentCntlOffset = 0;
    }
  } while ((CurrentStatOffset != 0) && (CurrentCntlOffset != 0));

  return Status;
}


EFI_STATUS
RasPcieErrStsCheck(
  IN       PCIE_PORT_PROFILE      *PciePortProfileInstance,
  IN OUT   PCIE_ERR_ENTRY         *PcieErrorEntry,
  OUT      UINT16                 *PcieErrorEntryCnt
)
{
  PCIE_ERR_ENTRY  *LocalErrEntry;
  PCIE_DEV_ENTRY  *PcieDevEntry;
  PCI_ADDR        PciCfgAddr;
  UINT8           Index;
  LIST_ENTRY      *Node;
  BOOLEAN         RootStatusSet;
  BOOLEAN         IsRootPortError;
  UINT32          RootErrStatus;

  //The available PCIE_ERR_ENTRY number should >= the PciePortProfileInstance->EndPointDevCnt + 1;
  LocalErrEntry = PcieErrorEntry;
  *PcieErrorEntryCnt = 0;
  RootStatusSet = FALSE;

  //Check Root Port Error
  PciCfgAddr.AddressValue = PciePortProfileInstance->RpPciAddr;
  DEBUG ((EFI_D_ERROR, "  RootPort Address: 0x%08x\n", PciCfgAddr.AddressValue));

  IsRootPortError = FALSE;
  if (RasPciErrCheck(PciCfgAddr, &RootStatusSet, &RootErrStatus)) {
    LocalErrEntry->DevAddr = PciCfgAddr.AddressValue;
    LocalErrEntry->DevType = PcieDeviceRootComplex;
    LocalErrEntry->RootErrSts = RootErrStatus;
    LocalErrEntry->EntryValid = TRUE;
    LocalErrEntry++;
    (*PcieErrorEntryCnt)++;

    IsRootPortError = TRUE;
  }

  // Clear Root Error Status if it is not a Root port error
  if (!IsRootPortError && RootStatusSet) {
    RasPcieStsClr (PciCfgAddr, 0x03);
  }

  if (RootStatusSet) {
    Node = NULL;
    //Check PCI devices under root port
    DEBUG ((EFI_D_ERROR, "[RAS]EndPointDevCnt : %d\n", PciePortProfileInstance->EndPointDevCnt));
    if (PciePortProfileInstance->EndPointDevCnt != 0) {
      //Search End Point device error
      for (Index = 0; Index <PciePortProfileInstance->EndPointDevCnt; Index++) {
        if (Index == 0) {
          DEBUG ((EFI_D_ERROR, "  Get First Node, Head Forward Link Addr: 0x%08x\n",PciePortProfileInstance->PciLinkList.ForwardLink));
          Node = GetFirstNode(&PciePortProfileInstance->PciLinkList);
        } else {
          DEBUG ((EFI_D_ERROR, "  Get Next Node, Current Node Address: 0x%08x\n",(UINTN)Node));
          Node = GetNextNode(&PciePortProfileInstance->PciLinkList, Node);
        }
        PcieDevEntry = (PCIE_DEV_ENTRY*)Node;
        PciCfgAddr.AddressValue = PcieDevEntry->DevAddr;
        DEBUG ((EFI_D_ERROR, "  Device Address: 0x%08x\n", PciCfgAddr.AddressValue));

        if (RasPciErrCheck(PciCfgAddr, &RootStatusSet, &RootErrStatus)) {
          LocalErrEntry->DevAddr = PcieDevEntry->DevAddr;
          LocalErrEntry->DevType = PcieDevEntry->DevType;
          LocalErrEntry->RootErrSts = RootErrStatus;
          LocalErrEntry->EntryValid = TRUE;
          LocalErrEntry++;
          (*PcieErrorEntryCnt)++;
        }
      }
    }
  }

  return EFI_SUCCESS;
}


EFI_STATUS
RasSlinkErrStsCheck(
  IN       PCIE_PORT_PROFILE    *PciePortProfileInstance,
  IN       RAS_ERR_LOG_DATA     *ErrLogData
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Index;
  LIST_ENTRY      *Node;
  PCIE_DEV_ENTRY  *PcieDevEntry;
  PCI_ADDR        PciCfgAddr;

  Node = NULL;
  //Check PCI devices under root port
  DEBUG ((EFI_D_ERROR, "[RAS]EndPointDevCnt : %d\n", PciePortProfileInstance->EndPointDevCnt));
  if (PciePortProfileInstance->EndPointDevCnt != 0) {
    //Search End Point device error
    for (Index = 0; Index <PciePortProfileInstance->EndPointDevCnt; Index++) {
      if (Index == 0) {
        DEBUG ((EFI_D_ERROR, "  Get First Node, Head Forward Link Addr: 0x%08x\n",PciePortProfileInstance->PciLinkList.ForwardLink));
        Node = GetFirstNode(&PciePortProfileInstance->PciLinkList);
      } else {
        DEBUG ((EFI_D_ERROR, "  Get Next Node, Current Node Address: 0x%08x\n",(UINTN)Node));
        Node = GetNextNode(&PciePortProfileInstance->PciLinkList, Node);
      }
      PcieDevEntry = (PCIE_DEV_ENTRY*)Node;
      PciCfgAddr.AddressValue = PcieDevEntry->DevAddr;
      DEBUG ((EFI_D_ERROR, "  Device Address: 0x%08x\n", PciCfgAddr.AddressValue));

      if (PcieDevEntry->DevType != PcieDeviceEndPoint) {
        continue;
      }
      //Check and Log Ccix error
      Status = RasCcixErrCheck(PciCfgAddr, ErrLogData);
    }
  }

  return Status;
}

VOID
GetCpuId (RAS_CPUID_FN* RasCpuIdFn)
{
  AsmCpuid (
    RasCpuIdFn->FunctionId,
    &RasCpuIdFn->EAX_Reg,
    &RasCpuIdFn->EBX_Reg,
    &RasCpuIdFn->ECX_Reg,
    &RasCpuIdFn->EDX_Reg
    );
}

VOID
GetPciMmioBase (
  OUT   UINT64 *MmioBase
)
{
  UINT64 MmioMsr;
  //Get PCI configuration MMIO base address.
  MmioMsr = AsmReadMsr64 (MSR_MMIO_CFG_BASE);
  *MmioBase = (MmioMsr & 0x0000FFFFFFF00000);
}

VOID
GetLocalApicBase (
  OUT   UINT64 *ApicBase
)
{
  *ApicBase = AsmReadMsr64 (MSR_APIC_BAR);
  *ApicBase &= LAPIC_BASE_ADDR_MASK;
}

VOID
GetLocalApicId(
  OUT       UINT32 *LocalApicId
)
{
  *LocalApicId = GetApicId();
}

VOID
GetSysEnterEip (
  OUT      UINT64* SysEnterEip
)
{
  UINT64 McgStatReg;
  McgStatReg = AsmReadMsr64 (MSR_MCG_STAT);

  if ((McgStatReg & MSR_MCG_EIPV) != 0) {
    *SysEnterEip = AsmReadMsr64 (MSR_SYSENTER_EIP);
  }
}

VOID
GetMcaBankCount(
  OUT      UINT8* BankCount
  )
{
  UINT64 MsrData;
  MsrData = AsmReadMsr64 (MSR_MCG_CAP);            // MCG_CAP
  *BankCount = (UINT8) (((MSR_MCG_CAP_STRUCT*)&MsrData)->Count);
}

VOID
MsrRegisterAccess (
  IN OUT   PLAT_RAS_MSR_ACCESS  *RasMsrAccess
)
{
    if (RasMsrAccess->IsWrite) {
      AsmWriteMsr64 (RasMsrAccess->RegisterAddress, RasMsrAccess->RegisterValue);
    } else {
      RasMsrAccess->RegisterValue = AsmReadMsr64 (RasMsrAccess->RegisterAddress);
    }
}

/*---------------------------------------------------------------------------------------*/
/*
 * Search MCA Bank instance ID number
 *
 * Search MCA Bank for match Hardware ID list and
 * return the instance numbering.
 *
 * @param[in]  McaIpidMsr             MCA_IPID MSR
 *
 */
UINT8
McaInstanceIdSearch (
  MCA_IPID_MSR McaIpidMsr
  )
{
  UINTN Index;
  MCA_INSTANCE_ID_ENTRY *McaInstanceIdList = NULL;

  if (McaIpidMsr.Field.HardwareID == MCA_UMC_ID) {
    McaInstanceIdList = &gUmcMcaInstanceTable[0];
  }
  if (McaIpidMsr.Field.HardwareID == MCA_NBIO_ID) {
    McaInstanceIdList = &gNbioMcaInstanceTable[0];
  }
  if (McaIpidMsr.Field.HardwareID == MCA_PCIE_ID) {
    McaInstanceIdList = &gPcieMcaInstanceTable[0];
  }
  //if not in the list always return 0xFF
  if (McaInstanceIdList == NULL) {
    return INSTANCEID_NOT_FOUND;
  }

  Index = 0;
  while (McaInstanceIdList[Index].InstanceID != MCA_INSTANCE_ID_END) {
    if (McaInstanceIdList[Index].InstanceID == McaIpidMsr.Field.InstanceId) {
      return McaInstanceIdList[Index].NumberOfInstance;
    }
    Index++;
  }

  //Instance ID no match
  return INSTANCEID_NOT_FOUND;
}

EFI_STATUS
AcquireNbioSecBusNum (
  IN       UINT8 DfNode,
  IN       UINT8 NbioNumber,
  OUT      UINT8 *NbioSecBusNum
  )
{
  UINTN                             PciAddress;
  UINT16                            VendorID;

  if (MAX_NBIO_PER_DIE > NbioNumber)  {
    PciAddress = ((DfNode + 24) << 15) + 0x000;
    VendorID = PciRead16 (PciAddress);
    if (VendorID != AMD_VENDOR_ID) {
      return EFI_INVALID_PARAMETER;
    }
    *NbioSecBusNum = (UINT8)RasGetDfReg(SspIomsInstanceIds[NbioNumber], DfNode, DF_CFGADDRESSCNTL);
    DEBUG ((EFI_D_ERROR, "Bus Number of the DfNode%d, IO Link%d: 0x%x\n", DfNode, NbioNumber, *NbioSecBusNum));
    return EFI_SUCCESS;
  }

  return EFI_INVALID_PARAMETER;
}

/*------------------------------------------------------------------
 Function: RasGetDfReg
 Purpose: Get a DF register
 Inputs: An instanceID (-1 for a broadcast read),
   a nodeID (for the PCIe bus/device),
   and a register address (function in bits 12:10)
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
RasGetDfReg (
  UINT32 instanceId,
  UINT32 nodeId,
  UINT32 regAddr
  )
{
  UINT32                               RegisterValue;
  PCI_ADDR                             PciAddr;
  FABRIC_IND_CFG_ACCESS_ADDR_REGISTER  FICAA3;
  UINT32                               Ficaa3Save;

  PciAddr.AddressValue = 0;
  PciAddr.Address.Device = nodeId + 0x18;   // 0x18 = PCIE device number of socket 0 die 0

  FICAA3.Value = 0;   // Variable initiate
  if ((instanceId & 0xFF) != FABRIC_REG_ACC_BC) {
    FICAA3.Field.CfgRegInstAccEn = 1;
  }

  PciAddr.Address.Function = FICAA3_FUNC;
  PciAddr.Address.Register = FICAA3_REG;

  //Save FICAA3 register value
  Ficaa3Save = PciRead32 (PciAddr.AddressValue);

  FICAA3.Field.IndCfgAccRegNum = ((UINT32) (regAddr & 0x3ff)) >> 2;
  FICAA3.Field.IndCfgAccFuncNum = ((UINT32) (regAddr >> 10 )) & 7;
  FICAA3.Field.CfgRegInstID = (UINT32) instanceId;
  PciWrite32 (PciAddr.AddressValue, FICAA3.Value);

  PciAddr.Address.Function = FICAD3_LO_FUNC;
  PciAddr.Address.Register = FICAD3_LO_REG;
  RegisterValue = PciRead32 (PciAddr.AddressValue);

  //Restore FICAA3 register value
  PciAddr.Address.Function = FICAA3_FUNC;
  PciAddr.Address.Register = FICAA3_REG;
  PciWrite32 (PciAddr.AddressValue, Ficaa3Save);

  return RegisterValue;
}

/*------------------------------------------------------------------
 Function: RasSetDfReg
 Purpose: Get a DF register
 Inputs: An instanceID (-1 for a broadcast read),
         a nodeID (for the PCIe bus/device),
         a register address (function in bits 12:10)
         and value write to the register
 Outputs: The value written to the DF register.
 *------------------------------------------------------------------*/
UINT32
RasSetDfReg (
  UINT32 instanceId,
  UINT32 nodeId,
  UINT32 regAddr,
  UINT32 regData
  )
{
  UINT32                               RegisterValue;
  PCI_ADDR                             PciAddr;
  FABRIC_IND_CFG_ACCESS_ADDR_REGISTER  FICAA3;
  UINT32                               Ficaa3Save;

  PciAddr.AddressValue = 0;
  PciAddr.Address.Device = nodeId + 0x18;   // 0x18 = PCIE device number of socket 0 die 0

  FICAA3.Value = 0;   // Variable initiate
  if ((instanceId & 0xFF) != FABRIC_REG_ACC_BC) {
    FICAA3.Field.CfgRegInstAccEn = 1;
  }

  PciAddr.Address.Function = FICAA3_FUNC;
  PciAddr.Address.Register = FICAA3_REG;

  //Save FICAA3 register value
  Ficaa3Save = PciRead32 (PciAddr.AddressValue);

  FICAA3.Field.IndCfgAccRegNum = ((UINT32) (regAddr & 0x3ff)) >> 2;
  FICAA3.Field.IndCfgAccFuncNum = ((UINT32) (regAddr >> 10 )) & 7;
  FICAA3.Field.CfgRegInstID = (UINT32) instanceId;
  PciWrite32 (PciAddr.AddressValue, FICAA3.Value);

  PciAddr.Address.Function = FICAD3_LO_FUNC;
  PciAddr.Address.Register = FICAD3_LO_REG;
  RegisterValue = PciWrite32 (PciAddr.AddressValue, regData);

  //Restore FICAA3 register value
  PciAddr.Address.Function = FICAA3_FUNC;
  PciAddr.Address.Register = FICAA3_REG;
  PciWrite32 (PciAddr.AddressValue, Ficaa3Save);

  return RegisterValue;
}

UINT64
ConvertToMilliseconds (
  IN       UINT64 TscTicks
  )
{
  UINT32      CpuFid;
  UINT32      CpuDfsId;
  UINT64      TscRateInMhz;
  UINT64      NumOfTicksPerMilliSec;
  PSTATE_MSR  PstateMsr;

  TscRateInMhz = 0;

  // The TSC increments at the rate specified by the P0 Pstate.
  // Core current operating frequency in MHz (CoreCOF) =
  //   (Core::X86::Msr::PStateDef[CpuFid[7:0]]/Core::X86::Msr::PStateDef[CpuDfsId])*200.
  PstateMsr.Value = AsmReadMsr64 (MSR_PSTATE_0);
  CpuFid   = (UINT32) PstateMsr.Field.CpuFid_7_0;
  CpuDfsId = (UINT32) PstateMsr.Field.CpuDfsId;
  if (CpuDfsId == 0) {
    TscRateInMhz = 0;
    ASSERT (FALSE);
  } else if ((CpuDfsId >= 8) && (CpuDfsId <= 0x30)) {
    TscRateInMhz = (UINTN) ((200 * CpuFid) / CpuDfsId);
  }
  if (TscRateInMhz == 0) {
    NumOfTicksPerMilliSec = 1000;
    ASSERT (FALSE);
  } else {
    NumOfTicksPerMilliSec = 1000 * TscRateInMhz;
  }

  return (TscTicks/NumOfTicksPerMilliSec);
}

BOOLEAN
NbioSearchErr (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  )
{
  UINT32 Index;
  UINT32 NbioParityErrorSts;
  UINT32 BaseAddr = 0;
  UINT32 MaxGroupId = 0;

  switch (RasNbioErrorInfo->TypeId) {
  case NBIO_GRP_UNCORR:
    BaseAddr = PARITY_ERROR_STATUS_UNCORR_GRP;
    MaxGroupId = MAX_UNCORR_GRP_NUM;
    break;
  case NBIO_GRP_CORR:
    BaseAddr = PARITY_ERROR_STATUS_CORR_GRP;
    MaxGroupId = MAX_CORR_GRP_NUM;
    break;
  case NBIO_GRP_UCP:
    BaseAddr = PARITY_ERROR_STATUS_UCP_GRP;
    MaxGroupId = MAX_UCP_GRP_NUM;
  }

  for (Index = 0; Index < MaxGroupId; Index++) {
    RasSmnRead ((UINT32)RasNbioErrorInfo->DieBusNumber,
                BaseAddr + (Index * 4),
                &NbioParityErrorSts);
    if (NbioParityErrorSts != 0) {
      RasNbioErrorInfo->GroupId = (UINT8)Index;
      RasNbioErrorInfo->NbioParityErrorStsAddr = BaseAddr + (Index * 4);
      RasNbioErrorInfo->NbioParityErrorSts = NbioParityErrorSts;
      return TRUE;
    }
  }
  return FALSE;
}


EFI_STATUS
NbioClrStatus (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  )
{
  RasSmnWrite ((UINT32)RasNbioErrorInfo->DieBusNumber,
               RasNbioErrorInfo->NbioParityErrorStsAddr,
               &RasNbioErrorInfo->NbioParityErrorSts);

  return EFI_SUCCESS;
}


EFI_STATUS
NbioResetCounter (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  )
{
  UINT32        ParityCounterAddr = 0;
  UINT32        MaxGroupId = 0;
  UINT32        ParityCounterReset;

  switch (RasNbioErrorInfo->TypeId) {
  case NBIO_GRP_CORR:
    ParityCounterAddr = PARITY_COUNTER_CORR_GRP;
    MaxGroupId = MAX_CORR_GRP_NUM;
    break;
  case NBIO_GRP_UCP:
    ParityCounterAddr = PARITY_COUNTER_UCP_GRP;
    MaxGroupId = MAX_UCP_GRP_NUM;
  }

  if (RasNbioErrorInfo->GroupId > MaxGroupId) {
    return EFI_INVALID_PARAMETER;
  }

  ParityCounterAddr += (RasNbioErrorInfo->GroupId * 4);
  ParityCounterReset = BIT31;

  RasSmnWrite ((UINT32)RasNbioErrorInfo->DieBusNumber,
               ParityCounterAddr,
               &ParityCounterReset);

  return EFI_SUCCESS;
}

VOID
RasGetMcaSeverityType(
  IN       RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  IN       UINT8                 BankIndex,
  OUT      UINT8                 *McaSeverityType
)
{
  MCA_STATUS_MSR    McaStatusMsr;

  McaStatusMsr.Value = RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Value;

  *McaSeverityType = ERROR_NONE;
  if (McaStatusMsr.Field.SyndV) {
    if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaSyndMsr.Field.ErrorPriority == 2) {
      *McaSeverityType = ERROR_SEVERITY_CORRECTED;
    } else if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaSyndMsr.Field.ErrorPriority == 3) {
      *McaSeverityType = ERROR_RECOVERABLE;
    } else if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaSyndMsr.Field.ErrorPriority > 3) {
      *McaSeverityType = ERROR_SEVERITY_FATAL;
    }
  } else {
    *McaSeverityType = ERROR_SEVERITY_CORRECTED;
    if (McaStatusMsr.Field.Deferred) {
      *McaSeverityType = ERROR_RECOVERABLE;
    }
    if (McaStatusMsr.Field.UC) {
      *McaSeverityType = ERROR_RECOVERABLE;
    }
    if (McaStatusMsr.Field.PCC) {
      *McaSeverityType = ERROR_SEVERITY_FATAL;
    }
  }
}

BOOLEAN
RasRetry (
  IN OUT   UINT32    *RetryCnt
  )
{
  if (*RetryCnt == 0) {
    return TRUE;
  } else {
    (*RetryCnt)--;
    return FALSE;
  }
}

EFI_STATUS
UpdateMemErrFruText (
     OUT   CHAR8*                FruTextDest,
  IN       UINT16                Node,
  IN       UINT16                Card,
  IN       UINT16                Module,
  IN       OEM_MEMORY_MAP_TABLE  *RasOemDimmMap
)
{
  CONST CHAR8* MemErrFruDfltStr;
  CONST CHAR8* MemErrFruTextOemPrefixStr;
  UINTN MemErrFruDfltStrSize;
  UINTN MemErrFruTextOemPrefixStrSize;
  UINT32 TblIndex;

  if (RasOemDimmMap == NULL) {
    //No Oem platform memory silscreen map data, use default string
    MemErrFruDfltStr = PcdGetPtr (PcdMemErrFruDefaultStr);
    MemErrFruDfltStrSize = AsciiStrSize (MemErrFruDfltStr);
    DEBUG ((EFI_D_ERROR, "[RAS]Memory Error Fru Text default string size %d\n", MemErrFruDfltStrSize));
    if (GENERIC_ERROR_DATA_ENTRY_FRUTEXT_SIZE < MemErrFruDfltStrSize) {
      AsciiStrCpyS (FruTextDest, 20, "DIMM# Sourced");       //Use hardcode string
      return EFI_SUCCESS;
    }
    // Use Pcd string
    AsciiStrCpyS (FruTextDest, 20, MemErrFruDfltStr);
    return EFI_SUCCESS;
  }

  MemErrFruTextOemPrefixStr = PcdGetPtr (PcdMemErrFruTextOemPrefixStr);
  MemErrFruTextOemPrefixStrSize = AsciiStrSize (MemErrFruTextOemPrefixStr);
  if (MEM_ERROR_MXA_PREFIX_FRUTEXT_SIZE < MemErrFruTextOemPrefixStrSize) {
    DEBUG ((EFI_D_ERROR, "[RAS]Memory Error Fru Text Prefix string over %d bytes\n", MEM_ERROR_MXA_PREFIX_FRUTEXT_SIZE));
    AsciiStrCpyS (FruTextDest, 20, "DIMM# Sourced");       //Use hardcode string
    return EFI_SUCCESS;
  }

  DEBUG ((EFI_D_ERROR, "[RAS]Memory Error Fru Text Oem Prefix string size %d\n", MemErrFruTextOemPrefixStrSize));

  //Copy pre-fix string into FruText field.
  AsciiStrCpyS (FruTextDest, 20, MemErrFruTextOemPrefixStr);

  for (TblIndex = 0; TblIndex < RasOemDimmMap->TableEntryNum; TblIndex++) {
    if ((RasOemDimmMap->MemoryMapTableEntry[TblIndex].Node == Node) &&
        (RasOemDimmMap->MemoryMapTableEntry[TblIndex].Card == Card) &&
        (RasOemDimmMap->MemoryMapTableEntry[TblIndex].Module == Module)) {
      //Overwrite prefix string null-terminal byte
      CopyMem (&FruTextDest[MemErrFruTextOemPrefixStrSize-1],
                    &RasOemDimmMap->MemoryMapTableEntry[TblIndex].FruText[0],
                    MEM_ERROR_MXA_SILKSCREEN_TEXT_SIZE);
      //Make sure there is a NULL Terminal in the end of string
      FruTextDest[MemErrFruTextOemPrefixStrSize + MEM_ERROR_MXA_SILKSCREEN_TEXT_SIZE -1] = '\0';
      return EFI_SUCCESS;
    }
  }

  DEBUG ((EFI_D_ERROR, "[RAS]OEM silkscreen text not found!!\n"));
  AsciiStrCpyS (&FruTextDest[MemErrFruTextOemPrefixStrSize-1], 20, "None");

  return EFI_SUCCESS;
}

VOID
STATIC
PcieDevCntlEnableOnFunction (
  IN       PCI_ADDR              Function,
  IN       PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  )
{
  UINT8       PcieCapPtr;
  UINT16      PcieDevCtrl;
  UINT16      AerCapPtr;

  AerCapPtr = RasFindPcieExtendedCapability (Function.AddressValue, PCIE_EXT_AER_CAP_ID, 0xFFFF);
  if (AerCapPtr != 0) {
    // Uncorrectable Error Mask Register (Offset 08h)
    PciWrite32(Function.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET, PcieAerSetting->UncorrectableMask);
    DEBUG ((EFI_D_ERROR, "  Uncorrectable Error Mask Register = 0x%08x\n",
    PciRead32(Function.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET)));

    // Uncorrectable Error Severity Register (Offset 0Ch)
    PciWrite32(Function.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET, PcieAerSetting->UncorrectableSeverity);
    DEBUG ((EFI_D_ERROR, "  Uncorrectable Error Severity Register = 0x%08x\n",
    PciRead32(Function.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET)));

    // Correctable Error Mask Register (Offset 14h)
    PciWrite32(Function.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET, PcieAerSetting->CorrectableMask);
    DEBUG ((EFI_D_ERROR, "  Correctable Error Mask Register = 0x%08x\n",
    PciRead32(Function.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET)));
  }

  PcieCapPtr = RasFindPciCapability (Function.AddressValue, PCIE_CAP_ID);
  if (PcieCapPtr != 0) {
     PcieDevCtrl = PciRead16 (Function.AddressValue + PcieCapPtr + PCIE_DEVICE_CONTROL_PTR);
     PcieDevCtrl |= PCIE_DEV_CORR_ERR + PCIE_DEV_NON_FATAL_ERR + PCIE_DEV_FATAL_ERR;
     PciWrite16(Function.AddressValue + PcieCapPtr + PCIE_DEVICE_CONTROL_PTR, PcieDevCtrl);
  }
}

VOID
STATIC
PcieDevCntlEnableOnDevice (
  IN       PCI_ADDR              Device,
  IN       PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  )
{
  UINT8                   MaxFunc;
  UINT8                   CurrentFunc;

  MaxFunc = RasPciIsMultiFunctionDevice (Device.AddressValue) ? 7 : 0;
  for (CurrentFunc = 0; CurrentFunc <= MaxFunc; CurrentFunc++) {
    Device.Address.Function = CurrentFunc;
    DEBUG ((EFI_D_ERROR, "  Checking Device: %d:%d:%d\n",
            Device.Address.Bus,
            Device.Address.Device,
            Device.Address.Function
            ));
    if (RasPciIsDevicePresent (Device.AddressValue)) {
      DEBUG ((EFI_D_ERROR, "  Enable Device Error report for Device = %d:%d:%d\n",
      Device.Address.Bus,
      Device.Address.Device,
      Device.Address.Function
      ));
      PcieDevCntlEnableOnFunction (Device, PcieAerSetting);
    }
  }
}

SCAN_STATUS
STATIC
PcieDevCntlCallback (
  IN       PCI_ADDR             Device,
  IN OUT   RAS_PCI_SCAN_DATA    *ScanData
  )
{
  SCAN_STATUS             ScanStatus;
  PCIE_DEVICE_TYPE        DeviceType;
  ScanStatus = SCAN_SUCCESS;
  DEBUG ((EFI_D_ERROR, "  PcieDevCntlCallback for Device = %d:%d:%d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    ));
  ScanStatus = SCAN_SUCCESS;
  DeviceType = RasGetPcieDeviceType (Device);
  DEBUG ((EFI_D_ERROR, "  PCI-E device type = 0x%x\n", DeviceType));
  switch (DeviceType) {
  case  PcieDeviceRootComplex:
  case  PcieDeviceDownstreamPort:
  case  PcieDeviceUpstreamPort:
    DEBUG ((EFI_D_ERROR, "  PCI-E device root port found\n"));
    ScanStatus = SCAN_SKIP_FUNCTIONS;
    break;
  case  PcieDevicePcieToPcix:
    DEBUG ((EFI_D_ERROR, "  PCI-E device PCIE to PCIx found\n"));
    ScanStatus = SCAN_SKIP_FUNCTIONS;
    break;
  case  PcieDeviceEndPoint:
  case  PcieDeviceLegacyEndPoint:
    DEBUG ((EFI_D_ERROR, "  PCI-E endpoint found\n"));
    PcieDevCntlEnableOnDevice (Device, (PCIe_AER_CONFIG_TEMP*)ScanData->Buffer);
    ScanStatus = SCAN_SKIP_FUNCTIONS;
    break;
  default:
    break;
  }
  return ScanStatus;
}

VOID
STATIC
PcieDevCntlConfigure (
  IN       PCI_ADDR  DownstreamPort,
  IN       PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  )
{
  RAS_PCI_SCAN_DATA  ScanData;
  ScanData.Buffer = PcieAerSetting;
  ScanData.RasScanCallback = PcieDevCntlCallback;
  RasPciScanSecondaryBus (DownstreamPort, &ScanData);
}

EFI_STATUS
EnDevErrReport(
  PCI_ADDR  PciPortAddr,
  PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  )
{
  UINT16      PcieDevCtrlOr;
  UINT16      PciBrdgeCtrlOr;
  UINT8       PcieCapPtr;

  //Get active port PCI-E cap pointer
  PcieCapPtr = RasFindPciCapability (PciPortAddr.AddressValue, PCIE_CAP_ID);

  if (PcieCapPtr != 0) {
    //Enable down link PCI-E end devices.
    PcieDevCntlConfigure(PciPortAddr, PcieAerSetting);

    //Enable active port dev_cntl and bridge control register.
    PcieDevCtrlOr = PCIE_DEV_CORR_ERR + PCIE_DEV_NON_FATAL_ERR + PCIE_DEV_FATAL_ERR;
    PciOr16(PciPortAddr.AddressValue + PcieCapPtr + PCIE_DEVICE_CONTROL_PTR, PcieDevCtrlOr);

    PciBrdgeCtrlOr = PCI_BRIDGECTRL_SERR_EN;
    PciOr16(PciPortAddr.AddressValue + PCI_BRIDGE_CONTROL_REG, PciBrdgeCtrlOr);
  }

  return EFI_SUCCESS;
}

BOOLEAN
IsHotPlugRp (
  PCI_ADDR  PciPortAddr
  )
{
  UINT8       PcieCapPtr;
  UINT32      Data32;
  BOOLEAN     IsHotPlugRp;

  IsHotPlugRp = FALSE;
  PcieCapPtr = RasFindPciCapability (PciPortAddr.AddressValue, PCIE_CAP_ID);
  if (PcieCapPtr != 0) {
    Data32 = PciRead32 (PciPortAddr.AddressValue + PcieCapPtr + PCIE_SLOT_CAP_REGISTER);
    if (Data32 & BIT6) { //BIT6: Hot-Plug Capable
      IsHotPlugRp = TRUE;
      DEBUG ((EFI_D_ERROR, "  It's a hot-plug capable root port.\n"));
    }
  }

  return IsHotPlugRp;
}

//WA
EFI_STATUS
RasSetPcieAerFeature (
  IN VOID                          *PciePortAerConfigStrucPtr
  )
{
  PCI_ADDR                     PciPortAddr;
  UINT16                       AerCapPtr;
  PCIe_AER_CONFIG_TEMP    *PcieAerSetting;

  PcieAerSetting = (PCIe_AER_CONFIG_TEMP *) PciePortAerConfigStrucPtr;

  if (PcieAerSetting->AerEnable == 1) {
    PciPortAddr.AddressValue = 0;
    PciPortAddr.Address.Bus = PcieAerSetting->PciBus;
    PciPortAddr.Address.Device = PcieAerSetting->PciDev;
    PciPortAddr.Address.Function = PcieAerSetting->PciFunc;

    DEBUG ((EFI_D_ERROR, "  Set AER Feature bits - 0x%02x: 0x%02x: 0x%02x, (0x%08x)\n",
      PciPortAddr.Address.Bus, PciPortAddr.Address.Device, PciPortAddr.Address.Function, PciPortAddr.AddressValue));

    AerCapPtr = RasFindPcieExtendedCapability (PciPortAddr.AddressValue, PCIE_EXT_AER_CAP_ID, 0xFFFF);
    if (AerCapPtr != 0) {
      if (IsHotPlugRp(PciPortAddr)) {
        // The processor does not support the optional Surprise Down feature of AER.
        PcieAerSetting->UncorrectableMask |= (BIT5 + BIT14); //BIT5: Surprise Down Error Mask, BIT14: Completion Timeout Mask
      }
      // Uncorrectable Error Mask Register (Offset 08h)
      PciWrite32(PciPortAddr.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET, PcieAerSetting->UncorrectableMask);
      DEBUG ((EFI_D_ERROR, "  Uncorrectable Error Mask Register = 0x%08x\n",
        PciRead32(PciPortAddr.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET)));

      // Uncorrectable Error Severity Register (Offset 0Ch)
      PciWrite32(PciPortAddr.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET, PcieAerSetting->UncorrectableSeverity);
      DEBUG ((EFI_D_ERROR, "  Uncorrectable Error Severity Register = 0x%08x\n",
        PciRead32(PciPortAddr.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET)));

      // Correctable Error Mask Register (Offset 14h)
      PciWrite32(PciPortAddr.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET, PcieAerSetting->CorrectableMask);
      DEBUG ((EFI_D_ERROR, "  Correctable Error Mask Register = 0x%08x\n",
        PciRead32(PciPortAddr.AddressValue + AerCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET)));
    }
  }

  return EFI_SUCCESS;
}

UINT8
RasBitPositionToInt (
  UINT32       BitMap
)
{
  UINT64   MapIdx;
  UINT8    BitPosition;

  for (MapIdx = 1,  BitPosition = 0; MapIdx < 0x100000000; MapIdx <<= 1,  BitPosition++) {
    if ((BitMap & MapIdx) != 0) {
      return BitPosition;
    }
  }
  return INVALID_BYTE_RETURN;
}
