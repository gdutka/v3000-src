/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_SOC_FP7_RMB_DXE_H_
#define _AMD_SOC_FP7_RMB_DXE_H_

extern EFI_GUID gAmdErrorLogDepexProtocolGuid;
extern EFI_GUID gAmdFchKeithDepexProtocolGuid;
extern EFI_GUID gAmdFchKTSmbusDepexProtocolGuid;
extern EFI_GUID gAmdFchKTSmmDispacherDepexProtocolGuid;
extern EFI_GUID gAmdFchKTSmmControlDepexProtocolGuid;
extern EFI_GUID gAmdPspDxeV2RmbDepexProtocolGuid;
extern EFI_GUID gAmdPspP2CmboxV2DepexProtocolGuid;
extern EFI_GUID gAmdPspSmmV2DepexProtocolGuid;
extern EFI_GUID gAmdPspHstiV2DepexProtocolGuid;
extern EFI_GUID gAmdMemFp7DxeDepexProtocolGuid;
extern EFI_GUID gAmdMemAmdDxeDepexProtocolGuid;
extern EFI_GUID gAmdNbioBaseRMBDxeDepexProtocolGuid;
extern EFI_GUID gAmdNbioSmuV13DxeDepexProtocol;
extern EFI_GUID gAmdNbioPcieRMBDxeDepexProtocolGuid;
extern EFI_GUID gAmdNbioIOMMURMBDxeDepexProtocolGuid;
extern EFI_GUID gAmdNbioALIBRMBDxeDepexProtocolGuid;
extern EFI_GUID gAmdCcxZen3RmbDepexProtocolGuid;
extern EFI_GUID gAmdCcxZen3DepexSmmProtocolGuid;
extern EFI_GUID gAmdFabricRmbDepexProtocolGuid;
extern EFI_GUID gAmdMemSmbiosV2DepexProtocolGuid;
extern EFI_GUID gAmdApcbServiceV3DepexProtocolGuid;
extern EFI_GUID gAmdMemRestoreDepexProtocolGuid;
extern EFI_GUID gApobRmbDxeDepexProtocolGuid;
extern EFI_GUID gAmdMpmDepexProtocolGuid;

//
// Silicon Driver installation list
//

EFI_GUID * SiliconDriverProtocolList[] = {
  &gAmdErrorLogDepexProtocolGuid,
  &gAmdFchKeithDepexProtocolGuid,
  &gAmdFchKTSmbusDepexProtocolGuid,
  &gAmdFchKTSmmDispacherDepexProtocolGuid,
  &gAmdFchKTSmmControlDepexProtocolGuid,
  &gAmdPspDxeV2RmbDepexProtocolGuid,
  &gAmdPspP2CmboxV2DepexProtocolGuid,
  &gAmdPspSmmV2DepexProtocolGuid,
  &gAmdPspHstiV2DepexProtocolGuid,
  &gAmdMemFp7DxeDepexProtocolGuid,
  &gAmdMemAmdDxeDepexProtocolGuid,
  &gAmdNbioBaseRMBDxeDepexProtocolGuid,
  &gAmdNbioSmuV13DxeDepexProtocol,
  &gAmdNbioPcieRMBDxeDepexProtocolGuid,
  &gAmdNbioIOMMURMBDxeDepexProtocolGuid,
  &gAmdNbioALIBRMBDxeDepexProtocolGuid,
  &gAmdCcxZen3RmbDepexProtocolGuid,
  &gAmdCcxZen3DepexSmmProtocolGuid,
  &gAmdFabricRmbDepexProtocolGuid,
  &gAmdMemSmbiosV2DepexProtocolGuid,
  &gAmdApcbServiceV3DepexProtocolGuid,
  &gAmdMemRestoreDepexProtocolGuid,
  &gApobRmbDxeDepexProtocolGuid,
  &gAmdMpmDepexProtocolGuid
};

UINT8 SiliconDriverProtocolListNumber = (UINT8) (sizeof (SiliconDriverProtocolList) / sizeof (SiliconDriverProtocolList[0]));

//
// Functions Prototypes
//
EFI_STATUS
EFIAPI
AmdSocFp6RnDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );


#endif // _AMD_SOC_FP7_RMB_DXE_H_


