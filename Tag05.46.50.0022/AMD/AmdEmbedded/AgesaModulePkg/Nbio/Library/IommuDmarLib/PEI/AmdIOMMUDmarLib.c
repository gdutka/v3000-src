/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "AmdNbioIOMMU.h"
#include <Library/PeiServicesTablePointerLib.h>

#define FILECODE   NBIO_LIBRARY_IOMMUDMARLIB_PEI_AMDIOMMUDMARLIB_FILECODE

/**
  Enables DMAr mitigation using IOMMU in PEI

  @param GnbHandle First GnbHandle Entry
**/
VOID
SetupAmdIommuDmar (
  IN GNB_HANDLE          *GnbHandle
  )
{
  CONST EFI_PEI_SERVICES            **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  // Now attempt to setup IOMMU. If it goes well install the ppi
  if (SetupIommuOnEachNbio (GnbHandle)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a IOMMU Has been setup and enabled\n", __FUNCTION__);
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a Has failed\n", __FUNCTION__);
    return;
  }

  // Setup IOMMU ppi
  if (IommuSetupPpi (PeiServices) == EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a IOMMU protocol has been installed for usage by PCI IO\n", __FUNCTION__);
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a Has failed\n", __FUNCTION__);
    return;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);

}
