/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#include <AMD.h>
#include "SmuV13Dxe.h"
#include <smu13_bios_if_RMB.h>
#include "smu13_RMB.h"
#include "Smu13FeaturesEnable.h"
#include "FanPolicy.h"
#include <Library/AmdBaseLib.h>
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/NbioHandleLib.h>

#define FILECODE        NBIO_SMU_RMB_DXE_PKGTYPEFIXUPS_FILECODE

#define SYSTEM_CONFIG_TDP_10W 1
#define SYSTEM_CONFIG_TDP_15W 2
#define SYSTEM_CONFIG_TDP_25W 3
#define SYSTEM_CONFIG_TDP_28W 4
#define SYSTEM_CONFIG_TDP_30W 5
#define SYSTEM_CONFIG_TDP_35W 6
#define SYSTEM_CONFIG_TDP_45W 7
#define SYSTEM_CONFIG_TDP_54W 8

//Defining Default TDP for the OPNs
#define POWER_LIMIT_15W 15 
#define POWER_LIMIT_45W 45
#define POWER_LIMIT_28W 28
/*----------------------------------------------------------------------------------------------------------*/
/*                                        SMU PACKAGE TYPE CONFIGURATION                                    */
/*----------------------------------------------------------------------------------------------------------*/

extern FAN_POLICY_TABLE  DefaultAM5FanTable[];
extern FAN_POLICY_TABLE  DefaultFP7FanTable[];
/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
SmuFixupPlatformConfigAM5 (
  IN     Smu13BiosIfTable_t        *PPTable
  )
{
  FAN_POLICY_TABLE              *FanPolicyTable;
  GNB_HANDLE                    *GnbHandle;
  PCIe_PLATFORM_CONFIG          *Pcie;
  PCIe_PLATFORM_CONFIG          *PcieFromHob;
  UINT32                        SmuArg[6];

  PcdSet32S (PcdSmuFeatureControlDefines, (UINT32) AM5_SMU_FEATURES_ENABLE_DEFAULT);
  PcdSet32S (PcdSmuFeatureControlDefinesExt, (UINT32) AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT);
  PcdSet32S (PcdSmuFeatureControlDefines64, (UINT32) AM5_SMU_FEATURES_ENABLE_64_DEFAULT);
  IDS_HDT_CONSOLE (MAIN_FLOW, "Smu Feature Default:\n  PcdSmuFeatureControlDefines = 0x%08x\n  PcdSmuFeatureControlDefinesExt = 0x%08x\n  PcdSmuFeatureControlDefines64 = 0x%08x\n",
                    (UINT32) AM5_SMU_FEATURES_ENABLE_DEFAULT,
                    (UINT32) AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT,
                    (UINT32) AM5_SMU_FEATURES_ENABLE_64_DEFAULT);

  PcdSet8S (PcdDcBtc, TRUE);
  PcdSet8S (PcdDcBtcVid, 0x6F);
  PcdSetBoolS (PcdFllBtcEnable, TRUE);

  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;
  GnbHandle = NbioGetHandle (Pcie);

  // read the TDP and TJMAX
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  switch (PcdGet8 (PcdCfgSystemConfiguration)) {
    case 1:
      SmuArg[0] = 0x230000;
      break;
    case 2:
      SmuArg[0] = 0x2D0000;
      break;
    case 3:
      SmuArg[0] = 0x410000;
      break;
    case 0:
    default:
      // read the TDP and TJMAX
      NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_GetSustainedPowerAndThmLimit, SmuArg, 0);
      break;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "SystemConfig=%d, TDP/TJMAX = 0x%x\n", PcdGet8 (PcdCfgSystemConfiguration), SmuArg[0]);

  // Fan Table policy default update.
  FanPolicyTable = (FAN_POLICY_TABLE *) &DefaultAM5FanTable[0];
  if (SmuArg[0] != 0) {
    while (FanPolicyTable->TdpTjMax != 0) {
      if (FanPolicyTable->TdpTjMax == SmuArg[0]) {
        PPTable->FanTable_Override = FanPolicyTable->FanPolicy.FanTable_Override;
        PPTable->FanTable_Hysteresis = FanPolicyTable->FanPolicy.FanTable_Hysteresis;
        PPTable->FanTable_TempLow = FanPolicyTable->FanPolicy.FanTable_TempLow;
        PPTable->FanTable_TempMed = FanPolicyTable->FanPolicy.FanTable_TempMed;
        PPTable->FanTable_TempHigh = FanPolicyTable->FanPolicy.FanTable_TempHigh;
        PPTable->FanTable_TempCritical = FanPolicyTable->FanPolicy.FanTable_TempCritical;
        PPTable->FanTable_PwmLow = FanPolicyTable->FanPolicy.FanTable_PwmLow;
        PPTable->FanTable_PwmMed = FanPolicyTable->FanPolicy.FanTable_PwmMed;
        PPTable->FanTable_PwmHigh = FanPolicyTable->FanPolicy.FanTable_PwmHigh;
        PPTable->FanTable_PwmFreq = FanPolicyTable->FanPolicy.FanTable_PwmFreq;
        PPTable->FanTable_Polarity = FanPolicyTable->FanPolicy.FanTable_Polarity;
        break;
      }
      FanPolicyTable ++;
    }
  }

  if (PcdGet8 (PcdCppcConstraintsEnabled) == 0) {
     PPTable->CppcConstraintsEnabled = 0x1;
     // set CPPC as enable for AM5 for request
     PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | (UINT32) (FEATURE_CPPC_BIT % 32));
     PPTable->PerfLimit_MaxRange = 0xFF;
     PPTable->PerfLimit_MinRange = 0x0;
     PPTable->EPP_MaxRange = 0xFF;
     PPTable->EPP_MinRange = 0x0;
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
SmuFixupPlatformConfigFP7 (
  IN     Smu13BiosIfTable_t        *PPTable
  )
{
  FAN_POLICY_TABLE              *FanPolicyTable;
  GNB_HANDLE                    *GnbHandle;
  PCIe_PLATFORM_CONFIG          *Pcie;
  PCIe_PLATFORM_CONFIG          *PcieFromHob;
  UINT32                        SmuArg[6];
  uint32_t                      PowerLimit = 0;
  UINT8                         SystemConfig = 0;

  PcdSet32S (PcdSmuFeatureControlDefines, (UINT32) FP7_SMU_FEATURES_ENABLE_DEFAULT);
  PcdSet32S (PcdSmuFeatureControlDefinesExt, (UINT32) FP7_SMU_FEATURES_ENABLE_EXT_DEFAULT);
  PcdSet32S (PcdSmuFeatureControlDefines64, (UINT32) FP7_SMU_FEATURES_ENABLE_64_DEFAULT);

  IDS_HDT_CONSOLE (MAIN_FLOW, "Smu Feature Default:\n  PcdSmuFeatureControlDefines = 0x%08x\n  PcdSmuFeatureControlDefinesExt = 0x%08x\n  PcdSmuFeatureControlDefines64 = 0x%08x\n",
                    (UINT32) FP7_SMU_FEATURES_ENABLE_DEFAULT,
                    (UINT32) FP7_SMU_FEATURES_ENABLE_EXT_DEFAULT,
                    (UINT32) FP7_SMU_FEATURES_ENABLE_64_DEFAULT);
  //Disable the STAPM
  PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) & ~((UINT32) (1 << (FEATURE_STAPM_BIT % 32))));
  IDS_HDT_CONSOLE (MAIN_FLOW, "FOX: STAPM Disable\n");

  PcdSet8S (PcdDcBtcVid, 0x97);

  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;
  GnbHandle = NbioGetHandle (Pcie);

  // read the TDP and TJMAX
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  
  // read the TDP and TJMAX
  NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_GetSustainedPowerAndThmLimit, SmuArg, 0);
  SystemConfig = PcdGet8 (PcdCfgSystemConfiguration);
  IDS_HDT_CONSOLE (MAIN_FLOW, "SystemConfig=%d, TDP = 0x%x\n", SystemConfig, SmuArg[0]);
  PowerLimit = ((SmuArg[0] & 0x00FF0000) >> 16);
  IDS_HDT_CONSOLE (MAIN_FLOW, "PowerLimit:%dW\n", PowerLimit);
  switch (PowerLimit) {
    case POWER_LIMIT_15W:
      switch (SystemConfig) {
        case SYSTEM_CONFIG_TDP_10W: //10W
          SmuArg[0] = 0xA0000;
          break;
        case SYSTEM_CONFIG_TDP_15W: //15W
          SmuArg[0] = 0xF0000;
          break;
        case SYSTEM_CONFIG_TDP_25W: //25W
          SmuArg[0] = 0x190000;
          break;
        default:
          SmuArg[0] = 0xF0000;
          PcdSet8S (PcdCfgSystemConfiguration, (UINT8)SYSTEM_CONFIG_TDP_15W);//Set Embedded 15W
          break;
        }
      break;
    case POWER_LIMIT_45W:
      switch (SystemConfig) {
        case SYSTEM_CONFIG_TDP_25W: //25W
          if (CheckOPN() == 1){ // Allow user to set 25W for particular OPN if Power limt is 45W
            SmuArg[0] = 0x190000;
            break;          
          }
        case SYSTEM_CONFIG_TDP_35W: //35W
          SmuArg[0] = 0x230000;
          break;
        case SYSTEM_CONFIG_TDP_45W: //45W
          SmuArg[0] = 0x2D0000;
          break;
        case SYSTEM_CONFIG_TDP_54W: //54W
          SmuArg[0] = 0x360000;
          break;
        default:
          SmuArg[0] = 0x2D0000; 
          PcdSet8S (PcdCfgSystemConfiguration, (UINT8) SYSTEM_CONFIG_TDP_45W);//Set Embedded 45W
          break;
      }
      break;
    case POWER_LIMIT_28W:
      switch (SystemConfig) {
        case SYSTEM_CONFIG_TDP_15W: //15W
          SmuArg[0] = 0xF0000;
          break;
        case SYSTEM_CONFIG_TDP_28W: //28W
          SmuArg[0] = 0x1C0000;
          break;
        case SYSTEM_CONFIG_TDP_30W: //30W
          SmuArg[0] = 0x1E0000;
          break;
        default:
          SmuArg[0] = 0x1E0000;
          PcdSet8S (PcdCfgSystemConfiguration, (UINT8) SYSTEM_CONFIG_TDP_30W);//Set Embedded 30W
          break;
      }
      break;
  }
  PPTable->EmbeddedPlatformEn = 1;
  PowerLimit = ((SmuArg[0] & 0x00FF0000) >> 16);
  IDS_HDT_CONSOLE (MAIN_FLOW, "Get PowerLimit: %dW\n",PowerLimit);
  if (!(PcdGet32 (PcdSmuFeatureControlDefinesExt) & ((UINT32) (1 << (FEATURE_STAPM_BIT % 32)))))
  {
    IDS_HDT_CONSOLE (MAIN_FLOW, "STAPM is disabled \n");
    //
    //Update the SLOW_PPT_LIMIT as same as TDP value & FAST_PPT_LIMIT as 1.2*TDP
    //
    PPTable->SLOW_PPT_LIMIT = PowerLimit * 1000;
    PPTable->FAST_PPT_LIMIT = PowerLimit * 1200;
    IDS_HDT_CONSOLE (MAIN_FLOW, "sPPT = %d, fPPT = %d \n",PPTable->SLOW_PPT_LIMIT,PPTable->FAST_PPT_LIMIT);
  }
  // Fan Table policy default update.
  FanPolicyTable = (FAN_POLICY_TABLE *) &DefaultFP7FanTable[0];
  if (SmuArg[0] != 0) {
    while (FanPolicyTable->TdpTjMax != 0) {
      if (FanPolicyTable->TdpTjMax == SmuArg[0]) {
        PPTable->FanTable_Override = FanPolicyTable->FanPolicy.FanTable_Override;
        PPTable->FanTable_Hysteresis = FanPolicyTable->FanPolicy.FanTable_Hysteresis;
        PPTable->FanTable_TempLow = FanPolicyTable->FanPolicy.FanTable_TempLow;
        PPTable->FanTable_TempMed = FanPolicyTable->FanPolicy.FanTable_TempMed;
        PPTable->FanTable_TempHigh = FanPolicyTable->FanPolicy.FanTable_TempHigh;
        PPTable->FanTable_TempCritical = FanPolicyTable->FanPolicy.FanTable_TempCritical;
        PPTable->FanTable_PwmLow = FanPolicyTable->FanPolicy.FanTable_PwmLow;
        PPTable->FanTable_PwmMed = FanPolicyTable->FanPolicy.FanTable_PwmMed;
        PPTable->FanTable_PwmHigh = FanPolicyTable->FanPolicy.FanTable_PwmHigh;
        PPTable->FanTable_PwmFreq = FanPolicyTable->FanPolicy.FanTable_PwmFreq;
        PPTable->FanTable_Polarity = FanPolicyTable->FanPolicy.FanTable_Polarity;
        break;
      }
      FanPolicyTable ++;
    }
    if (PcdGet8 (PcdP3TLimitCtrl) == 0xF) {
      switch (SmuArg[0]) {
      case 0xA0000:
          PPTable->P3T_LIMIT = 146;
          break;
      case 0xF0000:
        PPTable->P3T_LIMIT = 146;
        break;
      case 0x190000:
          PPTable->P3T_LIMIT = 167;
          break;
        case 0x230000:
          PPTable->P3T_LIMIT = 202;
          break;
        case 0x2D0000:
          PPTable->P3T_LIMIT = 216;
          break;
        case 0x360000:
          PPTable->P3T_LIMIT = 216;
          break;
        default:
          break;
      }
      PcdSet32S (PcdP3TLimit, PPTable->P3T_LIMIT);
    }
  }
  PcdSetBoolS(PcdFllBtcEnable, TRUE);

  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
SmuFixupPlatformConfig (
  IN     UINT32                     PackageType,
  IN     Smu13BiosIfTable_t         *PPTable
  )
{
  SOC_ID_STRUCT       SocId;
  SOC_ID_STRUCT       SocId2;
  SOC_ID_STRUCT       SocId3;
  SOC_ID_STRUCT       SocId4;
  UINT32              Value32Lower;
  UINT32              FMax = 0;
  UINT32              GfxMax = 0;
  UINT32              OpnConfigId;
  UINT32              Data32;
  UINT8               Index;
  UINT32              ConfigId[] = {4259, 4265, 4279, 4278, 4304, 4303,
                                    4351, 4350, 4541, 4540, 4640, 4639,
                                    4642, 4641, 4601, 4602, 4609, 4608,
                                    4610, 4611, 4603, 4604, 4492, 4491, 0};
  UINT32              ConfigIdB8[] = {5023, 5035, 4952, 4947, 5038, 5028, 4946, 4955, 0};
  UINT32              ConfigIdB10B12[] = {5022, 5036, 5015, 5091, 5101, 5014, 5090, 5100,
                                          5026, 5016, 5092, 5102, 5017, 5093, 5103, 5037,
                                          5025, 5146, 4944, 5094, 4945, 5095, 0};
  UINT8               Index2;
  UINT32              PerCoreDldoPsmMarginCtrl;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);
  // Enable default SMU Features based on package type
  IDS_HDT_CONSOLE (MAIN_FLOW, "PackageType = %d\n", PackageType);

  // Check Hardware Identification RMB FP7
  SocId.SocFamilyID = F19_RMB_RAW_ID;
  SocId.PackageType = ZEN3_PKG_FP7;

  SocId2.SocFamilyID = F19_RMB_RAW_ID;
  SocId2.PackageType = ZEN3_PKG_FP7r2;

  SocId3.SocFamilyID = F19_RMB_RAW_ID;
  SocId3.PackageType = ZEN3_PKG_FP7_B0;

  SocId4.SocFamilyID = F19_RMB_RAW_ID;
  SocId4.PackageType = ZEN3_PKG_FP7r2_B0;

  if (SocHardwareIdentificationCheck (&SocId) ||
      SocHardwareIdentificationCheck (&SocId2) ||
      SocHardwareIdentificationCheck (&SocId3) ||
      SocHardwareIdentificationCheck (&SocId4)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "RMB FP7\n");
    SmuFixupPlatformConfigFP7 (PPTable);
    if (SocHardwareIdentificationCheck (&SocId) ||
        SocHardwareIdentificationCheck (&SocId3)) {
      PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) | (UINT32) (1 << (FEATURE_DS_FCLK_BIT)));
      PPTable->VddmCldoOvDetEn = 1;
    }
    PPTable->VddmCldoFastStartupEn = 1;
    if (PcdGetBool(PcdUseFusedFmax) != TRUE) {
      if (SocHardwareIdentificationCheck (&SocId) ||
          SocHardwareIdentificationCheck (&SocId2)) {
        SmnRegisterRead (0, 0x5d744, &Data32);
        Data32 &= 0xFF0;
        Data32 = (Data32 >> 4) * 25;
        IDS_HDT_CONSOLE (MAIN_FLOW, "FMAX = %d\n", Data32);
        if (Data32 >= 4000) {
          PPTable->CclkFmaxOverride = 4000;
        }
        SmnRegisterRead (0, 0x5d9b0, &Data32);
        OpnConfigId = Data32 >> 27;
        SmnRegisterRead (0, 0x5d9b4, &Data32);
        Data32 &= 0x7FF;
        OpnConfigId |= Data32 << 5;
        Index = 0;
        IDS_HDT_CONSOLE (MAIN_FLOW, "OpnConfigId = %d\n", OpnConfigId);
        if (OpnConfigId != 0) {
          while (ConfigId[Index] != 0) {
            if (ConfigId[Index] == OpnConfigId) {
              PPTable->CclkFmaxOverride = 4000;
              break;
            }
            Index ++;
          }
        }
      }
    }
  }

  if (SocHardwareIdentificationCheck (&SocId2) ||
      SocHardwareIdentificationCheck (&SocId4)) {
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | ((UINT32) (1 << (FEATURE_DF_LIGHT_CSTATE % 32))));
    PPTable->DfCstateConfigOverride = 1;
    PPTable->DfCstateSelfRefrEn = 1;
    PPTable->DfCstateMmStutterOptEn = 1;
    PPTable->DfCstatePwrGateEn = 1;
    PPTable->DfCstateClkPwrDnEn = 1;
  }

  if (SocHardwareIdentificationCheck (&SocId3) ||
      SocHardwareIdentificationCheck (&SocId4)) {
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) & ~((UINT32) (1 << (FEATURE_RSMU_LOW_POWER_BIT % 32))));

    PerCoreDldoPsmMarginCtrl = PcdGet32 (PerCoreDldoPsmMarginCtrl) & 0xFFFF;
    if (PerCoreDldoPsmMarginCtrl) {
      Data32 = 0;
      SmnRegisterRead (0, 0x5d9e8, &Data32);
      OpnConfigId = Data32 >> 27;
      SmnRegisterRead (0, 0x5d9ec, &Data32);
      Data32 &= 0x7FF;
      OpnConfigId |= Data32 << 5;
      Index = 0;
      Index2 = 0;
      IDS_HDT_CONSOLE (MAIN_FLOW, "OpnConfigId = %d\n", OpnConfigId);
      if (OpnConfigId != 0) {
        while (ConfigIdB8[Index] != 0) {
          if (ConfigIdB8[Index] == OpnConfigId) {
            while (Index2 < 8) {
              PPTable->PerCoreDldoPsmMargin[Index2] = 3;
              Index2++;
            }
            break;
          }
          Index++;
        }
        if (ConfigIdB8[Index] == 0) {
          Index = 0;
          Index2 = 0;
          while (ConfigIdB10B12[Index] != 0) {
            if (ConfigIdB10B12[Index] == OpnConfigId) {
              while (Index2 < 8) {
                PPTable->PerCoreDldoPsmMargin[Index2] = 10;
                Index2++;
              }
              break;
            }
            Index++;
          }
        }
      }
      PerCoreDldoPsmMarginCtrl = (PcdGet32 (PerCoreDldoPsmMarginCtrl) & 0xFFFF0000) >> 16;
      Index2 = 0;
      while (Index2 < 16) {
        if (PerCoreDldoPsmMarginCtrl & (3 << Index2)) {
          PPTable->PerCoreDldoPsmMargin[Index2 / 2] += 5;
        }
        Index2 += 2;
      }
    }
  }

  if (SocHardwareIdentificationCheck (&SocId) ||
      SocHardwareIdentificationCheck (&SocId3)) {
    PcdSet32S (PcdSmuFeatureControlDefinesExt,
    PcdGet32 (PcdSmuFeatureControlDefinesExt) | ((UINT32) (1 << (FEATURE_DS_UMCCLK_BIT % 32))));
  }

  // Check Hardware Identification RMB AM5
  SocId.SocFamilyID = F19_RMB_RAW_ID;
  SocId.PackageType = ZEN3_PKG_AM5;

  SocId2.SocFamilyID = F19_RMB_RAW_ID;
  SocId2.PackageType = 3; // ZEN3_PKG_AM5_B0
  if (SocHardwareIdentificationCheck (&SocId) ||
      SocHardwareIdentificationCheck (&SocId2)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "RMB AM5\n");
    SmuFixupPlatformConfigAM5 (PPTable);
  }

  // For Cc6 Control En/Dis
  if (PcdGet8 (PcdAmdCc6Ctrl) == 0) {
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) & ~((UINT32) (1 << (FEATURE_CC6_BIT % 32))));
  } else {
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | (UINT32) (1 << (FEATURE_CC6_BIT % 32)));
  }

  if (PcdGet8 (PcdCfgPeApmEnable) == 1) {
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | (UINT32) (1 << (FEATURE_SMART_SHIFT_BIT % 32)));
  } else {
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) & ~((UINT32) (1 << (FEATURE_SMART_SHIFT_BIT % 32))));
  }

  // IDS_HDT_CONSOLE (MAIN_FLOW, "PcdAmdFabricCstate %x \n", PcdGet8 (PcdAmdFabricCstate));
  // if(PcdGet8 (PcdAmdFabricCstate) == 0x1) {
  //   PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | (UINT32) (1 << (FEATURE_DF_LIGHT_CSTATE % 32)));
  // } else {
  //   PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) & ~((UINT32) (1 << (FEATURE_DF_LIGHT_CSTATE % 32))));
  // }

  //if (PcdGet8 (PcdS0i3Enable) == 0x1) {
  //  PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | (UINT32) (1 << (FEATURE_S0I3_BIT % 32)));
  //} else {
  //  PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) & ~((UINT32) (1 << (FEATURE_S0I3_BIT % 32))));
  //}

  // if (PcdGet8 (PcdCfgIgpuContorl) == 0) {
  //   PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) & ~((UINT32) (1 << (FEATURE_GFX_DPM_BIT))));
  // } else {
  //   PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) | (UINT32) (1 << (FEATURE_GFX_DPM_BIT)));
  // }

  if (PcdGetBool (PcdPublicFanControlEnable) == FALSE) {
    PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) & ~((UINT32) (1 << (FEATURE_FAN_CONTROLLER_BIT % 32))));
  } else {
    PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) | (UINT32) (1 << (FEATURE_FAN_CONTROLLER_BIT % 32)));
  }

//  if (PcdGetBool (PcdAmdSmuZStateControlEnable) == FALSE) {
//    PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) & ~((UINT32) (1 << (FEATURE_ZSTATES_BIT % 32))));
//  } else {
//    PcdSet32S (PcdSmuFeatureControlDefines, PcdGet32 (PcdSmuFeatureControlDefines) | (UINT32) (1 << (FEATURE_ZSTATES_BIT % 32)));
//  }

  if (PcdGet8 (PcdCpuBoostClockOverride) == 0x01) {
    if (PcdGet32 (PcdFMaxFrequency) != 0) {
      Value32Lower = 0;
      SmnRegisterRead (0, 0x5D744, &Value32Lower);
      if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
        FMax = (Value32Lower >> 4) & 0xFF;
        if ((PcdGet32 (PcdFMaxFrequency) <= 200) ||
            ((UINT32)(0 - PcdGet32 (PcdFMaxFrequency)) <= 1000)) {
          FMax = FMax * 25;
          FMax += PcdGet32 (PcdFMaxFrequency);
          PPTable->CclkFmaxOverride = (UINT16)FMax;
          IDS_HDT_CONSOLE (MAIN_FLOW, "CPU Fmax= %d\n", FMax);
        }
      }
    }
  }

  if (PcdGet8 (PcdGpuBoostClockOverride) == 0x01) {
    if (PcdGet32 (PcdGFXFMaxFrequency) != 0) {
      Value32Lower = 0;
      SmnRegisterRead (0, 0x5D748, &Value32Lower);
      if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
        GfxMax = (Value32Lower >> 5) & 0xFF;
        if (PcdGet32 (PcdGFXFMaxFrequency) <= 200) {
          GfxMax = GfxMax * 25;
          GfxMax += PcdGet32 (PcdGFXFMaxFrequency);
          PPTable->GfxclkFmaxOverride = (UINT16)GfxMax;
          IDS_HDT_CONSOLE (MAIN_FLOW, "GPU Fmax= %d\n", GfxMax);
        }
      }
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a End\n", __FUNCTION__);
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
SmuPackageTypeWorkarounds (
  IN     UINT32                     PackageType,
  IN     GNB_HANDLE                 *GnbHandle
  )
{
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);
  // Enable default SMU Features based on package type
  IDS_HDT_CONSOLE (MAIN_FLOW, "PackageType = %d\n", PackageType);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a End\n", __FUNCTION__);
}

