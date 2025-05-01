/** @file
  PCI Capability Interpretation Lookup table

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _PCI_CAPABILITY_INTN_TABLE_H_
#define _PCI_CAPABILITY_INTN_TABLE_H_

#include <Uefi.h>
#include <Library/HiiLib.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/PciExpress30.h>
#include <IndustryStandard/PciExpress40.h>
#include <IndustryStandard/PciCapability.h>

#include <Protocol/H2OPciDeviceInfo.h>

#ifndef offsetof
  #define offsetof(s, m) (UINTN)&(((s *)0)->m)
#endif

typedef struct {
  UINTN           DataSize;
  UINTN           ValueOffset;
  EFI_STRING_ID   TextId;
  EFI_STRING_ID   HelpId;
} CAP_LOOKUP_TABLE;

typedef struct {
  UINT16             CapabilityId;
  UINT16              TableSize;
  CAP_LOOKUP_TABLE   *LookupTable;
} COMMON_CAP_INTERPRETATIONTABLE;

typedef struct {
  UINT16             CapabilityId;
  UINT16             TableType;
  UINT16              TableSize;
  CAP_LOOKUP_TABLE   *CapCommonTable;
  CAP_LOOKUP_TABLE   *CapDynamicTable;
  UINT16              DynamicTableSize;
} DYNAMIC_CAP_INTERPRETATIONTABLE;

#define PCI_CAP_MSI_TYPE32       0
#define PCI_CAP_MSI_TYPE64       1
#define PCI_CAP_PCIX_TYPE_DEVICE 0
#define PCI_CAP_PCIX_TYPE_BRIDGE 1


#define PCI_CAPABILITY_INTN_TABLE_STRING_PACKAGE_GUID \
  {0x179a2c35, 0x806e, 0x4235, { 0x86, 0x99, 0x1b, 0xc3, 0xc9, 0x1e, 0xb6, 0x92 } }


/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
LibPciCapabilityIntnStringInit (
  VOID
);

/**
  Create Specific Capability Interpretatin Item to SCU

  @param HiiHandle             - Target EFI_HII_HANDLE instance.
  @param CurrentPciDeviceInfo  - Current Pci device.
  @param StartOpCodeHandle     - Hii start Opcode to add new Item.
  @param CapabilityHdr         - Specific Capability Struct.
  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.
  @param Language              - Hii Supported Language.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  CapabilityHdr is NULL.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
**/
EFI_STATUS
LibCreatePciCapabilityInterpretation (
  IN       EFI_HII_HANDLE          HiiHandle,
  IN       PCI_DEVICE_INFO         *CurrentPciDeviceInfo,
  IN       VOID                    *StartOpCodeHandle,
  IN       EFI_PCI_CAPABILITY_HDR  *CapabilityHdr,
  IN  OUT  UINTN                   *InterpretationIndex,
  IN       EFI_QUESTION_ID         BaseQuestionId,
  IN       CHAR8                   *Language
);

/**
  Create Specific Capability Interpretatin Item to SCU

  @param HiiHandle             - Target EFI_HII_HANDLE instance.
  @param CurrentPciDeviceInfo  - Current Pci device.
  @param StartOpCodeHandle     - Hii start Opcode to add new Item.
  @param CapabilityHdr         - Specific Capability Struct.
  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.
  @param PciExpCap             - Pointer to PCIe Extended Capability.
  @param Language              - Hii Supported Language.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  CapabilityHdr is NULL.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
**/
EFI_STATUS
LibCreatePcieExtCapabilityInterpretation (
  IN       EFI_HII_HANDLE          HiiHandle,
  IN       PCI_DEVICE_INFO         *CurrentPciDeviceInfo,
  IN       VOID                    *StartOpCodeHandle,
  IN       PCI_EXP_EXT_HDR         *CapabilityHdr,
  IN  OUT  UINTN                   *InterpretationIndex,
  IN       EFI_QUESTION_ID         BaseQuestionId,
  IN       PCI_CAPABILITY_PCIEXP   *PciExpCap,     OPTIONAL
  IN       CHAR8                   *Language       OPTIONAL
  );

#endif
