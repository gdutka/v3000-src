/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB function to GetHostPciAddress and GetHandle.
 *
 * Contain code that create/locate and rebase configuration data area.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision:
 *
 */

#ifndef _NBIOHANDLELIB_LIB_H_
#define _NBIOHANDLELIB_LIB_H_

#include <GnbDxio.h>

/*----------------------------------------------------------------------------------------*/
/**
 * Get GNB handle
 *
 *
 * @param[in]       Pcie            Pointer to global PCIe configuration
 */
GNB_HANDLE *
NbioGetHandle (
  IN PCIe_PLATFORM_CONFIG     *Pcie
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Get GNB handle of next socket
 *
 *
 * @param[in]       NbioHandle        Pointer to current GnbHandle
 */
GNB_HANDLE *
NbioGetAltHandle (
  IN PCIe_PLATFORM_CONFIG            *Pcie
  );
GNB_HANDLE *
NbioGetNextSocket (
  IN GNB_HANDLE            *NbioHandle
  );

/*----------------------------------------------------------------------------------------*/
/*
 * Get PCI_ADDR of GNB
 *
 *
 * @param[in]  Handle           Pointer to GNB_HANDLE
 */
PCI_ADDR
NbioGetHostPciAddress (
  IN      GNB_HANDLE          *Handle
  );

#define GnbGetNextHandle(Descriptor) (GNB_HANDLE *) PcieConfigGetNextTopologyDescriptor (Descriptor, DESCRIPTOR_TERMINATE_TOPOLOGY)

#define GnbGetSocketId(Handle) (Handle != NULL ? (Handle)->SocketId : 0)
#define GnbGetDieNumber(Handle) (Handle != NULL ? (Handle)->DieNumber : 0)
#define GnbGetRBIndex(Handle) (Handle != NULL ? (Handle)->RBIndex : 0)



#endif



