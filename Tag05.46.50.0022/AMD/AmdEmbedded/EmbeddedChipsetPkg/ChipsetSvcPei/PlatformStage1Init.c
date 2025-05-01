/** @file
 PEI Chipset Services Library.

 This file contains only one function that is PeiCsSvcPlatformStage1Init().
 The function PeiCsSvcPlatformStage1Init() use chipset services to be
 Platform initialization in PEI phase stage 1.

***************************************************************************
* Copyright (c) 2020 - 2021, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PciExpressLib.h>
#include <Library/ChipsetConfigLib.h>
#include <Library/VariableLib.h>
#include <ChipsetSetupConfig.h>
#include <Hudson-2Reg.h>
#include <TpmPolicy.h>

/**
  Check if DTPM establishment is required.

  @retval TRUE        Required
  @retval FALSE       Not required
**/
STATIC
BOOLEAN
IsEstablishDtpmRequired (
  VOID
  )
{
  if (!FeaturePcdGet (PcdH2OPreferDtpmBootSupported)) {
    return FALSE;
  }

  //
  // Check if system is on the first boot.
  //
  return !IsVariableInVariableStoreRegion (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid);
}

/**
  Manage decoder in order to detect DTPM.

  @param[in,out] SystemConfiguration       Pointer to Setup variable.

  @retval EFI_SUCCESS          Operation completed successfully
  @retval Others               Operation failed
**/
STATIC
VOID
EstablishDtpm (
  IN OUT CHIPSET_CONFIGURATION    *SystemConfiguration
  )
{
  UINT8       TpmData;
  //
  // Detect DTPM exist or not, also distinguish it's type
  //
  DEBUG ((EFI_D_INFO, "Enable DTPM decorder and check if TPM presented\n"));
  //
  // Enable DTPM decode
  //
  PciExpressOr8 (
    PCI_EXPRESS_LIB_ADDRESS (
      FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      R_FCH_LPC_TPM_REGISTER
    ),
    (V_TPM12_ENABLE + V_TPM_LEGACY_ENABLE + V_WIDER_TPM_ENABLE)
    );

  TpmData = MmioRead8 ((UINTN) PcdGet64 (PcdTpmBaseAddress));
  if (TpmData != 0xFF) {
    if (TpmData != 0) {
      SystemConfiguration->EnableTPM = TPM_CHIP_DEVICE_ENABLE;
      SystemConfiguration->SpiOrLpcTPM = 0; // 0: LPC TPM  1: SPI TPM
      DEBUG ((EFI_D_INFO, "DTPM Type : LPC\n"));
    } else {
      //
      // Detect SPI TPM.
      // Enable RouteTpm2Spi
      //
      PciExpressOr8 (
        PCI_EXPRESS_LIB_ADDRESS (
          FCH_LPC_BUS,
          FCH_LPC_DEV,
          FCH_LPC_FUNC,
          R_FCH_LPC_SPI_BASE_ADDRESS
        ),
        V_ROUTE_TPM_TO_SPI
      );
      TpmData = MmioRead8 ((UINTN) PcdGet64 (PcdTpmBaseAddress));
      if ((TpmData != 0xFF) && (TpmData != 0)) {
        SystemConfiguration->EnableTPM = TPM_CHIP_DEVICE_ENABLE;
        SystemConfiguration->SpiOrLpcTPM = 1;
        DEBUG ((EFI_D_INFO, "DTPM Type : SPI\n"));
      }
      //
      // Disabling RouteTpm2Spi
      //
      PciExpressAnd8 (
        PCI_EXPRESS_LIB_ADDRESS (
          FCH_LPC_BUS,
          FCH_LPC_DEV,
          FCH_LPC_FUNC,
          R_FCH_LPC_SPI_BASE_ADDRESS
        ),
        (UINT8)(~V_ROUTE_TPM_TO_SPI)
      );
    }
  }
  //
  // Detection done
  // Disabling TPM decoder
  //
  PciExpressAnd8 (
    PCI_EXPRESS_LIB_ADDRESS (
      FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      R_FCH_LPC_TPM_REGISTER
    ),
    (UINT8)(~(V_TPM12_ENABLE + V_TPM_LEGACY_ENABLE + V_WIDER_TPM_ENABLE))
    );
}

EFI_STATUS
EnableTpm (
  IN CHIPSET_CONFIGURATION*  SystemConfiguration
  )
/*++
Routine Description:
  Enable TPM Decode
Arguments:
  PeiServices - PEI Service pointer.
Returns:
  None
--*/
{
  UINT32    SpiBaseAddress;
  UINT16    *SpiTpmRegister;
  UINT16    SpiTpmRegistrVaule;
  UINT32    SetTpmPolicy;
  EFI_STATUS  Status;

  DEBUG ((EFI_D_ERROR, "Configure TPM decoder... %x\n", SystemConfiguration->EnableTPM));

  if (SystemConfiguration->SetupVariableInvalid) {
    DEBUG ((EFI_D_ERROR, "SetupVariableInvalid deteted \n"));
    SystemConfiguration->EnableTPM = TPM_ALL_DISABLE;
  }

  //
  // Perfer using DTPM on the first boot
  // Process if required
  //
  if (IsEstablishDtpmRequired ()) {
    EstablishDtpm (SystemConfiguration);
  }

  if (SystemConfiguration->EnableTPM != TPM_ALL_DISABLE) {
    Status = PcdSet8S (PcdAmdPspSystemTpmAlgSmx, 1);
    ASSERT_EFI_ERROR (Status);
  }

  if (SystemConfiguration->EnableTPM == TPM_CHIP_DEVICE_ENABLE) {
    DEBUG ((EFI_D_ERROR, "Enabling TPM decoder...\n"));
    //Stall time depends on project request.
    //Stall->Stall (PeiServices, NULL, 100000);
    PciExpressOr8 (
      PCI_EXPRESS_LIB_ADDRESS (
        FCH_LPC_BUS,
        FCH_LPC_DEV,
        FCH_LPC_FUNC,
        R_FCH_LPC_TPM_REGISTER
      ),
      (V_TPM12_ENABLE + V_TPM_LEGACY_ENABLE + V_WIDER_TPM_ENABLE)
    );

    if(PcdGet8(PcdH2OSpiOrLpcTPMSupported)) {          // PcdsDynamicHii type 1: SPI TPM (default)  0: LPC TPM      Defined in AdvanceVfr.vfr
      DEBUG ((EFI_D_ERROR, "PcdH2OSpiOrLpcTPMSupported:1 - SPI TPM\n"));
      // Enable RouteTpm2Spi
      PciExpressOr8 (
        PCI_EXPRESS_LIB_ADDRESS (
          FCH_LPC_BUS,
          FCH_LPC_DEV,
          FCH_LPC_FUNC,
          R_FCH_LPC_SPI_BASE_ADDRESS
        ),
        V_ROUTE_TPM_TO_SPI
      );

      // Config SPI_TPM_CS_L
      MmioWrite8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x1D, 0);

      SpiBaseAddress = PciExpressRead32 (
                         PCI_EXPRESS_LIB_ADDRESS (
                           FCH_LPC_BUS,
                           FCH_LPC_DEV,
                           FCH_LPC_FUNC,
                           R_FCH_LPC_SPI_BASE_ADDRESS
                         )
                       );
      SpiBaseAddress = SpiBaseAddress & 0xFFFFFF00; //bit 0~7 disable
      SpiTpmRegister      = (UINT16*) (SpiBaseAddress + FCH_SPI_MMIO_REG22);
      SpiTpmRegistrVaule  = *SpiTpmRegister & 0xFFF0; //Bit 0~3 diable
      SpiTpmRegistrVaule  = SpiTpmRegistrVaule | 0x03;      // 16.66MHz
      *SpiTpmRegister     = SpiTpmRegistrVaule;
    } else {
      DEBUG ((EFI_D_ERROR, "PcdH2OSpiOrLpcTPMSupported:0 - LPC TPM\n"));
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Disabling TPM decoder...\n"));
    PciExpressAnd8 (
      PCI_EXPRESS_LIB_ADDRESS (
        FCH_LPC_BUS,
        FCH_LPC_DEV,
        FCH_LPC_FUNC,
        R_FCH_LPC_TPM_REGISTER
      ),
      (UINT8) (~(V_TPM12_ENABLE | V_TPM_LEGACY_ENABLE | V_WIDER_TPM_ENABLE))
    );
  }

  if (SystemConfiguration->EnableTPM == TPM_FIRMWARE_DEVICE_ENABLE ||
     SystemConfiguration->EnableTPM == HSP_TPM_FIRMWARE_DEVICE_ENABLE) {
    DEBUG ((EFI_D_ERROR, "Set fTPM Pcds...\n"));
    SetTpmPolicy = PcdGet32(PcdPeiTpmPolicy) | TPM2_STARTUP_IN_MP;
    Status = PcdSet32S(PcdPeiTpmPolicy, SetTpmPolicy);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // If TPM doesn't exist, also System guard feature doesn't be enabled.
  // Then TPM NV Index should not be set.
  //
  if (PcdGetBool (PcdSmmProtectionFeature) && (PcdGet8(PcdH2OTpmType) != 0)) {
    Status = PcdSet32S (PcdDxeTpmPolicy, (PcdGet32(PcdDxeTpmPolicy) | BIT30));
    ASSERT_EFI_ERROR (Status);
  } else {
    Status = PcdSet32S (PcdDxeTpmPolicy, (PcdGet32(PcdDxeTpmPolicy) & (~BIT30)));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EnableSerialPort (
  )
{
  PciExpressOr8 (
    PCI_EXPRESS_LIB_ADDRESS (FCH_LPC_BUS,
    FCH_LPC_DEV,
    FCH_LPC_FUNC,
    R_FCH_LPC_IO_PORT_DECODE_ENABLE_REGISTER1),
    V_SERIAL_PORT_ENABLE_0_ENABLE
  );
  return EFI_SUCCESS;
}

/**
 Platform initialization in PEI phase stage 1.

 @param[in]         None

 @retval            EFI_SUCCESS         This function alway return successfully
*/
EFI_STATUS
PlatformStage1Init (
  VOID
  )
{
  EFI_STATUS                       Status;
  CHIPSET_CONFIGURATION            ChipsetConfiguration;

  EnableSerialPort ();

  DEBUG ((EFI_D_ERROR, "\nPlatformStage1Init Start\n"));

  Status = GetChipsetConfiguration(&ChipsetConfiguration);
  if (!EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "  GetChipsetConfiguration:%r\n",Status));
    Status = PcdSet8S(PcdH2OSvmEnable, ChipsetConfiguration.EnableSVM);
    ASSERT_EFI_ERROR (Status);

    Status = PcdSet8S(PcdH2OSvmLock, ChipsetConfiguration.SVMLock);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((EFI_D_ERROR, "    PcdH2OSvmEnable: %x\n", PcdGet8 (PcdH2OSvmEnable)));
    DEBUG ((EFI_D_ERROR, "    PcdH2OSvmLock: %x\n", PcdGet8 (PcdH2OSvmLock)));

    DEBUG ((EFI_D_ERROR, "    AmdPspKvmControl:%x\n",ChipsetConfiguration.AmdPspKvmControl));
    if (ChipsetConfiguration.AmdPspKvmControl) {
      Status = PcdSetBoolS(PcdAmdPspKvmEnable, 1);
      ASSERT_EFI_ERROR (Status);
    } else {
      Status = PcdSetBoolS(PcdAmdPspKvmEnable, 0);
      ASSERT_EFI_ERROR (Status);
    }

    DEBUG ((EFI_D_INFO, "    EnableSmmProtection:%x\n",ChipsetConfiguration.EnableSmmProtection));
    Status = PcdSetBoolS (PcdSmmProtectionFeature, ChipsetConfiguration.EnableSmmProtection);
    ASSERT_EFI_ERROR (Status);

//[-start-221003-IB14740266-add]//
    Status = PcdSetBoolS(PcdSrIovSupport, (BOOLEAN)ChipsetConfiguration.PCIeSRIOVSupport);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Set PcdSrIovSupport Fail, Status : %r\n", Status));
    }
//[-end-221003-IB14740266-add]//

    if (FeaturePcdGet(PcdH2OTpmSupported) || FeaturePcdGet(PcdH2OTpm2Supported)) {
      EnableTpm (&ChipsetConfiguration);
    }

//[-start-230817-IB20840016-modify]//
    Status = PcdSet8S (PcdScpcLevel, ChipsetConfiguration.ScpcLevels);
    ASSERT_EFI_ERROR (Status);
    //Not impact graying out logic for CBS related options when Level3 is enabled
    if ((ChipsetConfiguration.ScpcLevels > 3) && (ChipsetConfiguration.ScpcLevels < 7)) {
      Status = PcdSet8S (PcdScpcLevel, ChipsetConfiguration.ScpcLevels - 3);
      ASSERT_EFI_ERROR (Status);
      Status = PcdSetBoolS (PcdAmdHspCoreEnable, 1);
      ASSERT_EFI_ERROR (Status);
    }
    if ((ChipsetConfiguration.ScpcLevels > 6) && (ChipsetConfiguration.ScpcLevels < 10)) {
      Status = PcdSet8S (PcdScpcLevel, ChipsetConfiguration.ScpcLevels - 6);
      ASSERT_EFI_ERROR (Status);
      Status = PcdSetBoolS (PcdAmdHspCoreEnable, 1);
      ASSERT_EFI_ERROR (Status);
    }

    switch (PcdGet8 (PcdScpcLevel)) {
      case 1:
        Status = PcdSetBoolS (PcdCfgIommuSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        break;
      case 2:
        Status = PcdSetBoolS (PcdCfgIommuSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdIvInfoDmaReMap, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdDmaProtection, TRUE);
        ASSERT_EFI_ERROR (Status);
        break;
      case 3:
        Status = PcdSetBoolS (PcdCfgIommuSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdIvInfoDmaReMap, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdDmaProtection, TRUE);
        ASSERT_EFI_ERROR (Status);
        Status = PcdSetBoolS (PcdModernStandbyEnableForLevel3, TRUE);
        ASSERT_EFI_ERROR (Status);
        break;
    }
//[-end-230817-IB20840016-modify]//

    DEBUG ((EFI_D_INFO, "    SMMCoreLock:%x\n",ChipsetConfiguration.SMMCoreLock));
    Status = PcdSetBoolS (PcdAmdSmmLock, ChipsetConfiguration.SMMCoreLock);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((EFI_D_INFO, "    SmmIsolationSupport:%x\n",ChipsetConfiguration.SmmIsolationSupport));
    Status = PcdSetBoolS (PcdSmmIsolationEnable, ChipsetConfiguration.SmmIsolationSupport);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSetBoolS (PcdAmdPspDrtmVirtualDevice, ChipsetConfiguration.SmmIsolationSupport);
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((EFI_D_ERROR, "PlatformStage1Init End\n"));

  return EFI_SUCCESS;
}
