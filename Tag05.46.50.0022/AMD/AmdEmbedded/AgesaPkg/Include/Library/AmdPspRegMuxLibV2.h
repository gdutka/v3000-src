/*****************************************************************************
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdPspRegMuxLibV2.h
 *
 * @brief Lib for Psp Mutex
 *
 * @details In Psp to X86 SMI interface, PSP will access some FCH SMI register to
 *          check if X86 is ready to receive a new SMI or PSP SMI is enabled and etc.
 *          Use Psp Mutex lib to avoid racing condition between X86 and PSP on
 *          accessing the same SMI register
 *
 */
#ifndef _AMD_PSP_REG_MUX_LIB_V2_H_
#define _AMD_PSP_REG_MUX_LIB_V2_H_

#include "AMD.h"
#include <Guid/EventGroup.h>
/**
 * @brief Accuquire the ownship of some FCH SMI register
 *
 * @details In Psp to X86 SMI interface, PSP will access some FCH SMI register to
 *          check if X86 is ready to receive a new SMI or PSP SMI is enabled and etc.
 *          To avoid racing conditon, call this lib to accquire the mutex before access these SMI register
 *          Note, acquire and release should in pair
 *
 * @return VOID
 */
VOID
AcquirePspAccRegMutex (
  );

/**
 * @brief Release the ownship of some FCH SMI register
 *
 * @details In Psp to X86 SMI interface, PSP will access some FCH SMI register to
 *          check if X86 is ready to receive a new SMI or PSP SMI is enabled and etc.
 *          call this lib to release the mutex after complete accessing these SMI register
 *          Note, acquire and release should in pair
 *
 * @return VOID
 */
VOID
ReleasePspAccRegMutex (
  );

#endif


