/** @file

Cxl Manager Protocol definition.

**/
/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

#ifndef __CXL_MANAGER_PROTOCOL_H__
#define __CXL_MANAGER_PROTOCOL_H__

#include <PiDxe.h>      ///< EFI_GCD_MEMORY_TYPE

#define AMD_CXL_MANAGER_PROTOCOL_GUID \
  {0x3926B776, 0xF572, 0x4659, {0x82, 0xD2, 0xED, 0xB1, 0xFA, 0x32, 0x3E, 0xF0}}

typedef struct _AMD_CXL_MANAGER_PROTOCOL AMD_CXL_MANAGER_PROTOCOL;

extern EFI_GUID gAmdCxlManagerProtocolGuid;

// Current PROTOCOL revision
#define AMD_CXL_MANAGER_PROTOCOL_REVISION    0x00UL

//
// Per CXL Spec
//
#define MAX_CXL_DEVICE_ID               255             // DeviceID: 1..255
#define MAX_CXL_MEM_POOLS_PER_ENDPOINT  2              // Max of 2 MemPools per endpoint


//*******************************************************
// Cxl Memory Flags Definitions
//*******************************************************
// These bit-flags can be "ORed" together as needed.
//
#define AMD_CXL_MEMORY_BDS_REGISTERED    0x00000001UL  // Registered with Cxl Manager
#define AMD_CXL_MEMORY_ABL_MAPPED        0x00000002UL  // Mapped by ABL

#pragma pack (push, 1)
typedef union {
  struct {
    UINT32  Function : 3;
    UINT32  Device : 5;
    UINT32  Bus : 8;
    UINT32  Segment : 16;
  } AsBits;

  UINT32  AsUint32;
} AMD_PCI_LOCATION;

typedef union {
  struct {
    AMD_PCI_LOCATION  PciLocation;
    UINT8             NbioPhysId;
    UINT8             SocketId;
    UINT16            Reserved;
  } AsStruct;

  UINT64  AsUint64;
} AMD_CXL_ROOT_PORT_INFO;

typedef struct {
  UINT64                   Base;          ///< Base address of memory range
  UINT64                   Size;          ///< Size of memory range
  UINT64                   Alignment;     ///< Alignment requirement for Base address
  UINT32                   EfiType;       ///< EFI_MEMORY_TYPE of memory range
  UINT32                   Flags;         ///< Flags for state of memory pool
  AMD_CXL_ROOT_PORT_INFO   RootPortInfo;  ///< Information provided by NBIO Driver
} AMD_CXL_MEMORY_POOL;
#pragma pack (pop)

//
// Protocol functions
//
/**
  Check if device location is CXL device.

  @param[in]   This            Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]  EndpointLocation CXL DeviceID (1..255).

  @retval EFI_SUCCESS     The device is a CXL device.
  @retval EFI_NO_FOUND    The device is NOT a CXL device

**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_IS_CXL_DEVICE) (
  IN  CONST AMD_CXL_MANAGER_PROTOCOL    *This,
  IN  AMD_PCI_LOCATION                  EndpointLocation
  );

/**
  Get RootBridge Information.

  @param[in]   This                Pointer to MD_CXL_MANAGER_PROTOCOL instance.
  @param[in]   EndpointLocation    Endpoint Location structure.
  @param[out]  RootBridgeInfo      Pointer to AMD_CXL_ROOT_PORT_INFO structure.

  @retval EFI_SUCCESS              The function completed successfully.
  @retval Other                    The requested operation could not be completed.

**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_GET_HOST_BRIDGE_INFO) (
  IN  AMD_CXL_MANAGER_PROTOCOL  *This,
  IN  AMD_PCI_LOCATION          EndpointLocation,
  OUT AMD_CXL_ROOT_PORT_INFO    *RootBridgeInfo
  );

/**
  Register a CXL memory pool.

  @param[in]       This               Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in, out]  MemPoolToRegister  Pointer to CXL memory pool.

  @retval EFI_SUCCESS     The function completed successfully.
  @retval Other           The requested operation could not be completed.

**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_REGISTER_MEMORY_POOL) (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN OUT AMD_CXL_MEMORY_POOL          *MemPoolToRegister
  );

/**
  Enable a CXL memory pool.
  By the time this function is called all memory pools should have been registered.

  @param[in]  This              Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]  EndpointLocation  Endpoint Location structure.
  @param[in]  TrafficClass      Traffic Class for CXL Virtual Channel.
  @param[in]  CxlVendorId       CXL Vendor ID in Protocol Layer DVSEC.
  @param[in]  MemPool           Pointer to CXL memory pool.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval Other                 The requested operation could not be completed.

**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_ENABLE_MEMORY_POOL) (
  IN CONST AMD_CXL_MANAGER_PROTOCOL    *This,
  IN AMD_PCI_LOCATION                  EndpointLocation,
  IN AMD_CXL_MEMORY_POOL               *MemPool
  );

/**
  Convert a CXL memory pool.

  @param[in]  This           Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]  Base           Base address of CXL memory pool.
  @param[in]  Size           Size of CXL memory pool.
  @param[in]  GcdMemoryType  GCD Memory Type for CXL memory pool.
  @param[in]  Attributes     Attribbutes for CXL memory pool.

  @retval EFI_SUCCESS        The function completed successfully.
  @retval Other              The requested operation could not be completed.

**/
typedef
EFI_STATUS
(EFIAPI *AMD_CXL_CONVERT_MEMORY_POOL) (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN UINT64                            Base,
  IN UINT64                            Size,
  IN EFI_GCD_MEMORY_TYPE               GcdmemoryType,
  IN UINT64                            Attributes
  );

/**
  Callback for ReadyToBoot event notification from Endpoint driver.

  @param[in]  This           Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.

**/
typedef
VOID
(EFIAPI *AMD_CXL_READY_TO_BOOT_NOTIFY) (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This
  );

//
//  Cxl Manager Protocol services.
//
struct _AMD_CXL_MANAGER_PROTOCOL {
  ///
  /// The revision to which this protocol interface adheres.
  ///  All future revisions must be backwards compatible.
  ///  If a future version is not backwards compatible, it is not the same GUID.
  ///
  UINT32                          Revision;
  ///
  /// The functions supported by this protocol.
  ///
  AMD_CXL_IS_CXL_DEVICE         IsCxlDevice;
  AMD_CXL_GET_HOST_BRIDGE_INFO  GetRootBridgeInfo;
  AMD_CXL_REGISTER_MEMORY_POOL  RegisterMemoryPool;
  AMD_CXL_ENABLE_MEMORY_POOL    EnableMemoryPool;
  AMD_CXL_CONVERT_MEMORY_POOL   ConvertMemoryPool;
  AMD_CXL_READY_TO_BOOT_NOTIFY  OnReadyToBootFromEndpoint;
};
#endif
