/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _MPM_KVM_H_
#define _MPM_KVM_H_

#include <IndustryStandard/Pci22.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/SimplePointer.h>
#include <Protocol/AbsolutePointer.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>


#include <AGESA.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/MpmMboxLib.h>
#include <Library/MpmFuncLib.h>
#include <Library/MpmDevLib.h>
// To make porting more easily keep the same protocol as PSP KVM
#include <Protocol/AmdPspKvmServiceProtocol.h>


#include "MpmKvmKeyboard.h"
#include "MpmKvmMouse.h"

#define MPM_KVM_DEV_SIGNATURE       SIGNATURE_32 ('m', 'k', 'v', 'm')





//todo clean unused field
typedef struct {
  UINTN                               Signature;
  KVM_KEYBOARD_CONSOLE_IN_DEV         *MpmKvmKbDev;
  KVM_MOUSE_DEV                       *MpmKvmMsDev;
  EFI_UNICODE_STRING_TABLE            *ControllerNameTable;
  EFI_HANDLE                          *KvmDevHandle;
  EFI_PCI_IO_PROTOCOL                 *KvmDevPciIo;
  KVM_NETCARD_INFO                    NetCardInfo;

  UINT32                              NetCardMmioBase; ///< NetCard PCIE MMIO base
  UINT32                              NetCardMmioSize; ///< NetCard PCIE MMIO base
  UINT32                              KvmRequest;      ///< Kvm Request status
  UINT64                              KvmDmaAddr;      ///< Kvm Dma buffer base
  UINT64                              KvmEntryAddress; ///< Kvm binary entry address
  UINT32                              KvmEntrySize;    ///< Kvm binary size
  MPM_DEV_PROTOCOL                    *KvmDevProtocol;      ///< Kvm device protocol
} MPM_KVM_DEV;


typedef enum {
  KVM_FIELD_REQUEST,
  KVM_FIELD_SESSION
} KVM_REG_FIELD;


typedef union {
  struct {
    UINT32  VendorID:16;
    UINT32  DeviceID:16;
  } Field;
  UINT32 VendorDeviceID;
} KVM_DEVID;




#define MPM_KVM_DEV_FROM_THIS(a)  CR (a, MPM_KVM_DEV, MpmKvmKbDev, MPM_KVM_DEV_SIGNATURE)


#define AMD_RMB_PSP_VID       0x1022
#define AMD_RMB_PSP_DID       0x1649


#define KVM_UNSUPPORTED       0x0





#define KVM_ENGINE_BINARY_OFFSET (5 * 1024 * 1024)

#define KVM_ABSOLUTE_MOUSE_PROTOCOL  0x0
#define KVM_SIMPLE_MOUSE_PROTOCOL  0x1

// #define ASSERT_EFI_ERROR(x) ASSERT (!EFI_ERROR (x))

#define KVM_BUFFER_ALIGNMENT     0x8000000ul // Must be 128M aligned

#define COMMAND_REGISTER_OFFSET 0x4
#define COMMAND_REGISTER_VALUE  0x7
#define CLASS_CODE_REGISTER_OFFSET 0x8
#define HOST_BRIDGE_CLASS_CODE 0x06040000
#define SUBORDINATE_BUS_NUMBER 0x1A

extern KVM_MOUSE_DEV    mKvmMouseDev;
extern KB_RING_BUFFER   *mKbRingBuffer;
extern KVM_MOUSE_BUFFER *mMRingBuffer;


EFI_STATUS
EFIAPI
MpmKvmDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
MpmKvmDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
MpmKvmDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  );

EFI_STATUS
EFIAPI
MpmKvmComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );


EFI_STATUS
EFIAPI
MpmKvmComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  );

///Get value of KvmRequest through PCI address
EFI_STATUS
MpmKvmGetRequestPciAddr (
  IN     PCI_ADDR    PciAddr,
  IN     UINT32                *KvmRequest
  );

///Get value of KvmRequest
EFI_STATUS
MpmKvmGetRequest (
  IN     UINT32                *KvmRequest
  );

///Set value of KvmRequest
EFI_STATUS
MpmKvmSetRequest (
  IN     UINT32                *KvmRequest
  );

///Get value of KvmSessionProgress
EFI_STATUS
MpmKvmGetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  );

///Set value of KvmSessionProgress
EFI_STATUS
MpmKvmSetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  );

EFI_STATUS
MpmKvmInitiateKvm (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *ModeInfo,
  IN KVM_IP_INFO                       *KvmIpInfo
  );

EFI_STATUS
MpmKvmTerminateKvm (
  VOID
  );

EFI_STATUS
MpmKvmRetrieveSmbiosTable (
  IN    UINT8                    KvmEnabled,
  OUT   SMBIOS_TABLE_TYPEA0    **SmbiosTableTypeA0
  );




#endif


