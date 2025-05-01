/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#ifndef _DASH_MCTP_SMBUS_H
#define _DASH_MCTP_SMBUS_H

//
// Standard header files included by modules in this driver.
//
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/SmbusLib.h>
#include <Library/CommonSetupLib.h>
#include <Library/DashAsfSmbusLib.h>
#include <Library/DashAsfBaseLib.h>

//
// Driver Consumed Protocol Prototypes
//
#include <Protocol/SmbusHc.h>
#include <Protocol/MctpIo.h>

#define MAX_SMBUS_DEVICES       107 // Max number of SMBus devices
#define LAN_SMBUS_SLAVE_ADDRESS 0xC8
#define MCTP_EID_VALUE          0x09

typedef struct _SMBUS_INSTANCE SMBUS_INSTANCE;

typedef
UINT8
(EFIAPI *SMBUS_IO_READ) (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  Offset
  );

typedef
VOID
(EFIAPI *SMBUS_IO_WRITE) (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  Offset,
  IN UINT8                                                  Data
  );

typedef
BOOLEAN
(EFIAPI *SMBUS_IO_DONE) (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  *StsReg
  );

///
/// SMBUS Instance
///
typedef struct _SMBUS_INSTANCE {
  UINTN                                     Signature;                      ///< Signature
  EFI_HANDLE                                Handle;                         ///< Handle
  UINT16                                    SmbusIoBase;                    ///< SMBUS IO base
  SMBUS_IO_READ                             SmbusIoRead;                    ///< SMBUS IO read
  SMBUS_IO_WRITE                            SmbusIoWrite;                   ///< SMBUS IO write
  SMBUS_IO_DONE                             IoDone;                         ///< IO Done flag
  UINT32                                    StallTime;                      ///< Stall time
  //
  // Published interface
  //
  EFI_SMBUS_HC_PROTOCOL                     SmbusController;                ///< SMBUS controller protocol
  UINT8                                     DeviceMapEntries;               ///< Device map entries
  EFI_SMBUS_DEVICE_MAP                      DeviceMap [MAX_SMBUS_DEVICES];  ///< Device map array
  LIST_ENTRY                                NotifyFunctionList;             ///< Notify function list
  EFI_EVENT                                 NotificationEvent;              ///< Notification event
} SMBUS_INSTANCE;

#define SMBUS_INSTANCE_SIGNATURE  SIGNATURE_32 ('p', 's', 'm', 'b')

#define SMBUS_INSTANCE_FROM_THIS(a) \
  CR (a, \
      SMBUS_INSTANCE, \
      SmbusController, \
      SMBUS_INSTANCE_SIGNATURE \
      )

#endif
