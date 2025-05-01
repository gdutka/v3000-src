/** @file

;*******************************************************************************
;* Copyright (c) 2013 - 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <MctpSmbusDxe.h>

MCTP_SMBUS_CONTEXT           mSmbusContext;

UINT8
PmioRead (
  IN UINT8              Reg
)
{
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;
  UINT8                             Data;

  gBS->LocateProtocol(&gEfiPciRootBridgeIoProtocolGuid, NULL, &IoDev);

//[-start-220126-IB14740207-modify]//
  IoDev->Mem.Read (IoDev, EfiPciWidthUint8, ACPI_MMIO_BASE + PMIO_BASE + Reg, 1, &Data);
//[-end-220126-IB14740207-modify]//
  return Data;
}

VOID
PmioWrite (
  IN  UINT8                   Reg,
  IN  UINT8                   Data
)
{
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;

  gBS->LocateProtocol(&gEfiPciRootBridgeIoProtocolGuid, NULL, &IoDev);

//[-start-220126-IB14740207-modify]//
  IoDev->Mem.Write (IoDev, EfiPciWidthUint8, ACPI_MMIO_BASE + PMIO_BASE + Reg, 1, &Data);
//[-end-220126-IB14740207-modify]//
}

VOID
SetAsfMasterEnable (
  IN VOID
)
{
  UINT8     Data;
  Data = PmioRead (0x02);   // PMx00 DecodeEn
  Data |= 0x01;             // AsfSmMasterEn
  PmioWrite (0x02, Data);
}

VOID
ClearAsfMasterEnable (
  IN VOID
)
{
  UINT8     Data;
  Data = PmioRead (0x02);   // PMx00 DecodeEn
  Data &= 0xFE;             // AsfSmMasterEn
  PmioWrite (0x02, Data);
}
UINT8
SmbusIoRead (
  IN MCTP_SMBUS_CONTEXT *This,
  IN UINT8              RegIndex
)
/*++

Routine Description:
   Read register value from SMBUS controller

Arguments:
  This                    - pointer to MCTP_SMBUS_CONTEXT structure
  RegIndex                - the register index of the register to be read

Returns:
  The register value of the corresponding register index

--*/

{
  return This->DashIoCfg->AsfReadReg(This->DashIoCfg, RegIndex);

}


/**
 Read register value from SMBUS controller

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure
 @param [in]   RegIndex      The register index of the register to be written
 @param [in]   Data            Data to be written to the register

 @retval none

**/
VOID
SmbusIoWrite (
  IN MCTP_SMBUS_CONTEXT  *This,
  IN UINT8               RegIndex,
  IN UINT8               Data
)
{
  This->DashIoCfg->AsfWriteReg (This->DashIoCfg, RegIndex, Data);
}


/**
 Set listen address to SMBUS controller

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure
 @param [in]   ListenAddr     The slave address which ASF slave responds in listen mode

 @retval none

**/
VOID
SetListenAddress (
  MCTP_SMBUS_PROTOCOL *This,
  UINT8 ListenAddr
)
{
  MCTP_SMBUS_CONTEXT *Context;

  Context = MCTP_SMBUS_CONTEXT_FROM_THIS (This);
  SmbusIoWrite (Context, LISTEN_ADDR, ListenAddr);
}


/**
 Enable listening for SMBUS Slave

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure

 @retval none

**/
VOID
EnableListening (
  MCTP_SMBUS_CONTEXT *This
)
{
  SmbusIoWrite (This, LISTEN_ADDR, SmbusIoRead (This, LISTEN_ADDR) | 0x01);
}

/**
 Disable listening for SMBUS Slave

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure

 @retval none

**/
VOID
DisableListening(
  MCTP_SMBUS_CONTEXT *This
)
{
  SmbusIoWrite (This, LISTEN_ADDR, SmbusIoRead (This, LISTEN_ADDR) & 0xFE);
}

VOID
SetSuspendSlave (
  MCTP_SMBUS_CONTEXT *This
)
{
  SmbusIoWrite (This, SLAVE_EN, SmbusIoRead (This, SLAVE_EN) | SUSPEND_SLAVE);
}

VOID
ClearSuspendSlave (
  MCTP_SMBUS_CONTEXT *This
)
{
  SmbusIoWrite (This, SLAVE_EN, SmbusIoRead (This, SLAVE_EN) & ~(UINT8)SUSPEND_SLAVE);
}
/**
 Set host semaphore for current SMBUS controller

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure

 @retval EFI_SUCCESS         Lock owned

**/
EFI_STATUS
AcquireLock (
  IN  MCTP_SMBUS_CONTEXT       *Context
)
{
  UINTN                   StallCount;
  UINTN                   Index;

  StallCount = 1*STALL_TIME / Context->StallTime;

  for (Index = 0; Index < StallCount; ++Index) {
    if ((SmbusIoRead(Context, SEMAPHORE) & EC_SEMAPHORE) != 0x00) {
      gBS->Stall (Context->StallTime);
    } else {
      break;
    }
  }
  if (Index == StallCount) {
    return EFI_TIMEOUT;
  } else {
    SmbusIoWrite(Context, SEMAPHORE, HOST_SEMAPHORE);
    return EFI_SUCCESS;
  }
}

/**
 Clear host semaphore for current SMBUS controller

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure

 @retval EFI_SUCCESS         Lock unowned

**/
EFI_STATUS
ReleaseLock (
  IN  MCTP_SMBUS_CONTEXT       *Context
)
{
  SmbusIoWrite(Context, SEMAPHORE, CLR_HOST_SEMAPHORE);
  return EFI_SUCCESS;
}

/**
 Check SMBUS controller data bank status

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure

 @retval TRUE                    Data bank 0 or 1 is full

**/
BOOLEAN
MctpSmbusBufferFull (
  IN  MCTP_SMBUS_PROTOCOL        *This
)
{
  UINT8              DataBankSelect;
  MCTP_SMBUS_CONTEXT *Context;

  Context = MCTP_SMBUS_CONTEXT_FROM_THIS (This);
  DataBankSelect = SmbusIoRead (Context, DATABANK_SELECT);

  if ( ((DataBankSelect & DATABANK0_FULL) == DATABANK0_FULL) ||
       ((DataBankSelect & DATABANK1_FULL) == DATABANK1_FULL)) {
    return TRUE;
  } else {
    return FALSE;
  }
}



/**
 Wait for buffer full indication of SMBUS Slave

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure

 @retval EFI_SUCCESS         command complete successfully
 @retval other                    command failed

**/
EFI_STATUS
WaitForSmbusBuffer (
  IN MCTP_SMBUS_CONTEXT   *This
)
{
  EFI_STATUS              Status;
  UINTN                   StallCount;
  UINTN                   Index;
  UINT8                   DataBankSelect;

  Status = EFI_TIMEOUT;

  EnableListening(This);

  StallCount = 5*STALL_TIME / This->StallTime;

  for (Index = 0; Index < StallCount; ++Index) {
    DataBankSelect = SmbusIoRead (This, DATABANK_SELECT);

    if ((DataBankSelect & DATABANK0_FULL) == DATABANK0_FULL) {
      SmbusIoWrite (This, DATABANK_SELECT, SET_READ_DATABANK0);
      Status = EFI_SUCCESS;
      break;

    } else if ((DataBankSelect & DATABANK1_FULL) == DATABANK1_FULL){
      SmbusIoWrite (This, DATABANK_SELECT, SET_READ_DATABANK1);
      Status = EFI_SUCCESS;
      break;
    }
    gBS->Stall (This->StallTime);
  }

  DisableListening(This);

  return Status;
}


/**
 Send block data to management controller for SMBUS master

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure
 @param [in]   SmbusAdder  SMBUS address of the data to be sent to
 @param [in]   Buffer           Block data to be sent
 @param [in]   BufferSize      Block data size

 @retval EFI_SUCCESS         command complete successfully
 @retval other                    command failed

**/
EFI_STATUS
MctpSmbusMasterSend (
  IN  MCTP_SMBUS_PROTOCOL *This,
  IN  UINT8               SmbusAddr,
  IN  VOID                *Buffer,
  IN  UINTN               BufferSize
)
{
  EFI_STATUS              Status;
  MCTP_SMBUS_CONTEXT      *Context;
  UINTN                   Index;
  BOOLEAN                 TimeoutFlag;
  UINTN                   StallCount;
  UINT8                   StatusReg;

  Context = MCTP_SMBUS_CONTEXT_FROM_THIS (This);

  Status = AcquireLock(Context);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  SetSuspendSlave (Context);
  SetAsfMasterEnable();

  SmbusIoRead (Context, HCTL );
  SmbusIoWrite (Context, HSTS, 0xff); //(HBSY + INTR + DERR + BERR + FAIL));
  SmbusIoWrite (Context, TSA, SmbusAddr);
  SmbusIoWrite (Context, HCMD, MCTP_SMBUS_COMMAND);

  SmbusIoWrite (Context, HD0, (UINT8)BufferSize);

  StatusReg = 0;
  for (Index = 0; Index < 5000; ++Index) {
    StatusReg = SmbusIoRead (Context, HSTS);
    if ((StatusReg & (INTR + DERR + BERR + FAIL + BYTE_DONE_STS)) == 0 ) {
      break;
    }
    gBS->Stall (Context->StallTime);
  }
  for (Index = 0; Index < BufferSize; Index++) {
    SmbusIoWrite (Context, HBD, ((UINT8 *)Buffer)[Index]);
  }
  SmbusIoWrite (Context, HCTL, SMBUS_OP_BLOCK | PEC_ENABLE | PEC_AUTO_APPEND | START);
  

  TimeoutFlag = TRUE;
  StallCount = 5*STALL_TIME / Context->StallTime;

  StatusReg = 0;
  for (Index = 0; Index < StallCount; ++Index) {
    StatusReg = SmbusIoRead (Context, HSTS);
    if (StatusReg & (INTR | BYTE_DONE_STS | DERR | BERR | FAIL)) {
      TimeoutFlag = FALSE;
      break;
    }
    gBS->Stall (Context->StallTime);
  }

  if (TimeoutFlag) {
    Status = EFI_TIMEOUT;
  } else if ((StatusReg & (HBSY + DERR + BERR + FAIL)) != 0 ) {
    Status = EFI_DEVICE_ERROR;
  }  else {
    Status = EFI_SUCCESS;
  }
  SmbusIoWrite (Context, HSTS, 0xff);//(HBSY + INTR + DERR + BERR + FAIL));

  ClearAsfMasterEnable();
  ClearSuspendSlave (Context);
  ReleaseLock(Context);

  return Status;
}


/**
 Receive block data from mangement controller

 @param [in]   This             Pointer to MCTP_SMBUS_CONTEXT structure
 @param [in]   Buffer           Block data to be received
 @param [in]   BufferSize      Block data size

 @retval EFI_SUCCESS         command complete successfully
 @retval other                    command failed

**/
EFI_STATUS
MctpSmbusSlaveReceive (
  IN  MCTP_SMBUS_PROTOCOL *This,
  OUT VOID                      *Buffer,
  IN OUT UINTN                  *BufferSize
)
{
  EFI_STATUS            Status;
  MCTP_SMBUS_CONTEXT    *Context;
  UINT8                 SmbAddress;
  UINT8                 SmbCommand;
  UINT8                 Len;
  UINT8                 Index;


  Context = MCTP_SMBUS_CONTEXT_FROM_THIS (This);

  Status = AcquireLock(Context);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = WaitForSmbusBuffer (Context);

  if (!EFI_ERROR(Status)) {
    SmbusIoWrite (Context, SET_DATA_READ_POINTER, 0);
    SmbAddress = SmbusIoRead (Context, HBD);
    SmbCommand = SmbusIoRead (Context, HBD);
    Len = SmbusIoRead (Context, HBD);

    if ((SmbAddress != Context->DashIoCfg->GetHostSmbusSlaveAddr()) ||      //same with below?
         (SmbCommand != 0x0f) ) {
      return EFI_PROTOCOL_ERROR;
    }

    if ((SmbAddress != (SmbusIoRead (Context, LISTEN_ADDR)&0xfe)) || (SmbCommand != 0x0F)) {
      Status = EFI_PROTOCOL_ERROR;
    } else if (Len > (UINT8)(*BufferSize)) {
      *BufferSize = Len;
      Status = EFI_BUFFER_TOO_SMALL;
    } else {
      *BufferSize = Len;
      for (Index = 0; Index < Len; Index++) {
        ((UINT8 *)Buffer)[Index] = SmbusIoRead(Context, HBD);
      }
      SmbusIoWrite (Context, DATABANK_SELECT, DATABANK0_FULL | DATABANK1_FULL);
      Status = EFI_SUCCESS;
    }
  }

  ReleaseLock(Context);
  return Status;
}

/**
  The user Entry Point for module MctpSmbusDxe.  The user code starts with this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
MctpSmbusDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                   Status;
  EFI_METRONOME_ARCH_PROTOCOL  *Metronome;
  DASH_IO_CFG_PROTOCOL         *DashIoCfg;


  Status = gBS->LocateProtocol (
                  &gDashIoCfgProtocolGuid,
                  NULL,
                  &DashIoCfg
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiMetronomeArchProtocolGuid,
                  NULL,
                  &Metronome
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  mSmbusContext.Signature                   = MCTP_SMBUS_SIGNATURE;
  mSmbusContext.DashIoCfg                   = DashIoCfg;
//  mSmbusContext.StallTime                   = Metronome->TickPeriod / 10; // TickPeriod = 1*100 ns units and StallTime will be zero and cause divided by zero error
  mSmbusContext.StallTime                   = Metronome->TickPeriod;  // 1
  mSmbusContext.EnableListening             = EnableListening;
  mSmbusContext.DisableListening            = DisableListening;
  mSmbusContext.MctpSmbus.BufferFull        = MctpSmbusBufferFull;
  mSmbusContext.MctpSmbus.MasterSend        = MctpSmbusMasterSend;
  mSmbusContext.MctpSmbus.SlaveReceive      = MctpSmbusSlaveReceive;
  mSmbusContext.MctpSmbus.SetListenAddress  = SetListenAddress;

  //
  // Protocol interface installation
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gMctpSmbusProtocolGuid,
                  &mSmbusContext.MctpSmbus,
                  NULL
                  );

  return Status;
}

