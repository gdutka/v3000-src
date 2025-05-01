/** @file
  This header file is for Hybrid Graphics Feature SMM driver.

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

#ifndef _HYBRID_GRAPHICS_SMM_H_
#define _HYBRID_GRAPHICS_SMM_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciExpressLib.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/SmmInt15Service.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HybridGraphicsInfo.h>
#include <HybridGraphicsDefine.h>
// #include <Library/MmPciLib.h>

//
// Int15 Function number
//
#define HYBRID_GRAPHICS_INT15_FUN_NUM         0x5F70
#define MXM_INT15_FUN_NUM                     0x5F80

#define GPP_BUS_NUM                           0x00
#define GPP_DEV_NUM                           0x01
#define GPP0_FUN_NUM                          0x01
#define GPP_LCTL_OFFSET                       0x68

// #define R_PCH_GP_N_CONFIG0                        0x100
// #define R_PCH_GPIO_LVL                            0x0C
// #define R_PCH_GPIO_LVL2                           0x38
// #define R_PCH_GPIO_LVL3                           0x48
// #define R_PCH_GP_N_CONFIG0                        0x100
// #define R_PCH_GPIO_LVL                            0x0C

#endif
