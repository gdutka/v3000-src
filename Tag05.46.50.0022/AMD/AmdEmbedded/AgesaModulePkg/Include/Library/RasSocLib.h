/*
*****************************************************************************
*
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _RAS_SOC_LIB_H_
#define _RAS_SOC_LIB_H_

#include <PiDxe.h>
#include <Protocol/MpService.h>
#include "AmdRas.h"

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#pragma pack (push, 1)

typedef struct {
  UINTN                 *ProcessorNumber;
  UINTN                 *McaBankNumber;
  RAS_THRESHOLD_CONFIG  *RasThresholdConfig;
  BOOLEAN               OvrflwChk;
} SET_MP_MCA_THRESHOLD_FNC_ARG;

typedef struct {
  EFI_MP_SERVICES_PROTOCOL      *MpServices;
  VOID                          *AmdRasPolicy;
} MP_FUNCTION_PARAM;

#pragma pack (pop)

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
RasSmnRead (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

VOID
RasSmnWrite (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

VOID
RasSmnRW (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  );

VOID
RasSmnRead8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8
  );

VOID
RasSmnWrite8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8
  );

VOID
RasSmnRW8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               AndMask,
  IN       UINT8               OrMask
  );

VOID
EFIAPI
CollectMcaErrorInfo (
  IN OUT   RAS_MCA_ERROR_INFO_V2* RasMcaErrorInfo
  );

EFI_STATUS
EFIAPI
SetCoreMcaThreshold (
  IN       SET_MP_MCA_THRESHOLD_FNC_ARG *pProcArg
  );

VOID
EFIAPI
McaStatusClr (
   IN VOID  *Buffer
 );

EFI_STATUS
UpdateFruTextToUmc (
  IN       UINT32              IohcBus,
  IN       AMD_FRUTEXT_ENTRY*  AmdFruTextSEntry
 );

VOID
EFIAPI
ProgramCoreMcaIpIdInstanceId (
  IN MP_FUNCTION_PARAM      *MpFuncParam
  );

EFI_STATUS
GetPhysicalCpuInfo (
  IN       VOID        *AmdRasPolicy,
  IN OUT   RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo
  );

UINT32
EFIAPI
GetUmcHarvestFuse (
  IN       UINTN     BusNumberBase
  );

UINT64
get_pcie_ccix_address (
  IN UINT64 address,
  IN BOOLEAN ccix_interleave_en,
  IN INT32 ccix_sdp_port_num_insert_sel,
  IN INT32 ccix_port_number,
  IN INT32 ccix_interleaved_addr_sel,
  IN INT32 ccix_interleaved_addr_remove0,
  IN INT32 ccix_interleaved_addr_remove1,
  IN INT32 ccix_if
  );

#endif //_RAS_SOC_LIB_H_


