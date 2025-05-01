/** @file
  Implementation of H2OSataDriveInfo.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "SataDriveInfoDxe.h"
#include "AtaCommands.h"

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/H2OCpLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SataDriveInfoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>

#include <Protocol/AtaPassThru.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/ScsiPassThruExt.h>

#include <Protocol/H2OSataDriveInfo.h>

#include <IndustryStandard/Atapi.h>
#include <IndustryStandard/Pci.h>

#include <Guid/H2OCp.h>

#define FIELD_SIZE_OF(TYPE, Field) ((UINTN) sizeof (((TYPE *) 0)->Field))

#ifndef BASE_LIST_FOR_EACH
#define BASE_LIST_FOR_EACH(Entry, ListHead)    \
  for(Entry = (ListHead)->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)
#endif

///
/// Function Declarations
///
EFI_STATUS
GetControllerInfoPool (
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **ControllerInfoPool
  );

EFI_STATUS
GetDriveInfoPool (
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **DriveInfoPool
  );

VOID
Refresh (
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This
  );

STATIC H2O_SATA_DRIVE_INFO_PRIVATE_DATA   mPrivateData = {
  H2O_SATA_DRIVE_INFO_PRIVATE_SIGNATURE,
  0,
  {
    sizeof (H2O_SATA_DRIVE_INFO_PROTOCOL),
    GetControllerInfoPool,
    GetDriveInfoPool,
    Refresh
  },
  INITIALIZE_LIST_HEAD_VARIABLE (mPrivateData.ControllerInfoPool),
  INITIALIZE_LIST_HEAD_VARIABLE (mPrivateData.DriveInfoPool)
};

/**
  Get the ControllerInfoPool.

  @param[in]    This                The pointer to the instance of SataDriveInfo Protocol.
  @param[out]   ControllerInfoPool  The pointer to ControllerInfoPool.

  @retval EFI_SUCCESS             Successfully get the ControllerInfoPool.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.

**/
EFI_STATUS
GetControllerInfoPool (
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **ControllerInfoPool
  )
{
  H2O_SATA_DRIVE_INFO_PRIVATE_DATA  *Private;

  if (This == NULL || ControllerInfoPool == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = H2O_SATA_DRIVE_INFO_PRIVATE_DATA_FROM_THIS (This);

  *ControllerInfoPool = &Private->ControllerInfoPool;

  return EFI_SUCCESS;
}

/**
  Get the DriveInfoPool.

  @param[in]    This            The pointer to the instance of SataDriveInfo Protocol.
  @param[out]   DriveInfoPool   The pointer to DriveInfoPool.

  @retval EFI_SUCCESS             Successfully get the DriveInfoPool.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.

**/
EFI_STATUS
GetDriveInfoPool (
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This,
  OUT LIST_ENTRY                    **DriveInfoPool
  )
{
  H2O_SATA_DRIVE_INFO_PRIVATE_DATA  *Private;

  if (This == NULL || DriveInfoPool == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = H2O_SATA_DRIVE_INFO_PRIVATE_DATA_FROM_THIS (This);

  *DriveInfoPool = &Private->DriveInfoPool;

  return EFI_SUCCESS;
}

SATA_DEVICE_PATH  mSataDevicePathTemplate = {
  {
    MESSAGING_DEVICE_PATH,
    MSG_SATA_DP,
    {
      (UINT8) (sizeof (SATA_DEVICE_PATH)),
      (UINT8) ((sizeof (SATA_DEVICE_PATH)) >> 8)
    }
  },
  0,
  0,
  0
};

SCSI_DEVICE_PATH  mScsiDevicePathTemplate = {
  {
      MESSAGING_DEVICE_PATH,
      MSG_SCSI_DP,
      {
        (UINT8) (sizeof (SCSI_DEVICE_PATH)),
        (UINT8) ((sizeof (SCSI_DEVICE_PATH)) >> 8)
      }
  },
  0,
  0
};

UINT32  mControllerSerialNumber = 0;
UINT32  mDriveSerialNumber      = 0;

/**
  This function frees a pool of memory if the address is non-NULL and then resets
  the address to NULL so that future calls will do nothing.

  @param[in out]    Buffer              On entry, optional pointer to the
                                        address of the buffer to free or NULL.
                                        On exit, optional pointer to NULL.

**/
VOID
InternalFreePool (
  IN  VOID  **Buffer
  )
{
  if (Buffer != NULL && *Buffer != NULL) {
    FreePool (*Buffer);
    *Buffer = NULL;
  }
}

/**
  Frees an aligned buffer for ATA device.

  This function frees an aligned buffer for the ATA device to perform
  ATA pass through operations.

  @param  Buffer            The aligned buffer to be freed.
  @param  BufferSize        The request buffer size.

**/
STATIC
VOID
FreeAlignedBuffer (
  IN VOID                     *Buffer,
  IN UINTN                    BufferSize
  )
{
  if (Buffer != NULL) {
    FreeAlignedPages (Buffer, EFI_SIZE_TO_PAGES (BufferSize));
  }
}


/**
  Helper function to initialize the BUS_CONTROLLER_INFO instance of the controller.

  @param[in]   DeviceHandle     The handle of the controller.
  @param[out]  ControllerInfo   The pointer to the BUS_CONTROLLER_INFO instance. It is caller's
                                responsibility to free this buffer.

**/
EFI_STATUS
InitializeControllerInfo (
  IN  EFI_HANDLE            DeviceHandle,
  OUT BUS_CONTROLLER_INFO   **ControllerInfo
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *DupDevPathPtr;
  EFI_ATA_PASS_THRU_PROTOCOL      *AtaPassThru;
  EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ExtScsiPassThru;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath;
  EFI_HANDLE                      ParentDeviceHandle;
  PCI_TYPE00                      PciConfigSpace;

  DevicePath          = NULL;
  DupDevPathPtr       = NULL;
  AtaPassThru         = NULL;
  ExtScsiPassThru     = NULL;
  PciIo               = NULL;
  RemainingDevicePath = NULL;
  ParentDeviceHandle  = NULL;

  if (DeviceHandle == NULL || ControllerInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID **) &DevicePath);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (DeviceHandle, &gEfiAtaPassThruProtocolGuid, (VOID **) &AtaPassThru);
  if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
    return Status;
  }

  Status = gBS->HandleProtocol (DeviceHandle, &gEfiExtScsiPassThruProtocolGuid, (VOID **) &ExtScsiPassThru);
  if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
    return Status;
  }

  Status = gBS->HandleProtocol (DeviceHandle, &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_UNSUPPORTED) {
      RemainingDevicePath = DuplicateDevicePath (DevicePath);
      DupDevPathPtr = RemainingDevicePath;
      Status = gBS->LocateDevicePath (&gEfiPciIoProtocolGuid, &RemainingDevicePath, &ParentDeviceHandle);
      if (!EFI_ERROR (Status)) {
        Status = gBS->HandleProtocol (ParentDeviceHandle, &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
      }
      if (DupDevPathPtr != NULL) {
        FreePool (DupDevPathPtr);
      }
    }
  }
  if (!EFI_ERROR (Status)) {
    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0, sizeof (PCI_TYPE00), (VOID *) &PciConfigSpace);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Fail to Read PCI Configuration Space, Status: %r\n", Status));
    }
  }

  (*ControllerInfo) = (BUS_CONTROLLER_INFO *) AllocateZeroPool (sizeof (BUS_CONTROLLER_INFO));
  if ((*ControllerInfo) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  (*ControllerInfo)->Signature        = BUS_CONTROLLER_INFO_SIGNATURE;
  (*ControllerInfo)->DeviceHandle     = DeviceHandle;
  (*ControllerInfo)->DevicePath       = DuplicateDevicePath (DevicePath);
  (*ControllerInfo)->AtaPassThru      = AtaPassThru;
  (*ControllerInfo)->ExtScsiPassThru  = ExtScsiPassThru;
  (*ControllerInfo)->PciIo            = PciIo;
  CopyMem (&(*ControllerInfo)->PciConfigSpace, &PciConfigSpace, sizeof (PCI_TYPE00));
  InitializeListHead (&(*ControllerInfo)->ChildList);

  return EFI_SUCCESS;
}

/**
  This function traverses all supported controllers on the platform.

  @retval EFI_SUCCESS             Successfully intialize the Controller Info Pool.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval others                  Other failure occurs.

**/
VOID
InitializeControllerInfoPool (
  VOID
  )
{
  EFI_STATUS            Status;
  UINTN                 NoHandles;
  EFI_HANDLE            *HandleBuffer;
  UINTN                 HandleIndex;
  EFI_GUID              **ProtocolBuffer;
  UINTN                 ProtocolBufferCount;
  UINTN                 ProtocolIndex;
  BUS_CONTROLLER_INFO   *ControllerInfo;

  DEBUG ((EFI_D_INFO, "%a() - Start", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    Print (L"%a() - Fail to LocateHandleBuffer, Status: %r\n", __FUNCTION__, Status);
    return;
  }

  for (HandleIndex = 0; HandleIndex < NoHandles; HandleIndex++) {
    ProtocolBuffer      = NULL;
    ProtocolBufferCount = 0;

    Status = gBS->ProtocolsPerHandle (
                    HandleBuffer[HandleIndex],
                    &ProtocolBuffer,
                    &ProtocolBufferCount
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    for (ProtocolIndex = 0; ProtocolIndex < ProtocolBufferCount; ProtocolIndex++) {
      if (CompareGuid (ProtocolBuffer[ProtocolIndex], &gEfiAtaPassThruProtocolGuid)) {
        break;
      } else if (CompareGuid (ProtocolBuffer[ProtocolIndex], &gEfiExtScsiPassThruProtocolGuid)) {
        break;
      }
    }

    if (ProtocolIndex < ProtocolBufferCount) {
      Status = InitializeControllerInfo (HandleBuffer[HandleIndex], &ControllerInfo);
      if (!EFI_ERROR (Status)) {
        ControllerInfo->SerialNumber = mControllerSerialNumber++;
        InsertTailList (&mPrivateData.ControllerInfoPool, &ControllerInfo->Link);
      }
    }

    FreePool (ProtocolBuffer);
  }

  FreePool (HandleBuffer);

  DEBUG ((EFI_D_INFO, "%a() - End", __FUNCTION__));
}

/**
  Helper function to initialize the DISK_DRIVE_INFO instance of the target drive.

  @param[in]   AtaPassThru          The pointer to the ATA Pass-Through protocol.
  @param[in]   Port                 The port number of the target drive.
  @param[in]   PortMultiplierPort   The port multiplier port number of the target drive.
  @param[out]  DriveInfo            The pointer to the DISK_DRIVE_INFO instance. It is caller's
                                    responsibility to free this buffer.

**/
EFI_STATUS
AtaInitializeDriveInfo (
  IN  EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN  UINT16                      Port,
  IN  UINT16                      PortMultiplierPort,
  OUT DISK_DRIVE_INFO             **DriveInfo
  )
{
  EFI_STATUS                Status;
  ATA_IDENTIFY_DATA         *IdentifyData;
  ATA_SMART_DATA            *SmartData;
  EFI_STATUS                ReturnStatus;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;

  IdentifyData    = NULL;
  SmartData       = NULL;
  ReturnStatus    = EFI_UNSUPPORTED;
  DevicePathNode  = NULL;

  if (AtaPassThru == NULL || DriveInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AtaIdentifyData (AtaPassThru, Port, PortMultiplierPort, &IdentifyData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((IdentifyData->command_set_supported_82 & BIT0) == BIT0 &&
      (IdentifyData->command_set_feature_enb_85 & BIT0) == BIT0) {
    AtaSmartReadData (AtaPassThru, Port, PortMultiplierPort, &SmartData);
    AtaSmartReturnStatus (AtaPassThru, Port, PortMultiplierPort, &ReturnStatus);
  }

  Status = AtaPassThru->BuildDevicePath (AtaPassThru, Port, PortMultiplierPort, &DevicePathNode);
  if (EFI_ERROR (Status)) {
    DevicePathNode = AllocateCopyPool (sizeof (SATA_DEVICE_PATH), &mSataDevicePathTemplate);
    if (DevicePathNode == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    ((SATA_DEVICE_PATH *) DevicePathNode)->HBAPortNumber            = Port;
    ((SATA_DEVICE_PATH *) DevicePathNode)->PortMultiplierPortNumber = PortMultiplierPort;
    ((SATA_DEVICE_PATH *) DevicePathNode)->Lun                      = 0;
  }


  (*DriveInfo) = AllocateZeroPool (sizeof (DISK_DRIVE_INFO));
  if ((*DriveInfo) == NULL) {
    FreeAlignedBuffer ((VOID *) IdentifyData, sizeof (ATA_IDENTIFY_DATA));
    FreeAlignedBuffer ((VOID *) SmartData, sizeof (ATA_SMART_DATA));
    FreePool (DevicePathNode);
    return EFI_OUT_OF_RESOURCES;
  }

  (*DriveInfo)->Signature         = DISK_DRIVE_INFO_SIGNATURE;
  (*DriveInfo)->DevicePath        = DevicePathNode;
  (*DriveInfo)->IdentifyData      = IdentifyData;
  (*DriveInfo)->SmartData         = SmartData;
  (*DriveInfo)->SmartReturnStatus = ReturnStatus;

  return EFI_SUCCESS;
}

/**
  Helper function to initialize the DISK_DRIVE_INFO instance of the target drive.

  @param[in]   ExtScsiPassThru  The pointer to the Extended SCSI Pass-Through protocol.
  @param[in]   Target           The pointer to the ID that is an array of size TARGET_MAX_BYTES
                                of the target drive.
  @param[in]   Lun              The Lun of the target drive.
  @param[out]  DriveInfo        The pointer to the DISK_DRIVE_INFO instance. It is caller's
                                responsibility to free this buffer.

**/
EFI_STATUS
ExtScsiInitializeDriveInfo (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ExtScsiPassThru,
  IN  UINT8                           *Target,
  IN  UINT64                          Lun,
  OUT DISK_DRIVE_INFO                 **DriveInfo
  )
{
  EFI_STATUS                Status;
  ATA_IDENTIFY_DATA         *IdentifyData;
  ATA_SMART_DATA            *SmartData;
  EFI_STATUS                ReturnStatus;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;

  IdentifyData    = NULL;
  SmartData       = NULL;
  ReturnStatus    = EFI_UNSUPPORTED;
  DevicePathNode  = NULL;

  if (ExtScsiPassThru == NULL || Target == NULL || DriveInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SatIdentifyDevice (ExtScsiPassThru, Target, Lun, &IdentifyData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((IdentifyData->command_set_supported_82 & BIT0) == BIT0 &&
      (IdentifyData->command_set_feature_enb_85 & BIT0) == BIT0) {
    SatSmartReadData (ExtScsiPassThru, Target, Lun, &SmartData);
    SatSmartReturnStatus (ExtScsiPassThru, Target, Lun, &ReturnStatus);
  }

  Status = ExtScsiPassThru->BuildDevicePath (ExtScsiPassThru, Target, Lun, &DevicePathNode);
  if (EFI_ERROR (Status)) {
    DevicePathNode = AllocateCopyPool (sizeof (SCSI_DEVICE_PATH), &mScsiDevicePathTemplate);
    if (DevicePathNode == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    ((SCSI_DEVICE_PATH *) DevicePathNode)->Pun  = *((UINT16 *) Target);
    ((SCSI_DEVICE_PATH *) DevicePathNode)->Lun  = (UINT16) Lun;
  }

  (*DriveInfo) = AllocateZeroPool (sizeof (DISK_DRIVE_INFO));
  if ((*DriveInfo) == NULL) {
    FreeAlignedBuffer ((VOID *) IdentifyData, sizeof (ATA_IDENTIFY_DATA));
    FreeAlignedBuffer ((VOID *) SmartData, sizeof (ATA_SMART_DATA));
    FreePool (DevicePathNode);
    return EFI_OUT_OF_RESOURCES;
  }

  (*DriveInfo)->Signature         = DISK_DRIVE_INFO_SIGNATURE;
  (*DriveInfo)->DevicePath        = DevicePathNode;
  (*DriveInfo)->IdentifyData      = IdentifyData;
  (*DriveInfo)->SmartData         = SmartData;
  (*DriveInfo)->SmartReturnStatus = ReturnStatus;

  return EFI_SUCCESS;
}

/**
  Helper function to check if the DISK_DRIVE_INFO instance
  is duplicated with existing one.

  @param[in]  NewDriveInfo  The pointer to the DISK_DRIVE_INFO instance.

**/
BOOLEAN
IsDuplicated (
  IN  DISK_DRIVE_INFO   *NewDriveInfo
  )
{
  LIST_ENTRY        *Node;
  DISK_DRIVE_INFO   *DriveInfo;

  if (NewDriveInfo == NULL) {
    return FALSE;
  }

  BASE_LIST_FOR_EACH (Node, &mPrivateData.DriveInfoPool) {
    DriveInfo = DISK_DRIVE_INFO_FROM_LINK (Node);
    if (CompareMem (
          DriveInfo->IdentifyData->SerialNo,
          NewDriveInfo->IdentifyData->SerialNo,
          FIELD_SIZE_OF (ATA_IDENTIFY_DATA, SerialNo)) == 0 &&
        CompareMem (
          DriveInfo->IdentifyData->ModelName,
          NewDriveInfo->IdentifyData->ModelName,
          FIELD_SIZE_OF (ATA_IDENTIFY_DATA, ModelName)) == 0) {
      //
      // As a workaround for controllers that do not support sending SMART RETURN STATUS command
      // over their ATA Pass Through Protocol, Update the SmartReturnStatus field with the status
      // retrieved by sending the command over the Extended SCSI Pass Through Protocol.
      //
      if (DriveInfo->SmartReturnStatus == EFI_UNSUPPORTED) {
        DriveInfo->SmartReturnStatus = NewDriveInfo->SmartReturnStatus;
      }

      return TRUE;
    }
  }

  return FALSE;
}

/**
  Helper function to free the Disk Drive Info.

  @param[in]  DriveInfo   The pointer to the DISK_DRIVE_INFO instance.

**/
VOID
DestroyDriveInfo (
  IN  DISK_DRIVE_INFO   *DriveInfo
  )
{
  if (DriveInfo == NULL) {
    return;
  }

  if (DriveInfo->DevicePath != NULL) {
    FreePool (DriveInfo->DevicePath);
  }
  if (DriveInfo->IdentifyData != NULL) {
    FreeAlignedBuffer (DriveInfo->IdentifyData, sizeof (ATA_IDENTIFY_DATA));
  }
  if (DriveInfo->SmartData != NULL) {
    FreeAlignedBuffer (DriveInfo->SmartData, sizeof (ATA_SMART_DATA));
  }
  FreePool (DriveInfo);
}

/**
  This function traverses all drive on the controller.

  Drives exposed under ATA interface will be prefered.
  If a drive exposed under SCSI interface is duplicated with an existing one
  already exposed under ATA interface, it will be ignored.

  @param[in]  ControllerInfo      The pointer to the BUS_CONTROLLER_INFO instance.

  @retval EFI_SUCCESS             Successfully intialize the Drive Info Pool.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
InitializeDriveInfoPool (
  IN  BUS_CONTROLLER_INFO   *ControllerInfo
  )
{
  EFI_STATUS                      Status;
  EFI_ATA_PASS_THRU_PROTOCOL      *AtaPassThru;
  UINT16                          Port;
  UINT16                          PortMultiplierPort;
  DISK_DRIVE_INFO                 *DriveInfo;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ExtScsiPassThru;
  UINT8                           Target[TARGET_MAX_BYTES];
  UINT8                           *TargetPtr;
  UINT64                          Lun;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  if (ControllerInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (ControllerInfo->AtaPassThru != NULL) {
    AtaPassThru = ControllerInfo->AtaPassThru;

    Port = 0xFFFF;
    while (TRUE) {
      Status = AtaPassThru->GetNextPort (AtaPassThru, &Port);
      if (EFI_ERROR (Status)) {
        break;
      }

      PortMultiplierPort = 0xFFFF;
      while (TRUE) {
        Status = AtaPassThru->GetNextDevice (AtaPassThru, Port, &PortMultiplierPort);
        if (EFI_ERROR (Status)) {
          break;
        }

        DriveInfo = NULL;
        Status = AtaInitializeDriveInfo (AtaPassThru, Port, PortMultiplierPort, &DriveInfo);
        if (!EFI_ERROR (Status)) {
          DriveInfo->SerialNumber = mDriveSerialNumber++;
          DriveInfo->Parent = ControllerInfo;
          DevicePath = AppendDevicePathNode (ControllerInfo->DevicePath, DriveInfo->DevicePath);
          FreePool (DriveInfo->DevicePath);
          DriveInfo->DevicePath = DevicePath;
          InsertTailList (&mPrivateData.DriveInfoPool, &DriveInfo->Link);
          InsertTailList (&ControllerInfo->ChildList, &DriveInfo->ChildLink);
        }
      }
    }
  }

  if (ControllerInfo->ExtScsiPassThru != NULL) {
    ExtScsiPassThru = ControllerInfo->ExtScsiPassThru;

    SetMem ((VOID *) Target, TARGET_MAX_BYTES, 0xFF);
    TargetPtr = &Target[0];
    while (TRUE) {
      Status = ExtScsiPassThru->GetNextTargetLun (ExtScsiPassThru, &TargetPtr, &Lun);
      if (EFI_ERROR (Status)) {
        break;
      }

      DriveInfo = NULL;
      Status = ExtScsiInitializeDriveInfo (ExtScsiPassThru, TargetPtr, Lun, &DriveInfo);
      if (!EFI_ERROR (Status)) {
        if (IsDuplicated (DriveInfo)) {
          DestroyDriveInfo (DriveInfo);
          continue;
        }
        DriveInfo->SerialNumber = mDriveSerialNumber++;
        DriveInfo->Parent = ControllerInfo;
        DevicePath = AppendDevicePathNode (ControllerInfo->DevicePath, DriveInfo->DevicePath);
        FreePool (DriveInfo->DevicePath);
        DriveInfo->DevicePath = DevicePath;
        InsertTailList (&mPrivateData.DriveInfoPool, &DriveInfo->Link);
        InsertTailList (&ControllerInfo->ChildList, &DriveInfo->ChildLink);
      }
    }
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Helper function to free the DriveInfoPool.

**/
VOID
DestroyDriveInfoPool (
  VOID
  )
{
  LIST_ENTRY        *Entry;
  DISK_DRIVE_INFO   *DriveInfo;

  for (Entry = mPrivateData.DriveInfoPool.ForwardLink;
        Entry != &mPrivateData.DriveInfoPool;
       ){
    RemoveEntryList (Entry);
    DriveInfo = DISK_DRIVE_INFO_FROM_LINK (Entry);
    Entry = Entry->ForwardLink;
    RemoveEntryList (&DriveInfo->ChildLink);
    DestroyDriveInfo (DriveInfo);
  }
}

/**
  Helper function to rescan and rebuild the pools.

  @param[in]  This  Points to the H2O_SATA_DRIVE_INFO_PROTOCOL.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval others                  Other failure occurs.

**/
VOID
Refresh (
  IN  H2O_SATA_DRIVE_INFO_PROTOCOL  *This
  )
{
  LIST_ENTRY            *Node;
  BUS_CONTROLLER_INFO   *ControllerInfo;

  DestroyDriveInfoPool ();

  BASE_LIST_FOR_EACH (Node, &mPrivateData.ControllerInfoPool) {
    ControllerInfo = BUS_CONTROLLER_INFO_FROM_LINK (Node);
    InitializeDriveInfoPool (ControllerInfo);
  }
}

/**
  Helper function to traverse all the Controller Info and its children.

**/
VOID
Traverse (
  VOID
  )
{
  LIST_ENTRY            *Node;
  BUS_CONTROLLER_INFO   *ControllerInfo;
  UINTN                 ControllerCount;
  LIST_ENTRY            *ChildNode;
  DISK_DRIVE_INFO       *DriveInfo;
  UINTN                 DriveCount;
  CHAR16                *ModelNameStr;
  CHAR16                *SerialNumberStr;

  ControllerCount = 0;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  BASE_LIST_FOR_EACH (Node, &mPrivateData.ControllerInfoPool) {
    ControllerInfo = BUS_CONTROLLER_INFO_FROM_LINK (Node);

    DEBUG ((
      EFI_D_INFO,
      "Controller #%d - %04x:%04x\n",
      ControllerCount,
      ControllerInfo->PciConfigSpace.Hdr.VendorId,
      ControllerInfo->PciConfigSpace.Hdr.DeviceId
      ));
    ControllerCount++;

    DriveCount = 0;

    BASE_LIST_FOR_EACH (ChildNode, &ControllerInfo->ChildList) {
      DriveInfo       = DISK_DRIVE_INFO_FROM_CHILD_LINK (ChildNode);

      ModelNameStr    = RetrieveModelNameFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
      SerialNumberStr = RetrieveSerialNumberFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);

      DEBUG ((EFI_D_INFO, "  Drive #%d - %s (%s)\n", DriveCount, ModelNameStr, SerialNumberStr));
      DriveCount++;

      InternalFreePool ((VOID **) &ModelNameStr);
      InternalFreePool ((VOID **) &SerialNumberStr);
    }
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Callback function for check point gH2OBdsCpConnectAllAfterGuid.

  This function will build private data for H2O SATA Drive Info Protocol and install it.

  @param[in]  Event   A pointer to the Event that triggered the callback.
  @param[in]  Handle  Checkpoint handle.

**/
VOID
H2OBdsCpConnectAllAfterCallback (
  IN  EFI_EVENT       Event,
  IN  H2O_CP_HANDLE   Handle
  )
{
  EFI_STATUS            Status;
  LIST_ENTRY            *Node;
  BUS_CONTROLLER_INFO   *ControllerInfo;

  DEBUG ((EFI_D_INFO, "SataDriveInfoDxe %a: Start\n", __FUNCTION__));

  H2OCpUnregisterHandler (Handle);

  InitializeControllerInfoPool ();

  BASE_LIST_FOR_EACH (Node, &mPrivateData.ControllerInfoPool) {
    ControllerInfo = BUS_CONTROLLER_INFO_FROM_LINK (Node);
    InitializeDriveInfoPool (ControllerInfo);
  }

  DEBUG_CODE_BEGIN ();
  Traverse ();
  DEBUG_CODE_END ();

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivateData.Handle,
                  &gH2OSataDriveInfoProtocolGuid,
                  (VOID *) &mPrivateData.H2oSataDriveInfo,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to InstallMultipleProtocolInterfaces, Status = %r\n", __FUNCTION__, Status));
    return;
  }

  DEBUG ((EFI_D_INFO, "SataDriveInfoDxe %a: End\n", __FUNCTION__));
}

/**
  The Entry Point of SataDriveInfoDxe.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurred when executing this entry point.

**/
EFI_STATUS
EFIAPI
SataDriveInfoDxeEntry (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  H2O_SATA_DRIVE_INFO_PROTOCOL  *SataDriveInfo;
  H2O_CP_HANDLE                 H2OBdsCpConnectAllAfterHandle;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gH2OSataDriveInfoProtocolGuid,
                  NULL,
                  (VOID **) &SataDriveInfo
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  //
  // Collect storage devices information after connect all
  //
  if (FeaturePcdGet (PcdH2OBdsCpConnectAllAfterSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpConnectAllAfterGuid,
               H2OBdsCpConnectAllAfterCallback,
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