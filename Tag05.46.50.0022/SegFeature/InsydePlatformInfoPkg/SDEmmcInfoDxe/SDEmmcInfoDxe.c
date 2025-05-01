/** @file
  This Protocol gets all SD/eMMC information and implements all datas
  that will be used.

;******************************************************************************
;* Copyright (c) 2023 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "SDEmmcInfoDxe.h"
#include <Library/DeviceInfoLookupDataLib.h>

SD_EMMC_PRIVATE_DATA        *mSDEmmcPrivateInfo = NULL;

/**
  Helper function to free allocated memory.

  @param[in]  This  Points to the H2O_SD_EMMC_INFORMATION.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.

**/
EFI_STATUS
SafeFreeSDEmmcPrivateInfo (
  IN  H2O_SD_EMMC_INFORMATION  *PrivateData
  )
{
  UINTN   Index;

  if (PrivateData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->SDEmmcInfoBuffer != NULL) {
    for (Index = 0; Index < PrivateData->SDEmmcControllerCount; Index++) {
      if (PrivateData->SDEmmcInfoBuffer[Index].DevicePath != NULL) {
        FreePool (PrivateData->SDEmmcInfoBuffer[Index].DevicePath);
      }
      if (PrivateData->SDEmmcInfoBuffer[Index].CardInfo != NULL) {
        FreePool (PrivateData->SDEmmcInfoBuffer[Index].CardInfo);
      }
    }

    FreePool (PrivateData->SDEmmcInfoBuffer);
    PrivateData->SDEmmcInfoBuffer       = NULL;
    PrivateData->SDEmmcControllerCount  = 0;
  }

  return EFI_SUCCESS;
}

/**
  Helper function to gather SD/Emmc Private infos.

  @param[in,out]  PrivateData   A pointer to H2O_SD_EMMC_INFORMATION.

  @retval EFI_SUCCESS           Function completes successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of resource.

**/
STATIC
EFI_STATUS
EFIAPI
GatherSDEmmcPrivateInfo (
  IN OUT  H2O_SD_EMMC_INFORMATION  *PrivateData
  )
{
  EFI_STATUS                  Status;
  SD_PASS_THRU_PROTOCOL       *SdEmmcPassThru;
  UINTN                       Index;
  UINTN                       SDEmmcHandleCount;
  EFI_HANDLE                  *SDEmmcHandleBuffer;
  CARD_INFO                   *CardInfo;
  SD_EMMC_INFO                *Buffer;

  SdEmmcPassThru = NULL;
  Buffer    = NULL;
  CardInfo  = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gSdPassThruProtocolGuid,
                  NULL,
                  &SDEmmcHandleCount,
                  &SDEmmcHandleBuffer
                  );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
    DEBUG ((EFI_D_ERROR, "%a LocateHandleBuffer: %r.\n", __FUNCTION__, Status));
    goto ErrorExit;
  }

  Buffer = (SD_EMMC_INFO *) AllocateZeroPool (sizeof (SD_EMMC_INFO) * SDEmmcHandleCount);
  if (Buffer == NULL) {
    DEBUG ((EFI_D_ERROR, "%a EFI_OUT_OF_RESOURCE.\n", __FUNCTION__));
    goto ErrorExit;
  }

  for (Index = 0; Index < SDEmmcHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    SDEmmcHandleBuffer[Index],
                    &gSdPassThruProtocolGuid,
                    (VOID **)&SdEmmcPassThru
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a:%d HandleProtocol: %r.\n", __FUNCTION__, __LINE__, Status));
      goto  ErrorExit;
    }

    Status = gBS->HandleProtocol (
                    SDEmmcHandleBuffer[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **)&Buffer[Index].BlockIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a:%d HandleProtocol: %r.\n", __FUNCTION__, __LINE__, Status));
      goto  ErrorExit;
    }

    CardInfo = (CARD_INFO *) AllocateZeroPool (sizeof (CARD_INFO));
    if (CardInfo == NULL) {
      DEBUG ((EFI_D_ERROR, "%a EFI_OUT_OF_RESOURCE.\n", __FUNCTION__));
      goto ErrorExit;
    }
    Status = SdEmmcPassThru->GetCardInfo (SdEmmcPassThru, CardInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a GetCardInfo: %r.\n", __FUNCTION__, Status));
      goto ErrorExit;
    }

    Buffer[Index].CardInfo   = CardInfo;
    Buffer[Index].Handle     = SDEmmcHandleBuffer[Index];
    Buffer[Index].DevicePath = DuplicateDevicePath (DevicePathFromHandle (SDEmmcHandleBuffer[Index]));

    CardInfo = NULL;
  }

  PrivateData->SDEmmcInfoBuffer      = Buffer;
  PrivateData->SDEmmcControllerCount = SDEmmcHandleCount;
  if (SDEmmcHandleBuffer != NULL) {
    FreePool (SDEmmcHandleBuffer);
  }
  return EFI_SUCCESS;

ErrorExit:
  if (SDEmmcHandleBuffer != NULL) {
    FreePool (SDEmmcHandleBuffer);
  }
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
  if (CardInfo == NULL) {
    FreePool (CardInfo);
  }
  return EFI_OUT_OF_RESOURCES;
}

/**
  Helper function to judge DevicePath is SD/eMMC type or not.

  @param[in]  DevicePath  A pointer to device path.

**/
BOOLEAN
IsSdEmmcDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  BOOLEAN                    IsSdEmmc;

  IsSdEmmc = FALSE;

  TempDevicePath = DevicePath;
  while (!IsDevicePathEnd (TempDevicePath)) {
    if ((DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH) &&
        ((DevicePathSubType (TempDevicePath) == MSG_SD_DP) || (DevicePathSubType (TempDevicePath) == MSG_EMMC_DP))){
      IsSdEmmc = TRUE;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }

  return IsSdEmmc;
}

/**
  Function to compare 2 OemPortNameInfo's TokenNumber.

  @param[in]  OemPortNameInfo1  Pointer to OemPortNameInfo to be compared.
  @param[in]  OemPortNameInfo2  Pointer to second OemPortNameInfo to be compared.

  @retval 0   OemPortNameInfo1's token number is equal to OemPortNameInfo2's.
  @retval <0  OemPortNameInfo1's token number is less than OemPortNameInfo2's.
  @retval >0  OemPortNameInfo1's token number is greater than OemPortNameInfo2's.

**/
INTN
EFIAPI
CompareOemPortNameInfo (
  IN CONST VOID   *OemPortNameInfo1,
  IN CONST VOID   *OemPortNameInfo2
  )
{
  if (((OEM_PORT_NAME_INFO *) OemPortNameInfo1)->TokenNumber == ((OEM_PORT_NAME_INFO *) OemPortNameInfo2)->TokenNumber) {
    return 0;
  } else {
    if (((OEM_PORT_NAME_INFO *) OemPortNameInfo1)->TokenNumber < ((OEM_PORT_NAME_INFO *) OemPortNameInfo2)->TokenNumber) {
      return -1;
    } else {
      return 1;
    }
  }
}

/**
  Helper function to gather OEM port name infos.

  @param[in,out]  PrivateData   A pointer to H2O_SD_EMMC_INFORMATION.

  @retval EFI_SUCCESS           Function completes successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of resource.

**/
STATIC
EFI_STATUS
EFIAPI
GatherOemPortNamePrivateInfo (
  IN OUT  H2O_OEM_PORT_NAME_INFORMATION  *PrivateData
  )
{
  UINTN                     NumberOfOemPorts;
  OEM_PORT_NAME_INFO        *PortNameInfos;
  EFI_STATUS                Status;
  CHAR16                    StrBuffer[50];

  DEBUG ((EFI_D_INFO, "%a (),  start\n", __FUNCTION__));
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL          *DeviceInfoLookup;
  H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *DLookUpTable;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES        DeviceInfoLookupAttributes;
  UINTN                                    Index;

  DeviceInfoLookup = NULL;
  InitDeviceInfoLookupProtocol (&DeviceInfoLookup);

  if (DeviceInfoLookup == NULL) {
    return EFI_NOT_FOUND;
  }

  DLookUpTable = NULL;
  NumberOfOemPorts = 0;
  DeviceInfoLookupAttributes.Uint8 = 0;
  DeviceInfoLookupAttributes.Bits.IsSdEmmc = 1;

  PortNameInfos = (OEM_PORT_NAME_INFO *) AllocateZeroPool (NumberOfOemPorts * sizeof (OEM_PORT_NAME_INFO));

  Status = DeviceInfoLookup->CreateNameTable (
                               DeviceInfoLookup,
                               DeviceInfoLookupAttributes.Uint8,
                               &DLookUpTable,
                               &NumberOfOemPorts
                               );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "%a (),  CreateNameTable Failed: %r\n", __FUNCTION__, Status));
    goto UndefineAnyPortname;
  }

  if (PortNameInfos == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < NumberOfOemPorts; Index++) {
    ZeroMem (StrBuffer, sizeof (StrBuffer));
    PortNameInfos[Index].DevicePath  = DuplicateDevicePath (DLookUpTable[Index].DevicePath);
    PortNameInfos[Index].Size        = sizeof (OEM_PORT_NAME_INFO);
    PortNameInfos[Index].TokenNumber = DLookUpTable[Index].PcdTokenNumber;

    UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        L"%s",
        DLookUpTable[Index].DisplayName
        );

    PortNameInfos[Index].PortName = AllocateCopyPool (StrnSizeS (StrBuffer, sizeof (StrBuffer)), StrBuffer);
  }

  PrivateData->OemPortsAmount     = NumberOfOemPorts;
  PrivateData->OemPortNameInfos   = PortNameInfos;

  DeviceInfoLookup->DestroyNameTable (DLookUpTable);
  return EFI_SUCCESS;

UndefineAnyPortname:
  PrivateData->OemPortsAmount     = 0;
  PrivateData->OemPortNameInfos   = NULL;
  return Status;
}

/**
  Helper function to rescan SD/EMMC device and rebuild private data.

  @param[in]  This  Points to the H2O_SD_EMMC_INFO_PROTOCOL.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
EFIAPI
Refresh (
  IN  H2O_SD_EMMC_INFO_PROTOCOL  *This
  )
{
  SD_EMMC_PRIVATE_DATA    *SDEmmcInfoPrivate;


  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SDEmmcInfoPrivate = SD_EMMC_PRIVATE_INFO_FROM_THIS(This);

  SafeFreeSDEmmcPrivateInfo (&SDEmmcInfoPrivate->SDEmmcInfo);

  return GatherSDEmmcPrivateInfo (&SDEmmcInfoPrivate->SDEmmcInfo);
}

/**
  This function helps to get SD_EMMC_INFO buffer that contain count and information.

  @param[in]  This              Points to the H2O_SD_EMMC_INFO_PROTOCOL.
  @param[OUT] SDEmmcNum         SD/EMMC device count.
  @param[OUT] SDEmmcInfoBuffer  SD/EMMC device information buffer.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
EFIAPI
GetSDEmmcInfo (
  IN  H2O_SD_EMMC_INFO_PROTOCOL   *This,
  OUT UINTN                       *SDEmmcNum,
  OUT SD_EMMC_INFO                **SDEmmcInfoBuffer
)
{
  SD_EMMC_PRIVATE_DATA    *SDEmmcInfoPrivate;

  if ((This == NULL)      ||
      (SDEmmcNum == NULL) ||
      (SDEmmcInfoBuffer == NULL)) {

    DEBUG ((EFI_D_ERROR, "%a EFI_INVALID_PARAMETER\n", __FUNCTION__));
    return  EFI_INVALID_PARAMETER;
  }

  SDEmmcInfoPrivate = SD_EMMC_PRIVATE_INFO_FROM_THIS(This);

  (*SDEmmcNum) = SDEmmcInfoPrivate->SDEmmcInfo.SDEmmcControllerCount;
  (*SDEmmcInfoBuffer) = (SD_EMMC_INFO *) AllocateCopyPool (
                                           ((*SDEmmcNum) * sizeof (SD_EMMC_INFO)),
                                           (VOID *) SDEmmcInfoPrivate->SDEmmcInfo.SDEmmcInfoBuffer
                                           );

  if ((*SDEmmcInfoBuffer) == NULL) {
    (*SDEmmcNum) = 0;
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  This function helps to get SD_EMMC_INFO buffer that contain count and information.

  @param[in]  This              Points to the H2O_SD_EMMC_INFO_PROTOCOL.
  @param[OUT] SDEmmcNum         SD/EMMC device count.
  @param[OUT] SDEmmcInfoBuffer  SD/EMMC device information buffer.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
EFIAPI
GetOemPortNameInfo (
  IN  H2O_SD_EMMC_INFO_PROTOCOL   *This,
  OUT UINTN                       *OemPortsAmount,
  OUT OEM_PORT_NAME_INFO          **OemPortNameInfoBuffer
)
{
  SD_EMMC_PRIVATE_DATA    *SDEmmcInfoPrivate;

  if ((This == NULL)      ||
      (OemPortsAmount == NULL) ||
      (OemPortNameInfoBuffer == NULL)) {

    DEBUG ((EFI_D_ERROR, "%a EFI_INVALID_PARAMETER\n", __FUNCTION__));
    return  EFI_INVALID_PARAMETER;
  }

  SDEmmcInfoPrivate = SD_EMMC_PRIVATE_INFO_FROM_THIS(This);

  (*OemPortsAmount) = SDEmmcInfoPrivate->OemPortNameInfo.OemPortsAmount;
  *OemPortNameInfoBuffer = NULL;
  if (SDEmmcInfoPrivate->OemPortNameInfo.OemPortNameInfos != NULL) {
    (*OemPortNameInfoBuffer) = (OEM_PORT_NAME_INFO *) AllocateCopyPool (
                                                        ((*OemPortsAmount) * sizeof (OEM_PORT_NAME_INFO)),
                                                        (VOID *) SDEmmcInfoPrivate->OemPortNameInfo.OemPortNameInfos
                                                        );
  }

  if ((*OemPortsAmount) == 0) {
    return EFI_NOT_FOUND;
  }

  if ((*OemPortNameInfoBuffer) == NULL) {
    (*OemPortsAmount) = 0;
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Callback function for check point gH2OBdsCpConnectAllAfterGuid.

  This function will build private data for SD/eMMC devices Info Protocol and install it.

  @param[in]  Event   A pointer to the Event that triggered the callback.
  @param[in]  Handle  Checkpoint handle.

**/
VOID
H2OBdsCpConnectAllAfterSDEmmcCallback (
  IN  EFI_EVENT       Event,
  IN  H2O_CP_HANDLE   Handle
  )
{
  EFI_STATUS                  Status;
  SD_EMMC_PRIVATE_DATA        *SDEmmcPrivateInfo = NULL;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  SDEmmcPrivateInfo = (SD_EMMC_PRIVATE_DATA *) AllocateZeroPool (sizeof (SD_EMMC_PRIVATE_DATA));
  if (SDEmmcPrivateInfo == NULL) {
    DEBUG ((EFI_D_ERROR, "%a EFI_OUT_OF_RESOURCE.\n", __FUNCTION__));
    goto ErrorExit;
  }

  // SD/Emmc Private Information
  Status = GatherSDEmmcPrivateInfo (&SDEmmcPrivateInfo->SDEmmcInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a GatherSDEmmcPrivateInfo: %r.\n", __FUNCTION__, Status));
    goto ErrorExit;
  }

  // SD/Emmc Private Information
  Status = GatherOemPortNamePrivateInfo (&SDEmmcPrivateInfo->OemPortNameInfo);
  if (Status == EFI_NOT_FOUND) {
    DEBUG ((EFI_D_VERBOSE, "%a(), Non of SD/eMMC device has defined.\n", __FUNCTION__, Status));
  } else if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a GatherOemPortNamePrivateInfo: %r.\n", __FUNCTION__, Status));
    goto ErrorExit;
  }

  // SD/Emmc Protocol Interface
  SDEmmcPrivateInfo->SDEmmcInfoProtcol.GetSDEmmcInfo      = GetSDEmmcInfo;
  SDEmmcPrivateInfo->SDEmmcInfoProtcol.GetOemPortNameInfo = GetOemPortNameInfo;
  SDEmmcPrivateInfo->SDEmmcInfoProtcol.Refresh            = Refresh;

  // SD/Emmc Signature
  SDEmmcPrivateInfo->Signature = SD_EMMC_INFO_SIGNATURE;

  Status = gBS->InstallProtocolInterface (
                  &SDEmmcPrivateInfo->ProtocolHandle,
                  &gH2OSDEmmcInfoProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &SDEmmcPrivateInfo->SDEmmcInfoProtcol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a InstallProtocolInterface: %r.\n", __FUNCTION__, Status));
    goto ErrorExit;
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
  return;

  ErrorExit:
  DEBUG ((EFI_D_INFO, "%a () Error.\n", __FUNCTION__));
  if (SDEmmcPrivateInfo != NULL) {
    FreePool (SDEmmcPrivateInfo);
  }
  return;
}

/**
  The Entry point of H2O_SD_EMMC_INFO_PROTOCOL driver and create private data.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
SDEmmcInfoDxeProtocolEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                  Status;
  H2O_SD_EMMC_INFO_PROTOCOL   *Protocol;
  H2O_CP_HANDLE               H2OBdsCpConnectAllAfterHandle;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol(
                  &gH2OSDEmmcInfoProtocolGuid,
                  NULL,
                  (VOID **)&Protocol
                  );
  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "H2OSDEmmcInfoProtocol Already Started.\n"));
    return EFI_ALREADY_STARTED;
  }

  //
  // Collect storage devices information after connect all
  //
  if (FeaturePcdGet (PcdH2OBdsCpConnectAllAfterSupported)) {
    Status = H2OCpRegisterHandler (
                &gH2OBdsCpConnectAllAfterGuid,
                H2OBdsCpConnectAllAfterSDEmmcCallback,
                H2O_CP_MEDIUM,
                &H2OBdsCpConnectAllAfterHandle
                );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to register checkpoint: %g, Status: %r\n", __FUNCTION__, &gH2OBdsCpConnectAllAfterGuid, Status));
      return Status;
    }
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;

}

