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
#include "AodVoltage.h"

AOD_MOUDLE_NAME ("AodVoltage")
AOD_SOC_NAME ("RMB")
extern EFI_GUID mAodFormSetGuid;
EFI_STATUS
EFIAPI
AodMemDdr5ReadPmic (
  IN  UINT8         DimmAddress,
  IN  UINT8         Register,
  OUT UINT8         *Data
  )
{
  EFI_STATUS                 Status;
  EFI_SMBUS_HC_PROTOCOL      *Smbus = NULL;
  EFI_SMBUS_DEVICE_ADDRESS   SmbusDeviceAddress;
  PMIC_ADDRESS               Address;
  UINTN                      Length = 1;
  if (Data == NULL) return EFI_INVALID_PARAMETER;
  Address.Bits.Dimm = (DimmAddress >> 1) & 7;
  Address.Bits.Lid  = 9;
  Status = gBS->LocateProtocol (&gEfiSmbusHcProtocolGuid, NULL, (VOID**) &Smbus);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_ERROR, "Can't load %g protocol status = %r\n", &gEfiSmbusHcProtocolGuid, Status));
    return Status;
  }

  SmbusDeviceAddress.SmbusDeviceAddress = Address.Raw;
  Status = Smbus->Execute (
                    Smbus,
                    SmbusDeviceAddress,
                    Register,
                    EfiSmbusReadByte,
                    FALSE,
                    &Length,
                    Data
                    );

  AOD_DEBUG_SOC ((EFI_D_ERROR, "Read Register %x Data = %x Status = %r\n", Register, *Data, Status));
  return Status;

}

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
  )
{
  EFI_STATUS          Status;
  UINT8               SpdAddress;
  UINT8               Vdd = 0;
  UINT8               Vddq = 0;
  *DimmVdd = 0;
  *DimmVddq = 0;
  for (SpdAddress = 0xA0; SpdAddress < 0xB0; SpdAddress += 2) {
    Status = AodMemDdr5ReadPmic (SpdAddress, 0x25, &Vddq);
    if (!EFI_ERROR (Status)) break;
  }
  if (EFI_ERROR (Status)) return Status;
  Status = AodMemDdr5ReadPmic (SpdAddress, 0x21, &Vdd);
  if (EFI_ERROR (Status))  return Status;

  *DimmVdd = (UINT16)(800 + (Vdd >> 1)*5);
  *DimmVddq = (UINT16)(800 + (Vddq >> 1)*5);
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDD = %d, VDDQ = %d\n", *DimmVdd, *DimmVddq));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AodDdr5ReadVpp (
  OUT  UINT16          *DimmVpp
  )
{
  EFI_STATUS          Status;
  UINT8               SpdAddress;
  UINT8               Vpp = 0;
  for (SpdAddress = 0xA0; SpdAddress < 0xB0; SpdAddress += 2) {
    Status = AodMemDdr5ReadPmic (SpdAddress, 0x27, &Vpp);
    if (!EFI_ERROR (Status)) break;
  }
  if (EFI_ERROR (Status)) return Status;
  AOD_DEBUG_SOC ((EFI_D_INFO, "VPP = %x\n", 1500 + (Vpp >> 1)*5));
  *DimmVpp = (UINT16)(1500 + (Vpp >> 1)*5);
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                       Status;
  UINT16                           Min,Max,Step,Defaut;
  VOID                             *StartOpCodeHandle;
  VOID                             *EndOpCodeHandle;
  VOID                             *DefaultOpCodeHandle;
  EFI_IFR_GUID_LABEL               *StartLabel;
  EFI_IFR_GUID_LABEL               *EndLabel;
  CHAR16                           Temp[0x80];
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  Status = AodGetOnBoardVoltageInfo (AOD_VOLTAGE_APU_VDDIO, &Defaut, &Min, &Max, &Step);
  if (EFI_ERROR (Status)) return Status;

  if(StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Create Hii Extended Label OpCode as the start and end opcode
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if(StartLabel == NULL || EndLabel == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_CBS_AOD_APU_VDD_IO_DYNAMIC_START;
  EndLabel->Number = LABEL_CBS_AOD_APU_VDD_IO_DYNAMIC_END;
  DefaultOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (DefaultOpCodeHandle == NULL) return EFI_OUT_OF_RESOURCES;
  HiiCreateDefaultOpCode (DefaultOpCodeHandle, EFI_HII_DEFAULT_CLASS_STANDARD, EFI_IFR_TYPE_NUM_SIZE_16, Defaut);
  ZeroMem(Temp, sizeof(Temp));
  UnicodeSPrint (Temp, sizeof(Temp), L"Adjust APU VDDIO, stepping is %dmV. Range is from %dmV to %dmV", Step, Min, Max);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_APU_VDDIO_ADJUST_HELP), Temp, NULL);
  HiiCreateNumericOpCode (
    StartOpCodeHandle,
    (EFI_QUESTION_ID) OFFSET_OF(AOD_CONFIG, AodApuVddIo),
    VARSTORE_ID_AODSETUP,
    (UINT16)OFFSET_OF(AOD_CONFIG, AodApuVddIo),
    STRING_TOKEN (STR_AOD_APU_VDDIO_ADJUST),
    STRING_TOKEN (STR_AOD_APU_VDDIO_ADJUST_HELP),
    EFI_IFR_FLAG_RESET_REQUIRED | EFI_IFR_FLAG_CALLBACK,
    EFI_IFR_NUMERIC_SIZE_2 | EFI_IFR_DISPLAY_UINT_DEC,
    Min,
    Max,
    Step,
    DefaultOpCodeHandle
    );
  Status = HiiUpdateForm (
             HiiHandle,
             &mAodFormSetGuid,
             SETUP_VDDIO_VOLTAGE_CONTROL_LABLE,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "HiiUpdateForm Status = %r \n", Status));
    return Status;
  }
  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
  }
  if (EndOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }
  if (DefaultOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (DefaultOpCodeHandle);
  }
  return Status;
}

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
  )
{
  EFI_STATUS                       Status;
  UINT16                           Min,Max,Step,Defaut;
  VOID                             *StartOpCodeHandle;
  VOID                             *EndOpCodeHandle;
  VOID                             *DefaultOpCodeHandle;
  EFI_IFR_GUID_LABEL               *StartLabel;
  EFI_IFR_GUID_LABEL               *EndLabel;
  CHAR16                           Temp[0x80];
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  Status = AodGetOnBoardVoltageInfo (AOD_VOLTAGE_VDD_MEM, &Defaut, &Min, &Max, &Step);
  if (EFI_ERROR (Status)) return Status;

  if(StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Create Hii Extended Label OpCode as the start and end opcode
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if(StartLabel == NULL || EndLabel == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_CBS_AOD_VDD_MEM_ADJUST_START;
  EndLabel->Number = LABEL_CBS_AOD_VDD_MEM_ADJUST_END;
  DefaultOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (DefaultOpCodeHandle == NULL) return EFI_OUT_OF_RESOURCES;
  HiiCreateDefaultOpCode (DefaultOpCodeHandle, EFI_HII_DEFAULT_CLASS_STANDARD, EFI_IFR_TYPE_NUM_SIZE_16, Defaut);
  ZeroMem(Temp, sizeof(Temp));
  UnicodeSPrint (Temp, sizeof(Temp), L"Adjust VDD_MEM, stepping is %dmV. Range is from %dmV to %dmV", Step, Min, Max);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_APU_VDD_MEM_ADJUST_HELP), Temp, NULL);
  HiiCreateNumericOpCode (
    StartOpCodeHandle,
    (EFI_QUESTION_ID) OFFSET_OF(AOD_CONFIG, AodVddMemAdjust),
    VARSTORE_ID_AODSETUP,
    (UINT16)OFFSET_OF(AOD_CONFIG, AodVddMemAdjust),
    STRING_TOKEN (STR_AOD_APU_VDD_MEM_ADJUST),
    STRING_TOKEN (STR_AOD_APU_VDD_MEM_ADJUST_HELP),
    EFI_IFR_FLAG_RESET_REQUIRED | EFI_IFR_FLAG_CALLBACK,
    EFI_IFR_NUMERIC_SIZE_2 | EFI_IFR_DISPLAY_UINT_DEC,
    Min,
    Max,
    Step,
    DefaultOpCodeHandle
    );
  Status = HiiUpdateForm (
             HiiHandle,
             &mAodFormSetGuid,
             SETUP_VDD_MEM_VOLTAGE_CONTROL_LABLE,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "HiiUpdateForm Status = %r \n", Status));
    return Status;
  }
  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
  }
  if (EndOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }
  if (DefaultOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (DefaultOpCodeHandle);
  }
  return Status;
}

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
  )
{
  EFI_STATUS                       Status;
  CHAR16                           Temp[0x80];
  UINT16                           ApuCurrent, DimmVddqCurrent, DimmVddCurrent, DimmVpp, VddMemCurrent;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  Status = AodGetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, &ApuCurrent);
  AOD_DEBUG_SOC ((EFI_D_INFO, "AodGetApuVddio Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    ZeroMem(Temp, sizeof(Temp));
    UnicodeSPrint (Temp, sizeof(Temp), L"%dmV", ApuCurrent);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_CURRENT_APU_VDDIO_VOLTAGE), Temp, NULL);
  }
  Status = AodGetOnBoardVoltage (AOD_VOLTAGE_VDD_MEM, &VddMemCurrent);
  AOD_DEBUG_SOC ((EFI_D_INFO, "AodGetApuVddio Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    ZeroMem(Temp, sizeof(Temp));
    UnicodeSPrint (Temp, sizeof(Temp), L"%dmV", VddMemCurrent);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_CURRENT_VDD_MEM_VOLTAGE), Temp, NULL);
  }
  Status = AodDdr5ReadVddIo (&DimmVddCurrent, &DimmVddqCurrent);
  AOD_DEBUG_SOC ((EFI_D_INFO, "AodDdr5ReadVddIo Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    ZeroMem(Temp, sizeof(Temp));
    UnicodeSPrint (Temp, sizeof(Temp), L"%dmV", DimmVddCurrent);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_CURRENT_VDD_VOLTAGE), Temp, NULL);
    ZeroMem(Temp, sizeof(Temp));
    UnicodeSPrint (Temp, sizeof(Temp), L"%dmV", DimmVddqCurrent);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_CURRENT_VDDQ_VOLTAGE), Temp, NULL);
  }
  Status = AodDdr5ReadVpp (&DimmVpp);
  AOD_DEBUG_SOC ((EFI_D_INFO, "AodDdr5ReadVpp Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    ZeroMem(Temp, sizeof(Temp));
    UnicodeSPrint (Temp, sizeof(Temp), L"%dmV", DimmVpp);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_AOD_CURRENT_VPP_VOLTAGE), Temp, NULL);
  }
  return Status;
}
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
  )
{
  AOD_CONFIG                              *AodConfig;
  EFI_STATUS                              Status = EFI_SUCCESS;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  AodConfig = (AOD_CONFIG *)IfrData;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != KEY_AOD_DIMM_VDDIO) {
    return EFI_UNSUPPORTED;
  }

  if (AodConfig->AodVddIoCtrl == 1) {
    Status = AodSetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, Value->u16);
    AodConfig->AodApuVddIo = Value->u16;
    AodConfig->AodDimmVddq = Value->u16;
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "End Status = %r\n", Status));
  return Status;
}

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
  )
{
  EFI_STATUS                              Status;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != OFFSET_OF(AOD_CONFIG, AodVddMemAdjust)) {
    return EFI_UNSUPPORTED;
  }

  Status = AodSetOnBoardVoltage (AOD_VOLTAGE_VDD_MEM, Value->u16);
  AOD_DEBUG_SOC ((EFI_D_INFO, "End Status = %r\n", Status));
  return Status;
}
/**
  This function used to sync APU VddIo into ABL
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
  )
{
  EFI_STATUS                              Status;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != OFFSET_OF(AOD_CONFIG, AodApuVddIo)) {
    return EFI_UNSUPPORTED;
  }

  Status = AodSetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, Value->u16);
  AOD_DEBUG_SOC ((EFI_D_INFO, "End Status = %r\n", Status));
  return Status;
}
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
  )
{
  AOD_CONFIG                              *AodConfig;
  EFI_STATUS                              Status = EFI_SUCCESS;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  AodConfig = (AOD_CONFIG *)IfrData;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != KEY_AOD_VDDIO_CTRL) {
    return EFI_UNSUPPORTED;
  }
  if (Value->u8 == 0) {
    AodConfig->AodApuVddIo = 1100;
    AodConfig->AodDimmVdd = 1100;
    AodConfig->AodDimmVddq = 1100;
    Status = AodSetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, Value->u16);
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "End Status = %r\n", Status));
  return Status;
}