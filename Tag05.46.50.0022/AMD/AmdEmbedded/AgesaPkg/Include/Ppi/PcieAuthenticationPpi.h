/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _AMD_PCIE_AUTHENTICATION_PPI_H_
#define _AMD_PCIE_AUTHENTICATION_PPI_H_

// Current PPI revision
#define AMD_PCIE_AUTHENTICATION_REVISION   0x01

typedef struct _PEI_AMD_PCIE_AUTHENTICATION_PPI PEI_AMD_PCIE_AUTHENTICATION_PPI;

typedef union {
  struct {
    UINT32  Function : 3;
    UINT32  Device : 5;
    UINT32  Bus : 8;
    UINT32  Segment : 16;
  } AsBits;
  UINT32  AsUint32;
} AMD_PCI_LOCATION;

/// PORT_AUTHENTICATION_DESC Structure definition
/**
 * @brief    PORT_AUTHENTICATION_DESC Structure definition
 *
 * @details  This data structure defines the information provided to the PCIE_AMD_AUTHENTICATION_PPI to identify
 *   the location of the device or slot to be authenticated.
 *
 */

typedef struct {
  IN AMD_PCI_LOCATION       PciLocation;          ///< Identifies the bus/device/function of the root port associated with this device
  IN UINT16                 StartLaneIndex;       ///< The starting logical lane number of the device (as assigned in the topology structure)
  IN UINT16                 EndLaneIndex;         ///< The ending logical lane number of the device (as assigned in the topology structure)
} PORT_AUTHENTICATION_DESC;

//
// PPI prototype
//
/**
 * @brief Callout from AGESA to allow platform authentication of PCIe device before training
 *
 * @param PortDescriptor  Pointer a structure that identifies the root port to be authenticated
 * @param Authenticated   Pointer to a BOOLEAN result of authentication
 *
 * @retval EFI_STATUS     EFI_SUCCESS - Port identified and "Authenticated" populated
 *                        EFI_NOT_FOUND - Port not found or authentication not supported
 */
typedef
EFI_STATUS
(EFIAPI * PCIE_BEFORE_TRAINING_AUTHENTICATION) (
  IN     PORT_AUTHENTICATION_DESC   *PortDescriptor,
  IN OUT BOOLEAN                    *Authenticated
);

//
// PPI prototype
//
/**
 * @brief Callout from AGESA to allow platform authentication of PCIe device with active link (trained)
 *
 * @param PortDescriptor  Pointer a structure that identifies the root port to be authenticated
 * @param Authenticated   Pointer to a BOOLEAN result of authentication
 *
 * @retval EFI_STATUS     EFI_SUCCESS - Port identified and "Authenticated" populated
 *                        EFI_NOT_FOUND - Port not found or authentication not supported
 */
typedef
EFI_STATUS
(EFIAPI * PCIE_AFTER_TRAINNIG_AUTHENTICATION) (
  IN     PORT_AUTHENTICATION_DESC   *PortDescriptor,
  IN OUT BOOLEAN                    *Authenticated
);


///
///< PPI to support PCIe device authentication
///
typedef struct _PEI_AMD_PCIE_AUTHENTICATION_PPI {
  PCIE_BEFORE_TRAINING_AUTHENTICATION   PcieAuthenticationBeforeTraining;
  PCIE_AFTER_TRAINNIG_AUTHENTICATION    PcieAuthenticationAfterTraining;
} PEI_AMD_PCIE_AUTHENTICATION_PPI;

extern EFI_GUID gAmdPcieAuthenticationGuid;

#endif //


