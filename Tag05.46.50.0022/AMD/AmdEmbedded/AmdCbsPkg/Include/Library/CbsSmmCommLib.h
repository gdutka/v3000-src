/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/


#ifndef _CBS_SMM_COMM_LIB_H
#define _CBS_SMM_COMM_LIB_H

/**
 * @brief Call SMM communication with gCbsSmmCommHandleGuid
 *        Smm communication handler will call UpdateCbsApcbTokens which used to update all CBS APCB items
 *
 * @param IfrData
 * @return EFI_STATUS
 */
EFI_STATUS
CbsSmmCommUpdateApcb (
  IN UINTN   IfrDataSize,
  IN UINT8   *IfrData
);

#endif // _CBS_SMM_COMM_LIB_H

