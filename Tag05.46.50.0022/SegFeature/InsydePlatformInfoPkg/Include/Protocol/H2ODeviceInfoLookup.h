/** @file
  Routine for collecting data for Device Info Lookup Protocol.

;******************************************************************************
;* Copyright (c) 2021 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef H2O_DEVICE_INFO_LOOKUP_H_
#define H2O_DEVICE_INFO_LOOKUP_H_

#include <Uefi.h>

#include <Protocol/DevicePath.h>

#define H2O_DEVICE_INFO_LOOKUP_PROTOCOL_GUID \
  { \
    0xe92f90a6, 0x9098, 0x452d, { 0x95, 0xcd, 0x5c, 0xde, 0x3c, 0x69, 0xf9, 0xa2 } \
  }

typedef struct _H2O_DEVICE_INFO_LOOKUP_PROTOCOL  H2O_DEVICE_INFO_LOOKUP_PROTOCOL;

typedef union {
  UINT8  Uint8;
  struct {
    UINT8  Bit0Reverved:1; // Reserved for future "IsSerialPort" bit
    UINT8  IsPci:1;
    UINT8  IsUsb:1;
    UINT8  IsSata:1;
    UINT8  IsNvme:1;
    UINT8  IsSdEmmc:1;
    UINT8  Reserved:2;
  } Bits;
} H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES;

typedef struct {
  CHAR16                    *DisplayName;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  CHAR8                     *AcpiPath;
  UINT32                    PcdTokenNumber;
  UINT8                     Attributes;
} H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY;

/**

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
typedef
EFI_STATUS
(EFIAPI *H2O_LOOKUP_DEVICE_INFO_BY_DEVICE_PATH) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT UINTN                           *Attributes OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL
  );

typedef
EFI_STATUS
(EFIAPI *H2O_LOOKUP_ANCESTOR_DEVICE_INFO_OF_TYPE) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  IN OUT UINTN                        *Attributes OPTIONAL,
  OUT EFI_DEVICE_PATH_PROTOCOL        **AncestorDevicePath OPTIONAL,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL
  );

typedef
EFI_STATUS
(EFIAPI *H2O_CREATE_DEVICE_INFO_LOOKUP_NAME_TABLE) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL           *This,
  IN UINTN                                     FilterAttributes,
  OUT H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  **NameTable OPTIONAL,
  OUT UINTN                                    *NumOfDevices OPTIONAL
  );

typedef
VOID
(EFIAPI *H2O_DESTROY_DEVICE_INFO_LOOKUP_NAME_TABLE) (
  IN H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *NameTable
  );

typedef
INTN
(EFIAPI *H2O_DEVICE_INFO_DISPLAY_NAME_COMPARE) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

/**

  @retval NULL
  @retval Other

**/
typedef
CHAR8 *
(EFIAPI *H2O_DEVICE_INFO_CREATE_DISPLAY_NAME_STRING) (
  IN CONST CHAR8  *SourceString
  );

typedef
EFI_STATUS
(EFIAPI *H2O_ADD_DEVICE_INFO_LOOKUP_ENTRY) (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN UINT32                               PcdTokenNumber,
  IN UINTN                                Attributes,
  IN CONST CHAR8                          *AcpiPath OPTIONAL,
  IN CONST CHAR16                         *DisplayName OPTIONAL,
  IN CONST CHAR16                         *DisplayNameSortKey OPTIONAL
  );


typedef
EFI_STATUS
(EFIAPI *H2O_LOCK_DEVICE_INFO_LOOKUP_DATA) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

typedef
EFI_STATUS
(EFIAPI *H2O_RESET_DEVICE_INFO_LOOKUP_DATA) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

/**

  @param[in,out] This

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED

**/
typedef
EFI_STATUS
(EFIAPI *H2O_LOAD_DEVICE_INFO_LOOKUP_DATA) (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

/**

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
typedef
EFI_STATUS
(EFIAPI *H2O_GET_USB_HC_DEVICE_PATH_BY_NUMBER) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN UINT16                           HostControllerNumber,
  OUT EFI_DEVICE_PATH_PROTOCOL        **HostControllerPath
  );

#define USB_PORT_MAX_DEPTH  6

typedef struct {
  UINT16  HostControllerNumber;
  UINT8   UsbPortNumbers[USB_PORT_MAX_DEPTH];
} USB_PORT_NUMBER_CHAIN;

/**

  @param[in]  This
  @param[in]  UsbPortNumberChain
  @param[out] PcdTokenNumber
  @param[out] AcpiPath
  @param[out] DisplayName
  @param[out] DownstreamPortCount

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
typedef
EFI_STATUS
(EFIAPI *H2O_LOOKUP_USB_PORT_INFO_BY_NUMBER_CHAIN) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL,
  OUT UINT8                           *DownstreamPortCount OPTIONAL
  );

/**

**/
typedef
VOID
(EFIAPI *H2O_DEVICE_PATH_TO_USB_PORT_NUMBER_CHAIN) (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  OUT USB_PORT_NUMBER_CHAIN           *UsbPortNumberChain OPTIONAL,
  OUT UINT8                           *UsbInterfaceNumber OPTIONAL
  );

typedef
INTN
(EFIAPI *H2O_USB_PORT_NUMBER_CHAIN_COMPARE) (
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain1,
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

typedef struct {
  //
  // Read-only variables
  //
  UINT16                                    MaxHostControllerNumber;
  UINT16                                    Reserved;
  //
  // Methods
  //
  H2O_GET_USB_HC_DEVICE_PATH_BY_NUMBER      GetUsbHcDevicePathByNumber;
  H2O_LOOKUP_USB_PORT_INFO_BY_NUMBER_CHAIN  GetUsbPortInfoByNumberChain;
  H2O_DEVICE_PATH_TO_USB_PORT_NUMBER_CHAIN  DevicePathToUsbPortNumberChain;
  H2O_USB_PORT_NUMBER_CHAIN_COMPARE         UsbPortNumberChainCompare;
} H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE;

struct _H2O_DEVICE_INFO_LOOKUP_PROTOCOL {
  struct {
    UINT8  Reserved;
  } Flags;
  UINT8                                        Reserved[3];
  H2O_LOOKUP_DEVICE_INFO_BY_DEVICE_PATH        GetDeviceInfoByDevicePath;
  H2O_LOOKUP_ANCESTOR_DEVICE_INFO_OF_TYPE      GetAncestorDeviceInfoOfType;
  H2O_DEVICE_INFO_DISPLAY_NAME_COMPARE         DeviceInfoDisplayNameCompare;
  H2O_CREATE_DEVICE_INFO_LOOKUP_NAME_TABLE     CreateNameTable;
  H2O_DESTROY_DEVICE_INFO_LOOKUP_NAME_TABLE    DestroyNameTable;
  H2O_DEVICE_INFO_CREATE_DISPLAY_NAME_STRING   CreateDisplayNameString;
  H2O_ADD_DEVICE_INFO_LOOKUP_ENTRY             AddDeviceInfoLookupEntry;
  H2O_LOCK_DEVICE_INFO_LOOKUP_DATA             LockDeviceInfoLookupData;
  H2O_RESET_DEVICE_INFO_LOOKUP_DATA            ResetDeviceInfoLookupData;
  H2O_LOAD_DEVICE_INFO_LOOKUP_DATA             LoadDeviceInfoLookupData;

  //
  // Optional. If the interface is not supported then the pointer to the
  // structure is NULL.
  //
  H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE         *Usb;
};

extern EFI_GUID gH2ODeviceInfoLookupProtocolGuid;

#endif // H2O_DEVICE_INFO_LOOKUP_H_
