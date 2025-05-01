/** @file
  H2O USB Viewer

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

#ifndef H2O_USB_VIEWER_DXE_H_
#define H2O_USB_VIEWER_DXE_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/UsbIo.h>
#include <Protocol/H2ODeviceInfoLookup.h>
#include <Protocol/H2OUsbTextLookup.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#include "UsbViewerSetup.h"

//
// Old versions of <Base.h> do not define UNREACHABLE().
//
#ifndef UNREACHABLE
 #ifdef __has_builtin
  #if __has_builtin (__builtin_unreachable)
   #define UNREACHABLE()  __builtin_unreachable ()
  #endif
 #else
  #define UNREACHABLE()  // No-op
 #endif
#endif

//
// H2O_BOARD_ID is a new type introduced in 05.41.40.
//
#if FixedPcdGet32 (PcdH2OCcbVersion) >= 0x05414000
#include <H2OBoardId.h>
#else
typedef UINT8 H2O_BOARD_ID;
#endif

//
// Defined in AutoGen.c
//
extern UINT8 UsbViewerDxeStrings[];
//
// Defined in auto-generated UsbViewerVfr.c (built from UsbViewerVfr.vfr)
//
extern UINT8 UsbViewerVfrBin[];

/**
**/
EFI_STATUS
GetProtocolWithCorrectStatus (
  IN EFI_HANDLE      Handle,
  IN CONST EFI_GUID  *Protocol,
  OUT VOID           **Interface OPTIONAL
  );

/**
**/
EFI_STATUS
H2OUsbViewerGetDefaultViewOption (
  OUT H2O_USB_VIEWER_VIEW_OPTION  *ViewOptionsBuffer
  );

//
// Defined in HiiConfigAccess.c
//

EFI_STATUS
EFIAPI
H2OUsbViewerExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Request,
  OUT EFI_STRING                           *Progress,
  OUT EFI_STRING                           *Results
  );

EFI_STATUS
EFIAPI
H2OUsbViewerRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  );

EFI_STATUS
EFIAPI
H2OUsbViewerFormCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN OUT EFI_IFR_TYPE_VALUE                *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  );

#define H2O_USB_VIEWER_PRIVATE_SIGNATURE  SIGNATURE_32 ('U', 'D', 'R', 'V')

//
// xHCI Extended Capability Codes
//
typedef enum {
  XhciUsbLegacySupportCapability          = 1,
  XhciSupportedProtocolCapability         = 2,
  XhciExtendedPowerManagementCapability   = 3,
  XhciIoVirtualizationCapability          = 4,
  XhciMessageInterruptCapability          = 5,
  XhciLocalMemoryCapability               = 6,
  XhciUsbDebugCapability                  = 10,
  XhciExtendedMessageInterruptCapability  = 17
} XHCI_EXTENDED_CAPABILITY_CODES;

#pragma pack (1)

typedef struct {
  UINT8  CapabilityId;
  UINT8  NextCapabilityPointer;
} XHCI_EXTENDED_CAPABILITY_HEADER;

typedef struct {
  UINT16  PsiValue:4;
  UINT16  PsiExponent:2;
  UINT16  PsiType:2;
  UINT16  PsiFullDuplex:1;
  UINT16  Reserved:5;
  UINT16  LinkProtocol:2;
  UINT16  PsiMantissa;
} XHCI_PROTOCOL_SPEED_ID;

typedef struct {
  XHCI_EXTENDED_CAPABILITY_HEADER  Header;
  UINT8                            RevisionMinor;
  UINT8                            RevisionMajor;
  UINT32                           NameString;
  UINT8                            CompatiblePortOffset;
  UINT8                            CompatiblePortCount;
  UINT16                           ProtocolDefined:12;
  UINT16                           ProtocolSpeedIdCount:4;
  UINT32                           ProtocolSlotType:5;
  UINT32                           Reserved:27;
  //
  // Variable length
  //
  // XHCI_PROTOCOL_SPEED_ID        ProtocolSpeedId[ProtocolSpeedIdCount];
} XHCI_SUPPORTED_PROTOCOL_CAPABILITY;

#pragma pack ()

#define MAX_XHCI_SUPPORTED_PROTOCOL_CAP_LENGTH  \
          (sizeof (XHCI_SUPPORTED_PROTOCOL_CAPABILITY) + \
          (15 * sizeof (XHCI_PROTOCOL_SPEED_ID)))

#define USB_HUB_MAX_PORTS              0xFF
#define USB_HUB_MAX_SUPERSPEED_PORTS   0x0F

//
// From "MdeModulePkg/Bus/Usb/UsbBusDxe/UsbHub.h"
//
#define USB_HUB_CLASS_CODE             0x09

//
// EFI_USB_HUB_DESCRIPTOR structures in various headers in MdeModulePkg are
// defined to the maximum possible length.
//
#define MAX_USB_HUB_DESCRIPTOR_LENGTH  (7 + (256 / 8) + (256 / 8))

#define USB_PORT_NUMBER_CHAIN_STRING_SIZE \
  sizeof (L"65535-255-255-255-255-255-255")

typedef enum {
  //
  // Used in the FORM_OPEN callback and the "goto" op-code handler.
  //
  H2OUsbViewerFullRefresh         = 0,
  //
  // Used when any View Option is changed (while viewing the same device).
  // Updates almost all items in the form except the title.
  //
  H2OUsbViewerViewChangeRefresh   = 1,
  //
  // Used on form's refresh event. Detects changes on the device list and calls
  // HiiUpdateForm() only when there is addition or removal of the devices.
  // Maintains question IDs of existing devices on the list and mark new
  // devices with "(New)" in the form display.
  //
  // (During form update, H2O Form Browser uses the question ID to determine
  // which form item should remain highlighted, i.e. the text cursor position.
  // If user highlights an item without a question ID, the text cursor
  // repositions to the first form item after form update.)
  //
  H2OUsbViewerDeviceChangeRefresh = 2,
  //
  // Reserved. Don't use.
  //
  H2OUsbViewerRefreshModeMaximum
} H2O_USB_VIEWER_REFRESH_MODE;

//
// Internal structure for sorting USB I/O and USB host controller handles.
//
typedef struct {
  EFI_HANDLE             Handle;
  USB_PORT_NUMBER_CHAIN  PortNumberChain;
  //
  // The first two fields of USB_HANDLE_INFO_ENTRY must have the same
  // definitions in H2O_USB_VIEWER_GOTO_OP_ENTRY.
  //
  UINT8                  InterfaceNumber;
  UINT8                  IsHub:1;
  UINT8                  HasDownstreamDevice:1;
  UINT8                  Reserved:6;
  UINT16                 GotoOpIndex;
} USB_HANDLE_INFO_ENTRY;

typedef struct {
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup;
  CONST UINT16                     *NameDbHcNumberMap;
} H2O_USB_VIEWER_SORT_CONTEXT;

typedef struct {
  UINT64  PeripheralCount;
  UINT64  HubCount;
} USB_DOWNSTREAM_DEVICE_COUNT_ENTRY;

typedef struct {
  EFI_HANDLE             Handle;
  USB_PORT_NUMBER_CHAIN  PortNumberChain;
  //
  // The first two fields of USB_HANDLE_INFO_ENTRY must have the same
  // definitions in H2O_USB_VIEWER_GOTO_OP_ENTRY.
  //
  UINT16                 PreviousDeviceGotoOpIndex;
  UINT8                  HasDownstreamDevice:1;
  UINT8                  Reserved:7;
} H2O_USB_VIEWER_GOTO_OP_ENTRY;

typedef struct {
  VOID    *PciConfigBuffer;
  UINT16  PciConfigBufferSize;
  UINT16  HciRevisionBcd;
  UINT8   HcPciInterfaceCode;
  UINT8   SerialBusReleaseBcd;
  UINT8   NumOfUsb2Ports;
  UINT8   MaxPortNumber;
  UINT8   Is64BitCapable;
} H2O_USB_VIEWER_HC_COMMON_INFO;

typedef struct {
  UINT16                     *PreferredLangIdArray;
  CHAR16                     *ManufacturerString;
  CHAR16                     *ProductString;
  CHAR16                     *SerialNumberString;
  CHAR16                     *ConfigurationString;
  EFI_USB_DEVICE_DESCRIPTOR  DeviceDescriptor;
  EFI_USB_CONFIG_DESCRIPTOR  ConfigurationDescriptor;
  UINT8                      LangIdCount;
} H2O_USB_VIEWER_USB_COMMON_DESCRIPTORS;

typedef struct {
  UINT32                             Signature;
  EFI_HII_CONFIG_ACCESS_PROTOCOL     ConfigAccess;
  EFI_STRING_ID                      UsbDeviceFormSubtitleStringId;
  EFI_STRING_ID                      DisconnectedDeviceStringId;
  EFI_STRING_ID                      DeviceCountSubtitleStringId;
  EFI_STRING_ID                      HostControllerFormTitleStringId;
  EFI_STRING_ID                      UsbFormTitleStringIds[USB_PORT_MAX_DEPTH];

  H2O_USB_VIEWER_VIEW_OPTION         ViewOption;

  EFI_HANDLE                         ServiceHandle;
  EFI_HII_HANDLE                     HiiHandle;
  H2O_USB_TEXT_LOOKUP_PROTOCOL       *UsbTextLookup;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL    *DeviceInfoLookup;
  EFI_EVENT                          FormRefreshEvent;
  EFI_IFR_STATEMENT_HEADER           *StatementStringIds;
  H2O_USB_VIEWER_GOTO_OP_ENTRY       *GotoOpEntries;
  USB_DOWNSTREAM_DEVICE_COUNT_ENTRY  *DownstreamCountEntries;

  EFI_HANDLE                         SelectedHostControllerHandle;
  EFI_HANDLE                         SelectedUsbDeviceHandleStack[USB_PORT_MAX_DEPTH];
  USB_PORT_NUMBER_CHAIN              SelectedPortNumberChain;
  USB_PORT_NUMBER_CHAIN              DisconnectedPortNumberChain;
  UINT16                             NumIfrStatementsAllocated;
  UINT16                             NumGotoOpsAllocated;
  UINT16                             LastDeviceGotoOpIndex;
  EFI_FORM_ID                        ActiveForm;
  UINT8                              FormOpenCount;

  EFI_EVENT                          SetupUtilityEvent;
} H2O_USB_VIEWER_PRIVATE_DATA;

#define H2O_USB_VIEWER_PRIVATE_FROM_CONFIG_ACCESS(a) \
  CR (a, H2O_USB_VIEWER_PRIVATE_DATA, ConfigAccess, H2O_USB_VIEWER_PRIVATE_SIGNATURE)

#define H2O_USB_VIEWER_FORM_REFRESH_TPL  TPL_CALLBACK

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH        VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

#endif // H2O_USB_VIEWER_DXE_H_
