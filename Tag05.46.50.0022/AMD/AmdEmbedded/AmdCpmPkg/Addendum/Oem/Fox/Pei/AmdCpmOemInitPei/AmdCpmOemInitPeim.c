/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <Library/IoLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Library/SmnAccessLib.h>
#include "AmdCpmOemInitPeim.h"
#include "FchRegistersKT.h"
#include <Ppi/I2cMaster.h>
#include <Ppi/M24Lc128Ppi.h>
#include <Ppi/NbioPcieComplexPpi.h>

extern  AMD_CPM_PLATFORM_ID_TABLE                 gCpmPlatformIdTable;
extern  AMD_CPM_PLATFORM_ID_CONVERT_TABLE         gCpmPlatformIdConvertTable;
extern  AMD_CPM_PRE_INIT_TABLE                    gCpmPreInitTable;
extern  AMD_CPM_GPIO_INIT_TABLE                   gCpmGpioInitTable;
extern  AMD_CPM_GEVENT_INIT_TABLE                 gCpmGeventInitTable;
extern  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE          gCpmGpioDeviceConfigTable;
extern  AMD_CPM_GPIO_DEVICE_DETECTION_TABLE       gCpmGpioDeviceDetectionTable;
extern  AMD_CPM_GPIO_DEVICE_RESET_TABLE           gCpmGpioDeviceResetTable;
extern  AMD_CPM_GPIO_DEVICE_POWER_TABLE           gCpmGpioDevicePowerTable;
extern  AMD_CPM_PCIE_CLOCK_TABLE                  gCpmPcieClockTable;
extern  AMD_CPM_DXIO_TOPOLOGY_TABLE               gCpmDxioTopologyTable;
extern  AMD_CPM_DEVICE_PATH_TABLE                 gCpmDevicePathTable;
extern  AMD_CPM_DISPLAY_FEATURE_TABLE             gCpmDisplayFeatureTable;
extern  AMD_CPM_SAVE_CONTEXT_TABLE                gCpmSaveContextTable;
extern  AMD_CPM_ZERO_POWER_ODD_TABLE              gCpmZeroPowerOddTable;
extern  AMD_CPM_UCSI_TABLE                        gCpmUcsiTable;
extern  AMD_CPM_WIRELESS_LAN_RECOVERY_TABLE       gCpmWirelessLanRecoveryTable;
extern  AMD_CPM_THUNDERBOLT_TABLE                 gCpmThunderboltTable;
//extern  AMD_CPM_WIRELESS_BUTTON_TABLE             gCpmWirelessButtonTable;
extern  AMD_CPM_PCI_RES_RES_TABLE                 gCpmPciResResTable;
extern  AMD_CPM_NVME_RTD3_TABLE                   gCpmNvmeRtd3Table;

extern  UINT32                                    usb_phy_tuning_set_size;
extern  C20_N6_PHY_TUNING_SET                     usb_phy_tuning_set_ver_b[];

EFI_STATUS
EFIAPI
AmdCpmOemInitPeimNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
AmdCpmOemInitPpiTableNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
AmdNbioPcieComplexPpiNotifyCallback(
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
);
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mCpmOemInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiSmbus2PpiGuid,
  AmdCpmOemInitPeimNotifyCallback
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmOemGpioInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmGpioInitFinishedPpiGuid,
  AmdCpmOemInitPpiTableNotifyCallback
};
STATIC EFI_PEI_NOTIFY_DESCRIPTOR mAmdNbioPcieComplexPpiNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieComplexPpiGuid,
  AmdNbioPcieComplexPpiNotifyCallback
};


EFI_STATUS
EFIAPI
AmdCpmTableOverride (
  IN       EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
EFIAPI
AmdCpmOverrideTableNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

VOID
UpdateDisplayPcdOnOemInitPeiEntry (
  IN       AMD_CPM_MAIN_TABLE           *MainTablePtr,
  IN       AMD_PBS_SETUP_OPTION         *AmdPbsConfiguration
  );


STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmOemTableOverrideNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmTablePpiGuid,
  AmdCpmOverrideTableNotifyCallback
};

//
// CPM Main Table
//
AMD_CPM_MAIN_TABLE gCpmMainTable = {
  {CPM_SIGNATURE_MAIN_TABLE, sizeof (gCpmMainTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  "Fox",    // PlatformName
  0xFF,         // BiosType
  1,            // CurrentPlatformId
  0,            // PcieMemIoBaseAddr
  0,            // AcpiMemIoBaseAddr
  NULL,         // Reserved for Internal Used
  NULL,         // Reserved for Internal Used
  NULL,         // Reserved for Internal Used
  NULL,         // Reserved for Internal Used
  NULL,         // Reserved for Internal Used
  0x2200,       // DisplayFeature
                //  - IsBrightnessByDriver
                //  - SpecialPostIgpu
  0,            // ZeroPowerOddEn
  0,            // AcpiThermalFanEn
  0,            // ExtClkGen
  0,            // UnusedGppClkOffEn
  0,            // AdaptiveS4En
  0,            // WirelessButtonEn
  0,            // Ec
  0,            // TdpLimitChangeEn
  0,            // SmiCheckToolEn
  0,            // LpcUartEn
  0,            // ProchotEn
  0,            // PtBrMemIoBaseAddr
  0,            // KbcSupport
  0,            // EcSupport
  0,            // UcsiEn
  0,            // ModernStandbyEn
  0,            // SensorFusionEn
  0,            // AcpPowerGatingEn
  0,            // SmbusIoBaseAddr
  0,            // SwSmiPort
  0             // AcpClockGatingEn
};

VOID *gCpmTableList[] = {
  &gCpmMainTable,
  &gCpmPlatformIdTable,
  &gCpmPlatformIdConvertTable,
  &gCpmPreInitTable,
  &gCpmSaveContextTable,
  &gCpmGpioInitTable,
  &gCpmGeventInitTable,
  &gCpmGpioDeviceConfigTable,
  &gCpmGpioDevicePowerTable,
  &gCpmGpioDeviceDetectionTable,
  &gCpmGpioDeviceResetTable,
  &gCpmPcieClockTable,
  &gCpmDxioTopologyTable,
  &gCpmDisplayFeatureTable,
  &gCpmDevicePathTable,
  &gCpmZeroPowerOddTable,
  &gCpmUcsiTable,
  &gCpmWirelessLanRecoveryTable,
//  &gCpmWirelessButtonTable,
  &gCpmThunderboltTable,
  &gCpmPciResResTable,
  &gCpmNvmeRtd3Table,
  NULL
};

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM OEM Init PEIM driver
 *
 * This function defines CPM OEM definition tables and installs AmdCpmOemTablePpi.
 * It also defines callback function to update these definition table on run time.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmOemInitPeimEntryPoint (
  IN       CPM_PEI_FILE_HANDLE      FileHandle,
  IN       CPM_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS              Status;
  AMD_PBS_SETUP_OPTION    AmdPbsConfiguration;
  UINT64                  Above4GBMmioLimit;
  PRESIL_CTRL0            PreSilCtrlValue;
  EFI_PEI_PPI_DESCRIPTOR  *PpiListEnvironment;
  UINT32                  value98;

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-Start\n", __FUNCTION__));
  DEBUG((DEBUG_INFO, "OEM-PEI-%a-NotifyPpi-gEfiPeiSmbus2PpiGuid", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-For-AmdCpmOemInitPeimNotifyCallback\n"));
  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmOemInitPeimNotify);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-GetAmdPbsConfiguration\n", __FUNCTION__));
  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-4-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  if ((AmdPbsConfiguration.PbsDbgGnbDxioTimingControlEnable != 0xf)) {
    PcdSetBoolS (PcdPcieDxioTimingControlEnable, AmdPbsConfiguration.PbsDbgGnbDxioTimingControlEnable);
  }

  //Check Display Condition PbsDbgGnbDxioTimingControlEnable=Enable
  if ((AmdPbsConfiguration.PbsDbgGnbDxioTimingControlEnable == 0x1)) {
    PcdSet32S (PcdPCIELinkReceiverDetectionPolling, AmdPbsConfiguration.PbsDbgGnbPcieLinkReceiverDetectionPolling);
  }

  //Check Display Condition PbsDbgGnbDxioTimingControlEnable=Enable
  if ((AmdPbsConfiguration.PbsDbgGnbDxioTimingControlEnable == 0x1)) {
    PcdSet32S (PcdPCIELinkL0Polling, AmdPbsConfiguration.PbsDbgGnbPcieLinkL0Polling);
  }

  Above4GBMmioLimit = LShiftU64 (1, AmdPbsConfiguration.Above4GBMmioLimit) - 1;
  if (PcdGet64 (PcdAmdMmioAbove4GLimit) != Above4GBMmioLimit) {
    PcdSet64S (PcdAmdMmioAbove4GLimit, Above4GBMmioLimit);
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-Set PcdAmdMmioAbove4GLimit to 0x%016LX\n", __FUNCTION__, PcdGet64 (PcdAmdMmioAbove4GLimit)));
  }

  SmnRegisterRead(0, MP0_C2PMSG_97_ADDR, &PreSilCtrlValue);
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmOemInitPeimEntryPoint PreSilCtrlValue = %x \n", PreSilCtrlValue.Field.Environment));
  PcdSet32S(PcdAmdEnvironmentFlag, PreSilCtrlValue.Field.Environment );

  SmnRegisterRead(0, MP0_C2PMSG_98_ADDR, &value98);
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmOemInitPeimEntryPoint value98 = %x \n", value98));
  PcdSet32S(PcdAmdEnvironment98, value98);

  // Turn off Xtal during S3/S5
  if (AmdPbsConfiguration.TurnOffXtalS3S5) {
    PcdSetBoolS(PcdTurnOffXtalS3S5, TRUE);
  } else {
    PcdSetBoolS(PcdTurnOffXtalS3S5, FALSE);
  }

  if (PreSilCtrlValue.Field.Environment == 0 ) //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
  {
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (EFI_PEI_PPI_DESCRIPTOR),
                               (VOID**)&PpiListEnvironment
                               );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-AllocatePool-PpiListEnvironment Failed 2-Status=%r\n", Status));
      return Status;
    }

    PpiListEnvironment->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    PpiListEnvironment->Guid = &gPlatformPkgEnvironmentGuid;
    PpiListEnvironment->Ppi = NULL;

    Status = (*PeiServices)->InstallPpi (
                                PeiServices,
                                PpiListEnvironment
                                );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "PpiListEnvironment-InstallPpi-gPlatformPkgEnvironmentGuid Failed -Status=%r\n", Status));
      return Status;
    }

    if (value98 & (UINT32) BIT16) {     // disable EC support if _98[16] = 1 (disable EC support), this is used to support simnow running
      PcdSetBoolS (PcdSpreadSpectrum, 0);
      PcdSet8S (PcdPsppPolicy, 0);
    }
  } //PreSilCtrlValue.Value.Environment
  else
  {
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (EFI_PEI_PPI_DESCRIPTOR),
                               (VOID**)&PpiListEnvironment
                               );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-AllocatePool-PpiListEnvironment Failed 2-Status=%r\n", Status));
      return Status;
    }

    PpiListEnvironment->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    PpiListEnvironment->Guid = &gPlatformPkgEmulationGuid;
    PpiListEnvironment->Ppi = NULL;

    Status = (*PeiServices)->InstallPpi (
                                PeiServices,
                                PpiListEnvironment
                                );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "PpiListEnvironment-InstallPpi-gPlatformPkgEmulationGuid Failed -Status=%r\n", Status));
      return Status;
    }

    PcdSet8S (PcdAmdPspSystemTpmConfig, 0xFF);
    PcdSetBoolS (PcdEspiEc0Enable, 0);
    PcdSetBoolS (PcdEspiKbc6064Enable, 0);
    PcdSet16S (PlatformIdOverride, 0);
    PcdSetBoolS (PcdNoneSioKbcSupport, 0);
    PcdSetBoolS (PcdEspiKbc6064Enable, 0);
    PcdSetBoolS (PcdAmdSmmLock, 0);
    PcdSetBoolS (PcdAmdPspApcbRecoveryEnable, 0);
    PcdSetBoolS (PcdCfgIommuSupport, 0);
    PcdSetBoolS (PcdSpreadSpectrum, 0);
    PcdSet8S (PcdPsppPolicy, 0);
    PcdSetBoolS (PcdAmdCpmPciHotPlugSupport, 0);
  }

  DEBUG((DEBUG_INFO, "USB4 PHY tuning set size = %08X\n", usb_phy_tuning_set_size));
  DEBUG((DEBUG_INFO, "Address of USB4 PHY tuning set = %08X\n", usb_phy_tuning_set_ver_b));

  PcdSet32S (PcdUsb4PhyTuningSettingTableContent, (UINT32)(UINTN)usb_phy_tuning_set_ver_b);
  PcdSet32S (PcdUsb4PhyTuningSettingTableSize, usb_phy_tuning_set_size);

  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmOemGpioInitPeimNotify);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmOemInitPeimEntryPoint, Notification of mCpmOemGpioInitPeimNotify failed, Status = %r\n", Status));
    return Status;
  }
  Status = (**PeiServices).NotifyPpi(PeiServices, &mAmdNbioPcieComplexPpiNotify);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmOemInitPeimEntryPoint, Notification of mCpmOemGpioInitPeimNotify failed, Status = %r\n", Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-5-Status=%r\n", __FUNCTION__, Status));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback Function of AMD CPM OEM Init PEIM driver
 *
 * @param[in]     PeiServices       The PEI core services table.
 * @param[in]     NotifyDescriptor  The descriptor for the notification event.
 * @param[in]     Ppi               Pointer to the PPI in question
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmOemInitPeimNotifyCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN  VOID                            *Ppi
  )
{
  EFI_STATUS              Status;
  AMD_CPM_OEM_TABLE_PPI   *AmdCpmOemTablePpi;
  EFI_PEI_PPI_DESCRIPTOR  *PpiListCpmOemTable;
  AMD_PBS_SETUP_OPTION    AmdPbsConfiguration;
  AMD_CPM_MAIN_TABLE      *MainTablePtr = NULL;
  AMD_CPM_MAIN_TABLE      *MainTableModPtr = NULL;
  UINTN                   *CpmTableList;
  UINTN                   Index;
  AMD_CPM_PCIE_CLOCK_TABLE *ClockTablePtr = NULL;
  AMD_CPM_PCIE_CLOCK_TABLE *ClockTableModPtr = NULL;

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-Start\n", __FUNCTION__));
  DEBUG((DEBUG_INFO, "OEM-PEI-%a-From-AmdCpmOemInitPeimEntryPoint", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-NotifyPpi-gEfiPeiSmbus2PpiGuid\n"));
  RECORD_TIME_PEI (BeginAmdCpmOemInitPeimDriver);
  DEBUG((DEBUG_INFO, "OEM-PEI-%a-GetAmdPbsConfiguration\n", __FUNCTION__));
  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  if (PcdGetBool (PcdLegacyFree) && PcdGetBool (PcdSerialIoDecode)) {
    UINT32  Address;
    UINT32  Data;
    Address = (UINT32)PcdGet64 (PcdPciExpressBaseAddress) + (0x0 << 20) + (0x14 << 15) + (0x3 << 12) + 0x44;
    Data = MmioRead32 (Address);
    switch (PcdGet64 (PcdSerialRegisterBase)) {
    case 0x3F8:
      Data |= BIT6;
      MmioWrite32 (Address, Data);
      break;
    case 0x2F8:
      Data |= BIT7;
      MmioWrite32 (Address, Data);
      break;
    case 0x2E8:
      Data |= BIT11;
      MmioWrite32 (Address, Data);
      break;
    case 0x3E8:
      Data |= BIT13;
      MmioWrite32 (Address, Data);
      break;
    }
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-LocatePpi-gAmdCpmOemTablePpiGuid\n", __FUNCTION__));
  Status = (*PeiServices)->LocatePpi (
                               (CPM_PEI_SERVICES**)PeiServices,
                               &gAmdCpmOemTablePpiGuid,
                               0,
                               NULL,
                               (VOID**)&AmdCpmOemTablePpi
                               );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-LocatePpi-gAmdCpmOemTablePpiGuid-Fail\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-AllocatePool-MainTableModPtr\n", __FUNCTION__));
    Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_CPM_MAIN_TABLE),
                             (VOID**)&MainTableModPtr
                             );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }
    MainTablePtr        = gCpmTableList[0];
    CopyMem (MainTableModPtr, MainTablePtr, sizeof (AMD_CPM_MAIN_TABLE));
    if (PcdGet32 (PcdAmdEnvironmentFlag) == 0) {
    DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.KbcSupport = 0x%x\n", AmdPbsConfiguration.KbcSupport));
    MainTableModPtr->KbcSupport = AmdPbsConfiguration.KbcSupport;
      if  ((PcdGet32 (PcdAmdEnvironment98) & BIT16) == 0) {
        MainTableModPtr->EcSupport = 0;
        MainTableModPtr->KbcSupport = 0;
      }
    } else {
      MainTableModPtr->EcSupport = 0;
      MainTableModPtr->KbcSupport = 0;
    }

    DEBUG((DEBUG_INFO, "OEM-PEI-%a-AllocatePool-CpmTableList-Size:0x%x\n", __FUNCTION__,sizeof(gCpmTableList)));
    Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof(gCpmTableList),
                             (VOID**)&CpmTableList
                             );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-3-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }
    ZeroMem(CpmTableList, sizeof(gCpmTableList));
    CpmTableList[0] = (UINTN) MainTableModPtr;

    Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_CPM_PCIE_CLOCK_TABLE),
                             (VOID**)&ClockTableModPtr
                             );

    ClockTablePtr = gCpmTableList[11];
    CopyMem (ClockTableModPtr, ClockTablePtr, sizeof (AMD_CPM_PCIE_CLOCK_TABLE));

    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (!EFI_ERROR (Status)) {
      if (AmdPbsConfiguration.dGpuHotPlugEnable == 1) {
        ClockTableModPtr->Item[0].ClkReq = CLK_DISABLE;
        ClockTableModPtr->Item[0].ClkId = GPP_SKIP;
      }
      if (AmdPbsConfiguration.GppHotPlugEnable == 1) {
        ClockTableModPtr->Item[2].ClkReq = CLK_DISABLE;
        ClockTableModPtr->Item[2].ClkId = GPP_SKIP;
      }
    }
    CpmTableList[11] = (UINTN) ClockTableModPtr;

    for (Index = 1; Index < sizeof (gCpmTableList)/sizeof(VOID*); Index ++) {
      if (Index == 11) continue;

      DEBUG((DEBUG_INFO, "OEM-PEI-%a-AssignNewTableList[0x%x]\n", __FUNCTION__, Index));
      CpmTableList[Index] = (UINTN)gCpmTableList[Index];
    }

    DEBUG((DEBUG_INFO, "OEM-PEI-%a-AllocatePool-AmdCpmOemTablePpi\n", __FUNCTION__));
    Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_CPM_OEM_TABLE_PPI),
                             (VOID**)&AmdCpmOemTablePpi
                             );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-4-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    AmdCpmOemTablePpi->Revision = AMD_CPM_OEM_REVISION;
    AmdCpmOemTablePpi->PlatformId = AMD_CPM_PLATFORM_ID_DEFAULT;
    AmdCpmOemTablePpi->TableList = CpmTableList;

    DEBUG((DEBUG_INFO, "OEM-PEI-%a-AllocatePool-PpiListCpmOemTable\n", __FUNCTION__));
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (EFI_PEI_PPI_DESCRIPTOR),
                               (VOID**)&PpiListCpmOemTable
                               );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-5-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    PpiListCpmOemTable->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    PpiListCpmOemTable->Guid = &gAmdCpmOemTablePpiGuid;
    PpiListCpmOemTable->Ppi = AmdCpmOemTablePpi;
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-InstallPpi-gAmdCpmOemTablePpiGuid\n", __FUNCTION__));
    Status = (*PeiServices)->InstallPpi (
                                PeiServices,
                                PpiListCpmOemTable
                                );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-6-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    DEBUG((DEBUG_INFO, "OEM-PEI-%a-NotifyPpi-gAmdCpmTablePpiGuid", __FUNCTION__));
    DEBUG((DEBUG_INFO, "-For-AmdCpmOverrideTableNotifyCallback\n"));
    Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmOemTableOverrideNotify);
  }

  RECORD_TIME_PEI (EndAmdCpmOemInitPeimDriver);

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-7-Status=%r\n", __FUNCTION__, Status));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback Function to Override CPM OEM Definition Tables
 *
 * @param[in]     PeiServices       The PEI core services table.
 * @param[in]     NotifyDescriptor  The descriptor for the notification event.
 * @param[in]     Ppi               Pointer to the PPI in question
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmOverrideTableNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  EFI_STATUS                            Status;
  AMD_CPM_OVERRIDE_TABLE_PPI            *AmdCpmOverrideTablePpi;
  EFI_PEI_PPI_DESCRIPTOR                *PpiListCpmOverrideTablePtr;

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-Start\n", __FUNCTION__));
  DEBUG((DEBUG_INFO, "OEM-PEI-%a-From-AmdCpmOemInitPeimNotifyCallback", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-NotifyPpi-gAmdCpmTablePpiGuid\n"));
  RECORD_TIME_PEI (BeginAmdCpmOemTableOverride);

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-AmdCpmTableOverride\n", __FUNCTION__));
  Status = AmdCpmTableOverride (PeiServices);

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-AllocatePool-AmdCpmOverrideTablePpi\n", __FUNCTION__));
  Status = (*PeiServices)->AllocatePool (
                              (CPM_PEI_SERVICES**)PeiServices,
                              sizeof (AMD_CPM_OVERRIDE_TABLE_PPI),
                              (VOID**)&AmdCpmOverrideTablePpi
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-AllocatePool-PpiListCpmOverrideTablePtr\n", __FUNCTION__));
  Status = (*PeiServices)->AllocatePool (
                              (CPM_PEI_SERVICES**)PeiServices,
                              sizeof (EFI_PEI_PPI_DESCRIPTOR),
                              (VOID**)&PpiListCpmOverrideTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  PpiListCpmOverrideTablePtr->Flags   = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListCpmOverrideTablePtr->Guid    = &gAmdCpmOverrideTablePpiGuid;
  PpiListCpmOverrideTablePtr->Ppi     = AmdCpmOverrideTablePpi;

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-InstallPpi-gAmdCpmOverrideTablePpiGuid\n", __FUNCTION__));
  Status = (*PeiServices)->InstallPpi (
                              (CPM_PEI_SERVICES**)PeiServices,
                              PpiListCpmOverrideTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-3-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  RECORD_TIME_PEI (EndAmdCpmOemTableOverride);
  DEBUG((DEBUG_INFO, "OEM-PEI-%a-End-4-Status=%r\n", __FUNCTION__, Status));
  return Status;
}


/**
 *
 * This function Programs MAC addresses to Ancillary Data Structure.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdXgbeMacAddr (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_PEI_M24LC128_PPI            *M24Lc128Ppi;
  UINT8                            MacBuffer[12];
  UINT32                           MacAddrVal;
  UINT64                           Value;
  UINT8                           *MacPointer;
  DXIO_PORT_DESCRIPTOR            *EngineDescriptor;
  DXIO_COMPLEX_DESCRIPTOR         *PcieTopologyData;
  PEI_AMD_NBIO_PCIE_COMPLEX_PPI   *NbioPcieComplexPpi;
  EFI_STATUS  Status = EFI_SUCCESS;

  UINT8 Index;
  //UINT8    IOMUXx13Value,IOMUXx14Value;

  DEBUG((EFI_D_ERROR, "Inside AmdXgbeMacAddr\n"));

  // store the IOMUX values; later these values will be restored
  //IOMUXx13Value = MmioRead8(FCH_IOMUXx13_SCL1_I2C3_SCL_EGPIO19);
 // IOMUXx14Value = MmioRead8(FCH_IOMUXx14_SDA1_I2C3_SDA_EGPIO20);

  // Enable I2C3 CLK and Data
 // MmioWrite8(FCH_IOMUXx13_SCL1_I2C3_SCL_EGPIO19,1);   //Programming IOMUX to enable I2C3_SCL
  //MmioWrite8(FCH_IOMUXx14_SDA1_I2C3_SDA_EGPIO20,1);   //Programming IOMUX to enable I2C3_SDA

  Status = (*PeiServices)->LocatePpi (PeiServices, &gM24Lc128PpiGuid, 0, NULL, &M24Lc128Ppi);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Could not locate gM24Lc128PpiGuid\n"));
    goto Exit;
  }

  Status = M24Lc128Ppi->Read (PeiServices, 3, 0x50, 0x51, 12, MacBuffer);
  DEBUG((EFI_D_ERROR, "M24Lc128Ppi->Read Status: %r\n",Status));
  MacPointer = MacBuffer;

  for (Index = 0; Index < 12; Index++)
    DEBUG((EFI_D_ERROR, "MacBuffer[%d] = %x\n", Index, MacBuffer[Index]));
  //
  //  Update the DXIO table MAC address
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioPcieComplexPpiGuid, 0, NULL, &NbioPcieComplexPpi);
  if (EFI_ERROR (Status)) {
     DEBUG((EFI_D_ERROR, "Could not locate gAmdNbioPcieComplexPpiGuid\n"));
     goto Exit;
  }
  NbioPcieComplexPpi->PcieGetComplex(NbioPcieComplexPpi, &PcieTopologyData);

  EngineDescriptor = PcieTopologyData->PciePortList;
  while (EngineDescriptor != NULL) {
    if (EngineDescriptor->EngineData.EngineType == DxioEthernetEngine) {
        // Initialize this descriptor for Port 0
        MacAddrVal = *((UINT32*) MacPointer);
        EngineDescriptor->EtherNet.MacAddressLo = MacAddrVal;  //Read the low value of the mac address here...;
        MacPointer += 4;
        MacAddrVal = *((UINT32*) MacPointer);
        MacAddrVal &= 0xFFFF;
        MacAddrVal |= 0x80000000;  // Set Valid
        EngineDescriptor->EtherNet.MacAddressHi = MacAddrVal;  //Read the high value of the mac address here...;
        MacPointer += 2;
    }
    EngineDescriptor = DxioInputParserGetNextDescriptor (EngineDescriptor);
  }
 //
 // Update Mac Address PCDs
 //
  MacPointer = MacBuffer;
  Value = *((UINT32*) (MacPointer + 4));
  DEBUG((EFI_D_ERROR, "DEBUG1 = 0x%lx\n", Value));
  Value &= 0xFFFF;
  Value = (Value << 32);
  DEBUG((EFI_D_ERROR, "DEBUG2 = 0x%lx\n", Value));
  Value |=  *((UINT32*) MacPointer);
  DEBUG((EFI_D_ERROR, "DEBUG3 = 0x%lx\n", Value));

  PcdSet64S(PcdXgbePort0MAC, Value);
  DEBUG((EFI_D_ERROR, "PcdXgbePort0MAC = 0x%lx\n", Value));
  MacPointer +=6;

  Value = *((UINT32*) (MacPointer + 4));
  Value &= 0xFFFF;
  Value =  (Value << 32);
  Value |=  *((UINT32*) MacPointer);

  PcdSet64S(PcdXgbePort1MAC,Value);
  DEBUG((EFI_D_ERROR, "PcdXgbePort1MAC  = 0x%lx\n", Value));

Exit:
  // Restore the IOMUX values
 // MmioWrite8(FCH_IOMUXx13_SCL1_I2C3_SCL_EGPIO19,IOMUXx13Value);
  //MmioWrite8(FCH_IOMUXx14_SDA1_I2C3_SDA_EGPIO20,IOMUXx14Value);

  return Status;
}
/**
 *
 * This function resets the external MDIO PHY.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    PHY Reset complete
 * @retval        EFI_ERROR      PHY Reset failed.
 */
EFI_STATUS
EFIAPI
XgbeMdioPhyReset (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  UINT16 Value = 0x0;
  EFI_STATUS Status = EFI_SUCCESS;
  AMD_CPM_TABLE_PPI* AmdCpmTablePpi = NULL;

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmOemInitPpiTableNotifyCallback, XgbeMdioPhyReset, Status = %r\n", Status));
    return Status;
  }
  //
  // GPIO 27 is used to reset the external PHY. By default, this signal is driven High.  To start the PHY in a known
  // state, reset the PHY by driving this Low, and pull it back to High state.
  //
  Value = GPIO_CONTROL_REG(GPIO_FUNCTION_0, GPIO_OUTPUT_LOW, GPIO_PU_PD_DIS, GPIO_STICKY_DIS);
  DEBUG((DEBUG_INFO, "Value(Low) = %x\n", Value));
  AmdCpmTablePpi->CommonFunction.SetGpio(AmdCpmTablePpi, PHY_RESET_GPIO, Value);

  // wait for 10msec. This delay is recommended by Marvell.
  AmdCpmTablePpi->CommonFunction.Stall(AmdCpmTablePpi, PHY_RESET_DELAY);

  //
  // Pull reset to High state
  //
  Value = GPIO_CONTROL_REG(GPIO_FUNCTION_0, GPIO_OUTPUT_HIGH, GPIO_PU_PD_DIS, GPIO_STICKY_DIS);
  DEBUG((DEBUG_INFO, "Value(High) = %x\n", Value));
  AmdCpmTablePpi->CommonFunction.SetGpio(AmdCpmTablePpi, PHY_RESET_GPIO, Value);

  return Status;
}

/**
 *
 * This function configures the RFMUX to enable DP Ports 2 and 3.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Successfully Enabled DP Ports.
 * @retval        EFI_ERROR      Failed to enable DP Ports.
 */


/**
 *
 * This function Changes the default functions of GPIOs 19 and 20 from I2C to SMBUS.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Successfully Enabled DP Ports.
 * @retval        EFI_ERROR      Failed to enable DP Ports.
 */
EFI_STATUS
EFIAPI
ProgramI2CGpioToSmbus (
  IN CONST EFI_PEI_SERVICES** PeiServices
  )
{

  UINT16 Value = 0x0;
  EFI_STATUS Status = EFI_SUCCESS;
  AMD_CPM_TABLE_PPI* AmdCpmTablePpi = NULL;

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmOemInitPpiTableNotifyCallback, ProgramI2CGpioToSmbus, Status = %r\n", Status));
    return Status;
  }

  Value = GPIO_CONTROL_REG(GPIO_FUNCTION_0, GPIO_NA, GPIO_PU_EN, GPIO_STICKY_DIS);
  DEBUG((DEBUG_INFO, "GPIO 19 Value = %x\n", Value));
  AmdCpmTablePpi->CommonFunction.SetGpio(AmdCpmTablePpi, I2C_SMBUS_GPIO_19, Value);

  Value = GPIO_CONTROL_REG(GPIO_FUNCTION_0, GPIO_NA, GPIO_PU_EN, GPIO_STICKY_DIS);
  DEBUG((DEBUG_INFO, "GPIO 20 Value = %x\n", Value));
  AmdCpmTablePpi->CommonFunction.SetGpio(AmdCpmTablePpi, I2C_SMBUS_GPIO_20, Value);

  return Status;
}

/**
 *
 * Callback  function to execute when gAmdCpmGpioInitFinishedPpiGuid is published.
 * This PPI is published when the default GPIO programming and its associated functionality
 * as defined in the OemTable is completed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Successfully Enabled DP Ports.
 * @retval        EFI_ERROR      Failed to enable DP Ports.
 */
EFI_STATUS
EFIAPI
AmdNbioPcieComplexPpiNotifyCallback(
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN  VOID                            *Ppi
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG((DEBUG_INFO, "In AmdNbioPcieComplexPpiNotifyCallback\n"));

  //ProgramMacAddress(PeiServices);
  Status = AmdXgbeMacAddr(PeiServices);
  return Status;
}

EFI_STATUS
EFIAPI
AmdCpmOemInitPpiTableNotifyCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDescriptor,
  IN  VOID                            *Ppi
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG((DEBUG_INFO, "In AmdCpmOemInitPpiTableNotifyCallback\n"));
  RECORD_TIME_PEI (BeginAmdCpmOemInitPeimDriver);

#if 0
  //
  // The FCH code enables these GPIOs to function as I2C rather than SMBUS. If we want to switch to SMBUS
  // functionality, we need to disable the I2C 3 in the FchRTDeviceEnableMap which disables the visibilty
  // from OS. Currently keeping the functionality of these GPIO pins to SMBUS as of now.
  //
  //
  // Change GPIO19 and GPIO20 to default functionality (Function 0) after we are done with reading the EEPROM.
  //
  Status = ProgramI2CGpioToSmbus(PeiServices);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "AmdCpmOemInitPpiTableNotifyCallback, ProgramI2CGpioToSmbus failed, Status = %r\n", Status));
    return Status;
  }
#endif

  Status = XgbeMdioPhyReset(PeiServices);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "AmdCpmOemInitPpiTableNotifyCallback, XgbeMdioPhyReset failed, Status = %r\n", Status));
    return Status;
  }

//. Status = ConfigureRfMuxforDPPorts(PeiServices);
//  if (EFI_ERROR(Status)) {
//    DEBUG((DEBUG_INFO, "AmdCpmOemInitPpiTableNotifyCallback, ConfigureRfMuxforDPPorts failed, Status = %r\n", Status));
 //   return Status;
 // }

  RECORD_TIME_PEI (EndAmdCpmOemInitPeimDriver);

  DEBUG((DEBUG_INFO, "AmdCpmOemInitPpiTableNotifyCallback, Status = %r\n", Status));
  return Status;
}


