/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

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
#ifndef _AOD_CURVE_OPTIMIZER_H_
#define _AOD_CURVE_OPTIMIZER_H_
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/MdeModuleHii.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Features/AODv2/Include/Aod.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include "AodSocLib.h"

/**
  This function used to generate per core curve optimizer

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGeneratePerCoreCurveOptionsRpl (
  IN  EFI_HII_HANDLE              HiiHandle
);

/**
  This function used to generate per CCD Freq optimizer

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateCcdFreqOptimizerOptionsRpl (
  IN  EFI_HII_HANDLE              HiiHandle
  );
#endif