/** @file
  Hdd password auto unlock PEIM

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "HddPasswordPei.h"


/**
  compare Pci device path

  @retval           True - same PCI devicepath
**/
BOOLEAN
CompareHostDpNode (
  IN  EFI_DEVICE_PATH_PROTOCOL              *ControllerDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL              *DevicePath
  )
{

  if (ControllerDevicePath == NULL || DevicePath == NULL) {
    return FALSE;
  }

  while (!IsDevicePathEnd (ControllerDevicePath) && !IsDevicePathEnd (DevicePath)) {
    if (ControllerDevicePath->Type == MESSAGING_DEVICE_PATH && (ControllerDevicePath->SubType == MSG_SATA_DP ||
        ControllerDevicePath->SubType == MSG_NVME_NAMESPACE_DP)) {
      break;
    }

    if (CompareMem(ControllerDevicePath, DevicePath, DevicePathNodeLength(ControllerDevicePath))) {
      return FALSE;
    }
    ControllerDevicePath = NextDevicePathNode (ControllerDevicePath);
    DevicePath = NextDevicePathNode (DevicePath);
  }

  return TRUE;
}


BOOLEAN
IsIntelVmd (
  IN  EFI_DEVICE_PATH_PROTOCOL              *DevicePath
  )
{
  PCI_DEVICE_PATH  *PciDp;

  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH && DevicePath->SubType == HW_PCI_DP) {
      PciDp = (PCI_DEVICE_PATH *) DevicePath;
      if (PciDp->Device == 0xe && PciDp->Function == 0) {
        return TRUE;
      }
    }
    DevicePath = NextDevicePathNode (DevicePath);
  }

  return FALSE;
}




/**
  This function converts Unicode string to ASCII string.

  @param[in]        RawPasswordPtr      Password string address.
  @param[in]        RawPasswordLength   Password string length.
  @param[OUT]       PasswordToHdd       Encode password address.
  @param[OUT]       PasswordToHddLength Encode string length.

  @retval           EFI_SUCCESS
**/
EFI_STATUS
HddPasswordStringProcess (
  IN VOID                               *RawPasswordPtr,
  IN UINTN                              RawPasswordLength,
  OUT VOID                              **PasswordToHdd
  )
{
  UINTN                                 Index;
  CHAR16                                *RawPassword;
  UINT8                                 *ProcessedPassword;

  if (RawPasswordLength == 0) {
    EFI_NOT_FOUND;
  }

  RawPassword = RawPasswordPtr;
  ProcessedPassword = (UINT8 *)PasswordToHdd;

  for (Index = 0; Index < RawPasswordLength; Index++) {
    //
    // Unicode to ASCII
    //

    ProcessedPassword[Index] = (UINT8)RawPassword[Index];
  }
  return EFI_SUCCESS;
}



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
  );

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
  );



OPAL_PEI_DEVICE  mOpalDevTemplate = {
  OPAL_PEI_DEVICE_SIGNATURE,
  {
    SecurityReceiveData,
    SecuritySendData
  },
  NULL,  //*SscPpi
  0,      //DeviceIndex
  0,      //PasswordLength
  0       //Password
};

/**

  The function returns whether or not the device is Opal Locked.
  TRUE means that the device is partially or fully locked.
  This will perform a Level 0 Discovery and parse the locking feature descriptor

  @param[in]      OpalDev             Opal object to determine if locked.
  @param[out]     BlockSidSupported   Whether device support BlockSid feature.

  @retval TRUE - it's locked by TCG OPAL.
  @retval FALE - it's not locked by TCG OPAL.


**/
BOOLEAN
IsOpalDeviceLocked(
  OPAL_SESSION                           *Session
  )
{
  OPAL_DISK_SUPPORT_ATTRIBUTE    SupportedAttributes;
  TCG_LOCKING_FEATURE_DESCRIPTOR LockingFeature;
  TCG_RESULT                     Ret;
  UINT16                         OpalBaseComId;


  Ret = OpalGetSupportedAttributesInfo (Session, &SupportedAttributes, &OpalBaseComId);
  if (Ret != TcgResultSuccess) {
    return FALSE;
  }

  Session->OpalBaseComId = OpalBaseComId;
  Ret = OpalGetLockingInfo(Session, &LockingFeature);
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
 IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Sscp
  )
{
  TCG_RESULT                    Result;
  BOOLEAN                       BlockSidSupport;
  UINT32                        PpStorageFlags;
  BOOLEAN                       BlockSIDEnabled;
  OPAL_PEI_DEVICE               *PeiDev;
  OPAL_SESSION                  Session;
  EFI_STATUS                    Status;

  ZeroMem(&Session, sizeof (Session));
  PeiDev = OPAL_PEI_DEVICE_FROM_THIS (Sscp);
  if (PeiDev->Signature != OPAL_PEI_DEVICE_SIGNATURE) {
    return;
  }

  Session.Sscp = Sscp;
  BlockSidSupport = FALSE;
  if (IsOpalDeviceLocked (&Session)) {
  DEBUG ((DEBUG_INFO, "Opal device is still locked.\n"));

    Result = OpalUtilUpdateGlobalLockingRange (
               &Session,
               PeiDev->Password,
               PeiDev->PasswordLength,
               FALSE,
               FALSE
               );
    Status = (Result == TcgResultSuccess) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
  DEBUG ((DEBUG_INFO, "Unlock device with Opalcomid: %x.\n", Session.OpalBaseComId));
  DEBUG ((DEBUG_INFO, "Unlock status: %r.\n", Status));

  }

  PpStorageFlags = Tcg2PhysicalPresenceLibGetManagementFlags ();
  if ((PpStorageFlags & TCG2_BIOS_STORAGE_MANAGEMENT_FLAG_ENABLE_BLOCK_SID) != 0) {
    BlockSIDEnabled = TRUE;
  } else {
    BlockSIDEnabled = FALSE;
  }
  if (BlockSIDEnabled && BlockSidSupport) {
    ZeroMem(&Session, sizeof (Session));
    Session.Sscp = &PeiDev->Sscp;
    Session.MediaId = 0;
    Session.OpalBaseComId = Session.OpalBaseComId;
    Result = OpalBlockSid (&Session, TRUE);
  }
}

/**
  extract Sata port number from device path.

  @param[in] DevicePath            device path.
  @param[in] DevicePathLength            device path length.
  @param[in out] HBAPortNumber           returned port number.

  @retval EFI_SUCCESS - Found SATA port
  @retval other - not a Sata device

**/

EFI_STATUS
GetSATAPortNumber (
  IN EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN UINTN                          DevicePathLength,
  IN OUT UINT16                         *HBAPortNumber
  )
{
  SATA_DEVICE_PATH                      *SataDevPath;
  EFI_DEVICE_PATH_PROTOCOL              *TmpDevicePath;
  UINTN                                 test;
  EFI_DEVICE_PATH_PROTOCOL              *Start;

  Start         = DevicePath;
  TmpDevicePath = DevicePath;
  test = (UINTN) TmpDevicePath - (UINTN) Start;

  while (!IsDevicePathEnd (TmpDevicePath)) {
    test = (UINTN) TmpDevicePath - (UINTN) Start;
    if (TmpDevicePath->Type == MESSAGING_DEVICE_PATH && TmpDevicePath->SubType == MSG_SATA_DP) {
      SataDevPath = (SATA_DEVICE_PATH *) TmpDevicePath;
      *HBAPortNumber = SataDevPath->HBAPortNumber;
      return EFI_SUCCESS;
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }
  return EFI_NOT_FOUND;
}

/**
  extract Nvmr NSID number from device path.

  @param[in] DevicePath            device path.
  @param[in] DevicePathLength            device path length.
  @param[in out] NamespaceId           returned port number.

  @retval EFI_SUCCESS - Found Nvme NSID
  @retval other - Not Nvme device

**/
EFI_STATUS
GetNvmeNameSpaceId (
  IN EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN UINTN                          DevicePathLength,
  IN OUT UINT32                         *NamespaceId
  )
{
  EFI_DEVICE_PATH_PROTOCOL       *TmpDevicePath;
  NVME_NAMESPACE_DEVICE_PATH     *NvmeDevicePath;
  UINTN                          test;
  EFI_DEVICE_PATH_PROTOCOL       *Start;

  Start         = DevicePath;
  TmpDevicePath = DevicePath;
  test = (UINTN) TmpDevicePath - (UINTN) Start;

  while (!IsDevicePathEnd (TmpDevicePath)) {
    test = (UINTN) TmpDevicePath - (UINTN) Start;
    if ((TmpDevicePath->Type == MESSAGING_DEVICE_PATH) &&
    (TmpDevicePath->SubType == MSG_NVME_NAMESPACE_DP) &&
    (DevicePathNodeLength(TmpDevicePath) == sizeof(NVME_NAMESPACE_DEVICE_PATH))) {
      NvmeDevicePath = (NVME_NAMESPACE_DEVICE_PATH*)TmpDevicePath;
      *NamespaceId = NvmeDevicePath->NamespaceId;
      return EFI_SUCCESS;
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }
  return EFI_NOT_FOUND;
}



/**
  Send unlock hdd password cmd through ATA PassThru PPI.

  @param[in] AtaPassThru           The pointer to the ATA PassThru PPI.
  @param[in] Port                  The port number of the ATA device.
  @param[in] PortMultiplierPort    The port multiplier port number of the ATA device.
  @param[in] Identifier            The identifier to set user or master password.
  @param[in] Password              The hdd password of attached ATA device.

  @retval EFI_SUCCESS              Successful to send unlock hdd password cmd.
  @retval EFI_INVALID_PARAMETER    The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES     Not enough memory to send unlock hdd password cmd.
  @retval EFI_DEVICE_ERROR         Can not send unlock hdd password cmd.

**/
EFI_STATUS
UnlockAtaDevice (
  IN EDKII_PEI_ATA_PASS_THRU_PPI    *AtaPassThru,
  IN UINT16                         Port,
  IN UINT16                         PortMultiplierPort,
  IN UINT16                         Identifier,
  IN UINT8                          *Password,
  IN UINT8                          PasswordType
  )
{
  EFI_STATUS                          Status;
  EFI_ATA_COMMAND_BLOCK               Acb;
  EFI_ATA_STATUS_BLOCK                *Asb;
  EFI_ATA_PASS_THRU_COMMAND_PACKET    Packet;
  STORAGE_SECURITY_COMMAND_PACKET     CommandPacket;

  if ((AtaPassThru == NULL) || (Password == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Prepare for ATA command block.
  //
  ZeroMem (&Acb, sizeof (Acb));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_SECURITY_UNLOCK_CMD;
  Acb.AtaDeviceHead = (UINT8) (PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4));

  //
  // Prepare for ATA pass through packet.
  //
  ZeroMem (&Packet, sizeof (Packet));
  Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT;
  Packet.Length   = EFI_ATA_PASS_THRU_LENGTH_BYTES;
  Packet.Asb      = Asb;
  Packet.Acb      = &Acb;

  Packet.OutDataBuffer = (VOID*)&CommandPacket;
  CommandPacket.SecurityCmdType = PasswordType & BIT0;
  CommandPacket.MasterPasswordIdentifier = Identifier;
  CopyMem (CommandPacket.HddPassword, Password, HDD_PASSWORD_MAX_NUMBER);
  Packet.OutTransferLength = sizeof (STORAGE_SECURITY_COMMAND_PACKET);
  Packet.Timeout           = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet
                          );
  if (!EFI_ERROR (Status) &&
      ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
      ((Asb->AtaError & ATA_ERRREG_ABRT) != 0)) {
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  return Status;
}


/**
  Send unlock hdd password cmd through ATA PassThru PPI.

  @param[in] AtaPassThru           The pointer to the ATA PassThru PPI.
  @param[in] Port                  The port number of the ATA device.
  @param[in] PortMultiplierPort    The port multiplier port number of the ATA device.
  @param[in] Identifier            The identifier to set user or master password.
  @param[in] Password              The hdd password of attached ATA device.

  @retval EFI_SUCCESS              Successful to send unlock hdd password cmd.
  @retval EFI_INVALID_PARAMETER    The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES     Not enough memory to send unlock hdd password cmd.
  @retval EFI_DEVICE_ERROR         Can not send unlock hdd password cmd.

**/
EFI_STATUS
UnlockHddPasswordWithAtaPassthru (
  IN EDKII_PEI_ATA_PASS_THRU_PPI    *AtaPassThruPpi
  )
{
  EFI_STATUS                             Status;
  EFI_STATUS                             GetMulPortStatus;
  UINT16                                 HBAPortNumber;
  UINT16                                 Port;
  UINT16                                 PortMultiplierPort;
  EFI_BOOT_MODE                          BootMode;
  EFI_BOOT_MODE                          OrgBootMode;

  UINT8                                 PasswordToHdd[HDD_PASSWORD_MAX_NUMBER + 1];
  HDD_PASSWORD_TABLE                    *HddPasswordTable;
  UINTN                                 HddPasswordTableIndex;
  UINTN                                 HddPasswordCount;
  HDD_PASSWORD_TABLE                    *HddPasswordTablePtr;
  EFI_DEVICE_PATH_PROTOCOL              *ControllerDevicePath;
  UINTN                                 DevicePathLength;
  TCG_OPAL_EXT_DATA                     *TcgExtDataPtr;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  UINT8                                 Count;


  HBAPortNumber = 0;

  //
  // Change boot mode to get correct variable
  //
  OrgBootMode = BOOT_WITH_FULL_CONFIGURATION;
  PeiServicesGetBootMode (&OrgBootMode);
  BootMode = BOOT_WITH_FULL_CONFIGURATION;
  PeiServicesSetBootMode(BootMode);

  HddPasswordTable = NULL;
  HddPasswordCount = 0;
  if (FeaturePcdGet (PcdH2OBaseCpHddPasswordGetTableSupported)) {
    Status = TriggerCpHddPasswordGetTable(&HddPasswordTable, &HddPasswordCount);
    if (HddPasswordTable == NULL) {
      Status = EFI_NOT_FOUND;
      goto EXIT;
    }
    if (EFI_ERROR (Status)) {
      if (Status == EFI_WARN_STALE_DATA) {
        if (FeaturePcdGet (PcdH2OBaseCpHddPasswordSetTableSupported) ) {
          TriggerCpHddPasswordSetTable(HddPasswordTable, HddPasswordCount);
        }
      }
    }
  }


  Status = AtaPassThruPpi->GetDevicePath (AtaPassThruPpi, &DevicePathLength, &ControllerDevicePath);
  if (EFI_ERROR(Status)) {
    Status = EFI_NOT_FOUND;
    goto EXIT;
  }
  Port = 0xFFFF;
  while (TRUE) {
    Status = AtaPassThruPpi->GetNextPort (AtaPassThruPpi, &Port);
    if (EFI_ERROR (Status)) {
      //
      // We cannot find more legal port then we are done.
      //
      break;
    }

    PortMultiplierPort = 0xFFFF;
    while (TRUE) {
      GetMulPortStatus = AtaPassThruPpi->GetNextDevice (AtaPassThruPpi, Port, &PortMultiplierPort);
      if (EFI_ERROR (GetMulPortStatus)) {
        //
        // We cannot find more legal port multiplier port number for ATA device
        // on the port, then we are done.
        //
        break;
      }

      ZeroMem (&PasswordToHdd, (HDD_PASSWORD_MAX_NUMBER + 1));
      HddPasswordTablePtr = (HDD_PASSWORD_TABLE *)HddPasswordTable;
      for (HddPasswordTableIndex = 0; HddPasswordTableIndex < HddPasswordCount;
           HddPasswordTableIndex++, HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr)) {
        TcgExtDataPtr = (TCG_OPAL_EXT_DATA *)(HddPasswordTablePtr + 1);
        DevicePath    = (EFI_DEVICE_PATH_PROTOCOL*)((UINTN)TcgExtDataPtr + GetExtDataSize ());
          //
          // This function would be used due to VMD would produce different devicepath in BDS and PEI,
          // thus need the node to node comparison.
          //
        if (!CompareHostDpNode(ControllerDevicePath, DevicePath)) {
          continue;
        }

        Status = GetSATAPortNumber(DevicePath, GetDevicePathSize(DevicePath), &HBAPortNumber);
        if (EFI_ERROR(Status)) {
          continue;
        }

        if (HBAPortNumber != Port ) {
          continue;
        }


        Status = HddPasswordStringProcess (
                   HddPasswordTablePtr->PasswordStr,
                   StrLen (HddPasswordTablePtr->PasswordStr),
                   (VOID **)&PasswordToHdd
                   );
        for (Count = 0; Count < ATA_COMMAND_RETRY_TIMES; Count++) {
          Status = UnlockAtaDevice(
                     AtaPassThruPpi,
                     Port,
                     PortMultiplierPort,
                     HddPasswordTablePtr->MasterPasswordIdentifier,
                     PasswordToHdd,
                     HddPasswordTablePtr->PasswordType
                     );
          if (!EFI_ERROR(Status)) {
            break;
          }
        }
      }
    }
    if (!EFI_ERROR (GetMulPortStatus) && PortMultiplierPort == 0xFFFF) {
      //
      // EDK2 has changed AtaPassThru behavior when getting the first PortMultiplierPort from
      // port that SATA is attached. If a device is directly attached on a port, previous call 
      // to this function will return the value 0xFFFF for PortMultiplierPort. In this case, 
      // there should be no more device on the port multiplier. 
      //
      // However, if the PortMultiplierPort is get through GetNextDevice() previously and 0xFFFF 
      // is returned but not iterated until EFI_NOT_FOUND, it will influence the next locate this 
      // protocol and EFI_SUCCESS may not return when getting the first PortMultiplierPort.
      //
      // Therefore, if AtaPassThru is produced by 3rd party, here will call GetNextDevice() again 
      // to reset the previous record and EFI_NOT_FOUND should return.
      //
      GetMulPortStatus = AtaPassThruPpi->GetNextDevice (AtaPassThruPpi, Port, &PortMultiplierPort);
    }
  }

EXIT:
  H2OFreePool ((VOID**)&HddPasswordTable);
  PeiServicesSetBootMode(OrgBootMode);
  return EFI_SUCCESS;
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

  return PeiDev->SscPpi->ReceiveData (
                           PeiDev->SscPpi,
                           PeiDev->DeviceIndex,
                           SSC_PPI_GENERIC_TIMEOUT,
                           SecurityProtocolId,
                           SecurityProtocolSpecificData,
                           PayloadBufferSize,
                           PayloadBuffer,
                           PayloadTransferSize
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

  return PeiDev->SscPpi->SendData (
                           PeiDev->SscPpi,
                           PeiDev->DeviceIndex,
                           SSC_PPI_GENERIC_TIMEOUT,
                           SecurityProtocolId,
                           SecurityProtocolSpecificData,
                           PayloadBufferSize,
                           PayloadBuffer
                           );
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
  IN     EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Sscp
  )
{
  OPAL_SESSION                       Session;
  TCG_SUPPORTED_SECURITY_PROTOCOLS   *SupportedProtocols;
  UINT8                              Buffer[BUFFER_SIZE];

  ZeroMem(&Session, sizeof (Session));
  Session.Sscp = Sscp;
  Session.MediaId = 0;
  ZeroMem(Buffer, BUFFER_SIZE);
  ASSERT(sizeof(Buffer) >= sizeof(TCG_SUPPORTED_SECURITY_PROTOCOLS));

  //
  // Retrieve supported protocols verify security protocol 1 is supported
  //
  SupportedProtocols = (TCG_SUPPORTED_SECURITY_PROTOCOLS*) Buffer;

  if (OpalRetrieveSupportedProtocolList (&Session,
                                        sizeof(TCG_SUPPORTED_SECURITY_PROTOCOLS),
                                        SupportedProtocols) == TcgResultFailure) {
    return FALSE;
  }

  if (TcgIsProtocolSupported (SupportedProtocols, TCG_SECURITY_PROTOCOL_ATA_DEVICE_SERVER_PASS)) {
    return TRUE;
  }

  return FALSE;
}


/**
  Unlock device with StorageSecurityCommand PEIM

  @param[in]  Sscp              Point to EFI_STORAGE_SECURITY_COMMAND_PROTOCOL.

  @retval EFI_SUCCESS           Unlock the locked device with TCG OPAL successfully.
  @retval EFI_NOT_FOUND         Can't find the required lockbox information.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.
  @retval Others                Something error occurred.
**/
EFI_STATUS
UnlockHddPasswordWithSscp (
  IN EDKII_PEI_STORAGE_SECURITY_CMD_PPI    *SscPpi
  )
{
  EFI_STATUS                            Status;
  EFI_DEVICE_PATH_PROTOCOL              *SscDevicePath;
  UINTN                                 SscDevicePathLength;
  UINTN                                 SscDeviceNum;
  UINTN                                 SscDeviceIndex;
  UINT64                                Timeout;
  ATA_DEVICE_SERVER_SEND_CMD_DATA       *AtaSecuritySendData;
  UINT16                                SecurityProtocolSpecificData;
  OPAL_SESSION                          Session;
  TCG_RESULT                            Result;
  OPAL_PEI_DEVICE                       *OpalDev;
  EFI_BOOT_MODE                         BootMode;
  EFI_BOOT_MODE                         OrgBootMode;
  HDD_PASSWORD_TABLE                    *HddPasswordTable;
  UINTN                                 HddPasswordTableIndex;
  UINTN                                 HddPasswordCount;
  HDD_PASSWORD_TABLE                    *HddPasswordTablePtr;
  UINT8                                 PasswordToHdd[HDD_PASSWORD_MAX_NUMBER + 1];
  TCG_OPAL_EXT_DATA                     *TcgExtDataPtr;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;

  Timeout             = 30000000;
  AtaSecuritySendData = NULL;
  Result              = 0;
  OpalDev             = NULL;
  HddPasswordTable    = NULL;

  OpalDev = (OPAL_PEI_DEVICE*)AllocateCopyPool (sizeof (OPAL_PEI_DEVICE), &mOpalDevTemplate);
  if (OpalDev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Change boot mode to get correct variable
  //
  OrgBootMode = BOOT_WITH_FULL_CONFIGURATION;
  PeiServicesGetBootMode (&OrgBootMode);
  BootMode = BOOT_WITH_FULL_CONFIGURATION;
  PeiServicesSetBootMode(BootMode);

  HddPasswordCount = 0;
  if (FeaturePcdGet (PcdH2OBaseCpHddPasswordGetTableSupported)) {
    Status = TriggerCpHddPasswordGetTable(&HddPasswordTable, &HddPasswordCount);
    if (HddPasswordTable == NULL) {
      Status = EFI_NOT_FOUND;
      goto EXIT;
    }
    if (EFI_ERROR (Status)) {
      if (Status == EFI_WARN_STALE_DATA) {
        if (FeaturePcdGet (PcdH2OBaseCpHddPasswordSetTableSupported) ) {
          TriggerCpHddPasswordSetTable(HddPasswordTable, HddPasswordCount);
        }
      } else {
        goto EXIT;
      }
    }
  }
  
  if (HddPasswordTable == NULL) {
    Status = EFI_NOT_FOUND;
    goto EXIT;
  }

  //
  // Go through all the devices managed by the SSC PPI instance.
  //
  Status = SscPpi->GetNumberofDevices (SscPpi, &SscDeviceNum);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  for (SscDeviceIndex = 1; SscDeviceIndex <= SscDeviceNum; SscDeviceIndex++) {
    Status = SscPpi->GetDevicePath (
                       SscPpi,
                       SscDeviceIndex,
                       &SscDevicePathLength,
                       &SscDevicePath
                       );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (SscDevicePathLength <= sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      //
      // Device path validity check.
      //
      continue;
    }
    //
    // Start to find password for each devices
    //
    ZeroMem (&PasswordToHdd, (HDD_PASSWORD_MAX_NUMBER + 1));
    HddPasswordTablePtr = (HDD_PASSWORD_TABLE *)HddPasswordTable;
    for (HddPasswordTableIndex = 0; HddPasswordTableIndex < HddPasswordCount;
         HddPasswordTableIndex++, HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr)) {
      TcgExtDataPtr = (TCG_OPAL_EXT_DATA *)(HddPasswordTablePtr + 1);
      DevicePath    = (EFI_DEVICE_PATH_PROTOCOL*)((UINTN)TcgExtDataPtr + GetExtDataSize ());
      if (CompareMem(DevicePath, SscDevicePath, SscDevicePathLength - END_DEVICE_PATH_LENGTH)) {
        continue;
      }

      Status = HddPasswordStringProcess (
                 HddPasswordTablePtr->PasswordStr,
                 StrLen (HddPasswordTablePtr->PasswordStr),
                 (VOID **)&PasswordToHdd
                 );

      if (!EFI_ERROR(Status)) {
        break;
      }
    }

    //
    // Find the matching device.
    //
    OpalDev->SscPpi           = SscPpi;
    OpalDev->DeviceIndex      = SscDeviceIndex;
    OpalDev->Sscp.ReceiveData = SecurityReceiveData;
    OpalDev->Sscp.SendData    = SecuritySendData;

    ZeroMem (&Session, sizeof (OPAL_SESSION));
    Session.MediaId = 0;
    Session.Sscp = &OpalDev->Sscp;
    Session.OpalBaseComId = OpalDev->OpalBaseComId;

    if (PcdGetBool (PcdH2OHddPasswordTcgOpalSupported) && IsOpalDeviceLocked (&Session)) {
      CopyMem (&OpalDev->Password, PasswordToHdd, StrLen (HddPasswordTablePtr->PasswordStr));
      OpalDev->PasswordLength = (UINT8)StrLen (HddPasswordTablePtr->PasswordStr);
      UnlockOpalPassword (&OpalDev->Sscp);
      continue;
    }

    AtaSecuritySendData = AllocateZeroPool(sizeof (ATA_DEVICE_SERVER_SEND_CMD_DATA));
    if (AtaSecuritySendData == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto EXIT;
    }

    CopyMem (&AtaSecuritySendData->Password, PasswordToHdd, StrLen (HddPasswordTablePtr->PasswordStr));
    AtaSecuritySendData->Byte1 = HddPasswordTablePtr->PasswordType;
    SecurityProtocolSpecificData = ATA_SECURITY_UNLOCK;
    Status = SscPpi->SendData (
               SscPpi,
               SscDeviceIndex,
               Timeout,
               TCG_SECURITY_PROTOCOL_ATA_DEVICE_SERVER_PASS,
               SwapBytes16(SecurityProtocolSpecificData),
               sizeof(ATA_DEVICE_SERVER_SEND_CMD_DATA),
               AtaSecuritySendData
               );
  }

EXIT:
  H2OFreePool((VOID**)&AtaSecuritySendData);
  H2OFreePool((VOID**)&OpalDev);
  H2OFreePool((VOID**)&HddPasswordTable);
  PeiServicesSetBootMode(OrgBootMode);
  
  return Status;
}



/**
  Entry point of the notification callback function itself within the PEIM.
  It is to unlock HDD password for S3.

  @param  PeiServices      Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor Address of the notification descriptor data structure.
  @param  Ppi              Address of the PPI that was installed.

  @return Status of the notification.
          The status code returned from this function is ignored.
**/
EFI_STATUS
EFIAPI
HddPasswordSscpNotify (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "%a: StorageSecurityComPpi:%x notified.\n", Ppi));
  UnlockHddPasswordWithSscp ((EDKII_PEI_STORAGE_SECURITY_CMD_PPI *) Ppi);
  return EFI_SUCCESS;
}



/**
  Entry point of the notification callback function itself within the PEIM.
  It is to unlock HDD password for S3.

  @param  PeiServices      Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor Address of the notification descriptor data structure.
  @param  Ppi              Address of the PPI that was installed.

  @return Status of the notification.
          The status code returned from this function is ignored.
**/
EFI_STATUS
EFIAPI
HddPasswordAtaPassThruNotify (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "%a: AtaPassthru:%x notified.\n", Ppi));
  UnlockHddPasswordWithAtaPassthru ((EDKII_PEI_ATA_PASS_THRU_PPI *) Ppi);
  return EFI_SUCCESS;
}


EFI_PEI_NOTIFY_DESCRIPTOR mHddPasswordAtaPassThruPpiNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiAtaPassThruPpiGuid,
  HddPasswordAtaPassThruNotify
};


EFI_PEI_NOTIFY_DESCRIPTOR mHddPasswordSscpPpiNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiStorageSecurityCommandPpiGuid,
  HddPasswordSscpNotify
};



/**
  Main entry for this module.

  @param FileHandle             Handle of the file being invoked.
  @param PeiServices            Pointer to PEI Services table.

  @return Status from PeiServicesNotifyPpi.

**/
EFI_STATUS
EFIAPI
HddPasswordPeiInit (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                              Status;
  EFI_BOOT_MODE                           BootMode;


  Status = PeiServicesRegisterForShadow (FileHandle);
  if (!EFI_ERROR (Status)) {
    return Status;
  }


  Status = PeiServicesGetBootMode (&BootMode);
  if ((EFI_ERROR (Status)) || (BootMode != BOOT_ON_S3_RESUME) && (BootMode != BOOT_IN_RECOVERY_MODE)) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a: Enters Recovery PEI path.\n", __FUNCTION__));

  Status = PeiServicesNotifyPpi (&mHddPasswordSscpPpiNotifyDesc);
  Status = PeiServicesNotifyPpi (&mHddPasswordAtaPassThruPpiNotifyDesc);

  if (FeaturePcdGet (PcdH2OBaseCpHddPasswordSetTableSupported) ) {
     H2O_CP_HANDLE       H2OBaseCpHddPasswordSetTableHandle;
     //
     // Register for callback on Storage Security Protocol publication
     //
     Status = H2OCpRegisterHandler (
                &gH2OBaseCpHddPasswordSetTableGuid,
                H2OBaseCpHddPasswordSetTableCallback,
                H2O_CP_MEDIUM,
                &H2OBaseCpHddPasswordSetTableHandle
                );
     if (EFI_ERROR (Status)) {
       DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBaseCpHddPasswordSetTableGuid, Status));
       return Status;
     }
     DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBaseCpHddPasswordSetTableGuid, Status));
    }


   if (FeaturePcdGet (PcdH2OBaseCpHddPasswordGetTableSupported)) {
     H2O_CP_HANDLE        H2OBaseCpHddPasswordGetTableHandle;
     //
     // Register for callback on Storage Security Protocol publication
     //
     Status = H2OCpRegisterHandler (
                &gH2OBaseCpHddPasswordGetTableGuid,
                H2OBaseCpHddPasswordGetTableCallback,
                H2O_CP_MEDIUM,
                &H2OBaseCpHddPasswordGetTableHandle
                );
     if (EFI_ERROR (Status)) {
       DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBaseCpHddPasswordGetTableGuid, Status));
       return Status;
     }
     DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBaseCpHddPasswordGetTableGuid, Status));
    }

  ASSERT_EFI_ERROR (Status);
  return Status;
}
