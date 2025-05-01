/*****************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains implementation for the SMM Access PPI
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision:  $   @e \$Date: $
 *
 */

#pragma once

#include <Ppi/SmmAccess.h>

#define  SMMADDR_ADDRESS      0xC0010112ul
#define  SMMMASK_ADDRESS      0xC0010113ul
#define      SMMLOCK          0x0000000000000001ull
#define      SMMBASELOCK      0x0000000080000000ull
#define  FCH_MMIO_ADDRESS     0xFED80000ull
#define  SMI_REGISTER_SPACE   0x00000200ull
#define  SMI_TRIG0_OFFSET     0x00000098ull
#define      SMIENB           0x80000000ul

///  Struct for Tseg configuration
typedef struct {
  IN      UINT64 TsegBase;  ///< TsegBase
  IN      UINT64 TsegMask;  ///< TsegMask
} CCX_SMM_TSEG_CONFIG;


EFI_STATUS
EFIAPI
CcxSmmOpen (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  );

EFI_STATUS
EFIAPI
CcxSmmClose (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  );

EFI_STATUS
EFIAPI
CcxSmmLock (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  );

EFI_STATUS
EFIAPI
CcxSmmGetCapabilities (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  );

EFI_STATUS
EFIAPI
CcxSmmAccessPpiInstall (
    IN EFI_PEI_SERVICES                **PeiServices
  );
