/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_CPM_PT_GPIO_INIT_FINISHED_PPI_H_
#define _AMD_CPM_PT_GPIO_INIT_FINISHED_PPI_H_

#include <AmdCpmBase.h>

//
// GUID definition
//
#define AMD_CPM_PT_GPIO_INIT_FINISHED_PPI_GUID \
  { 0x1248c135, 0x1f84, 0x433e, 0xba, 0xb3, 0xc9, 0x6f, 0xe5, 0x2f, 0x9a, 0x9e }

extern EFI_GUID gAmdCpmPtGpioInitFinishedPpiGuid;

/// AMD CPM PT GPIO Init Finished PPI Definition

typedef struct _AMD_CPM_PT_GPIO_INIT_FINISHED_PPI {
  UINTN                                   Revision;               ///< Revision Number
} AMD_CPM_PT_GPIO_INIT_FINISHED_PPI;

// Current PPI Revision
#define AMD_CPM_PT_GPIO_INIT_FINISHED_PPI_REV  0x00

#endif
