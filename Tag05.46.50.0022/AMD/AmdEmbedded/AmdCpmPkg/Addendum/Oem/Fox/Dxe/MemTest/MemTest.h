/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#pragma once

#include <PiDxe.h>
#include <Protocol/MpService.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#define MEMTEST_BLOCK_SIZE 0x200000
#define MEMTEST_LOOP_COUNT 1000

#define TEST_THREAD_2 1
#define ADJUST_PSM 1
#define SHOW_ERROR_MESSAGE_ON_SCREEN  0


extern EFI_MP_SERVICES_PROTOCOL  *gMpServices;

VOID
MemTestSetup (VOID);

VOID
MemTestCleanup (VOID);

// Run Memory test and return Error count.
UINTN
MemTestRun (VOID);
