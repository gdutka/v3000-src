/** @file
  Implementation of ATA Commands

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

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>

#include <Protocol/AtaPassThru.h>
#include <Protocol/ScsiPassThruExt.h>

#include <IndustryStandard/Atapi.h>

#include "SataDriveInfoDxe.h"
#include "AtaCommands.h"

/**
  Allocates an aligned buffer for ATA device.

  This function allocates an aligned buffer for the ATA device to perform
  ATA pass through operations. The alignment requirement is from ATA or
  Extended SCSI pass through interface.

  @param  IoAlign           The alignment requirement.
  @param  BufferSize        The request buffer size.

  @return A pointer to the aligned buffer or NULL if the allocation fails.

**/
STATIC
VOID *
AllocateAlignedBuffer (
  IN UINT32   IoAlign,
  IN UINTN    BufferSize
  )
{
  return AllocateAlignedPages (EFI_SIZE_TO_PAGES (BufferSize), IoAlign);
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
  Helper function to dump given data.

  @param[in]  Data        The pointer to the data.
  @param[in]  DataSize    The size (length in bytes) of the data.

**/
VOID
DumpData (
  IN  UINT8   *Data,
  IN  UINTN   DataSize
  )
{
  UINTN Index;

  if (Data == NULL || DataSize == 0) {
    return;
  }

  for (Index = 0; Index < DataSize; Index++) {
    if (Index != 0 && Index % 16 == 0) {
      DEBUG ((EFI_D_VERBOSE, "\n"));
    }
    DEBUG ((EFI_D_VERBOSE, "%02x ", *(Data + Index)));
  }

  DEBUG ((EFI_D_VERBOSE, "\n"));
}

/**
  Get Identify data from the drive via ATA Pass-Through Protocol

  @param[in]      AtaPassThru         The pointer to the ATA Pass-Through protocol.
  @param[in]      Port                The port number of the target drive.
  @param[in]      PortMultiplierPort  The port multiplier port number of the target drive.
  @param[in,out]  IdentifyData        A pointer to the ATA_IDENTIFY_DATA. It is caller's responsibility
                                      to free this buffer. Please note that the buffer is actually an
                                      aligned page.

  @retval EFI_SUCCESS             Successfully get Identify data from the drive.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval EFI_UNSUPPORTED         The drive does not support the command or the data is all zero.
  @retval EFI_CRC_ERROR           Bad checksum.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
AtaIdentifyData (
  IN    EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN    UINT16                      Port,
  IN    UINT16                      PortMultiplierPort,
  OUT   ATA_IDENTIFY_DATA           **IdentifyData
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;

  if (AtaPassThru == NULL || IdentifyData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  (*IdentifyData) = AllocateAlignedBuffer (AtaPassThru->Mode->IoAlign, sizeof (ATA_IDENTIFY_DATA));
  if ((*IdentifyData) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Asb = AllocateAlignedBuffer (AtaPassThru->Mode->IoAlign, sizeof (EFI_ATA_STATUS_BLOCK));
  if (Asb == NULL) {
    FreeAlignedBuffer ((VOID *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Packet, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));

  Acb.AtaCommand             = ATA_CMD_IDENTIFY_DRIVE;

  Packet.Protocol            = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
  Packet.Acb                 = &Acb;
  Packet.Asb                 = Asb;
  Packet.InDataBuffer        = (*IdentifyData);
  Packet.InTransferLength    = sizeof (ATA_IDENTIFY_DATA);
  Packet.Length              = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Timeout             = EFI_TIMER_PERIOD_SECONDS (EFI_ATA_PASS_THROUGH_DEFAULT_TIMEOUT);

  Status = AtaPassThru->PassThru (AtaPassThru, Port, PortMultiplierPort, &Packet, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to PassThru, Status: %r\n", __FUNCTION__, Status));
  }

  if (CalculateCheckSum8 ((UINT8 *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA)) != 0) {
    Status = EFI_CRC_ERROR;
  }

  if (IsZeroBuffer ((*IdentifyData), sizeof (ATA_IDENTIFY_DATA))) {
    Status = EFI_UNSUPPORTED;
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "====== Dump Identify Data ======\n"));
  DumpData ((UINT8 *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA));
  DEBUG ((EFI_D_VERBOSE, "================================\n"));
  DEBUG_CODE_END ();

  if (EFI_ERROR (Status)) {
    FreeAlignedBuffer ((VOID *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA));
    (*IdentifyData) = NULL;
  }
  FreeAlignedBuffer ((VOID *) Asb, sizeof (EFI_ATA_STATUS_BLOCK));

  return Status;
}

/**
  Get S.M.A.R.T data from the drive via ATA Pass-Through Protocol

  @param[in]      AtaPassThru         The pointer to the ATA Pass-Through protocol.
  @param[in]      Port                The port number of the target drive.
  @param[in]      PortMultiplierPort  The port multiplier port number of the target drive.
  @param[in,out]  SmartData           A pointer to the ATA_SMART_DATA. It is caller's responsibility
                                      to free this buffer. Please note that the buffer is actually an
                                      aligned page.

  @retval EFI_SUCCESS             Successfully get S.M.A.R.T data from the drive.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval EFI_UNSUPPORTED         The drive does not support the command or the data is all zero.
  @retval EFI_CRC_ERROR           Bad checksum.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
AtaSmartReadData (
  IN    EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN    UINT16                      Port,
  IN    UINT16                      PortMultiplierPort,
  OUT   ATA_SMART_DATA              **SmartData
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;

  if (AtaPassThru == NULL || SmartData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  (*SmartData) = AllocateAlignedBuffer (AtaPassThru->Mode->IoAlign, sizeof (ATA_SMART_DATA));
  if ((*SmartData) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Asb = AllocateAlignedBuffer (AtaPassThru->Mode->IoAlign, sizeof (EFI_ATA_STATUS_BLOCK));
  if (Asb == NULL) {
    FreeAlignedBuffer ((VOID *)(*SmartData), sizeof (ATA_SMART_DATA));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Packet, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));

  Acb.AtaCommand            = ATA_CMD_SMART;
  Acb.AtaFeatures           = ATA_SMART_READ_DATA;
  Acb.AtaCylinderLow        = ATA_CONSTANT_4F;
  Acb.AtaCylinderHigh       = ATA_CONSTANT_C2;

  Packet.Protocol           = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
  Packet.Acb                = &Acb;
  Packet.Asb                = Asb;
  Packet.InDataBuffer       = (*SmartData);
  Packet.InTransferLength   = sizeof (ATA_SMART_DATA);
  Packet.Length             = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Timeout            = EFI_TIMER_PERIOD_SECONDS (EFI_ATA_PASS_THROUGH_DEFAULT_TIMEOUT);

  Status = AtaPassThru->PassThru (AtaPassThru, Port, PortMultiplierPort, &Packet, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to PassThru, Status: %r\n", __FUNCTION__, Status));
  }

  if (CalculateCheckSum8 ((UINT8 *)(*SmartData), sizeof (ATA_SMART_DATA)) != 0) {
    Status = EFI_CRC_ERROR;
  }

  if (IsZeroBuffer ((*SmartData), sizeof (ATA_SMART_DATA))) {
    Status = EFI_UNSUPPORTED;
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "====== Dump S.M.A.R.T Data ======\n"));
  DumpData ((UINT8 *)(*SmartData), sizeof (ATA_SMART_DATA));
  DEBUG ((EFI_D_VERBOSE, "=================================\n\n"));
  DEBUG_CODE_END ();

  if (EFI_ERROR (Status)) {
    FreeAlignedBuffer ((VOID *)(*SmartData), sizeof (ATA_SMART_DATA));
    (*SmartData) = NULL;
  }
  FreeAlignedBuffer ((VOID *) Asb, sizeof (EFI_ATA_STATUS_BLOCK));

  return Status;
}

/**
  Get S.M.A.R.T return status from the drive via ATA Pass-Through Protocol

  @param[in]      AtaPassThru         The pointer to the ATA Pass-Through protocol.
  @param[in]      Port                The port number of the target drive.
  @param[in]      PortMultiplierPort  The port multiplier port number of the target drive.
  @param[in,out]  ReturnStatus        A pointer to the S.M.A.R.T return status.
                                      If EFI_SUCCESS, it indicates the device health is good.
                                      If EFI_DEVICE_ERROR, it indicates the device has detected
                                      a threshold exceeded condition.
                                      If EFI_UNSUPPORTED, it indicates the device might not support
                                      returning the status through the ATA interface.

  @retval EFI_SUCCESS             Successfully get S.M.A.R.T return status from the drive.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
AtaSmartReturnStatus (
  IN    EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN    UINT16                      Port,
  IN    UINT16                      PortMultiplierPort,
  OUT   EFI_STATUS                  *ReturnStatus
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;

  if (AtaPassThru == NULL || ReturnStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Asb = AllocateAlignedBuffer (AtaPassThru->Mode->IoAlign, sizeof (EFI_ATA_STATUS_BLOCK));
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Packet, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));

  Acb.AtaCommand            = ATA_CMD_SMART;
  Acb.AtaFeatures           = ATA_SMART_RETURN_STATUS;
  Acb.AtaCylinderLow        = ATA_CONSTANT_4F;
  Acb.AtaCylinderHigh       = ATA_CONSTANT_C2;

  Packet.Protocol           = EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA;
  Packet.Acb                = &Acb;
  Packet.Asb                = Asb;
  Packet.Timeout            = EFI_TIMER_PERIOD_SECONDS (EFI_ATA_PASS_THROUGH_DEFAULT_TIMEOUT);

  Status = AtaPassThru->PassThru (AtaPassThru, Port, PortMultiplierPort, &Packet, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to PassThru, Status: %r\n", __FUNCTION__, Status));
  }

  *ReturnStatus = EFI_UNSUPPORTED;
  if ((Packet.Asb->AtaError & BIT2) == 0) {
    if (Packet.Asb->AtaCylinderLow == 0x4F &&
        Packet.Asb->AtaCylinderHigh == 0xC2) {
      *ReturnStatus = EFI_SUCCESS;
    } else if (Packet.Asb->AtaCylinderLow == 0xF4 &&
               Packet.Asb->AtaCylinderHigh == 0x2C){
      *ReturnStatus = EFI_DEVICE_ERROR;
    }
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "S.M.A.R.T Return Status: %r\n\n", ReturnStatus));
  DEBUG_CODE_END ();

  FreeAlignedBuffer ((VOID *) Asb, sizeof (EFI_ATA_STATUS_BLOCK));

  return Status;
}

/**
  Get Identify data from the drive via Extended SCSI Pass-Through Protocol

  @param[in]      ExtScsiPassThru   The pointer to the Extended SCSI Pass-Through protocol.
  @param[in]      Target            The pointer to the ID that is an array of size TARGET_MAX_BYTES
                                    of the target drive.
  @param[in]      Lun               The Lun of the target drive.
  @param[in,out]  IdentifyData      A pointer to the ATA_IDENTIFY_DATA. It is caller's responsibility
                                    to free this buffer. Please note that the buffer is actually an
                                    aligned page.

  @retval EFI_SUCCESS             Successfully get Identify data from the drive.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval EFI_UNSUPPORTED         The drive does not support the command or the data is all zero.
  @retval EFI_CRC_ERROR           Bad checksum.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
SatIdentifyDevice (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL   *ExtScsiPassThru,
  IN  UINT8                             *Target,
  IN  UINT64                            Lun,
  OUT ATA_IDENTIFY_DATA                 **IdentifyData
  )
{
  EFI_STATUS                                  Status;
  EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET  Packet;
  UINT8                                       Cdb[SAT_SCSI_OP_LENGTH_TWELVE];
  UINT8                                       HostAdapterStatus;
  UINT8                                       TargetStatus;

  if (ExtScsiPassThru == NULL || Target == NULL || IdentifyData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  (*IdentifyData) = AllocateAlignedBuffer (ExtScsiPassThru->Mode->IoAlign, sizeof (ATA_IDENTIFY_DATA));
  if ((*IdentifyData) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Packet, sizeof (EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET));
  ZeroMem (&Cdb, sizeof (Cdb));
  ZeroMem ((*IdentifyData), sizeof (ATA_IDENTIFY_DATA));

  Cdb[0] = EFI_SCSI_OP_ATA_PASS_THROUGH12;
  Cdb[1] = (V_SAT_PROTOCOL_PIO_DATA_IN << N_SAT_PROTOCOL);
  Cdb[2] = (V_SAT_DIRECTION_FROM_DEVICE << N_SAT_DIRECTION) |
           (V_SAT_BYTE_BLOCK_BLOCK << N_SAT_BYTE_BLOCK) |
           V_SAT_TRANSFER_LENGTH_IN_SECTOR_COUNT;
  Cdb[4] = 1;   // transfer 1 block
  Cdb[9] = ATA_CMD_IDENTIFY_DRIVE;

  Packet.CdbLength        = sizeof (Cdb);
  Packet.Cdb              = Cdb;
  Packet.DataDirection    = EFI_EXT_SCSI_DATA_DIRECTION_READ;
  Packet.InDataBuffer     = (UINT8 *)(*IdentifyData);
  Packet.InTransferLength = sizeof (ATA_IDENTIFY_DATA);
  Packet.Timeout          = EFI_TIMER_PERIOD_SECONDS (SCSI_PACKET_DEFAULT_TIMEOUT);

  Status = ExtScsiPassThru->PassThru (ExtScsiPassThru, Target, Lun, &Packet, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to PassThru, Status: %r\n", __FUNCTION__, Status));
  }

  HostAdapterStatus = Packet.HostAdapterStatus;
  TargetStatus      = Packet.TargetStatus;

  DEBUG ((EFI_D_VERBOSE, "%a() - HostAdapterStatus: %02x\n", __FUNCTION__, HostAdapterStatus));
  DEBUG ((EFI_D_VERBOSE, "%a() - TargetStatus:      %02x\n", __FUNCTION__, TargetStatus));

  if (IsZeroBuffer ((*IdentifyData), sizeof (ATA_IDENTIFY_DATA))) {
    Status = EFI_UNSUPPORTED;
  }

  if (CalculateCheckSum8 ((UINT8 *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA)) != 0) {
    Status = EFI_CRC_ERROR;
  }

  DEBUG ((EFI_D_VERBOSE, "%a() - Status after integrity check: %r\n", __FUNCTION__, Status));

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "====== Dump Identify Data ======\n"));
  DumpData ((UINT8 *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA));
  DEBUG ((EFI_D_VERBOSE, "================================\n\n"));
  DEBUG_CODE_END ();

  if (EFI_ERROR (Status)) {
    FreeAlignedBuffer ((VOID *)(*IdentifyData), sizeof (ATA_IDENTIFY_DATA));\
    (*IdentifyData) = NULL;
  }

  return Status;
}

/**
  Get S.M.A.R.T data from the drive via Extended SCSI Pass-Through Protocol

  @param[in]      ExtScsiPassThru   The pointer to the Extended SCSI Pass-Through protocol.
  @param[in]      Target            The pointer to the ID that is an array of size TARGET_MAX_BYTES
                                    of the target drive.
  @param[in]      Lun               The Lun of the target drive.
  @param[in,out]  SmartData         A pointer to the ATA_SMART_DATA. It is caller's responsibility
                                    to free this buffer. Please note that the buffer is actually an
                                    aligned page.

  @retval EFI_SUCCESS             Successfully get Identify data from the drive.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval EFI_UNSUPPORTED         The drive does not support the command or the data is all zero.
  @retval EFI_CRC_ERROR           Bad checksum.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
SatSmartReadData (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL   *ExtScsiPassThru,
  IN  UINT8                             *Target,
  IN  UINT64                            Lun,
  OUT ATA_SMART_DATA                    **SmartData
  )
{
  EFI_STATUS                                  Status;
  EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET  Packet;
  UINT8                                       Cdb[SAT_SCSI_OP_LENGTH_TWELVE];
  UINT8                                       HostAdapterStatus;
  UINT8                                       TargetStatus;

  if (ExtScsiPassThru == NULL || Target == NULL || SmartData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  (*SmartData) = AllocateAlignedBuffer (ExtScsiPassThru->Mode->IoAlign, sizeof (ATA_SMART_DATA));
  if ((*SmartData) == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Packet, sizeof (EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET));
  ZeroMem (&Cdb, sizeof (Cdb));
  ZeroMem ((*SmartData), sizeof (ATA_SMART_DATA));

  Cdb[0] = EFI_SCSI_OP_ATA_PASS_THROUGH12;
  Cdb[1] = (V_SAT_PROTOCOL_PIO_DATA_IN << N_SAT_PROTOCOL);
  Cdb[2] = (V_SAT_DIRECTION_FROM_DEVICE << N_SAT_DIRECTION) |
           (V_SAT_BYTE_BLOCK_BLOCK << N_SAT_BYTE_BLOCK) |
           V_SAT_TRANSFER_LENGTH_IN_SECTOR_COUNT;
  Cdb[3] = ATA_SMART_READ_DATA;
  Cdb[4] = 1;   // transfer 1 block
  Cdb[6] = ATA_CONSTANT_4F;
  Cdb[7] = ATA_CONSTANT_C2;
  Cdb[9] = ATA_CMD_SMART;

  Packet.CdbLength        = sizeof (Cdb);
  Packet.Cdb              = Cdb;
  Packet.DataDirection    = EFI_EXT_SCSI_DATA_DIRECTION_READ;
  Packet.InDataBuffer     = (UINT8 *) (*SmartData);
  Packet.InTransferLength = sizeof (ATA_SMART_DATA);
  Packet.Timeout          = EFI_TIMER_PERIOD_SECONDS (SCSI_PACKET_DEFAULT_TIMEOUT);

  Status = ExtScsiPassThru->PassThru (ExtScsiPassThru, Target, Lun, &Packet, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to PassThru, Status: %r\n", __FUNCTION__, Status));
  }

  HostAdapterStatus = Packet.HostAdapterStatus;
  TargetStatus      = Packet.TargetStatus;

  DEBUG ((EFI_D_VERBOSE, "%a() - HostAdapterStatus: %02x\n", __FUNCTION__, HostAdapterStatus));
  DEBUG ((EFI_D_VERBOSE, "%a() - TargetStatus:      %02x\n", __FUNCTION__, TargetStatus));

  if (IsZeroBuffer ((*SmartData), sizeof (ATA_SMART_DATA))) {
    Status = EFI_UNSUPPORTED;
  }

  if (CalculateCheckSum8 ((UINT8 *)(*SmartData), sizeof (ATA_SMART_DATA)) != 0) {
    Status = EFI_CRC_ERROR;
  }

  DEBUG ((EFI_D_VERBOSE, "%a() - Status after integrity check: %r\n", __FUNCTION__, Status));

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "====== Dump S.M.A.R.T Data ======\n"));
  DumpData ((UINT8 *)(*SmartData), sizeof (ATA_SMART_DATA));
  DEBUG ((EFI_D_VERBOSE, "=================================\n\n"));
  DEBUG_CODE_END ();

  if (EFI_ERROR (Status)) {
    FreeAlignedBuffer ((VOID *)(*SmartData), sizeof (ATA_SMART_DATA));
    (*SmartData) = NULL;
  }

  return Status;
}

/**
  Get S.M.A.R.T return status from the drive via Extended SCSI Pass-Through Protocol

  @param[in]      ExtScsiPassThru   The pointer to the Extended SCSI Pass-Through protocol.
  @param[in]      Target            The pointer to the ID that is an array of size TARGET_MAX_BYTES
                                    of the target drive.
  @param[in]      Lun               The Lun of the target drive.
  @param[in,out]  ReturnStatus      A pointer to the S.M.A.R.T return status.
                                    If EFI_SUCCESS, it indicates the device health is good.
                                    If EFI_DEVICE_ERROR, it indicates the device has detected
                                    a threshold exceeded condition.
                                    If EFI_UNSUPPORTED, it indicates the device might not support
                                    returning the status through the ATA interface.

  @retval EFI_SUCCESS             Successfully get S.M.A.R.T return status from the drive.
  @retval EFI_INVALID_PARAMETER   Invalid Parameter.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
SatSmartReturnStatus (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL   *ExtScsiPassThru,
  IN  UINT8                             *Target,
  IN  UINT64                            Lun,
  OUT EFI_STATUS                        *ReturnStatus
  )
{
  EFI_STATUS                                  Status;
  EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET  Packet;
  UINT8                                       Cdb[SAT_SCSI_OP_LENGTH_TWELVE];
  UINT8                                       HostAdapterStatus;
  UINT8                                       TargetStatus;
  UINT8                                       *SenseData;

  if (ExtScsiPassThru == NULL || Target == NULL || ReturnStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SenseData = AllocateAlignedBuffer (ExtScsiPassThru->Mode->IoAlign, SCSI_PACKET_DEFAULT_SENSE_DATA_LENGTH);
  if (SenseData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Packet, sizeof (EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET));
  ZeroMem (&Cdb, sizeof (Cdb));
  ZeroMem (SenseData, EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (SCSI_PACKET_DEFAULT_SENSE_DATA_LENGTH)));

  Cdb[0] = EFI_SCSI_OP_ATA_PASS_THROUGH12;
  Cdb[1] = (V_SAT_PROTOCOL_NON_DATA << N_SAT_PROTOCOL);
  Cdb[2] = B_SAT_CHECK_CONDITION | (V_SAT_DIRECTION_FROM_DEVICE << N_SAT_DIRECTION);
  Cdb[3] = ATA_SMART_RETURN_STATUS;
  Cdb[6] = ATA_CONSTANT_4F;
  Cdb[7] = ATA_CONSTANT_C2;
  Cdb[9] = ATA_CMD_SMART;

  Packet.CdbLength        = sizeof (Cdb);
  Packet.Cdb              = Cdb;
  Packet.DataDirection    = EFI_EXT_SCSI_DATA_DIRECTION_READ;
  Packet.SenseData        = SenseData;
  Packet.SenseDataLength  = SCSI_PACKET_DEFAULT_SENSE_DATA_LENGTH;
  Packet.Timeout          = EFI_TIMER_PERIOD_SECONDS (SCSI_PACKET_DEFAULT_TIMEOUT);

  Status = ExtScsiPassThru->PassThru (ExtScsiPassThru, Target, Lun, &Packet, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to PassThru, Status: %r\n", __FUNCTION__, Status));
  }

  HostAdapterStatus = Packet.HostAdapterStatus;
  TargetStatus      = Packet.TargetStatus;

  DEBUG ((EFI_D_VERBOSE, "%a() - HostAdapterStatus: %02x\n", __FUNCTION__, HostAdapterStatus));
  DEBUG ((EFI_D_VERBOSE, "%a() - TargetStatus:      %02x\n", __FUNCTION__, TargetStatus));

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "====== Dump Sense Data ======\n"));
  DumpData (SenseData, Packet.SenseDataLength);
  DEBUG ((EFI_D_VERBOSE, "=============================\n\n"));
  DEBUG_CODE_END ();

  *ReturnStatus = EFI_UNSUPPORTED;
  if (SenseData[0] == 72 || SenseData[0] == 73) {           // Descriptor Format
    if (SenseData[2] == 0x00 && SenseData[3] == 0x1D) {     // ATA PASS THROUGH INFORMATION AVAILABLE
      if (SenseData[8] == 0x09 && SenseData[9] == 0x0C) {   // ATA Status Return Descriptor
        if (SenseData[17] == 0x4F && SenseData[19] == 0xC2) {
          *ReturnStatus = EFI_SUCCESS;
        } else if (SenseData[17] == 0xF4 && SenseData[19] == 0x2C) {
          *ReturnStatus = EFI_DEVICE_ERROR;
        }
      }
    }
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((EFI_D_VERBOSE, "S.M.A.R.T Return Status: %r\n\n", ReturnStatus));
  DEBUG_CODE_END ();

  FreeAlignedBuffer (SenseData, 32);

  return Status;
}