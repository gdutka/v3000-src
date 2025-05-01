/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#include "SmbiosTypeAdd.h"

EFI_SMBIOS_PROTOCOL *mEfiSmbiosProtocol = NULL;

/**
  This structure describes the attributes for a cooling device in the system.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
AddSmbiosType27()
{
  SMBIOS_TABLE_TYPE27     *Type27Data;
  EFI_SMBIOS_HANDLE       TemperatureProbeHandle;
  EFI_SMBIOS_TABLE_HEADER *TemperatureRecord;
  EFI_SMBIOS_TYPE         TemperatureType;
  UINTN                   TableTotalSize;
  EFI_STATUS              Status;

  //
  // Allocate Type27 Smbios memory buffer.
  //
  TableTotalSize = (sizeof(SMBIOS_TABLE_TYPE27) + 2);  // Additional null (00h), End of strings
  Type27Data     = NULL;
  Type27Data     = AllocateZeroPool(TableTotalSize);
  if (Type27Data == NULL) {
    DEBUG ((DEBUG_INFO, "Allocate Type27 memory buffer fail.\n"));
    return Status = EFI_OUT_OF_RESOURCES;
  }

  //
  // SMBIOS Type27
  //
  Type27Data->Hdr.Type   = EFI_SMBIOS_TYPE_COOLING_DEVICE;
  Type27Data->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE27);
  Type27Data->Hdr.Handle = SMBIOS_HANDLE_PI_RESERVED;

  //
  // Get Type28 Handle to TemperatureProbeHandle
  //
  TemperatureProbeHandle = SMBIOS_HANDLE_PI_RESERVED;
  TemperatureType        = EFI_SMBIOS_TYPE_TEMPERATURE_PROBE;
  TemperatureRecord      = NULL;
  Status = mEfiSmbiosProtocol->GetNext(
                            mEfiSmbiosProtocol,
                            &TemperatureProbeHandle,
                            &TemperatureType,
                            &TemperatureRecord,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type27 get Type28 data fail.\n"));
    goto Error;
  }

  Type27Data->TemperatureProbeHandle                  = TemperatureProbeHandle;
  Type27Data->DeviceTypeAndStatus.CoolingDevice       = 0;
  Type27Data->DeviceTypeAndStatus.CoolingDeviceStatus = 1;
  Type27Data->CoolingUnitGroup                        = 0x01;
  Type27Data->OEMDefined                              = 0x00000000;
  Type27Data->NominalSpeed                            = 0x0000;
  Type27Data->Description                             = 0;

  Status = mEfiSmbiosProtocol->Add(
                            mEfiSmbiosProtocol,
                            NULL,
                            &Type27Data->Hdr.Handle,
                            (EFI_SMBIOS_TABLE_HEADER *)Type27Data
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"SMBIOS Type27 add fail. Status = %r\n", Status));
  }

Error:
  gBS->FreePool(Type27Data);
  return Status;
}

/**
  This structure describes the attributes for a temperature probe in the system.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
AddSmbiosType28()
{
  SMBIOS_TABLE_TYPE28 *Type28Data;
  UINTN               TableTotalSize;
  EFI_STATUS          Status;

  //
  // Allocate Type28 Smbios memory buffer.
  //
  TableTotalSize = (sizeof(SMBIOS_TABLE_TYPE28) + 2);  // Additional null (00h), End of strings
  Type28Data     = NULL;
  Type28Data     = AllocateZeroPool(TableTotalSize);
  if (Type28Data == NULL) {
    DEBUG ((DEBUG_INFO, "Allocate Type28 memory buffer fail.\n"));
    return Status = EFI_OUT_OF_RESOURCES;
  }

  //
  // SMBIOS Type28
  //
  Type28Data->Hdr.Type                                 = EFI_SMBIOS_TYPE_TEMPERATURE_PROBE;
  Type28Data->Hdr.Length                               = sizeof(SMBIOS_TABLE_TYPE28);
  Type28Data->Hdr.Handle                               = SMBIOS_HANDLE_PI_RESERVED;
  Type28Data->Description                              = 0;
  Type28Data->LocationAndStatus.TemperatureProbeSite   = 0;
  Type28Data->LocationAndStatus.TemperatureProbeStatus = 0;
  Type28Data->MaximumValue                             = 0x0000;
  Type28Data->MinimumValue                             = 0x0000;
  Type28Data->Resolution                               = 0x0000;
  Type28Data->Tolerance                                = 0x0000;
  Type28Data->Accuracy                                 = 0x0000;
  Type28Data->OEMDefined                               = 0x00000000;
  Type28Data->NominalValue                             = 0x0000;

  Status = mEfiSmbiosProtocol->Add(
                            mEfiSmbiosProtocol,
                            NULL,
                            &Type28Data->Hdr.Handle,
                            (EFI_SMBIOS_TABLE_HEADER *)Type28Data
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type28 add fail. Status = %r\n", Status));
  }

  gBS->FreePool(Type28Data);
  return Status;
}

/**
  The information in this structure defines the attributes of a Management Device.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
AddSmbiosType34()
{
  SMBIOS_TABLE_TYPE34 *Type34Data;
  UINTN               TableTotalSize;
  EFI_STATUS          Status;

  //
  // Allocate Type34 Smbios memory buffer.
  //
  TableTotalSize = (sizeof(SMBIOS_TABLE_TYPE34) + 2);  // Additional null (00h), End of strings
  Type34Data     = NULL;
  Type34Data     = AllocateZeroPool(TableTotalSize);
  if (Type34Data == NULL) {
    DEBUG ((DEBUG_INFO, "Allocate Type34 memory buffer fail.\n"));
    return Status = EFI_OUT_OF_RESOURCES;
  }

  //
  // SMBIOS Type34
  //
  Type34Data->Hdr.Type    = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE;
  Type34Data->Hdr.Length  = sizeof(SMBIOS_TABLE_TYPE34);
  Type34Data->Hdr.Handle  = SMBIOS_HANDLE_PI_RESERVED;
  Type34Data->Description = 0;
  Type34Data->Type        = 0x01;
  Type34Data->Address     = 0x00000000;
  Type34Data->AddressType = 0x01;

  Status = mEfiSmbiosProtocol->Add(
                            mEfiSmbiosProtocol,
                            NULL,
                            &Type34Data->Hdr.Handle,
                            (EFI_SMBIOS_TABLE_HEADER *)Type34Data
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type34 add fail. Status = %r\n", Status));
  }

  gBS->FreePool(Type34Data);
  return Status;
}

/**
  This structure associates a cooling device or environmental probe with structures that define 
  the controlling hardware device and (optionally) the component’s thresholds.

  @param [in] SmBiosTypeHandleToComponentHandle     Get Smbios Handle to ComponentHandle.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
AddSmbiosType35(
  EFI_SMBIOS_TYPE SmBiosTypeHandleToComponentHandle
)
{
  SMBIOS_TABLE_TYPE35     *Type35Data;
  EFI_SMBIOS_HANDLE       ManagementDeviceHandle, ComponentHandle, ThresholdHandle;
  EFI_SMBIOS_TABLE_HEADER *SmBiosHandleRecord;
  EFI_SMBIOS_TYPE         SmBiosHandleType;
  UINTN                   TableTotalSize;
  EFI_STATUS              Status;

  //
  // Allocate Type35 Smbios memory buffer.
  //
  TableTotalSize = (sizeof(SMBIOS_TABLE_TYPE35) + 2);  // Additional null (00h), End of strings
  Type35Data     = NULL;
  Type35Data     = AllocateZeroPool(TableTotalSize);
  if (Type35Data == NULL) {
    DEBUG ((DEBUG_INFO, "Allocate Type35 memory buffer fail.\n"));
    return Status = EFI_OUT_OF_RESOURCES;
  }

  //
  // SMBIOS Type35
  //
  Type35Data->Hdr.Type    = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE_COMPONENT;
  Type35Data->Hdr.Length  = sizeof(SMBIOS_TABLE_TYPE35);
  Type35Data->Hdr.Handle  = SMBIOS_HANDLE_PI_RESERVED;
  Type35Data->Description = 0;

  //
  // Get Type34 Handle to ManagementDeviceHandle
  //
  ManagementDeviceHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmBiosHandleType       = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE;
  SmBiosHandleRecord     = NULL;
  Status = mEfiSmbiosProtocol->GetNext(
                            mEfiSmbiosProtocol,
                            &ManagementDeviceHandle,
                            &SmBiosHandleType,
                            &SmBiosHandleRecord,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type35 get Type34 record fail.\n"));
    goto Error;
  }

  Type35Data->ManagementDeviceHandle = ManagementDeviceHandle;

  //
  // Get Type27/Type28 Handle to ComponentHandle
  //
  ComponentHandle    = SMBIOS_HANDLE_PI_RESERVED;
  SmBiosHandleType   = SmBiosTypeHandleToComponentHandle;
  SmBiosHandleRecord = NULL;
  Status = mEfiSmbiosProtocol->GetNext(
                            mEfiSmbiosProtocol,
                            &ComponentHandle,
                            &SmBiosHandleType,
                            &SmBiosHandleRecord,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type35 get Type27/Type28 Record fail.\n"));
    DEBUG ((DEBUG_INFO, "Fail Type: 0x%x\n", SmBiosTypeHandleToComponentHandle));
    goto Error;
  }

  Type35Data->ComponentHandle = ComponentHandle;

  //
  // Get Type36 Handle to ThresholdHandle
  //
  ThresholdHandle    = SMBIOS_HANDLE_PI_RESERVED;
  SmBiosHandleType   = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE_THRESHOLD_DATA;
  SmBiosHandleRecord = NULL;
  Status = mEfiSmbiosProtocol->GetNext(
                            mEfiSmbiosProtocol,
                            &ThresholdHandle,
                            &SmBiosHandleType,
                            &SmBiosHandleRecord,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type35 get Type36 record fail.\n"));
    goto Error;
  }

  Type35Data->ThresholdHandle = ThresholdHandle;

  Status = mEfiSmbiosProtocol->Add(
                            mEfiSmbiosProtocol,
                            NULL,
                            &Type35Data->Hdr.Handle,
                            (EFI_SMBIOS_TABLE_HEADER *)Type35Data
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type35 add fail. Status = %r\n", Status));
  }

Error:
  gBS->FreePool(Type35Data);
  return Status;
}

/**
  The information in this structure defines threshold information for a component 
  (probe or cooling-unit) contained within a Management Device.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
AddSmbiosType36()
{
  SMBIOS_TABLE_TYPE36 *Type36Data;
  UINTN               TableTotalSize;
  EFI_STATUS          Status;

  //
  // Allocate Type36 Smbios memory buffer.
  //
  TableTotalSize = (sizeof(SMBIOS_TABLE_TYPE36) + 2);  // Additional null (00h), End of strings
  Type36Data     = NULL;
  Type36Data     = AllocateZeroPool(TableTotalSize);
  if (Type36Data == NULL) {
    DEBUG ((DEBUG_INFO, "Allocate Type36 memory buffer fail.\n"));
    return Status = EFI_OUT_OF_RESOURCES;
  }

  //
  // SMBIOS Type36
  //
  Type36Data->Hdr.Type                     = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE_THRESHOLD_DATA;
  Type36Data->Hdr.Length                   = sizeof(SMBIOS_TABLE_TYPE36);
  Type36Data->Hdr.Handle                   = SMBIOS_HANDLE_PI_RESERVED;
  Type36Data->LowerThresholdNonCritical    = 0x0000;
  Type36Data->UpperThresholdNonCritical    = 0x0000;
  Type36Data->LowerThresholdCritical       = 0x0000;
  Type36Data->UpperThresholdCritical       = 0x0000;
  Type36Data->LowerThresholdNonRecoverable = 0x0000;
  Type36Data->UpperThresholdNonRecoverable = 0x0000;

  Status = mEfiSmbiosProtocol->Add(
                            mEfiSmbiosProtocol,
                            NULL,
                            &Type36Data->Hdr.Handle,
                            (EFI_SMBIOS_TABLE_HEADER *)Type36Data
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type36 add fail. Status = %r\n", Status));
  }

  gBS->FreePool(Type36Data);
  return Status;
}

/**
  RealManage will got Sensor information from Type255

  @param [in] SmBiosTypeHandleToSensorHandleType     Get Smbios Handle to SensorHandle.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
AddSmbiosType255(
  EFI_SMBIOS_TYPE SmBiosTypeHandleToSensorHandleType
)
{
  SMBIOS_TABLE_TYPE255    *Type255Data;
  EFI_SMBIOS_TABLE_HEADER *SensorRecord;
  EFI_SMBIOS_HANDLE       SensorHandle;
  EFI_SMBIOS_TYPE         SmBiosHandleType;
  UINTN                   TableTotalSize;
  EFI_STATUS              Status;

  //
  // Allocate Type255 Smbios memory buffer.
  //
  TableTotalSize = (sizeof(SMBIOS_TABLE_TYPE255) + 2);  // Additional null (00h), End of strings
  Type255Data    = NULL;
  Type255Data    = AllocateZeroPool(TableTotalSize);
  if (Type255Data == NULL) {
    DEBUG ((DEBUG_INFO, "Allocate memory buffer fail.\n"));
    return Status = EFI_OUT_OF_RESOURCES;
  }

  //
  // SMBIOS Type255
  //
  Type255Data->Hdr.Type   = 255;
  Type255Data->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE255);
  Type255Data->Hdr.Handle = SMBIOS_HANDLE_PI_RESERVED;

  //
  // Get Type27/Type28 Handle to SensorHandle
  //
  SensorHandle     = SMBIOS_HANDLE_PI_RESERVED;
  SmBiosHandleType = SmBiosTypeHandleToSensorHandleType;
  SensorRecord     = NULL;
  Status = mEfiSmbiosProtocol->GetNext(
                            mEfiSmbiosProtocol,
                            &SensorHandle,
                            &SmBiosHandleType,
                            &SensorRecord,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SMBIOS Type255 get Type27/Type28 Record fail.\n"));
    DEBUG ((DEBUG_INFO, "Fail Type: 0x%x\n", SmBiosTypeHandleToSensorHandleType));
    goto Error;
  }

  Type255Data->SensorHandle = SensorHandle;
  Status = mEfiSmbiosProtocol->Add(
                            mEfiSmbiosProtocol,
                            NULL,
                            &Type255Data->Hdr.Handle,
                            (EFI_SMBIOS_TABLE_HEADER *)Type255Data
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"SMBIOS Type: 255 add fail. Status = %r\n", Status));
  }

Error:
  gBS->FreePool(Type255Data);
  return Status;
}

EFI_STATUS
EFIAPI
SmbiosTypeAddEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  COMMON_SETUP_OPTIONS  CommonSetupOptions;

  DEBUG ((DEBUG_INFO, "SmbiosTypeAddEntry Entry.\n"));

  Status = GetCommonSetupOptions (&CommonSetupOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"[SmbiosTypeAddEntry] Get Dash variable fail.\n"));
    return Status;
  }

  if (CommonSetupOptions.DashOption != 1) {
    DEBUG ((DEBUG_INFO,"[SmbiosTypeAddEntry] Dash variable is Disabled.\n"));
    return Status;
  }

  //
  // Locate SMBIOS Protocol instance
  //
  Status = gBS->LocateProtocol(
              &gEfiSmbiosProtocolGuid,
              NULL,
              &mEfiSmbiosProtocol
              );

  if(EFI_ERROR(Status)){
    DEBUG ((DEBUG_INFO, "Locate EFI SMBIOS Protocol fail.\n"));
    return Status;
  }

  AddSmbiosType28();      // Need to be add before Type 27
  AddSmbiosType27();

  AddSmbiosType34();

  AddSmbiosType36();      // Need to be add before Type 35
  AddSmbiosType35(EFI_SMBIOS_TYPE_COOLING_DEVICE);
  AddSmbiosType35(EFI_SMBIOS_TYPE_TEMPERATURE_PROBE);

  AddSmbiosType255(EFI_SMBIOS_TYPE_COOLING_DEVICE);
  AddSmbiosType255(EFI_SMBIOS_TYPE_TEMPERATURE_PROBE);

  DEBUG ((DEBUG_INFO, "SmbiosTypeAddEntry Exit.\n"));
  return EFI_SUCCESS;
}