/** @file
  SPI Access Init routines

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/DxeServicesTableLib.h>
//[-start-220905-IB14740261-add]//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
//[-end-220905-IB14740261-add]//

/**
  Initialization routine for SpiAccessLib

  @param  BaseAddress           SPI Base Address.

  @retval EFI_SUCCESS           SpiAccessLib successfully initialized
  @return Others                SpiAccessLib initialization failed

**/
//[-start-220905-IB14740261-modify]//
EFI_STATUS
EFIAPI
SpiAccessInit (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PHYSICAL_ADDRESS            BaseAddress;
  UINT64                          Length;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR GcdDescriptor;

  DEBUG ((DEBUG_INFO, "DXE SpiAccessInit \n"));

  BaseAddress = (UINTN)PcdGet32 (PcdFlashAreaBaseAddress);
  Length      = (UINTN)PcdGet32 (PcdFlashAreaSize);

  if (BaseAddress < (BASE_4GB - SIZE_16MB)) {
    BaseAddress = (BASE_4GB - SIZE_16MB);
  }
  if (Length > SIZE_16MB) {
    Length = SIZE_16MB;
  }

  //
  // Reserve SPI BAR space in GCD, which will be reported to OS by E820
  // It will assert if SPI BAR Memory Space is not allocated
  // The caller is responsible for the existence and allocation of the SPI BAR space Memory Spaces.
  //

  BaseAddress = BaseAddress & (~EFI_PAGE_MASK);
  Length = (Length + EFI_PAGE_SIZE - 1) & (~EFI_PAGE_MASK);

  Status = gDS->GetMemorySpaceDescriptor (BaseAddress, &GcdDescriptor);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((GcdDescriptor.Attributes & EFI_MEMORY_RUNTIME) == EFI_MEMORY_RUNTIME) {
    return EFI_SUCCESS;
  }

  Status = gDS->SetMemorySpaceAttributes (
                  BaseAddress,
                  Length,
                  EFI_MEMORY_RUNTIME
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
//[-end-220905-IB14740261-modify]//
