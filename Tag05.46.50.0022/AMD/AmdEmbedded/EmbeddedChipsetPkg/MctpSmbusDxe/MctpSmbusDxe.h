/** @file

;*******************************************************************************
;* Copyright (c) 2013 - 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <Protocol/Metronome.h>
#include <Protocol/DashIoCfg.h>
#include <Protocol/MctpSmbus.h>
#include <Protocol/PciRootBridgeIo.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>

#include <MmioAccess.h>

#define STALL_TIME                                          1000000 // 1,000,000 microseconds = 1 second
#define MCTP_SMBUS_COMMAND                                  0x0F

#define HSTS                                                0x00 // Host Status Register R/W
  #define HBSY                                                0x01  // RO
  #define INTR                                                0x02 // R/WC
  #define DERR                                                0x04 // R/WC
  #define BERR                                                0x08 // R/WC
  #define FAIL                                                0x10 // R/WC
  #define BYTE_DONE_STS                                       0x80 // R/WC
#define HCTL                                                0x02  // Host Control Register R/W
  #define SMBUS_OP_BLOCK                                      0x14
  #define START                                               0x40  // WO
  #define PEC_ENABLE                                          0x80
  #define PEC_AUTO_APPEND                                     0x20
#define HCMD                                                0x03  // Host Command Register R/W
#define TSA                                                 0x04  // Transmit Slave Address Register R/W
#define HD0                                                 0x05  // Data 0 Register R/W
#define HD1                                                 0x06  // Data 1 Register R/W
#define HBD                                                 0x07  // Host Block Data Register R/W
#define LISTEN_ADDR                                         0x09
#define SET_DATA_READ_POINTER                               0x12
#define DATABANK_SELECT                                     0x13
  #define DATABANK0_FULL                                      0x04
  #define DATABANK1_FULL                                      0x08
  #define SET_READ_DATABANK0                                  0x00
  #define SET_READ_DATABANK1                                  0x10  // 0x80: SB700        0x10: SB800, SB900
#define SEMAPHORE                                           0x14
  #define HOST_SEMAPHORE                                      0x01
  #define CLR_HOST_SEMAPHORE                                  0x02
  #define EC_SEMAPHORE                                        0x04
#define SLAVE_EN                                            0x15
  #define SUSPEND_SLAVE                                       0x08


#define MCTP_SMBUS_SIGNATURE                                SIGNATURE_32('M', 'C', 'S', 'M')

#define MCTP_SMBUS_CONTEXT_FROM_THIS(a) \
  CR (a, \
      MCTP_SMBUS_CONTEXT, \
      MctpSmbus, \
      MCTP_SMBUS_SIGNATURE \
      )

typedef struct  _MCTP_SMBUS_CONTEXT MCTP_SMBUS_CONTEXT;

typedef
VOID
(EFIAPI *SMBUS_SLAVE_ENABLE_LISTENING) (
  IN  MCTP_SMBUS_CONTEXT *This
);

typedef
VOID
(EFIAPI *SMBUS_SLAVE_DISABLE_LISTENING) (
  IN  MCTP_SMBUS_CONTEXT *This
);

typedef struct _MCTP_SMBUS_CONTEXT {
  UINT32                        Signature;
  MCTP_SMBUS_PROTOCOL           MctpSmbus;
  DASH_IO_CFG_PROTOCOL           *DashIoCfg;
  UINT32                        StallTime;
  SMBUS_SLAVE_ENABLE_LISTENING  EnableListening;
  SMBUS_SLAVE_DISABLE_LISTENING DisableListening;
} MCTP_SMBUS_CONTEXT;

