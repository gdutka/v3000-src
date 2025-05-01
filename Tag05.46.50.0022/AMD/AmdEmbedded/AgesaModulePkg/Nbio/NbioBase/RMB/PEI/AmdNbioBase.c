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
 * AmdNbioBaseZPPei Driver
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
#include <AmdSoc.h>
#include <GnbDxio.h>
#include <IdsHookId.h>
#include <GnbRegistersRMB.h>

#include <Guid/GnbNbioBaseZPInfoHob.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Guid/AmdMemoryInfoHob.h>

#include <Ppi/NbioBaseServicesZPPpi.h>
#include <Ppi/AmdMemoryInfoHobPpi.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/SocLogicalIdPpi.h>
#include <Library/AmdBaseLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/NbioTableBlastLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioAzaliaLib.h>
#include <Library/AmdSocBaseLib.h>

#define FILECODE NBIO_NBIOBASE_RMB_PEI_AMDNBIOBASE_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA      GnbEarlyInitTable [];
extern GNB_TABLE ROMDATA      GnbEarlyInitB0Table [];


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define IOHCMISC_BARSIZE_512MB                0x20000000ul            ///< Size of 512MB BAR
#define IOHCMISC_BARSIZE_1MB                  0x100000ul              ///< Size of 1MB BAR
#define IOHCMISC_BARSIZE_512KB                0x7D000ul               ///< Size of 512KB BAR
#define IOHCMISC_BARSIZE_4KB                  0x1000                  ///< Size of 4KB BAR
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
DebugGetConfig (
  IN  PEI_AMD_NBIO_BASE_SERVICES_PPI  *This,
  OUT GNB_BUILD_OPTIONS_ZP_DATA_HOB   **DebugOptions
  );

EFI_STATUS
EFIAPI
MultiDieConfigureCallbackPpi (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR        *NotifyDescriptor,
  IN VOID                             *Ppi
  );

EFI_STATUS
EFIAPI
MemoryConfigDoneCallbackPpi (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR        *NotifyDescriptor,
  IN VOID                             *Ppi
  );

EFI_STATUS
EFIAPI
NbioTopologyConfigureCallbackPpi (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR        *NotifyDescriptor,
  IN VOID                             *Ppi
  );

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */

STATIC PEI_AMD_NBIO_BASE_SERVICES_PPI mNbioBaseServicePpi = {
  AMD_NBIO_BASE_SERVICES_REVISION,  ///< revision
  NULL,
  0,
  NULL
};

STATIC EFI_PEI_PPI_DESCRIPTOR mNbioBaseServicesPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioBaseServicesRmbPpiGuid,
  &mNbioBaseServicePpi
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR   mNotifyMemDonePpi = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gAmdMemoryInfoHobPpiGuid,
  MemoryConfigDoneCallbackPpi
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR   mNotifyNbioTopologyPpi = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gAmdNbioPcieServicesPpiGuid,
  NbioTopologyConfigureCallbackPpi
};
/*----------------------------------------------------------------------------------------*/
/**
 * DebugGetConfig PPI function
 *
 *
 *
 * @param[in]  This          PEI_AMD_NBIO_BASE_SERVICES_PPI pointer
 * @param[out] DebugOptions  Pointer to GNB_BUILD_OPTIONS_ZP_DATA_HOB pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
DebugGetConfig (
  IN       PEI_AMD_NBIO_BASE_SERVICES_PPI      *This,
     OUT   GNB_BUILD_OPTIONS_ZP_DATA_HOB       **DebugOptions
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_HOB_POINTERS           Hob;
  GNB_BUILD_OPTIONS_ZP_DATA_HOB  *GnbBuildOptionsDataHob;

  GnbBuildOptionsDataHob = NULL;

  Status = PeiServicesGetHobList (&Hob.Raw);

  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
        CompareGuid ( &Hob.Guid->Name, &gGnbNbioBaseZPHobInfoGuid)) {
      GnbBuildOptionsDataHob = (GNB_BUILD_OPTIONS_ZP_DATA_HOB *) (Hob.Raw);
      Status = EFI_SUCCESS;
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  *DebugOptions = GnbBuildOptionsDataHob;
  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Callback to initialize NBIO Base function
 *
 *
 *
 * @param[in] GnbHandle          GnbHandle
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
NbioBaseInitCallback (
  IN       GNB_HANDLE          *GnbHandle,
  IN       SOC_LOGICAL_ID      *LogicalId
  )
{
  UINT32 Value;

  //nBIF ReqID
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP7_ADDRESS),
                  (UINT32) ~(RCC_DEV0_PORT_STRAP7_STRAP_RP_BUSNUM_DEV0_MASK |
                             RCC_DEV0_PORT_STRAP7_STRAP_DN_DEVNUM_DEV0_MASK |
                             RCC_DEV0_PORT_STRAP7_STRAP_DN_FUNCID_DEV0_MASK),
                  (GnbHandle->Address.Address.Bus << RCC_DEV0_PORT_STRAP7_STRAP_RP_BUSNUM_DEV0_OFFSET) |
                  (0x8 << RCC_DEV0_PORT_STRAP7_STRAP_DN_DEVNUM_DEV0_OFFSET) |
                  (0x1 << RCC_DEV0_PORT_STRAP7_STRAP_DN_FUNCID_DEV0_OFFSET),
                  0
                  );

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP7_ADDRESS),
                  (UINT32) ~(RCC_DEV1_PORT_STRAP7_STRAP_RP_BUSNUM_DEV1_MASK |
                             RCC_DEV1_PORT_STRAP7_STRAP_DN_DEVNUM_DEV1_MASK |
                             RCC_DEV1_PORT_STRAP7_STRAP_DN_FUNCID_DEV1_MASK),
                  (GnbHandle->Address.Address.Bus << RCC_DEV1_PORT_STRAP7_STRAP_RP_BUSNUM_DEV1_OFFSET) |
                  (0x8 << RCC_DEV1_PORT_STRAP7_STRAP_DN_DEVNUM_DEV1_OFFSET) |
                  (0x2 << RCC_DEV1_PORT_STRAP7_STRAP_DN_FUNCID_DEV1_OFFSET),
                  0
                  );

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP7_ADDRESS),
                  (UINT32) ~(RCC_DEV2_PORT_STRAP7_STRAP_RP_BUSNUM_DEV2_MASK |
                             RCC_DEV2_PORT_STRAP7_STRAP_DN_DEVNUM_DEV2_MASK |
                             RCC_DEV2_PORT_STRAP7_STRAP_DN_FUNCID_DEV2_MASK),
                  (GnbHandle->Address.Address.Bus << RCC_DEV2_PORT_STRAP7_STRAP_RP_BUSNUM_DEV2_OFFSET) |
                  (0x8 << RCC_DEV2_PORT_STRAP7_STRAP_DN_DEVNUM_DEV2_OFFSET) |
                  (0x3 << RCC_DEV2_PORT_STRAP7_STRAP_DN_FUNCID_DEV2_OFFSET),
                  0
                  );

  if ((LogicalId->Family == AMD_FAMILY_RMB) && ((LogicalId->Revision & AMD_REV_F19_RMB_Ax) == 0)) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV2_EPF3_STRAP3_ADDRESS),
                    (UINT32) ~(RCC_DEV2_EPF3_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F3_MASK),
                    (0x1 << RCC_DEV2_EPF3_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F3_OFFSET),
                    0
                    );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV2_EPF4_STRAP3_ADDRESS),
                    (UINT32) ~(RCC_DEV2_EPF4_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F4_MASK),
                    (0x1 << RCC_DEV2_EPF4_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F4_OFFSET),
                    0
                    );
  }

  Value = 0x18;
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                    SMN_ParityFatal_ACTION_CONTROL_ADDRESS,
                    &Value,
                    AccessWidth32
                    );

  Value = 0x18;
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                    SMN_ErrEvent_ACTION_CONTROL_ADDRESS,
                    &Value,
                    AccessWidth32
                    );

  Value = 0;
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                    SMN_PARITY_CONTROL_0_ADDRESS,
                    &Value,
                    AccessWidth32
                    );

  return AGESA_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Callback to Enable IOAPIC on GNB
 *
 *
 *
 * @param[in] GnbHandle          GnbHandle
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
IoapicEnableCallback (
  IN       GNB_HANDLE          *GnbHandle
  )
{
  UINT32              Value;
  UINT64              GnbNbIoapicAddress;

  if (GnbHandle->InstanceId == 0) {
    GnbNbIoapicAddress = PcdGet64 (PcdCfgGnbIoapicAddress);
    if (GnbNbIoapicAddress != 0x0ull) {
      Value = (UINT32) RShiftU64 (GnbNbIoapicAddress, 32);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                             NBIO_SPACE(GnbHandle, SMN_IOAPIC_BASE_ADDR_HI_ADDRESS), &Value, 0);
      Value = GnbNbIoapicAddress & 0xFFFFFF00;
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                            NBIO_SPACE(GnbHandle, SMN_IOAPIC_BASE_ADDR_LO_ADDRESS), &Value, 0);
      Value |= 1;
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                           NBIO_SPACE(GnbHandle,SMN_IOAPIC_BASE_ADDR_LO_ADDRESS), &Value, 0);
    }
    SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_FEATURES_ENABLE_ADDRESS), &Value);
    Value |= 1 << 2;    //Ioapic_id_ext_en
    Value |= 1 << 4;    //Ioapic_sb_feature_en
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_FEATURES_ENABLE_ADDRESS), &Value, 0);
  } else {
    SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_FEATURES_ENABLE_ADDRESS), &Value);
    Value |= 1 << 2;    //Ioapic_id_ext_en
    Value |= 1 << 4;    //Ioapic_sb_feature_en
    Value |= 1 << 5;    //Ioapic_secondary_en
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_FEATURES_ENABLE_ADDRESS), &Value, 0);
  }
  Value = GnbHandle->Address.Address.Bus;
  Value |= 1 << 8;

  SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_NB_BUS_NUM_CNTL_ADDRESS), &Value, 0);
  return AGESA_SUCCESS;
}

VOID
STATIC
AudioIOConfigCallback (
  IN       GNB_HANDLE          *GnbHandle
  )
{
  UINT8  Value;
  UINT32 AcpPinCfg;
  UINT32 RegisterValue;
  UINT32 PackageType;

  // Get ACP_PIN_CONFIG
  PackageType = LibAmdGetPackageType ((AMD_CONFIG_PARAMS *) NULL);
  Value = PcdGet8 (PcdAmdNbioAudioSelect);
  AcpPinCfg = Value;
  if (((1 << ZEN3_PKG_AM5) == PackageType) ||
      ((1 << 3) == PackageType)) {
    // For AM5, default value is 4, and valid data is 4~8
    //   04 - HDA(3SDI) +  PDM(2CH) --- (Default)
    //   05 - HDA(1SDI) +  PDM(6CH)
    //   06 - HDA(1SDI) + SDW0(1MDATA) + PDM(2CH)
    //   07 - SDW0(4MDATA) + PDM(6CH)
    //   08 - SDW0(4MDATA) + SDW1(1MDATA) + PDM(2CH)
    AcpPinCfg = 4;
    if ((Value >= 4) && (Value <= 8)) {
      AcpPinCfg = Value;
    }
  } else if (((1 << ZEN3_PKG_FP7) == PackageType) ||
             ((1 << ZEN3_PKG_FP7r2) == PackageType) ||
             ((1 << ZEN3_PKG_FP7_B0) == PackageType) ||
             ((1 << ZEN3_PKG_FP7r2_B0) == PackageType)) {
    // For FP7/FP7r2, default value is 10, and valid data is 9~14
    //   09 - 3I2S PORTS + 1 REFCLK + 1 INTR
    //   10 - HDA(3SDI) +  PDM(6CH) +I2S --- (Default)
    //   11 - HDA(3SDI) +  PDM(8CH)
    //   12 - HDA(1SDI) + SDW0(1MDATA) + PDM(6CH) + I2S
    //   13 - SDW0(4MDATA)+ SDW1(1MDATA) + PDM(6CH) + I2S
    //   14 - SDW0(4MDATA) + SDW1(1MDATA)+ PDM(8CH)
    AcpPinCfg = 10;
    if ((Value >= 9) && (Value <= 14)) {
      AcpPinCfg = Value;
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "ACP_PIN_CONFIG Entry: %x\n", Value);

  SmnRegisterRMW  (GnbHandle->Address.Address.Bus,
                   SMN_ACP_PIN_CONFIG_ADDRESS,
                   (UINT32) ~(ACP_PIN_CONFIG_ACP_PIN_CONFIG_MASK),
                   AcpPinCfg << ACP_PIN_CONFIG_ACP_PIN_CONFIG_OFFSET,
                   0
                   );
  SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_ACP_PGFSM_STATUS_ADDRESS, &RegisterValue);
  IDS_HDT_CONSOLE (MAIN_FLOW, "ACP_PGFSM_STATUS %x\n", RegisterValue);
  if (RegisterValue & (BIT1 | BIT3)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Power on ACP\n");
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_ACP_PGFSM_CONTROL_ADDRESS,
                    (UINT32)~(ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_MASK),
                    0x1 << ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_OFFSET,
                    0
                    );
    while (TRUE) {
      SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_ACP_PGFSM_STATUS_ADDRESS, &RegisterValue);
      if (RegisterValue == 0x0) {
        break;
      }
    }

    /*not use ACP but only HDA, so keep ACP banks and caches in shut down mode while keeping ACP on*/
    //Clear ACP soft reset bit (ACP_SOFT_RESET)
    IDS_HDT_CONSOLE (MAIN_FLOW, "Clear ACP soft reset bit\n");
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_ACP_SOFT_RESET_ADDRESS,
                    (UINT32)~(ACP_SOFT_RESET_SoftResetAud_MASK),
                    0x0 << ACP_SOFT_RESET_SoftResetAud_OFFSET,
                    0
                    );
    while (TRUE) {
      SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_ACP_SOFT_RESET_ADDRESS, &RegisterValue);
      RegisterValue &= ACP_SOFT_RESET_SoftResetAudDone_MASK;
      if (RegisterValue == 0) {
        break;
      }
    }
    //Keep ACP memories in shutdown if HDA option not available
    // IDS_HDT_CONSOLE (MAIN_FLOW, "Keep ACP memories in shutdown\n");
    // RegisterValue = 0xC00FFFFF;
    // SmnRegisterWrite (GnbHandle->Address.Address.Bus, SMN_ACP_PGMEM_SHUT_DOWN_CTRL_ADDRESS, &RegisterValue, 0);
  }
  if (((AcpPinCfg >= 7) && (AcpPinCfg <= 9)) ||
      ((AcpPinCfg >= 13) && (AcpPinCfg <= 14))) {
    // For AM5
    //   07 - SDW0(4MDATA) + PDM(6CH)
    //   08 - SDW0(4MDATA) + SDW1(1MDATA) + PDM(2CH)
    // For FP7/FP7r2
    //   09 - 3I2S PORTS + 1 REFCLK + 1 INTR
    //   13 - SDW0(4MDATA)+ SDW1(1MDATA) + PDM(6CH) + I2S
    //   14 - SDW0(4MDATA) + SDW1(1MDATA)+ PDM(8CH)

    // For not HDA cases, we should do below sequence
    IDS_HDT_CONSOLE (MAIN_FLOW, "Program Soundwire Entry\n");

    //
    // Set Audio_Az_Global_Control.CRTS = 0x0"
    //
    //First Set to 0 and confirm
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                     SMN_Audio_Az_Global_Control_ADDRESS,
                     (UINT32) ~(Audio_Az_Global_Control_CRST_MASK),
                     0x0 << Audio_Az_Global_Control_CRST_OFFSET,
                     0
                     );

    SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_Audio_Az_Global_Control_ADDRESS, &RegisterValue);

    while (RegisterValue != 0) {
      SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_Audio_Az_Global_Control_ADDRESS, &RegisterValue);
    }
   //Then Set to 1 and confirm
    RegisterValue = 1;
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                     SMN_Audio_Az_Global_Control_ADDRESS,
                     (UINT32) ~(Audio_Az_Global_Control_CRST_MASK),
                     RegisterValue << Audio_Az_Global_Control_CRST_OFFSET,
                     0
                     );

    SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_Audio_Az_Global_Control_ADDRESS, &RegisterValue);
    while (RegisterValue != 1) {
      SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_Audio_Az_Global_Control_ADDRESS, &RegisterValue);
    }
   //Finally Set to 0 and confirm
    RegisterValue = 0;
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                     SMN_Audio_Az_Global_Control_ADDRESS,
                     (UINT32) ~(Audio_Az_Global_Control_CRST_MASK),
                     RegisterValue << Audio_Az_Global_Control_CRST_OFFSET,
                     0
                     );
    SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_Audio_Az_Global_Control_ADDRESS, &RegisterValue);
    while (RegisterValue != 0) {
      SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_Audio_Az_Global_Control_ADDRESS, &RegisterValue);
    }

    //Disable F6 HDAudio
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                     SMN_NBIF0INTERNAL_RCC_DEV0_EPF6_STRAP0_ADDRESS,
                     (UINT32) ~(RCC_DEV0_EPF6_STRAP0_STRAP_FUNC_EN_DEV0_F6_MASK),
                     (0 << RCC_DEV0_EPF6_STRAP0_STRAP_FUNC_EN_DEV0_F6_OFFSET),
                     0
                     );

  }

  if (!PcdGetBool (PcdCfgAzaliaEnable)) {
    /*Disable DCE.AZ F1*/
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                     SMN_NBIF0INTERNAL_RCC_DEV0_EPF1_STRAP0_ADDRESS,
                    (UINT32) ~(RCC_DEV0_EPF1_STRAP0_STRAP_FUNC_EN_DEV0_F1_MASK),
                    (0 << RCC_DEV0_EPF1_STRAP0_STRAP_FUNC_EN_DEV0_F1_OFFSET),
                     0
                     );
    IDS_HDT_CONSOLE (MAIN_FLOW, "Disable Azalia \n");
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * This function assigns MMIO for a IOHC non-pci device. This function is generic and
 * assumes the lock enable bit are both bit 1 and 0 respectively.
 *
 *
 *
 * @param[in]  GnbHandle       Silicon handle to assign
 * @param[in]  LowOffset       Offset of low byte
 * @param[in]  HighOffset      Offset of high byte
 * @param[in]  MemorySize      Size of the allocated bar required
 * @param[in]  Alignment       Alignment of the allocated bar required
 * @param[in]  LockSettings    If the allocated memory range should be locked or not
 * @retval     EFI_STATUS
 */
EFI_STATUS
IohcNonPCIBarInit (
  GNB_HANDLE   *GnbHandle,
  UINT32       LowOffset,
  UINT32       HighOffset,
  UINT32       MemorySize,
  UINT32       Alignment,
  BOOLEAN      LockSettings
  )
{
  EFI_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  UINT64                 MmioBase, Length;
  UINT32                 BarLow, BarHigh;

  BarLow = 0;
  BarHigh = 0;
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Begin to allocate bars for SMN low %x high %x\n", __FUNCTION__, BarLow, BarHigh);

  ///
  /// See if the given BAR have already been assigned
  ///
  SmnRegisterRead (GnbHandle->Address.Address.Bus, HighOffset, &BarHigh);
  SmnRegisterRead (GnbHandle->Address.Address.Bus, LowOffset, &BarLow);

  if (BarLow == 0 && BarHigh == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Bars have not been assigned, attempting to allocate MMIO \n", __FUNCTION__);
    ///
    /// Assign bars:
    /// Allocate a chunk of MMIO first
    ///
    Length = MemorySize;
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = GnbHandle->SocketId;
    MmioTarget.RbNum = GnbHandle->DieNumber;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
    MmioBase  = 0;

    Status = FabricAllocateMmio (&MmioBase, &Length, Alignment, MmioTarget, &Attributes);
    ASSERT (Status == EFI_SUCCESS );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Allocate MMIO Fail\n", __FUNCTION__);
      return EFI_OUT_OF_RESOURCES;
    } else {
      IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Allocate MMIO @0x%lx\n", __FUNCTION__, MmioBase);
    }
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Bars have already been assigned!\n", __FUNCTION__);
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : End\n", __FUNCTION__);
    return EFI_SUCCESS;
  }

  ///
  /// Write the assigned memory address registers to SMN
  ///
  BarHigh = (UINT32) RShiftU64 (MmioBase, 32);
  if (LockSettings) {
    BarLow = (UINT32)(MmioBase | (BIT0 + BIT1)); /// Set lock and enable bit
  }
  else {
    BarLow = (UINT32)(MmioBase | BIT0); /// Set enable bit
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a : BarLo = %x , BarHigh %x\n", __FUNCTION__, BarLow, BarHigh);

  SmnRegisterWrite (GnbHandle->Address.Address.Bus, HighOffset, &BarHigh, 0);
  SmnRegisterWrite (GnbHandle->Address.Address.Bus, LowOffset, &BarLow, 0);

  SmnRegisterRead (GnbHandle->Address.Address.Bus, HighOffset, &BarHigh);
  SmnRegisterRead (GnbHandle->Address.Address.Bus, LowOffset, &BarLow);
  IDS_HDT_CONSOLE (MAIN_FLOW, "Read back : BarLo = %x , BarHigh %x\n", __FUNCTION__, BarLow, BarHigh);

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a : End\n", __FUNCTION__);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**

 * configure HDAudio verb table
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 */
 /*----------------------------------------------------------------------------------------*/

VOID
HDAudioVerbTableSetting (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  EFI_STATUS            Status;
  FABRIC_TARGET         MmioTarget;
  FABRIC_MMIO_ATTRIBUTE MmioAttr;
  UINT64                HDAudioMmioBase;
  UINT64                HDAudioMmioSize;
  VOID                 *VerbTableAddress;
  UINT32                Value;
  UINT32                MmioBase;
  UINT64                Align;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a \n", __FUNCTION__);
  SmnRegisterRead (GnbHandle->Address.Address.Bus,
                              NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC6_NBIF0_VENDOR_ID_ADDRESS), &Value);
  if (Value != 0xFFFFFFFF) {
    IDS_HDT_CONSOLE (GNB_TRACE, "HDAudio ID = 0x%x\n", Value);
    MmioBase = 0xFFFFFFFF;
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                              NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_1_ADDRESS), &MmioBase, 0);
    SmnRegisterRead (GnbHandle->Address.Address.Bus,
                              NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_1_ADDRESS), &MmioBase);
    GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE, "   MmioBase read from BAR10 %x\n", MmioBase));

    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = 0;
    MmioTarget.RbNum = 0;
    MmioAttr.ReadEnable = 1;
    MmioAttr.WriteEnable = 1;
    MmioAttr.NonPosted = 0;
    MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
    HDAudioMmioSize = 0xFFFFFFFF - MmioBase + 1;
    Align = ~MmioBase;

    Status = FabricAllocateMmio (&HDAudioMmioBase, &HDAudioMmioSize, Align, MmioTarget, &MmioAttr);
    GNB_DEBUG_CODE (IDS_HDT_CONSOLE (GNB_TRACE, "   HDAudioMmioBase = %x,HDAudioMmioSize = %x\n", \
                                                        (UINT32)HDAudioMmioBase, (UINT32)HDAudioMmioSize));
    if (Status == EFI_SUCCESS) {
      MmioBase = (UINT32) HDAudioMmioBase;
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                               NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_1_ADDRESS), &MmioBase, 0);
      Value = (MmioBase & 0xFFF00000) | ((MmioBase & 0xFFF00000) >> 16);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                               NBIO_SPACE (GnbHandle, SMN_DEV0_NBIF0RC_MEM_BASE_LIMIT_ADDRESS), &Value, 0);
      SmnRegisterRead (GnbHandle->Address.Address.Bus,
                               NBIO_SPACE (GnbHandle,  SMN_DEV0_NBIF0RC_SUB_BUS_NUMBER_LATENCY_ADDRESS), &Value);
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                              NBIO_SPACE (GnbHandle,   SMN_DEV0_NBIF0RC_SUB_BUS_NUMBER_LATENCY_ADDRESS),
                                                                            0xFF0000FF, 0x0a0a00, 0);
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle,   SMN_DEV0_NBIF0RC_COMMAND_ADDRESS),
                                                             (UINT32) ~(BIT1 | BIT2), (BIT1 | BIT2), 0);
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle,   SMN_DEV0_FUNC6_NBIF0_COMMAND_ADDRESS),
                                                             (UINT32) ~(BIT1 | BIT2), (BIT1 | BIT2), 0);
      VerbTableAddress = (VOID *) PcdGet32 (PcdCfgAzaliaCodecVerbTable);
      NbioConfigureVerbTable (MmioBase, VerbTableAddress);
      MmioBase = 0;
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle,   SMN_DEV0_FUNC6_NBIF0_COMMAND_ADDRESS),
                                                             (UINT32) ~(BIT1 | BIT2), 0, 0);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_1_ADDRESS), &MmioBase, 0);
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle,   SMN_DEV0_NBIF0RC_COMMAND_ADDRESS),
                                                             (UINT32) ~(BIT1 | BIT2), 0, 0);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle, SMN_DEV0_NBIF0RC_SUB_BUS_NUMBER_LATENCY_ADDRESS), &Value, 0);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle, SMN_DEV0_NBIF0RC_MEM_BASE_LIMIT_ADDRESS), &MmioBase, 0);
    }

    Value = PcdGet32 (PcdCfgAzaliaSsid);
    if (Value != 0) {
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          NBIO_SPACE (GnbHandle, SMN_DEV0_FUNC6_NBIF0_ADAPTER_ID_W_ADDRESS), &Value, 0);
      IDS_HDT_CONSOLE (GNB_TRACE, "NBIO HDAudio SSID=%x\n", Value);
    }
  }
}

/**
 *---------------------------------------------------------------------------------------
 *  NbioRmbA0Table
 *
 *  Description:
 *     Apply RMB A0 Table
 *  Parameters:
 *    @param[in]     GnbHandle     GnbHandle
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
NbioRmbA0Table (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "  Apply RMB A0 workaround\n");
  // 1. Enable chain dynamically break feature
  //    GDC.A2S_REQ_RSP_TAG_CNTL.WR_TAG_FOR_CHAIN_ENABLE = 1
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                SMN_NBIF0_A2S_REQ_RSP_TAG_CNTL_ADDRESS,
                (UINT32) ~(A2S_REQ_RSP_TAG_CNTL_WR_TAG_FOR_CHAIN_ENABLE_MASK),
                (1 << A2S_REQ_RSP_TAG_CNTL_WR_TAG_FOR_CHAIN_ENABLE_OFFSET),
                0
                );
  // 2. Enable chain for VC1, VC5. Keep default value is ok. Nothing to set.
  // 3. Disable chain:
  //    3.1 Disable chain for VC3 (switch0):  GDC.A2S_CNTL_SW0.SDP_WR_CHAIN_DIS = 1
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                SMN_NBIF0_A2S_CNTL_SW0_ADDRESS,
                (UINT32) ~(A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_MASK),
                (1 << A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_OFFSET),
                0
                );
  //    3.2 Disable chain for HSP (switch4):  GDC.A2S_CNTL_SW4.SDP_WR_CHAIN_DIS = 1
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                SMN_NBIF0_A2S_CNTL_SW4_ADDRESS,
                (UINT32) ~(A2S_CNTL_SW4_SDP_WR_CHAIN_DIS_MASK),
                (1 << A2S_CNTL_SW4_SDP_WR_CHAIN_DIS_OFFSET),
                0
                );
  // 4. VC0 chain is by default enabled, but no mem wr or msg_d will be sent to VC0. Nothing to set.
  // 5. Set A2S_TAG_ALLOC_0.TAG_ALLOC_FOR_VC1_WR to any value in 4~31.(default 4)
  // 6. Set A2S_TAG_ALLOC_2.TAG_ALLOC_FOR_VC5_WR to any value in 10~127.
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                SMN_NBIF0_A2S_TAG_ALLOC_2_ADDRESS,
                (UINT32) ~(A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_WR_MASK),
                (10 << A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_WR_OFFSET),
                0
                );
// --------------------------------------------------
// 14.6.9.5.6 PerfReg Final Setting
// --------------------------------------------------
  // SYSHUBMM::A2S_TAG_ALLOC_0[TAG_ALLOC_FOR_VC0_WR] = 4h.
  // SYSHUBMM::A2S_TAG_ALLOC_0[TAG_ALLOC_FOR_VC0_RD] = 4h.
  // SYSHUBMM::A2S_TAG_ALLOC_0[TAG_ALLOC_FOR_VC1_WR] = 4h.
  // SYSHUBMM::A2S_TAG_ALLOC_1[TAG_ALLOC_FOR_VC3_WR] = 4h.
  // SYSHUBMM::A2S_TAG_ALLOC_1[TAG_ALLOC_FOR_VC7_WR] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_1[TAG_ALLOC_FOR_VC7_RD] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_2[TAG_ALLOC_FOR_VC5_WR] = 48h.
  // SYSHUBMM::A2S_TAG_ALLOC_2[TAG_ALLOC_FOR_VC5_RD] = 48h.
  // SYSHUBMM::A2S_TAG_ALLOC_2[TAG_ALLOC_FOR_VC6_WR] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_2[TAG_ALLOC_FOR_VC6_RD] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_3[TAG_ALLOC_FOR_VC4_WR] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_3[TAG_ALLOC_FOR_VC4_RD] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_3[TAG_ALLOC_FOR_VC2_WR] = 0h.
  // SYSHUBMM::A2S_TAG_ALLOC_3[TAG_ALLOC_FOR_VC2_RD] = 0h.
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_A2S_TAG_ALLOC_0_ADDRESS,
    A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_WR_MASK |
    A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_RD_MASK |
    A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC1_WR_MASK,
    (0x4 << A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_WR_OFFSET) |
    (0x4 << A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_RD_OFFSET) |
    (0x4 << A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC1_WR_OFFSET),
    0
    );
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_A2S_TAG_ALLOC_1_ADDRESS,
    A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC3_WR_MASK |
    A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_WR_MASK |
    A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_RD_MASK,
    (0x4 << A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC3_WR_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_WR_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_RD_OFFSET),
    0
    );
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_A2S_TAG_ALLOC_2_ADDRESS,
    A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_WR_MASK |
    A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_RD_MASK |
    A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_WR_MASK |
    A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_RD_MASK,
    (0x48 << A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_WR_OFFSET) |
    (0x48 << A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_RD_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_WR_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_RD_OFFSET),
    0
    );
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_A2S_TAG_ALLOC_3_ADDRESS,
    A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_WR_MASK |
    A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_RD_MASK |
    A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_WR_MASK |
    A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_RD_MASK,
    (0x0 << A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_WR_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_RD_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_WR_OFFSET) |
    (0x0 << A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_RD_OFFSET),
    0
    );

  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV2_EPF0_STRAP2_ADDRESS),
    (UINT32)~(RCC_DEV0_EPF0_STRAP2_STRAP_NO_SOFT_RESET_DEV0_F0_MASK),
    (0x0 << RCC_DEV0_EPF0_STRAP2_STRAP_NO_SOFT_RESET_DEV0_F0_OFFSET),
    0
    );
}

/**
 *---------------------------------------------------------------------------------------
 *  NbioRmbB0Table
 *
 *  Description:
 *     Apply RMB B0 Table
 *  Parameters:
 *    @param[in]     GnbHandle     GnbHandle
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
NbioRmbB0Table (
  IN       GNB_HANDLE  *GnbHandle
  )
{
// --------------------------------------------------
// 14.6.9.5.1 Endpoint function interrupt enable
// --------------------------------------------------
  IDS_HDT_CONSOLE (GNB_TRACE, "  Apply Nbio Rmb B0 Table\n");
  // NBIO_NBIF_DEVICE_INTERRUPT_TBL
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  SMN_NBIF0_INTR_LINE_ENABLE_ADDRESS,
                  (UINT32) ~(INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV0_MASK |
                             INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV1_MASK |
                             INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV2_MASK),
                  (0xFF << INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV0_OFFSET) |
                  (0x3 << INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV1_OFFSET) |
                  (0x7F << INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV2_OFFSET),
                  0
                  );

  if (PcdGet8 (PcdUsb4DpiaDisable) != 0) {
    // disable USB rounter0/1
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0_INTR_LINE_ENABLE_ADDRESS,
                    (UINT32) ~(INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV2_MASK),
                    (0x1F << INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV2_OFFSET),
                    0
                    );
    // USB rounter0 - nbif2F5
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0INTERNAL_RCC_DEV2_EPF5_STRAP0_ADDRESS,
                    (UINT32) ~(RCC_DEV2_EPF5_STRAP0_STRAP_FUNC_EN_DEV2_F5_MASK),
                    (0x0 << RCC_DEV2_EPF5_STRAP0_STRAP_FUNC_EN_DEV2_F5_OFFSET),
                    0
                    );
    // USB rounter1 - nbif2F6
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0INTERNAL_RCC_DEV2_EPF6_STRAP0_ADDRESS,
                    (UINT32) ~(RCC_DEV2_EPF6_STRAP0_STRAP_FUNC_EN_DEV2_F6_MASK),
                    (0x0 << RCC_DEV2_EPF6_STRAP0_STRAP_FUNC_EN_DEV2_F6_OFFSET),
                    0
                    );
  }

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  SMN_NBIF0INTERNAL_RCC_DEV2_EPF5_STRAP3_ADDRESS,
                  (UINT32) ~(RCC_DEV2_EPF5_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F5_MASK),
                  (0x1 << RCC_DEV2_EPF5_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F5_OFFSET),
                  0
                  );
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  SMN_NBIF0INTERNAL_RCC_DEV2_EPF6_STRAP3_ADDRESS,
                  (UINT32) ~(RCC_DEV2_EPF6_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F6_MASK),
                  (0x1 << RCC_DEV2_EPF6_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F6_OFFSET),
                  0
                  );

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                  SMN_NBIF0_NGDC_PG_MISC_CTRL_ADDRESS,
                  (UINT32) ~(NGDC_PG_MISC_CTRL_NGDC_CFG_REFCLK_CYCLE_FOR_200NS_MASK),
                  (0x5 << NGDC_PG_MISC_CTRL_NGDC_CFG_REFCLK_CYCLE_FOR_200NS_OFFSET),
                  0
                  );

}

/**
 *---------------------------------------------------------------------------------------
 *  DmaControlSetting
 *
 *  Description:
 *     To set the DMA control bit
 *  Parameters:
 *    @param[in]     GnbHandle     GnbHandle
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
DmaControlSetting (
  IN       GNB_HANDLE                  *GnbHandle
  )
{

}


/*----------------------------------------------------------------------------------------*/
/**
 * This is the main function for early NBIO initialization.
 *
 *
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
AmdNbioBaseInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *PcieServicesPpi;
  PCIe_PLATFORM_CONFIG            *Pcie;
  UINT32                          Property;
  EFI_STATUS                      Status;
  GNB_HANDLE                      *GnbHandle;
  AMD_CONFIG_PARAMS               *StdHeader;
  GNB_PCIE_INFORMATION_DATA_HOB   *PciePlatformConfigHob;
  AMD_PEI_SOC_LOGICAL_ID_PPI      *SocLogicalIdPpi;
  SOC_LOGICAL_ID                  LogicalId;
  UINT32                          Value;
  BOOLEAN                         IsB0 = FALSE;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  StdHeader = NULL;
  GnbHandle = NULL;

  // Need topology structure
  (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gAmdNbioPcieServicesPpiGuid,
                    0,
                    NULL,
                    (VOID **)&PcieServicesPpi
                    );
  PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);

  // Adjust Property based on Debug Info and PCD's
  // From Earlier
  GnbHandle = NbioGetHandle (Pcie);
  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdSocLogicalIdPpiGuid, 0, NULL, &SocLogicalIdPpi);
  SocLogicalIdPpi->GetLogicalIdOnCurrentCore (&LogicalId);
  // A0 Setting
  if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Ax) != 0)) {
    // Disable PMA PG in A0
    PcdSetBoolS (PcdCfgDxioPmaPowerGating, FALSE);

    // Copy SB_LOCATION to the L1 and L2 in IOMMU. This only applies to NBIO3, where the FCH is located
    SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_SB_LOCATION_ADDRESS), &Value);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Read SB_LOCATION as 0x%x from 0x%x\n",\
                               Value, NBIO_SPACE(GnbHandle, SMN_SB_LOCATION_ADDRESS));
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                                NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_SB_LOCATION_ADDRESS), &Value, 0);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Wrote SB_LOCATION as 0x%x from 0x%x\n", Value,
                                 NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_SB_LOCATION_ADDRESS));
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_L2_SB_LOCATION_ADDRESS), &Value, 0);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Wrote SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, SMN_L2_SB_LOCATION_ADDRESS));
  }
  // B0 Setting
  if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
    // Enable PMA PG in B0
    PcdSetBoolS (PcdCfgDxioPmaPowerGating, TRUE);

    SmnRegisterRead (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_SB_LOCATION_ADDRESS), &Value);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Read SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, SMN_SB_LOCATION_ADDRESS));
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_L2_SB_LOCATION_ADDRESS), &Value, 0);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Wrote SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, SMN_L2_SB_LOCATION_ADDRESS));
    Value = 0x00020001;
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_SB_LOCATION_ADDRESS), &Value, 0);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Wrote SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_SB_LOCATION_ADDRESS));
    IsB0 = TRUE;
    PcdSet32S (PcdUsbSysHubSelect, 0x1F);
  }

  // To Check GFX IP is disable or not
  SmnRegisterRead (GnbHandle->Address.Address.Bus, SMN_FUSE_DATA_311_ADDRESS, &Value);
  IDS_HDT_CONSOLE (MAIN_FLOW, "  GFX disable bit 0x%x\n", Value);
  if (Value & BIT1) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  Disable PcdCfgIgpuContorl !!\n");
    PcdSet8S (PcdCfgIgpuContorl, 0);
  }

  // Need debug options
  Property = TABLE_PROPERTY_DEFAULT;
  Property |= PcdGet8 (PcdCfgIgpuContorl) ? 0 : TABLE_PROPERTY_IGFX_DISABLED;

  ///
  /// Blast tables that should be applied to each GnbHandle
  ///

  if (PcdGet8 (PcdNbioGlobalCgOverride)== 0) {
      PcdSetBoolS (PcdCfgSstunlClkGating, FALSE);
      PcdSetBoolS (PcdIOHCClkGatingSupport, FALSE);
      PcdSetBoolS (PcdIOHCPgEnable, FALSE);
      PcdSetBoolS (PcdCfgNbifMgcgClkGating, FALSE);
      PcdSetBoolS (PcdCfgSyshubMgcgClkGating, FALSE);
      PcdSet8S (PcdNbifMgcgHysteresis, 0);
      PcdSet8S (PcdSyshubMgcgHysteresis, 0);
      PcdSet8S (PcdNbifPgHysteresis, 0);
      PcdSetBoolS (PcdIOAGRPgEnable, FALSE);
  }

  IDS_HOOK (IDS_HOOK_NBIO_BASE_INIT, NULL, (void *)GnbHandle);

  Status = GnbBlastTable (
             GnbHandle,
             GnbEarlyInitTable,
             Property,
             0,
             StdHeader
             );

  if (IsB0 == FALSE) {
    // A0 Setting
    NbioRmbA0Table (GnbHandle);
  } else {
    // B0 Setting

    Status = GnbBlastTable (
             GnbHandle,
             GnbEarlyInitB0Table,
             Property,
             0,
             StdHeader
             );

    NbioRmbB0Table (GnbHandle);

  }

  if (PcdGet8 (PcdNbifMgcgHysteresis) != 0) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0_NBIF_MGCG_CTRL_LCLK_ADDRESS,
                    (UINT32) ~(NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_HYSTERESIS_LCLK_MASK),
                    ((PcdGet8 (PcdNbifMgcgHysteresis)) << NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_HYSTERESIS_LCLK_OFFSET),
                    0
                    );
  }
  if (PcdGet8 (PcdSyshubMgcgHysteresis) != 0) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0_SYSHUB_MGCG_CTRL_SOCCLK_ADDRESS,
                    (UINT32) ~(SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_HYSTERESIS_SOCCLK_MASK),
                    ((PcdGet8 (PcdSyshubMgcgHysteresis)) << SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_HYSTERESIS_SOCCLK_OFFSET),
                    0
                    );
  }
  if (PcdGet8 (PcdNbifPgHysteresis) != 0) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0_NBIF_PGMST_CTRL_ADDRESS,
                    (UINT32) ~(NBIF_PGMST_CTRL_NBIF_CFG_PG_HYSTERESIS_MASK),
                    ((PcdGet8 (PcdNbifPgHysteresis)) << NBIF_PGMST_CTRL_NBIF_CFG_PG_HYSTERESIS_OFFSET),
                    0
                    );
  }
  if (PcdGet8 (PcdSyshubMgcgHspClkHysteresis) != 0) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_SYSHUB_MGCG_CTRL_HSPCLK_ADDRESS,
                    (UINT32) ~(SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_HYSTERESIS_HSPCLK_MASK),
                    ((PcdGet8 (PcdSyshubMgcgHspClkHysteresis)) << SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_HYSTERESIS_HSPCLK_OFFSET),
                    0
                    );
  }
  if (PcdGet8 (PcdSyshubGdcMgcgHysteresis) != 0) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NGDC_MGCG_CTRL_ADDRESS,
                    (UINT32) ~(NGDC_MGCG_CTRL_NGDC_MGCG_HYSTERESIS_MASK),
                    ((PcdGet8 (PcdSyshubGdcMgcgHysteresis)) << NGDC_MGCG_CTRL_NGDC_MGCG_HYSTERESIS_OFFSET),
                    0
                    );
  }
  if (PcdGet8 (PcdSyshubPgHysteresis) != 0) {
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_NBIF0_NGDC_PGMST_CTRL_ADDRESS,
                    (UINT32) ~(NGDC_PGMST_CTRL_NGDC_CFG_PG_HYSTERESIS_MASK),
                    ((PcdGet8 (PcdSyshubPgHysteresis)) << NGDC_PGMST_CTRL_NGDC_CFG_PG_HYSTERESIS_OFFSET),
                    0
                    );
  }


  IoapicEnableCallback (GnbHandle);
  NbioBaseInitCallback (GnbHandle, &LogicalId);
  AudioIOConfigCallback (GnbHandle);

  if (PcdGetBool (PcdCfgHdAudioEnable)) {
    Value = PcdGet8 (PcdAmdNbioAudioSelect);
    // For AM5
    //   07 - SDW0(4MDATA) + PDM(6CH)
    //   08 - SDW0(4MDATA) + SDW1(1MDATA) + PDM(2CH)
    // For FP7/FP7r2
    //   09 - 3I2S PORTS + 1 REFCLK + 1 INTR
    //   13 - SDW0(4MDATA)+ SDW1(1MDATA) + PDM(6CH) + I2S
    //   14 - SDW0(4MDATA) + SDW1(1MDATA)+ PDM(8CH)
    if (!(((Value >= 7) && (Value <= 9)) ||
        ((Value >= 13) && (Value <= 14)))) {
      // For HDA cases, we should call HDAudioVerbTableSetting()
      IDS_HDT_CONSOLE (MAIN_FLOW, "Entry HDAudioVerbTableSetting\n");
      HDAudioVerbTableSetting (GnbHandle);
    }
  }
  ///
  /// Bar allocation for SMN non pci devices
  /// Note that PSPP bar allocation is slightly different and is
  /// done elsewhere
  ///
  if (PcdGetBool(PcdCfgIohcNonPCIBarInitSmu)) {
    IohcNonPCIBarInit(
      GnbHandle,
        SMN_SMU_BASE_ADDR_LO_ADDRESS,
        SMN_SMU_BASE_ADDR_HI_ADDRESS,
      IOHCMISC_BARSIZE_1MB,
      ALIGN_1M,
      TRUE
      );
  }
  if (PcdGetBool(PcdCfgIohcNonPCIBarInitDbg)) {
    IohcNonPCIBarInit(
      GnbHandle,
        SMN_DBG_BASE_ADDR_LO_ADDRESS,
        SMN_DBG_BASE_ADDR_HI_ADDRESS,
      IOHCMISC_BARSIZE_512KB,
      ALIGN_1M,
      TRUE
      );
  }
  if (PcdGetBool(PcdCfgIohcNonPCIBarInitFastReg)) {
    IohcNonPCIBarInit(
      GnbHandle,
        SMN_FASTREG_BASE_ADDR_LO_ADDRESS,
        SMN_FASTREG_BASE_ADDR_HI_ADDRESS,
      IOHCMISC_BARSIZE_1MB,
      ALIGN_1M,
      TRUE
      );
  }
  if (PcdGetBool(PcdCfgIohcNonPCIBarInitFastRegCtl)) {
    IohcNonPCIBarInit(
      GnbHandle,
        SMN_FASTREGCNTL_BASE_ADDR_LO_ADDRESS,
        SMN_FASTREGCNTL_BASE_ADDR_HI_ADDRESS,
      IOHCMISC_BARSIZE_4KB,
      ALIGN_4K,
      TRUE
      );
  }

  DmaControlSetting (GnbHandle);

  // SysHub USB select
  Value = PcdGet32 (PcdUsbSysHubSelect);
  if (Value != 0xFF) {
    SmnRegisterWrite(GnbHandle->Address.Address.Bus,
                     SMN_NBIF0_SYSHUB_SELECT_SHUBCLK_ADDRESS,
                     &Value,
                     0);
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback for NbioPcieServicesPpi installation when NbioTopology is completed
 *
 *
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  NotifyDescriptor  NotifyDescriptor pointer
 * @param[in]  Ppi               Ppi pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
NbioTopologyConfigureCallbackPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{

  AGESA_TESTPOINT (TpNbioTopologyConfigureCallbackEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  // At this point we know the NBIO topology, so we can initialize all NBIO base registers
  AmdNbioBaseInit (PeiServices);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpNbioTopologyConfigureCallbackExit, NULL);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * AmdNbiosBase driver entry point for RMB
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
AmdNbioBasePeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS    Status;

  AGESA_TESTPOINT (TpNbioBasePeiEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  // ACS=enabled + AER=disabled is an invalid configuration:
  if (PcdGetBool (PcdCfgACSEnable)) {
    PcdSetBoolS (PcdCfgAEREnable, TRUE);
  }

  // PcdAmdEdpcEnable should only take effect if it is set to true
  //AND PcdAmdPcieAerReportMechanism is set to MCA (0) or Legacy FW-First (2).
  if (PcdGet8 (PcdAmdPcieAerReportMechanism) == 1) {
    PcdSet8S (PcdAmdEdpcEnable, FALSE);
  }

  PcdSetBoolS (PcdSyshubPg, FALSE);

  // PCIe topology is dependent on NbioBaseServicesPpi, so Pcie driver will not publish PPI until after
  // this code is completed. We can simply request notification when gAmdNbioPcieServicesPpiGuid is published
  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyNbioTopologyPpi);

  // Publish callback for memory config done so that we can set top of memory
  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyMemDonePpi);

  // Install PPI for NbioBaseServices
  Status = PeiServicesInstallPpi (&mNbioBaseServicesPpiList); //This driver is done.
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpNbioBasePeiExit, NULL);
  return EFI_SUCCESS;
}


