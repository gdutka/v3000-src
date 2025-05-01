/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _EARLY_VGA_GET_VGA_INFO_H_
#define _EARLY_VGA_GET_VGA_INFO_H_

#define H2O_EARLY_VGA_GET_VGA_INFO_PPI_GUID \
  {0x996E5CAE, 0xF918, 0x4314, 0x97, 0xC5, 0xBD, 0x7D, 0x0C, 0x1B, 0x8B, 0xF6}

#define VGA_BAR_1          0x91000000
#define VGA_BAR_2          0xFE600000

typedef struct {
  UINT16                Segment;
  UINT8                 Bus;
  UINT8                 Device   : 5;
  UINT8                 Function : 3;
  UINT8                 SecondaryBus;
  UINT8                 SubordinateBus;
  UINT16                MemoryBase;
  UINT16                MemoryLimit;
  UINT32                BaseRegister0;
  UINT32                BaseRegister1;
} VGA_ROOTPORT_INFO;

  
typedef struct _EARLY_VGA_GET_VGA_INFO_PPI {
  VGA_ROOTPORT_INFO               VgaRootPortInfo;
} EARLY_VGA_GET_VGA_INFO_PPI;

extern EFI_GUID gH2OEarlyVgaGetVgaInfoPpiGuid;

#endif

