/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCX Set MCA Library - Zen3
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 *
 */
/*++
Module Name:

  CcxSetMcaZen3Lib.c
  Set MCA registers

Abstract:
--*/

#include <AGESA.h>
#include <Library/BaseLib.h>
#include <CcxRegistersZen3.h>
#include <Filecode.h>

#define FILECODE LIBRARY_CCXSETMCAZEN3LIB_CCXSETMCAZEN3LIB_FILECODE

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxSetMca
 *
 *  Description:
 *    This routine sets all MCA registers
 *
 */
VOID
CcxSetMca (
  )
{
  UINT8           McaIndex;
  UINT8           McaCount;
  UINT32          McaCtlMaskAddr;
  MCA_CONFIG_MSR  McaCfg;
  MCA_CTL_MASK_CS_STRUCT   McaCtlMsk;
  MCA_CTL_MASK_L3_STRUCT   McaCtlMskL3;
  MCA_IPID_LS_STRUCT       McaIpId;

  // MSR_0000_0179[7:0][Count]
  McaCount = (AsmReadMsr64 (0x00000179) & 0xFF);
  for (McaIndex = 0; McaIndex < McaCount; McaIndex++) {
    McaCfg.Value = AsmReadMsr64 (MSR_MCA_CFG_BANK0 + 0x10 * McaIndex);

    // MCi_CONFIG[32][McaXEnable] = 1 if MGC_CAP[Count] <= 32, else 0
    McaCfg.Field.McaXEnable = (McaCount <= 32) ? 1 : 0;

    // MCi_CONFIG[33][TransparentErrorLoggingEnable] = 0 if supported
    if (McaCfg.Field.TransparentErrorLoggingSupported == 1) {
      McaCfg.Field.TransparentErrorLoggingEnable = 0;
    }

    // MCi_CONFIG[34][LogDeferredInMcaStat] = 1 if supported
    if (McaCfg.Field.DeferredErrorLoggingSupported == 1) {
      McaCfg.Field.LogDeferredInMcaStat = 1;
    }
    AsmWriteMsr64 ((MSR_MCA_CFG_BANK0 + 0x10 * McaIndex), McaCfg.Value);

    // MSR read to MCA_IPID (MSRC000_2005 + (0x10 (bank size) * bank number))
    McaIpId.Value = AsmReadMsr64 (MCA_IPID_LS_ADDRESS + (0x10 * McaIndex));
    // This is a CS bank
    if ((McaIpId.Field.HardwareID == 0x2E) && (McaIpId.Field.McaType == 0x2)) {
      // Program the MCA_CTL_MASK_CS register
      McaCtlMaskAddr = MCA_CTL_MASK_LS_ADDRESS + McaIndex;
      McaCtlMsk.Value = AsmReadMsr64 (McaCtlMaskAddr);
      McaCtlMsk.Field.FTI_ADDR_VIOL = 1;
      AsmWriteMsr64 (McaCtlMaskAddr, McaCtlMsk.Value);
    } else if ((McaIpId.Field.HardwareID == 0xB0) && (McaIpId.Field.McaType == 0x7)) { // This is a L3 bank
      // Program the MCA_CTL_MASK_L3 register
      McaCtlMaskAddr = MCA_CTL_MASK_LS_ADDRESS + McaIndex;
      McaCtlMskL3.Value = AsmReadMsr64 (McaCtlMaskAddr);
      McaCtlMskL3.Field.Hwa = 1;
      AsmWriteMsr64 (McaCtlMaskAddr, McaCtlMskL3.Value);
    }
  }

}

