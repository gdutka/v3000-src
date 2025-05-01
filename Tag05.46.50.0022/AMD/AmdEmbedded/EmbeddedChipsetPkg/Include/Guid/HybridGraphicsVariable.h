/** @file
  HybridGraphicsVariable.h

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _HYBRID_GRAPHICS_VARIABLE_GUID_H_
#define _HYBRID_GRAPHICS_VARIABLE_GUID_H_

#define HYBRID_GRAPHICS_VARIABLE_GUID  \
  { \
    0xb2b7c21f, 0x1786, 0x4a64, 0xbe, 0x69, 0x16, 0xce, 0xf7, 0x64, 0x73, 0x31 \
  }

extern EFI_GUID                               gH2OHybridGraphicsVariableGuid;

#pragma pack (1)
typedef struct {
  UINT8                                       OptimusFlag;
  UINT8                                       DgpuBus;
  UINT8                                       NvDgpuGen;
} OPTIMUS_CONFIGURATION;

typedef struct {
  OPTIMUS_CONFIGURATION                       OptimusVariable;
} HG_VARIABLE_CONFIGURATION;
#pragma pack ()

#endif
