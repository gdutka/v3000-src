/** @file

Cxl Memory Manager Driver.

**/
/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

#include <AMD.h>
#include <Porting.h>
#include <Library/IdsLib.h>

#include "CxlManagerDxe.h"

#define FILECODE NBIO_COMMON_CXLMANAGERDXE_CXLMANAGERDXE_FILECODE

//
// Consumed Protocols
//
STATIC AMD_NBIO_CXL_SERVICES_PROTOCOL   *mCxlNbioProtocol = NULL;
STATIC AMD_APCB_SERVICE_PROTOCOL        *mCxlApcbProtocol = NULL;

//
// Produced Protocols
//
STATIC AMD_CXL_MANAGER_PROTOCOL  mCxlManagerProtocol = {
  AMD_CXL_MANAGER_PROTOCOL_REVISION,
  CxlMgrIsCxlDevice,
  CxlMgrGetRootBridgeInfo,
  CxlMgrRegisterMemoryPool,
  CxlMgrEnableMemoryPool,
  CxlMgrConvertMemoryPool,
  CxlMgrOnReadyToBootFromEndpoint
};

STATIC CONST UINT8            SocketIdList[MAX_NUMBER_OF_SOCKETS] = { 0, 1 };
STATIC CONST UINT8            NbioIdList[MAX_NBIO_INSTANCES_PER_SOCKET] = { 0, 1, 2, 3 };
STATIC NBIO_CXL_CONTROLLER    CxlControllerList[MAX_NUMBER_OF_SOCKETS][MAX_NBIO_INSTANCES_PER_SOCKET] = { { {0} } };

STATIC CXL_ROOT_BRIDGE        mCxlRootBridgeList[MAX_CXL_ROOT_BRIDGES];
STATIC UINTN                  mCxlRootBridgeCount = 0;

STATIC CXL_REGION_DESCRIPTOR  mCxlFromApcbList[MAX_ABL_MEMORY_RANGES] = { 0 };
STATIC CXL_REGION_DESCRIPTOR  mCxlToApcbList[MAX_ABL_MEMORY_RANGES] = { 0 };
STATIC CXL_ADDR_MAP_INFO      mCxlApobList[MAX_ABL_MEMORY_RANGES] = { 0 };

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 *mEfiMemoryTypeName[] = {
  "EfiReservedMemoryType",
  "EfiLoaderCode",
  "EfiLoaderData",
  "EfiBootServicesCode",
  "EfiBootServicesData",
  "EfiRuntimeServicesCode",
  "EfiRuntimeServicesData",
  "EfiConventionalMemory",
  "EfiUnusableMemory",
  "EfiACPIReclaimMemory",
  "EfiACPIMemoryNVS",
  "EfiMemoryMappedIO",
  "EfiMemoryMappedIOPortSpace",
  "EfiPalCode",
  "EfiPersistentMemory",
  "EfiMaxMemoryType"
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 *mGcdMemoryTypeName[] = {
  "EfiGcdMemoryTypeNonExistent",
  "EfiGcdMemoryTypeReserved",
  "EfiGcdMemoryTypeSystemMemory",
  "EfiGcdMemoryTypeMemoryMappedIo",
  "EfiGcdMemoryTypePersistent",
  "EfiGcdMemoryTypeMoreReliable",
  "EfiGcdMemoryTypeMaximum"
};

STATIC
CONST CHAR8 *
CxlMgrGcdMemoryTypeString (
  EFI_GCD_MEMORY_TYPE  GcdMemoryType
  )
{
  if (GcdMemoryType > EfiGcdMemoryTypeMaximum) {
    GcdMemoryType = EfiGcdMemoryTypeMaximum;
  }
  return mGcdMemoryTypeName[(UINTN)GcdMemoryType];
}

STATIC
CONST CHAR8 *
CxlMgrEfiMemoryTypeString (
  EFI_MEMORY_TYPE  EfiMemoryType
  )
{
  if (EfiMemoryType > EfiMaxMemoryType) {
    EfiMemoryType = EfiMaxMemoryType;
  }
  return mEfiMemoryTypeName[(UINTN)EfiMemoryType];
}

STATIC
UINT64
CxlMgrDecodeAlignmentFromPowerOfTwo (
  UINT8  Exponent
  )
{
  // Exponent is expected to be 0..63.
  // E.g., (Exponent = 32) returns SIZE_4GB.
  return 1ULL << (Exponent & 0x3F);
}

STATIC
UINT8
CxlMgrNbioIdToBitmap (
  IN  UINT8  NbioId
  )
{
  return BIT0 << (NbioId & 0x7);
}

STATIC
UINT64
CxlMgrAdjustMemoryPoolSize (
  IN  UINT64  Size,
  IN  UINT64  Alignment
  )
{
  UINT64  AndMask;

  if (Size != 0ULL && Alignment != 0ULL) {
    AndMask = Alignment - 1;
    if ((Size & AndMask) != 0ULL) {
      Size = (Size & ~AndMask) + Alignment;
    }
  }

  return Size;
}

STATIC
VOID
CxlMgrDumpApcbMemoryRange (
  IN CXL_REGION_DESCRIPTOR  *ApcbRange
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "  Size      = 0x%lX\n", ApcbRange->Size);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Alignment = %d (0x%lX)\n",
    ApcbRange->Alignment, CxlMgrDecodeAlignmentFromPowerOfTwo (ApcbRange->Alignment));
  IDS_HDT_CONSOLE (GNB_TRACE, "  Socket    = %d\n", ApcbRange->Socket);
  IDS_HDT_CONSOLE (GNB_TRACE, "  NbioIdMap = 0x%X\n", ApcbRange->PhysNbioMap);
  IDS_HDT_CONSOLE (GNB_TRACE, "  IntlvSize = %d\n", ApcbRange->IntlvSize);
  IDS_HDT_CONSOLE (GNB_TRACE, "  SubIlvMap = 0x%X %X %X %X\n",
    ApcbRange->SubIntlvMap[3], ApcbRange->SubIntlvMap[2], ApcbRange->SubIntlvMap[1], ApcbRange->SubIntlvMap[0]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  SubIlvSize= 0x%X\n", ApcbRange->SubIntlvSize);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Type      = %d\n", ApcbRange->Type);
}

STATIC
VOID
CxlMgrDumpApobMemoryRange (
  IN CXL_ADDR_MAP_INFO  *ApobRange
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "  Base      = 0x%lX\n", ApobRange->Base);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Size      = 0x%lX\n", ApobRange->Size);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Status    = %d\n", ApobRange->Status);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Socket    = %d\n", ApobRange->Socket);
  IDS_HDT_CONSOLE (GNB_TRACE, "  NbioIdMap = 0x%X\n", ApobRange->PhysNbioMap);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Alignment = %d (0x%lX)\n",
    ApobRange->Alignment, CxlMgrDecodeAlignmentFromPowerOfTwo (ApobRange->Alignment));
  IDS_HDT_CONSOLE (GNB_TRACE, "  IntlvSize = %d\n", ApobRange->IntlvSize);
  IDS_HDT_CONSOLE (GNB_TRACE, "  SubIlvMap = 0x%X %X %X %X\n",
    ApobRange->SubIntlvMap[3], ApobRange->SubIntlvMap[2], ApobRange->SubIntlvMap[1], ApobRange->SubIntlvMap[0]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  SubIlvSize= 0x%X\n", ApobRange->SubIntlvSize);
}

STATIC
VOID
CxlMgrDumpCxlMemoryPool (
  IN AMD_CXL_MEMORY_POOL  *MemPool
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "  Base      = 0x%lX\n", MemPool->Base);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Size      = 0x%lX\n", MemPool->Size);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Alignment = 0x%lX\n", MemPool->Alignment);
  IDS_HDT_CONSOLE (GNB_TRACE, "  EfiType   = 0x%X (%a)\n",
    MemPool->EfiType, CxlMgrEfiMemoryTypeString (MemPool->EfiType));
  IDS_HDT_CONSOLE (GNB_TRACE, "  Flags     = 0x%X\n", MemPool->Flags);
  IDS_HDT_CONSOLE (GNB_TRACE, "  RpInfo    = 0x%lX\n", MemPool->RootPortInfo.AsUint64);
}

STATIC
BOOLEAN
CxlMgrCxlControllersAvailableOnSocket (
  IN UINT8  SocketId
  )
{
  UINTN    SocketIndex;
  UINTN    NbioIndex;
  UINTN    NbioCount;
  BOOLEAN  IdMatch;

  for (SocketIndex = 0, IdMatch = FALSE; SocketIndex < MAX_NUMBER_OF_SOCKETS && !IdMatch; ++SocketIndex) {
    if (SocketId == SocketIdList[SocketIndex]) {
      IdMatch = TRUE;
    }
  }
  if (!IdMatch) {
    return FALSE;
  }

  NbioCount = 0;
  for (NbioIndex = 0; NbioIndex < MAX_NBIO_INSTANCES_PER_SOCKET; ++NbioIndex) {
    if (CxlControllerList[SocketIndex][NbioIndex].PortCount != 0) {
      ++NbioCount;
    }
  }

  return (NbioCount < MAX_CXL_CONTROLLERS_PER_SOCKET);
}

STATIC
NBIO_CXL_CONTROLLER *
CxlMgrFindCxlController (
  IN UINT8  SocketId,
  IN UINT8  NbioPhysId
  )
{
  NBIO_CXL_CONTROLLER  *CxlController;
  UINTN    SocketIndex;
  UINTN    NbioIndex;

  CxlController = NULL;
  for (SocketIndex = 0; SocketIndex < MAX_NUMBER_OF_SOCKETS; ++SocketIndex) {
    if (SocketId == SocketIdList[SocketIndex]) {
      for (NbioIndex = 0; NbioIndex < MAX_NBIO_INSTANCES_PER_SOCKET && CxlController == NULL; ++NbioIndex) {
        if (NbioPhysId == NbioIdList[NbioIndex]) {
          CxlController = &CxlControllerList[SocketIndex][NbioIndex];
        }
      }
      return CxlController;
    }
  }

  return NULL;
}

STATIC
AMD_CXL_MEMORY_POOL *
CxlMgrFindMatchingMemoryPoolOnCxlController (
  IN AMD_CXL_MEMORY_POOL  *MemPoolToFind,
  IN NBIO_CXL_CONTROLLER  *CxlController
  )
{
  AMD_CXL_MEMORY_POOL  *MemPoolOnPort;
  UINTN                 Index;

  for (Index = 0; Index < CxlController->PortCount; ++Index) {
    MemPoolOnPort = &CxlController->MemPool[Index];
    if (CompareMem (MemPoolOnPort, MemPoolToFind, sizeof (AMD_CXL_MEMORY_POOL)) == 0) {
      return MemPoolOnPort;
    }
  }
  return NULL;
}

STATIC
CXL_ROOT_BRIDGE *
CxlMgrFindRootBridge (
  IN  AMD_PCI_LOCATION  EndpointLocation
  )
{
  CXL_ROOT_BRIDGE    *RootBridge;
  UINTN              Index;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a - Endpoint:\n", __FUNCTION__);
  IDS_HDT_CONSOLE (GNB_TRACE, " Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
    EndpointLocation.AsBits.Segment, EndpointLocation.AsBits.Bus,
    EndpointLocation.AsBits.Device, EndpointLocation.AsBits.Function);

  for (Index = 0; Index < mCxlRootBridgeCount; ++Index) {
    RootBridge = &mCxlRootBridgeList[Index];
    IDS_HDT_CONSOLE (GNB_TRACE, "%a - Looking at Cxl RootBridge:\n", __FUNCTION__);
    IDS_HDT_CONSOLE (GNB_TRACE, " Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
      RootBridge->PciLocation.AsBits.Segment, RootBridge->PciLocation.AsBits.Bus,
      RootBridge->PciLocation.AsBits.Device, RootBridge->PciLocation.AsBits.Function);

    if (EndpointLocation.AsBits.Segment == RootBridge->PciLocation.AsBits.Segment &&
      EndpointLocation.AsBits.Bus == RootBridge->PciLocation.AsBits.Bus) {
      IDS_HDT_CONSOLE (GNB_TRACE, "%a - Found Cxl RootBridge:\n", __FUNCTION__);
      IDS_HDT_CONSOLE (GNB_TRACE, " Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
        RootBridge->PciLocation.AsBits.Segment, RootBridge->PciLocation.AsBits.Bus,
        RootBridge->PciLocation.AsBits.Device, RootBridge->PciLocation.AsBits.Function);
      return RootBridge;
    }
  }
  return NULL;
}

STATIC
EFI_STATUS
CxlMgrEnableMemoryPoolOnTheHost (
  IN AMD_PCI_LOCATION      EndpointLocation,
  IN AMD_CXL_MEMORY_POOL  *MemPoolToEnable,
  IN UINT64                MemPoolSize
  )
{
  CXL_ROOT_BRIDGE                *RootBridge;
  PCI_ADDR                       EndpointBDF;
  EFI_STATUS                     Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);

  Status = EFI_SUCCESS;
  // Find RootBridge that supports this Endpoint
  RootBridge = CxlMgrFindRootBridge (EndpointLocation);
  if (RootBridge == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Could not find a RootBridge for Endpoint at:\n");
    IDS_HDT_CONSOLE (GNB_TRACE, " Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
      EndpointLocation.AsBits.Segment, EndpointLocation.AsBits.Bus,
      EndpointLocation.AsBits.Device, EndpointLocation.AsBits.Function);
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Configure Root port for this memory pool
  EndpointBDF.Address.Segment = EndpointLocation.AsBits.Segment;
  EndpointBDF.Address.Bus = EndpointLocation.AsBits.Bus;
  EndpointBDF.Address.Device = EndpointLocation.AsBits.Device;
  EndpointBDF.Address.Function = EndpointLocation.AsBits.Function;
  EndpointBDF.Address.Register = 0;
  IDS_HDT_CONSOLE (GNB_TRACE, "EndpointBDF (PCI_ADDR):\n");
  IDS_HDT_CONSOLE (GNB_TRACE, " Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
    EndpointBDF.Address.Segment, EndpointBDF.Address.Bus,
    EndpointBDF.Address.Device, EndpointBDF.Address.Function);

ON_EXIT:
  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT (Status = %r)\n", __FUNCTION__, Status);
  return Status;
}

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
  IN  CONST AMD_CXL_MANAGER_PROTOCOL    *This,
  IN  AMD_PCI_LOCATION                  EndpointLocation
  )
{
  CXL_ROOT_BRIDGE   *RootBridge;

  RootBridge = CxlMgrFindRootBridge (EndpointLocation);
  if (RootBridge == NULL) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

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
  IN  AMD_CXL_MANAGER_PROTOCOL    *This,
  IN  AMD_PCI_LOCATION            EndpointLocation,
  OUT AMD_CXL_ROOT_PORT_INFO      *RootBridgeInfo
  )
{

  CXL_ROOT_BRIDGE              *RootBridge;
  // PCI_ADDR                     EndpointBDF;
  AMD_CXL_PORT_INFO_STRUCT     NbioPortInfo;
  EFI_STATUS                   Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);
  Status = EFI_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "Endpoint Location:\n");
  IDS_HDT_CONSOLE (GNB_TRACE, " Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
    EndpointLocation.AsBits.Segment, EndpointLocation.AsBits.Bus,
    EndpointLocation.AsBits.Device, EndpointLocation.AsBits.Function);

  // Find RootBridge that supports this Endpoint
  RootBridge = CxlMgrFindRootBridge(EndpointLocation);
  if (RootBridge == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Could not find a RootBridge for this Endpoint!\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Get AMD_CXL_PORT_INFO_STRUCT from NBIO Driver
  // EndpointBDF.Address.Segment = EndpointLocation.AsBits.Segment;
  // EndpointBDF.Address.Bus = EndpointLocation.AsBits.Bus;
  // EndpointBDF.Address.Device = EndpointLocation.AsBits.Device;
  // EndpointBDF.Address.Function = EndpointLocation.AsBits.Function;
  // EndpointBDF.Address.Register = 0;
  Status = mCxlNbioProtocol->CxlGetRootPortInformation (
    mCxlNbioProtocol,
    RootBridge->Bridge.RootPortIndex,
    &NbioPortInfo
    );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: CxlGetRootPortInformation() failed: Status = %r\n", Status);
    goto ON_EXIT;
  }
  RootBridgeInfo->AsUint64 = 0ULL;
  // RootBridgeInfo->AsStruct.PciLocation.AsBits.Segment = NbioPortInfo.RootPortBDF.Address.Segment;
  // RootBridgeInfo->AsStruct.PciLocation.AsBits.Bus = NbioPortInfo.RootPortBDF.Address.Bus;
  // RootBridgeInfo->AsStruct.PciLocation.AsBits.Device = NbioPortInfo.RootPortBDF.Address.Device;
  // RootBridgeInfo->AsStruct.PciLocation.AsBits.Function = NbioPortInfo.RootPortBDF.Address.Function;
  RootBridgeInfo->AsStruct.NbioPhysId = NbioPortInfo.PhysicalNbioInstance;
  RootBridgeInfo->AsStruct.SocketId = NbioPortInfo.SocketID;

ON_EXIT:
  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT (Status = %r)\n", __FUNCTION__, Status);
  return Status;
}

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
  )
{
  NBIO_CXL_CONTROLLER  *CxlController;
  UINT8                 SocketId;
  UINT8                 NbioPhysId;
  EFI_STATUS            Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);

  IDS_HDT_CONSOLE (GNB_TRACE, "CXL memory pol on ENTRY:\n");
  CxlMgrDumpCxlMemoryPool (MemPoolToRegister);

  // Validate MemoryPoolToRegister
  if (MemPoolToRegister->Size == 0ULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Invalid memory pool parameters:\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Make sure Alignmnet is compliant with ABL requirements
  if (MemPoolToRegister->Alignment < ABL_MIN_BASE_ADDRESS_ALIGNMENT) {
    MemPoolToRegister->Alignment = ABL_MIN_BASE_ADDRESS_ALIGNMENT;
  }

  // Find CXL controller for this memory pool
  SocketId = MemPoolToRegister->RootPortInfo.AsStruct.SocketId;
  NbioPhysId = MemPoolToRegister->RootPortInfo.AsStruct.NbioPhysId;
  IDS_HDT_CONSOLE (GNB_TRACE, "CXL Controller Instance:\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "  SocketId   = %d\n", SocketId);
  IDS_HDT_CONSOLE (GNB_TRACE, "  NbioPhysId = %d\n", NbioPhysId);
  CxlController = CxlMgrFindCxlController (SocketId, NbioPhysId);
  if (CxlController == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Unable to find a matching CXL Controller.\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Allocating a new Controller?
  if (CxlController->PortCount == 0) {
    if (!CxlMgrCxlControllersAvailableOnSocket (SocketId)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Error: No CXL Controller available on this socket.\n");
      Status = EFI_UNSUPPORTED;
      goto ON_EXIT;
    }
    CxlController->SocketId = SocketId;
    CxlController->NbioPhysId = NbioPhysId;
  }

  // Ports available on this Controller?
  if (CxlController->PortCount == MAX_PORTS_PER_CXL_CONTROLLER) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: No Port available on CXL controller.\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Mark memory pool as Registered
  MemPoolToRegister->Flags = AMD_CXL_MEMORY_BDS_REGISTERED;

  // Allocate Port for this memory pool
  CopyMem (&CxlController->MemPool[CxlController->PortCount], MemPoolToRegister, sizeof (AMD_CXL_MEMORY_POOL));
  ++CxlController->PortCount;
  Status = EFI_SUCCESS;

ON_EXIT:
  IDS_HDT_CONSOLE (GNB_TRACE, "CXL memory pool on EXIT:\n");
  CxlMgrDumpCxlMemoryPool (MemPoolToRegister);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT (Status = %r)\n", __FUNCTION__, Status);
  return Status;
}

/**
  Enable a CXL memory pool.
  By the time this function is called all memory pools should have been registered.

  @param[in]  This              Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]  EndpointLocation  Endpoint Location structure.
  @param[in]  MemPoolToEnable   Pointer to CXL memory pool.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval Other                 The requested operation could not be completed.

**/
EFI_STATUS
EFIAPI
CxlMgrEnableMemoryPool (
  IN CONST AMD_CXL_MANAGER_PROTOCOL    *This,
  IN AMD_PCI_LOCATION                  EndpointLocation,
  IN AMD_CXL_MEMORY_POOL               *MemPoolToEnable
  )
{
  NBIO_CXL_CONTROLLER   *CxlController1;
  AMD_CXL_MEMORY_POOL   *MemPoolOnPort;
  UINT64                MemPoolSize;
  UINT64                ApobAlignment;
  CXL_ADDR_MAP_INFO     *ApobRange;
  UINT64                ApobBase;
  UINT64                ApobSize;
  UINT64                ApobOffset;
  BOOLEAN               MemPoolMatchInApob;
  UINT8                 SocketId;
  UINT8                 NbioPhysId;
  UINT8                 NbioPhysIdMap;
  UINTN                 Index;
  EFI_STATUS            Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);

  IDS_HDT_CONSOLE (GNB_TRACE, "CXL memory pool on ENTRY:\n");
  CxlMgrDumpCxlMemoryPool (MemPoolToEnable);

  // Validate MemoryPoolToEnable
  if (MemPoolToEnable->Size == 0ULL ||
    MemPoolToEnable->Flags != AMD_CXL_MEMORY_BDS_REGISTERED) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Invalid memory pool parameters.\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Find CXL controller for this memory pool
  SocketId = MemPoolToEnable->RootPortInfo.AsStruct.SocketId;
  NbioPhysId = MemPoolToEnable->RootPortInfo.AsStruct.NbioPhysId;
  IDS_HDT_CONSOLE (GNB_TRACE, "CXL Controller Instance:\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "  SocketId   = %d\n", SocketId);
  IDS_HDT_CONSOLE (GNB_TRACE, "  NbioPhysId = %d\n", NbioPhysId);
  CxlController1 = CxlMgrFindCxlController (SocketId, NbioPhysId);

  if (CxlController1 == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Unable to find a matching CXL Controller.\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Find Registered memory pool for this CXL controller
  MemPoolOnPort = CxlMgrFindMatchingMemoryPoolOnCxlController (MemPoolToEnable, CxlController1);
  if (MemPoolOnPort == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Unable to find a matching memory pool already registered.\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }
  NbioPhysIdMap = CxlMgrNbioIdToBitmap (CxlController1->NbioPhysId);
  IDS_HDT_CONSOLE (GNB_TRACE, "  NbioPhysIdMap        = 0x%X\n", NbioPhysIdMap);

  // Map MemoryPoolToEnable from APOB memory range
  MemPoolMatchInApob = FALSE;
  for (Index = 0; Index < MAX_ABL_MEMORY_RANGES && !MemPoolMatchInApob; Index++) {
    ApobRange = &mCxlApobList[Index];
    ApobAlignment = CxlMgrDecodeAlignmentFromPowerOfTwo (ApobRange->Alignment);
    ApobBase = ApobRange->Base;
    ApobSize = ApobRange->Size;

    IDS_HDT_CONSOLE (GNB_TRACE, "Trying to match memory range in APOB[%d]:\n", Index);
    CxlMgrDumpApobMemoryRange (ApobRange);

    if ((ApobRange->Status == CXL_ADDR_SUCCESS) &&
      (ApobRange->Base != ABL_INVALID_ADDRESS) &&
      (ApobRange->Socket == SocketId) &&
      (ApobRange->Size >= MemPoolToEnable->Size) &&
      (ApobAlignment >= MemPoolToEnable->Alignment)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Found matching memory range in APOB!\n");
      MemPoolMatchInApob = TRUE;

      // Map from the bottom and adjust the base of the APOB range
      MemPoolToEnable->Base = ApobRange->Base;
      ApobOffset = CxlMgrAdjustMemoryPoolSize (MemPoolToEnable->Size, MemPoolToEnable->Alignment);
      if (ApobRange->Size >= ApobOffset) {
        ApobRange->Base += ApobOffset;
        ApobRange->Size -= ApobOffset;
      }
      else {
        ApobRange->Size = 0ULL;
      }
      IDS_HDT_CONSOLE (GNB_TRACE, " APOB Base = 0x%lX -> 0x%lX\n", ApobBase, ApobRange->Base);
      IDS_HDT_CONSOLE (GNB_TRACE, " APOB Size = 0x%lX -> 0x%lX\n", ApobSize, ApobRange->Size);
    }
  }
  if (!MemPoolMatchInApob) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Unable to find matching memory range in APOB.\n");
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  // Configure Root port for this memory pool
  MemPoolSize = MemPoolOnPort->Size;
  Status = CxlMgrEnableMemoryPoolOnTheHost (
    EndpointLocation,
    MemPoolToEnable,
    MemPoolSize
    );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: Unable to configure Cxl Root port.\n");
    goto ON_EXIT;
  }

  // Mark memory pool as Mapped
  MemPoolToEnable->Flags |= AMD_CXL_MEMORY_ABL_MAPPED;
  MemPoolOnPort->Flags = MemPoolToEnable->Flags;
  MemPoolOnPort->Base = MemPoolToEnable->Base;

ON_EXIT:
  IDS_HDT_CONSOLE (GNB_TRACE, "CXL memory pool on EXIT:\n");
  CxlMgrDumpCxlMemoryPool (MemPoolToEnable);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT (Status = %r)\n", __FUNCTION__, Status);
  return Status;
}

/**
  Convert a CXL memory pool.

  @param[in]  This           Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.
  @param[in]  Base           Base address of CXL memory pool.
  @param[in]  Size           Size of CXL memory pool.
  @param[in]  GcdMemoryType  GCD Memory Type for CXL memory pool.
  @param[in]  Attributes     Attribbutes for CXL memory pool.

  @retval EFI_SUCCESS     The function completed successfully.
  @retval Other           The requested operation could not be completed.

**/
EFI_STATUS
EFIAPI
CxlMgrConvertMemoryPool (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This,
  IN UINT64                            Base,
  IN UINT64                            Size,
  IN EFI_GCD_MEMORY_TYPE               GcdMemoryType,
  IN UINT64                            Attributes
  )
{
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR  GcdDescriptor;
  EFI_STATUS                       Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Pool: 0x%lX - 0x%lX\n", Base, Base + Size - 1);
  IDS_HDT_CONSOLE (GNB_TRACE, "Requested: GcdMemoryType = %d (%a), Attributes = 0x%lX\n",
    GcdMemoryType,
    CxlMgrGcdMemoryTypeString (GcdMemoryType),
    Attributes);

  // Get current GCD parameters
  Status = gDS->GetMemorySpaceDescriptor (Base, &GcdDescriptor);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: gDS->GetMemorySpaceDescriptor () failed! \n");
    goto ON_EXIT;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Current: GcdMemoryType = %d (%a), Attributes = 0x%lX\n",
    GcdDescriptor.GcdMemoryType,
    CxlMgrGcdMemoryTypeString (GcdDescriptor.GcdMemoryType),
    GcdDescriptor.Attributes);

  // Need to convert Type?
  if (GcdMemoryType != GcdDescriptor.GcdMemoryType) {
    // GCD Type convertion is a two-step process:
    //  Step1: Change to EfiGcdMemoryTypeNonExistent via gDS->RemoveMemorySpace()
    //  Step2: Change to desired GcdMemoryType and Attributes via gDS->AddMemorySpace()
    if (GcdDescriptor.GcdMemoryType != EfiGcdMemoryTypeNonExistent) {
      Status = gDS->RemoveMemorySpace (Base, Size);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Error: gDS->RemoveMemorySpace () failed! \n");
        goto ON_EXIT;
      }
    }
    if (GcdMemoryType != EfiGcdMemoryTypeNonExistent) {
      Status = gDS->AddMemorySpace (GcdMemoryType, Base, Size, Attributes);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Error: gDS->AddMemorySpace () failed! \n");
        goto ON_EXIT;
      }
    }

    // Get converted GCD parameters
    Status = gDS->GetMemorySpaceDescriptor (Base, &GcdDescriptor);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Error: gDS->GetMemorySpaceDescriptor () failed! \n");
      goto ON_EXIT;
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "After GcdMemoryType convertion: GcdMemoryType = %d (%a), Attributes = 0x%lX\n",
      GcdDescriptor.GcdMemoryType,
      CxlMgrGcdMemoryTypeString (GcdDescriptor.GcdMemoryType),
      GcdDescriptor.Attributes);
  }

  // Need to change Attributes?
  if (Attributes != GcdDescriptor.Attributes && GcdMemoryType != EfiGcdMemoryTypeNonExistent) {
    Status = gDS->SetMemorySpaceAttributes (Base, Size, Attributes);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Error: gDS->SetMemorySpaceAttributes () failed! \n");
      goto ON_EXIT;
    }

    // Get converted GCD parameters
    Status = gDS->GetMemorySpaceDescriptor (Base, &GcdDescriptor);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Error: gDS->GetMemorySpaceDescriptor () failed! \n");
      goto ON_EXIT;
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "After Attributes convertion: GcdMemoryType = %d (%a), Attributes = 0x%lX\n",
      GcdDescriptor.GcdMemoryType,
      CxlMgrGcdMemoryTypeString (GcdDescriptor.GcdMemoryType),
      GcdDescriptor.Attributes);
  }

ON_EXIT:
  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT (Status = %r)\n", __FUNCTION__, Status);
  return Status;
}

STATIC
BOOLEAN
CxlMgrFindCompatibleMemoryRangeOnApcbList (
  IN CXL_REGION_DESCRIPTOR  *RangeToFind,
  IN CXL_REGION_DESCRIPTOR  *ApcbList
  )
{
  CXL_REGION_DESCRIPTOR  *RangeOnList;
  UINTN                    Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - Comparing:\n", __FUNCTION__);
  CxlMgrDumpApcbMemoryRange (RangeToFind);
  for (Index = 0; Index < MAX_ABL_MEMORY_RANGES; ++Index) {
    RangeOnList = &ApcbList[Index];
    IDS_HDT_CONSOLE (GNB_TRACE, "To Index[%d]:\n", Index);
    CxlMgrDumpApcbMemoryRange (RangeOnList);
    if ((RangeOnList->Size >= RangeToFind->Size) &&
      (RangeOnList->Alignment >= RangeToFind->Alignment) &&
      (RangeOnList->Socket == RangeToFind->Socket) &&
      (RangeOnList->PhysNbioMap == RangeToFind->PhysNbioMap) &&
      (RangeOnList->IntlvSize == RangeToFind->IntlvSize) &&
      (RangeOnList->SubIntlvMap[0] == RangeToFind->SubIntlvMap[0]) &&
      (RangeOnList->SubIntlvMap[1] == RangeToFind->SubIntlvMap[1]) &&
      (RangeOnList->SubIntlvMap[2] == RangeToFind->SubIntlvMap[2]) &&
      (RangeOnList->SubIntlvMap[3] == RangeToFind->SubIntlvMap[3]) &&
      (RangeOnList->SubIntlvSize == RangeToFind->SubIntlvSize)) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Match found!\n");
      return TRUE;
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "No match found!\n");
  return FALSE;
}

STATIC
VOID
CxlMgrProcessMemoryRanges (
  VOID
  )
{
  NBIO_CXL_CONTROLLER     *CxlController;
  CXL_REGION_DESCRIPTOR   *ApcbRange;
  CXL_ADDR_MAP_INFO       *ApobRange;
  UINTN                    ApobIndex;
  UINTN                    ApcbIndex;
  UINTN                    SocketIndex;
  UINTN                    NbioIndex;
  UINTN                    PortIndex;
  EFI_STATUS               Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);

  // Parse APOB entries
  for (ApobIndex = 0; ApobIndex < MAX_ABL_MEMORY_RANGES; ApobIndex++) {
    ApobRange = &mCxlApobList[ApobIndex];
    if (ApobRange->Size != 0ULL) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Warning: APOB Entry[%d] was not claimed by a CXL device:\n", ApobIndex);
      CxlMgrDumpApobMemoryRange (ApobRange);
    }
  }

  // Parse Registered memory pools to create APCB descriptors
  ApcbIndex = 0;
  for (SocketIndex = 0; SocketIndex < MAX_NUMBER_OF_SOCKETS && ApcbIndex < MAX_ABL_MEMORY_RANGES; ++SocketIndex) {
    for (NbioIndex = 0; NbioIndex < MAX_NBIO_INSTANCES_PER_SOCKET && ApcbIndex < MAX_ABL_MEMORY_RANGES; ++NbioIndex) {
      CxlController = &CxlControllerList[SocketIndex][NbioIndex];
      IDS_HDT_CONSOLE (GNB_TRACE, "CXL Controller1:\n");
      IDS_HDT_CONSOLE (GNB_TRACE, "  SocketId   = %d\n", CxlController->SocketId);
      IDS_HDT_CONSOLE (GNB_TRACE, "  NbioPhysId = %d\n", CxlController->NbioPhysId);
      IDS_HDT_CONSOLE (GNB_TRACE, "  PortCount  = %d\n", CxlController->PortCount);
      for (PortIndex = 0; PortIndex < CxlController->PortCount; PortIndex++) {
        // Configure APCB descriptor
        ApcbRange = &mCxlToApcbList[ApcbIndex];
        ApcbRange->Size = CxlController->MemPool[PortIndex].Size;
        ApcbRange->Alignment = CXL_MIN_POWER_OF_TWO_ALIGNMENT;
        ApcbRange->Socket = CxlController->SocketId;
        ApcbRange->PhysNbioMap = CxlMgrNbioIdToBitmap (CxlController->NbioPhysId);
        if (PortIndex != 0) {
          ApcbRange->SubIntlvMap[PortIndex - 1] = 1;
        }
        ApcbRange->IntlvSize = (UINT8)DF_MEM_INTLV_SIZE_AUTO;
        ApcbRange->SubIntlvSize = (UINT8)DF_MEM_INTLV_SIZE_AUTO;
        IDS_HDT_CONSOLE (GNB_TRACE, "APCB Entry[%d]:\n", ApcbIndex);
        CxlMgrDumpApcbMemoryRange (ApcbRange);
        // Point to next APCB entry
        ++ApcbIndex;
      }
      CxlController->PortCount = 0;
    }
  }

  // Dump APCB entries
  IDS_HDT_CONSOLE (GNB_TRACE, "Previous APCB descriptors:\n");
  for (ApcbIndex = 0; ApcbIndex < MAX_ABL_MEMORY_RANGES; ApcbIndex++) {
    ApcbRange = &mCxlFromApcbList[ApcbIndex];
    IDS_HDT_CONSOLE (GNB_TRACE, " Entry[%d]:\n", ApcbIndex);
    CxlMgrDumpApcbMemoryRange (ApcbRange);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Current APCB descriptors:\n");
  for (ApcbIndex = 0; ApcbIndex < MAX_ABL_MEMORY_RANGES; ApcbIndex++) {
    ApcbRange = &mCxlToApcbList[ApcbIndex];
    IDS_HDT_CONSOLE (GNB_TRACE, " Entry[%d]:\n", ApcbIndex);
    CxlMgrDumpApcbMemoryRange (ApcbRange);
  }

  // Compare Current to Previous APCB entries
  if (CompareMem (mCxlToApcbList, mCxlFromApcbList, sizeof (mCxlToApcbList)) != 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Updating current APCB descriptors: ApcbSetType() ");
    Status = mCxlApcbProtocol->ApcbSetType (
      mCxlApcbProtocol,
      APCB_TYPE_PURPOSE_NORMAL,
      APCB_GROUP_DF,
      APCB_DF_TYPE_CXL_CONFIG,
      0,
      (UINT8 *)&mCxlToApcbList[0],
      sizeof (mCxlToApcbList)
      );
    IDS_HDT_CONSOLE (GNB_TRACE, "(Status = %r)\n", Status);

    if (!EFI_ERROR (Status)) {
      // Flush APCB data to SPI ROM
      IDS_HDT_CONSOLE (GNB_TRACE, "Flushing APCB descriptors to SPI ROM: ApcbFlushData() ");
      Status = mCxlApcbProtocol->ApcbFlushData (mCxlApcbProtocol);
      IDS_HDT_CONSOLE (GNB_TRACE, "(Status = %r)\n", Status);

      if (!EFI_ERROR (Status)) {
        // Trigger Warm-reset if previosu APCB entries do not cover current needs
        for (ApcbIndex = 0; ApcbIndex < MAX_ABL_MEMORY_RANGES; ApcbIndex++) {
          ApcbRange = &mCxlToApcbList[ApcbIndex];
          if (ApcbRange->Size != 0UL && !CxlMgrFindCompatibleMemoryRangeOnApcbList (ApcbRange, mCxlFromApcbList)) {
            IDS_HDT_CONSOLE (GNB_TRACE, "Triggering Warm-Reset to map APCB descriptors!\n");
            gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
          }
        }
      }
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT\n", __FUNCTION__);
}

/**
  Callback for ReadyToBoot event notification from Endpoint driver.

  @param[in]  This           Pointer to the AMD_CXL_MANAGER_PROTOCOL instance.

**/
VOID
EFIAPI
CxlMgrOnReadyToBootFromEndpoint (
  IN CONST AMD_CXL_MANAGER_PROTOCOL  *This
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);

  //
  // Perform backend processing
  //
  CxlMgrProcessMemoryRanges ();

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT\n", __FUNCTION__);
}


/**
  The Cxl Manager entry point.

  @param  ImageHandle    The firmware allocated handle for the EFI image.
  @param  SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS    The entry point is executed successfully.
  @retval other          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CxlManagerEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  APOB_SYSTEM_CXL_INFO_TYPE_STRUCT    *ApobCxlInfo;
  EFI_HANDLE                          Handle;
  UINT8                               ApcbPurpose;
  UINT8                               *ApcbDataBuffer;
  UINT32                              ApcbDataSize;
  UINTN                               Entries;
  UINT8                               Index;
  EFI_STATUS                          Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a - ENTRY\n", __FUNCTION__);

  // Locate CxlNbio protocol
  Status = gBS->LocateProtocol (
    &gAmdNbioCxlServicesProtocolGuid,
    NULL,
    (VOID **)&mCxlNbioProtocol
    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: gBS->LocateProtocol (gAmdNbioCxlServicesProtocolGuid) failed! \n");
    goto ON_EXIT;
  }

  // Locate CxlApcb protocol
  Status = gBS->LocateProtocol (
    &gAmdApcbDxeServiceProtocolGuid,
    NULL,
    (VOID **)&mCxlApcbProtocol
    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: gBS->LocateProtocol(gAmdApcbDxeServiceProtocolGuid) failed! \n");
    goto ON_EXIT;
  }

  // Get Cxl entries from APCB
  IDS_HDT_CONSOLE (GNB_TRACE, "Reading CXL_REGION_DESCRIPTOR entries from APCB: ApcbGetType()");
  ApcbPurpose = APCB_TYPE_PURPOSE_NORMAL;
  ApcbDataBuffer = NULL;
  ApcbDataSize = 0;
  Status = mCxlApcbProtocol->ApcbGetType (
    mCxlApcbProtocol,
    &ApcbPurpose,
    APCB_GROUP_DF,
    APCB_DF_TYPE_CXL_CONFIG,
    0,
    &ApcbDataBuffer,
    &ApcbDataSize
    );
  IDS_HDT_CONSOLE (GNB_TRACE, " - Status = %r\n", Status);
  if (!EFI_ERROR (Status)) {
    Entries = ApcbDataSize / sizeof (CXL_REGION_DESCRIPTOR);
    if (Entries == 0 || ApcbDataBuffer == NULL) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Warning: No CXL_REGION_DESCRIPTOR entries found in APCB!\n");
    }
    else if (Entries > MAX_ABL_MEMORY_RANGES) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Warning: Too many CXL_REGION_DESCRIPTOR entries (%d) found in APCB!\n", Entries);
    }
    else {
      for (Index = 0; Index < Entries; ++Index) {
        IDS_HDT_CONSOLE (GNB_TRACE, "APCB Range[%d]:\n", Index);
        CopyMem (&mCxlFromApcbList[Index], ApcbDataBuffer, sizeof (CXL_REGION_DESCRIPTOR));
        CxlMgrDumpApcbMemoryRange (&mCxlFromApcbList[Index]);
        ApcbDataBuffer += sizeof (CXL_REGION_DESCRIPTOR);
      }
    }
  }

  // Get Cxl entries from APOB
  IDS_HDT_CONSOLE (GNB_TRACE, "Reading CXL_ADDR_MAP_INFO entries from APOB: AmdPspGetApobEntryInstance()");
  Status = AmdPspGetApobEntryInstance (
    APOB_FABRIC,
    APOB_SYS_CXL_INFO_TYPE,
    0, FALSE,
    (APOB_TYPE_HEADER **)&ApobCxlInfo
    );
  IDS_HDT_CONSOLE (GNB_TRACE, " - Status = %r\n", Status);
  if (!EFI_ERROR (Status)) {
    Entries = sizeof (ApobCxlInfo->CxlInfo) / sizeof (CXL_ADDR_MAP_INFO);
    if (Entries > MAX_ABL_MEMORY_RANGES) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Warning: Too many CXL_ADDR_MAP_INFO entries (%d) in APOB! \n", Entries);
    }
    else {
      for (Index = 0; Index < Entries; ++Index) {
        IDS_HDT_CONSOLE (GNB_TRACE, "APOB Range[%d]:\n", Index);
        CopyMem (&mCxlApobList[Index], &ApobCxlInfo->CxlInfo[Index], sizeof (CXL_ADDR_MAP_INFO));
        CxlMgrDumpApobMemoryRange (&mCxlApobList[Index]);
      }
    }
  }

  // Get Cxl entries from AGESA
  IDS_HDT_CONSOLE (GNB_TRACE, "Reading CXL device list from AGESA: mCxlNbioProtocol()");
  for (Index = 0; Index < MAX_CXL_ROOT_BRIDGES; Index++) {
    Status = mCxlNbioProtocol->CxlGetRootPortInformation (
      mCxlNbioProtocol,
      Index,
      &mCxlRootBridgeList[Index].PortInfo
      );
      if (Status != EFI_SUCCESS) {
        break;
      }
    mCxlRootBridgeCount++;
    mCxlRootBridgeList[Index].Bridge.RootPortIndex = Index;
    mCxlRootBridgeList[Index].PciLocation.AsBits.Segment = mCxlRootBridgeList[Index].PortInfo.EndPointBDF.Address.Segment;
    mCxlRootBridgeList[Index].PciLocation.AsBits.Bus = mCxlRootBridgeList[Index].PortInfo.EndPointBDF.Address.Bus;
    mCxlRootBridgeList[Index].PciLocation.AsBits.Device = mCxlRootBridgeList[Index].PortInfo.EndPointBDF.Address.Device;
    mCxlRootBridgeList[Index].PciLocation.AsBits.Function = mCxlRootBridgeList[Index].PortInfo.EndPointBDF.Address.Function;
    IDS_HDT_CONSOLE (GNB_TRACE, "Found CXL device at 0x%x", mCxlRootBridgeList[Index].PortInfo.EndPointBDF.AddressValue);
  }

  // Install CxlManager Protocol
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdCxlManagerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mCxlManagerProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Error: gBS->InstallProtocolInterface (gAmdCxlManagerProtocolGuid) failed! \n");
  }

ON_EXIT:
  IDS_HDT_CONSOLE (GNB_TRACE, "%a - EXIT (Status = %r)\n", __FUNCTION__, Status);
  return Status;
}
