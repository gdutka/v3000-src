/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef _AMD_PCI_BUS_CONFIGURED_PPI_H_
#define _AMD_PCI_BUS_CONFIGURED_PPI_H_

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
//  Amd PCI Bus Configured PPI
typedef struct _PEI_AMD_PCI_BUS_CONFIGURED_PPI {
    UINT32  Size;       ///< size
    UINT32  Configured; ///< configuration
  } PEI_AMD_PCI_BUS_CONFIGURED_PPI;

// Current PPI revision
#define AMD_PCI_BUS_CONFIGURED_REVISION   0x01    ///< revision

extern EFI_GUID gAmdPciBusConfiguredPpiGuid;

#endif  /* _AMD_PCI_BUS_CONFIGUIRED_PPI_H_ */


