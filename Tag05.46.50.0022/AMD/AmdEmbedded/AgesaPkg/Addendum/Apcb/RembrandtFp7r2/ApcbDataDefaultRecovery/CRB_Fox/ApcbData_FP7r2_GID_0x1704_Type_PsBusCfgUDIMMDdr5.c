/*******************************************************************************
*
 * Copyright 2021 Advanced Micro Devices, Inc. All rights reserved.
 *
 * AMD is granting you permission to use this software and documentation (if
 * any) (collectively, the "Materials") pursuant to the terms and conditions
 * of the Software License Agreement included with the Materials.  If you do
 * not have a copy of the Software License Agreement, contact your AMD
 * representative for a copy.
 *
 * You agree that you will not reverse engineer or decompile the Materials, in
 * whole or in part, except as allowed by applicable law.
 *
 * WARRANTY DISCLAIMER: THE MATERIALS ARE PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND.  AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT, THAT THE
 * MATERIALS WILL RUN UNINTERRUPTED OR ERROR-FREE OR WARRANTIES ARISING FROM
 * CUSTOM OF TRADE OR COURSE OF USAGE.  THE ENTIRE RISK ASSOCIATED WITH THE
 * USE OF THE MATERIAL IS ASSUMED BY YOU.  Some jurisdictions do not allow the
 * exclusion of implied warranties, so the above exclusion may not apply to
 * You.
 *
 * LIMITATION OF LIABILITY AND INDEMNIFICATION:  AMD AND ITS LICENSORS WILL
 * NOT, UNDER ANY CIRCUMSTANCES BE LIABLE TO YOU FOR ANY PUNITIVE, DIRECT,
 * INCIDENTAL, INDIRECT, SPECIAL OR CONSEQUENTIAL DAMAGES ARISING FROM USE OF
 * THE MATERIALS OR THIS AGREEMENT EVEN IF AMD AND ITS LICENSORS HAVE BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  In no event shall AMD's total
 * liability to You for all damages, losses, and causes of action (whether in
 * contract, tort (including negligence) or otherwise) exceed the amount of
 * $100 USD.  You agree to defend, indemnify and hold harmless AMD and its
 * licensors, and any of their directors, officers, employees, affiliates or
 * agents from and against any and all loss, damage, liability and other
 * expenses (including reasonable attorneys' fees), resulting from Your use of
 * the Materials or violation of the terms and conditions of this Agreement.
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are "commercial items", as
 * that term is defined at 48 C.F.R. Section 2.101, consisting of "commercial
 * computer software" and "commercial computer software documentation", as
 * such terms are used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section
 * 227.7202, respectively. Consistent with 48 C.F.R. Section 12.212 or 48
 * C.F.R. Section 227.7202-1 through 227.7202-4, as applicable, the commercial
 * computer software and commercial computer software documentation are being
 * licensed to U.S.  Government end users (a) only as commercial items and (b)
 * with only those rights as are granted to all other end users pursuant to
 * the terms and conditions set forth in this Agreement. Unpublished rights
 * are reserved under the copyright laws of the United States.
 *
 * EXPORT RESTRICTIONS: You shall adhere to all applicable U.S. import/export
 * laws and regulations, as well as the import/export control laws and
 * regulations of other countries as applicable. You further agrees to not
 * export, re-export, or transfer, directly or indirectly, any product,
 * technical data, software or source code received from AMD under this
 * license, or the direct product of such technical data or software to any
 * country for which the United States or any other applicable government
 * requires an export license or other governmental approval without first
 * obtaining such licenses or approvals; or in violation of any applicable
 * laws or regulations of the United States or the country where the technical
 * data or software was obtained.  You acknowledge that the technical data and
 * software received will not, in the absence of authorization from U.S. or
 * local law and regulations as applicable, be used by or exported,
 * re-exported or transferred to: (i) any sanctioned or embargoed country, or
 * to nationals or residents of such countries; (ii) any restricted end-user
 * as identified on any applicable government end-user list; or (iii) any
 * party where the end-use involves nuclear, chemical/biological weapons,
 * rocket systems, or unmanned air vehicles.   For the most current Country
 * Group listings, or for additional information about the EAR or Licensee's
 * obligations under those regulations, please refer to the U.S. Bureau of
 * Industry and Security's website at http://www.bis.doc.gov/.
*******************************************************************************/

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

#define PT_DONT_CARE 0xFF
#define NP 1
#define V1_5  1
#define V1_35  2
#define V1_25  4
#define V1_2   1
#define V_TBD1 2
#define V_TBD2 4
#define V1_1   1
#define V0_6   2

#define DIMM_NP 1
#define DIMM_SR 2
#define DIMM_DR 4
#define DIMM_QR 8

//
// Timing
//
#define TMG_1N    1
#define TMG_2N    0
//
// Defined values for CK_ODT, CS_ODT, and CK_ODT
//
#define D5_ODT_OFF        0
#define D5_ODT_480        1
#define D5_ODT_240        2
#define D5_ODT_120        3
#define D5_ODT_80         4
#define D5_ODT_60         5
#define D5_ODT_40         7
//
// Defined values for RTT_NOM_WR,RTT_NOM_RD
//
#define D5_RTT_OFF        0
#define D5_RTT_240        1
#define D5_RTT_120        2
#define D5_RTT_80         3
#define D5_RTT_60         4
#define D5_RTT_48         5
#define D5_RTT_40         6
#define D5_RTT_34         7
//
// Generic, un-encoded impedance values for various purposes. They can be
// translated to the appropriate value for how it is consumed.
//
#define D5_IMP_OFF        0
#define D5_IMP_480        480
#define D5_IMP_240        240
#define D5_IMP_160        160
#define D5_IMP_120        120
#define D5_IMP_96         96
#define D5_IMP_80         80
#define D5_IMP_68         68
#define D5_IMP_60         60
#define D5_IMP_53         53
#define D5_IMP_48         48
#define D5_IMP_43         43  
#define D5_IMP_40         40  
#define D5_IMP_36         36
#define D5_IMP_34         34
#define D5_IMP_32         32
#define D5_IMP_30         30
#define D5_IMP_28         28
#define D5_IMP_26         26
#define D5_IMP_25         25
//
// DFE
//
#define DFE_DIS           0
#define DFE_EN            1

/// On die termination encoding for LPDDR5
typedef enum _RTT_ODT {
  RTT_ODT_DISABLE = 0,      ///< Disabled
  RTT_ODT_240,              ///< RZQ/1
  RTT_ODT_120,              ///< RZQ/2
  RTT_ODT_80,               ///< RZQ/3
  RTT_ODT_60,               ///< RZQ/4
  RTT_ODT_48,               ///< RZQ/5
  RTT_ODT_40                ///< RZQ/6
} RTT_ODT;

#define DDR400  ((UINT32) 1 << (DDR400_FREQUENCY  / 66))
#define DDR533  ((UINT32) 1 << (DDR533_FREQUENCY  / 66))
#define DDR667  ((UINT32) 1 << (DDR667_FREQUENCY  / 66))
#define DDR800  ((UINT32) 1 << (DDR800_FREQUENCY  / 66))
#define DDR1066 ((UINT32) 1 << (DDR1066_FREQUENCY / 66))
#define DDR1333 ((UINT32) 1 << (DDR1333_FREQUENCY / 66))
#define DDR1600 ((UINT32) 1 << (DDR1600_FREQUENCY / 66))
#define DDR1866 ((UINT32) 1 << (DDR1866_FREQUENCY / 66))
#define DDR2133 ((UINT32) 1 << (DDR2133_FREQUENCY / 66))
#define DDR2400 ((UINT32) 1 << (DDR2400_FREQUENCY / 66))
#define DDR2667 ((UINT32) 1 << (DDR2667_FREQUENCY / 66))
#define DDR2933 ((UINT32) 1 << (DDR2933_FREQUENCY / 66))
#define DDR3200 ((UINT32) 1 << (DDR3200_FREQUENCY / 66))

/// DDR5 Bus configuration entry for DRAMDOWN, SODIMM, or UDIMM
typedef struct _PSCFG_DDR5_BUS_ENTRY_S {
  UINT32 DimmPerCh;       ///< Bitmap of Dimm slot per chanel
  UINT32 Memclk;          ///< Max Memclk Freq
  UINT32 Dimm0;           ///< Bitmap of rank type of Dimm0
  UINT32 Dimm1;           ///< Bitmap of rank type of Dimm0
  UINT32 CaTimingMode;    ///< CaTimingMode
  UINT32 RttNomRd;        ///< RTT_NOM_RD
  UINT32 RttNomWr;        ///< RTT_NOM_WR
  UINT32 RttWr;           ///< RTT_WR
  UINT32 RttPark;         ///< RTT_PARK
  UINT32 DqsRttPark;      ///< DQS_RTT_PARK
  UINT32 DramDrv;         ///< DRAM_DRV
  UINT32 CkOdtA;          ///< CK_ODT Group A
  UINT32 CsOdtA;          ///< CS_ODT Group A
  UINT32 CaOdtA;          ///< CA_ODT Group A
  UINT32 CkOdtB;          ///< CK_ODT Group B
  UINT32 CsOdtB;          ///< CS_ODT Group B
  UINT32 CaOdtB;          ///< CA_ODT Group B
  UINT32 POdt;            ///< PODT
  UINT32 DqDrv;           ///< DQ Drv
  UINT32 AlertPu;         ///< Alert Pullup
  UINT32 CaDrv;           ///< CA Drv
  UINT32 PhyVref;         ///< PHY Vref
  UINT32 DqVref;          ///< DQ Vref
  UINT32 CaVref;          ///< CA Vref
  UINT32 CsVref;          ///< CS Vref
  UINT32 RxDfe;           ///< RXDFE
  UINT32 TxDfe;           ///< TXDFE
} PSCFG_DDR5_BUS_ENTRY_S;

#define MP0                      0x01
#define MP1                      0x02
#define MP2                      0x04
#define MP3                      0x08
#define MP_ALL                   0xFF

//SODIMM BUSCFG DDR5
APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_PS_UDIMM_DDR5_BUSCFG,         // TypeId
  sizeof(ApcbTypeHeader),       // SizeOfType, will be fixed up by tool
  0,                            // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,    // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK         // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

  //
  // Default UDIMM Table
  //
  PSCFG_DDR5_BUS_ENTRY_S PlatformCfgBusUDimmDdr5[] = {
    //                                                      RTT         RTT                                 DQS
    //                                             1N/      NOM         NOM         RTT         RTT         RTT        DRAM       CK         CS         CA         CK         CS         CA                    DQ        Alert       CA       PHY    DQ    CA    CS     Rx       Tx
    // DPCH     Max Memclk      Dimm0    Dimm1     2N       WR          RD          WR          PARK        PARK        Drv       ODT A      ODT A      ODT A      ODT B      ODT B      ODT B      PODT       Drv       Pullup      Drv      Vref  Vref  Vref  Vref    DFE      DFE
    {   1,  DDR4400_FREQUENCY, DIMM_SR, DIMM_NP, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   1,  DDR4800_FREQUENCY, DIMM_SR, DIMM_NP, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_40,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x4C, 0x4C, DFE_DIS, DFE_DIS },
	{   1,  DDR5200_FREQUENCY, DIMM_SR, DIMM_NP, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_EN  },
    {   1,  DDR2000_FREQUENCY, DIMM_DR, DIMM_NP, TMG_1N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   1,  DDR4400_FREQUENCY, DIMM_DR, DIMM_NP, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   1,  DDR4800_FREQUENCY, DIMM_DR, DIMM_NP, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_120, D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },

    {   2,  DDR2000_FREQUENCY, DIMM_NP, DIMM_SR, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   2,  DDR3200_FREQUENCY, DIMM_NP, DIMM_SR, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   2,  DDR4800_FREQUENCY, DIMM_NP, DIMM_SR, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_DIS },
    {   2,  DDR2000_FREQUENCY, DIMM_NP, DIMM_DR, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   2,  DDR3200_FREQUENCY, DIMM_NP, DIMM_DR, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_DIS },
	{   2,  DDR4400_FREQUENCY, DIMM_NP, DIMM_DR, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_120, D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_DIS },

    {   2,  DDR2000_FREQUENCY, DIMM_SR, DIMM_NP, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   2,  DDR3200_FREQUENCY, DIMM_SR, DIMM_NP, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   2,  DDR4800_FREQUENCY, DIMM_SR, DIMM_NP, TMG_2N, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_DIS },
    {   2,  DDR2000_FREQUENCY, DIMM_DR, DIMM_NP, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_40, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_DIS, DFE_DIS },
    {   2,  DDR3200_FREQUENCY, DIMM_DR, DIMM_NP, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_DIS },
	{   2,  DDR4400_FREQUENCY, DIMM_DR, DIMM_NP, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_120, D5_IMP_120, D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x63, 0x3F, 0x38, 0x38, DFE_EN,  DFE_DIS },

    {   2,  DDR2000_FREQUENCY, DIMM_SR, DIMM_SR, TMG_2N, D5_IMP_80,  D5_IMP_80,  D5_IMP_80,  D5_IMP_80,  D5_IMP_80,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x69, 0x27, 0x2D, 0x2D, DFE_DIS, DFE_DIS },
	{   2,  DDR3200_FREQUENCY, DIMM_SR, DIMM_SR, TMG_2N, D5_IMP_40,  D5_IMP_40,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x69, 0x27, 0x2A, 0x2D, DFE_DIS, DFE_DIS },
	{   2,  DDR4400_FREQUENCY, DIMM_SR, DIMM_SR, TMG_2N, D5_IMP_34,  D5_IMP_34,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x69, 0x27, 0x2A, 0x2D, DFE_EN,  DFE_EN  },
    {   2,  DDR2000_FREQUENCY, DIMM_DR, DIMM_SR, TMG_2N, D5_IMP_60,  D5_IMP_80,  D5_IMP_80,  D5_IMP_240, D5_IMP_60,  D5_IMP_34, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_40, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x6F, 0x3F, 0x28, 0x28, DFE_DIS, DFE_DIS },
    {   2,  DDR4000_FREQUENCY, DIMM_SR, DIMM_DR, TMG_2N, D5_IMP_60,  D5_IMP_80,  D5_IMP_80,  D5_IMP_240, D5_IMP_60,  D5_IMP_34, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_40, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x6F, 0x3F, 0x28, 0x28, DFE_EN,  DFE_EN  },
    {   2,  DDR2000_FREQUENCY, DIMM_DR, DIMM_DR, TMG_2N, D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_60,  D5_IMP_40, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_60, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x6F, 0x27, 0x2D, 0x2D, DFE_DIS, DFE_DIS },
    {   2,  DDR3200_FREQUENCY, DIMM_DR, DIMM_DR, TMG_2N, D5_IMP_40,  D5_IMP_40,  D5_IMP_80,  D5_IMP_120, D5_IMP_60,  D5_IMP_34, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_40, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x6F, 0x3F, 0x28, 0x28, DFE_EN,  DFE_EN  },
	{   2,  DDR4000_FREQUENCY, DIMM_DR, DIMM_DR, TMG_2N, D5_IMP_60,  D5_IMP_80,  D5_IMP_80,  D5_IMP_240, D5_IMP_60,  D5_IMP_34, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_OFF, D5_IMP_40, D5_IMP_40, D5_IMP_60, D5_IMP_40, D5_IMP_34, D5_IMP_60, D5_IMP_30, 0x6F, 0x3F, 0x28, 0x28, DFE_EN,  DFE_EN  },
    
	};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

