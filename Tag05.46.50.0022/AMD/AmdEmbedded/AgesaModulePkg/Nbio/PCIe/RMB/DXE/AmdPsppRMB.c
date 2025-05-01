/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <GnbDxio.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <Library/PcieConfigLib.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/GnbPciLib.h>

#include "AmdNbioPcieDxe.h"

#include <AmdPcieComplex.h>

#define FILECODE        NBIO_PCIE_RMB_DXE_AMDPSPPRMB_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


UINT16 DefaultSmartDeviceVendorList[] = {
  0x1002,
  0x1022,
  0x10DE,
  0xFFFF
};

VEN_DEV_ID DefaultBlacklistDeviceList[] = {
  {0x8086, 0x2526},
  {0x14C3, 0x7922},
  {0x14C3, 0x0616},
  {0xFFFF, 0xFFFF}
};

VEN_DEV_ID DefaultSmartDeviceVendorList2[] = {
  {0x10EC, 0x5261},
  {0x126F, 0x2261},
  {0x17A0, 0x9767},
  {0x1217, 0x8621},
  {0xFFFF, 0xFFFF}
};

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * Callback to init A+A feature to SMU
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

UINT32
STATIC
GetPsppDeviceType (
  IN       UINT16                VendorId,
  IN       UINT16                DeviceId
  )
{
  UINT32      PsppDeviceType;
  UINT32      Index;

  PsppDeviceType = NonSmartDevice;

  Index = 0;
  while (DefaultSmartDeviceVendorList[Index] != 0xFFFF) {
    if (DefaultSmartDeviceVendorList[Index] == VendorId) {
      return SmartDevice;
    }
    Index++;
  }
  Index = 0;
  while (DefaultBlacklistDeviceList[Index].Value != 0xFFFFFFFF) {
    if ((DefaultBlacklistDeviceList[Index].Field.VendorId == VendorId) &&
       (DefaultBlacklistDeviceList[Index].Field.DeviceId == DeviceId)) {
      return DumbDevice;
    }
    Index++;
  }
  Index = 0;
  while (DefaultSmartDeviceVendorList2[Index].Value != 0xFFFFFFFF) {
    if ((DefaultSmartDeviceVendorList2[Index].Field.VendorId == VendorId) &&
       (DefaultSmartDeviceVendorList2[Index].Field.DeviceId == DeviceId)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "SmartDeviceVendorList2\n");
      return SmartDevice;
    }
    Index++;
  }
  return PsppDeviceType;
}


/**
 * Callback to init A+A feature to SMU
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
AmdPsppCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT32                        SecondaryAddress;
  PCI_ADDR                      DeviceAddress;
  UINT32                        VendorDeviceId;
  UINT16                        DeviceId;
  UINT16                        VendorId;
  UINT32                        DeviceType;
  GNB_HANDLE                    *NbioHandle;
  UINT32                        SmuArg[6];
  UINT8                         PsppMode;
  UINT32                        PsppDeviceType;
  UINT32                        DeviceFunction;
  UINT32                        TargetSpeed;
  UINT32                        PciAddress = 0;
  UINT8                         PMData8 = 0;
  UINT8                         OrgPMData8 = 0;
  UINT32                        PciAddress2 = 0;
  UINT8                         ASPMData8 = 0;
  UINT8                         OrgAspmData8 = 0;
  UINT32                        PciAddress3 = 0;
  UINT8                         CurrentLinkSpeed8 = 0;
  UINT8                         WatchDogCounter = 0;
  UINT8                         PcieCapPtr = 0;
  UINT16                        ParentCapPtr = 0;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  PSPP_Policy_STRUCT            PsppSetting;
  PCIE_LC_SPEED_CNTL_PCIEPORT_STRUCT  LcSpeedCntl;

  // DXIO PSPP modes do not match AGESA definitions.  Translate the value to DXIO values.
  switch (PcdGet8 (PcdPsppPolicy)) {
  case AgesaPsppPerformance:
    PsppMode = DxioPsppPerformance;
    break;
  case AgesaPsppBalance:
    PsppMode = DxioPsppBalanced;
    break;
  case AgesaPsppPowerSaving:
    PsppMode = DxioPsppPowerSaving;
    break;
  default:
    return;
  }

  Wrapper = PcieConfigGetParentWrapper(Engine);
  NbioHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);

  GnbLibPciRead (Engine->Type.Port.Address.AddressValue | 0x18, AccessWidth32, &SecondaryAddress, (AMD_CONFIG_PARAMS *) NULL);
  DeviceAddress.AddressValue = 0;
  DeviceAddress.Address.Bus = (SecondaryAddress >> 8) & 0xFF;
  IDS_HDT_CONSOLE (GNB_TRACE, "Checking secondary bus 0x%x\n", SecondaryAddress);

  DeviceFunction = (Engine->Type.Port.Address.Address.Device << 3) + Engine->Type.Port.Address.Address.Function;
  if ((DeviceAddress.Address.Bus != 0xFF) && (DeviceAddress.Address.Bus != 0x0)) {
    GnbLibPciRead (DeviceAddress.AddressValue, AccessWidth32, &VendorDeviceId, (AMD_CONFIG_PARAMS *)NULL);
    GnbLibPciRead (DeviceAddress.AddressValue | 0x08, AccessWidth32, &DeviceType, (AMD_CONFIG_PARAMS *) NULL);
    VendorId = (UINT16) (VendorDeviceId & 0xFFFF);
    DeviceId = (UINT16) (VendorDeviceId >> 16);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Read device info on bus %d Vendor:%x Device %x and DeviceType\n", DeviceAddress.Address.Bus, VendorId, DeviceId, DeviceType >> 24);
    IDS_HDT_CONSOLE (GNB_TRACE, "PsppPolicyDC  %x \n", Engine->Type.Port.PsppPolicyDC);
    IDS_HDT_CONSOLE (GNB_TRACE, "PsppPolicyAC  %x \n", Engine->Type.Port.PsppPolicyAC);

    PsppDeviceType = GetPsppDeviceType (VendorId, DeviceId);
    Engine->Type.Port.PsppDeviceType =  (UINT8) PsppDeviceType;

    IDS_HOOK (IDS_HOOK_NBIO_PSPP_PORT_TUNING, (VOID *)NbioHandle, (VOID *)Engine);

    PsppSetting.Field.DC_MinPreferredSpeed = (UINT8) (Engine->Type.Port.PsppPolicyDC & 0xF);
    PsppSetting.Field.DC_MaxPreferredSpeed = (UINT8) ((Engine->Type.Port.PsppPolicyDC & 0xF0) >> 4);
    PsppSetting.Field.DC_DirectIndirect = (UINT8) ((Engine->Type.Port.PsppPolicyDC) >> 8);
    PsppSetting.Field.AC_MinPreferredSpeed = (UINT8) (Engine->Type.Port.PsppPolicyAC & 0xF);
    PsppSetting.Field.AC_MaxPreferredSpeed = (UINT8) ((Engine->Type.Port.PsppPolicyAC & 0xF0) >> 4);
    PsppSetting.Field.AC_DirectIndirect = (UINT8) ((Engine->Type.Port.PsppPolicyAC) >> 8);

    IDS_HDT_CONSOLE (GNB_TRACE, "  PsppSetting.Field.DC_MinPreferredSpeed %x and DeviceType\n", PsppSetting.Field.DC_MinPreferredSpeed);
    IDS_HDT_CONSOLE (GNB_TRACE, "  PsppSetting.Field.DC_MaxPreferredSpeed %x and DeviceType\n", PsppSetting.Field.DC_MaxPreferredSpeed);
    IDS_HDT_CONSOLE (GNB_TRACE, "  PsppSetting.Field.DC_DirectIndirect %x and DeviceType\n", PsppSetting.Field.DC_DirectIndirect);
    IDS_HDT_CONSOLE (GNB_TRACE, "  PsppSetting.Field.AC_MinPreferredSpeed %x and DeviceType\n", PsppSetting.Field.AC_MinPreferredSpeed);
    IDS_HDT_CONSOLE (GNB_TRACE, "  PsppSetting.Field.AC_MaxPreferredSpeed %x and DeviceType\n", PsppSetting.Field.AC_MaxPreferredSpeed);
    IDS_HDT_CONSOLE (GNB_TRACE, "  PsppSetting.Field.AC_DirectIndirect %x and DeviceType\n", PsppSetting.Field.AC_DirectIndirect);

    // skip PSPP policy setting
    if (PsppDeviceType != SmartDevice) {
      // Skip PSPP to support DB5 feature
      if (!((VendorId == 0x14C3 && DeviceId == 0x7922) ||
            (VendorId == 0x14C3 && DeviceId == 0x0616))) {
        LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);

        SmuArg[1] = DeviceFunction;
        SmuArg[2] = Engine->Type.Port.PsppDeviceType;
        SmuArg[3] = PsppMode;
        SmuArg[4] = PsppSetting.Value;
        NbioDxioServiceRequestV13 (NbioHandle, DXIO_MSG_RNTM_CHANGESPEEDPERPSPPPOLICY, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
      }
    } else {
      // Train to Gen 4
      LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
      SmuArg[1] = DxioPcieEngine;
      SmuArg[2] = DxioGen4;
      SmuArg[3] = Engine->Type.Port.PcieBridgeId;
      NbioDxioServiceRequestV13 (NbioHandle, DXIO_MSG_PHYS_TARGETFREQUPDATE | (DXIO_freqUpdType_single << 10), SmuArg, GNB_REG_ACC_FLAG_S3SAVE);

      Engine->Type.Port.PsppDeviceType = DumbDevice;
    }

    if ((VendorId== 0x8086 && DeviceId == 0x2526) ||
        (VendorId== 0x14C3 && DeviceId == 0x7922) ||
        (VendorId== 0x14C3 && DeviceId == 0x0616)) {
      WatchDogCounter = 0;

      ParentCapPtr = GnbLibFindPciCapability (DeviceAddress.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
      if (ParentCapPtr != 0) {
        PciAddress = MAKE_SBDFO (0, DeviceAddress.Address.Bus, 0, 0, ParentCapPtr + 0x4);
        GnbLibPciRead (PciAddress, AccessWidth8, &PMData8, (AMD_CONFIG_PARAMS *) NULL);
        OrgPMData8 = PMData8;
      }

      PcieCapPtr = GnbLibFindPciCapability (DeviceAddress.AddressValue, PCIE_CAP_ID, NULL);
      if (PcieCapPtr != 0) {
        PciAddress2 = MAKE_SBDFO (0, DeviceAddress.Address.Bus, 0, 0, PcieCapPtr + 0x10);
        GnbLibPciRead (PciAddress2, AccessWidth8, &ASPMData8, (AMD_CONFIG_PARAMS *) NULL);

        OrgAspmData8 = ASPMData8;
        // Disable device ASPM as 0;
        ASPMData8 = ASPMData8 & 0xFFFFFFFC;
        GnbLibPciWrite (PciAddress2, AccessWidth8, &ASPMData8, (AMD_CONFIG_PARAMS *) NULL);

        LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
        SmuArg[1] = DxioPcieEngine;
        SmuArg[2] = DxioGen2;
        SmuArg[3] = Engine->Type.Port.PcieBridgeId;
        NbioDxioServiceRequestV13 (NbioHandle, DXIO_MSG_PHYS_TARGETFREQUPDATE | (DXIO_freqUpdType_single << 10), SmuArg, GNB_REG_ACC_FLAG_S3SAVE);

        GnbLibPciWrite (PciAddress2, AccessWidth8, &OrgAspmData8, (AMD_CONFIG_PARAMS *) NULL);
        GnbLibPciWrite (PciAddress, AccessWidth8, &OrgPMData8, (AMD_CONFIG_PARAMS *) NULL);

        IDS_HDT_CONSOLE (GNB_TRACE, "  Engine->Type.Port.PcieBridgeId: %x\n", Engine->Type.Port.PcieBridgeId);

        //check current link state

        PciAddress3 = MAKE_SBDFO (0, DeviceAddress.Address.Bus, 0, 0, PcieCapPtr + 0x12);
        GnbLibPciRead (PciAddress3, AccessWidth8, &CurrentLinkSpeed8, (AMD_CONFIG_PARAMS *) NULL);

        IDS_HDT_CONSOLE (GNB_TRACE, "  PciAddress3: %x\n", PciAddress3);

        IDS_HDT_CONSOLE (GNB_TRACE, "  CurrentLinkSpeed8: %x\n", CurrentLinkSpeed8);
        IDS_HDT_CONSOLE (GNB_TRACE, "  CurrentLinkSpeed8 & (0xF): %x\n", CurrentLinkSpeed8 & (0xF));

        while ( (CurrentLinkSpeed8 & (0xF)) != 0x2) {
          // Send again
          LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
          SmuArg[1] = DxioPcieEngine;
          SmuArg[2] = DxioGen2;
          SmuArg[3] = Engine->Type.Port.PcieBridgeId;
          NbioDxioServiceRequestV13 (NbioHandle, DXIO_MSG_PHYS_TARGETFREQUPDATE | (DXIO_freqUpdType_single << 10), SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
          WatchDogCounter++;
          GnbLibPciRead (PciAddress3, AccessWidth8, &CurrentLinkSpeed8, (AMD_CONFIG_PARAMS *) NULL);

          IDS_HDT_CONSOLE (GNB_TRACE, "  CurrentLinkSpeed8: %x\n", CurrentLinkSpeed8);
          IDS_HDT_CONSOLE (GNB_TRACE, "  CurrentLinkSpeed8 & (0xF): %x\n", CurrentLinkSpeed8 & (0xF));
          if (WatchDogCounter == 10) {
            IDS_HDT_CONSOLE (GNB_TRACE, "  WatchDogCounter: %x Change Gen Speed fail \n", WatchDogCounter);
            break;
          }
        }
      }
    }

    if ((Engine->Type.Port.PsppDeviceType != DumbDevice) && (PsppMode == DxioPsppBalanced)) {
      TargetSpeed = Engine->Type.Port.PortData.LinkSpeedCapability;
      if (TargetSpeed == DxioGenMaxSupported) {
        TargetSpeed = DxioGen4;
      }
      SmnRegisterRead (NbioHandle->Address.Address.Bus,
                       PORT_SPACE (NbioHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS),
                       &LcSpeedCntl.Value
                       );

      if (TargetSpeed == DxioGen4) {
        if (LcSpeedCntl.Field.LC_OTHER_SIDE_SUPPORTS_GEN4 == 0) {
          IDS_HDT_CONSOLE (GNB_TRACE, "  Other side does not support Gen4\n");
          TargetSpeed = DxioGen3;
        }
      }

      if (TargetSpeed == DxioGen3) {
        if (LcSpeedCntl.Field.LC_OTHER_SIDE_SUPPORTS_GEN3 == 0) {
          IDS_HDT_CONSOLE (GNB_TRACE, "  Other side does not support Gen3\n");
          TargetSpeed = DxioGen2;
        }
      }

      if (TargetSpeed == DxioGen2) {
        if (LcSpeedCntl.Field.LC_OTHER_SIDE_SUPPORTS_GEN2 == 0) {
          IDS_HDT_CONSOLE (GNB_TRACE, "  Other side does not support Gen2\n");
          TargetSpeed = DxioGen1;
        }
      }

      if (TargetSpeed != DxioGen1) {
        LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
        SmuArg[1] = DxioPcieEngine;
        SmuArg[2] = TargetSpeed;
        SmuArg[3] = Engine->Type.Port.PcieBridgeId;
        NbioDxioServiceRequestV13 (NbioHandle, DXIO_MSG_PHYS_TARGETFREQUPDATE | (DXIO_freqUpdType_single << 10), SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
      }
    }
  }
}


/**
 *---------------------------------------------------------------------------------------
 *  APlusAConfigV13
 *
 *  Description:
 *     Configure SMU for A Plus A Graphics Config
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
AmdPsppConfig (
  IN       PCIe_PLATFORM_CONFIG       *Pcie
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "AmdPsppConfig Entry\n");

  // Only need special config for Balanced Mode
  if (AgesaPsppDisabled != PcdGet8 (PcdPsppPolicy)) {
    PcieConfigRunProcForAllEngines (
        DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
        AmdPsppCallback,
        NULL,
        Pcie
        );
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "AmdPsppConfig Exit\n");
  return;
}



