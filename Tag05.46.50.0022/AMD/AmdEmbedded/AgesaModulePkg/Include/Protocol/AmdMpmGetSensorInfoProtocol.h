/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_MPM_GET_SENSOR_INFO_PROTOCOL_H_
#define _AMD_MPM_GET_SENSOR_INFO_PROTOCOL_H_

#include <Library/MpmLib.h>

#pragma pack (push, 1)
//
// Extern the GUID for protocol users
//
extern EFI_GUID gAmdMpmGetSensorInfoProtocol;

typedef struct _AMD_MPM_GET_SENSOR_INFO_PROTOCOL AMD_MPM_GET_SENSOR_INFO_PROTOCOL;

/**
 * @struct for Sensor data
 * Structure for PLDM PlatformEventMessage
 */
//Possible states
typedef enum {
  PLDM_SENSOR_UNKNOWN,
  PLDM_SENSOR_GOOD,
  PLDM_SENSOR_BAD,
  PLDM_SENSOR_CLOSED,
  PLDM_SENSOE_OPEN,
  PLDM_SENSOR_LOCKED,
  PLDM_SENSOR_UNLOCKED,
  PLDM_SENSOR_HUMID,
  PLDM_SENSOR_NORMAL,
  PLDM_SENSOR_SMOKY,
  PLDM_SENSOR_PRESENT,
  PLDM_SENSOR_NOTPRESENT
} SENSOR_POSSIBLE_STATES_T;

typedef enum {
  SENSOR_TYPE_TEMPERATURE = 2,
  SENSOR_TYPE_VOLTAGE = 3,
  SENSOR_TYPE_CURRENT = 4,
  SENSOR_TYPE_TACHOMETER = 5,
  SENSOR_TYPE_HUMIDITY = 9,
  SENSOR_TYPE_SMOKE_DETECTION = 10,
  SENSOR_TYPE_AIR_FLOW = 12,
  SENSOR_TYPE_POWER_CONSUMPTION = 13,
  SENSOR_TYPE_POWER_PRODUCTION = 14,
  SENSOR_TYPE_PRESSURE = 15
} SENSOR_TYPE;

typedef enum {
  LOWER_THRESHOLD_NONCRITICAL,
  UPPER_THRESHOLD_NONCRITICAL,
  LOWER_THRESHOLD_CRITICAL,
  UPPER_THRESHOLD_CRITICAL,
  LOWER_THRESHOLD_FATAL,
  UPPER_THRESHOLD_FATAL,
} NUMERIC_THRESHOLDS_T;

typedef enum {
   FAN_CONTROL_MODES_UNKNOWN = 0,
   FAN_CONTROL_MODES_AUTOMATIC = 2,
   FAN_CONTROL_MODES_MANUAL = 3,
   FAN_CONTROL_MODES_VENDORSPECIFIC = 4
} FAN_CONTROL_MODES_T;

typedef struct {
  UINT16  SensorID;       // distinguish between each sensor
  CHAR8   SensorName[64];   //Descipition for the sensor, ex: temperature sensor; Voltage sensor.
  UINT16  SensorDatalength; // size in bytes of sensor payload
} SENSORHEADER_T;

typedef struct {
  SENSORHEADER_T Header;
  UINT16    SensorType;     // as per spec DSP1009
  UINT8     OtherSensorTypeDescription[128]; //if SensorType=1  (other) then this is valid
  UINT16    PossibleStates; // bitmap
  UINT8     CurrentState; //one of above states
  UINT64    PollingInterval;  //in ms
  UINT8     SensorContext[128];
} SENSOR_T;

typedef struct {
  UINT16    BaseUnits;    //sensor units like, rpm
  UINT32    UnitModifier;    //e.g. multiply of 10^1
  UINT16    RateUnits;
  UINT32    CurrentReading;
  UINT32    NominalReading;
  UINT32    NormalMax;
  UINT32    NormalMin;
  UINT32    MaxReadable;
  UINT32    MinReadable;
  UINT32    Resolution;    //BaseUnit*UnitModifier/RateUnit.
  UINT32    Tolerance;    // in %
  UINT32    Accuracy;    //e.g.+/- 10%
  BOOLEAN   IsLinear;  //
  UINT32    Hysteresis;
  UINT32    LowerThresholdNonCritical;
  UINT32    UpperThresholdNonCritical;
  UINT32    LowerThresholdCritical;
  UINT32    UpperThresholdCritical;
  UINT32    LowerThresholdFatal;
  UINT32    UpperThresholdFatal;
  UINT8     SupportedThresholds; //nth bit represents enum value
  UINT8     EnabledThresholds;  //nth bit represents enum value
  UINT8     SettableThresholds; //nth bit represents enum value
} NUMERICSENSOR_T;

typedef struct {
  UINT8    SystemName[128];     //Make
  UINT8    SystemModel[128];     //Model
  UINT8    SerialNumber[128];
  UINT8    OperationalStatus;    // 0-stopped, 1- running
  UINT32   VariableSpeed;    //get :0 to 100 in multiple of 10
  UINT32   DesiredSpeed;     //set :0 to 100 in multiple of 10
  UINT32   ControlMode;
  FAN_CONTROL_MODES_T ControlModesSupported[4];
} FANINFO_T;

typedef struct PLDM_SENSOR {
  SENSOR_T    Sensor;             // All sensor
  NUMERICSENSOR_T    NumericSensor; // Numeric Sensor & Tachometer
  FANINFO_T    FanInfo;            // Only for Tachometer (fan)
} PLDM_SENSOR_T;

#pragma pack (pop)

/**
  Get Sensor information

  @param[out] SensorInfo

  @retval EFI_SUCCESS      Successfully get the Sensor info
**/
typedef
EFI_STATUS
(EFIAPI *GET_SENSOR_INFO) (
  OUT PLDM_SENSOR_T  *SensorInfo
);

//
// Protocol Interface Structure
//
typedef struct _AMD_MPM_GET_SENSOR_INFO_PROTOCOL
{
  GET_SENSOR_INFO      GetSensorInfo;
} AMD_MPM_GET_SENSOR_INFO_PROTOCOL;

#endif
