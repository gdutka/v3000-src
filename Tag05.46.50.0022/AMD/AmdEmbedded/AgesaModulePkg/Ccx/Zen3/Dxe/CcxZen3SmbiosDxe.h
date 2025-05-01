/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU SMBIOS functions.
 *
 * Contains code for collecting SMBIOS information
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Ccx
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _CCX_ZEN3_SMBIOS_DXE_H_
#define _CCX_ZEN3_SMBIOS_DXE_H_

EFI_STATUS
EFIAPI
CcxZen3SmbiosServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _CCX_ZEN3_SMBIOS_DXE_H_


