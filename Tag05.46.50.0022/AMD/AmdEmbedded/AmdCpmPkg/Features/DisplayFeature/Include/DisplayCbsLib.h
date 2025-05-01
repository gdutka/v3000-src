/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_DISPLAY_CBS_LIB_H_
#define _AMD_DISPLAY_CBS_LIB_H_


/*----------------------------------------------------------------------------------------*/
/**
 * Identify Bristal APU Revision
 *
 * @retval        TRUE           Bristal APU
 * @retval        FALSE          Not Bristal APU
 */

BOOLEAN
DisplayFamilyIdentify (
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Update Cbs Uma Version option
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
UpdateAmdCbsUmaVersion (
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Update Cbs Uma Size ID
 * @param[in]     UmaSizeId      UMA Size ID
 *                               0: Default
 *                               1: Gaming
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
UpdateAmdCbsUmaSizeId (
  IN UINT8 UmaSizeId
  );

#endif
