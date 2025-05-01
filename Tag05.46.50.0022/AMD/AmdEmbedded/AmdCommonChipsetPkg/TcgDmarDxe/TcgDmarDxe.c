/** @file
  This driver will implement the feature which follow the SPEC 
  Windows_11_-_Manufacturing_Enablement_of_Secured-core_PC_-_v1.1.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "TcgDmarDxe.h"

#define DMA_PROTECTION_DISABLED_EVENT_STRING                 "DMA Protection Disabled"
#define DMA_PROTECTION_DISABLED_EVENT_STRING_LEN             (sizeof (DMA_PROTECTION_DISABLED_EVENT_STRING) - 1)

/**
  Extended a TPM log to PCR[7] when gEfiEndOfDxeEventGroupGuid is installed if PcdDmaProtection disable.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
ExtendPCR7CallBackFunction (
  IN EFI_EVENT                             Event,
  IN VOID                                  *Context
  )
{
  EFI_STATUS                               Status;
  UINT64                                   HashDataLen;
  
  DEBUG ((DEBUG_INFO, "ExtendPCR7CallBack START\n"));
  //
  // When IOMMU/DMA protection is disabled and a platform has a TPM enabled,
  // the platform SHALL extend an EV_EFI_ACTION event into PCR[7]
  //
  HashDataLen = DMA_PROTECTION_DISABLED_EVENT_STRING_LEN;
  Status      = EFI_SUCCESS;

  Status = TpmMeasureAndLogData (
             7,
             EV_EFI_ACTION,
             DMA_PROTECTION_DISABLED_EVENT_STRING,
             (UINT32) HashDataLen,
             DMA_PROTECTION_DISABLED_EVENT_STRING,
             HashDataLen
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "IOMMU TpmMeasureAndLogData Status: %d\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "IOMMU TpmMeasureAndLogData Successfully\n"));
  }

  DEBUG ((DEBUG_INFO, "ExtendPCR7CallBack END\n"));
}

/**
  Register an End of DXE event for extended a TPM log to PCR[7]

  @param[in] ImageHandle                   The firmware allocated handle for the UEFI image.
  @param[in] SystemTable                   A pointer to the EFI System Table.

  @retval EFI_SUCCESS                      The operation completed successfully.
  @retval Others                           An unexpected error occurred.
*/
EFI_STATUS
EFIAPI
TcgDmarDxeEntryPoint (
  IN     EFI_HANDLE                        ImageHandle,
  IN     EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                               Status;
  EFI_EVENT                                EndOfDxeEvent;

  Status      = EFI_SUCCESS;

  if (!PcdGetBool (PcdDmaProtection)) {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    ExtendPCR7CallBackFunction,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &EndOfDxeEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to register an End of DXE event for extended a TPM log to PCR[7], Status: %d\n", Status));
      gBS->CloseEvent (EndOfDxeEvent);
    }
  }

  return Status;
}
