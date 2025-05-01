/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#ifndef _AMD_PSP_FW_IMAGE_HEADER_LIB_H_
#define _AMD_PSP_FW_IMAGE_HEADER_LIB_H_


/**
 * @brief Check Apcb Data has PSP Fw Image Header or not
 *
 * @param[in]       UINT64  ApcbEntryAddress
 * @param[in,out]  UINT32* PSP Fw Image Header Length
 *
 * @return  TRUE    Apcb Data has PSP Fw Image Header
 * @return  FALSE   Apcb Data did not have PSP Fw Image Header
 */
BOOLEAN
ApcbFwImageHeaderCheck (
  UINT64  ApcbEntryAddress,
  UINT32  *PspFwImageHeaderLen
);

#endif //_AMD_PSP_FW_IMAGE_HEADER_LIB_H_
