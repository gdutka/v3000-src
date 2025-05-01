/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#ifndef _AOD_PLATFROM_LIB_H_
#define _AOD_PLATFROM_LIB_H_

#define AOD_VOLTAGE_APU_VDDIO     1
#define AOD_VOLTAGE_VDD_MEM       2
/**
  This function used to retrieve onboard Voltage information

  @param[in]  VoltageId             The Voltage Id
  @param[out] Defalut               The Default voltage setting
  @param[out] Min                   The Min voltage setting
  @param[out] Max                   The Max voltage setting
  @param[out] Step                  The Step voltage setting

  @retval EFI_SUCCESS if retrieve voltage information successfully
          EFI_UNSUPPORTED if voltage is unsupported.
          others by callee

**/
EFI_STATUS
AodGetOnBoardVoltageInfo (
  IN   UINT8        VoltageId,
  OUT  UINT16       *Defalut,
  OUT  UINT16       *Min,
  OUT  UINT16       *Max,
  OUT  UINT16       *Step
  );

/**
  This function used to get current VoltageId Voltage in mV

  @param[in]  VoltageId             The Voltage Id
  @param[out] Voltage               The voltage out pointer in mV

  @retval EFI_SUCCESS if get voltage successfully
          EFI_UNSUPPORTED if voltage is unsupported.
          others by callee

**/
EFI_STATUS
AodGetOnBoardVoltage (
  IN   UINT8        VoltageId,
  OUT  UINT16       *Voltage
  );

/**
  This function used to set current VoltageId Voltage in mV into VR

  @param[in]  VoltageId             The Voltage Id
  @param[out] Voltage               The voltage in mV

  @retval EFI_SUCCESS if set voltage successfully
          EFI_UNSUPPORTED if voltage is unsupported.
          others by callee

**/
EFI_STATUS
AodSetOnBoardVoltage (
  IN  UINT8        VoltageId,
  IN  UINT16       Voltage
  );
#endif