/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <IdsHookId.h>
#include <IdsNvIdRMB.h>
#include <IdsNvDefRMB.h>
#include <Filecode.h>
#include <FchPlatform.h>

#define FILECODE LIBRARY_FCHIDSHOOKRMBLIB_PEI_FCHIDSHOOKRMBLIBPEI_FILECODE

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
IDS_HOOK_STATUS
CmnHookFuncFchRmbPeiInitReset (
  HOOK_ID   HookId,
  VOID     *Handle,
  VOID     *Data
  )
{
  UINT64                          IdsNvValue;
  UINT32                          XhciUsb2PortDisable;
  UINT32                          XhciUsb3PortDisable;

  IDS_HDT_CONSOLE (FCH_TRACE, "CmnHookFuncFchRmbPeiInitReset FCH Reset Options Update\n");

  // USB
  XhciUsb2PortDisable = PcdGet32 (PcdXhciUsb2PortDisable);

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT0, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_DISABLED) {
      XhciUsb2PortDisable |= BIT0;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT0;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT1, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_DISABLED) {
      XhciUsb2PortDisable |= BIT1;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT1;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT2, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_DISABLED) {
      XhciUsb2PortDisable |= BIT2;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT2;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT3, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_DISABLED) {
      XhciUsb2PortDisable |= BIT3;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT3;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_HS_PORT0, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_DISABLED) {
      XhciUsb2PortDisable |= BIT4;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT4;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_HS_PORT1, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_DISABLED) {
      XhciUsb2PortDisable |= BIT5;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT5;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_HS_PORT2, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_DISABLED) {
      XhciUsb2PortDisable |= BIT6;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT6;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I2_HS_PORT0, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_DISABLED) {
      XhciUsb2PortDisable |= BIT8;
    } else {
      XhciUsb2PortDisable &= ~(UINT32)BIT8;
    }
  }

  if (FchCheckRmbB0()) {
    IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I3_HS_PORT0, &IdsNvValue)
    if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_AUTO) { // Auto
      if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_DISABLED) {
        XhciUsb2PortDisable |= BIT12;
      } else {
        XhciUsb2PortDisable &= ~(UINT32)BIT12;
      }
    }

    IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I4_HS_PORT0, &IdsNvValue)
    if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_AUTO) { // Auto
      if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_DISABLED) {
        XhciUsb2PortDisable |= BIT16;
      } else {
        XhciUsb2PortDisable &= ~(UINT32)BIT16;
      }
    }
  }

  FCH_PCDSET32 (PcdXhciUsb2PortDisable, XhciUsb2PortDisable);

  XhciUsb3PortDisable = PcdGet32 (PcdXhciUsb3PortDisable);

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_SS_PORT0, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_DISABLED) {
      XhciUsb3PortDisable |= BIT0;
    } else {
      XhciUsb3PortDisable &= ~(UINT32)BIT0;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_SS_PORT1, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_DISABLED) {
      XhciUsb3PortDisable |= BIT1;
    } else {
      XhciUsb3PortDisable &= ~(UINT32)BIT1;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_SS_PORT0, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_DISABLED) {
      XhciUsb3PortDisable |= BIT2;
    } else {
      XhciUsb3PortDisable &= ~(UINT32)BIT2;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_SS_PORT1, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_DISABLED) {
      XhciUsb3PortDisable |= BIT3;
    } else {
      XhciUsb3PortDisable &= ~(UINT32)BIT3;
    }
  }

  if (FchCheckRmbB0()) {
    IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I3_SS_PORT0, &IdsNvValue)
    if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_AUTO) { // Auto
      if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_DISABLED) {
        XhciUsb3PortDisable |= BIT6;
      } else {
        XhciUsb3PortDisable &= ~(UINT32)BIT6;
      }
    }

    IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I4_SS_PORT0, &IdsNvValue)
    if (IdsNvValue != IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_AUTO) { // Auto
      if (IdsNvValue == IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_DISABLED) {
        XhciUsb3PortDisable |= BIT8;
      } else {
        XhciUsb3PortDisable &= ~(UINT32)BIT8;
      }
    }
  }

  FCH_PCDSET32 (PcdXhciUsb3PortDisable, XhciUsb3PortDisable);

  return IDS_HOOK_SUCCESS;
}

#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define FCH_RMB_IDS_HOOKS_INT_PEI
#else
  #include "Internal/FchIdsHookRmbLibIntPei.h"
#endif

IDS_HOOK_ELEMENT FchRmbIdsHooksPei[] = {
  {
    IDS_HOOK_FCH_INIT_RESET,
    &CmnHookFuncFchRmbPeiInitReset
  },
  FCH_RMB_IDS_HOOKS_INT_PEI
  IDS_HOOKS_END
};

IDS_HOOK_TABLE FchRmbIdsHookTablePei = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  FchRmbIdsHooksPei
};


AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &FchRmbIdsHookTablePei;
  return AGESA_SUCCESS;
}
