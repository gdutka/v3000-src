/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Uefi/UefiSpec.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Protocol/AmdMpmGetSensorInfoProtocol.h>
#include <Library/AmdBaseLib.h>
#include <Library/MpmLib.h>
#include <Protocol/AmdCpmTableProtocol/AmdCpmTableProtocol.h>
#include <Protocol/SmbusHc.h>
#include <Library/TimerLib.h>

#define MAX_NUM_SENSOR_SUPPORT     10
#define FCH_PMIO2_BASE             0xFED80400
#define FCH_PMIO2_REG66            0x66 //fan0 detector control
#define FCH_PMIO2_REG69            0x69 //fan0 speed low
#define FCH_PMIO2_REG6A            0x6A //fan0 speed high

AMD_CPM_TABLE_PROTOCOL *CpmTableProtocolPtr;

//10 sensors support
//OEM and customer can modify the GetSensorInfo according the hardware platform
EFI_STATUS
EFIAPI
GetSensorInfo0 (OUT PLDM_SENSOR_T *SensorInfo)
{
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  UINT8 CurrentReadingTempH;
  UINT8 CurrentReadingTempL;
  CHAR8 *SensorNameString = "APU Internal temperature";
  //APU Internal temperature
  //Read temperature from ECRAM 0xF2, 0xF3

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  //Read Current Temperature
  TempOffset = 0xF3;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempH);
  TempOffset = 0xF2;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempL);

  SensorInfo->Sensor.Header.SensorID = 0;
  AsciiStrCpyS(SensorInfo->Sensor.Header.SensorName, AsciiStrLen(SensorNameString) + 1, SensorNameString);
  DEBUG ((DEBUG_INFO, "AsciiStrLen(SensorNameString) = 0x%x\n", AsciiStrLen(SensorNameString)));
  SensorInfo->Sensor.Header.SensorDatalength = sizeof (SENSOR_T) + sizeof (NUMERICSENSOR_T);
  SensorInfo->Sensor.SensorType = SENSOR_TYPE_TEMPERATURE;
  SensorInfo->Sensor.PollingInterval = 10;
  SensorInfo->Sensor.PossibleStates = PLDM_SENSOR_GOOD | PLDM_SENSOR_PRESENT;
  SensorInfo->Sensor.CurrentState = PLDM_SENSOR_GOOD;
  SensorInfo->NumericSensor.CurrentReading = CurrentReadingTempL | (CurrentReadingTempH << 8);
  SensorInfo->NumericSensor.LowerThresholdNonCritical = 40;
  SensorInfo->NumericSensor.UpperThresholdNonCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdCritical = 80;
  SensorInfo->NumericSensor.UpperThresholdCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdFatal = 120;
  SensorInfo->NumericSensor.UpperThresholdFatal = 0;
  SensorInfo->NumericSensor.SupportedThresholds = 0x3F;
  SensorInfo->NumericSensor.BaseUnits = 1;
  SensorInfo->NumericSensor.RateUnits = 1;
  SensorInfo->NumericSensor.UnitModifier = 1;
  SensorInfo->NumericSensor.Tolerance = 1;
  SensorInfo->NumericSensor.Accuracy = 1;
  DEBUG ((DEBUG_INFO, "GetSensorInfo0 temperature = 0x%x\n", SensorInfo->NumericSensor.CurrentReading));
  SensorInfo->NumericSensor.IsLinear = TRUE;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetSensorInfo1 (OUT PLDM_SENSOR_T *SensorInfo)
{
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  UINT8 CurrentReadingTempH;
  UINT8 CurrentReadingTempL;
  CHAR8 *SensorNameString = "Charger temperature";
  //Charger temperature
  //Read temperature from ECRAM 0xF4, 0xF5

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  //Read Current Temperature
  TempOffset = 0xF5;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempH);
  TempOffset = 0xF4;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempL);

  SensorInfo->Sensor.Header.SensorID = 1;
  AsciiStrCpyS(SensorInfo->Sensor.Header.SensorName, AsciiStrLen(SensorNameString) + 1, SensorNameString);
  DEBUG ((DEBUG_INFO, "AsciiStrLen(SensorNameString) = 0x%x\n", AsciiStrLen(SensorNameString)));
  SensorInfo->Sensor.Header.SensorDatalength = sizeof (SENSOR_T) + sizeof (NUMERICSENSOR_T);
  SensorInfo->Sensor.SensorType = SENSOR_TYPE_TEMPERATURE;
  SensorInfo->Sensor.PollingInterval = 10;
  SensorInfo->Sensor.PossibleStates = PLDM_SENSOR_GOOD | PLDM_SENSOR_PRESENT;
  SensorInfo->Sensor.CurrentState = PLDM_SENSOR_GOOD;
  SensorInfo->NumericSensor.CurrentReading = CurrentReadingTempL | (CurrentReadingTempH << 8);
  SensorInfo->NumericSensor.LowerThresholdNonCritical = 40;
  SensorInfo->NumericSensor.UpperThresholdNonCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdCritical = 80;
  SensorInfo->NumericSensor.UpperThresholdCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdFatal = 120;
  SensorInfo->NumericSensor.UpperThresholdFatal = 0;
  SensorInfo->NumericSensor.SupportedThresholds = 0x3F;
  SensorInfo->NumericSensor.BaseUnits = 1;
  SensorInfo->NumericSensor.RateUnits = 1;
  SensorInfo->NumericSensor.UnitModifier = 1;
  SensorInfo->NumericSensor.Tolerance = 1;
  SensorInfo->NumericSensor.Accuracy = 1;
  DEBUG ((DEBUG_INFO, "GetSensorInfo1 temperature = 0x%x\n", SensorInfo->NumericSensor.CurrentReading));
  SensorInfo->NumericSensor.IsLinear = TRUE;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetSensorInfo2 (OUT PLDM_SENSOR_T *SensorInfo)
{
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  UINT8 CurrentReadingTempH;
  UINT8 CurrentReadingTempL;
  CHAR8 *SensorNameString = "APU External temperature";
  //APU External temperature
  //Read temperature from ECRAM 0xF6, 0xF7

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  //Read Current Temperature
  TempOffset = 0xF7;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempH);
  TempOffset = 0xF6;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempL);

  SensorInfo->Sensor.Header.SensorID = 2;
  AsciiStrCpyS(SensorInfo->Sensor.Header.SensorName, AsciiStrLen(SensorNameString) + 1, SensorNameString);
  SensorInfo->Sensor.Header.SensorDatalength = sizeof (SENSOR_T) + sizeof (NUMERICSENSOR_T);
  SensorInfo->Sensor.SensorType = SENSOR_TYPE_TEMPERATURE;
  SensorInfo->Sensor.PollingInterval = 10;
  SensorInfo->Sensor.PossibleStates = PLDM_SENSOR_GOOD | PLDM_SENSOR_PRESENT;
  SensorInfo->Sensor.CurrentState = PLDM_SENSOR_GOOD;
  SensorInfo->NumericSensor.CurrentReading = CurrentReadingTempL | (CurrentReadingTempH << 8);
  SensorInfo->NumericSensor.LowerThresholdNonCritical = 40;
  SensorInfo->NumericSensor.UpperThresholdNonCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdCritical = 80;
  SensorInfo->NumericSensor.UpperThresholdCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdFatal = 120;
  SensorInfo->NumericSensor.UpperThresholdFatal = 0;
  SensorInfo->NumericSensor.SupportedThresholds = 0x3F;
  SensorInfo->NumericSensor.BaseUnits = 1;
  SensorInfo->NumericSensor.RateUnits = 1;
  SensorInfo->NumericSensor.UnitModifier = 1;
  SensorInfo->NumericSensor.Tolerance = 1;
  SensorInfo->NumericSensor.Accuracy = 1;
  DEBUG ((DEBUG_INFO, "GetSensorInfo2 temperature = 0x%x\n", SensorInfo->NumericSensor.CurrentReading));
  SensorInfo->NumericSensor.IsLinear = TRUE;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetSensorInfo3 (OUT PLDM_SENSOR_T *SensorInfo)
{
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  UINT8 CurrentReadingTempH;
  UINT8 CurrentReadingTempL;
  CHAR8 *SensorNameString = "IO temperature";
  //I/O temperature
  //Read temperature from ECRAM 0xF8, 0xF9

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  //Read Current Temperature
  TempOffset = 0xF9;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempH);
  TempOffset = 0xF8;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &CurrentReadingTempL);

  SensorInfo->Sensor.Header.SensorID = 3;
  AsciiStrCpyS(SensorInfo->Sensor.Header.SensorName, AsciiStrLen(SensorNameString) + 1, SensorNameString);
  SensorInfo->Sensor.Header.SensorDatalength = sizeof (SENSOR_T) + sizeof (NUMERICSENSOR_T);
  SensorInfo->Sensor.SensorType = SENSOR_TYPE_TEMPERATURE;
  SensorInfo->Sensor.PollingInterval = 10;
  SensorInfo->Sensor.PossibleStates = PLDM_SENSOR_GOOD | PLDM_SENSOR_PRESENT;
  SensorInfo->Sensor.CurrentState = PLDM_SENSOR_GOOD;
  SensorInfo->NumericSensor.CurrentReading = CurrentReadingTempL | (CurrentReadingTempH << 8);
  SensorInfo->NumericSensor.LowerThresholdNonCritical = 40;
  SensorInfo->NumericSensor.UpperThresholdNonCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdCritical = 80;
  SensorInfo->NumericSensor.UpperThresholdCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdFatal = 120;
  SensorInfo->NumericSensor.UpperThresholdFatal = 0;
  SensorInfo->NumericSensor.SupportedThresholds = 0x3F;
  SensorInfo->NumericSensor.BaseUnits = 1;
  SensorInfo->NumericSensor.RateUnits = 1;
  SensorInfo->NumericSensor.UnitModifier = 1;
  SensorInfo->NumericSensor.Tolerance = 1;
  SensorInfo->NumericSensor.Accuracy = 1;
  DEBUG ((DEBUG_INFO, "GetSensorInfo3 temperature = 0x%x\n", SensorInfo->NumericSensor.CurrentReading));
  SensorInfo->NumericSensor.IsLinear = TRUE;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetSensorInfo4 (OUT PLDM_SENSOR_T *SensorInfo)
{
  UINT8 TempOffset = 0;
  UINT8 Value;
  UINT8 Smbus0BuffValue;
  EFI_SMBUS_HC_PROTOCOL      *mSmbus = NULL;
  EFI_STATUS Status;
  EFI_SMBUS_DEVICE_ADDRESS  SmbusSlaveAddress;
  CHAR8 *SensorNameString = "DDR external temperature";
  UINTN Length = 1;
  UINT8 LocalTemHighByte;
  UINT8 LocalTemLowByte;
  UINT8 ThermalHysteresis;

  //DDR External temperature

  //Enable SMBUS0_BUFF
  TempOffset = 0xA8;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &Smbus0BuffValue);
  Value = Smbus0BuffValue;
  Value = Value | 0x1;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &Value);

  SmbusSlaveAddress.SmbusDeviceAddress = 0x4D;
  Status = gBS->LocateProtocol (&gEfiSmbusHcProtocolGuid, NULL, (VOID**) &mSmbus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Locate gEfiSmbusHcProtocolGuid failed\n"));
    return Status;
  }

  mSmbus->Execute (mSmbus, SmbusSlaveAddress, 0x00, EfiSmbusReadByte, FALSE, &Length, &LocalTemHighByte);
  DEBUG ((DEBUG_INFO, "Read LocalTemHighByte = x%x\n", LocalTemHighByte));
  mSmbus->Execute (mSmbus, SmbusSlaveAddress, 0x29, EfiSmbusReadByte, FALSE, &Length, &LocalTemLowByte);
  DEBUG ((DEBUG_INFO, "Read LocalTemLowByte Status = 0x%x\n", LocalTemLowByte));

  mSmbus->Execute (mSmbus, SmbusSlaveAddress, 0x21, EfiSmbusReadByte, FALSE, &Length, &ThermalHysteresis);
  DEBUG ((DEBUG_INFO, "Read ThermalHysteresis = 0x%x\n", ThermalHysteresis));


  SensorInfo->Sensor.Header.SensorID = 4;
  AsciiStrCpyS(SensorInfo->Sensor.Header.SensorName, AsciiStrLen(SensorNameString) + 1, SensorNameString);
  SensorInfo->Sensor.Header.SensorDatalength = sizeof (SENSOR_T) + sizeof (NUMERICSENSOR_T);
  SensorInfo->Sensor.SensorType = SENSOR_TYPE_TEMPERATURE;
  SensorInfo->Sensor.PollingInterval = 10;
  SensorInfo->Sensor.PossibleStates = PLDM_SENSOR_GOOD | PLDM_SENSOR_PRESENT;
  SensorInfo->Sensor.CurrentState = PLDM_SENSOR_GOOD;
  SensorInfo->NumericSensor.CurrentReading = LocalTemLowByte | (LocalTemHighByte << 8);
  SensorInfo->NumericSensor.LowerThresholdNonCritical = 40;
  SensorInfo->NumericSensor.UpperThresholdNonCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdCritical = 80;
  SensorInfo->NumericSensor.UpperThresholdCritical = 0;
  SensorInfo->NumericSensor.LowerThresholdFatal = 120;
  SensorInfo->NumericSensor.UpperThresholdFatal = 0;
  SensorInfo->NumericSensor.SupportedThresholds = 0x3F;
  SensorInfo->NumericSensor.BaseUnits = 1;
  SensorInfo->NumericSensor.RateUnits = 1;
  SensorInfo->NumericSensor.UnitModifier = 1;
  SensorInfo->NumericSensor.Tolerance = 1;
  SensorInfo->NumericSensor.Accuracy = 1;
  SensorInfo->NumericSensor.Hysteresis = ThermalHysteresis;
  DEBUG ((DEBUG_INFO, "GetSensorInfo4 temperature = 0x%x\n", SensorInfo->NumericSensor.CurrentReading));
  SensorInfo->NumericSensor.IsLinear = TRUE;

  //Restore SMBUS0_BUFF value
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &Smbus0BuffValue);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetSensorInfo5 (OUT PLDM_SENSOR_T *SensorInfo)
{
  CHAR8 *SensorNameString = "Chassis Fan information";
  UINT8 Value = 0;
  UINT8 FanSpeedLowByte = 0;
  UINT8 FanSpeedHighByte = 0;

  //Fan speed with fake data
  SensorInfo->Sensor.Header.SensorID = 5;
  AsciiStrCpyS(SensorInfo->Sensor.Header.SensorName, AsciiStrLen(SensorNameString) + 1, SensorNameString);
  SensorInfo->Sensor.Header.SensorDatalength = sizeof (PLDM_SENSOR_T);
  SensorInfo->Sensor.SensorType = SENSOR_TYPE_TACHOMETER;
  SensorInfo->NumericSensor.BaseUnits = 1;
  SensorInfo->NumericSensor.RateUnits = 1;
  SensorInfo->NumericSensor.UnitModifier = 1;
  SensorInfo->NumericSensor.Tolerance = 1;
  SensorInfo->NumericSensor.Accuracy = 1;
  SensorInfo->Sensor.PollingInterval = 10;
  SensorInfo->Sensor.PossibleStates = PLDM_SENSOR_GOOD | PLDM_SENSOR_PRESENT;
  SensorInfo->Sensor.CurrentState = PLDM_SENSOR_GOOD;
  SensorInfo->FanInfo.ControlMode = FAN_CONTROL_MODES_AUTOMATIC;
  SensorInfo->FanInfo.DesiredSpeed = 3000;
  SensorInfo->FanInfo.OperationalStatus = 1;
  SensorInfo->FanInfo.OperationalStatus = 1;
  SensorInfo->NumericSensor.IsLinear = TRUE;
  Value = MmioRead8(FCH_PMIO2_BASE + FCH_PMIO2_REG66);
  Value = Value | 1; //enable fan detector
  MmioWrite8 (FCH_PMIO2_BASE + FCH_PMIO2_REG66, Value);
  Value = MmioRead8(FCH_PMIO2_BASE + FCH_PMIO2_REG66);
  MicroSecondDelay(200*1000); //delay 200ms for reg69 and 6A to prepare the data
  FanSpeedLowByte = MmioRead8(FCH_PMIO2_BASE + FCH_PMIO2_REG69);
  FanSpeedLowByte = FanSpeedLowByte >> 1; //bit1-7: fan speed low, bit 0: reserved
  FanSpeedHighByte = MmioRead8(FCH_PMIO2_BASE + FCH_PMIO2_REG6A);
  SensorInfo->FanInfo.VariableSpeed = (FanSpeedHighByte << 8) | FanSpeedLowByte;
  DEBUG ((DEBUG_INFO, "GetSensorInfo5 Fan speed = 0x%x\n", SensorInfo->FanInfo.VariableSpeed));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetSensorInfo6 (OUT PLDM_SENSOR_T *SensorInfo)
{

  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetSensorInfo7 (OUT PLDM_SENSOR_T *SensorInfo)
{

  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetSensorInfo8 (OUT PLDM_SENSOR_T *SensorInfo)
{

  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetSensorInfo9 (OUT PLDM_SENSOR_T *SensorInfo)
{

  return EFI_UNSUPPORTED;
}

BOOLEAN  gGetSensorTable[MAX_NUM_SENSOR_SUPPORT] = {
  TRUE,
  TRUE,
  TRUE,
  TRUE,
  TRUE,
  TRUE,
  FALSE,
  FALSE,
  FALSE,
  FALSE
};

AMD_MPM_GET_SENSOR_INFO_PROTOCOL   mAmdGetSensorInfoProtocol[MAX_NUM_SENSOR_SUPPORT] = {
  GetSensorInfo0,
  GetSensorInfo1,
  GetSensorInfo2,
  GetSensorInfo3,
  GetSensorInfo4,
  GetSensorInfo5,
  GetSensorInfo6,
  GetSensorInfo7,
  GetSensorInfo8,
  GetSensorInfo9
};

EFI_STATUS
EFIAPI
SensorInfoDxeEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        mHandle[MAX_NUM_SENSOR_SUPPORT];

  DEBUG ((DEBUG_INFO, "%a: enter\n", __FUNCTION__));

  // Check MPM Enable, only MPM need this information, return if MPM is disabled
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  if ((PcdGet32 (PcdAmdEnvironment98) & BIT16) || (PcdGet32 (PcdAmdEnvironmentFlag) != 0)) {
    //return if EC disabled or Emulation platform
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Locate gAmdCpmTableProtocolGuid error\n", __FUNCTION__));
    return Status;
  }

  for (UINT8 i = 0; i < MAX_NUM_SENSOR_SUPPORT; i ++) {
    if (gGetSensorTable[i] == TRUE) {
      mHandle[i] = NULL;
      Status = gBS->InstallProtocolInterface (
                  &mHandle[i],
                  &gAmdMpmGetSensorInfoProtocol,
                  EFI_NATIVE_INTERFACE,
                  &mAmdGetSensorInfoProtocol[i]
                  );
      DEBUG ((DEBUG_INFO, "Install GetSensorInfo Protocol %d Status = %r\n", i, Status));
    }
  }
  return Status;
}

