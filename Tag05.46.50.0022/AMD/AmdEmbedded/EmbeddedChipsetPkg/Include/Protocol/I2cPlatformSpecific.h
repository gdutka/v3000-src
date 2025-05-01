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

#ifndef _I2C_PLATFORM_SPECIFIC_H_
#define _I2C_PLATFORM_SPECIFIC_H_
#include <I2cBusInstance.h>

#define I2C_PLATFORM_SPECIFIC_VERSION  0x00000001

#define EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL_GUID \
  { \
    0xC4700DAB, 0x41B6, 0x4FDF,  0xBA, 0xF2, 0xD2, 0xFF, 0x36, 0xE3, 0xAA, 0xCC \
  }
//
// Forward reference for pure ANSI compatability
//
typedef struct _I2C_BUS_INSTANCE                    I2C_BUS_INSTANCE;  
typedef struct _EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL  EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL;

/**
  Get the amount of I2c bus(host) on platform.

  This routine must be called at or below TPL_NOTIFY.

  @param[out]   TotalControllerNumber      The max number of I2c bus.

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_ERROR         The function operation failed.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL_GET_MAX_I2C_CONTROLLER_NUMBER) (
  OUT UINT8       *TotalControllerNumber                              
 );


/**
  Get the I2C configuration data.

  @param[in]  This                  Pointer to an EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL structure.
  @param[in]  I2cBusInstance        Pointer to the I2C Bus instance structure.
  @param[in]  ControllerNum         The index of I2C controller.
  @param[out] I2cHcType             Indicates the I2C controller type.    

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_ERROR         The function operation failed.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL_GET_I2C_CONFIG_DATA) (
  IN  EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL   *This,   
  IN  I2C_BUS_INSTANCE                     *I2cBusInstance,
  IN  UINTN                                ControllerNum,
  OUT UINT8                                *I2cHcType                          
 );

//
// The protocol interface is used to describe how to control or get platform specific data for I2C devices, controllers or bus.
//
#pragma pack(1)
struct _EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL {
  UINT8                                                             Revision;
  EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL_GET_MAX_I2C_CONTROLLER_NUMBER  GetMaxI2cControllerNumber;
  EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL_GET_I2C_CONFIG_DATA            GetI2cConfigData;  
};
#pragma pack()

extern EFI_GUID gEfiI2cPlatformSpecificProtocolGuid;

#endif
