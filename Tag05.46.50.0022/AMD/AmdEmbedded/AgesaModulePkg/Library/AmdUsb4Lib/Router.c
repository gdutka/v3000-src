/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>
#include "Library/AmdUsb4Lib.h"

#define FILECODE LIBRARY_AMDUSB4LIB_ROUTER_FILECODE

BOOLEAN
IsUSB4Router (
  IN     AMD_USB4_CM_ROUTER *pRouter
  )
{
  return ( pRouter->Usb4Version == 0x20 ) ? TRUE : FALSE;
}


EFI_STATUS
RouterOperation2 (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     UINT16                               OpCode,
  IN     UINT32                               OperationMetadata,
  IN     UINT32                               OpSize,
  IN     UINT32                               *OpData,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               *CompletionStatus,
     OUT UINT32                               **Data
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  UINT32                                Data32;
  UINT32                                AndMask;
  UINT32                                OrValue;
  UINT32                                *Ptr32;
  AMD_USB4_ROUTER_CS_26                 RouterCs26;

  Status            = EFI_SUCCESS;
  DataSize          = 0;
  Data32            = 0;
  AndMask           = 0;
  OrValue           = 0;
  Ptr32             = NULL;
  RouterCs26.Value  = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ((pCM == NULL) || (pRouter == NULL)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( CompletionMetadata == NULL || CompletionStatus == NULL ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Input parameters CompletionMetadata or ReturnStatus is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsUSB4Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_INFO ("Router Operations interface is not implemented in a TBT3 Router.\n");
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("OpSize = %d, OpData = 0x%x\n", OpSize, OpData);

  //
  // If the Operation requires data, write the data to ROUTER_CS_9 through ROUTER_CS_24.
  //
  if ( OpSize > 0 && OpSize <= 16 && OpData != NULL ) {
    if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
      for (DataSize = 0; DataSize < OpSize; OpSize++) {
        IDS_HDT_CONSOLE_USB4_DEBUG (" OpData[%d] = 0x%x\n", DataSize, OpData[DataSize]);
      }
    }
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,      // CsType
               0,                     // AdapterNum
               9,                     // Write Offset
               &OpSize,               // WriteDataSize
               OpData                 // WriteData
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to write Router Configuration Space Register.\n");
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // If the Operation requires Metadata, write the metadata to ROUTER_CS_25.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Write Operation Metadata: 0x%x\n", OperationMetadata);
  Data32    = OperationMetadata;
  DataSize  = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             25,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to write Operation Metadata.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Write the Operation Opcode to bits 15:0 of ROUTER_CS_26.
  //
  AndMask = 0xFFFF0000;
  OrValue = (UINT32)OpCode;
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Write Opcode: 0x%x\n", OpCode);
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             26,
             1,
             &AndMask,
             &OrValue
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to write Operation Opcode.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Set the Operation Valid bit in ROUTER_CS_26 to 1b.
  //
  AndMask = 0x00FFFFFF;
  OrValue = 0x80000000;
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Set the Operation Valid bit\n");
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             26,
             1,
             &AndMask,
             &OrValue
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to Set the Operation Valid bit.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Polling the OV bit.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Start to Polling OV bit flip\n");
  Status = AmdUsb4ReadBitTimeout (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             26,
             BIT31,
             0,
             ROUTER_READY_TIMEOUT_US
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Get Polling OV bit flip return status %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("    Failed to get OV bit flip.\n");

    // Clear ROUTER_CS_26 before exit
    IDS_HDT_CONSOLE_USB4_DEBUG ("    Clear ROUTER_CS_26 before exit.\n");
    DataSize  = 1;
    Data32    = 0;
    AmdUsb4Ring0WriteCS (
      pCM,
      pRouter->Header.TopologyID,
      CS_TARGET_ROUTER,
      0,
      26,
      &DataSize,
      &Data32
      );
    return Status;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("   OV bit flip.\n");

  //
  // Read ROUTER_CS_25 and ROUTER_CS_26
  //
  DataSize  = 0;
  Ptr32     = NULL;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,      // CsType
             0,                     // AdapterNum
             25,                    // ReadAddress
             2,                     // ReadSize 2 DWs
             &Ptr32,                // DataOut
             &DataSize              // DataSize
             );
  if ( (EFI_ERROR (Status)) || (2 != DataSize) || (NULL == Ptr32) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ROUTER_CS_25 and ROUTER_CS_26.\n");
    return EFI_DEVICE_ERROR;
  }

  *CompletionMetadata   = *Ptr32;
  RouterCs26.Value      = *(Ptr32+1);
  *CompletionStatus     = RouterCs26.Field.Status;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "CompletionMetaData     = 0x%x\n"
    "CompletionStatus       = 0x%x\n"
    "OperationNotSupported  = 0x%x\n"
    "OperationValid         = 0x%x\n",
    *CompletionMetadata,
    *CompletionStatus,
    RouterCs26.Field.OperationNotSupported,
    RouterCs26.Field.OperationValid
    );

  if ( RouterCs26.Field.OperationNotSupported ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("    Operation Not Supported.\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Read ROUTER_CS_9 to ROUTER_CS_24
  //
  DataSize  = 0;
  Ptr32     = NULL;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,      // CsType
             0,                     // AdapterNum
             9,                     // ReadAddress
             16 ,                   // ReadSize 2 DWs
             &Ptr32,                // DataOut
             &DataSize              // DataSize
             );
  if ( (EFI_ERROR (Status)) || (16 != DataSize) || (NULL == Ptr32) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ROUTER_CS_9 to ROUTER_CS_24.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Data[16] router data to out Data.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Org Data pointer to 0x%lx\n", *Data);
  *Data = Ptr32;
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Mod Data pointer to 0x%lx\n", *Data);

  switch (OpCode) {
    case QUERY_DP_RESOURCE_AVAILABILITY:
    case ALLOCATE_DP_RESOURCE:
    case DEALLOCATE_DP_RESOURCE:
      IDS_HDT_CONSOLE_USB4_DEBUG ("   DisplayPort Adapter Router Operation:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Opcode                    = 0x%x\n", OpCode);
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Completion Metadata: 0x%x\n", *CompletionMetadata);
      IDS_HDT_CONSOLE_USB4_DEBUG ("       DisplayPort Number      = 0x%x\n", *CompletionMetadata);
      break;
    case NVM_WRITE:
      break;
    case NVM_AUTHENTICATE_WRITE:
      break;
    case NVM_READ:
      break;
    case NVM_SET_OFFSET:
      break;
    case DROM_READ:
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Drom Read Operation:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Opcode                  = 0x%x\n", OpCode);
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Drom MetaData           = 0x%x\n", *CompletionMetadata);
      IDS_HDT_CONSOLE_USB4_DEBUG ("       Address               = 0x%x\n", ((*CompletionMetadata & 0x7FFC) >> 2));
      IDS_HDT_CONSOLE_USB4_DEBUG ("       Read Size             = 0x%x\n", ((*CompletionMetadata & 0xF8000) >> 15));
      break;
    case GET_NVM_SECTOR_SIZE:
      break;
    case GET_PCIE_DOWNSTREAM_ENTRY_MAPPING:
      break;
    case GET_CAPABILITIES:
      break;
    case SET_CAPABILITIES:
      break;
    case BUFFER_ALLOCATION_REQUEST:
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Buffer Allocation Request:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Opcode                    = 0x%x\n", OpCode);
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Completion Metadata: %x\n", *CompletionMetadata);
      IDS_HDT_CONSOLE_USB4_DEBUG ("       Length                  = 0x%x\n", (*CompletionMetadata & 0xFF));
      break;
    case BLOCK_SIDEBAND_PORT_OPERATIONS:
      break;
    case UNBLOCK_SIDEBAND_PORT_OPERATIONS:
      break;
    case GET_CONTAINER_ID:
      break;
    default:
      IDS_HDT_CONSOLE_USB4_ERROR ("   Unknown Opcode: 0x%x\n", OpCode);
      break;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}


EFI_STATUS
RouterOperation (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     UINT16                               OpCode,
  IN     UINT32                               OperationMetadata,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               **Data,
     OUT UINT32                               *CompletionStatus
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  AMD_USB4_ROUTER_CONFIGURATION_SPACE   *pRouterCs;

  Status     = EFI_SUCCESS;
  DataSize   = 0;
  pRouterCs  = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ((pCM == NULL) || (pRouter == NULL)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( CompletionMetadata == NULL || CompletionStatus == NULL ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Input parameters CompletionMetadata or ReturnStatus is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read Router Configuration Space in order to read ROUTER_CS_0 through ROUTER_CS_26
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) 27 DWs ---\n", pRouter->Header.TopologyID);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,      // CsType
             0,                     // AdapterNum
             0,                     // ReadAddress
             27,                    // ReadSize 27 DWs
             (UINT32**)&pRouterCs,  // DataOut
             &DataSize              // DataSize
             );

  if ((EFI_ERROR (Status)) || (27 != DataSize) || (NULL == pRouterCs)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to read Router Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) 27 DWs Success ---\n", pRouter->Header.TopologyID);
  //
  // Send Router Operation
  //
  pRouterCs->MetaData = OperationMetadata;
  pRouterCs->RouterCs26.Field.Opcode = OpCode;
  pRouterCs->RouterCs26.Field.OperationValid = 0x1;
  pRouterCs->RouterCs26.Field.OperationNotSupported = 0x0;
  DataSize = 2;
  IDS_HDT_CONSOLE_USB4_DEBUG ("   OperationMetadata: 0x%x, Opcode: 0x%x\n", OperationMetadata, OpCode);

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) 2 DWs ---\n", pRouter->Header.TopologyID);
  Status = AmdUsb4Ring0WriteCS (
                 pCM,
                 pRouter->Header.TopologyID,
                 CS_TARGET_ROUTER,
                 0,
                 25,
                 &DataSize,
                 &(pRouterCs->MetaData)
                 );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to write Router Configuration Space Registers.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) 2 DWs Success ---\n", pRouter->Header.TopologyID);

  //
  // Polling the OV bit.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Start to Polling OV bit flip\n");
  Status = AmdUsb4ReadBitTimeout (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             26,
             BIT31,
             0,
#if AMD_USB4_DEBUG_WAIT_OV_INFINITE_TIMEOUT
             (UINTN)(-1)
#else
             ROUTER_READY_TIMEOUT_US
#endif
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Get Polling OV bit flip return status %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("    Failed to get OV bit flip.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("   OV bit flip.\n");

  //
  // Read Router Configuration Space in order to verify Router Operation
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) 27 DWs again ---\n", pRouter->Header.TopologyID);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,      // CsType
             0,                     // AdapterNum
             0,                     // ReadAddress
             27,                    // ReadSize 27 DWs
             (UINT32**) &pRouterCs, // DataOut
             &DataSize              // DataSize
             );

  if ((EFI_ERROR (Status)) || (27 != DataSize) ) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Failed to read Router Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "--- Read Router (0x%lx) 27 DWs again Success ---\n",
    pRouter->Header.TopologyID
    );

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   OperationNotSupported     = 0x%x\n",
    pRouterCs->RouterCs26.Field.OperationNotSupported
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   OperationValid            = 0x%x\n",
    pRouterCs->RouterCs26.Field.OperationValid
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Status                    = 0x%x\n",
    pRouterCs->RouterCs26.Field.Status
    );

  *CompletionMetadata = pRouterCs->MetaData;
  *CompletionStatus = pRouterCs->RouterCs26.Field.Status;
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   CompletionMetadata = 0x%x, CompletionStatus = 0x%x\n",
    *CompletionMetadata,
    *CompletionStatus
    );

  if ( pRouterCs->RouterCs26.Field.OperationNotSupported ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("    Operation Not Supported.\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Data[16] router data to out Data.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Org Data pointer to 0x%lx\n", *Data);
  *Data = &(pRouterCs->Data[0]);
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Mod Data pointer to 0x%lx\n", *Data);

  switch (OpCode) {
    case QUERY_DP_RESOURCE_AVAILABILITY:
    case ALLOCATE_DP_RESOURCE:
    case DEALLOCATE_DP_RESOURCE:
      IDS_HDT_CONSOLE_USB4_DEBUG ("   DisplayPort Adapter Router Operation:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Opcode                    = 0x%x\n", OpCode);
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Completion Metadata: 0x%x\n", *CompletionMetadata);
      IDS_HDT_CONSOLE_USB4_DEBUG ("       DisplayPort Number      = 0x%x\n", *CompletionMetadata);
      break;
    case NVM_WRITE:
      break;
    case NVM_AUTHENTICATE_WRITE:
      break;
    case NVM_READ:
      break;
    case NVM_SET_OFFSET:
      break;
    case DROM_READ:
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Drom Read Operation:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Opcode                  = 0x%x\n", OpCode);
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Drom MetaData           = 0x%x\n", *CompletionMetadata);
      IDS_HDT_CONSOLE_USB4_DEBUG ("       Address               = 0x%x\n", ((*CompletionMetadata & 0x7FFC) >> 2));
      IDS_HDT_CONSOLE_USB4_DEBUG ("       Read Size             = 0x%x\n", ((*CompletionMetadata & 0xF8000) >> 15));
      break;
    case GET_NVM_SECTOR_SIZE:
      break;
    case GET_PCIE_DOWNSTREAM_ENTRY_MAPPING:
      break;
    case GET_CAPABILITIES:
      break;
    case SET_CAPABILITIES:
      break;
    case BUFFER_ALLOCATION_REQUEST:
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Buffer Allocation Request:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Opcode                    = 0x%x\n", OpCode);
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Completion Metadata: %x\n", *CompletionMetadata);
      IDS_HDT_CONSOLE_USB4_DEBUG ("       Length                  = 0x%x\n", (*CompletionMetadata & 0xFF));
      break;
    case BLOCK_SIDEBAND_PORT_OPERATIONS:
      break;
    case UNBLOCK_SIDEBAND_PORT_OPERATIONS:
      break;
    case GET_CONTAINER_ID:
      break;
    default:
      IDS_HDT_CONSOLE_USB4_ERROR ("   Unknown Opcode: 0x%x\n", OpCode);
      break;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}


EFI_STATUS
RouterProvision (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  EFI_STATUS  Status;
  UINT16      CapabilityPointer;
  UINT64      *CapabilityReg;
  UINT32      DataSize;
  UINT16      NextCapabilityPointer;
  UINT8       CapabilityID;

  AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY  *pRouterTmuCap;

  Status                = EFI_SUCCESS;
  CapabilityPointer     = 0;
  CapabilityReg         = NULL;
  DataSize              = 0;
  NextCapabilityPointer = 0;
  CapabilityID          = 0;
  pRouterTmuCap         = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Entry\n", __FUNCTION__);

  if ( NULL == pRouter ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   pRouter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  CapabilityPointer = (UINT16) pRouter->CapabilityOffset;
  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,                                    // CsType
               0         ,                                          // AdapterNum
               (UINT32) CapabilityPointer,                          // ReadAddress
               2,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 2)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to read capabilities\n");
      return EFI_DEVICE_ERROR;
    }

    NextCapabilityPointer = (UINT16) GETNEXTCAPPTR (*CapabilityReg);
    CapabilityID          = GETCAPID (*CapabilityReg);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   CapabilityID = 0x%x, NextCapabilityPointer = 0x%x\n",
      CapabilityID,
      NextCapabilityPointer
      );
    switch (CapabilityID) {
      case TMU_ROUTER_CONFIGURATION_CAPABILITY_ID:
        pRouter->TMUCapability = (UINT8) CapabilityPointer;
        IDS_HDT_CONSOLE_USB4_DEBUG ("   Found TMU Capability Offset 0x%x\n", pRouter->TMUCapability);

        DataSize = 0;
        Status = AmdUsb4Ring0ReadCS (
                   pCM,
                   pRouter->Header.TopologyID,
                   CS_TARGET_ROUTER,                                    // CsType
                   0,                                                   // AdapterNum
                   (UINT32)(pRouter->TMUCapability),                    // ReadAddress
                   4,                                                   // ReadSize
                   (UINT32**)&pRouterTmuCap,                            // DataOut
                   &DataSize                                            // DataSize
                   );
        if ( EFI_ERROR (Status) || ( 4 != DataSize ) ) {
          IDS_HDT_CONSOLE_USB4_ERROR (
            "   Failed to read Router (0x%lx) 4 DW TMU capability.\n",
            pRouter->Header.TopologyID
            );
          return EFI_DEVICE_ERROR;
        }
        // 16 - Hi-Fi Mode; 1000 - LowRes Mode
        pRouter->TMUInfo.TSPacketInterval   = pRouterTmuCap->TSPacketInterval;
        // 1 - Uni-Directional support; 0 - Uni-Directional do not support
        pRouter->TMUInfo.UCAP               = pRouterTmuCap->TmuRtrCs0.Field.UniDirectionalCapability;
        pRouter->TMUInfo.UniDirectionalMode = 0;
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   pRouter->TMUInfo.TSPacketInterval = %d\n",
          pRouter->TMUInfo.TSPacketInterval
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   pRouter->TMUInfo.UCAP             = %d\n",
          pRouter->TMUInfo.UCAP
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   pRouter->TMUInfo.UniDirectionalMode = %d\n",
          pRouter->TMUInfo.UniDirectionalMode
          );
        break;
      case VENDOR_SPECIFIC_CAPABILITY_ID:
        if ( 0 == GETNEXTCAPPTR (*CapabilityReg)
          && 0 == GETVSCLEN (*CapabilityReg)
          && 6 <= GETVSCID (*CapabilityReg) )
        {
          // It is VSEC
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "   Find Vendor Specific Extended Capability, VSEC ID: %d, Offset: 0x%x, Next Capability Pointer: 0x%x, "
            "VSEC Next Cap Ptr 0x%x, VSEC Length: 0x%x\n",
            GETVSCID (*CapabilityReg),
            CapabilityPointer,
            GETNEXTCAPPTR (*CapabilityReg),
            GETVSECNEXTCAPPTR (*CapabilityReg),
            GETVSECLEN (*CapabilityReg)
            );
          if ( GETVSCID (*CapabilityReg) == 6 ) {
            pRouter->VSECapability = CapabilityPointer;
            IDS_HDT_CONSOLE_USB4_DEBUG (
              "   VSEC 6 Capability is at 0x%x\n",
              pRouter->VSECapability
              );
          }

          if ( GETVSECNEXTCAPPTR (*CapabilityReg) > 0x1FFF ) {
            IDS_HDT_CONSOLE_USB4_WARNING ("VESC Next Cap Ptr is large than 0x1FFF. Quit.\n");
            NextCapabilityPointer = 0;
          } else {
            NextCapabilityPointer = GETVSECNEXTCAPPTR (*CapabilityReg);
          }
        } else {
          // It is VSC
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "   Find Vendor Specific Capability, VSC ID: %d, Offset: 0x%x, Next Capability Pointer: 0x%x\n",
            GETVSCID (*CapabilityReg),
            CapabilityPointer,
            GETNEXTCAPPTR (*CapabilityReg)
            );
          if ( GETVSCID (*CapabilityReg) < MAX_VSC_COUNT ) {
            pRouter->VSCapability[GETVSCID (*CapabilityReg)] = (UINT8) CapabilityPointer;
            IDS_HDT_CONSOLE_USB4_DEBUG (
              "   VSC ID %d Capability is at offset 0x%x\n",
              GETVSCID (*CapabilityReg),
              pRouter->VSCapability[GETVSCID (*CapabilityReg)]
              );
          }
        }
        break;
      default:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   Find Unknown Capability, Capability ID: %d, Offset: 0x%x, Next Capability Pointer: 0x%x.\n",
          CapabilityID,
          CapabilityPointer,
          NextCapabilityPointer
          );
        break;
    }
    CapabilityPointer = NextCapabilityPointer;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Router Enumeration
 *
 * @param[in] *pCM                Point to global structure AMD_USB4_CM
 * @param[in] *pRouter            Point to Router structure need to be enumerated
 *
 * @param[out] *pRouter           Point to Router structure need to be enumerated
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_DEVICE_ERROR       Failed.
 * @retval EFI_INVALID_PARAMETER  Invaild parameter
 * @retval EFI_UNSUPPORTED        Do not support this router
**/

EFI_STATUS
RouterEnumeration (
  IN     AMD_USB4_CM          *pCM,
  IN OUT AMD_USB4_CM_ROUTER   *pRouter
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  AMD_USB4_ROUTER_CONFIGURATION_SPACE   *pRouterCs;

  UINT32                                TimeOut;
  AMD_USB4_ROUTER_CS_6                  *pRouterCs6;
  UINT32                                CompletionMetadata;
  UINT8                                 NumBufferAllocation;
  UINT16                                ParameterIndex;
  UINT32                                *RouterOpData;
  UINT32                                Index;
  UINT32                                CompletionStatus;

  Status              = EFI_SUCCESS;
  DataSize            = 0;
  pRouterCs           = NULL;
  TimeOut             = 0;
  pRouterCs6          = NULL;
  CompletionMetadata  = 0;
  NumBufferAllocation = 0;
  ParameterIndex      = 0;
  RouterOpData        = NULL;
  Index               = 0;
  CompletionStatus    = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pCM || NULL == pRouter ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("pCM or pRouter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read Host Router Configuration Space in order to read ROUTER_CS_0 through ROUTER_CS_8
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) CS 9 DWs ---\n", pRouter->Header.TopologyID);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,      // CsType
             0,                     // AdapterNum
             0,                     // ReadAddress
             ROUTER_CS_BASE_SIZE_DW,// ReadSize 9 DWs
             (UINT32**)&pRouterCs,  // DataOut
             &DataSize              // DataSize
             );

  if (EFI_ERROR (Status) || ROUTER_CS_BASE_SIZE_DW != DataSize || NULL == pRouterCs ) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Failed to read Router Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) CS 9 DWs Done ---\n", pRouter->Header.TopologyID);

  pRouter->Header.AdapterNum  = 0;
  pRouter->VendorId         = pRouterCs->VendorId;
  pRouter->ProductId        = pRouterCs->ProductId;
  pRouter->UUID             = ( ((UINT64)pRouterCs->UUIDHigh)<<32 ) | ( (UINT64)pRouterCs->UUIDLow );
  pRouter->MaxAdapter       = (UINT8)(pRouterCs->RouterCs1.Field.MaxAdapter);
  pRouter->UpstreamAdapter  = (UINT8)(pRouterCs->RouterCs1.Field.UpstreamAdapter);
  pRouter->CapabilityOffset = (UINT8)(pRouterCs->RouterCs1.Field.NextCapabilityPointer);
  pRouter->Revision         = (UINT8)(pRouterCs->RouterCs1.Field.RevisionNumber);
  pRouter->Usb4Version      = (UINT8)(pRouterCs->RouterCs4.Field.Usb4Version);
  pRouter->Enumerated       = 0;
  pRouter->PcieSupport      = 0;
  pRouter->Usb3Support      = 0;
  pRouter->PcieTunnelOn     = 0;
  pRouter->Usb3TunnelOn     = 0;
  pRouter->TBT3Support      = ( pRouterCs->RouterCs6.Field.TBT3NotSupported ) ? 0: 1;
  pRouter->IsHost           = ( pRouter->Depth == 0 ) ? 1 : 0;
  pRouter->InternalHost     = (UINT8)(pRouterCs->RouterCs6.Field.InternalHostControllerImplemented);
  pRouter->InternalHostOn   = (UINT8)(pRouterCs->RouterCs5.Field.InternalHostControllerOn);
  pRouter->pAdapter         = NULL;
  pRouter->DFPAmount        = 0;
  pRouter->DpInAmount       = 0;
  pRouter->DpOutAmount      = 0;
  pRouter->Usb3UpAmount     = 0;
  pRouter->Usb3DownAmount   = 0;
  pRouter->PcieUpAmount     = 0;
  pRouter->PcieDownAmount   = 0;
  pRouter->pAdapterUsb3Up   = NULL;
  pRouter->pAdapterUsb3Down = NULL;
  pRouter->pAdapterPcieUp   = NULL;
  pRouter->pAdapterPcieDown = NULL;
  pRouter->pAdapterDpIn     = NULL;
  pRouter->pAdapterDpOut    = NULL;
  pRouter->pAdapterUFP      = NULL;
  pRouter->pAdapterDFP      = NULL;
  pRouter->TMUCapability    = 0;
  pRouter->DromVersion                    = 0xFFFFFFFF;
  pRouter->NumOfUnusedAdapEntries         = 0;
  pRouter->NumOfDpAdapEntries             = 0;
  pRouter->NumOfTbt3LaneAdapEntries       = 0;
  pRouter->NumOfTbt3UpStreamAdapEntries   = 0;
  pRouter->NumOfTbt3DownStreamAdapEntries = 0;
  pRouter->NumOfUsb3PortMapEntries        = 0;
  pRouter->VSECapability                  = 0;

  ZeroMem ((VOID*)pRouter->VSCapability, sizeof (pRouter->VSCapability));

  SetMem ((VOID*)(&(pRouter->UnusedAdapEntry[0])), sizeof (pRouter->UnusedAdapEntry), 0xFF);
  SetMem ((VOID*)(&(pRouter->DpAdapEntry[0])), sizeof (pRouter->DpAdapEntry), 0xFF);
  SetMem ((VOID*)(&(pRouter->Tbt3LaneAdapEntry[0])), sizeof (pRouter->Tbt3LaneAdapEntry), 0xFF);
  SetMem ((VOID*)(&(pRouter->Tbt3UpStreamAdapEntry[0])), sizeof (pRouter->Tbt3UpStreamAdapEntry), 0xFF);
  SetMem ((VOID*)(&(pRouter->Tbt3DownStreamAdapEntry[0])), sizeof (pRouter->Tbt3DownStreamAdapEntry), 0xFF);
  SetMem ((VOID*)(&(pRouter->Usb3PortMap[0])), sizeof (pRouter->Usb3PortMap), 0xFF);
  SetMem ((VOID*)(&(pRouter->DromTMU)), sizeof (pRouter->DromTMU), 0xFF);

  IDS_HDT_CONSOLE_USB4_DEBUG ("Get Read Response for Router Configuration Space\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  VendorId    = 0x%x\n",     pRouter->VendorId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  ProductId   = 0x%x\n",     pRouter->ProductId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TopologyID  = 0x%lx\n",    pRouter->Header.TopologyID);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TopologyIDValid= 0x%lx\n", pRouterCs->RouterCs3.Field.TopologyIDValid);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  RouterReady    = 0x%lx\n", pRouterCs->RouterCs6.Field.RouterReady);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  ConfigurationReady = 0x%lx\n", pRouterCs->RouterCs6.Field.ConfigurationReady);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxAdapter  = 0x%x\n",     pRouter->MaxAdapter);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  UpstreamAdapter = 0x%x\n", pRouter->UpstreamAdapter);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  NextCapPointer = 0x%x\n",  pRouter->CapabilityOffset);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Revision       = 0x%x\n",  pRouter->Revision);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Usb4Version    = 0x%x\n",  pRouter->Usb4Version);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  InternalHost   = 0x%x\n",  pRouter->InternalHost);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Depth = 0x%x\n",           pRouter->Depth);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  UUID = 0x%llx\n",          pRouter->UUID);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  IsHost = 0x%x\n",          pRouter->IsHost);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TBT3Support = 0x%x\n",     pRouter->TBT3Support);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  PcieTunnelOn = 0x%x\n",    pRouter->PcieTunnelOn);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Usb3TunnelOn = 0x%x\n",    pRouter->Usb3TunnelOn);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  USB4Version = 0x%x\n",     pRouter->Usb4Version);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  InternalHostOn = 0x%x\n",  pRouter->InternalHostOn);

  //
  // Check ROUTER_CS_4.USB4_Version
  //
  if ( IsUSB4Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_INFO ("This Router is a USB4 Router\n");
  } else {
    IDS_HDT_CONSOLE_USB4_INFO ("This Router is a TBT3 Router\n");
    return Tbt3RouterEnumeration (pCM, pRouter, pRouterCs);
  }

  // Call AmdUsb4WriteCS to write below fields.
  //   1) ROUTER_CS_1.Depth = Parameter.Depth
  //   2) ROUTER_CS_[3:2].TopologyID = Parameter.Topology
  //   3) ROUTER_CS_1.UpstreamAdapter = ROUTER_CS_1.UpstreamAdapter
  //      Note: If Depth == 0, UpstreamAdapter should be Host Interface Adapter Number.
  //   4) ROUTER_CS_4.CMUV = 0x10;
  //   5) ROUTER_CS_3.TopologhIDValid(Bit[31]) = 1;
  pRouterCs->RouterCs1.Field.Depth = pRouter->Depth;
  pRouterCs->RouterCs3.Field.TopologyIDHigh = (UINT32)((pRouter->Header.TopologyID)>>32);
  pRouterCs->RouterCs3.Field.TopologyIDValid = 1;
  pRouterCs->TopologyIDLow = (UINT32)(pRouter->Header.TopologyID);
  pRouterCs->RouterCs4.Field.CMUsb4Version = 0x10;
  pRouterCs->RouterCs4.Field.NotificationTimeout = 0xFE;

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) CS 4 DWs ---\n", pRouter->Header.TopologyID);
  DataSize = 4;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             1,
             &DataSize,
             &(pRouterCs->RouterCs1.Value)
             );

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write Host Router Configuration Space Registers.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) CS 4 DWs Success---\n", pRouter->Header.TopologyID);

  // Keep calling AmdUsb4ReadCS to get ROUTER_CS_6.
  IDS_HDT_CONSOLE_USB4_DEBUG ("Check Router Ready bit.\n");
  Status = EFI_TIMEOUT;
  TimeOut = ROUTER_READY_TIMEOUT_US/100;
  do {
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,      // CsType
               0,                     // AdapterNum
               6,                     // ReadAddress
               1,                     // ReadSize 1 DWs
               (UINT32**)&pRouterCs6, // DataOut
               &DataSize              // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Host Router Configuration 6 Space Register.\n");
      Status = EFI_DEVICE_ERROR;
      break;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("pRouterCs6->Value=0x%x.\n", pRouterCs6->Value);
    if ( pRouterCs6->Field.RouterReady ) {
      pRouter->Enumerated = 1;
      Status = EFI_SUCCESS;
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("Wait Router Ready. TimeOut %d\n", TimeOut);
    MicroSecondDelay (100);  // TODO - Decide the minimum delay time
  } while (--TimeOut);

  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Router is not ready.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_INFO ("Router (0x%lx) is Enumerated\n", pRouter->Header.TopologyID);

  // Call AmdUsb4RouterProvision to set Router parameters and settings.
  RouterProvision (pCM, pRouter);

  //
  // Determine Router's preferences for buffer allocation per Path type
  // Loop through returned data to determine number of allocated buffers requested for buffer parameters
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("Process Buffer Allocation Request Router Operation.\n");

  /*
  Status = RouterOperation (
             pCM,
             pRouter,
             BUFFER_ALLOCATION_REQUEST,
             0,
             &CompletionMetadata,
             &RouterOpData,
             &CompletionStatus
             );
  */

  Status = RouterOperation2 (
             pCM,
             pRouter,
             BUFFER_ALLOCATION_REQUEST,
             0,
             0,
             NULL,
             &CompletionMetadata,
             &CompletionStatus,
             &RouterOpData
             );

  if ( EFI_ERROR (Status) || ( CompletionStatus != 0 ) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Router Operation: Buffer Allocation Request failed to execute. Status: %r, ReturnStatus: 0x%x\n",
      Status,
      CompletionStatus
      );
    return EFI_DEVICE_ERROR;
  }

  //
  // Mask out Length[15:0] from the Metadata. Length[15:0] is equal to the number of buffer allocation parameters
  //
  NumBufferAllocation = (UINT8)(CompletionMetadata & 0xFF);
  IDS_HDT_CONSOLE_USB4_DEBUG ("   NumBufferAllocation: 0x%x\n", NumBufferAllocation);

  for (Index = 0; Index < NumBufferAllocation; Index++) {
    ParameterIndex = (UINT16)(RouterOpData[Index] & 0xFFFF);
    switch (ParameterIndex) {
      case BUFFER_ALLOCATION_MAX_USB3:
        pRouter->BufferAllocationMaxUSB3 = (UINT16)(RouterOpData[Index] >> 16);
        break;
      case BUFFER_ALLOCATION_MIN_DP_AUX:
        pRouter->BufferAllocationMinDPAux = (UINT16)(RouterOpData[Index] >> 16);
        break;
      case BUFFER_ALLOCATION_MIN_DP_MAIN:
        pRouter->BufferAllocationMinDPMain = (UINT16)(RouterOpData[Index] >> 16);
        break;
      case BUFFER_ALLOCATION_MAX_PCIE:
        pRouter->BufferAllocationMaxPcie = (UINT16)(RouterOpData[Index] >> 16);
        break;
      case BUFFER_ALLOCATION_MAX_HI:
        if (pRouter->IsHost) {
          pRouter->BufferAllocationMaxHI = (UINT16)(RouterOpData[Index] >> 16);
        } else {
          pRouter->BufferAllocationMaxHI = 0;
        }
        break;
      default:
        IDS_HDT_CONSOLE_USB4_DEBUG ("Unknown Parameter Index: 0x%x\n", ParameterIndex);
        break;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("Router buffer allocation\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("        BufferAllocationMaxUSB3   =  0x%x\n", pRouter->BufferAllocationMaxUSB3);
  IDS_HDT_CONSOLE_USB4_DEBUG ("        BufferAllocationMinDPAux  =  0x%x\n", pRouter->BufferAllocationMinDPAux);
  IDS_HDT_CONSOLE_USB4_DEBUG ("        BufferAllocationMinDPMain =  0x%x\n", pRouter->BufferAllocationMinDPMain);
  IDS_HDT_CONSOLE_USB4_DEBUG ("        BufferAllocationMaxPcie   =  0x%x\n", pRouter->BufferAllocationMaxPcie);
  if (pRouter->IsHost) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("        BufferAllocationMaxHI     =  0x%x\n", pRouter->BufferAllocationMaxHI);
  }

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Host Router Enumeration
 *
 * @param[in] *pCM                Point to global structure AMD_USB4_CM
 * @param[in] *pRouter            Point to Router structure need to be enumerated
 *
 * @param[out] *pRouter           Point to Router structure need to be enumerated
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_DEVICE_ERROR       Failed.
 * @retval EFI_INVALID_PARAMETER  Invaild parameter
**/

EFI_STATUS
HostRouterEnumeration (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pCM || NULL == pRouter ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("pCM or pRouter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  SetMem (pRouter, sizeof (AMD_USB4_CM_CELL), 0);

  pRouter->Header.TopologyID  = GET_TOPOLOGY_ID (0, 0, 0, 0, 0, 0, 0);
  pRouter->Header.CellType    = CELL_TYPE_HOST_ROUTER;
  pRouter->Header.Parent      = NULL;
  pRouter->Depth              = 0;

  return RouterEnumeration (
           pCM,
           pRouter
           );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Device Router Enumeration
 *
 * @param[in] *pCM                Point to global structure AMD_USB4_CM
 * @param[in] *pRouter            Point to Router structure need to be enumerated
 * @param[in] *pDFPLaneAdapter    Point to DFP Lane Adapter, which connect to device router
 *
 * @param[out] *pRouter           Point to Router structure need to be enumerated
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_DEVICE_ERROR       Failed.
 * @retval EFI_INVALID_PARAMETER  Invaild parameter
 * @retval EFI_UNSUPPORTED        Exceed the maximum depth or Router do not support
**/

EFI_STATUS
DeviceRouterEnumeration (
  IN     AMD_USB4_CM              *pCM,
  IN OUT AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_LANE_ADAPTER *pDFPLaneAdapter
  )
{
  EFI_STATUS        Status;
  UINT64            TopologyID;
  AMD_USB4_CM_CELL  *pRouterParent;
  UINT8             *pTopologyID;
  UINT8             Depth;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pCM || NULL == pRouter || NULL == pDFPLaneAdapter ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("pCM or pRouter or pDFPLaneAdapter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  SetMem (pRouter, sizeof (AMD_USB4_CM_CELL), 0);

  pRouterParent = (AMD_USB4_CM_CELL*)pDFPLaneAdapter->Header.Parent;
  TopologyID    = pRouterParent->Header.TopologyID;
  pTopologyID   = (UINT8*)&TopologyID;
  Depth         = pRouterParent->Router.Depth + 1;

  if ( Depth > MAX_ROUTER_DEPTH ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "New Router's depth is %d. CM only support %d layers\n",
      Depth,
      MAX_ROUTER_DEPTH
      );
    return EFI_UNSUPPORTED;
  }

  pTopologyID[pRouterParent->Router.Depth] = pDFPLaneAdapter->Header.AdapterNum;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Device Router TopologyID=0x%lx and Depth=%d\n",
    TopologyID,
    pRouterParent->Router.Depth + 1
    );

  pRouter->Header.TopologyID  = TopologyID;
  // Link child router's parent pointer to DFP Lane Adapter
  pRouter->Header.Parent      = (AMD_USB4_CM_CELL_HEADER*)pDFPLaneAdapter;
  pRouter->Header.CellType    = CELL_TYPE_DEVICE_ROUTER;
  pRouter->Depth              = Depth;

  Status = RouterEnumeration (
             pCM,
             pRouter
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Device Router enumerate failed. Status=%r\n", Status);
  } else {
    // Link Parent Router's Lane Adapter to the new Router
    pDFPLaneAdapter->AdpHeader.pLinkPartner = (AMD_USB4_CM_CELL*)pRouter;
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Lane Adapter (TopologyID: 0x%lx, Adapter Number: %d) link partner is Router(TopologyID: 0x%lx) 0x%x\n",
      pDFPLaneAdapter->Header.TopologyID,
      pDFPLaneAdapter->Header.AdapterNum,
      pRouter->Header.TopologyID,
      pDFPLaneAdapter->AdpHeader.pLinkPartner
      );
  }

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Enable Protocol Tunneling in Router
 *
 * @param[in]  *pCM               Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter           Point to Router CM Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
RouterTunnelSupport (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter
  )
{
  EFI_STATUS             Status;
  UINT32                 DataSize;
  UINT32                 TimeOut;
  AMD_USB4_CM_ROUTER     *pParentRouter;
  AMD_USB4_ROUTER_CS_5   *pRouterCs5;
  AMD_USB4_ROUTER_CS_6   *pRouterCs6;

  Status = EFI_SUCCESS;
  DataSize = 0;
  TimeOut = 0;
  pParentRouter = NULL;
  pRouterCs5 = NULL;
  pRouterCs6 = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if (pRouter->IsHost == 0) {
    //
    // If not Host Router, check Parent Router tunneling support
    //
    pParentRouter = (AMD_USB4_CM_ROUTER *)(pRouter->Header.Parent->Parent);
    if ((pParentRouter->Usb3TunnelOn == 1) && (pRouter->Usb3UpAmount != 0)) {
      pRouter->Usb3Support  = 1;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Device Router USB3 tunneling is supported! \n");
    }
    if ((pParentRouter->PcieTunnelOn == 1) && (pRouter->PcieUpAmount != 0)) {
      pRouter->PcieSupport  = 1;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Device Router PCIe tunneling is supported! \n");
    }
  } else {
    //
    // Host Router shall support USB3 tunneling
    //
    pRouter->Usb3Support  = 1;
    pRouter->Usb3TunnelOn = 1;
    IDS_HDT_CONSOLE_USB4_DEBUG ("Host Router always support USB3 Tunneling! \n");
    //
    // Host Router optionally support PCIe tunneling
    //
    if (pRouter->PcieDownAmount != 0) {
      pRouter->PcieSupport  = 1;
      pRouter->PcieTunnelOn = 1;
      IDS_HDT_CONSOLE_USB4_DEBUG ("This Host Router support PCIe Tunneling! \n");
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG ("WARNING: This Host Router does NOT support PCIe Tunneling! \n");
    }

    // return for Host Router
    return EFI_SUCCESS;
  }

  //
  // May add BandWidth check here
  //

  //
  // Enable Protocol Tunneling for Device Router
  //
  if ( !IsUSB4Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("TBT3 Protocol Tunneling is enabled by default.\n");
    if ( pRouter->PcieSupport ) {
      pRouter->PcieTunnelOn = 1;
    }
    pRouter->Usb3TunnelOn = 0;
    pRouter->Usb3Support  = 0;
    return EFI_SUCCESS;
  }

  if ( pRouter->PcieSupport || pRouter->Usb3Support ) {
    //
    // Read Router_CS_5
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,      // CsType
               0,                     // AdapterNum
               5,                     // ReadAddress
               1,                     // ReadSize 1 DWs
               (UINT32**)&pRouterCs5, // DataOut
               &DataSize              // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Router CS5 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    //
    // Turn On Protocol tunneling
    //
    if (pRouter->Usb3Support) {
      pRouter->Usb3TunnelOn = 1;
      pRouterCs5->Field.USB3TunnelingOn = 1;
      //No need to enable internal HC if USB3 tunnel is on
      //if ( pRouter->InternalHost ) {
      //  pRouterCs5->Field.InternalHostControllerOn = 0;
      //}
    } else {
      // If Usb3 tunneling is not enable, we enable internal xhci host
      // But it still can be disabled if PCIe tunneling is disabled.
      if ( pRouter->InternalHost ) {
        pRouterCs5->Field.InternalHostControllerOn = 1;
      }
    }

    if (pRouter->PcieSupport) {
      pRouter->PcieTunnelOn = 1;
      pRouterCs5->Field.PCIeTunnelingOn = 1;
    } else {
      // Internal Host Controller depends on PCIe tunneling.
      // If PCIe tunneling is disabled, we also disable the internal xhci host.
      if ( pRouterCs5->Field.InternalHostControllerOn )
        pRouterCs5->Field.InternalHostControllerOn = 0;
    }

    pRouterCs5->Field.ConfigurationValid = 1;
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,
               0,
               5,
               &DataSize,
               &(pRouterCs5->Value)
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write Router CS5 Register.\n");
      pRouter->Usb3TunnelOn = 0;
      pRouter->PcieTunnelOn = 0;
      return EFI_DEVICE_ERROR;
    }

    //
    // Record Time Stamp to support some Device Router timing of HPD
    //
    if ( pRouter->DpOutAmount > 0 ) {
      StartTimeout (&(pCM->HPDWaitTimeout), DPOUT_HPD_TIMEOUT);
    }

    //
    // Poll the ROUTER_CS_6.Configuration Ready bit
    //
    Status = EFI_TIMEOUT;
    TimeOut = ROUTER_READY_TIMEOUT_US/100;
    do {
      Status = AmdUsb4Ring0ReadCS (
                 pCM,
                 pRouter->Header.TopologyID,
                 CS_TARGET_ROUTER,
                 0,                     // AdapterNum
                 6,                     // ReadAddress
                 1,                     // ReadSize 1 DWs
                 (UINT32**)&pRouterCs6, // DataOut
                 &DataSize              // DataSize
                 );
      if (EFI_ERROR (Status) || 1 != DataSize ) {
        IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Router CS6 Register.\n");
        pRouter->Usb3TunnelOn = 0;
        pRouter->PcieTunnelOn = 0;
        Status = EFI_DEVICE_ERROR;
        break;
      }

      if ( pRouterCs6->Field.ConfigurationReady ) {
        Status = EFI_SUCCESS;
        break;
      }

      IDS_HDT_CONSOLE_USB4_DEBUG ("Waiting for Router Configuration Ready. TimeOut %d\n", TimeOut);
      MicroSecondDelay (100);  // TODO - Decide the minimum delay time
    } while (--TimeOut);

    if ( Status != EFI_SUCCESS ) {
      pRouter->Usb3TunnelOn = 0;
      pRouter->PcieTunnelOn = 0;
      IDS_HDT_CONSOLE_USB4_ERROR ("Router Configuration is not ready.\n");
      return EFI_DEVICE_ERROR;
    }
  }

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Host Router Initialization
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pRouter        Point to Router structure
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
**/
EFI_STATUS
HostRouterInitialization (
  IN     AMD_USB4_CM          *pCM,
  IN OUT AMD_USB4_CM_ROUTER   *pRouter
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  AGESA_TESTPOINT (TP_HOST_ROUTER_INIT_ROUTER, NULL);

  //
  // 1, Host Router enumeration
  //
  Status = HostRouterEnumeration (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Router Enumeration fail!\n");
    return EFI_DEVICE_ERROR;
  }

  AGESA_TESTPOINT (TP_HOST_ROUTER_INIT_DROM, NULL);

  //
  // 2, DROM reading
  //
  Status = ReadDRom (pCM, pRouter);
  IDS_HDT_CONSOLE_USB4_DEBUG ("Read DROM result Status: %r\n", Status);
  if ( Status != EFI_SUCCESS ) {
    pRouter->DromVersion = 0xFFFFFFFF;
  }

  AGESA_TESTPOINT (TP_HOST_ROUTER_INIT_ADAPTER, NULL);

  //
  // 3, Enumerate Adapters
  //
  Status = Usb4EnumerateAllAdapters (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Adapter Enumeration fail!\n");
    return EFI_DEVICE_ERROR;
  }

  AGESA_TESTPOINT (TP_HOST_ROUTER_INIT_LINK_ADAPTER, NULL);

  //
  // 4, Init internal routing
  //
  Status = Usb4LinkAdapters (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Adapter Enumeration fail!\n");
    return EFI_DEVICE_ERROR;
  }

  AGESA_TESTPOINT (TP_HOST_ROUTER_INIT_TURNON_TUNNEL, NULL);

  //
  // 5, Turn on tunnel in Router CS
  //
  Status = RouterTunnelSupport (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Cannot Turn on Protocol Tunneling!\n");
    return EFI_DEVICE_ERROR;
  }

  AGESA_TESTPOINT (TP_HOST_ROUTER_INIT_TMU, NULL);

  //
  // 6, TMU init
  //
  if (AMD_USB4_DEBUG_TMU_ENABLE && pCM->CmTmuMode) {
    Status = AmdUsb4HostRouterTmuEnable (pCM, pRouter, pCM->CmTmuMode);
    if ( Status != EFI_SUCCESS ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Cannot Initialize TMU on Host Router!\n");
      pCM->CmTmuMode = TMU_MODE_OFF;
    }
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Device Router detection
 *
 * @param[in]  *pCM            Point to global structure AMD_USB4_CM
 * @param[in]  *pAdapter       Point to DFP Lane Adapter
 *
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Input error.
 * @retval EFI_DEVICE_ERROR        HW error.
 * @retval EFI_NOT_FOUND           no Router found.
**/
EFI_STATUS
DetectRouter (
  IN     AMD_USB4_CM                  *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER     *pAdapter
  )
{
  UINT32                                          DataSize;
  EFI_STATUS                                      Status;
  AMD_USB4_ADP_CS4                                *pAdpCs4;
  AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY  *LaneAdapterCapability;
  BOOLEAN                                         RouterDetected;
  AMD_USB4_CM_ROUTER                              *pRouter;
  USB4_TIMEOUT                                    Timer;

  Status        = EFI_SUCCESS;
  pAdpCs4       = NULL;
  DataSize      = 0;
  pRouter       = NULL;

  RouterDetected  = FALSE;
  ZeroMem (&Timer, sizeof(USB4_TIMEOUT));

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pCM || NULL == pAdapter) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pRouter = (AMD_USB4_CM_ROUTER*) pAdapter->Header.Parent;

  RouterDetected = ( pAdapter->LaneAdp_AdapterState != ADAPTER_STATE_CL0 ) ? FALSE : TRUE;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Router[0x%lx].Adapter[%d]: Adapter State = 0x%x, Router Detected = 0x%x, TBT3-Compatible Mode = 0x%x\n",
    pAdapter->Header.TopologyID,
    pAdapter->Header.AdapterNum,
    pAdapter->LaneAdp_AdapterState,
    pAdapter->Usb4Port_RD,
    pAdapter->Usb4Port_TCM
    );

  if ( !RouterDetected ) {
    if ( pRouter->IsHost ) {

      IDS_HDT_CONSOLE_USB4_DEBUG (
        "Router[0x%lx].Adapter[%d]: Overrided Usb4Port_RD = 0x%x, Overrided Usb4Port_TCM = 0x%x\n",
        pAdapter->Header.TopologyID,
        pAdapter->Header.AdapterNum,
        pAdapter->Usb4Port_RD,
        pAdapter->Usb4Port_TCM
      );

      if ( pAdapter->Usb4Port_RD ) {

        // Delay some time for Lane training
        AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_DETECT_ROUTER | 0xF1, NULL);
        StartTimeout (
          &Timer,
          (pAdapter->Usb4Port_TCM) ? pCM->TBT3WaitCL0 : pCM->USB4WaitCL0
          );
        AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_DETECT_ROUTER | 0xF2, NULL);
        do {
          DataSize = sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
          Status = AmdUsb4Ring0ReadCS (
                     pCM,
                     pAdapter->Header.TopologyID,
                     CS_TARGET_ADAPTER,                                   // CsType
                     pAdapter->Header.AdapterNum,                         // AdapterNum
                     pAdapter->LaneCapability,                            // ReadAddress
                     DataSize,                                            // ReadSize
                     (UINT32**)&LaneAdapterCapability,                    // DataOut
                     &DataSize                                            // DataSize
                     );
          if (EFI_ERROR (Status)
            || ((sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) != DataSize))
          {
            IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
            continue;
          }

          pAdapter->LaneAdp_SupportSpeed  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkSpeeds;
          pAdapter->LaneAdp_SupportWidth  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkWidths;
          pAdapter->LaneAdp_TargetSpeed   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkSpeed;
          pAdapter->LaneAdp_TargetWidth   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkWidth;
          pAdapter->LaneAdp_CurrentSpeed  = LaneAdapterCapability->LaneAdpCs1.Field.CurrentLinkSpeed;
          pAdapter->LaneAdp_CurrentWidth  = LaneAdapterCapability->LaneAdpCs1.Field.NegotiatedLinkWidth;
          pAdapter->LaneAdp_LinkDisable   = LaneAdapterCapability->LaneAdpCs1.Field.LinkDisable;
          pAdapter->LaneAdp_LinkBonding   = LaneAdapterCapability->LaneAdpCs1.Field.LaneBonding;
          pAdapter->LaneAdp_AdapterState  = LaneAdapterCapability->LaneAdpCs1.Field.AdapterState;
          IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs0 = 0x%x\n", LaneAdapterCapability->LaneAdpCs0.Value);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs1 = 0x%x\n", LaneAdapterCapability->LaneAdpCs1.Value);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportSpeed = 0x%x\n", pAdapter->LaneAdp_SupportSpeed);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportWidth = 0x%x\n", pAdapter->LaneAdp_SupportWidth);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetSpeed = 0x%x\n", pAdapter->LaneAdp_TargetSpeed);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetWidth = 0x%x\n", pAdapter->LaneAdp_TargetWidth);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentSpeed = 0x%x\n", pAdapter->LaneAdp_CurrentSpeed);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentWidth = 0x%x\n", pAdapter->LaneAdp_CurrentWidth);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkDisable = 0x%x\n", pAdapter->LaneAdp_LinkDisable);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkBonding = 0x%x\n", pAdapter->LaneAdp_LinkBonding);
          IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_AdapterState = 0x%x\n", pAdapter->LaneAdp_AdapterState);
          IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

          if ( pAdapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0 ) {
            break;
          }
        } while (HasItTimedOut (&Timer) == EFI_SUCCESS);

        if ( pAdapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0 ) {
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "send out Router 0x%lx Adapter %d state change to CL0.\n",
            pAdapter->Header.TopologyID,
            pAdapter->Header.AdapterNum
            );
          RouterDetected = TRUE;
        }
      }
    } else {
      // read Lane adapter again
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "send out Router 0x%lx Adapter %d is not in CL0 in previous read, "
        "get Lane Capability (0x%x) again. "
        "pAdapter->PMTimer.OldTimerValue=0x%x pAdapter->PMTimer.EndValue=0x%x\n",
        pAdapter->Header.TopologyID,
        pAdapter->Header.AdapterNum,
        pAdapter->LaneCapability,
        pAdapter->PMTimer.OldTimerValue,
        pAdapter->PMTimer.EndValue
        );

      IDS_HDT_CONSOLE_USB4_INFO (
        "send out Router 0x%lx Adapter %d before wait CL0 PMTimer=0x%x\n",
        pAdapter->Header.TopologyID,
        pAdapter->Header.AdapterNum,
        GetACPITimer ()
        );

      do {
        DataSize = sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
        Status = AmdUsb4Ring0ReadCS (
                   pCM,
                   pAdapter->Header.TopologyID,
                   CS_TARGET_ADAPTER,                                   // CsType
                   pAdapter->Header.AdapterNum,                         // AdapterNum
                   pAdapter->LaneCapability,                            // ReadAddress
                   DataSize,                                            // ReadSize
                   (UINT32**)&LaneAdapterCapability,                    // DataOut
                   &DataSize                                            // DataSize
                   );
        if (EFI_ERROR (Status)
          || ((sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) != DataSize))
        {
          IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
          continue;
        }

        pAdapter->LaneAdp_SupportSpeed  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkSpeeds;
        pAdapter->LaneAdp_SupportWidth  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkWidths;
        pAdapter->LaneAdp_TargetSpeed   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkSpeed;
        pAdapter->LaneAdp_TargetWidth   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkWidth;
        pAdapter->LaneAdp_CurrentSpeed  = LaneAdapterCapability->LaneAdpCs1.Field.CurrentLinkSpeed;
        pAdapter->LaneAdp_CurrentWidth  = LaneAdapterCapability->LaneAdpCs1.Field.NegotiatedLinkWidth;
        pAdapter->LaneAdp_LinkDisable   = LaneAdapterCapability->LaneAdpCs1.Field.LinkDisable;
        pAdapter->LaneAdp_LinkBonding   = LaneAdapterCapability->LaneAdpCs1.Field.LaneBonding;
        pAdapter->LaneAdp_AdapterState  = LaneAdapterCapability->LaneAdpCs1.Field.AdapterState;
        IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs0 = 0x%x\n", LaneAdapterCapability->LaneAdpCs0.Value);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs1 = 0x%x\n", LaneAdapterCapability->LaneAdpCs1.Value);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportSpeed = 0x%x\n", pAdapter->LaneAdp_SupportSpeed);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportWidth = 0x%x\n", pAdapter->LaneAdp_SupportWidth);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetSpeed = 0x%x\n", pAdapter->LaneAdp_TargetSpeed);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetWidth = 0x%x\n", pAdapter->LaneAdp_TargetWidth);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentSpeed = 0x%x\n", pAdapter->LaneAdp_CurrentSpeed);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentWidth = 0x%x\n", pAdapter->LaneAdp_CurrentWidth);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkDisable = 0x%x\n", pAdapter->LaneAdp_LinkDisable);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkBonding = 0x%x\n", pAdapter->LaneAdp_LinkBonding);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_AdapterState = 0x%x\n", pAdapter->LaneAdp_AdapterState);
        IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

        if ( pAdapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0 ) {
          break;
        }

        //MicroSecondDelay (10000);
      } while (HasItTimedOut (&(pAdapter->PMTimer)) == EFI_SUCCESS);

      IDS_HDT_CONSOLE_USB4_DEBUG (
        "send out Router 0x%lx Adapter %d after wait CL0 PMTimer=0x%x\n",
        pAdapter->Header.TopologyID,
        pAdapter->Header.AdapterNum,
        GetACPITimer ()
        );

      if ( pAdapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0 ) {
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "send out Router 0x%lx Adapter %d state change to CL0.\n",
          pAdapter->Header.TopologyID,
          pAdapter->Header.AdapterNum
          );
        RouterDetected = TRUE;
      }
    }
  }

  if ( !RouterDetected ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "No Device Router on Router 0x%lx Adapter %d.\n",
      pAdapter->Header.TopologyID,
      pAdapter->Header.AdapterNum
      );
    return EFI_NOT_FOUND;
  }

  if ( IsUSB4Router ((AMD_USB4_CM_ROUTER*)pAdapter->Header.Parent) ) {
    //
    // Clear ADP_CS_4.lock
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pAdapter->Header.AdapterNum),           // AdapterNum
               4,                                                // ReadAddress
               1,                                                // ReadSize 1 DWs
               (UINT32**)&pAdpCs4,                               // DataOut
               &DataSize                                         // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ADP_CS_4 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    pAdpCs4->Field.Lock = 0;
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pAdapter->Header.AdapterNum),           // AdapterNum
               4,                                                // ReadAddress
               &DataSize,
               &(pAdpCs4->Value)
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write ADP_CS_4 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "The Router (0x%lx) Lane Adapter (%d) is unlocked.\n",
      pAdapter->Header.TopologyID,
      pAdapter->Header.AdapterNum
      );
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Router (0x%lx) is TBT3 router. The Lane Adapter remains unlocked by default.\n",
      pAdapter->Header.TopologyID
      );
  }

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Device Router detection and Initialization
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pParentRouter  Point to Parent Router
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      No DFP found.
**/
EFI_STATUS
DeviceRouterInitialization (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pParentRouter
  )
{
  UINT8                      Index;
  EFI_STATUS                 Status;
  AMD_USB4_CM_LANE_ADAPTER   *pAdapter;
  AMD_USB4_CM_ROUTER         *pRouter;

  Status = EFI_SUCCESS;
  pAdapter = NULL;
  pRouter = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pParentRouter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check Depth limit
  //
  if (pParentRouter->Depth == MAX_ROUTER_DEPTH) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Parent Router already reached Depth limit!\n");
    return EFI_UNSUPPORTED;
  }
  //
  // Find DFP Lane Adapter
  //
  if ((pParentRouter->DFPAmount) == 0) {
    IDS_HDT_CONSOLE_USB4_ERROR ("No DFP found!\n");
    return EFI_NOT_FOUND;
  }

  pAdapter = (AMD_USB4_CM_LANE_ADAPTER*)pParentRouter->pAdapterDFP;

  //
  // Loop for DFP Adapter
  //
  for (
    Index = 0;
    Index < pParentRouter->DFPAmount;
    Index++, pAdapter=(AMD_USB4_CM_LANE_ADAPTER*)pAdapter->AdpHeader.pSiblingAdapter
    )
  {
    //
    // Detect Connected Router
    //
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_DETECT_ROUTER, NULL);
    Status = DetectRouter (pCM, pAdapter);
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("No Router detect on DFP %x!\n", Index);
      continue;
    }

    //
    // Enumerate Device Router
    //
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_ROUTER, NULL);
    pRouter = (AMD_USB4_CM_ROUTER*)&(pCM->PtrCmData[pCM->CmDataIndex]); // Assign cell for new Device Router.
    pCM->CmDataIndex++;
    Status = DeviceRouterEnumeration (pCM, pRouter, pAdapter);
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Device Router Enumeration failed!\n");
      pCM->CmDataIndex--;   // If enumerate new Device Router failed, recycle the cell.
      continue;
    }

    //
    // DROM reading
    //
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_DROM, NULL);
    Status = ReadDRom (pCM, pRouter);
    if ( Status != EFI_SUCCESS ) {
      pRouter->DromVersion = 0xFFFFFFFF;
    }

    //
    // Enumerate All adapters of Device Router
    //
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_ADAPTER, NULL);
    Status = Usb4EnumerateAllAdapters (pCM, pRouter);
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Adapter Enumeration for Device Router failed!\n");
      continue;
    }

    /*
    //
    // Link Management
    //   - we may add parameters in pCM for bonding and speed control
    //   - parameter can come from platform - PCD
    //   - do Speed (Downstream Port Reset) first then do bonding
    //
    Status = AmdUsb4PortLinkManage (pCM, pRouter, TRUE);
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Link Speed configure failed!\n");
    }
    */

    Status = AmdUsb4PortLinkBonding (pCM, pRouter);
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_LINK_BONDING, NULL);
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Link Bonding failed!\n");
    }

    //
    // Init internal routing
    //
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_LINK_ADAPTER, NULL);
    Status = Usb4LinkAdapters (pCM, pRouter);
    if ( Status != EFI_SUCCESS ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Adapter Enumeration fail!\n");
      continue;
    }

    //
    // Turn on tunnel in Router CS
    //
    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_TURNON_TUNNEL, NULL);
    Status = RouterTunnelSupport (pCM, pRouter);
    if ( Status != EFI_SUCCESS ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Turn on Protocol Tunneling!\n");
      continue;
    }

    //
    // Enable TMU
    //
    if (AMD_USB4_DEBUG_TMU_ENABLE && pCM->CmTmuMode) {
      AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT_TMU, NULL);
      Status = AmdUsb4DeviceRouterTmuInit (pCM, pRouter);
      if ( Status != EFI_SUCCESS ) {
        IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Initialize TMU!\n");
        pCM->CmTmuMode = TMU_MODE_OFF;
      }
    }

    //
    // Check for Downstream Routers
    //
    Status = DeviceRouterInitialization (pCM, pRouter);
    if ((Status == EFI_DEVICE_ERROR) || (Status == EFI_INVALID_PARAMETER)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Found when initializing Device Router!\n");
    }
  }

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Check if Host Router has Device Router connected.
 * If there is nothing, set Host Router Topology ID valid to 0.
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pHostRouter    Point to Host Router
 *
**/
VOID
AnyDeviceRouterConnected (
  IN     AMD_USB4_CM            *pCM,
  IN     AMD_USB4_CM_ROUTER     *pHostRouter
  )
{
  EFI_STATUS                Status;
  UINT32                    i;
  AMD_USB4_CM_LANE_ADAPTER  *pLaneAdapter;
  BOOLEAN                   IsDevRouterConncected;
  UINT32                    AndMask;
  UINT32                    OrValue;

  Status                = EFI_SUCCESS;
  i                     = 0;
  pLaneAdapter          = NULL;
  IsDevRouterConncected = FALSE;
  AndMask               = (UINT32)(~BIT31);
  OrValue               = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Enter\n", __FUNCTION__);

  IDS_HDT_CONSOLE_USB4_DEBUG ("Router[0x%lx] DFPAmount is %d\n", pHostRouter->Header.TopologyID, pHostRouter->DFPAmount);
  pLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*) pHostRouter->pAdapterDFP;
  while ( pLaneAdapter != NULL ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "[#%d] Lane Adapter[%d] Link Partner 0x%x\n",
      i,
      pLaneAdapter->Header.AdapterNum,
      pLaneAdapter->AdpHeader.pLinkPartner
      );
    if ( pLaneAdapter->AdpHeader.pLinkPartner != NULL ) {
      IsDevRouterConncected = TRUE;
      break;
    }
    pLaneAdapter = (AMD_USB4_CM_LANE_ADAPTER*) pLaneAdapter->AdpHeader.pSiblingAdapter;
    i++;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("IsDevRouterConncected = %x\n", IsDevRouterConncected);

  if ( !IsDevRouterConncected ) {
    // Set Host Router Topology ID valid to 0
    Status = AmdUsb4Ring0RMWCS (
               pCM,
               pHostRouter->Header.TopologyID,
               CS_TARGET_ROUTER,
               0,
               3,
               1,
               &AndMask,
               &OrValue
               );
    IDS_HDT_CONSOLE_USB4_DEBUG ("Set Host Router Topology ID valid to 0 %r\n", Status);
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Exit\n", __FUNCTION__);
}
