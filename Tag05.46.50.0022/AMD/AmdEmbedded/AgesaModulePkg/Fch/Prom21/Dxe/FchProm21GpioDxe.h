/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_GPIO_DXE_H_
#define _FCH_PROM21_GPIO_DXE_H_

#define PT_DXE_PRIVATE_DATA_SIGNATURE   SIGNATURE_16 ('P', 'T')

// Promontory GPIO pin 0 to 7
#ifndef Promontory_GPIO_ITEM_SIZE
  #define Promontory_GPIO_ITEM_SIZE                      0x8
#endif

/// Defintion of Promontory GPIO common setting
typedef union {
  UINT16                          Raw;                         ///< GPIO setting value
  struct {
    UINT16                        GpioDeBounceTimer:3;         ///< GPIO sampling rate of the de-bounce timer
    UINT16                        GpioDeBounceTimeoutTh:3;     ///< GPIO De-bounce timer timeout threshold
    UINT16                        GpioInterruptOutputEn:1;     ///< GPIO interrupt signal output enable
    UINT16                        GpioInterruptActLevel:1;     ///<GPIO interrupt signal active level
    UINT16                        GPIOInterruptMode:1;         ///<GPIO interrupt mode
    UINT16                        Reserved2:7;                 ///< Reserved
  }                               Common;                      ///< Bit mapping for Common setting
} Promontory_GPIO_COMMON_SETTING;

/// Defintion of Promontory GPIO pin 0 to 7 setting
typedef union {
  UINT16                          Raw;                        ///< GPIO setting value
  struct {                                                    ///<
    UINT16                        OutEnB:1;                   ///< Output enable
    UINT16                        Out:1;                      ///< Output control if Output enable
    UINT16                        interruptEnable:1;          ///< GPIO pin interrupt enable
    UINT16                        interruptLevelTrigType:1;   ///< GPIO level trigger type
    UINT16                        interruptType:2;            ///< GPIO pin interrupt type
    UINT16                        interruptMask:1;            ///< GPIO pin interrupt mask
    UINT16                        Reserved2:9;                ///< Reserved
  }                               Gpio;                       ///< Bit mapping for GPIO setting
} Promontory_GPIO_SETTING;

/// Promontory GPIO setting item
typedef struct {
  UINT16                          Pin;                        ///< GPIO pin number
  Promontory_GPIO_SETTING     Setting;                    ///< GPIO setting
} Promontory_GPIO_ITEM;

/// Promontory GPIO init table
typedef struct {
  Promontory_GPIO_COMMON_SETTING     GpioCommon;                          ///< GPIO common setting
  Promontory_GPIO_ITEM               GpioList[Promontory_GPIO_ITEM_SIZE]; ///< GPIO setting list
} Promontory_GPIO_INIT_TABLE;

//
// Functions Prototypes
//
VOID
PTGpioInitial (
  IN       VOID        *PTGpioPtr
  );

EFI_STATUS
EFIAPI
PTGpioWriteDxe (
  IN       UINT8        Pin,
  IN       UINT8        Value
  );

EFI_STATUS
EFIAPI
PTGpioReadDxe (
  IN       UINT8        Pin,
  OUT      UINT8        *Value
  );

EFI_STATUS
EFIAPI
PTGpioInitialDxe (
  IN       VOID        *PTGpioPtr
  );

typedef
EFI_STATUS
(EFIAPI *PTInitialGpioDxe)(
  IN       VOID        *PTGpioPtr
  );

// Write GPIO function
typedef
EFI_STATUS
(EFIAPI *PTWriteGpioDxe) (
  IN       UINT8         Pin,
  IN       UINT8         Value
  );

// Read GPIO function
typedef
EFI_STATUS
(EFIAPI *PTReadGpioDxe) (
  IN       UINT8          Pin,
  OUT      UINT8          *Value
  );

// Module data structure
typedef struct _EFI_PT_GPIO_PROTOCOL  {
  PTInitialGpioDxe  PTInitiialGpio;
  PTWriteGpioDxe    PTWriteGpio;
  PTReadGpioDxe     PTReadGpio;
} EFI_PT_GPIO_PROTOCOL;

// Module data structure
typedef struct _PT_DXE_PRIVATE {
  UINT16                 Signature;          ///< Signature
  EFI_PT_GPIO_PROTOCOL   PtGpioProtocol;          ///< PT INIT PROTOCOL
} PT_DXE_PRIVATE;

// DXE_DRIVER ENTRY_POINT
EFI_STATUS
EFIAPI
FchProm21GpioDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );

#endif

