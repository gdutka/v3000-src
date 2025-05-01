/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTWIFI_KTWIFIENV_FILECODE


/**
 * FchInitEnvWifi - Config Wifi controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvWifi (
  IN  VOID     *FchDataPtr
  )
{
  UINT32                 Reg32;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  Reg32 = 0;

  if (LocalCfgPtr->Misc.AmdEnvironmentFlag == 0) {
      //
      // WLAN is disabled on RN.
      //  need to check if we still need WOL/WOB programming
      //
      if (LocalCfgPtr->Wifi.WifiWOLEnable) {
        Reg32 |= BIT16;
      }

      if (LocalCfgPtr->Wifi.WifiWOBEnable) {
        Reg32 |= BIT17;
      }

      FchSmnRW (0, 0x05800010, ~(UINT32) (BIT16 + BIT17), Reg32, NULL); //CONTROL_STATUS_REG

      //
      // WLAN is disabled
      //
      FchInitWifiKTPowerSaving (FchDataPtr);
  }
}

VOID
FchInitWifiKTPowerSaving (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  // Program mmacpi_gppclkcntrol[23] = 1
  // Program mmacpi_clkoutputcntrl[20] = 0
  // Program PMx70[15] = 1
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00, AccessWidth32, ~(UINT32) BIT23, BIT23);
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG04, AccessWidth32, ~(UINT32) BIT20, 0x00);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG70, AccessWidth32, ~(UINT32) BIT15, BIT15);

  // Program WLAN_PHY_GLB_SETTING_CTRL4 [0] = 1
  // Program wlan_wlar_reg ::WLAN_PWR_STATE_REG[0]: S0_STATE_REG = 0
  // Program WLAN_CLK_REG[1] = 1'b0, and WLAN_CLK_REG[10] = 1'b0
  // Program WLAN_STOP_CLK_REG [STOP_CLK_BT, STOP_CLK_BT, STOP_CLK_GPIO] = 3'b111
  // Program PMx70[15] = 0
  FchSmnRW (0, FCH_KT_WLAN_PHY_GLB_SETTING_CTRL4, ~(UINT32)BIT0, BIT0, NULL);
  FchSmnRW (0, FCH_KT_WLAN_PWR_STATE_REG, ~(UINT32)BIT0, 0x00, NULL);
  FchSmnRW (0, FCH_KT_WLAN_CLK_REG, ~(UINT32) (BIT1 + BIT10), 0x00, NULL);
  FchSmnRW (0, FCH_KT_WLAN_STOP_CLK_REG, ~(UINT32) (BIT0 + BIT1 + BIT2), BIT1 + BIT2, NULL);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG70, AccessWidth32, ~(UINT32) BIT15, 0x00);

  // Program mmacpi_clkoutputcntrl[20] = 1
  // Program mmacpi_clkoutputcntrl[26] = 1
  // Program mmacpi_gppclkcntrol[23] = 0
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG04, AccessWidth32, ~(UINT32) (BIT20 + BIT26), BIT20 + BIT26);
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00, AccessWidth32, ~(UINT32) BIT23, 0x00);
}

