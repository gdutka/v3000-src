/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AGESA gnb file
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  GNB
 * @e \$Revision: 315399 $   @e \$Date: 2015-03-25 20:03:45 -0500 (Wed, 25 Mar 2015) $
 *
 */

#ifndef _GFXINFOREGISTERS_H_
#define _GFXINFOREGISTERS_H_

#define SYSTEM_IA32_MAX_LINEAR_ADDRESS                         0xFFFFFFFF
#define UMA_BASE_HARDCODE                                      0xD0000000UL
#define UMA_SIZE_HARDCODE                                      0x10000000UL

#define Umc0CapAddress                                         0X50df0
#define Umc1CapAddress                                         0X150df0
#define Umc0ConfigAddress                                      0X0050100
#define Umc1ConfigAddress                                      0x0150100
#define Umc2ConfigAddress                                      0X0250100
#define Umc3ConfigAddress                                      0x0350100
#define MaskBit19                                              0x80000
#define MaskBit0To2                                            0x7

#define AddrMask_UMC0_Dimm0                                    0x00050020UL
#define AddrMask_UMC0_Dimm1                                    0x00050028UL
#define AddrMask_UMC1_Dimm0                                    0x00150020UL
#define AddrMask_UMC1_Dimm1                                    0x00150028UL
#define AddrMask_UMC2_Dimm0                                    0x00250020UL
#define AddrMask_UMC2_Dimm1                                    0x00250028UL
#define AddrMask_UMC3_Dimm0                                    0x00350020UL
#define AddrMask_UMC3_Dimm1                                    0x00350028UL

#endif
