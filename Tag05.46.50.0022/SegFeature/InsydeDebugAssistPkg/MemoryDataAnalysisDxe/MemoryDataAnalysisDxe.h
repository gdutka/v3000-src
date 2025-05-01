/** @file
  The header file of MemoryDataAnalysisDxe driver.

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __MEMORY_DATA_ANALYSIS_DXE_H__
#define __MEMORY_DATA_ANALYSIS_DXE_H__

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/MemoryDataAnalysis.h>

#define SERIAL_OUTPUT_BUFFER_SIZE    200

typedef struct {
  EFI_PHYSICAL_ADDRESS  ImageBase;
  UINT64                ImageSize;
  CHAR8                 *ImageName;
} IMAGE_INFORMATION;

typedef struct {
  UINTN              DataCount;             
  IMAGE_INFORMATION  *ImageInfo;
} LOADED_IMAGES_INFORMATION;

EFI_STATUS
EFIAPI
MemoryDataAnalysis (
  IN  EFI_PHYSICAL_ADDRESS  StartAddress,
  IN  UINTN                 Length
  );

#endif

