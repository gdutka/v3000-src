/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Gnb Frame Buffer Data V4 Hob GUID definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Gnb
 * @e \$Revision: 309090 $   @e \$Date: 2020-09-21 14:05:05 +0800 (Mon, 21 Sep 2019) $
 */
#ifndef _GNB_INTEGRATED_SYSINFO_V4_HOB_H_
#define _GNB_INTEGRATED_SYSINFO_V4_HOB_H_

#define _GNB_INTEGRATED_SYSINFO_V4_HOB_GUID \
{ \
  0x89d85219, 0x2ca1, 0x42de, 0x99, 0x32, 0x74, 0x88, 0xdf, 0x4f, 0x5f, 0x83 \
}
extern EFI_GUID gGnbIntegratedSysInfoHobV4Guid;

#include <NbioGfxV4.h>

#pragma pack (push, 1)

/// Memory descriptor structure for each memory rang
typedef struct {
  UINT64  Base;                             ///< Base address of memory rang
  UINT64  Size;                             ///< Size of memory rang
} NBIO_UMA_DESCRIPTOR;

/// Frame Buffer Data HOB data
typedef struct _NBIO_Frame_Buffer_DATA_V4_HOB {
  EFI_HOB_GUID_TYPE              EfiHobGuidType;                             ///< GUID Hob type structure
  ATOM_FUSION_SYSTEM_INFO_V6     AtomFusionSystemInfoV6;                      ///< Platform Config Structure
  NBIO_UMA_DESCRIPTOR            UMAInfo;
} NBIO_Frame_Buffer_DATA_V4_HOB;

#pragma pack (pop)

#endif


