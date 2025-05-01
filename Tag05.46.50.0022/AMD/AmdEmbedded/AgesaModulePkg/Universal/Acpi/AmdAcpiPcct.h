/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Generate Platform Communications Channel Table (PCCT).
 *
 * Contains implementation of the interfaces: Generate PCCT in AmdAcpiPcct.h. *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Universal
 * @e \$Revision$   @e \$Date$
 *
 */

 
#ifndef _AMD_ACPI_PCCT_H_
#define _AMD_ACPI_PCCT_H_
#include "Protocol/AcpiTable.h"
/*----------------------------------------------------------------------------------------
 *                    T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
AmdAcpiPcct (
  IN       EFI_ACPI_TABLE_PROTOCOL  *AcpiTableProtocol,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  );

#endif // _AMD_ACPI_PCCT_H_
