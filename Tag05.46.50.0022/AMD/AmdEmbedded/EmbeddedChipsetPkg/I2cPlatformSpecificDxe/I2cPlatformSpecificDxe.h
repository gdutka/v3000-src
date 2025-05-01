/** @file
  For I2C I2cPlatform Specific Dxe Driver

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
#ifndef _I2C_PLATFORM_SPECIFIC_DXE_H_
#define _I2C_PLATFORM_SPECIFIC_DXE_H_

#include <IndustryStandard/Pci.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/I2cPlatformSpecific.h>
#include "../I2cMaster/Dxe/I2cMasterDxe.h"

//////
BOOLEAN
AmdGpioEventArrived(
  IN  I2C_BUS_INSTANCE   *I2cBusInstance,
  IN  UINT8              IntGpioController,
  IN  UINT8              PadNumber,
  IN  UINT8              ActiveLevel
  );

VOID
AmdI2cwitchDevMode (
  IN I2C_BUS_INSTANCE      *I2cBusInstance,
  IN UINT32                DeviceMode
  );

UINTN
AmdI2cGetDevMode (
  IN I2C_BUS_INSTANCE      *I2cBusInstance
  );

//
// How to get value without calculate HCNT / LCNT:
// 1. Boot to OS and install I2C driver if controller avaliable.
// 2. Find I2C MMIO base.
// 3. Each chipset platform may has certain IC_SDA_HOLD Register offset,
//    look it up from spec and set it to SHTL
// 4. The same series CPU will execute the same I2C setting of mI2cBusInfo[]
//    (ex. 0x000906EA or 0x000906EC will run 0x000906E0 setting)
// 5. If you want to modify I2C_SCL and I2C_SDA value, you can set OemHookI2cSclSdaEnable
//    as TRUE to run OemHookI2cSclSda() hook function or modify mI2cBusInfo[] directly.
//     IC_SS_SCL_HCNT - I2C_MMIO[0x14]
//     IC_SS_SCL_LCNT - I2C_MMIO[0x18]
//     IC_SDA_HOLD - I2C_MMIO[SHTL]
//
//     IC_FS_SCL_HCNT - I2C_MMIO[0x1C]
//     IC_FS_SCL_LCNT - I2C_MMIO[0x20]
//     IC_SDA_HOLD - I2C_MMIO[SHTL]
//
//     IC_HS_SCL_HCNT - I2C_MMIO[0x24]
//     IC_HS_SCL_LCNT - I2C_MMIO[0x28]
//     IC_SDA_HOLD - I2C_MMIO[SHTL]
//
I2C_INFO mI2cBusInfo[] = {
//  cpuid        CRG      SHR      QkR                                                                        Init         GetDeviceMode        SwitcDevhMode          GpioArrived
  { 0x00660F00,  0x0000,  0x00, { {0x023C, 0x02F7, 0x38}, {0x0033, 0x0053, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},
  { 0x00670F00,  0x0000,  0x00, { {0x023C, 0x02F7, 0x38}, {0x0033, 0x0053, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},
  { 0x00660F51,  0x0000,  0x00, { {0x023C, 0x02F7, 0x38}, {0x0033, 0x0053, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //BR
  { 0x00800F00,  0x0000,  0x00, { {0x023C, 0x02F7, 0x38}, {0x0033, 0x0053, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //SM
  { 0x00810F00,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //RV for Ryzen 5
  { 0x00810F10,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //RV for Ryzen 7
  { 0x00810F80,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Picasso for Ryzen 7
  { 0x00810F81,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Picasso for Ryzen 5
  { 0x00870F00,  0x0000,  0x00, { {0x023C, 0x02F7, 0x38}, {0x0033, 0x0053, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Mastisse A0
  { 0x00870F10,  0x0000,  0x00, { {0x023C, 0x02F7, 0x38}, {0x0033, 0x0053, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Mastisse B0
  { 0x00860F00,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Renoir
  { 0x00A50F00,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Cezanne
  { 0x00860F80,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Lucienne
  { 0x00A40F00,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Rembrandt
  { 0x00A40F40,  0x0000,  0x7C, { {0x0285, 0x0357, 0x48}, {0x0087, 0x00F0, 0x30}, {0x0006, 0x0010, 0x09}, } , NULL,        AmdI2cGetDevMode,    AmdI2cwitchDevMode,    AmdGpioEventArrived},  //Rembrandt B0
  { 0xFFFFFFFF,  0x0,     0x0,  { {0x0, 0x0, 0x0},        {0x0, 0x0, 0x0},        {0x0, 0x0, 0x0},        } , NULL,        NULL,                NULL,                  NULL}
};
#endif