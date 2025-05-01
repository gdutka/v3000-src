/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCIX Configuration Services Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Nbio
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 */

#ifndef __NBIO_CCIX_SERVICES_PROTOCOL_H__
#define __NBIO_CCIX_SERVICES_PROTOCOL_H__

// Current PROTOCOL revision
#define AMD_NBIO_CCIX_SERVICES_REVISION    0x00

// TXCCIX_REQATTR_MEMTYPE
#define AMD_CCIX_MEMTYPE_DEVICE_NRNE    0
#define AMD_CCIX_MEMTYPE_DEVICE_NRE     1
#define AMD_CCIX_MEMTYPE_DEVICE_RE      2
#define AMD_CCIX_MEMTYPE_NON_CACHEABLE  3
#define AMD_CCIX_MEMTYPE_WB_NA          4
#define AMD_CCIX_MEMTYPE_WB_A           5

///
/// Forward declaration for the AMD_NBIO_CCIX_SERVICES_PROTOCOL
///
typedef struct _AMD_NBIO_CCIX_SERVICES_PROTOCOL AMD_NBIO_CCIX_SERVICES_PROTOCOL;

extern EFI_GUID gAmdNbioCcixServicesProtocolGuid;

#pragma pack (push, 1)
/// Port Configuration Structure
typedef struct _AMD_PORT_CONFIGURATION_STRUCT {
  UINT16    VendorID;                  ///< Value to be populated to PCIE_TX_CCIX_CNTL1.PCIE_TX_CCIX_CNTL1_TXCCIX_VENDOR_ID
  UINT8     InterleaveMap;             ///< Bits7:4 = NBIO IDs (Bit7..4 = NBIO3..0), Bit3:0 = Ports interleaved per NBIO (0, 2, or 4).
  UINT8     IntlvSize;                 ///< see DF_MEM_INTLV_SIZE_VALIDVAL
  UINT8     MemType;                   ///< TXCCIX_REQATTR_MEMTYPE
  UINT8     TC;                        ///< TXCCIX_CCIX_TC
  UINT8     SourceID;                  ///< TXCCIX_SRC_ID
  UINT8     TargetID;                  ///< TXCCIX_TGT_ID
  UINT64    MemPoolBase;               ///< Base address of memory pool assigned to this port
  UINT64    MemPoolSize;               ///< Size of memory pool assigned to this port
} AMD_PORT_CONFIGURATION_STRUCT;

/// Port Information Structure
typedef struct _AMD_PORT_INFORMATION_STRUCT {
  PCI_ADDR  RootPortBDF;                ///< Bus/Device/Function of Root Port in PCI_ADDR format
  UINT8     LogicalNbioInstance;        ///< Logical Instance ID of NBIO
  UINT8     PhysicalNbioInstance;       ///< Physical Instance ID of NBIO where this port is located
  UINT8     MaxMemPools;                ///< Max number of memory pools supported by this port
  UINT8     SocketID;                   ///< Socket ID for this port
} AMD_PORT_INFORMATION_STRUCT;
#pragma pack (pop)

//
// Protocol Definitions
//
/**
  This function gets information about a specific PCIe root port.

  This
    A pointer to the AMD_NBIO_CCIX_SERVICES_PROTOCOL instance.
  EndpointBDF
    Bus/Device/Function of Endpoint in PCI_ADDR format.
  PortInformation
    A pointer to an information structure to be populated by this function to
    identify the location of the CCIX port.
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CCIX_GET_ROOT_PORT_INFORMATION) (
  IN  AMD_NBIO_CCIX_SERVICES_PROTOCOL  *This,
  IN  PCI_ADDR                         EndpointBDF,
  OUT AMD_PORT_INFORMATION_STRUCT      *PortInformation
);

/**
  This function configures a specific PCIe root port for CCIX capabilities.

  This
    A pointer to the AMD_NBIO_CCIX_SERVICES_PROTOCOL instance.
  EndpointBDF
    Bus/Device/Function of Endpoint in PCI_ADDR format.
  PortConfiguration
    A pointer to a configuration structure that contains the information necessary
    to configurare the CCIX port.
  PortInformation OPTIONAL (can be NULL)
    A pointer to an information structure to be populated by this function to
    identify the location of the CCIX port.
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CCIX_CONFIGURE_ROOT_PORT) (
  IN  AMD_NBIO_CCIX_SERVICES_PROTOCOL  *This,
  IN  PCI_ADDR                         EndpointBDF,
  IN  AMD_PORT_CONFIGURATION_STRUCT    *PortConfiguration,
  OUT AMD_PORT_INFORMATION_STRUCT      *PortInformation     // OPTIONAL
);

///
/// The Protocol Definition for CCIX Services
///
struct _AMD_NBIO_CCIX_SERVICES_PROTOCOL {
  UINT32                              Revision;
  AMD_CCIX_GET_ROOT_PORT_INFORMATION  CcixGetRootPortInformation;
  AMD_CCIX_CONFIGURE_ROOT_PORT        CcixConfigureRootPort;
};

#endif


