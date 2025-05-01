/** @file
  Basic TIS (TCM/TCM Interface Specification) functions.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/TcmCommandLib.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/TpmTis.h>

#include <Tcm.h>

#define TCM_DID_0201               0x0201
#define TCM_DID_0601               0x0601

/**
  Check whether TCM chip exist.

  @param[in] TisReg  Pointer to TIS register.

  @retval    TRUE    TCM chip exists.
  @retval    FALSE   TCM chip is not found.
**/
BOOLEAN
TcmTisPcPresenceCheck (
  IN      TIS_PC_REGISTERS_PTR      TisReg
  )
{
  UINT8                             RegRead;

  RegRead = MmioRead8 ((UINTN)&TisReg->Access);
  return (BOOLEAN)(RegRead != (UINT8)-1);
}

/**
  Check whether the value of a TCM chip register satisfies the input BIT setting.

  @param[in]  Register     Address port of register to be checked.
  @param[in]  BitSet       Check these data bits are set.
  @param[in]  BitClear     Check these data bits are clear.
  @param[in]  TimeOut      The max wait time (unit MicroSecond) when checking register.

  @retval     EFI_SUCCESS  The register satisfies the check bit.
  @retval     EFI_TIMEOUT  The register can't run into the expected status in time.
**/
EFI_STATUS
TcmTisPcWaitRegisterBits (
  IN      UINT8                     *Register,
  IN      UINT8                     BitSet,
  IN      UINT8                     BitClear,
  IN      UINT32                    TimeOut
  )
{
  UINT8                             RegRead;
  UINT32                            WaitTime;

  MicroSecondDelay (60);

  for (WaitTime = 0; WaitTime < TimeOut; WaitTime += 30){
    RegRead = MmioRead8 ((UINTN)Register);
    if ((RegRead & BitSet) == BitSet && (RegRead & BitClear) == 0)
      return EFI_SUCCESS;
    MicroSecondDelay (30);
  }
  return EFI_TIMEOUT;
}

/**
  Get BurstCount by reading the burstCount field of a TIS regiger
  in the time of default TIS_TIMEOUT_D.

  @param[in]  TisReg                Pointer to TIS register.
  @param[out] BurstCount            Pointer to a buffer to store the got BurstConut.

  @retval     EFI_SUCCESS           Get BurstCount.
  @retval     EFI_INVALID_PARAMETER TisReg is NULL or BurstCount is NULL.
  @retval     EFI_TIMEOUT           BurstCount can't be got in time.
**/
EFI_STATUS
TcmTisPcReadBurstCount (
  IN      TIS_PC_REGISTERS_PTR      TisReg,
     OUT  UINT16                    *BurstCount
  )
{
  UINT32                            WaitTime;
  UINT8                             DataByte0;
  UINT8                             DataByte1;

  if (BurstCount == NULL || TisReg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MicroSecondDelay (60);

  WaitTime = 0;
  do {
    //
    // TIS_PC_REGISTERS_PTR->burstCount is UINT16, but it is not 2bytes aligned,
    // so it needs to use MmioRead8 to read two times
    //
    DataByte0   = MmioRead8 ((UINTN)&TisReg->BurstCount);
    DataByte1   = MmioRead8 ((UINTN)&TisReg->BurstCount + 1);
    *BurstCount = (UINT16)((DataByte1 << 8) + DataByte0);
    if (*BurstCount != 0) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay (30);
    WaitTime += 30;
  } while (WaitTime < TIS_TIMEOUT_D);

  return EFI_TIMEOUT;
}

/**
  Set TCM chip to ready state by sending ready command TIS_PC_STS_READY
  to Status Register in time.

  @param[in] TisReg                Pointer to TIS register.

  @retval    EFI_SUCCESS           TCM chip enters into ready state.
  @retval    EFI_INVALID_PARAMETER TisReg is NULL.
  @retval    EFI_TIMEOUT           TCM chip can't be set to ready state in time.
**/
EFI_STATUS
TcmTisPcPrepareCommand (
  IN      TIS_PC_REGISTERS_PTR      TisReg
  )
{
  EFI_STATUS                        Status;

  if (TisReg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite8((UINTN)&TisReg->Status, TIS_PC_STS_READY);
  MicroSecondDelay (60);
  Status = TcmTisPcWaitRegisterBits (
             &TisReg->Status,
             TIS_PC_STS_READY,
             0,
             TIS_TIMEOUT_B
             );
  return Status;
}

/**
  Get the control of TCM chip by sending requestUse command TIS_PC_ACC_RQUUSE
  to ACCESS Register in the time of default TIS_TIMEOUT_A.

  @param[in] TisReg                Pointer to TIS register.

  @retval    EFI_SUCCESS           Get the control of TCM chip.
  @retval    EFI_INVALID_PARAMETER TisReg is NULL.
  @retval    EFI_NOT_FOUND         TCM chip doesn't exit.
  @retval    EFI_TIMEOUT           Can't get the TCM control in time.
**/
EFI_STATUS
TcmTisPcRequestUseTcm (
  IN      TIS_PC_REGISTERS_PTR      TisReg
  )
{
  EFI_STATUS                        Status;

  if (TisReg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!TcmTisPcPresenceCheck (TisReg)) {
    return EFI_NOT_FOUND;
  }

  if (MmioRead16 ((UINTN)&TisReg->Vid) != TCM_VID) {
    DEBUG ((EFI_D_INFO, "TCM not found! VID(%x) != %x\n", TisReg->Vid, TCM_VID));
    return EFI_NOT_FOUND;
  }

  if ((MmioRead16 ((UINTN)&TisReg->Did) != TCM_DID_0201) &&
      (MmioRead16 ((UINTN)&TisReg->Did) != TCM_DID_0601)) {
    DEBUG ((EFI_D_INFO, "TCM not found! DID(%x) is not supported\n", TisReg->Did));
    return EFI_NOT_FOUND;
  }

  MmioWrite8((UINTN)&TisReg->Access, TIS_PC_ACC_RQUUSE);
  MicroSecondDelay (60);
  Status = TcmTisPcWaitRegisterBits (
             &TisReg->Access,
             (UINT8)(TIS_PC_ACC_ACTIVE |TIS_PC_VALID),
             0,
             TIS_TIMEOUT_A
             );
  return Status;
}

/**
  Send a command to TCM for execution and return response data.

  @param[in]      TisReg        TCM register space base address.
  @param[in]      BufferIn      Buffer for command data.
  @param[in]      SizeIn        Size of command data.
  @param[in, out] BufferOut     Buffer for response data.
  @param[in, out] SizeOut       Size of response data.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.
  @retval EFI_UNSUPPORTED       Unsupported TCM version

**/
EFI_STATUS
TcmTisTcmCommand (
  IN     TIS_PC_REGISTERS_PTR       TisReg,
  IN     UINT8                      *BufferIn,
  IN     UINT32                     SizeIn,
  IN OUT UINT8                      *BufferOut,
  IN OUT UINT32                     *SizeOut
  )
{
  EFI_STATUS                        Status;
  UINT16                            BurstCount;
  UINT32                            Index;
  UINT32                            TcmOutSize;
  UINT16                            Data16;
  UINT32                            Data32;
  UINT16                            RspTag;

  DEBUG_CODE (
    UINTN  DebugSize;

    DEBUG ((EFI_D_VERBOSE, "TcmTisTcmCommand Send - "));
    if (SizeIn > 0x100) {
      DebugSize = 0x40;
    } else {
      DebugSize = SizeIn;
    }
    for (Index = 0; Index < DebugSize; Index++) {
      DEBUG ((EFI_D_VERBOSE, "%02x ", BufferIn[Index]));
    }
    if (DebugSize != SizeIn) {
      DEBUG ((EFI_D_VERBOSE, "...... "));
      for (Index = SizeIn - 0x20; Index < SizeIn; Index++) {
        DEBUG ((EFI_D_VERBOSE, "%02x ", BufferIn[Index]));
      }
    }
    DEBUG ((EFI_D_VERBOSE, "\n"));
  );
  TcmOutSize = 0;

  Status = TcmTisPcPrepareCommand (TisReg);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "Tcm is not ready for command!\n"));
    return EFI_DEVICE_ERROR;
  }
  //
  // Send the command data to Tcm
  //
  Index = 0;
  while (Index < SizeIn) {
    Status = TcmTisPcReadBurstCount (TisReg, &BurstCount);
    if (EFI_ERROR (Status)) {
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }
    for (; BurstCount > 0 && Index < SizeIn; BurstCount--) {
      MicroSecondDelay (60);
      MmioWrite8((UINTN)&TisReg->DataFifo, *(BufferIn + Index));
      Index++;
    }
  }

  MicroSecondDelay (200);

  //
  // Check the Tcm status STS_EXPECT change from 1 to 0
  //
  Status = TcmTisPcWaitRegisterBits (
             &TisReg->Status,
             (UINT8) TIS_PC_VALID,
             TIS_PC_STS_EXPECT,
             TIS_TIMEOUT_C
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tcm The send buffer too small!\n"));
    Status = EFI_BUFFER_TOO_SMALL;
    goto Exit;
  }
  //
  // Executed the TCM command and waiting for the response data ready
  //
  MmioWrite8((UINTN)&TisReg->Status, TIS_PC_STS_GO);
  MicroSecondDelay (200);
  Status = TcmTisPcWaitRegisterBits (
             &TisReg->Status,
             (UINT8) (TIS_PC_VALID | TIS_PC_STS_DATA),
             0,
             TIS_TIMEOUT_B
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Wait for Tcm response data time out!!\n"));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }
  //
  // Get response data header
  //
  Index = 0;
  BurstCount = 0;
  while (Index < sizeof (TCM_RSP_COMMAND_HDR)) {
    Status = TcmTisPcReadBurstCount (TisReg, &BurstCount);
    if (EFI_ERROR (Status)) {
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }
    for (; BurstCount > 0 && Index < sizeof (TCM_RSP_COMMAND_HDR); BurstCount--) {
      MicroSecondDelay (60);
      *(BufferOut + Index) = MmioRead8 ((UINTN)&TisReg->DataFifo);
      Index++;
    }
  }
  DEBUG_CODE (
    DEBUG ((EFI_D_VERBOSE, "TcmTisTcmCommand ReceiveHeader - "));
    for (Index = 0; Index < sizeof (TCM_RSP_COMMAND_HDR); Index++) {
      DEBUG ((EFI_D_VERBOSE, "%02x ", BufferOut[Index]));
    }
    DEBUG ((EFI_D_VERBOSE, "\n"));
  );
  //
  // Check the response data header (tag, parasize and returncode)
  //
  CopyMem (&Data16, BufferOut, sizeof (UINT16));
  if (SwapBytes16 (Data16) != TCM_TAG_RSP_COMMAND ) {
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }
  RspTag = SwapBytes16 (Data16);
  if (RspTag != TCM_TAG_RSP_COMMAND && RspTag != TCM_TAG_RSP_AUTH1_COMMAND && RspTag != TCM_TAG_RSP_AUTH2_COMMAND) {
    DEBUG ((EFI_D_ERROR, "TCM: Response tag error - current tag value is %x\n", RspTag));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  CopyMem (&Data32, (BufferOut + 2), sizeof (UINT32));
  TcmOutSize  = SwapBytes32 (Data32);
  if (*SizeOut < TcmOutSize) {
    Status = EFI_BUFFER_TOO_SMALL;
    goto Exit;
  }
  *SizeOut = TcmOutSize;
  //
  // Continue reading the remaining data
  //
  while ( Index < TcmOutSize ) {
    for (; BurstCount > 0; BurstCount--) {
      MicroSecondDelay (60);
      *(BufferOut + Index) = MmioRead8 ((UINTN)&TisReg->DataFifo);
      Index++;
      if (Index == TcmOutSize) {
        Status = EFI_SUCCESS;
        goto Exit;
      }
    }
    Status = TcmTisPcReadBurstCount (TisReg, &BurstCount);
    if (EFI_ERROR (Status)) {
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }
  }
Exit:
  DEBUG_CODE (
    DEBUG ((EFI_D_VERBOSE, "TcmTisTcmCommand Receive - "));
    for (Index = 0; Index < TcmOutSize; Index++) {
      DEBUG ((EFI_D_VERBOSE, "%02x ", BufferOut[Index]));
    }
    DEBUG ((EFI_D_VERBOSE, "\n"));
  );
  MmioWrite8((UINTN)&TisReg->Status, TIS_PC_STS_READY);
  return Status;
}

/**
  This service enables the sending of commands to the TCM.

  @param[in]      InputParameterBlockSize  Size of the TCM input parameter block.
  @param[in]      InputParameterBlock      Pointer to the TCM input parameter block.
  @param[in,out]  OutputParameterBlockSize Size of the TCM output parameter block.
  @param[in]      OutputParameterBlock     Pointer to the TCM output parameter block.

  @retval EFI_SUCCESS            The command byte stream was successfully sent to the device and a response was successfully received.
  @retval EFI_DEVICE_ERROR       The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_BUFFER_TOO_SMALL   The output parameter block is too small.
**/
EFI_STATUS
EFIAPI
TcmSubmitCommand (
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN OUT UINT32        *OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  )
{
  return TcmTisTcmCommand (
           (TIS_PC_REGISTERS_PTR) (UINTN) PcdGet64 (PcdTpmBaseAddress),
           InputParameterBlock,
           InputParameterBlockSize,
           OutputParameterBlock,
           OutputParameterBlockSize
           );
}

/**
  This service requests use TCM.

  @retval EFI_SUCCESS      Get the control of TCM chip.
  @retval EFI_NOT_FOUND    TCM not found.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TisPcRequestUseTcm (
  VOID
  )
{
  return TcmTisPcRequestUseTcm ((TIS_PC_REGISTERS_PTR) (UINTN) PcdGet64 (PcdTpmBaseAddress));
}
