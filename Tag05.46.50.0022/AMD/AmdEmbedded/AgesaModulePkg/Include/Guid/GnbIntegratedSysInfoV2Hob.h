/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Gnb Frame Buffer Data V2 Hob GUID definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Gnb
 * @e \$Revision: 309090 $   @e \$Date: 2019-06-12 14:05:05 +0800 (Wed, 12 June 2019) $
 */
#ifndef _GNB_INTEGRATED_SYSINFO_V2_HOB_H_
#define _GNB_INTEGRATED_SYSINFO_V2_HOB_H_

#define _GNB_INTEGRATED_SYSINFO_V2_HOB_GUID \
{ \
  0x6013bf85, 0xe185, 0x4b68, 0xbb, 0x6c, 0xcd, 0xbe, 0xed, 0x47, 0x83, 0x0f \
}
extern EFI_GUID gGnbIntegratedSysInfoHobV2Guid;

#include <NbioGfxV2.h>

#pragma pack (push, 1)

/// Memory descriptor structure for each memory rang
typedef struct {
  UINT64  Base;                             ///< Base address of memory rang
  UINT64  Size;                             ///< Size of memory rang
} NBIO_UMA_DESCRIPTOR;

/// Frame Buffer Data HOB data
typedef struct _NBIO_Frame_Buffer_DATA_V2_HOB {
  EFI_HOB_GUID_TYPE              EfiHobGuidType;                             ///< GUID Hob type structure
  ATOM_FUSION_SYSTEM_INFO_V4     AtomFusionSystemInfoV4;                      ///< Platform Config Structure
  NBIO_UMA_DESCRIPTOR            UMAInfo;
} NBIO_Frame_Buffer_DATA_V2_HOB;

#pragma pack (pop)

#endif


