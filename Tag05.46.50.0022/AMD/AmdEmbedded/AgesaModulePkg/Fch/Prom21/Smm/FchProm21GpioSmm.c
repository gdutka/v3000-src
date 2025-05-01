/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Protocol/SmmBase2.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/debugLib.h>
#include <Library/BaseMemoryLib.h>

#include <Filecode.h>

#include <Protocol/FchProm21GpioSmmProtocol.h>
#include <Library/FchProm21Common.h>
#include "FchProm21GpioSmm.h"

#define FILECODE FCH_PROM21_SMM_FCHPROM21GPIOSMM_FILECODE

EFI_STATUS
PTGpioWriteSmm (
  IN       UINT8        Pin,
  IN       UINT8        Value
  )
{
 // UINT32        *GpioMmio = NULL ;
  EFI_STATUS             Status;

  Status = EFI_SUCCESS;
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOSMM-%a-Start\n", __FUNCTION__));
//
//  /////////////////
//  // Write Gpio  //
//  /////////////////
//  GpioMmio = (UINT32 *)(UINTN)(GPIO_IOAPIC_BAR);
//
//  if ( ( *GpioMmio & GpioBit[Pin]) == 0 ) {    // The pin doesn't programmed as Output Pin
//    Status = RETURN_UNSUPPORTED;
//    goto IncorrectGpioSetting;
//  }
//
//  GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR + 8);
//
//  if (Value) {
//    *GpioMmio |= GpioBit[Pin];
//  } else
//  {
//    *GpioMmio &= ~(GpioBit[Pin]);
//  }
//
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOSMM-%a-End\n", __FUNCTION__));
//  Status = EFI_SUCCESS;
//
//  IncorrectGpioSetting:
  return Status;
}

EFI_STATUS
PTGpioReadSmm (
  IN       UINT8        Pin,
  OUT      UINT8        *Value
  )
{
 // UINT32       *GpioMmio = NULL ;
  EFI_STATUS             Status;

  Status = EFI_SUCCESS;
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOSMM-%a-Start\n", __FUNCTION__));
//
//  /////////////////
//  // Read  Gpio  //
//  /////////////////
//  GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR);
//
//  if ( ( *GpioMmio & GpioBit[Pin]) != 0 ) {
//    Status = RETURN_UNSUPPORTED;   // The pin doesn't programmed as input Pin
//    goto IncorrectGpioSetting;
//  }
//
//  GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR + 4);
//
//  *Value = (UINT8)((*GpioMmio & GpioBit[Pin]) >> Pin);
//
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOSMM-%a-End\n", __FUNCTION__));
//  Status = EFI_SUCCESS;
//
//  IncorrectGpioSetting:
  return Status;
}

/*********************************************************************************
 * Name: FchPromontoryGpioSmmEntryPoint
 *
 * Description
 *   Entry point of the AMD Promontory Gpio SMM driver
 *   copy PT_INIT_PROTOCOL to SMM, and register Fch Smm callbacks
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchProm21GpioSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_HANDLE          PromontorySmmHandle;
  PT_SMM_PRIVATE      *PtSmmPrivate;

  DEBUG ((DEBUG_INFO, "PT-GPIOSMM-%a-Start\n", __FUNCTION__));
  PromontorySmmHandle = NULL;

  // Initialize the configuration structure and private data area
  //
  // Allocate memory for the private data
  Status = gSmst->SmmAllocatePool (
                         EfiRuntimeServicesData,
                         sizeof (PT_SMM_PRIVATE),
                         &PtSmmPrivate
                         );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (PtSmmPrivate, sizeof (PT_SMM_PRIVATE));

  // Initialize the private data structure
  PtSmmPrivate->Signature = PT_SMM_PRIVATE_DATA_SIGNATURE;
  PtSmmPrivate->PtGpioProtocol.PTWriteGpioSmm = PTGpioWriteSmm;
  PtSmmPrivate->PtGpioProtocol.PTReadGpioSmm  = PTGpioReadSmm;

  Status = gSmst->SmmInstallProtocolInterface (
               &PromontorySmmHandle,
               &gEfiPromontoryGpioSmmProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &PtSmmPrivate->PtGpioProtocol
               );

  DEBUG ((DEBUG_INFO, "PT-GPIOSMM-%a-End\n", __FUNCTION__));
  return Status;
}

