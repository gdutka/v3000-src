/**
 *  @file AmdCxlServicesProtocol.h
 *  @brief CXL Configuration Services Protocol prototype definition
 */
/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef __NBIO_CXL_SERVICES_PROTOCOL_H__
#define __NBIO_CXL_SERVICES_PROTOCOL_H__

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
// Current PROTOCOL revision
#define AMD_NBIO_CXL_SERVICES_REVISION    0x00

// TXCXL_REQATTR_MEMTYPE
#define AMD_CXL_MEMTYPE_DEVICE_NRNE    0
#define AMD_CXL_MEMTYPE_DEVICE_NRE     1
#define AMD_CXL_MEMTYPE_DEVICE_RE      2
#define AMD_CXL_MEMTYPE_NON_CACHEABLE  3
#define AMD_CXL_MEMTYPE_WB_NA          4
#define AMD_CXL_MEMTYPE_WB_A           5

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
/// Forward declaration for the AMD_NBIO_CXL_SERVICES_PROTOCOL
typedef struct _AMD_NBIO_CXL_SERVICES_PROTOCOL AMD_NBIO_CXL_SERVICES_PROTOCOL;

extern EFI_GUID gAmdNbioCxlServicesProtocolGuid;          ///< cxl services protocol calling

#pragma pack (push, 1)
/// Port Configuration Structure
typedef struct _AMD_CXL_PORT_CONFIG_STRUCT {
  UINT16    VendorID;                  ///< Value to be populated to PCIE_TX_CXL_CNTL1.PCIE_TX_CXL_CNTL1_TXCXL_VENDOR_ID
  UINT8     InterleaveMap;             ///< Bits7:4 = NBIO IDs (Bit7..4 = NBIO3..0), Bit3:0 = Ports interleaved per NBIO (0, 2, or 4).
  UINT8     IntlvSize;                 ///< see DF_MEM_INTLV_SIZE_VALIDVAL
  UINT64    MemPoolBase;               ///< Base address of memory pool assigned to this port
  UINT64    MemPoolSize;               ///< Size of memory pool assigned to this port
} AMD_CXL_PORT_CONFIG_STRUCT;

/// Port Information Structure
typedef struct _AMD_CXL_PORT_INFO_STRUCT {
  PCI_ADDR  EndPointBDF;                ///< Bus/Device/Function of Root Port in PCI_ADDR format
  UINT8     LogicalNbioInstance;        ///< Logical Instance ID of NBIO
  UINT8     PhysicalNbioInstance;       ///< Physical Instance ID of NBIO where this port is located
  UINT8     SocketID;                   ///< Socket ID for this port
  UINT8     MaxMemPools;                ///< Max number of memory pools supported by this port
  UINT32    UsRcrb;                     ///< Upstream Port RCRB address
  UINT32    DsRcrb;                     ///< Downstream Port RCRB address
  UINT32    UsMemBar0;                  ///< Upstream port MEMBAR0
  UINT32    DsMemBar0;                  ///< Downstream port MEMBAR0
} AMD_CXL_PORT_INFO_STRUCT;
#pragma pack (pop)

// Protocol Definitions
/**
  This function gets information about a specific PCIe root port.

  This
    A pointer to the AMD_NBIO_CXL_SERVICES_PROTOCOL instance.
  EndpointBDF
    Bus/Device/Function of Endpoint in PCI_ADDR format.
  PortInformation
    A pointer to an information structure to be populated by this function to
    identify the location of the CXL port.
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_GET_ROOT_PORT_INFORMATION) (
  IN  AMD_NBIO_CXL_SERVICES_PROTOCOL  *This,                ///< ptr
  IN  UINTN                           PortIndex,            ///< port index
  OUT AMD_CXL_PORT_INFO_STRUCT        *PortInformation      ///< port information ptr
);

/**
  This function configures a specific PCIe root port for CXL capabilities.

  This
    A pointer to the AMD_NBIO_CXL_SERVICES_PROTOCOL instance.
  EndpointBDF
    Bus/Device/Function of Endpoint in PCI_ADDR format.
  PortConfiguration
    A pointer to a configuration structure that contains the information necessary
    to configurare the CXL port.
  PortInformation OPTIONAL (can be NULL)
    A pointer to an information structure to be populated by this function to
    identify the location of the CXL port.
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_CONFIGURE_ROOT_PORT) (
  IN  AMD_NBIO_CXL_SERVICES_PROTOCOL  *This,          ///< this ptr
  IN  PCI_ADDR                        EndpointBDF     ///< end pt bdf
);

/**
  This function configures a specific PCIe root port for CXL presence

  This
    A pointer to the AMD_NBIO_CXL_SERVICES_PROTOCOL instance.
  EndpointBDF
    Bus/Device/Function of Endpoint in PCI_ADDR format.
  PortConfiguration
    A pointer to a configuration structure that contains the information necessary
    to configurare the CXL port such as Socket id,rbindex,port id,segment,bus base,limit or presence.
**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_PORT_PRESENCE_INFO) (                        ///< cxl port presence info
  IN  AMD_NBIO_CXL_SERVICES_PROTOCOL  *This,                  ///<
  IN  UINT8                           SocketId,                  ///<
  IN  UINT8                           RbIndex,                  ///<
  IN  UINT8                           PortId,                  ///<
  OUT UINT8                           *Segment,                  ///<
  OUT UINT8                           *BusBase,                  ///<
  OUT UINT8                           *BusLimit,                  ///<
  OUT BOOLEAN                         *Presence                  ///<
);

typedef
EFI_STATUS
(EFIAPI *AMD_CXL_GET_PORT_RB_LOCATION) (                      ///< get port rb location
  IN  AMD_NBIO_CXL_SERVICES_PROTOCOL  *This,                  ///<
  IN  UINT8                           Segment,                  ///<
  IN  UINT8                           BusBase,                  ///<
  OUT UINT8                           *SocketId,                  ///<
  OUT UINT8                           *RbIndex                  ///<
  );

/// The Protocol Definition for CXL Services
struct _AMD_NBIO_CXL_SERVICES_PROTOCOL {
  UINT32                              Revision;                             ///< revision
  AMD_CXL_GET_ROOT_PORT_INFORMATION   CxlGetRootPortInformation;            ///< cxl root port information
  AMD_CXL_CONFIGURE_ROOT_PORT         CxlConfigureRootPort;                 ///< configuring the root port
  AMD_CXL_PORT_PRESENCE_INFO          GetCxlPortPresenceInfo;               ///< cxl port presence information
  AMD_CXL_GET_PORT_RB_LOCATION        GetCxlPortRBLocation;                 ///< CXL port RB location
};

#endif


