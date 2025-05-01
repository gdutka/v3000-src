/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _OEM_APCB_V3_SMM_SERVICE_COMMUNICATION_H_
#define _OEM_APCB_V3_SMM_SERVICE_COMMUNICATION_H_


#define APCB_SMM_COMM_NO_PARAMETER_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMMUNICATION_CMN))

/// APCB SMM communication common buffer
typedef struct _APCB_SMM_COMMUNICATION_CMN {
  EFI_STATUS                   Status;     ///< Return Status
} APCB_SMM_COMMUNICATION_CMN;

#define APCB_SMM_COMM_ACCESS_VALUE_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_UPDATE_VALUE))

/// APCB SMM communication update value buffer
typedef struct _APCB_SMM_COMM_UPDATE_VALUE {
  EFI_STATUS                   Status;     ///< Return Status
  UINT32                       Value;      ///< Get/Set Value
} APCB_SMM_COMM_UPDATE_VALUE;

#endif // _OEM_APCB_V3_SMM_SERVICE_COMMUNICATION_H_

