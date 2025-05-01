/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Memory API, and related functions.
 *
 * Contains code that initializes channel translation table
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
/*++
Module Name:

  MemChanXLatDummyPei.c
  Init Memory channel translation table

Abstract:
--*/

#include "PiPei.h"
#include <Ppi/AmdMemPpi.h>
#include <Library/MemChanXLat.h>
#include "Filecode.h"

#define FILECODE MEM_AMDMEMCHANXLATDUMMYPEI_MEMCHANXLATDUMMYPEI_FILECODE

extern EFI_GUID gAmdMemChanXLatPpiGuid;

//
// Create a Channel Mapping Transaltion Table per CPU Package
//
HOST_TO_APCB_CHANNEL_XLAT DummyChannelXlatTable[] = {
  // Requested   Translated
  { 0xFF,       0xFF},
};

/*++

Routine Description:

  Initialize the memory channel translation table.

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdMemChanXLatPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                            Status;
  AMD_MEMORY_CHANNEL_XLAT_PPI           *mMemChanXLatPpi;
  EFI_PEI_PPI_DESCRIPTOR                *mMemChanXLatPpiList;

  //----------------------------------------------------------------
  //
  //            Allocate space for PPI data structure
  //
  //----------------------------------------------------------------
  Status = (*PeiServices)->AllocatePool (
                           PeiServices,
                           sizeof (AMD_MEMORY_CHANNEL_XLAT_PPI),
                           (VOID **) &mMemChanXLatPpi
                           );
  if (EFI_SUCCESS != Status) {
    return Status;
  }

  Status = (*PeiServices)->AllocatePool (
                           PeiServices,
                           sizeof (EFI_PEI_PPI_DESCRIPTOR),
                           (VOID **) &mMemChanXLatPpiList
                           );
  if (EFI_SUCCESS != Status) {
    return Status;
  }

  mMemChanXLatPpi->XLatTab = (VOID *)&DummyChannelXlatTable[0];
  mMemChanXLatPpi->Revision = AMD_MEMORY_CHANNEL_XLAT_REVISION;
  mMemChanXLatPpiList->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  mMemChanXLatPpiList->Guid  = &gAmdMemChanXLatPpiGuid;
  mMemChanXLatPpiList->Ppi   = mMemChanXLatPpi;

  Status = (**PeiServices).InstallPpi (PeiServices, mMemChanXLatPpiList);

  return (Status);
}


