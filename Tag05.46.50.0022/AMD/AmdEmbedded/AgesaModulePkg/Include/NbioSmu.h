/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains Core Dldo PSM definitios.
 *
 *
 */

#ifndef NBIO_SMU_H_
#define NBIO_SMU_H_

/// Core Dldo PSM offset Array
typedef struct {
  UINT32            CoreIndicator; ///< [31:28] --> CCD, [27:20] --> CORE, 0xFFFFFFFF --> all Core, 0xFFFFFFFE --> Array Terminate flag
  UINT16            PsmOffset;     ///< PSM offset
} CORE_PSM_OFFSET;

#endif /* NBIO_SMU_H_ */

