/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Gnb Frame Buffer Data V3 Hob GUID definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Gnb
 * @e \$Revision: 309090 $   @e \$Date: 2019-06-12 14:05:05 +0800 (Wed, 12 June 2019) $
 */
#ifndef _GNB_INTEGRATED_SYSINFO_V3_HOB_H_
#define _GNB_INTEGRATED_SYSINFO_V3_HOB_H_

#define _GNB_INTEGRATED_SYSINFO_V3_HOB_GUID \
{ \
  0x858cb1d4, 0x3d27, 0x4b7a, 0xb1, 0xdb, 0x97, 0xb6, 0xc3, 0xc1, 0x5b, 0x8c \
}
extern EFI_GUID gGnbIntegratedSysInfoHobV3Guid;

#include <NbioGfxV3.h>

#pragma pack (push, 1)

/// Memory descriptor structure for each memory rang
typedef struct {
  UINT64  Base;                             ///< Base address of memory rang
  UINT64  Size;                             ///< Size of memory rang
} NBIO_UMA_DESCRIPTOR;

/// Frame Buffer Data HOB data
typedef struct _NBIO_Frame_Buffer_DATA_V3_HOB {
  EFI_HOB_GUID_TYPE              EfiHobGuidType;                             ///< GUID Hob type structure
  ATOM_FUSION_SYSTEM_INFO_V5     AtomFusionSystemInfoV5;                      ///< Platform Config Structure
  NBIO_UMA_DESCRIPTOR            UMAInfo;
} NBIO_Frame_Buffer_DATA_V3_HOB;

#pragma pack (pop)

#endif


