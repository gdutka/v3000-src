/*******************************************************************************
*
* Copyright (C) 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
*
*******************************************************************************/

#ifndef _APCB_LPDDR5_SPD_K3KL6L60CM_4267_1R_H_
#define _APCB_LPDDR5_SPD_K3KL6L60CM_4267_1R_H_

#define K3KL6L60CM_4267_1R_BLOCK_0/*  //< [0-63] Base Configuration and DRAM Paramters                                           */ \
  0x23,                    /*  //< 0 Number of Serial PD Bytes Written / SPD Device Size  512bytes total but 384bytes used       */ \
  0x11,                    /*  //< 1 SPD Rev Version of SPD Documen: 1.1                                                         */ \
  0x13,                    /*  //< 2 Key Byte / DRAM Device  See DRAM Configuration Tab                                          */ \
  0x0E,                    /*  //< 3 Module Type Not Hybrid // Not Hybrid // Non Dimm solution                                   */ \
  0x85,                    /*  //< 4 SDRAM Density and Banks See DRAM Configuration Tab                                          */ \
  0x19,                    /*  //< 5 SDRAM Addressing See DRAM Configuration Tab                                                 */ \
  0x95,                    /*  //< 6 SDRAM Packing Type See DRAM Configuration Tab                                               */ \
  0x18,                    /*  //< 7 SDRAM Optional Features Max Act Window = 4096 * tREF// Max Act Count = Unlimited MAC        */ \
  0x00,                    /*  //< 8 SDRAM Thremal and Refresh Options Reserved                                                  */ \
  0x40,                    /*  //< 9 Other SDRAM Optional Features PPR Supported 1 row per Bank Groups // Soft PPR not Support   */ \
  0x00,                    /*  //< 10 Reserved -- must be coded as 0x00 Reserved                                                 */ \
  0x01,                    /*  //< 11 Module Nominal Voltage, VDD VDD2 Voltage Supply (for MR13 OP[7])                           */ \
  0x02,                    /*  //< 12 Module Organization See DRAM Configuration Tab                                             */ \
  0x02,                    /*  //< 13 Bus Width See DRAM Configuration Tab                                                       */ \
  0x00,                    /*  //< 14 Module Thermal Sensor  Thermal sensor not  incorporated onto this assem                    */ \
  0x00,                    /*  //< 15 Extended module type Reserved                                                              */ \
  0x00,                    /*  //< 16 Signal Loading See DRAM Configuration Tab                                                  */ \
  0x00,                    /*  //< 17 Timebases MTB = 125ps FTB = 1ps                                                            */ \
  0x04,                    /*  //< 18 SDRAM Minimum Cycle time(tckAVGmin) See DRAM Configuration Tab                             */ \
  0x0F,                    /*  //< 19 SDRAM Maximum Cycle time(tckAVGmmax) See DRAM Configuration Tab                            */ \
  0x92,                    /*  //< 20 CAS Latencies Supported, First Byte CL = 14, 10, 6                                         */ \
  0x54,                    /*  //< 21 CAS Latencies Supported, Second Byte CL = 28, 24, 20                                       */ \
  0x05,                    /*  //< 22 CAS Latencies Supported, Third Byte  CL = 36, 32                                           */ \
  0x00,                    /*  //< 23 CAS Latencies Supported, Fourth Byte Reserved                                              */ \
  0xB4,                    /*  //< 24 Minimum CAS Latency Time (t AA min) See DRAM Configuration Tab                             */ \
  0x00,                    /*  //< 25 Read & Write Latency Set Options Reserved                                                  */ \
  0x90,                    /*  //< 26 Minimum RAS to CAS Delay Time (t RCD min) See DRAM Configuration Tab                       */ \
  0xA8,                    /*  //< 27 Minimum Row Precharge Delay Time (t RPab min) See DRAM Configuration Tab                   */ \
  0x90,                    /*  //< 28 Minimum Row Precharge Delay Time (t RPpb min) See DRAM Configuration Tab                   */ \
  0x90,                    /*  //< 29 Minimum Refresh Recovery Delay Time (t RFCab min), LSB See DRAM Configuration Tab          */ \
  0x06,                    /*  //< 30 Minimum Refresh Recovery Delay Time (t RFCab min), MSB See DRAM Configuration Tab          */ \
  0xC0,                    /*  //< 31 Minimum Refresh Recovery Delay Time (t RFCpb min), LSB See DRAM Configuration Tab          */ \
  0x03,                    /*  //< 32 Minimum Refresh Recovery Delay Time (t RFCpb min), MSB See DRAM Configuration Tab          */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [33-36] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [37-40] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [41-44] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [45-48] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [49-52] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [53-56] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00,        /*  //< [57-59] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [60-63] Connector to SDRAM Bit Mapping Reserved                                               */ \

#define K3KL6L60CM_4267_1R_BLOCK_1/*  //< [64-127] Base Configuration and DRAM Paramters                                         */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [64-67] Connector to SDRAM Bit Mapping Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [68-71] Connector to SDRAM Bit Mapping Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [72-75] Connector to SDRAM Bit Mapping Reserved                                               */ \
  0x00, 0x00,              /*  //< [76-77] Connector to SDRAM Bit Mapping Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [78-81] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [82-85] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [86-89] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [90-93] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [94-97] Reserved -- must be coded as 0x00 Reserved                                            */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [98-101] Reserved -- must be coded as 0x00 Reserved                                           */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [102-105] Reserved -- must be coded as 0x00 Reserved                                          */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [106-109] Reserved -- must be coded as 0x00 Reserved                                          */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [110-113] Reserved -- must be coded as 0x00 Reserved                                          */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [114-117] Reserved -- must be coded as 0x00 Reserved                                          */ \
  0x00, 0x00,              /*  //< [118-119] Reserved -- must be coded as 0x00 Reserved                                          */ \
  0x00,                    /*  //< 120 Fine Offset for Minimum Row Precharge Delay Time (t RPpb min) See DRAM Configuration Tab  */ \
  0x00,                    /*  //< 121 Fine Offset for Minimum Row Precharge Delay Time (t RPab min) See DRAM Configuration Tab  */ \
  0x00,                    /*  //< 122 Fine Offset for Minimum RAS to CAS Delay Time (t RCD min) See DRAM Configuration Tab      */ \
  0x0E,                    /*  //< 123 Fine Offset for Minimum CAS Latency Time (t AA min) See DRAM Configuration Tab            */ \
  0xE7,                    /*  //< 124 Fine Offset for SDRAM Maximum Cycle Time (t CKAVG max) See DRAM Configuration Tab         */ \
  0xE0,                    /*  //< 125 Fine Offset for SDRAM Minimum Cycle Time (t CKAVG min) See DRAM Configuration Tab         */ \
  0x00,                    /*  //< 126 CRC for Base Configuration Section, Least Significant Byte Reserved                       */ \
  0x00,                    /*  //< 127 CRC for Base Configuration Section, Most Significant Byte Reserved                        */ \

#define K3KL6L60CM_4267_1R_BLOCK_2/*  //< [128-191] Module Specific Parameters                                                   */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [128-131] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [132-135] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [136-139] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [140-143] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [144-147] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [148-151] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [152-155] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [156-159] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [160-163] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [164-167] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [168-171] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [172-175] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [176-179] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [180-183] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [184-187] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [188-191] Module-Specific Section Reserved                                                    */ \

#define K3KL6L60CM_4267_1R_BLOCK_3/*  //< [192-255] Module Specific Parameters                                                   */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [192-195] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [196-199] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [200-203] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [204-207] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [208-211] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [212-215] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [216-219] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [220-223] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [224-227] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [228-231] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [232-235] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [236-239] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [240-243] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [244-247] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [248-251] Module-Specific Section Reserved                                                    */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [252-255] Module-Specific Section Reserved                                                    */ \

#define K3KL6L60CM_4267_1R_BLOCK_4/*  //< [256-319] Hybrid Memory Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [256-259] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [260-263] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [264-267] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [268-271] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [272-275] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [276-279] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [280-283] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [284-287] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [288-291] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [292-295] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [296-299] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [300-303] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [304-307] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [308-311] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [312-315] Hybrid Memory Architecture Specific Parameters Reserved                             */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [316-319] Hybrid Memory Architecture Specific Parameters Reserved                             */ \

#define K3KL6L60CM_4267_1R_BLOCK_5/*  //< [320-383] Module Manufaturing Information                                              */ \
  0x00,                    /*  //< 320 Module Manufacturer ID Code, LSB Reserved                                                 */ \
  0x00,                    /*  //< 321 Module Manufacturer ID Code, MSB Reserved                                                 */ \
  0x00,                    /*  //< 322 Module Manufacturing Location Reserved                                                    */ \
  0x00, 0x00,              /*  //< [323-324] Module Manufacturing Date Reserved                                                  */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [325-328] Module Serial Number Reserved                                                       */ \
  0x4B,                    /*  //< 329 Module Part Number K                                                                      */ \
  0x33,                    /*  //< 330 Module Part Number 3                                                                      */ \
  0x4B,                    /*  //< 331 Module Part Number K                                                                      */ \
  0x4C,                    /*  //< 332 Module Part Number L                                                                      */ \
  0x36,                    /*  //< 333 Module Part Number 6                                                                      */ \
  0x4C,                    /*  //< 334 Module Part Number L                                                                      */ \
  0x36,                    /*  //< 335 Module Part Number 6                                                                      */ \
  0x30,                    /*  //< 336 Module Part Number 0                                                                      */ \
  0x43,                    /*  //< 337 Module Part Number C                                                                      */ \
  0x4D,                    /*  //< 338 Module Part Number M                                                                      */ \
  0x2D,                    /*  //< 339 Module Part Number -                                                                      */ \
  0x4D,                    /*  //< 340 Module Part Number M                                                                      */ \
  0x47,                    /*  //< 341 Module Part Number G                                                                      */ \
  0x43,                    /*  //< 342 Module Part Number C                                                                      */ \
  0x54,                    /*  //< 343 Module Part Number T                                                                      */ \
  0x20, 0x20, 0x20, 0x20,  /*  //< [344-347] Module Part Number None                                                             */ \
  0x20,                    /*  //< 348 Module Part Number None                                                                   */ \
  0x00,                    /*  //< 349 Module Revision Code None                                                                 */ \
  0x80,                    /*  //< 350 DRAM Manufacturer ID Code, LSB # continuation codes                                       */ \
  0xCE,                    /*  //< 351 DRAM Manufacturer ID Code, MSB Samsung                                                    */ \
  0x43,                    /*  //< 352 DRAM Stepping C-Die                                                                       */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [353-356] Manufacturer's Specific Data Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [357-360] Manufacturer's Specific Data Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [361-364] Manufacturer's Specific Data Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [365-368] Manufacturer's Specific Data Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [369-372] Manufacturer's Specific Data Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [373-376] Manufacturer's Specific Data Reserved                                               */ \
  0x00, 0x00, 0x00, 0x00,  /*  //< [377-380] Manufacturer's Specific Data Reserved                                               */ \
  0x00,                    /*  //< 381 Manufacturer's Specific Data Reserved                                                     */ \
  0x00, 0x00,              /*  //< [382-383] Reserved Reserved                                                                   */ \

#define K3KL6L60CM_4267_1R_BLOCK_6/*  //< [384-447] End User Programable                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\

#define K3KL6L60CM_4267_1R_BLOCK_7/*  //< [448-511] End User Programable                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\

#define K3KL6L60CM_4267_1R_SPD_DATA \
  K3KL6L60CM_4267_1R_BLOCK_0 \
  K3KL6L60CM_4267_1R_BLOCK_1 \
  K3KL6L60CM_4267_1R_BLOCK_2 \
  K3KL6L60CM_4267_1R_BLOCK_3 \
  K3KL6L60CM_4267_1R_BLOCK_4 \
  K3KL6L60CM_4267_1R_BLOCK_5 \
  K3KL6L60CM_4267_1R_BLOCK_6 \
  K3KL6L60CM_4267_1R_BLOCK_7 \


#endif  //_APCB_LPDDR5_SPD_K3KL6L60CM_4267_1R_H_
