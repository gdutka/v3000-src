/*******************************************************************************
*
* Copyright 2020 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
* AMD is granting You permission to use this software and documentation (if
* any) (collectively, the "Software") pursuant to the terms and conditions of
* the Software License Agreement included with the Software. If You do not have
* a copy of the Software License Agreement, contact Your AMD representative for
* a copy.
*
* You agree that You will not reverse engineer or decompile the Software, in
* whole or in part, except as allowed by applicable law.
*
* WARRANTY DISCLAIMER: THE SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
* KIND. AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
* BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, QUALITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT AND WARRANTIES
* ARISING FROM CUSTOM OF TRADE OR COURSE OF USAGE WITH RESPECT TO THE SOFTWARE,
* INCLUDING WITHOUT LIMITATION, THAT THE SOFTWARE WILL RUN UNINTERRUPTED OR
* ERROR-FREE. THE ENTIRE RISK ASSOCIATED WITH THE USE OF THE SOFTWARE IS
* ASSUMED BY YOU. Some jurisdictions do not allow the exclusion of implied
* warranties, so the above exclusion may not apply to You, but only to the
* extent required by law.
*
* LIMITATION OF LIABILITY AND INDEMNIFICATION: TO THE EXTENT NOT PROHIBITED BY
* APPLICABLE LAW, AMD AND ITS LICENSORS WILL NOT, UNDER ANY CIRCUMSTANCES BE
* LIABLE TO YOU FOR ANY PUNITIVE, DIRECT, INCIDENTAL, INDIRECT, SPECIAL OR
* CONSEQUENTIAL DAMAGES ARISING FROM POSSESSION OR USE OF THE SOFTWARE OR
* OTHERWISE IN CONNECTION WITH ANY PROVISION OF THIS AGREEMENT EVEN IF AMD AND
* ITS LICENSORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. THIS
* INCLUDES, WITHOUT LIMITATION, DAMAGES DUE TO LOST OR MISAPPROPRIATED DATA,
* LOST PROFITS OR CONFIDENTIAL OR OTHER INFORMATION, FOR BUSINESS INTERRUPTION,
* FOR PERSONAL INJURY, FOR LOSS OF PRIVACY, FOR FAILURE TO MEET ANY DUTY
* INCLUDING OF GOOD FAITH OR REASONABLE CARE, FOR NEGLIGENCE AND FOR ANY OTHER
* PECUNIARY OR OTHER LOSS WHTSOEVER. In no event shall AMD's total liability to
* You for all damages, losses, and causes of action (whether in contract, tort
* (including negligence) or otherwise) exceed the amount of $50 USD. You agree
* to defend, indemnify and hold harmless AMD, its subsidiaries and affiliates
* and their respective licensors, directors, officers, employees, affiliates or
* agents from and against any and all loss, damage, liability and other
* expenses (including reasonable attorneys' fees), resulting from Your
* possession or use of the Software or violation of the terms and conditions of
* this Agreement.
*
* U.S. GOVERNMENT RESTRICTED RIGHTS: Notice to U.S. Government End Users. The
* Software and related documentation are "commercial items", as that term is
* defined at 48 C.F.R. Section 2.101, consisting of "commercial computer
* software" and "commercial computer software documentation", as such terms are
* used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section 227.7202,
* respectively. Consistent with 48 C.F.R. Section 12.212 or 48 C.F.R. Sections
* 227.7202-1 through 227.7202-4, as applicable, the commercial computer
* software and commercial computer software documentation are being licensed to
* U.S. Government end users: (a) only as commercial items, and (b) with only
* those rights as are granted to all other end users pursuant to the terms and
* conditions set forth in this Agreement. Unpublished rights are reserved under
* the copyright laws of the United States.
*
* EXPORT RESTRICTIONS:  You shall adhere to all applicable U.S. import/export
* laws and regulations, as well as the import/export control laws and
* regulations of other countries as applicable. You further agree You will not
* export, re-export, or transfer, directly or indirectly, any product,
* technical data, software or source code received from AMD under this license,
* or the direct product of such technical data or software to any country for
* which the United States or any other applicable government requires an export
* license or other governmental approval without first obtaining such licenses
* or approvals, or in violation of any applicable laws or regulations of the
* United States or the country where the technical data or software was
* obtained. You acknowledges the technical data and software received will not,
* in the absence of authorization from U.S. or local law and regulations as
* applicable, be used by or exported, re-exported or transferred to: (i) any
* sanctioned or embargoed country, or to nationals or residents of such
* countries; (ii) any restricted end-user as identified on any applicable
* government end-user list; or (iii) any party where the end-use involves
* nuclear, chemical/biological weapons, rocket systems, or unmanned air
* vehicles.  For the most current Country Group listings, or for additional
* information about the EAR or Your obligations under those regulations, please
* refer to the website of the U.S. Bureau of Industry and Security at
* http://www.bis.doc.gov/.
*******************************************************************************/

#ifndef _APCB_DDR5SPDMICRON_H_
#define _APCB_DDR5SPDMICRON_H_

//
// define SPD_DATA_DDR5_Micron_SODIMM_32GB_2Rx8
//
#define Micron_32GB_2Rx8_BLOCK_0 /*  //< [0-63] Base Configuration and DRAM Parameters                                            */ \
  0x30, /*  //< 0  1024 SPD bytes total                                                                                           */ \
  0x09, /*  //< 1  SPD Revision 0.9                                                                                               */ \
  0x12, /*  //< 2  DRAM Type: DDR5 SDRAM                                                                                          */ \
  0x03, /*  //< 3  Module Type: Not Hybrid / SODIMM Base Module Type                                                              */ \
  0x04, /*  //< 4  First SDRAM: Monolithic SDRAM / 16 Gb Density Per Die                                                          */ \
  0x00, /*  //< 5  16 Row bits, 10 Column bits                                                                                    */ \
  0x20, /*  //< 6  x8 SDRAM I/O Width                                                                                             */ \
  0x62, /*  //< 7  First SDRAM: 8 bank groups / 4 banks per bank group                                                            */ \
  0x00, /*  //< 8  Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 9  Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 10 Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 11 Second SDRAM: No memory                                                                                        */ \
  0x60, /*  //< 12 Post package repair supported / Soft PPR supported                                                             */ \
  0x00, 0x00, 0x00, /*  //< 13-15 Reserved                                                                                        */ \
  0x00, /*  //< 16 SDRAM Nominal Voltage: VDD 1.1v                                                                                */ \
  0x00, /*  //< 17 SDRAM Nominal Voltage: VDDQ 1.1v                                                                               */ \
  0x00, /*  //< 18 SDRAM Nominal Voltage: VDDP 1.1v                                                                               */ \
  0x00, /*  //< 19 Reserved                                                                                                       */ \
  0xA0, 0x01, /*  //< 20-21 tCKAVGmin = 0.416 ns (DDR5-4800)                                                                      */ \
  0xE8, 0x03, /*  //< 22-23 tCKAVGmax = 1.000 ns                                                                                  */ \
  0x72, /*  //< 24 CAS Latencies supported. CL = 22, 28, 30, 32                                                                   */ \
  0x0D, /*  //< 25 CAS Latencies supported. CL = 36, 40, 42                                                                       */ \
  0x00, /*  //< 26 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 27 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 28 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 29 Reserved                                                                                                       */ \
  0x00, 0x41, /*  //< 30-31 Minimum CAS Latency (tAAmin) = 16.640ns                                                               */ \
  0x00, 0x41, /*  //< 32-33 Minimum RAS-to-CAS Delay (tRCDmin) = 16.640ns                                                         */ \
  0x00, 0x41, /*  //< 33-34 Minimum Row Precharge Delay (tRPmin) = 16.640ns                                                       */ \
  0x00, 0x7D, /*  //< 36-37 Minimum Active-to-Precharge Delay (tRASmin) = 32ns                                                    */ \
  0x00, 0xBE, /*  //< 38-39 Minimum Active-to-Active/Refresh Delay (tRCmin) = 48.640ns                                            */ \
  0x00, 0x75, /*  //< 40-41 Minimum Write Recovery Time (tWRmin) = 29.952ns                                                       */ \
  0x27, 0x01, /*  //< 42-43 Minimum Refresh Recovery Delay Time (tRFC1min) = 295ns                                                */ \
  0xA0, 0x00, /*  //< 44-45 Minimum Refresh Recovery Delay Time (tRFC2min) = 160ns                                                */ \
  0x82, 0x00, /*  //< 46-47 Minimum Refresh Recovery Delay Time (tRFCsbmin) = 130ns                                               */ \
  0x00, 0x00, /*  //< 48-49 Minimum Refresh Recovery Delay Time (tRFC1_dlrmin)                                                    */ \
  0x00, 0x00, /*  //< 50-51 Minimum Refresh Recovery Delay Time (tRFC2_dlrmin)                                                    */ \
  0x00, 0x00, /*  //< 52-53 Minimum Refresh Recovery Delay Time (tRFCsb_dlrmin)                                                   */ \
  0x00, 0x00, 0x00, 0x00, /*  //< 54-57 Primary SDRAM Refresh Management                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /*  //< 58-63 Reserved                                                                      */

#define Micron_32GB_2Rx8_BLOCK_1 /*  //< [64-127] Base Configuration and DRAM Parameters                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 64-125 Reserved                     */ \
  0x3A, /*  //< 126 CRC                                                                                                           */ \
  0xB6  /*  //< 127 CRC                                                                                                           */

#define Micron_32GB_2Rx8_BLOCK_2 /*  //< [128-191] Reserved for future use                                                        */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_3 /*  //< [192-255] Standard Module Parameters                                                     */ \
  0x09, 0x00, 0x80, 0xB3, 0x80, 0x20, 0x80, 0xB3, 0x82, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x01, 0x82, 0x08, 0x22, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_4 /*  //< [256-319] Standard Module Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_5 /*  //< [320-383] Standard Module Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_6 /*  //< [384-447] Standard Module Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x76

#define Micron_32GB_2Rx8_BLOCK_7 /*  //< [448-511] Reserved for future use                                                        */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 448-509 Reserved                    */ \
  0x00, /*  //< 510 CRC                                                                                                           */ \
  0x00  /*  //< 511 CRC                                                                                                           */

#define Micron_32GB_2Rx8_BLOCK_8 /*  //< [512-575] Manufacturing information                                                      */ \
  0x80, 0x2C, /*  //< 512-513 Module Manufacturer's ID Code = 0x2C80                                                              */ \
  0x00, /*  //< 514 Module Manufacturing Location                                                                                 */ \
  0x00, 0x00, /*  //< 515-516 Module Manufacturing Date                                                                           */ \
  0x00, 0x00, 0x00, 0x00, /*  //< 517-520  Module Serial Number                                                                   */ \
  0x4D, /*  //< 521 Module Part Number = 'M'                                                                                      */ \
  0x54, /*  //< 522 Module Part Number = 'T'                                                                                      */ \
  0x43, /*  //< 523 Module Part Number = 'C'                                                                                      */ \
  0x31, /*  //< 524 Module Part Number = '1'                                                                                      */ \
  0x36, /*  //< 525 Module Part Number = '6'                                                                                      */ \
  0x43, /*  //< 526 Module Part Number = 'C'                                                                                      */ \
  0x32, /*  //< 527 Module Part Number = '2'                                                                                      */ \
  0x30, /*  //< 528 Module Part Number = '0'                                                                                      */ \
  0x38, /*  //< 529 Module Part Number = '8'                                                                                      */ \
  0x35, /*  //< 530 Module Part Number = '5'                                                                                      */ \
  0x53, /*  //< 531 Module Part Number = 'S'                                                                                      */ \
  0x31, /*  //< 532 Module Part Number = '1'                                                                                      */ \
  0x53, /*  //< 533 Module Part Number = 'S'                                                                                      */ \
  0x43, /*  //< 534 Module Part Number = 'C'                                                                                      */ \
  0x34, /*  //< 535 Module Part Number = '4'                                                                                      */ \
  0x38, /*  //< 536 Module Part Number = '8'                                                                                      */ \
  0x42, /*  //< 537 Module Part Number = 'B'                                                                                      */ \
  0x41, /*  //< 538 Module Part Number = 'A'                                                                                      */ \
  0x59, /*  //< 539 Module Part Number = 'Y'                                                                                      */ \
  0x20, /*  //< 540 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 541 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 542 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 543 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 544 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 545 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 546 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 547 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 548 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 549 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 550 Module Part Number = ' '                                                                                      */ \
  0x59, /*  //< 551 Revision 89                                                                                                   */ \
  0x80, 0x2C, /*  //< 552-553 DRAM Manufacturer's ID Code = 0x2C80                                                                */ \
  0x41, /*  //< 554 DRAM Stepping = 4.1                                                                                           */ \
  0x00, /*  //< 555 Manufacturer's Specific Data                                                                                  */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00  /*  //< 556-575 Reserved                                                                                */

#define Micron_32GB_2Rx8_BLOCK_9 /*  //< [576-639] Manufacturing information                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_A /*  //< [640-703] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_B /*  //< [704-767] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_C /*  //< [768-831] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_D /*  //< [832-895] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_E /*  //< [896-959] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_32GB_2Rx8_BLOCK_F /*  //< [960-1023] End User Programmable                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

//
// #define SPD_DATA_DDR5_Micron_SODIMM_16GB_1Rx8
//
#define Micron_16GB_1Rx8_BLOCK_0 /*  //< [0-63] Base Configuration and DRAM Parameters                                            */ \
  0x30, /*  //< 0  1024 SPD bytes total                                                                                           */ \
  0x09, /*  //< 1  SPD Revision 0.9                                                                                               */ \
  0x12, /*  //< 2  DRAM Type: DDR5 SDRAM                                                                                          */ \
  0x03, /*  //< 3  Module Type: Not Hybrid / SODIMM Base Module Type                                                              */ \
  0x04, /*  //< 4  First SDRAM: Monolithic SDRAM / 16 Gb Density Per Die                                                          */ \
  0x00, /*  //< 5  16 Row bits, 10 Column bits                                                                                    */ \
  0x20, /*  //< 6  x8 SDRAM I/O Width                                                                                             */ \
  0x62, /*  //< 7  First SDRAM: 8 bank groups / 4 banks per bank group                                                            */ \
  0x00, /*  //< 8  Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 9  Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 10 Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 11 Second SDRAM: No memory                                                                                        */ \
  0x60, /*  //< 12 Post package repair supported / Soft PPR supported                                                             */ \
  0x00, 0x00, 0x00, /*  //< 13-15 Reserved                                                                                        */ \
  0x00, /*  //< 16 SDRAM Nominal Voltage: VDD 1.1v                                                                                */ \
  0x00, /*  //< 17 SDRAM Nominal Voltage: VDDQ 1.1v                                                                               */ \
  0x00, /*  //< 18 SDRAM Nominal Voltage: VDDP 1.1v                                                                               */ \
  0x00, /*  //< 19 Reserved                                                                                                       */ \
  0xA0, 0x01, /*  //< 20-21 tCKAVGmin = 0.416 ns (DDR5-4800)                                                                      */ \
  0xE8, 0x03, /*  //< 22-23 tCKAVGmax = 1.000 ns                                                                                  */ \
  0x72, /*  //< 24 CAS Latencies supported. CL = 22, 28, 30, 32                                                                   */ \
  0x0D, /*  //< 25 CAS Latencies supported. CL = 36, 40, 42                                                                       */ \
  0x00, /*  //< 26 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 27 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 28 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 29 Reserved                                                                                                       */ \
  0x00, 0x41, /*  //< 30-31 Minimum CAS Latency (tAAmin) = 16.640ns                                                               */ \
  0x00, 0x41, /*  //< 32-33 Minimum RAS-to-CAS Delay (tRCDmin) = 16.640ns                                                         */ \
  0x00, 0x41, /*  //< 33-34 Minimum Row Precharge Delay (tRPmin) = 16.640ns                                                       */ \
  0x00, 0x7D, /*  //< 36-37 Minimum Active-to-Precharge Delay (tRASmin) = 32ns                                                    */ \
  0x00, 0xBE, /*  //< 38-39 Minimum Active-to-Active/Refresh Delay (tRCmin) = 48.640ns                                            */ \
  0x00, 0x75, /*  //< 40-41 Minimum Write Recovery Time (tWRmin) = 29.952ns                                                       */ \
  0x27, 0x01, /*  //< 42-43 Minimum Refresh Recovery Delay Time (tRFC1min) = 295ns                                                */ \
  0xA0, 0x00, /*  //< 44-45 Minimum Refresh Recovery Delay Time (tRFC2min) = 160ns                                                */ \
  0x82, 0x00, /*  //< 46-47 Minimum Refresh Recovery Delay Time (tRFCsbmin) = 130ns                                               */ \
  0x00, 0x00, /*  //< 48-49 Minimum Refresh Recovery Delay Time (tRFC1_dlrmin)                                                    */ \
  0x00, 0x00, /*  //< 50-51 Minimum Refresh Recovery Delay Time (tRFC2_dlrmin)                                                    */ \
  0x00, 0x00, /*  //< 52-53 Minimum Refresh Recovery Delay Time (tRFCsb_dlrmin)                                                   */ \
  0x00, 0x00, 0x00, 0x00, /*  //< 54-57 Primary SDRAM Refresh Management                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /*  //< 58-63 Reserved                                                                      */

#define Micron_16GB_1Rx8_BLOCK_1 /*  //< [64-127] Base Configuration and DRAM Parameters                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 64-125 Reserved                     */ \
  0x3A, /*  //< 126 CRC                                                                                                           */ \
  0xB6  /*  //< 127 CRC                                                                                                           */

#define Micron_16GB_1Rx8_BLOCK_2 /*  //< [128-191] Reserved for future use                                                        */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_3 /*  //< [192-255] Standard Module Parameters                                                     */ \
  0x09, 0x00, 0x80, 0xB3, 0x80, 0x20, 0x80, 0xB3, 0x82, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x00, 0x82, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_4 /*  //< [256-319] Standard Module Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_5 /*  //< [320-383] Standard Module Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_6 /*  //< [384-447] Standard Module Parameters                                                     */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0xBC

#define Micron_16GB_1Rx8_BLOCK_7 /*  //< [448-511] Reserved for future use                                                        */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 448-509 Reserved                    */ \
  0x00, /*  //< 510 CRC                                                                                                           */ \
  0x00  /*  //< 511 CRC                                                                                                           */

#define Micron_16GB_1Rx8_BLOCK_8 /*  //< [512-575] Manufacturing information                                                      */ \
  0x80, 0x2C, /*  //< 512-513 Module Manufacturer's ID Code = 0x2C80                                                              */ \
  0x00, /*  //< 514 Module Manufacturing Location                                                                                 */ \
  0x00, 0x00, /*  //< 515-516 Module Manufacturing Date                                                                           */ \
  0x00, 0x00, 0x00, 0x00, /*  //< 517-520  Module Serial Number                                                                   */ \
  0x4D, /*  //< 521 Module Part Number = 'M'                                                                                      */ \
  0x54, /*  //< 522 Module Part Number = 'T'                                                                                      */ \
  0x43, /*  //< 523 Module Part Number = 'C'                                                                                      */ \
  0x38, /*  //< 524 Module Part Number = '8'                                                                                      */ \
  0x43, /*  //< 525 Module Part Number = 'C'                                                                                      */ \
  0x31, /*  //< 526 Module Part Number = '1'                                                                                      */ \
  0x30, /*  //< 527 Module Part Number = '0'                                                                                      */ \
  0x38, /*  //< 528 Module Part Number = '8'                                                                                      */ \
  0x34, /*  //< 529 Module Part Number = '4'                                                                                      */ \
  0x53, /*  //< 530 Module Part Number = 'S'                                                                                      */ \
  0x31, /*  //< 531 Module Part Number = '1'                                                                                      */ \
  0x53, /*  //< 532 Module Part Number = 'S'                                                                                      */ \
  0x43, /*  //< 533 Module Part Number = 'C'                                                                                      */ \
  0x34, /*  //< 534 Module Part Number = '4'                                                                                      */ \
  0x38, /*  //< 535 Module Part Number = '8'                                                                                      */ \
  0x42, /*  //< 536 Module Part Number = 'B'                                                                                      */ \
  0x41, /*  //< 537 Module Part Number = 'A'                                                                                      */ \
  0x59, /*  //< 538 Module Part Number = 'Y'                                                                                      */ \
  0x20, /*  //< 539 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 540 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 541 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 542 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 543 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 544 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 545 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 546 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 547 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 548 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 549 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 550 Module Part Number = ' '                                                                                      */ \
  0x59, /*  //< 551 Revision 89                                                                                                   */ \
  0x80, 0x2C, /*  //< 552-553 DRAM Manufacturer's ID Code = 0x2C80                                                                */ \
  0x41, /*  //< 554 DRAM Stepping = 4.1                                                                                           */ \
  0x00, /*  //< 555 Manufacturer's Specific Data                                                                                  */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00  /*  //< 556-575 Reserved                                                                                */

#define Micron_16GB_1Rx8_BLOCK_9 /*  //< [576-639] Manufacturing information                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_A /*  //< [640-703] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_B /*  //< [704-767] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_C /*  //< [768-831] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_D /*  //< [832-895] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_E /*  //< [896-959] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_16GB_1Rx8_BLOCK_F /*  //< [960-1023] End User Programmable                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

//
// #define SPD_DATA_DDR5_Micron_SODIMM_8GB_1Rx16
//
#define Micron_8GB_1Rx8_BLOCK_0 /*  //< [0-63] Base Configuration and DRAM Parameters                                             */ \
  0x30, /*  //< 0  1024 SPD bytes total                                                                                           */ \
  0x09, /*  //< 1  SPD Revision 0.9                                                                                               */ \
  0x12, /*  //< 2  DRAM Type: DDR5 SDRAM                                                                                          */ \
  0x03, /*  //< 3  Module Type: Not Hybrid / SODIMM Base Module Type                                                              */ \
  0x04, /*  //< 4  First SDRAM: Monolithic SDRAM / 16 Gb Density Per Die                                                          */ \
  0x00, /*  //< 5  16 Row bits, 10 Column bits                                                                                    */ \
  0x40, /*  //< 6  x16 SDRAM I/O Width                                                                                            */ \
  0x42, /*  //< 7  First SDRAM: 4 bank groups / 4 banks per bank group                                                            */ \
  0x00, /*  //< 8  Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 9  Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 10 Second SDRAM: No memory                                                                                        */ \
  0x00, /*  //< 11 Second SDRAM: No memory                                                                                        */ \
  0x60, /*  //< 12 Post package repair supported / Soft PPR supported                                                             */ \
  0x00, 0x00, 0x00, /*  //< 13-15 Reserved                                                                                        */ \
  0x00, /*  //< 16 SDRAM Nominal Voltage: VDD 1.1v                                                                                */ \
  0x00, /*  //< 17 SDRAM Nominal Voltage: VDDQ 1.1v                                                                               */ \
  0x00, /*  //< 18 SDRAM Nominal Voltage: VDDP 1.1v                                                                               */ \
  0x00, /*  //< 19 Reserved                                                                                                       */ \
  0xA0, 0x01, /*  //< 20-21 tCKAVGmin = 0.416 ns (DDR5-4800)                                                                      */ \
  0xE8, 0x03, /*  //< 22-23 tCKAVGmax = 1.000 ns                                                                                  */ \
  0x72, /*  //< 24 CAS Latencies supported. CL = 22, 28, 30, 32                                                                   */ \
  0x0D, /*  //< 25 CAS Latencies supported. CL = 36, 40, 42                                                                       */ \
  0x00, /*  //< 26 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 27 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 28 CAS Latencies supported.                                                                                       */ \
  0x00, /*  //< 29 Reserved                                                                                                       */ \
  0x00, 0x41, /*  //< 30-31 Minimum CAS Latency (tAAmin) = 16.640ns                                                               */ \
  0x00, 0x41, /*  //< 32-33 Minimum RAS-to-CAS Delay (tRCDmin) = 16.640ns                                                         */ \
  0x00, 0x41, /*  //< 33-34 Minimum Row Precharge Delay (tRPmin) = 16.640ns                                                       */ \
  0x00, 0x7D, /*  //< 36-37 Minimum Active-to-Precharge Delay (tRASmin) = 32ns                                                    */ \
  0x00, 0xBE, /*  //< 38-39 Minimum Active-to-Active/Refresh Delay (tRCmin) = 48.640ns                                            */ \
  0x00, 0x75, /*  //< 40-41 Minimum Write Recovery Time (tWRmin) = 29.952ns                                                       */ \
  0x27, 0x01, /*  //< 42-43 Minimum Refresh Recovery Delay Time (tRFC1min) = 295ns                                                */ \
  0xA0, 0x00, /*  //< 44-45 Minimum Refresh Recovery Delay Time (tRFC2min) = 160ns                                                */ \
  0x82, 0x00, /*  //< 46-47 Minimum Refresh Recovery Delay Time (tRFCsbmin) = 130ns                                               */ \
  0x00, 0x00, /*  //< 48-49 Minimum Refresh Recovery Delay Time (tRFC1_dlrmin)                                                    */ \
  0x00, 0x00, /*  //< 50-51 Minimum Refresh Recovery Delay Time (tRFC2_dlrmin)                                                    */ \
  0x00, 0x00, /*  //< 52-53 Minimum Refresh Recovery Delay Time (tRFCsb_dlrmin)                                                   */ \
  0x00, 0x00, 0x00, 0x00, /*  //< 54-57 Primary SDRAM Refresh Management                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /*  //< 58-63 Reserved                                                                      */

#define Micron_8GB_1Rx8_BLOCK_1 /*  //< [64-127] Base Configuration and DRAM Parameters                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 64-125 Reserved                     */ \
   0x1F, /*  //< 126 CRC                                                                                                          */ \
   0x83  /*  //< 127 CRC                                                                                                          */

#define Micron_8GB_1Rx8_BLOCK_2 /*  //< [128-191] Reserved for future use                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_3 /*  //< [192-255] Standard Module Parameters                                                      */ \
  0x09, 0x00, 0x80, 0xB3, 0x80, 0x20, 0x80, 0xB3, 0x82, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x02, 0x81, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_4 /*  //< [256-319] Standard Module Parameters                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_5 /*  //< [320-383] Standard Module Parameters                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_6 /*  //< [384-447] Standard Module Parameters                                                      */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x05

#define Micron_8GB_1Rx8_BLOCK_7 /*  //< [448-511] Reserved for future use                                                         */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  //< 448-509 Reserved                    */ \
  0x00, /*  //< 510 CRC                                                                                                           */ \
  0x00  /*  //< 511 CRC                                                                                                           */

#define Micron_8GB_1Rx8_BLOCK_8 /*  //< [512-575] Manufacturing information                                                       */ \
  0x80, 0x2C, /*  //< 512-513 Module Manufacturer's ID Code = 0x2C80                                                              */ \
  0x00, /*  //< 514 Module Manufacturing Location                                                                                 */ \
  0x00, 0x00, /*  //< 515-516 Module Manufacturing Date                                                                           */ \
  0x00, 0x00, 0x00, 0x00, /*  //< 517-520  Module Serial Number                                                                   */ \
  0x4D, /*  //< 521 Module Part Number = 'M'                                                                                      */ \
  0x54, /*  //< 522 Module Part Number = 'T'                                                                                      */ \
  0x43, /*  //< 523 Module Part Number = 'C'                                                                                      */ \
  0x38, /*  //< 524 Module Part Number = '4'                                                                                      */ \
  0x43, /*  //< 525 Module Part Number = 'C'                                                                                      */ \
  0x31, /*  //< 526 Module Part Number = '1'                                                                                      */ \
  0x30, /*  //< 527 Module Part Number = '0'                                                                                      */ \
  0x38, /*  //< 528 Module Part Number = '1'                                                                                      */ \
  0x34, /*  //< 529 Module Part Number = '6'                                                                                      */ \
  0x33, /*  //< 530 Module Part Number = '3'                                                                                      */ \
  0x53, /*  //< 531 Module Part Number = 'S'                                                                                      */ \
  0x31, /*  //< 532 Module Part Number = '1'                                                                                      */ \
  0x53, /*  //< 533 Module Part Number = 'S'                                                                                      */ \
  0x43, /*  //< 534 Module Part Number = 'C'                                                                                      */ \
  0x34, /*  //< 535 Module Part Number = '4'                                                                                      */ \
  0x38, /*  //< 536 Module Part Number = '8'                                                                                      */ \
  0x42, /*  //< 537 Module Part Number = 'B'                                                                                      */ \
  0x41, /*  //< 538 Module Part Number = 'A'                                                                                      */ \
  0x59, /*  //< 539 Module Part Number = 'Y'                                                                                      */ \
  0x20, /*  //< 540 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 541 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 542 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 543 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 544 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 545 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 546 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 547 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 548 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 549 Module Part Number = ' '                                                                                      */ \
  0x20, /*  //< 550 Module Part Number = ' '                                                                                      */ \
  0x59, /*  //< 551 Revision 89                                                                                                   */ \
  0x80, 0x2C, /*  //< 552-553 DRAM Manufacturer's ID Code = 0x2C80                                                                */ \
  0x41, /*  //< 554 DRAM Stepping = 4.1                                                                                           */ \
  0x00, /*  //< 555 Manufacturer's Specific Data                                                                                  */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00  /*  //< 556-575 Reserved                                                                                */

#define Micron_8GB_1Rx8_BLOCK_9 /*  //< [576-639] Manufacturing information                                                       */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_A /*  //< [640-703] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_B /*  //< [704-767] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_C /*  //< [768-831] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_D /*  //< [832-895] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_E /*  //< [896-959] End User Programmable                                                           */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define Micron_8GB_1Rx8_BLOCK_F /*  //< [960-1023] End User Programmable                                                          */ \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#endif   //ifndef  _APCB_DDR5SPDMICRON_H_
