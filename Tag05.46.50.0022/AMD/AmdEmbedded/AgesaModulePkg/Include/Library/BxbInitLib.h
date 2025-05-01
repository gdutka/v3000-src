/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU services
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */
#ifndef _BXBSMUV11LIB_H_
#define _BXBSMUV11LIB_H_

VOID
BixbyComplexInit (
  IN       PCIe_PLATFORM_CONFIG *Pcie
  );

VOID
BixbySpeedConfig (
  IN       PCI_ADDR       EarlyTrainPort
  );

BOOLEAN
BixbyFindEarlyLink (
  IN       DXIO_COMPLEX_DESCRIPTOR   *PcieTopologyData,
  IN       GNB_HANDLE                *GnbHandle
  );


#endif

