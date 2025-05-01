/** @file
  A SMM driver for SMM platform initialization.

;******************************************************************************
;* Copyright (c) 2013 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "SmmPlatform.h"
#include <HybridGraphicsDefine.h>
#include <Protocol/HGDataInfo.h>
#include <ChipsetSmiTable.h>

//
// Global variables.
//
extern AMD_PBS_SETUP_OPTION                gAmdPbsConfiguration;
extern SYSTEM_CONFIGURATION                *gSetupVariable;

EFI_HG_NVS_AREA_PROTOCOL                   *mHgDxeInfoDataProtocol;

/**
  SMI handler to update both "DisplayMode" and "PrimaryDisplay" settings when
  OS driver want to set MUX mode through MXDM method.

  @param[in]      DispatchHandle    The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      DispatchContext   Points to an optional handler context which was specified when the
                                    handler was registered.
  @param[in, out] CommBuffer        A pointer to a collection of data in memory that will
                                    be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize    The size of the CommBuffer.

  @retval EFI_SUCCESS               The interrupt was handled successfully.
**/
EFI_STATUS
SetDisplayMode (
  IN  EFI_HANDLE      DispatchHandle,
  IN  CONST  VOID     *DispatchContext,
  IN  OUT  VOID       *CommBuffer,
  IN  OUT  UINTN      *CommBufferSize
)
{
  EFI_STATUS     Status;

  switch (mHgDxeInfoDataProtocol->DisplayMode) {
    case IgfxOnly:
      gAmdPbsConfiguration.PrimaryVideoAdaptor = DisplayModeIgpu;
      gSetupVariable->DisplayMode = IgfxOnly;
      break;
    case DgpuOnly:
      gAmdPbsConfiguration.PrimaryVideoAdaptor = DisplayModeDgpu;
      gSetupVariable->DisplayMode = DgpuOnly;
      break;
    case MsHybrid:
      gAmdPbsConfiguration.SpecialVgaFeature = DisplayModeHg;
      gSetupVariable->DisplayMode = MsHybrid;
      break;
    case Dynamic:
      gAmdPbsConfiguration.SpecialVgaFeature = DisplayModeHg;
      gSetupVariable->DisplayMode = Dynamic;
      break;
    default:
      return EFI_SUCCESS;
  }
  Status = CommonSetVariable (
             AMD_PBS_SETUP_VARIABLE_NAME,
             &gAmdPbsSystemConfigurationGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof (AMD_PBS_SETUP_OPTION),
             &gAmdPbsConfiguration
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = CommonSetVariable (
             SETUP_VARIABLE_NAME,
             &gSystemConfigurationGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof (SYSTEM_CONFIGURATION),
             &gSetupVariable
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
RegisterSetDisplaymode (
  VOID
  )
{
  EFI_STATUS                                Status;
  EFI_SMM_SW_REGISTER_CONTEXT               SmmSwDispatchContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL             *SmmSwDispatch2;
  EFI_HANDLE                                DispatchHandle;
  EFI_HG_NVS_AREA_PROTOCOL                  *HGNvsAreaProtocol;

  DispatchHandle            = NULL;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSwDispatch2ProtocolGuid,
                    NULL,
                    (VOID**)&SmmSwDispatch2
                    );
  ASSERT_EFI_ERROR(Status);

  if (FeaturePcdGet (PcdNvidiaOptimusSupported)) {
    Status = gBS->LocateProtocol (&gEfiHgNvsAreaProtocolGuid, NULL, (VOID **)&HGNvsAreaProtocol);
    ASSERT_EFI_ERROR (Status);
    mHgDxeInfoDataProtocol = HGNvsAreaProtocol;
    if (FeaturePcdGet (PcdHgNvidiaDdsFeatureSupport)) {
      //
      // Register SMI handler (0x7C) to change DisplayMode and PrimaryDisplay settings.
      //
      SmmSwDispatchContext.SwSmiInputValue = OPTIMUS_SET_DISPLAY_MODE;
      Status = SmmSwDispatch2->Register (
                                 SmmSwDispatch2,
                                 SetDisplayMode,
                                 &SmmSwDispatchContext,
                                 &DispatchHandle
                                 );
      ASSERT_EFI_ERROR (Status);
    }
  }

  return EFI_SUCCESS;

}