/** @file
  Implementation of SataDriveInfoLib.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/BlockIoCapacityLib.h>
#include <Library/SataDriveInfoLib.h>

#include <Protocol/H2OSataDriveInfo.h>

#include <IndustryStandard/Atapi.h>

typedef struct {
  UINT8   AttributeId;
  CHAR16  *AttributeName;
} SMART_ATTRIBUTE_TABLE;

STATIC SMART_ATTRIBUTE_TABLE mSmartAttributeTable[] = {
  { 0x01, L"Read Error Rate" },
  { 0x02, L"Throughput Performance" },
  { 0x03, L"Spin-Up Time" },
  { 0x04, L"Start/Stop Count" },
  { 0x05, L"Reallocated Sector Count" },
  { 0x06, L"Read Channel Margin" },
  { 0x07, L"Seek Error Rate" },
  { 0x08, L"Seek Time Performance" },
  { 0x09, L"Power-On Hours" },
  { 0x0A, L"Spin Retry Count" },
  { 0x0B, L"Calibration Retry Count" },
  { 0x0C, L"Power Cycle Count" },
  { 0x0D, L"Soft Read Error Rate" },
  { 0x16, L"Current Helium Level" },
  { 0xAA, L"Available Reserved Space" },
  { 0xAB, L"SSD Program Fail Count" },
  { 0xAC, L"SSD Erase Fail Count" },
  { 0xAD, L"SSD Wear Leveling Count" },
  { 0xAE, L"Unexpected Power Loss Count" },
  { 0xAF, L"Power Loss Protection Failure" },
  { 0xB0, L"Erase Fail Count" },
  { 0xB1, L"Wear Range Delta" },
  { 0xB3, L"Used Reserved Block Count Total" },
  { 0xB4, L"Unused Reserved Block Count Total" },
  { 0xB5, L"Program Fail Count Total/Non-4K Aligned Access Count" },
  { 0xB6, L"Erase Fail Count" },
  { 0xB7, L"SATA Downshift Error Count/Runtime Bad Block" },
  { 0xB8, L"End-to-End Error/IOEDC" },
  { 0xB9, L"Head Stability" },
  { 0xBA, L"Induced Op-Vibration Detection" },
  { 0xBB, L"Reported Uncorrectable Errors" },
  { 0xBC, L"Command Timeout" },
  { 0xBD, L"High Fly Writes" },
  { 0xBE, L"Temperature Difference/Airflow Temperature" },
  { 0xBF, L"G-Sense Error Rate" },
  { 0xC0, L"Power-Off Retract Count/Unsafe Shutdown Count" },
  { 0xC1, L"Load Cycle Count" },
  { 0xC2, L"Temperature Celsius" },
  { 0xC3, L"Hardware ECC Recovered" },
  { 0xC4, L"Reallocation Event Count" },
  { 0xC5, L"Current Pending Sector Count" },
  { 0xC6, L"(Offline) Uncorrectable Sector Count" },
  { 0xC7, L"UltraDMA CRC Error Count" },
  { 0xC8, L"Multi-Zone Error Rate/Write Error Rate" },
  { 0xC9, L"Soft Read Error Rate/TA Counter Detected" },
  { 0xCA, L"Data Address Mark Errors/TA Counter Increased" },
  { 0xCB, L"Run Out Cancel" },
  { 0xCC, L"Soft ECC Correction" },
  { 0xCD, L"Thermal Asperity Rate" },
  { 0xCE, L"Flying Height" },
  { 0xCF, L"Spin High Current" },
  { 0xD0, L"Spin Buzz" },
  { 0xD1, L"Offline Seek Performnce" },
  { 0xD2, L"Vibration During Write" },
  { 0xD3, L"Vibration During Write" },
  { 0xD4, L"Shock During Write" },
  { 0xDC, L"Disk Shift" },
  { 0xDD, L"G-Sense Error Rate" },
  { 0xDE, L"Loaded Hours" },
  { 0xDF, L"Load/Unload Retry Count" },
  { 0xE0, L"Load Friction" },
  { 0xE1, L"Load/Unload Cycle Count" },
  { 0xE2, L"Load-In Time" },
  { 0xE3, L"Torque Amplification Count" },
  { 0xE4, L"Power-off Retract Cycle" },
  { 0xE6, L"GMR Head Amplitude/ Drive Life Protection Status" },
  { 0xE7, L"Life Left/Temperature" },
  { 0xE8, L"Endurace Remaining/Available Reserved Space" },
  { 0xE9, L"Media Wearout Indicator" },
  { 0xEB, L"Good Block Count AND System (Free) Block Count" },
  { 0xF0, L"Head Flying Hours" },
  { 0xF1, L"Total LBAs Written" },
  { 0xF2, L"Total LBAs Read" },
  { 0xF3, L"Total LBAs Written Expanded" },
  { 0xF4, L"Total LBAs Read Expanded" },
  { 0xF9, L"NAND Writes (1GiB)" },
  { 0xFA, L"Read Error Retry Rate" },
  { 0xFB, L"Minimum Spares Remaining" },
  { 0xFC, L"Newly Added Bad Flash Block" },
  { 0xFE, L"Free Fall Protection" },
};

/**
  This function converts an ATA String to an standard C string.

  @param[in]  AtaString   The pointer to the ATA String.
  @param[in]  Length      Length in bytes of the ATA String. Length must be even;
                          otherwise ASSERT().
  @param[in]  AsciiStr    The pointer to the output string buffer.
                          Length in bytes of this buffer must be greater than the
                          ATA String to place the NULL terminator.

**/
STATIC
VOID
ConvertAtaStringToAsciiStr (
  IN      UINT8   *AtaString,
  IN      UINTN   Length,
  IN OUT  CHAR8   **AsciiStrPtr
  )
{
  UINTN   Index;
  CHAR8   *Ptr;
  CHAR8   *AsciiStr;

  if (AtaString == NULL || Length == 0 || AsciiStrPtr == NULL) {
    return;
  }

  ASSERT ((Length & 1) == 0);

  AsciiStr = *AsciiStrPtr;

  for (Index = 0; Index < Length; Index += 2) {
    *(UINT16 *)(AsciiStr + Index) = SwapBytes16 (*(UINT16 *)(AtaString + Index));
  }

  Ptr = AsciiStr + AsciiStrnLenS (AsciiStr, Length);
  while (Ptr > AsciiStr && Ptr[-1] == ' ') {
    Ptr--;
  }
  Ptr = '\0';

  while (*AsciiStr == ' ') {
    AsciiStr++;
  }

  *AsciiStrPtr = AsciiStr;
}

/**
  Helper function to retrieve Model Name from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Model Name.

**/
CHAR16 *
RetrieveModelNameFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  )
{
  CHAR8   AsciiModelName[MAX_MODEL_NAME_LEN + 1] = {0};
  CHAR8   *Ptr;
  CHAR16  ModelName[MAX_MODEL_NAME_LEN + 1] = {0};

  if (IdentifyData == NULL) {
    return NULL;
  }

  ASSERT (OFFSET_OF (ATA_IDENTIFY_DATA, ModelName) == OFFSET_OF (ATAPI_IDENTIFY_DATA, ModelName));

  Ptr = AsciiModelName;
  ConvertAtaStringToAsciiStr ((UINT8 *) IdentifyData->AtaData.ModelName, MAX_MODEL_NAME_LEN, &Ptr);
  UnicodeSPrintAsciiFormat (ModelName, sizeof (ModelName), "%a", Ptr);

  return (CHAR16 *) AllocateCopyPool (StrnSizeS (ModelName, MAX_MODEL_NAME_LEN + 1), ModelName);
}

/**
  Helper function to retrieve Serial Number from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Model Name.

**/
CHAR16 *
RetrieveSerialNumberFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  )
{
  CHAR8   AsciiSerialNumber[MAX_SERIAL_NUMBER_LEN + 1] = {0};
  CHAR8   *Ptr;
  CHAR16  SerialNumber[MAX_SERIAL_NUMBER_LEN + 1] = {0};

  if (IdentifyData == NULL) {
    return NULL;
  }

  ASSERT (OFFSET_OF (ATA_IDENTIFY_DATA, SerialNo) == OFFSET_OF (ATAPI_IDENTIFY_DATA, SerialNo));

  Ptr = AsciiSerialNumber;
  ConvertAtaStringToAsciiStr ((UINT8 *) IdentifyData->AtaData.SerialNo, MAX_SERIAL_NUMBER_LEN, &Ptr);
  UnicodeSPrintAsciiFormat (SerialNumber, sizeof (SerialNumber), "%a", Ptr);

  return (CHAR16 *) AllocateCopyPool (StrnSizeS (SerialNumber, MAX_SERIAL_NUMBER_LEN + 1), SerialNumber);
}

/**
  Helper function to retrieve Capacity from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Capacity string.

**/
CHAR16 *
RetrieveCapacityFromIdentifyData (
  IN  EFI_IDENTIFY_DATA  *IdentifyData
  )
{
  CONST ATA_IDENTIFY_DATA      *AtaIdentifyData;
  BOOLEAN                      Is48BitAddressSupported;
  BOOLEAN                      ExtendedNoOfSectorsSupported;
  UINT64                       NumOfSectors;
  UINT32                       SectorSizeInWords;
  BOOLEAN                      LogicalSectorSizeSupported;
  H2O_CAPACITY_REPRESENTATION  CapacityRepresentation;
  CHAR8                        Buffer[CAPACITY_REP_STR_MAX_SIZE];
  UINTN                        Length;
  CHAR16                       *CapacityStr;

  if (IdentifyData == NULL) {
    return 0;
  }

  AtaIdentifyData = (ATA_IDENTIFY_DATA *) IdentifyData;

  Is48BitAddressSupported =
    ((AtaIdentifyData->command_set_supported_83 & BIT10) != 0);
  if (!Is48BitAddressSupported) {
    //
    // The device doesn't support 48 bit addressing
    //
    NumOfSectors =
      ((UINT32) AtaIdentifyData->user_addressable_sectors_hi << 16) |
                AtaIdentifyData->user_addressable_sectors_lo;
  } else {
    ExtendedNoOfSectorsSupported =
      ((AtaIdentifyData->additional_supported & BIT3) != 0);
    if (ExtendedNoOfSectorsSupported) {
      //
      // If bit 3 of word 69 is set to one, maximum_lba_for_48bit_addressing
      // (word 100-103) may be (2^32 - 1) for compatibility reason. The actual
      // number is reported in extended_no_of_addressable_sectors. Maximum is
      // (2^48 - 1).
      //
      NumOfSectors = AtaIdentifyData->extended_no_of_addressable_sectors;
    } else {
      //
      // <IndustryStandard/Atapi.h> defines "maximum_lba_for_48bit_addressing"
      // as a UINT16 array with 4 words. (The &FooArray[0] expression helps
      // catch definition mistake.)
      //
      NumOfSectors =
        *(UINT64 *) &AtaIdentifyData->maximum_lba_for_48bit_addressing[0];
    }
  }

  SectorSizeInWords = 256;
  LogicalSectorSizeSupported =
    ((AtaIdentifyData->phy_logic_sector_support & BIT12) != 0);
  if (LogicalSectorSizeSupported) {
    SectorSizeInWords =
      (((UINT32) AtaIdentifyData->logic_sector_size_hi << 16) |
                 AtaIdentifyData->logic_sector_size_lo);
  }

  //
  // The words-to-bytes conversion (sizeof (UINT16)) is done in NumOfSectors to
  // prevent overflowing SectorSizeInWords.
  //
  ComputeBlockIoCapacity (
    SectorSizeInWords,
    ((NumOfSectors * sizeof (UINT16)) - 1),
    FORMULA_NORMAL,
    &CapacityRepresentation,
    NULL // CapacityTotalBytes
    );
  Length = BlockIoCapacityRepToString (
             &CapacityRepresentation,
             CAPACITY_REP_TO_STR_BASE_1000,
             Buffer
             );
  CapacityStr = AllocatePool (sizeof (CHAR16) * (Length + 1));
  if (CapacityStr != NULL) {
    AsciiStrToUnicodeStrS (Buffer, CapacityStr, (Length + 1));
  }

  return CapacityStr;
}

/**
  Helper function to retrieve Form Factor from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Form Factor string.

**/
CHAR16 *
RetrieveFormFactorFromIdentifyData (
  IN  EFI_IDENTIFY_DATA  *IdentifyData
  )
{
  CONST ATA_IDENTIFY_DATA  *AtaIdentifyData;
  UINT16                   NominalFormFactor;
  CONST CHAR16             *FormFactorStr;

  AtaIdentifyData   = (CONST ATA_IDENTIFY_DATA *) IdentifyData;
  NominalFormFactor = AtaIdentifyData->device_nominal_form_factor & B_NOMINAL_FORM_FACTOR_FIELD;

  switch (NominalFormFactor) {

  case AtaNominalFormFactor5Inch25:
    FormFactorStr = L"5.25 inch";
    break;

  case AtaNominalFormFactor3Inch5:
    FormFactorStr = L"3.5 inch";
    break;

  case AtaNominalFormFactor2Inch5:
    FormFactorStr = L"2.5 inch";
    break;

  case AtaNominalFormFactor1Inch8:
    FormFactorStr = L"1.8 inch";
    break;

  case AtaNominalFormFactorLessThan1Inch8:
    FormFactorStr = L"< 1.8 inch";
    break;

  case AtaNominalFormFactorMiniSata:
    FormFactorStr = L"mSATA";
    break;

  case AtaNominalFormFactorMdot2:
    FormFactorStr = L"M.2";
    break;

  case AtaNominalFormFactorMicroSSD:
    FormFactorStr = L"Micro SSD";
    break;

  case AtaNominalFormFactorCFast:
    FormFactorStr = L"CFast";
    break;

  case AtaNominalFormFactorNotReported:
  default:
    FormFactorStr = L"N/A";
    break;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (FormFactorStr), FormFactorStr);
}

/**
  Helper function to retrieve Rotation Rate from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Rotation Rate string.

**/
CHAR16 *
RetrieveRotationRateFromIdentifyData (
  IN  EFI_IDENTIFY_DATA  *IdentifyData
  )
{
  ATA_IDENTIFY_DATA *AtaIdentifyData;
  UINT16            RotationRate;
  CHAR16            *RotationRateStr;
  CHAR16            TempRotationRate[20] = {0};

  if (IdentifyData == NULL) {
    return NULL;
  }

  AtaIdentifyData = (ATA_IDENTIFY_DATA *) IdentifyData;
  RotationRate    = AtaIdentifyData->nominal_media_rotation_rate;
  RotationRateStr = NULL;

  switch (RotationRate) {

  case RotationRateNotReported:
  case RotationRateNonRotatingMedia:
    RotationRateStr = L"N/A";
    break;

  default:
    if (RotationRate >= ATA_DRIVE_ROTATION_RATE_MINIMUM &&
        RotationRate <= ATA_DRIVE_ROTATION_RATE_MAXIMUM
        ) {
      UnicodeSPrint (TempRotationRate, sizeof (TempRotationRate), L"%d rpm", RotationRate);
      RotationRateStr = TempRotationRate;
    }
    break;
  }

  if (RotationRateStr != NULL) {
    return (CHAR16 *) AllocateCopyPool (StrSize (RotationRateStr), RotationRateStr);
  }

  return NULL;
}

/**
  Helper function to check if S.M.A.R.T feature set is supported
  by parsing Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval TRUE    S.M.A.R.T feature set is supported by this drive.
  @retval FALSE   S.M.A.R.T feature set is NOT supported by this drive.

**/
BOOLEAN
IsSmartSupported (
  IN  EFI_IDENTIFY_DATA  *IdentifyData
  )
{
  return ((IdentifyData->AtaData.command_set_supported_82 & B_SMART_SUPPORTED) == B_SMART_SUPPORTED);
}

/**
  Helper function to check if S.M.A.R.T feature set is enabled
  by parsing Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval TRUE    S.M.A.R.T feature set is enabled on this drive.
  @retval FALSE   S.M.A.R.T feature set is disabled on this drive.

**/
BOOLEAN
IsSmartEnabled (
  IN  EFI_IDENTIFY_DATA  *IdentifyData
  )
{
  return ((IdentifyData->AtaData.command_set_feature_enb_85 & B_SMART_ENABLED) == B_SMART_ENABLED);
}

/**
  Helper function to get S.M.A.R.T attribute name by ID.

  This is just a sample imeplementation, all informations are from Wikipedia.

  @param[in]  AttributeId   Attribute ID.

  @retval Attribute name string.

**/
CHAR16 *
GetSmartAttributeNameById (
  IN  UINT8   AttributeId
  )
{
  UINTN   Index;
  CHAR16  *AttributeName;

  for (Index = 0; Index < ARRAY_SIZE (mSmartAttributeTable); Index++) {
    if (mSmartAttributeTable[Index].AttributeId == AttributeId) {
      break;
    }
  }

  if (Index >= ARRAY_SIZE (mSmartAttributeTable)) {
    AttributeName = L"Vendor Specific";
  } else {
    AttributeName = mSmartAttributeTable[Index].AttributeName;
  }

  return (CHAR16 *) AllocateCopyPool (StrSize (AttributeName), AttributeName);
}