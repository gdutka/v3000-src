/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file
 *
 * Enables DMAR mitigation during POST if called
 *
 */
#ifndef _GNBIOMMUDMARLIB_H_
#define _GNBIOMMUDMARLIB_H_


/**
  Enables DMAr mitigation using IOMMU and installs the relevant PPI or Protocol


  @param GnbHandle First GnbHandle Entry
**/
VOID
SetupAmdIommuDmar (
  IN GNB_HANDLE          *GnbHandle
  );


#endif /* _GNBIOMMUDMARLIB_H_s */
