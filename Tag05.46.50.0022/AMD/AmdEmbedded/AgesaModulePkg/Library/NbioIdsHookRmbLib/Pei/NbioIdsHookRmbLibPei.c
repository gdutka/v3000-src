/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "PiPei.h"
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <IdsHookId.h>

#define FILECODE LIBRARY_NBIOIDSHOOKRMBLIB_PEI_NBIOIDSHOOKRMBLIBPEI_FILECODE

/**
 *
 * IDS HOOK for NBIO Base initialization
 *
 *
 *
 * @param[in] HookId          IDS HOOK ID
 * @param[in] Handle          Handle
 * @param[in] Data            Data
 * @retval    AGESA_STATUS
 *
 **/
IDS_HOOK_STATUS
CmnHookFuncNbioBaseInit (
  HOOK_ID HookId,
  VOID *Handle,
  VOID *Data
  )
{
//  UINT64     IdsNvValue;

  //System Configuration
//  IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_SMU_SYSTEM_CONFIG, &IdsNvValue) {
//    if (IdsNvValue != IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_AUTO) {
//      PcdSet8S (PcdCfgSystemConfiguration, (UINT8)IdsNvValue);
//    }
//  }

//  //GFX Clock Frequency
//  IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFX_CLOCK_FREQUENCY, &IdsNvValue) {
//    if (IdsNvValue != 0) {
//      PcdSet16S (PcdMocGfxClockFrequency, (UINT16)IdsNvValue);
//    }
//  }

//  //GFX Core voltage
//  IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFX_CORE_VID, &IdsNvValue) {
//    if (IdsNvValue != 0) {
//      PcdSet8S (PcdMocGfxCoreVid, (UINT8)IdsNvValue);
//    }
//  }

  //Integrated Graphics Controller
//  IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFXI_GPU_CONTROL, &IdsNvValue) {
//    if (IdsNvValue == 0) {
//      PcdSet8S (PcdCfgIgpuContorl, (UINT8)IdsNvValue);
//    }
//  }

  return IDS_HOOK_SUCCESS;
}

/**
 *
 * IDS HOOK for NBIO PCIe Topology initialization
 *
 *
 *
 * @param[in] HookId          IDS HOOK ID
 * @param[in] Handle          Handle
 * @param[in] Data            Data
 * @retval    AGESA_STATUS
 *
 **/
IDS_HOOK_STATUS
CmnHookFuncNbioPcieTopology (
  HOOK_ID HookId,
  VOID *Handle,
  VOID *Data
  )
{
  return IDS_HOOK_SUCCESS;
}
/**
 *
 * IDS HOOK for NBIO PCIe User Config initialization
 *
 *
 *
 * @param[in] HookId          IDS HOOK ID
 * @param[in] Handle          Handle
 * @param[in] Data            Data
 * @retval    AGESA_STATUS
 *
 **/
IDS_HOOK_STATUS
CmnHookFuncNbioPcieUserConfig (
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
IDS_HOOK_STATUS
CmnHookFuncNbioSMUInit (
  HOOK_ID   HookId,
  VOID      *Handle,
  VOID      *Data
  )
{
//  UINT64     IdsNvValue;

  // XFR Enhancement
//  IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_XF_R20_CTL, &IdsNvValue) {
//    if (IdsNvValue != 0xF) {
//      if (IdsNvValue == 0) {
//        PcdSet32S (PcdMocPPTLimit, 0);
//        PcdSet32S (PcdMocTDCLimit, 0);
//        PcdSet32S (PcdMocEDCLimit, 0);
//        PcdSet32S (PcdCfgPPT, 0);
//        PcdSet32S (PcdCfgTDC, 0);
//        PcdSet32S (PcdCfgEDC, 0);
//        PcdSet32S (PcdMocPBOLimitScalar, 100);
//      } else if (IdsNvValue == 1) {
//        PcdSet32S (PcdMocPPTLimit, PcdGet32 (PcdCfgPPT));
//        PcdSet32S (PcdMocTDCLimit, PcdGet32 (PcdCfgTDC));
//        PcdSet32S (PcdMocEDCLimit, PcdGet32 (PcdCfgEDC));
//        PcdSet32S (PcdMocPBOLimitScalar, 700);
//      } else if (IdsNvValue == 2) {
//        IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_PPT_LIMIT, &IdsNvValue) {
//          PcdSet32S (PcdMocPPTLimit, (UINT32)IdsNvValue);
//        }
//        IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_TDC_LIMIT, &IdsNvValue) {
//          PcdSet32S (PcdMocTDCLimit, (UINT32)IdsNvValue);
//        }
//        IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_EDC_LIMIT, &IdsNvValue) {
//          PcdSet32S (PcdMocEDCLimit, (UINT32)IdsNvValue);
//        }
//      }
//    }
//  }
  //Precision Boost Overdrive Scalar
//  IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_SMUFIT_LIMIT_CTL, &IdsNvValue) {
//    if (IdsNvValue == 1) {
//      //customized Precision Boost Overdrive Scalar
//      IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_SMUFIT_LIMIT_SCALAR, &IdsNvValue) {
//        PcdSet32S (PcdMocPBOLimitScalar, (UINT32)IdsNvValue);
//      }
//    }
//  }

  return IDS_HOOK_SUCCESS;
}

#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define NBIO_RMB_IDS_HOOKS_PEI_INT
#else
  #include <Internal/NbioIdsHookRmbLibPeiInt.h>
#endif

IDS_HOOK_ELEMENT NbioRmbIdsHookFuncs[] = {
  {
    IDS_HOOK_NBIO_BASE_INIT,
    &CmnHookFuncNbioBaseInit
  },
  {
    IDS_HOOK_NBIO_PCIE_TOPOLOGY,
    &CmnHookFuncNbioPcieTopology
  },
  {
    IDS_HOOK_NBIO_PCIE_USER_CONFIG,
    &CmnHookFuncNbioPcieUserConfig
  },
  {
    IDS_HOOK_NBIO_SMU_INIT,
    &CmnHookFuncNbioSMUInit
  },
  NBIO_RMB_IDS_HOOKS_PEI_INT
  IDS_HOOKS_END
};

IDS_HOOK_TABLE NbioRmbIdsHookTable = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  NbioRmbIdsHookFuncs
};


AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &NbioRmbIdsHookTable;
  return AGESA_SUCCESS;
}


