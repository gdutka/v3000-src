/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Gnb PEI Gop Hob GUID definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Gnb
 * @e \$Revision: 309090 $   @e \$Date: 2019-11-01 14:05:05 +0800 (Wed, 1 Nov 2019) $
 */
#ifndef _GNB_PEI_GOP_HOB_H_
#define _GNB_PEI_GOP_HOB_H_

#define _GNB_PEI_GOP_HOB_GUID \
{ \
  0xa90f3dfd, 0xdb05, 0x4e73, 0x86, 0x75, 0xd, 0xd1, 0xf, 0x66, 0x9e, 0xf \
}
extern EFI_GUID gGnbPeiGopHobGuid;

#pragma pack (push, 1)

/// PEI GOP HOB data
typedef struct _AMD_PEI_GOP_POLICY_HEADER {
  UINT16 TableSize;
  UINT8  FormatRevision;   //mainly used for a hw function, when the parser is not backward compatible
  UINT8  ContentRevision;  //change it when a data table has a structure change, or a hw function has a input/output parameter change
} AMD_PEI_GOP_POLICY_HEADER;

struct _AMD_PEI_GOP_POLICY {
  AMD_PEI_GOP_POLICY_HEADER   Header;
  UINT32                      GfxBar0;                // BAR0: PciReg 0x10
  UINT32                      GfxBar1;                // BAR1: PciReg 0x14
  UINT32                      GfxBar2;                // BAR2: PciReg 0x18
  UINT32                      GfxBar3;                // BAR3: PciReg 0x1C
  UINT32                      GfxBar4;                // BAR4: PciReg 0x20
  UINT32                      GfxBar5;                // BAR5: PciReg 0x24
  VOID                        *VbiosImage;            // VBIOS image ptr
  BOOLEAN                     LidStatus;              // 1:Lid Close 0:Lid Open
  UINT32                      HorizontalResolution;   // Requested horizontal resolution
  UINT32                      VerticalResolution;     // Requested vertical resolution
};

typedef struct _AMD_PEI_GOP_CONFIG {
  AMD_PEI_GOP_POLICY_HEADER   Header;
  UINT32                      ConfigMemsizeValue;
  UINT32                      FbTopValue;
  UINT32                      FbBaseValue;
  UINT8                       MMRegBarIndex;
  UINT32                      FrameBuffOffset;
}AMD_PEI_GOP_CONFIG;

#pragma pack (pop)

#endif


