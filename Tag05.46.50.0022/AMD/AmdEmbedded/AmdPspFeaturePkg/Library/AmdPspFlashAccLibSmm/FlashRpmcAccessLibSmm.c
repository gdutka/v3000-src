/** @file
  Provide SPI interface for RPMC command.

;******************************************************************************
;* Copyright (c) 2018 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "FlashRpmcAccessLibSmm.h"

SPI_RPMC_PARAMETER_TABLE  mRpmcInfo;

VOID
EFIAPI
DumpBufferData (
  IN VOID   *Ptr,
  IN UINT8  BufferSize
  )
{
  UINT8  Index;

  for (Index = 0; Index < BufferSize; Index++) {
    DEBUG ((EFI_D_INFO, "%x ", *((UINT8 *) Ptr + Index)));
  }
  DEBUG ((EFI_D_INFO, "\n"));
}

EFI_STATUS
EFIAPI
ExecuteStatus (
  OUT RPMC_READ_DATA_CMD_INFO  *ReadData
  )
{
  UINT8                    DataOutSize;
  UINT8                    Dummy;
  RPMC_READ_DATA_CMD_INFO  ReadDataCmdInfo;
  BOOLEAN                  ErrorFlag;

  DEBUG ((EFI_D_INFO, "Entry %a ()\n", __FUNCTION__));

  if (ReadData != NULL) {
    DataOutSize  = sizeof (RPMC_READ_DATA_CMD_INFO);
  } else {
    DataOutSize = 1;
  }
  Dummy        = 0x00;
  ErrorFlag    = TRUE;

  SendSpiCmd (RPMC_OP2, &Dummy, 0x01, (UINT8 *) (&ReadDataCmdInfo), &DataOutSize);
  if ((ReadDataCmdInfo.ExtendedStatus | RPMC_CMD_SUCCESSFUL_COMPLETION) == RPMC_CMD_SUCCESSFUL_COMPLETION) {
    //
    //  The successful completion of OP1 command.
    //
    ErrorFlag = FALSE;
  }

  if (ErrorFlag) {
    DEBUG ((EFI_D_ERROR, "!!! Previoius OP1 Command executed ERROR(0x%x) !!!\n", ReadDataCmdInfo.ExtendedStatus));
    DEBUG ((EFI_D_ERROR, "Exit %a ()\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if (ReadData != NULL) {
    DEBUG ((EFI_D_INFO, "ReadDataCmdInfo.Tag            :\n"));
    DumpBufferData (&ReadDataCmdInfo.Tag, (sizeof (ReadDataCmdInfo.Tag) / sizeof (ReadDataCmdInfo.Tag[0])));
    DEBUG ((EFI_D_INFO, "ReadDataCmdInfo.CounterReadData: 0x%x\n", ReadDataCmdInfo.CounterReadData));
    DEBUG ((EFI_D_INFO, "ReadDataCmdInfo.Signature      :\n"));
    DumpBufferData (&ReadDataCmdInfo.Signature, (sizeof (ReadDataCmdInfo.Signature) / sizeof (ReadDataCmdInfo.Signature[0])));

    ReadData->ExtendedStatus = ReadDataCmdInfo.ExtendedStatus;
    ReadData->CounterReadData = ReadDataCmdInfo.CounterReadData;
    CopyMem (&ReadData->Tag[0], &ReadDataCmdInfo.Tag[0], sizeof (ReadDataCmdInfo.Tag) / sizeof (ReadDataCmdInfo.Tag[0]));
    CopyMem (&ReadData->Signature[0], &ReadDataCmdInfo.Signature[0], sizeof (ReadDataCmdInfo.Signature) / sizeof (ReadDataCmdInfo.Signature[0]));
  }

  DEBUG ((EFI_D_INFO, "Previous OP1 Command successful completion.\n"));
  DEBUG ((EFI_D_INFO, "Exit %a ()\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Confirm previous OP1 command executed status.
  If previous OP1 command is Request Monotonic Counter and if Serial Flash returns successful
  completion extended status then it must also return valid value in the Tag, Counter Data and
  Signature field.

  @param[out] ReadData          Return the current counter data and tag, signature if it is not NULL.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval Others                Some error occurs when executing this function.
**/
EFI_STATUS
EFIAPI
CmdStatus (
  OUT RPMC_READ_DATA_CMD_INFO  *ReadData
  )
{
  EFI_STATUS               Status;
  UINT8                    StatusReg;
  UINT8                    DataOutSize;
  RPMC_READ_DATA_CMD_INFO  ReadDataCmdInfo;
  UINT8                    Dummy;
  UINT32                   TimeoutCount;
  UINT32                   PollingCount;

  DEBUG ((EFI_D_INFO, "Entry %a ()\n", __FUNCTION__));

  Status              = EFI_SUCCESS;
  StatusReg           = 0xFF;
  DataOutSize         = 0x01;
  ZeroMem (&ReadDataCmdInfo, sizeof (RPMC_READ_DATA_CMD_INFO));
  Dummy               = 0x00;
  TimeoutCount        = PcdGet32 (PcdPspRpmcTimeoutMicroSeconds);
  PollingCount        = 0;

  // Refer to the parameter settings of the ROM part, the Busy_Polling_Method is defined as follows:
  // 0 = Poll for OP1 busy using OP2 Extended Status[0].
  // 1 = Poll for OP1 busy using RDSR(Opcode: 0x05).
  if (mRpmcInfo.BusyPollingMethod) {
    DEBUG ((EFI_D_INFO, "Poll for OP1 busy using RDSR\n"));

    //
    // Make sure the SPI rom is ready to take command.
    //
    while (TRUE) {
      SendSpiCmd (RDSR, NULL, 0x00, &StatusReg, &DataOutSize);
      DEBUG ((EFI_D_INFO, "BusyStatus: 0x%x\n", StatusReg));
      if ((StatusReg & (RPMC_RDSR_WIP|RPMC_RDSR_WEL)) == 0) {
        break;
      }

      PollingCount++;
      if (PollingCount > TimeoutCount) {
        DEBUG ((EFI_D_ERROR, "!!! Timeout occurred !!!\n"));
        DEBUG ((EFI_D_ERROR, "Exit %a ()\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
      MicroSecondDelay (1);
    }
  } else {
    DEBUG((EFI_D_INFO, "Poll for OP1 busy using OP2 Extended Status[0]\n"));

    //
    // Make sure the spi rom is ready to take command.
    //
    while (TRUE) {
      SendSpiCmd (RPMC_OP2, &Dummy, 0x01, (UINT8 *) (&ReadDataCmdInfo), &DataOutSize);
      DEBUG ((EFI_D_INFO, "BusyStatus: 0x%x\n", 0, ReadDataCmdInfo.ExtendedStatus));
      if ((ReadDataCmdInfo.ExtendedStatus & RPMC_CMD_BUSY) != RPMC_CMD_BUSY) {
        break;
      }

      PollingCount++;
      if (PollingCount > TimeoutCount) {
        DEBUG ((EFI_D_ERROR, "!!! Timeout occurred !!!\n"));
        DEBUG ((EFI_D_ERROR, "Exit %a ()\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
      MicroSecondDelay (1);
    }
  }

  //
  // SPI rom is idle.
  // Check the previous OP1 command executed status.
  //
  Status = ExecuteStatus (ReadData);

  DEBUG ((EFI_D_INFO, "Exit %a ()\n", __FUNCTION__));
  return Status;
}

/**
  Check the Rom part whether supports the RPMC function.

  @retval TRUE                  The ROM part supported RPMC function.
  @retval FALSE                 The ROM part unsupported RPMC function.
**/
BOOLEAN
EFIAPI
RpmcSupportStatus (
  VOID
  )
{
  UINT32                     HeaderAddr;
  UINT32                     SwappedAddress;
  UINT8                      AddrLen;
  SPI_SFDP_HEADER            SfdpHeader;
  UINT8                      DataOutSize;
  SPI_SFDP_PARAMETER_HEADER  SfdpTable;
  UINT8                      HeaderIndex;
  UINT8                      PtpAddrLen;

  DEBUG ((EFI_D_INFO, "Entry %a ()\n", __FUNCTION__));

  //
  // Because the SFDP header's start address is 0x00 and the opcode command format need 3 address + 1 dummy.
  //
  HeaderAddr = 0;
  AddrLen = sizeof (HeaderAddr);
  DataOutSize = sizeof (SPI_SFDP_HEADER);
  ZeroMem (&SfdpHeader, DataOutSize);
  ZeroMem (&SfdpTable, sizeof (SPI_SFDP_PARAMETER_HEADER));
  PtpAddrLen = 0x00;

  //
  // Get SFDP table header.
  //
  SendSpiCmd (RDSFDP, (UINT8 *) &HeaderAddr, AddrLen, (UINT8 *) &SfdpHeader, &DataOutSize);
  if ((SfdpHeader.SfdpSignature) == SFDP_SIGNATURE) {
    DEBUG ((EFI_D_INFO, "Found SFDP table header!\n"));

    //
    // Get parameter header ID number's offset.
    //
    HeaderAddr += sizeof (SPI_SFDP_HEADER);
    DataOutSize = sizeof (SPI_SFDP_PARAMETER_HEADER);
    for (HeaderIndex = 0; HeaderIndex <= SfdpHeader.NumOfParameterHeader; HeaderIndex++) {
      //
      // Shift 8 bits for dummy cycle.
      //
      SwappedAddress = SwapBytes32 (HeaderAddr << 8);

      //
      // Seach each header ID number.
      // If the header ID number is 0x03 that means the ROM part has supported RPMC function.
      //
      SendSpiCmd (RDSFDP, (UINT8 *) &SwappedAddress, AddrLen, (UINT8 *) &SfdpTable, &DataOutSize);
      if (SfdpTable.ID == RPMC_PARAMETER_ID) {
        DEBUG ((EFI_D_INFO, "Found RPMC table ID!\n"));
        PtpAddrLen = sizeof (SfdpTable.ParameterTablePointer) / sizeof (SfdpTable.ParameterTablePointer[0]);
        ZeroMem (&HeaderAddr, AddrLen);
        CopyMem (&HeaderAddr, &SfdpTable.ParameterTablePointer[0], PtpAddrLen);

        DataOutSize = sizeof (SPI_RPMC_PARAMETER_TABLE);
        SwappedAddress = SwapBytes32 (HeaderAddr << 8);
        SendSpiCmd (RDSFDP, (UINT8 *) &SwappedAddress, AddrLen, (UINT8 *) &mRpmcInfo , &DataOutSize);
        DEBUG ((EFI_D_INFO, "mRpmcInfo: %x\n", mRpmcInfo));
        DEBUG ((EFI_D_INFO, "Got RPMC Parameter ID."));
        DEBUG ((EFI_D_INFO, "Exit %a ()\n", __FUNCTION__));
        return TRUE;
      }

      //
      // Get next parameter header ID number's offset.
      //
      HeaderAddr += sizeof (SPI_SFDP_PARAMETER_HEADER);
    }
  }

  DEBUG ((EFI_D_INFO, "Not found RPMC ID."));
  DEBUG ((EFI_D_INFO, "Exit %a ()\n", __FUNCTION__));
  return FALSE;
}

/**
  Increment the Monotonic counter by 1 inside the Serial Flash Device.

  @param[in] CounterAddr        Target Monotonic counter address.
  @param[in] CounterData        Current Monotonic counter value.
  @param[in] Signature          Point to the 32 bytes Signature.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval Others                Some error occurs when executing this function.
**/
EFI_STATUS
EFIAPI
RpmcIncMc (
  IN UINT8   CounterAddr,
  IN UINT32  CounterData,
  IN UINT8   *Signature
  )
{
  EFI_STATUS                  Status;
  RPMC_INCREMENT_MC_CMD_INFO  IncMcCmdInfo;

  DEBUG ((EFI_D_INFO, "Entry %a ()\n", __FUNCTION__));

  Status = EFI_SUCCESS;
  ZeroMem (&IncMcCmdInfo, sizeof (RPMC_INCREMENT_MC_CMD_INFO));

  //
  // Initialize command data.
  //
  IncMcCmdInfo.CmdType     = CmdIncMc;
  IncMcCmdInfo.CounterAddr = CounterAddr;
  IncMcCmdInfo.Rsvd        = 0x00;
  IncMcCmdInfo.CounterData = CounterData;
  CopyMem (&IncMcCmdInfo.Signature[0], Signature, sizeof (IncMcCmdInfo.Signature) / sizeof (IncMcCmdInfo.Signature[0]));

  DEBUG ((EFI_D_INFO, "Ready to transfer SPI ROM data as follows:\n"));
  DEBUG ((EFI_D_INFO, "IncMcCmdInfo.CmdType    : 0x%x\n", IncMcCmdInfo.CmdType));
  DEBUG ((EFI_D_INFO, "IncMcCmdInfo.CounterAddr: 0x%x\n", IncMcCmdInfo.CounterAddr));
  DEBUG ((EFI_D_INFO, "IncMcCmdInfo.Rsvd       : 0x%x\n", IncMcCmdInfo.Rsvd));
  DEBUG ((EFI_D_INFO, "IncMcCmdInfo.CounterData: 0x%x\n", IncMcCmdInfo.CounterData));
  DEBUG ((EFI_D_INFO, "IncMcCmdInfo.Signature  :\n"));
  DumpBufferData (&IncMcCmdInfo.Signature, (sizeof (IncMcCmdInfo.Signature) / sizeof (IncMcCmdInfo.Signature[0])));

  //
  // Send RPMC command: Increment Monotonic Counter.
  //
  SendSpiCmd (RPMC_OP1, (UINT8 *) (&IncMcCmdInfo), sizeof (RPMC_INCREMENT_MC_CMD_INFO), NULL, 0);

  //
  // Confirm command execution status.
  //
  Status = CmdStatus (NULL);

  DEBUG ((EFI_D_INFO, "Exit %a ()\n", __FUNCTION__));
  return Status;
}

/**
  Request the Monotonic counter value inside the Serial Flash Devicee.

  @param[in]      CounterAddr   Target Monotonic counter address.
  @param[in]      Tag           Point to the 12 bytes payload to be added.
  @param[in]      Signature     Point to the 32 bytes Signature.
  @param[in, out] CounterData   The latest Monotonic counter value.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval Others                Some error occurs when executing this function.
**/
EFI_STATUS
EFIAPI
RpmcReqMc (
  IN     UINT8   CounterAddr,
  IN     UINT8   *Tag,
  IN     UINT8   *Signature,
  IN OUT UINT32  *CounterData,
  IN OUT UINT8   *OutputSignature
  )
{
  EFI_STATUS                Status;
  RPMC_REQUEST_MC_CMD_INFO  ReqMcCmdInfo;
  RPMC_READ_DATA_CMD_INFO   ReadData;

  DEBUG ((EFI_D_INFO, "Entry %a ()\n", __FUNCTION__));

  Status = EFI_SUCCESS;
  ZeroMem (&ReqMcCmdInfo, sizeof (RPMC_REQUEST_MC_CMD_INFO));
  ZeroMem (&ReadData, sizeof (RPMC_READ_DATA_CMD_INFO));

  //
  // Initialize command data.
  //
  ReqMcCmdInfo.CmdType     = CmdReqMc;
  ReqMcCmdInfo.CounterAddr = CounterAddr;
  ReqMcCmdInfo.Rsvd        = 0x00;
  CopyMem (&ReqMcCmdInfo.Tag[0], Tag, sizeof (ReqMcCmdInfo.Tag) / sizeof (ReqMcCmdInfo.Tag[0]));
  CopyMem (&ReqMcCmdInfo.Signature[0], Signature, sizeof (ReqMcCmdInfo.Signature) / sizeof (ReqMcCmdInfo.Signature[0]));

  DEBUG ((EFI_D_INFO, "Ready to transfer SPI ROM data as follows:\n"));
  DEBUG ((EFI_D_INFO, "ReqMcCmdInfo.CmdType    : 0x%x\n", ReqMcCmdInfo.CmdType));
  DEBUG ((EFI_D_INFO, "ReqMcCmdInfo.CounterAddr: 0x%x\n", ReqMcCmdInfo.CounterAddr));
  DEBUG ((EFI_D_INFO, "ReqMcCmdInfo.Rsvd       : 0x%x\n", ReqMcCmdInfo.Rsvd));
  DEBUG ((EFI_D_INFO, "ReqMcCmdInfo.Tag        :\n"));
  DumpBufferData (&ReqMcCmdInfo.Tag, (sizeof (ReqMcCmdInfo.Tag) / sizeof (ReqMcCmdInfo.Tag[0])));
  DEBUG ((EFI_D_INFO, "ReqMcCmdInfo.Signature  :\n"));
  DumpBufferData (&ReqMcCmdInfo.Signature, (sizeof (ReqMcCmdInfo.Signature) / sizeof (ReqMcCmdInfo.Signature[0])));

  //
  // Send RPMC command: Request Monotonic Counter.
  //
  SendSpiCmd (RPMC_OP1, (UINT8 *) (&ReqMcCmdInfo), sizeof (RPMC_REQUEST_MC_CMD_INFO), NULL, 0);

  //
  // Confirm command execution status.
  //
  Status = CmdStatus (&ReadData);
  if (Status == EFI_SUCCESS) {
    *CounterData = ReadData.CounterReadData;
    CopyMem (OutputSignature, &ReadData.Signature[0], sizeof (ReadData.Signature) / sizeof (ReadData.Signature[0]));

    DEBUG ((EFI_D_INFO, "CounterData    : 0x%x\n", *CounterData));
    DEBUG ((EFI_D_INFO, "OutputSignature: \n"));
    DumpBufferData (OutputSignature, sizeof (ReadData.Signature) / sizeof (ReadData.Signature[0]));
  }

  DEBUG ((EFI_D_INFO, "Exit %a ()\n", __FUNCTION__));
  return Status;;
}

