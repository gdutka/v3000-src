/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#ifndef _OEM_APCB_V3_SMM_SERVICE_H_
#define _OEM_APCB_V3_SMM_SERVICE_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmMemLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdApcbProtocol.h>

extern EFI_GUID gOemApcbV3SmmServiceCommHandleGuid;

#endif //_OEM_APCB_V3_SMM_SERVICE_H_


