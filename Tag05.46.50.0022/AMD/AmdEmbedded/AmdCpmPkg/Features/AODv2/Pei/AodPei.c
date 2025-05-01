/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
AOD_MOUDLE_NAME ("AodPei")
EFI_STATUS
EFIAPI
AodEndOfPei (
  IN EFI_PEI_SERVICES           **ppPeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *pNotifyDescriptor,
  IN VOID                       *pPpi
  );

EFI_PEI_NOTIFY_DESCRIPTOR   mAodEndofPeiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  AodEndOfPei
};

/**
  This function is runed at end of pei, used to calc some policy for dxe use.

  @param[in] ppPeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] pNotifyDescriptor   Address of the notification descriptor data structure.
  @param[in] pPpi               Address of the PPI that was installed.

  @retval EFI_SUCCESS if all policy parse passed.
**/
EFI_STATUS
EFIAPI
AodEndOfPei (
  IN EFI_PEI_SERVICES           **ppPeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *pNotifyDescriptor,
  IN VOID                       *pPpi
  )
{

  EFI_STATUS                                 Status;
  AOD_CORE_INFO                              AodCoreInfo;
  Status       = EFI_SUCCESS;
  AOD_DEBUG ((EFI_D_INFO, "Started\n"));
  ZeroMem (&AodCoreInfo, sizeof (AOD_CORE_INFO));

  Status = AodDispatchInitTable (AOD_UPDATE_CORE_INFO, &AodCoreInfo);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  AOD_UPDATE_CORE_INFO Status = %r\n", Status));
  }
  Status = AodSetCoreInfo (&AodCoreInfo);

  AOD_DEBUG ((EFI_D_INFO, "End\n"));
  return Status;
}
/**
 This is the declaration of AOD PEIM entry point.

 @param[in]         FileHandle          Handle of the file being invoked.
 @param[in]         PeiServices         Describes the list of possible PEI Services.

 @retval            EFI_SUCCESS         The operation completed successfully.
 @retval            Others              An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
AodPeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES        **PeiServices
  )
{
  EFI_STATUS                         Status;
  VOID                               *AodConfiguration;
  AOD_DEBUG ((EFI_D_INFO, "Started\n"));
  Status = (*PeiServices)->NotifyPpi (PeiServices, &mAodEndofPeiCallback);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Failed to register endofpei!\n"));
  }

  Status = AodGetConfiguration (&AodConfiguration);
  if (Status != EFI_SUCCESS) {
    AOD_DEBUG ((EFI_D_ERROR, "  Failed to get AodConfiguration!\n"));
    return Status;
  }

  Status = AodDispatchInitTable (AOD_SYNC_VARIABLE_PCD, AodConfiguration);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  AOD_SYNC_VARIABLE_PCD Status = %r\n", Status));
  }
  AOD_DEBUG ((EFI_D_INFO, "End\n"));

  return Status;
}
