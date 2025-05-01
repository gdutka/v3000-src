/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_GPIO_SMM_H_
#define _FCH_PROM21_GPIO_SMM_H_

#define PT_SMM_PRIVATE_DATA_SIGNATURE   SIGNATURE_16 ('P', 'T')

EFI_STATUS
PTGpioWriteSmm (
  IN       UINT8        Pin,
  IN       UINT8        Value
  );

EFI_STATUS
PTGpioReadSmm (
  IN       UINT8        Pin,
  OUT      UINT8        *Value
  );

// Write GPIO function
typedef
EFI_STATUS
(EFIAPI *PTWriteGpioSmm) (
  IN       UINT8         Pin,
  IN       UINT8         Value
  );

// Read GPIO function
typedef
EFI_STATUS
(EFIAPI *PTReadGpioSmm) (
  IN       UINT8          Pin,
  OUT      UINT8          *Value
  );

// Module data structure
typedef struct _EFI_PT_GPIO_SMM_PROTOCOL  {
  PTWriteGpioSmm    PTWriteGpioSmm;                   ///PTWriteGpioSmm
  PTReadGpioSmm     PTReadGpioSmm;                    ///PTReadGpioSmm
} EFI_PT_GPIO_SMM_PROTOCOL;

// Module data structure
typedef struct _PT_SMM_PRIVATE {
  UINT16                 Signature;                   ///< Signature
  EFI_PT_GPIO_SMM_PROTOCOL   PtGpioProtocol;          ///< PT INIT PROTOCOL
} PT_SMM_PRIVATE;

EFI_STATUS
EFIAPI
FchProm21GpioSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );

#endif

