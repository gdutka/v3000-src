/** @file
  PostCodeDxe driver header file

;******************************************************************************
;* Copyright (c) 2016 - 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _POST_CODE_DXE_H_
#define _POST_CODE_DXE_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/PostCodeConfigLib.h>
#include <Library/SerialPortPostCodeLib.h>
#include <Library/GraphicVgaLib.h>
#include <Library/VgaPostCodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PostCodeMiscLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DevicePathLib.h>
#include <Library/VariableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>

#include <Protocol/Cpu.h>
#include <Protocol/VgaSupport.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>

#include <Guid/ConsoleOutDevice.h>

#include <Pi/PiHob.h>

#define POSTCODE_MODE_VARIABLE_NAME    L"PostcodeMode"

#define CONSOLE_MODE 1
#define SERIAL_MODE 0


VOID
EFIAPI
PostCodeFeatureHandlerEntry (
  VOID
  );

VOID
EFIAPI
CheckIdtEntryCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

#endif 
