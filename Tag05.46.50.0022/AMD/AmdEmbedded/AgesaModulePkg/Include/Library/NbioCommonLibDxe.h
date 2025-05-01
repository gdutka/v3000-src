/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific PCIe definitions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2015-02-04 00:39:01 -0600 (Wed, 04 Feb 2015) $
 *
 */
#ifndef _NBIOCOMMONLIBDXE_H_
#define _NBIOCOMMONLIBDXE_H_


// Functions called from NbioCommonLib
EFI_STATUS
PcieGetLogicalId (
     OUT   SOC_LOGICAL_ID           *LogicalId
  );

EFI_STATUS
PcieGetPcieDxe (
     OUT     PCIe_PLATFORM_CONFIG          **Pcie
  );


#endif


