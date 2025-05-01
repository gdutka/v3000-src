/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2018, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/*++

@file:

  Platform.h

@brief:

  Header file for Platform Initialization Driver.

++*/

#ifndef _PLATFORM_DRIVER_H
#define _PLATFORM_DRIVER_H

#include <Uefi.h>
#include <ChipsetSetupConfig.h>
#include <UsbLegacy.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/CmosLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/S3BootScriptLib.h>

#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SetupUtility.h>

#include <ChipsetAccess.h>
//#include <Library/BaseInsydeChipsetLib.h>
#include <MeSetup.h>

typedef struct {
  UINT32              MrcS3RestoreSize;              // MRC_OUTPUT_S3_RESTORE_DATA
} SAVE_MEM_RESTORE_INFOSIZE;

//
// Data
// BUGBUG: These constants are defined in multiple places.
//
#define PLATFORM_NUM_SMBUS_RSVD_ADDRESSES 4
#define MAX_CHANNELS   2
//
// Maximum number of DIMM sockets supported by each channel
//
#define MAX_SLOTS      2
///
// Maximum number of DIMM sockets supported by the memory controller
//
#define MAX_SOCKETS   MAX_CHANNELS * MAX_SLOTS
//
// Define the SPD Defaul Address for DIMM 0
//
#define DIMM0_SPD_ADDRESS  0xA0
#define SMBUS_ADDR_CH_A_1 0xA0
#define SMBUS_ADDR_CH_A_2 0xA2
#define SMBUS_ADDR_CH_B_1 0xA4
#define SMBUS_ADDR_CH_B_2 0xA6

#define EFI_IA32_PLATFORM_ID            0x17
#define EFI_MSR_PSB_CLOCK_STATUS        0xCD
#define FAMILY_MODEL_6D                 0x6D0
#define FAMILY_MODEL_6E                 0x6E0
#define FAMILY_MODEL_6F                 0x6F0
#define FAMILY_MODEL_1067               0x10670

#define EFI_MSR_PLATFORM_INFO           0xCE
#define FAMILY_MODEL_MASK               0xFFFF0
#define EFI_MSR_PERF_STS                0x198

#define C4_EXIT_TIMING_FAST                         1
#define C4_EXIT_TIMING_SLOW                         2
#define C4_EXIT_TIMING_FORCE_SLOW                   3

#define XCMOS_INDEX_PORT           0x72
#define XCMOS_DATA_PORT            0x73

//
// Index to access array
//
#define PCH_USB_UHCI1 0
#define PCH_USB_UHCI2 1
#define PCH_USB_UHCI3 2
#define PCH_USB_UHCI4 3
#define PCH_USB_UHCI5 4
#define PCH_USB_UHCI6 5
#define PCH_USB_UHCI7 6

#define PCH_USB_EHCI1 0
#define PCH_USB_EHCI2 1


//
// Definition in ME BIOS Writer's Guide "DRAM INIT DONE RESPONSE Message"
//
typedef enum {
  ME_BIOS_ACTION_CONTINUE_POST = 0,    // BIOS should continue with normal POST
  ME_BIOS_ACTION_GLOBAL_RESET,         // BIOS should perform a global reset
  ME_BIOS_ACTION_NON_PCR,              // BIOS should perform a non power cycling reset
  ME_BIOS_ACTION_PCR                   // BIOS should perform a power cycling reset
} ME_BIOS_ACTION;

//
// Prototypes
//
EFI_STATUS
EFIAPI
DxePlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
EFIAPI
Ps2InitHardware (
  IN  EFI_HANDLE  Handle
  );

EFI_STATUS
EFIAPI
ProtectMemorySPD (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

EFI_STATUS
EFIAPI
ThermalReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );
EFI_STATUS
EFIAPI
DisableMePTT (
  VOID
  );
/**
  This function gets registered as a callback to perform InstallSmbiosFviOemType 
  after SmbiosProtocol was installed.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
InstallSmbiosFviOemTypeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );  
//
// Global externs
//
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *mPciRootBridgeIo;

#endif
