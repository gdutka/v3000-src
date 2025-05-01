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
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdMpmGetBatteryInfoProtocol.h>
#include <Library/AmdBaseLib.h>
#include <Protocol/AmdCpmTableProtocol/AmdCpmTableProtocol.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MpmLib.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>

//The length and offset defined here need to changed depend on EC
//Customer need to do the change based their design
#define LOCATION_STRING_LENGTH 16
#define MANUFACTURRE_LENGTH 16
#define MANUFACTURE_DATE_LENGTH 4
#define SERIALNUMNER_LENGTH 16
#define ASSERTTAGNUMBER_LENGTH 16
#define DEVICE_NAME_LENGTH 16
#define MODELPARTNUMBER_LENTTH 16
#define REVISION_LEVEL_LENGTH 16

#define OFFSET_ECSPACE_LOCATION_STRING 0
#define OFFSET_ECSPACE_MANUFACTURRE_STRING 0x10
#define OFFSET_ECSPACE_DEVICE_NAME_STRING 0x20

EFI_STATUS
EFIAPI
InstallSmbiosType22 ()
{
  EFI_STATUS Status;
  AMD_CPM_TABLE_PROTOCOL *CpmTableProtocolPtr;
  UINTN StructureSize;
  UINTN TotalSize;
  EFI_SMBIOS_STRING *StrPtr;
  UINTN StringSize;
  SMBIOS_TABLE_TYPE22 *SmbiosTableType22 = NULL;
  UINT8 DesignDeviceCapacityL;
  UINT8 DesignDeviceCapacityH;
  UINT8 DesignVoltageL;
  UINT8 DesignVoltageH;
  UINT8 AcBatteryPresent = 0;
  CHAR8 LocationString[LOCATION_STRING_LENGTH] = {" "};
  CHAR8 Manufacturer[MANUFACTURRE_LENGTH] = {" "};
  CHAR8 ManufactureDate[MANUFACTURE_DATE_LENGTH] = {" "};
  CHAR8 SerialNumber[SERIALNUMNER_LENGTH] = {" "};
  CHAR8 DeviceName[DEVICE_NAME_LENGTH] = {" "};
  UINT8 Index = 0;
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  EFI_SMBIOS_PROTOCOL *Smbios;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22, Locate SmbiosProtocol Status = %r\n", Status));
    return Status;
  }

  StructureSize = sizeof (SMBIOS_TABLE_TYPE22);
  //add the strings size and the NULL (00h) at the end of structure
  TotalSize = StructureSize + sizeof (LocationString) + sizeof (Manufacturer) + sizeof (ManufactureDate) \
              + sizeof (SerialNumber) + sizeof (DeviceName) + 1;

  SmbiosTableType22 = AllocateZeroPool (TotalSize);
  if (SmbiosTableType22 == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosTableType22->Hdr.Type = SMBIOS_TYPE_PORTABLE_BATTERY;
  SmbiosTableType22->Hdr.Length = (UINT8) StructureSize;
  SmbiosTableType22->Hdr.Handle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosTableType22->Location = 1;
  SmbiosTableType22->Manufacturer = 2;
  SmbiosTableType22->ManufactureDate = 3;
  SmbiosTableType22->SerialNumber = 4;
  SmbiosTableType22->DeviceName = 5;
  SmbiosTableType22->DeviceChemistry = 6;
  SmbiosTableType22->MaximumErrorInBatteryData = 0xFF;

  //Read Battery strings information from EC
  Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22: Locate gAmdCpmTableProtocolGuid error\n"));
    return Status;
  }

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);

  //Read AC/Battery present status
  // Bit[6] = 1 if battery is connected
  // Bit[7] = 1 if AC is connected
  TempOffset = 0xEE;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &AcBatteryPresent);
   DEBUG((DEBUG_INFO, "AcBatteryPresent = 0x%x\n", AcBatteryPresent));
  if ((AcBatteryPresent & BIT6) != BIT6) {
    return EFI_UNSUPPORTED;
  }

  //Read Design Capacity
  TempOffset = 0xE9;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &DesignDeviceCapacityH);
  TempOffset = 0xE8;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &DesignDeviceCapacityL);
  SmbiosTableType22->DeviceCapacity = DesignDeviceCapacityL | (DesignDeviceCapacityH << 8);

  //Read Design Voltage
  TempOffset = 0xE3;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &DesignVoltageH);
  TempOffset = 0xE2;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &DesignVoltageL);
  SmbiosTableType22->DesignVoltage = DesignVoltageL | (DesignVoltageH << 8);

  //Read Location String
  for (Index = 0; Index < LOCATION_STRING_LENGTH; Index ++) {
      TempOffset = (UINT8)(OFFSET_ECSPACE_LOCATION_STRING + Index);
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &LocationString[Index]);
  }

  //Read Manufacture String
  for (Index = 0; Index < MANUFACTURRE_LENGTH; Index ++) {
      TempOffset = (UINT8)(OFFSET_ECSPACE_MANUFACTURRE_STRING + Index);
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &Manufacturer[Index]);
  }

  //Read DeviceName String
  for (Index = 0; Index < DEVICE_NAME_LENGTH; Index ++) {
      TempOffset = (UINT8)(OFFSET_ECSPACE_DEVICE_NAME_STRING + Index);
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &DeviceName[Index]);
  }

  //Switch to page1
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 1;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);

  //Read Manufacture Date String
  TempOffset = 0x1;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &ManufactureDate[0]);
  TempOffset = 0x0;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &ManufactureDate[1]);

  //Read Serial Number String
  TempOffset = 0x3;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &SerialNumber[0]);
  TempOffset = 0x2;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &SerialNumber[1]);

  // Append all strings
  StrPtr = (EFI_SMBIOS_STRING*) (((UINT8 *) SmbiosTableType22) + StructureSize);
  StringSize = TotalSize - StructureSize;

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             LocationString
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22, LibAmdInsertSmbiosString Location String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             Manufacturer
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22, LibAmdInsertSmbiosString Manufacturer String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             ManufactureDate
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22, LibAmdInsertSmbiosString ManufactureDate String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             SerialNumber
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22, LibAmdInsertSmbiosString SerialNumber String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             DeviceName
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22, LibAmdInsertSmbiosString DeviceName String error\n"));
  }

  Status = Smbios->Add (Smbios, NULL, &SmbiosTableType22->Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType22);
  return Status;
}

/*
Type39 System power supply function
*/
EFI_STATUS
EFIAPI
InstallSmbiosType39 ()
{
  EFI_STATUS Status;
  AMD_CPM_TABLE_PROTOCOL *CpmTableProtocolPtr;
  UINTN StructureSize;
  UINTN TotalSize;
  EFI_SMBIOS_STRING *StrPtr;
  UINTN StringSize;
  SMBIOS_TABLE_TYPE39 *SmbiosTableType39 = NULL;
  UINT8 AcBatteryPresent;
  CHAR8 LocationString[LOCATION_STRING_LENGTH] = {"UNKNOWN"};
  CHAR8 DeviceName[DEVICE_NAME_LENGTH] = {"UNKNOWN"};
  CHAR8 Manufacturer[MANUFACTURRE_LENGTH] = {"UNKNOWN"};
  CHAR8 SerialNumber[SERIALNUMNER_LENGTH] = {"UNKNOWN"};
  CHAR8 AssetTagNumber[ASSERTTAGNUMBER_LENGTH] = {"UNKNOWN"};
  CHAR8 ModelPartNumber[MODELPARTNUMBER_LENTTH] = {"UNKNOWN"};
  CHAR8 RevisionLevel[REVISION_LEVEL_LENGTH] = {"UNKNOWN"};
  UINT8 Index = 0;
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  EFI_SMBIOS_PROTOCOL *Smbios;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, Locate SmbiosProtocol Status = %r\n", Status));
    return Status;
  }

  StructureSize = sizeof (SMBIOS_TABLE_TYPE39);
  //add the strings size and the NULL (00h) at the end of structure
  TotalSize = StructureSize + sizeof (LocationString) + sizeof (Manufacturer) + sizeof (AssetTagNumber) \
              + sizeof (SerialNumber) + sizeof (DeviceName) + sizeof (ModelPartNumber) + sizeof (RevisionLevel)+ 1;

  SmbiosTableType39 = AllocateZeroPool (TotalSize);
  if (SmbiosTableType39 == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosTableType39->Hdr.Type = SMBIOS_TYPE_SYSTEM_POWER_SUPPLY;
  SmbiosTableType39->Hdr.Length = (UINT8) StructureSize;
  SmbiosTableType39->Hdr.Handle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosTableType39->Location = 1;
  SmbiosTableType39->DeviceName = 2;
  SmbiosTableType39->Manufacturer = 3;
  SmbiosTableType39->SerialNumber = 4;
  SmbiosTableType39->AssetTagNumber = 5;
  SmbiosTableType39->ModelPartNumber = 6;
  SmbiosTableType39->RevisionLevel = 7;

  SmbiosTableType39->MaxPowerCapacity = 0x8000;
  SmbiosTableType39->InputVoltageProbeHandle = 0xFFFF;
  SmbiosTableType39->CoolingDeviceHandle = 0xFFFF;
  SmbiosTableType39->InputCurrentProbeHandle = 0xFFFF;

  //Read Battery strings information from EC
  Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39: Locate gAmdCpmTableProtocolGuid error\n"));
    return Status;
  }

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);

  //Read AC/Battery present status
  // Bit[6] = 1 if battery is connected
  // Bit[7] = 1 if AC is connected
  TempOffset = 0xEE;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &AcBatteryPresent);
  DEBUG((DEBUG_INFO, "AcBatteryPresent = 0x%x\n", AcBatteryPresent));
  if ((AcBatteryPresent & BIT6) == BIT6) {
    SmbiosTableType39->PowerSupplyCharacteristics.PowerSupplyPresent = 1;
    SmbiosTableType39->PowerSupplyCharacteristics.PowerSupplyType = 5;

    //Read Location String
    for (Index = 0; Index < LOCATION_STRING_LENGTH; Index ++) {
      TempOffset = (UINT8)(OFFSET_ECSPACE_LOCATION_STRING + Index);
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &LocationString[Index]);
    }

    //Read Manufacture String
    for (Index = 0; Index < MANUFACTURRE_LENGTH; Index ++) {
      TempOffset = (UINT8)(OFFSET_ECSPACE_MANUFACTURRE_STRING + Index);
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &Manufacturer[Index]);
    }

    //Read DeviceName String
    for (Index = 0; Index < DEVICE_NAME_LENGTH; Index ++) {
      TempOffset = (UINT8)(OFFSET_ECSPACE_DEVICE_NAME_STRING + Index);
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &DeviceName[Index]);
    }

    //Switch to page1
    TempOffset = 0x31;
    EcWriteValue = 0x89;
    CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
    TempOffset = 0x30;
    EcWriteValue = 1;
    CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);


    //Read Serial Number String
    TempOffset = 0x33;
    CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &SerialNumber[0]);
    TempOffset = 0x32;
    CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &SerialNumber[1]);
  }

  if ((AcBatteryPresent & BIT7)) {
    SmbiosTableType39->PowerSupplyCharacteristics.PowerSupplyPresent = 1;
  }
  // Append all strings
  StrPtr = (EFI_SMBIOS_STRING*) (((UINT8 *) SmbiosTableType39) + StructureSize);
  StringSize = TotalSize - StructureSize;

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             LocationString
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString Location String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             DeviceName
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString DeviceName String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             Manufacturer
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString Manufacturer String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             SerialNumber
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString SerialNumber String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             AssetTagNumber
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString AssetTagNumber String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             ModelPartNumber
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString ModelPartNumber String error\n"));
  }

  Status = LibAmdInsertSmbiosString (
             &StrPtr,
             &StringSize,
             RevisionLevel
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType39, LibAmdInsertSmbiosString RevisionLevel String error\n"));
  }

  Status = Smbios->Add (Smbios, NULL, &SmbiosTableType39->Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType39);
  return Status;
}

EFI_STATUS
EFIAPI
GetBatteryInfo (OUT BATTERY_INFO_T *BatteryInfo)
{
  EFI_STATUS Status;
  AMD_CPM_TABLE_PROTOCOL *CpmTableProtocolPtr;
  UINT8 TempOffset = 0;
  UINT8 EcWriteValue = 0;
  UINT8 AcBatteryPresent = 0;
  CHAR8 *BatteryVersion = "Version 1.0";
  CHAR8 *OtherChemistryDescription = "L-I-T-H";

  Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallSmbiosType22: Locate gAmdCpmTableProtocolGuid error\n"));
    return Status;
  }

  //Read the battery information from EC
  /*
  Offset (0xD0),  // ACPI_BATTERY_BASE
  MERL, 8,        // LSB of Measurement Accuracy  - RO for BIX
  MERH, 8,        // MSB of Measurement Accuracy  - RO for BIX

  BLOL, 8,        // LSB of battery Low threshold - RW
  BCRH, 8,        // MSB of battery Low threshold - RW

  BCRL, 8,        // LSB of battery Critical threshold - RW
  BLOH, 8,        // MSB of battery Critical threshold - RW

  BTPL, 8,        // LSB of BTP (Battery Trip Point) - RW
  BTPH, 8,        // MSB of BTP (Battery Trip Point) - RW

  BTML, 8,        // LSB of battery tempurature
  BTMH, 8,        // MSB of battery tempurature

  BMEL, 8,        // LSB of Battery max error Counter
  BMEH, 8,        // MSB of Battery max error Counter

  BCCL, 8,        // LSB of battery cycle counter
  BCCH, 8,        // MSB of battery cycle counter

  BTCL, 8,        // LSB of battery terminal current (0x0A)
  BTCH, 8,        // MSB of battery terminal current

  BTVL, 8,        // LSB of battery terminal voltage (0x09)
  BTVH, 8,        // MSB of battery terminal voltage

  DEVL, 8,        // LSB of battery design voltage() (0x19)
  DEVH, 8,        // MSB of battery design voltage()

  CAPL, 8,        // LSB of battery remaining capacity (0x0F)
  CAPH, 8,        // MSB of battery remaining capacity

  LFCL, 8,        // LSB of fully charged battery capacity (0x10)
  LFCH, 8,        // MSB of fully charged battery capacity

  DECL, 8,        // LSB of battery design capacity() (0x18)
  DECH, 8,        // MSB of battery design capacity()

  BCTR, 8,        // _DSM F1 charge throttle (0 to 100)

  BCON, 8,        // Indiactes how many batteries are supported by the system
  BTST, 8,        // Battery selector - to select other batteries is more than one battery are supported

  BATS, 8,        // Battery status
                  // Bit[0] = 1 if battery is charging or 0 for discharging
                  // Bit[7] = 1 if battery was tripped. Write 1 to clear.
                  //          _Q3E should be fired until this bit is cleared.
  CHSH, 8,        // Charger status
                  // Bit[6] = 1 if battery is connected
                  // Bit[7] = 1 if AC is connected
  BALT, 8,        // Bit[7:0] = Each bit stands for one battery.
                  //            An one indicates the battery has alert. Read to clear
  */
  UINT8 Offset;
  UINT8 BatteryLowThresholdL;
  UINT8 BatteryLowThresholdH;
  UINT8 BatteryCriticalThresholdL;
  UINT8 BatteryCriticalThresholdH;
  UINT8 BatteryCycleCounterL;
  UINT8 BatteryCycleCounterH;
  UINT8 BatteryRemainingCapacityL;
  UINT8 BatteryRemainingCapacityH;
  UINT8 BatteryChargedCapacityL;
  UINT8 BatteryChargedCapacityH;
  UINT8 BatteryDesignCapacityL;
  UINT8 BatteryDesignCapacityH;
  UINT8 BatteryStatusEc; //bit0: 1 charging, 0 no charging

  //Switch to page0
  TempOffset = 0x31;
  EcWriteValue = 0x89;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);
  TempOffset = 0x30;
  EcWriteValue = 0;
  CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &EcWriteValue);

  //Read AC/Battery present status
  // Bit[6] = 1 if battery is connected
  // Bit[7] = 1 if AC is connected
  TempOffset = 0xEE;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &TempOffset, &AcBatteryPresent);
   DEBUG((DEBUG_INFO, "AcBatteryPresent = 0x%x\n", AcBatteryPresent));
  if ((AcBatteryPresent & BIT6) != BIT6) {
    return EFI_UNSUPPORTED;
  }
  Offset = 0xD3;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryLowThresholdH);
  Offset = 0xD2;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryLowThresholdL);
  Offset = 0xD5;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryCriticalThresholdH);
  Offset = 0xD4;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryCriticalThresholdL);
  Offset = 0xDD;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryCycleCounterH);
  Offset = 0xDC;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryCycleCounterL);
  Offset = 0xE5;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryRemainingCapacityH);
  Offset = 0xE4;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryRemainingCapacityL);
  Offset = 0xE7;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryChargedCapacityH);
  Offset = 0xE6;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryChargedCapacityL);
  Offset = 0xE9;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryDesignCapacityH);
  Offset = 0xE8;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryDesignCapacityL);
  Offset = 0xED;
  CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &BatteryStatusEc);

  UINT16 BatteryLowThreshold = (BatteryLowThresholdH << 8) | BatteryLowThresholdL;
  UINT16 BatteryRemainingCapacity = (BatteryRemainingCapacityH << 8) | BatteryRemainingCapacityL;
  UINT16 BatteryCriticalThreshold =  (BatteryCriticalThresholdH << 8) | BatteryCriticalThresholdL;
  UINT16 BatteryChargedCapacity = (BatteryChargedCapacityH << 8) | BatteryChargedCapacityL;

  /*
    Battery Status:
    1-  Unknown
    2-  Fully Charged
    3-  Low
    4-  Critical
    5-  Charging
    6-  Charging and High
    7-  Charging and Low
    8-  Charging and Critical
    9-  Undefined
    10-  Partially Charged
    11-  Learning
    12-  Overcharged
    13-  DMTF Reserved
    32768-65535 - Vendor Specific
 */
  if (BatteryLowThreshold > ((BatteryRemainingCapacity / BatteryChargedCapacity))) {
    BatteryInfo->BatteryStatus = 3;
  } else if (BatteryCriticalThreshold > ((BatteryRemainingCapacity / BatteryChargedCapacity))) {
    BatteryInfo->BatteryStatus = 4;
  } else if ((BatteryStatusEc & 1) == 1) {
    BatteryInfo->BatteryStatus = 5;
  } else if (((BatteryStatusEc & 1) == 1) && (BatteryLowThreshold > ((BatteryRemainingCapacity / BatteryChargedCapacity)))){
    BatteryInfo->BatteryStatus = 7;
  } else if (((BatteryStatusEc & 1) == 1) && (BatteryCriticalThreshold > ((BatteryRemainingCapacity / BatteryChargedCapacity)))) {
    BatteryInfo->BatteryStatus = 8;
  }

  BatteryInfo->DesignCapacity = (BatteryDesignCapacityH << 8) | (BatteryDesignCapacityL);
  BatteryInfo->TimeOnBattery = 100;
  BatteryInfo->EstimatedRunTime = ((BatteryRemainingCapacityH << 8) | BatteryRemainingCapacityL) / ((BatteryChargedCapacityH << 8) | BatteryChargedCapacityL);
  BatteryInfo->EstimatedChargeRemaining = 10;
  BatteryInfo->Chemistry = 0;
  AsciiStrCpyS(BatteryInfo->SmartBatteryVersion, AsciiStrLen(BatteryVersion) + 1, BatteryVersion);
  BatteryInfo->TimeToFullCharge = 0;
  BatteryInfo->ExpectedLife = 0;
  BatteryInfo->MaxRechargeTime = 0;
  BatteryInfo->RechargeCount = (BatteryCycleCounterH << 8) | BatteryCycleCounterL;
  BatteryInfo->MaxRechargeCount = 0;
  BatteryInfo->RemainingCapacity = BatteryRemainingCapacity;
  AsciiStrCpyS(BatteryInfo->OtherChemistryDescription, AsciiStrLen(OtherChemistryDescription) + 1, OtherChemistryDescription);
  BatteryInfo->ChargingStatus = BatteryStatusEc & 0x1;
  BatteryInfo->HealthPercent = 100;
  return EFI_SUCCESS;
}

AMD_MPM_GET_BATTERY_INFO_PROTOCOL   mAmdGetBatteryInfoProtocol = {
  GetBatteryInfo
};

EFI_STATUS
EFIAPI
BatteryInfoDxeEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        mHandle              = NULL;

  DEBUG ((DEBUG_INFO, "%a: enter\n", __FUNCTION__));
  if ((PcdGet32 (PcdAmdEnvironment98) & BIT16)) { //return if EC disabled
    return EFI_UNSUPPORTED;
  }

  // Check MPM Enable, only MPM need this information, return if MPM is disabled
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  Status = InstallSmbiosType22 ();
  DEBUG((DEBUG_INFO, "InstallSmbiosType22 Status = %r\n", Status));

  Status = InstallSmbiosType39 ();
  DEBUG((DEBUG_INFO, "InstallSmbiosType39 Status = %r\n", Status));

  Status = gBS->InstallProtocolInterface (
                  &mHandle,
                  &gAmdMpmGetBatteryInfoProtocol,
                  EFI_NATIVE_INTERFACE,
                  &mAmdGetBatteryInfoProtocol
                  );
  DEBUG ((DEBUG_INFO, "Install GetBatteryInfo Protocol Status = %r\n", Status));
  return Status;
}

