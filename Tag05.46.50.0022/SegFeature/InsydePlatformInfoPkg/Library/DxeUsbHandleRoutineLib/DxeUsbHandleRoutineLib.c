/** @file
  DXE USB handle auxiliary routines.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <IndustryStandard/Usb.h>

#include <Protocol/DevicePath.h>
#include <Protocol/UsbIo.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DeviceInfoLookupDataLib.h>   // InsydePlatformInfoPkg

#include "UsbHandleRoutineLibInternal.h"

/**
  Returns an array of handles containing the matching USB class code.

  @param[in]  DescriptorType  USB Descriptor type that the class code should
                              match against. The only valid values are
                              USB_DESC_TYPE_DEVICE (1) and
                              USB_DESC_TYPE_INTERFACE (4).
                              INTERFACE_ASSOCIATION (11) is not yet supported.
  @param[in]  Class           The base class code to match. Specifying a
                              negative value here matches all classes.
  @param[in]  SubClass        The subclass code to match. Specifying a
                              negative value here matches all subclasses.
  @param[in]  Protocol        The USB Protocol code to match. Specifying a
                              negative value here matches all protocol codes.
  @param[out] NoHandles       The number of handles returned in Buffer.
  @param[out] Buffer          A pointer to the buffer to return the requested
                              array of handles. It is the caller's
                              responsibility to call FreePool() when the caller
                              no longer requires the contents of Buffer.

  @retval EFI_SUCCESS           The array of handles was returned in Buffer,
                                and the number of handles in Buffer was
                                returned in NoHandles.
  @retval EFI_NOT_FOUND         No handles match the search.
  @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the
                                matching results.
  @retval EFI_INVALID_PARAMETER NoHandles is NULL.
  @retval EFI_INVALID_PARAMETER Buffer is NULL.
  @retval EFI_INVALID_PARAMETER Class, SubClass or Protocol contains a value
                                greater than what's permitted in USB
                                Specification (0xFF).

**/
EFI_STATUS
EFIAPI
LocateUsbHandlesByClassCode (
  IN UINT8        DescriptorType,
  IN INTN         Class,
  IN INTN         SubClass,
  IN INTN         Protocol,
  OUT UINTN       *NoHandles,
  OUT EFI_HANDLE  **Buffer
  )
{
  EFI_TPL                       OldTpl;
  EFI_STATUS                    Status;
  UINTN                         AllHandleCount;
  UINTN                         Index;
  EFI_STATUS                    WarnStatus;
  EFI_USB_IO_PROTOCOL           *UsbIo;
  EFI_USB_DEVICE_DESCRIPTOR     DeviceDescriptor;
  EFI_USB_INTERFACE_DESCRIPTOR  InterfaceDescriptor;

  if (NoHandles == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (DescriptorType == USB_DESC_TYPE_DEVICE_QUALIFIER) {
    DescriptorType = USB_DESC_TYPE_DEVICE;
  }
  if (DescriptorType != USB_DESC_TYPE_DEVICE &&
      DescriptorType != USB_DESC_TYPE_INTERFACE) {
    return EFI_INVALID_PARAMETER;
  }
  if (Class > 0xFF || SubClass > 0xFF || Protocol > 0xFF) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Raise TPL to block hotplug events and prevent uninstallation of any
  // protocol. A potential use-after-free could happen if we don't do this.
  //
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  (*NoHandles)   = 0;
  AllHandleCount = 0;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiUsbIoProtocolGuid,
                  NULL, // SearchKey
                  &AllHandleCount,
                  Buffer
                  );
  if (AllHandleCount <= 0) {
    Status = EFI_NOT_FOUND;
  }
  if (EFI_ERROR (Status)) {
    goto LocateHandleFail;
  }

  for (Index = 0; Index < AllHandleCount; Index++) {
    WarnStatus = gBS->OpenProtocol (
                        (*Buffer)[Index],
                        (EFI_GUID *) &gEfiUsbIoProtocolGuid,
                        (VOID **) &UsbIo,
                        gImageHandle, // AgentHandle
                        NULL,         // ControllerHandle
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL
                        );
    ASSERT_EFI_ERROR (WarnStatus);

    if (DescriptorType == USB_DESC_TYPE_DEVICE) {
      WarnStatus = UsbIo->UsbGetDeviceDescriptor (
                            UsbIo,
                            &DeviceDescriptor
                            );
      if (EFI_ERROR (WarnStatus)) {
        continue;
      }
      if (Class >= 0 && Class != DeviceDescriptor.DeviceClass) {
        continue;
      }
      if (SubClass >= 0 && SubClass != DeviceDescriptor.DeviceSubClass) {
        continue;
      }
      if (Protocol >= 0 && Protocol != DeviceDescriptor.DeviceProtocol) {
        continue;
      }
    } else {
      ASSERT (DescriptorType == USB_DESC_TYPE_INTERFACE);
      WarnStatus = UsbIo->UsbGetInterfaceDescriptor (
                            UsbIo,
                            &InterfaceDescriptor
                            );
      if (EFI_ERROR (WarnStatus)) {
        continue;
      }
      if (Class >= 0 && Class != InterfaceDescriptor.InterfaceClass) {
        continue;
      }
      if (SubClass >= 0 && SubClass != InterfaceDescriptor.InterfaceSubClass) {
        continue;
      }
      if (Protocol >= 0 && Protocol != InterfaceDescriptor.InterfaceProtocol) {
        continue;
      }
    }

    (*Buffer)[(*NoHandles)++] = (*Buffer)[Index];
  }

  if (*NoHandles <= 0) {
    Status = EFI_NOT_FOUND;
  }

LocateHandleFail:
  gBS->RestoreTPL (OldTpl);

  return Status;
}

/**

  @param[in]  Handle

**/
BOOLEAN
IsUsbHubHandle (
  IN EFI_HANDLE  Handle
  )
{
  EFI_TPL                    OldTpl;
  EFI_STATUS                 Status;
  EFI_USB_IO_PROTOCOL        *UsbIo;
  EFI_USB_DEVICE_DESCRIPTOR  UsbDeviceDescriptor = {0};

  //
  // Raise TPL to block hotplug events and prevent uninstallation of any
  // protocol. A potential use-after-free could happen if we don't do this.
  //
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = gBS->OpenProtocol (
                  Handle,
                  &gEfiUsbIoProtocolGuid,
                  (VOID **) &UsbIo,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = UsbIo->UsbGetDeviceDescriptor (UsbIo, &UsbDeviceDescriptor);
  }
  gBS->RestoreTPL (OldTpl);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (UsbDeviceDescriptor.DeviceClass == USB_HUB_CLASS_CODE) {
    if (UsbDeviceDescriptor.DeviceSubClass != USB_HUB_SUBCLASS_CODE) {
      DEBUG ((
        EFI_D_WARN,
        "Handle %p: Using a reserved USB hub subclass code (%02Xh).\n",
        Handle,
        (unsigned int) UsbDeviceDescriptor.DeviceSubClass
        ));
    }
    return TRUE;
  }
  return FALSE;
}

/**

  @param[in]  UsbIo
  @param[in]  UsbSpecVersion
  @param[out] HubDescriptor
  @param[out] TransferResult

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_TIMEOUT
  @retval EFI_DEVICE_ERROR

**/
EFI_STATUS
GetUsbHubDescriptor (
  IN EFI_USB_IO_PROTOCOL  *UsbIo,
  IN UINT16               UsbSpecVersion OPTIONAL,
  OUT VOID                **HubDescriptor,
  OUT UINT32              *TransferResult
  )
{
  BOOLEAN                 MayHaveSuperSpeed;
  EFI_USB_DEVICE_REQUEST  Request;
  EFI_STATUS              Status;
  UINT8                   Buffer[USB_DESC_MAX_LENGTH];

  if ((UsbIo == NULL) || (HubDescriptor == NULL) || (TransferResult == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UsbIo/HubDescriptor/TransferResult is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  MayHaveSuperSpeed = (UsbSpecVersion == 0x0000 || UsbSpecVersion >= 0x0300);

  Request.RequestType = (USB_DEV_GET_DESCRIPTOR_REQ_TYPE | USB_REQ_TYPE_CLASS);
  Request.Request     = USB_REQ_GET_DESCRIPTOR;
  Request.Index       = 0;

  *HubDescriptor = NULL;
  if (MayHaveSuperSpeed) {
    Request.Value = ((USB_DESC_TYPE_HUB_SUPER_SPEED << 8) | 0);
    Request.Length = 12;

    *HubDescriptor = AllocatePool (Request.Length);
    if (*HubDescriptor == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = UsbIo->UsbControlTransfer (
                      UsbIo,
                      &Request,
                      EfiUsbDataIn,
                      PcdGet32 (PcdUsbTransferTimeoutValue),
                      *HubDescriptor,
                      (UINTN) Request.Length,
                      TransferResult
                      );
    if (!EFI_ERROR (Status)) {
      return Status;
    }
    FreePool (*HubDescriptor);
    *HubDescriptor = NULL;
    if (Status == EFI_DEVICE_ERROR &&
        ((*TransferResult) & EFI_USB_ERR_STALL) == 0)
    {
      return Status;
    }
  }

  //
  // Get the length of the descriptor.
  //
  Request.Value = ((USB_DESC_TYPE_HUB << 8) | 0);
  Request.Length = (UINT16) sizeof (USB_DESC_HEAD);

  Status = UsbIo->UsbControlTransfer (
                    UsbIo,
                    &Request,
                    EfiUsbDataIn,
                    PcdGet32 (PcdUsbTransferTimeoutValue),
                    Buffer,
                    (UINTN) Request.Length,
                    TransferResult
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the descriptor data.
  //
  Request.Length = ((USB_DESC_HEAD *) Buffer)->Len;

  Status = UsbIo->UsbControlTransfer (
                    UsbIo,
                    &Request,
                    EfiUsbDataIn,
                    PcdGet32 (PcdUsbTransferTimeoutValue),
                    Buffer,
                    (UINTN) Request.Length,
                    TransferResult
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *HubDescriptor = AllocatePool (Request.Length);
  if (*HubDescriptor == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (*HubDescriptor, Buffer, Request.Length);
  return Status;
}

/**

  @param[in]  DevicePath
  @param[out] UsbDevicePathOffset
  @param[out] UsbDevicePathLength

**/
VOID
FindUsbDevicePathNodeAndLength (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINTN                          *UsbDevicePathOffset,
  OUT UINTN                          *UsbDevicePathLength OPTIONAL
  )
{
  EFI_STATUS                      Status;
  CONST EFI_DEVICE_PATH_PROTOCOL  *FirstUsbDevicePathNode;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  UINT8                           Depth;
  INTN                            PtrDiff;

  if ((DevicePath == NULL) || (UsbDevicePathOffset == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DevicePath/UsbDevicePathOffset is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // Note: We can't tolerate bad length of a USB Device Path node.
  //
  Status = FindDevicePathNodeByType (
             DevicePath,
             MESSAGING_DEVICE_PATH,
             MSG_USB_DP,
             (UINT16) sizeof (USB_DEVICE_PATH),
             UsbDevicePathOffset
             );
  if (EFI_ERROR (Status)) {
    if (UsbDevicePathLength != NULL) {
      *UsbDevicePathLength = 0;
    }
    return;
  }

  if (UsbDevicePathLength == NULL) {
    return;
  }

  DevicePathNode = (CONST EFI_DEVICE_PATH_PROTOCOL *) (
                     (CONST UINT8 *) DevicePath + (*UsbDevicePathOffset));
  FirstUsbDevicePathNode = DevicePathNode;
  for (Depth = 0; Depth < USB_PORT_MAX_DEPTH; Depth++) {
    if (DevicePathType (DevicePathNode) != MESSAGING_DEVICE_PATH ||
        DevicePathSubType (DevicePathNode) != MSG_USB_DP ||
        DevicePathNodeLength (DevicePathNode) != sizeof (USB_DEVICE_PATH))
    {
      break;
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
  PtrDiff = ((CONST UINT8 *) DevicePathNode -
              (CONST UINT8 *) FirstUsbDevicePathNode);
  ASSERT (PtrDiff > 0);
  *UsbDevicePathLength = (UINTN) PtrDiff;
  return;
}

/**

  @param[in]  UsbDeviceHandle
  @param[out] NumUsbInterfaces
  @param[out] UsbInterfaceHandles

  @retval  EFI_SUCCESS
  @retval  EFI_UNSUPPORTED       UsbDeviceHandle does not support Device Path
                                 Protocol.
  @retval  EFI_NOT_FOUND
  @retval  EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
LocateUsbInterfaceHandlesOfDevice (
  IN EFI_HANDLE   UsbDeviceHandle,
  OUT UINT8       *NumUsbInterfaces OPTIONAL,
  OUT EFI_HANDLE  **UsbInterfaceHandles OPTIONAL
  )
{
  UINT8                           InterfaceCount;
  EFI_HANDLE                      InterfaceHandles[MAX_UINT8] = {NULL};
  EFI_TPL                         OldTpl;
  EFI_STATUS                      ReturnStatus;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathSearchKey;
  UINTN                           UsbDevicePathOffset;
  UINTN                           UsbDevicePathLength;
  UINTN                           DevicePathSearchKeySize;
  UINTN                           HandleCount;
  EFI_HANDLE                      *HandleBuffer;
  UINTN                           Index;
  EFI_STATUS                      Status;
  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  INTN                            CompareResult;
  UINT8                           InterfaceNumber;

  ASSERT (UsbDeviceHandle != NULL);

  InterfaceCount = 0;

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  //
  // Get device path.
  //
  ReturnStatus = gBS->OpenProtocol (
                        UsbDeviceHandle,
                        &gEfiDevicePathProtocolGuid,
                        (VOID **) &DevicePathSearchKey,
                        gImageHandle, // AgentHandle
                        NULL,         // ControllerHandle
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL
                        );
  if (ReturnStatus == EFI_INVALID_PARAMETER) {
    //
    // If the handle is destroyed before OpenProtocol() can open it, the
    // function returns EFI_INVALID_PARAMETER. This behavior is unspecified in
    // UEFI specification but seen in EDK II's implementation (MdeModulePkg).
    // Change to a status that makes more sense in DEBUG() output.
    //
    ReturnStatus = EFI_UNSUPPORTED;
  }
  if (EFI_ERROR (ReturnStatus)) {
    goto NoDevicePath;
  }
  FindUsbDevicePathNodeAndLength (
    DevicePathSearchKey,
    &UsbDevicePathOffset,
    &UsbDevicePathLength
    );
  ASSERT (UsbDevicePathLength >= sizeof (USB_DEVICE_PATH));
  DevicePathSearchKeySize =
    UsbDevicePathOffset + UsbDevicePathLength - sizeof (USB_DEVICE_PATH) +
    OFFSET_OF (USB_DEVICE_PATH, InterfaceNumber);

  //
  // Locate USB interface handles with the specified Device Path search key.
  //
  HandleCount = 0;
  ReturnStatus = gBS->LocateHandleBuffer (
                        ByProtocol,
                        &gEfiUsbIoProtocolGuid,
                        NULL, // SearchKey
                        &HandleCount,
                        &HandleBuffer
                        );
  if (HandleCount <= 0) {
    ReturnStatus = EFI_NOT_FOUND;
  }
  if (EFI_ERROR (ReturnStatus)) {
    goto LocateHandleFail;
  }
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->OpenProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &DevicePath,
                    gImageHandle, // AgentHandle
                    NULL,         // ControllerHandle
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_WARN,
        "Handle %p: cannot open EFI Device Path Protocol: %r\n",
        HandleBuffer[Index],
        Status
        ));
      continue;
    }
    //
    // DevicePath might be shorter than DevicePathSearchKey.
    // CompareMem from BaseMemoryLib reads no further than the first mismatched
    // byte - a behaviour not required by C standard memcmp().
    //
    CompareResult = CompareMem (
                      DevicePath,
                      DevicePathSearchKey,
                      DevicePathSearchKeySize
                      );
    if (CompareResult != 0) {
      continue;
    }
    InterfaceNumber = *((CONST UINT8 *) DevicePath + DevicePathSearchKeySize);
    if (InterfaceNumber >= MAX_UINT8) {
      DEBUG ((
        EFI_D_WARN,
        "Handle %p: USB Interface number in EFI Device Path is invalid.\n",
        HandleBuffer[Index]
        ));
      continue;
    }
    if (InterfaceNumber + 1 > InterfaceCount) {
      InterfaceCount = (UINT8) (InterfaceNumber + 1);
    }
    if (InterfaceHandles[InterfaceNumber] != NULL) {
      DEBUG ((
        EFI_D_WARN,
        "USB I/O of handles %p and %p refer to the same Interface.\n",
        HandleBuffer[Index],
        InterfaceHandles[InterfaceNumber]
        ));
    }
    InterfaceHandles[InterfaceNumber] = HandleBuffer[Index];
  }
  FreePool (HandleBuffer);

LocateHandleFail:
NoDevicePath:
  gBS->RestoreTPL (OldTpl);

  if (NumUsbInterfaces != NULL) {
    *NumUsbInterfaces = InterfaceCount;
  }
  if (UsbInterfaceHandles != NULL) {
    *UsbInterfaceHandles = NULL;
    if (InterfaceCount > 0) {
      *UsbInterfaceHandles = AllocateCopyPool (
                               InterfaceCount * sizeof(*InterfaceHandles),
                               InterfaceHandles
                               );
      if (*UsbInterfaceHandles == NULL) {
        ReturnStatus = EFI_OUT_OF_RESOURCES;
      }
    }
  }

  return ReturnStatus;
}
