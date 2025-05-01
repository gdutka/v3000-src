/** @file
  WinNt driver

;******************************************************************************
;* Copyright (c) 2014, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/PchReset/PchReset.h>
#include <Protocol/MemInfo/MemInfo.h>
#include <Protocol/MeBiosPayloadData/MeBiosPayloadData.h>
#include <Protocol/MePlatformPolicy.h>

EFI_STATUS
EFIAPI
WinNtPchReset (
  IN     PCH_RESET_PROTOCOL       *This,
  IN     PCH_RESET_TYPE           PchResetType
  )
{
  gBS->RaiseTPL (TPL_NOTIFY);
  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

  return EFI_SUCCESS;
}


/**
  Initialize the state information for ChipsetWinNtDriver

  @param[in] ImageHandle          Image handle of the loaded driver
  @param[in] SystemTable          Pointer to the System Table

  @retval EFI_SUCCESS             Thread can be successfully created
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR        Cannot create the timer service
**/
EFI_STATUS
EFIAPI
ChipsetWinNtDriverInit (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  PCH_RESET_PROTOCOL            *PchResetProtocol;
  MEM_INFO_PROTOCOL             *MemoryInfo;
  DXE_MBP_DATA_PROTOCOL         *MBPDataProtocol;
  DXE_ME_POLICY_PROTOCOL        *MEPlatformPolicy;

  ///
  /// Allocate Runtime memory for the PchReset protocol instance.
  ///
  PchResetProtocol = AllocateRuntimeZeroPool (sizeof (PCH_RESET_PROTOCOL));
  ASSERT (PchResetProtocol != NULL);
  if (PchResetProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PchResetProtocol->Reset = WinNtPchReset;

  ///
  /// Install protocol interface
  ///
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gPchResetProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  PchResetProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  MemoryInfo = AllocatePool (sizeof (MEM_INFO_PROTOCOL));
  ASSERT (MemoryInfo != NULL);
  if (MemoryInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gMemInfoProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  MemoryInfo
                  );

  MBPDataProtocol = AllocatePool (sizeof (DXE_MBP_DATA_PROTOCOL));
  ASSERT (MBPDataProtocol != NULL);
  if (MBPDataProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gMeBiosPayloadDataProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  MBPDataProtocol
                  );

  MEPlatformPolicy = AllocatePool (sizeof (DXE_ME_POLICY_PROTOCOL));
  ASSERT (MEPlatformPolicy != NULL);
  if (MEPlatformPolicy == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MEPlatformPolicy->MeConfig.EndOfPostDone = 1;
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gDxePlatformMePolicyGuid,
                  EFI_NATIVE_INTERFACE,
                  MEPlatformPolicy
                  );

  return Status;
}
