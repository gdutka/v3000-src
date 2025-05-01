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

#define FILECODE LIBRARY_FCHIDSHOOKRMBLIB_DXE_FCHIDSHOOKRMBLIBDXE_FILECODE

IDS_HOOK_STATUS
CmnHookFuncFchRmbDxeInitEnv (
  HOOK_ID   HookId,
  VOID     *Handle,
  VOID     *Data
  )
{
  UINT32                           PcdData32;
  UINT64                           IdsNvValue;
  FCH_DATA_BLOCK                   *FchParams;
  UINT8                            I3CMode8;
  UINTN                            Size;
  UINTN                            *PtrBuffer;

  IDS_HDT_CONSOLE (FCH_TRACE, "CmnHookFuncFchRmbDxeInitEnv Options Update\n");

  Size = 8;
  FchParams = (FCH_DATA_BLOCK *)Data;

  // I2C, I3C, Uart, ESPI
  PcdData32 = PcdGet32 (FchRTDeviceEnableMap);
  I3CMode8 = PcdGet8 (FchI3CMode);
  FchParams->FchRunTime.HidControl[0]        = *((HID_CONTROL *) PcdGetPtr (PcdFchHidControl));
  FchParams->FchRunTime.HidControl[1]        = *((HID_CONTROL *) PcdGetPtr (PcdFchHidControl) + 1 );
  PtrBuffer = (UINTN *)(&FchParams->FchRunTime.HidControl[0].SpiReadMode);

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C0_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_I3_C0_CONFIG_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_I3_C0_CONFIG_I3CENABLED) {
      PcdData32 |= BIT21;
      PcdData32 &= ~ BIT5;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C0_MODE, &IdsNvValue)
      if (IdsNvValue != IDSOPT_CMN_FCH_I3_C0_MODE_AUTO) { // Auto
        if (IdsNvValue) {
          I3CMode8 |= BIT0;
        } else {
          I3CMode8 &= ~ BIT0;
        }
      }
    } else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C0_CONFIG_BOTHDISABLED){
      PcdData32 &= ~ BIT21;
      PcdData32 &= ~ BIT5;
    }else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C0_CONFIG_I2CENABLED) {
      PcdData32 &= ~ BIT21;
      PcdData32 |= BIT5;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C1_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_I3_C1_CONFIG_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_I3_C1_CONFIG_I3CENABLED) {
      PcdData32 |= BIT13;
      PcdData32 &= ~ BIT6;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C1_MODE, &IdsNvValue)
      if (IdsNvValue != IDSOPT_CMN_FCH_I3_C1_MODE_AUTO) { // Auto
        if (IdsNvValue) {
          I3CMode8 |= BIT1;
        } else {
          I3CMode8 &= ~ BIT1;
        }
      }
    } else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C1_CONFIG_BOTHDISABLED){
      PcdData32 &= ~ BIT13;
      PcdData32 &= ~ BIT6;
    }else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C1_CONFIG_I2CENABLED) {
      PcdData32 &= ~ BIT13;
      PcdData32 |= BIT6;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C2_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_I3_C2_CONFIG_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_I3_C2_CONFIG_I3CENABLED) {
      PcdData32 |= BIT14;
      PcdData32 &= ~ BIT7;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C2_MODE, &IdsNvValue)
      if (IdsNvValue != IDSOPT_CMN_FCH_I3_C2_MODE_AUTO) { // Auto
        if (IdsNvValue) {
          I3CMode8 |= BIT2;
        } else {
          I3CMode8 &= ~ BIT2;
        }
      }
    } else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C2_CONFIG_BOTHDISABLED){
      PcdData32 &= ~ BIT14;
      PcdData32 &= ~ BIT7;
    }else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C2_CONFIG_I2CENABLED) {
      PcdData32 &= ~ BIT14;
      PcdData32 |= BIT7;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C3_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_I3_C3_CONFIG_AUTO) { // Auto
    if (IdsNvValue == IDSOPT_CMN_FCH_I3_C3_CONFIG_I3CENABLED) {
      PcdData32 |= BIT15;
      PcdData32 &= ~ BIT8;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C3_MODE, &IdsNvValue)
      if (IdsNvValue != IDSOPT_CMN_FCH_I3_C3_MODE_AUTO) { // Auto
        if (IdsNvValue) {
          I3CMode8 |= BIT3;
        } else {
          I3CMode8 &= ~ BIT3;
        }
      }
    } else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C3_CONFIG_BOTHDISABLED){
      PcdData32 &= ~ BIT15;
      PcdData32 &= ~ BIT8;
    }else if (IdsNvValue == IDSOPT_CMN_FCH_I3_C3_CONFIG_I2CENABLED) {
      PcdData32 &= ~ BIT15;
      PcdData32 |= BIT8;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART0_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_UART0_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT11;
    } else {
      PcdData32 &= ~ BIT11;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART1_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_UART1_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT12;
    } else {
      PcdData32 &= ~ BIT12;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART2_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_UART2_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT16;
    } else {
      PcdData32 &= ~ BIT16;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART3_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_UART3_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT26;
    } else {
      PcdData32 &= ~ BIT26;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART4_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_UART4_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT20;
    } else {
      PcdData32 &= ~ BIT20;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_ESPI_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_ESPI_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT27;
    } else {
      PcdData32 &= ~ BIT27;
    }
  }

  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HFP_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_HFP_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT29;
    } else {
      PcdData32 &= ~ BIT29;
    }
  }
  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_HID_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT30;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPI_READ_MODE, &IdsNvValue)
      FchParams->FchRunTime.HidControl[0].SpiReadMode = (UINT8) IdsNvValue;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPEED, &IdsNvValue)
      FchParams->FchRunTime.HidControl[0].SpiSpeed = (UINT8) IdsNvValue;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPI_SPEED6_N, &IdsNvValue)
      FchParams->FchRunTime.HidControl[0].Spi_spd6 = (UINT8) IdsNvValue;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPI_SPEED7_N, &IdsNvValue)
      FchParams->FchRunTime.HidControl[0].Spi_spd7 = (UINT8) IdsNvValue;
    } else {
      PcdData32 &= ~ BIT30;
    }
  }
  IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_CONFIG, &IdsNvValue)
  if (IdsNvValue != IDSOPT_CMN_FCH_HID2_CONFIG_AUTO) { // Auto
    if (IdsNvValue) {
      PcdData32 |= BIT31;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPI_READ_MODE, &IdsNvValue)
      FchParams->FchRunTime.HidControl[1].SpiReadMode = (UINT8) IdsNvValue;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPEED, &IdsNvValue)
      FchParams->FchRunTime.HidControl[1].SpiSpeed = (UINT8) IdsNvValue;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPI_SPEED6_N, &IdsNvValue)
      FchParams->FchRunTime.HidControl[1].Spi_spd6 = (UINT8) IdsNvValue;
      IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPI_SPEED7_N, &IdsNvValue)
      FchParams->FchRunTime.HidControl[1].Spi_spd7 = (UINT8) IdsNvValue;
    } else {
      PcdData32 &= ~ BIT31;
    }
  }

  FCH_PCDSET32 (FchRTDeviceEnableMap, PcdData32);
  FCH_PCDSET8 (FchI3CMode, I3CMode8);
  FCH_PCDSETPTR (PcdFchHidControl, &Size, PtrBuffer);

  return IDS_HOOK_SUCCESS;
}

#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define FCH_RMB_IDS_HOOKS_INT_DXE
#else
  #include "Internal/FchIdsHookRmbLibIntDxe.h"
#endif

IDS_HOOK_ELEMENT FchRmbIdsHooksDxe[] = {
  {
    IDS_HOOK_FCH_INIT_ENV,
    &CmnHookFuncFchRmbDxeInitEnv
  },
  FCH_RMB_IDS_HOOKS_INT_DXE
  IDS_HOOKS_END
};

IDS_HOOK_TABLE FchRmbIdsHookTableDxe = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  FchRmbIdsHooksDxe
};

AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &FchRmbIdsHookTableDxe;
  return AGESA_SUCCESS;
}


