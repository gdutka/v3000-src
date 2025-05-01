/** @file
  H2O Memory Device Info Protocol Header File.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _H2O_MEMORY_DEVICE_INFO_H_
#define _H2O_MEMORY_DEVICE_INFO_H_

#include <IndustryStandard/SmBios.h>

//
// H2O_MEMORY_DEVICE_INFO_PROTOCOL_GUID Protocol GUID
//
#define H2O_MEMORY_DEVICE_INFO_PROTOCOL_GUID \
  {0x8588EB59, 0x3547, 0x46DD, 0x93, 0xDE, 0x8F, 0x31, 0xB3, 0xE5, 0x04, 0xE5}

typedef struct _H2O_MEMORY_DEVICE_INFO_PROTOCOL  H2O_MEMORY_DEVICE_INFO_PROTOCOL;

/**
  Get The Number Of Smbios Type17 And Record

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[out]  NumberOfMemoryInfo          The number of smbios type17 returned in Buffer.
  @param[out]  NumberOfInstallMemory       The number of memory are installed returned in Buffer.
  @param[out]  NumberOfUnInstallMemory     The number of memory are uninstalled returned in Buffer.
  @param[out]  SmbiosType17Record          A pointer to a allocated buffer that contains SmbiosType17Record
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get device information successfully.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 17.
  @retval EFI_OUT_OF_RESOURCES             Allocated buffer for SmbiosType17Record fail.
  @retval EFI_INVALID_PARAMETER            The Param this is NULL.

**/
typedef
EFI_STATUS
(EFIAPI *GET_ALL_MEMORY_DEVICE_INFO) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  OUT  UINTN                               *NumberOfMemoryInfo,
  OUT  UINTN                               *NumberOfInstallMemory,
  OUT  UINTN                               *NumberOfUnInstallMemory,
  OUT  SMBIOS_TABLE_TYPE17                 ***SmbiosType17Records OPTIONAL
  );

/**
  Get Total Memory Size

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[out]  TotalMemorySize             The contains memory size pointer

  @retval EFI_SUCCESS                      Get total memory size successfully.
  @retval EFI_INVALID_PARAMETER            The Param This or TotalMemorySize is NULL.
  @retval EFI_NOT_FOUND                    Cannot find smbios type 19.

**/
typedef
EFI_STATUS
(EFIAPI *GET_TOTAL_MEMORY_SIZE) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  OUT  UINT64                              *TotalMemorySize
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_DEVICE_LOCATOR_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **DeviceLocatorString
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_BANK_LOCATOR_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **BankLocatorString
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MANUFACTURER_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **ManufacturerString
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MEMORY_TYPE_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **MemoryTypeString,
  IN   CHAR8                               *Languages  OPTIONAL
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MEMORY_TYPE_DETAIL_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **MemoryTypeDetailString,
  IN   CHAR8                               *Languages  OPTIONAL
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_DRAM_TYPE_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **DramTypeString,
  IN   CHAR8                               *Languages  OPTIONAL
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_SERIAL_NUMBER_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **SerialNumberString
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_PART_NUMBER_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  CHAR16                              **PartNumberString
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MEMORY_SIZE) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT32                              *MemorySize
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MAXIMUM_CAPABLE_MEMORY_SPEED) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT32                              *MaximumCapableMemorySpeed
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_CONFIGURE_MEMORY_SPEED) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT32                              *ConfigureMemorySpeed
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_CONFIGURE_VOLTAGE) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT16                              *ConfigureVoltage
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MAXIMUM_VOLTAGE) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT16                              *MaximumVoltage
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_MINIMUM_VOLTAGE) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex,
  OUT  UINT16                              *MinimumVoltage
  );

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
typedef
EFI_STATUS
(EFIAPI *GET_ECC_TYPE_STRING) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  OUT  CHAR16                              **EccTypeString,
  IN   CHAR8                               *Languages  OPTIONAL
  );

/**
  Get Memory Install Status

  @param[in]   This                        Points to the H2O_MEMORY_DEVICE_INFO_PROTOCOL.
  @param[in]   MemoryInfoIndex             Which memory info index to get.

  @retval TRUE                             The memory are installed in slot.
  @retval FALSE                            The memory are not installed in slot.

**/
typedef
BOOLEAN
(EFIAPI *GET_MEMORY_INSTALL_STATUS) (
  IN   H2O_MEMORY_DEVICE_INFO_PROTOCOL     *This,
  IN   UINTN                               MemoryInfoIndex
  );

//
// H2O_MEMORY_DEVICE_INFO_PROTOCOL
//
struct _H2O_MEMORY_DEVICE_INFO_PROTOCOL {
  GET_ALL_MEMORY_DEVICE_INFO               GetAllMemoryDeviceInfo;
  GET_TOTAL_MEMORY_SIZE                    GetTotalMemorySize;
  GET_DEVICE_LOCATOR_STRING                GetDeviceLocatorString;
  GET_BANK_LOCATOR_STRING                  GetBankLocatorString;
  GET_MANUFACTURER_STRING                  GetManufacturerString;
  GET_MEMORY_TYPE_STRING                   GetMemoryTypeString;
  GET_MEMORY_TYPE_DETAIL_STRING            GetMemoryTypeDetailString;
  GET_DRAM_TYPE_STRING                     GetDramTypeString;
  GET_SERIAL_NUMBER_STRING                 GetSerialNumberString;
  GET_PART_NUMBER_STRING                   GetPartNumberString;
  GET_MEMORY_SIZE                          GetMemorySize;
  GET_MAXIMUM_CAPABLE_MEMORY_SPEED         GetMaximumCapableMemorySpeed;
  GET_CONFIGURE_MEMORY_SPEED               GetConfigureMemorySpeed;
  GET_MINIMUM_VOLTAGE                      GetMinimumVoltage;
  GET_MAXIMUM_VOLTAGE                      GetMaximumVoltage;
  GET_CONFIGURE_VOLTAGE                    GetConfigureVoltage;
  GET_ECC_TYPE_STRING                      GetEccTypeString;
  GET_MEMORY_INSTALL_STATUS                GetMemoryInstallStatus;
};

//
// Extern the GUID for protocol use.
//
extern EFI_GUID gH2OMemoryDeviceInfoProtocolGuid;

#endif

