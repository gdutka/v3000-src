/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#ifndef __AOD_V2_CORE_INFO_H__
#define __AOD_V2_CORE_INFO_H__

#define AOD_MAX_CCD_COUNT_LIMIT           12
#define AOD_MAX_CORE_PER_CCD_LIMIT        8
///
/// AOD Core Status
///
typedef enum {
  AOD_CORE_FUSED_OFF =  0,
  AOD_CORE_ACTIVE,
  AOD_CORE_SOFT_OFF,
} AOD_CORE_STATUS;
#pragma pack(push,1)
typedef struct {
  UINT8          AodPhysicalCcdCount;               ///< Physical Ccd count
  UINT8          AodLogicalCcdCount;                ///< Logical Ccd count = Physcial subtract fused
  UINT8          AodPhysicalCoreCountPerCcd;        ///< Physical core count
  UINT8          AodLogicalCoreCountPerCcd;         ///< Logical core count = Physcial subtract fused
  UINT8          AodActivceCoreCountPerCcd;         ///< Active core count = Logical subtract disabled
  UINT8          AodLogicalCoreCount;               ///< Total logical core number
  UINT32         AodCcdBitMap;                      ///< 1: CCD Present, 0: CCD Fused off
  UINT8          AodCoreStatus[AOD_MAX_CCD_COUNT_LIMIT][AOD_MAX_CORE_PER_CCD_LIMIT];
} AOD_CORE_INFO;
#endif
#pragma pack(pop)