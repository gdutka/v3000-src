/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTESPI_KTESPILIB_FILECODE

#define MAX_ESPI_RETRY     100000          // 100ms

#define SET_CONFIGURATION  0
#define GET_CONFIGURATION  1
#define IN_BAND_RESET      2
#define PC_MSG_DOWN_STREAM 4
#define VM_DOWN_STREAM     5
#define OOB_DOWN_STREAM    6

#define SLAVE_REG_ID           0x04
#define SLAVE_GENERAL_CAPCFG   0x08
#define SLAVE_PC_CAPCFG        0x10
#define SLAVE_VW_CAPCFG        0x20
#define SLAVE_OOB_CAPCFG       0x30
#define SLAVE_FA_CAPCFG        0x40
#define SLAVE_MEC1701_SCRATCH_REG 0x80      // This is a scratch register of MEC1701 eSPI and does nothing.

//ESPIx00
#define DNCMD_STATUS       BIT3

//ESPIx2C Master Capability
#define MASTER_FA_SUPPORT                   BIT0
#define MASTER_OOB_SUPPORT                  BIT1
#define MASTER_VW_SUPPORT                   BIT2
#define MASTER_PERIPHERAL_SUPPORT           BIT3

//ESPIx68  Slave0 Configuration
#define SLAVE_FA_ENABLE                     BIT0
#define SLAVE_OOB_ENABLE                    BIT1
#define SLAVE_VW_ENABLE                     BIT2
#define SLAVE_PC_ENABLE             BIT3

//SLAVE offset 0x08   SLAVE_GENERAL_CAPCFG
#define SLAVE_FA_SUPPORT                    BIT3
#define SLAVE_OOB_SUPPORT                   BIT2
#define SLAVE_VW_SUPPORT                    BIT1
#define SLAVE_PERIPHERAL_SUPPORT            BIT0

/// eSPIx00 eSPI Software Specific Register 0
typedef union {
  struct {
    UINT32      SWCommandType:3;
    UINT32      CommandStatus:1;
    UINT32      SlaveNSelect:2;
    UINT32      Reserved_7_6:2;
    UINT32      AddrByte0PCycTypeVWCntOOBCycType:8;
    UINT32      AddrByte0VWIdxOOBPktByte1:8;
    UINT32      VWDataOOBPktByte2:8;
  } Field;
  UINT32 Value;
} ESPIx00_DN_DXHDR0;

/// eSPIx04 eSPI Software Specific Register 1
typedef union {
  struct {
    UINT32      VWIdxOOBPktbyte3:8;
    UINT32      VWDataOOBByteCnt:8;
    UINT32      VWIdxOOBPktByte5:8;
    UINT32      VWDataOOBPktByte6:8;
  } Field;
  UINT32 Value;
} ESPIx04_DN_DXHDR1;

/// eSPIx2C eSPI Master Capability
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

/// eSPIx68 eSPI Slave N Configuration
typedef union {
  struct {
    UINT32      FlashAccessChannelEnable:1;
    UINT32      OOBMessageChannelEnable:1;
    UINT32      VWChannelEnable:1;
    UINT32      PChannelEnable:1;
    UINT32      OOBValidBitEnable:1;
    UINT32      Reserved_24_5:20;
    UINT32      OperatingFreq:3;
    UINT32      IOModeSelect:2;
    UINT32      AlertMode:1;
    UINT32      CRCCheckingEnable:1;
  } Field;
  UINT32 Value;
} ESPIx68_SLAVE0_CONFIG;

///
/// Slave Registers
///
/// Offset 04h: Device Identification
typedef union {
  struct {
    UINT32      RO_VersionID:8;
    UINT32      Reserved_31_8:24;
  } Field;
  UINT32 Value;
} ESPI_SL04_DEVICE_ID;

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

/// Offset 40h: Channel 2 Capabilities and Configurations
typedef union {
  struct {
    UINT32      FAEn:1;
    UINT32      RO_FAReady:1;
    UINT32      FlashBlockEraseSize:3;
    UINT32      RO_ChMaxPayloadSizeSupported:3;
    UINT32      ChMaxPayloadSizeSelected:3;
    UINT32      RO_FlashSharingMode:1;
    UINT32      ChMaxReadReqSize:3;
    UINT32      Reserved_31_15:17;
  } Field;
  UINT32 Value;
} ESPI_SL40_SLAVE_FA_CAPCFG;


UINT32 GetEspiBase ()
{
  UINT32 base = 0xFEC20000;
  //ReadPci ((LPC_BUS_DEV_FUN << 16) + 0xA0, AccessWidth32, &base, NULL);
  //base &= 0xFFFFFFC0;
  //base += 0x00010000;

  // eSPI Master Capability
  if ( ACPIMMIO32 (base + 0x2C) == 0 || ACPIMMIO32 (base + 0x2C) == 0xFFFFFFFF) {
    ASSERT (FALSE);
  }

  return base;
}
/*
wait4stClear ()
{
    while(eSPIx00.Field.CommandStatus) {
        IoWrite16(0x80, 0xE291); // delay
        IoRead8(0x404);          // delay
    }
}
*/
VOID
wait4stClear (
  IN  UINT32     EspiBase
  )
{
  UINT32 Retry;

  for ( Retry = 0; Retry < MAX_ESPI_RETRY; Retry++ ) {
    if ( (ACPIMMIO32 (EspiBase + 0x00) & DNCMD_STATUS) == 0 ) {
      break;
    }
    FchStall (2, NULL); // delay
  }
}
/*
eSPI_InBandRst (UINT32 slave)
{
    eSPIx00.Field.CommandStatus = 0;
    wait4stClear();
    eSPIx00.Field.SlaveNSelect = slave;
    eSPIx68.Field.OperatingFreq = 0;
    eSPIx00.Field.SWCommandType = IN_BAND_RESET;
    eSPIx00.Field.CommandStatus = 1;
    wait4stClear();
}
*/
VOID
KTeSPI_InBandRst  (
  IN  UINT32     EspiBase
  )
{
  //RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, 0x00);
  wait4stClear (EspiBase);
  //RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32) (BIT4 + BIT5), 0x00);
  RwMem (EspiBase + 0x68, AccessWidth32, 0xF1FFFFFF, 0x00);              // [27:25] CLK_FREQ_SEL
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFFFFFFF8, IN_BAND_RESET);     // [2:0] DNCMD_TYPE
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, BIT3);
  wait4stClear (EspiBase);
}

/*
UINT32 eSPI_GetConfiguration(UINT32 slave, UINT16 offset)
{
    wait4stClear();
    eSPIx00.Field.SlaveNSelect = slave;
    eSPIx00.Field.SWCommandType = GET_CONFIGURATION;
    eSPIx00.Field.AddrByte0PCycTypeVWCntOOBCycType = ((offset >> 8) & 0xFF);
    eSPIx00.Field.AddrByte0VWIdxOOBPktByte1 = (offset & 0xFF);
    eSPIx04.Value = 0;
    eSPIx00.Field.CommandStatus = 1;
    wait4stClear();
    return eSPIx04.Value;
}
*/
UINT32
KTeSPI_GetConfiguration  (
  IN  UINT32     EspiBase,
  IN  UINT32     RegAddr
  )
{
  wait4stClear (EspiBase);
  //RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32) (BIT4 + BIT5), 0x00);
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFFFFFFF8, GET_CONFIGURATION);     // [2:0] DNCMD_TYPE
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFF00FFFF, (RegAddr & 0xFF) << 16);
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFFFF00FF, RegAddr & 0xFF00);
  RwMem (EspiBase + 0x04, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, BIT3);
  wait4stClear (EspiBase);
  return ACPIMMIO32 (EspiBase + 0x04);
}

/*
eSPI_SetConfiguration (UINT32 slave, UINT32 offset, UINT32 value)
{
    wait4stClear();
    eSPIx00.Field.SlaveNSelect = slave;
    eSPIx00.Field.SWCommandType = SET_CONFIGURATION;
    eSPIx00.Field.AddrByte0PCycTypeVWCntOOBCycType = ((offset >> 8) & 0xFF);
    eSPIx00.Field.AddrByte0VWIdxOOBPktByte1 = (offset & 0xFF);
    eSPIx04.Value = value;
    eSPIx00.Field.CommandStatus = 1;
    wait4stClear();
}
*/
VOID
KTeSPI_SetConfiguration  (
  IN  UINT32     EspiBase,
  IN  UINT32     RegAddr,
  IN  UINT32     Value
  )
{
  wait4stClear (EspiBase);
  //RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32) (BIT4 + BIT5), 0x00);
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFFFFFFF8, SET_CONFIGURATION);     // [2:0] DNCMD_TYPE
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFF00FFFF, (RegAddr & 0xFF) << 16);
  RwMem (EspiBase + 0x00, AccessWidth32, 0xFFFF00FF, RegAddr & 0xFF00);
  RwMem (EspiBase + 0x04, AccessWidth32, 0x00, Value);
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, BIT3);
  wait4stClear (EspiBase);
}

/*----------------------------------------------------------------------------------------*/
/**
 * KTeSPI_PltResetAssert - Assert PltReset#
 *
 *  STEP 1: Poll DN_TXHDR_0th.DNCMD_STATUS=0
 *  STEP 2: Program DN_TXHDR_0th=0x005
 *  STEP 3: Program DN_TXHDR_1=0x0
 *          Not mentioned in the sequence as default is 0(but better program to 0 in case there was programming before)
 *  STEP 4: Program DN_TXHDR_2=0x0
 *          Not mentioned in the sequence as default is 0(but better program to 0 in case there was programming before)
 *  STEP 5: Program DN_TXDATA_PORT=0x2003             // Assert PLTRST#
 *  STEP 6: Program DN_TXHDR_0=0x00D                  // Set Status bit and wait for HW to clear it.
 *
 *
 *  @param[in] EspiBase      ESPI MMIO base.
 */
VOID
KTeSPI_PltResetAssert (
  IN  UINT32     EspiBase
  )
{
  wait4stClear (EspiBase);
  RwMem (EspiBase + 0x00, AccessWidth32, 0x00, 0x05);
  RwMem (EspiBase + 0x04, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x08, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x0C, AccessWidth32, 0x00, 0x2003);
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, BIT3);
  wait4stClear (EspiBase);
}

/*----------------------------------------------------------------------------------------*/
/**
 * KTeSPI_PltResetDeassert - De-assert PltReset#
 *
 *  STEP 1: Poll DN_TXHDR_0th.DNCMD_STATUS=0
 *  STEP 2: Program DN_TXHDR_0th=0x005
 *  STEP 3: Program DN_TXHDR_1=0x0
 *          Not mentioned in the sequence as default is 0
 *          (but better program to 0 in case there was programming before)
 *  STEP 4: Program DN_TXHDR_2=0x0
 *          Not mentioned in the sequence as default is 0
 *          (but better program to 0 in case there was programming before)
 *  STEP 5: Program DN_TXDATA_PORT=0x2203             // De-assert PLTRST#
 *  STEP 6: Program DN_TXHDR_0=0x00D                  // Set Status bit and wait for HW to clear it.
 *
 *
 *  @param[in] EspiBase      ESPI MMIO base.
 */
VOID
KTeSPI_PltResetDeassert (
  IN  UINT32     EspiBase
  )
{
  wait4stClear (EspiBase);
  RwMem (EspiBase + 0x00, AccessWidth32, 0x00, 0x05);
  RwMem (EspiBase + 0x04, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x08, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x0C, AccessWidth32, 0x00, 0x2203);
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, BIT3);
  wait4stClear (EspiBase);
}

/**
 * FchinitKTEspiSlave0Device - Config ESPI controller and device
 *
 *  // It recommends software to do device initialization sequence as below:
 *  // 1.Set PAD to PU (done in previous steps)
 *  // 2.Set Master SlaveN Configuration register (MMIO_reg0x68)
 *  Clock Frequency to 16.7Mhz, then send In-Band Reset.
 *  // 3.Check Slave Alert Mode of platform setting
 *  // 4. Get_Configuration to read Slave Device Capability
 *  Register.
 *  // 5. Set_Configuration to write Slave Device General
 *  Configuration Register, such as CRC, IO mode, Alert mode,
 *  Clock Frequency.
 *  // 6. Change Master SlaveN Configuration registers
 *  (MMIO_reg0x68), make sure Master SlaveN's configuration is
 *  same as device.
 *  // 7. Set Alert enable bit of eSPI Global Control Register1
 *  (eSPI_Global_Control_Reg_1_0x34[20]) to 1b
 *  // 8. Get_Configuration to read Slave Device's each channel
 *  Capablity Register.
 *  // 9. Set_Configuration to write Slave Device's each channel
 *  Configuration Register.
 *  // 10. Get_Configuration to read Slave Device's each channel
 *  ready bit, wait for each channel ready.
 *  // 11. Enable Master SlaveN Configuration registers
 *  (MMIO_reg0x68) to enable each channel's enable bit.
 *
 * @param[in] EspiBase ESPI MMIO base.
 *
 */
VOID
KTeSPI_SendVwCmd (
  IN  UINT32     EspiBase,
  IN  UINT16     cmd
  )
{
  wait4stClear (EspiBase);
  RwMem (EspiBase + 0x00, AccessWidth32, 0x00, 0x05);
  RwMem (EspiBase + 0x04, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x08, AccessWidth32, 0x00, 0x00);
  RwMem (EspiBase + 0x0C, AccessWidth32, 0x00, cmd);
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)BIT3, BIT3);
  wait4stClear (EspiBase);
}

VOID
KTeSPI_NoticeSlaveBeforeUserReset (
  )
{
  UINT32     EspiBase;
  EspiBase = GetEspiBase ();
  KTeSPI_SendVwCmd (EspiBase, 0x1107);   // Assert HostRstWarn
  ACPIMMIO32 (EspiBase + 0xAC) |= BIT25; //HOST_RST_WARN_DIS 0xAC[25] to 1 // Disable HW method for HostRstWarn
  FchStall (32, NULL);                   //delay 32us
  KTeSPI_SendVwCmd (EspiBase, 0x2003);   //0x2003 // Assert PLTRST#
  FchStall (32, NULL);                   //delay 32us
}
/**
 * FchinitKTEspiSlave0Device - Config ESPI controller and device
 *
 *  // It recommends software to do device initialization sequence as below:
 *  // 1.Set PAD to PU (done in previous steps)
 *  // 2.Set Master SlaveN Configuration register (MMIO_reg0x68)
 *  Clock Frequency to 16.7Mhz, then send In-Band Reset.
 *  // 3.Check Slave Alert Mode of platform setting
 *  // 4. Get_Configuration to read Slave Device Capability
 *  Register.
 *  // 5. Set_Configuration to write Slave Device General
 *  Configuration Register, such as CRC, IO mode, Alert mode,
 *  Clock Frequency.
 *  // 6. Change Master SlaveN Configuration registers
 *  (MMIO_reg0x68), make sure Master SlaveN's configuration is
 *  same as device.
 *  // 7. Set Alert enable bit of eSPI Global Control Register1
 *  (eSPI_Global_Control_Reg_1_0x34[20]) to 1b
 *  // 8. Get_Configuration to read Slave Device's each channel
 *  Capablity Register.
 *  // 9. Set_Configuration to write Slave Device's each channel
 *  Configuration Register.
 *  // 10. Get_Configuration to read Slave Device's each channel
 *  ready bit, wait for each channel ready.
 *  // 11. Enable Master SlaveN Configuration registers
 *  (MMIO_reg0x68) to enable each channel's enable bit.
 *
 * @param[in] EspiBase ESPI MMIO base.
 *
 */
VOID
FchinitKTEspiSlave0Device (
  IN  VOID       *FchDataPtr,
  IN  UINT32     EspiBase
  )
{
  ESPIx2C_MASTER_CAP              MasterCapibility;
  ESPIx68_SLAVE0_CONFIG           Slave0Config;
  ESPI_SL08_SLAVE_GENERAL_CAPCFG  DeviceCapCfg;
  ESPI_SL10_SLAVE_PC_CAPCFG       SlavePCCapCfg;
  ESPI_SL20_SLAVE_VW_CAPCFG       SlaveVWCapCfg;
  ESPI_SL30_SLAVE_OOB_CAPCFG      SlaveOOBCapCfg;
  ESPI_SL40_SLAVE_FA_CAPCFG       SlaveFACapCfg;
  FCH_RESET_DATA_BLOCK            *LocalCfgPtr;
  UINT8                           AlertMode;

  IDS_HDT_CONSOLE (FCH_TRACE, " %a Entry\n",__FUNCTION__);

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  AlertMode   = LocalCfgPtr->Espi.AlertMode;
  //
  // 1, In-Band Reset
  //
  KTeSPI_InBandRst (EspiBase);

  //
  // Check Slave Alert Mode of platform setting
  //
  if (AlertMode == 1) {     //Dedicated Alert pin connection
    ACPIMMIO32 (EspiBase + 0x68) |= BIT30; //force Alert mode to fix get configuration failure on port 0
  }

  //
  // 4 - 6, check and update Slave Device Capability and Slave0 configuration
  //
  MasterCapibility.Value = ACPIMMIO32 (EspiBase + 0x2C);
  DeviceCapCfg.Value     = KTeSPI_GetConfiguration (EspiBase, SLAVE_GENERAL_CAPCFG);
  Slave0Config.Value     = 0;
  //
  // Check support for each channel
  //
  if ( MasterCapibility.Field.FlashAccessChannelSupport && DeviceCapCfg.Field.RO_FASupported ) {
    Slave0Config.Field.FlashAccessChannelEnable = 1;
  }

  if ( MasterCapibility.Field.OOBMessageChannelSupport && DeviceCapCfg.Field.RO_OOBMsgSupported ) {
    Slave0Config.Field.OOBMessageChannelEnable = 1;
  }

  if ( MasterCapibility.Field.VWChannelSupport && DeviceCapCfg.Field.RO_VWSupported ) {
    Slave0Config.Field.VWChannelEnable = 1;
  }

  if ( MasterCapibility.Field.PChannelSupport && DeviceCapCfg.Field.RO_PCSupported ) {
    Slave0Config.Field.PChannelEnable = 1;
  }

  //Operating Support Frequency in FCH eSPI Registers
  //000b 16.7 MHz
  //001b 16.7 MHz, 33 MHz
  //011b 16.7 MHz, 33 MHz, 66 MHz

  //Operating Frequency in FCH eSPI Registers
  //000b 16.6 MHz.
  //001b 33 MHz.
  //010b 66 MHz.
  //111b-011b Reserved

  //General Capabilities and Configurations in eSPI Spec v1.0
  //Bits Frequency
  //000b 20 MHz
  //001b 25 MHz
  //010b 33 MHz
  //011b 50 MHz
  //100b 66 MHz
  //Others Reserved
  if ((MasterCapibility.Field.OperatingSupportFreq == 0x03)
    && (DeviceCapCfg.Field.RO_MaxFreqSupported == 0x04)
    && (LocalCfgPtr->Espi.OperatingFreq >= 0x02))
  {
    //66MHz
    Slave0Config.Field.OperatingFreq = 0x02;
    DeviceCapCfg.Field.OperatingFreq = 0x04;
  } else if ((MasterCapibility.Field.OperatingSupportFreq >= 0x01)
    && (DeviceCapCfg.Field.RO_MaxFreqSupported >= 0x02)
    && (LocalCfgPtr->Espi.OperatingFreq >= 0x01))
  {
    //33MHz
    Slave0Config.Field.OperatingFreq = 0x01;
    DeviceCapCfg.Field.OperatingFreq = 0x02;
  } else {
    //16.7MHz
    Slave0Config.Field.OperatingFreq = 0x00;
    DeviceCapCfg.Field.OperatingFreq = 0x00;
  }

  //
  // I/O Mode Select:
  //
  //
  //Operating Support Mode in FCH eSPI Registers
  //00b Single IO
  //01b Dual IO, Single IO
  //10b Quad IO, Dual IO, Single IO
  //11b Reserved

  //I/O Mode Support in eSPI Spec v1.0
  //00b Single I/O
  //01b Single and Dual I/O
  //10b Single and Quad I/O
  //11b Single, Dual and Quad I/O

  //I/O Mode Select in eSPI v1.0 and in FCH eSPI Registers
  //00b Single I/O
  //01b Dual I/O
  //10b Quad I/O
  //11b Reserved
  if ( (MasterCapibility.Field.IOMode == 2)
    && (DeviceCapCfg.Field.RO_IOModeSupported & BIT1)
    && (LocalCfgPtr->Espi.IoMode >= 0x02))
  {
    // Quad Mode
    Slave0Config.Field.IOModeSelect = 0x02;
    DeviceCapCfg.Field.IOModeSelect = 0x02;
  } else if ( (MasterCapibility.Field.IOMode >= 1)
    && (DeviceCapCfg.Field.RO_IOModeSupported & BIT0)
    && (LocalCfgPtr->Espi.IoMode >= 0x01))
  {
    // Dual Mode
    Slave0Config.Field.IOModeSelect = 0x01;
    DeviceCapCfg.Field.IOModeSelect = 0x01;
  } else {
    // single Mode
    Slave0Config.Field.IOModeSelect = 0x00;
    DeviceCapCfg.Field.IOModeSelect = 0x00;
  }

  //
  // Alert Mode
  //
  Slave0Config.Field.AlertMode = AlertMode;
  DeviceCapCfg.Field.AlertMode = AlertMode;

  //
  // CRC Checking
  //
  Slave0Config.Field.CRCCheckingEnable = MasterCapibility.Field.CRCCheck;
  DeviceCapCfg.Field.CRCCheckingEn     = MasterCapibility.Field.CRCCheck;

  //
  // update Device configuration and Slave0 configuration
  //
  KTeSPI_SetConfiguration (EspiBase, SLAVE_GENERAL_CAPCFG, DeviceCapCfg.Value);
  ACPIMMIO32 (EspiBase + 0x68) = (Slave0Config.Value & 0xFFFFFFF0);

  //
  // 7. Set Alert enable bit of eSPI Global Control Register1
  //  (eSPI_Global_Control_Reg_1_0x34[20]) to 1b
  //
  ACPIMMIO32 (EspiBase + 0x34) |= BIT20;

  //
  // 8 - 11 Configure and enable channels
  //
  // FA Channel
  //
  if ( Slave0Config.Field.FlashAccessChannelEnable ) {
    SlaveFACapCfg.Value      = KTeSPI_GetConfiguration (EspiBase, SLAVE_FA_CAPCFG);
    SlaveFACapCfg.Field.FAEn = 1;
    KTeSPI_SetConfiguration (EspiBase, SLAVE_FA_CAPCFG, SlaveFACapCfg.Value);
    do {
      SlaveFACapCfg.Value = KTeSPI_GetConfiguration (EspiBase, SLAVE_FA_CAPCFG);
      FchStall (2, NULL); // delay
    } while (!SlaveFACapCfg.Field.RO_FAReady);
    ACPIMMIO32 (EspiBase + 0x68) |= SLAVE_FA_ENABLE;
  }

  //
  // OOB Channel
  //
  if ( Slave0Config.Field.OOBMessageChannelEnable ) {
    SlaveOOBCapCfg.Value       = KTeSPI_GetConfiguration (EspiBase, SLAVE_OOB_CAPCFG);
    SlaveOOBCapCfg.Field.OOBEn = 1;
    KTeSPI_SetConfiguration (EspiBase, SLAVE_OOB_CAPCFG, SlaveOOBCapCfg.Value);
    do {
      SlaveOOBCapCfg.Value = KTeSPI_GetConfiguration (EspiBase, SLAVE_OOB_CAPCFG);
      FchStall (2, NULL); // delay
    } while (!SlaveOOBCapCfg.Field.RO_OOBReady);
    ACPIMMIO32 (EspiBase + 0x68) |= SLAVE_OOB_ENABLE;
  }

  //
  // VW Channel
  //
  if ( Slave0Config.Field.VWChannelEnable ) {
    SlaveVWCapCfg.Value      = KTeSPI_GetConfiguration (EspiBase, SLAVE_VW_CAPCFG);
    SlaveVWCapCfg.Field.VWEn = 1;
    KTeSPI_SetConfiguration (EspiBase, SLAVE_VW_CAPCFG, SlaveVWCapCfg.Value);
    do {
      SlaveVWCapCfg.Value = KTeSPI_GetConfiguration (EspiBase, SLAVE_VW_CAPCFG);
      FchStall (2, NULL); // delay
    } while (!SlaveVWCapCfg.Field.RO_VWReady);
    ACPIMMIO32 (EspiBase + 0x68) |= SLAVE_VW_ENABLE;
  }

  //De-assert PLTRST#
  KTeSPI_PltResetDeassert (EspiBase);

  //
  // PC Channel
  //
  if ( Slave0Config.Field.PChannelEnable ) {
    SlavePCCapCfg.Value      = KTeSPI_GetConfiguration (EspiBase, SLAVE_PC_CAPCFG);
    SlavePCCapCfg.Field.PCEn = 1;
    KTeSPI_SetConfiguration (EspiBase, SLAVE_PC_CAPCFG, SlavePCCapCfg.Value);
    do {
      SlavePCCapCfg.Value = KTeSPI_GetConfiguration (EspiBase, SLAVE_PC_CAPCFG);
      FchStall (2, NULL); // delay
    } while (!SlavePCCapCfg.Field.RO_PCReady);
    ACPIMMIO32 (EspiBase + 0x68) |= SLAVE_PC_ENABLE;
  }
}

/**
 * FchinitEspiIoMux - Config ESPI controller
 *
 *
 *
 */
VOID
FchInitKTEspiIoMux (
  IN VOID  *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK  *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;

  // Alert Pad BP_ESPI_ALERT_L/AGPIO22 for either channel
  if (LocalCfgPtr->Espi.AlertMode == 0) { // Non-Alert Mode
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x16, AccessWidth8, 0x00, 0x01);
  }
  // Enable PU, disable PD
  RwMem (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x58, AccessWidth32,  ~ (UINT32) (BIT20 + BIT21), BIT20);

  // ESPI_CS
  RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x1E, AccessWidth8, 0x00, 0x01);           //ESPI_CS_L

  switch (LocalCfgPtr->Espi.Channel) {
  case 0x00:
    //IDS_HDT_CONSOLE (FCH_TRACE, " Channel0 use SPI0 Data/Clk...\n");

    // Disable PD and enable PU for Data PAD
    // BP_SPI_DAT[1]/ESPI_DAT[1] and BP_SPI_DAT[0]/ESPI_DAT[0]
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + 0x90, AccessWidth32, ~(UINT32)(BIT3 + BIT2 + BIT1 + BIT0), BIT2 + BIT0);
    break;
  case 0x01:
  case 0x0F:
    //IDS_HDT_CONSOLE (FCH_TRACE, " Channel1 use SPI1 Data/Clk...\n");

    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x51, AccessWidth8, 0x00, 0x00);           //ESPI_D0
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x50, AccessWidth8, 0x00, 0x00);           //ESPI_D1
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x44, AccessWidth8, 0x00, 0x00);           //ESPI_D2
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x45, AccessWidth8, 0x00, 0x00);           //ESPI_D3
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x4D, AccessWidth8, 0x00, 0x00);           //ESPI_CLK

    // Disable PD and enable PU for Data PAD
    //BP_SPI1_DAT[1]/ESPI_DAT[1]/EGPIO80
    RwMem (ACPI_MMIO_BASE + GPIO_BANK1_BASE + 0x40, AccessWidth32, ~(UINT32)(BIT21 + BIT20), BIT20);
    //BP_SPI1_DAT[0]/ESPI_DAT[0]/EGPIO81
    RwMem (ACPI_MMIO_BASE + GPIO_BANK1_BASE + 0x44, AccessWidth32, ~(UINT32)(BIT21 + BIT20), BIT20);
    break;
  }
}

/**
 * EspiEc0DisableLpcDecoding - Config ESPI controller
 *
 *
 */
VOID
EspiEc0DisableLpcDecoding ()
{
  //KT should have LPC decoding disabled
}

/**
 * FchInitKTEspiEnableKbc6064 - Config ESPI controller
 *
 * @param[in] Enable Enable/disable ESPI 60h/64h decoding.
 */
VOID
FchInitKTEspiEnableKbc6064 (
  IN  BOOLEAN     Enable
  )
{
  UINT32     EspiBase;

  EspiBase = GetEspiBase ();

  if ( Enable ) {
    RwMem (EspiBase + 0x40, AccessWidth32, ~(UINT32)BIT1, BIT1);  // 60h/64h
  } else {
    RwMem (EspiBase + 0x40, AccessWidth32, ~(UINT32)BIT1, 0x00);  // 60h/64h
  }

}
/**
 * FchInitKTEspiTimer - Config ESPI controller
 *
 *
 *
 * @param[in] EspiBase ESPI MMIO base.
 *
 */
VOID
FchInitKTEspiTimer (
  IN  UINT32     EspiBase
  )
{
  // eSPIx30 [29:24, WaitStateCounter]     = 3Fh,
  //         [23:8,  WatchDogCounter]      = 1400h,
  //         [6:4,   Glb_Alink_Idle_Timer] = 111b - 2048 clocks
  RwMem (EspiBase + 0x30, AccessWidth32, 0xFF00008F, 0x00140070);
  RwMem (EspiBase + 0x30, AccessWidth32, ~(UINT32)BIT0, BIT0);                            // WDG_EN
  RwMem (EspiBase + 0x30, AccessWidth32, ~(UINT32)BIT1, BIT1);                            // WAIT_CHKEN.
  RwMem (EspiBase + 0x30, AccessWidth32, 0xC0FFFFFF, 0x3F000000);                         // WAIT_CNT.
}

/**
 * FchInitKTEspiDevice0 - Config ESPI controller
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitKTEspiDevice0 (
  IN  VOID     *FchDataPtr
  )
{
  UINT32                EspiBase;
  UINT32                SpiBase;
  UINT32                EspiCntrlReg;
  FCH_RESET_DATA_BLOCK  *LocalCfgPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, " %a Entry\n",__FUNCTION__);

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;

  EspiBase = GetEspiBase ();
  SpiBase  = EspiBase - 0x10000;

  EspiEc0DisableLpcDecoding ();

  if (LocalCfgPtr->Espi.Channel == 0) {
    RwMem (SpiBase + 0x10, AccessWidth32, ~(UINT32)BIT2, 0x00);
  }
  if ((LocalCfgPtr->Espi.Channel == 1) || (LocalCfgPtr->Espi.Channel == 0xF)) {
    RwMem (SpiBase + 0x10, AccessWidth32, ~(UINT32)BIT2, BIT2);
  }
  FchInitKTEspiIoMux (FchDataPtr);

  //6.1 eSPI Control Register Initialization
  //  1  Enable PCI Bus Watch Dog Timer
  //  2  Enable eSPI Bus Wait State Control Timer
  //  3  Enable eSPI clock gating function if neeed.
  //  4  Set each SlaveN Error Interrupt Enable.
  //  5  Set eSPI Controller ERROR interrupt Mapping, default is SMI.
  //  6  Set eSPI error protection bit if needed.
  //  7  Set each SlaveN Register command interrupt Enable.
  //  8  Set eSPI Register Command interrupt Mapping, default is SMI.
  //  9  Set each SlaveN IO Range, MMIO Range and decoding enable.
  //  10 Enable eSPI subtractive decoding if needed
  //  11 Assign IRQ0~IRQ23 to each SlaveN by setting each SlaveN IRQ Mask bit, IRQ plority.
  //  12 Enable Configurable Virtual Wire Index/Data Received Register.
  //  13 Set eSPI Bus Master Enable.

  //  eSPIx00[5:4, SlaveNSelect] = 00b - Slave 0 selected.
  //  Make sure the following operation is for slave 0
  RwMem (EspiBase + 0x00, AccessWidth32, ~(UINT32)(BIT4 + BIT5), 0x00);                   // Select Slave0

  FchInitKTEspiTimer (EspiBase);                                                            // 1 - 2

  //FchinitEspiClockGating (0, EspiBase);                                                 // 3
  //FchinitEspiErrCmdInterrupt (0, EspiBase);                                             // 4 - 8
  //FchinitEspiDecoding (0, EspiBase);                                                    // 9 - 10
  FchInitKTEspiIoDecode(FchDataPtr);
  FchInitKTEspiMmioDecode(FchDataPtr);
  RwMem (EspiBase + 0x40, AccessWidth32, ~(UINT32)BIT1, BIT1);                            // 60h/64h
  // CRCCheckingEnable/AlertMode; OOBValidBitEnable/PChannelEnable/VWChannelEnable/OOBMessageChannelEnable
  //RwMem (EspiBase + 0x68, AccessWidth32, 0x00, 0xC000001E);
  //FchinitEspiVWIrq (0, EspiBase);                                                       // 11 - 12
  RwMem (EspiBase + 0xA4, AccessWidth32, 0x00, 0x00040506);
  RwMem (EspiBase + 0xA8, AccessWidth32, 0x00, 0x00000007);

  if (LocalCfgPtr->Espi.IrqPolarity){
    RwMem (EspiBase + 0xAC, AccessWidth32, 0xFF000000, LocalCfgPtr->Espi.IrqPolarity & 0x00FFFFFF);
  }

  if (LocalCfgPtr->Espi.IrqMask != 0x00FFFFFF) {
    RwMem (
      ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG40,
      AccessWidth32,
      0xFF000000,
      LocalCfgPtr->Espi.IrqMask & 0x00FFFFFF
      );
  }

  ReadMem (SpiBase + 0x10, AccessWidth32, &EspiCntrlReg);                             //Program eSPI_Req_NotWith_VW_Req
  if (EspiCntrlReg & BIT2) {
    RwMem (EspiBase + 0x34, AccessWidth32, ~(UINT32)BIT21, BIT21);
  } else {
    RwMem (EspiBase + 0x34, AccessWidth32, ~(UINT32)BIT21, 0);
  }

  RwMem (EspiBase + 0x6C, AccessWidth32, 0x00, 0xFFFFFFFF);                               // Enable interrupt
  RwMem (EspiBase + 0x34, AccessWidth32, ~(UINT32)BIT1, BIT1);                            // BUS_MASTER_EN

  FchinitKTEspiSlave0Device (FchDataPtr, EspiBase);
}

VOID
FchInitKTEspiEnableIo80 (
  IN  BOOLEAN     Enable
  )
{
  UINT32       EspiBase;

  EspiBase = GetEspiBase();
  if (Enable){
    RwMem (EspiBase + 0x40, AccessWidth32, ~(UINT32)BIT2, BIT2);                            // Enable 80 IO decode
  }else{
    RwMem (EspiBase + 0x40, AccessWidth32, ~(UINT32)BIT2, 0x00);                            // Disable 80 IO decode
  }
}

/**
 * FchInitKTEspiIoDecode - Config ESPI controller
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 */
VOID
FchInitKTEspiIoDecode (
    IN  VOID       *FchDataPtr
  )
{
  UINT32                          EspiBase;
  FCH_RESET_DATA_BLOCK            *LocalCfgPtr;
  UINT32                          Slave0DecodeEnBitMap = 0; // BIT8  - IO Range 0
                                                            // BIT9  - IO Range 1
                                                            // BIT10 - IO Range 2
                                                            // BIT11 - IO Range 3
                                                            // BIT16 - IO Range 4
                                                            // BIT17 - IO Range 5
                                                            // BIT18 - IO Range 6
                                                            // BIT19 - IO Range 7
                                                            // BIT20 - IO Range 8
                                                            // BIT21 - IO Range 9
                                                            // BIT22 - IO Range 10
                                                            // BIT23 - IO Range 11
                                                            // BIT24 - IO Range 12
                                                            // BIT25 - IO Range 13
                                                            // BIT26 - IO Range 14
                                                            // BIT27 - IO Range 15
  UINT32                          Slave0IoBaseReg0 = 0;
  UINT32                          Slave0IoBaseReg1 = 0;
  UINT32                          Slave0IoSize = 0;
  UINT32                          Slave0IoBaseReg2 = 0;
  UINT32                          Slave0IoBaseReg3 = 0;
  UINT32                          Slave0IoSize1 = 0;
  UINT32                          Slave0IoBaseReg4 = 0;
  UINT32                          Slave0IoBaseReg5 = 0;
  UINT32                          Slave0IoSize2 = 0;
  UINT32                          Slave0IoBaseReg6 = 0;
  UINT32                          Slave0IoBaseReg7 = 0;
  UINT32                          Slave0IoSize3 = 0;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  EspiBase = GetEspiBase ();

  ReadMem (EspiBase + 0x44, AccessWidth32, &Slave0IoBaseReg0);                   // IO range 0
  ReadMem (EspiBase + 0x48, AccessWidth32, &Slave0IoBaseReg1);                   // IO range 1
  ReadMem (EspiBase + 0x4C, AccessWidth32, &Slave0IoSize);                       // IO range size
  ReadMem (EspiBase + 0x80, AccessWidth32, &Slave0IoBaseReg2);                   // IO range 2
  ReadMem (EspiBase + 0x84, AccessWidth32, &Slave0IoBaseReg3);                   // IO range 3
  ReadMem (EspiBase + 0x88, AccessWidth32, &Slave0IoSize1);                      // IO range size 1
  ReadMem (EspiBase + 0x8C, AccessWidth32, &Slave0IoBaseReg4);                   // IO range 4
  ReadMem (EspiBase + 0x90, AccessWidth32, &Slave0IoBaseReg5);                   // IO range 5
  ReadMem (EspiBase + 0x94, AccessWidth32, &Slave0IoSize2);                      // IO range size 2
  ReadMem (EspiBase + 0xB0, AccessWidth32, &Slave0IoBaseReg6);                   // IO range 6
  ReadMem (EspiBase + 0xB4, AccessWidth32, &Slave0IoBaseReg7);                   // IO range 7
  ReadMem (EspiBase + 0xB8, AccessWidth32, &Slave0IoSize3);                      // IO range size 3
  ReadMem (EspiBase + 0x40, AccessWidth32, &Slave0DecodeEnBitMap);
  Slave0DecodeEnBitMap &= 0xFFF0F00;
  if ((Slave0DecodeEnBitMap & BIT8) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[0].Enable){
      Slave0IoBaseReg0 &= 0xFFFF0000;
      Slave0IoBaseReg0 |= LocalCfgPtr->Espi.Decode.Io[0].Base;        // IO range
      Slave0IoSize &= 0xFFFFFF00;
      Slave0IoSize |= LocalCfgPtr->Espi.Decode.Io[0].Size;            // IO range size
      Slave0DecodeEnBitMap |= BIT8;                                   // Enable IO range 0
    }else{
      Slave0DecodeEnBitMap &= (~BIT8);                                // Disable IO range 0
    }
  }

  if ((Slave0DecodeEnBitMap & BIT9) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[1].Enable){
      Slave0IoBaseReg0 &= 0x0000FFFF;
      Slave0IoBaseReg0 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[1].Base) << 16);// IO range
      Slave0IoSize &= 0xFFFF00FF;
      Slave0IoSize |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[1].Size) << 8);     // IO range size
      Slave0DecodeEnBitMap |= BIT9;                                             // Enable IO range 1
    }else{
      Slave0DecodeEnBitMap &= (~BIT9);                                          // Disable IO range 1
    }
  }

  if ((Slave0DecodeEnBitMap & BIT10) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[2].Enable){
      Slave0IoBaseReg1 &= 0xFFFF0000;
      Slave0IoBaseReg1 |= LocalCfgPtr->Espi.Decode.Io[2].Base;                // IO range
      Slave0IoSize &= 0xFF00FFFF;
      Slave0IoSize |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[2].Size) << 16);  // IO range size
      Slave0DecodeEnBitMap |= BIT10;                                          // Enable IO range 2
    }else{
      Slave0DecodeEnBitMap &= (~BIT10);                                       // Disable IO range 2
    }
  }

  if ((Slave0DecodeEnBitMap & BIT11) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[3].Enable){
      Slave0IoBaseReg1 &= 0x0000FFFF;
      Slave0IoBaseReg1 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[3].Base) << 16);// IO range
      Slave0IoSize &= 0x00FFFFFF;
      Slave0IoSize |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[3].Size) << 24);    // IO range size
      Slave0DecodeEnBitMap |= BIT11;                                            // Enable IO range 3
    }else{
      Slave0DecodeEnBitMap &= (~BIT11);                                         // Disable IO range 3
    }
  }

  if ((Slave0DecodeEnBitMap & BIT16) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[4].Enable){
      Slave0IoBaseReg2 &= 0xFFFF0000;
      Slave0IoBaseReg2 |= LocalCfgPtr->Espi.Decode.Io[4].Base;        // IO range
      Slave0IoSize1 &= 0xFFFFFF00;
      Slave0IoSize1 |= LocalCfgPtr->Espi.Decode.Io[4].Size;           // IO range size
      Slave0DecodeEnBitMap |= BIT16;                                  // Enable IO range 4
    }else{
      Slave0DecodeEnBitMap &= (~BIT16);                               // Disable IO range 4
    }
  }

  if ((Slave0DecodeEnBitMap & BIT17) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[5].Enable){
      Slave0IoBaseReg2 &= 0x0000FFFF;
      Slave0IoBaseReg2 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[5].Base) << 16);// IO range
      Slave0IoSize1 &= 0xFFFF00FF;
      Slave0IoSize1 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[5].Size) << 8);    // IO range size
      Slave0DecodeEnBitMap |= BIT17;                                            // Enable IO range 5
    }else{
      Slave0DecodeEnBitMap &= (~BIT17);                                         // Disable IO range 5
    }
  }

  if ((Slave0DecodeEnBitMap & BIT18) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[6].Enable){
      Slave0IoBaseReg3 &= 0xFFFF0000;
      Slave0IoBaseReg3 |= LocalCfgPtr->Espi.Decode.Io[6].Base;                // IO range
      Slave0IoSize1 &= 0xFF00FFFF;
      Slave0IoSize1 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[6].Size) << 16); // IO range size
      Slave0DecodeEnBitMap |= BIT18;                                          // Enable IO range 6
    }else{
      Slave0DecodeEnBitMap &= (~BIT18);                                       // Disable IO range 6
    }
  }

  if ((Slave0DecodeEnBitMap & BIT19) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[7].Enable){
      Slave0IoBaseReg3 &= 0x0000FFFF;
      Slave0IoBaseReg3 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[7].Base) << 16);// IO range
      Slave0IoSize1 &= 0x00FFFFFF;
      Slave0IoSize1 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[7].Size) << 24);    // IO range size
      Slave0DecodeEnBitMap |= BIT19;                                            // Enable IO range 7
    }else{
      Slave0DecodeEnBitMap &= (~BIT19);                                         // Disable IO range 7
    }
  }

  if ((Slave0DecodeEnBitMap & BIT20) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[8].Enable){
      Slave0IoBaseReg4 &= 0xFFFF0000;
      Slave0IoBaseReg4 |= LocalCfgPtr->Espi.Decode.Io[8].Base;        // IO range
      Slave0IoSize2 &= 0xFFFFFF00;
      Slave0IoSize2 |= LocalCfgPtr->Espi.Decode.Io[8].Size;           // IO range size
      Slave0DecodeEnBitMap |= BIT20;                                  // Enable IO range 8
    }else{
      Slave0DecodeEnBitMap &= (~BIT20);                               // Disable IO range 8
    }
  }

  if ((Slave0DecodeEnBitMap & BIT21) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[9].Enable){
      Slave0IoBaseReg4 &= 0x0000FFFF;
      Slave0IoBaseReg4 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[9].Base) << 16);// IO range
      Slave0IoSize2 &= 0xFFFF00FF;
      Slave0IoSize2 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[9].Size) << 8);    // IO range size
      Slave0DecodeEnBitMap |= BIT21;                                            // Enable IO range 9
    }else{
      Slave0DecodeEnBitMap &= (~BIT21);                                         // Disable IO range 9
    }
  }

  if ((Slave0DecodeEnBitMap & BIT22) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[10].Enable){
      Slave0IoBaseReg5 &= 0xFFFF0000;
      Slave0IoBaseReg5 |= LocalCfgPtr->Espi.Decode.Io[10].Base;               // IO range
      Slave0IoSize2 &= 0xFF00FFFF;
      Slave0IoSize2 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[10].Size) << 16);// IO range size
      Slave0DecodeEnBitMap |= BIT22;                                          // Enable IO range 10
    }else{
      Slave0DecodeEnBitMap &= (~BIT22);                                       // Disable IO range 10
    }
  }

  if ((Slave0DecodeEnBitMap & BIT23) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[11].Enable){
      Slave0IoBaseReg5 &= 0x0000FFFF;
      Slave0IoBaseReg5 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[11].Base) << 16);// IO range
      Slave0IoSize2 &= 0x00FFFFFF;
      Slave0IoSize2 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[11].Size) << 24);   // IO range size
      Slave0DecodeEnBitMap |= BIT23;                                             // Enable IO range 11
    }else{
      Slave0DecodeEnBitMap &= (~BIT23);                                          // Disable IO range 11
    }
  }

  if ((Slave0DecodeEnBitMap & BIT24) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[12].Enable){
      Slave0IoBaseReg6 &= 0xFFFF0000;
      Slave0IoBaseReg6 |= LocalCfgPtr->Espi.Decode.Io[12].Base;       // IO range
      Slave0IoSize3 &= 0xFFFFFF00;
      Slave0IoSize3 |= LocalCfgPtr->Espi.Decode.Io[12].Size;          // IO range size
      Slave0DecodeEnBitMap |= BIT24;                                  // Enable IO range 12
    }else{
      Slave0DecodeEnBitMap &= (~BIT24);                               // Disable IO range 12
    }
  }

  if ((Slave0DecodeEnBitMap & BIT25) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[13].Enable){
      Slave0IoBaseReg6 &= 0x0000FFFF;
      Slave0IoBaseReg6 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[13].Base) << 16);// IO range
      Slave0IoSize3 &= 0xFFFF00FF;
      Slave0IoSize3 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[13].Size) << 8);    // IO range size
      Slave0DecodeEnBitMap |= BIT25;                                             // Enable IO range 13
    }else{
      Slave0DecodeEnBitMap &= (~BIT25);                                          // Disable IO range 13
    }
  }

  if ((Slave0DecodeEnBitMap & BIT26) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[14].Enable){
      Slave0IoBaseReg7 &= 0xFFFF0000;
      Slave0IoBaseReg7 |= LocalCfgPtr->Espi.Decode.Io[14].Base;               // IO range
      Slave0IoSize3 &= 0xFF00FFFF;
      Slave0IoSize3 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[14].Size) << 16);// IO range size
      Slave0DecodeEnBitMap |= BIT26;                                          // Enable IO range 14
    }else{
      Slave0DecodeEnBitMap &= (~BIT26);                                       // Disable IO range 14
    }
  }

  if ((Slave0DecodeEnBitMap & BIT27) == 0) {  //Make sure IO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Io[15].Enable){
      Slave0IoBaseReg7 &= 0x0000FFFF;
      Slave0IoBaseReg7 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[15].Base) << 16);// IO range
      Slave0IoSize3 &= 0x00FFFFFF;
      Slave0IoSize3 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Io[15].Size) << 24);   // IO range size
      Slave0DecodeEnBitMap |= BIT27;                                             // Enable IO range 15
    }else{
      Slave0DecodeEnBitMap &= (~BIT27);                                          // Disable IO range 15
    }
  }

  RwMem (EspiBase + 0x44, AccessWidth32, 0x00000000, Slave0IoBaseReg0);                   // IO range 0
  RwMem (EspiBase + 0x48, AccessWidth32, 0x00000000, Slave0IoBaseReg1);                   // IO range 1
  RwMem (EspiBase + 0x4C, AccessWidth32, 0x00000000, Slave0IoSize);                       // IO range size
  RwMem (EspiBase + 0x80, AccessWidth32, 0x00000000, Slave0IoBaseReg2);                   // IO range 2
  RwMem (EspiBase + 0x84, AccessWidth32, 0x00000000, Slave0IoBaseReg3);                   // IO range 3
  RwMem (EspiBase + 0x88, AccessWidth32, 0x00000000, Slave0IoSize1);                      // IO range size 1
  RwMem (EspiBase + 0x8C, AccessWidth32, 0x00000000, Slave0IoBaseReg4);                   // IO range 4
  RwMem (EspiBase + 0x90, AccessWidth32, 0x00000000, Slave0IoBaseReg5);                   // IO range 5
  RwMem (EspiBase + 0x94, AccessWidth32, 0x00000000, Slave0IoSize2);                      // IO range size 2
  RwMem (EspiBase + 0xB0, AccessWidth32, 0x00000000, Slave0IoBaseReg6);                   // IO range 6
  RwMem (EspiBase + 0xB4, AccessWidth32, 0x00000000, Slave0IoBaseReg7);                   // IO range 7
  RwMem (EspiBase + 0xB8, AccessWidth32, 0x00000000, Slave0IoSize3);                      // IO range size 3
  RwMem (EspiBase + 0x40, AccessWidth32, 0xF000F0FF, Slave0DecodeEnBitMap);               // Enable IO range
}

/**
 * FchInitKTEspiMmioDecode - Config ESPI controller
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 */
VOID
FchInitKTEspiMmioDecode (
    IN  VOID       *FchDataPtr
  )
{
  UINT32                          EspiBase;
  FCH_RESET_DATA_BLOCK            *LocalCfgPtr;
  UINT32                          Slave0DecodeEnBitMap = 0; // BIT12 - MMIO Range 0
                                                            // BIT13 - MMIO Range 1
                                                            // BIT14 - MMIO Range 2
                                                            // BIT15 - MMIO Range 3
                                                            // BIT28 - MMIO Range 4
  UINT32                          Slave0MmioBaseReg0 = 0;
  UINT32                          Slave0MmioBaseReg1 = 0;
  UINT32                          Slave0MmioBaseReg2 = 0;
  UINT32                          Slave0MmioBaseReg3 = 0;
  UINT32                          Slave0MmioBaseReg4 = 0;
  UINT32                          Slave0MmioSizeReg0 = 0;
  UINT32                          Slave0MmioSizeReg1 = 0;
  UINT32                          Slave0MmioSizeReg2 = 0;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  EspiBase = GetEspiBase ();

  ReadMem (EspiBase + 0x50, AccessWidth32, &Slave0MmioBaseReg0);                   // MMIO range 0
  ReadMem (EspiBase + 0x54, AccessWidth32, &Slave0MmioBaseReg1);                   // MMIO range 1
  ReadMem (EspiBase + 0x58, AccessWidth32, &Slave0MmioBaseReg2);                   // MMIO range 2
  ReadMem (EspiBase + 0x5C, AccessWidth32, &Slave0MmioBaseReg3);                   // MMIO range 3
  ReadMem (EspiBase + 0xBC, AccessWidth32, &Slave0MmioBaseReg4);                   // MMIO range 4
  ReadMem (EspiBase + 0x60, AccessWidth32, &Slave0MmioSizeReg0);                   // MMIO range size 0
  ReadMem (EspiBase + 0x64, AccessWidth32, &Slave0MmioSizeReg1);                   // MMIO range size 1
  ReadMem (EspiBase + 0xC0, AccessWidth32, &Slave0MmioSizeReg2);                   // MMIO range size 2
  ReadMem (EspiBase + 0x40, AccessWidth32, &Slave0DecodeEnBitMap);
  Slave0DecodeEnBitMap &= 0x1000F000;
  if ((Slave0DecodeEnBitMap & BIT12) == 0) {  //Make sure MMIO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Mmio[0].Enable){
      Slave0MmioBaseReg0 = LocalCfgPtr->Espi.Decode.Mmio[0].Base;       // MMIO range
      Slave0MmioSizeReg0 &= 0xFFFF0000;
      Slave0MmioSizeReg0 |= LocalCfgPtr->Espi.Decode.Mmio[0].Size;      // MMIO range size
      Slave0DecodeEnBitMap |= BIT12;                                    // Enable MMIO range 0
    }else{
      Slave0DecodeEnBitMap &= (~BIT12);                                 // Disable MMIO range 0
    }
  }

  if ((Slave0DecodeEnBitMap & BIT13) == 0) {  //Make sure MMIO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Mmio[1].Enable){
      Slave0MmioBaseReg1 = LocalCfgPtr->Espi.Decode.Mmio[1].Base;                   // MMIO range
      Slave0MmioSizeReg0 &= 0x0000FFFF;
      Slave0MmioSizeReg0 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Mmio[1].Size) << 16);// MMIO range size
      Slave0DecodeEnBitMap |= BIT13;                                                // Enable MMIO range 1
    }else{
      Slave0DecodeEnBitMap &= (~BIT13);                                             // Disable MMIO range 1
    }
  }

  if ((Slave0DecodeEnBitMap & BIT14) == 0) {  //Make sure MMIO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Mmio[2].Enable){
      Slave0MmioBaseReg2 = LocalCfgPtr->Espi.Decode.Mmio[2].Base;                  // MMIO range
      Slave0MmioSizeReg1 &= 0xFFFF0000;
      Slave0MmioSizeReg1 |= LocalCfgPtr->Espi.Decode.Mmio[2].Size;                 // MMIO range size
      Slave0DecodeEnBitMap |= BIT14;                                               // Enable MMIO range 2
    }else{
      Slave0DecodeEnBitMap &= (~BIT14);                                            // Disable MMIO range 2
    }
  }

  if ((Slave0DecodeEnBitMap & BIT15) == 0) {  //Make sure MMIO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Mmio[3].Enable){
      Slave0MmioBaseReg3 = LocalCfgPtr->Espi.Decode.Mmio[3].Base;                   // MMIO range
      Slave0MmioSizeReg1 &= 0x0000FFFF;
      Slave0MmioSizeReg1 |= ((UINT32)(LocalCfgPtr->Espi.Decode.Mmio[3].Size) << 16);// MMIO range size
      Slave0DecodeEnBitMap |= BIT15;                                                // Enable MMIO range 3
    }else{
      Slave0DecodeEnBitMap &= (~BIT15);                                             // Disable MMIO range 3
    }
  }

  if ((Slave0DecodeEnBitMap & BIT28) == 0) {  //Make sure MMIO decoding is not configured by PSP previously
    if (LocalCfgPtr->Espi.Decode.Mmio[4].Enable){
      Slave0MmioBaseReg4 = LocalCfgPtr->Espi.Decode.Mmio[4].Base;                  // MMIO range
      Slave0MmioSizeReg2 &= 0xFE000000;
      Slave0MmioSizeReg2 |= LocalCfgPtr->Espi.Decode.Mmio[4].Size;                 // MMIO range size
      Slave0DecodeEnBitMap |= BIT28;                                               // Enable MMIO range 4
    }else{
      Slave0DecodeEnBitMap &= (~BIT28);                                            // Disable MMIO range 4
    }
  }

  RwMem (EspiBase + 0x50, AccessWidth32, 0x00000000, Slave0MmioBaseReg0);                   // MMIO range 0
  RwMem (EspiBase + 0x54, AccessWidth32, 0x00000000, Slave0MmioBaseReg1);                   // MMIO range 1
  RwMem (EspiBase + 0x58, AccessWidth32, 0x00000000, Slave0MmioBaseReg2);                   // MMIO range 2
  RwMem (EspiBase + 0x5C, AccessWidth32, 0x00000000, Slave0MmioBaseReg3);                   // MMIO range 3
  RwMem (EspiBase + 0xBC, AccessWidth32, 0x00000000, Slave0MmioBaseReg4);                   // MMIO range 4
  RwMem (EspiBase + 0x60, AccessWidth32, 0x00000000, Slave0MmioSizeReg0);                   // MMIO range size 0
  RwMem (EspiBase + 0x64, AccessWidth32, 0x00000000, Slave0MmioSizeReg1);                   // MMIO range size 1
  RwMem (EspiBase + 0xC0, AccessWidth32, 0xFE000000, Slave0MmioSizeReg2);                   // MMIO range size 2
  RwMem (EspiBase + 0x40, AccessWidth32, 0xEFFF0FFF, Slave0DecodeEnBitMap);                 // Enable MMIO range
}
