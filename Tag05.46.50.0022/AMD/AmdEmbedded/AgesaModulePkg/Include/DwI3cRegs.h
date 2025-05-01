/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEI driver.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  FCH PEIM
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 10:28:05 -0800 (Tue, 09 Dec 2014) $
 *
 */

#ifndef _FCH_DW_I3C_REGS_H_
#define _FCH_DW_I3C_REGS_H_

//#include "FchPlatform.h"
//
// EQUate Definitions
//

// "DDR5_SPD5" spec says supporting upto 12.5 MHz for I3C.
// Refer to I3C User Guide page 116.
#define AMD_I3C_MAX_CLOCK_HZ    (125*1000*1000) // Maximum 125 MHz
#define AMD_I3C_SPEED           (12500*1000) // Maximum 12.5 MHz

// Example of how to calculate frequency PP frequency.Try different frequency? OD = 1MHz, PP = 4MHz according to email "RE: I3C bus and SPD talking"
// If PP = 4MHz. 4MHz -> period = 1/4MHz = 250ns. I3C Push-Pull SCL Minimum High Period/core_clk period = 250ns/5ns ~=50 = 0x32
// Thus, PP_LCNT + PP_HCNT = 0x20.
// And PP_LCNT should be greater than HCNT.
// Note: the frequency definition below is not calculated based on the example above.
#define AMD_I3C_PP_LCNT        (0x9)
#define AMD_I3C_PP_HCNT        (0x9 << 16)

// Example of how to calculate frequency OD frequency.
// 1MHz -> period = 1/1MHz = 1000ns. SCL low = Open-Drain SCL Minimum Low Period/core_clk period = 1000ns/5ns ~=200 = 0xC8
// Thus, OD_LCNT + OD_HCNT = 0xC8.
// And OD_LCNT should be greater than HCNT. (0x7C + 0x4C = 0xC8)
#define AMD_I3C_OD_LCNT        (0x7C)
#define AMD_I3C_OD_HCNT        (0x4C << 16)

#define I2C_MODE_FM_SPEED      (0x0)
#define I2C_MODE_FM_PLUS_SPEED (0x1)
#define I2C_MODE_I2C_FM_SPEED  (0x7) // when sending SETAASA, must set speed to 0x7 according to design team
#define I3C_MODE_SDR1_SPEED    (0x1)
#define I3C_MODE_SDR2_SPEED    (0x2)

//------------------------------------------------------------------;
//   DW I3cBus Registers                                           ;
//------------------------------------------------------------------;
/*
 * Registers offset
 */
#define DW_DEVICE_CTRL                               (0x0)
#define DW_DEVICE_ADDR                               (0x4)
#define DW_HW_CAPABILITY                             (0x8)
#define DW_COMMAND_QUEUE_PORT                        (0xC)
#define DW_RESPONSE_QUEUE_PORT                       (0x10)
#define DW_RX_DATA_PORT                              (0x14)
#define DW_TX_DATA_PORT                              (0x14)
#define DW_QUEUE_THLD_CTRL                           (0x1C)
#define DW_DATA_BUFFER_THLD_CTRL                     (0x20)
#define DW_RESET_CTRL                                (0x34)
#define DW_INTR_STATUS                               (0x3C)
#define DW_INTR_STATUS_EN                            (0x40)
#define DW_INTR_SIGNAL_EN                            (0x44)
#define DW_QUEUE_STATUS_LEVEL                        (0x4C)
#define DW_DATA_BUFFER_STATUS_LEVEL                  (0x50)
#define DW_PRESENT_STATE                             (0x54)
#define DW_DEVICE_ADDR_TABLE_POINTER                 (0x5C)
#define DW_DEVICE_CTRL_EXTENDED                      (0xB0)
#define DW_SCL_I3C_OD_TIMING                         (0xB4)
#define DW_SCL_I3C_PP_TIMING                         (0xB8)
#define SCL_EXT_TERMN_LCNT_TIMING                    (0xCC)
#define DW_SDA_HOLD_DLY_TIMING                       (0xD0)
#define DW_BUS_FREE_AVAIL_TIMING                     (0xD4)
#define DW_QUEUE_SIZE_CAPABILITY                     (0xE8)
#define DW_DEV_CHAR_TABLE1_LOC4                      (0x20C) // Device Dynamic Address assigned bit[7:0]
#define DW_DEV_CHAR_TABLE2_LOC4                      (0x21C)
#define DW_DEV_CHAR_TABLE3_LOC4                      (0x22C)
#define DW_DEV_ADDR_TABLE1_LOC1                      (0x2C0)

// Device Control Register (DW_DEVICE_CTRL)
#define DWC_MIPI_I3C_CONTROLLER_ENABLE               (1UL << 31)
#define DWC_RESUME                                   (1UL << 30)
#define DWC_ABORT                                    (1UL << 29)
#define I2C_SLAVE_PRESENT                            (1UL << 7)
#define IBA_INCLUDE                                  (1UL << 0)

// Device Address Register (DW_DEVICE_ADDR 0x4)
#define DYNAMIC_ADDR_VALID         (1UL << 31)
#define DYNAMIC_ADDR(a)            ((a & 0x7F) << 16) // bit [22:16]
#define STATIC_ADDR_VALID          (1 << 15)
#define STATIC_ADDR(a)             (a & 0x7F) // bit 0-6


// Command Queue Port Register (DW_COMMAND_QUEUE_PORT 0xC)
// Address Assignment Command Data Structure (I3C databook 2.7.6.1.4)
// CMD_ATTR bit 2-0
#define TRANSFER_CMD               (0x0)
#define TRANSFER_ARG               (0x1)
#define SHORT_DATA_ARG             (0x2)
#define ADDR_ASSIGNMENT_CMD        (0x3)
#define SETDASA_CCC      (0x87 << 7) // bit[14:7]

#define RSTDAA_BROADCAST 0x6 // will use Transfer Cmd struct, so no need to shift bits
#define SETNEWDA_CCC     0x88 // will use Transfer Cmd struct, so no need to shift bits
#define SETAASA_CCC      0x29 // will use Transfer Cmd struct, so no need to shift bits

// DEV_INDX
#define DEV_INDX(a)                ((a & 0x1F) << 16) // bit[20:16]
// DEV_COUNT
#define DEV_COUNT(a)               ((a & 0x1F) << 21) // bit[25:21]
// Response On Completion
#define ROC                        (1 << 26)
// Termination On Completion. (Controls whether a STOP need to be issued after the transfer)
#define TOC                        (1 << 30)

typedef union {
  /// Bitfields of Command Queue Port Register (DW_COMMAND_QUEUE_PORT 0xC)
  struct {
    UINT32 CmdAttr:3;              ///< 0:2 Command Attribute
    UINT32 Tid:4;                  ///< 3:6 Transaction ID
    UINT32 Cmd:8;                  ///< 7:14 Transfer Command
    UINT32 Cp:1;                   ///< 15 Command Present
    UINT32 DevIndx:5;               ///< 16:20 Device Index
    UINT32 Speed:3;                 ///< 21:23 Speed
    UINT32 Rsrv1:2;                 ///< 24:25 Reserved
    UINT32 Roc:1;                  ///< 26 Response On Completion
    UINT32 Sdap:1;                  ///< 27 Short Data Argument Present
    UINT32 RnW:1;                   ///< 28 Read and Write. ( 0 - Write Transfer. 1 - Read Transfer)
    UINT32 Rsrv2:1;                 ///< 29 Reserved
    UINT32 Toc:1;                   ///< 30 Termination On Completion
    UINT32 Rsrv3:1;                 ///< 31 Reserved
  } TransferCmdData;
  struct {
    UINT32 CmdAttr:3;               ///< 0:2 Command Attribute
    UINT32 ByteStrb:3;              ///< 3:5 Byte Strobe
    UINT32 Rsrv:2;                  ///< 6:7 Reserved
    UINT32 DataByte0:8;             ///< 8:15 Data Byte-0
    UINT32 DataByte1:8;             ///< 16:23 Data Byte-1
    UINT32 DataByte2:8;             ///< 24:31 Data Byte-2
  } ShortDataArg;
  struct {
    UINT32 CmdAttr:3;              ///< 0:2 Command Attribute
    UINT32 Rsrv:13;                ///< 3: 15 Reserved
    UINT32 Dl:16;                  ///< 16:31 Data Length
  } TransferArgData;
  struct {
    UINT32 CmdAttr:3;              ///< 0:2 Command Attribute
    UINT32 Tid:4;                  ///< 3: 6 Transaction ID
    UINT32 Cmd:8;                  ///< 7:14 Transfer Command
    UINT32 Rsrv1:1;                 ///< 15 Command Present
    UINT32 DevIndx:5;               ///< 16:20 Device Index
    UINT32 DevCount:5;              ///< 21:25 Device Count
    UINT32 Roc:1;                   ///< 26 Response On Completion
    UINT32 Rsrv2:3;                 ///< 27:29 Short Data Argument Present
    UINT32 Toc:1;                   ///< 30 Termination On Completion
    UINT32 Rsrv3:1;                 ///< 31 Reserved
  } AddrAssignCmdData;
  UINT32  Value;
} I3C_CMD_DATA_STRUCTURE;

// DW_RESPONSE_QUEUE_PORT 0x10
#define DL_MASK                       (0xFFFFUL << 0)
#define CCCT_MASK                     (0xFFUL << 16)
#define TID_MASK                      (0xFUL << 24)
#define ERR_STS_MASK                  (0xFUL << 28)
#define CRC_ERR                       (0x1UL << 28)
#define PARITY_ERR                    (0x2UL << 28)
#define FRAME_ERR                     (0x3UL << 28)
#define I3C_BROADCASE_ADDR_NACK_ERR   (0x4UL << 28)
#define ADDR_NACKED_ERR               (0x5UL << 28)
#define BUFFER_OVERFLOW_UNDERFLOW_ERR (0x6UL << 28)
#define TRANSFER_ABORTED_ERR          (0x8UL << 28)
#define I2C_SLAVE_WRITE_DATA_NACK_ERR (0x9UL << 28)

// Queue Threshold Control Register (DW_QUEUE_THLD_CTRL 0x1C)
#define CMD_EMPTY_BUF_THLD(a)         ((a & 0xFF) << 0)
#define RESP_BUF_THLD(a)              ((a & 0xFF) << 8)
#define IBI_DATA_THLD(a)              ((a & 0xFF) << 16)
#define IBI_STATUS_THLD(a)            ((a & 0xFF) << 24)

// DW_DATA_BUFFER_THLD_CTRL 0x20
#define TX_EMPTY_BUF_THLD(a)         ((a & 0x7) << 0)
#define RX_BUF_THLD(a)               ((a & 0x7) << 8)
#define TX_START_THLD(a)             ((a & 0x7) << 16)
#define RX_START_THLD(a)             ((a & 0x7) << 24)

// Reset Control Register (DW_RESET_CTRL 0x34)
#define SOFT_RST                   (1 << 0)
#define TX_FIFO_RST                (1 << 3)
#define RX_FIFO_RST                (1 << 4)

// Interrupt Status Register (DW_INTR_STATUS 0x3C)
#define BUS_RESET_DONE_STS         (1 << 15)
#define BUSOWNER_UPDATED_STS       (1 << 13)
#define IBI_UPDATED_STS            (1 << 12)
#define READ_REQ_RECV_STS          (1 << 11)
#define DEFSLV_STS                 (1 << 10)
#define TRANSFER_ERR_STS           (1 << 9)
#define DYN_ADDR_ASSGN_STS         (1 << 8)
#define CCC_UPDATED_STS            (1 << 6)
#define TRANSFER_ABORT_STS         (1 << 5)
#define RESP_READY_STS             (1 << 4)
#define CMD_QUEUE_READY_STS        (1 << 3)
#define IBI_THLD_STS               (1 << 2)
#define RX_THLD_STS                (1 << 1)
#define TX_THLD_STS                (1 << 0)

// Interrupt Status Enable Register (DW_INTR_STATUS_EN 0x40)
#define BUS_RESET_DONE_STS_EN      (1 << 15)
#define TRANSFER_ERR_STS_EN        (1 << 9)
#define TRANSFER_ABORT_STS_EN      (1 << 5)
#define RESP_READY_STS_EN          (1 << 4)
#define CMD_QUEUE_READY_STS_EN     (1 << 3)
#define IBI_THLD_STS_EN            (1 << 2)
#define RX_THLD_STS_EN             (1 << 1)
#define TX_THLD_STS_EN             (1 << 0)

// Interrupt Signal Enable Register (DW_INTR_SIGNAL_EN 0x44)
#define BUS_RESET_DONE_SIGNAL_EN      (1 << 15)
#define TRANSFER_ERR_SIGNAL_EN        (1 << 9)
#define TRANSFER_ABORT_SIGNAL_EN      (1 << 5)
#define RESP_READY_SIGNAL_EN          (1 << 4)
#define CMD_QUEUE_READY_SIGNAL_EN     (1 << 3)
#define IBI_THLD_SIGNAL_EN            (1 << 2)
#define RX_THLD_SIGNAL_EN             (1 << 1)
#define TX_THLD_SIGNAL_EN             (1 << 0)

// Present State Register (DW_PRESENT_STATE 0x54)
#define SCL_LINE_SIGNAL_LEVEL      (1 << 0)
#define SDA_LINE_SIGNAL_LEVEL      (1 << 1)
#define CURRENT_MASTER             (1 << 2)
#define CM_TFR_STS                 (0x3F << 8)
#define MASTER_IDLE                (1 << 28)

#define CM_TFR_STS_IDLE_STATE      (0x0 << 8)
#define CM_TFR_STS_HALT_STATE      (0xF << 8)

// DW_DATA_BUFFER_STATUS_LEVEL
#define RX_BUF_BLR                 (0xFF << 16)

// I3C Open Drain Timing Register (SCL_I3C_OD_TIMING 0xB4)
#define I3C_OD_LCNT_MASK           (0xFFUL)
#define I3C_OD_HCNT_MASK           (0xFF0000UL)

// I3C Push Pull Timing Register (SCL_I3C_PP_TIMING 0xB8)
#define I3C_PP_LCNT_MASK           (0xFFUL)
#define I3C_PP_HCNT_MASK           (0xFF0000UL)

// DW_SDA_HOLD_DLY_TIMING 0xD0
#define SDA_TX_HOLD                (0x1 << 16)

// DW_BUS_FREE_AVAIL_TIMING 0xD4
#define BUS_FREE_TIME              (0x110)

// DW_QUEUE_SIZE_CAPABILITY 0xE8
#define TX_BUF_SIZE_MASK           (0xF)
#define RX_BUF_SIZE_MASK           (0xF0)
#define TX_BUF_SIZE_OFFSET         (0)
#define RX_BUF_SIZE_OFFSET         (4)
#define TX_BUF_SIZE                (0x5 << TX_BUF_SIZE_OFFSET) // Transmit buffer size (0x5 = 64 DWORDS)
#define RX_BUF_SIZE                (0x5 << RX_BUF_SIZE_OFFSET) // Receive buffer size (0x5 = 64 DWORDS)
typedef struct _QUEUE_SIZE_CAPABILITY {
  UINT32 key; // 0,1,2,3,4,5
  UINT32 value; // (2,4,8,16,32,64) DWORDS = (2,4,8,16,32,64)*4 BYTES
} QUEUE_SIZE_CAPABILITY;

// Device Address Table Location of Device1 (DW_DEV_ADDR_TABLE1_LOC1 0x2C0)
#define DEV_NACK_RETRY_CNT(a)           ((a & 0x3) << 29) // bit[30:29]
#define DEV_DYNAMIC_ADDR(a)             ((a & 0x7F) << 16) // bit[23:16]
#define DEV_DYNAMIC_ADDR_PARITY_SHIFT   (23) // bit 23
#define SIR_REJECT                      (1 << 13) // bit 13
#define IBI_PEC_EN                      (1 << 11) // bit 11
#define DEV_STATIC_ADDR(a)              (a & 0x7F) // bit[6:0]
#define DEV_TYPE(a)                     ((a & 0x1UL) << 31) // bit[31]

// ToDo: ?? What the timeout values should be? Copied from I2c directly
/* timeout and retry values */
#define DW_BUS_WAIT_TIMEOUT                     (20) /* ms */
#define DW_RX_DATA_RETRY                        (10)
#define DW_TX_DATA_RETRY                        (100)
#define DW_STATUS_WAIT_RETRY                    (10)
#define DW_BUS_WAIT_INACTIVE                    (20)

// I3cRx/Tx Data Buffer
#define DW_I3C_DATA_MASK                        (0x00FFUL)

#endif // _FCH_I3C_H_


