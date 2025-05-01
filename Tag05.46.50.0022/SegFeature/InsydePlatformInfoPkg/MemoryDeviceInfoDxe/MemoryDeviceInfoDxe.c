/** @file
  Memory Device Info Protocol Dxe

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "MemoryDeviceInfoDxe.h"

EFI_HII_HANDLE                     mStringPackHandle = NULL;

MEMORY_TYPE_INFO mMemoryTypeInfo[] = {{MemoryTypeOther,                    STRING_TOKEN (STR_MEMORY_TYPE_OTHER_STRING)},
                                      {MemoryTypeUnknown,                  STRING_TOKEN (STR_MEMORY_TYPE_UNKNOWN_STRING)},
                                      {MemoryTypeDram,                     STRING_TOKEN (STR_MEMORY_TYPE_DRAM_STRING)},
                                      {MemoryTypeEdram,                    STRING_TOKEN (STR_MEMORY_TYPE_EDRAM_STRING)},
                                      {MemoryTypeVram,                     STRING_TOKEN (STR_MEMORY_TYPE_VRAM_STRING)},
                                      {MemoryTypeSram,                     STRING_TOKEN (STR_MEMORY_TYPE_SRAM_STRING)},
                                      {MemoryTypeRam,                      STRING_TOKEN (STR_MEMORY_TYPE_RAM_STRING)},
                                      {MemoryTypeRom,                      STRING_TOKEN (STR_MEMORY_TYPE_ROM_STRING)},
                                      {MemoryTypeFlash,                    STRING_TOKEN (STR_MEMORY_TYPE_FLASH_STRING)},
                                      {MemoryTypeEeprom,                   STRING_TOKEN (STR_MEMORY_TYPE_EEPROM_STRING)},
                                      {MemoryTypeFeprom,                   STRING_TOKEN (STR_MEMORY_TYPE_FEPROM_STRING)},
                                      {MemoryTypeEprom,                    STRING_TOKEN (STR_MEMORY_TYPE_EPROM_STRING)},
                                      {MemoryTypeCdram,                    STRING_TOKEN (STR_MEMORY_TYPE_CDRAM_STRING)},
                                      {MemoryType3Dram,                    STRING_TOKEN (STR_MEMORY_TYPE_3DRAM_STRING)},
                                      {MemoryTypeSdram,                    STRING_TOKEN (STR_MEMORY_TYPE_SDRAM_STRING)},
                                      {MemoryTypeSgram,                    STRING_TOKEN (STR_MEMORY_TYPE_SGRAM_STRING)},
                                      {MemoryTypeRdram,                    STRING_TOKEN (STR_MEMORY_TYPE_RDRAM_STRING)},
                                      {MemoryTypeDdr,                      STRING_TOKEN (STR_MEMORY_TYPE_DDR_STRING)},
                                      {MemoryTypeDdr2,                     STRING_TOKEN (STR_MEMORY_TYPE_DDR2_STRING)},
                                      {MemoryTypeDdr2FbDimm,               STRING_TOKEN (STR_MEMORY_TYPE_DDR2_FB_DIMM_STRING)},
                                      {MemoryTypeDdr3,                     STRING_TOKEN (STR_MEMORY_TYPE_DDR3_STRING)},
                                      {MemoryTypeFbd2,                     STRING_TOKEN (STR_MEMORY_TYPE_FBD2_STRING)},
                                      {MemoryTypeDdr4,                     STRING_TOKEN (STR_MEMORY_TYPE_DDR4_STRING)},
                                      {MemoryTypeLpddr,                    STRING_TOKEN (STR_MEMORY_TYPE_LPDDR_STRING)},
                                      {MemoryTypeLpddr2,                   STRING_TOKEN (STR_MEMORY_TYPE_LPDDR2_STRING)},
                                      {MemoryTypeLpddr3,                   STRING_TOKEN (STR_MEMORY_TYPE_LPDDR3_STRING)},
                                      {MemoryTypeLpddr4,                   STRING_TOKEN (STR_MEMORY_TYPE_LPDDR4_STRING)},
#if FixedPcdGet16 (PcdSmbiosVersion) > 0x0301
                                      {MemoryTypeLogicalNonVolatileDevice, STRING_TOKEN (STR_MEMORY_TYPE_LOGICAL_NON_VOLATILE_DEVICE_STRING)},
#endif
#if FixedPcdGet16 (PcdSmbiosVersion) > 0x0302
                                      {MemoryTypeHBM,                      STRING_TOKEN (STR_MEMORY_TYPE_HBM_STRING)},
                                      {MemoryTypeHBM2,                     STRING_TOKEN (STR_MEMORY_TYPE_HBM2_STRING)},
                                      {MemoryTypeDdr5,                     STRING_TOKEN (STR_MEMORY_TYPE_DDR5_STRING)},
                                      {MemoryTypeLpddr5,                   STRING_TOKEN (STR_MEMORY_TYPE_LPDDR5_STRING)}
#endif
                                     };

MEMORY_TYPE_DETAIL_INFO mMemoryTypeDetailInfo[] = {STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_RESERVED_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_OTHER_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_UNKNOWN_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_FAST_PAGED_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_STATIC_COLUMN_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_PSEUDO_STATIC_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_RAMBUS_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_SYNCHRONOUS_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_CMOS_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_EDO_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_WINDOW_DRAM_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_CACHE_DRAM_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_NON_VOLATILE_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_BUFFERED_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_UNBUFFERED_STRING),
                                                   STRING_TOKEN (STR_MEMORY_TYPE_DETAIL_LRDIMM_STRING)
                                                  };

DRAM_TYPE_INFO mDramTypeInfo[] = {STRING_TOKEN (STR_DRAM_TYPE_HYBRID_DIMM_STRING),
                                  STRING_TOKEN (STR_DRAM_TYPE_RDIMM_STRING),
                                  STRING_TOKEN (STR_DRAM_TYPE_UDIMM_STRING),
                                  STRING_TOKEN (STR_DRAM_TYPE_LRDIMM_STRING)
                                 };

ECC_TYPE_INFO mEccTypeInfo[] = {{MemoryErrorCorrectionOther,        STRING_TOKEN (STR_ECC_TYPE_OTHER_STRING)},
                                {MemoryErrorCorrectionUnknown,      STRING_TOKEN (STR_ECC_TYPE_UNKNOWN_STRING)},
                                {MemoryErrorCorrectionNone,         STRING_TOKEN (STR_ECC_TYPE_NONE_STRING)},
                                {MemoryErrorCorrectionParity,       STRING_TOKEN (STR_ECC_TYPE_PARITY_STRING)},
                                {MemoryErrorCorrectionSingleBitEcc, STRING_TOKEN (STR_ECC_TYPE_SINGLE_BIT_ECC_STRING)},
                                {MemoryErrorCorrectionMultiBitEcc,  STRING_TOKEN (STR_ECC_TYPE_MULTI_BIT_ECC_STRING)},
                                {MemoryErrorCorrectionCrc,          STRING_TOKEN (STR_ECC_TYPE_CRC_STRING)}
                               };

/**
  Check Select MemoryInfo Is Valid

  @param[in]   PrivateData                  Points to the H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA.
  @param[in]   MemoryInfoIndex              Which memory info index to get.

  @retval EFI_SUCCESS                       The select memory info is valid.
  @retval EFI_INVALID_PARAMETER             The Param PrivateData is NULL.
  @retval EFI_UNSUPPORTED                   The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                     Cannot find smbios type 17.

**/
STATIC
EFI_STATUS
CheckSelectMemoryInfoIsValid (
  IN   H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA  *PrivateData,
  IN   UINTN                                MemoryInfoIndex
  )
{
  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->NumberOfMemoryInfo == 0) {
    DEBUG ((EFI_D_INFO, "%a() - The Memory Devices(Smbios type17) are not found in system\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  } else if (MemoryInfoIndex >= PrivateData->NumberOfMemoryInfo) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The MemoryInfoIndex(%d) is over table(%d), please check input Param\n",
      __FUNCTION__,
      MemoryInfoIndex,
      PrivateData->NumberOfMemoryInfo
      ));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  The MemoryType value to string

  @param[in]  MemoryType  The smbios type17 MemoryType value.
  @param[in]  Language    The language of the string to retrieve.  If this
                          parameter is NULL, then the current platform
                          language is used.  The format of Language must
                          follow the language format assumed the HII Database.

  @retval     The MemoryType String

**/
STATIC
CHAR16 *
GetMemoryType (
  IN  UINT8   MemoryType,
  IN  CHAR8   *Languages  OPTIONAL
  )
{
  UINTN          Index;
  EFI_STRING_ID  StringToken;
  CHAR16         *String;

  StringToken = STRING_TOKEN (STR_NONE_STRING);
  for (Index = 0; Index < (sizeof (mMemoryTypeInfo) / sizeof (MEMORY_TYPE_INFO)); Index++) {
    if (MemoryType == mMemoryTypeInfo[Index].MemoryType) {
      StringToken = mMemoryTypeInfo[Index].MemoryTypeString;
      break;
    }
  }

  String = HiiGetString (mStringPackHandle, StringToken, Languages);

  return String;
}

/**
  The MemoryTypeDetail value to string

  @param[in]  MemoryTypeDetail  The smbios type17 MemoryTypeDetail value.
  @param[in]  Language          The language of the string to retrieve.  If this
                                parameter is NULL, then the current platform
                                language is used.  The format of Language must
                                follow the language format assumed the HII Database.

  @retval     The MemoryTypeDetail String

**/
STATIC
CHAR16 *
GetMemoryTypeDetail (
  IN  MEMORY_DEVICE_TYPE_DETAIL       MemoryTypeDetail,
  IN  CHAR8                           *Languages  OPTIONAL
  )
{
  UINTN                               Index;
  UINTN                               TotalStrSize;
  UINTN                               CommaStrSize;
  UINTN                               TempStrSize;
  CHAR16                              *String;
  CHAR16                              *TempString;
  CHAR16                              *CommaString = L", ";
  BOOLEAN                             FirstType;
  UINT16                              CheckMemoryTypeDetailBit;
  MEMORY_DEVICE_TYPE_DETAIL_CONVERT   MemoryTypeDetailConvert;

  TotalStrSize = 0;
  TempStrSize = 0;
  CommaStrSize = StrSize (CommaString);
  FirstType = TRUE;
  String = NULL;

  MemoryTypeDetailConvert.MemoryTypeDetailBit = MemoryTypeDetail;

  if (MemoryTypeDetailConvert.MemoryTypeDetailWord == 0x00) {
    return HiiGetString (mStringPackHandle, STRING_TOKEN (STR_NONE_STRING), Languages);;
  }

  for (Index = 0; Index < (sizeof (mMemoryTypeDetailInfo) / sizeof (MEMORY_TYPE_DETAIL_INFO)); Index++) {
    CheckMemoryTypeDetailBit = (MemoryTypeDetailConvert.MemoryTypeDetailWord >> Index) & BIT0;
    if (CheckMemoryTypeDetailBit) {
      TempString = HiiGetString (mStringPackHandle, mMemoryTypeDetailInfo[Index], Languages);
      if (TempString == NULL) {
        DEBUG ((EFI_D_ERROR, "%a() - Fail to get memory type detail hii string\n", __FUNCTION__));
        continue;
      }
      TempStrSize = FirstType ? StrSize (TempString) : StrSize (TempString) + CommaStrSize;
      String = ReallocatePool (
                 (TotalStrSize == 0) ? TempStrSize : TotalStrSize,
                 TotalStrSize + TempStrSize,
                 (VOID *) String
                 );
      if (String == NULL) {
        DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate memory type detail string buffer\n", __FUNCTION__));
        continue;
      }
      if (!FirstType) {
        StrCatS (String, (TotalStrSize + TempStrSize), CommaString);
      }
      StrCatS (String, (TotalStrSize + TempStrSize), TempString);
      TotalStrSize += TempStrSize;
      FirstType = FALSE;
      FreePool (TempString);
    }
  }

  return String;
}

/**
  The MemoryTypeDetail value olny DramType to string

  @param[in]  DramType  The smbios type17 MemoryTypeDetail value.
  @param[in]  Language  The language of the string to retrieve.  If this
                        parameter is NULL, then the current platform
                        language is used.  The format of Language must
                        follow the language format assumed the HII Database.

  @retval     The DramType String

**/
STATIC
CHAR16 *
GetDramType (
  IN  MEMORY_DEVICE_TYPE_DETAIL       DramType,
  IN  CHAR8                           *Languages  OPTIONAL
  )
{
  UINTN                               Index;
  UINT16                              CheckMemoryTypeDetailBit;
  MEMORY_DEVICE_TYPE_DETAIL_CONVERT   MemoryTypeDetailConvert;
  EFI_STRING_ID                       StringToken;
  CHAR16                              *String;

  StringToken = STRING_TOKEN (STR_NONE_STRING);

  MemoryTypeDetailConvert.MemoryTypeDetailBit = DramType;

  CheckMemoryTypeDetailBit = MemoryTypeDetailConvert.MemoryTypeDetailWord >> 12;
  for (Index = 0; Index < (sizeof (mDramTypeInfo) / sizeof (DRAM_TYPE_INFO)); Index++) {
    if ((CheckMemoryTypeDetailBit >> Index) & BIT0) {
      StringToken = mDramTypeInfo[Index];
      break;
    }
  }

  String = HiiGetString (mStringPackHandle, StringToken, Languages);

  return String;
}

/**
  The EccType value to string

  @param[in]  EccType  The smbios type17 MemoryTypeDetail value.
  @param[in]  Language The language of the string to retrieve.  If this
                       parameter is NULL, then the current platform
                       language is used.  The format of Language must
                       follow the language format assumed the HII Database.

  @retval     The EccType String

**/
STATIC
CHAR16 *
GetEccType (
  IN  UINT8                        EccType,
  IN  CHAR8                        *Languages  OPTIONAL
  )
{
  UINTN                            Index;
  EFI_STRING_ID                    StringToken;
  CHAR16                           *String;

  StringToken = STRING_TOKEN (STR_NONE_STRING);
  for (Index = 0; Index < (sizeof (mEccTypeInfo) / sizeof (ECC_TYPE_INFO)); Index++) {
    if (EccType == mEccTypeInfo[Index].EccType) {
      StringToken = mEccTypeInfo[Index].EccTypeString;
      break;
    }
  }

  String = HiiGetString (mStringPackHandle, StringToken, Languages);

  return String;
}

/**
  Get Memory Install Status

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.

  @retval TRUE                             The memory are installed in slot.
  @retval FALSE                            The memory are not installed in slot.

**/
BOOLEAN
EFIAPI
GetMemoryInstallStatus (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;

  if ((This == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return FALSE;
  }

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled;
}

/**
  Get Minimum Voltage

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  MinimumVoltage              The contains minimum voltage pointer.

  @retval EFI_SUCCESS                      Get minimum voltage successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or MinimumVoltage is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetMinimumVoltage (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT16                              *MinimumVoltage
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (MinimumVoltage == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MinimumVoltage is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *MinimumVoltage = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *MinimumVoltage = SmbiosTableType17->MinimumVoltage;

  return Status;
}

/**
  Get Maximum Voltage

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  MaximumVoltage              The contains maximum voltage pointer.

  @retval EFI_SUCCESS                      Get maximum voltage successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or MaximumVoltage is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetMaximumVoltage (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT16                              *MaximumVoltage
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (MaximumVoltage == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MaximumVoltage is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *MaximumVoltage = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *MaximumVoltage = SmbiosTableType17->MaximumVoltage;

  return Status;
}

/**
  Get Configure Voltage

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  ConfigureVoltage            The contains configure voltage pointer.

  @retval EFI_SUCCESS                      Get configure voltage successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or ConfigureVoltage is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetConfigureVoltage (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT16                              *ConfigureVoltage
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (ConfigureVoltage == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/ConfigureVoltage is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *ConfigureVoltage = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *ConfigureVoltage = SmbiosTableType17->ConfiguredVoltage;

  return Status;
}

/**
  Get Configure Memory Speed

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  ConfigureMemorySpeed        The contains configure memory speed pointer

  @retval EFI_SUCCESS                      Get configure memory speed successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or ConfigureMemorySpeed is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetConfigureMemorySpeed (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT32                              *ConfigureMemorySpeed
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (ConfigureMemorySpeed == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/ConfigureMemorySpeed is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *ConfigureMemorySpeed = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *ConfigureMemorySpeed = SmbiosTableType17->ConfiguredMemoryClockSpeed;

  return Status;
}

/**
  Get Maximum Capable Memory Speed

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  MaximumCapableMemorySpeed   The contains maximum capable memory speed pointer

  @retval EFI_SUCCESS                      Get maximum capable memory speed successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or MaximumCapableMemorySpeed is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetMaximumCapableMemorySpeed (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT32                              *MaximumCapableMemorySpeed
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (MaximumCapableMemorySpeed == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MaximumCapableMemorySpeed is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *MaximumCapableMemorySpeed = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *MaximumCapableMemorySpeed = SmbiosTableType17->Speed;

  return Status;
}

/**
  Get Memory Size

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  MemorySize                  The contains memory size pointer

  @retval EFI_SUCCESS                      Get memory size successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or MemorySize is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetMemorySize (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT32                              *MemorySize
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;
  UINT32                                   TempMemorySize;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (MemorySize == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MemorySize is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *MemorySize = 0;
  TempMemorySize = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  if (SmbiosTableType17->Size < 0x7FFF) {
    TempMemorySize = SmbiosTableType17->Size;
  } else {
    //
    // If Memory Size Exceed 32GB, The Memory Size Change From ExtendedSize Offset To Get It
    //
    TempMemorySize = SmbiosTableType17->ExtendedSize;
  }

  *MemorySize = TempMemorySize;

  return Status;
}

/**
  Get Part Number String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  PartNumberString            A pointer to the buffer to return, that contains part number string.
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get part number string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or PartNumberString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_DEVICE_ERROR                 Cannot find PartNumberString in smbios table type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetPartNumberString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **PartNumberString
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (PartNumberString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/PartNumberString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *PartNumberString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *PartNumberString = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType17, SmbiosTableType17->PartNumber);
  if (*PartNumberString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get part number string\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get Serial Number String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  SerialNumberString          A pointer to the buffer to return, that contains serial number string.
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get serial number string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or SerialNumberString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_DEVICE_ERROR                 Cannot find SerialNumberString in smbios table type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetSerialNumberString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **SerialNumberString
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (SerialNumberString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/SerialNumberString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *SerialNumberString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *SerialNumberString = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType17, SmbiosTableType17->SerialNumber);
  if (*SerialNumberString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get serial number string\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get Memory Type String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  MemoryTypeString            A pointer to a allocated buffer that contains memory type string.
                                           It is caller's responsibility to free this buffer.
  @param[in]   Language                    The language of the string to retrieve.  If this
                                           parameter is NULL, then the current platform
                                           language is used.  The format of Language must
                                           follow the language format assumed the HII Database.

  @retval EFI_SUCCESS                      Get memory type string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or MemoryTypeString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.
  @retval EFI_OUT_OF_RESOURCES             Allocated string buffer fail.

**/
EFI_STATUS
EFIAPI
GetMemoryTypeString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **MemoryTypeString,
  IN   CHAR8                               *Languages  OPTIONAL
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;
  CHAR16                                   *String;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (MemoryTypeString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MemoryTypeString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *MemoryTypeString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  String = GetMemoryType (SmbiosTableType17->MemoryType, Languages);
  if (String == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get memory type hii string\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  *MemoryTypeString = String;

  return EFI_SUCCESS;
}

/**
  Get Memory Type Detail String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  MemoryTypeDetailString      A pointer to a allocated buffer that contains memory type detail string.
                                           It is caller's responsibility to free this buffer.
  @param[in]   Language                    The language of the string to retrieve.  If this
                                           parameter is NULL, then the current platform
                                           language is used.  The format of Language must
                                           follow the language format assumed the HII Database.

  @retval EFI_SUCCESS                      Get memory type detail string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or MemoryTypeDetailString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.
  @retval EFI_OUT_OF_RESOURCES             Allocated string buffer fail.

**/
EFI_STATUS
EFIAPI
GetMemoryTypeDetailString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **MemoryTypeDetailString,
  IN   CHAR8                               *Languages  OPTIONAL
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;
  CHAR16                                   *String;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (MemoryTypeDetailString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MemoryTypeDetailString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *MemoryTypeDetailString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  String = GetMemoryTypeDetail (SmbiosTableType17->TypeDetail, Languages);
  if (String == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate String buffer\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  *MemoryTypeDetailString = String;

  return Status;
}

/**
  Get Dram Type String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  DramTypeString              A pointer to a allocated buffer that contains dram type string.
                                           It is caller's responsibility to free this buffer.
  @param[in]   Language                    The language of the string to retrieve.  If this
                                           parameter is NULL, then the current platform
                                           language is used.  The format of Language must
                                           follow the language format assumed the HII Database.

  @retval EFI_SUCCESS                      Get device locator string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or ManufacturerString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.
  @retval EFI_OUT_OF_RESOURCES             Allocated string buffer fail.

**/
EFI_STATUS
EFIAPI
GetDramTypeString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **DramTypeString,
  IN   CHAR8                               *Languages  OPTIONAL
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;
  CHAR16                                   *String;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (DramTypeString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/DramTypeString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *DramTypeString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  String = GetDramType (SmbiosTableType17->TypeDetail, Languages);
  if (String == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get dram type hii string\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  *DramTypeString = String;

  return Status;
}

/**
  Get Manufacturer String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  ManufacturerString          A pointer to the buffer to return, that contains manufacturer string.
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get device locator string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or ManufacturerString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_DEVICE_ERROR                 Cannot find ManufacturerString in smbios table type 17.
  @retval EFI_NOT_READY                    The SmbiosType17 exist but memory not install.

**/
EFI_STATUS
EFIAPI
GetManufacturerString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **ManufacturerString
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (ManufacturerString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/ManufacturerString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *ManufacturerString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PrivateData->MemoryInfo[MemoryInfoIndex].MemoryInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *ManufacturerString = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType17, SmbiosTableType17->Manufacturer);
  if (*ManufacturerString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get manufacturer string\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get Bank Locator String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  BankLocatorString           A pointer to the buffer to return, that contains bank locator string.
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get device locator string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or BankLocatorString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_DEVICE_ERROR                 Cannot find BankLocatorString in smbios table type 17.

**/
EFI_STATUS
EFIAPI
GetBankLocatorString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **BankLocatorString
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (BankLocatorString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/BankLocatorString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *BankLocatorString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *BankLocatorString = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType17, SmbiosTableType17->BankLocator);
  if (*BankLocatorString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get bank locator string\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get Device Locator String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.
  @param[out]  DeviceLocatorString         A pointer to the buffer to return, that contains device locator string.
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get device locator string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or DeviceLocatorString is NULL.
  @retval EFI_UNSUPPORTED                  The Param MemoryInfoIndex is over data record maximum.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_DEVICE_ERROR                 Cannot find DeviceLocatorString in smbios table type 17.

**/
EFI_STATUS
EFIAPI
GetDeviceLocatorString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **DeviceLocatorString
  )
{
  EFI_STATUS                               Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      *SmbiosTableType17;

  Status = EFI_SUCCESS;

  if ((This == NULL) || (DeviceLocatorString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/DeviceLocatorString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *DeviceLocatorString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  Status = CheckSelectMemoryInfoIsValid (PrivateData, MemoryInfoIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmbiosTableType17 = PrivateData->MemoryInfo[MemoryInfoIndex].MemoryDevice;
  *DeviceLocatorString = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType17, SmbiosTableType17->DeviceLocator);
  if (*DeviceLocatorString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get device locator string\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get Ecc Type String

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[out]  EccTypeString               A pointer to a allocated buffer that contains ecc type string.
                                           It is caller's responsibility to free this buffer.
  @param[in]   Language                    The language of the string to retrieve.  If this
                                           parameter is NULL, then the current platform
                                           language is used.  The format of Language must
                                           follow the language format assumed the HII Database.

  @retval EFI_SUCCESS                      Get ecc type string successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or EccTypeString is NULL.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 16.
  @retval EFI_OUT_OF_RESOURCES             Allocated string buffer fail.

**/
EFI_STATUS
EFIAPI
GetEccTypeString (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  OUT  CHAR16                              **EccTypeString,
  IN   CHAR8                               *Languages  OPTIONAL
  )
{
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  CHAR16                                   *String;

  if ((This == NULL) || (EccTypeString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/EccTypeString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *EccTypeString = NULL;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  if (PrivateData->NumberOfPhysicalMemoryArray == 0) {
    DEBUG ((EFI_D_INFO, "%a() - The Physical Memory Array(Smbios type16) are not found in system\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  String = GetEccType (PrivateData->PhysicalMemoryArray[0]->MemoryErrorCorrection, Languages);
  if (String == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get ecc type hii string\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  *EccTypeString = String;

  return EFI_SUCCESS;
}


/**
  Get Total Memory Size

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[out]  TotalMemorySize             The contains memory size pointer

  @retval EFI_SUCCESS                      Get total memory size successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or TotalMemorySize is NULL.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 19.

**/
EFI_STATUS
EFIAPI
GetTotalMemorySize (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  OUT  UINT64                              *TotalMemorySize
  )
{
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE19                      **MemoryArrayMappedAddress;
  UINTN                                    Index;
  UINT64                                   TempTotalMemorySize;

  if ((This == NULL) || (TotalMemorySize == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/TotalMemorySize is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *TotalMemorySize = 0;
  TempTotalMemorySize = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  if (PrivateData->NumberOfMemoryArrayMappedAddress == 0) {
    DEBUG ((EFI_D_INFO, "%a() - The Memory Array Mapped Address(Smbios type19) are not found in system\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  MemoryArrayMappedAddress = PrivateData->MemoryArrayMappedAddress;

  for (Index = 0; Index < PrivateData->NumberOfMemoryArrayMappedAddress; Index++) {
    if ((MemoryArrayMappedAddress[Index]->StartingAddress == 0xFFFFFFFF) &&
        (MemoryArrayMappedAddress[Index]->EndingAddress == 0xFFFFFFFF)) {
      TempTotalMemorySize += ((MemoryArrayMappedAddress[Index]->ExtendedEndingAddress -
                               MemoryArrayMappedAddress[Index]->ExtendedStartingAddress) + 1);
    } else {
      TempTotalMemorySize += MultU64x32 ((UINT64)((MemoryArrayMappedAddress[Index]->EndingAddress -
                                                   MemoryArrayMappedAddress[Index]->StartingAddress) + 1), SIZE_1KB) ;
    }
  }

  *TotalMemorySize = TempTotalMemorySize;

  return EFI_SUCCESS;
}

/**
  Get The Number Of Smbios Type17 And Record

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[out]  NumberOfMemoryInfo          The number of smbios type17 returned in Buffer.
  @param[out]  NumberOfInstallMemory       The number of memory are installed returned in Buffer.
  @param[out]  NumberOfUnInstallMemory     The number of memory are uninstalled returned in Buffer.
  @param[out]  SmbiosTableType17s          A pointer to a allocated buffer that contains SmbiosTableType17
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get device information successfully.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_OUT_OF_RESOURCES             Allocated buffer for SmbiosTableType17 fail.
  @retval EFI_INVALID_PARAMETER            The Param this is NULL.

**/
EFI_STATUS
EFIAPI
GetAllMemoryDeviceInfo (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  OUT  UINTN                               *NumberOfMemoryInfo,
  OUT  UINTN                               *NumberOfInstallMemory,
  OUT  UINTN                               *NumberOfUnInstallMemory,
  OUT  SMBIOS_TABLE_TYPE17                 ***SmbiosTableType17s OPTIONAL
  )
{
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA      *PrivateData;
  SMBIOS_TABLE_TYPE17                      **TempSmbiosTableType17s = NULL;
  UINTN                                    Index;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The memory device info protocol is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *NumberOfMemoryInfo = 0;
  *NumberOfInstallMemory = 0;
  *NumberOfUnInstallMemory = 0;

  PrivateData = H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  if (PrivateData->NumberOfMemoryInfo == 0) {
    DEBUG ((EFI_D_INFO, "%a() - The Memory Devices(Smbios type17) are not found in system\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  *NumberOfMemoryInfo = PrivateData->NumberOfMemoryInfo;
  *NumberOfInstallMemory = PrivateData->NumberOfInstallMemory;
  *NumberOfUnInstallMemory = PrivateData->NumberOfUnInstallMemory;

  if (SmbiosTableType17s == NULL) {
    return EFI_SUCCESS;
  }

  TempSmbiosTableType17s = (SMBIOS_TABLE_TYPE17 **) AllocateZeroPool (PrivateData->NumberOfMemoryInfo * sizeof (SMBIOS_TABLE_TYPE17 *));
  if (TempSmbiosTableType17s == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate TempSmbiosTableType17s buffer\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < PrivateData->NumberOfMemoryInfo; Index++) {
    TempSmbiosTableType17s[Index] = PrivateData->MemoryInfo[Index].MemoryDevice;
  }

  *SmbiosTableType17s = TempSmbiosTableType17s;

  return EFI_SUCCESS;
}

/**
 Initialize Memory Device Info Protocol.

 @param[in] None.

 @retval EFI_SUCCESS           The memory device info protocol is initialize successfully.
 @retval EFI_OUT_OF_RESOURCES  The function allocate memory buffer fail.
 @retval other                 Some error occurs when executing this function.

**/
EFI_STATUS
InitializeMemoryDeviceProtocol (
  VOID
  )
{
  EFI_STATUS                           Status;
  H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA  *Private;
  EFI_SMBIOS_PROTOCOL                  *Smbios;
  EFI_SMBIOS_HANDLE                    SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER              *Record;
  UINTN                                NumberOfMemoryInfo;
  MEMORY_INFO                          *MemoryInfo;
  UINTN                                NumberOfPhysicalMemoryArray;
  SMBIOS_TABLE_TYPE16                  **SmbiosTableType16;
  UINTN                                NumberOfMemoryArrayMappedAddress;
  SMBIOS_TABLE_TYPE19                  **SmbiosTableType19;
  UINTN                                NumberOfInstallMemory;
  UINTN                                NumberOfUnInstallMemory;
  UINT64                               TotalMemorySize;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  NumberOfMemoryInfo = 0;
  NumberOfPhysicalMemoryArray = 0;
  NumberOfMemoryArrayMappedAddress = 0;
  NumberOfInstallMemory = 0;
  NumberOfUnInstallMemory = 0;
  TotalMemorySize = 0;
  MemoryInfo = NULL;
  SmbiosTableType16 = NULL;
  SmbiosTableType19 = NULL;

  Status = gBS->LocateProtocol (
              &gEfiSmbiosProtocolGuid,
              NULL,
              (VOID **) &Smbios
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to locate protocol gEfiSmbiosProtocolGuid(%g), Status: %r\n",
      __FUNCTION__,
      gEfiSmbiosProtocolGuid,
      Status
      ));
    return Status;
  }

  Private = (H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA *) AllocateZeroPool (sizeof (H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA));
  if (Private == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate Private data\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }

    //
    // Gather Smbios Type 16 Data
    //
    if (Record->Type == SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY) {
      SmbiosTableType16 = (SMBIOS_TABLE_TYPE16 **) ReallocatePool (
                                                     NumberOfPhysicalMemoryArray * sizeof (EFI_SMBIOS_TABLE_HEADER *),
                                                     (NumberOfPhysicalMemoryArray + 1) * sizeof (EFI_SMBIOS_TABLE_HEADER *),
                                                     (VOID *) SmbiosTableType16
                                                     );
      if (SmbiosTableType16 == NULL) {
        DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate SmbiosTableType16 buffer\n", __FUNCTION__));
        continue;
      }
      SmbiosTableType16[NumberOfPhysicalMemoryArray++] = (SMBIOS_TABLE_TYPE16 *) Record;
    }

    //
    // Gather Smbios Type 17 Data
    //
    if (Record->Type == EFI_SMBIOS_TYPE_MEMORY_DEVICE) {
      MemoryInfo = (MEMORY_INFO *) ReallocatePool (
                                     NumberOfMemoryInfo * sizeof (MEMORY_INFO),
                                     (NumberOfMemoryInfo + 1) * sizeof (MEMORY_INFO),
                                     (VOID *) MemoryInfo
                                     );
      if (MemoryInfo == NULL) {
        DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate MemoryInfo buffer\n", __FUNCTION__));
        continue;
      }

      MemoryInfo[NumberOfMemoryInfo].MemoryDevice = (SMBIOS_TABLE_TYPE17 *) Record;
      if (MemoryInfo[NumberOfMemoryInfo].MemoryDevice->Size != 0x00) {
        MemoryInfo[NumberOfMemoryInfo].MemoryInstalled = TRUE;
        NumberOfInstallMemory++;
      } else {
        MemoryInfo[NumberOfMemoryInfo].MemoryInstalled = FALSE;
        NumberOfUnInstallMemory++;
      }
      NumberOfMemoryInfo++;
    }

    //
    // Gather Smbios Type 19 Data
    //
    if (Record->Type == EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS) {
      SmbiosTableType19 = (SMBIOS_TABLE_TYPE19 **) ReallocatePool (
                                                     NumberOfMemoryArrayMappedAddress * sizeof (EFI_SMBIOS_TABLE_HEADER *),
                                                     (NumberOfMemoryArrayMappedAddress + 1) * sizeof (EFI_SMBIOS_TABLE_HEADER *),
                                                     (VOID *) SmbiosTableType19
                                                     );
      if (SmbiosTableType19 == NULL) {
        DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate SmbiosTableType19 buffer\n", __FUNCTION__));
        continue;
      }
      SmbiosTableType19[NumberOfMemoryArrayMappedAddress++] = (SMBIOS_TABLE_TYPE19 *)Record;
    }
  } while (SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED);

  Private->Signature = H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA_SIGNATURE;
  Private->NumberOfMemoryInfo = NumberOfMemoryInfo;
  Private->MemoryInfo = MemoryInfo;
  Private->NumberOfInstallMemory = NumberOfInstallMemory;
  Private->NumberOfUnInstallMemory = NumberOfUnInstallMemory;
  Private->NumberOfPhysicalMemoryArray = NumberOfPhysicalMemoryArray;
  Private->PhysicalMemoryArray = SmbiosTableType16;
  Private->NumberOfMemoryArrayMappedAddress = NumberOfMemoryArrayMappedAddress;
  Private->MemoryArrayMappedAddress = SmbiosTableType19;

  //
  // Install protocol
  //
  Private->MemoryDeviceInfo.GetAllMemoryDeviceInfo             = GetAllMemoryDeviceInfo;
  Private->MemoryDeviceInfo.GetTotalMemorySize                 = GetTotalMemorySize;
  Private->MemoryDeviceInfo.GetDeviceLocatorString             = GetDeviceLocatorString;
  Private->MemoryDeviceInfo.GetBankLocatorString               = GetBankLocatorString;
  Private->MemoryDeviceInfo.GetManufacturerString              = GetManufacturerString;
  Private->MemoryDeviceInfo.GetMemoryTypeString                = GetMemoryTypeString;
  Private->MemoryDeviceInfo.GetMemoryTypeDetailString          = GetMemoryTypeDetailString;
  Private->MemoryDeviceInfo.GetDramTypeString                  = GetDramTypeString;
  Private->MemoryDeviceInfo.GetSerialNumberString              = GetSerialNumberString;
  Private->MemoryDeviceInfo.GetPartNumberString                = GetPartNumberString;
  Private->MemoryDeviceInfo.GetMemorySize                      = GetMemorySize;
  Private->MemoryDeviceInfo.GetMaximumCapableMemorySpeed       = GetMaximumCapableMemorySpeed;
  Private->MemoryDeviceInfo.GetConfigureMemorySpeed            = GetConfigureMemorySpeed;
  Private->MemoryDeviceInfo.GetMinimumVoltage                  = GetMinimumVoltage;
  Private->MemoryDeviceInfo.GetMaximumVoltage                  = GetMaximumVoltage;
  Private->MemoryDeviceInfo.GetConfigureVoltage                = GetConfigureVoltage;
  Private->MemoryDeviceInfo.GetEccTypeString                   = GetEccTypeString;
  Private->MemoryDeviceInfo.GetMemoryInstallStatus             = GetMemoryInstallStatus;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->Handle,
                  &gH2OMemoryDeviceInfoProtocolGuid,
                  &Private->MemoryDeviceInfo,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to InstallMultipleProtocolInterfaces for gH2OMemoryDeviceInfoProtocolGuid(%g), Status = %r\n",
      __FUNCTION__,
      gH2OMemoryDeviceInfoProtocolGuid,
      Status
      ));
    FreePool (Private);
  }

  //
  // Initialize strings to HII database
  //
  mStringPackHandle = HiiAddPackages (
                        &gEfiCallerIdGuid,
                        NULL,
                        MemoryDeviceInfoDxeStrings,
                        NULL
                        );
  if (mStringPackHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to add hii string\n", __FUNCTION__));
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return Status;
}

/**
 Notify Initialize Memory Device Info Protocol.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
MemoryDeviceInfoProtocolInitNotifyFunc (
  IN EFI_EVENT                        Event,
  IN H2O_CP_HANDLE                    Handle
  )
{
  H2OCpUnregisterHandler (Handle);
  InitializeMemoryDeviceProtocol ();
}

/**
  The entry point for the driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
MemoryDeviceInfoEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  H2O_CP_HANDLE                       CpHandle;
  EFI_STATUS                          Status;

  if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpDisplayBeforeGuid,
               MemoryDeviceInfoProtocolInitNotifyFunc,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    DEBUG ((EFI_D_INFO, "Checkpoint Registered : %g (%r)\n", &gH2OBdsCpDisplayBeforeGuid, Status));

    return Status;
  }

  return EFI_UNSUPPORTED;
}

