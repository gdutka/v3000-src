/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Generate Notify CPU method and GPE LXX method.
 *
 * Contains implementation of the interfaces: Generate Notify CPU method and
 * GPE LXX method in AmdAcpiNotifyCpu.h. *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Universal
 * @e \$Revision$   @e \$Date$
 *
 */


#ifndef _AMD_ACPI_NOTIFY_CPU_H_
#define _AMD_ACPI_NOTIFY_CPU_H_
#include "Protocol/AcpiTable.h"
/*----------------------------------------------------------------------------------------
 *                    T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
*/
AGESA_STATUS
AmdCreateNotifyCpuMethod (
  IN OUT   VOID                   **SsdtPtr,
  IN       UINT32                   CoreCount,
  IN OUT   UINT32                  *GpeScopeSize
  );

AGESA_STATUS
AmdCreateGpe (
  IN OUT   VOID                   **SsdtPtr,
  IN OUT   UINT32                  *GpeScopeSize
  );

#endif // _AMD_ACPI_NOTIFY_CPU_H_
