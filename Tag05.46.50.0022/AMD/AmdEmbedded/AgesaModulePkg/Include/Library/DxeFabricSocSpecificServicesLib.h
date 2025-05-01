/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Fabric SOC specific services definitions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _DXE_FABRIC_SOC_SPECIFIC_SERVICES_LIB_H_
#define _DXE_FABRIC_SOC_SPECIFIC_SERVICES_LIB_H_

EFI_STATUS
EFIAPI
FabricSocSpecificServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

#endif


