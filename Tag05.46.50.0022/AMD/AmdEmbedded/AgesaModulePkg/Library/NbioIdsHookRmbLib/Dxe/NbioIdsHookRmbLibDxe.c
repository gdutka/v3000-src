/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/PcdLib.h>
#include <IdsHookId.h>
#include <IdsNvIdRMB.h>
#include <IdsNvDefRMB.h>

#include "smu13_bios_if_RMB.h"

#define FILECODE LIBRARY_NBIOIDSHOOKRMBLIB_DXE_NBIOIDSHOOKRMBLIBDXE_FILECODE

/**
 *
 * IDS HOOK for NBIO After CCX
 *
 *
 *
 * @param[in] HookId          IDS HOOK ID
 * @param[in] Handle          Handle
 * @param[in] Data            Data
 * @retval    AGESA_STATUS
 *
 **/
STATIC
IDS_HOOK_STATUS
CmnHookFuncNbioAfterCcx (
  HOOK_ID HookId,
  VOID *Handle,
  VOID *Data
  )
{
  return IDS_HOOK_SUCCESS;
}
/**
 *
 * IDS HOOK for NBIO PCIe AER initialization
 *
 *
 *
 * @param[in] HookId          IDS HOOK ID
 * @param[in] Handle          Handle
 * @param[in] Data            Data
 * @retval    AGESA_STATUS
 *
 **/
STATIC
IDS_HOOK_STATUS
CmnHookFuncNbioPcieAerInit (
  HOOK_ID HookId,
  VOID *Handle,
  VOID *Data
  )
{
  return IDS_HOOK_SUCCESS;
}

/**
 *
 * IDS HOOK for SMU initialization
 *
 *
 *
 * @param[in] HookId          IDS HOOK ID
 * @param[in] Handle          Handle
 * @param[in] Data            Data
 * @retval    AGESA_STATUS
 *
 **/
STATIC
IDS_HOOK_STATUS
CmnHookFuncNbioSMUInit (
  HOOK_ID   HookId,
  VOID      *Handle,
  VOID      *Data
  )
{

  UINT64     IdsNvValue;
  UINT32     PcdSmuFeatureControlDefines;
  Smu13BiosIfTable_t  *PPTable;

  PPTable = (Smu13BiosIfTable_t*)Data;

  ASSERT (Handle != NULL);

  // First UINT32 of Common/External SMU Feature Flags
  //PcdSmuFeatureControlDefines = PcdGet32 (PcdSmuFeatureControlDefines);
  //
  // Leaving this commented out until it is needed
  //
  // Save new value
  //PcdSet32S (PcdSmuFeatureControlDefines, PcdSmuFeatureControlDefines);

  // Second UINT32 of Common/External SMU Feature Flags
  PcdSmuFeatureControlDefines = PcdGet32 (PcdSmuFeatureControlDefinesExt);





  // Save new value
  PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdSmuFeatureControlDefines);

  IDS_HDT_CONSOLE (GNB_TRACE, "PcdSmuFeatureControlDefines(0x%08x)\n", PcdSmuFeatureControlDefines);

  //PPT Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_PPT_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_PPT_CTL_MANUAL:
      IDS_NV_READ_SKIP (IDSNVID_DBG_FAST_PPT_LIMIT, &IdsNvValue) {
        PPTable->FAST_PPT_LIMIT = (UINT32) IdsNvValue;
      }
      IDS_NV_READ_SKIP (IDSNVID_DBG_SLOW_PPT_LIMIT, &IdsNvValue) {
        PPTable->SLOW_PPT_LIMIT = (UINT32) IdsNvValue;
      }
      IDS_NV_READ_SKIP (IDSNVID_DBG_SLOW_PPT_TIME_CONSTANT, &IdsNvValue) {
        PPTable->SLOW_PPT_TIME_CONSTANT = (UINT32) IdsNvValue;
      }
      break;
    case IDSOPT_DBG_PPT_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //STAPM Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_STAPM_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_STAPM_CTL_AUTO:
      break;
    case IDSOPT_DBG_STAPM_CTL_MANUAL:

      //STAPM Boost Override
      IDS_NV_READ_SKIP (IDSNVID_DBG_STAPM_BOOST_OVERRIDE, &IdsNvValue) {
        switch (IdsNvValue) {
        case IDSOPT_DBG_STAPM_BOOST_OVERRIDE_AUTO:
          break;
        case IDSOPT_DBG_STAPM_BOOST_OVERRIDE_0:
          PPTable->STAPM_BOOST_OVERRIDE = 0;
          break;
        case IDSOPT_DBG_STAPM_BOOST_OVERRIDE_1:
          PPTable->STAPM_BOOST_OVERRIDE = 1;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      //STAPM Boost
      IDS_NV_READ_SKIP (IDSNVID_DBG_STAPM_BOOST, &IdsNvValue) {
        switch (IdsNvValue) {
        case IDSOPT_DBG_STAPM_BOOST_AUTO:
          break;
        case IDSOPT_DBG_STAPM_BOOST_1:
          PPTable->STAPM_BOOST = 1;
          IDS_NV_READ_SKIP (IDSNVID_DBG_TSKIN_TIME_CONSTANT, &IdsNvValue) {
            PcdSet32S (PcdStapmTimeConstant, (UINT32)IdsNvValue);
          }
          break;
        case IDSOPT_DBG_STAPM_BOOST_0:
          PPTable->STAPM_BOOST = 0;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //Thermal Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_THM_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_THM_CTL_MANUAL:
      IDS_NV_READ_SKIP (IDSNVID_DBG_TJ_MAX_LIMIT, &IdsNvValue) {
      PPTable->THERMAL_LIMIT = (UINT32) IdsNvValue;
      }
      break;
    case IDSOPT_DBG_THM_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //TDC Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_TDC_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_TDC_CTL_MANUAL:
      IDS_NV_READ_SKIP (IDSNVID_DBG_TDCVDD_LIMIT, &IdsNvValue) {
        PPTable->VRM_VDD_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      IDS_NV_READ_SKIP (IDSNVID_DBG_TDCSOC_LIMIT, &IdsNvValue) {
        PPTable->VRM_SOC_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      IDS_NV_READ_SKIP (IDSNVID_DBG_TDCGFX_LIMIT, &IdsNvValue) {
        PPTable->VRM_SR_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      break;
    case IDSOPT_DBG_TDC_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //EDC Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_EDC_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_EDC_CTL_MANUAL:
      IDS_NV_READ_SKIP (IDSNVID_DBG_EDCVDD_LIMIT, &IdsNvValue) {
        PPTable->VRM_VDD_MAXIMUM_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      IDS_NV_READ_SKIP (IDSNVID_DBG_EDCSOC_LIMIT, &IdsNvValue) {
        PPTable->VRM_SOC_MAXIMUM_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      IDS_NV_READ_SKIP (IDSNVID_DBG_EDCGFX_LIMIT, &IdsNvValue) {
        PPTable->VRM_SR_MAXIMUM_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      break;
    case IDSOPT_DBG_EDC_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //PSI3 Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_PS_I3_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_PS_I3_CTL_MANUAL:
      IDS_NV_READ_SKIP (IDSNVID_DBG_PS_I3_VDD_LIMIT, &IdsNvValue) {
        PPTable->PSI3_VDD_CURRENT_LIMIT = (UINT32) IdsNvValue;
      }

      // IDS_NV_READ_SKIP (IDSNVID_DBG_PS_I3_GFX_LIMIT, &IdsNvValue) {
      //   PPTable->PSI3_GFX_CURRENT_LIMIT = (UINT32) IdsNvValue;
      // }

      break;
    case IDSOPT_DBG_PS_I3_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //PROCHOT Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_PROCHOT_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_PROCHOT_CTL_MANUAL:
      IDS_NV_READ_SKIP (IDSNVID_DBG_PROCHOT_RAMP_TIME, &IdsNvValue) {
        PPTable->PROCHOT_L_DEASSERTION_RAMP_TIME = (UINT32) IdsNvValue;
      }
      break;
    case IDSOPT_DBG_PROCHOT_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  // STT control from C code with PCD

  //Fan Control
  IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_CTL, &IdsNvValue) {
    switch (IdsNvValue) {
    case IDSOPT_DBG_FAN_CTL_MANUAL:

      //Fan Table Control
      IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_CTL, &IdsNvValue) {
        switch (IdsNvValue) {
        case IDSOPT_DBG_FAN_TABLE_CTL_MANUAL:

          IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_CTL, &IdsNvValue) {
            PcdSet8S (PcdFanTableOverride, (UINT8)IdsNvValue);
            PPTable->FanTable_Override = PcdGet8 (PcdFanTableOverride);
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_LOW, &IdsNvValue) {
              PcdSet8S (PcdFanTableTempLow, (UINT8)IdsNvValue);
              PPTable->FanTable_TempLow = PcdGet8 (PcdFanTableTempLow);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_MED, &IdsNvValue) {
              PcdSet8S (PcdFanTableTempMed, (UINT8)IdsNvValue);
              PPTable->FanTable_TempMed = PcdGet8 (PcdFanTableTempMed);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_HIGH, &IdsNvValue) {
              PcdSet8S (PcdFanTableTempHigh, (UINT8)IdsNvValue);
              PPTable->FanTable_TempHigh = PcdGet8 (PcdFanTableTempHigh);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_CRITICAL, &IdsNvValue) {
              PcdSet8S (PcdFanTableTempCritical, (UINT8)IdsNvValue);
              PPTable->FanTable_TempCritical = PcdGet8 (PcdFanTableTempCritical);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_T_PWM_LOW, &IdsNvValue) {
              PcdSet8S (PcdFanTablePwmLow, (UINT8)IdsNvValue);
              PPTable->FanTable_PwmLow = PcdGet8 (PcdFanTablePwmLow);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_PWM_MED, &IdsNvValue) {
              PcdSet8S (PcdFanTablePwmMed, (UINT8)IdsNvValue);
              PPTable->FanTable_PwmMed = PcdGet8 (PcdFanTablePwmMed);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_PWM_HIGH, &IdsNvValue) {
              PcdSet8S (PcdFanTablePwmHigh, (UINT8)IdsNvValue);
              PPTable->FanTable_PwmHigh = PcdGet8 (PcdFanTablePwmHigh);
            }
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_HYST, &IdsNvValue) {
              PcdSet8S (PcdFanTableHysteresis, (UINT8)IdsNvValue);
              PPTable->FanTable_Hysteresis = PcdGet8 (PcdFanTableHysteresis);
            }
            //Fan Polarity
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_POLARITY, &IdsNvValue) {
              PcdSet8S (PcdFanTablePolarity, (UINT8)IdsNvValue);
              PPTable->FanTable_Polarity = PcdGet8 (PcdFanTablePolarity);
            }
            //Pwm Frequency
            IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_PWM_FREQ, &IdsNvValue) {
              PcdSet8S (PcdFanTablePwmFreq, (UINT8)IdsNvValue);
              PPTable->FanTable_PwmFreq = PcdGet8 (PcdFanTablePwmFreq);
            }
          }
          break;
        case IDSOPT_DBG_FAN_TABLE_CTL_AUTO:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      //Force PWM Control
      IDS_NV_READ_SKIP (IDSNVID_DBG_FORCE_PWM_CTL, &IdsNvValue) {
        switch (IdsNvValue) {
        case IDSOPT_DBG_FORCE_PWM_CTL_FORCE:
          PcdSet8S (PcdForceFanPwmEn, (UINT8)IdsNvValue);
          PPTable->ForceFanPwmEn = PcdGet8 (PcdForceFanPwmEn);
          IDS_NV_READ_SKIP (IDSNVID_DBG_FORCE_PWM, &IdsNvValue) {
            PcdSet8S (PcdForceFanPwm, (UINT8)IdsNvValue);
            PPTable->ForceFanPwm = PcdGet8 (PcdForceFanPwm);
          }
          break;
        case IDSOPT_DBG_FORCE_PWM_CTL_UNFORCE:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      break;
    case IDSOPT_DBG_FAN_CTL_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

//VDDP Voltage - APCB
//VDDM Voltage - APCB

  return IDS_HOOK_SUCCESS;
}

#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define NBIO_RMB_IDS_HOOKS_DXE_INT
#else
  #include <Internal/NbioIdsHookRmbLibDxeInt.h>
#endif

STATIC IDS_HOOK_ELEMENT NbioIdsHookFuncs[] = {
  {
    IDS_HOOK_NBIO_AFTER_CCX,
    &CmnHookFuncNbioAfterCcx
  },
  {
    IDS_HOOK_NBIO_PCIE_AER_INIT,
    &CmnHookFuncNbioPcieAerInit
  },
  {
    IDS_HOOK_NBIO_SMU_INIT,
    &CmnHookFuncNbioSMUInit
  },
  NBIO_RMB_IDS_HOOKS_DXE_INT
  IDS_HOOKS_END
};

STATIC IDS_HOOK_TABLE NbioIdsHookTable = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  NbioIdsHookFuncs
};


AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &NbioIdsHookTable;
  return AGESA_SUCCESS;
}


