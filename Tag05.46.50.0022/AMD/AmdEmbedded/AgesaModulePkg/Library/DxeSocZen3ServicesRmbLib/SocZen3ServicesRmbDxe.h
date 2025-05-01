/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _SOC_ZEN3_SERVICES_RMB_DXE_H_
#define _SOC_ZEN3_SERVICES_RMB_DXE_H_

EFI_STATUS
SocZen3RmbGetOpnFmax (
  IN       AMD_SOC_ZEN3_SERVICES_PROTOCOL  *This,
     OUT   UINT32                          *FMax
  );

EFI_STATUS
SocZen3RmbGetOpnCorePresence (
  IN       AMD_SOC_ZEN3_SERVICES_PROTOCOL  *This,
     OUT   UINT32                          *CcdMap,
     OUT   UINT32                          *CoreDisMap,
     OUT   BOOLEAN                         *SmtEnable
  );

EFI_STATUS
SocZen3RmbGetOcSmuMsgId (
  IN       AMD_SOC_ZEN3_SERVICES_PROTOCOL   *This,
     OUT   SOC_ZEN3_OC_MSG_ID_TABLE        **OcMsgIdTable
  );

#endif
