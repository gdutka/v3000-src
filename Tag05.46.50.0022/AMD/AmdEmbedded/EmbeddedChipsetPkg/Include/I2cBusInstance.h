/** @file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _I2C_BUS_INSTANCE_H_
#define _I2C_BUS_INSTANCE_H_

#include <Protocol/I2cMaster.h>
#include <Protocol/DevicePath.h>
#include <Protocol/I2cBusConfigurationManagement.h>

//
// Forward reference for pure ANSI compatability
//
typedef struct _I2C_BUS_INSTANCE                    I2C_BUS_INSTANCE;  

typedef
UINT8
(EFIAPI *I2C_IO_READ8) (
  IN      I2C_BUS_INSTANCE          *Private,
  IN      UINT16                    Offset
  );

typedef
UINT16
(EFIAPI *I2C_IO_READ16) (
  IN      I2C_BUS_INSTANCE          *Private,
  IN      UINT16                    Offset
  );

typedef
UINT32
(EFIAPI *I2C_IO_READ32) (
  IN      I2C_BUS_INSTANCE          *Private,
  IN      UINT16                    Offset
  );

typedef
VOID
(EFIAPI *I2C_IO_WRITE8) (
  IN      I2C_BUS_INSTANCE          *Private,
  IN      UINT16                    Offset,
  IN      UINT8                     Data
  );

typedef
VOID
(EFIAPI *I2C_IO_WRITE16) (
  IN      I2C_BUS_INSTANCE          *Private,
  IN      UINT16                    Offset,
  IN      UINT16                    Data
  );

typedef
VOID
(EFIAPI *I2C_IO_WRITE32) (
  IN      I2C_BUS_INSTANCE          *Private,
  IN      UINT16                    Offset,
  IN      UINT32                    Data
  );

enum CLK_SPEED{
  I2C_STANDARD = 0,
  I2C_FAST,
  I2C_HIGH,
  MAX_I2C_SPEED
};

typedef 
struct _SPEED_CLOCK_INFO{
  UINT16   SclHcnt;
  UINT16   SclLcnt;
  UINT8    SdaHold;
  UINT8    Reserved1;
  UINT16   Reserved2;  
}SPEED_CLOCK_INFO;

typedef
UINTN
(*GET_DEVICE_MODE) (
  IN I2C_BUS_INSTANCE    *I2cBusInstance
  );

typedef
VOID
(*SWITCH_DEVICE_MODE) (
  IN I2C_BUS_INSTANCE    *I2cBusInstance,
  IN UINT32              DeviceMode
  );


typedef
BOOLEAN
(*GPIO_EVENT_ARRIVED)(
  IN  I2C_BUS_INSTANCE   *I2cBusInstance,
  IN  UINT8              IntGpioController,  
  IN  UINT8              PadNumber, //GpioIntPin,
  IN  UINT8              ActiveLevel
  );

typedef
VOID
(*CHIP_INIT)(
  IN  I2C_BUS_INSTANCE   *this
  );

typedef
struct _I2C_INFO{
  UINT32              Cpuid;
  UINT16              CRG;  // I2C DMA Controller Registers offset
  UINT16              SdaHoldTimeLen; // I2C SDA Hold Time Length
  SPEED_CLOCK_INFO    SpeedClockInfo[MAX_I2C_SPEED];
  CHIP_INIT           Init;
  GET_DEVICE_MODE     GetDeviceMode;
  SWITCH_DEVICE_MODE  SwitcDevhMode;
  GPIO_EVENT_ARRIVED  GpioArrived;  
}I2C_INFO;

typedef 
struct _PCI_DEV_DEF{
  UINTN  Seg;
  UINTN  Bus;
  UINTN  Dev;
  UINTN  Func;
}PCI_DEV_DEF;

//
// Declare I2C Bus Instance
//
struct _I2C_BUS_INSTANCE {
  //
  // Private Data
  //
  UINTN                 Signature;
  
  EFI_HANDLE            Handle;

  LIST_ENTRY            AsyncQueue;

  UINTN                 CacheMode;
  UINTN                 CacheAvailable;
  UINTN                 CacheSize;
  UINT8                 *Cache;
  
  UINT64                I2cBase;
  UINT32                I2cBaseLen;
  UINT32                I2cBusSpeed;
    
  UINT32                GpioBase;
  UINT32                MaxFifoLen;
  UINT32                IntervalTime;
  
  UINTN                 RetryCount;
  UINTN                 CurrentRetryCount;

  EFI_EVENT             TimEvent;

  PCI_DEV_DEF           PciI2c;
  
  I2C_IO_READ8          I2cHcRead8;
  I2C_IO_READ16         I2cHcRead16;
  I2C_IO_READ32         I2cHcRead32;
  
  I2C_IO_WRITE8         I2cHcWrite8;
  I2C_IO_WRITE16        I2cHcWrite16;
  I2C_IO_WRITE32        I2cHcWrite32;
  
  I2C_INFO              Info;  
  //
  // Published interface
  //
  EFI_DEVICE_PATH_PROTOCOL                       *DevicePath;  
  EFI_I2C_MASTER_PROTOCOL                        MasterApi;
  EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL  ConfigApi;
};

#endif
