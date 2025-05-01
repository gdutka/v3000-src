/** @file
  Header of AtaCommands

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __ATA_COMMANDS_H__
#define __ATA_COMMANDS_H__

#include <Uefi.h>

#include <Protocol/AtaPassThru.h>
#include <Protocol/ScsiPassThruExt.h>

#include <IndustryStandard/Atapi.h>

#include "SataDriveInfoDxe.h"

#define EFI_ATA_PASS_THROUGH_DEFAULT_TIMEOUT            3

#define EFI_SCSI_OP_ATA_PASS_THROUGH12                  0xA1

#define SCSI_PACKET_DEFAULT_SENSE_DATA_LENGTH           32
#define SCSI_PACKET_DEFAULT_TIMEOUT                     60

#define SAT_SCSI_OP_LENGTH_TWELVE                       12

//
// Bit Definitions in Cdb[1]
//
#define B_SAT_PROTOCOL                                  (BIT4 | BIT3 | BIT2 | BIT1)
#define N_SAT_PROTOCOL                                  1
#define   V_SAT_PROTOCOL_NON_DATA                       3
#define   V_SAT_PROTOCOL_PIO_DATA_IN                    4

//
// Bit Definitions in Cdb[2]
//
#define B_SAT_CHECK_CONDITION                           BIT5
#define B_SAT_DIRECTION                                 BIT3
#define N_SAT_DIRECTION                                 3
#define   V_SAT_DIRECTION_FROM_DEVICE                   1
#define   V_SAT_DIRECTION_TO_DEVICE                     0
#define B_SAT_BYTE_BLOCK                                BIT2
#define N_SAT_BYTE_BLOCK                                2
#define   V_SAT_BYTE_BLOCK_BLOCK                        1
#define   V_SAT_BYTE_BLOCK_BYTE                         0
#define B_SAT_TRANSFER_LENGTH                           (BIT1 | BIT0)
#define   V_SAT_TRANSFER_LENGTH_NO_DATA                 0
#define   V_SAT_TRANSFER_LENGTH_IN_SECTOR_COUNT         2

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

#endif