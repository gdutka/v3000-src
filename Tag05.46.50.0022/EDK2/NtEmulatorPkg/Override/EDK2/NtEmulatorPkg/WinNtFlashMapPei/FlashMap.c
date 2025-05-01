/**@file

Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

Module Name:

  FlashMap.c

Abstract:

  PEIM to build GUIDed HOBs for platform specific flash map

**/

//
// The package level header files this module uses
//
#include <PiPei.h>

#include <WinNtPeim.h>
//
// The protocols, PPI and GUID defintions for this module
//
#include <Ppi/NtFwh.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/FlashRegionLib.h>

EFI_STATUS
EFIAPI
PeimInitializeFlashMap (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:
  Build GUIDed HOBs for platform specific flash map

Arguments:
  FfsHeader   - A pointer to the EFI_FFS_FILE_HEADER structure.
  PeiServices - General purpose services available to every PEIM.

Returns:
  EFI_STATUS

--*/
// TODO:    EFI_SUCCESS - add return value to function comment
{
  EFI_STATUS                    Status;
  NT_FWH_PPI                    *NtFwhPpi;
  EFI_PHYSICAL_ADDRESS          FdBase;
  UINT64                        FdSize;
  EFI_PEI_PPI_DESCRIPTOR        *PpiDescriptor;
  H2O_FLASH_DEVICE_MAP_HEADER   *FdmHeader;
  UINT32                        Index;
  UINT8                         Sum;
  UINT8                         *Data;

  DEBUG ((EFI_D_ERROR, "NT 32 Flash Map PEIM Loaded\n"));

  //
  // Get the Fwh Information PPI
  //
  Status = PeiServicesLocatePpi (
            &gNtFwhPpiGuid, // GUID
            0,              // INSTANCE
            &PpiDescriptor, // EFI_PEI_PPI_DESCRIPTOR
            (VOID**)&NtFwhPpi       // PPI
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Assume that FD0 contains the Flash map.
  //
  Status = NtFwhPpi->NtFwh (0, &FdBase, &FdSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Relocate the base of FV region
  //
  Status = PcdSet32S (PcdFlashNvStorageVariableBase, PcdGet32 (PcdWinNtFlashNvStorageVariableBase) + (UINT32) FdBase);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet32S (PcdFlashNvStorageFtwWorkingBase, PcdGet32 (PcdWinNtFlashNvStorageFtwWorkingBase) + (UINT32) FdBase);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet32S (PcdFlashNvStorageFtwSpareBase, PcdGet32 (PcdWinNtFlashNvStorageFtwSpareBase) + (UINT32) FdBase);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet64S (PcdH2OFlashDeviceMapStart, PcdGet64 (PcdH2OFlashDeviceMapStart) + FdBase);
  ASSERT_EFI_ERROR (Status);

  //
  // Update Fd base address
  //
  FdmHeader = (H2O_FLASH_DEVICE_MAP_HEADER*)(UINTN) PcdGet64 (PcdH2OFlashDeviceMapStart);
  FdmHeader->FdBaseAddr = FdBase;
  FdmHeader->Checksum   = 0;

  Data = (UINT8*) FdmHeader;

  //
  // calculator sum
  //

  Sum = 0;
  for (Index = 0; Index < sizeof (H2O_FLASH_DEVICE_MAP_HEADER); Index++, Data++){
    Sum += (*Data);
  }
  FdmHeader->Checksum = -Sum;

  return EFI_SUCCESS;
}

