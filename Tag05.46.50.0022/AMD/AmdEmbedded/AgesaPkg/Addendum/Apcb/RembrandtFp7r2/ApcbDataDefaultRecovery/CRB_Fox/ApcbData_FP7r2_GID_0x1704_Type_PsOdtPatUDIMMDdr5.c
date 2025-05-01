/*****************************************************************************
 * Copyright 2015 - 2021 Advanced Micro Devices, Inc. All rights reserved.
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
*****************************************************************************
*/

/* $NoKeywords:$ */
/**
 * @file
 *
 * @e \$Revision:$   @e \$Date:$
 */

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

#define DIMM_NP 1
#define DIMM_SR 2
#define DIMM_DR 4
#define DIMM_QR 8

typedef struct _PSCFG_DDR5_ODTPAT_ENTRY {
  UINT16      Dimm0;              ///< Bitmap of dimm0 rank type or dimm0 population of LRDIMM
  UINT16      Dimm1;              ///< Bitmap of dimm1 rank type or dimm1 population of LRDIMM
  UINT32      ODTPatCS0;          ///< ODT Pattern for CS0
  UINT32      ODTPatCS1;          ///< ODT Pattern for CS1
  UINT32      ODTPatCS2;          ///< ODT Pattern for CS2
  UINT32      ODTPatCS3;          ///< ODT Pattern for CS3
} PSCFG_DDR5_ODTPAT_ENTRY;

// UDIMM ODTPAT DDR4
APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_PS_UDIMM_DDR5_ODT_PAT,        // TypeId
  sizeof(ApcbTypeHeader),        // SizeOfType, will be fixed up by tool
  0,                             // InstanceId
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

PSCFG_DDR5_ODTPAT_ENTRY OdtPatEntry [] = {
  //                     CS0          CS1         CS2         CS3
  { DIMM_SR, DIMM_NP, 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
  { DIMM_DR, DIMM_NP, 0x00000202, 0x00000101, 0x00000000, 0x00000000 },
  { DIMM_NP, DIMM_SR, 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
  { DIMM_NP, DIMM_DR, 0x00000000, 0x00000000, 0x00000202, 0x00000101 },
  { DIMM_SR, DIMM_SR, 0x00000404, 0x00000000, 0x00000101, 0x00000000 },
  { DIMM_SR, DIMM_DR, 0x00000C0C, 0x00000000, 0x00000909, 0x00000505 },
  { DIMM_DR, DIMM_SR, 0x00000606, 0x00000505, 0x00000303, 0x00000000 },
  { DIMM_DR, DIMM_DR, 0x00000E0E, 0X00000D0D, 0X00000B0B, 0X00000707 },
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();




