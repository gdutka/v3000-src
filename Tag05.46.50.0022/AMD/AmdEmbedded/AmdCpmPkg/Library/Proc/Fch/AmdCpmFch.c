/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmBase.h>
#include <AmdCpmBaseIo.h>
#include CPM_PPI_DEFINITION (AmdCpmTablePpi)

#define GPIO_BASE                       0x100
#define SMI_BASE                        0x200
#define PMIO_BASE                       0x300
#define PMIO2_BASE                      0x400
#define RAM_BASE                        0x500
#define CMOSRAM_BASE                    0x600
#define CMOS_BASE                       0x700
#define ACPI_BASE                       0x800
#define IOMUX_BASE                      0xD00
#define MISC_BASE                       0xE00
#define MISC2_BASE                      0x1300
#define GPIO_BANK_BASE                  0x1500
#define REMOTE_GPIO_BANK_BASE           0x1200
#define REMOTE_IOMUX_BASE               0x12C0

#define AMD_SMIREG_EVENT_ENABLE         0x04
#define AMD_SMIREG_SCITRIG              0x08
#define AMD_SMIREG_SCILEVEL             0x0C
#define AMD_SMIREG_SMISCIEN             0x14
#define AMD_SMIREG_SWSCIEN              0x18
#define AMD_SMIREG_SWSCIDATA            0x1C
#define AMD_SMIREG_SCIS0EN              0x20
#define AMD_SMIREG_SCIMAP0              0x40
#define AMD_SMIREG_SMITRIG              0x98
#define AMD_SMIREG_SMICONTROL0          0xA0
#define AMD_SMICONTROL_MASK             0x03
#define AMD_GEVENT_SCIMASK              0xE0    // And Mask ~ 0x1F

#define IO_DELAY_PORT                   0xDE
#define MAX_TIMER                       0xFFFFFFFFul
#define ELAPSED_TIME_UNIT               28
#define ELAPSED_TIME_DIVIDER            100

#define SB_SATA_BUS                     0x00
#define SB_SATA_DEVICE                  0x11
#define SB_SATA_FUNCTION                0x00

#define PtGpioFun                       0x02
#define PtTempBus                       0x1F

#define FchSmnGpio                      0x02D02500
#define FchSmnIomux                     0x02D01D00
#define FchSmnMisc                      0x02D01E00
#define FchSmnMisc2                     0x02D02300
#define FchSmnRemoteGpio                0x02D02200
#define FchSmnRemoteIomux               0x02D022C0

#define FCH_LPC_BUS                     0
#define FCH_LPC_DEV                     20
#define FCH_LPC_FUNC                    3
#define FCH_LPC_REG4A                   0x4A        // IO/Mem Port Decode Enable Register 6- RW

#define SMB_IOREG00                     0x00        // SMBusStatus
#define SMB_IOREG01                     0x01        // SMBusSlaveStatus
#define SMB_IOREG02                     0x02        // SMBusControl
#define SMB_IOREG03                     0x03        // SMBusHostCmd
#define SMB_IOREG04                     0x04        // SMBusAddress
#define SMB_IOREG05                     0x05        // SMBusData0
#define SMB_IOREG06                     0x06        // SMBusData1
#define SMB_IOREG07                     0x07        // SMBusBlockData
#define SMB_IOREG08                     0x08        // SMBusSlaveControl
#define SMB_ALL_HOST_STATUS             0x1F        // HostBusy + SMBInterrupt + DeviceErr + BusCollision + Failed
#define SMB_ALL_SLAVE_STATUS            0x3F        // SlaveBusy + SlaveInit + SlaveStatus + Shadow1Status + Shadow2Status + AlertStatus
#define SMB_CMD_BYTE_DATA               0x02 << 2   // Byte Data Read or Write
#define SMB_CMD_WORD_DATA               0x03 << 2   // Word Data Read or Write
#define SMB_CMD_BLOCK                   0x05 << 2   // Block Read or Write
#define SMBUS_READ_ENABLE               BIT0

/*
 * I2C Registers offset
 */
#define FCH_I2C_IC_CON                               (0x0)
#define FCH_I2C_IC_TAR                               (0x4)
#define FCH_I2C_IC_SAR                               (0x8)
#define FCH_I2C_IC_DATA_CMD                          (0x10)
#define FCH_I2C_IC_SS_SCL_HCNT                       (0x14)
#define FCH_I2C_IC_SS_SCL_LCNT                       (0x18)
#define FCH_I2C_IC_FS_SCL_HCNT                       (0x1c)
#define FCH_I2C_IC_FS_SCL_LCNT                       (0x20)
#define FCH_I2C_IC_HS_SCL_HCNT                       (0x24)
#define FCH_I2C_IC_HS_SCL_LCNT                       (0x28)
#define FCH_I2C_IC_INTR_STAT                         (0x2c)
#define FCH_I2C_IC_INTR_MASK                         (0x30)
#define FCH_I2C_IC_RAW_INTR_STAT                     (0x34)
#define FCH_I2C_IC_RX_TL                             (0x38)
#define FCH_I2C_IC_TX_TL                             (0x3C)
#define FCH_I2C_IC_CLR_INTR                          (0x40)
#define FCH_I2C_IC_CLR_RX_UNDER                      (0x44)
#define FCH_I2C_IC_CLR_RX_OVER                       (0x48)
#define FCH_I2C_IC_CLR_TX_ABRT                       (0x54)
#define FCH_I2C_IC_CLR_ACTIVITY                      (0x5c)
#define FCH_I2C_IC_CLR_STOP_DET                      (0x60)
#define FCH_I2C_IC_CLR_START_DET                     (0x64)
#define FCH_I2C_IC_ENABLE                            (0x6c)
#define FCH_I2C_IC_STATUS                            (0x70)
#define FCH_I2C_IC_TXFLR                             (0x74)
#define FCH_I2C_IC_RXFLR                             (0x78)
#define FCH_I2C_IC_SDA_HOLD                          (0x7C)
#define FCH_I2C_IC_TX_ABRT_SOURCE                    (0x80)
#define FCH_I2C_IC_ENABLE_STATUS                     (0x9c)
#define FCH_I2C_IC_FS_SPKLEN                         (0xa0)
#define FCH_I2C_IC_HS_SPKLEN                         (0xa4)
#define FCH_I2C_IC_CLR_RESTART_DET                   (0xa8)
#define FCH_I2C_IC_COMP_PARAM_1                      (0xf4)
#define FCH_I2C_IC_COMP_TYPE                         (0xfc)
// Component Parameter Register
#define FCH_I2C_RX_BUFFER_DEPTH_MASK             (0x0000FF00UL)
#define FCH_I2C_RX_BUFFER_DEPTH_SHIFT            (8)
#define FCH_I2C_TX_BUFFER_DEPTH_MASK             (0x00FF0000UL)
#define FCH_I2C_TX_BUFFER_DEPTH_SHIFT            (16)
AMD_CPM_FCH_I2C_CONFIG  mI2CConfigRplRmb[4] = {
// Scl  Sda  IoMux    AoAcBar     MmioBar
  {145, 146,   0,   0xFED81E4A, 0xFEDC2000}, //I2C0
  {147, 148,   0,   0xFED81E4C, 0xFEDC3000}, //I2C1
  {113, 114,   1,   0xFED81E4E, 0xFEDC4000}, //I2C2
  {19,  20,    1,   0xFED81E50, 0xFEDC5000}, //I2C3
};

AMD_CPM_FCH_I2C_CONFIG  mI2CConfiStp[6] = {
// Scl  Sda  IoMux    AoAcBar     MmioBar
  {145, 146,   1,   0xFED81E4A, 0xFEDC2000}, //I2C0
  {147, 148,   1,   0xFED81E4C, 0xFEDC3000}, //I2C1
  {149, 150,   1,   0xFED81E4E, 0xFEDC4000}, //I2C2
  {151, 152,   1,   0xFED81E50, 0xFEDC5000}, //I2C3
  {13,  14,    0,   0xFED81E52, 0xFEDC6000}, //I2C4
  {19,  20,    0,   0xFED81E54, 0xFEDCB000}, //I2C5
};

AMD_CPM_SATA_MODE_MASK  SataModeMask[] = {
  {0x7800, 0x01},           // IDE
  {0x7801, 0x02},           // AHCI
  {0x7802, 0x04},           // RAID
  {0x7803, 0x04},           // RAID5
  {0x7804, 0x08},           // AMD AHCI
  {0x7805, 0x04},           // Dot Hill RAID
  {0x780A, 0x04},           // 3rd Party RAID
  {0x4390, 0x01},           // IDE
  {0x4391, 0x02},           // AHCI
  {0x4392, 0x04},           // RAID
  {0x4393, 0x04},           // RAID5
  {0x4394, 0x08},           // AMD AHCI
  {0x7900, 0x01},           // IDE
  {0x7901, 0x02},           // AHCI
  {0x7902, 0x04},           // RAID
  {0x7903, 0x04},           // RAID5
  {0x7904, 0x08},           // AMD AHCI
  {0x7905, 0x04},           // Dot Hill RAID
  {0x790A, 0x04},           // 3rd Party RAID
  {0xFFFF, 0xFF},           //EOT
};

PCIE_BRIDGE_NAME FchPcieBridgeName[] = {
  {0, 21, 0, 0x30, AMD_CPM_FCH_PCIE_GPP0_NAME},
  {0, 21, 1, 0x31, AMD_CPM_FCH_PCIE_GPP1_NAME},
  {0, 21, 2, 0x32, AMD_CPM_FCH_PCIE_GPP2_NAME},
  {0, 21, 3, 0x33, AMD_CPM_FCH_PCIE_GPP3_NAME},
  0
};

UINT8 FchGeventGpioMappingTable[] = {
  0x7E,         // GEVENT0 => GPIO126
  0x81,         // GEVENT1 => GPIO129
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x0E,         // GEVENT6 => GPIO14
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x20,         // GEVENT9 => GPIO32
  0x06,         // GEVENT10 => GPIO6
  0x07,         // GEVENT11 => GPIO7
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x12,         // GEVENT14 => GPIO18
  0x18,         // GEVENT15 => GPIO24
  0x17,         // GEVENT16 => GPIO23
  0x19,         // GEVENT17 => GPIO25
  0x0B,         // GEVENT18 => GPIO11
  0x01,         // GEVENT19 => GPIO1
  0x0F,         // GEVENT20 => GPIO15
  0x0D,         // GEVENT21 => GPIO13
  0x09          // GEVENT22 => GPIO9
};

UINT8 FchGeventGpioMappingTable2[] = {
  0x41,         // GEVENT0 => GPIO65
  0x42,         // GEVENT1 => GPIO66
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x0E,         // GEVENT6 => GPIO14
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x44,         // GEVENT9 => GPIO68
  0x06,         // GEVENT10 => GPIO6
  0x07,         // GEVENT11 => GPIO7
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x12,         // GEVENT14 => GPIO18
  0x18,         // GEVENT15 => GPIO24
  0x17,         // GEVENT16 => GPIO23
  0x45,         // GEVENT17 => GPIO69
  0x0B,         // GEVENT18 => GPIO11
  0x01,         // GEVENT19 => GPIO1
  0x0F,         // GEVENT20 => GPIO15
  0x0D,         // GEVENT21 => GPIO13
  0x09,         // GEVENT22 => GPIO9
  0x08          // GEVENT23 => GPIO8
};

UINT8 FchGeventGpioMappingTableZP[] = {
  0x59,         // GEVENT0 => GPIO89
  0x5A,         // GEVENT1 => GPIO90
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x5B,         // GEVENT6 => GPIO91
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x56,         // GEVENT9 => GPIO86
  0x06,         // GEVENT10 => GPIO6
  0x4C,         // GEVENT11 => GPIO7      // ZP = GPIO76
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x12,         // GEVENT14 => GPIO18
  0x18,         // GEVENT15 => GPIO24
  0x17,         // GEVENT16 => GPIO23
  0x81,         // GEVENT17 => GPIO129
  0x54,         // GEVENT18 => GPIO84
  0x01,         // GEVENT19 => GPIO1
  0x28,         // GEVENT20 => GPIO40
  0x00,         // GEVENT21 => GPIO0
  0x09,         // GEVENT22 => GPIO9
  0x08          // GEVENT23 => GPIO8
};

UINT8 FchGeventGpioMappingTableSSP[] = {  // SSP & MTS
  0x59,         // GEVENT0 => GPIO89
  0x5A,         // GEVENT1 => GPIO90
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x5B,         // GEVENT6 => GPIO91
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x56,         // GEVENT9 => GPIO86
  0x06,         // GEVENT10 => GPIO6
  0x4C,         // GEVENT11 => GPIO7      // SSP & MTS = GPIO76
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x1D,         // GEVENT14 => GPIO18     // SSP & MTS = GPIO29
  0x1E,         // GEVENT15 => GPIO24     // SSP & MTS = GPIO30
  0x17,         // GEVENT16 => GPIO23
  0x81,         // GEVENT17 => GPIO129
  0x54,         // GEVENT18 => GPIO84
  0x01,         // GEVENT19 => GPIO1
  0x28,         // GEVENT20 => GPIO40
  0x00,         // GEVENT21 => GPIO0
  0x09,         // GEVENT22 => GPIO9
  0x08          // GEVENT23 => GPIO8
};

UINT8 FchGeventGpioMappingTableRV[] = {   // RV, RV2, RN & VN
  0x59,         // GEVENT0 => GPIO89
  0x5A,         // GEVENT1 => GPIO90
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x5B,         // GEVENT6 => GPIO91
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x56,         // GEVENT9 => GPIO86
  0x06,         // GEVENT10 => GPIO6
  0x07,         // GEVENT11 => GPIO7
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x12,         // GEVENT14 => GPIO18
  0x18,         // GEVENT15 => GPIO24
  0x17,         // GEVENT16 => GPIO23
  0x81,         // GEVENT17 => GPIO129
  0x54,         // GEVENT18 => GPIO84
  0x01,         // GEVENT19 => GPIO1
  0x28,         // GEVENT20 => GPIO40
  0x00,         // GEVENT21 => GPIO0
  0x09,         // GEVENT22 => GPIO9
  0x08          // GEVENT23 => GPIO8
};

UINT8 FchGeventGpioMappingTableRMB[] = {  // RMB, RPL
  0x59,         // GEVENT0 => GPIO89
  0x5A,         // GEVENT1 => GPIO90
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x0B,         // GEVENT5 => GPIO11
  0x5B,         // GEVENT6 => GPIO91
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x1D,         // GEVENT9 => GPIO29
  0x06,         // GEVENT10 => GPIO6
  0x07,         // GEVENT11 => GPIO7
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x12,         // GEVENT14 => GPIO18
  0x18,         // GEVENT15 => GPIO24
  0x17,         // GEVENT16 => GPIO23
  0x20,         // GEVENT17 => GPIO32
  0x54,         // GEVENT18 => GPIO84
  0x01,         // GEVENT19 => GPIO1
  0x28,         // GEVENT20 => GPIO40
  0x00,         // GEVENT21 => GPIO0
  0x09,         // GEVENT22 => GPIO9
  0x08          // GEVENT23 => GPIO8
};

UINT8 FchGeventGpioMappingTableSTP[] = {  // STP
  0x59,         // GEVENT0 => GPIO89
  0x73,         // GEVENT1 => GPIO115
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x74,         // GEVENT6 => GPIO116
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x56,         // GEVENT9 => GPIO86
  0x06,         // GEVENT10 => GPIO6
  0x4C,         // GEVENT11 => GPIO76  // STP = GPIO76
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x18,         // GEVENT14 => GPIO24
  0x1A,         // GEVENT15 => GPIO26
  0x17,         // GEVENT16 => GPIO23
  0x81,         // GEVENT17 => GPIO129
  0x1C,         // GEVENT18 => GPIO28
  0x01,         // GEVENT19 => GPIO1
  0x68,         // GEVENT20 => GPIO104
  0x00,         // GEVENT21 => GPIO0
  0x69,         // GEVENT22 => GPIO105
  0x6A          // GEVENT23 => GPIO106
};

/*---------------------------------------------------------------------------------------*/
/**
 * Execute I2C Read/Write Bytes
 *
 *
 * @param[in]  This               The pointer of AMD CPM Table Ppi or Protocol.
 * @param[in]  Select             The I2C number.
 * @param[in]  Address            The address of  device. 7bit device address.
 * @param[in]  ReadCount          The Counts of Read Bytes
 * @param[in]  ReadBuffer         The Read Bytes Buffer pointer
 * @param[in]  WriteCount         The Counts of Write Bytes
 * @param[in]  WriteBuffer        The Write Bytes Buffer pointer
 *
 * @retval     EFI_SUCCESS, EFI_INVALID_PARAMETER, EFI_TIMEOUT or EFI_DEVICE_ERROR
 *
 */
EFI_STATUS
EFIAPI
CpmReadWriteI2CBytes (
  IN       VOID                        *This,
  IN       UINT8                       Select,
  IN       UINT8                       Address,
  IN       UINTN                       ReadCount,
  OUT      UINT8                       *ReadBuffer,
  IN       UINTN                       WriteCount,
  OUT      UINT8                       *WriteBuffer
  )
{
  EFI_STATUS                      Status;
  AMD_CPM_COMMON_FUNCTION         *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  AMD_CPM_READTSC_FN              I2CReadTsc;
  AMD_CPM_MMIOWRITE8_FN           MWrite8;
  AMD_CPM_MMIOREAD8_FN            MRead8;
  AMD_CPM_MMIOREAD32_FN           MRead32;
  AMD_CPM_MMIOWRITE32_FN          MWrite32;
  BOOLEAN                         IsDeviceIdeInEntry;
  AMD_CPM_STALL_FN                Stall;
  UINT32                          Base;
  UINT32                          AoAc;
  UINT32                          Data32;
  UINT8                           SclMux;
  UINT8                           SdaMux;
  UINT64                          TimeOut;
  UINT64                          RdTstFreq;
  UINTN                           ReadLength;
  UINTN                           WriteLength;
  BOOLEAN                         ReadsNeeded;
  AMD_CPM_FCH_I2C_CONFIG          *I2CConfig;

  if (((ReadCount + WriteCount) < 2) || (WriteBuffer == NULL) || (WriteCount == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  if ((ReadCount != 0) && (ReadBuffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
    case CPM_FCH_REVISION_ID_STP:
      if (Select > 5) return EFI_UNSUPPORTED;
      I2CConfig = mI2CConfiStp;
      break;
    case CPM_FCH_REVISION_ID_RMB:
    case CPM_FCH_REVISION_ID_RPL:
      if (Select > 3) return EFI_UNSUPPORTED;
      I2CConfig = mI2CConfigRplRmb;
      break;
    default:
      return EFI_UNSUPPORTED;
  }
  //
  // Init parameters
  //
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr      = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  Stall             = CommonFunctionPtr->Stall;
  MWrite8           = CommonFunctionPtr->MmioWrite8;
  MRead8            = CommonFunctionPtr->MmioRead8;
  MRead32           = CommonFunctionPtr->MmioRead32;
  MWrite32          = CommonFunctionPtr->MmioWrite32;
  I2CReadTsc        = CommonFunctionPtr->ReadTsc;
  ReadsNeeded       = FALSE;
  ReadLength        = ReadCount;
  WriteLength       = WriteCount;
  Base              = I2CConfig[Select].IoBar;
  AoAc              = I2CConfig[Select].AoacAddress;
  RdTstFreq         = I2CReadTsc ();
  Stall (This, 100);
  RdTstFreq         = I2CReadTsc () - RdTstFreq;
  TimeOut           = I2CReadTsc () + MultU64x32(RdTstFreq, 10000);

  //
  // Save and Set I2C GPIO IOMux
  //
  SclMux = MRead8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + I2CConfig[Select].SclPin);
  SdaMux = MRead8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + I2CConfig[Select].SdaPin);
  MWrite8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + I2CConfig[Select].SclPin, I2CConfig[Select].Function);
  MWrite8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + I2CConfig[Select].SdaPin, I2CConfig[Select].Function);
  //
  // if device is in power off state, put device on
  //
  IsDeviceIdeInEntry = FALSE;
  if ((MRead8 (AoAc + 1) & 7) != 7) {
    MWrite8 (AoAc, 0x7C);
    IsDeviceIdeInEntry = TRUE;
    while ((MRead8 (AoAc + 1) & 7) != 7) {
      if (TimeOut <= I2CReadTsc ()) {
        Status = EFI_TIMEOUT;
        DEBUG ((EFI_D_ERROR, "I2cTimeout waiting for I2c D0: %x\n", MRead8 (AoAc+1)));
        goto ErrExit;
      }
      Stall (This, 10);
    }
  }
  //
  // Wait for I2C Idle
  //
  while (0x20 == (MRead32 (Base + FCH_I2C_IC_STATUS) & 0x20)) {
    if (TimeOut <= I2CReadTsc ()) {
      Status = EFI_TIMEOUT;
      DEBUG ((EFI_D_ERROR, "I2cTimeout waiting for I2c Idle: %x\n", MRead32 (Base + FCH_I2C_IC_STATUS)));
      goto ErrExit;
    }
    Stall (This, 10);
  }
  //
  // Program 0 to IC_ENABLE to Disable I2C
  //
  if ((MRead32 (Base + FCH_I2C_IC_ENABLE) & 1) == 1) { //if I2C is enabled
    MWrite32 (Base + FCH_I2C_IC_ENABLE, 0); // Disable the Controller
    while (0 != (MRead32 (Base + FCH_I2C_IC_ENABLE_STATUS) & 0x1)) {
      if (TimeOut <= I2CReadTsc ()) {
        Status = EFI_TIMEOUT;
        DEBUG ((EFI_D_ERROR, "I2cTimeout waiting I2cDisable Start: %x\n", MRead32 (Base + FCH_I2C_IC_ENABLE_STATUS)));
        goto ErrExit;
      }
      Stall (This, 10);
    }
  }
  //
  // Program IC_CON register fields:
  //1. Set IC_SALVE_DSIABLE to 1      - Slave Disable
  //2. Set IC_RESTART_EN to 1         - enable restart mode
  //3. Set IC_10BIT Master/Slave to 0 - enable 7-bit addressing
  //4. Set IC_MAX_SPPED_MODE to 2     - Fast mode
  //5. Set IC_MASTER_MODE to 1        - enable master
  //
  MWrite32 (Base + FCH_I2C_IC_CON, 0x65);
  //
  // Program address of target slave by writing to TAR register
  //
  MWrite32 (Base + FCH_I2C_IC_TAR, Address);
  //
  // Program IC_FS_LCNT/IC_FS_HCNT to set Low preiod of SCL
  //
  MWrite32 (Base + FCH_I2C_IC_FS_SCL_HCNT, 0x87);
  MWrite32 (Base + FCH_I2C_IC_FS_SCL_LCNT, 0xf0);
  MWrite32 (Base + FCH_I2C_IC_SDA_HOLD,    (0x30 << 16) | 0x30); //Fast Mode
  MWrite32 (Base + FCH_I2C_IC_RX_TL, 0);
  MWrite32 (Base + FCH_I2C_IC_TX_TL, 0);
  //
  // Program IC_INTR_MASK to enable all interruputs and Clear interrupt status
  //
  MWrite32 (Base + FCH_I2C_IC_INTR_MASK, 0);
  MRead32 (Base + FCH_I2C_IC_CLR_INTR);
  MRead32 (Base + FCH_I2C_IC_CLR_TX_ABRT);

  //
  // Program 1 to IC_ENABLE to Enable I2C
  //
  MWrite32 (Base + FCH_I2C_IC_ENABLE, 1); // Enable the Controller
  while (0 == (MRead32 (Base + FCH_I2C_IC_ENABLE_STATUS) & 1)) {
    if (TimeOut <= I2CReadTsc ()) {
      DEBUG ((EFI_D_ERROR, "I2cTimeout waiting I2cEnable : %x\n", MRead32 (Base + FCH_I2C_IC_ENABLE_STATUS)));
      Status = EFI_TIMEOUT;
      goto ErrExit;
    }
    Stall (This, 10);
  }
  MRead32 (Base + FCH_I2C_IC_CLR_INTR);
  MRead32 (Base + FCH_I2C_IC_CLR_TX_ABRT);

  //
  // Start Tx and Rx
  //
  while ((WriteLength + ReadLength) > 0) {
    if (TimeOut <= I2CReadTsc ()) {
      Status = EFI_TIMEOUT;
      DEBUG ((EFI_D_ERROR, "I2cTimeout for Tx:0x%x Rx:0x%X\n %x\n", WriteLength, ReadLength));
      goto ErrExit;
    }
    //Wait Rx Empty
    Data32 = (UINT32)(*((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_STATUS)));
    if ((Data32 & 0x8) != 0) {
      Stall (This, 10);
      continue;
    }
    //Wait Tx Fifo Empty
    Data32 = (UINT32)(*((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_TXFLR)));
    if (Data32 != 0) {
      Stall (This, 10);
      continue;
    }
    //Start Tx
    if (WriteLength > 1) {
      *((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_DATA_CMD)) = (UINT32)(*WriteBuffer);
      WriteLength --;
      WriteBuffer ++;
    } else if ((WriteLength == 1) && (ReadLength != 0)) { // If Write prepare end and Read Not End
      *((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_DATA_CMD)) = (UINT32)(*WriteBuffer);
      WriteLength --;
      WriteBuffer ++;
    } else if ((WriteLength == 1) && (ReadLength == 0)) { // If Write prepare end and Read is End
      *((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_DATA_CMD)) = (UINT32)(*WriteBuffer | 0x200);
      WriteLength --;
      WriteBuffer ++;
    } else if ((WriteLength == 0) && (ReadLength > 1)) { //If write is end and Read is Not End
      *((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_DATA_CMD)) = 0x100;
      ReadLength --;
      ReadsNeeded = TRUE;
    } else if ((WriteLength == 0) && (ReadLength == 1)) { //If write is end and Read prepare End
      *((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_DATA_CMD)) = 0x300;
      ReadLength --;
      ReadsNeeded = TRUE;
    }
    Stall (This, 10);
    Data32 = (UINT32)(*((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_RAW_INTR_STAT)));
    if (0 != (Data32 & BIT6)) {

      Status = EFI_DEVICE_ERROR;
      DEBUG ((EFI_D_ERROR, "I2C TX ABRT Transmit = %x\n", MRead32 (Base + FCH_I2C_IC_CLR_TX_ABRT)));
      goto ErrExit;
    }
    //
    // Wait Rx Fifo have Data and Read Rx
    //
    if (ReadsNeeded) {
      ReadsNeeded = FALSE;
      Data32 = (UINT32)(*((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_STATUS)));
      while ((Data32 & 0x8) != 0x8) {
        Data32 = (UINT32)(*((volatile UINT32*)(UINTN)(Base + FCH_I2C_IC_STATUS)));
        if (TimeOut <= I2CReadTsc ()) {
          Status = EFI_DEVICE_ERROR;
          DEBUG ((EFI_D_ERROR, "I2C Slave No Rx Out To Host\n"));
          goto ErrExit;
        }
      }
      *ReadBuffer = (UINT8)(*((volatile UINT8*)(UINTN)(Base + FCH_I2C_IC_DATA_CMD)));
      ReadBuffer ++;
      Stall (This, 10);
    }
  }
  //
  // Wait for I2C Idle
  //
  while (0x20 == (MRead32 (Base + FCH_I2C_IC_STATUS) & 0x20)) {
    if (TimeOut <= I2CReadTsc ()) {
      Status = EFI_TIMEOUT;
      DEBUG ((EFI_D_ERROR, "I2cTimeout waiting for I2c Idle: %x\n", MRead32 (Base + FCH_I2C_IC_STATUS)));
      goto ErrExit;
    }
    Stall (This, 10);
  }
ErrExit:
  //
  // Program 0 to IC_ENABLE to Disable I2C
  //
  MWrite32 (Base + FCH_I2C_IC_ENABLE, 0); // Disable the Controller
  while (0 != (MRead32 (Base + FCH_I2C_IC_ENABLE_STATUS) & 0x1)) {
    if (TimeOut <= I2CReadTsc ()) {
      DEBUG ((EFI_D_ERROR, "I2cTimeout waiting Disable End: %x\n", MRead32 (Base + FCH_I2C_IC_ENABLE_STATUS)));
      Status = EFI_TIMEOUT;
      break;
    }
    Stall (This, 10);
  }
  //
  // Restore I2C GPIO IOMux
  //
  MWrite8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + I2CConfig[Select].SclPin, SclMux);
  MWrite8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + I2CConfig[Select].SdaPin, SdaMux);
  if ((EFI_ERROR (Status) && (Status != EFI_INVALID_PARAMETER)) || IsDeviceIdeInEntry) {
    //
    // Do a power loss to I2C
    //
    MWrite8 (AoAc, 0x77);
    while ((MRead8 (AoAc+1) & 7) != 0) {
      if (TimeOut <= I2CReadTsc ()) {
        DEBUG ((EFI_D_ERROR, "I2cTimeout waiting for I2c D3: %x\n", MRead8 (AoAc+1)));
        return EFI_INVALID_PARAMETER;
      }
      Stall (This, 10);
    }
    //
    // Apply power to I2C
    //
    if (!IsDeviceIdeInEntry) {
      MWrite8 (AoAc, 0x7C);
      while ((MRead8 (AoAc+1) & 7) != 7) {
        if (TimeOut <= I2CReadTsc ()) {
          DEBUG ((EFI_D_ERROR, "I2cTimeout waiting for I2c D0: %x\n", MRead8 (AoAc+1)));
          return EFI_INVALID_PARAMETER;
        }
        Stall (This, 10);
      }
    }
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Execute I2C Read Bytes
 *
 *
 * @param[in]  This               The pointer of AMD CPM Table Ppi or Protocol.
 * @param[in]  Select             The I2C number. 0 = I2C0, 1 = I2C1, 2 = I2C2, 3 = I2C3, 4 = I2C4, 5 = I2C5
 * @param[in]  Address            The address of  device. 7bit device address.
 *                                Bit[2:0] = A2/A1/A0 HW selectable pin.
 *                                Bit[6:3] = HW fixed device adress.
 * @param[in]  Offset             The Offset of  register.
 * @param[in]  Length             The Length of Read Data.
 * @param[out] *Value             Data Pointer to save register value.
 *
 * @retval     EFI_SUCCESS, EFI_INVALID_PARAMETER, EFI_TIMEOUT or EFI_DEVICE_ERROR
 *
 */
EFI_STATUS
CpmReadI2C (
  IN       VOID                        *This,
  IN       UINT8                       Select,
  IN       UINT8                       Address,
  IN       UINT8                       Offset,
  IN       UINTN                       Length,
     OUT   UINT8                       *Value
  )
{
  EFI_STATUS         Status;
  UINTN              Index;
  if (Length > 0xFE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CpmReadWriteI2CBytes (This, Select, Address, Length, Value, 1, &Offset);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "I2C%d, Address = 0x%X, Offset = 0x%X, Length = 0x%X, Value = ", \
      Select, Address, Offset, Length));
    for (Index = 0; Index < Length; Index ++) {
      DEBUG ((DEBUG_INFO, "%02X ", Value[Index]));
    }
    DEBUG ((DEBUG_INFO, "Status = %r\n", Status));
  } else {
    DEBUG ((DEBUG_ERROR, "I2C%d, Address = 0x%X, Offset = 0x%X, Length = 0x%X, Status = %r\n", \
      Select, Address, Offset, Length, Status));
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Execute I2C Write Bytes
 *
 *
 * @param[in]  This               The pointer of AMD CPM Table Ppi or Protocol.
 * @param[in]  Select             The I2C number. 0 = I2C0, 1 = I2C1, 2 = I2C2, 3 = I2C3, 4 = I2C4, 5 = I2C5
 * @param[in]  Address            The address of I2C device. 7bit I2C device address.
 *                                Bit[2:0] = A2/A1/A0 HW selectable pin.
 *                                Bit[6:3] = HW fixed device adress.
 * @param[in]  Offset             The Offset of I2C register.
 * @param[in]  Length             The Length of Write Data.
 * @param[in]  *Value             Data Pointer to register value.
 *
 * @retval     EFI_SUCCESS, EFI_INVALID_PARAMETER, EFI_TIMEOUT or EFI_DEVICE_ERROR
 *
 */
EFI_STATUS
CpmWriteI2C (
  IN       VOID                        *This,
  IN       UINT8                       Select,
  IN       UINT8                       Address,
  IN       UINT8                       Offset,
  IN       UINTN                       Length,
  IN       UINT8                       *Value
  )
{
  EFI_STATUS  Status;
  UINT8       Data[0xff];
  UINT8       Index;
  if (Length > 0xFE) {
    return EFI_INVALID_PARAMETER;
  }
  Data[0] = Offset;
  for (Index = 0; Index < Length; Index ++) {
    Data[Index+1] = Value[Index];
  }
  Status = CpmReadWriteI2CBytes (This, Select, Address, 0, NULL, Length+1, Data);
  DEBUG ((DEBUG_INFO, "I2C%d, Address = 0x%X, Offset = 0x%X, Length = 0x%X, Status = %r", \
    Select, Address, Offset, Length, Status));
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Execute Smbus Read
 *
 *
 * @param[in]  This               The pointer of AMD CPM Table Ppi or Protocol.
 * @param[in]  Select             The Smbus number.
 *                                0 = Smbus0, 1 = Smbus1.
 * @param[in]  Address            The address of Smbus device. 7bit Smbus device address.
 *                                Bit[2:0] = A2/A1/A0 HW selectable pin.
 *                                Bit[6:3] = HW fixed device adress.
 * @param[in]  Offset             The Offset of Smbus register.
 * @param[in]  Length             The Length of Read Data.
 * @param[out] *Value             Data Pointer to save register value.
 *
 * @retval     EFI_SUCCESS, EFI_INVALID_PARAMETER, EFI_TIMEOUT or EFI_DEVICE_ERROR
 *
 */
EFI_STATUS
CpmReadSmbus (
  IN       VOID                        *This,
  IN       UINT8                       Select,
  IN       UINT8                       Address,
  IN       UINT8                       Offset,
  IN       UINTN                       Length,
     OUT   UINT8                       *Value
  )
{
  EFI_STATUS              Status;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  BOOLEAN                 IsBusy;
  UINT16                  SmbusIoBase;
  UINT16                  LoopCount;
  UINT8                   retryCount;
  UINT8                   bValue;
  UINT8                   StsReg;
  UINT8                   SmbusOperation;
  UINT8                   SlvAddrReg;
  UINT8                   HostCmdReg;
  UINT8                   *CallBuffer;
  UINT8                   Index;
  UINT8                   BlockCount;
  UINT8                   AsfSmMasterEn;
  UINT8                   AsfPecEnable;

  MainTablePtr      = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  Status            = EFI_SUCCESS;
  CallBuffer        = Value;
  BlockCount        = 0;
  AsfSmMasterEn     = 0;
  AsfPecEnable      = 0;

  // Initialize variables
  // Process Operation
  if ((Length > 2) && (Length <= 72)) {
    SmbusOperation  = SMB_CMD_BLOCK;
  } else if (Length == 2) {
    SmbusOperation  = SMB_CMD_WORD_DATA;
  } else if (Length == 1) {
    SmbusOperation  = SMB_CMD_BYTE_DATA;
  } else {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-0-INVALID PARAMETER\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  SlvAddrReg      = (UINT8) ((Address << 1) | SMBUS_READ_ENABLE);
  HostCmdReg      = (UINT8) Offset;

  if (!MainTablePtr->SmbusIoBaseAddr) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-0-ERROR-SmbusIoBaseAddr = 0x%04X\n", __FUNCTION__, MainTablePtr->SmbusIoBaseAddr));
    return EFI_INVALID_PARAMETER;
  } else {
    // Get SMBus IO base address from MainTablePtr->SmbusIoBaseAddr
    SmbusIoBase = MainTablePtr->SmbusIoBaseAddr;
    if (Select == 1) {
      SmbusIoBase += 0x20;
    }
  }
  AsfSmMasterEn = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02));
  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn | BIT0);
  if (Select == 1) {
    AsfPecEnable = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG02);
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable & ~BIT7);
  }

  // Keep polling until host gets SMBUS control
  LoopCount = 100;
  do {
    bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG08);
    // Set HostSemaphore
    bValue |= BIT4;
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG08, bValue);
    bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
    bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
    bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG08);
    LoopCount--;
    if (LoopCount == 0) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-1-TIMEOUT-Set HostSemaphore Fail\n", __FUNCTION__));
      if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
      CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
      return EFI_TIMEOUT;
    }
  } while ((bValue & BIT4) == 0); // Loop until semaphore has established

  // Clear host status and wait until host is idle
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      // Clear all status bits
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, SMB_ALL_HOST_STATUS);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG00);
      if ( ! ( bValue & BIT0) ) {
        // Bit0 = 0, controller is idle
        IsBusy = FALSE;
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    // Set [1] = Kill current host transaction
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, BIT1);
  }
  if (IsBusy) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-2-TIMEOUT-Host is Busy\n", __FUNCTION__));
    if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
    return EFI_TIMEOUT;
  }

  // Clear slave status and wait until slave is idle
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      // Clear SMB_ALL_SLAVE_STATUS
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG01, SMB_ALL_SLAVE_STATUS);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG01);
      if ( ! ( bValue & BIT0) ) {
        // Bit0 = 0, slave is idle
        IsBusy = FALSE;
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    // Set BIT1 = SlaveInit
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG01, BIT1);
  }
  if (IsBusy) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-3-TIMEOUT-Slave is Busy\n", __FUNCTION__));
    if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
    return EFI_TIMEOUT;
  }

  for (retryCount = 0; retryCount < 3; retryCount++) {
    // Clear all status Bits
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, SMB_ALL_HOST_STATUS);

    // Set SMBus slave address for the device to send/receive from
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG04, SlvAddrReg);

    // Set Command register
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG03, HostCmdReg);

    // Select SMBus protocol
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, SmbusOperation);

    //
    // Do a read from Reg02 to reset the counter if it's going to be a
    // block read/write operation
    //
    StsReg = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG02);

    // Read Block
    if (Length > 2) {
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG05, (UINT8) Length);
    }

    // Set Control Register (Initiate Operation, Interrupt disabled)
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, SmbusOperation + BIT6); // [6] = Start

    // Wait for controller IO to finish work
    do {
      LoopCount = 1000;
      do {
        StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
        StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
        StsReg = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG00);
        if ( ! ( StsReg & BIT0) ) {
          // Bit0 = 0, Host is not busy
          break;
        }
        LoopCount--;
      } while (LoopCount);
      if ( LoopCount == 0) {
        // Set [1] = Kill current host transaction
        CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, BIT1);
        DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-4-TIMEOUT-StsReg = 0x%X\n", __FUNCTION__, StsReg));
        if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
        CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
        return EFI_TIMEOUT;
      }
      StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      StsReg = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG00);
    } while ((StsReg & (BIT3 + BIT2 + BIT1)) == 0);

    if (StsReg & BIT2) {
      Status = EFI_DEVICE_ERROR;
      break;
    } else if (StsReg & BIT3) {
      Status = EFI_DEVICE_ERROR;
      // Clear bus error for another try
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, BIT3);
      continue;
    } else {
      //
      // Successful completion
      //
      if (SmbusOperation == SMB_CMD_BLOCK) {
        BlockCount = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG05);
        if (Length < (UINTN) BlockCount) {
          BlockCount = (UINT8) Length;
        }
        for (Index = 0; Index < BlockCount; Index++) {
          CallBuffer[Index] = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG07);
        }
      } else if (SmbusOperation == SMB_CMD_WORD_DATA) {
        // Read a byte from Host data 1 & 0
        CallBuffer[1] = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG06);
        CallBuffer[0] = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG05);
      } else {
        // Read a byte from Host data 0
        CallBuffer[0] = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG05);
      }
      break;
    }
  }

  // Clear host controller status
  CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, SMB_ALL_HOST_STATUS);

  // Release SMBUS ownership
  bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG08);
  // Clear Host Semaphore
  bValue |= BIT5;
  CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG08, bValue);

  if (StsReg & BIT2) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-5-DEVICE_ERROR-StsReg = 0x%X\n", __FUNCTION__, StsReg));
  } else if (StsReg & BIT3) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-6-BUS_ERROR-StsReg = 0x%X\n", __FUNCTION__, StsReg));
  } else {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-7-SUCCESS", __FUNCTION__));
    DEBUG((DEBUG_INFO, "-Smbus = %d, Address = 0x%X, Offset = 0x%X, Length = 0x%X, Value =", Select, Address << 1, Offset, Length));
    for (Index = 0; Index < (UINT8) Length; Index++) {
      DEBUG((DEBUG_INFO, " 0x%02X", CallBuffer[Index]));
      if (((Index & 0xF) == 0xF) || ((Index + 1) == (UINT8) Length)) {
        DEBUG((DEBUG_INFO, "\n"));
      }
    }
  }
  if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Execute Smbus Write
 *
 *
 * @param[in]  This               The pointer of AMD CPM Table Ppi or Protocol.
 * @param[in]  Select             The Smbus number.
 *                                0 = Smbus0, 1 = Smbus1.
 * @param[in]  Address            The address of Smbus device. 7bit Smbus device address.
 *                                Bit[2:0] = A2/A1/A0 HW selectable pin.
 *                                Bit[6:3] = HW fixed device adress.
 * @param[in]  Offset             The Offset of Smbus register.
 * @param[in]  Length             The Length of Write Data.
 * @param[in]  *Value             Data Pointer to register value.
 *
 * @retval     EFI_SUCCESS, EFI_INVALID_PARAMETER, EFI_TIMEOUT or EFI_DEVICE_ERROR
 *
 */
EFI_STATUS
CpmWriteSmbus (
  IN       VOID                        *This,
  IN       UINT8                       Select,
  IN       UINT8                       Address,
  IN       UINT8                       Offset,
  IN       UINTN                       Length,
  IN       UINT8                       *Value
  )
{
  EFI_STATUS              Status;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  BOOLEAN                 IsBusy;
  UINT16                  SmbusIoBase;
  UINT16                  LoopCount;
  UINT8                   retryCount;
  UINT8                   bValue;
  UINT8                   StsReg;
  UINT8                   SmbusOperation;
  UINT8                   SlvAddrReg;
  UINT8                   HostCmdReg;
  UINT8                   *CallBuffer;
  UINT8                   Index;
  UINT8                   AsfSmMasterEn;
  UINT8                   AsfPecEnable;

  MainTablePtr      = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  Status            = EFI_SUCCESS;
  CallBuffer        = Value;
  AsfSmMasterEn     = 0;
  AsfPecEnable      = 0;

  // Initialize variables
  // Process Operation
  if ((Length > 2) && (Length <= 72)) {
    SmbusOperation  = SMB_CMD_BLOCK;
  } else if (Length == 2) {
    SmbusOperation  = SMB_CMD_WORD_DATA;
  } else if (Length == 1) {
    SmbusOperation  = SMB_CMD_BYTE_DATA;
  } else {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-0-INVALID PARAMETER\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  SlvAddrReg      = (UINT8) (Address << 1);
  HostCmdReg      = (UINT8) Offset;

  if (!MainTablePtr->SmbusIoBaseAddr) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-0-ERROR-SmbusIoBaseAddr = 0x%04X\n", __FUNCTION__, MainTablePtr->SmbusIoBaseAddr));
    return EFI_INVALID_PARAMETER;
  } else {
    // Get SMBus IO base address from MainTablePtr->SmbusIoBaseAddr
    SmbusIoBase = MainTablePtr->SmbusIoBaseAddr;
    if (Select == 1) {
      SmbusIoBase += 0x20;
    }
  }
  AsfSmMasterEn = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02));
  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn | BIT0);
  if (Select == 1) {
    AsfPecEnable = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG02);
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable & ~BIT7);
  }

  // Keep polling until host gets SMBUS control
  LoopCount = 100;
  do {
    bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG08);
    // Set HostSemaphore
    bValue |= BIT4;
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG08, bValue);
    bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
    bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
    bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG08);
    LoopCount--;
    if (LoopCount == 0) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-1-TIMEOUT-Set HostSemaphore Fail\n", __FUNCTION__));
      if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
      CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
      return EFI_TIMEOUT;
    }
  } while ((bValue & BIT4) == 0); // Loop until semaphore has established

  // Clear host status and wait until host is idle
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      // Clear all status bits
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, SMB_ALL_HOST_STATUS);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG00);
      if ( ! ( bValue & BIT0) ) {
        // Bit0 = 0, controller is idle
        IsBusy = FALSE;
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    // Set [1] = Kill current host transaction
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, BIT1);
  }
  if (IsBusy) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-2-TIMEOUT-Host is Busy\n", __FUNCTION__));
    if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
    return EFI_TIMEOUT;
  }

  // Clear slave status and wait until slave is idle
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      // Clear SMB_ALL_SLAVE_STATUS
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG01, SMB_ALL_SLAVE_STATUS);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG01);
      if ( ! ( bValue & BIT0) ) {
        // Bit0 = 0, slave is idle
        IsBusy = FALSE;
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    // Set BIT1 = SlaveInit
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG01, BIT1);
  }
  if (IsBusy) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-3-TIMEOUT-Slave is Busy\n", __FUNCTION__));
    if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
    return EFI_TIMEOUT;
  }

  for (retryCount = 0; retryCount < 3; retryCount++) {
    // Clear all status Bits
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, SMB_ALL_HOST_STATUS);

    // Set SMBus slave address for the device to send/receive from
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG04, SlvAddrReg);

    // Set Command register
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG03, HostCmdReg);

    // Select SMBus protocol
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, SmbusOperation);

    //
    // Do a read from Reg02 to reset the counter if it's going to be a
    // block read/write operation
    //
    StsReg = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG02);

    // Write Block
    if (SmbusOperation == SMB_CMD_BLOCK) {
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG05, (UINT8) Length);
      for (Index = 0; Index < (UINT8) Length; Index++) {
        CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG07, CallBuffer[Index]);
      }
    } else if (SmbusOperation == SMB_CMD_WORD_DATA) {
      // Write byte to Host data 1 & 0
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG06, CallBuffer[1]);
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG05, CallBuffer[0]);
    } else {
      // Write byte to Host data 0
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG05, CallBuffer[0]);
    }

    // Set Control Register (Initiate Operation, Interrupt disabled)
    CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, SmbusOperation + BIT6); // [6] = Start

    // Wait for controller IO to finish work
    do {
      LoopCount = 1000;
      do {
        StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
        StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
        StsReg = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG00);
        if ( ! ( StsReg & BIT0) ) {
          // Bit0 = 0, Host is not busy
          break;
        }
        LoopCount--;
      } while (LoopCount);
      if ( LoopCount == 0) {
        // Set [1] = Kill current host transaction
        CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, BIT1);
        DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-4-TIMEOUT-StsReg = 0x%X\n", __FUNCTION__, StsReg));
        if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
        CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
        return EFI_TIMEOUT;
      }
      StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      StsReg = CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      StsReg = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG00);
    } while ((StsReg & (BIT3 + BIT2 + BIT1)) == 0);

    if (StsReg & BIT2) {
      Status = EFI_DEVICE_ERROR;
      break;
    } else if (StsReg & BIT3) {
      Status = EFI_DEVICE_ERROR;
      // Clear bus error for another try
      CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, BIT3);
      continue;
    } else {
      //
      // Successful completion
      //
      break;
    }
  }

  // Clear host controller status
  CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG00, SMB_ALL_HOST_STATUS);

  // Release SMBUS ownership
  bValue = CommonFunctionPtr->IoRead8 (SmbusIoBase + SMB_IOREG08);
  // Clear Host Semaphore
  bValue |= BIT5;
  CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG08, bValue);

  if (StsReg & BIT2) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-5-DEVICE_ERROR-StsReg = 0x%X\n", __FUNCTION__, StsReg));
  } else if (StsReg & BIT3) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-6-BUS_ERROR-StsReg = 0x%X\n", __FUNCTION__, StsReg));
  } else {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-7-SUCCESS", __FUNCTION__));
    DEBUG((DEBUG_INFO, "-Smbus = %d, Address = 0x%X, Offset = 0x%X, Length = 0x%X, Value =", Select, Address << 1, Offset, Length));
    for (Index = 0; Index < (UINT8) Length; Index++) {
      DEBUG((DEBUG_INFO, " 0x%02X", CallBuffer[Index]));
      if (((Index & 0xF) == 0xF) || ((Index + 1) == (UINT8) Length)) {
        DEBUG((DEBUG_INFO, "\n"));
      }
    }
  }
  if (Select == 1) CommonFunctionPtr->IoWrite8 (SmbusIoBase + SMB_IOREG02, AsfPecEnable);
  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x02), AsfSmMasterEn);
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of FCH GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmFchGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   Value;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  Value = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    Value = ((CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + GPIO_BASE + Pin)) & 0x80) == 0x80);
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
  case CPM_FCH_REVISION_ID_NL:
  case CPM_FCH_REVISION_ID_AM:
  case CPM_FCH_REVISION_ID_ST:
  case CPM_FCH_REVISION_ID_BR:
  case CPM_FCH_REVISION_ID_ZP:
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_MTS:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_VN:
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
  case CPM_FCH_REVISION_ID_STP:
    if (Pin < 0x100) {
      Value = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2)) & 0x01;
    } else {
      Value = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + REMOTE_GPIO_BANK_BASE + ((Pin & 0xFF) << 2) + 2)) & 0x01;
    }
    break;
  }
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-Pin = GPIO%d, PinStatus = 0x%02X\n", __FUNCTION__, Pin, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Bixby GPIO & Remote GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket  The Socket Number.
 * @param[in] Die     The Die Number.
 * @param[in] Bridge  The Bridge Number.
 * @param[in] Pin     The GPIO Pin Number.
 *
 * @retval            The State of GPIO Pin.
 *
 */
UINT8
CpmBxbGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Socket,
  IN       UINT16                      Die,
  IN       UINT16                      Bridge,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   Value;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  Value = 0;

  if (Pin < 0x100) {
    Value = CommonFunctionPtr->SmnBxbRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2)) & 0x01;
  } else {
    Value = CommonFunctionPtr->SmnBxbRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2)) & 0x01;
  }
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-Socket = 0x%04X Die = 0x%04X Bridge = 0x%04X", __FUNCTION__, Socket, Die, Bridge));
  DEBUG((DEBUG_INFO, " Pin = GPIO%d PinStatus = %X\n", Pin, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of ECRAM GPIO Pin on reference board
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmEcRamGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   Data;
  UINT8                   PinNum;
  UINT8                   Offset;
  EFI_STATUS              Status;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  PinNum              = (UINT8)Pin;
  Offset              = CFG_CPM_ECRAM_GPIO_OFFSET + 1 + (PinNum >> 3);
  if (MainTablePtr->EcSupport) {
    Status = CommonFunctionPtr->KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Data);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-CPM_DEADLOOP-Pin = GPIO%d\n", __FUNCTION__, Pin));
      CPM_DEADLOOP ();
    }
  } else {
    Data = 0xFF;
  }
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-Pin = GPIO%d, PinStatus = %X\n", __FUNCTION__, Pin, ((Data >> (PinNum & 0x07)) & 0x01)));
  return ((Data >> (PinNum & 0x07)) & 0x01);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of Promontory GPIO Pin on reference board
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmPtGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT32                  BridgeAddress;
  UINT32                  DeviceAddress;
  UINT32                  PtGpioMmio;
  UINT8                   Bus;
  UINT8                   BusSec;
  UINT8                   BridgeCommand;
  UINT8                   DeviceCommand;
  UINT8                   Data;

  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr      = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  BridgeAddress     = (UINT32)MainTablePtr->PtBrMemIoBaseAddr;

  Data = 0;
  if (BridgeAddress != 0) {
    Bus = CommonFunctionPtr->MmioRead8 (BridgeAddress + 0x19);              // PT Bridge Offset 0x19
    BusSec = CommonFunctionPtr->MmioRead8 (BridgeAddress + 0x1A);           // PT Bridge Offset 0x1A

    if (Bus != 0xFF) {
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x19, PtTempBus);      // PT Bridge Offset 0x19
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x1A, PtTempBus);      // PT Bridge Offset 0x1A
      DeviceAddress = (UINT32)(MainTablePtr->PcieMemIoBaseAddr + (PtTempBus << 20) + (PtGpioFun << 12));

      BridgeCommand = CommonFunctionPtr->MmioRead8 (BridgeAddress + 0x04);  // PT Bridge Offset 0x04
      DeviceCommand = CommonFunctionPtr->MmioRead8 (DeviceAddress + 0x04);  // PT GPIO Device Offset 0x04

      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x04, 0x07);           // PT Bridge Offset 0x04
      CommonFunctionPtr->MmioWrite8 (DeviceAddress + 0x04, 0x06);           // PT GPIO Device Offset 0x04

      PtGpioMmio = CommonFunctionPtr->MmioRead32 (DeviceAddress + 0x40) & ~(UINT32)BIT2;    // PT GPIO Device Offset 0x40

      Data = CommonFunctionPtr->MmioRead8 (PtGpioMmio);                                     // PT GPIO MMIO Offset 0x00
      if ((Data & (UINT8)(1 << Pin)) == (UINT8)(1 << Pin)) {
        CommonFunctionPtr->MmioWrite8 (PtGpioMmio, (Data & ~(UINT8)(1 << Pin)));            // PT GPIO MMIO Offset 0x00
      }

      Data = (CommonFunctionPtr->MmioRead8 (PtGpioMmio + 0x04) & (UINT8)(1 << Pin)) >> Pin; // PT GPIO MMIO Offset 0x04

      CommonFunctionPtr->MmioWrite8 (DeviceAddress + 0x04, DeviceCommand);  // PT GPIO Device Offset 0x04
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x04, BridgeCommand);  // PT Bridge Offset 0x04
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x19, Bus);            // PT Bridge Offset 0x19
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x1A, BusSec);         // PT Bridge Offset 0x1A
    }
  }

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-BridgeAddress = 0x%08X Pin = 0x%04X Data = 0x%02X\n", __FUNCTION__, BridgeAddress, Pin, Data));
  return Data;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Multi Socket/Die FCH GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket  The Socket Number.
 * @param[in] Die     The Die Number.
 * @param[in] Bridge  The Bridge Number.
 * @param[in] Pin     The GPIO Pin Number.
 *
 * @retval            The State of GPIO Pin.
 *
 */
UINT8
CpmSmnGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Socket,
  IN       UINT16                      Die,
  IN       UINT16                      Bridge,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   Value;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  Value = 0;

  if (Pin < 0x100) {
    Value = CommonFunctionPtr->SmnRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2)) & 0x01;
  } else {
    Value = CommonFunctionPtr->SmnRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2)) & 0x01;
  }
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-Socket = 0x%04X Die = 0x%04X Bridge = 0x%04X", __FUNCTION__, Socket, Die, Bridge));
  DEBUG((DEBUG_INFO, " Pin = GPIO%d PinStatus = %X\n", Pin, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get SMBUS GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @param[in] Pin:
 * SMBUS GPIO Setting for BIT[15:8] = 0x06.
 *                    BIT[31]:            The Smbus number. 0 = Smbus0, 1 = Smbus1.
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:16]:         Reserved
 * GPIO Controller Selection.
 *                    BIT[15:8] = 0x06:   SMBUS IO Expander GPIO Pin
 * SMBUS GPIO Pin Number for BIT[15:8] = 0x06.
 *                    BIT[7:6] = 0x0:     SMBUS IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     SMBUS IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     SMBUS IO Expander Port 0
 *                    BIT[5:4] = 0x1:     SMBUS IO Expander Port 1
 *                    BIT[3:0]:           SMBUS GPIO Pin Number
 *
 * @retval            The State of GPIO Pin.
 *
 */
UINT8
CpmSmbusGetGpio (
  IN       VOID                        *This,
  IN       UINT32                      Pin
  )
{
  EFI_STATUS              Status;
  UINT8                   Data;
  UINT8                   Select;
  UINT8                   Address;
  UINT8                   Chip;
  UINT8                   Port;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  Select = (UINT8) ((Pin >> 31) & 1);
  Address = (UINT8) ((Pin >> 24) & 0x7F);
  Chip = (UINT8) ((Pin >> 6) & 3);
  Port = (UINT8) ((Pin >> 4) & 3);

  Status = CpmReadSmbus (This, Select, Address, 0 + Port, 1, &Data);        // Read Input port register (00h)
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Data >>= (UINT8) (Pin & 0x7);
  Data &= BIT0;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Smbus%d IO Expander %s Port%d", __FUNCTION__, Select, (Chip == 0) ? L"8-bit" : L"16-bit", Port));
  DEBUG((DEBUG_INFO, ", Address = 0x%02X, Pin = GPIO%d, PinStatus = %d\n", Address << 1, (Pin & 0x7), Data));
  return Data;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get I2C GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @param[in] Pin:
 * I2C GPIO Setting for BIT[15:8] = 0x07.
 *                    BIT[31]:            Reserved.
 *                    BIT[30:27]:         Fixed address of Device Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:21]:         The I2C number. 0 = I2C0, 1 = I2C1, 2 = I2C2, 3 = I2C3, 4 = I2C4, 5 = I2C5
 *                    BIT[20:16]:         Reserved
 * GPIO Controller Selection.
 *                    BIT[15:8] = 0x07:   I2C IO Expander GPIO Pin
 * I2C GPIO Pin Number for BIT[15:8] = 0x07.
 *                    BIT[7:6] = 0x0:     I2C IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     I2C IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     I2C IO Expander Port 0
 *                    BIT[5:4] = 0x1:     I2C IO Expander Port 1
 *                    BIT[3:0]:           I2C GPIO Pin Number
 *
 * @retval            The State of GPIO Pin.
 *
 */
UINT8
CpmI2CGetGpio (
  IN       VOID                        *This,
  IN       UINT32                      Pin
  )
{
  EFI_STATUS              Status;
  UINT8                   Data;
  UINT8                   Select;
  UINT8                   Address;
  UINT8                   Chip;
  UINT8                   Port;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  Select = (UINT8) ((Pin >> 21) & 7);
  Address = (UINT8) ((Pin >> 24) & 0x7F);
  Chip = (UINT8) ((Pin >> 6) & 3);
  Port = (UINT8) ((Pin >> 4) & 3);

  Status = CpmReadI2C (This, Select, Address, 0 + Port, 1, &Data);        // Read Input port register (00h)
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Data >>= (UINT8) (Pin & 0x7);
  Data &= BIT0;

  DEBUG ((DEBUG_INFO, "  LIB-FCH-%a-I2C%d IO Expander %s Port%d", \
    __FUNCTION__, Select, (Chip == 0) ? L"8-bit" : L"16-bit", Port));
  DEBUG((DEBUG_INFO, ", Address = 0x%02X, Pin = GPIO%d, PinStatus = %d\n", Address << 1, (Pin & 0x7), Data));
  return Data;
}
/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of GPIO Pin
 *
 *
 * @param[in] This:   The pointer of AMD CPM Table Ppi or Protocol
 *
 * @param[in] Pin:
 * FCH/Bixby GPIO/Remote GPIO setting for BIT[15:8] = 0x00/0x01/0x04/0x05.
 *                    BIT[31]:            Multi Socket Number
 *                    BIT[30:29]:         Multi Die Number
 *                    BIT[28:27]:         Multi Bridge Number
 *                    BIT[26:16]:         Reserved
 * SMBUS GPIO Setting for BIT[15:8] = 0x06.
 *                    BIT[31]:            The Smbus number. 0 = Smbus0, 1 = Smbus1.
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:16]:         Reserved
 * I2C GPIO Setting for BIT[15:8] = 0x07.
 *                    BIT[31]:            Reserved
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:21]:         The I2C number. 0 = I2C0, 1 = I2C1, 2 = I2C2, 3 = I2C3, 4 = I2C4, 5 = I2C5
 *                    BIT[20:16]:         Reserved
 * GPIO Controller Selection.
 *                    BIT[15:8] = 0x00:   FCH GPIO Pin
 *                    BIT[15:8] = 0x01:   Remote FCH GPIO Pin
 *                    BIT[15:8] = 0x02:   ECRAM GPIO Pin on Reference Board
 *                    BIT[15:8] = 0x03:   Promontory GPIO Pin
 *                    BIT[15:8] = 0x04:   Bixby GPIO Pin
 *                    BIT[15:8] = 0x05:   Bixby Remote GPIO Pin
 *                    BIT[15:8] = 0x06:   SMBUS IO Expander GPIO Pin
 *                    BIT[15:8] = 0x07:   I2C IO Expander GPIO Pin
 *                    BIT[15:8] > 0x06:   Reserved
 * Non-SMBUS GPIO Pin Number for BIT[15:8] = 0x00 ~ 0x05
 *                    BIT[7:0]:           GPIO Pin Number
 * SMBUS GPIO Pin Number for BIT[15:8] = 0x06.
 *                    BIT[7:6] = 0x0:     SMBUS IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     SMBUS IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     SMBUS IO Expander Port 0
 *                    BIT[5:4] = 0x1:     SMBUS IO Expander Port 1
 *                    BIT[3:0]:           GPIO Pin Number
 * I2C GPIO Pin Number for BIT[15:8] = 0x07.
 *                    BIT[7:6] = 0x0:     I2C IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     I2C IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     I2C IO Expander Port 0
 *                    BIT[5:4] = 0x1:     I2C IO Expander Port 1
 *                    BIT[3:0]:           GPIO Pin Number
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmGetGpio (
  IN       VOID                        *This,
  IN       UINT32                      Pin
  )
{
  UINT16  Select;
  UINT16  PinNum;
  UINT8   Value;

  Select = (UINT16) ((Pin >> 8) & 0x7);
  PinNum = (UINT16) (Pin & 0xFF);
  Value = 0;
  switch (Select) {
  case 0:
  case 1:
    if ((Pin & 0xF8000000) == 0) {
      Value = CpmFchGetGpio (This, (UINT16) (Pin & 0x1FF));
    } else {
      Value = CpmSmnGetGpio (This, (UINT16) ((Pin & 0x80000000) >> 31), (UINT16) ((Pin & 0x60000000) >> 29), (UINT16) ((Pin & 0x18000000) >> 27), (UINT16) (Pin & 0x1FF));
    }
    break;
  case 4:
  case 5:
    Value = CpmBxbGetGpio (This, (UINT16) ((Pin & 0x80000000) >> 31), (UINT16) ((Pin & 0x60000000) >> 29), (UINT16) ((Pin & 0x18000000) >> 27), (UINT16) (Pin & 0x1FF));
    break;
  case 2:
    Value = CpmEcRamGetGpio (This, PinNum);
    break;
  case 3:
    Value = CpmPtGetGpio (This, PinNum);
    break;
  case 6:
    Value = CpmSmbusGetGpio (This, Pin);
    break;
  case 7:
    Value = CpmI2CGetGpio (This, Pin);
    break;
  }
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set FCH GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. See Definition of AMD_CPM_GPIO_SETTING
 *
 */
VOID
CpmFchSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GPIO_SETTING    GpioPin;
  UINT8                   temp;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-Pin = GPIO%d, Value = 0x%04X\n", Pin, Value));
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  GpioPin.Raw = Value;

  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    temp = CommonFunctionPtr->MmioRead8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BASE + Pin));
    if (GpioPin.Gpio.PresetEn) {
      temp = (temp & 0xE3) | 0x08;
      if (GpioPin.Gpio.Sticky) {
        temp |= 0x04;
      }
      if (GpioPin.Gpio.PullUp) {
        temp &= 0xF7;
      }
      if (GpioPin.Gpio.PullDown) {
        temp |= 0x10;
      }
    }
    if (GpioPin.Gpio.SetEnB == 0) {
      if (GpioPin.Gpio.OutEnB == 0) {
        temp &= 0x9F;
        if (GpioPin.Gpio.Out) {
          temp |= 0x40;
        }
      } else {
        temp |= 0x20;
      }
    }
    CommonFunctionPtr->MmioWrite8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BASE + Pin), temp);
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
  case CPM_FCH_REVISION_ID_NL:
  case CPM_FCH_REVISION_ID_AM:
  case CPM_FCH_REVISION_ID_ST:
  case CPM_FCH_REVISION_ID_BR:
  case CPM_FCH_REVISION_ID_ZP:
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_MTS:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_VN:
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
  case CPM_FCH_REVISION_ID_STP:
    if (Pin < 0x100) {
      temp = CommonFunctionPtr->MmioRead8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2));
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a MmioRead8 (0x%08X) = 0x%02X\n", __FUNCTION__, (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2), temp));
    } else {
      temp = CommonFunctionPtr->MmioRead8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + REMOTE_GPIO_BANK_BASE + ((Pin & 0xFF) << 2) + 2));
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a MmioRead8 (0x%08X) = 0x%02X\n", __FUNCTION__, (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + REMOTE_GPIO_BANK_BASE + ((Pin & 0xFF) << 2) + 2), temp));
    }
    if (GpioPin.Gpio.PresetEn) {
      temp = temp & 0xC7;
      if (GpioPin.Gpio.PullUpSel) {
        temp |= 0x08;
      }
      if (GpioPin.Gpio.PullUp) {
        temp |= 0x10;
      }
      if (GpioPin.Gpio.PullDown) {
        temp |= 0x20;
      }
    }
    if (GpioPin.Gpio.SetEnB == 0) {
      if (GpioPin.Gpio.OutEnB == 0) {
        temp |= 0x80;
        if (GpioPin.Gpio.Out) {
          temp |= 0x40;
        } else {
          temp &= 0xBF;
        }
      } else {
        temp &= 0x7F;
      }
    }
    if (Pin < 0x100) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a MmioWrite8 (0x%08X, 0x%02X)\n", __FUNCTION__, (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2), temp));
      CommonFunctionPtr->MmioWrite8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2), temp);
    } else {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a MmioWrite8 (0x%08X, 0x%02X)\n", __FUNCTION__, (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + REMOTE_GPIO_BANK_BASE + ((Pin & 0xFF) << 2) + 2), temp));
      CommonFunctionPtr->MmioWrite8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + REMOTE_GPIO_BANK_BASE + ((Pin & 0xFF) << 2) + 2), temp);
    }
    break;
  }

  if (GpioPin.Gpio.IoMuxEn) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-GpioPin.Gpio.IoMuxEn = 0x%02X, GpioPin.Gpio.IoMux = 0x%02X\n", __FUNCTION__, (UINT8) GpioPin.Gpio.IoMuxEn, (UINT8) GpioPin.Gpio.IoMux));
    if (Pin < 0x100) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a MmioWrite8 (0x%08X, 0x%02X)\n", __FUNCTION__, (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + Pin), (UINT8) GpioPin.Gpio.IoMux));
      CommonFunctionPtr->MmioWrite8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + Pin), (UINT8) GpioPin.Gpio.IoMux);
    } else {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a MmioWrite8 (0x%08X, 0x%02X)\n", __FUNCTION__, (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + REMOTE_IOMUX_BASE + (Pin & 0xFF)), (UINT8) GpioPin.Gpio.IoMux));
      CommonFunctionPtr->MmioWrite8 ((UINT32) (MainTablePtr->AcpiMemIoBaseAddr + REMOTE_IOMUX_BASE + (Pin & 0xFF)), (UINT8) GpioPin.Gpio.IoMux);
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set Bixby GPIO & Remote GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket  The Socket Number.
 * @param[in] Die     The Die Number.
 * @param[in] Bridge  The Bridge Number.
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. See Definition of AMD_CPM_GPIO_SETTING
 *
 */
VOID
CpmBxbSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Socket,
  IN       UINT16                      Die,
  IN       UINT16                      Bridge,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GPIO_SETTING    GpioPin;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   temp;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-Socket = 0x%04X, Die = 0x%04X, Bridge = 0x%04X, Pin = GPIO%d, Value = 0x%04X\n", Socket, Die, Bridge, Pin, Value));
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  GpioPin.Raw = Value;

  if (Pin < 0x100) {
    temp = CommonFunctionPtr->SmnBxbRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2));
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnBxbRead8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X) = 0x%02X\n", (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2), temp));
  } else {
    temp = CommonFunctionPtr->SmnBxbRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2));
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnBxbRead8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X) = 0x%02X\n", (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2), temp));
  }
  if (GpioPin.Gpio.PresetEn) {
    temp = temp & 0xC7;
    if (GpioPin.Gpio.PullUpSel) {
      temp |= 0x08;
    }
    if (GpioPin.Gpio.PullUp) {
      temp |= 0x10;
    }
    if (GpioPin.Gpio.PullDown) {
      temp |= 0x20;
    }
  }
  if (GpioPin.Gpio.SetEnB == 0) {
    if (GpioPin.Gpio.OutEnB == 0) {
      temp |= 0x80;
      if (GpioPin.Gpio.Out) {
        temp |= 0x40;
      } else {
        temp &= 0xBF;
      }
    } else {
      temp &= 0x7F;
    }
  }
  if (Pin < 0x100) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnBxbWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2), temp));
    CommonFunctionPtr->SmnBxbWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2), temp);
  } else {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnBxbWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2), temp));
    CommonFunctionPtr->SmnBxbWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2), temp);
  }

  if (GpioPin.Gpio.IoMuxEn) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-GpioPin.Gpio.IoMuxEn = 0x%02X, GpioPin.Gpio.IoMux = 0x%02X\n", __FUNCTION__, (UINT8) GpioPin.Gpio.IoMuxEn, (UINT8) GpioPin.Gpio.IoMux));
    if (Pin < 0x100) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnBxbWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
      DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnIomux, (UINT32) Pin, (UINT8) GpioPin.Gpio.IoMux));
      CommonFunctionPtr->SmnBxbWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnIomux, (UINT32) Pin, (UINT8) GpioPin.Gpio.IoMux);
    } else {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnBxbWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
      DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnRemoteIomux, (UINT32) (Pin & 0xFF), (UINT8) GpioPin.Gpio.IoMux));
      CommonFunctionPtr->SmnBxbWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteIomux, (UINT32) (Pin & 0xFF), (UINT8) GpioPin.Gpio.IoMux);
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set ECRAM GPIO Pin on Reference Board
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. 0: Low. 1: High
 *
 */
VOID
CpmEcRamSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   PinNum;
  UINT8                   Offset;
  UINT8                   Data;
  UINT8                   Mask;
  EFI_STATUS              Status;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-Pin = GPIO%d, Value = 0x%04X\n", Pin, Value));
  Status = EFI_SUCCESS;
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  PinNum              = (UINT8)Pin;
  Offset              = CFG_CPM_ECRAM_GPIO_OFFSET + 1 + (PinNum >> 3);
  Mask                = 0x01 << (PinNum & 0x07);
  if (MainTablePtr->EcSupport) {
    Status = CpmKbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Data);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-CPM_DEADLOOP-Pin = GPIO%d Value = 0x%04X\n", __FUNCTION__, Pin, Value));
      CPM_DEADLOOP ();
    }
    Data = (Data & (0xFF ^ Mask)) | (((UINT8)Value & 0x01) << (PinNum & 0x07));
    Status = CpmEcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Data);
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-Pin = GPIO%d, Value = 0x%04X\n", __FUNCTION__, Pin, Value));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set Promontory GPIO Pin on Reference Board
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. 0: Low. 1: High
 *
 */
VOID
CpmPtSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT32                  BridgeAddress;
  UINT32                  DeviceAddress;
  UINT32                  PtGpioMmio;
  UINT8                   Bus;
  UINT8                   BusSec;
  UINT8                   BridgeCommand;
  UINT8                   DeviceCommand;
  UINT8                   Data;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-Pin = GPIO%d, Value = 0x%04X\n", Pin, Value));
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  BridgeAddress       = (UINT32)MainTablePtr->PtBrMemIoBaseAddr;

  if (BridgeAddress != 0) {
    Bus = CommonFunctionPtr->MmioRead8 (BridgeAddress + 0x19);              // PT Bridge Offset 0x19
    BusSec = CommonFunctionPtr->MmioRead8 (BridgeAddress + 0x1A);           // PT Bridge Offset 0x1A

    if (Bus != 0xFF) {
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x19, PtTempBus);      // PT Bridge Offset 0x19
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x1A, PtTempBus);      // PT Bridge Offset 0x1A
      DeviceAddress = (UINT32)(MainTablePtr->PcieMemIoBaseAddr + (PtTempBus << 20) + (PtGpioFun << 12));

      BridgeCommand = CommonFunctionPtr->MmioRead8 (BridgeAddress + 0x04);  // PT Bridge Offset 0x04
      DeviceCommand = CommonFunctionPtr->MmioRead8 (DeviceAddress + 0x04);  // PT GPIO Device Offset 0x04

      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x04, 0x07);           // PT Bridge Offset 0x04
      CommonFunctionPtr->MmioWrite8 (DeviceAddress + 0x04, 0x06);           // PT GPIO Device Offset 0x04

      PtGpioMmio = CommonFunctionPtr->MmioRead32 (DeviceAddress + 0x40) & ~(UINT32)BIT2;  // PT GPIO Device Offset 0x40

      Data = CommonFunctionPtr->MmioRead8 (PtGpioMmio);                                   // PT GPIO MMIO Offset 0x00
      if ((Data & (UINT8)(1 << Pin)) != (UINT8)(1 << Pin)) {
        CommonFunctionPtr->MmioWrite8 (PtGpioMmio, (Data | (UINT8)(1 << Pin)));           // PT GPIO MMIO Offset 0x00
      }

      Data = CommonFunctionPtr->MmioRead8 (PtGpioMmio + 0x08) & ~(UINT8)(1 << Pin);       // PT GPIO MMIO Offset 0x08
      CommonFunctionPtr->MmioWrite8 ((PtGpioMmio + 0x08), (Data | (UINT8)(Value << Pin)));// PT GPIO MMIO Offset 0x08

      CommonFunctionPtr->MmioWrite8 (DeviceAddress + 0x04, DeviceCommand);  // PT GPIO Device Offset 0x04
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x04, BridgeCommand);  // PT Bridge Offset 0x04
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x19, Bus);            // PT Bridge Offset 0x19
      CommonFunctionPtr->MmioWrite8 (BridgeAddress + 0x1A, BusSec);         // PT Bridge Offset 0x1A
    }
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-BridgeAddress = 0x%08X Pin = GPIO%d, Value = 0x%04X\n", __FUNCTION__, BridgeAddress, Pin, Value));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set Multi Socket/Die FCH GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket  The Socket Number.
 * @param[in] Die     The Die Number.
 * @param[in] Bridge  The Bridge Number.
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. See Definition of AMD_CPM_GPIO_SETTING
 *
 */
VOID
CpmSmnSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Socket,
  IN       UINT16                      Die,
  IN       UINT16                      Bridge,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GPIO_SETTING    GpioPin;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   temp;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-Socket = 0x%04X, Die = 0x%04X, Bridge = 0x%04X, Pin = GPIO%d, Value = 0x%04X\n", Socket, Die, Bridge, Pin, Value));
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  GpioPin.Raw = Value;

  if (Pin < 0x100) {
    temp = CommonFunctionPtr->SmnRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2));
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnRead8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X) = 0x%02X\n", (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2), temp));
  } else {
    temp = CommonFunctionPtr->SmnRead8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2));
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnRead8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X) = 0x%02X\n", (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2), temp));
  }
  if (GpioPin.Gpio.PresetEn) {
    temp = temp & 0xC7;
    if (GpioPin.Gpio.PullUpSel) {
      temp |= 0x08;
    }
    if (GpioPin.Gpio.PullUp) {
      temp |= 0x10;
    }
    if (GpioPin.Gpio.PullDown) {
      temp |= 0x20;
    }
  }
  if (GpioPin.Gpio.SetEnB == 0) {
    if (GpioPin.Gpio.OutEnB == 0) {
      temp |= 0x80;
      if (GpioPin.Gpio.Out) {
        temp |= 0x40;
      } else {
        temp &= 0xBF;
      }
    } else {
      temp &= 0x7F;
    }
  }
  if (Pin < 0x100) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2), temp));
    CommonFunctionPtr->SmnWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnGpio, (UINT32) ((Pin << 2) + 2), temp);
  } else {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
    DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2), temp));
    CommonFunctionPtr->SmnWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteGpio, (UINT32) (((Pin & 0xFF) << 2) + 2), temp);
  }

  if (GpioPin.Gpio.IoMuxEn) {
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-GpioPin.Gpio.IoMuxEn = 0x%02X, GpioPin.Gpio.IoMux = 0x%02X\n", __FUNCTION__, (UINT8) GpioPin.Gpio.IoMuxEn, (UINT8) GpioPin.Gpio.IoMux));
    if (Pin < 0x100) {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
      DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnIomux, (UINT32) Pin, (UINT8) GpioPin.Gpio.IoMux));
      CommonFunctionPtr->SmnWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnIomux, (UINT32) Pin, (UINT8) GpioPin.Gpio.IoMux);
    } else {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmnWrite8 (0x%04X, 0x%04X, 0x%04X,", __FUNCTION__, Socket, Die, Bridge));
      DEBUG((DEBUG_INFO, " 0x%08X, 0x%08X, 0x%02X)\n", (UINT32) FchSmnRemoteIomux, (UINT32) (Pin & 0xFF), (UINT8) GpioPin.Gpio.IoMux));
      CommonFunctionPtr->SmnWrite8 (This, Socket, Die, Bridge, (UINT32) FchSmnRemoteIomux, (UINT32) (Pin & 0xFF), (UINT8) GpioPin.Gpio.IoMux);
    }
  }

}

/*---------------------------------------------------------------------------------------*/
/**
 * Set SMBUS GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @param[in] Pin:
 * SMBUS GPIO Setting for BIT[15:8] = 0x06.
 *                    BIT[31]:            The Smbus number. 0 = Smbus0, 1 = Smbus1.
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:16]:         Reserved
 * GPIO Controller Selection.
 *                    BIT[15:8] = 0x06:   SMBUS IO Expander GPIO Pin
 * SMBUS GPIO Pin Number for BIT[15:8] = 0x06.
 *                    BIT[7:6] = 0x0:     SMBUS IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     SMBUS IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     SMBUS IO Expander Port 0
 *                    BIT[5:4] = 0x1:     SMBUS IO Expander Port 1
 *                    BIT[3:0]:           SMBUS GPIO Pin Number
 *
 * @param[in] Value   The Setting of GPIO Pin. See Definition of AMD_CPM_GPIO_SETTING
 *
 */
VOID
CpmSmbusSetGpio (
  IN       VOID                        *This,
  IN       UINT32                      Pin,
  IN       UINT16                      Value
  )
{
  EFI_STATUS              Status;
  UINT8                   Data;
  UINT8                   DataRead;
  UINT8                   Select;
  UINT8                   Address;
  UINT8                   Chip;
  UINT8                   Port;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  Select = (UINT8) ((Pin >> 31) & 1);
  Address = (UINT8) ((Pin >> 24) & 0x7F);
  Chip = (UINT8) ((Pin >> 6) & 3);
  Port = (UINT8) ((Pin >> 4) & 3);

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Smbus%d IO Expander %s Port%d", __FUNCTION__, Select, (Chip == 0) ? L"8-bit" : L"16-bit", Port));
  DEBUG((DEBUG_INFO, ", Address = 0x%02X, Pin = GPIO%d, Value = %d\n", Address << 1, (Pin & 0x7), Value));

  Status = CpmReadSmbus (This, Select, Address, (3 * (Chip + 1)) + Port, 1, &Data);       // Read Configuration register (03h)
  if (EFI_ERROR (Status)) {
    return;
  }

  if (Value == 2) {
    if (Data & (UINT8) (1 << (UINT8) (Pin & 0x7))) {
      // Clear Interrupt
      Status = CpmReadSmbus (This, Select, Address, 0 + Port, 1, &Data);                  // Read Input port register (00h)
    } else {
      Data |= (UINT8) (1 << (UINT8) (Pin & 0x7));
      Status = CpmWriteSmbus (This, Select, Address, (3 * (Chip + 1)) + Port, 1, &Data);  // Write Configuration register (03h)
      if (EFI_ERROR (Status)) {
        return;
      }

      // Changing an I/O from an output to an input may cause a false interrupt to occur.
      // Clear Interrupt
      Status = CpmReadSmbus (This, Select, Address, 0 + Port, 1, &Data);                  // Read Input port register (00h)
    }
  } else {
    if (Data & (UINT8) (1 << (UINT8) (Pin & 0x7))) {
      Data &= (UINT8) (~(1 << (UINT8) (Pin & 0x7)));
      Status = CpmWriteSmbus (This, Select, Address, (3 * (Chip + 1)) + Port, 1, &Data);  // Write Configuration register (03h)
      if (EFI_ERROR (Status)) {
        return;
      }
    }
    Status = CpmReadSmbus (This, Select, Address, (1 * (Chip + 1)) + Port, 1, &Data);     // Read Output port register (01h)
    if (EFI_ERROR (Status)) {
      return;
    }
    DataRead = Data;
    if (Value == 1) {
      Data |= (UINT8) (1 << (UINT8) (Pin & 0x7));
    } else {
      Data &= (UINT8) (~(1 << (UINT8) (Pin & 0x7)));
    }
    if (DataRead != Data) {
      Status = CpmWriteSmbus (This, Select, Address, (1 * (Chip + 1)) + Port, 1, &Data);  // Write Output port register (01h)
    }
  }
  return;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set I2C GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @param[in] Pin:
 * I2C GPIO Setting for BIT[15:8] = 0x07.
 *                    BIT[31]:            Reserved
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:21]:         The I2C number. 0 = I2C0, 1 = I2C1, 2 = I2C2, 3 = I2C3, 4 = I2C4, 5 = I2C5
 *                    BIT[20:16]:         Reserved
 * GPIO Controller Selection.
 *                    BIT[15:8] = 0x07:   I2C IO Expander GPIO Pin
 * I2C GPIO Pin Number for BIT[15:8] = 0x07.
 *                    BIT[7:6] = 0x0:     I2C IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     I2C IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     I2C IO Expander Port 0
 *                    BIT[5:4] = 0x1:     I2C IO Expander Port 1
 *                    BIT[3:0]:           I2C GPIO Pin Number
 *
 * @param[in] Value   The Setting of GPIO Pin. See Definition of AMD_CPM_GPIO_SETTING
 *
 */
VOID
CpmI2CSetGpio (
  IN       VOID                        *This,
  IN       UINT32                      Pin,
  IN       UINT16                      Value
  )
{
  EFI_STATUS              Status;
  UINT8                   Data;
  UINT8                   DataRead;
  UINT8                   Select;
  UINT8                   Address;
  UINT8                   Chip;
  UINT8                   Port;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  Select = (UINT8)((Pin >> 21) & 7);
  Address = (UINT8) ((Pin >> 24) & 0x7F);
  Chip = (UINT8) ((Pin >> 6) & 3);
  Port = (UINT8) ((Pin >> 4) & 3);

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-I2C%d IO Expander %s Port%d", __FUNCTION__, Select, (Chip == 0) ? L"8-bit" : L"16-bit", Port));
  DEBUG((DEBUG_INFO, ", Address = 0x%02X, Pin = GPIO%d, Value = %d\n", Address << 1, (Pin & 0x7), Value));

  Status = CpmReadI2C (This, Select, Address, (3 * (Chip + 1)) + Port, 1, &Data);       // Read Configuration register (03h)
  if (EFI_ERROR (Status)) {
    return;
  }
  if (Value == 2) {
    if (Data & (UINT8) (1 << (UINT8) (Pin & 0x7))) {
      // Clear Interrupt
      Status = CpmReadI2C (This, Select, Address, 0 + Port, 1, &Data);                  // Read Input port register (00h)
    } else {
      Data |= (UINT8) (1 << (UINT8) (Pin & 0x7));
      Status = CpmWriteI2C (This, Select, Address, (3 * (Chip + 1)) + Port, 1, &Data);  // Write Configuration register (03h)
      if (EFI_ERROR (Status)) {
        return;
      }

      // Changing an I/O from an output to an input may cause a false interrupt to occur.
      // Clear Interrupt
      Status = CpmReadI2C (This, Select, Address, 0 + Port, 1, &Data);                  // Read Input port register (00h)
    }
  } else {
    if (Data & (UINT8) (1 << (UINT8) (Pin & 0x7))) {
      Data &= (UINT8) (~(1 << (UINT8) (Pin & 0x7)));
      Status = CpmWriteI2C (This, Select, Address, (3 * (Chip + 1)) + Port, 1, &Data);  // Write Configuration register (03h)
      if (EFI_ERROR (Status)) {
        return;
      }
    }
    Status = CpmReadI2C (This, Select, Address, (1 * (Chip + 1)) + Port, 1, &Data);     // Read Output port register (01h)
    if (EFI_ERROR (Status)) {
      return;
    }
    DataRead = Data;
    if (Value == 1) {
      Data |= (UINT8) (1 << (UINT8) (Pin & 0x7));
    } else {
      Data &= (UINT8) (~(1 << (UINT8) (Pin & 0x7)));
    }
    if (DataRead != Data) {
      Status = CpmWriteI2C (This, Select, Address, (1 * (Chip + 1)) + Port, 1, &Data);  // Write Output port register (01h)
    }
  }
  return;
}
/*---------------------------------------------------------------------------------------*/
/**
 * Set GPIO Pin
 *
 *
 * @param[in] This:   The pointer of AMD CPM Table Ppi or Protocol
 *
 * @param[in] Pin:
 * FCH/Bixby GPIO/Remote GPIO setting for BIT[15:8] = 0x00/0x01/0x04/0x05.
 *                    BIT[31]:            Multi Socket Number
 *                    BIT[30:29]:         Multi Die Number
 *                    BIT[28:27]:         Multi Bridge Number
 *                    BIT[26:16]:         Reserved
 * SMBUS GPIO Setting for BIT[15:8] = 0x06.
 *                    BIT[31]:            The Smbus number. 0 = Smbus0, 1 = Smbus1.
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:16]:         Reserved
 * I2C GPIO Setting for BIT[15:8] = 0x07.
 *                    BIT[31]:            Reserved.
 *                    BIT[30:27]:         Fixed address of Slave Address.
 *                    BIT[26:24]:         Programmable or HW Selectable address of Slave Address (A2/A1/A0).
 *                    BIT[23:21]:         The I2C number. 0 = I2C0, 1 = I2C1, 2 = I2C2, 3 = I2C3, 4 = I2C4, 5 = I2C5
 *                    BIT[20:16]:         Reserved
 * GPIO Controller Selection.
 *                    BIT[15:8] = 0x00:   FCH GPIO Pin
 *                    BIT[15:8] = 0x01:   Remote FCH GPIO Pin
 *                    BIT[15:8] = 0x02:   ECRAM GPIO Pin on Reference Board
 *                    BIT[15:8] = 0x03:   Promontory GPIO Pin
 *                    BIT[15:8] = 0x04:   Bixby GPIO Pin
 *                    BIT[15:8] = 0x05:   Bixby Remote GPIO Pin
 *                    BIT[15:8] = 0x06:   SMBUS IO Expander GPIO Pin
 *                    BIT[15:8] = 0x07:   I2C IO Expander GPIO Pin
 *                    BIT[15:8] > 0x06:   Reserved
 * Non-SMBUS GPIO Pin Number for BIT[15:8] = 0x00 ~ 0x05
 *                    BIT[7:0]:           GPIO Pin Number
 * SMBUS GPIO Pin Number for BIT[15:8] = 0x06.
 *                    BIT[7:6] = 0x0:     SMBUS IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     SMBUS IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     SMBUS IO Expander Port 0
 *                    BIT[5:4] = 0x1:     SMBUS IO Expander Port 1
 *                    BIT[3:0]:           GPIO Pin Number
 * I2C GPIO Pin Number for BIT[15:8] = 0x07.
 *                    BIT[7:6] = 0x0:     I2C IO Expander 8-bit Controller
 *                    BIT[7:6] = 0x1:     I2C IO Expander 16-bit Controller
 *                    BIT[5:4] = 0x0:     I2C IO Expander Port 0
 *                    BIT[5:4] = 0x1:     I2C IO Expander Port 1
 *                    BIT[3:0]:           GPIO Pin Number
 *
 * @param[in] Value:  The Setting of GPIO Pin.
 *
 */
VOID
CpmSetGpio (
  IN       VOID                        *This,
  IN       UINT32                      Pin,
  IN       UINT16                      Value
  )
{
  UINT16  Select;
  UINT16  PinNum;

  Select = (UINT16) ((Pin >> 8) & 0x7);
  PinNum = (UINT16) (Pin & 0xFF);

  switch (Select) {
  case 0:
  case 1:
    if ((Pin & 0xF8000000) == 0) {
      CpmFchSetGpio (This, (UINT16) (Pin & 0x1FF), Value);
    } else {
      CpmSmnSetGpio (This, (UINT16) ((Pin & 0x80000000) >> 31), (UINT16) ((Pin & 0x60000000) >> 29), (UINT16) ((Pin & 0x18000000) >> 27), (UINT16) (Pin & 0x1FF), Value);
    }
    break;
  case 4:
  case 5:
    CpmBxbSetGpio (This, (UINT16) ((Pin & 0x80000000) >> 31), (UINT16) ((Pin & 0x60000000) >> 29), (UINT16) ((Pin & 0x18000000) >> 27), (UINT16) (Pin & 0x1FF), Value);
    break;
  case 2:
    CpmEcRamSetGpio (This, PinNum, Value);
    break;
  case 3:
    CpmPtSetGpio (This, PinNum, Value);
    break;
  case 6:
    CpmSmbusSetGpio (This, Pin, Value);
    break;
  case 7:
    CpmI2CSetGpio (This, Pin, Value);
  default:
    break;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of GEVENT Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 *
 * @retval    The State of GEVENT Pin
 *
 */
UINT8
CpmGetGevent (
  IN       VOID                        *This,
  IN       UINT8                       Pin
  )
{
  UINT8   Value;
  Value = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    Value = CpmGetGpio (This, (UINT32) (Pin + 0x60));
    break;
  case CPM_FCH_REVISION_ID_ML:
    Value = 0;
    if (Pin <= 22) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTable[Pin]);
    }
    break;
  case CPM_FCH_REVISION_ID_CZ:
  case CPM_FCH_REVISION_ID_NL:
  case CPM_FCH_REVISION_ID_AM:
  case CPM_FCH_REVISION_ID_ST:
  case CPM_FCH_REVISION_ID_BR:
    Value = 0;
    if (Pin <= 23) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTable2[Pin]);
    }
    break;
  case CPM_FCH_REVISION_ID_ZP:
    Value = 0;
    if (Pin <= 23) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTableZP[Pin]);
    }
    break;
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_MTS:
    Value = 0;
    if (Pin <= 23) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTableSSP[Pin]);
    }
    break;
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_VN:
    Value = 0;
    if (Pin <= 23) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTableRV[Pin]);
    }
    break;
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
    Value = 0;
    if (Pin <= 23) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTableRMB[Pin]);
    }
    break;
  case CPM_FCH_REVISION_ID_STP:
    Value = 0;
    if (Pin <= 23) {
      Value = CpmGetGpio (This, (UINT32) FchGeventGpioMappingTableSTP[Pin]);
    }
    break;
  }
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the State of GEVENT Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 * @param[in] Value   The Setting of GEVENT Pin. See Definition of AMD_CPM_GEVENT_SETTING
 *
 */
VOID
CpmSetGevent (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GEVENT_SETTING  GeventPin;
  UINT32                  andMask32;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT16                  SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  GeventPin.Raw = Value;

  if (Pin < 0x40) {
    SciMap = GeventPin.Gevent.SciMap;
    andMask32 = ~(1 << SciMap);

    //EventEnable: 0-Disable, 1-Enable.
    if (GeventPin.Gevent.EventEnable) {
      if (GeventPin.Gevent.SciTrigAuto) {
        //SciTrig: 0-Falling Edge, 1-Rising Edge.
        CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCITRIG),
                                                andMask32,
                                                (CommonFunctionPtr->GetGevent (This, (UINT8) Pin) ? 0 : 1) << SciMap);
      } else {
        //SciTrig: 0-Falling Edge, 1-Rising Edge.
        CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCITRIG),
                                                andMask32,
                                                GeventPin.Gevent.SciTrig << SciMap);
      }

      //SciLevl: 0-Edge trigger, 1-Level Trigger.
      CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCILEVEL),
                                              andMask32,
                                              GeventPin.Gevent.SciLevl << SciMap);

      //SmiSciEn: 0-Not send SMI, 1-Send SMI
      CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMISCIEN),
                                              andMask32,
                                              GeventPin.Gevent.SmiSciEn << SciMap);

      //SciS0En: 0-Disable, 1-Enable
      CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCIS0EN),
                                              andMask32,
                                              GeventPin.Gevent.SciS0En << SciMap);

      //SciMap: 00000b ~ 11111b.
      CommonFunctionPtr->MmioAndThenOr8 (     (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCIMAP0 + Pin),
                                              (UINT8)AMD_GEVENT_SCIMASK,
                                              (UINT8)GeventPin.Gevent.SciMap);

      if (Pin < 24) {
        //SmiTrig: 0-Active Low, 1-Active High.
        CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMITRIG),
                                                ~(1 << Pin),
                                                GeventPin.Gevent.SmiTrig << Pin);
      }

      //SmiControl: 0-Disable, 1-SMI, 2-NMI, 3-IRQ13
      CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMICONTROL0 + Pin / 4),
                                              ~(AMD_SMICONTROL_MASK << ((Pin & 0x3) * 2)),
                                              GeventPin.Gevent.SmiControl << ((Pin & 0x3) * 2));
    }
  } else if ((Pin & 0xFF00) == 0x0100) {
    CommonFunctionPtr->MmioWrite16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + ((Pin & 0xFF) << 2)), Value);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set SMI Control for GEVENT Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 * @param[in] Value   The Setting of GEVENT Pin. See Definition of AMD_CPM_GEVENT_SETTING
 *
 */
VOID
CpmSetSmiControl (
  IN       VOID                        *This,
  IN       UINT8                       Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GEVENT_SETTING  GeventPin;
  UINT32                  andMask32;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT16                  SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  GeventPin.Raw = Value;

  if (Pin < 32) {
    SciMap = GeventPin.Gevent.SciMap;
    andMask32 = ~(1 << Pin);

    //SmiControl: 0-Disable, 1-SMI, 2-NMI, 3-IRQ13
    CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMICONTROL0 + Pin / 4),
                                            ~(AMD_SMICONTROL_MASK << ((Pin & 0x3) * 2)),
                                            GeventPin.Gevent.SmiControl << ((Pin & 0x3) * 2));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the way to set corresponding bit in Event Status
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 * @param[in] Value   The bit controls the way to set Event Status.
 *                      0: Active Low. 1: Active High
 *
 */
VOID
CpmSetGeventSciTrig (
  IN       VOID                        *This,
  IN       UINT8                       Pin,
  IN       UINT8                       Value
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT32                  andMask32;
  UINT8                   SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SciMap = CommonFunctionPtr->GetSciMap (This, Pin);
  andMask32 = ~(1 << SciMap);
  CommonFunctionPtr->MmioAndThenOr32 (    (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCITRIG),
                                          andMask32,
                                          Value << SciMap);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Trigger a GEVENT SCI interrupt
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 *
 */
VOID
CpmSetGeventSci (
  IN       VOID                        *This,
  IN       UINT8                       Pin
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SciMap = CommonFunctionPtr->GetSciMap (This, Pin);
  CommonFunctionPtr->MmioOr32 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIEN), 1 << SciMap);
  CommonFunctionPtr->MmioWrite32 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIDATA), 1 << SciMap);
  CommonFunctionPtr->MmioWrite32 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIDATA), 0);
  CommonFunctionPtr->MmioWrite32 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIEN), 0);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH Revision Id
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The FCH Revision Id
 *
 */
UINT8
CpmGetFchRevisionId (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   FchRevision;
  UINT8                   CpuRevision;
  UINT32                  Data;
  UINT8                   FchSmbusRevision;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start\n", __FUNCTION__));
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  CpuRevision         = CommonFunctionPtr->GetCpuRevisionId (This);
  FchRevision         = CPM_FCH_REVISION_ID_DEFAULT;
  FchSmbusRevision    = 0;
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-CpuRevision = 0x%02X\n", __FUNCTION__, CpuRevision));
  switch (CpuRevision) {
  case CPM_CPU_REVISION_ID_KB:
    FchRevision = CPM_FCH_REVISION_ID_KB;
    break;
  case CPM_CPU_REVISION_ID_ML:
    FchRevision = CPM_FCH_REVISION_ID_ML;
    break;
  case CPM_CPU_REVISION_ID_CZ:
    CommonFunctionPtr->CpuidRead (This, NULL, &Data);
    if (Data != 0x00000003) {
      FchRevision = CPM_FCH_REVISION_ID_CZ;
    } else {
      FchSmbusRevision = CommonFunctionPtr->PciRead8 (This, 0, 0x14, 0, 0x08);
      if ((FchSmbusRevision < 0x11) || (FchSmbusRevision > 0x16)) {
        FchRevision = CPM_FCH_REVISION_ID_CZ;
      }
    }
    break;
  case CPM_CPU_REVISION_ID_NL:
    FchRevision = CPM_FCH_REVISION_ID_NL;
    break;
  case CPM_CPU_REVISION_ID_AM:
    FchRevision = CPM_FCH_REVISION_ID_AM;
    break;
  case CPM_CPU_REVISION_ID_ST:
    CommonFunctionPtr->CpuidRead (This, NULL, &Data);
    if (Data != 0x00000003) {
      FchRevision = CPM_FCH_REVISION_ID_ST;
    }
    break;
  case CPM_CPU_REVISION_ID_BR:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_BR\n"));
    FchRevision = CPM_FCH_REVISION_ID_BR;
    break;
  case CPM_CPU_REVISION_ID_ZP:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_ZP\n"));
    FchRevision = CPM_FCH_REVISION_ID_ZP;
    break;
  case CPM_CPU_REVISION_ID_RV:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_RV\n"));
    FchRevision = CPM_FCH_REVISION_ID_RV;
    break;
  case CPM_CPU_REVISION_ID_RV2:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_RV2\n"));
    FchRevision = CPM_FCH_REVISION_ID_RV2;
    break;
  case CPM_CPU_REVISION_ID_SSP:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_SSP\n"));
    FchRevision = CPM_FCH_REVISION_ID_SSP;
    break;
  case CPM_CPU_REVISION_ID_MTS:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_MTS\n"));
    FchRevision = CPM_FCH_REVISION_ID_MTS;
    break;
  case CPM_CPU_REVISION_ID_RN:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_RN\n"));
    FchRevision = CPM_FCH_REVISION_ID_RN;
    break;
  case CPM_CPU_REVISION_ID_VN:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_VN\n"));
    FchRevision = CPM_FCH_REVISION_ID_VN;
    break;
  case CPM_CPU_REVISION_ID_RMB:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_RMB\n"));
    FchRevision = CPM_FCH_REVISION_ID_RMB;
    break;
  case CPM_CPU_REVISION_ID_RPL:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_RPL\n"));
    FchRevision = CPM_FCH_REVISION_ID_RPL;
    break;
  case CPM_CPU_REVISION_ID_STP:
    DEBUG((DEBUG_INFO, "  case CPM_CPU_REVISION_ID_STP\n"));
    FchRevision = CPM_FCH_REVISION_ID_STP;
    break;
  }
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End-FchRevision = 0x%02X\n", __FUNCTION__, FchRevision));
  return FchRevision;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH Strap Setting
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The State of FCH Strap Pin
 *              BIT0: ImcEnableStrap: 0: Disable. 1: Enable
 *              BIT1: ClkGenStrap: 0: External. 1: Internal
 *              BIT2: S5+ Support: 0: Disable. 1: Enable
 *
 */
UINT32
CpmGetStrap (
  IN       VOID                        *This
  )
{
  UINT8                   StrapStatus;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT32                  StrapValue;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  StrapValue = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
    StrapStatus = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x80));
    StrapValue |= ((StrapStatus & BIT2) >> 2) | ((StrapStatus & BIT4) >> 3);
    StrapStatus = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x82));
    StrapValue |= StrapStatus & BIT2;
    break;
  case CPM_FCH_REVISION_ID_KB:
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
  case CPM_FCH_REVISION_ID_NL:
  case CPM_FCH_REVISION_ID_AM:
  case CPM_FCH_REVISION_ID_ST:
  case CPM_FCH_REVISION_ID_BR:
  case CPM_FCH_REVISION_ID_ZP:
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_MTS:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_STP:
    StrapStatus = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x80));
    StrapValue |= (StrapStatus & BIT2) >> 2;
    StrapStatus = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x82));
    StrapValue |= StrapStatus & BIT1;
    break;
  case CPM_FCH_REVISION_ID_VN:
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
    StrapStatus = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x82));
    StrapValue |= StrapStatus & BIT1;
    break;
  }
  return StrapValue;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set FCH ClkReq
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket  CPU Socket Number
 * @param[in] Die     CPU Die Number
 * @param[in] Bridge  CPU Bridge Number
 * @param[in] Miscid  Select MISC or MISC2 clkreq mapping
 * @param[in] ClkId   PCIE Clock Pin Number
 * @param[in] ClkReq  ClkReq Mapping
 *
 */
VOID
CpmSetClkReq (
  IN       VOID                        *This,
  IN       UINT8                       Socket,
  IN       UINT8                       Die,
  IN       UINT8                       Bridge,
  IN       UINT8                       Miscid,
  IN       UINT8                       ClkId,
  IN       UINT8                       ClkReq
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   AndData;
  UINT8                   OrData;
  UINT32                  MiscAddress;

  DEBUG((DEBUG_INFO, "  LIB-FCH-CpmSetClkReq-Start-Socket = 0x%02X Die = 0x%02X Bridge = 0x%02X", Socket, Die, Bridge));
  DEBUG((DEBUG_INFO, " Miscid = 0x%02X ClkId = 0x%02X ClkReq = 0x%02X\n", Miscid, ClkId, ClkReq));
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
    if (ClkId <= 9) {
      CommonFunctionPtr->MmioAndThenOr8 ( (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 1)),
                                          0x0F << (ClkId % 2 ? 0: 4),
                                          ClkReq << (ClkId % 2 ? 4: 0)
                                          );
    } else {
      switch (ClkId) {
      case CPM_CLKID_APU_CLK:
      case CPM_CLKID_DISP2_CLK:
      case CPM_CLKID_PCIE_RCLK_OUTPUT:
      case CPM_CLKID_DISP_CLK:
        CommonFunctionPtr->MmioAndThenOr8 ( (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05),
                                        0xFE ^ (1 << ((ClkId & 0x7F) + 1)),
                                        0x01 | ((ClkReq ? 0 : 1) << ((ClkId & 0x7F) + 1))
                                        );
        break;
      case CPM_CLKID_PCIE_RCLK:
        CommonFunctionPtr->MmioAndThenOr8 ( (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05),
                                        0xDF,
                                        ClkReq ? 0 : 0x20
                                        );
        break;
      case CPM_CLKID_CLOCK_BUFFER_BIAS:
        CommonFunctionPtr->MmioAndThenOr8 ( (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x06),
                                        0xFE,
                                        ClkReq ? 0 : 1
                                        );
        break;
      case CPM_CLKID_OSCOUT2_OUTOFF:
        CommonFunctionPtr->MmioAndThenOr8 ( (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x40),
                                        0x7F,
                                        ClkReq ? 0 : 0x80
                                        );
        break;
      }
    }
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_KB:
  case CPM_FCH_REVISION_ID_CZ:
  case CPM_FCH_REVISION_ID_ST:
  case CPM_FCH_REVISION_ID_BR:
    if (ClkId <= 3 || ClkId == 9) {
      AndData = ~(UINT8) (0x0F << ((ClkId & 1) * 4));
      OrData = (UINT8) ((ClkReq & 0x0F) << ((ClkId & 1) * 4));
      DEBUG((DEBUG_INFO, "  LIB-FCH-CpmSetClkReq-MmioAndThenOr8 (0x%08X, 0x%02X, 0x%02X)\n", (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 1)), AndData, OrData));
      CommonFunctionPtr->MmioAndThenOr8 ( (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 1)), AndData, OrData);
    } else if (ClkId == CPM_CLKID_PCIE_RCLK) {
      AndData = 0xDF;
      OrData = (UINT8) (ClkReq ? 0 : 0x20);
      DEBUG((DEBUG_INFO, "  LIB-FCH-CpmSetClkReq-MmioAndThenOr8 (0x%08X, 0x%02X, 0x%02X)\n", (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05), AndData, OrData));
      CommonFunctionPtr->MmioAndThenOr8 ( (UINT32) (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05), AndData, OrData);
    }
    break;
  case CPM_FCH_REVISION_ID_NL:
  case CPM_FCH_REVISION_ID_AM:
    if (ClkId <= 2) {
      CommonFunctionPtr->MmioAndThenOr8 ( (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x00),
                                        0x03 << (ClkId * 2),
                                        (ClkReq == 0xFF ? 0x03 : ClkReq) << (ClkId * 2)
                                        );
    }
    break;
  case CPM_FCH_REVISION_ID_ZP:
    AndData = ~(UINT8) (0x03 << ((ClkId & 3) * 2));
    OrData = (UINT8) ((ClkReq == 0 ? 0: (ClkReq == 0xFF ? 3: 1)) << ((ClkId & 3) * 2));
    if ((Socket == 0) && (Die == 0)) {
      MiscAddress = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 2));
      DEBUG((DEBUG_INFO, "  LIB-FCH-CpmSetClkReq-MmioAndThenOr8 (0x%08X, 0x%02X, 0x%02X)\n", MiscAddress, AndData, OrData));
      CommonFunctionPtr->MmioAndThenOr8 (MiscAddress, AndData, OrData);
    } else {
      MiscAddress = FchSmnMisc;
      DEBUG((DEBUG_INFO, "  LIB-FCH-CpmSetClkReq-SmnAndThenOr8 (0x%X, 0x%X, 0x%X,", Socket, Die, Bridge));
      DEBUG((DEBUG_INFO, " 0x%X, 0x%X, 0x%X, 0x%X)\n", MiscAddress, (ClkId >> 2), AndData, OrData));
      CommonFunctionPtr->SmnAndThenOr8 (This, Socket, Die, Bridge, MiscAddress, (ClkId >> 2), AndData, OrData);
    }
    break;
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_MTS:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_VN:
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
  case CPM_FCH_REVISION_ID_STP:
    AndData = ~(UINT8) (0x03 << ((ClkId & 3) * 2));
    OrData = (UINT8) ((ClkReq == 0 ? 0: (ClkReq == 0xFF ? 3: 1)) << ((ClkId & 3) * 2));
    if (Miscid == 0x02) {
      MiscAddress = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC2_BASE + (ClkId >> 2));
    } else {
      MiscAddress = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 2));
    }
    DEBUG((DEBUG_INFO, "  LIB-FCH-CpmSetClkReq-MmioAndThenOr8 (0x%08X, 0x%02X, 0x%02X)\n", MiscAddress, AndData, OrData));
    CommonFunctionPtr->MmioAndThenOr8 (MiscAddress, AndData, OrData);
    break;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH ClkReq
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Socket  CPU Socket Number
 * @param[in] Die     CPU Die Number
 * @param[in] Bridge  CPU Bridge Number
 * @param[in] Miscid  Select MISC or MISC2 clkreq mapping
 * @param[in] ClkId   PCIE Clock Pin Number
 *
 * @retval    ClkReq  ClkReq Mapping
 *
 */
UINT8
CpmGetClkReq (
  IN       VOID                        *This,
  IN       UINT8                       Socket,
  IN       UINT8                       Die,
  IN       UINT8                       Bridge,
  IN       UINT8                       Miscid,
  IN       UINT8                       ClkId
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   ClkReq;
  UINT32                  MiscAddress;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_ZP:
    if ((Socket == 0) && (Die == 0)) {
      MiscAddress = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 2));
      ClkReq = (CommonFunctionPtr->MmioRead8 (MiscAddress) >> ((ClkId & 3) * 2)) & 3;
    } else {
      MiscAddress = FchSmnMisc;
      ClkReq = (CommonFunctionPtr->SmnRead8 (This, Socket, Die, Bridge, MiscAddress, (ClkId >> 2)) >> ((ClkId & 3) * 2)) & 3;
    }
    break;
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_MTS:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_VN:
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
  case CPM_FCH_REVISION_ID_STP:
    if (Miscid == 0x02) {
      MiscAddress = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC2_BASE + (ClkId >> 2));
    } else {
      MiscAddress = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 2));
    }
    ClkReq = (CommonFunctionPtr->MmioRead8 (MiscAddress) >> ((ClkId & 3) * 2)) & 3;
    break;
  default:
    ClkReq = 3;
    break;
  }
  DEBUG((DEBUG_INFO, "  LIB-FCH-CpmGetClkReq-End-Socket = 0x%02X Die = 0x%02X Bridge = 0x%02X", Socket, Die, Bridge));
  DEBUG((DEBUG_INFO, " Miscid = 0x%02X ClkId = 0x%02X ClkReq = 0x%02X\n", Miscid, ClkId, ClkReq));
  return ClkReq;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Add some delay in 1us/unit
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Delay   Delay in 1us/unit
 *
 */
VOID
CpmStall (
  IN       VOID                        *This,
  IN       UINT32                      Delay
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT16 timerAddr;
  UINT32 startTime;
  UINT32 elapsedTime;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  timerAddr = MainTablePtr->AcpiPmTimerIoAddr;
  if ( timerAddr == 0 ) {
    Delay = Delay / 2;
    while ( Delay != 0 ) {
      CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      Delay --;
    }
  } else {
    startTime = CommonFunctionPtr->IoRead32 (timerAddr);
    for ( ;; ) {
      elapsedTime = CommonFunctionPtr->IoRead32 (timerAddr);
      if ( elapsedTime < startTime ) {
        elapsedTime = elapsedTime + MAX_TIMER - startTime;
      } else {
        elapsedTime = elapsedTime - startTime;
      }
      if ((elapsedTime * ELAPSED_TIME_UNIT / ELAPSED_TIME_DIVIDER) > Delay) {
        break;
      }
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set Fan On
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Fan     Fan Number
 *
 */
VOID
CpmSetFanOn (
  IN       VOID                        *This,
  IN       UINT8                       Fan
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10), 0x07);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set ProcHot
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Enable  Prochot Enable: 0: Disable. 1: Enable
 * @param[in] Fan     FanOut Pin Number
 * @param[in] Freq    Frequency for FanOut Pin
 *
 */
VOID
CpmSetProchot (
  IN       VOID                        *This,
  IN       UINT8                       Enable,
  IN       UINT8                       Fan,
  IN       UINT8                       Freq
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  if (Enable) {
    CommonFunctionPtr->MmioOr8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x01), 0x04);            // SET FAN Active with high
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x02), Freq);         // SET FAN Frequence
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x03), 0x01);         // SET FAN Duty number
    CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x04), 0x00);
    CommonFunctionPtr->MmioAndThenOr8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10), 0xF8, 0x06);      // SET FAN enabled as manually control
  } else {
    CommonFunctionPtr->MmioOr8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10), 0x07);                   // Disable FAN for PROCHOT
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Current SATA Mode
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] BridgeBus Bus number of SATA Bridge
 * @param[in] BridgeDev Device number of SATA Bridge
 * @param[in] BridgeFun Function number of SATA Bridge
 * @param[in] SataDev   Device number of SATA Controller
 * @param[in] SataFun   Function number of SATA Controller
 *
 * @retval    The SATA Mode
 *              BIT0: IDE Mode
 *              BIT1: AHCI Mode
 *              BIT2: RAID Mode
 *              BIT3: AMD AHCI Mode
 *
 */
UINT8
CpmGetSataMode (
  IN       VOID                        *This,
  IN       UINT8                       BridgeBus,
  IN       UINT8                       BridgeDev,
  IN       UINT8                       BridgeFun,
  IN       UINT8                       SataDev,
  IN       UINT8                       SataFun
  )
{
  UINT8                   Mask;
  UINT16                  DeviceId;
  AMD_CPM_SATA_MODE_MASK  *TablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   FchRevision;
  UINT8                   BusNumber;

  Mask = 0;
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  FchRevision       = CpmGetFchRevisionId (This);

  DeviceId = 0x7901;      // Default set to SATA AHCI Mode (MS Driver)
  if (FchRevision < CPM_FCH_REVISION_ID_ZP) {
    DeviceId = CommonFunctionPtr->PciRead16 (This, SB_SATA_BUS, SB_SATA_DEVICE, SB_SATA_FUNCTION, 0x02);
  } else {
    BusNumber = CommonFunctionPtr->PciRead8 (This, BridgeBus, BridgeDev, BridgeFun, 0x19);
    DeviceId = CommonFunctionPtr->PciRead16 (This, BusNumber, SataDev, SataFun, 0x02);
  }

  TablePtr = &SataModeMask[0];
  while ( TablePtr->DeviceId != 0xFFFF ) {
    if ( TablePtr->DeviceId == DeviceId ) {
      Mask = TablePtr->Mask;
      break;
    }
    TablePtr ++;
  }
  return ( Mask );
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check whether the device is in FCH
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Device    The PCI Device Number
 * @param[in] Function  The PCI Function Number
 *
 * @retval    TRUE      The Device is in FCH
 *            FALSE     The Device is not in FCH
 *
 */
BOOLEAN
EFIAPI
CpmIsFchDevice (
  IN       VOID                        *This,
  IN       UINT8                       Device,
  IN       UINT8                       Function
  )
{
  if (Device <= 0x10) {
    return 0;
  } else {
    return 1;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH PCIe Bridge Name in ASL Code
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Device    The PCI Device Number
 * @param[in] Function  The PCI Function Number
 *
 * @retval    PCIe Bridge Name in ASL code
 *
 */
UINT32
CpmGetFchPcieAslName (
  IN       VOID                       *This,
  IN       UINT8                      Device,
  IN       UINT8                      Function
  )
{
  PCIE_BRIDGE_NAME  *BridgeNamePtr;
  UINT32  Name;

  Name = 0;
  BridgeNamePtr = &FchPcieBridgeName[0];
  while (BridgeNamePtr->Device != 0) {
    if (BridgeNamePtr->Device == Device && BridgeNamePtr->Function == Function) {
      switch (BridgeNamePtr->NameId) {
      case 0x30:
        Name = PcdGet32 (FchPcieGpp0AslName);
        break;
      case 0x31:
        Name = PcdGet32 (FchPcieGpp1AslName);
        break;
      case 0x32:
        Name = PcdGet32 (FchPcieGpp2AslName);
        break;
      case 0x33:
        Name = PcdGet32 (FchPcieGpp3AslName);
        break;
      }
      if (!Name) {
        Name = BridgeNamePtr->Name;
      }
      break;
    }
    BridgeNamePtr ++;
  }
  return Name;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the Mapping Bit Number of GEVENT pin in Event Status
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] GeventPin GEVENT Pin Number
 *
 * @retval    SCI Mapping Bit Number
 *
 */
UINT8
CpmGetSciMap (
  IN       VOID                        *This,
  IN       UINT8                       GeventPin
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  return CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + 0x40 + GeventPin));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Current Boot Mode
 *
 *
 * @retval    Boot Mode
 *
 */
UINT8
CpmGetBootMode (
  )
{
  UINT16                  Address;
  UINT16                  Value;

  if (PcdGet8 (CpmBootModeOverride) & BIT7) {
    Value = (UINT16)PcdGet8 (CpmBootModeOverride);
    Value &= 0x7F;
    Value <<= 10;
  } else {
    Address = CpmMmioRead16 (0xFED80362);
    Value = CpmIoRead16 (Address) & 0x1C00;
  }

  return (UINT8) (Value >> 10);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check whether it is a RTC wake up
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    TRUE      RTC wake up
 *            FALSE     Not RTC wake up
 *
 */
BOOLEAN
EFIAPI
CpmIsRtcWakeup (
  IN       VOID                        *This
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   Status;
  UINT8                   Enable;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  Status = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + 0x01));
  Enable = CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + 0x03));
  if ((Status & BIT2) && (Enable & BIT2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read RTC register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of RTC register
 *
 * @retval    RTC register value
 *
 */
UINT8
EFIAPI
CpmGetRtc (
  IN       VOID                        *This,
  IN       UINT8                       Index
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  return CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + CMOS_BASE + Index));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write RTC register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of RTC register
 * @param[in] Value     The value of RTC register
 *
 */
VOID
EFIAPI
CpmSetRtc (
  IN       VOID                        *This,
  IN       UINT8                       Index,
  IN       UINT8                       Value
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + CMOS_BASE + Index), Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read ACPI register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of ACPI register
 *
 * @retval    RTC register value
 *
 */
UINT8
EFIAPI
CpmGetAcpi (
  IN       VOID                        *This,
  IN       UINT8                       Index
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  return CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + Index));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write ACPI register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of ACPI register
 * @param[in] Value     The value of ACPI register
 *
 */
VOID
EFIAPI
CpmSetAcpi (
  IN       VOID                        *This,
  IN       UINT8                       Index,
  IN       UINT8                       Value
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr->MmioWrite8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + Index), Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read the data from Save Context Area
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Offset    The offset of data to read
 *
 * @retval    RTC register value
 *
 */
UINT8
EFIAPI
CpmGetSaveContext (
  IN       VOID                        *This,
  IN       UINT8                       Offset
  )
{
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION     *CommonFunctionPtr;
  AMD_CPM_SAVE_CONTEXT_TABLE  *SaveContextTablePtr;
  UINT8                       Data;
  UINT32                      Address;
  UINT8                       Flag;

  Data = 0;
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SaveContextTablePtr = CommonFunctionPtr->GetTablePtr (This, CPM_SIGNATURE_SAVE_CONTEXT);
  if (SaveContextTablePtr) {
    Flag = Offset & 0x80;
    Offset = Offset & 0x7F;
    if (Flag && (Offset < sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT))) {
      Address = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset);
      Data = CommonFunctionPtr->MmioRead8 (Address + (UINT32)Offset);
    } else if (Offset < SaveContextTablePtr->BufferSize - sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT)) {
      Address = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset + sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT));
      Data = CommonFunctionPtr->MmioRead8 (Address + (UINT32)Offset);
    } else {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-CPM_DEADLOOP-Offset = 0x%02X\n", __FUNCTION__, Offset));
      CPM_DEADLOOP ();
    }
  }
  return Data;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write the data to Save Context Area
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Offset    The offset of data to read
 * @param[in] Data      The data to write
 *
 */
VOID
EFIAPI
CpmSetSaveContext (
  IN       VOID                        *This,
  IN       UINT8                       Offset,
  IN       UINT8                       Data
  )
{
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION     *CommonFunctionPtr;
  AMD_CPM_SAVE_CONTEXT_TABLE  *SaveContextTablePtr;
  UINT32                      Address;
  UINT8                       Flag;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SaveContextTablePtr = CommonFunctionPtr->GetTablePtr (This, CPM_SIGNATURE_SAVE_CONTEXT);
  if (SaveContextTablePtr) {
    Flag = Offset & 0x80;
    Offset = Offset & 0x7F;
    if (Flag && (Offset < sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT))) {
      Address = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset);
      CommonFunctionPtr->MmioWrite8 (Address + (UINT32)Offset, Data);
    } else if (!Flag && (Offset < SaveContextTablePtr->BufferSize - sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT))) {
      Address = (UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset + sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT));
      CommonFunctionPtr->MmioWrite8 (Address + (UINT32)Offset, Data);
    } else {
      DEBUG((DEBUG_INFO, "  LIB-FCH-%a-CPM_DEADLOOP-Offset = 0x%02X Data = 0x%02X\n", __FUNCTION__, Offset, Data));
      CPM_DEADLOOP ();
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get ACPI SMI Command register
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    ACPI SMI Command register offset
 */
UINT16
CpmGetAcpiSmiCmd (
  IN       VOID                        *This
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  return CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x6A));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Register CPM Common FCH Function
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 */
VOID
EFIAPI
CpmRegisterFch (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   Data[4];
  UINT8                   FchRevision;

  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-Start\n", __FUNCTION__));
  MainTablePtr = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  FchRevision = CpmGetFchRevisionId (This);
  ((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb = FchRevision;

  switch (FchRevision) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    CommonFunctionPtr->IoWrite8 (0xCD6, 0x24);
    Data[0] = CommonFunctionPtr->IoRead8 (0xCD7);
    if (!(Data[0] & BIT0)) {
      CommonFunctionPtr->IoWrite8 (0xCD7, Data[0] | BIT0);
    }
    if (!MainTablePtr->AcpiMemIoBaseAddr) {
      CommonFunctionPtr->IoWrite8 (0xCD6, 0x25);
      Data[1] = CommonFunctionPtr->IoRead8 (0xCD7);
      CommonFunctionPtr->IoWrite8 (0xCD6, 0x26);
      Data[2] = CommonFunctionPtr->IoRead8 (0xCD7);
      CommonFunctionPtr->IoWrite8 (0xCD6, 0x27);
      Data[3] = CommonFunctionPtr->IoRead8 (0xCD7);
      MainTablePtr->AcpiMemIoBaseAddr = (Data[3] << 24) + (Data[2] << 16) + (Data[1] << 8);
    }
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
  case CPM_FCH_REVISION_ID_NL:
  case CPM_FCH_REVISION_ID_AM:
  case CPM_FCH_REVISION_ID_ST:
  case CPM_FCH_REVISION_ID_BR:
  case CPM_FCH_REVISION_ID_ZP:
    MainTablePtr->AcpiMemIoBaseAddr = 0xFED80000;
    break;
  case CPM_FCH_REVISION_ID_RV:
  case CPM_FCH_REVISION_ID_RV2:
  case CPM_FCH_REVISION_ID_RN:
  case CPM_FCH_REVISION_ID_VN:
  case CPM_FCH_REVISION_ID_RMB:
  case CPM_FCH_REVISION_ID_RPL:
    MainTablePtr->AcpiMemIoBaseAddr = 0xFED80000;
    // Turn on/off IO port 80 decoding for LPC - Register 0x48, bit 21
    CommonFunctionPtr->PciAndThenOr8( This,
                                      FCH_LPC_BUS,
                                      FCH_LPC_DEV,
                                      FCH_LPC_FUNC,
                                      FCH_LPC_REG4A,
                                      0xDF,                                   // AND data
                                      PcdGet8 (PcdIoPort80LpcDecodeEn) << 5); // OR data
    break;
  case CPM_FCH_REVISION_ID_SSP:
  case CPM_FCH_REVISION_ID_MTS:
  case CPM_FCH_REVISION_ID_STP:
    MainTablePtr->AcpiMemIoBaseAddr = 0xFED80000;
    break;
  }

  if (MainTablePtr->SmbusIoBaseAddr != (CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x00)) & 0xFF00)) {
    // Read SMBus IO base address
    MainTablePtr->SmbusIoBaseAddr = CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x00)) & 0xFF00;
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SmbusIoBaseAddr = 0x%04X\n", __FUNCTION__, MainTablePtr->SmbusIoBaseAddr));
  }

  if (MainTablePtr->SwSmiPort != CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x6A))) {
    // Read AcpiSmiCmd IO base address
    MainTablePtr->SwSmiPort = CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x6A));
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-SwSmiPort = 0x%04X\n", __FUNCTION__, MainTablePtr->SwSmiPort));
  }

  if (MainTablePtr->AcpiPmTimerIoAddr != (CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x64)) & 0xFFFE)) {
    // Read ACPI PM Timer IO Address
    MainTablePtr->AcpiPmTimerIoAddr = CommonFunctionPtr->MmioRead16 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x64)) & 0xFFFE;
    DEBUG((DEBUG_INFO, "  LIB-FCH-%a-AcpiPmTimerIoAddr = 0x%04X\n", __FUNCTION__, MainTablePtr->AcpiPmTimerIoAddr));
  }

  CommonFunctionPtr->GetGpio          = CpmGetGpio;
  CommonFunctionPtr->SetGpio          = CpmSetGpio;
  CommonFunctionPtr->GetGevent        = CpmGetGevent;
  CommonFunctionPtr->SetGevent        = CpmSetGevent;
  CommonFunctionPtr->SetSmiControl    = CpmSetSmiControl;
  CommonFunctionPtr->SetGeventSciTrig = CpmSetGeventSciTrig;
  CommonFunctionPtr->SetGeventSci     = CpmSetGeventSci;
  CommonFunctionPtr->GetStrap         = CpmGetStrap;
  CommonFunctionPtr->SetClkReq        = CpmSetClkReq;
  CommonFunctionPtr->GetClkReq        = CpmGetClkReq;
  CommonFunctionPtr->Stall            = CpmStall;
  CommonFunctionPtr->SetFanOn         = CpmSetFanOn;
  CommonFunctionPtr->SetProchot       = CpmSetProchot;
  CommonFunctionPtr->GetSataMode      = CpmGetSataMode;
  CommonFunctionPtr->IsFchDevice      = CpmIsFchDevice;
  CommonFunctionPtr->GetSciMap        = CpmGetSciMap;
  CommonFunctionPtr->GetFchPcieAslName  = CpmGetFchPcieAslName;
  CommonFunctionPtr->GetBootMode      = CpmGetBootMode;
  CommonFunctionPtr->IsRtcWakeup      = CpmIsRtcWakeup;
  CommonFunctionPtr->GetRtc           = CpmGetRtc;
  CommonFunctionPtr->SetRtc           = CpmSetRtc;
  CommonFunctionPtr->GetAcpi          = CpmGetAcpi;
  CommonFunctionPtr->SetAcpi          = CpmSetAcpi;
  CommonFunctionPtr->GetSaveContext   = CpmGetSaveContext;
  CommonFunctionPtr->SetSaveContext   = CpmSetSaveContext;
  CommonFunctionPtr->GetAcpiSmiCmd    = CpmGetAcpiSmiCmd;
  CommonFunctionPtr->ReadSmbus        = CpmReadSmbus;
  CommonFunctionPtr->WriteSmbus       = CpmWriteSmbus;
  CommonFunctionPtr->ReadI2C          = CpmReadI2C;
  CommonFunctionPtr->WriteI2C         = CpmWriteI2C;
  DEBUG((DEBUG_INFO, "  LIB-FCH-%a-End\n", __FUNCTION__));
  return;
}
