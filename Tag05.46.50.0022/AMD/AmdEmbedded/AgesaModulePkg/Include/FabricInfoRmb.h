/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 19h Rembrandt Information definition
 *
 */

#ifndef _FABRIC_INFO_RMB_H_
#define _FABRIC_INFO_RMB_H_
#pragma pack (push, 1)

// Fabric IDs
#define  RMB_CS0_FABRIC_ID         0
#define  RMB_CS1_FABRIC_ID         1
#define  RMB_CS2_FABRIC_ID         2
#define  RMB_CS3_FABRIC_ID         3

#define  RMB_CSUMC0_FABRIC_ID     RMB_CS0_FABRIC_ID
#define  RMB_CSUMC1_FABRIC_ID     RMB_CS1_FABRIC_ID
#define  RMB_CSUMC2_FABRIC_ID     RMB_CS2_FABRIC_ID
#define  RMB_CSUMC3_FABRIC_ID     RMB_CS2_FABRIC_ID

#define  RMB_GCM0_FABRIC_ID       4
#define  RMB_GCM1_FABRIC_ID       5
#define  RMB_GCM2_FABRIC_ID       6
#define  RMB_GCM3_FABRIC_ID       7

#define  RMB_CCM0_FABRIC_ID       8

#define  RMB_NCM0_FABRIC_ID       9
#define  RMB_NCM1_FABRIC_ID       0xA

#define  RMB_MMHUB_FABRIC_ID      RMB_NCM0_FABRIC_ID
#define  RMB_DCE_FABRIC_ID        RMB_NCM1_FABRIC_ID

#define  RMB_NCS0_FABRIC_ID       0xB
#define  RMB_NCS1_FABRIC_ID       0xC

#define  RMB_ATHUB_FABRIC_ID      RMB_NCS0_FABRIC_ID
#define  RMB_CREST_FABRIC_ID      RMB_NCS1_FABRIC_ID

#define  RMB_IOMS0_FABRIC_ID      0xD

#define  RMB_PIE_FABRIC_ID        0xE

// Instance IDs
#define  RMB_CS0_INSTANCE_ID       0
#define  RMB_CS1_INSTANCE_ID       1
#define  RMB_CS2_INSTANCE_ID       2
#define  RMB_CS3_INSTANCE_ID       3

#define  RMB_CSUMC0_INSTANCE_ID   RMB_CS0_INSTANCE_ID
#define  RMB_CSUMC1_INSTANCE_ID   RMB_CS1_INSTANCE_ID
#define  RMB_CSUMC2_INSTANCE_ID   RMB_CS2_INSTANCE_ID

#define  RMB_GCM0_INSTANCE_ID     4
#define  RMB_GCM1_INSTANCE_ID     5
#define  RMB_GCM2_INSTANCE_ID     6
#define  RMB_GCM3_INSTANCE_ID     7

#define  RMB_CCM0_INSTANCE_ID     8

#define  RMB_NCM0_INSTANCE_ID     9
#define  RMB_NCM1_INSTANCE_ID     0xA

#define  RMB_MMHUB_INSTANCE_ID    RMB_NCM0_INSTANCE_ID
#define  RMB_DCE_INSTANCE_ID      RMB_NCM1_INSTANCE_ID

#define  RMB_NCS0_INSTANCE_ID     0xB
#define  RMB_NCS1_INSTANCE_ID     0xC

#define  RMB_ATHUB_INSTANCE_ID    RMB_NCS0_INSTANCE_ID
#define  RMB_CREST_INSTANCE_ID    RMB_NCS1_INSTANCE_ID

#define  RMB_IOMS0_INSTANCE_ID    0xD

#define  RMB_PIE_INSTANCE_ID      0xE

#define  RMB_TCDX0_INSTANCE_ID    0xF
#define  RMB_TCDX1_INSTANCE_ID    0x10
#define  RMB_TCDX2_INSTANCE_ID    0x11
#define  RMB_TCDX3_INSTANCE_ID    0x12
#define  RMB_TCDX4_INSTANCE_ID    0x13

#define  RMB_NUM_CS_BLOCKS         4
#define  RMB_NUM_CS_UMC_BLOCKS     4
#define  RMB_NUM_CCM_BLOCKS        1
#define  RMB_NUM_IOMS_BLOCKS       1
#define  RMB_NUM_PIE_BLOCKS        1
#define  RMB_NUM_TCDX_BLOCKS       5

#define  RMB_NUM_DF_BLOCKS        20

/* PCI Bus Regions */
#define RMB_NUMBER_OF_BUS_REGIONS         0x8
#define RMB_BUS_REGION_REGISTER_OFFSET    0x4

/* x86 IO Regions */
#define RMB_NUMBER_OF_X86IO_REGIONS       0x8
#define RMB_X86IO_REGION_REGISTER_OFFSET  0x8

/* DRAM Regions */
#define RMB_NUMBER_OF_DRAM_REGIONS        0x2
#define RMB_DRAM_REGION_REGISTER_OFFSET   0x8

/* MMIO Regions */
#define RMB_NUMBER_OF_MMIO_REGIONS        0x8
#define RMB_MMIO_REGION_REGISTER_OFFSET   0x10

#define RMB_MAX_SOCKETS                   1   ///< Max number of sockets in system
#define RMB_MAX_DIES_PER_SOCKET           1   ///< Max number of dies per socket
#define RMB_MAX_CCD_PER_SOCKET            0   ///< Max number of CCD per socket
#define RMB_MAX_CHANNELS_PER_DIE          2   ///< Max Channels per die
#define RMB_MAX_HOST_BRIDGES_PER_DIE      1   ///< Max host bridges per die

#define RMB_FABRIC_ID_SOCKET_SHIFT        0
#define RMB_FABRIC_ID_SOCKET_SIZE_MASK    0
#define RMB_FABRIC_ID_DIE_SHIFT           0
#define RMB_FABRIC_ID_DIE_SIZE_MASK       0

#pragma pack (pop)
#endif /* _FABRIC_INFO_RMB_H_ */
