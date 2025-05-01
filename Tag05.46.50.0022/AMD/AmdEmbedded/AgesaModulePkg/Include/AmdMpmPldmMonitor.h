/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef  _AMD_MPM_PLDM_MONITOR_H_
#define _AMD_MPM_PLDM_MONITOR_H_
//Possible States
typedef enum {
  SENSOE_STATE_UNKNOWN = 0,
  SENSOE_STATE_GOOD,
  SENSOE_STATE_BAD,
  SENSOE_STATE_CLOSED,
  SENSOE_STATE_OPEN,
  SENSOE_STATE_LOCKED,
  SENSOE_STATE_UNLOCKED,
  SENSOE_STATE_HUMID,
  SENSOE_STATE_NORMAL,
  SENSOE_STATE_SMOKY,
  SENSOE_STATE_PRESENT,
  SENSOE_STATE_NOTPRESENT
} SENSOR_POSSIBLE_STATES;

typedef enum {
  LOWERTHRESHOLDNONCRITICAL = 0,
  UPPERTHRESHOLDNONCRITICAL,
  LOWERTHRESHOLDCRITICAL,
  UPPERTHRESHOLDCRITICAL,
  LOWERTHRESHOLDFATAL,
  UPPERTHRESHOLDFATAL,
} NUMERIC_THRESHOLDS;

typedef enum {
  MUNKNOWN = 0,
  AUTOMATIC = 2,
  MANUAL = 3,
  VENDORSPECIFIC = 4
} FAN_CONTROL_MODES;

#pragma pack (push, 1)
typedef struct {
  UINT16   SensorId;       // DISTINGUISH BETWEEN EACH SENSOR
  UINT8    SensorDatalength; // size in bytes of sensor payload
} SENSOR_HEADER;

typedef struct {
  SENSOR_HEADER Header;
  UINT16    SensorType;     // as per spec DSP1009
  UINT8     OtherSensorTypeDescription[128]; //if SensorType=1  (other) then this is valid
  UINT16    PossibleStates; // bitmap
  SENSOR_POSSIBLE_STATES    CurrentState; //one of above states
  UINT64    PollingInterval;  //in ms
  UINT8     SensorContext[128];
} SENSOR;

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
} NUMERIC_SENSOR;

typedef struct {
  UINT8    SystemName[1024];     //Make
  UINT8    SystemModel[1024];     //Model
  UINT8    SerialNumber[1024];
  UINT8    OperationalStatus;    // 0-stopped, 1- running
  UINT32   VariableSpeed;    //get :0 to 100 in multiple of 10
  UINT32   DesiredSpeed;     //set :0 to 100 in multiple of 10
  UINT32   ControlMode;
  FAN_CONTROL_MODES ControlModesSupported[4];
} FAN_INFO;

typedef struct PLDM_SENSOR {
  SENSOR    Sensor;             // All sensor
  NUMERIC_SENSOR    NumericSensor; // Numeric Sensor & Tachometer
  FAN_INFO    FanInfo;            // Only for Tachometer (fan)
} PLDM_SENSOR;
#pragma pack (pop)
#endif
