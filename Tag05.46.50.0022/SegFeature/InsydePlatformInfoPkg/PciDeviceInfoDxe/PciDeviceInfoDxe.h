/** @file
  Header of PciDeviceInfoDxe.

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

#ifndef __PCI_DEVICE_INFO_DXE_H__
#define __PCI_DEVICE_INFO_DXE_H__

#include <Uefi.h>

#include <Protocol/H2OPciDeviceInfo.h>

#define H2O_PCI_DEVICE_INFO_PRIVATE_SIGNATURE   SIGNATURE_32 ('h', 'p', 'd', 'i')

typedef struct {
  UINT32                        Signature;
  EFI_HANDLE                    Handle;
  H2O_PCI_DEVICE_INFO_PROTOCOL  H2oPciDeviceInfo;
  LIST_ENTRY                    PciDeviceInfoPool;
} H2O_PCI_DEVICE_INFO_PRIVATE_DATA;

#define H2O_PCI_DEVICE_INFO_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, H2O_PCI_DEVICE_INFO_PRIVATE_DATA, H2oPciDeviceInfo, H2O_PCI_DEVICE_INFO_PRIVATE_SIGNATURE)

#endif