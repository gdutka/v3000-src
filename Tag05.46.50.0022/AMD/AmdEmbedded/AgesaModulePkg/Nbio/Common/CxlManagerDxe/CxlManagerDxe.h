/** @file

Cxl Manager DXE Driver.

**/
/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/
#ifndef __CXL_MANAGER_DXE_H__
#define __CXL_MANAGER_DXE_H__

#include <Uefi.h>
#include <IndustryStandard/Pci.h>

#include <AMD.h>
#include <Porting.h>
#include <Filecode.h>

#include <RS/APOB.h>
#include <RS/ApcbDataGroups.h>
#include <RS/ApcbV3TokenUid.h>
#include <RS/ApcbDfCxlConfig.h>
#include <RS/ApcbV3Priority.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
//#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

#include <Library/AmdCxlPcieLib.h>
#include <Library/AmdPspApobLib.h>

#include <Guid/MemoryTypeInformation.h>

#include <Protocol/PciIo.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/AmdCxlManagerProtocol.h>
#include <Protocol/AmdCxlServicesProtocol.h>
#include <Protocol/AmdApcbProtocol.h>

#define CXL_RootBridge_DEFAULT_PORT_ID  0
#define CXL_RootBridge_DEFAULT_LINK_ID  0

#define CXL_DEFAULT_DEVICE_ID            1
#define CXL_MIN_POWER_OF_TWO_ALIGNMENT   32    //< 2^32 = SIZE_4GB

#define MAX_NUMBER_OF_SOCKETS             2
#define MAX_NBIO_INSTANCES_PER_SOCKET     4
#define MAX_NBIO_VALID_INTERLEAVE_PAIRS   4

#define MAX_PORTS_PER_CXL_CONTROLLER     4
#define MAX_CXL_CONTROLLERS_PER_SOCKET   4
#define MAX_CXL_CONTROLLERS_PER_SYSTEM   (MAX_CXL_CONTROLLERS_PER_SOCKET * MAX_NUMBER_OF_SOCKETS)

#define MAX_CXL_ENDPOINTS                8

#define MAX_CXL_ROOT_BRIDGES            (MAX_CXL_CONTROLLERS_PER_SYSTEM * MAX_PORTS_PER_CXL_CONTROLLER)
#define MAX_ABL_MEMORY_RANGES             MAX_CXL_CONTROLLERS_PER_SYSTEM

#define IS_SUPPORTED_PORTCOUNT_FOR_PORT_INTERLEAVE(p)        (((p) == 2) || ((p) == 4))
#define IS_SUPPORTED_PORTCOUNT_FOR_CONTROLLER_INTERLEAVE(p)  (((p) == 1) || ((p) == 2) || ((p) == 4))

#define ABL_MIN_BASE_ADDRESS_ALIGNMENT    SIZE_256MB
#define ABL_INVALID_ADDRESS               0xFFFFFFFFFFFFFFFFULL

typedef struct {
  UINT8                 SocketId;
  UINT8                 NbioPhysId;
  UINTN                 PortCount;
  AMD_CXL_MEMORY_POOL  MemPool[MAX_PORTS_PER_CXL_CONTROLLER];
} NBIO_CXL_CONTROLLER;

typedef struct {
  AMD_PCI_LOCATION  PciLocation;
  struct {
    UINT8  SecondaryBus;
    UINT8  SubordinateBus;
    UINT8  RootPortIndex;
    UINT8  Reserved;
  } Bridge;
  AMD_CXL_PORT_INFO_STRUCT  PortInfo;
} CXL_ROOT_BRIDGE;

/**
  Get CXL DeviceID.

  @param[in]   This       Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[out]  DeviceID   CXL DeviceID (1..255).

  @retval EFI_SUCCESS     The function completed successfully.
  @retval Other           The requested operation could not be completed.

**/
EFI_STATUS
EFIAPI
CxlMgrIsCxlDevice (
  IN  CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN  AMD_PCI_LOCATION                EndpointLocation
  );

/**
  Get RootBridge Information.

  @param[in]   This                Pointer to AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]   EndpointLocation    Endpoint Location structure.
  @param[out]  RootBridgeInfo      Pointer to AMD_CXL_ROOT_PORT_INFO structure.

  @retval EFI_SUCCESS              The function completed successfully.
  @retval Other                    The requested operation could not be completed.

**/
EFI_STATUS
EFIAPI
CxlMgrGetRootBridgeInfo (
  IN  AMD_CXL_MANAGER_PROTOCOL   *This,
  IN  AMD_PCI_LOCATION           EndpointLocation,
  OUT AMD_CXL_ROOT_PORT_INFO     *RootBridgeInfo
  );

/**
  Register a CXL memory pool.

  @param[in]       This               Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in, out]  MemPoolToRegister  Pointer to CXL memory pool.

  @retval EFI_SUCCESS     The function completed successfully.
  @retval Other           The requested operation could not be completed.

**/
EFI_STATUS
EFIAPI
CxlMgrRegisterMemoryPool (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN OUT AMD_CXL_MEMORY_POOL          *MemPoolToRegister
  );

/**
  Enable a CXL memory pool.
  By the time this function is called all memory pools should have been registered.

  @param[in]  This              Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]  SlaveAgentIDM     SlaveAgent ID Map.
  @param[in]  EndpointLocation  Endpoint Location structure.
  @param[in]  TrafficClass      Traffic Class for CXL Virtual Channel.
  @param[in]  CxlVendorId      CXL Vendor ID in Protocol Layer DVSEC.
  @param[in]  MemPoolToEnable   Pointer to CXL memory pool.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval Other                 The requested operation could not be completed.

**/
EFI_STATUS
EFIAPI
CxlMgrEnableMemoryPool (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN AMD_PCI_LOCATION                  EndpointLocation,
  IN AMD_CXL_MEMORY_POOL              *MemPool
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
EFI_STATUS
EFIAPI
CxlMgrConvertMemoryPool (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN UINT64                            Base,
  IN UINT64                            Size,
  IN EFI_GCD_MEMORY_TYPE               GcdMemoryType,
  IN UINT64                            Attributes
  );

/**
  Callback for ReadyToBoot event notification from Endpoint driver.

  @param[in]  This           Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.

**/
VOID
EFIAPI
CxlMgrOnReadyToBootFromEndpoint (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This
  );

/**
  PciEnumerationComplete notification event handler.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.

**/
VOID
EFIAPI
CxlMgrOnPciEnumerationComplete (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

/**
  ExitBootServices notification event handler.

  This stage is notified just before the firmware/Preboot environment transfers
  management of all system resources to the OS or next level execution environment.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
CxlMgrOnExitBootServices (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

#endif
