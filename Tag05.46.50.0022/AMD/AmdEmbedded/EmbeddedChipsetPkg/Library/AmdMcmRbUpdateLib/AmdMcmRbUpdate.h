/** @file
  Provide xHCI controller initialize function
;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _AMD_MCM_RB_UPDATE_H_
#define _AMD_MCM_RB_UPDATE_H_

#include <Base.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/AmdMcmRbUpdateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/S3BootScriptLib.h>

#define IOHC_NB_SMN_INDEX2           0xB8
#define IOHC_NB_SMN_DATA2            0xBC
#define SMN_IOMMU_BASE               0x13F00000
#define SMN_IOHCMISC_BASE            0x13B00000
#define PCI_REG_IOMMU_BASE_LO        0x44
#define PCI_REG_IOMMU_BASE_HI        0x48
#define PCI_REG_IOMMU_VF_BASE_LO     0xD0
#define PCI_REG_IOMMU_VF_BASE_HI     0xD4
#define PCI_REG_IOMMU_VF_CNT_BASE_LO 0xD8
#define PCI_REG_IOMMU_VF_CNT_BASE_HI 0xDC
#define SMN_REG_CCP_BASE_LO          0x102D8
#define SMN_REG_CCP_BASE_HI          0x102DC
#define SMN_REG_MP0_MAILBOX_BASE_LO  0x102E0
#define SMN_REG_MP0_MAILBOX_BASE_HI  0x102E4
#define SMN_REG_MP1_MAILBOX_BASE_LO  0x102E8
#define SMN_REG_MP1_MAILBOX_BASE_HI  0x102EC
#define SMN_REG_IOAPIC_BASE_LO       0x102F0
#define SMN_REG_IOAPIC_BASE_HI       0x102F4
#define SMN_REG_DEBUG_REG_BASE_LO    0x102F8
#define SMN_REG_DEBUG_REG_BASE_HI    0x102FC
#define SMN_REG_SMN_MMIO_APERTURE_LO 0x10300
#define SMN_REG_SMN_MMIO_APERTURE_HI 0x10304
#define SMN_REG_SMN_MMIO_CTRL_REG_LO 0x10308
#define SMN_REG_SMN_MMIO_CTRL_REG_HI 0x1030C

UINT32
SmnRegisterRead (
  IN UINTN         IohcBusNum,
  IN UINTN         SmnIndexValue
  );

UINT32
SmnRegisterWrite (
  IN UINTN         IohcBusNum,
  IN UINTN         SmnIndexValue,
  IN UINTN         SmnDataValue
  );

#endif
