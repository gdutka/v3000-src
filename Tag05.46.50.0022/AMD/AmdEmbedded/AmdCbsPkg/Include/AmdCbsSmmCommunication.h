/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CBS_SMM_COMMUNICATION_H_
#define _AMD_CBS_SMM_COMMUNICATION_H_

extern EFI_GUID gCbsSmmCommHandleGuid;

#define CBS_SMM_COMM_ID_UPDATE_APCB          0xCB     ///< ID for update CBS settings to APCB


/// CBS SMM communication common buffer
typedef struct _CBS_SMM_COMMUNICATION_CMN {
  UINT8                       id;         ///< ID of smm communication buffer
} CBS_SMM_COMMUNICATION_CMN;

#define CBS_SMM_COMM_UPDATE_APCB_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (CBS_SMM_COMM_UPDATE_APCB))

/// CBS SMM communication update APCB buffer
typedef struct _CBS_SMM_COMM_UPDATE_APCB {
  UINT8                       id;         ///< ID of smm communication buffer, used for dispatch
  UINT8                       CbsVariable[];    ///< CBS variable data
} CBS_SMM_COMM_UPDATE_APCB;


#endif // _AMD_CBS_SMM_COMMUNICATION_H_


