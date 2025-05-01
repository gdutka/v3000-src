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

typedef struct _MEM_OC_SAFE_BOOT_CONTROL_INDEX_IO_STRUCT {
  UINT32                          Size;
  UINT8                           Method;
  UINT8                           IndexPort;    ///< 72
  UINT8                           DataPort;     ///< 73
  UINT8                           Address;      ///< 0x04
  UINT8                           UnlockValue;  ///< 0x55
  UINT8                           LockValue;    ///< 0xAA
} MEM_OC_SAFE_BOOT_CONTROL_INDEX_IO_STRUCT;

#define MEM_OC_VDDIO_CONTROL_METHOD_DIS  0        ///< VDDIO Voltage Control is disabled
#define MEM_OC_VDDIO_CONTROL_METHOD_EN   1        ///< VDDIO Voltage Control is enabled when Memory OC mode
#define MEM_OC_VDDIO_CONTROL_METHOD_DEF  2        ///< VDDIO Voltage Control default initialization is enabled
#define MEM_OC_VDDIO_CONTROL_ACCESS_TYPE_DIS      0
#define MEM_OC_VDDIO_CONTROL_ACCESS_TYPE_INDEX_IO 1
#define MEM_OC_VDDIO_CONTROL_SMBUS_HC_0  0
#define MEM_OC_VDDIO_CONTROL_SMBUS_HC_1  1
#define MEM_OC_VDDIO_CONTROL_SMBUS_WRITE_BYTE 1
#define MEM_OC_VDDIO_CONTROL_SMBUS_WRITE_WORD 2
#define MEM_OC_VDDIO_CONTROL_SMBUS_ATTRIBUTE_FIXED     0   ///< From SmbusData0, SmbusData1
#define MEM_OC_VDDIO_CONTROL_SMBUS_ATTRIBUTE_UPDATABLE 1   ///< From IndexIo when Memory OC mode

typedef struct _MEM_OC_VDDIO_VOLT_CTRL_INFO {
  UINT8                           SmbusHc;
  UINT8                           SmbusAdderess;
  UINT8                           SmbusHostCmd;
  UINT8                           Operation;
  UINT8                           Attr;
  UINT8                           SmbusData0;
  UINT8                           SmbusData1;
} MEM_OC_VDDIO_VOLT_CTRL_INFO;

typedef struct _MEM_OC_VDDIO_VOLT_CTRL_INDEX_IO {
  UINT8                           AccessType;   ///< Index IO
  UINT8                           IndexPort;    ///< 72
  UINT8                           DataPort;     ///< 73
  UINT8                           Offset;       ///< 0x04
} MEM_OC_VDDIO_VOLT_CTRL_INDEX_IO;

typedef struct _MEM_OC_VDDIO_CONTROL_SMBUS_STRUCT {
  UINT32                          Size;
  UINT8                           Method;
  UINT8                           NumOfCtrlInfo;
  MEM_OC_VDDIO_VOLT_CTRL_INDEX_IO VoltCtrlIndexIoData0;
  MEM_OC_VDDIO_VOLT_CTRL_INDEX_IO VoltCtrlIndexIoData1;
  MEM_OC_VDDIO_VOLT_CTRL_INFO     VoltCtrlInfo[];
} MEM_OC_VDDIO_CONTROL_SMBUS_STRUCT;

typedef struct _MEM_OC_CONFIG_STRUCT {
  UINT32                                    Size;
  MEM_OC_SAFE_BOOT_CONTROL_INDEX_IO_STRUCT  SafeBootCtrlIndexIo;
  MEM_OC_VDDIO_CONTROL_SMBUS_STRUCT         MemVddioCtrl;
} MEM_OC_CONFIG_STRUCT;



#endif // _APCB_MEM_OVERCLOCK_CONFIG_H_



