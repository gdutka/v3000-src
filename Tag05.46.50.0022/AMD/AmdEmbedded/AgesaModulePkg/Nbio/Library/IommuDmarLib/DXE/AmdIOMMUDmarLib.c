/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AmdNbioIOMMU.h"

#define FILECODE        NBIO_LIBRARY_IOMMUDMARLIB_DXE_AMDIOMMUDMARLIB_FILECODE

/**
  Enables DMAr mitigation using IOMMU in PEI


  @param GnbHandle First GnbHandle Entry
**/
VOID
SetupAmdIommuDmar (
  IN GNB_HANDLE          *GnbHandle
  )
{

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  // Now attempt to setup IOMMU. If it goes well install the protocol
  if (SetupIommuOnEachNbio (GnbHandle)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a IOMMU Has been setup and enabled\n", __FUNCTION__);
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a Has failed\n", __FUNCTION__);
    return;
  }
  // Setup IOMMU protocol
  if (IommuSetupProtocol () == EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a IOMMU protocol has been installed for usage by PCI IO and RB IO\n", __FUNCTION__);
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a Has failed\n", __FUNCTION__);
    return;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);

}
