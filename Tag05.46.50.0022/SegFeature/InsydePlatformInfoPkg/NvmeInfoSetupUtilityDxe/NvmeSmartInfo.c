/** @file
  This source file implements functions that update the SMART section of the
  device information form.

;******************************************************************************
;* Copyright (c) 2021 - 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <Protocol/H2ONvmeInfo.h>

#include <Library/BaseMemoryLib.h>
#include <Library/HiiExLib.h>
#include <Library/HiiLib.h>                   // MdeModulePkg
#include <Library/MemoryAllocationLib.h>
#include <Library/NvmeInfoLib.h>              // InsydePlatformInfoPkg
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "NvmeInfoSetupUtilityDxe.h"
#include "NvmeDetailDisplay.h"
#include "NvmeInfoFormset.h"

extern H2O_SETUP_NVME_INFO_PRIVATE_DATA mNvmeInfoData;

#pragma pack(1)

//
// SMART / Health Information (Log Identifier 02h)
// (NVM Express Base Specification revisions 1.4 - 2.0)
//
typedef struct {
  //
  // Critical Warnings (byte 0)
  //
  UINT8  CriticalWarningAvailableSpare:1;
  UINT8  CriticalWarningTemperature:1;
  UINT8  CriticalWarningReliability:1;
  UINT8  CriticalWarningMediaReadOnly:1;
  UINT8  CriticalWarningVolatileBackup:1;
  UINT8  CriticalWarningPersistentMemoryRegion:1;
  UINT8  CriticalWarningReserved:2;
  //
  //
  //
  UINT16 CompositeTemp;
  UINT8  AvailableSpare;
  UINT8  AvailableSpareThreshold;
  UINT8  PercentageUsed;
  //
  // Endurance Group Critical Warnings has same definition as byte 0 of
  // Endurance Group Information (Log Identifier 09h)
  //
  UINT8  EnduranceGroupCriticalWarning;
  UINT8  Reserved1[25];
  UINT8  DataUnitsRead[16];
  UINT8  DataUnitsWritten[16];
  UINT8  HostReadCommands[16];
  UINT8  HostWriteCommands[16];
  UINT8  ControllerBusyTime[16];
  UINT8  PowerCycles[16];
  UINT8  PowerOnHours[16];
  UINT8  UnsafeShutdowns[16];
  UINT8  MediaAndDataIntegrityErrors[16];
  UINT8  NumberErrorInformationLogEntries[16];
  UINT32 WarningCompositeTemperatureTime;
  UINT32 CriticalCompositeTemperatureTime;
  UINT16 TemperatureSensor[8];
  UINT32 TMT1TransitionCount; /* Thermal Management Temperature 1 Transition Count */
  UINT32 TMT2TransitionCount; /* Thermal Management Temperature 2 Transition Count */
  UINT32 TotalTimeTMT1;       /* Total Time For Thermal Management Temperature 1 */
  UINT32 TotalTimeTMT2;       /* Total Time For Thermal Management Temperature 2 */
  UINT8  Reserved2[280];
} NVME_SMART_HEALTH_INFO_LOG_1_4;

#pragma pack()

STATIC
EFI_STATUS
CreateTextOpCodeWithValueString (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN OUT VOID            *OpCodeHandle,
  IN EFI_QUESTION_ID     QuestionId,
  IN EFI_STRING_ID       PromptStringId OPTIONAL,
  IN EFI_STRING_ID       HelpStringId OPTIONAL,
  IN EFI_STRING_ID       ValueStringId,
  IN CONST CHAR16        *ValueString
  )
{
  EFI_STRING_ID    NewStringId;
  UINT8            *OpCodePtr;

  ASSERT (ValueStringId != 0x0000);
  if ((HiiHandle == NULL) || (OpCodeHandle == NULL) || (ValueString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiHandle/OpCodeHandle/ValueString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  NewStringId = HiiSetString (
                  HiiHandle,
                  ValueStringId,
                  (CONST EFI_STRING) ValueString,
                  NULL // SupportedLanguages
                  );
  if (NewStringId == 0x0000) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // H2O Form Browser Text Mode renders a text op-code without the ability
  // to highlight it by text cursor. Creating action op-code instead is a usual
  // workaround, but note that Metro Display Engine (graphical mode) renders
  // text and action op-codes differently.
  //
  if (FeaturePcdGet (PcdH2OFormBrowserLocalTextDESupported)) {
    if (PromptStringId != STRING_TOKEN (0x0000)) {
      OpCodePtr = HiiCreateActionOpCodeEx (
                    OpCodeHandle,
                    QuestionId,
                    PromptStringId,
                    HelpStringId,
                    ValueStringId,
                    0x00,                 // QuestionFlags,
                    STRING_TOKEN (0x0000) // QuestionConfig
                    );
    } else {
      OpCodePtr = HiiCreateActionOpCode (
                    OpCodeHandle,
                    QuestionId,
                    ValueStringId,
                    HelpStringId,
                    0x00,                 // QuestionFlags,
                    STRING_TOKEN (0x0000) // QuestionConfig
                    );
    }
  } else {
    if (PromptStringId != STRING_TOKEN (0x0000)) {
      OpCodePtr = HiiCreateTextOpCode (
                    OpCodeHandle,
                    PromptStringId,
                    HelpStringId,
                    ValueStringId
                    );
    } else {
      OpCodePtr = HiiCreateTextOpCode (
                    OpCodeHandle,
                    ValueStringId,
                    HelpStringId,
                    STRING_TOKEN (0x0000) // TextTwo
                    );
    }
  }
  if (OpCodePtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreateTextOpCodeWithFormattedValue (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN OUT VOID            *OpCodeHandle,
  IN EFI_QUESTION_ID     QuestionId,
  IN EFI_STRING_ID       PromptStringId OPTIONAL,
  IN EFI_STRING_ID       HelpStringId OPTIONAL,
  IN EFI_STRING_ID       ValueStringId,
  IN CONST CHAR16        *ValueStringFormat,
  ...
  )
{
  VA_LIST     Marker;
  CHAR16      *StringBuffer;
  EFI_STATUS  Status;

  VA_START (Marker, ValueStringFormat);
  StringBuffer = CatVSPrint (
                   NULL,
                   ValueStringFormat,
                   Marker
                   );
  VA_END (Marker);

  if (StringBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = CreateTextOpCodeWithValueString (
             HiiHandle,
             OpCodeHandle,
             QuestionId,
             PromptStringId,
             HelpStringId,
             ValueStringId,
             StringBuffer
             );
  FreePool (StringBuffer);
  return Status;
}

STATIC
CHAR16 *
CreateTemperatureValueString (
  IN UINT16  TemperatureInKelvins
  )
{
  INT32          DegreesCelsius;
  INT32          DegreesFahrenheit;
  CHAR16         *String;

  //
  // 0 degrees Celsius is approximately 273.15 kelvins. Omit decimal fractions
  // because the precision is only one kelvin.
  //
  DegreesCelsius = ((INT32) TemperatureInKelvins) - 273;
  //
  // Fahrenheit scale is rounded to nearest integer.
  // E.g. 1 degree Celsius = 33.8 degrees Fahrenheit -> Round to 34.
  //
  DegreesFahrenheit = (INT32) ((UINT32) TemperatureInKelvins * 9 / 5) - 459;

  String = CatSPrint (
             NULL,
             L"% 4d \xB0" L"C (% 4d \xB0" L"F)",
             (int) DegreesCelsius,
             (int) DegreesFahrenheit
             );
  return String;
}

STATIC
EFI_STATUS
CreateTemperatureTextOpCode (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN OUT VOID            *OpCodeHandle,
  IN EFI_QUESTION_ID     QuestionId,
  IN EFI_STRING_ID       PromptStringId,
  IN EFI_STRING_ID       ValueStringId,
  IN UINT16              TemperatureInKelvins
  )
{
  CHAR16      *TemperatureString;
  EFI_STATUS  Status;

  TemperatureString = CreateTemperatureValueString (TemperatureInKelvins);
  if (TemperatureString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = CreateTextOpCodeWithValueString (
             HiiHandle,
             OpCodeHandle,
             QuestionId,
             PromptStringId,
             STRING_TOKEN (0x0000), // HelpStringId
             ValueStringId,
             TemperatureString
             );
  FreePool (TemperatureString);
  return Status;
}

STATIC
EFI_STATUS
CreateUint128TextOpCode (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN OUT VOID            *OpCodeHandle,
  IN EFI_QUESTION_ID     QuestionId,
  IN EFI_STRING_ID       PromptStringId,
  IN EFI_STRING_ID       ValueStringId,
  IN CONST UINT8         *DataPtr
  )
{
  UINT64        UpperBits;
  EFI_STATUS    Status;

  UpperBits = ((CONST UINT64 *) DataPtr)[1];
  if (UpperBits > 0 || ((CONST UINT64 *) DataPtr)[0] >= 1000000000000ULL) {
    //
    // Tentative string until we figure out a best way to represent a number
    // in the large scale.
    //
    Status = CreateTextOpCodeWithFormattedValue (
               HiiHandle,
               OpCodeHandle,
               QuestionId,
               PromptStringId,
               STRING_TOKEN (0x0000), // HelpStringId
               ValueStringId,
               L"(>= 10^12)"
               );
  } else {
    Status = CreateTextOpCodeWithFormattedValue (
               HiiHandle,
               OpCodeHandle,
               QuestionId,
               PromptStringId,
               STRING_TOKEN (0x0000), // HelpStringId
               ValueStringId,
               L"%,lu",
               *(UINT64 *) DataPtr
               );
  }
  return Status;
}

STATIC
EFI_STATUS
CreateNvmeSmartInfoOpCodes (
  IN OUT EFI_HII_HANDLE                HiiHandle,
  IN OUT VOID                          *OpCodeHandle,
  IN EFI_QUESTION_ID                   QuestionIdStart,
  IN CONST NVME_SMART_HEALTH_INFO_LOG  *SmartHealthInfo
  )
{
  CONST NVME_SMART_HEALTH_INFO_LOG_1_4  *SmartHealthInfoNew;
  BOOLEAN                               Overflow;

  if (OpCodeHandle == NULL || SmartHealthInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (STR_NVME_SMART_HEALTH_INFORMATION), STRING_TOKEN (0x0000), 0, 0);

  (VOID) CreateTextOpCodeWithValueString (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 0),
           STRING_TOKEN (STR_NVME_SMART_CRITICAL_WARNING),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_00),
           ((((UINT8 *) SmartHealthInfo)[0] != 0) ? L"Need Attention!" : L"No warnings")
           );

  (VOID) CreateTemperatureTextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 1),
           STRING_TOKEN (STR_NVME_SMART_COMPOSITE_TEMPERATURE),
           STRING_TOKEN (STR_NVME_SMART_VALUE_01),
           SmartHealthInfo->CompositeTemp
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 2),
           STRING_TOKEN (STR_NVME_SMART_AVAILABLE_SPARE),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_02),
           L"%u%%",
           (unsigned int) SmartHealthInfo->AvailableSpare
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 3),
           STRING_TOKEN (STR_NVME_SMART_AVAILABLE_SPARE_THRESHOLD),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_03),
           L"%u%%",
           (unsigned int) SmartHealthInfo->AvailableSpareThreshold
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 4),
           STRING_TOKEN (STR_NVME_SMART_PERCENTAGE_USED),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_04),
           L"%u%%",
           (unsigned int) SmartHealthInfo->PercentageUsed
           );

  (VOID) CreateTextOpCodeWithValueString (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 5),
           STRING_TOKEN (STR_NVME_ENDURANCE_CRITICAL_WARNING),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_05),
           ((((UINT8 *) SmartHealthInfo)[6] != 0) ? L"Need Attention!" : L"No warnings")
           );

  //
  // Value in "Data Units Read" is reported in "thousands of 512 byte" data
  // units (== 512000 bytes). Convert the unit to megabytes (== 1000000 bytes).
  //
  Overflow =
    ((CONST UINT64 *) SmartHealthInfo->DataUnitsRead)[1] > 0 ||
    ((CONST UINT64 *) SmartHealthInfo->DataUnitsRead)[0] > MAX_UINT64 / 64;
  if (Overflow) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 6),
             STRING_TOKEN (STR_NVME_SMART_DATA_READ),
             STRING_TOKEN (0x0000), // HelpStringId
             STRING_TOKEN (STR_NVME_SMART_VALUE_06),
             L"(overflow)"
             );
  } else {
    (VOID) CreateTextOpCodeWithFormattedValue (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 6),
             STRING_TOKEN (STR_NVME_SMART_DATA_READ),
             STRING_TOKEN (0x0000), // HelpStringId
             STRING_TOKEN (STR_NVME_SMART_VALUE_06),
             L"%,lu MB",
             (((CONST UINT64 *) SmartHealthInfo->DataUnitsRead)[0] * 64 / 125)
             );
  }

  Overflow =
    ((CONST UINT64 *) SmartHealthInfo->DataUnitsWritten)[1] > 0 ||
    ((CONST UINT64 *) SmartHealthInfo->DataUnitsWritten)[0] > MAX_UINT64 / 64;
  if (Overflow) {
    (VOID) CreateTextOpCodeWithFormattedValue (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 7),
             STRING_TOKEN (STR_NVME_SMART_DATA_WRITTEN),
             STRING_TOKEN (0x0000), // HelpStringId
             STRING_TOKEN (STR_NVME_SMART_VALUE_07),
             L"(overflow)"
             );
  } else {
    (VOID) CreateTextOpCodeWithFormattedValue (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 7),
             STRING_TOKEN (STR_NVME_SMART_DATA_WRITTEN),
             STRING_TOKEN (0x0000), // HelpStringId
             STRING_TOKEN (STR_NVME_SMART_VALUE_07),
             L"%,lu MB",
             (((CONST UINT64 *) SmartHealthInfo->DataUnitsWritten)[0] * 64 / 125)
             );
  }

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 8),
           STRING_TOKEN (STR_NVME_SMART_HOST_READ_COMMANDS),
           STRING_TOKEN (STR_NVME_SMART_VALUE_08),
           SmartHealthInfo->HostReadCommands
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 9),
           STRING_TOKEN (STR_NVME_SMART_HOST_WRITE_COMMANDS),
           STRING_TOKEN (STR_NVME_SMART_VALUE_09),
           SmartHealthInfo->HostWriteCommands
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 10),
           STRING_TOKEN (STR_NVME_SMART_CONTROLLER_BUSY_TIME),
           STRING_TOKEN (STR_NVME_SMART_VALUE_10),
           SmartHealthInfo->ControllerBusyTime
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 11),
           STRING_TOKEN (STR_NVME_SMART_POWER_CYCLES),
           STRING_TOKEN (STR_NVME_SMART_VALUE_11),
           SmartHealthInfo->PowerCycles
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 12),
           STRING_TOKEN (STR_NVME_SMART_POWER_ON_HOURS),
           STRING_TOKEN (STR_NVME_SMART_VALUE_12),
           SmartHealthInfo->PowerOnHours
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 13),
           STRING_TOKEN (STR_NVME_SMART_UNSAFE_SHUTDOWNS),
           STRING_TOKEN (STR_NVME_SMART_VALUE_13),
           SmartHealthInfo->UnsafeShutdowns
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 14),
           STRING_TOKEN (STR_NVME_SMART_MEDIA_AND_DATA_INTEGRITY_ERRORS),
           STRING_TOKEN (STR_NVME_SMART_VALUE_14),
           SmartHealthInfo->MediaAndDataIntegrityErrors
           );

  (VOID) CreateUint128TextOpCode (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 15),
           STRING_TOKEN (STR_NVME_NUMBER_ERROR_INFORMATION_LOG_ENTRIES),
           STRING_TOKEN (STR_NVME_SMART_VALUE_15),
           SmartHealthInfo->NumberErrorInformationLogEntries
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 16),
           STRING_TOKEN (STR_NVME_SMART_WARNING_COMPOSITE_TEMPERATURE_TIME),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_16),
           L"%u",
           (unsigned int) SmartHealthInfo->WarningCompositeTemperatureTime
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 17),
           STRING_TOKEN (STR_NVME_SMART_CRITICAL_COMPOSITE_TEMPERATURE_TIME),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_17),
           L"%u",
           (unsigned int) SmartHealthInfo->CriticalCompositeTemperatureTime
           );

  if (SmartHealthInfo->TemperatureSensor[0] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 18),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_1),
             STRING_TOKEN (STR_NVME_SMART_VALUE_18),
             SmartHealthInfo->TemperatureSensor[0]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[1] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 19),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_2),
             STRING_TOKEN (STR_NVME_SMART_VALUE_19),
             SmartHealthInfo->TemperatureSensor[1]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[2] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 20),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_3),
             STRING_TOKEN (STR_NVME_SMART_VALUE_20),
             SmartHealthInfo->TemperatureSensor[2]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[3] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 21),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_4),
             STRING_TOKEN (STR_NVME_SMART_VALUE_21),
             SmartHealthInfo->TemperatureSensor[3]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[4] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 22),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_5),
             STRING_TOKEN (STR_NVME_SMART_VALUE_22),
             SmartHealthInfo->TemperatureSensor[4]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[5] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 23),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_6),
             STRING_TOKEN (STR_NVME_SMART_VALUE_23),
             SmartHealthInfo->TemperatureSensor[5]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[6] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 24),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_7),
             STRING_TOKEN (STR_NVME_SMART_VALUE_24),
             SmartHealthInfo->TemperatureSensor[6]
             );
  }
  if (SmartHealthInfo->TemperatureSensor[7] > 0) {
    (VOID) CreateTemperatureTextOpCode (
             HiiHandle,
             OpCodeHandle,
             (QuestionIdStart + 25),
             STRING_TOKEN (STR_NVME_SMART_TEMPERATURE_SENSOR_8),
             STRING_TOKEN (STR_NVME_SMART_VALUE_25),
             SmartHealthInfo->TemperatureSensor[7]
             );
  }
  
  SmartHealthInfoNew = (CONST NVME_SMART_HEALTH_INFO_LOG_1_4 *) SmartHealthInfo;

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 26),
           STRING_TOKEN (STR_NVME_SMART_TMT1_TRANSITION_COUNT),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_26),
           L"%u",
           (unsigned int) SmartHealthInfoNew->TMT1TransitionCount
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 27),
           STRING_TOKEN (STR_NVME_SMART_TMT2_TRANSITION_COUNT),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_27),
           L"%u",
           (unsigned int) SmartHealthInfoNew->TMT2TransitionCount
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 28),
           STRING_TOKEN (STR_NVME_SMART_TOTAL_TIME_TMT1),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_28),
           L"%u",
           (unsigned int) SmartHealthInfoNew->TotalTimeTMT1
           );

  (VOID) CreateTextOpCodeWithFormattedValue (
           HiiHandle,
           OpCodeHandle,
           (QuestionIdStart + 29),
           STRING_TOKEN (STR_NVME_SMART_TOTAL_TIME_TMT2),
           STRING_TOKEN (0x0000), // HelpStringId
           STRING_TOKEN (STR_NVME_SMART_VALUE_29),
           L"%u",
           (unsigned int) SmartHealthInfoNew->TotalTimeTMT2
           );

  HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (0x0000), STRING_TOKEN (0x0000), 0, 0);

  return EFI_SUCCESS;
}

EFI_STATUS
AddSmartInfoSection (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL        *NvmePassThru;
  NVME_SMART_HEALTH_INFO_LOG                *SmartHealthInfo;

  if (ControllerEntry == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (
                  ControllerEntry->Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID **) &NvmePassThru
                  );
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  SmartHealthInfo = NULL;
  mNvmeInfoData.NvmeInfo->GetSmartHealthInfo (
                            NvmePassThru,
                            0xFFFFFFFF,
                            &SmartHealthInfo
                            );
  if (SmartHealthInfo == NULL) {
    return EFI_ABORTED;
  }

  CreateNvmeSmartInfoOpCodes (
    mNvmeInfoData.HiiHandle,
    OpCodeHandle,
    NVME_SMART_INFORMATION_QUESTION_ID_BASE,
    SmartHealthInfo
    );
  
  FreePool (SmartHealthInfo);

  return EFI_SUCCESS;
}