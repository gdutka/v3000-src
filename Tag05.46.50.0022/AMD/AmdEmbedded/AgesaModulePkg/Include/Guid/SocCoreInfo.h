/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _SOC_CORE_INFO_H_
#define _SOC_CORE_INFO_H_

#define MAX_CCD_COUNT         12
#define MAX_CORE_PER_CCD      8

#pragma pack(1)
///
/// Core Thread Status
///
typedef enum {
  Core_Single_Thread =  0,
  Core_Multi_Thread,
} CORE_THREAD_STATUS;

///
/// Core Status
///
typedef enum {
  Core_Fused_Off =  0,
  Core_Active,
  Core_Soft_Off,
} CORE_STATUS;

///
/// CCD Status
///
typedef enum {
  Ccd_Fused_Off =  0,
  Ccd_Present,
} CCD_STATUS;

///
/// CCD Status Map
///
typedef union {
  struct {
    CCD_STATUS Ccd0Status:1;
    CCD_STATUS Ccd1Status:1;
    CCD_STATUS Ccd2Status:1;
    CCD_STATUS Ccd3Status:1;
    CCD_STATUS Ccd4Status:1;
    CCD_STATUS Ccd5Status:1;
    CCD_STATUS Ccd6Status:1;
    CCD_STATUS Ccd7Status:1;
    UINT32 Reserved_31_8:24;
  } Field;
  UINT32 Value;
} CCD_STATUS_MAP;

///
/// Soc Core Info
///
typedef struct {
  UINT32 CcdMaxCount;                                            ///< Max support CCD Count
  UINT32 CoreMaxCountPerCcd;                                     ///< Max support Core Count Per CCD
  CORE_THREAD_STATUS SmtEnable;                                  ///< CPU/APU support Multi thread or Single thread
  CCD_STATUS_MAP CcdEnableMap;                                   ///< 1: CCD Present, 0: CCD Fused off
  UINT32 CoreDisMap[MAX_CCD_COUNT];                              ///< 1: Core Fused Off, 0: Core Physical exist
  UINT32 CoreActiveMap[MAX_CCD_COUNT];                           ///< 1: Core active, 0: Core non-active
  CORE_THREAD_STATUS CoreThreadStatus;                           ///< 1: Multi thread, 0: Single thread
  CORE_STATUS CoreStatus[MAX_CCD_COUNT][MAX_CORE_PER_CCD];       ///< refer to CORE_STATUS
} SOC_CORE_INFO;

#pragma pack()

extern EFI_GUID gSocCoreInfoGuid;

#endif
