/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

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

#ifndef _AOD_ON_BOARD_VOLTAGE_H_
#define _AOD_ON_BOARD_VOLTAGE_H_
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Guid/SocCoreInfo.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/MdeModuleHii.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/AmdApcbProtocol.h>
#include <RMB/ApcbV3TokenUid.h>
#include <RMB/ApcbV3Priority.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/SmBus.h>
#include <Protocol/SmbusHc.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include <Features/AODv2/Include/Library/AodPlatformLib.h>
#include "AodSocLib.h"


typedef union _PMIC_ADDRESS {
  struct {
    UINT8 Dimm      : 3;
    UINT8 Lid       : 4;
    UINT8 Reserved0 : 1;
  } Bits;
  UINT8 Raw;
} PMIC_ADDRESS;

/**
  This function used to read PMIC SWA
  R25 [7:1]: SWB_VOLTAGE_SETTING
  R21 [7:1]: SWA_VOLTAGE_SETTING
  Switch Node B Output Regulator Voltage Setting3,4,5
  000 0000 = 800 mV
  000 0001 = 805 mV
  000 0010 = 810 mV
  ...
  011 1100 = 1100 mV
  ...
  111 1101 = 1425 mV
  111 1110 = 1430 mV
  111 1111 = 1435 mV
  @param[in]  DimmAddress          The Spd Address
  @param[in]  Register             The offset of PMIC
  @param[out] Data                 The Data output

  @retval EFI_SUCCESS if it is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodDdr5ReadVddIo (
  OUT  UINT16          *DimmVdd,
  OUT  UINT16          *DimmVddq
  );

EFI_STATUS
EFIAPI
AodDdr5ReadVpp (
  OUT  UINT16          *DimmVpp
  );
/**
  This function used to generate APU VDDIO options hii data

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateApuVddIoOptions (
  IN EFI_HII_HANDLE                       HiiHandle
  );
/**
  This function used to update the voltage string
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodUpdateStringCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );

/**
  This function used to sync DIMM VddIo Into APU VddIo
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodDimmVddIoCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );

/**
  This function used to sync DIMM VddIo Into APU VddIo
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodApuVddIoCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );

/**
  This function used to sync DIMM VddIo Into APU VddIo when AodApuVddIo is 1
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodVddIoCtrlCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );

/**
  This function used to generate VDD_MEM options hii data

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateVddMemOptions (
  IN EFI_HII_HANDLE                       HiiHandle
  );

/**
  This function used to Set VDD_MEM into ABL
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodApuVddMemCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );
#endif