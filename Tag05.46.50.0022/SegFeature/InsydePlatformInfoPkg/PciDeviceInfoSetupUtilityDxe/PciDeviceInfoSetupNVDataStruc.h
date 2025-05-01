/** @file
  Define NVData structures used by the PCI Device Information Setup Utility.

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

#ifndef __PCI_DEVICE_INFO_SETUP_NVDATA_STRUC_H__
#define __PCI_DEVICE_INFO_SETUP_NVDATA_STRUC_H__

#include <Guid/PciDeviceInfoSetupUtilityHii.h>

//
// +------------------+----------------+-----------------+---------------------------+
// | Bit 15-13        | Bit 12         | Bit 11-10       | Bit 9-0                   |
// | ID Type          | Form ID        | Sub Type        | Serial                    |
// +------------------+----------------+-----------------+---------------------------+
// | Form ID     (1h) |                                    Root Form     (0h)        |
// |                  |                                    Sub Form      (1h)        |
// | Varstore ID (2h) |                                    Configuration (0h)        |
// | Question ID (3h) | Root Form (0h) | (Static)   (0h) | (Static)      (000h-3FFh) |
// |                                   | Device     (1h) | Unique ID     (000h-3FFh) |
// |                  | Sub Form  (1h) | (Static)   (0h) | (Static)      (000h-3FFh) |
// |                                   | PCI Cap    (1h) | Index         (000h-3FFh) |
// |                                   | PCIe Cap   (2h) | Index         (000h-3FFh) |
// | Label ID    (4h) | Root Form (0h) |                   Device        (0h)        |
// |                  | Sub Form  (1h) |                   PCI Cap       (0h)        |
// |                                   |                   PCIe Cap      (1h)        |
// |                  |                                    End           (1FFFh)     |
// +------------------+----------------+-----------------+---------------------------+
//

//
// Policy define
//
#define B_SETUP_POLICY_ID_TYPE                        (BIT15 | BIT14 | BIT13)
#define N_SETUP_POLICY_ID_TYPE                        13
#define B_SETUP_POLICY_FORM_ID                        (BIT12)
#define N_SETUP_POLICY_FORM_ID                        12
#define B_SETUP_POLICY_SUB_TYPE                       (BIT11 | BIT10)
#define N_SETUP_POLICY_SUB_TYPE                       10
#define B_SETUP_POLICY_SERIAL                         (BIT9 | BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define N_SETUP_POLICY_SERIAL                         0

#define B_SETUP_POLICY_DYNAMIC_BASE                   (B_SETUP_POLICY_ID_TYPE | B_SETUP_POLICY_FORM_ID | B_SETUP_POLICY_SUB_TYPE)

//
// Form Id define
//
// H2O_PCI_DEVICE_INFO_ROOT_FORM_ID                                   0x2000  // Referenced from PciDeviceInfoSetupUtilityHii.h
#define H2O_PCI_DEVICE_INFO_DEVICE_INFO_FORM_ID                       0x2001

//
// Varstore define
//
//  PCI_DEVICE_INFO_CONFIGURATION_VARSTORE_ID                         0x4000  // Referenced from PciDeviceInfoSetupUtilityHii.h

//
// Question Id define
//
#define H2O_PCI_DEVICE_INFO_SHOW_ACTUAL_DEVICE_NAME_QUESTION_ID       0x6000
#define H2O_PCI_DEVICE_INFO_SHOW_DEVICE_TREE_QUESTION_ID              0x6001
#define H2O_PCI_DEVICE_INFO_FILTERING_MODE_QUESTION_ID                0x6002
#define H2O_PCI_DEVICE_INFO_FORM_REFRESH_QUESTION_ID                  0x6003
#define H2O_PCI_DEVICE_INFO_DYNAMIC_DEVICE_BASE_QUESTION_ID           0x6400  // Valid from 0x6400 to 0x67FF, supports up to 1024 devices.
#define H2O_PCI_DEVICE_INFO_DYNAMIC_PCI_CAPABILITY_BASE_QUESTION_ID   0x7400  // Valid from 0x7400 to 0x77FF.
#define H2O_PCI_DEVICE_INFO_DYNAMIC_PCIE_CAPABILITY_BASE_QUESTION_ID  0x7800  // Valid from 0x7800 to 0x7BFF.
#define H2O_PCI_DEVICE_INFO_DYNAMIC_CAPABILITY_INFO_BASE_QUESTION_ID  0x7C00  // Valid from 0x7C00 to 0x7FFF.


#define H2O_PCI_DEVICE_INFO_DEVICE_PATH_QUESTION_ID                   0x7000
#define H2O_PCI_DEVICE_INFO_PCI_LOCATION_QUESTION_ID                  0x7001
#define H2O_PCI_DEVICE_INFO_SLOT_NAME_QUESTION_ID                     0x7002
#define H2O_PCI_DEVICE_INFO_VENDOR_ID_QUESTION_ID                     0x7003
#define H2O_PCI_DEVICE_INFO_DEVICE_ID_QUESTION_ID                     0x7004
#define H2O_PCI_DEVICE_INFO_HEADER_TYPE_QUESTION_ID                   0x7005
#define H2O_PCI_DEVICE_INFO_CLASS_CODE_QUESTION_ID                    0x7006
#define H2O_PCI_DEVICE_INFO_MAX_LINK_WIDTH_QUESTION_ID                0x7007
#define H2O_PCI_DEVICE_INFO_CURRENT_LINK_WIDTH_QUESTION_ID            0x7008
#define H2O_PCI_DEVICE_INFO_MAX_LINK_SPEED_QUESTION_ID                0x7009
#define H2O_PCI_DEVICE_INFO_CURRENT_LINK_SPEED_QUESTION_ID            0x7010
#define H2O_PCI_DEVICE_INFO_DISPLAY_CAPABILITY_INTPN_QUESTION_ID      0x7011

//
// Label define
//
#define H2O_PCI_DEVICE_INFO_DEVICE_LIST_LABEL_START                   0x8000
#define H2O_PCI_DEVICE_INFO_DEVICE_LIST_LABEL_END                     0x9FFF

#define H2O_PCI_DEVICE_INFO_PCI_CAPABILITY_LIST_LABEL_START           0x9000
#define H2O_PCI_DEVICE_INFO_PCI_CAPABILITY_LIST_LABEL_END             0x9FFF

#define H2O_PCI_DEVICE_INFO_PCIE_CAPABILITY_LIST_LABEL_START          0x9001
#define H2O_PCI_DEVICE_INFO_PCIE_CAPABILITY_LIST_LABEL_END            0x9FFF

#endif