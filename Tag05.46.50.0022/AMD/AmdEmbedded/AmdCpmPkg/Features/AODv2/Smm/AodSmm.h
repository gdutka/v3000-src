/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

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
#ifndef __AOD_SMM_H__
#define __AOD_SMM_H__
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmBase2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Protocol/FchSmmSwDispatch2.h>
#include <Protocol/AmdCpmVariableHookProtocol.h>
#include <Library/SmmMemLib.h>
#include <Features/AODv2/Include/Protocol/AodCmdSmmProtocol.h>
#include <Features/AODv2/Include/AodSetup.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include <Features/AODv2/Include/Aod.h>
///
/// "DATABASE" RECORD
/// Linked list data structures
///
#define CMD_RECORD_SIGNATURE SIGNATURE_32 ('C', 'M', 'D', 'R')

typedef struct _CMD_RECORD {
  UINT32                        Signature;
  LIST_ENTRY                    Link;
  BOOLEAN                       Processed;
  UINT8                         CallbackPhase;
  UINT32                        CmdId;

  ///
  /// Callback function
  ///
  AOD_CMD_SMM_CALLBACK          Callback;

} CMD_RECORD;

#define CMD_RECORD_FROM_LINK(_record)  CR (_record, CMD_RECORD, Link, CMD_RECORD_SIGNATURE)

///
/// Create private data for the protocols that we'll publish
///
typedef struct {
  LIST_ENTRY                        CallbackDataBase;
  EFI_HANDLE                        SmiHandle;
  VOID                              *Nvs;
  AOD_CMD_SMM_DISPATCH_PROTOCOL     CmdSmmProtocol;
} PRIVATE_DATA;

EFI_STATUS
EFIAPI
AodCmdSmmRegister (
  IN  CONST  AOD_CMD_SMM_DISPATCH_PROTOCOL      *This,
  IN    AOD_CMD_SMM_REGISTER_CONTEXT            *Context,
  IN    AOD_CMD_SMM_CALLBACK                    Callback,
  OUT   EFI_HANDLE                              *Handle
  );

/**
  Unregister a cmdid dispatch function

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
AodCmdSmmUnRegister (
  IN  CONST  AOD_CMD_SMM_DISPATCH_PROTOCOL            *This,
  IN    EFI_HANDLE                               *Handle
  );

/**
  hook to sync L"AodSetup" into APCB/CMOS

  @param[in] Context             L"AodSetup" POINTER

  @retval EFI_SUCCESS            The function has been successfully called.
**/
EFI_STATUS
EFIAPI
AodVariableHook (
  IN  VOID            *Context
  );
#endif
