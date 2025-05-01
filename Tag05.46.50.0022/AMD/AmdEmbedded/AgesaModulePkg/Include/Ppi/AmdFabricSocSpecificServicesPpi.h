/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _AMD_FABRIC_SOC_SPECIFIC_SERVICES_PPI_H_
#define _AMD_FABRIC_SOC_SPECIFIC_SERVICES_PPI_H_

//----------------------------------------------------
//
//  AMD Fabric SoC specific services PPI
//
//-------------------------------------------------------

/// Fabric SoC specific services PPI
typedef struct _PEI_AMD_FABRIC_SOC_SPECIFIC_SERVICES_PPI {
  UINTN    Revision;                   ///< Revision Number
  BOOLEAN  HasCcix;                    ///< If DF has CCIX
  UINTN    *DfGlblCtrlInstanceIds;     ///< D18F0x3F8 Global Control register instances
  UINT64   *DramScrubRateEncodings;    ///< Dram scrub rate (D18F6x44)
} PEI_AMD_FABRIC_SOC_SPECIFIC_SERVICES_PPI;


extern EFI_GUID gAmdFabricSocSpecificServicesPpiGuid;

#endif  //_AMD_FABRIC_SOC_SPECIFIC_SERVICES_PPI_H_
