/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Flash Access PEI Library
 * It will route the Flash access to SMM libary through SMM communication protocol
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD PSP UEFI Driver
 * @e sub-project   PspStorage Library
 * @e \$Revision$   @e \$Date$
 *
 *//*****************************************************************************
 *
 * Copyright 2008 - 2019 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 * AMD is granting you permission to use this software and documentation (if
 * any) (collectively, the "Materials") pursuant to the terms and conditions of
 * the Software License Agreement included with the Materials.  If you do not
 * have a copy of the Software License Agreement, contact your AMD
 * representative for a copy.
 *
 * You agree that you will not reverse engineer or decompile the Materials, in
 * whole or in part, except as allowed by applicable law.
 *
 * WARRANTY DISCLAIMER:  THE MATERIALS ARE PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND.  AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT, THAT THE
 * MATERIALS WILL RUN UNINTERRUPTED OR ERROR-FREE OR WARRANTIES ARISING FROM
 * CUSTOM OF TRADE OR COURSE OF USAGE.  THE ENTIRE RISK ASSOCIATED WITH THE USE
 * OF THE MATERIAL IS ASSUMED BY YOU.  Some jurisdictions do not allow the
 * exclusion of implied warranties, so the above exclusion may not apply to
 * You.
 *
 * LIMITATION OF LIABILITY AND INDEMNIFICATION:  AMD AND ITS LICENSORS WILL
 * NOT, UNDER ANY CIRCUMSTANCES BE LIABLE TO YOU FOR ANY PUNITIVE, DIRECT,
 * INCIDENTAL, INDIRECT, SPECIAL OR CONSEQUENTIAL DAMAGES ARISING FROM USE OF
 * THE MATERIALS OR THIS AGREEMENT EVEN IF AMD AND ITS LICENSORS HAVE BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  In no event shall AMD's total
 * liability to You for all damages, losses, and causes of action (whether in
 * contract, tort (including negligence) or otherwise) exceed the amount of
 * $100 USD. You agree to defend, indemnify and hold harmless AMD and its
 * licensors, and any of their directors, officers, employees, affiliates or
 * agents from and against any and all loss, damage, liability and other
 * expenses (including reasonable attorneys' fees), resulting from Your use of
 * the Materials or violation of the terms and conditions of this Agreement.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS:  The Materials are provided with
 * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
 * subject to the restrictions as set forth in FAR 52.227-14 and
 * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
 * Government constitutes acknowledgment of AMD's proprietary rights in them.
 *
 * EXPORT RESTRICTIONS: The Materials may be subject to export restrictions as
 * stated in the Software License Agreement.
 *******************************************************************************
 */
#include "AmdPspFlashAccLibPei.h"

BOOLEAN                               mRpmcSupport = FALSE;
SPI_RPMC_PARAMETER                    mRpmcInfo;


/**
 * Retrieve block size of Flash Device
 *
 * @param[in]       BlockSize       Size of each block
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspGetFlashBlockSize (
     OUT   UINTN                                       *BlockSize
  )
{
  if (BlockSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *BlockSize = GetFlashBlockSize();

  return EFI_SUCCESS;
}

/**
 * Read data from Flash device
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been read on output
 * @param[in]       Buffer       Buffer contain the read data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspReadFlash (
  IN       UINTN                                        FlashAddress,
  IN OUT   UINTN                                        *NumBytes,
     OUT   UINT8                                        *Buffer
  )
{
  if (Buffer == NULL || *NumBytes == 0)
  {
    return EFI_INVALID_PARAMETER;
  }

  return FlashRead ( (UINT8*)Buffer, (UINT8*)FlashAddress, (UINTN)*NumBytes);
}


/**
 * Write data to Flash device
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been written on output
 * @param[in]       Buffer       Buffer contain the written data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspWriteFlash (
  IN       UINTN                                        FlashAddress,
  IN OUT   UINTN                                        *NumBytes,
  IN       UINT8                                        *Buffer
  )
{
  EFI_STATUS                          Status;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FlashProgram (
              (UINT8 *)FlashAddress,
              (UINT8 *)Buffer,
              NumBytes,
              (FlashAddress & ~(0xFFFF))
              );

  
  return Status;
}

/**
 * Erase Flash region according to input in unit of block size
 *
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been erased on output
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEraseFlash (
  IN       UINTN                                        FlashAddress,
  IN OUT   UINTN                                        *NumBytes
  )
{
  if ((FlashAddress == 0) && (NumBytes == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return FlashErase (FlashAddress, *NumBytes);
}

/**
 * Increment the Monotonic counter by 1 inside the Serial Flash Device
 *
 *
 * @param[in]       CounterAddr   CounterAddr for IncMc command
 * @param[in]       CounterData   CounterData for IncMc command
 * @param[in]       Signature     Point to the 32 bytes Signature for IncMc command
 *
 * @retval EFI_SUCCESS      Command success
 * @retval Others           Error happens during handling the command
 */
EFI_STATUS
EFIAPI
PspRpmcIncMc (
  IN UINT8   CounterAddr,
  IN UINT32  CounterData,
  IN UINT8*  Signature
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (!mRpmcSupport) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO , "PspRpmcIncMc %x %x\n", CounterAddr, CounterData));

  Status = RpmcIncMc (
             CounterAddr,
             CounterData,
             Signature
             );
  if (EFI_ERROR (Status)) {
    DEBUG (((DEBUG_ERROR | DEBUG_INFO), "!!! RpmcIncMc () ERROR(%r) !!!\n", Status));
  }

  return Status;
}

/**
 * Request the Monotonic counter value inside the Serial Flash Device
 *
 *
 * @param[in]       CounterAddr   CounterAddr for ReqMc command
 * @param[in]       Tag           Tag for ReqMc command
 * @param[in]       Signature     Point to the 32 bytes Signature for ReqMc command
 * @param[in, out]  CounterData   Point to the output CounterData for ReqMc command
  *
 * @retval EFI_SUCCESS      Command success, CounterData is valid
 * @retval Others           Error happens during handling the command
 */
EFI_STATUS
EFIAPI
PspRpmcReqMc (
  IN   UINT8   CounterAddr,
  IN   UINT8   *Tag,
  IN   UINT8   *Signature,
  IN  OUT UINT32  *CounterData
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (!mRpmcSupport) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "PspRpmcReqMc %x\n", CounterAddr));

  Status = RpmcReqMc (
             CounterAddr,
             Tag,
             Signature,
             CounterData
             );
  if (EFI_ERROR (Status)) {
    DEBUG (((DEBUG_ERROR | DEBUG_INFO), "!!! RpmcReqMc () ERROR(%r) !!!\n", Status));
  }
  DEBUG ((DEBUG_INFO, "CounterData %x\n", *CounterData));

  return Status;
}
/**
 * Initial Psp Storage required information
 *
 *
 * @param[in]  SystemTable Point to EFI system table structure
 *
 * @retval EFI_SUCCESS    Initial success
 * @retval Others         Error happens during initialize
 */
EFI_STATUS
EFIAPI
AmdPspFlashAccLibPeiConstructor (
  VOID
  )
{
  
  mRpmcSupport = RpmcSupportStatus ();
  
  return EFI_SUCCESS;
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
  IN OUT UINT32  *CounterData
  )
{
  EFI_STATUS                Status;
  RPMC_REQUEST_MC_CMD_INFO  ReqMcCmdInfo;
  UINT8                     Index;
  RPMC_READ_DATA_CMD_INFO   ReadData;
  UINT8                     OutputSignature[32];

  DEBUG ((DEBUG_INFO, "Entry RpmcReqMc ()\n"));

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

  DEBUG ((DEBUG_INFO, "Ready to transfer spi data as follow:\n"));
  DEBUG ((DEBUG_INFO, "ReqMcCmdInfo.CmdType    : 0x%x\n", ReqMcCmdInfo.CmdType));
  DEBUG ((DEBUG_INFO, "ReqMcCmdInfo.CounterAddr: 0x%x\n", ReqMcCmdInfo.CounterAddr));
  DEBUG ((DEBUG_INFO, "ReqMcCmdInfo.Rsvd       : 0x%x\n", ReqMcCmdInfo.Rsvd));
  DEBUG ((DEBUG_INFO, "ReqMcCmdInfo.Tag        :\n"));
  for (Index = 0; Index < (sizeof (ReqMcCmdInfo.Tag) / sizeof (ReqMcCmdInfo.Tag[0])); Index++) {
    DEBUG ((DEBUG_INFO, "%x ", ReqMcCmdInfo.Tag[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "ReqMcCmdInfo.Signature  :\n"));
  for (Index = 0; Index < (sizeof (ReqMcCmdInfo.Signature) / sizeof (ReqMcCmdInfo.Signature[0])); Index++) {
    DEBUG ((DEBUG_INFO, "%x ", ReqMcCmdInfo.Signature[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

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

    DEBUG ((DEBUG_INFO, "CounterData    : 0x%x\n", *CounterData));
    DEBUG ((DEBUG_INFO, "OutputSignature: \n"));
    for (Index = 0; Index < 32; Index++) {
      DEBUG ((DEBUG_INFO, "%x ", OutputSignature[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }

  DEBUG ((DEBUG_INFO, "Exit RpmcReqMc ()\n"));
  return Status;;
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
  UINT8                       Index;

  DEBUG ((DEBUG_INFO, "Entry RpmcIncMc ()\n"));

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

  DEBUG((DEBUG_INFO, "sizeof (RPMC_INCREMENT_MC_CMD_INFO): 0x%x\n", sizeof (RPMC_INCREMENT_MC_CMD_INFO)));
  
  DEBUG ((DEBUG_INFO, "Ready to transfer spi data as follow:\n"));
  DEBUG ((DEBUG_INFO, "IncMcCmdInfo.CmdType    : 0x%x\n", IncMcCmdInfo.CmdType));
  DEBUG ((DEBUG_INFO, "IncMcCmdInfo.CounterAddr: 0x%x\n", IncMcCmdInfo.CounterAddr));
  DEBUG ((DEBUG_INFO, "IncMcCmdInfo.Rsvd       : 0x%x\n", IncMcCmdInfo.Rsvd));
  DEBUG ((DEBUG_INFO, "IncMcCmdInfo.CounterData: 0x%x\n", IncMcCmdInfo.CounterData));
  DEBUG ((DEBUG_INFO, "IncMcCmdInfo.Signature  :\n"));
  for (Index = 0; Index < (sizeof (IncMcCmdInfo.Signature) / sizeof (IncMcCmdInfo.Signature[0])); Index++) {
    DEBUG ((DEBUG_INFO, "%x ", IncMcCmdInfo.Signature[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  //
  // Send RPMC command: Increment Monotonic Counter.
  //
  SendSpiCmd (RPMC_OP1, (UINT8 *) (&IncMcCmdInfo), sizeof (RPMC_INCREMENT_MC_CMD_INFO), NULL, 0);

  //
  // Confirm command execution status.
  //
  Status = CmdStatus (NULL);

  DEBUG ((DEBUG_INFO, "Exit RpmcIncMc ()\n"));
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
  UINT8                      HeaderAddr[4];
  UINT8                      AddrLen;
  SPI_SFDP_HEADER            SfdpHeader;
  UINT8                      DataOutSize;
  SPI_SFDP_PARAMETER_HEADER  SfdpTable;
  UINT8                      HeaderIndex;
  UINT8                      PtpAddrLen;

  DEBUG ((DEBUG_INFO, "Entry RpmcSupportStatus ()\n"));

  //
  // Because the SFDP header's start address is 0x00 and the opcode command format need 3 address + 1 dummy.
  //
  AddrLen     = sizeof (HeaderAddr) / sizeof (HeaderAddr[0]);
  ZeroMem (&HeaderAddr[0], AddrLen);
  DataOutSize = sizeof (SPI_SFDP_HEADER);
  ZeroMem (&SfdpHeader, DataOutSize);
  ZeroMem (&SfdpTable, sizeof (SPI_SFDP_PARAMETER_HEADER));
  PtpAddrLen  = 0x00;

  //
  // Get SFDP table header.
  //
  SendSpiCmd (RDSFDP, &HeaderAddr[0], AddrLen, (UINT8 *) (&SfdpHeader), &DataOutSize);
  if ((SfdpHeader.SfdpSignature) == SFDP_SIGNATURE) {
    DEBUG ((DEBUG_INFO, "Found SFDP table header!\n"));

    DataOutSize = sizeof (SPI_SFDP_PARAMETER_HEADER);
    //
    // Get parameter header ID number's offset.
    //
    HeaderAddr[2] = HeaderAddr[2] + sizeof (SPI_SFDP_HEADER);
    for (HeaderIndex = 0; HeaderIndex <= SfdpHeader.NumOfParameterHeader; HeaderIndex++) {
      //
      // Seach each header ID number.
      // If the header ID number is 0x03 that means the ROM part has supported RPMC function.
      //
      SendSpiCmd (RDSFDP, &HeaderAddr[0], AddrLen, (UINT8 *) &SfdpTable, &DataOutSize);
      if (SfdpTable.ID == RPMC_PARAMETER_ID) {
        DEBUG ((DEBUG_INFO, "Found RPMC table ID!\n"));
        PtpAddrLen = sizeof (SfdpTable.ParameterTablePointer) / sizeof (SfdpTable.ParameterTablePointer[0]);
        CopyMem (&HeaderAddr[0], &SfdpTable.ParameterTablePointer[0], PtpAddrLen);
        
        DataOutSize = 0x01;
        SendSpiCmd (RDSFDP, &HeaderAddr[0], AddrLen, &mRpmcInfo.Byte0Raw , &DataOutSize);
        DEBUG ((DEBUG_INFO, "mRpmcInfo.Byte0Raw: %x\n", mRpmcInfo.Byte0Raw));
        DEBUG ((DEBUG_INFO, "Got RPMC Parameter ID. Exit RpmcSupportStatus ()\n"));
        return TRUE;
      }

      //
      // Get next parameter header ID number's offset.
      //
      HeaderAddr[2] = HeaderAddr[2] + sizeof (SPI_SFDP_PARAMETER_HEADER);
    }
  }

  DEBUG ((DEBUG_INFO, "Not found RPMC ID. Exit RpmcSupportStatus ()\n"));
  return FALSE;
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
  UINT16                   TimeoutCount;
  UINT8                    Index;

  DEBUG ((DEBUG_INFO, "Entry CmdStatus ()\n"));

  Status              = EFI_SUCCESS;
  StatusReg           = 0xFF;
  DataOutSize         = 0x01;
  ZeroMem (&ReadDataCmdInfo, sizeof (RPMC_READ_DATA_CMD_INFO));
  Dummy               = 0x00;
  TimeoutCount        = 0x00;
  
  // Refer to the parameter settings of the ROM part, the Busy_Polling_Method is defined as follows:
  // 0 = Poll for OP1 busy using OP2 Extended Status[0].
  // 1 = Poll for OP1 busy using RDSR(Opcode: 0x05).
  if (mRpmcInfo.Byte0.BusyPollingMethod) {
    DEBUG ((DEBUG_INFO, "Poll for OP1 busy using RDSR\n"));
    //
    // Make sure the SPI rom is ready to take command.
    //
    while (TRUE) {
      SendSpiCmd (RDSR, NULL, 0x00, &StatusReg, &DataOutSize);
      DEBUG ((DEBUG_INFO, "BusyStatus: 0x%x\n", StatusReg));
      if (StatusReg == 0x00) {
        break;
      }

      TimeoutCount++;
      if (TimeoutCount > 1000) {
        DEBUG (((DEBUG_ERROR | DEBUG_INFO), "!!! Timeout occurred !!!\n"));
        return EFI_NOT_READY;
      }
    }
  } else {
    DEBUG((DEBUG_INFO, "Poll for OP1 busy using OP2 Extended Status[0]\n"));

    //
    // Make sure the spi rom is ready to take command.
    //
    while (TRUE) {
      SendSpiCmd (RPMC_OP2, &Dummy, 0x01, (UINT8 *) (&ReadDataCmdInfo), &DataOutSize);
      DEBUG ((DEBUG_INFO, "BusyStatus: 0x%x\n", 0, ReadDataCmdInfo.ExtendedStatus));
      if ((ReadDataCmdInfo.ExtendedStatus & RPMC_CMD_BUSY) != RPMC_CMD_BUSY) {
        break;
      }

      TimeoutCount++;
      if (TimeoutCount > 1000) {
        DEBUG (((DEBUG_ERROR | DEBUG_INFO), "!!! Timeout occurred !!!\n"));
        return EFI_NOT_READY;
      }
    }
  }

  //
  // SPI rom is idle. 
  // Check the previous OP1 command executed status.
  //
  Status = ExecuteStatus (ReadData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Exit CmdStatus ()\n"));
    return Status;
  }

  if (ReadData != NULL) {
    DEBUG((DEBUG_INFO, "ReadData->CounterReadData: %x\n", ReadData->CounterReadData));
    DEBUG ((DEBUG_INFO, "ReadData->Tag           :\n"));
    for (Index = 0; Index < (sizeof (ReadData->Tag) / sizeof (ReadData->Tag[0])); Index++) {
      DEBUG ((DEBUG_INFO, "%x ", ReadData->Tag[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    DEBUG ((DEBUG_INFO, "IncMcCmdInfo.Signature  :\n"));
    for (Index = 0; Index < (sizeof (ReadData->Signature) / sizeof (ReadData->Signature[0])); Index++) {
      DEBUG ((DEBUG_INFO, "%x ", ReadData->Signature[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }

  DEBUG ((DEBUG_INFO, "Exit CmdStatus ()\n"));
  return EFI_SUCCESS;
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
  UINT8                    Index;
  
  DEBUG ((DEBUG_INFO, "Entry ExecuteStatus ()\n"));

  DataOutSize = sizeof (RPMC_READ_DATA_CMD_INFO);
  Dummy       = 0x00;

  SendSpiCmd (RPMC_OP2, &Dummy, 0x01, (UINT8 *) (&ReadDataCmdInfo), &DataOutSize);
  
  DEBUG((DEBUG_INFO, "ReadDataCmdInfo.ExtendedStatus : 0x%x\n", ReadDataCmdInfo.ExtendedStatus));
  DEBUG ((DEBUG_INFO, "ReadDataCmdInfo.Tag            :\n"));
  for (Index = 0; Index < (sizeof (ReadDataCmdInfo.Tag) / sizeof (ReadDataCmdInfo.Tag[0])); Index++) {
    DEBUG ((DEBUG_INFO, "%x ", ReadDataCmdInfo.Tag[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "ReadDataCmdInfo.CounterReadData: 0x%x\n", ReadDataCmdInfo.CounterReadData));
  DEBUG ((DEBUG_INFO, "ReadDataCmdInfo.Signature      : \n"));
  for (Index = 0; Index < (sizeof (ReadDataCmdInfo.Signature) / sizeof (ReadDataCmdInfo.Signature[0])); Index++) {
    DEBUG ((DEBUG_INFO, "%x ", ReadDataCmdInfo.Signature[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  if (ReadDataCmdInfo.ExtendedStatus != RPMC_CMD_SUCCESSFUL_COMPLETION) {
    //
    // Previous OP1 command executed wrong.
    //
    DEBUG (((DEBUG_ERROR | DEBUG_INFO), "!!! Previoius OP1 Command executed ERROR(0x%x) !!!\n", ReadDataCmdInfo.ExtendedStatus));
    DEBUG ((DEBUG_INFO, "Exit ExecuteStatus ()\n"));
    return EFI_DEVICE_ERROR;
  }

  if (ReadData != NULL) {
    ReadData->ExtendedStatus = ReadDataCmdInfo.ExtendedStatus;
    ReadData->CounterReadData = ReadDataCmdInfo.CounterReadData;
    CopyMem (&ReadData->Tag[0], &ReadDataCmdInfo.Tag[0], sizeof (ReadDataCmdInfo.Tag) / sizeof (ReadDataCmdInfo.Tag[0]));
    CopyMem (&ReadData->Signature[0], &ReadDataCmdInfo.Signature[0], sizeof (ReadDataCmdInfo.Signature) / sizeof (ReadDataCmdInfo.Signature[0]));

    DEBUG ((DEBUG_INFO, "ReadData.CounterReadData: 0x%x\n", ReadData->CounterReadData));
    DEBUG ((DEBUG_INFO, "ReadData.Tag            :\n"));
    for (Index = 0; Index < (sizeof (ReadData->Tag) / sizeof (ReadData->Tag[0])); Index++) {
      DEBUG ((DEBUG_INFO, "%x ", ReadData->Tag[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    DEBUG ((DEBUG_INFO, "ReadData.Signature      : \n"));
    for (Index = 0; Index < (sizeof (ReadData->Signature) / sizeof (ReadData->Signature[0])); Index++) {
      DEBUG ((DEBUG_INFO, "%x ", ReadData->Signature[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }

  DEBUG ((DEBUG_INFO, "Previous OP1 Command successful completion. Exit ExecuteStatus ()\n"));
  return EFI_SUCCESS;
}

