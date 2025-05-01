/** @file
  WinNt driver

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
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/PchReset.h>
#include <PlatformInfo.h>
#include <Protocol/MemInfo.h>
#include <Protocol/SaPolicy.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/ConfigBlockLib.h>
#include <ConfigBlock.h>
#include <ConfigBlock/GraphicsDxeConfig.h>
#include <Protocol/DxeTbtPolicy.h>
#include <Protocol/AcpiTable.h>


VOID *
EFIAPI
WinNtPchGetResetData (
  IN   EFI_GUID *Guid,
  OUT  UINTN    *DataSize
  )
{
  *DataSize = 0;
  return NULL;
}

EFI_STATUS
EFIAPI
RootBridgeIoPciRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  OUT    VOID                                   *Buffer
  )
{
  if (Width == EfiPciWidthUint32) {
    ZeroMem (Buffer, sizeof (UINT32) * Count);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ChipsetWinNtDummyFunc (
  VOID * Content
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InstallAcpiTable (
  IN   EFI_ACPI_TABLE_PROTOCOL                    *This,
  IN   VOID                                       *AcpiTableBuffer,
  IN   UINTN                                      AcpiTableBufferSize,
  OUT  UINTN                                      *TableKey
  )
{
  return EFI_UNSUPPORTED;
}


EFI_STATUS
EFIAPI
UninstallAcpiTable (
  IN  EFI_ACPI_TABLE_PROTOCOL                    *This,
  IN  UINTN                                      TableKey
  )
{
  return EFI_UNSUPPORTED;
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
ChipsetWinNtDxeInit (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
//PCH_RESET_PROTOCOL            *PchResetProtocol;
//PLATFORM_INFO                 *PlatformInfoProtocol;
  MEM_INFO_PROTOCOL             *MemoryInfo;
//EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
//PCH_INFO_PROTOCOL             *PchInfo;
  SA_POLICY_PROTOCOL            *DxeSaPolicy;
  GRAPHICS_DXE_CONFIG            GraphicsDxeConfig;
  GRAPHICS_DXE_CONFIG            *GraphicsDxeConfigPtr;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  DXE_TBT_POLICY_PROTOCOL       *DxeTbtConfig;

  AcpiTableProtocol = AllocatePool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  AcpiTableProtocol->InstallAcpiTable  = InstallAcpiTable;
  AcpiTableProtocol->UninstallAcpiTable  = UninstallAcpiTable;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiAcpiTableProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  AcpiTableProtocol
                  );
  ASSERT_EFI_ERROR (Status);


  DxeTbtConfig = AllocatePool (sizeof (DXE_TBT_POLICY_PROTOCOL));
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gDxeTbtPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  DxeTbtConfig
                  );
  ASSERT_EFI_ERROR (Status);


  ///
  /// Allocate Runtime memory for the PchReset protocol instance.
  ///
//PchResetProtocol = AllocateRuntimeZeroPool (sizeof (PCH_RESET_PROTOCOL));
//ASSERT (PchResetProtocol != NULL);
//if (PchResetProtocol == NULL) {
//  return EFI_OUT_OF_RESOURCES;
//}
//
//PchResetProtocol->Reset = WinNtPchReset;
//PchResetProtocol->GetResetData = WinNtPchGetResetData;
//
/////
///// Install protocol interface
/////
//Handle = NULL;
//Status = gBS->InstallProtocolInterface (
//                &Handle,
//                &gPchResetProtocolGuid,
//                EFI_NATIVE_INTERFACE,
//                PchResetProtocol
//                );
//ASSERT_EFI_ERROR (Status);


//PlatformInfoProtocol = AllocatePool (sizeof (PLATFORM_INFO));
//ASSERT (PlatformInfoProtocol != NULL);
//if (PlatformInfoProtocol == NULL) {
//  return EFI_OUT_OF_RESOURCES;
//}
//
//Handle = NULL;
//Status = gBS->InstallProtocolInterface (
//                &Handle,
//                &gPlatformInfoProtocolGuid,
//                EFI_NATIVE_INTERFACE,
//                PlatformInfoProtocol
//                );
//ASSERT_EFI_ERROR (Status);


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

  ASSERT_EFI_ERROR (Status);

//PchInfo = AllocatePool (sizeof (PCH_INFO_PROTOCOL));
//ASSERT (PchInfo != NULL);
//if (PchInfo == NULL) {
//  return EFI_OUT_OF_RESOURCES;
//}
//Handle = NULL;
//Status = gBS->InstallMultipleProtocolInterfaces (
//                &Handle,
//                &gPchInfoProtocolGuid,
//                PchInfo,
//                NULL
//                );

  DxeSaPolicy = NULL;
  Status = CreateConfigBlockTable (sizeof (SA_POLICY_PROTOCOL) + sizeof (GRAPHICS_DXE_CONFIG), (VOID **)&DxeSaPolicy);
  ASSERT_EFI_ERROR (Status);
  if (DxeSaPolicy == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (&(GraphicsDxeConfig.Header.GuidHob.Name), &gGraphicsDxeConfigGuid, sizeof (EFI_GUID));
  GraphicsDxeConfig.Header.GuidHob.Header.HobLength = sizeof (GRAPHICS_DXE_CONFIG);
  GraphicsDxeConfig.Header.Revision                 = SA_POLICY_PROTOCOL_REVISION;
  GraphicsDxeConfigPtr = &GraphicsDxeConfig;
  AddConfigBlock (DxeSaPolicy, &GraphicsDxeConfigPtr);

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gSaPolicyProtocolGuid,
                  DxeSaPolicy,
                  NULL
                  );
//PciRootBridgeIo = AllocatePool (sizeof (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL));
//ASSERT (PciRootBridgeIo != NULL);
//if (PciRootBridgeIo == NULL) {
//  return EFI_OUT_OF_RESOURCES;
//}
//Handle = NULL;
//Status = gBS->InstallProtocolInterface (
//                &Handle,
//                &gEfiPciRootBridgeIoProtocolGuid,
//                EFI_NATIVE_INTERFACE,
//                PciRootBridgeIo
//                );

//ASSERT_EFI_ERROR (Status);

  PcdSet64S (PcdFuncBoardHookPlatformSetupOverride, (UINT64) (UINTN) ChipsetWinNtDummyFunc);

  return Status;
}
