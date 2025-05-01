/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef _AMD_NBIO_PCIE_COMPLEX_PPI_H_
#define _AMD_NBIO_PCIE_COMPLEX_PPI_H_

// Current PPI revision
#define AMD_NBIO_PCIE_COMPLEX_REVISION   0x01
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct _PEI_AMD_NBIO_PCIE_COMPLEX_PPI PEI_AMD_NBIO_PCIE_COMPLEX_PPI;

/**
 * @brief PPI Protocol 
 * @details Returns the PCIe complex structure
 */
typedef
EFI_STATUS
(EFIAPI * AMD_NBIO_PCIE_GET_COMPLEX_STRUCT) (
  IN     PEI_AMD_NBIO_PCIE_COMPLEX_PPI   *This,       /***<A pointer to the _PEI_AMD_NBIO_PCIE_COMPLEX_PPI instance.*/
     OUT DXIO_COMPLEX_DESCRIPTOR         **UserConfig /***<A pointer to a pointer to store the address of
                                                      *the Platform PCIe Complex Structure*/
);


///< The Ppi to return PCIe Complex Descriptor
typedef struct _PEI_AMD_NBIO_PCIE_COMPLEX_PPI {
  UINT32 Revision;         ///< PPI revision
  AMD_NBIO_PCIE_GET_COMPLEX_STRUCT     PcieGetComplex;      ///< pcie complex
} PEI_AMD_NBIO_PCIE_COMPLEX_PPI;

extern EFI_GUID gAmdNbioPcieComplexPpiGuid;

#endif //


