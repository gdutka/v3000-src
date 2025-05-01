/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * IOMMU IVRS Table Creation
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <AGESA.h>
#include  <Gnb.h>
#include  <GnbDxio.h>
#include  <GnbIommu.h>
#include  <Library/AmdBaseLib.h>


#ifndef _IVRS_LIB_V2_H_
#define _IVRS_LIB_V2_H_

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

#define IVRS_TABLE_LENGTH 8 * 1024

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GnbCreateIvrsEntry (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       IVRS_BLOCK_TYPE            Type,
  IN       VOID                       *Ivrs
  );

AGESA_STATUS
GnbBuildIvmdList (
  IN       IVRS_BLOCK_TYPE            Type,
  IN       VOID                       *Ivrs
  );

VOID
GnbIommuIvrsTableDump (
  IN       VOID                       *Ivrs
  );

VOID
GnbCreateIvhd (
  IN       GNB_HANDLE                 *GnbHandle,
     OUT   IVRS_IVHD_ENTRY            *Ivhd
  );

VOID
NbioIvhdAddApicEntry (
  IN       GNB_HANDLE                 *GnbHandle,
     OUT   IVRS_IVHD_ENTRY            *Ivhd
  );

VOID
NbioIvhdAddF0DeviceEntries (
  IN       IVRS_IVHD_ENTRY            *Ivhd
  );

VOID
NbioIvhdAddDeviceRangeEntry (
  IN       PCI_ADDR             StartRange,
  IN       PCI_ADDR             EndRange,
  IN       UINT8                DataSetting,
  IN       IVRS_IVHD_ENTRY      *Ivhd
  );

VOID
NbioIvhdAddDeviceAliasRangeEntry (
  IN       PCI_ADDR             StartRange,
  IN       PCI_ADDR             EndRange,
  IN       PCI_ADDR             Alias,
  IN       UINT8                DataSetting,
  IN       IVRS_IVHD_ENTRY      *Ivhd
  );

VOID
NbioIvhdAddSpecialDeviceEntry (
  IN       IVHD_SPECIAL_DEVICE  SpecialDevice,
  IN       PCI_ADDR             Device,
  IN       UINT8                Id,
  IN       UINT8                DataSetting,
  IN       IVRS_IVHD_ENTRY      *Ivhd
  );

VOID
SbCreateIvhdEntries (
     OUT   IVRS_IVHD_ENTRY            *Ivhd
  );


#endif

