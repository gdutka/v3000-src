/** @file
  Type definitions for Capsule Processor Dxe module

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _EC_CAPSULE_DXE_H_
#define _EC_CAPSULE_DXE_H_

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/VariableLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>
#include <Library/ChipsetCapsuleLib.h>
#include <Library/ChipsetSignatureLib.h>
#include <Library/OemGraphicsLib.h>
#include <Library/BvdtLib.h>
#include <Library/HobLib.h>
#include <Library/BdsCpLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/FirmwareManagement.h>
#include <Protocol/SimpleTextOut.h>
#include <Guid/EfiSystemResourceTable.h>
#include <Guid/FmpCapsule.h>
#include <Guid/ImageAuthentication.h>
#include <Guid/H2OBdsCheckPoint.h>
#include <SecureFlash.h>
#include <Guid/EventGroup.h>
#include <Library/BaseOemSvcChipsetLib.h>

#define ESRT_LAST_ATTEMPT_VERSION       L"EsrtLastAttemptVersion"
#define ESRT_LAST_ATTEMPT_STATUS        L"EsrtLastAttemptStatus"
#define CAPSULE_RESULT_VARIABLE         L"Capsule0000"
#define CAPSULE_RESULT_MAX_VARIABLE     L"CapsuleMax"
#define CAPSULE_RESULT_LAST_VARIABLE    L"CapsuleLast"
#define EFI_CAPSULE_REPORT_GUID         { 0x39b68c46, 0xf7fb, 0x441b, { 0xb6, 0xec, 0x16, 0xb0, 0xf6, 0x98, 0x21, 0xf3 }}

#define CAPSULE_FMP_SIGNATURE                   SIGNATURE_32('C','F','M','P')
#define CAPSULE_FMP_INSTANCE_FROM_THIS(a)       CR (a, CAPSULE_FMP_INSTANCE, Fmp, CAPSULE_FMP_SIGNATURE)

typedef struct {
  EFI_SIGNATURE_LIST    SignatureListHeader;
  EFI_SIGNATURE_DATA    SignatureData;
} CERTIFICATE_DATA;

typedef struct {
  UINT32                VariableTotalSize;
  UINT32                Reserved;       //for alignment
  EFI_GUID              CapsuleGuid;
  EFI_TIME              CapsuleProcessed;
  EFI_STATUS            CapsuleStatus;
} EFI_CAPSULE_RESULT_VARIABLE_HEADER;

#endif