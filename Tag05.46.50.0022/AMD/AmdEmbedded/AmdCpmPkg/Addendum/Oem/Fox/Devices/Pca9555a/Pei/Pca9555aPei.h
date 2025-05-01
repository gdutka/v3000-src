/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _PCA9555A_PEI_H_
#define _PCA9555A_PEI_H_

EFI_STATUS
EFIAPI
Pca9555aSet (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Offset,
  IN       UINT16            Data
  );

EFI_STATUS
EFIAPI
Pca9555aGet (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Offset,
  OUT      UINT16            *Data
  );

#endif // _PCA9555AA_PEI_H_

