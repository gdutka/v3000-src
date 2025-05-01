/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Filecode.h>
#include "AGESA.h"
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/DxeSocLogicalIdServicesLib.h>
#include <Library/DxeSocZen3ServicesLib.h>
#include <Protocol/AmdSocProtocol.h>
#include "AmdSocFp7r2RmbDxe.h"

#define FILECODE SOC_AMDSOCFP7R2RMBDXE_AMDSOCFP7R2RMBDXE_FILECODE

extern  EFI_BOOT_SERVICES       *gBS;

//
// Driver Global Data
//

/*********************************************************************************
 * Name: AmdSocFp7r2RmbDxeInit
 *
 * Description
 *   Entry point of the AMD SOC Cezanne FP7r2 DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
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
AmdSocFp7r2RmbDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  EFI_HANDLE          Handle;
  SOC_ID_STRUCT       SocId;
  SOC_ID_STRUCT       SocId2;
  UINT8 i;

  DEBUG ((EFI_D_ERROR, "*****************************DXE SOC FP7r2 RMB Driver Entry*********************\n"));

  SocId.SocFamilyID = F19_RMB_RAW_ID;
  SocId.PackageType = ZEN3_PKG_FP7r2;

  SocId2.SocFamilyID = F19_RMB_RAW_ID;
  SocId2.PackageType = ZEN3_PKG_FP7r2_B0;

  // Check Hardware Identification
  if (SocHardwareIdentificationCheck (&SocId) || SocHardwareIdentificationCheck (&SocId2)) {
    //
    // Publish the logical ID protocol
    //
    Status = SocLogicalIdServiceProtocolInstall (ImageHandle, SystemTable);

    //
    // Publish the Zen3 services protocol
    //
    Status = SocZen3ServicesProtocolInstall (ImageHandle, SystemTable);

    IDS_HOOK (IDS_HOOK_BEGINNING_OF_AGESA, NULL, NULL);

    //
    // Publish the Silicon installation protocol
    //

    Handle = ImageHandle;
    for (i = 0 ; i < SiliconDriverProtocolListNumber; i++) {
      Status = gBS->InstallProtocolInterface (
                    &Handle,
                    SiliconDriverProtocolList[i],
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    }
  }
  DEBUG ((EFI_D_ERROR, "*****************************DXE SOC FP7r2 RMB Driver Exit*********************\n"));
  return (Status);
}

