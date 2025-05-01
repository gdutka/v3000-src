/** @file
  Header file of H2OPciDevcieInfo

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __H2O_PCI_DEVICE_INFO_H__
#define __H2O_PCI_DEVICE_INFO_H__

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>

#include <IndustryStandard/Pci.h>

//
// Definition of the PciDeviceInfo
//
typedef struct _PCI_DEVICE_INFO PCI_DEVICE_INFO;

#define PCI_DEVICE_INFO_SIGNATURE SIGNATURE_32 ('p', 'c', 'i', 'd')

struct _PCI_DEVICE_INFO {
  UINT32                          Signature;
  LIST_ENTRY                      Link;
  EFI_HANDLE                      DeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINT64                          BaseAddress;
  PCI_TYPE00                      PciConfigSpace;
  UINT8                           PcieCapOffset;
  PCI_CAPABILITY_PCIEXP           PcieCap;
  BOOLEAN                         IsRootBridge;
  BOOLEAN                         IsBridge;
  PCI_DEVICE_INFO                 *Parent;
  LIST_ENTRY                      ChildLink;
  LIST_ENTRY                      ChildList;
  UINT32                          SerialNumber;
  CHAR16                          *SlotNameStr;
};

#define PCI_DEVICE_INFO_FROM_LINK(a) \
  CR (a, PCI_DEVICE_INFO, Link, PCI_DEVICE_INFO_SIGNATURE)

#define PCI_DEVICE_INFO_FROM_CHILD_LINK(a) \
  CR (a, PCI_DEVICE_INFO, ChildLink, PCI_DEVICE_INFO_SIGNATURE)

//
// Definition of the H2oPciDeviceInfo Protocol
//
typedef struct _H2O_PCI_DEVICE_INFO_PROTOCOL H2O_PCI_DEVICE_INFO_PROTOCOL;

/**
  Get the PciDeviceInfoPool.

  @param[in]    This                The pointer to the instance of H2oPciDeviceInfo Protocol.
  @param[out]   PciDeviceInfoPool   The pointer to PciDeviceInfoPool.

  @retval EFI_SUCCESS             Successfully get the PciDeviceInfoPool.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.

**/
typedef
EFI_STATUS
(EFIAPI *H2O_PCI_DEVICE_INFO_GET_PCI_DEVICE_INFO_POOL) (
  IN  H2O_PCI_DEVICE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **PciDeviceInfoPool
  );

struct _H2O_PCI_DEVICE_INFO_PROTOCOL {
  UINT32                                        Size;
  H2O_PCI_DEVICE_INFO_GET_PCI_DEVICE_INFO_POOL  GetPciDeviceInfoPool;
};

// {9c046d19-8381-4e23-9310-a3646b98335d}
#define H2O_PCI_DEVICE_INFO_PROTOCOL_GUID \
  { \
    0x9c046d19, 0x8381, 0x4e23, { 0x93, 0x10, 0xa3, 0x64, 0x6b, 0x98, 0x33, 0x5d } \
  }

extern EFI_GUID gH2OPciDeviceInfoProtocolGuid;

#define SEGMENT_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS(Address) \
  (RShiftU64 ((Address), 32) & 0xffff)

#define BUS_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS(Address) \
  (((Address) >> 20) & 0xff)

#define DEVICE_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS(Address) \
  (((Address) >> 15) & 0x1f)

#define FUNCTION_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS(Address) \
  (((Address) >> 12) & 0x07)

#define REGISTER_FROM_PCI_SEGMENT_LIB_ADDRESS(Address) \
  ((Address) & 0xfff)

#endif