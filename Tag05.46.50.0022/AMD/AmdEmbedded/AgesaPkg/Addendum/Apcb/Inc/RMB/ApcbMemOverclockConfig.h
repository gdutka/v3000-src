/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _APCB_MEM_OVERCLOCK_CONFIG_H_
#define _APCB_MEM_OVERCLOCK_CONFIG_H_
//
// @APCB_START
//


///===============================================================================
/// Mempory Overclock configuration
#define MEM_OC_SAFE_BOOT_CONTROL_METHOD_DIS  0      ///< Safe Boot is disabled
#define MEM_OC_SAFE_BOOT_CONTROL_METHOD_G3   1      ///< Safe Boot is applied only when mechanical off
#define MEM_OC_SAFE_BOOT_CONTROL_METHOD_G2   2      ///< Safe Boot is applied only when soft-off or suspend to disk

#define MEM_OC_CONTROL_METHOD_DIS  0        ///< VDDIO Voltage Control is disabled
#define MEM_OC_CONTROL_METHOD_EN   1        ///< VDDIO Voltage Control is enabled when Memory OC mode
#define MEM_OC_CONTROL_METHOD_DEF  2        ///< VDDIO Voltage Control default initialization is enabled
#define MEM_OC_CONTROL_ACCESS_TYPE_DIS      0
#define MEM_OC_CONTROL_ACCESS_TYPE_INDEX_IO 1
#define MEM_OC_CONTROL_SMBUS_HC_0  0
#define MEM_OC_CONTROL_SMBUS_HC_1  1
#define MEM_OC_CONTROL_SMBUS_WRITE_BYTE 1
#define MEM_OC_CONTROL_SMBUS_WRITE_WORD 2
#define MEM_OC_CONTROL_EC_WRITE         3
#define MEM_OC_CONTROL_EC_PULL_NE       4
#define MEM_OC_CONTROL_SMBUS_ATTRIBUTE_FIXED     0   ///< From SmbusData0, SmbusData1
#define MEM_OC_CONTROL_SMBUS_ATTRIBUTE_UPDATABLE 1   ///< From IndexIo when Memory OC mode
#define MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED        2
#define MEM_OC_CONTROL_EC_ATTRIBUTE_UPDATABLE    3
#define MEM_OC_ACCESS_METHOD_SMBUS 0
#define MEM_OC_ACCESS_METHOD_EC    1

typedef struct _MEM_OC_SAFE_BOOT_CONTROL_INDEX_IO_STRUCT {
  UINT8                          Method;
  UINT8                          IndexPort;    ///< 72
  UINT8                          DataPort;     ///< 73
  UINT8                          Address;      ///< 0x04
  UINT8                          UnlockValue;  ///< 0x55
  UINT8                          LockValue;    ///< 0xAA
  UINT16                         Reserved_0;
  UINT32                         Reserved_1;
} MEM_OC_SAFE_BOOT_CONTROL_INDEX_IO_STRUCT;

typedef enum _MEM_OC_CONFIG_SIGNATURE {
  MEM_OC_CONFIG_VDDIO,
  MEM_OC_CONFIG_VDD,
  MEM_OC_CONFIG_SAFE_BOOT
} MEM_OC_CONFIG_SIGNATURE;

typedef struct _MEM_OC_VOLT_CTRL_INFO {
  UINT32                          SmbusHc;
  UINT32                          SmbusAddress;
  UINT32                          SmbusHostCmd;
  UINT8                           Operation;
  UINT8                           Attr;
  UINT8                           SmbusData0;
  UINT8                           SmbusData1;
} MEM_OC_VOLT_CTRL_INFO;

typedef struct _MEM_OC_VOLT_CTRL_INFO_EC {
  UINT32                          EcCmdStsPort;
  UINT32                          EcDataPort;
  UINT32                          Offset;
  UINT8                           Operation;
  UINT8                           Attr;
  UINT8                           EcData;
  UINT8                           Reserved;
} MEM_OC_VOLT_CTRL_INFO_EC;

typedef struct _MEM_OC_VOLT_CTRL_INDEX_IO {
  UINT8                           AccessType;   ///< Index IO
  UINT8                           IndexPort;    ///< 72
  UINT8                           DataPort;     ///< 73
  UINT8                           Offset;       ///< 0x04
} MEM_OC_VOLT_CTRL_INDEX_IO;

typedef struct _MEM_OC_VDDIO_CONTROL_SMBUS_STRUCT {
  UINT16                          Method;
  UINT16                          NumOfCtrlInfo;
  MEM_OC_VOLT_CTRL_INDEX_IO       VoltCtrlIndexIoData0;
  MEM_OC_VOLT_CTRL_INDEX_IO       VoltCtrlIndexIoData1;
  MEM_OC_VOLT_CTRL_INFO           VoltCtrlInfo[4];
} MEM_OC_VOLT_CONTROL_STRUCT;

typedef struct _MEM_OC_CONFIG_STRUCT {
  MEM_OC_CONFIG_SIGNATURE                   Signature;
  MEM_OC_VOLT_CONTROL_STRUCT                MemVoltCtrl;
} MEM_OC_CONFIG_STRUCT;

typedef struct _MEM_OC_SAFE_BOOT_CONFIG_STRUCT {
  MEM_OC_CONFIG_SIGNATURE                   Signature;
  MEM_OC_SAFE_BOOT_CONTROL_INDEX_IO_STRUCT  SafeBootCtrlIndexIo;
} MEM_OC_SAFE_BOOT_CONFIG_STRUCT;

#endif // _APCB_MEM_OVERCLOCK_CONFIG_H_



