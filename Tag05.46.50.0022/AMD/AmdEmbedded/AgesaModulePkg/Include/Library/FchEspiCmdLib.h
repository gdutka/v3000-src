/*
*****************************************************************************
*
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _FCH_ESPI_CMD_LIB_H_
#define _FCH_ESPI_CMD_LIB_H_

#include <Uefi/UefiBaseType.h>

#define MAX_ESPI_RETRY              100000ul

//
//Master Registers
//
#define SET_CONFIGURATION           0
#define GET_CONFIGURATION           1
#define IN_BAND_RESET               2

#define IO_DECODE_RANGE0  0x0100
#define IO_DECODE_RANGE1  0x0200
#define IO_DECODE_RANGE2  0x0400
#define IO_DECODE_RANGE3  0x0800
#define MMIO_DECODE_RANGE0           0x1000
#define MMIO_DECODE_RANGE1           0x2000
#define MMIO_DECODE_RANGE2           0x4000
#define MMIO_DECODE_RANGE3           0x8000

/// eSPIx00 eSPI Software Specific Register 0
#define ESPI_DN_TXHDR_0                     0x00
typedef union {
  struct {
    UINT32      SWCommandType:3;
    UINT32      CommandStatus:1;
    UINT32      Reserved:4;
    UINT32      DnCmdHdata0:8;
    UINT32      DnCmdHdata1:8;
    UINT32      DnCmdHdata2:8;
  } Field;
  UINT32 Value;
} ESPIx00_DN_TXHDR0;

/// eSPIx04 eSPI Software Specific Register 1
#define ESPI_DN_TXHDR_1                     0x04
typedef union {
  struct {
    UINT32      DnCmdHdata3:8;
    UINT32      DnCmdHdata4:8;
    UINT32      DnCmdHdata5:8;
    UINT32      DnCmdHdata6:8;
  } Field;
  UINT32 Value;
} ESPIx04_DN_TXHDR1;

/// eSPIx68 eSPI Slave0 Configuration
#define ESPI_SLAVE0_CONFIG                  0x68
typedef union {
  struct {
    UINT32      FlashAccessChannelEnable:1;
    UINT32      OOBMessageChannelEnable:1;
    UINT32      VWChannelEnable:1;
    UINT32      PChannelEnable:1;
    UINT32      Reserved_24_4:21;
    UINT32      OperatingFreq:3;
    UINT32      IOModeSelect:2;
    UINT32      AlertMode:1;
    UINT32      CRCCheckingEnable:1;
  } Field;
  UINT32 Value;
} ESPIx68_SLAVE0_CONFIG;


/** 
///TODO:
#define ESPI_DN_TXHDR_2                     0x08
typedef union {
  struct {
    UINT32      DnCmdHdata7:8;
    UINT32      Reserved:24;
  } Field;
  UINT32 Value;
} ESPIx08_DN_TXHDR2;

#define ESPI_DN_TXDATA_PORT                0x0C
typedef union {
  struct {
    UINT32      DnTxData0:8;
    UINT32      DnTxData1:8;
    UINT32      DnTxData2:8;
    UINT32      DnTxData3:8;
  } Field;
  UINT32 Value;
} ESPIx0C_DN_TXDATA_PORT;

#define ESPI_UP_RXHDR_0                     0x10
typedef union {
  struct {
    UINT32      UpCommandType:3;
    UINT32      UpCommandStatus:1;
    UINT32      SlaveSel:2;
    UINT32      Reserved:2;
    UINT32      UpCmdHdata0:8;
    UINT32      UpCmdHdata1:8;
    UINT32      UpCmdHdata2:8;
  } Field;
  UINT32 Value;
} ESPIx10_UP_RXHDR0;

#define ESPI_UP_RXHDR_1                     0x14
typedef union {
  struct {
    UINT32      UpCmdHdata3:8;
    UINT32      UpCmdHdata4:8;
    UINT32      UpCmdHdata5:8;
    UINT32      UpCmdHdata6:8;
  } Field;
  UINT32 Value;
} ESPIx14_UP_RXHDR1;

/// eSPIx2C eSPI Master Capability
#define ESPI_MASTER_CAP                     0x2C
typedef union {
  struct {
    UINT32      FlashAccessChannelSupport:1;
    UINT32      OOBMessageChannelSupport:1;
    UINT32      VWChannelSupport:1;
    UINT32      PChannelSupport:1;
    UINT32      MasterVersion:3;
    UINT32      FlashAccessChannelMaxPayload:3;
    UINT32      OOBMessageChannelMaxPayload:3;
    UINT32      OperatingMaxVWCount:6;
    UINT32      PChannelMaxPayloadSize:3;
    UINT32      NumberOfSlave:3;
    UINT32      OperatingSupportFreq:3;
    UINT32      IOMode:2;
    UINT32      AlertMode:1;
    UINT32      CRCCheck:1;
  } Field;
  UINT32 Value;
} ESPIx2C_MASTER_CAP;

/// eSPIx30 eSPI Global Control 0
#define ESPI_GLOBAL_CTRL0                   0x30
typedef union {
  struct {
    UINT32      WdgEn:1;
    UINT32      WaitChkEn:1;
    UINT32      PrClkgatEn:1;
    UINT32      AlStopEn:1;
    UINT32      AlIdleTimer:3;
    UINT32      RgDbgclkGatingEn:1;
    UINT32      WdgCnt:16;
    UINT32      WaitCnt:6;
    UINT32      PrRstEnPltrst:1;
    UINT32      Reserved:1;
  } Field;
  UINT32 Value;
} ESPIx30_GLOBAL_CTRL0;

/// eSPIx6C eSPI Slave0 interrupt enable
#define ESPI_SLAVE0_INT_EN                  0x6C
typedef union {
  struct {
    UINT32      BusErrIntEn:1;
    UINT32      WaitTimoutIntEn:1;
    UINT32      CRCErrorIntEn:1;
    UINT32      Reserved_3:1;
    UINT32      NoRSPIntEN:1;
    UINT32      FatalErrorIntEn:1;
    UINT32      NonFatalErrorIntEn:1;
    UINT32      InvalidRSPIntEn:1;
    UINT32      InvalidCTIntEn:1;
    UINT32      UnsucssCPLIntEn:1;
    UINT32      IllegalTagIntEn:1;
    UINT32      IllegalLenIntEn:1;
    UINT32      RXOOBOverflowIntEn:1;
    UINT32      RXMsgOverflowIntEn:1;
    UINT32      RXFlashOverflowIntEn:1;
    UINT32      ProtocolErrIntEn:1;
    UINT32      Reserved_16:1;
    UINT32      Reserved_17:1;
    UINT32      MstAbortIntEn:1;
    UINT32      WdgTimeoutIntEn:1;
    UINT32      Reserved_20:1;
    UINT32      Reserved_21:1;
    UINT32      Reserved_22:1;
    UINT32      Reserved_23:1;
    UINT32      RXVWGrp0IntEn:1;
    UINT32      RXVWGrp1IntEn:1;
    UINT32      RXVWGrp2IntEn:1;
    UINT32      RXVWGrp3IntEn:1;
    UINT32      DnCmdIntEn:1;
    UINT32      RXMsgIntEn:1;
    UINT32      RXOOBIntEn:1;
    UINT32      FlashReqIntEn:1;
  } Field;
  UINT32 Value;
} ESPIX6C_SLAVE0_INT_EN;

/// eSPIx70 eSPI Slave0 Interrupt Status
#define ESPI_SLAVE0_INT_STS                  0x70
typedef union {
  struct {
    UINT32      BusErrInt:1;
    UINT32      WaitTimeoutInt:1;
    UINT32      CrcErrInt:1;
    UINT32      Reserved_3:1;
    UINT32      NoRspInt:1;
    UINT32      FatalErrInt:1;
    UINT32      NonFatalErrInt:1;
    UINT32      UnknownRspInt:1;
    UINT32      UnknownCtInt:1;
    UINT32      UnsucssCplInt:1;
    UINT32      IllegalTagInt:1;
    UINT32      IllegalLenInt:1;
    UINT32      RxOobOverflowInt:1;
    UINT32      RxMsgOverflowInt:1;
    UINT32      RxFlashOverflowInt:1;
    UINT32      ProtocolErrInt:1;
    UINT32      Reserved_17_16:2;
    UINT32      MstAbortInt:1;
    UINT32      WdgTimeoutInt:1;
    UINT32      Reserved_23_20:4;
    UINT32      RxVwGrp0Int:1;
    UINT32      RxVwGrp1Int:1;
    UINT32      RxVwGrp2Int:1;
    UINT32      RxVwGrp3Int:1;
    UINT32      DnCmdInt:1;
    UINT32      RxMsgInt:1;
    UINT32      RxOobInt:1;
    UINT32      FlashReqInt:1;
  } Field;
  UINT32 Value;
} ESPIx70_SLAVE0_INT_STS;

/// Offset 08h: General Capabilities and Configurations
typedef union {
  struct {
    UINT32      RO_PCSupported:1;
    UINT32      RO_VWSupported:1;
    UINT32      RO_OOBMsgSupported:1;
    UINT32      RO_FASupported:1;
    UINT32      Reserved_7_3:4;
    UINT32      Reserved_11_8:4;
    UINT32      RO_MaxWaitStateAllowed:4;
    UINT32      RO_MaxFreqSupported:3;
    UINT32      RO_OpenDrainAlertSupported:1;
    UINT32      OperatingFreq:3;
    UINT32      OpenDrainAlertSelect:1;
    UINT32      RO_IOModeSupported:2;
    UINT32      IOModeSelect:2;
    UINT32      AlertMode:1;
    UINT32      Reserved_29:1;
    UINT32      ResponseModifierEn:1;
    UINT32      CRCCheckingEn:1;
  } Field;
  UINT32 Value;
} ESPI_SL08_SLAVE_GENERAL_CAPCFG;

/// Offset 10h: Channel 0 Capabilities and Configurations
typedef union {
  struct {
    UINT32      PCEn:1;
    UINT32      RO_PCReady:1;
    UINT32      BusMasterEn:1;
    UINT32      Reserved_3:1;
    UINT32      RO_PCMaxPayloadSizeSupported:3;
    UINT32      Reserved_7:1;
    UINT32      PCMaxPayloadSizeSelected:3;
    UINT32      Reserved_11:1;
    UINT32      PCMaxReadRequestSize:3;
    UINT32      Reserved_31_15:17;
  } Field;
  UINT32 Value;
} ESPI_SL10_SLAVE_PC_CAPCFG;

/// Offset 20h: Channel 1 Capabilities and Configurations
typedef union {
  struct {
    UINT32      VWEn:1;
    UINT32      RO_VWReady:1;
    UINT32      Reserved_7_2:6;
    UINT32      RO_MaxVWCntSupported:6;
    UINT32      Reserved_15_14:2;
    UINT32      OpMaxVWCnt:6;
    UINT32      Reserved_31_22:10;
  } Field;
  UINT32 Value;
} ESPI_SL20_SLAVE_VW_CAPCFG;

/// Offset 30h: Channel 2 Capabilities and Configurations
typedef union {
  struct {
    UINT32      OOBEn:1;
    UINT32      RO_OOBReady:1;
    UINT32      Reserved_3_2:2;
    UINT32      RO_MsgChMaxPayloadSizeSupported:3;
    UINT32      Reserved_7:1;
    UINT32      MsgChMaxPayloadSizeSelected:3;
    UINT32      Reserved_31_11:21;
  } Field;
  UINT32 Value;
} ESPI_SL30_SLAVE_OOB_CAPCFG;
**/

//Data structures for IO decode configuartions
typedef union  {
	struct {
	UINT16 base_addr_range0;
	UINT16 base_addr_range1;
	}Field;
  UINT32 Value;  
} IO_TARGET_RANGE0;

typedef union  {
	struct {
	UINT16 base_addr_range2;
	UINT16 base_addr_range3;
	}Field;
  UINT32 Value;  
} IO_TARGET_RANGE1;

typedef union {
	struct {
		UINT32 io_range0_size:8;
		UINT32 io_range1_size:8;
		UINT32 io_range2_size:8;
		UINT32 io_range3_size:8;
	}Field;
  UINT32 Value;  
} IO_TARGET_RANGE2;

typedef union {
  struct {
    UINT32 mmio_range0_size : 16;
    UINT32 mmio_range1_size : 16;
  }Field;
  UINT32 Value;         
} MMIO_TARGET_RANGE4;

typedef union {
  struct {
    UINT32 mmio_range2_size : 16;
    UINT32 mmio_range3_size : 16;
  }Field;
  UINT32 Value;         
}MMIO_TARGET_RANGE5;

typedef struct {
	UINT32 io_mmio_dc_enable;
	IO_TARGET_RANGE0 range0;
	IO_TARGET_RANGE1 range1;
	IO_TARGET_RANGE2 range2;
	UINT32 mmio_target_range0;
	UINT32 mmio_target_range1;
	UINT32 mmio_target_range2;
	UINT32 mmio_target_range3;
	MMIO_TARGET_RANGE4 mmio_range4;
	MMIO_TARGET_RANGE5 mmio_range5;
} IO_MMIO_DECODE_CONFIG;

typedef struct {
  UINT32 addr;
  UINT32 data;
} PERIPH_MEM_RW;

typedef union {
	UINT8 data_b;
	UINT16 data_w;
	UINT32 data_l;
} IO_DATA;

typedef struct{
	UINT8 len;
	UINT16 port;
	IO_DATA data;
} PERIPH_IO_RW;

//
// eSPI Command functions
//
VOID
FchEspiCmd_InBandRst  (
  IN  UINT32     EspiBase
  );

UINT32
FchEspiCmd_GetConfiguration  (
  IN  UINT32     EspiBase,
  IN  UINT32     RegAddr
  );

VOID
FchEspiCmd_SetConfiguration  (
  IN  UINT32     EspiBase,
  IN  UINT32     RegAddr,
  IN  UINT32     Value
  );

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralIORead(
  IN  UINT32     EspiBase,
  IN OUT PERIPH_IO_RW    *Value
  );

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralIOWrite(
  IN  UINT32     EspiBase,
  IN  PERIPH_IO_RW     *Value
  );

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralMemRead(
  IN  UINT32     EspiBase,
  IN OUT PERIPH_MEM_RW    *Value
  );

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralMemWrite(
  IN  UINT32     EspiBase,
  IN  PERIPH_MEM_RW     *Value
  ); 

#endif

