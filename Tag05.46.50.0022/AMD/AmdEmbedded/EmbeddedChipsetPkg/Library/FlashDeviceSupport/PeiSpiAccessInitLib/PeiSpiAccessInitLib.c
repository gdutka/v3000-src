/** @file
  SPI Access Init routines

;******************************************************************************
;* Copyright (c) 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Ppi/Spi.h>
#include <Library/PeiServicesLib.h>
#include <LibraryPrivate/PeiPchInitLib/PchSpi.h>

extern PCH_SPI_PROTOCOL       *mSpiProtocol;

/**
  Detect whether the system is at EFI runtime or not

  @param  None

  @retval TRUE                  System is at EFI runtime
  @retval FALSE                 System is not at EFI runtime

**/
BOOLEAN
EFIAPI
AtRuntime (
  VOID
  )
{
  return FALSE;
}

/**
  Initialization routine for SpiAccessLib

  @param  None

  @retval EFI_SUCCESS           SpiAccessLib successfully initialized
  @return Others                SpiAccessLib initialization failed

**/
EFI_STATUS
EFIAPI
SpiAccessInit (
  VOID
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "PEI SpiAccessInit \n"));

  //
  // Original SPI PPI is running on flashrom. Reinsatll SPI PPI on memory to avoid updating PEI area will hang up issue 
  //
  ReInstallPchSpi();
  //
  // Locate the SPI protocol.
  //
  Status = PeiServicesLocatePpi (
             &gPchSpiPpiGuid,
             0,
             NULL,
             (VOID **) &mSpiProtocol
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
/**
  This routine uses to free the allocated resource by SpiAccessInit ().

  @retval EFI_SUCCESS    Free allocated resource successful.
  @return Others         Free allocated resource failed.
**/
EFI_STATUS
EFIAPI
SpiAccessDestroy (
  VOID
  )
{
  return EFI_SUCCESS;
}
