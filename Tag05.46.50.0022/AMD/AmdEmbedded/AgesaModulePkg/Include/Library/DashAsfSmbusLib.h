/*****************************************************************************
 * Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#ifndef _DASH_ASF_SMBUS_LIB_H_
#define _DASH_ASF_SMBUS_LIB_H_

// **** ASF (Alert Standard Format) Registers Definition ****

#define ASF_HOST_STATUS         0x00  //  bit7   LastByte
                                      // bit4    PECError
                                      // bit3    BusCollision
                                      // bit2    DevError
                                      // bit1    Intr
                                      // bit0    HostBusy
#define ASF_HOST_CONTROL        0x02  // bit7    PECEnable
                                      // bit6    Start
                                      // bit5    PECAppend
                                      // bit4:2  Protocol
                                      // bit1    KillHost
#define ASF_HOST_COMMAND        0x03  // bit7:0  HostCommand
#define ASF_SLAVE_ADDRESS       0x04  // bit7:1  Address
                                      // bit0    RW (0 = Write, 1 = Read)
#define ASF_DATA0               0x05
#define ASF_DATA1               0x06
#define ASF_DATA_INDEX          0x07
#define ASF_PEC                 0x08
#define ASF_LISTEN_ADR          0x09  // bit7:1  ListenAdr
                                      // bit0    ListenAdrEn
#define ASF_STATUS              0x0A  // bit7    SlaveBusy
                                      // bit6    SlaveIntr
                                      // bit3    RemotePowerCycle
                                      // bit2    RemotePowerUp
                                      // bit1    RemotePowerDown
                                      // bit0    RemoteReset
#define ASF_STATUS_MASK0        0x0B  // bit5    FanSpeed0StatusEnable
                                      // bit0    Temp0StatusEnable
#define ASF_STATUS_MASK1        0x0C
#define ASF_SLAVE_STATUS        0x0D  // bit3    WrongSP
                                      // bit2    SlaveDevError
                                      // bit1    SlaveBusCollision
                                      // bit0    SlavePECError
#define ASF_REMOTECTRL_ADR      0x0E
#define ASF_SENSOR_ADR          0x0F
#define ASF_DATAREAD_POINTER    0x10
#define ASF_DATAWRITE_POINTER   0x11
#define ASF_SETDATAREAD_POINTER 0x12
#define ASF_DATABANKSEL         0x13  // bit7    SetReadHostDataBank
                                      // bit5:4  SetReadRevDataBank
                                      // bit3    Databank1Full
                                      // bit2    Databank0Full
                                      // bit1    DataBank[1]
                                      // bit0    DataBank[0]
#define ASF_SEMAPHORE           0x14  // bit3    ClrEcSemaphore
                                      // bit2    EcSemaphore
                                      // bit1    ClrHostSemaphore
                                      // bit0    HostSemaphore
#define ASF_SLAVE_EN            0x15  // bit7    FairArbEn
                                      // bit6    TmrOutEn
                                      // bit5    LegacySensorEn
                                      // bit4    KillSlave
                                      // bit3    SuspendSlave
                                      // bit1    SlaveIntrListenEn
#define ASF_DELAYMASTER_TIMER   0x16  // bit7:0  FairArbTimer

#define DASH_WAIT_SMBUS_SLAVE_BUFFER_STALL  200    // Delay time for SMBUS slave buffer ready(ms).
#define DASH_SMBUS_IOBASE            0xB00
#define DASH_SMBUS_ASF_IOBASE        0xB20

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *----------------------------------------------------------------------------
 */

EFI_STATUS
DashWaitForSmbusBuf(VOID);

VOID
DashResetAsfSlave(VOID);

UINT8
DashSmbusIoRead (
  IN UINT8 Offset
  );

VOID
DashSmbusIoWrite (
  IN UINT8              Offset,
  IN UINT8              Data
  );

EFI_STATUS
EFIAPI
DashListenModeRead (
  IN UINT8 *BufferSize,
  IN UINT8 *MctpBuffer
);

VOID
DashEnableSmbusListening(VOID);

#endif // _DASH_ASF_SMBUS_LIB_H_

