/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#ifndef _ASF_MSG_H
#define _ASF_MSG_H

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/SmbusLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DashAsfSmbusLib.h>
#include <Library/DashAsfBaseLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/CommonSetupLib.h>
#include <IndustryStandard/Pci22.h>

#include <Protocol/SmbusHc.h>

#define SYSTEM_STATE_S0   0x00
#define SYSTEM_STATE_S3   0x03
#define SYSTEM_STATE_S4   0x04
#define SYSTEM_STATE_S5   0x05

//
// ASF_MESSAGE_SENSOR_DEVICE_AND_SYSTEM_STATE
//
#define ASF_MESSAGE_SENSOR_DEVICE_AND_SYSTEM_STATE    0x01
// ASF_MESSAGE_SENSOR_DEVICE_AND_SYSTEM_STATE SUB-COMMAND VALUE
#define ASF_MESSAGE_SYSTEM_STATE_SUBCOMMAND           0x18


//
// ASF_MESSAGE_MANAGEMENT_CONTROL
//
#define ASF_MESSAGE_MANAGEMENT_CONTROL                0x02
// ASF_MESSAGE_MANAGEMENT_CONTROL SUB-COMMAND VALUE
#define ASF_MESSAGE_START_WATCH_DOG_TIMER_SUBCOMMAND  0x13
#define ASF_MESSAGE_STOP_WATCH_DOG_TIMER_SUBCOMMAND   0x14


#define NO_BOOT_OPTIONS_RESPONSE_SIZE                 0x02
#define RETURN_BOOT_OPTIONS_RESPONSE_SIZE             0x0D
#define BOOT_OPTIONS_CLEAR_SIZE                       0x02
//
// ASF_MESSAGE_CONFIGURATION
//
#define ASF_MESSAGE_CONFIGURATION                     0x03
// ASF_MESSAGE_CONFIGURATION SUB-COMMAND VALUE
#define ASF_MESSAGE_CLEAR_BOOT_OPTIONS                0x15
#define ASF_MESSAGE_RETURN_BOOT_OPTIONS               0x16
#define ASF_MESSAGE_NO_BOOT_OPTIONS                   0x17


#define MESSAGE_WITHOUT_RETRANSMISSION_SIZE           0x0D
//
// ASF_MESSAGE_MESSAGING
//
#define ASF_MESSAGE_MESSAGING                         0x04
// ASF_MESSAGE_MESSAGING SUB-COMMAND VALUE
#define ASF_MESSAGE_WITH_RETRANSMISSION               0x15
#define ASF_MESSAGE_WITHOUT_RETRANSMISSION            0x16


//ASF VERSION NUMBER
#define ASF_MESSAGE_VERSIONNUMBER                     0x10

//ASF EVENT SENSOR TYPE
#define ASF_EVENT_SENSOR_TYPE_TEMPERATURE             0x01
#define ASF_EVENT_SENSOR_TYPE_VOLTAGE                 0x02
#define ASF_EVENT_SENSOR_TYPE_FAN                     0x04
#define ASF_EVENT_SENSOR_TYPE_CHASSIS_INTRUSION       0x05
#define ASF_EVENT_SENSOR_TYPE_SYS_FW_ERR_OR_PROGRESS  0x0F
#define ASF_EVENT_SENSOR_TYPE_OS_CRITICAL_STOP        0x20
#define ASF_EVENT_SENSOR_TYPE_WATCHDOG2               0x23
#define ASF_EVENT_SENSOR_TYPE_ENTITY_PRESENCE         0x25

#define ASF_EVENT_SENSOR_TYPE_

//ASF EVENT TYPE
#define ASF_EVENT_TYPE_SENSOR_SPECIFIC                0x6F

//ASF EVENT OFFSET
#define ASF_EVENT_OFFSET_TIMER_EXPIRED                  0x00
#define ASF_EVENT_OFFSET_SYSTEM_FIRMWARE_ERROR          0x00
#define ASF_EVENT_OFFSET_SYSTEM_FIRMWARE_HANG           0x01
#define ASF_EVENT_OFFSET_SYSTEM_FIRMWARE_PROGRESS_ENTRY 0x02
#define ASF_EVENT_OFFSET_SYSTEM_FIRMWARE_PROGRESS_EXIT  0x82

//ASF EVENT SOURCE TYPE
#define ASF_EVENT_SOURCE_TYPE_ASF_1_0_IMPLEMENTATION  0x68

//ASF EVENT SEVERITY
#define ASF_EVENT_SEVERITY_MONITOR                    0x01
#define ASF_EVENT_SEVERITY_NON_CRITICAL               0x08
#define ASF_EVENT_SEVERITY_CRITICAL_CONDITION         0x10

//ASF SENSOR DEVICE
#define ASF_SENSOR_DEVICE_UNSPECIFIED                 0xFF

//ASF SENSOR NUMBER
#define ASF_SENSOR_NUMBER_UNSPECIFIED                 0xFF

//ASF ENTITY CODE
#define ASF_ENTITY_CODE_UNSPECIFIED                   0x00
#define ASF_ENTITY_CODE_PROCESSOR                     0x03
#define ASF_ENTITY_CODE_DISK_OR_DISK_BAY              0x04
#define ASF_ENTITY_CODE_SYSTEM_BOARD                  0x07
#define ASF_ENTITY_CODE_ADD_IN_BOARD                  0x0B
#define ASF_ENTITY_CODE_MEMORY_DEVICE                 0x20
#define ASF_ENTITY_CODE_BIOS                          0x22
#define ASF_ENTITY_CODE_OS                            0x23

//ASF EVENT DATA0 FOR WATCHDOG
#define ASF_WATCHDOG_DATA0                            0x40

//ASF EVENT DATA1 FOR WATCHDOG
#define ASF_OS_BOOT_FAILURED                          0x03
#define ASF_SYSTEM_BOOT_FAILURED                      0x06

//ASF EVENT DATA0 FOR SYSTEM FW PROGRESS EVENT
#define ASF_SYSTEM_FW_PROGRESS_DATA0                  0x40

//ASF SYSTEM FIRMWARE PROGRESS EVENT CODE
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_UNSPECIFIED                 0x00
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_MEMORY_INITIAL              0x01
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_HDD_INITIA                  0x02
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_AP_INITIAL                  0x03
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_USER_AUTHENTICATION         0x04
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_USER_INITIAL_SYSTEM_SETUP   0x05
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_USB_RESOURCE_CONFIG         0x06
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_PCI_RESOURCE_CONFIG         0x07
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_OPTION_ROM_INITIAL          0x08
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_VIDEO_INITIAL               0x09
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_CACHE_INITIAL               0x0A
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_SMBUS_INITIAL               0x0B
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_KBC_INITIAL                 0x0C
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_EC_INITIAL                  0x0D
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_ATTACH_DOCK                 0x0E
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_ENABLE_DOCK                 0x0F
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_ENJECT_DOCK                 0x10
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_DISABLE_DOCK                0x11
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_OS_WAKE_UP                  0x12
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_BOOT_TO_OS                  0x13
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_MOTHERBOARD_INITIAL         0x14
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_RESERVED                    0x15
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_FDD_INITIAL                 0x16
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_KEYBOARD_TEST               0x17
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_POINTING_DEVICE_TEST        0x18
#define ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_BSP_INITIAL                 0x19


#define MAX_SMBUS_DEVICES         107 // Max number of SMBus devices
#define LAN_SMBUS_SLAVE_ADDRESS   0xC8

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

#pragma pack (1)
typedef struct {
  UINT8 SubCmd;
  UINT8 Version;
  UINT8 TimeoutLo;
  UINT8 TimeoutHi;
  UINT8 EventSensorType;
  UINT8 EventType;
  UINT8 EventOffset;
  UINT8 EventSourceType;
  UINT8 EventSeverity;
  UINT8 SensorDevice;
  UINT8 SensorNumber;
  UINT8 Entity;
  UINT8 EntityInstance;
  UINT8 Data0;
  UINT8 Data1;
  UINT8 Data2;
  UINT8 Data3;
  UINT8 Data4;
  UINT8 Data5;
} ASF_MSG_START_WATCHDOG_TIMER;

typedef struct {
  UINT8 SubCmd;
  UINT8 Version;
} ASF_MSG_STOP_WATCHDOG_TIMER;

typedef struct {
  UINT8 SubCmd;
  UINT8 Version;
  UINT8 EventSensorType;
  UINT8 EventType;
  UINT8 EventOffset;
  UINT8 EventSourceType;
  UINT8 EventSeverity;
  UINT8 SensorDevice;
  UINT8 SensorNumber;
  UINT8 Entity;
  UINT8 EntityInstance;
  UINT8 Data0;
  UINT8 Data1;
  UINT8 Data2;
  UINT8 Data3;
  UINT8 Data4;
  UINT8 Data5;
} ASF_MSG_WITHOUT_RETRANSMISSION;

typedef struct {
  UINT8 SubCmd;
  UINT8 Version;
  UINT8 SystemState;
} ASF_MSG_SET_SYSTEM_STATE;

typedef struct {
  UINT8  SubCmd;
  UINT8  Version;
  UINT32 IanaId;
  UINT8  SpecialCommand;
  UINT16 SpecialCommandParam;
  UINT16 BootOption;
  UINT16 OemParameters;
} ASF_MSG_BOOT_OPTIONS;

typedef struct {
  UINT8 SubCmd;
  UINT8 Version;
} ASF_MSG_BOOT_OPTIONS_CLEAR;
#pragma pack ()
#endif // _ASF_MSG_H
