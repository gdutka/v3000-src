/*****************************************************************************
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB function to install NBIO services protocol
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision:
 *
 */

#ifndef _NBIO_SERVICES_LIB_DXE_H_
#define _NBIO_SERVICES_LIB_DXE_H_

EFI_STATUS
EFIAPI
NbioServiceProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

#endif //_NBIO_SERVICES_LIB_DXE_H_



