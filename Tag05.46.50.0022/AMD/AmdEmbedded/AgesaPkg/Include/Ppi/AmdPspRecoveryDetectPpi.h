/**
 * @file AmdPspRecoveryDetectPpi.h
 *
 * @brief   PspRecovery Detect PPI prototype definition
 *
 */
/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _AMD_PSP_RECOVERY_DETECTPPI_H_
#define _AMD_PSP_RECOVERY_DETECTPPI_H_

/**
 * @brief AMD_PSP_RECOVERY_DETECT_PPI prototype
 *
 * @details SBIOS need notify this PPI, if this PPI has been installed, denote some firmware is corrupted
 * SBIOS need to boot with recovery path, to recover the corrupted firmwares.
 */
typedef struct _AMD_PSP_RECOVERY_DETECT_PPI {
  UINT8       Version;     ///< PPI Version
} AMD_PSP_RECOVERY_DETECT_PPI;

extern EFI_GUID gAmdPspRecoveryDetectPpiGuid;

#endif


