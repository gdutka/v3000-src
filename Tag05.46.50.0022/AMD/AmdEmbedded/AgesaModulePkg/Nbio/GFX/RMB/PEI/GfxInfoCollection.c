/*
*****************************************************************************
*
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
* ***************************************************************************
*
*/
#include <PiDxe.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <NbioGfxV4.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include "GfxEnumConnectors.h"
#include <Library/FabricRegisterAccLib.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>

#define FILECODE        NBIO_GFX_RMB_PEI_GFXINFOCOLLECTION_FILECODE

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Dump  display path settings
 *
 *
 *
 * @param[in] DisplayPath            Display path
 */

VOID
GfxDebugDumpDisplayPath (
  IN       EXT_DISPLAY_PATH            *DisplayPath
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "     usDeviceTag = 0x%x\n",
   DisplayPath->usDeviceTag
   );
  IDS_HDT_CONSOLE (GNB_TRACE, "     usDeviceACPIEnum = 0x%x\n",
   DisplayPath->usDeviceACPIEnum
   );
  IDS_HDT_CONSOLE (GNB_TRACE, "     usDeviceConnector = 0x%x\n",
   DisplayPath->usDeviceConnector
   );

  IDS_HDT_CONSOLE (GNB_TRACE, "     ucExtAUXDDCLutIndex = 0x%x\n",
   DisplayPath->ucExtAUXDDCLutIndex
   );

  IDS_HDT_CONSOLE (GNB_TRACE, "     ucExtHPDPINLutIndex = 0x%x\n",
   DisplayPath->ucExtHPDPINLutIndex
   );

  IDS_HDT_CONSOLE (GNB_TRACE, "     usExtEncoderObjId = 0x%x\n",
   DisplayPath->usExtEncoderObjId
   );
  IDS_HDT_CONSOLE (GNB_TRACE, "     ucChannelMapping = 0x%x\n",
   DisplayPath->ucChannelMapping
   );
  IDS_HDT_CONSOLE (GNB_TRACE, "     ucChPNInvert = 0x%x\n",
   DisplayPath->ucChPNInvert
   );
  IDS_HDT_CONSOLE (GNB_TRACE, "     usCaps = 0x%x\n",
   DisplayPath->usCaps
   );
}

/**
  Check the DRAM map registers to know is there any non-interleaving region in the system.
  // system_config
  enum atom_system_vbiosmisc_def{
    INTEGRATED_SYSTEM_INFO__GET_EDID_CALLBACK_FUNC_SUPPORT = 0x01,
    INTEGRATED_SYSTEM_INFO__ NON_INTERLEAVING_REGION = 0x80,  // [7] = 0 NON_INTERLEAVING_REGION not exist; [7] = 1 NON_INTERLEAVING_REGION exist
  };

  @retval   TRUE, non-interleaving region exist
  @retval   FALSE, no non-interleaving region exist
**/
BOOLEAN
IsNonInterleavingRegionExist (
  VOID
  )
{
  UINT32                       Index;
  DRAM_BASE_ADDRESS_REGISTER   DramBaseAddr;

  for (Index = 0; Index < RMB_NUMBER_OF_DRAM_REGIONS; Index++) {
    DramBaseAddr.Value = FabricRegisterAccRead (0, 0, DRAMBASEADDR0_FUNC, (DRAMBASEADDR0_REG + (Index * DRAM_REGION_REGISTER_OFFSET)), RMB_IOMS0_INSTANCE_ID);
    if ((DramBaseAddr.Field.AddrRngVal == 1) && (DramBaseAddr.Field.IntLvNumChan == 0)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "  non-interleaving region exist: TRUE\n");
      return TRUE;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "  non-interleaving region exist: FALSE\n");
  return FALSE;
}

/**
  Populate SystemInfoTable with values from PCDs


  @param SystemInfoTable Pointer to a SystemInfoTable. Memory is not pre allocated
**/
VOID
PopulateSystemInfoTable (
  ATOM_FUSION_SYSTEM_INFO_V6      *SystemInfoTable
  )
{
  AGESA_STATUS                    Status;
  UINT8                           Index;
  CONST EFI_PEI_SERVICES          **PeiServices;

  IDS_HDT_CONSOLE (GNB_TRACE, "PopulateSystemInfoTable RMB Enter\n");

  ZeroMem (SystemInfoTable, sizeof (ATOM_FUSION_SYSTEM_INFO_V6));
  PeiServices = GetPeiServicesTablePointer();
  Status = AGESA_SUCCESS;

  //
  // Common Table Header Initialize
  //
  SystemInfoTable->sIntegratedSysInfo.table_header.structuresize = sizeof (ATOM_INTEGRATED_SYSTEM_INFO_V2_2);
//  ASSERT (SystemInfoTable->sIntegratedSysInfo.table_header.structuresize == 1024);
  SystemInfoTable->sIntegratedSysInfo.table_header.format_revision = 2;
  SystemInfoTable->sIntegratedSysInfo.table_header.content_revision = 2;
  IDS_HDT_CONSOLE (GNB_TRACE, "ATOM_INTEGRATED_SYSTEM_INFO_V2_2 size[%x] Ver%d.%d\n",
                              SystemInfoTable->sIntegratedSysInfo.table_header.structuresize,
                              SystemInfoTable->sIntegratedSysInfo.table_header.format_revision,
                              SystemInfoTable->sIntegratedSysInfo.table_header.content_revision
                              );

  SystemInfoTable->sIntegratedSysInfo.vbios_misc = 0;   // BLDCFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE
  if (IsNonInterleavingRegionExist ()) {
    SystemInfoTable->sIntegratedSysInfo.vbios_misc |= BIT7; // [7] = 1 NON_INTERLEAVING_REGION exist
  }
  SystemInfoTable->sIntegratedSysInfo.gpucapinfo = 0;   // GPUCAPINFO_TMDS_HDMI_USE_CASCADE_PLL_MODE | GPUCAPINFO_DP_USE_SINGLE_PLL_MODE;
  if (PcdGetBool (PcdCfgSysInfoGpuCapsDdsSupport) == TRUE) {
    SystemInfoTable->sIntegratedSysInfo.gpucapinfo |= SYS_INFO_GPUCAPS__DDS_SUPPORT;
  }
  if (PcdGetBool (PcdCfgSysInfoGpuCapsBr3SdrSupport) == TRUE) {
    SystemInfoTable->sIntegratedSysInfo.gpucapinfo |= SYS_INFO_GPUCAPS__BR3_SDR_SUPPORT;
  }
  SystemInfoTable->sIntegratedSysInfo.system_config = 0;

  SystemInfoTable->sIntegratedSysInfo.cpucapinfo = 0;
  SystemInfoTable->sIntegratedSysInfo.gpuclk_ss_percentage = PcdGet16 (PcdCfgPcieRefClkSpreadSpectrum);
  SystemInfoTable->sIntegratedSysInfo.gpuclk_ss_type = 0; // bit0 = down/center spread mode. so far dalwaysown spread mode(=0)
  SystemInfoTable->sIntegratedSysInfo.dpphy_override = PcdGet16 (PcdAmdDpPhyOverride);

  SystemInfoTable->sIntegratedSysInfo.htc_hyst_limit = 0;
  SystemInfoTable->sIntegratedSysInfo.htc_tmp_limit = 0;

  //ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO extdispconninfo;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sHeader.structuresize = sizeof (ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO);
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sHeader.format_revision = 1;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sHeader.content_revision = 1;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.stereopinid = 0;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.ucRemoteDisplayConfig = 0;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.ucFixDPVoltageSwing = (UINT8) PcdGet8 (PcdDisplayFixVoltageSwing);

  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_backlight_pwm_hz = PcdGet16 (PcdBackLightPwmHz);
  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_pwr_on_vary_bl_to_blon = PcdGet8 (PcdPwrOnVaryBlToBlon);
  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_pwr_down_bloff_to_vary_bloff = PcdGet8 (PcdPwrDownBloffToVaryBlOff);

  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_backlight_pwm_hz = PcdGet16 (PcdBackLightPwmHz);
  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_pwr_on_vary_bl_to_blon = PcdGet8 (PcdPwrOnVaryBlToBlon);
  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_pwr_down_bloff_to_vary_bloff = PcdGet8 (PcdPwrDownBloffToVaryBlOff);

  //
  // Get the sPath data - Locate PCIe configuration data to get definitions of display connectors
  //
  GfxIntegratedEnumerateAllConnectors (&SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sPath[0]);

  for (Index = 0; Index < DdiPort; Index++) {
    IDS_HDT_CONSOLE (GNB_TRACE, "sPath[%x] address  0x%x\n",Index ,&SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sPath[Index]);
    GfxDebugDumpDisplayPath (&SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sPath[Index]);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "PopulateSystemInfoTable RMB Exit [0x%x]\n", Status);

}
