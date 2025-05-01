/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#ifndef _APCB_DDR5_SPD_MT48B1G16_1R_H_
#define _APCB_DDR5_SPD_MT48B1G16_1R_H_

//
// Base Configuration and DRAM Parameters
//
#define MT48B1G16_1R_BLOCK_0 /*  //< [0-63] Base Configuration and DRAM Parameters                                             */ \
  0x30,                      /*  //< 0  1024 SPD bytes total                                                                   */ \
  0x09,                      /*  //< 1  SPD Revision 0.9                                                                       */ \
  0x12,                      /*  //< 2  DRAM Type: DDR5 SDRAM                                                                  */ \
  0x03,                      /*  //< 3  Module Type: Not Hybrid / SODIMM Base Module Type                                      */ \
  0x04,                      /*  //< 4  First SDRAM: Monolithic SDRAM / 16 Gb Density Per Die                                  */ \
  0x00,                      /*  //< 5  16 Row bits, 10 Column bits                                                            */ \
  0x40,                      /*  //< 6  x16 SDRAM I/O Width                                                                    */ \
  0x42,                      /*  //< 7  First SDRAM: 4 bank groups / 4 banks per bank group                                    */ \
  0x00,                      /*  //< 8  Second SDRAM: No memory                                                                */ \
  0x00,                      /*  //< 9  Second SDRAM: No memory                                                                */ \
  0x00,                      /*  //< 10 Second SDRAM: No memory                                                                */ \
  0x00,                      /*  //< 11 Second SDRAM: No memory                                                                */ \
  0x60,                      /*  //< 12 Post package repair supported / Soft PPR supported                                     */ \
  0x00, 0x00, 0x00,          /*  //< 13-15 Reserved                                                                            */ \
  0x00,                      /*  //< 16 SDRAM Nominal Voltage: VDD 1.1v                                                        */ \
  0x00,                      /*  //< 17 SDRAM Nominal Voltage: VDDQ 1.1v                                                       */ \
  0x00,                      /*  //< 18 SDRAM Nominal Voltage: VDDP 1.1v                                                       */ \
  0x00,                      /*  //< 19 Reserved                                                                               */ \
  0xA0, 0x01,                /*  //< 20-21 tCKAVGmin = 0.416 ns (DDR5-4800)                                                    */ \
  0xE8, 0x03,                /*  //< 22-23 tCKAVGmax = 1.000 ns                                                                */ \
  0x72,                      /*  //< 24 CAS Latencies supported. CL = 22, 28, 30, 32                                           */ \
  0x0D,                      /*  //< 25 CAS Latencies supported. CL = 36, 40, 42                                               */ \
  0x00,                      /*  //< 26 CAS Latencies supported.                                                               */ \
  0x00,                      /*  //< 27 CAS Latencies supported.                                                               */ \
  0x00,                      /*  //< 28 CAS Latencies supported.                                                               */ \
  0x00,                      /*  //< 29 Reserved                                                                               */ \
  0x00, 0x41,                /*  //< 30-31 Minimum CAS Latency (tAAmin) = 16.640ns                                             */ \
  0x00, 0x41,                /*  //< 32-33 Minimum RAS-to-CAS Delay (tRCDmin) = 16.640ns                                       */ \
  0x00, 0x41,                /*  //< 33-34 Minimum Row Precharge Delay (tRPmin) = 16.640ns                                     */ \
  0x00, 0x7D,                /*  //< 36-37 Minimum Active-to-Precharge Delay (tRASmin) = 32ns                                  */ \
  0x00, 0xBE,                /*  //< 38-39 Minimum Active-to-Active/Refresh Delay (tRCmin) = 48.640ns                          */ \
  0x00, 0x75,                /*  //< 40-41 Minimum Write Recovery Time (tWRmin) = 29.952ns                                     */ \
  0x27, 0x01,                /*  //< 42-43 Minimum Refresh Recovery Delay Time (tRFC1min) = 295ns                              */ \
  0xA0, 0x00,                /*  //< 44-45 Minimum Refresh Recovery Delay Time (tRFC2min) = 160ns                              */ \
  0x82, 0x00,                /*  //< 46-47 Minimum Refresh Recovery Delay Time (tRFCsbmin) = 130ns                             */ \
  0x00, 0x00,                /*  //< 48-49 Minimum Refresh Recovery Delay Time (tRFC1_dlrmin)                                  */ \
  0x00, 0x00,                /*  //< 50-51 Minimum Refresh Recovery Delay Time (tRFC2_dlrmin)                                  */ \
  0x00, 0x00,                /*  //< 52-53 Minimum Refresh Recovery Delay Time (tRFCsb_dlrmin)                                 */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 54-57 Primary SDRAM Refresh Management                                                    */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 58-61 Reserved                                                                            */ \
  0x00, 0x00                 /*  //< 62-63 Reserved                                                                            */

#define MT48B1G16_1R_BLOCK_1 /*  //< [64-127] Base Configuration and DRAM Parameters                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_2 /*  //< [128-191] Reserved for future use                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

//
// Standard Module Parameters
//
#define MT48B1G16_1R_BLOCK_3 /*  //< [192-255] Standard Module Parameters                                                      */ \
  0x09,                      /*  //< 192 SPD Revision 0.9                                                                      */ \
  0x00,                      /*  //< 193 Reserved                                                                              */ \
  0x80, 0xB3,                /*  //< 194-195 SPD Manufacturer's ID Code = 0xB380                                               */ \
  0x80,                      /*  //< 196 SPD Device Type: Installed / SPD5118                                                  */ \
  0x20,                      /*  //< 197 SPD Device Revision Revision Number 2.0                                               */ \
  0x80, 0xB3,                /*  //< 198-199 PMIC 0 Manufacturer's ID Code = 0xB380                                            */ \
  0x82,                      /*  //< 200 PMIC 0 Device Type: PMIC Installed / PMIC5100                                         */ \
  0x10,                      /*  //< 201 PMIC 0 Revision Number 1.0                                                            */ \
  0x00, 0x00,                /*  //< 202-203 PMIC 1 Manufacturer's ID Code = 0x0000                                            */ \
  0x00,                      /*  //< 204 PMIC 1 Device Type                                                                    */ \
  0x00,                      /*  //< 205 PMIC 1 Revision Number                                                                */ \
  0x00, 0x00,                /*  //< 206-207 PMIC 2 Manufacturer's ID Code = 0x0000                                            */ \
  0x00,                      /*  //< 208 PMIC 2 Device Type                                                                    */ \
  0x00,                      /*  //< 209 PMIC 2 Revision Number                                                                */ \
  0x00, 0x00,                /*  //< 210-211 Thermal Sensor Manufacturer's ID Code = 0x0000                                    */ \
  0x00,                      /*  //< 212 Thermal Sensor Device Type                                                            */ \
  0x00,                      /*  //< 213 Thermal Sensor Revision Number                                                        */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 214-217 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 218-221 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 222-225 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 226-229 Reserved                                                                          */ \
  0x00,                      /*  //< 230 Module Nominal Height                                                                 */ \
  0x00,                      /*  //< 231 Module Maximum Thickness                                                              */ \
  0xFF,                      /*  //< 232 Reference Raw Card Used: ZZ (no JEDEC reference raw card design used)                 */ \
  0x81,                      /*  //< 233 DIMM Attributes: 1 row / Temperature Grade XT                                         */ \
  0x00,                      /*  //< 234 Module Organization: 1 Package Rank                                                   */ \
  0x22,                      /*  //< 235 Bus Width: 32 bits Primary bus width per Channel / 2 channels per DIMM                */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 236-239 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 240-243 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 244-247 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 248-251 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00     /*  //< 252-255 Reserved                                                                          */ \

#define MT48B1G16_1R_BLOCK_4 /*  //< [256-319] Standard Module Parameters                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_5 /*  //< [320-383] Standard Module Parameters                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_6 /*  //< [384-447] Standard Module Parameters                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x05

#define MT48B1G16_1R_BLOCK_7 /*  //< [448-511] Reserved for future use                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 448-509 Reserved                 */ \
  0x00,                      /*  //< 510 CRC                                                                                   */ \
  0x00                       /*  //< 511 CRC                                                                                   */

//
// Manufacturing information
//
#define MT48B1G16_1R_BLOCK_8 /*  //< [512-575] Manufacturing information                                                       */ \
  0x80, 0x2C,                /*  //< 512-513 Module Manufacturer's ID Code = 0x2C80                                            */ \
  0x00,                      /*  //< 514 Module Manufacturing Location                                                         */ \
  0x00, 0x00,                /*  //< 515-516 Module Manufacturing Date                                                         */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 517-520 Module Serial Number                                                              */ \
  0x4D,                      /*  //< 521 Module Part Number = 'M'                                                              */ \
  0x54,                      /*  //< 522 Module Part Number = 'T'                                                              */ \
  0x34,                      /*  //< 523 Module Part Number = '4'                                                              */ \
  0x38,                      /*  //< 524 Module Part Number = '8'                                                              */ \
  0x42,                      /*  //< 525 Module Part Number = 'B'                                                              */ \
  0x31,                      /*  //< 526 Module Part Number = '1'                                                              */ \
  0x47,                      /*  //< 527 Module Part Number = 'G'                                                              */ \
  0x31,                      /*  //< 528 Module Part Number = '1'                                                              */ \
  0x36,                      /*  //< 529 Module Part Number = '6'                                                              */ \
  0x5F,                      /*  //< 530 Module Part Number = '_'                                                              */ \
  0x31,                      /*  //< 531 Module Part Number = '1'                                                              */ \
  0x52,                      /*  //< 532 Module Part Number = 'R'                                                              */ \
  0x20,                      /*  //< 533 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 534 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 535 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 536 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 537 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 538 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 539 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 540 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 541 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 542 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 543 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 544 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 545 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 546 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 547 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 548 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 549 Module Part Number = ' '                                                              */ \
  0x20,                      /*  //< 550 Module Part Number = ' '                                                              */ \
  0x59,                      /*  //< 551 Revision 89                                                                           */ \
  0x80, 0x2C,                /*  //< 552-553 DRAM Manufacturer's ID Code = 0x2C80                                              */ \
  0x41,                      /*  //< 554 DRAM Stepping = 4.1                                                                   */ \
  0x00,                      /*  //< 555 Manufacturer's Specific Data                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 556-559 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 560-563 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 564-567 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00,    /*  //< 568-571 Reserved                                                                          */ \
  0x00, 0x00, 0x00, 0x00     /*  //< 556-575 Reserved                                                                          */

#define MT48B1G16_1R_BLOCK_9 /*  //< [576-639] Manufacturing information                                                       */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

//
// End User Programmable
//
#define MT48B1G16_1R_BLOCK_A /*  //< [640-703] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_B /*  //< [704-767] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_C /*  //< [768-831] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_D /*  //< [832-895] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_E /*  //< [896-959] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MT48B1G16_1R_BLOCK_F /*  //< [960-1023] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#endif  //ifndef  _APCB_DDR5_SPD_MT48B1G16_1R_H_
