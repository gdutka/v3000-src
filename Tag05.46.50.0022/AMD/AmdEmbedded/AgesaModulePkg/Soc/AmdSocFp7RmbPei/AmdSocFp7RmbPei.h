/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_SOC_FP7_RMB_PEI_H_
#define _AMD_SOC_FP7_RMB_PEI_H_

extern EFI_GUID gAmdErrorLogDepexPpiGuid;
extern EFI_GUID gAmdNbioRMBEarlyPhaseDepexPpiGuid;
extern EFI_GUID gAmdFchKeithDepexPpiGuid;
extern EFI_GUID gAmdFchKTSmbusDepexPpiGuid;
extern EFI_GUID gAmdPspPeiV2RmbDepexPpiGuid;
extern EFI_GUID gAmdMemFp7PeimDepexPpiGuid;
extern EFI_GUID gAmdMemPeimDepexPpiGuid;
extern EFI_GUID gAmdNbioBaseRMBPeiDepexPpiGuid;
extern EFI_GUID gAmdNbioSmuV13PeiDepexPpiGuid;
extern EFI_GUID gAmdNbioPcieRMBPeiDepexPpiGuid;
extern EFI_GUID gAmdNbioIOMMURMBPeiDepexPpiGuid;
extern EFI_GUID gAmdCcxZen3CznDepexPpiGuid;
extern EFI_GUID gAmdFabricRmbDepexPpiGuid;
extern EFI_GUID gAmdMemSmbiosV2Fp7PpiGuid;
extern EFI_GUID gAmdMemChanXLatDummyDepexPpiGuid;
extern EFI_GUID gAmdMemoryPeiHobFp7DepexPpiGuid;
extern EFI_GUID gAmdMemRestoreDepexPpiGuid;
extern EFI_GUID gApobRmbPeiDepexPpiGuid;
extern EFI_GUID gAmdMpmPeiDepexPpiGuid;

//
// Silicon Driver installation list
//

EFI_GUID * SiliconDriverPpiList[] = {
  &gAmdErrorLogDepexPpiGuid,
  &gAmdNbioRMBEarlyPhaseDepexPpiGuid,
  &gAmdFchKeithDepexPpiGuid,
  &gAmdFchKTSmbusDepexPpiGuid,
  &gAmdPspPeiV2RmbDepexPpiGuid,
  &gAmdMemFp7PeimDepexPpiGuid,
  &gAmdMemPeimDepexPpiGuid,
  &gAmdNbioBaseRMBPeiDepexPpiGuid,
  &gAmdNbioSmuV13PeiDepexPpiGuid,
  &gAmdNbioPcieRMBPeiDepexPpiGuid,
  &gAmdNbioIOMMURMBPeiDepexPpiGuid,
  &gAmdCcxZen3RmbDepexPpiGuid,
  &gAmdFabricRmbDepexPpiGuid,
  &gAmdMemSmbiosV2Fp7PpiGuid,
  &gAmdMemChanXLatDummyDepexPpiGuid,
  &gAmdMemoryPeiHobFp7DepexPpiGuid,
  &gAmdMemRestoreDepexPpiGuid,
  &gApobRmbPeiDepexPpiGuid,
  &gAmdMpmPeiDepexPpiGuid
};

UINT8 SiliconDriverPpiListNumber = (UINT8) (sizeof (SiliconDriverPpiList) / sizeof (SiliconDriverPpiList[0]));


#endif // _AMD_SOC_FP7_RMB_PEI_H_



