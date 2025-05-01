/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_PSP_INTRUSION_DETECTION_COMMON_H_
#define _AMD_PSP_INTRUSION_DETECTION_COMMON_H_

#define AMD_INTRUSION_DETECTION_VARIABLE_NAME  L"INTRUSION_VARIABLE"
EFI_GUID mAmdPspIntrusionDetectionFlagGuid = {
           0x83477443, 0xCAA4, 0x46D3, {0x9B, 0x0C, 0x68, 0x3B, 0xF0, 0x3F, 0x95, 0x96 }};
#define INTRUSION_EVENT_RECORDED    0x1
#define INTRUSION_TRIGGER_RESET     0x2

typedef enum {
  INTRUSION_VARIABLE_INITIALIZED                = 0x0,
  INTRUSION_VARIABLE_CLEAR_TPM_IN_PROGRESS      = 0x1,           // BIT0
  INTRUSION_VARIABLE_NOT_EXIST                  = 0x10000000ul,  // BIT28
} INTRUSION_VARIABLE;

#endif  // _AMD_PSP_INTRUSION_DETECTION_COMMON_H_