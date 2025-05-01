/** @file
  PEIM for autounlock Intel Remap devices for S3 resume

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/** @file
  Opal Password PEI driver which is used to unlock Opal Password for S3.

Copyright (c) 2016 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "OpalPasswordPei.h"
EFI_GUID mOpalDeviceAtaGuid = OPAL_DEVICE_ATA_GUID;
EFI_GUID mOpalDeviceNvmeGuid = OPAL_DEVICE_NVME_GUID;
EFI_GUID mOpalDeviceRemapNvmeGuid = OPAL_DEVICE_REMAP_NVME_GUID;  // RPPO-CNL-0102: RoyalParkOverrideContent

#define OPAL_PCIE_ROOTPORT_SAVESIZE               (0x40)
#define STORE_INVALID_ROOTPORT_INDEX              ((UINT8) -1)



/**
  Get IOMMU PPI.

  @return Pointer to IOMMU PPI.

**/
EDKII_IOMMU_PPI *
GetIoMmu (
  VOID
  )
{
  EFI_STATUS                Status;
  EDKII_IOMMU_PPI           *IoMmu;

  IoMmu = NULL;
  Status = PeiServicesLocatePpi (
             &gEdkiiIoMmuPpiGuid,
             0,
             NULL,
             (VOID **) &IoMmu
             );
  if (!EFI_ERROR (Status) && (IoMmu != NULL)) {
    return IoMmu;
  }

  return NULL;
}

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param Pages                  The number of pages to allocate.
  @param HostAddress            A pointer to store the base system memory address of the
                                allocated range.
  @param DeviceAddress          The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param Mapping                A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
EFI_STATUS
IoMmuAllocateBuffer (
  IN UINTN                  Pages,
  OUT VOID                  **HostAddress,
  OUT EFI_PHYSICAL_ADDRESS  *DeviceAddress,
  OUT VOID                  **Mapping
  )
{
  EFI_STATUS            Status;
  UINTN                 NumberOfBytes;
  EFI_PHYSICAL_ADDRESS  HostPhyAddress;
  EDKII_IOMMU_PPI       *IoMmu;

  *HostAddress = NULL;
  *DeviceAddress = 0;
  *Mapping = NULL;

  IoMmu = GetIoMmu ();

  if (IoMmu != NULL) {
    Status = IoMmu->AllocateBuffer (
                      IoMmu,
                      EfiBootServicesData,
                      Pages,
                      HostAddress,
                      0
                      );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    NumberOfBytes = EFI_PAGES_TO_SIZE (Pages);
    Status = IoMmu->Map (
                      IoMmu,
                      EdkiiIoMmuOperationBusMasterCommonBuffer,
                      *HostAddress,
                      &NumberOfBytes,
                      DeviceAddress,
                      Mapping
                      );
    if (EFI_ERROR (Status)) {
      IoMmu->FreeBuffer (IoMmu, Pages, *HostAddress);
      *HostAddress = NULL;
      return EFI_OUT_OF_RESOURCES;
    }
    Status = IoMmu->SetAttribute (
                      IoMmu,
                      *Mapping,
                      EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
                      );
    if (EFI_ERROR (Status)) {
      IoMmu->Unmap (IoMmu, *Mapping);
      IoMmu->FreeBuffer (IoMmu, Pages, *HostAddress);
      *Mapping = NULL;
      *HostAddress = NULL;
      return Status;
    }
  } else {
    Status = PeiServicesAllocatePages (
               EfiBootServicesData,
               Pages,
               &HostPhyAddress
               );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
    *HostAddress = (VOID *) (UINTN) HostPhyAddress;
    *DeviceAddress = HostPhyAddress;
    *Mapping = NULL;
  }
  return Status;
}

/**
  Frees memory that was allocated with AllocateBuffer().

  @param Pages              The number of pages to free.
  @param HostAddress        The base system memory address of the allocated range.
  @param Mapping            The mapping value returned from Map().

**/
VOID
IoMmuFreeBuffer (
  IN UINTN                  Pages,
  IN VOID                   *HostAddress,
  IN VOID                   *Mapping
  )
{
  EDKII_IOMMU_PPI       *IoMmu;

  IoMmu = GetIoMmu ();

  if (IoMmu != NULL) {
    IoMmu->SetAttribute (IoMmu, Mapping, 0);
    IoMmu->Unmap (IoMmu, Mapping);
    IoMmu->FreeBuffer (IoMmu, Pages, HostAddress);
  } else {
    FreePages (
      HostAddress,
      Pages
      );
  }
}

/**
  Provide IO action support.

  @param[in]     PeiDev             The opal device need to perform trusted IO.
  @param[in]     IoType             OPAL_IO_TYPE indicating whether to perform a Trusted Send or Trusted Receive.
  @param[in]     SecurityProtocol   Security Protocol
  @param[in]     SpSpecific         Security Protocol Specific
  @param[in]     TransferLength     Transfer Length of Buffer (in bytes) - always a multiple of 512
  @param[in]     Buffer             Address of Data to transfer

  @retval        EFI_SUCCESS        Perform the IO action success.
  @retval        Others             Perform the IO action failed.

**/
EFI_STATUS
PerformTrustedIo (
  OPAL_PEI_DEVICE  *PeiDev,
  OPAL_IO_TYPE     IoType,
  UINT8            SecurityProtocol,
  UINT16           SpSpecific,
  UINTN            TransferLength,
  VOID             *Buffer
  )
{
  EFI_STATUS                    Status;
  UINTN                         BufferSizeBlocks;
  EFI_ATA_COMMAND_BLOCK         AtaCommandBlock;
  OPAL_DEVICE_ATA               *DevInfoAta;
  AHCI_CONTEXT                  *AhciContext;
  NVME_CONTEXT                  *NvmeContext;

  Status = EFI_DEVICE_ERROR;
  if (PeiDev->DeviceType == OPAL_DEVICE_TYPE_ATA) {
    DevInfoAta = (OPAL_DEVICE_ATA *) PeiDev->Device;
    AhciContext = (AHCI_CONTEXT *) PeiDev->Context;

    BufferSizeBlocks = TransferLength / 512;

    ZeroMem( &AtaCommandBlock, sizeof( EFI_ATA_COMMAND_BLOCK ) );
    AtaCommandBlock.AtaCommand = ( IoType == OpalSend ) ? ATA_COMMAND_TRUSTED_SEND : ATA_COMMAND_TRUSTED_RECEIVE;
    AtaCommandBlock.AtaSectorCount = ( UINT8 )BufferSizeBlocks;
    AtaCommandBlock.AtaSectorNumber = ( UINT8 )( BufferSizeBlocks >> 8 );
    AtaCommandBlock.AtaFeatures = SecurityProtocol;
    AtaCommandBlock.AtaCylinderLow = ( UINT8 )( SpSpecific >> 8 );
    AtaCommandBlock.AtaCylinderHigh = ( UINT8 )( SpSpecific );
    AtaCommandBlock.AtaDeviceHead = ATA_DEVICE_LBA;


    ZeroMem( AhciContext->Buffer, HDD_PAYLOAD );
    ASSERT( TransferLength <= HDD_PAYLOAD );

    if (IoType == OpalSend) {
      CopyMem( AhciContext->Buffer, Buffer, TransferLength );
    }

    Status = AhciPioTransfer(
                AhciContext,
                (UINT8) DevInfoAta->Port,
                (UINT8) DevInfoAta->PortMultiplierPort,
                NULL,
                0,
                ( IoType == OpalSend ) ? FALSE : TRUE,   // i/o direction
                &AtaCommandBlock,
                NULL,
                AhciContext->Buffer,
                (UINT32)TransferLength,
                ATA_TIMEOUT
                );

    if (IoType == OpalRecv) {
      CopyMem( Buffer, AhciContext->Buffer, TransferLength );
    }
  } else if (PeiDev->DeviceType == OPAL_DEVICE_TYPE_NVME) {
    NvmeContext = (NVME_CONTEXT *) PeiDev->Context;
    Status = NvmeSecuritySendReceive (
                NvmeContext,
                IoType == OpalSend,
                SecurityProtocol,
                SwapBytes16(SpSpecific),
                TransferLength,
                Buffer
              );
  } else {
    DEBUG((DEBUG_ERROR, "DeviceType(%x) not support.\n", PeiDev->DeviceType));
  }

  return Status;
}



/**
  Send a security protocol command to a device that receives data and/or the result
  of one or more commands sent by SendData.

  The ReceiveData function sends a security protocol command to the given MediaId.
  The security protocol command sent is defined by SecurityProtocolId and contains
  the security protocol specific data SecurityProtocolSpecificData. The function
  returns the data from the security protocol command in PayloadBuffer.

  For devices supporting the SCSI command set, the security protocol command is sent
  using the SECURITY PROTOCOL IN command defined in SPC-4.

  For devices supporting the ATA command set, the security protocol command is sent
  using one of the TRUSTED RECEIVE commands defined in ATA8-ACS if PayloadBufferSize
  is non-zero.

  If the PayloadBufferSize is zero, the security protocol command is sent using the
  Trusted Non-Data command defined in ATA8-ACS.

  If PayloadBufferSize is too small to store the available data from the security
  protocol command, the function shall copy PayloadBufferSize bytes into the
  PayloadBuffer and return EFI_WARN_BUFFER_TOO_SMALL.

  If PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize is non-zero,
  the function shall return EFI_INVALID_PARAMETER.

  If the given MediaId does not support security protocol commands, the function shall
  return EFI_UNSUPPORTED. If there is no media in the device, the function returns
  EFI_NO_MEDIA. If the MediaId is not the ID for the current media in the device,
  the function returns EFI_MEDIA_CHANGED.

  If the security protocol fails to complete within the Timeout period, the function
  shall return EFI_TIMEOUT.

  If the security protocol command completes without an error, the function shall
  return EFI_SUCCESS. If the security protocol command completes with an error, the
  function shall return EFI_DEVICE_ERROR.

  @param  This                         Indicates a pointer to the calling context.
  @param  MediaId                      ID of the medium to receive data from.
  @param  Timeout                      The timeout, in 100ns units, to use for the execution
                                       of the security protocol command. A Timeout value of 0
                                       means that this function will wait indefinitely for the
                                       security protocol command to execute. If Timeout is greater
                                       than zero, then this function will return EFI_TIMEOUT
                                       if the time required to execute the receive data command
                                       is greater than Timeout.
  @param  SecurityProtocolId           The value of the "Security Protocol" parameter of
                                       the security protocol command to be sent.
  @param  SecurityProtocolSpecificData The value of the "Security Protocol Specific" parameter
                                       of the security protocol command to be sent.
  @param  PayloadBufferSize            Size in bytes of the payload data buffer.
  @param  PayloadBuffer                A pointer to a destination buffer to store the security
                                       protocol command specific payload data for the security
                                       protocol command. The caller is responsible for having
                                       either implicit or explicit ownership of the buffer.
  @param  PayloadTransferSize          A pointer to a buffer to store the size in bytes of the
                                       data written to the payload data buffer.

  @retval EFI_SUCCESS                  The security protocol command completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER        The PayloadBuffer or PayloadTransferSize is NULL and
                                       PayloadBufferSize is non-zero.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
EFIAPI
SecurityReceiveData (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
  IN  UINT32                                   MediaId,
  IN  UINT64                                   Timeout,
  IN  UINT8                                    SecurityProtocolId,
  IN  UINT16                                   SecurityProtocolSpecificData,
  IN  UINTN                                    PayloadBufferSize,
  OUT VOID                                     *PayloadBuffer,
  OUT UINTN                                    *PayloadTransferSize
  )
{
  OPAL_PEI_DEVICE               *PeiDev;

  PeiDev = OPAL_PEI_DEVICE_FROM_THIS (This);
  if (PeiDev == NULL) {
    return EFI_DEVICE_ERROR;
  }

  return PerformTrustedIo (
                        PeiDev,
                        OpalRecv,
                        SecurityProtocolId,
                        SecurityProtocolSpecificData,
                        PayloadBufferSize,
                        PayloadBuffer
                        );
}

/**
  Send a security protocol command to a device.

  The SendData function sends a security protocol command containing the payload
  PayloadBuffer to the given MediaId. The security protocol command sent is
  defined by SecurityProtocolId and contains the security protocol specific data
  SecurityProtocolSpecificData. If the underlying protocol command requires a
  specific padding for the command payload, the SendData function shall add padding
  bytes to the command payload to satisfy the padding requirements.

  For devices supporting the SCSI command set, the security protocol command is sent
  using the SECURITY PROTOCOL OUT command defined in SPC-4.

  For devices supporting the ATA command set, the security protocol command is sent
  using one of the TRUSTED SEND commands defined in ATA8-ACS if PayloadBufferSize
  is non-zero. If the PayloadBufferSize is zero, the security protocol command is
  sent using the Trusted Non-Data command defined in ATA8-ACS.

  If PayloadBuffer is NULL and PayloadBufferSize is non-zero, the function shall
  return EFI_INVALID_PARAMETER.

  If the given MediaId does not support security protocol commands, the function
  shall return EFI_UNSUPPORTED. If there is no media in the device, the function
  returns EFI_NO_MEDIA. If the MediaId is not the ID for the current media in the
  device, the function returns EFI_MEDIA_CHANGED.

  If the security protocol fails to complete within the Timeout period, the function
  shall return EFI_TIMEOUT.

  If the security protocol command completes without an error, the function shall return
  EFI_SUCCESS. If the security protocol command completes with an error, the function
  shall return EFI_DEVICE_ERROR.

  @param  This                         Indicates a pointer to the calling context.
  @param  MediaId                      ID of the medium to receive data from.
  @param  Timeout                      The timeout, in 100ns units, to use for the execution
                                       of the security protocol command. A Timeout value of 0
                                       means that this function will wait indefinitely for the
                                       security protocol command to execute. If Timeout is greater
                                       than zero, then this function will return EFI_TIMEOUT
                                       if the time required to execute the send data command
                                       is greater than Timeout.
  @param  SecurityProtocolId           The value of the "Security Protocol" parameter of
                                       the security protocol command to be sent.
  @param  SecurityProtocolSpecificData The value of the "Security Protocol Specific" parameter
                                       of the security protocol command to be sent.
  @param  PayloadBufferSize            Size in bytes of the payload data buffer.
  @param  PayloadBuffer                A pointer to a destination buffer to store the security
                                       protocol command specific payload data for the security
                                       protocol command.

  @retval EFI_SUCCESS                  The security protocol command completed successfully.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER        The PayloadBuffer is NULL and PayloadBufferSize is non-zero.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
EFIAPI
SecuritySendData (
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
  IN UINT32                                   MediaId,
  IN UINT64                                   Timeout,
  IN UINT8                                    SecurityProtocolId,
  IN UINT16                                   SecurityProtocolSpecificData,
  IN UINTN                                    PayloadBufferSize,
  IN VOID                                     *PayloadBuffer
  )
{
  OPAL_PEI_DEVICE               *PeiDev;

  PeiDev = OPAL_PEI_DEVICE_FROM_THIS (This);
  if (PeiDev == NULL) {
    return EFI_DEVICE_ERROR;
  }

  return PerformTrustedIo (
                          PeiDev,
                          OpalSend,
                          SecurityProtocolId,
                          SecurityProtocolSpecificData,
                          PayloadBufferSize,
                          PayloadBuffer
                          );

}

/**
  Save/Restore RootPort configuration space.

  @param[in]     DevInfoNvme            Pointer to NVMe device info.
  @param[in]     SaveAction             TRUE: Save, FALSE: Restore
  @param[in,out] PcieConfBufferList    Configuration space data buffer for save/restore

  @return PCIE base address of this RootPort
**/
UINTN
SaveRestoreRootportConfSpace (
  IN     OPAL_DEVICE_NVME               *DevInfoNvme,
  IN     BOOLEAN                        SaveAction,
  IN OUT UINT8                          **PcieConfBufferList
  )
{
  UINTN             RpBase;
  UINTN             Length;
  OPAL_PCI_DEVICE   *DevNode;
  UINT8             *StorePcieConfData;
  UINTN             Index;

  Length = 0;
  Index  = 0;
  RpBase = 0;

  while (sizeof (OPAL_DEVICE_NVME) + Length < DevInfoNvme->Length) {
    DevNode = (OPAL_PCI_DEVICE *)((UINT8*)DevInfoNvme->PciBridgeNode + Length);
    RpBase = PCI_LIB_ADDRESS (DevNode->Bus, DevNode->Device, DevNode->Function, 0x0);

    if (PcieConfBufferList != NULL) {
      if (SaveAction) {
        StorePcieConfData = (UINT8 *) AllocateZeroPool (OPAL_PCIE_ROOTPORT_SAVESIZE);
        ASSERT (StorePcieConfData != NULL);
        OpalPciRead (StorePcieConfData, RpBase, OPAL_PCIE_ROOTPORT_SAVESIZE);
        PcieConfBufferList[Index] = StorePcieConfData;
      } else {
        // Skip PCIe Command & Status registers
        StorePcieConfData = PcieConfBufferList[Index];
        OpalPciWrite (RpBase, StorePcieConfData, 4);
        OpalPciWrite (RpBase + 8, StorePcieConfData + 8, OPAL_PCIE_ROOTPORT_SAVESIZE - 8);

        FreePool (StorePcieConfData);
      }
    }

    Length += sizeof (OPAL_PCI_DEVICE);
    Index ++;
  }

  return RpBase;
}

/**
  Configure RootPort for downstream PCIe NAND devices.

  @param[in] RpBase             - PCIe configuration space address of this RootPort
  @param[in] BusNumber          - Bus number
  @param[in] MemoryBase         - Memory base address
  @param[in] MemoryLength       - Memory size

**/
VOID
ConfigureRootPortForPcieNand (
  IN UINTN   RpBase,
  IN UINTN   BusNumber,
  IN UINT32  MemoryBase,
  IN UINT32  MemoryLength
  )
{
  UINT32  MemoryLimit;

  DEBUG ((DEBUG_INFO, "ConfigureRootPortForPcieNand, BusNumber: %x, MemoryBase: %x, MemoryLength: %x\n",
    BusNumber, MemoryBase, MemoryLength));

  if (MemoryLength == 0) {
    MemoryLimit = MemoryBase;
  } else {
    MemoryLimit = MemoryBase + MemoryLength + 0xFFFFF; // 1M
  }

  ///
  /// Configue PCIE configuration space for RootPort
  ///
  PciWrite8  (RpBase + NVME_PCIE_BNUM + 1,  (UINT8) BusNumber);           // Secondary Bus Number registers
  PciWrite8  (RpBase + NVME_PCIE_BNUM + 2,  (UINT8) BusNumber);           // Subordinate Bus Number registers
  PciWrite8  (RpBase + NVME_PCIE_IOBL,      0xFF);                        // I/O Base registers
  PciWrite8  (RpBase + NVME_PCIE_IOBL + 1,  0x00);                        // I/O Limit registers
  PciWrite16 (RpBase + NVME_PCIE_MBL,       (UINT16) RShiftU64 ((UINTN)MemoryBase, 16));  // Memory Base register
  PciWrite16 (RpBase + NVME_PCIE_MBL + 2,   (UINT16) RShiftU64 ((UINTN)MemoryLimit, 16)); // Memory Limit register
  PciWrite16 (RpBase + NVME_PCIE_PMBL,      0xFFFF);                      // Prefetchable Memory Base registers
  PciWrite16 (RpBase + NVME_PCIE_PMBL + 2,  0x0000);                      // Prefetchable Memory Limit registers
  PciWrite32 (RpBase + NVME_PCIE_PMBU32,    0xFFFFFFFF);                  // Prefetchable Memory Upper Base registers
  PciWrite32 (RpBase + NVME_PCIE_PMLU32,    0x00000000);                  // Prefetchable Memory Upper Limit registers
}

/**

  The function returns whether or not the device is Opal Locked.
  TRUE means that the device is partially or fully locked.
  This will perform a Level 0 Discovery and parse the locking feature descriptor

  @param[in]      OpalDev             Opal object to determine if locked.
  @param[out]     BlockSidSupported   Whether device support BlockSid feature.

**/
BOOLEAN
IsOpalDeviceLocked(
  OPAL_PEI_DEVICE    *OpalDev,
  BOOLEAN            *BlockSidSupported
  )
{
  OPAL_SESSION                   Session;
  OPAL_DISK_SUPPORT_ATTRIBUTE    SupportedAttributes;
  TCG_LOCKING_FEATURE_DESCRIPTOR LockingFeature;
  UINT16                         OpalBaseComId;
  TCG_RESULT                     Ret;

  Session.Sscp = &OpalDev->Sscp;
  Session.MediaId = 0;

  Ret = OpalGetSupportedAttributesInfo (&Session, &SupportedAttributes, &OpalBaseComId);
  if (Ret != TcgResultSuccess) {
    return FALSE;
  }

  Session.OpalBaseComId  = OpalBaseComId;
  *BlockSidSupported     = SupportedAttributes.BlockSid == 1 ? TRUE : FALSE;

  Ret = OpalGetLockingInfo(&Session, &LockingFeature);
  if (Ret != TcgResultSuccess) {
    return FALSE;
  }

  return OpalDeviceLocked (&SupportedAttributes, &LockingFeature);
}

/**
  Unlock OPAL password for S3.

  @param[in] OpalDev            Opal object to unlock.

**/
VOID
UnlockOpalPassword (
  IN OPAL_PEI_DEVICE            *OpalDev
  )
{
  TCG_RESULT                    Result;
  OPAL_SESSION                  Session;
  BOOLEAN                       BlockSidSupport;
  UINT32                        PpStorageFlags;
  BOOLEAN                       BlockSIDEnabled;

  BlockSidSupport = FALSE;
  if (IsOpalDeviceLocked (OpalDev, &BlockSidSupport)) {
    ZeroMem(&Session, sizeof (Session));
    Session.Sscp = &OpalDev->Sscp;
    Session.MediaId = 0;
    Session.OpalBaseComId = OpalDev->Device->OpalBaseComId;

    Result = OpalUtilUpdateGlobalLockingRange (
               &Session,
               OpalDev->Device->Password,
               OpalDev->Device->PasswordLength,
               FALSE,
               FALSE
               );
    DEBUG ((
      DEBUG_INFO,
      "%a() OpalUtilUpdateGlobalLockingRange() Result = 0x%x\n",
      __FUNCTION__,
      Result
      ));
  }

  PpStorageFlags = Tcg2PhysicalPresenceLibGetManagementFlags ();
  if ((PpStorageFlags & TCG2_BIOS_STORAGE_MANAGEMENT_FLAG_ENABLE_BLOCK_SID) != 0) {
    BlockSIDEnabled = TRUE;
  } else {
    BlockSIDEnabled = FALSE;
  }
  if (BlockSIDEnabled && BlockSidSupport) {
    ZeroMem(&Session, sizeof (Session));
    Session.Sscp = &OpalDev->Sscp;
    Session.MediaId = 0;
    Session.OpalBaseComId = OpalDev->Device->OpalBaseComId;
    Result = OpalBlockSid (&Session, TRUE);
    DEBUG ((
      DEBUG_INFO,
      "%a() OpalBlockSid() Result = 0x%x\n",
      __FUNCTION__,
      Result
      ));
  }
}

//
// RPPO-CNL-0101: RoyalParkOverrideBegin
//
/**
  Convert port number from bitmap fomat to phsical port number for the SATA drive on RST mode.

  @param[in, out] Port                The port number of SATA drive.
  @param[in]      PortMultiplierPort  The port multiplier number of SATA drive.

**/
VOID
GetPhysicalPortNum (
  IN OUT UINT16        *Port,
  IN UINT16            PortMultiplierPort
  )
{
  UINT16 TmpValue;
  UINT16 PhsicalPortNum;

  if (Port == NULL) {
    return;
  }

  //
  // Check if SATA drive under RST driver controlled.
  //
  if (PortMultiplierPort == 0x8000) {
    if (*Port < (1 << GetPchMaxSataPortNum (SATA_1_CONTROLLER_INDEX))) {
      //
      // Convert process
      //
      PhsicalPortNum = 0;
      TmpValue = *Port;
      while (TmpValue != 1) {
        TmpValue = TmpValue >> 1;
        PhsicalPortNum ++;
      }

      *Port = PhsicalPortNum;
    }
  }
}


/**
  Unlock ATA OPAL password for S3.

**/
VOID
UnlockOpalPasswordAta (
  VOID
  )
{
  EFI_STATUS                    Status;
  OPAL_DEVICE_ATA               *DevInfoAta;
  UINT8                         Bus;
  UINT8                         Device;
  UINT8                         Function;
  OPAL_PEI_DEVICE               OpalDev;
  UINT8                         BaseClassCode;
  UINT8                         SubClassCode;
  UINT8                         SataCmdSt;
  AHCI_CONTEXT                  AhciContext;
  UINT32                        AhciBar;
  UINT32                        OrgAhciBar;
  
  UINTN                         IntelRemapDevInfoTableSize; 
  OPAL_DEVICE_INTEL_REMAP        *DevInfoPtr;
  OPAL_DEVICE_INTEL_REMAP        *DevInfo;
  UINTN                          NumOfDevInfoTable;
  UINTN                          DevInfoTableIndex;  

  UINT32                         *RaidControllerMmiosBaseAddress;  

  OPAL_SESSION                   Session;  
  OPAL_DISK_SUPPORT_ATTRIBUTE    SupportedAttributes;
  
  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  IntelRemapDevInfoTableSize = 0;
  DevInfoPtr  = NULL;
  RaidControllerMmiosBaseAddress = (UINT32*) PcdGetPtr (PcdAhciPcieMemBaseAddress);
  DevInfoAta    = NULL;
  DevInfo       = NULL;

  Status = CommonGetVariableDataAndSize (
             SAVE_HDD_PASSWORD_FOR_REMAP_DEVICE_NAME,
             &gSaveIntelRemapDevInfoGuid,
             &IntelRemapDevInfoTableSize,
             (VOID **) &DevInfo
             );

  if (EFI_ERROR (Status)) {
    return;
  }

  NumOfDevInfoTable = IntelRemapDevInfoTableSize / sizeof(OPAL_DEVICE_INTEL_REMAP);
  DevInfoPtr   = (OPAL_DEVICE_INTEL_REMAP*)DevInfo;
  for (DevInfoTableIndex = 0; DevInfoTableIndex < NumOfDevInfoTable; DevInfoTableIndex++) {
    if (DevInfoPtr[DevInfoTableIndex].DeviceType != OPAL_DEVICE_TYPE_ATA) {
      continue;
    }
    
    Bus = DevInfoPtr[DevInfoTableIndex].Bus;
    Device = DevInfoPtr[DevInfoTableIndex].Device;
    Function = DevInfoPtr[DevInfoTableIndex].Function;

    SataCmdSt = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET));
    PciWrite8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), 0x6);

    BaseClassCode = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x0B));
    SubClassCode  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x0A));
    if ((BaseClassCode != PCI_CLASS_MASS_STORAGE) ||
        ((SubClassCode != PCI_CLASS_MASS_STORAGE_SATADPA) && (SubClassCode != PCI_CLASS_MASS_STORAGE_RAID))) {
      DEBUG ((DEBUG_ERROR, "%a() ClassCode/SubClassCode are not supported\n", __FUNCTION__));
    } else {
      OrgAhciBar = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x24));
      //
      // If there's no Abar, use deviceinfo2 PcdH2ODeviceInfoOnboradAhci
      //
      if (OrgAhciBar == 0) {
        PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x24), RaidControllerMmiosBaseAddress[0]);
        AhciBar = RaidControllerMmiosBaseAddress[0];
      } else {
         AhciBar = OrgAhciBar; 
      }

      GetPhysicalPortNum (&(DevInfoPtr[DevInfoTableIndex].Port), DevInfoPtr[DevInfoTableIndex].PortMultiplierPort);  // RPPO-CNL-0101: RoyalParkOverrideContent

      ZeroMem (&AhciContext, sizeof (AHCI_CONTEXT));
      AhciContext.AhciBar = AhciBar;
      AhciAllocateResource (&AhciContext);
      Status = AhciModeInitialize (&AhciContext, (UINT8)DevInfoPtr[DevInfoTableIndex].Port);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a() AhciModeInitialize() error, Status: %r\n", __FUNCTION__, Status));
      } else {
        OpalDev.Signature = OPAL_PEI_DEVICE_SIGNATURE;
        OpalDev.Sscp.ReceiveData = SecurityReceiveData;
        OpalDev.Sscp.SendData = SecuritySendData;
        OpalDev.DeviceType = OPAL_DEVICE_TYPE_ATA;
        DevInfoAta = AllocateZeroPool(sizeof(OPAL_DEVICE_ATA));
        DevInfoAta->BarAddr = AhciBar;        
        CopyMem(DevInfoAta->Password, DevInfoPtr[DevInfoTableIndex].Password, DevInfoPtr[DevInfoTableIndex].PasswordLength);
        DevInfoAta->PasswordLength = DevInfoPtr[DevInfoTableIndex].PasswordLength;
        DevInfoAta->Port           = DevInfoPtr[DevInfoTableIndex].Port;
        DevInfoAta->PortMultiplierPort = DevInfoPtr[DevInfoTableIndex].PortMultiplierPort;
        OpalDev.Device = (OPAL_DEVICE_COMMON *) DevInfoAta;
        OpalDev.Context = &AhciContext;
        Session.Sscp = &OpalDev.Sscp;
        Session.MediaId = 0;
        OpalGetSupportedAttributesInfo(&Session, &SupportedAttributes, &DevInfoAta->OpalBaseComId);      
        UnlockOpalPassword (&OpalDev);
      }
      AhciFreeResource (&AhciContext);
      PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x24), OrgAhciBar);
      PciWrite8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), SataCmdSt);
    }
    
  }

  if (DevInfo != NULL) {
    FreePool(DevInfo);
  }


  if (DevInfoAta != NULL) {
    FreePool(DevInfoAta);
  }

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));
}


/**
  Initialize the Opal disk base on the hardware info get from device.

  @param[in]  Sscp            Point to EFI_STORAGE_SECURITY_COMMAND_PROTOCOL.
  @param[in]  MediaId         Media ID.
  @param[out] OpalDisk        The Opal device.

  @retval EFI_SUCCESS         Initialize the device success.
  @retval EFI_DEVICE_ERROR    Get info from device failed.

**/
BOOLEAN
OpalUpdateAhciServePasswordAttribute (
  IN     EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Sscp,
  IN     UINT16                                 MediaId
  )
{
  OPAL_SESSION                       Session;
  TCG_SUPPORTED_SECURITY_PROTOCOLS   *SupportedProtocols;
  UINT8                              Buffer[BUFFER_SIZE];  
  
  ZeroMem(&Session, sizeof (Session));
  Session.Sscp = Sscp;
  Session.MediaId = MediaId;
  ZeroMem(Buffer, BUFFER_SIZE);
  ASSERT(sizeof(Buffer) >= sizeof(TCG_SUPPORTED_SECURITY_PROTOCOLS));

  //
  // Retrieve supported protocols verify security protocol 1 is supported
  //
  SupportedProtocols = (TCG_SUPPORTED_SECURITY_PROTOCOLS*) Buffer;

  if (OpalRetrieveSupportedProtocolList (&Session, 
                                        sizeof(TCG_SUPPORTED_SECURITY_PROTOCOLS), 
                                        SupportedProtocols) == TcgResultFailure) {
    DEBUG ((DEBUG_INFO, "OpalRetrieveSupportedProtocolList failed\n"));
    return FALSE;
  }

  if (TcgIsProtocolSupported (SupportedProtocols, TCG_SECURITY_PROTOCOL_ATA_DEVICE_SERVER_PASS)) {
    return TRUE;
  }

  return FALSE;
}



/**
  Unlock re-mapped NVMe OPAL password for S3.

**/
VOID
UnlockOpalPasswordRemapNvme (
  VOID
  )
{
  EFI_STATUS                    Status;
  OPAL_DEVICE_COMMON            *DevInfoRemap;
  OPAL_PEI_DEVICE               OpalDev;
  UINT8                         BaseClassCode;
  UINT8                         SubClassCode;
  UINT8                         SataCmdSt;
  UINT32                        AhciBar;
  UINT32                        OrgAhciBar;

  PEI_OPAL_DEVICE_INFO_PPI      *OpalDeviceInfoPpi;
  PEI_OPAL_DEVICE_INFO          OpalDeviceInfo;
  UINTN                         RemapMmiosBaseAddress;
  UINT32                        *RaidControllerMmiosBaseAddress;
  UINT32                        NvmeNamespaceId;
  NVME_CONTEXT                  NvmeContext;

  UINTN                         IntelRemapDevInfoTableSize; 

  OPAL_DEVICE_INTEL_REMAP        *DevInfoPtr;
  OPAL_DEVICE_INTEL_REMAP        *DevInfo;
  UINTN                          NumOfDevInfoTable;
  UINTN                          DevInfoTableIndex;  
  OPAL_SESSION                   Session;  
  OPAL_DISK_SUPPORT_ATTRIBUTE    SupportedAttributes;
  BOOLEAN                        SupportAhciServerPassword;
  ATA_DEVICE_SERVER_SEND_CMD_DATA*AtaSecuritySendData;
  UINT16                         SecurityProtocolSpecificData;
  
  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  
  IntelRemapDevInfoTableSize = 0;
  DevInfoPtr  = NULL;
  DevInfo     = NULL;
  RaidControllerMmiosBaseAddress = (UINT32*) PcdGetPtr (PcdAhciPcieMemBaseAddress);
  NvmeNamespaceId = 0;  
  DevInfoRemap    = NULL;
  SupportAhciServerPassword          = FALSE;
  
  Status = CommonGetVariableDataAndSize (
             SAVE_HDD_PASSWORD_FOR_REMAP_DEVICE_NAME,
             &gSaveIntelRemapDevInfoGuid,
             &IntelRemapDevInfoTableSize,
             (VOID **) &DevInfo
             );

  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Locate PPI whcih should be installed by pre-OS RST driver
  //
  OpalDeviceInfoPpi = NULL;
  ZeroMem (&OpalDeviceInfo, sizeof (PEI_OPAL_DEVICE_INFO));

  Status = PeiServicesLocatePpi (
             &gPeiOpalDeviceInfoPpiGuid,
             0,
             NULL,
             (VOID **) &OpalDeviceInfoPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a() Locate OpalDeviceInfoPpi error, Status: %r\n", __FUNCTION__, Status));
    return;
  }
  
  NumOfDevInfoTable = IntelRemapDevInfoTableSize / sizeof(OPAL_DEVICE_INTEL_REMAP);
  DevInfoPtr   = (OPAL_DEVICE_INTEL_REMAP*)DevInfo;
  for (DevInfoTableIndex = 0; DevInfoTableIndex < NumOfDevInfoTable; DevInfoTableIndex++) {
     //
     // If not remapped device skip
     //
     if (DevInfoPtr[DevInfoTableIndex].DeviceType != OPAL_DEVICE_TYPE_REMAP_NVME) {
       continue;
     }
     
     OpalDeviceInfo.Bus = DevInfoPtr[DevInfoTableIndex].Bus;
     OpalDeviceInfo.Device = DevInfoPtr[DevInfoTableIndex].Device;
     OpalDeviceInfo.Function = DevInfoPtr[DevInfoTableIndex].Function;
     OpalDeviceInfo.Port = DevInfoPtr[DevInfoTableIndex].Port;
     OpalDeviceInfo.PortMultiplierPort = DevInfoPtr[DevInfoTableIndex].PortMultiplierPort;
     SataCmdSt = PciRead8 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, PCI_COMMAND_OFFSET));
     PciWrite8 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, PCI_COMMAND_OFFSET), 0x6);
     BaseClassCode = PciRead8 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, 0x0B));
     SubClassCode  = PciRead8 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, 0x0A));
     if ((BaseClassCode != PCI_CLASS_MASS_STORAGE) ||
         ((SubClassCode != PCI_CLASS_MASS_STORAGE_SATADPA) && (SubClassCode != PCI_CLASS_MASS_STORAGE_RAID))) {
       DEBUG ((DEBUG_ERROR, "%a() ClassCode/SubClassCode are not supported\n", __FUNCTION__));
       return;
     } 
     OrgAhciBar = PciRead32 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, 0x24));
     //
     // If there's no Abar, use deviceinfo2 PcdH2ODeviceInfoOnboradAhci
     //
     if (OrgAhciBar == 0) {
       PciWrite32 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, 0x24), RaidControllerMmiosBaseAddress[0]);
       AhciBar = RaidControllerMmiosBaseAddress[0];
     } else {
        AhciBar = OrgAhciBar; 
     }
     

     Status = OpalDeviceInfoPpi->GetDeviceInfo (OpalDeviceInfoPpi, &OpalDeviceInfo, &RemapMmiosBaseAddress, &NvmeNamespaceId);
     if (EFI_ERROR(Status)) {
       return;
     }
     DevInfoRemap = AllocateZeroPool(sizeof(OPAL_DEVICE_COMMON));
     DevInfoRemap->BarAddr = RemapMmiosBaseAddress;
     CopyMem(DevInfoRemap->Password, DevInfoPtr[DevInfoTableIndex].Password, DevInfoPtr[DevInfoTableIndex].PasswordLength);
     DevInfoRemap->PasswordLength = (UINT8)DevInfoPtr[DevInfoTableIndex].PasswordLength;
     
     ZeroMem (&NvmeContext, sizeof (NVME_CONTEXT));
     NvmeContext.Nbar = RemapMmiosBaseAddress;
     NvmeContext.PciBase = PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, 0x0);
     NvmeContext.NvmeInitWaitTime = 0;
     NvmeContext.Nsid = NvmeNamespaceId;
     NvmeContext.Remapped = TRUE;
     NvmeAllocateResource (&NvmeContext);
     Status = NvmeControllerInit (&NvmeContext);

     OpalDev.Signature = OPAL_PEI_DEVICE_SIGNATURE;
     OpalDev.Sscp.ReceiveData = SecurityReceiveData;
     OpalDev.Sscp.SendData = SecuritySendData;
     OpalDev.DeviceType = OPAL_DEVICE_TYPE_NVME;
     OpalDev.Device = (OPAL_DEVICE_COMMON *) DevInfoRemap;
     OpalDev.Context = &NvmeContext;
      
     Session.Sscp = &OpalDev.Sscp;
     Session.MediaId = 0;
     SupportAhciServerPassword = OpalUpdateAhciServePasswordAttribute(Session.Sscp, (UINT16)Session.MediaId);     
     
     if (SupportAhciServerPassword) {
       AtaSecuritySendData = AllocateZeroPool(sizeof (ATA_DEVICE_SERVER_SEND_CMD_DATA));
       
       AtaSecuritySendData->Byte1 = USER_PSW;
       CopyMem (&AtaSecuritySendData->Password, DevInfoPtr[DevInfoTableIndex].Password, DevInfoPtr[DevInfoTableIndex].PasswordLength);
       SecurityProtocolSpecificData = ATA_SECURITY_UNLOCK;
       Status = SecuritySendData(
                  &OpalDev.Sscp,
                  0,
                  TRUSTED_COMMAND_TIMEOUT_NS,
                  TCG_SECURITY_PROTOCOL_ATA_DEVICE_SERVER_PASS,
                  SwapBytes16(SecurityProtocolSpecificData),
                  sizeof(ATA_DEVICE_SERVER_SEND_CMD_DATA),
                  AtaSecuritySendData
                  );
       
       FreePool(AtaSecuritySendData);
     } else {
        OpalGetSupportedAttributesInfo(&Session, &SupportedAttributes, &DevInfoRemap->OpalBaseComId);      
        UnlockOpalPassword (&OpalDev);
     }
     Status = NvmeControllerExit (&NvmeContext);
     NvmeFreeResource (&NvmeContext);
     PciWrite32 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, 0x24), OrgAhciBar);
     PciWrite8 (PCI_LIB_ADDRESS (OpalDeviceInfo.Bus, OpalDeviceInfo.Device, OpalDeviceInfo.Function, PCI_COMMAND_OFFSET), SataCmdSt);  
  }


  if (DevInfo != NULL) {
    FreePool(DevInfo);
  }

  if (OpalDeviceInfoPpi != NULL) {
    FreePool(OpalDeviceInfoPpi);
  }

  if (DevInfoRemap != NULL) {
    FreePool(DevInfoRemap);
  }

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));
}


/**
  Unlock OPAL password for S3.

**/
VOID
OpalPasswordS3 (
  VOID
  )
{
  UnlockOpalPasswordAta ();
  UnlockOpalPasswordRemapNvme ();  // RPPO-CNL-0102: RoyalParkOverrideContent
}

/**
  Entry point of the notification callback function itself within the PEIM.
  It is to unlock OPAL password for S3.

  @param  PeiServices      Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor Address of the notification descriptor data structure.
  @param  Ppi              Address of the PPI that was installed.

  @return Status of the notification.
          The status code returned from this function is ignored.
**/
EFI_STATUS
EFIAPI
OpalPasswordEndOfPeiNotify(
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_BOOT_MODE                     BootMode;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  if (BootMode != BOOT_ON_S3_RESUME) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a() - enter at S3 resume\n", __FUNCTION__));

  OpalPasswordS3 ();

  DEBUG ((DEBUG_INFO, "%a() - exit at S3 resume\n", __FUNCTION__));

  return EFI_SUCCESS;
}

EFI_PEI_NOTIFY_DESCRIPTOR mOpalPasswordEndOfPeiNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  OpalPasswordEndOfPeiNotify
};

/**
  Main entry for this module.

  @param FileHandle             Handle of the file being invoked.
  @param PeiServices            Pointer to PEI Services table.

  @return Status from PeiServicesNotifyPpi.

**/
EFI_STATUS
EFIAPI
RemapPwdPeiInit (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;

  Status = PeiServicesNotifyPpi (&mOpalPasswordEndOfPeiNotifyDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

