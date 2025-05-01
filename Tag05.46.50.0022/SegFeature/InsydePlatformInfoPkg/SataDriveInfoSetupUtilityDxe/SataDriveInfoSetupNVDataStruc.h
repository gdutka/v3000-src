/** @file
  Define NVData structures used by the SATA Drive Information Setup Utility.

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

#ifndef __SATA_DRIVE_INFO_SETUP_NVDATA_STRUC_H__
#define __SATA_DRIVE_INFO_SETUP_NVDATA_STRUC_H__

#include <Guid/SataDriveInfoSetupUtilityHii.h>

//
// +------------------+----------------+-----------------+-------------------------+
// | Bit 15-13        | Bit 12-10      | Bit 9-8         | Bit 7-0                 |
// | ID Type          | Form ID        | Sub Type        | Serial                  |
// +------------------+----------------+-----------------+-------------------------+
// | Form ID     (1h) |                                    Root Form     (0h)      |
// |                  |                                    Sub Form      (1h)      |
// | Varstore ID (2h) |                                    Configuration (0h)      |
// | Question ID (3h) | Root Form (0h) | (Static)   (0h) | (Static)      (00h-FFh) |
// |                                   | Controller (1h) | Serial Number (00h-FFh) |
// |                                   | Drive      (2h) | Serial Number (00h-FFh) |
// |                                   | Empty Port (3h) | Index         (00h-FFh) |
// |                  | Sub Form  (1h) | (Static)   (0h) | (Static)      (00h-FFh) |
// |                                   | Attribute  (2h) | Index         (00h-1Dh) |
// | Label ID    (4h) | Root Form (0h) |                   Port          (0h)      |
// |                  | Sub Form  (1h) |                   Attribute     (0h)      |
// |                  |                                    End           (1FFFh)   |
// +------------------+----------------+-----------------+-------------------------+
//

//
// Policy define
//
#define B_SETUP_POLICY_ID_TYPE                        (BIT15 | BIT14 | BIT13)
#define N_SETUP_POLICY_ID_TYPE                        13
#define B_SETUP_POLICY_FORM_ID                        (BIT12 | BIT11 | BIT10)
#define N_SETUP_POLICY_FORM_ID                        10
#define B_SETUP_POLICY_SUB_TYPE                       (BIT9 | BIT8)
#define N_SETUP_POLICY_SUB_TYPE                       8
#define B_SETUP_POLICY_SERIAL                         (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define N_SETUP_POLICY_SERIAL                         0

#define B_SETUP_POLICY_DYNAMIC_BASE                   (B_SETUP_POLICY_ID_TYPE | B_SETUP_POLICY_FORM_ID | B_SETUP_POLICY_SUB_TYPE)

//
// Form Id define
//
//  H2O_SATA_DRIVE_INFO_ROOT_FORM_ID                                  0x2000  // Referenced from SataDriveInfoSetupUtilityHii.c
#define H2O_SATA_DRIVE_INFO_DRIVE_INFO_FORM_ID                        0x2001

//
// Varstore define
//
//  SATA_DRIVE_INFO_CONFIGURATION_VARSTORE_ID                         0x4000  // Referenced from SataDriveInfoSetupUtilityHii.c

//
// Question Id define
//
#define H2O_SATA_DRIVE_INFO_HIDE_EMPTY_PORTS_QUESTION_ID              0x6000
#define H2O_SATA_DRIVE_INFO_DISPLAY_MODE_QUESTION_ID                  0x6001
#define H2O_SATA_DRIVE_INFO_NUMBER_OF_PHYSICAL_PORTS_QUESTION_ID      0x6002
#define H2O_SATA_DRIVE_INFO_FORM_REFRESH_QUESTION_ID                  0x6003
#define H2O_SATA_DRIVE_INFO_DYNAMIC_CONTROLLER_BASE_QUESTION_ID       0x6100
#define H2O_SATA_DRIVE_INFO_DYNAMIC_DRIVE_BASE_QUESTION_ID            0x6200
#define H2O_SATA_DRIVE_INFO_DYNAMIC_PORT_BASE_QUESTION_ID             0x6300

#define H2O_SATA_DRIVE_INFO_DEVICE_PATH_QUESTION_ID                   0x6400
#define H2O_SATA_DRIVE_INFO_PARENT_DEVICE_PATH_QUESTION_ID            0x6401
#define H2O_SATA_DRIVE_INFO_SERIAL_NUMBER_QUESTION_ID                 0x6402
#define H2O_SATA_DRIVE_INFO_CAPACITY_QUESTION_ID                      0x6403
#define H2O_SATA_DRIVE_INFO_FORM_FACTOR_QUESTION_ID                   0x6404
#define H2O_SATA_DRIVE_INFO_ROTATION_RATE_QUESTION_ID                 0x6405
#define H2O_SATA_DRIVE_INFO_SMART_SUPPORTED_QUESTION_ID               0x6406
#define H2O_SATA_DRIVE_INFO_SMART_ENABLED_QUESTION_ID                 0x6407
#define H2O_SATA_DRIVE_INFO_SMART_STATUS_QUESTION_ID                  0x6408
#define H2O_SATA_DRIVE_INFO_DYNAMIC_SMART_ATTRIBUTE_BASE_QUESTION_ID  0x6600

//
// Label define
//
#define H2O_SATA_DRIVE_INFO_DRIVE_LIST_LABEL_START                    0x8000
#define H2O_SATA_DRIVE_INFO_DRIVE_LIST_LABEL_END                      0x9FFF
#define H2O_SATA_DRIVE_INFO_SMART_ATTRIB_LIST_LABEL_START             0x8400
#define H2O_SATA_DRIVE_INFO_SMART_ATTRIB_LIST_LABEL_END               0x9FFF

#endif