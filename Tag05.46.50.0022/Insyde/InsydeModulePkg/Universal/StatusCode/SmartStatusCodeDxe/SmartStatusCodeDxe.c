/** @file
  BootService DXE driver for SMART Report Status Code

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

#include <Protocol/AtaPassThru.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/DevicePath.h>
#include <Protocol/BlockIo.h>
#include <Protocol/PciIo.h>

#include <Guid/EventGroup.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/H2OCpLib.h>
#include <Library/H2OLib.h>

#include <IndustryStandard/Atapi.h>
#include <IndustryStandard/Nvme.h>

STATIC NVME_SMART_HEALTH_INFO_LOG     mNvmeHealthBuffer;
STATIC EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET mNvmeCommandPacket;
STATIC EFI_NVM_EXPRESS_COMMAND                  mNvmeCommand;
STATIC EFI_NVM_EXPRESS_COMPLETION               mNvmeCompletion;
STATIC NVME_ADMIN_CONTROLLER_DATA               mNvmeControllerDataBuffer;
STATIC EFI_ATA_COMMAND_BLOCK                    mAcb;
STATIC EFI_ATA_STATUS_BLOCK                     mAsb;
STATIC ATA_IDENTIFY_DATA                        mIdentifyData;
STATIC EFI_ATA_PASS_THRU_COMMAND_PACKET         mAtaPassThruCmdPacket;


#define SMART_STATUS_CHECKPOINT_NOTIFY_TPL    H2O_CP_MEDIUM_LOW
#define ATA_IDENTIFY_CMD            0xEC
#define NVME_ADMIN_QUEUE            0x00
#define NVME_GENERIC_TIMEOUT           EFI_TIMER_PERIOD_SECONDS (5)
#define NVME_CONTROLLER_ID             0
#define NVME_ALL_NAMESPACES            0xFFFFFFFF




/**
  Find NVMe namespaceid from devicepath

  @param[in]   DevicePath             NVMe devicepath
  @param[out]   Nsid                  NamespaceId

  @retval      EFI_SUCCESS            Find the namespaceid
  @retval      Others                 Failured

**/

BOOLEAN
FindNvmeNsid (
  IN EFI_DEVICE_PATH_PROTOCOL    *DevicePath,    
  OUT UINT32                     *Nsid 
  )
{
  NVME_NAMESPACE_DEVICE_PATH           *NVMeDevPath;

  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH && DevicePath->SubType == MSG_NVME_NAMESPACE_DP) {
      NVMeDevPath = (NVME_NAMESPACE_DEVICE_PATH *) DevicePath;
      *Nsid = (UINT8)NVMeDevPath->NamespaceId;
      return TRUE;
    } 
    DevicePath = NextDevicePathNode (DevicePath);
  }
  
  return FALSE;
}



/**
  Find SATA port from device path

  @param[in] DevicePath          SATA device path
  @param[out] Port               SATA port number
  @param[out] PortMultiplier     SATA portmultiplier number

  @retval EFI_SUCCESS   find the SATA device path
  @retval EFI_NOT_FOUND Cannot find SATA device path

**/

BOOLEAN
FindSataDevicePort (
  IN   EFI_DEVICE_PATH_PROTOCOL              *DevicePath,
  OUT  UINT16 *Port, 
  OUT  UINT16 *PortMultiplier
  )
{
  SATA_DEVICE_PATH                     *SataDevPath;

  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH && DevicePath->SubType == MSG_SATA_DP) {
      SataDevPath = (SATA_DEVICE_PATH *) DevicePath;
      *Port = SataDevPath->HBAPortNumber;
      *PortMultiplier = SataDevPath->PortMultiplierPortNumber;
      return TRUE;
    } 
    DevicePath = NextDevicePathNode (DevicePath);
  }
  
  return FALSE;
}

/**
  Check if the device is a RAID volume

  @param[in] DevicePath          SATA device path

  @retval TRUE  It is a RAID volume
  @retval FALSE Not a RAID volume

**/

BOOLEAN
IsRaidVolume (
  IN   EFI_DEVICE_PATH_PROTOCOL              *DevicePath
  )
{
  UINT32                                NsidMap;
  UINT16                                PortMultiplier;
  UINT16                                PortMap;
  UINT16                                Count;

  if (FindSataDevicePort (DevicePath, &PortMap, &PortMultiplier)) {
    for (Count = 0;PortMap != 0; PortMap >>= 1) {
      if (PortMap & BIT0) {
        Count++;
      }
    }
    if (Count > 1) {
      return TRUE;
    }
  }


  if (FindNvmeNsid (DevicePath, &NsidMap)) {
    for (Count = 0; NsidMap != 0; NsidMap >>= 1) {
      if (NsidMap & BIT0) {
        Count++;
      }
    }
    if (Count > 1) {
      return TRUE;
    }
  }
  
return FALSE;
}

/**
  Check if the controller is a Intel Raid controller

  @param[in] DevicePath          SATA device path

  @retval TRUE  It is a RAID volume
  @retval FALSE Not a RAID volume

**/


BOOLEAN
IsIntelRaidController (
  IN   EFI_DEVICE_PATH_PROTOCOL              *ParentDevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL              *TempDevicePath;
  EFI_STATUS                            AtaPassthruStatus;
  EFI_STATUS                            NvmePassthruStatus;
  EFI_STATUS                            PciIoStatus;
  EFI_HANDLE                            ControllerHandle;

  TempDevicePath = ParentDevicePath;
  AtaPassthruStatus = gBS->LocateDevicePath(&gEfiAtaPassThruProtocolGuid, &TempDevicePath, &ControllerHandle);
  TempDevicePath = ParentDevicePath;
  NvmePassthruStatus = gBS->LocateDevicePath(&gEfiNvmExpressPassThruProtocolGuid, &TempDevicePath, &ControllerHandle);
  TempDevicePath = ParentDevicePath;
  PciIoStatus = gBS->LocateDevicePath(&gEfiPciIoProtocolGuid, &TempDevicePath, &ControllerHandle);
  if (!EFI_ERROR(PciIoStatus) && !EFI_ERROR(NvmePassthruStatus) && !EFI_ERROR(AtaPassthruStatus)) {
    return TRUE;
  }
  return FALSE;  
}

/**
  compare device path between controller and device

  @param[in] ParentDevicePath          parent controller's device path
  @param[in] DevicePath          Raid device path
  @param[in] SourceNsid          Nsid extract from passthru for compare
  @param[in] SourcePort          SATA port extract from passthru for compare

  @retval TRUE  It is a device belong to the controller
  @retval FALSE it belongs to different controller

**/


BOOLEAN
CompareDp (
  IN   EFI_DEVICE_PATH_PROTOCOL              *ParentDevicePath,
  IN   EFI_DEVICE_PATH_PROTOCOL              *DevicePath,
  IN   UINT32                                SourceNsid,
  IN   UINT16                                SourcePort
  )
{
  UINT32                                Nsid;
  UINT32                                NsidMap;
  UINT16                                Port;
  UINT16                                PortMultiplier;
  UINT16                                PortMap;

  //
  // Check if it is the right controller first
  //
  if (CompareMem(DevicePath, ParentDevicePath, GetDevicePathSize(ParentDevicePath) - END_DEVICE_PATH_LENGTH)){
    return FALSE;
  }


  //
  // Check if it is the right device
  //
  if (FindSataDevicePort (DevicePath, &Port, &PortMultiplier)) {
    if (!IsIntelRaidController (ParentDevicePath)) {
      if (SourcePort == Port) {
        return TRUE;
      }
    }
    //
    // treat port that extract from devicepath as bit map
    //
    PortMap = Port;
    for (Port = 0; PortMap != 0; Port++, PortMap >>= 1);
    Port--;
    if (SourcePort == Port) {
      return TRUE;
    }
  }
  
  if (FindNvmeNsid (DevicePath, &Nsid)) {
    if (!IsIntelRaidController (ParentDevicePath)) {
      if (SourceNsid == Nsid) {
        return TRUE;
      }
    }

    //
    // treat Nsid that extract from devicepath as bit map
    //
    NsidMap= Nsid;
    for (Nsid = 0; NsidMap != 0; Nsid++, NsidMap >>= 1);

    if (SourceNsid == Nsid) {
      return TRUE;
    }
  }
 return FALSE;
}

/**
  Get NVMe SMART log page attribute from controller identify structure

  @param[in]   NvmeDevice             Nvmexpresspassthru
  @param[in]   Nsid                   NnamespaceId
  @param[out]  Lpa                    Log page attribute
  @param[out]  IsNsidValid            Check if Nsid parameter is usde as Nsid  

  @retval      EFI_SUCCESS            Get the attribute successfully.
  @retval      Others                 Failure to get the attribute.

**/
EFI_STATUS
EFIAPI
NvmeGetLogAttribute (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *NvmeDevice,
  IN UINT32                               Nsid,
  OUT UINT8                               *Lpa,
  OUT BOOLEAN                             *IsNsidFromDevicePathValid
  )
{
  EFI_STATUS                               Status;

  
  ZeroMem (&mNvmeCommandPacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&mNvmeCommand, sizeof(EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&mNvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION));
  ZeroMem (&mNvmeControllerDataBuffer, sizeof(NVME_ADMIN_CONTROLLER_DATA));

  mNvmeCommand.Cdw0.Opcode = NVME_ADMIN_IDENTIFY_CMD;
  //
  // According to Nvm Express 1.1 spec Figure 38, When not used, the field shall be cleared to 0h.
  // For the Identify command, the Namespace Identifier is only used for the Namespace data structure.
  //
  mNvmeCommand.Nsid        = 0;
  mNvmeCommandPacket.NvmeCmd        = &mNvmeCommand;
  mNvmeCommandPacket.NvmeCompletion = &mNvmeCompletion;
  mNvmeCommandPacket.TransferBuffer = (VOID*)&mNvmeControllerDataBuffer;
  mNvmeCommandPacket.TransferLength = sizeof (NVME_ADMIN_CONTROLLER_DATA);
  mNvmeCommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  mNvmeCommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set bit 0 (Cns bit) to 1 to identify a controller
  //
  mNvmeCommand.Cdw10                = 1;
  mNvmeCommand.Flags                = CDW10_VALID;
  *IsNsidFromDevicePathValid                      = TRUE;
  
  Status = NvmeDevice->PassThru (
                               NvmeDevice,
                               0,
                               &mNvmeCommandPacket,
                               NULL
                               );
  if (EFI_ERROR(Status)) {
    //
    // namespace used as controller index here
    //
    Status = NvmeDevice->PassThru (
                                 NvmeDevice,
                                 Nsid,
                                 &mNvmeCommandPacket,
                                 NULL
                                 );
    if (EFI_ERROR(Status)) {
      return EFI_UNSUPPORTED;
    }
    *IsNsidFromDevicePathValid = FALSE;
  }
  
  if (!EFI_ERROR(Status)) {
    *Lpa = mNvmeControllerDataBuffer.Lpa;
    return Status;
  }
  return Status;
}


/**
  Send SMART Return Status command to check if the execution of SMART cmd is successful or not.

  @param  PciIo               The PCI IO protocol instance.
  @param  AhciRegisters       The pointer to the EFI_AHCI_REGISTERS.
  @param  Port                The number of port.
  @param  PortMultiplier      The port multiplier port number.
  @param  AtaStatusBlock      A pointer to EFI_ATA_STATUS_BLOCK data structure.

  @retval EFI_SUCCESS          Successfully get the return status of S.M.A.R.T command execution.
                               and underthreshold
  @retval EFI_UNSUPPORTED      Can't determine the error, report SMART not support
  @retval EFI_DEVICE_ERROR     Overthreshold warning happened

**/
EFI_STATUS
EFIAPI
NvmeSmartReturnStatusCheck (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *NvmeDevice,
  IN UINT32                               Nsid,
  IN EFI_DEVICE_PATH_PROTOCOL             *ParentDevicePath
  )
{
  EFI_STATUS                               Status;
  UINT8                                    Lpa;
  NVME_SMART_HEALTH_INFO_LOG               *tempLog;
  EFI_DEVICE_PATH_PROTOCOL                 *DevicePath;
  EFI_HANDLE                                *HandleBuffer;
  UINTN                                     HandleCount;
  UINTN                                     HandleIndex;
  BOOLEAN                                   IsNsidFromDevicePathValid;
  

  Lpa = 0;
  DevicePath        = NULL;

  Status = NvmeGetLogAttribute(NvmeDevice, Nsid, &Lpa, &IsNsidFromDevicePathValid);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_FOUND;
  }
  
  ZeroMem (&mNvmeCommandPacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&mNvmeCommand, sizeof(EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&mNvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION));
  ZeroMem (&mNvmeHealthBuffer, sizeof (NVME_SMART_HEALTH_INFO_LOG));

  mNvmeCommand.Cdw0.Opcode = NVME_ADMIN_GET_LOG_PAGE_CMD;
  //
  // According to Nvm Express 1.1 spec Figure 38, When not used, the field shall be cleared to 0h.
  // For the Identify command, the Namespace Identifier is only used for the Namespace data structure.
  //
  mNvmeCommand.Nsid         =  (Lpa & BIT0) ? Nsid : 0xffffffff;
  mNvmeCommandPacket.NvmeCmd        = &mNvmeCommand;
  mNvmeCommandPacket.NvmeCompletion = &mNvmeCompletion;
  mNvmeCommandPacket.TransferBuffer = (VOID*)&mNvmeHealthBuffer;
  mNvmeCommandPacket.TransferLength = sizeof (NVME_SMART_HEALTH_INFO_LOG);
  mNvmeCommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  mNvmeCommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set log identifier to 2 for SMART/Health information
  //
  mNvmeCommand.Cdw10                = 0x02 | (((sizeof (NVME_SMART_HEALTH_INFO_LOG) / 4) - 1) << 16);
  mNvmeCommand.Flags                = CDW10_VALID;
  if (IsNsidFromDevicePathValid) {
    Status = NvmeDevice->PassThru (
                                 NvmeDevice,
                                 mNvmeCommand.Nsid,
                                 &mNvmeCommandPacket,
                                 NULL
                                 );
    if (EFI_ERROR(Status)) {
      return EFI_UNSUPPORTED;
    }
  } else {
    //
    // namespace used as controller index here
    //
    Status = NvmeDevice->PassThru (
                                 NvmeDevice,
                                 Nsid,
                                 &mNvmeCommandPacket,
                                 NULL
                                 );
  }

  if (EFI_ERROR(Status)) {
    return EFI_UNSUPPORTED;
  }
  
  DEBUG ((DEBUG_INFO, "NVME SMART command with mNVMeCommand.Nsid:%r\n", Status));  
  

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    return EFI_UNSUPPORTED;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&DevicePath
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }

    if (IsIntelRaidController(ParentDevicePath) && IsRaidVolume (DevicePath)) {
      continue;
    }
    
    if (CompareDp (ParentDevicePath, DevicePath, Nsid, 0xffff)) {
      break;
    }
  }

  if (HandleIndex == HandleCount){
    return EFI_NOT_FOUND;
  }
  
  if (EFI_ERROR(Status)) {
    REPORT_STATUS_CODE_WITH_DEVICE_PATH (
      EFI_PROGRESS_CODE,
      (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_NOTSUPPORTED),
      DevicePath
      );
    return EFI_UNSUPPORTED;
  }
  
  tempLog = &mNvmeHealthBuffer;
  if (
      tempLog->CriticalWarningAvailableSpare ||
      tempLog->CriticalWarningTemperature ||
      tempLog->CriticalWarningReliability ||
      tempLog->CriticalWarningMediaReadOnly ||
      tempLog->CriticalWarningVolatileBackup
      ) {
   REPORT_STATUS_CODE_WITH_DEVICE_PATH (
     EFI_PROGRESS_CODE,
     (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_OVERTHRESHOLD),
     DevicePath
     );      
   return EFI_DEVICE_ERROR;
  }

  REPORT_STATUS_CODE_WITH_DEVICE_PATH (
    EFI_PROGRESS_CODE,
    (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_UNDERTHRESHOLD),
    DevicePath
    );
  return EFI_SUCCESS;
}



/**
  Send SMART Return Status command to check if the execution of SMART cmd is successful or not.

  @param  AtaPassThru       Atapassthru instance
  @param  Port              The number of port.
  @param  PortMultiplier    The port multiplier port number.
  @param  DevicePath        devicepath of the controller 

  @retval EFI_SUCCESS          Successfully get the return status of S.M.A.R.T command execution.
                               and underthreshold
  @retval EFI_UNSUPPORTED      Can't determine the error, report SMART not support
  @retval EFI_DEVICE_ERROR     Overthreshold warning happened
  @retval EFI_INVALID_PARAMETER Support SMART but disabled

**/
EFI_STATUS
EFIAPI
AtaSmartReturnStatusCheck (
  IN EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN UINT16                       Port,
  IN UINT16                       PortMultiplier,
  IN EFI_DEVICE_PATH_PROTOCOL     *ParentDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  ATA_IDENTIFY_DATA                        *IdentifyData;
  EFI_ATA_PASS_THRU_COMMAND_PACKET         *AtaPassThruCmdPacket;
  EFI_HANDLE                                *HandleBuffer;
  UINTN                                     HandleCount;
  UINTN                                     HandleIndex;

  ZeroMem (&mAcb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (&mAtaPassThruCmdPacket, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&mAsb, sizeof (EFI_ATA_STATUS_BLOCK));  

  mAtaPassThruCmdPacket.Acb                  = &mAcb;
  mAtaPassThruCmdPacket.Asb                  = &mAsb;
  IdentifyData = &mIdentifyData;
  AtaPassThruCmdPacket = &mAtaPassThruCmdPacket;
  mAtaPassThruCmdPacket.Acb->AtaCommand      = ATA_IDENTIFY_CMD;
  mAtaPassThruCmdPacket.InDataBuffer         = (VOID *)&mIdentifyData;
  mAtaPassThruCmdPacket.InTransferLength     = sizeof(ATA_IDENTIFY_DATA);
  mAtaPassThruCmdPacket.Protocol             = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
  mAtaPassThruCmdPacket.Length               = EFI_ATA_PASS_THRU_LENGTH_BYTES;
  mAtaPassThruCmdPacket.Timeout              = EFI_TIMER_PERIOD_SECONDS (3);
  Status = AtaPassThru->PassThru (
                                   AtaPassThru,
                                   Port,
                                   PortMultiplier,
                                   &mAtaPassThruCmdPacket,
                                   NULL
                                   );
  if (EFI_ERROR(Status)) {
    return RETURN_ABORTED;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Find devicepath
  // 
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&DevicePath
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }
    //
    // VMD RAID volume is not supported
    //
    if (IsIntelRaidController(ParentDevicePath) && IsRaidVolume (DevicePath)) {
      continue;
    }
    
    if (CompareDp (ParentDevicePath, DevicePath, 0xffffffff, Port)) {
      break;
    }
  }  

  if (HandleIndex == HandleCount){
    return EFI_NOT_FOUND;
  }
  
  if (!(mIdentifyData.command_set_supported_82 & BIT0)) {
    REPORT_STATUS_CODE_WITH_DEVICE_PATH (
      EFI_PROGRESS_CODE,
      (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_NOTSUPPORTED),
      DevicePath
      );
    if (!(mIdentifyData.command_set_feature_enb_85 & BIT0)) {
      REPORT_STATUS_CODE_WITH_DEVICE_PATH (
        EFI_PROGRESS_CODE,
        (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_DISABLE),
        DevicePath
        );
      return EFI_INVALID_PARAMETER;
    }
    return EFI_UNSUPPORTED;
  }

  mAcb.AtaCommand      = ATA_CMD_SMART;
  mAcb.AtaFeatures     = ATA_SMART_RETURN_STATUS;
  mAcb.AtaCylinderLow  = ATA_CONSTANT_4F;
  mAcb.AtaCylinderHigh = ATA_CONSTANT_C2;

  mAtaPassThruCmdPacket.Protocol            = EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA;
  mAtaPassThruCmdPacket.Timeout             = EFI_TIMER_PERIOD_SECONDS (3);
  mAtaPassThruCmdPacket.Acb = &mAcb;
  mAtaPassThruCmdPacket.Asb = &mAsb;

  Status = AtaPassThru->PassThru (
                                   AtaPassThru,
                                   Port,
                                   PortMultiplier,
                                   &mAtaPassThruCmdPacket,
                                   NULL
                                   );
   
  if (!EFI_ERROR (Status) && 
      (mAsb.AtaCylinderLow == 0xf4) && (mAsb.AtaCylinderHigh == 0x2c)) {
    REPORT_STATUS_CODE_WITH_DEVICE_PATH (
      EFI_PROGRESS_CODE,
      (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_OVERTHRESHOLD),
      DevicePath
      );
    return EFI_DEVICE_ERROR;
  }

  REPORT_STATUS_CODE_WITH_DEVICE_PATH (
    EFI_PROGRESS_CODE,
    (EFI_IO_BUS_ATA_ATAPI | EFI_IOB_ATA_BUS_SMART_UNDERTHRESHOLD),
    DevicePath
    );
  return EFI_SUCCESS;
}


/**
  Check ATA SMART error and report status

**/

VOID
CheckAtaSmartErrorAndReportStatus (
  VOID
  )
{
  EFI_STATUS                           Status;
  EFI_STATUS                           GetMulPortStatus;
  UINTN                                HandleNum;
  EFI_HANDLE                           *AtaPassThruHandles;
  UINT8                                Index;
  EFI_DEVICE_PATH_PROTOCOL             *ParentDevicePath;  
  EFI_ATA_PASS_THRU_PROTOCOL          *AtaDevice;
  UINT16                               Port;
  UINT16                               PortMultiplierPort;

  
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiAtaPassThruProtocolGuid,
                  NULL,
                  &HandleNum,
                  &AtaPassThruHandles
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (
                    AtaPassThruHandles[Index],
                    &gEfiAtaPassThruProtocolGuid,
                    (VOID **) &AtaDevice
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "gBS->HandleProtocol : Status=%r\n", Status));
      continue;
    }
    
    Status = gBS->HandleProtocol (
                    AtaPassThruHandles[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &ParentDevicePath
                    );

    //
    //Go through all of the ports and portmultiplierports and try SE if a device is present
    //
    Port = 0xFFFF;
    while (TRUE) {
      Status = AtaDevice->GetNextPort (AtaDevice, &Port);
      if (EFI_ERROR (Status)) {
        break;
      }

      PortMultiplierPort = 0xFFFF;
      while (TRUE) {
        GetMulPortStatus = AtaDevice->GetNextDevice (AtaDevice, Port, &PortMultiplierPort);
        if (EFI_ERROR (GetMulPortStatus)) {
          break;
        }
        Status = AtaSmartReturnStatusCheck(AtaDevice, Port, PortMultiplierPort, ParentDevicePath);
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
        GetMulPortStatus = AtaDevice->GetNextDevice (AtaDevice, Port, &PortMultiplierPort);
      }
    }
  }

  H2OFreePool((VOID**)&AtaPassThruHandles);
}

/**
  Check ATA SMART error and report status

**/

VOID
CheckNvmeSmartErrorAndReportStatus (
  VOID
  )
{
  EFI_STATUS                           Status;
  UINTN                                HandleNum;
  EFI_HANDLE                           *NvmePassThruHandles;
  UINTN                                Index;
  EFI_DEVICE_PATH_PROTOCOL             *ParentDevicePath;  
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *NvmeDevice;
  UINT32                              NamespaceId;
  
  
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  NULL,
                  &HandleNum,
                  &NvmePassThruHandles
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (
                    NvmePassThruHandles[Index],
                    &gEfiNvmExpressPassThruProtocolGuid,
                    (VOID **) &NvmeDevice
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "gBS->HandleProtocol: Status=%r\n", Status));
      continue;
    }
    Status = gBS->HandleProtocol (
                    NvmePassThruHandles[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &ParentDevicePath
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }
    NamespaceId = NVME_ALL_NAMESPACES;
    while (TRUE) {
      Status = NvmeDevice->GetNextNamespace (NvmeDevice, &NamespaceId);
      if (EFI_ERROR (Status)) {
        break;
      }
      Status = NvmeSmartReturnStatusCheck(NvmeDevice, NamespaceId, ParentDevicePath);
    }

  }

  H2OFreePool((VOID**)&NvmePassThruHandles);
}


/**

  Check SMART status and report status for NVMe and SATA
  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBdsCpBootDeviceSelectCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  H2OCpUnregisterHandler (Handle);
  CheckAtaSmartErrorAndReportStatus ();
  CheckNvmeSmartErrorAndReportStatus ();
}


/**
  Entry point of Smart Status Code Driver.

  This function is the entry point of this DXE Status Code Driver.
  It initializes registers status code handlers, and registers event for EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
SmartStatusCodeDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                          Status;

  if (FeaturePcdGet (PcdH2OBdsCpBootDeviceSelectSupported)) {
    H2O_CP_HANDLE                         H2OBdsCpBootDeviceSelectHandle;
    //
    // Register for callback on Storage Security Protocol publication
    //
    H2OBdsCpBootDeviceSelectHandle = NULL;
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpBootDeviceSelectGuid,
               H2OBdsCpBootDeviceSelectCallback,
               SMART_STATUS_CHECKPOINT_NOTIFY_TPL,
               &H2OBdsCpBootDeviceSelectHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpBootDeviceSelectGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpBootDeviceSelectGuid, Status));
  }    

  return EFI_SUCCESS;
}
