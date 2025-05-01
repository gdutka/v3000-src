/** @file
  DeviceInfoLookupDxe header

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

#ifndef DEVICE_INFO_LOOKUP_DXE_H_
#define DEVICE_INFO_LOOKUP_DXE_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <Protocol/H2ODeviceInfoLookup.h>

#include <Library/DebugLib.h>
#include <Library/HiiExLib.h>

#define H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT  1

#define H2O_DEVICE_INFO_LOOKUP_PRIVATE_SIGNATURE  SIGNATURE_32 ('D', 'I', 'L', 'P')

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  CHAR8                     *AcpiPath;
  UINT32                    PcdTokenNumber;
  UINT16                    DisplayNameIndex;
  UINT8                     Attributes;
} H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY;

typedef struct {
  UINT32  PcdTokenNumber;
  UINT32  AcpiName;
  UINT32  DownstreamNodeStartIndex;
  UINT8   MaxDownstreamPortNumber;
  UINT16  DisplayNameIndex;
} H2O_USB_PORT_INFO_TRIE_NODE;

typedef struct {
  UINT32                             Signature;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    Protocol;
  H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *InfoEntries;
  CHAR16                             **DisplayNameSortKeys;
  CHAR16                             **DisplayNameStrings;
  UINT16                             InfoEntryCount;
  UINT16                             DisplayNameStringCount;
  UINT8                              IsBeingWritten;
  UINT8                              IsDeviceInfoPcdLoaded;
#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  //
  // Data for USB port lookup
  //
  H2O_USB_PORT_INFO_TRIE_NODE        *UsbPortInfoTrieNodes;
  EFI_DEVICE_PATH_PROTOCOL           **UsbHcDevicePaths;
  CHAR8                              **UsbHcAcpiPaths;
  UINT32                             UsbPortInfoTrieNodeCount;
  UINT16                             UsbHostControllerCount;
#endif
  EFI_HII_HANDLE                     HiiHandle;
  PCD_STRING_NAME_TO_ID              *StringTokenNameToIds;
} H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA;

#define H2O_DEVICE_INFO_LOOKUP_PRIVATE_FROM_THIS(this) \
  CR (this, H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA, Protocol, H2O_DEVICE_INFO_LOOKUP_PRIVATE_SIGNATURE)

VOID
DebugPrintDevicePath (
  IN UINTN                           ErrorLevel,
  IN CONST CHAR8                     *FunctionName,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  );

/**

  @retval NULL
  @retval Other

**/
CHAR8 *
AsciiStrDuplicate (
  IN CONST CHAR8  *String
  );

VOID
FreePoolIfNonNull (
  IN VOID  *Buffer
  );

EFI_STATUS
EFIAPI
GetDeviceInfoByDevicePath (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT UINTN                           *Attributes OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL
  );

/**
**/
EFI_STATUS
EFIAPI
GetAncestorDeviceInfoOfType (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  IN OUT UINTN                        *Attributes OPTIONAL,
  OUT EFI_DEVICE_PATH_PROTOCOL        **AncestorDevicePath OPTIONAL,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL
  );

INTN
EFIAPI
DeviceInfoDisplayNameCompare (
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

VOID
EFIAPI
DestroyNameTable (
  IN H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *NameTable
  );

EFI_STATUS
EFIAPI
CreateNameTable (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL           *This,
  IN UINTN                                     FilterAttributes,
  OUT H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  **NameTable OPTIONAL,
  OUT UINTN                                    *NumOfDevices OPTIONAL
  );

VOID
AcquireDeviceInfoLookupDataWriteLock (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  );

/**

  @param[in,out] Buffer         A pointer to a pointer to the original buffer.
                                If the buffer would be reallocated, the pointer
                                would be updated to point to a new buffer.
                                The Buffer parameter may point to a NULL
                                pointer if NumOfElements is zero.
  @param[in]     NumOfElements  Number of elements in the original buffer.
  @param[in]     ElementSize    Size of each element in bytes in the original
                                buffer.

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
GrowBufferIfFull (
  IN OUT VOID  **Buffer,
  IN UINTN     NumOfElements,
  IN UINTN     Addend,
  IN UINTN     ElementSize
  );

VOID
ReleaseDeviceInfoLookupDataWriteLock (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  );

EFI_STATUS
EFIAPI
AddDeviceInfoLookupEntry (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN UINT32                               PcdTokenNumber,
  IN UINTN                                Attributes,
  IN CONST CHAR8                          *AcpiPath OPTIONAL,
  IN CONST CHAR16                         *DisplayName OPTIONAL,
  IN CONST CHAR16                         *DisplayNameSortKey OPTIONAL
  );

EFI_STATUS
EFIAPI
LockDeviceInfoLookupData (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

EFI_STATUS
EFIAPI
ResetDeviceInfoLookupData (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

CHAR16 *
CreateDisplayNameTextForDebug (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT16                               DisplayNameIndex
  );

/**

  @param[in,out] This

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED

**/
EFI_STATUS
EFIAPI
LoadDeviceInfoLookupData (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

/**

  @retval NULL
  @retval Other

**/
CHAR8 *
EFIAPI
CreateDisplayNameString (
  IN CONST CHAR8  *UserString
  );

/**

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
InsertDisplayNameString (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST CHAR16                             *Name,
  IN CONST CHAR16                             *NameSortKey OPTIONAL,
  OUT UINTN                                   *NameStringIndex
  );

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT

/**

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
GetUsbHcDevicePathByNumber (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN UINT16                           HostControllerNumber,
  OUT EFI_DEVICE_PATH_PROTOCOL        **HostControllerPath
  );

/**

  @implements H2O_LOOKUP_USB_PORT_INFO_BY_NUMBER_CHAIN

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
EFI_STATUS
EFIAPI
GetUsbPortInfoByNumberChain (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain,
  OUT UINT32                          *PcdTokenNumber OPTIONAL,
  OUT CHAR8                           **AcpiPath OPTIONAL,
  OUT CHAR16                          **DisplayName OPTIONAL,
  OUT UINT8                           *DownstreamPortCount OPTIONAL
  );

VOID
EFIAPI
DevicePathToUsbPortNumberChain (
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This,
  IN CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  OUT USB_PORT_NUMBER_CHAIN           *UsbPortNumberChain OPTIONAL,
  OUT UINT8                           *UsbInterfaceNumber OPTIONAL
  );

/**
  Compares two UsbPortNumberChain entries according to port names in the
  database.

  @implements H2O_USB_PORT_NUMBER_CHAIN_COMPARE

  @param[in]  UsbPortNumberChain1  The pointer to first entry.
  @param[in]  UsbPortNumberChain2  The pointer to second entry.
  @param[in]  This

  @retval 0   UsbPortNumberChain1 is equal to UsbPortNumberChain2.
  @retval <0  UsbPortNumberChain1 precedes UsbPortNumberChain2.
  @retval >0  UsbPortNumberChain1 succeeds UsbPortNumberChain2.

**/
INTN
EFIAPI
UsbPortNumberChainCompare (
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain1,
  IN CONST USB_PORT_NUMBER_CHAIN      *UsbPortNumberChain2,
  IN H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *This
  );

EFI_STATUS
GetUsbPortInfoByDevicePath (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  OUT UINT32                              *PcdTokenNumber OPTIONAL,
  OUT UINTN                               *Attributes OPTIONAL,
  OUT CHAR8                               **AcpiPath OPTIONAL,
  OUT CHAR16                              **DisplayName OPTIONAL
  );

UINT16
LookupUsbPortInfoDisplayNameIndex (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL       *DevicePath
  );

/**

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
AddUsbPortLookupEntry (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST EFI_DEVICE_PATH_PROTOCOL           *DevicePath,
  IN UINT32                                   PcdTokenNumber,
  IN UINTN                                    Attributes,
  IN CONST CHAR8                              *AcpiPath OPTIONAL,
  IN CONST CHAR16                             *DisplayName OPTIONAL,
  IN CONST CHAR16                             *DisplayNameSortKey OPTIONAL
  );

EFI_STATUS
ResetDeviceInfoUsbPortLookupData (
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  );

VOID
DebugPrintUsbPortInfoTrie (
  IN UINTN                                ErrorLevel,
  IN H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData
  );

VOID
UpdateUsbPortInfoDisplayNameIndices (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT16                                   NewNameIndex
  );

#endif // H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT

#endif // DEVICE_INFO_LOOKUP_DXE_H_
