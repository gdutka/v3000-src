/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

#ifndef _AOD_SYNC_APCB_H_
#define _AOD_SYNC_APCB_H_
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <AmdSoc.h>
#include <AMD.h>
#include <RPL/ApcbV3TokenUid.h>
#include <RPL/ApcbV3Priority.h>
#include <Guid/SocCoreInfo.h>
#include <Features/AODv2/Include/Aod.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include "AodSocLib.h"

/**
  This function used to update APCB token by AodVariable

  @param[in]  AodVariable                 Points to the L"AodSetup" Variable
  @param[in]  vApcbProtocol               Points to the gAodCmdProtocol protocol

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
EFI_STATUS
AodUpdateApcbTokensRpl (
  VOID *AodVariable,
  VOID *vApcbProtocol
  );
#endif