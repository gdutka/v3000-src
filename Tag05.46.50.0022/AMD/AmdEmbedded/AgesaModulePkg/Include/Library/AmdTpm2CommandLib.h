/*
 ******************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_PSP_TPM2_COMMAND_LIB_H_
#define _AMD_PSP_TPM2_COMMAND_LIB_H_

#include "Porting.h"
#include <IndustryStandard/Tpm20.h>
#include <Protocol/Tcg2Protocol.h>

#pragma pack(1)
typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_CLEAR          AuthHandle;
  UINT32                 AuthorizationSize;
  TPMS_AUTH_COMMAND      AuthSession;
} TPM2_CLEAR_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER       Header;
  UINT32                     ParameterSize;
  TPMS_AUTH_RESPONSE         AuthSession;
} TPM2_CLEAR_RESPONSE;
#pragma pack()

/**
 * @brief Remove all TPM context associated with TPM_RH_PLATFORM
 *
 */
EFI_STATUS
EFIAPI
Tpm2Clear (
  IN EFI_TCG2_PROTOCOL           *Tcg2Protocol
  );
#endif // _AMD_PSP_TPM2_COMMAND_LIB_H_


