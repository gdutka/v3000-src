/** @file
  PCI Capability Lookup table

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

#include <Library/PciCapabilityIntnTableLib.h>

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
LibPciCapabilityIntnStringInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

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
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  return EFI_UNSUPPORTED;
}