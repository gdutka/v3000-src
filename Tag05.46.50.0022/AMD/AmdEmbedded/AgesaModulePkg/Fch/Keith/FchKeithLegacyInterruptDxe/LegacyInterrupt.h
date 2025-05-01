/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_LEGACY_INTERRUPT_H_
#define _AMD_LEGACY_INTERRUPT_H_

#define FCH_IOMAP_REGC00    0xC00
#define FCH_IOMAP_REGC01    0xC01

#define FCH_PCI_BUS         0
#define FCH_PCI_DEV         20
#define FCH_PCI_FUNC        0

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FchDxeLib.h>

// Consumed Protocols
#include <Protocol/PciRootBridgeIo.h>

// Produced Protocols
#include <Protocol/LegacyInterrupt.h>

//
// Module specific definitions
//
#define     MAX_NUMBER_PIRQS     8

#endif // _AMD_LEGACY_INTERRUPT_H_

