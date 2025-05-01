/** @file
SDEmmcInfoSetupUtilityDxe's header

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __SD_EMMC_INFO_FORM_POLICY_H__
#define __SD_EMMC_INFO_FORM_POLICY_H__

#include <Guid/SDEmmcInfoSetupUtilityHii.h>

//
// Variable Store ID
//
#define SD_EMMC_INFO_VARSTORE_ID                                       0xA000

//
// Form ID definition
//
#define H2O_SD_EMMC_INFO_LIST_FORM_ID                                  0x0001
#define H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID                        0x0002
#define H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_FORM_ID                      0x0003

//
// Question ID definition
//
#define H2O_SD_EMMC_INFO_QUESTION_ID_POLICY_FORM                       (BIT13 | BIT12)
#define H2O_SD_EMMC_INFO_QUESTION_ID_POLICY_TYPE                       (BIT10 | BIT11 | BIT9 | BIT8)
#define H2O_SD_EMMC_INFO_QUESTION_ID_POLICY_SERIAL                     (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

#define H2O_SD_EMMC_INFO_UNSUPPORTED_QUESTION_ID                       0x0001
#define H2O_SD_EMMC_INFO_DISPLAY_MODE_QUESTION_ID                      0x0301

#define H20_SD_EMMC_INFO_DYNAMIC_SD_DEVICE_BASE_QUESTION_ID            0x0500
#define H20_SD_EMMC_INFO_DYNAMIC_EMMC_DEVICE_BASE_QUESTION_ID          0x0600

#define H20_SD_EMMC_INFO_DYNAMIC_SD_INFO_BASE_QUESTION_ID              0x1500
#define H20_SD_EMMC_INFO_DYNAMIC_EMMC_INFO_BASE_QUESTION_ID            0x1600

//
// Label definition
//
#define H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_START                       0x0001
#define H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_END                         0x0002
#define H2O_SD_EMMC_INFO_SD_DEVICE_INFO_LABEL_START                    0x0003
#define H2O_SD_EMMC_INFO_SD_DEVICE_INFO_LABEL_END                      0x0004
#define H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_LABEL_START                  0x0005
#define H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_LABEL_END                    0x0006


#endif
