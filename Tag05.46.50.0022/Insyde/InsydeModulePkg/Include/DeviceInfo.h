/** @file
  Device Info Definition

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_

//
// pci_code_type
//
#define ATTR_NAME_PCI_CODE_TYPE                    "pci_code_type"
#define ATTR_VALUE_EFI                             "efi"
#define ATTR_VALUE_LEGACY                          "legacy"

#define ATTR_NAME_PCIE_PEI_BAR0                    "pcie_pei_bar0"
#define ATTR_NAME_PCIE_PEI_BAR1                    "pcie_pei_bar1"
#define ATTR_NAME_PCIE_PEI_BAR2                    "pcie_pei_bar2"
#define ATTR_NAME_PCIE_PEI_BAR3                    "pcie_pei_bar3"
#define ATTR_NAME_PCIE_PEI_BAR4                    "pcie_pei_bar4"
#define ATTR_NAME_PCIE_PEI_BAR5                    "pcie_pei_bar5"
#define ATTR_NAME_PCIE_PEI_ENABLE                  "pcie_pei_enable"
#define ATTR_VALUE_YES                             "yes"
#define ATTR_VALUE_NO                              "no"
#define ATTR_VALUE_CRISISRECOVERY                  "crisisrecovery"

#endif
